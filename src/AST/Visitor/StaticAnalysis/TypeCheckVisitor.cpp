//
// Created by Jonas Zell on 04.07.17.
//

#include <iostream>
#include <fstream>
#include "TypeCheckVisitor.h"
#include "../Visitor.cpp"
#include "../../SymbolTable.h"
#include "Class.h"
#include "../../../Message/Warning.h"
#include "../../Statement/Declaration/ExtendStmt.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/CollectionType.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../Operator/Conversion/LvalueToRvalue.h"
#include "../../../Util.h"

string self_str = "self";
std::vector<string> TypeCheckVisitor::currentNamespace = { "" };
std::vector<string> TypeCheckVisitor::importedNamespaces = {""};
std::vector<GenericType*>* TypeCheckVisitor::currentClassGenerics = nullptr;
bool TypeCheckVisitor::inProtocolDefinition = false;

TypeCheckVisitor::TypeCheckVisitor() {

}

TypeCheckVisitor::TypeCheckVisitor(TypeCheckVisitor *parent) :
    parent(parent),
    continuable(parent->continuable),
    breakable(parent->breakable),
    scope(parent->scope + std::to_string(parent->children.size())),
    currentClass(parent->currentClass),
    currentSelf(parent->currentSelf),
    declaredReturnType(parent->declaredReturnType),
    currentBlockUnsafe(parent->currentBlockUnsafe),
    uninitializedFields(parent->uninitializedFields)
{
    parent->addChild(this);
}

TypeCheckVisitor TypeCheckVisitor::makeFunctionVisitor(Type *declaredReturnTy) {
    TypeCheckVisitor funcVisitor(this);

    funcVisitor.declaredReturnType = declaredReturnTy;
    funcVisitor.returnable = true;

    return funcVisitor;
}

TypeCheckVisitor TypeCheckVisitor::makeMethodVisitor(Type *declaredReturnTy, string &className) {
    TypeCheckVisitor funcVisitor(this);

    funcVisitor.declaredReturnType = declaredReturnTy;
    funcVisitor.returnable = true;
    funcVisitor.currentClass = className;

    return funcVisitor;
}

void TypeCheckVisitor::connectTree(AstNode *root) {
    for (const auto& child : root->get_children()) {
        child->parent = root;
        connectTree(child.get());
    }
}

void TypeCheckVisitor::dump() {
    int tab = 0;
    auto current = parent;
    while (current != nullptr) {
        ++tab; current = current->parent;
    }
}

void TypeCheckVisitor::DeclareClasses(CompoundStmt::SharedPtr root) {

    for (const auto& stmt : root->getStatements()) {
        if (stmt->get_type() == NodeType::CLASS_DECL) {
            auto cl_dec = std::static_pointer_cast<ClassDecl>(stmt);

            currentClassGenerics = &cl_dec->generics;

            DeclareClass(cl_dec.get());

            currentClassGenerics = nullptr;
        }
        if (stmt->get_type() == NodeType::NAMESPACE_DECL) {
            auto ns_dec = std::static_pointer_cast<NamespaceDecl>(stmt);
            pushNamespace(ns_dec->nsName);
            DeclareClasses(ns_dec->contents);
            popNamespace();
        }
        if (stmt->get_type() == NodeType::DECLARATION) {
            stmt->accept(*this);
        }
        if (stmt->get_type() == NodeType::USING_STMT) {
            stmt->accept(*this);
        }
        if (stmt->get_type() == NodeType::TYPEDEF_DECL) {
            stmt->accept(*this);
        }
        if (stmt->get_type() == NodeType::EOF_STMT) {
            stmt->accept(*this);
        }
    }

    for (const auto& stmt : root->getStatements()) {
        if (stmt->get_type() == NodeType::CLASS_DECL) {
            auto cl = std::static_pointer_cast<ClassDecl>(stmt);
            currentClassGenerics = &cl->generics;
            DeclareClassMethods(cl.get());
            currentClassGenerics = nullptr;
        }
        if (stmt->get_type() == NodeType::NAMESPACE_DECL) {
            auto ns_dec = std::static_pointer_cast<NamespaceDecl>(stmt);
            pushNamespace(ns_dec->nsName);
            DeclareClasses(ns_dec->contents);
            popNamespace();
        }
        if (stmt->get_type() == NodeType::FUNCTION_DECL) {
            DeclareFunction(std::static_pointer_cast<FunctionDecl>(stmt).get());
        }
        if (stmt->get_type() == NodeType::USING_STMT) {
            stmt->accept(*this);
        }
        if (stmt->get_type() == NodeType::EOF_STMT) {
            stmt->accept(*this);
        }
    }
}

/**
 * Declares a variable in the current context
 * @param name
 * @param type
 * @param cause
 */
string TypeCheckVisitor::declare_var(string& name, Type* type, bool global, AstNode *cause) {
    string var_name;
    if (global) {
        var_name = ns_prefix() + name;
    }
    else {
        var_name = name + scope;
    }

    if (SymbolTable::hasVariable(var_name, importedNamespaces)) {
        RuntimeError::raise(ERR_REDECLARED_VAR, "Redeclaration of variable " + name, cause);
    }

    if (global) {
        type = type->getPointerTo();
    }

    SymbolTable::declareVariable(var_name, type);

    return var_name;
}

/**
 * Declares a function (overload) in the current context
 * @param fun
 * @param args
 * @param ret
 * @param cause
 */
Type*& TypeCheckVisitor::declare_fun(
    Function::UniquePtr&& func,
    std::vector<GenericType*>& generics,
    AstNode* decl
) {
    auto overloads = SymbolTable::getFunction(func->getName(), currentNamespace.back());
    auto& args = func->getArgTypes();
    auto score = util::func_score(args);

    for (auto it = overloads.first; it != overloads.second; ++it) {
        auto& overload = it->second;

//        auto res = util::func_call_compatible(func->getArgTypes(), overload->getArgTypes(), generics,
//            overload->getGenerics());
//
//        if (res.perfect_match) {
//            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot redeclare function " + func->getName() + " with the same "
//                "signature as a previous declaration", decl);
//        }
    }

    auto& name = func->getName();
    auto& ret = func->getReturnType();

    SymbolTable::declareFunction(name, std::move(func));

    return ret;
}

/**
 * Pushes a type on the type stack
 * @param type
 */
void TypeCheckVisitor::pushTy(Type *type) {
    typeStack.push(type);
}

/**
 * Removes a type from the type stack and returns it
 * @return
 */
Type* TypeCheckVisitor::popTy() {
    auto top = typeStack.top();
    typeStack.pop();
    
    return top;
}

/**
 * Returns a variable if it exists, throws otherwise
 * @param ident
 * @param cause
 * @return
 */
std::pair<Type*, string> TypeCheckVisitor::get_var(
    string& ident, 
    AstNode* cause)
{
    auto current = this;
    auto ns = currentNamespace.back();
    string curr;

    while (current != nullptr) {
        curr = ident + current->scope;
        if (SymbolTable::hasVariable(curr)) {
            break;
        }

        current = current->parent;
    }

    if (current == nullptr) {
        if (SymbolTable::hasVariable(ident, importedNamespaces)) {
            return SymbolTable::getVariable(ident, importedNamespaces);
        }

        RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Reference to undeclared identifier " + ident, cause);
    }

    return SymbolTable::getVariable(curr, ns);
}

bool TypeCheckVisitor::has_var(string ident) {
    auto current = this;
    auto ns = currentNamespace.back();

    while (current != nullptr) {
        if (SymbolTable::hasVariable(ident + current->scope, ns)) {
            break;
        }

        current = current->parent;
    }

    if (current == nullptr) {
        return false;
    }

    return true;
}

/**
 * Checks for a matching function overload, throws if none or more than one is found
 * @param fun
 * @param args
 * @param cause
 * @return
 */
FunctionResult TypeCheckVisitor::get_fun(
    string& fun,
    std::vector<Type*>& args,
    std::vector<Type*>& generics
) {
    string context;
    auto overloads = SymbolTable::getFunction(fun, importedNamespaces);

    FunctionResult result;
    result.compatibility = CompatibilityType::FUNC_NOT_FOUND;

    int bestMatch = 0;

    if (overloads.first == overloads.second) {
        return result;
    }

    result.compatibility = CompatibilityType::NO_MATCHING_CALL;

    for (auto it = overloads.first; it != overloads.second; ++it) {
        auto& overload = it->second;
        std::vector<Type*> givenArgs;
        std::vector<Type*>& neededArgs = overload->getArgTypes();

        givenArgs.reserve(args.size());

        size_t i = 0;
        for (const auto& arg : args) {
            if (neededArgs.size() <= i || (!neededArgs.at(i)->isReference() && arg->isLvalue()
                && !arg->isCStyleArray()))
            {
                givenArgs.push_back(arg->toRvalue());
            }
            else {
                givenArgs.push_back(arg);
            }
        }

        auto res = util::func_call_compatible(givenArgs, neededArgs, generics, overload->getGenerics());

        if (res.is_compatible) {
            result.compatibility = CompatibilityType::COMPATIBLE;
        }

        if (res.perfect_match) {
            result.func = overload.get();
            result.neededCasts = res.needed_casts;

            return result;
        }

        if (res.is_compatible && res.compat_score >= bestMatch) {
            result.func = overload.get();
            result.neededCasts = res.needed_casts;
        }
    }

    return result;
}

void TypeCheckVisitor::ApplyCasts(
    std::vector<Expression::SharedPtr>& args,
    std::vector<Type*>& argTypes,
    unordered_map<size_t, Type *>& casts
) {
    assert(args.size() == argTypes.size() && "Incompatible arg count!");

    for (auto& _cast : casts) {
        assert(args.size() > _cast.first && "Invalid cast index!");
        wrapImplicitCast(args.at(_cast.first), argTypes.at(_cast.first), _cast.second);
    }
}

/**
 * Returns from a context
 * @param ret_type
 * @param cause
 */
void TypeCheckVisitor::return_(
    Type* ret_type,
    AstNode *cause
) {
    if (declaredReturnType != nullptr) {
        if (!ret_type->implicitlyCastableTo(declaredReturnType)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Returned value of type " + ret_type->toString() + " is not "
                "compatible with declared return type " + declaredReturnType->toString(), cause);
        }

        ++returned;
    }
    else {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'return' is only allowed in function bodies", cause);
    }
}

void TypeCheckVisitor::resolve(Type** ty) {

    // resolve unqalified generic types, for example:
    //   let x: Array<Int> will be parsed as having one generic Type Int,
    //   but the parser doesn't know that this corresponds to the generic
    //   parameter "T" of class Array
    Type::resolveUnqualified(*ty);

    if (isa<ObjectType>(*ty) && (*ty)->getClassName() == "Self" && !currentClass.empty()) {
        *ty = new GenericType("Self");
    }

    SymbolTable::resolveTypedef(*ty, importedNamespaces);

    for (const auto& cont : (*ty)->getTypeReferences()) {
        resolve(cont);
    }

    if (isa<PointerType>(*ty)) {
        auto asPtr = cast<PointerType>(*ty);
        auto& lengthExpr = asPtr->getLengthExpr();

        if (lengthExpr != nullptr) {
            auto indexTy = lengthExpr->accept(*this);

            Type* int64Ty = IntegerType::get(64);
            if (*indexTy != int64Ty) {
                wrapImplicitCast(lengthExpr, indexTy, int64Ty);
            }
            else {
                delete int64Ty;
            }
        }
    }

    if (isa<ObjectType>(*ty)) {
        auto asObj = cast<ObjectType>(*ty);
        if (SymbolTable::hasClass(asObj->getClassName()) && SymbolTable::getClass(asObj->getClassName())->isStruct()) {
            asObj->isLvalue(false);
            asObj->isStruct(true);
        }
    }
}

