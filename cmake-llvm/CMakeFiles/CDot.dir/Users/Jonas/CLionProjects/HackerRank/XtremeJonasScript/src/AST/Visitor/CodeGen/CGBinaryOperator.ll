; ModuleID = '/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/AST/Visitor/CodeGen/CGBinaryOperator.cpp'
source_filename = "/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/AST/Visitor/CodeGen/CGBinaryOperator.cpp"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.12.0"

%"class.llvm::Value" = type { i32 (...)**, %"class.llvm::Type"*, %"class.llvm::Use"*, i8, i8, i16, i32 }
%"class.llvm::Type" = type { %"class.llvm::LLVMContext"*, i32, i32, %"class.llvm::Type"** }
%"class.llvm::LLVMContext" = type { %"class.llvm::LLVMContextImpl"* }
%"class.llvm::LLVMContextImpl" = type opaque
%"class.llvm::Use" = type { %"class.llvm::Value"*, %"class.llvm::Use"*, %"class.llvm::PointerIntPair" }
%"class.llvm::PointerIntPair" = type { i64 }
%struct.TypeSpecifier = type { i32, %"class.std::__1::basic_string", %"class.std::__1::shared_ptr", i8, i8, i8, i8, i64, i32, i8, i8, %"class.std::__1::basic_string", i8, [7 x i8], %"class.std::__1::unordered_map", i8, i8, i8, i8, i8, i8, i8, i32, %"class.std::__1::vector", %"class.std::__1::vector.19", %union.anon.25 }
%"class.std::__1::shared_ptr" = type { %class.Expression*, %"class.std::__1::__shared_weak_count"* }
%class.Expression = type opaque
%"class.std::__1::__shared_weak_count" = type { %"class.std::__1::__shared_count", i64 }
%"class.std::__1::__shared_count" = type { i32 (...)**, i64 }
%"class.std::__1::basic_string" = type { %"class.std::__1::__compressed_pair" }
%"class.std::__1::__compressed_pair" = type { %"class.std::__1::__libcpp_compressed_pair_imp" }
%"class.std::__1::__libcpp_compressed_pair_imp" = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" = type { %union.anon }
%union.anon = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" = type { i64, i64, i8* }
%"class.std::__1::unordered_map" = type { %"class.std::__1::__hash_table" }
%"class.std::__1::__hash_table" = type <{ %"class.std::__1::unique_ptr", %"class.std::__1::__compressed_pair.7", %"class.std::__1::__compressed_pair.11", %"class.std::__1::__compressed_pair.13", [4 x i8] }>
%"class.std::__1::unique_ptr" = type { %"class.std::__1::__compressed_pair.1" }
%"class.std::__1::__compressed_pair.1" = type { %"class.std::__1::__libcpp_compressed_pair_imp.2" }
%"class.std::__1::__libcpp_compressed_pair_imp.2" = type { %"struct.std::__1::__hash_node_base"**, %"class.std::__1::__bucket_list_deallocator" }
%"struct.std::__1::__hash_node_base" = type { %"struct.std::__1::__hash_node_base"* }
%"class.std::__1::__bucket_list_deallocator" = type { %"class.std::__1::__compressed_pair.3" }
%"class.std::__1::__compressed_pair.3" = type { %"class.std::__1::__libcpp_compressed_pair_imp.4" }
%"class.std::__1::__libcpp_compressed_pair_imp.4" = type { i64 }
%"class.std::__1::__compressed_pair.7" = type { %"class.std::__1::__libcpp_compressed_pair_imp.8" }
%"class.std::__1::__libcpp_compressed_pair_imp.8" = type { %"struct.std::__1::__hash_node_base" }
%"class.std::__1::__compressed_pair.11" = type { %"class.std::__1::__libcpp_compressed_pair_imp.12" }
%"class.std::__1::__libcpp_compressed_pair_imp.12" = type { i64 }
%"class.std::__1::__compressed_pair.13" = type { %"class.std::__1::__libcpp_compressed_pair_imp.14" }
%"class.std::__1::__libcpp_compressed_pair_imp.14" = type { float }
%"class.std::__1::vector" = type { %"class.std::__1::__vector_base" }
%"class.std::__1::__vector_base" = type { %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"*, %"class.std::__1::__compressed_pair.15" }
%"class.std::__1::__compressed_pair.15" = type { %"class.std::__1::__libcpp_compressed_pair_imp.16" }
%"class.std::__1::__libcpp_compressed_pair_imp.16" = type { %"class.std::__1::basic_string"* }
%"class.std::__1::vector.19" = type { %"class.std::__1::__vector_base.20" }
%"class.std::__1::__vector_base.20" = type { %struct.TypeSpecifier*, %struct.TypeSpecifier*, %"class.std::__1::__compressed_pair.21" }
%"class.std::__1::__compressed_pair.21" = type { %"class.std::__1::__libcpp_compressed_pair_imp.22" }
%"class.std::__1::__libcpp_compressed_pair_imp.22" = type { %struct.TypeSpecifier* }
%union.anon.25 = type { %struct.TypeSpecifier* }
%struct.CGValue = type { %"class.llvm::Value"*, i8, i64, i8, i8, i8, i32 }
%"class.llvm::IRBuilder" = type <{ %"class.llvm::IRBuilderBase", %"class.llvm::ConstantFolder", [7 x i8] }>
%"class.llvm::IRBuilderBase" = type { %"class.llvm::DebugLoc", %"class.llvm::BasicBlock"*, %"class.llvm::ilist_iterator", %"class.llvm::LLVMContext"*, %"class.llvm::MDNode"*, %"class.llvm::FastMathFlags", %"class.llvm::ArrayRef" }
%"class.llvm::DebugLoc" = type { %"class.llvm::TypedTrackingMDRef" }
%"class.llvm::TypedTrackingMDRef" = type { %"class.llvm::TrackingMDRef" }
%"class.llvm::TrackingMDRef" = type { %"class.llvm::Metadata"* }
%"class.llvm::Metadata" = type { i8, i8, i16, i32 }
%"class.llvm::BasicBlock" = type { %"class.llvm::Value", %"class.llvm::ilist_node_with_parent", %"class.llvm::SymbolTableList", %"class.llvm::Function"* }
%"class.llvm::ilist_node_with_parent" = type { %"class.llvm::ilist_node" }
%"class.llvm::ilist_node" = type { %"class.llvm::ilist_node_impl" }
%"class.llvm::ilist_node_impl" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::ilist_node_base" = type { %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"* }
%"class.llvm::SymbolTableList" = type { %"class.llvm::iplist_impl" }
%"class.llvm::iplist_impl" = type { %"class.llvm::simple_ilist" }
%"class.llvm::simple_ilist" = type { %"class.llvm::ilist_sentinel" }
%"class.llvm::ilist_sentinel" = type { %"class.llvm::ilist_node_impl.26" }
%"class.llvm::ilist_node_impl.26" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::Function" = type { %"class.llvm::GlobalObject", %"class.llvm::ilist_node.27", %"class.llvm::SymbolTableList.29", %"class.llvm::SymbolTableList.36", %"class.std::__1::unique_ptr.44", %"class.llvm::AttributeSet" }
%"class.llvm::GlobalObject" = type { %"class.llvm::GlobalValue", %"class.llvm::Comdat"* }
%"class.llvm::GlobalValue" = type { %"class.llvm::Constant", %"class.llvm::Type"*, i32, i32, %"class.llvm::Module"* }
%"class.llvm::Constant" = type { %"class.llvm::User" }
%"class.llvm::User" = type { %"class.llvm::Value" }
%"class.llvm::Module" = type opaque
%"class.llvm::Comdat" = type opaque
%"class.llvm::ilist_node.27" = type { %"class.llvm::ilist_node_impl.28" }
%"class.llvm::ilist_node_impl.28" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::SymbolTableList.29" = type { %"class.llvm::iplist_impl.30" }
%"class.llvm::iplist_impl.30" = type { %"class.llvm::simple_ilist.33" }
%"class.llvm::simple_ilist.33" = type { %"class.llvm::ilist_sentinel.35" }
%"class.llvm::ilist_sentinel.35" = type { %"class.llvm::ilist_node_impl" }
%"class.llvm::SymbolTableList.36" = type { %"class.llvm::iplist_impl.37" }
%"class.llvm::iplist_impl.37" = type { %"class.llvm::simple_ilist.40" }
%"class.llvm::simple_ilist.40" = type { %"class.llvm::ilist_sentinel.42" }
%"class.llvm::ilist_sentinel.42" = type { %"class.llvm::ilist_node_impl.43" }
%"class.llvm::ilist_node_impl.43" = type { %"class.llvm::ilist_node_base" }
%"class.std::__1::unique_ptr.44" = type { %"class.std::__1::__compressed_pair.45" }
%"class.std::__1::__compressed_pair.45" = type { %"class.std::__1::__libcpp_compressed_pair_imp.46" }
%"class.std::__1::__libcpp_compressed_pair_imp.46" = type { %"class.llvm::ValueSymbolTable"* }
%"class.llvm::ValueSymbolTable" = type opaque
%"class.llvm::AttributeSet" = type { %"class.llvm::AttributeSetImpl"* }
%"class.llvm::AttributeSetImpl" = type opaque
%"class.llvm::ilist_iterator" = type { %"class.llvm::ilist_node_impl.26"* }
%"class.llvm::MDNode" = type { %"class.llvm::Metadata", i32, i32, %"class.llvm::ContextAndReplaceableUses" }
%"class.llvm::ContextAndReplaceableUses" = type { %"class.llvm::PointerUnion" }
%"class.llvm::PointerUnion" = type { %"class.llvm::PointerIntPair.47" }
%"class.llvm::PointerIntPair.47" = type { i64 }
%"class.llvm::FastMathFlags" = type { i32 }
%"class.llvm::ArrayRef" = type { %"class.llvm::OperandBundleDefT"*, i64 }
%"class.llvm::OperandBundleDefT" = type { %"class.std::__1::basic_string", %"class.std::__1::vector.48" }
%"class.std::__1::vector.48" = type { %"class.std::__1::__vector_base.49" }
%"class.std::__1::__vector_base.49" = type { %"class.llvm::Value"**, %"class.llvm::Value"**, %"class.std::__1::__compressed_pair.50" }
%"class.std::__1::__compressed_pair.50" = type { %"class.std::__1::__libcpp_compressed_pair_imp.51" }
%"class.std::__1::__libcpp_compressed_pair_imp.51" = type { %"class.llvm::Value"** }
%"class.llvm::ConstantFolder" = type { i8 }
%"class.llvm::Twine" = type <{ %"union.llvm::Twine::Child", %"union.llvm::Twine::Child", i8, i8, [6 x i8] }>
%"union.llvm::Twine::Child" = type { %"class.llvm::Twine"* }
%"class.llvm::BinaryOperator" = type { %"class.llvm::Instruction" }
%"class.llvm::Instruction" = type { %"class.llvm::User", %"class.llvm::ilist_node_with_parent.54", %"class.llvm::BasicBlock"*, %"class.llvm::DebugLoc" }
%"class.llvm::ilist_node_with_parent.54" = type { %"class.llvm::ilist_node.55" }
%"class.llvm::ilist_node.55" = type { %"class.llvm::ilist_node_impl.26" }
%"class.llvm::IRBuilderDefaultInserter" = type { i8 }
%"class.llvm::SymbolTableListTraits" = type { i8 }
%"struct.llvm::ilist_detail::SpecificNodeAccess" = type { i8 }
%"class.llvm::PointerUnion.56" = type { %"class.llvm::PointerIntPair.57" }
%"class.llvm::PointerIntPair.57" = type { i64 }

@_ZN4llvm24DisableABIBreakingChecksE = external global i32, align 4
@_ZN4llvm30VerifyDisableABIBreakingChecksE = weak hidden global i32* @_ZN4llvm24DisableABIBreakingChecksE, align 8
@.str = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@.str.1 = private unnamed_addr constant [15 x i8] c"Invalid twine!\00", align 1
@__func__._ZN4llvm5TwineC2EPKc = private unnamed_addr constant [6 x i8] c"Twine\00", align 1
@.str.2 = private unnamed_addr constant [56 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/ADT/Twine.h\00", align 1
@.str.3 = private unnamed_addr constant [30 x i8] c"isValid() && \22Invalid twine!\22\00", align 1
@.str.4 = private unnamed_addr constant [29 x i8] c"isa<> used on a null pointer\00", align 1
@__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_ = private unnamed_addr constant [5 x i8] c"doit\00", align 1
@.str.5 = private unnamed_addr constant [62 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/Support/Casting.h\00", align 1
@.str.6 = private unnamed_addr constant [38 x i8] c"Val && \22isa<> used on a null pointer\22\00", align 1
@.str.7 = private unnamed_addr constant [42 x i8] c"cast<Ty>() argument of incompatible type!\00", align 1
@__func__._ZN4llvm4castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_ = private unnamed_addr constant [5 x i8] c"cast\00", align 1
@.str.8 = private unnamed_addr constant [59 x i8] c"isa<X>(Val) && \22cast<Ty>() argument of incompatible type!\22\00", align 1
@.str.9 = private unnamed_addr constant [25 x i8] c"Expected values to match\00", align 1
@__func__._ZN4llvm13TrackingMDRef7retrackERS0_ = private unnamed_addr constant [8 x i8] c"retrack\00", align 1
@.str.10 = private unnamed_addr constant [63 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/IR/TrackingMDRef.h\00", align 1
@.str.11 = private unnamed_addr constant [41 x i8] c"MD == X.MD && \22Expected values to match\22\00", align 1
@.str.12 = private unnamed_addr constant [28 x i8] c"Integer too large for field\00", align 1
@__func__._ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE9updateIntEll = private unnamed_addr constant [10 x i8] c"updateInt\00", align 1
@.str.13 = private unnamed_addr constant [65 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/ADT/PointerIntPair.h\00", align 1
@.str.14 = private unnamed_addr constant [59 x i8] c"(IntWord & ~IntMask) == 0 && \22Integer too large for field\22\00", align 1
@.str.15 = private unnamed_addr constant [36 x i8] c"Pointer is not sufficiently aligned\00", align 1
@__func__._ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE13updatePointerElS1_ = private unnamed_addr constant [14 x i8] c"updatePointer\00", align 1
@.str.16 = private unnamed_addr constant [74 x i8] c"(PtrWord & ~PointerBitMask) == 0 && \22Pointer is not sufficiently aligned\22\00", align 1

; Function Attrs: ssp uwtable
define %"class.llvm::Value"* @_ZN16CGBinaryOperator9CreateAddER13TypeSpecifierR7CGValueS3_RN4llvm9IRBuilderINS4_14ConstantFolderENS4_24IRBuilderDefaultInserterEEE(%struct.TypeSpecifier* dereferenceable(216), %struct.CGValue* dereferenceable(32), %struct.CGValue* dereferenceable(32), %"class.llvm::IRBuilder"* dereferenceable(72)) #0 align 2 {
  %5 = alloca %"class.llvm::Value"*, align 8
  %6 = alloca %struct.TypeSpecifier*, align 8
  %7 = alloca %struct.CGValue*, align 8
  %8 = alloca %struct.CGValue*, align 8
  %9 = alloca %"class.llvm::IRBuilder"*, align 8
  %10 = alloca %"class.llvm::Twine", align 8
  %11 = alloca %"class.llvm::Twine", align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %6, align 8
  store %struct.CGValue* %1, %struct.CGValue** %7, align 8
  store %struct.CGValue* %2, %struct.CGValue** %8, align 8
  store %"class.llvm::IRBuilder"* %3, %"class.llvm::IRBuilder"** %9, align 8
  %12 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %6, align 8
  %13 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %12, i32 0, i32 0
  %14 = load i32, i32* %13, align 8
  switch i32 %14, label %33 [
    i32 0, label %15
    i32 1, label %15
    i32 2, label %24
    i32 3, label %24
  ]

; <label>:15:                                     ; preds = %4, %4
  %16 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %9, align 8
  %17 = load %struct.CGValue*, %struct.CGValue** %7, align 8
  %18 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %17, i32 0, i32 0
  %19 = load %"class.llvm::Value"*, %"class.llvm::Value"** %18, align 8
  %20 = load %struct.CGValue*, %struct.CGValue** %8, align 8
  %21 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %20, i32 0, i32 0
  %22 = load %"class.llvm::Value"*, %"class.llvm::Value"** %21, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %10, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %23 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE9CreateAddEPNS_5ValueES5_RKNS_5TwineEbb(%"class.llvm::IRBuilder"* %16, %"class.llvm::Value"* %19, %"class.llvm::Value"* %22, %"class.llvm::Twine"* dereferenceable(24) %10, i1 zeroext false, i1 zeroext false)
  store %"class.llvm::Value"* %23, %"class.llvm::Value"** %5, align 8
  br label %33

; <label>:24:                                     ; preds = %4, %4
  %25 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %9, align 8
  %26 = load %struct.CGValue*, %struct.CGValue** %7, align 8
  %27 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %26, i32 0, i32 0
  %28 = load %"class.llvm::Value"*, %"class.llvm::Value"** %27, align 8
  %29 = load %struct.CGValue*, %struct.CGValue** %8, align 8
  %30 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %29, i32 0, i32 0
  %31 = load %"class.llvm::Value"*, %"class.llvm::Value"** %30, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %11, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %32 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateFAddEPNS_5ValueES5_RKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"* %25, %"class.llvm::Value"* %28, %"class.llvm::Value"* %31, %"class.llvm::Twine"* dereferenceable(24) %11, %"class.llvm::MDNode"* null)
  store %"class.llvm::Value"* %32, %"class.llvm::Value"** %5, align 8
  br label %33

; <label>:33:                                     ; preds = %15, %24, %4
  %34 = load %"class.llvm::Value"*, %"class.llvm::Value"** %5, align 8
  ret %"class.llvm::Value"* %34
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE9CreateAddEPNS_5ValueES5_RKNS_5TwineEbb(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24), i1 zeroext, i1 zeroext) #0 align 2 {
  %7 = alloca %"class.llvm::Value"*, align 8
  %8 = alloca %"class.llvm::IRBuilder"*, align 8
  %9 = alloca %"class.llvm::Value"*, align 8
  %10 = alloca %"class.llvm::Value"*, align 8
  %11 = alloca %"class.llvm::Twine"*, align 8
  %12 = alloca i8, align 1
  %13 = alloca i8, align 1
  %14 = alloca %"class.llvm::Constant"*, align 8
  %15 = alloca %"class.llvm::Constant"*, align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %8, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %9, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %10, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %11, align 8
  %16 = zext i1 %4 to i8
  store i8 %16, i8* %12, align 1
  %17 = zext i1 %5 to i8
  store i8 %17, i8* %13, align 1
  %18 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %8, align 8
  %19 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %20 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %19)
  store %"class.llvm::Constant"* %20, %"class.llvm::Constant"** %14, align 8
  %21 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %14, align 8
  %22 = icmp ne %"class.llvm::Constant"* %21, null
  br i1 %22, label %23, label %41

; <label>:23:                                     ; preds = %6
  %24 = load %"class.llvm::Value"*, %"class.llvm::Value"** %10, align 8
  %25 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %24)
  store %"class.llvm::Constant"* %25, %"class.llvm::Constant"** %15, align 8
  %26 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %15, align 8
  %27 = icmp ne %"class.llvm::Constant"* %26, null
  br i1 %27, label %28, label %40

; <label>:28:                                     ; preds = %23
  %29 = getelementptr inbounds %"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* %18, i32 0, i32 1
  %30 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %14, align 8
  %31 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %15, align 8
  %32 = load i8, i8* %12, align 1
  %33 = trunc i8 %32 to i1
  %34 = load i8, i8* %13, align 1
  %35 = trunc i8 %34 to i1
  %36 = call %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder9CreateAddEPNS_8ConstantES2_bb(%"class.llvm::ConstantFolder"* %29, %"class.llvm::Constant"* %30, %"class.llvm::Constant"* %31, i1 zeroext %33, i1 zeroext %35)
  %37 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %11, align 8
  %38 = call %"class.llvm::Constant"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertEPNS_8ConstantERKNS_5TwineE(%"class.llvm::IRBuilder"* %18, %"class.llvm::Constant"* %36, %"class.llvm::Twine"* dereferenceable(24) %37)
  %39 = bitcast %"class.llvm::Constant"* %38 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %39, %"class.llvm::Value"** %7, align 8
  br label %51

; <label>:40:                                     ; preds = %23
  br label %41

; <label>:41:                                     ; preds = %40, %6
  %42 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %43 = load %"class.llvm::Value"*, %"class.llvm::Value"** %10, align 8
  %44 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %11, align 8
  %45 = load i8, i8* %12, align 1
  %46 = trunc i8 %45 to i1
  %47 = load i8, i8* %13, align 1
  %48 = trunc i8 %47 to i1
  %49 = call %"class.llvm::BinaryOperator"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE23CreateInsertNUWNSWBinOpENS_11Instruction9BinaryOpsEPNS_5ValueES7_RKNS_5TwineEbb(%"class.llvm::IRBuilder"* %18, i32 11, %"class.llvm::Value"* %42, %"class.llvm::Value"* %43, %"class.llvm::Twine"* dereferenceable(24) %44, i1 zeroext %46, i1 zeroext %48)
  %50 = bitcast %"class.llvm::BinaryOperator"* %49 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %50, %"class.llvm::Value"** %7, align 8
  br label %51

; <label>:51:                                     ; preds = %41, %28
  %52 = load %"class.llvm::Value"*, %"class.llvm::Value"** %7, align 8
  ret %"class.llvm::Value"* %52
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"*, i8*) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::Twine"*, align 8
  %4 = alloca i8*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %3, align 8
  store i8* %1, i8** %4, align 8
  %5 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %3, align 8
  %6 = load i8*, i8** %4, align 8
  call void @_ZN4llvm5TwineC2EPKc(%"class.llvm::Twine"* %5, i8* %6)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateFAddEPNS_5ValueES5_RKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::MDNode"*) #0 align 2 {
  %6 = alloca %"class.llvm::Value"*, align 8
  %7 = alloca %"class.llvm::IRBuilder"*, align 8
  %8 = alloca %"class.llvm::Value"*, align 8
  %9 = alloca %"class.llvm::Value"*, align 8
  %10 = alloca %"class.llvm::Twine"*, align 8
  %11 = alloca %"class.llvm::MDNode"*, align 8
  %12 = alloca %"class.llvm::Constant"*, align 8
  %13 = alloca %"class.llvm::Constant"*, align 8
  %14 = alloca %"class.llvm::Twine", align 8
  %15 = alloca %"class.llvm::FastMathFlags", align 4
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %7, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %8, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %9, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %10, align 8
  store %"class.llvm::MDNode"* %4, %"class.llvm::MDNode"** %11, align 8
  %16 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %7, align 8
  %17 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %18 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %17)
  store %"class.llvm::Constant"* %18, %"class.llvm::Constant"** %12, align 8
  %19 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %20 = icmp ne %"class.llvm::Constant"* %19, null
  br i1 %20, label %21, label %35

