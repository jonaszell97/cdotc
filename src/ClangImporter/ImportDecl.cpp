//
// Created by Jonas Zell on 30.07.18.
//

#include "ImporterImpl.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "Driver/Compiler.h"
#include "Sema/SemaPass.h"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Frontend/CompilerInstance.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::support;

using ImporterImpl = ClangImporter::ImporterImpl;

DeclarationName ImporterImpl::getName(const clang::DeclarationName &Name)
{
//   Identifier,
//      ObjCZeroArgSelector,
//      ObjCOneArgSelector,
//      ObjCMultiArgSelector,
//      CXXConstructorName,
//      CXXDestructorName,
//      CXXConversionFunctionName,
//      CXXDeductionGuideName,
//      CXXOperatorName,
//      CXXLiteralOperatorName,
//      CXXUsingDirective
   if (!Name)
      return DeclarationName();

   auto &Tbl = CI.getContext().getDeclNameTable();
   auto &Idents = CI.getContext().getIdentifiers();

   switch (Name.getNameKind()) {
   case clang::DeclarationName::Identifier:
      return DeclarationName(Idents.get(Name.getAsIdentifierInfo()->getName()));
   case clang::DeclarationName::CXXConstructorName:
      return Tbl.getConstructorName(getType(Name.getCXXNameType()));
   case clang::DeclarationName::CXXDestructorName:
      return Tbl.getConstructorName(getType(Name.getCXXNameType()));
   default:
      return DeclarationName();
   }
}

static AccessSpecifier getAccess(clang::AccessSpecifier AS)
{
   switch (AS) {
   case clang::AccessSpecifier::AS_none:
      return AccessSpecifier::Default;
   case clang::AccessSpecifier::AS_public:
      return AccessSpecifier::Public;
   case clang::AccessSpecifier::AS_private:
      return AccessSpecifier::Private;
   case clang::AccessSpecifier::AS_protected:
      return AccessSpecifier::Protected;
   }
}

static FunctionDecl *importFunctionDecl(ImporterImpl &I,
                                        clang::FunctionDecl *ClangFn) {
   auto &Ctx = I.CI.getContext();
   auto RetTy = SourceType(I.getType(ClangFn->getReturnType()));
   if (!RetTy) {
      I.CI.getSema().diagnose(
         diag::warn_generic_warn,
         "type cannot be imported",
         I.getSourceLoc(ClangFn->getReturnTypeSourceRange()));

      return nullptr;
   }

   SmallVector<FuncArgDecl*, 4> Args;
   for (auto *Param : ClangFn->parameters()) {
      auto Loc = I.getSourceLoc(Param->getLocation());
      auto Name = I.getName(Param->getDeclName());
      auto ParamTy = I.getType(Param->getType());

      if (!ParamTy) {
         I.CI.getSema().diagnose(
            diag::warn_generic_warn,
            "type cannot be imported",
            I.getSourceLoc(Param->getTypeSourceInfo()
                                ->getTypeLoc().getSourceRange()));

         continue;
      }

      auto *P = FuncArgDecl::Create(Ctx, Loc, Loc,
                                    Name, Name.getIdentifierInfo(),
                                    ArgumentConvention::Borrowed,
                                    SourceType(ParamTy),
                                    nullptr, false, false, false);

      Args.push_back(P);
   }

   auto *F = FunctionDecl::Create(Ctx, getAccess(ClangFn->getAccess()),
                                  I.getSourceLoc(ClangFn->getLocation()),
                                  I.getName(ClangFn->getDeclName()), Args,
                                  RetTy, nullptr, {});

   F->setCstyleVararg(ClangFn->isVariadic());
   F->setExternC(!I.IsCXX);
   F->setExternCXX(I.IsCXX);

   I.DeclMap[ClangFn] = F;
   I.CI.getSema().ActOnDecl(&I.CI.getSema().getDeclContext(), F);
   I.importDecls(ClangFn, F);

   return F;
}

