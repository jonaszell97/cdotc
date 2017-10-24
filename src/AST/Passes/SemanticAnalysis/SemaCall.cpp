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

using namespace cdot::diag;

void SemaPass::PrepareCallArgs(
   std::vector<pair<string, std::shared_ptr<Expression>>>& args,
   std::vector<Argument> &givenArgs,
   CallCompatability &comp)
{
   if (comp.func == nullptr && comp.method == nullptr) {
      return;
   }

   size_t i = 0;
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
   std::vector<Argument> &resolvedArgs)
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
         given = resolvedArgs[i].type;
      }

      if (i >= declaredArgs.size() || (vararg && i >= declaredArgs.size() - 1)) {
         if (given->isAutoTy()) {
            given = arg.second->accept(*this);
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

      if (given->isAutoTy()) {
         arg.second->setContextualType(declared);
         arg.second->accept(*this);
      }

      if (declared->isStruct() && given->isStruct()
          && !declared.isLvalue() && !arg.second->createsTemporary()) {
         arg.second->needsByValPass(true);
      }

      toRvalueIfNecessary(given, arg.second, !declared.isLvalue());
      ++i;
   }
}

void SemaPass::HandleFunctionCall(CallExpr *node)
{
   auto& resolvedArgs = node->resolvedArgs;
   auto result = getFunction(node->ident, resolvedArgs, node->resolvedGenerics);
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
               latestScope->captures->emplace_back(var.first.second, var.first.first.type);
               node->isCapturedVar = true;
            }

            return HandleAnonCall(node);
         }
      }

      // possible implicit method call
      auto& currentCl = currentClass();
      if (!currentCl.empty() && SymbolTable::hasClass(currentCl)) {
         auto cl = SymbolTable::getClass(currentCl);

         auto compat = cl->hasMethod(node->ident, resolvedArgs, {}, ObjectType::get(currentCl));
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
         diag::err(err_no_matching_call) << 0 << node->ident << node << diag::cont;
         size_t i = result.failedCandidates.size();
         for (const auto& cand : result.failedCandidates) {
            auto& func = cand.func;
            if (cand.failedConstraint != nullptr) {
               diag::note(note_cand_failed_constraint) << cand.failedConstraint->reportFailure()
                                                       << func->getDecl()
                                                       << diag::cont;
            }
            else if (node->resolvedGenerics.size() < func->getGenerics().size()) {
               auto &missingGen = func->getGenerics()[node->resolvedGenerics.size()];
               diag::note(note_cand_generic_type_not_inferred) << missingGen.genericTypeName
                                                               << func->getDecl() << diag::cont;
            }
            else if (node->resolvedArgs.size() != func->getArguments().size()) {
               diag::note(note_cand_mismatched_arg_count) << func->getArguments().size()
                                                          << node->resolvedArgs.size()
                                                          << func->getDecl()
                                                          << diag::whole_line << diag::cont;
            }
            else {
               auto &givenArg = node->resolvedArgs[cand.incompatibleArg];
               auto &neededArg = func->getArguments()[cand.incompatibleArg];
               diag::note(note_cand_no_implicit_conv) << givenArg.type << neededArg.type
                                                      << cand.incompatibleArg + 1
                                                      << func->getDecl()->args[cand.incompatibleArg]
                                                      << diag::cont;
            }
         }

         exit(1);
      }
      if (result.compatibility == CompatibilityType::FUNC_NOT_FOUND) {
         diag::err(err_func_not_found) << 0 << node->ident << node << diag::term;
      }

      llvm_unreachable("No other options possible");
   }

   node->resolvedGenerics = result.generics;
   node->resolvedArgs = result.resolvedArgs;

   auto& func = result.func;
   func->addUse();

   auto& declaredArgs = func->getArguments();
   PrepareCallArgs(node->args, resolvedArgs, result);
   ApplyCasts(node->args, resolvedArgs, declaredArgs, result.neededCasts);

   node->binding = SymbolTable::mangleFunction(func->getName(), declaredArgs);
   node->declaredArgTypes = &func->getArguments();
   node->func = func;

   if (func->hasStructReturn()) {
      node->hasStructReturn = true;
      node->structReturnType = *func->getReturnType();
   }

   auto& retTy = func->getReturnType();
   if (retTy->isGeneric() || (retTy->isObject() && retTy->getRecord()->isGeneric())) {
      auto genericTy = retTy;

      // the covariance of the generic type to cast from
      node->genericOriginTy = genericTy;

      // changes the generic type (i.e. T) with the concrete one
      resolveGenerics(retTy, node->resolvedGenerics);

      node->returnType = retTy;
      node->needsGenericCast = true;

      // concrete generic type of this particular instance
      node->genericDestTy = retTy;
   }
   else {
      node->returnType = func->getReturnType();
   }
}

