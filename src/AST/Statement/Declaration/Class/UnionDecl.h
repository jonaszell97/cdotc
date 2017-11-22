//
// Created by Jonas Zell on 10.10.17.
//

#ifndef CDOT_UNIONDECL_H
#define CDOT_UNIONDECL_H

#include "RecordDecl.h"

namespace cdot {

namespace cl {
class Union;
}

namespace ast {

class UnionDecl: public RecordDecl {
public:
   typedef unordered_map<string, std::shared_ptr<TypeRef>> UnionTypes;
   typedef std::shared_ptr<UnionDecl> SharedPtr;
   typedef std::unique_ptr<UnionDecl> UniquePtr;

   UnionDecl(
      string&& name,
      UnionTypes&& types,
      bool isConst,

      std::vector<std::shared_ptr<Statement>> &&methods,
      std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
      std::vector<std::shared_ptr<PropDecl>> &&properties,

      std::vector<std::shared_ptr<Statement>> &&innerdecls
   );

   const UnionTypes &getContainedTypes() const {
      return containedTypes;
   }

   void setContainedTypes(const UnionTypes &containedTypes) {
      UnionDecl::containedTypes = containedTypes;
   }

   bool isConst() const;
   void isConst(bool is_const);

   Union *getDeclaredUnion() const {
      return declaredUnion;
   }

   void setDeclaredUnion(Union *declaredUnion) {
      UnionDecl::declaredUnion = declaredUnion;
   }

protected:
   UnionTypes containedTypes;
   bool is_const = false;

   Union *declaredUnion;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_UNIONDECL_H
