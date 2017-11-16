//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "../ASTIncludes.h"

#include "../../../Util.h"
#include "../../SymbolTable.h"

#include "Record/Class.h"
#include "Record/Union.h"
#include "Record/Enum.h"

#include "../../../Variant/Type/BuiltinType.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/AutoType.h"

#include "../../../Message/Exceptions.h"
#include "../../../Message/Diagnostics.h"
#include "../../../Parser.h"
#include "../Declaration/DeclPass.h"
#include "OverloadResolver.h"

using namespace cdot::diag;

namespace {

llvm::StringMap<BuiltinFn> builtinFunctions = {
   {"sizeof", BuiltinFn::SIZEOF},
   {"alignof", BuiltinFn::ALIGNOF},
   {"typeof", BuiltinFn::TYPEOF},
   {"stackalloc", BuiltinFn::STACK_ALLOC},
   {"__builtin_memcpy", BuiltinFn::MEMCPY},
   {"__builtin_memcpy", BuiltinFn::MEMSET},
   {"__builtin_bitcast", BuiltinFn::BITCAST},
   {"__nullptr", BuiltinFn::NULLPTR},
   {"__builtin_isnull", BuiltinFn::ISNULL},
   {"__builtin_unwrap_protocol", BuiltinFn::UNWRAP_PROTO},
   {"__builtin_sizeof", BuiltinFn::BuiltinSizeof},
};

} // anonymous namespace

void SemaPass::PrepareCallArgs(
   std::vector<pair<string, std::shared_ptr<Expression>>>& args,
   std::vector<Argument> &givenArgs,
   CallCompatability &comp)
{
   if (comp.func == nullptr && comp.method == nullptr) {
      return;
   }

   auto& declaredArgs = comp.func != nullptr ? comp.func->getArguments()
                                             : comp.method->getArguments();

   std::vector<pair<string, std::shared_ptr<Expression>>> realArgs;
   for (const auto &order : comp.argOrder) {
      if (order.second) {
         auto& arg = declaredArgs[order.first];
         realArgs.emplace_back(arg.label, arg.defaultVal);
      }
      else {
         realArgs.push_back(args[order.first]);
      }
   }

   args = realArgs;
   PrepareCallArgs(args, givenArgs, declaredArgs, comp.resolvedArgs);
}

void SemaPass::PrepareCallArgs(
   std::vector<pair<string, std::shared_ptr<Expression>>> &args,
   std::vector<Argument> &givenArgs,
   std::vector<Argument> &declaredArgs,
   std::vector<Type> &resolvedArgs)
{
   size_t i = 0;
   bool vararg = !declaredArgs.empty() && declaredArgs.back().cstyleVararg;
   for (auto& arg : args) {
      arg.second->addUse();

      if (i >= givenArgs.size()) {
         break;
      }

      auto &given = givenArgs[i].type;
      if (given->isAutoTy()) {
         given = resolvedArgs[i];
      }

      if (i >= declaredArgs.size() || (vararg && i >= declaredArgs.size() - 1)){
         if (given->isAutoTy()) {
            given = getResult(arg.second);
         }

         // assume that c style varargs need a primitive...
         if (given->isBoxedPrimitive()) {
            wrapImplicitCast(arg.second, given, Type(given->unbox()));
         }

         // and an rvalue
         toRvalueIfNecessary(given, arg.second);

         ++i;
         continue;
      }

      auto &declared = i >= declaredArgs.size() ? declaredArgs.back().type
                                                : declaredArgs[i].type;

      if (given->isTypeGroup()) {
         arg.second->setContextualType(declared);
         given = getResult(arg.second);
      }

      if (declared->isStruct() && given->isStruct()
          && !declared.isLvalue() && !arg.second->createsTemporary()) {
         arg.second->needsByValPass(true);
      }

      toRvalueIfNecessary(given, arg.second, !declared.isLvalue());
      ++i;
   }
}

