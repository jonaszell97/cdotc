
/// \brief Single query instance.
private:   CompileModuleQuery *CompileModuleInstance = nullptr;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<StringRef, ParseModuleFileQuery*> ParseModuleFileQueries;

/// \brief Single query instance.
private:   CreateDefaultModuleQuery *CreateDefaultModuleInstance = nullptr;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<ParseSourceFileQuery> ParseSourceFileQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<ParseMainSourceFileQuery> ParseMainSourceFileQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<Module*, ParseSourceFilesQuery*> ParseSourceFilesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<Module*, CreateILModuleQuery*> CreateILModuleQueries;

/// \brief Single query instance.
private:   SetupIRGenQuery *SetupIRGenInstance = nullptr;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<Module*, CreateLLVMModuleQuery*> CreateLLVMModuleQueries;








/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<Query*, MeasureExecutionTimeQuery*> MeasureExecutionTimeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<CanType, IsPersistableQuery*> IsPersistableQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<CanType, IsEquatableQuery*> IsEquatableQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<CanType, IsCopyableQuery*> IsCopyableQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<CanType, IsMoveOnlyQuery*> IsMoveOnlyQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<CanType, IsUnpopulatedQuery*> IsUnpopulatedQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<CanType, IsImplicitlyCopyableQuery*> IsImplicitlyCopyableQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<CanType, IsTriviallyCopyableQuery*> IsTriviallyCopyableQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<CanType, NeedsRetainOrReleaseQuery*> NeedsRetainOrReleaseQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<CanType, NeedsDeinitilizationQuery*> NeedsDeinitilizationQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<CanType, NeedsStructReturnQuery*> NeedsStructReturnQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<CanType, ContainsAssociatedTypeConstraintQuery*> ContainsAssociatedTypeConstraintQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<std::pair<QualType, QualType>, bool> IsImplicitlyConvertibleQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<std::pair<QualType, std::pair<QualType, uint8_t>>, bool> IsValidParameterValueQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<std::pair<QualType, QualType>, ConversionSequence*> GetConversionSequenceQueries;


/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<SubstAssociatedTypesQuery> SubstAssociatedTypesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<SubstTemplateParamTypesQuery> SubstTemplateParamTypesQueries;


/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::NamedDecl*, std::vector<TypeCapability>> CheckTypeCapabilitiesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<std::pair<QualType, ast::DeclContext*>, QualType> ApplyCapabilitesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<char, GetBuiltinModuleQuery*> GetBuiltinModuleQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<char, GetBuiltinFuncQuery*> GetBuiltinFuncQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<GetBuiltinAliasQuery> GetBuiltinAliasQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<uint32_t, GetBuiltinRecordQuery*> GetBuiltinRecordQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<char, GetBuiltinProtocolQuery*> GetBuiltinProtocolQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<FindPrecedenceGroupQuery> FindPrecedenceGroupQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<FindOperatorQuery> FindOperatorQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<ResolveImportQuery> ResolveImportQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<ResolveUsingQuery> ResolveUsingQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::StaticIfDecl*, ResolveStaticIfQuery*> ResolveStaticIfQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::StaticForDecl*, ResolveStaticForQuery*> ResolveStaticForQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::StaticAssertDecl*, ResolveStaticAssertQuery*> ResolveStaticAssertQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::StaticPrintDecl*, ResolveStaticPrintQuery*> ResolveStaticPrintQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::StaticExpr*, ResolveStaticExprQuery*> ResolveStaticExprQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<ResolveStaticExprToBoolQuery> ResolveStaticExprToBoolQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<ExpandMacroQuery> ExpandMacroQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::MacroExpansionDecl*, ExpandMacroDeclQuery*> ExpandMacroDeclQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<StringRef, ExpandMacrosQuery*> ExpandMacrosQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<ImportExternalSourceFileQuery> ImportExternalSourceFileQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<QualType, FindExtensionsQuery*> FindExtensionsQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::SourceType, GetExtensionTypeKindQuery*> GetExtensionTypeKindQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<MatchExtensionTypeQuery> MatchExtensionTypeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<ExtensionAppliesQuery> ExtensionAppliesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::SourceType, GetExtendedDeclQuery*> GetExtendedDeclQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::ExtensionDecl*, ResolveExtensionQuery*> ResolveExtensionQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<ResolveConformanceToProtocolQuery> ResolveConformanceToProtocolQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<QualType, ResolveExplicitConformancesQuery*> ResolveExplicitConformancesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<ResolveDeclaredConformancesQuery> ResolveDeclaredConformancesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<AddSingleConformanceQuery> AddSingleConformanceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseSet<ast::RecordDecl*> DeclareSelfAliasQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<ResolveAssociatedTypesQuery> ResolveAssociatedTypesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<GetReferencedAssociatedTypesQuery> GetReferencedAssociatedTypesQueries;


