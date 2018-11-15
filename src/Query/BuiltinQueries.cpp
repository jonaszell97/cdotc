//
// Created by Jonas Zell on 24.08.18.
//

#include "Query.h"

#include "AST/Decl.h"
#include "Module/ModuleManager.h"
#include "Sema/SemaPass.h"
#include "QueryContext.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;

QueryResult GetBuiltinModuleQuery::run()
{
   auto It = Cache.find(Mod);
   if (It != Cache.end())
      return finish(It->getSecond());

   auto *StdII = sema().getIdentifier("std");
   auto &ModuleMgr = QC.CI.getModuleMgr();
   if (QC.CI.getOptions().noPrelude() && !ModuleMgr.IsModuleLoaded(StdII)) {
      return finish(nullptr);
   }

   if (Mod == Std) {
      auto Loc = QC.CI.getMainFileLoc();

      auto *M = ModuleMgr.LookupModule(Loc, Loc, StdII);
      Cache[Mod] = M;

      return finish(M);
   }

   IdentifierInfo *Idents[] { StdII, nullptr };
   switch (Mod) {
   case Std: llvm_unreachable("already handled!");
   case Prelude:
      Idents[1] = sema().getIdentifier("prelude");
      break;
   case Builtin:
      Idents[1] = sema().getIdentifier("builtin");
      break;
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

   auto *M = QC.CI.getModuleMgr().GetModule(Idents);
   Cache[Mod] = M;

   return finish(M);
}

QueryResult GetBuiltinFuncQuery::run()
{
   auto It = Cache.find(Fn);
   if (It != Cache.end())
      return finish(It->getSecond());

   Module *Mod;
   IdentifierInfo *II;

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

   NamedDecl *LookupRes;
   if (QC.LookupSingle(LookupRes, Mod->getDecl(), II)) {
      return fail();
   }

   auto *Res = dyn_cast<CallableDecl>(LookupRes);
   Cache[Fn] = Res;

   return finish(Res);
}

QueryResult GetBuiltinRecordQuery::run()
{
   auto It = Cache.find(R);
   if (It != Cache.end())
      return finish(It->getSecond());

   // Synthesize some types that the runtime needs even if there's no std
   // module.
   if (QC.CI.getOptions().noPrelude()) {
      auto &Idents = QC.Context.getIdentifiers();
      auto *NS = QC.Sema->getPrivateNamespace();

      switch (R) {
      case TypeInfo: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto *S = StructDecl::Create(QC.Context, AccessSpecifier::Private,
                                      Loc, Idents.get("TypeInfo"), {}, {});

         S->setSynthesized(true);
         Cache[R] = S;
         finish(S);

         QualType T = QC.Context.getRecordType(S);

         // let baseClass: UnsafePtr<TypeInfo>
         auto *baseClass = FieldDecl::Create(QC.Context,
                                             AccessSpecifier::Private, Loc,
                                             Loc, Idents.get("baseClass"),
                                             SourceType(
                                                QC.Context.getPointerType(T)),
                                             false, true, nullptr);

         // let vtable: UnsafePtr<UnsafeRawPtr>
         auto *vtable = FieldDecl::Create(QC.Context,
                                          AccessSpecifier::Private, Loc,
                                          Loc, Idents.get("vtable"),
                                          SourceType(QC.Context.getPointerType(
                                             QC.Context.getPointerType(
                                                QC.Context.getVoidType()))),
                                          false, true, nullptr);

         // let deinit: @thin (UnsafeMutableRawPtr) -> Void
         auto *DeinitTy = QC.Context.getFunctionType(
            QC.Context.getVoidType(),
            {QC.Context.getMutablePointerType(QC.Context.getVoidType())});

         auto *deinit = FieldDecl::Create(QC.Context,
                                          AccessSpecifier::Private, Loc,
                                          Loc, Idents.get("deinit"),
                                          SourceType(DeinitTy),
                                          false, true, nullptr);

         // let name: UnsafePtr<Byte>
         auto *name = FieldDecl::Create(QC.Context,
                                        AccessSpecifier::Private, Loc,
                                        Loc, Idents.get("name"),
                                        SourceType(QC.Context.getPointerType(
                                           QC.Context.getCharTy())),
                                        false, true, nullptr);

         // let valueWitnessTable: UnsafePtr<ValueWitnessTable>
         RecordDecl *VWT;
         if (auto Err = QC.GetBuiltinRecord(VWT, ValueWitnessTable)) {
            return Query::finish(Err);
         }

         auto *valueWitnessTable = FieldDecl::Create(
            QC.Context, AccessSpecifier::Private, Loc,
            Loc, Idents.get("valueWitnessTable"),
            SourceType(QC.Context.getPointerType(QC.Context.getRecordType(VWT))),
            false, true, nullptr);

         // let conformances: UnsafePtr<ProtocolConformance>
         RecordDecl *PC;
         if (auto Err = QC.GetBuiltinRecord(PC, ProtocolConformance)) {
            return Query::finish(Err);
         }

         auto *conformances = FieldDecl::Create(
            QC.Context, AccessSpecifier::Private, Loc,
            Loc, Idents.get("conformances"),
            SourceType(QC.Context.getPointerType(QC.Context.getRecordType(PC))),
            false, true, nullptr);

         // let size: UInt
         auto *size = FieldDecl::Create(QC.Context,
                                        AccessSpecifier::Private, Loc,
                                        Loc, Idents.get("size"),
                                        SourceType(QC.Context.getUIntTy()),
                                        false, true, nullptr);

         // let alignment: UInt
         auto *alignment = FieldDecl::Create(QC.Context,
                                        AccessSpecifier::Private, Loc,
                                        Loc, Idents.get("alignment"),
                                        SourceType(QC.Context.getUIntTy()),
                                        false, true, nullptr);

         // let stride: UInt
         auto *stride = FieldDecl::Create(QC.Context,
                                        AccessSpecifier::Private, Loc,
                                        Loc, Idents.get("stride"),
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
         return finish(S);
      }
      case ValueWitnessTable: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto *S = StructDecl::Create(QC.Context, AccessSpecifier::Private,
                                      Loc, Idents.get("ValueWitnessTable"),
                                      {}, {});

         S->setSynthesized(true);
         Cache[R] = S;
         finish(S);

         // let copyFn: @thin (UnsafeMutableRawPtr, UnsafeRawPtr) -> Void
         auto *CopyTy = QC.Context.getFunctionType(
            QC.Context.getVoidType(),
            {QC.Context.getMutablePointerType(QC.Context.getVoidType()),
               QC.Context.getPointerType(QC.Context.getVoidType())});

         auto *copy = FieldDecl::Create(QC.Context,
                                        AccessSpecifier::Private, Loc,
                                        Loc, Idents.get("copyFn"),
                                        SourceType(CopyTy),
                                        false, true, nullptr);

         // let deinit: @thin (UnsafeMutableRawPtr) -> Void
         auto *DeinitTy = QC.Context.getFunctionType(
            QC.Context.getVoidType(),
            {QC.Context.getMutablePointerType(QC.Context.getVoidType())});

         auto *deinit = FieldDecl::Create(QC.Context,
                                          AccessSpecifier::Private, Loc,
                                          Loc, Idents.get("deinit"),
                                          SourceType(DeinitTy),
                                          false, true, nullptr);

         QC.Sema->ActOnDecl(S, copy);
         QC.Sema->ActOnDecl(S, deinit);

         QC.Sema->ActOnDecl(NS, S);
         return finish(S);
      }
      case ProtocolConformance: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto *S = StructDecl::Create(QC.Context, AccessSpecifier::Private,
                                      Loc, Idents.get("ProtocolConformance"),
                                      {}, {});

         S->setSynthesized(true);
         Cache[R] = S;
         finish(S);

         // var typeInfo: UnsafePtr<TypeInfo>
         RecordDecl *TI;
         if (auto Err = QC.GetBuiltinRecord(TI, TypeInfo)) {
            return Query::finish(Err);
         }

         auto *typeInfo = FieldDecl::Create(
            QC.Context, AccessSpecifier::Private, Loc,
            Loc, Idents.get("typeInfo"),
            SourceType(QC.Context.getPointerType(QC.Context.getRecordType(TI))),
            false, true, nullptr);

         // var vtable: UnsafePtr<UnsafeRawPtr>
         auto *vtable = FieldDecl::Create(QC.Context,
                                          AccessSpecifier::Private, Loc,
                                          Loc, Idents.get("vtable"),
                                          SourceType(QC.Context.getPointerType(
                                             QC.Context.getPointerType(
                                                QC.Context.getVoidType()))),
                                          false, true, nullptr);

         QC.Sema->ActOnDecl(S, typeInfo);
         QC.Sema->ActOnDecl(S, vtable);

         QC.Sema->ActOnDecl(NS, S);
         return finish(S);
      }
      case ExistentialContainer: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto *S = StructDecl::Create(QC.Context, AccessSpecifier::Private,
                                      Loc, Idents.get("ExistentialContainer"),
                                      {}, {});

         S->setSynthesized(true);
         Cache[R] = S;
         finish(S);

         // var value: UnsafeMutableRawPtr
         auto *value = FieldDecl::Create(QC.Context,
                                          AccessSpecifier::Private, Loc,
                                          Loc, Idents.get("value"),
                                          SourceType(
                                             QC.Context.getMutablePointerType(
                                                QC.Context.getVoidType())),
                                          false, true, nullptr);

         // var typeInfo: UnsafePtr<TypeInfo>
         RecordDecl *TI;
         if (auto Err = QC.GetBuiltinRecord(TI, TypeInfo)) {
            return Query::finish(Err);
         }

         auto *typeInfo = FieldDecl::Create(
            QC.Context, AccessSpecifier::Private, Loc,
            Loc, Idents.get("typeInfo"),
            SourceType(QC.Context.getPointerType(QC.Context.getRecordType(TI))),
            false, true, nullptr);

         // var conformance: UnsafePtr<ProtocolConformance>
         RecordDecl *PC;
         if (auto Err = QC.GetBuiltinRecord(PC, ProtocolConformance)) {
            return Query::finish(Err);
         }

         auto *conformance = FieldDecl::Create(
            QC.Context, AccessSpecifier::Private, Loc,
            Loc, Idents.get("conformance"),
            SourceType(QC.Context.getPointerType(QC.Context.getRecordType(PC))),
            false, true, nullptr);

         QC.Sema->ActOnDecl(S, value);
         QC.Sema->ActOnDecl(S, typeInfo);
         QC.Sema->ActOnDecl(S, conformance);

         QC.Sema->ActOnDecl(NS, S);
         return finish(S);
      }
      default:
         return finish(nullptr);
      }
   }

   Module *Mod;
   IdentifierInfo *II;

   switch (R) {
   case Array:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      II = sema().getIdentifier("Array");
      break;
   case Dictionary:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      II = sema().getIdentifier("Dictionary");
      break;
   case String:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      II = sema().getIdentifier("String");
      break;
   case Option:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      II = sema().getIdentifier("Option");
      break;
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

   NamedDecl *LookupRes;
   if (QC.LookupSingle(LookupRes, Mod->getDecl(), II)) {
      return fail();
   }

   auto *Res = dyn_cast<RecordDecl>(LookupRes);
   Cache[R] = Res;

   return finish(Res);
}

