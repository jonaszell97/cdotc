//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_MODULEDECL_H
#define CDOT_MODULEDECL_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class CompoundStmt;

class NamespaceDecl : public Statement {
public:
   NamespaceDecl(string &&nsName,
                 std::shared_ptr<CompoundStmt> &&body,
                 bool isAnonymous)
      : Statement(NamespaceDeclID), nsName(move(nsName)), contents(move(body)),
        isAnonymousNamespace_(isAnonymous)
   {

   }

   typedef std::shared_ptr<NamespaceDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == NamespaceDeclID;
   }

protected:
   string nsName;
   std::shared_ptr<CompoundStmt> contents;

   bool isAnonymousNamespace_ = false;

public:
   const string &getNsName() const
   {
      return nsName;
   }

   void setNsName(const string &nsName)
   {
      NamespaceDecl::nsName = nsName;
   }

   const std::shared_ptr<CompoundStmt> &getContents() const
   {
      return contents;
   }

   void setContents(const std::shared_ptr<CompoundStmt> &contents)
   {
      NamespaceDecl::contents = contents;
   }

   bool isAnonymousNamespace() const
   {
      return isAnonymousNamespace_;
   }

   void isAnonymousNamespace(bool isAnonymousNamespace)
   {
      NamespaceDecl::isAnonymousNamespace_ = isAnonymousNamespace;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_MODULEDECL_H