void SemaPass::visit(CallExpr *node)
{
   //MAINCALL
   for (size_t i = node->resolvedArgs.size(); i < node->args.size(); ++i) {
      auto& arg = node->args.at(i);
      Type ty = getAmbiguousResult(arg.second);

      node->resolvedArgs.emplace_back(arg.first, ty, arg.second);
   }

   if (node->type == CallType::FUNC_CALL) {
      if (builtinFunctions.find(node->getIdent()) != builtinFunctions.end()) {
         HandleBuiltinCall(node);
      }
      else if (node->ident.empty()) {
         HandleCallOperator(node);
      }
      else if (SymbolTable::hasRecord(node->ident, importedNamespaces)
               || SymbolTable::hasRecordTemplate(node->ident)) {
         HandleConstructorCall(node);
      }
      else {
         HandleFunctionCall(node);
      }
   }
   else if (node->type == CallType::METHOD_CALL) {
      HandleMethodCall(node);
   }
   else if (node->type == CallType::ANON_CALL) {
      HandleAnonCall(node);
   }

   switch (node->getType()) {
      case CallType::METHOD_CALL:
      case CallType::CONSTR_CALL:
         if (node->getMethod()) {
            currentCallable->copyThrows(node->getMethod());
         }

         break;
      case CallType::FUNC_CALL:
         if (!node->isBuiltin()) {
            currentCallable->copyThrows(node->func);
         }

         break;
      default:
         break;
   }

   return ReturnMemberExpr(node, node->returnType);
}

void SemaPass::HandleFunctionCall(CallExpr *node)
{
   // resolve given template args
   if (!node->getTemplateArgs()->isResolved()
       && SymbolTable::hasFunctionTemplate(node->getIdent(),
                                           importedNamespaces)) {
      auto &Constraints = SymbolTable::getConstraints(node->getIdent());
      DeclPass::resolveTemplateArgs(
         node->getTemplateArgs(),
         Constraints,
         [this](TypeRef *node) {
            node->accept(this);
         },
         node
      );
   }

   auto& resolvedArgs = node->resolvedArgs;
   auto result = getFunction(node->ident, resolvedArgs,
                             node->getTemplateArgs()->get());

   if (!result.isCompatible()) {
      // lambda or saved function call
      if (hasVariable(node->ident)) {
         auto var = getVariable(node->ident, node);
         auto fun = var.first.first.type;

         if (fun->isFunctionTy()) {
            pushTy(fun);
            node->type = CallType::ANON_CALL;
            node->binding = var.first.second;

            if (latestScope->inLambda && var.second) {
               latestScope->captures->emplace_back(var.first.second,
                                                   var.first.first.type);
               node->is_captured_var = true;
            }

            return HandleAnonCall(node);
         }

         if (fun->isObject()) {
            pushTy(fun);
            node->setCallOpBinding(var.first.second);

            return HandleCallOperator(node);
         }
      }

      // possible implicit method call
      auto& currentCl = currentClass();
      if (!currentCl.empty() && SymbolTable::hasClass(currentCl)) {
         auto cl = SymbolTable::getClass(currentCl);

         auto compat = getMethod(cl, node->ident, resolvedArgs);

         if (compat.compatibility == CompatibilityType::COMPATIBLE) {
            auto selfTy = Type(ObjectType::get(currentCl));
            pushTy(selfTy);

            node->type = CallType::METHOD_CALL;
            node->implicitSelfCall = true;
            node->selfBinding = latestScope->currentSelf;

            return HandleMethodCall(node);
         }
      }

      if (result.compatibility == CompatibilityType::NO_MATCHING_CALL) {
         diag::err(err_no_matching_call) << 0 << node->ident << node
                                         << diag::cont;
         for (const auto& cand : result.failedCandidates) {
            auto& func = cand.func;
            auto &templateArgs = node->getTemplateArgs()->get();
            if (cand.failedConstraint != nullptr) {
               diag::note(note_cand_failed_constraint)
                  << cand.failedConstraint->reportFailure()
                  << func->getDeclaration()
                  << diag::cont;
            }
            else if (node->resolvedArgs.size() != func->getArguments().size()) {
               diag::note(note_cand_mismatched_arg_count)
                  << func->getArguments().size()
                  << node->resolvedArgs.size()
                  << func->getDeclaration()
                  << diag::whole_line << diag::cont;
            }
            else {
               auto &givenArg = result.resolvedArgs[cand.incompatibleArg];
               auto &neededArg = func->getArguments()[cand.incompatibleArg];
               diag::note(note_cand_no_implicit_conv)
                  << givenArg << neededArg.type
                  << cand.incompatibleArg + 1
                  << func->getDeclaration()->getArgs()[cand.incompatibleArg]
                  << diag::cont;
            }
         }

         exit(1);
      }
      if (result.compatibility == CompatibilityType::FUNC_NOT_FOUND) {
         diag::err(err_func_not_found) << 0 << node->ident << node
                                       << diag::term;
      }

      llvm_unreachable("No other options possible");
   }

   node->declaredArgTypes = result.resolvedNeededArgs;

   auto& func = result.func;
   if (func->isTemplate()) {
      func = DeclPass::declareFunctionInstantiation(
         *func->getTemplate(),
         result.generics
      );
   }

   func->addUse();

   PrepareCallArgs(node->args, resolvedArgs, result);
   ApplyCasts(node, result.resolvedArgs, node->declaredArgTypes);

   node->func = func;
   node->setBinding(SymbolTable::mangleFunction(func->getName(),
                                                func->getArguments()));
   node->setReturnType(func->getReturnType());

   if (node->getReturnType()->needsStructReturn()) {
      node->setHasStructReturn(true);
   }
}

