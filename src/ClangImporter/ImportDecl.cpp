#include "ImporterImpl.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/Driver/Compiler.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/Builtin.h"
#include "cdotc/Sema/SemaPass.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdefaulted-function-deleted"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Frontend/CompilerInstance.h>

#pragma clang diagnostic pop

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::support;

using ImporterImpl = ClangImporter::ImporterImpl;

DeclarationName ImporterImpl::getName(const clang::DeclarationName& Name)
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

   auto& Tbl = CI.getContext().getDeclNameTable();
   auto& Idents = CI.getContext().getIdentifiers();

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
      return AccessSpecifier::Public;
   case clang::AccessSpecifier::AS_public:
      return AccessSpecifier::Public;
   case clang::AccessSpecifier::AS_private:
      return AccessSpecifier::Private;
   case clang::AccessSpecifier::AS_protected:
      return AccessSpecifier::Protected;
   }
}

static FunctionDecl* importFunctionDecl(ImporterImpl& I,
                                        clang::FunctionDecl* ClangFn)
{
   auto& Ctx = I.CI.getContext();
   auto RetTy = SourceType(I.getType(ClangFn->getReturnType()));
   if (!RetTy) {
      I.CI.getSema().diagnose(
          diag::warn_generic_warn, "type cannot be imported",
          I.getSourceLoc(ClangFn->getReturnTypeSourceRange()));

      return nullptr;
   }

   SmallVector<FuncArgDecl*, 4> Args;
   for (auto* Param : ClangFn->parameters()) {
      auto Loc = I.getSourceLoc(Param->getLocation());
      auto Name = I.getName(Param->getDeclName());
      auto ParamTy = I.getType(Param->getType());

      if (!ParamTy) {
         I.CI.getSema().diagnose(
             diag::warn_generic_warn, "type cannot be imported",
             I.getSourceLoc(
                 Param->getTypeSourceInfo()->getTypeLoc().getSourceRange()));

         continue;
      }

      IdentifierInfo* Label = nullptr;
      if (Name && !Name.getIdentifierInfo()->getIdentifier().startswith("_")) {
         Label = Name.getIdentifierInfo();
      }

      auto* P = FuncArgDecl::Create(
          Ctx, Loc, Loc, Name, Label, ArgumentConvention::Borrowed,
          SourceType(ParamTy), nullptr, false, false, false);

      P->setImportedFromClang(true);
      Args.push_back(P);
   }

   auto* F = FunctionDecl::Create(Ctx, getAccess(ClangFn->getAccess()),
                                  I.getSourceLoc(ClangFn->getLocation()),
                                  I.getName(ClangFn->getDeclName()), Args,
                                  RetTy, nullptr, {});

   F->setImportedFromClang(true);
   F->setCstyleVararg(ClangFn->isVariadic());
   F->setExternC(!I.IsCXX);
   F->setExternCXX(I.IsCXX);

   I.DeclMap[ClangFn] = F;
   I.CI.getSema().ActOnDecl(&I.CI.getSema().getDeclContext(), F);
   I.importDecls(ClangFn, F);

   return F;
}

static NamespaceDecl* importNamespace(ImporterImpl& I, clang::NamespaceDecl* NS)
{
   auto& Ctx = I.CI.getContext();

   DeclarationName Name;
   if (!NS->isAnonymousNamespace()) {
      Name = I.getName(NS->getDeclName());
   }

   auto* D = NamespaceDecl::Create(Ctx, I.getSourceLoc(NS->getLocStart()),
                                   I.getSourceLoc(NS->getLocStart()), Name);

   D->setImportedFromClang(true);
   D->setRBraceLoc(I.getSourceLoc(NS->getRBraceLoc()));

   I.CI.getSema().ActOnDecl(&I.CI.getSema().getDeclContext(), D);
   I.importDecls(NS, D);

   return D;
}

