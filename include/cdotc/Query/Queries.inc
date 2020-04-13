#ifdef CDOT_QUERY_DECL
class CompileModuleQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CompileModuleQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CompileModuleQuery(QueryContext& QC, SourceRange Loc = {});
};

class ParseSourceFileQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ParseSourceFileQueryID; }

   std::string description() const;
   std::string summary() const;

   ast::ModuleDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ParseSourceFileQuery(QueryContext& QC, StringRef FileName,
                        SourceRange Loc = {});

   QueryResult finish(ast::ModuleDecl* Result, Status St = Done);

   StringRef FileName;
   llvm::Optional<ast::ModuleDecl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, StringRef FileName);
};

class TypeCheckASTQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypeCheckASTQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypeCheckASTQuery(QueryContext& QC, ast::ModuleDecl* Mod,
                     SourceRange Loc = {});

   ast::ModuleDecl* Mod;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::ModuleDecl* Mod);
};

class CreateILModuleQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CreateILModuleQueryID; }

   std::string description() const;
   std::string summary() const;

   il::Module* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateILModuleQuery(QueryContext& QC, llvm::ArrayRef<StringRef> SourceFiles,
                       SourceRange Loc = {});

   QueryResult finish(il::Module* Result, Status St = Done);

   llvm::ArrayRef<StringRef> SourceFiles;
   llvm::Optional<il::Module*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID,
                       llvm::ArrayRef<StringRef> SourceFiles);
};

class SetupIRGenQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::SetupIRGenQueryID; }

   std::string description() const;
   std::string summary() const;

   il::IRGen* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   SetupIRGenQuery(QueryContext& QC, SourceRange Loc = {});

   QueryResult finish(std::unique_ptr<il::IRGen>&& Result, Status St = Done);

   llvm::Optional<std::unique_ptr<il::IRGen>> Result;
};

class CreateLLVMModuleQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::CreateLLVMModuleQueryID;
   }

   std::string description() const;
   std::string summary() const;

   llvm::Module* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateLLVMModuleQuery(QueryContext& QC,
                         llvm::ArrayRef<StringRef> SourceFiles,
                         SourceRange Loc = {});

   QueryResult finish(llvm::Module* Result, Status St = Done);

   llvm::ArrayRef<StringRef> SourceFiles;
   llvm::Optional<llvm::Module*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID,
                       llvm::ArrayRef<StringRef> SourceFiles);
};

class CreateObjectQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CreateObjectQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateObjectQuery(QueryContext& QC, llvm::ArrayRef<StringRef> SourceFiles,
                     llvm::raw_ostream& OS, SourceRange Loc = {});

   llvm::ArrayRef<StringRef> SourceFiles;
   llvm::raw_ostream& OS;
};

class CreateStaticLibQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CreateStaticLibQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateStaticLibQuery(QueryContext& QC, llvm::ArrayRef<StringRef> SourceFiles,
                        StringRef OutFile, SourceRange Loc = {});

   llvm::ArrayRef<StringRef> SourceFiles;
   StringRef OutFile;
};

class CreateDynamicLibQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::CreateDynamicLibQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateDynamicLibQuery(QueryContext& QC,
                         llvm::ArrayRef<StringRef> SourceFiles,
                         StringRef OutFile, SourceRange Loc = {});

   llvm::ArrayRef<StringRef> SourceFiles;
   StringRef OutFile;
};

class CreateExecutableQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::CreateExecutableQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateExecutableQuery(QueryContext& QC,
                         llvm::ArrayRef<StringRef> SourceFiles,
                         StringRef OutFile, SourceRange Loc = {});

   llvm::ArrayRef<StringRef> SourceFiles;
   StringRef OutFile;
};

class EmitILQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::EmitILQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   EmitILQuery(QueryContext& QC, llvm::ArrayRef<StringRef> SourceFiles,
               llvm::raw_ostream& OS, SourceRange Loc = {});

   llvm::ArrayRef<StringRef> SourceFiles;
   llvm::raw_ostream& OS;
};

class EmitIRQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::EmitIRQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   EmitIRQuery(QueryContext& QC, llvm::ArrayRef<StringRef> SourceFiles,
               llvm::raw_ostream& OS, SourceRange Loc = {});

   llvm::ArrayRef<StringRef> SourceFiles;
   llvm::raw_ostream& OS;
};

class PrintUsedMemoryQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrintUsedMemoryQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrintUsedMemoryQuery(QueryContext& QC, SourceRange Loc = {});
};

class MeasureExecutionTimeQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::MeasureExecutionTimeQueryID;
   }

   std::string description() const;
   std::string summary() const;

   long long get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   MeasureExecutionTimeQuery(QueryContext& QC, Query* Q, SourceRange Loc = {});

   QueryResult finish(long long Result, Status St = Done);

   Query* Q;
   llvm::Optional<long long> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, Query* Q);
};

class IsEquatableQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsEquatableQueryID; }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsEquatableQuery(QueryContext& QC, QualType T, SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   QualType T;
   llvm::Optional<bool> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class IsCopyableQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsCopyableQueryID; }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsCopyableQuery(QueryContext& QC, QualType T, SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   QualType T;
   llvm::Optional<bool> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class IsMoveOnlyQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsMoveOnlyQueryID; }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsMoveOnlyQuery(QueryContext& QC, QualType T, SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   QualType T;
   llvm::Optional<bool> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class IsImplicitlyCopyableQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::IsImplicitlyCopyableQueryID;
   }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsImplicitlyCopyableQuery(QueryContext& QC, QualType T,
                             SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   QualType T;
   llvm::Optional<bool> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class IsTriviallyCopyableQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::IsTriviallyCopyableQueryID;
   }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsTriviallyCopyableQuery(QueryContext& QC, QualType T, SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   QualType T;
   llvm::Optional<bool> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class NeedsRetainOrReleaseQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::NeedsRetainOrReleaseQueryID;
   }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   NeedsRetainOrReleaseQuery(QueryContext& QC, QualType T,
                             SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   QualType T;
   llvm::Optional<bool> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class NeedsDeinitilizationQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::NeedsDeinitilizationQueryID;
   }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   NeedsDeinitilizationQuery(QueryContext& QC, QualType T,
                             SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   QualType T;
   llvm::Optional<bool> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class NeedsStructReturnQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::NeedsStructReturnQueryID;
   }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   NeedsStructReturnQuery(QueryContext& QC, QualType T, SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   QualType T;
   llvm::Optional<bool> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class SubstAssociatedTypesQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::SubstAssociatedTypesQueryID;
   }

   std::string description() const;
   std::string summary() const;

   QualType get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   SubstAssociatedTypesQuery(QueryContext& QC, QualType T, QualType Self,
                             SourceRange Loc = {});

   QueryResult finish(QualType Result, Status St = Done);

   QualType T;
   QualType Self;
   llvm::Optional<QualType> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T, QualType Self);
};

class SubstTemplateParamTypesQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::SubstTemplateParamTypesQueryID;
   }

   std::string description() const;
   std::string summary() const;

   QualType get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   SubstTemplateParamTypesQuery(
       QueryContext& QC, QualType T,
       sema::MultiLevelFinalTemplateArgList TemplateArgs, SourceRange Loc = {});

   QueryResult finish(QualType Result, Status St = Done);

   QualType T;
   sema::MultiLevelFinalTemplateArgList TemplateArgs;
   llvm::Optional<QualType> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void
   Profile(llvm::FoldingSetNodeID& ID, QualType T,
           const sema::MultiLevelFinalTemplateArgList& TemplateArgs);
};

class GetBuiltinModuleQuery : public Query {

public:
   enum ModuleKind : char {
      Std,
      Prelude,
      Builtin,
      Reflect,
      Sys,
      Runtime,
      Async,
      Test,
   };

private:
   llvm::DenseMap<char, Module*> Cache;

public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::GetBuiltinModuleQueryID;
   }

   std::string description() const;
   std::string summary() const;

   Module* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetBuiltinModuleQuery(QueryContext& QC,
                         GetBuiltinModuleQuery::ModuleKind Mod,
                         SourceRange Loc = {});

   QueryResult finish(Module* Result, Status St = Done);

   GetBuiltinModuleQuery::ModuleKind Mod;
   llvm::Optional<Module*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID,
                       GetBuiltinModuleQuery::ModuleKind Mod);
};

class GetBuiltinFuncQuery : public Query {

public:
   enum FunctionKind : char {
      PureVirtual,
      CopyClass,
      AtomicRelease,
   };

private:
   llvm::DenseMap<char, ast::CallableDecl*> Cache;

public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetBuiltinFuncQueryID; }

   std::string description() const;
   std::string summary() const;

   ast::CallableDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetBuiltinFuncQuery(QueryContext& QC, GetBuiltinFuncQuery::FunctionKind Fn,
                       SourceRange Loc = {});

   QueryResult finish(ast::CallableDecl* Result, Status St = Done);

   GetBuiltinFuncQuery::FunctionKind Fn;
   llvm::Optional<ast::CallableDecl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID,
                       GetBuiltinFuncQuery::FunctionKind Fn);
};

class GetBuiltinRecordQuery : public Query {

public:
   enum RecordKind : char {
      Array,
      Dictionary,
      String,
      Option,
      Box,
      Promise,
      Future,
      CoroHandle,

      TypeInfo,
      ValueWitnessTable,
      ProtocolConformance,
      ExistentialContainer,
   };

private:
   llvm::DenseMap<char, ast::RecordDecl*> Cache;

public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::GetBuiltinRecordQueryID;
   }

   std::string description() const;
   std::string summary() const;

   ast::RecordDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetBuiltinRecordQuery(QueryContext& QC, GetBuiltinRecordQuery::RecordKind R,
                         SourceRange Loc = {});

   QueryResult finish(ast::RecordDecl* Result, Status St = Done);

   GetBuiltinRecordQuery::RecordKind R;
   llvm::Optional<ast::RecordDecl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID,
                       GetBuiltinRecordQuery::RecordKind R);
};

class GetBuiltinProtocolQuery : public Query {

public:
   enum ProtocolKind : char {
      Any,
      Equatable,
      Hashable,
      Copyable,
      MoveOnly,
      ImplicitlyCopyable,
      StringRepresentable,
      Persistable,
      Awaiter,
      Awaitable,
   };

private:
   llvm::DenseMap<char, ast::ProtocolDecl*> Cache;

public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::GetBuiltinProtocolQueryID;
   }

   std::string description() const;
   std::string summary() const;

   ast::ProtocolDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetBuiltinProtocolQuery(QueryContext& QC,
                           GetBuiltinProtocolQuery::ProtocolKind P,
                           SourceRange Loc = {});

   QueryResult finish(ast::ProtocolDecl* Result, Status St = Done);

   GetBuiltinProtocolQuery::ProtocolKind P;
   llvm::Optional<ast::ProtocolDecl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID,
                       GetBuiltinProtocolQuery::ProtocolKind P);
};

class FindPrecedenceGroupQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::FindPrecedenceGroupQueryID;
   }

   std::string description() const;
   std::string summary() const;

   ast::PrecedenceGroupDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   FindPrecedenceGroupQuery(QueryContext& QC, DeclarationName Name,
                            bool DiagOnError = true, SourceRange Loc = {});

   QueryResult finish(ast::PrecedenceGroupDecl* Result, Status St = Done);

   DeclarationName Name;
   bool DiagOnError;
   llvm::Optional<ast::PrecedenceGroupDecl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, DeclarationName Name,
                       bool DiagOnError);
};

class FindOperatorQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::FindOperatorQueryID; }

   std::string description() const;
   std::string summary() const;

   ast::OperatorDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   FindOperatorQuery(QueryContext& QC, DeclarationName Name,
                     ast::DeclContext* DC, bool DiagOnError = true,
                     SourceRange Loc = {});

   QueryResult finish(ast::OperatorDecl* Result, Status St = Done);

   DeclarationName Name;
   ast::DeclContext* DC;
   bool DiagOnError;
   llvm::Optional<ast::OperatorDecl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, DeclarationName Name,
                       ast::DeclContext* DC, bool DiagOnError);
};

class ResolveImportQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveImportQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveImportQuery(QueryContext& QC, ast::ImportDecl* I,
                      SourceRange Loc = {});

   ast::ImportDecl* I;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::ImportDecl* I);
};

class ResolveUsingQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveUsingQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveUsingQuery(QueryContext& QC, ast::UsingDecl* U, SourceRange Loc = {});

   ast::UsingDecl* U;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::UsingDecl* U);
};

class ResolveStaticIfQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveStaticIfQueryID; }

   std::string description() const;
   std::string summary() const;

   ast::Decl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveStaticIfQuery(QueryContext& QC, ast::StaticIfDecl* Decl,
                        SourceRange Loc = {});

   QueryResult finish(ast::Decl* Result, Status St = Done);

   ast::StaticIfDecl* Decl;
   llvm::Optional<ast::Decl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::StaticIfDecl* Decl);
};

class ResolveStaticForQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::ResolveStaticForQueryID;
   }

   std::string description() const;
   std::string summary() const;

   ast::Decl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveStaticForQuery(QueryContext& QC, ast::StaticForDecl* Decl,
                         SourceRange Loc = {});

   QueryResult finish(ast::Decl* Result, Status St = Done);

   ast::StaticForDecl* Decl;
   llvm::Optional<ast::Decl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::StaticForDecl* Decl);
};

class ResolveStaticAssertQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::ResolveStaticAssertQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveStaticAssertQuery(QueryContext& QC, ast::StaticAssertDecl* Decl,
                            SourceRange Loc = {});

   ast::StaticAssertDecl* Decl;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::StaticAssertDecl* Decl);
};

class ResolveStaticPrintQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::ResolveStaticPrintQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveStaticPrintQuery(QueryContext& QC, ast::StaticPrintDecl* Decl,
                           SourceRange Loc = {});

   ast::StaticPrintDecl* Decl;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::StaticPrintDecl* Decl);
};

class ResolveStaticExprQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::ResolveStaticExprQueryID;
   }

   std::string description() const;
   std::string summary() const;

   il::Constant* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveStaticExprQuery(QueryContext& QC, ast::StaticExpr* Expr,
                          SourceRange Loc = {});

   QueryResult finish(il::Constant* Result, Status St = Done);

   ast::StaticExpr* Expr;
   llvm::Optional<il::Constant*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::StaticExpr* Expr);
};

class ResolveStaticExprToBoolQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::ResolveStaticExprToBoolQueryID;
   }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveStaticExprToBoolQuery(QueryContext& QC, ast::StaticExpr* Expr,
                                bool DiagOnError, SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   ast::StaticExpr* Expr;
   bool DiagOnError;
   llvm::Optional<bool> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::StaticExpr* Expr,
                       bool DiagOnError);
};

class ExpandMacroQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ExpandMacroQueryID; }

   std::string description() const;
   std::string summary() const;

   StmtOrDecl get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ExpandMacroQuery(QueryContext& QC, StmtOrDecl SOD, DeclarationName Name,
                    ast::DeclContext* DC, unsigned Delim,
                    llvm::ArrayRef<lex::Token> Tokens, unsigned ExpectedKind,
                    SourceRange Loc = {});

   QueryResult finish(StmtOrDecl Result, Status St = Done);

   StmtOrDecl SOD;
   DeclarationName Name;
   ast::DeclContext* DC;
   unsigned Delim;
   llvm::ArrayRef<lex::Token> Tokens;
   unsigned ExpectedKind;
   llvm::Optional<StmtOrDecl> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, StmtOrDecl SOD,
                       DeclarationName Name, ast::DeclContext* DC,
                       unsigned Delim, llvm::ArrayRef<lex::Token> Tokens,
                       unsigned ExpectedKind);
};

class ExpandMacroDeclQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ExpandMacroDeclQueryID; }

   std::string description() const;
   std::string summary() const;

   ast::Decl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ExpandMacroDeclQuery(QueryContext& QC, ast::MacroExpansionDecl* Decl,
                        SourceRange Loc = {});

   QueryResult finish(ast::Decl* Result, Status St = Done);

   ast::MacroExpansionDecl* Decl;
   llvm::Optional<ast::Decl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID,
                       ast::MacroExpansionDecl* Decl);
};

class ExpandMacrosQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ExpandMacrosQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ExpandMacrosQuery(QueryContext& QC, StringRef SourceFile,
                     SourceRange Loc = {});

   StringRef SourceFile;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, StringRef SourceFile);
};

class FindExtensionsQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::FindExtensionsQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   FindExtensionsQuery(QueryContext& QC, QualType T, SourceRange Loc = {});

   QualType T;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class GetExtensionTypeKindQuery : public Query {

public:
   enum ResultKind {

      Nominal,

      Builtin,

      Pointer,

      Tuple,

      Function,

      Array,
   };

public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::GetExtensionTypeKindQueryID;
   }

   std::string description() const;
   std::string summary() const;

   GetExtensionTypeKindQuery::ResultKind get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetExtensionTypeKindQuery(QueryContext& QC, ast::SourceType T,
                             SourceRange Loc = {});

   QueryResult finish(GetExtensionTypeKindQuery::ResultKind Result,
                      Status St = Done);

   ast::SourceType T;
   llvm::Optional<GetExtensionTypeKindQuery::ResultKind> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::SourceType T);
};

class MatchExtensionTypeQuery : public Query, public llvm::FoldingSetNode {

public:
   enum ResultKind {

      AppliesDirectly,

      Applies,

      MightApply,

      DoesNotApply
   };

public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::MatchExtensionTypeQueryID;
   }

   std::string description() const;
   std::string summary() const;

   MatchExtensionTypeQuery::ResultKind get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   MatchExtensionTypeQuery(QueryContext& QC, QualType T,
                           ast::SourceType PatternTy, SourceRange Loc = {});

   QueryResult finish(MatchExtensionTypeQuery::ResultKind Result,
                      Status St = Done);

   QualType T;
   ast::SourceType PatternTy;
   llvm::Optional<MatchExtensionTypeQuery::ResultKind> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T,
                       ast::SourceType PatternTy);
};

class ExtensionAppliesQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::ExtensionAppliesQueryID;
   }

   std::string description() const;
   std::string summary() const;

   MatchExtensionTypeQuery::ResultKind get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ExtensionAppliesQuery(QueryContext& QC, ast::ExtensionDecl* Ext, QualType T,
                         SourceRange Loc = {});

   QueryResult finish(MatchExtensionTypeQuery::ResultKind Result,
                      Status St = Done);

   ast::ExtensionDecl* Ext;
   QualType T;
   llvm::Optional<MatchExtensionTypeQuery::ResultKind> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::ExtensionDecl* Ext,
                       QualType T);
};

class GetExtendedDeclQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetExtendedDeclQueryID; }

   std::string description() const;
   std::string summary() const;

   ast::NamedDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetExtendedDeclQuery(QueryContext& QC, ast::SourceType ExtendedTy,
                        SourceRange Loc = {});

   QueryResult finish(ast::NamedDecl* Result, Status St = Done);

   ast::SourceType ExtendedTy;
   llvm::Optional<ast::NamedDecl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::SourceType ExtendedTy);
};

class ResolveExtensionQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::ResolveExtensionQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveExtensionQuery(QueryContext& QC, ast::ExtensionDecl* Ext,
                         SourceRange Loc = {});

   ast::ExtensionDecl* Ext;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::ExtensionDecl* Ext);
};

class ResolveConformanceToProtocolQuery : public Query,
                                          public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::ResolveConformanceToProtocolQueryID;
   }

   std::string description() const;
   std::string summary() const;

   ast::ProtocolDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveConformanceToProtocolQuery(QueryContext& QC, QualType T,
                                     ast::SourceType Conf,
                                     SourceRange Loc = {});

   QueryResult finish(ast::ProtocolDecl* Result, Status St = Done);

   QualType T;
   ast::SourceType Conf;
   llvm::Optional<ast::ProtocolDecl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T,
                       ast::SourceType Conf);
};

class ResolveDeclaredConformancesQuery : public Query,
                                         public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::ResolveDeclaredConformancesQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveDeclaredConformancesQuery(
       QueryContext& QC, QualType T,
       llvm::ArrayRef<ast::SourceType> Conformances, SourceRange Loc = {});

   QualType T;
   llvm::ArrayRef<ast::SourceType> Conformances;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T,
                       llvm::ArrayRef<ast::SourceType> Conformances);
};

class AddInheritedConformanceQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::AddInheritedConformanceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   AddInheritedConformanceQuery(QueryContext& QC, QualType T,
                                ast::ProtocolDecl* Proto, SourceRange Loc = {});

   QualType T;
   ast::ProtocolDecl* Proto;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T,
                       ast::ProtocolDecl* Proto);
};

class ResolveAssociatedTypesQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::ResolveAssociatedTypesQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveAssociatedTypesQuery(QueryContext& QC, QualType T,
                               SourceRange Loc = {});

   QualType T;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class GetReferencedAssociatedTypesQuery : public Query,
                                          public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::GetReferencedAssociatedTypesQueryID;
   }

   std::string description() const;
   std::string summary() const;

   ArrayRef<ast::AssociatedTypeDecl*> get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetReferencedAssociatedTypesQuery(QueryContext& QC,
                                     ast::NamedDecl* ConstrainedDecl,
                                     ast::DeclConstraint* C,
                                     SourceRange Loc = {});

   QueryResult finish(SmallVector<ast::AssociatedTypeDecl*, 2>&& Result,
                      Status St = Done);

   ast::NamedDecl* ConstrainedDecl;
   ast::DeclConstraint* C;
   llvm::Optional<SmallVector<ast::AssociatedTypeDecl*, 2>> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID,
                       ast::NamedDecl* ConstrainedDecl, ast::DeclConstraint* C);
};

class ReferencedAssociatedTypesReadyQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::ReferencedAssociatedTypesReadyQueryID;
   }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ReferencedAssociatedTypesReadyQuery(QueryContext& QC, QualType T,
                                       ast::NamedDecl* ConstrainedDecl,
                                       SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   QualType T;
   ast::NamedDecl* ConstrainedDecl;
   llvm::Optional<bool> Result;
};

class CheckProtocolExtensionApplicabilityQuery : public Query,
                                                 public llvm::FoldingSetNode {

public:
   enum ResultKind : uint8_t {

      CantTell,

      DoesNotApply,

      DoesApply,
   };

public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::CheckProtocolExtensionApplicabilityQueryID;
   }

   std::string description() const;
   std::string summary() const;

   CheckProtocolExtensionApplicabilityQuery::ResultKind get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckProtocolExtensionApplicabilityQuery(QueryContext& QC, QualType T,
                                            ast::ExtensionDecl* Ext,
                                            SourceRange Loc = {});

   QueryResult
   finish(CheckProtocolExtensionApplicabilityQuery::ResultKind Result,
          Status St = Done);

   QualType T;
   ast::ExtensionDecl* Ext;
   llvm::Optional<CheckProtocolExtensionApplicabilityQuery::ResultKind> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T,
                       ast::ExtensionDecl* Ext);
};

class VerifyConstraintQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::VerifyConstraintQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   VerifyConstraintQuery(QueryContext& QC, ast::DeclConstraint* C,
                         ast::NamedDecl* ConstrainedDecl, SourceRange Loc = {});

   ast::DeclConstraint* C;
   ast::NamedDecl* ConstrainedDecl;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::DeclConstraint* C,
                       ast::NamedDecl* ConstrainedDecl);
};

class IsConstraintSatisfiedQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::IsConstraintSatisfiedQueryID;
   }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsConstraintSatisfiedQuery(QueryContext& QC, ast::DeclConstraint* C,
                              QualType Self, ast::NamedDecl* OriginalDecl,
                              SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   ast::DeclConstraint* C;
   QualType Self;
   ast::NamedDecl* OriginalDecl;
   llvm::Optional<bool> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::DeclConstraint* C,
                       QualType Self, ast::NamedDecl* OriginalDecl);
};

class CheckConformancesQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::CheckConformancesQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckConformancesQuery(QueryContext& QC, QualType T, SourceRange Loc = {});

   QualType T;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class CheckSingleConformanceQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::CheckSingleConformanceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckSingleConformanceQuery(QueryContext& QC, QualType T,
                               ast::ProtocolDecl* P, SourceRange Loc = {});

   QualType T;
   ast::ProtocolDecl* P;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T,
                       ast::ProtocolDecl* P);
};

class ConformsToQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ConformsToQueryID; }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ConformsToQuery(QueryContext& QC, QualType T, ast::ProtocolDecl* P,
                   SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   QualType T;
   ast::ProtocolDecl* P;
   llvm::Optional<bool> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T,
                       ast::ProtocolDecl* P);
};

class GetTypeSizeQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetTypeSizeQueryID; }

   std::string description() const;
   std::string summary() const;

   unsigned get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetTypeSizeQuery(QueryContext& QC, QualType T, SourceRange Loc = {});

   QueryResult finish(unsigned Result, Status St = Done);

   QualType T;
   llvm::Optional<unsigned> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class GetTypeAlignmentQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::GetTypeAlignmentQueryID;
   }

   std::string description() const;
   std::string summary() const;

   unsigned short get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetTypeAlignmentQuery(QueryContext& QC, QualType T, SourceRange Loc = {});

   QueryResult finish(unsigned short Result, Status St = Done);

   QualType T;
   llvm::Optional<unsigned short> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class GetTypeStrideQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetTypeStrideQueryID; }

   std::string description() const;
   std::string summary() const;

   unsigned get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetTypeStrideQuery(QueryContext& QC, QualType T, SourceRange Loc = {});

   QueryResult finish(unsigned Result, Status St = Done);

   QualType T;
   llvm::Optional<unsigned> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class CalculateRecordSizeQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::CalculateRecordSizeQueryID;
   }

   std::string description() const;
   std::string summary() const;

   unsigned get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CalculateRecordSizeQuery(QueryContext& QC, ast::RecordDecl* R,
                            SourceRange Loc = {});

   QueryResult finish(unsigned Result, Status St = Done);

   ast::RecordDecl* R;
   llvm::Optional<unsigned> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::RecordDecl* R);
};

class IsAccessibleQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsAccessibleQueryID; }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsAccessibleQuery(QueryContext& QC, ast::DeclContext* DC, ast::NamedDecl* ND,
                     SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   ast::DeclContext* DC;
   ast::NamedDecl* ND;
   llvm::Optional<bool> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::DeclContext* DC,
                       ast::NamedDecl* ND);
};

class CheckAccessibilityQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::CheckAccessibilityQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckAccessibilityQuery(QueryContext& QC, ast::DeclContext* DC,
                           ast::NamedDecl* ND, SourceRange Loc = {});

   ast::DeclContext* DC;
   ast::NamedDecl* ND;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::DeclContext* DC,
                       ast::NamedDecl* ND);
};

class TypeCheckDeclContextQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::TypeCheckDeclContextQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypeCheckDeclContextQuery(QueryContext& QC, ast::DeclContext* DC,
                             SourceRange Loc = {});

   ast::DeclContext* DC;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::DeclContext* DC);
};

class PrepareDeclInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareDeclInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareDeclInterfaceQuery(QueryContext& QC, ast::Decl* D,
                             SourceRange Loc = {});

   ast::Decl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::Decl* D);
};

class TypecheckDeclQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckDeclQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckDeclQuery(QueryContext& QC, ast::Decl* D, SourceRange Loc = {});

   ast::Decl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::Decl* D);
};

class PrepareCallableInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareCallableInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareCallableInterfaceQuery(QueryContext& QC, ast::CallableDecl* D,
                                 SourceRange Loc = {});

   ast::CallableDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::CallableDecl* D);
};

class TypecheckCallableQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::TypecheckCallableQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckCallableQuery(QueryContext& QC, ast::CallableDecl* D,
                          SourceRange Loc = {});

   ast::CallableDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::CallableDecl* D);
};

class PrepareFunctionInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareFunctionInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareFunctionInterfaceQuery(QueryContext& QC, ast::FunctionDecl* D,
                                 SourceRange Loc = {});

   ast::FunctionDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::FunctionDecl* D);
};

class TypecheckFunctionQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::TypecheckFunctionQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckFunctionQuery(QueryContext& QC, ast::FunctionDecl* D,
                          SourceRange Loc = {});

   ast::FunctionDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::FunctionDecl* D);
};

class PrepareMethodInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareMethodInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareMethodInterfaceQuery(QueryContext& QC, ast::MethodDecl* D,
                               SourceRange Loc = {});

   ast::MethodDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::MethodDecl* D);
};

class TypecheckMethodQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckMethodQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckMethodQuery(QueryContext& QC, ast::MethodDecl* D,
                        SourceRange Loc = {});

   ast::MethodDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::MethodDecl* D);
};

class PrepareInitInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareInitInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareInitInterfaceQuery(QueryContext& QC, ast::InitDecl* D,
                             SourceRange Loc = {});

   ast::InitDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::InitDecl* D);
};

class TypecheckInitQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckInitQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckInitQuery(QueryContext& QC, ast::InitDecl* D, SourceRange Loc = {});

   ast::InitDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::InitDecl* D);
};

class PrepareDeinitInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareDeinitInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareDeinitInterfaceQuery(QueryContext& QC, ast::DeinitDecl* D,
                               SourceRange Loc = {});

   ast::DeinitDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::DeinitDecl* D);
};

class TypecheckDeinitQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckDeinitQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckDeinitQuery(QueryContext& QC, ast::DeinitDecl* D,
                        SourceRange Loc = {});

   ast::DeinitDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::DeinitDecl* D);
};

class PrepareFuncArgInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareFuncArgInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareFuncArgInterfaceQuery(QueryContext& QC, ast::FuncArgDecl* D,
                                SourceRange Loc = {});

   ast::FuncArgDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::FuncArgDecl* D);
};

class TypecheckFuncArgQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::TypecheckFuncArgQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckFuncArgQuery(QueryContext& QC, ast::FuncArgDecl* D,
                         SourceRange Loc = {});

   ast::FuncArgDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::FuncArgDecl* D);
};

class PrepareTemplateParamInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareTemplateParamInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareTemplateParamInterfaceQuery(QueryContext& QC,
                                      ast::TemplateParamDecl* D,
                                      SourceRange Loc = {});

   ast::TemplateParamDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::TemplateParamDecl* D);
};

class TypecheckTemplateParamQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::TypecheckTemplateParamQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckTemplateParamQuery(QueryContext& QC, ast::TemplateParamDecl* D,
                               SourceRange Loc = {});

   ast::TemplateParamDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::TemplateParamDecl* D);
};

class PrepareAssociatedTypeInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareAssociatedTypeInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareAssociatedTypeInterfaceQuery(QueryContext& QC,
                                       ast::AssociatedTypeDecl* D,
                                       SourceRange Loc = {});

   ast::AssociatedTypeDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::AssociatedTypeDecl* D);
};

class TypecheckAssociatedTypeQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::TypecheckAssociatedTypeQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckAssociatedTypeQuery(QueryContext& QC, ast::AssociatedTypeDecl* D,
                                SourceRange Loc = {});

   ast::AssociatedTypeDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::AssociatedTypeDecl* D);
};

class PrepareRecordInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareRecordInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareRecordInterfaceQuery(QueryContext& QC, ast::RecordDecl* D,
                               SourceRange Loc = {});

   ast::RecordDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::RecordDecl* D);
};

class TypecheckRecordQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckRecordQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckRecordQuery(QueryContext& QC, ast::RecordDecl* D,
                        SourceRange Loc = {});

   ast::RecordDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::RecordDecl* D);
};

class PrepareStructInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareStructInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareStructInterfaceQuery(QueryContext& QC, ast::StructDecl* D,
                               SourceRange Loc = {});

   ast::StructDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::StructDecl* D);
};

class TypecheckStructQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckStructQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckStructQuery(QueryContext& QC, ast::StructDecl* D,
                        SourceRange Loc = {});

   ast::StructDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::StructDecl* D);
};

class PrepareClassInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareClassInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareClassInterfaceQuery(QueryContext& QC, ast::ClassDecl* D,
                              SourceRange Loc = {});

   ast::ClassDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::ClassDecl* D);
};

class TypecheckClassQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckClassQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckClassQuery(QueryContext& QC, ast::ClassDecl* D,
                       SourceRange Loc = {});

   ast::ClassDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::ClassDecl* D);
};

class PrepareEnumInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareEnumInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareEnumInterfaceQuery(QueryContext& QC, ast::EnumDecl* D,
                             SourceRange Loc = {});

   ast::EnumDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::EnumDecl* D);
};

class TypecheckEnumQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckEnumQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckEnumQuery(QueryContext& QC, ast::EnumDecl* D, SourceRange Loc = {});

   ast::EnumDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::EnumDecl* D);
};

class PrepareProtocolInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareProtocolInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareProtocolInterfaceQuery(QueryContext& QC, ast::ProtocolDecl* D,
                                 SourceRange Loc = {});

   ast::ProtocolDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::ProtocolDecl* D);
};

class TypecheckProtocolQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::TypecheckProtocolQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckProtocolQuery(QueryContext& QC, ast::ProtocolDecl* D,
                          SourceRange Loc = {});

   ast::ProtocolDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::ProtocolDecl* D);
};

class PrepareExtensionInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareExtensionInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareExtensionInterfaceQuery(QueryContext& QC, ast::ExtensionDecl* D,
                                  SourceRange Loc = {});

   ast::ExtensionDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::ExtensionDecl* D);
};

class TypecheckExtensionQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::TypecheckExtensionQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckExtensionQuery(QueryContext& QC, ast::ExtensionDecl* D,
                           SourceRange Loc = {});

   ast::ExtensionDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::ExtensionDecl* D);
};

class PrepareFieldInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareFieldInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareFieldInterfaceQuery(QueryContext& QC, ast::FieldDecl* D,
                              SourceRange Loc = {});

   ast::FieldDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::FieldDecl* D);
};

class TypecheckFieldQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckFieldQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckFieldQuery(QueryContext& QC, ast::FieldDecl* D,
                       SourceRange Loc = {});

   ast::FieldDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::FieldDecl* D);
};

class PreparePropInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PreparePropInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PreparePropInterfaceQuery(QueryContext& QC, ast::PropDecl* D,
                             SourceRange Loc = {});

   ast::PropDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::PropDecl* D);
};

class TypecheckPropQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckPropQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckPropQuery(QueryContext& QC, ast::PropDecl* D, SourceRange Loc = {});

   ast::PropDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::PropDecl* D);
};

class PrepareSubscriptInterfaceQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareSubscriptInterfaceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareSubscriptInterfaceQuery(QueryContext& QC, ast::SubscriptDecl* D,
                                  SourceRange Loc = {});

   ast::SubscriptDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::SubscriptDecl* D);
};

class TypecheckSubscriptQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::TypecheckSubscriptQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckSubscriptQuery(QueryContext& QC, ast::SubscriptDecl* D,
                           SourceRange Loc = {});

   ast::SubscriptDecl* D;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::SubscriptDecl* D);
};

class AssignInitNameQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::AssignInitNameQueryID; }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   AssignInitNameQuery(QueryContext& QC, ast::InitDecl* Init,
                       SourceRange Loc = {});

   ast::InitDecl* Init;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::InitDecl* Init);
};

class LookupFirstQuery : public Query, public llvm::FoldingSetNode {

private:
   unsigned NumDeclsInContext = -1;

public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::LookupFirstQueryID; }

   std::string description() const;
   std::string summary() const;

   const SingleLevelLookupResult* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   void refresh();

   LookupFirstQuery(QueryContext& QC, ast::DeclContext* DC,
                    DeclarationName Name, bool LocalLookup = true,
                    SourceRange Loc = {});

   QueryResult finish(SingleLevelLookupResult&& Result, Status St = Done);

   ast::DeclContext* DC;
   DeclarationName Name;
   bool LocalLookup;
   llvm::Optional<SingleLevelLookupResult> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::DeclContext* DC,
                       DeclarationName Name, bool LocalLookup);
};

class LookupSingleQuery : public Query, public llvm::FoldingSetNode {

private:
   unsigned NumDeclsInContext = -1;

public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::LookupSingleQueryID; }

   std::string description() const;
   std::string summary() const;

   ast::NamedDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   void refresh();

   LookupSingleQuery(QueryContext& QC, ast::DeclContext* DC,
                     DeclarationName Name, bool LocalLookup = true,
                     bool DiagOnError = true, SourceRange Loc = {});

   QueryResult finish(ast::NamedDecl* Result, Status St = Done);

   ast::DeclContext* DC;
   DeclarationName Name;
   bool LocalLookup;
   bool DiagOnError;
   llvm::Optional<ast::NamedDecl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::DeclContext* DC,
                       DeclarationName Name, bool LocalLookup);
};

class MultiLevelLookupQuery : public Query, public llvm::FoldingSetNode {

private:
   unsigned NumDeclsInContext = -1;

public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::MultiLevelLookupQueryID;
   }

   std::string description() const;
   std::string summary() const;

   const MultiLevelLookupResult* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   void refresh();

   MultiLevelLookupQuery(QueryContext& QC, ast::DeclContext* DC,
                         DeclarationName Name, bool LocalLookup = true,
                         SourceRange Loc = {});

   QueryResult finish(MultiLevelLookupResult&& Result, Status St = Done);

   ast::DeclContext* DC;
   DeclarationName Name;
   bool LocalLookup;
   llvm::Optional<MultiLevelLookupResult> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::DeclContext* DC,
                       DeclarationName Name, bool LocalLookup);
};

class DirectLookupQuery : public Query, public llvm::FoldingSetNode {

private:
   unsigned NumDeclsInContext = -1;

public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::DirectLookupQueryID; }

   std::string description() const;
   std::string summary() const;

   const MultiLevelLookupResult* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   void refresh();

   DirectLookupQuery(QueryContext& QC, ast::DeclContext* DC,
                     DeclarationName Name, bool LookInExtensions = true,
                     bool PrepareNameLookup = true, SourceRange Loc = {});

   QueryResult finish(MultiLevelLookupResult&& Result, Status St = Done);

   ast::DeclContext* DC;
   DeclarationName Name;
   bool LookInExtensions;
   bool PrepareNameLookup;
   llvm::Optional<MultiLevelLookupResult> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::DeclContext* DC,
                       DeclarationName Name, bool LookInExtensions,
                       bool PrepareNameLookup);
};

class NestedNameLookupQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::NestedNameLookupQueryID;
   }

   std::string description() const;
   std::string summary() const;

   const MultiLevelLookupResult* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   NestedNameLookupQuery(QueryContext& QC, ast::DeclContext* DC,
                         llvm::ArrayRef<DeclarationName> Names,
                         SourceRange Loc = {});

   QueryResult finish(MultiLevelLookupResult&& Result, Status St = Done);

   ast::DeclContext* DC;
   llvm::ArrayRef<DeclarationName> Names;
   llvm::Optional<MultiLevelLookupResult> Result;
};

class PrepareNameLookupQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareNameLookupQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareNameLookupQuery(QueryContext& QC, ast::DeclContext* DC,
                          SourceRange Loc = {});

   ast::DeclContext* DC;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::DeclContext* DC);
};

class GetAssociatedTypeQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::GetAssociatedTypeQueryID;
   }

   std::string description() const;
   std::string summary() const;

   ast::AssociatedTypeDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetAssociatedTypeQuery(QueryContext& QC, ast::RecordDecl* R,
                          DeclarationName Name,
                          llvm::ArrayRef<ast::ExtensionDecl*> Extensions,
                          SourceRange Loc = {});

   QueryResult finish(ast::AssociatedTypeDecl* Result, Status St = Done);

   ast::RecordDecl* R;
   DeclarationName Name;
   llvm::ArrayRef<ast::ExtensionDecl*> Extensions;
   llvm::Optional<ast::AssociatedTypeDecl*> Result;
};

class CheckTypeEquivalenceQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::CheckTypeEquivalenceQueryID;
   }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckTypeEquivalenceQuery(QueryContext& QC, QualType LHS, QualType RHS,
                             QualType Self, ast::DeclContext* LHSDecl = nullptr,
                             SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   QualType LHS;
   QualType RHS;
   QualType Self;
   ast::DeclContext* LHSDecl;
   llvm::Optional<bool> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType LHS, QualType RHS,
                       QualType Self, ast::DeclContext* LHSDecl);
};

class InstantiateProtocolDefaultImplQuery : public Query,
                                            public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::InstantiateProtocolDefaultImplQueryID;
   }

   std::string description() const;
   std::string summary() const;

   ast::NamedDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   InstantiateProtocolDefaultImplQuery(QueryContext& QC, ast::NamedDecl* Impl,
                                       QualType Self, SourceRange Loc = {});

   QueryResult finish(ast::NamedDecl* Result, Status St = Done);

   ast::NamedDecl* Impl;
   QualType Self;
   llvm::Optional<ast::NamedDecl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::NamedDecl* Impl,
                       QualType Self);
};

class InstantiateRecordQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::InstantiateRecordQueryID;
   }

   std::string description() const;
   std::string summary() const;

   ast::RecordDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   InstantiateRecordQuery(QueryContext& QC, ast::RecordDecl* Template,
                          sema::FinalTemplateArgumentList* TemplateArgs,
                          SourceLocation PointOfInstantiation,
                          SourceRange Loc = {});

   QueryResult finish(ast::RecordDecl* Result, Status St = Done);

   ast::RecordDecl* Template;
   sema::FinalTemplateArgumentList* TemplateArgs;
   SourceLocation PointOfInstantiation;
   llvm::Optional<ast::RecordDecl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::RecordDecl* Template,
                       sema::FinalTemplateArgumentList* TemplateArgs);
};

class CheckTemplateExtensionApplicabilityQuery : public Query,
                                                 public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::CheckTemplateExtensionApplicabilityQueryID;
   }

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckTemplateExtensionApplicabilityQuery(QueryContext& QC,
                                            ast::RecordDecl* Inst,
                                            ast::ExtensionDecl* Ext,
                                            SourceRange Loc = {});

   QueryResult finish(bool Result, Status St = Done);

   ast::RecordDecl* Inst;
   ast::ExtensionDecl* Ext;
   llvm::Optional<bool> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::RecordDecl* Inst,
                       ast::ExtensionDecl* Ext);
};

class InstantiateTemplateMemberQuery : public Query,
                                       public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::InstantiateTemplateMemberQueryID;
   }

   std::string description() const;
   std::string summary() const;

   ast::NamedDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   InstantiateTemplateMemberQuery(QueryContext& QC,
                                  ast::NamedDecl* TemplateMember,
                                  ast::RecordDecl* Inst, SourceRange Loc = {});

   QueryResult finish(ast::NamedDecl* Result, Status St = Done);

   ast::NamedDecl* TemplateMember;
   ast::RecordDecl* Inst;
   llvm::Optional<ast::NamedDecl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID,
                       ast::NamedDecl* TemplateMember, ast::RecordDecl* Inst);
};

class PrepareInstantiationForNameLookupQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::PrepareInstantiationForNameLookupQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareInstantiationForNameLookupQuery(QueryContext& QC,
                                          ast::RecordDecl* Inst,
                                          SourceRange Loc = {});

   ast::RecordDecl* Inst;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::RecordDecl* Inst);
};

class GetILModuleForDeclQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::GetILModuleForDeclQueryID;
   }

   std::string description() const;
   std::string summary() const;

   il::Module* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetILModuleForDeclQuery(QueryContext& QC, ast::Decl* D,
                           SourceRange Loc = {});

   QueryResult finish(il::Module* Result, Status St = Done);

   ast::Decl* D;
   llvm::Optional<il::Module*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::Decl* D);
};

class GenerateILForContextQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::GenerateILForContextQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GenerateILForContextQuery(QueryContext& QC, ast::DeclContext* DC,
                             SourceRange Loc = {});

   ast::DeclContext* DC;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::DeclContext* DC);
};

class GenerateRecordILQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::GenerateRecordILQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GenerateRecordILQuery(QueryContext& QC, ast::RecordDecl* R,
                         SourceRange Loc = {});

   ast::RecordDecl* R;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::RecordDecl* R);
};

class GetILGlobalQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetILGlobalQueryID; }

   std::string description() const;
   std::string summary() const;

   il::GlobalVariable* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetILGlobalQuery(QueryContext& QC, ast::VarDecl* GV, SourceRange Loc = {});

   QueryResult finish(il::GlobalVariable* Result, Status St = Done);

   ast::VarDecl* GV;
   llvm::Optional<il::GlobalVariable*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::VarDecl* GV);
};

class GenerateLazyILGlobalDefinitionQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::GenerateLazyILGlobalDefinitionQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GenerateLazyILGlobalDefinitionQuery(QueryContext& QC, ast::VarDecl* GV,
                                       SourceRange Loc = {});

   ast::VarDecl* GV;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::VarDecl* GV);
};

class GetILFunctionQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetILFunctionQueryID; }

   std::string description() const;
   std::string summary() const;

   il::Function* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetILFunctionQuery(QueryContext& QC, ast::CallableDecl* C,
                      SourceRange Loc = {});

   QueryResult finish(il::Function* Result, Status St = Done);

   ast::CallableDecl* C;
   llvm::Optional<il::Function*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::CallableDecl* C);
};

class GenerateILFunctionBodyQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::GenerateILFunctionBodyQueryID;
   }

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GenerateILFunctionBodyQuery(QueryContext& QC, ast::CallableDecl* C,
                               SourceRange Loc = {});

   ast::CallableDecl* C;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::CallableDecl* C);
};

class GetILTypeInfoQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetILTypeInfoQueryID; }

   std::string description() const;
   std::string summary() const;

   il::GlobalVariable* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetILTypeInfoQuery(QueryContext& QC, QualType T, SourceRange Loc = {});

   QueryResult finish(il::GlobalVariable* Result, Status St = Done);

   QualType T;
   llvm::Optional<il::GlobalVariable*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class CreateILBasicTypeInfoQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::CreateILBasicTypeInfoQueryID;
   }

   std::string description() const;
   std::string summary() const;

   il::Constant* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateILBasicTypeInfoQuery(QueryContext& QC, QualType T,
                              SourceRange Loc = {});

   QueryResult finish(il::Constant* Result, Status St = Done);

   QualType T;
   llvm::Optional<il::Constant*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, QualType T);
};

class CreateILRecordTypeInfoQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::CreateILRecordTypeInfoQueryID;
   }

   std::string description() const;
   std::string summary() const;

   il::Constant* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateILRecordTypeInfoQuery(QueryContext& QC, ast::RecordDecl* R,
                               SourceRange Loc = {});

   QueryResult finish(il::Constant* Result, Status St = Done);

   ast::RecordDecl* R;
   llvm::Optional<il::Constant*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::RecordDecl* R);
};

class OpenFileForReadQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::OpenFileForReadQueryID; }

   std::string description() const;
   std::string summary() const;

   llvm::MemoryBuffer* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   OpenFileForReadQuery(QueryContext& QC, StringRef FileName,
                        SourceRange Loc = {});

   QueryResult finish(std::unique_ptr<llvm::MemoryBuffer>&& Result,
                      Status St = Done);

   StringRef FileName;
   llvm::Optional<std::unique_ptr<llvm::MemoryBuffer>> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, StringRef FileName);
};

class OpenTmpFileQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::OpenTmpFileQueryID; }

   std::string description() const;
   std::string summary() const;

   llvm::raw_ostream* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   OpenTmpFileQuery(QueryContext& QC, StringRef Extension,
                    SourceRange Loc = {});

   QueryResult finish(std::unique_ptr<llvm::raw_ostream>&& Result,
                      Status St = Done);

   StringRef Extension;
   llvm::Optional<std::unique_ptr<llvm::raw_ostream>> Result;
};

class GetDefaultTemplateArgQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::GetDefaultTemplateArgQueryID;
   }

   std::string description() const;
   std::string summary() const;

   const sema::TemplateArgument* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetDefaultTemplateArgQuery(QueryContext& QC, ast::TemplateParamDecl* P,
                              SourceRange Loc = {});

   QueryResult finish(sema::TemplateArgument&& Result, Status St = Done);

   ast::TemplateParamDecl* P;
   llvm::Optional<sema::TemplateArgument> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::TemplateParamDecl* P);
};

class CreateSelfArgumentQuery : public Query {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::CreateSelfArgumentQueryID;
   }

   std::string description() const;
   std::string summary() const;

   ast::FuncArgDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateSelfArgumentQuery(QueryContext& QC, QualType Self,
                           SourceLocation SelfLoc, SourceRange Loc = {});

   QueryResult finish(ast::FuncArgDecl* Result, Status St = Done);

   QualType Self;
   SourceLocation SelfLoc;
   llvm::Optional<ast::FuncArgDecl*> Result;
};

class FindEquivalentDeclQuery : public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::FindEquivalentDeclQueryID;
   }

   std::string description() const;
   std::string summary() const;

   ast::NamedDecl* get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   FindEquivalentDeclQuery(QueryContext& QC, ast::NamedDecl* Decl,
                           ast::DeclContext* DC, QualType Self = QualType(),
                           bool LookInExtensions = true, SourceRange Loc = {});

   QueryResult finish(ast::NamedDecl* Result, Status St = Done);

   ast::NamedDecl* Decl;
   ast::DeclContext* DC;
   QualType Self;
   bool LookInExtensions;
   llvm::Optional<ast::NamedDecl*> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::NamedDecl* Decl,
                       ast::DeclContext* DC, QualType Self,
                       bool LookInExtensions);
};

class EquivalentSignaturesQuery : public Query, public llvm::FoldingSetNode {

public:
   enum ResultKind {

      Equivalent,

      ReturnTypeMismatch,

      ParamCountMismatch,

      ParamTypeMismatch,

      LabelMismatch,

      ThrowsMismatch,

      UnsafeMismatch,

      AsyncMismatch,

      StaticMismatch
   };

public:
   static bool classof(const Query* Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K)
   {
      return K == Kind::EquivalentSignaturesQueryID;
   }

   std::string description() const;
   std::string summary() const;

   EquivalentSignaturesQuery::ResultKind get() const;

   friend class Query; // for run().

   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   EquivalentSignaturesQuery(QueryContext& QC, ast::CallableDecl* C1,
                             ast::CallableDecl* C2, QualType Self,
                             SourceRange Loc = {});

   QueryResult finish(EquivalentSignaturesQuery::ResultKind Result,
                      Status St = Done);

   ast::CallableDecl* C1;
   ast::CallableDecl* C2;
   QualType Self;
   llvm::Optional<EquivalentSignaturesQuery::ResultKind> Result;

public:
   void Profile(llvm::FoldingSetNodeID& ID) const;
   static void Profile(llvm::FoldingSetNodeID& ID, ast::CallableDecl* C1,
                       ast::CallableDecl* C2, QualType Self);
};

#endif

#ifdef CDOT_QUERY_IMPL
CompileModuleQuery::CompileModuleQuery(QueryContext& QC, SourceRange Loc)
    : Query(CompileModuleQueryID, QC, Loc)
{
}

std::string CompileModuleQuery::description() const
{
   std::string s;
   s += "compiling module ";
   s += QC.CI.getCompilationModule()->getName()->getIdentifier();
   return s;
}

std::string CompileModuleQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CompileModule]";
   ;

   return OS.str();
}

ParseSourceFileQuery::ParseSourceFileQuery(QueryContext& QC, StringRef FileName,
                                           SourceRange Loc)
    : Query(ParseSourceFileQueryID, QC, Loc), FileName(FileName)
{
}

ast::ModuleDecl* ParseSourceFileQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ParseSourceFileQuery::description() const
{
   std::string s;
   s += "parsing source file ";
   s += this->FileName;
   return s;
}

std::string ParseSourceFileQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ParseSourceFile]";
   OS << " FileName = ";
   OS << "'" << FileName << "'";
   ;

   return OS.str();
}

QueryResult ParseSourceFileQuery::finish(ast::ModuleDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ParseSourceFileQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, FileName);
}

void ParseSourceFileQuery::Profile(llvm::FoldingSetNodeID& ID,
                                   StringRef FileName)
{
   ID.AddString(FileName);
}

TypeCheckASTQuery::TypeCheckASTQuery(QueryContext& QC, ast::ModuleDecl* Mod,
                                     SourceRange Loc)
    : Query(TypeCheckASTQueryID, QC, Loc), Mod(Mod)
{
}

std::string TypeCheckASTQuery::description() const
{
   std::string s;
   s += "typechecking module '";
   s += Mod->getFullName();
   s += "'";
   return s;
}

std::string TypeCheckASTQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypeCheckAST]";
   OS << " Mod = ";
   OS << "'" << Mod->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypeCheckASTQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Mod);
}

void TypeCheckASTQuery::Profile(llvm::FoldingSetNodeID& ID,
                                ast::ModuleDecl* Mod)
{
   ID.AddPointer(Mod);
}

CreateILModuleQuery::CreateILModuleQuery(QueryContext& QC,
                                         llvm::ArrayRef<StringRef> SourceFiles,
                                         SourceRange Loc)
    : Query(CreateILModuleQueryID, QC, Loc), SourceFiles(SourceFiles)
{
}

il::Module* CreateILModuleQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string CreateILModuleQuery::description() const
{
   std::string s;
   s += "creating IL module";
   return s;
}

std::string CreateILModuleQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CreateILModule]";
   OS << " SourceFiles = ";
   OS << '[';
   unsigned i = 0;
   for (auto& El : SourceFiles) {
      if (i++ != 0)
         OS << ", ";
      OS << "'" << El << "'";
   }
   OS << ']';

   ;

   return OS.str();
}

QueryResult CreateILModuleQuery::finish(il::Module* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CreateILModuleQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, SourceFiles);
}

void CreateILModuleQuery::Profile(llvm::FoldingSetNodeID& ID,
                                  llvm::ArrayRef<StringRef> SourceFiles)
{
   for (auto& El : SourceFiles)
      ID.AddString(El);
}

SetupIRGenQuery::SetupIRGenQuery(QueryContext& QC, SourceRange Loc)
    : Query(SetupIRGenQueryID, QC, Loc)
{
}

il::IRGen* SetupIRGenQuery::get() const
{
   assert(ok() && "query has not successfully completed!");

   return Result.getValue().get();
}

std::string SetupIRGenQuery::description() const
{
   std::string s;
   s += "setting up IRGen";
   return s;
}

std::string SetupIRGenQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[SetupIRGen]";
   ;

   return OS.str();
}

QueryResult SetupIRGenQuery::finish(std::unique_ptr<il::IRGen>&& Result,
                                    Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

CreateLLVMModuleQuery::CreateLLVMModuleQuery(
    QueryContext& QC, llvm::ArrayRef<StringRef> SourceFiles, SourceRange Loc)
    : Query(CreateLLVMModuleQueryID, QC, Loc), SourceFiles(SourceFiles)
{
}

llvm::Module* CreateLLVMModuleQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string CreateLLVMModuleQuery::description() const
{
   std::string s;
   s += "creating LLVM module";
   return s;
}

std::string CreateLLVMModuleQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CreateLLVMModule]";
   OS << " SourceFiles = ";
   OS << '[';
   unsigned i = 0;
   for (auto& El : SourceFiles) {
      if (i++ != 0)
         OS << ", ";
      OS << "'" << El << "'";
   }
   OS << ']';

   ;

   return OS.str();
}

QueryResult CreateLLVMModuleQuery::finish(llvm::Module* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CreateLLVMModuleQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, SourceFiles);
}

void CreateLLVMModuleQuery::Profile(llvm::FoldingSetNodeID& ID,
                                    llvm::ArrayRef<StringRef> SourceFiles)
{
   for (auto& El : SourceFiles)
      ID.AddString(El);
}

CreateObjectQuery::CreateObjectQuery(QueryContext& QC,
                                     llvm::ArrayRef<StringRef> SourceFiles,
                                     llvm::raw_ostream& OS, SourceRange Loc)
    : Query(CreateObjectQueryID, QC, Loc), SourceFiles(SourceFiles), OS(OS)
{
}

std::string CreateObjectQuery::description() const
{
   std::string s;
   s += "creating object file";
   return s;
}

std::string CreateObjectQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CreateObject]";
   OS << " SourceFiles = ";
   OS << '[';
   unsigned i = 0;
   for (auto& El : SourceFiles) {
      if (i++ != 0)
         OS << ", ";
      OS << "'" << El << "'";
   }
   OS << ']';

   OS << ',';
   OS << " OS = ";
   OS << &OS;
   ;

   return OS.str();
}

CreateStaticLibQuery::CreateStaticLibQuery(
    QueryContext& QC, llvm::ArrayRef<StringRef> SourceFiles, StringRef OutFile,
    SourceRange Loc)
    : Query(CreateStaticLibQueryID, QC, Loc), SourceFiles(SourceFiles),
      OutFile(OutFile)
{
}

std::string CreateStaticLibQuery::description() const
{
   std::string s;
   s += "creating static library '";
   s += this->OutFile;
   s += "'";
   return s;
}

std::string CreateStaticLibQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CreateStaticLib]";
   OS << " SourceFiles = ";
   OS << '[';
   unsigned i = 0;
   for (auto& El : SourceFiles) {
      if (i++ != 0)
         OS << ", ";
      OS << "'" << El << "'";
   }
   OS << ']';

   OS << ',';
   OS << " OutFile = ";
   OS << "'" << OutFile << "'";
   ;

   return OS.str();
}

CreateDynamicLibQuery::CreateDynamicLibQuery(
    QueryContext& QC, llvm::ArrayRef<StringRef> SourceFiles, StringRef OutFile,
    SourceRange Loc)
    : Query(CreateDynamicLibQueryID, QC, Loc), SourceFiles(SourceFiles),
      OutFile(OutFile)
{
}

std::string CreateDynamicLibQuery::description() const
{
   std::string s;
   s += "creating dynamic library '";
   s += this->OutFile;
   s += "'";
   return s;
}

std::string CreateDynamicLibQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CreateDynamicLib]";
   OS << " SourceFiles = ";
   OS << '[';
   unsigned i = 0;
   for (auto& El : SourceFiles) {
      if (i++ != 0)
         OS << ", ";
      OS << "'" << El << "'";
   }
   OS << ']';

   OS << ',';
   OS << " OutFile = ";
   OS << "'" << OutFile << "'";
   ;

   return OS.str();
}

CreateExecutableQuery::CreateExecutableQuery(
    QueryContext& QC, llvm::ArrayRef<StringRef> SourceFiles, StringRef OutFile,
    SourceRange Loc)
    : Query(CreateExecutableQueryID, QC, Loc), SourceFiles(SourceFiles),
      OutFile(OutFile)
{
}

std::string CreateExecutableQuery::description() const
{
   std::string s;
   s += "creating executable '";
   s += this->OutFile;
   s += "'";
   return s;
}

std::string CreateExecutableQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CreateExecutable]";
   OS << " SourceFiles = ";
   OS << '[';
   unsigned i = 0;
   for (auto& El : SourceFiles) {
      if (i++ != 0)
         OS << ", ";
      OS << "'" << El << "'";
   }
   OS << ']';

   OS << ',';
   OS << " OutFile = ";
   OS << "'" << OutFile << "'";
   ;

   return OS.str();
}

EmitILQuery::EmitILQuery(QueryContext& QC,
                         llvm::ArrayRef<StringRef> SourceFiles,
                         llvm::raw_ostream& OS, SourceRange Loc)
    : Query(EmitILQueryID, QC, Loc), SourceFiles(SourceFiles), OS(OS)
{
}

std::string EmitILQuery::description() const
{
   std::string s;
   s += "emitting IL module";
   return s;
}

std::string EmitILQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[EmitIL]";
   OS << " SourceFiles = ";
   OS << '[';
   unsigned i = 0;
   for (auto& El : SourceFiles) {
      if (i++ != 0)
         OS << ", ";
      OS << "'" << El << "'";
   }
   OS << ']';

   OS << ',';
   OS << " OS = ";
   OS << &OS;
   ;

   return OS.str();
}

EmitIRQuery::EmitIRQuery(QueryContext& QC,
                         llvm::ArrayRef<StringRef> SourceFiles,
                         llvm::raw_ostream& OS, SourceRange Loc)
    : Query(EmitIRQueryID, QC, Loc), SourceFiles(SourceFiles), OS(OS)
{
}

std::string EmitIRQuery::description() const
{
   std::string s;
   s += "emitting IR module";
   return s;
}

std::string EmitIRQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[EmitIR]";
   OS << " SourceFiles = ";
   OS << '[';
   unsigned i = 0;
   for (auto& El : SourceFiles) {
      if (i++ != 0)
         OS << ", ";
      OS << "'" << El << "'";
   }
   OS << ']';

   OS << ',';
   OS << " OS = ";
   OS << &OS;
   ;

   return OS.str();
}

PrintUsedMemoryQuery::PrintUsedMemoryQuery(QueryContext& QC, SourceRange Loc)
    : Query(PrintUsedMemoryQueryID, QC, Loc)
{
}

std::string PrintUsedMemoryQuery::description() const
{
   std::string s;
   s += "printing used memory";
   return s;
}

std::string PrintUsedMemoryQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrintUsedMemory]";
   ;

   return OS.str();
}

MeasureExecutionTimeQuery::MeasureExecutionTimeQuery(QueryContext& QC, Query* Q,
                                                     SourceRange Loc)
    : Query(MeasureExecutionTimeQueryID, QC, Loc), Q(Q)
{
}

long long MeasureExecutionTimeQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string MeasureExecutionTimeQuery::description() const
{
   std::string s;
   s += "measuring execution time of query '";
   s += Q->description();
   s += "'";
   return s;
}

std::string MeasureExecutionTimeQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[MeasureExecutionTime]";
   OS << " Q = ";
   OS << Q;
   ;

   return OS.str();
}

QueryResult MeasureExecutionTimeQuery::finish(long long Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void MeasureExecutionTimeQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Q);
}

void MeasureExecutionTimeQuery::Profile(llvm::FoldingSetNodeID& ID, Query* Q)
{
   ID.AddPointer(Q);
}

IsEquatableQuery::IsEquatableQuery(QueryContext& QC, QualType T,
                                   SourceRange Loc)
    : Query(IsEquatableQueryID, QC, Loc), T(T)
{
}

bool IsEquatableQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsEquatableQuery::description() const
{
   std::string s;
   s += "checking ";
   s += "IsEquatable";
   s += " on ";
   s += T.toDiagString();
   return s;
}

std::string IsEquatableQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsEquatable]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

QueryResult IsEquatableQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsEquatableQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void IsEquatableQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

IsCopyableQuery::IsCopyableQuery(QueryContext& QC, QualType T, SourceRange Loc)
    : Query(IsCopyableQueryID, QC, Loc), T(T)
{
}

bool IsCopyableQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsCopyableQuery::description() const
{
   std::string s;
   s += "checking ";
   s += "IsCopyable";
   s += " on ";
   s += T.toDiagString();
   return s;
}

std::string IsCopyableQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsCopyable]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

QueryResult IsCopyableQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsCopyableQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void IsCopyableQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

IsMoveOnlyQuery::IsMoveOnlyQuery(QueryContext& QC, QualType T, SourceRange Loc)
    : Query(IsMoveOnlyQueryID, QC, Loc), T(T)
{
}

bool IsMoveOnlyQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsMoveOnlyQuery::description() const
{
   std::string s;
   s += "checking ";
   s += "IsMoveOnly";
   s += " on ";
   s += T.toDiagString();
   return s;
}

std::string IsMoveOnlyQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsMoveOnly]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

QueryResult IsMoveOnlyQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsMoveOnlyQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void IsMoveOnlyQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

IsImplicitlyCopyableQuery::IsImplicitlyCopyableQuery(QueryContext& QC,
                                                     QualType T,
                                                     SourceRange Loc)
    : Query(IsImplicitlyCopyableQueryID, QC, Loc), T(T)
{
}

bool IsImplicitlyCopyableQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsImplicitlyCopyableQuery::description() const
{
   std::string s;
   s += "checking ";
   s += "IsImplicitlyCopyable";
   s += " on ";
   s += T.toDiagString();
   return s;
}

std::string IsImplicitlyCopyableQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsImplicitlyCopyable]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

QueryResult IsImplicitlyCopyableQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsImplicitlyCopyableQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void IsImplicitlyCopyableQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

IsTriviallyCopyableQuery::IsTriviallyCopyableQuery(QueryContext& QC, QualType T,
                                                   SourceRange Loc)
    : Query(IsTriviallyCopyableQueryID, QC, Loc), T(T)
{
}

bool IsTriviallyCopyableQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsTriviallyCopyableQuery::description() const
{
   std::string s;
   s += "checking ";
   s += "IsTriviallyCopyable";
   s += " on ";
   s += T.toDiagString();
   return s;
}

std::string IsTriviallyCopyableQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsTriviallyCopyable]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

QueryResult IsTriviallyCopyableQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsTriviallyCopyableQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void IsTriviallyCopyableQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

NeedsRetainOrReleaseQuery::NeedsRetainOrReleaseQuery(QueryContext& QC,
                                                     QualType T,
                                                     SourceRange Loc)
    : Query(NeedsRetainOrReleaseQueryID, QC, Loc), T(T)
{
}

bool NeedsRetainOrReleaseQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string NeedsRetainOrReleaseQuery::description() const
{
   std::string s;
   s += "checking ";
   s += "NeedsRetainOrRelease";
   s += " on ";
   s += T.toDiagString();
   return s;
}

std::string NeedsRetainOrReleaseQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[NeedsRetainOrRelease]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

QueryResult NeedsRetainOrReleaseQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void NeedsRetainOrReleaseQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void NeedsRetainOrReleaseQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

NeedsDeinitilizationQuery::NeedsDeinitilizationQuery(QueryContext& QC,
                                                     QualType T,
                                                     SourceRange Loc)
    : Query(NeedsDeinitilizationQueryID, QC, Loc), T(T)
{
}

