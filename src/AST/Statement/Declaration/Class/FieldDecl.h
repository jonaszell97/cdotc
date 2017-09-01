//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CLASSMEMBERDECL_H
#define CDOT_CLASSMEMBERDECL_H


#include "../../Statement.h"

class TypeRef;
class Expression;

namespace cdot {
namespace cl {
   struct Method;
   struct Field;
}
}

class FieldDecl : public Statement {
public:
   FieldDecl(std::string, std::shared_ptr<TypeRef>, AccessModifier = AccessModifier::PUBLIC, bool = false, bool = false,
      std::shared_ptr<Expression> = {});

   inline void setDefault(std::shared_ptr<Expression> expr) {
      defaultVal = expr;
   }

   typedef std::shared_ptr<FieldDecl> SharedPtr;
   typedef std::unique_ptr<FieldDecl> UniquePtr;

   std::vector<std::shared_ptr<AstNode>> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::FIELD_DECL;
   }

   llvm::Value* accept(CodeGenVisitor& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckVisitor& v) override {
      return v.visit(this);
   }

   virtual inline void addGetter(std::shared_ptr<CompoundStmt> body = nullptr) {
      hasGetter = true;
      getterBody = body;
   }

   virtual inline void addSetter(std::shared_ptr<CompoundStmt> body = nullptr) {
      hasSetter = true;
      setterBody = body;
   }

   friend class ConstExprVisitor;
   friend class CodeGenVisitor;
   friend class TypeCheckVisitor;

protected:
   bool hasGetter = false;
   bool hasSetter = false;
   std::shared_ptr<CompoundStmt> getterBody = nullptr;
   std::shared_ptr<CompoundStmt> setterBody = nullptr;
   string getterSelfBinding;
   string setterSelfBinding;
   cdot::cl::Method *getterMethod;
   cdot::cl::Method *setterMethod;
   std::shared_ptr<FuncArgDecl> newVal = nullptr;

   bool isStatic;
   bool isConst;
   AccessModifier am;
   std::shared_ptr<TypeRef> type;
   string fieldName;
   std::shared_ptr<Expression> defaultVal;
   Type** declaredType;

   bool isProtocolField = false;

   // codegen
   string className;
   string getterBinding;
   string setterBinding;
};


#endif //CDOT_CLASSMEMBERDECL_H
