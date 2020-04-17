#include "cdotc/Query/Query.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/Module/ModuleManager.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;

QueryResult GetBuiltinModuleQuery::run()
{
   auto& ModuleMgr = QC.CI.getModuleMgr();
   auto Loc = QC.CI.getMainFileLoc();

   if (Mod == Builtin) {
      auto* II = QC.Sema->getIdentifier("builtin");
      auto* M = ModuleMgr.LookupModule(Loc, Loc, II);

      return finish(M);
   }

   if (Mod == Policy) {
      auto* II = QC.Sema->getIdentifier("policy");
      auto* M = ModuleMgr.LookupModule(Loc, Loc, II);

      return finish(M);
   }

   if (Mod == Prelude) {
      auto* II = QC.Sema->getIdentifier("core");
      auto* M = ModuleMgr.LookupModule(Loc, Loc, II);

      return finish(M);
   }

   auto* StdII = sema().getIdentifier("core");
   if (QC.CI.getOptions().noPrelude() && !ModuleMgr.IsModuleLoaded(StdII)) {
      return fail();
   }

   if (Mod == Std) {
      auto* M = ModuleMgr.LookupModule(Loc, Loc, StdII);
      return finish(M);
   }

   IdentifierInfo* Idents[]{StdII, nullptr};
   switch (Mod) {
   case Std:
   case Prelude:
   case Builtin:
   case Policy:
      llvm_unreachable("already handled!");
   case Reflect:
      Idents[1] = sema().getIdentifier("reflect");
      break;
   case Sys:
      Idents[1] = sema().getIdentifier("sys");
      break;
   case Runtime:
      Idents[1] = sema().getIdentifier("rt");
      break;
   case Async:
      Idents[1] = sema().getIdentifier("async");
      break;
   case Test:
      Idents[1] = sema().getIdentifier("test");
      break;
   }

   auto* M = QC.CI.getModuleMgr().GetModule(Idents);
   if (!M) {
      return fail();
   }

   return finish(M);
}

QueryResult GetBuiltinFuncQuery::run()
{
   Module* Mod;
   IdentifierInfo* II;

   switch (Fn) {
   case PureVirtual:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      II = sema().getIdentifier("_cdot_PureVirtual");
      break;
   case CopyClass:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Runtime)) {
         return fail();
      }

      II = sema().getIdentifier("_cdot_CopyClass");
      break;
   case AtomicRelease:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Runtime)) {
         return fail();
      }

      II = sema().getIdentifier("_cdot_AtomicRelease");
      break;
   }

   NamedDecl* LookupRes;
   if (QC.LookupSingle(LookupRes, Mod->getDecl(), II, LookupOpts::Restricted)) {
      return fail();
   }

   auto* Res = dyn_cast<CallableDecl>(LookupRes);
   return finish(Res);
}

