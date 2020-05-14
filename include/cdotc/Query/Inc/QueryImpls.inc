bool Query::isPure() const
{
   switch (K) {
   case CompileModuleQueryID: return false;
   case VerifyModuleQueryID: return false;
   case RunTestModuleQueryID: return false;
   case ParseModuleFileQueryID: return false;
   case CreateDefaultModuleQueryID: return false;
   case ParseSourceFileQueryID: return false;
   case ParseMainSourceFileQueryID: return false;
   case ParseSourceFilesQueryID: return false;
   case CreateILModuleQueryID: return false;
   case SetupIRGenQueryID: return false;
   case CreateLLVMModuleQueryID: return false;
   case CreateObjectQueryID: return false;
   case CreateStaticLibQueryID: return false;
   case CreateDynamicLibQueryID: return false;
   case CreateExecutableQueryID: return false;
   case EmitILQueryID: return false;
   case EmitIRQueryID: return false;
   case PrintUsedMemoryQueryID: return false;
   case MeasureExecutionTimeQueryID: return false;
   case IsPersistableQueryID: return false;
   case IsEquatableQueryID: return false;
   case IsCopyableQueryID: return false;
   case IsMoveOnlyQueryID: return false;
   case IsUnpopulatedQueryID: return false;
   case IsImplicitlyCopyableQueryID: return false;
   case IsTriviallyCopyableQueryID: return false;
   case NeedsRetainOrReleaseQueryID: return false;
   case NeedsDeinitilizationQueryID: return false;
   case NeedsStructReturnQueryID: return false;
   case PassByValueQueryID: return false;
   case ContainsTemplateQueryID: return false;
   case ContainsAssociatedTypeConstraintQueryID: return false;
   case ContainsProtocolWithAssociatedTypesQueryID: return false;
   case IsImplicitlyConvertibleQueryID: return false;
   case IsValidParameterValueQueryID: return false;
   case GetConversionSequenceQueryID: return false;
   case SubstTypeVariablesQueryID: return false;
   case SubstAssociatedTypesQueryID: return false;
   case SubstTemplateParamTypesQueryID: return false;
   case SubstTemplateParamTypesNonFinalQueryID: return false;
   case CheckTypeCapabilitiesQueryID: return false;
   case GetBuiltinModuleQueryID: return false;
   case GetBuiltinFuncQueryID: return false;
   case GetBuiltinAliasQueryID: return false;
   case GetBuiltinRecordQueryID: return false;
   case GetBuiltinProtocolQueryID: return false;
   case FindPrecedenceGroupQueryID: return false;
   case FindOperatorQueryID: return false;
   case ResolveImportQueryID: return false;
   case ResolveUsingQueryID: return false;
   case ResolveStaticIfQueryID: return false;
   case ResolveStaticForQueryID: return false;
   case ResolveStaticAssertQueryID: return false;
   case ResolveStaticPrintQueryID: return false;
   case ResolveStaticExprQueryID: return false;
   case ResolveStaticExprToBoolQueryID: return false;
   case ExpandMacroQueryID: return false;
   case ExpandMacroDeclQueryID: return false;
   case ExpandMacrosQueryID: return false;
   case ImportExternalSourceFileQueryID: return false;
   case FindExtensionsQueryID: return false;
   case GetExtensionTypeKindQueryID: return false;
   case MatchExtensionTypeQueryID: return false;
   case ExtensionAppliesQueryID: return false;
   case GetExtendedDeclQueryID: return false;
   case ResolveExtensionQueryID: return false;
   case ResolveConformanceToProtocolQueryID: return false;
   case ResolveExplicitConformancesQueryID: return false;
   case ResolveConformancesToProtocolsQueryID: return false;
   case ResolveDeclaredConformancesQueryID: return false;
   case AddSingleConformanceQueryID: return false;
   case DeclareSelfAliasQueryID: return false;
   case ReferencedAssociatedTypesReadyQueryID: return false;
   case CheckProtocolExtensionApplicabilityQueryID: return false;
   case VerifyConstraintQueryID: return false;
   case VerifyConstraintsQueryID: return false;
   case IsConstraintSatisfiedQueryID: return false;
   case GetConstrainedTypeQueryID: return false;
   case IsSupersetOfQueryID: return false;
   case GetReferencedAssociatedTypesQueryID: return false;
   case GetNeededAssociatedTypesQueryID: return false;
   case CheckConformancesQueryID: return false;
   case CheckSingleConformanceQueryID: return false;
   case CheckAssociatedTypeConstraintsQueryID: return false;
   case TypecheckConstraintsQueryID: return false;
   case ConformsToQueryID: return false;
   case IsCovariantQueryID: return false;
   case IsContravariantQueryID: return false;
   case GetTypeSizeQueryID: return false;
   case GetTypeAlignmentQueryID: return false;
   case GetTypeStrideQueryID: return false;
   case CalculateRecordSizeQueryID: return false;
   case GetRecordMetaQueryID: return false;
   case IsBuiltinIntegerTypeQueryID: return false;
   case IsBuiltinFloatingPointTypeQueryID: return false;
   case IsBuiltinBoolTypeQueryID: return false;
   case AddImplicitConformanceQueryID: return false;
   case GetImplicitConformanceQueryID: return false;
   case CheckBuiltinConformancesQueryID: return false;
   case IsAccessibleQueryID: return false;
   case CheckAccessibilityQueryID: return false;
   case ResolveNestedNameSpecToTypeQueryID: return false;
   case ResolveNestedNameSpecToDeclQueryID: return false;
   case TypeCheckDeclContextQueryID: return false;
   case PrepareDeclInterfaceQueryID: return false;
   case TypecheckDeclQueryID: return false;
   case PreparePrecedenceGroupInterfaceQueryID: return false;
   case TypecheckPrecedenceGroupQueryID: return false;
   case PrepareOperatorInterfaceQueryID: return false;
   case TypecheckOperatorQueryID: return false;
   case PrepareGlobalVarInterfaceQueryID: return false;
   case TypecheckGlobalVarQueryID: return false;
   case PrepareCallableInterfaceQueryID: return false;
   case TypecheckCallableQueryID: return false;
   case PrepareFunctionInterfaceQueryID: return false;
   case TypecheckFunctionQueryID: return false;
   case PrepareMethodInterfaceQueryID: return false;
   case TypecheckMethodQueryID: return false;
   case PrepareInitInterfaceQueryID: return false;
   case TypecheckInitQueryID: return false;
   case PrepareDeinitInterfaceQueryID: return false;
   case TypecheckDeinitQueryID: return false;
   case PrepareFuncArgInterfaceQueryID: return false;
   case TypecheckFuncArgQueryID: return false;
   case PrepareTemplateParamInterfaceQueryID: return false;
   case TypecheckTemplateParamQueryID: return false;
   case PrepareAssociatedTypeInterfaceQueryID: return false;
   case TypecheckAssociatedTypeQueryID: return false;
   case PrepareAliasInterfaceQueryID: return false;
   case TypecheckAliasQueryID: return false;
   case PrepareRecordInterfaceQueryID: return false;
   case TypecheckRecordQueryID: return false;
   case PrepareStructInterfaceQueryID: return false;
   case TypecheckStructQueryID: return false;
   case PrepareClassInterfaceQueryID: return false;
   case TypecheckClassQueryID: return false;
   case PrepareEnumInterfaceQueryID: return false;
   case TypecheckEnumQueryID: return false;
   case PrepareProtocolInterfaceQueryID: return false;
   case TypecheckProtocolQueryID: return false;
   case PrepareExtensionInterfaceQueryID: return false;
   case TypecheckExtensionQueryID: return false;
   case PrepareFieldInterfaceQueryID: return false;
   case TypecheckFieldQueryID: return false;
   case PreparePropInterfaceQueryID: return false;
   case TypecheckPropQueryID: return false;
   case PrepareSubscriptInterfaceQueryID: return false;
   case TypecheckSubscriptQueryID: return false;
   case PrepareEnumCaseInterfaceQueryID: return false;
   case TypecheckEnumCaseQueryID: return false;
   case PrepareNamespaceInterfaceQueryID: return false;
   case TypecheckNamespaceQueryID: return false;
   case PrepareCompoundInterfaceQueryID: return false;
   case TypecheckCompoundQueryID: return false;
   case PrepareModuleInterfaceQueryID: return false;
   case TypecheckModuleQueryID: return false;
   case PrepareTemplateParametersQueryID: return false;
   case DeclareImplicitInitializersQueryID: return false;
   case DeclareImplicitDefaultInitQueryID: return false;
   case DeclareMemberwiseInitQueryID: return false;
   case DeclareImplicitDefaultDeinitQueryID: return false;
   case ResolveRawTypeQueryID: return false;
   case AssignInitNameQueryID: return false;
   case CreateBaseInitQueryID: return false;
   case LookupFirstQueryID: return false;
   case LookupSingleQueryID: return false;
   case MultiLevelLookupQueryID: return false;
   case MultiLevelTypeLookupQueryID: return false;
   case RestrictedLookupQueryID: return true;
   case DirectLookupQueryID: return false;
   case NestedNameLookupQueryID: return false;
   case ResolveMacrosQueryID: return false;
   case ResolveStaticDeclarationsQueryID: return false;
   case ResolveWildcardDeclarationsQueryID: return false;
   case ResolveMetaDeclarationsQueryID: return false;
   case GetAssociatedTypeImplQueryID: return false;
   case GetAssociatedTypeDeclQueryID: return false;
   case CheckTypeEquivalenceQueryID: return false;
   case CheckTemplateExtensionApplicabilityQueryID: return false;
   case GetILModuleForDeclQueryID: return false;
   case GenerateILForDeclQueryID: return false;
   case GenerateILForContextQueryID: return false;
   case GenerateRecordILQueryID: return false;
   case GetILGlobalQueryID: return false;
   case GenerateLazyILGlobalDefinitionQueryID: return false;
   case GetILFunctionQueryID: return false;
   case GenerateILFunctionBodyQueryID: return false;
   case GetILTypeInfoQueryID: return false;
   case CreateILBasicTypeInfoQueryID: return false;
   case CreateILRecordTypeInfoQueryID: return false;
   case GetBoolValueQueryID: return true;
   case GetStringValueQueryID: return true;
   case GetIntValueQueryID: return false;
   case OpenFileForReadQueryID: return false;
   case OpenTmpFileQueryID: return false;
   case GetDefaultTemplateArgQueryID: return false;
   case CreateSelfArgumentQueryID: return false;
   case FindEquivalentDeclQueryID: return false;
   case EquivalentSignaturesQueryID: return false;
   }
}

bool Query::canBeCached() const
{
   switch (K) {
   case CompileModuleQueryID: return true;
   case VerifyModuleQueryID: return true;
   case RunTestModuleQueryID: return true;
   case ParseModuleFileQueryID: return true;
   case CreateDefaultModuleQueryID: return true;
   case ParseSourceFileQueryID: return true;
   case ParseMainSourceFileQueryID: return true;
   case ParseSourceFilesQueryID: return true;
   case CreateILModuleQueryID: return true;
   case SetupIRGenQueryID: return true;
   case CreateLLVMModuleQueryID: return true;
   case CreateObjectQueryID: return false;
   case CreateStaticLibQueryID: return false;
   case CreateDynamicLibQueryID: return false;
   case CreateExecutableQueryID: return false;
   case EmitILQueryID: return false;
   case EmitIRQueryID: return false;
   case PrintUsedMemoryQueryID: return false;
   case MeasureExecutionTimeQueryID: return true;
   case IsPersistableQueryID: return true;
   case IsEquatableQueryID: return true;
   case IsCopyableQueryID: return true;
   case IsMoveOnlyQueryID: return true;
   case IsUnpopulatedQueryID: return true;
   case IsImplicitlyCopyableQueryID: return true;
   case IsTriviallyCopyableQueryID: return true;
   case NeedsRetainOrReleaseQueryID: return true;
   case NeedsDeinitilizationQueryID: return true;
   case NeedsStructReturnQueryID: return true;
   case PassByValueQueryID: return true;
   case ContainsTemplateQueryID: return true;
   case ContainsAssociatedTypeConstraintQueryID: return true;
   case ContainsProtocolWithAssociatedTypesQueryID: return true;
   case IsImplicitlyConvertibleQueryID: return true;
   case IsValidParameterValueQueryID: return true;
   case GetConversionSequenceQueryID: return true;
   case SubstTypeVariablesQueryID: return false;
   case SubstAssociatedTypesQueryID: return true;
   case SubstTemplateParamTypesQueryID: return true;
   case SubstTemplateParamTypesNonFinalQueryID: return false;
   case CheckTypeCapabilitiesQueryID: return true;
   case GetBuiltinModuleQueryID: return true;
   case GetBuiltinFuncQueryID: return true;
   case GetBuiltinAliasQueryID: return true;
   case GetBuiltinRecordQueryID: return true;
   case GetBuiltinProtocolQueryID: return true;
   case FindPrecedenceGroupQueryID: return true;
   case FindOperatorQueryID: return true;
   case ResolveImportQueryID: return true;
   case ResolveUsingQueryID: return true;
   case ResolveStaticIfQueryID: return true;
   case ResolveStaticForQueryID: return true;
   case ResolveStaticAssertQueryID: return true;
   case ResolveStaticPrintQueryID: return true;
   case ResolveStaticExprQueryID: return true;
   case ResolveStaticExprToBoolQueryID: return true;
   case ExpandMacroQueryID: return true;
   case ExpandMacroDeclQueryID: return true;
   case ExpandMacrosQueryID: return true;
   case ImportExternalSourceFileQueryID: return true;
   case FindExtensionsQueryID: return true;
   case GetExtensionTypeKindQueryID: return true;
   case MatchExtensionTypeQueryID: return true;
   case ExtensionAppliesQueryID: return true;
   case GetExtendedDeclQueryID: return true;
   case ResolveExtensionQueryID: return true;
   case ResolveConformanceToProtocolQueryID: return true;
   case ResolveExplicitConformancesQueryID: return true;
   case ResolveConformancesToProtocolsQueryID: return true;
   case ResolveDeclaredConformancesQueryID: return true;
   case AddSingleConformanceQueryID: return true;
   case DeclareSelfAliasQueryID: return true;
   case ReferencedAssociatedTypesReadyQueryID: return false;
   case CheckProtocolExtensionApplicabilityQueryID: return true;
   case VerifyConstraintQueryID: return true;
   case VerifyConstraintsQueryID: return true;
   case IsConstraintSatisfiedQueryID: return true;
   case GetConstrainedTypeQueryID: return true;
   case IsSupersetOfQueryID: return true;
   case GetReferencedAssociatedTypesQueryID: return true;
   case GetNeededAssociatedTypesQueryID: return true;
   case CheckConformancesQueryID: return true;
   case CheckSingleConformanceQueryID: return true;
   case CheckAssociatedTypeConstraintsQueryID: return true;
   case TypecheckConstraintsQueryID: return true;
   case ConformsToQueryID: return true;
   case IsCovariantQueryID: return true;
   case IsContravariantQueryID: return true;
   case GetTypeSizeQueryID: return true;
   case GetTypeAlignmentQueryID: return true;
   case GetTypeStrideQueryID: return true;
   case CalculateRecordSizeQueryID: return true;
   case GetRecordMetaQueryID: return false;
   case IsBuiltinIntegerTypeQueryID: return true;
   case IsBuiltinFloatingPointTypeQueryID: return true;
   case IsBuiltinBoolTypeQueryID: return true;
   case AddImplicitConformanceQueryID: return true;
   case GetImplicitConformanceQueryID: return false;
   case CheckBuiltinConformancesQueryID: return true;
   case IsAccessibleQueryID: return true;
   case CheckAccessibilityQueryID: return true;
   case ResolveNestedNameSpecToTypeQueryID: return true;
   case ResolveNestedNameSpecToDeclQueryID: return true;
   case TypeCheckDeclContextQueryID: return true;
   case PrepareDeclInterfaceQueryID: return true;
   case TypecheckDeclQueryID: return true;
   case PreparePrecedenceGroupInterfaceQueryID: return true;
   case TypecheckPrecedenceGroupQueryID: return true;
   case PrepareOperatorInterfaceQueryID: return true;
   case TypecheckOperatorQueryID: return true;
   case PrepareGlobalVarInterfaceQueryID: return true;
   case TypecheckGlobalVarQueryID: return true;
   case PrepareCallableInterfaceQueryID: return true;
   case TypecheckCallableQueryID: return true;
   case PrepareFunctionInterfaceQueryID: return true;
   case TypecheckFunctionQueryID: return true;
   case PrepareMethodInterfaceQueryID: return true;
   case TypecheckMethodQueryID: return true;
   case PrepareInitInterfaceQueryID: return true;
   case TypecheckInitQueryID: return true;
   case PrepareDeinitInterfaceQueryID: return true;
   case TypecheckDeinitQueryID: return true;
   case PrepareFuncArgInterfaceQueryID: return true;
   case TypecheckFuncArgQueryID: return true;
   case PrepareTemplateParamInterfaceQueryID: return true;
   case TypecheckTemplateParamQueryID: return true;
   case PrepareAssociatedTypeInterfaceQueryID: return true;
   case TypecheckAssociatedTypeQueryID: return true;
   case PrepareAliasInterfaceQueryID: return true;
   case TypecheckAliasQueryID: return true;
   case PrepareRecordInterfaceQueryID: return true;
   case TypecheckRecordQueryID: return true;
   case PrepareStructInterfaceQueryID: return true;
   case TypecheckStructQueryID: return true;
   case PrepareClassInterfaceQueryID: return true;
   case TypecheckClassQueryID: return true;
   case PrepareEnumInterfaceQueryID: return true;
   case TypecheckEnumQueryID: return true;
   case PrepareProtocolInterfaceQueryID: return true;
   case TypecheckProtocolQueryID: return true;
   case PrepareExtensionInterfaceQueryID: return true;
   case TypecheckExtensionQueryID: return true;
   case PrepareFieldInterfaceQueryID: return true;
   case TypecheckFieldQueryID: return true;
   case PreparePropInterfaceQueryID: return true;
   case TypecheckPropQueryID: return true;
   case PrepareSubscriptInterfaceQueryID: return true;
   case TypecheckSubscriptQueryID: return true;
   case PrepareEnumCaseInterfaceQueryID: return true;
   case TypecheckEnumCaseQueryID: return true;
   case PrepareNamespaceInterfaceQueryID: return true;
   case TypecheckNamespaceQueryID: return true;
   case PrepareCompoundInterfaceQueryID: return true;
   case TypecheckCompoundQueryID: return true;
   case PrepareModuleInterfaceQueryID: return true;
   case TypecheckModuleQueryID: return true;
   case PrepareTemplateParametersQueryID: return true;
   case DeclareImplicitInitializersQueryID: return true;
   case DeclareImplicitDefaultInitQueryID: return true;
   case DeclareMemberwiseInitQueryID: return true;
   case DeclareImplicitDefaultDeinitQueryID: return true;
   case ResolveRawTypeQueryID: return true;
   case AssignInitNameQueryID: return true;
   case CreateBaseInitQueryID: return true;
   case LookupFirstQueryID: return true;
   case LookupSingleQueryID: return true;
   case MultiLevelLookupQueryID: return true;
   case MultiLevelTypeLookupQueryID: return true;
   case RestrictedLookupQueryID: return true;
   case DirectLookupQueryID: return true;
   case NestedNameLookupQueryID: return false;
   case ResolveMacrosQueryID: return true;
   case ResolveStaticDeclarationsQueryID: return true;
   case ResolveWildcardDeclarationsQueryID: return true;
   case ResolveMetaDeclarationsQueryID: return true;
   case GetAssociatedTypeImplQueryID: return false;
   case GetAssociatedTypeDeclQueryID: return false;
   case CheckTypeEquivalenceQueryID: return true;
   case CheckTemplateExtensionApplicabilityQueryID: return true;
   case GetILModuleForDeclQueryID: return true;
   case GenerateILForDeclQueryID: return true;
   case GenerateILForContextQueryID: return true;
   case GenerateRecordILQueryID: return true;
   case GetILGlobalQueryID: return true;
   case GenerateLazyILGlobalDefinitionQueryID: return true;
   case GetILFunctionQueryID: return true;
   case GenerateILFunctionBodyQueryID: return true;
   case GetILTypeInfoQueryID: return true;
   case CreateILBasicTypeInfoQueryID: return true;
   case CreateILRecordTypeInfoQueryID: return true;
   case GetBoolValueQueryID: return true;
   case GetStringValueQueryID: return true;
   case GetIntValueQueryID: return true;
   case OpenFileForReadQueryID: return true;
   case OpenTmpFileQueryID: return false;
   case GetDefaultTemplateArgQueryID: return true;
   case CreateSelfArgumentQueryID: return false;
   case FindEquivalentDeclQueryID: return true;
   case EquivalentSignaturesQueryID: return true;
   }
}

