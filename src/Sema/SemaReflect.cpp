//
// Created by Jonas Zell on 05.04.18.
//

#include "SemaPass.h"

#include "Basic/TargetInfo.h"
#include "IL/Constants.h"
#include "IL/Function.h"
#include "IL/GlobalVariable.h"
#include "ILGen/ILGenPass.h"

using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

namespace cdot {
namespace ast {

enum ReflectionIdent {
   sizeOf = 0, alignOf, line, column, fileName, sourceLocation,
   function, mangledFunction, targetInfo, RI_inCTFE,

   mirror,

   SourceLocation, TargetInfo,
   RI_AccessSpecifier, RI_Type, RI_NamedDecl, RI_RecordDecl, RI_FunctionDecl,
   RI_StructDecl, RI_ClassDecl, RI_EnumDecl, RI_UnionDecl, RI_EnumCaseDecl,
   RI_FieldDecl, RI_MethodDecl, RI_FuncArgDecl, RI_VarDecl,

   RI_debug,
   Endianness, little, big,
   OperatingSystem,

   underlyingType, IsDefaultInitializable, defaultValue,

   _last
};

void SemaPass::initReflectionIdents()
{
   static_assert(sizeof(ReflectionIdents) / sizeof(IdentifierInfo*) > _last,
                 "not enough space for reflection identifiers!");

   auto &Idents = Context.getIdentifiers();

   ReflectionIdents[sizeOf] = &Idents.get("sizeOf");
   ReflectionIdents[alignOf] = &Idents.get("alignOf");
   ReflectionIdents[line] = &Idents.get("line");
   ReflectionIdents[column] = &Idents.get("column");
   ReflectionIdents[fileName] = &Idents.get("fileName");
   ReflectionIdents[sourceLocation] = &Idents.get("sourceLocation");
   ReflectionIdents[function] = &Idents.get("function");
   ReflectionIdents[mangledFunction] = &Idents.get("mangledFunction");
   ReflectionIdents[targetInfo] = &Idents.get("targetInfo");
   ReflectionIdents[mirror] = &Idents.get("mirror");
   ReflectionIdents[RI_inCTFE] = &Idents.get("inCTFE");

   ReflectionIdents[SourceLocation] = &Idents.get("SourceLocation");
   ReflectionIdents[TargetInfo] = &Idents.get("TargetInfo");

   ReflectionIdents[RI_AccessSpecifier] = &Idents.get("AccessSpecifier");
   ReflectionIdents[RI_Type] = &Idents.get("Type");
   ReflectionIdents[RI_NamedDecl] = &Idents.get("NamedDecl");
   ReflectionIdents[RI_RecordDecl] = &Idents.get("Record");
   ReflectionIdents[RI_StructDecl] = &Idents.get("Struct");
   ReflectionIdents[RI_ClassDecl] = &Idents.get("Class");
   ReflectionIdents[RI_UnionDecl] = &Idents.get("Union");
   ReflectionIdents[RI_EnumDecl] = &Idents.get("Enum");
   ReflectionIdents[RI_EnumCaseDecl] = &Idents.get("EnumCase");
   ReflectionIdents[RI_FieldDecl] = &Idents.get("Field");
   ReflectionIdents[RI_FunctionDecl] = &Idents.get("Function");
   ReflectionIdents[RI_MethodDecl] = &Idents.get("Method");
   ReflectionIdents[RI_FuncArgDecl] = &Idents.get("Argument");
   ReflectionIdents[RI_VarDecl] = &Idents.get("VarDecl");

   ReflectionIdents[RI_debug] = &Idents.get("debug");

   ReflectionIdents[Endianness] = &Idents.get("Endianness");
   ReflectionIdents[little] = &Idents.get("little");
   ReflectionIdents[big] = &Idents.get("big");

   ReflectionIdents[OperatingSystem] = &Idents.get("OperatingSystem");
   ReflectionIdents[underlyingType] = &Idents.get("underlyingType");
   ReflectionIdents[IsDefaultInitializable] = &Idents.get("IsDefaultInitializable");
   ReflectionIdents[defaultValue] = &Idents.get("defaultValue");

   ReflectionIdentsInitialized = true;
}

class ReflectionBuilder {
   SemaPass &SP;
   ast::ASTContext &Context;
   il::ILBuilder &Builder;
   Expression *Expr;
   llvm::DenseMap<uintptr_t, il::Constant*> ReflMap;
   il::ConstantEnum *AccessSpecConstants[5] = { nullptr };

public:
   ReflectionBuilder(SemaPass &SP, Expression *Expr)
      : SP(SP), Context(SP.getContext()),
        Builder(SP.getILGen().Builder), Expr(Expr)
   {}

   void setExpr(Expression *E) { Expr = E; }

   std::pair<il::Constant*, QualType> getMirror(QualType Ty)
   {
      auto C = ReflectType(Ty);
      return { C, C->getType() };
   }

