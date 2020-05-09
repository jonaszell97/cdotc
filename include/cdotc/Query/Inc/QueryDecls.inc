class CompileModuleQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CompileModuleQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CompileModuleQuery(QueryContext &QC);

};

class VerifyModuleQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::VerifyModuleQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   VerifyModuleQuery(QueryContext &QC);

};

class ParseModuleFileQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ParseModuleFileQueryID; }

   using result_type = Module*;

   std::string description() const;
   std::string summary() const;

   Module* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ParseModuleFileQuery(QueryContext &QC, StringRef FileName);

   QueryResult finish(Module* Result, Status St = Done);

   StringRef FileName;
   llvm::Optional<Module*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, StringRef FileName);

};

class CreateDefaultModuleQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CreateDefaultModuleQueryID; }

   using result_type = Module*;

   std::string description() const;
   std::string summary() const;

   Module* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateDefaultModuleQuery(QueryContext &QC);

   QueryResult finish(Module* Result, Status St = Done);

   llvm::Optional<Module*> Result;
};

class ParseSourceFileQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ParseSourceFileQueryID; }

   using result_type = ast::SourceFileDecl*;

   std::string description() const;
   std::string summary() const;

   ast::SourceFileDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ParseSourceFileQuery(QueryContext &QC, Module* Mod, StringRef FileName);

   QueryResult finish(ast::SourceFileDecl* Result, Status St = Done);

   Module* Mod;
   StringRef FileName;
   llvm::Optional<ast::SourceFileDecl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, Module* Mod, StringRef FileName);

};

class ParseMainSourceFileQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ParseMainSourceFileQueryID; }

   using result_type = ast::SourceFileDecl*;

   std::string description() const;
   std::string summary() const;

   ast::SourceFileDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ParseMainSourceFileQuery(QueryContext &QC, Module* Mod, StringRef FileName);

   QueryResult finish(ast::SourceFileDecl* Result, Status St = Done);

   Module* Mod;
   StringRef FileName;
   llvm::Optional<ast::SourceFileDecl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, Module* Mod, StringRef FileName);

};

class ParseSourceFilesQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ParseSourceFilesQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ParseSourceFilesQuery(QueryContext &QC, Module* Mod);

   Module* Mod;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, Module* Mod);

};

class CreateILModuleQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CreateILModuleQueryID; }

   using result_type = il::Module*;

   std::string description() const;
   std::string summary() const;

   il::Module* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateILModuleQuery(QueryContext &QC, Module* Mod);

   QueryResult finish(il::Module* Result, Status St = Done);

   Module* Mod;
   llvm::Optional<il::Module*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, Module* Mod);

};

class SetupIRGenQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::SetupIRGenQueryID; }

   using result_type = il::IRGen*;

   std::string description() const;
   std::string summary() const;

   il::IRGen* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   SetupIRGenQuery(QueryContext &QC);

   QueryResult finish(std::unique_ptr<il::IRGen> &&Result, Status St = Done);

   llvm::Optional<std::unique_ptr<il::IRGen>> Result;
};

class CreateLLVMModuleQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CreateLLVMModuleQueryID; }

   using result_type = llvm::Module*;

   std::string description() const;
   std::string summary() const;

   llvm::Module* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateLLVMModuleQuery(QueryContext &QC, Module* Mod);

   QueryResult finish(llvm::Module* Result, Status St = Done);

   Module* Mod;
   llvm::Optional<llvm::Module*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, Module* Mod);

};

class CreateObjectQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CreateObjectQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateObjectQuery(QueryContext &QC, llvm::raw_ostream& OS);

   llvm::raw_ostream& OS;
};

class CreateStaticLibQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CreateStaticLibQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateStaticLibQuery(QueryContext &QC, StringRef OutFile);

   StringRef OutFile;
};

class CreateDynamicLibQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CreateDynamicLibQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateDynamicLibQuery(QueryContext &QC, StringRef OutFile);

   StringRef OutFile;
};

class CreateExecutableQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CreateExecutableQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateExecutableQuery(QueryContext &QC, StringRef OutFile);

   StringRef OutFile;
};

class EmitILQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::EmitILQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   EmitILQuery(QueryContext &QC, llvm::raw_ostream& OS);

   llvm::raw_ostream& OS;
};

class EmitIRQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::EmitIRQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   EmitIRQuery(QueryContext &QC, llvm::raw_ostream& OS);

   llvm::raw_ostream& OS;
};

class PrintUsedMemoryQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrintUsedMemoryQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrintUsedMemoryQuery(QueryContext &QC);

};

class MeasureExecutionTimeQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::MeasureExecutionTimeQueryID; }

   using result_type = long long;

   std::string description() const;
   std::string summary() const;

   long long get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   MeasureExecutionTimeQuery(QueryContext &QC, Query* Q);

   QueryResult finish(long long Result, Status St = Done);

   Query* Q;
   llvm::Optional<long long> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, Query* Q);

};

class IsPersistableQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsPersistableQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsPersistableQuery(QueryContext &QC, CanType T);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class IsEquatableQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsEquatableQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsEquatableQuery(QueryContext &QC, CanType T);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class IsCopyableQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsCopyableQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsCopyableQuery(QueryContext &QC, CanType T);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class IsMoveOnlyQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsMoveOnlyQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsMoveOnlyQuery(QueryContext &QC, CanType T);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class IsUnpopulatedQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsUnpopulatedQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsUnpopulatedQuery(QueryContext &QC, CanType T);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class IsImplicitlyCopyableQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsImplicitlyCopyableQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsImplicitlyCopyableQuery(QueryContext &QC, CanType T);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class IsTriviallyCopyableQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsTriviallyCopyableQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsTriviallyCopyableQuery(QueryContext &QC, CanType T);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class NeedsRetainOrReleaseQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::NeedsRetainOrReleaseQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   NeedsRetainOrReleaseQuery(QueryContext &QC, CanType T);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class NeedsDeinitilizationQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::NeedsDeinitilizationQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   NeedsDeinitilizationQuery(QueryContext &QC, CanType T);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class NeedsStructReturnQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::NeedsStructReturnQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   NeedsStructReturnQuery(QueryContext &QC, CanType T);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class PassByValueQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PassByValueQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PassByValueQuery(QueryContext &QC, CanType T);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class ContainsProtocolWithAssociatedTypesQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ContainsProtocolWithAssociatedTypesQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ContainsProtocolWithAssociatedTypesQuery(QueryContext &QC, CanType T);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class ContainsTemplateQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ContainsTemplateQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ContainsTemplateQuery(QueryContext &QC, CanType T);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class ContainsAssociatedTypeConstraintQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ContainsAssociatedTypeConstraintQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ContainsAssociatedTypeConstraintQuery(QueryContext &QC, CanType T);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class IsImplicitlyConvertibleQuery: public Query {

    public:
        struct ResultType {
            /// Whether or not the types are implicitly convertible.
            bool implicitlyConvertible;

            /// The conversion penalty.
            unsigned conversionPenalty;
        };
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsImplicitlyConvertibleQueryID; }

   using result_type = IsImplicitlyConvertibleQuery::ResultType;

   std::string description() const;
   std::string summary() const;

   IsImplicitlyConvertibleQuery::ResultType get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsImplicitlyConvertibleQuery(QueryContext &QC, QualType From, QualType To, uint8_t flags = 0);

   QueryResult finish(IsImplicitlyConvertibleQuery::ResultType Result, Status St = Done);

   QualType From;
   QualType To;
   uint8_t flags;
   llvm::Optional<IsImplicitlyConvertibleQuery::ResultType> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType From, QualType To, uint8_t flags);

};

class IsValidParameterValueQuery: public Query, public llvm::FoldingSetNode {

    public:
        struct ResultType {
            /// Whether or not the types are implicitly convertible.
            bool isValid;

            /// The conversion penalty.
            unsigned conversionPenalty;
        };
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsValidParameterValueQueryID; }

   using result_type = IsValidParameterValueQuery::ResultType;

   std::string description() const;
   std::string summary() const;

   IsValidParameterValueQuery::ResultType get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsValidParameterValueQuery(QueryContext &QC, QualType givenType, QualType paramType, bool isSelf = false, bool importedFromClang = false);

   QueryResult finish(IsValidParameterValueQuery::ResultType Result, Status St = Done);

   QualType givenType;
   QualType paramType;
   bool isSelf;
   bool importedFromClang;
   llvm::Optional<IsValidParameterValueQuery::ResultType> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType givenType, QualType paramType, bool isSelf, bool importedFromClang);

};

class GetConversionSequenceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetConversionSequenceQueryID; }

   using result_type = ConversionSequence*;

   std::string description() const;
   std::string summary() const;

   ConversionSequence* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetConversionSequenceQuery(QueryContext &QC, QualType From, QualType To, uint8_t flags = 0);

   QueryResult finish(ConversionSequence* Result, Status St = Done);

   QualType From;
   QualType To;
   uint8_t flags;
   llvm::Optional<ConversionSequence*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType From, QualType To, uint8_t flags);

};

class SubstTypeVariablesQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::SubstTypeVariablesQueryID; }

   using result_type = QualType;

   std::string description() const;
   std::string summary() const;

   QualType get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   SubstTypeVariablesQuery(QueryContext &QC, QualType T, const llvm::DenseMap<TypeVariableType*, QualType>& ReplacementMap, SourceRange SR);

   QueryResult finish(QualType Result, Status St = Done);

   QualType T;
   const llvm::DenseMap<TypeVariableType*, QualType>& ReplacementMap;
   SourceRange SR;
   llvm::Optional<QualType> Result;
};

class SubstAssociatedTypesQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::SubstAssociatedTypesQueryID; }

   using result_type = QualType;

   std::string description() const;
   std::string summary() const;

   QualType get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   void refresh();

   SubstAssociatedTypesQuery(QueryContext &QC, QualType T, QualType Self, SourceRange SR);

   QueryResult finish(QualType Result, Status St = Done);

   QualType T;
   QualType Self;
   SourceRange SR;
   llvm::Optional<QualType> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T, QualType Self);

};

class SubstTemplateParamTypesQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::SubstTemplateParamTypesQueryID; }

   using result_type = QualType;

   std::string description() const;
   std::string summary() const;

   QualType get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   SubstTemplateParamTypesQuery(QueryContext &QC, QualType T, sema::MultiLevelFinalTemplateArgList TemplateArgs, SourceRange SR);

   QueryResult finish(QualType Result, Status St = Done);

   QualType T;
   sema::MultiLevelFinalTemplateArgList TemplateArgs;
   SourceRange SR;
   llvm::Optional<QualType> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T, const sema::MultiLevelFinalTemplateArgList &TemplateArgs);

};

class SubstTemplateParamTypesNonFinalQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::SubstTemplateParamTypesNonFinalQueryID; }

   using result_type = QualType;

   std::string description() const;
   std::string summary() const;

   QualType get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   SubstTemplateParamTypesNonFinalQuery(QueryContext &QC, QualType T, const sema::MultiLevelTemplateArgList& TemplateArgs, SourceRange SR);

   QueryResult finish(QualType Result, Status St = Done);

   QualType T;
   const sema::MultiLevelTemplateArgList& TemplateArgs;
   SourceRange SR;
   llvm::Optional<QualType> Result;
};

class CheckTypeCapabilitiesQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CheckTypeCapabilitiesQueryID; }

   using result_type = ArrayRef<TypeCapability>;

   std::string description() const;
   std::string summary() const;

   ArrayRef<TypeCapability> get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckTypeCapabilitiesQuery(QueryContext &QC, ast::NamedDecl* ND);

   QueryResult finish(std::vector<TypeCapability> &&Result, Status St = Done);

   ast::NamedDecl* ND;
   llvm::Optional<std::vector<TypeCapability>> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::NamedDecl* ND);

};

class GetBuiltinModuleQuery: public Query {

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
            Policy,
            Atomic,
        };

    private:
        llvm::DenseMap<char, Module*> Cache;
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetBuiltinModuleQueryID; }

   using result_type = Module*;

   std::string description() const;
   std::string summary() const;

   Module* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetBuiltinModuleQuery(QueryContext &QC, GetBuiltinModuleQuery::ModuleKind Mod);

   QueryResult finish(Module* Result, Status St = Done);

   GetBuiltinModuleQuery::ModuleKind Mod;
   llvm::Optional<Module*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, GetBuiltinModuleQuery::ModuleKind Mod);

};

class GetBuiltinFuncQuery: public Query {

    public:
        enum FunctionKind : char {
            PureVirtual,
            CopyClass,
            AtomicRelease,
        };

    private:
        llvm::DenseMap<char, ast::CallableDecl*> Cache;
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetBuiltinFuncQueryID; }

   using result_type = ast::CallableDecl*;

   std::string description() const;
   std::string summary() const;

   ast::CallableDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetBuiltinFuncQuery(QueryContext &QC, GetBuiltinFuncQuery::FunctionKind Fn);

   QueryResult finish(ast::CallableDecl* Result, Status St = Done);

   GetBuiltinFuncQuery::FunctionKind Fn;
   llvm::Optional<ast::CallableDecl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, GetBuiltinFuncQuery::FunctionKind Fn);

};

class GetBuiltinAliasQuery: public Query, public llvm::FoldingSetNode {

    public:
        enum AliasKind: uint8_t {
            /// The default type for signed integer literals.
            DefaultSignedIntegerLiteralType,

            /// The default type for unsigned integer literals.
            DefaultUnsignedIntegerLiteralType,

            /// The default type for floating point literals.
            DefaultFloatingPointLiteralType,

            /// The default type for boolean literals.
            DefaultBooleanLiteralType,

            /// The default type for character literals.
            DefaultCharacterLiteralType,

            /// The default type for string literals.
            DefaultStringLiteralType,

            /// The default type for array literals.
            DefaultArrayLiteralType,

            /// The default type for dictionary literals.
            DefaultDictionaryLiteralType,
        };
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetBuiltinAliasQueryID; }

   using result_type = ast::AliasDecl*;

   std::string description() const;
   std::string summary() const;

   ast::AliasDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetBuiltinAliasQuery(QueryContext &QC, GetBuiltinAliasQuery::AliasKind K, ast::DeclContext* DC = nullptr);

   QueryResult finish(ast::AliasDecl* Result, Status St = Done);

   GetBuiltinAliasQuery::AliasKind K;
   ast::DeclContext* DC;
   llvm::Optional<ast::AliasDecl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, GetBuiltinAliasQuery::AliasKind K, ast::DeclContext* DC);

};

class GetBuiltinRecordQuery: public Query {

    public:
        enum RecordKind: uint32_t {
            Bool,
            Character,
            Int8, UInt8,
            Int16, UInt16,
            Int32, UInt32,
            Int64, UInt64,
            Int128, UInt128,
            Float, Double,

            UnsafePtr, UnsafeMutablePtr,
            UnsafeRawPtr, UnsafeMutableRawPtr,
            UnsafeBufferPtr, UnsafeMutableBufferPtr,

            Array,
            Dictionary,
            Atomic,
            String, StringBuffer, StringStorage,
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
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetBuiltinRecordQueryID; }

   using result_type = ast::RecordDecl*;

   std::string description() const;
   std::string summary() const;

   ast::RecordDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetBuiltinRecordQuery(QueryContext &QC, GetBuiltinRecordQuery::RecordKind R);

   QueryResult finish(ast::RecordDecl* Result, Status St = Done);

   GetBuiltinRecordQuery::RecordKind R;
   llvm::Optional<ast::RecordDecl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, GetBuiltinRecordQuery::RecordKind R);

};

class GetBuiltinProtocolQuery: public Query {

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
            TruthValue,
            RawRepresentable,
            Dereferenceable,
            Unwrappable,
        };
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetBuiltinProtocolQueryID; }

   using result_type = ast::ProtocolDecl*;

   std::string description() const;
   std::string summary() const;

   ast::ProtocolDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetBuiltinProtocolQuery(QueryContext &QC, GetBuiltinProtocolQuery::ProtocolKind P);

   QueryResult finish(ast::ProtocolDecl* Result, Status St = Done);

   GetBuiltinProtocolQuery::ProtocolKind P;
   llvm::Optional<ast::ProtocolDecl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, GetBuiltinProtocolQuery::ProtocolKind P);

};

class FindPrecedenceGroupQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::FindPrecedenceGroupQueryID; }

   using result_type = ast::PrecedenceGroupDecl*;

   std::string description() const;
   std::string summary() const;

   ast::PrecedenceGroupDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   FindPrecedenceGroupQuery(QueryContext &QC, DeclarationName Name, bool DiagOnError = true, SourceRange Loc = SourceRange());

   QueryResult finish(ast::PrecedenceGroupDecl* Result, Status St = Done);

   DeclarationName Name;
   bool DiagOnError;
   SourceRange Loc;
   llvm::Optional<ast::PrecedenceGroupDecl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, DeclarationName Name, bool DiagOnError);

};

class FindOperatorQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::FindOperatorQueryID; }

   using result_type = ast::OperatorDecl*;

   std::string description() const;
   std::string summary() const;

   ast::OperatorDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   FindOperatorQuery(QueryContext &QC, DeclarationName Name, bool DiagOnError = true, SourceRange Loc = SourceRange());

   QueryResult finish(ast::OperatorDecl* Result, Status St = Done);

   DeclarationName Name;
   bool DiagOnError;
   SourceRange Loc;
   llvm::Optional<ast::OperatorDecl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, DeclarationName Name, bool DiagOnError);

};

class ResolveImportQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveImportQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveImportQuery(QueryContext &QC, ast::ImportDecl* I, llvm::SmallVectorImpl<ast::NamedDecl*>* FoundDecls = nullptr);

   ast::ImportDecl* I;
   llvm::SmallVectorImpl<ast::NamedDecl*>* FoundDecls;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::ImportDecl* I);

};

class ResolveUsingQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveUsingQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveUsingQuery(QueryContext &QC, ast::UsingDecl* U, llvm::SmallVectorImpl<ast::NamedDecl*>* FoundDecls = nullptr);

   ast::UsingDecl* U;
   llvm::SmallVectorImpl<ast::NamedDecl*>* FoundDecls;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::UsingDecl* U);

};

class ResolveStaticIfQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveStaticIfQueryID; }

   using result_type = ast::Decl*;

   std::string description() const;
   std::string summary() const;

   ast::Decl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveStaticIfQuery(QueryContext &QC, ast::StaticIfDecl* Decl);

   QueryResult finish(ast::Decl* Result, Status St = Done);

   ast::StaticIfDecl* Decl;
   llvm::Optional<ast::Decl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::StaticIfDecl* Decl);

};

class ResolveStaticForQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveStaticForQueryID; }

   using result_type = ast::Decl*;

   std::string description() const;
   std::string summary() const;

   ast::Decl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveStaticForQuery(QueryContext &QC, ast::StaticForDecl* Decl);

   QueryResult finish(ast::Decl* Result, Status St = Done);

   ast::StaticForDecl* Decl;
   llvm::Optional<ast::Decl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::StaticForDecl* Decl);

};

class ResolveStaticAssertQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveStaticAssertQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveStaticAssertQuery(QueryContext &QC, ast::StaticAssertDecl* Decl);

   ast::StaticAssertDecl* Decl;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::StaticAssertDecl* Decl);

};

class ResolveStaticPrintQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveStaticPrintQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveStaticPrintQuery(QueryContext &QC, ast::StaticPrintDecl* Decl);

   ast::StaticPrintDecl* Decl;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::StaticPrintDecl* Decl);

};

class ResolveStaticExprQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveStaticExprQueryID; }

   using result_type = il::Constant*;

   std::string description() const;
   std::string summary() const;

   il::Constant* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveStaticExprQuery(QueryContext &QC, ast::StaticExpr* Expr);

   QueryResult finish(il::Constant* Result, Status St = Done);

   ast::StaticExpr* Expr;
   llvm::Optional<il::Constant*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::StaticExpr* Expr);

};

class ResolveStaticExprToBoolQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveStaticExprToBoolQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveStaticExprToBoolQuery(QueryContext &QC, ast::StaticExpr* Expr, bool DiagOnError);

   QueryResult finish(bool Result, Status St = Done);

   ast::StaticExpr* Expr;
   bool DiagOnError;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::StaticExpr* Expr, bool DiagOnError);

};

class ExpandMacroQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ExpandMacroQueryID; }

   using result_type = StmtOrDecl;

   std::string description() const;
   std::string summary() const;

   StmtOrDecl get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ExpandMacroQuery(QueryContext &QC, StmtOrDecl SOD, DeclarationName Name, ast::DeclContext* DC, unsigned Delim, llvm::ArrayRef<lex::Token> Tokens, unsigned ExpectedKind);

   QueryResult finish(StmtOrDecl Result, Status St = Done);

   StmtOrDecl SOD;
   DeclarationName Name;
   ast::DeclContext* DC;
   unsigned Delim;
   llvm::ArrayRef<lex::Token> Tokens;
   unsigned ExpectedKind;
   llvm::Optional<StmtOrDecl> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, StmtOrDecl SOD, DeclarationName Name, ast::DeclContext* DC, unsigned Delim, llvm::ArrayRef<lex::Token> Tokens, unsigned ExpectedKind);

};

class ExpandMacroDeclQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ExpandMacroDeclQueryID; }

   using result_type = ast::Decl*;

   std::string description() const;
   std::string summary() const;

   ast::Decl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ExpandMacroDeclQuery(QueryContext &QC, ast::MacroExpansionDecl* Decl);

   QueryResult finish(ast::Decl* Result, Status St = Done);

   ast::MacroExpansionDecl* Decl;
   llvm::Optional<ast::Decl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::MacroExpansionDecl* Decl);

};

class ExpandMacrosQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ExpandMacrosQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ExpandMacrosQuery(QueryContext &QC, StringRef SourceFile);

   StringRef SourceFile;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, StringRef SourceFile);

};

class ImportExternalSourceFileQuery: public Query, public llvm::FoldingSetNode {

    public:
        enum HeaderKind {
            /// A C header.
            C,

            /// A C++ header.
            CXX,

            /// A C system header.
            CSystem,
        };
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ImportExternalSourceFileQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ImportExternalSourceFileQuery(QueryContext &QC, StringRef FileName, ImportExternalSourceFileQuery::HeaderKind Kind, ast::DeclContext* DC, SourceLocation Loc, bool Optional = false);

   StringRef FileName;
   ImportExternalSourceFileQuery::HeaderKind Kind;
   ast::DeclContext* DC;
   SourceLocation Loc;
   bool Optional;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, StringRef FileName, ImportExternalSourceFileQuery::HeaderKind Kind, ast::DeclContext* DC, SourceLocation Loc, bool Optional);

};

class FindExtensionsQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::FindExtensionsQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   FindExtensionsQuery(QueryContext &QC, QualType T);

   QualType T;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T);

};

class GetExtensionTypeKindQuery: public Query {

    public:
        enum ResultKind {
            /// Extends a nominal type.
            Nominal,

            /// Extends a builtin type.
            Builtin,

            /// Extends a pointer type.
            Pointer,

            /// Extends a tuple type.
            Tuple,

            /// Extends a function type.
            Function,

            /// Extends an array type.
            Array,
        };
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetExtensionTypeKindQueryID; }

   using result_type = GetExtensionTypeKindQuery::ResultKind;

   std::string description() const;
   std::string summary() const;

   GetExtensionTypeKindQuery::ResultKind get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetExtensionTypeKindQuery(QueryContext &QC, ast::SourceType T);

   QueryResult finish(GetExtensionTypeKindQuery::ResultKind Result, Status St = Done);

   ast::SourceType T;
   llvm::Optional<GetExtensionTypeKindQuery::ResultKind> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::SourceType T);

};

class MatchExtensionTypeQuery: public Query, public llvm::FoldingSetNode {

    public:
        enum ResultKind {
            /// The extension applies only to this declaration.
            AppliesDirectly,

            /// The extension applies to this and possibly other declarations.
            Applies,

            /// The extension might apply, but we can't tell for sure.
            MightApply,

            /// The extension can't possibly apply.
            DoesNotApply
        };
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::MatchExtensionTypeQueryID; }

   using result_type = MatchExtensionTypeQuery::ResultKind;

   std::string description() const;
   std::string summary() const;

   MatchExtensionTypeQuery::ResultKind get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   MatchExtensionTypeQuery(QueryContext &QC, QualType T, ast::SourceType PatternTy);

   QueryResult finish(MatchExtensionTypeQuery::ResultKind Result, Status St = Done);

   QualType T;
   ast::SourceType PatternTy;
   llvm::Optional<MatchExtensionTypeQuery::ResultKind> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T, ast::SourceType PatternTy);

};

class ExtensionAppliesQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ExtensionAppliesQueryID; }

   using result_type = MatchExtensionTypeQuery::ResultKind;

   std::string description() const;
   std::string summary() const;

   MatchExtensionTypeQuery::ResultKind get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ExtensionAppliesQuery(QueryContext &QC, ast::ExtensionDecl* Ext, QualType T);

   QueryResult finish(MatchExtensionTypeQuery::ResultKind Result, Status St = Done);

   ast::ExtensionDecl* Ext;
   QualType T;
   llvm::Optional<MatchExtensionTypeQuery::ResultKind> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::ExtensionDecl* Ext, QualType T);

};

class GetExtendedDeclQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetExtendedDeclQueryID; }

   using result_type = ast::NamedDecl*;

   std::string description() const;
   std::string summary() const;

   ast::NamedDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetExtendedDeclQuery(QueryContext &QC, ast::SourceType ExtendedTy);

   QueryResult finish(ast::NamedDecl* Result, Status St = Done);

   ast::SourceType ExtendedTy;
   llvm::Optional<ast::NamedDecl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::SourceType ExtendedTy);

};

class ResolveExtensionQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveExtensionQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveExtensionQuery(QueryContext &QC, ast::ExtensionDecl* Ext);

   ast::ExtensionDecl* Ext;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::ExtensionDecl* Ext);

};

class ResolveConformanceToProtocolQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveConformanceToProtocolQueryID; }

   using result_type = ast::ProtocolDecl*;

   std::string description() const;
   std::string summary() const;

   ast::ProtocolDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveConformanceToProtocolQuery(QueryContext &QC, QualType T, const ast::SourceType& Conf);

   QueryResult finish(ast::ProtocolDecl* Result, Status St = Done);

   QualType T;
   const ast::SourceType& Conf;
   llvm::Optional<ast::ProtocolDecl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T, const ast::SourceType& Conf);

};

class ResolveExplicitConformancesQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveExplicitConformancesQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveExplicitConformancesQuery(QueryContext &QC, QualType T);

   QualType T;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T);

};

class ResolveConformancesToProtocolsQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveConformancesToProtocolsQueryID; }

   using result_type = ArrayRef<ast::ProtocolDecl*>;

   std::string description() const;
   std::string summary() const;

   ArrayRef<ast::ProtocolDecl*> get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveConformancesToProtocolsQuery(QueryContext &QC, QualType T, llvm::MutableArrayRef<ast::SourceType> Conformances);

   QueryResult finish(llvm::SmallVector<ast::ProtocolDecl*, 2> &&Result, Status St = Done);

   QualType T;
   llvm::MutableArrayRef<ast::SourceType> Conformances;
   llvm::Optional<llvm::SmallVector<ast::ProtocolDecl*, 2>> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T, llvm::MutableArrayRef<ast::SourceType> Conformances);

};

class ResolveDeclaredConformancesQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveDeclaredConformancesQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveDeclaredConformancesQuery(QueryContext &QC, QualType T, llvm::MutableArrayRef<ast::SourceType> Conformances, ConformanceKind Kind, ast::ConstraintSet* CS = nullptr);

   QualType T;
   llvm::MutableArrayRef<ast::SourceType> Conformances;
   ConformanceKind Kind;
   ast::ConstraintSet* CS;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T, llvm::MutableArrayRef<ast::SourceType> Conformances, ConformanceKind Kind, ast::ConstraintSet* CS);

};

class AddSingleConformanceQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::AddSingleConformanceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   AddSingleConformanceQuery(QueryContext &QC, QualType T, ast::ProtocolDecl* Proto, ConformanceKind Kind, ast::ConstraintSet* CS = nullptr, bool AddRecursiveConformances = true);

   QualType T;
   ast::ProtocolDecl* Proto;
   ConformanceKind Kind;
   ast::ConstraintSet* CS;
   bool AddRecursiveConformances;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T, ast::ProtocolDecl* Proto, ConformanceKind Kind, ast::ConstraintSet* CS, bool AddRecursiveConformances);

};

class DeclareSelfAliasQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::DeclareSelfAliasQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   DeclareSelfAliasQuery(QueryContext &QC, ast::RecordDecl* R);

   ast::RecordDecl* R;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R);

};

class ReferencedAssociatedTypesReadyQuery: public Query {

    public:
        enum ResultKind : uint8_t {
            /// All referenced associated types are ready.
            Ready,

            /// Some associated types are not ready.
            NotReady,

            /// The type directly references 'Self'.
            ReferencesSelf,
        };
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ReferencedAssociatedTypesReadyQueryID; }

   using result_type = ReferencedAssociatedTypesReadyQuery::ResultKind;

   std::string description() const;
   std::string summary() const;

   ReferencedAssociatedTypesReadyQuery::ResultKind get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ReferencedAssociatedTypesReadyQuery(QueryContext &QC, QualType T, ast::ConstraintSet* CS);

   QueryResult finish(ReferencedAssociatedTypesReadyQuery::ResultKind Result, Status St = Done);

   QualType T;
   ast::ConstraintSet* CS;
   llvm::Optional<ReferencedAssociatedTypesReadyQuery::ResultKind> Result;
};

class CheckProtocolExtensionApplicabilityQuery: public Query, public llvm::FoldingSetNode {

    public:
        enum ResultKind : uint8_t {
            /// We can't tell yet whether or not this extension applies.
            CantTell,

            /// This extension doesn't apply.
            DoesNotApply,

            /// This extension does apply.
            DoesApply,
        };
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CheckProtocolExtensionApplicabilityQueryID; }

   using result_type = CheckProtocolExtensionApplicabilityQuery::ResultKind;

   std::string description() const;
   std::string summary() const;

   CheckProtocolExtensionApplicabilityQuery::ResultKind get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckProtocolExtensionApplicabilityQuery(QueryContext &QC, QualType T, ast::ExtensionDecl* Ext);

   QueryResult finish(CheckProtocolExtensionApplicabilityQuery::ResultKind Result, Status St = Done);

   QualType T;
   ast::ExtensionDecl* Ext;
   llvm::Optional<CheckProtocolExtensionApplicabilityQuery::ResultKind> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T, ast::ExtensionDecl* Ext);

};

class VerifyConstraintQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::VerifyConstraintQueryID; }

   using result_type = ast::DeclConstraint*;

   std::string description() const;
   std::string summary() const;

   ast::DeclConstraint* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   VerifyConstraintQuery(QueryContext &QC, const ast::ParsedConstraint* C, ast::NamedDecl* ConstrainedDecl);

   QueryResult finish(ast::DeclConstraint* Result, Status St = Done);

   const ast::ParsedConstraint* C;
   ast::NamedDecl* ConstrainedDecl;
   llvm::Optional<ast::DeclConstraint*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, const ast::ParsedConstraint* C, ast::NamedDecl* ConstrainedDecl);

};

class VerifyConstraintsQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::VerifyConstraintsQueryID; }

   using result_type = ast::ConstraintSet*;

   std::string description() const;
   std::string summary() const;

   ast::ConstraintSet* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   VerifyConstraintsQuery(QueryContext &QC, ast::NamedDecl* ConstrainedDecl);

   QueryResult finish(ast::ConstraintSet* Result, Status St = Done);

   ast::NamedDecl* ConstrainedDecl;
   llvm::Optional<ast::ConstraintSet*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::NamedDecl* ConstrainedDecl);

};

class IsConstraintSatisfiedQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsConstraintSatisfiedQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsConstraintSatisfiedQuery(QueryContext &QC, const ast::DeclConstraint* C, ast::NamedDecl* ConcreteDecl, ast::NamedDecl* OriginalDecl, sema::FinalTemplateArgumentList* TemplateArgs = nullptr);

   QueryResult finish(bool Result, Status St = Done);

   const ast::DeclConstraint* C;
   ast::NamedDecl* ConcreteDecl;
   ast::NamedDecl* OriginalDecl;
   sema::FinalTemplateArgumentList* TemplateArgs;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, const ast::DeclConstraint* C, ast::NamedDecl* ConcreteDecl, ast::NamedDecl* OriginalDecl, sema::FinalTemplateArgumentList* TemplateArgs);

};

class GetConstrainedTypeQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetConstrainedTypeQueryID; }

   using result_type = QualType;

   std::string description() const;
   std::string summary() const;

   QualType get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetConstrainedTypeQuery(QueryContext &QC, const ast::DeclConstraint* C, ast::NamedDecl* ConcreteDecl, ast::NamedDecl* OriginalDecl, sema::FinalTemplateArgumentList* TemplateArgs = nullptr);

   QueryResult finish(QualType Result, Status St = Done);

   const ast::DeclConstraint* C;
   ast::NamedDecl* ConcreteDecl;
   ast::NamedDecl* OriginalDecl;
   sema::FinalTemplateArgumentList* TemplateArgs;
   llvm::Optional<QualType> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, const ast::DeclConstraint* C, ast::NamedDecl* ConcreteDecl, ast::NamedDecl* OriginalDecl, sema::FinalTemplateArgumentList* TemplateArgs);

};

class IsSupersetOfQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsSupersetOfQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsSupersetOfQuery(QueryContext &QC, ast::ConstraintSet* C1, ast::ConstraintSet* C2);

   QueryResult finish(bool Result, Status St = Done);

   ast::ConstraintSet* C1;
   ast::ConstraintSet* C2;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::ConstraintSet* C1, ast::ConstraintSet* C2);

};

class GetReferencedAssociatedTypesQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetReferencedAssociatedTypesQueryID; }

   using result_type = llvm::ArrayRef<ast::AssociatedTypeDecl*>;

   std::string description() const;
   std::string summary() const;

   llvm::ArrayRef<ast::AssociatedTypeDecl*> get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetReferencedAssociatedTypesQuery(QueryContext &QC, ast::ConstraintSet* CS);

   QueryResult finish(std::vector<ast::AssociatedTypeDecl*> &&Result, Status St = Done);

   ast::ConstraintSet* CS;
   llvm::Optional<std::vector<ast::AssociatedTypeDecl*>> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::ConstraintSet* CS);

};

class GetNeededAssociatedTypesQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetNeededAssociatedTypesQueryID; }

   using result_type = llvm::ArrayRef<ast::AssociatedTypeDecl*>;

   std::string description() const;
   std::string summary() const;

   llvm::ArrayRef<ast::AssociatedTypeDecl*> get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetNeededAssociatedTypesQuery(QueryContext &QC, ast::RecordDecl* R);

   QueryResult finish(llvm::SmallVector<ast::AssociatedTypeDecl*, 4> &&Result, Status St = Done);

   ast::RecordDecl* R;
   llvm::Optional<llvm::SmallVector<ast::AssociatedTypeDecl*, 4>> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R);

};

class CheckConformancesQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CheckConformancesQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckConformancesQuery(QueryContext &QC, QualType T, std::vector<ast::NamedDecl*> * ExtensionDecls = nullptr);

   QualType T;
   std::vector<ast::NamedDecl*> * ExtensionDecls;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T, std::vector<ast::NamedDecl*> * ExtensionDecls);

};

class CheckSingleConformanceQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CheckSingleConformanceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckSingleConformanceQuery(QueryContext &QC, QualType T, ast::ProtocolDecl* P);

   QualType T;
   ast::ProtocolDecl* P;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T, ast::ProtocolDecl* P);

};

class CheckAssociatedTypeConstraintsQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CheckAssociatedTypeConstraintsQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckAssociatedTypeConstraintsQuery(QueryContext &QC, ast::RecordDecl* R);

   ast::RecordDecl* R;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R);

};

class TypecheckConstraintsQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckConstraintsQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckConstraintsQuery(QueryContext &QC, ast::NamedDecl* ND);

   ast::NamedDecl* ND;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::NamedDecl* ND);

};

class ConformsToQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ConformsToQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   void refresh();

   ConformsToQuery(QueryContext &QC, CanType T, ast::ProtocolDecl* P);

   QueryResult finish(bool Result, Status St = Done);

   CanType T;
   ast::ProtocolDecl* P;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T, ast::ProtocolDecl* P);

};

class IsCovariantQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsCovariantQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   void refresh();

   IsCovariantQuery(QueryContext &QC, QualType T, QualType Covar);

   QueryResult finish(bool Result, Status St = Done);

   QualType T;
   QualType Covar;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T, QualType Covar);

};

class IsContravariantQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsContravariantQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   void refresh();

   IsContravariantQuery(QueryContext &QC, QualType T, QualType Contravar);

   QueryResult finish(bool Result, Status St = Done);

   QualType T;
   QualType Contravar;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T, QualType Contravar);

};

class GetTypeSizeQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetTypeSizeQueryID; }

   using result_type = unsigned;

   std::string description() const;
   std::string summary() const;

   unsigned get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetTypeSizeQuery(QueryContext &QC, CanType T);

   QueryResult finish(unsigned Result, Status St = Done);

   CanType T;
   llvm::Optional<unsigned> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class GetTypeAlignmentQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetTypeAlignmentQueryID; }

   using result_type = unsigned short;

   std::string description() const;
   std::string summary() const;

   unsigned short get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetTypeAlignmentQuery(QueryContext &QC, CanType T);

   QueryResult finish(unsigned short Result, Status St = Done);

   CanType T;
   llvm::Optional<unsigned short> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class GetTypeStrideQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetTypeStrideQueryID; }

   using result_type = unsigned;

   std::string description() const;
   std::string summary() const;

   unsigned get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetTypeStrideQuery(QueryContext &QC, CanType T);

   QueryResult finish(unsigned Result, Status St = Done);

   CanType T;
   llvm::Optional<unsigned> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T);

};

class CalculateRecordSizeQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CalculateRecordSizeQueryID; }

   using result_type = unsigned;

   std::string description() const;
   std::string summary() const;

   unsigned get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CalculateRecordSizeQuery(QueryContext &QC, ast::RecordDecl* R);

   QueryResult finish(unsigned Result, Status St = Done);

   ast::RecordDecl* R;
   llvm::Optional<unsigned> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R);

};

class GetRecordMetaQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetRecordMetaQueryID; }

   using result_type = const RecordMetaInfo*;

   std::string description() const;
   std::string summary() const;

   const RecordMetaInfo* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetRecordMetaQuery(QueryContext &QC, ast::RecordDecl* R, uint8_t Initialize = true);

   QueryResult finish(const RecordMetaInfo* Result, Status St = Done);

   ast::RecordDecl* R;
   uint8_t Initialize;
   llvm::Optional<const RecordMetaInfo*> Result;
};

class IsBuiltinIntegerTypeQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsBuiltinIntegerTypeQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsBuiltinIntegerTypeQuery(QueryContext &QC, ast::RecordDecl* R);

   QueryResult finish(bool Result, Status St = Done);

   ast::RecordDecl* R;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R);

};

class IsBuiltinFloatingPointTypeQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsBuiltinFloatingPointTypeQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsBuiltinFloatingPointTypeQuery(QueryContext &QC, ast::RecordDecl* R);

   QueryResult finish(bool Result, Status St = Done);

   ast::RecordDecl* R;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R);

};

class IsBuiltinBoolTypeQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsBuiltinBoolTypeQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsBuiltinBoolTypeQuery(QueryContext &QC, ast::RecordDecl* R);

   QueryResult finish(bool Result, Status St = Done);

   ast::RecordDecl* R;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R);

};

class AddImplicitConformanceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::AddImplicitConformanceQueryID; }

   using result_type = ast::MethodDecl*;

   std::string description() const;
   std::string summary() const;

   ast::MethodDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   AddImplicitConformanceQuery(QueryContext &QC, ast::RecordDecl* R, ast::ImplicitConformanceKind K, ast::MethodDecl* Impl = nullptr);

   QueryResult finish(ast::MethodDecl* Result, Status St = Done);

   ast::RecordDecl* R;
   ast::ImplicitConformanceKind K;
   ast::MethodDecl* Impl;
   llvm::Optional<ast::MethodDecl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R, ast::ImplicitConformanceKind K, ast::MethodDecl* Impl);

};

class GetImplicitConformanceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetImplicitConformanceQueryID; }

   using result_type = ast::MethodDecl*;

   std::string description() const;
   std::string summary() const;

   ast::MethodDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetImplicitConformanceQuery(QueryContext &QC, ast::RecordDecl* R, ast::ImplicitConformanceKind K);

   QueryResult finish(ast::MethodDecl* Result, Status St = Done);

   ast::RecordDecl* R;
   ast::ImplicitConformanceKind K;
   llvm::Optional<ast::MethodDecl*> Result;
};

class CheckBuiltinConformancesQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CheckBuiltinConformancesQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckBuiltinConformancesQuery(QueryContext &QC, ast::RecordDecl* R);

   ast::RecordDecl* R;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R);

};

class IsAccessibleQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::IsAccessibleQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   IsAccessibleQuery(QueryContext &QC, ast::DeclContext* DC, ast::NamedDecl* ND);

   QueryResult finish(bool Result, Status St = Done);

   ast::DeclContext* DC;
   ast::NamedDecl* ND;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC, ast::NamedDecl* ND);

};

class CheckAccessibilityQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CheckAccessibilityQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckAccessibilityQuery(QueryContext &QC, ast::DeclContext* DC, ast::NamedDecl* ND, SourceLocation Loc);

   ast::DeclContext* DC;
   ast::NamedDecl* ND;
   SourceLocation Loc;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC, ast::NamedDecl* ND, SourceLocation Loc);

};

class ResolveNestedNameSpecToTypeQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveNestedNameSpecToTypeQueryID; }

   using result_type = QualType;

   std::string description() const;
   std::string summary() const;

   QualType get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveNestedNameSpecToTypeQuery(QueryContext &QC, NestedNameSpecifierWithLoc* Name, ast::DeclContext* DC = nullptr, bool IssueDiag = true);

   QueryResult finish(QualType Result, Status St = Done);

   NestedNameSpecifierWithLoc* Name;
   ast::DeclContext* DC;
   bool IssueDiag;
   llvm::Optional<QualType> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, NestedNameSpecifierWithLoc* Name, ast::DeclContext* DC, bool IssueDiag);

};

class ResolveNestedNameSpecToDeclQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveNestedNameSpecToDeclQueryID; }

   using result_type = ast::NamedDecl*;

   std::string description() const;
   std::string summary() const;

   ast::NamedDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveNestedNameSpecToDeclQuery(QueryContext &QC, NestedNameSpecifierWithLoc* Name, ast::DeclContext* DC = nullptr, bool IssueDiag = true);

   QueryResult finish(ast::NamedDecl* Result, Status St = Done);

   NestedNameSpecifierWithLoc* Name;
   ast::DeclContext* DC;
   bool IssueDiag;
   llvm::Optional<ast::NamedDecl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, NestedNameSpecifierWithLoc* Name, ast::DeclContext* DC, bool IssueDiag);

};

class TypeCheckDeclContextQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypeCheckDeclContextQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypeCheckDeclContextQuery(QueryContext &QC, ast::DeclContext* DC);

   ast::DeclContext* DC;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC);

};

class PrepareDeclInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareDeclInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareDeclInterfaceQuery(QueryContext &QC, ast::Decl* D);

   ast::Decl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::Decl* D);

};

class TypecheckDeclQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckDeclQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckDeclQuery(QueryContext &QC, ast::Decl* D);

   ast::Decl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::Decl* D);

};

class PreparePrecedenceGroupInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PreparePrecedenceGroupInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PreparePrecedenceGroupInterfaceQuery(QueryContext &QC, ast::PrecedenceGroupDecl* D);

   ast::PrecedenceGroupDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::PrecedenceGroupDecl* D);

};

class TypecheckPrecedenceGroupQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckPrecedenceGroupQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckPrecedenceGroupQuery(QueryContext &QC, ast::PrecedenceGroupDecl* D);

   ast::PrecedenceGroupDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::PrecedenceGroupDecl* D);

};

class PrepareOperatorInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareOperatorInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareOperatorInterfaceQuery(QueryContext &QC, ast::OperatorDecl* D);

   ast::OperatorDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::OperatorDecl* D);

};

class TypecheckOperatorQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckOperatorQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckOperatorQuery(QueryContext &QC, ast::OperatorDecl* D);

   ast::OperatorDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::OperatorDecl* D);

};

class PrepareGlobalVarInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareGlobalVarInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareGlobalVarInterfaceQuery(QueryContext &QC, ast::GlobalVarDecl* D);

   ast::GlobalVarDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::GlobalVarDecl* D);

};

class TypecheckGlobalVarQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckGlobalVarQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckGlobalVarQuery(QueryContext &QC, ast::GlobalVarDecl* D);

   ast::GlobalVarDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::GlobalVarDecl* D);

};

class PrepareCallableInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareCallableInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareCallableInterfaceQuery(QueryContext &QC, ast::CallableDecl* D);

   ast::CallableDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::CallableDecl* D);

};

class TypecheckCallableQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckCallableQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckCallableQuery(QueryContext &QC, ast::CallableDecl* D);

   ast::CallableDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::CallableDecl* D);

};

class PrepareFunctionInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareFunctionInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareFunctionInterfaceQuery(QueryContext &QC, ast::FunctionDecl* D);

   ast::FunctionDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::FunctionDecl* D);

};

class TypecheckFunctionQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckFunctionQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckFunctionQuery(QueryContext &QC, ast::FunctionDecl* D);

   ast::FunctionDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::FunctionDecl* D);

};

class PrepareMethodInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareMethodInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareMethodInterfaceQuery(QueryContext &QC, ast::MethodDecl* D);

   ast::MethodDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::MethodDecl* D);

};

class TypecheckMethodQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckMethodQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckMethodQuery(QueryContext &QC, ast::MethodDecl* D);

   ast::MethodDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::MethodDecl* D);

};

class PrepareInitInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareInitInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareInitInterfaceQuery(QueryContext &QC, ast::InitDecl* D);

   ast::InitDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::InitDecl* D);

};

class TypecheckInitQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckInitQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckInitQuery(QueryContext &QC, ast::InitDecl* D);

   ast::InitDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::InitDecl* D);

};

class PrepareDeinitInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareDeinitInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareDeinitInterfaceQuery(QueryContext &QC, ast::DeinitDecl* D);

   ast::DeinitDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeinitDecl* D);

};

class TypecheckDeinitQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckDeinitQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckDeinitQuery(QueryContext &QC, ast::DeinitDecl* D);

   ast::DeinitDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeinitDecl* D);

};

class PrepareFuncArgInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareFuncArgInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareFuncArgInterfaceQuery(QueryContext &QC, ast::FuncArgDecl* D);

   ast::FuncArgDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::FuncArgDecl* D);

};

class TypecheckFuncArgQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckFuncArgQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckFuncArgQuery(QueryContext &QC, ast::FuncArgDecl* D);

   ast::FuncArgDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::FuncArgDecl* D);

};

class PrepareTemplateParamInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareTemplateParamInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareTemplateParamInterfaceQuery(QueryContext &QC, ast::TemplateParamDecl* D);

   ast::TemplateParamDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::TemplateParamDecl* D);

};

class TypecheckTemplateParamQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckTemplateParamQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckTemplateParamQuery(QueryContext &QC, ast::TemplateParamDecl* D);

   ast::TemplateParamDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::TemplateParamDecl* D);

};

class PrepareAssociatedTypeInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareAssociatedTypeInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareAssociatedTypeInterfaceQuery(QueryContext &QC, ast::AssociatedTypeDecl* D);

   ast::AssociatedTypeDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::AssociatedTypeDecl* D);

};

class TypecheckAssociatedTypeQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckAssociatedTypeQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckAssociatedTypeQuery(QueryContext &QC, ast::AssociatedTypeDecl* D);

   ast::AssociatedTypeDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::AssociatedTypeDecl* D);

};

class PrepareAliasInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareAliasInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareAliasInterfaceQuery(QueryContext &QC, ast::AliasDecl* D);

   ast::AliasDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::AliasDecl* D);

};

class TypecheckAliasQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckAliasQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckAliasQuery(QueryContext &QC, ast::AliasDecl* D);

   ast::AliasDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::AliasDecl* D);

};

class PrepareRecordInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareRecordInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareRecordInterfaceQuery(QueryContext &QC, ast::RecordDecl* D);

   ast::RecordDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* D);

};

class TypecheckRecordQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckRecordQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckRecordQuery(QueryContext &QC, ast::RecordDecl* D);

   ast::RecordDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* D);

};

class PrepareStructInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareStructInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareStructInterfaceQuery(QueryContext &QC, ast::StructDecl* D);

   ast::StructDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::StructDecl* D);

};

class TypecheckStructQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckStructQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckStructQuery(QueryContext &QC, ast::StructDecl* D);

   ast::StructDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::StructDecl* D);

};

class PrepareClassInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareClassInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareClassInterfaceQuery(QueryContext &QC, ast::ClassDecl* D);

   ast::ClassDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::ClassDecl* D);

};

class TypecheckClassQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckClassQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckClassQuery(QueryContext &QC, ast::ClassDecl* D);

   ast::ClassDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::ClassDecl* D);

};

class PrepareEnumInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareEnumInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareEnumInterfaceQuery(QueryContext &QC, ast::EnumDecl* D);

   ast::EnumDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::EnumDecl* D);

};

class TypecheckEnumQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckEnumQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckEnumQuery(QueryContext &QC, ast::EnumDecl* D);

   ast::EnumDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::EnumDecl* D);

};

class PrepareProtocolInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareProtocolInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareProtocolInterfaceQuery(QueryContext &QC, ast::ProtocolDecl* D);

   ast::ProtocolDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::ProtocolDecl* D);

};

class TypecheckProtocolQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckProtocolQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckProtocolQuery(QueryContext &QC, ast::ProtocolDecl* D);

   ast::ProtocolDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::ProtocolDecl* D);

};

class PrepareExtensionInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareExtensionInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareExtensionInterfaceQuery(QueryContext &QC, ast::ExtensionDecl* D);

   ast::ExtensionDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::ExtensionDecl* D);

};

class TypecheckExtensionQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckExtensionQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckExtensionQuery(QueryContext &QC, ast::ExtensionDecl* D);

   ast::ExtensionDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::ExtensionDecl* D);

};

class PrepareFieldInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareFieldInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareFieldInterfaceQuery(QueryContext &QC, ast::FieldDecl* D);

   ast::FieldDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::FieldDecl* D);

};

class TypecheckFieldQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckFieldQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckFieldQuery(QueryContext &QC, ast::FieldDecl* D);

   ast::FieldDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::FieldDecl* D);

};

class PreparePropInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PreparePropInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PreparePropInterfaceQuery(QueryContext &QC, ast::PropDecl* D);

   ast::PropDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::PropDecl* D);

};

class TypecheckPropQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckPropQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckPropQuery(QueryContext &QC, ast::PropDecl* D);

   ast::PropDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::PropDecl* D);

};

class PrepareSubscriptInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareSubscriptInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareSubscriptInterfaceQuery(QueryContext &QC, ast::SubscriptDecl* D);

   ast::SubscriptDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::SubscriptDecl* D);

};

class TypecheckSubscriptQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckSubscriptQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckSubscriptQuery(QueryContext &QC, ast::SubscriptDecl* D);

   ast::SubscriptDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::SubscriptDecl* D);

};

class PrepareEnumCaseInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareEnumCaseInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareEnumCaseInterfaceQuery(QueryContext &QC, ast::EnumCaseDecl* D);

   ast::EnumCaseDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::EnumCaseDecl* D);

};

class TypecheckEnumCaseQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckEnumCaseQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckEnumCaseQuery(QueryContext &QC, ast::EnumCaseDecl* D);

   ast::EnumCaseDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::EnumCaseDecl* D);

};

class PrepareNamespaceInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareNamespaceInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareNamespaceInterfaceQuery(QueryContext &QC, ast::NamespaceDecl* D);

   ast::NamespaceDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::NamespaceDecl* D);

};

class TypecheckNamespaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckNamespaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckNamespaceQuery(QueryContext &QC, ast::NamespaceDecl* D);

   ast::NamespaceDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::NamespaceDecl* D);

};

class PrepareCompoundInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareCompoundInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareCompoundInterfaceQuery(QueryContext &QC, ast::CompoundDecl* D);

   ast::CompoundDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::CompoundDecl* D);

};

class TypecheckCompoundQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckCompoundQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckCompoundQuery(QueryContext &QC, ast::CompoundDecl* D);

   ast::CompoundDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::CompoundDecl* D);

};

class PrepareModuleInterfaceQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareModuleInterfaceQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareModuleInterfaceQuery(QueryContext &QC, ast::ModuleDecl* D);

   ast::ModuleDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::ModuleDecl* D);

};

class TypecheckModuleQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::TypecheckModuleQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   TypecheckModuleQuery(QueryContext &QC, ast::ModuleDecl* D);

   ast::ModuleDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::ModuleDecl* D);

};

class PrepareTemplateParametersQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::PrepareTemplateParametersQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   PrepareTemplateParametersQuery(QueryContext &QC, ast::NamedDecl* Decl);

   ast::NamedDecl* Decl;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::NamedDecl* Decl);

};

class DeclareImplicitInitializersQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::DeclareImplicitInitializersQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   DeclareImplicitInitializersQuery(QueryContext &QC, ast::RecordDecl* R);

   ast::RecordDecl* R;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R);

};

class DeclareImplicitDefaultInitQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::DeclareImplicitDefaultInitQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   DeclareImplicitDefaultInitQuery(QueryContext &QC, ast::StructDecl* S);

   ast::StructDecl* S;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::StructDecl* S);

};

class DeclareMemberwiseInitQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::DeclareMemberwiseInitQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   DeclareMemberwiseInitQuery(QueryContext &QC, ast::StructDecl* S);

   ast::StructDecl* S;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::StructDecl* S);

};

class DeclareImplicitDefaultDeinitQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::DeclareImplicitDefaultDeinitQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   DeclareImplicitDefaultDeinitQuery(QueryContext &QC, ast::RecordDecl* S);

   ast::RecordDecl* S;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* S);

};

class ResolveRawTypeQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveRawTypeQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveRawTypeQuery(QueryContext &QC, ast::EnumDecl* E);

   ast::EnumDecl* E;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::EnumDecl* E);

};

class AssignInitNameQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::AssignInitNameQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   AssignInitNameQuery(QueryContext &QC, ast::InitDecl* Init);

   ast::InitDecl* Init;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::InitDecl* Init);

};

class CreateBaseInitQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CreateBaseInitQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateBaseInitQuery(QueryContext &QC, ast::InitDecl* D);

   ast::InitDecl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::InitDecl* D);

};

class LookupFirstQuery: public Query, public llvm::FoldingSetNode {

    private:
        /// Revision ID of the last lookup.
        int32_t RevisionID = -1;
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::LookupFirstQueryID; }

   using result_type = const SingleLevelLookupResult*;

   std::string description() const;
   std::string summary() const;

   const SingleLevelLookupResult* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   void refresh();

   LookupFirstQuery(QueryContext &QC, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts = DefaultLookupOpts);

   QueryResult finish(SingleLevelLookupResult &&Result, Status St = Done);

   ast::DeclContext* DC;
   DeclarationName Name;
   LookupOpts Opts;
   llvm::Optional<SingleLevelLookupResult> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts);

};

class LookupSingleQuery: public Query, public llvm::FoldingSetNode {

    private:
        /// Revision ID of the last lookup.
        int32_t RevisionID = -1;
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::LookupSingleQueryID; }

   using result_type = ast::NamedDecl*;

   std::string description() const;
   std::string summary() const;

   ast::NamedDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   void refresh();

   LookupSingleQuery(QueryContext &QC, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts = DefaultLookupOpts | LookupOpts::IssueDiag);

   QueryResult finish(ast::NamedDecl* Result, Status St = Done);

   ast::DeclContext* DC;
   DeclarationName Name;
   LookupOpts Opts;
   llvm::Optional<ast::NamedDecl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts);

};

class MultiLevelLookupQuery: public Query, public llvm::FoldingSetNode {

    private:
        /// Revision ID of the last lookup.
        int32_t RevisionID = -1;
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::MultiLevelLookupQueryID; }

   using result_type = const MultiLevelLookupResult*;

   std::string description() const;
   std::string summary() const;

   const MultiLevelLookupResult* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   void refresh();

   MultiLevelLookupQuery(QueryContext &QC, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts = DefaultLookupOpts);

   QueryResult finish(MultiLevelLookupResult &&Result, Status St = Done);

   ast::DeclContext* DC;
   DeclarationName Name;
   LookupOpts Opts;
   llvm::Optional<MultiLevelLookupResult> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts);

};

class MultiLevelTypeLookupQuery: public Query, public llvm::FoldingSetNode {

    private:
        /// Revision ID of the last lookup.
        int32_t RevisionID = -1;
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::MultiLevelTypeLookupQueryID; }

   using result_type = const MultiLevelLookupResult*;

   std::string description() const;
   std::string summary() const;

   const MultiLevelLookupResult* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   void refresh();

   MultiLevelTypeLookupQuery(QueryContext &QC, CanType T, DeclarationName Name, LookupOpts Opts = DefaultLookupOpts);

   QueryResult finish(MultiLevelLookupResult &&Result, Status St = Done);

   CanType T;
   DeclarationName Name;
   LookupOpts Opts;
   llvm::Optional<MultiLevelLookupResult> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, CanType T, DeclarationName Name, LookupOpts Opts);

};

class RestrictedLookupQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::RestrictedLookupQueryID; }

   using result_type = const MultiLevelLookupResult*;

   std::string description() const;
   std::string summary() const;

   const MultiLevelLookupResult* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   RestrictedLookupQuery(QueryContext &QC, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts = DefaultLookupOpts);

   QueryResult finish(MultiLevelLookupResult &&Result, Status St = Done);

   ast::DeclContext* DC;
   DeclarationName Name;
   LookupOpts Opts;
   llvm::Optional<MultiLevelLookupResult> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts);

};

class DirectLookupQuery: public Query, public llvm::FoldingSetNode {

    private:
        /// Revision ID of the last lookup.
        int32_t RevisionID = -1;
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::DirectLookupQueryID; }

   using result_type = const MultiLevelLookupResult*;

   std::string description() const;
   std::string summary() const;

   const MultiLevelLookupResult* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   void refresh();

   DirectLookupQuery(QueryContext &QC, ast::DeclContext* DC, DeclarationName Name, bool LookInExtensions = true, LookupOpts Opts = DefaultLookupOpts, ast::ConstraintSet* Constraints = nullptr);

   QueryResult finish(MultiLevelLookupResult &&Result, Status St = Done);

   ast::DeclContext* DC;
   DeclarationName Name;
   bool LookInExtensions;
   LookupOpts Opts;
   ast::ConstraintSet* Constraints;
   llvm::Optional<MultiLevelLookupResult> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC, DeclarationName Name, bool LookInExtensions, LookupOpts Opts, ast::ConstraintSet* Constraints);

};

class NestedNameLookupQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::NestedNameLookupQueryID; }

   using result_type = const MultiLevelLookupResult*;

   std::string description() const;
   std::string summary() const;

   const MultiLevelLookupResult* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   NestedNameLookupQuery(QueryContext &QC, ast::DeclContext* DC, llvm::ArrayRef<DeclarationName> Names, LookupOpts Opts = DefaultLookupOpts | LookupOpts::IssueDiag, SourceRange Loc = SourceRange());

   QueryResult finish(MultiLevelLookupResult &&Result, Status St = Done);

   ast::DeclContext* DC;
   llvm::ArrayRef<DeclarationName> Names;
   LookupOpts Opts;
   SourceRange Loc;
   llvm::Optional<MultiLevelLookupResult> Result;
};

class ResolveMacrosQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveMacrosQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveMacrosQuery(QueryContext &QC, ast::DeclContext* DC);

   ast::DeclContext* DC;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC);

};

class ResolveStaticDeclarationsQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveStaticDeclarationsQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveStaticDeclarationsQuery(QueryContext &QC, ast::DeclContext* DC);

   ast::DeclContext* DC;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC);

};

class ResolveWildcardDeclarationsQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveWildcardDeclarationsQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveWildcardDeclarationsQuery(QueryContext &QC, ast::DeclContext* DC);

   ast::DeclContext* DC;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC);

};

class ResolveMetaDeclarationsQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::ResolveMetaDeclarationsQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   ResolveMetaDeclarationsQuery(QueryContext &QC, ast::DeclContext* DC);

   ast::DeclContext* DC;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC);

};

class GetAssociatedTypeImplQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetAssociatedTypeImplQueryID; }

   using result_type = ast::AliasDecl*;

   std::string description() const;
   std::string summary() const;

   ast::AliasDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetAssociatedTypeImplQuery(QueryContext &QC, ast::RecordDecl* R, DeclarationName Name, llvm::ArrayRef<ast::ExtensionDecl*> Extensions, ast::ConstraintSet* Constraints = nullptr);

   QueryResult finish(ast::AliasDecl* Result, Status St = Done);

   ast::RecordDecl* R;
   DeclarationName Name;
   llvm::ArrayRef<ast::ExtensionDecl*> Extensions;
   ast::ConstraintSet* Constraints;
   llvm::Optional<ast::AliasDecl*> Result;
};

class GetAssociatedTypeDeclQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetAssociatedTypeDeclQueryID; }

   using result_type = ast::AssociatedTypeDecl*;

   std::string description() const;
   std::string summary() const;

   ast::AssociatedTypeDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetAssociatedTypeDeclQuery(QueryContext &QC, ast::ProtocolDecl* P, DeclarationName Name, ast::ConstraintSet* Constraints = nullptr);

   QueryResult finish(ast::AssociatedTypeDecl* Result, Status St = Done);

   ast::ProtocolDecl* P;
   DeclarationName Name;
   ast::ConstraintSet* Constraints;
   llvm::Optional<ast::AssociatedTypeDecl*> Result;
};

class CheckTypeEquivalenceQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CheckTypeEquivalenceQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckTypeEquivalenceQuery(QueryContext &QC, QualType LHS, QualType RHS, QualType Self, ast::DeclContext* LHSDecl = nullptr, ast::DeclContext* DeclCtx = nullptr);

   QueryResult finish(bool Result, Status St = Done);

   QualType LHS;
   QualType RHS;
   QualType Self;
   ast::DeclContext* LHSDecl;
   ast::DeclContext* DeclCtx;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType LHS, QualType RHS, QualType Self, ast::DeclContext* LHSDecl, ast::DeclContext* DeclCtx);

};

class CheckTemplateExtensionApplicabilityQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CheckTemplateExtensionApplicabilityQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CheckTemplateExtensionApplicabilityQuery(QueryContext &QC, ast::RecordDecl* Inst, ast::ExtensionDecl* Ext);

   QueryResult finish(bool Result, Status St = Done);

   ast::RecordDecl* Inst;
   ast::ExtensionDecl* Ext;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* Inst, ast::ExtensionDecl* Ext);

};

class GetILModuleForDeclQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetILModuleForDeclQueryID; }

   using result_type = il::Module*;

   std::string description() const;
   std::string summary() const;

   il::Module* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetILModuleForDeclQuery(QueryContext &QC, ast::Decl* D);

   QueryResult finish(il::Module* Result, Status St = Done);

   ast::Decl* D;
   llvm::Optional<il::Module*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::Decl* D);

};

class GenerateILForDeclQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GenerateILForDeclQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GenerateILForDeclQuery(QueryContext &QC, ast::Decl* D);

   ast::Decl* D;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::Decl* D);

};

class GenerateILForContextQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GenerateILForContextQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GenerateILForContextQuery(QueryContext &QC, ast::DeclContext* DC);

   ast::DeclContext* DC;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC);

};

class GenerateRecordILQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GenerateRecordILQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GenerateRecordILQuery(QueryContext &QC, ast::RecordDecl* R);

   ast::RecordDecl* R;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R);

};

class GetILGlobalQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetILGlobalQueryID; }

   using result_type = il::GlobalVariable*;

   std::string description() const;
   std::string summary() const;

   il::GlobalVariable* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetILGlobalQuery(QueryContext &QC, ast::VarDecl* GV);

   QueryResult finish(il::GlobalVariable* Result, Status St = Done);

   ast::VarDecl* GV;
   llvm::Optional<il::GlobalVariable*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::VarDecl* GV);

};

class GenerateLazyILGlobalDefinitionQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GenerateLazyILGlobalDefinitionQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GenerateLazyILGlobalDefinitionQuery(QueryContext &QC, ast::VarDecl* GV);

   ast::VarDecl* GV;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::VarDecl* GV);

};

class GetILFunctionQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetILFunctionQueryID; }

   using result_type = il::Function*;

   std::string description() const;
   std::string summary() const;

   il::Function* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetILFunctionQuery(QueryContext &QC, ast::CallableDecl* C);

   QueryResult finish(il::Function* Result, Status St = Done);

   ast::CallableDecl* C;
   llvm::Optional<il::Function*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::CallableDecl* C);

};

class GenerateILFunctionBodyQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GenerateILFunctionBodyQueryID; }

   using result_type = void;

   std::string description() const;
   std::string summary() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GenerateILFunctionBodyQuery(QueryContext &QC, ast::CallableDecl* C);

   ast::CallableDecl* C;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::CallableDecl* C);

};

class GetILTypeInfoQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetILTypeInfoQueryID; }

   using result_type = il::GlobalVariable*;

   std::string description() const;
   std::string summary() const;

   il::GlobalVariable* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetILTypeInfoQuery(QueryContext &QC, QualType T);

   QueryResult finish(il::GlobalVariable* Result, Status St = Done);

   QualType T;
   llvm::Optional<il::GlobalVariable*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T);

};

class CreateILBasicTypeInfoQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CreateILBasicTypeInfoQueryID; }

   using result_type = il::Constant*;

   std::string description() const;
   std::string summary() const;

   il::Constant* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateILBasicTypeInfoQuery(QueryContext &QC, QualType T);

   QueryResult finish(il::Constant* Result, Status St = Done);

   QualType T;
   llvm::Optional<il::Constant*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, QualType T);

};

class CreateILRecordTypeInfoQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CreateILRecordTypeInfoQueryID; }

   using result_type = il::Constant*;

   std::string description() const;
   std::string summary() const;

   il::Constant* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateILRecordTypeInfoQuery(QueryContext &QC, ast::RecordDecl* R);

   QueryResult finish(il::Constant* Result, Status St = Done);

   ast::RecordDecl* R;
   llvm::Optional<il::Constant*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R);

};

class GetBoolValueQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetBoolValueQueryID; }

   using result_type = bool;

   std::string description() const;
   std::string summary() const;

   bool get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetBoolValueQuery(QueryContext &QC, il::Constant* C, uint8_t AllowWrapperTypes = true);

   QueryResult finish(bool Result, Status St = Done);

   il::Constant* C;
   uint8_t AllowWrapperTypes;
   llvm::Optional<bool> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, il::Constant* C, uint8_t AllowWrapperTypes);

};

class GetStringValueQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetStringValueQueryID; }

   using result_type = llvm::StringRef;

   std::string description() const;
   std::string summary() const;

   llvm::StringRef get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetStringValueQuery(QueryContext &QC, il::Constant* C, uint8_t AllowWrapperTypes = true);

   QueryResult finish(llvm::StringRef Result, Status St = Done);

   il::Constant* C;
   uint8_t AllowWrapperTypes;
   llvm::Optional<llvm::StringRef> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, il::Constant* C, uint8_t AllowWrapperTypes);

};

class GetIntValueQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetIntValueQueryID; }

   using result_type = llvm::APSInt;

   std::string description() const;
   std::string summary() const;

   llvm::APSInt get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetIntValueQuery(QueryContext &QC, il::Constant* C, uint8_t AllowWrapperTypes = true);

   QueryResult finish(llvm::APSInt &&Result, Status St = Done);

   il::Constant* C;
   uint8_t AllowWrapperTypes;
   llvm::Optional<llvm::APSInt> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, il::Constant* C, uint8_t AllowWrapperTypes);

};

class OpenFileForReadQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::OpenFileForReadQueryID; }

   using result_type = llvm::MemoryBuffer*;

   std::string description() const;
   std::string summary() const;

   llvm::MemoryBuffer* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   OpenFileForReadQuery(QueryContext &QC, StringRef FileName);

   QueryResult finish(std::unique_ptr<llvm::MemoryBuffer> &&Result, Status St = Done);

   StringRef FileName;
   llvm::Optional<std::unique_ptr<llvm::MemoryBuffer>> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, StringRef FileName);

};

class OpenTmpFileQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::OpenTmpFileQueryID; }

   using result_type = llvm::raw_ostream*;

   std::string description() const;
   std::string summary() const;

   llvm::raw_ostream* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   OpenTmpFileQuery(QueryContext &QC, StringRef Extension);

   QueryResult finish(std::unique_ptr<llvm::raw_ostream> &&Result, Status St = Done);

   StringRef Extension;
   llvm::Optional<std::unique_ptr<llvm::raw_ostream>> Result;
};

class GetDefaultTemplateArgQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::GetDefaultTemplateArgQueryID; }

   using result_type = const sema::TemplateArgument*;

   std::string description() const;
   std::string summary() const;

   const sema::TemplateArgument* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   GetDefaultTemplateArgQuery(QueryContext &QC, ast::TemplateParamDecl* P);

   QueryResult finish(sema::TemplateArgument &&Result, Status St = Done);

   ast::TemplateParamDecl* P;
   llvm::Optional<sema::TemplateArgument> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::TemplateParamDecl* P);

};

class CreateSelfArgumentQuery: public Query {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::CreateSelfArgumentQueryID; }

   using result_type = ast::FuncArgDecl*;

   std::string description() const;
   std::string summary() const;

   ast::FuncArgDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   CreateSelfArgumentQuery(QueryContext &QC, QualType Self, SourceLocation SelfLoc);

   QueryResult finish(ast::FuncArgDecl* Result, Status St = Done);

   QualType Self;
   SourceLocation SelfLoc;
   llvm::Optional<ast::FuncArgDecl*> Result;
};

class FindEquivalentDeclQuery: public Query, public llvm::FoldingSetNode {
public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::FindEquivalentDeclQueryID; }

   using result_type = ast::NamedDecl*;

   std::string description() const;
   std::string summary() const;

   ast::NamedDecl* get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   FindEquivalentDeclQuery(QueryContext &QC, ast::NamedDecl* Decl, ast::DeclContext* DC, QualType Self = QualType(), bool LookInExtensions = true);

   QueryResult finish(ast::NamedDecl* Result, Status St = Done);

   ast::NamedDecl* Decl;
   ast::DeclContext* DC;
   QualType Self;
   bool LookInExtensions;
   llvm::Optional<ast::NamedDecl*> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::NamedDecl* Decl, ast::DeclContext* DC, QualType Self, bool LookInExtensions);

};

class EquivalentSignaturesQuery: public Query, public llvm::FoldingSetNode {

    public:
        enum ResultKind {
            /// The signatures are equivalent.
            Equivalent,

            /// There is a return type mismatch.
            ReturnTypeMismatch,

            /// Parameter count mismatch.
            ParamCountMismatch,

            /// There is a parameter type mismatch.
            ParamTypeMismatch,

            /// There is a label mismatch.
            LabelMismatch,

            /// 'throws' does not match.
            ThrowsMismatch,

            /// 'unsafe' does not match.
            UnsafeMismatch,

            /// 'async' does not match.
            AsyncMismatch,

            /// 'static' does not match.
            StaticMismatch
        };
    public:
   static bool classof(const Query *Q) { return classofKind(Q->kind()); }
   static bool classofKind(Kind K) { return K == Kind::EquivalentSignaturesQueryID; }

   using result_type = EquivalentSignaturesQuery::ResultKind;

   std::string description() const;
   std::string summary() const;

   EquivalentSignaturesQuery::ResultKind get() const;

   friend class Query; // for run().
   friend class QueryContext; // for C'tor.

private:
   QueryResult run();

   EquivalentSignaturesQuery(QueryContext &QC, ast::CallableDecl* C1, ast::CallableDecl* C2, QualType Self);

   QueryResult finish(EquivalentSignaturesQuery::ResultKind Result, Status St = Done);

   ast::CallableDecl* C1;
   ast::CallableDecl* C2;
   QualType Self;
   llvm::Optional<EquivalentSignaturesQuery::ResultKind> Result;
public:
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID, ast::CallableDecl* C1, ast::CallableDecl* C2, QualType Self);

};