void SemaPass::HandleBuiltinCall(CallExpr *node)
{
   node->is_builtin = true;
   node->builtinFnKind = builtinFunctions[node->getIdent()];

   switch (node->builtinFnKind) {
      case BuiltinFn::ALIGNOF:
      case BuiltinFn::SIZEOF:
      case BuiltinFn::TYPEOF:
      case BuiltinFn::NULLPTR: {
         if (node->args.size() != 1
             || node->args.front().second->get_type() != NodeType::TYPE_REF) {
            RuntimeError::raise("Expected type as argument to " + node->ident,
                                node);
         }

         auto typeref = std::static_pointer_cast<TypeRef>(
            node->args.at(0).second);
         if (!typeref->resolved) {
            typeref->accept(this);
         }

         if (node->builtinFnKind == BuiltinFn::SIZEOF
             || node->builtinFnKind == BuiltinFn::ALIGNOF) {
            *node->returnType = ObjectType::get("Int");

            bool needsCast = false;
            if (node->contextualType->isIntegerTy()) {
               node->returnType = node->contextualType;
               needsCast = true;
            }
            else if (node->contextualType->isBoxedPrimitive()) {
               auto unboxed = node->contextualType->unbox();
               if (unboxed->isIntegerTy()) {
                  node->returnType = node->contextualType;
                  needsCast = true;
               }
            }
            else if (!node->contextualType->isAutoTy()) {
               diag::err(err_type_mismatch) << node->returnType
                                            << node->contextualType
                                            << node << diag::term;
            }

            if (needsCast) {
               node->needs_cast = true;
               *node->castFrom = IntegerType::get()->box();
               node->castTo = node->returnType;
            }
         }

         if (node->builtinFnKind == BuiltinFn::NULLPTR) {
            auto ty = typeref->getType();
            bool refcounted = false;
            if (ty->isObject()) {
               refcounted = ty->getRecord()->isRefcounted();
            }

            if (!refcounted) {
               *node->returnType = ty->getPointerTo();
            }
            else {
               node->returnType = ty;
            }
         }

         break;
      }
      case BuiltinFn::BITCAST: {
         if (node->args.size() != 2
             || node->args.at(1).second->get_type() != NodeType::TYPE_REF) {
            RuntimeError::raise("Expected type as second argument to "
                                + node->ident, node);
         }

         auto& arg = node->args.front();
         node->resolvedArgs.emplace_back(arg.first, getResult(arg.second),
                                         arg.second);

         auto typeref = std::static_pointer_cast<TypeRef>(
            node->args.at(1).second);
         if (!typeref->resolved) {
            typeref->accept(this);
         }

         toRvalueIfNecessary(node->resolvedArgs.front().type,
                             node->args.front().second);
         node->returnType = typeref->getType();

         break;
      }
      case BuiltinFn::ISNULL: {
         if (node->args.size() != 1) {
            diag::err(err_incomp_arg_counts) << 1 << node->args.size()
                                             << node << diag::term;
         }

         *node->returnType = ObjectType::get("Bool");

         auto& arg = node->args.front();
         auto ty = getResult(arg.second);
         node->resolvedArgs.emplace_back(arg.first, ty, arg.second);

         if (!ty->isPointerTy() && !ty->isRawFunctionTy()
             && !ty->isRefcounted()) {
            diag::err(err_unexpected_value) << "pointer type"
                                            << arg.second << diag::term;
         }

         toRvalueIfNecessary(ty, arg.second);
         break;
      }
      case BuiltinFn::UNWRAP_PROTO: {
         if (node->args.size() != 1) {
            diag::err(err_incomp_arg_counts) << 1 << node->args.size()
                                             << node << diag::term;
         }

         auto& arg = node->args.front();
         auto ty = getResult(arg.second);
         node->resolvedArgs.emplace_back(arg.first, ty, arg.second);

         if (!ty->isProtocol()) {
            diag::err(err_unexpected_value) << "protocol"
                                            << arg.second << diag::term;
         }

         toRvalueIfNecessary(ty, arg.second);
         break;
      }
      case BuiltinFn::STACK_ALLOC: {
         if (node->args.size() != 2) {
            diag::err(err_incomp_arg_counts) << 2 << node->args.size()
                                             << node << diag::term;
         }

         auto typeref = std::static_pointer_cast<TypeRef>(
            node->args.at(0).second);
         if (!typeref->resolved) {
            typeref->accept(this);
         }

         auto& arg = node->args[1].second;
         auto ty = typeref->getType();
         toRvalueIfNecessary(ty, arg);

         auto sizeTy = getResult(arg);
         if (!sizeTy->isIntegerTy()) {
            wrapImplicitCast(arg, sizeTy, Type(IntegerType::get()));
         }

         *node->returnType = ty->getPointerTo();

         break;
      }
      case BuiltinFn::BuiltinSizeof: {
         if (node->args.size() != 1) {
            diag::err(err_incomp_arg_counts) << 1 << node->args.size()
                                             << node << diag::term;
         }

         *node->returnType = IntegerType::get();

         auto ty = getResult(node->args.front().second);
         if (ty->isVoidTy()) {
            diag::err(err_generic_error) << "void type has no size" << node
                                         << diag::term;
         }

         toRvalueIfNecessary(ty, node->args.front().second);

         break;
      }
      case BuiltinFn::MEMCPY:
      case BuiltinFn::MEMSET:
         return HandleFunctionCall(node);
   }
}