StructDecl* ImporterImpl::importStruct(clang::RecordDecl* ClangRec)
{
   if (DeclMap.find(ClangRec) != DeclMap.end())
      return cast<StructDecl>(DeclMap[ClangRec]);

   assert(ClangRec->isStruct() && "not a struct!");

   auto& Ctx = CI.getContext();
   auto& Sema = CI.getSema();

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

   auto* Rec = StructDecl::Create(Ctx, getAccess(ClangRec->getAccess()),
                                  getSourceLoc(ClangRec->getLocStart()), Name,
                                  {}, {});

   Rec->setImportedFromClang(true);
   DeclMap[ClangRec] = Rec;

   for (clang::FieldDecl* F : ClangRec->fields()) {
      if (F->isBitField()) {
         // Import as an opaque structure.
         Ctx.addAttribute(Rec, new (Ctx) OpaqueAttr);
         break;
      }

      auto FieldTy = getType(F->getType());
      if (!FieldTy) {
         CI.getSema().diagnose(
             diag::warn_generic_warn, "type cannot be imported",
             getSourceLoc(
                 F->getTypeSourceInfo()->getTypeLoc().getSourceRange()));

         continue;
      }

      SourceLocation FieldLoc = getSourceLoc(F->getLocStart());

      Expression* DefaultVal = nullptr;
      if (CI.getSema().hasDefaultValue(FieldTy)) {
         DefaultVal = BuiltinIdentExpr::Create(Ctx, FieldLoc,
                                               BuiltinIdentifier::defaultValue);
         DefaultVal->setExprType(FieldTy);
      }

      auto* Field = FieldDecl::Create(
          Ctx, getAccess(F->getAccess()), SourceLocation(), FieldLoc,
          getName(F->getDeclName()), SourceType(FieldTy), false,
          F->getType().isConstQualified(), DefaultVal);

      Field->setImportedFromClang(true);
      Sema.ActOnDecl(Rec, Field);
      DeclMap[F] = Field;
   }

   // Add implicit Copyable and ImplicitlyCopyable conformances.
   if (auto* Copyable = Sema.getCopyableDecl()) {
      Ctx.getConformanceTable().addExplicitConformance(Ctx, Rec, Copyable);
   }
   if (auto* Copyable = Sema.getImplicitlyCopyableDecl()) {
      Ctx.getConformanceTable().addExplicitConformance(Ctx, Rec, Copyable);
   }

   Sema.ActOnDecl(&Sema.getDeclContext(), Rec);
   importDecls(ClangRec, Rec);

   return Rec;
}

EnumDecl* ImporterImpl::importEnum(clang::EnumDecl* ClangE)
{
   if (DeclMap.find(ClangE) != DeclMap.end())
      return cast<EnumDecl>(DeclMap[ClangE]);

   auto& Ctx = CI.getContext();
   auto& Sema = CI.getSema();

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

   auto* D = EnumDecl::Create(Ctx, getAccess(ClangE->getAccess()),
                              getSourceLoc(ClangE->getLocStart()), Name, {}, {},
                              SourceType(getType(ClangE->getIntegerType())));

   D->setImportedFromClang(true);
   DeclMap[ClangE] = D;

   for (clang::EnumConstantDecl* EnumVal : ClangE->enumerators()) {
      auto Loc = getSourceLoc(EnumVal->getLocStart());
      auto ValName = getName(EnumVal->getDeclName());

      auto* Case = EnumCaseDecl::Create(Ctx, AccessSpecifier::Public, Loc, Loc,
                                        ValName, nullptr, {});

      // FIXME
      Case->setImportedFromClang(true);
      Case->setRawValue((long)EnumVal->getInitVal().getSExtValue());
      Sema.ActOnDecl(D, Case);

      DeclMap[EnumVal] = Case;
   }

   // If this is not a CXX enum class and the enum is unnamed, make all cases
   // visible as aliases.
   if (!ClangE->getDeclName()) {
      for (auto* Case : D->getCases()) {
         auto* EC = new (Ctx) EnumCaseExpr(Case->getSourceLoc(), Case);
         auto* SE = StaticExpr::Create(Ctx, EC);
         auto* Alias = AliasDecl::Create(
             Ctx, Case->getSourceLoc(), AccessSpecifier::Public,
             Case->getDeclName(), SourceType(Ctx.getRecordType(D)), SE, {});

         Sema.ActOnDecl(&Sema.getDeclContext(), Alias);
      }
   }

   Sema.ActOnDecl(&Sema.getDeclContext(), D);
   importDecls(ClangE, D);
   return D;
}