   std::pair<il::Constant*, QualType> getMirror(NamedDecl *ND)
   {
      auto C = BuildNamedDeclMirror(ND);
      if (auto GV = dyn_cast<il::GlobalVariable>(C)) {
         return { GV, GV->getInitializer()->getType() };
      }

      return { C, C->getType() };
   }

private:
   void cacheValue(NamedDecl *ND, il::Constant *C)
   {
      ReflMap[(uintptr_t)ND] = C;
   }

   void cacheValue(QualType Ty, il::Constant *C)
   {
      ReflMap[(uintptr_t)Ty.getAsOpaquePtr()] = C;
   }

   il::Constant *getCachedValue(NamedDecl *ND)
   {
      return ReflMap[(uintptr_t)ND];
   }

   il::Constant *getCachedValue(QualType Ty)
   {
      return ReflMap[(uintptr_t)Ty.getAsOpaquePtr()];
   }

   il::Constant *ReflectType(QualType Ty)
   {
      if (Ty->isRecordType()) {
         return BuildRecordMirror(Ty->getRecord());
      }

      return BuildType(Ty);
   }

   il::GlobalVariable *MakeGlobal(il::Constant *C, struct SourceLocation Loc)
   {
      auto GV = Builder.CreateGlobalVariable(C, true, StringRef(), Loc);
      GV->setLinkage(il::GlobalVariable::InternalLinkage);
      GV->setUnnamedAddr(il::GlobalVariable::UnnamedAddr::Global);

      return GV;
   }

   il::ConstantClass *BuildNamedDeclMirror(NamedDecl *ND)
   {
      switch (ND->getKind()) {
      case Decl::EnumDeclID:
         return BuildEnumMirror(cast<EnumDecl>(ND));
      case Decl::StructDeclID:
         return BuildStructMirror(cast<StructDecl>(ND));
      case Decl::ClassDeclID:
         return BuildClassMirror(cast<ClassDecl>(ND));
      case Decl::UnionDeclID:
         return BuildUnionMirror(cast<UnionDecl>(ND));
      default:
         llvm_unreachable("");
      }
   }

   il::ConstantClass *BuildRecordMirror(RecordDecl *R)
   {
      switch (R->getKind()) {
      case Decl::EnumDeclID:
         return BuildEnumMirror(cast<EnumDecl>(R));
      case Decl::StructDeclID:
         return BuildStructMirror(cast<StructDecl>(R));
      case Decl::ClassDeclID:
         return BuildClassMirror(cast<ClassDecl>(R));
      case Decl::UnionDeclID:
         return BuildUnionMirror(cast<UnionDecl>(R));
      default:
         llvm_unreachable("");
      }
   }

   il::Constant *BuildStringView(llvm::StringRef Str);
   il::Constant *BuildStringView(DeclarationName Name)
   {
      std::string str;
      llvm::raw_string_ostream OS(str);
      OS << Name;

      return BuildStringView(OS.str());
   }

   il::Constant *BuildArrayView(QualType ElementTy,
                                llvm::ArrayRef<il::Constant*> Values);

   il::Constant *BuildOption(QualType Ty, il::Constant *Val = nullptr);

   il::ConstantEnum *BuildAccessSpec(AccessSpecifier AS);
   il::ConstantEnum *BuildType(QualType Ty);

   il::ConstantInt *BuildWord(uint64_t Val)
   {
      return Builder.GetConstantInt(Context.getUIntTy(), Val);
   }

   il::ConstantClass *BuildNamedDeclBase(NamedDecl *ND);
   il::ConstantClass *BuildVarDeclBase(VarDecl *Var);

   il::ConstantClass *BuildFieldMirror(FieldDecl *F);
   il::ConstantClass *BuildArgumentMirror(FuncArgDecl *Arg);
   il::ConstantClass *BuildMethodMirror(MethodDecl *M);

   il::ConstantClass *BuildRecordBase(RecordDecl *R);
   il::ConstantClass *BuildFunctionBase(CallableDecl *F, bool cache = true);

   il::ConstantClass *BuildStructMirror(StructDecl *S, bool cache = true);

   il::ConstantClass *BuildClassMirror(ClassDecl *C);
   il::ConstantClass *BuildUnionMirror(UnionDecl *U);

   il::ConstantClass *BuildEnumMirror(EnumDecl *E);
   il::ConstantClass *BuildEnumCaseMirror(EnumCaseDecl *Case);

   il::GlobalVariable *GetTypeInfo(RecordDecl *R)
   {
      return SP.getILGen().GetTypeInfo(SP.getContext().getRecordType(R));
   }
};

static RecordDecl *requireStringViewDecl(SemaPass &SP, Expression *Expr)
{
   auto SV = SP.getStringViewDecl();
   if (!SV) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "StringView");

      return nullptr;
   }

   return SV;
}

il::Constant *ReflectionBuilder::BuildStringView(llvm::StringRef Str)
{
   auto SV = requireStringViewDecl(SP, Expr);
   if (!SV) {
      return nullptr;
   }

   return SP.getILGen().MakeStringView(Str);
}