; <label>:21:                                     ; preds = %5
  %22 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %23 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %22)
  store %"class.llvm::Constant"* %23, %"class.llvm::Constant"** %13, align 8
  %24 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %13, align 8
  %25 = icmp ne %"class.llvm::Constant"* %24, null
  br i1 %25, label %26, label %34

; <label>:26:                                     ; preds = %21
  %27 = getelementptr inbounds %"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* %16, i32 0, i32 1
  %28 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %29 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %13, align 8
  %30 = call %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateFAddEPNS_8ConstantES2_(%"class.llvm::ConstantFolder"* %27, %"class.llvm::Constant"* %28, %"class.llvm::Constant"* %29)
  %31 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %32 = call %"class.llvm::Constant"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertEPNS_8ConstantERKNS_5TwineE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Constant"* %30, %"class.llvm::Twine"* dereferenceable(24) %31)
  %33 = bitcast %"class.llvm::Constant"* %32 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %33, %"class.llvm::Value"** %6, align 8
  br label %51

; <label>:34:                                     ; preds = %21
  br label %35

; <label>:35:                                     ; preds = %34, %5
  %36 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %37 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %14, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %38 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator10CreateFAddEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"* %36, %"class.llvm::Value"* %37, %"class.llvm::Twine"* dereferenceable(24) %14)
  %39 = bitcast %"class.llvm::BinaryOperator"* %38 to %"class.llvm::Instruction"*
  %40 = load %"class.llvm::MDNode"*, %"class.llvm::MDNode"** %11, align 8
  %41 = bitcast %"class.llvm::IRBuilder"* %16 to %"class.llvm::IRBuilderBase"*
  %42 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %41, i32 0, i32 5
  %43 = bitcast %"class.llvm::FastMathFlags"* %15 to i8*
  %44 = bitcast %"class.llvm::FastMathFlags"* %42 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %43, i8* %44, i64 4, i32 4, i1 false)
  %45 = getelementptr inbounds %"class.llvm::FastMathFlags", %"class.llvm::FastMathFlags"* %15, i32 0, i32 0
  %46 = load i32, i32* %45, align 4
  %47 = call %"class.llvm::Instruction"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE19AddFPMathAttributesEPNS_11InstructionEPNS_6MDNodeENS_13FastMathFlagsE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Instruction"* %39, %"class.llvm::MDNode"* %40, i32 %46)
  %48 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %49 = call %"class.llvm::Instruction"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_11InstructionEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Instruction"* %47, %"class.llvm::Twine"* dereferenceable(24) %48)
  %50 = bitcast %"class.llvm::Instruction"* %49 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %50, %"class.llvm::Value"** %6, align 8
  br label %51

; <label>:51:                                     ; preds = %35, %26
  %52 = load %"class.llvm::Value"*, %"class.llvm::Value"** %6, align 8
  ret %"class.llvm::Value"* %52
}

; Function Attrs: ssp uwtable
define %"class.llvm::Value"* @_ZN16CGBinaryOperator9CreateSubER13TypeSpecifierR7CGValueS3_RN4llvm9IRBuilderINS4_14ConstantFolderENS4_24IRBuilderDefaultInserterEEE(%struct.TypeSpecifier* dereferenceable(216), %struct.CGValue* dereferenceable(32), %struct.CGValue* dereferenceable(32), %"class.llvm::IRBuilder"* dereferenceable(72)) #0 align 2 {
  %5 = alloca %"class.llvm::Value"*, align 8
  %6 = alloca %struct.TypeSpecifier*, align 8
  %7 = alloca %struct.CGValue*, align 8
  %8 = alloca %struct.CGValue*, align 8
  %9 = alloca %"class.llvm::IRBuilder"*, align 8
  %10 = alloca %"class.llvm::Twine", align 8
  %11 = alloca %"class.llvm::Twine", align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %6, align 8
  store %struct.CGValue* %1, %struct.CGValue** %7, align 8
  store %struct.CGValue* %2, %struct.CGValue** %8, align 8
  store %"class.llvm::IRBuilder"* %3, %"class.llvm::IRBuilder"** %9, align 8
  %12 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %6, align 8
  %13 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %12, i32 0, i32 0
  %14 = load i32, i32* %13, align 8
  switch i32 %14, label %33 [
    i32 0, label %15
    i32 1, label %15
    i32 2, label %24
    i32 3, label %24
  ]

; <label>:15:                                     ; preds = %4, %4
  %16 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %9, align 8
  %17 = load %struct.CGValue*, %struct.CGValue** %7, align 8
  %18 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %17, i32 0, i32 0
  %19 = load %"class.llvm::Value"*, %"class.llvm::Value"** %18, align 8
  %20 = load %struct.CGValue*, %struct.CGValue** %8, align 8
  %21 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %20, i32 0, i32 0
  %22 = load %"class.llvm::Value"*, %"class.llvm::Value"** %21, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %10, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %23 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE12CreateNSWSubEPNS_5ValueES5_RKNS_5TwineE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Value"* %19, %"class.llvm::Value"* %22, %"class.llvm::Twine"* dereferenceable(24) %10)
  store %"class.llvm::Value"* %23, %"class.llvm::Value"** %5, align 8
  br label %33

; <label>:24:                                     ; preds = %4, %4
  %25 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %9, align 8
  %26 = load %struct.CGValue*, %struct.CGValue** %7, align 8
  %27 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %26, i32 0, i32 0
  %28 = load %"class.llvm::Value"*, %"class.llvm::Value"** %27, align 8
  %29 = load %struct.CGValue*, %struct.CGValue** %8, align 8
  %30 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %29, i32 0, i32 0
  %31 = load %"class.llvm::Value"*, %"class.llvm::Value"** %30, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %11, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %32 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateFSubEPNS_5ValueES5_RKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"* %25, %"class.llvm::Value"* %28, %"class.llvm::Value"* %31, %"class.llvm::Twine"* dereferenceable(24) %11, %"class.llvm::MDNode"* null)
  store %"class.llvm::Value"* %32, %"class.llvm::Value"** %5, align 8
  br label %33

; <label>:33:                                     ; preds = %15, %24, %4
  %34 = load %"class.llvm::Value"*, %"class.llvm::Value"** %5, align 8
  ret %"class.llvm::Value"* %34
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE12CreateNSWSubEPNS_5ValueES5_RKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %5 = alloca %"class.llvm::IRBuilder"*, align 8
  %6 = alloca %"class.llvm::Value"*, align 8
  %7 = alloca %"class.llvm::Value"*, align 8
  %8 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %5, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %6, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %7, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %8, align 8
  %9 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %5, align 8
  %10 = load %"class.llvm::Value"*, %"class.llvm::Value"** %6, align 8
  %11 = load %"class.llvm::Value"*, %"class.llvm::Value"** %7, align 8
  %12 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %8, align 8
  %13 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE9CreateSubEPNS_5ValueES5_RKNS_5TwineEbb(%"class.llvm::IRBuilder"* %9, %"class.llvm::Value"* %10, %"class.llvm::Value"* %11, %"class.llvm::Twine"* dereferenceable(24) %12, i1 zeroext false, i1 zeroext true)
  ret %"class.llvm::Value"* %13
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateFSubEPNS_5ValueES5_RKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::MDNode"*) #0 align 2 {
  %6 = alloca %"class.llvm::Value"*, align 8
  %7 = alloca %"class.llvm::IRBuilder"*, align 8
  %8 = alloca %"class.llvm::Value"*, align 8
  %9 = alloca %"class.llvm::Value"*, align 8
  %10 = alloca %"class.llvm::Twine"*, align 8
  %11 = alloca %"class.llvm::MDNode"*, align 8
  %12 = alloca %"class.llvm::Constant"*, align 8
  %13 = alloca %"class.llvm::Constant"*, align 8
  %14 = alloca %"class.llvm::Twine", align 8
  %15 = alloca %"class.llvm::FastMathFlags", align 4
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %7, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %8, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %9, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %10, align 8
  store %"class.llvm::MDNode"* %4, %"class.llvm::MDNode"** %11, align 8
  %16 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %7, align 8
  %17 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %18 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %17)
  store %"class.llvm::Constant"* %18, %"class.llvm::Constant"** %12, align 8
  %19 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %20 = icmp ne %"class.llvm::Constant"* %19, null
  br i1 %20, label %21, label %35

; <label>:21:                                     ; preds = %5
  %22 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %23 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %22)
  store %"class.llvm::Constant"* %23, %"class.llvm::Constant"** %13, align 8
  %24 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %13, align 8
  %25 = icmp ne %"class.llvm::Constant"* %24, null
  br i1 %25, label %26, label %34

; <label>:26:                                     ; preds = %21
  %27 = getelementptr inbounds %"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* %16, i32 0, i32 1
  %28 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %29 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %13, align 8
  %30 = call %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateFSubEPNS_8ConstantES2_(%"class.llvm::ConstantFolder"* %27, %"class.llvm::Constant"* %28, %"class.llvm::Constant"* %29)
  %31 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %32 = call %"class.llvm::Constant"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertEPNS_8ConstantERKNS_5TwineE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Constant"* %30, %"class.llvm::Twine"* dereferenceable(24) %31)
  %33 = bitcast %"class.llvm::Constant"* %32 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %33, %"class.llvm::Value"** %6, align 8
  br label %51

; <label>:34:                                     ; preds = %21
  br label %35

; <label>:35:                                     ; preds = %34, %5
  %36 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %37 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %14, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %38 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator10CreateFSubEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"* %36, %"class.llvm::Value"* %37, %"class.llvm::Twine"* dereferenceable(24) %14)
  %39 = bitcast %"class.llvm::BinaryOperator"* %38 to %"class.llvm::Instruction"*
  %40 = load %"class.llvm::MDNode"*, %"class.llvm::MDNode"** %11, align 8
  %41 = bitcast %"class.llvm::IRBuilder"* %16 to %"class.llvm::IRBuilderBase"*
  %42 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %41, i32 0, i32 5
  %43 = bitcast %"class.llvm::FastMathFlags"* %15 to i8*
  %44 = bitcast %"class.llvm::FastMathFlags"* %42 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %43, i8* %44, i64 4, i32 4, i1 false)
  %45 = getelementptr inbounds %"class.llvm::FastMathFlags", %"class.llvm::FastMathFlags"* %15, i32 0, i32 0
  %46 = load i32, i32* %45, align 4
  %47 = call %"class.llvm::Instruction"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE19AddFPMathAttributesEPNS_11InstructionEPNS_6MDNodeENS_13FastMathFlagsE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Instruction"* %39, %"class.llvm::MDNode"* %40, i32 %46)
  %48 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %49 = call %"class.llvm::Instruction"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_11InstructionEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Instruction"* %47, %"class.llvm::Twine"* dereferenceable(24) %48)
  %50 = bitcast %"class.llvm::Instruction"* %49 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %50, %"class.llvm::Value"** %6, align 8
  br label %51

; <label>:51:                                     ; preds = %35, %26
  %52 = load %"class.llvm::Value"*, %"class.llvm::Value"** %6, align 8
  ret %"class.llvm::Value"* %52
}

; Function Attrs: ssp uwtable
define %"class.llvm::Value"* @_ZN16CGBinaryOperator9CreateMulER13TypeSpecifierR7CGValueS3_RN4llvm9IRBuilderINS4_14ConstantFolderENS4_24IRBuilderDefaultInserterEEE(%struct.TypeSpecifier* dereferenceable(216), %struct.CGValue* dereferenceable(32), %struct.CGValue* dereferenceable(32), %"class.llvm::IRBuilder"* dereferenceable(72)) #0 align 2 {
  %5 = alloca %"class.llvm::Value"*, align 8
  %6 = alloca %struct.TypeSpecifier*, align 8
  %7 = alloca %struct.CGValue*, align 8
  %8 = alloca %struct.CGValue*, align 8
  %9 = alloca %"class.llvm::IRBuilder"*, align 8
  %10 = alloca %"class.llvm::Twine", align 8
  %11 = alloca %"class.llvm::Twine", align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %6, align 8
  store %struct.CGValue* %1, %struct.CGValue** %7, align 8
  store %struct.CGValue* %2, %struct.CGValue** %8, align 8
  store %"class.llvm::IRBuilder"* %3, %"class.llvm::IRBuilder"** %9, align 8
  %12 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %6, align 8
  %13 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %12, i32 0, i32 0
  %14 = load i32, i32* %13, align 8
  switch i32 %14, label %33 [
    i32 0, label %15
    i32 1, label %15
    i32 2, label %24
    i32 3, label %24
  ]

; <label>:15:                                     ; preds = %4, %4
  %16 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %9, align 8
  %17 = load %struct.CGValue*, %struct.CGValue** %7, align 8
  %18 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %17, i32 0, i32 0
  %19 = load %"class.llvm::Value"*, %"class.llvm::Value"** %18, align 8
  %20 = load %struct.CGValue*, %struct.CGValue** %8, align 8
  %21 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %20, i32 0, i32 0
  %22 = load %"class.llvm::Value"*, %"class.llvm::Value"** %21, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %10, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %23 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE9CreateMulEPNS_5ValueES5_RKNS_5TwineEbb(%"class.llvm::IRBuilder"* %16, %"class.llvm::Value"* %19, %"class.llvm::Value"* %22, %"class.llvm::Twine"* dereferenceable(24) %10, i1 zeroext false, i1 zeroext false)
  store %"class.llvm::Value"* %23, %"class.llvm::Value"** %5, align 8
  br label %33

; <label>:24:                                     ; preds = %4, %4
  %25 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %9, align 8
  %26 = load %struct.CGValue*, %struct.CGValue** %7, align 8
  %27 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %26, i32 0, i32 0
  %28 = load %"class.llvm::Value"*, %"class.llvm::Value"** %27, align 8
  %29 = load %struct.CGValue*, %struct.CGValue** %8, align 8
  %30 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %29, i32 0, i32 0
  %31 = load %"class.llvm::Value"*, %"class.llvm::Value"** %30, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %11, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %32 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateFMulEPNS_5ValueES5_RKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"* %25, %"class.llvm::Value"* %28, %"class.llvm::Value"* %31, %"class.llvm::Twine"* dereferenceable(24) %11, %"class.llvm::MDNode"* null)
  store %"class.llvm::Value"* %32, %"class.llvm::Value"** %5, align 8
  br label %33

; <label>:33:                                     ; preds = %15, %24, %4
  %34 = load %"class.llvm::Value"*, %"class.llvm::Value"** %5, align 8
  ret %"class.llvm::Value"* %34
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE9CreateMulEPNS_5ValueES5_RKNS_5TwineEbb(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24), i1 zeroext, i1 zeroext) #0 align 2 {
  %7 = alloca %"class.llvm::Value"*, align 8
  %8 = alloca %"class.llvm::IRBuilder"*, align 8
  %9 = alloca %"class.llvm::Value"*, align 8
  %10 = alloca %"class.llvm::Value"*, align 8
  %11 = alloca %"class.llvm::Twine"*, align 8
  %12 = alloca i8, align 1
  %13 = alloca i8, align 1
  %14 = alloca %"class.llvm::Constant"*, align 8
  %15 = alloca %"class.llvm::Constant"*, align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %8, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %9, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %10, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %11, align 8
  %16 = zext i1 %4 to i8
  store i8 %16, i8* %12, align 1
  %17 = zext i1 %5 to i8
  store i8 %17, i8* %13, align 1
  %18 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %8, align 8
  %19 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %20 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %19)
  store %"class.llvm::Constant"* %20, %"class.llvm::Constant"** %14, align 8
  %21 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %14, align 8
  %22 = icmp ne %"class.llvm::Constant"* %21, null
  br i1 %22, label %23, label %41

; <label>:23:                                     ; preds = %6
  %24 = load %"class.llvm::Value"*, %"class.llvm::Value"** %10, align 8
  %25 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %24)
  store %"class.llvm::Constant"* %25, %"class.llvm::Constant"** %15, align 8
  %26 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %15, align 8
  %27 = icmp ne %"class.llvm::Constant"* %26, null
  br i1 %27, label %28, label %40

; <label>:28:                                     ; preds = %23
  %29 = getelementptr inbounds %"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* %18, i32 0, i32 1
  %30 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %14, align 8
  %31 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %15, align 8
  %32 = load i8, i8* %12, align 1
  %33 = trunc i8 %32 to i1
  %34 = load i8, i8* %13, align 1
  %35 = trunc i8 %34 to i1
  %36 = call %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder9CreateMulEPNS_8ConstantES2_bb(%"class.llvm::ConstantFolder"* %29, %"class.llvm::Constant"* %30, %"class.llvm::Constant"* %31, i1 zeroext %33, i1 zeroext %35)
  %37 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %11, align 8
  %38 = call %"class.llvm::Constant"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertEPNS_8ConstantERKNS_5TwineE(%"class.llvm::IRBuilder"* %18, %"class.llvm::Constant"* %36, %"class.llvm::Twine"* dereferenceable(24) %37)
  %39 = bitcast %"class.llvm::Constant"* %38 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %39, %"class.llvm::Value"** %7, align 8
  br label %51

; <label>:40:                                     ; preds = %23
  br label %41

; <label>:41:                                     ; preds = %40, %6
  %42 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %43 = load %"class.llvm::Value"*, %"class.llvm::Value"** %10, align 8
  %44 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %11, align 8
  %45 = load i8, i8* %12, align 1
  %46 = trunc i8 %45 to i1
  %47 = load i8, i8* %13, align 1
  %48 = trunc i8 %47 to i1
  %49 = call %"class.llvm::BinaryOperator"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE23CreateInsertNUWNSWBinOpENS_11Instruction9BinaryOpsEPNS_5ValueES7_RKNS_5TwineEbb(%"class.llvm::IRBuilder"* %18, i32 15, %"class.llvm::Value"* %42, %"class.llvm::Value"* %43, %"class.llvm::Twine"* dereferenceable(24) %44, i1 zeroext %46, i1 zeroext %48)
  %50 = bitcast %"class.llvm::BinaryOperator"* %49 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %50, %"class.llvm::Value"** %7, align 8
  br label %51

; <label>:51:                                     ; preds = %41, %28
  %52 = load %"class.llvm::Value"*, %"class.llvm::Value"** %7, align 8
  ret %"class.llvm::Value"* %52
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateFMulEPNS_5ValueES5_RKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::MDNode"*) #0 align 2 {
  %6 = alloca %"class.llvm::Value"*, align 8
  %7 = alloca %"class.llvm::IRBuilder"*, align 8
  %8 = alloca %"class.llvm::Value"*, align 8
  %9 = alloca %"class.llvm::Value"*, align 8
  %10 = alloca %"class.llvm::Twine"*, align 8
  %11 = alloca %"class.llvm::MDNode"*, align 8
  %12 = alloca %"class.llvm::Constant"*, align 8
  %13 = alloca %"class.llvm::Constant"*, align 8
  %14 = alloca %"class.llvm::Twine", align 8
  %15 = alloca %"class.llvm::FastMathFlags", align 4
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %7, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %8, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %9, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %10, align 8
  store %"class.llvm::MDNode"* %4, %"class.llvm::MDNode"** %11, align 8
  %16 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %7, align 8
  %17 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %18 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %17)
  store %"class.llvm::Constant"* %18, %"class.llvm::Constant"** %12, align 8
  %19 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %20 = icmp ne %"class.llvm::Constant"* %19, null
  br i1 %20, label %21, label %35

; <label>:21:                                     ; preds = %5
  %22 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %23 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %22)
  store %"class.llvm::Constant"* %23, %"class.llvm::Constant"** %13, align 8
  %24 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %13, align 8
  %25 = icmp ne %"class.llvm::Constant"* %24, null
  br i1 %25, label %26, label %34

; <label>:26:                                     ; preds = %21
  %27 = getelementptr inbounds %"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* %16, i32 0, i32 1
  %28 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %29 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %13, align 8
  %30 = call %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateFMulEPNS_8ConstantES2_(%"class.llvm::ConstantFolder"* %27, %"class.llvm::Constant"* %28, %"class.llvm::Constant"* %29)
  %31 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %32 = call %"class.llvm::Constant"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertEPNS_8ConstantERKNS_5TwineE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Constant"* %30, %"class.llvm::Twine"* dereferenceable(24) %31)
  %33 = bitcast %"class.llvm::Constant"* %32 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %33, %"class.llvm::Value"** %6, align 8
  br label %51

; <label>:34:                                     ; preds = %21
  br label %35

; <label>:35:                                     ; preds = %34, %5
  %36 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %37 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %14, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %38 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator10CreateFMulEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"* %36, %"class.llvm::Value"* %37, %"class.llvm::Twine"* dereferenceable(24) %14)
  %39 = bitcast %"class.llvm::BinaryOperator"* %38 to %"class.llvm::Instruction"*
  %40 = load %"class.llvm::MDNode"*, %"class.llvm::MDNode"** %11, align 8
  %41 = bitcast %"class.llvm::IRBuilder"* %16 to %"class.llvm::IRBuilderBase"*
  %42 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %41, i32 0, i32 5
  %43 = bitcast %"class.llvm::FastMathFlags"* %15 to i8*
  %44 = bitcast %"class.llvm::FastMathFlags"* %42 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %43, i8* %44, i64 4, i32 4, i1 false)
  %45 = getelementptr inbounds %"class.llvm::FastMathFlags", %"class.llvm::FastMathFlags"* %15, i32 0, i32 0
  %46 = load i32, i32* %45, align 4
  %47 = call %"class.llvm::Instruction"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE19AddFPMathAttributesEPNS_11InstructionEPNS_6MDNodeENS_13FastMathFlagsE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Instruction"* %39, %"class.llvm::MDNode"* %40, i32 %46)
  %48 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %49 = call %"class.llvm::Instruction"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_11InstructionEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Instruction"* %47, %"class.llvm::Twine"* dereferenceable(24) %48)
  %50 = bitcast %"class.llvm::Instruction"* %49 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %50, %"class.llvm::Value"** %6, align 8
  br label %51