bool NeedsDeinitilizationQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string NeedsDeinitilizationQuery::description() const
{
   std::string s;
   s += "checking ";
   s += "NeedsDeinitilization";
   s += " on ";
   s += T.toDiagString();
   return s;
}

std::string NeedsDeinitilizationQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[NeedsDeinitilization]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

QueryResult NeedsDeinitilizationQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void NeedsDeinitilizationQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void NeedsDeinitilizationQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

NeedsStructReturnQuery::NeedsStructReturnQuery(QueryContext& QC, QualType T,
                                               SourceRange Loc)
    : Query(NeedsStructReturnQueryID, QC, Loc), T(T)
{
}

bool NeedsStructReturnQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string NeedsStructReturnQuery::description() const
{
   std::string s;
   s += "checking ";
   s += "NeedsStructReturn";
   s += " on ";
   s += T.toDiagString();
   return s;
}

std::string NeedsStructReturnQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[NeedsStructReturn]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

QueryResult NeedsStructReturnQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void NeedsStructReturnQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void NeedsStructReturnQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

SubstAssociatedTypesQuery::SubstAssociatedTypesQuery(QueryContext& QC,
                                                     QualType T, QualType Self,
                                                     SourceRange Loc)
    : Query(SubstAssociatedTypesQueryID, QC, Loc), T(T), Self(Self)
{
}

QualType SubstAssociatedTypesQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string SubstAssociatedTypesQuery::description() const
{
   std::string s;
   s += "substituting ";
   s += T.toDiagString();
   s += " with Self = ";
   s += Self.toDiagString();
   return s;
}

std::string SubstAssociatedTypesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[SubstAssociatedTypes]";
   OS << " T = ";
   OS << "'" << T << "'";
   OS << ',';
   OS << " Self = ";
   OS << "'" << Self << "'";
   ;

   return OS.str();
}

QueryResult SubstAssociatedTypesQuery::finish(QualType Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void SubstAssociatedTypesQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T, Self);
}

void SubstAssociatedTypesQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T,
                                        QualType Self)
{
   ID.AddPointer(T.getAsOpaquePtr());
   ID.AddPointer(Self.getAsOpaquePtr());
}

SubstTemplateParamTypesQuery::SubstTemplateParamTypesQuery(
    QueryContext& QC, QualType T,
    sema::MultiLevelFinalTemplateArgList TemplateArgs, SourceRange Loc)
    : Query(SubstTemplateParamTypesQueryID, QC, Loc), T(T),
      TemplateArgs(std::move(TemplateArgs))
{
}

QualType SubstTemplateParamTypesQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string SubstTemplateParamTypesQuery::description() const
{
   std::string s;
   s += "substituting ";
   s += T.toDiagString();
   s += " with TemplateArgs = ";
   s += TemplateArgs.toString();
   return s;
}

std::string SubstTemplateParamTypesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[SubstTemplateParamTypes]";
   OS << " T = ";
   OS << "'" << T << "'";
   OS << ',';
   OS << " TemplateArgs = ";
   OS << TemplateArgs.toString();
   ;

   return OS.str();
}

QueryResult SubstTemplateParamTypesQuery::finish(QualType Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void SubstTemplateParamTypesQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T, TemplateArgs);
}

void SubstTemplateParamTypesQuery::Profile(
    llvm::FoldingSetNodeID& ID, QualType T,
    const sema::MultiLevelFinalTemplateArgList& TemplateArgs)
{
   ID.AddPointer(T.getAsOpaquePtr());
   TemplateArgs.Profile(ID);
}

GetBuiltinModuleQuery::GetBuiltinModuleQuery(
    QueryContext& QC, GetBuiltinModuleQuery::ModuleKind Mod, SourceRange Loc)
    : Query(GetBuiltinModuleQueryID, QC, Loc), Mod(Mod)
{
}

Module* GetBuiltinModuleQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetBuiltinModuleQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetBuiltinModule";
   return s;
}

std::string GetBuiltinModuleQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetBuiltinModule]";
   OS << " Mod = ";
   OS << (uint64_t)Mod;
   ;

   return OS.str();
}

QueryResult GetBuiltinModuleQuery::finish(Module* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetBuiltinModuleQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Mod);
}

void GetBuiltinModuleQuery::Profile(llvm::FoldingSetNodeID& ID,
                                    GetBuiltinModuleQuery::ModuleKind Mod)
{
   ID.AddInteger((uint64_t)Mod);
}

GetBuiltinFuncQuery::GetBuiltinFuncQuery(QueryContext& QC,
                                         GetBuiltinFuncQuery::FunctionKind Fn,
                                         SourceRange Loc)
    : Query(GetBuiltinFuncQueryID, QC, Loc), Fn(Fn)
{
}

ast::CallableDecl* GetBuiltinFuncQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetBuiltinFuncQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetBuiltinFunc";
   return s;
}

std::string GetBuiltinFuncQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetBuiltinFunc]";
   OS << " Fn = ";
   OS << (uint64_t)Fn;
   ;

   return OS.str();
}

QueryResult GetBuiltinFuncQuery::finish(ast::CallableDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetBuiltinFuncQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Fn);
}

void GetBuiltinFuncQuery::Profile(llvm::FoldingSetNodeID& ID,
                                  GetBuiltinFuncQuery::FunctionKind Fn)
{
   ID.AddInteger((uint64_t)Fn);
}

GetBuiltinRecordQuery::GetBuiltinRecordQuery(
    QueryContext& QC, GetBuiltinRecordQuery::RecordKind R, SourceRange Loc)
    : Query(GetBuiltinRecordQueryID, QC, Loc), R(R)
{
}

ast::RecordDecl* GetBuiltinRecordQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetBuiltinRecordQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetBuiltinRecord";
   return s;
}

std::string GetBuiltinRecordQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetBuiltinRecord]";
   OS << " R = ";
   OS << (uint64_t)R;
   ;

   return OS.str();
}

QueryResult GetBuiltinRecordQuery::finish(ast::RecordDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetBuiltinRecordQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, R);
}

void GetBuiltinRecordQuery::Profile(llvm::FoldingSetNodeID& ID,
                                    GetBuiltinRecordQuery::RecordKind R)
{
   ID.AddInteger((uint64_t)R);
}

GetBuiltinProtocolQuery::GetBuiltinProtocolQuery(
    QueryContext& QC, GetBuiltinProtocolQuery::ProtocolKind P, SourceRange Loc)
    : Query(GetBuiltinProtocolQueryID, QC, Loc), P(P)
{
}

ast::ProtocolDecl* GetBuiltinProtocolQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetBuiltinProtocolQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetBuiltinProtocol";
   return s;
}

std::string GetBuiltinProtocolQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetBuiltinProtocol]";
   OS << " P = ";
   OS << (uint64_t)P;
   ;

   return OS.str();
}

QueryResult GetBuiltinProtocolQuery::finish(ast::ProtocolDecl* Result,
                                            Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetBuiltinProtocolQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, P);
}

void GetBuiltinProtocolQuery::Profile(llvm::FoldingSetNodeID& ID,
                                      GetBuiltinProtocolQuery::ProtocolKind P)
{
   ID.AddInteger((uint64_t)P);
}

FindPrecedenceGroupQuery::FindPrecedenceGroupQuery(QueryContext& QC,
                                                   DeclarationName Name,
                                                   bool DiagOnError,
                                                   SourceRange Loc)
    : Query(FindPrecedenceGroupQueryID, QC, Loc), Name(Name),
      DiagOnError(DiagOnError)
{
}

ast::PrecedenceGroupDecl* FindPrecedenceGroupQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string FindPrecedenceGroupQuery::description() const
{
   std::string s;
   s += "looking up precedence group '";
   s += Name.toString();
   s += "'";
   return s;
}

std::string FindPrecedenceGroupQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[FindPrecedenceGroup]";
   OS << " Name = ";
   OS << "'" << Name << "'";
   OS << ',';
   OS << " DiagOnError = ";
   OS << DiagOnError;
   ;

   return OS.str();
}

QueryResult FindPrecedenceGroupQuery::finish(ast::PrecedenceGroupDecl* Result,
                                             Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void FindPrecedenceGroupQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Name, DiagOnError);
}

void FindPrecedenceGroupQuery::Profile(llvm::FoldingSetNodeID& ID,
                                       DeclarationName Name, bool DiagOnError)
{
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddInteger(DiagOnError);
}

FindOperatorQuery::FindOperatorQuery(QueryContext& QC, DeclarationName Name,
                                     ast::DeclContext* DC, bool DiagOnError,
                                     SourceRange Loc)
    : Query(FindOperatorQueryID, QC, Loc), Name(Name), DC(DC),
      DiagOnError(DiagOnError)
{
}

ast::OperatorDecl* FindOperatorQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string FindOperatorQuery::description() const
{
   std::string s;
   s += "looking up operator '";
   s += Name.toString();
   s += "'";
   return s;
}

std::string FindOperatorQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[FindOperator]";
   OS << " Name = ";
   OS << "'" << Name << "'";
   OS << ',';
   OS << " DC = ";
   OS << "'" << DC->getNameAsString() << "'";
   OS << ',';
   OS << " DiagOnError = ";
   OS << DiagOnError;
   ;

   return OS.str();
}

QueryResult FindOperatorQuery::finish(ast::OperatorDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void FindOperatorQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Name, DC, DiagOnError);
}

void FindOperatorQuery::Profile(llvm::FoldingSetNodeID& ID,
                                DeclarationName Name, ast::DeclContext* DC,
                                bool DiagOnError)
{
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddPointer(DC);
   ID.AddInteger(DiagOnError);
}

ResolveImportQuery::ResolveImportQuery(QueryContext& QC, ast::ImportDecl* I,
                                       SourceRange Loc)
    : Query(ResolveImportQueryID, QC, Loc), I(I)
{
}

std::string ResolveImportQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveImport";
   return s;
}

std::string ResolveImportQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveImport]";
   OS << " I = ";
   OS << "'" << I->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void ResolveImportQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, I);
}

void ResolveImportQuery::Profile(llvm::FoldingSetNodeID& ID, ast::ImportDecl* I)
{
   ID.AddPointer(I);
}

ResolveUsingQuery::ResolveUsingQuery(QueryContext& QC, ast::UsingDecl* U,
                                     SourceRange Loc)
    : Query(ResolveUsingQueryID, QC, Loc), U(U)
{
}

std::string ResolveUsingQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveUsing";
   return s;
}

std::string ResolveUsingQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveUsing]";
   OS << " U = ";
   OS << "'" << U->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void ResolveUsingQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, U);
}

void ResolveUsingQuery::Profile(llvm::FoldingSetNodeID& ID, ast::UsingDecl* U)
{
   ID.AddPointer(U);
}

ResolveStaticIfQuery::ResolveStaticIfQuery(QueryContext& QC,
                                           ast::StaticIfDecl* Decl,
                                           SourceRange Loc)
    : Query(ResolveStaticIfQueryID, QC, Loc), Decl(Decl)
{
}

ast::Decl* ResolveStaticIfQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ResolveStaticIfQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveStaticIf";
   return s;
}

std::string ResolveStaticIfQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveStaticIf]";
   OS << " Decl = ";
   OS << "'" << Decl->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult ResolveStaticIfQuery::finish(ast::Decl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ResolveStaticIfQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Decl);
}

void ResolveStaticIfQuery::Profile(llvm::FoldingSetNodeID& ID,
                                   ast::StaticIfDecl* Decl)
{
   ID.AddPointer(Decl);
}

ResolveStaticForQuery::ResolveStaticForQuery(QueryContext& QC,
                                             ast::StaticForDecl* Decl,
                                             SourceRange Loc)
    : Query(ResolveStaticForQueryID, QC, Loc), Decl(Decl)
{
}

ast::Decl* ResolveStaticForQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ResolveStaticForQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveStaticFor";
   return s;
}

std::string ResolveStaticForQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveStaticFor]";
   OS << " Decl = ";
   OS << "'" << Decl->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult ResolveStaticForQuery::finish(ast::Decl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ResolveStaticForQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Decl);
}

void ResolveStaticForQuery::Profile(llvm::FoldingSetNodeID& ID,
                                    ast::StaticForDecl* Decl)
{
   ID.AddPointer(Decl);
}

ResolveStaticAssertQuery::ResolveStaticAssertQuery(QueryContext& QC,
                                                   ast::StaticAssertDecl* Decl,
                                                   SourceRange Loc)
    : Query(ResolveStaticAssertQueryID, QC, Loc), Decl(Decl)
{
}

std::string ResolveStaticAssertQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveStaticAssert";
   return s;
}

std::string ResolveStaticAssertQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveStaticAssert]";
   OS << " Decl = ";
   OS << "'" << Decl->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void ResolveStaticAssertQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Decl);
}

void ResolveStaticAssertQuery::Profile(llvm::FoldingSetNodeID& ID,
                                       ast::StaticAssertDecl* Decl)
{
   ID.AddPointer(Decl);
}

ResolveStaticPrintQuery::ResolveStaticPrintQuery(QueryContext& QC,
                                                 ast::StaticPrintDecl* Decl,
                                                 SourceRange Loc)
    : Query(ResolveStaticPrintQueryID, QC, Loc), Decl(Decl)
{
}

std::string ResolveStaticPrintQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveStaticPrint";
   return s;
}

std::string ResolveStaticPrintQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveStaticPrint]";
   OS << " Decl = ";
   OS << "'" << Decl->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void ResolveStaticPrintQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Decl);
}

void ResolveStaticPrintQuery::Profile(llvm::FoldingSetNodeID& ID,
                                      ast::StaticPrintDecl* Decl)
{
   ID.AddPointer(Decl);
}

ResolveStaticExprQuery::ResolveStaticExprQuery(QueryContext& QC,
                                               ast::StaticExpr* Expr,
                                               SourceRange Loc)
    : Query(ResolveStaticExprQueryID, QC, Loc), Expr(Expr)
{
}

il::Constant* ResolveStaticExprQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ResolveStaticExprQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveStaticExpr";
   return s;
}

std::string ResolveStaticExprQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveStaticExpr]";
   OS << " Expr = ";
   OS << Expr;
   ;

   return OS.str();
}

QueryResult ResolveStaticExprQuery::finish(il::Constant* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ResolveStaticExprQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Expr);
}

void ResolveStaticExprQuery::Profile(llvm::FoldingSetNodeID& ID,
                                     ast::StaticExpr* Expr)
{
   ID.AddPointer(Expr);
}

ResolveStaticExprToBoolQuery::ResolveStaticExprToBoolQuery(
    QueryContext& QC, ast::StaticExpr* Expr, bool DiagOnError, SourceRange Loc)
    : Query(ResolveStaticExprToBoolQueryID, QC, Loc), Expr(Expr),
      DiagOnError(DiagOnError)
{
}

bool ResolveStaticExprToBoolQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ResolveStaticExprToBoolQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveStaticExprToBool";
   return s;
}

std::string ResolveStaticExprToBoolQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveStaticExprToBool]";
   OS << " Expr = ";
   OS << Expr;
   OS << ',';
   OS << " DiagOnError = ";
   OS << DiagOnError;
   ;

   return OS.str();
}

QueryResult ResolveStaticExprToBoolQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ResolveStaticExprToBoolQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Expr, DiagOnError);
}

void ResolveStaticExprToBoolQuery::Profile(llvm::FoldingSetNodeID& ID,
                                           ast::StaticExpr* Expr,
                                           bool DiagOnError)
{
   ID.AddPointer(Expr);
   ID.AddInteger(DiagOnError);
}

ExpandMacroQuery::ExpandMacroQuery(QueryContext& QC, StmtOrDecl SOD,
                                   DeclarationName Name, ast::DeclContext* DC,
                                   unsigned Delim,
                                   llvm::ArrayRef<lex::Token> Tokens,
                                   unsigned ExpectedKind, SourceRange Loc)
    : Query(ExpandMacroQueryID, QC, Loc), SOD(SOD), Name(Name), DC(DC),
      Delim(Delim), Tokens(Tokens), ExpectedKind(ExpectedKind)
{
}

StmtOrDecl ExpandMacroQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ExpandMacroQuery::description() const
{
   std::string s;
   s += "expanding macro '";
   s += Name.toString();
   s += "'";
   return s;
}

std::string ExpandMacroQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ExpandMacro]";
   OS << " SOD = ";
   OS << SOD.getAsOpaquePtr();
   OS << ',';
   OS << " Name = ";
   OS << "'" << Name << "'";
   OS << ',';
   OS << " DC = ";
   OS << "'" << DC->getNameAsString() << "'";
   OS << ',';
   OS << " Delim = ";
   OS << Delim;
   OS << ',';
   OS << " Tokens = ";
   OS << '[';
   unsigned i = 0;
   for (auto& El : Tokens) {
      if (i++ != 0)
         OS << ", ";
      OS << El;
   }
   OS << ']';

   OS << ',';
   OS << " ExpectedKind = ";
   OS << ExpectedKind;
   ;

   return OS.str();
}

QueryResult ExpandMacroQuery::finish(StmtOrDecl Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ExpandMacroQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, SOD, Name, DC, Delim, Tokens, ExpectedKind);
}

void ExpandMacroQuery::Profile(llvm::FoldingSetNodeID& ID, StmtOrDecl SOD,
                               DeclarationName Name, ast::DeclContext* DC,
                               unsigned Delim,
                               llvm::ArrayRef<lex::Token> Tokens,
                               unsigned ExpectedKind)
{
   ID.AddPointer(SOD.getAsOpaquePtr());
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddPointer(DC);
   ID.AddInteger(Delim);
   for (auto& El : Tokens)
      El.Profile(ID);
   ID.AddInteger(ExpectedKind);
}

ExpandMacroDeclQuery::ExpandMacroDeclQuery(QueryContext& QC,
                                           ast::MacroExpansionDecl* Decl,
                                           SourceRange Loc)
    : Query(ExpandMacroDeclQueryID, QC, Loc), Decl(Decl)
{
}

ast::Decl* ExpandMacroDeclQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ExpandMacroDeclQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ExpandMacroDecl";
   return s;
}

std::string ExpandMacroDeclQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ExpandMacroDecl]";
   OS << " Decl = ";
   OS << "'" << Decl->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult ExpandMacroDeclQuery::finish(ast::Decl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ExpandMacroDeclQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Decl);
}

void ExpandMacroDeclQuery::Profile(llvm::FoldingSetNodeID& ID,
                                   ast::MacroExpansionDecl* Decl)
{
   ID.AddPointer(Decl);
}

ExpandMacrosQuery::ExpandMacrosQuery(QueryContext& QC, StringRef SourceFile,
                                     SourceRange Loc)
    : Query(ExpandMacrosQueryID, QC, Loc), SourceFile(SourceFile)
{
}

std::string ExpandMacrosQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ExpandMacros";
   return s;
}

std::string ExpandMacrosQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ExpandMacros]";
   OS << " SourceFile = ";
   OS << "'" << SourceFile << "'";
   ;

   return OS.str();
}

void ExpandMacrosQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, SourceFile);
}

void ExpandMacrosQuery::Profile(llvm::FoldingSetNodeID& ID,
                                StringRef SourceFile)
{
   ID.AddString(SourceFile);
}

FindExtensionsQuery::FindExtensionsQuery(QueryContext& QC, QualType T,
                                         SourceRange Loc)
    : Query(FindExtensionsQueryID, QC, Loc), T(T)
{
}

std::string FindExtensionsQuery::description() const
{
   std::string s;
   s += "running ";
   s += "FindExtensions";
   return s;
}

std::string FindExtensionsQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[FindExtensions]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

void FindExtensionsQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void FindExtensionsQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

GetExtensionTypeKindQuery::GetExtensionTypeKindQuery(QueryContext& QC,
                                                     ast::SourceType T,
                                                     SourceRange Loc)
    : Query(GetExtensionTypeKindQueryID, QC, Loc), T(T)
{
}

GetExtensionTypeKindQuery::ResultKind GetExtensionTypeKindQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetExtensionTypeKindQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetExtensionTypeKind";
   return s;
}

std::string GetExtensionTypeKindQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetExtensionTypeKind]";
   OS << " T = ";
   OS << T;
   ;

   return OS.str();
}

QueryResult
GetExtensionTypeKindQuery::finish(GetExtensionTypeKindQuery::ResultKind Result,
                                  Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetExtensionTypeKindQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void GetExtensionTypeKindQuery::Profile(llvm::FoldingSetNodeID& ID,
                                        ast::SourceType T)
{
   T.Profile(ID);
}

MatchExtensionTypeQuery::MatchExtensionTypeQuery(QueryContext& QC, QualType T,
                                                 ast::SourceType PatternTy,
                                                 SourceRange Loc)
    : Query(MatchExtensionTypeQueryID, QC, Loc), T(T), PatternTy(PatternTy)
{
}

MatchExtensionTypeQuery::ResultKind MatchExtensionTypeQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string MatchExtensionTypeQuery::description() const
{
   std::string s;
   s += "running ";
   s += "MatchExtensionType";
   return s;
}

std::string MatchExtensionTypeQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[MatchExtensionType]";
   OS << " T = ";
   OS << "'" << T << "'";
   OS << ',';
   OS << " PatternTy = ";
   OS << PatternTy;
   ;

   return OS.str();
}

QueryResult
MatchExtensionTypeQuery::finish(MatchExtensionTypeQuery::ResultKind Result,
                                Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void MatchExtensionTypeQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T, PatternTy);
}

void MatchExtensionTypeQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T,
                                      ast::SourceType PatternTy)
{
   ID.AddPointer(T.getAsOpaquePtr());
   PatternTy.Profile(ID);
}

ExtensionAppliesQuery::ExtensionAppliesQuery(QueryContext& QC,
                                             ast::ExtensionDecl* Ext,
                                             QualType T, SourceRange Loc)
    : Query(ExtensionAppliesQueryID, QC, Loc), Ext(Ext), T(T)
{
}

MatchExtensionTypeQuery::ResultKind ExtensionAppliesQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ExtensionAppliesQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ExtensionApplies";
   return s;
}

std::string ExtensionAppliesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ExtensionApplies]";
   OS << " Ext = ";
   OS << "'" << Ext->Decl::getNameAsString() << "'";
   OS << ',';
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

QueryResult
ExtensionAppliesQuery::finish(MatchExtensionTypeQuery::ResultKind Result,
                              Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ExtensionAppliesQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Ext, T);
}

void ExtensionAppliesQuery::Profile(llvm::FoldingSetNodeID& ID,
                                    ast::ExtensionDecl* Ext, QualType T)
{
   ID.AddPointer(Ext);
   ID.AddPointer(T.getAsOpaquePtr());
}

GetExtendedDeclQuery::GetExtendedDeclQuery(QueryContext& QC,
                                           ast::SourceType ExtendedTy,
                                           SourceRange Loc)
    : Query(GetExtendedDeclQueryID, QC, Loc), ExtendedTy(ExtendedTy)
{
}

ast::NamedDecl* GetExtendedDeclQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetExtendedDeclQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetExtendedDecl";
   return s;
}

std::string GetExtendedDeclQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetExtendedDecl]";
   OS << " ExtendedTy = ";
   OS << ExtendedTy;
   ;

   return OS.str();
}

QueryResult GetExtendedDeclQuery::finish(ast::NamedDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetExtendedDeclQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, ExtendedTy);
}

void GetExtendedDeclQuery::Profile(llvm::FoldingSetNodeID& ID,
                                   ast::SourceType ExtendedTy)
{
   ExtendedTy.Profile(ID);
}

ResolveExtensionQuery::ResolveExtensionQuery(QueryContext& QC,
                                             ast::ExtensionDecl* Ext,
                                             SourceRange Loc)
    : Query(ResolveExtensionQueryID, QC, Loc), Ext(Ext)
{
}

std::string ResolveExtensionQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveExtension";
   return s;
}

std::string ResolveExtensionQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveExtension]";
   OS << " Ext = ";
   OS << "'" << Ext->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void ResolveExtensionQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Ext);
}

void ResolveExtensionQuery::Profile(llvm::FoldingSetNodeID& ID,
                                    ast::ExtensionDecl* Ext)
{
   ID.AddPointer(Ext);
}

ResolveConformanceToProtocolQuery::ResolveConformanceToProtocolQuery(
    QueryContext& QC, QualType T, ast::SourceType Conf, SourceRange Loc)
    : Query(ResolveConformanceToProtocolQueryID, QC, Loc), T(T), Conf(Conf)
{
}

ast::ProtocolDecl* ResolveConformanceToProtocolQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ResolveConformanceToProtocolQuery::description() const
{
   std::string s;
   s += "resolving conformance of '";
   s += T.toDiagString();
   s += "'";
   return s;
}

std::string ResolveConformanceToProtocolQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveConformanceToProtocol]";
   OS << " T = ";
   OS << "'" << T << "'";
   OS << ',';
   OS << " Conf = ";
   OS << Conf;
   ;

   return OS.str();
}

QueryResult ResolveConformanceToProtocolQuery::finish(ast::ProtocolDecl* Result,
                                                      Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ResolveConformanceToProtocolQuery::Profile(
    llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T, Conf);
}

void ResolveConformanceToProtocolQuery::Profile(llvm::FoldingSetNodeID& ID,
                                                QualType T,
                                                ast::SourceType Conf)
{
   ID.AddPointer(T.getAsOpaquePtr());
   Conf.Profile(ID);
}

ResolveDeclaredConformancesQuery::ResolveDeclaredConformancesQuery(
    QueryContext& QC, QualType T, llvm::ArrayRef<ast::SourceType> Conformances,
    SourceRange Loc)
    : Query(ResolveDeclaredConformancesQueryID, QC, Loc), T(T),
      Conformances(Conformances)
{
}

std::string ResolveDeclaredConformancesQuery::description() const
{
   std::string s;
   s += "resolving declared conformances of '";
   s += T.toDiagString();
   s += "'";
   return s;
}

std::string ResolveDeclaredConformancesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveDeclaredConformances]";
   OS << " T = ";
   OS << "'" << T << "'";
   OS << ',';
   OS << " Conformances = ";
   OS << '[';
   unsigned i = 0;
   for (auto& El : Conformances) {
      if (i++ != 0)
         OS << ", ";
      OS << El;
   }
   OS << ']';

   ;

   return OS.str();
}

void ResolveDeclaredConformancesQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T, Conformances);
}

void ResolveDeclaredConformancesQuery::Profile(
    llvm::FoldingSetNodeID& ID, QualType T,
    llvm::ArrayRef<ast::SourceType> Conformances)
{
   ID.AddPointer(T.getAsOpaquePtr());
   for (auto& El : Conformances)
      El.Profile(ID);
}

AddInheritedConformanceQuery::AddInheritedConformanceQuery(
    QueryContext& QC, QualType T, ast::ProtocolDecl* Proto, SourceRange Loc)
    : Query(AddInheritedConformanceQueryID, QC, Loc), T(T), Proto(Proto)
{
}

std::string AddInheritedConformanceQuery::description() const
{
   std::string s;
   s += "running ";
   s += "AddInheritedConformance";
   return s;
}

std::string AddInheritedConformanceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[AddInheritedConformance]";
   OS << " T = ";
   OS << "'" << T << "'";
   OS << ',';
   OS << " Proto = ";
   OS << "'" << Proto->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void AddInheritedConformanceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T, Proto);
}

void AddInheritedConformanceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                           QualType T, ast::ProtocolDecl* Proto)
{
   ID.AddPointer(T.getAsOpaquePtr());
   ID.AddPointer(Proto);
}

ResolveAssociatedTypesQuery::ResolveAssociatedTypesQuery(QueryContext& QC,
                                                         QualType T,
                                                         SourceRange Loc)
    : Query(ResolveAssociatedTypesQueryID, QC, Loc), T(T)
{
}

std::string ResolveAssociatedTypesQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveAssociatedTypes";
   return s;
}

std::string ResolveAssociatedTypesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveAssociatedTypes]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

void ResolveAssociatedTypesQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void ResolveAssociatedTypesQuery::Profile(llvm::FoldingSetNodeID& ID,
                                          QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

GetReferencedAssociatedTypesQuery::GetReferencedAssociatedTypesQuery(
    QueryContext& QC, ast::NamedDecl* ConstrainedDecl, ast::DeclConstraint* C,
    SourceRange Loc)
    : Query(GetReferencedAssociatedTypesQueryID, QC, Loc),
      ConstrainedDecl(ConstrainedDecl), C(C)
{
}

ArrayRef<ast::AssociatedTypeDecl*>
GetReferencedAssociatedTypesQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetReferencedAssociatedTypesQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetReferencedAssociatedTypes";
   return s;
}

std::string GetReferencedAssociatedTypesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetReferencedAssociatedTypes]";
   OS << " ConstrainedDecl = ";
   OS << "'" << ConstrainedDecl->Decl::getNameAsString() << "'";
   OS << ',';
   OS << " C = ";
   OS << C;
   ;

   return OS.str();
}

QueryResult GetReferencedAssociatedTypesQuery::finish(
    SmallVector<ast::AssociatedTypeDecl*, 2>&& Result, Status St)
{
   this->Result = std::move(Result);

   C->setReferencedAssociatedTypes(this->Result.getValue());

   return Query::finish(St);
}

void GetReferencedAssociatedTypesQuery::Profile(
    llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, ConstrainedDecl, C);
}

void GetReferencedAssociatedTypesQuery::Profile(llvm::FoldingSetNodeID& ID,
                                                ast::NamedDecl* ConstrainedDecl,
                                                ast::DeclConstraint* C)
{
   ID.AddPointer(ConstrainedDecl);
   ID.AddPointer(C);
}

ReferencedAssociatedTypesReadyQuery::ReferencedAssociatedTypesReadyQuery(
    QueryContext& QC, QualType T, ast::NamedDecl* ConstrainedDecl,
    SourceRange Loc)
    : Query(ReferencedAssociatedTypesReadyQueryID, QC, Loc), T(T),
      ConstrainedDecl(ConstrainedDecl)
{
}

bool ReferencedAssociatedTypesReadyQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ReferencedAssociatedTypesReadyQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ReferencedAssociatedTypesReady";
   return s;
}

std::string ReferencedAssociatedTypesReadyQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ReferencedAssociatedTypesReady]";
   OS << " T = ";
   OS << "'" << T << "'";
   OS << ',';
   OS << " ConstrainedDecl = ";
   OS << "'" << ConstrainedDecl->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult ReferencedAssociatedTypesReadyQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

CheckProtocolExtensionApplicabilityQuery::
    CheckProtocolExtensionApplicabilityQuery(QueryContext& QC, QualType T,
                                             ast::ExtensionDecl* Ext,
                                             SourceRange Loc)
    : Query(CheckProtocolExtensionApplicabilityQueryID, QC, Loc), T(T), Ext(Ext)
{
}

CheckProtocolExtensionApplicabilityQuery::ResultKind
CheckProtocolExtensionApplicabilityQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string CheckProtocolExtensionApplicabilityQuery::description() const
{
   std::string s;
   s += "running ";
   s += "CheckProtocolExtensionApplicability";
   return s;
}

std::string CheckProtocolExtensionApplicabilityQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CheckProtocolExtensionApplicability]";
   OS << " T = ";
   OS << "'" << T << "'";
   OS << ',';
   OS << " Ext = ";
   OS << "'" << Ext->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult CheckProtocolExtensionApplicabilityQuery::finish(
    CheckProtocolExtensionApplicabilityQuery::ResultKind Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CheckProtocolExtensionApplicabilityQuery::Profile(
    llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T, Ext);
}

void CheckProtocolExtensionApplicabilityQuery::Profile(
    llvm::FoldingSetNodeID& ID, QualType T, ast::ExtensionDecl* Ext)
{
   ID.AddPointer(T.getAsOpaquePtr());
   ID.AddPointer(Ext);
}

VerifyConstraintQuery::VerifyConstraintQuery(QueryContext& QC,
                                             ast::DeclConstraint* C,
                                             ast::NamedDecl* ConstrainedDecl,
                                             SourceRange Loc)
    : Query(VerifyConstraintQueryID, QC, Loc), C(C),
      ConstrainedDecl(ConstrainedDecl)
{
}

std::string VerifyConstraintQuery::description() const
{
   std::string s;
   s += "running ";
   s += "VerifyConstraint";
   return s;
}

std::string VerifyConstraintQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[VerifyConstraint]";
   OS << " C = ";
   OS << C;
   OS << ',';
   OS << " ConstrainedDecl = ";
   OS << "'" << ConstrainedDecl->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void VerifyConstraintQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, C, ConstrainedDecl);
}

void VerifyConstraintQuery::Profile(llvm::FoldingSetNodeID& ID,
                                    ast::DeclConstraint* C,
                                    ast::NamedDecl* ConstrainedDecl)
{
   ID.AddPointer(C);
   ID.AddPointer(ConstrainedDecl);
}

IsConstraintSatisfiedQuery::IsConstraintSatisfiedQuery(
    QueryContext& QC, ast::DeclConstraint* C, QualType Self,
    ast::NamedDecl* OriginalDecl, SourceRange Loc)
    : Query(IsConstraintSatisfiedQueryID, QC, Loc), C(C), Self(Self),
      OriginalDecl(OriginalDecl)
{
}

bool IsConstraintSatisfiedQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsConstraintSatisfiedQuery::description() const
{
   std::string s;
   s += "running ";
   s += "IsConstraintSatisfied";
   return s;
}

std::string IsConstraintSatisfiedQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsConstraintSatisfied]";
   OS << " C = ";
   OS << C;
   OS << ',';
   OS << " Self = ";
   OS << "'" << Self << "'";
   OS << ',';
   OS << " OriginalDecl = ";
   OS << "'" << OriginalDecl->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult IsConstraintSatisfiedQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsConstraintSatisfiedQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, C, Self, OriginalDecl);
}

void IsConstraintSatisfiedQuery::Profile(llvm::FoldingSetNodeID& ID,
                                         ast::DeclConstraint* C, QualType Self,
                                         ast::NamedDecl* OriginalDecl)
{
   ID.AddPointer(C);
   ID.AddPointer(Self.getAsOpaquePtr());
   ID.AddPointer(OriginalDecl);
}

CheckConformancesQuery::CheckConformancesQuery(QueryContext& QC, QualType T,
                                               SourceRange Loc)
    : Query(CheckConformancesQueryID, QC, Loc), T(T)
{
}

std::string CheckConformancesQuery::description() const
{
   std::string s;
   s += "checking conformances of '";
   s += T.toDiagString();
   s += "'";
   return s;
}

std::string CheckConformancesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CheckConformances]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

void CheckConformancesQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void CheckConformancesQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

CheckSingleConformanceQuery::CheckSingleConformanceQuery(QueryContext& QC,
                                                         QualType T,
                                                         ast::ProtocolDecl* P,
                                                         SourceRange Loc)
    : Query(CheckSingleConformanceQueryID, QC, Loc), T(T), P(P)
{
}

std::string CheckSingleConformanceQuery::description() const
{
   std::string s;
   s += "checking conformance of '";
   s += T.toDiagString();
   s += "' to '";
   s += P->getFullName();
   s += "'";
   return s;
}

std::string CheckSingleConformanceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CheckSingleConformance]";
   OS << " T = ";
   OS << "'" << T << "'";
   OS << ',';
   OS << " P = ";
   OS << "'" << P->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void CheckSingleConformanceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T, P);
}

void CheckSingleConformanceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                          QualType T, ast::ProtocolDecl* P)
{
   ID.AddPointer(T.getAsOpaquePtr());
   ID.AddPointer(P);
}

ConformsToQuery::ConformsToQuery(QueryContext& QC, QualType T,
                                 ast::ProtocolDecl* P, SourceRange Loc)
    : Query(ConformsToQueryID, QC, Loc), T(T), P(P)
{
}

bool ConformsToQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ConformsToQuery::description() const
{
   std::string s;
   s += "checking if ";
   s += T.toDiagString();
   s += " conforms to ";
   s += P->getFullName();
   return s;
}

std::string ConformsToQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ConformsTo]";
   OS << " T = ";
   OS << "'" << T << "'";
   OS << ',';
   OS << " P = ";
   OS << "'" << P->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult ConformsToQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ConformsToQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T, P);
}

void ConformsToQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T,
                              ast::ProtocolDecl* P)
{
   ID.AddPointer(T.getAsOpaquePtr());
   ID.AddPointer(P);
}

GetTypeSizeQuery::GetTypeSizeQuery(QueryContext& QC, QualType T,
                                   SourceRange Loc)
    : Query(GetTypeSizeQueryID, QC, Loc), T(T)
{
}

unsigned GetTypeSizeQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetTypeSizeQuery::description() const
{
   std::string s;
   s += "computing size of type ";
   s += T.toDiagString();
   return s;
}

std::string GetTypeSizeQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetTypeSize]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

QueryResult GetTypeSizeQuery::finish(unsigned Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetTypeSizeQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void GetTypeSizeQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

GetTypeAlignmentQuery::GetTypeAlignmentQuery(QueryContext& QC, QualType T,
                                             SourceRange Loc)
    : Query(GetTypeAlignmentQueryID, QC, Loc), T(T)
{
}

unsigned short GetTypeAlignmentQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetTypeAlignmentQuery::description() const
{
   std::string s;
   s += "computing alignment of type ";
   s += T.toDiagString();
   return s;
}

std::string GetTypeAlignmentQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetTypeAlignment]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

QueryResult GetTypeAlignmentQuery::finish(unsigned short Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetTypeAlignmentQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void GetTypeAlignmentQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

GetTypeStrideQuery::GetTypeStrideQuery(QueryContext& QC, QualType T,
                                       SourceRange Loc)
    : Query(GetTypeStrideQueryID, QC, Loc), T(T)
{
}

unsigned GetTypeStrideQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetTypeStrideQuery::description() const
{
   std::string s;
   s += "computing stride of type ";
   s += T.toDiagString();
   return s;
}

std::string GetTypeStrideQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetTypeStride]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

QueryResult GetTypeStrideQuery::finish(unsigned Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetTypeStrideQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void GetTypeStrideQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

CalculateRecordSizeQuery::CalculateRecordSizeQuery(QueryContext& QC,
                                                   ast::RecordDecl* R,
                                                   SourceRange Loc)
    : Query(CalculateRecordSizeQueryID, QC, Loc), R(R)
{
}

unsigned CalculateRecordSizeQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string CalculateRecordSizeQuery::description() const
{
   std::string s;
   s += "calculating size of record ";
   s += R->getFullName();
   return s;
}

std::string CalculateRecordSizeQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CalculateRecordSize]";
   OS << " R = ";
   OS << "'" << R->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult CalculateRecordSizeQuery::finish(unsigned Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CalculateRecordSizeQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, R);
}

void CalculateRecordSizeQuery::Profile(llvm::FoldingSetNodeID& ID,
                                       ast::RecordDecl* R)
{
   ID.AddPointer(R);
}

IsAccessibleQuery::IsAccessibleQuery(QueryContext& QC, ast::DeclContext* DC,
                                     ast::NamedDecl* ND, SourceRange Loc)
    : Query(IsAccessibleQueryID, QC, Loc), DC(DC), ND(ND)
{
}

bool IsAccessibleQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsAccessibleQuery::description() const
{
   std::string s;
   s += "checking if '";
   s += ND->getFullName();
   s += "' is accessible";
   return s;
}

std::string IsAccessibleQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsAccessible]";
   OS << " DC = ";
   OS << "'" << DC->getNameAsString() << "'";
   OS << ',';
   OS << " ND = ";
   OS << "'" << ND->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult IsAccessibleQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsAccessibleQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, DC, ND);
}

void IsAccessibleQuery::Profile(llvm::FoldingSetNodeID& ID,
                                ast::DeclContext* DC, ast::NamedDecl* ND)
{
   ID.AddPointer(DC);
   ID.AddPointer(ND);
}

CheckAccessibilityQuery::CheckAccessibilityQuery(QueryContext& QC,
                                                 ast::DeclContext* DC,
                                                 ast::NamedDecl* ND,
                                                 SourceRange Loc)
    : Query(CheckAccessibilityQueryID, QC, Loc), DC(DC), ND(ND)
{
}

std::string CheckAccessibilityQuery::description() const
{
   std::string s;
   s += "checking if '";
   s += ND->getFullName();
   s += "' is accessible";
   return s;
}

std::string CheckAccessibilityQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CheckAccessibility]";
   OS << " DC = ";
   OS << "'" << DC->getNameAsString() << "'";
   OS << ',';
   OS << " ND = ";
   OS << "'" << ND->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void CheckAccessibilityQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, DC, ND);
}

void CheckAccessibilityQuery::Profile(llvm::FoldingSetNodeID& ID,
                                      ast::DeclContext* DC, ast::NamedDecl* ND)
{
   ID.AddPointer(DC);
   ID.AddPointer(ND);
}

TypeCheckDeclContextQuery::TypeCheckDeclContextQuery(QueryContext& QC,
                                                     ast::DeclContext* DC,
                                                     SourceRange Loc)
    : Query(TypeCheckDeclContextQueryID, QC, Loc), DC(DC)
{
}

std::string TypeCheckDeclContextQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += DC->getNameAsString();
   s += "'";
   return s;
}

std::string TypeCheckDeclContextQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypeCheckDeclContext]";
   OS << " DC = ";
   OS << "'" << DC->getNameAsString() << "'";
   ;

   return OS.str();
}

void TypeCheckDeclContextQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, DC);
}

void TypeCheckDeclContextQuery::Profile(llvm::FoldingSetNodeID& ID,
                                        ast::DeclContext* DC)
{
   ID.AddPointer(DC);
}

PrepareDeclInterfaceQuery::PrepareDeclInterfaceQuery(QueryContext& QC,
                                                     ast::Decl* D,
                                                     SourceRange Loc)
    : Query(PrepareDeclInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareDeclInterfaceQuery::description() const
{
   std::string s;
   s += "running ";
   s += "PrepareDeclInterface";
   return s;
}

std::string PrepareDeclInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareDeclInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareDeclInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareDeclInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                        ast::Decl* D)
{
   ID.AddPointer(D);
}

TypecheckDeclQuery::TypecheckDeclQuery(QueryContext& QC, ast::Decl* D,
                                       SourceRange Loc)
    : Query(TypecheckDeclQueryID, QC, Loc), D(D)
{
}

std::string TypecheckDeclQuery::description() const
{
   std::string s;
   s += "running ";
   s += "TypecheckDecl";
   return s;
}

std::string TypecheckDeclQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckDecl]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckDeclQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckDeclQuery::Profile(llvm::FoldingSetNodeID& ID, ast::Decl* D)
{
   ID.AddPointer(D);
}

PrepareCallableInterfaceQuery::PrepareCallableInterfaceQuery(
    QueryContext& QC, ast::CallableDecl* D, SourceRange Loc)
    : Query(PrepareCallableInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareCallableInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareCallableInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareCallableInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareCallableInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareCallableInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                            ast::CallableDecl* D)
{
   ID.AddPointer(D);
}

TypecheckCallableQuery::TypecheckCallableQuery(QueryContext& QC,
                                               ast::CallableDecl* D,
                                               SourceRange Loc)
    : Query(TypecheckCallableQueryID, QC, Loc), D(D)
{
}

std::string TypecheckCallableQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckCallableQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckCallable]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckCallableQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckCallableQuery::Profile(llvm::FoldingSetNodeID& ID,
                                     ast::CallableDecl* D)
{
   ID.AddPointer(D);
}

PrepareFunctionInterfaceQuery::PrepareFunctionInterfaceQuery(
    QueryContext& QC, ast::FunctionDecl* D, SourceRange Loc)
    : Query(PrepareFunctionInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareFunctionInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareFunctionInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareFunctionInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareFunctionInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareFunctionInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                            ast::FunctionDecl* D)
{
   ID.AddPointer(D);
}

TypecheckFunctionQuery::TypecheckFunctionQuery(QueryContext& QC,
                                               ast::FunctionDecl* D,
                                               SourceRange Loc)
    : Query(TypecheckFunctionQueryID, QC, Loc), D(D)
{
}

std::string TypecheckFunctionQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckFunctionQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckFunction]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckFunctionQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckFunctionQuery::Profile(llvm::FoldingSetNodeID& ID,
                                     ast::FunctionDecl* D)
{
   ID.AddPointer(D);
}

PrepareMethodInterfaceQuery::PrepareMethodInterfaceQuery(QueryContext& QC,
                                                         ast::MethodDecl* D,
                                                         SourceRange Loc)
    : Query(PrepareMethodInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareMethodInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareMethodInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareMethodInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareMethodInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareMethodInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                          ast::MethodDecl* D)
{
   ID.AddPointer(D);
}

TypecheckMethodQuery::TypecheckMethodQuery(QueryContext& QC, ast::MethodDecl* D,
                                           SourceRange Loc)
    : Query(TypecheckMethodQueryID, QC, Loc), D(D)
{
}

std::string TypecheckMethodQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckMethodQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckMethod]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckMethodQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckMethodQuery::Profile(llvm::FoldingSetNodeID& ID,
                                   ast::MethodDecl* D)
{
   ID.AddPointer(D);
}

PrepareInitInterfaceQuery::PrepareInitInterfaceQuery(QueryContext& QC,
                                                     ast::InitDecl* D,
                                                     SourceRange Loc)
    : Query(PrepareInitInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareInitInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareInitInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareInitInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareInitInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareInitInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                        ast::InitDecl* D)
{
   ID.AddPointer(D);
}

TypecheckInitQuery::TypecheckInitQuery(QueryContext& QC, ast::InitDecl* D,
                                       SourceRange Loc)
    : Query(TypecheckInitQueryID, QC, Loc), D(D)
{
}

std::string TypecheckInitQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckInitQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckInit]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckInitQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckInitQuery::Profile(llvm::FoldingSetNodeID& ID, ast::InitDecl* D)
{
   ID.AddPointer(D);
}

PrepareDeinitInterfaceQuery::PrepareDeinitInterfaceQuery(QueryContext& QC,
                                                         ast::DeinitDecl* D,
                                                         SourceRange Loc)
    : Query(PrepareDeinitInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareDeinitInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareDeinitInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareDeinitInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareDeinitInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareDeinitInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                          ast::DeinitDecl* D)
{
   ID.AddPointer(D);
}

TypecheckDeinitQuery::TypecheckDeinitQuery(QueryContext& QC, ast::DeinitDecl* D,
                                           SourceRange Loc)
    : Query(TypecheckDeinitQueryID, QC, Loc), D(D)
{
}

std::string TypecheckDeinitQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckDeinitQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckDeinit]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckDeinitQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckDeinitQuery::Profile(llvm::FoldingSetNodeID& ID,
                                   ast::DeinitDecl* D)
{
   ID.AddPointer(D);
}

PrepareFuncArgInterfaceQuery::PrepareFuncArgInterfaceQuery(QueryContext& QC,
                                                           ast::FuncArgDecl* D,
                                                           SourceRange Loc)
    : Query(PrepareFuncArgInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareFuncArgInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareFuncArgInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareFuncArgInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareFuncArgInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareFuncArgInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                           ast::FuncArgDecl* D)
{
   ID.AddPointer(D);
}

TypecheckFuncArgQuery::TypecheckFuncArgQuery(QueryContext& QC,
                                             ast::FuncArgDecl* D,
                                             SourceRange Loc)
    : Query(TypecheckFuncArgQueryID, QC, Loc), D(D)
{
}

std::string TypecheckFuncArgQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckFuncArgQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckFuncArg]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckFuncArgQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckFuncArgQuery::Profile(llvm::FoldingSetNodeID& ID,
                                    ast::FuncArgDecl* D)
{
   ID.AddPointer(D);
}

PrepareTemplateParamInterfaceQuery::PrepareTemplateParamInterfaceQuery(
    QueryContext& QC, ast::TemplateParamDecl* D, SourceRange Loc)
    : Query(PrepareTemplateParamInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareTemplateParamInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareTemplateParamInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareTemplateParamInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareTemplateParamInterfaceQuery::Profile(
    llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareTemplateParamInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                                 ast::TemplateParamDecl* D)
{
   ID.AddPointer(D);
}

TypecheckTemplateParamQuery::TypecheckTemplateParamQuery(
    QueryContext& QC, ast::TemplateParamDecl* D, SourceRange Loc)
    : Query(TypecheckTemplateParamQueryID, QC, Loc), D(D)
{
}

std::string TypecheckTemplateParamQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckTemplateParamQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckTemplateParam]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckTemplateParamQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckTemplateParamQuery::Profile(llvm::FoldingSetNodeID& ID,
                                          ast::TemplateParamDecl* D)
{
   ID.AddPointer(D);
}

PrepareAssociatedTypeInterfaceQuery::PrepareAssociatedTypeInterfaceQuery(
    QueryContext& QC, ast::AssociatedTypeDecl* D, SourceRange Loc)
    : Query(PrepareAssociatedTypeInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareAssociatedTypeInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareAssociatedTypeInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareAssociatedTypeInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareAssociatedTypeInterfaceQuery::Profile(
    llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareAssociatedTypeInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                                  ast::AssociatedTypeDecl* D)
{
   ID.AddPointer(D);
}

TypecheckAssociatedTypeQuery::TypecheckAssociatedTypeQuery(
    QueryContext& QC, ast::AssociatedTypeDecl* D, SourceRange Loc)
    : Query(TypecheckAssociatedTypeQueryID, QC, Loc), D(D)
{
}

std::string TypecheckAssociatedTypeQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckAssociatedTypeQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckAssociatedType]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckAssociatedTypeQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckAssociatedTypeQuery::Profile(llvm::FoldingSetNodeID& ID,
                                           ast::AssociatedTypeDecl* D)
{
   ID.AddPointer(D);
}

PrepareRecordInterfaceQuery::PrepareRecordInterfaceQuery(QueryContext& QC,
                                                         ast::RecordDecl* D,
                                                         SourceRange Loc)
    : Query(PrepareRecordInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareRecordInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareRecordInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareRecordInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareRecordInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareRecordInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                          ast::RecordDecl* D)
{
   ID.AddPointer(D);
}

TypecheckRecordQuery::TypecheckRecordQuery(QueryContext& QC, ast::RecordDecl* D,
                                           SourceRange Loc)
    : Query(TypecheckRecordQueryID, QC, Loc), D(D)
{
}

std::string TypecheckRecordQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckRecordQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckRecord]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckRecordQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckRecordQuery::Profile(llvm::FoldingSetNodeID& ID,
                                   ast::RecordDecl* D)
{
   ID.AddPointer(D);
}

PrepareStructInterfaceQuery::PrepareStructInterfaceQuery(QueryContext& QC,
                                                         ast::StructDecl* D,
                                                         SourceRange Loc)
    : Query(PrepareStructInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareStructInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareStructInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareStructInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareStructInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareStructInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                          ast::StructDecl* D)
{
   ID.AddPointer(D);
}

TypecheckStructQuery::TypecheckStructQuery(QueryContext& QC, ast::StructDecl* D,
                                           SourceRange Loc)
    : Query(TypecheckStructQueryID, QC, Loc), D(D)
{
}

std::string TypecheckStructQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckStructQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckStruct]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckStructQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckStructQuery::Profile(llvm::FoldingSetNodeID& ID,
                                   ast::StructDecl* D)
{
   ID.AddPointer(D);
}

PrepareClassInterfaceQuery::PrepareClassInterfaceQuery(QueryContext& QC,
                                                       ast::ClassDecl* D,
                                                       SourceRange Loc)
    : Query(PrepareClassInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareClassInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareClassInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareClassInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareClassInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareClassInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                         ast::ClassDecl* D)
{
   ID.AddPointer(D);
}

TypecheckClassQuery::TypecheckClassQuery(QueryContext& QC, ast::ClassDecl* D,
                                         SourceRange Loc)
    : Query(TypecheckClassQueryID, QC, Loc), D(D)
{
}

std::string TypecheckClassQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckClassQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckClass]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckClassQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckClassQuery::Profile(llvm::FoldingSetNodeID& ID, ast::ClassDecl* D)
{
   ID.AddPointer(D);
}

PrepareEnumInterfaceQuery::PrepareEnumInterfaceQuery(QueryContext& QC,
                                                     ast::EnumDecl* D,
                                                     SourceRange Loc)
    : Query(PrepareEnumInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareEnumInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareEnumInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareEnumInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareEnumInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareEnumInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                        ast::EnumDecl* D)
{
   ID.AddPointer(D);
}

TypecheckEnumQuery::TypecheckEnumQuery(QueryContext& QC, ast::EnumDecl* D,
                                       SourceRange Loc)
    : Query(TypecheckEnumQueryID, QC, Loc), D(D)
{
}

std::string TypecheckEnumQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckEnumQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckEnum]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckEnumQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckEnumQuery::Profile(llvm::FoldingSetNodeID& ID, ast::EnumDecl* D)
{
   ID.AddPointer(D);
}

PrepareProtocolInterfaceQuery::PrepareProtocolInterfaceQuery(
    QueryContext& QC, ast::ProtocolDecl* D, SourceRange Loc)
    : Query(PrepareProtocolInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareProtocolInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareProtocolInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareProtocolInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareProtocolInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareProtocolInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                            ast::ProtocolDecl* D)
{
   ID.AddPointer(D);
}

TypecheckProtocolQuery::TypecheckProtocolQuery(QueryContext& QC,
                                               ast::ProtocolDecl* D,
                                               SourceRange Loc)
    : Query(TypecheckProtocolQueryID, QC, Loc), D(D)
{
}

std::string TypecheckProtocolQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckProtocolQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckProtocol]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckProtocolQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckProtocolQuery::Profile(llvm::FoldingSetNodeID& ID,
                                     ast::ProtocolDecl* D)
{
   ID.AddPointer(D);
}

PrepareExtensionInterfaceQuery::PrepareExtensionInterfaceQuery(
    QueryContext& QC, ast::ExtensionDecl* D, SourceRange Loc)
    : Query(PrepareExtensionInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareExtensionInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareExtensionInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareExtensionInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareExtensionInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareExtensionInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                             ast::ExtensionDecl* D)
{
   ID.AddPointer(D);
}

TypecheckExtensionQuery::TypecheckExtensionQuery(QueryContext& QC,
                                                 ast::ExtensionDecl* D,
                                                 SourceRange Loc)
    : Query(TypecheckExtensionQueryID, QC, Loc), D(D)
{
}

std::string TypecheckExtensionQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckExtensionQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckExtension]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckExtensionQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckExtensionQuery::Profile(llvm::FoldingSetNodeID& ID,
                                      ast::ExtensionDecl* D)
{
   ID.AddPointer(D);
}

PrepareFieldInterfaceQuery::PrepareFieldInterfaceQuery(QueryContext& QC,
                                                       ast::FieldDecl* D,
                                                       SourceRange Loc)
    : Query(PrepareFieldInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareFieldInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareFieldInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareFieldInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareFieldInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareFieldInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                         ast::FieldDecl* D)
{
   ID.AddPointer(D);
}

TypecheckFieldQuery::TypecheckFieldQuery(QueryContext& QC, ast::FieldDecl* D,
                                         SourceRange Loc)
    : Query(TypecheckFieldQueryID, QC, Loc), D(D)
{
}

std::string TypecheckFieldQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckFieldQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckField]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckFieldQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckFieldQuery::Profile(llvm::FoldingSetNodeID& ID, ast::FieldDecl* D)
{
   ID.AddPointer(D);
}

PreparePropInterfaceQuery::PreparePropInterfaceQuery(QueryContext& QC,
                                                     ast::PropDecl* D,
                                                     SourceRange Loc)
    : Query(PreparePropInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PreparePropInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PreparePropInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PreparePropInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PreparePropInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PreparePropInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                        ast::PropDecl* D)
{
   ID.AddPointer(D);
}

TypecheckPropQuery::TypecheckPropQuery(QueryContext& QC, ast::PropDecl* D,
                                       SourceRange Loc)
    : Query(TypecheckPropQueryID, QC, Loc), D(D)
{
}

std::string TypecheckPropQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckPropQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckProp]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckPropQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckPropQuery::Profile(llvm::FoldingSetNodeID& ID, ast::PropDecl* D)
{
   ID.AddPointer(D);
}

PrepareSubscriptInterfaceQuery::PrepareSubscriptInterfaceQuery(
    QueryContext& QC, ast::SubscriptDecl* D, SourceRange Loc)
    : Query(PrepareSubscriptInterfaceQueryID, QC, Loc), D(D)
{
}

std::string PrepareSubscriptInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareSubscriptInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareSubscriptInterface]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareSubscriptInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void PrepareSubscriptInterfaceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                             ast::SubscriptDecl* D)
{
   ID.AddPointer(D);
}

TypecheckSubscriptQuery::TypecheckSubscriptQuery(QueryContext& QC,
                                                 ast::SubscriptDecl* D,
                                                 SourceRange Loc)
    : Query(TypecheckSubscriptQueryID, QC, Loc), D(D)
{
}

std::string TypecheckSubscriptQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckSubscriptQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckSubscript]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void TypecheckSubscriptQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void TypecheckSubscriptQuery::Profile(llvm::FoldingSetNodeID& ID,
                                      ast::SubscriptDecl* D)
{
   ID.AddPointer(D);
}

AssignInitNameQuery::AssignInitNameQuery(QueryContext& QC, ast::InitDecl* Init,
                                         SourceRange Loc)
    : Query(AssignInitNameQueryID, QC, Loc), Init(Init)
{
}

std::string AssignInitNameQuery::description() const
{
   std::string s;
   s += "running ";
   s += "AssignInitName";
   return s;
}

std::string AssignInitNameQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[AssignInitName]";
   OS << " Init = ";
   OS << "'" << Init->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void AssignInitNameQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Init);
}

void AssignInitNameQuery::Profile(llvm::FoldingSetNodeID& ID,
                                  ast::InitDecl* Init)
{
   ID.AddPointer(Init);
}

LookupFirstQuery::LookupFirstQuery(QueryContext& QC, ast::DeclContext* DC,
                                   DeclarationName Name, bool LocalLookup,
                                   SourceRange Loc)
    : Query(LookupFirstQueryID, QC, Loc), DC(DC), Name(Name),
      LocalLookup(LocalLookup)
{

   NumDeclsInContext = DC->getAllNamedDecls().size();
}

void LookupFirstQuery::refresh()
{

   if (Stat == Idle)
      return;
   if (NumDeclsInContext != DC->getAllNamedDecls().size())
      Stat = Idle;
}

const SingleLevelLookupResult* LookupFirstQuery::get() const
{
   assert(ok() && "query has not successfully completed!");

   return &Result.getValue();
}

std::string LookupFirstQuery::description() const
{
   std::string s;
   s += "looking up first occurence of '";
   s += Name.toString();
   s += "' in '";
   s += DC->getNameAsString();
   s += "'";
   return s;
}

std::string LookupFirstQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[LookupFirst]";
   OS << " DC = ";
   OS << "'" << DC->getNameAsString() << "'";
   OS << ',';
   OS << " Name = ";
   OS << "'" << Name << "'";
   OS << ',';
   OS << " LocalLookup = ";
   OS << LocalLookup;
   ;

   return OS.str();
}

QueryResult LookupFirstQuery::finish(SingleLevelLookupResult&& Result,
                                     Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void LookupFirstQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, DC, Name, LocalLookup);
}

void LookupFirstQuery::Profile(llvm::FoldingSetNodeID& ID, ast::DeclContext* DC,
                               DeclarationName Name, bool LocalLookup)
{
   ID.AddPointer(DC);
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddInteger(LocalLookup);
}

LookupSingleQuery::LookupSingleQuery(QueryContext& QC, ast::DeclContext* DC,
                                     DeclarationName Name, bool LocalLookup,
                                     bool DiagOnError, SourceRange Loc)
    : Query(LookupSingleQueryID, QC, Loc), DC(DC), Name(Name),
      LocalLookup(LocalLookup), DiagOnError(DiagOnError)
{

   NumDeclsInContext = DC->getAllNamedDecls().size();
}

void LookupSingleQuery::refresh()
{

   if (Stat == Idle)
      return;
   if (NumDeclsInContext != DC->getAllNamedDecls().size())
      Stat = Idle;
}

ast::NamedDecl* LookupSingleQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string LookupSingleQuery::description() const
{
   std::string s;
   s += "looking up single occurence of '";
   s += Name.toString();
   s += "' in '";
   s += DC->getNameAsString();
   s += "'";
   return s;
}

std::string LookupSingleQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[LookupSingle]";
   OS << " DC = ";
   OS << "'" << DC->getNameAsString() << "'";
   OS << ',';
   OS << " Name = ";
   OS << "'" << Name << "'";
   OS << ',';
   OS << " LocalLookup = ";
   OS << LocalLookup;
   OS << ',';
   OS << " DiagOnError = ";
   OS << DiagOnError;
   ;

   return OS.str();
}

QueryResult LookupSingleQuery::finish(ast::NamedDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void LookupSingleQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, DC, Name, LocalLookup);
}

void LookupSingleQuery::Profile(llvm::FoldingSetNodeID& ID,
                                ast::DeclContext* DC, DeclarationName Name,
                                bool LocalLookup)
{
   ID.AddPointer(DC);
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddInteger(LocalLookup);
}

MultiLevelLookupQuery::MultiLevelLookupQuery(QueryContext& QC,
                                             ast::DeclContext* DC,
                                             DeclarationName Name,
                                             bool LocalLookup, SourceRange Loc)
    : Query(MultiLevelLookupQueryID, QC, Loc), DC(DC), Name(Name),
      LocalLookup(LocalLookup)
{

   NumDeclsInContext = DC->getAllNamedDecls().size();
}

void MultiLevelLookupQuery::refresh()
{

   if (Stat == Idle)
      return;
   if (NumDeclsInContext != DC->getAllNamedDecls().size())
      Stat = Idle;
}

const MultiLevelLookupResult* MultiLevelLookupQuery::get() const
{
   assert(ok() && "query has not successfully completed!");

   return &Result.getValue();
}

std::string MultiLevelLookupQuery::description() const
{
   std::string s;
   s += "looking up '";
   s += Name.toString();
   s += "' in '";
   s += DC->getNameAsString();
   s += "'";
   return s;
}

std::string MultiLevelLookupQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[MultiLevelLookup]";
   OS << " DC = ";
   OS << "'" << DC->getNameAsString() << "'";
   OS << ',';
   OS << " Name = ";
   OS << "'" << Name << "'";
   OS << ',';
   OS << " LocalLookup = ";
   OS << LocalLookup;
   ;

   return OS.str();
}

QueryResult MultiLevelLookupQuery::finish(MultiLevelLookupResult&& Result,
                                          Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void MultiLevelLookupQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, DC, Name, LocalLookup);
}

void MultiLevelLookupQuery::Profile(llvm::FoldingSetNodeID& ID,
                                    ast::DeclContext* DC, DeclarationName Name,
                                    bool LocalLookup)
{
   ID.AddPointer(DC);
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddInteger(LocalLookup);
}

DirectLookupQuery::DirectLookupQuery(QueryContext& QC, ast::DeclContext* DC,
                                     DeclarationName Name,
                                     bool LookInExtensions,
                                     bool PrepareNameLookup, SourceRange Loc)
    : Query(DirectLookupQueryID, QC, Loc), DC(DC), Name(Name),
      LookInExtensions(LookInExtensions), PrepareNameLookup(PrepareNameLookup)
{

   NumDeclsInContext = DC->getAllNamedDecls().size();
}

void DirectLookupQuery::refresh()
{

   if (Stat == Idle)
      return;
   if (NumDeclsInContext != DC->getAllNamedDecls().size())
      Stat = Idle;
}

const MultiLevelLookupResult* DirectLookupQuery::get() const
{
   assert(ok() && "query has not successfully completed!");

   return &Result.getValue();
}

std::string DirectLookupQuery::description() const
{
   std::string s;
   s += "directly looking up '";
   s += Name.toString();
   s += "' in '";
   s += DC->getNameAsString();
   s += "'";
   return s;
}

std::string DirectLookupQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[DirectLookup]";
   OS << " DC = ";
   OS << "'" << DC->getNameAsString() << "'";
   OS << ',';
   OS << " Name = ";
   OS << "'" << Name << "'";
   OS << ',';
   OS << " LookInExtensions = ";
   OS << LookInExtensions;
   OS << ',';
   OS << " PrepareNameLookup = ";
   OS << PrepareNameLookup;
   ;

   return OS.str();
}

QueryResult DirectLookupQuery::finish(MultiLevelLookupResult&& Result,
                                      Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void DirectLookupQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, DC, Name, LookInExtensions, PrepareNameLookup);
}

void DirectLookupQuery::Profile(llvm::FoldingSetNodeID& ID,
                                ast::DeclContext* DC, DeclarationName Name,
                                bool LookInExtensions, bool PrepareNameLookup)
{
   ID.AddPointer(DC);
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddInteger(LookInExtensions);
   ID.AddInteger(PrepareNameLookup);
}

NestedNameLookupQuery::NestedNameLookupQuery(
    QueryContext& QC, ast::DeclContext* DC,
    llvm::ArrayRef<DeclarationName> Names, SourceRange Loc)
    : Query(NestedNameLookupQueryID, QC, Loc), DC(DC), Names(Names)
{
}

const MultiLevelLookupResult* NestedNameLookupQuery::get() const
{
   assert(ok() && "query has not successfully completed!");

   return &Result.getValue();
}

std::string NestedNameLookupQuery::description() const
{
   std::string s;
   s += "doing nested name lookup in '";
   s += DC->getNameAsString();
   s += "'";
   return s;
}

