public: QueryResult CompileModule();
public: QueryResult ParseModuleFile(Module* &Result, StringRef FileName);
public: QueryResult CreateDefaultModule(Module* &Result);
public: QueryResult ParseSourceFile(ast::SourceFileDecl* &Result, Module* Mod, StringRef FileName);
public: QueryResult ParseMainSourceFile(ast::SourceFileDecl* &Result, Module* Mod, StringRef FileName);
public: QueryResult ParseSourceFiles(Module* Mod);
public: QueryResult CreateILModule(il::Module* &Result, Module* Mod);
public: QueryResult SetupIRGen(il::IRGen* &Result);
public: QueryResult CreateLLVMModule(llvm::Module* &Result, Module* Mod);
public: QueryResult CreateObject(llvm::raw_ostream& OS);
public: QueryResult CreateStaticLib(StringRef OutFile);
public: QueryResult CreateDynamicLib(StringRef OutFile);
public: QueryResult CreateExecutable(StringRef OutFile);
public: QueryResult EmitIL(llvm::raw_ostream& OS);
public: QueryResult EmitIR(llvm::raw_ostream& OS);
public: QueryResult PrintUsedMemory();
public: QueryResult MeasureExecutionTime(long long &Result, Query* Q);
public: QueryResult IsPersistable(bool &Result, CanType T);
public: QueryResult IsEquatable(bool &Result, CanType T);
public: QueryResult IsCopyable(bool &Result, CanType T);
public: QueryResult IsMoveOnly(bool &Result, CanType T);
public: QueryResult IsUnpopulated(bool &Result, CanType T);
public: QueryResult IsImplicitlyCopyable(bool &Result, CanType T);
public: QueryResult IsTriviallyCopyable(bool &Result, CanType T);
public: QueryResult NeedsRetainOrRelease(bool &Result, CanType T);
public: QueryResult NeedsDeinitilization(bool &Result, CanType T);
public: QueryResult NeedsStructReturn(bool &Result, CanType T);
public: QueryResult ContainsAssociatedTypeConstraint(bool &Result, CanType T);
public: QueryResult IsImplicitlyConvertible(bool &Result, QualType From, QualType To);
public: QueryResult GetConversionSequence(ConversionSequence* &Result, QualType From, QualType To);
public: QueryResult SubstTypeVariables(QualType &Result, QualType T, const llvm::DenseMap<TypeVariableType*, QualType>& ReplacementMap, SourceRange SR);
public: QueryResult SubstAssociatedTypes(QualType &Result, QualType T, QualType Self, SourceRange SR);
public: QueryResult SubstGenericTypes(QualType &Result, QualType T, sema::MultiLevelFinalTemplateArgList TemplateArgs, SourceRange SR);
public: QueryResult SubstGenericTypesNonFinal(QualType &Result, QualType T, const sema::MultiLevelTemplateArgList& TemplateArgs, SourceRange SR);
public: QueryResult CheckTypeCapabilities(ArrayRef<TypeCapability> &Result, ast::NamedDecl* ND);
public: QueryResult ApplyCapabilites(QualType &Result, QualType T, ast::DeclContext* DC);
public: QueryResult GetBuiltinModule(Module* &Result, GetBuiltinModuleQuery::ModuleKind Mod);
public: QueryResult GetBuiltinFunc(ast::CallableDecl* &Result, GetBuiltinFuncQuery::FunctionKind Fn);
public: QueryResult GetBuiltinAlias(ast::AliasDecl* &Result, GetBuiltinAliasQuery::AliasKind K, ast::DeclContext* DC = nullptr);
public: QueryResult GetBuiltinRecord(ast::RecordDecl* &Result, GetBuiltinRecordQuery::RecordKind R);
public: QueryResult GetBuiltinProtocol(ast::ProtocolDecl* &Result, GetBuiltinProtocolQuery::ProtocolKind P);
public: QueryResult FindPrecedenceGroup(ast::PrecedenceGroupDecl* &Result, DeclarationName Name, bool DiagOnError = true, SourceRange Loc = SourceRange());
public: QueryResult FindOperator(ast::OperatorDecl* &Result, DeclarationName Name, bool DiagOnError = true, SourceRange Loc = SourceRange());
public: QueryResult ResolveImport(ast::ImportDecl* I, llvm::SmallVectorImpl<ast::NamedDecl*>* FoundDecls = nullptr);
public: QueryResult ResolveUsing(ast::UsingDecl* U, llvm::SmallVectorImpl<ast::NamedDecl*>* FoundDecls = nullptr);
public: QueryResult ResolveStaticIf(ast::Decl* &Result, ast::StaticIfDecl* Decl);
public: QueryResult ResolveStaticFor(ast::Decl* &Result, ast::StaticForDecl* Decl);
public: QueryResult ResolveStaticAssert(ast::StaticAssertDecl* Decl);
public: QueryResult ResolveStaticPrint(ast::StaticPrintDecl* Decl);
public: QueryResult ResolveStaticExpr(il::Constant* &Result, ast::StaticExpr* Expr);
public: QueryResult ResolveStaticExprToBool(bool &Result, ast::StaticExpr* Expr, bool DiagOnError);
public: QueryResult ExpandMacro(StmtOrDecl &Result, StmtOrDecl SOD, DeclarationName Name, ast::DeclContext* DC, unsigned Delim, llvm::ArrayRef<lex::Token> Tokens, unsigned ExpectedKind);
public: QueryResult ExpandMacroDecl(ast::Decl* &Result, ast::MacroExpansionDecl* Decl);
public: QueryResult ExpandMacros(StringRef SourceFile);
public: QueryResult ImportExternalSourceFile(StringRef FileName, ImportExternalSourceFileQuery::HeaderKind Kind, ast::DeclContext* DC, SourceLocation Loc, bool Optional = false);
public: QueryResult FindExtensions(QualType T);
public: QueryResult GetExtensionTypeKind(GetExtensionTypeKindQuery::ResultKind &Result, ast::SourceType T);
public: QueryResult MatchExtensionType(MatchExtensionTypeQuery::ResultKind &Result, QualType T, ast::SourceType PatternTy);
public: QueryResult ExtensionApplies(MatchExtensionTypeQuery::ResultKind &Result, ast::ExtensionDecl* Ext, QualType T);
public: QueryResult GetExtendedDecl(ast::NamedDecl* &Result, ast::SourceType ExtendedTy);
public: QueryResult ResolveExtension(ast::ExtensionDecl* Ext);
public: QueryResult ResolveConformanceToProtocol(ast::ProtocolDecl* &Result, QualType T, const ast::SourceType& Conf);
public: QueryResult ResolveExplicitConformances(QualType T);
public: QueryResult ResolveDeclaredConformances(QualType T, llvm::ArrayRef<ast::SourceType> Conformances, ConformanceKind Kind);
public: QueryResult AddSingleConformance(QualType T, ast::ProtocolDecl* Proto, ConformanceKind Kind, bool AddRecursiveConformances = true, llvm::function_ref<void(ast::ProtocolDecl*)> Callback = {});
public: QueryResult DeclareSelfAlias(ast::RecordDecl* R);
public: QueryResult ResolveAssociatedTypes(QualType T, bool FinishImmediately = false);
public: QueryResult GetReferencedAssociatedTypes(ArrayRef<ast::AssociatedTypeDecl*> &Result, ast::NamedDecl* ConstrainedDecl, ast::DeclConstraint* C);
public: QueryResult ReferencedAssociatedTypesReady(ReferencedAssociatedTypesReadyQuery::ResultKind &Result, QualType T, ast::NamedDecl* ConstrainedDecl);
public: QueryResult CheckProtocolExtensionApplicability(CheckProtocolExtensionApplicabilityQuery::ResultKind &Result, QualType T, ast::ExtensionDecl* Ext);
public: QueryResult VerifyConstraint(ast::DeclConstraint* C, ast::NamedDecl* ConstrainedDecl);
public: QueryResult IsConstraintSatisfied(bool &Result, ast::DeclConstraint* C, QualType Self, ast::NamedDecl* OriginalDecl, bool PrepareNameLookup = false);
public: QueryResult GetConstrainedType(QualType &Result, ast::DeclConstraint* C, QualType Self, ast::NamedDecl* OriginalDecl);
public: QueryResult GetNeededAssociatedTypes(llvm::ArrayRef<ast::AssociatedTypeDecl*> &Result, ast::RecordDecl* R);
public: QueryResult CheckConformances(QualType T);
public: QueryResult CheckSingleConformance(QualType T, ast::ProtocolDecl* P);
public: QueryResult CheckAssociatedTypeConstraints(ast::RecordDecl* R);
public: QueryResult TypecheckConstraints(ast::NamedDecl* ND);
public: QueryResult ConformsTo(bool &Result, CanType T, ast::ProtocolDecl* P);
public: QueryResult IsCovariant(bool &Result, QualType T, QualType Covar);
public: QueryResult IsContravariant(bool &Result, QualType T, QualType Contravar);
public: QueryResult GetTypeSize(unsigned &Result, CanType T);
public: QueryResult GetTypeAlignment(unsigned short &Result, CanType T);
public: QueryResult GetTypeStride(unsigned &Result, CanType T);
public: QueryResult CalculateRecordSize(unsigned &Result, ast::RecordDecl* R);
public: QueryResult GetRecordMeta(const RecordMetaInfo* &Result, ast::RecordDecl* R, uint8_t Initialize = true);
public: bool IsBuiltinIntegerType(ast::RecordDecl* R);
public: bool IsBuiltinFloatingPointType(ast::RecordDecl* R);
public: bool IsBuiltinBoolType(ast::RecordDecl* R);
public: QueryResult AddImplicitConformance(ast::MethodDecl* &Result, ast::RecordDecl* R, ast::ImplicitConformanceKind K, ast::MethodDecl* Impl = nullptr);
public: QueryResult GetImplicitConformance(ast::MethodDecl* &Result, ast::RecordDecl* R, ast::ImplicitConformanceKind K);
public: QueryResult CheckBuiltinConformances(ast::RecordDecl* R);
public: QueryResult IsAccessible(bool &Result, ast::DeclContext* DC, ast::NamedDecl* ND);
public: QueryResult CheckAccessibility(ast::DeclContext* DC, ast::NamedDecl* ND, SourceLocation Loc);
public: QueryResult ResolveNestedNameSpecToType(QualType &Result, NestedNameSpecifierWithLoc* Name, ast::DeclContext* DC = nullptr, bool IssueDiag = true);
public: QueryResult ResolveNestedNameSpecToDecl(ast::NamedDecl* &Result, NestedNameSpecifierWithLoc* Name, ast::DeclContext* DC = nullptr, bool IssueDiag = true);
public: QueryResult TypeCheckDeclContext(ast::DeclContext* DC);
public: QueryResult PrepareDeclInterface(ast::Decl* D);
public: QueryResult TypecheckDecl(ast::Decl* D);
private: QueryResult PreparePrecedenceGroupInterface(ast::PrecedenceGroupDecl* D);
private: QueryResult TypecheckPrecedenceGroup(ast::PrecedenceGroupDecl* D);
private: QueryResult PrepareOperatorInterface(ast::OperatorDecl* D);
private: QueryResult TypecheckOperator(ast::OperatorDecl* D);
private: QueryResult PrepareGlobalVarInterface(ast::GlobalVarDecl* D);
private: QueryResult TypecheckGlobalVar(ast::GlobalVarDecl* D);
private: QueryResult PrepareCallableInterface(ast::CallableDecl* D);
private: QueryResult TypecheckCallable(ast::CallableDecl* D);
private: QueryResult PrepareFunctionInterface(ast::FunctionDecl* D);
private: QueryResult TypecheckFunction(ast::FunctionDecl* D);
private: QueryResult PrepareMethodInterface(ast::MethodDecl* D);
private: QueryResult TypecheckMethod(ast::MethodDecl* D);
private: QueryResult PrepareInitInterface(ast::InitDecl* D);
private: QueryResult TypecheckInit(ast::InitDecl* D);
private: QueryResult PrepareDeinitInterface(ast::DeinitDecl* D);
private: QueryResult TypecheckDeinit(ast::DeinitDecl* D);
private: QueryResult PrepareFuncArgInterface(ast::FuncArgDecl* D);
private: QueryResult TypecheckFuncArg(ast::FuncArgDecl* D);
private: QueryResult PrepareTemplateParamInterface(ast::TemplateParamDecl* D);
private: QueryResult TypecheckTemplateParam(ast::TemplateParamDecl* D);
private: QueryResult PrepareAssociatedTypeInterface(ast::AssociatedTypeDecl* D);
private: QueryResult TypecheckAssociatedType(ast::AssociatedTypeDecl* D);
private: QueryResult PrepareAliasInterface(ast::AliasDecl* D);
private: QueryResult TypecheckAlias(ast::AliasDecl* D);
private: QueryResult PrepareRecordInterface(ast::RecordDecl* D);
private: QueryResult TypecheckRecord(ast::RecordDecl* D);
private: QueryResult PrepareStructInterface(ast::StructDecl* D);
private: QueryResult TypecheckStruct(ast::StructDecl* D);
private: QueryResult PrepareClassInterface(ast::ClassDecl* D);
private: QueryResult TypecheckClass(ast::ClassDecl* D);
private: QueryResult PrepareEnumInterface(ast::EnumDecl* D);
private: QueryResult TypecheckEnum(ast::EnumDecl* D);
private: QueryResult PrepareProtocolInterface(ast::ProtocolDecl* D);
private: QueryResult TypecheckProtocol(ast::ProtocolDecl* D);
private: QueryResult PrepareExtensionInterface(ast::ExtensionDecl* D);
private: QueryResult TypecheckExtension(ast::ExtensionDecl* D);
private: QueryResult PrepareFieldInterface(ast::FieldDecl* D);
private: QueryResult TypecheckField(ast::FieldDecl* D);
private: QueryResult PreparePropInterface(ast::PropDecl* D);
private: QueryResult TypecheckProp(ast::PropDecl* D);
private: QueryResult PrepareSubscriptInterface(ast::SubscriptDecl* D);
private: QueryResult TypecheckSubscript(ast::SubscriptDecl* D);
private: QueryResult PrepareEnumCaseInterface(ast::EnumCaseDecl* D);
private: QueryResult TypecheckEnumCase(ast::EnumCaseDecl* D);
private: QueryResult PrepareNamespaceInterface(ast::NamespaceDecl* D);
private: QueryResult TypecheckNamespace(ast::NamespaceDecl* D);
private: QueryResult PrepareCompoundInterface(ast::CompoundDecl* D);
private: QueryResult TypecheckCompound(ast::CompoundDecl* D);
private: QueryResult PrepareModuleInterface(ast::ModuleDecl* D);
private: QueryResult TypecheckModule(ast::ModuleDecl* D);
public: QueryResult PrepareTemplateParameters(ast::NamedDecl* Decl);
public: QueryResult DeclareImplicitInitializers(ast::RecordDecl* R);
public: QueryResult DeclareImplicitDefaultInit(ast::StructDecl* S);
public: QueryResult DeclareMemberwiseInit(ast::StructDecl* S);
public: QueryResult DeclareImplicitDefaultDeinit(ast::RecordDecl* S);
public: QueryResult ResolveRawType(ast::EnumDecl* E);
public: QueryResult AssignInitName(ast::InitDecl* Init);
public: QueryResult CreateBaseInit(ast::InitDecl* D);
public: QueryResult LookupFirst(const SingleLevelLookupResult* &Result, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts = DefaultLookupOpts);
public: QueryResult LookupSingle(ast::NamedDecl* &Result, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts = DefaultLookupOpts | LookupOpts::IssueDiag);
public: QueryResult MultiLevelLookup(const MultiLevelLookupResult* &Result, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts = DefaultLookupOpts);
public: QueryResult MultiLevelTypeLookup(const MultiLevelLookupResult* &Result, CanType T, DeclarationName Name, LookupOpts Opts = DefaultLookupOpts);
public: QueryResult DirectLookup(const MultiLevelLookupResult* &Result, ast::DeclContext* DC, DeclarationName Name, bool LookInExtensions = true, LookupOpts Opts = DefaultLookupOpts);
public: QueryResult NestedNameLookup(const MultiLevelLookupResult* &Result, ast::DeclContext* DC, llvm::ArrayRef<DeclarationName> Names, LookupOpts Opts = DefaultLookupOpts | LookupOpts::IssueDiag, SourceRange Loc = SourceRange());
public: QueryResult ResolveMacros(ast::DeclContext* DC);
public: QueryResult ResolveStaticDeclarations(ast::DeclContext* DC);
public: QueryResult ResolveWildcardDeclarations(ast::DeclContext* DC);
public: QueryResult PrepareTypeNameLookup(ast::DeclContext* DC);
public: QueryResult PrepareNameLookup(ast::DeclContext* DC);
public: QueryResult InstantiateFields(ast::StructDecl* S);
public: QueryResult InstantiateCases(ast::EnumDecl* E);
public: QueryResult GetAssociatedType(ast::AssociatedTypeDecl* &Result, ast::RecordDecl* R, DeclarationName Name, llvm::ArrayRef<ast::ExtensionDecl*> Extensions);
public: QueryResult CheckTypeEquivalence(bool &Result, QualType LHS, QualType RHS, QualType Self, ast::DeclContext* LHSDecl = nullptr);
public: QueryResult InstantiateProtocolDefaultImpl(ast::NamedDecl* &Result, ast::NamedDecl* Impl, QualType Self, bool ActOnDecl = true);
public: QueryResult InstantiateRecord(ast::RecordDecl* &Result, ast::RecordDecl* Template, sema::FinalTemplateArgumentList* TemplateArgs, SourceLocation PointOfInstantiation, ast::RecordDecl* OuterInst = nullptr);
public: QueryResult InstantiateAlias(ast::AliasDecl* &Result, ast::AliasDecl* Template, sema::FinalTemplateArgumentList* TemplateArgs, SourceLocation PointOfInstantiation);
public: QueryResult InstantiateCallable(ast::CallableDecl* &Result, ast::CallableDecl* Template, sema::FinalTemplateArgumentList* TemplateArgs, SourceLocation PointOfInstantiation);
public: QueryResult InstantiateFunction(ast::FunctionDecl* &Result, ast::FunctionDecl* Template, sema::FinalTemplateArgumentList* TemplateArgs, SourceLocation PointOfInstantiation);
public: QueryResult InstantiateMethod(ast::MethodDecl* &Result, ast::MethodDecl* Template, sema::FinalTemplateArgumentList* TemplateArgs, SourceLocation PointOfInstantiation);
public: QueryResult InstantiateMethodBody(ast::CallableDecl* Inst);
public: QueryResult InstantiateDecl(ast::Decl* &Result, ast::Decl* Template, sema::MultiLevelFinalTemplateArgList TemplateArgs);
public: QueryResult CheckTemplateExtensionApplicability(bool &Result, ast::RecordDecl* Inst, ast::ExtensionDecl* Ext);
public: QueryResult PrepareDeclForInstantiation(ast::NamedDecl* ND);
public: QueryResult InstantiateTemplateMember(ast::NamedDecl* &Result, ast::NamedDecl* TemplateMember, ast::RecordDecl* Inst, sema::FinalTemplateArgumentList* TemplateArgs = nullptr, SourceLocation PointOfInstantiation = SourceLocation());
public: QueryResult PrepareInstantiationForNameLookup(ast::RecordDecl* Inst);
public: QueryResult GetILModuleForDecl(il::Module* &Result, ast::Decl* D);
public: QueryResult GenerateILForContext(ast::DeclContext* DC);
private: QueryResult GenerateRecordIL(ast::RecordDecl* R);
public: QueryResult GetILGlobal(il::GlobalVariable* &Result, ast::VarDecl* GV);
private: QueryResult GenerateLazyILGlobalDefinition(ast::VarDecl* GV);
public: QueryResult GetILFunction(il::Function* &Result, ast::CallableDecl* C);
private: QueryResult GenerateILFunctionBody(ast::CallableDecl* C);
public: QueryResult GetILTypeInfo(il::GlobalVariable* &Result, QualType T);
private: QueryResult CreateILBasicTypeInfo(il::Constant* &Result, QualType T);
private: QueryResult CreateILRecordTypeInfo(il::Constant* &Result, ast::RecordDecl* R);
public: QueryResult GetBoolValue(bool &Result, il::Constant* C, uint8_t AllowWrapperTypes = true);
public: QueryResult GetStringValue(llvm::StringRef &Result, il::Constant* C, uint8_t AllowWrapperTypes = true);
public: QueryResult GetIntValue(llvm::APSInt &Result, il::Constant* C, uint8_t AllowWrapperTypes = true);
public: QueryResult OpenFileForRead(llvm::MemoryBuffer* &Result, StringRef FileName);
public: QueryResult OpenTmpFile(llvm::raw_ostream* &Result, StringRef Extension);
public: QueryResult GetDefaultTemplateArg(const sema::TemplateArgument* &Result, ast::TemplateParamDecl* P);
public: QueryResult CreateSelfArgument(ast::FuncArgDecl* &Result, QualType Self, SourceLocation SelfLoc);
public: QueryResult FindEquivalentDecl(ast::NamedDecl* &Result, ast::NamedDecl* Decl, ast::DeclContext* DC, QualType Self = QualType(), bool LookInExtensions = true);
public: QueryResult EquivalentSignatures(EquivalentSignaturesQuery::ResultKind &Result, ast::CallableDecl* C1, ast::CallableDecl* C2, QualType Self);


