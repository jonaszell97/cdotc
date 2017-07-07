//
// Created by Jonas Zell on 04.07.17.
//

#ifndef CDOT_STRUCTDECL_H
#define CDOT_STRUCTDECL_H


#include "ClassDecl.h"

class StructDecl : public ClassDecl {
public:
    StructDecl(std::string, std::vector<FieldDecl::SharedPtr>, std::vector<MethodDecl::SharedPtr>,
            ConstrDecl::SharedPtr = {}, std::vector<std::pair<std::string, OperatorDecl::SharedPtr>> = {},
            std::vector<std::pair<std::string, OperatorDecl::SharedPtr>> = {});

    typedef std::shared_ptr<StructDecl> SharedPtr;
    typedef std::unique_ptr<StructDecl> UniquePtr;

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class TypeCheckVisitor;
};


#endif //CDOT_STRUCTDECL_H
