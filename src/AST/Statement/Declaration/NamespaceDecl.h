//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_MODULEDECL_H
#define CDOT_MODULEDECL_H

#include "DeclContext.h"
#include "NamedDecl.h"

namespace cdot {
namespace ast {

class CompoundStmt;

class NamespaceDecl: public NamedDecl, public DeclContext {
public:
   NamespaceDecl(string &&nsName,
                 CompoundStmt* body,
                 bool isAnonymous)
      : NamedDecl(NamespaceDeclID, (AccessModifier)0, move(nsName)),
        DeclContext(NamespaceDeclID),
        body(body), anonymousNamespace(isAnonymous)
   {

   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == NamespaceDeclID;
   }

   static bool classof(DeclContext const* T) { return true; }

   static DeclContext *castToDeclContext(NamespaceDecl const *D)
   {
      return static_cast<DeclContext*>(const_cast<NamespaceDecl*>(D));
   }

   static NamespaceDecl *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<NamespaceDecl*>(const_cast<DeclContext*>(Ctx));
   }

   friend class TransformImpl;

protected:
   CompoundStmt* body;

   size_t namespaceId = size_t(-1);
   bool anonymousNamespace = false;

   NamespaceDecl *outerNamespace = nullptr;

public:
   CompoundStmt* getBody() const
   {
      return body;
   }

   void setBody(CompoundStmt *body)
   {
      NamespaceDecl::body = body;
   }

   bool isAnonymousNamespace() const
   {
      return anonymousNamespace;
   }

   size_t getNamespaceId() const
   {
      return namespaceId;
   }

   void setNamespaceId(size_t namespaceId)
   {
      NamespaceDecl::namespaceId = namespaceId;
   }

   NamespaceDecl *getOuterNamespace() const
   {
      return outerNamespace;
   }

   void setOuterNamespace(NamespaceDecl *outerNamespace)
   {
      this->outerNamespace = outerNamespace;
      if (outerNamespace)
         anonymousNamespace |= outerNamespace->isAnonymousNamespace();
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_MODULEDECL_H
