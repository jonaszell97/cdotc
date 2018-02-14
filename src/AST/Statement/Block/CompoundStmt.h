//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_COMPOUNDSTATEMENT_H
#define CDOT_COMPOUNDSTATEMENT_H

#include "../Declaration/NamedDecl.h"
#include "../Declaration/DeclContext.h"

namespace cdot {
namespace ast {

class ImportStmt;

class CompoundStmt : public Statement {
public:
   explicit CompoundStmt(bool preserveScope = false);

   void addStatement(Statement* stmt)
   {
      statements.push_back(stmt);
   }

   CompoundStmt(std::vector<Statement* > &&stmts,
                bool preserveScope);

   bool preservesScope() const
   {
      return preserveScope;
   }
   
   size_t size()
   {
      return statements.size();
   }

   std::vector<Statement*> const& getStatements() const
   {
      return statements;
   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == CompoundStmtID;
   }

   friend class TransformImpl;

protected:
   std::vector<Statement*> statements;
   bool preserveScope = false;
};

class TranslationUnit: public NamedDecl, public DeclContext {
public:
   TranslationUnit(std::string &&fileName, size_t sourceId)
      : NamedDecl(TranslationUnitID, (AccessModifier)0, move(fileName)),
        DeclContext(TranslationUnitID),
        sourceId(sourceId),
        importStmts(nullptr), statements(nullptr)
   {}

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      return kind == TranslationUnitID;
   }

   static bool classof(DeclContext const* T)
   {
      return T->getDeclKind() == TranslationUnitID;
   }

   static DeclContext *castToDeclContext(TranslationUnit const *D)
   {
      return static_cast<DeclContext*>(const_cast<TranslationUnit*>(D));
   }

   static TranslationUnit *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<TranslationUnit*>(const_cast<DeclContext*>(Ctx));
   }

   friend class TransformImpl;

private:
   size_t sourceId;

   size_t numImports = 0;
   size_t numStmts = 0;

   ImportStmt** importStmts;
   Statement** statements;

public:
   const std::string &getFileName() const
   {
      return name;
   }

   size_t getSourceId() const
   {
      return sourceId;
   }

   using import_range = llvm::iterator_range<ImportStmt**>;
   using stmt_range   = llvm::iterator_range<Statement**>;

   import_range getImports() const
   {
      return import_range(importStmts, importStmts + numImports);
   }

   stmt_range getStatements() const
   {
      return stmt_range(statements, statements + numStmts);
   }

   ImportStmt *getFirstImport() const { return importStmts[0]; }
   Statement *getFirstStmt()    const { return statements[0]; }

   void setImportStmts(size_t numImports, ImportStmt **importStmts)
   {
      TranslationUnit::numImports = numImports;
      TranslationUnit::importStmts = importStmts;
   }

   void setStatements(size_t numStmts, Statement **statements)
   {
      TranslationUnit::numStmts = numStmts;
      TranslationUnit::statements = statements;
   }
};

class GlobalDeclContext: public DeclContext {
public:
   GlobalDeclContext()
      // using a sentinel value here because this class should never be cast
      // to or from an ASTNode
      : DeclContext(AstNode::BreakStmtID)
   {}

   static bool classofKind(AstNode::NodeType kind)
   {
      return kind == AstNode::BreakStmtID;
   }

   static bool classof(DeclContext const *T)
   {
      return classofKind(T->getDeclKind());
   }

   static DeclContext *castToDeclContext(GlobalDeclContext const *D)
   {
      return static_cast<DeclContext*>(const_cast<GlobalDeclContext*>(D));
   }

   static GlobalDeclContext *castFromDeclContext(DeclContext const *Ctx)
   {
      return static_cast<GlobalDeclContext*>(const_cast<DeclContext*>(Ctx));
   }

private:
   TranslationUnit **translationUnits = nullptr;
   size_t translationUnitCount = 0;

public:
   using iterator_range = llvm::iterator_range<TranslationUnit**>;

   iterator_range getTranslationUnits() const
   {
      return iterator_range(translationUnits,
                            translationUnits + translationUnitCount);
   }

   size_t getTranslationUnitCount() const
   {
      return translationUnitCount;
   }

   void setTranslationUnits(TranslationUnit **translationUnits,
                            size_t numTranslationUnits) {
      GlobalDeclContext::translationUnits = translationUnits;
      GlobalDeclContext::translationUnitCount = numTranslationUnits;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_COMPOUNDSTATEMENT_H
