//
// Created by Jonas Zell on 06.08.17.
//

#ifndef CDOT_TYPEREF_H
#define CDOT_TYPEREF_H


#include "../Expression/Expression.h"

class TypeRef : public Expression {
public:
   explicit TypeRef();
   explicit TypeRef(Type*);
   
   ~TypeRef() override;

   string toString();

   void isGenericConstraint(bool constr) {
      isGenericConstraint_ = constr;
   }

   inline Type*& getType(bool force = false) {
      assert((force || resolved) && "Resolve type before accessing!");
      return type;
   }

   inline void setType(Type* t) {
      if (type == t) {
         return;
      }

      delete type;
      type = t;
   }

   typedef std::shared_ptr<TypeRef> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override;

   NodeType get_type() override {
      return NodeType::TYPE_REF;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   void accept(AbstractPass* v) override {
      v->visit(this);
   }

   Variant accept(ConstExprPass& v) override {
      return v.visit(this);
   }

   ADD_FRIEND_PASSES

protected:
   bool isGenericConstraint_ = false;
   bool resolved = false;
   Type* type = nullptr;
};


#endif //CDOT_TYPEREF_H