; <label>:51:                                     ; preds = %35, %26
  %52 = load %"class.llvm::Value"*, %"class.llvm::Value"** %6, align 8
  ret %"class.llvm::Value"* %52
}

; Function Attrs: ssp uwtable
define %"class.llvm::Value"* @_ZN16CGBinaryOperator9CreateDivER13TypeSpecifierR7CGValueS3_RN4llvm9IRBuilderINS4_14ConstantFolderENS4_24IRBuilderDefaultInserterEEE(%struct.TypeSpecifier* dereferenceable(216), %struct.CGValue* dereferenceable(32), %struct.CGValue* dereferenceable(32), %"class.llvm::IRBuilder"* dereferenceable(72)) #0 align 2 {
  %5 = alloca %"class.llvm::Value"*, align 8
  %6 = alloca %struct.TypeSpecifier*, align 8
  %7 = alloca %struct.CGValue*, align 8
  %8 = alloca %struct.CGValue*, align 8
  %9 = alloca %"class.llvm::IRBuilder"*, align 8
  %10 = alloca %"class.llvm::Twine", align 8
  %11 = alloca %"class.llvm::Twine", align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %6, align 8
  store %struct.CGValue* %1, %struct.CGValue** %7, align 8
  store %struct.CGValue* %2, %struct.CGValue** %8, align 8
  store %"class.llvm::IRBuilder"* %3, %"class.llvm::IRBuilder"** %9, align 8
  %12 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %6, align 8
  %13 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %12, i32 0, i32 0
  %14 = load i32, i32* %13, align 8
  switch i32 %14, label %33 [
    i32 0, label %15
    i32 1, label %15
    i32 2, label %24
    i32 3, label %24
  ]

; <label>:15:                                     ; preds = %4, %4
  %16 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %9, align 8
  %17 = load %struct.CGValue*, %struct.CGValue** %7, align 8
  %18 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %17, i32 0, i32 0
  %19 = load %"class.llvm::Value"*, %"class.llvm::Value"** %18, align 8
  %20 = load %struct.CGValue*, %struct.CGValue** %8, align 8
  %21 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %20, i32 0, i32 0
  %22 = load %"class.llvm::Value"*, %"class.llvm::Value"** %21, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %10, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %23 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateSDivEPNS_5ValueES5_RKNS_5TwineEb(%"class.llvm::IRBuilder"* %16, %"class.llvm::Value"* %19, %"class.llvm::Value"* %22, %"class.llvm::Twine"* dereferenceable(24) %10, i1 zeroext false)
  store %"class.llvm::Value"* %23, %"class.llvm::Value"** %5, align 8
  br label %33

; <label>:24:                                     ; preds = %4, %4
  %25 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %9, align 8
  %26 = load %struct.CGValue*, %struct.CGValue** %7, align 8
  %27 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %26, i32 0, i32 0
  %28 = load %"class.llvm::Value"*, %"class.llvm::Value"** %27, align 8
  %29 = load %struct.CGValue*, %struct.CGValue** %8, align 8
  %30 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %29, i32 0, i32 0
  %31 = load %"class.llvm::Value"*, %"class.llvm::Value"** %30, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %11, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %32 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateFDivEPNS_5ValueES5_RKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"* %25, %"class.llvm::Value"* %28, %"class.llvm::Value"* %31, %"class.llvm::Twine"* dereferenceable(24) %11, %"class.llvm::MDNode"* null)
  store %"class.llvm::Value"* %32, %"class.llvm::Value"** %5, align 8
  br label %33

; <label>:33:                                     ; preds = %15, %24, %4
  %34 = load %"class.llvm::Value"*, %"class.llvm::Value"** %5, align 8
  ret %"class.llvm::Value"* %34
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateSDivEPNS_5ValueES5_RKNS_5TwineEb(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24), i1 zeroext) #0 align 2 {
  %6 = alloca %"class.llvm::Value"*, align 8
  %7 = alloca %"class.llvm::IRBuilder"*, align 8
  %8 = alloca %"class.llvm::Value"*, align 8
  %9 = alloca %"class.llvm::Value"*, align 8
  %10 = alloca %"class.llvm::Twine"*, align 8
  %11 = alloca i8, align 1
  %12 = alloca %"class.llvm::Constant"*, align 8
  %13 = alloca %"class.llvm::Constant"*, align 8
  %14 = alloca %"class.llvm::Twine", align 8
  %15 = alloca %"class.llvm::Twine", align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %7, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %8, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %9, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %10, align 8
  %16 = zext i1 %4 to i8
  store i8 %16, i8* %11, align 1
  %17 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %7, align 8
  %18 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %19 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %18)
  store %"class.llvm::Constant"* %19, %"class.llvm::Constant"** %12, align 8
  %20 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %21 = icmp ne %"class.llvm::Constant"* %20, null
  br i1 %21, label %22, label %38

; <label>:22:                                     ; preds = %5
  %23 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %24 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %23)
  store %"class.llvm::Constant"* %24, %"class.llvm::Constant"** %13, align 8
  %25 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %13, align 8
  %26 = icmp ne %"class.llvm::Constant"* %25, null
  br i1 %26, label %27, label %37

; <label>:27:                                     ; preds = %22
  %28 = getelementptr inbounds %"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* %17, i32 0, i32 1
  %29 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %30 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %13, align 8
  %31 = load i8, i8* %11, align 1
  %32 = trunc i8 %31 to i1
  %33 = call %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateSDivEPNS_8ConstantES2_b(%"class.llvm::ConstantFolder"* %28, %"class.llvm::Constant"* %29, %"class.llvm::Constant"* %30, i1 zeroext %32)
  %34 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %35 = call %"class.llvm::Constant"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertEPNS_8ConstantERKNS_5TwineE(%"class.llvm::IRBuilder"* %17, %"class.llvm::Constant"* %33, %"class.llvm::Twine"* dereferenceable(24) %34)
  %36 = bitcast %"class.llvm::Constant"* %35 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %36, %"class.llvm::Value"** %6, align 8
  br label %55

; <label>:37:                                     ; preds = %22
  br label %38

; <label>:38:                                     ; preds = %37, %5
  %39 = load i8, i8* %11, align 1
  %40 = trunc i8 %39 to i1
  br i1 %40, label %48, label %41

; <label>:41:                                     ; preds = %38
  %42 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %43 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %14, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %44 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator10CreateSDivEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"* %42, %"class.llvm::Value"* %43, %"class.llvm::Twine"* dereferenceable(24) %14)
  %45 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %46 = call %"class.llvm::BinaryOperator"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_14BinaryOperatorEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"* %17, %"class.llvm::BinaryOperator"* %44, %"class.llvm::Twine"* dereferenceable(24) %45)
  %47 = bitcast %"class.llvm::BinaryOperator"* %46 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %47, %"class.llvm::Value"** %6, align 8
  br label %55

; <label>:48:                                     ; preds = %38
  %49 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %50 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %15, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %51 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator15CreateExactSDivEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"* %49, %"class.llvm::Value"* %50, %"class.llvm::Twine"* dereferenceable(24) %15)
  %52 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %53 = call %"class.llvm::BinaryOperator"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_14BinaryOperatorEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"* %17, %"class.llvm::BinaryOperator"* %51, %"class.llvm::Twine"* dereferenceable(24) %52)
  %54 = bitcast %"class.llvm::BinaryOperator"* %53 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %54, %"class.llvm::Value"** %6, align 8
  br label %55

; <label>:55:                                     ; preds = %48, %41, %27
  %56 = load %"class.llvm::Value"*, %"class.llvm::Value"** %6, align 8
  ret %"class.llvm::Value"* %56
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateFDivEPNS_5ValueES5_RKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::MDNode"*) #0 align 2 {
  %6 = alloca %"class.llvm::Value"*, align 8
  %7 = alloca %"class.llvm::IRBuilder"*, align 8
  %8 = alloca %"class.llvm::Value"*, align 8
  %9 = alloca %"class.llvm::Value"*, align 8
  %10 = alloca %"class.llvm::Twine"*, align 8
  %11 = alloca %"class.llvm::MDNode"*, align 8
  %12 = alloca %"class.llvm::Constant"*, align 8
  %13 = alloca %"class.llvm::Constant"*, align 8
  %14 = alloca %"class.llvm::Twine", align 8
  %15 = alloca %"class.llvm::FastMathFlags", align 4
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %7, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %8, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %9, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %10, align 8
  store %"class.llvm::MDNode"* %4, %"class.llvm::MDNode"** %11, align 8
  %16 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %7, align 8
  %17 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %18 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %17)
  store %"class.llvm::Constant"* %18, %"class.llvm::Constant"** %12, align 8
  %19 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %20 = icmp ne %"class.llvm::Constant"* %19, null
  br i1 %20, label %21, label %35

; <label>:21:                                     ; preds = %5
  %22 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %23 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %22)
  store %"class.llvm::Constant"* %23, %"class.llvm::Constant"** %13, align 8
  %24 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %13, align 8
  %25 = icmp ne %"class.llvm::Constant"* %24, null
  br i1 %25, label %26, label %34

; <label>:26:                                     ; preds = %21
  %27 = getelementptr inbounds %"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* %16, i32 0, i32 1
  %28 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %29 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %13, align 8
  %30 = call %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateFDivEPNS_8ConstantES2_(%"class.llvm::ConstantFolder"* %27, %"class.llvm::Constant"* %28, %"class.llvm::Constant"* %29)
  %31 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %32 = call %"class.llvm::Constant"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertEPNS_8ConstantERKNS_5TwineE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Constant"* %30, %"class.llvm::Twine"* dereferenceable(24) %31)
  %33 = bitcast %"class.llvm::Constant"* %32 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %33, %"class.llvm::Value"** %6, align 8
  br label %51

; <label>:34:                                     ; preds = %21
  br label %35

; <label>:35:                                     ; preds = %34, %5
  %36 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %37 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %14, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %38 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator10CreateFDivEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"* %36, %"class.llvm::Value"* %37, %"class.llvm::Twine"* dereferenceable(24) %14)
  %39 = bitcast %"class.llvm::BinaryOperator"* %38 to %"class.llvm::Instruction"*
  %40 = load %"class.llvm::MDNode"*, %"class.llvm::MDNode"** %11, align 8
  %41 = bitcast %"class.llvm::IRBuilder"* %16 to %"class.llvm::IRBuilderBase"*
  %42 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %41, i32 0, i32 5
  %43 = bitcast %"class.llvm::FastMathFlags"* %15 to i8*
  %44 = bitcast %"class.llvm::FastMathFlags"* %42 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %43, i8* %44, i64 4, i32 4, i1 false)
  %45 = getelementptr inbounds %"class.llvm::FastMathFlags", %"class.llvm::FastMathFlags"* %15, i32 0, i32 0
  %46 = load i32, i32* %45, align 4
  %47 = call %"class.llvm::Instruction"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE19AddFPMathAttributesEPNS_11InstructionEPNS_6MDNodeENS_13FastMathFlagsE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Instruction"* %39, %"class.llvm::MDNode"* %40, i32 %46)
  %48 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %49 = call %"class.llvm::Instruction"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_11InstructionEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Instruction"* %47, %"class.llvm::Twine"* dereferenceable(24) %48)
  %50 = bitcast %"class.llvm::Instruction"* %49 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %50, %"class.llvm::Value"** %6, align 8
  br label %51

; <label>:51:                                     ; preds = %35, %26
  %52 = load %"class.llvm::Value"*, %"class.llvm::Value"** %6, align 8
  ret %"class.llvm::Value"* %52
}

; Function Attrs: ssp uwtable
define %"class.llvm::Value"* @_ZN16CGBinaryOperator9CreateRemER13TypeSpecifierR7CGValueS3_RN4llvm9IRBuilderINS4_14ConstantFolderENS4_24IRBuilderDefaultInserterEEE(%struct.TypeSpecifier* dereferenceable(216), %struct.CGValue* dereferenceable(32), %struct.CGValue* dereferenceable(32), %"class.llvm::IRBuilder"* dereferenceable(72)) #0 align 2 {
  %5 = alloca %"class.llvm::Value"*, align 8
  %6 = alloca %struct.TypeSpecifier*, align 8
  %7 = alloca %struct.CGValue*, align 8
  %8 = alloca %struct.CGValue*, align 8
  %9 = alloca %"class.llvm::IRBuilder"*, align 8
  %10 = alloca %"class.llvm::Twine", align 8
  %11 = alloca %"class.llvm::Twine", align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %6, align 8
  store %struct.CGValue* %1, %struct.CGValue** %7, align 8
  store %struct.CGValue* %2, %struct.CGValue** %8, align 8
  store %"class.llvm::IRBuilder"* %3, %"class.llvm::IRBuilder"** %9, align 8
  %12 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %6, align 8
  %13 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %12, i32 0, i32 0
  %14 = load i32, i32* %13, align 8
  switch i32 %14, label %33 [
    i32 0, label %15
    i32 1, label %15
    i32 2, label %24
    i32 3, label %24
  ]

; <label>:15:                                     ; preds = %4, %4
  %16 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %9, align 8
  %17 = load %struct.CGValue*, %struct.CGValue** %7, align 8
  %18 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %17, i32 0, i32 0
  %19 = load %"class.llvm::Value"*, %"class.llvm::Value"** %18, align 8
  %20 = load %struct.CGValue*, %struct.CGValue** %8, align 8
  %21 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %20, i32 0, i32 0
  %22 = load %"class.llvm::Value"*, %"class.llvm::Value"** %21, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %10, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %23 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateSRemEPNS_5ValueES5_RKNS_5TwineE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Value"* %19, %"class.llvm::Value"* %22, %"class.llvm::Twine"* dereferenceable(24) %10)
  store %"class.llvm::Value"* %23, %"class.llvm::Value"** %5, align 8
  br label %33

; <label>:24:                                     ; preds = %4, %4
  %25 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %9, align 8
  %26 = load %struct.CGValue*, %struct.CGValue** %7, align 8
  %27 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %26, i32 0, i32 0
  %28 = load %"class.llvm::Value"*, %"class.llvm::Value"** %27, align 8
  %29 = load %struct.CGValue*, %struct.CGValue** %8, align 8
  %30 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %29, i32 0, i32 0
  %31 = load %"class.llvm::Value"*, %"class.llvm::Value"** %30, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %11, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %32 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateFRemEPNS_5ValueES5_RKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"* %25, %"class.llvm::Value"* %28, %"class.llvm::Value"* %31, %"class.llvm::Twine"* dereferenceable(24) %11, %"class.llvm::MDNode"* null)
  store %"class.llvm::Value"* %32, %"class.llvm::Value"** %5, align 8
  br label %33

; <label>:33:                                     ; preds = %15, %24, %4
  %34 = load %"class.llvm::Value"*, %"class.llvm::Value"** %5, align 8
  ret %"class.llvm::Value"* %34
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateSRemEPNS_5ValueES5_RKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %5 = alloca %"class.llvm::Value"*, align 8
  %6 = alloca %"class.llvm::IRBuilder"*, align 8
  %7 = alloca %"class.llvm::Value"*, align 8
  %8 = alloca %"class.llvm::Value"*, align 8
  %9 = alloca %"class.llvm::Twine"*, align 8
  %10 = alloca %"class.llvm::Constant"*, align 8
  %11 = alloca %"class.llvm::Constant"*, align 8
  %12 = alloca %"class.llvm::Twine", align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %6, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %7, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %8, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %9, align 8
  %13 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %6, align 8
  %14 = load %"class.llvm::Value"*, %"class.llvm::Value"** %7, align 8
  %15 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %14)
  store %"class.llvm::Constant"* %15, %"class.llvm::Constant"** %10, align 8
  %16 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %10, align 8
  %17 = icmp ne %"class.llvm::Constant"* %16, null
  br i1 %17, label %18, label %32

; <label>:18:                                     ; preds = %4
  %19 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %20 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %19)
  store %"class.llvm::Constant"* %20, %"class.llvm::Constant"** %11, align 8
  %21 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %11, align 8
  %22 = icmp ne %"class.llvm::Constant"* %21, null
  br i1 %22, label %23, label %31

; <label>:23:                                     ; preds = %18
  %24 = getelementptr inbounds %"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* %13, i32 0, i32 1
  %25 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %10, align 8
  %26 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %11, align 8
  %27 = call %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateSRemEPNS_8ConstantES2_(%"class.llvm::ConstantFolder"* %24, %"class.llvm::Constant"* %25, %"class.llvm::Constant"* %26)
  %28 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %9, align 8
  %29 = call %"class.llvm::Constant"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertEPNS_8ConstantERKNS_5TwineE(%"class.llvm::IRBuilder"* %13, %"class.llvm::Constant"* %27, %"class.llvm::Twine"* dereferenceable(24) %28)
  %30 = bitcast %"class.llvm::Constant"* %29 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %30, %"class.llvm::Value"** %5, align 8
  br label %39

; <label>:31:                                     ; preds = %18
  br label %32

; <label>:32:                                     ; preds = %31, %4
  %33 = load %"class.llvm::Value"*, %"class.llvm::Value"** %7, align 8
  %34 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %12, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %35 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator10CreateSRemEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"* %33, %"class.llvm::Value"* %34, %"class.llvm::Twine"* dereferenceable(24) %12)
  %36 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %9, align 8
  %37 = call %"class.llvm::BinaryOperator"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_14BinaryOperatorEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"* %13, %"class.llvm::BinaryOperator"* %35, %"class.llvm::Twine"* dereferenceable(24) %36)
  %38 = bitcast %"class.llvm::BinaryOperator"* %37 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %38, %"class.llvm::Value"** %5, align 8
  br label %39

; <label>:39:                                     ; preds = %32, %23
  %40 = load %"class.llvm::Value"*, %"class.llvm::Value"** %5, align 8
  ret %"class.llvm::Value"* %40
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateFRemEPNS_5ValueES5_RKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::MDNode"*) #0 align 2 {
  %6 = alloca %"class.llvm::Value"*, align 8
  %7 = alloca %"class.llvm::IRBuilder"*, align 8
  %8 = alloca %"class.llvm::Value"*, align 8
  %9 = alloca %"class.llvm::Value"*, align 8
  %10 = alloca %"class.llvm::Twine"*, align 8
  %11 = alloca %"class.llvm::MDNode"*, align 8
  %12 = alloca %"class.llvm::Constant"*, align 8
  %13 = alloca %"class.llvm::Constant"*, align 8
  %14 = alloca %"class.llvm::Twine", align 8
  %15 = alloca %"class.llvm::FastMathFlags", align 4
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %7, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %8, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %9, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %10, align 8
  store %"class.llvm::MDNode"* %4, %"class.llvm::MDNode"** %11, align 8
  %16 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %7, align 8
  %17 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %18 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %17)
  store %"class.llvm::Constant"* %18, %"class.llvm::Constant"** %12, align 8
  %19 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %20 = icmp ne %"class.llvm::Constant"* %19, null
  br i1 %20, label %21, label %35

; <label>:21:                                     ; preds = %5
  %22 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %23 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %22)
  store %"class.llvm::Constant"* %23, %"class.llvm::Constant"** %13, align 8
  %24 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %13, align 8
  %25 = icmp ne %"class.llvm::Constant"* %24, null
  br i1 %25, label %26, label %34

; <label>:26:                                     ; preds = %21
  %27 = getelementptr inbounds %"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* %16, i32 0, i32 1
  %28 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %29 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %13, align 8
  %30 = call %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateFRemEPNS_8ConstantES2_(%"class.llvm::ConstantFolder"* %27, %"class.llvm::Constant"* %28, %"class.llvm::Constant"* %29)
  %31 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %32 = call %"class.llvm::Constant"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertEPNS_8ConstantERKNS_5TwineE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Constant"* %30, %"class.llvm::Twine"* dereferenceable(24) %31)
  %33 = bitcast %"class.llvm::Constant"* %32 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %33, %"class.llvm::Value"** %6, align 8
  br label %51

; <label>:34:                                     ; preds = %21
  br label %35

; <label>:35:                                     ; preds = %34, %5
  %36 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %37 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %14, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %38 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator10CreateFRemEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"* %36, %"class.llvm::Value"* %37, %"class.llvm::Twine"* dereferenceable(24) %14)
  %39 = bitcast %"class.llvm::BinaryOperator"* %38 to %"class.llvm::Instruction"*
  %40 = load %"class.llvm::MDNode"*, %"class.llvm::MDNode"** %11, align 8
  %41 = bitcast %"class.llvm::IRBuilder"* %16 to %"class.llvm::IRBuilderBase"*
  %42 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %41, i32 0, i32 5
  %43 = bitcast %"class.llvm::FastMathFlags"* %15 to i8*
  %44 = bitcast %"class.llvm::FastMathFlags"* %42 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %43, i8* %44, i64 4, i32 4, i1 false)
  %45 = getelementptr inbounds %"class.llvm::FastMathFlags", %"class.llvm::FastMathFlags"* %15, i32 0, i32 0
  %46 = load i32, i32* %45, align 4
  %47 = call %"class.llvm::Instruction"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE19AddFPMathAttributesEPNS_11InstructionEPNS_6MDNodeENS_13FastMathFlagsE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Instruction"* %39, %"class.llvm::MDNode"* %40, i32 %46)
  %48 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %49 = call %"class.llvm::Instruction"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_11InstructionEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Instruction"* %47, %"class.llvm::Twine"* dereferenceable(24) %48)
  %50 = bitcast %"class.llvm::Instruction"* %49 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %50, %"class.llvm::Value"** %6, align 8
  br label %51

; <label>:51:                                     ; preds = %35, %26
  %52 = load %"class.llvm::Value"*, %"class.llvm::Value"** %6, align 8
  ret %"class.llvm::Value"* %52
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm5TwineC2EPKc(%"class.llvm::Twine"*, i8*) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::Twine"*, align 8
  %4 = alloca i8*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %3, align 8
  store i8* %1, i8** %4, align 8
  %5 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %3, align 8
  %6 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %5, i32 0, i32 0
  %7 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %5, i32 0, i32 1
  %8 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %5, i32 0, i32 3
  store i8 1, i8* %8, align 1
  %9 = load i8*, i8** %4, align 8
  %10 = getelementptr inbounds i8, i8* %9, i64 0
  %11 = load i8, i8* %10, align 1
  %12 = sext i8 %11 to i32
  %13 = icmp ne i32 %12, 0
  br i1 %13, label %14, label %19

