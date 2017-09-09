//
// Created by Jonas Zell on 28.06.17.
//

#ifndef CDOT_CASESTMT_H
#define CDOT_CASESTMT_H

#include "LabelStmt.h"
class Expression;

namespace cdot {
   namespace cl {
      struct EnumCase;
   }
}

class CaseStmt : public LabelStmt {
public:
   explicit CaseStmt(std::shared_ptr<Expression>);
   CaseStmt();
   ~CaseStmt() override;

   void setBody(Statement::SharedPtr stmt) {
      body = stmt;
   }

   typedef std::shared_ptr<CaseStmt> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::CASE_STMT;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   Variant accept(ConstExprPass& v) override {
      return v.visit(this);
   }

   friend class ConstExprPass;
   friend class CodeGen;
   friend class TypeCheckPass;
   friend class DeclPass;

protected:
   bool isDefault = false;
   std::shared_ptr<Expression> caseVal;
   std::shared_ptr<Statement> body;
   Type *caseType = nullptr;

   // codegen
   bool fallthrough = false;
   bool isEnumLetCase = false;
   bool isEnumVarCase = false;
   bool needsGenericCast = false;
   Type* genericOriginTy = nullptr;
   Type* genericDestTy = nullptr;

   cdot::cl::EnumCase* enumCaseVal;
   std::vector<pair<string, Type*>> letIdentifiers;
   std::vector<string> letBindings;
};


#endif //CDOT_CASESTMT_H