void TypeCheckVisitor::checkExistance(ObjectType *objTy, AstNode* cause) {
    if (!SymbolTable::hasClass(objTy->getClassName())) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Class " + objTy->getClassName() + " does not exist", cause);
    }
}

void TypeCheckVisitor::pushNamespace(string &ns) {
    auto newNs = currentNamespace.size() == 1 ? ns : currentNamespace.back() + "." + ns;
    currentNamespace.push_back(newNs);
    importedNamespaces.push_back(newNs + ".");

    SymbolTable::declareNamespace(newNs);
}

void TypeCheckVisitor::popNamespace() {
    importedNamespaces.pop_back();
    currentNamespace.pop_back();
}

Type* TypeCheckVisitor::ReturnMemberExpr(Expression *node, Type *ty) {

    if (node->memberExpr != nullptr) {
        if (ty->isLvalue()) {
            ty = ty->toRvalue();
            node->lvalueCast = true;
        }

        pushTy(ty);
        ty = node->memberExpr->accept(*this);
    }

    if (node->parentExpr == nullptr && !node->lvalue && ty->isLvalue()) {
        node->needsLvalueToRvalueConversion = true;
        ty = ty->toRvalue();
    }

    return ty;
}

void TypeCheckVisitor::wrapImplicitCast(Expression::SharedPtr& target, Type*& originTy, Type*& destTy) {
//    if (originTy->isLvalue() && !destTy->isLvalue()) {
//        originTy = originTy->toRvalue();
//    }

    auto cast = new ImplicitCastExpr(originTy, destTy, target);

    cast->setIndex(target->startIndex, target->endIndex, target->sourceFileId);
    target->setParent(cast);
    cast->children.push_back(&target);

    target.reset(cast);
}

