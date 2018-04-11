//
// Created by Jonas Zell on 05.04.18.
//

#include "Sema/SemaPass.h"

using namespace cdot::diag;

namespace cdot {
namespace ast {

enum ReflectionIdent {
   SizeOf, AlignOf, Line, Column, FileName, SourceLoc,
};

void SemaPass::initReflectionIdents()
{
   auto &Idents = Context.getIdentifiers();

   ReflectionIdents[SizeOf] = &Idents.get("sizeOf");
   ReflectionIdents[AlignOf] = &Idents.get("alignOf");
   ReflectionIdents[Line] = &Idents.get("line");
   ReflectionIdents[Column] = &Idents.get("column");
   ReflectionIdents[FileName] = &Idents.get("fileName");
   ReflectionIdents[SourceLoc] = &Idents.get("sourceLocation");

   ReflectionIdentsInitialized = true;
}

ExprResult SemaPass::HandleReflectionAlias(AliasDecl *Alias, Expression *Expr)
{
   auto Name = Alias->getDeclName().getManglingName();
   if (!Name.isSimpleIdentifier()) {
      diagnose(Expr, err_compiler_ns_unknown_entity, Alias->getDeclName(),
               Alias->getSourceRange());

      return ExprError();
   }

   if (!ReflectionIdentsInitialized)
      initReflectionIdents();

   Expression *ResultExpr = nullptr;

   auto *II = Name.getIdentifierInfo();
   if (II == ReflectionIdents[SizeOf]) {
      if (!Alias->isInstantiation() || Alias->getTemplateArgs().size() != 1
                              || !Alias->getTemplateArgs().front().isType()) {
         diagnose(Expr, err_compiler_ns_bad_def, Alias->getDeclName(),
                  Alias->getSourceRange());

         return ExprError();
      }

      QualType Ty = Alias->getTemplateArgs().front().getType();
      if (Ty->isDependentType()) {
         Expr->setIsTypeDependent(true);
         Expr->setExprType(Context.getUIntTy());

         return Expr;
      }

      auto &TI = Context.getTargetInfo();
      auto Size = TI.getSizeOfType(Ty);

      ResultExpr = IntegerLiteral::Create(Context, Expr->getSourceRange(),
                                          Context.getUIntTy(),
                                          llvm::APSInt(llvm::APInt(64, Size)));
   }
   else if (II == ReflectionIdents[AlignOf]) {
      if (!Alias->isInstantiation() || Alias->getTemplateArgs().size() != 1
                              || !Alias->getTemplateArgs().front().isType()) {
         diagnose(Expr, err_compiler_ns_bad_def, Alias->getDeclName(),
                  Alias->getSourceRange());

         return ExprError();
      }

      QualType Ty = Alias->getTemplateArgs().front().getType();
      if (Ty->isDependentType()) {
         Expr->setIsTypeDependent(true);
         Expr->setExprType(Context.getUIntTy());

         return Expr;
      }

      auto &TI = Context.getTargetInfo();
      auto Size = TI.getAlignOfType(Ty);

      ResultExpr = IntegerLiteral::Create(Context, Expr->getSourceRange(),
                                          Context.getUIntTy(),
                                          llvm::APSInt(llvm::APInt(64, Size)));
   }
   else if (II == ReflectionIdents[Line]) {
      auto LineAndCol = Diags.getFileMgr()->getLineAndCol(Expr->getSourceLoc());
      ResultExpr = IntegerLiteral::Create(Context, Expr->getSourceRange(),
                                          Context.getUIntTy(),
                                          llvm::APSInt(llvm::APInt(
                                             64, LineAndCol.line)));
   }
   else if (II == ReflectionIdents[Column]) {
      auto LineAndCol = Diags.getFileMgr()->getLineAndCol(Expr->getSourceLoc());
      ResultExpr = IntegerLiteral::Create(Context, Expr->getSourceRange(),
                                          Context.getUIntTy(),
                                          llvm::APSInt(llvm::APInt(
                                             64, LineAndCol.col)));
   }
   else if (II == ReflectionIdents[FileName]) {
      auto FileName = Diags.getFileMgr()->getFileName(Expr->getSourceLoc());
      auto Str = StringLiteral::Create(Context, Expr->getSourceRange(),
                                       FileName.str());

      Str->setContextualType(Context.getRecordType(getStringViewDecl()));
      ResultExpr = Str;
   }

   if (!ResultExpr) {
      diagnose(Expr, err_compiler_ns_unknown_entity, Alias->getDeclName(),
               Alias->getSourceRange());

      return ExprError();
   }

   return visitExpr(ResultExpr);
}

} // namespace ast
} // namespace cdot