QueryResult GetBuiltinAliasQuery::run()
{
   DeclContext* DC;
   if (this->DC) {
      DC = this->DC;
   }
   else {
      Module* Mod;
      if (auto Err = QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {
         return Query::finish(Err);
      }
      if (!Mod) {
         return finish(nullptr);
      }

      DC = Mod->getDecl();
   }

   IdentifierInfo* II;
   switch (K) {
   case DefaultSignedIntegerLiteralType:
      II = QC.Sema->getIdentifier("DefaultSignedIntegerLiteralType");
      break;
   case DefaultUnsignedIntegerLiteralType:
      II = QC.Sema->getIdentifier("DefaultUnsignedIntegerLiteralType");
      break;
   case DefaultFloatingPointLiteralType:
      II = QC.Sema->getIdentifier("DefaultFloatingPointLiteralType");
      break;
   case DefaultBooleanLiteralType:
      II = QC.Sema->getIdentifier("DefaultBooleanLiteralType");
      break;
   case DefaultCharacterLiteralType:
      II = QC.Sema->getIdentifier("DefaultCharacterLiteralType");
      break;
   case DefaultStringLiteralType:
      II = QC.Sema->getIdentifier("DefaultStringLiteralType");
      break;
   case DefaultArrayLiteralType:
      II = QC.Sema->getIdentifier("DefaultArrayLiteralType");
      break;
   case DefaultDictionaryLiteralType:
      II = QC.Sema->getIdentifier("DefaultDictionaryLiteralType");
      break;
   }

   NamedDecl* LookupRes;
   if (QC.LookupSingle(LookupRes, DC, II, LookupOpts::Restricted)) {
      return fail();
   }

#ifndef NDEBUG
   if (!LookupRes) {
      if (QC.LookupSingle(LookupRes, QC.CI.getCompilationModule()->getDecl(),
                          II, LookupOpts::Restricted)) {
         return fail();
      }
   }
#endif

   return finish(dyn_cast_or_null<AliasDecl>(LookupRes));
}

#ifndef NDEBUG

static StringRef getBuiltinRecordName(GetBuiltinRecordQuery::RecordKind kind)
{
   switch (kind) {
   case GetBuiltinRecordQuery::Bool:
      return "Bool";
   case GetBuiltinRecordQuery::Character:
      return "Character";
   case GetBuiltinRecordQuery::Int8:
      return "Int8";
   case GetBuiltinRecordQuery::UInt8:
      return "UInt8";
   case GetBuiltinRecordQuery::Int16:
      return "Int16";
   case GetBuiltinRecordQuery::UInt16:
      return "UInt16";
   case GetBuiltinRecordQuery::Int32:
      return "Int32";
   case GetBuiltinRecordQuery::UInt32:
      return "UInt32";
   case GetBuiltinRecordQuery::Int64:
      return "Int64";
   case GetBuiltinRecordQuery::UInt64:
      return "UInt64";
   case GetBuiltinRecordQuery::Int128:
      return "Int128";
   case GetBuiltinRecordQuery::UInt128:
      return "UInt128";
   case GetBuiltinRecordQuery::Float:
      return "Float";
   case GetBuiltinRecordQuery::Double:
      return "Double";
   case GetBuiltinRecordQuery::UnsafePtr:
      return "UnsafePtr";
   case GetBuiltinRecordQuery::UnsafeMutablePtr:
      return "UnsafeMutablePtr";
   case GetBuiltinRecordQuery::UnsafeRawPtr:
      return "UnsafeRawPtr";
   case GetBuiltinRecordQuery::UnsafeMutableRawPtr:
      return "UnsafeMutableRawPtr";
   case GetBuiltinRecordQuery::UnsafeBufferPtr:
      return "UnsafeBufferPtr";
   case GetBuiltinRecordQuery::UnsafeMutableBufferPtr:
      return "UnsafeMutableBufferPtr";
   case GetBuiltinRecordQuery::Array:
      return "Array";
   case GetBuiltinRecordQuery::Dictionary:
      return "Dictionary";
   case GetBuiltinRecordQuery::String:
      return "String";
   case GetBuiltinRecordQuery::Option:
      return "Option";
   default:
      return "";
   }
}

#endif

QueryResult GetBuiltinRecordQuery::run()
{
   // Synthesize some types that the runtime needs even if there's no std
   // module.
   if (QC.CI.getOptions().noPrelude()) {
      auto& Idents = QC.Context.getIdentifiers();
      auto* NS = QC.Sema->getPrivateNamespace();

#ifndef NDEBUG
      auto& name = Idents.get(getBuiltinRecordName(R));
      NamedDecl* LookupRes;
      if (QC.LookupSingle(LookupRes, QC.CI.getCompilationModule()->getDecl(),
                          &name, LookupOpts::Restricted)) {
         return fail();
      }

      if (auto* result = dyn_cast_or_null<RecordDecl>(LookupRes)) {
         return finish(result);
      }
#endif

      switch (R) {
      case TypeInfo: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto* S = StructDecl::Create(QC.Context, AccessSpecifier::Private, Loc,
                                      Idents.get("TypeInfo"), {}, {});

         S->setSynthesized(true);
         finish(S);

         QualType T = QC.Context.getRecordType(S);

         // let baseClass: UnsafePtr<TypeInfo>
         auto* baseClass = FieldDecl::Create(
             QC.Context, AccessSpecifier::Private, Loc, Loc,
             Idents.get("baseClass"), SourceType(QC.Context.getPointerType(T)),
             false, true, nullptr);

         // let vtable: UnsafePtr<UnsafeRawPtr>
         auto* vtable = FieldDecl::Create(
             QC.Context, AccessSpecifier::Private, Loc, Loc,
             Idents.get("vtable"),
             SourceType(QC.Context.getPointerType(
                 QC.Context.getPointerType(QC.Context.getVoidType()))),
             false, true, nullptr);

         // let deinit: @thin (UnsafeMutableRawPtr) -> Void
         auto* DeinitTy = QC.Context.getFunctionType(
             QC.Context.getVoidType(),
             {QC.Context.getMutablePointerType(QC.Context.getVoidType())});

         auto* deinit = FieldDecl::Create(
             QC.Context, AccessSpecifier::Private, Loc, Loc,
             Idents.get("deinit"), SourceType(DeinitTy), false, true, nullptr);

         // let name: UnsafePtr<Byte>
         auto* name = FieldDecl::Create(
             QC.Context, AccessSpecifier::Private, Loc, Loc, Idents.get("name"),
             SourceType(QC.Context.getPointerType(QC.Context.getCharTy())),
             false, true, nullptr);

         // let valueWitnessTable: UnsafePtr<ValueWitnessTable>
         RecordDecl* VWT;
         if (auto Err = QC.GetBuiltinRecord(VWT, ValueWitnessTable)) {
            return Query::finish(Err);
         }

         auto* valueWitnessTable
             = FieldDecl::Create(QC.Context, AccessSpecifier::Private, Loc, Loc,
                                 Idents.get("valueWitnessTable"),
                                 SourceType(QC.Context.getPointerType(
                                     QC.Context.getRecordType(VWT))),
                                 false, true, nullptr);

         // let conformances: UnsafePtr<ProtocolConformance>
         RecordDecl* PC;
         if (auto Err = QC.GetBuiltinRecord(PC, ProtocolConformance)) {
            return Query::finish(Err);
         }

         auto* conformances
             = FieldDecl::Create(QC.Context, AccessSpecifier::Private, Loc, Loc,
                                 Idents.get("conformances"),
                                 SourceType(QC.Context.getPointerType(
                                     QC.Context.getRecordType(PC))),
                                 false, true, nullptr);

         // let size: UInt
         auto* size = FieldDecl::Create(
             QC.Context, AccessSpecifier::Private, Loc, Loc, Idents.get("size"),
             SourceType(QC.Context.getUIntTy()), false, true, nullptr);

         // let alignment: UInt
         auto* alignment = FieldDecl::Create(
             QC.Context, AccessSpecifier::Private, Loc, Loc,
             Idents.get("alignment"), SourceType(QC.Context.getUIntTy()), false,
             true, nullptr);

         // let stride: UInt
         auto* stride = FieldDecl::Create(QC.Context, AccessSpecifier::Private,
                                          Loc, Loc, Idents.get("stride"),
                                          SourceType(QC.Context.getUIntTy()),
                                          false, true, nullptr);

         QC.Sema->ActOnDecl(S, baseClass);
         QC.Sema->ActOnDecl(S, vtable);
         QC.Sema->ActOnDecl(S, deinit);
         QC.Sema->ActOnDecl(S, name);
         QC.Sema->ActOnDecl(S, valueWitnessTable);
         QC.Sema->ActOnDecl(S, conformances);
         QC.Sema->ActOnDecl(S, size);
         QC.Sema->ActOnDecl(S, alignment);
         QC.Sema->ActOnDecl(S, stride);

         QC.Sema->ActOnDecl(NS, S);
         S->addAttribute(new (QC.Context) NoDeriveAttr());

         return finish(S);
      }
      case ValueWitnessTable: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto* S = StructDecl::Create(QC.Context, AccessSpecifier::Private, Loc,
                                      Idents.get("ValueWitnessTable"), {}, {});

         S->setSynthesized(true);
         finish(S);

         // let copyFn: @thin (UnsafeMutableRawPtr, UnsafeRawPtr) -> Void
         auto* CopyTy = QC.Context.getFunctionType(
             QC.Context.getVoidType(),
             {QC.Context.getMutablePointerType(QC.Context.getVoidType()),
              QC.Context.getPointerType(QC.Context.getVoidType())});

         auto* copy = FieldDecl::Create(
             QC.Context, AccessSpecifier::Private, Loc, Loc,
             Idents.get("copyFn"), SourceType(CopyTy), false, true, nullptr);

         // let deinit: @thin (UnsafeMutableRawPtr) -> Void
         auto* DeinitTy = QC.Context.getFunctionType(
             QC.Context.getVoidType(),
             {QC.Context.getMutablePointerType(QC.Context.getVoidType())});

         auto* deinit = FieldDecl::Create(
             QC.Context, AccessSpecifier::Private, Loc, Loc,
             Idents.get("deinit"), SourceType(DeinitTy), false, true, nullptr);

         QC.Sema->ActOnDecl(S, copy);
         QC.Sema->ActOnDecl(S, deinit);

         QC.Sema->ActOnDecl(NS, S);
         S->addAttribute(new (QC.Context) NoDeriveAttr());

         return finish(S);
      }
      case ProtocolConformance: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto* S
             = StructDecl::Create(QC.Context, AccessSpecifier::Private, Loc,
                                  Idents.get("ProtocolConformance"), {}, {});

         S->setSynthesized(true);
         finish(S);

         // var typeInfo: UnsafePtr<TypeInfo>
         RecordDecl* TI;
         if (auto Err = QC.GetBuiltinRecord(TI, TypeInfo)) {
            return Query::finish(Err);
         }

         auto* typeInfo
             = FieldDecl::Create(QC.Context, AccessSpecifier::Private, Loc, Loc,
                                 Idents.get("typeInfo"),
                                 SourceType(QC.Context.getPointerType(
                                     QC.Context.getRecordType(TI))),
                                 false, true, nullptr);

         // var vtable: UnsafePtr<UnsafeRawPtr>
         auto* vtable = FieldDecl::Create(
             QC.Context, AccessSpecifier::Private, Loc, Loc,
             Idents.get("vtable"),
             SourceType(QC.Context.getPointerType(
                 QC.Context.getPointerType(QC.Context.getVoidType()))),
             false, true, nullptr);

         QC.Sema->ActOnDecl(S, typeInfo);
         QC.Sema->ActOnDecl(S, vtable);

         QC.Sema->ActOnDecl(NS, S);
         S->addAttribute(new (QC.Context) NoDeriveAttr());

         return finish(S);
      }
      case ExistentialContainer: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto* S
             = StructDecl::Create(QC.Context, AccessSpecifier::Private, Loc,
                                  Idents.get("ExistentialContainer"), {}, {});

         S->setSynthesized(true);
         finish(S);

         // var value: UnsafeMutableRawPtr
         auto* value
             = FieldDecl::Create(QC.Context, AccessSpecifier::Private, Loc, Loc,
                                 Idents.get("value"),
                                 SourceType(QC.Context.getMutablePointerType(
                                     QC.Context.getVoidType())),
                                 false, true, nullptr);

         // var typeInfo: UnsafePtr<TypeInfo>
         RecordDecl* TI;
         if (auto Err = QC.GetBuiltinRecord(TI, TypeInfo)) {
            return Query::finish(Err);
         }

         auto* typeInfo
             = FieldDecl::Create(QC.Context, AccessSpecifier::Private, Loc, Loc,
                                 Idents.get("typeInfo"),
                                 SourceType(QC.Context.getPointerType(
                                     QC.Context.getRecordType(TI))),
                                 false, true, nullptr);

         // var conformance: UnsafePtr<ProtocolConformance>
         RecordDecl* PC;
         if (auto Err = QC.GetBuiltinRecord(PC, ProtocolConformance)) {
            return Query::finish(Err);
         }

         auto* conformance
             = FieldDecl::Create(QC.Context, AccessSpecifier::Private, Loc, Loc,
                                 Idents.get("conformance"),
                                 SourceType(QC.Context.getPointerType(
                                     QC.Context.getRecordType(PC))),
                                 false, true, nullptr);

         QC.Sema->ActOnDecl(S, value);
         QC.Sema->ActOnDecl(S, typeInfo);
         QC.Sema->ActOnDecl(S, conformance);

         QC.Sema->ActOnDecl(NS, S);
         S->addAttribute(new (QC.Context) NoDeriveAttr());

         return finish(S);
      }
      case Bool: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto* Name = QC.Sema->getIdentifier("Bool");
         auto* S = StructDecl::Create(QC.Context, AccessSpecifier::Public, Loc,
                                      Name, {}, {});

         QualType BoolTy = QC.Context.getBoolTy();

         S->setSynthesized(true);
         auto* ValName = QC.Sema->getIdentifier("value");
         auto* F = FieldDecl::Create(QC.Context, AccessSpecifier::Private, Loc,
                                     Loc, ValName, SourceType(BoolTy), false,
                                     false, nullptr);

         QC.Sema->ActOnDecl(S, F);
         QC.Sema->ActOnDecl(NS, S);
         QC.DeclareImplicitInitializers(S);

         // def prefix !() -> Bool
         DeclarationName DN
             = QC.Context.getDeclNameTable().getPrefixOperatorName(
                 *QC.Sema->getIdentifier("!"));

         auto* negate = MethodDecl::CreateOperator(
             QC.Context, AccessSpecifier::Public, Loc, DN,
             SourceType(QC.Context.getRecordType(S)),
             {QC.Sema->MakeSelfArg(Loc)}, {}, nullptr, false);

         // return Self(not_i1(self.value))
         auto* Self = SelfExpr::Create(QC.Context, Loc, false);
         auto* ValueRef = MemberRefExpr::Create(QC.Context, Self, F, Loc);

         auto* FnTy = QC.Context.getFunctionType(BoolTy, BoolTy);
         auto* Not = UnaryOperator::Create(QC.Context, Loc, op::UnaryLNot, FnTy,
                                           ValueRef, true);

         auto* Ref = MemberRefExpr::Create(QC.Context, Self,
                                           S->getMemberwiseInitializer(), Loc);

         auto* Call
             = AnonymousCallExpr::Create(QC.Context, Loc, Ref, Not, ValName);

         auto* Ret = ReturnStmt::Create(QC.Context, Loc, Call);
         auto* CS = CompoundStmt::Create(QC.Context, Ret, false, Loc, Loc);

         negate->setBody(CS);
         QC.Sema->ActOnDecl(S, negate);

         return finish(S);
      }

