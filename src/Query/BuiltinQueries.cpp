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

   if (Mod == Std) {
      auto Loc = QC.CI.getMainFileLoc();
      auto *II = sema().getIdentifier("std");

      auto *M = QC.CI.getModuleMgr().LookupModule(Loc, Loc, II);
      Cache[Mod] = M;

      return finish(M);
   }

   IdentifierInfo *Idents[] = {
      sema().getIdentifier("std"), nullptr
   };

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