void SemaPass::HandleEnumCase(CallExpr *node)
{
   auto cl = SymbolTable::getClass(node->getClassName());
   if (!cl->isEnum()) {
      return;
   }

   auto en = cl->getAs<Enum>();
   auto res = getCase(en, node->getIdent(), node->getResolvedArgs());

   if (res.compatibility != CompatibilityType::COMPATIBLE) {
      diag::err(err_enum_case_not_found)
         << node->getClassName() << node->getIdent() << true
         << node << diag::term;
   }

   auto &case_ = en->getCase(node->getIdent());

   node->setDeclaredArgTypes(std::move(res.resolvedNeededArgs));

   PrepareCallArgs(node->getArgs(), node->getResolvedArgs(), res);
   ApplyCasts(node, res.resolvedArgs, node->getDeclaredArgTypes());

   node->isEnumCase(true);
   node->setCaseVal(case_.rawValue);

   std::vector<Type > enumCaseTypes;
   for (const auto &arg : node->getDeclaredArgTypes()) {
      enumCaseTypes.push_back(arg.type);
   }

   auto obj = ObjectType::get(node->getClassName());
   node->setReturnType(Type(obj));
}

void SemaPass::HandleMethodCall(
   CallExpr *node)
{
   Class* cl;
   string className;
   string fullName;
   Type latest;

   if (node->parentExpr == nullptr && node->enum_case) {
      auto& inf = node->contextualType;
      if (inf->isAutoTy() || !SymbolTable::hasClass(inf->getClassName())) {
         RuntimeError::raise("Could not infer type of enum case "
                             + node->ident, node);
      }

      node->className = inf->getClassName();
      node->is_ns_member = true;

      return HandleEnumCase(node);
   }

   if (node->is_ns_member) {
      fullName = node->className + "." + node->ident;

      if (SymbolTable::hasRecord(fullName, importedNamespaces)
          || SymbolTable::hasRecordTemplate(fullName)) {
         node->type = CallType::CONSTR_CALL;
         node->ident = fullName;

         return HandleConstructorCall(node);
      }

      if (SymbolTable::numFunctionsWithName(fullName) > 0) {
         node->type = CallType::FUNC_CALL;
         node->ident = fullName;

         return HandleFunctionCall(node);
      }

      if (!SymbolTable::hasClass(node->className)) {
         RuntimeError::raise("Function " + fullName + " does not exist",
                             node);
      }

      HandleEnumCase(node);
      if (!node->returnType->isAutoTy()) {
         return;
      }

      cl = SymbolTable::getClass(node->className);
      className = node->className;
   }
   else {
      assert(!typeStack.empty() && "Nothing to call method on!");

      latest = popTy();
      if (node->isPointerAccess_()) {
         if (!latest->isPointerTy()) {
            RuntimeError::raise("Value is not a pointer", node);
         }

         latest = latest->asPointerTy()->getPointeeType();
      }

      if (!latest->isObject()) {
         RuntimeError::raise("Cannot call method on value of type "
                             + latest.toString(), node);
      }

      className = latest->getClassName();
      if (!SymbolTable::hasClass(className)) {
         RuntimeError::raise("Class " + latest.toString()
                             + " does not exist", node);
      }

      cl = SymbolTable::getClass(className, importedNamespaces);
      node->setClassName(className);

      if (cl->isProtocol()) {
         node->castFrom = latest;
         node->is_protocol_call = true;
      }
   }

   // resolve given template args
   if (!node->getTemplateArgs()->isResolved()) {
      if (!cl->hasMethodTemplate(node->getIdent())) {
         diag::err(err_no_template_method)
            << cl->getNameSelector() << cl->getName()
            << node->getIdent() << node << diag::term;
      }

      auto &Constraints = cl->getMethodConstraints(node->getIdent());
      DeclPass::resolveTemplateArgs(
         node->getTemplateArgs(),
         Constraints,
         [this](TypeRef *node) {
            node->accept(this);
         },
         node
      );
   }

   checkClassAccessibility(cl, node);

   BuiltinType* caller = node->isNsMember() ? nullptr : *latest;
   auto &givenArgs = node->getResolvedArgs();
   auto methodResult = getMethod(cl, node->getIdent(), givenArgs,
                                 node->getTemplateArgs()->get());

   if (methodResult.compatibility != CompatibilityType::COMPATIBLE) {
      if (cl->hasField(node->getIdent())) {
         auto field = cl->getField(node->getIdent());
         Type fieldTy(field->fieldType);
         if (fieldTy->isFunctionTy()) {
            node->setType(CallType::ANON_CALL);

            auto memberExpr = std::make_shared<MemberRefExpr>(
               std::move(node->getIdent())
            );

            assert(node->getParent());

            CopyNodeProperties(node, memberExpr.get());

            auto parent = static_cast<Expression*>(node->getParent());
            memberExpr->setMemberExpr(parent->getMemberExpr());

            parent->setMemberExpr(memberExpr);
            node->setParentExpr(memberExpr.get());

            return ReturnMemberExpr(parent, latest);
         }
      }

      throwMethodNotFound(methodResult, node, cl);
   }
   else if (node->isNsMember() && !methodResult.method->is_static) {
      diag::err(err_method_must_be_static) << methodResult.method->getName()
                                           << node << diag::term;
   }

   auto& method = methodResult.method;
   method->addUse();

   if (method->isTemplate()) {
      method = DeclPass::declareMethodInstantiation(
         *method->getMethodTemplate(), methodResult.generics
      );
   }

   // check if method is accessible from current context
   checkMemberAccessibility(cl, method->getName(),
                            method->getAccessModifier(), node);

   auto ty = method->getReturnType();
   node->returnType = ty;

   // methods with 'ref' return type
   node->returnType.isLvalue(method->getReturnType().isLvalue());
   node->binding = method->getMangledName();
   node->method = method;
   node->is_virtual = cl->isAbstract() || cl->isVirtual(method);
   node->is_static = method->is_static;
   node->declaredArgTypes = method->getArguments();

   PrepareCallArgs(node->args, givenArgs, methodResult);
   ApplyCasts(node, methodResult.resolvedArgs, node->declaredArgTypes);

   // check if this method has a hidden byval struct parameter
   if (method->hasStructReturn()) {
      node->has_struct_return = true;
   }

   // if we call another constructor, we can assume that all fields
   // will be initialized there, otherwise that constructor will fail.
   // this will accept circular calls as valid, but those will cause
   // other problems anyway
   // FIXME calls in conditions
   auto& uninitializedFields = latestScope->uninitializedFields;
   if (method->getName() == "init" && currentClass() == cl->getName() &&
       uninitializedFields != nullptr)
   {
      uninitializedFields->clear();
   }
}