/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<CheckProtocolExtensionApplicabilityQuery> CheckProtocolExtensionApplicabilityQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<VerifyConstraintQuery> VerifyConstraintQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<IsConstraintSatisfiedQuery> IsConstraintSatisfiedQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<GetConstrainedTypeQuery> GetConstrainedTypeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::RecordDecl*, GetNeededAssociatedTypesQuery*> GetNeededAssociatedTypesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<QualType, CheckConformancesQuery*> CheckConformancesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<CheckSingleConformanceQuery> CheckSingleConformanceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::RecordDecl*, CheckAssociatedTypeConstraintsQuery*> CheckAssociatedTypeConstraintsQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::NamedDecl*, TypecheckConstraintsQuery*> TypecheckConstraintsQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<ConformsToQuery> ConformsToQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<IsCovariantQuery> IsCovariantQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<IsContravariantQuery> IsContravariantQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<CanType, GetTypeSizeQuery*> GetTypeSizeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<CanType, GetTypeAlignmentQuery*> GetTypeAlignmentQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<CanType, GetTypeStrideQuery*> GetTypeStrideQueries;

    private:
        
        llvm::DenseMap<ast::RecordDecl*, RecordMetaInfo> RecordMeta;
    
/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::RecordDecl*, CalculateRecordSizeQuery*> CalculateRecordSizeQueries;


/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::RecordDecl*, bool> IsBuiltinIntegerTypeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::RecordDecl*, bool> IsBuiltinFloatingPointTypeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::RecordDecl*, bool> IsBuiltinBoolTypeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<std::pair<ast::RecordDecl*, std::pair<ast::ImplicitConformanceKind, ast::MethodDecl*>>, ast::MethodDecl*> AddImplicitConformanceQueries;


/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::RecordDecl*, CheckBuiltinConformancesQuery*> CheckBuiltinConformancesQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<IsAccessibleQuery> IsAccessibleQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<CheckAccessibilityQuery> CheckAccessibilityQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<ResolveNestedNameSpecToTypeQuery> ResolveNestedNameSpecToTypeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<ResolveNestedNameSpecToDeclQuery> ResolveNestedNameSpecToDeclQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::DeclContext*, TypeCheckDeclContextQuery*> TypeCheckDeclContextQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseSet<ast::Decl*> PrepareDeclInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseSet<ast::Decl*> TypecheckDeclQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::PrecedenceGroupDecl*, PreparePrecedenceGroupInterfaceQuery*> PreparePrecedenceGroupInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::PrecedenceGroupDecl*, TypecheckPrecedenceGroupQuery*> TypecheckPrecedenceGroupQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::OperatorDecl*, PrepareOperatorInterfaceQuery*> PrepareOperatorInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::OperatorDecl*, TypecheckOperatorQuery*> TypecheckOperatorQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::GlobalVarDecl*, PrepareGlobalVarInterfaceQuery*> PrepareGlobalVarInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::GlobalVarDecl*, TypecheckGlobalVarQuery*> TypecheckGlobalVarQueries;

            private:
                
                llvm::DenseMap<IdentifierInfo*, ast::CallableDecl*> ExternCFuncs;
            
