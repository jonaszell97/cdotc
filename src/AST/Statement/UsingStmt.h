//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_IMPORTSTMT_H
#define CDOT_IMPORTSTMT_H

#include "Statement.h"

namespace cdot {
namespace ast {

enum class UsingKind {
   FUNCTION,
   CLASS,
   VARIABLE,
   TYPEDEF,
   NAMESPACE
};

class UsingStmt : public Statement {
public:
   explicit UsingStmt(string&& importNamespace, std::vector<string>&& item);
   explicit UsingStmt(std::vector<string> &&fullNames);

   typedef std::shared_ptr<UsingStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == UsingStmtID;
   }

protected:
   string importNamespace;
   std::vector<string> importedItems;

   bool isWildcardImport = false;

   std::vector<string> fullNames;
   UsingKind kind;

   bool resolved = false;

public:
   const string &getImportNamespace() const
   {
      return importNamespace;
   }

   void setImportNamespace(const string &importNamespace)
   {
      UsingStmt::importNamespace = importNamespace;
   }

   const std::vector<string> &getImportedItems() const
   {
      return importedItems;
   }

   void setImportedItems(const std::vector<string> &importedItems)
   {
      UsingStmt::importedItems = importedItems;
   }

   bool isIsWildcardImport() const
   {
      return isWildcardImport;
   }

   void setIsWildcardImport(bool isWildcardImport)
   {
      UsingStmt::isWildcardImport = isWildcardImport;
   }

   std::vector<string> &getFullNames()
   {
      return fullNames;
   }

   void setFullNames(const std::vector<string> &fullNames)
   {
      UsingStmt::fullNames = fullNames;
   }

   UsingKind getKind() const
   {
      return kind;
   }

   void setKind(UsingKind kind)
   {
      UsingStmt::kind = kind;
   }

   bool isResolved() const
   {
      return resolved;
   }

   void setResolved(bool resolved)
   {
      UsingStmt::resolved = resolved;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_IMPORTSTMT_H