il::Constant*
ReflectionBuilder::BuildArrayView(QualType ElementTy,
                                  llvm::ArrayRef<il::Constant*> Values) {
   auto AV = SP.getArrayViewDecl();
   if (!AV) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "ArrayView");

      return nullptr;
   }

   ResolvedTemplateArg Arg(AV->getTemplateParams().front(), ElementTy);
   auto *TemplateArgs = FinalTemplateArgumentList::Create(Context, { Arg });

   auto Inst = SP.Instantiator.InstantiateRecord(Expr, AV, TemplateArgs);
   if (!Inst)
      return nullptr;

   auto *ArrTy = Context.getArrayType(ElementTy, Values.size());
   auto *Arr = Builder.GetConstantArray(ArrTy, Values);
   auto *GV = Builder.CreateGlobalVariable(Arr, true, "", AV->getSourceLoc());

   auto *Size = Builder.GetConstantInt(Context.getUIntTy(), Values.size());
   auto *FirstElPtr = il::ConstantExpr::getBitCast(
      GV, ElementTy->getPointerTo(Context));

   return Builder.GetConstantStruct(
      cast<StructDecl>(Inst.getValue()), { FirstElPtr, Size });
}

il::Constant* ReflectionBuilder::BuildOption(QualType Ty, il::Constant *Val)
{
   auto Opt = SP.getOptionDecl();
   if (!Opt) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "Option");

      return nullptr;
   }

   ResolvedTemplateArg Arg(Opt->getTemplateParams().front(), Ty);
   auto *TemplateArgs = FinalTemplateArgumentList::Create(Context, { Arg });

   auto Inst = SP.Instantiator.InstantiateRecord(Expr, Opt, TemplateArgs);
   if (!Inst)
      return nullptr;

   auto InstVal = cast<EnumDecl>(Inst.getValue());
   auto case_it = InstVal->decl_begin<EnumCaseDecl>();

   if (Val) {
      ++case_it;
      return Builder.GetConstantEnum(*case_it, { Val });
   }

   return Builder.GetConstantEnum(*case_it, {});
}

il::ConstantEnum *ReflectionBuilder::BuildAccessSpec(AccessSpecifier AS)
{
   if (auto Val = AccessSpecConstants[(unsigned)AS])
      return Val;

   auto ASDecl = dyn_cast_or_null<EnumDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_AccessSpecifier]]);

   if (!ASDecl) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "AccessSpecifier");

      return nullptr;
   }

   auto case_it = ASDecl->decl_begin<EnumCaseDecl>();
   for (unsigned i = 0; i < (unsigned)AS; i++) {
      ++case_it;
   }

   auto Val = Builder.GetConstantEnum(*case_it, {});
   AccessSpecConstants[(unsigned)AS] = Val;

   return Val;
}

il::ConstantEnum* ReflectionBuilder::BuildType(QualType Ty)
{
   if (auto Val = getCachedValue(Ty))
      return cast<il::ConstantEnum>(Val);

   auto TypeDecl = dyn_cast_or_null<EnumDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_Type]]);

   if (!TypeDecl) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "Type");

      return nullptr;
   }

   enum TypeKinds : unsigned {
      VoidTy = 0,
      IntegerTy,
      FloatTy,
      DoubleTy,
      PointerTy,
      ReferenceTy,
      ArrayTy,
      TupleTy,
      FunctionTy,
      RecordTy,
   };

   TypeKinds Kind;
   llvm::SmallVector<il::Constant*, 4> CaseVals;

   switch (Ty->getTypeID()) {
   case Type::BuiltinTypeID: {
      if (Ty->isIntegerType()) {
         Kind = IntegerTy;
         CaseVals.push_back(Builder.GetConstantInt(Context.getUIntTy(),
                                                   Ty->getBitwidth()));
         CaseVals.push_back(Ty->isUnsigned() ? Builder.GetTrue()
                                             : Builder.GetFalse());
      }
      else if (Ty->isVoidType()) {
         Kind = VoidTy;
      }
      else {
         assert(Ty->isFPType() && "bad builtin type kind");
         Kind = Ty->isFloatTy() ? FloatTy : DoubleTy;
      }

      break;
   }
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
      Kind = PointerTy;
      CaseVals.push_back(BuildType(Ty->getPointeeType()));
      CaseVals.push_back(Ty->isMutablePointerType() ? Builder.GetTrue()
                                                    : Builder.GetFalse());

      break;
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
      Kind = ReferenceTy;
      CaseVals.push_back(BuildType(Ty->getReferencedType()));
      CaseVals.push_back(Ty->isMutableReferenceType() ? Builder.GetTrue()
                                                      : Builder.GetFalse());

      break;
   case Type::ArrayTypeID:
      Kind = ArrayTy;
      CaseVals.push_back(BuildType(Ty->asArrayType()->getElementType()));
      CaseVals.push_back(BuildWord(Ty->asArrayType()->getNumElements()));

      break;
   case Type::TupleTypeID: {
      Kind = TupleTy;

      auto Tup = Ty->asTupleType();
      for (auto &Cont : Tup->getContainedTypes()) {
         CaseVals.push_back(BuildType(Cont));
      }

      auto *Arr = BuildArrayView(Context.getRecordType(TypeDecl), CaseVals);
      CaseVals.clear();
      CaseVals.push_back(Arr);

      break;
   }
   case Type::FunctionTypeID:
   case Type::LambdaTypeID: {
      Kind = FunctionTy;

      auto Func = Ty->asFunctionType();
      for (auto &Cont : Func->getParamTypes()) {
         CaseVals.push_back(BuildType(Cont));
      }

      auto *Ret = BuildType(Func->getReturnType());
      auto *Args = BuildArrayView(Context.getRecordType(TypeDecl), CaseVals);
      auto *Thin = Ty->isLambdaType() ? Builder.GetFalse() : Builder.GetTrue();

      CaseVals.clear();
      CaseVals.push_back(Ret);
      CaseVals.push_back(Args);
      CaseVals.push_back(Thin);

      break;
   }
   case Type::RecordTypeID:
      Kind = RecordTy;
      CaseVals.push_back(BuildRecordMirror(Ty->getRecord()));
      break;
   default:
      llvm_unreachable("should not appear here!");
   }

   auto case_it = TypeDecl->decl_begin<EnumCaseDecl>();
   for (unsigned i = 0; i < Kind; i++) {
      ++case_it;
   }

   auto Val = Builder.GetConstantEnum(*case_it, CaseVals);
   cacheValue(Ty, Val);

   return Val;
}

