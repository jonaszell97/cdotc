//
// Created by Jonas Zell on 28.06.17.
//

#ifndef CDOT_CASESTMT_H
#define CDOT_CASESTMT_H

#include "LabelStmt.h"

namespace cdot {

namespace cl {

struct EnumCase;

} // namespace cdot

namespace ast {

class Expression;

class CaseStmt : public LabelStmt {
public:
   explicit CaseStmt(std::shared_ptr<Expression> &&caseVal);
   CaseStmt();

   void setBody(Statement::SharedPtr &&stmt)
   {
      body = std::move(stmt);
   }

   typedef std::shared_ptr<CaseStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == CaseStmtID;
   }

protected:
   bool isDefault = false;
   std::shared_ptr<Expression> caseVal;
   std::shared_ptr<Statement> body;
   QualType caseType;

   // codegen
   bool isEnumLetCase = false;
   bool isEnumVarCase = false;

   cl::Method* operatorEquals = nullptr;

   cdot::cl::EnumCase* enumCaseVal;
   std::vector<pair<string, QualType>> letIdentifiers;
   std::vector<string> letBindings;

public:
   bool isIsDefault() const;

   std::shared_ptr<Expression> &getCaseVal();

   const std::shared_ptr<Statement> &getBody() const;

   const QualType &getCaseType() const;

   bool isIsEnumLetCase() const;

   bool isIsEnumVarCase() const;

   cl::Method *getOperatorEquals() const;

   cdot::cl::EnumCase *getEnumCaseVal() const;

   std::vector<pair<string, QualType>> &getLetIdentifiers();
   std::vector<string> &getLetBindings();

   void setIsDefault(bool isDefault);

   void setCaseVal(const std::shared_ptr<Expression> &caseVal);

   void setCaseType(const QualType &caseType);

   void setIsEnumLetCase(bool isEnumLetCase);

   void setIsEnumVarCase(bool isEnumVarCase);

   void setOperatorEquals(cdot::cl::Method *operatorEquals);

   void setEnumCaseVal(cdot::cl::EnumCase *enumCaseVal);

   void
   setLetIdentifiers(const std::vector<pair<string, QualType>> &letIdentifiers);

   void setLetBindings(const std::vector<string> &letBindings);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_CASESTMT_H