void SemaPass::HandleBuiltinCall(CallExpr *node)
{
   node->isBuiltin = true;

   auto& Builtin = util::builtinTypes[node->ident];
   if (!Builtin.second.empty()) {
      node->returnType = Type(Builtin.second[0]);
   }

   node->builtinFnKind = Builtin.first;
   switch (node->builtinFnKind) {
      case BuiltinFn::ALIGNOF:
      case BuiltinFn::SIZEOF:
      case BuiltinFn::TYPEOF:
      case BuiltinFn::NULLPTR: {
         if (node->args.size() != 1 || node->args.front().second->get_type() != NodeType::TYPE_REF) {
            RuntimeError::raise("Expected type as argument to " + node->ident, node);
         }

         auto typeref = std::static_pointer_cast<TypeRef>(node->args.at(0).second);
         if (!typeref->resolved) {
            typeref->accept(*this);
         }

         if (node->builtinFnKind == BuiltinFn::SIZEOF || node->builtinFnKind == BuiltinFn::ALIGNOF) {
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
               diag::err(err_type_mismatch) << node->returnType << node->contextualType
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
         if (node->args.size() != 2 || node->args.at(1).second->get_type() != NodeType::TYPE_REF) {
            RuntimeError::raise("Expected type as second argument to " + node->ident, node);
         }

         auto& arg = node->args.front();
         node->resolvedArgs.emplace_back(arg.first, arg.second->accept(*this), arg.second);

         auto typeref = std::static_pointer_cast<TypeRef>(node->args.at(1).second);
         if (!typeref->resolved) {
            typeref->accept(*this);
         }

         toRvalueIfNecessary(node->resolvedArgs.front().type, node->args.front().second);
         node->returnType = typeref->getType();

         break;
      }
      case BuiltinFn::ISNULL: {
         if (node->args.size() != 1) {
            diag::err(err_incomp_arg_counts) << 1 << node->args.size()
                                             << node << diag::term;
         }

         auto& arg = node->args.front();
         auto ty = arg.second->accept(*this);
         node->resolvedArgs.emplace_back(arg.first, ty, arg.second);

         if (!ty->isPointerTy() && !ty->isRawFunctionTy() && !ty->isRefcounted()) {
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
         auto ty = arg.second->accept(*this);
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

         auto typeref = std::static_pointer_cast<TypeRef>(node->args.at(0).second);
         if (!typeref->resolved) {
            typeref->accept(*this);
         }

         auto& arg = node->args[1].second;
         auto ty = typeref->getType();
         toRvalueIfNecessary(ty, arg);

         auto sizeTy = arg->accept(*this);
         if (!sizeTy->isIntegerTy()) {
            wrapImplicitCast(arg, sizeTy, Type(IntegerType::get()));
         }

         *node->returnType = ty->getPointerTo();

         break;
      }
      case BuiltinFn::MEMCPY:
      case BuiltinFn::MEMSET:
         return HandleFunctionCall(node);
   }
}

void SemaPass::HandleEnumCase(CallExpr *node)
{
   auto cl = SymbolTable::getClass(node->className);
   if (!cl->isEnum()) {
      return;
   }

   auto en = static_cast<Enum *>(cl);
   auto res = en->hasCase(node->ident, node->resolvedArgs, node->resolvedGenerics);
   if (res.compatibility != CompatibilityType::COMPATIBLE) {
//      diag::err(err_enum_case_not_found) << node->className << node->ident << true
//                                         << node << diag::term;
   }

   auto &case_ = en->getCase(node->ident);
   std::vector<Argument> neededArgs;
   for (const auto &ty : case_.associatedValues) {
      neededArgs.emplace_back(ty.first, ty.second);
   }

   node->resolvedGenerics = res.generics;
   node->resolvedArgs = res.resolvedArgs;

   PrepareCallArgs(node->args, node->resolvedArgs, res);
   ApplyCasts(node->args, node->resolvedArgs, neededArgs, res.neededCasts);

   node->enum_case = true;
   node->caseVal = case_.rawValue;
   node->resolvedGenerics = res.generics;

   std::vector<Type > enumCaseTypes;
   for (const auto &arg : neededArgs) {
      enumCaseTypes.push_back(arg.type);
   }

   auto obj = ObjectType::get(node->className, node->resolvedGenerics);
   node->returnType = Type(obj);
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
         RuntimeError::raise("Could not infer type of enum case " + node->ident, node);
      }

      node->className = inf->getClassName();
      node->isNsMember = true;

      return HandleEnumCase(node);
   }

   if (node->isNsMember) {
      fullName = node->className + "." + node->ident;

      if (SymbolTable::hasClass(fullName)) {
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
         RuntimeError::raise("Function " + fullName + " does not exist", node);
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
      if (node->isPointerAccess_) {
         if (!latest->isPointerTy()) {
            RuntimeError::raise("Value is not a pointer", node);
         }

         latest = latest->asPointerTy()->getPointeeType();
      }

      if (!latest->isObject()) {
         RuntimeError::raise("Cannot call method on value of type " + latest.toString(), node);
      }

      className = latest->getClassName();
      if (!SymbolTable::hasClass(className)) {
         RuntimeError::raise("Class " + latest.toString() + " does not exist", node);
      }

      cl = SymbolTable::getClass(className, importedNamespaces);
      node->className = className;

      if (cl->isProtocol()) {
         node->castFrom = latest;
         node->isProtocolCall = true;
      }
   }

   checkClassAccessibility(cl, node);

   BuiltinType* caller = node->isNsMember ? nullptr : *latest;
   auto &givenArgs = node->resolvedArgs;
   auto methodResult = cl->hasMethod(node->ident, givenArgs, node->resolvedGenerics, caller);

   node->resolvedGenerics = methodResult.generics;

   if (methodResult.compatibility != CompatibilityType::COMPATIBLE) {
      if (cl->hasField(node->ident)) {
         auto field = cl->getField(node->ident);
         Type fieldTy(field->fieldType);
         if (fieldTy->isFunctionTy()) {
            auto anon = std::make_shared<CallExpr>(
               CallType::ANON_CALL,
               std::move(node->args)
            );

            CopyNodeProperties(node, anon.get());

            anon->parentExpr = node;
            anon->resolvedArgs = std::move(node->resolvedArgs);
            node->memberExpr = anon;

            node->returnType = fieldTy;
            node->returnType.isLvalue(true);

            node->setIsAnonymousCall(true);
            node->setAnonymousFieldIndex(cl->getFieldOffset(field->fieldName));

            return;
         }
      }

      node->resolvedArgs = methodResult.resolvedArgs;
      throwMethodNotFound(methodResult, node, cl);
   }
   else if (node->isIsNsMember() && !methodResult.method->isStatic) {
      diag::err(err_method_must_be_static) << methodResult.method->getName()
                                           << node << diag::term;
   }

   auto& method = methodResult.method;
   method->addUse();

   // check if method is accessible from current context
   checkMemberAccessibility(cl, method->getName(), method->getAccessModifier(), node);

   auto ty = method->getReturnType();
   bool isGeneric = false;
   if (method->isStatic) {
      isGeneric = castGenericIfNecessary(node, node->resolvedGenerics,
                                         ty, currentClass() != cl->getName());
   }
   else {
      isGeneric = castGenericIfNecessary(node, latest->getConcreteGenericTypes(),
                                         ty, currentClass() != cl->getName());
   }

   if (isGeneric) {
      node->returnType = node->castTo;
      node->returnType.isLvalue(method->getReturnType().isLvalue());
   }
   else {
      node->returnType = ty;
   }

   // returning an object with non-resolved generics
   if (isGeneric) {
      if (method->isStatic) {
         resolveGenerics(node->returnType, node->resolvedGenerics);
      }
      else {
         resolveGenerics(node->returnType, latest->getConcreteGenericTypes());
      }
   }

   if (method->isProtocolDefaultImpl) {
      assert(node->parentExpr != nullptr && "No parent expression to cast!");
      node->parentExpr->needs_cast = true;
      node->parentExpr->castFrom = latest;
      node->parentExpr->castTo = Type(ObjectType::get(method->protocolName));
   }

   // methods with 'ref' return type
   node->returnType.isLvalue(method->getReturnType().isLvalue());
   node->binding = method->getMangledName();
   node->method = method;
   node->is_virtual = cl->isAbstract() || cl->isVirtual(method);
   node->isStatic = method->isStatic;

   auto& declaredArgs = method->getArguments();
   PrepareCallArgs(node->args, givenArgs, methodResult);
   ApplyCasts(node->args, givenArgs, declaredArgs, methodResult.neededCasts);

   node->declaredArgTypes = &method->getArguments();

   // check if this method has a hidden byval struct parameter
   if (method->hasStructReturn()) {
      node->hasStructReturn = true;
      node->structReturnType = *method->getReturnType();
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
   auto record = SymbolTable::getRecord(node->ident, importedNamespaces);
   if (record->isUnion()) {
      if (node->resolvedArgs.size() != 1) {
         diag::err(err_union_initializer_arg_count) << node << diag::term;
      }

      auto& ty = node->resolvedArgs.front().type;
      auto un = record->getAs<Union>();

      if (!un->initializableWith(*ty)) {
         diag::err(err_union_initializer_type) << node << diag::term;
      }

      toRvalueIfNecessary(ty, node->args.front().second);

      *node->returnType = ObjectType::get(un->getName());
      node->setUnionConstr(true);
      node->type = CallType::CONSTR_CALL;

      return;
   }

   Class* cl = record->getAs<Class>();
   checkClassAccessibility(cl, node);

   // resolve generics
   size_t i = 0;
   auto& classGenerics = cl->getGenerics();
   for (const auto& gen : node->generics) {
      if (i < classGenerics.size()) {
         gen->accept(*this);
         auto resolved = GenericType::get(classGenerics[i].genericTypeName, *gen->getType());

         node->resolvedGenerics.push_back(resolved);
         ++i;
      }
      else {
         break;
      }
   }

   string methodName = "init";
   auto& givenArgs = node->resolvedArgs;
   auto constrResult = cl->hasMethod(methodName, givenArgs, node->resolvedGenerics);

   node->resolvedGenerics = constrResult.generics;
   node->resolvedArgs = constrResult.resolvedArgs;

   if (constrResult.compatibility != CompatibilityType::COMPATIBLE) {
      throwMethodNotFound(constrResult, node, cl);
   }

   auto& method = constrResult.method;
   node->method = method;

   method->addUse();
   cl->addUse();

   // check accessibility
   checkMemberAccessibility(cl, method->getName(), method->getAccessModifier(), node);

   node->ident = cl->getName();
   if (cl->isAbstract()) {
      RuntimeError::raise("Cannot instantiate abstract class " + node->ident, node);
   }

   ObjectType* returnType = ObjectType::get(node->ident, node->resolvedGenerics);
   *node->returnType = returnType;

   node->type = CallType::CONSTR_CALL;
   node->binding = method->getMangledName();

   auto& declaredArgs = method->getArguments();
   PrepareCallArgs(node->args, givenArgs, constrResult);
   ApplyCasts(node->args, givenArgs, declaredArgs, constrResult.neededCasts);

   node->declaredArgTypes = &method->getArguments();
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
      if (cand.failedConstraint != nullptr) {
         diag::note(note_cand_failed_constraint) << cand.failedConstraint->reportFailure()
                                                 << method->loc
                                                 << diag::cont;
      }
      else if (node->resolvedGenerics.size() < method->getGenerics().size()) {
         auto &missingGen = method->getGenerics()[node->resolvedGenerics.size()];
         diag::note(note_cand_generic_type_not_inferred) << missingGen.genericTypeName
                                                         << method->declaration << diag::cont;
      }
      else if (node->args.size() != method->getArguments().size()) {
         diag::note(note_cand_mismatched_arg_count) << method->getArguments().size()
                                                    << node->args.size()
                                                    << method->loc
                                                    << diag::cont;
      }
      else {
         auto &givenArg = node->resolvedArgs[cand.incompatibleArg];
         auto &neededArg = method->getArguments()[cand.incompatibleArg];
         diag::note(note_cand_no_implicit_conv) << givenArg.type << neededArg.type
                                                << cand.incompatibleArg + 1
                                                << method->loc << diag::cont;
      }
   }

   exit(1);
}

void SemaPass::HandleCallOperator(CallExpr *node)
{
   auto latest = popTy();
   if (latest->isObject() || !SymbolTable::hasClass(latest->getClassName())) {
      pushTy(latest);
      return HandleAnonCall(node);
   }

   auto& className = latest->getClassName();
   string methodName = "postfix ()";
   auto cl = SymbolTable::getClass(className, importedNamespaces);

   auto& givenArgs = node->resolvedArgs;
   auto callOpResult = cl->hasMethod(methodName, givenArgs, {}, *latest);

   if (callOpResult.compatibility != CompatibilityType::COMPATIBLE) {
      RuntimeError::raise("Call operator with signature " + util::args_to_string(givenArgs) + " does"
         " not exist on class " + className, node);
   }

   node->resolvedArgs = callOpResult.resolvedArgs;

   auto& method = callOpResult.method;
   node->type = CallType::METHOD_CALL;
   node->ident = "postfix ()";
   node->binding = method->getMangledName();

   auto& declaredArgs = method->getArguments();
   PrepareCallArgs(node->args, givenArgs, callOpResult);
   ApplyCasts(node->args, givenArgs, declaredArgs, callOpResult.neededCasts);

   node->returnType = method->getReturnType();
   node->declaredArgTypes = &method->getArguments();
}

void SemaPass::HandleAnonCall(CallExpr *node)
{
   auto latest = popTy();
   auto& givenArgs = node->resolvedArgs;

   if (latest->isPointerTy()) {
      latest = latest->asPointerTy()->getPointeeType();
   }

   if (!latest->isFunctionTy()) {
      RuntimeError::raise("Given object of type " + latest.toString() + " is not callable", node);
   }

   auto func = cast<FunctionType>(*latest);

   std::vector<GenericType*> givenGenerics;
   std::vector<GenericConstraint> neededGenerics;
   std::vector<Argument> neededArgs(func->getArgTypes());

   auto result = util::findMatchingCall(givenArgs, neededArgs, givenGenerics, neededGenerics);
   if (!result.isCompatible()) {
      diag::err(err_no_matching_call) << 0 << node->ident << node << diag::term;
   }

   node->type = CallType::ANON_CALL;
   node->resolvedArgs = result.resolvedArgs;

   PrepareCallArgs(node->args, givenArgs, neededArgs, result.resolvedArgs);
   ApplyCasts(node->args, givenArgs, neededArgs, result.neededCasts);

   node->returnType = func->getReturnType();
   node->functionType = func;

   if (node->returnType->isStruct()) {
      node->hasStructReturn = true;
      node->structReturnType = *node->returnType;
   }
}