il::ConstantClass* ReflectionBuilder::BuildNamedDeclBase(NamedDecl *ND)
{
   auto NDDecl = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_NamedDecl]]);

   if (!NDDecl) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "NamedDecl");

      return nullptr;
   }

   auto AS = BuildAccessSpec(ND->getAccess());
   auto Name = BuildStringView(ND->getDeclName());

   if (!AS || !Name)
      return nullptr;

   return Builder.GetConstantClass(NDDecl, GetTypeInfo(NDDecl), { AS, Name });
}

il::ConstantClass* ReflectionBuilder::BuildRecordBase(RecordDecl *R)
{
   auto RD = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_RecordDecl]]);
   if (!RD) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "Record");

      return nullptr;
   }

   auto MD = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_MethodDecl]]);
   if (!MD) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "Method");

      return nullptr;
   }

   auto NDBase = BuildNamedDeclBase(R);

   llvm::SmallVector<il::Constant*, 4> Methods;
   for (auto M : R->getDecls<MethodDecl>()) {
      auto Val = BuildMethodMirror(M);
      if (!Val)
         continue;

      Methods.push_back(Val);
   }

   auto MethodConst = BuildArrayView(Context.getRecordType(MD), Methods);
   return Builder.GetConstantClass(RD, GetTypeInfo(RD),
                                   { MethodConst }, NDBase);
}

il::ConstantClass* ReflectionBuilder::BuildFunctionBase(CallableDecl *F,
                                                        bool cache) {
   if (cache) {
      if (auto Val = getCachedValue(F))
         return cast<il::ConstantClass>(Val);
   }

   auto FD = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_FunctionDecl]]);
   if (!FD) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "Function");

      return nullptr;
   }

   auto Ty = BuildType(F->getFunctionType());
   if (!Ty)
      return nullptr;

   il::Constant *Fn = SP.getILGen().DeclareFunction(F);
   if (!Fn)
      return nullptr;

   Fn = il::ConstantExpr::getBitCast(Fn, Context.getInt8PtrTy());

   auto NDBase = BuildNamedDeclBase(F);
   if (!NDBase)
      return nullptr;

   auto Val = Builder.GetConstantClass(FD, GetTypeInfo(FD), { Ty, Fn }, NDBase);
   if (cache) {
      cacheValue(F, Val);
   }

   return Val;
}

il::ConstantClass* ReflectionBuilder::BuildMethodMirror(MethodDecl *M)
{
   if (M->isTemplate())
      return nullptr;

   if (auto Val = getCachedValue(M))
      return cast<il::ConstantClass>(Val);

   auto MD = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_MethodDecl]]);
   if (!MD) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "Method");

      return nullptr;
   }

   auto FnBase = BuildFunctionBase(M);
   if (!FnBase)
      return nullptr;

   auto Val = Builder.GetConstantClass(MD, GetTypeInfo(MD), {}, FnBase);
   cacheValue(M, Val);

   SP.maybeInstantiateMemberFunction(M, Expr);

   return Val;
}

il::ConstantClass* ReflectionBuilder::BuildVarDeclBase(VarDecl *Var)
{
   auto VD = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_VarDecl]]);
   if (!VD) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "VarDecl");

      return nullptr;
   }

   auto Ty = BuildType(Var->getType());
   if (!Ty)
      return nullptr;

   auto IsLet = Var->isConst() ? Builder.GetTrue() : Builder.GetFalse();

   auto NDBase = BuildNamedDeclBase(Var);
   if (!NDBase)
      return nullptr;

   return Builder.GetConstantClass(VD, GetTypeInfo(VD), { Ty, IsLet }, NDBase);
}