bool Query::canBeSerialized() const
{
   switch (K) {
   case CompileModuleQueryID: return false;
   case VerifyModuleQueryID: return false;
   case RunTestModuleQueryID: return false;
   case ParseModuleFileQueryID: return false;
   case CreateDefaultModuleQueryID: return false;
   case ParseSourceFileQueryID: return false;
   case ParseMainSourceFileQueryID: return false;
   case ParseSourceFilesQueryID: return false;
   case CreateILModuleQueryID: return false;
   case SetupIRGenQueryID: return false;
   case CreateLLVMModuleQueryID: return false;
   case CreateObjectQueryID: return false;
   case CreateStaticLibQueryID: return false;
   case CreateDynamicLibQueryID: return false;
   case CreateExecutableQueryID: return false;
   case EmitILQueryID: return false;
   case EmitIRQueryID: return false;
   case PrintUsedMemoryQueryID: return false;
   case MeasureExecutionTimeQueryID: return false;
   case IsPersistableQueryID: return false;
   case IsEquatableQueryID: return false;
   case IsCopyableQueryID: return false;
   case IsMoveOnlyQueryID: return false;
   case IsUnpopulatedQueryID: return false;
   case IsImplicitlyCopyableQueryID: return false;
   case IsTriviallyCopyableQueryID: return false;
   case NeedsRetainOrReleaseQueryID: return false;
   case NeedsDeinitilizationQueryID: return false;
   case NeedsStructReturnQueryID: return false;
   case PassByValueQueryID: return false;
   case ContainsTemplateQueryID: return false;
   case ContainsAssociatedTypeConstraintQueryID: return false;
   case ContainsProtocolWithAssociatedTypesQueryID: return false;
   case IsImplicitlyConvertibleQueryID: return false;
   case IsValidParameterValueQueryID: return false;
   case GetConversionSequenceQueryID: return false;
   case SubstTypeVariablesQueryID: return false;
   case SubstAssociatedTypesQueryID: return false;
   case SubstTemplateParamTypesQueryID: return false;
   case SubstTemplateParamTypesNonFinalQueryID: return false;
   case CheckTypeCapabilitiesQueryID: return false;
   case GetBuiltinModuleQueryID: return false;
   case GetBuiltinFuncQueryID: return false;
   case GetBuiltinAliasQueryID: return false;
   case GetBuiltinRecordQueryID: return false;
   case GetBuiltinProtocolQueryID: return false;
   case FindPrecedenceGroupQueryID: return false;
   case FindOperatorQueryID: return false;
   case ResolveImportQueryID: return false;
   case ResolveUsingQueryID: return false;
   case ResolveStaticIfQueryID: return false;
   case ResolveStaticForQueryID: return false;
   case ResolveStaticAssertQueryID: return false;
   case ResolveStaticPrintQueryID: return false;
   case ResolveStaticExprQueryID: return false;
   case ResolveStaticExprToBoolQueryID: return false;
   case ExpandMacroQueryID: return false;
   case ExpandMacroDeclQueryID: return false;
   case ExpandMacrosQueryID: return false;
   case ImportExternalSourceFileQueryID: return false;
   case FindExtensionsQueryID: return false;
   case GetExtensionTypeKindQueryID: return false;
   case MatchExtensionTypeQueryID: return false;
   case ExtensionAppliesQueryID: return false;
   case GetExtendedDeclQueryID: return false;
   case ResolveExtensionQueryID: return false;
   case ResolveConformanceToProtocolQueryID: return false;
   case ResolveExplicitConformancesQueryID: return false;
   case ResolveConformancesToProtocolsQueryID: return false;
   case ResolveDeclaredConformancesQueryID: return false;
   case AddSingleConformanceQueryID: return false;
   case DeclareSelfAliasQueryID: return false;
   case ReferencedAssociatedTypesReadyQueryID: return false;
   case CheckProtocolExtensionApplicabilityQueryID: return false;
   case VerifyConstraintQueryID: return false;
   case VerifyConstraintsQueryID: return false;
   case IsConstraintSatisfiedQueryID: return false;
   case GetConstrainedTypeQueryID: return false;
   case IsSupersetOfQueryID: return false;
   case GetReferencedAssociatedTypesQueryID: return false;
   case GetNeededAssociatedTypesQueryID: return false;
   case CheckConformancesQueryID: return false;
   case CheckSingleConformanceQueryID: return false;
   case CheckAssociatedTypeConstraintsQueryID: return false;
   case TypecheckConstraintsQueryID: return false;
   case ConformsToQueryID: return false;
   case IsCovariantQueryID: return false;
   case IsContravariantQueryID: return false;
   case GetTypeSizeQueryID: return false;
   case GetTypeAlignmentQueryID: return false;
   case GetTypeStrideQueryID: return false;
   case CalculateRecordSizeQueryID: return true;
   case GetRecordMetaQueryID: return false;
   case IsBuiltinIntegerTypeQueryID: return false;
   case IsBuiltinFloatingPointTypeQueryID: return false;
   case IsBuiltinBoolTypeQueryID: return false;
   case AddImplicitConformanceQueryID: return false;
   case GetImplicitConformanceQueryID: return false;
   case CheckBuiltinConformancesQueryID: return false;
   case IsAccessibleQueryID: return false;
   case CheckAccessibilityQueryID: return false;
   case ResolveNestedNameSpecToTypeQueryID: return false;
   case ResolveNestedNameSpecToDeclQueryID: return false;
   case TypeCheckDeclContextQueryID: return false;
   case PrepareDeclInterfaceQueryID: return false;
   case TypecheckDeclQueryID: return false;
   case PreparePrecedenceGroupInterfaceQueryID: return false;
   case TypecheckPrecedenceGroupQueryID: return false;
   case PrepareOperatorInterfaceQueryID: return false;
   case TypecheckOperatorQueryID: return false;
   case PrepareGlobalVarInterfaceQueryID: return false;
   case TypecheckGlobalVarQueryID: return false;
   case PrepareCallableInterfaceQueryID: return false;
   case TypecheckCallableQueryID: return false;
   case PrepareFunctionInterfaceQueryID: return false;
   case TypecheckFunctionQueryID: return false;
   case PrepareMethodInterfaceQueryID: return false;
   case TypecheckMethodQueryID: return false;
   case PrepareInitInterfaceQueryID: return false;
   case TypecheckInitQueryID: return false;
   case PrepareDeinitInterfaceQueryID: return false;
   case TypecheckDeinitQueryID: return false;
   case PrepareFuncArgInterfaceQueryID: return false;
   case TypecheckFuncArgQueryID: return false;
   case PrepareTemplateParamInterfaceQueryID: return false;
   case TypecheckTemplateParamQueryID: return false;
   case PrepareAssociatedTypeInterfaceQueryID: return false;
   case TypecheckAssociatedTypeQueryID: return false;
   case PrepareAliasInterfaceQueryID: return false;
   case TypecheckAliasQueryID: return false;
   case PrepareRecordInterfaceQueryID: return false;
   case TypecheckRecordQueryID: return false;
   case PrepareStructInterfaceQueryID: return false;
   case TypecheckStructQueryID: return false;
   case PrepareClassInterfaceQueryID: return false;
   case TypecheckClassQueryID: return false;
   case PrepareEnumInterfaceQueryID: return false;
   case TypecheckEnumQueryID: return false;
   case PrepareProtocolInterfaceQueryID: return false;
   case TypecheckProtocolQueryID: return false;
   case PrepareExtensionInterfaceQueryID: return false;
   case TypecheckExtensionQueryID: return false;
   case PrepareFieldInterfaceQueryID: return false;
   case TypecheckFieldQueryID: return false;
   case PreparePropInterfaceQueryID: return false;
   case TypecheckPropQueryID: return false;
   case PrepareSubscriptInterfaceQueryID: return false;
   case TypecheckSubscriptQueryID: return false;
   case PrepareEnumCaseInterfaceQueryID: return false;
   case TypecheckEnumCaseQueryID: return false;
   case PrepareNamespaceInterfaceQueryID: return false;
   case TypecheckNamespaceQueryID: return false;
   case PrepareCompoundInterfaceQueryID: return false;
   case TypecheckCompoundQueryID: return false;
   case PrepareModuleInterfaceQueryID: return false;
   case TypecheckModuleQueryID: return false;
   case PrepareTemplateParametersQueryID: return false;
   case DeclareImplicitInitializersQueryID: return false;
   case DeclareImplicitDefaultInitQueryID: return false;
   case DeclareMemberwiseInitQueryID: return false;
   case DeclareImplicitDefaultDeinitQueryID: return false;
   case ResolveRawTypeQueryID: return false;
   case AssignInitNameQueryID: return false;
   case CreateBaseInitQueryID: return false;
   case LookupFirstQueryID: return false;
   case LookupSingleQueryID: return false;
   case MultiLevelLookupQueryID: return false;
   case MultiLevelTypeLookupQueryID: return false;
   case RestrictedLookupQueryID: return false;
   case DirectLookupQueryID: return false;
   case NestedNameLookupQueryID: return false;
   case ResolveMacrosQueryID: return false;
   case ResolveStaticDeclarationsQueryID: return false;
   case ResolveWildcardDeclarationsQueryID: return false;
   case ResolveMetaDeclarationsQueryID: return false;
   case GetAssociatedTypeImplQueryID: return false;
   case GetAssociatedTypeDeclQueryID: return false;
   case CheckTypeEquivalenceQueryID: return false;
   case CheckTemplateExtensionApplicabilityQueryID: return false;
   case GetILModuleForDeclQueryID: return false;
   case GenerateILForDeclQueryID: return false;
   case GenerateILForContextQueryID: return false;
   case GenerateRecordILQueryID: return false;
   case GetILGlobalQueryID: return false;
   case GenerateLazyILGlobalDefinitionQueryID: return false;
   case GetILFunctionQueryID: return false;
   case GenerateILFunctionBodyQueryID: return false;
   case GetILTypeInfoQueryID: return false;
   case CreateILBasicTypeInfoQueryID: return false;
   case CreateILRecordTypeInfoQueryID: return false;
   case GetBoolValueQueryID: return false;
   case GetStringValueQueryID: return false;
   case GetIntValueQueryID: return false;
   case OpenFileForReadQueryID: return false;
   case OpenTmpFileQueryID: return false;
   case GetDefaultTemplateArgQueryID: return false;
   case CreateSelfArgumentQueryID: return false;
   case FindEquivalentDeclQueryID: return false;
   case EquivalentSignaturesQueryID: return false;
   }
}

bool Query::canBeDependent() const
{
   switch (K) {
   case CompileModuleQueryID: return false;
   case VerifyModuleQueryID: return false;
   case RunTestModuleQueryID: return false;
   case ParseModuleFileQueryID: return false;
   case CreateDefaultModuleQueryID: return false;
   case ParseSourceFileQueryID: return false;
   case ParseMainSourceFileQueryID: return false;
   case ParseSourceFilesQueryID: return false;
   case CreateILModuleQueryID: return false;
   case SetupIRGenQueryID: return false;
   case CreateLLVMModuleQueryID: return false;
   case CreateObjectQueryID: return false;
   case CreateStaticLibQueryID: return false;
   case CreateDynamicLibQueryID: return false;
   case CreateExecutableQueryID: return false;
   case EmitILQueryID: return false;
   case EmitIRQueryID: return false;
   case PrintUsedMemoryQueryID: return false;
   case MeasureExecutionTimeQueryID: return false;
   case IsPersistableQueryID: return true;
   case IsEquatableQueryID: return true;
   case IsCopyableQueryID: return true;
   case IsMoveOnlyQueryID: return true;
   case IsUnpopulatedQueryID: return true;
   case IsImplicitlyCopyableQueryID: return true;
   case IsTriviallyCopyableQueryID: return true;
   case NeedsRetainOrReleaseQueryID: return true;
   case NeedsDeinitilizationQueryID: return true;
   case NeedsStructReturnQueryID: return true;
   case PassByValueQueryID: return true;
   case ContainsTemplateQueryID: return true;
   case ContainsAssociatedTypeConstraintQueryID: return true;
   case ContainsProtocolWithAssociatedTypesQueryID: return true;
   case IsImplicitlyConvertibleQueryID: return true;
   case IsValidParameterValueQueryID: return false;
   case GetConversionSequenceQueryID: return true;
   case SubstTypeVariablesQueryID: return false;
   case SubstAssociatedTypesQueryID: return false;
   case SubstTemplateParamTypesQueryID: return false;
   case SubstTemplateParamTypesNonFinalQueryID: return false;
   case CheckTypeCapabilitiesQueryID: return false;
   case GetBuiltinModuleQueryID: return false;
   case GetBuiltinFuncQueryID: return false;
   case GetBuiltinAliasQueryID: return false;
   case GetBuiltinRecordQueryID: return false;
   case GetBuiltinProtocolQueryID: return false;
   case FindPrecedenceGroupQueryID: return false;
   case FindOperatorQueryID: return false;
   case ResolveImportQueryID: return false;
   case ResolveUsingQueryID: return false;
   case ResolveStaticIfQueryID: return false;
   case ResolveStaticForQueryID: return false;
   case ResolveStaticAssertQueryID: return false;
   case ResolveStaticPrintQueryID: return false;
   case ResolveStaticExprQueryID: return false;
   case ResolveStaticExprToBoolQueryID: return false;
   case ExpandMacroQueryID: return false;
   case ExpandMacroDeclQueryID: return false;
   case ExpandMacrosQueryID: return false;
   case ImportExternalSourceFileQueryID: return false;
   case FindExtensionsQueryID: return false;
   case GetExtensionTypeKindQueryID: return false;
   case MatchExtensionTypeQueryID: return false;
   case ExtensionAppliesQueryID: return false;
   case GetExtendedDeclQueryID: return false;
   case ResolveExtensionQueryID: return false;
   case ResolveConformanceToProtocolQueryID: return false;
   case ResolveExplicitConformancesQueryID: return false;
   case ResolveConformancesToProtocolsQueryID: return false;
   case ResolveDeclaredConformancesQueryID: return false;
   case AddSingleConformanceQueryID: return true;
   case DeclareSelfAliasQueryID: return false;
   case ReferencedAssociatedTypesReadyQueryID: return false;
   case CheckProtocolExtensionApplicabilityQueryID: return false;
   case VerifyConstraintQueryID: return false;
   case VerifyConstraintsQueryID: return false;
   case IsConstraintSatisfiedQueryID: return true;
   case GetConstrainedTypeQueryID: return true;
   case IsSupersetOfQueryID: return false;
   case GetReferencedAssociatedTypesQueryID: return false;
   case GetNeededAssociatedTypesQueryID: return false;
   case CheckConformancesQueryID: return false;
   case CheckSingleConformanceQueryID: return false;
   case CheckAssociatedTypeConstraintsQueryID: return false;
   case TypecheckConstraintsQueryID: return false;
   case ConformsToQueryID: return true;
   case IsCovariantQueryID: return true;
   case IsContravariantQueryID: return true;
   case GetTypeSizeQueryID: return true;
   case GetTypeAlignmentQueryID: return true;
   case GetTypeStrideQueryID: return true;
   case CalculateRecordSizeQueryID: return true;
   case GetRecordMetaQueryID: return false;
   case IsBuiltinIntegerTypeQueryID: return false;
   case IsBuiltinFloatingPointTypeQueryID: return false;
   case IsBuiltinBoolTypeQueryID: return false;
   case AddImplicitConformanceQueryID: return false;
   case GetImplicitConformanceQueryID: return false;
   case CheckBuiltinConformancesQueryID: return true;
   case IsAccessibleQueryID: return false;
   case CheckAccessibilityQueryID: return false;
   case ResolveNestedNameSpecToTypeQueryID: return false;
   case ResolveNestedNameSpecToDeclQueryID: return false;
   case TypeCheckDeclContextQueryID: return false;
   case PrepareDeclInterfaceQueryID: return false;
   case TypecheckDeclQueryID: return false;
   case PreparePrecedenceGroupInterfaceQueryID: return false;
   case TypecheckPrecedenceGroupQueryID: return false;
   case PrepareOperatorInterfaceQueryID: return false;
   case TypecheckOperatorQueryID: return false;
   case PrepareGlobalVarInterfaceQueryID: return false;
   case TypecheckGlobalVarQueryID: return false;
   case PrepareCallableInterfaceQueryID: return false;
   case TypecheckCallableQueryID: return false;
   case PrepareFunctionInterfaceQueryID: return false;
   case TypecheckFunctionQueryID: return false;
   case PrepareMethodInterfaceQueryID: return false;
   case TypecheckMethodQueryID: return false;
   case PrepareInitInterfaceQueryID: return false;
   case TypecheckInitQueryID: return false;
   case PrepareDeinitInterfaceQueryID: return false;
   case TypecheckDeinitQueryID: return false;
   case PrepareFuncArgInterfaceQueryID: return false;
   case TypecheckFuncArgQueryID: return false;
   case PrepareTemplateParamInterfaceQueryID: return false;
   case TypecheckTemplateParamQueryID: return false;
   case PrepareAssociatedTypeInterfaceQueryID: return false;
   case TypecheckAssociatedTypeQueryID: return false;
   case PrepareAliasInterfaceQueryID: return false;
   case TypecheckAliasQueryID: return false;
   case PrepareRecordInterfaceQueryID: return false;
   case TypecheckRecordQueryID: return false;
   case PrepareStructInterfaceQueryID: return false;
   case TypecheckStructQueryID: return false;
   case PrepareClassInterfaceQueryID: return false;
   case TypecheckClassQueryID: return false;
   case PrepareEnumInterfaceQueryID: return false;
   case TypecheckEnumQueryID: return false;
   case PrepareProtocolInterfaceQueryID: return false;
   case TypecheckProtocolQueryID: return false;
   case PrepareExtensionInterfaceQueryID: return false;
   case TypecheckExtensionQueryID: return false;
   case PrepareFieldInterfaceQueryID: return false;
   case TypecheckFieldQueryID: return false;
   case PreparePropInterfaceQueryID: return false;
   case TypecheckPropQueryID: return false;
   case PrepareSubscriptInterfaceQueryID: return false;
   case TypecheckSubscriptQueryID: return false;
   case PrepareEnumCaseInterfaceQueryID: return false;
   case TypecheckEnumCaseQueryID: return false;
   case PrepareNamespaceInterfaceQueryID: return false;
   case TypecheckNamespaceQueryID: return false;
   case PrepareCompoundInterfaceQueryID: return false;
   case TypecheckCompoundQueryID: return false;
   case PrepareModuleInterfaceQueryID: return false;
   case TypecheckModuleQueryID: return false;
   case PrepareTemplateParametersQueryID: return false;
   case DeclareImplicitInitializersQueryID: return false;
   case DeclareImplicitDefaultInitQueryID: return false;
   case DeclareMemberwiseInitQueryID: return false;
   case DeclareImplicitDefaultDeinitQueryID: return false;
   case ResolveRawTypeQueryID: return false;
   case AssignInitNameQueryID: return false;
   case CreateBaseInitQueryID: return false;
   case LookupFirstQueryID: return false;
   case LookupSingleQueryID: return false;
   case MultiLevelLookupQueryID: return false;
   case MultiLevelTypeLookupQueryID: return false;
   case RestrictedLookupQueryID: return false;
   case DirectLookupQueryID: return false;
   case NestedNameLookupQueryID: return false;
   case ResolveMacrosQueryID: return false;
   case ResolveStaticDeclarationsQueryID: return false;
   case ResolveWildcardDeclarationsQueryID: return false;
   case ResolveMetaDeclarationsQueryID: return false;
   case GetAssociatedTypeImplQueryID: return false;
   case GetAssociatedTypeDeclQueryID: return false;
   case CheckTypeEquivalenceQueryID: return false;
   case CheckTemplateExtensionApplicabilityQueryID: return false;
   case GetILModuleForDeclQueryID: return false;
   case GenerateILForDeclQueryID: return false;
   case GenerateILForContextQueryID: return false;
   case GenerateRecordILQueryID: return false;
   case GetILGlobalQueryID: return false;
   case GenerateLazyILGlobalDefinitionQueryID: return false;
   case GetILFunctionQueryID: return false;
   case GenerateILFunctionBodyQueryID: return false;
   case GetILTypeInfoQueryID: return false;
   case CreateILBasicTypeInfoQueryID: return false;
   case CreateILRecordTypeInfoQueryID: return false;
   case GetBoolValueQueryID: return false;
   case GetStringValueQueryID: return false;
   case GetIntValueQueryID: return false;
   case OpenFileForReadQueryID: return false;
   case OpenTmpFileQueryID: return false;
   case GetDefaultTemplateArgQueryID: return false;
   case CreateSelfArgumentQueryID: return false;
   case FindEquivalentDeclQueryID: return false;
   case EquivalentSignaturesQueryID: return false;
   }
}

CompileModuleQuery::CompileModuleQuery(QueryContext &QC) : Query(CompileModuleQueryID, QC) {

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

   OS << "[CompileModule]"
;;

   return OS.str();
}

VerifyModuleQuery::VerifyModuleQuery(QueryContext &QC) : Query(VerifyModuleQueryID, QC) {

}

std::string VerifyModuleQuery::description() const
{
   std::string s;
   s += "verifying module ";
   s += QC.CI.getCompilationModule()->getName()->getIdentifier();
   return s;
}

std::string VerifyModuleQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[VerifyModule]"
;;

   return OS.str();
}

RunTestModuleQuery::RunTestModuleQuery(QueryContext &QC, Module* Mod) : Query(RunTestModuleQueryID, QC), Mod(Mod) {

}

std::string RunTestModuleQuery::description() const
{
   std::string s;
   s += "running ";
   s += "RunTestModule";
   return s;
}

std::string RunTestModuleQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[RunTestModule]"
;   OS << " Mod = ";
OS << Mod;
;

   return OS.str();
}

void RunTestModuleQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Mod);
}

void RunTestModuleQuery::Profile(llvm::FoldingSetNodeID &ID, Module* Mod)
{
   ID.AddPointer(Mod);
}

ParseModuleFileQuery::ParseModuleFileQuery(QueryContext &QC, StringRef FileName) : Query(ParseModuleFileQueryID, QC), FileName(FileName) {

}

Module* ParseModuleFileQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ParseModuleFileQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ParseModuleFile";
   return s;
}

std::string ParseModuleFileQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ParseModuleFile]"
;   OS << " FileName = ";
OS << "'" << FileName << "'";
;

   return OS.str();
}

QueryResult ParseModuleFileQuery::finish(Module* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ParseModuleFileQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, FileName);
}

void ParseModuleFileQuery::Profile(llvm::FoldingSetNodeID &ID, StringRef FileName)
{
   ID.AddString(FileName);
}

CreateDefaultModuleQuery::CreateDefaultModuleQuery(QueryContext &QC) : Query(CreateDefaultModuleQueryID, QC) {

}

Module* CreateDefaultModuleQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string CreateDefaultModuleQuery::description() const
{
   std::string s;
   s += "running ";
   s += "CreateDefaultModule";
   return s;
}

std::string CreateDefaultModuleQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CreateDefaultModule]"
;;

   return OS.str();
}

QueryResult CreateDefaultModuleQuery::finish(Module* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

ParseSourceFileQuery::ParseSourceFileQuery(QueryContext &QC, Module* Mod, StringRef FileName) : Query(ParseSourceFileQueryID, QC), Mod(Mod), FileName(FileName) {

}

ast::SourceFileDecl* ParseSourceFileQuery::get() const
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

   OS << "[ParseSourceFile]"
;   OS << " Mod = ";
OS << Mod;
   OS << ',';
   OS << " FileName = ";
OS << "'" << FileName << "'";
;

   return OS.str();
}

QueryResult ParseSourceFileQuery::finish(ast::SourceFileDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ParseSourceFileQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Mod, FileName);
}

void ParseSourceFileQuery::Profile(llvm::FoldingSetNodeID &ID, Module* Mod, StringRef FileName)
{
   ID.AddPointer(Mod);
   ID.AddString(FileName);
}

ParseMainSourceFileQuery::ParseMainSourceFileQuery(QueryContext &QC, Module* Mod, StringRef FileName) : Query(ParseMainSourceFileQueryID, QC), Mod(Mod), FileName(FileName) {

}

ast::SourceFileDecl* ParseMainSourceFileQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ParseMainSourceFileQuery::description() const
{
   std::string s;
   s += "parsing source file ";
   s += this->FileName;
   return s;
}

std::string ParseMainSourceFileQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ParseMainSourceFile]"
;   OS << " Mod = ";
OS << Mod;
   OS << ',';
   OS << " FileName = ";
OS << "'" << FileName << "'";
;

   return OS.str();
}

QueryResult ParseMainSourceFileQuery::finish(ast::SourceFileDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ParseMainSourceFileQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Mod, FileName);
}

void ParseMainSourceFileQuery::Profile(llvm::FoldingSetNodeID &ID, Module* Mod, StringRef FileName)
{
   ID.AddPointer(Mod);
   ID.AddString(FileName);
}

ParseSourceFilesQuery::ParseSourceFilesQuery(QueryContext &QC, Module* Mod) : Query(ParseSourceFilesQueryID, QC), Mod(Mod) {

}

std::string ParseSourceFilesQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ParseSourceFiles";
   return s;
}

std::string ParseSourceFilesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ParseSourceFiles]"
;   OS << " Mod = ";
OS << Mod;
;

   return OS.str();
}

void ParseSourceFilesQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Mod);
}

void ParseSourceFilesQuery::Profile(llvm::FoldingSetNodeID &ID, Module* Mod)
{
   ID.AddPointer(Mod);
}

CreateILModuleQuery::CreateILModuleQuery(QueryContext &QC, Module* Mod) : Query(CreateILModuleQueryID, QC), Mod(Mod) {

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

   OS << "[CreateILModule]"
;   OS << " Mod = ";
OS << Mod;
;

   return OS.str();
}

QueryResult CreateILModuleQuery::finish(il::Module* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CreateILModuleQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Mod);
}

void CreateILModuleQuery::Profile(llvm::FoldingSetNodeID &ID, Module* Mod)
{
   ID.AddPointer(Mod);
}

SetupIRGenQuery::SetupIRGenQuery(QueryContext &QC) : Query(SetupIRGenQueryID, QC) {

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

   OS << "[SetupIRGen]"
;;

   return OS.str();
}

QueryResult SetupIRGenQuery::finish(std::unique_ptr<il::IRGen> &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

CreateLLVMModuleQuery::CreateLLVMModuleQuery(QueryContext &QC, Module* Mod) : Query(CreateLLVMModuleQueryID, QC), Mod(Mod) {

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

   OS << "[CreateLLVMModule]"
;   OS << " Mod = ";
OS << Mod;
;

   return OS.str();
}

QueryResult CreateLLVMModuleQuery::finish(llvm::Module* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CreateLLVMModuleQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Mod);
}

void CreateLLVMModuleQuery::Profile(llvm::FoldingSetNodeID &ID, Module* Mod)
{
   ID.AddPointer(Mod);
}

CreateObjectQuery::CreateObjectQuery(QueryContext &QC, llvm::raw_ostream& OS) : Query(CreateObjectQueryID, QC), OS(OS) {

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

   OS << "[CreateObject]"
;   OS << " OS = ";
OS << &OS;
;

   return OS.str();
}

CreateStaticLibQuery::CreateStaticLibQuery(QueryContext &QC, StringRef OutFile) : Query(CreateStaticLibQueryID, QC), OutFile(OutFile) {

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

   OS << "[CreateStaticLib]"
;   OS << " OutFile = ";
OS << "'" << OutFile << "'";
;

   return OS.str();
}

CreateDynamicLibQuery::CreateDynamicLibQuery(QueryContext &QC, StringRef OutFile) : Query(CreateDynamicLibQueryID, QC), OutFile(OutFile) {

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

   OS << "[CreateDynamicLib]"
;   OS << " OutFile = ";
OS << "'" << OutFile << "'";
;

   return OS.str();
}

CreateExecutableQuery::CreateExecutableQuery(QueryContext &QC, StringRef OutFile) : Query(CreateExecutableQueryID, QC), OutFile(OutFile) {

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

   OS << "[CreateExecutable]"
;   OS << " OutFile = ";
OS << "'" << OutFile << "'";
;

   return OS.str();
}

EmitILQuery::EmitILQuery(QueryContext &QC, llvm::raw_ostream& OS) : Query(EmitILQueryID, QC), OS(OS) {

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

   OS << "[EmitIL]"
;   OS << " OS = ";
OS << &OS;
;

   return OS.str();
}

EmitIRQuery::EmitIRQuery(QueryContext &QC, llvm::raw_ostream& OS) : Query(EmitIRQueryID, QC), OS(OS) {

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

   OS << "[EmitIR]"
;   OS << " OS = ";
OS << &OS;
;

   return OS.str();
}

PrintUsedMemoryQuery::PrintUsedMemoryQuery(QueryContext &QC) : Query(PrintUsedMemoryQueryID, QC) {

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

   OS << "[PrintUsedMemory]"
;;

   return OS.str();
}

MeasureExecutionTimeQuery::MeasureExecutionTimeQuery(QueryContext &QC, Query* Q) : Query(MeasureExecutionTimeQueryID, QC), Q(Q) {
assert(Q && "parameter 'Q' should not be null!");

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

   OS << "[MeasureExecutionTime]"
;   OS << " Q = ";
OS << Q;
;

   return OS.str();
}

QueryResult MeasureExecutionTimeQuery::finish(long long Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void MeasureExecutionTimeQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Q);
}

void MeasureExecutionTimeQuery::Profile(llvm::FoldingSetNodeID &ID, Query* Q)
{
   ID.AddPointer(Q);
}

IsPersistableQuery::IsPersistableQuery(QueryContext &QC, CanType T) : Query(IsPersistableQueryID, QC), T(T) {

}

bool IsPersistableQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsPersistableQuery::description() const
{
   std::string s;
   s += "checking ";
   s += "IsPersistable";
   s += " on ";
   s += T.toDiagString();
   return s;
}

std::string IsPersistableQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsPersistable]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult IsPersistableQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsPersistableQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void IsPersistableQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

IsEquatableQuery::IsEquatableQuery(QueryContext &QC, CanType T) : Query(IsEquatableQueryID, QC), T(T) {

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

   OS << "[IsEquatable]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult IsEquatableQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsEquatableQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void IsEquatableQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

IsCopyableQuery::IsCopyableQuery(QueryContext &QC, CanType T) : Query(IsCopyableQueryID, QC), T(T) {

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

   OS << "[IsCopyable]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult IsCopyableQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsCopyableQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void IsCopyableQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

IsMoveOnlyQuery::IsMoveOnlyQuery(QueryContext &QC, CanType T) : Query(IsMoveOnlyQueryID, QC), T(T) {

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

   OS << "[IsMoveOnly]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult IsMoveOnlyQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsMoveOnlyQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void IsMoveOnlyQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

IsUnpopulatedQuery::IsUnpopulatedQuery(QueryContext &QC, CanType T) : Query(IsUnpopulatedQueryID, QC), T(T) {

}

bool IsUnpopulatedQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsUnpopulatedQuery::description() const
{
   std::string s;
   s += "checking ";
   s += "IsUnpopulated";
   s += " on ";
   s += T.toDiagString();
   return s;
}

std::string IsUnpopulatedQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsUnpopulated]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult IsUnpopulatedQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsUnpopulatedQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void IsUnpopulatedQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

IsImplicitlyCopyableQuery::IsImplicitlyCopyableQuery(QueryContext &QC, CanType T) : Query(IsImplicitlyCopyableQueryID, QC), T(T) {

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

   OS << "[IsImplicitlyCopyable]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult IsImplicitlyCopyableQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsImplicitlyCopyableQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void IsImplicitlyCopyableQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

IsTriviallyCopyableQuery::IsTriviallyCopyableQuery(QueryContext &QC, CanType T) : Query(IsTriviallyCopyableQueryID, QC), T(T) {

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

   OS << "[IsTriviallyCopyable]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult IsTriviallyCopyableQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsTriviallyCopyableQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void IsTriviallyCopyableQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

NeedsRetainOrReleaseQuery::NeedsRetainOrReleaseQuery(QueryContext &QC, CanType T) : Query(NeedsRetainOrReleaseQueryID, QC), T(T) {

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

   OS << "[NeedsRetainOrRelease]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult NeedsRetainOrReleaseQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void NeedsRetainOrReleaseQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void NeedsRetainOrReleaseQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

NeedsDeinitilizationQuery::NeedsDeinitilizationQuery(QueryContext &QC, CanType T) : Query(NeedsDeinitilizationQueryID, QC), T(T) {

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

   OS << "[NeedsDeinitilization]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult NeedsDeinitilizationQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void NeedsDeinitilizationQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void NeedsDeinitilizationQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

NeedsStructReturnQuery::NeedsStructReturnQuery(QueryContext &QC, CanType T) : Query(NeedsStructReturnQueryID, QC), T(T) {

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

   OS << "[NeedsStructReturn]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult NeedsStructReturnQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void NeedsStructReturnQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void NeedsStructReturnQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

PassByValueQuery::PassByValueQuery(QueryContext &QC, CanType T) : Query(PassByValueQueryID, QC), T(T) {

}

bool PassByValueQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string PassByValueQuery::description() const
{
   std::string s;
   s += "checking ";
   s += "PassByValue";
   s += " on ";
   s += T.toDiagString();
   return s;
}

std::string PassByValueQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PassByValue]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult PassByValueQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void PassByValueQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void PassByValueQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

ContainsTemplateQuery::ContainsTemplateQuery(QueryContext &QC, CanType T) : Query(ContainsTemplateQueryID, QC), T(T) {

}

bool ContainsTemplateQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ContainsTemplateQuery::description() const
{
   std::string s;
   s += "checking ";
   s += "ContainsTemplate";
   s += " on ";
   s += T.toDiagString();
   return s;
}

std::string ContainsTemplateQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ContainsTemplate]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult ContainsTemplateQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ContainsTemplateQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void ContainsTemplateQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

ContainsAssociatedTypeConstraintQuery::ContainsAssociatedTypeConstraintQuery(QueryContext &QC, CanType T) : Query(ContainsAssociatedTypeConstraintQueryID, QC), T(T) {

}

bool ContainsAssociatedTypeConstraintQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ContainsAssociatedTypeConstraintQuery::description() const
{
   std::string s;
   s += "checking ";
   s += "ContainsAssociatedTypeConstraint";
   s += " on ";
   s += T.toDiagString();
   return s;
}

std::string ContainsAssociatedTypeConstraintQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ContainsAssociatedTypeConstraint]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult ContainsAssociatedTypeConstraintQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ContainsAssociatedTypeConstraintQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void ContainsAssociatedTypeConstraintQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

ContainsProtocolWithAssociatedTypesQuery::ContainsProtocolWithAssociatedTypesQuery(QueryContext &QC, QualType T) : Query(ContainsProtocolWithAssociatedTypesQueryID, QC), T(T) {

}

ast::ProtocolDecl* ContainsProtocolWithAssociatedTypesQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ContainsProtocolWithAssociatedTypesQuery::description() const
{
   std::string s;
   s += "checking ";
   s += "ContainsProtocolWithAssociatedTypes";
   s += " on ";
   s += T.toDiagString();
   return s;
}

std::string ContainsProtocolWithAssociatedTypesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ContainsProtocolWithAssociatedTypes]"
;   OS << " T = ";
OS << "'" << T << "'";
;

   return OS.str();
}

QueryResult ContainsProtocolWithAssociatedTypesQuery::finish(ast::ProtocolDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ContainsProtocolWithAssociatedTypesQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void ContainsProtocolWithAssociatedTypesQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

IsImplicitlyConvertibleQuery::IsImplicitlyConvertibleQuery(QueryContext &QC, QualType From, QualType To, uint8_t flags) : Query(IsImplicitlyConvertibleQueryID, QC), From(From), To(To), flags(flags) {

}

IsImplicitlyConvertibleQuery::ResultType IsImplicitlyConvertibleQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsImplicitlyConvertibleQuery::description() const
{
   std::string s;
   s += "running ";
   s += "IsImplicitlyConvertible";
   return s;
}

std::string IsImplicitlyConvertibleQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsImplicitlyConvertible]"
;   OS << " From = ";
OS << "'" << From << "'";
   OS << ',';
   OS << " To = ";
OS << "'" << To << "'";
   OS << ',';
   OS << " flags = ";
OS << flags;
;

   return OS.str();
}

QueryResult IsImplicitlyConvertibleQuery::finish(IsImplicitlyConvertibleQuery::ResultType Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsImplicitlyConvertibleQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, From, To, flags);
}

void IsImplicitlyConvertibleQuery::Profile(llvm::FoldingSetNodeID &ID, QualType From, QualType To, uint8_t flags)
{
   ID.AddPointer(From.getAsOpaquePtr());
   ID.AddPointer(To.getAsOpaquePtr());
   ID.AddInteger(flags);
}

IsValidParameterValueQuery::IsValidParameterValueQuery(QueryContext &QC, QualType givenType, QualType paramType, bool isSelf, bool importedFromClang) : Query(IsValidParameterValueQueryID, QC), givenType(givenType), paramType(paramType), isSelf(isSelf), importedFromClang(importedFromClang) {

}

IsValidParameterValueQuery::ResultType IsValidParameterValueQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsValidParameterValueQuery::description() const
{
   std::string s;
   s += "running ";
   s += "IsValidParameterValue";
   return s;
}

std::string IsValidParameterValueQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsValidParameterValue]"
;   OS << " givenType = ";
OS << "'" << givenType << "'";
   OS << ',';
   OS << " paramType = ";
OS << "'" << paramType << "'";
   OS << ',';
   OS << " isSelf = ";
OS << isSelf;
   OS << ',';
   OS << " importedFromClang = ";
OS << importedFromClang;
;

   return OS.str();
}

QueryResult IsValidParameterValueQuery::finish(IsValidParameterValueQuery::ResultType Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsValidParameterValueQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, givenType, paramType, isSelf, importedFromClang);
}

void IsValidParameterValueQuery::Profile(llvm::FoldingSetNodeID &ID, QualType givenType, QualType paramType, bool isSelf, bool importedFromClang)
{
   ID.AddPointer(givenType.getAsOpaquePtr());
   ID.AddPointer(paramType.getAsOpaquePtr());
   ID.AddInteger(isSelf);
   ID.AddInteger(importedFromClang);
}

GetConversionSequenceQuery::GetConversionSequenceQuery(QueryContext &QC, QualType From, QualType To, uint8_t flags) : Query(GetConversionSequenceQueryID, QC), From(From), To(To), flags(flags) {

}

ConversionSequence* GetConversionSequenceQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetConversionSequenceQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetConversionSequence";
   return s;
}

std::string GetConversionSequenceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetConversionSequence]"
;   OS << " From = ";
OS << "'" << From << "'";
   OS << ',';
   OS << " To = ";
OS << "'" << To << "'";
   OS << ',';
   OS << " flags = ";
OS << flags;
;

   return OS.str();
}

QueryResult GetConversionSequenceQuery::finish(ConversionSequence* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetConversionSequenceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, From, To, flags);
}

void GetConversionSequenceQuery::Profile(llvm::FoldingSetNodeID &ID, QualType From, QualType To, uint8_t flags)
{
   ID.AddPointer(From.getAsOpaquePtr());
   ID.AddPointer(To.getAsOpaquePtr());
   ID.AddInteger(flags);
}

SubstTypeVariablesQuery::SubstTypeVariablesQuery(QueryContext &QC, QualType T, const llvm::DenseMap<TypeVariableType*, QualType>& ReplacementMap, SourceRange SR) : Query(SubstTypeVariablesQueryID, QC), T(T), ReplacementMap(ReplacementMap), SR(SR) {

}

QualType SubstTypeVariablesQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string SubstTypeVariablesQuery::description() const
{
   std::string s;
   s += "running ";
   s += "SubstTypeVariables";
   return s;
}

std::string SubstTypeVariablesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[SubstTypeVariables]"
;   OS << " T = ";
OS << "'" << T << "'";
;

   return OS.str();
}

QueryResult SubstTypeVariablesQuery::finish(QualType Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

SubstAssociatedTypesQuery::SubstAssociatedTypesQuery(QueryContext &QC, QualType T, QualType Self, SourceRange SR) : Query(SubstAssociatedTypesQueryID, QC), T(T), Self(Self), SR(SR) {

}

void SubstAssociatedTypesQuery::refresh()
{

   if (Stat == Idle) return;
   if (
        Self && Self->isRecordType() && QC.Sema->IsBeingResolved(Self->getRecord())
    ) {
      Stat = Idle;
      
   }
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

   OS << "[SubstAssociatedTypes]"
;   OS << " T = ";
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

void SubstAssociatedTypesQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T, Self);
}

void SubstAssociatedTypesQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T, QualType Self)
{
   ID.AddPointer(T.getAsOpaquePtr());
   ID.AddPointer(Self.getAsOpaquePtr());
}

SubstTemplateParamTypesQuery::SubstTemplateParamTypesQuery(QueryContext &QC, QualType T, sema::MultiLevelFinalTemplateArgList TemplateArgs, SourceRange SR) : Query(SubstTemplateParamTypesQueryID, QC), T(T), TemplateArgs(std::move(TemplateArgs)), SR(SR) {

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

   OS << "[SubstTemplateParamTypes]"
;   OS << " T = ";
OS << "'" << T << "'";
   OS << ',';
   OS << " TemplateArgs = ";
TemplateArgs.print(OS);
;

   return OS.str();
}

QueryResult SubstTemplateParamTypesQuery::finish(QualType Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void SubstTemplateParamTypesQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T, TemplateArgs);
}

void SubstTemplateParamTypesQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T, const sema::MultiLevelFinalTemplateArgList &TemplateArgs)
{
   ID.AddPointer(T.getAsOpaquePtr());
   TemplateArgs.Profile(ID);
}

SubstTemplateParamTypesNonFinalQuery::SubstTemplateParamTypesNonFinalQuery(QueryContext &QC, QualType T, const sema::MultiLevelTemplateArgList& TemplateArgs, SourceRange SR) : Query(SubstTemplateParamTypesNonFinalQueryID, QC), T(T), TemplateArgs(TemplateArgs), SR(SR) {

}

QualType SubstTemplateParamTypesNonFinalQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string SubstTemplateParamTypesNonFinalQuery::description() const
{
   std::string s;
   s += "substituting ";
   s += T.toDiagString();
   s += " with TemplateArgs = ";
   s += TemplateArgs.toString();
   return s;
}

std::string SubstTemplateParamTypesNonFinalQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[SubstTemplateParamTypesNonFinal]"
;   OS << " T = ";
OS << "'" << T << "'";
   OS << ',';
   OS << " TemplateArgs = ";
OS << &TemplateArgs;
;

   return OS.str();
}