std::string NestedNameLookupQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[NestedNameLookup]";
   OS << " DC = ";
   OS << "'" << DC->getNameAsString() << "'";
   OS << ',';
   OS << " Names = ";
   OS << '[';
   unsigned i = 0;
   for (auto& El : Names) {
      if (i++ != 0)
         OS << ", ";
      OS << "'" << El << "'";
   }
   OS << ']';

   ;

   return OS.str();
}

QueryResult NestedNameLookupQuery::finish(MultiLevelLookupResult&& Result,
                                          Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

PrepareNameLookupQuery::PrepareNameLookupQuery(QueryContext& QC,
                                               ast::DeclContext* DC,
                                               SourceRange Loc)
    : Query(PrepareNameLookupQueryID, QC, Loc), DC(DC)
{
}

std::string PrepareNameLookupQuery::description() const
{
   std::string s;
   s += "preparing '";
   s += DC->getNameAsString();
   s += "' for name lookup";
   return s;
}

std::string PrepareNameLookupQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareNameLookup]";
   OS << " DC = ";
   OS << "'" << DC->getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareNameLookupQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, DC);
}

void PrepareNameLookupQuery::Profile(llvm::FoldingSetNodeID& ID,
                                     ast::DeclContext* DC)
{
   ID.AddPointer(DC);
}

GetAssociatedTypeQuery::GetAssociatedTypeQuery(
    QueryContext& QC, ast::RecordDecl* R, DeclarationName Name,
    llvm::ArrayRef<ast::ExtensionDecl*> Extensions, SourceRange Loc)
    : Query(GetAssociatedTypeQueryID, QC, Loc), R(R), Name(Name),
      Extensions(Extensions)
{
}

ast::AssociatedTypeDecl* GetAssociatedTypeQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetAssociatedTypeQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetAssociatedType";
   return s;
}

std::string GetAssociatedTypeQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetAssociatedType]";
   OS << " R = ";
   OS << "'" << R->Decl::getNameAsString() << "'";
   OS << ',';
   OS << " Name = ";
   OS << "'" << Name << "'";
   OS << ',';
   OS << " Extensions = ";
   OS << '[';
   unsigned i = 0;
   for (auto& El : Extensions) {
      if (i++ != 0)
         OS << ", ";
      OS << "'" << El->Decl::getNameAsString() << "'";
   }
   OS << ']';

   ;

   return OS.str();
}

QueryResult GetAssociatedTypeQuery::finish(ast::AssociatedTypeDecl* Result,
                                           Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

CheckTypeEquivalenceQuery::CheckTypeEquivalenceQuery(QueryContext& QC,
                                                     QualType LHS, QualType RHS,
                                                     QualType Self,
                                                     ast::DeclContext* LHSDecl,
                                                     SourceRange Loc)
    : Query(CheckTypeEquivalenceQueryID, QC, Loc), LHS(LHS), RHS(RHS),
      Self(Self), LHSDecl(LHSDecl)
{
}

bool CheckTypeEquivalenceQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string CheckTypeEquivalenceQuery::description() const
{
   std::string s;
   s += "running ";
   s += "CheckTypeEquivalence";
   return s;
}

std::string CheckTypeEquivalenceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CheckTypeEquivalence]";
   OS << " LHS = ";
   OS << "'" << LHS << "'";
   OS << ',';
   OS << " RHS = ";
   OS << "'" << RHS << "'";
   OS << ',';
   OS << " Self = ";
   OS << "'" << Self << "'";
   OS << ',';
   OS << " LHSDecl = ";
   OS << "'" << LHSDecl->getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult CheckTypeEquivalenceQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CheckTypeEquivalenceQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, LHS, RHS, Self, LHSDecl);
}

void CheckTypeEquivalenceQuery::Profile(llvm::FoldingSetNodeID& ID,
                                        QualType LHS, QualType RHS,
                                        QualType Self,
                                        ast::DeclContext* LHSDecl)
{
   ID.AddPointer(LHS.getAsOpaquePtr());
   ID.AddPointer(RHS.getAsOpaquePtr());
   ID.AddPointer(Self.getAsOpaquePtr());
   ID.AddPointer(LHSDecl);
}

InstantiateProtocolDefaultImplQuery::InstantiateProtocolDefaultImplQuery(
    QueryContext& QC, ast::NamedDecl* Impl, QualType Self, SourceRange Loc)
    : Query(InstantiateProtocolDefaultImplQueryID, QC, Loc), Impl(Impl),
      Self(Self)
{
}

ast::NamedDecl* InstantiateProtocolDefaultImplQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string InstantiateProtocolDefaultImplQuery::description() const
{
   std::string s;
   s += "running ";
   s += "InstantiateProtocolDefaultImpl";
   return s;
}

std::string InstantiateProtocolDefaultImplQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[InstantiateProtocolDefaultImpl]";
   OS << " Impl = ";
   OS << "'" << Impl->Decl::getNameAsString() << "'";
   OS << ',';
   OS << " Self = ";
   OS << "'" << Self << "'";
   ;

   return OS.str();
}

QueryResult InstantiateProtocolDefaultImplQuery::finish(ast::NamedDecl* Result,
                                                        Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void InstantiateProtocolDefaultImplQuery::Profile(
    llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Impl, Self);
}

void InstantiateProtocolDefaultImplQuery::Profile(llvm::FoldingSetNodeID& ID,
                                                  ast::NamedDecl* Impl,
                                                  QualType Self)
{
   ID.AddPointer(Impl);
   ID.AddPointer(Self.getAsOpaquePtr());
}

InstantiateRecordQuery::InstantiateRecordQuery(
    QueryContext& QC, ast::RecordDecl* Template,
    sema::FinalTemplateArgumentList* TemplateArgs,
    SourceLocation PointOfInstantiation, SourceRange Loc)
    : Query(InstantiateRecordQueryID, QC, Loc), Template(Template),
      TemplateArgs(TemplateArgs), PointOfInstantiation(PointOfInstantiation)
{
}

ast::RecordDecl* InstantiateRecordQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string InstantiateRecordQuery::description() const
{
   std::string s;
   s += "running ";
   s += "InstantiateRecord";
   return s;
}

std::string InstantiateRecordQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[InstantiateRecord]";
   OS << " Template = ";
   OS << "'" << Template->Decl::getNameAsString() << "'";
   OS << ',';
   OS << " TemplateArgs = ";
   OS << TemplateArgs;
   OS << ',';
   OS << " PointOfInstantiation = ";
   OS << PointOfInstantiation.getOffset();
   ;

   return OS.str();
}

QueryResult InstantiateRecordQuery::finish(ast::RecordDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void InstantiateRecordQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Template, TemplateArgs);
}

void InstantiateRecordQuery::Profile(
    llvm::FoldingSetNodeID& ID, ast::RecordDecl* Template,
    sema::FinalTemplateArgumentList* TemplateArgs)
{
   ID.AddPointer(Template);
   ID.AddPointer(TemplateArgs);
}

CheckTemplateExtensionApplicabilityQuery::
    CheckTemplateExtensionApplicabilityQuery(QueryContext& QC,
                                             ast::RecordDecl* Inst,
                                             ast::ExtensionDecl* Ext,
                                             SourceRange Loc)
    : Query(CheckTemplateExtensionApplicabilityQueryID, QC, Loc), Inst(Inst),
      Ext(Ext)
{
}

bool CheckTemplateExtensionApplicabilityQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string CheckTemplateExtensionApplicabilityQuery::description() const
{
   std::string s;
   s += "running ";
   s += "CheckTemplateExtensionApplicability";
   return s;
}

std::string CheckTemplateExtensionApplicabilityQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CheckTemplateExtensionApplicability]";
   OS << " Inst = ";
   OS << "'" << Inst->Decl::getNameAsString() << "'";
   OS << ',';
   OS << " Ext = ";
   OS << "'" << Ext->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult CheckTemplateExtensionApplicabilityQuery::finish(bool Result,
                                                             Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CheckTemplateExtensionApplicabilityQuery::Profile(
    llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Inst, Ext);
}

void CheckTemplateExtensionApplicabilityQuery::Profile(
    llvm::FoldingSetNodeID& ID, ast::RecordDecl* Inst, ast::ExtensionDecl* Ext)
{
   ID.AddPointer(Inst);
   ID.AddPointer(Ext);
}

InstantiateTemplateMemberQuery::InstantiateTemplateMemberQuery(
    QueryContext& QC, ast::NamedDecl* TemplateMember, ast::RecordDecl* Inst,
    SourceRange Loc)
    : Query(InstantiateTemplateMemberQueryID, QC, Loc),
      TemplateMember(TemplateMember), Inst(Inst)
{
}

ast::NamedDecl* InstantiateTemplateMemberQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string InstantiateTemplateMemberQuery::description() const
{
   std::string s;
   s += "running ";
   s += "InstantiateTemplateMember";
   return s;
}

std::string InstantiateTemplateMemberQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[InstantiateTemplateMember]";
   OS << " TemplateMember = ";
   OS << "'" << TemplateMember->Decl::getNameAsString() << "'";
   OS << ',';
   OS << " Inst = ";
   OS << "'" << Inst->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult InstantiateTemplateMemberQuery::finish(ast::NamedDecl* Result,
                                                   Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void InstantiateTemplateMemberQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, TemplateMember, Inst);
}

void InstantiateTemplateMemberQuery::Profile(llvm::FoldingSetNodeID& ID,
                                             ast::NamedDecl* TemplateMember,
                                             ast::RecordDecl* Inst)
{
   ID.AddPointer(TemplateMember);
   ID.AddPointer(Inst);
}

PrepareInstantiationForNameLookupQuery::PrepareInstantiationForNameLookupQuery(
    QueryContext& QC, ast::RecordDecl* Inst, SourceRange Loc)
    : Query(PrepareInstantiationForNameLookupQueryID, QC, Loc), Inst(Inst)
{
}

std::string PrepareInstantiationForNameLookupQuery::description() const
{
   std::string s;
   s += "running ";
   s += "PrepareInstantiationForNameLookup";
   return s;
}

std::string PrepareInstantiationForNameLookupQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareInstantiationForNameLookup]";
   OS << " Inst = ";
   OS << "'" << Inst->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void PrepareInstantiationForNameLookupQuery::Profile(
    llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Inst);
}

void PrepareInstantiationForNameLookupQuery::Profile(llvm::FoldingSetNodeID& ID,
                                                     ast::RecordDecl* Inst)
{
   ID.AddPointer(Inst);
}

GetILModuleForDeclQuery::GetILModuleForDeclQuery(QueryContext& QC, ast::Decl* D,
                                                 SourceRange Loc)
    : Query(GetILModuleForDeclQueryID, QC, Loc), D(D)
{
}

il::Module* GetILModuleForDeclQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetILModuleForDeclQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetILModuleForDecl";
   return s;
}

std::string GetILModuleForDeclQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetILModuleForDecl]";
   OS << " D = ";
   OS << "'" << D->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult GetILModuleForDeclQuery::finish(il::Module* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetILModuleForDeclQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, D);
}

void GetILModuleForDeclQuery::Profile(llvm::FoldingSetNodeID& ID, ast::Decl* D)
{
   ID.AddPointer(D);
}

GenerateILForContextQuery::GenerateILForContextQuery(QueryContext& QC,
                                                     ast::DeclContext* DC,
                                                     SourceRange Loc)
    : Query(GenerateILForContextQueryID, QC, Loc), DC(DC)
{
}

std::string GenerateILForContextQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GenerateILForContext";
   return s;
}

std::string GenerateILForContextQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GenerateILForContext]";
   OS << " DC = ";
   OS << "'" << DC->getNameAsString() << "'";
   ;

   return OS.str();
}

void GenerateILForContextQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, DC);
}

void GenerateILForContextQuery::Profile(llvm::FoldingSetNodeID& ID,
                                        ast::DeclContext* DC)
{
   ID.AddPointer(DC);
}

GenerateRecordILQuery::GenerateRecordILQuery(QueryContext& QC,
                                             ast::RecordDecl* R,
                                             SourceRange Loc)
    : Query(GenerateRecordILQueryID, QC, Loc), R(R)
{
}

std::string GenerateRecordILQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GenerateRecordIL";
   return s;
}

std::string GenerateRecordILQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GenerateRecordIL]";
   OS << " R = ";
   OS << "'" << R->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void GenerateRecordILQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, R);
}

void GenerateRecordILQuery::Profile(llvm::FoldingSetNodeID& ID,
                                    ast::RecordDecl* R)
{
   ID.AddPointer(R);
}

GetILGlobalQuery::GetILGlobalQuery(QueryContext& QC, ast::VarDecl* GV,
                                   SourceRange Loc)
    : Query(GetILGlobalQueryID, QC, Loc), GV(GV)
{
}

il::GlobalVariable* GetILGlobalQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetILGlobalQuery::description() const
{
   std::string s;
   s += "declaring IL global variable for '";
   s += GV->getFullName();
   s += "'";
   return s;
}

std::string GetILGlobalQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetILGlobal]";
   OS << " GV = ";
   OS << "'" << GV->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult GetILGlobalQuery::finish(il::GlobalVariable* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetILGlobalQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, GV);
}

void GetILGlobalQuery::Profile(llvm::FoldingSetNodeID& ID, ast::VarDecl* GV)
{
   ID.AddPointer(GV);
}

GenerateLazyILGlobalDefinitionQuery::GenerateLazyILGlobalDefinitionQuery(
    QueryContext& QC, ast::VarDecl* GV, SourceRange Loc)
    : Query(GenerateLazyILGlobalDefinitionQueryID, QC, Loc), GV(GV)
{
}

std::string GenerateLazyILGlobalDefinitionQuery::description() const
{
   std::string s;
   s += "generating lazy initialization IL for global variable '";
   s += GV->getFullName();
   s += "'";
   return s;
}

std::string GenerateLazyILGlobalDefinitionQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GenerateLazyILGlobalDefinition]";
   OS << " GV = ";
   OS << "'" << GV->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void GenerateLazyILGlobalDefinitionQuery::Profile(
    llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, GV);
}

void GenerateLazyILGlobalDefinitionQuery::Profile(llvm::FoldingSetNodeID& ID,
                                                  ast::VarDecl* GV)
{
   ID.AddPointer(GV);
}

GetILFunctionQuery::GetILFunctionQuery(QueryContext& QC, ast::CallableDecl* C,
                                       SourceRange Loc)
    : Query(GetILFunctionQueryID, QC, Loc), C(C)
{
}

il::Function* GetILFunctionQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetILFunctionQuery::description() const
{
   std::string s;
   s += "declaring IL function for '";
   s += C->getFullName();
   s += "'";
   return s;
}

std::string GetILFunctionQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetILFunction]";
   OS << " C = ";
   OS << "'" << C->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult GetILFunctionQuery::finish(il::Function* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetILFunctionQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, C);
}

void GetILFunctionQuery::Profile(llvm::FoldingSetNodeID& ID,
                                 ast::CallableDecl* C)
{
   ID.AddPointer(C);
}

GenerateILFunctionBodyQuery::GenerateILFunctionBodyQuery(QueryContext& QC,
                                                         ast::CallableDecl* C,
                                                         SourceRange Loc)
    : Query(GenerateILFunctionBodyQueryID, QC, Loc), C(C)
{
}

std::string GenerateILFunctionBodyQuery::description() const
{
   std::string s;
   s += "generating IL for function '";
   s += C->getFullName();
   s += "'";
   return s;
}

std::string GenerateILFunctionBodyQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GenerateILFunctionBody]";
   OS << " C = ";
   OS << "'" << C->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

void GenerateILFunctionBodyQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, C);
}

void GenerateILFunctionBodyQuery::Profile(llvm::FoldingSetNodeID& ID,
                                          ast::CallableDecl* C)
{
   ID.AddPointer(C);
}

GetILTypeInfoQuery::GetILTypeInfoQuery(QueryContext& QC, QualType T,
                                       SourceRange Loc)
    : Query(GetILTypeInfoQueryID, QC, Loc), T(T)
{
}

il::GlobalVariable* GetILTypeInfoQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetILTypeInfoQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetILTypeInfo";
   return s;
}

std::string GetILTypeInfoQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetILTypeInfo]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

QueryResult GetILTypeInfoQuery::finish(il::GlobalVariable* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetILTypeInfoQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void GetILTypeInfoQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

CreateILBasicTypeInfoQuery::CreateILBasicTypeInfoQuery(QueryContext& QC,
                                                       QualType T,
                                                       SourceRange Loc)
    : Query(CreateILBasicTypeInfoQueryID, QC, Loc), T(T)
{
}

il::Constant* CreateILBasicTypeInfoQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string CreateILBasicTypeInfoQuery::description() const
{
   std::string s;
   s += "running ";
   s += "CreateILBasicTypeInfo";
   return s;
}

std::string CreateILBasicTypeInfoQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CreateILBasicTypeInfo]";
   OS << " T = ";
   OS << "'" << T << "'";
   ;

   return OS.str();
}

QueryResult CreateILBasicTypeInfoQuery::finish(il::Constant* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CreateILBasicTypeInfoQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, T);
}

void CreateILBasicTypeInfoQuery::Profile(llvm::FoldingSetNodeID& ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

CreateILRecordTypeInfoQuery::CreateILRecordTypeInfoQuery(QueryContext& QC,
                                                         ast::RecordDecl* R,
                                                         SourceRange Loc)
    : Query(CreateILRecordTypeInfoQueryID, QC, Loc), R(R)
{
}

il::Constant* CreateILRecordTypeInfoQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string CreateILRecordTypeInfoQuery::description() const
{
   std::string s;
   s += "running ";
   s += "CreateILRecordTypeInfo";
   return s;
}

std::string CreateILRecordTypeInfoQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CreateILRecordTypeInfo]";
   OS << " R = ";
   OS << "'" << R->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult CreateILRecordTypeInfoQuery::finish(il::Constant* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CreateILRecordTypeInfoQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, R);
}

void CreateILRecordTypeInfoQuery::Profile(llvm::FoldingSetNodeID& ID,
                                          ast::RecordDecl* R)
{
   ID.AddPointer(R);
}

OpenFileForReadQuery::OpenFileForReadQuery(QueryContext& QC, StringRef FileName,
                                           SourceRange Loc)
    : Query(OpenFileForReadQueryID, QC, Loc), FileName(FileName)
{
}

llvm::MemoryBuffer* OpenFileForReadQuery::get() const
{
   assert(ok() && "query has not successfully completed!");

   return Result.getValue().get();
}

std::string OpenFileForReadQuery::description() const
{
   std::string s;
   s += "opening file ";
   s += this->FileName;
   return s;
}

std::string OpenFileForReadQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[OpenFileForRead]";
   OS << " FileName = ";
   OS << "'" << FileName << "'";
   ;

   return OS.str();
}

QueryResult
OpenFileForReadQuery::finish(std::unique_ptr<llvm::MemoryBuffer>&& Result,
                             Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void OpenFileForReadQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, FileName);
}

void OpenFileForReadQuery::Profile(llvm::FoldingSetNodeID& ID,
                                   StringRef FileName)
{
   ID.AddString(FileName);
}

OpenTmpFileQuery::OpenTmpFileQuery(QueryContext& QC, StringRef Extension,
                                   SourceRange Loc)
    : Query(OpenTmpFileQueryID, QC, Loc), Extension(Extension)
{
}

llvm::raw_ostream* OpenTmpFileQuery::get() const
{
   assert(ok() && "query has not successfully completed!");

   return Result.getValue().get();
}

std::string OpenTmpFileQuery::description() const
{
   std::string s;
   s += "opening temp file with extension .";
   s += this->Extension;
   return s;
}

std::string OpenTmpFileQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[OpenTmpFile]";
   OS << " Extension = ";
   OS << "'" << Extension << "'";
   ;

   return OS.str();
}

QueryResult
OpenTmpFileQuery::finish(std::unique_ptr<llvm::raw_ostream>&& Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

GetDefaultTemplateArgQuery::GetDefaultTemplateArgQuery(
    QueryContext& QC, ast::TemplateParamDecl* P, SourceRange Loc)
    : Query(GetDefaultTemplateArgQueryID, QC, Loc), P(P)
{
}

const sema::TemplateArgument* GetDefaultTemplateArgQuery::get() const
{
   assert(ok() && "query has not successfully completed!");

   return &Result.getValue();
}

std::string GetDefaultTemplateArgQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetDefaultTemplateArg";
   return s;
}

std::string GetDefaultTemplateArgQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetDefaultTemplateArg]";
   OS << " P = ";
   OS << "'" << P->Decl::getNameAsString() << "'";
   ;

   return OS.str();
}

QueryResult GetDefaultTemplateArgQuery::finish(sema::TemplateArgument&& Result,
                                               Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void GetDefaultTemplateArgQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, P);
}

void GetDefaultTemplateArgQuery::Profile(llvm::FoldingSetNodeID& ID,
                                         ast::TemplateParamDecl* P)
{
   ID.AddPointer(P);
}

CreateSelfArgumentQuery::CreateSelfArgumentQuery(QueryContext& QC,
                                                 QualType Self,
                                                 SourceLocation SelfLoc,
                                                 SourceRange Loc)
    : Query(CreateSelfArgumentQueryID, QC, Loc), Self(Self), SelfLoc(SelfLoc)
{
}

ast::FuncArgDecl* CreateSelfArgumentQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string CreateSelfArgumentQuery::description() const
{
   std::string s;
   s += "running ";
   s += "CreateSelfArgument";
   return s;
}

std::string CreateSelfArgumentQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CreateSelfArgument]";
   OS << " Self = ";
   OS << "'" << Self << "'";
   OS << ',';
   OS << " SelfLoc = ";
   OS << SelfLoc.getOffset();
   ;

   return OS.str();
}

QueryResult CreateSelfArgumentQuery::finish(ast::FuncArgDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

FindEquivalentDeclQuery::FindEquivalentDeclQuery(
    QueryContext& QC, ast::NamedDecl* Decl, ast::DeclContext* DC, QualType Self,
    bool LookInExtensions, SourceRange Loc)
    : Query(FindEquivalentDeclQueryID, QC, Loc), Decl(Decl), DC(DC), Self(Self),
      LookInExtensions(LookInExtensions)
{
}

ast::NamedDecl* FindEquivalentDeclQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string FindEquivalentDeclQuery::description() const
{
   std::string s;
   s += "running ";
   s += "FindEquivalentDecl";
   return s;
}

std::string FindEquivalentDeclQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[FindEquivalentDecl]";
   OS << " Decl = ";
   OS << "'" << Decl->Decl::getNameAsString() << "'";
   OS << ',';
   OS << " DC = ";
   OS << "'" << DC->getNameAsString() << "'";
   OS << ',';
   OS << " Self = ";
   OS << "'" << Self << "'";
   OS << ',';
   OS << " LookInExtensions = ";
   OS << LookInExtensions;
   ;

   return OS.str();
}

QueryResult FindEquivalentDeclQuery::finish(ast::NamedDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void FindEquivalentDeclQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, Decl, DC, Self, LookInExtensions);
}

void FindEquivalentDeclQuery::Profile(llvm::FoldingSetNodeID& ID,
                                      ast::NamedDecl* Decl,
                                      ast::DeclContext* DC, QualType Self,
                                      bool LookInExtensions)
{
   ID.AddPointer(Decl);
   ID.AddPointer(DC);
   ID.AddPointer(Self.getAsOpaquePtr());
   ID.AddInteger(LookInExtensions);
}

EquivalentSignaturesQuery::EquivalentSignaturesQuery(QueryContext& QC,
                                                     ast::CallableDecl* C1,
                                                     ast::CallableDecl* C2,
                                                     QualType Self,
                                                     SourceRange Loc)
    : Query(EquivalentSignaturesQueryID, QC, Loc), C1(C1), C2(C2), Self(Self)
{
}

EquivalentSignaturesQuery::ResultKind EquivalentSignaturesQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string EquivalentSignaturesQuery::description() const
{
   std::string s;
   s += "running ";
   s += "EquivalentSignatures";
   return s;
}

std::string EquivalentSignaturesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[EquivalentSignatures]";
   OS << " C1 = ";
   OS << "'" << C1->Decl::getNameAsString() << "'";
   OS << ',';
   OS << " C2 = ";
   OS << "'" << C2->Decl::getNameAsString() << "'";
   OS << ',';
   OS << " Self = ";
   OS << "'" << Self << "'";
   ;

   return OS.str();
}