il::ConstantClass* ReflectionBuilder::BuildFieldMirror(FieldDecl *F)
{
   if (auto Val = getCachedValue(F))
      return cast<il::ConstantClass>(Val);

   auto FD = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_FieldDecl]]);
   if (!FD) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "Field");

      return nullptr;
   }

   auto VDBase = BuildVarDeclBase(F);
   if (!VDBase)
      return nullptr;

   auto Val = Builder.GetConstantClass(FD, GetTypeInfo(FD), {}, VDBase);
   cacheValue(F, Val);

   return Val;
}

il::ConstantClass* ReflectionBuilder::BuildArgumentMirror(FuncArgDecl *Arg)
{
   if (auto Val = getCachedValue(Arg))
      return cast<il::ConstantClass>(Val);

   auto ArgDecl = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_FuncArgDecl]]);
   if (!ArgDecl) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "Argument");

      return nullptr;
   }

   auto VDBase = BuildVarDeclBase(Arg);
   if (!VDBase)
      return nullptr;

   auto Val = Builder.GetConstantClass(ArgDecl, GetTypeInfo(ArgDecl), {},
                                       VDBase);

   cacheValue(Arg, Val);
   return Val;
}

il::ConstantClass *ReflectionBuilder::BuildEnumCaseMirror(EnumCaseDecl *Case)
{
   if (auto Val = getCachedValue(Case))
      return cast<il::ConstantClass>(Val);

   auto CaseDecl = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_EnumCaseDecl]]);

   if (!CaseDecl) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "EnumCase");

      return nullptr;
   }

   auto ArgDecl = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_FuncArgDecl]]);

   if (!ArgDecl) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "Argument");

      return nullptr;
   }

   QualType ElementTy = Context.getRecordType(ArgDecl);
   llvm::SmallVector<il::Constant*, 4> CaseVals;
   for (auto &Val : Case->getArgs()) {
      CaseVals.push_back(BuildArgumentMirror(Val));
   }

   auto *CaseValPtr = BuildArrayView(ElementTy, {});
   auto NDBase = BuildNamedDeclBase(Case);

   if (!CaseValPtr || !NDBase)
      return nullptr;

   return Builder.GetConstantClass(CaseDecl, GetTypeInfo(CaseDecl),
                                   { CaseValPtr }, NDBase);
}

il::ConstantClass* ReflectionBuilder::BuildStructMirror(StructDecl *S,
                                                        bool cache) {
   if (cache) {
      if (auto Val = getCachedValue(S))
         return cast<il::ConstantClass>(Val);
   }

   auto SD = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_StructDecl]]);

   if (!SD) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "Struct");

      return nullptr;
   }

   auto FwdDecl = Builder.ForwardDeclareConstantClass(SD, GetTypeInfo(SD));
   cacheValue(S, FwdDecl);

   llvm::SmallVector<il::Constant*, 4> Fields;
   for (auto C : S->getFields()) {
      auto Mirror = BuildFieldMirror(C);
      if (!Mirror)
         return nullptr;

      Fields.push_back(Mirror);
   }

   auto FieldDecl = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_FieldDecl]]);
   if (!FieldDecl)
      return nullptr;

   auto FieldVal = BuildArrayView(Context.getRecordType(FieldDecl), Fields);

   auto RDBase = BuildRecordBase(S);
   if (!RDBase)
      return nullptr;

   auto RealVal = Builder.ReplaceForwardDecl(FwdDecl, { FieldVal }, RDBase);
   cacheValue(S, RealVal);

   return RealVal;
}

il::ConstantClass* ReflectionBuilder::BuildClassMirror(ClassDecl *C)
{
   if (auto Val = getCachedValue(C))
      return cast<il::ConstantClass>(Val);

   auto CD = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_ClassDecl]]);
   if (!CD) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "Class");

      return nullptr;
   }

   auto FwdDecl = Builder.ForwardDeclareConstantClass(CD, GetTypeInfo(CD));
   cacheValue(C, FwdDecl);

   auto ClassTy = Context.getRecordType(CD);

   il::Constant *BaseVal;
   if (auto Base = C->getParentClass()) {
      BaseVal = BuildOption(ClassTy, BuildClassMirror(Base));
   }
   else {
      BaseVal = BuildOption(ClassTy);
   }

   auto SDBase = BuildStructMirror(C, false);
   if (!SDBase)
      return nullptr;

   auto RealVal = Builder.ReplaceForwardDecl(FwdDecl, { BaseVal }, SDBase);
   cacheValue(C, RealVal);

   return RealVal;
}

il::ConstantClass* ReflectionBuilder::BuildUnionMirror(UnionDecl *U)
{
   if (auto Val = getCachedValue(U))
      return cast<il::ConstantClass>(Val);

   auto UD = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_UnionDecl]]);
   if (!UD) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "Union");

      return nullptr;
   }

   auto FwdDecl = Builder.ForwardDeclareConstantClass(UD, GetTypeInfo(UD));
   cacheValue(U, FwdDecl);

   auto SDBase = BuildStructMirror(U, false);
   if (!SDBase)
      return nullptr;

   auto RealVal = Builder.ReplaceForwardDecl(FwdDecl, {}, SDBase);
   cacheValue(U, RealVal);

   return RealVal;
}