QueryResult GetBuiltinProtocolQuery::run()
{
   auto It = Cache.find(P);
   if (It != Cache.end())
      return finish(It->getSecond());

   // Synthesize some types that the runtime needs even if there's no std
   // module.
   if (QC.CI.getOptions().noPrelude()) {
      auto &Idents = QC.Context.getIdentifiers();
      auto *NS = QC.Sema->getPrivateNamespace();

      switch (P) {
      case Any: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto *P = ProtocolDecl::Create(QC.Context, AccessSpecifier::Private,
                                        Loc, Idents.get("Any"), {}, {});

         P->setIsAny(true);
         QC.Sema->ActOnDecl(NS, P);
         Cache[this->P] = P;

         return finish(P);
      }
      case Copyable: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto *P = ProtocolDecl::Create(QC.Context, AccessSpecifier::Private,
                                        Loc, Idents.get("Copyable"),
                                        {}, {});

         // Make sure the 'Self' type exists.
         QC.Sema->ActOnDecl(NS, P);
         QC.DeclareSelfAlias(P);

         // def copy() -> Self
         auto *SelfTy = QC.Context.getAssociatedType(
            P->getAssociatedType(Idents.get("Self")));

         auto *copy = MethodDecl::Create(QC.Context, AccessSpecifier::Public,
                                         Loc, Idents.get("copy"),
                                         SourceType(SelfTy),
                                         { QC.Sema->MakeSelfArg(Loc) },
                                         {}, nullptr, false);

         QC.Sema->ActOnDecl(P, copy);

         Cache[this->P] = P;
         return finish(P);
      }
      case ImplicitlyCopyable: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto *P = ProtocolDecl::Create(QC.Context, AccessSpecifier::Private,
                                        Loc, Idents.get("ImplicitlyCopyable"),
                                        {}, {});

         QC.Sema->ActOnDecl(NS, P);
         Cache[this->P] = P;

         return finish(P);
      }
      case MoveOnly: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto *P = ProtocolDecl::Create(QC.Context, AccessSpecifier::Private,
                                        Loc, Idents.get("MoveOnly"),
                                        {}, {});

         QC.Sema->ActOnDecl(NS, P);
         Cache[this->P] = P;

         return finish(P);
      }
      case Equatable: {
         auto &Tbl = QC.Context.getDeclNameTable();

         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto *P = ProtocolDecl::Create(QC.Context, AccessSpecifier::Private,
                                        Loc, Idents.get("Equatable"),
                                        {}, {});

         // Make sure the 'Self' type exists.
         QC.Sema->ActOnDecl(NS, P);
         QC.DeclareSelfAlias(P);

         auto *SelfTy = QC.Context.getAssociatedType(
            P->getAssociatedType(Idents.get("Self")));

         // def infix == (Self) -> Bool
         auto equalsName = Tbl.getInfixOperatorName(Idents.get("=="));
         auto *RHSArg = FuncArgDecl::Create(QC.Context, Loc, Loc,
                                            Idents.get("rhs"), nullptr,
                                            ArgumentConvention::Default,
                                            SourceType(SelfTy), nullptr, false);

         auto *equals = MethodDecl::Create(QC.Context, AccessSpecifier::Public,
                                           Loc, equalsName,
                                           SourceType(QC.Context.getBoolTy()),
                                           {QC.Sema->MakeSelfArg(Loc), RHSArg},
                                           {}, nullptr, false);

         // def infix != (Self) -> Bool
         auto nequalsName = Tbl.getInfixOperatorName(Idents.get("!="));
         RHSArg = FuncArgDecl::Create(QC.Context, Loc, Loc,
                                      Idents.get("rhs"), nullptr,
                                      ArgumentConvention::Default,
                                      SourceType(SelfTy), nullptr, false);

         auto *nequals = MethodDecl::Create(QC.Context, AccessSpecifier::Public,
                                            Loc, nequalsName,
                                            SourceType(QC.Context.getBoolTy()),
                                            {QC.Sema->MakeSelfArg(Loc), RHSArg},
                                            {}, nullptr, false);

         QC.Sema->ActOnDecl(P, equals);
         QC.Sema->ActOnDecl(P, nequals);
         Cache[this->P] = P;

         // Create the extension that provides a default implementation for !=.
         auto *Ext = ExtensionDecl::Create(QC.Context, AccessSpecifier::Public,
                                           Loc, P, {});

         // default def infix != (Self) -> Bool
         RHSArg = FuncArgDecl::Create(QC.Context, Loc, Loc,
                                      Idents.get("rhs"), nullptr,
                                      ArgumentConvention::Default,
                                      SourceType(SelfTy), nullptr, false);

         auto *defImpl = MethodDecl::Create(QC.Context, AccessSpecifier::Public,
                                            Loc, nequalsName,
                                            SourceType(QC.Context.getBoolTy()),
                                            {QC.Sema->MakeSelfArg(Loc), RHSArg},
                                            {}, nullptr, false);

         // Syntesize `return !(self == rhs)`
         SequenceElement CmpElements[] = {
            SequenceElement(SelfExpr::Create(QC.Context, Loc, false)),
            SequenceElement(op::CompEQ, Loc),
            SequenceElement(new(QC.Context)IdentifierRefExpr(SourceRange(Loc),
                                                             Idents.get("rhs")))
         };

         SequenceElement NotElements[] = {
            SequenceElement(op::UnaryNot, Loc),
            SequenceElement(ExprSequence::Create(QC.Context, CmpElements))
         };

         auto *RetExpr = ExprSequence::Create(QC.Context, NotElements);
         auto *RetStmt = ReturnStmt::Create(QC.Context, Loc, RetExpr);
         auto *Body = CompoundStmt::Create(QC.Context, {RetStmt}, true,
                                           Loc, Loc);

         defImpl->setBody(Body);
         defImpl->setDefault(true);

         QC.Sema->ActOnDecl(Ext, defImpl);
         QC.Sema->ActOnDecl(NS, Ext);

         return finish(P);
      }
      case Hashable: {
         SourceLocation Loc = QC.CI.getMainFileLoc();
         auto *P = ProtocolDecl::Create(QC.Context, AccessSpecifier::Private,
                                        Loc, Idents.get("Hashable"),
                                        {}, {});

         // def hashValue() -> UInt64
         auto *func = MethodDecl::Create(QC.Context, AccessSpecifier::Public,
                                         Loc, Idents.get("hashValue"),
                                         SourceType(QC.Context.getUInt64Ty()),
                                         { QC.Sema->MakeSelfArg(Loc) },
                                         {}, nullptr, false);

         QC.Sema->ActOnDecl(P, func);
         QC.Sema->ActOnDecl(NS, P);

         Cache[this->P] = P;
         return finish(P);
      }
      default:
         return finish(nullptr);
      }
   }

   Module *Mod;
   IdentifierInfo *II;

   switch (P) {
   case Any:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      II = sema().getIdentifier("Any");
      break;
   case Equatable:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      II = sema().getIdentifier("Equatable");
      break;
   case Hashable:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      II = sema().getIdentifier("Hashable");
      break;
   case Copyable:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      II = sema().getIdentifier("Copyable");
      break;
   case MoveOnly:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      II = sema().getIdentifier("MoveOnly");
      break;
   case ImplicitlyCopyable:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      II = sema().getIdentifier("ImplicitlyCopyable");
      break;
   case StringRepresentable:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      II = sema().getIdentifier("StringRepresentable");
      break;
   case Persistable:
      if (QC.GetBuiltinModule(Mod, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      II = sema().getIdentifier("Persistable");
      break;
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

   NamedDecl *LookupRes;
   if (QC.LookupSingle(LookupRes, Mod->getDecl(), II)) {
      return fail();
   }

   auto *Res = dyn_cast<ProtocolDecl>(LookupRes);
   Cache[P] = Res;

   return finish(Res);
}