#define SYNTHESIZE(NAME)                                                       \
   case NAME: {                                                                \
      SourceLocation Loc = QC.CI.getMainFileLoc();                             \
      auto* Name = QC.Sema->getIdentifier(#NAME);                              \
      auto* S = StructDecl::Create(QC.Context, AccessSpecifier::Public, Loc,   \
                                   Name, {}, {});                              \
      S->setSynthesized(true);                                                 \
      auto* ValName = QC.Sema->getIdentifier("value");                         \
      auto* F = FieldDecl::Create(                                             \
          QC.Context, AccessSpecifier::Private, Loc, Loc, ValName,             \
          SourceType(QC.Context.get##NAME##Ty()), false, false, nullptr);      \
      QC.Sema->ActOnDecl(S, F);                                                \
      QC.Sema->ActOnDecl(NS, S);                                               \
      return finish(S);                                                        \
   }

         SYNTHESIZE(Int8)
         SYNTHESIZE(UInt8)
         SYNTHESIZE(Int16)
         SYNTHESIZE(UInt16)
         SYNTHESIZE(Int32)
         SYNTHESIZE(UInt32)
         SYNTHESIZE(Int64)
         SYNTHESIZE(UInt64)
         SYNTHESIZE(Int128)
         SYNTHESIZE(UInt128)
         SYNTHESIZE(Float)
         SYNTHESIZE(Double)

#undef SYNTHESIZE

      default:
         return finish(nullptr);
      }
   }

   Module* Mod;
   IdentifierInfo* II;

   switch (R) {
#define PRELUDE_RECORD(NAME)                                                   \
   case NAME:                                                                  \
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {          \
         return fail();                                                        \
      }                                                                        \
                                                                               \
      II = sema().getIdentifier(#NAME);                                        \
      break;

      PRELUDE_RECORD(Bool)
      PRELUDE_RECORD(Character)

      PRELUDE_RECORD(Int8)
      PRELUDE_RECORD(UInt8)
      PRELUDE_RECORD(Int16)
      PRELUDE_RECORD(UInt16)
      PRELUDE_RECORD(Int32)
      PRELUDE_RECORD(UInt32)
      PRELUDE_RECORD(Int64)
      PRELUDE_RECORD(UInt64)
      PRELUDE_RECORD(Int128)
      PRELUDE_RECORD(UInt128)

      PRELUDE_RECORD(Float)
      PRELUDE_RECORD(Double)

      PRELUDE_RECORD(UnsafePtr)
      PRELUDE_RECORD(UnsafeMutablePtr)
      PRELUDE_RECORD(UnsafeRawPtr)
      PRELUDE_RECORD(UnsafeMutableRawPtr)
      PRELUDE_RECORD(UnsafeBufferPtr)
      PRELUDE_RECORD(UnsafeMutableBufferPtr)

      PRELUDE_RECORD(Array)
      PRELUDE_RECORD(Dictionary)
      PRELUDE_RECORD(String)
      PRELUDE_RECORD(Option)

#undef PRELUDE_RECORD

   case Box:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Runtime)) {
         return fail();
      }

      II = sema().getIdentifier("Box");
      break;
   case Promise:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Async)) {
         return fail();
      }

      II = sema().getIdentifier("Promise");
      break;
   case Future:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Async)) {
         return fail();
      }

      II = sema().getIdentifier("Future");
      break;
   case CoroHandle:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Async)) {
         return fail();
      }

      II = sema().getIdentifier("CoroHandle");
      break;
   case TypeInfo:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Runtime)) {
         return fail();
      }

      II = sema().getIdentifier("TypeInfo");
      break;
   case ValueWitnessTable:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Runtime)) {
         return fail();
      }

      II = sema().getIdentifier("ValueWitnessTable");
      break;
   case ProtocolConformance:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Runtime)) {
         return fail();
      }

      II = sema().getIdentifier("ProtocolConformance");
      break;
   case ExistentialContainer:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Runtime)) {
         return fail();
      }

      II = sema().getIdentifier("ExistentialContainer");
      break;
   }

   NamedDecl* LookupRes;
   if (QC.LookupSingle(LookupRes, Mod->getDecl(), II, LookupOpts::Restricted)) {
      return fail();
   }

   return finish(dyn_cast_or_null<RecordDecl>(LookupRes));
}