/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::CallableDecl*, PrepareCallableInterfaceQuery*> PrepareCallableInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::CallableDecl*, TypecheckCallableQuery*> TypecheckCallableQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::FunctionDecl*, PrepareFunctionInterfaceQuery*> PrepareFunctionInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::FunctionDecl*, TypecheckFunctionQuery*> TypecheckFunctionQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::MethodDecl*, PrepareMethodInterfaceQuery*> PrepareMethodInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::MethodDecl*, TypecheckMethodQuery*> TypecheckMethodQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::InitDecl*, PrepareInitInterfaceQuery*> PrepareInitInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::InitDecl*, TypecheckInitQuery*> TypecheckInitQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::DeinitDecl*, PrepareDeinitInterfaceQuery*> PrepareDeinitInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::DeinitDecl*, TypecheckDeinitQuery*> TypecheckDeinitQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::FuncArgDecl*, PrepareFuncArgInterfaceQuery*> PrepareFuncArgInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::FuncArgDecl*, TypecheckFuncArgQuery*> TypecheckFuncArgQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::TemplateParamDecl*, PrepareTemplateParamInterfaceQuery*> PrepareTemplateParamInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::TemplateParamDecl*, TypecheckTemplateParamQuery*> TypecheckTemplateParamQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::AssociatedTypeDecl*, PrepareAssociatedTypeInterfaceQuery*> PrepareAssociatedTypeInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::AssociatedTypeDecl*, TypecheckAssociatedTypeQuery*> TypecheckAssociatedTypeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::AliasDecl*, PrepareAliasInterfaceQuery*> PrepareAliasInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::AliasDecl*, TypecheckAliasQuery*> TypecheckAliasQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::RecordDecl*, PrepareRecordInterfaceQuery*> PrepareRecordInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::RecordDecl*, TypecheckRecordQuery*> TypecheckRecordQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::StructDecl*, PrepareStructInterfaceQuery*> PrepareStructInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::StructDecl*, TypecheckStructQuery*> TypecheckStructQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::ClassDecl*, PrepareClassInterfaceQuery*> PrepareClassInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::ClassDecl*, TypecheckClassQuery*> TypecheckClassQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::EnumDecl*, PrepareEnumInterfaceQuery*> PrepareEnumInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::EnumDecl*, TypecheckEnumQuery*> TypecheckEnumQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::ProtocolDecl*, PrepareProtocolInterfaceQuery*> PrepareProtocolInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::ProtocolDecl*, TypecheckProtocolQuery*> TypecheckProtocolQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::ExtensionDecl*, PrepareExtensionInterfaceQuery*> PrepareExtensionInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::ExtensionDecl*, TypecheckExtensionQuery*> TypecheckExtensionQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::FieldDecl*, PrepareFieldInterfaceQuery*> PrepareFieldInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::FieldDecl*, TypecheckFieldQuery*> TypecheckFieldQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::PropDecl*, PreparePropInterfaceQuery*> PreparePropInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::PropDecl*, TypecheckPropQuery*> TypecheckPropQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::SubscriptDecl*, PrepareSubscriptInterfaceQuery*> PrepareSubscriptInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::SubscriptDecl*, TypecheckSubscriptQuery*> TypecheckSubscriptQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::EnumCaseDecl*, PrepareEnumCaseInterfaceQuery*> PrepareEnumCaseInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::EnumCaseDecl*, TypecheckEnumCaseQuery*> TypecheckEnumCaseQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::NamespaceDecl*, PrepareNamespaceInterfaceQuery*> PrepareNamespaceInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::NamespaceDecl*, TypecheckNamespaceQuery*> TypecheckNamespaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::CompoundDecl*, PrepareCompoundInterfaceQuery*> PrepareCompoundInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::CompoundDecl*, TypecheckCompoundQuery*> TypecheckCompoundQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::ModuleDecl*, PrepareModuleInterfaceQuery*> PrepareModuleInterfaceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::ModuleDecl*, TypecheckModuleQuery*> TypecheckModuleQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::NamedDecl*, PrepareTemplateParametersQuery*> PrepareTemplateParametersQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::RecordDecl*, DeclareImplicitInitializersQuery*> DeclareImplicitInitializersQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::StructDecl*, DeclareImplicitDefaultInitQuery*> DeclareImplicitDefaultInitQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::StructDecl*, DeclareMemberwiseInitQuery*> DeclareMemberwiseInitQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::RecordDecl*, DeclareImplicitDefaultDeinitQuery*> DeclareImplicitDefaultDeinitQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::EnumDecl*, ResolveRawTypeQuery*> ResolveRawTypeQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::InitDecl*, AssignInitNameQuery*> AssignInitNameQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseSet<ast::InitDecl*> CreateBaseInitQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<LookupFirstQuery> LookupFirstQueries;

    public:
        template<class T>
        T *LookupSingleAs(ast::DeclContext *DC, DeclarationName Name,
                          LookupOpts Opts = DefaultLookupOpts) {
            ast::NamedDecl *Result;
            if (LookupSingle(Result, DC, Name, Opts)) {
                return nullptr;
            }

            return support::dyn_cast_or_null<T>(Result);
        }
    