static CompoundStmt* CreateUnionGetterBody(SemaPass& SP, ASTContext& Ctx,
                                           QualType FieldTy, FieldDecl* Storage,
                                           SourceLocation Loc)
{
   // Create a reference to 'self'.
   auto* SelfRef = SelfExpr::Create(Ctx, Loc, false);

   // Create a reference to 'self._storage'.
   auto* StorageRef = MemberRefExpr::Create(Ctx, SelfRef, Storage, Loc);

   // Cast '_storage' to a pointer.
   ConversionSequenceBuilder Builder;
   Builder.addStep(CastKind::BitCast, Ctx.getUInt8PtrTy());

   auto* ConvSeq = ConversionSequence::Create(Ctx, Builder);
   auto* Cast = ImplicitCastExpr::Create(Ctx, StorageRef, ConvSeq);

   // Cast that pointer to a pointer of the right type.
   Builder = ConversionSequenceBuilder();
   Builder.addStep(CastKind::BitCast, Ctx.getPointerType(FieldTy));

   ConvSeq = ConversionSequence::Create(Ctx, Builder);
   Cast = ImplicitCastExpr::Create(Ctx, Cast, ConvSeq);

   // Dereference the pointer.
   auto* FnTy = Ctx.getFunctionType(Ctx.getMutableReferenceType(FieldTy),
                                    {Ctx.getPointerType(FieldTy)});
   Expression* Deref
       = UnaryOperator::Create(Ctx, Loc, op::Deref, FnTy, Cast, true);

   // Create a copy of the loaded value if necessary.
   if (!SP.IsImplicitlyCopyableType(FieldTy)) {
      Builder = ConversionSequenceBuilder();
      Builder.addStep(CastKind::LValueToRValue,
                      FnTy->getReturnType()->getReferencedType());

      Builder.addStep(CastKind::Copy,
                      FnTy->getReturnType()->getReferencedType());

      ConvSeq = ConversionSequence::Create(Ctx, Builder);
      Deref = ImplicitCastExpr::Create(Ctx, Deref, ConvSeq);
   }

   // Return the dereferenced pointer.
   auto* Ret = ReturnStmt::Create(Ctx, Loc, Deref);

   // Create the body.
   return CompoundStmt::Create(Ctx, Ret, false, Loc, Loc);
}

static CompoundStmt* CreateUnionSetterBody(SemaPass& SP, ASTContext& Ctx,
                                           QualType FieldTy, FieldDecl* Storage,
                                           SourceLocation Loc)
{
   // Create a reference to 'self'.
   auto* SelfRef = SelfExpr::Create(Ctx, Loc, false);

   // Create a reference to 'self._storage'.
   auto* StorageRef = MemberRefExpr::Create(Ctx, SelfRef, Storage, Loc);

   // Cast '_storage' to a pointer.
   ConversionSequenceBuilder Builder;
   Builder.addStep(CastKind::BitCast, Ctx.getUInt8PtrTy());

   auto* ConvSeq = ConversionSequence::Create(Ctx, Builder);
   auto* Cast = ImplicitCastExpr::Create(Ctx, StorageRef, ConvSeq);

   // Cast that pointer to a pointer of the right type.
   Builder = ConversionSequenceBuilder();
   Builder.addStep(CastKind::BitCast, Ctx.getPointerType(FieldTy));

   ConvSeq = ConversionSequence::Create(Ctx, Builder);
   Cast = ImplicitCastExpr::Create(Ctx, Cast, ConvSeq);

   // Dereference the pointer, yielding a reference.
   auto* FnTy = Ctx.getFunctionType(Ctx.getMutableReferenceType(FieldTy),
                                    {Ctx.getPointerType(FieldTy)});
   auto* Deref = UnaryOperator::Create(Ctx, Loc, op::Deref, FnTy, Cast, true);

   // Create a reference to the 'newValue' argument.
   auto* NewValRef
       = new (Ctx) IdentifierRefExpr(Loc, nullptr, SP.getIdentifier("newVal"));

   // Store the pointer.
   auto* Assign = AssignExpr::Create(Ctx, Loc, Deref, NewValRef, false);

   // Create the body.
   return CompoundStmt::Create(Ctx, Assign, false, Loc, Loc);
}