/**
 * Visits all the nodes children
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(NamespaceDecl *node) {
    pushNamespace(node->nsName);
    node->contents->accept(*this);
    popNamespace();

    return nullptr;
}

/**
 * Visits all the nodes children
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(CompoundStmt *node) {
    auto prevUnsafe = currentBlockUnsafe;
    currentBlockUnsafe = node->isUnsafe_ || prevUnsafe;

    if (isNewlyCreated || node->preserveScope) {
        isNewlyCreated = false;
        for (const auto& child : node->get_children()) {
            child->accept(*this);
        }
    }
    else {
        TypeCheckVisitor t(this);
        for (const auto& child : node->get_children()) {
            child->accept(t);
        }
    }

    currentBlockUnsafe = prevUnsafe;
    return nullptr;
}

namespace {

    bool isReservedIdentifier(string& ident) {
        return (
            ident == "_"    ||
            ident == "Self"
        );
    }

}

void TypeCheckVisitor::DeclareFunction(FunctionDecl *node) {
    if (SymbolTable::hasClass(node->funcName)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot declare a function with the same name as a class declaration",
            node);
    }
    if (isReservedIdentifier(node->funcName)) {
        RuntimeError::raise(ERR_TYPE_ERROR, node->funcName + " is a reserved identifier", node);
    }

    node->returnType->accept(*this);
    auto& return_type = node->returnType->getType();
    return_type->isLvalue(return_type->isReference());

    auto qualified_name = ns_prefix() + node->funcName;
    if (qualified_name == "main") {
        if (!isa<IntegerType>(return_type) && !isa<VoidType>(return_type)) {
            Warning::issue("Declared return type of main function is always ignored", node);
        }

        return_type = IntegerType::get(64);
        node->returnType->setType(return_type);
    }

    Function::UniquePtr fun = std::make_unique<Function>(qualified_name, return_type, node->generics);

    if (return_type->isStruct()) {
        node->hasHiddenParam = true;
        fun->hasHiddenParam(true);
    }

    std::vector<Type*> argTypes;
    for (const auto &arg : node->args) {
        arg->accept(*this);
        auto& resolvedArg = arg->argType->getType();
        resolvedArg->isLvalue(arg->mut);
        argTypes.push_back(resolvedArg);

        fun->addArgument(resolvedArg, arg->defaultVal);
    }

    declare_fun(std::move(fun), node->generics, node);

    if (qualified_name == "main") {
        node->binding = qualified_name;
    }
    else {
        node->binding = SymbolTable::mangleFunction(qualified_name, argTypes);
    }
}

void TypeCheckVisitor::CopyNodeProperties(Expression *src, Expression *dst) {
    dst->isLhsOfAssigment_ = src->isLhsOfAssigment_;
    dst->setIndex(src->startIndex, src->endIndex, src->sourceFileId);

    if (src->isHiddenReturnValue_) {
        dst->isHiddenReturnValue();
    }
    else if (src->isReturnValue_) {
        dst->isReturnValue();
    }
}

/**
 * Checks if all code paths in the function return a value of the correct type and places the function in the symbol
 * table
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(FunctionDecl *node) {

    auto& return_type = node->returnType->getType();
    auto funcVisitor = makeFunctionVisitor(return_type);

    for (const auto& arg : node->args) {
        arg->binding = funcVisitor.declare_var(arg->argName, arg->argType->getType());
    }

    attributes = node->attributes;
    node->body->accept(funcVisitor);

    attributes.clear();

    node->captures = funcVisitor.captures;
    node->captureTypes = funcVisitor.captureTypes;
    node->copyTargets = funcVisitor.copy_targets;

    if (funcVisitor.branches - funcVisitor.returned > 0 && !return_type->isNullable() &&
        !isa<VoidType>(return_type) && node->funcName != "main")
    {
        RuntimeError::raise(ERR_TYPE_ERROR, "Not all code paths return a value", node);
    }
    // implicit 0 return for main function
    else if (funcVisitor.branches - funcVisitor.returned > 0 && node->funcName == "main") {
        return_type = IntegerType::get();
        node->body->implicitZeroReturn = true;
    }
    // implicit void return
    else if (funcVisitor.branches - funcVisitor.returned > 0) {
        node->body->addStatement(std::make_shared<ReturnStmt>());
    }

    return nullptr;
}

/**
 * Returns the type of a variable, if it exists
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(IdentifierRefExpr *node) {

    string ns_name = node->ident;

    for (const auto& ns : importedNamespaces) {
        auto curr = ns + ns_name;
        if (SymbolTable::isNamespace(curr)) {
            ns_name = curr;
            break;
        }
    }

    if (SymbolTable::isNamespace(ns_name)) {
        auto current = node->memberExpr;

        while (current != nullptr && current->get_type() == NodeType::MEMBER_EXPR) {
            auto new_ns = ns_name + "." + current->ident;

            if (!SymbolTable::isNamespace(new_ns)) {
                break;
            }

            ns_name = new_ns;
            current = current->memberExpr;
        }

        node->isNamespace = true;
        node->memberExpr = current;

        if (node->memberExpr == nullptr) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot use a namespace as a value", node);
        }

        if (node->memberExpr->get_type() == NodeType::MEMBER_EXPR) {
            auto member_expr = std::static_pointer_cast<MemberRefExpr>(node->memberExpr);
            member_expr->isNsMember = true;
            member_expr->className = ns_name;
            member_expr->ident = ns_name + "." + member_expr->ident;
        }
        else if (node->memberExpr->get_type() == NodeType::CALL_EXPR) {
            auto member_expr = std::static_pointer_cast<CallExpr>(node->memberExpr);
            member_expr->ident = ns_name + "." + member_expr->ident;
            member_expr->type = CallType::FUNC_CALL;
        }
        else if (node->memberExpr->get_type() == NodeType::ARRAY_ACCESS_EXPR) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot access index on a namespace", node);
        }
        else {
            assert(false && "Unknown operation");
        }

        return node->memberExpr->accept(*this);
    }

    if (node->ident == "super") {
        if (currentClass.empty()) {
            RuntimeError::raise(ERR_CONTEXT_ERROR, "'super' can only be used in instance methods", node);
        }

        auto currentCl = SymbolTable::getClass(currentClass);
        if (currentCl->get_parent() == nullptr) {
            RuntimeError::raise(ERR_CONTEXT_ERROR, "Class " + currentClass + " does not have a base class", node);
        }

        node->binding = currentSelf;
        node->ident = "self";
        node->isSuper = true;
        node->superClassName = currentCl->get_parent()->getName();

        auto type = ObjectType::get(node->superClassName)->toRvalue();
        return ReturnMemberExpr(node, type);
    }

    bool implicit_this = node->ident != "self" && !has_var(node->ident) && !currentClass.empty();
    if (node->ident == "self" || implicit_this) {
        if (currentClass.empty()) {
            RuntimeError::raise(ERR_CONTEXT_ERROR, "'this' can only be used in instance methods", node);
        }

        if (implicit_this) {
            auto mem_ref = std::make_shared<MemberRefExpr>(node->ident);
            CopyNodeProperties(node, mem_ref.get());
            mem_ref->parent = node;

            mem_ref->setMemberExpr(node->memberExpr);
            node->setMemberExpr(mem_ref);
        }

        node->binding = currentSelf;
        node->ident = "self";

        //TODO check if 'self' is mutable
        auto type = SymbolTable::getClass(currentClass)->getType()->toRvalue();

        return ReturnMemberExpr(node, type);
    }

    auto res = get_var(node->ident, node);

    // add capture for this variable
    if (isNestedFunction && res.second != scope) {
        captureTypes.push_back(res.first);
        captures.push_back({node->ident, SymbolTable::mangleVariable(node->ident, res.second)});
        copy_targets.push_back(SymbolTable::mangleVariable(node->ident, res.second));
        node->isCapturedVar = true;

        node->binding = currentSelf;

        return res.first;
    }
    else {
        node->bind(res.second);
     }

    node->declaration = declarations[node->binding];

    return ReturnMemberExpr(node, res.first);
}

/**
 * Declares one or more variables
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(DeclStmt *node) {
    if (node->declared) {
        return nullptr;
    }

    auto& ident = node->identifier;

    if (isReservedIdentifier(ident)) {
        RuntimeError::raise(ERR_TYPE_ERROR, ident + " is a reserved identifier", node);
    }

    node->type->accept(*this);
    auto& decl_type = node->type->getType();
    auto& val = node->value;

    if (decl_type->isUnsafePointer() && !currentBlockUnsafe) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Raw Pointer types and C-Style arrays are only allowed in 'unsafe' "
            "blocks", node);
    }

    if (val != nullptr) {

        val->isGlobal(node->is_global);
        val->setInferredType(decl_type);

        pushTy(decl_type);

        if (val->get_type() == NodeType::LAMBDA_EXPR) {
            std::static_pointer_cast<LambdaExpr>(val)->self_ident = ident;
        }

        Type* given_type = val->accept(*this);
        given_type->isConst(node->is_const);

        // in case it has changed
        decl_type = node->type->getType();

        if (decl_type->isInferred()) {
            if (given_type->isNull()) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Null requires a contextual type", val.get());
            }

            delete decl_type;
            decl_type = given_type;
        }
        else if (isa<VoidType>(given_type) && !decl_type->isNullable()) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign null to non-nullable variable of type " +
                decl_type->toString(), val.get());
        }
        else if (!given_type->implicitlyCastableTo(decl_type)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible types " + given_type->toString() + " and " +
                decl_type->toString(), val.get());
        }

        if (*decl_type != given_type) {
            wrapImplicitCast(node->value, given_type, decl_type);
        }
    }
    else if (!decl_type->isNullable() && !isa<PrimitiveType>(decl_type) && !isa<PointerType>(decl_type)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Expected non-nullable variable " + ident + " to be defined",
            node);
    }

    resolve(&decl_type);

    auto allocType = decl_type;

    if (isa<ObjectType>(decl_type) && cast<ObjectType>(decl_type)->isStruct()) {
        node->isStructAlloca = true;
    }
    else if (decl_type->isCStyleArray() && cast<PointerType>(decl_type)->getPointeeType()->isStruct()) {
        node->isStructAlloca = true;
    }
    else if (!node->is_global) {
        allocType = allocType->getPointerTo();
    }

    node->binding = declare_var(ident, allocType, node->is_global, node);
    declarations.emplace(node->binding, node);

    node->declared = true;
    return nullptr;
}

/**
 * Checks a for statement
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(ForStmt *node) {
    TypeCheckVisitor init_visitor(this);
    if (node->initialization) {
        node->initialization->accept(init_visitor);
    }

    if (node->increment) {
        node->increment->accept(init_visitor);
    }

    if (node->termination) {
        node->termination->accept(init_visitor);
    }

    if (node->body) {
        TypeCheckVisitor body_visitor(&init_visitor);
        body_visitor.continuable = true;
        body_visitor.breakable = true;

        node->body->accept(body_visitor);
    }

    return nullptr;
}

/**
 * Checks a while statement
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(WhileStmt *node) {
    node->condition->accept(*this);
    TypeCheckVisitor body_visitor(this);
    body_visitor.continuable = true;
    body_visitor.breakable = true;

    node->body->accept(body_visitor);

    return nullptr;
}

pair<Type*, std::vector<Type*>> TypeCheckVisitor::unify(std::vector<Expression::SharedPtr>& types) {
    Type* unified = nullptr;
    std::vector<Type*> evaledTypes;
    evaledTypes.reserve(types.size());

    bool anyCompatible = false;

    for (auto& expr : types) {
        auto type = expr->accept(*this);
        evaledTypes.push_back(type);

        if (unified == nullptr) {
            unified = type;
            continue;
        }

        if (!unified->implicitlyCastableTo(type)) {
            if (!type->implicitlyCastableTo(unified)) {
                unified = ObjectType::get("Any");
                anyCompatible = true;
            }
        }
        else if (!anyCompatible) {
            unified = type;
        }
    }

    size_t i = 0;
    for (auto& expr : types) {
        wrapImplicitCast(expr, evaledTypes.at(i), unified);
        ++i;
    }

    return { unified, evaledTypes };
}

/**
 * Checks an array literal
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(CollectionLiteral *node) {

    auto isInferred = node->inferredType != nullptr;
    if (!isInferred || node->inferredType->isInferred()) {
        Type* elTy;
        if (node->elements.empty()) {
            elTy = ObjectType::get("Any")->getPointerTo();
        }
        else {
            elTy = unify(node->elements).first;
        }

        if (node->hasAttribute(Attr::CArray)) {
            auto ty = elTy->getPointerTo();
            ty->isCStyleArray(true);
            ty->isLvalue(false);

            node->type = std::make_shared<TypeRef>(ty);
            node->type->accept(*this);
        }
        else {
            node->type = std::make_shared<TypeRef>((new CollectionType(elTy))->toRvalue());
            node->type->accept(*this);
            node->type->getType()->isLvalue(false);
        }

        return ReturnMemberExpr(node, node->type->getType());
    }

    if (!isa<PointerType>(node->inferredType) &&
        !isa<CollectionType>(node->inferredType))
    {
        RuntimeError::raise(ERR_TYPE_ERROR, "Collection literal cannot return value of type " +
            node->inferredType->toString(), node);
    }

    bool isCarray = false;
    Type* elType;
    if (isa<PointerType>(node->inferredType)) {
        auto ptrType = cast<PointerType>(node->inferredType);
        ptrType->isCStyleArray(true);
        elType = ptrType->getPointeeType();
        isCarray = true;
    }
    else {
        auto ptrType = cast<CollectionType>(node->inferredType);
        elType = ptrType->getConcreteGeneric("T");
    }

    if (!node->elements.empty()) {
        auto givenType = unify(node->elements).first;

        if (!givenType->implicitlyCastableTo(elType)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible collection elements: Expected " + elType->toString() +
                ", but got " + givenType->toString(), node);
        } else if (givenType->isBoxedEquivOf(elType)) {
            elType = givenType;
        } else if (*elType != givenType) {
            for (auto &el : node->elements) {
                wrapImplicitCast(el, givenType, elType);
            }
        }
    }

    auto ty = elType->getPointerTo();
    ty->isCStyleArray(isa<PointerType>(node->inferredType));

    if (isCarray) {
        node->type = std::make_shared<TypeRef>(ty);
    }
    else {
        node->type = std::make_shared<TypeRef>(node->inferredType);
    }

    node->type->accept(*this);
    node->type->getType()->isLvalue(false);

    if (node->declaration != nullptr) {
        node->declaration->type = node->type;
    }

    return ReturnMemberExpr(node, node->type->getType());
}

/**
 * Returns the type of the literal
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(LiteralExpr *node) {

    if ((node->memberExpr == nullptr && node->inferredType != nullptr && isa<PrimitiveType>(node->inferredType)) ||
        node->hasAttribute(Attr::Primitive)
    ) {
        node->isPrimitive = true;
        return node->type;
    }

    switch (node->type->getTypeID()) {
        case TypeID::IntegerTypeID: {
            auto asInt = cast<IntegerType>(node->type);
            string className = "Int";
            if (asInt->isUnsigned()) {
                className = "U" + className;
            }

            auto bitWidth = asInt->getBitwidth();
            if (bitWidth != (sizeof(int *) * 8)) {
                className += std::to_string(bitWidth);
            }

            if (className == "Int8") {
                className = "Char";
            }

            if (className == "Int1") {
                className = "Bool";
            }

            node->className = className;
            break;
        }
        case TypeID::FPTypeID: {
            auto asFloat = cast<FPType>(node->type);
            node->className = asFloat->getPrecision() == 64 ? "Double" : "Float";
            break;
        }
        case TypeID::VoidTypeID: {
            node->isPrimitive = true;
            return node->type;
        }
        default:
            llvm_unreachable("Unknown literal type");
    }

    // TODO remove this
    if (!SymbolTable::hasClass(node->className)) {
        node->isPrimitive = true;
        return node->type;
    }

    node->constructor = SymbolTable::getClass(node->className)
        ->getMemberwiseInitializer()->mangledName;

    auto ty = ObjectType::get(node->className)->toRvalue();

    return ReturnMemberExpr(node, ty);
}

/**
 * Returns type string
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(StringLiteral *node) {

    for (const auto& attr : node->attributes) {
        switch (attr.kind) {
            case Attr::CString:
                node->raw = true;
                break;
            default:
                RuntimeError::raise(ERR_TYPE_ERROR, "Attribute " + attr.name + " is not applicable on string literals",
                    node);
        }
    }

    if (node->inferredType != nullptr && isa<PointerType>(node->inferredType)) {
        node->raw = true;
    }

    if (node->raw) {
        auto ty = new PointerType(IntegerType::get(8), node->value.length());
        ty->isCStyleArray(true);

        return ty;
    }

    auto str = ObjectType::get("String")->toRvalue();

    return ReturnMemberExpr(node, str);
}

/**
 * Returns the arrays type
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(SubscriptExpr *node) {

    auto ts = popTy()->deepCopy();
    resolve(&ts);

    bool final_return_lval = false;

    auto current = node->parentExpr;
    while (current != nullptr) {
        final_return_lval = current->lvalue;
        current = current->parentExpr;
    }

    if (!isa<ObjectType>(ts)) {
        node->_index->setInferredType(IntegerType::get(64));
    }

    Type* index = node->_index->accept(*this);

    if (isa<ObjectType>(ts)) {
        auto& className = ts->getClassName();

        if (!SymbolTable::hasClass(className)) {
            goto end;
        }
        
        auto cl = SymbolTable::getClass(className, importedNamespaces);
        cdot::cl::MethodResult methodResult;

        std::vector<Type*> argTypes{ index };
        string op;

        if (final_return_lval) {
            op = "postfix []&";
            methodResult = cl->hasMethod(op, argTypes);
        }
        else {
            try_rvalue_op:
            op = "postfix []";
            methodResult = cl->hasMethod(op, argTypes);
        }

        if (methodResult.compatibility == CompatibilityType::COMPATIBLE) {
            auto call = std::make_shared<CallExpr>(CallType::METHOD_CALL, std::vector<Expression::SharedPtr>{
                node->_index }, op);
            ApplyCasts(call->args, argTypes, methodResult.neededCasts);

            call->setIndex(node->startIndex, node->endIndex, node->sourceFileId);
            call->parentExpr = node;
            call->parent = node;

            call->argTypes.push_back(index);

            auto expr = std::static_pointer_cast<Expression>(call);
            node->children.push_back(&expr);

            node->overridenCall = call;
            node->isSubscriptOp = true;

            pushTy(ts);
            auto type = call->accept(*this);

            return ReturnMemberExpr(node, type);
        }
        else if (final_return_lval && op != "postfix []") {
            goto try_rvalue_op;
        }
        else {
            RuntimeError::raise(ERR_TYPE_ERROR, "Operator [](" + index->toString() + ") is not defined on class " +
                ts->toString(), node);
        }
    }

    end:
    if (!isa<PointerType>(ts)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Operator [](" + index->toString() + ") is not defined on type " +
            ts->toString(), node);
    }

    Type* int64Ty = IntegerType::get(64);
    if (!index->implicitlyCastableTo(int64Ty)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Array indices have to be integral, " + index->toString() + " given", node);
    }
    else if (*index != int64Ty) {
        wrapImplicitCast(node->_index, index, int64Ty);
    }
    else {
        delete int64Ty;
    }

    ts->isCStyleArray(false);
    if (cast<PointerType>(ts)->getPointeeType()->isStruct()) {
        ts = cast<PointerType>(ts)->getPointeeType();
        ts->isLvalue(false);
    }

    return ReturnMemberExpr(node, ts);
}

void TypeCheckVisitor::PrepareCallArgs(
    std::vector<Expression::SharedPtr> &args,
    std::vector<Type*> &declaredArgTypes,
    std::vector<Type*> &argTypes
) {
    size_t i = 0;
    for (const auto& arg : args) {
        if (i >= argTypes.size() || i >= declaredArgTypes.size()) {
            break;
        }

        if (declaredArgTypes.at(i)->isStruct() && argTypes.at(i)->isStruct() && !declaredArgTypes.at(i)->isReference()) {
            arg->needsByValPass = true;
        }

        if (!declaredArgTypes.at(i)->isReference() && argTypes.at(i)->isLvalue()) {
            arg->needsLvalueToRvalueConversion = true;
        }

        ++i;
    }
}

void TypeCheckVisitor::PrepareCallArgs(
    std::vector<Expression::SharedPtr> &args,
    std::vector<Type *> &declaredArgTypes,
    std::vector<Type*>& argTypes,
    std::vector<Expression::SharedPtr> &defaultValues
) {
    size_t i = 0;
    while (args.size() < declaredArgTypes.size()) {
        auto defVal = defaultValues[args.size() + i];
        if (defVal == nullptr) {
            break;
        }

        args.push_back(defVal);
        ++i;
    }

    PrepareCallArgs(args, declaredArgTypes, argTypes);
}

void TypeCheckVisitor::HandleFunctionCall(CallExpr *node) {

    auto& argTypes = node->argTypes;
    auto result = get_fun(node->ident, argTypes, node->generics);

    if (result.compatibility != CompatibilityType::COMPATIBLE) {

        // possible implicit method call
        if (!currentClass.empty()) {
            auto cl = SymbolTable::getClass(currentClass);
            auto compat = cl->hasMethod(node->ident, argTypes);
            if (compat.compatibility == CompatibilityType::COMPATIBLE) {
                pushTy(ObjectType::get(currentClass));

                node->type = CallType::METHOD_CALL;
                node->implicitSelfCall = true;
                node->selfBinding = currentSelf;

                return HandleMethodCall(node);
            }
        }

        if (result.compatibility == CompatibilityType::NO_MATCHING_CALL) {
            RuntimeError::raise(ERR_UNDECLARED_VARIABLE,
                "No matching call for function " + node->ident + " found", node);
        }
        if (result.compatibility == CompatibilityType::FUNC_NOT_FOUND) {
            RuntimeError::raise(ERR_UNDECLARED_VARIABLE,
                "Function " + node->ident + " does not exist", node);
        }

        assert(false && "Well, what happened there?");
    }

    ApplyCasts(node->args, argTypes, result.neededCasts);

    auto& func = result.func;

    auto& declaredArgTypes = func->getArgTypes();
    auto& defaultValues = func->getArgDefaults();

    PrepareCallArgs(node->args, declaredArgTypes, argTypes, defaultValues);
    node->binding = SymbolTable::mangleFunction(func->getName(), func->getArgTypes());

    if (func->hasHiddenParam()) {
        node->hasHiddenParamReturn = true;
        node->hiddenParamType = func->getReturnType();
    }

    node->returnType = func->getReturnType();
}

void TypeCheckVisitor::HandleMethodCall(CallExpr *node) {
    if (typeStack.empty()) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot call method on given value", node);
    }

    auto latest = popTy();
    auto& argTypes = node->argTypes;
    if (!isa<ObjectType>(latest) && !isa<GenericType>(latest)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Class " + latest->toString() + " does not exist", node);
    }

    auto& className = latest->getClassName();
    if (!SymbolTable::hasClass(className)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Class " + latest->toString() + " does not exist", node);
    }

    cdot::cl::Class* cl = SymbolTable::getClass(className, importedNamespaces);
    node->class_name = className;

    if (cl->isProtocol()) {
        node->castFrom = latest;
        node->reverseProtoShift = true;
    }

    auto methodResult = cl->hasMethod(node->ident, argTypes);
    if (methodResult.compatibility != CompatibilityType::COMPATIBLE) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Method " + node->ident + " does not exist on class " +
            className, node);
    }

    ApplyCasts(node->args, argTypes, methodResult.neededCasts);

    cdot::cl::Method* method = cl->getMethod(methodResult.method->mangledName);
    if (method->returnType->isGeneric()) {
        auto ret = method->returnType->deepCopy();
        node->genericReturnType = ret;
        bool isReference = ret->isReference();

        Type::resolveGeneric(&ret, latest->getConcreteGenericTypes());

        ret->isReference(isReference);
        node->returnType = ret;
        node->needsGenericCast = true;
    }
    else {
        node->returnType = method->returnType->deepCopy();
    }

    node->returnType->isLvalue(method->returnType->isReference());
    node->binding = method->mangledName;
    node->method = method;
    node->is_virtual = cl->isAbstract() || cl->is_virtual(method);
    node->isStatic = method->isStatic;

    if (method->accessModifier == AccessModifier::PROTECTED && !cl->protectedPropAccessibleFrom(currentClass))
    {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Protected method " + method->methodName + " of class " +
            className + " is not accessible", node);
    }
    else if (method->accessModifier == AccessModifier::PRIVATE && !cl->privatePropAccessibleFrom(currentClass))
    {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Private method " + method->methodName + " of class " +
            className + " is not accessible", node);
    }

    auto& declaredArgTypes = method->argumentTypes;
    auto& defaultValues = method->argumentDefaults;

    PrepareCallArgs(node->args, declaredArgTypes, argTypes, defaultValues);

    if (method->hasHiddenParam) {
        node->hasHiddenParamReturn = true;
        node->hiddenParamType = method->returnType;
    }
}

void TypeCheckVisitor::HandleConstructorCall(CallExpr *node) {

    auto& argTypes = node->argTypes;
    auto cl = SymbolTable::getClass(node->ident, importedNamespaces);

    auto constrResult = cl->hasMethod("init", argTypes, node->generics);
    if (constrResult.compatibility != CompatibilityType::COMPATIBLE) {
        if (node->generics.size() != cl->getGenerics().size()) {
            auto missingGeneric = cl->getGenerics().at(node->generics.size());

            RuntimeError::raise(ERR_TYPE_ERROR, "Could not infer generic type " +
                missingGeneric->getGenericClassName() + " of class " + node->ident +  " from context", node);
        }

        RuntimeError::raise(ERR_TYPE_ERROR, "Class " + node->ident + " does not define a constructor with "
            "given signature", node);
    }

    ApplyCasts(node->args, argTypes, constrResult.neededCasts);

    auto& method = constrResult.method;

    if (method->accessModifier == AccessModifier::PROTECTED &&
        !cl->protectedPropAccessibleFrom(currentClass))
    {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Protected method " + method->methodName + " of class " +
            node->ident + " is not accessible", node);
    }
    else if (method->accessModifier == AccessModifier::PRIVATE &&
        !cl->privatePropAccessibleFrom(currentClass))
    {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Private method " + method->methodName + " of class " +
            node->ident + " is not accessible", node);
    }

    if (cl->isAbstract()) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot instantiate abstract class " + node->ident, node);
    }

    auto concreteClassGenerics = cl->getConcreteGenerics();
    ObjectType* returnType;

    if (cl->isGeneric()) {
        auto& cl_gen = cl->getGenerics();
        returnType = ObjectType::get(node->ident);

        size_t i = 0;
        for (const auto& gen : cl_gen) {
            returnType->specifyGenericType(gen->getGenericClassName(), node->generics.at(i++));
        }

        returnType = cast<ObjectType>(returnType->toRvalue());
    }
    else {
        returnType = cast<ObjectType>(method->returnType->deepCopy());
    }

    returnType->getConcreteGenericTypes().insert(concreteClassGenerics.begin(), concreteClassGenerics.end());
    node->returnType = returnType;

    node->type = CallType::CONSTR_CALL;
    node->binding = method->mangledName;

    auto& declaredArgTypes = method->argumentTypes;
    auto& defaultValues = method->argumentDefaults;

    PrepareCallArgs(node->args, declaredArgTypes, argTypes, defaultValues);
}

void TypeCheckVisitor::HandleCallOperator(CallExpr *node) {

    auto latest = popTy();
    if (!isa<ObjectType>(latest) || !SymbolTable::hasClass(latest->getClassName())) {
        return HandleAnonCall(node);
    }

    auto& className = latest->getClassName();
    auto cl = SymbolTable::getClass(className, importedNamespaces);

    auto& argTypes = node->argTypes;
    auto callOpResult = cl->hasMethod("postfix ()", argTypes);

    if (callOpResult.compatibility != CompatibilityType::COMPATIBLE) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Call operator with signature " + util::args_to_string(argTypes) + " does"
                " not exist on class " + className, node);
    }

    ApplyCasts(node->args, argTypes, callOpResult.neededCasts);

    auto& method = callOpResult.method;

    node->type = CallType::METHOD_CALL;
    node->ident = "postfix ()";
    node->binding = method->mangledName;

    auto& declaredArgTypes = method->argumentTypes;
    auto& defaultValues = method->argumentDefaults;

    PrepareCallArgs(node->args, declaredArgTypes, argTypes, defaultValues);

    node->returnType = method->returnType;
}

void TypeCheckVisitor::HandleAnonCall(CallExpr *node) {

    auto latest = popTy();
    auto& argTypes = node->argTypes;

    if (isa<PointerType>(latest)) {
        latest = cast<PointerType>(latest)->getPointeeType();
    }

    if (!isa<FunctionType>(latest)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Given object of type " + latest->toString() + " is not callable", node);
    }

    node->type = CallType::ANON_CALL;
    auto asFun = cast<FunctionType>(latest);

    auto& declaredArgTypes = asFun->getArgTypes();
    PrepareCallArgs(node->args, declaredArgTypes, argTypes);

    node->returnType = asFun->getReturnType();
}

/**
 * Checks a function call for the validity of the arguments
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(CallExpr *node) {

    auto& givenArgs = node->argTypes;
    givenArgs.reserve(node->args.size());

    for (size_t i = givenArgs.size(); i < node->args.size(); ++i) {
        const auto& arg = node->args.at(i);
        arg->returnLvalue(true);

        auto argTy = arg->accept(*this);
        givenArgs.push_back(argTy);
    }

    if (node->type == CallType::FUNC_CALL) {
        if (node->ident.empty()) {
            HandleCallOperator(node);
        }
        else if (SymbolTable::hasClass(node->ident)) {
            HandleConstructorCall(node);
        }
        else {
            HandleFunctionCall(node);
        }
    }
    else if (node->type == CallType::METHOD_CALL) {
        HandleMethodCall(node);
    }

    if (node->returnType->isReference()) {
        Expression* current = node;
        while (current->parentExpr != nullptr) {
            current = current->parentExpr;
        }

        current->needsLvalueToRvalueConversion = false;
    }

    return ReturnMemberExpr(node, node->returnType);
}

/**
 * Checks if a class member exists and is of correct type
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(MemberRefExpr *node) {

    Type* latest;
    string className;
    cdot::cl::Class* cl;

    if (node->isNsMember) {
        if (!has_var(node->ident) && SymbolTable::hasClass(node->className)) {
            cl = SymbolTable::getClass(node->className);
            latest = cl->getType();
            node->ident = node->ident.substr(node->ident.find_last_of('.') + 1);
            goto implicit_method_call;
        }

        auto var = get_var(node->ident, node);
        node->binding = var.second;

        return var.first;
    }

    latest = popTy();
    className = latest->getClassName();
    if (className.empty()) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot access property " + node->ident + " on value of type "
            + latest->toString(), node);
    }

    cl = SymbolTable::getClass(className, importedNamespaces);
    node->className = className;

    if (cl->hasField(node->ident)) {
        cdot::cl::Field *field = cl->getField(node->ident);

        if (field->hasGetter && !node->isLhsOfAssigment_ && currentClass != className) {
            auto call = std::make_shared<CallExpr>(
                CallType::METHOD_CALL,
                std::vector<Expression::SharedPtr>{},
                field->getterName

            );
            call->setMemberExpr(node->memberExpr);
            call->setIndex(node->startIndex, node->endIndex, node->sourceFileId);
            call->setParent(node->parent);

            node->getterOrSetterCall = call;

            pushTy(latest);
            return call->accept(*this);
        }

        if (field->hasSetter && node->isLhsOfAssigment_ && currentClass != className) {
            Expression* current = node;
            node->isSetterCall = true;

            while (current->parentExpr != nullptr) {
                current = current->parentExpr;
            }

            current->isSetterCall = true;
            current->setterName = field->setterName;

            return latest;
        }

        if (currentClass == className && node->isLhsOfAssigment_ && uninitializedFields != nullptr) {
            auto index = std::find(uninitializedFields->begin(), uninitializedFields->end(), node->ident);
            if (index != uninitializedFields->end()) {
                uninitializedFields->erase(index);
            }
        }

        auto& field_type = field->fieldType;
        auto& concreteGenerics = latest->getConcreteGenericTypes();

        // if this is called inside of the class, we don't have any concrete generics yet
        if (!concreteGenerics.empty() && field_type->isGeneric()) {
            auto fieldTy = field_type->deepCopy();
            node->genericReturnType = field_type;

            Type::resolveGeneric(&fieldTy, concreteGenerics);

            node->fieldType = fieldTy;
            node->needsGenericCast = true;
        }
        else {
            node->fieldType = field->fieldType->deepCopy();
        }

        node->binding = field->mangledName;

        if (field->accessModifier == AccessModifier::PROTECTED && !cl->protectedPropAccessibleFrom(currentClass)) {
            RuntimeError::raise(ERR_CONTEXT_ERROR, "Protected field " + field->fieldName + " of class " +
                className + " is not accessible", node);
        }
        else if (field->accessModifier == AccessModifier::PRIVATE && !cl->privatePropAccessibleFrom(currentClass)) {
            RuntimeError::raise(ERR_CONTEXT_ERROR, "Private field " + field->fieldName + " of class " +
                className + " is not accessible", node);
        }

        Type* resTy;
        if (node->fieldType->isStruct()) {
            resTy = node->fieldType;
        }
        else {
            resTy = node->fieldType->getPointerTo();
        }

        return ReturnMemberExpr(node, resTy);
    }
    // method call with omitted parens
    else {
        implicit_method_call:
        auto possibleMethod = cl->hasMethod(node->ident, {});
        if (possibleMethod.compatibility == CompatibilityType::COMPATIBLE) {
            Expression::SharedPtr call = std::make_shared<CallExpr>(CallType::METHOD_CALL,
                std::vector<Expression::SharedPtr>{}, node->ident
            );

            CopyNodeProperties(node, call.get());
            pushTy(latest);

            node->parentExpr->memberExpr = call;

            return call->accept(*this);
        }
    }

    RuntimeError::raise(ERR_TYPE_ERROR, "Field " + node->ident + " does not exist on class " + className, node);
    return nullptr;
}

Type* TypeCheckVisitor::HandleCastOp(Type *fst, BinaryOperator *node) {
    if (node->rhs->get_type() != NodeType::TYPE_REF) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Expected type name after 'as'", node->rhs.get());
    }

    auto toTypeRef = std::static_pointer_cast<TypeRef>(node->rhs);
    toTypeRef->accept(*this);

    auto& toType = toTypeRef->getType();
    node->operandType = fst;

    if (toType->isUnsafePointer() && !currentBlockUnsafe) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Raw Pointer types and C-Style arrays are only allowed inside unsafe "
            "blocks", node);
    }

    if (isa<ObjectType>(fst)) {
        auto& className = fst->getClassName();
        auto op = "infix as " + toType->toString();

        std::vector<Type*> argTypes;
        auto class_decl = SymbolTable::getClass(className, importedNamespaces);
        auto castOp = class_decl->hasMethod(op, argTypes);

        if (castOp.compatibility == CompatibilityType::COMPATIBLE) {
            auto call = std::make_shared<CallExpr>(CallType::METHOD_CALL, std::vector<Expression::SharedPtr>{}, op);
            CopyNodeProperties(node, call.get());

            node->overridenCall = call;
            call->memberExpr = node->memberExpr;

            pushTy(fst);
            auto res = call->accept(*this);

            return res;
        }
    }

    if (node->op != "as!" && !fst->explicitlyCastableTo(toType)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot cast from " + fst->toString() + " to " +
            toType->toString(), node);
    }

    return toType;
}

Type* TypeCheckVisitor::HandleAssignmentOp(Type *fst, Type *snd, BinaryOperator *node) {

    if (!snd->implicitlyCastableTo(fst)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign value of type " + snd->toString() + " to variable of "
            "type " + fst->toString(), node->lhs.get());
    }
    else if (*fst != snd) {
        Warning::issue("Implicit cast from " + snd->toString() + " to " + fst->toString(), node);
        wrapImplicitCast(node->rhs, snd, fst);
    }

    auto prevOp = node->op;
    auto op = util::isAssignmentOperator(prevOp);

    if (op != "=") {
        node->op = op;
        HandleBinaryOperator(fst, snd, cdot::getBinaryOpType(op), node);
        node->op = prevOp;
    }

    node->lhs->returnLvalue(true);
    node->operandType = fst;
    return new VoidType;
}

Type* TypeCheckVisitor::HandleArithmeticOp(Type *fst, Type *snd, BinaryOperator *node) {
    auto& op = node->op;

    if ((op == "+" || op == "-") && (!isa<PrimitiveType>(fst) || !isa<PrimitiveType>(snd))) {
        if (!currentBlockUnsafe) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Pointer arithmetic is only allowed in unsafe blocks", node);
        }

        Type* ptr;
        Type* intgr;

        if (fst->isLvalue() && snd->isLvalue()) {
            goto err;
        }

        if (fst->isLvalue()) {
            ptr = fst;
            intgr = snd;
        }
        else if (snd->isLvalue()) {
            ptr = snd;
            intgr = fst;
        }
        else {
            goto err;
        }

        Type* int64Ty = IntegerType::get(64);
        if (*ptr != int64Ty) {
            wrapImplicitCast(node->lhs, fst, int64Ty);
        }
        if (*intgr != int64Ty) {
            wrapImplicitCast(node->rhs, snd, int64Ty);
        }

        node->pointerArithmeticType = ptr;
        node->operandType = int64Ty;

        return ptr;
    }

    if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%") {
        if ((!isa<PrimitiveType>(fst) || !isa<PrimitiveType>(snd))) {
            goto err;
        }

        Type* ret_type = cast<PrimitiveType>(fst)->ArithmeticReturnType(op, snd);

        if (isa<VoidType>(ret_type)) {
            goto err;
        }

        if (*fst != ret_type) {
            wrapImplicitCast(node->lhs, fst, ret_type);
        }
        if (*snd != ret_type) {
            wrapImplicitCast(node->rhs, snd, ret_type);
        }

        ret_type->isLvalue(false);
        node->operandType = ret_type;

        return ret_type;
    }

    if (op == "**") {
        Type* int64Ty = IntegerType::get(64);

        if (!snd->implicitlyCastableTo(int64Ty)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Right hand side of '**' must be of type integer",
                node->rhs.get());
        }

        Type* retType = FPType::getDoubleTy();

        if (*fst != retType) {
            wrapImplicitCast(node->lhs, fst, retType);
        }
        if (*snd != int64Ty) {
            wrapImplicitCast(node->rhs, snd, int64Ty);
        }
        else {
            delete int64Ty;
        }

        retType->isLvalue(false);
        node->operandType = retType;

        return retType;
    }

    err:
    RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
        ->toString() + " and " + snd->toString(), node);
    return nullptr;
}

Type* TypeCheckVisitor::HandleBitwiseOp(Type *fst, Type *snd, BinaryOperator *node) {
    auto& op = node->op;
    Type* int64Ty = IntegerType::get(64);

    if (!fst->implicitlyCastableTo(int64Ty)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
            ->toString() + " and " + snd->toString(), node);
    }
    if (!snd->implicitlyCastableTo(int64Ty)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
            ->toString() + " and " + snd->toString(), node);
    }

    if (*fst != int64Ty) {
        wrapImplicitCast(node->lhs, fst, int64Ty);
    }
    if (*snd != int64Ty) {
        wrapImplicitCast(node->rhs, snd, int64Ty);
    }

    int64Ty->isLvalue(false);
    node->operandType = int64Ty;

    return int64Ty;
}

Type* TypeCheckVisitor::HandleLogicalOp(Type *fst, Type *snd, BinaryOperator *node) {
    Type* boolTy = IntegerType::get(1);

    if (fst != boolTy || snd != boolTy) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + node->op + "' values of type " + fst
            ->toString() + " and " + snd->toString(), node);
    }

    if (*fst != boolTy) {
        wrapImplicitCast(node->lhs, fst, boolTy);
    }
    if (*snd != boolTy) {
        wrapImplicitCast(node->rhs, snd, boolTy);
    }

    boolTy->isLvalue(false);
    node->operandType = boolTy;

    return boolTy;
}

Type* TypeCheckVisitor::HandleEqualityOp(Type *fst, Type *snd, BinaryOperator *node) {
    // pointer comparison operators
    if (node->op.length() == 3) {
        Type* int64Ty = IntegerType::get();

        if (!isa<PointerType>(fst) && !isa<PointerType>(snd) && !isa<ObjectType>(fst) && !isa<ObjectType>(snd)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Expected at least one operand of " + node->op + " to be a pointer",
                node);
        }

        if (!fst->explicitlyCastableTo(int64Ty) || !fst->explicitlyCastableTo(int64Ty)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Binary operator " + node->op + " is not applicable to types " +
                fst->toString() + " and " + snd->toString(), node);
        }

        if (*fst != int64Ty) {
            wrapImplicitCast(node->lhs, fst, int64Ty);
        }

        if (*snd != int64Ty) {
            wrapImplicitCast(node->rhs, snd, int64Ty);
        }

        node->operandType = int64Ty;

        return IntegerType::get(1);
    }

    if (!snd->implicitlyCastableTo(fst)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot compare values of type " + fst
            ->toString() + " and " + snd->toString() + " for equality", node);
    }

    if (*snd != fst) {
        wrapImplicitCast(node->rhs, snd, fst);
    }

    auto boolTy = IntegerType::get(1);
    boolTy->isLvalue(false);
    node->operandType = boolTy;

    return boolTy;
}

Type* TypeCheckVisitor::HandleComparisonOp(Type *fst, Type *snd, BinaryOperator *node) {
    if (!isa<PrimitiveType>(fst) || !isa<PrimitiveType>(snd)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + node->op + "' values of type " + fst
            ->toString() + " and " + snd->toString(), node);
    }

    if (*snd != fst) {
        wrapImplicitCast(node->rhs, snd, fst);
    }

    auto boolTy = IntegerType::get(1);
    boolTy->isLvalue(false);
    node->operandType = boolTy;

    return boolTy;
}

Type* TypeCheckVisitor::HandleOtherOp(Type *fst, Type *snd, BinaryOperator *node) {
    auto& op = node->op;

    if (op == "..") {
        Type *intTy = IntegerType::get(64);
        if (!fst->implicitlyCastableTo(intTy)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
                ->toString() + " and " + snd->toString(), node);
        }
        if (!snd->implicitlyCastableTo(intTy)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
                ->toString() + " and " + snd->toString(), node);
        }

        if (*fst != intTy) {
            wrapImplicitCast(node->lhs, fst, intTy);
        }
        if (*snd != intTy) {
            wrapImplicitCast(node->rhs, snd, intTy);
        }

        auto collTy = new CollectionType(intTy);
        collTy->isLvalue(false);
        node->operandType = intTy;

        return collTy;
    }

    RuntimeError::raise(ERR_TYPE_ERROR, "Binary operator " + node->op + " is not defined for arguments of type " +
        fst->toString() + " and " + snd->toString(), node);

    return nullptr;
}

Type* TypeCheckVisitor::HandleBinaryOperator(Type *lhs, Type *rhs, BinaryOperatorType opTy, BinaryOperator *node) {
    switch (opTy) {
        case BinaryOperatorType::ASSIGNMENT:
            return HandleAssignmentOp(lhs, rhs, node);
        case BinaryOperatorType::ARITHMETIC:
            return HandleArithmeticOp(lhs, rhs, node);
        case BinaryOperatorType::BITWISE:
            return HandleBitwiseOp(lhs, rhs, node);
        case BinaryOperatorType::LOGICAL:
            return HandleLogicalOp(lhs, rhs, node);
        case BinaryOperatorType::EQUALITY:
            return HandleEqualityOp(lhs, rhs, node);
        case BinaryOperatorType::COMPARISON:
            return HandleComparisonOp(lhs, rhs, node);
        case BinaryOperatorType::OTHER:
        default:
            return HandleOtherOp(lhs, rhs, node);
    }
}

/**
 * Returns the type of a binary expression based on the input types
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(BinaryOperator *node) {
    //MAINBINARY
    auto opType = cdot::getBinaryOpType(node->op);
    node->opType = opType;

    auto isAssignment = opType == BinaryOperatorType::ASSIGNMENT;
    if (isAssignment) {
        node->lhs->returnLvalue(true);
        node->lhs->isLhsOfAssigment();
    }

    Type* fst = node->lhs->accept(*this);

    if (node->lhs->isSetterCall) {
        auto call = std::make_shared<CallExpr>(
            CallType::METHOD_CALL,
            std::vector<Expression::SharedPtr>{ node->rhs },
            node->lhs->setterName
        );

        node->overridenCall = call;

        CopyNodeProperties(node, call.get());
        call->setMemberExpr(node->memberExpr);
        call->setParent(node->parent);

        pushTy(fst);
        return call->accept(*this);
    }

    if (opType == BinaryOperatorType::CAST) {
        return HandleCastOp(fst, node);
    }

    bool is_overload = false;
    string className;
    std::vector<Type*> overloadArgs;

    if (isAssignment) {
        if (fst->isConst()) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Trying to reassign constant", node->lhs.get());
        }
        else if (!fst->isLvalue() && !fst->isStruct()) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign to rvalue of type " + fst->toString(),
                node->lhs.get());
        }

        bool wasLvalue = fst->isLvalue();

        // now that we know it's an lvalue, use the pointee type for compatibilty checks
        fst = fst->toRvalue();
        node->isStructAssignment = fst->isStruct();

        if (node->isStructAssignment) {
            node->lhs->needsLvalueToRvalueConversion = false;
            node->lhs->returnLvalue(true);
        }
    }

    node->rhs->setInferredType(fst);
    node->rhs->returnLvalue(true);
    Type* snd = node->rhs->accept(*this);

    if (isa<ObjectType>(fst) || isa<GenericType>(fst)) {
        className = fst->getClassName();
        overloadArgs.push_back(snd);
        is_overload = true;
    }
    else if (util::is_reversible(node->op) && snd->isObject()) {
        className = snd->getClassName();
        overloadArgs.push_back(fst);
        is_overload = true;
    }

    if (is_overload) {
        std::vector<Type*> argTypes{ snd };
        cdot::cl::Class* cl = SymbolTable::getClass(className, importedNamespaces);
        auto binOpResult = cl->hasMethod("infix " + node->op, argTypes);

        if (binOpResult.compatibility == CompatibilityType::COMPATIBLE) {
            // custom operators need rvalue argument
            if (isAssignment) {
                node->lhs->needsLvalueToRvalueConversion = true;
            }

            pushTy(fst);
            auto call = std::make_shared<CallExpr>(CallType::METHOD_CALL, std::vector<Expression::SharedPtr>{
                    node->rhs }, "infix " + node->op);

            ApplyCasts(call->args, argTypes, binOpResult.neededCasts);

            // we already resolved the argument, don't want to visit it again
            call->argTypes.push_back(snd);

            call->parentExpr = node;
            CopyNodeProperties(node, call.get());

            node->overridenCall = call;

            auto res = call->accept(*this);

            return res;
        }
    }

    if (snd->isLvalue()) {
        snd = snd->toRvalue();

        if (!node->isStructAssignment) {
            node->rhs->needsLvalueToRvalueConversion = true;
        }
    }

    auto res = HandleBinaryOperator(fst, snd, opType, node);
    return ReturnMemberExpr(node, res);
}

/**
 * Checks if the condition is boolean and the values are of the same type
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(TertiaryOperator *node) {
    Type* cond = node->condition->accept(*this);

    if (!cond->implicitlyCastableTo(IntegerType::get(1))) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Condition of tertiary operator '?:' must be boolean or implement "
            "instance method 'toBool() -> bool'", node);
    }

    Type* fst = node->lhs->accept(*this);
    Type* snd = node->rhs->accept(*this);

    if (!fst->implicitlyCastableTo(snd)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply tertiary operator '?:' to values of type " + fst->toString() +
            " and " + snd->toString(), node);
    }
    else if (*fst != snd) {
        Warning::issue("Implicit cast from " + snd->toString() + " to " + fst->toString(), node->rhs.get());
        wrapImplicitCast(node->rhs, snd, fst);
    }

    node->resultType = fst;

    return fst;
}

/**
 * Returns the result type of a unary expression depending on the input type
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(UnaryOperator *node) {

    string op = node->op;

    node->target->returnLvalue(true);
    Type* target = node->target->accept(*this);

    if (isa<ObjectType>(target->toRvalue()) || isa<GenericType>(target->toRvalue())) {
        auto& className = target->toRvalue()->getClassName();

        std::vector<Type*> argTypes;
        auto class_decl = SymbolTable::getClass(className, importedNamespaces);
        auto unOpResult = class_decl->hasMethod((node->prefix ? "prefix " : "postfix ") + op, argTypes);

        if (unOpResult.compatibility == CompatibilityType::COMPATIBLE) {
            auto call = std::make_shared<CallExpr>(CallType::METHOD_CALL, std::vector<Expression::SharedPtr>{},
                (node->prefix ? "prefix " : "postfix ") + op);

            node->overridenCall = call;
            node->operandType = target;
            call->memberExpr = node->memberExpr;

            pushTy(target->toRvalue());
            if (target->isLvalue()) {
                node->target->needsLvalueToRvalueConversion = true;
            }

            auto res = call->accept(*this);
            return res;
        }
    }

    if (op == "++" || op == "--") {
        if (!isa<PointerType>(target) || !target->isLvalue()) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator " + op + " can not be applied to temporary rvalue of "
                "type " + target->toString(), node->target.get());
        }

        auto pointee = cast<PointerType>(target)->getPointeeType();

        if (!isa<PrimitiveType>(pointee)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator " + op + " is not applicable to type " + target->toString(),
                node->target.get());
        }

        node->operandType = pointee;
        return pointee;
    }

    if (op == "*") {
        if (!currentBlockUnsafe) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Pointer operators are only allowed inside unsafe blocks", node);
        }
        if (!target->isLvalue()) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Trying to dereference non-pointer type", node->target.get());
        }


        node->operandType = target;
        return target->toRvalue();
    }

    if (op == "&") {
        if (!currentBlockUnsafe) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Pointer operators are only allowed inside unsafe blocks", node);
        }

        if (target->isLvalue()) {
            node->operandType = target;
            return target;
        }
        else {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply unary operator '&' to non-reference value", node->target.get());
        }
    }

    target = target->toRvalue();
    node->target->needsLvalueToRvalueConversion = true;
    node->operandType = target;

    if (op == "+" || op == "-") {
        if (!isa<PrimitiveType>(target)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator " + op + " is not applicable to type " + target->toString(),
                node->target.get());
        }
        if (op == "-" && isa<IntegerType>(target) && cast<IntegerType>(target)->isUnsigned()) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator '-' cannot be applied to unsigned integer",
                node->target.get());
        }

        return target;
    }

    if (op == "~") {
        if (!isa<IntegerType>(target)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator '~' is only applicable to type Int", node->target.get());
        }

        return target;
    }

    if (op == "!") {
        auto boolTy = IntegerType::get(1);
        if (!target->implicitlyCastableTo(boolTy)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator '!' is not applicable to type " + target
                ->toString(), node->target.get());
        }

        if (target != boolTy) {
            Warning::issue("Implicit cast to boolean", node->target.get());
        }

        return boolTy;
    }

    RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator " + node->op + " is not defined on class " +
        target->toString(), node);

    llvm_unreachable("You see that error up there?");
}

/**
 * Checks if a break statement is valid
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(BreakStmt *node) {
    if (!breakable) {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'break' is only allowed in switch, for and while statements",
            node);
    }

    return nullptr;
}

/**
 * Checks if a continue statement is valid
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(ContinueStmt *node) {
    if (!continuable) {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'continue' is only allowed in for and while statements",
            node);
    }

    return nullptr;
}

/**
 * Checks if the condition is boolean (or convertible), and if all branches return
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(IfStmt *node) {
    Type* cond = node->condition->accept(*this);

    if (!cond->implicitlyCastableTo(IntegerType::get(1))) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Condition must be boolean", node->condition.get());
    }

    // if there's no else, the remaining code path needs to return either way
    if (node->elseBranch) {

        TypeCheckVisitor if_visitor(this);
        if_visitor.returnable = true;

        node->ifBranch->accept(if_visitor);

        TypeCheckVisitor else_visitor(this);
        else_visitor.returnable = true;

        node->elseBranch->accept(else_visitor);

        // all branches return
        if (if_visitor.branches - if_visitor.returned <= 0 && !isa<VoidType>(declaredReturnType)
                && else_visitor.branches - else_visitor.returned <= 0) {
            returned++;
        }
    }
    else {
        TypeCheckVisitor if_visitor(this);
        if_visitor.returnable = returnable;

        node->ifBranch->accept(if_visitor);
    }

    return nullptr;
}

/**
 * Checks if the switch and case types are compatible, and if all branches return
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(SwitchStmt *node) {
    Type* switch_type = node->switchValue->accept(*this);

    return nullptr;
}

/**
 * Iterates over all children
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(CaseStmt *node) {
    node->fallthrough = true;
    for (const auto& child : node->get_children()) {
        if (child->get_type() == NodeType::BREAK_STMT) {
            node->fallthrough = false;
        }
        child->accept(*this);
    }

    return nullptr;
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(LabelStmt *node) {
    if (std::find(labels.begin(), labels.end(), node->labelName) != labels.end()) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Label '" + node->labelName + "' already exists in the same scope", node);
    }

    labels.push_back(node->labelName);

    return nullptr;
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(GotoStmt *node) {
    if (!has_label(node->labelName)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "No label '" + node->labelName + "' to go to", node);
    }

    return nullptr;
}

/**
 * Checks a function argument declaration for type consistency of default value and existence of declared type
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(FuncArgDecl *node) {

    node->argType->accept(*this);
    auto& ts = node->argType->getType();

    node->isStruct = ts->isStruct();

    if (node->hasAttribute(Attr::NoCopy) || (node->isStruct && node->mut)) {
        if (!isa<ObjectType>(ts)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "@nocopy cannot be used on non-object types", node);
        }

        node->mut = false;
        node->isStruct = false;
        if (isa<ObjectType>(ts)) {
            cast<ObjectType>(ts)->isStruct(false);
        }
    }
    else if (node->mut && (!isa<PointerType>(ts) || ts->isCStyleArray())) {
        ts = ts->getPointerTo();

        ts->isLvalue(true);
        ts->isReference(true);

        node->isStruct = false; // we don't want to memcpy a ref parameter
        node->argType->type = ts;
    }

    if (node->defaultVal) {
        Type* def_type = node->defaultVal->accept(*this);

        if (!ts->implicitlyCastableTo(def_type)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Default value for parameter " + node->argName + " must be of type "
                "" + node->argType->toString(), node->defaultVal.get());
        }
    }

    return ts;
}

/**
 * Returns the current context
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(ReturnStmt *node) {

    if (node->returnValue) {
        node->returnValue->inferredType = declaredReturnType;

        if (declaredReturnType != nullptr && declaredReturnType->isReference()) {
            node->returnValue->returnLvalue(true);
        }

        if (declaredReturnType->isStruct()) {
            node->returnValue->isHiddenReturnValue();
            node->hiddenParamReturn = true;
        }
        else {
            node->returnValue->isReturnValue();
        }

        node->returnValue->setInferredType(declaredReturnType);

        Type* ret_type = node->returnValue->accept(*this);

        node->returnType = declaredReturnType;
        return_(ret_type, node->returnValue.get());

        // in case the declaration is only known now
        if (declaredReturnType->isStruct()) {
            node->returnValue->isHiddenReturnValue();
        }

        if (*ret_type != declaredReturnType) {
            wrapImplicitCast(node->returnValue, ret_type, declaredReturnType);
        }
    }
    else {
        node->returnType = new VoidType();
        return_(node->returnType, node);
    }

    return nullptr;
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(Expression *node) {
    return nullptr;
}

cdot::cl::Class* TypeCheckVisitor::DeclareClass(ClassDecl *node) {

    node->qualifiedName = ns_prefix() + node->className;

    if (node->isExtension()) {
        auto cl = SymbolTable::getClass(node->qualifiedName);
        node->declaredClass = cl;

        for (const auto& proto : node->conformsTo) {
            cl->addConformity(proto);
        }

        return cl;
    }

    if (isReservedIdentifier(node->className)) {
        RuntimeError::raise(ERR_TYPE_ERROR, node->className + " is a reserved identifier", node);
    }

    currentClass = node->qualifiedName;

    if (node->parentClass == nullptr && node->className != "Any") {
        node->parentClass = ObjectType::get("Any");
    }

    cdot::cl::Class* cl;
    if (node->is_protocol || node->is_struct) {
        cl = SymbolTable::declareClass(node->qualifiedName, node->conformsTo, node->generics, node->is_protocol, node);
    }
    else {
        cl = SymbolTable::declareClass(node->qualifiedName, node->parentClass, node->conformsTo, node->generics, node,
            node->is_abstract);
    }

    node->declaredClass = cl;
    currentClass = "";

    return cl;
}

cdot::cl::Class* TypeCheckVisitor::DeclareClassMethods(ClassDecl *node) {

    auto cl = node->declaredClass;

    if (!node->is_extension) {
        cl->defineParentClass();
        currentClass = node->qualifiedName;
    }

    inProtocolDefinition = cl->isProtocol();

    if (!node->is_extension && node->parentClass != nullptr) {
        if (!SymbolTable::hasClass(node->parentClass->getClassName())) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Class " + node->parentClass->getClassName() + " does not exist", node);
        }

        auto parentClass = SymbolTable::getClass(node->parentClass->getClassName());
        Type::resolveUnqualified(node->parentClass);

        auto &concreteGenerics = node->parentClass->getConcreteGenericTypes();
        auto givenCount = concreteGenerics.size();
        auto neededCount = parentClass->getGenerics().size();

        if (givenCount != neededCount) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Class " + node->parentClass->getClassName() + " expects " + std::to_string
                (neededCount) + " generic type parameter(s), " + std::to_string(givenCount) + " were given", node
            );
        }

        size_t i = 0;
        for (const auto &needed : parentClass->getGenerics()) {
            auto &given = concreteGenerics[needed->getGenericClassName()];
            resolve(&given);

            if (isa<ObjectType>(given)) {
                checkExistance(cast<ObjectType>(given), node);
            }

            if (!GenericType::GenericTypesCompatible(given, needed)) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Given type parameter " + given->toString() + " is not compatible"
                    " with needed parameter " + needed->getGenericClassName() + " of class " +
                    node->parentClass->getClassName(), node
                );
            }

            cl->defineConcreteGeneric(needed->getGenericClassName(), given);
        }
    }

    for (const auto& prot : node->conformsTo) {
        auto& protocolName = prot->getClassName();

        if (!SymbolTable::hasClass(protocolName)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Protocol " + protocolName + " does not exist", node);
        }

        Type::resolveUnqualified(prot);

        auto protocol = SymbolTable::getClass(protocolName);
        if (!protocol->isProtocol()) {
            RuntimeError::raise(ERR_TYPE_ERROR, protocolName + " is not a protocol", node);
        }

        auto& concreteGenerics = prot->getConcreteGenericTypes();
        auto givenCount = concreteGenerics.size();
        auto neededCount = protocol->getGenerics().size();

        if (givenCount != neededCount) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Protocol " + protocolName + " expects " + std::to_string
                (neededCount) + " generic type parameter(s), " + std::to_string(givenCount) + " were given", node);
        }

        size_t i = 0;
        for (const auto& needed : protocol->getGenerics()) {
            auto& given = concreteGenerics[needed->getGenericClassName()];
            resolve(&given);

            if (!GenericType::GenericTypesCompatible(given, needed)) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Given type parameter " + given->toString() + " is not compatible"
                    " with needed parameter " + needed->getGenericClassName() + " of protocol " + protocolName, node);
            }
        }
    }

    pushNamespace(node->className);
    node->declaredClass = cl;

    for (const auto& td : node->typedefs) {
        td->accept(*this);
    }
    for (const auto& field : node->fields) {
        DeclareField(field.get(), cl);
    }
    for (const auto& method : node->methods) {
        DeclareMethod(method.get(), cl);
    }

    if (node->isStruct() || (node->is_extension && cl->isStruct())) {
        cl->declareMemberwiseInitializer();
    }

    if (!node->constructors.empty()) {
        for (const auto& constr : node->constructors) {
            DeclareConstr(constr.get(), cl);
        }
    }

    if (!node->is_extension && !node->is_abstract) {
        node->defaultConstr = cl->declareMethod("init.def", cl->getType()->toRvalue(),
            AccessModifier::PUBLIC, {}, {}, {}, {}, false
        );

        node->selfBinding = SymbolTable::mangleVariable(self_str, TypeCheckVisitor(this).scope);
    }

    popNamespace();
    currentClass = "";
    inProtocolDefinition = false;

    if (!node->is_extension && !ObjectType::hasStructureType(node->qualifiedName)) {
        auto prefix = node->is_struct ? "struct." : (node->is_protocol ? "proto." : "class.");
        auto class_type = llvm::StructType::create(CodeGenVisitor::Context, prefix + node->qualifiedName);
        ObjectType::declareStructureType(node->qualifiedName, class_type);
    }

    return cl;
}

void TypeCheckVisitor::DeclareField(FieldDecl *node, cdot::cl::Class *cl) {

    node->type->accept(*this);
    auto field_type = node->type->getType()->deepCopy();

    if (!cl->isProtocol() && cl->getDeclaration()->constructors.empty() && !field_type->hasDefaultValue() &&
        node->defaultVal == nullptr
    ) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Member " + node->fieldName + " does not have a default constructor and "
            "has to be explicitly initialized", node);
    }

    auto& qualified_name = cl->getName();
    node->className = qualified_name;

    if (node->isStatic) {
        return;
    }

    auto field = cl->declareField(node->fieldName, field_type, node->am, node->defaultVal);
    node->declaredType = &field->fieldType;

    if (cl->isProtocol()) {
        node->isProtocolField = true;
    }

    if (node->hasGetter) {
        std::vector<Type*> argTypes;
        string getterName = util::generate_getter_name(node->fieldName) + ".";
        node->getterBinding = SymbolTable::mangleMethod(node->className, getterName, argTypes);

        auto getterRetType = field_type->deepCopy();
        getterRetType->isLvalue(false);
        node->getterMethod = cl->declareMethod(getterName, getterRetType, AccessModifier::PUBLIC, {}, {}, {}, {},
            false);

        field->hasGetter = true;
        field->getterName = getterName;
    }

    if (node->hasSetter) {
        std::vector<Type*> argTypes{ field_type };
        string setterName = util::generate_setter_name(node->fieldName) + ".";
        auto setterRetType = new VoidType;
        node->setterBinding = SymbolTable::mangleMethod(node->className, setterName, argTypes);

        node->setterMethod = cl->declareMethod(setterName, setterRetType, AccessModifier::PUBLIC, {node->fieldName},
            argTypes, {}, {}, false);

        field->hasSetter = true;
        field->setterName = setterName;
    }
}

void TypeCheckVisitor::DefineField(FieldDecl *node, cdot::cl::Class *cl) {
    auto& field_type = node->type->getType();

    if (node->defaultVal != nullptr) {
        Type* def_type = node->defaultVal->accept(*this);

        if (field_type->isInferred()) {
            field_type = def_type;

            if (!node->isStatic) {
                *node->declaredType = field_type;
            }
        }
        else if (!field_type->implicitlyCastableTo(def_type)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Default value for field " + node->fieldName + " must be of type " +
                node->type->toString(), node->defaultVal.get());
        }
        else if (*def_type != field_type) {
            Warning::issue("Implicit cast from " + def_type->toString() + " to " + node->type->toString(),
                node->defaultVal.get());
        }
    }

    if (node->isStatic) {
        node->binding = declare_var(node->fieldName, field_type, true);
        return;
    }

    if (node->hasGetter && node->getterBody != nullptr) {
        auto visitor = makeMethodVisitor(node->getterMethod->returnType, node->className);
        visitor.currentSelf = SymbolTable::mangleVariable(self_str, visitor.scope);

        node->getterBody->accept(visitor);
        node->getterSelfBinding = visitor.currentSelf;

        if (visitor.returned == 0) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Returning Void from a method with declared return type " +
                node->getterMethod->returnType->toString(), node);
        }

        if (visitor.branches - visitor.returned > 0) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Not all code paths return a value", node->getterBody.get());
        }
    }

    if (node->hasSetter && node->setterBody != nullptr) {
        auto visitor = makeMethodVisitor(node->setterMethod->returnType, node->className);
        string newValStr = "newVal";

        auto typeref = std::make_shared<TypeRef>(field_type);
        typeref->resolved = true;

        node->newVal = std::make_shared<FuncArgDecl>(newValStr, typeref);

        visitor.currentSelf = SymbolTable::mangleVariable(self_str, visitor.scope);
        node->newVal->binding = visitor.declare_var(newValStr, field_type);

        node->setterSelfBinding = visitor.currentSelf;
        node->setterBody->accept(visitor);
    }
}

void TypeCheckVisitor::DeclareMethod(MethodDecl *node, cdot::cl::Class *cl) {
    std::vector<Type*> argTypes;
    std::vector<string> argNames;
    std::vector<Expression::SharedPtr> argDefaults;

    for (const auto &arg : node->args) {
        arg->accept(*this);
        auto& resolvedArg = arg->argType->getType();
        resolvedArg->isLvalue(arg->mut);
        argTypes.push_back(resolvedArg);

        argNames.push_back(arg->argName);
        argDefaults.push_back(arg->defaultVal);
    }

    node->class_name = cl->getName();
    node->binding = SymbolTable::mangleMethod(node->class_name, node->methodName, argTypes);

    auto result = cl->hasMethod(node->methodName, argTypes, false, false);
    if (result.compatibility == CompatibilityType::COMPATIBLE) {
        if (node->isAlias) {
            cl->declareMethodAlias(node->alias, result.method->mangledName);
            return;
        }

        RuntimeError::raise(ERR_TYPE_ERROR, "Method " + node->methodName + " cannot be redeclared with a "
            "similar signature to a previous declaration", node);
    }
    else if (node->isAlias) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Aliased method " + node->methodName + " does not exist", node);
    }

    node->returnType->accept(*this);
    auto return_type = node->returnType->getType()->deepCopy();
    return_type->isLvalue(return_type->isReference());

    node->method = cl->declareMethod(node->methodName, return_type, node->am, argNames,
        argTypes, argDefaults, node->generics, node->isStatic);

    if (return_type->isStruct()) {
        node->method->hasHiddenParam = true;
    }
}

void TypeCheckVisitor::DeclareConstr(ConstrDecl *node, cdot::cl::Class *cl) {

    if (node->memberwise) {
        cl->declareMemberwiseInitializer();
        return;
    }

    std::vector<Type*> argTypes;
    std::vector<string> argNames;
    std::vector<Expression::SharedPtr> argDefaults;

    for (auto arg : node->args) {
        arg->accept(*this);
        auto& resolvedArg = arg->argType->getType();
        resolvedArg->isLvalue(arg->mut);
        argTypes.push_back(resolvedArg);

        argNames.push_back(arg->argName);
        argDefaults.push_back(arg->defaultVal);
    }

    string method_name = "init";

    node->className = cl->getName();

    node->binding = SymbolTable::mangleMethod(node->className, method_name, argTypes);
    node->method = cl->declareMethod(method_name, cl->getType()->toRvalue(), node->am, argNames,
        argTypes, argDefaults, {}, false);
}

void TypeCheckVisitor::DefineClass(ClassDecl *node, cdot::cl::Class *cl) {

    pushNamespace(node->className);
    currentClass = node->qualifiedName;
    inProtocolDefinition = cl->isProtocol();

    for (const auto& field : node->fields) {
        DefineField(field.get(), cl);
    }

    for (const auto& method : node->methods) {
        DefineMethod(method.get(), cl);
    }

    for (const auto& constr : node->constructors) {
        DefineConstr(constr.get(), cl);
    }

    popNamespace();
    currentClass = "";
    inProtocolDefinition = false;

    try {
        cl->finalize();
    } catch (string err) {
        RuntimeError::raise(ERR_TYPE_ERROR, err, node);
    }
}

void TypeCheckVisitor::DefineMethod(MethodDecl *node, cdot::cl::Class *cl) {

    if (node->isAlias) {
        return;
    }

    auto& return_type = node->returnType->getType();
    auto method_visitor = makeMethodVisitor(return_type, cl->getName());

    if (!node->isStatic) {
        method_visitor.currentSelf = SymbolTable::mangleVariable(self_str, method_visitor.scope);
        node->selfBinding = method_visitor.currentSelf;
    }

    attributes = node->attributes;

    if (node->body) {
        for (const auto &arg : node->args) {
            arg->binding = method_visitor.declare_var(arg->argName, arg->argType->getType());
        }

        node->body->accept(method_visitor);

        if (method_visitor.returned == 0) {
            if (!isa<VoidType>(node->returnType->getType())) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Returning Void from a method with declared return type " +
                    node->returnType->getType()->toString(), node->returnType.get());
            }
        }
        else {
            return_type = method_visitor.declaredReturnType;
        }

        node->method->returnType = return_type;

        if (method_visitor.branches - method_visitor.returned > 0 && !isa<VoidType>(return_type) &&
            !method_visitor.declaredReturnType->isNullable())
        {
            RuntimeError::raise(ERR_TYPE_ERROR, "Not all code paths return a value", node->body.get());
        }
    }
    else if (return_type->isInferred()) {
        return_type = new VoidType();
    }

    attributes.clear();
}

void TypeCheckVisitor::DefineConstr(ConstrDecl *node, cdot::cl::Class *cl) {

    if (node->memberwise) {
        return;
    }

    auto visitor = makeMethodVisitor(new VoidType, cl->getName());
    visitor.currentSelf = SymbolTable::mangleVariable(self_str, visitor.scope);
    node->selfBinding = visitor.currentSelf;

    std::vector<string> uninitialized;
    for (const auto& field : cl->getFields()) {
        if (!field.second->fieldType->hasDefaultValue() && field.second->defaultVal == nullptr) {
            uninitialized.push_back(field.second->fieldName);
        }
    }

    visitor.uninitializedFields = &uninitialized;

    for (auto& arg : node->args) {
        arg->binding = visitor.declare_var(arg->argName, arg->argType->getType());
    }

    node->body->accept(visitor);

    if (!uninitialized.empty()) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Non-nullable member " + uninitialized.front() + " of "
            "class " + cl->getName() + " does not define a default constructor and has to be explicitly initialized "
            "in every constructor", node->body.get());
    }
}

/**
 * Declares a new class
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(ClassDecl *node) {
    currentClassGenerics = &node->generics;
    DefineClass(node, node->declaredClass);

    currentClassGenerics = nullptr;
    return nullptr;
}

/**
 * Declares a class constructor
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(ConstrDecl *node) {
    return nullptr;
}

/**
 * Declares a class field and checks for correct type
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(FieldDecl *node) {
    return nullptr;
}

/**
 * Declares a method and checks if all code paths return
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(MethodDecl *node) {
    return nullptr;
}

/**
 * Checks if all code paths return the correct type
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(LambdaExpr *node) {

    node->_return_type->accept(*this);

    auto func_visitor = makeFunctionVisitor(node->_return_type->getType());
    func_visitor.currentSelf = SymbolTable::mangleVariable(self_str, func_visitor.scope);
    func_visitor.isNestedFunction = true;

    std::vector<Type*> arg_types;
    for (auto arg : node->_args) {
        Type* ts = arg->accept(*this);
        arg_types.push_back(ts);
        arg->binding = func_visitor.declare_var(arg->argName, arg->argType->getType());
    }

    auto ts = new FunctionType(node->_return_type->getType(), std::move(arg_types));

    if (node->self_ident != "") {
        func_visitor.declare_var(node->self_ident, ts);
    }

    if (node->is_single_expr) {
        node->_return_type->setType(node->_body->accept(func_visitor));
    }
    else {
        node->_body->accept(func_visitor);
        node->_return_type->setType(func_visitor.declaredReturnType);

        if (func_visitor.branches - func_visitor.returned > 0 && !isa<VoidType>(node->_return_type->getType()) &&
            !func_visitor.declaredReturnType->isNullable())
        {
            RuntimeError::raise(ERR_TYPE_ERROR, "Not all code paths return a value", node->_body.get());
        }
    }

    node->captures = func_visitor.captures;
    node->capture_types = func_visitor.captureTypes;
    node->env_binding = func_visitor.currentSelf;

    // type might be used before LambdaExpr Codegen, so declare it here
    std::vector<llvm::Type*> capture_types;
    for (auto& capture : node->capture_types) {
        auto type = capture->getLlvmType();
        if (!type->isPointerTy()) {
            type = type->getPointerTo();
        }

        capture_types.push_back(type);
    }

    auto env_type = llvm::StructType::create(CodeGenVisitor::Context, capture_types, ".env");

    std::vector<llvm::Type*> larg_types{ env_type->getPointerTo() };
    for (const auto& arg : node->_args) {
        larg_types.push_back(arg->argType->getType()->getLlvmType());
    }

    auto func_type = llvm::FunctionType::get(node->_return_type->getType()->getLlvmType(), larg_types, false);

    node->lambda_type = llvm::StructType::create(CodeGenVisitor::Context, { env_type->getPointerTo(),
        func_type->getPointerTo() }, ".lambda");

    if (node->memberExpr == nullptr) {
        return ts;
    }
    if (node->memberExpr->get_type() != NodeType::CALL_EXPR) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Only call expressions can immediately follow a lambda expression", node);
        return nullptr;
    }
    else {
        pushTy(ts);
        auto res = node->memberExpr->accept(*this);

        return res;
    }
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(UsingStmt *node) {
    if (isBuilitinNamespace(node->nsName)) {
        Builtin::ImportBuiltin(node->nsName);
    }

    auto nsName = node->nsName;
    if (!SymbolTable::isNamespace(nsName)) {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Imported namespace " + node->nsName + " does not exist", node);
    }

    importedNamespaces.push_back(nsName + ".");

    return nullptr;
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(EndOfFileStmt *node) {
    importedNamespaces.clear();
    importedNamespaces.push_back("");

    return nullptr;
}

Type* TypeCheckVisitor::visit(ImplicitCastExpr *node) {
    resolve(&node->to);

    node->target->returnLvalue(false);
    node->target->accept(*this);

    return node->to;
}

Type* TypeCheckVisitor::visit(ExtendStmt *node) {

    if (!SymbolTable::hasClass(node->extended_class)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Class " + node->extended_class + " does not exist", node);
    }

    auto cl = SymbolTable::getClass(node->extended_class, importedNamespaces);
    currentClass = cl->getName();

    for (const auto& field : node->fields) {
        field->accept(*this);
    }

    for (const auto& method : node->methods) {
        method->accept(*this);
    }

    for (const auto& method : node->methods) {
        method->accept(*this);
    }

    return nullptr;
}

Type* TypeCheckVisitor::visit(TypedefDecl *node) {

    node->origin->accept(*this);
    SymbolTable::declareTypedef(currentNamespace.back() + node->alias, node->origin->getType());

    return nullptr;
}

Type* TypeCheckVisitor::visit(TypeRef *node) {
    assert(!node->resolved && "Duplicate resolving");

    if (!node->resolved) {
        if (isa<ObjectType>(node->type) && node->type->getClassName() == "Self" && !inProtocolDefinition) {
            RuntimeError::raise(ERR_TYPE_ERROR, "'Self' is only valid in protocol definitions", node);
        }

        resolve(&node->type);
        node->resolved = true;

        if (isa<ObjectType>(node->type)) {
            if (!SymbolTable::hasClass(node->type->getClassName())) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Unknown typename " + node->type->toString(), node);
            }
        }
    }

    return node->type;
}

Type* TypeCheckVisitor::visit(DeclareStmt *node) {
    if (node->type != nullptr) {
        node->type->accept(*this);
    }

    auto qualified_name = ns_prefix() + node->declaredName;

    switch (node->declKind) {
        case DeclarationType::VAR_DECL: {
            auto type = node->type->getType();
            declare_var(node->declaredName, type, true, node);
            break;
        }
        case DeclarationType::FUNC_DECL: {
            auto type = node->type->getType();
            Function::UniquePtr fun = std::make_unique<Function>(qualified_name, type, node->generics);

            std::vector<Type*> arg_types;
            for (const auto &arg : node->args) {
                arg_types.push_back(arg->accept(*this));
                fun->addArgument(arg_types.back(), arg->defaultVal);
            }

            declare_fun(std::move(fun), node->generics, node);
            node->bind(SymbolTable::mangleFunction(qualified_name, arg_types));

            break;
        }
        case DeclarationType::CLASS_DECL: {
            if (node->extends == nullptr) {
                node->extends = ObjectType::get("Any");
            }

            auto cl = SymbolTable::declareClass(qualified_name, node->extends, node->conformsTo, node->generics, nullptr,
                node->is_abstract);

            auto class_type = llvm::StructType::create(CodeGenVisitor::Context, "class." + qualified_name);
            ObjectType::declareStructureType(qualified_name, class_type);

            break;
        }
    }

    return nullptr;
}

Type* TypeCheckVisitor::visit(LvalueToRvalue* node) {
    auto ty = node->target->accept(*this);
    return ReturnMemberExpr(node, ty);
}

Type* TypeCheckVisitor::visit(DebugStmt* node) {
    return nullptr;
}