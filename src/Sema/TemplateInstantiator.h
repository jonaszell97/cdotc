//
// Created by Jonas Zell on 05.12.17.
//

#ifndef CDOT_TEMPLATEINSTANTIATOR_H
#define CDOT_TEMPLATEINSTANTIATOR_H

#include "AST/Type.h"
#include "AST/StmtOrDecl.h"
#include "Lex/SourceLocation.h"
#include "Sema/ActionResult.h"
#include "Support/Optional.h"

#include <llvm/ADT/ArrayRef.h>

#include <queue>

namespace cdot {

struct Variant;
struct SourceLocation;
class IdentifierInfo;

namespace il {
   class Constant;
} // namespace il

namespace ast {
   class Expression;
   class NamedDecl;
   class FunctionDecl;
   class RecordDecl;
   class MethodDecl;
   class Statement;
   class StaticExpr;
   class SemaPass;
   class AliasDecl;
   class CallableDecl;
} // namespace ast

namespace sema {
   class TemplateArgList;
   class MultiLevelTemplateArgList;
   class MultiLevelFinalTemplateArgList;
   class FinalTemplateArgumentList;
} // namespace sema

template<> struct IsLowBitAvailable<ast::RecordDecl*> {
   static constexpr bool value = true;
};

template<> struct IsLowBitAvailable<ast::FunctionDecl*> {
   static constexpr bool value = true;
};

template<> struct IsLowBitAvailable<ast::MethodDecl*> {
   static constexpr bool value = true;
};

template<> struct IsLowBitAvailable<ast::AliasDecl*> {
   static constexpr bool value = true;
};

using RecordInstResult   = ActionResult<ast::RecordDecl*>;
using FunctionInstResult = ActionResult<ast::FunctionDecl*>;
using MethodInstResult   = ActionResult<ast::MethodDecl*>;
using AliasInstResult    = ActionResult<ast::AliasDecl*>;

class TemplateInstantiator {
public:
   using TemplateArgs = sema::FinalTemplateArgumentList;

   explicit TemplateInstantiator(ast::SemaPass &SP)
      : SP(SP)
   {

   }

   RecordInstResult InstantiateRecord(StmtOrDecl POI,
                                      ast::RecordDecl *rec,
                                      TemplateArgs *templateArgs,
                                      bool *isNew = nullptr);

   RecordInstResult InstantiateRecord(StmtOrDecl POI,
                                      ast::RecordDecl *rec,
                                      const sema::TemplateArgList &templateArgs,
                                      bool *isNew = nullptr);

   FunctionInstResult InstantiateFunction(StmtOrDecl POI,
                                          ast::FunctionDecl *F,
                                          TemplateArgs *templateArgs,
                                          bool *isNew = nullptr);

   FunctionInstResult InstantiateFunction(StmtOrDecl POI,
                                          ast::FunctionDecl *F,
                                          const sema::TemplateArgList &templateArgs,
                                          bool *isNew = nullptr);

   MethodInstResult InstantiateMethod(StmtOrDecl POI,
                                      ast::MethodDecl *M,
                                      TemplateArgs *templateArgs,
                                      bool *isNew = nullptr);

   MethodInstResult InstantiateMethod(StmtOrDecl POI,
                                      ast::MethodDecl *M,
                                      const sema::TemplateArgList &templateArgs,
                                      bool *isNew = nullptr);

   FunctionType *InstantiateFunctionType(StmtOrDecl SOD,
                                         ast::CallableDecl *Template,
                                     const sema::TemplateArgList &templateArgs);

   MethodInstResult
   InstantiateProtocolDefaultImpl(SourceLocation instantiatedFrom,
                                  ast::RecordDecl *Rec,
                                  ast::MethodDecl *M);

   StmtResult
   InstantiateStatement(SourceLocation instantiatedFrom,
                        ast::Statement* stmt,
                        const sema::TemplateArgList &templateArgs);

   StmtResult
   InstantiateStatement(SourceLocation instantiatedFrom,
                        ast::Statement* stmt,
                        sema::MultiLevelFinalTemplateArgList &&templateArgs);

   ExprResult InstantiateTypeExpr(ast::RecordDecl *Rec, ast::Expression *E);

   DeclResult
   InstantiateDecl(SourceLocation instantiatedFrom,
                   ast::Decl* D,
                   sema::MultiLevelFinalTemplateArgList &&templateArgs);

   StmtResult InstantiateStatement(StmtOrDecl POI,
                                   ast::Statement* stmt,
                                   IdentifierInfo *SubstName,
                                   il::Constant *SubstVal);

   StmtResult InstantiateMethodBody(StmtOrDecl POI,
                                    ast::MethodDecl *Method);

   ExprResult
   InstantiateStaticExpr(SourceLocation instantiatedFrom,
                         ast::Expression* stmt,
                         const sema::TemplateArgList &templateArgs);

   AliasInstResult InstantiateAlias(ast::AliasDecl *alias,
                                    SourceLocation instantiatedFrom,
                                    TemplateArgs *templateArgs);

private:
   ast::SemaPass &SP;
   unsigned InstantiationDepth = 0;
   std::queue<ast::NamedDecl*> PendingInstantiations;

   struct InstantiationDepthRAII {
      InstantiationDepthRAII(TemplateInstantiator &Inst)
         : Inst(Inst)
      {
         ++Inst.InstantiationDepth;
      }

      ~InstantiationDepthRAII()
      {
         if (!Popped)
            pop();
      }

      void pop()
      {
         assert(!Popped && "popping twice!");
         Popped = true;
         --Inst.InstantiationDepth;
      }

   private:
      TemplateInstantiator &Inst;
      bool Popped = false;
   };

   bool checkInstantiationDepth(ast::NamedDecl *Inst);
};

} // namespace cdot

#endif //CDOT_TEMPLATEINSTANTIATOR_H
