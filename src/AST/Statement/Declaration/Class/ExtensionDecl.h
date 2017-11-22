//
// Created by Jonas Zell on 14.11.17.
//

#ifndef CDOT_EXTENSIONDECL_H
#define CDOT_EXTENSIONDECL_H

#include "RecordDecl.h"

namespace cdot {
namespace ast {

class ExtensionDecl: public RecordDecl {
public:
   ExtensionDecl(
      AccessModifier am,
      string &&recordName,

      std::vector<std::shared_ptr<TypeRef>> &&conformsTo,

      std::vector<std::shared_ptr<Statement>> &&methods,
      std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
      std::vector<std::shared_ptr<PropDecl>> &&properties,

      std::vector<std::shared_ptr<ConstrDecl>> &&initializers,
      std::vector<Statement::SharedPtr> &&innerDeclarations
   );

   typedef std::shared_ptr<ExtensionDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ExtensionDeclID;
   }

protected:
   std::vector<std::shared_ptr<ConstrDecl>> initializers;

public:
   const std::vector<std::shared_ptr<ConstrDecl>> &getInitializers() const;
   void setInitializers(
      const std::vector<std::shared_ptr<ConstrDecl>> &initializers);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_EXTENSIONDECL_H