; <label>:14:                                     ; preds = %2
  %15 = load i8*, i8** %4, align 8
  %16 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %5, i32 0, i32 0
  %17 = bitcast %"union.llvm::Twine::Child"* %16 to i8**
  store i8* %15, i8** %17, align 8
  %18 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %5, i32 0, i32 2
  store i8 3, i8* %18, align 8
  br label %21

; <label>:19:                                     ; preds = %2
  %20 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %5, i32 0, i32 2
  store i8 1, i8* %20, align 8
  br label %21

; <label>:21:                                     ; preds = %19, %14
  %22 = call zeroext i1 @_ZNK4llvm5Twine7isValidEv(%"class.llvm::Twine"* %5)
  br i1 %22, label %23, label %24

; <label>:23:                                     ; preds = %21
  br label %24

; <label>:24:                                     ; preds = %23, %21
  %25 = phi i1 [ false, %21 ], [ true, %23 ]
  %26 = xor i1 %25, true
  br i1 %26, label %27, label %29

; <label>:27:                                     ; preds = %24
  call void @__assert_rtn(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__._ZN4llvm5TwineC2EPKc, i32 0, i32 0), i8* getelementptr inbounds ([56 x i8], [56 x i8]* @.str.2, i32 0, i32 0), i32 274, i8* getelementptr inbounds ([30 x i8], [30 x i8]* @.str.3, i32 0, i32 0)) #8
  unreachable
                                                  ; No predecessors!
  br label %30

; <label>:29:                                     ; preds = %24
  br label %30

; <label>:30:                                     ; preds = %29, %28
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZNK4llvm5Twine7isValidEv(%"class.llvm::Twine"*) #0 align 2 {
  %2 = alloca i1, align 1
  %3 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %3, align 8
  %4 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %3, align 8
  %5 = call zeroext i1 @_ZNK4llvm5Twine9isNullaryEv(%"class.llvm::Twine"* %4)
  br i1 %5, label %6, label %11

; <label>:6:                                      ; preds = %1
  %7 = call zeroext i8 @_ZNK4llvm5Twine10getRHSKindEv(%"class.llvm::Twine"* %4)
  %8 = zext i8 %7 to i32
  %9 = icmp ne i32 %8, 1
  br i1 %9, label %10, label %11

; <label>:10:                                     ; preds = %6
  store i1 false, i1* %2, align 1
  br label %46

; <label>:11:                                     ; preds = %6, %1
  %12 = call zeroext i8 @_ZNK4llvm5Twine10getRHSKindEv(%"class.llvm::Twine"* %4)
  %13 = zext i8 %12 to i32
  %14 = icmp eq i32 %13, 0
  br i1 %14, label %15, label %16

; <label>:15:                                     ; preds = %11
  store i1 false, i1* %2, align 1
  br label %46

; <label>:16:                                     ; preds = %11
  %17 = call zeroext i8 @_ZNK4llvm5Twine10getRHSKindEv(%"class.llvm::Twine"* %4)
  %18 = zext i8 %17 to i32
  %19 = icmp ne i32 %18, 1
  br i1 %19, label %20, label %25

; <label>:20:                                     ; preds = %16
  %21 = call zeroext i8 @_ZNK4llvm5Twine10getLHSKindEv(%"class.llvm::Twine"* %4)
  %22 = zext i8 %21 to i32
  %23 = icmp eq i32 %22, 1
  br i1 %23, label %24, label %25

; <label>:24:                                     ; preds = %20
  store i1 false, i1* %2, align 1
  br label %46

; <label>:25:                                     ; preds = %20, %16
  %26 = call zeroext i8 @_ZNK4llvm5Twine10getLHSKindEv(%"class.llvm::Twine"* %4)
  %27 = zext i8 %26 to i32
  %28 = icmp eq i32 %27, 2
  br i1 %28, label %29, label %35

; <label>:29:                                     ; preds = %25
  %30 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %4, i32 0, i32 0
  %31 = bitcast %"union.llvm::Twine::Child"* %30 to %"class.llvm::Twine"**
  %32 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %31, align 8
  %33 = call zeroext i1 @_ZNK4llvm5Twine8isBinaryEv(%"class.llvm::Twine"* %32)
  br i1 %33, label %35, label %34

; <label>:34:                                     ; preds = %29
  store i1 false, i1* %2, align 1
  br label %46

; <label>:35:                                     ; preds = %29, %25
  %36 = call zeroext i8 @_ZNK4llvm5Twine10getRHSKindEv(%"class.llvm::Twine"* %4)
  %37 = zext i8 %36 to i32
  %38 = icmp eq i32 %37, 2
  br i1 %38, label %39, label %45

; <label>:39:                                     ; preds = %35
  %40 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %4, i32 0, i32 1
  %41 = bitcast %"union.llvm::Twine::Child"* %40 to %"class.llvm::Twine"**
  %42 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %41, align 8
  %43 = call zeroext i1 @_ZNK4llvm5Twine8isBinaryEv(%"class.llvm::Twine"* %42)
  br i1 %43, label %45, label %44

; <label>:44:                                     ; preds = %39
  store i1 false, i1* %2, align 1
  br label %46

; <label>:45:                                     ; preds = %39, %35
  store i1 true, i1* %2, align 1
  br label %46

; <label>:46:                                     ; preds = %45, %44, %34, %24, %15, %10
  %47 = load i1, i1* %2, align 1
  ret i1 %47
}

; Function Attrs: noreturn
declare void @__assert_rtn(i8*, i8*, i32, i8*) #1

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZNK4llvm5Twine9isNullaryEv(%"class.llvm::Twine"*) #0 align 2 {
  %2 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %2, align 8
  %3 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %2, align 8
  %4 = call zeroext i1 @_ZNK4llvm5Twine6isNullEv(%"class.llvm::Twine"* %3)
  br i1 %4, label %7, label %5

; <label>:5:                                      ; preds = %1
  %6 = call zeroext i1 @_ZNK4llvm5Twine7isEmptyEv(%"class.llvm::Twine"* %3)
  br label %7

; <label>:7:                                      ; preds = %5, %1
  %8 = phi i1 [ true, %1 ], [ %6, %5 ]
  ret i1 %8
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr zeroext i8 @_ZNK4llvm5Twine10getRHSKindEv(%"class.llvm::Twine"*) #2 align 2 {
  %2 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %2, align 8
  %3 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %3, i32 0, i32 3
  %5 = load i8, i8* %4, align 1
  ret i8 %5
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr zeroext i8 @_ZNK4llvm5Twine10getLHSKindEv(%"class.llvm::Twine"*) #2 align 2 {
  %2 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %2, align 8
  %3 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %3, i32 0, i32 2
  %5 = load i8, i8* %4, align 8
  ret i8 %5
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZNK4llvm5Twine8isBinaryEv(%"class.llvm::Twine"*) #2 align 2 {
  %2 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %2, align 8
  %3 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %2, align 8
  %4 = call zeroext i8 @_ZNK4llvm5Twine10getLHSKindEv(%"class.llvm::Twine"* %3)
  %5 = zext i8 %4 to i32
  %6 = icmp ne i32 %5, 0
  br i1 %6, label %7, label %11

; <label>:7:                                      ; preds = %1
  %8 = call zeroext i8 @_ZNK4llvm5Twine10getRHSKindEv(%"class.llvm::Twine"* %3)
  %9 = zext i8 %8 to i32
  %10 = icmp ne i32 %9, 1
  br label %11

; <label>:11:                                     ; preds = %7, %1
  %12 = phi i1 [ false, %1 ], [ %10, %7 ]
  ret i1 %12
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZNK4llvm5Twine6isNullEv(%"class.llvm::Twine"*) #0 align 2 {
  %2 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %2, align 8
  %3 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %2, align 8
  %4 = call zeroext i8 @_ZNK4llvm5Twine10getLHSKindEv(%"class.llvm::Twine"* %3)
  %5 = zext i8 %4 to i32
  %6 = icmp eq i32 %5, 0
  ret i1 %6
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZNK4llvm5Twine7isEmptyEv(%"class.llvm::Twine"*) #0 align 2 {
  %2 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %2, align 8
  %3 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %2, align 8
  %4 = call zeroext i8 @_ZNK4llvm5Twine10getLHSKindEv(%"class.llvm::Twine"* %3)
  %5 = zext i8 %4 to i32
  %6 = icmp eq i32 %5, 1
  ret i1 %6
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"*) #3 {
  %2 = alloca %"class.llvm::Value"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %2, align 8
  %3 = call zeroext i1 @_ZN4llvm3isaINS_8ConstantEPNS_5ValueEEEbRKT0_(%"class.llvm::Value"** dereferenceable(8) %2)
  br i1 %3, label %4, label %7

; <label>:4:                                      ; preds = %1
  %5 = load %"class.llvm::Value"*, %"class.llvm::Value"** %2, align 8
  %6 = call %"class.llvm::Constant"* @_ZN4llvm4castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %5)
  br label %8

; <label>:7:                                      ; preds = %1
  br label %8

; <label>:8:                                      ; preds = %7, %4
  %9 = phi %"class.llvm::Constant"* [ %6, %4 ], [ null, %7 ]
  ret %"class.llvm::Constant"* %9
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertEPNS_8ConstantERKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Constant"*, %"class.llvm::Twine"* dereferenceable(24)) #2 align 2 {
  %4 = alloca %"class.llvm::IRBuilder"*, align 8
  %5 = alloca %"class.llvm::Constant"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %4, align 8
  store %"class.llvm::Constant"* %1, %"class.llvm::Constant"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %7 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %4, align 8
  %8 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %5, align 8
  ret %"class.llvm::Constant"* %8
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder9CreateAddEPNS_8ConstantES2_bb(%"class.llvm::ConstantFolder"*, %"class.llvm::Constant"*, %"class.llvm::Constant"*, i1 zeroext, i1 zeroext) #0 align 2 {
  %6 = alloca %"class.llvm::ConstantFolder"*, align 8
  %7 = alloca %"class.llvm::Constant"*, align 8
  %8 = alloca %"class.llvm::Constant"*, align 8
  %9 = alloca i8, align 1
  %10 = alloca i8, align 1
  store %"class.llvm::ConstantFolder"* %0, %"class.llvm::ConstantFolder"** %6, align 8
  store %"class.llvm::Constant"* %1, %"class.llvm::Constant"** %7, align 8
  store %"class.llvm::Constant"* %2, %"class.llvm::Constant"** %8, align 8
  %11 = zext i1 %3 to i8
  store i8 %11, i8* %9, align 1
  %12 = zext i1 %4 to i8
  store i8 %12, i8* %10, align 1
  %13 = load %"class.llvm::ConstantFolder"*, %"class.llvm::ConstantFolder"** %6, align 8
  %14 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %7, align 8
  %15 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %8, align 8
  %16 = load i8, i8* %9, align 1
  %17 = trunc i8 %16 to i1
  %18 = load i8, i8* %10, align 1
  %19 = trunc i8 %18 to i1
  %20 = call %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr6getAddEPNS_8ConstantES2_bb(%"class.llvm::Constant"* %14, %"class.llvm::Constant"* %15, i1 zeroext %17, i1 zeroext %19)
  ret %"class.llvm::Constant"* %20
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::BinaryOperator"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE23CreateInsertNUWNSWBinOpENS_11Instruction9BinaryOpsEPNS_5ValueES7_RKNS_5TwineEbb(%"class.llvm::IRBuilder"*, i32, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24), i1 zeroext, i1 zeroext) #0 align 2 {
  %8 = alloca %"class.llvm::IRBuilder"*, align 8
  %9 = alloca i32, align 4
  %10 = alloca %"class.llvm::Value"*, align 8
  %11 = alloca %"class.llvm::Value"*, align 8
  %12 = alloca %"class.llvm::Twine"*, align 8
  %13 = alloca i8, align 1
  %14 = alloca i8, align 1
  %15 = alloca %"class.llvm::BinaryOperator"*, align 8
  %16 = alloca %"class.llvm::Twine", align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %8, align 8
  store i32 %1, i32* %9, align 4
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %10, align 8
  store %"class.llvm::Value"* %3, %"class.llvm::Value"** %11, align 8
  store %"class.llvm::Twine"* %4, %"class.llvm::Twine"** %12, align 8
  %17 = zext i1 %5 to i8
  store i8 %17, i8* %13, align 1
  %18 = zext i1 %6 to i8
  store i8 %18, i8* %14, align 1
  %19 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %8, align 8
  %20 = load i32, i32* %9, align 4
  %21 = load %"class.llvm::Value"*, %"class.llvm::Value"** %10, align 8
  %22 = load %"class.llvm::Value"*, %"class.llvm::Value"** %11, align 8
  call void @_ZN4llvm5TwineC1Ev(%"class.llvm::Twine"* %16)
  %23 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator6CreateENS_11Instruction9BinaryOpsEPNS_5ValueES4_RKNS_5TwineEPS1_(i32 %20, %"class.llvm::Value"* %21, %"class.llvm::Value"* %22, %"class.llvm::Twine"* dereferenceable(24) %16, %"class.llvm::Instruction"* null)
  %24 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %12, align 8
  %25 = call %"class.llvm::BinaryOperator"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_14BinaryOperatorEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"* %19, %"class.llvm::BinaryOperator"* %23, %"class.llvm::Twine"* dereferenceable(24) %24)
  store %"class.llvm::BinaryOperator"* %25, %"class.llvm::BinaryOperator"** %15, align 8
  %26 = load i8, i8* %13, align 1
  %27 = trunc i8 %26 to i1
  br i1 %27, label %28, label %31

; <label>:28:                                     ; preds = %7
  %29 = load %"class.llvm::BinaryOperator"*, %"class.llvm::BinaryOperator"** %15, align 8
  %30 = bitcast %"class.llvm::BinaryOperator"* %29 to %"class.llvm::Instruction"*
  call void @_ZN4llvm11Instruction20setHasNoUnsignedWrapEb(%"class.llvm::Instruction"* %30, i1 zeroext true)
  br label %31

; <label>:31:                                     ; preds = %28, %7
  %32 = load i8, i8* %14, align 1
  %33 = trunc i8 %32 to i1
  br i1 %33, label %34, label %37

; <label>:34:                                     ; preds = %31
  %35 = load %"class.llvm::BinaryOperator"*, %"class.llvm::BinaryOperator"** %15, align 8
  %36 = bitcast %"class.llvm::BinaryOperator"* %35 to %"class.llvm::Instruction"*
  call void @_ZN4llvm11Instruction18setHasNoSignedWrapEb(%"class.llvm::Instruction"* %36, i1 zeroext true)
  br label %37

; <label>:37:                                     ; preds = %34, %31
  %38 = load %"class.llvm::BinaryOperator"*, %"class.llvm::BinaryOperator"** %15, align 8
  ret %"class.llvm::BinaryOperator"* %38
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm3isaINS_8ConstantEPNS_5ValueEEEbRKT0_(%"class.llvm::Value"** dereferenceable(8)) #3 {
  %2 = alloca %"class.llvm::Value"**, align 8
  store %"class.llvm::Value"** %0, %"class.llvm::Value"*** %2, align 8
  %3 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_8ConstantEKPNS_5ValueEPKS2_E4doitERS4_(%"class.llvm::Value"** dereferenceable(8) %3)
  ret i1 %4
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZN4llvm4castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"*) #3 {
  %2 = alloca %"class.llvm::Value"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %2, align 8
  %3 = call zeroext i1 @_ZN4llvm3isaINS_8ConstantEPNS_5ValueEEEbRKT0_(%"class.llvm::Value"** dereferenceable(8) %2)
  br i1 %3, label %4, label %5

; <label>:4:                                      ; preds = %1
  br label %5

; <label>:5:                                      ; preds = %4, %1
  %6 = phi i1 [ false, %1 ], [ true, %4 ]
  %7 = xor i1 %6, true
  br i1 %7, label %8, label %10

; <label>:8:                                      ; preds = %5
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm4castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.5, i32 0, i32 0), i32 236, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.8, i32 0, i32 0)) #8
  unreachable
                                                  ; No predecessors!
  br label %11

; <label>:10:                                     ; preds = %5
  br label %11

; <label>:11:                                     ; preds = %10, %9
  %12 = call %"class.llvm::Constant"* @_ZN4llvm16cast_convert_valINS_8ConstantEPNS_5ValueES3_E4doitERKS3_(%"class.llvm::Value"** dereferenceable(8) %2)
  ret %"class.llvm::Constant"* %12
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_8ConstantEKPNS_5ValueEPKS2_E4doitERS4_(%"class.llvm::Value"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Value"**, align 8
  %3 = alloca %"class.llvm::Value"*, align 8
  store %"class.llvm::Value"** %0, %"class.llvm::Value"*** %2, align 8
  %4 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %2, align 8
  %5 = call %"class.llvm::Value"* @_ZN4llvm13simplify_typeIKPNS_5ValueEE18getSimplifiedValueERS3_(%"class.llvm::Value"** dereferenceable(8) %4)
  store %"class.llvm::Value"* %5, %"class.llvm::Value"** %3, align 8
  %6 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_8ConstantEPKNS_5ValueES4_E4doitERKS4_(%"class.llvm::Value"** dereferenceable(8) %3)
  ret i1 %6
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_8ConstantEPKNS_5ValueES4_E4doitERKS4_(%"class.llvm::Value"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Value"**, align 8
  store %"class.llvm::Value"** %0, %"class.llvm::Value"*** %2, align 8
  %3 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %2, align 8
  %4 = load %"class.llvm::Value"*, %"class.llvm::Value"** %3, align 8
  %5 = call zeroext i1 @_ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_(%"class.llvm::Value"* %4)
  ret i1 %5
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm13simplify_typeIKPNS_5ValueEE18getSimplifiedValueERS3_(%"class.llvm::Value"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Value"**, align 8
  store %"class.llvm::Value"** %0, %"class.llvm::Value"*** %2, align 8
  %3 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %2, align 8
  %4 = call dereferenceable(8) %"class.llvm::Value"** @_ZN4llvm13simplify_typeIPNS_5ValueEE18getSimplifiedValueERS2_(%"class.llvm::Value"** dereferenceable(8) %3)
  %5 = load %"class.llvm::Value"*, %"class.llvm::Value"** %4, align 8
  ret %"class.llvm::Value"* %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_(%"class.llvm::Value"*) #3 align 2 {
  %2 = alloca %"class.llvm::Value"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %2, align 8
  %3 = load %"class.llvm::Value"*, %"class.llvm::Value"** %2, align 8
  %4 = icmp ne %"class.llvm::Value"* %3, null
  br i1 %4, label %5, label %6

; <label>:5:                                      ; preds = %1
  br label %6

; <label>:6:                                      ; preds = %5, %1
  %7 = phi i1 [ false, %1 ], [ true, %5 ]
  %8 = xor i1 %7, true
  br i1 %8, label %9, label %11

; <label>:9:                                      ; preds = %6
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.5, i32 0, i32 0), i32 95, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.6, i32 0, i32 0)) #8
  unreachable
                                                  ; No predecessors!
  br label %12

; <label>:11:                                     ; preds = %6
  br label %12

; <label>:12:                                     ; preds = %11, %10
  %13 = load %"class.llvm::Value"*, %"class.llvm::Value"** %2, align 8
  %14 = call zeroext i1 @_ZN4llvm8isa_implINS_8ConstantENS_5ValueEvE4doitERKS2_(%"class.llvm::Value"* dereferenceable(32) %13)
  ret i1 %14
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm8isa_implINS_8ConstantENS_5ValueEvE4doitERKS2_(%"class.llvm::Value"* dereferenceable(32)) #3 align 2 {
  %2 = alloca %"class.llvm::Value"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %2, align 8
  %3 = load %"class.llvm::Value"*, %"class.llvm::Value"** %2, align 8
  %4 = call i32 @_ZNK4llvm5Value10getValueIDEv(%"class.llvm::Value"* %3)
  %5 = icmp uge i32 %4, 5
  br i1 %5, label %6, label %10

; <label>:6:                                      ; preds = %1
  %7 = load %"class.llvm::Value"*, %"class.llvm::Value"** %2, align 8
  %8 = call i32 @_ZNK4llvm5Value10getValueIDEv(%"class.llvm::Value"* %7)
  %9 = icmp ule i32 %8, 21
  br label %10

; <label>:10:                                     ; preds = %6, %1
  %11 = phi i1 [ false, %1 ], [ %9, %6 ]
  ret i1 %11
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i32 @_ZNK4llvm5Value10getValueIDEv(%"class.llvm::Value"*) #2 align 2 {
  %2 = alloca %"class.llvm::Value"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %2, align 8
  %3 = load %"class.llvm::Value"*, %"class.llvm::Value"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Value", %"class.llvm::Value"* %3, i32 0, i32 3
  %5 = load i8, i8* %4, align 8
  %6 = zext i8 %5 to i32
  ret i32 %6
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr dereferenceable(8) %"class.llvm::Value"** @_ZN4llvm13simplify_typeIPNS_5ValueEE18getSimplifiedValueERS2_(%"class.llvm::Value"** dereferenceable(8)) #2 align 2 {
  %2 = alloca %"class.llvm::Value"**, align 8
  store %"class.llvm::Value"** %0, %"class.llvm::Value"*** %2, align 8
  %3 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %2, align 8
  ret %"class.llvm::Value"** %3
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZN4llvm16cast_convert_valINS_8ConstantEPNS_5ValueES3_E4doitERKS3_(%"class.llvm::Value"** dereferenceable(8)) #2 align 2 {
  %2 = alloca %"class.llvm::Value"**, align 8
  %3 = alloca %"class.llvm::Constant"*, align 8
  store %"class.llvm::Value"** %0, %"class.llvm::Value"*** %2, align 8
  %4 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %2, align 8
  %5 = load %"class.llvm::Value"*, %"class.llvm::Value"** %4, align 8
  %6 = bitcast %"class.llvm::Value"* %5 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %6, %"class.llvm::Constant"** %3, align 8
  %7 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %3, align 8
  ret %"class.llvm::Constant"* %7
}