QueryResult SubstTemplateParamTypesNonFinalQuery::finish(QualType Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

CheckTypeCapabilitiesQuery::CheckTypeCapabilitiesQuery(QueryContext &QC, ast::NamedDecl* ND) : Query(CheckTypeCapabilitiesQueryID, QC), ND(ND) {
assert(ND && "parameter 'ND' should not be null!");

}

ArrayRef<TypeCapability> CheckTypeCapabilitiesQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string CheckTypeCapabilitiesQuery::description() const
{
   std::string s;
   s += "running ";
   s += "CheckTypeCapabilities";
   return s;
}

std::string CheckTypeCapabilitiesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CheckTypeCapabilities]"
;   OS << " ND = ";
OS << "'" << (ND ? ND->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult CheckTypeCapabilitiesQuery::finish(std::vector<TypeCapability> &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void CheckTypeCapabilitiesQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, ND);
}

void CheckTypeCapabilitiesQuery::Profile(llvm::FoldingSetNodeID &ID, ast::NamedDecl* ND)
{
   ID.AddPointer(ND);
}

GetBuiltinModuleQuery::GetBuiltinModuleQuery(QueryContext &QC, GetBuiltinModuleQuery::ModuleKind Mod) : Query(GetBuiltinModuleQueryID, QC), Mod(Mod) {

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

   OS << "[GetBuiltinModule]"
;   OS << " Mod = ";
OS << (uint64_t)Mod;
;

   return OS.str();
}

QueryResult GetBuiltinModuleQuery::finish(Module* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetBuiltinModuleQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Mod);
}

void GetBuiltinModuleQuery::Profile(llvm::FoldingSetNodeID &ID, GetBuiltinModuleQuery::ModuleKind Mod)
{
   ID.AddInteger((uint64_t)Mod);
}

GetBuiltinFuncQuery::GetBuiltinFuncQuery(QueryContext &QC, GetBuiltinFuncQuery::FunctionKind Fn) : Query(GetBuiltinFuncQueryID, QC), Fn(Fn) {

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

   OS << "[GetBuiltinFunc]"
;   OS << " Fn = ";
OS << (uint64_t)Fn;
;

   return OS.str();
}

QueryResult GetBuiltinFuncQuery::finish(ast::CallableDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetBuiltinFuncQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Fn);
}

void GetBuiltinFuncQuery::Profile(llvm::FoldingSetNodeID &ID, GetBuiltinFuncQuery::FunctionKind Fn)
{
   ID.AddInteger((uint64_t)Fn);
}

GetBuiltinAliasQuery::GetBuiltinAliasQuery(QueryContext &QC, GetBuiltinAliasQuery::AliasKind K, ast::DeclContext* DC) : Query(GetBuiltinAliasQueryID, QC), K(K), DC(DC) {

}

ast::AliasDecl* GetBuiltinAliasQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetBuiltinAliasQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetBuiltinAlias";
   return s;
}

std::string GetBuiltinAliasQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetBuiltinAlias]"
;   OS << " K = ";
OS << (uint64_t)K;
   OS << ',';
   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult GetBuiltinAliasQuery::finish(ast::AliasDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetBuiltinAliasQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, K, DC);
}

void GetBuiltinAliasQuery::Profile(llvm::FoldingSetNodeID &ID, GetBuiltinAliasQuery::AliasKind K, ast::DeclContext* DC)
{
   ID.AddInteger((uint64_t)K);
   ID.AddPointer(DC);
}

GetBuiltinRecordQuery::GetBuiltinRecordQuery(QueryContext &QC, GetBuiltinRecordQuery::RecordKind R) : Query(GetBuiltinRecordQueryID, QC), R(R) {

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

   OS << "[GetBuiltinRecord]"
;   OS << " R = ";
OS << (uint64_t)R;
;

   return OS.str();
}

QueryResult GetBuiltinRecordQuery::finish(ast::RecordDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetBuiltinRecordQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, R);
}

void GetBuiltinRecordQuery::Profile(llvm::FoldingSetNodeID &ID, GetBuiltinRecordQuery::RecordKind R)
{
   ID.AddInteger((uint64_t)R);
}

GetBuiltinProtocolQuery::GetBuiltinProtocolQuery(QueryContext &QC, GetBuiltinProtocolQuery::ProtocolKind P) : Query(GetBuiltinProtocolQueryID, QC), P(P) {

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

   OS << "[GetBuiltinProtocol]"
;   OS << " P = ";
OS << (uint64_t)P;
;

   return OS.str();
}

QueryResult GetBuiltinProtocolQuery::finish(ast::ProtocolDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetBuiltinProtocolQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, P);
}

void GetBuiltinProtocolQuery::Profile(llvm::FoldingSetNodeID &ID, GetBuiltinProtocolQuery::ProtocolKind P)
{
   ID.AddInteger((uint64_t)P);
}

FindPrecedenceGroupQuery::FindPrecedenceGroupQuery(QueryContext &QC, DeclarationName Name, bool DiagOnError, SourceRange Loc) : Query(FindPrecedenceGroupQueryID, QC), Name(Name), DiagOnError(DiagOnError), Loc(Loc) {

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

   OS << "[FindPrecedenceGroup]"
;   OS << " Name = ";
OS << "'" << Name << "'";
   OS << ',';
   OS << " DiagOnError = ";
OS << DiagOnError;
;

   return OS.str();
}

QueryResult FindPrecedenceGroupQuery::finish(ast::PrecedenceGroupDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void FindPrecedenceGroupQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Name, DiagOnError);
}

void FindPrecedenceGroupQuery::Profile(llvm::FoldingSetNodeID &ID, DeclarationName Name, bool DiagOnError)
{
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddInteger(DiagOnError);
}

FindOperatorQuery::FindOperatorQuery(QueryContext &QC, DeclarationName Name, bool DiagOnError, SourceRange Loc) : Query(FindOperatorQueryID, QC), Name(Name), DiagOnError(DiagOnError), Loc(Loc) {

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

   OS << "[FindOperator]"
;   OS << " Name = ";
OS << "'" << Name << "'";
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

void FindOperatorQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Name, DiagOnError);
}

void FindOperatorQuery::Profile(llvm::FoldingSetNodeID &ID, DeclarationName Name, bool DiagOnError)
{
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddInteger(DiagOnError);
}

ResolveImportQuery::ResolveImportQuery(QueryContext &QC, ast::ImportDecl* I, llvm::SmallVectorImpl<ast::NamedDecl*>* FoundDecls) : Query(ResolveImportQueryID, QC), I(I), FoundDecls(FoundDecls) {

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

   OS << "[ResolveImport]"
;   OS << " I = ";
OS << "'" << (I ? I->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void ResolveImportQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, I);
}

void ResolveImportQuery::Profile(llvm::FoldingSetNodeID &ID, ast::ImportDecl* I)
{
   ID.AddPointer(I);
}

ResolveUsingQuery::ResolveUsingQuery(QueryContext &QC, ast::UsingDecl* U, llvm::SmallVectorImpl<ast::NamedDecl*>* FoundDecls) : Query(ResolveUsingQueryID, QC), U(U), FoundDecls(FoundDecls) {

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

   OS << "[ResolveUsing]"
;   OS << " U = ";
OS << "'" << (U ? U->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void ResolveUsingQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, U);
}

void ResolveUsingQuery::Profile(llvm::FoldingSetNodeID &ID, ast::UsingDecl* U)
{
   ID.AddPointer(U);
}

ResolveStaticIfQuery::ResolveStaticIfQuery(QueryContext &QC, ast::StaticIfDecl* Decl) : Query(ResolveStaticIfQueryID, QC), Decl(Decl) {

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

   OS << "[ResolveStaticIf]"
;   OS << " Decl = ";
OS << "'" << (Decl ? Decl->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult ResolveStaticIfQuery::finish(ast::Decl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ResolveStaticIfQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Decl);
}

void ResolveStaticIfQuery::Profile(llvm::FoldingSetNodeID &ID, ast::StaticIfDecl* Decl)
{
   ID.AddPointer(Decl);
}

ResolveStaticForQuery::ResolveStaticForQuery(QueryContext &QC, ast::StaticForDecl* Decl) : Query(ResolveStaticForQueryID, QC), Decl(Decl) {

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

   OS << "[ResolveStaticFor]"
;   OS << " Decl = ";
OS << "'" << (Decl ? Decl->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult ResolveStaticForQuery::finish(ast::Decl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ResolveStaticForQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Decl);
}

void ResolveStaticForQuery::Profile(llvm::FoldingSetNodeID &ID, ast::StaticForDecl* Decl)
{
   ID.AddPointer(Decl);
}

ResolveStaticAssertQuery::ResolveStaticAssertQuery(QueryContext &QC, ast::StaticAssertDecl* Decl) : Query(ResolveStaticAssertQueryID, QC), Decl(Decl) {

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

   OS << "[ResolveStaticAssert]"
;   OS << " Decl = ";
OS << "'" << (Decl ? Decl->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void ResolveStaticAssertQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Decl);
}

void ResolveStaticAssertQuery::Profile(llvm::FoldingSetNodeID &ID, ast::StaticAssertDecl* Decl)
{
   ID.AddPointer(Decl);
}

ResolveStaticPrintQuery::ResolveStaticPrintQuery(QueryContext &QC, ast::StaticPrintDecl* Decl) : Query(ResolveStaticPrintQueryID, QC), Decl(Decl) {

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

   OS << "[ResolveStaticPrint]"
;   OS << " Decl = ";
OS << "'" << (Decl ? Decl->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void ResolveStaticPrintQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Decl);
}

void ResolveStaticPrintQuery::Profile(llvm::FoldingSetNodeID &ID, ast::StaticPrintDecl* Decl)
{
   ID.AddPointer(Decl);
}

ResolveStaticExprQuery::ResolveStaticExprQuery(QueryContext &QC, ast::StaticExpr* Expr) : Query(ResolveStaticExprQueryID, QC), Expr(Expr) {

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

   OS << "[ResolveStaticExpr]"
;   OS << " Expr = ";
OS << Expr;
;

   return OS.str();
}

QueryResult ResolveStaticExprQuery::finish(il::Constant* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ResolveStaticExprQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Expr);
}

void ResolveStaticExprQuery::Profile(llvm::FoldingSetNodeID &ID, ast::StaticExpr* Expr)
{
   ID.AddPointer(Expr);
}

ResolveStaticExprToBoolQuery::ResolveStaticExprToBoolQuery(QueryContext &QC, ast::StaticExpr* Expr, bool DiagOnError) : Query(ResolveStaticExprToBoolQueryID, QC), Expr(Expr), DiagOnError(DiagOnError) {

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

   OS << "[ResolveStaticExprToBool]"
;   OS << " Expr = ";
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

void ResolveStaticExprToBoolQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Expr, DiagOnError);
}

void ResolveStaticExprToBoolQuery::Profile(llvm::FoldingSetNodeID &ID, ast::StaticExpr* Expr, bool DiagOnError)
{
   ID.AddPointer(Expr);
   ID.AddInteger(DiagOnError);
}

ExpandMacroQuery::ExpandMacroQuery(QueryContext &QC, StmtOrDecl SOD, DeclarationName Name, ast::DeclContext* DC, unsigned Delim, llvm::ArrayRef<lex::Token> Tokens, unsigned ExpectedKind) : Query(ExpandMacroQueryID, QC), SOD(SOD), Name(Name), DC(DC), Delim(Delim), Tokens(Tokens), ExpectedKind(ExpectedKind) {
assert(DC && "parameter 'DC' should not be null!");

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

   OS << "[ExpandMacro]"
;   OS << " SOD = ";
OS << SOD.getAsOpaquePtr();
   OS << ',';
   OS << " Name = ";
OS << "'" << Name << "'";
   OS << ',';
   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Delim = ";
OS << Delim;
   OS << ',';
   OS << " Tokens = ";
OS << '[';
   unsigned i = 0;
   for (auto &El : Tokens) {
      if (i == 5) { OS << ", ..."; break; }
      if (i++ != 0) OS << ", ";
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

void ExpandMacroQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, SOD, Name, DC, Delim, Tokens, ExpectedKind);
}

void ExpandMacroQuery::Profile(llvm::FoldingSetNodeID &ID, StmtOrDecl SOD, DeclarationName Name, ast::DeclContext* DC, unsigned Delim, llvm::ArrayRef<lex::Token> Tokens, unsigned ExpectedKind)
{
   ID.AddPointer(SOD.getAsOpaquePtr());
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddPointer(DC);
   ID.AddInteger(Delim);
   for (auto &El : Tokens)
      El.Profile(ID);
   ID.AddInteger(ExpectedKind);
}

ExpandMacroDeclQuery::ExpandMacroDeclQuery(QueryContext &QC, ast::MacroExpansionDecl* Decl) : Query(ExpandMacroDeclQueryID, QC), Decl(Decl) {

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

   OS << "[ExpandMacroDecl]"
;   OS << " Decl = ";
OS << "'" << (Decl ? Decl->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult ExpandMacroDeclQuery::finish(ast::Decl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ExpandMacroDeclQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Decl);
}

void ExpandMacroDeclQuery::Profile(llvm::FoldingSetNodeID &ID, ast::MacroExpansionDecl* Decl)
{
   ID.AddPointer(Decl);
}

ExpandMacrosQuery::ExpandMacrosQuery(QueryContext &QC, StringRef SourceFile) : Query(ExpandMacrosQueryID, QC), SourceFile(SourceFile) {

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

   OS << "[ExpandMacros]"
;   OS << " SourceFile = ";
OS << "'" << SourceFile << "'";
;

   return OS.str();
}

void ExpandMacrosQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, SourceFile);
}

void ExpandMacrosQuery::Profile(llvm::FoldingSetNodeID &ID, StringRef SourceFile)
{
   ID.AddString(SourceFile);
}

ImportExternalSourceFileQuery::ImportExternalSourceFileQuery(QueryContext &QC, StringRef FileName, ImportExternalSourceFileQuery::HeaderKind Kind, ast::DeclContext* DC, SourceLocation Loc, bool Optional) : Query(ImportExternalSourceFileQueryID, QC), FileName(FileName), Kind(Kind), DC(DC), Loc(Loc), Optional(Optional) {
assert(DC && "parameter 'DC' should not be null!");

}

std::string ImportExternalSourceFileQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ImportExternalSourceFile";
   return s;
}

std::string ImportExternalSourceFileQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ImportExternalSourceFile]"
;   OS << " FileName = ";
OS << "'" << FileName << "'";
   OS << ',';
   OS << " Kind = ";
OS << (uint64_t)Kind;
   OS << ',';
   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Loc = ";
OS << QC.CI.getFileMgr().getSourceLocationAsString(Loc);
   OS << ',';
   OS << " Optional = ";
OS << Optional;
;

   return OS.str();
}

void ImportExternalSourceFileQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, FileName, Kind, DC, Loc, Optional);
}

void ImportExternalSourceFileQuery::Profile(llvm::FoldingSetNodeID &ID, StringRef FileName, ImportExternalSourceFileQuery::HeaderKind Kind, ast::DeclContext* DC, SourceLocation Loc, bool Optional)
{
   ID.AddString(FileName);
   ID.AddInteger((uint64_t)Kind);
   ID.AddPointer(DC);
   ID.AddInteger(Loc.getOffset());
   ID.AddInteger(Optional);
}

FindExtensionsQuery::FindExtensionsQuery(QueryContext &QC, QualType T) : Query(FindExtensionsQueryID, QC), T(T) {

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

   OS << "[FindExtensions]"
;   OS << " T = ";
OS << "'" << T << "'";
;

   return OS.str();
}

void FindExtensionsQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void FindExtensionsQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

GetExtensionTypeKindQuery::GetExtensionTypeKindQuery(QueryContext &QC, ast::SourceType T) : Query(GetExtensionTypeKindQueryID, QC), T(T) {

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

   OS << "[GetExtensionTypeKind]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult GetExtensionTypeKindQuery::finish(GetExtensionTypeKindQuery::ResultKind Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetExtensionTypeKindQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void GetExtensionTypeKindQuery::Profile(llvm::FoldingSetNodeID &ID, ast::SourceType T)
{
   T.Profile(ID);
}

MatchExtensionTypeQuery::MatchExtensionTypeQuery(QueryContext &QC, QualType T, ast::SourceType PatternTy) : Query(MatchExtensionTypeQueryID, QC), T(T), PatternTy(PatternTy) {

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

   OS << "[MatchExtensionType]"
;   OS << " T = ";
OS << "'" << T << "'";
   OS << ',';
   OS << " PatternTy = ";
OS << PatternTy;
;

   return OS.str();
}

QueryResult MatchExtensionTypeQuery::finish(MatchExtensionTypeQuery::ResultKind Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void MatchExtensionTypeQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T, PatternTy);
}

void MatchExtensionTypeQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T, ast::SourceType PatternTy)
{
   ID.AddPointer(T.getAsOpaquePtr());
   PatternTy.Profile(ID);
}

ExtensionAppliesQuery::ExtensionAppliesQuery(QueryContext &QC, ast::ExtensionDecl* Ext, QualType T) : Query(ExtensionAppliesQueryID, QC), Ext(Ext), T(T) {

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

   OS << "[ExtensionApplies]"
;   OS << " Ext = ";
OS << "'" << (Ext ? Ext->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " T = ";
OS << "'" << T << "'";
;

   return OS.str();
}

QueryResult ExtensionAppliesQuery::finish(MatchExtensionTypeQuery::ResultKind Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ExtensionAppliesQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Ext, T);
}

void ExtensionAppliesQuery::Profile(llvm::FoldingSetNodeID &ID, ast::ExtensionDecl* Ext, QualType T)
{
   ID.AddPointer(Ext);
   ID.AddPointer(T.getAsOpaquePtr());
}

GetExtendedDeclQuery::GetExtendedDeclQuery(QueryContext &QC, ast::SourceType ExtendedTy) : Query(GetExtendedDeclQueryID, QC), ExtendedTy(ExtendedTy) {

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

   OS << "[GetExtendedDecl]"
;   OS << " ExtendedTy = ";
OS << ExtendedTy;
;

   return OS.str();
}

QueryResult GetExtendedDeclQuery::finish(ast::NamedDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetExtendedDeclQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, ExtendedTy);
}

void GetExtendedDeclQuery::Profile(llvm::FoldingSetNodeID &ID, ast::SourceType ExtendedTy)
{
   ExtendedTy.Profile(ID);
}

ResolveExtensionQuery::ResolveExtensionQuery(QueryContext &QC, ast::ExtensionDecl* Ext) : Query(ResolveExtensionQueryID, QC), Ext(Ext) {

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

   OS << "[ResolveExtension]"
;   OS << " Ext = ";
OS << "'" << (Ext ? Ext->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void ResolveExtensionQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Ext);
}

void ResolveExtensionQuery::Profile(llvm::FoldingSetNodeID &ID, ast::ExtensionDecl* Ext)
{
   ID.AddPointer(Ext);
}

ResolveConformanceToProtocolQuery::ResolveConformanceToProtocolQuery(QueryContext &QC, QualType T, const ast::SourceType& Conf) : Query(ResolveConformanceToProtocolQueryID, QC), T(T), Conf(Conf) {

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

   OS << "[ResolveConformanceToProtocol]"
;   OS << " T = ";
OS << "'" << T << "'";
   OS << ',';
   OS << " Conf = ";
OS << &Conf;
;

   return OS.str();
}

QueryResult ResolveConformanceToProtocolQuery::finish(ast::ProtocolDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ResolveConformanceToProtocolQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T, Conf);
}

void ResolveConformanceToProtocolQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T, const ast::SourceType& Conf)
{
   ID.AddPointer(T.getAsOpaquePtr());
   ID.AddPointer(&Conf);
}

ResolveExplicitConformancesQuery::ResolveExplicitConformancesQuery(QueryContext &QC, QualType T) : Query(ResolveExplicitConformancesQueryID, QC), T(T) {

}

std::string ResolveExplicitConformancesQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveExplicitConformances";
   return s;
}

std::string ResolveExplicitConformancesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveExplicitConformances]"
;   OS << " T = ";
OS << "'" << T << "'";
;

   return OS.str();
}

void ResolveExplicitConformancesQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void ResolveExplicitConformancesQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

ResolveConformancesToProtocolsQuery::ResolveConformancesToProtocolsQuery(QueryContext &QC, QualType T, llvm::MutableArrayRef<ast::SourceType> Conformances) : Query(ResolveConformancesToProtocolsQueryID, QC), T(T), Conformances(Conformances) {

}

ArrayRef<ast::ProtocolDecl*> ResolveConformancesToProtocolsQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ResolveConformancesToProtocolsQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveConformancesToProtocols";
   return s;
}

std::string ResolveConformancesToProtocolsQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveConformancesToProtocols]"
;   OS << " T = ";
OS << "'" << T << "'";
   OS << ',';
   OS << " Conformances = ";
OS << '[';
   unsigned i = 0;
   for (auto &El : Conformances) {
      if (i == 5) { OS << ", ..."; break; }
      if (i++ != 0) OS << ", ";
      OS << El;
   }
   OS << ']';

;

   return OS.str();
}