friend class CompileModuleQuery;
friend class ParseModuleFileQuery;
friend class CreateDefaultModuleQuery;
friend class ParseSourceFileQuery;
friend class ParseMainSourceFileQuery;
friend class ParseSourceFilesQuery;
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
friend class IsPersistableQuery;
friend class IsEquatableQuery;
friend class IsCopyableQuery;
friend class IsMoveOnlyQuery;
friend class IsUnpopulatedQuery;
friend class IsImplicitlyCopyableQuery;
friend class IsTriviallyCopyableQuery;
friend class NeedsRetainOrReleaseQuery;
friend class NeedsDeinitilizationQuery;
friend class NeedsStructReturnQuery;
friend class ContainsAssociatedTypeConstraintQuery;
friend class IsImplicitlyConvertibleQuery;
friend class GetConversionSequenceQuery;
friend class SubstTypeVariablesQuery;
friend class SubstAssociatedTypesQuery;
friend class SubstGenericTypesQuery;
friend class SubstGenericTypesNonFinalQuery;
friend class CheckTypeCapabilitiesQuery;
friend class ApplyCapabilitesQuery;
friend class GetBuiltinModuleQuery;
friend class GetBuiltinFuncQuery;
friend class GetBuiltinAliasQuery;
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
friend class ImportExternalSourceFileQuery;
friend class FindExtensionsQuery;
friend class GetExtensionTypeKindQuery;
friend class MatchExtensionTypeQuery;
friend class ExtensionAppliesQuery;
friend class GetExtendedDeclQuery;
friend class ResolveExtensionQuery;
friend class ResolveConformanceToProtocolQuery;
friend class ResolveExplicitConformancesQuery;
friend class ResolveDeclaredConformancesQuery;
friend class AddSingleConformanceQuery;
friend class DeclareSelfAliasQuery;
friend class ResolveAssociatedTypesQuery;
friend class GetReferencedAssociatedTypesQuery;
friend class ReferencedAssociatedTypesReadyQuery;
friend class CheckProtocolExtensionApplicabilityQuery;
friend class VerifyConstraintQuery;
friend class IsConstraintSatisfiedQuery;
friend class GetConstrainedTypeQuery;
friend class GetNeededAssociatedTypesQuery;
friend class CheckConformancesQuery;
friend class CheckSingleConformanceQuery;
friend class CheckAssociatedTypeConstraintsQuery;
friend class TypecheckConstraintsQuery;
friend class ConformsToQuery;
friend class IsCovariantQuery;
friend class IsContravariantQuery;
friend class GetTypeSizeQuery;
friend class GetTypeAlignmentQuery;
friend class GetTypeStrideQuery;
friend class CalculateRecordSizeQuery;
friend class GetRecordMetaQuery;
friend class IsBuiltinIntegerTypeQuery;
friend class IsBuiltinFloatingPointTypeQuery;
friend class IsBuiltinBoolTypeQuery;
friend class AddImplicitConformanceQuery;
friend class GetImplicitConformanceQuery;
friend class CheckBuiltinConformancesQuery;
friend class IsAccessibleQuery;
friend class CheckAccessibilityQuery;
friend class ResolveNestedNameSpecToTypeQuery;
friend class ResolveNestedNameSpecToDeclQuery;
friend class TypeCheckDeclContextQuery;
friend class PrepareDeclInterfaceQuery;
friend class TypecheckDeclQuery;
friend class PreparePrecedenceGroupInterfaceQuery;
friend class TypecheckPrecedenceGroupQuery;
friend class PrepareOperatorInterfaceQuery;
friend class TypecheckOperatorQuery;
friend class PrepareGlobalVarInterfaceQuery;
friend class TypecheckGlobalVarQuery;
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
friend class PrepareAliasInterfaceQuery;
friend class TypecheckAliasQuery;
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
friend class PrepareEnumCaseInterfaceQuery;
friend class TypecheckEnumCaseQuery;
friend class PrepareNamespaceInterfaceQuery;
friend class TypecheckNamespaceQuery;
friend class PrepareCompoundInterfaceQuery;
friend class TypecheckCompoundQuery;
friend class PrepareModuleInterfaceQuery;
friend class TypecheckModuleQuery;
friend class PrepareTemplateParametersQuery;
friend class DeclareImplicitInitializersQuery;
friend class DeclareImplicitDefaultInitQuery;
friend class DeclareMemberwiseInitQuery;
friend class DeclareImplicitDefaultDeinitQuery;
friend class ResolveRawTypeQuery;
friend class AssignInitNameQuery;
friend class CreateBaseInitQuery;
friend class LookupFirstQuery;
friend class LookupSingleQuery;
friend class MultiLevelLookupQuery;
friend class MultiLevelTypeLookupQuery;
friend class DirectLookupQuery;
friend class NestedNameLookupQuery;
friend class ResolveMacrosQuery;
friend class ResolveStaticDeclarationsQuery;
friend class ResolveWildcardDeclarationsQuery;
friend class PrepareTypeNameLookupQuery;
friend class PrepareNameLookupQuery;
friend class InstantiateFieldsQuery;
friend class InstantiateCasesQuery;
friend class GetAssociatedTypeQuery;
friend class CheckTypeEquivalenceQuery;
friend class InstantiateProtocolDefaultImplQuery;
friend class InstantiateRecordQuery;
friend class InstantiateAliasQuery;
friend class InstantiateCallableQuery;
friend class InstantiateFunctionQuery;
friend class InstantiateMethodQuery;
friend class InstantiateMethodBodyQuery;
friend class InstantiateDeclQuery;
friend class CheckTemplateExtensionApplicabilityQuery;
friend class PrepareDeclForInstantiationQuery;
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
friend class GetBoolValueQuery;
friend class GetStringValueQuery;
friend class GetIntValueQuery;
friend class OpenFileForReadQuery;
friend class OpenTmpFileQuery;
friend class GetDefaultTemplateArgQuery;
friend class CreateSelfArgumentQuery;
friend class FindEquivalentDeclQuery;
friend class EquivalentSignaturesQuery;