void SemaPass::HandleConstructorCall(CallExpr *node)
{
   bool clearTemplateArgs = false;
   if (SymbolTable::hasRecordTemplate(node->ident)) {
      auto &Template = *SymbolTable::getRecordTemplate(node->ident);
      if (!node->getTemplateArgs()->isResolved()) {
         DeclPass::resolveTemplateArgs(
            node->getTemplateArgs(),
            Template.constraints,
            [this](TypeRef *node) {
               node->accept(this);
            },
            node
         );
      }

      auto &templateArgs = node->getTemplateArgs()->get();
      if (templateArgs.empty()) {
         OverloadResolver resolver(node->getResolvedArgs(), templateArgs,
                                   resolverFn, TypeResolverFn,
                                   Template.constraints);

         std::vector<TemplateArg> resolvedTemplateArgs;
         for (auto &init : Template.initializers) {
            resolver.inferTemplateArgs(node->getResolvedArgs(), init.args,
                                       resolvedTemplateArgs);

            if (resolvedTemplateArgs.size() == Template.constraints.size()) {
               node->getTemplateArgs()->set(std::move(resolvedTemplateArgs));
               break;
            }
         }

         DeclPass::resolveTemplateArgs(
            node->getTemplateArgs(),
            Template.constraints,
            [this](TypeRef *node) {
               node->accept(this);
            },
            node
         );
      }

      clearTemplateArgs = true;
   }

   auto record = getRecord(
      node->ident,
      node->getTemplateArgs()
   );

   if (record->isUnion()) {
      if (node->resolvedArgs.size() != 1) {
         diag::err(err_union_initializer_arg_count) << node
                                                    << diag::term;
      }

      auto& ty = node->resolvedArgs.front().type;
      auto un = record->getAs<Union>();

      if (!un->initializableWith(*ty)) {
         diag::err(err_union_initializer_type) << node
                                               << diag::term;
      }

      toRvalueIfNecessary(ty, node->args.front().second);

      *node->returnType = ObjectType::get(un->getName());
      node->setUnionConstr(true);
      node->type = CallType::CONSTR_CALL;

      return;
   }

   Class* cl = record->getAs<Class>();
   checkClassAccessibility(cl, node);

   auto& givenArgs = node->resolvedArgs;
   auto constrResult = getMethod(cl, "init", givenArgs);

   if (constrResult.compatibility != CompatibilityType::COMPATIBLE) {
      throwMethodNotFound(constrResult, node, cl);
   }

   auto& method = constrResult.method;

   method->addUse();
   cl->addUse();

   // check accessibility
   checkMemberAccessibility(cl, method->getName(), method->getAccessModifier(),
                            node);
   if (cl->isAbstract()) {
      RuntimeError::raise("Cannot instantiate abstract class " + node->ident,
                          node);
   }

   node->type = CallType::CONSTR_CALL;

   *node->returnType = ObjectType::get(cl->getName());
   node->binding = method->getMangledName();
   node->method = method;
   node->ident = cl->getName();

   node->declaredArgTypes = std::move(constrResult.resolvedNeededArgs);

   PrepareCallArgs(node->args, givenArgs, constrResult);
   ApplyCasts(node, constrResult.resolvedArgs, node->declaredArgTypes);
}