QueryResult ResolveConformancesToProtocolsQuery::finish(llvm::SmallVector<ast::ProtocolDecl*, 2> &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void ResolveConformancesToProtocolsQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T, Conformances);
}

void ResolveConformancesToProtocolsQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T, llvm::MutableArrayRef<ast::SourceType> Conformances)
{
   ID.AddPointer(T.getAsOpaquePtr());
   for (auto &El : Conformances)
      El.Profile(ID);
}

ResolveDeclaredConformancesQuery::ResolveDeclaredConformancesQuery(QueryContext &QC, QualType T, llvm::MutableArrayRef<ast::SourceType> Conformances, ConformanceKind Kind, ast::ConstraintSet* CS) : Query(ResolveDeclaredConformancesQueryID, QC), T(T), Conformances(Conformances), Kind(Kind), CS(CS) {

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

   OS << "[ResolveDeclaredConformances]"
;   OS << " T = ";
OS << "'" << T << "'";
   OS << ',';
   OS << " Conformances = ";
OS << '[';
   unsigned i = 0;
   for (auto &El : Conformances) {
      if (i == 5) { OS << ", ..."; break; }
      if (i++ != 0) OS << ", ";
      OS << El;
   }
   OS << ']';

   OS << ',';
   OS << " Kind = ";
OS << (uint64_t)Kind;
   OS << ',';
   OS << " CS = ";
OS << CS;
;

   return OS.str();
}

void ResolveDeclaredConformancesQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T, Conformances, Kind, CS);
}

void ResolveDeclaredConformancesQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T, llvm::MutableArrayRef<ast::SourceType> Conformances, ConformanceKind Kind, ast::ConstraintSet* CS)
{
   ID.AddPointer(T.getAsOpaquePtr());
   for (auto &El : Conformances)
      El.Profile(ID);
   ID.AddInteger((uint64_t)Kind);
   ID.AddPointer(CS);
}

AddSingleConformanceQuery::AddSingleConformanceQuery(QueryContext &QC, QualType T, ast::ProtocolDecl* Proto, ConformanceKind Kind, ast::ConstraintSet* CS, bool AddRecursiveConformances) : Query(AddSingleConformanceQueryID, QC), T(T), Proto(Proto), Kind(Kind), CS(CS), AddRecursiveConformances(AddRecursiveConformances) {

}

std::string AddSingleConformanceQuery::description() const
{
   std::string s;
   s += "running ";
   s += "AddSingleConformance";
   return s;
}

std::string AddSingleConformanceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[AddSingleConformance]"
;   OS << " T = ";
OS << "'" << T << "'";
   OS << ',';
   OS << " Proto = ";
OS << "'" << (Proto ? Proto->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Kind = ";
OS << (uint64_t)Kind;
   OS << ',';
   OS << " CS = ";
OS << CS;
   OS << ',';
   OS << " AddRecursiveConformances = ";
OS << AddRecursiveConformances;
;

   return OS.str();
}

void AddSingleConformanceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T, Proto, Kind, CS, AddRecursiveConformances);
}

void AddSingleConformanceQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T, ast::ProtocolDecl* Proto, ConformanceKind Kind, ast::ConstraintSet* CS, bool AddRecursiveConformances)
{
   ID.AddPointer(T.getAsOpaquePtr());
   ID.AddPointer(Proto);
   ID.AddInteger((uint64_t)Kind);
   ID.AddPointer(CS);
   ID.AddInteger(AddRecursiveConformances);
}

DeclareSelfAliasQuery::DeclareSelfAliasQuery(QueryContext &QC, ast::RecordDecl* R) : Query(DeclareSelfAliasQueryID, QC), R(R) {
assert(R && "parameter 'R' should not be null!");

}

std::string DeclareSelfAliasQuery::description() const
{
   std::string s;
   s += "running ";
   s += "DeclareSelfAlias";
   return s;
}

std::string DeclareSelfAliasQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[DeclareSelfAlias]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void DeclareSelfAliasQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, R);
}

void DeclareSelfAliasQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R)
{
   ID.AddPointer(R);
}

ReferencedAssociatedTypesReadyQuery::ReferencedAssociatedTypesReadyQuery(QueryContext &QC, QualType T, ast::ConstraintSet* CS) : Query(ReferencedAssociatedTypesReadyQueryID, QC), T(T), CS(CS) {

}

ReferencedAssociatedTypesReadyQuery::ResultKind ReferencedAssociatedTypesReadyQuery::get() const
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

   OS << "[ReferencedAssociatedTypesReady]"
;   OS << " T = ";
OS << "'" << T << "'";
   OS << ',';
   OS << " CS = ";
OS << CS;
;

   return OS.str();
}

QueryResult ReferencedAssociatedTypesReadyQuery::finish(ReferencedAssociatedTypesReadyQuery::ResultKind Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

CheckProtocolExtensionApplicabilityQuery::CheckProtocolExtensionApplicabilityQuery(QueryContext &QC, QualType T, ast::ExtensionDecl* Ext) : Query(CheckProtocolExtensionApplicabilityQueryID, QC), T(T), Ext(Ext) {

}

CheckProtocolExtensionApplicabilityQuery::ResultKind CheckProtocolExtensionApplicabilityQuery::get() const
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

   OS << "[CheckProtocolExtensionApplicability]"
;   OS << " T = ";
OS << "'" << T << "'";
   OS << ',';
   OS << " Ext = ";
OS << "'" << (Ext ? Ext->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult CheckProtocolExtensionApplicabilityQuery::finish(CheckProtocolExtensionApplicabilityQuery::ResultKind Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CheckProtocolExtensionApplicabilityQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T, Ext);
}

void CheckProtocolExtensionApplicabilityQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T, ast::ExtensionDecl* Ext)
{
   ID.AddPointer(T.getAsOpaquePtr());
   ID.AddPointer(Ext);
}

VerifyConstraintQuery::VerifyConstraintQuery(QueryContext &QC, const ast::ParsedConstraint* C, ast::NamedDecl* ConstrainedDecl) : Query(VerifyConstraintQueryID, QC), C(C), ConstrainedDecl(ConstrainedDecl) {
assert(ConstrainedDecl && "parameter 'ConstrainedDecl' should not be null!");

}

ast::DeclConstraint* VerifyConstraintQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
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

   OS << "[VerifyConstraint]"
;   OS << " C = ";
OS << C;
   OS << ',';
   OS << " ConstrainedDecl = ";
OS << "'" << (ConstrainedDecl ? ConstrainedDecl->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult VerifyConstraintQuery::finish(ast::DeclConstraint* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void VerifyConstraintQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, C, ConstrainedDecl);
}

void VerifyConstraintQuery::Profile(llvm::FoldingSetNodeID &ID, const ast::ParsedConstraint* C, ast::NamedDecl* ConstrainedDecl)
{
   ID.AddPointer(C);
   ID.AddPointer(ConstrainedDecl);
}

VerifyConstraintsQuery::VerifyConstraintsQuery(QueryContext &QC, ast::NamedDecl* ConstrainedDecl) : Query(VerifyConstraintsQueryID, QC), ConstrainedDecl(ConstrainedDecl) {
assert(ConstrainedDecl && "parameter 'ConstrainedDecl' should not be null!");

}

ast::ConstraintSet* VerifyConstraintsQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string VerifyConstraintsQuery::description() const
{
   std::string s;
   s += "running ";
   s += "VerifyConstraints";
   return s;
}

std::string VerifyConstraintsQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[VerifyConstraints]"
;   OS << " ConstrainedDecl = ";
OS << "'" << (ConstrainedDecl ? ConstrainedDecl->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult VerifyConstraintsQuery::finish(ast::ConstraintSet* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void VerifyConstraintsQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, ConstrainedDecl);
}

void VerifyConstraintsQuery::Profile(llvm::FoldingSetNodeID &ID, ast::NamedDecl* ConstrainedDecl)
{
   ID.AddPointer(ConstrainedDecl);
}

IsConstraintSatisfiedQuery::IsConstraintSatisfiedQuery(QueryContext &QC, const ast::DeclConstraint* C, ast::NamedDecl* ConcreteDecl, ast::NamedDecl* OriginalDecl, sema::FinalTemplateArgumentList* TemplateArgs) : Query(IsConstraintSatisfiedQueryID, QC), C(C), ConcreteDecl(ConcreteDecl), OriginalDecl(OriginalDecl), TemplateArgs(TemplateArgs) {
assert(ConcreteDecl && "parameter 'ConcreteDecl' should not be null!");
assert(OriginalDecl && "parameter 'OriginalDecl' should not be null!");

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

   OS << "[IsConstraintSatisfied]"
;   OS << " C = ";
OS << C;
   OS << ',';
   OS << " ConcreteDecl = ";
OS << "'" << (ConcreteDecl ? ConcreteDecl->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " OriginalDecl = ";
OS << "'" << (OriginalDecl ? OriginalDecl->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " TemplateArgs = ";
if (TemplateArgs)   TemplateArgs->print(OS);else OS << "<null>";
;

   return OS.str();
}

QueryResult IsConstraintSatisfiedQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsConstraintSatisfiedQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, C, ConcreteDecl, OriginalDecl, TemplateArgs);
}

void IsConstraintSatisfiedQuery::Profile(llvm::FoldingSetNodeID &ID, const ast::DeclConstraint* C, ast::NamedDecl* ConcreteDecl, ast::NamedDecl* OriginalDecl, sema::FinalTemplateArgumentList* TemplateArgs)
{
   ID.AddPointer(C);
   ID.AddPointer(ConcreteDecl);
   ID.AddPointer(OriginalDecl);
   ID.AddPointer(TemplateArgs);
}

GetConstrainedTypeQuery::GetConstrainedTypeQuery(QueryContext &QC, const ast::DeclConstraint* C, ast::NamedDecl* ConcreteDecl, ast::NamedDecl* OriginalDecl, sema::FinalTemplateArgumentList* TemplateArgs) : Query(GetConstrainedTypeQueryID, QC), C(C), ConcreteDecl(ConcreteDecl), OriginalDecl(OriginalDecl), TemplateArgs(TemplateArgs) {
assert(ConcreteDecl && "parameter 'ConcreteDecl' should not be null!");
assert(OriginalDecl && "parameter 'OriginalDecl' should not be null!");

}

QualType GetConstrainedTypeQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetConstrainedTypeQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetConstrainedType";
   return s;
}

std::string GetConstrainedTypeQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetConstrainedType]"
;   OS << " C = ";
OS << C;
   OS << ',';
   OS << " ConcreteDecl = ";
OS << "'" << (ConcreteDecl ? ConcreteDecl->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " OriginalDecl = ";
OS << "'" << (OriginalDecl ? OriginalDecl->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " TemplateArgs = ";
if (TemplateArgs)   TemplateArgs->print(OS);else OS << "<null>";
;

   return OS.str();
}

QueryResult GetConstrainedTypeQuery::finish(QualType Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetConstrainedTypeQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, C, ConcreteDecl, OriginalDecl, TemplateArgs);
}

void GetConstrainedTypeQuery::Profile(llvm::FoldingSetNodeID &ID, const ast::DeclConstraint* C, ast::NamedDecl* ConcreteDecl, ast::NamedDecl* OriginalDecl, sema::FinalTemplateArgumentList* TemplateArgs)
{
   ID.AddPointer(C);
   ID.AddPointer(ConcreteDecl);
   ID.AddPointer(OriginalDecl);
   ID.AddPointer(TemplateArgs);
}

IsSupersetOfQuery::IsSupersetOfQuery(QueryContext &QC, ast::ConstraintSet* C1, ast::ConstraintSet* C2) : Query(IsSupersetOfQueryID, QC), C1(C1), C2(C2) {

}

bool IsSupersetOfQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsSupersetOfQuery::description() const
{
   std::string s;
   s += "running ";
   s += "IsSupersetOf";
   return s;
}

std::string IsSupersetOfQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsSupersetOf]"
;   OS << " C1 = ";
OS << C1;
   OS << ',';
   OS << " C2 = ";
OS << C2;
;

   return OS.str();
}

QueryResult IsSupersetOfQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsSupersetOfQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, C1, C2);
}

void IsSupersetOfQuery::Profile(llvm::FoldingSetNodeID &ID, ast::ConstraintSet* C1, ast::ConstraintSet* C2)
{
   ID.AddPointer(C1);
   ID.AddPointer(C2);
}

GetReferencedAssociatedTypesQuery::GetReferencedAssociatedTypesQuery(QueryContext &QC, ast::ConstraintSet* CS) : Query(GetReferencedAssociatedTypesQueryID, QC), CS(CS) {

}

llvm::ArrayRef<ast::AssociatedTypeDecl*> GetReferencedAssociatedTypesQuery::get() const
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

   OS << "[GetReferencedAssociatedTypes]"
;   OS << " CS = ";
OS << CS;
;

   return OS.str();
}

QueryResult GetReferencedAssociatedTypesQuery::finish(std::vector<ast::AssociatedTypeDecl*> &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void GetReferencedAssociatedTypesQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, CS);
}

void GetReferencedAssociatedTypesQuery::Profile(llvm::FoldingSetNodeID &ID, ast::ConstraintSet* CS)
{
   ID.AddPointer(CS);
}

GetNeededAssociatedTypesQuery::GetNeededAssociatedTypesQuery(QueryContext &QC, ast::RecordDecl* R) : Query(GetNeededAssociatedTypesQueryID, QC), R(R) {
assert(R && "parameter 'R' should not be null!");

}

llvm::ArrayRef<ast::AssociatedTypeDecl*> GetNeededAssociatedTypesQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetNeededAssociatedTypesQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetNeededAssociatedTypes";
   return s;
}

std::string GetNeededAssociatedTypesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetNeededAssociatedTypes]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult GetNeededAssociatedTypesQuery::finish(llvm::SmallVector<ast::AssociatedTypeDecl*, 4> &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void GetNeededAssociatedTypesQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, R);
}

void GetNeededAssociatedTypesQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R)
{
   ID.AddPointer(R);
}

CheckConformancesQuery::CheckConformancesQuery(QueryContext &QC, QualType T, std::vector<ast::NamedDecl*> * ExtensionDecls) : Query(CheckConformancesQueryID, QC), T(T), ExtensionDecls(ExtensionDecls) {

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

   OS << "[CheckConformances]"
;   OS << " T = ";
OS << "'" << T << "'";
   OS << ',';
   OS << " ExtensionDecls = ";
OS << ExtensionDecls;
;

   return OS.str();
}

void CheckConformancesQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T, ExtensionDecls);
}

void CheckConformancesQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T, std::vector<ast::NamedDecl*> * ExtensionDecls)
{
   ID.AddPointer(T.getAsOpaquePtr());
   ID.AddPointer(ExtensionDecls);
}

CheckSingleConformanceQuery::CheckSingleConformanceQuery(QueryContext &QC, QualType T, ast::ProtocolDecl* P) : Query(CheckSingleConformanceQueryID, QC), T(T), P(P) {

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

   OS << "[CheckSingleConformance]"
;   OS << " T = ";
OS << "'" << T << "'";
   OS << ',';
   OS << " P = ";
OS << "'" << (P ? P->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void CheckSingleConformanceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T, P);
}

void CheckSingleConformanceQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T, ast::ProtocolDecl* P)
{
   ID.AddPointer(T.getAsOpaquePtr());
   ID.AddPointer(P);
}

CheckAssociatedTypeConstraintsQuery::CheckAssociatedTypeConstraintsQuery(QueryContext &QC, ast::RecordDecl* R) : Query(CheckAssociatedTypeConstraintsQueryID, QC), R(R) {
assert(R && "parameter 'R' should not be null!");

}

std::string CheckAssociatedTypeConstraintsQuery::description() const
{
   std::string s;
   s += "running ";
   s += "CheckAssociatedTypeConstraints";
   return s;
}

std::string CheckAssociatedTypeConstraintsQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CheckAssociatedTypeConstraints]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void CheckAssociatedTypeConstraintsQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, R);
}

void CheckAssociatedTypeConstraintsQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R)
{
   ID.AddPointer(R);
}

TypecheckConstraintsQuery::TypecheckConstraintsQuery(QueryContext &QC, ast::NamedDecl* ND) : Query(TypecheckConstraintsQueryID, QC), ND(ND) {
assert(ND && "parameter 'ND' should not be null!");

}

std::string TypecheckConstraintsQuery::description() const
{
   std::string s;
   s += "running ";
   s += "TypecheckConstraints";
   return s;
}

std::string TypecheckConstraintsQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckConstraints]"
;   OS << " ND = ";
OS << "'" << (ND ? ND->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckConstraintsQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, ND);
}

void TypecheckConstraintsQuery::Profile(llvm::FoldingSetNodeID &ID, ast::NamedDecl* ND)
{
   ID.AddPointer(ND);
}

ConformsToQuery::ConformsToQuery(QueryContext &QC, CanType T, ast::ProtocolDecl* P) : Query(ConformsToQueryID, QC), T(T), P(P) {
assert(P && "parameter 'P' should not be null!");

}

void ConformsToQuery::refresh()
{

   if (Stat == Idle) return;
   if (
        T->isRecordType() && QC.Sema->IsBeingResolved(T->getRecord())
    ) {
      Stat = Idle;
      
   }
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

   OS << "[ConformsTo]"
;   OS << " T = ";
OS << T;
   OS << ',';
   OS << " P = ";
OS << "'" << (P ? P->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult ConformsToQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ConformsToQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T, P);
}

void ConformsToQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T, ast::ProtocolDecl* P)
{
   T.Profile(ID);
   ID.AddPointer(P);
}

IsCovariantQuery::IsCovariantQuery(QueryContext &QC, QualType T, QualType Covar) : Query(IsCovariantQueryID, QC), T(T), Covar(Covar) {

}

void IsCovariantQuery::refresh()
{

   if (Stat == Idle) return;
   if (
        T->isRecordType() && QC.Sema->IsBeingResolved(T->getRecord())
    ) {
      Stat = Idle;
      
   }
}

bool IsCovariantQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsCovariantQuery::description() const
{
   std::string s;
   s += "running ";
   s += "IsCovariant";
   return s;
}

std::string IsCovariantQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsCovariant]"
;   OS << " T = ";
OS << "'" << T << "'";
   OS << ',';
   OS << " Covar = ";
OS << "'" << Covar << "'";
;

   return OS.str();
}

QueryResult IsCovariantQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsCovariantQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T, Covar);
}

void IsCovariantQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T, QualType Covar)
{
   ID.AddPointer(T.getAsOpaquePtr());
   ID.AddPointer(Covar.getAsOpaquePtr());
}

IsContravariantQuery::IsContravariantQuery(QueryContext &QC, QualType T, QualType Contravar) : Query(IsContravariantQueryID, QC), T(T), Contravar(Contravar) {

}

void IsContravariantQuery::refresh()
{

   if (Stat == Idle) return;
   if (
        T->isRecordType() && QC.Sema->IsBeingResolved(T->getRecord())
    ) {
      Stat = Idle;
      
   }
}

bool IsContravariantQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsContravariantQuery::description() const
{
   std::string s;
   s += "running ";
   s += "IsContravariant";
   return s;
}

std::string IsContravariantQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsContravariant]"
;   OS << " T = ";
OS << "'" << T << "'";
   OS << ',';
   OS << " Contravar = ";
OS << "'" << Contravar << "'";
;

   return OS.str();
}

QueryResult IsContravariantQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsContravariantQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T, Contravar);
}

void IsContravariantQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T, QualType Contravar)
{
   ID.AddPointer(T.getAsOpaquePtr());
   ID.AddPointer(Contravar.getAsOpaquePtr());
}

GetTypeSizeQuery::GetTypeSizeQuery(QueryContext &QC, CanType T) : Query(GetTypeSizeQueryID, QC), T(T) {

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

   OS << "[GetTypeSize]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult GetTypeSizeQuery::finish(unsigned Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetTypeSizeQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void GetTypeSizeQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

GetTypeAlignmentQuery::GetTypeAlignmentQuery(QueryContext &QC, CanType T) : Query(GetTypeAlignmentQueryID, QC), T(T) {

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

   OS << "[GetTypeAlignment]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult GetTypeAlignmentQuery::finish(unsigned short Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetTypeAlignmentQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void GetTypeAlignmentQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

GetTypeStrideQuery::GetTypeStrideQuery(QueryContext &QC, CanType T) : Query(GetTypeStrideQueryID, QC), T(T) {

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

   OS << "[GetTypeStride]"
;   OS << " T = ";
OS << T;
;

   return OS.str();
}

QueryResult GetTypeStrideQuery::finish(unsigned Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetTypeStrideQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void GetTypeStrideQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T)
{
   T.Profile(ID);
}

CalculateRecordSizeQuery::CalculateRecordSizeQuery(QueryContext &QC, ast::RecordDecl* R) : Query(CalculateRecordSizeQueryID, QC), R(R) {
assert(R && "parameter 'R' should not be null!");

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

   OS << "[CalculateRecordSize]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult CalculateRecordSizeQuery::finish(unsigned Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CalculateRecordSizeQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, R);
}

void CalculateRecordSizeQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R)
{
   ID.AddPointer(R);
}

GetRecordMetaQuery::GetRecordMetaQuery(QueryContext &QC, ast::RecordDecl* R, uint8_t Initialize) : Query(GetRecordMetaQueryID, QC), R(R), Initialize(Initialize) {
assert(R && "parameter 'R' should not be null!");

}

const RecordMetaInfo* GetRecordMetaQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetRecordMetaQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetRecordMeta";
   return s;
}

