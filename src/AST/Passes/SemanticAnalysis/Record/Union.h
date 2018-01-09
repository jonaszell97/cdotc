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
      Namespace *NS,
      bool isConst,
      std::vector<TemplateParameter> &&templateParams,
      const SourceLocation &loc,
      UnionDecl *decl
   );

   const unordered_map<string, Type *> &getFields() const;
   void setFields(const unordered_map<string, Type *> &types);

   bool isConst() const;
   void isConst(bool is_const);

   bool hasField(llvm::StringRef name);
   Type *getFieldTy(llvm::StringRef name);

   void declareField(const string &fieldName,
                     QualType fieldTy);

   Type* initializableWith(Type *ty);

   void calculateSizeImpl();

   static bool classof(Record const * T)
   {
      return T->getTypeID() == UnionID;
   }

protected:
   unordered_map<string, Type*> fields;
   bool is_const;
};

}
}


#endif //CDOT_UNION_H
