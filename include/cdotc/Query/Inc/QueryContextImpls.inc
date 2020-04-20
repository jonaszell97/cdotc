template<> CompileModuleQuery *QueryContext::getQuery<CompileModuleQuery>()
{
   return CompileModuleInstance;
}

QueryResult QueryContext::CompileModule()

{
   if (!CompileModuleInstance)
      CompileModuleInstance = new(*this) CompileModuleQuery(*this);
auto *_Q = CompileModuleInstance;

#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ParseModuleFileQuery *QueryContext::getQuery<ParseModuleFileQuery>(StringRef FileName)
{
   auto It = ParseModuleFileQueries.find(FileName);
   if (It != ParseModuleFileQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::ParseModuleFile(Module* &Result, StringRef FileName)

{
   auto Key = FileName;
   auto It = ParseModuleFileQueries.find(Key);
   ParseModuleFileQuery *_Q;
   if (It == ParseModuleFileQueries.end()) {
      _Q = new(*this) ParseModuleFileQuery(*this, FileName);
      ParseModuleFileQueries[_Q->FileName] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> CreateDefaultModuleQuery *QueryContext::getQuery<CreateDefaultModuleQuery>()
{
   return CreateDefaultModuleInstance;
}

QueryResult QueryContext::CreateDefaultModule(Module* &Result)

{
   if (!CreateDefaultModuleInstance)
      CreateDefaultModuleInstance = new(*this) CreateDefaultModuleQuery(*this);
auto *_Q = CreateDefaultModuleInstance;

#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ParseSourceFileQuery *QueryContext::getQuery<ParseSourceFileQuery>(Module* Mod, StringRef FileName)
{
   llvm::FoldingSetNodeID ID;
   ParseSourceFileQuery::Profile(ID, Mod, FileName);
   void *InsertPos;
   return ParseSourceFileQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::ParseSourceFile(ast::SourceFileDecl* &Result, Module* Mod, StringRef FileName)

{
   llvm::FoldingSetNodeID ID;
   ParseSourceFileQuery::Profile(ID, Mod, FileName);
   void *InsertPos;
   auto *_Q = ParseSourceFileQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) ParseSourceFileQuery(*this, Mod, FileName);
      ParseSourceFileQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ParseMainSourceFileQuery *QueryContext::getQuery<ParseMainSourceFileQuery>(Module* Mod, StringRef FileName)
{
   llvm::FoldingSetNodeID ID;
   ParseMainSourceFileQuery::Profile(ID, Mod, FileName);
   void *InsertPos;
   return ParseMainSourceFileQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::ParseMainSourceFile(ast::SourceFileDecl* &Result, Module* Mod, StringRef FileName)

{
   llvm::FoldingSetNodeID ID;
   ParseMainSourceFileQuery::Profile(ID, Mod, FileName);
   void *InsertPos;
   auto *_Q = ParseMainSourceFileQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) ParseMainSourceFileQuery(*this, Mod, FileName);
      ParseMainSourceFileQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ParseSourceFilesQuery *QueryContext::getQuery<ParseSourceFilesQuery>(Module* Mod)
{
   auto It = ParseSourceFilesQueries.find(Mod);
   if (It != ParseSourceFilesQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::ParseSourceFiles(Module* Mod)

{
   auto Key = Mod;
   auto It = ParseSourceFilesQueries.find(Key);
   ParseSourceFilesQuery *_Q;
   if (It == ParseSourceFilesQueries.end()) {
      _Q = new(*this) ParseSourceFilesQuery(*this, Mod);
      ParseSourceFilesQueries[_Q->Mod] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> CreateILModuleQuery *QueryContext::getQuery<CreateILModuleQuery>(Module* Mod)
{
   auto It = CreateILModuleQueries.find(Mod);
   if (It != CreateILModuleQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::CreateILModule(il::Module* &Result, Module* Mod)

{
   auto Key = Mod;
   auto It = CreateILModuleQueries.find(Key);
   CreateILModuleQuery *_Q;
   if (It == CreateILModuleQueries.end()) {
      _Q = new(*this) CreateILModuleQuery(*this, Mod);
      CreateILModuleQueries[_Q->Mod] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> SetupIRGenQuery *QueryContext::getQuery<SetupIRGenQuery>()
{
   return SetupIRGenInstance;
}

QueryResult QueryContext::SetupIRGen(il::IRGen* &Result)

{
   if (!SetupIRGenInstance)
      SetupIRGenInstance = new(*this) SetupIRGenQuery(*this);
auto *_Q = SetupIRGenInstance;

#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> CreateLLVMModuleQuery *QueryContext::getQuery<CreateLLVMModuleQuery>(Module* Mod)
{
   auto It = CreateLLVMModuleQueries.find(Mod);
   if (It != CreateLLVMModuleQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::CreateLLVMModule(llvm::Module* &Result, Module* Mod)

{
   auto Key = Mod;
   auto It = CreateLLVMModuleQueries.find(Key);
   CreateLLVMModuleQuery *_Q;
   if (It == CreateLLVMModuleQueries.end()) {
      _Q = new(*this) CreateLLVMModuleQuery(*this, Mod);
      CreateLLVMModuleQueries[_Q->Mod] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::CreateObject(llvm::raw_ostream& OS)

{
   auto *_Q = new(*this) CreateObjectQuery(*this, OS);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::CreateStaticLib(StringRef OutFile)

{
   auto *_Q = new(*this) CreateStaticLibQuery(*this, OutFile);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::CreateDynamicLib(StringRef OutFile)

{
   auto *_Q = new(*this) CreateDynamicLibQuery(*this, OutFile);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::CreateExecutable(StringRef OutFile)

{
   auto *_Q = new(*this) CreateExecutableQuery(*this, OutFile);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::EmitIL(llvm::raw_ostream& OS)

{
   auto *_Q = new(*this) EmitILQuery(*this, OS);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::EmitIR(llvm::raw_ostream& OS)

{
   auto *_Q = new(*this) EmitIRQuery(*this, OS);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::PrintUsedMemory()

{
   auto *_Q = new(*this) PrintUsedMemoryQuery(*this);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> MeasureExecutionTimeQuery *QueryContext::getQuery<MeasureExecutionTimeQuery>(Query* Q)
{
   auto It = MeasureExecutionTimeQueries.find(Q);
   if (It != MeasureExecutionTimeQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::MeasureExecutionTime(long long &Result, Query* Q)

{
   auto Key = Q;
   auto It = MeasureExecutionTimeQueries.find(Key);
   MeasureExecutionTimeQuery *_Q;
   if (It == MeasureExecutionTimeQueries.end()) {
      _Q = new(*this) MeasureExecutionTimeQuery(*this, Q);
      MeasureExecutionTimeQueries[_Q->Q] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> IsPersistableQuery *QueryContext::getQuery<IsPersistableQuery>(CanType T)
{
   auto It = IsPersistableQueries.find(T);
   if (It != IsPersistableQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::IsPersistable(bool &Result, CanType T)

{
   auto Key = T;
   auto It = IsPersistableQueries.find(Key);
   IsPersistableQuery *_Q;
   if (It == IsPersistableQueries.end()) {
      _Q = new(*this) IsPersistableQuery(*this, T);
      IsPersistableQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> IsEquatableQuery *QueryContext::getQuery<IsEquatableQuery>(CanType T)
{
   auto It = IsEquatableQueries.find(T);
   if (It != IsEquatableQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::IsEquatable(bool &Result, CanType T)

{
   auto Key = T;
   auto It = IsEquatableQueries.find(Key);
   IsEquatableQuery *_Q;
   if (It == IsEquatableQueries.end()) {
      _Q = new(*this) IsEquatableQuery(*this, T);
      IsEquatableQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> IsCopyableQuery *QueryContext::getQuery<IsCopyableQuery>(CanType T)
{
   auto It = IsCopyableQueries.find(T);
   if (It != IsCopyableQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::IsCopyable(bool &Result, CanType T)

{
   auto Key = T;
   auto It = IsCopyableQueries.find(Key);
   IsCopyableQuery *_Q;
   if (It == IsCopyableQueries.end()) {
      _Q = new(*this) IsCopyableQuery(*this, T);
      IsCopyableQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> IsMoveOnlyQuery *QueryContext::getQuery<IsMoveOnlyQuery>(CanType T)
{
   auto It = IsMoveOnlyQueries.find(T);
   if (It != IsMoveOnlyQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::IsMoveOnly(bool &Result, CanType T)

{
   auto Key = T;
   auto It = IsMoveOnlyQueries.find(Key);
   IsMoveOnlyQuery *_Q;
   if (It == IsMoveOnlyQueries.end()) {
      _Q = new(*this) IsMoveOnlyQuery(*this, T);
      IsMoveOnlyQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> IsUnpopulatedQuery *QueryContext::getQuery<IsUnpopulatedQuery>(CanType T)
{
   auto It = IsUnpopulatedQueries.find(T);
   if (It != IsUnpopulatedQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::IsUnpopulated(bool &Result, CanType T)

{
   auto Key = T;
   auto It = IsUnpopulatedQueries.find(Key);
   IsUnpopulatedQuery *_Q;
   if (It == IsUnpopulatedQueries.end()) {
      _Q = new(*this) IsUnpopulatedQuery(*this, T);
      IsUnpopulatedQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> IsImplicitlyCopyableQuery *QueryContext::getQuery<IsImplicitlyCopyableQuery>(CanType T)
{
   auto It = IsImplicitlyCopyableQueries.find(T);
   if (It != IsImplicitlyCopyableQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::IsImplicitlyCopyable(bool &Result, CanType T)

{
   auto Key = T;
   auto It = IsImplicitlyCopyableQueries.find(Key);
   IsImplicitlyCopyableQuery *_Q;
   if (It == IsImplicitlyCopyableQueries.end()) {
      _Q = new(*this) IsImplicitlyCopyableQuery(*this, T);
      IsImplicitlyCopyableQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> IsTriviallyCopyableQuery *QueryContext::getQuery<IsTriviallyCopyableQuery>(CanType T)
{
   auto It = IsTriviallyCopyableQueries.find(T);
   if (It != IsTriviallyCopyableQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::IsTriviallyCopyable(bool &Result, CanType T)

{
   auto Key = T;
   auto It = IsTriviallyCopyableQueries.find(Key);
   IsTriviallyCopyableQuery *_Q;
   if (It == IsTriviallyCopyableQueries.end()) {
      _Q = new(*this) IsTriviallyCopyableQuery(*this, T);
      IsTriviallyCopyableQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> NeedsRetainOrReleaseQuery *QueryContext::getQuery<NeedsRetainOrReleaseQuery>(CanType T)
{
   auto It = NeedsRetainOrReleaseQueries.find(T);
   if (It != NeedsRetainOrReleaseQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::NeedsRetainOrRelease(bool &Result, CanType T)

{
   auto Key = T;
   auto It = NeedsRetainOrReleaseQueries.find(Key);
   NeedsRetainOrReleaseQuery *_Q;
   if (It == NeedsRetainOrReleaseQueries.end()) {
      _Q = new(*this) NeedsRetainOrReleaseQuery(*this, T);
      NeedsRetainOrReleaseQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> NeedsDeinitilizationQuery *QueryContext::getQuery<NeedsDeinitilizationQuery>(CanType T)
{
   auto It = NeedsDeinitilizationQueries.find(T);
   if (It != NeedsDeinitilizationQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::NeedsDeinitilization(bool &Result, CanType T)

{
   auto Key = T;
   auto It = NeedsDeinitilizationQueries.find(Key);
   NeedsDeinitilizationQuery *_Q;
   if (It == NeedsDeinitilizationQueries.end()) {
      _Q = new(*this) NeedsDeinitilizationQuery(*this, T);
      NeedsDeinitilizationQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> NeedsStructReturnQuery *QueryContext::getQuery<NeedsStructReturnQuery>(CanType T)
{
   auto It = NeedsStructReturnQueries.find(T);
   if (It != NeedsStructReturnQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::NeedsStructReturn(bool &Result, CanType T)

{
   auto Key = T;
   auto It = NeedsStructReturnQueries.find(Key);
   NeedsStructReturnQuery *_Q;
   if (It == NeedsStructReturnQueries.end()) {
      _Q = new(*this) NeedsStructReturnQuery(*this, T);
      NeedsStructReturnQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PassByValueQuery *QueryContext::getQuery<PassByValueQuery>(CanType T)
{
   auto It = PassByValueQueries.find(T);
   if (It != PassByValueQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PassByValue(bool &Result, CanType T)

{
   auto Key = T;
   auto It = PassByValueQueries.find(Key);
   PassByValueQuery *_Q;
   if (It == PassByValueQueries.end()) {
      _Q = new(*this) PassByValueQuery(*this, T);
      PassByValueQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ContainsAssociatedTypeConstraintQuery *QueryContext::getQuery<ContainsAssociatedTypeConstraintQuery>(CanType T)
{
   auto It = ContainsAssociatedTypeConstraintQueries.find(T);
   if (It != ContainsAssociatedTypeConstraintQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::ContainsAssociatedTypeConstraint(bool &Result, CanType T)

{
   auto Key = T;
   auto It = ContainsAssociatedTypeConstraintQueries.find(Key);
   ContainsAssociatedTypeConstraintQuery *_Q;
   if (It == ContainsAssociatedTypeConstraintQueries.end()) {
      _Q = new(*this) ContainsAssociatedTypeConstraintQuery(*this, T);
      ContainsAssociatedTypeConstraintQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::IsImplicitlyConvertible(IsImplicitlyConvertibleQuery::ResultType &Result, QualType From, QualType To)

{
   auto Key = std::make_pair(From, To);
   auto It = IsImplicitlyConvertibleQueries.find(Key);
   if (It != IsImplicitlyConvertibleQueries.end() && !shouldReset()) {
      Result = It->getSecond();      return QueryResult(QueryResult::Success);   }
IsImplicitlyConvertibleQuery _Q(*this, From, To);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

Result = _Q.get();
IsImplicitlyConvertibleQueries[Key] = Result;
return QueryResult(QueryResult::Success);
}

QueryResult QueryContext::IsValidParameterValue(IsValidParameterValueQuery::ResultType &Result, QualType givenType, QualType paramType, uint8_t isSelf)

{
   auto Key = std::make_pair(givenType, std::make_pair(paramType, isSelf));
   auto It = IsValidParameterValueQueries.find(Key);
   if (It != IsValidParameterValueQueries.end() && !shouldReset()) {
      Result = It->getSecond();      return QueryResult(QueryResult::Success);   }
IsValidParameterValueQuery _Q(*this, givenType, paramType, isSelf);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

Result = _Q.get();
IsValidParameterValueQueries[Key] = Result;
return QueryResult(QueryResult::Success);
}

QueryResult QueryContext::GetConversionSequence(ConversionSequence* &Result, QualType From, QualType To)

{
   auto Key = std::make_pair(From, To);
   auto It = GetConversionSequenceQueries.find(Key);
   if (It != GetConversionSequenceQueries.end() && !shouldReset()) {
      Result = It->getSecond();      return QueryResult(QueryResult::Success);   }
GetConversionSequenceQuery _Q(*this, From, To);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

Result = _Q.get();
GetConversionSequenceQueries[Key] = Result;
return QueryResult(QueryResult::Success);
}

QueryResult QueryContext::SubstTypeVariables(QualType &Result, QualType T, const llvm::DenseMap<TypeVariableType*, QualType>& ReplacementMap, SourceRange SR)

{
SubstTypeVariablesQuery _Q(*this, T, ReplacementMap, SR);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

Result = _Q.get();
return QueryResult(QueryResult::Success);
}

template<> SubstAssociatedTypesQuery *QueryContext::getQuery<SubstAssociatedTypesQuery>(QualType T, QualType Self)
{
   llvm::FoldingSetNodeID ID;
   SubstAssociatedTypesQuery::Profile(ID, T, Self);
   void *InsertPos;
   return SubstAssociatedTypesQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::SubstAssociatedTypes(QualType &Result, QualType T, QualType Self, SourceRange SR)

{
   llvm::FoldingSetNodeID ID;
   SubstAssociatedTypesQuery::Profile(ID, T, Self);
   void *InsertPos;
   auto *_Q = SubstAssociatedTypesQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) SubstAssociatedTypesQuery(*this, T, Self, SR);
      SubstAssociatedTypesQueries.InsertNode(_Q, InsertPos);
   }   _Q->refresh();

#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> SubstTemplateParamTypesQuery *QueryContext::getQuery<SubstTemplateParamTypesQuery>(QualType T, const sema::MultiLevelFinalTemplateArgList &TemplateArgs)
{
   llvm::FoldingSetNodeID ID;
   SubstTemplateParamTypesQuery::Profile(ID, T, TemplateArgs);
   void *InsertPos;
   return SubstTemplateParamTypesQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::SubstTemplateParamTypes(QualType &Result, QualType T, sema::MultiLevelFinalTemplateArgList TemplateArgs, SourceRange SR)

{
   llvm::FoldingSetNodeID ID;
   SubstTemplateParamTypesQuery::Profile(ID, T, TemplateArgs);
   void *InsertPos;
   auto *_Q = SubstTemplateParamTypesQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) SubstTemplateParamTypesQuery(*this, T, std::move(TemplateArgs), SR);
      SubstTemplateParamTypesQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::SubstTemplateParamTypesNonFinal(QualType &Result, QualType T, const sema::MultiLevelTemplateArgList& TemplateArgs, SourceRange SR)

{
SubstTemplateParamTypesNonFinalQuery _Q(*this, T, TemplateArgs, SR);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

Result = _Q.get();
return QueryResult(QueryResult::Success);
}

template<> CheckTypeCapabilitiesQuery *QueryContext::getQuery<CheckTypeCapabilitiesQuery>(ast::NamedDecl* ND)
{
   auto It = CheckTypeCapabilitiesQueries.find(ND);
   if (It != CheckTypeCapabilitiesQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::CheckTypeCapabilities(ArrayRef<TypeCapability> &Result, ast::NamedDecl* ND)

{
   auto Key = ND;
   auto It = CheckTypeCapabilitiesQueries.find(Key);
   CheckTypeCapabilitiesQuery *_Q;
   if (It == CheckTypeCapabilitiesQueries.end()) {
      _Q = new(*this) CheckTypeCapabilitiesQuery(*this, ND);
      CheckTypeCapabilitiesQueries[_Q->ND] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetBuiltinModuleQuery *QueryContext::getQuery<GetBuiltinModuleQuery>(GetBuiltinModuleQuery::ModuleKind Mod)
{
   auto It = GetBuiltinModuleQueries.find(Mod);
   if (It != GetBuiltinModuleQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetBuiltinModule(Module* &Result, GetBuiltinModuleQuery::ModuleKind Mod)

{
   auto Key = Mod;
   auto It = GetBuiltinModuleQueries.find(Key);
   GetBuiltinModuleQuery *_Q;
   if (It == GetBuiltinModuleQueries.end()) {
      _Q = new(*this) GetBuiltinModuleQuery(*this, Mod);
      GetBuiltinModuleQueries[_Q->Mod] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetBuiltinFuncQuery *QueryContext::getQuery<GetBuiltinFuncQuery>(GetBuiltinFuncQuery::FunctionKind Fn)
{
   auto It = GetBuiltinFuncQueries.find(Fn);
   if (It != GetBuiltinFuncQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetBuiltinFunc(ast::CallableDecl* &Result, GetBuiltinFuncQuery::FunctionKind Fn)

{
   auto Key = Fn;
   auto It = GetBuiltinFuncQueries.find(Key);
   GetBuiltinFuncQuery *_Q;
   if (It == GetBuiltinFuncQueries.end()) {
      _Q = new(*this) GetBuiltinFuncQuery(*this, Fn);
      GetBuiltinFuncQueries[_Q->Fn] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetBuiltinAliasQuery *QueryContext::getQuery<GetBuiltinAliasQuery>(GetBuiltinAliasQuery::AliasKind K, ast::DeclContext* DC)
{
   llvm::FoldingSetNodeID ID;
   GetBuiltinAliasQuery::Profile(ID, K, DC);
   void *InsertPos;
   return GetBuiltinAliasQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::GetBuiltinAlias(ast::AliasDecl* &Result, GetBuiltinAliasQuery::AliasKind K, ast::DeclContext* DC)

{
   llvm::FoldingSetNodeID ID;
   GetBuiltinAliasQuery::Profile(ID, K, DC);
   void *InsertPos;
   auto *_Q = GetBuiltinAliasQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) GetBuiltinAliasQuery(*this, K, DC);
      GetBuiltinAliasQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetBuiltinRecordQuery *QueryContext::getQuery<GetBuiltinRecordQuery>(GetBuiltinRecordQuery::RecordKind R)
{
   auto It = GetBuiltinRecordQueries.find(R);
   if (It != GetBuiltinRecordQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetBuiltinRecord(ast::RecordDecl* &Result, GetBuiltinRecordQuery::RecordKind R)

{
   auto Key = R;
   auto It = GetBuiltinRecordQueries.find(Key);
   GetBuiltinRecordQuery *_Q;
   if (It == GetBuiltinRecordQueries.end()) {
      _Q = new(*this) GetBuiltinRecordQuery(*this, R);
      GetBuiltinRecordQueries[_Q->R] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetBuiltinProtocolQuery *QueryContext::getQuery<GetBuiltinProtocolQuery>(GetBuiltinProtocolQuery::ProtocolKind P)
{
   auto It = GetBuiltinProtocolQueries.find(P);
   if (It != GetBuiltinProtocolQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetBuiltinProtocol(ast::ProtocolDecl* &Result, GetBuiltinProtocolQuery::ProtocolKind P)

{
   auto Key = P;
   auto It = GetBuiltinProtocolQueries.find(Key);
   GetBuiltinProtocolQuery *_Q;
   if (It == GetBuiltinProtocolQueries.end()) {
      _Q = new(*this) GetBuiltinProtocolQuery(*this, P);
      GetBuiltinProtocolQueries[_Q->P] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> FindPrecedenceGroupQuery *QueryContext::getQuery<FindPrecedenceGroupQuery>(DeclarationName Name, bool DiagOnError)
{
   llvm::FoldingSetNodeID ID;
   FindPrecedenceGroupQuery::Profile(ID, Name, DiagOnError);
   void *InsertPos;
   return FindPrecedenceGroupQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::FindPrecedenceGroup(ast::PrecedenceGroupDecl* &Result, DeclarationName Name, bool DiagOnError, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   FindPrecedenceGroupQuery::Profile(ID, Name, DiagOnError);
   void *InsertPos;
   auto *_Q = FindPrecedenceGroupQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) FindPrecedenceGroupQuery(*this, Name, DiagOnError, Loc);
      FindPrecedenceGroupQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> FindOperatorQuery *QueryContext::getQuery<FindOperatorQuery>(DeclarationName Name, bool DiagOnError)
{
   llvm::FoldingSetNodeID ID;
   FindOperatorQuery::Profile(ID, Name, DiagOnError);
   void *InsertPos;
   return FindOperatorQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::FindOperator(ast::OperatorDecl* &Result, DeclarationName Name, bool DiagOnError, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   FindOperatorQuery::Profile(ID, Name, DiagOnError);
   void *InsertPos;
   auto *_Q = FindOperatorQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) FindOperatorQuery(*this, Name, DiagOnError, Loc);
      FindOperatorQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveImportQuery *QueryContext::getQuery<ResolveImportQuery>(ast::ImportDecl* I)
{
   llvm::FoldingSetNodeID ID;
   ResolveImportQuery::Profile(ID, I);
   void *InsertPos;
   return ResolveImportQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::ResolveImport(ast::ImportDecl* I, llvm::SmallVectorImpl<ast::NamedDecl*>* FoundDecls)

{
   llvm::FoldingSetNodeID ID;
   ResolveImportQuery::Profile(ID, I);
   void *InsertPos;
   auto *_Q = ResolveImportQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) ResolveImportQuery(*this, I, FoundDecls);
      ResolveImportQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Idle;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveUsingQuery *QueryContext::getQuery<ResolveUsingQuery>(ast::UsingDecl* U)
{
   llvm::FoldingSetNodeID ID;
   ResolveUsingQuery::Profile(ID, U);
   void *InsertPos;
   return ResolveUsingQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::ResolveUsing(ast::UsingDecl* U, llvm::SmallVectorImpl<ast::NamedDecl*>* FoundDecls)

{
   llvm::FoldingSetNodeID ID;
   ResolveUsingQuery::Profile(ID, U);
   void *InsertPos;
   auto *_Q = ResolveUsingQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) ResolveUsingQuery(*this, U, FoundDecls);
      ResolveUsingQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Idle;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveStaticIfQuery *QueryContext::getQuery<ResolveStaticIfQuery>(ast::StaticIfDecl* Decl)
{
   auto It = ResolveStaticIfQueries.find(Decl);
   if (It != ResolveStaticIfQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::ResolveStaticIf(ast::Decl* &Result, ast::StaticIfDecl* Decl)

{
   auto Key = Decl;
   auto It = ResolveStaticIfQueries.find(Key);
   ResolveStaticIfQuery *_Q;
   if (It == ResolveStaticIfQueries.end()) {
      _Q = new(*this) ResolveStaticIfQuery(*this, Decl);
      ResolveStaticIfQueries[_Q->Decl] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Idle;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveStaticForQuery *QueryContext::getQuery<ResolveStaticForQuery>(ast::StaticForDecl* Decl)
{
   auto It = ResolveStaticForQueries.find(Decl);
   if (It != ResolveStaticForQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::ResolveStaticFor(ast::Decl* &Result, ast::StaticForDecl* Decl)

{
   auto Key = Decl;
   auto It = ResolveStaticForQueries.find(Key);
   ResolveStaticForQuery *_Q;
   if (It == ResolveStaticForQueries.end()) {
      _Q = new(*this) ResolveStaticForQuery(*this, Decl);
      ResolveStaticForQueries[_Q->Decl] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Idle;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveStaticAssertQuery *QueryContext::getQuery<ResolveStaticAssertQuery>(ast::StaticAssertDecl* Decl)
{
   auto It = ResolveStaticAssertQueries.find(Decl);
   if (It != ResolveStaticAssertQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::ResolveStaticAssert(ast::StaticAssertDecl* Decl)

{
   auto Key = Decl;
   auto It = ResolveStaticAssertQueries.find(Key);
   ResolveStaticAssertQuery *_Q;
   if (It == ResolveStaticAssertQueries.end()) {
      _Q = new(*this) ResolveStaticAssertQuery(*this, Decl);
      ResolveStaticAssertQueries[_Q->Decl] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Idle;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveStaticPrintQuery *QueryContext::getQuery<ResolveStaticPrintQuery>(ast::StaticPrintDecl* Decl)
{
   auto It = ResolveStaticPrintQueries.find(Decl);
   if (It != ResolveStaticPrintQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::ResolveStaticPrint(ast::StaticPrintDecl* Decl)

{
   auto Key = Decl;
   auto It = ResolveStaticPrintQueries.find(Key);
   ResolveStaticPrintQuery *_Q;
   if (It == ResolveStaticPrintQueries.end()) {
      _Q = new(*this) ResolveStaticPrintQuery(*this, Decl);
      ResolveStaticPrintQueries[_Q->Decl] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Idle;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveStaticExprQuery *QueryContext::getQuery<ResolveStaticExprQuery>(ast::StaticExpr* Expr)
{
   auto It = ResolveStaticExprQueries.find(Expr);
   if (It != ResolveStaticExprQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::ResolveStaticExpr(il::Constant* &Result, ast::StaticExpr* Expr)

{
   auto Key = Expr;
   auto It = ResolveStaticExprQueries.find(Key);
   ResolveStaticExprQuery *_Q;
   if (It == ResolveStaticExprQueries.end()) {
      _Q = new(*this) ResolveStaticExprQuery(*this, Expr);
      ResolveStaticExprQueries[_Q->Expr] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveStaticExprToBoolQuery *QueryContext::getQuery<ResolveStaticExprToBoolQuery>(ast::StaticExpr* Expr, bool DiagOnError)
{
   llvm::FoldingSetNodeID ID;
   ResolveStaticExprToBoolQuery::Profile(ID, Expr, DiagOnError);
   void *InsertPos;
   return ResolveStaticExprToBoolQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::ResolveStaticExprToBool(bool &Result, ast::StaticExpr* Expr, bool DiagOnError)

{
   llvm::FoldingSetNodeID ID;
   ResolveStaticExprToBoolQuery::Profile(ID, Expr, DiagOnError);
   void *InsertPos;
   auto *_Q = ResolveStaticExprToBoolQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) ResolveStaticExprToBoolQuery(*this, Expr, DiagOnError);
      ResolveStaticExprToBoolQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ExpandMacroQuery *QueryContext::getQuery<ExpandMacroQuery>(StmtOrDecl SOD, DeclarationName Name, ast::DeclContext* DC, unsigned Delim, llvm::ArrayRef<lex::Token> Tokens, unsigned ExpectedKind)
{
   llvm::FoldingSetNodeID ID;
   ExpandMacroQuery::Profile(ID, SOD, Name, DC, Delim, Tokens, ExpectedKind);
   void *InsertPos;
   return ExpandMacroQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::ExpandMacro(StmtOrDecl &Result, StmtOrDecl SOD, DeclarationName Name, ast::DeclContext* DC, unsigned Delim, llvm::ArrayRef<lex::Token> Tokens, unsigned ExpectedKind)

{
   llvm::FoldingSetNodeID ID;
   ExpandMacroQuery::Profile(ID, SOD, Name, DC, Delim, Tokens, ExpectedKind);
   void *InsertPos;
   auto *_Q = ExpandMacroQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) ExpandMacroQuery(*this, SOD, Name, DC, Delim, Tokens, ExpectedKind);
      ExpandMacroQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ExpandMacroDeclQuery *QueryContext::getQuery<ExpandMacroDeclQuery>(ast::MacroExpansionDecl* Decl)
{
   auto It = ExpandMacroDeclQueries.find(Decl);
   if (It != ExpandMacroDeclQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::ExpandMacroDecl(ast::Decl* &Result, ast::MacroExpansionDecl* Decl)

{
   auto Key = Decl;
   auto It = ExpandMacroDeclQueries.find(Key);
   ExpandMacroDeclQuery *_Q;
   if (It == ExpandMacroDeclQueries.end()) {
      _Q = new(*this) ExpandMacroDeclQuery(*this, Decl);
      ExpandMacroDeclQueries[_Q->Decl] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Idle;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ExpandMacrosQuery *QueryContext::getQuery<ExpandMacrosQuery>(StringRef SourceFile)
{
   auto It = ExpandMacrosQueries.find(SourceFile);
   if (It != ExpandMacrosQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::ExpandMacros(StringRef SourceFile)

{
   auto Key = SourceFile;
   auto It = ExpandMacrosQueries.find(Key);
   ExpandMacrosQuery *_Q;
   if (It == ExpandMacrosQueries.end()) {
      _Q = new(*this) ExpandMacrosQuery(*this, SourceFile);
      ExpandMacrosQueries[_Q->SourceFile] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ImportExternalSourceFileQuery *QueryContext::getQuery<ImportExternalSourceFileQuery>(StringRef FileName, ImportExternalSourceFileQuery::HeaderKind Kind, ast::DeclContext* DC, SourceLocation Loc, bool Optional)
{
   llvm::FoldingSetNodeID ID;
   ImportExternalSourceFileQuery::Profile(ID, FileName, Kind, DC, Loc, Optional);
   void *InsertPos;
   return ImportExternalSourceFileQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::ImportExternalSourceFile(StringRef FileName, ImportExternalSourceFileQuery::HeaderKind Kind, ast::DeclContext* DC, SourceLocation Loc, bool Optional)

{
   llvm::FoldingSetNodeID ID;
   ImportExternalSourceFileQuery::Profile(ID, FileName, Kind, DC, Loc, Optional);
   void *InsertPos;
   auto *_Q = ImportExternalSourceFileQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) ImportExternalSourceFileQuery(*this, FileName, Kind, DC, Loc, Optional);
      ImportExternalSourceFileQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> FindExtensionsQuery *QueryContext::getQuery<FindExtensionsQuery>(QualType T)
{
   auto It = FindExtensionsQueries.find(T);
   if (It != FindExtensionsQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::FindExtensions(QualType T)

{
   auto Key = T;
   auto It = FindExtensionsQueries.find(Key);
   FindExtensionsQuery *_Q;
   if (It == FindExtensionsQueries.end()) {
      _Q = new(*this) FindExtensionsQuery(*this, T);
      FindExtensionsQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetExtensionTypeKindQuery *QueryContext::getQuery<GetExtensionTypeKindQuery>(ast::SourceType T)
{
   auto It = GetExtensionTypeKindQueries.find(T);
   if (It != GetExtensionTypeKindQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetExtensionTypeKind(GetExtensionTypeKindQuery::ResultKind &Result, ast::SourceType T)

{
   auto Key = T;
   auto It = GetExtensionTypeKindQueries.find(Key);
   GetExtensionTypeKindQuery *_Q;
   if (It == GetExtensionTypeKindQueries.end()) {
      _Q = new(*this) GetExtensionTypeKindQuery(*this, T);
      GetExtensionTypeKindQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> MatchExtensionTypeQuery *QueryContext::getQuery<MatchExtensionTypeQuery>(QualType T, ast::SourceType PatternTy)
{
   llvm::FoldingSetNodeID ID;
   MatchExtensionTypeQuery::Profile(ID, T, PatternTy);
   void *InsertPos;
   return MatchExtensionTypeQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::MatchExtensionType(MatchExtensionTypeQuery::ResultKind &Result, QualType T, ast::SourceType PatternTy)

{
   llvm::FoldingSetNodeID ID;
   MatchExtensionTypeQuery::Profile(ID, T, PatternTy);
   void *InsertPos;
   auto *_Q = MatchExtensionTypeQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) MatchExtensionTypeQuery(*this, T, PatternTy);
      MatchExtensionTypeQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ExtensionAppliesQuery *QueryContext::getQuery<ExtensionAppliesQuery>(ast::ExtensionDecl* Ext, QualType T)
{
   llvm::FoldingSetNodeID ID;
   ExtensionAppliesQuery::Profile(ID, Ext, T);
   void *InsertPos;
   return ExtensionAppliesQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::ExtensionApplies(MatchExtensionTypeQuery::ResultKind &Result, ast::ExtensionDecl* Ext, QualType T)

{
   llvm::FoldingSetNodeID ID;
   ExtensionAppliesQuery::Profile(ID, Ext, T);
   void *InsertPos;
   auto *_Q = ExtensionAppliesQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) ExtensionAppliesQuery(*this, Ext, T);
      ExtensionAppliesQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetExtendedDeclQuery *QueryContext::getQuery<GetExtendedDeclQuery>(ast::SourceType ExtendedTy)
{
   auto It = GetExtendedDeclQueries.find(ExtendedTy);
   if (It != GetExtendedDeclQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetExtendedDecl(ast::NamedDecl* &Result, ast::SourceType ExtendedTy)

{
   auto Key = ExtendedTy;
   auto It = GetExtendedDeclQueries.find(Key);
   GetExtendedDeclQuery *_Q;
   if (It == GetExtendedDeclQueries.end()) {
      _Q = new(*this) GetExtendedDeclQuery(*this, ExtendedTy);
      GetExtendedDeclQueries[_Q->ExtendedTy] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveExtensionQuery *QueryContext::getQuery<ResolveExtensionQuery>(ast::ExtensionDecl* Ext)
{
   auto It = ResolveExtensionQueries.find(Ext);
   if (It != ResolveExtensionQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::ResolveExtension(ast::ExtensionDecl* Ext)

{
   auto Key = Ext;
   auto It = ResolveExtensionQueries.find(Key);
   ResolveExtensionQuery *_Q;
   if (It == ResolveExtensionQueries.end()) {
      _Q = new(*this) ResolveExtensionQuery(*this, Ext);
      ResolveExtensionQueries[_Q->Ext] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveConformanceToProtocolQuery *QueryContext::getQuery<ResolveConformanceToProtocolQuery>(QualType T, const ast::SourceType& Conf)
{
   llvm::FoldingSetNodeID ID;
   ResolveConformanceToProtocolQuery::Profile(ID, T, Conf);
   void *InsertPos;
   return ResolveConformanceToProtocolQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::ResolveConformanceToProtocol(ast::ProtocolDecl* &Result, QualType T, const ast::SourceType& Conf)

{
   llvm::FoldingSetNodeID ID;
   ResolveConformanceToProtocolQuery::Profile(ID, T, Conf);
   void *InsertPos;
   auto *_Q = ResolveConformanceToProtocolQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) ResolveConformanceToProtocolQuery(*this, T, Conf);
      ResolveConformanceToProtocolQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveExplicitConformancesQuery *QueryContext::getQuery<ResolveExplicitConformancesQuery>(QualType T)
{
   auto It = ResolveExplicitConformancesQueries.find(T);
   if (It != ResolveExplicitConformancesQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::ResolveExplicitConformances(QualType T)

{
   auto Key = T;
   auto It = ResolveExplicitConformancesQueries.find(Key);
   ResolveExplicitConformancesQuery *_Q;
   if (It == ResolveExplicitConformancesQueries.end()) {
      _Q = new(*this) ResolveExplicitConformancesQuery(*this, T);
      ResolveExplicitConformancesQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Idle;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveConformancesToProtocolsQuery *QueryContext::getQuery<ResolveConformancesToProtocolsQuery>(QualType T, llvm::MutableArrayRef<ast::SourceType> Conformances)
{
   llvm::FoldingSetNodeID ID;
   ResolveConformancesToProtocolsQuery::Profile(ID, T, Conformances);
   void *InsertPos;
   return ResolveConformancesToProtocolsQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::ResolveConformancesToProtocols(ArrayRef<ast::ProtocolDecl*> &Result, QualType T, llvm::MutableArrayRef<ast::SourceType> Conformances)

{
   llvm::FoldingSetNodeID ID;
   ResolveConformancesToProtocolsQuery::Profile(ID, T, Conformances);
   void *InsertPos;
   auto *_Q = ResolveConformancesToProtocolsQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) ResolveConformancesToProtocolsQuery(*this, T, Conformances);
      ResolveConformancesToProtocolsQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveDeclaredConformancesQuery *QueryContext::getQuery<ResolveDeclaredConformancesQuery>(QualType T, llvm::MutableArrayRef<ast::SourceType> Conformances, ConformanceKind Kind, ast::ConstraintSet* CS)
{
   llvm::FoldingSetNodeID ID;
   ResolveDeclaredConformancesQuery::Profile(ID, T, Conformances, Kind, CS);
   void *InsertPos;
   return ResolveDeclaredConformancesQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::ResolveDeclaredConformances(QualType T, llvm::MutableArrayRef<ast::SourceType> Conformances, ConformanceKind Kind, ast::ConstraintSet* CS)

{
   llvm::FoldingSetNodeID ID;
   ResolveDeclaredConformancesQuery::Profile(ID, T, Conformances, Kind, CS);
   void *InsertPos;
   auto *_Q = ResolveDeclaredConformancesQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) ResolveDeclaredConformancesQuery(*this, T, Conformances, Kind, CS);
      ResolveDeclaredConformancesQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> AddSingleConformanceQuery *QueryContext::getQuery<AddSingleConformanceQuery>(QualType T, ast::ProtocolDecl* Proto, ConformanceKind Kind, ast::ConstraintSet* CS, bool AddRecursiveConformances)
{
   llvm::FoldingSetNodeID ID;
   AddSingleConformanceQuery::Profile(ID, T, Proto, Kind, CS, AddRecursiveConformances);
   void *InsertPos;
   return AddSingleConformanceQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::AddSingleConformance(QualType T, ast::ProtocolDecl* Proto, ConformanceKind Kind, ast::ConstraintSet* CS, bool AddRecursiveConformances)

{
   llvm::FoldingSetNodeID ID;
   AddSingleConformanceQuery::Profile(ID, T, Proto, Kind, CS, AddRecursiveConformances);
   void *InsertPos;
   auto *_Q = AddSingleConformanceQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) AddSingleConformanceQuery(*this, T, Proto, Kind, CS, AddRecursiveConformances);
      AddSingleConformanceQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::DeclareSelfAlias(ast::RecordDecl* R)

{
   auto Key = R;
   auto It = DeclareSelfAliasQueries.find(Key);
   if (It != DeclareSelfAliasQueries.end() && !shouldReset()) {
      return QueryResult(QueryResult::Success);   }
DeclareSelfAliasQuery _Q(*this, R);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

DeclareSelfAliasQueries.insert(R);
return QueryResult(QueryResult::Success);
}

QueryResult QueryContext::ReferencedAssociatedTypesReady(ReferencedAssociatedTypesReadyQuery::ResultKind &Result, QualType T, ast::ConstraintSet* CS)

{

        if (!CS) {
            Result = ReferencedAssociatedTypesReadyQuery::ResultKind::Ready;
            return QueryResult(QueryResult::Success);
        }
       auto *_Q = new(*this) ReferencedAssociatedTypesReadyQuery(*this, T, CS);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> CheckProtocolExtensionApplicabilityQuery *QueryContext::getQuery<CheckProtocolExtensionApplicabilityQuery>(QualType T, ast::ExtensionDecl* Ext)
{
   llvm::FoldingSetNodeID ID;
   CheckProtocolExtensionApplicabilityQuery::Profile(ID, T, Ext);
   void *InsertPos;
   return CheckProtocolExtensionApplicabilityQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::CheckProtocolExtensionApplicability(CheckProtocolExtensionApplicabilityQuery::ResultKind &Result, QualType T, ast::ExtensionDecl* Ext)

{
   llvm::FoldingSetNodeID ID;
   CheckProtocolExtensionApplicabilityQuery::Profile(ID, T, Ext);
   void *InsertPos;
   auto *_Q = CheckProtocolExtensionApplicabilityQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) CheckProtocolExtensionApplicabilityQuery(*this, T, Ext);
      CheckProtocolExtensionApplicabilityQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> VerifyConstraintQuery *QueryContext::getQuery<VerifyConstraintQuery>(const ast::ParsedConstraint* C, ast::NamedDecl* ConstrainedDecl)
{
   llvm::FoldingSetNodeID ID;
   VerifyConstraintQuery::Profile(ID, C, ConstrainedDecl);
   void *InsertPos;
   return VerifyConstraintQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::VerifyConstraint(ast::DeclConstraint* &Result, const ast::ParsedConstraint* C, ast::NamedDecl* ConstrainedDecl)

{
   llvm::FoldingSetNodeID ID;
   VerifyConstraintQuery::Profile(ID, C, ConstrainedDecl);
   void *InsertPos;
   auto *_Q = VerifyConstraintQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) VerifyConstraintQuery(*this, C, ConstrainedDecl);
      VerifyConstraintQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> VerifyConstraintsQuery *QueryContext::getQuery<VerifyConstraintsQuery>(ast::NamedDecl* ConstrainedDecl)
{
   auto It = VerifyConstraintsQueries.find(ConstrainedDecl);
   if (It != VerifyConstraintsQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::VerifyConstraints(ast::ConstraintSet* &Result, ast::NamedDecl* ConstrainedDecl)

{
   auto Key = ConstrainedDecl;
   auto It = VerifyConstraintsQueries.find(Key);
   VerifyConstraintsQuery *_Q;
   if (It == VerifyConstraintsQueries.end()) {
      _Q = new(*this) VerifyConstraintsQuery(*this, ConstrainedDecl);
      VerifyConstraintsQueries[_Q->ConstrainedDecl] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> IsConstraintSatisfiedQuery *QueryContext::getQuery<IsConstraintSatisfiedQuery>(const ast::DeclConstraint* C, QualType Self, ast::NamedDecl* OriginalDecl)
{
   llvm::FoldingSetNodeID ID;
   IsConstraintSatisfiedQuery::Profile(ID, C, Self, OriginalDecl);
   void *InsertPos;
   return IsConstraintSatisfiedQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::IsConstraintSatisfied(bool &Result, const ast::DeclConstraint* C, QualType Self, ast::NamedDecl* OriginalDecl)

{
   llvm::FoldingSetNodeID ID;
   IsConstraintSatisfiedQuery::Profile(ID, C, Self, OriginalDecl);
   void *InsertPos;
   auto *_Q = IsConstraintSatisfiedQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) IsConstraintSatisfiedQuery(*this, C, Self, OriginalDecl);
      IsConstraintSatisfiedQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetConstrainedTypeQuery *QueryContext::getQuery<GetConstrainedTypeQuery>(const ast::DeclConstraint* C, QualType Self, ast::NamedDecl* OriginalDecl)
{
   llvm::FoldingSetNodeID ID;
   GetConstrainedTypeQuery::Profile(ID, C, Self, OriginalDecl);
   void *InsertPos;
   return GetConstrainedTypeQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::GetConstrainedType(QualType &Result, const ast::DeclConstraint* C, QualType Self, ast::NamedDecl* OriginalDecl)

{
   llvm::FoldingSetNodeID ID;
   GetConstrainedTypeQuery::Profile(ID, C, Self, OriginalDecl);
   void *InsertPos;
   auto *_Q = GetConstrainedTypeQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) GetConstrainedTypeQuery(*this, C, Self, OriginalDecl);
      GetConstrainedTypeQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

bool QueryContext::IsSupersetOf(ast::ConstraintSet* C1, ast::ConstraintSet* C2)

{

        if (!C2 || C2->empty()) {
            return true;
        }
        if (!C1 || C1->empty()) {
            return C2->empty();
        }
        if (C1 == C2) {
            return true;
        }
       auto Key = std::make_pair(C1, C2);
   auto It = IsSupersetOfQueries.find(Key);
   if (It != IsSupersetOfQueries.end() && !shouldReset()) {
      return It->getSecond();   }
IsSupersetOfQuery _Q(*this, C1, C2);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) llvm_unreachable("infallible query failed!");

auto Result = _Q.get();
IsSupersetOfQueries[Key] = Result;
return Result;
}

template<> GetReferencedAssociatedTypesQuery *QueryContext::getQuery<GetReferencedAssociatedTypesQuery>(ast::ConstraintSet* CS)
{
   auto It = GetReferencedAssociatedTypesQueries.find(CS);
   if (It != GetReferencedAssociatedTypesQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetReferencedAssociatedTypes(llvm::ArrayRef<ast::AssociatedTypeDecl*> &Result, ast::ConstraintSet* CS)

{
   auto Key = CS;
   auto It = GetReferencedAssociatedTypesQueries.find(Key);
   GetReferencedAssociatedTypesQuery *_Q;
   if (It == GetReferencedAssociatedTypesQueries.end()) {
      _Q = new(*this) GetReferencedAssociatedTypesQuery(*this, CS);
      GetReferencedAssociatedTypesQueries[_Q->CS] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetNeededAssociatedTypesQuery *QueryContext::getQuery<GetNeededAssociatedTypesQuery>(ast::RecordDecl* R)
{
   auto It = GetNeededAssociatedTypesQueries.find(R);
   if (It != GetNeededAssociatedTypesQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetNeededAssociatedTypes(llvm::ArrayRef<ast::AssociatedTypeDecl*> &Result, ast::RecordDecl* R)

{
   auto Key = R;
   auto It = GetNeededAssociatedTypesQueries.find(Key);
   GetNeededAssociatedTypesQuery *_Q;
   if (It == GetNeededAssociatedTypesQueries.end()) {
      _Q = new(*this) GetNeededAssociatedTypesQuery(*this, R);
      GetNeededAssociatedTypesQueries[_Q->R] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> CheckConformancesQuery *QueryContext::getQuery<CheckConformancesQuery>(QualType T, std::vector<ast::NamedDecl*> * ExtensionDecls)
{
   llvm::FoldingSetNodeID ID;
   CheckConformancesQuery::Profile(ID, T, ExtensionDecls);
   void *InsertPos;
   return CheckConformancesQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::CheckConformances(QualType T, std::vector<ast::NamedDecl*> * ExtensionDecls)

{
   llvm::FoldingSetNodeID ID;
   CheckConformancesQuery::Profile(ID, T, ExtensionDecls);
   void *InsertPos;
   auto *_Q = CheckConformancesQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) CheckConformancesQuery(*this, T, ExtensionDecls);
      CheckConformancesQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> CheckSingleConformanceQuery *QueryContext::getQuery<CheckSingleConformanceQuery>(QualType T, ast::ProtocolDecl* P)
{
   llvm::FoldingSetNodeID ID;
   CheckSingleConformanceQuery::Profile(ID, T, P);
   void *InsertPos;
   return CheckSingleConformanceQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::CheckSingleConformance(QualType T, ast::ProtocolDecl* P)

{
   llvm::FoldingSetNodeID ID;
   CheckSingleConformanceQuery::Profile(ID, T, P);
   void *InsertPos;
   auto *_Q = CheckSingleConformanceQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) CheckSingleConformanceQuery(*this, T, P);
      CheckSingleConformanceQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> CheckAssociatedTypeConstraintsQuery *QueryContext::getQuery<CheckAssociatedTypeConstraintsQuery>(ast::RecordDecl* R)
{
   auto It = CheckAssociatedTypeConstraintsQueries.find(R);
   if (It != CheckAssociatedTypeConstraintsQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::CheckAssociatedTypeConstraints(ast::RecordDecl* R)

{
   auto Key = R;
   auto It = CheckAssociatedTypeConstraintsQueries.find(Key);
   CheckAssociatedTypeConstraintsQuery *_Q;
   if (It == CheckAssociatedTypeConstraintsQueries.end()) {
      _Q = new(*this) CheckAssociatedTypeConstraintsQuery(*this, R);
      CheckAssociatedTypeConstraintsQueries[_Q->R] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckConstraintsQuery *QueryContext::getQuery<TypecheckConstraintsQuery>(ast::NamedDecl* ND)
{
   auto It = TypecheckConstraintsQueries.find(ND);
   if (It != TypecheckConstraintsQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckConstraints(ast::NamedDecl* ND)

{
   auto Key = ND;
   auto It = TypecheckConstraintsQueries.find(Key);
   TypecheckConstraintsQuery *_Q;
   if (It == TypecheckConstraintsQueries.end()) {
      _Q = new(*this) TypecheckConstraintsQuery(*this, ND);
      TypecheckConstraintsQueries[_Q->ND] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ConformsToQuery *QueryContext::getQuery<ConformsToQuery>(CanType T, ast::ProtocolDecl* P)
{
   llvm::FoldingSetNodeID ID;
   ConformsToQuery::Profile(ID, T, P);
   void *InsertPos;
   return ConformsToQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::ConformsTo(bool &Result, CanType T, ast::ProtocolDecl* P)

{
   llvm::FoldingSetNodeID ID;
   ConformsToQuery::Profile(ID, T, P);
   void *InsertPos;
   auto *_Q = ConformsToQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) ConformsToQuery(*this, T, P);
      ConformsToQueries.InsertNode(_Q, InsertPos);
   }   _Q->refresh();

#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> IsCovariantQuery *QueryContext::getQuery<IsCovariantQuery>(QualType T, QualType Covar)
{
   llvm::FoldingSetNodeID ID;
   IsCovariantQuery::Profile(ID, T, Covar);
   void *InsertPos;
   return IsCovariantQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::IsCovariant(bool &Result, QualType T, QualType Covar)

{
   llvm::FoldingSetNodeID ID;
   IsCovariantQuery::Profile(ID, T, Covar);
   void *InsertPos;
   auto *_Q = IsCovariantQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) IsCovariantQuery(*this, T, Covar);
      IsCovariantQueries.InsertNode(_Q, InsertPos);
   }   _Q->refresh();

#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> IsContravariantQuery *QueryContext::getQuery<IsContravariantQuery>(QualType T, QualType Contravar)
{
   llvm::FoldingSetNodeID ID;
   IsContravariantQuery::Profile(ID, T, Contravar);
   void *InsertPos;
   return IsContravariantQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::IsContravariant(bool &Result, QualType T, QualType Contravar)

{
   llvm::FoldingSetNodeID ID;
   IsContravariantQuery::Profile(ID, T, Contravar);
   void *InsertPos;
   auto *_Q = IsContravariantQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) IsContravariantQuery(*this, T, Contravar);
      IsContravariantQueries.InsertNode(_Q, InsertPos);
   }   _Q->refresh();

#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetTypeSizeQuery *QueryContext::getQuery<GetTypeSizeQuery>(CanType T)
{
   auto It = GetTypeSizeQueries.find(T);
   if (It != GetTypeSizeQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetTypeSize(unsigned &Result, CanType T)

{
   auto Key = T;
   auto It = GetTypeSizeQueries.find(Key);
   GetTypeSizeQuery *_Q;
   if (It == GetTypeSizeQueries.end()) {
      _Q = new(*this) GetTypeSizeQuery(*this, T);
      GetTypeSizeQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetTypeAlignmentQuery *QueryContext::getQuery<GetTypeAlignmentQuery>(CanType T)
{
   auto It = GetTypeAlignmentQueries.find(T);
   if (It != GetTypeAlignmentQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetTypeAlignment(unsigned short &Result, CanType T)

{
   auto Key = T;
   auto It = GetTypeAlignmentQueries.find(Key);
   GetTypeAlignmentQuery *_Q;
   if (It == GetTypeAlignmentQueries.end()) {
      _Q = new(*this) GetTypeAlignmentQuery(*this, T);
      GetTypeAlignmentQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetTypeStrideQuery *QueryContext::getQuery<GetTypeStrideQuery>(CanType T)
{
   auto It = GetTypeStrideQueries.find(T);
   if (It != GetTypeStrideQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetTypeStride(unsigned &Result, CanType T)

{
   auto Key = T;
   auto It = GetTypeStrideQueries.find(Key);
   GetTypeStrideQuery *_Q;
   if (It == GetTypeStrideQueries.end()) {
      _Q = new(*this) GetTypeStrideQuery(*this, T);
      GetTypeStrideQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> CalculateRecordSizeQuery *QueryContext::getQuery<CalculateRecordSizeQuery>(ast::RecordDecl* R)
{
   auto It = CalculateRecordSizeQueries.find(R);
   if (It != CalculateRecordSizeQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::CalculateRecordSize(unsigned &Result, ast::RecordDecl* R)

{
   auto Key = R;
   auto It = CalculateRecordSizeQueries.find(Key);
   CalculateRecordSizeQuery *_Q;
   if (It == CalculateRecordSizeQueries.end()) {
      _Q = new(*this) CalculateRecordSizeQuery(*this, R);
      CalculateRecordSizeQueries[_Q->R] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::GetRecordMeta(const RecordMetaInfo* &Result, ast::RecordDecl* R, uint8_t Initialize)

{
GetRecordMetaQuery _Q(*this, R, Initialize);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

Result = _Q.get();
return QueryResult(QueryResult::Success);
}

bool QueryContext::IsBuiltinIntegerType(ast::RecordDecl* R)

{
   auto Key = R;
   auto It = IsBuiltinIntegerTypeQueries.find(Key);
   if (It != IsBuiltinIntegerTypeQueries.end() && !shouldReset()) {
      return It->getSecond();   }
IsBuiltinIntegerTypeQuery _Q(*this, R);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) llvm_unreachable("infallible query failed!");

auto Result = _Q.get();
IsBuiltinIntegerTypeQueries[Key] = Result;
return Result;
}

bool QueryContext::IsBuiltinFloatingPointType(ast::RecordDecl* R)

{
   auto Key = R;
   auto It = IsBuiltinFloatingPointTypeQueries.find(Key);
   if (It != IsBuiltinFloatingPointTypeQueries.end() && !shouldReset()) {
      return It->getSecond();   }
IsBuiltinFloatingPointTypeQuery _Q(*this, R);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) llvm_unreachable("infallible query failed!");

auto Result = _Q.get();
IsBuiltinFloatingPointTypeQueries[Key] = Result;
return Result;
}

bool QueryContext::IsBuiltinBoolType(ast::RecordDecl* R)

{
   auto Key = R;
   auto It = IsBuiltinBoolTypeQueries.find(Key);
   if (It != IsBuiltinBoolTypeQueries.end() && !shouldReset()) {
      return It->getSecond();   }
IsBuiltinBoolTypeQuery _Q(*this, R);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) llvm_unreachable("infallible query failed!");

auto Result = _Q.get();
IsBuiltinBoolTypeQueries[Key] = Result;
return Result;
}

QueryResult QueryContext::AddImplicitConformance(ast::MethodDecl* &Result, ast::RecordDecl* R, ast::ImplicitConformanceKind K, ast::MethodDecl* Impl)

{
   auto Key = std::make_pair(R, std::make_pair(K, Impl));
   auto It = AddImplicitConformanceQueries.find(Key);
   if (It != AddImplicitConformanceQueries.end() && !shouldReset()) {
      Result = It->getSecond();      return QueryResult(QueryResult::Success);   }
AddImplicitConformanceQuery _Q(*this, R, K, Impl);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

Result = _Q.get();
AddImplicitConformanceQueries[Key] = Result;
return QueryResult(QueryResult::Success);
}

QueryResult QueryContext::GetImplicitConformance(ast::MethodDecl* &Result, ast::RecordDecl* R, ast::ImplicitConformanceKind K)

{
GetImplicitConformanceQuery _Q(*this, R, K);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

Result = _Q.get();
return QueryResult(QueryResult::Success);
}

template<> CheckBuiltinConformancesQuery *QueryContext::getQuery<CheckBuiltinConformancesQuery>(ast::RecordDecl* R)
{
   auto It = CheckBuiltinConformancesQueries.find(R);
   if (It != CheckBuiltinConformancesQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::CheckBuiltinConformances(ast::RecordDecl* R)

{
   auto Key = R;
   auto It = CheckBuiltinConformancesQueries.find(Key);
   CheckBuiltinConformancesQuery *_Q;
   if (It == CheckBuiltinConformancesQueries.end()) {
      _Q = new(*this) CheckBuiltinConformancesQuery(*this, R);
      CheckBuiltinConformancesQueries[_Q->R] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> IsAccessibleQuery *QueryContext::getQuery<IsAccessibleQuery>(ast::DeclContext* DC, ast::NamedDecl* ND)
{
   llvm::FoldingSetNodeID ID;
   IsAccessibleQuery::Profile(ID, DC, ND);
   void *InsertPos;
   return IsAccessibleQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::IsAccessible(bool &Result, ast::DeclContext* DC, ast::NamedDecl* ND)

{
   llvm::FoldingSetNodeID ID;
   IsAccessibleQuery::Profile(ID, DC, ND);
   void *InsertPos;
   auto *_Q = IsAccessibleQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) IsAccessibleQuery(*this, DC, ND);
      IsAccessibleQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> CheckAccessibilityQuery *QueryContext::getQuery<CheckAccessibilityQuery>(ast::DeclContext* DC, ast::NamedDecl* ND, SourceLocation Loc)
{
   llvm::FoldingSetNodeID ID;
   CheckAccessibilityQuery::Profile(ID, DC, ND, Loc);
   void *InsertPos;
   return CheckAccessibilityQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::CheckAccessibility(ast::DeclContext* DC, ast::NamedDecl* ND, SourceLocation Loc)

{
   llvm::FoldingSetNodeID ID;
   CheckAccessibilityQuery::Profile(ID, DC, ND, Loc);
   void *InsertPos;
   auto *_Q = CheckAccessibilityQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) CheckAccessibilityQuery(*this, DC, ND, Loc);
      CheckAccessibilityQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveNestedNameSpecToTypeQuery *QueryContext::getQuery<ResolveNestedNameSpecToTypeQuery>(NestedNameSpecifierWithLoc* Name, ast::DeclContext* DC, bool IssueDiag)
{
   llvm::FoldingSetNodeID ID;
   ResolveNestedNameSpecToTypeQuery::Profile(ID, Name, DC, IssueDiag);
   void *InsertPos;
   return ResolveNestedNameSpecToTypeQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::ResolveNestedNameSpecToType(QualType &Result, NestedNameSpecifierWithLoc* Name, ast::DeclContext* DC, bool IssueDiag)

{
   llvm::FoldingSetNodeID ID;
   ResolveNestedNameSpecToTypeQuery::Profile(ID, Name, DC, IssueDiag);
   void *InsertPos;
   auto *_Q = ResolveNestedNameSpecToTypeQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) ResolveNestedNameSpecToTypeQuery(*this, Name, DC, IssueDiag);
      ResolveNestedNameSpecToTypeQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveNestedNameSpecToDeclQuery *QueryContext::getQuery<ResolveNestedNameSpecToDeclQuery>(NestedNameSpecifierWithLoc* Name, ast::DeclContext* DC, bool IssueDiag)
{
   llvm::FoldingSetNodeID ID;
   ResolveNestedNameSpecToDeclQuery::Profile(ID, Name, DC, IssueDiag);
   void *InsertPos;
   return ResolveNestedNameSpecToDeclQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::ResolveNestedNameSpecToDecl(ast::NamedDecl* &Result, NestedNameSpecifierWithLoc* Name, ast::DeclContext* DC, bool IssueDiag)

{
   llvm::FoldingSetNodeID ID;
   ResolveNestedNameSpecToDeclQuery::Profile(ID, Name, DC, IssueDiag);
   void *InsertPos;
   auto *_Q = ResolveNestedNameSpecToDeclQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) ResolveNestedNameSpecToDeclQuery(*this, Name, DC, IssueDiag);
      ResolveNestedNameSpecToDeclQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypeCheckDeclContextQuery *QueryContext::getQuery<TypeCheckDeclContextQuery>(ast::DeclContext* DC)
{
   auto It = TypeCheckDeclContextQueries.find(DC);
   if (It != TypeCheckDeclContextQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypeCheckDeclContext(ast::DeclContext* DC)

{
   auto Key = DC;
   auto It = TypeCheckDeclContextQueries.find(Key);
   TypeCheckDeclContextQuery *_Q;
   if (It == TypeCheckDeclContextQueries.end()) {
      _Q = new(*this) TypeCheckDeclContextQuery(*this, DC);
      TypeCheckDeclContextQueries[_Q->DC] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::PrepareDeclInterface(ast::Decl* D)

{
   auto Key = D;
   auto It = PrepareDeclInterfaceQueries.find(Key);
   if (It != PrepareDeclInterfaceQueries.end() && !shouldReset()) {
      return QueryResult(QueryResult::Success);   }
PrepareDeclInterfaceQuery _Q(*this, D);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

PrepareDeclInterfaceQueries.insert(D);
return QueryResult(QueryResult::Success);
}

QueryResult QueryContext::TypecheckDecl(ast::Decl* D)

{
   auto Key = D;
   auto It = TypecheckDeclQueries.find(Key);
   if (It != TypecheckDeclQueries.end() && !shouldReset()) {
      return QueryResult(QueryResult::Success);   }
TypecheckDeclQuery _Q(*this, D);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

TypecheckDeclQueries.insert(D);
return QueryResult(QueryResult::Success);
}

template<> PreparePrecedenceGroupInterfaceQuery *QueryContext::getQuery<PreparePrecedenceGroupInterfaceQuery>(ast::PrecedenceGroupDecl* D)
{
   auto It = PreparePrecedenceGroupInterfaceQueries.find(D);
   if (It != PreparePrecedenceGroupInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PreparePrecedenceGroupInterface(ast::PrecedenceGroupDecl* D)

{
   auto Key = D;
   auto It = PreparePrecedenceGroupInterfaceQueries.find(Key);
   PreparePrecedenceGroupInterfaceQuery *_Q;
   if (It == PreparePrecedenceGroupInterfaceQueries.end()) {
      _Q = new(*this) PreparePrecedenceGroupInterfaceQuery(*this, D);
      PreparePrecedenceGroupInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckPrecedenceGroupQuery *QueryContext::getQuery<TypecheckPrecedenceGroupQuery>(ast::PrecedenceGroupDecl* D)
{
   auto It = TypecheckPrecedenceGroupQueries.find(D);
   if (It != TypecheckPrecedenceGroupQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckPrecedenceGroup(ast::PrecedenceGroupDecl* D)

{
   auto Key = D;
   auto It = TypecheckPrecedenceGroupQueries.find(Key);
   TypecheckPrecedenceGroupQuery *_Q;
   if (It == TypecheckPrecedenceGroupQueries.end()) {
      _Q = new(*this) TypecheckPrecedenceGroupQuery(*this, D);
      TypecheckPrecedenceGroupQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareOperatorInterfaceQuery *QueryContext::getQuery<PrepareOperatorInterfaceQuery>(ast::OperatorDecl* D)
{
   auto It = PrepareOperatorInterfaceQueries.find(D);
   if (It != PrepareOperatorInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareOperatorInterface(ast::OperatorDecl* D)

{
   auto Key = D;
   auto It = PrepareOperatorInterfaceQueries.find(Key);
   PrepareOperatorInterfaceQuery *_Q;
   if (It == PrepareOperatorInterfaceQueries.end()) {
      _Q = new(*this) PrepareOperatorInterfaceQuery(*this, D);
      PrepareOperatorInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckOperatorQuery *QueryContext::getQuery<TypecheckOperatorQuery>(ast::OperatorDecl* D)
{
   auto It = TypecheckOperatorQueries.find(D);
   if (It != TypecheckOperatorQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckOperator(ast::OperatorDecl* D)

{
   auto Key = D;
   auto It = TypecheckOperatorQueries.find(Key);
   TypecheckOperatorQuery *_Q;
   if (It == TypecheckOperatorQueries.end()) {
      _Q = new(*this) TypecheckOperatorQuery(*this, D);
      TypecheckOperatorQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareGlobalVarInterfaceQuery *QueryContext::getQuery<PrepareGlobalVarInterfaceQuery>(ast::GlobalVarDecl* D)
{
   auto It = PrepareGlobalVarInterfaceQueries.find(D);
   if (It != PrepareGlobalVarInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareGlobalVarInterface(ast::GlobalVarDecl* D)

{
   auto Key = D;
   auto It = PrepareGlobalVarInterfaceQueries.find(Key);
   PrepareGlobalVarInterfaceQuery *_Q;
   if (It == PrepareGlobalVarInterfaceQueries.end()) {
      _Q = new(*this) PrepareGlobalVarInterfaceQuery(*this, D);
      PrepareGlobalVarInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckGlobalVarQuery *QueryContext::getQuery<TypecheckGlobalVarQuery>(ast::GlobalVarDecl* D)
{
   auto It = TypecheckGlobalVarQueries.find(D);
   if (It != TypecheckGlobalVarQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckGlobalVar(ast::GlobalVarDecl* D)

{
   auto Key = D;
   auto It = TypecheckGlobalVarQueries.find(Key);
   TypecheckGlobalVarQuery *_Q;
   if (It == TypecheckGlobalVarQueries.end()) {
      _Q = new(*this) TypecheckGlobalVarQuery(*this, D);
      TypecheckGlobalVarQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareCallableInterfaceQuery *QueryContext::getQuery<PrepareCallableInterfaceQuery>(ast::CallableDecl* D)
{
   auto It = PrepareCallableInterfaceQueries.find(D);
   if (It != PrepareCallableInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareCallableInterface(ast::CallableDecl* D)

{
   auto Key = D;
   auto It = PrepareCallableInterfaceQueries.find(Key);
   PrepareCallableInterfaceQuery *_Q;
   if (It == PrepareCallableInterfaceQueries.end()) {
      _Q = new(*this) PrepareCallableInterfaceQuery(*this, D);
      PrepareCallableInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckCallableQuery *QueryContext::getQuery<TypecheckCallableQuery>(ast::CallableDecl* D)
{
   auto It = TypecheckCallableQueries.find(D);
   if (It != TypecheckCallableQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckCallable(ast::CallableDecl* D)

{
   auto Key = D;
   auto It = TypecheckCallableQueries.find(Key);
   TypecheckCallableQuery *_Q;
   if (It == TypecheckCallableQueries.end()) {
      _Q = new(*this) TypecheckCallableQuery(*this, D);
      TypecheckCallableQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareFunctionInterfaceQuery *QueryContext::getQuery<PrepareFunctionInterfaceQuery>(ast::FunctionDecl* D)
{
   auto It = PrepareFunctionInterfaceQueries.find(D);
   if (It != PrepareFunctionInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareFunctionInterface(ast::FunctionDecl* D)

{
   auto Key = D;
   auto It = PrepareFunctionInterfaceQueries.find(Key);
   PrepareFunctionInterfaceQuery *_Q;
   if (It == PrepareFunctionInterfaceQueries.end()) {
      _Q = new(*this) PrepareFunctionInterfaceQuery(*this, D);
      PrepareFunctionInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckFunctionQuery *QueryContext::getQuery<TypecheckFunctionQuery>(ast::FunctionDecl* D)
{
   auto It = TypecheckFunctionQueries.find(D);
   if (It != TypecheckFunctionQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckFunction(ast::FunctionDecl* D)

{
   auto Key = D;
   auto It = TypecheckFunctionQueries.find(Key);
   TypecheckFunctionQuery *_Q;
   if (It == TypecheckFunctionQueries.end()) {
      _Q = new(*this) TypecheckFunctionQuery(*this, D);
      TypecheckFunctionQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareMethodInterfaceQuery *QueryContext::getQuery<PrepareMethodInterfaceQuery>(ast::MethodDecl* D)
{
   auto It = PrepareMethodInterfaceQueries.find(D);
   if (It != PrepareMethodInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareMethodInterface(ast::MethodDecl* D)

{
   auto Key = D;
   auto It = PrepareMethodInterfaceQueries.find(Key);
   PrepareMethodInterfaceQuery *_Q;
   if (It == PrepareMethodInterfaceQueries.end()) {
      _Q = new(*this) PrepareMethodInterfaceQuery(*this, D);
      PrepareMethodInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckMethodQuery *QueryContext::getQuery<TypecheckMethodQuery>(ast::MethodDecl* D)
{
   auto It = TypecheckMethodQueries.find(D);
   if (It != TypecheckMethodQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckMethod(ast::MethodDecl* D)

{
   auto Key = D;
   auto It = TypecheckMethodQueries.find(Key);
   TypecheckMethodQuery *_Q;
   if (It == TypecheckMethodQueries.end()) {
      _Q = new(*this) TypecheckMethodQuery(*this, D);
      TypecheckMethodQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareInitInterfaceQuery *QueryContext::getQuery<PrepareInitInterfaceQuery>(ast::InitDecl* D)
{
   auto It = PrepareInitInterfaceQueries.find(D);
   if (It != PrepareInitInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareInitInterface(ast::InitDecl* D)

{
   auto Key = D;
   auto It = PrepareInitInterfaceQueries.find(Key);
   PrepareInitInterfaceQuery *_Q;
   if (It == PrepareInitInterfaceQueries.end()) {
      _Q = new(*this) PrepareInitInterfaceQuery(*this, D);
      PrepareInitInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckInitQuery *QueryContext::getQuery<TypecheckInitQuery>(ast::InitDecl* D)
{
   auto It = TypecheckInitQueries.find(D);
   if (It != TypecheckInitQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckInit(ast::InitDecl* D)

{
   auto Key = D;
   auto It = TypecheckInitQueries.find(Key);
   TypecheckInitQuery *_Q;
   if (It == TypecheckInitQueries.end()) {
      _Q = new(*this) TypecheckInitQuery(*this, D);
      TypecheckInitQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareDeinitInterfaceQuery *QueryContext::getQuery<PrepareDeinitInterfaceQuery>(ast::DeinitDecl* D)
{
   auto It = PrepareDeinitInterfaceQueries.find(D);
   if (It != PrepareDeinitInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareDeinitInterface(ast::DeinitDecl* D)

{
   auto Key = D;
   auto It = PrepareDeinitInterfaceQueries.find(Key);
   PrepareDeinitInterfaceQuery *_Q;
   if (It == PrepareDeinitInterfaceQueries.end()) {
      _Q = new(*this) PrepareDeinitInterfaceQuery(*this, D);
      PrepareDeinitInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckDeinitQuery *QueryContext::getQuery<TypecheckDeinitQuery>(ast::DeinitDecl* D)
{
   auto It = TypecheckDeinitQueries.find(D);
   if (It != TypecheckDeinitQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckDeinit(ast::DeinitDecl* D)

{
   auto Key = D;
   auto It = TypecheckDeinitQueries.find(Key);
   TypecheckDeinitQuery *_Q;
   if (It == TypecheckDeinitQueries.end()) {
      _Q = new(*this) TypecheckDeinitQuery(*this, D);
      TypecheckDeinitQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareFuncArgInterfaceQuery *QueryContext::getQuery<PrepareFuncArgInterfaceQuery>(ast::FuncArgDecl* D)
{
   auto It = PrepareFuncArgInterfaceQueries.find(D);
   if (It != PrepareFuncArgInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareFuncArgInterface(ast::FuncArgDecl* D)

{
   auto Key = D;
   auto It = PrepareFuncArgInterfaceQueries.find(Key);
   PrepareFuncArgInterfaceQuery *_Q;
   if (It == PrepareFuncArgInterfaceQueries.end()) {
      _Q = new(*this) PrepareFuncArgInterfaceQuery(*this, D);
      PrepareFuncArgInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckFuncArgQuery *QueryContext::getQuery<TypecheckFuncArgQuery>(ast::FuncArgDecl* D)
{
   auto It = TypecheckFuncArgQueries.find(D);
   if (It != TypecheckFuncArgQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckFuncArg(ast::FuncArgDecl* D)

{
   auto Key = D;
   auto It = TypecheckFuncArgQueries.find(Key);
   TypecheckFuncArgQuery *_Q;
   if (It == TypecheckFuncArgQueries.end()) {
      _Q = new(*this) TypecheckFuncArgQuery(*this, D);
      TypecheckFuncArgQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareTemplateParamInterfaceQuery *QueryContext::getQuery<PrepareTemplateParamInterfaceQuery>(ast::TemplateParamDecl* D)
{
   auto It = PrepareTemplateParamInterfaceQueries.find(D);
   if (It != PrepareTemplateParamInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareTemplateParamInterface(ast::TemplateParamDecl* D)

{
   auto Key = D;
   auto It = PrepareTemplateParamInterfaceQueries.find(Key);
   PrepareTemplateParamInterfaceQuery *_Q;
   if (It == PrepareTemplateParamInterfaceQueries.end()) {
      _Q = new(*this) PrepareTemplateParamInterfaceQuery(*this, D);
      PrepareTemplateParamInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckTemplateParamQuery *QueryContext::getQuery<TypecheckTemplateParamQuery>(ast::TemplateParamDecl* D)
{
   auto It = TypecheckTemplateParamQueries.find(D);
   if (It != TypecheckTemplateParamQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckTemplateParam(ast::TemplateParamDecl* D)

{
   auto Key = D;
   auto It = TypecheckTemplateParamQueries.find(Key);
   TypecheckTemplateParamQuery *_Q;
   if (It == TypecheckTemplateParamQueries.end()) {
      _Q = new(*this) TypecheckTemplateParamQuery(*this, D);
      TypecheckTemplateParamQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareAssociatedTypeInterfaceQuery *QueryContext::getQuery<PrepareAssociatedTypeInterfaceQuery>(ast::AssociatedTypeDecl* D)
{
   auto It = PrepareAssociatedTypeInterfaceQueries.find(D);
   if (It != PrepareAssociatedTypeInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareAssociatedTypeInterface(ast::AssociatedTypeDecl* D)

{
   auto Key = D;
   auto It = PrepareAssociatedTypeInterfaceQueries.find(Key);
   PrepareAssociatedTypeInterfaceQuery *_Q;
   if (It == PrepareAssociatedTypeInterfaceQueries.end()) {
      _Q = new(*this) PrepareAssociatedTypeInterfaceQuery(*this, D);
      PrepareAssociatedTypeInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckAssociatedTypeQuery *QueryContext::getQuery<TypecheckAssociatedTypeQuery>(ast::AssociatedTypeDecl* D)
{
   auto It = TypecheckAssociatedTypeQueries.find(D);
   if (It != TypecheckAssociatedTypeQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckAssociatedType(ast::AssociatedTypeDecl* D)

{
   auto Key = D;
   auto It = TypecheckAssociatedTypeQueries.find(Key);
   TypecheckAssociatedTypeQuery *_Q;
   if (It == TypecheckAssociatedTypeQueries.end()) {
      _Q = new(*this) TypecheckAssociatedTypeQuery(*this, D);
      TypecheckAssociatedTypeQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareAliasInterfaceQuery *QueryContext::getQuery<PrepareAliasInterfaceQuery>(ast::AliasDecl* D)
{
   auto It = PrepareAliasInterfaceQueries.find(D);
   if (It != PrepareAliasInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareAliasInterface(ast::AliasDecl* D)

{
   auto Key = D;
   auto It = PrepareAliasInterfaceQueries.find(Key);
   PrepareAliasInterfaceQuery *_Q;
   if (It == PrepareAliasInterfaceQueries.end()) {
      _Q = new(*this) PrepareAliasInterfaceQuery(*this, D);
      PrepareAliasInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckAliasQuery *QueryContext::getQuery<TypecheckAliasQuery>(ast::AliasDecl* D)
{
   auto It = TypecheckAliasQueries.find(D);
   if (It != TypecheckAliasQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckAlias(ast::AliasDecl* D)

{
   auto Key = D;
   auto It = TypecheckAliasQueries.find(Key);
   TypecheckAliasQuery *_Q;
   if (It == TypecheckAliasQueries.end()) {
      _Q = new(*this) TypecheckAliasQuery(*this, D);
      TypecheckAliasQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareRecordInterfaceQuery *QueryContext::getQuery<PrepareRecordInterfaceQuery>(ast::RecordDecl* D)
{
   auto It = PrepareRecordInterfaceQueries.find(D);
   if (It != PrepareRecordInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareRecordInterface(ast::RecordDecl* D)

{
   auto Key = D;
   auto It = PrepareRecordInterfaceQueries.find(Key);
   PrepareRecordInterfaceQuery *_Q;
   if (It == PrepareRecordInterfaceQueries.end()) {
      _Q = new(*this) PrepareRecordInterfaceQuery(*this, D);
      PrepareRecordInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckRecordQuery *QueryContext::getQuery<TypecheckRecordQuery>(ast::RecordDecl* D)
{
   auto It = TypecheckRecordQueries.find(D);
   if (It != TypecheckRecordQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckRecord(ast::RecordDecl* D)

{
   auto Key = D;
   auto It = TypecheckRecordQueries.find(Key);
   TypecheckRecordQuery *_Q;
   if (It == TypecheckRecordQueries.end()) {
      _Q = new(*this) TypecheckRecordQuery(*this, D);
      TypecheckRecordQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareStructInterfaceQuery *QueryContext::getQuery<PrepareStructInterfaceQuery>(ast::StructDecl* D)
{
   auto It = PrepareStructInterfaceQueries.find(D);
   if (It != PrepareStructInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareStructInterface(ast::StructDecl* D)

{
   auto Key = D;
   auto It = PrepareStructInterfaceQueries.find(Key);
   PrepareStructInterfaceQuery *_Q;
   if (It == PrepareStructInterfaceQueries.end()) {
      _Q = new(*this) PrepareStructInterfaceQuery(*this, D);
      PrepareStructInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckStructQuery *QueryContext::getQuery<TypecheckStructQuery>(ast::StructDecl* D)
{
   auto It = TypecheckStructQueries.find(D);
   if (It != TypecheckStructQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckStruct(ast::StructDecl* D)

{
   auto Key = D;
   auto It = TypecheckStructQueries.find(Key);
   TypecheckStructQuery *_Q;
   if (It == TypecheckStructQueries.end()) {
      _Q = new(*this) TypecheckStructQuery(*this, D);
      TypecheckStructQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareClassInterfaceQuery *QueryContext::getQuery<PrepareClassInterfaceQuery>(ast::ClassDecl* D)
{
   auto It = PrepareClassInterfaceQueries.find(D);
   if (It != PrepareClassInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareClassInterface(ast::ClassDecl* D)

{
   auto Key = D;
   auto It = PrepareClassInterfaceQueries.find(Key);
   PrepareClassInterfaceQuery *_Q;
   if (It == PrepareClassInterfaceQueries.end()) {
      _Q = new(*this) PrepareClassInterfaceQuery(*this, D);
      PrepareClassInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckClassQuery *QueryContext::getQuery<TypecheckClassQuery>(ast::ClassDecl* D)
{
   auto It = TypecheckClassQueries.find(D);
   if (It != TypecheckClassQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckClass(ast::ClassDecl* D)

{
   auto Key = D;
   auto It = TypecheckClassQueries.find(Key);
   TypecheckClassQuery *_Q;
   if (It == TypecheckClassQueries.end()) {
      _Q = new(*this) TypecheckClassQuery(*this, D);
      TypecheckClassQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareEnumInterfaceQuery *QueryContext::getQuery<PrepareEnumInterfaceQuery>(ast::EnumDecl* D)
{
   auto It = PrepareEnumInterfaceQueries.find(D);
   if (It != PrepareEnumInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareEnumInterface(ast::EnumDecl* D)

{
   auto Key = D;
   auto It = PrepareEnumInterfaceQueries.find(Key);
   PrepareEnumInterfaceQuery *_Q;
   if (It == PrepareEnumInterfaceQueries.end()) {
      _Q = new(*this) PrepareEnumInterfaceQuery(*this, D);
      PrepareEnumInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckEnumQuery *QueryContext::getQuery<TypecheckEnumQuery>(ast::EnumDecl* D)
{
   auto It = TypecheckEnumQueries.find(D);
   if (It != TypecheckEnumQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckEnum(ast::EnumDecl* D)

{
   auto Key = D;
   auto It = TypecheckEnumQueries.find(Key);
   TypecheckEnumQuery *_Q;
   if (It == TypecheckEnumQueries.end()) {
      _Q = new(*this) TypecheckEnumQuery(*this, D);
      TypecheckEnumQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareProtocolInterfaceQuery *QueryContext::getQuery<PrepareProtocolInterfaceQuery>(ast::ProtocolDecl* D)
{
   auto It = PrepareProtocolInterfaceQueries.find(D);
   if (It != PrepareProtocolInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareProtocolInterface(ast::ProtocolDecl* D)

{
   auto Key = D;
   auto It = PrepareProtocolInterfaceQueries.find(Key);
   PrepareProtocolInterfaceQuery *_Q;
   if (It == PrepareProtocolInterfaceQueries.end()) {
      _Q = new(*this) PrepareProtocolInterfaceQuery(*this, D);
      PrepareProtocolInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckProtocolQuery *QueryContext::getQuery<TypecheckProtocolQuery>(ast::ProtocolDecl* D)
{
   auto It = TypecheckProtocolQueries.find(D);
   if (It != TypecheckProtocolQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckProtocol(ast::ProtocolDecl* D)

{
   auto Key = D;
   auto It = TypecheckProtocolQueries.find(Key);
   TypecheckProtocolQuery *_Q;
   if (It == TypecheckProtocolQueries.end()) {
      _Q = new(*this) TypecheckProtocolQuery(*this, D);
      TypecheckProtocolQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareExtensionInterfaceQuery *QueryContext::getQuery<PrepareExtensionInterfaceQuery>(ast::ExtensionDecl* D)
{
   auto It = PrepareExtensionInterfaceQueries.find(D);
   if (It != PrepareExtensionInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareExtensionInterface(ast::ExtensionDecl* D)

{
   auto Key = D;
   auto It = PrepareExtensionInterfaceQueries.find(Key);
   PrepareExtensionInterfaceQuery *_Q;
   if (It == PrepareExtensionInterfaceQueries.end()) {
      _Q = new(*this) PrepareExtensionInterfaceQuery(*this, D);
      PrepareExtensionInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckExtensionQuery *QueryContext::getQuery<TypecheckExtensionQuery>(ast::ExtensionDecl* D)
{
   auto It = TypecheckExtensionQueries.find(D);
   if (It != TypecheckExtensionQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckExtension(ast::ExtensionDecl* D)

{
   auto Key = D;
   auto It = TypecheckExtensionQueries.find(Key);
   TypecheckExtensionQuery *_Q;
   if (It == TypecheckExtensionQueries.end()) {
      _Q = new(*this) TypecheckExtensionQuery(*this, D);
      TypecheckExtensionQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareFieldInterfaceQuery *QueryContext::getQuery<PrepareFieldInterfaceQuery>(ast::FieldDecl* D)
{
   auto It = PrepareFieldInterfaceQueries.find(D);
   if (It != PrepareFieldInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareFieldInterface(ast::FieldDecl* D)

{
   auto Key = D;
   auto It = PrepareFieldInterfaceQueries.find(Key);
   PrepareFieldInterfaceQuery *_Q;
   if (It == PrepareFieldInterfaceQueries.end()) {
      _Q = new(*this) PrepareFieldInterfaceQuery(*this, D);
      PrepareFieldInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckFieldQuery *QueryContext::getQuery<TypecheckFieldQuery>(ast::FieldDecl* D)
{
   auto It = TypecheckFieldQueries.find(D);
   if (It != TypecheckFieldQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckField(ast::FieldDecl* D)

{
   auto Key = D;
   auto It = TypecheckFieldQueries.find(Key);
   TypecheckFieldQuery *_Q;
   if (It == TypecheckFieldQueries.end()) {
      _Q = new(*this) TypecheckFieldQuery(*this, D);
      TypecheckFieldQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PreparePropInterfaceQuery *QueryContext::getQuery<PreparePropInterfaceQuery>(ast::PropDecl* D)
{
   auto It = PreparePropInterfaceQueries.find(D);
   if (It != PreparePropInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PreparePropInterface(ast::PropDecl* D)

{
   auto Key = D;
   auto It = PreparePropInterfaceQueries.find(Key);
   PreparePropInterfaceQuery *_Q;
   if (It == PreparePropInterfaceQueries.end()) {
      _Q = new(*this) PreparePropInterfaceQuery(*this, D);
      PreparePropInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckPropQuery *QueryContext::getQuery<TypecheckPropQuery>(ast::PropDecl* D)
{
   auto It = TypecheckPropQueries.find(D);
   if (It != TypecheckPropQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckProp(ast::PropDecl* D)

{
   auto Key = D;
   auto It = TypecheckPropQueries.find(Key);
   TypecheckPropQuery *_Q;
   if (It == TypecheckPropQueries.end()) {
      _Q = new(*this) TypecheckPropQuery(*this, D);
      TypecheckPropQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareSubscriptInterfaceQuery *QueryContext::getQuery<PrepareSubscriptInterfaceQuery>(ast::SubscriptDecl* D)
{
   auto It = PrepareSubscriptInterfaceQueries.find(D);
   if (It != PrepareSubscriptInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareSubscriptInterface(ast::SubscriptDecl* D)

{
   auto Key = D;
   auto It = PrepareSubscriptInterfaceQueries.find(Key);
   PrepareSubscriptInterfaceQuery *_Q;
   if (It == PrepareSubscriptInterfaceQueries.end()) {
      _Q = new(*this) PrepareSubscriptInterfaceQuery(*this, D);
      PrepareSubscriptInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckSubscriptQuery *QueryContext::getQuery<TypecheckSubscriptQuery>(ast::SubscriptDecl* D)
{
   auto It = TypecheckSubscriptQueries.find(D);
   if (It != TypecheckSubscriptQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckSubscript(ast::SubscriptDecl* D)

{
   auto Key = D;
   auto It = TypecheckSubscriptQueries.find(Key);
   TypecheckSubscriptQuery *_Q;
   if (It == TypecheckSubscriptQueries.end()) {
      _Q = new(*this) TypecheckSubscriptQuery(*this, D);
      TypecheckSubscriptQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareEnumCaseInterfaceQuery *QueryContext::getQuery<PrepareEnumCaseInterfaceQuery>(ast::EnumCaseDecl* D)
{
   auto It = PrepareEnumCaseInterfaceQueries.find(D);
   if (It != PrepareEnumCaseInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareEnumCaseInterface(ast::EnumCaseDecl* D)

{
   auto Key = D;
   auto It = PrepareEnumCaseInterfaceQueries.find(Key);
   PrepareEnumCaseInterfaceQuery *_Q;
   if (It == PrepareEnumCaseInterfaceQueries.end()) {
      _Q = new(*this) PrepareEnumCaseInterfaceQuery(*this, D);
      PrepareEnumCaseInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckEnumCaseQuery *QueryContext::getQuery<TypecheckEnumCaseQuery>(ast::EnumCaseDecl* D)
{
   auto It = TypecheckEnumCaseQueries.find(D);
   if (It != TypecheckEnumCaseQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckEnumCase(ast::EnumCaseDecl* D)

{
   auto Key = D;
   auto It = TypecheckEnumCaseQueries.find(Key);
   TypecheckEnumCaseQuery *_Q;
   if (It == TypecheckEnumCaseQueries.end()) {
      _Q = new(*this) TypecheckEnumCaseQuery(*this, D);
      TypecheckEnumCaseQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareNamespaceInterfaceQuery *QueryContext::getQuery<PrepareNamespaceInterfaceQuery>(ast::NamespaceDecl* D)
{
   auto It = PrepareNamespaceInterfaceQueries.find(D);
   if (It != PrepareNamespaceInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareNamespaceInterface(ast::NamespaceDecl* D)

{
   auto Key = D;
   auto It = PrepareNamespaceInterfaceQueries.find(Key);
   PrepareNamespaceInterfaceQuery *_Q;
   if (It == PrepareNamespaceInterfaceQueries.end()) {
      _Q = new(*this) PrepareNamespaceInterfaceQuery(*this, D);
      PrepareNamespaceInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckNamespaceQuery *QueryContext::getQuery<TypecheckNamespaceQuery>(ast::NamespaceDecl* D)
{
   auto It = TypecheckNamespaceQueries.find(D);
   if (It != TypecheckNamespaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckNamespace(ast::NamespaceDecl* D)

{
   auto Key = D;
   auto It = TypecheckNamespaceQueries.find(Key);
   TypecheckNamespaceQuery *_Q;
   if (It == TypecheckNamespaceQueries.end()) {
      _Q = new(*this) TypecheckNamespaceQuery(*this, D);
      TypecheckNamespaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareCompoundInterfaceQuery *QueryContext::getQuery<PrepareCompoundInterfaceQuery>(ast::CompoundDecl* D)
{
   auto It = PrepareCompoundInterfaceQueries.find(D);
   if (It != PrepareCompoundInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareCompoundInterface(ast::CompoundDecl* D)

{
   auto Key = D;
   auto It = PrepareCompoundInterfaceQueries.find(Key);
   PrepareCompoundInterfaceQuery *_Q;
   if (It == PrepareCompoundInterfaceQueries.end()) {
      _Q = new(*this) PrepareCompoundInterfaceQuery(*this, D);
      PrepareCompoundInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckCompoundQuery *QueryContext::getQuery<TypecheckCompoundQuery>(ast::CompoundDecl* D)
{
   auto It = TypecheckCompoundQueries.find(D);
   if (It != TypecheckCompoundQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckCompound(ast::CompoundDecl* D)

{
   auto Key = D;
   auto It = TypecheckCompoundQueries.find(Key);
   TypecheckCompoundQuery *_Q;
   if (It == TypecheckCompoundQueries.end()) {
      _Q = new(*this) TypecheckCompoundQuery(*this, D);
      TypecheckCompoundQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareModuleInterfaceQuery *QueryContext::getQuery<PrepareModuleInterfaceQuery>(ast::ModuleDecl* D)
{
   auto It = PrepareModuleInterfaceQueries.find(D);
   if (It != PrepareModuleInterfaceQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareModuleInterface(ast::ModuleDecl* D)

{
   auto Key = D;
   auto It = PrepareModuleInterfaceQueries.find(Key);
   PrepareModuleInterfaceQuery *_Q;
   if (It == PrepareModuleInterfaceQueries.end()) {
      _Q = new(*this) PrepareModuleInterfaceQuery(*this, D);
      PrepareModuleInterfaceQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> TypecheckModuleQuery *QueryContext::getQuery<TypecheckModuleQuery>(ast::ModuleDecl* D)
{
   auto It = TypecheckModuleQueries.find(D);
   if (It != TypecheckModuleQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::TypecheckModule(ast::ModuleDecl* D)

{
   auto Key = D;
   auto It = TypecheckModuleQueries.find(Key);
   TypecheckModuleQuery *_Q;
   if (It == TypecheckModuleQueries.end()) {
      _Q = new(*this) TypecheckModuleQuery(*this, D);
      TypecheckModuleQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> PrepareTemplateParametersQuery *QueryContext::getQuery<PrepareTemplateParametersQuery>(ast::NamedDecl* Decl)
{
   auto It = PrepareTemplateParametersQueries.find(Decl);
   if (It != PrepareTemplateParametersQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::PrepareTemplateParameters(ast::NamedDecl* Decl)

{
   auto Key = Decl;
   auto It = PrepareTemplateParametersQueries.find(Key);
   PrepareTemplateParametersQuery *_Q;
   if (It == PrepareTemplateParametersQueries.end()) {
      _Q = new(*this) PrepareTemplateParametersQuery(*this, Decl);
      PrepareTemplateParametersQueries[_Q->Decl] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> DeclareImplicitInitializersQuery *QueryContext::getQuery<DeclareImplicitInitializersQuery>(ast::RecordDecl* R)
{
   auto It = DeclareImplicitInitializersQueries.find(R);
   if (It != DeclareImplicitInitializersQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::DeclareImplicitInitializers(ast::RecordDecl* R)

{
   auto Key = R;
   auto It = DeclareImplicitInitializersQueries.find(Key);
   DeclareImplicitInitializersQuery *_Q;
   if (It == DeclareImplicitInitializersQueries.end()) {
      _Q = new(*this) DeclareImplicitInitializersQuery(*this, R);
      DeclareImplicitInitializersQueries[_Q->R] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> DeclareImplicitDefaultInitQuery *QueryContext::getQuery<DeclareImplicitDefaultInitQuery>(ast::StructDecl* S)
{
   auto It = DeclareImplicitDefaultInitQueries.find(S);
   if (It != DeclareImplicitDefaultInitQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::DeclareImplicitDefaultInit(ast::StructDecl* S)

{
   auto Key = S;
   auto It = DeclareImplicitDefaultInitQueries.find(Key);
   DeclareImplicitDefaultInitQuery *_Q;
   if (It == DeclareImplicitDefaultInitQueries.end()) {
      _Q = new(*this) DeclareImplicitDefaultInitQuery(*this, S);
      DeclareImplicitDefaultInitQueries[_Q->S] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> DeclareMemberwiseInitQuery *QueryContext::getQuery<DeclareMemberwiseInitQuery>(ast::StructDecl* S)
{
   auto It = DeclareMemberwiseInitQueries.find(S);
   if (It != DeclareMemberwiseInitQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::DeclareMemberwiseInit(ast::StructDecl* S)

{
   auto Key = S;
   auto It = DeclareMemberwiseInitQueries.find(Key);
   DeclareMemberwiseInitQuery *_Q;
   if (It == DeclareMemberwiseInitQueries.end()) {
      _Q = new(*this) DeclareMemberwiseInitQuery(*this, S);
      DeclareMemberwiseInitQueries[_Q->S] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> DeclareImplicitDefaultDeinitQuery *QueryContext::getQuery<DeclareImplicitDefaultDeinitQuery>(ast::RecordDecl* S)
{
   auto It = DeclareImplicitDefaultDeinitQueries.find(S);
   if (It != DeclareImplicitDefaultDeinitQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::DeclareImplicitDefaultDeinit(ast::RecordDecl* S)

{
   auto Key = S;
   auto It = DeclareImplicitDefaultDeinitQueries.find(Key);
   DeclareImplicitDefaultDeinitQuery *_Q;
   if (It == DeclareImplicitDefaultDeinitQueries.end()) {
      _Q = new(*this) DeclareImplicitDefaultDeinitQuery(*this, S);
      DeclareImplicitDefaultDeinitQueries[_Q->S] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> ResolveRawTypeQuery *QueryContext::getQuery<ResolveRawTypeQuery>(ast::EnumDecl* E)
{
   auto It = ResolveRawTypeQueries.find(E);
   if (It != ResolveRawTypeQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::ResolveRawType(ast::EnumDecl* E)

{
   auto Key = E;
   auto It = ResolveRawTypeQueries.find(Key);
   ResolveRawTypeQuery *_Q;
   if (It == ResolveRawTypeQueries.end()) {
      _Q = new(*this) ResolveRawTypeQuery(*this, E);
      ResolveRawTypeQueries[_Q->E] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> AssignInitNameQuery *QueryContext::getQuery<AssignInitNameQuery>(ast::InitDecl* Init)
{
   auto It = AssignInitNameQueries.find(Init);
   if (It != AssignInitNameQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::AssignInitName(ast::InitDecl* Init)

{
   auto Key = Init;
   auto It = AssignInitNameQueries.find(Key);
   AssignInitNameQuery *_Q;
   if (It == AssignInitNameQueries.end()) {
      _Q = new(*this) AssignInitNameQuery(*this, Init);
      AssignInitNameQueries[_Q->Init] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::CreateBaseInit(ast::InitDecl* D)

{
   auto Key = D;
   auto It = CreateBaseInitQueries.find(Key);
   if (It != CreateBaseInitQueries.end() && !shouldReset()) {
      return QueryResult(QueryResult::Success);   }
CreateBaseInitQuery _Q(*this, D);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

CreateBaseInitQueries.insert(D);
return QueryResult(QueryResult::Success);
}

template<> LookupFirstQuery *QueryContext::getQuery<LookupFirstQuery>(ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts)
{
   llvm::FoldingSetNodeID ID;
   LookupFirstQuery::Profile(ID, DC, Name, Opts);
   void *InsertPos;
   return LookupFirstQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::LookupFirst(const SingleLevelLookupResult* &Result, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts)

{
   llvm::FoldingSetNodeID ID;
   LookupFirstQuery::Profile(ID, DC, Name, Opts);
   void *InsertPos;
   auto *_Q = LookupFirstQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) LookupFirstQuery(*this, DC, Name, Opts);
      LookupFirstQueries.InsertNode(_Q, InsertPos);
   }   _Q->refresh();

#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Idle;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> LookupSingleQuery *QueryContext::getQuery<LookupSingleQuery>(ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts)
{
   llvm::FoldingSetNodeID ID;
   LookupSingleQuery::Profile(ID, DC, Name, Opts);
   void *InsertPos;
   return LookupSingleQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::LookupSingle(ast::NamedDecl* &Result, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts)

{
   llvm::FoldingSetNodeID ID;
   LookupSingleQuery::Profile(ID, DC, Name, Opts);
   void *InsertPos;
   auto *_Q = LookupSingleQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) LookupSingleQuery(*this, DC, Name, Opts);
      LookupSingleQueries.InsertNode(_Q, InsertPos);
   }   _Q->refresh();

#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Idle;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::MultiLevelLookup(const MultiLevelLookupResult* &Result, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts)

{
   auto *_Q = new(*this) MultiLevelLookupQuery(*this, DC, Name, Opts);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Idle;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::MultiLevelTypeLookup(const MultiLevelLookupResult* &Result, CanType T, DeclarationName Name, LookupOpts Opts)

{
   auto *_Q = new(*this) MultiLevelTypeLookupQuery(*this, T, Name, Opts);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> RestrictedLookupQuery *QueryContext::getQuery<RestrictedLookupQuery>(ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts)
{
   llvm::FoldingSetNodeID ID;
   RestrictedLookupQuery::Profile(ID, DC, Name, Opts);
   void *InsertPos;
   return RestrictedLookupQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::RestrictedLookup(const MultiLevelLookupResult* &Result, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts)

{
   llvm::FoldingSetNodeID ID;
   RestrictedLookupQuery::Profile(ID, DC, Name, Opts);
   void *InsertPos;
   auto *_Q = RestrictedLookupQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) RestrictedLookupQuery(*this, DC, Name, Opts);
      RestrictedLookupQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Idle;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> DirectLookupQuery *QueryContext::getQuery<DirectLookupQuery>(ast::DeclContext* DC, DeclarationName Name, bool LookInExtensions, LookupOpts Opts, ast::ConstraintSet* Constraints)
{
   llvm::FoldingSetNodeID ID;
   DirectLookupQuery::Profile(ID, DC, Name, LookInExtensions, Opts, Constraints);
   void *InsertPos;
   return DirectLookupQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::DirectLookup(const MultiLevelLookupResult* &Result, ast::DeclContext* DC, DeclarationName Name, bool LookInExtensions, LookupOpts Opts, ast::ConstraintSet* Constraints)

{
   llvm::FoldingSetNodeID ID;
   DirectLookupQuery::Profile(ID, DC, Name, LookInExtensions, Opts, Constraints);
   void *InsertPos;
   auto *_Q = DirectLookupQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) DirectLookupQuery(*this, DC, Name, LookInExtensions, Opts, Constraints);
      DirectLookupQueries.InsertNode(_Q, InsertPos);
   }   _Q->refresh();

#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Idle;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::NestedNameLookup(const MultiLevelLookupResult* &Result, ast::DeclContext* DC, llvm::ArrayRef<DeclarationName> Names, LookupOpts Opts, SourceRange Loc)

{
   auto *_Q = new(*this) NestedNameLookupQuery(*this, DC, Names, Opts, Loc);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::ResolveMacros(ast::DeclContext* DC)

{
   auto Key = DC;
   auto It = ResolveMacrosQueries.find(Key);
   if (It != ResolveMacrosQueries.end() && !shouldReset()) {
      return QueryResult(QueryResult::Success);   }
ResolveMacrosQuery _Q(*this, DC);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

ResolveMacrosQueries.insert(DC);
return QueryResult(QueryResult::Success);
}

QueryResult QueryContext::ResolveStaticDeclarations(ast::DeclContext* DC)

{
   auto Key = DC;
   auto It = ResolveStaticDeclarationsQueries.find(Key);
   if (It != ResolveStaticDeclarationsQueries.end() && !shouldReset()) {
      return QueryResult(QueryResult::Success);   }
ResolveStaticDeclarationsQuery _Q(*this, DC);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

ResolveStaticDeclarationsQueries.insert(DC);
return QueryResult(QueryResult::Success);
}

QueryResult QueryContext::ResolveWildcardDeclarations(ast::DeclContext* DC)

{
   auto Key = DC;
   auto It = ResolveWildcardDeclarationsQueries.find(Key);
   if (It != ResolveWildcardDeclarationsQueries.end() && !shouldReset()) {
      return QueryResult(QueryResult::Success);   }
ResolveWildcardDeclarationsQuery _Q(*this, DC);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

ResolveWildcardDeclarationsQueries.insert(DC);
return QueryResult(QueryResult::Success);
}

template<> ResolveMetaDeclarationsQuery *QueryContext::getQuery<ResolveMetaDeclarationsQuery>(ast::DeclContext* DC)
{
   auto It = ResolveMetaDeclarationsQueries.find(DC);
   if (It != ResolveMetaDeclarationsQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::ResolveMetaDeclarations(ast::DeclContext* DC)

{
   auto Key = DC;
   auto It = ResolveMetaDeclarationsQueries.find(Key);
   ResolveMetaDeclarationsQuery *_Q;
   if (It == ResolveMetaDeclarationsQueries.end()) {
      _Q = new(*this) ResolveMetaDeclarationsQuery(*this, DC);
      ResolveMetaDeclarationsQueries[_Q->DC] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Idle;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::GetAssociatedTypeImpl(ast::AliasDecl* &Result, ast::RecordDecl* R, DeclarationName Name, llvm::ArrayRef<ast::ExtensionDecl*> Extensions, ast::ConstraintSet* Constraints)

{
   auto *_Q = new(*this) GetAssociatedTypeImplQuery(*this, R, Name, Extensions, Constraints);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::GetAssociatedTypeDecl(ast::AssociatedTypeDecl* &Result, ast::ProtocolDecl* P, DeclarationName Name, ast::ConstraintSet* Constraints)

{
   auto *_Q = new(*this) GetAssociatedTypeDeclQuery(*this, P, Name, Constraints);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> CheckTypeEquivalenceQuery *QueryContext::getQuery<CheckTypeEquivalenceQuery>(QualType LHS, QualType RHS, QualType Self, ast::DeclContext* LHSDecl, ast::DeclContext* DeclCtx)
{
   llvm::FoldingSetNodeID ID;
   CheckTypeEquivalenceQuery::Profile(ID, LHS, RHS, Self, LHSDecl, DeclCtx);
   void *InsertPos;
   return CheckTypeEquivalenceQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::CheckTypeEquivalence(bool &Result, QualType LHS, QualType RHS, QualType Self, ast::DeclContext* LHSDecl, ast::DeclContext* DeclCtx)

{
   llvm::FoldingSetNodeID ID;
   CheckTypeEquivalenceQuery::Profile(ID, LHS, RHS, Self, LHSDecl, DeclCtx);
   void *InsertPos;
   auto *_Q = CheckTypeEquivalenceQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) CheckTypeEquivalenceQuery(*this, LHS, RHS, Self, LHSDecl, DeclCtx);
      CheckTypeEquivalenceQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> CheckTemplateExtensionApplicabilityQuery *QueryContext::getQuery<CheckTemplateExtensionApplicabilityQuery>(ast::RecordDecl* Inst, ast::ExtensionDecl* Ext)
{
   llvm::FoldingSetNodeID ID;
   CheckTemplateExtensionApplicabilityQuery::Profile(ID, Inst, Ext);
   void *InsertPos;
   return CheckTemplateExtensionApplicabilityQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::CheckTemplateExtensionApplicability(bool &Result, ast::RecordDecl* Inst, ast::ExtensionDecl* Ext)

{
   llvm::FoldingSetNodeID ID;
   CheckTemplateExtensionApplicabilityQuery::Profile(ID, Inst, Ext);
   void *InsertPos;
   auto *_Q = CheckTemplateExtensionApplicabilityQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) CheckTemplateExtensionApplicabilityQuery(*this, Inst, Ext);
      CheckTemplateExtensionApplicabilityQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetILModuleForDeclQuery *QueryContext::getQuery<GetILModuleForDeclQuery>(ast::Decl* D)
{
   auto It = GetILModuleForDeclQueries.find(D);
   if (It != GetILModuleForDeclQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetILModuleForDecl(il::Module* &Result, ast::Decl* D)

{
   auto Key = D;
   auto It = GetILModuleForDeclQueries.find(Key);
   GetILModuleForDeclQuery *_Q;
   if (It == GetILModuleForDeclQueries.end()) {
      _Q = new(*this) GetILModuleForDeclQuery(*this, D);
      GetILModuleForDeclQueries[_Q->D] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GenerateILForContextQuery *QueryContext::getQuery<GenerateILForContextQuery>(ast::DeclContext* DC)
{
   auto It = GenerateILForContextQueries.find(DC);
   if (It != GenerateILForContextQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GenerateILForContext(ast::DeclContext* DC)

{
   auto Key = DC;
   auto It = GenerateILForContextQueries.find(Key);
   GenerateILForContextQuery *_Q;
   if (It == GenerateILForContextQueries.end()) {
      _Q = new(*this) GenerateILForContextQuery(*this, DC);
      GenerateILForContextQueries[_Q->DC] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GenerateRecordILQuery *QueryContext::getQuery<GenerateRecordILQuery>(ast::RecordDecl* R)
{
   auto It = GenerateRecordILQueries.find(R);
   if (It != GenerateRecordILQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GenerateRecordIL(ast::RecordDecl* R)

{
   auto Key = R;
   auto It = GenerateRecordILQueries.find(Key);
   GenerateRecordILQuery *_Q;
   if (It == GenerateRecordILQueries.end()) {
      _Q = new(*this) GenerateRecordILQuery(*this, R);
      GenerateRecordILQueries[_Q->R] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetILGlobalQuery *QueryContext::getQuery<GetILGlobalQuery>(ast::VarDecl* GV)
{
   auto It = GetILGlobalQueries.find(GV);
   if (It != GetILGlobalQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetILGlobal(il::GlobalVariable* &Result, ast::VarDecl* GV)

{
   auto Key = GV;
   auto It = GetILGlobalQueries.find(Key);
   GetILGlobalQuery *_Q;
   if (It == GetILGlobalQueries.end()) {
      _Q = new(*this) GetILGlobalQuery(*this, GV);
      GetILGlobalQueries[_Q->GV] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GenerateLazyILGlobalDefinitionQuery *QueryContext::getQuery<GenerateLazyILGlobalDefinitionQuery>(ast::VarDecl* GV)
{
   auto It = GenerateLazyILGlobalDefinitionQueries.find(GV);
   if (It != GenerateLazyILGlobalDefinitionQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GenerateLazyILGlobalDefinition(ast::VarDecl* GV)

{
   auto Key = GV;
   auto It = GenerateLazyILGlobalDefinitionQueries.find(Key);
   GenerateLazyILGlobalDefinitionQuery *_Q;
   if (It == GenerateLazyILGlobalDefinitionQueries.end()) {
      _Q = new(*this) GenerateLazyILGlobalDefinitionQuery(*this, GV);
      GenerateLazyILGlobalDefinitionQueries[_Q->GV] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetILFunctionQuery *QueryContext::getQuery<GetILFunctionQuery>(ast::CallableDecl* C)
{
   auto It = GetILFunctionQueries.find(C);
   if (It != GetILFunctionQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetILFunction(il::Function* &Result, ast::CallableDecl* C)

{
   auto Key = C;
   auto It = GetILFunctionQueries.find(Key);
   GetILFunctionQuery *_Q;
   if (It == GetILFunctionQueries.end()) {
      _Q = new(*this) GetILFunctionQuery(*this, C);
      GetILFunctionQueries[_Q->C] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GenerateILFunctionBodyQuery *QueryContext::getQuery<GenerateILFunctionBodyQuery>(ast::CallableDecl* C)
{
   auto It = GenerateILFunctionBodyQueries.find(C);
   if (It != GenerateILFunctionBodyQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GenerateILFunctionBody(ast::CallableDecl* C)

{
   auto Key = C;
   auto It = GenerateILFunctionBodyQueries.find(Key);
   GenerateILFunctionBodyQuery *_Q;
   if (It == GenerateILFunctionBodyQueries.end()) {
      _Q = new(*this) GenerateILFunctionBodyQuery(*this, C);
      GenerateILFunctionBodyQueries[_Q->C] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetILTypeInfoQuery *QueryContext::getQuery<GetILTypeInfoQuery>(QualType T)
{
   auto It = GetILTypeInfoQueries.find(T);
   if (It != GetILTypeInfoQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetILTypeInfo(il::GlobalVariable* &Result, QualType T)

{
   auto Key = T;
   auto It = GetILTypeInfoQueries.find(Key);
   GetILTypeInfoQuery *_Q;
   if (It == GetILTypeInfoQueries.end()) {
      _Q = new(*this) GetILTypeInfoQuery(*this, T);
      GetILTypeInfoQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> CreateILBasicTypeInfoQuery *QueryContext::getQuery<CreateILBasicTypeInfoQuery>(QualType T)
{
   auto It = CreateILBasicTypeInfoQueries.find(T);
   if (It != CreateILBasicTypeInfoQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::CreateILBasicTypeInfo(il::Constant* &Result, QualType T)

{
   auto Key = T;
   auto It = CreateILBasicTypeInfoQueries.find(Key);
   CreateILBasicTypeInfoQuery *_Q;
   if (It == CreateILBasicTypeInfoQueries.end()) {
      _Q = new(*this) CreateILBasicTypeInfoQuery(*this, T);
      CreateILBasicTypeInfoQueries[_Q->T] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> CreateILRecordTypeInfoQuery *QueryContext::getQuery<CreateILRecordTypeInfoQuery>(ast::RecordDecl* R)
{
   auto It = CreateILRecordTypeInfoQueries.find(R);
   if (It != CreateILRecordTypeInfoQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::CreateILRecordTypeInfo(il::Constant* &Result, ast::RecordDecl* R)

{
   auto Key = R;
   auto It = CreateILRecordTypeInfoQueries.find(Key);
   CreateILRecordTypeInfoQuery *_Q;
   if (It == CreateILRecordTypeInfoQueries.end()) {
      _Q = new(*this) CreateILRecordTypeInfoQuery(*this, R);
      CreateILRecordTypeInfoQueries[_Q->R] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::GetBoolValue(bool &Result, il::Constant* C, uint8_t AllowWrapperTypes)

{
   auto Key = std::make_pair(C, AllowWrapperTypes);
   auto It = GetBoolValueQueries.find(Key);
   if (It != GetBoolValueQueries.end() && !shouldReset()) {
      Result = It->getSecond();      return QueryResult(QueryResult::Success);   }
GetBoolValueQuery _Q(*this, C, AllowWrapperTypes);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

Result = _Q.get();
GetBoolValueQueries[Key] = Result;
return QueryResult(QueryResult::Success);
}

QueryResult QueryContext::GetStringValue(llvm::StringRef &Result, il::Constant* C, uint8_t AllowWrapperTypes)

{
   auto Key = std::make_pair(C, AllowWrapperTypes);
   auto It = GetStringValueQueries.find(Key);
   if (It != GetStringValueQueries.end() && !shouldReset()) {
      Result = It->getSecond();      return QueryResult(QueryResult::Success);   }
GetStringValueQuery _Q(*this, C, AllowWrapperTypes);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

Result = _Q.get();
GetStringValueQueries[Key] = Result;
return QueryResult(QueryResult::Success);
}

QueryResult QueryContext::GetIntValue(llvm::APSInt &Result, il::Constant* C, uint8_t AllowWrapperTypes)

{
   auto Key = std::make_pair(C, AllowWrapperTypes);
   auto It = GetIntValueQueries.find(Key);
   if (It != GetIntValueQueries.end() && !shouldReset()) {
      Result = It->getSecond();      return QueryResult(QueryResult::Success);   }
GetIntValueQuery _Q(*this, C, AllowWrapperTypes);

ExecutingQuery EQ(*this, &_Q);
auto MaybeErr = _Q.run();
if (MaybeErr.isErr()) return MaybeErr;

Result = _Q.get();
GetIntValueQueries[Key] = Result;
return QueryResult(QueryResult::Success);
}

template<> OpenFileForReadQuery *QueryContext::getQuery<OpenFileForReadQuery>(StringRef FileName)
{
   auto It = OpenFileForReadQueries.find(FileName);
   if (It != OpenFileForReadQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::OpenFileForRead(llvm::MemoryBuffer* &Result, StringRef FileName)

{
   auto Key = FileName;
   auto It = OpenFileForReadQueries.find(Key);
   OpenFileForReadQuery *_Q;
   if (It == OpenFileForReadQueries.end()) {
      _Q = new(*this) OpenFileForReadQuery(*this, FileName);
      OpenFileForReadQueries[_Q->FileName] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::OpenTmpFile(llvm::raw_ostream* &Result, StringRef Extension)

{
   auto *_Q = new(*this) OpenTmpFileQuery(*this, Extension);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> GetDefaultTemplateArgQuery *QueryContext::getQuery<GetDefaultTemplateArgQuery>(ast::TemplateParamDecl* P)
{
   auto It = GetDefaultTemplateArgQueries.find(P);
   if (It != GetDefaultTemplateArgQueries.end()) return It->getSecond();
   return nullptr;
}

QueryResult QueryContext::GetDefaultTemplateArg(const sema::TemplateArgument* &Result, ast::TemplateParamDecl* P)

{
   auto Key = P;
   auto It = GetDefaultTemplateArgQueries.find(Key);
   GetDefaultTemplateArgQuery *_Q;
   if (It == GetDefaultTemplateArgQueries.end()) {
      _Q = new(*this) GetDefaultTemplateArgQuery(*this, P);
      GetDefaultTemplateArgQueries[_Q->P] = _Q;
   } else {
      _Q = It->getSecond();
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

QueryResult QueryContext::CreateSelfArgument(ast::FuncArgDecl* &Result, QualType Self, SourceLocation SelfLoc)

{
   auto *_Q = new(*this) CreateSelfArgumentQuery(*this, Self, SelfLoc);
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> FindEquivalentDeclQuery *QueryContext::getQuery<FindEquivalentDeclQuery>(ast::NamedDecl* Decl, ast::DeclContext* DC, QualType Self, bool LookInExtensions)
{
   llvm::FoldingSetNodeID ID;
   FindEquivalentDeclQuery::Profile(ID, Decl, DC, Self, LookInExtensions);
   void *InsertPos;
   return FindEquivalentDeclQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::FindEquivalentDecl(ast::NamedDecl* &Result, ast::NamedDecl* Decl, ast::DeclContext* DC, QualType Self, bool LookInExtensions)

{
   llvm::FoldingSetNodeID ID;
   FindEquivalentDeclQuery::Profile(ID, Decl, DC, Self, LookInExtensions);
   void *InsertPos;
   auto *_Q = FindEquivalentDeclQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) FindEquivalentDeclQuery(*this, Decl, DC, Self, LookInExtensions);
      FindEquivalentDeclQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

template<> EquivalentSignaturesQuery *QueryContext::getQuery<EquivalentSignaturesQuery>(ast::CallableDecl* C1, ast::CallableDecl* C2, QualType Self)
{
   llvm::FoldingSetNodeID ID;
   EquivalentSignaturesQuery::Profile(ID, C1, C2, Self);
   void *InsertPos;
   return EquivalentSignaturesQueries.FindNodeOrInsertPos(ID, InsertPos);
}

QueryResult QueryContext::EquivalentSignatures(EquivalentSignaturesQuery::ResultKind &Result, ast::CallableDecl* C1, ast::CallableDecl* C2, QualType Self)

{
   llvm::FoldingSetNodeID ID;
   EquivalentSignaturesQuery::Profile(ID, C1, C2, Self);
   void *InsertPos;
   auto *_Q = EquivalentSignaturesQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new(*this) EquivalentSignaturesQuery(*this, C1, C2, Self);
      EquivalentSignaturesQueries.InsertNode(_Q, InsertPos);
   }
#ifndef NDEBUG
   if (shouldReset()) _Q->Stat = Query::Idle;
#endif
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle:
      {
         ExecutingQuery EQ(*this, _Q);
         _Q->Stat = Query::Running;
         if (auto _R = _Q->run()) {
            return _R;
         }
      }

      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Done:
   case Query::DoneWithError:
      Result = _Q->get();
      return QueryResult(QueryResult::Success);
   case Query::Aborted:
      return QueryResult(QueryResult::Error);
   case Query::Dependent:
      return QueryResult(QueryResult::Dependent);
   }

}