void SemaPass::throwMethodNotFound(
   CallCompatability &result,
   CallExpr *node,
   Class *cl)
{
   diag::err(err_no_matching_call) << 1 << node->ident
                                   << node << diag::cont;

   size_t i = result.failedCandidates.size();
   for (const auto& cand : result.failedCandidates) {
      auto& method = cand.method;
      auto &templateArgs = node->getTemplateArgs()->get();
      if (cand.failedConstraint != nullptr) {
         diag::note(note_cand_failed_constraint)
            << cand.failedConstraint->reportFailure()
            << method->loc << diag::cont;
      }
      else if (node->args.size() != method->getArguments().size()) {
         diag::note(note_cand_mismatched_arg_count)
            << method->getArguments().size()
            << node->args.size()
            << method->loc << diag::cont;
      }
      else {
         auto &givenArg = node->resolvedArgs[cand.incompatibleArg];
         auto &neededArg = method->getArguments()[cand.incompatibleArg];
         diag::note(note_cand_no_implicit_conv)
            << givenArg.type << neededArg.type
            << cand.incompatibleArg + 1
            << method->loc << diag::cont;
      }
   }

   exit(1);
}

void SemaPass::HandleCallOperator(CallExpr *node)
{
   auto latest = popTy();
   if (!latest->isObject() || !SymbolTable::hasClass(latest->getClassName())) {
      pushTy(latest);
      return HandleAnonCall(node);
   }

   auto& className = latest->getClassName();
   string methodName = "postfix ()";
   auto cl = SymbolTable::getClass(className, importedNamespaces);

   auto& givenArgs = node->resolvedArgs;
   auto callOpResult = getMethod(cl, methodName, givenArgs);

   if (callOpResult.compatibility != CompatibilityType::COMPATIBLE) {
      RuntimeError::raise("Call operator with signature "
                          + util::args_to_string(givenArgs) + " does"
         " not exist on class " + className, node);
   }

   auto& method = callOpResult.method;
   node->setType(CallType::METHOD_CALL);
   node->setBinding(method->getMangledName());
   node->setDeclaredArgTypes(std::move(callOpResult.resolvedNeededArgs));
   node->isCallOp(true);
   node->setMethod(method);
   node->loadBeforeCall(latest.needsLvalueToRvalueConv());

   PrepareCallArgs(node->args, givenArgs, callOpResult);
   ApplyCasts(node, callOpResult.resolvedArgs, node->declaredArgTypes);

   node->returnType = method->getReturnType();
}