static PropDecl* CreateUnionAccessor(SemaPass& SP, ASTContext& Ctx,
                                     QualType FieldTy, DeclarationName Name,
                                     FieldDecl* Storage, SourceLocation Loc,
                                     bool Const)
{
   CompoundStmt* GetterBody
       = CreateUnionGetterBody(SP, Ctx, FieldTy, Storage, Loc);
   CompoundStmt* SetterBody = nullptr;

   if (!Const) {
      SetterBody = CreateUnionSetterBody(SP, Ctx, FieldTy, Storage, Loc);
   }

   SourceType Type(FieldTy);

   // Create the getter.
   DeclarationName DN = Ctx.getDeclNameTable().getAccessorName(
       *Name.getIdentifierInfo(), DeclarationName::Getter);

   auto* GetterMethod
       = MethodDecl::Create(Ctx, AccessSpecifier::Public, Loc, DN, Type,
                            SP.MakeSelfArg(Loc), {}, GetterBody, false);

   // Create the setter.
   MethodDecl* SetterMethod = nullptr;
   if (!Const) {
      DN = Ctx.getDeclNameTable().getAccessorName(*Name.getIdentifierInfo(),
                                                  DeclarationName::Setter);

      auto* NewValArg = FuncArgDecl::Create(
          Ctx, Loc, Loc, SP.getIdentifier("newVal"), nullptr,
          ArgumentConvention::Owned, Type, nullptr, false);

      NewValArg->setSynthesized(true);

      FuncArgDecl* Args[]{SP.MakeSelfArg(Loc), NewValArg};
      SetterMethod = MethodDecl::Create(Ctx, AccessSpecifier::Public, Loc, DN,
                                        SourceType(Ctx.getVoidType()), Args, {},
                                        SetterBody, false);
   }

   return PropDecl::Create(Ctx, AccessSpecifier::Public, Loc, Name,
                           SourceType(FieldTy), false, false, GetterMethod,
                           SetterMethod);
}

static InitDecl* CreateUnionInit(SemaPass& SP, ASTContext& Ctx,
                                 QualType FieldTy, DeclarationName Name,
                                 FieldDecl* Storage, SourceLocation Loc)
{
   // Create a reference to 'self'.
   auto* SelfRef = SelfExpr::Create(Ctx, Loc, false);

   // Create a reference to 'self._storage'.
   auto* StorageRef = MemberRefExpr::Create(Ctx, SelfRef, Storage, Loc);

   // Initialize with a default value to keep definitive initialization
   // analysis happy.
   auto* DefaultVal
       = BuiltinIdentExpr::Create(Ctx, Loc, BuiltinIdentifier::defaultValue);
   auto* DefaultAssign
       = AssignExpr::Create(Ctx, Loc, StorageRef, DefaultVal, false);

   // Cast '_storage' to a pointer.
   ConversionSequenceBuilder Builder;
   Builder.addStep(CastKind::BitCast, Ctx.getUInt8PtrTy());

   auto* ConvSeq = ConversionSequence::Create(Ctx, Builder);
   auto* Cast = ImplicitCastExpr::Create(Ctx, StorageRef, ConvSeq);

   // Cast that pointer to a pointer of the right type.
   Builder = ConversionSequenceBuilder();
   Builder.addStep(CastKind::BitCast, Ctx.getPointerType(FieldTy));

   ConvSeq = ConversionSequence::Create(Ctx, Builder);
   Cast = ImplicitCastExpr::Create(Ctx, Cast, ConvSeq);

   // Dereference the pointer, yielding a reference.
   auto* FnTy = Ctx.getFunctionType(Ctx.getMutableReferenceType(FieldTy),
                                    {Ctx.getPointerType(FieldTy)});
   auto* Deref = UnaryOperator::Create(Ctx, Loc, op::Deref, FnTy, Cast, true);

   // Create the argument.
   auto* ArgDecl = FuncArgDecl::Create(
       Ctx, Loc, Loc, Name, Name.getIdentifierInfo(), ArgumentConvention::Owned,
       SourceType(FieldTy), nullptr, false, false, false);

   // Create a reference to the argument.
   auto* NewValRef = DeclRefExpr::Create(Ctx, ArgDecl, Loc);

   // Store the pointer.
   auto* Assign = AssignExpr::Create(Ctx, Loc, Deref, NewValRef, false);

   // Create the body.
   auto* Body
       = CompoundStmt::Create(Ctx, {DefaultAssign, Assign}, false, Loc, Loc);

   // Create the initializer.
   return InitDecl::Create(Ctx, AccessSpecifier::Public, Loc, ArgDecl, {},
                           Body);
}