#ifndef NDEBUG

static StringRef
getBuiltinProtocolName(GetBuiltinProtocolQuery::ProtocolKind kind)
{
   switch (kind) {
   case GetBuiltinProtocolQuery::Any:
      return "Any";
   case GetBuiltinProtocolQuery::Equatable:
      return "Equatable";
   case GetBuiltinProtocolQuery::Hashable:
      return "Hashable";
   case GetBuiltinProtocolQuery::Copyable:
      return "Copyable";
   case GetBuiltinProtocolQuery::MoveOnly:
      return "MoveOnly";
   case GetBuiltinProtocolQuery::ImplicitlyCopyable:
      return "ImplicitlyCopyable";
   case GetBuiltinProtocolQuery::StringRepresentable:
      return "StringRepresentable";
   case GetBuiltinProtocolQuery::Persistable:
      return "Persistable";
   case GetBuiltinProtocolQuery::Awaiter:
      return "Awaiter";
   case GetBuiltinProtocolQuery::Awaitable:
      return "Awaitable";
   case GetBuiltinProtocolQuery::TruthValue:
      return "TruthValue";
   case GetBuiltinProtocolQuery::RawRepresentable:
      return "RawRepresentable";
   case GetBuiltinProtocolQuery::Dereferenceable:
      return "Dereferenceable";
   case GetBuiltinProtocolQuery::Unwrappable:
      return "Unwrappable";
   default:
      return "";
   }
}