il::ConstantClass* ReflectionBuilder::BuildEnumMirror(EnumDecl *E)
{
   if (auto Val = getCachedValue(E))
      return cast<il::ConstantClass>(Val);

   auto ED = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_EnumDecl]]);

   if (!ED) {
      SP.diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "Enum");

      return nullptr;
   }

   auto FwdDecl = Builder.ForwardDeclareConstantClass(ED, GetTypeInfo(ED));
   cacheValue(E, FwdDecl);

   auto RawTy = BuildType(E->getRawType());
   if (!RawTy)
      return nullptr;

   llvm::SmallVector<il::Constant*, 4> Cases;
   for (auto C : E->getCases()) {
      auto Mirror = BuildEnumCaseMirror(C);
      if (!Mirror)
         return nullptr;

      Cases.push_back(Mirror);
   }

   auto CaseDecl = dyn_cast_or_null<ClassDecl>(
      SP.BuiltinDecls[SP.ReflectionIdents[RI_EnumCaseDecl]]);
   if (!CaseDecl)
      return nullptr;

   auto CaseVals = BuildArrayView(Context.getRecordType(CaseDecl), Cases);
   if (!CaseVals)
      return nullptr;

   auto RDBase = BuildRecordBase(E);
   if (!RDBase)
      return nullptr;

   auto RealVal = Builder.ReplaceForwardDecl(FwdDecl, { RawTy, CaseVals },
      RDBase);

   cacheValue(E, RealVal);
   return RealVal;
}