StructDecl* ImporterImpl::importUnion(clang::RecordDecl* ClangU)
{
   if (DeclMap.find(ClangU) != DeclMap.end())
      return cast<StructDecl>(DeclMap[ClangU]);

   assert(ClangU->isUnion() && "not a struct!");

   auto& Ctx = CI.getContext();
   auto& TI = CI.getContext().getTargetInfo();
   auto& Sema = CI.getSema();

   DeclarationName Name;
   if (!ClangU->getDeclName()) {
      std::string str;
      str += "_UnnamedUnion_";
      str += std::to_string(NumUnnamedStructs++);

      Name = Ctx.getIdentifiers().get(str);
   }
   else {
      Name = getName(ClangU->getDeclName());
   }

   auto* Rec
       = StructDecl::Create(Ctx, getAccess(ClangU->getAccess()),
                            getSourceLoc(ClangU->getLocStart()), Name, {}, {});

   Rec->setImportedFromClang(true);
   DeclMap[ClangU] = Rec;

   unsigned MaxSize = 0;
   unsigned MaxAlign = 1;

   // Create the union storage
   auto* Storage = FieldDecl::Create(
       Ctx, AccessSpecifier::Private, Rec->getSourceLoc(), Rec->getSourceLoc(),
       Sema.getIdentifier("_storage"), SourceType(), false, false, nullptr);

   for (clang::FieldDecl* F : ClangU->fields()) {
      if (F->isBitField()) {
         // Import as an opaque structure.
         Ctx.addAttribute(Rec, new (Ctx) OpaqueAttr);
         break;
      }

      auto FieldTy = getType(F->getType());
      if (!FieldTy) {
         CI.getSema().diagnose(
             diag::warn_generic_warn, "type cannot be imported",
             getSourceLoc(
                 F->getTypeSourceInfo()->getTypeLoc().getSourceRange()));

         continue;
      }

      auto Size = TI.getSizeOfType(FieldTy);
      auto Align = TI.getAlignOfType(FieldTy);

      if (Size > MaxSize) {
         MaxSize = Size;
      }
      if (Align > MaxAlign) {
         MaxAlign = Align;
      }

      SourceLocation FieldLoc = getSourceLoc(F->getLocStart());
      auto* Prop = CreateUnionAccessor(
          CI.getSema(), Ctx, FieldTy, getName(F->getDeclName()), Storage,
          FieldLoc, F->getType().isConstQualified());

      Prop->setImportedFromClang(true);
      Sema.ActOnDecl(Rec, Prop);
      DeclMap[F] = Prop;

      auto* Init = CreateUnionInit(CI.getSema(), Ctx, FieldTy,
                                   Prop->getDeclName(), Storage, FieldLoc);

      Init->setImportedFromClang(true);
      Sema.ActOnDecl(Rec, Init);
   }

   Storage->getType().setResolvedType(
       Ctx.getArrayType(Ctx.getUInt8Ty(), MaxSize));

   Storage->setImportedFromClang(true);
   Sema.ActOnDecl(Rec, Storage);

   Sema.ActOnDecl(&Sema.getDeclContext(), Rec);
   importDecls(ClangU, Rec);

   return Rec;
}

AliasDecl* ImporterImpl::importTypedef(clang::TypedefNameDecl* ClangTD)
{
   auto& Ctx = CI.getContext();

   auto RetTy = SourceType(getType(ClangTD->getUnderlyingType()));
   if (!RetTy) {
      CI.getSema().diagnose(
          diag::warn_generic_warn, "type cannot be imported",
          getSourceLoc(
              ClangTD->getTypeSourceInfo()->getTypeLoc().getSourceRange()));

      return nullptr;
   }

   auto Loc = getSourceLoc(ClangTD->getLocStart());
   auto* E = new (Ctx)
       IdentifierRefExpr(Loc, IdentifierKind::MetaType, Ctx.getMetaType(RetTy));

   auto Name = getName(ClangTD->getDeclName());
   auto* SE = StaticExpr::Create(Ctx, E);
   auto* Alias = AliasDecl::Create(Ctx, Loc, getAccess(ClangTD->getAccess()),
                                   Name, SourceType(Ctx.getAutoTy()), SE, {});

   Alias->setImportedFromClang(true);
   CI.getSema().ActOnDecl(&CI.getSema().getDeclContext(), Alias);

   return Alias;
}