#endif

QueryResult GetBuiltinProtocolQuery::run()
{
   // Synthesize some types that the runtime needs even if there's no std
   // module.
   if (QC.CI.getOptions().noPrelude()) {
      auto& Idents = QC.Context.getIdentifiers();
      auto* NS = QC.Sema->getPrivateNamespace();

#ifndef NDEBUG
      auto& name = Idents.get(getBuiltinProtocolName(P));

      NamedDecl* LookupRes;
      if (QC.LookupSingle(LookupRes, QC.CI.getCompilationModule()->getDecl(),
                          &name, LookupOpts::Restricted)) {
         return fail();
      }

      if (auto* result = dyn_cast_or_null<ProtocolDecl>(LookupRes)) {
         return finish(result);
      }
#endif

      switch (P) {
      case Any: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto* P = ProtocolDecl::Create(QC.Context, AccessSpecifier::Private,
                                        Loc, Idents.get("Any"), {}, {});

         P->setIsAny(true);
         QC.Sema->ActOnDecl(NS, P);

         return finish(P);
      }
      case Copyable: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto* P = ProtocolDecl::Create(QC.Context, AccessSpecifier::Private,
                                        Loc, Idents.get("Copyable"), {}, {});

         // Make sure the 'Self' type exists.
         QC.Sema->ActOnDecl(NS, P);
         QC.DeclareSelfAlias(P);

         // def copy() -> Self
         auto* SelfTy = QC.Context.getAssociatedType(
             P->getAssociatedType(Idents.get("Self")));

         auto* copy = MethodDecl::Create(
             QC.Context, AccessSpecifier::Public, Loc, Idents.get("copy"),
             SourceType(SelfTy), {QC.Sema->MakeSelfArg(Loc)}, {}, nullptr,
             false);

         QC.Sema->ActOnDecl(P, copy);
         return finish(P);
      }
      case ImplicitlyCopyable: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto* P
             = ProtocolDecl::Create(QC.Context, AccessSpecifier::Private, Loc,
                                    Idents.get("ImplicitlyCopyable"), {}, {});

         QC.Sema->ActOnDecl(NS, P);
         return finish(P);
      }
      case MoveOnly: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto* P = ProtocolDecl::Create(QC.Context, AccessSpecifier::Private,
                                        Loc, Idents.get("MoveOnly"), {}, {});

         QC.Sema->ActOnDecl(NS, P);
         return finish(P);
      }
      case TruthValue: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto* P = ProtocolDecl::Create(QC.Context, AccessSpecifier::Private,
                                        Loc, Idents.get("TruthValue"), {}, {});

         RecordDecl* BoolDecl;
         if (auto Err
             = QC.GetBuiltinRecord(BoolDecl, GetBuiltinRecordQuery::Bool)) {
            return Query::finish(Err);
         }

         DeclarationName Name = Idents.get("truthValue");
         DeclarationName PropName
             = QC.Context.getDeclNameTable().getAccessorName(
                 *Name.getIdentifierInfo(), DeclarationName::Getter);

         auto* Getter = MethodDecl::Create(
             QC.Context, AccessSpecifier::Public, Loc, PropName,
             SourceType(QC.Context.getRecordType(BoolDecl)),
             {QC.Sema->MakeSelfArg(Loc)}, {}, nullptr, false);

         auto* Prop = PropDecl::Create(QC.Context, AccessSpecifier::Public, Loc,
                                       Name, Getter->getReturnType(), false,
                                       false, Getter, nullptr);

         QC.Sema->ActOnDecl(P, Prop);
         QC.Sema->ActOnDecl(NS, P);

         return finish(P);
      }
      case Equatable: {
         auto& Tbl = QC.Context.getDeclNameTable();

         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto* P = ProtocolDecl::Create(QC.Context, AccessSpecifier::Private,
                                        Loc, Idents.get("Equatable"), {}, {});

         // Make sure the 'Self' type exists.
         QC.Sema->ActOnDecl(NS, P);
         QC.DeclareSelfAlias(P);

         auto* SelfTy = QC.Context.getAssociatedType(
             P->getAssociatedType(Idents.get("Self")));

         RecordDecl* BoolDecl;
         if (auto Err
             = QC.GetBuiltinRecord(BoolDecl, GetBuiltinRecordQuery::Bool)) {
            llvm_unreachable("no Bool declaration!");
         }

         QualType BoolTy = QC.Context.getRecordType(BoolDecl);

         // def infix == (Self) -> Bool
         auto equalsName = Tbl.getInfixOperatorName(Idents.get("=="));
         auto* RHSArg = FuncArgDecl::Create(
             QC.Context, Loc, Loc, Idents.get("rhs"), nullptr,
             ArgumentConvention::Default, SourceType(SelfTy), nullptr, false);

         auto* equals = MethodDecl::Create(QC.Context, AccessSpecifier::Public,
                                           Loc, equalsName, SourceType(BoolTy),
                                           {QC.Sema->MakeSelfArg(Loc), RHSArg},
                                           {}, nullptr, false);

         // def infix != (Self) -> Bool
         auto nequalsName = Tbl.getInfixOperatorName(Idents.get("!="));
         RHSArg = FuncArgDecl::Create(QC.Context, Loc, Loc, Idents.get("rhs"),
                                      nullptr, ArgumentConvention::Default,
                                      SourceType(SelfTy), nullptr, false);

         auto* nequals = MethodDecl::Create(
             QC.Context, AccessSpecifier::Public, Loc, nequalsName,
             SourceType(BoolTy), {QC.Sema->MakeSelfArg(Loc), RHSArg}, {},
             nullptr, false);

         QC.Sema->ActOnDecl(P, equals);
         QC.Sema->ActOnDecl(P, nequals);

         // Create the extension that provides a default implementation for !=.
         auto* Ext = ExtensionDecl::Create(QC.Context, AccessSpecifier::Public,
                                           Loc, P, {});

         // default def infix != (Self) -> Bool
         RHSArg = FuncArgDecl::Create(QC.Context, Loc, Loc, Idents.get("rhs"),
                                      nullptr, ArgumentConvention::Default,
                                      SourceType(SelfTy), nullptr, false);

         auto* defImpl = MethodDecl::Create(
             QC.Context, AccessSpecifier::Public, Loc, nequalsName,
             SourceType(BoolTy), {QC.Sema->MakeSelfArg(Loc), RHSArg}, {},
             nullptr, false);

         // Syntesize `return !(self == rhs)`
         SequenceElement CmpElements[]
             = {SequenceElement(SelfExpr::Create(QC.Context, Loc, false)),
                SequenceElement(op::CompEQ,
                                SequenceElement::Left | SequenceElement::Right,
                                Loc),
                SequenceElement(new (QC.Context) IdentifierRefExpr(
                    SourceRange(Loc), Idents.get("rhs")))};

         SequenceElement NotElements[]
             = {SequenceElement(op::UnaryLNot, SequenceElement::None, Loc),
                SequenceElement(ExprSequence::Create(QC.Context, CmpElements))};

         auto* RetExpr = ExprSequence::Create(QC.Context, NotElements);
         auto* RetStmt = ReturnStmt::Create(QC.Context, Loc, RetExpr);
         auto* Body
             = CompoundStmt::Create(QC.Context, {RetStmt}, true, Loc, Loc);

         defImpl->setBody(Body);
         defImpl->setDefault(true);

         QC.Sema->ActOnDecl(Ext, defImpl);
         QC.Sema->ActOnDecl(NS, Ext);

         return finish(P);
      }
      case Hashable: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto* P = ProtocolDecl::Create(QC.Context, AccessSpecifier::Private,
                                        Loc, Idents.get("Hashable"), {}, {});

         // def hashValue() -> UInt64
         auto* func = MethodDecl::Create(
             QC.Context, AccessSpecifier::Public, Loc, Idents.get("hashValue"),
             SourceType(QC.Context.getUInt64Ty()), {QC.Sema->MakeSelfArg(Loc)},
             {}, nullptr, false);

         QC.Sema->ActOnDecl(P, func);
         QC.Sema->ActOnDecl(NS, P);

         return finish(P);
      }
      case RawRepresentable: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto& Sema = *QC.Sema;

         auto* P
             = ProtocolDecl::Create(QC.Context, AccessSpecifier::Private, Loc,
                                    Idents.get("RawRepresentable"), {}, {});

         QC.DeclareSelfAlias(P);

         // associatedType RawType
         auto* RawType = AssociatedTypeDecl::Create(
             QC.Context, Loc, Sema.getIdentifier("RawType"), SourceType(),
             SourceType());

         Sema.ActOnDecl(P, RawType);
         Sema.ActOnDecl(NS, P);

         QC.PrepareDeclInterface(RawType);

         SourceType T(QC.Context.getAssociatedType(RawType));

         // init? (rawValue: RawType) (can't use a fallible init)
         auto* ArgName = Sema.getIdentifier("rawValue");
         auto* Arg = FuncArgDecl::Create(Sema.Context, Loc, Loc, ArgName,
                                         ArgName, ArgumentConvention::Owned, T,
                                         nullptr, false);

         auto* Init = InitDecl::Create(Sema.Context, AccessSpecifier::Public,
                                       Loc, Arg, {}, nullptr, {}, false);

         // prop rawValue: RawType { get }
         DeclarationName PropName
             = Sema.Context.getDeclNameTable().getAccessorName(
                 *ArgName, DeclarationName::Getter);

         auto* Self = Sema.MakeSelfArg(Loc);
         auto* Getter
             = MethodDecl::Create(Sema.Context, AccessSpecifier::Public, Loc,
                                  PropName, T, Self, {}, nullptr, false);

         auto* Prop
             = PropDecl::Create(Sema.Context, AccessSpecifier::Public, Loc,
                                ArgName, T, false, false, Getter, nullptr);

         Sema.ActOnDecl(P, Init);
         Sema.ActOnDecl(P, Prop);

         return finish(P);
      }
      default:
         return finish(nullptr);
      }
   }

   Module* Mod;
   IdentifierInfo* II;

   switch (P) {
#define PRELUDE_RECORD(NAME, MODULE)                                           \
   case NAME:                                                                  \
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::MODULE)) {           \
         return fail();                                                        \
      }                                                                        \
                                                                               \
      II = sema().getIdentifier(#NAME);                                        \
      break;

      PRELUDE_RECORD(Any, Policy)
      PRELUDE_RECORD(Hashable, Prelude)
      PRELUDE_RECORD(Equatable, Prelude)
      PRELUDE_RECORD(Copyable, Policy)
      PRELUDE_RECORD(MoveOnly, Policy)
      PRELUDE_RECORD(ImplicitlyCopyable, Policy)
      PRELUDE_RECORD(StringRepresentable, Prelude)
      PRELUDE_RECORD(TruthValue, Prelude)
      PRELUDE_RECORD(RawRepresentable, Prelude)
      PRELUDE_RECORD(Persistable, Prelude)
      PRELUDE_RECORD(Dereferenceable, Prelude)
      PRELUDE_RECORD(Unwrappable, Prelude)

#undef PRELUDE_RECORD
   case Awaiter:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Async)) {
         return fail();
      }

      II = sema().getIdentifier("Awaiter");
      break;
   case Awaitable:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Async)) {
         return fail();
      }

      II = sema().getIdentifier("Awaitable");
      break;
   }

   NamedDecl* LookupRes;
   if (QC.LookupSingle(LookupRes, Mod->getDecl(), II, LookupOpts::Restricted)) {
      return fail();
   }

   return finish(dyn_cast_or_null<ProtocolDecl>(LookupRes));
}