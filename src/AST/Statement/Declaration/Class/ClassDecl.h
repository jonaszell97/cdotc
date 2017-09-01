//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CLASSDECLEXPR_H
#define CDOT_CLASSDECLEXPR_H

#include "../../../Statement/Statement.h"

class FieldDecl;
class MethodDecl;
class ConstrDecl;
class TypedefDecl;

namespace cdot {
    namespace cl {
        struct Method;
        class Class;
    }
}

class ClassDecl : public Statement {
public:
    ClassDecl(string, std::vector<std::shared_ptr<FieldDecl>>&&, std::vector<std::shared_ptr<MethodDecl>>&&,
        std::vector<std::shared_ptr<ConstrDecl>>&&, std::vector<std::shared_ptr<TypedefDecl>>&&,
        std::vector<ObjectType*>&&, AccessModifier, bool, ObjectType*, std::vector<ObjectType*>&&);

    ClassDecl(string, std::vector<std::shared_ptr<FieldDecl>>&&, std::vector<std::shared_ptr<MethodDecl>>&&,
        std::vector<std::shared_ptr<ConstrDecl>>&&, std::vector<std::shared_ptr<TypedefDecl>>&&,
        std::vector<ObjectType*>&&, AccessModifier, std::vector<ObjectType*>&&);

    virtual inline bool isStruct() {
        return is_struct;
    }

    virtual inline void isStruct(bool str) {
        is_struct = str;
    }

    virtual inline bool isExtension() {
        return is_extension;
    }

    virtual inline void isExtension(bool ext) {
        is_extension = ext;
    }

    virtual inline ObjectType* getParentClass() {
        return parentClass;
    }

    typedef std::shared_ptr<ClassDecl> SharedPtr;
    typedef std::unique_ptr<ClassDecl> UniquePtr;

    std::vector<std::shared_ptr<AstNode>> get_children() override;
    void __dump(int depth) override;

    NodeType get_type() override {
        return NodeType::CLASS_DECL;
    }

    llvm::Value* accept(CodeGenVisitor& v) override {
        return v.visit(this);
    }

    Type* accept(TypeCheckVisitor& v) override {
        return v.visit(this);
    }

    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;
    friend class cdot::cl::Class;

protected:
    ObjectType* parentClass = nullptr;
    std::vector<ObjectType*> conformsTo;

    bool is_abstract = false;
    bool is_protocol = false;
    bool is_struct = false;
    bool is_extension = false;

    AccessModifier am;
    string className;
    string qualifiedName;

    std::vector<std::shared_ptr<ConstrDecl>> constructors;
    std::vector<std::shared_ptr<FieldDecl>> fields;
    std::vector<std::shared_ptr<MethodDecl>> methods;
    std::vector<std::shared_ptr<TypedefDecl>> typedefs;

    std::vector<ObjectType*> generics;

    // codegen
    string selfBinding;
    cdot::cl::Class* declaredClass;
    cdot::cl::Method* defaultConstr = nullptr;
};


#endif //CDOT_CLASSDECLEXPR_H