GlobalVarDecl* ImporterImpl::importGlobalVar(clang::VarDecl* ClangVar)
{
   if (ClangVar->hasLocalStorage())
      return nullptr;

   auto& Ctx = CI.getContext();

   auto Type = getType(ClangVar->getType());
   if (!Type) {
      CI.getSema().diagnose(
          diag::warn_generic_warn, "type cannot be imported",
          getSourceLoc(
              ClangVar->getTypeSourceInfo()->getTypeLoc().getSourceRange()));

      return nullptr;
   }

   auto Access = getAccess(ClangVar->getAccess());
   auto Loc = getSourceLoc(ClangVar->getLocation());
   auto Name = getName(ClangVar->getDeclName());

   auto* GV = GlobalVarDecl::Create(Ctx, Access, Loc, Loc,
                                    ClangVar->getType().isConstQualified()
                                        || ClangVar->isConstexpr(),
                                    Name, SourceType(Type), nullptr);

   GV->setImportedFromClang(true);
   CI.getSema().ActOnDecl(&CI.getSema().getDeclContext(), GV);

   return GV;
}

Decl* ImporterImpl::importDecl(clang::Decl* D)
{
   switch (D->getKind()) {
   case clang::Decl::Function:
      return importFunctionDecl(*this, cast<clang::FunctionDecl>(D));
   case clang::Decl::Namespace:
      return importNamespace(*this, cast<clang::NamespaceDecl>(D));
   case clang::Decl::Enum:
      return importEnum(cast<clang::EnumDecl>(D));
   case clang::Decl::Record: {
      auto* R = cast<clang::RecordDecl>(D);
      switch (R->getTagKind()) {
      case clang::TTK_Struct:
         return importStruct(R);
      case clang::TTK_Enum:
         return importEnum(cast<clang::EnumDecl>(R));
      case clang::TTK_Union:
         return importUnion(R);
      default:
         break;
      }

      return nullptr;
   }
   case clang::Decl::Typedef:
   case clang::Decl::TypeAlias:
      return importTypedef(cast<clang::TypedefNameDecl>(D));
   case clang::Decl::Var:
      return importGlobalVar(cast<clang::VarDecl>(D));
   case clang::Decl::ExternCContext: {
      auto* ExternC = CompoundDecl::Create(CI.getContext(),
                                           SourceLocation(BaseOffset), true);

      CI.getSema().ActOnDecl(&CI.getSema().getDeclContext(), ExternC);

      SemaPass::DeclContextRAII DCR(CI.getSema(), ExternC);
      for (auto* SubDecl : cast<clang::ExternCContextDecl>(D)->decls()) {
         importDecl(SubDecl);
      }

      return nullptr;
   }
   // Ignore these.
   case clang::Decl::EnumConstant:
   case clang::Decl::Field:
   case clang::Decl::IndirectField:
   case clang::Decl::CXXMethod:
   case clang::Decl::ParmVar:
      return nullptr;
   default:
      CI.getSema().diagnose(diag::warn_generic_warn,
                            "cannot import declaration",
                            getSourceLoc(D->getLocation()));

      return nullptr;
   }
}

void ImporterImpl::importDecls(clang::DeclContext* ClangDC, DeclContext* DC)
{
   auto& Sema = CI.getSema();
   SemaPass::DeclContextRAII DCR(Sema, DC);

   for (auto* ClangDecl : ClangDC->decls()) {
      if (ClangDecl->getMostRecentDecl() != ClangDecl)
         continue;

      importDecl(ClangDecl);
   }
}

void ImporterImpl::importDecls(clang::ASTContext& C, DeclContext* DC)
{
   auto* TU = C.getTranslationUnitDecl();
   for (auto* ClangDecl : TU->decls()) {
      if (ClangDecl->getMostRecentDecl() != ClangDecl)
         continue;

      importDecl(ClangDecl);
   }
}