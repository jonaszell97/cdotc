//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CONSTRDECL_H
#define CDOT_CONSTRDECL_H

#include "../CallableDecl.h"

namespace cdot {

namespace cl {

struct Method;

} // namespace cl

namespace ast {

class FuncArgDecl;
class CompoundStmt;

class ConstrDecl : public CallableDecl {
public:
   ConstrDecl();
   ConstrDecl(std::vector<std::shared_ptr<FuncArgDecl>> &&args,
              AccessModifier am,
              std::vector<TemplateParameter> &&TPs,
              std::shared_ptr<CompoundStmt> &&body,
              string &&name = "init");

   typedef std::shared_ptr<ConstrDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ConstrDeclID;
   }

protected:
   bool memberwise = false;
   cdot::cl::Record *record;

public:
   bool isMemberwise() const
   {
      return memberwise;
   }

   void setMemberwise(bool memberwise)
   {
      ConstrDecl::memberwise = memberwise;
   }

   cdot::cl::Method *getMethod() const;

   cl::Record *getRecord() const
   {
      return record;
   }

   void setRecord(cl::Record *record)
   {
      ConstrDecl::record = record;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_CONSTRDECL_H
