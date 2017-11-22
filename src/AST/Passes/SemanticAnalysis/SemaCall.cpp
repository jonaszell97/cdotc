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

#include "../../../Variant/Type/Type.h"
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
#include "../../../parse/Parser.h"
#include "../Declaration/DeclPass.h"
#include "OverloadResolver.h"

using namespace cdot::diag;
using ast::Function;

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

namespace cdot {
namespace ast {

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
   std::vector<QualType> &resolvedArgs)
{
   size_t i = 0;
   bool vararg = !declaredArgs.empty() && declaredArgs.back().cstyleVararg;
   for (auto& arg : args) {
      if (i >= givenArgs.size()) {
         break;
      }

      auto &given = givenArgs[i].type;
      if (given->isAutoTy()) {
         given = resolvedArgs[i];
      }

      if (i >= declaredArgs.size() || (vararg && i >= declaredArgs.size() - 1)){
         if (given->isAutoTy()) {
            given = VisitNode(arg.second);
         }

         // assume that c style varargs need a primitive...
         if (given->isBoxedPrimitive()) {
            wrapImplicitCast(arg.second, given, QualType(given->unbox()));
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
         given = VisitNode(arg.second);
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
   auto &args = node->getArgs();
   for (size_t i = node->getResolvedArgs().size(); i < args.size(); ++i) {
      auto& arg = args.at(i);
      QualType ty = getAmbiguousResult(arg.second);

      node->getResolvedArgs().emplace_back(arg.first, ty, arg.second);
   }

   if (node->getType() == CallType::FUNC_CALL) {
      if (builtinFunctions.find(node->getIdent()) != builtinFunctions.end()) {
         HandleBuiltinCall(node);
      }
      else if (node->getIdent().empty()) {
         HandleCallOperator(node);
      }
      else if (SymbolTable::hasRecord(node->getIdent(), importedNamespaces)
               || SymbolTable::hasRecordTemplate(node->getIdent())) {
         HandleConstructorCall(node);
      }
      else {
         HandleFunctionCall(node);
      }
   }
   else if (node->getType() == CallType::METHOD_CALL) {
      HandleMethodCall(node);
   }
   else if (node->getType() == CallType::ANON_CALL) {
      HandleAnonCall(node);
   }

   switch (node->getType()) {
      case CallType::METHOD_CALL:
      case CallType::CONSTR_CALL:
         if (node->getMethod()) {
            latestScope->function->copyThrows(node->getMethod());
         }

         break;
      case CallType::FUNC_CALL:
         if (!node->isBuiltin()) {
            latestScope->function->copyThrows(node->getFunc());
         }

         break;
      default:
         break;
   }

   return ReturnMemberExpr(node, node->getReturnType());
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
            VisitNode(node);
         },
         node
      );
   }

   auto& resolvedArgs = node->getResolvedArgs();
   auto result = getFunction(node->getIdent(), resolvedArgs,
                             node->getTemplateArgs()->get());

   if (!result.isCompatible()) {
      // lambda or saved function call
      if (hasVariable(node->getIdent())) {
         auto var = getVariable(node->getIdent(), node);
         auto &fun = var.V->type;

         if (fun->isFunctionTy()) {
            pushTy(fun);
            node->setType(CallType::ANON_CALL);
            node->setBinding(var.scope);

            if (var.escapesLambdaScope) {
               latestScope->captures->insert(var.scope);
               node->setIsCapturedVar(true);
            }

            return HandleAnonCall(node);
         }

         if (fun->isObjectTy()) {
            pushTy(fun);
            node->setCallOpBinding(var.scope);

            return HandleCallOperator(node);
         }
      }

      // possible implicit method call
      auto& currentCl = currentClass();
      if (!currentCl.empty() && SymbolTable::hasClass(currentCl)) {
         auto cl = SymbolTable::getClass(currentCl);

         auto compat = getMethod(cl, node->getIdent(), resolvedArgs);
         if (compat.compatibility == CompatibilityType::COMPATIBLE) {
            auto selfTy = QualType(ObjectType::get(currentCl));
            pushTy(selfTy);

            node->setType(CallType::METHOD_CALL);
            node->setImplicitSelfCall(true);

            return HandleMethodCall(node);
         }
      }

      if (result.compatibility == CompatibilityType::NO_MATCHING_CALL) {
         diag::err(err_no_matching_call) << 0 << node->getIdent() << node
                                         << diag::cont;
         
         auto &resolvedArgs = node->getResolvedArgs();
         for (const auto& cand : result.failedCandidates) {
            auto& func = cand.func;
            auto &templateArgs = node->getTemplateArgs()->get();
            if (cand.failedConstraint != nullptr) {
               diag::note(note_cand_failed_constraint)
                  << cand.failedConstraint->reportFailure()
                  << func->getDeclaration()
                  << diag::cont;
            }
            else if (resolvedArgs.size() != func->getArguments().size()) {
               diag::note(note_cand_mismatched_arg_count)
                  << func->getArguments().size()
                  << resolvedArgs.size()
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
         diag::err(err_func_not_found) << 0 << node->getIdent() << node
                                       << diag::term;
      }

      llvm_unreachable("No other options possible");
   }

   node->setDeclaredArgTypes(std::move(result.resolvedNeededArgs));

   auto& func = result.func;
   if (func->isTemplate()) {
      func = DeclPass::declareFunctionInstantiation(
         *func->getTemplate(),
         result.generics
      );
   }

   PrepareCallArgs(node->getArgs(), resolvedArgs, result);
   ApplyCasts(node, result.resolvedArgs, node->getDeclaredArgTypes());

   node->setFunc(func);
   node->setBinding(func->getMangledName());
   node->setReturnType(func->getReturnType());

   if (node->getReturnType()->needsStructReturn()) {
      node->setHasStructReturn(true);
   }
}

void SemaPass::HandleBuiltinCall(CallExpr *node)
{
   node->setIsBuiltin(true);
   node->setBuiltinFnKind(builtinFunctions[node->getIdent()]);

   auto &args = node->getArgs();
   auto FnKind = node->getBuiltinFnKind();

   switch (node->getBuiltinFnKind()) {
      case BuiltinFn::ALIGNOF:
      case BuiltinFn::SIZEOF:
      case BuiltinFn::TYPEOF:
      case BuiltinFn::NULLPTR: {
         if (args.size() != 1 || !isa<TypeRef>(args[0].second)) {
            RuntimeError::raise("Expected type as argument to "
                                + node->getIdent(), node);
         }

         auto typeref = std::static_pointer_cast<TypeRef>(
            args.at(0).second);
         if (!typeref->isResolved()) {
            VisitNode(typeref);
         }

         if (FnKind == BuiltinFn::SIZEOF || FnKind == BuiltinFn::ALIGNOF) {
            *node->getReturnType() = ObjectType::get("Int");

            bool needsCast = false;
            if (node->getContextualType()->isIntegerTy()) {
               node->setReturnType(node->getContextualType());
               needsCast = true;
            }
            else if (node->getContextualType()->isBoxedPrimitive()) {
               auto unboxed = node->getContextualType()->unbox();
               if (unboxed->isIntegerTy()) {
                  node->setReturnType(node->getContextualType());
                  needsCast = true;
               }
            }
            else if (!node->getContextualType()->isAutoTy()) {
               diag::err(err_type_mismatch) << node->getReturnType()
                                            << node->getContextualType()
                                            << node << diag::term;
            }

            if (needsCast) {
               node->needsCast(true);
               node->setCastFrom(QualType(IntegerType::get()->box()));
               node->setCastTo(node->getReturnType());
            }
         }

         if (FnKind == BuiltinFn::NULLPTR) {
            auto ty = typeref->getType();
            bool refcounted = false;
            if (ty->isObjectTy()) {
               refcounted = ty->getRecord()->isRefcounted();
            }

            if (!refcounted) {
               *node->getReturnType() = ty->getPointerTo();
            }
            else {
               node->setReturnType(ty);
            }
         }

         break;
      }
      case BuiltinFn::BITCAST: {
         if (args.size() != 2 || !isa<TypeRef>(args[1].second)) {
            RuntimeError::raise("Expected type as second argument to "
                                + node->getIdent(), node);
         }

         auto& arg = args.front();
         node->getResolvedArgs().emplace_back(arg.first, VisitNode(arg.second),
                                              arg.second);

         auto typeref = std::static_pointer_cast<TypeRef>(
            args.at(1).second);
         if (!typeref->isResolved()) {
            VisitNode(typeref);
         }

         toRvalueIfNecessary(node->getResolvedArgs().front().type,
                             args.front().second);
         node->setReturnType(typeref->getType());

         break;
      }
      case BuiltinFn::ISNULL: {
         if (args.size() != 1) {
            diag::err(err_incomp_arg_counts) << 1 << args.size()
                                             << node << diag::term;
         }

         *node->getReturnType() = ObjectType::get("Bool");

         auto& arg = args.front();
         auto ty = VisitNode(arg.second);
         node->getResolvedArgs().emplace_back(arg.first, ty, arg.second);

         if (!ty->isPointerTy() && !ty->isRawFunctionTy()
             && !ty->isRefcounted()) {
            diag::err(err_unexpected_value) << "pointer type"
                                            << arg.second << diag::term;
         }

         toRvalueIfNecessary(ty, arg.second);
         break;
      }
      case BuiltinFn::UNWRAP_PROTO: {
         if (args.size() != 1) {
            diag::err(err_incomp_arg_counts) << 1 << args.size()
                                             << node << diag::term;
         }

         auto& arg = args.front();
         auto ty = VisitNode(arg.second);
         node->getResolvedArgs().emplace_back(arg.first, ty, arg.second);

         if (!ty->isProtocol()) {
            diag::err(err_unexpected_value) << "protocol"
                                            << arg.second << diag::term;
         }

         toRvalueIfNecessary(ty, arg.second);
         break;
      }
      case BuiltinFn::STACK_ALLOC: {
         if (args.size() != 2) {
            diag::err(err_incomp_arg_counts) << 2 << args.size()
                                             << node << diag::term;
         }

         auto typeref = std::static_pointer_cast<TypeRef>(
            args.at(0).second);
         if (!typeref->isResolved()) {
            VisitNode(typeref);
         }

         auto& arg = args[1].second;
         auto ty = typeref->getType();
         toRvalueIfNecessary(ty, arg);

         auto sizeTy = VisitNode(arg);
         if (!sizeTy->isIntegerTy()) {
            wrapImplicitCast(arg, sizeTy, QualType(IntegerType::get()));
         }

         *node->getReturnType() = ty->getPointerTo();

         break;
      }
      case BuiltinFn::BuiltinSizeof: {
         if (args.size() != 1) {
            diag::err(err_incomp_arg_counts) << 1 << args.size()
                                             << node << diag::term;
         }

         *node->getReturnType() = IntegerType::get();

         auto ty = VisitNode(args.front().second);
         if (ty->isVoidTy()) {
            diag::err(err_generic_error) << "void type has no size" << node
                                         << diag::term;
         }

         toRvalueIfNecessary(ty, args.front().second);

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

   std::vector<QualType > enumCaseTypes;
   for (const auto &arg : node->getDeclaredArgTypes()) {
      enumCaseTypes.push_back(arg.type);
   }

   auto obj = ObjectType::get(node->getClassName());
   node->setReturnType(QualType(obj));
}

void SemaPass::HandleMethodCall(
   CallExpr *node)
{
   Class* cl;
   string className;
   string fullName;
   QualType latest;

   if (node->getParentExpr() == nullptr && node->isEnumCase()) {
      auto& inf = node->getContextualType();
      if (inf->isAutoTy() || !SymbolTable::hasClass(inf->getClassName())) {
         RuntimeError::raise("Could not infer type of enum case "
                             + node->getIdent(), node);
      }

      node->setClassName(inf->getClassName());
      node->setIsNsMember(true);

      return HandleEnumCase(node);
   }

   if (node->isNsMember()) {
      fullName = node->getClassName() + "." + node->getIdent();

      if (SymbolTable::hasRecord(fullName, importedNamespaces)
          || SymbolTable::hasRecordTemplate(fullName)) {
         node->setType(CallType::CONSTR_CALL);
         node->setIdent(fullName);

         return HandleConstructorCall(node);
      }

      if (SymbolTable::numFunctionsWithName(fullName) > 0) {
         node->setType(CallType::FUNC_CALL);
         node->setIdent(fullName);

         return HandleFunctionCall(node);
      }

      if (!SymbolTable::hasClass(node->getClassName())) {
         RuntimeError::raise("Function " + fullName + " does not exist",
                             node);
      }

      HandleEnumCase(node);
      if (!node->getReturnType()->isAutoTy()) {
         return;
      }

      cl = SymbolTable::getClass(node->getClassName());
      className = node->getClassName();
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

      if (!latest->isObjectTy()) {
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
         node->setCastFrom(latest);
         node->setIsProtocolCall(true);
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
            VisitNode(node);
         },
         node
      );
   }

   checkClassAccessibility(cl, node);

   Type* caller = node->isNsMember() ? nullptr : *latest;
   auto &givenArgs = node->getResolvedArgs();
   auto methodResult = getMethod(cl, node->getIdent(), givenArgs,
                                 node->getTemplateArgs()->get());

   if (methodResult.compatibility != CompatibilityType::COMPATIBLE) {
      if (cl->hasField(node->getIdent())) {
         auto field = cl->getField(node->getIdent());
         QualType fieldTy(field->fieldType);
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
   if (method->isTemplate()) {
      method = DeclPass::declareMethodInstantiation(
         *method->getMethodTemplate(), methodResult.generics
      );
   }

   // check if method is accessible from current context
   checkMemberAccessibility(cl, method->getName(),
                            method->getAccessModifier(), node);

   auto ty = method->getReturnType();
   node->setReturnType(ty);

   // methods with 'ref' return type
   node->getReturnType().isLvalue(method->getReturnType().isLvalue());
   node->setBinding(method->getMangledName());
   node->setMethod(method);
   node->setIsVirtual(cl->isAbstract() || cl->isVirtual(method));
   node->setIsStatic(method->isStatic());

   PrepareCallArgs(node->getArgs(), givenArgs, methodResult);
   ApplyCasts(node, methodResult.resolvedArgs, method->getArguments());

   // check if this method has a hidden byval struct parameter
   if (method->hasStructReturn()) {
      node->setHasStructReturn(true);
   }
}

void SemaPass::HandleConstructorCall(CallExpr *node)
{
   bool clearTemplateArgs = false;
   if (SymbolTable::hasRecordTemplate(node->getIdent())) {
      auto &Template = *SymbolTable::getRecordTemplate(node->getIdent());
      if (!node->getTemplateArgs()->isResolved()) {
         DeclPass::resolveTemplateArgs(
            node->getTemplateArgs(),
            Template.constraints,
            [this](TypeRef *node) {
               VisitNode(node);
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
               VisitNode(node);
            },
            node
         );
      }

      clearTemplateArgs = true;
   }

   auto record = getRecord(
      node->getIdent(),
      node->getTemplateArgs()
   );

   if (record->isUnion()) {
      if (node->getResolvedArgs().size() != 1) {
         diag::err(err_union_initializer_arg_count) << node
                                                    << diag::term;
      }

      auto& ty = node->getResolvedArgs().front().type;
      auto un = record->getAs<Union>();


      auto neededTy = un->initializableWith(*ty);
      if (!neededTy) {
         diag::err(err_union_initializer_type) << node
                                               << diag::term;
      }

      if (ty->isTypeGroup()) {
         node->getArgs().front().second->setContextualType(QualType(neededTy));
         ty = VisitNode(node->getArgs().front().second);
      }

      toRvalueIfNecessary(ty, node->getArgs().front().second);
      wrapImplicitCast(node->getArgs().front().second, ty,
                       QualType(neededTy));

      *node->getReturnType() = ObjectType::get(un->getName());
      node->setUnionConstr(true);
      node->setType(CallType::CONSTR_CALL);

      return;
   }

   Class* cl = record->getAs<Class>();
   checkClassAccessibility(cl, node);

   auto& givenArgs = node->getResolvedArgs();
   auto constrResult = getMethod(cl, "init", givenArgs);

   if (constrResult.compatibility != CompatibilityType::COMPATIBLE) {
      throwMethodNotFound(constrResult, node, cl);
   }

   auto& method = constrResult.method;

   // check accessibility
   checkMemberAccessibility(cl, method->getName(), method->getAccessModifier(),
                            node);
   if (cl->isAbstract()) {
      RuntimeError::raise("Cannot instantiate abstract class "
                          + node->getIdent(), node);
   }

   node->setType(CallType::CONSTR_CALL);

   *node->getReturnType() = ObjectType::get(cl->getName());
   node->setMethod(method);
   node->setIdent(cl->getName());

   PrepareCallArgs(node->getArgs(), givenArgs, constrResult);
   ApplyCasts(node, constrResult.resolvedArgs, method->getArguments());
}

void SemaPass::throwMethodNotFound(
   CallCompatability &result,
   CallExpr *node,
   Class *cl)
{
   diag::err(err_no_matching_call) << 1 << node->getIdent()
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
      else if (node->getArgs().size() != method->getArguments().size()) {
         diag::note(note_cand_mismatched_arg_count)
            << method->getArguments().size()
            << node->getArgs().size()
            << method->loc << diag::cont;
      }
      else {
         auto &givenArg = node->getResolvedArgs()[cand.incompatibleArg];
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
   if (!latest->isObjectTy() || !SymbolTable::hasClass(latest->getClassName())) {
      pushTy(latest);
      return HandleAnonCall(node);
   }

   auto& className = latest->getClassName();
   string methodName = "postfix ()";
   auto cl = SymbolTable::getClass(className, importedNamespaces);

   auto& givenArgs = node->getResolvedArgs();
   auto callOpResult = getMethod(cl, methodName, givenArgs);

   if (callOpResult.compatibility != CompatibilityType::COMPATIBLE) {
      RuntimeError::raise("Call operator with signature "
                          + util::args_to_string(givenArgs) + " does"
         " not exist on class " + className, node);
   }

   auto& method = callOpResult.method;
   node->setType(CallType::METHOD_CALL);
   node->setBinding(method->getMangledName());
   node->isCallOp(true);
   node->setMethod(method);
   node->loadBeforeCall(latest.needsLvalueToRvalueConv());

   PrepareCallArgs(node->getArgs(), givenArgs, callOpResult);
   ApplyCasts(node, callOpResult.resolvedArgs,
              callOpResult.resolvedNeededArgs);

   node->setReturnType(method->getReturnType());
}

void SemaPass::HandleAnonCall(CallExpr *node)
{
   auto latest = popTy();
   auto& givenArgs = node->getResolvedArgs();

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

   PrepareCallArgs(node->getArgs(), givenArgs, result);
   ApplyCasts(node, result.resolvedArgs, node->getDeclaredArgTypes());

   node->setReturnType(func->getReturnType());
   node->setFunctionType(func);

   if (node->getReturnType()->isStruct()) {
      node->setHasStructReturn(true);
   }
}

} // namespace ast
} // namespace cdot