declare %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr6getAddEPNS_8ConstantES2_bb(%"class.llvm::Constant"*, %"class.llvm::Constant"*, i1 zeroext, i1 zeroext) #4

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::BinaryOperator"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_14BinaryOperatorEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::BinaryOperator"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %4 = alloca %"class.llvm::IRBuilder"*, align 8
  %5 = alloca %"class.llvm::BinaryOperator"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  %7 = alloca %"class.llvm::ilist_iterator", align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %4, align 8
  store %"class.llvm::BinaryOperator"* %1, %"class.llvm::BinaryOperator"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %8 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %4, align 8
  %9 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderDefaultInserter"*
  %10 = load %"class.llvm::BinaryOperator"*, %"class.llvm::BinaryOperator"** %5, align 8
  %11 = bitcast %"class.llvm::BinaryOperator"* %10 to %"class.llvm::Instruction"*
  %12 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %6, align 8
  %13 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderBase"*
  %14 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %13, i32 0, i32 1
  %15 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %14, align 8
  %16 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderBase"*
  %17 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %16, i32 0, i32 2
  %18 = bitcast %"class.llvm::ilist_iterator"* %7 to i8*
  %19 = bitcast %"class.llvm::ilist_iterator"* %17 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %18, i8* %19, i64 8, i32 8, i1 false)
  %20 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %7, i32 0, i32 0
  %21 = load %"class.llvm::ilist_node_impl.26"*, %"class.llvm::ilist_node_impl.26"** %20, align 8
  call void @_ZNK4llvm24IRBuilderDefaultInserter12InsertHelperEPNS_11InstructionERKNS_5TwineEPNS_10BasicBlockENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEE(%"class.llvm::IRBuilderDefaultInserter"* %9, %"class.llvm::Instruction"* %11, %"class.llvm::Twine"* dereferenceable(24) %12, %"class.llvm::BasicBlock"* %15, %"class.llvm::ilist_node_impl.26"* %21)
  %22 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderBase"*
  %23 = load %"class.llvm::BinaryOperator"*, %"class.llvm::BinaryOperator"** %5, align 8
  %24 = bitcast %"class.llvm::BinaryOperator"* %23 to %"class.llvm::Instruction"*
  call void @_ZNK4llvm13IRBuilderBase20SetInstDebugLocationEPNS_11InstructionE(%"class.llvm::IRBuilderBase"* %22, %"class.llvm::Instruction"* %24)
  %25 = load %"class.llvm::BinaryOperator"*, %"class.llvm::BinaryOperator"** %5, align 8
  ret %"class.llvm::BinaryOperator"* %25
}

declare %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator6CreateENS_11Instruction9BinaryOpsEPNS_5ValueES4_RKNS_5TwineEPS1_(i32, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::Instruction"*) #4

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm5TwineC1Ev(%"class.llvm::Twine"*) unnamed_addr #0 align 2 {
  %2 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %2, align 8
  %3 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %2, align 8
  call void @_ZN4llvm5TwineC2Ev(%"class.llvm::Twine"* %3)
  ret void
}

declare void @_ZN4llvm11Instruction20setHasNoUnsignedWrapEb(%"class.llvm::Instruction"*, i1 zeroext) #4

declare void @_ZN4llvm11Instruction18setHasNoSignedWrapEb(%"class.llvm::Instruction"*, i1 zeroext) #4

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNK4llvm24IRBuilderDefaultInserter12InsertHelperEPNS_11InstructionERKNS_5TwineEPNS_10BasicBlockENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEE(%"class.llvm::IRBuilderDefaultInserter"*, %"class.llvm::Instruction"*, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::BasicBlock"*, %"class.llvm::ilist_node_impl.26"*) #0 align 2 {
  %6 = alloca %"class.llvm::ilist_iterator", align 8
  %7 = alloca %"class.llvm::IRBuilderDefaultInserter"*, align 8
  %8 = alloca %"class.llvm::Instruction"*, align 8
  %9 = alloca %"class.llvm::Twine"*, align 8
  %10 = alloca %"class.llvm::BasicBlock"*, align 8
  %11 = alloca %"class.llvm::ilist_iterator", align 8
  %12 = alloca %"class.llvm::ilist_iterator", align 8
  %13 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %6, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.26"* %4, %"class.llvm::ilist_node_impl.26"** %13, align 8
  store %"class.llvm::IRBuilderDefaultInserter"* %0, %"class.llvm::IRBuilderDefaultInserter"** %7, align 8
  store %"class.llvm::Instruction"* %1, %"class.llvm::Instruction"** %8, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %9, align 8
  store %"class.llvm::BasicBlock"* %3, %"class.llvm::BasicBlock"** %10, align 8
  %14 = load %"class.llvm::IRBuilderDefaultInserter"*, %"class.llvm::IRBuilderDefaultInserter"** %7, align 8
  %15 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %10, align 8
  %16 = icmp ne %"class.llvm::BasicBlock"* %15, null
  br i1 %16, label %17, label %28

; <label>:17:                                     ; preds = %5
  %18 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %10, align 8
  %19 = call dereferenceable(16) %"class.llvm::SymbolTableList"* @_ZN4llvm10BasicBlock11getInstListEv(%"class.llvm::BasicBlock"* %18)
  %20 = bitcast %"class.llvm::SymbolTableList"* %19 to %"class.llvm::iplist_impl"*
  %21 = bitcast %"class.llvm::ilist_iterator"* %11 to i8*
  %22 = bitcast %"class.llvm::ilist_iterator"* %6 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %21, i8* %22, i64 8, i32 8, i1 false)
  %23 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %8, align 8
  %24 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %11, i32 0, i32 0
  %25 = load %"class.llvm::ilist_node_impl.26"*, %"class.llvm::ilist_node_impl.26"** %24, align 8
  %26 = call %"class.llvm::ilist_node_impl.26"* @_ZN4llvm11iplist_implINS_12simple_ilistINS_11InstructionEJEEENS_21SymbolTableListTraitsIS2_EEE6insertENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS2_Lb0ELb0EvEELb0ELb0EEEPS2_(%"class.llvm::iplist_impl"* %20, %"class.llvm::ilist_node_impl.26"* %25, %"class.llvm::Instruction"* %23)
  %27 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %12, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.26"* %26, %"class.llvm::ilist_node_impl.26"** %27, align 8
  br label %28

; <label>:28:                                     ; preds = %17, %5
  %29 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %8, align 8
  %30 = bitcast %"class.llvm::Instruction"* %29 to %"class.llvm::Value"*
  %31 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %9, align 8
  call void @_ZN4llvm5Value7setNameERKNS_5TwineE(%"class.llvm::Value"* %30, %"class.llvm::Twine"* dereferenceable(24) %31)
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #5

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNK4llvm13IRBuilderBase20SetInstDebugLocationEPNS_11InstructionE(%"class.llvm::IRBuilderBase"*, %"class.llvm::Instruction"*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.llvm::IRBuilderBase"*, align 8
  %4 = alloca %"class.llvm::Instruction"*, align 8
  %5 = alloca %"class.llvm::DebugLoc", align 8
  %6 = alloca i8*
  %7 = alloca i32
  store %"class.llvm::IRBuilderBase"* %0, %"class.llvm::IRBuilderBase"** %3, align 8
  store %"class.llvm::Instruction"* %1, %"class.llvm::Instruction"** %4, align 8
  %8 = load %"class.llvm::IRBuilderBase"*, %"class.llvm::IRBuilderBase"** %3, align 8
  %9 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %8, i32 0, i32 0
  %10 = call zeroext i1 @_ZNK4llvm8DebugLoccvbEv(%"class.llvm::DebugLoc"* %9)
  br i1 %10, label %11, label %19

; <label>:11:                                     ; preds = %2
  %12 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %4, align 8
  %13 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %8, i32 0, i32 0
  call void @_ZN4llvm8DebugLocC1ERKS0_(%"class.llvm::DebugLoc"* %5, %"class.llvm::DebugLoc"* dereferenceable(8) %13)
  invoke void @_ZN4llvm11Instruction11setDebugLocENS_8DebugLocE(%"class.llvm::Instruction"* %12, %"class.llvm::DebugLoc"* %5)
          to label %14 unwind label %15

; <label>:14:                                     ; preds = %11
  call void @_ZN4llvm8DebugLocD1Ev(%"class.llvm::DebugLoc"* %5) #9
  br label %19

; <label>:15:                                     ; preds = %11
  %16 = landingpad { i8*, i32 }
          cleanup
  %17 = extractvalue { i8*, i32 } %16, 0
  store i8* %17, i8** %6, align 8
  %18 = extractvalue { i8*, i32 } %16, 1
  store i32 %18, i32* %7, align 4
  call void @_ZN4llvm8DebugLocD1Ev(%"class.llvm::DebugLoc"* %5) #9
  br label %20

; <label>:19:                                     ; preds = %14, %2
  ret void

; <label>:20:                                     ; preds = %15
  %21 = load i8*, i8** %6, align 8
  %22 = load i32, i32* %7, align 4
  %23 = insertvalue { i8*, i32 } undef, i8* %21, 0
  %24 = insertvalue { i8*, i32 } %23, i32 %22, 1
  resume { i8*, i32 } %24
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr dereferenceable(16) %"class.llvm::SymbolTableList"* @_ZN4llvm10BasicBlock11getInstListEv(%"class.llvm::BasicBlock"*) #2 align 2 {
  %2 = alloca %"class.llvm::BasicBlock"*, align 8
  store %"class.llvm::BasicBlock"* %0, %"class.llvm::BasicBlock"** %2, align 8
  %3 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::BasicBlock", %"class.llvm::BasicBlock"* %3, i32 0, i32 2
  ret %"class.llvm::SymbolTableList"* %4
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.26"* @_ZN4llvm11iplist_implINS_12simple_ilistINS_11InstructionEJEEENS_21SymbolTableListTraitsIS2_EEE6insertENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS2_Lb0ELb0EvEELb0ELb0EEEPS2_(%"class.llvm::iplist_impl"*, %"class.llvm::ilist_node_impl.26"*, %"class.llvm::Instruction"*) #0 align 2 {
  %4 = alloca %"class.llvm::ilist_iterator", align 8
  %5 = alloca %"class.llvm::ilist_iterator", align 8
  %6 = alloca %"class.llvm::iplist_impl"*, align 8
  %7 = alloca %"class.llvm::Instruction"*, align 8
  %8 = alloca %"class.llvm::ilist_iterator", align 8
  %9 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %5, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.26"* %1, %"class.llvm::ilist_node_impl.26"** %9, align 8
  store %"class.llvm::iplist_impl"* %0, %"class.llvm::iplist_impl"** %6, align 8
  store %"class.llvm::Instruction"* %2, %"class.llvm::Instruction"** %7, align 8
  %10 = load %"class.llvm::iplist_impl"*, %"class.llvm::iplist_impl"** %6, align 8
  %11 = bitcast %"class.llvm::iplist_impl"* %10 to %"class.llvm::SymbolTableListTraits"*
  %12 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %7, align 8
  call void @_ZN4llvm21SymbolTableListTraitsINS_11InstructionEE13addNodeToListEPS1_(%"class.llvm::SymbolTableListTraits"* %11, %"class.llvm::Instruction"* %12)
  %13 = bitcast %"class.llvm::iplist_impl"* %10 to %"class.llvm::simple_ilist"*
  %14 = bitcast %"class.llvm::ilist_iterator"* %8 to i8*
  %15 = bitcast %"class.llvm::ilist_iterator"* %5 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %14, i8* %15, i64 8, i32 8, i1 false)
  %16 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %7, align 8
  %17 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %8, i32 0, i32 0
  %18 = load %"class.llvm::ilist_node_impl.26"*, %"class.llvm::ilist_node_impl.26"** %17, align 8
  %19 = call %"class.llvm::ilist_node_impl.26"* @_ZN4llvm12simple_ilistINS_11InstructionEJEE6insertENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEERS1_(%"class.llvm::simple_ilist"* %13, %"class.llvm::ilist_node_impl.26"* %18, %"class.llvm::Instruction"* dereferenceable(64) %16)
  %20 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %4, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.26"* %19, %"class.llvm::ilist_node_impl.26"** %20, align 8
  %21 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %4, i32 0, i32 0
  %22 = load %"class.llvm::ilist_node_impl.26"*, %"class.llvm::ilist_node_impl.26"** %21, align 8
  ret %"class.llvm::ilist_node_impl.26"* %22
}

declare void @_ZN4llvm5Value7setNameERKNS_5TwineE(%"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #4

declare void @_ZN4llvm21SymbolTableListTraitsINS_11InstructionEE13addNodeToListEPS1_(%"class.llvm::SymbolTableListTraits"*, %"class.llvm::Instruction"*) #4

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.26"* @_ZN4llvm12simple_ilistINS_11InstructionEJEE6insertENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEERS1_(%"class.llvm::simple_ilist"*, %"class.llvm::ilist_node_impl.26"*, %"class.llvm::Instruction"* dereferenceable(64)) #0 align 2 {
  %4 = alloca %"class.llvm::ilist_iterator", align 8
  %5 = alloca %"class.llvm::ilist_iterator", align 8
  %6 = alloca %"class.llvm::simple_ilist"*, align 8
  %7 = alloca %"class.llvm::Instruction"*, align 8
  %8 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %5, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.26"* %1, %"class.llvm::ilist_node_impl.26"** %8, align 8
  store %"class.llvm::simple_ilist"* %0, %"class.llvm::simple_ilist"** %6, align 8
  store %"class.llvm::Instruction"* %2, %"class.llvm::Instruction"** %7, align 8
  %9 = load %"class.llvm::simple_ilist"*, %"class.llvm::simple_ilist"** %6, align 8
  %10 = call %"class.llvm::ilist_node_impl.26"* @_ZNK4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EE10getNodePtrEv(%"class.llvm::ilist_iterator"* %5)
  %11 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %7, align 8
  %12 = call %"class.llvm::ilist_node_impl.26"* @_ZN4llvm12ilist_detail18SpecificNodeAccessINS0_12node_optionsINS_11InstructionELb0ELb0EvEEE10getNodePtrEPS3_(%"class.llvm::Instruction"* %11)
  call void @_ZN4llvm10ilist_baseILb0EE12insertBeforeINS_15ilist_node_implINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEEEEEEvRT_SA_(%"class.llvm::ilist_node_impl.26"* dereferenceable(16) %10, %"class.llvm::ilist_node_impl.26"* dereferenceable(16) %12)
  %13 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %7, align 8
  call void @_ZN4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EEC1EPS3_(%"class.llvm::ilist_iterator"* %4, %"class.llvm::Instruction"* %13)
  %14 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %4, i32 0, i32 0
  %15 = load %"class.llvm::ilist_node_impl.26"*, %"class.llvm::ilist_node_impl.26"** %14, align 8
  ret %"class.llvm::ilist_node_impl.26"* %15
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm10ilist_baseILb0EE12insertBeforeINS_15ilist_node_implINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEEEEEEvRT_SA_(%"class.llvm::ilist_node_impl.26"* dereferenceable(16), %"class.llvm::ilist_node_impl.26"* dereferenceable(16)) #0 align 2 {
  %3 = alloca %"class.llvm::ilist_node_impl.26"*, align 8
  %4 = alloca %"class.llvm::ilist_node_impl.26"*, align 8
  store %"class.llvm::ilist_node_impl.26"* %0, %"class.llvm::ilist_node_impl.26"** %3, align 8
  store %"class.llvm::ilist_node_impl.26"* %1, %"class.llvm::ilist_node_impl.26"** %4, align 8
  %5 = load %"class.llvm::ilist_node_impl.26"*, %"class.llvm::ilist_node_impl.26"** %3, align 8
  %6 = bitcast %"class.llvm::ilist_node_impl.26"* %5 to %"class.llvm::ilist_node_base"*
  %7 = load %"class.llvm::ilist_node_impl.26"*, %"class.llvm::ilist_node_impl.26"** %4, align 8
  %8 = bitcast %"class.llvm::ilist_node_impl.26"* %7 to %"class.llvm::ilist_node_base"*
  call void @_ZN4llvm10ilist_baseILb0EE16insertBeforeImplERNS_15ilist_node_baseILb0EEES4_(%"class.llvm::ilist_node_base"* dereferenceable(16) %6, %"class.llvm::ilist_node_base"* dereferenceable(16) %8)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.26"* @_ZNK4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EE10getNodePtrEv(%"class.llvm::ilist_iterator"*) #2 align 2 {
  %2 = alloca %"class.llvm::ilist_iterator"*, align 8
  store %"class.llvm::ilist_iterator"* %0, %"class.llvm::ilist_iterator"** %2, align 8
  %3 = load %"class.llvm::ilist_iterator"*, %"class.llvm::ilist_iterator"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %3, i32 0, i32 0
  %5 = load %"class.llvm::ilist_node_impl.26"*, %"class.llvm::ilist_node_impl.26"** %4, align 8
  ret %"class.llvm::ilist_node_impl.26"* %5
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.26"* @_ZN4llvm12ilist_detail18SpecificNodeAccessINS0_12node_optionsINS_11InstructionELb0ELb0EvEEE10getNodePtrEPS3_(%"class.llvm::Instruction"*) #0 align 2 {
  %2 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"* %0, %"class.llvm::Instruction"** %2, align 8
  %3 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %2, align 8
  %4 = call %"class.llvm::ilist_node_impl.26"* @_ZN4llvm12ilist_detail10NodeAccess10getNodePtrINS0_12node_optionsINS_11InstructionELb0ELb0EvEEEEPNS_15ilist_node_implIT_EENS7_7pointerE(%"class.llvm::Instruction"* %3)
  ret %"class.llvm::ilist_node_impl.26"* %4
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EEC1EPS3_(%"class.llvm::ilist_iterator"*, %"class.llvm::Instruction"*) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::ilist_iterator"*, align 8
  %4 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::ilist_iterator"* %0, %"class.llvm::ilist_iterator"** %3, align 8
  store %"class.llvm::Instruction"* %1, %"class.llvm::Instruction"** %4, align 8
  %5 = load %"class.llvm::ilist_iterator"*, %"class.llvm::ilist_iterator"** %3, align 8
  %6 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %4, align 8
  call void @_ZN4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EEC2EPS3_(%"class.llvm::ilist_iterator"* %5, %"class.llvm::Instruction"* %6)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm10ilist_baseILb0EE16insertBeforeImplERNS_15ilist_node_baseILb0EEES4_(%"class.llvm::ilist_node_base"* dereferenceable(16), %"class.llvm::ilist_node_base"* dereferenceable(16)) #0 align 2 {
  %3 = alloca %"class.llvm::ilist_node_base"*, align 8
  %4 = alloca %"class.llvm::ilist_node_base"*, align 8
  %5 = alloca %"class.llvm::ilist_node_base"*, align 8
  store %"class.llvm::ilist_node_base"* %0, %"class.llvm::ilist_node_base"** %3, align 8
  store %"class.llvm::ilist_node_base"* %1, %"class.llvm::ilist_node_base"** %4, align 8
  %6 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %3, align 8
  %7 = call %"class.llvm::ilist_node_base"* @_ZNK4llvm15ilist_node_baseILb0EE7getPrevEv(%"class.llvm::ilist_node_base"* %6)
  store %"class.llvm::ilist_node_base"* %7, %"class.llvm::ilist_node_base"** %5, align 8
  %8 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %4, align 8
  %9 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %3, align 8
  call void @_ZN4llvm15ilist_node_baseILb0EE7setNextEPS1_(%"class.llvm::ilist_node_base"* %8, %"class.llvm::ilist_node_base"* %9)
  %10 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %4, align 8
  %11 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %5, align 8
  call void @_ZN4llvm15ilist_node_baseILb0EE7setPrevEPS1_(%"class.llvm::ilist_node_base"* %10, %"class.llvm::ilist_node_base"* %11)
  %12 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %5, align 8
  %13 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %4, align 8
  call void @_ZN4llvm15ilist_node_baseILb0EE7setNextEPS1_(%"class.llvm::ilist_node_base"* %12, %"class.llvm::ilist_node_base"* %13)
  %14 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %3, align 8
  %15 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %4, align 8
  call void @_ZN4llvm15ilist_node_baseILb0EE7setPrevEPS1_(%"class.llvm::ilist_node_base"* %14, %"class.llvm::ilist_node_base"* %15)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_base"* @_ZNK4llvm15ilist_node_baseILb0EE7getPrevEv(%"class.llvm::ilist_node_base"*) #2 align 2 {
  %2 = alloca %"class.llvm::ilist_node_base"*, align 8
  store %"class.llvm::ilist_node_base"* %0, %"class.llvm::ilist_node_base"** %2, align 8
  %3 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::ilist_node_base", %"class.llvm::ilist_node_base"* %3, i32 0, i32 0
  %5 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %4, align 8
  ret %"class.llvm::ilist_node_base"* %5
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm15ilist_node_baseILb0EE7setNextEPS1_(%"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"*) #2 align 2 {
  %3 = alloca %"class.llvm::ilist_node_base"*, align 8
  %4 = alloca %"class.llvm::ilist_node_base"*, align 8
  store %"class.llvm::ilist_node_base"* %0, %"class.llvm::ilist_node_base"** %3, align 8
  store %"class.llvm::ilist_node_base"* %1, %"class.llvm::ilist_node_base"** %4, align 8
  %5 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %3, align 8
  %6 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %4, align 8
  %7 = getelementptr inbounds %"class.llvm::ilist_node_base", %"class.llvm::ilist_node_base"* %5, i32 0, i32 1
  store %"class.llvm::ilist_node_base"* %6, %"class.llvm::ilist_node_base"** %7, align 8
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm15ilist_node_baseILb0EE7setPrevEPS1_(%"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"*) #2 align 2 {
  %3 = alloca %"class.llvm::ilist_node_base"*, align 8
  %4 = alloca %"class.llvm::ilist_node_base"*, align 8
  store %"class.llvm::ilist_node_base"* %0, %"class.llvm::ilist_node_base"** %3, align 8
  store %"class.llvm::ilist_node_base"* %1, %"class.llvm::ilist_node_base"** %4, align 8
  %5 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %3, align 8
  %6 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %4, align 8
  %7 = getelementptr inbounds %"class.llvm::ilist_node_base", %"class.llvm::ilist_node_base"* %5, i32 0, i32 0
  store %"class.llvm::ilist_node_base"* %6, %"class.llvm::ilist_node_base"** %7, align 8
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.26"* @_ZN4llvm12ilist_detail10NodeAccess10getNodePtrINS0_12node_optionsINS_11InstructionELb0ELb0EvEEEEPNS_15ilist_node_implIT_EENS7_7pointerE(%"class.llvm::Instruction"*) #2 align 2 {
  %2 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"* %0, %"class.llvm::Instruction"** %2, align 8
  %3 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %2, align 8
  %4 = icmp eq %"class.llvm::Instruction"* %3, null
  br i1 %4, label %9, label %5

; <label>:5:                                      ; preds = %1
  %6 = bitcast %"class.llvm::Instruction"* %3 to i8*
  %7 = getelementptr inbounds i8, i8* %6, i64 32
  %8 = bitcast i8* %7 to %"class.llvm::ilist_node_impl.26"*
  br label %9

; <label>:9:                                      ; preds = %5, %1
  %10 = phi %"class.llvm::ilist_node_impl.26"* [ %8, %5 ], [ null, %1 ]
  ret %"class.llvm::ilist_node_impl.26"* %10
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EEC2EPS3_(%"class.llvm::ilist_iterator"*, %"class.llvm::Instruction"*) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::ilist_iterator"*, align 8
  %4 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::ilist_iterator"* %0, %"class.llvm::ilist_iterator"** %3, align 8
  store %"class.llvm::Instruction"* %1, %"class.llvm::Instruction"** %4, align 8
  %5 = load %"class.llvm::ilist_iterator"*, %"class.llvm::ilist_iterator"** %3, align 8
  %6 = bitcast %"class.llvm::ilist_iterator"* %5 to %"struct.llvm::ilist_detail::SpecificNodeAccess"*
  %7 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %5, i32 0, i32 0
  %8 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %4, align 8
  %9 = call %"class.llvm::ilist_node_impl.26"* @_ZN4llvm12ilist_detail18SpecificNodeAccessINS0_12node_optionsINS_11InstructionELb0ELb0EvEEE10getNodePtrEPS3_(%"class.llvm::Instruction"* %8)
  store %"class.llvm::ilist_node_impl.26"* %9, %"class.llvm::ilist_node_impl.26"** %7, align 8
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZNK4llvm8DebugLoccvbEv(%"class.llvm::DebugLoc"*) #0 align 2 {
  %2 = alloca %"class.llvm::DebugLoc"*, align 8
  store %"class.llvm::DebugLoc"* %0, %"class.llvm::DebugLoc"** %2, align 8
  %3 = load %"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::DebugLoc", %"class.llvm::DebugLoc"* %3, i32 0, i32 0
  %5 = call %"class.llvm::MDNode"* @_ZNK4llvm18TypedTrackingMDRefINS_6MDNodeEEcvPS1_Ev(%"class.llvm::TypedTrackingMDRef"* %4)
  %6 = icmp ne %"class.llvm::MDNode"* %5, null
  ret i1 %6
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm11Instruction11setDebugLocENS_8DebugLocE(%"class.llvm::Instruction"*, %"class.llvm::DebugLoc"*) #0 align 2 {
  %3 = alloca %"class.llvm::DebugLoc"*, align 8
  %4 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"* %0, %"class.llvm::Instruction"** %4, align 8
  %5 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %4, align 8
  %6 = getelementptr inbounds %"class.llvm::Instruction", %"class.llvm::Instruction"* %5, i32 0, i32 3
  store %"class.llvm::DebugLoc"* %1, %"class.llvm::DebugLoc"** %3, align 8
  %7 = load %"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"** %3, align 8
  %8 = call dereferenceable(8) %"class.llvm::DebugLoc"* @_ZN4llvm8DebugLocaSEOS0_(%"class.llvm::DebugLoc"* %6, %"class.llvm::DebugLoc"* dereferenceable(8) %7)
  ret void
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZN4llvm8DebugLocC1ERKS0_(%"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"* dereferenceable(8)) unnamed_addr #3 align 2 {
  %3 = alloca %"class.llvm::DebugLoc"*, align 8
  %4 = alloca %"class.llvm::DebugLoc"*, align 8
  store %"class.llvm::DebugLoc"* %0, %"class.llvm::DebugLoc"** %3, align 8
  store %"class.llvm::DebugLoc"* %1, %"class.llvm::DebugLoc"** %4, align 8
  %5 = load %"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"** %3, align 8
  %6 = load %"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"** %4, align 8
  call void @_ZN4llvm8DebugLocC2ERKS0_(%"class.llvm::DebugLoc"* %5, %"class.llvm::DebugLoc"* dereferenceable(8) %6)
  ret void
}