std::string GetRecordMetaQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetRecordMeta]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Initialize = ";
OS << Initialize;
;

   return OS.str();
}

QueryResult GetRecordMetaQuery::finish(const RecordMetaInfo* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

IsBuiltinIntegerTypeQuery::IsBuiltinIntegerTypeQuery(QueryContext &QC, ast::RecordDecl* R) : Query(IsBuiltinIntegerTypeQueryID, QC), R(R) {
assert(R && "parameter 'R' should not be null!");

}

bool IsBuiltinIntegerTypeQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsBuiltinIntegerTypeQuery::description() const
{
   std::string s;
   s += "running ";
   s += "IsBuiltinIntegerType";
   return s;
}

std::string IsBuiltinIntegerTypeQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsBuiltinIntegerType]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult IsBuiltinIntegerTypeQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsBuiltinIntegerTypeQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, R);
}

void IsBuiltinIntegerTypeQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R)
{
   ID.AddPointer(R);
}

IsBuiltinFloatingPointTypeQuery::IsBuiltinFloatingPointTypeQuery(QueryContext &QC, ast::RecordDecl* R) : Query(IsBuiltinFloatingPointTypeQueryID, QC), R(R) {
assert(R && "parameter 'R' should not be null!");

}

bool IsBuiltinFloatingPointTypeQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsBuiltinFloatingPointTypeQuery::description() const
{
   std::string s;
   s += "running ";
   s += "IsBuiltinFloatingPointType";
   return s;
}

std::string IsBuiltinFloatingPointTypeQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsBuiltinFloatingPointType]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult IsBuiltinFloatingPointTypeQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsBuiltinFloatingPointTypeQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, R);
}

void IsBuiltinFloatingPointTypeQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R)
{
   ID.AddPointer(R);
}

IsBuiltinBoolTypeQuery::IsBuiltinBoolTypeQuery(QueryContext &QC, ast::RecordDecl* R) : Query(IsBuiltinBoolTypeQueryID, QC), R(R) {
assert(R && "parameter 'R' should not be null!");

}

bool IsBuiltinBoolTypeQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string IsBuiltinBoolTypeQuery::description() const
{
   std::string s;
   s += "running ";
   s += "IsBuiltinBoolType";
   return s;
}

std::string IsBuiltinBoolTypeQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[IsBuiltinBoolType]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult IsBuiltinBoolTypeQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsBuiltinBoolTypeQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, R);
}

void IsBuiltinBoolTypeQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R)
{
   ID.AddPointer(R);
}

AddImplicitConformanceQuery::AddImplicitConformanceQuery(QueryContext &QC, ast::RecordDecl* R, ast::ImplicitConformanceKind K, ast::MethodDecl* Impl) : Query(AddImplicitConformanceQueryID, QC), R(R), K(K), Impl(Impl) {
assert(R && "parameter 'R' should not be null!");

}

ast::MethodDecl* AddImplicitConformanceQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string AddImplicitConformanceQuery::description() const
{
   std::string s;
   s += "running ";
   s += "AddImplicitConformance";
   return s;
}

std::string AddImplicitConformanceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[AddImplicitConformance]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " K = ";
OS << (uint64_t)K;
   OS << ',';
   OS << " Impl = ";
OS << "'" << (Impl ? Impl->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult AddImplicitConformanceQuery::finish(ast::MethodDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void AddImplicitConformanceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, R, K, Impl);
}

void AddImplicitConformanceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R, ast::ImplicitConformanceKind K, ast::MethodDecl* Impl)
{
   ID.AddPointer(R);
   ID.AddInteger((uint64_t)K);
   ID.AddPointer(Impl);
}

GetImplicitConformanceQuery::GetImplicitConformanceQuery(QueryContext &QC, ast::RecordDecl* R, ast::ImplicitConformanceKind K) : Query(GetImplicitConformanceQueryID, QC), R(R), K(K) {
assert(R && "parameter 'R' should not be null!");

}

ast::MethodDecl* GetImplicitConformanceQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetImplicitConformanceQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetImplicitConformance";
   return s;
}

std::string GetImplicitConformanceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetImplicitConformance]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " K = ";
OS << (uint64_t)K;
;

   return OS.str();
}

QueryResult GetImplicitConformanceQuery::finish(ast::MethodDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

CheckBuiltinConformancesQuery::CheckBuiltinConformancesQuery(QueryContext &QC, ast::RecordDecl* R) : Query(CheckBuiltinConformancesQueryID, QC), R(R) {
assert(R && "parameter 'R' should not be null!");

}

std::string CheckBuiltinConformancesQuery::description() const
{
   std::string s;
   s += "running ";
   s += "CheckBuiltinConformances";
   return s;
}

std::string CheckBuiltinConformancesQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CheckBuiltinConformances]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void CheckBuiltinConformancesQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, R);
}

void CheckBuiltinConformancesQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R)
{
   ID.AddPointer(R);
}

IsAccessibleQuery::IsAccessibleQuery(QueryContext &QC, ast::DeclContext* DC, ast::NamedDecl* ND) : Query(IsAccessibleQueryID, QC), DC(DC), ND(ND) {
assert(DC && "parameter 'DC' should not be null!");
assert(ND && "parameter 'ND' should not be null!");

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

   OS << "[IsAccessible]"
;   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " ND = ";
OS << "'" << (ND ? ND->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult IsAccessibleQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void IsAccessibleQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, DC, ND);
}

void IsAccessibleQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC, ast::NamedDecl* ND)
{
   ID.AddPointer(DC);
   ID.AddPointer(ND);
}

CheckAccessibilityQuery::CheckAccessibilityQuery(QueryContext &QC, ast::DeclContext* DC, ast::NamedDecl* ND, SourceLocation Loc) : Query(CheckAccessibilityQueryID, QC), DC(DC), ND(ND), Loc(Loc) {
assert(DC && "parameter 'DC' should not be null!");
assert(ND && "parameter 'ND' should not be null!");

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

   OS << "[CheckAccessibility]"
;   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " ND = ";
OS << "'" << (ND ? ND->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Loc = ";
OS << QC.CI.getFileMgr().getSourceLocationAsString(Loc);
;

   return OS.str();
}

void CheckAccessibilityQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, DC, ND, Loc);
}

void CheckAccessibilityQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC, ast::NamedDecl* ND, SourceLocation Loc)
{
   ID.AddPointer(DC);
   ID.AddPointer(ND);
   ID.AddInteger(Loc.getOffset());
}

ResolveNestedNameSpecToTypeQuery::ResolveNestedNameSpecToTypeQuery(QueryContext &QC, NestedNameSpecifierWithLoc* Name, ast::DeclContext* DC, bool IssueDiag) : Query(ResolveNestedNameSpecToTypeQueryID, QC), Name(Name), DC(DC), IssueDiag(IssueDiag) {

}

QualType ResolveNestedNameSpecToTypeQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ResolveNestedNameSpecToTypeQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveNestedNameSpecToType";
   return s;
}

std::string ResolveNestedNameSpecToTypeQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveNestedNameSpecToType]"
;   OS << " Name = ";
OS << Name;
   OS << ',';
   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " IssueDiag = ";
OS << IssueDiag;
;

   return OS.str();
}

QueryResult ResolveNestedNameSpecToTypeQuery::finish(QualType Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ResolveNestedNameSpecToTypeQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Name, DC, IssueDiag);
}

void ResolveNestedNameSpecToTypeQuery::Profile(llvm::FoldingSetNodeID &ID, NestedNameSpecifierWithLoc* Name, ast::DeclContext* DC, bool IssueDiag)
{
   ID.AddPointer(Name);
   ID.AddPointer(DC);
   ID.AddInteger(IssueDiag);
}

ResolveNestedNameSpecToDeclQuery::ResolveNestedNameSpecToDeclQuery(QueryContext &QC, NestedNameSpecifierWithLoc* Name, ast::DeclContext* DC, bool IssueDiag) : Query(ResolveNestedNameSpecToDeclQueryID, QC), Name(Name), DC(DC), IssueDiag(IssueDiag) {

}

ast::NamedDecl* ResolveNestedNameSpecToDeclQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string ResolveNestedNameSpecToDeclQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveNestedNameSpecToDecl";
   return s;
}

std::string ResolveNestedNameSpecToDeclQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveNestedNameSpecToDecl]"
;   OS << " Name = ";
OS << Name;
   OS << ',';
   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " IssueDiag = ";
OS << IssueDiag;
;

   return OS.str();
}

QueryResult ResolveNestedNameSpecToDeclQuery::finish(ast::NamedDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void ResolveNestedNameSpecToDeclQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Name, DC, IssueDiag);
}

void ResolveNestedNameSpecToDeclQuery::Profile(llvm::FoldingSetNodeID &ID, NestedNameSpecifierWithLoc* Name, ast::DeclContext* DC, bool IssueDiag)
{
   ID.AddPointer(Name);
   ID.AddPointer(DC);
   ID.AddInteger(IssueDiag);
}

TypeCheckDeclContextQuery::TypeCheckDeclContextQuery(QueryContext &QC, ast::DeclContext* DC) : Query(TypeCheckDeclContextQueryID, QC), DC(DC) {
assert(DC && "parameter 'DC' should not be null!");

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

   OS << "[TypeCheckDeclContext]"
;   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypeCheckDeclContextQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, DC);
}

void TypeCheckDeclContextQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC)
{
   ID.AddPointer(DC);
}

PrepareDeclInterfaceQuery::PrepareDeclInterfaceQuery(QueryContext &QC, ast::Decl* D) : Query(PrepareDeclInterfaceQueryID, QC), D(D) {
assert(D && "parameter 'D' should not be null!");

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

   OS << "[PrepareDeclInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareDeclInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareDeclInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::Decl* D)
{
   ID.AddPointer(D);
}

TypecheckDeclQuery::TypecheckDeclQuery(QueryContext &QC, ast::Decl* D) : Query(TypecheckDeclQueryID, QC), D(D) {
assert(D && "parameter 'D' should not be null!");

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

   OS << "[TypecheckDecl]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckDeclQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckDeclQuery::Profile(llvm::FoldingSetNodeID &ID, ast::Decl* D)
{
   ID.AddPointer(D);
}

PreparePrecedenceGroupInterfaceQuery::PreparePrecedenceGroupInterfaceQuery(QueryContext &QC, ast::PrecedenceGroupDecl* D) : Query(PreparePrecedenceGroupInterfaceQueryID, QC), D(D) {

}

std::string PreparePrecedenceGroupInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PreparePrecedenceGroupInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PreparePrecedenceGroupInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PreparePrecedenceGroupInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PreparePrecedenceGroupInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::PrecedenceGroupDecl* D)
{
   ID.AddPointer(D);
}

TypecheckPrecedenceGroupQuery::TypecheckPrecedenceGroupQuery(QueryContext &QC, ast::PrecedenceGroupDecl* D) : Query(TypecheckPrecedenceGroupQueryID, QC), D(D) {

}

std::string TypecheckPrecedenceGroupQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckPrecedenceGroupQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckPrecedenceGroup]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckPrecedenceGroupQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckPrecedenceGroupQuery::Profile(llvm::FoldingSetNodeID &ID, ast::PrecedenceGroupDecl* D)
{
   ID.AddPointer(D);
}

PrepareOperatorInterfaceQuery::PrepareOperatorInterfaceQuery(QueryContext &QC, ast::OperatorDecl* D) : Query(PrepareOperatorInterfaceQueryID, QC), D(D) {

}

std::string PrepareOperatorInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareOperatorInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareOperatorInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareOperatorInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareOperatorInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::OperatorDecl* D)
{
   ID.AddPointer(D);
}

TypecheckOperatorQuery::TypecheckOperatorQuery(QueryContext &QC, ast::OperatorDecl* D) : Query(TypecheckOperatorQueryID, QC), D(D) {

}

std::string TypecheckOperatorQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckOperatorQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckOperator]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckOperatorQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckOperatorQuery::Profile(llvm::FoldingSetNodeID &ID, ast::OperatorDecl* D)
{
   ID.AddPointer(D);
}

PrepareGlobalVarInterfaceQuery::PrepareGlobalVarInterfaceQuery(QueryContext &QC, ast::GlobalVarDecl* D) : Query(PrepareGlobalVarInterfaceQueryID, QC), D(D) {

}

std::string PrepareGlobalVarInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareGlobalVarInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareGlobalVarInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareGlobalVarInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareGlobalVarInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::GlobalVarDecl* D)
{
   ID.AddPointer(D);
}

TypecheckGlobalVarQuery::TypecheckGlobalVarQuery(QueryContext &QC, ast::GlobalVarDecl* D) : Query(TypecheckGlobalVarQueryID, QC), D(D) {

}

std::string TypecheckGlobalVarQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckGlobalVarQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckGlobalVar]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckGlobalVarQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckGlobalVarQuery::Profile(llvm::FoldingSetNodeID &ID, ast::GlobalVarDecl* D)
{
   ID.AddPointer(D);
}

PrepareCallableInterfaceQuery::PrepareCallableInterfaceQuery(QueryContext &QC, ast::CallableDecl* D) : Query(PrepareCallableInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareCallableInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareCallableInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareCallableInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::CallableDecl* D)
{
   ID.AddPointer(D);
}

