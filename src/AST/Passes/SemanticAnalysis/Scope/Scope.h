//
// Created by Jonas Zell on 29.01.18.
//

#ifndef CDOT_SCOPE_H
#define CDOT_SCOPE_H

#include <llvm/ADT/ArrayRef.h>

namespace cdot {
namespace ast {
   class CallableDecl;
   class MethodDecl;
   class LambdaExpr;
   class RecordDecl;
   class TemplateParamDecl;
} // namespace cdot

class Scope {
public:
   enum TypeID : unsigned char {
#  define CDOT_SCOPE(Name) \
      Name##ID,
#  include "Scopes.def"
   };

   TypeID getTypeID() const
   {
      return typeID;
   }

   Scope *getEnclosingScope() const
   {
      return enclosingScope;
   }

   bool hadError() const
   {
      return HadError;
   }

   void setHadError(bool HadError)
   {
      Scope::HadError = HadError;
      if (enclosingScope)
         enclosingScope->setHadError(true);
   }

   static bool classofKind(TypeID id) { return true; }
   static bool classof(Scope const *S) { return classofKind(S->getTypeID()); }

protected:
   explicit Scope(TypeID typeID, Scope *enclosingScope = nullptr)
      : typeID(typeID), HadError(false), enclosingScope(enclosingScope)
   {}

#ifndef NDEBUG
   virtual
#endif
   ~Scope() = default;

   TypeID typeID : 7;
   bool HadError : 1;

   Scope *enclosingScope;
};

class BlockScope: public Scope {
public:
   explicit BlockScope(Scope *enclosingScope = nullptr)
      : Scope(BlockScopeID, enclosingScope), scopeID(lastID++)
   {}

   unsigned long long getScopeID() const
   {
      return scopeID;
   }

   static bool classofKind(TypeID id)
   {
      switch (id) {
         case BlockScopeID:
         case FunctionScopeID:
         case MethodScopeID:
         case LambdaScopeID:
            return true;
         default:
            return false;
      }
   }

   static bool classof(Scope const *S) { return classofKind(S->getTypeID()); }

protected:
   explicit BlockScope(TypeID id, Scope *enclosingScope = nullptr)
      : Scope(id, enclosingScope), scopeID(lastID++)
   {}

   static unsigned long long lastID;
   unsigned long long scopeID;
};

class FunctionScope: public BlockScope {
public:
   explicit FunctionScope(ast::CallableDecl *CD,
                          Scope *enclosingScope = nullptr)
      : BlockScope(FunctionScopeID, enclosingScope),
        CD(CD)
   {}

   ast::CallableDecl *getCallableDecl() const
   {
      return CD;
   }

   static bool classofKind(TypeID id)
   {
      switch (id) {
         case FunctionScopeID:
         case MethodScopeID:
         case LambdaScopeID:
            return true;
         default:
            return false;
      }
   }

   static bool classof(Scope const *S) { return classofKind(S->getTypeID()); }

protected:
   FunctionScope(TypeID typeID,
                 ast::CallableDecl *CD,
                 Scope *enclosingScope = nullptr)
      : BlockScope(typeID, enclosingScope),
        CD(CD)
   {}

   ast::CallableDecl *CD;
};

class MethodScope: public FunctionScope {
public:
   explicit MethodScope(ast::MethodDecl *M, Scope *enclosingScope = nullptr);
   ast::MethodDecl *getMethodDecl() const;

   static bool classofKind(TypeID id) { return id == MethodScopeID; }
   static bool classof(Scope const *S) { return classofKind(S->getTypeID()); }
};

class LambdaScope: public FunctionScope {
public:
   explicit LambdaScope(ast::LambdaExpr *L, Scope *enclosingScope = nullptr);

   ast::LambdaExpr *getLambdaExpr() const
   {
      return L;
   }

   static bool classofKind(TypeID id) { return id == LambdaScopeID; }
   static bool classof(Scope const *S) { return classofKind(S->getTypeID()); }

private:
   ast::LambdaExpr *L;
};

class LoopScope: public Scope {
public:
   LoopScope(bool continuable,
             bool breakable,
             bool lastCaseInMatch,
             bool nextCaseHasArgs,
             Scope *enclosingScope = nullptr)
      : Scope(LoopScopeID, enclosingScope),
        continuable(continuable),
        breakable(breakable),
        lastCaseInMatch(lastCaseInMatch),
        argsInNextCase(nextCaseHasArgs)
   { }

   bool isContinuable() const
   {
      return continuable;
   }

   bool isBreakable() const
   {
      return breakable;
   }

   bool isLastCaseInMatch() const
   {
      return lastCaseInMatch;
   }

   bool nextCaseHasArguments() const
   {
      return argsInNextCase;
   }

   static bool classofKind(TypeID id) { return id == LoopScopeID; }
   static bool classof(Scope const *S) { return classofKind(S->getTypeID()); }

private:
   bool continuable     : 1;
   bool breakable       : 1;
   bool lastCaseInMatch : 1;
   bool argsInNextCase  : 1;
};

class RecordScope: public Scope {
public:
   explicit RecordScope(ast::RecordDecl *R, Scope *enclosingScope = nullptr)
      : Scope(RecordScopeID, enclosingScope), R(R)
   {
   }

   ast::RecordDecl *getRecordDecl() const
   {
      return R;
   }

   static bool classofKind(TypeID id) { return id == RecordScopeID; }
   static bool classof(Scope const *S) { return classofKind(S->getTypeID()); }

private:
   ast::RecordDecl *R;
};

class TemplateScope: public Scope {
public:
   explicit
   TemplateScope(llvm::ArrayRef<ast::TemplateParamDecl*> templateParams,
                 Scope *enclosingScope = nullptr)
      : Scope(TemplateScopeID, enclosingScope),
        templateParams(templateParams)
   {
   }

   const llvm::ArrayRef<ast::TemplateParamDecl *> &getTemplateParams() const
   {
      return templateParams;
   }

   static bool classofKind(TypeID id) { return id == TemplateScopeID; }
   static bool classof(Scope const *S) { return classofKind(S->getTypeID()); }

private:
   llvm::ArrayRef<ast::TemplateParamDecl*> templateParams;
};

} // namespace cdot

#endif //CDOT_SCOPE_H
