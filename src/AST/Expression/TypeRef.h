//
// Created by Jonas Zell on 06.08.17.
//

#ifndef CDOT_TYPEREF_H
#define CDOT_TYPEREF_H


#include "../Expression/Expression.h"

class TypeRef : public Expression {
public:
   enum TypeKind {
      Auto,
      Primitive,
      FunctionType,
      TupleType,
      ObjectType
   };

   typedef std::shared_ptr<TypeRef> SharedPtr;

   TypeRef();
   TypeRef(string& className, std::vector<pair<string, TypeRef::SharedPtr>> generics);
   TypeRef(TypeRef::SharedPtr returnType, std::vector<pair<string, TypeRef::SharedPtr>> &argTypes);
   explicit TypeRef(std::vector<pair<string, TypeRef::SharedPtr>> &tupleTypes);

   TypeRef(const Type &ty) : type(ty), resolved(true) {}

   string toString();

   inline Type getType(bool force = false)
   {
      assert((force || resolved) && "Resolve type before accessing!");
      return type;
   }

   void isVararg(bool va)
   {
      vararg = va;
   }

   bool isVararg()
   {
      return vararg;
   }

   void isCStyleVararg(bool va)
   {
      cstyleVararg = va;
      vararg = va;
   }

   bool isCStyleVararg()
   {
      return cstyleVararg;
   }

   bool isReference()
   {
      return is_reference;
   }

   void isReference(bool ref)
   {
      is_reference = ref;
   }

   bool isOption()
   {
      return is_option;
   }

   void isOption(bool opt)
   {
      is_option = opt;
   }

   size_t& getPointerDepth()
   {
      return pointerDepth;
   }

   inline void setType(Type& t)
   {
      type = t;
   }

   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override;

   NodeType get_type() override {
      return NodeType::TYPE_REF;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type accept(SemaPass& v) override {
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
   TypeKind kind;
   bool resolved = false;

   bool is_option = false;
   bool is_reference = false;
   size_t pointerDepth = 0;

   Type type;

   string className;
   std::vector<pair<string, TypeRef::SharedPtr>> containedTypes;
   TypeRef::SharedPtr returnType = nullptr;

   bool vararg = false;
   bool cstyleVararg = false;
};


#endif //CDOT_TYPEREF_H