ExprResult SemaPass::HandleReflectionAlias(AliasDecl *Alias, Expression *Expr)
{
   if (auto AE = Alias->getAliasExpr()) {
      Expr->setExprType(AE->getExprType());
      return Expr;
   }

   auto *ReflectMod = getReflectModule();
   if (!ReflectMod) {
      diagnose(Expr, err_compiler_ns_unknown_entity, Alias->getDeclName(),
               Alias->getSourceRange());

      return ExprError();
   }

   ensureDeclared(ReflectMod);

   auto It = ReflectionValues.find(Alias);
   if (It != ReflectionValues.end())
      return It->getSecond();

   auto Name = Alias->getDeclName().getManglingName();
   if (!Name.isSimpleIdentifier()) {
      diagnose(Expr, err_compiler_ns_unknown_entity, Alias->getDeclName(),
               Alias->getSourceRange());

      return ExprError();
   }

   if (!ReflectionIdentsInitialized)
      initReflectionIdents();

   if (!ReflBuilder)
      ReflBuilder = new(Context) ReflectionBuilder(*this, Expr);
   else
      ReflBuilder->setExpr(Expr);

   bool DoCache = false;
   Expression *ResultExpr = nullptr;

   auto getReflectionDecl = [&](ReflectionIdent Ident) {
      auto *II = ReflectionIdents[Ident];
      (void)this->Lookup(*ReflectMod, II);

      return BuiltinDecls[ReflectionIdents[Ident]];
   };

   auto *II = Name.getIdentifierInfo();

   // look the declaration up to make sure it's deserialized.
   (void) Lookup(*ReflectMod, II);

   if (II == ReflectionIdents[sizeOf]) {
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

      if (Ty->isRecordType()) {
         if (!ensureSizeKnown(Ty->getRecord(), Expr->getSourceLoc()))
            return ExprError();
      }

      auto &TI = Context.getTargetInfo();
      auto Size = TI.getSizeOfType(Ty);

      ResultExpr = IntegerLiteral::Create(Context, Expr->getSourceRange(),
                                          Context.getUIntTy(),
                                          llvm::APSInt(llvm::APInt(64, Size)));
   }
   else if (II == ReflectionIdents[alignOf]) {
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

      if (Ty->isRecordType()) {
         if (!ensureSizeKnown(Ty->getRecord(), Expr->getSourceLoc()))
            return ExprError();
      }

      auto &TI = Context.getTargetInfo();
      auto Size = TI.getAlignOfType(Ty);

      ResultExpr = IntegerLiteral::Create(Context, Expr->getSourceRange(),
                                          Context.getUIntTy(),
                                          llvm::APSInt(llvm::APInt(64, Size)));
   }
   else if (II == ReflectionIdents[line]) {
      if (Bits.InDefaultArgumentValue) {
         Expr->setIsMagicArgumentValue(true);
      }

      auto LineAndCol = Diags.getFileMgr()->getLineAndCol(Expr->getSourceLoc());
      ResultExpr = IntegerLiteral::Create(Context, Expr->getSourceRange(),
                                          Context.getUIntTy(),
                                          llvm::APSInt(llvm::APInt(
                                             64, LineAndCol.line)));
   }
   else if (II == ReflectionIdents[column]) {
      if (Bits.InDefaultArgumentValue) {
         Expr->setIsMagicArgumentValue(true);
      }

      auto LineAndCol = Diags.getFileMgr()->getLineAndCol(Expr->getSourceLoc());
      ResultExpr = IntegerLiteral::Create(Context, Expr->getSourceRange(),
                                          Context.getUIntTy(),
                                          llvm::APSInt(llvm::APInt(
                                             64, LineAndCol.col)));
   }
   else if (II == ReflectionIdents[fileName]) {
      if (Bits.InDefaultArgumentValue) {
         Expr->setIsMagicArgumentValue(true);
      }

      auto SV = requireStringViewDecl(*this, Expr);
      if (!SV) {
         return ExprError();
      }

      auto FileName = Diags.getFileMgr()->getFileName(Expr->getSourceLoc());
      il::Constant *CS = ILGen->MakeStringView(FileName);

      ResultExpr = StaticExpr::Create(Context, Context.getRecordType(SV),
                                      Expr->getSourceRange(), CS);
   }
   else if (II == ReflectionIdents[sourceLocation]) {
      if (Bits.InDefaultArgumentValue) {
         Expr->setIsMagicArgumentValue(true);
      }

      auto LineAndCol = Diags.getFileMgr()->getLineAndCol(Expr->getSourceLoc());
      auto FileName = Diags.getFileMgr()->getFileName(Expr->getSourceLoc());

      auto SLDecl = cast_or_null<StructDecl>(getReflectionDecl(SourceLocation));
      if (!SLDecl) {
         diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "SourceLocation");
         return ExprError();
      }

      auto SV = requireStringViewDecl(*this, Expr);
      if (!SV) {
         return ExprError();
      }

      auto &Builder = ILGen->Builder;

      il::Constant *ConstFileName = ILGen->MakeStringView(FileName);
      il::Constant *ConstLoc = Builder.GetConstantStruct(SLDecl, {
         Builder.GetConstantInt(Context.getUIntTy(), LineAndCol.line),
         Builder.GetConstantInt(Context.getUIntTy(), LineAndCol.col),
         ConstFileName
      });

      ResultExpr = StaticExpr::Create(Context, Context.getRecordType(SLDecl),
                                      Expr->getSourceRange(), ConstLoc);
   }
   else if (II == ReflectionIdents[function]) {
      if (Bits.InDefaultArgumentValue) {
         Expr->setIsMagicArgumentValue(true);
      }

      auto SV = requireStringViewDecl(*this, Expr);
      if (!SV) {
         return ExprError();
      }

      auto Fn = getCurrentFun();
      if (!Fn) {
         diagnose(Expr, err_reflect_outside_func, 0, Expr->getSourceRange());
         return ExprError();
      }

      auto FuncName = Fn->getFullName();
      il::Constant *CS = ILGen->MakeStringView(FuncName);

      ResultExpr = StaticExpr::Create(Context, Context.getRecordType(SV),
                                      Expr->getSourceRange(), CS);
   }
   else if (II == ReflectionIdents[mangledFunction]) {
      if (Bits.InDefaultArgumentValue) {
         Expr->setIsMagicArgumentValue(true);
      }

      auto SV = requireStringViewDecl(*this, Expr);
      if (!SV) {
         return ExprError();
      }

      auto Fn = getCurrentFun();
      if (!Fn) {
         diagnose(Expr, err_reflect_outside_func, 1, Expr->getSourceRange());
         return ExprError();
      }

      std::string MangledName;
      llvm::raw_string_ostream OS(MangledName);

      mangle.mangle(Fn, OS);
      OS.flush();

      il::Constant *CS = ILGen->MakeStringView(MangledName);
      ResultExpr = StaticExpr::Create(Context, Context.getRecordType(SV),
                                      Expr->getSourceRange(), CS);
   }
   else if (II == ReflectionIdents[targetInfo]) {
      auto TIDecl = cast_or_null<StructDecl>(getReflectionDecl(TargetInfo));
      if (!TIDecl) {
         diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "TargetInfo");
         return ExprError();
      }

      // 'Endianness' enum
      auto EndiannessDecl = dyn_cast_or_null<EnumDecl>(
         BuiltinDecls[ReflectionIdents[Endianness]]);

      if (!EndiannessDecl) {
         diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "Endianness");
         return ExprError();
      }

      // 'OperatingSystem' enum
      auto OSDecl = cast_or_null<EnumDecl>(getReflectionDecl(OperatingSystem));
      if (!OSDecl) {
         diagnose(Expr, err_reflection_decl_not_found, Expr->getSourceRange(),
                  "OperatingSystem");
         return ExprError();
      }

      const cdot::TargetInfo &TI = ILGen->getTargetInfo();
      auto &Builder = ILGen->Builder;
      auto PointerSize = Builder.GetConstantInt(Context.getUIntTy(),
                                                TI.getPointerSizeInBytes());
      auto PointerAlign = Builder.GetConstantInt(Context.getUIntTy(),
                                                TI.getPointerAlignInBytes());

      il::Constant *Endianness;
      if (TI.getTriple().isLittleEndian()) {
         Endianness = Builder.GetConstantEnum(
            EndiannessDecl->hasCase(ReflectionIdents[little]), {});
      }
      else {
         Endianness = Builder.GetConstantEnum(
            EndiannessDecl->hasCase(ReflectionIdents[big]), {});
      }

      llvm::StringRef OSIdent;
      switch (TI.getTriple().getOS()) {
      case llvm::Triple::OSType::Darwin: OSIdent = "darwin"; break;
      case llvm::Triple::OSType::DragonFly: OSIdent = "dragonFly"; break;
      case llvm::Triple::OSType::FreeBSD: OSIdent = "freeBSD"; break;
      case llvm::Triple::OSType::Fuchsia: OSIdent = "fuchsia"; break;
      case llvm::Triple::OSType::IOS: OSIdent = "iOS"; break;
      case llvm::Triple::OSType::KFreeBSD: OSIdent = "kFreeBSD"; break;
      case llvm::Triple::OSType::Linux: OSIdent = "linux"; break;
      case llvm::Triple::OSType::Lv2: OSIdent = "ps3"; break;
      case llvm::Triple::OSType::MacOSX: OSIdent = "macOS"; break;
      case llvm::Triple::OSType::NetBSD: OSIdent = "netBSD"; break;
      case llvm::Triple::OSType::OpenBSD: OSIdent = "openBSD"; break;
      case llvm::Triple::OSType::Solaris: OSIdent = "solaris"; break;
      case llvm::Triple::OSType::Win32: OSIdent = "windows"; break;
      case llvm::Triple::OSType::PS4: OSIdent = "PS4"; break;
      case llvm::Triple::OSType::TvOS: OSIdent = "tvOS"; break;
      case llvm::Triple::OSType::WatchOS: OSIdent = "watchOS"; break;
      default: OSIdent = "unknownOS"; break;
      }

      il::Constant *OS = Builder.GetConstantEnum(
         OSDecl->hasCase(Context.getIdentifiers().get(OSIdent)), {}
      );

      il::Constant *InfoConstant = Builder.GetConstantStruct(TIDecl, {
         PointerSize, PointerAlign, Endianness, OS
      });

      DoCache = true;
      ResultExpr = StaticExpr::Create(Context, Context.getRecordType(TIDecl),
                                      Expr->getSourceRange(), InfoConstant);
   }
   else if (II == ReflectionIdents[mirror]) {
      if (!Alias->isInstantiation() || Alias->getTemplateArgs().size() != 1
          || !Alias->getTemplateArgs().front().isType()) {
         diagnose(Expr, err_compiler_ns_bad_def, Alias->getDeclName(),
                  Alias->getSourceRange());

         return ExprError();
      }

      QualType Ty = Alias->getTemplateArgs().front().getType();
      if (Ty->isDependentType()) {
         Expr->setIsTypeDependent(true);
         Expr->setExprType(UnknownAnyTy);

         return Expr;
      }

      auto Result = ReflBuilder->getMirror(Ty);
      if (!Result.first)
         return ExprError();

      ResultExpr = StaticExpr::Create(Context, Result.second,
                                      Expr->getSourceRange(), Result.first);
   }
   else if (II == ReflectionIdents[RI_inCTFE]) {
      DoCache = true;
      ResultExpr = StaticExpr::Create(Context, Context.getBoolTy(),
                                      Expr->getSourceRange(),
                                      ILGen->Builder.GetMagicConstant(
                                         il::MagicConstant::__ctfe));
   }
   else if (II == ReflectionIdents[RI_debug]) {
      DoCache = true;
      ResultExpr = BoolLiteral::Create(Context, Expr->getSourceLoc(),
         Context.getBoolTy(), compilationUnit->getOptions().emitDebugInfo());
   }
   else if (II == ReflectionIdents[underlyingType]) {
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

      if (!Ty->isEnum()) {
         diagnose(Expr, err_generic_error,
                  "underlyingType expects an enum type",
                  Expr->getSourceRange());

         return ExprError();
      }

      QualType UnderlyingTy = Context.getMetaType(
         cast<EnumDecl>(Ty->getRecord())->getRawType());

      ResultExpr = new(Context) IdentifierRefExpr(Expr->getSourceRange(),
                                                  IdentifierKind::MetaType,
                                                  UnderlyingTy);
   }
   else if (II == ReflectionIdents[IsDefaultInitializable]) {
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

      ResultExpr = BoolLiteral::Create(Context, Expr->getSourceLoc(),
                                       Context.getBoolTy(),
                                       hasDefaultValue(Ty));
   }
   else if (II == ReflectionIdents[defaultValue]) {
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

      ResultExpr = StaticExpr::Create(Context, Ty, Expr->getSourceRange(),
                                      ILGen->getDefaultValue(Ty));
   }

   if (!ResultExpr) {
      diagnose(Expr, err_compiler_ns_unknown_entity, Alias->getDeclName(),
               Alias->getSourceRange());

      return ExprError();
   }

   auto Res = visitExpr(ResultExpr);
   if (Expr->isMagicArgumentValue()) {
      Expr->setExprType(Res.getValue()->getExprType());
      return Expr;
   }

   if (DoCache)
      ReflectionValues.try_emplace(Alias, Res.get());

   return Res;
}

} // namespace ast
} // namespace cdot