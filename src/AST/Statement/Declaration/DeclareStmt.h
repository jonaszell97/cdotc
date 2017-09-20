//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_DECLARESTMT_H
#define CDOT_DECLARESTMT_H


#include "../Statement.h"

class FuncArgDecl;
class TypeRef;

enum class DeclarationType {
   CLASS_DECL,
   FUNC_DECL,
   VAR_DECL
};

class DeclareStmt : public Statement {
public:
   DeclareStmt(string, std::shared_ptr<TypeRef>, bool);
   DeclareStmt(string, std::shared_ptr<TypeRef>, std::vector<std::shared_ptr<FuncArgDecl>>, std::vector<ObjectType*>);
   DeclareStmt(AccessModifier, string, ObjectType*, std::vector<ObjectType*>, bool, std::vector<ObjectType*>);

   std::vector<AstNode::SharedPtr> get_children() override;

   typedef std::shared_ptr<DeclareStmt> SharedPtr;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::DECLARE_STMT;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   void accept(DeclPass &v) override {
      v.visit(this);
   }

   Variant accept(ConstExprPass &v) override {
      return v.visit(this);
   }

   friend class ConstExprPass;
   friend class CodeGen;
   friend class TypeCheckPass;
   friend class DeclPass;

protected:
   DeclarationType declKind;
   string declaredName;

   // class declaration
   std::vector<ObjectType*> generics;
   ObjectType* extends;
   std::vector<ObjectType*> conformsTo;
   AccessModifier am;
   bool is_abstract;

   // function declaration
   std::vector<std::shared_ptr<FuncArgDecl>> args;
   std::shared_ptr<TypeRef> type = nullptr;

   // variable
   bool isConst;
};


#endif //CDOT_DECLARESTMT_H