/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<LookupSingleQuery> LookupSingleQueries;



/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<DirectLookupQuery> DirectLookupQueries;


/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseSet<ast::DeclContext*> ResolveMacrosQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseSet<ast::DeclContext*> ResolveStaticDeclarationsQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseSet<ast::DeclContext*> ResolveWildcardDeclarationsQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::DeclContext*, PrepareTypeNameLookupQuery*> PrepareTypeNameLookupQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::DeclContext*, PrepareNameLookupQuery*> PrepareNameLookupQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::StructDecl*, InstantiateFieldsQuery*> InstantiateFieldsQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::EnumDecl*, InstantiateCasesQuery*> InstantiateCasesQueries;


/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<CheckTypeEquivalenceQuery> CheckTypeEquivalenceQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<InstantiateProtocolDefaultImplQuery> InstantiateProtocolDefaultImplQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<InstantiateRecordQuery> InstantiateRecordQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<InstantiateAliasQuery> InstantiateAliasQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<InstantiateCallableQuery> InstantiateCallableQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<InstantiateFunctionQuery> InstantiateFunctionQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<InstantiateMethodQuery> InstantiateMethodQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::CallableDecl*, InstantiateMethodBodyQuery*> InstantiateMethodBodyQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<InstantiateDeclQuery> InstantiateDeclQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<CheckTemplateExtensionApplicabilityQuery> CheckTemplateExtensionApplicabilityQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::NamedDecl*, PrepareDeclForInstantiationQuery*> PrepareDeclForInstantiationQueries;

    public:
        template<class T>
        T *InstantiateTemplateMember(T *Decl, ast::RecordDecl *Inst)
        {
            if (Decl->getDeclContext()->lookThroughExtension() == Inst)
                return Decl;

            ast::NamedDecl *DeclInst;
            if (InstantiateTemplateMember(DeclInst, Decl, Inst)) {
                return Decl;
            }

            return support::cast<T>(DeclInst);
        }
    
/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<InstantiateTemplateMemberQuery> InstantiateTemplateMemberQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::RecordDecl*, PrepareInstantiationForNameLookupQuery*> PrepareInstantiationForNameLookupQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::Decl*, GetILModuleForDeclQuery*> GetILModuleForDeclQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::DeclContext*, GenerateILForContextQuery*> GenerateILForContextQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::RecordDecl*, GenerateRecordILQuery*> GenerateRecordILQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::VarDecl*, GetILGlobalQuery*> GetILGlobalQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::VarDecl*, GenerateLazyILGlobalDefinitionQuery*> GenerateLazyILGlobalDefinitionQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::CallableDecl*, GetILFunctionQuery*> GetILFunctionQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::CallableDecl*, GenerateILFunctionBodyQuery*> GenerateILFunctionBodyQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<QualType, GetILTypeInfoQuery*> GetILTypeInfoQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<QualType, CreateILBasicTypeInfoQuery*> CreateILBasicTypeInfoQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::RecordDecl*, CreateILRecordTypeInfoQuery*> CreateILRecordTypeInfoQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<std::pair<il::Constant*, uint8_t>, bool> GetBoolValueQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<std::pair<il::Constant*, uint8_t>, llvm::StringRef> GetStringValueQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<std::pair<il::Constant*, uint8_t>, llvm::APSInt> GetIntValueQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<StringRef, OpenFileForReadQuery*> OpenFileForReadQueries;


/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::DenseMap<ast::TemplateParamDecl*, GetDefaultTemplateArgQuery*> GetDefaultTemplateArgQueries;


/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<FindEquivalentDeclQuery> FindEquivalentDeclQueries;

/// \brief Maps from query kinds to a folding set containing instances of
/// that query.
private:   llvm::FoldingSet<EquivalentSignaturesQuery> EquivalentSignaturesQueries;