declare i32 @__gxx_personality_v0(...)

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm8DebugLocD1Ev(%"class.llvm::DebugLoc"*) unnamed_addr #6 align 2 {
  %2 = alloca %"class.llvm::DebugLoc"*, align 8
  store %"class.llvm::DebugLoc"* %0, %"class.llvm::DebugLoc"** %2, align 8
  %3 = load %"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"** %2, align 8
  call void @_ZN4llvm8DebugLocD2Ev(%"class.llvm::DebugLoc"* %3) #9
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::MDNode"* @_ZNK4llvm18TypedTrackingMDRefINS_6MDNodeEEcvPS1_Ev(%"class.llvm::TypedTrackingMDRef"*) #0 align 2 {
  %2 = alloca %"class.llvm::TypedTrackingMDRef"*, align 8
  store %"class.llvm::TypedTrackingMDRef"* %0, %"class.llvm::TypedTrackingMDRef"** %2, align 8
  %3 = load %"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"** %2, align 8
  %4 = call %"class.llvm::MDNode"* @_ZNK4llvm18TypedTrackingMDRefINS_6MDNodeEE3getEv(%"class.llvm::TypedTrackingMDRef"* %3)
  ret %"class.llvm::MDNode"* %4
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::MDNode"* @_ZNK4llvm18TypedTrackingMDRefINS_6MDNodeEE3getEv(%"class.llvm::TypedTrackingMDRef"*) #0 align 2 {
  %2 = alloca %"class.llvm::TypedTrackingMDRef"*, align 8
  store %"class.llvm::TypedTrackingMDRef"* %0, %"class.llvm::TypedTrackingMDRef"** %2, align 8
  %3 = load %"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::TypedTrackingMDRef", %"class.llvm::TypedTrackingMDRef"* %3, i32 0, i32 0
  %5 = call %"class.llvm::Metadata"* @_ZNK4llvm13TrackingMDRef3getEv(%"class.llvm::TrackingMDRef"* %4)
  %6 = bitcast %"class.llvm::Metadata"* %5 to %"class.llvm::MDNode"*
  ret %"class.llvm::MDNode"* %6
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::Metadata"* @_ZNK4llvm13TrackingMDRef3getEv(%"class.llvm::TrackingMDRef"*) #2 align 2 {
  %2 = alloca %"class.llvm::TrackingMDRef"*, align 8
  store %"class.llvm::TrackingMDRef"* %0, %"class.llvm::TrackingMDRef"** %2, align 8
  %3 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %3, i32 0, i32 0
  %5 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %4, align 8
  ret %"class.llvm::Metadata"* %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr dereferenceable(8) %"class.llvm::DebugLoc"* @_ZN4llvm8DebugLocaSEOS0_(%"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"* dereferenceable(8)) #3 align 2 {
  %3 = alloca %"class.llvm::DebugLoc"*, align 8
  %4 = alloca %"class.llvm::DebugLoc"*, align 8
  store %"class.llvm::DebugLoc"* %0, %"class.llvm::DebugLoc"** %3, align 8
  store %"class.llvm::DebugLoc"* %1, %"class.llvm::DebugLoc"** %4, align 8
  %5 = load %"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"** %3, align 8
  %6 = getelementptr inbounds %"class.llvm::DebugLoc", %"class.llvm::DebugLoc"* %5, i32 0, i32 0
  %7 = load %"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"** %4, align 8
  %8 = getelementptr inbounds %"class.llvm::DebugLoc", %"class.llvm::DebugLoc"* %7, i32 0, i32 0
  %9 = call dereferenceable(8) %"class.llvm::TypedTrackingMDRef"* @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEEaSEOS2_(%"class.llvm::TypedTrackingMDRef"* %6, %"class.llvm::TypedTrackingMDRef"* dereferenceable(8) %8)
  ret %"class.llvm::DebugLoc"* %5
}

; Function Attrs: ssp uwtable
define linkonce_odr dereferenceable(8) %"class.llvm::TypedTrackingMDRef"* @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEEaSEOS2_(%"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"* dereferenceable(8)) #0 align 2 {
  %3 = alloca %"class.llvm::TrackingMDRef"*, align 8
  %4 = alloca %"class.llvm::TypedTrackingMDRef"*, align 8
  %5 = alloca %"class.llvm::TypedTrackingMDRef"*, align 8
  store %"class.llvm::TypedTrackingMDRef"* %0, %"class.llvm::TypedTrackingMDRef"** %4, align 8
  store %"class.llvm::TypedTrackingMDRef"* %1, %"class.llvm::TypedTrackingMDRef"** %5, align 8
  %6 = load %"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"** %4, align 8
  %7 = getelementptr inbounds %"class.llvm::TypedTrackingMDRef", %"class.llvm::TypedTrackingMDRef"* %6, i32 0, i32 0
  %8 = load %"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"** %5, align 8
  %9 = getelementptr inbounds %"class.llvm::TypedTrackingMDRef", %"class.llvm::TypedTrackingMDRef"* %8, i32 0, i32 0
  store %"class.llvm::TrackingMDRef"* %9, %"class.llvm::TrackingMDRef"** %3, align 8
  %10 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %3, align 8
  %11 = call dereferenceable(8) %"class.llvm::TrackingMDRef"* @_ZN4llvm13TrackingMDRefaSEOS0_(%"class.llvm::TrackingMDRef"* %7, %"class.llvm::TrackingMDRef"* dereferenceable(8) %10)
  ret %"class.llvm::TypedTrackingMDRef"* %6
}

; Function Attrs: ssp uwtable
define linkonce_odr dereferenceable(8) %"class.llvm::TrackingMDRef"* @_ZN4llvm13TrackingMDRefaSEOS0_(%"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"* dereferenceable(8)) #0 align 2 {
  %3 = alloca %"class.llvm::TrackingMDRef"*, align 8
  %4 = alloca %"class.llvm::TrackingMDRef"*, align 8
  %5 = alloca %"class.llvm::TrackingMDRef"*, align 8
  store %"class.llvm::TrackingMDRef"* %0, %"class.llvm::TrackingMDRef"** %4, align 8
  store %"class.llvm::TrackingMDRef"* %1, %"class.llvm::TrackingMDRef"** %5, align 8
  %6 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %4, align 8
  %7 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %5, align 8
  %8 = icmp eq %"class.llvm::TrackingMDRef"* %7, %6
  br i1 %8, label %9, label %10

; <label>:9:                                      ; preds = %2
  store %"class.llvm::TrackingMDRef"* %6, %"class.llvm::TrackingMDRef"** %3, align 8
  br label %16

; <label>:10:                                     ; preds = %2
  call void @_ZN4llvm13TrackingMDRef7untrackEv(%"class.llvm::TrackingMDRef"* %6)
  %11 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %5, align 8
  %12 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %11, i32 0, i32 0
  %13 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %12, align 8
  %14 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %6, i32 0, i32 0
  store %"class.llvm::Metadata"* %13, %"class.llvm::Metadata"** %14, align 8
  %15 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %5, align 8
  call void @_ZN4llvm13TrackingMDRef7retrackERS0_(%"class.llvm::TrackingMDRef"* %6, %"class.llvm::TrackingMDRef"* dereferenceable(8) %15)
  store %"class.llvm::TrackingMDRef"* %6, %"class.llvm::TrackingMDRef"** %3, align 8
  br label %16

; <label>:16:                                     ; preds = %10, %9
  %17 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %3, align 8
  ret %"class.llvm::TrackingMDRef"* %17
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm13TrackingMDRef7untrackEv(%"class.llvm::TrackingMDRef"*) #0 align 2 {
  %2 = alloca %"class.llvm::TrackingMDRef"*, align 8
  store %"class.llvm::TrackingMDRef"* %0, %"class.llvm::TrackingMDRef"** %2, align 8
  %3 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %3, i32 0, i32 0
  %5 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %4, align 8
  %6 = icmp ne %"class.llvm::Metadata"* %5, null
  br i1 %6, label %7, label %9

; <label>:7:                                      ; preds = %1
  %8 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %3, i32 0, i32 0
  call void @_ZN4llvm16MetadataTracking7untrackERPNS_8MetadataE(%"class.llvm::Metadata"** dereferenceable(8) %8)
  br label %9

; <label>:9:                                      ; preds = %7, %1
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm13TrackingMDRef7retrackERS0_(%"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"* dereferenceable(8)) #0 align 2 {
  %3 = alloca %"class.llvm::TrackingMDRef"*, align 8
  %4 = alloca %"class.llvm::TrackingMDRef"*, align 8
  store %"class.llvm::TrackingMDRef"* %0, %"class.llvm::TrackingMDRef"** %3, align 8
  store %"class.llvm::TrackingMDRef"* %1, %"class.llvm::TrackingMDRef"** %4, align 8
  %5 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %3, align 8
  %6 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %5, i32 0, i32 0
  %7 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %6, align 8
  %8 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %4, align 8
  %9 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %8, i32 0, i32 0
  %10 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %9, align 8
  %11 = icmp eq %"class.llvm::Metadata"* %7, %10
  br i1 %11, label %12, label %13

; <label>:12:                                     ; preds = %2
  br label %13

; <label>:13:                                     ; preds = %12, %2
  %14 = phi i1 [ false, %2 ], [ true, %12 ]
  %15 = xor i1 %14, true
  br i1 %15, label %16, label %18

; <label>:16:                                     ; preds = %13
  call void @__assert_rtn(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @__func__._ZN4llvm13TrackingMDRef7retrackERS0_, i32 0, i32 0), i8* getelementptr inbounds ([63 x i8], [63 x i8]* @.str.10, i32 0, i32 0), i32 88, i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str.11, i32 0, i32 0)) #8
  unreachable
                                                  ; No predecessors!
  br label %19

; <label>:18:                                     ; preds = %13
  br label %19

; <label>:19:                                     ; preds = %18, %17
  %20 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %4, align 8
  %21 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %20, i32 0, i32 0
  %22 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %21, align 8
  %23 = icmp ne %"class.llvm::Metadata"* %22, null
  br i1 %23, label %24, label %31

; <label>:24:                                     ; preds = %19
  %25 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %4, align 8
  %26 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %25, i32 0, i32 0
  %27 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %5, i32 0, i32 0
  %28 = call zeroext i1 @_ZN4llvm16MetadataTracking7retrackERPNS_8MetadataES3_(%"class.llvm::Metadata"** dereferenceable(8) %26, %"class.llvm::Metadata"** dereferenceable(8) %27)
  %29 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %4, align 8
  %30 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %29, i32 0, i32 0
  store %"class.llvm::Metadata"* null, %"class.llvm::Metadata"** %30, align 8
  br label %31

; <label>:31:                                     ; preds = %24, %19
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm16MetadataTracking7untrackERPNS_8MetadataE(%"class.llvm::Metadata"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Metadata"**, align 8
  store %"class.llvm::Metadata"** %0, %"class.llvm::Metadata"*** %2, align 8
  %3 = load %"class.llvm::Metadata"**, %"class.llvm::Metadata"*** %2, align 8
  %4 = bitcast %"class.llvm::Metadata"** %3 to i8*
  %5 = load %"class.llvm::Metadata"**, %"class.llvm::Metadata"*** %2, align 8
  %6 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %5, align 8
  call void @_ZN4llvm16MetadataTracking7untrackEPvRNS_8MetadataE(i8* %4, %"class.llvm::Metadata"* dereferenceable(8) %6)
  ret void
}

declare void @_ZN4llvm16MetadataTracking7untrackEPvRNS_8MetadataE(i8*, %"class.llvm::Metadata"* dereferenceable(8)) #4

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm16MetadataTracking7retrackERPNS_8MetadataES3_(%"class.llvm::Metadata"** dereferenceable(8), %"class.llvm::Metadata"** dereferenceable(8)) #0 align 2 {
  %3 = alloca %"class.llvm::Metadata"**, align 8
  %4 = alloca %"class.llvm::Metadata"**, align 8
  store %"class.llvm::Metadata"** %0, %"class.llvm::Metadata"*** %3, align 8
  store %"class.llvm::Metadata"** %1, %"class.llvm::Metadata"*** %4, align 8
  %5 = load %"class.llvm::Metadata"**, %"class.llvm::Metadata"*** %3, align 8
  %6 = bitcast %"class.llvm::Metadata"** %5 to i8*
  %7 = load %"class.llvm::Metadata"**, %"class.llvm::Metadata"*** %3, align 8
  %8 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %7, align 8
  %9 = load %"class.llvm::Metadata"**, %"class.llvm::Metadata"*** %4, align 8
  %10 = bitcast %"class.llvm::Metadata"** %9 to i8*
  %11 = call zeroext i1 @_ZN4llvm16MetadataTracking7retrackEPvRNS_8MetadataES1_(i8* %6, %"class.llvm::Metadata"* dereferenceable(8) %8, i8* %10)
  ret i1 %11
}

declare zeroext i1 @_ZN4llvm16MetadataTracking7retrackEPvRNS_8MetadataES1_(i8*, %"class.llvm::Metadata"* dereferenceable(8), i8*) #4

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZN4llvm8DebugLocC2ERKS0_(%"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"* dereferenceable(8)) unnamed_addr #3 align 2 {
  %3 = alloca %"class.llvm::DebugLoc"*, align 8
  %4 = alloca %"class.llvm::DebugLoc"*, align 8
  store %"class.llvm::DebugLoc"* %0, %"class.llvm::DebugLoc"** %3, align 8
  store %"class.llvm::DebugLoc"* %1, %"class.llvm::DebugLoc"** %4, align 8
  %5 = load %"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"** %3, align 8
  %6 = getelementptr inbounds %"class.llvm::DebugLoc", %"class.llvm::DebugLoc"* %5, i32 0, i32 0
  %7 = load %"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"** %4, align 8
  %8 = getelementptr inbounds %"class.llvm::DebugLoc", %"class.llvm::DebugLoc"* %7, i32 0, i32 0
  call void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEEC1ERKS2_(%"class.llvm::TypedTrackingMDRef"* %6, %"class.llvm::TypedTrackingMDRef"* dereferenceable(8) %8)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEEC1ERKS2_(%"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"* dereferenceable(8)) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::TypedTrackingMDRef"*, align 8
  %4 = alloca %"class.llvm::TypedTrackingMDRef"*, align 8
  store %"class.llvm::TypedTrackingMDRef"* %0, %"class.llvm::TypedTrackingMDRef"** %3, align 8
  store %"class.llvm::TypedTrackingMDRef"* %1, %"class.llvm::TypedTrackingMDRef"** %4, align 8
  %5 = load %"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"** %3, align 8
  %6 = load %"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"** %4, align 8
  call void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEEC2ERKS2_(%"class.llvm::TypedTrackingMDRef"* %5, %"class.llvm::TypedTrackingMDRef"* dereferenceable(8) %6)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEEC2ERKS2_(%"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"* dereferenceable(8)) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::TypedTrackingMDRef"*, align 8
  %4 = alloca %"class.llvm::TypedTrackingMDRef"*, align 8
  store %"class.llvm::TypedTrackingMDRef"* %0, %"class.llvm::TypedTrackingMDRef"** %3, align 8
  store %"class.llvm::TypedTrackingMDRef"* %1, %"class.llvm::TypedTrackingMDRef"** %4, align 8
  %5 = load %"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"** %3, align 8
  %6 = getelementptr inbounds %"class.llvm::TypedTrackingMDRef", %"class.llvm::TypedTrackingMDRef"* %5, i32 0, i32 0
  %7 = load %"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"** %4, align 8
  %8 = getelementptr inbounds %"class.llvm::TypedTrackingMDRef", %"class.llvm::TypedTrackingMDRef"* %7, i32 0, i32 0
  call void @_ZN4llvm13TrackingMDRefC1ERKS0_(%"class.llvm::TrackingMDRef"* %6, %"class.llvm::TrackingMDRef"* dereferenceable(8) %8)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm13TrackingMDRefC1ERKS0_(%"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"* dereferenceable(8)) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::TrackingMDRef"*, align 8
  %4 = alloca %"class.llvm::TrackingMDRef"*, align 8
  store %"class.llvm::TrackingMDRef"* %0, %"class.llvm::TrackingMDRef"** %3, align 8
  store %"class.llvm::TrackingMDRef"* %1, %"class.llvm::TrackingMDRef"** %4, align 8
  %5 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %3, align 8
  %6 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %4, align 8
  call void @_ZN4llvm13TrackingMDRefC2ERKS0_(%"class.llvm::TrackingMDRef"* %5, %"class.llvm::TrackingMDRef"* dereferenceable(8) %6)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm13TrackingMDRefC2ERKS0_(%"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"* dereferenceable(8)) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::TrackingMDRef"*, align 8
  %4 = alloca %"class.llvm::TrackingMDRef"*, align 8
  store %"class.llvm::TrackingMDRef"* %0, %"class.llvm::TrackingMDRef"** %3, align 8
  store %"class.llvm::TrackingMDRef"* %1, %"class.llvm::TrackingMDRef"** %4, align 8
  %5 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %3, align 8
  %6 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %5, i32 0, i32 0
  %7 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %4, align 8
  %8 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %7, i32 0, i32 0
  %9 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %8, align 8
  store %"class.llvm::Metadata"* %9, %"class.llvm::Metadata"** %6, align 8
  call void @_ZN4llvm13TrackingMDRef5trackEv(%"class.llvm::TrackingMDRef"* %5)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm13TrackingMDRef5trackEv(%"class.llvm::TrackingMDRef"*) #0 align 2 {
  %2 = alloca %"class.llvm::TrackingMDRef"*, align 8
  store %"class.llvm::TrackingMDRef"* %0, %"class.llvm::TrackingMDRef"** %2, align 8
  %3 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %3, i32 0, i32 0
  %5 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %4, align 8
  %6 = icmp ne %"class.llvm::Metadata"* %5, null
  br i1 %6, label %7, label %10