static NamespaceDecl *importNamespace(ImporterImpl &I,
                                      clang::NamespaceDecl *NS) {
   auto &Ctx = I.CI.getContext();

   DeclarationName Name;
   if (!NS->isAnonymousNamespace()) {
      Name = I.getName(NS->getDeclName());
   }

   auto *D = NamespaceDecl::Create(Ctx,
                                   I.getSourceLoc(NS->getLocStart()),
                                   I.getSourceLoc(NS->getLocStart()),
                                   Name);

   D->setRBraceLoc(I.getSourceLoc(NS->getRBraceLoc()));

   I.CI.getSema().ActOnDecl(&I.CI.getSema().getDeclContext(), D);
   I.importDecls(NS, D);

   return D;
}

StructDecl *ImporterImpl::importStruct(clang::RecordDecl *ClangRec)
{
   if (DeclMap.find(ClangRec) != DeclMap.end())
      return cast<StructDecl>(DeclMap[ClangRec]);

   assert(ClangRec->isStruct() && "not a struct!");

   auto &Ctx = CI.getContext();
   auto &Sema = CI.getSema();

   DeclarationName Name;
   if (!ClangRec->getDeclName()) {
      std::string str;
      str += "_UnnamedStruct_";
      str += std::to_string(NumUnnamedStructs++);

      Name = Ctx.getIdentifiers().get(str);
   }
   else {
      Name = getName(ClangRec->getDeclName());
   }

   auto *Rec = StructDecl::Create(Ctx, getAccess(ClangRec->getAccess()),
                                  getSourceLoc(ClangRec->getLocStart()),
                                  Name, {}, {});

   DeclMap[ClangRec] = Rec;

   for (clang::FieldDecl *F : ClangRec->fields()) {
      if (F->isBitField()) {
         // TODO
         Sema.diagnose(diag::warn_generic_warn,
                       "cannot import bitfields",
                       getSourceLoc(F->getLocStart()));

         continue;
      }

      auto FieldTy = getType(F->getType());
      if (!FieldTy) {
         CI.getSema().diagnose(
            diag::warn_generic_warn,
            "type cannot be imported",
            getSourceLoc(F->getTypeSourceInfo()
                            ->getTypeLoc().getSourceRange()));

         continue;
      }

      auto *Field = FieldDecl::Create(Ctx, getAccess(F->getAccess()),
                                      SourceLocation(),
                                      getSourceLoc(F->getLocStart()),
                                      getName(F->getDeclName()),
                                      SourceType(FieldTy), false,
                                      F->getType().isConstQualified(), nullptr);

      Sema.ActOnDecl(Rec, Field);
      DeclMap[F] = Field;
   }

   Sema.ActOnDecl(&Sema.getDeclContext(), Rec);
   importDecls(ClangRec, Rec);

   return Rec;
}

EnumDecl *ImporterImpl::importEnum(clang::EnumDecl *ClangE)
{
   if (DeclMap.find(ClangE) != DeclMap.end())
      return cast<EnumDecl>(DeclMap[ClangE]);

   auto &Ctx = CI.getContext();
   auto &Sema = CI.getSema();

   DeclarationName Name;
   if (!ClangE->getDeclName()) {
      std::string str;
      str += "_UnnamedEnum_";
      str += std::to_string(NumUnnamedEnums++);

      Name = Ctx.getIdentifiers().get(str);
   }
   else {
      Name = getName(ClangE->getDeclName());
   }

   auto *D = EnumDecl::Create(Ctx, getAccess(ClangE->getAccess()),
                              getSourceLoc(ClangE->getLocStart()),
                              Name, {}, {},
                              SourceType(getType(ClangE->getIntegerType())));

   DeclMap[ClangE] = D;

   for (clang::EnumConstantDecl *EnumVal : ClangE->enumerators()) {
      auto Loc = getSourceLoc(EnumVal->getLocStart());
      auto ValName = getName(EnumVal->getDeclName());

      auto *Case = EnumCaseDecl::Create(Ctx, AccessSpecifier::Public,
                                        Loc, Loc, ValName, nullptr, {});

      // FIXME
      Case->setRawValue((long)EnumVal->getInitVal().getSExtValue());
      Sema.ActOnDecl(D, Case);

      DeclMap[EnumVal] = Case;
   }

   // If this is not a CXX enum class and the enum is unnamed, make all cases
   // visible as aliases.
   if (!ClangE->getDeclName()) {
      for (auto *Case : D->getCases()) {
         auto *EC = new(Ctx) EnumCaseExpr(Case->getSourceLoc(), Case);
         EC->setExprType(Ctx.getRecordType(D));
         EC->setSemanticallyChecked(true);

         auto *SE = StaticExpr::Create(Ctx, EC);
         auto *Alias = AliasDecl::Create(Ctx, Case->getSourceLoc(),
                                         AccessSpecifier::Public,
                                         Case->getDeclName(),
                                         SourceType(Ctx.getRecordType(D)),
                                         SE, {});

         Sema.ActOnDecl(&Sema.getDeclContext(), Alias);
      }
   }

   Sema.ActOnDecl(&Sema.getDeclContext(), D);
   importDecls(ClangE, D);
   return D;
}