QueryResult
EquivalentSignaturesQuery::finish(EquivalentSignaturesQuery::ResultKind Result,
                                  Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void EquivalentSignaturesQuery::Profile(llvm::FoldingSetNodeID& ID) const
{
   Profile(ID, C1, C2, Self);
}

void EquivalentSignaturesQuery::Profile(llvm::FoldingSetNodeID& ID,
                                        ast::CallableDecl* C1,
                                        ast::CallableDecl* C2, QualType Self)
{
   ID.AddPointer(C1);
   ID.AddPointer(C2);
   ID.AddPointer(Self.getAsOpaquePtr());
}

#endif

#ifdef CDOT_QUERY_CLASS_IMPL
bool Query::isPure() const
{
   switch (K) {
   case CompileModuleQueryID:
      return false;
   case ParseSourceFileQueryID:
      return false;
   case TypeCheckASTQueryID:
      return false;
   case CreateILModuleQueryID:
      return false;
   case SetupIRGenQueryID:
      return false;
   case CreateLLVMModuleQueryID:
      return false;
   case CreateObjectQueryID:
      return false;
   case CreateStaticLibQueryID:
      return false;
   case CreateDynamicLibQueryID:
      return false;
   case CreateExecutableQueryID:
      return false;
   case EmitILQueryID:
      return false;
   case EmitIRQueryID:
      return false;
   case PrintUsedMemoryQueryID:
      return false;
   case MeasureExecutionTimeQueryID:
      return false;
   case IsEquatableQueryID:
      return false;
   case IsCopyableQueryID:
      return false;
   case IsMoveOnlyQueryID:
      return false;
   case IsImplicitlyCopyableQueryID:
      return false;
   case IsTriviallyCopyableQueryID:
      return false;
   case NeedsRetainOrReleaseQueryID:
      return false;
   case NeedsDeinitilizationQueryID:
      return false;
   case NeedsStructReturnQueryID:
      return false;
   case SubstAssociatedTypesQueryID:
      return false;
   case SubstTemplateParamTypesQueryID:
      return false;
   case GetBuiltinModuleQueryID:
      return false;
   case GetBuiltinFuncQueryID:
      return false;
   case GetBuiltinRecordQueryID:
      return false;
   case GetBuiltinProtocolQueryID:
      return false;
   case FindPrecedenceGroupQueryID:
      return false;
   case FindOperatorQueryID:
      return false;
   case ResolveImportQueryID:
      return false;
   case ResolveUsingQueryID:
      return false;
   case ResolveStaticIfQueryID:
      return false;
   case ResolveStaticForQueryID:
      return false;
   case ResolveStaticAssertQueryID:
      return false;
   case ResolveStaticPrintQueryID:
      return false;
   case ResolveStaticExprQueryID:
      return false;
   case ResolveStaticExprToBoolQueryID:
      return false;
   case ExpandMacroQueryID:
      return false;
   case ExpandMacroDeclQueryID:
      return false;
   case ExpandMacrosQueryID:
      return false;
   case FindExtensionsQueryID:
      return false;
   case GetExtensionTypeKindQueryID:
      return false;
   case MatchExtensionTypeQueryID:
      return false;
   case ExtensionAppliesQueryID:
      return false;
   case GetExtendedDeclQueryID:
      return false;
   case ResolveExtensionQueryID:
      return false;
   case ResolveConformanceToProtocolQueryID:
      return false;
   case ResolveDeclaredConformancesQueryID:
      return false;
   case AddInheritedConformanceQueryID:
      return false;
   case ResolveAssociatedTypesQueryID:
      return false;
   case GetReferencedAssociatedTypesQueryID:
      return false;
   case ReferencedAssociatedTypesReadyQueryID:
      return false;
   case CheckProtocolExtensionApplicabilityQueryID:
      return false;
   case VerifyConstraintQueryID:
      return false;
   case IsConstraintSatisfiedQueryID:
      return false;
   case CheckConformancesQueryID:
      return false;
   case CheckSingleConformanceQueryID:
      return false;
   case ConformsToQueryID:
      return false;
   case GetTypeSizeQueryID:
      return false;
   case GetTypeAlignmentQueryID:
      return false;
   case GetTypeStrideQueryID:
      return false;
   case CalculateRecordSizeQueryID:
      return false;
   case IsAccessibleQueryID:
      return false;
   case CheckAccessibilityQueryID:
      return false;
   case TypeCheckDeclContextQueryID:
      return false;
   case PrepareDeclInterfaceQueryID:
      return false;
   case TypecheckDeclQueryID:
      return false;
   case PrepareCallableInterfaceQueryID:
      return false;
   case TypecheckCallableQueryID:
      return false;
   case PrepareFunctionInterfaceQueryID:
      return false;
   case TypecheckFunctionQueryID:
      return false;
   case PrepareMethodInterfaceQueryID:
      return false;
   case TypecheckMethodQueryID:
      return false;
   case PrepareInitInterfaceQueryID:
      return false;
   case TypecheckInitQueryID:
      return false;
   case PrepareDeinitInterfaceQueryID:
      return false;
   case TypecheckDeinitQueryID:
      return false;
   case PrepareFuncArgInterfaceQueryID:
      return false;
   case TypecheckFuncArgQueryID:
      return false;
   case PrepareTemplateParamInterfaceQueryID:
      return false;
   case TypecheckTemplateParamQueryID:
      return false;
   case PrepareAssociatedTypeInterfaceQueryID:
      return false;
   case TypecheckAssociatedTypeQueryID:
      return false;
   case PrepareRecordInterfaceQueryID:
      return false;
   case TypecheckRecordQueryID:
      return false;
   case PrepareStructInterfaceQueryID:
      return false;
   case TypecheckStructQueryID:
      return false;
   case PrepareClassInterfaceQueryID:
      return false;
   case TypecheckClassQueryID:
      return false;
   case PrepareEnumInterfaceQueryID:
      return false;
   case TypecheckEnumQueryID:
      return false;
   case PrepareProtocolInterfaceQueryID:
      return false;
   case TypecheckProtocolQueryID:
      return false;
   case PrepareExtensionInterfaceQueryID:
      return false;
   case TypecheckExtensionQueryID:
      return false;
   case PrepareFieldInterfaceQueryID:
      return false;
   case TypecheckFieldQueryID:
      return false;
   case PreparePropInterfaceQueryID:
      return false;
   case TypecheckPropQueryID:
      return false;
   case PrepareSubscriptInterfaceQueryID:
      return false;
   case TypecheckSubscriptQueryID:
      return false;
   case AssignInitNameQueryID:
      return false;
   case LookupFirstQueryID:
      return false;
   case LookupSingleQueryID:
      return false;
   case MultiLevelLookupQueryID:
      return false;
   case DirectLookupQueryID:
      return false;
   case NestedNameLookupQueryID:
      return false;
   case PrepareNameLookupQueryID:
      return false;
   case GetAssociatedTypeQueryID:
      return false;
   case CheckTypeEquivalenceQueryID:
      return false;
   case InstantiateProtocolDefaultImplQueryID:
      return false;
   case InstantiateRecordQueryID:
      return false;
   case CheckTemplateExtensionApplicabilityQueryID:
      return false;
   case InstantiateTemplateMemberQueryID:
      return false;
   case PrepareInstantiationForNameLookupQueryID:
      return false;
   case GetILModuleForDeclQueryID:
      return false;
   case GenerateILForContextQueryID:
      return false;
   case GenerateRecordILQueryID:
      return false;
   case GetILGlobalQueryID:
      return false;
   case GenerateLazyILGlobalDefinitionQueryID:
      return false;
   case GetILFunctionQueryID:
      return false;
   case GenerateILFunctionBodyQueryID:
      return false;
   case GetILTypeInfoQueryID:
      return false;
   case CreateILBasicTypeInfoQueryID:
      return false;
   case CreateILRecordTypeInfoQueryID:
      return false;
   case OpenFileForReadQueryID:
      return false;
   case OpenTmpFileQueryID:
      return false;
   case GetDefaultTemplateArgQueryID:
      return false;
   case CreateSelfArgumentQueryID:
      return false;
   case FindEquivalentDeclQueryID:
      return false;
   case EquivalentSignaturesQueryID:
      return false;
   }
}

bool Query::canBeCached() const
{
   switch (K) {
   case CompileModuleQueryID:
      return true;
   case ParseSourceFileQueryID:
      return true;
   case TypeCheckASTQueryID:
      return true;
   case CreateILModuleQueryID:
      return true;
   case SetupIRGenQueryID:
      return true;
   case CreateLLVMModuleQueryID:
      return true;
   case CreateObjectQueryID:
      return false;
   case CreateStaticLibQueryID:
      return false;
   case CreateDynamicLibQueryID:
      return false;
   case CreateExecutableQueryID:
      return false;
   case EmitILQueryID:
      return false;
   case EmitIRQueryID:
      return false;
   case PrintUsedMemoryQueryID:
      return false;
   case MeasureExecutionTimeQueryID:
      return true;
   case IsEquatableQueryID:
      return true;
   case IsCopyableQueryID:
      return true;
   case IsMoveOnlyQueryID:
      return true;
   case IsImplicitlyCopyableQueryID:
      return true;
   case IsTriviallyCopyableQueryID:
      return true;
   case NeedsRetainOrReleaseQueryID:
      return true;
   case NeedsDeinitilizationQueryID:
      return true;
   case NeedsStructReturnQueryID:
      return true;
   case SubstAssociatedTypesQueryID:
      return true;
   case SubstTemplateParamTypesQueryID:
      return true;
   case GetBuiltinModuleQueryID:
      return true;
   case GetBuiltinFuncQueryID:
      return true;
   case GetBuiltinRecordQueryID:
      return true;
   case GetBuiltinProtocolQueryID:
      return true;
   case FindPrecedenceGroupQueryID:
      return true;
   case FindOperatorQueryID:
      return true;
   case ResolveImportQueryID:
      return true;
   case ResolveUsingQueryID:
      return true;
   case ResolveStaticIfQueryID:
      return true;
   case ResolveStaticForQueryID:
      return true;
   case ResolveStaticAssertQueryID:
      return true;
   case ResolveStaticPrintQueryID:
      return true;
   case ResolveStaticExprQueryID:
      return true;
   case ResolveStaticExprToBoolQueryID:
      return true;
   case ExpandMacroQueryID:
      return true;
   case ExpandMacroDeclQueryID:
      return true;
   case ExpandMacrosQueryID:
      return true;
   case FindExtensionsQueryID:
      return true;
   case GetExtensionTypeKindQueryID:
      return true;
   case MatchExtensionTypeQueryID:
      return true;
   case ExtensionAppliesQueryID:
      return true;
   case GetExtendedDeclQueryID:
      return true;
   case ResolveExtensionQueryID:
      return true;
   case ResolveConformanceToProtocolQueryID:
      return true;
   case ResolveDeclaredConformancesQueryID:
      return true;
   case AddInheritedConformanceQueryID:
      return true;
   case ResolveAssociatedTypesQueryID:
      return true;
   case GetReferencedAssociatedTypesQueryID:
      return true;
   case ReferencedAssociatedTypesReadyQueryID:
      return false;
   case CheckProtocolExtensionApplicabilityQueryID:
      return true;
   case VerifyConstraintQueryID:
      return true;
   case IsConstraintSatisfiedQueryID:
      return true;
   case CheckConformancesQueryID:
      return true;
   case CheckSingleConformanceQueryID:
      return true;
   case ConformsToQueryID:
      return true;
   case GetTypeSizeQueryID:
      return true;
   case GetTypeAlignmentQueryID:
      return true;
   case GetTypeStrideQueryID:
      return true;
   case CalculateRecordSizeQueryID:
      return true;
   case IsAccessibleQueryID:
      return true;
   case CheckAccessibilityQueryID:
      return true;
   case TypeCheckDeclContextQueryID:
      return true;
   case PrepareDeclInterfaceQueryID:
      return true;
   case TypecheckDeclQueryID:
      return true;
   case PrepareCallableInterfaceQueryID:
      return true;
   case TypecheckCallableQueryID:
      return true;
   case PrepareFunctionInterfaceQueryID:
      return true;
   case TypecheckFunctionQueryID:
      return true;
   case PrepareMethodInterfaceQueryID:
      return true;
   case TypecheckMethodQueryID:
      return true;
   case PrepareInitInterfaceQueryID:
      return true;
   case TypecheckInitQueryID:
      return true;
   case PrepareDeinitInterfaceQueryID:
      return true;
   case TypecheckDeinitQueryID:
      return true;
   case PrepareFuncArgInterfaceQueryID:
      return true;
   case TypecheckFuncArgQueryID:
      return true;
   case PrepareTemplateParamInterfaceQueryID:
      return true;
   case TypecheckTemplateParamQueryID:
      return true;
   case PrepareAssociatedTypeInterfaceQueryID:
      return true;
   case TypecheckAssociatedTypeQueryID:
      return true;
   case PrepareRecordInterfaceQueryID:
      return true;
   case TypecheckRecordQueryID:
      return true;
   case PrepareStructInterfaceQueryID:
      return true;
   case TypecheckStructQueryID:
      return true;
   case PrepareClassInterfaceQueryID:
      return true;
   case TypecheckClassQueryID:
      return true;
   case PrepareEnumInterfaceQueryID:
      return true;
   case TypecheckEnumQueryID:
      return true;
   case PrepareProtocolInterfaceQueryID:
      return true;
   case TypecheckProtocolQueryID:
      return true;
   case PrepareExtensionInterfaceQueryID:
      return true;
   case TypecheckExtensionQueryID:
      return true;
   case PrepareFieldInterfaceQueryID:
      return true;
   case TypecheckFieldQueryID:
      return true;
   case PreparePropInterfaceQueryID:
      return true;
   case TypecheckPropQueryID:
      return true;
   case PrepareSubscriptInterfaceQueryID:
      return true;
   case TypecheckSubscriptQueryID:
      return true;
   case AssignInitNameQueryID:
      return true;
   case LookupFirstQueryID:
      return true;
   case LookupSingleQueryID:
      return true;
   case MultiLevelLookupQueryID:
      return true;
   case DirectLookupQueryID:
      return true;
   case NestedNameLookupQueryID:
      return false;
   case PrepareNameLookupQueryID:
      return true;
   case GetAssociatedTypeQueryID:
      return false;
   case CheckTypeEquivalenceQueryID:
      return true;
   case InstantiateProtocolDefaultImplQueryID:
      return true;
   case InstantiateRecordQueryID:
      return true;
   case CheckTemplateExtensionApplicabilityQueryID:
      return true;
   case InstantiateTemplateMemberQueryID:
      return true;
   case PrepareInstantiationForNameLookupQueryID:
      return true;
   case GetILModuleForDeclQueryID:
      return true;
   case GenerateILForContextQueryID:
      return true;
   case GenerateRecordILQueryID:
      return true;
   case GetILGlobalQueryID:
      return true;
   case GenerateLazyILGlobalDefinitionQueryID:
      return true;
   case GetILFunctionQueryID:
      return true;
   case GenerateILFunctionBodyQueryID:
      return true;
   case GetILTypeInfoQueryID:
      return true;
   case CreateILBasicTypeInfoQueryID:
      return true;
   case CreateILRecordTypeInfoQueryID:
      return true;
   case OpenFileForReadQueryID:
      return true;
   case OpenTmpFileQueryID:
      return false;
   case GetDefaultTemplateArgQueryID:
      return true;
   case CreateSelfArgumentQueryID:
      return false;
   case FindEquivalentDeclQueryID:
      return true;
   case EquivalentSignaturesQueryID:
      return true;
   }
}

bool Query::canBeSerialized() const
{
   switch (K) {
   case CompileModuleQueryID:
      return false;
   case ParseSourceFileQueryID:
      return false;
   case TypeCheckASTQueryID:
      return false;
   case CreateILModuleQueryID:
      return false;
   case SetupIRGenQueryID:
      return false;
   case CreateLLVMModuleQueryID:
      return false;
   case CreateObjectQueryID:
      return false;
   case CreateStaticLibQueryID:
      return false;
   case CreateDynamicLibQueryID:
      return false;
   case CreateExecutableQueryID:
      return false;
   case EmitILQueryID:
      return false;
   case EmitIRQueryID:
      return false;
   case PrintUsedMemoryQueryID:
      return false;
   case MeasureExecutionTimeQueryID:
      return false;
   case IsEquatableQueryID:
      return false;
   case IsCopyableQueryID:
      return false;
   case IsMoveOnlyQueryID:
      return false;
   case IsImplicitlyCopyableQueryID:
      return false;
   case IsTriviallyCopyableQueryID:
      return false;
   case NeedsRetainOrReleaseQueryID:
      return false;
   case NeedsDeinitilizationQueryID:
      return false;
   case NeedsStructReturnQueryID:
      return false;
   case SubstAssociatedTypesQueryID:
      return false;
   case SubstTemplateParamTypesQueryID:
      return false;
   case GetBuiltinModuleQueryID:
      return false;
   case GetBuiltinFuncQueryID:
      return false;
   case GetBuiltinRecordQueryID:
      return false;
   case GetBuiltinProtocolQueryID:
      return false;
   case FindPrecedenceGroupQueryID:
      return false;
   case FindOperatorQueryID:
      return false;
   case ResolveImportQueryID:
      return false;
   case ResolveUsingQueryID:
      return false;
   case ResolveStaticIfQueryID:
      return false;
   case ResolveStaticForQueryID:
      return false;
   case ResolveStaticAssertQueryID:
      return false;
   case ResolveStaticPrintQueryID:
      return false;
   case ResolveStaticExprQueryID:
      return false;
   case ResolveStaticExprToBoolQueryID:
      return false;
   case ExpandMacroQueryID:
      return false;
   case ExpandMacroDeclQueryID:
      return false;
   case ExpandMacrosQueryID:
      return false;
   case FindExtensionsQueryID:
      return false;
   case GetExtensionTypeKindQueryID:
      return false;
   case MatchExtensionTypeQueryID:
      return false;
   case ExtensionAppliesQueryID:
      return false;
   case GetExtendedDeclQueryID:
      return false;
   case ResolveExtensionQueryID:
      return false;
   case ResolveConformanceToProtocolQueryID:
      return false;
   case ResolveDeclaredConformancesQueryID:
      return false;
   case AddInheritedConformanceQueryID:
      return false;
   case ResolveAssociatedTypesQueryID:
      return false;
   case GetReferencedAssociatedTypesQueryID:
      return false;
   case ReferencedAssociatedTypesReadyQueryID:
      return false;
   case CheckProtocolExtensionApplicabilityQueryID:
      return false;
   case VerifyConstraintQueryID:
      return false;
   case IsConstraintSatisfiedQueryID:
      return false;
   case CheckConformancesQueryID:
      return false;
   case CheckSingleConformanceQueryID:
      return false;
   case ConformsToQueryID:
      return false;
   case GetTypeSizeQueryID:
      return false;
   case GetTypeAlignmentQueryID:
      return false;
   case GetTypeStrideQueryID:
      return false;
   case CalculateRecordSizeQueryID:
      return false;
   case IsAccessibleQueryID:
      return false;
   case CheckAccessibilityQueryID:
      return false;
   case TypeCheckDeclContextQueryID:
      return false;
   case PrepareDeclInterfaceQueryID:
      return false;
   case TypecheckDeclQueryID:
      return false;
   case PrepareCallableInterfaceQueryID:
      return false;
   case TypecheckCallableQueryID:
      return false;
   case PrepareFunctionInterfaceQueryID:
      return false;
   case TypecheckFunctionQueryID:
      return false;
   case PrepareMethodInterfaceQueryID:
      return false;
   case TypecheckMethodQueryID:
      return false;
   case PrepareInitInterfaceQueryID:
      return false;
   case TypecheckInitQueryID:
      return false;
   case PrepareDeinitInterfaceQueryID:
      return false;
   case TypecheckDeinitQueryID:
      return false;
   case PrepareFuncArgInterfaceQueryID:
      return false;
   case TypecheckFuncArgQueryID:
      return false;
   case PrepareTemplateParamInterfaceQueryID:
      return false;
   case TypecheckTemplateParamQueryID:
      return false;
   case PrepareAssociatedTypeInterfaceQueryID:
      return false;
   case TypecheckAssociatedTypeQueryID:
      return false;
   case PrepareRecordInterfaceQueryID:
      return false;
   case TypecheckRecordQueryID:
      return false;
   case PrepareStructInterfaceQueryID:
      return false;
   case TypecheckStructQueryID:
      return false;
   case PrepareClassInterfaceQueryID:
      return false;
   case TypecheckClassQueryID:
      return false;
   case PrepareEnumInterfaceQueryID:
      return false;
   case TypecheckEnumQueryID:
      return false;
   case PrepareProtocolInterfaceQueryID:
      return false;
   case TypecheckProtocolQueryID:
      return false;
   case PrepareExtensionInterfaceQueryID:
      return false;
   case TypecheckExtensionQueryID:
      return false;
   case PrepareFieldInterfaceQueryID:
      return false;
   case TypecheckFieldQueryID:
      return false;
   case PreparePropInterfaceQueryID:
      return false;
   case TypecheckPropQueryID:
      return false;
   case PrepareSubscriptInterfaceQueryID:
      return false;
   case TypecheckSubscriptQueryID:
      return false;
   case AssignInitNameQueryID:
      return false;
   case LookupFirstQueryID:
      return false;
   case LookupSingleQueryID:
      return false;
   case MultiLevelLookupQueryID:
      return false;
   case DirectLookupQueryID:
      return false;
   case NestedNameLookupQueryID:
      return false;
   case PrepareNameLookupQueryID:
      return false;
   case GetAssociatedTypeQueryID:
      return false;
   case CheckTypeEquivalenceQueryID:
      return false;
   case InstantiateProtocolDefaultImplQueryID:
      return false;
   case InstantiateRecordQueryID:
      return false;
   case CheckTemplateExtensionApplicabilityQueryID:
      return false;
   case InstantiateTemplateMemberQueryID:
      return false;
   case PrepareInstantiationForNameLookupQueryID:
      return false;
   case GetILModuleForDeclQueryID:
      return false;
   case GenerateILForContextQueryID:
      return false;
   case GenerateRecordILQueryID:
      return false;
   case GetILGlobalQueryID:
      return false;
   case GenerateLazyILGlobalDefinitionQueryID:
      return false;
   case GetILFunctionQueryID:
      return false;
   case GenerateILFunctionBodyQueryID:
      return false;
   case GetILTypeInfoQueryID:
      return false;
   case CreateILBasicTypeInfoQueryID:
      return false;
   case CreateILRecordTypeInfoQueryID:
      return false;
   case OpenFileForReadQueryID:
      return false;
   case OpenTmpFileQueryID:
      return false;
   case GetDefaultTemplateArgQueryID:
      return false;
   case CreateSelfArgumentQueryID:
      return false;
   case FindEquivalentDeclQueryID:
      return false;
   case EquivalentSignaturesQueryID:
      return false;
   }
}

#endif

#ifdef CDOT_QUERY_CONTEXT_FIELDS

/// \brief Single query instance.
private:
CompileModuleQuery* CompileModuleInstance = nullptr;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<StringRef, ParseSourceFileQuery*> ParseSourceFileQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::ModuleDecl*, TypeCheckASTQuery*> TypeCheckASTQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<llvm::ArrayRef<StringRef>, CreateILModuleQuery*>
    CreateILModuleQueries;

/// \brief Single query instance.
private:
SetupIRGenQuery* SetupIRGenInstance = nullptr;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<llvm::ArrayRef<StringRef>, CreateLLVMModuleQuery*>
    CreateLLVMModuleQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<Query*, MeasureExecutionTimeQuery*> MeasureExecutionTimeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, IsEquatableQuery*> IsEquatableQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, IsCopyableQuery*> IsCopyableQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, IsMoveOnlyQuery*> IsMoveOnlyQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, IsImplicitlyCopyableQuery*>
    IsImplicitlyCopyableQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, IsTriviallyCopyableQuery*> IsTriviallyCopyableQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, NeedsRetainOrReleaseQuery*>
    NeedsRetainOrReleaseQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, NeedsDeinitilizationQuery*>
    NeedsDeinitilizationQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, NeedsStructReturnQuery*> NeedsStructReturnQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<SubstAssociatedTypesQuery> SubstAssociatedTypesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<SubstTemplateParamTypesQuery> SubstTemplateParamTypesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<char, GetBuiltinModuleQuery*> GetBuiltinModuleQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<char, GetBuiltinFuncQuery*> GetBuiltinFuncQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<char, GetBuiltinRecordQuery*> GetBuiltinRecordQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<char, GetBuiltinProtocolQuery*> GetBuiltinProtocolQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<FindPrecedenceGroupQuery> FindPrecedenceGroupQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<FindOperatorQuery> FindOperatorQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::ImportDecl*, ResolveImportQuery*> ResolveImportQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::UsingDecl*, ResolveUsingQuery*> ResolveUsingQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::StaticIfDecl*, ResolveStaticIfQuery*>
    ResolveStaticIfQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::StaticForDecl*, ResolveStaticForQuery*>
    ResolveStaticForQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::StaticAssertDecl*, ResolveStaticAssertQuery*>
    ResolveStaticAssertQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::StaticPrintDecl*, ResolveStaticPrintQuery*>
    ResolveStaticPrintQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::StaticExpr*, ResolveStaticExprQuery*>
    ResolveStaticExprQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<ResolveStaticExprToBoolQuery> ResolveStaticExprToBoolQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<ExpandMacroQuery> ExpandMacroQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::MacroExpansionDecl*, ExpandMacroDeclQuery*>
    ExpandMacroDeclQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<StringRef, ExpandMacrosQuery*> ExpandMacrosQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, FindExtensionsQuery*> FindExtensionsQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::SourceType, GetExtensionTypeKindQuery*>
    GetExtensionTypeKindQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<MatchExtensionTypeQuery> MatchExtensionTypeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<ExtensionAppliesQuery> ExtensionAppliesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::SourceType, GetExtendedDeclQuery*> GetExtendedDeclQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::ExtensionDecl*, ResolveExtensionQuery*>
    ResolveExtensionQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<ResolveConformanceToProtocolQuery>
    ResolveConformanceToProtocolQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<ResolveDeclaredConformancesQuery>
    ResolveDeclaredConformancesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<AddInheritedConformanceQuery> AddInheritedConformanceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, ResolveAssociatedTypesQuery*>
    ResolveAssociatedTypesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<GetReferencedAssociatedTypesQuery>
    GetReferencedAssociatedTypesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<CheckProtocolExtensionApplicabilityQuery>
    CheckProtocolExtensionApplicabilityQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<VerifyConstraintQuery> VerifyConstraintQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<IsConstraintSatisfiedQuery> IsConstraintSatisfiedQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, CheckConformancesQuery*> CheckConformancesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<CheckSingleConformanceQuery> CheckSingleConformanceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<ConformsToQuery> ConformsToQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, GetTypeSizeQuery*> GetTypeSizeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, GetTypeAlignmentQuery*> GetTypeAlignmentQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, GetTypeStrideQuery*> GetTypeStrideQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::RecordDecl*, CalculateRecordSizeQuery*>
    CalculateRecordSizeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<IsAccessibleQuery> IsAccessibleQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<CheckAccessibilityQuery> CheckAccessibilityQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::DeclContext*, TypeCheckDeclContextQuery*>
    TypeCheckDeclContextQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::Decl*, PrepareDeclInterfaceQuery*>
    PrepareDeclInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::Decl*, TypecheckDeclQuery*> TypecheckDeclQueries;

private:
llvm::DenseMap<IdentifierInfo*, ast::CallableDecl*> ExternCFuncs;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::CallableDecl*, PrepareCallableInterfaceQuery*>
    PrepareCallableInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::CallableDecl*, TypecheckCallableQuery*>
    TypecheckCallableQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::FunctionDecl*, PrepareFunctionInterfaceQuery*>
    PrepareFunctionInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::FunctionDecl*, TypecheckFunctionQuery*>
    TypecheckFunctionQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::MethodDecl*, PrepareMethodInterfaceQuery*>
    PrepareMethodInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::MethodDecl*, TypecheckMethodQuery*> TypecheckMethodQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::InitDecl*, PrepareInitInterfaceQuery*>
    PrepareInitInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::InitDecl*, TypecheckInitQuery*> TypecheckInitQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::DeinitDecl*, PrepareDeinitInterfaceQuery*>
    PrepareDeinitInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::DeinitDecl*, TypecheckDeinitQuery*> TypecheckDeinitQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::FuncArgDecl*, PrepareFuncArgInterfaceQuery*>
    PrepareFuncArgInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::FuncArgDecl*, TypecheckFuncArgQuery*>
    TypecheckFuncArgQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::TemplateParamDecl*, PrepareTemplateParamInterfaceQuery*>
    PrepareTemplateParamInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::TemplateParamDecl*, TypecheckTemplateParamQuery*>
    TypecheckTemplateParamQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::AssociatedTypeDecl*, PrepareAssociatedTypeInterfaceQuery*>
    PrepareAssociatedTypeInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::AssociatedTypeDecl*, TypecheckAssociatedTypeQuery*>
    TypecheckAssociatedTypeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::RecordDecl*, PrepareRecordInterfaceQuery*>
    PrepareRecordInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::RecordDecl*, TypecheckRecordQuery*> TypecheckRecordQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::StructDecl*, PrepareStructInterfaceQuery*>
    PrepareStructInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::StructDecl*, TypecheckStructQuery*> TypecheckStructQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::ClassDecl*, PrepareClassInterfaceQuery*>
    PrepareClassInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::ClassDecl*, TypecheckClassQuery*> TypecheckClassQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::EnumDecl*, PrepareEnumInterfaceQuery*>
    PrepareEnumInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::EnumDecl*, TypecheckEnumQuery*> TypecheckEnumQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::ProtocolDecl*, PrepareProtocolInterfaceQuery*>
    PrepareProtocolInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::ProtocolDecl*, TypecheckProtocolQuery*>
    TypecheckProtocolQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::ExtensionDecl*, PrepareExtensionInterfaceQuery*>
    PrepareExtensionInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::ExtensionDecl*, TypecheckExtensionQuery*>
    TypecheckExtensionQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::FieldDecl*, PrepareFieldInterfaceQuery*>
    PrepareFieldInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::FieldDecl*, TypecheckFieldQuery*> TypecheckFieldQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::PropDecl*, PreparePropInterfaceQuery*>
    PreparePropInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::PropDecl*, TypecheckPropQuery*> TypecheckPropQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::SubscriptDecl*, PrepareSubscriptInterfaceQuery*>
    PrepareSubscriptInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::SubscriptDecl*, TypecheckSubscriptQuery*>
    TypecheckSubscriptQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::InitDecl*, AssignInitNameQuery*> AssignInitNameQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<LookupFirstQuery> LookupFirstQueries;

public:
template<class T>
T* LookupSingleAs(ast::DeclContext* DC, DeclarationName Name,
                  bool LocalLookup = true, bool DiagOnError = true)
{
   ast::NamedDecl* Result;
   if (LookupSingle(Result, DC, Name, LocalLookup, DiagOnError)) {
      return nullptr;
   }

   return support::dyn_cast_or_null<T>(Result);
}

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<LookupSingleQuery> LookupSingleQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<MultiLevelLookupQuery> MultiLevelLookupQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<DirectLookupQuery> DirectLookupQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::DeclContext*, PrepareNameLookupQuery*>
    PrepareNameLookupQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<CheckTypeEquivalenceQuery> CheckTypeEquivalenceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<InstantiateProtocolDefaultImplQuery>
    InstantiateProtocolDefaultImplQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<InstantiateRecordQuery> InstantiateRecordQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<CheckTemplateExtensionApplicabilityQuery>
    CheckTemplateExtensionApplicabilityQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<InstantiateTemplateMemberQuery>
    InstantiateTemplateMemberQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::RecordDecl*, PrepareInstantiationForNameLookupQuery*>
    PrepareInstantiationForNameLookupQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::Decl*, GetILModuleForDeclQuery*> GetILModuleForDeclQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::DeclContext*, GenerateILForContextQuery*>
    GenerateILForContextQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::RecordDecl*, GenerateRecordILQuery*>
    GenerateRecordILQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::VarDecl*, GetILGlobalQuery*> GetILGlobalQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::VarDecl*, GenerateLazyILGlobalDefinitionQuery*>
    GenerateLazyILGlobalDefinitionQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::CallableDecl*, GetILFunctionQuery*> GetILFunctionQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::CallableDecl*, GenerateILFunctionBodyQuery*>
    GenerateILFunctionBodyQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, GetILTypeInfoQuery*> GetILTypeInfoQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<QualType, CreateILBasicTypeInfoQuery*>
    CreateILBasicTypeInfoQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::RecordDecl*, CreateILRecordTypeInfoQuery*>
    CreateILRecordTypeInfoQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<StringRef, OpenFileForReadQuery*> OpenFileForReadQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::DenseMap<ast::TemplateParamDecl*, GetDefaultTemplateArgQuery*>
    GetDefaultTemplateArgQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<FindEquivalentDeclQuery> FindEquivalentDeclQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:
llvm::FoldingSet<EquivalentSignaturesQuery> EquivalentSignaturesQueries;
#endif

#ifdef CDOT_QUERY_CONTEXT_DECLS
public:
QueryResult CompileModule(SourceRange Loc = {});

public:
QueryResult ParseSourceFile(ast::ModuleDecl*& Result, StringRef FileName,
                            SourceRange Loc = {});

public:
QueryResult TypeCheckAST(ast::ModuleDecl* Mod, SourceRange Loc = {});

public:
QueryResult CreateILModule(il::Module*& Result,
                           llvm::ArrayRef<StringRef> SourceFiles,
                           SourceRange Loc = {});

public:
QueryResult SetupIRGen(il::IRGen*& Result, SourceRange Loc = {});

public:
QueryResult CreateLLVMModule(llvm::Module*& Result,
                             llvm::ArrayRef<StringRef> SourceFiles,
                             SourceRange Loc = {});

public:
QueryResult CreateObject(llvm::ArrayRef<StringRef> SourceFiles,
                         llvm::raw_ostream& OS, SourceRange Loc = {});

public:
QueryResult CreateStaticLib(llvm::ArrayRef<StringRef> SourceFiles,
                            StringRef OutFile, SourceRange Loc = {});

public:
QueryResult CreateDynamicLib(llvm::ArrayRef<StringRef> SourceFiles,
                             StringRef OutFile, SourceRange Loc = {});

public:
QueryResult CreateExecutable(llvm::ArrayRef<StringRef> SourceFiles,
                             StringRef OutFile, SourceRange Loc = {});

public:
QueryResult EmitIL(llvm::ArrayRef<StringRef> SourceFiles, llvm::raw_ostream& OS,
                   SourceRange Loc = {});

public:
QueryResult EmitIR(llvm::ArrayRef<StringRef> SourceFiles, llvm::raw_ostream& OS,
                   SourceRange Loc = {});

public:
QueryResult PrintUsedMemory(SourceRange Loc = {});

public:
QueryResult MeasureExecutionTime(long long& Result, Query* Q,
                                 SourceRange Loc = {});

public:
QueryResult IsEquatable(bool& Result, QualType T, SourceRange Loc = {});

public:
QueryResult IsCopyable(bool& Result, QualType T, SourceRange Loc = {});

public:
QueryResult IsMoveOnly(bool& Result, QualType T, SourceRange Loc = {});

public:
QueryResult IsImplicitlyCopyable(bool& Result, QualType T,
                                 SourceRange Loc = {});

public:
QueryResult IsTriviallyCopyable(bool& Result, QualType T, SourceRange Loc = {});

public:
QueryResult NeedsRetainOrRelease(bool& Result, QualType T,
                                 SourceRange Loc = {});

public:
QueryResult NeedsDeinitilization(bool& Result, QualType T,
                                 SourceRange Loc = {});

public:
QueryResult NeedsStructReturn(bool& Result, QualType T, SourceRange Loc = {});

public:
QueryResult SubstAssociatedTypes(QualType& Result, QualType T, QualType Self,
                                 SourceRange Loc = {});

public:
QueryResult
SubstTemplateParamTypes(QualType& Result, QualType T,
                        sema::MultiLevelFinalTemplateArgList TemplateArgs,
                        SourceRange Loc = {});

public:
QueryResult GetBuiltinModule(Module*& Result,
                             GetBuiltinModuleQuery::ModuleKind Mod,
                             SourceRange Loc = {});

public:
QueryResult GetBuiltinFunc(ast::CallableDecl*& Result,
                           GetBuiltinFuncQuery::FunctionKind Fn,
                           SourceRange Loc = {});

public:
QueryResult GetBuiltinRecord(ast::RecordDecl*& Result,
                             GetBuiltinRecordQuery::RecordKind R,
                             SourceRange Loc = {});

public:
QueryResult GetBuiltinProtocol(ast::ProtocolDecl*& Result,
                               GetBuiltinProtocolQuery::ProtocolKind P,
                               SourceRange Loc = {});

public:
QueryResult FindPrecedenceGroup(ast::PrecedenceGroupDecl*& Result,
                                DeclarationName Name, bool DiagOnError = true,
                                SourceRange Loc = {});

public:
QueryResult FindOperator(ast::OperatorDecl*& Result, DeclarationName Name,
                         ast::DeclContext* DC, bool DiagOnError = true,
                         SourceRange Loc = {});

public:
QueryResult ResolveImport(ast::ImportDecl* I, SourceRange Loc = {});

public:
QueryResult ResolveUsing(ast::UsingDecl* U, SourceRange Loc = {});

public:
QueryResult ResolveStaticIf(ast::Decl*& Result, ast::StaticIfDecl* Decl,
                            SourceRange Loc = {});

public:
QueryResult ResolveStaticFor(ast::Decl*& Result, ast::StaticForDecl* Decl,
                             SourceRange Loc = {});

public:
QueryResult ResolveStaticAssert(ast::StaticAssertDecl* Decl,
                                SourceRange Loc = {});

public:
QueryResult ResolveStaticPrint(ast::StaticPrintDecl* Decl,
                               SourceRange Loc = {});

public:
QueryResult ResolveStaticExpr(il::Constant*& Result, ast::StaticExpr* Expr,
                              SourceRange Loc = {});

public:
QueryResult ResolveStaticExprToBool(bool& Result, ast::StaticExpr* Expr,
                                    bool DiagOnError, SourceRange Loc = {});

public:
QueryResult ExpandMacro(StmtOrDecl& Result, StmtOrDecl SOD,
                        DeclarationName Name, ast::DeclContext* DC,
                        unsigned Delim, llvm::ArrayRef<lex::Token> Tokens,
                        unsigned ExpectedKind, SourceRange Loc = {});

public:
QueryResult ExpandMacroDecl(ast::Decl*& Result, ast::MacroExpansionDecl* Decl,
                            SourceRange Loc = {});

public:
QueryResult ExpandMacros(StringRef SourceFile, SourceRange Loc = {});

public:
QueryResult FindExtensions(QualType T, SourceRange Loc = {});

public:
QueryResult GetExtensionTypeKind(GetExtensionTypeKindQuery::ResultKind& Result,
                                 ast::SourceType T, SourceRange Loc = {});

public:
QueryResult MatchExtensionType(MatchExtensionTypeQuery::ResultKind& Result,
                               QualType T, ast::SourceType PatternTy,
                               SourceRange Loc = {});

public:
QueryResult ExtensionApplies(MatchExtensionTypeQuery::ResultKind& Result,
                             ast::ExtensionDecl* Ext, QualType T,
                             SourceRange Loc = {});

public:
QueryResult GetExtendedDecl(ast::NamedDecl*& Result, ast::SourceType ExtendedTy,
                            SourceRange Loc = {});

public:
QueryResult ResolveExtension(ast::ExtensionDecl* Ext, SourceRange Loc = {});

public:
QueryResult ResolveConformanceToProtocol(ast::ProtocolDecl*& Result, QualType T,
                                         ast::SourceType Conf,
                                         SourceRange Loc = {});

public:
QueryResult
ResolveDeclaredConformances(QualType T,
                            llvm::ArrayRef<ast::SourceType> Conformances,
                            SourceRange Loc = {});

public:
QueryResult AddInheritedConformance(QualType T, ast::ProtocolDecl* Proto,
                                    SourceRange Loc = {});

public:
QueryResult ResolveAssociatedTypes(QualType T, SourceRange Loc = {});

public:
QueryResult
GetReferencedAssociatedTypes(ArrayRef<ast::AssociatedTypeDecl*>& Result,
                             ast::NamedDecl* ConstrainedDecl,
                             ast::DeclConstraint* C, SourceRange Loc = {});

public:
QueryResult ReferencedAssociatedTypesReady(bool& Result, QualType T,
                                           ast::NamedDecl* ConstrainedDecl,
                                           SourceRange Loc = {});

public:
QueryResult CheckProtocolExtensionApplicability(
    CheckProtocolExtensionApplicabilityQuery::ResultKind& Result, QualType T,
    ast::ExtensionDecl* Ext, SourceRange Loc = {});

public:
QueryResult VerifyConstraint(ast::DeclConstraint* C,
                             ast::NamedDecl* ConstrainedDecl,
                             SourceRange Loc = {});

public:
QueryResult IsConstraintSatisfied(bool& Result, ast::DeclConstraint* C,
                                  QualType Self, ast::NamedDecl* OriginalDecl,
                                  SourceRange Loc = {});

public:
QueryResult CheckConformances(QualType T, SourceRange Loc = {});

public:
QueryResult CheckSingleConformance(QualType T, ast::ProtocolDecl* P,
                                   SourceRange Loc = {});

public:
QueryResult ConformsTo(bool& Result, QualType T, ast::ProtocolDecl* P,
                       SourceRange Loc = {});

public:
QueryResult GetTypeSize(unsigned& Result, QualType T, SourceRange Loc = {});

public:
QueryResult GetTypeAlignment(unsigned short& Result, QualType T,
                             SourceRange Loc = {});

public:
QueryResult GetTypeStride(unsigned& Result, QualType T, SourceRange Loc = {});

public:
QueryResult CalculateRecordSize(unsigned& Result, ast::RecordDecl* R,
                                SourceRange Loc = {});

public:
QueryResult IsAccessible(bool& Result, ast::DeclContext* DC, ast::NamedDecl* ND,
                         SourceRange Loc = {});

public:
QueryResult CheckAccessibility(ast::DeclContext* DC, ast::NamedDecl* ND,
                               SourceRange Loc = {});

public:
QueryResult TypeCheckDeclContext(ast::DeclContext* DC, SourceRange Loc = {});

public:
QueryResult PrepareDeclInterface(ast::Decl* D, SourceRange Loc = {});

public:
QueryResult TypecheckDecl(ast::Decl* D, SourceRange Loc = {});

private:
QueryResult PrepareCallableInterface(ast::CallableDecl* D,
                                     SourceRange Loc = {});

private:
QueryResult TypecheckCallable(ast::CallableDecl* D, SourceRange Loc = {});

private:
QueryResult PrepareFunctionInterface(ast::FunctionDecl* D,
                                     SourceRange Loc = {});

private:
QueryResult TypecheckFunction(ast::FunctionDecl* D, SourceRange Loc = {});

private:
QueryResult PrepareMethodInterface(ast::MethodDecl* D, SourceRange Loc = {});

private:
QueryResult TypecheckMethod(ast::MethodDecl* D, SourceRange Loc = {});

private:
QueryResult PrepareInitInterface(ast::InitDecl* D, SourceRange Loc = {});

private:
QueryResult TypecheckInit(ast::InitDecl* D, SourceRange Loc = {});

private:
QueryResult PrepareDeinitInterface(ast::DeinitDecl* D, SourceRange Loc = {});

private:
QueryResult TypecheckDeinit(ast::DeinitDecl* D, SourceRange Loc = {});

private:
QueryResult PrepareFuncArgInterface(ast::FuncArgDecl* D, SourceRange Loc = {});

private:
QueryResult TypecheckFuncArg(ast::FuncArgDecl* D, SourceRange Loc = {});

private:
QueryResult PrepareTemplateParamInterface(ast::TemplateParamDecl* D,
                                          SourceRange Loc = {});

private:
QueryResult TypecheckTemplateParam(ast::TemplateParamDecl* D,
                                   SourceRange Loc = {});

private:
QueryResult PrepareAssociatedTypeInterface(ast::AssociatedTypeDecl* D,
                                           SourceRange Loc = {});

private:
QueryResult TypecheckAssociatedType(ast::AssociatedTypeDecl* D,
                                    SourceRange Loc = {});

private:
QueryResult PrepareRecordInterface(ast::RecordDecl* D, SourceRange Loc = {});

private:
QueryResult TypecheckRecord(ast::RecordDecl* D, SourceRange Loc = {});

private:
QueryResult PrepareStructInterface(ast::StructDecl* D, SourceRange Loc = {});

private:
QueryResult TypecheckStruct(ast::StructDecl* D, SourceRange Loc = {});

private:
QueryResult PrepareClassInterface(ast::ClassDecl* D, SourceRange Loc = {});

private:
QueryResult TypecheckClass(ast::ClassDecl* D, SourceRange Loc = {});

private:
QueryResult PrepareEnumInterface(ast::EnumDecl* D, SourceRange Loc = {});

private:
QueryResult TypecheckEnum(ast::EnumDecl* D, SourceRange Loc = {});

private:
QueryResult PrepareProtocolInterface(ast::ProtocolDecl* D,
                                     SourceRange Loc = {});

private:
QueryResult TypecheckProtocol(ast::ProtocolDecl* D, SourceRange Loc = {});

private:
QueryResult PrepareExtensionInterface(ast::ExtensionDecl* D,
                                      SourceRange Loc = {});

private:
QueryResult TypecheckExtension(ast::ExtensionDecl* D, SourceRange Loc = {});

private:
QueryResult PrepareFieldInterface(ast::FieldDecl* D, SourceRange Loc = {});

private:
QueryResult TypecheckField(ast::FieldDecl* D, SourceRange Loc = {});

private:
QueryResult PreparePropInterface(ast::PropDecl* D, SourceRange Loc = {});

private:
QueryResult TypecheckProp(ast::PropDecl* D, SourceRange Loc = {});

private:
QueryResult PrepareSubscriptInterface(ast::SubscriptDecl* D,
                                      SourceRange Loc = {});

private:
QueryResult TypecheckSubscript(ast::SubscriptDecl* D, SourceRange Loc = {});

public:
QueryResult AssignInitName(ast::InitDecl* Init, SourceRange Loc = {});

public:
QueryResult LookupFirst(const SingleLevelLookupResult*& Result,
                        ast::DeclContext* DC, DeclarationName Name,
                        bool LocalLookup = true, SourceRange Loc = {});

public:
QueryResult LookupSingle(ast::NamedDecl*& Result, ast::DeclContext* DC,
                         DeclarationName Name, bool LocalLookup = true,
                         bool DiagOnError = true, SourceRange Loc = {});

public:
QueryResult MultiLevelLookup(const MultiLevelLookupResult*& Result,
                             ast::DeclContext* DC, DeclarationName Name,
                             bool LocalLookup = true, SourceRange Loc = {});

public:
QueryResult DirectLookup(const MultiLevelLookupResult*& Result,
                         ast::DeclContext* DC, DeclarationName Name,
                         bool LookInExtensions = true,
                         bool PrepareNameLookup = true, SourceRange Loc = {});

public:
QueryResult NestedNameLookup(const MultiLevelLookupResult*& Result,
                             ast::DeclContext* DC,
                             llvm::ArrayRef<DeclarationName> Names,
                             SourceRange Loc = {});

public:
QueryResult PrepareNameLookup(ast::DeclContext* DC, SourceRange Loc = {});

public:
QueryResult GetAssociatedType(ast::AssociatedTypeDecl*& Result,
                              ast::RecordDecl* R, DeclarationName Name,
                              llvm::ArrayRef<ast::ExtensionDecl*> Extensions,
                              SourceRange Loc = {});

public:
QueryResult CheckTypeEquivalence(bool& Result, QualType LHS, QualType RHS,
                                 QualType Self,
                                 ast::DeclContext* LHSDecl = nullptr,
                                 SourceRange Loc = {});

public:
QueryResult InstantiateProtocolDefaultImpl(ast::NamedDecl*& Result,
                                           ast::NamedDecl* Impl, QualType Self,
                                           SourceRange Loc = {});

public:
QueryResult InstantiateRecord(ast::RecordDecl*& Result,
                              ast::RecordDecl* Template,
                              sema::FinalTemplateArgumentList* TemplateArgs,
                              SourceLocation PointOfInstantiation,
                              SourceRange Loc = {});

public:
QueryResult CheckTemplateExtensionApplicability(bool& Result,
                                                ast::RecordDecl* Inst,
                                                ast::ExtensionDecl* Ext,
                                                SourceRange Loc = {});

public:
QueryResult InstantiateTemplateMember(ast::NamedDecl*& Result,
                                      ast::NamedDecl* TemplateMember,
                                      ast::RecordDecl* Inst,
                                      SourceRange Loc = {});

public:
QueryResult PrepareInstantiationForNameLookup(ast::RecordDecl* Inst,
                                              SourceRange Loc = {});

public:
QueryResult GetILModuleForDecl(il::Module*& Result, ast::Decl* D,
                               SourceRange Loc = {});

public:
QueryResult GenerateILForContext(ast::DeclContext* DC, SourceRange Loc = {});

private:
QueryResult GenerateRecordIL(ast::RecordDecl* R, SourceRange Loc = {});

public:
QueryResult GetILGlobal(il::GlobalVariable*& Result, ast::VarDecl* GV,
                        SourceRange Loc = {});

private:
QueryResult GenerateLazyILGlobalDefinition(ast::VarDecl* GV,
                                           SourceRange Loc = {});

public:
QueryResult GetILFunction(il::Function*& Result, ast::CallableDecl* C,
                          SourceRange Loc = {});

private:
QueryResult GenerateILFunctionBody(ast::CallableDecl* C, SourceRange Loc = {});

public:
QueryResult GetILTypeInfo(il::GlobalVariable*& Result, QualType T,
                          SourceRange Loc = {});

private:
QueryResult CreateILBasicTypeInfo(il::Constant*& Result, QualType T,
                                  SourceRange Loc = {});

private:
QueryResult CreateILRecordTypeInfo(il::Constant*& Result, ast::RecordDecl* R,
                                   SourceRange Loc = {});

public:
QueryResult OpenFileForRead(llvm::MemoryBuffer*& Result, StringRef FileName,
                            SourceRange Loc = {});

public:
QueryResult OpenTmpFile(llvm::raw_ostream*& Result, StringRef Extension,
                        SourceRange Loc = {});

public:
QueryResult GetDefaultTemplateArg(const sema::TemplateArgument*& Result,
                                  ast::TemplateParamDecl* P,
                                  SourceRange Loc = {});

public:
QueryResult CreateSelfArgument(ast::FuncArgDecl*& Result, QualType Self,
                               SourceLocation SelfLoc, SourceRange Loc = {});

public:
QueryResult FindEquivalentDecl(ast::NamedDecl*& Result, ast::NamedDecl* Decl,
                               ast::DeclContext* DC, QualType Self = QualType(),
                               bool LookInExtensions = true,
                               SourceRange Loc = {});

public:
QueryResult EquivalentSignatures(EquivalentSignaturesQuery::ResultKind& Result,
                                 ast::CallableDecl* C1, ast::CallableDecl* C2,
                                 QualType Self, SourceRange Loc = {});

friend class CompileModuleQuery;
friend class ParseSourceFileQuery;
friend class TypeCheckASTQuery;
friend class CreateILModuleQuery;
friend class SetupIRGenQuery;
friend class CreateLLVMModuleQuery;
friend class CreateObjectQuery;
friend class CreateStaticLibQuery;
friend class CreateDynamicLibQuery;
friend class CreateExecutableQuery;
friend class EmitILQuery;
friend class EmitIRQuery;
friend class PrintUsedMemoryQuery;
friend class MeasureExecutionTimeQuery;
friend class IsEquatableQuery;
friend class IsCopyableQuery;
friend class IsMoveOnlyQuery;
friend class IsImplicitlyCopyableQuery;
friend class IsTriviallyCopyableQuery;
friend class NeedsRetainOrReleaseQuery;
friend class NeedsDeinitilizationQuery;
friend class NeedsStructReturnQuery;
friend class SubstAssociatedTypesQuery;
friend class SubstTemplateParamTypesQuery;
friend class GetBuiltinModuleQuery;
friend class GetBuiltinFuncQuery;
friend class GetBuiltinRecordQuery;
friend class GetBuiltinProtocolQuery;
friend class FindPrecedenceGroupQuery;
friend class FindOperatorQuery;
friend class ResolveImportQuery;
friend class ResolveUsingQuery;
friend class ResolveStaticIfQuery;
friend class ResolveStaticForQuery;
friend class ResolveStaticAssertQuery;
friend class ResolveStaticPrintQuery;
friend class ResolveStaticExprQuery;
friend class ResolveStaticExprToBoolQuery;
friend class ExpandMacroQuery;
friend class ExpandMacroDeclQuery;
friend class ExpandMacrosQuery;
friend class FindExtensionsQuery;
friend class GetExtensionTypeKindQuery;
friend class MatchExtensionTypeQuery;
friend class ExtensionAppliesQuery;
friend class GetExtendedDeclQuery;
friend class ResolveExtensionQuery;
friend class ResolveConformanceToProtocolQuery;
friend class ResolveDeclaredConformancesQuery;
friend class AddInheritedConformanceQuery;
friend class ResolveAssociatedTypesQuery;
friend class GetReferencedAssociatedTypesQuery;
friend class ReferencedAssociatedTypesReadyQuery;
friend class CheckProtocolExtensionApplicabilityQuery;
friend class VerifyConstraintQuery;
friend class IsConstraintSatisfiedQuery;
friend class CheckConformancesQuery;
friend class CheckSingleConformanceQuery;
friend class ConformsToQuery;
friend class GetTypeSizeQuery;
friend class GetTypeAlignmentQuery;
friend class GetTypeStrideQuery;
friend class CalculateRecordSizeQuery;
friend class IsAccessibleQuery;
friend class CheckAccessibilityQuery;
friend class TypeCheckDeclContextQuery;
friend class PrepareDeclInterfaceQuery;
friend class TypecheckDeclQuery;
friend class PrepareCallableInterfaceQuery;
friend class TypecheckCallableQuery;
friend class PrepareFunctionInterfaceQuery;
friend class TypecheckFunctionQuery;
friend class PrepareMethodInterfaceQuery;
friend class TypecheckMethodQuery;
friend class PrepareInitInterfaceQuery;
friend class TypecheckInitQuery;
friend class PrepareDeinitInterfaceQuery;
friend class TypecheckDeinitQuery;
friend class PrepareFuncArgInterfaceQuery;
friend class TypecheckFuncArgQuery;
friend class PrepareTemplateParamInterfaceQuery;
friend class TypecheckTemplateParamQuery;
friend class PrepareAssociatedTypeInterfaceQuery;
friend class TypecheckAssociatedTypeQuery;
friend class PrepareRecordInterfaceQuery;
friend class TypecheckRecordQuery;
friend class PrepareStructInterfaceQuery;
friend class TypecheckStructQuery;
friend class PrepareClassInterfaceQuery;
friend class TypecheckClassQuery;
friend class PrepareEnumInterfaceQuery;
friend class TypecheckEnumQuery;
friend class PrepareProtocolInterfaceQuery;
friend class TypecheckProtocolQuery;
friend class PrepareExtensionInterfaceQuery;
friend class TypecheckExtensionQuery;
friend class PrepareFieldInterfaceQuery;
friend class TypecheckFieldQuery;
friend class PreparePropInterfaceQuery;
friend class TypecheckPropQuery;
friend class PrepareSubscriptInterfaceQuery;
friend class TypecheckSubscriptQuery;
friend class AssignInitNameQuery;
friend class LookupFirstQuery;
friend class LookupSingleQuery;
friend class MultiLevelLookupQuery;
friend class DirectLookupQuery;
friend class NestedNameLookupQuery;
friend class PrepareNameLookupQuery;
friend class GetAssociatedTypeQuery;
friend class CheckTypeEquivalenceQuery;
friend class InstantiateProtocolDefaultImplQuery;
friend class InstantiateRecordQuery;
friend class CheckTemplateExtensionApplicabilityQuery;
friend class InstantiateTemplateMemberQuery;
friend class PrepareInstantiationForNameLookupQuery;
friend class GetILModuleForDeclQuery;
friend class GenerateILForContextQuery;
friend class GenerateRecordILQuery;
friend class GetILGlobalQuery;
friend class GenerateLazyILGlobalDefinitionQuery;
friend class GetILFunctionQuery;
friend class GenerateILFunctionBodyQuery;
friend class GetILTypeInfoQuery;
friend class CreateILBasicTypeInfoQuery;
friend class CreateILRecordTypeInfoQuery;
friend class OpenFileForReadQuery;
friend class OpenTmpFileQuery;
friend class GetDefaultTemplateArgQuery;
friend class CreateSelfArgumentQuery;
friend class FindEquivalentDeclQuery;
friend class EquivalentSignaturesQuery;
#endif

#ifdef CDOT_QUERY_CONTEXT_IMPL
QueryResult QueryContext::CompileModule(SourceRange Loc)

{
   if (!CompileModuleInstance)
      CompileModuleInstance = new (*this) CompileModuleQuery(*this);
   auto* _Q = CompileModuleInstance;

   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ParseSourceFile(ast::ModuleDecl*& Result,
                                          StringRef FileName, SourceRange Loc)

{
   auto It = ParseSourceFileQueries.find(FileName);
   ParseSourceFileQuery* _Q;
   if (It == ParseSourceFileQueries.end()) {
      _Q = new (*this) ParseSourceFileQuery(*this, FileName);
      ParseSourceFileQueries[_Q->FileName] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypeCheckAST(ast::ModuleDecl* Mod, SourceRange Loc)

{
   auto It = TypeCheckASTQueries.find(Mod);
   TypeCheckASTQuery* _Q;
   if (It == TypeCheckASTQueries.end()) {
      _Q = new (*this) TypeCheckASTQuery(*this, Mod);
      TypeCheckASTQueries[_Q->Mod] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::CreateILModule(il::Module*& Result,
                                         llvm::ArrayRef<StringRef> SourceFiles,
                                         SourceRange Loc)

{
   auto It = CreateILModuleQueries.find(SourceFiles);
   CreateILModuleQuery* _Q;
   if (It == CreateILModuleQueries.end()) {
      _Q = new (*this) CreateILModuleQuery(*this, SourceFiles);
      CreateILModuleQueries[_Q->SourceFiles] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::SetupIRGen(il::IRGen*& Result, SourceRange Loc)

{
   if (!SetupIRGenInstance)
      SetupIRGenInstance = new (*this) SetupIRGenQuery(*this);
   auto* _Q = SetupIRGenInstance;

   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::CreateLLVMModule(llvm::Module*& Result,
                               llvm::ArrayRef<StringRef> SourceFiles,
                               SourceRange Loc)

{
   auto It = CreateLLVMModuleQueries.find(SourceFiles);
   CreateLLVMModuleQuery* _Q;
   if (It == CreateLLVMModuleQueries.end()) {
      _Q = new (*this) CreateLLVMModuleQuery(*this, SourceFiles);
      CreateLLVMModuleQueries[_Q->SourceFiles] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::CreateObject(llvm::ArrayRef<StringRef> SourceFiles,
                                       llvm::raw_ostream& OS, SourceRange Loc)

{
   auto* _Q = new (*this) CreateObjectQuery(*this, SourceFiles, OS);
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::CreateStaticLib(llvm::ArrayRef<StringRef> SourceFiles,
                                          StringRef OutFile, SourceRange Loc)

{
   auto* _Q = new (*this) CreateStaticLibQuery(*this, SourceFiles, OutFile);
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::CreateDynamicLib(llvm::ArrayRef<StringRef> SourceFiles,
                               StringRef OutFile, SourceRange Loc)

{
   auto* _Q = new (*this) CreateDynamicLibQuery(*this, SourceFiles, OutFile);
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::CreateExecutable(llvm::ArrayRef<StringRef> SourceFiles,
                               StringRef OutFile, SourceRange Loc)

{
   auto* _Q = new (*this) CreateExecutableQuery(*this, SourceFiles, OutFile);
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::EmitIL(llvm::ArrayRef<StringRef> SourceFiles,
                                 llvm::raw_ostream& OS, SourceRange Loc)

{
   auto* _Q = new (*this) EmitILQuery(*this, SourceFiles, OS);
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::EmitIR(llvm::ArrayRef<StringRef> SourceFiles,
                                 llvm::raw_ostream& OS, SourceRange Loc)

{
   auto* _Q = new (*this) EmitIRQuery(*this, SourceFiles, OS);
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrintUsedMemory(SourceRange Loc)

{
   auto* _Q = new (*this) PrintUsedMemoryQuery(*this);
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::MeasureExecutionTime(long long& Result, Query* Q,
                                               SourceRange Loc)

{
   auto It = MeasureExecutionTimeQueries.find(Q);
   MeasureExecutionTimeQuery* _Q;
   if (It == MeasureExecutionTimeQueries.end()) {
      _Q = new (*this) MeasureExecutionTimeQuery(*this, Q);
      MeasureExecutionTimeQueries[_Q->Q] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::IsEquatable(bool& Result, QualType T, SourceRange Loc)

{
   auto It = IsEquatableQueries.find(T);
   IsEquatableQuery* _Q;
   if (It == IsEquatableQueries.end()) {
      _Q = new (*this) IsEquatableQuery(*this, T);
      IsEquatableQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::IsCopyable(bool& Result, QualType T, SourceRange Loc)

{
   auto It = IsCopyableQueries.find(T);
   IsCopyableQuery* _Q;
   if (It == IsCopyableQueries.end()) {
      _Q = new (*this) IsCopyableQuery(*this, T);
      IsCopyableQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::IsMoveOnly(bool& Result, QualType T, SourceRange Loc)

{
   auto It = IsMoveOnlyQueries.find(T);
   IsMoveOnlyQuery* _Q;
   if (It == IsMoveOnlyQueries.end()) {
      _Q = new (*this) IsMoveOnlyQuery(*this, T);
      IsMoveOnlyQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::IsImplicitlyCopyable(bool& Result, QualType T,
                                               SourceRange Loc)

{
   auto It = IsImplicitlyCopyableQueries.find(T);
   IsImplicitlyCopyableQuery* _Q;
   if (It == IsImplicitlyCopyableQueries.end()) {
      _Q = new (*this) IsImplicitlyCopyableQuery(*this, T);
      IsImplicitlyCopyableQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::IsTriviallyCopyable(bool& Result, QualType T,
                                              SourceRange Loc)

{
   auto It = IsTriviallyCopyableQueries.find(T);
   IsTriviallyCopyableQuery* _Q;
   if (It == IsTriviallyCopyableQueries.end()) {
      _Q = new (*this) IsTriviallyCopyableQuery(*this, T);
      IsTriviallyCopyableQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::NeedsRetainOrRelease(bool& Result, QualType T,
                                               SourceRange Loc)

{
   auto It = NeedsRetainOrReleaseQueries.find(T);
   NeedsRetainOrReleaseQuery* _Q;
   if (It == NeedsRetainOrReleaseQueries.end()) {
      _Q = new (*this) NeedsRetainOrReleaseQuery(*this, T);
      NeedsRetainOrReleaseQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::NeedsDeinitilization(bool& Result, QualType T,
                                               SourceRange Loc)

{
   auto It = NeedsDeinitilizationQueries.find(T);
   NeedsDeinitilizationQuery* _Q;
   if (It == NeedsDeinitilizationQueries.end()) {
      _Q = new (*this) NeedsDeinitilizationQuery(*this, T);
      NeedsDeinitilizationQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::NeedsStructReturn(bool& Result, QualType T,
                                            SourceRange Loc)

{
   auto It = NeedsStructReturnQueries.find(T);
   NeedsStructReturnQuery* _Q;
   if (It == NeedsStructReturnQueries.end()) {
      _Q = new (*this) NeedsStructReturnQuery(*this, T);
      NeedsStructReturnQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::SubstAssociatedTypes(QualType& Result, QualType T,
                                               QualType Self, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   SubstAssociatedTypesQuery::Profile(ID, T, Self);
   void* InsertPos;
   auto* _Q = SubstAssociatedTypesQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) SubstAssociatedTypesQuery(*this, T, Self);
      SubstAssociatedTypesQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::SubstTemplateParamTypes(
    QualType& Result, QualType T,
    sema::MultiLevelFinalTemplateArgList TemplateArgs, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   SubstTemplateParamTypesQuery::Profile(ID, T, TemplateArgs);
   void* InsertPos;
   auto* _Q = SubstTemplateParamTypesQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this)
          SubstTemplateParamTypesQuery(*this, T, std::move(TemplateArgs));
      SubstTemplateParamTypesQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GetBuiltinModule(
    Module*& Result, GetBuiltinModuleQuery::ModuleKind Mod, SourceRange Loc)

{
   auto It = GetBuiltinModuleQueries.find(Mod);
   GetBuiltinModuleQuery* _Q;
   if (It == GetBuiltinModuleQueries.end()) {
      _Q = new (*this) GetBuiltinModuleQuery(*this, Mod);
      GetBuiltinModuleQueries[_Q->Mod] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GetBuiltinFunc(ast::CallableDecl*& Result,
                                         GetBuiltinFuncQuery::FunctionKind Fn,
                                         SourceRange Loc)

{
   auto It = GetBuiltinFuncQueries.find(Fn);
   GetBuiltinFuncQuery* _Q;
   if (It == GetBuiltinFuncQueries.end()) {
      _Q = new (*this) GetBuiltinFuncQuery(*this, Fn);
      GetBuiltinFuncQueries[_Q->Fn] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GetBuiltinRecord(ast::RecordDecl*& Result,
                                           GetBuiltinRecordQuery::RecordKind R,
                                           SourceRange Loc)

{
   auto It = GetBuiltinRecordQueries.find(R);
   GetBuiltinRecordQuery* _Q;
   if (It == GetBuiltinRecordQueries.end()) {
      _Q = new (*this) GetBuiltinRecordQuery(*this, R);
      GetBuiltinRecordQueries[_Q->R] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::GetBuiltinProtocol(ast::ProtocolDecl*& Result,
                                 GetBuiltinProtocolQuery::ProtocolKind P,
                                 SourceRange Loc)

{
   auto It = GetBuiltinProtocolQueries.find(P);
   GetBuiltinProtocolQuery* _Q;
   if (It == GetBuiltinProtocolQueries.end()) {
      _Q = new (*this) GetBuiltinProtocolQuery(*this, P);
      GetBuiltinProtocolQueries[_Q->P] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::FindPrecedenceGroup(ast::PrecedenceGroupDecl*& Result,
                                              DeclarationName Name,
                                              bool DiagOnError, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   FindPrecedenceGroupQuery::Profile(ID, Name, DiagOnError);
   void* InsertPos;
   auto* _Q = FindPrecedenceGroupQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) FindPrecedenceGroupQuery(*this, Name, DiagOnError);
      FindPrecedenceGroupQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::FindOperator(ast::OperatorDecl*& Result,
                                       DeclarationName Name,
                                       ast::DeclContext* DC, bool DiagOnError,
                                       SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   FindOperatorQuery::Profile(ID, Name, DC, DiagOnError);
   void* InsertPos;
   auto* _Q = FindOperatorQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) FindOperatorQuery(*this, Name, DC, DiagOnError);
      FindOperatorQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ResolveImport(ast::ImportDecl* I, SourceRange Loc)

{
   auto It = ResolveImportQueries.find(I);
   ResolveImportQuery* _Q;
   if (It == ResolveImportQueries.end()) {
      _Q = new (*this) ResolveImportQuery(*this, I);
      ResolveImportQueries[_Q->I] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ResolveUsing(ast::UsingDecl* U, SourceRange Loc)

{
   auto It = ResolveUsingQueries.find(U);
   ResolveUsingQuery* _Q;
   if (It == ResolveUsingQueries.end()) {
      _Q = new (*this) ResolveUsingQuery(*this, U);
      ResolveUsingQueries[_Q->U] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ResolveStaticIf(ast::Decl*& Result,
                                          ast::StaticIfDecl* Decl,
                                          SourceRange Loc)

{
   auto It = ResolveStaticIfQueries.find(Decl);
   ResolveStaticIfQuery* _Q;
   if (It == ResolveStaticIfQueries.end()) {
      _Q = new (*this) ResolveStaticIfQuery(*this, Decl);
      ResolveStaticIfQueries[_Q->Decl] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ResolveStaticFor(ast::Decl*& Result,
                                           ast::StaticForDecl* Decl,
                                           SourceRange Loc)

{
   auto It = ResolveStaticForQueries.find(Decl);
   ResolveStaticForQuery* _Q;
   if (It == ResolveStaticForQueries.end()) {
      _Q = new (*this) ResolveStaticForQuery(*this, Decl);
      ResolveStaticForQueries[_Q->Decl] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ResolveStaticAssert(ast::StaticAssertDecl* Decl,
                                              SourceRange Loc)

{
   auto It = ResolveStaticAssertQueries.find(Decl);
   ResolveStaticAssertQuery* _Q;
   if (It == ResolveStaticAssertQueries.end()) {
      _Q = new (*this) ResolveStaticAssertQuery(*this, Decl);
      ResolveStaticAssertQueries[_Q->Decl] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ResolveStaticPrint(ast::StaticPrintDecl* Decl,
                                             SourceRange Loc)

{
   auto It = ResolveStaticPrintQueries.find(Decl);
   ResolveStaticPrintQuery* _Q;
   if (It == ResolveStaticPrintQueries.end()) {
      _Q = new (*this) ResolveStaticPrintQuery(*this, Decl);
      ResolveStaticPrintQueries[_Q->Decl] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ResolveStaticExpr(il::Constant*& Result,
                                            ast::StaticExpr* Expr,
                                            SourceRange Loc)

{
   auto It = ResolveStaticExprQueries.find(Expr);
   ResolveStaticExprQuery* _Q;
   if (It == ResolveStaticExprQueries.end()) {
      _Q = new (*this) ResolveStaticExprQuery(*this, Expr);
      ResolveStaticExprQueries[_Q->Expr] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ResolveStaticExprToBool(bool& Result,
                                                  ast::StaticExpr* Expr,
                                                  bool DiagOnError,
                                                  SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   ResolveStaticExprToBoolQuery::Profile(ID, Expr, DiagOnError);
   void* InsertPos;
   auto* _Q = ResolveStaticExprToBoolQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) ResolveStaticExprToBoolQuery(*this, Expr, DiagOnError);
      ResolveStaticExprToBoolQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ExpandMacro(StmtOrDecl& Result, StmtOrDecl SOD,
                                      DeclarationName Name,
                                      ast::DeclContext* DC, unsigned Delim,
                                      llvm::ArrayRef<lex::Token> Tokens,
                                      unsigned ExpectedKind, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   ExpandMacroQuery::Profile(ID, SOD, Name, DC, Delim, Tokens, ExpectedKind);
   void* InsertPos;
   auto* _Q = ExpandMacroQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this)
          ExpandMacroQuery(*this, SOD, Name, DC, Delim, Tokens, ExpectedKind);
      ExpandMacroQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ExpandMacroDecl(ast::Decl*& Result,
                                          ast::MacroExpansionDecl* Decl,
                                          SourceRange Loc)

{
   auto It = ExpandMacroDeclQueries.find(Decl);
   ExpandMacroDeclQuery* _Q;
   if (It == ExpandMacroDeclQueries.end()) {
      _Q = new (*this) ExpandMacroDeclQuery(*this, Decl);
      ExpandMacroDeclQueries[_Q->Decl] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ExpandMacros(StringRef SourceFile, SourceRange Loc)

{
   auto It = ExpandMacrosQueries.find(SourceFile);
   ExpandMacrosQuery* _Q;
   if (It == ExpandMacrosQueries.end()) {
      _Q = new (*this) ExpandMacrosQuery(*this, SourceFile);
      ExpandMacrosQueries[_Q->SourceFile] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::FindExtensions(QualType T, SourceRange Loc)

{
   auto It = FindExtensionsQueries.find(T);
   FindExtensionsQuery* _Q;
   if (It == FindExtensionsQueries.end()) {
      _Q = new (*this) FindExtensionsQuery(*this, T);
      FindExtensionsQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GetExtensionTypeKind(
    GetExtensionTypeKindQuery::ResultKind& Result, ast::SourceType T,
    SourceRange Loc)

{
   auto It = GetExtensionTypeKindQueries.find(T);
   GetExtensionTypeKindQuery* _Q;
   if (It == GetExtensionTypeKindQueries.end()) {
      _Q = new (*this) GetExtensionTypeKindQuery(*this, T);
      GetExtensionTypeKindQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::MatchExtensionType(MatchExtensionTypeQuery::ResultKind& Result,
                                 QualType T, ast::SourceType PatternTy,
                                 SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   MatchExtensionTypeQuery::Profile(ID, T, PatternTy);
   void* InsertPos;
   auto* _Q = MatchExtensionTypeQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) MatchExtensionTypeQuery(*this, T, PatternTy);
      MatchExtensionTypeQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::ExtensionApplies(MatchExtensionTypeQuery::ResultKind& Result,
                               ast::ExtensionDecl* Ext, QualType T,
                               SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   ExtensionAppliesQuery::Profile(ID, Ext, T);
   void* InsertPos;
   auto* _Q = ExtensionAppliesQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) ExtensionAppliesQuery(*this, Ext, T);
      ExtensionAppliesQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GetExtendedDecl(ast::NamedDecl*& Result,
                                          ast::SourceType ExtendedTy,
                                          SourceRange Loc)

{
   auto It = GetExtendedDeclQueries.find(ExtendedTy);
   GetExtendedDeclQuery* _Q;
   if (It == GetExtendedDeclQueries.end()) {
      _Q = new (*this) GetExtendedDeclQuery(*this, ExtendedTy);
      GetExtendedDeclQueries[_Q->ExtendedTy] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ResolveExtension(ast::ExtensionDecl* Ext,
                                           SourceRange Loc)

{
   auto It = ResolveExtensionQueries.find(Ext);
   ResolveExtensionQuery* _Q;
   if (It == ResolveExtensionQueries.end()) {
      _Q = new (*this) ResolveExtensionQuery(*this, Ext);
      ResolveExtensionQueries[_Q->Ext] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::ResolveConformanceToProtocol(ast::ProtocolDecl*& Result,
                                           QualType T, ast::SourceType Conf,
                                           SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   ResolveConformanceToProtocolQuery::Profile(ID, T, Conf);
   void* InsertPos;
   auto* _Q
       = ResolveConformanceToProtocolQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) ResolveConformanceToProtocolQuery(*this, T, Conf);
      ResolveConformanceToProtocolQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ResolveDeclaredConformances(
    QualType T, llvm::ArrayRef<ast::SourceType> Conformances, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   ResolveDeclaredConformancesQuery::Profile(ID, T, Conformances);
   void* InsertPos;
   auto* _Q
       = ResolveDeclaredConformancesQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) ResolveDeclaredConformancesQuery(*this, T, Conformances);
      ResolveDeclaredConformancesQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::AddInheritedConformance(QualType T,
                                                  ast::ProtocolDecl* Proto,
                                                  SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   AddInheritedConformanceQuery::Profile(ID, T, Proto);
   void* InsertPos;
   auto* _Q = AddInheritedConformanceQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) AddInheritedConformanceQuery(*this, T, Proto);
      AddInheritedConformanceQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ResolveAssociatedTypes(QualType T, SourceRange Loc)

{
   auto It = ResolveAssociatedTypesQueries.find(T);
   ResolveAssociatedTypesQuery* _Q;
   if (It == ResolveAssociatedTypesQueries.end()) {
      _Q = new (*this) ResolveAssociatedTypesQuery(*this, T);
      ResolveAssociatedTypesQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GetReferencedAssociatedTypes(
    ArrayRef<ast::AssociatedTypeDecl*>& Result, ast::NamedDecl* ConstrainedDecl,
    ast::DeclConstraint* C, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   GetReferencedAssociatedTypesQuery::Profile(ID, ConstrainedDecl, C);
   void* InsertPos;
   auto* _Q
       = GetReferencedAssociatedTypesQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this)
          GetReferencedAssociatedTypesQuery(*this, ConstrainedDecl, C);
      GetReferencedAssociatedTypesQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ReferencedAssociatedTypesReady(
    bool& Result, QualType T, ast::NamedDecl* ConstrainedDecl, SourceRange Loc)

{
   auto* _Q = new (*this)
       ReferencedAssociatedTypesReadyQuery(*this, T, ConstrainedDecl);
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::CheckProtocolExtensionApplicability(
    CheckProtocolExtensionApplicabilityQuery::ResultKind& Result, QualType T,
    ast::ExtensionDecl* Ext, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   CheckProtocolExtensionApplicabilityQuery::Profile(ID, T, Ext);
   void* InsertPos;
   auto* _Q = CheckProtocolExtensionApplicabilityQueries.FindNodeOrInsertPos(
       ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) CheckProtocolExtensionApplicabilityQuery(*this, T, Ext);
      CheckProtocolExtensionApplicabilityQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::VerifyConstraint(ast::DeclConstraint* C,
                                           ast::NamedDecl* ConstrainedDecl,
                                           SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   VerifyConstraintQuery::Profile(ID, C, ConstrainedDecl);
   void* InsertPos;
   auto* _Q = VerifyConstraintQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) VerifyConstraintQuery(*this, C, ConstrainedDecl);
      VerifyConstraintQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::IsConstraintSatisfied(bool& Result,
                                                ast::DeclConstraint* C,
                                                QualType Self,
                                                ast::NamedDecl* OriginalDecl,
                                                SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   IsConstraintSatisfiedQuery::Profile(ID, C, Self, OriginalDecl);
   void* InsertPos;
   auto* _Q = IsConstraintSatisfiedQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) IsConstraintSatisfiedQuery(*this, C, Self, OriginalDecl);
      IsConstraintSatisfiedQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::CheckConformances(QualType T, SourceRange Loc)

{
   auto It = CheckConformancesQueries.find(T);
   CheckConformancesQuery* _Q;
   if (It == CheckConformancesQueries.end()) {
      _Q = new (*this) CheckConformancesQuery(*this, T);
      CheckConformancesQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::CheckSingleConformance(QualType T,
                                                 ast::ProtocolDecl* P,
                                                 SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   CheckSingleConformanceQuery::Profile(ID, T, P);
   void* InsertPos;
   auto* _Q = CheckSingleConformanceQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) CheckSingleConformanceQuery(*this, T, P);
      CheckSingleConformanceQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::ConformsTo(bool& Result, QualType T,
                                     ast::ProtocolDecl* P, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   ConformsToQuery::Profile(ID, T, P);
   void* InsertPos;
   auto* _Q = ConformsToQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) ConformsToQuery(*this, T, P);
      ConformsToQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GetTypeSize(unsigned& Result, QualType T,
                                      SourceRange Loc)

{
   auto It = GetTypeSizeQueries.find(T);
   GetTypeSizeQuery* _Q;
   if (It == GetTypeSizeQueries.end()) {
      _Q = new (*this) GetTypeSizeQuery(*this, T);
      GetTypeSizeQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GetTypeAlignment(unsigned short& Result, QualType T,
                                           SourceRange Loc)

{
   auto It = GetTypeAlignmentQueries.find(T);
   GetTypeAlignmentQuery* _Q;
   if (It == GetTypeAlignmentQueries.end()) {
      _Q = new (*this) GetTypeAlignmentQuery(*this, T);
      GetTypeAlignmentQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GetTypeStride(unsigned& Result, QualType T,
                                        SourceRange Loc)

{
   auto It = GetTypeStrideQueries.find(T);
   GetTypeStrideQuery* _Q;
   if (It == GetTypeStrideQueries.end()) {
      _Q = new (*this) GetTypeStrideQuery(*this, T);
      GetTypeStrideQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::CalculateRecordSize(unsigned& Result,
                                              ast::RecordDecl* R,
                                              SourceRange Loc)

{
   auto It = CalculateRecordSizeQueries.find(R);
   CalculateRecordSizeQuery* _Q;
   if (It == CalculateRecordSizeQueries.end()) {
      _Q = new (*this) CalculateRecordSizeQuery(*this, R);
      CalculateRecordSizeQueries[_Q->R] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::IsAccessible(bool& Result, ast::DeclContext* DC,
                                       ast::NamedDecl* ND, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   IsAccessibleQuery::Profile(ID, DC, ND);
   void* InsertPos;
   auto* _Q = IsAccessibleQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) IsAccessibleQuery(*this, DC, ND);
      IsAccessibleQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::CheckAccessibility(ast::DeclContext* DC,
                                             ast::NamedDecl* ND,
                                             SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   CheckAccessibilityQuery::Profile(ID, DC, ND);
   void* InsertPos;
   auto* _Q = CheckAccessibilityQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) CheckAccessibilityQuery(*this, DC, ND);
      CheckAccessibilityQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypeCheckDeclContext(ast::DeclContext* DC,
                                               SourceRange Loc)

{
   auto It = TypeCheckDeclContextQueries.find(DC);
   TypeCheckDeclContextQuery* _Q;
   if (It == TypeCheckDeclContextQueries.end()) {
      _Q = new (*this) TypeCheckDeclContextQuery(*this, DC);
      TypeCheckDeclContextQueries[_Q->DC] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareDeclInterface(ast::Decl* D, SourceRange Loc)

{
   auto It = PrepareDeclInterfaceQueries.find(D);
   PrepareDeclInterfaceQuery* _Q;
   if (It == PrepareDeclInterfaceQueries.end()) {
      _Q = new (*this) PrepareDeclInterfaceQuery(*this, D);
      PrepareDeclInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckDecl(ast::Decl* D, SourceRange Loc)

{
   auto It = TypecheckDeclQueries.find(D);
   TypecheckDeclQuery* _Q;
   if (It == TypecheckDeclQueries.end()) {
      _Q = new (*this) TypecheckDeclQuery(*this, D);
      TypecheckDeclQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareCallableInterface(ast::CallableDecl* D,
                                                   SourceRange Loc)

{
   auto It = PrepareCallableInterfaceQueries.find(D);
   PrepareCallableInterfaceQuery* _Q;
   if (It == PrepareCallableInterfaceQueries.end()) {
      _Q = new (*this) PrepareCallableInterfaceQuery(*this, D);
      PrepareCallableInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckCallable(ast::CallableDecl* D,
                                            SourceRange Loc)

{
   auto It = TypecheckCallableQueries.find(D);
   TypecheckCallableQuery* _Q;
   if (It == TypecheckCallableQueries.end()) {
      _Q = new (*this) TypecheckCallableQuery(*this, D);
      TypecheckCallableQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareFunctionInterface(ast::FunctionDecl* D,
                                                   SourceRange Loc)

{
   auto It = PrepareFunctionInterfaceQueries.find(D);
   PrepareFunctionInterfaceQuery* _Q;
   if (It == PrepareFunctionInterfaceQueries.end()) {
      _Q = new (*this) PrepareFunctionInterfaceQuery(*this, D);
      PrepareFunctionInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckFunction(ast::FunctionDecl* D,
                                            SourceRange Loc)

{
   auto It = TypecheckFunctionQueries.find(D);
   TypecheckFunctionQuery* _Q;
   if (It == TypecheckFunctionQueries.end()) {
      _Q = new (*this) TypecheckFunctionQuery(*this, D);
      TypecheckFunctionQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareMethodInterface(ast::MethodDecl* D,
                                                 SourceRange Loc)

{
   auto It = PrepareMethodInterfaceQueries.find(D);
   PrepareMethodInterfaceQuery* _Q;
   if (It == PrepareMethodInterfaceQueries.end()) {
      _Q = new (*this) PrepareMethodInterfaceQuery(*this, D);
      PrepareMethodInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckMethod(ast::MethodDecl* D, SourceRange Loc)

{
   auto It = TypecheckMethodQueries.find(D);
   TypecheckMethodQuery* _Q;
   if (It == TypecheckMethodQueries.end()) {
      _Q = new (*this) TypecheckMethodQuery(*this, D);
      TypecheckMethodQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareInitInterface(ast::InitDecl* D,
                                               SourceRange Loc)

{
   auto It = PrepareInitInterfaceQueries.find(D);
   PrepareInitInterfaceQuery* _Q;
   if (It == PrepareInitInterfaceQueries.end()) {
      _Q = new (*this) PrepareInitInterfaceQuery(*this, D);
      PrepareInitInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckInit(ast::InitDecl* D, SourceRange Loc)

{
   auto It = TypecheckInitQueries.find(D);
   TypecheckInitQuery* _Q;
   if (It == TypecheckInitQueries.end()) {
      _Q = new (*this) TypecheckInitQuery(*this, D);
      TypecheckInitQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareDeinitInterface(ast::DeinitDecl* D,
                                                 SourceRange Loc)

{
   auto It = PrepareDeinitInterfaceQueries.find(D);
   PrepareDeinitInterfaceQuery* _Q;
   if (It == PrepareDeinitInterfaceQueries.end()) {
      _Q = new (*this) PrepareDeinitInterfaceQuery(*this, D);
      PrepareDeinitInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckDeinit(ast::DeinitDecl* D, SourceRange Loc)

{
   auto It = TypecheckDeinitQueries.find(D);
   TypecheckDeinitQuery* _Q;
   if (It == TypecheckDeinitQueries.end()) {
      _Q = new (*this) TypecheckDeinitQuery(*this, D);
      TypecheckDeinitQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareFuncArgInterface(ast::FuncArgDecl* D,
                                                  SourceRange Loc)

{
   auto It = PrepareFuncArgInterfaceQueries.find(D);
   PrepareFuncArgInterfaceQuery* _Q;
   if (It == PrepareFuncArgInterfaceQueries.end()) {
      _Q = new (*this) PrepareFuncArgInterfaceQuery(*this, D);
      PrepareFuncArgInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckFuncArg(ast::FuncArgDecl* D, SourceRange Loc)

{
   auto It = TypecheckFuncArgQueries.find(D);
   TypecheckFuncArgQuery* _Q;
   if (It == TypecheckFuncArgQueries.end()) {
      _Q = new (*this) TypecheckFuncArgQuery(*this, D);
      TypecheckFuncArgQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::PrepareTemplateParamInterface(ast::TemplateParamDecl* D,
                                            SourceRange Loc)

{
   auto It = PrepareTemplateParamInterfaceQueries.find(D);
   PrepareTemplateParamInterfaceQuery* _Q;
   if (It == PrepareTemplateParamInterfaceQueries.end()) {
      _Q = new (*this) PrepareTemplateParamInterfaceQuery(*this, D);
      PrepareTemplateParamInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckTemplateParam(ast::TemplateParamDecl* D,
                                                 SourceRange Loc)

{
   auto It = TypecheckTemplateParamQueries.find(D);
   TypecheckTemplateParamQuery* _Q;
   if (It == TypecheckTemplateParamQueries.end()) {
      _Q = new (*this) TypecheckTemplateParamQuery(*this, D);
      TypecheckTemplateParamQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::PrepareAssociatedTypeInterface(ast::AssociatedTypeDecl* D,
                                             SourceRange Loc)

{
   auto It = PrepareAssociatedTypeInterfaceQueries.find(D);
   PrepareAssociatedTypeInterfaceQuery* _Q;
   if (It == PrepareAssociatedTypeInterfaceQueries.end()) {
      _Q = new (*this) PrepareAssociatedTypeInterfaceQuery(*this, D);
      PrepareAssociatedTypeInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckAssociatedType(ast::AssociatedTypeDecl* D,
                                                  SourceRange Loc)

{
   auto It = TypecheckAssociatedTypeQueries.find(D);
   TypecheckAssociatedTypeQuery* _Q;
   if (It == TypecheckAssociatedTypeQueries.end()) {
      _Q = new (*this) TypecheckAssociatedTypeQuery(*this, D);
      TypecheckAssociatedTypeQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareRecordInterface(ast::RecordDecl* D,
                                                 SourceRange Loc)

{
   auto It = PrepareRecordInterfaceQueries.find(D);
   PrepareRecordInterfaceQuery* _Q;
   if (It == PrepareRecordInterfaceQueries.end()) {
      _Q = new (*this) PrepareRecordInterfaceQuery(*this, D);
      PrepareRecordInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckRecord(ast::RecordDecl* D, SourceRange Loc)

{
   auto It = TypecheckRecordQueries.find(D);
   TypecheckRecordQuery* _Q;
   if (It == TypecheckRecordQueries.end()) {
      _Q = new (*this) TypecheckRecordQuery(*this, D);
      TypecheckRecordQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareStructInterface(ast::StructDecl* D,
                                                 SourceRange Loc)

{
   auto It = PrepareStructInterfaceQueries.find(D);
   PrepareStructInterfaceQuery* _Q;
   if (It == PrepareStructInterfaceQueries.end()) {
      _Q = new (*this) PrepareStructInterfaceQuery(*this, D);
      PrepareStructInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckStruct(ast::StructDecl* D, SourceRange Loc)

{
   auto It = TypecheckStructQueries.find(D);
   TypecheckStructQuery* _Q;
   if (It == TypecheckStructQueries.end()) {
      _Q = new (*this) TypecheckStructQuery(*this, D);
      TypecheckStructQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareClassInterface(ast::ClassDecl* D,
                                                SourceRange Loc)

{
   auto It = PrepareClassInterfaceQueries.find(D);
   PrepareClassInterfaceQuery* _Q;
   if (It == PrepareClassInterfaceQueries.end()) {
      _Q = new (*this) PrepareClassInterfaceQuery(*this, D);
      PrepareClassInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckClass(ast::ClassDecl* D, SourceRange Loc)

{
   auto It = TypecheckClassQueries.find(D);
   TypecheckClassQuery* _Q;
   if (It == TypecheckClassQueries.end()) {
      _Q = new (*this) TypecheckClassQuery(*this, D);
      TypecheckClassQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareEnumInterface(ast::EnumDecl* D,
                                               SourceRange Loc)

{
   auto It = PrepareEnumInterfaceQueries.find(D);
   PrepareEnumInterfaceQuery* _Q;
   if (It == PrepareEnumInterfaceQueries.end()) {
      _Q = new (*this) PrepareEnumInterfaceQuery(*this, D);
      PrepareEnumInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckEnum(ast::EnumDecl* D, SourceRange Loc)

{
   auto It = TypecheckEnumQueries.find(D);
   TypecheckEnumQuery* _Q;
   if (It == TypecheckEnumQueries.end()) {
      _Q = new (*this) TypecheckEnumQuery(*this, D);
      TypecheckEnumQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareProtocolInterface(ast::ProtocolDecl* D,
                                                   SourceRange Loc)

{
   auto It = PrepareProtocolInterfaceQueries.find(D);
   PrepareProtocolInterfaceQuery* _Q;
   if (It == PrepareProtocolInterfaceQueries.end()) {
      _Q = new (*this) PrepareProtocolInterfaceQuery(*this, D);
      PrepareProtocolInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckProtocol(ast::ProtocolDecl* D,
                                            SourceRange Loc)

{
   auto It = TypecheckProtocolQueries.find(D);
   TypecheckProtocolQuery* _Q;
   if (It == TypecheckProtocolQueries.end()) {
      _Q = new (*this) TypecheckProtocolQuery(*this, D);
      TypecheckProtocolQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareExtensionInterface(ast::ExtensionDecl* D,
                                                    SourceRange Loc)

{
   auto It = PrepareExtensionInterfaceQueries.find(D);
   PrepareExtensionInterfaceQuery* _Q;
   if (It == PrepareExtensionInterfaceQueries.end()) {
      _Q = new (*this) PrepareExtensionInterfaceQuery(*this, D);
      PrepareExtensionInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckExtension(ast::ExtensionDecl* D,
                                             SourceRange Loc)

{
   auto It = TypecheckExtensionQueries.find(D);
   TypecheckExtensionQuery* _Q;
   if (It == TypecheckExtensionQueries.end()) {
      _Q = new (*this) TypecheckExtensionQuery(*this, D);
      TypecheckExtensionQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareFieldInterface(ast::FieldDecl* D,
                                                SourceRange Loc)

{
   auto It = PrepareFieldInterfaceQueries.find(D);
   PrepareFieldInterfaceQuery* _Q;
   if (It == PrepareFieldInterfaceQueries.end()) {
      _Q = new (*this) PrepareFieldInterfaceQuery(*this, D);
      PrepareFieldInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckField(ast::FieldDecl* D, SourceRange Loc)

{
   auto It = TypecheckFieldQueries.find(D);
   TypecheckFieldQuery* _Q;
   if (It == TypecheckFieldQueries.end()) {
      _Q = new (*this) TypecheckFieldQuery(*this, D);
      TypecheckFieldQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PreparePropInterface(ast::PropDecl* D,
                                               SourceRange Loc)

{
   auto It = PreparePropInterfaceQueries.find(D);
   PreparePropInterfaceQuery* _Q;
   if (It == PreparePropInterfaceQueries.end()) {
      _Q = new (*this) PreparePropInterfaceQuery(*this, D);
      PreparePropInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckProp(ast::PropDecl* D, SourceRange Loc)

{
   auto It = TypecheckPropQueries.find(D);
   TypecheckPropQuery* _Q;
   if (It == TypecheckPropQueries.end()) {
      _Q = new (*this) TypecheckPropQuery(*this, D);
      TypecheckPropQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareSubscriptInterface(ast::SubscriptDecl* D,
                                                    SourceRange Loc)

{
   auto It = PrepareSubscriptInterfaceQueries.find(D);
   PrepareSubscriptInterfaceQuery* _Q;
   if (It == PrepareSubscriptInterfaceQueries.end()) {
      _Q = new (*this) PrepareSubscriptInterfaceQuery(*this, D);
      PrepareSubscriptInterfaceQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::TypecheckSubscript(ast::SubscriptDecl* D,
                                             SourceRange Loc)

{
   auto It = TypecheckSubscriptQueries.find(D);
   TypecheckSubscriptQuery* _Q;
   if (It == TypecheckSubscriptQueries.end()) {
      _Q = new (*this) TypecheckSubscriptQuery(*this, D);
      TypecheckSubscriptQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::AssignInitName(ast::InitDecl* Init, SourceRange Loc)

{
   auto It = AssignInitNameQueries.find(Init);
   AssignInitNameQuery* _Q;
   if (It == AssignInitNameQueries.end()) {
      _Q = new (*this) AssignInitNameQuery(*this, Init);
      AssignInitNameQueries[_Q->Init] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::LookupFirst(const SingleLevelLookupResult*& Result,
                                      ast::DeclContext* DC,
                                      DeclarationName Name, bool LocalLookup,
                                      SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   LookupFirstQuery::Profile(ID, DC, Name, LocalLookup);
   void* InsertPos;
   auto* _Q = LookupFirstQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) LookupFirstQuery(*this, DC, Name, LocalLookup);
      LookupFirstQueries.InsertNode(_Q, InsertPos);
   }
   _Q->refresh();

   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::LookupSingle(ast::NamedDecl*& Result,
                                       ast::DeclContext* DC,
                                       DeclarationName Name, bool LocalLookup,
                                       bool DiagOnError, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   LookupSingleQuery::Profile(ID, DC, Name, LocalLookup);
   void* InsertPos;
   auto* _Q = LookupSingleQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this)
          LookupSingleQuery(*this, DC, Name, LocalLookup, DiagOnError);
      LookupSingleQueries.InsertNode(_Q, InsertPos);
   }
   _Q->refresh();

   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::MultiLevelLookup(const MultiLevelLookupResult*& Result,
                               ast::DeclContext* DC, DeclarationName Name,
                               bool LocalLookup, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   MultiLevelLookupQuery::Profile(ID, DC, Name, LocalLookup);
   void* InsertPos;
   auto* _Q = MultiLevelLookupQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) MultiLevelLookupQuery(*this, DC, Name, LocalLookup);
      MultiLevelLookupQueries.InsertNode(_Q, InsertPos);
   }
   _Q->refresh();

   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::DirectLookup(const MultiLevelLookupResult*& Result,
                                       ast::DeclContext* DC,
                                       DeclarationName Name,
                                       bool LookInExtensions,
                                       bool PrepareNameLookup, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   DirectLookupQuery::Profile(ID, DC, Name, LookInExtensions,
                              PrepareNameLookup);
   void* InsertPos;
   auto* _Q = DirectLookupQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) DirectLookupQuery(*this, DC, Name, LookInExtensions,
                                         PrepareNameLookup);
      DirectLookupQueries.InsertNode(_Q, InsertPos);
   }
   _Q->refresh();

   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::NestedNameLookup(
    const MultiLevelLookupResult*& Result, ast::DeclContext* DC,
    llvm::ArrayRef<DeclarationName> Names, SourceRange Loc)

{
   auto* _Q = new (*this) NestedNameLookupQuery(*this, DC, Names);
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::PrepareNameLookup(ast::DeclContext* DC,
                                            SourceRange Loc)

{
   auto It = PrepareNameLookupQueries.find(DC);
   PrepareNameLookupQuery* _Q;
   if (It == PrepareNameLookupQueries.end()) {
      _Q = new (*this) PrepareNameLookupQuery(*this, DC);
      PrepareNameLookupQueries[_Q->DC] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GetAssociatedType(
    ast::AssociatedTypeDecl*& Result, ast::RecordDecl* R, DeclarationName Name,
    llvm::ArrayRef<ast::ExtensionDecl*> Extensions, SourceRange Loc)

{
   auto* _Q = new (*this) GetAssociatedTypeQuery(*this, R, Name, Extensions);
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::CheckTypeEquivalence(bool& Result, QualType LHS,
                                               QualType RHS, QualType Self,
                                               ast::DeclContext* LHSDecl,
                                               SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   CheckTypeEquivalenceQuery::Profile(ID, LHS, RHS, Self, LHSDecl);
   void* InsertPos;
   auto* _Q = CheckTypeEquivalenceQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this)
          CheckTypeEquivalenceQuery(*this, LHS, RHS, Self, LHSDecl);
      CheckTypeEquivalenceQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::InstantiateProtocolDefaultImpl(ast::NamedDecl*& Result,
                                             ast::NamedDecl* Impl,
                                             QualType Self, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   InstantiateProtocolDefaultImplQuery::Profile(ID, Impl, Self);
   void* InsertPos;
   auto* _Q = InstantiateProtocolDefaultImplQueries.FindNodeOrInsertPos(
       ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) InstantiateProtocolDefaultImplQuery(*this, Impl, Self);
      InstantiateProtocolDefaultImplQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::InstantiateRecord(
    ast::RecordDecl*& Result, ast::RecordDecl* Template,
    sema::FinalTemplateArgumentList* TemplateArgs,
    SourceLocation PointOfInstantiation, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   InstantiateRecordQuery::Profile(ID, Template, TemplateArgs);
   void* InsertPos;
   auto* _Q = InstantiateRecordQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) InstantiateRecordQuery(*this, Template, TemplateArgs,
                                              PointOfInstantiation);
      InstantiateRecordQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::CheckTemplateExtensionApplicability(
    bool& Result, ast::RecordDecl* Inst, ast::ExtensionDecl* Ext,
    SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   CheckTemplateExtensionApplicabilityQuery::Profile(ID, Inst, Ext);
   void* InsertPos;
   auto* _Q = CheckTemplateExtensionApplicabilityQueries.FindNodeOrInsertPos(
       ID, InsertPos);
   if (!_Q) {
      _Q = new (*this)
          CheckTemplateExtensionApplicabilityQuery(*this, Inst, Ext);
      CheckTemplateExtensionApplicabilityQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::InstantiateTemplateMember(ast::NamedDecl*& Result,
                                        ast::NamedDecl* TemplateMember,
                                        ast::RecordDecl* Inst, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   InstantiateTemplateMemberQuery::Profile(ID, TemplateMember, Inst);
   void* InsertPos;
   auto* _Q
       = InstantiateTemplateMemberQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this)
          InstantiateTemplateMemberQuery(*this, TemplateMember, Inst);
      InstantiateTemplateMemberQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::PrepareInstantiationForNameLookup(ast::RecordDecl* Inst,
                                                SourceRange Loc)

{
   auto It = PrepareInstantiationForNameLookupQueries.find(Inst);
   PrepareInstantiationForNameLookupQuery* _Q;
   if (It == PrepareInstantiationForNameLookupQueries.end()) {
      _Q = new (*this) PrepareInstantiationForNameLookupQuery(*this, Inst);
      PrepareInstantiationForNameLookupQueries[_Q->Inst] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GetILModuleForDecl(il::Module*& Result, ast::Decl* D,
                                             SourceRange Loc)

{
   auto It = GetILModuleForDeclQueries.find(D);
   GetILModuleForDeclQuery* _Q;
   if (It == GetILModuleForDeclQueries.end()) {
      _Q = new (*this) GetILModuleForDeclQuery(*this, D);
      GetILModuleForDeclQueries[_Q->D] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GenerateILForContext(ast::DeclContext* DC,
                                               SourceRange Loc)

{
   auto It = GenerateILForContextQueries.find(DC);
   GenerateILForContextQuery* _Q;
   if (It == GenerateILForContextQueries.end()) {
      _Q = new (*this) GenerateILForContextQuery(*this, DC);
      GenerateILForContextQueries[_Q->DC] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GenerateRecordIL(ast::RecordDecl* R, SourceRange Loc)

{
   auto It = GenerateRecordILQueries.find(R);
   GenerateRecordILQuery* _Q;
   if (It == GenerateRecordILQueries.end()) {
      _Q = new (*this) GenerateRecordILQuery(*this, R);
      GenerateRecordILQueries[_Q->R] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GetILGlobal(il::GlobalVariable*& Result,
                                      ast::VarDecl* GV, SourceRange Loc)

{
   auto It = GetILGlobalQueries.find(GV);
   GetILGlobalQuery* _Q;
   if (It == GetILGlobalQueries.end()) {
      _Q = new (*this) GetILGlobalQuery(*this, GV);
      GetILGlobalQueries[_Q->GV] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GenerateLazyILGlobalDefinition(ast::VarDecl* GV,
                                                         SourceRange Loc)

{
   auto It = GenerateLazyILGlobalDefinitionQueries.find(GV);
   GenerateLazyILGlobalDefinitionQuery* _Q;
   if (It == GenerateLazyILGlobalDefinitionQueries.end()) {
      _Q = new (*this) GenerateLazyILGlobalDefinitionQuery(*this, GV);
      GenerateLazyILGlobalDefinitionQueries[_Q->GV] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GetILFunction(il::Function*& Result,
                                        ast::CallableDecl* C, SourceRange Loc)

{
   auto It = GetILFunctionQueries.find(C);
   GetILFunctionQuery* _Q;
   if (It == GetILFunctionQueries.end()) {
      _Q = new (*this) GetILFunctionQuery(*this, C);
      GetILFunctionQueries[_Q->C] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GenerateILFunctionBody(ast::CallableDecl* C,
                                                 SourceRange Loc)

{
   auto It = GenerateILFunctionBodyQueries.find(C);
   GenerateILFunctionBodyQuery* _Q;
   if (It == GenerateILFunctionBodyQueries.end()) {
      _Q = new (*this) GenerateILFunctionBodyQuery(*this, C);
      GenerateILFunctionBodyQueries[_Q->C] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::GetILTypeInfo(il::GlobalVariable*& Result, QualType T,
                                        SourceRange Loc)

{
   auto It = GetILTypeInfoQueries.find(T);
   GetILTypeInfoQuery* _Q;
   if (It == GetILTypeInfoQueries.end()) {
      _Q = new (*this) GetILTypeInfoQuery(*this, T);
      GetILTypeInfoQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::CreateILBasicTypeInfo(il::Constant*& Result,
                                                QualType T, SourceRange Loc)

{
   auto It = CreateILBasicTypeInfoQueries.find(T);
   CreateILBasicTypeInfoQuery* _Q;
   if (It == CreateILBasicTypeInfoQueries.end()) {
      _Q = new (*this) CreateILBasicTypeInfoQuery(*this, T);
      CreateILBasicTypeInfoQueries[_Q->T] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::CreateILRecordTypeInfo(il::Constant*& Result,
                                                 ast::RecordDecl* R,
                                                 SourceRange Loc)

{
   auto It = CreateILRecordTypeInfoQueries.find(R);
   CreateILRecordTypeInfoQuery* _Q;
   if (It == CreateILRecordTypeInfoQueries.end()) {
      _Q = new (*this) CreateILRecordTypeInfoQuery(*this, R);
      CreateILRecordTypeInfoQueries[_Q->R] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::OpenFileForRead(llvm::MemoryBuffer*& Result,
                                          StringRef FileName, SourceRange Loc)

{
   auto It = OpenFileForReadQueries.find(FileName);
   OpenFileForReadQuery* _Q;
   if (It == OpenFileForReadQueries.end()) {
      _Q = new (*this) OpenFileForReadQuery(*this, FileName);
      OpenFileForReadQueries[_Q->FileName] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::OpenTmpFile(llvm::raw_ostream*& Result,
                                      StringRef Extension, SourceRange Loc)

{
   auto* _Q = new (*this) OpenTmpFileQuery(*this, Extension);
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::GetDefaultTemplateArg(const sema::TemplateArgument*& Result,
                                    ast::TemplateParamDecl* P, SourceRange Loc)

{
   auto It = GetDefaultTemplateArgQueries.find(P);
   GetDefaultTemplateArgQuery* _Q;
   if (It == GetDefaultTemplateArgQueries.end()) {
      _Q = new (*this) GetDefaultTemplateArgQuery(*this, P);
      GetDefaultTemplateArgQueries[_Q->P] = _Q;
   }
   else {
      _Q = It->getSecond();
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::CreateSelfArgument(ast::FuncArgDecl*& Result,
                                             QualType Self,
                                             SourceLocation SelfLoc,
                                             SourceRange Loc)

{
   auto* _Q = new (*this) CreateSelfArgumentQuery(*this, Self, SelfLoc);
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult
QueryContext::FindEquivalentDecl(ast::NamedDecl*& Result, ast::NamedDecl* Decl,
                                 ast::DeclContext* DC, QualType Self,
                                 bool LookInExtensions, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   FindEquivalentDeclQuery::Profile(ID, Decl, DC, Self, LookInExtensions);
   void* InsertPos;
   auto* _Q = FindEquivalentDeclQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this)
          FindEquivalentDeclQuery(*this, Decl, DC, Self, LookInExtensions);
      FindEquivalentDeclQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

QueryResult QueryContext::EquivalentSignatures(
    EquivalentSignaturesQuery::ResultKind& Result, ast::CallableDecl* C1,
    ast::CallableDecl* C2, QualType Self, SourceRange Loc)

{
   llvm::FoldingSetNodeID ID;
   EquivalentSignaturesQuery::Profile(ID, C1, C2, Self);
   void* InsertPos;
   auto* _Q = EquivalentSignaturesQueries.FindNodeOrInsertPos(ID, InsertPos);
   if (!_Q) {
      _Q = new (*this) EquivalentSignaturesQuery(*this, C1, C2, Self);
      EquivalentSignaturesQueries.InsertNode(_Q, InsertPos);
   }
   switch (_Q->status()) {
   case Query::Running:
      diagnoseCircularDependency(_Q);
      return QueryResult(QueryResult::Error);
   case Query::Idle: {
      ExecutingQuery EQ(*this, _Q);
      if (auto _R = _Q->Query::run()) {
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

#endif

#undef CDOT_QUERY_DECL
#undef CDOT_QUERY_IMPL
#undef CDOT_QUERY_CLASS_IMPL
#undef CDOT_QUERY_CONTEXT_FIELDS
#undef CDOT_QUERY_CONTEXT_DECLS
#undef CDOT_QUERY_CONTEXT_IMPL