TypecheckCallableQuery::TypecheckCallableQuery(QueryContext &QC, ast::CallableDecl* D) : Query(TypecheckCallableQueryID, QC), D(D) {

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

   OS << "[TypecheckCallable]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckCallableQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckCallableQuery::Profile(llvm::FoldingSetNodeID &ID, ast::CallableDecl* D)
{
   ID.AddPointer(D);
}

PrepareFunctionInterfaceQuery::PrepareFunctionInterfaceQuery(QueryContext &QC, ast::FunctionDecl* D) : Query(PrepareFunctionInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareFunctionInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareFunctionInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareFunctionInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::FunctionDecl* D)
{
   ID.AddPointer(D);
}

TypecheckFunctionQuery::TypecheckFunctionQuery(QueryContext &QC, ast::FunctionDecl* D) : Query(TypecheckFunctionQueryID, QC), D(D) {

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

   OS << "[TypecheckFunction]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckFunctionQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckFunctionQuery::Profile(llvm::FoldingSetNodeID &ID, ast::FunctionDecl* D)
{
   ID.AddPointer(D);
}

PrepareMethodInterfaceQuery::PrepareMethodInterfaceQuery(QueryContext &QC, ast::MethodDecl* D) : Query(PrepareMethodInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareMethodInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareMethodInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareMethodInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::MethodDecl* D)
{
   ID.AddPointer(D);
}

TypecheckMethodQuery::TypecheckMethodQuery(QueryContext &QC, ast::MethodDecl* D) : Query(TypecheckMethodQueryID, QC), D(D) {

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

   OS << "[TypecheckMethod]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckMethodQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckMethodQuery::Profile(llvm::FoldingSetNodeID &ID, ast::MethodDecl* D)
{
   ID.AddPointer(D);
}

PrepareInitInterfaceQuery::PrepareInitInterfaceQuery(QueryContext &QC, ast::InitDecl* D) : Query(PrepareInitInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareInitInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareInitInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareInitInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::InitDecl* D)
{
   ID.AddPointer(D);
}

TypecheckInitQuery::TypecheckInitQuery(QueryContext &QC, ast::InitDecl* D) : Query(TypecheckInitQueryID, QC), D(D) {

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

   OS << "[TypecheckInit]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckInitQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckInitQuery::Profile(llvm::FoldingSetNodeID &ID, ast::InitDecl* D)
{
   ID.AddPointer(D);
}

PrepareDeinitInterfaceQuery::PrepareDeinitInterfaceQuery(QueryContext &QC, ast::DeinitDecl* D) : Query(PrepareDeinitInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareDeinitInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareDeinitInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareDeinitInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeinitDecl* D)
{
   ID.AddPointer(D);
}

TypecheckDeinitQuery::TypecheckDeinitQuery(QueryContext &QC, ast::DeinitDecl* D) : Query(TypecheckDeinitQueryID, QC), D(D) {

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

   OS << "[TypecheckDeinit]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckDeinitQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckDeinitQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeinitDecl* D)
{
   ID.AddPointer(D);
}

PrepareFuncArgInterfaceQuery::PrepareFuncArgInterfaceQuery(QueryContext &QC, ast::FuncArgDecl* D) : Query(PrepareFuncArgInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareFuncArgInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareFuncArgInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareFuncArgInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::FuncArgDecl* D)
{
   ID.AddPointer(D);
}

TypecheckFuncArgQuery::TypecheckFuncArgQuery(QueryContext &QC, ast::FuncArgDecl* D) : Query(TypecheckFuncArgQueryID, QC), D(D) {

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

   OS << "[TypecheckFuncArg]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckFuncArgQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckFuncArgQuery::Profile(llvm::FoldingSetNodeID &ID, ast::FuncArgDecl* D)
{
   ID.AddPointer(D);
}

PrepareTemplateParamInterfaceQuery::PrepareTemplateParamInterfaceQuery(QueryContext &QC, ast::TemplateParamDecl* D) : Query(PrepareTemplateParamInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareTemplateParamInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareTemplateParamInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareTemplateParamInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::TemplateParamDecl* D)
{
   ID.AddPointer(D);
}

TypecheckTemplateParamQuery::TypecheckTemplateParamQuery(QueryContext &QC, ast::TemplateParamDecl* D) : Query(TypecheckTemplateParamQueryID, QC), D(D) {

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

   OS << "[TypecheckTemplateParam]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckTemplateParamQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckTemplateParamQuery::Profile(llvm::FoldingSetNodeID &ID, ast::TemplateParamDecl* D)
{
   ID.AddPointer(D);
}

PrepareAssociatedTypeInterfaceQuery::PrepareAssociatedTypeInterfaceQuery(QueryContext &QC, ast::AssociatedTypeDecl* D) : Query(PrepareAssociatedTypeInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareAssociatedTypeInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareAssociatedTypeInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareAssociatedTypeInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::AssociatedTypeDecl* D)
{
   ID.AddPointer(D);
}

TypecheckAssociatedTypeQuery::TypecheckAssociatedTypeQuery(QueryContext &QC, ast::AssociatedTypeDecl* D) : Query(TypecheckAssociatedTypeQueryID, QC), D(D) {

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

   OS << "[TypecheckAssociatedType]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckAssociatedTypeQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckAssociatedTypeQuery::Profile(llvm::FoldingSetNodeID &ID, ast::AssociatedTypeDecl* D)
{
   ID.AddPointer(D);
}

PrepareAliasInterfaceQuery::PrepareAliasInterfaceQuery(QueryContext &QC, ast::AliasDecl* D) : Query(PrepareAliasInterfaceQueryID, QC), D(D) {

}

std::string PrepareAliasInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareAliasInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareAliasInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareAliasInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareAliasInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::AliasDecl* D)
{
   ID.AddPointer(D);
}

TypecheckAliasQuery::TypecheckAliasQuery(QueryContext &QC, ast::AliasDecl* D) : Query(TypecheckAliasQueryID, QC), D(D) {

}

std::string TypecheckAliasQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckAliasQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckAlias]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckAliasQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckAliasQuery::Profile(llvm::FoldingSetNodeID &ID, ast::AliasDecl* D)
{
   ID.AddPointer(D);
}

PrepareRecordInterfaceQuery::PrepareRecordInterfaceQuery(QueryContext &QC, ast::RecordDecl* D) : Query(PrepareRecordInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareRecordInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareRecordInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareRecordInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* D)
{
   ID.AddPointer(D);
}

TypecheckRecordQuery::TypecheckRecordQuery(QueryContext &QC, ast::RecordDecl* D) : Query(TypecheckRecordQueryID, QC), D(D) {

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

   OS << "[TypecheckRecord]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckRecordQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckRecordQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* D)
{
   ID.AddPointer(D);
}

PrepareStructInterfaceQuery::PrepareStructInterfaceQuery(QueryContext &QC, ast::StructDecl* D) : Query(PrepareStructInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareStructInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareStructInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareStructInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::StructDecl* D)
{
   ID.AddPointer(D);
}

TypecheckStructQuery::TypecheckStructQuery(QueryContext &QC, ast::StructDecl* D) : Query(TypecheckStructQueryID, QC), D(D) {

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

   OS << "[TypecheckStruct]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckStructQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckStructQuery::Profile(llvm::FoldingSetNodeID &ID, ast::StructDecl* D)
{
   ID.AddPointer(D);
}

PrepareClassInterfaceQuery::PrepareClassInterfaceQuery(QueryContext &QC, ast::ClassDecl* D) : Query(PrepareClassInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareClassInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareClassInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareClassInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::ClassDecl* D)
{
   ID.AddPointer(D);
}

TypecheckClassQuery::TypecheckClassQuery(QueryContext &QC, ast::ClassDecl* D) : Query(TypecheckClassQueryID, QC), D(D) {

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

   OS << "[TypecheckClass]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckClassQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckClassQuery::Profile(llvm::FoldingSetNodeID &ID, ast::ClassDecl* D)
{
   ID.AddPointer(D);
}

PrepareEnumInterfaceQuery::PrepareEnumInterfaceQuery(QueryContext &QC, ast::EnumDecl* D) : Query(PrepareEnumInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareEnumInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareEnumInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareEnumInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::EnumDecl* D)
{
   ID.AddPointer(D);
}

TypecheckEnumQuery::TypecheckEnumQuery(QueryContext &QC, ast::EnumDecl* D) : Query(TypecheckEnumQueryID, QC), D(D) {

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

   OS << "[TypecheckEnum]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckEnumQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckEnumQuery::Profile(llvm::FoldingSetNodeID &ID, ast::EnumDecl* D)
{
   ID.AddPointer(D);
}

PrepareProtocolInterfaceQuery::PrepareProtocolInterfaceQuery(QueryContext &QC, ast::ProtocolDecl* D) : Query(PrepareProtocolInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareProtocolInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareProtocolInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareProtocolInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::ProtocolDecl* D)
{
   ID.AddPointer(D);
}

TypecheckProtocolQuery::TypecheckProtocolQuery(QueryContext &QC, ast::ProtocolDecl* D) : Query(TypecheckProtocolQueryID, QC), D(D) {

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

   OS << "[TypecheckProtocol]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckProtocolQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckProtocolQuery::Profile(llvm::FoldingSetNodeID &ID, ast::ProtocolDecl* D)
{
   ID.AddPointer(D);
}

PrepareExtensionInterfaceQuery::PrepareExtensionInterfaceQuery(QueryContext &QC, ast::ExtensionDecl* D) : Query(PrepareExtensionInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareExtensionInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareExtensionInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareExtensionInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::ExtensionDecl* D)
{
   ID.AddPointer(D);
}

TypecheckExtensionQuery::TypecheckExtensionQuery(QueryContext &QC, ast::ExtensionDecl* D) : Query(TypecheckExtensionQueryID, QC), D(D) {

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

   OS << "[TypecheckExtension]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckExtensionQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckExtensionQuery::Profile(llvm::FoldingSetNodeID &ID, ast::ExtensionDecl* D)
{
   ID.AddPointer(D);
}

PrepareFieldInterfaceQuery::PrepareFieldInterfaceQuery(QueryContext &QC, ast::FieldDecl* D) : Query(PrepareFieldInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareFieldInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareFieldInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareFieldInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::FieldDecl* D)
{
   ID.AddPointer(D);
}

TypecheckFieldQuery::TypecheckFieldQuery(QueryContext &QC, ast::FieldDecl* D) : Query(TypecheckFieldQueryID, QC), D(D) {

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

   OS << "[TypecheckField]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckFieldQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckFieldQuery::Profile(llvm::FoldingSetNodeID &ID, ast::FieldDecl* D)
{
   ID.AddPointer(D);
}

PreparePropInterfaceQuery::PreparePropInterfaceQuery(QueryContext &QC, ast::PropDecl* D) : Query(PreparePropInterfaceQueryID, QC), D(D) {

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

   OS << "[PreparePropInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PreparePropInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PreparePropInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::PropDecl* D)
{
   ID.AddPointer(D);
}

TypecheckPropQuery::TypecheckPropQuery(QueryContext &QC, ast::PropDecl* D) : Query(TypecheckPropQueryID, QC), D(D) {

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

   OS << "[TypecheckProp]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckPropQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckPropQuery::Profile(llvm::FoldingSetNodeID &ID, ast::PropDecl* D)
{
   ID.AddPointer(D);
}

PrepareSubscriptInterfaceQuery::PrepareSubscriptInterfaceQuery(QueryContext &QC, ast::SubscriptDecl* D) : Query(PrepareSubscriptInterfaceQueryID, QC), D(D) {

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

   OS << "[PrepareSubscriptInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareSubscriptInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareSubscriptInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::SubscriptDecl* D)
{
   ID.AddPointer(D);
}

TypecheckSubscriptQuery::TypecheckSubscriptQuery(QueryContext &QC, ast::SubscriptDecl* D) : Query(TypecheckSubscriptQueryID, QC), D(D) {

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

   OS << "[TypecheckSubscript]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckSubscriptQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckSubscriptQuery::Profile(llvm::FoldingSetNodeID &ID, ast::SubscriptDecl* D)
{
   ID.AddPointer(D);
}

PrepareEnumCaseInterfaceQuery::PrepareEnumCaseInterfaceQuery(QueryContext &QC, ast::EnumCaseDecl* D) : Query(PrepareEnumCaseInterfaceQueryID, QC), D(D) {

}

std::string PrepareEnumCaseInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareEnumCaseInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareEnumCaseInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareEnumCaseInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareEnumCaseInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::EnumCaseDecl* D)
{
   ID.AddPointer(D);
}

TypecheckEnumCaseQuery::TypecheckEnumCaseQuery(QueryContext &QC, ast::EnumCaseDecl* D) : Query(TypecheckEnumCaseQueryID, QC), D(D) {

}

std::string TypecheckEnumCaseQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckEnumCaseQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckEnumCase]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckEnumCaseQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckEnumCaseQuery::Profile(llvm::FoldingSetNodeID &ID, ast::EnumCaseDecl* D)
{
   ID.AddPointer(D);
}

PrepareNamespaceInterfaceQuery::PrepareNamespaceInterfaceQuery(QueryContext &QC, ast::NamespaceDecl* D) : Query(PrepareNamespaceInterfaceQueryID, QC), D(D) {

}

std::string PrepareNamespaceInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareNamespaceInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareNamespaceInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareNamespaceInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareNamespaceInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::NamespaceDecl* D)
{
   ID.AddPointer(D);
}

TypecheckNamespaceQuery::TypecheckNamespaceQuery(QueryContext &QC, ast::NamespaceDecl* D) : Query(TypecheckNamespaceQueryID, QC), D(D) {

}

std::string TypecheckNamespaceQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckNamespaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckNamespace]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckNamespaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckNamespaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::NamespaceDecl* D)
{
   ID.AddPointer(D);
}

PrepareCompoundInterfaceQuery::PrepareCompoundInterfaceQuery(QueryContext &QC, ast::CompoundDecl* D) : Query(PrepareCompoundInterfaceQueryID, QC), D(D) {

}

std::string PrepareCompoundInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareCompoundInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareCompoundInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareCompoundInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareCompoundInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::CompoundDecl* D)
{
   ID.AddPointer(D);
}

TypecheckCompoundQuery::TypecheckCompoundQuery(QueryContext &QC, ast::CompoundDecl* D) : Query(TypecheckCompoundQueryID, QC), D(D) {

}

std::string TypecheckCompoundQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckCompoundQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckCompound]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckCompoundQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckCompoundQuery::Profile(llvm::FoldingSetNodeID &ID, ast::CompoundDecl* D)
{
   ID.AddPointer(D);
}

PrepareModuleInterfaceQuery::PrepareModuleInterfaceQuery(QueryContext &QC, ast::ModuleDecl* D) : Query(PrepareModuleInterfaceQueryID, QC), D(D) {

}

std::string PrepareModuleInterfaceQuery::description() const
{
   std::string s;
   s += "preparing interface of '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string PrepareModuleInterfaceQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareModuleInterface]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareModuleInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void PrepareModuleInterfaceQuery::Profile(llvm::FoldingSetNodeID &ID, ast::ModuleDecl* D)
{
   ID.AddPointer(D);
}

TypecheckModuleQuery::TypecheckModuleQuery(QueryContext &QC, ast::ModuleDecl* D) : Query(TypecheckModuleQueryID, QC), D(D) {

}

std::string TypecheckModuleQuery::description() const
{
   std::string s;
   s += "typechecking '";
   s += D->Decl::getNameAsString();
   s += "'";
   return s;
}

std::string TypecheckModuleQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[TypecheckModule]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void TypecheckModuleQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void TypecheckModuleQuery::Profile(llvm::FoldingSetNodeID &ID, ast::ModuleDecl* D)
{
   ID.AddPointer(D);
}

PrepareTemplateParametersQuery::PrepareTemplateParametersQuery(QueryContext &QC, ast::NamedDecl* Decl) : Query(PrepareTemplateParametersQueryID, QC), Decl(Decl) {
assert(Decl && "parameter 'Decl' should not be null!");

}

std::string PrepareTemplateParametersQuery::description() const
{
   std::string s;
   s += "running ";
   s += "PrepareTemplateParameters";
   return s;
}

std::string PrepareTemplateParametersQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[PrepareTemplateParameters]"
;   OS << " Decl = ";
OS << "'" << (Decl ? Decl->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void PrepareTemplateParametersQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Decl);
}

void PrepareTemplateParametersQuery::Profile(llvm::FoldingSetNodeID &ID, ast::NamedDecl* Decl)
{
   ID.AddPointer(Decl);
}

DeclareImplicitInitializersQuery::DeclareImplicitInitializersQuery(QueryContext &QC, ast::RecordDecl* R) : Query(DeclareImplicitInitializersQueryID, QC), R(R) {
assert(R && "parameter 'R' should not be null!");

}

std::string DeclareImplicitInitializersQuery::description() const
{
   std::string s;
   s += "running ";
   s += "DeclareImplicitInitializers";
   return s;
}

std::string DeclareImplicitInitializersQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[DeclareImplicitInitializers]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void DeclareImplicitInitializersQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, R);
}

void DeclareImplicitInitializersQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R)
{
   ID.AddPointer(R);
}

DeclareImplicitDefaultInitQuery::DeclareImplicitDefaultInitQuery(QueryContext &QC, ast::StructDecl* S) : Query(DeclareImplicitDefaultInitQueryID, QC), S(S) {

}

std::string DeclareImplicitDefaultInitQuery::description() const
{
   std::string s;
   s += "running ";
   s += "DeclareImplicitDefaultInit";
   return s;
}

std::string DeclareImplicitDefaultInitQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[DeclareImplicitDefaultInit]"
;   OS << " S = ";
OS << "'" << (S ? S->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void DeclareImplicitDefaultInitQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, S);
}

void DeclareImplicitDefaultInitQuery::Profile(llvm::FoldingSetNodeID &ID, ast::StructDecl* S)
{
   ID.AddPointer(S);
}

DeclareMemberwiseInitQuery::DeclareMemberwiseInitQuery(QueryContext &QC, ast::StructDecl* S) : Query(DeclareMemberwiseInitQueryID, QC), S(S) {

}

std::string DeclareMemberwiseInitQuery::description() const
{
   std::string s;
   s += "running ";
   s += "DeclareMemberwiseInit";
   return s;
}

std::string DeclareMemberwiseInitQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[DeclareMemberwiseInit]"
;   OS << " S = ";
OS << "'" << (S ? S->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void DeclareMemberwiseInitQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, S);
}

void DeclareMemberwiseInitQuery::Profile(llvm::FoldingSetNodeID &ID, ast::StructDecl* S)
{
   ID.AddPointer(S);
}

DeclareImplicitDefaultDeinitQuery::DeclareImplicitDefaultDeinitQuery(QueryContext &QC, ast::RecordDecl* S) : Query(DeclareImplicitDefaultDeinitQueryID, QC), S(S) {

}

std::string DeclareImplicitDefaultDeinitQuery::description() const
{
   std::string s;
   s += "running ";
   s += "DeclareImplicitDefaultDeinit";
   return s;
}

std::string DeclareImplicitDefaultDeinitQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[DeclareImplicitDefaultDeinit]"
;   OS << " S = ";
OS << "'" << (S ? S->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void DeclareImplicitDefaultDeinitQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, S);
}

void DeclareImplicitDefaultDeinitQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* S)
{
   ID.AddPointer(S);
}

ResolveRawTypeQuery::ResolveRawTypeQuery(QueryContext &QC, ast::EnumDecl* E) : Query(ResolveRawTypeQueryID, QC), E(E) {

}

std::string ResolveRawTypeQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveRawType";
   return s;
}

std::string ResolveRawTypeQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveRawType]"
;   OS << " E = ";
OS << "'" << (E ? E->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void ResolveRawTypeQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, E);
}

void ResolveRawTypeQuery::Profile(llvm::FoldingSetNodeID &ID, ast::EnumDecl* E)
{
   ID.AddPointer(E);
}

AssignInitNameQuery::AssignInitNameQuery(QueryContext &QC, ast::InitDecl* Init) : Query(AssignInitNameQueryID, QC), Init(Init) {

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

   OS << "[AssignInitName]"
;   OS << " Init = ";
OS << "'" << (Init ? Init->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void AssignInitNameQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Init);
}

void AssignInitNameQuery::Profile(llvm::FoldingSetNodeID &ID, ast::InitDecl* Init)
{
   ID.AddPointer(Init);
}

CreateBaseInitQuery::CreateBaseInitQuery(QueryContext &QC, ast::InitDecl* D) : Query(CreateBaseInitQueryID, QC), D(D) {

}

std::string CreateBaseInitQuery::description() const
{
   std::string s;
   s += "running ";
   s += "CreateBaseInit";
   return s;
}

std::string CreateBaseInitQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[CreateBaseInit]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void CreateBaseInitQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void CreateBaseInitQuery::Profile(llvm::FoldingSetNodeID &ID, ast::InitDecl* D)
{
   ID.AddPointer(D);
}

LookupFirstQuery::LookupFirstQuery(QueryContext &QC, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts) : Query(LookupFirstQueryID, QC), DC(DC), Name(Name), Opts(Opts) {
assert(DC && "parameter 'DC' should not be null!");

        RevisionID = DC->getRevisionID();
    
}

void LookupFirstQuery::refresh()
{

   if (Stat == Idle) return;
   if (
        RevisionID < DC->getRevisionID()
    ) {
      Stat = Idle;
      
        RevisionID = DC->getRevisionID();
    
   }
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

   OS << "[LookupFirst]"
;   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Name = ";
OS << "'" << Name << "'";
   OS << ',';
   OS << " Opts = ";
OS << Opts;
;

   return OS.str();
}

QueryResult LookupFirstQuery::finish(SingleLevelLookupResult &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void LookupFirstQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, DC, Name, Opts);
}

void LookupFirstQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts)
{
   ID.AddPointer(DC);
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddInteger((uint8_t)(Opts));
}

LookupSingleQuery::LookupSingleQuery(QueryContext &QC, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts) : Query(LookupSingleQueryID, QC), DC(DC), Name(Name), Opts(Opts) {
assert(DC && "parameter 'DC' should not be null!");

        RevisionID = DC->getRevisionID();
    
}

void LookupSingleQuery::refresh()
{

   if (Stat == Idle) return;
   if (
        RevisionID < DC->getRevisionID()
    ) {
      Stat = Idle;
      
        RevisionID = DC->getRevisionID();
    
   }
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

   OS << "[LookupSingle]"
;   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Name = ";
OS << "'" << Name << "'";
   OS << ',';
   OS << " Opts = ";
OS << Opts;
;

   return OS.str();
}

QueryResult LookupSingleQuery::finish(ast::NamedDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void LookupSingleQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, DC, Name, Opts);
}

void LookupSingleQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts)
{
   ID.AddPointer(DC);
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddInteger((uint8_t)(Opts));
}

MultiLevelLookupQuery::MultiLevelLookupQuery(QueryContext &QC, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts) : Query(MultiLevelLookupQueryID, QC), DC(DC), Name(Name), Opts(Opts) {
assert(DC && "parameter 'DC' should not be null!");

        RevisionID = DC->getRevisionID();
    
}

void MultiLevelLookupQuery::refresh()
{

   if (Stat == Idle) return;
   if (
        RevisionID < DC->getRevisionID() || (Opts & LookupOpts::LocalLookup) != LookupOpts::None
    ) {
      Stat = Idle;
      
        RevisionID = DC->getRevisionID();
    
   }
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

   OS << "[MultiLevelLookup]"
;   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Name = ";
OS << "'" << Name << "'";
   OS << ',';
   OS << " Opts = ";
OS << Opts;
;

   return OS.str();
}

QueryResult MultiLevelLookupQuery::finish(MultiLevelLookupResult &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void MultiLevelLookupQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, DC, Name, Opts);
}

void MultiLevelLookupQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts)
{
   ID.AddPointer(DC);
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddInteger((uint8_t)(Opts));
}

MultiLevelTypeLookupQuery::MultiLevelTypeLookupQuery(QueryContext &QC, CanType T, DeclarationName Name, LookupOpts Opts) : Query(MultiLevelTypeLookupQueryID, QC), T(T), Name(Name), Opts(Opts) {

        if (auto *RT = T->removeMetaType()->asRecordType())
            RevisionID = RT->getRecord()->getRevisionID();
    
}

void MultiLevelTypeLookupQuery::refresh()
{

   if (Stat == Idle) return;
   if (
        !T->removeMetaType()->isRecordType() || RevisionID < T->removeMetaType()->getRecord()->getRevisionID()
    ) {
      Stat = Idle;
      
        if (auto *RT = T->removeMetaType()->asRecordType())
           RevisionID = RT->getRecord()->getRevisionID();
    
   }
}

const MultiLevelLookupResult* MultiLevelTypeLookupQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   
        return &Result.getValue();
    
}

std::string MultiLevelTypeLookupQuery::description() const
{
   std::string s;
   s += "running ";
   s += "MultiLevelTypeLookup";
   return s;
}

std::string MultiLevelTypeLookupQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[MultiLevelTypeLookup]"
;   OS << " T = ";
OS << T;
   OS << ',';
   OS << " Name = ";
OS << "'" << Name << "'";
   OS << ',';
   OS << " Opts = ";
OS << Opts;
;

   return OS.str();
}

QueryResult MultiLevelTypeLookupQuery::finish(MultiLevelLookupResult &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void MultiLevelTypeLookupQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T, Name, Opts);
}

void MultiLevelTypeLookupQuery::Profile(llvm::FoldingSetNodeID &ID, CanType T, DeclarationName Name, LookupOpts Opts)
{
   T.Profile(ID);
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddInteger((uint8_t)(Opts));
}

RestrictedLookupQuery::RestrictedLookupQuery(QueryContext &QC, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts) : Query(RestrictedLookupQueryID, QC), DC(DC), Name(Name), Opts(Opts) {
assert(DC && "parameter 'DC' should not be null!");

}

const MultiLevelLookupResult* RestrictedLookupQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   
        return &Result.getValue();
    
}

std::string RestrictedLookupQuery::description() const
{
   std::string s;
   s += "running ";
   s += "RestrictedLookup";
   return s;
}

std::string RestrictedLookupQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[RestrictedLookup]"
;   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Name = ";
OS << "'" << Name << "'";
   OS << ',';
   OS << " Opts = ";
OS << Opts;
;

   return OS.str();
}

QueryResult RestrictedLookupQuery::finish(MultiLevelLookupResult &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void RestrictedLookupQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, DC, Name, Opts);
}

void RestrictedLookupQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC, DeclarationName Name, LookupOpts Opts)
{
   ID.AddPointer(DC);
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddInteger((uint8_t)(Opts));
}

DirectLookupQuery::DirectLookupQuery(QueryContext &QC, ast::DeclContext* DC, DeclarationName Name, bool LookInExtensions, LookupOpts Opts, ast::ConstraintSet* Constraints) : Query(DirectLookupQueryID, QC), DC(DC), Name(Name), LookInExtensions(LookInExtensions), Opts(Opts), Constraints(Constraints) {
assert(DC && "parameter 'DC' should not be null!");

        RevisionID = DC->getRevisionID();
    
}

void DirectLookupQuery::refresh()
{

   if (Stat == Idle) return;
   if (
        RevisionID < DC->getRevisionID()
    ) {
      Stat = Idle;
      
        RevisionID = DC->getRevisionID();
    
   }
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

   OS << "[DirectLookup]"
;   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Name = ";
OS << "'" << Name << "'";
   OS << ',';
   OS << " LookInExtensions = ";
OS << LookInExtensions;
   OS << ',';
   OS << " Opts = ";
OS << Opts;
   OS << ',';
   OS << " Constraints = ";
OS << Constraints;
;

   return OS.str();
}

QueryResult DirectLookupQuery::finish(MultiLevelLookupResult &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void DirectLookupQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, DC, Name, LookInExtensions, Opts, Constraints);
}

void DirectLookupQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC, DeclarationName Name, bool LookInExtensions, LookupOpts Opts, ast::ConstraintSet* Constraints)
{
   ID.AddPointer(DC);
   ID.AddPointer(Name.getAsOpaquePtr());
   ID.AddInteger(LookInExtensions);
   ID.AddInteger((uint8_t)(Opts));
   ID.AddPointer(Constraints);
}