AliasDecl* ImporterImpl::importTypedef(clang::TypedefNameDecl *ClangTD)
{
   auto &Ctx = CI.getContext();

   auto RetTy = SourceType(getType(ClangTD->getUnderlyingType()));
   if (!RetTy) {
      CI.getSema().diagnose(
         diag::warn_generic_warn,
         "type cannot be imported",
         getSourceLoc(ClangTD->getTypeSourceInfo()->getTypeLoc()
                             .getSourceRange()));

      return nullptr;
   }

   auto Loc = getSourceLoc(ClangTD->getLocStart());
   auto *E = new(Ctx) IdentifierRefExpr(Loc, IdentifierKind::MetaType,
                                        Ctx.getMetaType(RetTy));

   auto Name = getName(ClangTD->getDeclName());
   auto *SE = StaticExpr::Create(Ctx, E);
   auto *Alias = AliasDecl::Create(Ctx, Loc, getAccess(ClangTD->getAccess()),
                                   Name, SourceType(Ctx.getAutoTy()),
                                   SE, {});

   CI.getSema().ActOnDecl(&CI.getSema().getDeclContext(), Alias);
   return Alias;
}

Decl* ImporterImpl::importDecl(clang::Decl *D)
{
   switch (D->getKind()) {
   case clang::Decl::Function:
      return importFunctionDecl(*this, cast<clang::FunctionDecl>(D));
   case clang::Decl::Namespace:
      return importNamespace(*this, cast<clang::NamespaceDecl>(D));
   case clang::Decl::Record: {
      auto *R = cast<clang::RecordDecl>(D);
      switch (R->getTagKind()) {
      case clang::TTK_Struct:
         return importStruct(R);
      case clang::TTK_Enum:
         return importEnum(cast<clang::EnumDecl>(R));
      default:
         break;
      }

      return nullptr;
   }
   case clang::Decl::Typedef:
   case clang::Decl::TypeAlias:
      return importTypedef(cast<clang::TypedefNameDecl>(D));
   // Ignore these.
   case clang::Decl::EnumConstant:
   case clang::Decl::Field:
   case clang::Decl::CXXMethod:
   default:
      return nullptr;
   }
}

void ImporterImpl::importDecls(clang::DeclContext *ClangDC, DeclContext *DC)
{
   auto &Sema = CI.getSema();
   SemaPass::DeclContextRAII DCR(Sema, DC);

   for (auto *ClangDecl : ClangDC->decls()) {
      if (ClangDecl->getMostRecentDecl() != ClangDecl)
         continue;

      importDecl(ClangDecl);
   }
}

void ImporterImpl::importDecls(clang::ASTContext &C, DeclContext *DC)
{
   auto *TU = C.getTranslationUnitDecl();
   for (auto *ClangDecl : TU->decls()) {
      if (ClangDecl->getMostRecentDecl() != ClangDecl)
         continue;

      importDecl(ClangDecl);
   }
}