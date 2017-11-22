//
// Created by Jonas Zell on 14.11.17.
//

#ifndef CDOT_RECORDSUBDECL_H
#define CDOT_RECORDSUBDECL_H

#include "../../Statement.h"

namespace cdot {
namespace ast {

class RecordSubDecl: public Statement {
protected:
   RecordSubDecl(
      NodeType typeID,
      std::string &&name,
      AccessModifier access,
      bool isStatic
   );

   std::string name;
   AccessModifier access;
   bool is_static;

   cdot::cl::Record *record = nullptr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == FieldDeclID || T->getTypeID() == PropDeclID;
   }

public:
   const string &getName() const;
   void setName(const string &name);

   AccessModifier getAccess() const;
   void setAccess(AccessModifier access);

   bool isStatic() const;
   void isStatic(bool is_static);

   cdot::cl::Record *getRecord() const;
   void setRecord(cdot::cl::Record *record);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_RECORDSUBDECL_H