; <label>:7:                                      ; preds = %1
  %8 = getelementptr inbounds %"class.llvm::TrackingMDRef", %"class.llvm::TrackingMDRef"* %3, i32 0, i32 0
  %9 = call zeroext i1 @_ZN4llvm16MetadataTracking5trackERPNS_8MetadataE(%"class.llvm::Metadata"** dereferenceable(8) %8)
  br label %10

; <label>:10:                                     ; preds = %7, %1
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm16MetadataTracking5trackERPNS_8MetadataE(%"class.llvm::Metadata"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Metadata"**, align 8
  %3 = alloca %"class.llvm::PointerUnion.56", align 8
  store %"class.llvm::Metadata"** %0, %"class.llvm::Metadata"*** %2, align 8
  %4 = load %"class.llvm::Metadata"**, %"class.llvm::Metadata"*** %2, align 8
  %5 = bitcast %"class.llvm::Metadata"** %4 to i8*
  %6 = load %"class.llvm::Metadata"**, %"class.llvm::Metadata"*** %2, align 8
  %7 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %6, align 8
  call void @_ZN4llvm12PointerUnionIPNS_15MetadataAsValueEPNS_8MetadataEEC1ES4_(%"class.llvm::PointerUnion.56"* %3, %"class.llvm::Metadata"* null)
  %8 = getelementptr inbounds %"class.llvm::PointerUnion.56", %"class.llvm::PointerUnion.56"* %3, i32 0, i32 0
  %9 = getelementptr inbounds %"class.llvm::PointerIntPair.57", %"class.llvm::PointerIntPair.57"* %8, i32 0, i32 0
  %10 = load i64, i64* %9, align 8
  %11 = call zeroext i1 @_ZN4llvm16MetadataTracking5trackEPvRNS_8MetadataENS_12PointerUnionIPNS_15MetadataAsValueEPS2_EE(i8* %5, %"class.llvm::Metadata"* dereferenceable(8) %7, i64 %10)
  ret i1 %11
}

declare zeroext i1 @_ZN4llvm16MetadataTracking5trackEPvRNS_8MetadataENS_12PointerUnionIPNS_15MetadataAsValueEPS2_EE(i8*, %"class.llvm::Metadata"* dereferenceable(8), i64) #4

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm12PointerUnionIPNS_15MetadataAsValueEPNS_8MetadataEEC1ES4_(%"class.llvm::PointerUnion.56"*, %"class.llvm::Metadata"*) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::PointerUnion.56"*, align 8
  %4 = alloca %"class.llvm::Metadata"*, align 8
  store %"class.llvm::PointerUnion.56"* %0, %"class.llvm::PointerUnion.56"** %3, align 8
  store %"class.llvm::Metadata"* %1, %"class.llvm::Metadata"** %4, align 8
  %5 = load %"class.llvm::PointerUnion.56"*, %"class.llvm::PointerUnion.56"** %3, align 8
  %6 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %4, align 8
  call void @_ZN4llvm12PointerUnionIPNS_15MetadataAsValueEPNS_8MetadataEEC2ES4_(%"class.llvm::PointerUnion.56"* %5, %"class.llvm::Metadata"* %6)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm12PointerUnionIPNS_15MetadataAsValueEPNS_8MetadataEEC2ES4_(%"class.llvm::PointerUnion.56"*, %"class.llvm::Metadata"*) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::PointerUnion.56"*, align 8
  %4 = alloca %"class.llvm::Metadata"*, align 8
  store %"class.llvm::PointerUnion.56"* %0, %"class.llvm::PointerUnion.56"** %3, align 8
  store %"class.llvm::Metadata"* %1, %"class.llvm::Metadata"** %4, align 8
  %5 = load %"class.llvm::PointerUnion.56"*, %"class.llvm::PointerUnion.56"** %3, align 8
  %6 = getelementptr inbounds %"class.llvm::PointerUnion.56", %"class.llvm::PointerUnion.56"* %5, i32 0, i32 0
  %7 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %4, align 8
  %8 = call i8* @_ZN4llvm21PointerLikeTypeTraitsIPNS_8MetadataEE16getAsVoidPointerES2_(%"class.llvm::Metadata"* %7)
  call void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEEC1ES1_b(%"class.llvm::PointerIntPair.57"* %6, i8* %8, i1 zeroext true)
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr i8* @_ZN4llvm21PointerLikeTypeTraitsIPNS_8MetadataEE16getAsVoidPointerES2_(%"class.llvm::Metadata"*) #6 align 2 {
  %2 = alloca %"class.llvm::Metadata"*, align 8
  store %"class.llvm::Metadata"* %0, %"class.llvm::Metadata"** %2, align 8
  %3 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %2, align 8
  %4 = bitcast %"class.llvm::Metadata"* %3 to i8*
  ret i8* %4
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEEC1ES1_b(%"class.llvm::PointerIntPair.57"*, i8*, i1 zeroext) unnamed_addr #0 align 2 {
  %4 = alloca %"class.llvm::PointerIntPair.57"*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i8, align 1
  store %"class.llvm::PointerIntPair.57"* %0, %"class.llvm::PointerIntPair.57"** %4, align 8
  store i8* %1, i8** %5, align 8
  %7 = zext i1 %2 to i8
  store i8 %7, i8* %6, align 1
  %8 = load %"class.llvm::PointerIntPair.57"*, %"class.llvm::PointerIntPair.57"** %4, align 8
  %9 = load i8*, i8** %5, align 8
  %10 = load i8, i8* %6, align 1
  %11 = trunc i8 %10 to i1
  call void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEEC2ES1_b(%"class.llvm::PointerIntPair.57"* %8, i8* %9, i1 zeroext %11)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEEC2ES1_b(%"class.llvm::PointerIntPair.57"*, i8*, i1 zeroext) unnamed_addr #0 align 2 {
  %4 = alloca %"class.llvm::PointerIntPair.57"*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i8, align 1
  store %"class.llvm::PointerIntPair.57"* %0, %"class.llvm::PointerIntPair.57"** %4, align 8
  store i8* %1, i8** %5, align 8
  %7 = zext i1 %2 to i8
  store i8 %7, i8* %6, align 1
  %8 = load %"class.llvm::PointerIntPair.57"*, %"class.llvm::PointerIntPair.57"** %4, align 8
  %9 = load i8*, i8** %5, align 8
  %10 = load i8, i8* %6, align 1
  %11 = trunc i8 %10 to i1
  call void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEE16setPointerAndIntES1_b(%"class.llvm::PointerIntPair.57"* %8, i8* %9, i1 zeroext %11)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEE16setPointerAndIntES1_b(%"class.llvm::PointerIntPair.57"*, i8*, i1 zeroext) #0 align 2 {
  %4 = alloca %"class.llvm::PointerIntPair.57"*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i8, align 1
  store %"class.llvm::PointerIntPair.57"* %0, %"class.llvm::PointerIntPair.57"** %4, align 8
  store i8* %1, i8** %5, align 8
  %7 = zext i1 %2 to i8
  store i8 %7, i8* %6, align 1
  %8 = load %"class.llvm::PointerIntPair.57"*, %"class.llvm::PointerIntPair.57"** %4, align 8
  %9 = load i8*, i8** %5, align 8
  %10 = call i64 @_ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE13updatePointerElS1_(i64 0, i8* %9)
  %11 = load i8, i8* %6, align 1
  %12 = trunc i8 %11 to i1
  %13 = zext i1 %12 to i64
  %14 = call i64 @_ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE9updateIntEll(i64 %10, i64 %13)
  %15 = getelementptr inbounds %"class.llvm::PointerIntPair.57", %"class.llvm::PointerIntPair.57"* %8, i32 0, i32 0
  store i64 %14, i64* %15, align 8
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr i64 @_ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE9updateIntEll(i64, i64) #0 align 2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  %5 = alloca i64, align 8
  store i64 %0, i64* %3, align 8
  store i64 %1, i64* %4, align 8
  %6 = load i64, i64* %4, align 8
  store i64 %6, i64* %5, align 8
  %7 = load i64, i64* %5, align 8
  %8 = and i64 %7, -2
  %9 = icmp eq i64 %8, 0
  br i1 %9, label %10, label %11

; <label>:10:                                     ; preds = %2
  br label %11

; <label>:11:                                     ; preds = %10, %2
  %12 = phi i1 [ false, %2 ], [ true, %10 ]
  %13 = xor i1 %12, true
  br i1 %13, label %14, label %16

; <label>:14:                                     ; preds = %11
  call void @__assert_rtn(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @__func__._ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE9updateIntEll, i32 0, i32 0), i8* getelementptr inbounds ([65 x i8], [65 x i8]* @.str.13, i32 0, i32 0), i32 167, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.14, i32 0, i32 0)) #8
  unreachable
                                                  ; No predecessors!
  br label %17

; <label>:16:                                     ; preds = %11
  br label %17

; <label>:17:                                     ; preds = %16, %15
  %18 = load i64, i64* %3, align 8
  %19 = and i64 %18, -3
  %20 = load i64, i64* %5, align 8
  %21 = shl i64 %20, 1
  %22 = or i64 %19, %21
  ret i64 %22
}

; Function Attrs: ssp uwtable
define linkonce_odr i64 @_ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE13updatePointerElS1_(i64, i8*) #0 align 2 {
  %3 = alloca i64, align 8
  %4 = alloca i8*, align 8
  %5 = alloca i64, align 8
  store i64 %0, i64* %3, align 8
  store i8* %1, i8** %4, align 8
  %6 = load i8*, i8** %4, align 8
  %7 = call i8* @_ZN4llvm22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEE16getAsVoidPointerEPv(i8* %6)
  %8 = ptrtoint i8* %7 to i64
  store i64 %8, i64* %5, align 8
  %9 = load i64, i64* %5, align 8
  %10 = and i64 %9, 3
  %11 = icmp eq i64 %10, 0
  br i1 %11, label %12, label %13

; <label>:12:                                     ; preds = %2
  br label %13

; <label>:13:                                     ; preds = %12, %2
  %14 = phi i1 [ false, %2 ], [ true, %12 ]
  %15 = xor i1 %14, true
  br i1 %15, label %16, label %18

; <label>:16:                                     ; preds = %13
  call void @__assert_rtn(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @__func__._ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE13updatePointerElS1_, i32 0, i32 0), i8* getelementptr inbounds ([65 x i8], [65 x i8]* @.str.13, i32 0, i32 0), i32 160, i8* getelementptr inbounds ([74 x i8], [74 x i8]* @.str.16, i32 0, i32 0)) #8
  unreachable
                                                  ; No predecessors!
  br label %19

; <label>:18:                                     ; preds = %13
  br label %19

; <label>:19:                                     ; preds = %18, %17
  %20 = load i64, i64* %5, align 8
  %21 = load i64, i64* %3, align 8
  %22 = and i64 %21, 3
  %23 = or i64 %20, %22
  ret i64 %23
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr i8* @_ZN4llvm22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEE16getAsVoidPointerEPv(i8*) #6 align 2 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = load i8*, i8** %2, align 8
  ret i8* %3
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm8DebugLocD2Ev(%"class.llvm::DebugLoc"*) unnamed_addr #6 align 2 {
  %2 = alloca %"class.llvm::DebugLoc"*, align 8
  store %"class.llvm::DebugLoc"* %0, %"class.llvm::DebugLoc"** %2, align 8
  %3 = load %"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::DebugLoc", %"class.llvm::DebugLoc"* %3, i32 0, i32 0
  call void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEED1Ev(%"class.llvm::TypedTrackingMDRef"* %4) #9
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEED1Ev(%"class.llvm::TypedTrackingMDRef"*) unnamed_addr #6 align 2 {
  %2 = alloca %"class.llvm::TypedTrackingMDRef"*, align 8
  store %"class.llvm::TypedTrackingMDRef"* %0, %"class.llvm::TypedTrackingMDRef"** %2, align 8
  %3 = load %"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"** %2, align 8
  call void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEED2Ev(%"class.llvm::TypedTrackingMDRef"* %3) #9
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEED2Ev(%"class.llvm::TypedTrackingMDRef"*) unnamed_addr #6 align 2 {
  %2 = alloca %"class.llvm::TypedTrackingMDRef"*, align 8
  store %"class.llvm::TypedTrackingMDRef"* %0, %"class.llvm::TypedTrackingMDRef"** %2, align 8
  %3 = load %"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::TypedTrackingMDRef", %"class.llvm::TypedTrackingMDRef"* %3, i32 0, i32 0
  call void @_ZN4llvm13TrackingMDRefD1Ev(%"class.llvm::TrackingMDRef"* %4) #9
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm13TrackingMDRefD1Ev(%"class.llvm::TrackingMDRef"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.llvm::TrackingMDRef"*, align 8
  store %"class.llvm::TrackingMDRef"* %0, %"class.llvm::TrackingMDRef"** %2, align 8
  %3 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %2, align 8
  call void @_ZN4llvm13TrackingMDRefD2Ev(%"class.llvm::TrackingMDRef"* %3) #9
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm13TrackingMDRefD2Ev(%"class.llvm::TrackingMDRef"*) unnamed_addr #2 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"class.llvm::TrackingMDRef"*, align 8
  store %"class.llvm::TrackingMDRef"* %0, %"class.llvm::TrackingMDRef"** %2, align 8
  %3 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %2, align 8
  invoke void @_ZN4llvm13TrackingMDRef7untrackEv(%"class.llvm::TrackingMDRef"* %3)
          to label %4 unwind label %5

; <label>:4:                                      ; preds = %1
  ret void

; <label>:5:                                      ; preds = %1
  %6 = landingpad { i8*, i32 }
          catch i8* null
  %7 = extractvalue { i8*, i32 } %6, 0
  call void @__clang_call_terminate(i8* %7) #10
  unreachable
}

; Function Attrs: noinline noreturn nounwind
define linkonce_odr hidden void @__clang_call_terminate(i8*) #7 {
  %2 = call i8* @__cxa_begin_catch(i8* %0) #9
  call void @_ZSt9terminatev() #10
  unreachable
}

declare i8* @__cxa_begin_catch(i8*)

declare void @_ZSt9terminatev()

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm5TwineC2Ev(%"class.llvm::Twine"*) unnamed_addr #0 align 2 {
  %2 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %2, align 8
  %3 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %3, i32 0, i32 0
  %5 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %3, i32 0, i32 1
  %6 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %3, i32 0, i32 2
  store i8 1, i8* %6, align 8
  %7 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %3, i32 0, i32 3
  store i8 1, i8* %7, align 1
  %8 = call zeroext i1 @_ZNK4llvm5Twine7isValidEv(%"class.llvm::Twine"* %3)
  br i1 %8, label %9, label %10

; <label>:9:                                      ; preds = %1
  br label %10

; <label>:10:                                     ; preds = %9, %1
  %11 = phi i1 [ false, %1 ], [ true, %9 ]
  %12 = xor i1 %11, true
  br i1 %12, label %13, label %15

; <label>:13:                                     ; preds = %10
  call void @__assert_rtn(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__._ZN4llvm5TwineC2EPKc, i32 0, i32 0), i8* getelementptr inbounds ([56 x i8], [56 x i8]* @.str.2, i32 0, i32 0), i32 256, i8* getelementptr inbounds ([30 x i8], [30 x i8]* @.str.3, i32 0, i32 0)) #8
  unreachable
                                                  ; No predecessors!
  br label %16

; <label>:15:                                     ; preds = %10
  br label %16

; <label>:16:                                     ; preds = %15, %14
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateFAddEPNS_8ConstantES2_(%"class.llvm::ConstantFolder"*, %"class.llvm::Constant"*, %"class.llvm::Constant"*) #0 align 2 {
  %4 = alloca %"class.llvm::ConstantFolder"*, align 8
  %5 = alloca %"class.llvm::Constant"*, align 8
  %6 = alloca %"class.llvm::Constant"*, align 8
  store %"class.llvm::ConstantFolder"* %0, %"class.llvm::ConstantFolder"** %4, align 8
  store %"class.llvm::Constant"* %1, %"class.llvm::Constant"** %5, align 8
  store %"class.llvm::Constant"* %2, %"class.llvm::Constant"** %6, align 8
  %7 = load %"class.llvm::ConstantFolder"*, %"class.llvm::ConstantFolder"** %4, align 8
  %8 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %5, align 8
  %9 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %6, align 8
  %10 = call %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getFAddEPNS_8ConstantES2_(%"class.llvm::Constant"* %8, %"class.llvm::Constant"* %9)
  ret %"class.llvm::Constant"* %10
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Instruction"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_11InstructionEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Instruction"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %4 = alloca %"class.llvm::IRBuilder"*, align 8
  %5 = alloca %"class.llvm::Instruction"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  %7 = alloca %"class.llvm::ilist_iterator", align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %4, align 8
  store %"class.llvm::Instruction"* %1, %"class.llvm::Instruction"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %8 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %4, align 8
  %9 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderDefaultInserter"*
  %10 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %5, align 8
  %11 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %6, align 8
  %12 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderBase"*
  %13 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %12, i32 0, i32 1
  %14 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %13, align 8
  %15 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderBase"*
  %16 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %15, i32 0, i32 2
  %17 = bitcast %"class.llvm::ilist_iterator"* %7 to i8*
  %18 = bitcast %"class.llvm::ilist_iterator"* %16 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %17, i8* %18, i64 8, i32 8, i1 false)
  %19 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %7, i32 0, i32 0
  %20 = load %"class.llvm::ilist_node_impl.26"*, %"class.llvm::ilist_node_impl.26"** %19, align 8
  call void @_ZNK4llvm24IRBuilderDefaultInserter12InsertHelperEPNS_11InstructionERKNS_5TwineEPNS_10BasicBlockENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEE(%"class.llvm::IRBuilderDefaultInserter"* %9, %"class.llvm::Instruction"* %10, %"class.llvm::Twine"* dereferenceable(24) %11, %"class.llvm::BasicBlock"* %14, %"class.llvm::ilist_node_impl.26"* %20)
  %21 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderBase"*
  %22 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %5, align 8
  call void @_ZNK4llvm13IRBuilderBase20SetInstDebugLocationEPNS_11InstructionE(%"class.llvm::IRBuilderBase"* %21, %"class.llvm::Instruction"* %22)
  %23 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %5, align 8
  ret %"class.llvm::Instruction"* %23
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Instruction"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE19AddFPMathAttributesEPNS_11InstructionEPNS_6MDNodeENS_13FastMathFlagsE(%"class.llvm::IRBuilder"*, %"class.llvm::Instruction"*, %"class.llvm::MDNode"*, i32) #0 align 2 {
  %5 = alloca %"class.llvm::FastMathFlags", align 4
  %6 = alloca %"class.llvm::IRBuilder"*, align 8
  %7 = alloca %"class.llvm::Instruction"*, align 8
  %8 = alloca %"class.llvm::MDNode"*, align 8
  %9 = alloca %"class.llvm::FastMathFlags", align 4
  %10 = getelementptr inbounds %"class.llvm::FastMathFlags", %"class.llvm::FastMathFlags"* %5, i32 0, i32 0
  store i32 %3, i32* %10, align 4
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %6, align 8
  store %"class.llvm::Instruction"* %1, %"class.llvm::Instruction"** %7, align 8
  store %"class.llvm::MDNode"* %2, %"class.llvm::MDNode"** %8, align 8
  %11 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %6, align 8
  %12 = load %"class.llvm::MDNode"*, %"class.llvm::MDNode"** %8, align 8
  %13 = icmp ne %"class.llvm::MDNode"* %12, null
  br i1 %13, label %18, label %14

; <label>:14:                                     ; preds = %4
  %15 = bitcast %"class.llvm::IRBuilder"* %11 to %"class.llvm::IRBuilderBase"*
  %16 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %15, i32 0, i32 4
  %17 = load %"class.llvm::MDNode"*, %"class.llvm::MDNode"** %16, align 8
  store %"class.llvm::MDNode"* %17, %"class.llvm::MDNode"** %8, align 8
  br label %18

; <label>:18:                                     ; preds = %14, %4
  %19 = load %"class.llvm::MDNode"*, %"class.llvm::MDNode"** %8, align 8
  %20 = icmp ne %"class.llvm::MDNode"* %19, null
  br i1 %20, label %21, label %24

; <label>:21:                                     ; preds = %18
  %22 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %7, align 8
  %23 = load %"class.llvm::MDNode"*, %"class.llvm::MDNode"** %8, align 8
  call void @_ZN4llvm11Instruction11setMetadataEjPNS_6MDNodeE(%"class.llvm::Instruction"* %22, i32 3, %"class.llvm::MDNode"* %23)
  br label %24

; <label>:24:                                     ; preds = %21, %18
  %25 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %7, align 8
  %26 = bitcast %"class.llvm::FastMathFlags"* %9 to i8*
  %27 = bitcast %"class.llvm::FastMathFlags"* %5 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %26, i8* %27, i64 4, i32 4, i1 false)
  %28 = getelementptr inbounds %"class.llvm::FastMathFlags", %"class.llvm::FastMathFlags"* %9, i32 0, i32 0
  %29 = load i32, i32* %28, align 4
  call void @_ZN4llvm11Instruction16setFastMathFlagsENS_13FastMathFlagsE(%"class.llvm::Instruction"* %25, i32 %29)
  %30 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %7, align 8
  ret %"class.llvm::Instruction"* %30
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator10CreateFAddEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %4 = alloca %"class.llvm::Value"*, align 8
  %5 = alloca %"class.llvm::Value"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %4, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %7 = load %"class.llvm::Value"*, %"class.llvm::Value"** %4, align 8
  %8 = load %"class.llvm::Value"*, %"class.llvm::Value"** %5, align 8
  %9 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %6, align 8
  %10 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator6CreateENS_11Instruction9BinaryOpsEPNS_5ValueES4_RKNS_5TwineEPS1_(i32 12, %"class.llvm::Value"* %7, %"class.llvm::Value"* %8, %"class.llvm::Twine"* dereferenceable(24) %9, %"class.llvm::Instruction"* null)
  ret %"class.llvm::BinaryOperator"* %10
}

declare %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getFAddEPNS_8ConstantES2_(%"class.llvm::Constant"*, %"class.llvm::Constant"*) #4

declare void @_ZN4llvm11Instruction11setMetadataEjPNS_6MDNodeE(%"class.llvm::Instruction"*, i32, %"class.llvm::MDNode"*) #4

