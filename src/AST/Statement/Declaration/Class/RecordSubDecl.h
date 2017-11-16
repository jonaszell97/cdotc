//
// Created by Jonas Zell on 14.11.17.
//

#ifndef CDOT_RECORDSUBDECL_H
#define CDOT_RECORDSUBDECL_H

#include "../../Statement.h"

class RecordSubDecl: public Statement {
protected:
   RecordSubDecl(
      std::string &&name,
      AccessModifier access,
      bool isStatic
   );

   std::string name;
   AccessModifier access;
   bool is_static;

   cl::Record *record = nullptr;

public:
   const string &getName() const;
   void setName(const string &name);

   AccessModifier getAccess() const;
   void setAccess(AccessModifier access);

   bool isStatic() const;
   void isStatic(bool is_static);

   cl::Record *getRecord() const;
   void setRecord(cl::Record *record);
};


#endif //CDOT_RECORDSUBDECL_H