NestedNameLookupQuery::NestedNameLookupQuery(QueryContext &QC, ast::DeclContext* DC, llvm::ArrayRef<DeclarationName> Names, LookupOpts Opts, SourceRange Loc) : Query(NestedNameLookupQueryID, QC), DC(DC), Names(Names), Opts(Opts), Loc(Loc) {
assert(DC && "parameter 'DC' should not be null!");

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

   OS << "[NestedNameLookup]"
;   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Names = ";
OS << '[';
   unsigned i = 0;
   for (auto &El : Names) {
      if (i == 5) { OS << ", ..."; break; }
      if (i++ != 0) OS << ", ";
      OS << "'" << El << "'";
   }
   OS << ']';

   OS << ',';
   OS << " Opts = ";
OS << Opts;
;

   return OS.str();
}

QueryResult NestedNameLookupQuery::finish(MultiLevelLookupResult &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

ResolveMacrosQuery::ResolveMacrosQuery(QueryContext &QC, ast::DeclContext* DC) : Query(ResolveMacrosQueryID, QC), DC(DC) {
assert(DC && "parameter 'DC' should not be null!");

}

std::string ResolveMacrosQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveMacros";
   return s;
}

std::string ResolveMacrosQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveMacros]"
;   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void ResolveMacrosQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, DC);
}

void ResolveMacrosQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC)
{
   ID.AddPointer(DC);
}

ResolveStaticDeclarationsQuery::ResolveStaticDeclarationsQuery(QueryContext &QC, ast::DeclContext* DC) : Query(ResolveStaticDeclarationsQueryID, QC), DC(DC) {
assert(DC && "parameter 'DC' should not be null!");

}

std::string ResolveStaticDeclarationsQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveStaticDeclarations";
   return s;
}

std::string ResolveStaticDeclarationsQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveStaticDeclarations]"
;   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void ResolveStaticDeclarationsQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, DC);
}

void ResolveStaticDeclarationsQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC)
{
   ID.AddPointer(DC);
}

ResolveWildcardDeclarationsQuery::ResolveWildcardDeclarationsQuery(QueryContext &QC, ast::DeclContext* DC) : Query(ResolveWildcardDeclarationsQueryID, QC), DC(DC) {
assert(DC && "parameter 'DC' should not be null!");

}

std::string ResolveWildcardDeclarationsQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveWildcardDeclarations";
   return s;
}

std::string ResolveWildcardDeclarationsQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveWildcardDeclarations]"
;   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void ResolveWildcardDeclarationsQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, DC);
}

void ResolveWildcardDeclarationsQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC)
{
   ID.AddPointer(DC);
}

ResolveMetaDeclarationsQuery::ResolveMetaDeclarationsQuery(QueryContext &QC, ast::DeclContext* DC) : Query(ResolveMetaDeclarationsQueryID, QC), DC(DC) {
assert(DC && "parameter 'DC' should not be null!");

}

std::string ResolveMetaDeclarationsQuery::description() const
{
   std::string s;
   s += "running ";
   s += "ResolveMetaDeclarations";
   return s;
}

std::string ResolveMetaDeclarationsQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[ResolveMetaDeclarations]"
;   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void ResolveMetaDeclarationsQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, DC);
}

void ResolveMetaDeclarationsQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC)
{
   ID.AddPointer(DC);
}

GetAssociatedTypeImplQuery::GetAssociatedTypeImplQuery(QueryContext &QC, ast::RecordDecl* R, DeclarationName Name, llvm::ArrayRef<ast::ExtensionDecl*> Extensions, ast::ConstraintSet* Constraints) : Query(GetAssociatedTypeImplQueryID, QC), R(R), Name(Name), Extensions(Extensions), Constraints(Constraints) {
assert(R && "parameter 'R' should not be null!");

}

ast::AliasDecl* GetAssociatedTypeImplQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetAssociatedTypeImplQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetAssociatedTypeImpl";
   return s;
}

std::string GetAssociatedTypeImplQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetAssociatedTypeImpl]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Name = ";
OS << "'" << Name << "'";
   OS << ',';
   OS << " Extensions = ";
OS << '[';
   unsigned i = 0;
   for (auto &El : Extensions) {
      if (i == 5) { OS << ", ..."; break; }
      if (i++ != 0) OS << ", ";
      OS << "'" << (El ? El->Decl::getNameAsString() : "<null>") << "'";
   }
   OS << ']';

   OS << ',';
   OS << " Constraints = ";
OS << Constraints;
;

   return OS.str();
}

QueryResult GetAssociatedTypeImplQuery::finish(ast::AliasDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

GetAssociatedTypeDeclQuery::GetAssociatedTypeDeclQuery(QueryContext &QC, ast::ProtocolDecl* P, DeclarationName Name, ast::ConstraintSet* Constraints) : Query(GetAssociatedTypeDeclQueryID, QC), P(P), Name(Name), Constraints(Constraints) {

}

ast::AssociatedTypeDecl* GetAssociatedTypeDeclQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetAssociatedTypeDeclQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetAssociatedTypeDecl";
   return s;
}

std::string GetAssociatedTypeDeclQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetAssociatedTypeDecl]"
;   OS << " P = ";
OS << "'" << (P ? P->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Name = ";
OS << "'" << Name << "'";
   OS << ',';
   OS << " Constraints = ";
OS << Constraints;
;

   return OS.str();
}

QueryResult GetAssociatedTypeDeclQuery::finish(ast::AssociatedTypeDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

CheckTypeEquivalenceQuery::CheckTypeEquivalenceQuery(QueryContext &QC, QualType LHS, QualType RHS, QualType Self, ast::DeclContext* LHSDecl, ast::DeclContext* DeclCtx) : Query(CheckTypeEquivalenceQueryID, QC), LHS(LHS), RHS(RHS), Self(Self), LHSDecl(LHSDecl), DeclCtx(DeclCtx) {

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

   OS << "[CheckTypeEquivalence]"
;   OS << " LHS = ";
OS << "'" << LHS << "'";
   OS << ',';
   OS << " RHS = ";
OS << "'" << RHS << "'";
   OS << ',';
   OS << " Self = ";
OS << "'" << Self << "'";
   OS << ',';
   OS << " LHSDecl = ";
OS << "'" << (LHSDecl ? LHSDecl->getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " DeclCtx = ";
OS << "'" << (DeclCtx ? DeclCtx->getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult CheckTypeEquivalenceQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CheckTypeEquivalenceQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, LHS, RHS, Self, LHSDecl, DeclCtx);
}

void CheckTypeEquivalenceQuery::Profile(llvm::FoldingSetNodeID &ID, QualType LHS, QualType RHS, QualType Self, ast::DeclContext* LHSDecl, ast::DeclContext* DeclCtx)
{
   ID.AddPointer(LHS.getAsOpaquePtr());
   ID.AddPointer(RHS.getAsOpaquePtr());
   ID.AddPointer(Self.getAsOpaquePtr());
   ID.AddPointer(LHSDecl);
   ID.AddPointer(DeclCtx);
}

CheckTemplateExtensionApplicabilityQuery::CheckTemplateExtensionApplicabilityQuery(QueryContext &QC, ast::RecordDecl* Inst, ast::ExtensionDecl* Ext) : Query(CheckTemplateExtensionApplicabilityQueryID, QC), Inst(Inst), Ext(Ext) {
assert(Inst && "parameter 'Inst' should not be null!");

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

   OS << "[CheckTemplateExtensionApplicability]"
;   OS << " Inst = ";
OS << "'" << (Inst ? Inst->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Ext = ";
OS << "'" << (Ext ? Ext->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult CheckTemplateExtensionApplicabilityQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CheckTemplateExtensionApplicabilityQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Inst, Ext);
}

void CheckTemplateExtensionApplicabilityQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* Inst, ast::ExtensionDecl* Ext)
{
   ID.AddPointer(Inst);
   ID.AddPointer(Ext);
}

GetILModuleForDeclQuery::GetILModuleForDeclQuery(QueryContext &QC, ast::Decl* D) : Query(GetILModuleForDeclQueryID, QC), D(D) {
assert(D && "parameter 'D' should not be null!");

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

   OS << "[GetILModuleForDecl]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult GetILModuleForDeclQuery::finish(il::Module* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetILModuleForDeclQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void GetILModuleForDeclQuery::Profile(llvm::FoldingSetNodeID &ID, ast::Decl* D)
{
   ID.AddPointer(D);
}

GenerateILForDeclQuery::GenerateILForDeclQuery(QueryContext &QC, ast::Decl* D) : Query(GenerateILForDeclQueryID, QC), D(D) {
assert(D && "parameter 'D' should not be null!");

}

std::string GenerateILForDeclQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GenerateILForDecl";
   return s;
}

std::string GenerateILForDeclQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GenerateILForDecl]"
;   OS << " D = ";
OS << "'" << (D ? D->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void GenerateILForDeclQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, D);
}

void GenerateILForDeclQuery::Profile(llvm::FoldingSetNodeID &ID, ast::Decl* D)
{
   ID.AddPointer(D);
}

GenerateILForContextQuery::GenerateILForContextQuery(QueryContext &QC, ast::DeclContext* DC) : Query(GenerateILForContextQueryID, QC), DC(DC) {
assert(DC && "parameter 'DC' should not be null!");

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

   OS << "[GenerateILForContext]"
;   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void GenerateILForContextQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, DC);
}

void GenerateILForContextQuery::Profile(llvm::FoldingSetNodeID &ID, ast::DeclContext* DC)
{
   ID.AddPointer(DC);
}

GenerateRecordILQuery::GenerateRecordILQuery(QueryContext &QC, ast::RecordDecl* R) : Query(GenerateRecordILQueryID, QC), R(R) {
assert(R && "parameter 'R' should not be null!");

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

   OS << "[GenerateRecordIL]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void GenerateRecordILQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, R);
}

void GenerateRecordILQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R)
{
   ID.AddPointer(R);
}

GetILGlobalQuery::GetILGlobalQuery(QueryContext &QC, ast::VarDecl* GV) : Query(GetILGlobalQueryID, QC), GV(GV) {

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

   OS << "[GetILGlobal]"
;   OS << " GV = ";
OS << "'" << (GV ? GV->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult GetILGlobalQuery::finish(il::GlobalVariable* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetILGlobalQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, GV);
}

void GetILGlobalQuery::Profile(llvm::FoldingSetNodeID &ID, ast::VarDecl* GV)
{
   ID.AddPointer(GV);
}

GenerateLazyILGlobalDefinitionQuery::GenerateLazyILGlobalDefinitionQuery(QueryContext &QC, ast::VarDecl* GV) : Query(GenerateLazyILGlobalDefinitionQueryID, QC), GV(GV) {

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

   OS << "[GenerateLazyILGlobalDefinition]"
;   OS << " GV = ";
OS << "'" << (GV ? GV->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void GenerateLazyILGlobalDefinitionQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, GV);
}

void GenerateLazyILGlobalDefinitionQuery::Profile(llvm::FoldingSetNodeID &ID, ast::VarDecl* GV)
{
   ID.AddPointer(GV);
}

GetILFunctionQuery::GetILFunctionQuery(QueryContext &QC, ast::CallableDecl* C) : Query(GetILFunctionQueryID, QC), C(C) {

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

   OS << "[GetILFunction]"
;   OS << " C = ";
OS << "'" << (C ? C->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult GetILFunctionQuery::finish(il::Function* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetILFunctionQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, C);
}

void GetILFunctionQuery::Profile(llvm::FoldingSetNodeID &ID, ast::CallableDecl* C)
{
   ID.AddPointer(C);
}

GenerateILFunctionBodyQuery::GenerateILFunctionBodyQuery(QueryContext &QC, ast::CallableDecl* C) : Query(GenerateILFunctionBodyQueryID, QC), C(C) {

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

   OS << "[GenerateILFunctionBody]"
;   OS << " C = ";
OS << "'" << (C ? C->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

void GenerateILFunctionBodyQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, C);
}

void GenerateILFunctionBodyQuery::Profile(llvm::FoldingSetNodeID &ID, ast::CallableDecl* C)
{
   ID.AddPointer(C);
}

GetILTypeInfoQuery::GetILTypeInfoQuery(QueryContext &QC, QualType T) : Query(GetILTypeInfoQueryID, QC), T(T) {

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

   OS << "[GetILTypeInfo]"
;   OS << " T = ";
OS << "'" << T << "'";
;

   return OS.str();
}

QueryResult GetILTypeInfoQuery::finish(il::GlobalVariable* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetILTypeInfoQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void GetILTypeInfoQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

CreateILBasicTypeInfoQuery::CreateILBasicTypeInfoQuery(QueryContext &QC, QualType T) : Query(CreateILBasicTypeInfoQueryID, QC), T(T) {

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

   OS << "[CreateILBasicTypeInfo]"
;   OS << " T = ";
OS << "'" << T << "'";
;

   return OS.str();
}

QueryResult CreateILBasicTypeInfoQuery::finish(il::Constant* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CreateILBasicTypeInfoQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, T);
}

void CreateILBasicTypeInfoQuery::Profile(llvm::FoldingSetNodeID &ID, QualType T)
{
   ID.AddPointer(T.getAsOpaquePtr());
}

CreateILRecordTypeInfoQuery::CreateILRecordTypeInfoQuery(QueryContext &QC, ast::RecordDecl* R) : Query(CreateILRecordTypeInfoQueryID, QC), R(R) {
assert(R && "parameter 'R' should not be null!");

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

   OS << "[CreateILRecordTypeInfo]"
;   OS << " R = ";
OS << "'" << (R ? R->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult CreateILRecordTypeInfoQuery::finish(il::Constant* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void CreateILRecordTypeInfoQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, R);
}

void CreateILRecordTypeInfoQuery::Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl* R)
{
   ID.AddPointer(R);
}

GetBoolValueQuery::GetBoolValueQuery(QueryContext &QC, il::Constant* C, uint8_t AllowWrapperTypes) : Query(GetBoolValueQueryID, QC), C(C), AllowWrapperTypes(AllowWrapperTypes) {

}

bool GetBoolValueQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetBoolValueQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetBoolValue";
   return s;
}

std::string GetBoolValueQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetBoolValue]"
;   OS << " C = ";
OS << C;
   OS << ',';
   OS << " AllowWrapperTypes = ";
OS << AllowWrapperTypes;
;

   return OS.str();
}

QueryResult GetBoolValueQuery::finish(bool Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetBoolValueQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, C, AllowWrapperTypes);
}

void GetBoolValueQuery::Profile(llvm::FoldingSetNodeID &ID, il::Constant* C, uint8_t AllowWrapperTypes)
{
   ID.AddPointer(C);
   ID.AddInteger(AllowWrapperTypes);
}

GetStringValueQuery::GetStringValueQuery(QueryContext &QC, il::Constant* C, uint8_t AllowWrapperTypes) : Query(GetStringValueQueryID, QC), C(C), AllowWrapperTypes(AllowWrapperTypes) {

}

llvm::StringRef GetStringValueQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetStringValueQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetStringValue";
   return s;
}

std::string GetStringValueQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetStringValue]"
;   OS << " C = ";
OS << C;
   OS << ',';
   OS << " AllowWrapperTypes = ";
OS << AllowWrapperTypes;
;

   return OS.str();
}

QueryResult GetStringValueQuery::finish(llvm::StringRef Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void GetStringValueQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, C, AllowWrapperTypes);
}

void GetStringValueQuery::Profile(llvm::FoldingSetNodeID &ID, il::Constant* C, uint8_t AllowWrapperTypes)
{
   ID.AddPointer(C);
   ID.AddInteger(AllowWrapperTypes);
}

GetIntValueQuery::GetIntValueQuery(QueryContext &QC, il::Constant* C, uint8_t AllowWrapperTypes) : Query(GetIntValueQueryID, QC), C(C), AllowWrapperTypes(AllowWrapperTypes) {

}

llvm::APSInt GetIntValueQuery::get() const
{
   assert(ok() && "query has not successfully completed!");
   return Result.getValue();
}

std::string GetIntValueQuery::description() const
{
   std::string s;
   s += "running ";
   s += "GetIntValue";
   return s;
}

std::string GetIntValueQuery::summary() const
{
   std::string s;
   llvm::raw_string_ostream OS(s);

   OS << "[GetIntValue]"
;   OS << " C = ";
OS << C;
   OS << ',';
   OS << " AllowWrapperTypes = ";
OS << AllowWrapperTypes;
;

   return OS.str();
}

QueryResult GetIntValueQuery::finish(llvm::APSInt &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void GetIntValueQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, C, AllowWrapperTypes);
}

void GetIntValueQuery::Profile(llvm::FoldingSetNodeID &ID, il::Constant* C, uint8_t AllowWrapperTypes)
{
   ID.AddPointer(C);
   ID.AddInteger(AllowWrapperTypes);
}

OpenFileForReadQuery::OpenFileForReadQuery(QueryContext &QC, StringRef FileName) : Query(OpenFileForReadQueryID, QC), FileName(FileName) {

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

   OS << "[OpenFileForRead]"
;   OS << " FileName = ";
OS << "'" << FileName << "'";
;

   return OS.str();
}

QueryResult OpenFileForReadQuery::finish(std::unique_ptr<llvm::MemoryBuffer> &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void OpenFileForReadQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, FileName);
}

void OpenFileForReadQuery::Profile(llvm::FoldingSetNodeID &ID, StringRef FileName)
{
   ID.AddString(FileName);
}

OpenTmpFileQuery::OpenTmpFileQuery(QueryContext &QC, StringRef Extension) : Query(OpenTmpFileQueryID, QC), Extension(Extension) {

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

   OS << "[OpenTmpFile]"
;   OS << " Extension = ";
OS << "'" << Extension << "'";
;

   return OS.str();
}

QueryResult OpenTmpFileQuery::finish(std::unique_ptr<llvm::raw_ostream> &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

GetDefaultTemplateArgQuery::GetDefaultTemplateArgQuery(QueryContext &QC, ast::TemplateParamDecl* P) : Query(GetDefaultTemplateArgQueryID, QC), P(P) {

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

   OS << "[GetDefaultTemplateArg]"
;   OS << " P = ";
OS << "'" << (P ? P->Decl::getNameAsString() : "<null>") << "'";
;

   return OS.str();
}

QueryResult GetDefaultTemplateArgQuery::finish(sema::TemplateArgument &&Result, Status St)
{
   this->Result = std::move(Result);

   return Query::finish(St);
}

void GetDefaultTemplateArgQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, P);
}

void GetDefaultTemplateArgQuery::Profile(llvm::FoldingSetNodeID &ID, ast::TemplateParamDecl* P)
{
   ID.AddPointer(P);
}

CreateSelfArgumentQuery::CreateSelfArgumentQuery(QueryContext &QC, QualType Self, SourceLocation SelfLoc) : Query(CreateSelfArgumentQueryID, QC), Self(Self), SelfLoc(SelfLoc) {

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

   OS << "[CreateSelfArgument]"
;   OS << " Self = ";
OS << "'" << Self << "'";
   OS << ',';
   OS << " SelfLoc = ";
OS << QC.CI.getFileMgr().getSourceLocationAsString(SelfLoc);
;

   return OS.str();
}

QueryResult CreateSelfArgumentQuery::finish(ast::FuncArgDecl* Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

FindEquivalentDeclQuery::FindEquivalentDeclQuery(QueryContext &QC, ast::NamedDecl* Decl, ast::DeclContext* DC, QualType Self, bool LookInExtensions) : Query(FindEquivalentDeclQueryID, QC), Decl(Decl), DC(DC), Self(Self), LookInExtensions(LookInExtensions) {
assert(Decl && "parameter 'Decl' should not be null!");
assert(DC && "parameter 'DC' should not be null!");

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

   OS << "[FindEquivalentDecl]"
;   OS << " Decl = ";
OS << "'" << (Decl ? Decl->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " DC = ";
OS << "'" << (DC ? DC->getNameAsString() : "<null>") << "'";
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

void FindEquivalentDeclQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, Decl, DC, Self, LookInExtensions);
}

void FindEquivalentDeclQuery::Profile(llvm::FoldingSetNodeID &ID, ast::NamedDecl* Decl, ast::DeclContext* DC, QualType Self, bool LookInExtensions)
{
   ID.AddPointer(Decl);
   ID.AddPointer(DC);
   ID.AddPointer(Self.getAsOpaquePtr());
   ID.AddInteger(LookInExtensions);
}

EquivalentSignaturesQuery::EquivalentSignaturesQuery(QueryContext &QC, ast::CallableDecl* C1, ast::CallableDecl* C2, QualType Self) : Query(EquivalentSignaturesQueryID, QC), C1(C1), C2(C2), Self(Self) {

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

   OS << "[EquivalentSignatures]"
;   OS << " C1 = ";
OS << "'" << (C1 ? C1->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " C2 = ";
OS << "'" << (C2 ? C2->Decl::getNameAsString() : "<null>") << "'";
   OS << ',';
   OS << " Self = ";
OS << "'" << Self << "'";
;

   return OS.str();
}

QueryResult EquivalentSignaturesQuery::finish(EquivalentSignaturesQuery::ResultKind Result, Status St)
{
   this->Result = Result;

   return Query::finish(St);
}

void EquivalentSignaturesQuery::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, C1, C2, Self);
}

void EquivalentSignaturesQuery::Profile(llvm::FoldingSetNodeID &ID, ast::CallableDecl* C1, ast::CallableDecl* C2, QualType Self)
{
   ID.AddPointer(C1);
   ID.AddPointer(C2);
   ID.AddPointer(Self.getAsOpaquePtr());
}