declare void @_ZN4llvm11Instruction16setFastMathFlagsENS_13FastMathFlagsE(%"class.llvm::Instruction"*, i32) #4

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE9CreateSubEPNS_5ValueES5_RKNS_5TwineEbb(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24), i1 zeroext, i1 zeroext) #0 align 2 {
  %7 = alloca %"class.llvm::Value"*, align 8
  %8 = alloca %"class.llvm::IRBuilder"*, align 8
  %9 = alloca %"class.llvm::Value"*, align 8
  %10 = alloca %"class.llvm::Value"*, align 8
  %11 = alloca %"class.llvm::Twine"*, align 8
  %12 = alloca i8, align 1
  %13 = alloca i8, align 1
  %14 = alloca %"class.llvm::Constant"*, align 8
  %15 = alloca %"class.llvm::Constant"*, align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %8, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %9, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %10, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %11, align 8
  %16 = zext i1 %4 to i8
  store i8 %16, i8* %12, align 1
  %17 = zext i1 %5 to i8
  store i8 %17, i8* %13, align 1
  %18 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %8, align 8
  %19 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %20 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %19)
  store %"class.llvm::Constant"* %20, %"class.llvm::Constant"** %14, align 8
  %21 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %14, align 8
  %22 = icmp ne %"class.llvm::Constant"* %21, null
  br i1 %22, label %23, label %41

; <label>:23:                                     ; preds = %6
  %24 = load %"class.llvm::Value"*, %"class.llvm::Value"** %10, align 8
  %25 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %24)
  store %"class.llvm::Constant"* %25, %"class.llvm::Constant"** %15, align 8
  %26 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %15, align 8
  %27 = icmp ne %"class.llvm::Constant"* %26, null
  br i1 %27, label %28, label %40

; <label>:28:                                     ; preds = %23
  %29 = getelementptr inbounds %"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* %18, i32 0, i32 1
  %30 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %14, align 8
  %31 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %15, align 8
  %32 = load i8, i8* %12, align 1
  %33 = trunc i8 %32 to i1
  %34 = load i8, i8* %13, align 1
  %35 = trunc i8 %34 to i1
  %36 = call %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder9CreateSubEPNS_8ConstantES2_bb(%"class.llvm::ConstantFolder"* %29, %"class.llvm::Constant"* %30, %"class.llvm::Constant"* %31, i1 zeroext %33, i1 zeroext %35)
  %37 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %11, align 8
  %38 = call %"class.llvm::Constant"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertEPNS_8ConstantERKNS_5TwineE(%"class.llvm::IRBuilder"* %18, %"class.llvm::Constant"* %36, %"class.llvm::Twine"* dereferenceable(24) %37)
  %39 = bitcast %"class.llvm::Constant"* %38 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %39, %"class.llvm::Value"** %7, align 8
  br label %51

; <label>:40:                                     ; preds = %23
  br label %41

; <label>:41:                                     ; preds = %40, %6
  %42 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %43 = load %"class.llvm::Value"*, %"class.llvm::Value"** %10, align 8
  %44 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %11, align 8
  %45 = load i8, i8* %12, align 1
  %46 = trunc i8 %45 to i1
  %47 = load i8, i8* %13, align 1
  %48 = trunc i8 %47 to i1
  %49 = call %"class.llvm::BinaryOperator"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE23CreateInsertNUWNSWBinOpENS_11Instruction9BinaryOpsEPNS_5ValueES7_RKNS_5TwineEbb(%"class.llvm::IRBuilder"* %18, i32 13, %"class.llvm::Value"* %42, %"class.llvm::Value"* %43, %"class.llvm::Twine"* dereferenceable(24) %44, i1 zeroext %46, i1 zeroext %48)
  %50 = bitcast %"class.llvm::BinaryOperator"* %49 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %50, %"class.llvm::Value"** %7, align 8
  br label %51

; <label>:51:                                     ; preds = %41, %28
  %52 = load %"class.llvm::Value"*, %"class.llvm::Value"** %7, align 8
  ret %"class.llvm::Value"* %52
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder9CreateSubEPNS_8ConstantES2_bb(%"class.llvm::ConstantFolder"*, %"class.llvm::Constant"*, %"class.llvm::Constant"*, i1 zeroext, i1 zeroext) #0 align 2 {
  %6 = alloca %"class.llvm::ConstantFolder"*, align 8
  %7 = alloca %"class.llvm::Constant"*, align 8
  %8 = alloca %"class.llvm::Constant"*, align 8
  %9 = alloca i8, align 1
  %10 = alloca i8, align 1
  store %"class.llvm::ConstantFolder"* %0, %"class.llvm::ConstantFolder"** %6, align 8
  store %"class.llvm::Constant"* %1, %"class.llvm::Constant"** %7, align 8
  store %"class.llvm::Constant"* %2, %"class.llvm::Constant"** %8, align 8
  %11 = zext i1 %3 to i8
  store i8 %11, i8* %9, align 1
  %12 = zext i1 %4 to i8
  store i8 %12, i8* %10, align 1
  %13 = load %"class.llvm::ConstantFolder"*, %"class.llvm::ConstantFolder"** %6, align 8
  %14 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %7, align 8
  %15 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %8, align 8
  %16 = load i8, i8* %9, align 1
  %17 = trunc i8 %16 to i1
  %18 = load i8, i8* %10, align 1
  %19 = trunc i8 %18 to i1
  %20 = call %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr6getSubEPNS_8ConstantES2_bb(%"class.llvm::Constant"* %14, %"class.llvm::Constant"* %15, i1 zeroext %17, i1 zeroext %19)
  ret %"class.llvm::Constant"* %20
}

declare %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr6getSubEPNS_8ConstantES2_bb(%"class.llvm::Constant"*, %"class.llvm::Constant"*, i1 zeroext, i1 zeroext) #4

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateFSubEPNS_8ConstantES2_(%"class.llvm::ConstantFolder"*, %"class.llvm::Constant"*, %"class.llvm::Constant"*) #0 align 2 {
  %4 = alloca %"class.llvm::ConstantFolder"*, align 8
  %5 = alloca %"class.llvm::Constant"*, align 8
  %6 = alloca %"class.llvm::Constant"*, align 8
  store %"class.llvm::ConstantFolder"* %0, %"class.llvm::ConstantFolder"** %4, align 8
  store %"class.llvm::Constant"* %1, %"class.llvm::Constant"** %5, align 8
  store %"class.llvm::Constant"* %2, %"class.llvm::Constant"** %6, align 8
  %7 = load %"class.llvm::ConstantFolder"*, %"class.llvm::ConstantFolder"** %4, align 8
  %8 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %5, align 8
  %9 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %6, align 8
  %10 = call %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getFSubEPNS_8ConstantES2_(%"class.llvm::Constant"* %8, %"class.llvm::Constant"* %9)
  ret %"class.llvm::Constant"* %10
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator10CreateFSubEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %4 = alloca %"class.llvm::Value"*, align 8
  %5 = alloca %"class.llvm::Value"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %4, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %7 = load %"class.llvm::Value"*, %"class.llvm::Value"** %4, align 8
  %8 = load %"class.llvm::Value"*, %"class.llvm::Value"** %5, align 8
  %9 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %6, align 8
  %10 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator6CreateENS_11Instruction9BinaryOpsEPNS_5ValueES4_RKNS_5TwineEPS1_(i32 14, %"class.llvm::Value"* %7, %"class.llvm::Value"* %8, %"class.llvm::Twine"* dereferenceable(24) %9, %"class.llvm::Instruction"* null)
  ret %"class.llvm::BinaryOperator"* %10
}

declare %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getFSubEPNS_8ConstantES2_(%"class.llvm::Constant"*, %"class.llvm::Constant"*) #4

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder9CreateMulEPNS_8ConstantES2_bb(%"class.llvm::ConstantFolder"*, %"class.llvm::Constant"*, %"class.llvm::Constant"*, i1 zeroext, i1 zeroext) #0 align 2 {
  %6 = alloca %"class.llvm::ConstantFolder"*, align 8
  %7 = alloca %"class.llvm::Constant"*, align 8
  %8 = alloca %"class.llvm::Constant"*, align 8
  %9 = alloca i8, align 1
  %10 = alloca i8, align 1
  store %"class.llvm::ConstantFolder"* %0, %"class.llvm::ConstantFolder"** %6, align 8
  store %"class.llvm::Constant"* %1, %"class.llvm::Constant"** %7, align 8
  store %"class.llvm::Constant"* %2, %"class.llvm::Constant"** %8, align 8
  %11 = zext i1 %3 to i8
  store i8 %11, i8* %9, align 1
  %12 = zext i1 %4 to i8
  store i8 %12, i8* %10, align 1
  %13 = load %"class.llvm::ConstantFolder"*, %"class.llvm::ConstantFolder"** %6, align 8
  %14 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %7, align 8
  %15 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %8, align 8
  %16 = load i8, i8* %9, align 1
  %17 = trunc i8 %16 to i1
  %18 = load i8, i8* %10, align 1
  %19 = trunc i8 %18 to i1
  %20 = call %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr6getMulEPNS_8ConstantES2_bb(%"class.llvm::Constant"* %14, %"class.llvm::Constant"* %15, i1 zeroext %17, i1 zeroext %19)
  ret %"class.llvm::Constant"* %20
}

declare %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr6getMulEPNS_8ConstantES2_bb(%"class.llvm::Constant"*, %"class.llvm::Constant"*, i1 zeroext, i1 zeroext) #4

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateFMulEPNS_8ConstantES2_(%"class.llvm::ConstantFolder"*, %"class.llvm::Constant"*, %"class.llvm::Constant"*) #0 align 2 {
  %4 = alloca %"class.llvm::ConstantFolder"*, align 8
  %5 = alloca %"class.llvm::Constant"*, align 8
  %6 = alloca %"class.llvm::Constant"*, align 8
  store %"class.llvm::ConstantFolder"* %0, %"class.llvm::ConstantFolder"** %4, align 8
  store %"class.llvm::Constant"* %1, %"class.llvm::Constant"** %5, align 8
  store %"class.llvm::Constant"* %2, %"class.llvm::Constant"** %6, align 8
  %7 = load %"class.llvm::ConstantFolder"*, %"class.llvm::ConstantFolder"** %4, align 8
  %8 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %5, align 8
  %9 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %6, align 8
  %10 = call %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getFMulEPNS_8ConstantES2_(%"class.llvm::Constant"* %8, %"class.llvm::Constant"* %9)
  ret %"class.llvm::Constant"* %10
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator10CreateFMulEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %4 = alloca %"class.llvm::Value"*, align 8
  %5 = alloca %"class.llvm::Value"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %4, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %7 = load %"class.llvm::Value"*, %"class.llvm::Value"** %4, align 8
  %8 = load %"class.llvm::Value"*, %"class.llvm::Value"** %5, align 8
  %9 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %6, align 8
  %10 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator6CreateENS_11Instruction9BinaryOpsEPNS_5ValueES4_RKNS_5TwineEPS1_(i32 16, %"class.llvm::Value"* %7, %"class.llvm::Value"* %8, %"class.llvm::Twine"* dereferenceable(24) %9, %"class.llvm::Instruction"* null)
  ret %"class.llvm::BinaryOperator"* %10
}

declare %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getFMulEPNS_8ConstantES2_(%"class.llvm::Constant"*, %"class.llvm::Constant"*) #4

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateSDivEPNS_8ConstantES2_b(%"class.llvm::ConstantFolder"*, %"class.llvm::Constant"*, %"class.llvm::Constant"*, i1 zeroext) #0 align 2 {
  %5 = alloca %"class.llvm::ConstantFolder"*, align 8
  %6 = alloca %"class.llvm::Constant"*, align 8
  %7 = alloca %"class.llvm::Constant"*, align 8
  %8 = alloca i8, align 1
  store %"class.llvm::ConstantFolder"* %0, %"class.llvm::ConstantFolder"** %5, align 8
  store %"class.llvm::Constant"* %1, %"class.llvm::Constant"** %6, align 8
  store %"class.llvm::Constant"* %2, %"class.llvm::Constant"** %7, align 8
  %9 = zext i1 %3 to i8
  store i8 %9, i8* %8, align 1
  %10 = load %"class.llvm::ConstantFolder"*, %"class.llvm::ConstantFolder"** %5, align 8
  %11 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %6, align 8
  %12 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %7, align 8
  %13 = load i8, i8* %8, align 1
  %14 = trunc i8 %13 to i1
  %15 = call %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getSDivEPNS_8ConstantES2_b(%"class.llvm::Constant"* %11, %"class.llvm::Constant"* %12, i1 zeroext %14)
  ret %"class.llvm::Constant"* %15
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator10CreateSDivEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %4 = alloca %"class.llvm::Value"*, align 8
  %5 = alloca %"class.llvm::Value"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %4, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %7 = load %"class.llvm::Value"*, %"class.llvm::Value"** %4, align 8
  %8 = load %"class.llvm::Value"*, %"class.llvm::Value"** %5, align 8
  %9 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %6, align 8
  %10 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator6CreateENS_11Instruction9BinaryOpsEPNS_5ValueES4_RKNS_5TwineEPS1_(i32 18, %"class.llvm::Value"* %7, %"class.llvm::Value"* %8, %"class.llvm::Twine"* dereferenceable(24) %9, %"class.llvm::Instruction"* null)
  ret %"class.llvm::BinaryOperator"* %10
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator15CreateExactSDivEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %4 = alloca %"class.llvm::Value"*, align 8
  %5 = alloca %"class.llvm::Value"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %4, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %7 = load %"class.llvm::Value"*, %"class.llvm::Value"** %4, align 8
  %8 = load %"class.llvm::Value"*, %"class.llvm::Value"** %5, align 8
  %9 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %6, align 8
  %10 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator11CreateExactENS_11Instruction9BinaryOpsEPNS_5ValueES4_RKNS_5TwineE(i32 18, %"class.llvm::Value"* %7, %"class.llvm::Value"* %8, %"class.llvm::Twine"* dereferenceable(24) %9)
  ret %"class.llvm::BinaryOperator"* %10
}

declare %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getSDivEPNS_8ConstantES2_b(%"class.llvm::Constant"*, %"class.llvm::Constant"*, i1 zeroext) #4

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator11CreateExactENS_11Instruction9BinaryOpsEPNS_5ValueES4_RKNS_5TwineE(i32, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %5 = alloca i32, align 4
  %6 = alloca %"class.llvm::Value"*, align 8
  %7 = alloca %"class.llvm::Value"*, align 8
  %8 = alloca %"class.llvm::Twine"*, align 8
  %9 = alloca %"class.llvm::BinaryOperator"*, align 8
  store i32 %0, i32* %5, align 4
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %6, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %7, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %8, align 8
  %10 = load i32, i32* %5, align 4
  %11 = load %"class.llvm::Value"*, %"class.llvm::Value"** %6, align 8
  %12 = load %"class.llvm::Value"*, %"class.llvm::Value"** %7, align 8
  %13 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %8, align 8
  %14 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator6CreateENS_11Instruction9BinaryOpsEPNS_5ValueES4_RKNS_5TwineEPS1_(i32 %10, %"class.llvm::Value"* %11, %"class.llvm::Value"* %12, %"class.llvm::Twine"* dereferenceable(24) %13, %"class.llvm::Instruction"* null)
  store %"class.llvm::BinaryOperator"* %14, %"class.llvm::BinaryOperator"** %9, align 8
  %15 = load %"class.llvm::BinaryOperator"*, %"class.llvm::BinaryOperator"** %9, align 8
  %16 = bitcast %"class.llvm::BinaryOperator"* %15 to %"class.llvm::Instruction"*
  call void @_ZN4llvm11Instruction10setIsExactEb(%"class.llvm::Instruction"* %16, i1 zeroext true)
  %17 = load %"class.llvm::BinaryOperator"*, %"class.llvm::BinaryOperator"** %9, align 8
  ret %"class.llvm::BinaryOperator"* %17
}

declare void @_ZN4llvm11Instruction10setIsExactEb(%"class.llvm::Instruction"*, i1 zeroext) #4

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateFDivEPNS_8ConstantES2_(%"class.llvm::ConstantFolder"*, %"class.llvm::Constant"*, %"class.llvm::Constant"*) #0 align 2 {
  %4 = alloca %"class.llvm::ConstantFolder"*, align 8
  %5 = alloca %"class.llvm::Constant"*, align 8
  %6 = alloca %"class.llvm::Constant"*, align 8
  store %"class.llvm::ConstantFolder"* %0, %"class.llvm::ConstantFolder"** %4, align 8
  store %"class.llvm::Constant"* %1, %"class.llvm::Constant"** %5, align 8
  store %"class.llvm::Constant"* %2, %"class.llvm::Constant"** %6, align 8
  %7 = load %"class.llvm::ConstantFolder"*, %"class.llvm::ConstantFolder"** %4, align 8
  %8 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %5, align 8
  %9 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %6, align 8
  %10 = call %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getFDivEPNS_8ConstantES2_(%"class.llvm::Constant"* %8, %"class.llvm::Constant"* %9)
  ret %"class.llvm::Constant"* %10
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator10CreateFDivEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %4 = alloca %"class.llvm::Value"*, align 8
  %5 = alloca %"class.llvm::Value"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %4, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %7 = load %"class.llvm::Value"*, %"class.llvm::Value"** %4, align 8
  %8 = load %"class.llvm::Value"*, %"class.llvm::Value"** %5, align 8
  %9 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %6, align 8
  %10 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator6CreateENS_11Instruction9BinaryOpsEPNS_5ValueES4_RKNS_5TwineEPS1_(i32 19, %"class.llvm::Value"* %7, %"class.llvm::Value"* %8, %"class.llvm::Twine"* dereferenceable(24) %9, %"class.llvm::Instruction"* null)
  ret %"class.llvm::BinaryOperator"* %10
}

declare %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getFDivEPNS_8ConstantES2_(%"class.llvm::Constant"*, %"class.llvm::Constant"*) #4

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateSRemEPNS_8ConstantES2_(%"class.llvm::ConstantFolder"*, %"class.llvm::Constant"*, %"class.llvm::Constant"*) #0 align 2 {
  %4 = alloca %"class.llvm::ConstantFolder"*, align 8
  %5 = alloca %"class.llvm::Constant"*, align 8
  %6 = alloca %"class.llvm::Constant"*, align 8
  store %"class.llvm::ConstantFolder"* %0, %"class.llvm::ConstantFolder"** %4, align 8
  store %"class.llvm::Constant"* %1, %"class.llvm::Constant"** %5, align 8
  store %"class.llvm::Constant"* %2, %"class.llvm::Constant"** %6, align 8
  %7 = load %"class.llvm::ConstantFolder"*, %"class.llvm::ConstantFolder"** %4, align 8
  %8 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %5, align 8
  %9 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %6, align 8
  %10 = call %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getSRemEPNS_8ConstantES2_(%"class.llvm::Constant"* %8, %"class.llvm::Constant"* %9)
  ret %"class.llvm::Constant"* %10
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator10CreateSRemEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %4 = alloca %"class.llvm::Value"*, align 8
  %5 = alloca %"class.llvm::Value"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %4, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %7 = load %"class.llvm::Value"*, %"class.llvm::Value"** %4, align 8
  %8 = load %"class.llvm::Value"*, %"class.llvm::Value"** %5, align 8
  %9 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %6, align 8
  %10 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator6CreateENS_11Instruction9BinaryOpsEPNS_5ValueES4_RKNS_5TwineEPS1_(i32 21, %"class.llvm::Value"* %7, %"class.llvm::Value"* %8, %"class.llvm::Twine"* dereferenceable(24) %9, %"class.llvm::Instruction"* null)
  ret %"class.llvm::BinaryOperator"* %10
}

declare %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getSRemEPNS_8ConstantES2_(%"class.llvm::Constant"*, %"class.llvm::Constant"*) #4

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateFRemEPNS_8ConstantES2_(%"class.llvm::ConstantFolder"*, %"class.llvm::Constant"*, %"class.llvm::Constant"*) #0 align 2 {
  %4 = alloca %"class.llvm::ConstantFolder"*, align 8
  %5 = alloca %"class.llvm::Constant"*, align 8
  %6 = alloca %"class.llvm::Constant"*, align 8
  store %"class.llvm::ConstantFolder"* %0, %"class.llvm::ConstantFolder"** %4, align 8
  store %"class.llvm::Constant"* %1, %"class.llvm::Constant"** %5, align 8
  store %"class.llvm::Constant"* %2, %"class.llvm::Constant"** %6, align 8
  %7 = load %"class.llvm::ConstantFolder"*, %"class.llvm::ConstantFolder"** %4, align 8
  %8 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %5, align 8
  %9 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %6, align 8
  %10 = call %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getFRemEPNS_8ConstantES2_(%"class.llvm::Constant"* %8, %"class.llvm::Constant"* %9)
  ret %"class.llvm::Constant"* %10
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator10CreateFRemEPNS_5ValueES2_RKNS_5TwineE(%"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %4 = alloca %"class.llvm::Value"*, align 8
  %5 = alloca %"class.llvm::Value"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %4, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %7 = load %"class.llvm::Value"*, %"class.llvm::Value"** %4, align 8
  %8 = load %"class.llvm::Value"*, %"class.llvm::Value"** %5, align 8
  %9 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %6, align 8
  %10 = call %"class.llvm::BinaryOperator"* @_ZN4llvm14BinaryOperator6CreateENS_11Instruction9BinaryOpsEPNS_5ValueES4_RKNS_5TwineEPS1_(i32 22, %"class.llvm::Value"* %7, %"class.llvm::Value"* %8, %"class.llvm::Twine"* dereferenceable(24) %9, %"class.llvm::Instruction"* null)
  ret %"class.llvm::BinaryOperator"* %10
}

declare %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getFRemEPNS_8ConstantES2_(%"class.llvm::Constant"*, %"class.llvm::Constant"*) #4

attributes #0 = { ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noreturn "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { inlinehint ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { argmemonly nounwind }
attributes #6 = { inlinehint nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { noinline noreturn nounwind }
attributes #8 = { noreturn }
attributes #9 = { nounwind }
attributes #10 = { noreturn nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"Apple LLVM version 8.1.0 (clang-802.0.42)"}
