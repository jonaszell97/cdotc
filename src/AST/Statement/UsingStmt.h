//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_IMPORTSTMT_H
#define CDOT_IMPORTSTMT_H

#include "Statement.h"

namespace cdot {

namespace module {
   class Module;
} // namespace module

namespace ast {

class UsingStmt : public Statement {
public:
   UsingStmt(std::vector<string> &&declContextSpecifier,
             std::vector<string> &&importedItems,
             bool wildCardImport)
      : Statement(UsingStmtID),
        declContextSpecifier(move(declContextSpecifier)),
        importedItems(move(importedItems)),
        wildCardImport(wildCardImport)
   {}

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == UsingStmtID;
   }

   friend class TransformImpl;

protected:
   std::vector<std::string> declContextSpecifier;
   std::vector<std::string> importedItems;

   bool wildCardImport;

public:
   const std::vector<std::string> &getDeclContextSpecifier() const
   {
      return declContextSpecifier;
   }

   const std::vector<std::string> &getImportedItems() const
   {
      return importedItems;
   }

   bool isWildCardImport() const
   {
      return wildCardImport;
   }
};

class ModuleStmt: public Statement {
public:
   explicit ModuleStmt(std::vector<std::string> &&qualifiedModuleName)
      : Statement(ModuleStmtID),
        qualifiedModuleName(std::move(qualifiedModuleName))
   {}

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == ModuleStmtID;
   }

private:
   std::vector<std::string> qualifiedModuleName;

public:
   const std::vector<std::string> &getQualifiedModuleName() const
   {
      return qualifiedModuleName;
   }
};

class ImportStmt: public Statement {
public:
   explicit ImportStmt(std::vector<std::string> &&qualifiedModuleName)
      : Statement(ImportStmtID),
        qualifiedModuleName(std::move(qualifiedModuleName)),
        module(nullptr)
   {}

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == ImportStmtID;
   }

private:
   std::vector<std::string> qualifiedModuleName;
   module::Module *module;

public:
   const std::vector<std::string> &getQualifiedModuleName() const
   {
      return qualifiedModuleName;
   }

   module::Module *getModule() const
   {
      return module;
   }

   void setModule(module::Module *module)
   {
      ImportStmt::module = module;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_IMPORTSTMT_H