void SemaPass::HandleAnonCall(CallExpr *node)
{
   auto latest = popTy();
   auto& givenArgs = node->resolvedArgs;

   if (latest->isPointerTy()) {
      latest = latest->asPointerTy()->getPointeeType();
   }

   if (!latest->isFunctionTy()) {
      RuntimeError::raise("Given object of type " + latest.toString()
                          + " is not callable", node);
   }

   auto func = cast<FunctionType>(*latest);

   std::vector<TemplateArg> givenGenerics;
   std::vector<TemplateConstraint> neededGenerics;

   CallCompatability result;
   OverloadResolver::isCallCompatible(result, givenArgs, func->getArgTypes(),
                                      resolverFn);

   if (!result.isCompatible()) {
      diag::err(err_no_matching_call) << 0 << node->getIdent()
                                      << node << diag::term;
   }

   node->setType(CallType::ANON_CALL);
   node->setDeclaredArgTypes(std::vector<Argument>(func->getArgTypes()));

   PrepareCallArgs(node->args, givenArgs, result);
   ApplyCasts(node, result.resolvedArgs, node->getDeclaredArgTypes());

   node->setReturnType(func->getReturnType());
   node->setFunctionType(func);

   if (node->getReturnType()->isStruct()) {
      node->setHasStructReturn(true);
   }
}