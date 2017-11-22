//
// Created by Jonas Zell on 10.10.17.
//

#ifndef CDOT_UNION_H
#define CDOT_UNION_H

#include "Record.h"

#include <unordered_map>
#include <string>

using std::string;
using std::unordered_map;

namespace cdot {
class Type;

namespace ast {
class UnionDecl;
}

namespace cl {

class Union: public Record {
public:
   Union(
      const string& name,
      bool isConst,
      const SourceLocation &loc,
      UnionDecl *decl
   );

   bool isUnion() const override
   {
      return true;
   }

   const unordered_map<string, Type *> &getFields() const;
   void setFields(const unordered_map<string, Type *> &types);

   bool isConst() const;
   void isConst(bool is_const);

   bool hasField(string &name);
   Type *getFieldTy(string &name);

   void declareField(
      const string &fieldName,
      Type *fieldTy
   );

   Type* initializableWith(Type *ty);

protected:
   unordered_map<string, Type*> fields;
   bool is_const;
};

}
}


#endif //CDOT_UNION_H
