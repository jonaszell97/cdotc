; ModuleID = '/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/AST/Visitor/CodeGen/CGMemory.cpp'
source_filename = "/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/AST/Visitor/CodeGen/CGMemory.cpp"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.12.0"

%"class.llvm::BasicBlock" = type { %"class.llvm::Value", %"class.llvm::ilist_node_with_parent", %"class.llvm::SymbolTableList", %"class.llvm::Function"* }
%"class.llvm::Value" = type { i32 (...)**, %"class.llvm::Type"*, %"class.llvm::Use"*, i8, i8, i16, i32 }
%"class.llvm::Type" = type { %"class.llvm::LLVMContext"*, i32, i32, %"class.llvm::Type"** }
%"class.llvm::LLVMContext" = type { %"class.llvm::LLVMContextImpl"* }
%"class.llvm::LLVMContextImpl" = type opaque
%"class.llvm::Use" = type { %"class.llvm::Value"*, %"class.llvm::Use"*, %"class.llvm::PointerIntPair" }
%"class.llvm::PointerIntPair" = type { i64 }
%"class.llvm::ilist_node_with_parent" = type { %"class.llvm::ilist_node" }
%"class.llvm::ilist_node" = type { %"class.llvm::ilist_node_impl" }
%"class.llvm::ilist_node_impl" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::ilist_node_base" = type { %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"* }
%"class.llvm::SymbolTableList" = type { %"class.llvm::iplist_impl" }
%"class.llvm::iplist_impl" = type { %"class.llvm::simple_ilist" }
%"class.llvm::simple_ilist" = type { %"class.llvm::ilist_sentinel" }
%"class.llvm::ilist_sentinel" = type { %"class.llvm::ilist_node_impl.0" }
%"class.llvm::ilist_node_impl.0" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::Function" = type { %"class.llvm::GlobalObject", %"class.llvm::ilist_node.1", %"class.llvm::SymbolTableList.3", %"class.llvm::SymbolTableList.10", %"class.std::__1::unique_ptr", %"class.llvm::AttributeSet" }
%"class.llvm::GlobalObject" = type { %"class.llvm::GlobalValue", %"class.llvm::Comdat"* }
%"class.llvm::GlobalValue" = type { %"class.llvm::Constant", %"class.llvm::Type"*, i32, i32, %"class.llvm::Module"* }
%"class.llvm::Constant" = type { %"class.llvm::User" }
%"class.llvm::User" = type { %"class.llvm::Value" }
%"class.llvm::Module" = type opaque
%"class.llvm::Comdat" = type opaque
%"class.llvm::ilist_node.1" = type { %"class.llvm::ilist_node_impl.2" }
%"class.llvm::ilist_node_impl.2" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::SymbolTableList.3" = type { %"class.llvm::iplist_impl.4" }
%"class.llvm::iplist_impl.4" = type { %"class.llvm::simple_ilist.7" }
%"class.llvm::simple_ilist.7" = type { %"class.llvm::ilist_sentinel.9" }
%"class.llvm::ilist_sentinel.9" = type { %"class.llvm::ilist_node_impl" }
%"class.llvm::SymbolTableList.10" = type { %"class.llvm::iplist_impl.11" }
%"class.llvm::iplist_impl.11" = type { %"class.llvm::simple_ilist.14" }
%"class.llvm::simple_ilist.14" = type { %"class.llvm::ilist_sentinel.16" }
%"class.llvm::ilist_sentinel.16" = type { %"class.llvm::ilist_node_impl.17" }
%"class.llvm::ilist_node_impl.17" = type { %"class.llvm::ilist_node_base" }
%"class.std::__1::unique_ptr" = type { %"class.std::__1::__compressed_pair" }
%"class.std::__1::__compressed_pair" = type { %"class.std::__1::__libcpp_compressed_pair_imp" }
%"class.std::__1::__libcpp_compressed_pair_imp" = type { %"class.llvm::ValueSymbolTable"* }
%"class.llvm::ValueSymbolTable" = type opaque
%"class.llvm::AttributeSet" = type { %"class.llvm::AttributeSetImpl"* }
%"class.llvm::AttributeSetImpl" = type opaque
%"class.std::__1::unique_ptr.23" = type { %"class.std::__1::__compressed_pair.24" }
%"class.std::__1::__compressed_pair.24" = type { %"class.std::__1::__libcpp_compressed_pair_imp.25" }
%"class.std::__1::__libcpp_compressed_pair_imp.25" = type { %"class.llvm::Module"* }
%"class.llvm::IRBuilder" = type <{ %"class.llvm::IRBuilderBase", %"class.llvm::ConstantFolder", [7 x i8] }>
%"class.llvm::IRBuilderBase" = type { %"class.llvm::DebugLoc", %"class.llvm::BasicBlock"*, %"class.llvm::ilist_iterator", %"class.llvm::LLVMContext"*, %"class.llvm::MDNode"*, %"class.llvm::FastMathFlags", %"class.llvm::ArrayRef" }
%"class.llvm::DebugLoc" = type { %"class.llvm::TypedTrackingMDRef" }
%"class.llvm::TypedTrackingMDRef" = type { %"class.llvm::TrackingMDRef" }
%"class.llvm::TrackingMDRef" = type { %"class.llvm::Metadata"* }
%"class.llvm::Metadata" = type { i8, i8, i16, i32 }
%"class.llvm::ilist_iterator" = type { %"class.llvm::ilist_node_impl.0"* }
%"class.llvm::MDNode" = type { %"class.llvm::Metadata", i32, i32, %"class.llvm::ContextAndReplaceableUses" }
%"class.llvm::ContextAndReplaceableUses" = type { %"class.llvm::PointerUnion" }
%"class.llvm::PointerUnion" = type { %"class.llvm::PointerIntPair.28" }
%"class.llvm::PointerIntPair.28" = type { i64 }
%"class.llvm::FastMathFlags" = type { i32 }
%"class.llvm::ArrayRef" = type { %"class.llvm::OperandBundleDefT"*, i64 }
%"class.llvm::OperandBundleDefT" = type { %"class.std::__1::basic_string", %"class.std::__1::vector" }
%"class.std::__1::basic_string" = type { %"class.std::__1::__compressed_pair.18" }
%"class.std::__1::__compressed_pair.18" = type { %"class.std::__1::__libcpp_compressed_pair_imp.19" }
%"class.std::__1::__libcpp_compressed_pair_imp.19" = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" = type { %union.anon }
%union.anon = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" = type { i64, i64, i8* }
%"class.std::__1::vector" = type { %"class.std::__1::__vector_base" }
%"class.std::__1::__vector_base" = type { %"class.llvm::Value"**, %"class.llvm::Value"**, %"class.std::__1::__compressed_pair.29" }
%"class.std::__1::__compressed_pair.29" = type { %"class.std::__1::__libcpp_compressed_pair_imp.30" }
%"class.std::__1::__libcpp_compressed_pair_imp.30" = type { %"class.llvm::Value"** }
%"class.llvm::ConstantFolder" = type { i8 }
%"class.std::__1::unordered_map" = type { %"class.std::__1::__hash_table" }
%"class.std::__1::__hash_table" = type <{ %"class.std::__1::unique_ptr.37", %"class.std::__1::__compressed_pair.44", %"class.std::__1::__compressed_pair.48", %"class.std::__1::__compressed_pair.50", [4 x i8] }>
%"class.std::__1::unique_ptr.37" = type { %"class.std::__1::__compressed_pair.38" }
%"class.std::__1::__compressed_pair.38" = type { %"class.std::__1::__libcpp_compressed_pair_imp.39" }
%"class.std::__1::__libcpp_compressed_pair_imp.39" = type { %"struct.std::__1::__hash_node_base"**, %"class.std::__1::__bucket_list_deallocator" }
%"struct.std::__1::__hash_node_base" = type { %"struct.std::__1::__hash_node_base"* }
%"class.std::__1::__bucket_list_deallocator" = type { %"class.std::__1::__compressed_pair.40" }
%"class.std::__1::__compressed_pair.40" = type { %"class.std::__1::__libcpp_compressed_pair_imp.41" }
%"class.std::__1::__libcpp_compressed_pair_imp.41" = type { i64 }
%"class.std::__1::__compressed_pair.44" = type { %"class.std::__1::__libcpp_compressed_pair_imp.45" }
%"class.std::__1::__libcpp_compressed_pair_imp.45" = type { %"struct.std::__1::__hash_node_base" }
%"class.std::__1::__compressed_pair.48" = type { %"class.std::__1::__libcpp_compressed_pair_imp.49" }
%"class.std::__1::__libcpp_compressed_pair_imp.49" = type { i64 }
%"class.std::__1::__compressed_pair.50" = type { %"class.std::__1::__libcpp_compressed_pair_imp.51" }
%"class.std::__1::__libcpp_compressed_pair_imp.51" = type { float }
%"struct.std::__1::piecewise_construct_t" = type { i8 }
%"class.std::__1::allocator" = type { i8 }
%"class.llvm::GlobalVariable" = type <{ %"class.llvm::GlobalObject", %"class.llvm::ilist_node.21", i8, [7 x i8] }>
%"class.llvm::ilist_node.21" = type { %"class.llvm::ilist_node_impl.22" }
%"class.llvm::ilist_node_impl.22" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::Twine" = type <{ %"union.llvm::Twine::Child", %"union.llvm::Twine::Child", i8, i8, [6 x i8] }>
%"union.llvm::Twine::Child" = type { %"class.llvm::Twine"* }
%"class.llvm::StructType" = type { %"class.llvm::CompositeType", i8* }
%"class.llvm::CompositeType" = type { %"class.llvm::Type" }
%"class.llvm::CallInst" = type { %"class.llvm::Instruction", %"class.llvm::AttributeSet", %"class.llvm::FunctionType"* }
%"class.llvm::Instruction" = type { %"class.llvm::User", %"class.llvm::ilist_node_with_parent.34", %"class.llvm::BasicBlock"*, %"class.llvm::DebugLoc" }
%"class.llvm::ilist_node_with_parent.34" = type { %"class.llvm::ilist_node.35" }
%"class.llvm::ilist_node.35" = type { %"class.llvm::ilist_node_impl.0" }
%"class.llvm::FunctionType" = type { %"class.llvm::Type" }
%"class.llvm::ArrayRef.36" = type { %"class.llvm::Value"**, i64 }
%"class.std::initializer_list" = type { %"class.llvm::Value"**, i64 }
%"class.llvm::AllocaInst" = type { %"class.llvm::UnaryInstruction", %"class.llvm::Type"* }
%"class.llvm::UnaryInstruction" = type { %"class.llvm::Instruction" }
%"class.llvm::ConstantAggregateZero" = type { %"class.llvm::ConstantData" }
%"class.llvm::ConstantData" = type { %"class.llvm::Constant" }
%"class.llvm::PointerType" = type { %"class.llvm::Type", %"class.llvm::Type"* }
%"class.llvm::ConstantPointerNull" = type { %"class.llvm::ConstantData" }
%"class.llvm::IntegerType" = type { %"class.llvm::Type" }
%"class.llvm::ConstantInt" = type { %"class.llvm::ConstantData", %"class.llvm::APInt" }
%"class.llvm::APInt" = type { i32, %union.anon.33 }
%union.anon.33 = type { i64 }
%"class.std::__1::__basic_string_common" = type { i8 }
%"class.std::__1::tuple.91" = type { i8 }
%"class.std::__1::__tuple_leaf" = type { %"class.std::__1::basic_string"* }
%"struct.std::__1::__tuple_indices" = type { i8 }
%"struct.std::__1::__tuple_types" = type { i8 }
%"struct.std::__1::__tuple_indices.98" = type { i8 }
%"struct.std::__1::__tuple_types.100" = type { i8 }
%"struct.std::__1::__tuple_impl" = type { %"class.std::__1::__tuple_leaf" }
%"class.std::__1::tuple" = type { %"struct.std::__1::__tuple_impl" }
%"struct.std::__1::equal_to" = type { i8 }
%"class.std::__1::__unordered_map_equal" = type { i8 }
%"union.std::__1::__hash_value_type" = type { %"struct.std::__1::pair.92" }
%"struct.std::__1::pair.92" = type { %"class.std::__1::basic_string", %"class.llvm::Constant"* }
%"struct.std::__1::__less" = type { i8 }
%"class.std::__1::__libcpp_compressed_pair_imp.96" = type { %"struct.std::__1::__hash_node"*, %"class.std::__1::__hash_node_destructor" }
%"struct.std::__1::__hash_node" = type { %"struct.std::__1::__hash_node_base", i64, %"union.std::__1::__hash_value_type" }
%"class.std::__1::__hash_node_destructor" = type <{ %"class.std::__1::allocator.46"*, i8, [7 x i8] }>
%"class.std::__1::allocator.46" = type { i8 }
%"class.std::__1::__compressed_pair.95" = type { %"class.std::__1::__libcpp_compressed_pair_imp.96" }
%"struct.std::__1::integral_constant.99" = type { i8 }
%"struct.std::__1::__has_destroy" = type { i8 }
%"class.std::__1::unique_ptr.94" = type { %"class.std::__1::__compressed_pair.95" }
%"class.std::__1::__unordered_map_hasher" = type { i8 }
%"class.std::__1::__hash_iterator" = type { %"struct.std::__1::__hash_node_base"* }
%"struct.std::__1::pair" = type <{ %"class.std::__1::__hash_iterator", i8, [7 x i8] }>
%"struct.std::__1::hash" = type { i8 }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short" = type { %union.anon.20, [23 x i8] }
%union.anon.20 = type { i8 }
%struct.TypeSpecifier = type { i32, %"class.std::__1::basic_string", %"class.std::__1::shared_ptr", i8, i8, i8, i8, i64, i32, i8, i8, %"class.std::__1::basic_string", i8, [7 x i8], %"class.std::__1::unordered_map.52", i8, i8, i8, i8, i8, i8, i8, i32, %"class.std::__1::vector.74", %"class.std::__1::vector.80", %union.anon.86 }
%"class.std::__1::shared_ptr" = type { %class.Expression*, %"class.std::__1::__shared_weak_count"* }
%class.Expression = type opaque
%"class.std::__1::__shared_weak_count" = type { %"class.std::__1::__shared_count", i64 }
%"class.std::__1::__shared_count" = type { i32 (...)**, i64 }
%"class.std::__1::unordered_map.52" = type { %"class.std::__1::__hash_table.53" }
%"class.std::__1::__hash_table.53" = type <{ %"class.std::__1::unique_ptr.54", %"class.std::__1::__compressed_pair.63", %"class.std::__1::__compressed_pair.67", %"class.std::__1::__compressed_pair.70", [4 x i8] }>
%"class.std::__1::unique_ptr.54" = type { %"class.std::__1::__compressed_pair.55" }
%"class.std::__1::__compressed_pair.55" = type { %"class.std::__1::__libcpp_compressed_pair_imp.56" }
%"class.std::__1::__libcpp_compressed_pair_imp.56" = type { %"struct.std::__1::__hash_node_base.57"**, %"class.std::__1::__bucket_list_deallocator.58" }
%"struct.std::__1::__hash_node_base.57" = type { %"struct.std::__1::__hash_node_base.57"* }
%"class.std::__1::__bucket_list_deallocator.58" = type { %"class.std::__1::__compressed_pair.59" }
%"class.std::__1::__compressed_pair.59" = type { %"class.std::__1::__libcpp_compressed_pair_imp.60" }
%"class.std::__1::__libcpp_compressed_pair_imp.60" = type { i64 }
%"class.std::__1::__compressed_pair.63" = type { %"class.std::__1::__libcpp_compressed_pair_imp.64" }
%"class.std::__1::__libcpp_compressed_pair_imp.64" = type { %"struct.std::__1::__hash_node_base.57" }
%"class.std::__1::__compressed_pair.67" = type { %"class.std::__1::__libcpp_compressed_pair_imp.68" }
%"class.std::__1::__libcpp_compressed_pair_imp.68" = type { i64 }
%"class.std::__1::__compressed_pair.70" = type { %"class.std::__1::__libcpp_compressed_pair_imp.71" }
%"class.std::__1::__libcpp_compressed_pair_imp.71" = type { float }
%"class.std::__1::vector.74" = type { %"class.std::__1::__vector_base.75" }
%"class.std::__1::__vector_base.75" = type { %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"*, %"class.std::__1::__compressed_pair.76" }
%"class.std::__1::__compressed_pair.76" = type { %"class.std::__1::__libcpp_compressed_pair_imp.77" }
%"class.std::__1::__libcpp_compressed_pair_imp.77" = type { %"class.std::__1::basic_string"* }
%"class.std::__1::vector.80" = type { %"class.std::__1::__vector_base.81" }
%"class.std::__1::__vector_base.81" = type { %struct.TypeSpecifier*, %struct.TypeSpecifier*, %"class.std::__1::__compressed_pair.82" }
%"class.std::__1::__compressed_pair.82" = type { %"class.std::__1::__libcpp_compressed_pair_imp.83" }
%"class.std::__1::__libcpp_compressed_pair_imp.83" = type { %struct.TypeSpecifier* }
%union.anon.86 = type { %struct.TypeSpecifier* }
%"struct.llvm::ilist_detail::SpecificNodeAccess" = type { i8 }
%"class.llvm::CastInst" = type { %"class.llvm::UnaryInstruction" }
%"class.llvm::IRBuilderDefaultInserter" = type { i8 }
%"class.llvm::SymbolTableListTraits" = type { i8 }
%"class.llvm::PointerUnion.87" = type { %"class.llvm::PointerIntPair.88" }
%"class.llvm::PointerIntPair.88" = type { i64 }
%"class.llvm::GetElementPtrInst" = type { %"class.llvm::Instruction", %"class.llvm::Type"*, %"class.llvm::Type"* }
%"class.llvm::Optional" = type { %"struct.llvm::AlignedCharArrayUnion.89", i8, [3 x i8] }
%"struct.llvm::AlignedCharArrayUnion.89" = type { %"struct.llvm::AlignedCharArray.90" }
%"struct.llvm::AlignedCharArray.90" = type { [4 x i8] }
%"class.llvm::VectorType" = type { %"class.llvm::SequentialType" }
%"class.llvm::SequentialType" = type { %"class.llvm::CompositeType", %"class.llvm::Type"*, i64 }
%"struct.std::__1::integral_constant" = type { i8 }
%"struct.std::__1::__has_construct" = type { i8 }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw" = type { [3 x i64] }
%"struct.std::__1::__murmur2_or_cityhash" = type { i8 }
%"struct.std::__1::pair.97" = type { i64, i64 }
%"class.std::__1::allocator.42" = type { i8 }
%"class.llvm::OperandBundleUser" = type { i8 }

@_ZN4llvm24DisableABIBreakingChecksE = external global i32, align 4
@_ZN4llvm30VerifyDisableABIBreakingChecksE = weak hidden global i32* @_ZN4llvm24DisableABIBreakingChecksE, align 8
@_ZN8CGMemory17CurrentEntryBlockE = global %"class.llvm::BasicBlock"* null, align 8
@_ZN14CodeGenVisitor6ModuleE = external global %"class.std::__1::unique_ptr.23", align 8
@.str = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@_ZN14CodeGenVisitor7BuilderE = external global %"class.llvm::IRBuilder", align 8
@_ZN14CodeGenVisitor9FunctionsE = external global %"class.std::__1::unordered_map", align 8
@.str.1 = private unnamed_addr constant [7 x i8] c"malloc\00", align 1
@.str.2 = private unnamed_addr constant [15 x i8] c"Invalid twine!\00", align 1
@__func__._ZN4llvm5TwineC2EPKc = private unnamed_addr constant [6 x i8] c"Twine\00", align 1
@.str.3 = private unnamed_addr constant [56 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/ADT/Twine.h\00", align 1
@.str.4 = private unnamed_addr constant [30 x i8] c"isValid() && \22Invalid twine!\22\00", align 1
@.str.5 = private unnamed_addr constant [29 x i8] c"isa<> used on a null pointer\00", align 1
@__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_ = private unnamed_addr constant [5 x i8] c"doit\00", align 1
@.str.6 = private unnamed_addr constant [62 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/Support/Casting.h\00", align 1
@.str.7 = private unnamed_addr constant [38 x i8] c"Val && \22isa<> used on a null pointer\22\00", align 1
@.str.8 = private unnamed_addr constant [42 x i8] c"cast<Ty>() argument of incompatible type!\00", align 1
@__func__._ZN4llvm4castINS_10StructTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_ = private unnamed_addr constant [5 x i8] c"cast\00", align 1
@.str.9 = private unnamed_addr constant [59 x i8] c"isa<X>(Val) && \22cast<Ty>() argument of incompatible type!\22\00", align 1
@.str.10 = private unnamed_addr constant [25 x i8] c"Expected values to match\00", align 1
@__func__._ZN4llvm13TrackingMDRef7retrackERS0_ = private unnamed_addr constant [8 x i8] c"retrack\00", align 1
@.str.11 = private unnamed_addr constant [63 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/IR/TrackingMDRef.h\00", align 1
@.str.12 = private unnamed_addr constant [41 x i8] c"MD == X.MD && \22Expected values to match\22\00", align 1
@.str.13 = private unnamed_addr constant [28 x i8] c"Integer too large for field\00", align 1
@__func__._ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE9updateIntEll = private unnamed_addr constant [10 x i8] c"updateInt\00", align 1
@.str.14 = private unnamed_addr constant [65 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/ADT/PointerIntPair.h\00", align 1
@.str.15 = private unnamed_addr constant [59 x i8] c"(IntWord & ~IntMask) == 0 && \22Integer too large for field\22\00", align 1
@.str.16 = private unnamed_addr constant [36 x i8] c"Pointer is not sufficiently aligned\00", align 1
@__func__._ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE13updatePointerElS1_ = private unnamed_addr constant [14 x i8] c"updatePointer\00", align 1
@.str.17 = private unnamed_addr constant [74 x i8] c"(PtrWord & ~PointerBitMask) == 0 && \22Pointer is not sufficiently aligned\22\00", align 1
@__func__._ZNKR4llvm8OptionalIjEdeEv = private unnamed_addr constant [10 x i8] c"operator*\00", align 1
@.str.18 = private unnamed_addr constant [59 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/ADT/Optional.h\00", align 1
@.str.19 = private unnamed_addr constant [7 x i8] c"hasVal\00", align 1
@__func__._ZNK4llvm8OptionalIjE10getPointerEv = private unnamed_addr constant [11 x i8] c"getPointer\00", align 1
@__func__._ZN4llvm17GetElementPtrInst6CreateEPNS_4TypeEPNS_5ValueENS_8ArrayRefIS4_EERKNS_5TwineEPNS_11InstructionE = private unnamed_addr constant [7 x i8] c"Create\00", align 1
@.str.20 = private unnamed_addr constant [62 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/IR/Instructions.h\00", align 1
@.str.21 = private unnamed_addr constant [84 x i8] c"PointeeType == cast<PointerType>(Ptr->getType()->getScalarType())->getElementType()\00", align 1
@_ZTVN4llvm17GetElementPtrInstE = external unnamed_addr constant [5 x i8*]
@__func__._ZN4llvm17GetElementPtrInstC2EPNS_4TypeEPNS_5ValueENS_8ArrayRefIS4_EEjRKNS_5TwineEPNS_11InstructionE = private unnamed_addr constant [18 x i8] c"GetElementPtrInst\00", align 1
@.str.22 = private unnamed_addr constant [85 x i8] c"ResultElementType == cast<PointerType>(getType()->getScalarType())->getElementType()\00", align 1
@.str.23 = private unnamed_addr constant [44 x i8] c"Invalid GetElementPtrInst indices for type!\00", align 1
@__func__._ZN4llvm12checkGEPTypeEPNS_4TypeE = private unnamed_addr constant [13 x i8] c"checkGEPType\00", align 1
@.str.24 = private unnamed_addr constant [52 x i8] c"Ty && \22Invalid GetElementPtrInst indices for type!\22\00", align 1
@.str.25 = private unnamed_addr constant [20 x i8] c"Constructor throws?\00", align 1
@.str.26 = private unnamed_addr constant [54 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/IR/User.h\00", align 1
@_ZNSt3__1L19piecewise_constructE = internal constant %"struct.std::__1::piecewise_construct_t" undef, align 1
@_ZTVN4llvm8CallInstE = external unnamed_addr constant [5 x i8*]

; Function Attrs: ssp uwtable
define %"class.llvm::Value"* @_ZN8CGMemory12CreateAllocaEPN4llvm4TypeENSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEPNS0_5ValueEtbPNS0_10BasicBlockE(%"class.llvm::Type"*, %"class.std::__1::basic_string"*, %"class.llvm::Value"*, i16 zeroext, i1 zeroext, %"class.llvm::BasicBlock"*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %7 = alloca %"class.std::__1::allocator"*, align 8
  %8 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %9 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca i8*, align 8
  %13 = alloca %"class.std::__1::basic_string"*, align 8
  %14 = alloca i8*, align 8
  %15 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.25"*, align 8
  %16 = alloca %"class.std::__1::__compressed_pair.24"*, align 8
  %17 = alloca %"class.std::__1::unique_ptr.23"*, align 8
  %18 = alloca %"class.llvm::Value"*, align 8
  %19 = alloca %"class.llvm::Type"*, align 8
  %20 = alloca %"class.llvm::Value"*, align 8
  %21 = alloca i16, align 2
  %22 = alloca i8, align 1
  %23 = alloca %"class.llvm::BasicBlock"*, align 8
  %24 = alloca %"class.llvm::GlobalVariable"*, align 8
  %25 = alloca i8*
  %26 = alloca i32
  %27 = alloca %"class.llvm::Twine", align 8
  %28 = alloca %"class.llvm::IRBuilder"*, align 8
  %29 = alloca %"class.llvm::StructType"*, align 8
  %30 = alloca %"class.llvm::Value"*, align 8
  %31 = alloca %"class.llvm::Twine", align 8
  %32 = alloca %"class.llvm::Twine", align 8
  %33 = alloca %"class.llvm::CallInst"*, align 8
  %34 = alloca %"class.std::__1::basic_string", align 8
  %35 = alloca %"class.llvm::ArrayRef.36", align 8
  %36 = alloca %"class.std::initializer_list", align 8
  %37 = alloca [1 x %"class.llvm::Value"*], align 8
  %38 = alloca %"class.llvm::Twine", align 8
  %39 = alloca %"class.llvm::Twine", align 8
  %40 = alloca %"class.llvm::BasicBlock"*, align 8
  %41 = alloca %"class.llvm::AllocaInst"*, align 8
  %42 = alloca %"class.llvm::Twine", align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %19, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %20, align 8
  store i16 %3, i16* %21, align 2
  %43 = zext i1 %4 to i8
  store i8 %43, i8* %22, align 1
  store %"class.llvm::BasicBlock"* %5, %"class.llvm::BasicBlock"** %23, align 8
  %44 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %23, align 8
  %45 = icmp eq %"class.llvm::BasicBlock"* %44, null
  br i1 %45, label %46, label %75

; <label>:46:                                     ; preds = %6
  %47 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** @_ZN8CGMemory17CurrentEntryBlockE, align 8
  %48 = icmp eq %"class.llvm::BasicBlock"* %47, null
  br i1 %48, label %49, label %73

; <label>:49:                                     ; preds = %46
  %50 = call i8* @_ZN4llvm14GlobalVariablenwEm(i64 88)
  %51 = bitcast i8* %50 to %"class.llvm::GlobalVariable"*
  store %"class.std::__1::unique_ptr.23"* @_ZN14CodeGenVisitor6ModuleE, %"class.std::__1::unique_ptr.23"** %17, align 8
  %52 = load %"class.std::__1::unique_ptr.23"*, %"class.std::__1::unique_ptr.23"** %17, align 8
  %53 = getelementptr inbounds %"class.std::__1::unique_ptr.23", %"class.std::__1::unique_ptr.23"* %52, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.24"* %53, %"class.std::__1::__compressed_pair.24"** %16, align 8
  %54 = load %"class.std::__1::__compressed_pair.24"*, %"class.std::__1::__compressed_pair.24"** %16, align 8
  %55 = bitcast %"class.std::__1::__compressed_pair.24"* %54 to %"class.std::__1::__libcpp_compressed_pair_imp.25"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.25"* %55, %"class.std::__1::__libcpp_compressed_pair_imp.25"** %15, align 8
  %56 = load %"class.std::__1::__libcpp_compressed_pair_imp.25"*, %"class.std::__1::__libcpp_compressed_pair_imp.25"** %15, align 8
  %57 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.25", %"class.std::__1::__libcpp_compressed_pair_imp.25"* %56, i32 0, i32 0
  %58 = load %"class.llvm::Module"*, %"class.llvm::Module"** %57, align 8
  br label %59

; <label>:59:                                     ; preds = %49
  %60 = load %"class.llvm::Type"*, %"class.llvm::Type"** %19, align 8
  %61 = load %"class.llvm::Type"*, %"class.llvm::Type"** %19, align 8
  %62 = invoke %"class.llvm::ConstantAggregateZero"* @_ZN4llvm21ConstantAggregateZero3getEPNS_4TypeE(%"class.llvm::Type"* %61)
          to label %63 unwind label %69

; <label>:63:                                     ; preds = %59
  %64 = bitcast %"class.llvm::ConstantAggregateZero"* %62 to %"class.llvm::Constant"*
  invoke void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %27, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
          to label %65 unwind label %69

; <label>:65:                                     ; preds = %63
  invoke void @_ZN4llvm14GlobalVariableC1ERNS_6ModuleEPNS_4TypeEbNS_11GlobalValue12LinkageTypesEPNS_8ConstantERKNS_5TwineEPS0_NS5_15ThreadLocalModeEjb(%"class.llvm::GlobalVariable"* %51, %"class.llvm::Module"* nonnull %58, %"class.llvm::Type"* %60, i1 zeroext false, i32 0, %"class.llvm::Constant"* %64, %"class.llvm::Twine"* dereferenceable(24) %27, %"class.llvm::GlobalVariable"* null, i32 0, i32 0, i1 zeroext false)
          to label %66 unwind label %69

; <label>:66:                                     ; preds = %65
  store %"class.llvm::GlobalVariable"* %51, %"class.llvm::GlobalVariable"** %24, align 8
  %67 = load %"class.llvm::GlobalVariable"*, %"class.llvm::GlobalVariable"** %24, align 8
  %68 = bitcast %"class.llvm::GlobalVariable"* %67 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %68, %"class.llvm::Value"** %18, align 8
  br label %155

; <label>:69:                                     ; preds = %65, %63, %59
  %70 = landingpad { i8*, i32 }
          cleanup
  %71 = extractvalue { i8*, i32 } %70, 0
  store i8* %71, i8** %25, align 8
  %72 = extractvalue { i8*, i32 } %70, 1
  store i32 %72, i32* %26, align 4
  call void @_ZN4llvm4UserdlEPv(i8* %50) #14
  br label %157

; <label>:73:                                     ; preds = %46
  %74 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** @_ZN8CGMemory17CurrentEntryBlockE, align 8
  store %"class.llvm::BasicBlock"* %74, %"class.llvm::BasicBlock"** %23, align 8
  br label %75

; <label>:75:                                     ; preds = %73, %6
  store %"class.llvm::IRBuilder"* @_ZN14CodeGenVisitor7BuilderE, %"class.llvm::IRBuilder"** %28, align 8
  %76 = load i8, i8* %22, align 1
  %77 = trunc i8 %76 to i1
  br i1 %77, label %78, label %140

; <label>:78:                                     ; preds = %75
  %79 = call zeroext i1 @_ZN4llvm3isaINS_10StructTypeEPNS_4TypeEEEbRKT0_(%"class.llvm::Type"** dereferenceable(8) %19)
  br i1 %79, label %80, label %140

; <label>:80:                                     ; preds = %78
  %81 = load %"class.llvm::Type"*, %"class.llvm::Type"** %19, align 8
  %82 = call %"class.llvm::StructType"* @_ZN4llvm4castINS_10StructTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Type"* %81)
  store %"class.llvm::StructType"* %82, %"class.llvm::StructType"** %29, align 8
  %83 = load %"class.llvm::StructType"*, %"class.llvm::StructType"** %29, align 8
  %84 = bitcast %"class.llvm::StructType"* %83 to %"class.llvm::Type"*
  %85 = call %"class.llvm::PointerType"* @_ZNK4llvm4Type12getPointerToEj(%"class.llvm::Type"* %84, i32 0)
  %86 = call %"class.llvm::ConstantPointerNull"* @_ZN4llvm19ConstantPointerNull3getEPNS_11PointerTypeE(%"class.llvm::PointerType"* %85)
  %87 = bitcast %"class.llvm::ConstantPointerNull"* %86 to %"class.llvm::Value"*
  %88 = call %"class.llvm::IntegerType"* @_ZN4llvm13IRBuilderBase10getInt64TyEv(%"class.llvm::IRBuilderBase"* getelementptr inbounds (%"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* @_ZN14CodeGenVisitor7BuilderE, i32 0, i32 0))
  %89 = call %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt3getEPNS_11IntegerTypeEyb(%"class.llvm::IntegerType"* %88, i64 1, i1 zeroext false)
  %90 = bitcast %"class.llvm::ConstantInt"* %89 to %"class.llvm::Value"*
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %31, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %91 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE9CreateGEPEPNS_5ValueES5_RKNS_5TwineE(%"class.llvm::IRBuilder"* @_ZN14CodeGenVisitor7BuilderE, %"class.llvm::Value"* %87, %"class.llvm::Value"* %90, %"class.llvm::Twine"* dereferenceable(24) %31)
  %92 = call %"class.llvm::IntegerType"* @_ZN4llvm13IRBuilderBase10getInt64TyEv(%"class.llvm::IRBuilderBase"* getelementptr inbounds (%"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* @_ZN14CodeGenVisitor7BuilderE, i32 0, i32 0))
  %93 = bitcast %"class.llvm::IntegerType"* %92 to %"class.llvm::Type"*
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %32, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %94 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE14CreatePtrToIntEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* @_ZN14CodeGenVisitor7BuilderE, %"class.llvm::Value"* %91, %"class.llvm::Type"* %93, %"class.llvm::Twine"* dereferenceable(24) %32)
  store %"class.llvm::Value"* %94, %"class.llvm::Value"** %30, align 8
  store %"class.std::__1::basic_string"* %34, %"class.std::__1::basic_string"** %13, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i8** %14, align 8
  %95 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8
  %96 = load i8*, i8** %14, align 8
  store %"class.std::__1::basic_string"* %95, %"class.std::__1::basic_string"** %11, align 8
  store i8* %96, i8** %12, align 8
  %97 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8
  %98 = bitcast %"class.std::__1::basic_string"* %97 to %"class.std::__1::__basic_string_common"*
  %99 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %97, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %99, %"class.std::__1::__compressed_pair.18"** %10, align 8
  %100 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %10, align 8
  store %"class.std::__1::__compressed_pair.18"* %100, %"class.std::__1::__compressed_pair.18"** %9, align 8
  %101 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %9, align 8
  %102 = bitcast %"class.std::__1::__compressed_pair.18"* %101 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %102, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %8, align 8
  %103 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %8, align 8
  %104 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.19"* %103 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %104, %"class.std::__1::allocator"** %7, align 8
  %105 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %7, align 8
  %106 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %103, i32 0, i32 0
  %107 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %106 to i8*
  call void @llvm.memset.p0i8.i64(i8* %107, i8 0, i64 24, i32 8, i1 false) #14
  %108 = load i8*, i8** %12, align 8
  %109 = load i8*, i8** %12, align 8
  %110 = call i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %109)
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %97, i8* %108, i64 %110)
  %111 = invoke dereferenceable(8) %"class.llvm::Constant"** @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantENS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S9_EEEEEixEOS6_(%"class.std::__1::unordered_map"* @_ZN14CodeGenVisitor9FunctionsE, %"class.std::__1::basic_string"* dereferenceable(24) %34)
          to label %112 unwind label %136

; <label>:112:                                    ; preds = %80
  %113 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %111, align 8
  %114 = bitcast %"class.llvm::Constant"* %113 to %"class.llvm::Value"*
  %115 = getelementptr inbounds [1 x %"class.llvm::Value"*], [1 x %"class.llvm::Value"*]* %37, i64 0, i64 0
  %116 = load %"class.llvm::Value"*, %"class.llvm::Value"** %30, align 8
  store %"class.llvm::Value"* %116, %"class.llvm::Value"** %115, align 8
  %117 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %36, i32 0, i32 0
  %118 = getelementptr inbounds [1 x %"class.llvm::Value"*], [1 x %"class.llvm::Value"*]* %37, i64 0, i64 0
  store %"class.llvm::Value"** %118, %"class.llvm::Value"*** %117, align 8
  %119 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %36, i32 0, i32 1
  store i64 1, i64* %119, align 8
  invoke void @_ZN4llvm8ArrayRefIPNS_5ValueEEC1ERKSt16initializer_listIS2_E(%"class.llvm::ArrayRef.36"* %35, %"class.std::initializer_list"* dereferenceable(16) %36)
          to label %120 unwind label %136

; <label>:120:                                    ; preds = %112
  invoke void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %38, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
          to label %121 unwind label %136

; <label>:121:                                    ; preds = %120
  %122 = bitcast %"class.llvm::ArrayRef.36"* %35 to { %"class.llvm::Value"**, i64 }*
  %123 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %122, i32 0, i32 0
  %124 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %123, align 8
  %125 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %122, i32 0, i32 1
  %126 = load i64, i64* %125, align 8
  %127 = invoke %"class.llvm::CallInst"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCallEPNS_5ValueENS_8ArrayRefIS5_EERKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"* @_ZN14CodeGenVisitor7BuilderE, %"class.llvm::Value"* %114, %"class.llvm::Value"** %124, i64 %126, %"class.llvm::Twine"* dereferenceable(24) %38, %"class.llvm::MDNode"* null)
          to label %128 unwind label %136

; <label>:128:                                    ; preds = %121
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %34) #14
  store %"class.llvm::CallInst"* %127, %"class.llvm::CallInst"** %33, align 8
  %129 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %33, align 8
  %130 = bitcast %"class.llvm::CallInst"* %129 to %"class.llvm::Value"*
  %131 = load %"class.llvm::StructType"*, %"class.llvm::StructType"** %29, align 8
  %132 = bitcast %"class.llvm::StructType"* %131 to %"class.llvm::Type"*
  %133 = call %"class.llvm::PointerType"* @_ZNK4llvm4Type12getPointerToEj(%"class.llvm::Type"* %132, i32 0)
  %134 = bitcast %"class.llvm::PointerType"* %133 to %"class.llvm::Type"*
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %39, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %135 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE13CreateBitCastEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* @_ZN14CodeGenVisitor7BuilderE, %"class.llvm::Value"* %130, %"class.llvm::Type"* %134, %"class.llvm::Twine"* dereferenceable(24) %39)
  store %"class.llvm::Value"* %135, %"class.llvm::Value"** %18, align 8
  br label %155

; <label>:136:                                    ; preds = %121, %120, %112, %80
  %137 = landingpad { i8*, i32 }
          cleanup
  %138 = extractvalue { i8*, i32 } %137, 0
  store i8* %138, i8** %25, align 8
  %139 = extractvalue { i8*, i32 } %137, 1
  store i32 %139, i32* %26, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %34) #14
  br label %157

; <label>:140:                                    ; preds = %78, %75
  %141 = call %"class.llvm::BasicBlock"* @_ZNK4llvm13IRBuilderBase14GetInsertBlockEv(%"class.llvm::IRBuilderBase"* getelementptr inbounds (%"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* @_ZN14CodeGenVisitor7BuilderE, i32 0, i32 0))
  store %"class.llvm::BasicBlock"* %141, %"class.llvm::BasicBlock"** %40, align 8
  %142 = load %"class.llvm::Value"*, %"class.llvm::Value"** %20, align 8
  %143 = icmp eq %"class.llvm::Value"* %142, null
  br i1 %143, label %146, label %144

; <label>:144:                                    ; preds = %140
  %145 = call zeroext i1 @_ZN4llvm3isaINS_8ConstantEPNS_5ValueEEEbRKT0_(%"class.llvm::Value"** dereferenceable(8) %20)
  br i1 %145, label %146, label %148

; <label>:146:                                    ; preds = %144, %140
  %147 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %23, align 8
  call void @_ZN4llvm13IRBuilderBase14SetInsertPointEPNS_10BasicBlockE(%"class.llvm::IRBuilderBase"* getelementptr inbounds (%"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* @_ZN14CodeGenVisitor7BuilderE, i32 0, i32 0), %"class.llvm::BasicBlock"* %147)
  br label %148

; <label>:148:                                    ; preds = %146, %144
  %149 = load %"class.llvm::Type"*, %"class.llvm::Type"** %19, align 8
  %150 = load %"class.llvm::Value"*, %"class.llvm::Value"** %20, align 8
  call void @_ZN4llvm5TwineC1ERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE(%"class.llvm::Twine"* %42, %"class.std::__1::basic_string"* dereferenceable(24) %1)
  %151 = call %"class.llvm::AllocaInst"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE12CreateAllocaEPNS_4TypeEPNS_5ValueERKNS_5TwineE(%"class.llvm::IRBuilder"* @_ZN14CodeGenVisitor7BuilderE, %"class.llvm::Type"* %149, %"class.llvm::Value"* %150, %"class.llvm::Twine"* dereferenceable(24) %42)
  store %"class.llvm::AllocaInst"* %151, %"class.llvm::AllocaInst"** %41, align 8
  %152 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %40, align 8
  call void @_ZN4llvm13IRBuilderBase14SetInsertPointEPNS_10BasicBlockE(%"class.llvm::IRBuilderBase"* getelementptr inbounds (%"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* @_ZN14CodeGenVisitor7BuilderE, i32 0, i32 0), %"class.llvm::BasicBlock"* %152)
  %153 = load %"class.llvm::AllocaInst"*, %"class.llvm::AllocaInst"** %41, align 8
  %154 = bitcast %"class.llvm::AllocaInst"* %153 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %154, %"class.llvm::Value"** %18, align 8
  br label %155

; <label>:155:                                    ; preds = %148, %128, %66
  %156 = load %"class.llvm::Value"*, %"class.llvm::Value"** %18, align 8
  ret %"class.llvm::Value"* %156

; <label>:157:                                    ; preds = %136, %69
  %158 = load i8*, i8** %25, align 8
  %159 = load i32, i32* %26, align 4
  %160 = insertvalue { i8*, i32 } undef, i8* %158, 0
  %161 = insertvalue { i8*, i32 } %160, i32 %159, 1
  resume { i8*, i32 } %161
}

; Function Attrs: ssp uwtable
define linkonce_odr i8* @_ZN4llvm14GlobalVariablenwEm(i64) #0 align 2 {
  %2 = alloca i64, align 8
  store i64 %0, i64* %2, align 8
  %3 = load i64, i64* %2, align 8
  %4 = call i8* @_ZN4llvm4UsernwEmj(i64 %3, i32 1)
  ret i8* %4
}

declare i32 @__gxx_personality_v0(...)

declare %"class.llvm::ConstantAggregateZero"* @_ZN4llvm21ConstantAggregateZero3getEPNS_4TypeE(%"class.llvm::Type"*) #1

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

declare void @_ZN4llvm14GlobalVariableC1ERNS_6ModuleEPNS_4TypeEbNS_11GlobalValue12LinkageTypesEPNS_8ConstantERKNS_5TwineEPS0_NS5_15ThreadLocalModeEjb(%"class.llvm::GlobalVariable"*, %"class.llvm::Module"* nonnull, %"class.llvm::Type"*, i1 zeroext, i32, %"class.llvm::Constant"*, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::GlobalVariable"*, i32, i32, i1 zeroext) unnamed_addr #1

; Function Attrs: nounwind
declare void @_ZN4llvm4UserdlEPv(i8*) #2

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm3isaINS_10StructTypeEPNS_4TypeEEEbRKT0_(%"class.llvm::Type"** dereferenceable(8)) #3 {
  %2 = alloca %"class.llvm::Type"**, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %3 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_10StructTypeEKPNS_4TypeEPKS2_E4doitERS4_(%"class.llvm::Type"** dereferenceable(8) %3)
  ret i1 %4
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr %"class.llvm::StructType"* @_ZN4llvm4castINS_10StructTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Type"*) #3 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = call zeroext i1 @_ZN4llvm3isaINS_10StructTypeEPNS_4TypeEEEbRKT0_(%"class.llvm::Type"** dereferenceable(8) %2)
  br i1 %3, label %4, label %5

; <label>:4:                                      ; preds = %1
  br label %5

; <label>:5:                                      ; preds = %4, %1
  %6 = phi i1 [ false, %1 ], [ true, %4 ]
  %7 = xor i1 %6, true
  br i1 %7, label %8, label %10

; <label>:8:                                      ; preds = %5
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm4castINS_10StructTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 236, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.9, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %11

; <label>:10:                                     ; preds = %5
  br label %11

; <label>:11:                                     ; preds = %10, %9
  %12 = call %"class.llvm::StructType"* @_ZN4llvm16cast_convert_valINS_10StructTypeEPNS_4TypeES3_E4doitERKS3_(%"class.llvm::Type"** dereferenceable(8) %2)
  ret %"class.llvm::StructType"* %12
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE14CreatePtrToIntEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Type"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %5 = alloca %"class.llvm::IRBuilder"*, align 8
  %6 = alloca %"class.llvm::Value"*, align 8
  %7 = alloca %"class.llvm::Type"*, align 8
  %8 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %5, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %6, align 8
  store %"class.llvm::Type"* %2, %"class.llvm::Type"** %7, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %8, align 8
  %9 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %5, align 8
  %10 = load %"class.llvm::Value"*, %"class.llvm::Value"** %6, align 8
  %11 = load %"class.llvm::Type"*, %"class.llvm::Type"** %7, align 8
  %12 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %8, align 8
  %13 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCastENS_11Instruction7CastOpsEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %9, i32 45, %"class.llvm::Value"* %10, %"class.llvm::Type"* %11, %"class.llvm::Twine"* dereferenceable(24) %12)
  ret %"class.llvm::Value"* %13
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE9CreateGEPEPNS_5ValueES5_RKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
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
  %13 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE9CreateGEPEPNS_4TypeEPNS_5ValueES7_RKNS_5TwineE(%"class.llvm::IRBuilder"* %9, %"class.llvm::Type"* null, %"class.llvm::Value"* %10, %"class.llvm::Value"* %11, %"class.llvm::Twine"* dereferenceable(24) %12)
  ret %"class.llvm::Value"* %13
}

declare %"class.llvm::ConstantPointerNull"* @_ZN4llvm19ConstantPointerNull3getEPNS_11PointerTypeE(%"class.llvm::PointerType"*) #1

declare %"class.llvm::PointerType"* @_ZNK4llvm4Type12getPointerToEj(%"class.llvm::Type"*, i32) #1

declare %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt3getEPNS_11IntegerTypeEyb(%"class.llvm::IntegerType"*, i64, i1 zeroext) #1

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::IntegerType"* @_ZN4llvm13IRBuilderBase10getInt64TyEv(%"class.llvm::IRBuilderBase"*) #0 align 2 {
  %2 = alloca %"class.llvm::IRBuilderBase"*, align 8
  store %"class.llvm::IRBuilderBase"* %0, %"class.llvm::IRBuilderBase"** %2, align 8
  %3 = load %"class.llvm::IRBuilderBase"*, %"class.llvm::IRBuilderBase"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %3, i32 0, i32 3
  %5 = load %"class.llvm::LLVMContext"*, %"class.llvm::LLVMContext"** %4, align 8
  %6 = call %"class.llvm::IntegerType"* @_ZN4llvm4Type10getInt64TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8) %5)
  ret %"class.llvm::IntegerType"* %6
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::CallInst"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCallEPNS_5ValueENS_8ArrayRefIS5_EERKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::MDNode"*) #0 align 2 {
  %7 = alloca %"class.llvm::ArrayRef.36", align 8
  %8 = alloca %"class.llvm::IRBuilder"*, align 8
  %9 = alloca %"class.llvm::Value"*, align 8
  %10 = alloca %"class.llvm::Twine"*, align 8
  %11 = alloca %"class.llvm::MDNode"*, align 8
  %12 = alloca %"class.llvm::PointerType"*, align 8
  %13 = alloca %"class.llvm::FunctionType"*, align 8
  %14 = alloca %"class.llvm::ArrayRef.36", align 8
  %15 = bitcast %"class.llvm::ArrayRef.36"* %7 to { %"class.llvm::Value"**, i64 }*
  %16 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %15, i32 0, i32 0
  store %"class.llvm::Value"** %2, %"class.llvm::Value"*** %16, align 8
  %17 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %15, i32 0, i32 1
  store i64 %3, i64* %17, align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %8, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %9, align 8
  store %"class.llvm::Twine"* %4, %"class.llvm::Twine"** %10, align 8
  store %"class.llvm::MDNode"* %5, %"class.llvm::MDNode"** %11, align 8
  %18 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %8, align 8
  %19 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %20 = call %"class.llvm::Type"* @_ZNK4llvm5Value7getTypeEv(%"class.llvm::Value"* %19)
  %21 = call %"class.llvm::PointerType"* @_ZN4llvm4castINS_11PointerTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Type"* %20)
  store %"class.llvm::PointerType"* %21, %"class.llvm::PointerType"** %12, align 8
  %22 = load %"class.llvm::PointerType"*, %"class.llvm::PointerType"** %12, align 8
  %23 = call %"class.llvm::Type"* @_ZNK4llvm11PointerType14getElementTypeEv(%"class.llvm::PointerType"* %22)
  %24 = call %"class.llvm::FunctionType"* @_ZN4llvm4castINS_12FunctionTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Type"* %23)
  store %"class.llvm::FunctionType"* %24, %"class.llvm::FunctionType"** %13, align 8
  %25 = load %"class.llvm::FunctionType"*, %"class.llvm::FunctionType"** %13, align 8
  %26 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %27 = bitcast %"class.llvm::ArrayRef.36"* %14 to i8*
  %28 = bitcast %"class.llvm::ArrayRef.36"* %7 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %27, i8* %28, i64 16, i32 8, i1 false)
  %29 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %30 = load %"class.llvm::MDNode"*, %"class.llvm::MDNode"** %11, align 8
  %31 = bitcast %"class.llvm::ArrayRef.36"* %14 to { %"class.llvm::Value"**, i64 }*
  %32 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %31, i32 0, i32 0
  %33 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %32, align 8
  %34 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %31, i32 0, i32 1
  %35 = load i64, i64* %34, align 8
  %36 = call %"class.llvm::CallInst"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCallEPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS7_EERKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"* %18, %"class.llvm::FunctionType"* %25, %"class.llvm::Value"* %26, %"class.llvm::Value"** %33, i64 %35, %"class.llvm::Twine"* dereferenceable(24) %29, %"class.llvm::MDNode"* %30)
  ret %"class.llvm::CallInst"* %36
}

; Function Attrs: ssp uwtable
define linkonce_odr dereferenceable(8) %"class.llvm::Constant"** @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantENS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S9_EEEEEixEOS6_(%"class.std::__1::unordered_map"*, %"class.std::__1::basic_string"* dereferenceable(24)) #0 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %3 = alloca %"class.std::__1::tuple.91"*, align 8
  %4 = alloca %"class.std::__1::tuple.91"*, align 8
  %5 = alloca %"class.std::__1::tuple.91", align 1
  %6 = alloca %"class.std::__1::basic_string"*, align 8
  %7 = alloca %"class.std::__1::__tuple_leaf"*, align 8
  %8 = alloca %"class.std::__1::basic_string"*, align 8
  %9 = alloca %"class.std::__1::basic_string"*, align 8
  %10 = alloca %"struct.std::__1::__tuple_indices", align 1
  %11 = alloca %"struct.std::__1::__tuple_types", align 1
  %12 = alloca %"struct.std::__1::__tuple_indices.98", align 1
  %13 = alloca %"struct.std::__1::__tuple_types.100", align 1
  %14 = alloca %"struct.std::__1::__tuple_impl"*, align 8
  %15 = alloca %"class.std::__1::basic_string"*, align 8
  %16 = alloca %"struct.std::__1::__tuple_indices", align 1
  %17 = alloca %"struct.std::__1::__tuple_types", align 1
  %18 = alloca %"struct.std::__1::__tuple_indices.98", align 1
  %19 = alloca %"struct.std::__1::__tuple_types.100", align 1
  %20 = alloca %"struct.std::__1::__tuple_impl"*, align 8
  %21 = alloca %"class.std::__1::basic_string"*, align 8
  %22 = alloca %"class.std::__1::basic_string"*, align 8
  %23 = alloca %"class.std::__1::tuple"*, align 8
  %24 = alloca %"class.std::__1::basic_string"*, align 8
  %25 = alloca %"struct.std::__1::__tuple_indices", align 1
  %26 = alloca %"struct.std::__1::__tuple_types", align 1
  %27 = alloca %"struct.std::__1::__tuple_indices.98", align 1
  %28 = alloca %"struct.std::__1::__tuple_types.100", align 1
  %29 = alloca %"class.std::__1::tuple"*, align 8
  %30 = alloca %"class.std::__1::basic_string"*, align 8
  %31 = alloca %"class.std::__1::basic_string"*, align 8
  %32 = alloca %"class.std::__1::tuple", align 8
  %33 = alloca %"class.std::__1::basic_string"*, align 8
  %34 = alloca i8*, align 8
  %35 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %36 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %37 = alloca %"class.std::__1::basic_string"*, align 8
  %38 = alloca i8*, align 8
  %39 = alloca i8*, align 8
  %40 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %41 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %42 = alloca %"class.std::__1::basic_string"*, align 8
  %43 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %44 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %45 = alloca %"class.std::__1::basic_string"*, align 8
  %46 = alloca %"class.std::__1::basic_string"*, align 8
  %47 = alloca %"class.std::__1::basic_string"*, align 8
  %48 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %49 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %50 = alloca %"class.std::__1::basic_string"*, align 8
  %51 = alloca i8*, align 8
  %52 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %53 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %54 = alloca %"class.std::__1::basic_string"*, align 8
  %55 = alloca i8*, align 8
  %56 = alloca i8*, align 8
  %57 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %58 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %59 = alloca %"class.std::__1::basic_string"*, align 8
  %60 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %61 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %62 = alloca %"class.std::__1::basic_string"*, align 8
  %63 = alloca %"class.std::__1::basic_string"*, align 8
  %64 = alloca %"class.std::__1::basic_string"*, align 8
  %65 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %66 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %67 = alloca %"class.std::__1::basic_string"*, align 8
  %68 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %69 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %70 = alloca %"class.std::__1::basic_string"*, align 8
  %71 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %72 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %73 = alloca %"class.std::__1::basic_string"*, align 8
  %74 = alloca %"class.std::__1::basic_string"*, align 8
  %75 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %76 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %77 = alloca %"class.std::__1::basic_string"*, align 8
  %78 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %79 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %80 = alloca %"class.std::__1::basic_string"*, align 8
  %81 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %82 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %83 = alloca %"class.std::__1::basic_string"*, align 8
  %84 = alloca %"class.std::__1::basic_string"*, align 8
  %85 = alloca i1, align 1
  %86 = alloca %"class.std::__1::basic_string"*, align 8
  %87 = alloca %"class.std::__1::basic_string"*, align 8
  %88 = alloca i64, align 8
  %89 = alloca i8*, align 8
  %90 = alloca i8*, align 8
  %91 = alloca %"struct.std::__1::equal_to"*, align 8
  %92 = alloca %"class.std::__1::basic_string"*, align 8
  %93 = alloca %"class.std::__1::basic_string"*, align 8
  %94 = alloca %"class.std::__1::__unordered_map_equal"*, align 8
  %95 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %96 = alloca %"class.std::__1::basic_string"*, align 8
  %97 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %98 = alloca %"class.std::__1::tuple"*, align 8
  %99 = alloca %"class.std::__1::tuple.91"*, align 8
  %100 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %101 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %102 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %103 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.49"*, align 8
  %104 = alloca %"class.std::__1::__compressed_pair.48"*, align 8
  %105 = alloca %"class.std::__1::__hash_table"*, align 8
  %106 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.51"*, align 8
  %107 = alloca %"class.std::__1::__compressed_pair.50"*, align 8
  %108 = alloca %"class.std::__1::__hash_table"*, align 8
  %109 = alloca i64, align 8
  %110 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.49"*, align 8
  %111 = alloca %"class.std::__1::__compressed_pair.48"*, align 8
  %112 = alloca %"class.std::__1::__hash_table"*, align 8
  %113 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.51"*, align 8
  %114 = alloca %"class.std::__1::__compressed_pair.50"*, align 8
  %115 = alloca %"class.std::__1::__hash_table"*, align 8
  %116 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.51"*, align 8
  %117 = alloca %"class.std::__1::__compressed_pair.50"*, align 8
  %118 = alloca %"class.std::__1::__hash_table"*, align 8
  %119 = alloca float, align 4
  %120 = alloca %"struct.std::__1::__less"*, align 8
  %121 = alloca i64*, align 8
  %122 = alloca i64*, align 8
  %123 = alloca %"struct.std::__1::__less", align 1
  %124 = alloca i64*, align 8
  %125 = alloca i64*, align 8
  %126 = alloca i64*, align 8
  %127 = alloca i64*, align 8
  %128 = alloca %"struct.std::__1::__less", align 1
  %129 = alloca i64, align 8
  %130 = alloca i64, align 8
  %131 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.41"*, align 8
  %132 = alloca %"class.std::__1::__compressed_pair.40"*, align 8
  %133 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %134 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %135 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %136 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %137 = alloca %"class.std::__1::__hash_table"*, align 8
  %138 = alloca i64, align 8
  %139 = alloca i64, align 8
  %140 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %141 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %142 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %143 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %144 = alloca i64, align 8
  %145 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %146 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %147 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %148 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %149 = alloca i8*, align 8
  %150 = alloca %"class.std::__1::allocator.46"*, align 8
  %151 = alloca %"struct.std::__1::__hash_node"*, align 8
  %152 = alloca i64, align 8
  %153 = alloca %"class.std::__1::allocator.46"*, align 8
  %154 = alloca %"struct.std::__1::__hash_node"*, align 8
  %155 = alloca i64, align 8
  %156 = alloca %"class.std::__1::allocator.46"*, align 8
  %157 = alloca %"struct.std::__1::pair.92"*, align 8
  %158 = alloca %"class.std::__1::allocator.46"*, align 8
  %159 = alloca %"struct.std::__1::pair.92"*, align 8
  %160 = alloca %"struct.std::__1::integral_constant.99", align 1
  %161 = alloca %"struct.std::__1::__has_destroy", align 1
  %162 = alloca %"struct.std::__1::pair.92"*, align 8
  %163 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %164 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %165 = alloca %"struct.std::__1::__hash_node"*, align 8
  %166 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %167 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %168 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %169 = alloca %"struct.std::__1::__hash_node"*, align 8
  %170 = alloca %"struct.std::__1::__hash_node"*, align 8
  %171 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %172 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %173 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %174 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %175 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %176 = alloca i64, align 8
  %177 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.45"*, align 8
  %178 = alloca %"class.std::__1::__compressed_pair.44"*, align 8
  %179 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %180 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %181 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %182 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %183 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %184 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %185 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %186 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %187 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %188 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %189 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %190 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %191 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %192 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %193 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %194 = alloca i64, align 8
  %195 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %196 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %197 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %198 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %199 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %200 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %201 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %202 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %203 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %204 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %205 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %206 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %207 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %208 = alloca i64, align 8
  %209 = alloca i64, align 8
  %210 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %211 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %212 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %213 = alloca i64, align 8
  %214 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %215 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %216 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %217 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %218 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %219 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %220 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %221 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %222 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %223 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %224 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %225 = alloca %"struct.std::__1::__hash_node"*, align 8
  %226 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.49"*, align 8
  %227 = alloca %"class.std::__1::__compressed_pair.48"*, align 8
  %228 = alloca %"class.std::__1::__hash_table"*, align 8
  %229 = alloca i64, align 8
  %230 = alloca i64, align 8
  %231 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.41"*, align 8
  %232 = alloca %"class.std::__1::__compressed_pair.40"*, align 8
  %233 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %234 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %235 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %236 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %237 = alloca %"class.std::__1::__hash_table"*, align 8
  %238 = alloca %"class.std::__1::__unordered_map_hasher"*, align 8
  %239 = alloca %"class.std::__1::basic_string"*, align 8
  %240 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %241 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %242 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %243 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %244 = alloca i8*, align 8
  %245 = alloca %"class.std::__1::allocator.46"*, align 8
  %246 = alloca %"struct.std::__1::__hash_node"*, align 8
  %247 = alloca i64, align 8
  %248 = alloca %"class.std::__1::allocator.46"*, align 8
  %249 = alloca %"struct.std::__1::__hash_node"*, align 8
  %250 = alloca i64, align 8
  %251 = alloca %"class.std::__1::allocator.46"*, align 8
  %252 = alloca %"struct.std::__1::pair.92"*, align 8
  %253 = alloca %"class.std::__1::allocator.46"*, align 8
  %254 = alloca %"struct.std::__1::pair.92"*, align 8
  %255 = alloca %"struct.std::__1::integral_constant.99", align 1
  %256 = alloca %"struct.std::__1::__has_destroy", align 1
  %257 = alloca %"struct.std::__1::pair.92"*, align 8
  %258 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %259 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %260 = alloca %"struct.std::__1::__hash_node"*, align 8
  %261 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %262 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %263 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %264 = alloca %"struct.std::__1::__hash_node"*, align 8
  %265 = alloca %"struct.std::__1::__hash_node"*, align 8
  %266 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %267 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %268 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %269 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %270 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %271 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %272 = alloca i8*, align 8
  %273 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %274 = alloca %"struct.std::__1::pair"*, align 8
  %275 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %276 = alloca i8*, align 8
  %277 = alloca %"struct.std::__1::pair"*, align 8
  %278 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %279 = alloca i8*, align 8
  %280 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.49"*, align 8
  %281 = alloca %"class.std::__1::__compressed_pair.48"*, align 8
  %282 = alloca %"class.std::__1::__hash_table"*, align 8
  %283 = alloca %"struct.std::__1::pair", align 8
  %284 = alloca %"class.std::__1::__hash_table"*, align 8
  %285 = alloca %"class.std::__1::basic_string"*, align 8
  %286 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %287 = alloca %"class.std::__1::tuple"*, align 8
  %288 = alloca %"class.std::__1::tuple.91"*, align 8
  %289 = alloca i64, align 8
  %290 = alloca i64, align 8
  %291 = alloca i8, align 1
  %292 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %293 = alloca i64, align 8
  %294 = alloca %"class.std::__1::unique_ptr.94", align 8
  %295 = alloca i64, align 8
  %296 = alloca i8*
  %297 = alloca i32
  %298 = alloca i64, align 8
  %299 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %300 = alloca %"class.std::__1::__hash_iterator", align 8
  %301 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %302 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %303 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %304 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %305 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %306 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %307 = alloca %"class.std::__1::basic_string"*, align 8
  %308 = alloca %"class.std::__1::unordered_map"*, align 8
  %309 = alloca %"class.std::__1::basic_string"*, align 8
  %310 = alloca %"class.std::__1::tuple", align 8
  %311 = alloca %"class.std::__1::tuple.91", align 1
  %312 = alloca %"class.std::__1::tuple.91", align 1
  %313 = alloca %"struct.std::__1::pair", align 8
  store %"class.std::__1::unordered_map"* %0, %"class.std::__1::unordered_map"** %308, align 8
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %309, align 8
  %314 = load %"class.std::__1::unordered_map"*, %"class.std::__1::unordered_map"** %308, align 8
  %315 = getelementptr inbounds %"class.std::__1::unordered_map", %"class.std::__1::unordered_map"* %314, i32 0, i32 0
  %316 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %309, align 8
  %317 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %309, align 8
  store %"class.std::__1::basic_string"* %317, %"class.std::__1::basic_string"** %307, align 8
  %318 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %307, align 8
  store %"class.std::__1::basic_string"* %318, %"class.std::__1::basic_string"** %33, align 8
  %319 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8
  store %"class.std::__1::basic_string"* %319, %"class.std::__1::basic_string"** %31, align 8
  %320 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %31, align 8
  store %"class.std::__1::tuple"* %32, %"class.std::__1::tuple"** %29, align 8
  store %"class.std::__1::basic_string"* %320, %"class.std::__1::basic_string"** %30, align 8
  %321 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %29, align 8
  %322 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %30, align 8
  store %"class.std::__1::tuple"* %321, %"class.std::__1::tuple"** %23, align 8
  store %"class.std::__1::basic_string"* %322, %"class.std::__1::basic_string"** %24, align 8
  %323 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %23, align 8
  %324 = getelementptr inbounds %"class.std::__1::tuple", %"class.std::__1::tuple"* %323, i32 0, i32 0
  %325 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8
  store %"class.std::__1::basic_string"* %325, %"class.std::__1::basic_string"** %22, align 8
  %326 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %22, align 8
  store %"struct.std::__1::__tuple_impl"* %324, %"struct.std::__1::__tuple_impl"** %20, align 8
  store %"class.std::__1::basic_string"* %326, %"class.std::__1::basic_string"** %21, align 8
  %327 = load %"struct.std::__1::__tuple_impl"*, %"struct.std::__1::__tuple_impl"** %20, align 8
  %328 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %21, align 8
  store %"struct.std::__1::__tuple_impl"* %327, %"struct.std::__1::__tuple_impl"** %14, align 8
  store %"class.std::__1::basic_string"* %328, %"class.std::__1::basic_string"** %15, align 8
  %329 = load %"struct.std::__1::__tuple_impl"*, %"struct.std::__1::__tuple_impl"** %14, align 8
  %330 = bitcast %"struct.std::__1::__tuple_impl"* %329 to %"class.std::__1::__tuple_leaf"*
  %331 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %15, align 8
  store %"class.std::__1::basic_string"* %331, %"class.std::__1::basic_string"** %9, align 8
  %332 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %9, align 8
  store %"class.std::__1::__tuple_leaf"* %330, %"class.std::__1::__tuple_leaf"** %7, align 8
  store %"class.std::__1::basic_string"* %332, %"class.std::__1::basic_string"** %8, align 8
  %333 = load %"class.std::__1::__tuple_leaf"*, %"class.std::__1::__tuple_leaf"** %7, align 8
  %334 = getelementptr inbounds %"class.std::__1::__tuple_leaf", %"class.std::__1::__tuple_leaf"* %333, i32 0, i32 0
  %335 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %8, align 8
  store %"class.std::__1::basic_string"* %335, %"class.std::__1::basic_string"** %6, align 8
  %336 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8
  store %"class.std::__1::basic_string"* %336, %"class.std::__1::basic_string"** %334, align 8
  %337 = getelementptr inbounds %"class.std::__1::tuple", %"class.std::__1::tuple"* %32, i32 0, i32 0
  %338 = getelementptr inbounds %"struct.std::__1::__tuple_impl", %"struct.std::__1::__tuple_impl"* %337, i32 0, i32 0
  %339 = getelementptr inbounds %"class.std::__1::__tuple_leaf", %"class.std::__1::__tuple_leaf"* %338, i32 0, i32 0
  %340 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %339, align 8
  %341 = getelementptr inbounds %"class.std::__1::tuple", %"class.std::__1::tuple"* %310, i32 0, i32 0
  %342 = getelementptr inbounds %"struct.std::__1::__tuple_impl", %"struct.std::__1::__tuple_impl"* %341, i32 0, i32 0
  %343 = getelementptr inbounds %"class.std::__1::__tuple_leaf", %"class.std::__1::__tuple_leaf"* %342, i32 0, i32 0
  store %"class.std::__1::basic_string"* %340, %"class.std::__1::basic_string"** %343, align 8
  store %"class.std::__1::tuple.91"* %5, %"class.std::__1::tuple.91"** %4, align 8
  %344 = load %"class.std::__1::tuple.91"*, %"class.std::__1::tuple.91"** %4, align 8
  store %"class.std::__1::tuple.91"* %344, %"class.std::__1::tuple.91"** %3, align 8
  %345 = load %"class.std::__1::tuple.91"*, %"class.std::__1::tuple.91"** %3, align 8
  store %"class.std::__1::__hash_table"* %315, %"class.std::__1::__hash_table"** %284, align 8
  store %"class.std::__1::basic_string"* %316, %"class.std::__1::basic_string"** %285, align 8
  store %"struct.std::__1::piecewise_construct_t"* @_ZNSt3__1L19piecewise_constructE, %"struct.std::__1::piecewise_construct_t"** %286, align 8
  store %"class.std::__1::tuple"* %310, %"class.std::__1::tuple"** %287, align 8
  store %"class.std::__1::tuple.91"* %311, %"class.std::__1::tuple.91"** %288, align 8
  %346 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %284, align 8
  store %"class.std::__1::__hash_table"* %346, %"class.std::__1::__hash_table"** %282, align 8
  %347 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %282, align 8
  %348 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %347, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.48"* %348, %"class.std::__1::__compressed_pair.48"** %281, align 8
  %349 = load %"class.std::__1::__compressed_pair.48"*, %"class.std::__1::__compressed_pair.48"** %281, align 8
  %350 = bitcast %"class.std::__1::__compressed_pair.48"* %349 to %"class.std::__1::__libcpp_compressed_pair_imp.49"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.49"* %350, %"class.std::__1::__libcpp_compressed_pair_imp.49"** %280, align 8
  %351 = load %"class.std::__1::__libcpp_compressed_pair_imp.49"*, %"class.std::__1::__libcpp_compressed_pair_imp.49"** %280, align 8
  %352 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.49"* %351 to %"class.std::__1::__unordered_map_hasher"*
  %353 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %285, align 8
  store %"class.std::__1::__unordered_map_hasher"* %352, %"class.std::__1::__unordered_map_hasher"** %238, align 8
  store %"class.std::__1::basic_string"* %353, %"class.std::__1::basic_string"** %239, align 8
  %354 = load %"class.std::__1::__unordered_map_hasher"*, %"class.std::__1::__unordered_map_hasher"** %238, align 8
  %355 = bitcast %"class.std::__1::__unordered_map_hasher"* %354 to %"struct.std::__1::hash"*
  %356 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %239, align 8
  %357 = call i64 @_ZNKSt3__14hashINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEclERKS6_(%"struct.std::__1::hash"* %355, %"class.std::__1::basic_string"* dereferenceable(24) %356) #14
  store i64 %357, i64* %289, align 8
  store %"class.std::__1::__hash_table"* %346, %"class.std::__1::__hash_table"** %237, align 8
  %358 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %237, align 8
  %359 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %358, i32 0, i32 0
  store %"class.std::__1::unique_ptr.37"* %359, %"class.std::__1::unique_ptr.37"** %236, align 8
  %360 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %236, align 8
  %361 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %360, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %361, %"class.std::__1::__compressed_pair.38"** %235, align 8
  %362 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %235, align 8
  %363 = bitcast %"class.std::__1::__compressed_pair.38"* %362 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %363, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %234, align 8
  %364 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %234, align 8
  %365 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %364, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator"* %365, %"class.std::__1::__bucket_list_deallocator"** %233, align 8
  %366 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %233, align 8
  %367 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %366, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.40"* %367, %"class.std::__1::__compressed_pair.40"** %232, align 8
  %368 = load %"class.std::__1::__compressed_pair.40"*, %"class.std::__1::__compressed_pair.40"** %232, align 8
  %369 = bitcast %"class.std::__1::__compressed_pair.40"* %368 to %"class.std::__1::__libcpp_compressed_pair_imp.41"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.41"* %369, %"class.std::__1::__libcpp_compressed_pair_imp.41"** %231, align 8
  %370 = load %"class.std::__1::__libcpp_compressed_pair_imp.41"*, %"class.std::__1::__libcpp_compressed_pair_imp.41"** %231, align 8
  %371 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.41", %"class.std::__1::__libcpp_compressed_pair_imp.41"* %370, i32 0, i32 0
  %372 = load i64, i64* %371, align 8
  store i64 %372, i64* %290, align 8
  store i8 0, i8* %291, align 1
  %373 = load i64, i64* %290, align 8
  %374 = icmp ne i64 %373, 0
  br i1 %374, label %375, label %717

; <label>:375:                                    ; preds = %2
  %376 = load i64, i64* %289, align 8
  %377 = load i64, i64* %290, align 8
  store i64 %376, i64* %229, align 8
  store i64 %377, i64* %230, align 8
  %378 = load i64, i64* %230, align 8
  %379 = load i64, i64* %230, align 8
  %380 = sub i64 %379, 1
  %381 = and i64 %378, %380
  %382 = icmp ne i64 %381, 0
  br i1 %382, label %388, label %383

; <label>:383:                                    ; preds = %375
  %384 = load i64, i64* %229, align 8
  %385 = load i64, i64* %230, align 8
  %386 = sub i64 %385, 1
  %387 = and i64 %384, %386
  br label %400

; <label>:388:                                    ; preds = %375
  %389 = load i64, i64* %229, align 8
  %390 = load i64, i64* %230, align 8
  %391 = icmp ult i64 %389, %390
  br i1 %391, label %392, label %394

; <label>:392:                                    ; preds = %388
  %393 = load i64, i64* %229, align 8
  br label %398

; <label>:394:                                    ; preds = %388
  %395 = load i64, i64* %229, align 8
  %396 = load i64, i64* %230, align 8
  %397 = urem i64 %395, %396
  br label %398

; <label>:398:                                    ; preds = %394, %392
  %399 = phi i64 [ %393, %392 ], [ %397, %394 ]
  br label %400

; <label>:400:                                    ; preds = %398, %383
  %401 = phi i64 [ %387, %383 ], [ %399, %398 ]
  store i64 %401, i64* %293, align 8
  %402 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %346, i32 0, i32 0
  %403 = load i64, i64* %293, align 8
  store %"class.std::__1::unique_ptr.37"* %402, %"class.std::__1::unique_ptr.37"** %143, align 8
  store i64 %403, i64* %144, align 8
  %404 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %143, align 8
  %405 = load i64, i64* %144, align 8
  %406 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %404, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %406, %"class.std::__1::__compressed_pair.38"** %142, align 8
  %407 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %142, align 8
  %408 = bitcast %"class.std::__1::__compressed_pair.38"* %407 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %408, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %141, align 8
  %409 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %141, align 8
  %410 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %409, i32 0, i32 0
  %411 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %410, align 8
  %412 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %411, i64 %405
  %413 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %412, align 8
  store %"struct.std::__1::__hash_node_base"* %413, %"struct.std::__1::__hash_node_base"** %292, align 8
  %414 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %292, align 8
  %415 = icmp ne %"struct.std::__1::__hash_node_base"* %414, null
  br i1 %415, label %416, label %716

; <label>:416:                                    ; preds = %400
  %417 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %292, align 8
  %418 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %417, i32 0, i32 0
  %419 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %418, align 8
  store %"struct.std::__1::__hash_node_base"* %419, %"struct.std::__1::__hash_node_base"** %292, align 8
  br label %420

; <label>:420:                                    ; preds = %711, %416
  %421 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %292, align 8
  %422 = icmp ne %"struct.std::__1::__hash_node_base"* %421, null
  br i1 %422, label %423, label %456

; <label>:423:                                    ; preds = %420
  %424 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %292, align 8
  store %"struct.std::__1::__hash_node_base"* %424, %"struct.std::__1::__hash_node_base"** %140, align 8
  %425 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %140, align 8
  %426 = bitcast %"struct.std::__1::__hash_node_base"* %425 to %"struct.std::__1::__hash_node"*
  %427 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %426, i32 0, i32 1
  %428 = load i64, i64* %427, align 8
  %429 = load i64, i64* %290, align 8
  store i64 %428, i64* %129, align 8
  store i64 %429, i64* %130, align 8
  %430 = load i64, i64* %130, align 8
  %431 = load i64, i64* %130, align 8
  %432 = sub i64 %431, 1
  %433 = and i64 %430, %432
  %434 = icmp ne i64 %433, 0
  br i1 %434, label %440, label %435

; <label>:435:                                    ; preds = %423
  %436 = load i64, i64* %129, align 8
  %437 = load i64, i64* %130, align 8
  %438 = sub i64 %437, 1
  %439 = and i64 %436, %438
  br label %452

; <label>:440:                                    ; preds = %423
  %441 = load i64, i64* %129, align 8
  %442 = load i64, i64* %130, align 8
  %443 = icmp ult i64 %441, %442
  br i1 %443, label %444, label %446

; <label>:444:                                    ; preds = %440
  %445 = load i64, i64* %129, align 8
  br label %450

; <label>:446:                                    ; preds = %440
  %447 = load i64, i64* %129, align 8
  %448 = load i64, i64* %130, align 8
  %449 = urem i64 %447, %448
  br label %450

; <label>:450:                                    ; preds = %446, %444
  %451 = phi i64 [ %445, %444 ], [ %449, %446 ]
  br label %452

; <label>:452:                                    ; preds = %450, %435
  %453 = phi i64 [ %439, %435 ], [ %451, %450 ]
  %454 = load i64, i64* %293, align 8
  %455 = icmp eq i64 %453, %454
  br label %456

; <label>:456:                                    ; preds = %452, %420
  %457 = phi i1 [ false, %420 ], [ %455, %452 ]
  br i1 %457, label %458, label %715

; <label>:458:                                    ; preds = %456
  store %"class.std::__1::__hash_table"* %346, %"class.std::__1::__hash_table"** %118, align 8
  %459 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %118, align 8
  %460 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %459, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.50"* %460, %"class.std::__1::__compressed_pair.50"** %117, align 8
  %461 = load %"class.std::__1::__compressed_pair.50"*, %"class.std::__1::__compressed_pair.50"** %117, align 8
  %462 = bitcast %"class.std::__1::__compressed_pair.50"* %461 to %"class.std::__1::__libcpp_compressed_pair_imp.51"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.51"* %462, %"class.std::__1::__libcpp_compressed_pair_imp.51"** %116, align 8
  %463 = load %"class.std::__1::__libcpp_compressed_pair_imp.51"*, %"class.std::__1::__libcpp_compressed_pair_imp.51"** %116, align 8
  %464 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.51"* %463 to %"class.std::__1::__unordered_map_equal"*
  %465 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %292, align 8
  store %"struct.std::__1::__hash_node_base"* %465, %"struct.std::__1::__hash_node_base"** %102, align 8
  %466 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %102, align 8
  store %"struct.std::__1::__hash_node_base"* %466, %"struct.std::__1::__hash_node_base"** %101, align 8
  %467 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %101, align 8
  store %"struct.std::__1::__hash_node_base"* %467, %"struct.std::__1::__hash_node_base"** %100, align 8
  %468 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %100, align 8
  %469 = bitcast %"struct.std::__1::__hash_node_base"* %468 to i8*
  %470 = bitcast %"struct.std::__1::__hash_node_base"* %468 to %"struct.std::__1::__hash_node"*
  %471 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %470, i32 0, i32 2
  %472 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %285, align 8
  store %"class.std::__1::__unordered_map_equal"* %464, %"class.std::__1::__unordered_map_equal"** %94, align 8
  store %"union.std::__1::__hash_value_type"* %471, %"union.std::__1::__hash_value_type"** %95, align 8
  store %"class.std::__1::basic_string"* %472, %"class.std::__1::basic_string"** %96, align 8
  %473 = load %"class.std::__1::__unordered_map_equal"*, %"class.std::__1::__unordered_map_equal"** %94, align 8
  %474 = bitcast %"class.std::__1::__unordered_map_equal"* %473 to %"struct.std::__1::equal_to"*
  %475 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %95, align 8
  %476 = bitcast %"union.std::__1::__hash_value_type"* %475 to %"struct.std::__1::pair.92"*
  %477 = getelementptr inbounds %"struct.std::__1::pair.92", %"struct.std::__1::pair.92"* %476, i32 0, i32 0
  %478 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %96, align 8
  store %"struct.std::__1::equal_to"* %474, %"struct.std::__1::equal_to"** %91, align 8
  store %"class.std::__1::basic_string"* %477, %"class.std::__1::basic_string"** %92, align 8
  store %"class.std::__1::basic_string"* %478, %"class.std::__1::basic_string"** %93, align 8
  %479 = load %"struct.std::__1::equal_to"*, %"struct.std::__1::equal_to"** %91, align 8
  %480 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %92, align 8
  %481 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %93, align 8
  store %"class.std::__1::basic_string"* %480, %"class.std::__1::basic_string"** %86, align 8
  store %"class.std::__1::basic_string"* %481, %"class.std::__1::basic_string"** %87, align 8
  %482 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %86, align 8
  store %"class.std::__1::basic_string"* %482, %"class.std::__1::basic_string"** %84, align 8
  %483 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %84, align 8
  store %"class.std::__1::basic_string"* %483, %"class.std::__1::basic_string"** %83, align 8
  %484 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %83, align 8
  %485 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %484, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %485, %"class.std::__1::__compressed_pair.18"** %82, align 8
  %486 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %82, align 8
  %487 = bitcast %"class.std::__1::__compressed_pair.18"* %486 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %487, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %81, align 8
  %488 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %81, align 8
  %489 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %488, i32 0, i32 0
  %490 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %489, i32 0, i32 0
  %491 = bitcast %union.anon* %490 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %492 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %491, i32 0, i32 0
  %493 = bitcast %union.anon.20* %492 to i8*
  %494 = load i8, i8* %493, align 8
  %495 = zext i8 %494 to i32
  %496 = and i32 %495, 1
  %497 = icmp ne i32 %496, 0
  br i1 %497, label %498, label %509

; <label>:498:                                    ; preds = %458
  store %"class.std::__1::basic_string"* %483, %"class.std::__1::basic_string"** %77, align 8
  %499 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %77, align 8
  %500 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %499, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %500, %"class.std::__1::__compressed_pair.18"** %76, align 8
  %501 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %76, align 8
  %502 = bitcast %"class.std::__1::__compressed_pair.18"* %501 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %502, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %75, align 8
  %503 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %75, align 8
  %504 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %503, i32 0, i32 0
  %505 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %504, i32 0, i32 0
  %506 = bitcast %union.anon* %505 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %507 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %506, i32 0, i32 1
  %508 = load i64, i64* %507, align 8
  br label %524

; <label>:509:                                    ; preds = %458
  store %"class.std::__1::basic_string"* %483, %"class.std::__1::basic_string"** %80, align 8
  %510 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %80, align 8
  %511 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %510, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %511, %"class.std::__1::__compressed_pair.18"** %79, align 8
  %512 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %79, align 8
  %513 = bitcast %"class.std::__1::__compressed_pair.18"* %512 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %513, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %78, align 8
  %514 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %78, align 8
  %515 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %514, i32 0, i32 0
  %516 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %515, i32 0, i32 0
  %517 = bitcast %union.anon* %516 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %518 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %517, i32 0, i32 0
  %519 = bitcast %union.anon.20* %518 to i8*
  %520 = load i8, i8* %519, align 8
  %521 = zext i8 %520 to i32
  %522 = ashr i32 %521, 1
  %523 = sext i32 %522 to i64
  br label %524

; <label>:524:                                    ; preds = %509, %498
  %525 = phi i64 [ %508, %498 ], [ %523, %509 ]
  store i64 %525, i64* %88, align 8
  %526 = load i64, i64* %88, align 8
  %527 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %87, align 8
  store %"class.std::__1::basic_string"* %527, %"class.std::__1::basic_string"** %74, align 8
  %528 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %74, align 8
  store %"class.std::__1::basic_string"* %528, %"class.std::__1::basic_string"** %73, align 8
  %529 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %73, align 8
  %530 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %529, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %530, %"class.std::__1::__compressed_pair.18"** %72, align 8
  %531 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %72, align 8
  %532 = bitcast %"class.std::__1::__compressed_pair.18"* %531 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %532, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %71, align 8
  %533 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %71, align 8
  %534 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %533, i32 0, i32 0
  %535 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %534, i32 0, i32 0
  %536 = bitcast %union.anon* %535 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %537 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %536, i32 0, i32 0
  %538 = bitcast %union.anon.20* %537 to i8*
  %539 = load i8, i8* %538, align 8
  %540 = zext i8 %539 to i32
  %541 = and i32 %540, 1
  %542 = icmp ne i32 %541, 0
  br i1 %542, label %543, label %554

; <label>:543:                                    ; preds = %524
  store %"class.std::__1::basic_string"* %528, %"class.std::__1::basic_string"** %67, align 8
  %544 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %67, align 8
  %545 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %544, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %545, %"class.std::__1::__compressed_pair.18"** %66, align 8
  %546 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %66, align 8
  %547 = bitcast %"class.std::__1::__compressed_pair.18"* %546 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %547, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %65, align 8
  %548 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %65, align 8
  %549 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %548, i32 0, i32 0
  %550 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %549, i32 0, i32 0
  %551 = bitcast %union.anon* %550 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %552 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %551, i32 0, i32 1
  %553 = load i64, i64* %552, align 8
  br label %569

; <label>:554:                                    ; preds = %524
  store %"class.std::__1::basic_string"* %528, %"class.std::__1::basic_string"** %70, align 8
  %555 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %70, align 8
  %556 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %555, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %556, %"class.std::__1::__compressed_pair.18"** %69, align 8
  %557 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %69, align 8
  %558 = bitcast %"class.std::__1::__compressed_pair.18"* %557 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %558, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %68, align 8
  %559 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %68, align 8
  %560 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %559, i32 0, i32 0
  %561 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %560, i32 0, i32 0
  %562 = bitcast %union.anon* %561 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %563 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %562, i32 0, i32 0
  %564 = bitcast %union.anon.20* %563 to i8*
  %565 = load i8, i8* %564, align 8
  %566 = zext i8 %565 to i32
  %567 = ashr i32 %566, 1
  %568 = sext i32 %567 to i64
  br label %569

; <label>:569:                                    ; preds = %554, %543
  %570 = phi i64 [ %553, %543 ], [ %568, %554 ]
  %571 = icmp ne i64 %526, %570
  br i1 %571, label %572, label %573

; <label>:572:                                    ; preds = %569
  store i1 false, i1* %85, align 1
  br label %708

; <label>:573:                                    ; preds = %569
  %574 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %86, align 8
  store %"class.std::__1::basic_string"* %574, %"class.std::__1::basic_string"** %64, align 8
  %575 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8
  store %"class.std::__1::basic_string"* %575, %"class.std::__1::basic_string"** %63, align 8
  %576 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8
  store %"class.std::__1::basic_string"* %576, %"class.std::__1::basic_string"** %62, align 8
  %577 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8
  %578 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %577, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %578, %"class.std::__1::__compressed_pair.18"** %61, align 8
  %579 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %61, align 8
  %580 = bitcast %"class.std::__1::__compressed_pair.18"* %579 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %580, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %60, align 8
  %581 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %60, align 8
  %582 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %581, i32 0, i32 0
  %583 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %582, i32 0, i32 0
  %584 = bitcast %union.anon* %583 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %585 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %584, i32 0, i32 0
  %586 = bitcast %union.anon.20* %585 to i8*
  %587 = load i8, i8* %586, align 8
  %588 = zext i8 %587 to i32
  %589 = and i32 %588, 1
  %590 = icmp ne i32 %589, 0
  br i1 %590, label %591, label %602

; <label>:591:                                    ; preds = %573
  store %"class.std::__1::basic_string"* %576, %"class.std::__1::basic_string"** %54, align 8
  %592 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8
  %593 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %592, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %593, %"class.std::__1::__compressed_pair.18"** %53, align 8
  %594 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %53, align 8
  %595 = bitcast %"class.std::__1::__compressed_pair.18"* %594 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %595, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %52, align 8
  %596 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %52, align 8
  %597 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %596, i32 0, i32 0
  %598 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %597, i32 0, i32 0
  %599 = bitcast %union.anon* %598 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %600 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %599, i32 0, i32 2
  %601 = load i8*, i8** %600, align 8
  br label %615

; <label>:602:                                    ; preds = %573
  store %"class.std::__1::basic_string"* %576, %"class.std::__1::basic_string"** %59, align 8
  %603 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %59, align 8
  %604 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %603, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %604, %"class.std::__1::__compressed_pair.18"** %58, align 8
  %605 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %58, align 8
  %606 = bitcast %"class.std::__1::__compressed_pair.18"* %605 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %606, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %57, align 8
  %607 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %57, align 8
  %608 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %607, i32 0, i32 0
  %609 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %608, i32 0, i32 0
  %610 = bitcast %union.anon* %609 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %611 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %610, i32 0, i32 1
  %612 = getelementptr inbounds [23 x i8], [23 x i8]* %611, i64 0, i64 0
  store i8* %612, i8** %56, align 8
  %613 = load i8*, i8** %56, align 8
  store i8* %613, i8** %55, align 8
  %614 = load i8*, i8** %55, align 8
  br label %615

; <label>:615:                                    ; preds = %602, %591
  %616 = phi i8* [ %601, %591 ], [ %614, %602 ]
  store i8* %616, i8** %51, align 8
  %617 = load i8*, i8** %51, align 8
  store i8* %617, i8** %89, align 8
  %618 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %87, align 8
  store %"class.std::__1::basic_string"* %618, %"class.std::__1::basic_string"** %47, align 8
  %619 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %47, align 8
  store %"class.std::__1::basic_string"* %619, %"class.std::__1::basic_string"** %46, align 8
  %620 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %46, align 8
  store %"class.std::__1::basic_string"* %620, %"class.std::__1::basic_string"** %45, align 8
  %621 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8
  %622 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %621, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %622, %"class.std::__1::__compressed_pair.18"** %44, align 8
  %623 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %44, align 8
  %624 = bitcast %"class.std::__1::__compressed_pair.18"* %623 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %624, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %43, align 8
  %625 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %43, align 8
  %626 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %625, i32 0, i32 0
  %627 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %626, i32 0, i32 0
  %628 = bitcast %union.anon* %627 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %629 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %628, i32 0, i32 0
  %630 = bitcast %union.anon.20* %629 to i8*
  %631 = load i8, i8* %630, align 8
  %632 = zext i8 %631 to i32
  %633 = and i32 %632, 1
  %634 = icmp ne i32 %633, 0
  br i1 %634, label %635, label %646

; <label>:635:                                    ; preds = %615
  store %"class.std::__1::basic_string"* %620, %"class.std::__1::basic_string"** %37, align 8
  %636 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8
  %637 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %636, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %637, %"class.std::__1::__compressed_pair.18"** %36, align 8
  %638 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %36, align 8
  %639 = bitcast %"class.std::__1::__compressed_pair.18"* %638 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %639, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %35, align 8
  %640 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %35, align 8
  %641 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %640, i32 0, i32 0
  %642 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %641, i32 0, i32 0
  %643 = bitcast %union.anon* %642 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %644 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %643, i32 0, i32 2
  %645 = load i8*, i8** %644, align 8
  br label %659

; <label>:646:                                    ; preds = %615
  store %"class.std::__1::basic_string"* %620, %"class.std::__1::basic_string"** %42, align 8
  %647 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8
  %648 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %647, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %648, %"class.std::__1::__compressed_pair.18"** %41, align 8
  %649 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %41, align 8
  %650 = bitcast %"class.std::__1::__compressed_pair.18"* %649 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %650, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %40, align 8
  %651 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %40, align 8
  %652 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %651, i32 0, i32 0
  %653 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %652, i32 0, i32 0
  %654 = bitcast %union.anon* %653 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %655 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %654, i32 0, i32 1
  %656 = getelementptr inbounds [23 x i8], [23 x i8]* %655, i64 0, i64 0
  store i8* %656, i8** %39, align 8
  %657 = load i8*, i8** %39, align 8
  store i8* %657, i8** %38, align 8
  %658 = load i8*, i8** %38, align 8
  br label %659

; <label>:659:                                    ; preds = %646, %635
  %660 = phi i8* [ %645, %635 ], [ %658, %646 ]
  store i8* %660, i8** %34, align 8
  %661 = load i8*, i8** %34, align 8
  store i8* %661, i8** %90, align 8
  %662 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %86, align 8
  store %"class.std::__1::basic_string"* %662, %"class.std::__1::basic_string"** %50, align 8
  %663 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8
  %664 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %663, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %664, %"class.std::__1::__compressed_pair.18"** %49, align 8
  %665 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %49, align 8
  %666 = bitcast %"class.std::__1::__compressed_pair.18"* %665 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %666, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %48, align 8
  %667 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %48, align 8
  %668 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %667, i32 0, i32 0
  %669 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %668, i32 0, i32 0
  %670 = bitcast %union.anon* %669 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %671 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %670, i32 0, i32 0
  %672 = bitcast %union.anon.20* %671 to i8*
  %673 = load i8, i8* %672, align 8
  %674 = zext i8 %673 to i32
  %675 = and i32 %674, 1
  %676 = icmp ne i32 %675, 0
  br i1 %676, label %677, label %684

; <label>:677:                                    ; preds = %659
  %678 = load i8*, i8** %89, align 8
  %679 = load i8*, i8** %90, align 8
  %680 = load i64, i64* %88, align 8
  %681 = invoke i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8* %678, i8* %679, i64 %680)
          to label %682 unwind label %705

; <label>:682:                                    ; preds = %677
  %683 = icmp eq i32 %681, 0
  store i1 %683, i1* %85, align 1
  br label %708

; <label>:684:                                    ; preds = %659
  br label %685

; <label>:685:                                    ; preds = %697, %684
  %686 = load i64, i64* %88, align 8
  %687 = icmp ne i64 %686, 0
  br i1 %687, label %688, label %704

; <label>:688:                                    ; preds = %685
  %689 = load i8*, i8** %89, align 8
  %690 = load i8, i8* %689, align 1
  %691 = sext i8 %690 to i32
  %692 = load i8*, i8** %90, align 8
  %693 = load i8, i8* %692, align 1
  %694 = sext i8 %693 to i32
  %695 = icmp ne i32 %691, %694
  br i1 %695, label %696, label %697

; <label>:696:                                    ; preds = %688
  store i1 false, i1* %85, align 1
  br label %708

; <label>:697:                                    ; preds = %688
  %698 = load i64, i64* %88, align 8
  %699 = add i64 %698, -1
  store i64 %699, i64* %88, align 8
  %700 = load i8*, i8** %89, align 8
  %701 = getelementptr inbounds i8, i8* %700, i32 1
  store i8* %701, i8** %89, align 8
  %702 = load i8*, i8** %90, align 8
  %703 = getelementptr inbounds i8, i8* %702, i32 1
  store i8* %703, i8** %90, align 8
  br label %685

; <label>:704:                                    ; preds = %685
  store i1 true, i1* %85, align 1
  br label %708

; <label>:705:                                    ; preds = %677
  %706 = landingpad { i8*, i32 }
          catch i8* null
  %707 = extractvalue { i8*, i32 } %706, 0
  call void @__clang_call_terminate(i8* %707) #16
  unreachable

; <label>:708:                                    ; preds = %704, %696, %682, %572
  %709 = load i1, i1* %85, align 1
  br i1 %709, label %710, label %711

; <label>:710:                                    ; preds = %708
  br label %1150

; <label>:711:                                    ; preds = %708
  %712 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %292, align 8
  %713 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %712, i32 0, i32 0
  %714 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %713, align 8
  store %"struct.std::__1::__hash_node_base"* %714, %"struct.std::__1::__hash_node_base"** %292, align 8
  br label %420

; <label>:715:                                    ; preds = %456
  br label %716

; <label>:716:                                    ; preds = %715, %400
  br label %717

; <label>:717:                                    ; preds = %716, %2
  %718 = load i64, i64* %289, align 8
  %719 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %286, align 8
  store %"struct.std::__1::piecewise_construct_t"* %719, %"struct.std::__1::piecewise_construct_t"** %97, align 8
  %720 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %97, align 8
  %721 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %287, align 8
  store %"class.std::__1::tuple"* %721, %"class.std::__1::tuple"** %98, align 8
  %722 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %98, align 8
  %723 = load %"class.std::__1::tuple.91"*, %"class.std::__1::tuple.91"** %288, align 8
  store %"class.std::__1::tuple.91"* %723, %"class.std::__1::tuple.91"** %99, align 8
  %724 = load %"class.std::__1::tuple.91"*, %"class.std::__1::tuple.91"** %99, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE21__construct_node_hashIRKNS_21piecewise_construct_tEJNS_5tupleIJOS7_EEENSQ_IJEEEEEENS_10unique_ptrINS_11__hash_nodeISB_PvEENS_22__hash_node_destructorINS5_ISX_EEEEEEmOT_DpOT0_(%"class.std::__1::unique_ptr.94"* sret %294, %"class.std::__1::__hash_table"* %346, i64 %718, %"struct.std::__1::piecewise_construct_t"* dereferenceable(1) %720, %"class.std::__1::tuple"* dereferenceable(8) %722, %"class.std::__1::tuple.91"* dereferenceable(1) %724)
  store %"class.std::__1::__hash_table"* %346, %"class.std::__1::__hash_table"** %105, align 8
  %725 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %105, align 8
  %726 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %725, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.48"* %726, %"class.std::__1::__compressed_pair.48"** %104, align 8
  %727 = load %"class.std::__1::__compressed_pair.48"*, %"class.std::__1::__compressed_pair.48"** %104, align 8
  %728 = bitcast %"class.std::__1::__compressed_pair.48"* %727 to %"class.std::__1::__libcpp_compressed_pair_imp.49"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.49"* %728, %"class.std::__1::__libcpp_compressed_pair_imp.49"** %103, align 8
  %729 = load %"class.std::__1::__libcpp_compressed_pair_imp.49"*, %"class.std::__1::__libcpp_compressed_pair_imp.49"** %103, align 8
  %730 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.49", %"class.std::__1::__libcpp_compressed_pair_imp.49"* %729, i32 0, i32 0
  %731 = load i64, i64* %730, align 8
  %732 = add i64 %731, 1
  %733 = uitofp i64 %732 to float
  %734 = load i64, i64* %290, align 8
  %735 = uitofp i64 %734 to float
  store %"class.std::__1::__hash_table"* %346, %"class.std::__1::__hash_table"** %108, align 8
  %736 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %108, align 8
  %737 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %736, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.50"* %737, %"class.std::__1::__compressed_pair.50"** %107, align 8
  %738 = load %"class.std::__1::__compressed_pair.50"*, %"class.std::__1::__compressed_pair.50"** %107, align 8
  %739 = bitcast %"class.std::__1::__compressed_pair.50"* %738 to %"class.std::__1::__libcpp_compressed_pair_imp.51"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.51"* %739, %"class.std::__1::__libcpp_compressed_pair_imp.51"** %106, align 8
  %740 = load %"class.std::__1::__libcpp_compressed_pair_imp.51"*, %"class.std::__1::__libcpp_compressed_pair_imp.51"** %106, align 8
  %741 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.51", %"class.std::__1::__libcpp_compressed_pair_imp.51"* %740, i32 0, i32 0
  %742 = load float, float* %741, align 4
  %743 = fmul float %735, %742
  %744 = fcmp ogt float %733, %743
  br i1 %744, label %748, label %745

; <label>:745:                                    ; preds = %717
  %746 = load i64, i64* %290, align 8
  %747 = icmp eq i64 %746, 0
  br i1 %747, label %748, label %910

; <label>:748:                                    ; preds = %745, %717
  %749 = load i64, i64* %290, align 8
  %750 = mul i64 2, %749
  %751 = load i64, i64* %290, align 8
  store i64 %751, i64* %109, align 8
  %752 = load i64, i64* %109, align 8
  %753 = icmp ugt i64 %752, 2
  br i1 %753, label %754, label %761

; <label>:754:                                    ; preds = %748
  %755 = load i64, i64* %109, align 8
  %756 = load i64, i64* %109, align 8
  %757 = sub i64 %756, 1
  %758 = and i64 %755, %757
  %759 = icmp ne i64 %758, 0
  %760 = xor i1 %759, true
  br label %761

; <label>:761:                                    ; preds = %754, %748
  %762 = phi i1 [ false, %748 ], [ %760, %754 ]
  %763 = xor i1 %762, true
  %764 = zext i1 %763 to i64
  %765 = add i64 %750, %764
  store i64 %765, i64* %295, align 8
  store %"class.std::__1::__hash_table"* %346, %"class.std::__1::__hash_table"** %112, align 8
  %766 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %112, align 8
  %767 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %766, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.48"* %767, %"class.std::__1::__compressed_pair.48"** %111, align 8
  %768 = load %"class.std::__1::__compressed_pair.48"*, %"class.std::__1::__compressed_pair.48"** %111, align 8
  %769 = bitcast %"class.std::__1::__compressed_pair.48"* %768 to %"class.std::__1::__libcpp_compressed_pair_imp.49"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.49"* %769, %"class.std::__1::__libcpp_compressed_pair_imp.49"** %110, align 8
  %770 = load %"class.std::__1::__libcpp_compressed_pair_imp.49"*, %"class.std::__1::__libcpp_compressed_pair_imp.49"** %110, align 8
  %771 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.49", %"class.std::__1::__libcpp_compressed_pair_imp.49"* %770, i32 0, i32 0
  %772 = load i64, i64* %771, align 8
  %773 = add i64 %772, 1
  %774 = uitofp i64 %773 to float
  store %"class.std::__1::__hash_table"* %346, %"class.std::__1::__hash_table"** %115, align 8
  %775 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %115, align 8
  %776 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %775, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.50"* %776, %"class.std::__1::__compressed_pair.50"** %114, align 8
  %777 = load %"class.std::__1::__compressed_pair.50"*, %"class.std::__1::__compressed_pair.50"** %114, align 8
  %778 = bitcast %"class.std::__1::__compressed_pair.50"* %777 to %"class.std::__1::__libcpp_compressed_pair_imp.51"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.51"* %778, %"class.std::__1::__libcpp_compressed_pair_imp.51"** %113, align 8
  %779 = load %"class.std::__1::__libcpp_compressed_pair_imp.51"*, %"class.std::__1::__libcpp_compressed_pair_imp.51"** %113, align 8
  %780 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.51", %"class.std::__1::__libcpp_compressed_pair_imp.51"* %779, i32 0, i32 0
  %781 = load float, float* %780, align 4
  %782 = fdiv float %774, %781
  store float %782, float* %119, align 4
  %783 = load float, float* %119, align 4
  %784 = call float @ceilf(float %783) #12
  %785 = fptoui float %784 to i64
  store i64 %785, i64* %298, align 8
  store i64* %295, i64** %126, align 8
  store i64* %298, i64** %127, align 8
  %786 = load i64*, i64** %126, align 8
  %787 = load i64*, i64** %127, align 8
  store i64* %786, i64** %124, align 8
  store i64* %787, i64** %125, align 8
  %788 = load i64*, i64** %124, align 8
  %789 = load i64*, i64** %125, align 8
  store %"struct.std::__1::__less"* %123, %"struct.std::__1::__less"** %120, align 8
  store i64* %788, i64** %121, align 8
  store i64* %789, i64** %122, align 8
  %790 = load %"struct.std::__1::__less"*, %"struct.std::__1::__less"** %120, align 8
  %791 = load i64*, i64** %121, align 8
  %792 = load i64, i64* %791, align 8
  %793 = load i64*, i64** %122, align 8
  %794 = load i64, i64* %793, align 8
  %795 = icmp ult i64 %792, %794
  br i1 %795, label %796, label %798

; <label>:796:                                    ; preds = %761
  %797 = load i64*, i64** %125, align 8
  br label %800

; <label>:798:                                    ; preds = %761
  %799 = load i64*, i64** %124, align 8
  br label %800

; <label>:800:                                    ; preds = %798, %796
  %801 = phi i64* [ %797, %796 ], [ %799, %798 ]
  %802 = load i64, i64* %801, align 8
  invoke void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE6rehashEm(%"class.std::__1::__hash_table"* %346, i64 %802)
          to label %803 unwind label %845

; <label>:803:                                    ; preds = %800
  store %"class.std::__1::__hash_table"* %346, %"class.std::__1::__hash_table"** %137, align 8
  %804 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %137, align 8
  %805 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %804, i32 0, i32 0
  store %"class.std::__1::unique_ptr.37"* %805, %"class.std::__1::unique_ptr.37"** %136, align 8
  %806 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %136, align 8
  %807 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %806, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %807, %"class.std::__1::__compressed_pair.38"** %135, align 8
  %808 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %135, align 8
  %809 = bitcast %"class.std::__1::__compressed_pair.38"* %808 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %809, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %134, align 8
  %810 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %134, align 8
  %811 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %810, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator"* %811, %"class.std::__1::__bucket_list_deallocator"** %133, align 8
  %812 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %133, align 8
  %813 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %812, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.40"* %813, %"class.std::__1::__compressed_pair.40"** %132, align 8
  %814 = load %"class.std::__1::__compressed_pair.40"*, %"class.std::__1::__compressed_pair.40"** %132, align 8
  %815 = bitcast %"class.std::__1::__compressed_pair.40"* %814 to %"class.std::__1::__libcpp_compressed_pair_imp.41"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.41"* %815, %"class.std::__1::__libcpp_compressed_pair_imp.41"** %131, align 8
  %816 = load %"class.std::__1::__libcpp_compressed_pair_imp.41"*, %"class.std::__1::__libcpp_compressed_pair_imp.41"** %131, align 8
  %817 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.41", %"class.std::__1::__libcpp_compressed_pair_imp.41"* %816, i32 0, i32 0
  %818 = load i64, i64* %817, align 8
  store i64 %818, i64* %290, align 8
  %819 = load i64, i64* %289, align 8
  %820 = load i64, i64* %290, align 8
  store i64 %819, i64* %138, align 8
  store i64 %820, i64* %139, align 8
  %821 = load i64, i64* %139, align 8
  %822 = load i64, i64* %139, align 8
  %823 = sub i64 %822, 1
  %824 = and i64 %821, %823
  %825 = icmp ne i64 %824, 0
  br i1 %825, label %831, label %826

; <label>:826:                                    ; preds = %803
  %827 = load i64, i64* %138, align 8
  %828 = load i64, i64* %139, align 8
  %829 = sub i64 %828, 1
  %830 = and i64 %827, %829
  br label %843

; <label>:831:                                    ; preds = %803
  %832 = load i64, i64* %138, align 8
  %833 = load i64, i64* %139, align 8
  %834 = icmp ult i64 %832, %833
  br i1 %834, label %835, label %837

; <label>:835:                                    ; preds = %831
  %836 = load i64, i64* %138, align 8
  br label %841

; <label>:837:                                    ; preds = %831
  %838 = load i64, i64* %138, align 8
  %839 = load i64, i64* %139, align 8
  %840 = urem i64 %838, %839
  br label %841

; <label>:841:                                    ; preds = %837, %835
  %842 = phi i64 [ %836, %835 ], [ %840, %837 ]
  br label %843

; <label>:843:                                    ; preds = %841, %826
  %844 = phi i64 [ %830, %826 ], [ %842, %841 ]
  store i64 %844, i64* %293, align 8
  br label %910

; <label>:845:                                    ; preds = %800
  %846 = landingpad { i8*, i32 }
          cleanup
  %847 = extractvalue { i8*, i32 } %846, 0
  store i8* %847, i8** %296, align 8
  %848 = extractvalue { i8*, i32 } %846, 1
  store i32 %848, i32* %297, align 4
  store %"class.std::__1::unique_ptr.94"* %294, %"class.std::__1::unique_ptr.94"** %172, align 8
  %849 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %172, align 8
  store %"class.std::__1::unique_ptr.94"* %849, %"class.std::__1::unique_ptr.94"** %171, align 8
  %850 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %171, align 8
  store %"class.std::__1::unique_ptr.94"* %850, %"class.std::__1::unique_ptr.94"** %168, align 8
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %169, align 8
  %851 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %168, align 8
  %852 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %851, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %852, %"class.std::__1::__compressed_pair.95"** %167, align 8
  %853 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %167, align 8
  %854 = bitcast %"class.std::__1::__compressed_pair.95"* %853 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %854, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %166, align 8
  %855 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %166, align 8
  %856 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %855, i32 0, i32 0
  %857 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %856, align 8
  store %"struct.std::__1::__hash_node"* %857, %"struct.std::__1::__hash_node"** %170, align 8
  %858 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %169, align 8
  %859 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %851, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %859, %"class.std::__1::__compressed_pair.95"** %148, align 8
  %860 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %148, align 8
  %861 = bitcast %"class.std::__1::__compressed_pair.95"* %860 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %861, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %147, align 8
  %862 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %147, align 8
  %863 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %862, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* %858, %"struct.std::__1::__hash_node"** %863, align 8
  %864 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %170, align 8
  %865 = icmp ne %"struct.std::__1::__hash_node"* %864, null
  br i1 %865, label %866, label %905

; <label>:866:                                    ; preds = %845
  %867 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %851, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %867, %"class.std::__1::__compressed_pair.95"** %146, align 8
  %868 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %146, align 8
  %869 = bitcast %"class.std::__1::__compressed_pair.95"* %868 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %869, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %145, align 8
  %870 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %145, align 8
  %871 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %870, i32 0, i32 1
  %872 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %170, align 8
  store %"class.std::__1::__hash_node_destructor"* %871, %"class.std::__1::__hash_node_destructor"** %164, align 8
  store %"struct.std::__1::__hash_node"* %872, %"struct.std::__1::__hash_node"** %165, align 8
  %873 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %164, align 8
  %874 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %873, i32 0, i32 1
  %875 = load i8, i8* %874, align 8
  %876 = trunc i8 %875 to i1
  br i1 %876, label %877, label %890

; <label>:877:                                    ; preds = %866
  %878 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %873, i32 0, i32 0
  %879 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %878, align 8
  %880 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %165, align 8
  %881 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %880, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %881, %"union.std::__1::__hash_value_type"** %163, align 8
  %882 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %163, align 8
  %883 = bitcast %"union.std::__1::__hash_value_type"* %882 to %"struct.std::__1::pair.92"*
  store %"struct.std::__1::pair.92"* %883, %"struct.std::__1::pair.92"** %162, align 8
  %884 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %162, align 8
  %885 = bitcast %"struct.std::__1::pair.92"* %884 to i8*
  store %"class.std::__1::allocator.46"* %879, %"class.std::__1::allocator.46"** %158, align 8
  store %"struct.std::__1::pair.92"* %884, %"struct.std::__1::pair.92"** %159, align 8
  %886 = bitcast %"struct.std::__1::__has_destroy"* %161 to %"struct.std::__1::integral_constant.99"*
  %887 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %158, align 8
  %888 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %159, align 8
  store %"class.std::__1::allocator.46"* %887, %"class.std::__1::allocator.46"** %156, align 8
  store %"struct.std::__1::pair.92"* %888, %"struct.std::__1::pair.92"** %157, align 8
  %889 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %157, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEED1Ev(%"struct.std::__1::pair.92"* %889) #14
  br label %890

; <label>:890:                                    ; preds = %877, %866
  %891 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %165, align 8
  %892 = icmp ne %"struct.std::__1::__hash_node"* %891, null
  br i1 %892, label %893, label %904

; <label>:893:                                    ; preds = %890
  %894 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %873, i32 0, i32 0
  %895 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %894, align 8
  %896 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %165, align 8
  store %"class.std::__1::allocator.46"* %895, %"class.std::__1::allocator.46"** %153, align 8
  store %"struct.std::__1::__hash_node"* %896, %"struct.std::__1::__hash_node"** %154, align 8
  store i64 1, i64* %155, align 8
  %897 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %153, align 8
  %898 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %154, align 8
  %899 = load i64, i64* %155, align 8
  store %"class.std::__1::allocator.46"* %897, %"class.std::__1::allocator.46"** %150, align 8
  store %"struct.std::__1::__hash_node"* %898, %"struct.std::__1::__hash_node"** %151, align 8
  store i64 %899, i64* %152, align 8
  %900 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %150, align 8
  %901 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %151, align 8
  %902 = bitcast %"struct.std::__1::__hash_node"* %901 to i8*
  store i8* %902, i8** %149, align 8
  %903 = load i8*, i8** %149, align 8
  call void @_ZdlPv(i8* %903) #17
  br label %904

; <label>:904:                                    ; preds = %893, %890
  br label %905

; <label>:905:                                    ; preds = %904, %845
  %906 = load i8*, i8** %296, align 8
  %907 = load i32, i32* %297, align 4
  %908 = insertvalue { i8*, i32 } undef, i8* %906, 0
  %909 = insertvalue { i8*, i32 } %908, i32 %907, 1
  resume { i8*, i32 } %909

; <label>:910:                                    ; preds = %843, %745
  %911 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %346, i32 0, i32 0
  %912 = load i64, i64* %293, align 8
  store %"class.std::__1::unique_ptr.37"* %911, %"class.std::__1::unique_ptr.37"** %175, align 8
  store i64 %912, i64* %176, align 8
  %913 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %175, align 8
  %914 = load i64, i64* %176, align 8
  %915 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %913, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %915, %"class.std::__1::__compressed_pair.38"** %174, align 8
  %916 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %174, align 8
  %917 = bitcast %"class.std::__1::__compressed_pair.38"* %916 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %917, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %173, align 8
  %918 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %173, align 8
  %919 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %918, i32 0, i32 0
  %920 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %919, align 8
  %921 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %920, i64 %914
  %922 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %921, align 8
  store %"struct.std::__1::__hash_node_base"* %922, %"struct.std::__1::__hash_node_base"** %299, align 8
  %923 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %299, align 8
  %924 = icmp eq %"struct.std::__1::__hash_node_base"* %923, null
  br i1 %924, label %925, label %1047

; <label>:925:                                    ; preds = %910
  %926 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %346, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.44"* %926, %"class.std::__1::__compressed_pair.44"** %178, align 8
  %927 = load %"class.std::__1::__compressed_pair.44"*, %"class.std::__1::__compressed_pair.44"** %178, align 8
  %928 = bitcast %"class.std::__1::__compressed_pair.44"* %927 to %"class.std::__1::__libcpp_compressed_pair_imp.45"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.45"* %928, %"class.std::__1::__libcpp_compressed_pair_imp.45"** %177, align 8
  %929 = load %"class.std::__1::__libcpp_compressed_pair_imp.45"*, %"class.std::__1::__libcpp_compressed_pair_imp.45"** %177, align 8
  %930 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.45", %"class.std::__1::__libcpp_compressed_pair_imp.45"* %929, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %930, %"struct.std::__1::__hash_node_base"** %181, align 8
  %931 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %181, align 8
  store %"struct.std::__1::__hash_node_base"* %931, %"struct.std::__1::__hash_node_base"** %180, align 8
  %932 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %180, align 8
  store %"struct.std::__1::__hash_node_base"* %932, %"struct.std::__1::__hash_node_base"** %179, align 8
  %933 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %179, align 8
  %934 = bitcast %"struct.std::__1::__hash_node_base"* %933 to i8*
  store %"struct.std::__1::__hash_node_base"* %933, %"struct.std::__1::__hash_node_base"** %299, align 8
  %935 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %299, align 8
  %936 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %935, i32 0, i32 0
  %937 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %936, align 8
  store %"class.std::__1::unique_ptr.94"* %294, %"class.std::__1::unique_ptr.94"** %184, align 8
  %938 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %184, align 8
  %939 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %938, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %939, %"class.std::__1::__compressed_pair.95"** %183, align 8
  %940 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %183, align 8
  %941 = bitcast %"class.std::__1::__compressed_pair.95"* %940 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %941, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %182, align 8
  %942 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %182, align 8
  %943 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %942, i32 0, i32 0
  %944 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %943, align 8
  %945 = bitcast %"struct.std::__1::__hash_node"* %944 to %"struct.std::__1::__hash_node_base"*
  %946 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %945, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %937, %"struct.std::__1::__hash_node_base"** %946, align 8
  store %"class.std::__1::unique_ptr.94"* %294, %"class.std::__1::unique_ptr.94"** %187, align 8
  %947 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %187, align 8
  %948 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %947, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %948, %"class.std::__1::__compressed_pair.95"** %186, align 8
  %949 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %186, align 8
  %950 = bitcast %"class.std::__1::__compressed_pair.95"* %949 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %950, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %185, align 8
  %951 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %185, align 8
  %952 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %951, i32 0, i32 0
  %953 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %952, align 8
  %954 = bitcast %"struct.std::__1::__hash_node"* %953 to %"struct.std::__1::__hash_node_base"*
  store %"struct.std::__1::__hash_node_base"* %954, %"struct.std::__1::__hash_node_base"** %190, align 8
  %955 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %190, align 8
  store %"struct.std::__1::__hash_node_base"* %955, %"struct.std::__1::__hash_node_base"** %189, align 8
  %956 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %189, align 8
  store %"struct.std::__1::__hash_node_base"* %956, %"struct.std::__1::__hash_node_base"** %188, align 8
  %957 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %188, align 8
  %958 = bitcast %"struct.std::__1::__hash_node_base"* %957 to i8*
  %959 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %299, align 8
  %960 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %959, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %957, %"struct.std::__1::__hash_node_base"** %960, align 8
  %961 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %299, align 8
  %962 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %346, i32 0, i32 0
  %963 = load i64, i64* %293, align 8
  store %"class.std::__1::unique_ptr.37"* %962, %"class.std::__1::unique_ptr.37"** %193, align 8
  store i64 %963, i64* %194, align 8
  %964 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %193, align 8
  %965 = load i64, i64* %194, align 8
  %966 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %964, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %966, %"class.std::__1::__compressed_pair.38"** %192, align 8
  %967 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %192, align 8
  %968 = bitcast %"class.std::__1::__compressed_pair.38"* %967 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %968, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %191, align 8
  %969 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %191, align 8
  %970 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %969, i32 0, i32 0
  %971 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %970, align 8
  %972 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %971, i64 %965
  store %"struct.std::__1::__hash_node_base"* %961, %"struct.std::__1::__hash_node_base"** %972, align 8
  store %"class.std::__1::unique_ptr.94"* %294, %"class.std::__1::unique_ptr.94"** %197, align 8
  %973 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %197, align 8
  %974 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %973, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %974, %"class.std::__1::__compressed_pair.95"** %196, align 8
  %975 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %196, align 8
  %976 = bitcast %"class.std::__1::__compressed_pair.95"* %975 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %976, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %195, align 8
  %977 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %195, align 8
  %978 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %977, i32 0, i32 0
  %979 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %978, align 8
  %980 = bitcast %"struct.std::__1::__hash_node"* %979 to %"struct.std::__1::__hash_node_base"*
  %981 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %980, i32 0, i32 0
  %982 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %981, align 8
  %983 = icmp ne %"struct.std::__1::__hash_node_base"* %982, null
  br i1 %983, label %984, label %1046

; <label>:984:                                    ; preds = %925
  store %"class.std::__1::unique_ptr.94"* %294, %"class.std::__1::unique_ptr.94"** %200, align 8
  %985 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %200, align 8
  %986 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %985, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %986, %"class.std::__1::__compressed_pair.95"** %199, align 8
  %987 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %199, align 8
  %988 = bitcast %"class.std::__1::__compressed_pair.95"* %987 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %988, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %198, align 8
  %989 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %198, align 8
  %990 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %989, i32 0, i32 0
  %991 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %990, align 8
  %992 = bitcast %"struct.std::__1::__hash_node"* %991 to %"struct.std::__1::__hash_node_base"*
  store %"struct.std::__1::__hash_node_base"* %992, %"struct.std::__1::__hash_node_base"** %203, align 8
  %993 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %203, align 8
  store %"struct.std::__1::__hash_node_base"* %993, %"struct.std::__1::__hash_node_base"** %202, align 8
  %994 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %202, align 8
  store %"struct.std::__1::__hash_node_base"* %994, %"struct.std::__1::__hash_node_base"** %201, align 8
  %995 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %201, align 8
  %996 = bitcast %"struct.std::__1::__hash_node_base"* %995 to i8*
  %997 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %346, i32 0, i32 0
  store %"class.std::__1::unique_ptr.94"* %294, %"class.std::__1::unique_ptr.94"** %206, align 8
  %998 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %206, align 8
  %999 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %998, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %999, %"class.std::__1::__compressed_pair.95"** %205, align 8
  %1000 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %205, align 8
  %1001 = bitcast %"class.std::__1::__compressed_pair.95"* %1000 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1001, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %204, align 8
  %1002 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %204, align 8
  %1003 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1002, i32 0, i32 0
  %1004 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %1003, align 8
  %1005 = bitcast %"struct.std::__1::__hash_node"* %1004 to %"struct.std::__1::__hash_node_base"*
  %1006 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %1005, i32 0, i32 0
  %1007 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %1006, align 8
  store %"struct.std::__1::__hash_node_base"* %1007, %"struct.std::__1::__hash_node_base"** %207, align 8
  %1008 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %207, align 8
  %1009 = bitcast %"struct.std::__1::__hash_node_base"* %1008 to %"struct.std::__1::__hash_node"*
  %1010 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %1009, i32 0, i32 1
  %1011 = load i64, i64* %1010, align 8
  %1012 = load i64, i64* %290, align 8
  store i64 %1011, i64* %208, align 8
  store i64 %1012, i64* %209, align 8
  %1013 = load i64, i64* %209, align 8
  %1014 = load i64, i64* %209, align 8
  %1015 = sub i64 %1014, 1
  %1016 = and i64 %1013, %1015
  %1017 = icmp ne i64 %1016, 0
  br i1 %1017, label %1023, label %1018

; <label>:1018:                                   ; preds = %984
  %1019 = load i64, i64* %208, align 8
  %1020 = load i64, i64* %209, align 8
  %1021 = sub i64 %1020, 1
  %1022 = and i64 %1019, %1021
  br label %1035

; <label>:1023:                                   ; preds = %984
  %1024 = load i64, i64* %208, align 8
  %1025 = load i64, i64* %209, align 8
  %1026 = icmp ult i64 %1024, %1025
  br i1 %1026, label %1027, label %1029

; <label>:1027:                                   ; preds = %1023
  %1028 = load i64, i64* %208, align 8
  br label %1033

; <label>:1029:                                   ; preds = %1023
  %1030 = load i64, i64* %208, align 8
  %1031 = load i64, i64* %209, align 8
  %1032 = urem i64 %1030, %1031
  br label %1033

; <label>:1033:                                   ; preds = %1029, %1027
  %1034 = phi i64 [ %1028, %1027 ], [ %1032, %1029 ]
  br label %1035

; <label>:1035:                                   ; preds = %1033, %1018
  %1036 = phi i64 [ %1022, %1018 ], [ %1034, %1033 ]
  store %"class.std::__1::unique_ptr.37"* %997, %"class.std::__1::unique_ptr.37"** %212, align 8
  store i64 %1036, i64* %213, align 8
  %1037 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %212, align 8
  %1038 = load i64, i64* %213, align 8
  %1039 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %1037, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %1039, %"class.std::__1::__compressed_pair.38"** %211, align 8
  %1040 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %211, align 8
  %1041 = bitcast %"class.std::__1::__compressed_pair.38"* %1040 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %1041, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %210, align 8
  %1042 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %210, align 8
  %1043 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %1042, i32 0, i32 0
  %1044 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %1043, align 8
  %1045 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %1044, i64 %1038
  store %"struct.std::__1::__hash_node_base"* %995, %"struct.std::__1::__hash_node_base"** %1045, align 8
  br label %1046

; <label>:1046:                                   ; preds = %1035, %925
  br label %1070

; <label>:1047:                                   ; preds = %910
  %1048 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %299, align 8
  %1049 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %1048, i32 0, i32 0
  %1050 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %1049, align 8
  store %"class.std::__1::unique_ptr.94"* %294, %"class.std::__1::unique_ptr.94"** %216, align 8
  %1051 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %216, align 8
  %1052 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %1051, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %1052, %"class.std::__1::__compressed_pair.95"** %215, align 8
  %1053 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %215, align 8
  %1054 = bitcast %"class.std::__1::__compressed_pair.95"* %1053 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1054, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %214, align 8
  %1055 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %214, align 8
  %1056 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1055, i32 0, i32 0
  %1057 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %1056, align 8
  %1058 = bitcast %"struct.std::__1::__hash_node"* %1057 to %"struct.std::__1::__hash_node_base"*
  %1059 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %1058, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %1050, %"struct.std::__1::__hash_node_base"** %1059, align 8
  store %"class.std::__1::unique_ptr.94"* %294, %"class.std::__1::unique_ptr.94"** %219, align 8
  %1060 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %219, align 8
  %1061 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %1060, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %1061, %"class.std::__1::__compressed_pair.95"** %218, align 8
  %1062 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %218, align 8
  %1063 = bitcast %"class.std::__1::__compressed_pair.95"* %1062 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1063, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %217, align 8
  %1064 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %217, align 8
  %1065 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1064, i32 0, i32 0
  %1066 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %1065, align 8
  %1067 = bitcast %"struct.std::__1::__hash_node"* %1066 to %"struct.std::__1::__hash_node_base"*
  %1068 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %299, align 8
  %1069 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %1068, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %1067, %"struct.std::__1::__hash_node_base"** %1069, align 8
  br label %1070

; <label>:1070:                                   ; preds = %1047, %1046
  store %"class.std::__1::unique_ptr.94"* %294, %"class.std::__1::unique_ptr.94"** %224, align 8
  %1071 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %224, align 8
  %1072 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %1071, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %1072, %"class.std::__1::__compressed_pair.95"** %223, align 8
  %1073 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %223, align 8
  %1074 = bitcast %"class.std::__1::__compressed_pair.95"* %1073 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1074, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %222, align 8
  %1075 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %222, align 8
  %1076 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1075, i32 0, i32 0
  %1077 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %1076, align 8
  store %"struct.std::__1::__hash_node"* %1077, %"struct.std::__1::__hash_node"** %225, align 8
  %1078 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %1071, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %1078, %"class.std::__1::__compressed_pair.95"** %221, align 8
  %1079 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %221, align 8
  %1080 = bitcast %"class.std::__1::__compressed_pair.95"* %1079 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1080, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %220, align 8
  %1081 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %220, align 8
  %1082 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1081, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %1082, align 8
  %1083 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %225, align 8
  %1084 = bitcast %"struct.std::__1::__hash_node"* %1083 to %"struct.std::__1::__hash_node_base"*
  store %"struct.std::__1::__hash_node_base"* %1084, %"struct.std::__1::__hash_node_base"** %292, align 8
  store %"class.std::__1::__hash_table"* %346, %"class.std::__1::__hash_table"** %228, align 8
  %1085 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %228, align 8
  %1086 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %1085, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.48"* %1086, %"class.std::__1::__compressed_pair.48"** %227, align 8
  %1087 = load %"class.std::__1::__compressed_pair.48"*, %"class.std::__1::__compressed_pair.48"** %227, align 8
  %1088 = bitcast %"class.std::__1::__compressed_pair.48"* %1087 to %"class.std::__1::__libcpp_compressed_pair_imp.49"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.49"* %1088, %"class.std::__1::__libcpp_compressed_pair_imp.49"** %226, align 8
  %1089 = load %"class.std::__1::__libcpp_compressed_pair_imp.49"*, %"class.std::__1::__libcpp_compressed_pair_imp.49"** %226, align 8
  %1090 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.49", %"class.std::__1::__libcpp_compressed_pair_imp.49"* %1089, i32 0, i32 0
  %1091 = load i64, i64* %1090, align 8
  %1092 = add i64 %1091, 1
  store i64 %1092, i64* %1090, align 8
  store i8 1, i8* %291, align 1
  store %"class.std::__1::unique_ptr.94"* %294, %"class.std::__1::unique_ptr.94"** %267, align 8
  %1093 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %267, align 8
  store %"class.std::__1::unique_ptr.94"* %1093, %"class.std::__1::unique_ptr.94"** %266, align 8
  %1094 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %266, align 8
  store %"class.std::__1::unique_ptr.94"* %1094, %"class.std::__1::unique_ptr.94"** %263, align 8
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %264, align 8
  %1095 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %263, align 8
  %1096 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %1095, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %1096, %"class.std::__1::__compressed_pair.95"** %262, align 8
  %1097 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %262, align 8
  %1098 = bitcast %"class.std::__1::__compressed_pair.95"* %1097 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1098, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %261, align 8
  %1099 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %261, align 8
  %1100 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1099, i32 0, i32 0
  %1101 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %1100, align 8
  store %"struct.std::__1::__hash_node"* %1101, %"struct.std::__1::__hash_node"** %265, align 8
  %1102 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %264, align 8
  %1103 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %1095, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %1103, %"class.std::__1::__compressed_pair.95"** %243, align 8
  %1104 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %243, align 8
  %1105 = bitcast %"class.std::__1::__compressed_pair.95"* %1104 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1105, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %242, align 8
  %1106 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %242, align 8
  %1107 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1106, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* %1102, %"struct.std::__1::__hash_node"** %1107, align 8
  %1108 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %265, align 8
  %1109 = icmp ne %"struct.std::__1::__hash_node"* %1108, null
  br i1 %1109, label %1110, label %1149

; <label>:1110:                                   ; preds = %1070
  %1111 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %1095, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %1111, %"class.std::__1::__compressed_pair.95"** %241, align 8
  %1112 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %241, align 8
  %1113 = bitcast %"class.std::__1::__compressed_pair.95"* %1112 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1113, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %240, align 8
  %1114 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %240, align 8
  %1115 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %1114, i32 0, i32 1
  %1116 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %265, align 8
  store %"class.std::__1::__hash_node_destructor"* %1115, %"class.std::__1::__hash_node_destructor"** %259, align 8
  store %"struct.std::__1::__hash_node"* %1116, %"struct.std::__1::__hash_node"** %260, align 8
  %1117 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %259, align 8
  %1118 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %1117, i32 0, i32 1
  %1119 = load i8, i8* %1118, align 8
  %1120 = trunc i8 %1119 to i1
  br i1 %1120, label %1121, label %1134

; <label>:1121:                                   ; preds = %1110
  %1122 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %1117, i32 0, i32 0
  %1123 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %1122, align 8
  %1124 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %260, align 8
  %1125 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %1124, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %1125, %"union.std::__1::__hash_value_type"** %258, align 8
  %1126 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %258, align 8
  %1127 = bitcast %"union.std::__1::__hash_value_type"* %1126 to %"struct.std::__1::pair.92"*
  store %"struct.std::__1::pair.92"* %1127, %"struct.std::__1::pair.92"** %257, align 8
  %1128 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %257, align 8
  %1129 = bitcast %"struct.std::__1::pair.92"* %1128 to i8*
  store %"class.std::__1::allocator.46"* %1123, %"class.std::__1::allocator.46"** %253, align 8
  store %"struct.std::__1::pair.92"* %1128, %"struct.std::__1::pair.92"** %254, align 8
  %1130 = bitcast %"struct.std::__1::__has_destroy"* %256 to %"struct.std::__1::integral_constant.99"*
  %1131 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %253, align 8
  %1132 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %254, align 8
  store %"class.std::__1::allocator.46"* %1131, %"class.std::__1::allocator.46"** %251, align 8
  store %"struct.std::__1::pair.92"* %1132, %"struct.std::__1::pair.92"** %252, align 8
  %1133 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %252, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEED1Ev(%"struct.std::__1::pair.92"* %1133) #14
  br label %1134

; <label>:1134:                                   ; preds = %1121, %1110
  %1135 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %260, align 8
  %1136 = icmp ne %"struct.std::__1::__hash_node"* %1135, null
  br i1 %1136, label %1137, label %1148

; <label>:1137:                                   ; preds = %1134
  %1138 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %1117, i32 0, i32 0
  %1139 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %1138, align 8
  %1140 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %260, align 8
  store %"class.std::__1::allocator.46"* %1139, %"class.std::__1::allocator.46"** %248, align 8
  store %"struct.std::__1::__hash_node"* %1140, %"struct.std::__1::__hash_node"** %249, align 8
  store i64 1, i64* %250, align 8
  %1141 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %248, align 8
  %1142 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %249, align 8
  %1143 = load i64, i64* %250, align 8
  store %"class.std::__1::allocator.46"* %1141, %"class.std::__1::allocator.46"** %245, align 8
  store %"struct.std::__1::__hash_node"* %1142, %"struct.std::__1::__hash_node"** %246, align 8
  store i64 %1143, i64* %247, align 8
  %1144 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %245, align 8
  %1145 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %246, align 8
  %1146 = bitcast %"struct.std::__1::__hash_node"* %1145 to i8*
  store i8* %1146, i8** %244, align 8
  %1147 = load i8*, i8** %244, align 8
  call void @_ZdlPv(i8* %1147) #17
  br label %1148

; <label>:1148:                                   ; preds = %1137, %1134
  br label %1149

; <label>:1149:                                   ; preds = %1148, %1070
  br label %1150

; <label>:1150:                                   ; preds = %710, %1149
  %1151 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %292, align 8
  store %"class.std::__1::__hash_iterator"* %300, %"class.std::__1::__hash_iterator"** %270, align 8
  store %"struct.std::__1::__hash_node_base"* %1151, %"struct.std::__1::__hash_node_base"** %271, align 8
  %1152 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %270, align 8
  %1153 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %271, align 8
  store %"class.std::__1::__hash_iterator"* %1152, %"class.std::__1::__hash_iterator"** %268, align 8
  store %"struct.std::__1::__hash_node_base"* %1153, %"struct.std::__1::__hash_node_base"** %269, align 8
  %1154 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %268, align 8
  %1155 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %1154, i32 0, i32 0
  %1156 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %269, align 8
  store %"struct.std::__1::__hash_node_base"* %1156, %"struct.std::__1::__hash_node_base"** %1155, align 8
  store %"struct.std::__1::pair"* %283, %"struct.std::__1::pair"** %277, align 8
  store %"class.std::__1::__hash_iterator"* %300, %"class.std::__1::__hash_iterator"** %278, align 8
  store i8* %291, i8** %279, align 8
  %1157 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %277, align 8
  %1158 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %278, align 8
  %1159 = load i8*, i8** %279, align 8
  store %"struct.std::__1::pair"* %1157, %"struct.std::__1::pair"** %274, align 8
  store %"class.std::__1::__hash_iterator"* %1158, %"class.std::__1::__hash_iterator"** %275, align 8
  store i8* %1159, i8** %276, align 8
  %1160 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %274, align 8
  %1161 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %1160, i32 0, i32 0
  %1162 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %275, align 8
  store %"class.std::__1::__hash_iterator"* %1162, %"class.std::__1::__hash_iterator"** %273, align 8
  %1163 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %273, align 8
  %1164 = bitcast %"class.std::__1::__hash_iterator"* %1161 to i8*
  %1165 = bitcast %"class.std::__1::__hash_iterator"* %1163 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1164, i8* %1165, i64 8, i32 8, i1 false) #14
  %1166 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %1160, i32 0, i32 1
  %1167 = load i8*, i8** %276, align 8
  store i8* %1167, i8** %272, align 8
  %1168 = load i8*, i8** %272, align 8
  %1169 = load i8, i8* %1168, align 1
  %1170 = trunc i8 %1169 to i1
  %1171 = zext i1 %1170 to i8
  store i8 %1171, i8* %1166, align 8
  %1172 = bitcast %"struct.std::__1::pair"* %283 to { %"struct.std::__1::__hash_node_base"*, i8 }*
  %1173 = load { %"struct.std::__1::__hash_node_base"*, i8 }, { %"struct.std::__1::__hash_node_base"*, i8 }* %1172, align 8
  %1174 = bitcast %"struct.std::__1::pair"* %313 to { %"struct.std::__1::__hash_node_base"*, i8 }*
  %1175 = getelementptr inbounds { %"struct.std::__1::__hash_node_base"*, i8 }, { %"struct.std::__1::__hash_node_base"*, i8 }* %1174, i32 0, i32 0
  %1176 = extractvalue { %"struct.std::__1::__hash_node_base"*, i8 } %1173, 0
  store %"struct.std::__1::__hash_node_base"* %1176, %"struct.std::__1::__hash_node_base"** %1175, align 8
  %1177 = getelementptr inbounds { %"struct.std::__1::__hash_node_base"*, i8 }, { %"struct.std::__1::__hash_node_base"*, i8 }* %1174, i32 0, i32 1
  %1178 = extractvalue { %"struct.std::__1::__hash_node_base"*, i8 } %1173, 1
  store i8 %1178, i8* %1177, align 8
  %1179 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %313, i32 0, i32 0
  store %"class.std::__1::__hash_iterator"* %1179, %"class.std::__1::__hash_iterator"** %306, align 8
  %1180 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %306, align 8
  %1181 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %1180, i32 0, i32 0
  %1182 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %1181, align 8
  store %"struct.std::__1::__hash_node_base"* %1182, %"struct.std::__1::__hash_node_base"** %305, align 8
  %1183 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %305, align 8
  store %"struct.std::__1::__hash_node_base"* %1183, %"struct.std::__1::__hash_node_base"** %304, align 8
  %1184 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %304, align 8
  store %"struct.std::__1::__hash_node_base"* %1184, %"struct.std::__1::__hash_node_base"** %303, align 8
  %1185 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %303, align 8
  %1186 = bitcast %"struct.std::__1::__hash_node_base"* %1185 to i8*
  %1187 = bitcast %"struct.std::__1::__hash_node_base"* %1185 to %"struct.std::__1::__hash_node"*
  %1188 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %1187, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %1188, %"union.std::__1::__hash_value_type"** %302, align 8
  %1189 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %302, align 8
  store %"union.std::__1::__hash_value_type"* %1189, %"union.std::__1::__hash_value_type"** %301, align 8
  %1190 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %301, align 8
  %1191 = bitcast %"union.std::__1::__hash_value_type"* %1190 to i8*
  %1192 = bitcast %"union.std::__1::__hash_value_type"* %1190 to %"struct.std::__1::pair.92"*
  %1193 = getelementptr inbounds %"struct.std::__1::pair.92", %"struct.std::__1::pair.92"* %1192, i32 0, i32 1
  ret %"class.llvm::Constant"** %1193
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm8ArrayRefIPNS_5ValueEEC1ERKSt16initializer_listIS2_E(%"class.llvm::ArrayRef.36"*, %"class.std::initializer_list"* dereferenceable(16)) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::ArrayRef.36"*, align 8
  %4 = alloca %"class.std::initializer_list"*, align 8
  store %"class.llvm::ArrayRef.36"* %0, %"class.llvm::ArrayRef.36"** %3, align 8
  store %"class.std::initializer_list"* %1, %"class.std::initializer_list"** %4, align 8
  %5 = load %"class.llvm::ArrayRef.36"*, %"class.llvm::ArrayRef.36"** %3, align 8
  %6 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %4, align 8
  call void @_ZN4llvm8ArrayRefIPNS_5ValueEEC2ERKSt16initializer_listIS2_E(%"class.llvm::ArrayRef.36"* %5, %"class.std::initializer_list"* dereferenceable(16) %6)
  ret void
}

; Function Attrs: nounwind
declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"*) unnamed_addr #2

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE13CreateBitCastEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Type"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %5 = alloca %"class.llvm::IRBuilder"*, align 8
  %6 = alloca %"class.llvm::Value"*, align 8
  %7 = alloca %"class.llvm::Type"*, align 8
  %8 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %5, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %6, align 8
  store %"class.llvm::Type"* %2, %"class.llvm::Type"** %7, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %8, align 8
  %9 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %5, align 8
  %10 = load %"class.llvm::Value"*, %"class.llvm::Value"** %6, align 8
  %11 = load %"class.llvm::Type"*, %"class.llvm::Type"** %7, align 8
  %12 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %8, align 8
  %13 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCastENS_11Instruction7CastOpsEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %9, i32 47, %"class.llvm::Value"* %10, %"class.llvm::Type"* %11, %"class.llvm::Twine"* dereferenceable(24) %12)
  ret %"class.llvm::Value"* %13
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::BasicBlock"* @_ZNK4llvm13IRBuilderBase14GetInsertBlockEv(%"class.llvm::IRBuilderBase"*) #4 align 2 {
  %2 = alloca %"class.llvm::IRBuilderBase"*, align 8
  store %"class.llvm::IRBuilderBase"* %0, %"class.llvm::IRBuilderBase"** %2, align 8
  %3 = load %"class.llvm::IRBuilderBase"*, %"class.llvm::IRBuilderBase"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %3, i32 0, i32 1
  %5 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %4, align 8
  ret %"class.llvm::BasicBlock"* %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm3isaINS_8ConstantEPNS_5ValueEEEbRKT0_(%"class.llvm::Value"** dereferenceable(8)) #3 {
  %2 = alloca %"class.llvm::Value"**, align 8
  store %"class.llvm::Value"** %0, %"class.llvm::Value"*** %2, align 8
  %3 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_8ConstantEKPNS_5ValueEPKS2_E4doitERS4_(%"class.llvm::Value"** dereferenceable(8) %3)
  ret i1 %4
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm13IRBuilderBase14SetInsertPointEPNS_10BasicBlockE(%"class.llvm::IRBuilderBase"*, %"class.llvm::BasicBlock"*) #0 align 2 {
  %3 = alloca %"class.llvm::IRBuilderBase"*, align 8
  %4 = alloca %"class.llvm::BasicBlock"*, align 8
  %5 = alloca %"class.llvm::ilist_iterator", align 8
  store %"class.llvm::IRBuilderBase"* %0, %"class.llvm::IRBuilderBase"** %3, align 8
  store %"class.llvm::BasicBlock"* %1, %"class.llvm::BasicBlock"** %4, align 8
  %6 = load %"class.llvm::IRBuilderBase"*, %"class.llvm::IRBuilderBase"** %3, align 8
  %7 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %4, align 8
  %8 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %6, i32 0, i32 1
  store %"class.llvm::BasicBlock"* %7, %"class.llvm::BasicBlock"** %8, align 8
  %9 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %6, i32 0, i32 2
  %10 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %6, i32 0, i32 1
  %11 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %10, align 8
  %12 = call %"class.llvm::ilist_node_impl.0"* @_ZN4llvm10BasicBlock3endEv(%"class.llvm::BasicBlock"* %11)
  %13 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %5, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.0"* %12, %"class.llvm::ilist_node_impl.0"** %13, align 8
  %14 = bitcast %"class.llvm::ilist_iterator"* %9 to i8*
  %15 = bitcast %"class.llvm::ilist_iterator"* %5 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %14, i8* %15, i64 8, i32 8, i1 false)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::AllocaInst"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE12CreateAllocaEPNS_4TypeEPNS_5ValueERKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Type"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %5 = alloca %"class.llvm::IRBuilder"*, align 8
  %6 = alloca %"class.llvm::Type"*, align 8
  %7 = alloca %"class.llvm::Value"*, align 8
  %8 = alloca %"class.llvm::Twine"*, align 8
  %9 = alloca %"class.llvm::Twine", align 8
  %10 = alloca i8*
  %11 = alloca i32
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %5, align 8
  store %"class.llvm::Type"* %1, %"class.llvm::Type"** %6, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %7, align 8
  store %"class.llvm::Twine"* %3, %"class.llvm::Twine"** %8, align 8
  %12 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %5, align 8
  %13 = call i8* @_ZN4llvm16UnaryInstructionnwEm(i64 72)
  %14 = bitcast i8* %13 to %"class.llvm::AllocaInst"*
  %15 = load %"class.llvm::Type"*, %"class.llvm::Type"** %6, align 8
  %16 = load %"class.llvm::Value"*, %"class.llvm::Value"** %7, align 8
  invoke void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %9, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
          to label %17 unwind label %21

; <label>:17:                                     ; preds = %4
  invoke void @_ZN4llvm10AllocaInstC1EPNS_4TypeEPNS_5ValueERKNS_5TwineEPNS_11InstructionE(%"class.llvm::AllocaInst"* %14, %"class.llvm::Type"* %15, %"class.llvm::Value"* %16, %"class.llvm::Twine"* dereferenceable(24) %9, %"class.llvm::Instruction"* null)
          to label %18 unwind label %21

; <label>:18:                                     ; preds = %17
  %19 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %8, align 8
  %20 = call %"class.llvm::AllocaInst"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_10AllocaInstEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"* %12, %"class.llvm::AllocaInst"* %14, %"class.llvm::Twine"* dereferenceable(24) %19)
  ret %"class.llvm::AllocaInst"* %20

; <label>:21:                                     ; preds = %17, %4
  %22 = landingpad { i8*, i32 }
          cleanup
  %23 = extractvalue { i8*, i32 } %22, 0
  store i8* %23, i8** %10, align 8
  %24 = extractvalue { i8*, i32 } %22, 1
  store i32 %24, i32* %11, align 4
  call void @_ZN4llvm4UserdlEPv(i8* %13) #14
  br label %25

; <label>:25:                                     ; preds = %21
  %26 = load i8*, i8** %10, align 8
  %27 = load i32, i32* %11, align 4
  %28 = insertvalue { i8*, i32 } undef, i8* %26, 0
  %29 = insertvalue { i8*, i32 } %28, i32 %27, 1
  resume { i8*, i32 } %29
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm5TwineC1ERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE(%"class.llvm::Twine"*, %"class.std::__1::basic_string"* dereferenceable(24)) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::Twine"*, align 8
  %4 = alloca %"class.std::__1::basic_string"*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %3, align 8
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %4, align 8
  %5 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %3, align 8
  %6 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %4, align 8
  call void @_ZN4llvm5TwineC2ERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE(%"class.llvm::Twine"* %5, %"class.std::__1::basic_string"* dereferenceable(24) %6)
  ret void
}

; Function Attrs: ssp uwtable
define %"class.llvm::Value"* @_ZN8CGMemory12CreateAllocaER13TypeSpecifierNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEEPN4llvm5ValueEbPNS9_10BasicBlockE(%struct.TypeSpecifier* dereferenceable(216), %"class.std::__1::basic_string"*, %"class.llvm::Value"*, i1 zeroext, %"class.llvm::BasicBlock"*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %6 = alloca %struct.TypeSpecifier*, align 8
  %7 = alloca %"class.llvm::Value"*, align 8
  %8 = alloca i8, align 1
  %9 = alloca %"class.llvm::BasicBlock"*, align 8
  %10 = alloca %"class.std::__1::basic_string", align 8
  %11 = alloca i8*
  %12 = alloca i32
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %6, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %7, align 8
  %13 = zext i1 %3 to i8
  store i8 %13, i8* %8, align 1
  store %"class.llvm::BasicBlock"* %4, %"class.llvm::BasicBlock"** %9, align 8
  %14 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %6, align 8
  %15 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %14)
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %10, %"class.std::__1::basic_string"* dereferenceable(24) %1)
  %16 = load %"class.llvm::Value"*, %"class.llvm::Value"** %7, align 8
  %17 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %6, align 8
  %18 = invoke zeroext i16 @_ZN6CGType12getAlignmentER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %17)
          to label %19 unwind label %24

; <label>:19:                                     ; preds = %5
  %20 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %9, align 8
  %21 = icmp ne %"class.llvm::BasicBlock"* %20, null
  %22 = invoke %"class.llvm::Value"* @_ZN8CGMemory12CreateAllocaEPN4llvm4TypeENSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEPNS0_5ValueEtbPNS0_10BasicBlockE(%"class.llvm::Type"* %15, %"class.std::__1::basic_string"* %10, %"class.llvm::Value"* %16, i16 zeroext %18, i1 zeroext %21, %"class.llvm::BasicBlock"* null)
          to label %23 unwind label %24

; <label>:23:                                     ; preds = %19
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %10) #14
  ret %"class.llvm::Value"* %22

; <label>:24:                                     ; preds = %19, %5
  %25 = landingpad { i8*, i32 }
          cleanup
  %26 = extractvalue { i8*, i32 } %25, 0
  store i8* %26, i8** %11, align 8
  %27 = extractvalue { i8*, i32 } %25, 1
  store i32 %27, i32* %12, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %10) #14
  br label %28

; <label>:28:                                     ; preds = %24
  %29 = load i8*, i8** %11, align 8
  %30 = load i32, i32* %12, align 4
  %31 = insertvalue { i8*, i32 } undef, i8* %29, 0
  %32 = insertvalue { i8*, i32 } %31, i32 %30, 1
  resume { i8*, i32 } %32
}

declare %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216)) #1

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"*, %"class.std::__1::basic_string"* dereferenceable(24)) unnamed_addr #1

declare zeroext i16 @_ZN6CGType12getAlignmentER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216)) #1

declare i8* @_ZN4llvm4UsernwEmj(i64, i32) #1

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
  call void @__assert_rtn(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__._ZN4llvm5TwineC2EPKc, i32 0, i32 0), i8* getelementptr inbounds ([56 x i8], [56 x i8]* @.str.3, i32 0, i32 0), i32 274, i8* getelementptr inbounds ([30 x i8], [30 x i8]* @.str.4, i32 0, i32 0)) #15
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
declare void @__assert_rtn(i8*, i8*, i32, i8*) #5

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
define linkonce_odr zeroext i8 @_ZNK4llvm5Twine10getRHSKindEv(%"class.llvm::Twine"*) #4 align 2 {
  %2 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %2, align 8
  %3 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %3, i32 0, i32 3
  %5 = load i8, i8* %4, align 1
  ret i8 %5
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr zeroext i8 @_ZNK4llvm5Twine10getLHSKindEv(%"class.llvm::Twine"*) #4 align 2 {
  %2 = alloca %"class.llvm::Twine"*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %2, align 8
  %3 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %3, i32 0, i32 2
  %5 = load i8, i8* %4, align 8
  ret i8 %5
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZNK4llvm5Twine8isBinaryEv(%"class.llvm::Twine"*) #4 align 2 {
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

declare %"class.llvm::IntegerType"* @_ZN4llvm4Type10getInt64TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8)) #1

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.0"* @_ZN4llvm10BasicBlock3endEv(%"class.llvm::BasicBlock"*) #3 align 2 {
  %2 = alloca %"class.llvm::ilist_iterator", align 8
  %3 = alloca %"class.llvm::BasicBlock"*, align 8
  store %"class.llvm::BasicBlock"* %0, %"class.llvm::BasicBlock"** %3, align 8
  %4 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %3, align 8
  %5 = getelementptr inbounds %"class.llvm::BasicBlock", %"class.llvm::BasicBlock"* %4, i32 0, i32 2
  %6 = bitcast %"class.llvm::SymbolTableList"* %5 to %"class.llvm::iplist_impl"*
  %7 = bitcast %"class.llvm::iplist_impl"* %6 to %"class.llvm::simple_ilist"*
  %8 = call %"class.llvm::ilist_node_impl.0"* @_ZN4llvm12simple_ilistINS_11InstructionEJEE3endEv(%"class.llvm::simple_ilist"* %7)
  %9 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %2, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.0"* %8, %"class.llvm::ilist_node_impl.0"** %9, align 8
  %10 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %2, i32 0, i32 0
  %11 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %10, align 8
  ret %"class.llvm::ilist_node_impl.0"* %11
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #6

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.0"* @_ZN4llvm12simple_ilistINS_11InstructionEJEE3endEv(%"class.llvm::simple_ilist"*) #0 align 2 {
  %2 = alloca %"class.llvm::ilist_iterator", align 8
  %3 = alloca %"class.llvm::simple_ilist"*, align 8
  store %"class.llvm::simple_ilist"* %0, %"class.llvm::simple_ilist"** %3, align 8
  %4 = load %"class.llvm::simple_ilist"*, %"class.llvm::simple_ilist"** %3, align 8
  %5 = getelementptr inbounds %"class.llvm::simple_ilist", %"class.llvm::simple_ilist"* %4, i32 0, i32 0
  %6 = bitcast %"class.llvm::ilist_sentinel"* %5 to %"class.llvm::ilist_node_impl.0"*
  call void @_ZN4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EEC1ERNS_15ilist_node_implIS4_EE(%"class.llvm::ilist_iterator"* %2, %"class.llvm::ilist_node_impl.0"* dereferenceable(16) %6)
  %7 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %2, i32 0, i32 0
  %8 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %7, align 8
  ret %"class.llvm::ilist_node_impl.0"* %8
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EEC1ERNS_15ilist_node_implIS4_EE(%"class.llvm::ilist_iterator"*, %"class.llvm::ilist_node_impl.0"* dereferenceable(16)) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::ilist_iterator"*, align 8
  %4 = alloca %"class.llvm::ilist_node_impl.0"*, align 8
  store %"class.llvm::ilist_iterator"* %0, %"class.llvm::ilist_iterator"** %3, align 8
  store %"class.llvm::ilist_node_impl.0"* %1, %"class.llvm::ilist_node_impl.0"** %4, align 8
  %5 = load %"class.llvm::ilist_iterator"*, %"class.llvm::ilist_iterator"** %3, align 8
  %6 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %4, align 8
  call void @_ZN4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EEC2ERNS_15ilist_node_implIS4_EE(%"class.llvm::ilist_iterator"* %5, %"class.llvm::ilist_node_impl.0"* dereferenceable(16) %6)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EEC2ERNS_15ilist_node_implIS4_EE(%"class.llvm::ilist_iterator"*, %"class.llvm::ilist_node_impl.0"* dereferenceable(16)) unnamed_addr #4 align 2 {
  %3 = alloca %"class.llvm::ilist_iterator"*, align 8
  %4 = alloca %"class.llvm::ilist_node_impl.0"*, align 8
  store %"class.llvm::ilist_iterator"* %0, %"class.llvm::ilist_iterator"** %3, align 8
  store %"class.llvm::ilist_node_impl.0"* %1, %"class.llvm::ilist_node_impl.0"** %4, align 8
  %5 = load %"class.llvm::ilist_iterator"*, %"class.llvm::ilist_iterator"** %3, align 8
  %6 = bitcast %"class.llvm::ilist_iterator"* %5 to %"struct.llvm::ilist_detail::SpecificNodeAccess"*
  %7 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %5, i32 0, i32 0
  %8 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %4, align 8
  store %"class.llvm::ilist_node_impl.0"* %8, %"class.llvm::ilist_node_impl.0"** %7, align 8
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm5TwineC2ERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE(%"class.llvm::Twine"*, %"class.std::__1::basic_string"* dereferenceable(24)) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::Twine"*, align 8
  %4 = alloca %"class.std::__1::basic_string"*, align 8
  store %"class.llvm::Twine"* %0, %"class.llvm::Twine"** %3, align 8
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %4, align 8
  %5 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %3, align 8
  %6 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %5, i32 0, i32 0
  %7 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %5, i32 0, i32 1
  %8 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %5, i32 0, i32 2
  store i8 4, i8* %8, align 8
  %9 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %5, i32 0, i32 3
  store i8 1, i8* %9, align 1
  %10 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %4, align 8
  %11 = getelementptr inbounds %"class.llvm::Twine", %"class.llvm::Twine"* %5, i32 0, i32 0
  %12 = bitcast %"union.llvm::Twine::Child"* %11 to %"class.std::__1::basic_string"**
  store %"class.std::__1::basic_string"* %10, %"class.std::__1::basic_string"** %12, align 8
  %13 = call zeroext i1 @_ZNK4llvm5Twine7isValidEv(%"class.llvm::Twine"* %5)
  br i1 %13, label %14, label %15

; <label>:14:                                     ; preds = %2
  br label %15

; <label>:15:                                     ; preds = %14, %2
  %16 = phi i1 [ false, %2 ], [ true, %14 ]
  %17 = xor i1 %16, true
  br i1 %17, label %18, label %20

; <label>:18:                                     ; preds = %15
  call void @__assert_rtn(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__._ZN4llvm5TwineC2EPKc, i32 0, i32 0), i8* getelementptr inbounds ([56 x i8], [56 x i8]* @.str.3, i32 0, i32 0), i32 281, i8* getelementptr inbounds ([30 x i8], [30 x i8]* @.str.4, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %21

; <label>:20:                                     ; preds = %15
  br label %21

; <label>:21:                                     ; preds = %20, %19
  ret void
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
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 95, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.7, i32 0, i32 0)) #15
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
define linkonce_odr i32 @_ZNK4llvm5Value10getValueIDEv(%"class.llvm::Value"*) #4 align 2 {
  %2 = alloca %"class.llvm::Value"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %2, align 8
  %3 = load %"class.llvm::Value"*, %"class.llvm::Value"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Value", %"class.llvm::Value"* %3, i32 0, i32 3
  %5 = load i8, i8* %4, align 8
  %6 = zext i8 %5 to i32
  ret i32 %6
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr dereferenceable(8) %"class.llvm::Value"** @_ZN4llvm13simplify_typeIPNS_5ValueEE18getSimplifiedValueERS2_(%"class.llvm::Value"** dereferenceable(8)) #4 align 2 {
  %2 = alloca %"class.llvm::Value"**, align 8
  store %"class.llvm::Value"** %0, %"class.llvm::Value"*** %2, align 8
  %3 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %2, align 8
  ret %"class.llvm::Value"** %3
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::StructType"* @_ZN4llvm16cast_convert_valINS_10StructTypeEPNS_4TypeES3_E4doitERKS3_(%"class.llvm::Type"** dereferenceable(8)) #4 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  %3 = alloca %"class.llvm::StructType"*, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %4 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %5 = load %"class.llvm::Type"*, %"class.llvm::Type"** %4, align 8
  %6 = bitcast %"class.llvm::Type"* %5 to %"class.llvm::StructType"*
  store %"class.llvm::StructType"* %6, %"class.llvm::StructType"** %3, align 8
  %7 = load %"class.llvm::StructType"*, %"class.llvm::StructType"** %3, align 8
  ret %"class.llvm::StructType"* %7
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_10StructTypeEKPNS_4TypeEPKS2_E4doitERS4_(%"class.llvm::Type"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  %3 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %4 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %5 = call %"class.llvm::Type"* @_ZN4llvm13simplify_typeIKPNS_4TypeEE18getSimplifiedValueERS3_(%"class.llvm::Type"** dereferenceable(8) %4)
  store %"class.llvm::Type"* %5, %"class.llvm::Type"** %3, align 8
  %6 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_10StructTypeEPKNS_4TypeES4_E4doitERKS4_(%"class.llvm::Type"** dereferenceable(8) %3)
  ret i1 %6
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_10StructTypeEPKNS_4TypeES4_E4doitERKS4_(%"class.llvm::Type"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %3 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %4 = load %"class.llvm::Type"*, %"class.llvm::Type"** %3, align 8
  %5 = call zeroext i1 @_ZN4llvm11isa_impl_clINS_10StructTypeEPKNS_4TypeEE4doitES4_(%"class.llvm::Type"* %4)
  ret i1 %5
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Type"* @_ZN4llvm13simplify_typeIKPNS_4TypeEE18getSimplifiedValueERS3_(%"class.llvm::Type"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %3 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %4 = call dereferenceable(8) %"class.llvm::Type"** @_ZN4llvm13simplify_typeIPNS_4TypeEE18getSimplifiedValueERS2_(%"class.llvm::Type"** dereferenceable(8) %3)
  %5 = load %"class.llvm::Type"*, %"class.llvm::Type"** %4, align 8
  ret %"class.llvm::Type"* %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm11isa_impl_clINS_10StructTypeEPKNS_4TypeEE4doitES4_(%"class.llvm::Type"*) #3 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = icmp ne %"class.llvm::Type"* %3, null
  br i1 %4, label %5, label %6

; <label>:5:                                      ; preds = %1
  br label %6

; <label>:6:                                      ; preds = %5, %1
  %7 = phi i1 [ false, %1 ], [ true, %5 ]
  %8 = xor i1 %7, true
  br i1 %8, label %9, label %11

; <label>:9:                                      ; preds = %6
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 95, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.7, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %12

; <label>:11:                                     ; preds = %6
  br label %12

; <label>:12:                                     ; preds = %11, %10
  %13 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %14 = call zeroext i1 @_ZN4llvm8isa_implINS_10StructTypeENS_4TypeEvE4doitERKS2_(%"class.llvm::Type"* dereferenceable(24) %13)
  ret i1 %14
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm8isa_implINS_10StructTypeENS_4TypeEvE4doitERKS2_(%"class.llvm::Type"* dereferenceable(24)) #3 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm10StructType7classofEPKNS_4TypeE(%"class.llvm::Type"* %3)
  ret i1 %4
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm10StructType7classofEPKNS_4TypeE(%"class.llvm::Type"*) #3 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call i32 @_ZNK4llvm4Type9getTypeIDEv(%"class.llvm::Type"* %3)
  %5 = icmp eq i32 %4, 13
  ret i1 %5
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i32 @_ZNK4llvm4Type9getTypeIDEv(%"class.llvm::Type"*) #4 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Type", %"class.llvm::Type"* %3, i32 0, i32 1
  %5 = load i32, i32* %4, align 8
  %6 = and i32 %5, 255
  ret i32 %6
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr dereferenceable(8) %"class.llvm::Type"** @_ZN4llvm13simplify_typeIPNS_4TypeEE18getSimplifiedValueERS2_(%"class.llvm::Type"** dereferenceable(8)) #4 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %3 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  ret %"class.llvm::Type"** %3
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCastENS_11Instruction7CastOpsEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"*, i32, %"class.llvm::Value"*, %"class.llvm::Type"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %6 = alloca %"class.llvm::Value"*, align 8
  %7 = alloca %"class.llvm::IRBuilder"*, align 8
  %8 = alloca i32, align 4
  %9 = alloca %"class.llvm::Value"*, align 8
  %10 = alloca %"class.llvm::Type"*, align 8
  %11 = alloca %"class.llvm::Twine"*, align 8
  %12 = alloca %"class.llvm::Constant"*, align 8
  %13 = alloca %"class.llvm::Twine", align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %7, align 8
  store i32 %1, i32* %8, align 4
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %9, align 8
  store %"class.llvm::Type"* %3, %"class.llvm::Type"** %10, align 8
  store %"class.llvm::Twine"* %4, %"class.llvm::Twine"** %11, align 8
  %14 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %7, align 8
  %15 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %16 = call %"class.llvm::Type"* @_ZNK4llvm5Value7getTypeEv(%"class.llvm::Value"* %15)
  %17 = load %"class.llvm::Type"*, %"class.llvm::Type"** %10, align 8
  %18 = icmp eq %"class.llvm::Type"* %16, %17
  br i1 %18, label %19, label %21

; <label>:19:                                     ; preds = %5
  %20 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  store %"class.llvm::Value"* %20, %"class.llvm::Value"** %6, align 8
  br label %43

; <label>:21:                                     ; preds = %5
  %22 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %23 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %22)
  store %"class.llvm::Constant"* %23, %"class.llvm::Constant"** %12, align 8
  %24 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %25 = icmp ne %"class.llvm::Constant"* %24, null
  br i1 %25, label %26, label %35

; <label>:26:                                     ; preds = %21
  %27 = getelementptr inbounds %"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* %14, i32 0, i32 1
  %28 = load i32, i32* %8, align 4
  %29 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %30 = load %"class.llvm::Type"*, %"class.llvm::Type"** %10, align 8
  %31 = call %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateCastENS_11Instruction7CastOpsEPNS_8ConstantEPNS_4TypeE(%"class.llvm::ConstantFolder"* %27, i32 %28, %"class.llvm::Constant"* %29, %"class.llvm::Type"* %30)
  %32 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %11, align 8
  %33 = call %"class.llvm::Constant"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertEPNS_8ConstantERKNS_5TwineE(%"class.llvm::IRBuilder"* %14, %"class.llvm::Constant"* %31, %"class.llvm::Twine"* dereferenceable(24) %32)
  %34 = bitcast %"class.llvm::Constant"* %33 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %34, %"class.llvm::Value"** %6, align 8
  br label %43

; <label>:35:                                     ; preds = %21
  %36 = load i32, i32* %8, align 4
  %37 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %38 = load %"class.llvm::Type"*, %"class.llvm::Type"** %10, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %13, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %39 = call %"class.llvm::CastInst"* @_ZN4llvm8CastInst6CreateENS_11Instruction7CastOpsEPNS_5ValueEPNS_4TypeERKNS_5TwineEPS1_(i32 %36, %"class.llvm::Value"* %37, %"class.llvm::Type"* %38, %"class.llvm::Twine"* dereferenceable(24) %13, %"class.llvm::Instruction"* null)
  %40 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %11, align 8
  %41 = call %"class.llvm::CastInst"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_8CastInstEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"* %14, %"class.llvm::CastInst"* %39, %"class.llvm::Twine"* dereferenceable(24) %40)
  %42 = bitcast %"class.llvm::CastInst"* %41 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %42, %"class.llvm::Value"** %6, align 8
  br label %43

; <label>:43:                                     ; preds = %35, %26, %19
  %44 = load %"class.llvm::Value"*, %"class.llvm::Value"** %6, align 8
  ret %"class.llvm::Value"* %44
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::Type"* @_ZNK4llvm5Value7getTypeEv(%"class.llvm::Value"*) #4 align 2 {
  %2 = alloca %"class.llvm::Value"*, align 8
  store %"class.llvm::Value"* %0, %"class.llvm::Value"** %2, align 8
  %3 = load %"class.llvm::Value"*, %"class.llvm::Value"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Value", %"class.llvm::Value"* %3, i32 0, i32 1
  %5 = load %"class.llvm::Type"*, %"class.llvm::Type"** %4, align 8
  ret %"class.llvm::Type"* %5
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
define linkonce_odr %"class.llvm::Constant"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertEPNS_8ConstantERKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Constant"*, %"class.llvm::Twine"* dereferenceable(24)) #4 align 2 {
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
define linkonce_odr %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder10CreateCastENS_11Instruction7CastOpsEPNS_8ConstantEPNS_4TypeE(%"class.llvm::ConstantFolder"*, i32, %"class.llvm::Constant"*, %"class.llvm::Type"*) #0 align 2 {
  %5 = alloca %"class.llvm::ConstantFolder"*, align 8
  %6 = alloca i32, align 4
  %7 = alloca %"class.llvm::Constant"*, align 8
  %8 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::ConstantFolder"* %0, %"class.llvm::ConstantFolder"** %5, align 8
  store i32 %1, i32* %6, align 4
  store %"class.llvm::Constant"* %2, %"class.llvm::Constant"** %7, align 8
  store %"class.llvm::Type"* %3, %"class.llvm::Type"** %8, align 8
  %9 = load %"class.llvm::ConstantFolder"*, %"class.llvm::ConstantFolder"** %5, align 8
  %10 = load i32, i32* %6, align 4
  %11 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %7, align 8
  %12 = load %"class.llvm::Type"*, %"class.llvm::Type"** %8, align 8
  %13 = call %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getCastEjPNS_8ConstantEPNS_4TypeEb(i32 %10, %"class.llvm::Constant"* %11, %"class.llvm::Type"* %12, i1 zeroext false)
  ret %"class.llvm::Constant"* %13
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::CastInst"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_8CastInstEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::CastInst"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %4 = alloca %"class.llvm::IRBuilder"*, align 8
  %5 = alloca %"class.llvm::CastInst"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  %7 = alloca %"class.llvm::ilist_iterator", align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %4, align 8
  store %"class.llvm::CastInst"* %1, %"class.llvm::CastInst"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %8 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %4, align 8
  %9 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderDefaultInserter"*
  %10 = load %"class.llvm::CastInst"*, %"class.llvm::CastInst"** %5, align 8
  %11 = bitcast %"class.llvm::CastInst"* %10 to %"class.llvm::Instruction"*
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
  %21 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %20, align 8
  call void @_ZNK4llvm24IRBuilderDefaultInserter12InsertHelperEPNS_11InstructionERKNS_5TwineEPNS_10BasicBlockENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEE(%"class.llvm::IRBuilderDefaultInserter"* %9, %"class.llvm::Instruction"* %11, %"class.llvm::Twine"* dereferenceable(24) %12, %"class.llvm::BasicBlock"* %15, %"class.llvm::ilist_node_impl.0"* %21)
  %22 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderBase"*
  %23 = load %"class.llvm::CastInst"*, %"class.llvm::CastInst"** %5, align 8
  %24 = bitcast %"class.llvm::CastInst"* %23 to %"class.llvm::Instruction"*
  call void @_ZNK4llvm13IRBuilderBase20SetInstDebugLocationEPNS_11InstructionE(%"class.llvm::IRBuilderBase"* %22, %"class.llvm::Instruction"* %24)
  %25 = load %"class.llvm::CastInst"*, %"class.llvm::CastInst"** %5, align 8
  ret %"class.llvm::CastInst"* %25
}

declare %"class.llvm::CastInst"* @_ZN4llvm8CastInst6CreateENS_11Instruction7CastOpsEPNS_5ValueEPNS_4TypeERKNS_5TwineEPS1_(i32, %"class.llvm::Value"*, %"class.llvm::Type"*, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::Instruction"*) #1

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
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm4castINS_10StructTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 236, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.9, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %11

; <label>:10:                                     ; preds = %5
  br label %11

; <label>:11:                                     ; preds = %10, %9
  %12 = call %"class.llvm::Constant"* @_ZN4llvm16cast_convert_valINS_8ConstantEPNS_5ValueES3_E4doitERKS3_(%"class.llvm::Value"** dereferenceable(8) %2)
  ret %"class.llvm::Constant"* %12
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZN4llvm16cast_convert_valINS_8ConstantEPNS_5ValueES3_E4doitERKS3_(%"class.llvm::Value"** dereferenceable(8)) #4 align 2 {
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

declare %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr7getCastEjPNS_8ConstantEPNS_4TypeEb(i32, %"class.llvm::Constant"*, %"class.llvm::Type"*, i1 zeroext) #1

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNK4llvm24IRBuilderDefaultInserter12InsertHelperEPNS_11InstructionERKNS_5TwineEPNS_10BasicBlockENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEE(%"class.llvm::IRBuilderDefaultInserter"*, %"class.llvm::Instruction"*, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::BasicBlock"*, %"class.llvm::ilist_node_impl.0"*) #0 align 2 {
  %6 = alloca %"class.llvm::ilist_iterator", align 8
  %7 = alloca %"class.llvm::IRBuilderDefaultInserter"*, align 8
  %8 = alloca %"class.llvm::Instruction"*, align 8
  %9 = alloca %"class.llvm::Twine"*, align 8
  %10 = alloca %"class.llvm::BasicBlock"*, align 8
  %11 = alloca %"class.llvm::ilist_iterator", align 8
  %12 = alloca %"class.llvm::ilist_iterator", align 8
  %13 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %6, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.0"* %4, %"class.llvm::ilist_node_impl.0"** %13, align 8
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
  %25 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %24, align 8
  %26 = call %"class.llvm::ilist_node_impl.0"* @_ZN4llvm11iplist_implINS_12simple_ilistINS_11InstructionEJEEENS_21SymbolTableListTraitsIS2_EEE6insertENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS2_Lb0ELb0EvEELb0ELb0EEEPS2_(%"class.llvm::iplist_impl"* %20, %"class.llvm::ilist_node_impl.0"* %25, %"class.llvm::Instruction"* %23)
  %27 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %12, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.0"* %26, %"class.llvm::ilist_node_impl.0"** %27, align 8
  br label %28

; <label>:28:                                     ; preds = %17, %5
  %29 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %8, align 8
  %30 = bitcast %"class.llvm::Instruction"* %29 to %"class.llvm::Value"*
  %31 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %9, align 8
  call void @_ZN4llvm5Value7setNameERKNS_5TwineE(%"class.llvm::Value"* %30, %"class.llvm::Twine"* dereferenceable(24) %31)
  ret void
}

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
  call void @_ZN4llvm8DebugLocD1Ev(%"class.llvm::DebugLoc"* %5) #14
  br label %19

; <label>:15:                                     ; preds = %11
  %16 = landingpad { i8*, i32 }
          cleanup
  %17 = extractvalue { i8*, i32 } %16, 0
  store i8* %17, i8** %6, align 8
  %18 = extractvalue { i8*, i32 } %16, 1
  store i32 %18, i32* %7, align 4
  call void @_ZN4llvm8DebugLocD1Ev(%"class.llvm::DebugLoc"* %5) #14
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
define linkonce_odr dereferenceable(16) %"class.llvm::SymbolTableList"* @_ZN4llvm10BasicBlock11getInstListEv(%"class.llvm::BasicBlock"*) #4 align 2 {
  %2 = alloca %"class.llvm::BasicBlock"*, align 8
  store %"class.llvm::BasicBlock"* %0, %"class.llvm::BasicBlock"** %2, align 8
  %3 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::BasicBlock", %"class.llvm::BasicBlock"* %3, i32 0, i32 2
  ret %"class.llvm::SymbolTableList"* %4
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.0"* @_ZN4llvm11iplist_implINS_12simple_ilistINS_11InstructionEJEEENS_21SymbolTableListTraitsIS2_EEE6insertENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS2_Lb0ELb0EvEELb0ELb0EEEPS2_(%"class.llvm::iplist_impl"*, %"class.llvm::ilist_node_impl.0"*, %"class.llvm::Instruction"*) #0 align 2 {
  %4 = alloca %"class.llvm::ilist_iterator", align 8
  %5 = alloca %"class.llvm::ilist_iterator", align 8
  %6 = alloca %"class.llvm::iplist_impl"*, align 8
  %7 = alloca %"class.llvm::Instruction"*, align 8
  %8 = alloca %"class.llvm::ilist_iterator", align 8
  %9 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %5, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.0"* %1, %"class.llvm::ilist_node_impl.0"** %9, align 8
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
  %18 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %17, align 8
  %19 = call %"class.llvm::ilist_node_impl.0"* @_ZN4llvm12simple_ilistINS_11InstructionEJEE6insertENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEERS1_(%"class.llvm::simple_ilist"* %13, %"class.llvm::ilist_node_impl.0"* %18, %"class.llvm::Instruction"* dereferenceable(64) %16)
  %20 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %4, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.0"* %19, %"class.llvm::ilist_node_impl.0"** %20, align 8
  %21 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %4, i32 0, i32 0
  %22 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %21, align 8
  ret %"class.llvm::ilist_node_impl.0"* %22
}

declare void @_ZN4llvm5Value7setNameERKNS_5TwineE(%"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #1

declare void @_ZN4llvm21SymbolTableListTraitsINS_11InstructionEE13addNodeToListEPS1_(%"class.llvm::SymbolTableListTraits"*, %"class.llvm::Instruction"*) #1

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.0"* @_ZN4llvm12simple_ilistINS_11InstructionEJEE6insertENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEERS1_(%"class.llvm::simple_ilist"*, %"class.llvm::ilist_node_impl.0"*, %"class.llvm::Instruction"* dereferenceable(64)) #0 align 2 {
  %4 = alloca %"class.llvm::ilist_iterator", align 8
  %5 = alloca %"class.llvm::ilist_iterator", align 8
  %6 = alloca %"class.llvm::simple_ilist"*, align 8
  %7 = alloca %"class.llvm::Instruction"*, align 8
  %8 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %5, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.0"* %1, %"class.llvm::ilist_node_impl.0"** %8, align 8
  store %"class.llvm::simple_ilist"* %0, %"class.llvm::simple_ilist"** %6, align 8
  store %"class.llvm::Instruction"* %2, %"class.llvm::Instruction"** %7, align 8
  %9 = load %"class.llvm::simple_ilist"*, %"class.llvm::simple_ilist"** %6, align 8
  %10 = call %"class.llvm::ilist_node_impl.0"* @_ZNK4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EE10getNodePtrEv(%"class.llvm::ilist_iterator"* %5)
  %11 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %7, align 8
  %12 = call %"class.llvm::ilist_node_impl.0"* @_ZN4llvm12ilist_detail18SpecificNodeAccessINS0_12node_optionsINS_11InstructionELb0ELb0EvEEE10getNodePtrEPS3_(%"class.llvm::Instruction"* %11)
  call void @_ZN4llvm10ilist_baseILb0EE12insertBeforeINS_15ilist_node_implINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEEEEEEvRT_SA_(%"class.llvm::ilist_node_impl.0"* dereferenceable(16) %10, %"class.llvm::ilist_node_impl.0"* dereferenceable(16) %12)
  %13 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %7, align 8
  call void @_ZN4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EEC1EPS3_(%"class.llvm::ilist_iterator"* %4, %"class.llvm::Instruction"* %13)
  %14 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %4, i32 0, i32 0
  %15 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %14, align 8
  ret %"class.llvm::ilist_node_impl.0"* %15
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm10ilist_baseILb0EE12insertBeforeINS_15ilist_node_implINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEEEEEEvRT_SA_(%"class.llvm::ilist_node_impl.0"* dereferenceable(16), %"class.llvm::ilist_node_impl.0"* dereferenceable(16)) #0 align 2 {
  %3 = alloca %"class.llvm::ilist_node_impl.0"*, align 8
  %4 = alloca %"class.llvm::ilist_node_impl.0"*, align 8
  store %"class.llvm::ilist_node_impl.0"* %0, %"class.llvm::ilist_node_impl.0"** %3, align 8
  store %"class.llvm::ilist_node_impl.0"* %1, %"class.llvm::ilist_node_impl.0"** %4, align 8
  %5 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %3, align 8
  %6 = bitcast %"class.llvm::ilist_node_impl.0"* %5 to %"class.llvm::ilist_node_base"*
  %7 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %4, align 8
  %8 = bitcast %"class.llvm::ilist_node_impl.0"* %7 to %"class.llvm::ilist_node_base"*
  call void @_ZN4llvm10ilist_baseILb0EE16insertBeforeImplERNS_15ilist_node_baseILb0EEES4_(%"class.llvm::ilist_node_base"* dereferenceable(16) %6, %"class.llvm::ilist_node_base"* dereferenceable(16) %8)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.0"* @_ZNK4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EE10getNodePtrEv(%"class.llvm::ilist_iterator"*) #4 align 2 {
  %2 = alloca %"class.llvm::ilist_iterator"*, align 8
  store %"class.llvm::ilist_iterator"* %0, %"class.llvm::ilist_iterator"** %2, align 8
  %3 = load %"class.llvm::ilist_iterator"*, %"class.llvm::ilist_iterator"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %3, i32 0, i32 0
  %5 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %4, align 8
  ret %"class.llvm::ilist_node_impl.0"* %5
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.0"* @_ZN4llvm12ilist_detail18SpecificNodeAccessINS0_12node_optionsINS_11InstructionELb0ELb0EvEEE10getNodePtrEPS3_(%"class.llvm::Instruction"*) #0 align 2 {
  %2 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"* %0, %"class.llvm::Instruction"** %2, align 8
  %3 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %2, align 8
  %4 = call %"class.llvm::ilist_node_impl.0"* @_ZN4llvm12ilist_detail10NodeAccess10getNodePtrINS0_12node_optionsINS_11InstructionELb0ELb0EvEEEEPNS_15ilist_node_implIT_EENS7_7pointerE(%"class.llvm::Instruction"* %3)
  ret %"class.llvm::ilist_node_impl.0"* %4
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
define linkonce_odr %"class.llvm::ilist_node_base"* @_ZNK4llvm15ilist_node_baseILb0EE7getPrevEv(%"class.llvm::ilist_node_base"*) #4 align 2 {
  %2 = alloca %"class.llvm::ilist_node_base"*, align 8
  store %"class.llvm::ilist_node_base"* %0, %"class.llvm::ilist_node_base"** %2, align 8
  %3 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::ilist_node_base", %"class.llvm::ilist_node_base"* %3, i32 0, i32 0
  %5 = load %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"** %4, align 8
  ret %"class.llvm::ilist_node_base"* %5
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm15ilist_node_baseILb0EE7setNextEPS1_(%"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"*) #4 align 2 {
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
define linkonce_odr void @_ZN4llvm15ilist_node_baseILb0EE7setPrevEPS1_(%"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"*) #4 align 2 {
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
define linkonce_odr %"class.llvm::ilist_node_impl.0"* @_ZN4llvm12ilist_detail10NodeAccess10getNodePtrINS0_12node_optionsINS_11InstructionELb0ELb0EvEEEEPNS_15ilist_node_implIT_EENS7_7pointerE(%"class.llvm::Instruction"*) #4 align 2 {
  %2 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"* %0, %"class.llvm::Instruction"** %2, align 8
  %3 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %2, align 8
  %4 = icmp eq %"class.llvm::Instruction"* %3, null
  br i1 %4, label %9, label %5

; <label>:5:                                      ; preds = %1
  %6 = bitcast %"class.llvm::Instruction"* %3 to i8*
  %7 = getelementptr inbounds i8, i8* %6, i64 32
  %8 = bitcast i8* %7 to %"class.llvm::ilist_node_impl.0"*
  br label %9

; <label>:9:                                      ; preds = %5, %1
  %10 = phi %"class.llvm::ilist_node_impl.0"* [ %8, %5 ], [ null, %1 ]
  ret %"class.llvm::ilist_node_impl.0"* %10
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
  %9 = call %"class.llvm::ilist_node_impl.0"* @_ZN4llvm12ilist_detail18SpecificNodeAccessINS0_12node_optionsINS_11InstructionELb0ELb0EvEEE10getNodePtrEPS3_(%"class.llvm::Instruction"* %8)
  store %"class.llvm::ilist_node_impl.0"* %9, %"class.llvm::ilist_node_impl.0"** %7, align 8
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

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm8DebugLocD1Ev(%"class.llvm::DebugLoc"*) unnamed_addr #7 align 2 {
  %2 = alloca %"class.llvm::DebugLoc"*, align 8
  store %"class.llvm::DebugLoc"* %0, %"class.llvm::DebugLoc"** %2, align 8
  %3 = load %"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"** %2, align 8
  call void @_ZN4llvm8DebugLocD2Ev(%"class.llvm::DebugLoc"* %3) #14
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
define linkonce_odr %"class.llvm::Metadata"* @_ZNK4llvm13TrackingMDRef3getEv(%"class.llvm::TrackingMDRef"*) #4 align 2 {
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
  call void @__assert_rtn(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @__func__._ZN4llvm13TrackingMDRef7retrackERS0_, i32 0, i32 0), i8* getelementptr inbounds ([63 x i8], [63 x i8]* @.str.11, i32 0, i32 0), i32 88, i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str.12, i32 0, i32 0)) #15
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

declare void @_ZN4llvm16MetadataTracking7untrackEPvRNS_8MetadataE(i8*, %"class.llvm::Metadata"* dereferenceable(8)) #1

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

declare zeroext i1 @_ZN4llvm16MetadataTracking7retrackEPvRNS_8MetadataES1_(i8*, %"class.llvm::Metadata"* dereferenceable(8), i8*) #1

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
  %3 = alloca %"class.llvm::PointerUnion.87", align 8
  store %"class.llvm::Metadata"** %0, %"class.llvm::Metadata"*** %2, align 8
  %4 = load %"class.llvm::Metadata"**, %"class.llvm::Metadata"*** %2, align 8
  %5 = bitcast %"class.llvm::Metadata"** %4 to i8*
  %6 = load %"class.llvm::Metadata"**, %"class.llvm::Metadata"*** %2, align 8
  %7 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %6, align 8
  call void @_ZN4llvm12PointerUnionIPNS_15MetadataAsValueEPNS_8MetadataEEC1ES4_(%"class.llvm::PointerUnion.87"* %3, %"class.llvm::Metadata"* null)
  %8 = getelementptr inbounds %"class.llvm::PointerUnion.87", %"class.llvm::PointerUnion.87"* %3, i32 0, i32 0
  %9 = getelementptr inbounds %"class.llvm::PointerIntPair.88", %"class.llvm::PointerIntPair.88"* %8, i32 0, i32 0
  %10 = load i64, i64* %9, align 8
  %11 = call zeroext i1 @_ZN4llvm16MetadataTracking5trackEPvRNS_8MetadataENS_12PointerUnionIPNS_15MetadataAsValueEPS2_EE(i8* %5, %"class.llvm::Metadata"* dereferenceable(8) %7, i64 %10)
  ret i1 %11
}

declare zeroext i1 @_ZN4llvm16MetadataTracking5trackEPvRNS_8MetadataENS_12PointerUnionIPNS_15MetadataAsValueEPS2_EE(i8*, %"class.llvm::Metadata"* dereferenceable(8), i64) #1

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm12PointerUnionIPNS_15MetadataAsValueEPNS_8MetadataEEC1ES4_(%"class.llvm::PointerUnion.87"*, %"class.llvm::Metadata"*) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::PointerUnion.87"*, align 8
  %4 = alloca %"class.llvm::Metadata"*, align 8
  store %"class.llvm::PointerUnion.87"* %0, %"class.llvm::PointerUnion.87"** %3, align 8
  store %"class.llvm::Metadata"* %1, %"class.llvm::Metadata"** %4, align 8
  %5 = load %"class.llvm::PointerUnion.87"*, %"class.llvm::PointerUnion.87"** %3, align 8
  %6 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %4, align 8
  call void @_ZN4llvm12PointerUnionIPNS_15MetadataAsValueEPNS_8MetadataEEC2ES4_(%"class.llvm::PointerUnion.87"* %5, %"class.llvm::Metadata"* %6)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm12PointerUnionIPNS_15MetadataAsValueEPNS_8MetadataEEC2ES4_(%"class.llvm::PointerUnion.87"*, %"class.llvm::Metadata"*) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::PointerUnion.87"*, align 8
  %4 = alloca %"class.llvm::Metadata"*, align 8
  store %"class.llvm::PointerUnion.87"* %0, %"class.llvm::PointerUnion.87"** %3, align 8
  store %"class.llvm::Metadata"* %1, %"class.llvm::Metadata"** %4, align 8
  %5 = load %"class.llvm::PointerUnion.87"*, %"class.llvm::PointerUnion.87"** %3, align 8
  %6 = getelementptr inbounds %"class.llvm::PointerUnion.87", %"class.llvm::PointerUnion.87"* %5, i32 0, i32 0
  %7 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %4, align 8
  %8 = call i8* @_ZN4llvm21PointerLikeTypeTraitsIPNS_8MetadataEE16getAsVoidPointerES2_(%"class.llvm::Metadata"* %7)
  call void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEEC1ES1_b(%"class.llvm::PointerIntPair.88"* %6, i8* %8, i1 zeroext true)
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr i8* @_ZN4llvm21PointerLikeTypeTraitsIPNS_8MetadataEE16getAsVoidPointerES2_(%"class.llvm::Metadata"*) #7 align 2 {
  %2 = alloca %"class.llvm::Metadata"*, align 8
  store %"class.llvm::Metadata"* %0, %"class.llvm::Metadata"** %2, align 8
  %3 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %2, align 8
  %4 = bitcast %"class.llvm::Metadata"* %3 to i8*
  ret i8* %4
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEEC1ES1_b(%"class.llvm::PointerIntPair.88"*, i8*, i1 zeroext) unnamed_addr #0 align 2 {
  %4 = alloca %"class.llvm::PointerIntPair.88"*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i8, align 1
  store %"class.llvm::PointerIntPair.88"* %0, %"class.llvm::PointerIntPair.88"** %4, align 8
  store i8* %1, i8** %5, align 8
  %7 = zext i1 %2 to i8
  store i8 %7, i8* %6, align 1
  %8 = load %"class.llvm::PointerIntPair.88"*, %"class.llvm::PointerIntPair.88"** %4, align 8
  %9 = load i8*, i8** %5, align 8
  %10 = load i8, i8* %6, align 1
  %11 = trunc i8 %10 to i1
  call void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEEC2ES1_b(%"class.llvm::PointerIntPair.88"* %8, i8* %9, i1 zeroext %11)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEEC2ES1_b(%"class.llvm::PointerIntPair.88"*, i8*, i1 zeroext) unnamed_addr #0 align 2 {
  %4 = alloca %"class.llvm::PointerIntPair.88"*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i8, align 1
  store %"class.llvm::PointerIntPair.88"* %0, %"class.llvm::PointerIntPair.88"** %4, align 8
  store i8* %1, i8** %5, align 8
  %7 = zext i1 %2 to i8
  store i8 %7, i8* %6, align 1
  %8 = load %"class.llvm::PointerIntPair.88"*, %"class.llvm::PointerIntPair.88"** %4, align 8
  %9 = load i8*, i8** %5, align 8
  %10 = load i8, i8* %6, align 1
  %11 = trunc i8 %10 to i1
  call void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEE16setPointerAndIntES1_b(%"class.llvm::PointerIntPair.88"* %8, i8* %9, i1 zeroext %11)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEE16setPointerAndIntES1_b(%"class.llvm::PointerIntPair.88"*, i8*, i1 zeroext) #0 align 2 {
  %4 = alloca %"class.llvm::PointerIntPair.88"*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i8, align 1
  store %"class.llvm::PointerIntPair.88"* %0, %"class.llvm::PointerIntPair.88"** %4, align 8
  store i8* %1, i8** %5, align 8
  %7 = zext i1 %2 to i8
  store i8 %7, i8* %6, align 1
  %8 = load %"class.llvm::PointerIntPair.88"*, %"class.llvm::PointerIntPair.88"** %4, align 8
  %9 = load i8*, i8** %5, align 8
  %10 = call i64 @_ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE13updatePointerElS1_(i64 0, i8* %9)
  %11 = load i8, i8* %6, align 1
  %12 = trunc i8 %11 to i1
  %13 = zext i1 %12 to i64
  %14 = call i64 @_ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE9updateIntEll(i64 %10, i64 %13)
  %15 = getelementptr inbounds %"class.llvm::PointerIntPair.88", %"class.llvm::PointerIntPair.88"* %8, i32 0, i32 0
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
  call void @__assert_rtn(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @__func__._ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE9updateIntEll, i32 0, i32 0), i8* getelementptr inbounds ([65 x i8], [65 x i8]* @.str.14, i32 0, i32 0), i32 167, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.15, i32 0, i32 0)) #15
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
  call void @__assert_rtn(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @__func__._ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE13updatePointerElS1_, i32 0, i32 0), i8* getelementptr inbounds ([65 x i8], [65 x i8]* @.str.14, i32 0, i32 0), i32 160, i8* getelementptr inbounds ([74 x i8], [74 x i8]* @.str.17, i32 0, i32 0)) #15
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
define linkonce_odr i8* @_ZN4llvm22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEE16getAsVoidPointerEPv(i8*) #7 align 2 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = load i8*, i8** %2, align 8
  ret i8* %3
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm8DebugLocD2Ev(%"class.llvm::DebugLoc"*) unnamed_addr #7 align 2 {
  %2 = alloca %"class.llvm::DebugLoc"*, align 8
  store %"class.llvm::DebugLoc"* %0, %"class.llvm::DebugLoc"** %2, align 8
  %3 = load %"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::DebugLoc", %"class.llvm::DebugLoc"* %3, i32 0, i32 0
  call void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEED1Ev(%"class.llvm::TypedTrackingMDRef"* %4) #14
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEED1Ev(%"class.llvm::TypedTrackingMDRef"*) unnamed_addr #7 align 2 {
  %2 = alloca %"class.llvm::TypedTrackingMDRef"*, align 8
  store %"class.llvm::TypedTrackingMDRef"* %0, %"class.llvm::TypedTrackingMDRef"** %2, align 8
  %3 = load %"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"** %2, align 8
  call void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEED2Ev(%"class.llvm::TypedTrackingMDRef"* %3) #14
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEED2Ev(%"class.llvm::TypedTrackingMDRef"*) unnamed_addr #7 align 2 {
  %2 = alloca %"class.llvm::TypedTrackingMDRef"*, align 8
  store %"class.llvm::TypedTrackingMDRef"* %0, %"class.llvm::TypedTrackingMDRef"** %2, align 8
  %3 = load %"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::TypedTrackingMDRef", %"class.llvm::TypedTrackingMDRef"* %3, i32 0, i32 0
  call void @_ZN4llvm13TrackingMDRefD1Ev(%"class.llvm::TrackingMDRef"* %4) #14
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm13TrackingMDRefD1Ev(%"class.llvm::TrackingMDRef"*) unnamed_addr #4 align 2 {
  %2 = alloca %"class.llvm::TrackingMDRef"*, align 8
  store %"class.llvm::TrackingMDRef"* %0, %"class.llvm::TrackingMDRef"** %2, align 8
  %3 = load %"class.llvm::TrackingMDRef"*, %"class.llvm::TrackingMDRef"** %2, align 8
  call void @_ZN4llvm13TrackingMDRefD2Ev(%"class.llvm::TrackingMDRef"* %3) #14
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm13TrackingMDRefD2Ev(%"class.llvm::TrackingMDRef"*) unnamed_addr #4 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  call void @__clang_call_terminate(i8* %7) #16
  unreachable
}

; Function Attrs: noinline noreturn nounwind
define linkonce_odr hidden void @__clang_call_terminate(i8*) #8 {
  %2 = call i8* @__cxa_begin_catch(i8* %0) #14
  call void @_ZSt9terminatev() #16
  unreachable
}

declare i8* @__cxa_begin_catch(i8*)

declare void @_ZSt9terminatev()

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE9CreateGEPEPNS_4TypeEPNS_5ValueES7_RKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Type"*, %"class.llvm::Value"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %6 = alloca %"class.llvm::Value"*, align 8
  %7 = alloca %"class.llvm::IRBuilder"*, align 8
  %8 = alloca %"class.llvm::Type"*, align 8
  %9 = alloca %"class.llvm::Value"*, align 8
  %10 = alloca %"class.llvm::Value"*, align 8
  %11 = alloca %"class.llvm::Twine"*, align 8
  %12 = alloca %"class.llvm::Constant"*, align 8
  %13 = alloca %"class.llvm::Constant"*, align 8
  %14 = alloca %"class.llvm::ArrayRef.36", align 8
  %15 = alloca %"class.llvm::Twine", align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %7, align 8
  store %"class.llvm::Type"* %1, %"class.llvm::Type"** %8, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %9, align 8
  store %"class.llvm::Value"* %3, %"class.llvm::Value"** %10, align 8
  store %"class.llvm::Twine"* %4, %"class.llvm::Twine"** %11, align 8
  %16 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %7, align 8
  %17 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %18 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %17)
  store %"class.llvm::Constant"* %18, %"class.llvm::Constant"** %12, align 8
  %19 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %20 = icmp ne %"class.llvm::Constant"* %19, null
  br i1 %20, label %21, label %36

; <label>:21:                                     ; preds = %5
  %22 = load %"class.llvm::Value"*, %"class.llvm::Value"** %10, align 8
  %23 = call %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"* %22)
  store %"class.llvm::Constant"* %23, %"class.llvm::Constant"** %13, align 8
  %24 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %13, align 8
  %25 = icmp ne %"class.llvm::Constant"* %24, null
  br i1 %25, label %26, label %35

; <label>:26:                                     ; preds = %21
  %27 = getelementptr inbounds %"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* %16, i32 0, i32 1
  %28 = load %"class.llvm::Type"*, %"class.llvm::Type"** %8, align 8
  %29 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %12, align 8
  %30 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %13, align 8
  %31 = call %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder19CreateGetElementPtrEPNS_4TypeEPNS_8ConstantES4_(%"class.llvm::ConstantFolder"* %27, %"class.llvm::Type"* %28, %"class.llvm::Constant"* %29, %"class.llvm::Constant"* %30)
  %32 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %11, align 8
  %33 = call %"class.llvm::Constant"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertEPNS_8ConstantERKNS_5TwineE(%"class.llvm::IRBuilder"* %16, %"class.llvm::Constant"* %31, %"class.llvm::Twine"* dereferenceable(24) %32)
  %34 = bitcast %"class.llvm::Constant"* %33 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %34, %"class.llvm::Value"** %6, align 8
  br label %48

; <label>:35:                                     ; preds = %21
  br label %36

; <label>:36:                                     ; preds = %35, %5
  %37 = load %"class.llvm::Type"*, %"class.llvm::Type"** %8, align 8
  %38 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  call void @_ZN4llvm8ArrayRefIPNS_5ValueEEC1ERKS2_(%"class.llvm::ArrayRef.36"* %14, %"class.llvm::Value"** dereferenceable(8) %10)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %15, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %39 = bitcast %"class.llvm::ArrayRef.36"* %14 to { %"class.llvm::Value"**, i64 }*
  %40 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %39, i32 0, i32 0
  %41 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %40, align 8
  %42 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %39, i32 0, i32 1
  %43 = load i64, i64* %42, align 8
  %44 = call %"class.llvm::GetElementPtrInst"* @_ZN4llvm17GetElementPtrInst6CreateEPNS_4TypeEPNS_5ValueENS_8ArrayRefIS4_EERKNS_5TwineEPNS_11InstructionE(%"class.llvm::Type"* %37, %"class.llvm::Value"* %38, %"class.llvm::Value"** %41, i64 %43, %"class.llvm::Twine"* dereferenceable(24) %15, %"class.llvm::Instruction"* null)
  %45 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %11, align 8
  %46 = call %"class.llvm::GetElementPtrInst"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_17GetElementPtrInstEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"* %16, %"class.llvm::GetElementPtrInst"* %44, %"class.llvm::Twine"* dereferenceable(24) %45)
  %47 = bitcast %"class.llvm::GetElementPtrInst"* %46 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %47, %"class.llvm::Value"** %6, align 8
  br label %48

; <label>:48:                                     ; preds = %36, %26
  %49 = load %"class.llvm::Value"*, %"class.llvm::Value"** %6, align 8
  ret %"class.llvm::Value"* %49
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZNK4llvm14ConstantFolder19CreateGetElementPtrEPNS_4TypeEPNS_8ConstantES4_(%"class.llvm::ConstantFolder"*, %"class.llvm::Type"*, %"class.llvm::Constant"*, %"class.llvm::Constant"*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %5 = alloca %"class.llvm::ConstantFolder"*, align 8
  %6 = alloca %"class.llvm::Type"*, align 8
  %7 = alloca %"class.llvm::Constant"*, align 8
  %8 = alloca %"class.llvm::Constant"*, align 8
  %9 = alloca %"class.llvm::Optional", align 4
  %10 = alloca i8*
  %11 = alloca i32
  store %"class.llvm::ConstantFolder"* %0, %"class.llvm::ConstantFolder"** %5, align 8
  store %"class.llvm::Type"* %1, %"class.llvm::Type"** %6, align 8
  store %"class.llvm::Constant"* %2, %"class.llvm::Constant"** %7, align 8
  store %"class.llvm::Constant"* %3, %"class.llvm::Constant"** %8, align 8
  %12 = load %"class.llvm::ConstantFolder"*, %"class.llvm::ConstantFolder"** %5, align 8
  %13 = load %"class.llvm::Type"*, %"class.llvm::Type"** %6, align 8
  %14 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %7, align 8
  %15 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %8, align 8
  call void @_ZN4llvm8OptionalIjEC1ENS_8NoneTypeE(%"class.llvm::Optional"* %9, i32 0)
  %16 = invoke %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr16getGetElementPtrEPNS_4TypeEPNS_8ConstantES4_bNS_8OptionalIjEES2_(%"class.llvm::Type"* %13, %"class.llvm::Constant"* %14, %"class.llvm::Constant"* %15, i1 zeroext false, %"class.llvm::Optional"* %9, %"class.llvm::Type"* null)
          to label %17 unwind label %18

; <label>:17:                                     ; preds = %4
  call void @_ZN4llvm8OptionalIjED1Ev(%"class.llvm::Optional"* %9) #14
  ret %"class.llvm::Constant"* %16

; <label>:18:                                     ; preds = %4
  %19 = landingpad { i8*, i32 }
          cleanup
  %20 = extractvalue { i8*, i32 } %19, 0
  store i8* %20, i8** %10, align 8
  %21 = extractvalue { i8*, i32 } %19, 1
  store i32 %21, i32* %11, align 4
  call void @_ZN4llvm8OptionalIjED1Ev(%"class.llvm::Optional"* %9) #14
  br label %22

; <label>:22:                                     ; preds = %18
  %23 = load i8*, i8** %10, align 8
  %24 = load i32, i32* %11, align 4
  %25 = insertvalue { i8*, i32 } undef, i8* %23, 0
  %26 = insertvalue { i8*, i32 } %25, i32 %24, 1
  resume { i8*, i32 } %26
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::GetElementPtrInst"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_17GetElementPtrInstEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::GetElementPtrInst"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %4 = alloca %"class.llvm::IRBuilder"*, align 8
  %5 = alloca %"class.llvm::GetElementPtrInst"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  %7 = alloca %"class.llvm::ilist_iterator", align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %4, align 8
  store %"class.llvm::GetElementPtrInst"* %1, %"class.llvm::GetElementPtrInst"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %8 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %4, align 8
  %9 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderDefaultInserter"*
  %10 = load %"class.llvm::GetElementPtrInst"*, %"class.llvm::GetElementPtrInst"** %5, align 8
  %11 = bitcast %"class.llvm::GetElementPtrInst"* %10 to %"class.llvm::Instruction"*
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
  %21 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %20, align 8
  call void @_ZNK4llvm24IRBuilderDefaultInserter12InsertHelperEPNS_11InstructionERKNS_5TwineEPNS_10BasicBlockENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEE(%"class.llvm::IRBuilderDefaultInserter"* %9, %"class.llvm::Instruction"* %11, %"class.llvm::Twine"* dereferenceable(24) %12, %"class.llvm::BasicBlock"* %15, %"class.llvm::ilist_node_impl.0"* %21)
  %22 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderBase"*
  %23 = load %"class.llvm::GetElementPtrInst"*, %"class.llvm::GetElementPtrInst"** %5, align 8
  %24 = bitcast %"class.llvm::GetElementPtrInst"* %23 to %"class.llvm::Instruction"*
  call void @_ZNK4llvm13IRBuilderBase20SetInstDebugLocationEPNS_11InstructionE(%"class.llvm::IRBuilderBase"* %22, %"class.llvm::Instruction"* %24)
  %25 = load %"class.llvm::GetElementPtrInst"*, %"class.llvm::GetElementPtrInst"** %5, align 8
  ret %"class.llvm::GetElementPtrInst"* %25
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::GetElementPtrInst"* @_ZN4llvm17GetElementPtrInst6CreateEPNS_4TypeEPNS_5ValueENS_8ArrayRefIS4_EERKNS_5TwineEPNS_11InstructionE(%"class.llvm::Type"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::Instruction"*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %7 = alloca %"class.llvm::ArrayRef.36", align 8
  %8 = alloca %"class.llvm::Type"*, align 8
  %9 = alloca %"class.llvm::Value"*, align 8
  %10 = alloca %"class.llvm::Twine"*, align 8
  %11 = alloca %"class.llvm::Instruction"*, align 8
  %12 = alloca i32, align 4
  %13 = alloca %"class.llvm::ArrayRef.36", align 8
  %14 = alloca i8*
  %15 = alloca i32
  %16 = bitcast %"class.llvm::ArrayRef.36"* %7 to { %"class.llvm::Value"**, i64 }*
  %17 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %16, i32 0, i32 0
  store %"class.llvm::Value"** %2, %"class.llvm::Value"*** %17, align 8
  %18 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %16, i32 0, i32 1
  store i64 %3, i64* %18, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %8, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %9, align 8
  store %"class.llvm::Twine"* %4, %"class.llvm::Twine"** %10, align 8
  store %"class.llvm::Instruction"* %5, %"class.llvm::Instruction"** %11, align 8
  %19 = call i64 @_ZNK4llvm8ArrayRefIPNS_5ValueEE4sizeEv(%"class.llvm::ArrayRef.36"* %7)
  %20 = trunc i64 %19 to i32
  %21 = add i32 1, %20
  store i32 %21, i32* %12, align 4
  %22 = load %"class.llvm::Type"*, %"class.llvm::Type"** %8, align 8
  %23 = icmp ne %"class.llvm::Type"* %22, null
  br i1 %23, label %30, label %24

; <label>:24:                                     ; preds = %6
  %25 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %26 = call %"class.llvm::Type"* @_ZNK4llvm5Value7getTypeEv(%"class.llvm::Value"* %25)
  %27 = call %"class.llvm::Type"* @_ZNK4llvm4Type13getScalarTypeEv(%"class.llvm::Type"* %26) #18
  %28 = call %"class.llvm::PointerType"* @_ZN4llvm4castINS_11PointerTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Type"* %27)
  %29 = call %"class.llvm::Type"* @_ZNK4llvm11PointerType14getElementTypeEv(%"class.llvm::PointerType"* %28)
  store %"class.llvm::Type"* %29, %"class.llvm::Type"** %8, align 8
  br label %43

; <label>:30:                                     ; preds = %6
  %31 = load %"class.llvm::Type"*, %"class.llvm::Type"** %8, align 8
  %32 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %33 = call %"class.llvm::Type"* @_ZNK4llvm5Value7getTypeEv(%"class.llvm::Value"* %32)
  %34 = call %"class.llvm::Type"* @_ZNK4llvm4Type13getScalarTypeEv(%"class.llvm::Type"* %33) #18
  %35 = call %"class.llvm::PointerType"* @_ZN4llvm4castINS_11PointerTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Type"* %34)
  %36 = call %"class.llvm::Type"* @_ZNK4llvm11PointerType14getElementTypeEv(%"class.llvm::PointerType"* %35)
  %37 = icmp eq %"class.llvm::Type"* %31, %36
  %38 = xor i1 %37, true
  br i1 %38, label %39, label %41

; <label>:39:                                     ; preds = %30
  call void @__assert_rtn(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @__func__._ZN4llvm17GetElementPtrInst6CreateEPNS_4TypeEPNS_5ValueENS_8ArrayRefIS4_EERKNS_5TwineEPNS_11InstructionE, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.20, i32 0, i32 0), i32 868, i8* getelementptr inbounds ([84 x i8], [84 x i8]* @.str.21, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %42

; <label>:41:                                     ; preds = %30
  br label %42

; <label>:42:                                     ; preds = %41, %40
  br label %43

; <label>:43:                                     ; preds = %42, %24
  %44 = load i32, i32* %12, align 4
  %45 = call i8* @_ZN4llvm4UsernwEmj(i64 80, i32 %44)
  %46 = bitcast i8* %45 to %"class.llvm::GetElementPtrInst"*
  %47 = load %"class.llvm::Type"*, %"class.llvm::Type"** %8, align 8
  %48 = load %"class.llvm::Value"*, %"class.llvm::Value"** %9, align 8
  %49 = bitcast %"class.llvm::ArrayRef.36"* %13 to i8*
  %50 = bitcast %"class.llvm::ArrayRef.36"* %7 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %49, i8* %50, i64 16, i32 8, i1 false)
  %51 = load i32, i32* %12, align 4
  %52 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %53 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %11, align 8
  %54 = bitcast %"class.llvm::ArrayRef.36"* %13 to { %"class.llvm::Value"**, i64 }*
  %55 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %54, i32 0, i32 0
  %56 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %55, align 8
  %57 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %54, i32 0, i32 1
  %58 = load i64, i64* %57, align 8
  invoke void @_ZN4llvm17GetElementPtrInstC1EPNS_4TypeEPNS_5ValueENS_8ArrayRefIS4_EEjRKNS_5TwineEPNS_11InstructionE(%"class.llvm::GetElementPtrInst"* %46, %"class.llvm::Type"* %47, %"class.llvm::Value"* %48, %"class.llvm::Value"** %56, i64 %58, i32 %51, %"class.llvm::Twine"* dereferenceable(24) %52, %"class.llvm::Instruction"* %53)
          to label %59 unwind label %60

; <label>:59:                                     ; preds = %43
  ret %"class.llvm::GetElementPtrInst"* %46

; <label>:60:                                     ; preds = %43
  %61 = landingpad { i8*, i32 }
          cleanup
  %62 = extractvalue { i8*, i32 } %61, 0
  store i8* %62, i8** %14, align 8
  %63 = extractvalue { i8*, i32 } %61, 1
  store i32 %63, i32* %15, align 4
  call void @_ZN4llvm4UserdlEPvj(i8* %45, i32 %44) #14
  br label %64

; <label>:64:                                     ; preds = %60
  %65 = load i8*, i8** %14, align 8
  %66 = load i32, i32* %15, align 4
  %67 = insertvalue { i8*, i32 } undef, i8* %65, 0
  %68 = insertvalue { i8*, i32 } %67, i32 %66, 1
  resume { i8*, i32 } %68
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm8ArrayRefIPNS_5ValueEEC1ERKS2_(%"class.llvm::ArrayRef.36"*, %"class.llvm::Value"** dereferenceable(8)) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::ArrayRef.36"*, align 8
  %4 = alloca %"class.llvm::Value"**, align 8
  store %"class.llvm::ArrayRef.36"* %0, %"class.llvm::ArrayRef.36"** %3, align 8
  store %"class.llvm::Value"** %1, %"class.llvm::Value"*** %4, align 8
  %5 = load %"class.llvm::ArrayRef.36"*, %"class.llvm::ArrayRef.36"** %3, align 8
  %6 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %4, align 8
  call void @_ZN4llvm8ArrayRefIPNS_5ValueEEC2ERKS2_(%"class.llvm::ArrayRef.36"* %5, %"class.llvm::Value"** dereferenceable(8) %6)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr16getGetElementPtrEPNS_4TypeEPNS_8ConstantES4_bNS_8OptionalIjEES2_(%"class.llvm::Type"*, %"class.llvm::Constant"*, %"class.llvm::Constant"*, i1 zeroext, %"class.llvm::Optional"*, %"class.llvm::Type"*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %7 = alloca %"class.llvm::Type"*, align 8
  %8 = alloca %"class.llvm::Constant"*, align 8
  %9 = alloca %"class.llvm::Constant"*, align 8
  %10 = alloca i8, align 1
  %11 = alloca %"class.llvm::Type"*, align 8
  %12 = alloca %"class.llvm::ArrayRef.36", align 8
  %13 = alloca %"class.llvm::Value"*, align 8
  %14 = alloca %"class.llvm::Optional", align 4
  %15 = alloca i8*
  %16 = alloca i32
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %7, align 8
  store %"class.llvm::Constant"* %1, %"class.llvm::Constant"** %8, align 8
  store %"class.llvm::Constant"* %2, %"class.llvm::Constant"** %9, align 8
  %17 = zext i1 %3 to i8
  store i8 %17, i8* %10, align 1
  store %"class.llvm::Type"* %5, %"class.llvm::Type"** %11, align 8
  %18 = load %"class.llvm::Type"*, %"class.llvm::Type"** %7, align 8
  %19 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %8, align 8
  %20 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %9, align 8
  %21 = call %"class.llvm::Value"* @_ZN4llvm4castINS_5ValueENS_8ConstantEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Constant"* %20)
  store %"class.llvm::Value"* %21, %"class.llvm::Value"** %13, align 8
  call void @_ZN4llvm8ArrayRefIPNS_5ValueEEC1ERKS2_(%"class.llvm::ArrayRef.36"* %12, %"class.llvm::Value"** dereferenceable(8) %13)
  %22 = load i8, i8* %10, align 1
  %23 = trunc i8 %22 to i1
  call void @_ZN4llvm8OptionalIjEC1ERKS1_(%"class.llvm::Optional"* %14, %"class.llvm::Optional"* dereferenceable(8) %4)
  %24 = load %"class.llvm::Type"*, %"class.llvm::Type"** %11, align 8
  %25 = bitcast %"class.llvm::ArrayRef.36"* %12 to { %"class.llvm::Value"**, i64 }*
  %26 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %25, i32 0, i32 0
  %27 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %26, align 8
  %28 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %25, i32 0, i32 1
  %29 = load i64, i64* %28, align 8
  %30 = invoke %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr16getGetElementPtrEPNS_4TypeEPNS_8ConstantENS_8ArrayRefIPNS_5ValueEEEbNS_8OptionalIjEES2_(%"class.llvm::Type"* %18, %"class.llvm::Constant"* %19, %"class.llvm::Value"** %27, i64 %29, i1 zeroext %23, %"class.llvm::Optional"* %14, %"class.llvm::Type"* %24)
          to label %31 unwind label %32

; <label>:31:                                     ; preds = %6
  call void @_ZN4llvm8OptionalIjED1Ev(%"class.llvm::Optional"* %14) #14
  ret %"class.llvm::Constant"* %30

; <label>:32:                                     ; preds = %6
  %33 = landingpad { i8*, i32 }
          cleanup
  %34 = extractvalue { i8*, i32 } %33, 0
  store i8* %34, i8** %15, align 8
  %35 = extractvalue { i8*, i32 } %33, 1
  store i32 %35, i32* %16, align 4
  call void @_ZN4llvm8OptionalIjED1Ev(%"class.llvm::Optional"* %14) #14
  br label %36

; <label>:36:                                     ; preds = %32
  %37 = load i8*, i8** %15, align 8
  %38 = load i32, i32* %16, align 4
  %39 = insertvalue { i8*, i32 } undef, i8* %37, 0
  %40 = insertvalue { i8*, i32 } %39, i32 %38, 1
  resume { i8*, i32 } %40
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm8OptionalIjEC1ENS_8NoneTypeE(%"class.llvm::Optional"*, i32) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::Optional"*, align 8
  %4 = alloca i32, align 4
  store %"class.llvm::Optional"* %0, %"class.llvm::Optional"** %3, align 8
  store i32 %1, i32* %4, align 4
  %5 = load %"class.llvm::Optional"*, %"class.llvm::Optional"** %3, align 8
  %6 = load i32, i32* %4, align 4
  call void @_ZN4llvm8OptionalIjEC2ENS_8NoneTypeE(%"class.llvm::Optional"* %5, i32 %6)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm8OptionalIjED1Ev(%"class.llvm::Optional"*) unnamed_addr #4 align 2 {
  %2 = alloca %"class.llvm::Optional"*, align 8
  store %"class.llvm::Optional"* %0, %"class.llvm::Optional"** %2, align 8
  %3 = load %"class.llvm::Optional"*, %"class.llvm::Optional"** %2, align 8
  call void @_ZN4llvm8OptionalIjED2Ev(%"class.llvm::Optional"* %3) #14
  ret void
}

declare %"class.llvm::Constant"* @_ZN4llvm12ConstantExpr16getGetElementPtrEPNS_4TypeEPNS_8ConstantENS_8ArrayRefIPNS_5ValueEEEbNS_8OptionalIjEES2_(%"class.llvm::Type"*, %"class.llvm::Constant"*, %"class.llvm::Value"**, i64, i1 zeroext, %"class.llvm::Optional"*, %"class.llvm::Type"*) #1

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm4castINS_5ValueENS_8ConstantEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Constant"*) #3 {
  %2 = alloca %"class.llvm::Constant"*, align 8
  store %"class.llvm::Constant"* %0, %"class.llvm::Constant"** %2, align 8
  %3 = call zeroext i1 @_ZN4llvm3isaINS_5ValueEPNS_8ConstantEEEbRKT0_(%"class.llvm::Constant"** dereferenceable(8) %2)
  br i1 %3, label %4, label %5

; <label>:4:                                      ; preds = %1
  br label %5

; <label>:5:                                      ; preds = %4, %1
  %6 = phi i1 [ false, %1 ], [ true, %4 ]
  %7 = xor i1 %6, true
  br i1 %7, label %8, label %10

; <label>:8:                                      ; preds = %5
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm4castINS_10StructTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 236, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.9, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %11

; <label>:10:                                     ; preds = %5
  br label %11

; <label>:11:                                     ; preds = %10, %9
  %12 = call %"class.llvm::Value"* @_ZN4llvm16cast_convert_valINS_5ValueEPNS_8ConstantES3_E4doitERKS3_(%"class.llvm::Constant"** dereferenceable(8) %2)
  ret %"class.llvm::Value"* %12
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm8OptionalIjEC1ERKS1_(%"class.llvm::Optional"*, %"class.llvm::Optional"* dereferenceable(8)) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::Optional"*, align 8
  %4 = alloca %"class.llvm::Optional"*, align 8
  store %"class.llvm::Optional"* %0, %"class.llvm::Optional"** %3, align 8
  store %"class.llvm::Optional"* %1, %"class.llvm::Optional"** %4, align 8
  %5 = load %"class.llvm::Optional"*, %"class.llvm::Optional"** %3, align 8
  %6 = load %"class.llvm::Optional"*, %"class.llvm::Optional"** %4, align 8
  call void @_ZN4llvm8OptionalIjEC2ERKS1_(%"class.llvm::Optional"* %5, %"class.llvm::Optional"* dereferenceable(8) %6)
  ret void
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm3isaINS_5ValueEPNS_8ConstantEEEbRKT0_(%"class.llvm::Constant"** dereferenceable(8)) #3 {
  %2 = alloca %"class.llvm::Constant"**, align 8
  store %"class.llvm::Constant"** %0, %"class.llvm::Constant"*** %2, align 8
  %3 = load %"class.llvm::Constant"**, %"class.llvm::Constant"*** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_5ValueEKPNS_8ConstantEPKS2_E4doitERS4_(%"class.llvm::Constant"** dereferenceable(8) %3)
  ret i1 %4
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm16cast_convert_valINS_5ValueEPNS_8ConstantES3_E4doitERKS3_(%"class.llvm::Constant"** dereferenceable(8)) #4 align 2 {
  %2 = alloca %"class.llvm::Constant"**, align 8
  %3 = alloca %"class.llvm::Value"*, align 8
  store %"class.llvm::Constant"** %0, %"class.llvm::Constant"*** %2, align 8
  %4 = load %"class.llvm::Constant"**, %"class.llvm::Constant"*** %2, align 8
  %5 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %4, align 8
  %6 = bitcast %"class.llvm::Constant"* %5 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %6, %"class.llvm::Value"** %3, align 8
  %7 = load %"class.llvm::Value"*, %"class.llvm::Value"** %3, align 8
  ret %"class.llvm::Value"* %7
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_5ValueEKPNS_8ConstantEPKS2_E4doitERS4_(%"class.llvm::Constant"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Constant"**, align 8
  %3 = alloca %"class.llvm::Constant"*, align 8
  store %"class.llvm::Constant"** %0, %"class.llvm::Constant"*** %2, align 8
  %4 = load %"class.llvm::Constant"**, %"class.llvm::Constant"*** %2, align 8
  %5 = call %"class.llvm::Constant"* @_ZN4llvm13simplify_typeIKPNS_8ConstantEE18getSimplifiedValueERS3_(%"class.llvm::Constant"** dereferenceable(8) %4)
  store %"class.llvm::Constant"* %5, %"class.llvm::Constant"** %3, align 8
  %6 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_5ValueEPKNS_8ConstantES4_E4doitERKS4_(%"class.llvm::Constant"** dereferenceable(8) %3)
  ret i1 %6
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_5ValueEPKNS_8ConstantES4_E4doitERKS4_(%"class.llvm::Constant"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Constant"**, align 8
  store %"class.llvm::Constant"** %0, %"class.llvm::Constant"*** %2, align 8
  %3 = load %"class.llvm::Constant"**, %"class.llvm::Constant"*** %2, align 8
  %4 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %3, align 8
  %5 = call zeroext i1 @_ZN4llvm11isa_impl_clINS_5ValueEPKNS_8ConstantEE4doitES4_(%"class.llvm::Constant"* %4)
  ret i1 %5
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZN4llvm13simplify_typeIKPNS_8ConstantEE18getSimplifiedValueERS3_(%"class.llvm::Constant"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Constant"**, align 8
  store %"class.llvm::Constant"** %0, %"class.llvm::Constant"*** %2, align 8
  %3 = load %"class.llvm::Constant"**, %"class.llvm::Constant"*** %2, align 8
  %4 = call dereferenceable(8) %"class.llvm::Constant"** @_ZN4llvm13simplify_typeIPNS_8ConstantEE18getSimplifiedValueERS2_(%"class.llvm::Constant"** dereferenceable(8) %3)
  %5 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %4, align 8
  ret %"class.llvm::Constant"* %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm11isa_impl_clINS_5ValueEPKNS_8ConstantEE4doitES4_(%"class.llvm::Constant"*) #3 align 2 {
  %2 = alloca %"class.llvm::Constant"*, align 8
  store %"class.llvm::Constant"* %0, %"class.llvm::Constant"** %2, align 8
  %3 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %2, align 8
  %4 = icmp ne %"class.llvm::Constant"* %3, null
  br i1 %4, label %5, label %6

; <label>:5:                                      ; preds = %1
  br label %6

; <label>:6:                                      ; preds = %5, %1
  %7 = phi i1 [ false, %1 ], [ true, %5 ]
  %8 = xor i1 %7, true
  br i1 %8, label %9, label %11

; <label>:9:                                      ; preds = %6
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 95, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.7, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %12

; <label>:11:                                     ; preds = %6
  br label %12

; <label>:12:                                     ; preds = %11, %10
  %13 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %2, align 8
  %14 = call zeroext i1 @_ZN4llvm8isa_implINS_5ValueENS_8ConstantEvE4doitERKS2_(%"class.llvm::Constant"* dereferenceable(32) %13)
  ret i1 %14
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm8isa_implINS_5ValueENS_8ConstantEvE4doitERKS2_(%"class.llvm::Constant"* dereferenceable(32)) #7 align 2 {
  %2 = alloca %"class.llvm::Constant"*, align 8
  store %"class.llvm::Constant"* %0, %"class.llvm::Constant"** %2, align 8
  ret i1 true
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr dereferenceable(8) %"class.llvm::Constant"** @_ZN4llvm13simplify_typeIPNS_8ConstantEE18getSimplifiedValueERS2_(%"class.llvm::Constant"** dereferenceable(8)) #4 align 2 {
  %2 = alloca %"class.llvm::Constant"**, align 8
  store %"class.llvm::Constant"** %0, %"class.llvm::Constant"*** %2, align 8
  %3 = load %"class.llvm::Constant"**, %"class.llvm::Constant"*** %2, align 8
  ret %"class.llvm::Constant"** %3
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm8OptionalIjEC2ERKS1_(%"class.llvm::Optional"*, %"class.llvm::Optional"* dereferenceable(8)) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::Optional"*, align 8
  %4 = alloca %"class.llvm::Optional"*, align 8
  store %"class.llvm::Optional"* %0, %"class.llvm::Optional"** %3, align 8
  store %"class.llvm::Optional"* %1, %"class.llvm::Optional"** %4, align 8
  %5 = load %"class.llvm::Optional"*, %"class.llvm::Optional"** %3, align 8
  %6 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %5, i32 0, i32 0
  %7 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %5, i32 0, i32 1
  %8 = load %"class.llvm::Optional"*, %"class.llvm::Optional"** %4, align 8
  %9 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %8, i32 0, i32 1
  %10 = load i8, i8* %9, align 4
  %11 = trunc i8 %10 to i1
  %12 = zext i1 %11 to i8
  store i8 %12, i8* %7, align 4
  %13 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %5, i32 0, i32 1
  %14 = load i8, i8* %13, align 4
  %15 = trunc i8 %14 to i1
  br i1 %15, label %16, label %25

; <label>:16:                                     ; preds = %2
  %17 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %5, i32 0, i32 0
  %18 = bitcast %"struct.llvm::AlignedCharArrayUnion.89"* %17 to %"struct.llvm::AlignedCharArray.90"*
  %19 = getelementptr inbounds %"struct.llvm::AlignedCharArray.90", %"struct.llvm::AlignedCharArray.90"* %18, i32 0, i32 0
  %20 = getelementptr inbounds [4 x i8], [4 x i8]* %19, i32 0, i32 0
  %21 = bitcast i8* %20 to i32*
  %22 = load %"class.llvm::Optional"*, %"class.llvm::Optional"** %4, align 8
  %23 = call dereferenceable(4) i32* @_ZNKR4llvm8OptionalIjEdeEv(%"class.llvm::Optional"* %22)
  %24 = load i32, i32* %23, align 4
  store i32 %24, i32* %21, align 4
  br label %25

; <label>:25:                                     ; preds = %16, %2
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr dereferenceable(4) i32* @_ZNKR4llvm8OptionalIjEdeEv(%"class.llvm::Optional"*) #0 align 2 {
  %2 = alloca %"class.llvm::Optional"*, align 8
  store %"class.llvm::Optional"* %0, %"class.llvm::Optional"** %2, align 8
  %3 = load %"class.llvm::Optional"*, %"class.llvm::Optional"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %3, i32 0, i32 1
  %5 = load i8, i8* %4, align 4
  %6 = trunc i8 %5 to i1
  %7 = xor i1 %6, true
  br i1 %7, label %8, label %10

; <label>:8:                                      ; preds = %1
  call void @__assert_rtn(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @__func__._ZNKR4llvm8OptionalIjEdeEv, i32 0, i32 0), i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.18, i32 0, i32 0), i32 128, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.19, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %11

; <label>:10:                                     ; preds = %1
  br label %11

; <label>:11:                                     ; preds = %10, %9
  %12 = call i32* @_ZNK4llvm8OptionalIjE10getPointerEv(%"class.llvm::Optional"* %3)
  ret i32* %12
}

; Function Attrs: ssp uwtable
define linkonce_odr i32* @_ZNK4llvm8OptionalIjE10getPointerEv(%"class.llvm::Optional"*) #0 align 2 {
  %2 = alloca %"class.llvm::Optional"*, align 8
  store %"class.llvm::Optional"* %0, %"class.llvm::Optional"** %2, align 8
  %3 = load %"class.llvm::Optional"*, %"class.llvm::Optional"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %3, i32 0, i32 1
  %5 = load i8, i8* %4, align 4
  %6 = trunc i8 %5 to i1
  %7 = xor i1 %6, true
  br i1 %7, label %8, label %10

; <label>:8:                                      ; preds = %1
  call void @__assert_rtn(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @__func__._ZNK4llvm8OptionalIjE10getPointerEv, i32 0, i32 0), i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.18, i32 0, i32 0), i32 119, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.19, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %11

; <label>:10:                                     ; preds = %1
  br label %11

; <label>:11:                                     ; preds = %10, %9
  %12 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %3, i32 0, i32 0
  %13 = bitcast %"struct.llvm::AlignedCharArrayUnion.89"* %12 to %"struct.llvm::AlignedCharArray.90"*
  %14 = getelementptr inbounds %"struct.llvm::AlignedCharArray.90", %"struct.llvm::AlignedCharArray.90"* %13, i32 0, i32 0
  %15 = getelementptr inbounds [4 x i8], [4 x i8]* %14, i32 0, i32 0
  %16 = bitcast i8* %15 to i32*
  ret i32* %16
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm8OptionalIjEC2ENS_8NoneTypeE(%"class.llvm::Optional"*, i32) unnamed_addr #4 align 2 {
  %3 = alloca %"class.llvm::Optional"*, align 8
  %4 = alloca i32, align 4
  store %"class.llvm::Optional"* %0, %"class.llvm::Optional"** %3, align 8
  store i32 %1, i32* %4, align 4
  %5 = load %"class.llvm::Optional"*, %"class.llvm::Optional"** %3, align 8
  %6 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %5, i32 0, i32 0
  %7 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %5, i32 0, i32 1
  store i8 0, i8* %7, align 4
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm8OptionalIjED2Ev(%"class.llvm::Optional"*) unnamed_addr #4 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"class.llvm::Optional"*, align 8
  store %"class.llvm::Optional"* %0, %"class.llvm::Optional"** %2, align 8
  %3 = load %"class.llvm::Optional"*, %"class.llvm::Optional"** %2, align 8
  invoke void @_ZN4llvm8OptionalIjE5resetEv(%"class.llvm::Optional"* %3)
          to label %4 unwind label %5

; <label>:4:                                      ; preds = %1
  ret void

; <label>:5:                                      ; preds = %1
  %6 = landingpad { i8*, i32 }
          catch i8* null
  %7 = extractvalue { i8*, i32 } %6, 0
  call void @__clang_call_terminate(i8* %7) #16
  unreachable
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm8OptionalIjE5resetEv(%"class.llvm::Optional"*) #0 align 2 {
  %2 = alloca %"class.llvm::Optional"*, align 8
  store %"class.llvm::Optional"* %0, %"class.llvm::Optional"** %2, align 8
  %3 = load %"class.llvm::Optional"*, %"class.llvm::Optional"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %3, i32 0, i32 1
  %5 = load i8, i8* %4, align 4
  %6 = trunc i8 %5 to i1
  br i1 %6, label %7, label %11

; <label>:7:                                      ; preds = %1
  %8 = call dereferenceable(4) i32* @_ZNR4llvm8OptionalIjEdeEv(%"class.llvm::Optional"* %3)
  %9 = load i32, i32* %8, align 4
  %10 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %3, i32 0, i32 1
  store i8 0, i8* %10, align 4
  br label %11

; <label>:11:                                     ; preds = %7, %1
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr dereferenceable(4) i32* @_ZNR4llvm8OptionalIjEdeEv(%"class.llvm::Optional"*) #0 align 2 {
  %2 = alloca %"class.llvm::Optional"*, align 8
  store %"class.llvm::Optional"* %0, %"class.llvm::Optional"** %2, align 8
  %3 = load %"class.llvm::Optional"*, %"class.llvm::Optional"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %3, i32 0, i32 1
  %5 = load i8, i8* %4, align 4
  %6 = trunc i8 %5 to i1
  %7 = xor i1 %6, true
  br i1 %7, label %8, label %10

; <label>:8:                                      ; preds = %1
  call void @__assert_rtn(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @__func__._ZNKR4llvm8OptionalIjEdeEv, i32 0, i32 0), i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.18, i32 0, i32 0), i32 129, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.19, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %11

; <label>:10:                                     ; preds = %1
  br label %11

; <label>:11:                                     ; preds = %10, %9
  %12 = call i32* @_ZN4llvm8OptionalIjE10getPointerEv(%"class.llvm::Optional"* %3)
  ret i32* %12
}

; Function Attrs: ssp uwtable
define linkonce_odr i32* @_ZN4llvm8OptionalIjE10getPointerEv(%"class.llvm::Optional"*) #0 align 2 {
  %2 = alloca %"class.llvm::Optional"*, align 8
  store %"class.llvm::Optional"* %0, %"class.llvm::Optional"** %2, align 8
  %3 = load %"class.llvm::Optional"*, %"class.llvm::Optional"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %3, i32 0, i32 1
  %5 = load i8, i8* %4, align 4
  %6 = trunc i8 %5 to i1
  %7 = xor i1 %6, true
  br i1 %7, label %8, label %10

; <label>:8:                                      ; preds = %1
  call void @__assert_rtn(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @__func__._ZNK4llvm8OptionalIjE10getPointerEv, i32 0, i32 0), i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.18, i32 0, i32 0), i32 120, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.19, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %11

; <label>:10:                                     ; preds = %1
  br label %11

; <label>:11:                                     ; preds = %10, %9
  %12 = getelementptr inbounds %"class.llvm::Optional", %"class.llvm::Optional"* %3, i32 0, i32 0
  %13 = bitcast %"struct.llvm::AlignedCharArrayUnion.89"* %12 to %"struct.llvm::AlignedCharArray.90"*
  %14 = getelementptr inbounds %"struct.llvm::AlignedCharArray.90", %"struct.llvm::AlignedCharArray.90"* %13, i32 0, i32 0
  %15 = getelementptr inbounds [4 x i8], [4 x i8]* %14, i32 0, i32 0
  %16 = bitcast i8* %15 to i32*
  ret i32* %16
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNK4llvm8ArrayRefIPNS_5ValueEE4sizeEv(%"class.llvm::ArrayRef.36"*) #4 align 2 {
  %2 = alloca %"class.llvm::ArrayRef.36"*, align 8
  store %"class.llvm::ArrayRef.36"* %0, %"class.llvm::ArrayRef.36"** %2, align 8
  %3 = load %"class.llvm::ArrayRef.36"*, %"class.llvm::ArrayRef.36"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::ArrayRef.36", %"class.llvm::ArrayRef.36"* %3, i32 0, i32 1
  %5 = load i64, i64* %4, align 8
  ret i64 %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr %"class.llvm::PointerType"* @_ZN4llvm4castINS_11PointerTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Type"*) #3 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = call zeroext i1 @_ZN4llvm3isaINS_11PointerTypeEPNS_4TypeEEEbRKT0_(%"class.llvm::Type"** dereferenceable(8) %2)
  br i1 %3, label %4, label %5

; <label>:4:                                      ; preds = %1
  br label %5

; <label>:5:                                      ; preds = %4, %1
  %6 = phi i1 [ false, %1 ], [ true, %4 ]
  %7 = xor i1 %6, true
  br i1 %7, label %8, label %10

; <label>:8:                                      ; preds = %5
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm4castINS_10StructTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 236, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.9, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %11

; <label>:10:                                     ; preds = %5
  br label %11

; <label>:11:                                     ; preds = %10, %9
  %12 = call %"class.llvm::PointerType"* @_ZN4llvm16cast_convert_valINS_11PointerTypeEPNS_4TypeES3_E4doitERKS3_(%"class.llvm::Type"** dereferenceable(8) %2)
  ret %"class.llvm::PointerType"* %12
}

; Function Attrs: nounwind readonly
declare %"class.llvm::Type"* @_ZNK4llvm4Type13getScalarTypeEv(%"class.llvm::Type"*) #9

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::Type"* @_ZNK4llvm11PointerType14getElementTypeEv(%"class.llvm::PointerType"*) #4 align 2 {
  %2 = alloca %"class.llvm::PointerType"*, align 8
  store %"class.llvm::PointerType"* %0, %"class.llvm::PointerType"** %2, align 8
  %3 = load %"class.llvm::PointerType"*, %"class.llvm::PointerType"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::PointerType", %"class.llvm::PointerType"* %3, i32 0, i32 1
  %5 = load %"class.llvm::Type"*, %"class.llvm::Type"** %4, align 8
  ret %"class.llvm::Type"* %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZN4llvm17GetElementPtrInstC1EPNS_4TypeEPNS_5ValueENS_8ArrayRefIS4_EEjRKNS_5TwineEPNS_11InstructionE(%"class.llvm::GetElementPtrInst"*, %"class.llvm::Type"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, i32, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::Instruction"*) unnamed_addr #3 align 2 {
  %9 = alloca %"class.llvm::ArrayRef.36", align 8
  %10 = alloca %"class.llvm::GetElementPtrInst"*, align 8
  %11 = alloca %"class.llvm::Type"*, align 8
  %12 = alloca %"class.llvm::Value"*, align 8
  %13 = alloca i32, align 4
  %14 = alloca %"class.llvm::Twine"*, align 8
  %15 = alloca %"class.llvm::Instruction"*, align 8
  %16 = bitcast %"class.llvm::ArrayRef.36"* %9 to { %"class.llvm::Value"**, i64 }*
  %17 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %16, i32 0, i32 0
  store %"class.llvm::Value"** %3, %"class.llvm::Value"*** %17, align 8
  %18 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %16, i32 0, i32 1
  store i64 %4, i64* %18, align 8
  store %"class.llvm::GetElementPtrInst"* %0, %"class.llvm::GetElementPtrInst"** %10, align 8
  store %"class.llvm::Type"* %1, %"class.llvm::Type"** %11, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %12, align 8
  store i32 %5, i32* %13, align 4
  store %"class.llvm::Twine"* %6, %"class.llvm::Twine"** %14, align 8
  store %"class.llvm::Instruction"* %7, %"class.llvm::Instruction"** %15, align 8
  %19 = load %"class.llvm::GetElementPtrInst"*, %"class.llvm::GetElementPtrInst"** %10, align 8
  %20 = load %"class.llvm::Type"*, %"class.llvm::Type"** %11, align 8
  %21 = load %"class.llvm::Value"*, %"class.llvm::Value"** %12, align 8
  %22 = load i32, i32* %13, align 4
  %23 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %14, align 8
  %24 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %15, align 8
  %25 = bitcast %"class.llvm::ArrayRef.36"* %9 to { %"class.llvm::Value"**, i64 }*
  %26 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %25, i32 0, i32 0
  %27 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %26, align 8
  %28 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %25, i32 0, i32 1
  %29 = load i64, i64* %28, align 8
  call void @_ZN4llvm17GetElementPtrInstC2EPNS_4TypeEPNS_5ValueENS_8ArrayRefIS4_EEjRKNS_5TwineEPNS_11InstructionE(%"class.llvm::GetElementPtrInst"* %19, %"class.llvm::Type"* %20, %"class.llvm::Value"* %21, %"class.llvm::Value"** %27, i64 %29, i32 %22, %"class.llvm::Twine"* dereferenceable(24) %23, %"class.llvm::Instruction"* %24)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm4UserdlEPvj(i8*, i32) #4 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca i8*, align 8
  %4 = alloca i32, align 4
  store i8* %0, i8** %3, align 8
  store i32 %1, i32* %4, align 4
  invoke void @_ZN4llvm25llvm_unreachable_internalEPKcS1_j(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.25, i32 0, i32 0), i8* getelementptr inbounds ([54 x i8], [54 x i8]* @.str.26, i32 0, i32 0), i32 104) #15
          to label %5 unwind label %7

; <label>:5:                                      ; preds = %2
  unreachable
                                                  ; No predecessors!
  ret void

; <label>:7:                                      ; preds = %2
  %8 = landingpad { i8*, i32 }
          catch i8* null
  %9 = extractvalue { i8*, i32 } %8, 0
  call void @__clang_call_terminate(i8* %9) #16
  unreachable
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm3isaINS_11PointerTypeEPNS_4TypeEEEbRKT0_(%"class.llvm::Type"** dereferenceable(8)) #3 {
  %2 = alloca %"class.llvm::Type"**, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %3 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_11PointerTypeEKPNS_4TypeEPKS2_E4doitERS4_(%"class.llvm::Type"** dereferenceable(8) %3)
  ret i1 %4
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::PointerType"* @_ZN4llvm16cast_convert_valINS_11PointerTypeEPNS_4TypeES3_E4doitERKS3_(%"class.llvm::Type"** dereferenceable(8)) #4 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  %3 = alloca %"class.llvm::PointerType"*, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %4 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %5 = load %"class.llvm::Type"*, %"class.llvm::Type"** %4, align 8
  %6 = bitcast %"class.llvm::Type"* %5 to %"class.llvm::PointerType"*
  store %"class.llvm::PointerType"* %6, %"class.llvm::PointerType"** %3, align 8
  %7 = load %"class.llvm::PointerType"*, %"class.llvm::PointerType"** %3, align 8
  ret %"class.llvm::PointerType"* %7
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_11PointerTypeEKPNS_4TypeEPKS2_E4doitERS4_(%"class.llvm::Type"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  %3 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %4 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %5 = call %"class.llvm::Type"* @_ZN4llvm13simplify_typeIKPNS_4TypeEE18getSimplifiedValueERS3_(%"class.llvm::Type"** dereferenceable(8) %4)
  store %"class.llvm::Type"* %5, %"class.llvm::Type"** %3, align 8
  %6 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_11PointerTypeEPKNS_4TypeES4_E4doitERKS4_(%"class.llvm::Type"** dereferenceable(8) %3)
  ret i1 %6
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_11PointerTypeEPKNS_4TypeES4_E4doitERKS4_(%"class.llvm::Type"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %3 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %4 = load %"class.llvm::Type"*, %"class.llvm::Type"** %3, align 8
  %5 = call zeroext i1 @_ZN4llvm11isa_impl_clINS_11PointerTypeEPKNS_4TypeEE4doitES4_(%"class.llvm::Type"* %4)
  ret i1 %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm11isa_impl_clINS_11PointerTypeEPKNS_4TypeEE4doitES4_(%"class.llvm::Type"*) #3 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = icmp ne %"class.llvm::Type"* %3, null
  br i1 %4, label %5, label %6

; <label>:5:                                      ; preds = %1
  br label %6

; <label>:6:                                      ; preds = %5, %1
  %7 = phi i1 [ false, %1 ], [ true, %5 ]
  %8 = xor i1 %7, true
  br i1 %8, label %9, label %11

; <label>:9:                                      ; preds = %6
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 95, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.7, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %12

; <label>:11:                                     ; preds = %6
  br label %12

; <label>:12:                                     ; preds = %11, %10
  %13 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %14 = call zeroext i1 @_ZN4llvm8isa_implINS_11PointerTypeENS_4TypeEvE4doitERKS2_(%"class.llvm::Type"* dereferenceable(24) %13)
  ret i1 %14
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm8isa_implINS_11PointerTypeENS_4TypeEvE4doitERKS2_(%"class.llvm::Type"* dereferenceable(24)) #7 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call i32 @_ZNK4llvm4Type9getTypeIDEv(%"class.llvm::Type"* %3)
  %5 = icmp eq i32 %4, 15
  ret i1 %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZN4llvm17GetElementPtrInstC2EPNS_4TypeEPNS_5ValueENS_8ArrayRefIS4_EEjRKNS_5TwineEPNS_11InstructionE(%"class.llvm::GetElementPtrInst"*, %"class.llvm::Type"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, i32, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::Instruction"*) unnamed_addr #3 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %9 = alloca %"class.llvm::ArrayRef.36", align 8
  %10 = alloca %"class.llvm::GetElementPtrInst"*, align 8
  %11 = alloca %"class.llvm::Type"*, align 8
  %12 = alloca %"class.llvm::Value"*, align 8
  %13 = alloca i32, align 4
  %14 = alloca %"class.llvm::Twine"*, align 8
  %15 = alloca %"class.llvm::Instruction"*, align 8
  %16 = alloca %"class.llvm::ArrayRef.36", align 8
  %17 = alloca %"class.llvm::ArrayRef.36", align 8
  %18 = alloca i8*
  %19 = alloca i32
  %20 = alloca %"class.llvm::ArrayRef.36", align 8
  %21 = bitcast %"class.llvm::ArrayRef.36"* %9 to { %"class.llvm::Value"**, i64 }*
  %22 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %21, i32 0, i32 0
  store %"class.llvm::Value"** %3, %"class.llvm::Value"*** %22, align 8
  %23 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %21, i32 0, i32 1
  store i64 %4, i64* %23, align 8
  store %"class.llvm::GetElementPtrInst"* %0, %"class.llvm::GetElementPtrInst"** %10, align 8
  store %"class.llvm::Type"* %1, %"class.llvm::Type"** %11, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %12, align 8
  store i32 %5, i32* %13, align 4
  store %"class.llvm::Twine"* %6, %"class.llvm::Twine"** %14, align 8
  store %"class.llvm::Instruction"* %7, %"class.llvm::Instruction"** %15, align 8
  %24 = load %"class.llvm::GetElementPtrInst"*, %"class.llvm::GetElementPtrInst"** %10, align 8
  %25 = bitcast %"class.llvm::GetElementPtrInst"* %24 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([5 x i8*], [5 x i8*]* @_ZTVN4llvm17GetElementPtrInstE, i32 0, i32 2) to i32 (...)**), i32 (...)*** %25, align 8
  %26 = bitcast %"class.llvm::GetElementPtrInst"* %24 to %"class.llvm::Instruction"*
  %27 = load %"class.llvm::Type"*, %"class.llvm::Type"** %11, align 8
  %28 = load %"class.llvm::Value"*, %"class.llvm::Value"** %12, align 8
  %29 = bitcast %"class.llvm::ArrayRef.36"* %16 to i8*
  %30 = bitcast %"class.llvm::ArrayRef.36"* %9 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %29, i8* %30, i64 16, i32 8, i1 false)
  %31 = bitcast %"class.llvm::ArrayRef.36"* %16 to { %"class.llvm::Value"**, i64 }*
  %32 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %31, i32 0, i32 0
  %33 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %32, align 8
  %34 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %31, i32 0, i32 1
  %35 = load i64, i64* %34, align 8
  %36 = call %"class.llvm::Type"* @_ZN4llvm17GetElementPtrInst16getGEPReturnTypeEPNS_4TypeEPNS_5ValueENS_8ArrayRefIS4_EE(%"class.llvm::Type"* %27, %"class.llvm::Value"* %28, %"class.llvm::Value"** %33, i64 %35)
  %37 = call %"class.llvm::Use"* @_ZN4llvm21VariadicOperandTraitsINS_17GetElementPtrInstELj1EE6op_endEPS1_(%"class.llvm::GetElementPtrInst"* %24)
  %38 = load i32, i32* %13, align 4
  %39 = zext i32 %38 to i64
  %40 = sub i64 0, %39
  %41 = getelementptr inbounds %"class.llvm::Use", %"class.llvm::Use"* %37, i64 %40
  %42 = load i32, i32* %13, align 4
  %43 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %15, align 8
  call void @_ZN4llvm11InstructionC2EPNS_4TypeEjPNS_3UseEjPS0_(%"class.llvm::Instruction"* %26, %"class.llvm::Type"* %36, i32 32, %"class.llvm::Use"* %41, i32 %42, %"class.llvm::Instruction"* %43)
  %44 = bitcast %"class.llvm::GetElementPtrInst"* %24 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([5 x i8*], [5 x i8*]* @_ZTVN4llvm17GetElementPtrInstE, i32 0, i32 2) to i32 (...)**), i32 (...)*** %44, align 8
  %45 = getelementptr inbounds %"class.llvm::GetElementPtrInst", %"class.llvm::GetElementPtrInst"* %24, i32 0, i32 1
  %46 = load %"class.llvm::Type"*, %"class.llvm::Type"** %11, align 8
  store %"class.llvm::Type"* %46, %"class.llvm::Type"** %45, align 8
  %47 = getelementptr inbounds %"class.llvm::GetElementPtrInst", %"class.llvm::GetElementPtrInst"* %24, i32 0, i32 2
  %48 = load %"class.llvm::Type"*, %"class.llvm::Type"** %11, align 8
  %49 = bitcast %"class.llvm::ArrayRef.36"* %17 to i8*
  %50 = bitcast %"class.llvm::ArrayRef.36"* %9 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %49, i8* %50, i64 16, i32 8, i1 false)
  %51 = bitcast %"class.llvm::ArrayRef.36"* %17 to { %"class.llvm::Value"**, i64 }*
  %52 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %51, i32 0, i32 0
  %53 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %52, align 8
  %54 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %51, i32 0, i32 1
  %55 = load i64, i64* %54, align 8
  %56 = invoke %"class.llvm::Type"* @_ZN4llvm17GetElementPtrInst14getIndexedTypeEPNS_4TypeENS_8ArrayRefIPNS_5ValueEEE(%"class.llvm::Type"* %48, %"class.llvm::Value"** %53, i64 %55)
          to label %57 unwind label %72

; <label>:57:                                     ; preds = %8
  store %"class.llvm::Type"* %56, %"class.llvm::Type"** %47, align 8
  %58 = getelementptr inbounds %"class.llvm::GetElementPtrInst", %"class.llvm::GetElementPtrInst"* %24, i32 0, i32 2
  %59 = load %"class.llvm::Type"*, %"class.llvm::Type"** %58, align 8
  %60 = bitcast %"class.llvm::GetElementPtrInst"* %24 to %"class.llvm::Value"*
  %61 = invoke %"class.llvm::Type"* @_ZNK4llvm5Value7getTypeEv(%"class.llvm::Value"* %60)
          to label %62 unwind label %72

; <label>:62:                                     ; preds = %57
  %63 = call %"class.llvm::Type"* @_ZNK4llvm4Type13getScalarTypeEv(%"class.llvm::Type"* %61) #18
  %64 = invoke %"class.llvm::PointerType"* @_ZN4llvm4castINS_11PointerTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Type"* %63)
          to label %65 unwind label %72

; <label>:65:                                     ; preds = %62
  %66 = invoke %"class.llvm::Type"* @_ZNK4llvm11PointerType14getElementTypeEv(%"class.llvm::PointerType"* %64)
          to label %67 unwind label %72

; <label>:67:                                     ; preds = %65
  %68 = icmp eq %"class.llvm::Type"* %59, %66
  %69 = xor i1 %68, true
  br i1 %69, label %70, label %78

; <label>:70:                                     ; preds = %67
  invoke void @__assert_rtn(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @__func__._ZN4llvm17GetElementPtrInstC2EPNS_4TypeEPNS_5ValueENS_8ArrayRefIS4_EEjRKNS_5TwineEPNS_11InstructionE, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.20, i32 0, i32 0), i32 1066, i8* getelementptr inbounds ([85 x i8], [85 x i8]* @.str.22, i32 0, i32 0)) #15
          to label %71 unwind label %72

; <label>:71:                                     ; preds = %70
  unreachable

; <label>:72:                                     ; preds = %79, %70, %65, %62, %57, %8
  %73 = landingpad { i8*, i32 }
          cleanup
  %74 = extractvalue { i8*, i32 } %73, 0
  store i8* %74, i8** %18, align 8
  %75 = extractvalue { i8*, i32 } %73, 1
  store i32 %75, i32* %19, align 4
  %76 = bitcast %"class.llvm::GetElementPtrInst"* %24 to %"class.llvm::Instruction"*
  call void @_ZN4llvm11InstructionD2Ev(%"class.llvm::Instruction"* %76) #14
  br label %90
                                                  ; No predecessors!
  br label %79

; <label>:78:                                     ; preds = %67
  br label %79

; <label>:79:                                     ; preds = %78, %77
  %80 = load %"class.llvm::Value"*, %"class.llvm::Value"** %12, align 8
  %81 = bitcast %"class.llvm::ArrayRef.36"* %20 to i8*
  %82 = bitcast %"class.llvm::ArrayRef.36"* %9 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %81, i8* %82, i64 16, i32 8, i1 false)
  %83 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %14, align 8
  %84 = bitcast %"class.llvm::ArrayRef.36"* %20 to { %"class.llvm::Value"**, i64 }*
  %85 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %84, i32 0, i32 0
  %86 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %85, align 8
  %87 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %84, i32 0, i32 1
  %88 = load i64, i64* %87, align 8
  invoke void @_ZN4llvm17GetElementPtrInst4initEPNS_5ValueENS_8ArrayRefIS2_EERKNS_5TwineE(%"class.llvm::GetElementPtrInst"* %24, %"class.llvm::Value"* %80, %"class.llvm::Value"** %86, i64 %88, %"class.llvm::Twine"* dereferenceable(24) %83)
          to label %89 unwind label %72

; <label>:89:                                     ; preds = %79
  ret void

; <label>:90:                                     ; preds = %72
  %91 = load i8*, i8** %18, align 8
  %92 = load i32, i32* %19, align 4
  %93 = insertvalue { i8*, i32 } undef, i8* %91, 0
  %94 = insertvalue { i8*, i32 } %93, i32 %92, 1
  resume { i8*, i32 } %94
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Type"* @_ZN4llvm17GetElementPtrInst16getGEPReturnTypeEPNS_4TypeEPNS_5ValueENS_8ArrayRefIS4_EE(%"class.llvm::Type"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64) #0 align 2 {
  %5 = alloca %"class.llvm::Type"*, align 8
  %6 = alloca %"class.llvm::ArrayRef.36", align 8
  %7 = alloca %"class.llvm::Type"*, align 8
  %8 = alloca %"class.llvm::Value"*, align 8
  %9 = alloca %"class.llvm::Type"*, align 8
  %10 = alloca %"class.llvm::ArrayRef.36", align 8
  %11 = alloca i32, align 4
  %12 = alloca %"class.llvm::ArrayRef.36"*, align 8
  %13 = alloca %"class.llvm::Value"**, align 8
  %14 = alloca %"class.llvm::Value"**, align 8
  %15 = alloca %"class.llvm::Value"*, align 8
  %16 = alloca i32, align 4
  %17 = bitcast %"class.llvm::ArrayRef.36"* %6 to { %"class.llvm::Value"**, i64 }*
  %18 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %17, i32 0, i32 0
  store %"class.llvm::Value"** %2, %"class.llvm::Value"*** %18, align 8
  %19 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %17, i32 0, i32 1
  store i64 %3, i64* %19, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %7, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %8, align 8
  %20 = load %"class.llvm::Type"*, %"class.llvm::Type"** %7, align 8
  %21 = bitcast %"class.llvm::ArrayRef.36"* %10 to i8*
  %22 = bitcast %"class.llvm::ArrayRef.36"* %6 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %21, i8* %22, i64 16, i32 8, i1 false)
  %23 = bitcast %"class.llvm::ArrayRef.36"* %10 to { %"class.llvm::Value"**, i64 }*
  %24 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %23, i32 0, i32 0
  %25 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %24, align 8
  %26 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %23, i32 0, i32 1
  %27 = load i64, i64* %26, align 8
  %28 = call %"class.llvm::Type"* @_ZN4llvm17GetElementPtrInst14getIndexedTypeEPNS_4TypeENS_8ArrayRefIPNS_5ValueEEE(%"class.llvm::Type"* %20, %"class.llvm::Value"** %25, i64 %27)
  %29 = call %"class.llvm::Type"* @_ZN4llvm12checkGEPTypeEPNS_4TypeE(%"class.llvm::Type"* %28)
  %30 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %31 = call %"class.llvm::Type"* @_ZNK4llvm5Value7getTypeEv(%"class.llvm::Value"* %30)
  %32 = call i32 @_ZNK4llvm4Type22getPointerAddressSpaceEv(%"class.llvm::Type"* %31)
  %33 = call %"class.llvm::PointerType"* @_ZN4llvm11PointerType3getEPNS_4TypeEj(%"class.llvm::Type"* %29, i32 %32)
  %34 = bitcast %"class.llvm::PointerType"* %33 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %34, %"class.llvm::Type"** %9, align 8
  %35 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %36 = call %"class.llvm::Type"* @_ZNK4llvm5Value7getTypeEv(%"class.llvm::Value"* %35)
  %37 = call zeroext i1 @_ZNK4llvm4Type10isVectorTyEv(%"class.llvm::Type"* %36)
  br i1 %37, label %38, label %46

; <label>:38:                                     ; preds = %4
  %39 = load %"class.llvm::Value"*, %"class.llvm::Value"** %8, align 8
  %40 = call %"class.llvm::Type"* @_ZNK4llvm5Value7getTypeEv(%"class.llvm::Value"* %39)
  %41 = call i32 @_ZNK4llvm4Type20getVectorNumElementsEv(%"class.llvm::Type"* %40)
  store i32 %41, i32* %11, align 4
  %42 = load %"class.llvm::Type"*, %"class.llvm::Type"** %9, align 8
  %43 = load i32, i32* %11, align 4
  %44 = call %"class.llvm::VectorType"* @_ZN4llvm10VectorType3getEPNS_4TypeEj(%"class.llvm::Type"* %42, i32 %43)
  %45 = bitcast %"class.llvm::VectorType"* %44 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %45, %"class.llvm::Type"** %5, align 8
  br label %75

; <label>:46:                                     ; preds = %4
  store %"class.llvm::ArrayRef.36"* %6, %"class.llvm::ArrayRef.36"** %12, align 8
  %47 = load %"class.llvm::ArrayRef.36"*, %"class.llvm::ArrayRef.36"** %12, align 8
  %48 = call %"class.llvm::Value"** @_ZNK4llvm8ArrayRefIPNS_5ValueEE5beginEv(%"class.llvm::ArrayRef.36"* %47)
  store %"class.llvm::Value"** %48, %"class.llvm::Value"*** %13, align 8
  %49 = load %"class.llvm::ArrayRef.36"*, %"class.llvm::ArrayRef.36"** %12, align 8
  %50 = call %"class.llvm::Value"** @_ZNK4llvm8ArrayRefIPNS_5ValueEE3endEv(%"class.llvm::ArrayRef.36"* %49)
  store %"class.llvm::Value"** %50, %"class.llvm::Value"*** %14, align 8
  br label %51

; <label>:51:                                     ; preds = %70, %46
  %52 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %13, align 8
  %53 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %14, align 8
  %54 = icmp ne %"class.llvm::Value"** %52, %53
  br i1 %54, label %55, label %73

; <label>:55:                                     ; preds = %51
  %56 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %13, align 8
  %57 = load %"class.llvm::Value"*, %"class.llvm::Value"** %56, align 8
  store %"class.llvm::Value"* %57, %"class.llvm::Value"** %15, align 8
  %58 = load %"class.llvm::Value"*, %"class.llvm::Value"** %15, align 8
  %59 = call %"class.llvm::Type"* @_ZNK4llvm5Value7getTypeEv(%"class.llvm::Value"* %58)
  %60 = call zeroext i1 @_ZNK4llvm4Type10isVectorTyEv(%"class.llvm::Type"* %59)
  br i1 %60, label %61, label %69

; <label>:61:                                     ; preds = %55
  %62 = load %"class.llvm::Value"*, %"class.llvm::Value"** %15, align 8
  %63 = call %"class.llvm::Type"* @_ZNK4llvm5Value7getTypeEv(%"class.llvm::Value"* %62)
  %64 = call i32 @_ZNK4llvm4Type20getVectorNumElementsEv(%"class.llvm::Type"* %63)
  store i32 %64, i32* %16, align 4
  %65 = load %"class.llvm::Type"*, %"class.llvm::Type"** %9, align 8
  %66 = load i32, i32* %16, align 4
  %67 = call %"class.llvm::VectorType"* @_ZN4llvm10VectorType3getEPNS_4TypeEj(%"class.llvm::Type"* %65, i32 %66)
  %68 = bitcast %"class.llvm::VectorType"* %67 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %68, %"class.llvm::Type"** %5, align 8
  br label %75

; <label>:69:                                     ; preds = %55
  br label %70

; <label>:70:                                     ; preds = %69
  %71 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %13, align 8
  %72 = getelementptr inbounds %"class.llvm::Value"*, %"class.llvm::Value"** %71, i32 1
  store %"class.llvm::Value"** %72, %"class.llvm::Value"*** %13, align 8
  br label %51

; <label>:73:                                     ; preds = %51
  %74 = load %"class.llvm::Type"*, %"class.llvm::Type"** %9, align 8
  store %"class.llvm::Type"* %74, %"class.llvm::Type"** %5, align 8
  br label %75

; <label>:75:                                     ; preds = %73, %61, %38
  %76 = load %"class.llvm::Type"*, %"class.llvm::Type"** %5, align 8
  ret %"class.llvm::Type"* %76
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::Use"* @_ZN4llvm21VariadicOperandTraitsINS_17GetElementPtrInstELj1EE6op_endEPS1_(%"class.llvm::GetElementPtrInst"*) #4 align 2 {
  %2 = alloca %"class.llvm::GetElementPtrInst"*, align 8
  store %"class.llvm::GetElementPtrInst"* %0, %"class.llvm::GetElementPtrInst"** %2, align 8
  %3 = load %"class.llvm::GetElementPtrInst"*, %"class.llvm::GetElementPtrInst"** %2, align 8
  %4 = bitcast %"class.llvm::GetElementPtrInst"* %3 to %"class.llvm::Use"*
  ret %"class.llvm::Use"* %4
}

declare void @_ZN4llvm11InstructionC2EPNS_4TypeEjPNS_3UseEjPS0_(%"class.llvm::Instruction"*, %"class.llvm::Type"*, i32, %"class.llvm::Use"*, i32, %"class.llvm::Instruction"*) unnamed_addr #1

declare %"class.llvm::Type"* @_ZN4llvm17GetElementPtrInst14getIndexedTypeEPNS_4TypeENS_8ArrayRefIPNS_5ValueEEE(%"class.llvm::Type"*, %"class.llvm::Value"**, i64) #1

declare void @_ZN4llvm17GetElementPtrInst4initEPNS_5ValueENS_8ArrayRefIS2_EERKNS_5TwineE(%"class.llvm::GetElementPtrInst"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, %"class.llvm::Twine"* dereferenceable(24)) #1

; Function Attrs: nounwind
declare void @_ZN4llvm11InstructionD2Ev(%"class.llvm::Instruction"*) unnamed_addr #2

declare %"class.llvm::PointerType"* @_ZN4llvm11PointerType3getEPNS_4TypeEj(%"class.llvm::Type"*, i32) #1

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr %"class.llvm::Type"* @_ZN4llvm12checkGEPTypeEPNS_4TypeE(%"class.llvm::Type"*) #3 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = icmp ne %"class.llvm::Type"* %3, null
  br i1 %4, label %5, label %6

; <label>:5:                                      ; preds = %1
  br label %6

; <label>:6:                                      ; preds = %5, %1
  %7 = phi i1 [ false, %1 ], [ true, %5 ]
  %8 = xor i1 %7, true
  br i1 %8, label %9, label %11

; <label>:9:                                      ; preds = %6
  call void @__assert_rtn(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @__func__._ZN4llvm12checkGEPTypeEPNS_4TypeE, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.20, i32 0, i32 0), i32 823, i8* getelementptr inbounds ([52 x i8], [52 x i8]* @.str.24, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %12

; <label>:11:                                     ; preds = %6
  br label %12

; <label>:12:                                     ; preds = %11, %10
  %13 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  ret %"class.llvm::Type"* %13
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr i32 @_ZNK4llvm4Type22getPointerAddressSpaceEv(%"class.llvm::Type"*) #3 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call %"class.llvm::Type"* @_ZNK4llvm4Type13getScalarTypeEv(%"class.llvm::Type"* %3) #18
  %5 = call %"class.llvm::PointerType"* @_ZN4llvm4castINS_11PointerTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Type"* %4)
  %6 = call i32 @_ZNK4llvm11PointerType15getAddressSpaceEv(%"class.llvm::PointerType"* %5)
  ret i32 %6
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZNK4llvm4Type10isVectorTyEv(%"class.llvm::Type"*) #4 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call i32 @_ZNK4llvm4Type9getTypeIDEv(%"class.llvm::Type"* %3)
  %5 = icmp eq i32 %4, 16
  ret i1 %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr i32 @_ZNK4llvm4Type20getVectorNumElementsEv(%"class.llvm::Type"*) #3 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call %"class.llvm::VectorType"* @_ZN4llvm4castINS_10VectorTypeEKNS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES7_(%"class.llvm::Type"* %3)
  %5 = bitcast %"class.llvm::VectorType"* %4 to %"class.llvm::SequentialType"*
  %6 = call i64 @_ZNK4llvm14SequentialType14getNumElementsEv(%"class.llvm::SequentialType"* %5)
  %7 = trunc i64 %6 to i32
  ret i32 %7
}

declare %"class.llvm::VectorType"* @_ZN4llvm10VectorType3getEPNS_4TypeEj(%"class.llvm::Type"*, i32) #1

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::Value"** @_ZNK4llvm8ArrayRefIPNS_5ValueEE5beginEv(%"class.llvm::ArrayRef.36"*) #4 align 2 {
  %2 = alloca %"class.llvm::ArrayRef.36"*, align 8
  store %"class.llvm::ArrayRef.36"* %0, %"class.llvm::ArrayRef.36"** %2, align 8
  %3 = load %"class.llvm::ArrayRef.36"*, %"class.llvm::ArrayRef.36"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::ArrayRef.36", %"class.llvm::ArrayRef.36"* %3, i32 0, i32 0
  %5 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %4, align 8
  ret %"class.llvm::Value"** %5
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::Value"** @_ZNK4llvm8ArrayRefIPNS_5ValueEE3endEv(%"class.llvm::ArrayRef.36"*) #4 align 2 {
  %2 = alloca %"class.llvm::ArrayRef.36"*, align 8
  store %"class.llvm::ArrayRef.36"* %0, %"class.llvm::ArrayRef.36"** %2, align 8
  %3 = load %"class.llvm::ArrayRef.36"*, %"class.llvm::ArrayRef.36"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::ArrayRef.36", %"class.llvm::ArrayRef.36"* %3, i32 0, i32 0
  %5 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %4, align 8
  %6 = getelementptr inbounds %"class.llvm::ArrayRef.36", %"class.llvm::ArrayRef.36"* %3, i32 0, i32 1
  %7 = load i64, i64* %6, align 8
  %8 = getelementptr inbounds %"class.llvm::Value"*, %"class.llvm::Value"** %5, i64 %7
  ret %"class.llvm::Value"** %8
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr i32 @_ZNK4llvm11PointerType15getAddressSpaceEv(%"class.llvm::PointerType"*) #3 align 2 {
  %2 = alloca %"class.llvm::PointerType"*, align 8
  store %"class.llvm::PointerType"* %0, %"class.llvm::PointerType"** %2, align 8
  %3 = load %"class.llvm::PointerType"*, %"class.llvm::PointerType"** %2, align 8
  %4 = bitcast %"class.llvm::PointerType"* %3 to %"class.llvm::Type"*
  %5 = call i32 @_ZNK4llvm4Type15getSubclassDataEv(%"class.llvm::Type"* %4)
  ret i32 %5
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i32 @_ZNK4llvm4Type15getSubclassDataEv(%"class.llvm::Type"*) #4 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::Type", %"class.llvm::Type"* %3, i32 0, i32 1
  %5 = load i32, i32* %4, align 8
  %6 = lshr i32 %5, 8
  ret i32 %6
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr %"class.llvm::VectorType"* @_ZN4llvm4castINS_10VectorTypeEKNS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES7_(%"class.llvm::Type"*) #3 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = call zeroext i1 @_ZN4llvm3isaINS_10VectorTypeEPKNS_4TypeEEEbRKT0_(%"class.llvm::Type"** dereferenceable(8) %2)
  br i1 %3, label %4, label %5

; <label>:4:                                      ; preds = %1
  br label %5

; <label>:5:                                      ; preds = %4, %1
  %6 = phi i1 [ false, %1 ], [ true, %4 ]
  %7 = xor i1 %6, true
  br i1 %7, label %8, label %10

; <label>:8:                                      ; preds = %5
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm4castINS_10StructTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 236, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.9, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %11

; <label>:10:                                     ; preds = %5
  br label %11

; <label>:11:                                     ; preds = %10, %9
  %12 = call %"class.llvm::VectorType"* @_ZN4llvm16cast_convert_valINS_10VectorTypeEPKNS_4TypeES4_E4doitERKS4_(%"class.llvm::Type"** dereferenceable(8) %2)
  ret %"class.llvm::VectorType"* %12
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNK4llvm14SequentialType14getNumElementsEv(%"class.llvm::SequentialType"*) #4 align 2 {
  %2 = alloca %"class.llvm::SequentialType"*, align 8
  store %"class.llvm::SequentialType"* %0, %"class.llvm::SequentialType"** %2, align 8
  %3 = load %"class.llvm::SequentialType"*, %"class.llvm::SequentialType"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::SequentialType", %"class.llvm::SequentialType"* %3, i32 0, i32 2
  %5 = load i64, i64* %4, align 8
  ret i64 %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm3isaINS_10VectorTypeEPKNS_4TypeEEEbRKT0_(%"class.llvm::Type"** dereferenceable(8)) #3 {
  %2 = alloca %"class.llvm::Type"**, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %3 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_10VectorTypeEKPKNS_4TypeES4_E4doitERS5_(%"class.llvm::Type"** dereferenceable(8) %3)
  ret i1 %4
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::VectorType"* @_ZN4llvm16cast_convert_valINS_10VectorTypeEPKNS_4TypeES4_E4doitERKS4_(%"class.llvm::Type"** dereferenceable(8)) #4 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  %3 = alloca %"class.llvm::VectorType"*, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %4 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %5 = load %"class.llvm::Type"*, %"class.llvm::Type"** %4, align 8
  %6 = bitcast %"class.llvm::Type"* %5 to %"class.llvm::VectorType"*
  store %"class.llvm::VectorType"* %6, %"class.llvm::VectorType"** %3, align 8
  %7 = load %"class.llvm::VectorType"*, %"class.llvm::VectorType"** %3, align 8
  ret %"class.llvm::VectorType"* %7
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_10VectorTypeEKPKNS_4TypeES4_E4doitERS5_(%"class.llvm::Type"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  %3 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %4 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %5 = call %"class.llvm::Type"* @_ZN4llvm13simplify_typeIKPKNS_4TypeEE18getSimplifiedValueERS4_(%"class.llvm::Type"** dereferenceable(8) %4)
  store %"class.llvm::Type"* %5, %"class.llvm::Type"** %3, align 8
  %6 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_10VectorTypeEPKNS_4TypeES4_E4doitERKS4_(%"class.llvm::Type"** dereferenceable(8) %3)
  ret i1 %6
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_10VectorTypeEPKNS_4TypeES4_E4doitERKS4_(%"class.llvm::Type"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %3 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %4 = load %"class.llvm::Type"*, %"class.llvm::Type"** %3, align 8
  %5 = call zeroext i1 @_ZN4llvm11isa_impl_clINS_10VectorTypeEPKNS_4TypeEE4doitES4_(%"class.llvm::Type"* %4)
  ret i1 %5
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Type"* @_ZN4llvm13simplify_typeIKPKNS_4TypeEE18getSimplifiedValueERS4_(%"class.llvm::Type"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %3 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %4 = call dereferenceable(8) %"class.llvm::Type"** @_ZN4llvm13simplify_typeIPKNS_4TypeEE18getSimplifiedValueERS3_(%"class.llvm::Type"** dereferenceable(8) %3)
  %5 = load %"class.llvm::Type"*, %"class.llvm::Type"** %4, align 8
  ret %"class.llvm::Type"* %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm11isa_impl_clINS_10VectorTypeEPKNS_4TypeEE4doitES4_(%"class.llvm::Type"*) #3 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = icmp ne %"class.llvm::Type"* %3, null
  br i1 %4, label %5, label %6

; <label>:5:                                      ; preds = %1
  br label %6

; <label>:6:                                      ; preds = %5, %1
  %7 = phi i1 [ false, %1 ], [ true, %5 ]
  %8 = xor i1 %7, true
  br i1 %8, label %9, label %11

; <label>:9:                                      ; preds = %6
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 95, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.7, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %12

; <label>:11:                                     ; preds = %6
  br label %12

; <label>:12:                                     ; preds = %11, %10
  %13 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %14 = call zeroext i1 @_ZN4llvm8isa_implINS_10VectorTypeENS_4TypeEvE4doitERKS2_(%"class.llvm::Type"* dereferenceable(24) %13)
  ret i1 %14
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm8isa_implINS_10VectorTypeENS_4TypeEvE4doitERKS2_(%"class.llvm::Type"* dereferenceable(24)) #3 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm10VectorType7classofEPKNS_4TypeE(%"class.llvm::Type"* %3)
  ret i1 %4
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm10VectorType7classofEPKNS_4TypeE(%"class.llvm::Type"*) #7 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call i32 @_ZNK4llvm4Type9getTypeIDEv(%"class.llvm::Type"* %3)
  %5 = icmp eq i32 %4, 16
  ret i1 %5
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr dereferenceable(8) %"class.llvm::Type"** @_ZN4llvm13simplify_typeIPKNS_4TypeEE18getSimplifiedValueERS3_(%"class.llvm::Type"** dereferenceable(8)) #4 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %3 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  ret %"class.llvm::Type"** %3
}

; Function Attrs: noreturn
declare void @_ZN4llvm25llvm_unreachable_internalEPKcS1_j(i8*, i8*, i32) #5

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm8ArrayRefIPNS_5ValueEEC2ERKS2_(%"class.llvm::ArrayRef.36"*, %"class.llvm::Value"** dereferenceable(8)) unnamed_addr #4 align 2 {
  %3 = alloca %"class.llvm::ArrayRef.36"*, align 8
  %4 = alloca %"class.llvm::Value"**, align 8
  store %"class.llvm::ArrayRef.36"* %0, %"class.llvm::ArrayRef.36"** %3, align 8
  store %"class.llvm::Value"** %1, %"class.llvm::Value"*** %4, align 8
  %5 = load %"class.llvm::ArrayRef.36"*, %"class.llvm::ArrayRef.36"** %3, align 8
  %6 = getelementptr inbounds %"class.llvm::ArrayRef.36", %"class.llvm::ArrayRef.36"* %5, i32 0, i32 0
  %7 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %4, align 8
  store %"class.llvm::Value"** %7, %"class.llvm::Value"*** %6, align 8
  %8 = getelementptr inbounds %"class.llvm::ArrayRef.36", %"class.llvm::ArrayRef.36"* %5, i32 0, i32 1
  store i64 1, i64* %8, align 8
  ret void
}

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"*, i8*, i64) #1

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8*) #3 align 2 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = load i8*, i8** %2, align 8
  %4 = call i64 @strlen(i8* %3)
  ret i64 %4
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #6

declare i64 @strlen(i8*) #1

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE21__construct_node_hashIRKNS_21piecewise_construct_tEJNS_5tupleIJOS7_EEENSQ_IJEEEEEENS_10unique_ptrINS_11__hash_nodeISB_PvEENS_22__hash_node_destructorINS5_ISX_EEEEEEmOT_DpOT0_(%"class.std::__1::unique_ptr.94"* noalias sret, %"class.std::__1::__hash_table"*, i64, %"struct.std::__1::piecewise_construct_t"* dereferenceable(1), %"class.std::__1::tuple"* dereferenceable(8), %"class.std::__1::tuple.91"* dereferenceable(1)) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %7 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %8 = alloca %"class.std::__1::tuple"*, align 8
  %9 = alloca %"class.std::__1::tuple.91"*, align 8
  %10 = alloca %"class.std::__1::tuple"*, align 8
  %11 = alloca %"class.std::__1::tuple.91"*, align 8
  %12 = alloca %"class.std::__1::tuple"*, align 8
  %13 = alloca %"class.std::__1::tuple.91"*, align 8
  %14 = alloca %"class.std::__1::tuple"*, align 8
  %15 = alloca %"class.std::__1::tuple.91"*, align 8
  %16 = alloca %"class.std::__1::basic_string"*, align 8
  %17 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %18 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %19 = alloca %"class.std::__1::basic_string"*, align 8
  %20 = alloca [3 x i64]*, align 8
  %21 = alloca i32, align 4
  %22 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %23 = alloca %"class.std::__1::basic_string"*, align 8
  %24 = alloca %"class.std::__1::basic_string"*, align 8
  %25 = alloca %"class.std::__1::basic_string"*, align 8
  %26 = alloca %"class.std::__1::basic_string"*, align 8
  %27 = alloca %"class.std::__1::__tuple_leaf"*, align 8
  %28 = alloca %"class.std::__1::tuple"*, align 8
  %29 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %30 = alloca %"struct.std::__1::__tuple_indices", align 1
  %31 = alloca %"struct.std::__1::__tuple_indices.98", align 1
  %32 = alloca %"struct.std::__1::pair.92"*, align 8
  %33 = alloca %"class.std::__1::tuple"*, align 8
  %34 = alloca %"class.std::__1::tuple.91"*, align 8
  %35 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %36 = alloca %"struct.std::__1::__tuple_indices", align 1
  %37 = alloca %"struct.std::__1::__tuple_indices.98", align 1
  %38 = alloca %"struct.std::__1::pair.92"*, align 8
  %39 = alloca %"class.std::__1::tuple"*, align 8
  %40 = alloca %"class.std::__1::tuple.91"*, align 8
  %41 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %42 = alloca %"class.std::__1::tuple", align 8
  %43 = alloca %"class.std::__1::tuple.91", align 1
  %44 = alloca %"struct.std::__1::pair.92"*, align 8
  %45 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %46 = alloca %"struct.std::__1::__tuple_indices", align 1
  %47 = alloca %"struct.std::__1::__tuple_indices.98", align 1
  %48 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %49 = alloca %"class.std::__1::allocator.46"*, align 8
  %50 = alloca %"struct.std::__1::pair.92"*, align 8
  %51 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %52 = alloca %"class.std::__1::tuple"*, align 8
  %53 = alloca %"class.std::__1::tuple.91"*, align 8
  %54 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %55 = alloca %"class.std::__1::tuple", align 8
  %56 = alloca %"class.std::__1::tuple.91", align 1
  %57 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %58 = alloca %"struct.std::__1::integral_constant", align 1
  %59 = alloca %"class.std::__1::allocator.46"*, align 8
  %60 = alloca %"struct.std::__1::pair.92"*, align 8
  %61 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %62 = alloca %"class.std::__1::tuple"*, align 8
  %63 = alloca %"class.std::__1::tuple.91"*, align 8
  %64 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %65 = alloca %"class.std::__1::allocator.46"*, align 8
  %66 = alloca %"struct.std::__1::pair.92"*, align 8
  %67 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %68 = alloca %"class.std::__1::tuple"*, align 8
  %69 = alloca %"class.std::__1::tuple.91"*, align 8
  %70 = alloca %"struct.std::__1::integral_constant", align 1
  %71 = alloca %"struct.std::__1::__has_construct", align 1
  %72 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %73 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %74 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %75 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %76 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %77 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %78 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %79 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %80 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %81 = alloca %"struct.std::__1::pair.92"*, align 8
  %82 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %83 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %84 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %85 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %86 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %87 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %88 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %89 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %90 = alloca i8*, align 8
  %91 = alloca %"class.std::__1::allocator.46"*, align 8
  %92 = alloca %"struct.std::__1::__hash_node"*, align 8
  %93 = alloca i64, align 8
  %94 = alloca %"class.std::__1::allocator.46"*, align 8
  %95 = alloca %"struct.std::__1::__hash_node"*, align 8
  %96 = alloca i64, align 8
  %97 = alloca %"class.std::__1::allocator.46"*, align 8
  %98 = alloca %"struct.std::__1::pair.92"*, align 8
  %99 = alloca %"class.std::__1::allocator.46"*, align 8
  %100 = alloca %"struct.std::__1::pair.92"*, align 8
  %101 = alloca %"struct.std::__1::integral_constant.99", align 1
  %102 = alloca %"struct.std::__1::__has_destroy", align 1
  %103 = alloca %"struct.std::__1::pair.92"*, align 8
  %104 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %105 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %106 = alloca %"struct.std::__1::__hash_node"*, align 8
  %107 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %108 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %109 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %110 = alloca %"struct.std::__1::__hash_node"*, align 8
  %111 = alloca %"struct.std::__1::__hash_node"*, align 8
  %112 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %113 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %114 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %115 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %116 = alloca %"struct.std::__1::__hash_node"**, align 8
  %117 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %118 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %119 = alloca %"struct.std::__1::__hash_node"*, align 8
  %120 = alloca %"struct.std::__1::__hash_node"**, align 8
  %121 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %122 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %123 = alloca %"struct.std::__1::__hash_node"*, align 8
  %124 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %125 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %126 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %127 = alloca %"struct.std::__1::__hash_node"*, align 8
  %128 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %129 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %130 = alloca %"struct.std::__1::__hash_node"*, align 8
  %131 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %132 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %133 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %134 = alloca %"struct.std::__1::__hash_node"*, align 8
  %135 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %136 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %137 = alloca %"class.std::__1::allocator.46"*, align 8
  %138 = alloca i8, align 1
  %139 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %140 = alloca %"class.std::__1::allocator.46"*, align 8
  %141 = alloca i8, align 1
  %142 = alloca i64, align 8
  %143 = alloca %"class.std::__1::allocator.46"*, align 8
  %144 = alloca i64, align 8
  %145 = alloca i8*, align 8
  %146 = alloca %"class.std::__1::allocator.46"*, align 8
  %147 = alloca i64, align 8
  %148 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %149 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %150 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %151 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %152 = alloca i8*, align 8
  %153 = alloca %"class.std::__1::allocator.46"*, align 8
  %154 = alloca %"struct.std::__1::__hash_node"*, align 8
  %155 = alloca i64, align 8
  %156 = alloca %"class.std::__1::allocator.46"*, align 8
  %157 = alloca %"struct.std::__1::__hash_node"*, align 8
  %158 = alloca i64, align 8
  %159 = alloca %"class.std::__1::allocator.46"*, align 8
  %160 = alloca %"struct.std::__1::pair.92"*, align 8
  %161 = alloca %"class.std::__1::allocator.46"*, align 8
  %162 = alloca %"struct.std::__1::pair.92"*, align 8
  %163 = alloca %"struct.std::__1::integral_constant.99", align 1
  %164 = alloca %"struct.std::__1::__has_destroy", align 1
  %165 = alloca %"struct.std::__1::pair.92"*, align 8
  %166 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %167 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %168 = alloca %"struct.std::__1::__hash_node"*, align 8
  %169 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.96"*, align 8
  %170 = alloca %"class.std::__1::__compressed_pair.95"*, align 8
  %171 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %172 = alloca %"struct.std::__1::__hash_node"*, align 8
  %173 = alloca %"struct.std::__1::__hash_node"*, align 8
  %174 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %175 = alloca %"class.std::__1::unique_ptr.94"*, align 8
  %176 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.45"*, align 8
  %177 = alloca %"class.std::__1::__compressed_pair.44"*, align 8
  %178 = alloca %"class.std::__1::__hash_table"*, align 8
  %179 = alloca %"class.std::__1::__hash_table"*, align 8
  %180 = alloca i64, align 8
  %181 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %182 = alloca %"class.std::__1::tuple"*, align 8
  %183 = alloca %"class.std::__1::tuple.91"*, align 8
  %184 = alloca %"class.std::__1::allocator.46"*, align 8
  %185 = alloca i1, align 1
  %186 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %187 = alloca i8*
  %188 = alloca i32
  store %"class.std::__1::__hash_table"* %1, %"class.std::__1::__hash_table"** %179, align 8
  store i64 %2, i64* %180, align 8
  store %"struct.std::__1::piecewise_construct_t"* %3, %"struct.std::__1::piecewise_construct_t"** %181, align 8
  store %"class.std::__1::tuple"* %4, %"class.std::__1::tuple"** %182, align 8
  store %"class.std::__1::tuple.91"* %5, %"class.std::__1::tuple.91"** %183, align 8
  %189 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %179, align 8
  store %"class.std::__1::__hash_table"* %189, %"class.std::__1::__hash_table"** %178, align 8
  %190 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %178, align 8
  %191 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %190, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.44"* %191, %"class.std::__1::__compressed_pair.44"** %177, align 8
  %192 = load %"class.std::__1::__compressed_pair.44"*, %"class.std::__1::__compressed_pair.44"** %177, align 8
  %193 = bitcast %"class.std::__1::__compressed_pair.44"* %192 to %"class.std::__1::__libcpp_compressed_pair_imp.45"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.45"* %193, %"class.std::__1::__libcpp_compressed_pair_imp.45"** %176, align 8
  %194 = load %"class.std::__1::__libcpp_compressed_pair_imp.45"*, %"class.std::__1::__libcpp_compressed_pair_imp.45"** %176, align 8
  %195 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.45"* %194 to %"class.std::__1::allocator.46"*
  store %"class.std::__1::allocator.46"* %195, %"class.std::__1::allocator.46"** %184, align 8
  store i1 false, i1* %185, align 1
  %196 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %184, align 8
  store %"class.std::__1::allocator.46"* %196, %"class.std::__1::allocator.46"** %146, align 8
  store i64 1, i64* %147, align 8
  %197 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %146, align 8
  %198 = load i64, i64* %147, align 8
  store %"class.std::__1::allocator.46"* %197, %"class.std::__1::allocator.46"** %143, align 8
  store i64 %198, i64* %144, align 8
  store i8* null, i8** %145, align 8
  %199 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %143, align 8
  %200 = load i64, i64* %144, align 8
  %201 = mul i64 %200, 48
  store i64 %201, i64* %142, align 8
  %202 = load i64, i64* %142, align 8
  %203 = call i8* @_Znwm(i64 %202) #19
  %204 = bitcast i8* %203 to %"struct.std::__1::__hash_node"*
  %205 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %184, align 8
  store %"class.std::__1::__hash_node_destructor"* %186, %"class.std::__1::__hash_node_destructor"** %139, align 8
  store %"class.std::__1::allocator.46"* %205, %"class.std::__1::allocator.46"** %140, align 8
  store i8 0, i8* %141, align 1
  %206 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %139, align 8
  %207 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %140, align 8
  %208 = load i8, i8* %141, align 1
  %209 = trunc i8 %208 to i1
  store %"class.std::__1::__hash_node_destructor"* %206, %"class.std::__1::__hash_node_destructor"** %136, align 8
  store %"class.std::__1::allocator.46"* %207, %"class.std::__1::allocator.46"** %137, align 8
  %210 = zext i1 %209 to i8
  store i8 %210, i8* %138, align 1
  %211 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %136, align 8
  %212 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %211, i32 0, i32 0
  %213 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %137, align 8
  store %"class.std::__1::allocator.46"* %213, %"class.std::__1::allocator.46"** %212, align 8
  %214 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %211, i32 0, i32 1
  %215 = load i8, i8* %138, align 1
  %216 = trunc i8 %215 to i1
  %217 = zext i1 %216 to i8
  store i8 %217, i8* %214, align 8
  store %"class.std::__1::unique_ptr.94"* %0, %"class.std::__1::unique_ptr.94"** %133, align 8
  store %"struct.std::__1::__hash_node"* %204, %"struct.std::__1::__hash_node"** %134, align 8
  store %"class.std::__1::__hash_node_destructor"* %186, %"class.std::__1::__hash_node_destructor"** %135, align 8
  %218 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %133, align 8
  %219 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %134, align 8
  %220 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %135, align 8
  store %"class.std::__1::unique_ptr.94"* %218, %"class.std::__1::unique_ptr.94"** %129, align 8
  store %"struct.std::__1::__hash_node"* %219, %"struct.std::__1::__hash_node"** %130, align 8
  store %"class.std::__1::__hash_node_destructor"* %220, %"class.std::__1::__hash_node_destructor"** %131, align 8
  %221 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %129, align 8
  %222 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %221, i32 0, i32 0
  %223 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %130, align 8
  %224 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %131, align 8
  store %"class.std::__1::__hash_node_destructor"* %224, %"class.std::__1::__hash_node_destructor"** %128, align 8
  %225 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %128, align 8
  %226 = bitcast %"class.std::__1::__hash_node_destructor"* %132 to i8*
  %227 = bitcast %"class.std::__1::__hash_node_destructor"* %225 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %226, i8* %227, i64 16, i32 8, i1 false) #14
  %228 = bitcast %"class.std::__1::__hash_node_destructor"* %132 to { %"class.std::__1::allocator.46"*, i8 }*
  %229 = getelementptr inbounds { %"class.std::__1::allocator.46"*, i8 }, { %"class.std::__1::allocator.46"*, i8 }* %228, i32 0, i32 0
  %230 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %229, align 8
  %231 = getelementptr inbounds { %"class.std::__1::allocator.46"*, i8 }, { %"class.std::__1::allocator.46"*, i8 }* %228, i32 0, i32 1
  %232 = load i8, i8* %231, align 8
  %233 = bitcast %"class.std::__1::__hash_node_destructor"* %125 to { %"class.std::__1::allocator.46"*, i8 }*
  %234 = getelementptr inbounds { %"class.std::__1::allocator.46"*, i8 }, { %"class.std::__1::allocator.46"*, i8 }* %233, i32 0, i32 0
  store %"class.std::__1::allocator.46"* %230, %"class.std::__1::allocator.46"** %234, align 8
  %235 = getelementptr inbounds { %"class.std::__1::allocator.46"*, i8 }, { %"class.std::__1::allocator.46"*, i8 }* %233, i32 0, i32 1
  store i8 %232, i8* %235, align 8
  store %"class.std::__1::__compressed_pair.95"* %222, %"class.std::__1::__compressed_pair.95"** %126, align 8
  store %"struct.std::__1::__hash_node"* %223, %"struct.std::__1::__hash_node"** %127, align 8
  %236 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %126, align 8
  %237 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %127, align 8
  %238 = bitcast %"class.std::__1::__hash_node_destructor"* %125 to { %"class.std::__1::allocator.46"*, i8 }*
  %239 = getelementptr inbounds { %"class.std::__1::allocator.46"*, i8 }, { %"class.std::__1::allocator.46"*, i8 }* %238, i32 0, i32 0
  %240 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %239, align 8
  %241 = getelementptr inbounds { %"class.std::__1::allocator.46"*, i8 }, { %"class.std::__1::allocator.46"*, i8 }* %238, i32 0, i32 1
  %242 = load i8, i8* %241, align 8
  %243 = bitcast %"class.std::__1::__hash_node_destructor"* %121 to { %"class.std::__1::allocator.46"*, i8 }*
  %244 = getelementptr inbounds { %"class.std::__1::allocator.46"*, i8 }, { %"class.std::__1::allocator.46"*, i8 }* %243, i32 0, i32 0
  store %"class.std::__1::allocator.46"* %240, %"class.std::__1::allocator.46"** %244, align 8
  %245 = getelementptr inbounds { %"class.std::__1::allocator.46"*, i8 }, { %"class.std::__1::allocator.46"*, i8 }* %243, i32 0, i32 1
  store i8 %242, i8* %245, align 8
  store %"class.std::__1::__compressed_pair.95"* %236, %"class.std::__1::__compressed_pair.95"** %122, align 8
  store %"struct.std::__1::__hash_node"* %237, %"struct.std::__1::__hash_node"** %123, align 8
  %246 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %122, align 8
  %247 = bitcast %"class.std::__1::__compressed_pair.95"* %246 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"struct.std::__1::__hash_node"** %123, %"struct.std::__1::__hash_node"*** %120, align 8
  %248 = load %"struct.std::__1::__hash_node"**, %"struct.std::__1::__hash_node"*** %120, align 8
  %249 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %248, align 8
  store %"class.std::__1::__hash_node_destructor"* %121, %"class.std::__1::__hash_node_destructor"** %114, align 8
  %250 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %114, align 8
  %251 = bitcast %"class.std::__1::__hash_node_destructor"* %124 to i8*
  %252 = bitcast %"class.std::__1::__hash_node_destructor"* %250 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %251, i8* %252, i64 16, i32 8, i1 false) #14
  %253 = bitcast %"class.std::__1::__hash_node_destructor"* %124 to { %"class.std::__1::allocator.46"*, i8 }*
  %254 = getelementptr inbounds { %"class.std::__1::allocator.46"*, i8 }, { %"class.std::__1::allocator.46"*, i8 }* %253, i32 0, i32 0
  %255 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %254, align 8
  %256 = getelementptr inbounds { %"class.std::__1::allocator.46"*, i8 }, { %"class.std::__1::allocator.46"*, i8 }* %253, i32 0, i32 1
  %257 = load i8, i8* %256, align 8
  %258 = bitcast %"class.std::__1::__hash_node_destructor"* %117 to { %"class.std::__1::allocator.46"*, i8 }*
  %259 = getelementptr inbounds { %"class.std::__1::allocator.46"*, i8 }, { %"class.std::__1::allocator.46"*, i8 }* %258, i32 0, i32 0
  store %"class.std::__1::allocator.46"* %255, %"class.std::__1::allocator.46"** %259, align 8
  %260 = getelementptr inbounds { %"class.std::__1::allocator.46"*, i8 }, { %"class.std::__1::allocator.46"*, i8 }* %258, i32 0, i32 1
  store i8 %257, i8* %260, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %247, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %118, align 8
  store %"struct.std::__1::__hash_node"* %249, %"struct.std::__1::__hash_node"** %119, align 8
  %261 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %118, align 8
  %262 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %261, i32 0, i32 0
  store %"struct.std::__1::__hash_node"** %119, %"struct.std::__1::__hash_node"*** %116, align 8
  %263 = load %"struct.std::__1::__hash_node"**, %"struct.std::__1::__hash_node"*** %116, align 8
  %264 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %263, align 8
  store %"struct.std::__1::__hash_node"* %264, %"struct.std::__1::__hash_node"** %262, align 8
  %265 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %261, i32 0, i32 1
  store %"class.std::__1::__hash_node_destructor"* %117, %"class.std::__1::__hash_node_destructor"** %115, align 8
  %266 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %115, align 8
  %267 = bitcast %"class.std::__1::__hash_node_destructor"* %265 to i8*
  %268 = bitcast %"class.std::__1::__hash_node_destructor"* %266 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %267, i8* %268, i64 16, i32 8, i1 false) #14
  %269 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %184, align 8
  store %"class.std::__1::unique_ptr.94"* %0, %"class.std::__1::unique_ptr.94"** %85, align 8
  %270 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %85, align 8
  %271 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %270, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %271, %"class.std::__1::__compressed_pair.95"** %84, align 8
  %272 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %84, align 8
  %273 = bitcast %"class.std::__1::__compressed_pair.95"* %272 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %273, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %83, align 8
  %274 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %83, align 8
  %275 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %274, i32 0, i32 0
  %276 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %275, align 8
  %277 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %276, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %277, %"union.std::__1::__hash_value_type"** %82, align 8
  %278 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %82, align 8
  %279 = bitcast %"union.std::__1::__hash_value_type"* %278 to %"struct.std::__1::pair.92"*
  store %"struct.std::__1::pair.92"* %279, %"struct.std::__1::pair.92"** %81, align 8
  %280 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %81, align 8
  %281 = bitcast %"struct.std::__1::pair.92"* %280 to i8*
  br label %282

; <label>:282:                                    ; preds = %6
  %283 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %181, align 8
  store %"struct.std::__1::piecewise_construct_t"* %283, %"struct.std::__1::piecewise_construct_t"** %7, align 8
  %284 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %7, align 8
  %285 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %182, align 8
  store %"class.std::__1::tuple"* %285, %"class.std::__1::tuple"** %8, align 8
  %286 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %8, align 8
  %287 = load %"class.std::__1::tuple.91"*, %"class.std::__1::tuple.91"** %183, align 8
  store %"class.std::__1::tuple.91"* %287, %"class.std::__1::tuple.91"** %9, align 8
  %288 = load %"class.std::__1::tuple.91"*, %"class.std::__1::tuple.91"** %9, align 8
  store %"class.std::__1::allocator.46"* %269, %"class.std::__1::allocator.46"** %65, align 8
  store %"struct.std::__1::pair.92"* %280, %"struct.std::__1::pair.92"** %66, align 8
  store %"struct.std::__1::piecewise_construct_t"* %284, %"struct.std::__1::piecewise_construct_t"** %67, align 8
  store %"class.std::__1::tuple"* %286, %"class.std::__1::tuple"** %68, align 8
  store %"class.std::__1::tuple.91"* %288, %"class.std::__1::tuple.91"** %69, align 8
  %289 = bitcast %"struct.std::__1::__has_construct"* %71 to %"struct.std::__1::integral_constant"*
  %290 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %65, align 8
  %291 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %66, align 8
  %292 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %67, align 8
  store %"struct.std::__1::piecewise_construct_t"* %292, %"struct.std::__1::piecewise_construct_t"** %64, align 8
  %293 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %64, align 8
  %294 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %68, align 8
  store %"class.std::__1::tuple"* %294, %"class.std::__1::tuple"** %10, align 8
  %295 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %10, align 8
  %296 = load %"class.std::__1::tuple.91"*, %"class.std::__1::tuple.91"** %69, align 8
  store %"class.std::__1::tuple.91"* %296, %"class.std::__1::tuple.91"** %11, align 8
  %297 = load %"class.std::__1::tuple.91"*, %"class.std::__1::tuple.91"** %11, align 8
  store %"class.std::__1::allocator.46"* %290, %"class.std::__1::allocator.46"** %59, align 8
  store %"struct.std::__1::pair.92"* %291, %"struct.std::__1::pair.92"** %60, align 8
  store %"struct.std::__1::piecewise_construct_t"* %293, %"struct.std::__1::piecewise_construct_t"** %61, align 8
  store %"class.std::__1::tuple"* %295, %"class.std::__1::tuple"** %62, align 8
  store %"class.std::__1::tuple.91"* %297, %"class.std::__1::tuple.91"** %63, align 8
  %298 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %59, align 8
  %299 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %60, align 8
  %300 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %61, align 8
  store %"struct.std::__1::piecewise_construct_t"* %300, %"struct.std::__1::piecewise_construct_t"** %57, align 8
  %301 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %57, align 8
  %302 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %62, align 8
  store %"class.std::__1::tuple"* %302, %"class.std::__1::tuple"** %12, align 8
  %303 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %12, align 8
  %304 = load %"class.std::__1::tuple.91"*, %"class.std::__1::tuple.91"** %63, align 8
  store %"class.std::__1::tuple.91"* %304, %"class.std::__1::tuple.91"** %13, align 8
  %305 = load %"class.std::__1::tuple.91"*, %"class.std::__1::tuple.91"** %13, align 8
  store %"class.std::__1::allocator.46"* %298, %"class.std::__1::allocator.46"** %49, align 8
  store %"struct.std::__1::pair.92"* %299, %"struct.std::__1::pair.92"** %50, align 8
  store %"struct.std::__1::piecewise_construct_t"* %301, %"struct.std::__1::piecewise_construct_t"** %51, align 8
  store %"class.std::__1::tuple"* %303, %"class.std::__1::tuple"** %52, align 8
  store %"class.std::__1::tuple.91"* %305, %"class.std::__1::tuple.91"** %53, align 8
  %306 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %49, align 8
  %307 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %50, align 8
  %308 = bitcast %"struct.std::__1::pair.92"* %307 to i8*
  %309 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %51, align 8
  store %"struct.std::__1::piecewise_construct_t"* %309, %"struct.std::__1::piecewise_construct_t"** %48, align 8
  %310 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %48, align 8
  %311 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %52, align 8
  store %"class.std::__1::tuple"* %311, %"class.std::__1::tuple"** %14, align 8
  %312 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %14, align 8
  %313 = bitcast %"class.std::__1::tuple"* %55 to i8*
  %314 = bitcast %"class.std::__1::tuple"* %312 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %313, i8* %314, i64 8, i32 8, i1 false)
  %315 = load %"class.std::__1::tuple.91"*, %"class.std::__1::tuple.91"** %53, align 8
  store %"class.std::__1::tuple.91"* %315, %"class.std::__1::tuple.91"** %15, align 8
  %316 = load %"class.std::__1::tuple.91"*, %"class.std::__1::tuple.91"** %15, align 8
  %317 = getelementptr inbounds %"class.std::__1::tuple", %"class.std::__1::tuple"* %55, i32 0, i32 0
  %318 = getelementptr inbounds %"struct.std::__1::__tuple_impl", %"struct.std::__1::__tuple_impl"* %317, i32 0, i32 0
  %319 = getelementptr inbounds %"class.std::__1::__tuple_leaf", %"class.std::__1::__tuple_leaf"* %318, i32 0, i32 0
  %320 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %319, align 8
  %321 = getelementptr inbounds %"class.std::__1::tuple", %"class.std::__1::tuple"* %42, i32 0, i32 0
  %322 = getelementptr inbounds %"struct.std::__1::__tuple_impl", %"struct.std::__1::__tuple_impl"* %321, i32 0, i32 0
  %323 = getelementptr inbounds %"class.std::__1::__tuple_leaf", %"class.std::__1::__tuple_leaf"* %322, i32 0, i32 0
  store %"class.std::__1::basic_string"* %320, %"class.std::__1::basic_string"** %323, align 8
  store %"struct.std::__1::pair.92"* %307, %"struct.std::__1::pair.92"** %44, align 8
  %324 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %44, align 8
  store %"struct.std::__1::pair.92"* %324, %"struct.std::__1::pair.92"** %38, align 8
  store %"class.std::__1::tuple"* %42, %"class.std::__1::tuple"** %39, align 8
  store %"class.std::__1::tuple.91"* %43, %"class.std::__1::tuple.91"** %40, align 8
  %325 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %38, align 8
  %326 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %39, align 8
  %327 = load %"class.std::__1::tuple.91"*, %"class.std::__1::tuple.91"** %40, align 8
  store %"struct.std::__1::pair.92"* %325, %"struct.std::__1::pair.92"** %32, align 8
  store %"class.std::__1::tuple"* %326, %"class.std::__1::tuple"** %33, align 8
  store %"class.std::__1::tuple.91"* %327, %"class.std::__1::tuple.91"** %34, align 8
  %328 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %32, align 8
  %329 = getelementptr inbounds %"struct.std::__1::pair.92", %"struct.std::__1::pair.92"* %328, i32 0, i32 0
  %330 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %33, align 8
  store %"class.std::__1::tuple"* %330, %"class.std::__1::tuple"** %28, align 8
  %331 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %28, align 8
  %332 = getelementptr inbounds %"class.std::__1::tuple", %"class.std::__1::tuple"* %331, i32 0, i32 0
  %333 = bitcast %"struct.std::__1::__tuple_impl"* %332 to %"class.std::__1::__tuple_leaf"*
  store %"class.std::__1::__tuple_leaf"* %333, %"class.std::__1::__tuple_leaf"** %27, align 8
  %334 = load %"class.std::__1::__tuple_leaf"*, %"class.std::__1::__tuple_leaf"** %27, align 8
  %335 = getelementptr inbounds %"class.std::__1::__tuple_leaf", %"class.std::__1::__tuple_leaf"* %334, i32 0, i32 0
  %336 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %335, align 8
  store %"class.std::__1::basic_string"* %336, %"class.std::__1::basic_string"** %16, align 8
  %337 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %16, align 8
  store %"class.std::__1::basic_string"* %329, %"class.std::__1::basic_string"** %25, align 8
  store %"class.std::__1::basic_string"* %337, %"class.std::__1::basic_string"** %26, align 8
  %338 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %25, align 8
  %339 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %26, align 8
  store %"class.std::__1::basic_string"* %338, %"class.std::__1::basic_string"** %23, align 8
  store %"class.std::__1::basic_string"* %339, %"class.std::__1::basic_string"** %24, align 8
  %340 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8
  %341 = bitcast %"class.std::__1::basic_string"* %340 to %"class.std::__1::__basic_string_common"*
  %342 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %340, i32 0, i32 0
  %343 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8
  %344 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %343, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %344, %"class.std::__1::__compressed_pair.18"** %22, align 8
  %345 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %22, align 8
  %346 = bitcast %"class.std::__1::__compressed_pair.18"* %342 to i8*
  %347 = bitcast %"class.std::__1::__compressed_pair.18"* %345 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %346, i8* %347, i64 24, i32 8, i1 false) #14
  %348 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8
  store %"class.std::__1::basic_string"* %348, %"class.std::__1::basic_string"** %19, align 8
  %349 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %19, align 8
  %350 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %349, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %350, %"class.std::__1::__compressed_pair.18"** %18, align 8
  %351 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %18, align 8
  %352 = bitcast %"class.std::__1::__compressed_pair.18"* %351 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %352, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %17, align 8
  %353 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %17, align 8
  %354 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %353, i32 0, i32 0
  %355 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %354, i32 0, i32 0
  %356 = bitcast %union.anon* %355 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %357 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %356, i32 0, i32 0
  store [3 x i64]* %357, [3 x i64]** %20, align 8
  store i32 0, i32* %21, align 4
  br label %358

; <label>:358:                                    ; preds = %361, %282
  %359 = load i32, i32* %21, align 4
  %360 = icmp ult i32 %359, 3
  br i1 %360, label %361, label %368

; <label>:361:                                    ; preds = %358
  %362 = load i32, i32* %21, align 4
  %363 = zext i32 %362 to i64
  %364 = load [3 x i64]*, [3 x i64]** %20, align 8
  %365 = getelementptr inbounds [3 x i64], [3 x i64]* %364, i64 0, i64 %363
  store i64 0, i64* %365, align 8
  %366 = load i32, i32* %21, align 4
  %367 = add i32 %366, 1
  store i32 %367, i32* %21, align 4
  br label %358

; <label>:368:                                    ; preds = %358
  %369 = getelementptr inbounds %"struct.std::__1::pair.92", %"struct.std::__1::pair.92"* %328, i32 0, i32 1
  store %"class.llvm::Constant"* null, %"class.llvm::Constant"** %369, align 8
  br label %370

; <label>:370:                                    ; preds = %368
  store %"class.std::__1::unique_ptr.94"* %0, %"class.std::__1::unique_ptr.94"** %74, align 8
  %371 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %74, align 8
  %372 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %371, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %372, %"class.std::__1::__compressed_pair.95"** %73, align 8
  %373 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %73, align 8
  %374 = bitcast %"class.std::__1::__compressed_pair.95"* %373 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %374, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %72, align 8
  %375 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %72, align 8
  %376 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %375, i32 0, i32 1
  %377 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %376, i32 0, i32 1
  store i8 1, i8* %377, align 8
  %378 = load i64, i64* %180, align 8
  store %"class.std::__1::unique_ptr.94"* %0, %"class.std::__1::unique_ptr.94"** %77, align 8
  %379 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %77, align 8
  %380 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %379, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %380, %"class.std::__1::__compressed_pair.95"** %76, align 8
  %381 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %76, align 8
  %382 = bitcast %"class.std::__1::__compressed_pair.95"* %381 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %382, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %75, align 8
  %383 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %75, align 8
  %384 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %383, i32 0, i32 0
  %385 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %384, align 8
  %386 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %385, i32 0, i32 1
  store i64 %378, i64* %386, align 8
  store %"class.std::__1::unique_ptr.94"* %0, %"class.std::__1::unique_ptr.94"** %80, align 8
  %387 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %80, align 8
  %388 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %387, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %388, %"class.std::__1::__compressed_pair.95"** %79, align 8
  %389 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %79, align 8
  %390 = bitcast %"class.std::__1::__compressed_pair.95"* %389 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %390, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %78, align 8
  %391 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %78, align 8
  %392 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %391, i32 0, i32 0
  %393 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %392, align 8
  %394 = bitcast %"struct.std::__1::__hash_node"* %393 to %"struct.std::__1::__hash_node_base"*
  %395 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %394, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* null, %"struct.std::__1::__hash_node_base"** %395, align 8
  store i1 true, i1* %185, align 1
  %396 = load i1, i1* %185, align 1
  br i1 %396, label %516, label %458
                                                  ; No predecessors!
  %398 = landingpad { i8*, i32 }
          cleanup
  %399 = extractvalue { i8*, i32 } %398, 0
  store i8* %399, i8** %187, align 8
  %400 = extractvalue { i8*, i32 } %398, 1
  store i32 %400, i32* %188, align 4
  store %"class.std::__1::unique_ptr.94"* %0, %"class.std::__1::unique_ptr.94"** %113, align 8
  %401 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %113, align 8
  store %"class.std::__1::unique_ptr.94"* %401, %"class.std::__1::unique_ptr.94"** %112, align 8
  %402 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %112, align 8
  store %"class.std::__1::unique_ptr.94"* %402, %"class.std::__1::unique_ptr.94"** %109, align 8
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %110, align 8
  %403 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %109, align 8
  %404 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %403, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %404, %"class.std::__1::__compressed_pair.95"** %108, align 8
  %405 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %108, align 8
  %406 = bitcast %"class.std::__1::__compressed_pair.95"* %405 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %406, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %107, align 8
  %407 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %107, align 8
  %408 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %407, i32 0, i32 0
  %409 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %408, align 8
  store %"struct.std::__1::__hash_node"* %409, %"struct.std::__1::__hash_node"** %111, align 8
  %410 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %110, align 8
  %411 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %403, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %411, %"class.std::__1::__compressed_pair.95"** %89, align 8
  %412 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %89, align 8
  %413 = bitcast %"class.std::__1::__compressed_pair.95"* %412 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %413, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %88, align 8
  %414 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %88, align 8
  %415 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %414, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* %410, %"struct.std::__1::__hash_node"** %415, align 8
  %416 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %111, align 8
  %417 = icmp ne %"struct.std::__1::__hash_node"* %416, null
  br i1 %417, label %418, label %457

; <label>:418:                                    ; preds = %397
  %419 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %403, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %419, %"class.std::__1::__compressed_pair.95"** %87, align 8
  %420 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %87, align 8
  %421 = bitcast %"class.std::__1::__compressed_pair.95"* %420 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %421, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %86, align 8
  %422 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %86, align 8
  %423 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %422, i32 0, i32 1
  %424 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %111, align 8
  store %"class.std::__1::__hash_node_destructor"* %423, %"class.std::__1::__hash_node_destructor"** %105, align 8
  store %"struct.std::__1::__hash_node"* %424, %"struct.std::__1::__hash_node"** %106, align 8
  %425 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %105, align 8
  %426 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %425, i32 0, i32 1
  %427 = load i8, i8* %426, align 8
  %428 = trunc i8 %427 to i1
  br i1 %428, label %429, label %442

; <label>:429:                                    ; preds = %418
  %430 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %425, i32 0, i32 0
  %431 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %430, align 8
  %432 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %106, align 8
  %433 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %432, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %433, %"union.std::__1::__hash_value_type"** %104, align 8
  %434 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %104, align 8
  %435 = bitcast %"union.std::__1::__hash_value_type"* %434 to %"struct.std::__1::pair.92"*
  store %"struct.std::__1::pair.92"* %435, %"struct.std::__1::pair.92"** %103, align 8
  %436 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %103, align 8
  %437 = bitcast %"struct.std::__1::pair.92"* %436 to i8*
  store %"class.std::__1::allocator.46"* %431, %"class.std::__1::allocator.46"** %99, align 8
  store %"struct.std::__1::pair.92"* %436, %"struct.std::__1::pair.92"** %100, align 8
  %438 = bitcast %"struct.std::__1::__has_destroy"* %102 to %"struct.std::__1::integral_constant.99"*
  %439 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %99, align 8
  %440 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %100, align 8
  store %"class.std::__1::allocator.46"* %439, %"class.std::__1::allocator.46"** %97, align 8
  store %"struct.std::__1::pair.92"* %440, %"struct.std::__1::pair.92"** %98, align 8
  %441 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %98, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEED1Ev(%"struct.std::__1::pair.92"* %441) #14
  br label %442

; <label>:442:                                    ; preds = %429, %418
  %443 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %106, align 8
  %444 = icmp ne %"struct.std::__1::__hash_node"* %443, null
  br i1 %444, label %445, label %456

; <label>:445:                                    ; preds = %442
  %446 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %425, i32 0, i32 0
  %447 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %446, align 8
  %448 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %106, align 8
  store %"class.std::__1::allocator.46"* %447, %"class.std::__1::allocator.46"** %94, align 8
  store %"struct.std::__1::__hash_node"* %448, %"struct.std::__1::__hash_node"** %95, align 8
  store i64 1, i64* %96, align 8
  %449 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %94, align 8
  %450 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %95, align 8
  %451 = load i64, i64* %96, align 8
  store %"class.std::__1::allocator.46"* %449, %"class.std::__1::allocator.46"** %91, align 8
  store %"struct.std::__1::__hash_node"* %450, %"struct.std::__1::__hash_node"** %92, align 8
  store i64 %451, i64* %93, align 8
  %452 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %91, align 8
  %453 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %92, align 8
  %454 = bitcast %"struct.std::__1::__hash_node"* %453 to i8*
  store i8* %454, i8** %90, align 8
  %455 = load i8*, i8** %90, align 8
  call void @_ZdlPv(i8* %455) #17
  br label %456

; <label>:456:                                    ; preds = %445, %442
  br label %457

; <label>:457:                                    ; preds = %397, %456
  br label %517

; <label>:458:                                    ; preds = %370
  store %"class.std::__1::unique_ptr.94"* %0, %"class.std::__1::unique_ptr.94"** %175, align 8
  %459 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %175, align 8
  store %"class.std::__1::unique_ptr.94"* %459, %"class.std::__1::unique_ptr.94"** %174, align 8
  %460 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %174, align 8
  store %"class.std::__1::unique_ptr.94"* %460, %"class.std::__1::unique_ptr.94"** %171, align 8
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %172, align 8
  %461 = load %"class.std::__1::unique_ptr.94"*, %"class.std::__1::unique_ptr.94"** %171, align 8
  %462 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %461, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %462, %"class.std::__1::__compressed_pair.95"** %170, align 8
  %463 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %170, align 8
  %464 = bitcast %"class.std::__1::__compressed_pair.95"* %463 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %464, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %169, align 8
  %465 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %169, align 8
  %466 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %465, i32 0, i32 0
  %467 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %466, align 8
  store %"struct.std::__1::__hash_node"* %467, %"struct.std::__1::__hash_node"** %173, align 8
  %468 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %172, align 8
  %469 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %461, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %469, %"class.std::__1::__compressed_pair.95"** %151, align 8
  %470 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %151, align 8
  %471 = bitcast %"class.std::__1::__compressed_pair.95"* %470 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %471, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %150, align 8
  %472 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %150, align 8
  %473 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %472, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* %468, %"struct.std::__1::__hash_node"** %473, align 8
  %474 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %173, align 8
  %475 = icmp ne %"struct.std::__1::__hash_node"* %474, null
  br i1 %475, label %476, label %515

; <label>:476:                                    ; preds = %458
  %477 = getelementptr inbounds %"class.std::__1::unique_ptr.94", %"class.std::__1::unique_ptr.94"* %461, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.95"* %477, %"class.std::__1::__compressed_pair.95"** %149, align 8
  %478 = load %"class.std::__1::__compressed_pair.95"*, %"class.std::__1::__compressed_pair.95"** %149, align 8
  %479 = bitcast %"class.std::__1::__compressed_pair.95"* %478 to %"class.std::__1::__libcpp_compressed_pair_imp.96"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.96"* %479, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %148, align 8
  %480 = load %"class.std::__1::__libcpp_compressed_pair_imp.96"*, %"class.std::__1::__libcpp_compressed_pair_imp.96"** %148, align 8
  %481 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.96", %"class.std::__1::__libcpp_compressed_pair_imp.96"* %480, i32 0, i32 1
  %482 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %173, align 8
  store %"class.std::__1::__hash_node_destructor"* %481, %"class.std::__1::__hash_node_destructor"** %167, align 8
  store %"struct.std::__1::__hash_node"* %482, %"struct.std::__1::__hash_node"** %168, align 8
  %483 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %167, align 8
  %484 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %483, i32 0, i32 1
  %485 = load i8, i8* %484, align 8
  %486 = trunc i8 %485 to i1
  br i1 %486, label %487, label %500

; <label>:487:                                    ; preds = %476
  %488 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %483, i32 0, i32 0
  %489 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %488, align 8
  %490 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %168, align 8
  %491 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %490, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %491, %"union.std::__1::__hash_value_type"** %166, align 8
  %492 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %166, align 8
  %493 = bitcast %"union.std::__1::__hash_value_type"* %492 to %"struct.std::__1::pair.92"*
  store %"struct.std::__1::pair.92"* %493, %"struct.std::__1::pair.92"** %165, align 8
  %494 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %165, align 8
  %495 = bitcast %"struct.std::__1::pair.92"* %494 to i8*
  store %"class.std::__1::allocator.46"* %489, %"class.std::__1::allocator.46"** %161, align 8
  store %"struct.std::__1::pair.92"* %494, %"struct.std::__1::pair.92"** %162, align 8
  %496 = bitcast %"struct.std::__1::__has_destroy"* %164 to %"struct.std::__1::integral_constant.99"*
  %497 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %161, align 8
  %498 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %162, align 8
  store %"class.std::__1::allocator.46"* %497, %"class.std::__1::allocator.46"** %159, align 8
  store %"struct.std::__1::pair.92"* %498, %"struct.std::__1::pair.92"** %160, align 8
  %499 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %160, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEED1Ev(%"struct.std::__1::pair.92"* %499) #14
  br label %500

; <label>:500:                                    ; preds = %487, %476
  %501 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %168, align 8
  %502 = icmp ne %"struct.std::__1::__hash_node"* %501, null
  br i1 %502, label %503, label %514

; <label>:503:                                    ; preds = %500
  %504 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %483, i32 0, i32 0
  %505 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %504, align 8
  %506 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %168, align 8
  store %"class.std::__1::allocator.46"* %505, %"class.std::__1::allocator.46"** %156, align 8
  store %"struct.std::__1::__hash_node"* %506, %"struct.std::__1::__hash_node"** %157, align 8
  store i64 1, i64* %158, align 8
  %507 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %156, align 8
  %508 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %157, align 8
  %509 = load i64, i64* %158, align 8
  store %"class.std::__1::allocator.46"* %507, %"class.std::__1::allocator.46"** %153, align 8
  store %"struct.std::__1::__hash_node"* %508, %"struct.std::__1::__hash_node"** %154, align 8
  store i64 %509, i64* %155, align 8
  %510 = load %"class.std::__1::allocator.46"*, %"class.std::__1::allocator.46"** %153, align 8
  %511 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %154, align 8
  %512 = bitcast %"struct.std::__1::__hash_node"* %511 to i8*
  store i8* %512, i8** %152, align 8
  %513 = load i8*, i8** %152, align 8
  call void @_ZdlPv(i8* %513) #17
  br label %514

; <label>:514:                                    ; preds = %503, %500
  br label %515

; <label>:515:                                    ; preds = %458, %514
  br label %516

; <label>:516:                                    ; preds = %515, %370
  ret void

; <label>:517:                                    ; preds = %457
  %518 = load i8*, i8** %187, align 8
  %519 = load i32, i32* %188, align 4
  %520 = insertvalue { i8*, i32 } undef, i8* %518, 0
  %521 = insertvalue { i8*, i32 } %520, i32 %519, 1
  resume { i8*, i32 } %521
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE6rehashEm(%"class.std::__1::__hash_table"*, i64) #0 align 2 {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.51"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair.50"*, align 8
  %5 = alloca %"class.std::__1::__hash_table"*, align 8
  %6 = alloca float, align 4
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  %9 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.49"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair.48"*, align 8
  %11 = alloca %"class.std::__1::__hash_table"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.51"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair.50"*, align 8
  %14 = alloca %"class.std::__1::__hash_table"*, align 8
  %15 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.49"*, align 8
  %16 = alloca %"class.std::__1::__compressed_pair.48"*, align 8
  %17 = alloca %"class.std::__1::__hash_table"*, align 8
  %18 = alloca float, align 4
  %19 = alloca %"struct.std::__1::__less"*, align 8
  %20 = alloca i64*, align 8
  %21 = alloca i64*, align 8
  %22 = alloca %"struct.std::__1::__less", align 1
  %23 = alloca i64*, align 8
  %24 = alloca i64*, align 8
  %25 = alloca i64*, align 8
  %26 = alloca i64*, align 8
  %27 = alloca %"struct.std::__1::__less", align 1
  %28 = alloca i64, align 8
  %29 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.41"*, align 8
  %30 = alloca %"class.std::__1::__compressed_pair.40"*, align 8
  %31 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %32 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %33 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %34 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %35 = alloca %"class.std::__1::__hash_table"*, align 8
  %36 = alloca %"class.std::__1::__hash_table"*, align 8
  %37 = alloca i64, align 8
  %38 = alloca i64, align 8
  %39 = alloca i64, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %36, align 8
  store i64 %1, i64* %37, align 8
  %40 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %36, align 8
  %41 = load i64, i64* %37, align 8
  %42 = icmp eq i64 %41, 1
  br i1 %42, label %43, label %44

; <label>:43:                                     ; preds = %2
  store i64 2, i64* %37, align 8
  br label %54

; <label>:44:                                     ; preds = %2
  %45 = load i64, i64* %37, align 8
  %46 = load i64, i64* %37, align 8
  %47 = sub i64 %46, 1
  %48 = and i64 %45, %47
  %49 = icmp ne i64 %48, 0
  br i1 %49, label %50, label %53

; <label>:50:                                     ; preds = %44
  %51 = load i64, i64* %37, align 8
  %52 = call i64 @_ZNSt3__112__next_primeEm(i64 %51)
  store i64 %52, i64* %37, align 8
  br label %53

; <label>:53:                                     ; preds = %50, %44
  br label %54

; <label>:54:                                     ; preds = %53, %43
  store %"class.std::__1::__hash_table"* %40, %"class.std::__1::__hash_table"** %35, align 8
  %55 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %35, align 8
  %56 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %55, i32 0, i32 0
  store %"class.std::__1::unique_ptr.37"* %56, %"class.std::__1::unique_ptr.37"** %34, align 8
  %57 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %34, align 8
  %58 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %57, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %58, %"class.std::__1::__compressed_pair.38"** %33, align 8
  %59 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %33, align 8
  %60 = bitcast %"class.std::__1::__compressed_pair.38"* %59 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %60, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %32, align 8
  %61 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %32, align 8
  %62 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %61, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator"* %62, %"class.std::__1::__bucket_list_deallocator"** %31, align 8
  %63 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %31, align 8
  %64 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %63, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.40"* %64, %"class.std::__1::__compressed_pair.40"** %30, align 8
  %65 = load %"class.std::__1::__compressed_pair.40"*, %"class.std::__1::__compressed_pair.40"** %30, align 8
  %66 = bitcast %"class.std::__1::__compressed_pair.40"* %65 to %"class.std::__1::__libcpp_compressed_pair_imp.41"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.41"* %66, %"class.std::__1::__libcpp_compressed_pair_imp.41"** %29, align 8
  %67 = load %"class.std::__1::__libcpp_compressed_pair_imp.41"*, %"class.std::__1::__libcpp_compressed_pair_imp.41"** %29, align 8
  %68 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.41", %"class.std::__1::__libcpp_compressed_pair_imp.41"* %67, i32 0, i32 0
  %69 = load i64, i64* %68, align 8
  store i64 %69, i64* %38, align 8
  %70 = load i64, i64* %37, align 8
  %71 = load i64, i64* %38, align 8
  %72 = icmp ugt i64 %70, %71
  br i1 %72, label %73, label %75

; <label>:73:                                     ; preds = %54
  %74 = load i64, i64* %37, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE8__rehashEm(%"class.std::__1::__hash_table"* %40, i64 %74)
  br label %167

; <label>:75:                                     ; preds = %54
  %76 = load i64, i64* %37, align 8
  %77 = load i64, i64* %38, align 8
  %78 = icmp ult i64 %76, %77
  br i1 %78, label %79, label %166

; <label>:79:                                     ; preds = %75
  %80 = load i64, i64* %38, align 8
  store i64 %80, i64* %28, align 8
  %81 = load i64, i64* %28, align 8
  %82 = icmp ugt i64 %81, 2
  br i1 %82, label %83, label %90

; <label>:83:                                     ; preds = %79
  %84 = load i64, i64* %28, align 8
  %85 = load i64, i64* %28, align 8
  %86 = sub i64 %85, 1
  %87 = and i64 %84, %86
  %88 = icmp ne i64 %87, 0
  %89 = xor i1 %88, true
  br label %90

; <label>:90:                                     ; preds = %79, %83
  %91 = phi i1 [ false, %79 ], [ %89, %83 ]
  br i1 %91, label %92, label %120

; <label>:92:                                     ; preds = %90
  store %"class.std::__1::__hash_table"* %40, %"class.std::__1::__hash_table"** %17, align 8
  %93 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %17, align 8
  %94 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %93, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.48"* %94, %"class.std::__1::__compressed_pair.48"** %16, align 8
  %95 = load %"class.std::__1::__compressed_pair.48"*, %"class.std::__1::__compressed_pair.48"** %16, align 8
  %96 = bitcast %"class.std::__1::__compressed_pair.48"* %95 to %"class.std::__1::__libcpp_compressed_pair_imp.49"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.49"* %96, %"class.std::__1::__libcpp_compressed_pair_imp.49"** %15, align 8
  %97 = load %"class.std::__1::__libcpp_compressed_pair_imp.49"*, %"class.std::__1::__libcpp_compressed_pair_imp.49"** %15, align 8
  %98 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.49", %"class.std::__1::__libcpp_compressed_pair_imp.49"* %97, i32 0, i32 0
  %99 = load i64, i64* %98, align 8
  %100 = uitofp i64 %99 to float
  store %"class.std::__1::__hash_table"* %40, %"class.std::__1::__hash_table"** %5, align 8
  %101 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %5, align 8
  %102 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %101, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.50"* %102, %"class.std::__1::__compressed_pair.50"** %4, align 8
  %103 = load %"class.std::__1::__compressed_pair.50"*, %"class.std::__1::__compressed_pair.50"** %4, align 8
  %104 = bitcast %"class.std::__1::__compressed_pair.50"* %103 to %"class.std::__1::__libcpp_compressed_pair_imp.51"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.51"* %104, %"class.std::__1::__libcpp_compressed_pair_imp.51"** %3, align 8
  %105 = load %"class.std::__1::__libcpp_compressed_pair_imp.51"*, %"class.std::__1::__libcpp_compressed_pair_imp.51"** %3, align 8
  %106 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.51", %"class.std::__1::__libcpp_compressed_pair_imp.51"* %105, i32 0, i32 0
  %107 = load float, float* %106, align 4
  %108 = fdiv float %100, %107
  store float %108, float* %6, align 4
  %109 = load float, float* %6, align 4
  %110 = call float @ceilf(float %109) #12
  %111 = fptoui float %110 to i64
  store i64 %111, i64* %8, align 8
  %112 = load i64, i64* %8, align 8
  %113 = sub i64 %112, 1
  store i64 %113, i64* %7, align 8
  %114 = load i64, i64* %7, align 8
  %115 = call i64 @llvm.ctlz.i64(i64 %114, i1 true) #14
  %116 = trunc i64 %115 to i32
  %117 = sext i32 %116 to i64
  %118 = sub i64 64, %117
  %119 = shl i64 1, %118
  br label %141

; <label>:120:                                    ; preds = %90
  store %"class.std::__1::__hash_table"* %40, %"class.std::__1::__hash_table"** %11, align 8
  %121 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %11, align 8
  %122 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %121, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.48"* %122, %"class.std::__1::__compressed_pair.48"** %10, align 8
  %123 = load %"class.std::__1::__compressed_pair.48"*, %"class.std::__1::__compressed_pair.48"** %10, align 8
  %124 = bitcast %"class.std::__1::__compressed_pair.48"* %123 to %"class.std::__1::__libcpp_compressed_pair_imp.49"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.49"* %124, %"class.std::__1::__libcpp_compressed_pair_imp.49"** %9, align 8
  %125 = load %"class.std::__1::__libcpp_compressed_pair_imp.49"*, %"class.std::__1::__libcpp_compressed_pair_imp.49"** %9, align 8
  %126 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.49", %"class.std::__1::__libcpp_compressed_pair_imp.49"* %125, i32 0, i32 0
  %127 = load i64, i64* %126, align 8
  %128 = uitofp i64 %127 to float
  store %"class.std::__1::__hash_table"* %40, %"class.std::__1::__hash_table"** %14, align 8
  %129 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %14, align 8
  %130 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %129, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.50"* %130, %"class.std::__1::__compressed_pair.50"** %13, align 8
  %131 = load %"class.std::__1::__compressed_pair.50"*, %"class.std::__1::__compressed_pair.50"** %13, align 8
  %132 = bitcast %"class.std::__1::__compressed_pair.50"* %131 to %"class.std::__1::__libcpp_compressed_pair_imp.51"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.51"* %132, %"class.std::__1::__libcpp_compressed_pair_imp.51"** %12, align 8
  %133 = load %"class.std::__1::__libcpp_compressed_pair_imp.51"*, %"class.std::__1::__libcpp_compressed_pair_imp.51"** %12, align 8
  %134 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.51", %"class.std::__1::__libcpp_compressed_pair_imp.51"* %133, i32 0, i32 0
  %135 = load float, float* %134, align 4
  %136 = fdiv float %128, %135
  store float %136, float* %18, align 4
  %137 = load float, float* %18, align 4
  %138 = call float @ceilf(float %137) #12
  %139 = fptoui float %138 to i64
  %140 = call i64 @_ZNSt3__112__next_primeEm(i64 %139)
  br label %141

; <label>:141:                                    ; preds = %120, %92
  %142 = phi i64 [ %119, %92 ], [ %140, %120 ]
  store i64 %142, i64* %39, align 8
  store i64* %37, i64** %25, align 8
  store i64* %39, i64** %26, align 8
  %143 = load i64*, i64** %25, align 8
  %144 = load i64*, i64** %26, align 8
  store i64* %143, i64** %23, align 8
  store i64* %144, i64** %24, align 8
  %145 = load i64*, i64** %23, align 8
  %146 = load i64*, i64** %24, align 8
  store %"struct.std::__1::__less"* %22, %"struct.std::__1::__less"** %19, align 8
  store i64* %145, i64** %20, align 8
  store i64* %146, i64** %21, align 8
  %147 = load %"struct.std::__1::__less"*, %"struct.std::__1::__less"** %19, align 8
  %148 = load i64*, i64** %20, align 8
  %149 = load i64, i64* %148, align 8
  %150 = load i64*, i64** %21, align 8
  %151 = load i64, i64* %150, align 8
  %152 = icmp ult i64 %149, %151
  br i1 %152, label %153, label %155

; <label>:153:                                    ; preds = %141
  %154 = load i64*, i64** %24, align 8
  br label %157

; <label>:155:                                    ; preds = %141
  %156 = load i64*, i64** %23, align 8
  br label %157

; <label>:157:                                    ; preds = %153, %155
  %158 = phi i64* [ %154, %153 ], [ %156, %155 ]
  %159 = load i64, i64* %158, align 8
  store i64 %159, i64* %37, align 8
  %160 = load i64, i64* %37, align 8
  %161 = load i64, i64* %38, align 8
  %162 = icmp ult i64 %160, %161
  br i1 %162, label %163, label %165

; <label>:163:                                    ; preds = %157
  %164 = load i64, i64* %37, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE8__rehashEm(%"class.std::__1::__hash_table"* %40, i64 %164)
  br label %165

; <label>:165:                                    ; preds = %163, %157
  br label %166

; <label>:166:                                    ; preds = %165, %75
  br label %167

; <label>:167:                                    ; preds = %166, %73
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNKSt3__14hashINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEclERKS6_(%"struct.std::__1::hash"*, %"class.std::__1::basic_string"* dereferenceable(24)) #4 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %5 = alloca %"class.std::__1::basic_string"*, align 8
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %8 = alloca %"class.std::__1::basic_string"*, align 8
  %9 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca i8*, align 8
  %14 = alloca i8*, align 8
  %15 = alloca %"struct.std::__1::__murmur2_or_cityhash", align 1
  %16 = alloca i8*, align 8
  %17 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %18 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %19 = alloca %"class.std::__1::basic_string"*, align 8
  %20 = alloca i8*, align 8
  %21 = alloca i8*, align 8
  %22 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %23 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %24 = alloca %"class.std::__1::basic_string"*, align 8
  %25 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %26 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %27 = alloca %"class.std::__1::basic_string"*, align 8
  %28 = alloca %"class.std::__1::basic_string"*, align 8
  %29 = alloca %"class.std::__1::basic_string"*, align 8
  %30 = alloca i8*, align 8
  %31 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %32 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %33 = alloca %"class.std::__1::basic_string"*, align 8
  %34 = alloca i8*, align 8
  %35 = alloca i8*, align 8
  %36 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %37 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %38 = alloca %"class.std::__1::basic_string"*, align 8
  %39 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %40 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %41 = alloca %"class.std::__1::basic_string"*, align 8
  %42 = alloca %"class.std::__1::basic_string"*, align 8
  %43 = alloca %"class.std::__1::basic_string"*, align 8
  %44 = alloca %"struct.std::__1::hash"*, align 8
  %45 = alloca %"class.std::__1::basic_string"*, align 8
  store %"struct.std::__1::hash"* %0, %"struct.std::__1::hash"** %44, align 8
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %45, align 8
  %46 = load %"struct.std::__1::hash"*, %"struct.std::__1::hash"** %44, align 8
  %47 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8
  store %"class.std::__1::basic_string"* %47, %"class.std::__1::basic_string"** %43, align 8
  %48 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %43, align 8
  store %"class.std::__1::basic_string"* %48, %"class.std::__1::basic_string"** %42, align 8
  %49 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8
  store %"class.std::__1::basic_string"* %49, %"class.std::__1::basic_string"** %41, align 8
  %50 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8
  %51 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %50, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %51, %"class.std::__1::__compressed_pair.18"** %40, align 8
  %52 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %40, align 8
  %53 = bitcast %"class.std::__1::__compressed_pair.18"* %52 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %53, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %39, align 8
  %54 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %39, align 8
  %55 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %54, i32 0, i32 0
  %56 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %55, i32 0, i32 0
  %57 = bitcast %union.anon* %56 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %58 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %57, i32 0, i32 0
  %59 = bitcast %union.anon.20* %58 to i8*
  %60 = load i8, i8* %59, align 8
  %61 = zext i8 %60 to i32
  %62 = and i32 %61, 1
  %63 = icmp ne i32 %62, 0
  br i1 %63, label %64, label %75

; <label>:64:                                     ; preds = %2
  store %"class.std::__1::basic_string"* %49, %"class.std::__1::basic_string"** %33, align 8
  %65 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8
  %66 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %65, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %66, %"class.std::__1::__compressed_pair.18"** %32, align 8
  %67 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %32, align 8
  %68 = bitcast %"class.std::__1::__compressed_pair.18"* %67 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %68, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %31, align 8
  %69 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %31, align 8
  %70 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %69, i32 0, i32 0
  %71 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %70, i32 0, i32 0
  %72 = bitcast %union.anon* %71 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %73 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %72, i32 0, i32 2
  %74 = load i8*, i8** %73, align 8
  br label %88

; <label>:75:                                     ; preds = %2
  store %"class.std::__1::basic_string"* %49, %"class.std::__1::basic_string"** %38, align 8
  %76 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8
  %77 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %76, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %77, %"class.std::__1::__compressed_pair.18"** %37, align 8
  %78 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %37, align 8
  %79 = bitcast %"class.std::__1::__compressed_pair.18"* %78 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %79, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %36, align 8
  %80 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %36, align 8
  %81 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %80, i32 0, i32 0
  %82 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %81, i32 0, i32 0
  %83 = bitcast %union.anon* %82 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %84 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %83, i32 0, i32 1
  %85 = getelementptr inbounds [23 x i8], [23 x i8]* %84, i64 0, i64 0
  store i8* %85, i8** %35, align 8
  %86 = load i8*, i8** %35, align 8
  store i8* %86, i8** %34, align 8
  %87 = load i8*, i8** %34, align 8
  br label %88

; <label>:88:                                     ; preds = %64, %75
  %89 = phi i8* [ %74, %64 ], [ %87, %75 ]
  store i8* %89, i8** %30, align 8
  %90 = load i8*, i8** %30, align 8
  %91 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8
  store %"class.std::__1::basic_string"* %91, %"class.std::__1::basic_string"** %29, align 8
  %92 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %29, align 8
  store %"class.std::__1::basic_string"* %92, %"class.std::__1::basic_string"** %28, align 8
  %93 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8
  store %"class.std::__1::basic_string"* %93, %"class.std::__1::basic_string"** %27, align 8
  %94 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %27, align 8
  %95 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %94, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %95, %"class.std::__1::__compressed_pair.18"** %26, align 8
  %96 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %26, align 8
  %97 = bitcast %"class.std::__1::__compressed_pair.18"* %96 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %97, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %25, align 8
  %98 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %25, align 8
  %99 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %98, i32 0, i32 0
  %100 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %99, i32 0, i32 0
  %101 = bitcast %union.anon* %100 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %102 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %101, i32 0, i32 0
  %103 = bitcast %union.anon.20* %102 to i8*
  %104 = load i8, i8* %103, align 8
  %105 = zext i8 %104 to i32
  %106 = and i32 %105, 1
  %107 = icmp ne i32 %106, 0
  br i1 %107, label %108, label %119

; <label>:108:                                    ; preds = %88
  store %"class.std::__1::basic_string"* %93, %"class.std::__1::basic_string"** %19, align 8
  %109 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %19, align 8
  %110 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %109, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %110, %"class.std::__1::__compressed_pair.18"** %18, align 8
  %111 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %18, align 8
  %112 = bitcast %"class.std::__1::__compressed_pair.18"* %111 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %112, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %17, align 8
  %113 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %17, align 8
  %114 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %113, i32 0, i32 0
  %115 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %114, i32 0, i32 0
  %116 = bitcast %union.anon* %115 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %117 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %116, i32 0, i32 2
  %118 = load i8*, i8** %117, align 8
  br label %132

; <label>:119:                                    ; preds = %88
  store %"class.std::__1::basic_string"* %93, %"class.std::__1::basic_string"** %24, align 8
  %120 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8
  %121 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %120, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %121, %"class.std::__1::__compressed_pair.18"** %23, align 8
  %122 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %23, align 8
  %123 = bitcast %"class.std::__1::__compressed_pair.18"* %122 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %123, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %22, align 8
  %124 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %22, align 8
  %125 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %124, i32 0, i32 0
  %126 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %125, i32 0, i32 0
  %127 = bitcast %union.anon* %126 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %128 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %127, i32 0, i32 1
  %129 = getelementptr inbounds [23 x i8], [23 x i8]* %128, i64 0, i64 0
  store i8* %129, i8** %21, align 8
  %130 = load i8*, i8** %21, align 8
  store i8* %130, i8** %20, align 8
  %131 = load i8*, i8** %20, align 8
  br label %132

; <label>:132:                                    ; preds = %108, %119
  %133 = phi i8* [ %118, %108 ], [ %131, %119 ]
  store i8* %133, i8** %16, align 8
  %134 = load i8*, i8** %16, align 8
  %135 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8
  store %"class.std::__1::basic_string"* %135, %"class.std::__1::basic_string"** %12, align 8
  %136 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8
  store %"class.std::__1::basic_string"* %136, %"class.std::__1::basic_string"** %11, align 8
  %137 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8
  %138 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %137, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %138, %"class.std::__1::__compressed_pair.18"** %10, align 8
  %139 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %10, align 8
  %140 = bitcast %"class.std::__1::__compressed_pair.18"* %139 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %140, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %9, align 8
  %141 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %9, align 8
  %142 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %141, i32 0, i32 0
  %143 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %142, i32 0, i32 0
  %144 = bitcast %union.anon* %143 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %145 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %144, i32 0, i32 0
  %146 = bitcast %union.anon.20* %145 to i8*
  %147 = load i8, i8* %146, align 8
  %148 = zext i8 %147 to i32
  %149 = and i32 %148, 1
  %150 = icmp ne i32 %149, 0
  br i1 %150, label %151, label %162

; <label>:151:                                    ; preds = %132
  store %"class.std::__1::basic_string"* %136, %"class.std::__1::basic_string"** %5, align 8
  %152 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %5, align 8
  %153 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %152, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %153, %"class.std::__1::__compressed_pair.18"** %4, align 8
  %154 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %4, align 8
  %155 = bitcast %"class.std::__1::__compressed_pair.18"* %154 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %155, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %3, align 8
  %156 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %3, align 8
  %157 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %156, i32 0, i32 0
  %158 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %157, i32 0, i32 0
  %159 = bitcast %union.anon* %158 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %160 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %159, i32 0, i32 1
  %161 = load i64, i64* %160, align 8
  br label %177

; <label>:162:                                    ; preds = %132
  store %"class.std::__1::basic_string"* %136, %"class.std::__1::basic_string"** %8, align 8
  %163 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %8, align 8
  %164 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %163, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %164, %"class.std::__1::__compressed_pair.18"** %7, align 8
  %165 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %7, align 8
  %166 = bitcast %"class.std::__1::__compressed_pair.18"* %165 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %166, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %6, align 8
  %167 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %6, align 8
  %168 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %167, i32 0, i32 0
  %169 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %168, i32 0, i32 0
  %170 = bitcast %union.anon* %169 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %171 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %170, i32 0, i32 0
  %172 = bitcast %union.anon.20* %171 to i8*
  %173 = load i8, i8* %172, align 8
  %174 = zext i8 %173 to i32
  %175 = ashr i32 %174, 1
  %176 = sext i32 %175 to i64
  br label %177

; <label>:177:                                    ; preds = %151, %162
  %178 = phi i64 [ %161, %151 ], [ %176, %162 ]
  %179 = getelementptr inbounds i8, i8* %134, i64 %178
  store i8* %90, i8** %13, align 8
  store i8* %179, i8** %14, align 8
  %180 = load i8*, i8** %13, align 8
  %181 = load i8*, i8** %14, align 8
  %182 = load i8*, i8** %13, align 8
  %183 = ptrtoint i8* %181 to i64
  %184 = ptrtoint i8* %182 to i64
  %185 = sub i64 %183, %184
  %186 = invoke i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EEclEPKvm(%"struct.std::__1::__murmur2_or_cityhash"* %15, i8* %180, i64 %185)
          to label %187 unwind label %189

; <label>:187:                                    ; preds = %177
  br label %188

; <label>:188:                                    ; preds = %187
  ret i64 %186

; <label>:189:                                    ; preds = %177
  %190 = landingpad { i8*, i32 }
          catch i8* null
  %191 = extractvalue { i8*, i32 } %190, 0
  call void @__clang_call_terminate(i8* %191) #16
  unreachable
}

; Function Attrs: ssp uwtable
define linkonce_odr i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EEclEPKvm(%"struct.std::__1::__murmur2_or_cityhash"*, i8*, i64) #0 align 2 {
  %4 = alloca i8*, align 8
  %5 = alloca i64, align 8
  %6 = alloca i8*, align 8
  %7 = alloca i64, align 8
  %8 = alloca i8*, align 8
  %9 = alloca i64, align 8
  %10 = alloca i64*, align 8
  %11 = alloca i64*, align 8
  %12 = alloca %"struct.std::__1::pair.97"*, align 8
  %13 = alloca %"struct.std::__1::pair.97"*, align 8
  %14 = alloca i8*, align 8
  %15 = alloca i64, align 8
  %16 = alloca i8*, align 8
  %17 = alloca i64, align 8
  %18 = alloca i64*, align 8
  %19 = alloca i64*, align 8
  %20 = alloca %"struct.std::__1::pair.97"*, align 8
  %21 = alloca %"struct.std::__1::pair.97"*, align 8
  %22 = alloca i64*, align 8
  %23 = alloca i64*, align 8
  %24 = alloca i64*, align 8
  %25 = alloca i64*, align 8
  %26 = alloca i64*, align 8
  %27 = alloca i64, align 8
  %28 = alloca i8*, align 8
  %29 = alloca i64, align 8
  %30 = alloca i8*, align 8
  %31 = alloca i64, align 8
  %32 = alloca i8*, align 8
  %33 = alloca i64, align 8
  %34 = alloca i8*, align 8
  %35 = alloca i64, align 8
  %36 = alloca i8*, align 8
  %37 = alloca i64, align 8
  %38 = alloca i64, align 8
  %39 = alloca %"struct.std::__1::__murmur2_or_cityhash"*, align 8
  %40 = alloca i8*, align 8
  %41 = alloca i64, align 8
  %42 = alloca i8*, align 8
  %43 = alloca i64, align 8
  %44 = alloca i64, align 8
  %45 = alloca i64, align 8
  %46 = alloca %"struct.std::__1::pair.97", align 8
  %47 = alloca %"struct.std::__1::pair.97", align 8
  %48 = alloca %"struct.std::__1::pair.97", align 8
  %49 = alloca %"struct.std::__1::pair.97", align 8
  store %"struct.std::__1::__murmur2_or_cityhash"* %0, %"struct.std::__1::__murmur2_or_cityhash"** %39, align 8
  store i8* %1, i8** %40, align 8
  store i64 %2, i64* %41, align 8
  %50 = load %"struct.std::__1::__murmur2_or_cityhash"*, %"struct.std::__1::__murmur2_or_cityhash"** %39, align 8
  %51 = load i8*, i8** %40, align 8
  store i8* %51, i8** %42, align 8
  %52 = load i64, i64* %41, align 8
  %53 = icmp ule i64 %52, 32
  br i1 %53, label %54, label %65

; <label>:54:                                     ; preds = %3
  %55 = load i64, i64* %41, align 8
  %56 = icmp ule i64 %55, 16
  br i1 %56, label %57, label %61

; <label>:57:                                     ; preds = %54
  %58 = load i8*, i8** %42, align 8
  %59 = load i64, i64* %41, align 8
  %60 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE18__hash_len_0_to_16EPKcm(i8* %58, i64 %59)
  store i64 %60, i64* %38, align 8
  br label %287

; <label>:61:                                     ; preds = %54
  %62 = load i8*, i8** %42, align 8
  %63 = load i64, i64* %41, align 8
  %64 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE19__hash_len_17_to_32EPKcm(i8* %62, i64 %63)
  store i64 %64, i64* %38, align 8
  br label %287

; <label>:65:                                     ; preds = %3
  %66 = load i64, i64* %41, align 8
  %67 = icmp ule i64 %66, 64
  br i1 %67, label %68, label %72

; <label>:68:                                     ; preds = %65
  %69 = load i8*, i8** %42, align 8
  %70 = load i64, i64* %41, align 8
  %71 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE19__hash_len_33_to_64EPKcm(i8* %69, i64 %70)
  store i64 %71, i64* %38, align 8
  br label %287

; <label>:72:                                     ; preds = %65
  br label %73

; <label>:73:                                     ; preds = %72
  %74 = load i8*, i8** %42, align 8
  %75 = load i64, i64* %41, align 8
  %76 = getelementptr inbounds i8, i8* %74, i64 %75
  %77 = getelementptr inbounds i8, i8* %76, i64 -40
  store i8* %77, i8** %36, align 8
  %78 = bitcast i64* %37 to i8*
  %79 = load i8*, i8** %36, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %78, i8* %79, i64 8, i32 1, i1 false) #14
  %80 = load i64, i64* %37, align 8
  store i64 %80, i64* %43, align 8
  %81 = load i8*, i8** %42, align 8
  %82 = load i64, i64* %41, align 8
  %83 = getelementptr inbounds i8, i8* %81, i64 %82
  %84 = getelementptr inbounds i8, i8* %83, i64 -16
  store i8* %84, i8** %34, align 8
  %85 = bitcast i64* %35 to i8*
  %86 = load i8*, i8** %34, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %85, i8* %86, i64 8, i32 1, i1 false) #14
  %87 = load i64, i64* %35, align 8
  %88 = load i8*, i8** %42, align 8
  %89 = load i64, i64* %41, align 8
  %90 = getelementptr inbounds i8, i8* %88, i64 %89
  %91 = getelementptr inbounds i8, i8* %90, i64 -56
  store i8* %91, i8** %32, align 8
  %92 = bitcast i64* %33 to i8*
  %93 = load i8*, i8** %32, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %92, i8* %93, i64 8, i32 1, i1 false) #14
  %94 = load i64, i64* %33, align 8
  %95 = add i64 %87, %94
  store i64 %95, i64* %44, align 8
  %96 = load i8*, i8** %42, align 8
  %97 = load i64, i64* %41, align 8
  %98 = getelementptr inbounds i8, i8* %96, i64 %97
  %99 = getelementptr inbounds i8, i8* %98, i64 -48
  store i8* %99, i8** %30, align 8
  %100 = bitcast i64* %31 to i8*
  %101 = load i8*, i8** %30, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %100, i8* %101, i64 8, i32 1, i1 false) #14
  %102 = load i64, i64* %31, align 8
  %103 = load i64, i64* %41, align 8
  %104 = add i64 %102, %103
  %105 = load i8*, i8** %42, align 8
  %106 = load i64, i64* %41, align 8
  %107 = getelementptr inbounds i8, i8* %105, i64 %106
  %108 = getelementptr inbounds i8, i8* %107, i64 -24
  store i8* %108, i8** %28, align 8
  %109 = bitcast i64* %29 to i8*
  %110 = load i8*, i8** %28, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %109, i8* %110, i64 8, i32 1, i1 false) #14
  %111 = load i64, i64* %29, align 8
  %112 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE13__hash_len_16Emm(i64 %104, i64 %111)
  store i64 %112, i64* %45, align 8
  %113 = load i8*, i8** %42, align 8
  %114 = load i64, i64* %41, align 8
  %115 = getelementptr inbounds i8, i8* %113, i64 %114
  %116 = getelementptr inbounds i8, i8* %115, i64 -64
  %117 = load i64, i64* %41, align 8
  %118 = load i64, i64* %45, align 8
  %119 = call { i64, i64 } @_ZNSt3__121__murmur2_or_cityhashImLm64EE29__weak_hash_len_32_with_seedsEPKcmm(i8* %116, i64 %117, i64 %118)
  %120 = bitcast %"struct.std::__1::pair.97"* %46 to { i64, i64 }*
  %121 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %120, i32 0, i32 0
  %122 = extractvalue { i64, i64 } %119, 0
  store i64 %122, i64* %121, align 8
  %123 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %120, i32 0, i32 1
  %124 = extractvalue { i64, i64 } %119, 1
  store i64 %124, i64* %123, align 8
  %125 = load i8*, i8** %42, align 8
  %126 = load i64, i64* %41, align 8
  %127 = getelementptr inbounds i8, i8* %125, i64 %126
  %128 = getelementptr inbounds i8, i8* %127, i64 -32
  %129 = load i64, i64* %44, align 8
  %130 = add i64 %129, -5435081209227447693
  %131 = load i64, i64* %43, align 8
  %132 = call { i64, i64 } @_ZNSt3__121__murmur2_or_cityhashImLm64EE29__weak_hash_len_32_with_seedsEPKcmm(i8* %128, i64 %130, i64 %131)
  %133 = bitcast %"struct.std::__1::pair.97"* %47 to { i64, i64 }*
  %134 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %133, i32 0, i32 0
  %135 = extractvalue { i64, i64 } %132, 0
  store i64 %135, i64* %134, align 8
  %136 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %133, i32 0, i32 1
  %137 = extractvalue { i64, i64 } %132, 1
  store i64 %137, i64* %136, align 8
  %138 = load i64, i64* %43, align 8
  %139 = mul i64 %138, -5435081209227447693
  %140 = load i8*, i8** %42, align 8
  store i8* %140, i8** %16, align 8
  %141 = bitcast i64* %17 to i8*
  %142 = load i8*, i8** %16, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %141, i8* %142, i64 8, i32 1, i1 false) #14
  %143 = load i64, i64* %17, align 8
  %144 = add i64 %139, %143
  store i64 %144, i64* %43, align 8
  %145 = load i64, i64* %41, align 8
  %146 = sub i64 %145, 1
  %147 = and i64 %146, -64
  store i64 %147, i64* %41, align 8
  br label %148

; <label>:148:                                    ; preds = %264, %73
  %149 = load i64, i64* %43, align 8
  %150 = load i64, i64* %44, align 8
  %151 = add i64 %149, %150
  %152 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %46, i32 0, i32 0
  %153 = load i64, i64* %152, align 8
  %154 = add i64 %151, %153
  %155 = load i8*, i8** %42, align 8
  %156 = getelementptr inbounds i8, i8* %155, i64 8
  store i8* %156, i8** %8, align 8
  %157 = bitcast i64* %9 to i8*
  %158 = load i8*, i8** %8, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %157, i8* %158, i64 8, i32 1, i1 false) #14
  %159 = load i64, i64* %9, align 8
  %160 = add i64 %154, %159
  %161 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %160, i32 37)
  %162 = mul i64 %161, -5435081209227447693
  store i64 %162, i64* %43, align 8
  %163 = load i64, i64* %44, align 8
  %164 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %46, i32 0, i32 1
  %165 = load i64, i64* %164, align 8
  %166 = add i64 %163, %165
  %167 = load i8*, i8** %42, align 8
  %168 = getelementptr inbounds i8, i8* %167, i64 48
  store i8* %168, i8** %6, align 8
  %169 = bitcast i64* %7 to i8*
  %170 = load i8*, i8** %6, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %169, i8* %170, i64 8, i32 1, i1 false) #14
  %171 = load i64, i64* %7, align 8
  %172 = add i64 %166, %171
  %173 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %172, i32 42)
  %174 = mul i64 %173, -5435081209227447693
  store i64 %174, i64* %44, align 8
  %175 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %47, i32 0, i32 1
  %176 = load i64, i64* %175, align 8
  %177 = load i64, i64* %43, align 8
  %178 = xor i64 %177, %176
  store i64 %178, i64* %43, align 8
  %179 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %46, i32 0, i32 0
  %180 = load i64, i64* %179, align 8
  %181 = load i8*, i8** %42, align 8
  %182 = getelementptr inbounds i8, i8* %181, i64 40
  store i8* %182, i8** %4, align 8
  %183 = bitcast i64* %5 to i8*
  %184 = load i8*, i8** %4, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %183, i8* %184, i64 8, i32 1, i1 false) #14
  %185 = load i64, i64* %5, align 8
  %186 = add i64 %180, %185
  %187 = load i64, i64* %44, align 8
  %188 = add i64 %187, %186
  store i64 %188, i64* %44, align 8
  %189 = load i64, i64* %45, align 8
  %190 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %47, i32 0, i32 0
  %191 = load i64, i64* %190, align 8
  %192 = add i64 %189, %191
  %193 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %192, i32 33)
  %194 = mul i64 %193, -5435081209227447693
  store i64 %194, i64* %45, align 8
  %195 = load i8*, i8** %42, align 8
  %196 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %46, i32 0, i32 1
  %197 = load i64, i64* %196, align 8
  %198 = mul i64 %197, -5435081209227447693
  %199 = load i64, i64* %43, align 8
  %200 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %47, i32 0, i32 0
  %201 = load i64, i64* %200, align 8
  %202 = add i64 %199, %201
  %203 = call { i64, i64 } @_ZNSt3__121__murmur2_or_cityhashImLm64EE29__weak_hash_len_32_with_seedsEPKcmm(i8* %195, i64 %198, i64 %202)
  %204 = bitcast %"struct.std::__1::pair.97"* %48 to { i64, i64 }*
  %205 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %204, i32 0, i32 0
  %206 = extractvalue { i64, i64 } %203, 0
  store i64 %206, i64* %205, align 8
  %207 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %204, i32 0, i32 1
  %208 = extractvalue { i64, i64 } %203, 1
  store i64 %208, i64* %207, align 8
  store %"struct.std::__1::pair.97"* %46, %"struct.std::__1::pair.97"** %12, align 8
  store %"struct.std::__1::pair.97"* %48, %"struct.std::__1::pair.97"** %13, align 8
  %209 = load %"struct.std::__1::pair.97"*, %"struct.std::__1::pair.97"** %12, align 8
  %210 = load %"struct.std::__1::pair.97"*, %"struct.std::__1::pair.97"** %13, align 8
  %211 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %210, i32 0, i32 0
  store i64* %211, i64** %11, align 8
  %212 = load i64*, i64** %11, align 8
  %213 = load i64, i64* %212, align 8
  %214 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %209, i32 0, i32 0
  store i64 %213, i64* %214, align 8
  %215 = load %"struct.std::__1::pair.97"*, %"struct.std::__1::pair.97"** %13, align 8
  %216 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %215, i32 0, i32 1
  store i64* %216, i64** %10, align 8
  %217 = load i64*, i64** %10, align 8
  %218 = load i64, i64* %217, align 8
  %219 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %209, i32 0, i32 1
  store i64 %218, i64* %219, align 8
  %220 = load i8*, i8** %42, align 8
  %221 = getelementptr inbounds i8, i8* %220, i64 32
  %222 = load i64, i64* %45, align 8
  %223 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %47, i32 0, i32 1
  %224 = load i64, i64* %223, align 8
  %225 = add i64 %222, %224
  %226 = load i64, i64* %44, align 8
  %227 = load i8*, i8** %42, align 8
  %228 = getelementptr inbounds i8, i8* %227, i64 16
  store i8* %228, i8** %14, align 8
  %229 = bitcast i64* %15 to i8*
  %230 = load i8*, i8** %14, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %229, i8* %230, i64 8, i32 1, i1 false) #14
  %231 = load i64, i64* %15, align 8
  %232 = add i64 %226, %231
  %233 = call { i64, i64 } @_ZNSt3__121__murmur2_or_cityhashImLm64EE29__weak_hash_len_32_with_seedsEPKcmm(i8* %221, i64 %225, i64 %232)
  %234 = bitcast %"struct.std::__1::pair.97"* %49 to { i64, i64 }*
  %235 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %234, i32 0, i32 0
  %236 = extractvalue { i64, i64 } %233, 0
  store i64 %236, i64* %235, align 8
  %237 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %234, i32 0, i32 1
  %238 = extractvalue { i64, i64 } %233, 1
  store i64 %238, i64* %237, align 8
  store %"struct.std::__1::pair.97"* %47, %"struct.std::__1::pair.97"** %20, align 8
  store %"struct.std::__1::pair.97"* %49, %"struct.std::__1::pair.97"** %21, align 8
  %239 = load %"struct.std::__1::pair.97"*, %"struct.std::__1::pair.97"** %20, align 8
  %240 = load %"struct.std::__1::pair.97"*, %"struct.std::__1::pair.97"** %21, align 8
  %241 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %240, i32 0, i32 0
  store i64* %241, i64** %19, align 8
  %242 = load i64*, i64** %19, align 8
  %243 = load i64, i64* %242, align 8
  %244 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %239, i32 0, i32 0
  store i64 %243, i64* %244, align 8
  %245 = load %"struct.std::__1::pair.97"*, %"struct.std::__1::pair.97"** %21, align 8
  %246 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %245, i32 0, i32 1
  store i64* %246, i64** %18, align 8
  %247 = load i64*, i64** %18, align 8
  %248 = load i64, i64* %247, align 8
  %249 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %239, i32 0, i32 1
  store i64 %248, i64* %249, align 8
  store i64* %45, i64** %25, align 8
  store i64* %43, i64** %26, align 8
  %250 = load i64*, i64** %25, align 8
  store i64* %250, i64** %24, align 8
  %251 = load i64*, i64** %24, align 8
  %252 = load i64, i64* %251, align 8
  store i64 %252, i64* %27, align 8
  %253 = load i64*, i64** %26, align 8
  store i64* %253, i64** %22, align 8
  %254 = load i64*, i64** %22, align 8
  %255 = load i64, i64* %254, align 8
  %256 = load i64*, i64** %25, align 8
  store i64 %255, i64* %256, align 8
  store i64* %27, i64** %23, align 8
  %257 = load i64*, i64** %23, align 8
  %258 = load i64, i64* %257, align 8
  %259 = load i64*, i64** %26, align 8
  store i64 %258, i64* %259, align 8
  %260 = load i8*, i8** %42, align 8
  %261 = getelementptr inbounds i8, i8* %260, i64 64
  store i8* %261, i8** %42, align 8
  %262 = load i64, i64* %41, align 8
  %263 = sub i64 %262, 64
  store i64 %263, i64* %41, align 8
  br label %264

; <label>:264:                                    ; preds = %148
  %265 = load i64, i64* %41, align 8
  %266 = icmp ne i64 %265, 0
  br i1 %266, label %148, label %267

; <label>:267:                                    ; preds = %264
  %268 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %46, i32 0, i32 0
  %269 = load i64, i64* %268, align 8
  %270 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %47, i32 0, i32 0
  %271 = load i64, i64* %270, align 8
  %272 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE13__hash_len_16Emm(i64 %269, i64 %271)
  %273 = load i64, i64* %44, align 8
  %274 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE11__shift_mixEm(i64 %273)
  %275 = mul i64 %274, -5435081209227447693
  %276 = add i64 %272, %275
  %277 = load i64, i64* %45, align 8
  %278 = add i64 %276, %277
  %279 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %46, i32 0, i32 1
  %280 = load i64, i64* %279, align 8
  %281 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %47, i32 0, i32 1
  %282 = load i64, i64* %281, align 8
  %283 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE13__hash_len_16Emm(i64 %280, i64 %282)
  %284 = load i64, i64* %43, align 8
  %285 = add i64 %283, %284
  %286 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE13__hash_len_16Emm(i64 %278, i64 %285)
  store i64 %286, i64* %38, align 8
  br label %287

; <label>:287:                                    ; preds = %267, %68, %61, %57
  %288 = load i64, i64* %38, align 8
  ret i64 %288
}

; Function Attrs: ssp uwtable
define linkonce_odr i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE18__hash_len_0_to_16EPKcm(i8*, i64) #0 align 2 {
  %3 = alloca i8*, align 8
  %4 = alloca i32, align 4
  %5 = alloca i8*, align 8
  %6 = alloca i32, align 4
  %7 = alloca i8*, align 8
  %8 = alloca i64, align 8
  %9 = alloca i8*, align 8
  %10 = alloca i64, align 8
  %11 = alloca i64, align 8
  %12 = alloca i8*, align 8
  %13 = alloca i64, align 8
  %14 = alloca i64, align 8
  %15 = alloca i64, align 8
  %16 = alloca i32, align 4
  %17 = alloca i32, align 4
  %18 = alloca i8, align 1
  %19 = alloca i8, align 1
  %20 = alloca i8, align 1
  %21 = alloca i32, align 4
  %22 = alloca i32, align 4
  store i8* %0, i8** %12, align 8
  store i64 %1, i64* %13, align 8
  %23 = load i64, i64* %13, align 8
  %24 = icmp ugt i64 %23, 8
  br i1 %24, label %25, label %47

; <label>:25:                                     ; preds = %2
  %26 = load i8*, i8** %12, align 8
  store i8* %26, i8** %9, align 8
  %27 = bitcast i64* %10 to i8*
  %28 = load i8*, i8** %9, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %27, i8* %28, i64 8, i32 1, i1 false) #14
  %29 = load i64, i64* %10, align 8
  store i64 %29, i64* %14, align 8
  %30 = load i8*, i8** %12, align 8
  %31 = load i64, i64* %13, align 8
  %32 = getelementptr inbounds i8, i8* %30, i64 %31
  %33 = getelementptr inbounds i8, i8* %32, i64 -8
  store i8* %33, i8** %7, align 8
  %34 = bitcast i64* %8 to i8*
  %35 = load i8*, i8** %7, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %34, i8* %35, i64 8, i32 1, i1 false) #14
  %36 = load i64, i64* %8, align 8
  store i64 %36, i64* %15, align 8
  %37 = load i64, i64* %14, align 8
  %38 = load i64, i64* %15, align 8
  %39 = load i64, i64* %13, align 8
  %40 = add i64 %38, %39
  %41 = load i64, i64* %13, align 8
  %42 = trunc i64 %41 to i32
  %43 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE22__rotate_by_at_least_1Emi(i64 %40, i32 %42)
  %44 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE13__hash_len_16Emm(i64 %37, i64 %43)
  %45 = load i64, i64* %15, align 8
  %46 = xor i64 %44, %45
  store i64 %46, i64* %11, align 8
  br label %110

; <label>:47:                                     ; preds = %2
  %48 = load i64, i64* %13, align 8
  %49 = icmp uge i64 %48, 4
  br i1 %49, label %50, label %70

; <label>:50:                                     ; preds = %47
  %51 = load i8*, i8** %12, align 8
  store i8* %51, i8** %5, align 8
  %52 = bitcast i32* %6 to i8*
  %53 = load i8*, i8** %5, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %52, i8* %53, i64 4, i32 1, i1 false) #14
  %54 = load i32, i32* %6, align 4
  store i32 %54, i32* %16, align 4
  %55 = load i8*, i8** %12, align 8
  %56 = load i64, i64* %13, align 8
  %57 = getelementptr inbounds i8, i8* %55, i64 %56
  %58 = getelementptr inbounds i8, i8* %57, i64 -4
  store i8* %58, i8** %3, align 8
  %59 = bitcast i32* %4 to i8*
  %60 = load i8*, i8** %3, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %59, i8* %60, i64 4, i32 1, i1 false) #14
  %61 = load i32, i32* %4, align 4
  store i32 %61, i32* %17, align 4
  %62 = load i64, i64* %13, align 8
  %63 = load i32, i32* %16, align 4
  %64 = shl i32 %63, 3
  %65 = zext i32 %64 to i64
  %66 = add i64 %62, %65
  %67 = load i32, i32* %17, align 4
  %68 = zext i32 %67 to i64
  %69 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE13__hash_len_16Emm(i64 %66, i64 %68)
  store i64 %69, i64* %11, align 8
  br label %110

; <label>:70:                                     ; preds = %47
  %71 = load i64, i64* %13, align 8
  %72 = icmp ugt i64 %71, 0
  br i1 %72, label %73, label %109

; <label>:73:                                     ; preds = %70
  %74 = load i8*, i8** %12, align 8
  %75 = getelementptr inbounds i8, i8* %74, i64 0
  %76 = load i8, i8* %75, align 1
  store i8 %76, i8* %18, align 1
  %77 = load i64, i64* %13, align 8
  %78 = lshr i64 %77, 1
  %79 = load i8*, i8** %12, align 8
  %80 = getelementptr inbounds i8, i8* %79, i64 %78
  %81 = load i8, i8* %80, align 1
  store i8 %81, i8* %19, align 1
  %82 = load i64, i64* %13, align 8
  %83 = sub i64 %82, 1
  %84 = load i8*, i8** %12, align 8
  %85 = getelementptr inbounds i8, i8* %84, i64 %83
  %86 = load i8, i8* %85, align 1
  store i8 %86, i8* %20, align 1
  %87 = load i8, i8* %18, align 1
  %88 = zext i8 %87 to i32
  %89 = load i8, i8* %19, align 1
  %90 = zext i8 %89 to i32
  %91 = shl i32 %90, 8
  %92 = add i32 %88, %91
  store i32 %92, i32* %21, align 4
  %93 = load i64, i64* %13, align 8
  %94 = load i8, i8* %20, align 1
  %95 = zext i8 %94 to i32
  %96 = shl i32 %95, 2
  %97 = zext i32 %96 to i64
  %98 = add i64 %93, %97
  %99 = trunc i64 %98 to i32
  store i32 %99, i32* %22, align 4
  %100 = load i32, i32* %21, align 4
  %101 = zext i32 %100 to i64
  %102 = mul i64 %101, -7286425919675154353
  %103 = load i32, i32* %22, align 4
  %104 = zext i32 %103 to i64
  %105 = mul i64 %104, -3942382747735136937
  %106 = xor i64 %102, %105
  %107 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE11__shift_mixEm(i64 %106)
  %108 = mul i64 %107, -7286425919675154353
  store i64 %108, i64* %11, align 8
  br label %110

; <label>:109:                                    ; preds = %70
  store i64 -7286425919675154353, i64* %11, align 8
  br label %110

; <label>:110:                                    ; preds = %109, %73, %50, %25
  %111 = load i64, i64* %11, align 8
  ret i64 %111
}

; Function Attrs: ssp uwtable
define linkonce_odr i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE19__hash_len_17_to_32EPKcm(i8*, i64) #0 align 2 {
  %3 = alloca i8*, align 8
  %4 = alloca i64, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i64, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i64, align 8
  %9 = alloca i8*, align 8
  %10 = alloca i64, align 8
  %11 = alloca i8*, align 8
  %12 = alloca i64, align 8
  %13 = alloca i64, align 8
  %14 = alloca i64, align 8
  %15 = alloca i64, align 8
  %16 = alloca i64, align 8
  store i8* %0, i8** %11, align 8
  store i64 %1, i64* %12, align 8
  %17 = load i8*, i8** %11, align 8
  store i8* %17, i8** %9, align 8
  %18 = bitcast i64* %10 to i8*
  %19 = load i8*, i8** %9, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %18, i8* %19, i64 8, i32 1, i1 false) #14
  %20 = load i64, i64* %10, align 8
  %21 = mul i64 %20, -5435081209227447693
  store i64 %21, i64* %13, align 8
  %22 = load i8*, i8** %11, align 8
  %23 = getelementptr inbounds i8, i8* %22, i64 8
  store i8* %23, i8** %7, align 8
  %24 = bitcast i64* %8 to i8*
  %25 = load i8*, i8** %7, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %24, i8* %25, i64 8, i32 1, i1 false) #14
  %26 = load i64, i64* %8, align 8
  store i64 %26, i64* %14, align 8
  %27 = load i8*, i8** %11, align 8
  %28 = load i64, i64* %12, align 8
  %29 = getelementptr inbounds i8, i8* %27, i64 %28
  %30 = getelementptr inbounds i8, i8* %29, i64 -8
  store i8* %30, i8** %5, align 8
  %31 = bitcast i64* %6 to i8*
  %32 = load i8*, i8** %5, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %31, i8* %32, i64 8, i32 1, i1 false) #14
  %33 = load i64, i64* %6, align 8
  %34 = mul i64 %33, -7286425919675154353
  store i64 %34, i64* %15, align 8
  %35 = load i8*, i8** %11, align 8
  %36 = load i64, i64* %12, align 8
  %37 = getelementptr inbounds i8, i8* %35, i64 %36
  %38 = getelementptr inbounds i8, i8* %37, i64 -16
  store i8* %38, i8** %3, align 8
  %39 = bitcast i64* %4 to i8*
  %40 = load i8*, i8** %3, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %39, i8* %40, i64 8, i32 1, i1 false) #14
  %41 = load i64, i64* %4, align 8
  %42 = mul i64 %41, -4348849565147123417
  store i64 %42, i64* %16, align 8
  %43 = load i64, i64* %13, align 8
  %44 = load i64, i64* %14, align 8
  %45 = sub i64 %43, %44
  %46 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %45, i32 43)
  %47 = load i64, i64* %15, align 8
  %48 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %47, i32 30)
  %49 = add i64 %46, %48
  %50 = load i64, i64* %16, align 8
  %51 = add i64 %49, %50
  %52 = load i64, i64* %13, align 8
  %53 = load i64, i64* %14, align 8
  %54 = xor i64 %53, -3942382747735136937
  %55 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %54, i32 20)
  %56 = add i64 %52, %55
  %57 = load i64, i64* %15, align 8
  %58 = sub i64 %56, %57
  %59 = load i64, i64* %12, align 8
  %60 = add i64 %58, %59
  %61 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE13__hash_len_16Emm(i64 %51, i64 %60)
  ret i64 %61
}

; Function Attrs: ssp uwtable
define linkonce_odr i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE19__hash_len_33_to_64EPKcm(i8*, i64) #0 align 2 {
  %3 = alloca i8*, align 8
  %4 = alloca i64, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i64, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i64, align 8
  %9 = alloca i8*, align 8
  %10 = alloca i64, align 8
  %11 = alloca i8*, align 8
  %12 = alloca i64, align 8
  %13 = alloca i8*, align 8
  %14 = alloca i64, align 8
  %15 = alloca i8*, align 8
  %16 = alloca i64, align 8
  %17 = alloca i8*, align 8
  %18 = alloca i64, align 8
  %19 = alloca i8*, align 8
  %20 = alloca i64, align 8
  %21 = alloca i8*, align 8
  %22 = alloca i64, align 8
  %23 = alloca i8*, align 8
  %24 = alloca i64, align 8
  %25 = alloca i64, align 8
  %26 = alloca i64, align 8
  %27 = alloca i64, align 8
  %28 = alloca i64, align 8
  %29 = alloca i64, align 8
  %30 = alloca i64, align 8
  %31 = alloca i64, align 8
  %32 = alloca i64, align 8
  %33 = alloca i64, align 8
  store i8* %0, i8** %23, align 8
  store i64 %1, i64* %24, align 8
  %34 = load i8*, i8** %23, align 8
  %35 = getelementptr inbounds i8, i8* %34, i64 24
  store i8* %35, i8** %21, align 8
  %36 = bitcast i64* %22 to i8*
  %37 = load i8*, i8** %21, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %36, i8* %37, i64 8, i32 1, i1 false) #14
  %38 = load i64, i64* %22, align 8
  store i64 %38, i64* %25, align 8
  %39 = load i8*, i8** %23, align 8
  store i8* %39, i8** %19, align 8
  %40 = bitcast i64* %20 to i8*
  %41 = load i8*, i8** %19, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %40, i8* %41, i64 8, i32 1, i1 false) #14
  %42 = load i64, i64* %20, align 8
  %43 = load i64, i64* %24, align 8
  %44 = load i8*, i8** %23, align 8
  %45 = load i64, i64* %24, align 8
  %46 = getelementptr inbounds i8, i8* %44, i64 %45
  %47 = getelementptr inbounds i8, i8* %46, i64 -16
  store i8* %47, i8** %17, align 8
  %48 = bitcast i64* %18 to i8*
  %49 = load i8*, i8** %17, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %48, i8* %49, i64 8, i32 1, i1 false) #14
  %50 = load i64, i64* %18, align 8
  %51 = add i64 %43, %50
  %52 = mul i64 %51, -4348849565147123417
  %53 = add i64 %42, %52
  store i64 %53, i64* %26, align 8
  %54 = load i64, i64* %26, align 8
  %55 = load i64, i64* %25, align 8
  %56 = add i64 %54, %55
  %57 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %56, i32 52)
  store i64 %57, i64* %27, align 8
  %58 = load i64, i64* %26, align 8
  %59 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %58, i32 37)
  store i64 %59, i64* %28, align 8
  %60 = load i8*, i8** %23, align 8
  %61 = getelementptr inbounds i8, i8* %60, i64 8
  store i8* %61, i8** %15, align 8
  %62 = bitcast i64* %16 to i8*
  %63 = load i8*, i8** %15, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %62, i8* %63, i64 8, i32 1, i1 false) #14
  %64 = load i64, i64* %16, align 8
  %65 = load i64, i64* %26, align 8
  %66 = add i64 %65, %64
  store i64 %66, i64* %26, align 8
  %67 = load i64, i64* %26, align 8
  %68 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %67, i32 7)
  %69 = load i64, i64* %28, align 8
  %70 = add i64 %69, %68
  store i64 %70, i64* %28, align 8
  %71 = load i8*, i8** %23, align 8
  %72 = getelementptr inbounds i8, i8* %71, i64 16
  store i8* %72, i8** %11, align 8
  %73 = bitcast i64* %12 to i8*
  %74 = load i8*, i8** %11, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %73, i8* %74, i64 8, i32 1, i1 false) #14
  %75 = load i64, i64* %12, align 8
  %76 = load i64, i64* %26, align 8
  %77 = add i64 %76, %75
  store i64 %77, i64* %26, align 8
  %78 = load i64, i64* %26, align 8
  %79 = load i64, i64* %25, align 8
  %80 = add i64 %78, %79
  store i64 %80, i64* %29, align 8
  %81 = load i64, i64* %27, align 8
  %82 = load i64, i64* %26, align 8
  %83 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %82, i32 31)
  %84 = add i64 %81, %83
  %85 = load i64, i64* %28, align 8
  %86 = add i64 %84, %85
  store i64 %86, i64* %30, align 8
  %87 = load i8*, i8** %23, align 8
  %88 = getelementptr inbounds i8, i8* %87, i64 16
  store i8* %88, i8** %7, align 8
  %89 = bitcast i64* %8 to i8*
  %90 = load i8*, i8** %7, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %89, i8* %90, i64 8, i32 1, i1 false) #14
  %91 = load i64, i64* %8, align 8
  %92 = load i8*, i8** %23, align 8
  %93 = load i64, i64* %24, align 8
  %94 = getelementptr inbounds i8, i8* %92, i64 %93
  %95 = getelementptr inbounds i8, i8* %94, i64 -32
  store i8* %95, i8** %5, align 8
  %96 = bitcast i64* %6 to i8*
  %97 = load i8*, i8** %5, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %96, i8* %97, i64 8, i32 1, i1 false) #14
  %98 = load i64, i64* %6, align 8
  %99 = add i64 %91, %98
  store i64 %99, i64* %26, align 8
  %100 = load i8*, i8** %23, align 8
  %101 = load i64, i64* %24, align 8
  %102 = getelementptr inbounds i8, i8* %100, i64 %101
  %103 = getelementptr inbounds i8, i8* %102, i64 -8
  store i8* %103, i8** %3, align 8
  %104 = bitcast i64* %4 to i8*
  %105 = load i8*, i8** %3, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %104, i8* %105, i64 8, i32 1, i1 false) #14
  %106 = load i64, i64* %4, align 8
  %107 = load i64, i64* %25, align 8
  %108 = add i64 %107, %106
  store i64 %108, i64* %25, align 8
  %109 = load i64, i64* %26, align 8
  %110 = load i64, i64* %25, align 8
  %111 = add i64 %109, %110
  %112 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %111, i32 52)
  store i64 %112, i64* %27, align 8
  %113 = load i64, i64* %26, align 8
  %114 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %113, i32 37)
  store i64 %114, i64* %28, align 8
  %115 = load i8*, i8** %23, align 8
  %116 = load i64, i64* %24, align 8
  %117 = getelementptr inbounds i8, i8* %115, i64 %116
  %118 = getelementptr inbounds i8, i8* %117, i64 -24
  store i8* %118, i8** %9, align 8
  %119 = bitcast i64* %10 to i8*
  %120 = load i8*, i8** %9, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %119, i8* %120, i64 8, i32 1, i1 false) #14
  %121 = load i64, i64* %10, align 8
  %122 = load i64, i64* %26, align 8
  %123 = add i64 %122, %121
  store i64 %123, i64* %26, align 8
  %124 = load i64, i64* %26, align 8
  %125 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %124, i32 7)
  %126 = load i64, i64* %28, align 8
  %127 = add i64 %126, %125
  store i64 %127, i64* %28, align 8
  %128 = load i8*, i8** %23, align 8
  %129 = load i64, i64* %24, align 8
  %130 = getelementptr inbounds i8, i8* %128, i64 %129
  %131 = getelementptr inbounds i8, i8* %130, i64 -16
  store i8* %131, i8** %13, align 8
  %132 = bitcast i64* %14 to i8*
  %133 = load i8*, i8** %13, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %132, i8* %133, i64 8, i32 1, i1 false) #14
  %134 = load i64, i64* %14, align 8
  %135 = load i64, i64* %26, align 8
  %136 = add i64 %135, %134
  store i64 %136, i64* %26, align 8
  %137 = load i64, i64* %26, align 8
  %138 = load i64, i64* %25, align 8
  %139 = add i64 %137, %138
  store i64 %139, i64* %31, align 8
  %140 = load i64, i64* %27, align 8
  %141 = load i64, i64* %26, align 8
  %142 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %141, i32 31)
  %143 = add i64 %140, %142
  %144 = load i64, i64* %28, align 8
  %145 = add i64 %143, %144
  store i64 %145, i64* %32, align 8
  %146 = load i64, i64* %29, align 8
  %147 = load i64, i64* %32, align 8
  %148 = add i64 %146, %147
  %149 = mul i64 %148, -7286425919675154353
  %150 = load i64, i64* %31, align 8
  %151 = load i64, i64* %30, align 8
  %152 = add i64 %150, %151
  %153 = mul i64 %152, -4348849565147123417
  %154 = add i64 %149, %153
  %155 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE11__shift_mixEm(i64 %154)
  store i64 %155, i64* %33, align 8
  %156 = load i64, i64* %33, align 8
  %157 = mul i64 %156, -4348849565147123417
  %158 = load i64, i64* %30, align 8
  %159 = add i64 %157, %158
  %160 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE11__shift_mixEm(i64 %159)
  %161 = mul i64 %160, -7286425919675154353
  ret i64 %161
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE13__hash_len_16Emm(i64, i64) #4 align 2 {
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  %5 = alloca i64, align 8
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  store i64 %0, i64* %3, align 8
  store i64 %1, i64* %4, align 8
  store i64 -7070675565921424023, i64* %5, align 8
  %8 = load i64, i64* %3, align 8
  %9 = load i64, i64* %4, align 8
  %10 = xor i64 %8, %9
  %11 = mul i64 %10, -7070675565921424023
  store i64 %11, i64* %6, align 8
  %12 = load i64, i64* %6, align 8
  %13 = lshr i64 %12, 47
  %14 = load i64, i64* %6, align 8
  %15 = xor i64 %14, %13
  store i64 %15, i64* %6, align 8
  %16 = load i64, i64* %4, align 8
  %17 = load i64, i64* %6, align 8
  %18 = xor i64 %16, %17
  %19 = mul i64 %18, -7070675565921424023
  store i64 %19, i64* %7, align 8
  %20 = load i64, i64* %7, align 8
  %21 = lshr i64 %20, 47
  %22 = load i64, i64* %7, align 8
  %23 = xor i64 %22, %21
  store i64 %23, i64* %7, align 8
  %24 = load i64, i64* %7, align 8
  %25 = mul i64 %24, -7070675565921424023
  store i64 %25, i64* %7, align 8
  %26 = load i64, i64* %7, align 8
  ret i64 %26
}

; Function Attrs: ssp uwtable
define linkonce_odr { i64, i64 } @_ZNSt3__121__murmur2_or_cityhashImLm64EE29__weak_hash_len_32_with_seedsEPKcmm(i8*, i64, i64) #0 align 2 {
  %4 = alloca i8*, align 8
  %5 = alloca i64, align 8
  %6 = alloca i8*, align 8
  %7 = alloca i64, align 8
  %8 = alloca i8*, align 8
  %9 = alloca i64, align 8
  %10 = alloca i8*, align 8
  %11 = alloca i64, align 8
  %12 = alloca %"struct.std::__1::pair.97", align 8
  %13 = alloca i8*, align 8
  %14 = alloca i64, align 8
  %15 = alloca i64, align 8
  store i8* %0, i8** %13, align 8
  store i64 %1, i64* %14, align 8
  store i64 %2, i64* %15, align 8
  %16 = load i8*, i8** %13, align 8
  store i8* %16, i8** %10, align 8
  %17 = bitcast i64* %11 to i8*
  %18 = load i8*, i8** %10, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %17, i8* %18, i64 8, i32 1, i1 false) #14
  %19 = load i64, i64* %11, align 8
  %20 = load i8*, i8** %13, align 8
  %21 = getelementptr inbounds i8, i8* %20, i64 8
  store i8* %21, i8** %8, align 8
  %22 = bitcast i64* %9 to i8*
  %23 = load i8*, i8** %8, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %22, i8* %23, i64 8, i32 1, i1 false) #14
  %24 = load i64, i64* %9, align 8
  %25 = load i8*, i8** %13, align 8
  %26 = getelementptr inbounds i8, i8* %25, i64 16
  store i8* %26, i8** %4, align 8
  %27 = bitcast i64* %5 to i8*
  %28 = load i8*, i8** %4, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %27, i8* %28, i64 8, i32 1, i1 false) #14
  %29 = load i64, i64* %5, align 8
  %30 = load i8*, i8** %13, align 8
  %31 = getelementptr inbounds i8, i8* %30, i64 24
  store i8* %31, i8** %6, align 8
  %32 = bitcast i64* %7 to i8*
  %33 = load i8*, i8** %6, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %32, i8* %33, i64 8, i32 1, i1 false) #14
  %34 = load i64, i64* %7, align 8
  %35 = load i64, i64* %14, align 8
  %36 = load i64, i64* %15, align 8
  %37 = call { i64, i64 } @_ZNSt3__121__murmur2_or_cityhashImLm64EE29__weak_hash_len_32_with_seedsEmmmmmm(i64 %19, i64 %24, i64 %29, i64 %34, i64 %35, i64 %36)
  %38 = bitcast %"struct.std::__1::pair.97"* %12 to { i64, i64 }*
  %39 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %38, i32 0, i32 0
  %40 = extractvalue { i64, i64 } %37, 0
  store i64 %40, i64* %39, align 8
  %41 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %38, i32 0, i32 1
  %42 = extractvalue { i64, i64 } %37, 1
  store i64 %42, i64* %41, align 8
  %43 = bitcast %"struct.std::__1::pair.97"* %12 to { i64, i64 }*
  %44 = load { i64, i64 }, { i64, i64 }* %43, align 8
  ret { i64, i64 } %44
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64, i32) #4 align 2 {
  %3 = alloca i64, align 8
  %4 = alloca i32, align 4
  store i64 %0, i64* %3, align 8
  store i32 %1, i32* %4, align 4
  %5 = load i32, i32* %4, align 4
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %7, label %9

; <label>:7:                                      ; preds = %2
  %8 = load i64, i64* %3, align 8
  br label %20

; <label>:9:                                      ; preds = %2
  %10 = load i64, i64* %3, align 8
  %11 = load i32, i32* %4, align 4
  %12 = zext i32 %11 to i64
  %13 = lshr i64 %10, %12
  %14 = load i64, i64* %3, align 8
  %15 = load i32, i32* %4, align 4
  %16 = sub nsw i32 64, %15
  %17 = zext i32 %16 to i64
  %18 = shl i64 %14, %17
  %19 = or i64 %13, %18
  br label %20

; <label>:20:                                     ; preds = %9, %7
  %21 = phi i64 [ %8, %7 ], [ %19, %9 ]
  ret i64 %21
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE11__shift_mixEm(i64) #4 align 2 {
  %2 = alloca i64, align 8
  store i64 %0, i64* %2, align 8
  %3 = load i64, i64* %2, align 8
  %4 = load i64, i64* %2, align 8
  %5 = lshr i64 %4, 47
  %6 = xor i64 %3, %5
  ret i64 %6
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE22__rotate_by_at_least_1Emi(i64, i32) #4 align 2 {
  %3 = alloca i64, align 8
  %4 = alloca i32, align 4
  store i64 %0, i64* %3, align 8
  store i32 %1, i32* %4, align 4
  %5 = load i64, i64* %3, align 8
  %6 = load i32, i32* %4, align 4
  %7 = zext i32 %6 to i64
  %8 = lshr i64 %5, %7
  %9 = load i64, i64* %3, align 8
  %10 = load i32, i32* %4, align 4
  %11 = sub nsw i32 64, %10
  %12 = zext i32 %11 to i64
  %13 = shl i64 %9, %12
  %14 = or i64 %8, %13
  ret i64 %14
}

; Function Attrs: ssp uwtable
define linkonce_odr { i64, i64 } @_ZNSt3__121__murmur2_or_cityhashImLm64EE29__weak_hash_len_32_with_seedsEmmmmmm(i64, i64, i64, i64, i64, i64) #0 align 2 {
  %7 = alloca i64*, align 8
  %8 = alloca i64*, align 8
  %9 = alloca %"struct.std::__1::pair.97"*, align 8
  %10 = alloca i64*, align 8
  %11 = alloca i64*, align 8
  %12 = alloca %"struct.std::__1::pair.97"*, align 8
  %13 = alloca i64*, align 8
  %14 = alloca i64*, align 8
  %15 = alloca %"struct.std::__1::pair.97", align 8
  %16 = alloca i64, align 8
  %17 = alloca i64, align 8
  %18 = alloca i64, align 8
  %19 = alloca i64, align 8
  %20 = alloca i64, align 8
  %21 = alloca i64, align 8
  %22 = alloca i64, align 8
  %23 = alloca i64, align 8
  %24 = alloca i64, align 8
  store i64 %0, i64* %16, align 8
  store i64 %1, i64* %17, align 8
  store i64 %2, i64* %18, align 8
  store i64 %3, i64* %19, align 8
  store i64 %4, i64* %20, align 8
  store i64 %5, i64* %21, align 8
  %25 = load i64, i64* %16, align 8
  %26 = load i64, i64* %20, align 8
  %27 = add i64 %26, %25
  store i64 %27, i64* %20, align 8
  %28 = load i64, i64* %21, align 8
  %29 = load i64, i64* %20, align 8
  %30 = add i64 %28, %29
  %31 = load i64, i64* %19, align 8
  %32 = add i64 %30, %31
  %33 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %32, i32 21)
  store i64 %33, i64* %21, align 8
  %34 = load i64, i64* %20, align 8
  store i64 %34, i64* %22, align 8
  %35 = load i64, i64* %17, align 8
  %36 = load i64, i64* %20, align 8
  %37 = add i64 %36, %35
  store i64 %37, i64* %20, align 8
  %38 = load i64, i64* %18, align 8
  %39 = load i64, i64* %20, align 8
  %40 = add i64 %39, %38
  store i64 %40, i64* %20, align 8
  %41 = load i64, i64* %20, align 8
  %42 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %41, i32 44)
  %43 = load i64, i64* %21, align 8
  %44 = add i64 %43, %42
  store i64 %44, i64* %21, align 8
  %45 = load i64, i64* %20, align 8
  %46 = load i64, i64* %19, align 8
  %47 = add i64 %45, %46
  store i64 %47, i64* %23, align 8
  %48 = load i64, i64* %21, align 8
  %49 = load i64, i64* %22, align 8
  %50 = add i64 %48, %49
  store i64 %50, i64* %24, align 8
  store %"struct.std::__1::pair.97"* %15, %"struct.std::__1::pair.97"** %12, align 8
  store i64* %23, i64** %13, align 8
  store i64* %24, i64** %14, align 8
  %51 = load %"struct.std::__1::pair.97"*, %"struct.std::__1::pair.97"** %12, align 8
  %52 = load i64*, i64** %13, align 8
  %53 = load i64*, i64** %14, align 8
  store %"struct.std::__1::pair.97"* %51, %"struct.std::__1::pair.97"** %9, align 8
  store i64* %52, i64** %10, align 8
  store i64* %53, i64** %11, align 8
  %54 = load %"struct.std::__1::pair.97"*, %"struct.std::__1::pair.97"** %9, align 8
  %55 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %54, i32 0, i32 0
  %56 = load i64*, i64** %10, align 8
  store i64* %56, i64** %8, align 8
  %57 = load i64*, i64** %8, align 8
  %58 = load i64, i64* %57, align 8
  store i64 %58, i64* %55, align 8
  %59 = getelementptr inbounds %"struct.std::__1::pair.97", %"struct.std::__1::pair.97"* %54, i32 0, i32 1
  %60 = load i64*, i64** %11, align 8
  store i64* %60, i64** %7, align 8
  %61 = load i64*, i64** %7, align 8
  %62 = load i64, i64* %61, align 8
  store i64 %62, i64* %59, align 8
  %63 = bitcast %"struct.std::__1::pair.97"* %15 to { i64, i64 }*
  %64 = load { i64, i64 }, { i64, i64 }* %63, align 8
  ret { i64, i64 } %64
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8*, i8*, i64) #3 align 2 {
  %4 = alloca i8*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i64, align 8
  store i8* %0, i8** %4, align 8
  store i8* %1, i8** %5, align 8
  store i64 %2, i64* %6, align 8
  %7 = load i64, i64* %6, align 8
  %8 = icmp eq i64 %7, 0
  br i1 %8, label %9, label %10

; <label>:9:                                      ; preds = %3
  br label %15

; <label>:10:                                     ; preds = %3
  %11 = load i8*, i8** %4, align 8
  %12 = load i8*, i8** %5, align 8
  %13 = load i64, i64* %6, align 8
  %14 = call i32 @memcmp(i8* %11, i8* %12, i64 %13)
  br label %15

; <label>:15:                                     ; preds = %10, %9
  %16 = phi i32 [ 0, %9 ], [ %14, %10 ]
  ret i32 %16
}

declare i32 @memcmp(i8*, i8*, i64) #1

; Function Attrs: nobuiltin
declare noalias i8* @_Znwm(i64) #10

declare i64 @_ZNSt3__112__next_primeEm(i64) #1

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE8__rehashEm(%"class.std::__1::__hash_table"*, i64) #0 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.41"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair.40"*, align 8
  %5 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %8 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %9 = alloca i64, align 8
  %10 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.45"*, align 8
  %11 = alloca %"class.std::__1::__compressed_pair.44"*, align 8
  %12 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %13 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %14 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %15 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %16 = alloca i64, align 8
  %17 = alloca i64, align 8
  %18 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %19 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %20 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %21 = alloca i64, align 8
  %22 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %23 = alloca i64, align 8
  %24 = alloca i64, align 8
  %25 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %26 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %27 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %28 = alloca i64, align 8
  %29 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %30 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %31 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %32 = alloca i64, align 8
  %33 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.51"*, align 8
  %34 = alloca %"class.std::__1::__compressed_pair.50"*, align 8
  %35 = alloca %"class.std::__1::__hash_table"*, align 8
  %36 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %37 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %38 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %39 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %40 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %41 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %42 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %43 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %44 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %45 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %46 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %47 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %48 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %49 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.41"*, align 8
  %50 = alloca %"class.std::__1::__compressed_pair.40"*, align 8
  %51 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %52 = alloca i8*, align 8
  %53 = alloca %"class.std::__1::allocator.42"*, align 8
  %54 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %55 = alloca i64, align 8
  %56 = alloca %"class.std::__1::allocator.42"*, align 8
  %57 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %58 = alloca i64, align 8
  %59 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.41"*, align 8
  %60 = alloca %"class.std::__1::__compressed_pair.40"*, align 8
  %61 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %62 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %63 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %64 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %65 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %66 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %67 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %68 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %69 = alloca i64, align 8
  %70 = alloca %"class.std::__1::allocator.42"*, align 8
  %71 = alloca i64, align 8
  %72 = alloca i8*, align 8
  %73 = alloca %"class.std::__1::allocator.42"*, align 8
  %74 = alloca i64, align 8
  %75 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.41"*, align 8
  %76 = alloca %"class.std::__1::__compressed_pair.40"*, align 8
  %77 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %78 = alloca i8*, align 8
  %79 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %80 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %81 = alloca %"class.std::__1::basic_string"*, align 8
  %82 = alloca i8*, align 8
  %83 = alloca i8*, align 8
  %84 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %85 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %86 = alloca %"class.std::__1::basic_string"*, align 8
  %87 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %88 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %89 = alloca %"class.std::__1::basic_string"*, align 8
  %90 = alloca %"class.std::__1::basic_string"*, align 8
  %91 = alloca %"class.std::__1::basic_string"*, align 8
  %92 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %93 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %94 = alloca %"class.std::__1::basic_string"*, align 8
  %95 = alloca i8*, align 8
  %96 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %97 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %98 = alloca %"class.std::__1::basic_string"*, align 8
  %99 = alloca i8*, align 8
  %100 = alloca i8*, align 8
  %101 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %102 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %103 = alloca %"class.std::__1::basic_string"*, align 8
  %104 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %105 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %106 = alloca %"class.std::__1::basic_string"*, align 8
  %107 = alloca %"class.std::__1::basic_string"*, align 8
  %108 = alloca %"class.std::__1::basic_string"*, align 8
  %109 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %110 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %111 = alloca %"class.std::__1::basic_string"*, align 8
  %112 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %113 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %114 = alloca %"class.std::__1::basic_string"*, align 8
  %115 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %116 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %117 = alloca %"class.std::__1::basic_string"*, align 8
  %118 = alloca %"class.std::__1::basic_string"*, align 8
  %119 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %120 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %121 = alloca %"class.std::__1::basic_string"*, align 8
  %122 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %123 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %124 = alloca %"class.std::__1::basic_string"*, align 8
  %125 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.19"*, align 8
  %126 = alloca %"class.std::__1::__compressed_pair.18"*, align 8
  %127 = alloca %"class.std::__1::basic_string"*, align 8
  %128 = alloca %"class.std::__1::basic_string"*, align 8
  %129 = alloca i1, align 1
  %130 = alloca %"class.std::__1::basic_string"*, align 8
  %131 = alloca %"class.std::__1::basic_string"*, align 8
  %132 = alloca i64, align 8
  %133 = alloca i8*, align 8
  %134 = alloca i8*, align 8
  %135 = alloca %"struct.std::__1::equal_to"*, align 8
  %136 = alloca %"class.std::__1::basic_string"*, align 8
  %137 = alloca %"class.std::__1::basic_string"*, align 8
  %138 = alloca %"class.std::__1::__unordered_map_equal"*, align 8
  %139 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %140 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %141 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %142 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %143 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %144 = alloca i64, align 8
  %145 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %146 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %147 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %148 = alloca i64, align 8
  %149 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.39"*, align 8
  %150 = alloca %"class.std::__1::__compressed_pair.38"*, align 8
  %151 = alloca %"class.std::__1::unique_ptr.37"*, align 8
  %152 = alloca %"class.std::__1::__hash_table"*, align 8
  %153 = alloca i64, align 8
  %154 = alloca %"class.std::__1::allocator.42"*, align 8
  %155 = alloca i64, align 8
  %156 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %157 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %158 = alloca i64, align 8
  %159 = alloca i64, align 8
  %160 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %152, align 8
  store i64 %1, i64* %153, align 8
  %161 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %152, align 8
  %162 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %161, i32 0, i32 0
  store %"class.std::__1::unique_ptr.37"* %162, %"class.std::__1::unique_ptr.37"** %151, align 8
  %163 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %151, align 8
  %164 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %163, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %164, %"class.std::__1::__compressed_pair.38"** %150, align 8
  %165 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %150, align 8
  %166 = bitcast %"class.std::__1::__compressed_pair.38"* %165 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %166, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %149, align 8
  %167 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %149, align 8
  %168 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %167, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator"* %168, %"class.std::__1::__bucket_list_deallocator"** %77, align 8
  %169 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %77, align 8
  %170 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %169, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.40"* %170, %"class.std::__1::__compressed_pair.40"** %76, align 8
  %171 = load %"class.std::__1::__compressed_pair.40"*, %"class.std::__1::__compressed_pair.40"** %76, align 8
  %172 = bitcast %"class.std::__1::__compressed_pair.40"* %171 to %"class.std::__1::__libcpp_compressed_pair_imp.41"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.41"* %172, %"class.std::__1::__libcpp_compressed_pair_imp.41"** %75, align 8
  %173 = load %"class.std::__1::__libcpp_compressed_pair_imp.41"*, %"class.std::__1::__libcpp_compressed_pair_imp.41"** %75, align 8
  %174 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.41"* %173 to %"class.std::__1::allocator.42"*
  store %"class.std::__1::allocator.42"* %174, %"class.std::__1::allocator.42"** %154, align 8
  %175 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %161, i32 0, i32 0
  %176 = load i64, i64* %153, align 8
  %177 = icmp ugt i64 %176, 0
  br i1 %177, label %178, label %189

; <label>:178:                                    ; preds = %2
  %179 = load %"class.std::__1::allocator.42"*, %"class.std::__1::allocator.42"** %154, align 8
  %180 = load i64, i64* %153, align 8
  store %"class.std::__1::allocator.42"* %179, %"class.std::__1::allocator.42"** %73, align 8
  store i64 %180, i64* %74, align 8
  %181 = load %"class.std::__1::allocator.42"*, %"class.std::__1::allocator.42"** %73, align 8
  %182 = load i64, i64* %74, align 8
  store %"class.std::__1::allocator.42"* %181, %"class.std::__1::allocator.42"** %70, align 8
  store i64 %182, i64* %71, align 8
  store i8* null, i8** %72, align 8
  %183 = load %"class.std::__1::allocator.42"*, %"class.std::__1::allocator.42"** %70, align 8
  %184 = load i64, i64* %71, align 8
  %185 = mul i64 %184, 8
  store i64 %185, i64* %69, align 8
  %186 = load i64, i64* %69, align 8
  %187 = call i8* @_Znwm(i64 %186) #19
  %188 = bitcast i8* %187 to %"struct.std::__1::__hash_node_base"**
  br label %190

; <label>:189:                                    ; preds = %2
  br label %190

; <label>:190:                                    ; preds = %189, %178
  %191 = phi %"struct.std::__1::__hash_node_base"** [ %188, %178 ], [ null, %189 ]
  store %"class.std::__1::unique_ptr.37"* %175, %"class.std::__1::unique_ptr.37"** %66, align 8
  store %"struct.std::__1::__hash_node_base"** %191, %"struct.std::__1::__hash_node_base"*** %67, align 8
  %192 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %66, align 8
  %193 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %192, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %193, %"class.std::__1::__compressed_pair.38"** %65, align 8
  %194 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %65, align 8
  %195 = bitcast %"class.std::__1::__compressed_pair.38"* %194 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %195, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %64, align 8
  %196 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %64, align 8
  %197 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %196, i32 0, i32 0
  %198 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %197, align 8
  store %"struct.std::__1::__hash_node_base"** %198, %"struct.std::__1::__hash_node_base"*** %68, align 8
  %199 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %67, align 8
  %200 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %192, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %200, %"class.std::__1::__compressed_pair.38"** %48, align 8
  %201 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %48, align 8
  %202 = bitcast %"class.std::__1::__compressed_pair.38"* %201 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %202, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %47, align 8
  %203 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %47, align 8
  %204 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %203, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"** %199, %"struct.std::__1::__hash_node_base"*** %204, align 8
  %205 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %68, align 8
  %206 = icmp ne %"struct.std::__1::__hash_node_base"** %205, null
  br i1 %206, label %207, label %236

; <label>:207:                                    ; preds = %190
  %208 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %192, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %208, %"class.std::__1::__compressed_pair.38"** %46, align 8
  %209 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %46, align 8
  %210 = bitcast %"class.std::__1::__compressed_pair.38"* %209 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %210, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %45, align 8
  %211 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %45, align 8
  %212 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %211, i32 0, i32 1
  %213 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %68, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %212, %"class.std::__1::__bucket_list_deallocator"** %62, align 8
  store %"struct.std::__1::__hash_node_base"** %213, %"struct.std::__1::__hash_node_base"*** %63, align 8
  %214 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %62, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %214, %"class.std::__1::__bucket_list_deallocator"** %61, align 8
  %215 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %61, align 8
  %216 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %215, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.40"* %216, %"class.std::__1::__compressed_pair.40"** %60, align 8
  %217 = load %"class.std::__1::__compressed_pair.40"*, %"class.std::__1::__compressed_pair.40"** %60, align 8
  %218 = bitcast %"class.std::__1::__compressed_pair.40"* %217 to %"class.std::__1::__libcpp_compressed_pair_imp.41"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.41"* %218, %"class.std::__1::__libcpp_compressed_pair_imp.41"** %59, align 8
  %219 = load %"class.std::__1::__libcpp_compressed_pair_imp.41"*, %"class.std::__1::__libcpp_compressed_pair_imp.41"** %59, align 8
  %220 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.41"* %219 to %"class.std::__1::allocator.42"*
  %221 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %63, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %214, %"class.std::__1::__bucket_list_deallocator"** %51, align 8
  %222 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %51, align 8
  %223 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %222, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.40"* %223, %"class.std::__1::__compressed_pair.40"** %50, align 8
  %224 = load %"class.std::__1::__compressed_pair.40"*, %"class.std::__1::__compressed_pair.40"** %50, align 8
  %225 = bitcast %"class.std::__1::__compressed_pair.40"* %224 to %"class.std::__1::__libcpp_compressed_pair_imp.41"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.41"* %225, %"class.std::__1::__libcpp_compressed_pair_imp.41"** %49, align 8
  %226 = load %"class.std::__1::__libcpp_compressed_pair_imp.41"*, %"class.std::__1::__libcpp_compressed_pair_imp.41"** %49, align 8
  %227 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.41", %"class.std::__1::__libcpp_compressed_pair_imp.41"* %226, i32 0, i32 0
  %228 = load i64, i64* %227, align 8
  store %"class.std::__1::allocator.42"* %220, %"class.std::__1::allocator.42"** %56, align 8
  store %"struct.std::__1::__hash_node_base"** %221, %"struct.std::__1::__hash_node_base"*** %57, align 8
  store i64 %228, i64* %58, align 8
  %229 = load %"class.std::__1::allocator.42"*, %"class.std::__1::allocator.42"** %56, align 8
  %230 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %57, align 8
  %231 = load i64, i64* %58, align 8
  store %"class.std::__1::allocator.42"* %229, %"class.std::__1::allocator.42"** %53, align 8
  store %"struct.std::__1::__hash_node_base"** %230, %"struct.std::__1::__hash_node_base"*** %54, align 8
  store i64 %231, i64* %55, align 8
  %232 = load %"class.std::__1::allocator.42"*, %"class.std::__1::allocator.42"** %53, align 8
  %233 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %54, align 8
  %234 = bitcast %"struct.std::__1::__hash_node_base"** %233 to i8*
  store i8* %234, i8** %52, align 8
  %235 = load i8*, i8** %52, align 8
  call void @_ZdlPv(i8* %235) #17
  br label %236

; <label>:236:                                    ; preds = %190, %207
  %237 = load i64, i64* %153, align 8
  %238 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %161, i32 0, i32 0
  store %"class.std::__1::unique_ptr.37"* %238, %"class.std::__1::unique_ptr.37"** %44, align 8
  %239 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %44, align 8
  %240 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %239, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %240, %"class.std::__1::__compressed_pair.38"** %43, align 8
  %241 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %43, align 8
  %242 = bitcast %"class.std::__1::__compressed_pair.38"* %241 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %242, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %42, align 8
  %243 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %42, align 8
  %244 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %243, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator"* %244, %"class.std::__1::__bucket_list_deallocator"** %5, align 8
  %245 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %5, align 8
  %246 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %245, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.40"* %246, %"class.std::__1::__compressed_pair.40"** %4, align 8
  %247 = load %"class.std::__1::__compressed_pair.40"*, %"class.std::__1::__compressed_pair.40"** %4, align 8
  %248 = bitcast %"class.std::__1::__compressed_pair.40"* %247 to %"class.std::__1::__libcpp_compressed_pair_imp.41"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.41"* %248, %"class.std::__1::__libcpp_compressed_pair_imp.41"** %3, align 8
  %249 = load %"class.std::__1::__libcpp_compressed_pair_imp.41"*, %"class.std::__1::__libcpp_compressed_pair_imp.41"** %3, align 8
  %250 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.41", %"class.std::__1::__libcpp_compressed_pair_imp.41"* %249, i32 0, i32 0
  store i64 %237, i64* %250, align 8
  %251 = load i64, i64* %153, align 8
  %252 = icmp ugt i64 %251, 0
  br i1 %252, label %253, label %724

; <label>:253:                                    ; preds = %236
  store i64 0, i64* %155, align 8
  br label %254

; <label>:254:                                    ; preds = %270, %253
  %255 = load i64, i64* %155, align 8
  %256 = load i64, i64* %153, align 8
  %257 = icmp ult i64 %255, %256
  br i1 %257, label %258, label %273

; <label>:258:                                    ; preds = %254
  %259 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %161, i32 0, i32 0
  %260 = load i64, i64* %155, align 8
  store %"class.std::__1::unique_ptr.37"* %259, %"class.std::__1::unique_ptr.37"** %8, align 8
  store i64 %260, i64* %9, align 8
  %261 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %8, align 8
  %262 = load i64, i64* %9, align 8
  %263 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %261, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %263, %"class.std::__1::__compressed_pair.38"** %7, align 8
  %264 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %7, align 8
  %265 = bitcast %"class.std::__1::__compressed_pair.38"* %264 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %265, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %6, align 8
  %266 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %6, align 8
  %267 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %266, i32 0, i32 0
  %268 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %267, align 8
  %269 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %268, i64 %262
  store %"struct.std::__1::__hash_node_base"* null, %"struct.std::__1::__hash_node_base"** %269, align 8
  br label %270

; <label>:270:                                    ; preds = %258
  %271 = load i64, i64* %155, align 8
  %272 = add i64 %271, 1
  store i64 %272, i64* %155, align 8
  br label %254

; <label>:273:                                    ; preds = %254
  %274 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %161, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.44"* %274, %"class.std::__1::__compressed_pair.44"** %11, align 8
  %275 = load %"class.std::__1::__compressed_pair.44"*, %"class.std::__1::__compressed_pair.44"** %11, align 8
  %276 = bitcast %"class.std::__1::__compressed_pair.44"* %275 to %"class.std::__1::__libcpp_compressed_pair_imp.45"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.45"* %276, %"class.std::__1::__libcpp_compressed_pair_imp.45"** %10, align 8
  %277 = load %"class.std::__1::__libcpp_compressed_pair_imp.45"*, %"class.std::__1::__libcpp_compressed_pair_imp.45"** %10, align 8
  %278 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.45", %"class.std::__1::__libcpp_compressed_pair_imp.45"* %277, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %278, %"struct.std::__1::__hash_node_base"** %14, align 8
  %279 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %14, align 8
  store %"struct.std::__1::__hash_node_base"* %279, %"struct.std::__1::__hash_node_base"** %13, align 8
  %280 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %13, align 8
  store %"struct.std::__1::__hash_node_base"* %280, %"struct.std::__1::__hash_node_base"** %12, align 8
  %281 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %12, align 8
  %282 = bitcast %"struct.std::__1::__hash_node_base"* %281 to i8*
  store %"struct.std::__1::__hash_node_base"* %281, %"struct.std::__1::__hash_node_base"** %156, align 8
  %283 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %156, align 8
  %284 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %283, i32 0, i32 0
  %285 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %284, align 8
  store %"struct.std::__1::__hash_node_base"* %285, %"struct.std::__1::__hash_node_base"** %157, align 8
  %286 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %157, align 8
  %287 = icmp ne %"struct.std::__1::__hash_node_base"* %286, null
  br i1 %287, label %288, label %723

; <label>:288:                                    ; preds = %273
  %289 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %157, align 8
  store %"struct.std::__1::__hash_node_base"* %289, %"struct.std::__1::__hash_node_base"** %15, align 8
  %290 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %15, align 8
  %291 = bitcast %"struct.std::__1::__hash_node_base"* %290 to %"struct.std::__1::__hash_node"*
  %292 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %291, i32 0, i32 1
  %293 = load i64, i64* %292, align 8
  %294 = load i64, i64* %153, align 8
  store i64 %293, i64* %16, align 8
  store i64 %294, i64* %17, align 8
  %295 = load i64, i64* %17, align 8
  %296 = load i64, i64* %17, align 8
  %297 = sub i64 %296, 1
  %298 = and i64 %295, %297
  %299 = icmp ne i64 %298, 0
  br i1 %299, label %305, label %300

; <label>:300:                                    ; preds = %288
  %301 = load i64, i64* %16, align 8
  %302 = load i64, i64* %17, align 8
  %303 = sub i64 %302, 1
  %304 = and i64 %301, %303
  br label %317

; <label>:305:                                    ; preds = %288
  %306 = load i64, i64* %16, align 8
  %307 = load i64, i64* %17, align 8
  %308 = icmp ult i64 %306, %307
  br i1 %308, label %309, label %311

; <label>:309:                                    ; preds = %305
  %310 = load i64, i64* %16, align 8
  br label %315

; <label>:311:                                    ; preds = %305
  %312 = load i64, i64* %16, align 8
  %313 = load i64, i64* %17, align 8
  %314 = urem i64 %312, %313
  br label %315

; <label>:315:                                    ; preds = %311, %309
  %316 = phi i64 [ %310, %309 ], [ %314, %311 ]
  br label %317

; <label>:317:                                    ; preds = %300, %315
  %318 = phi i64 [ %304, %300 ], [ %316, %315 ]
  store i64 %318, i64* %158, align 8
  %319 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %156, align 8
  %320 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %161, i32 0, i32 0
  %321 = load i64, i64* %158, align 8
  store %"class.std::__1::unique_ptr.37"* %320, %"class.std::__1::unique_ptr.37"** %20, align 8
  store i64 %321, i64* %21, align 8
  %322 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %20, align 8
  %323 = load i64, i64* %21, align 8
  %324 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %322, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %324, %"class.std::__1::__compressed_pair.38"** %19, align 8
  %325 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %19, align 8
  %326 = bitcast %"class.std::__1::__compressed_pair.38"* %325 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %326, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %18, align 8
  %327 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %18, align 8
  %328 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %327, i32 0, i32 0
  %329 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %328, align 8
  %330 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %329, i64 %323
  store %"struct.std::__1::__hash_node_base"* %319, %"struct.std::__1::__hash_node_base"** %330, align 8
  %331 = load i64, i64* %158, align 8
  store i64 %331, i64* %159, align 8
  %332 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %157, align 8
  store %"struct.std::__1::__hash_node_base"* %332, %"struct.std::__1::__hash_node_base"** %156, align 8
  %333 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %157, align 8
  %334 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %333, i32 0, i32 0
  %335 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %334, align 8
  store %"struct.std::__1::__hash_node_base"* %335, %"struct.std::__1::__hash_node_base"** %157, align 8
  br label %336

; <label>:336:                                    ; preds = %718, %317
  %337 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %157, align 8
  %338 = icmp ne %"struct.std::__1::__hash_node_base"* %337, null
  br i1 %338, label %339, label %722

; <label>:339:                                    ; preds = %336
  %340 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %157, align 8
  store %"struct.std::__1::__hash_node_base"* %340, %"struct.std::__1::__hash_node_base"** %22, align 8
  %341 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %22, align 8
  %342 = bitcast %"struct.std::__1::__hash_node_base"* %341 to %"struct.std::__1::__hash_node"*
  %343 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %342, i32 0, i32 1
  %344 = load i64, i64* %343, align 8
  %345 = load i64, i64* %153, align 8
  store i64 %344, i64* %23, align 8
  store i64 %345, i64* %24, align 8
  %346 = load i64, i64* %24, align 8
  %347 = load i64, i64* %24, align 8
  %348 = sub i64 %347, 1
  %349 = and i64 %346, %348
  %350 = icmp ne i64 %349, 0
  br i1 %350, label %356, label %351

; <label>:351:                                    ; preds = %339
  %352 = load i64, i64* %23, align 8
  %353 = load i64, i64* %24, align 8
  %354 = sub i64 %353, 1
  %355 = and i64 %352, %354
  br label %368

; <label>:356:                                    ; preds = %339
  %357 = load i64, i64* %23, align 8
  %358 = load i64, i64* %24, align 8
  %359 = icmp ult i64 %357, %358
  br i1 %359, label %360, label %362

; <label>:360:                                    ; preds = %356
  %361 = load i64, i64* %23, align 8
  br label %366

; <label>:362:                                    ; preds = %356
  %363 = load i64, i64* %23, align 8
  %364 = load i64, i64* %24, align 8
  %365 = urem i64 %363, %364
  br label %366

; <label>:366:                                    ; preds = %362, %360
  %367 = phi i64 [ %361, %360 ], [ %365, %362 ]
  br label %368

; <label>:368:                                    ; preds = %351, %366
  %369 = phi i64 [ %355, %351 ], [ %367, %366 ]
  store i64 %369, i64* %158, align 8
  %370 = load i64, i64* %158, align 8
  %371 = load i64, i64* %159, align 8
  %372 = icmp eq i64 %370, %371
  br i1 %372, label %373, label %375

; <label>:373:                                    ; preds = %368
  %374 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %157, align 8
  store %"struct.std::__1::__hash_node_base"* %374, %"struct.std::__1::__hash_node_base"** %156, align 8
  br label %717

; <label>:375:                                    ; preds = %368
  %376 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %161, i32 0, i32 0
  %377 = load i64, i64* %158, align 8
  store %"class.std::__1::unique_ptr.37"* %376, %"class.std::__1::unique_ptr.37"** %27, align 8
  store i64 %377, i64* %28, align 8
  %378 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %27, align 8
  %379 = load i64, i64* %28, align 8
  %380 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %378, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %380, %"class.std::__1::__compressed_pair.38"** %26, align 8
  %381 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %26, align 8
  %382 = bitcast %"class.std::__1::__compressed_pair.38"* %381 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %382, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %25, align 8
  %383 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %25, align 8
  %384 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %383, i32 0, i32 0
  %385 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %384, align 8
  %386 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %385, i64 %379
  %387 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %386, align 8
  %388 = icmp eq %"struct.std::__1::__hash_node_base"* %387, null
  br i1 %388, label %389, label %404

; <label>:389:                                    ; preds = %375
  %390 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %156, align 8
  %391 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %161, i32 0, i32 0
  %392 = load i64, i64* %158, align 8
  store %"class.std::__1::unique_ptr.37"* %391, %"class.std::__1::unique_ptr.37"** %31, align 8
  store i64 %392, i64* %32, align 8
  %393 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %31, align 8
  %394 = load i64, i64* %32, align 8
  %395 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %393, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %395, %"class.std::__1::__compressed_pair.38"** %30, align 8
  %396 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %30, align 8
  %397 = bitcast %"class.std::__1::__compressed_pair.38"* %396 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %397, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %29, align 8
  %398 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %29, align 8
  %399 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %398, i32 0, i32 0
  %400 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %399, align 8
  %401 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %400, i64 %394
  store %"struct.std::__1::__hash_node_base"* %390, %"struct.std::__1::__hash_node_base"** %401, align 8
  %402 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %157, align 8
  store %"struct.std::__1::__hash_node_base"* %402, %"struct.std::__1::__hash_node_base"** %156, align 8
  %403 = load i64, i64* %158, align 8
  store i64 %403, i64* %159, align 8
  br label %716

; <label>:404:                                    ; preds = %375
  %405 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %157, align 8
  store %"struct.std::__1::__hash_node_base"* %405, %"struct.std::__1::__hash_node_base"** %160, align 8
  br label %406

; <label>:406:                                    ; preds = %676, %404
  %407 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %160, align 8
  %408 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %407, i32 0, i32 0
  %409 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %408, align 8
  %410 = icmp ne %"struct.std::__1::__hash_node_base"* %409, null
  br i1 %410, label %411, label %673

; <label>:411:                                    ; preds = %406
  store %"class.std::__1::__hash_table"* %161, %"class.std::__1::__hash_table"** %35, align 8
  %412 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %35, align 8
  %413 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %412, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.50"* %413, %"class.std::__1::__compressed_pair.50"** %34, align 8
  %414 = load %"class.std::__1::__compressed_pair.50"*, %"class.std::__1::__compressed_pair.50"** %34, align 8
  %415 = bitcast %"class.std::__1::__compressed_pair.50"* %414 to %"class.std::__1::__libcpp_compressed_pair_imp.51"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.51"* %415, %"class.std::__1::__libcpp_compressed_pair_imp.51"** %33, align 8
  %416 = load %"class.std::__1::__libcpp_compressed_pair_imp.51"*, %"class.std::__1::__libcpp_compressed_pair_imp.51"** %33, align 8
  %417 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.51"* %416 to %"class.std::__1::__unordered_map_equal"*
  %418 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %157, align 8
  store %"struct.std::__1::__hash_node_base"* %418, %"struct.std::__1::__hash_node_base"** %38, align 8
  %419 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %38, align 8
  store %"struct.std::__1::__hash_node_base"* %419, %"struct.std::__1::__hash_node_base"** %37, align 8
  %420 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %37, align 8
  store %"struct.std::__1::__hash_node_base"* %420, %"struct.std::__1::__hash_node_base"** %36, align 8
  %421 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %36, align 8
  %422 = bitcast %"struct.std::__1::__hash_node_base"* %421 to i8*
  %423 = bitcast %"struct.std::__1::__hash_node_base"* %421 to %"struct.std::__1::__hash_node"*
  %424 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %423, i32 0, i32 2
  %425 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %160, align 8
  %426 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %425, i32 0, i32 0
  %427 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %426, align 8
  store %"struct.std::__1::__hash_node_base"* %427, %"struct.std::__1::__hash_node_base"** %41, align 8
  %428 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %41, align 8
  store %"struct.std::__1::__hash_node_base"* %428, %"struct.std::__1::__hash_node_base"** %40, align 8
  %429 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %40, align 8
  store %"struct.std::__1::__hash_node_base"* %429, %"struct.std::__1::__hash_node_base"** %39, align 8
  %430 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %39, align 8
  %431 = bitcast %"struct.std::__1::__hash_node_base"* %430 to i8*
  %432 = bitcast %"struct.std::__1::__hash_node_base"* %430 to %"struct.std::__1::__hash_node"*
  %433 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %432, i32 0, i32 2
  store %"class.std::__1::__unordered_map_equal"* %417, %"class.std::__1::__unordered_map_equal"** %138, align 8
  store %"union.std::__1::__hash_value_type"* %424, %"union.std::__1::__hash_value_type"** %139, align 8
  store %"union.std::__1::__hash_value_type"* %433, %"union.std::__1::__hash_value_type"** %140, align 8
  %434 = load %"class.std::__1::__unordered_map_equal"*, %"class.std::__1::__unordered_map_equal"** %138, align 8
  %435 = bitcast %"class.std::__1::__unordered_map_equal"* %434 to %"struct.std::__1::equal_to"*
  %436 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %139, align 8
  %437 = bitcast %"union.std::__1::__hash_value_type"* %436 to %"struct.std::__1::pair.92"*
  %438 = getelementptr inbounds %"struct.std::__1::pair.92", %"struct.std::__1::pair.92"* %437, i32 0, i32 0
  %439 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %140, align 8
  %440 = bitcast %"union.std::__1::__hash_value_type"* %439 to %"struct.std::__1::pair.92"*
  %441 = getelementptr inbounds %"struct.std::__1::pair.92", %"struct.std::__1::pair.92"* %440, i32 0, i32 0
  store %"struct.std::__1::equal_to"* %435, %"struct.std::__1::equal_to"** %135, align 8
  store %"class.std::__1::basic_string"* %438, %"class.std::__1::basic_string"** %136, align 8
  store %"class.std::__1::basic_string"* %441, %"class.std::__1::basic_string"** %137, align 8
  %442 = load %"struct.std::__1::equal_to"*, %"struct.std::__1::equal_to"** %135, align 8
  %443 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %136, align 8
  %444 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %137, align 8
  store %"class.std::__1::basic_string"* %443, %"class.std::__1::basic_string"** %130, align 8
  store %"class.std::__1::basic_string"* %444, %"class.std::__1::basic_string"** %131, align 8
  %445 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %130, align 8
  store %"class.std::__1::basic_string"* %445, %"class.std::__1::basic_string"** %128, align 8
  %446 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %128, align 8
  store %"class.std::__1::basic_string"* %446, %"class.std::__1::basic_string"** %127, align 8
  %447 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %127, align 8
  %448 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %447, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %448, %"class.std::__1::__compressed_pair.18"** %126, align 8
  %449 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %126, align 8
  %450 = bitcast %"class.std::__1::__compressed_pair.18"* %449 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %450, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %125, align 8
  %451 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %125, align 8
  %452 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %451, i32 0, i32 0
  %453 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %452, i32 0, i32 0
  %454 = bitcast %union.anon* %453 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %455 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %454, i32 0, i32 0
  %456 = bitcast %union.anon.20* %455 to i8*
  %457 = load i8, i8* %456, align 8
  %458 = zext i8 %457 to i32
  %459 = and i32 %458, 1
  %460 = icmp ne i32 %459, 0
  br i1 %460, label %461, label %472

; <label>:461:                                    ; preds = %411
  store %"class.std::__1::basic_string"* %446, %"class.std::__1::basic_string"** %121, align 8
  %462 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %121, align 8
  %463 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %462, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %463, %"class.std::__1::__compressed_pair.18"** %120, align 8
  %464 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %120, align 8
  %465 = bitcast %"class.std::__1::__compressed_pair.18"* %464 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %465, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %119, align 8
  %466 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %119, align 8
  %467 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %466, i32 0, i32 0
  %468 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %467, i32 0, i32 0
  %469 = bitcast %union.anon* %468 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %470 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %469, i32 0, i32 1
  %471 = load i64, i64* %470, align 8
  br label %487

; <label>:472:                                    ; preds = %411
  store %"class.std::__1::basic_string"* %446, %"class.std::__1::basic_string"** %124, align 8
  %473 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %124, align 8
  %474 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %473, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %474, %"class.std::__1::__compressed_pair.18"** %123, align 8
  %475 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %123, align 8
  %476 = bitcast %"class.std::__1::__compressed_pair.18"* %475 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %476, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %122, align 8
  %477 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %122, align 8
  %478 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %477, i32 0, i32 0
  %479 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %478, i32 0, i32 0
  %480 = bitcast %union.anon* %479 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %481 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %480, i32 0, i32 0
  %482 = bitcast %union.anon.20* %481 to i8*
  %483 = load i8, i8* %482, align 8
  %484 = zext i8 %483 to i32
  %485 = ashr i32 %484, 1
  %486 = sext i32 %485 to i64
  br label %487

; <label>:487:                                    ; preds = %472, %461
  %488 = phi i64 [ %471, %461 ], [ %486, %472 ]
  store i64 %488, i64* %132, align 8
  %489 = load i64, i64* %132, align 8
  %490 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %131, align 8
  store %"class.std::__1::basic_string"* %490, %"class.std::__1::basic_string"** %118, align 8
  %491 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %118, align 8
  store %"class.std::__1::basic_string"* %491, %"class.std::__1::basic_string"** %117, align 8
  %492 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %117, align 8
  %493 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %492, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %493, %"class.std::__1::__compressed_pair.18"** %116, align 8
  %494 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %116, align 8
  %495 = bitcast %"class.std::__1::__compressed_pair.18"* %494 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %495, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %115, align 8
  %496 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %115, align 8
  %497 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %496, i32 0, i32 0
  %498 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %497, i32 0, i32 0
  %499 = bitcast %union.anon* %498 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %500 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %499, i32 0, i32 0
  %501 = bitcast %union.anon.20* %500 to i8*
  %502 = load i8, i8* %501, align 8
  %503 = zext i8 %502 to i32
  %504 = and i32 %503, 1
  %505 = icmp ne i32 %504, 0
  br i1 %505, label %506, label %517

; <label>:506:                                    ; preds = %487
  store %"class.std::__1::basic_string"* %491, %"class.std::__1::basic_string"** %111, align 8
  %507 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %111, align 8
  %508 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %507, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %508, %"class.std::__1::__compressed_pair.18"** %110, align 8
  %509 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %110, align 8
  %510 = bitcast %"class.std::__1::__compressed_pair.18"* %509 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %510, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %109, align 8
  %511 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %109, align 8
  %512 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %511, i32 0, i32 0
  %513 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %512, i32 0, i32 0
  %514 = bitcast %union.anon* %513 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %515 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %514, i32 0, i32 1
  %516 = load i64, i64* %515, align 8
  br label %532

; <label>:517:                                    ; preds = %487
  store %"class.std::__1::basic_string"* %491, %"class.std::__1::basic_string"** %114, align 8
  %518 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %114, align 8
  %519 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %518, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %519, %"class.std::__1::__compressed_pair.18"** %113, align 8
  %520 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %113, align 8
  %521 = bitcast %"class.std::__1::__compressed_pair.18"* %520 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %521, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %112, align 8
  %522 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %112, align 8
  %523 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %522, i32 0, i32 0
  %524 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %523, i32 0, i32 0
  %525 = bitcast %union.anon* %524 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %526 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %525, i32 0, i32 0
  %527 = bitcast %union.anon.20* %526 to i8*
  %528 = load i8, i8* %527, align 8
  %529 = zext i8 %528 to i32
  %530 = ashr i32 %529, 1
  %531 = sext i32 %530 to i64
  br label %532

; <label>:532:                                    ; preds = %517, %506
  %533 = phi i64 [ %516, %506 ], [ %531, %517 ]
  %534 = icmp ne i64 %489, %533
  br i1 %534, label %535, label %536

; <label>:535:                                    ; preds = %532
  store i1 false, i1* %129, align 1
  br label %671

; <label>:536:                                    ; preds = %532
  %537 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %130, align 8
  store %"class.std::__1::basic_string"* %537, %"class.std::__1::basic_string"** %108, align 8
  %538 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %108, align 8
  store %"class.std::__1::basic_string"* %538, %"class.std::__1::basic_string"** %107, align 8
  %539 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %107, align 8
  store %"class.std::__1::basic_string"* %539, %"class.std::__1::basic_string"** %106, align 8
  %540 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %106, align 8
  %541 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %540, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %541, %"class.std::__1::__compressed_pair.18"** %105, align 8
  %542 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %105, align 8
  %543 = bitcast %"class.std::__1::__compressed_pair.18"* %542 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %543, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %104, align 8
  %544 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %104, align 8
  %545 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %544, i32 0, i32 0
  %546 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %545, i32 0, i32 0
  %547 = bitcast %union.anon* %546 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %548 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %547, i32 0, i32 0
  %549 = bitcast %union.anon.20* %548 to i8*
  %550 = load i8, i8* %549, align 8
  %551 = zext i8 %550 to i32
  %552 = and i32 %551, 1
  %553 = icmp ne i32 %552, 0
  br i1 %553, label %554, label %565

; <label>:554:                                    ; preds = %536
  store %"class.std::__1::basic_string"* %539, %"class.std::__1::basic_string"** %98, align 8
  %555 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %98, align 8
  %556 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %555, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %556, %"class.std::__1::__compressed_pair.18"** %97, align 8
  %557 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %97, align 8
  %558 = bitcast %"class.std::__1::__compressed_pair.18"* %557 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %558, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %96, align 8
  %559 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %96, align 8
  %560 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %559, i32 0, i32 0
  %561 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %560, i32 0, i32 0
  %562 = bitcast %union.anon* %561 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %563 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %562, i32 0, i32 2
  %564 = load i8*, i8** %563, align 8
  br label %578

; <label>:565:                                    ; preds = %536
  store %"class.std::__1::basic_string"* %539, %"class.std::__1::basic_string"** %103, align 8
  %566 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %103, align 8
  %567 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %566, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %567, %"class.std::__1::__compressed_pair.18"** %102, align 8
  %568 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %102, align 8
  %569 = bitcast %"class.std::__1::__compressed_pair.18"* %568 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %569, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %101, align 8
  %570 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %101, align 8
  %571 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %570, i32 0, i32 0
  %572 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %571, i32 0, i32 0
  %573 = bitcast %union.anon* %572 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %574 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %573, i32 0, i32 1
  %575 = getelementptr inbounds [23 x i8], [23 x i8]* %574, i64 0, i64 0
  store i8* %575, i8** %100, align 8
  %576 = load i8*, i8** %100, align 8
  store i8* %576, i8** %99, align 8
  %577 = load i8*, i8** %99, align 8
  br label %578

; <label>:578:                                    ; preds = %565, %554
  %579 = phi i8* [ %564, %554 ], [ %577, %565 ]
  store i8* %579, i8** %95, align 8
  %580 = load i8*, i8** %95, align 8
  store i8* %580, i8** %133, align 8
  %581 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %131, align 8
  store %"class.std::__1::basic_string"* %581, %"class.std::__1::basic_string"** %91, align 8
  %582 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %91, align 8
  store %"class.std::__1::basic_string"* %582, %"class.std::__1::basic_string"** %90, align 8
  %583 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %90, align 8
  store %"class.std::__1::basic_string"* %583, %"class.std::__1::basic_string"** %89, align 8
  %584 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %89, align 8
  %585 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %584, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %585, %"class.std::__1::__compressed_pair.18"** %88, align 8
  %586 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %88, align 8
  %587 = bitcast %"class.std::__1::__compressed_pair.18"* %586 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %587, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %87, align 8
  %588 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %87, align 8
  %589 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %588, i32 0, i32 0
  %590 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %589, i32 0, i32 0
  %591 = bitcast %union.anon* %590 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %592 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %591, i32 0, i32 0
  %593 = bitcast %union.anon.20* %592 to i8*
  %594 = load i8, i8* %593, align 8
  %595 = zext i8 %594 to i32
  %596 = and i32 %595, 1
  %597 = icmp ne i32 %596, 0
  br i1 %597, label %598, label %609

; <label>:598:                                    ; preds = %578
  store %"class.std::__1::basic_string"* %583, %"class.std::__1::basic_string"** %81, align 8
  %599 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %81, align 8
  %600 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %599, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %600, %"class.std::__1::__compressed_pair.18"** %80, align 8
  %601 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %80, align 8
  %602 = bitcast %"class.std::__1::__compressed_pair.18"* %601 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %602, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %79, align 8
  %603 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %79, align 8
  %604 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %603, i32 0, i32 0
  %605 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %604, i32 0, i32 0
  %606 = bitcast %union.anon* %605 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %607 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %606, i32 0, i32 2
  %608 = load i8*, i8** %607, align 8
  br label %622

; <label>:609:                                    ; preds = %578
  store %"class.std::__1::basic_string"* %583, %"class.std::__1::basic_string"** %86, align 8
  %610 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %86, align 8
  %611 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %610, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %611, %"class.std::__1::__compressed_pair.18"** %85, align 8
  %612 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %85, align 8
  %613 = bitcast %"class.std::__1::__compressed_pair.18"* %612 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %613, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %84, align 8
  %614 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %84, align 8
  %615 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %614, i32 0, i32 0
  %616 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %615, i32 0, i32 0
  %617 = bitcast %union.anon* %616 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %618 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %617, i32 0, i32 1
  %619 = getelementptr inbounds [23 x i8], [23 x i8]* %618, i64 0, i64 0
  store i8* %619, i8** %83, align 8
  %620 = load i8*, i8** %83, align 8
  store i8* %620, i8** %82, align 8
  %621 = load i8*, i8** %82, align 8
  br label %622

; <label>:622:                                    ; preds = %609, %598
  %623 = phi i8* [ %608, %598 ], [ %621, %609 ]
  store i8* %623, i8** %78, align 8
  %624 = load i8*, i8** %78, align 8
  store i8* %624, i8** %134, align 8
  %625 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %130, align 8
  store %"class.std::__1::basic_string"* %625, %"class.std::__1::basic_string"** %94, align 8
  %626 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %94, align 8
  %627 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %626, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.18"* %627, %"class.std::__1::__compressed_pair.18"** %93, align 8
  %628 = load %"class.std::__1::__compressed_pair.18"*, %"class.std::__1::__compressed_pair.18"** %93, align 8
  %629 = bitcast %"class.std::__1::__compressed_pair.18"* %628 to %"class.std::__1::__libcpp_compressed_pair_imp.19"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.19"* %629, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %92, align 8
  %630 = load %"class.std::__1::__libcpp_compressed_pair_imp.19"*, %"class.std::__1::__libcpp_compressed_pair_imp.19"** %92, align 8
  %631 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.19", %"class.std::__1::__libcpp_compressed_pair_imp.19"* %630, i32 0, i32 0
  %632 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %631, i32 0, i32 0
  %633 = bitcast %union.anon* %632 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %634 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %633, i32 0, i32 0
  %635 = bitcast %union.anon.20* %634 to i8*
  %636 = load i8, i8* %635, align 8
  %637 = zext i8 %636 to i32
  %638 = and i32 %637, 1
  %639 = icmp ne i32 %638, 0
  br i1 %639, label %640, label %647

; <label>:640:                                    ; preds = %622
  %641 = load i8*, i8** %133, align 8
  %642 = load i8*, i8** %134, align 8
  %643 = load i64, i64* %132, align 8
  %644 = invoke i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8* %641, i8* %642, i64 %643)
          to label %645 unwind label %668

; <label>:645:                                    ; preds = %640
  %646 = icmp eq i32 %644, 0
  store i1 %646, i1* %129, align 1
  br label %671

; <label>:647:                                    ; preds = %622
  br label %648

; <label>:648:                                    ; preds = %660, %647
  %649 = load i64, i64* %132, align 8
  %650 = icmp ne i64 %649, 0
  br i1 %650, label %651, label %667

; <label>:651:                                    ; preds = %648
  %652 = load i8*, i8** %133, align 8
  %653 = load i8, i8* %652, align 1
  %654 = sext i8 %653 to i32
  %655 = load i8*, i8** %134, align 8
  %656 = load i8, i8* %655, align 1
  %657 = sext i8 %656 to i32
  %658 = icmp ne i32 %654, %657
  br i1 %658, label %659, label %660

; <label>:659:                                    ; preds = %651
  store i1 false, i1* %129, align 1
  br label %671

; <label>:660:                                    ; preds = %651
  %661 = load i64, i64* %132, align 8
  %662 = add i64 %661, -1
  store i64 %662, i64* %132, align 8
  %663 = load i8*, i8** %133, align 8
  %664 = getelementptr inbounds i8, i8* %663, i32 1
  store i8* %664, i8** %133, align 8
  %665 = load i8*, i8** %134, align 8
  %666 = getelementptr inbounds i8, i8* %665, i32 1
  store i8* %666, i8** %134, align 8
  br label %648

; <label>:667:                                    ; preds = %648
  store i1 true, i1* %129, align 1
  br label %671

; <label>:668:                                    ; preds = %640
  %669 = landingpad { i8*, i32 }
          catch i8* null
  %670 = extractvalue { i8*, i32 } %669, 0
  call void @__clang_call_terminate(i8* %670) #16
  unreachable

; <label>:671:                                    ; preds = %535, %645, %659, %667
  %672 = load i1, i1* %129, align 1
  br label %673

; <label>:673:                                    ; preds = %671, %406
  %674 = phi i1 [ false, %406 ], [ %672, %671 ]
  br i1 %674, label %675, label %680

; <label>:675:                                    ; preds = %673
  br label %676

; <label>:676:                                    ; preds = %675
  %677 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %160, align 8
  %678 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %677, i32 0, i32 0
  %679 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %678, align 8
  store %"struct.std::__1::__hash_node_base"* %679, %"struct.std::__1::__hash_node_base"** %160, align 8
  br label %406

; <label>:680:                                    ; preds = %673
  %681 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %160, align 8
  %682 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %681, i32 0, i32 0
  %683 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %682, align 8
  %684 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %156, align 8
  %685 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %684, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %683, %"struct.std::__1::__hash_node_base"** %685, align 8
  %686 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %161, i32 0, i32 0
  %687 = load i64, i64* %158, align 8
  store %"class.std::__1::unique_ptr.37"* %686, %"class.std::__1::unique_ptr.37"** %143, align 8
  store i64 %687, i64* %144, align 8
  %688 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %143, align 8
  %689 = load i64, i64* %144, align 8
  %690 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %688, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %690, %"class.std::__1::__compressed_pair.38"** %142, align 8
  %691 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %142, align 8
  %692 = bitcast %"class.std::__1::__compressed_pair.38"* %691 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %692, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %141, align 8
  %693 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %141, align 8
  %694 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %693, i32 0, i32 0
  %695 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %694, align 8
  %696 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %695, i64 %689
  %697 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %696, align 8
  %698 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %697, i32 0, i32 0
  %699 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %698, align 8
  %700 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %160, align 8
  %701 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %700, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %699, %"struct.std::__1::__hash_node_base"** %701, align 8
  %702 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %157, align 8
  %703 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %161, i32 0, i32 0
  %704 = load i64, i64* %158, align 8
  store %"class.std::__1::unique_ptr.37"* %703, %"class.std::__1::unique_ptr.37"** %147, align 8
  store i64 %704, i64* %148, align 8
  %705 = load %"class.std::__1::unique_ptr.37"*, %"class.std::__1::unique_ptr.37"** %147, align 8
  %706 = load i64, i64* %148, align 8
  %707 = getelementptr inbounds %"class.std::__1::unique_ptr.37", %"class.std::__1::unique_ptr.37"* %705, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.38"* %707, %"class.std::__1::__compressed_pair.38"** %146, align 8
  %708 = load %"class.std::__1::__compressed_pair.38"*, %"class.std::__1::__compressed_pair.38"** %146, align 8
  %709 = bitcast %"class.std::__1::__compressed_pair.38"* %708 to %"class.std::__1::__libcpp_compressed_pair_imp.39"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.39"* %709, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %145, align 8
  %710 = load %"class.std::__1::__libcpp_compressed_pair_imp.39"*, %"class.std::__1::__libcpp_compressed_pair_imp.39"** %145, align 8
  %711 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.39", %"class.std::__1::__libcpp_compressed_pair_imp.39"* %710, i32 0, i32 0
  %712 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %711, align 8
  %713 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %712, i64 %706
  %714 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %713, align 8
  %715 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %714, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %702, %"struct.std::__1::__hash_node_base"** %715, align 8
  br label %716

; <label>:716:                                    ; preds = %680, %389
  br label %717

; <label>:717:                                    ; preds = %716, %373
  br label %718

; <label>:718:                                    ; preds = %717
  %719 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %156, align 8
  %720 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %719, i32 0, i32 0
  %721 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %720, align 8
  store %"struct.std::__1::__hash_node_base"* %721, %"struct.std::__1::__hash_node_base"** %157, align 8
  br label %336

; <label>:722:                                    ; preds = %336
  br label %723

; <label>:723:                                    ; preds = %722, %273
  br label %724

; <label>:724:                                    ; preds = %723, %236
  ret void
}

; Function Attrs: nobuiltin nounwind
declare void @_ZdlPv(i8*) #11

; Function Attrs: nounwind readnone
declare i64 @llvm.ctlz.i64(i64, i1) #12

; Function Attrs: nounwind readnone
declare float @ceilf(float) #13

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEED1Ev(%"struct.std::__1::pair.92"*) unnamed_addr #7 align 2 {
  %2 = alloca %"struct.std::__1::pair.92"*, align 8
  store %"struct.std::__1::pair.92"* %0, %"struct.std::__1::pair.92"** %2, align 8
  %3 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %2, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEED2Ev(%"struct.std::__1::pair.92"* %3) #14
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEED2Ev(%"struct.std::__1::pair.92"*) unnamed_addr #7 align 2 {
  %2 = alloca %"struct.std::__1::pair.92"*, align 8
  store %"struct.std::__1::pair.92"* %0, %"struct.std::__1::pair.92"** %2, align 8
  %3 = load %"struct.std::__1::pair.92"*, %"struct.std::__1::pair.92"** %2, align 8
  %4 = getelementptr inbounds %"struct.std::__1::pair.92", %"struct.std::__1::pair.92"* %3, i32 0, i32 0
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %4) #14
  ret void
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr %"class.llvm::FunctionType"* @_ZN4llvm4castINS_12FunctionTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Type"*) #3 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = call zeroext i1 @_ZN4llvm3isaINS_12FunctionTypeEPNS_4TypeEEEbRKT0_(%"class.llvm::Type"** dereferenceable(8) %2)
  br i1 %3, label %4, label %5

; <label>:4:                                      ; preds = %1
  br label %5

; <label>:5:                                      ; preds = %4, %1
  %6 = phi i1 [ false, %1 ], [ true, %4 ]
  %7 = xor i1 %6, true
  br i1 %7, label %8, label %10

; <label>:8:                                      ; preds = %5
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm4castINS_10StructTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 236, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.9, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %11

; <label>:10:                                     ; preds = %5
  br label %11

; <label>:11:                                     ; preds = %10, %9
  %12 = call %"class.llvm::FunctionType"* @_ZN4llvm16cast_convert_valINS_12FunctionTypeEPNS_4TypeES3_E4doitERKS3_(%"class.llvm::Type"** dereferenceable(8) %2)
  ret %"class.llvm::FunctionType"* %12
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::CallInst"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCallEPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS7_EERKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"*, %"class.llvm::FunctionType"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::MDNode"*) #0 align 2 {
  %8 = alloca %"class.llvm::ArrayRef.36", align 8
  %9 = alloca %"class.llvm::IRBuilder"*, align 8
  %10 = alloca %"class.llvm::FunctionType"*, align 8
  %11 = alloca %"class.llvm::Value"*, align 8
  %12 = alloca %"class.llvm::Twine"*, align 8
  %13 = alloca %"class.llvm::MDNode"*, align 8
  %14 = alloca %"class.llvm::CallInst"*, align 8
  %15 = alloca %"class.llvm::ArrayRef.36", align 8
  %16 = alloca %"class.llvm::ArrayRef", align 8
  %17 = alloca %"class.llvm::Twine", align 8
  %18 = alloca %"class.llvm::FastMathFlags", align 4
  %19 = bitcast %"class.llvm::ArrayRef.36"* %8 to { %"class.llvm::Value"**, i64 }*
  %20 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %19, i32 0, i32 0
  store %"class.llvm::Value"** %3, %"class.llvm::Value"*** %20, align 8
  %21 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %19, i32 0, i32 1
  store i64 %4, i64* %21, align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %9, align 8
  store %"class.llvm::FunctionType"* %1, %"class.llvm::FunctionType"** %10, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %11, align 8
  store %"class.llvm::Twine"* %5, %"class.llvm::Twine"** %12, align 8
  store %"class.llvm::MDNode"* %6, %"class.llvm::MDNode"** %13, align 8
  %22 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %9, align 8
  %23 = load %"class.llvm::FunctionType"*, %"class.llvm::FunctionType"** %10, align 8
  %24 = load %"class.llvm::Value"*, %"class.llvm::Value"** %11, align 8
  %25 = bitcast %"class.llvm::ArrayRef.36"* %15 to i8*
  %26 = bitcast %"class.llvm::ArrayRef.36"* %8 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %25, i8* %26, i64 16, i32 8, i1 false)
  %27 = bitcast %"class.llvm::IRBuilder"* %22 to %"class.llvm::IRBuilderBase"*
  %28 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %27, i32 0, i32 6
  %29 = bitcast %"class.llvm::ArrayRef"* %16 to i8*
  %30 = bitcast %"class.llvm::ArrayRef"* %28 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %29, i8* %30, i64 16, i32 8, i1 false)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %17, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %31 = bitcast %"class.llvm::ArrayRef.36"* %15 to { %"class.llvm::Value"**, i64 }*
  %32 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %31, i32 0, i32 0
  %33 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %32, align 8
  %34 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %31, i32 0, i32 1
  %35 = load i64, i64* %34, align 8
  %36 = bitcast %"class.llvm::ArrayRef"* %16 to { %"class.llvm::OperandBundleDefT"*, i64 }*
  %37 = getelementptr inbounds { %"class.llvm::OperandBundleDefT"*, i64 }, { %"class.llvm::OperandBundleDefT"*, i64 }* %36, i32 0, i32 0
  %38 = load %"class.llvm::OperandBundleDefT"*, %"class.llvm::OperandBundleDefT"** %37, align 8
  %39 = getelementptr inbounds { %"class.llvm::OperandBundleDefT"*, i64 }, { %"class.llvm::OperandBundleDefT"*, i64 }* %36, i32 0, i32 1
  %40 = load i64, i64* %39, align 8
  %41 = call %"class.llvm::CallInst"* @_ZN4llvm8CallInst6CreateEPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS4_EENS5_INS_17OperandBundleDefTIS4_EEEERKNS_5TwineEPNS_11InstructionE(%"class.llvm::FunctionType"* %23, %"class.llvm::Value"* %24, %"class.llvm::Value"** %33, i64 %35, %"class.llvm::OperandBundleDefT"* %38, i64 %40, %"class.llvm::Twine"* dereferenceable(24) %17, %"class.llvm::Instruction"* null)
  store %"class.llvm::CallInst"* %41, %"class.llvm::CallInst"** %14, align 8
  %42 = call zeroext i1 @_ZN4llvm3isaINS_14FPMathOperatorEPNS_8CallInstEEEbRKT0_(%"class.llvm::CallInst"** dereferenceable(8) %14)
  br i1 %42, label %43, label %55

; <label>:43:                                     ; preds = %7
  %44 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %14, align 8
  %45 = bitcast %"class.llvm::CallInst"* %44 to %"class.llvm::Instruction"*
  %46 = load %"class.llvm::MDNode"*, %"class.llvm::MDNode"** %13, align 8
  %47 = bitcast %"class.llvm::IRBuilder"* %22 to %"class.llvm::IRBuilderBase"*
  %48 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %47, i32 0, i32 5
  %49 = bitcast %"class.llvm::FastMathFlags"* %18 to i8*
  %50 = bitcast %"class.llvm::FastMathFlags"* %48 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %49, i8* %50, i64 4, i32 4, i1 false)
  %51 = getelementptr inbounds %"class.llvm::FastMathFlags", %"class.llvm::FastMathFlags"* %18, i32 0, i32 0
  %52 = load i32, i32* %51, align 4
  %53 = call %"class.llvm::Instruction"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE19AddFPMathAttributesEPNS_11InstructionEPNS_6MDNodeENS_13FastMathFlagsE(%"class.llvm::IRBuilder"* %22, %"class.llvm::Instruction"* %45, %"class.llvm::MDNode"* %46, i32 %52)
  %54 = call %"class.llvm::CallInst"* @_ZN4llvm4castINS_8CallInstENS_11InstructionEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Instruction"* %53)
  store %"class.llvm::CallInst"* %54, %"class.llvm::CallInst"** %14, align 8
  br label %55

; <label>:55:                                     ; preds = %43, %7
  %56 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %14, align 8
  %57 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %12, align 8
  %58 = call %"class.llvm::CallInst"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_8CallInstEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"* %22, %"class.llvm::CallInst"* %56, %"class.llvm::Twine"* dereferenceable(24) %57)
  ret %"class.llvm::CallInst"* %58
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm3isaINS_12FunctionTypeEPNS_4TypeEEEbRKT0_(%"class.llvm::Type"** dereferenceable(8)) #3 {
  %2 = alloca %"class.llvm::Type"**, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %3 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_12FunctionTypeEKPNS_4TypeEPKS2_E4doitERS4_(%"class.llvm::Type"** dereferenceable(8) %3)
  ret i1 %4
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::FunctionType"* @_ZN4llvm16cast_convert_valINS_12FunctionTypeEPNS_4TypeES3_E4doitERKS3_(%"class.llvm::Type"** dereferenceable(8)) #4 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  %3 = alloca %"class.llvm::FunctionType"*, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %4 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %5 = load %"class.llvm::Type"*, %"class.llvm::Type"** %4, align 8
  %6 = bitcast %"class.llvm::Type"* %5 to %"class.llvm::FunctionType"*
  store %"class.llvm::FunctionType"* %6, %"class.llvm::FunctionType"** %3, align 8
  %7 = load %"class.llvm::FunctionType"*, %"class.llvm::FunctionType"** %3, align 8
  ret %"class.llvm::FunctionType"* %7
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_12FunctionTypeEKPNS_4TypeEPKS2_E4doitERS4_(%"class.llvm::Type"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  %3 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %4 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %5 = call %"class.llvm::Type"* @_ZN4llvm13simplify_typeIKPNS_4TypeEE18getSimplifiedValueERS3_(%"class.llvm::Type"** dereferenceable(8) %4)
  store %"class.llvm::Type"* %5, %"class.llvm::Type"** %3, align 8
  %6 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_12FunctionTypeEPKNS_4TypeES4_E4doitERKS4_(%"class.llvm::Type"** dereferenceable(8) %3)
  ret i1 %6
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_12FunctionTypeEPKNS_4TypeES4_E4doitERKS4_(%"class.llvm::Type"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Type"**, align 8
  store %"class.llvm::Type"** %0, %"class.llvm::Type"*** %2, align 8
  %3 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %2, align 8
  %4 = load %"class.llvm::Type"*, %"class.llvm::Type"** %3, align 8
  %5 = call zeroext i1 @_ZN4llvm11isa_impl_clINS_12FunctionTypeEPKNS_4TypeEE4doitES4_(%"class.llvm::Type"* %4)
  ret i1 %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm11isa_impl_clINS_12FunctionTypeEPKNS_4TypeEE4doitES4_(%"class.llvm::Type"*) #3 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = icmp ne %"class.llvm::Type"* %3, null
  br i1 %4, label %5, label %6

; <label>:5:                                      ; preds = %1
  br label %6

; <label>:6:                                      ; preds = %5, %1
  %7 = phi i1 [ false, %1 ], [ true, %5 ]
  %8 = xor i1 %7, true
  br i1 %8, label %9, label %11

; <label>:9:                                      ; preds = %6
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 95, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.7, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %12

; <label>:11:                                     ; preds = %6
  br label %12

; <label>:12:                                     ; preds = %11, %10
  %13 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %14 = call zeroext i1 @_ZN4llvm8isa_implINS_12FunctionTypeENS_4TypeEvE4doitERKS2_(%"class.llvm::Type"* dereferenceable(24) %13)
  ret i1 %14
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm8isa_implINS_12FunctionTypeENS_4TypeEvE4doitERKS2_(%"class.llvm::Type"* dereferenceable(24)) #3 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm12FunctionType7classofEPKNS_4TypeE(%"class.llvm::Type"* %3)
  ret i1 %4
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm12FunctionType7classofEPKNS_4TypeE(%"class.llvm::Type"*) #7 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call i32 @_ZNK4llvm4Type9getTypeIDEv(%"class.llvm::Type"* %3)
  %5 = icmp eq i32 %4, 12
  ret i1 %5
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::CallInst"* @_ZN4llvm8CallInst6CreateEPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS4_EENS5_INS_17OperandBundleDefTIS4_EEEERKNS_5TwineEPNS_11InstructionE(%"class.llvm::FunctionType"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, %"class.llvm::OperandBundleDefT"*, i64, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::Instruction"*) #0 align 2 {
  %9 = alloca %"class.llvm::ArrayRef.36", align 8
  %10 = alloca %"class.llvm::ArrayRef", align 8
  %11 = alloca %"class.llvm::FunctionType"*, align 8
  %12 = alloca %"class.llvm::Value"*, align 8
  %13 = alloca %"class.llvm::Twine"*, align 8
  %14 = alloca %"class.llvm::Instruction"*, align 8
  %15 = alloca i32, align 4
  %16 = alloca %"class.llvm::ArrayRef", align 8
  %17 = alloca i32, align 4
  %18 = alloca %"class.llvm::ArrayRef.36", align 8
  %19 = alloca %"class.llvm::ArrayRef", align 8
  %20 = bitcast %"class.llvm::ArrayRef.36"* %9 to { %"class.llvm::Value"**, i64 }*
  %21 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %20, i32 0, i32 0
  store %"class.llvm::Value"** %2, %"class.llvm::Value"*** %21, align 8
  %22 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %20, i32 0, i32 1
  store i64 %3, i64* %22, align 8
  %23 = bitcast %"class.llvm::ArrayRef"* %10 to { %"class.llvm::OperandBundleDefT"*, i64 }*
  %24 = getelementptr inbounds { %"class.llvm::OperandBundleDefT"*, i64 }, { %"class.llvm::OperandBundleDefT"*, i64 }* %23, i32 0, i32 0
  store %"class.llvm::OperandBundleDefT"* %4, %"class.llvm::OperandBundleDefT"** %24, align 8
  %25 = getelementptr inbounds { %"class.llvm::OperandBundleDefT"*, i64 }, { %"class.llvm::OperandBundleDefT"*, i64 }* %23, i32 0, i32 1
  store i64 %5, i64* %25, align 8
  store %"class.llvm::FunctionType"* %0, %"class.llvm::FunctionType"** %11, align 8
  store %"class.llvm::Value"* %1, %"class.llvm::Value"** %12, align 8
  store %"class.llvm::Twine"* %6, %"class.llvm::Twine"** %13, align 8
  store %"class.llvm::Instruction"* %7, %"class.llvm::Instruction"** %14, align 8
  %26 = call i64 @_ZNK4llvm8ArrayRefIPNS_5ValueEE4sizeEv(%"class.llvm::ArrayRef.36"* %9)
  %27 = trunc i64 %26 to i32
  %28 = bitcast %"class.llvm::ArrayRef"* %16 to i8*
  %29 = bitcast %"class.llvm::ArrayRef"* %10 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %28, i8* %29, i64 16, i32 8, i1 false)
  %30 = bitcast %"class.llvm::ArrayRef"* %16 to { %"class.llvm::OperandBundleDefT"*, i64 }*
  %31 = getelementptr inbounds { %"class.llvm::OperandBundleDefT"*, i64 }, { %"class.llvm::OperandBundleDefT"*, i64 }* %30, i32 0, i32 0
  %32 = load %"class.llvm::OperandBundleDefT"*, %"class.llvm::OperandBundleDefT"** %31, align 8
  %33 = getelementptr inbounds { %"class.llvm::OperandBundleDefT"*, i64 }, { %"class.llvm::OperandBundleDefT"*, i64 }* %30, i32 0, i32 1
  %34 = load i64, i64* %33, align 8
  %35 = call i32 @_ZN4llvm17OperandBundleUserINS_8CallInstEPNS_3UseEE17CountBundleInputsENS_8ArrayRefINS_17OperandBundleDefTIPNS_5ValueEEEEE(%"class.llvm::OperandBundleDefT"* %32, i64 %34)
  %36 = add i32 %27, %35
  %37 = add i32 %36, 1
  store i32 %37, i32* %15, align 4
  %38 = call i64 @_ZNK4llvm8ArrayRefINS_17OperandBundleDefTIPNS_5ValueEEEE4sizeEv(%"class.llvm::ArrayRef"* %10)
  %39 = mul i64 %38, 16
  %40 = trunc i64 %39 to i32
  store i32 %40, i32* %17, align 4
  %41 = load i32, i32* %15, align 4
  %42 = load i32, i32* %17, align 4
  %43 = call i8* @_ZN4llvm4UsernwEmjj(i64 80, i32 %41, i32 %42)
  %44 = bitcast i8* %43 to %"class.llvm::CallInst"*
  %45 = load %"class.llvm::FunctionType"*, %"class.llvm::FunctionType"** %11, align 8
  %46 = load %"class.llvm::Value"*, %"class.llvm::Value"** %12, align 8
  %47 = bitcast %"class.llvm::ArrayRef.36"* %18 to i8*
  %48 = bitcast %"class.llvm::ArrayRef.36"* %9 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %47, i8* %48, i64 16, i32 8, i1 false)
  %49 = bitcast %"class.llvm::ArrayRef"* %19 to i8*
  %50 = bitcast %"class.llvm::ArrayRef"* %10 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %49, i8* %50, i64 16, i32 8, i1 false)
  %51 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %13, align 8
  %52 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %14, align 8
  %53 = bitcast %"class.llvm::ArrayRef.36"* %18 to { %"class.llvm::Value"**, i64 }*
  %54 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %53, i32 0, i32 0
  %55 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %54, align 8
  %56 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %53, i32 0, i32 1
  %57 = load i64, i64* %56, align 8
  call void @_ZN4llvm8CallInstC1EPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS4_EENS5_INS_17OperandBundleDefTIS4_EEEERKNS_5TwineEPNS_11InstructionE(%"class.llvm::CallInst"* %44, %"class.llvm::FunctionType"* %45, %"class.llvm::Value"* %46, %"class.llvm::Value"** %55, i64 %57, %"class.llvm::ArrayRef"* byval align 8 %19, %"class.llvm::Twine"* dereferenceable(24) %51, %"class.llvm::Instruction"* %52)
  ret %"class.llvm::CallInst"* %44
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm3isaINS_14FPMathOperatorEPNS_8CallInstEEEbRKT0_(%"class.llvm::CallInst"** dereferenceable(8)) #3 {
  %2 = alloca %"class.llvm::CallInst"**, align 8
  store %"class.llvm::CallInst"** %0, %"class.llvm::CallInst"*** %2, align 8
  %3 = load %"class.llvm::CallInst"**, %"class.llvm::CallInst"*** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_14FPMathOperatorEKPNS_8CallInstEPKS2_E4doitERS4_(%"class.llvm::CallInst"** dereferenceable(8) %3)
  ret i1 %4
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr %"class.llvm::CallInst"* @_ZN4llvm4castINS_8CallInstENS_11InstructionEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Instruction"*) #3 {
  %2 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"* %0, %"class.llvm::Instruction"** %2, align 8
  %3 = call zeroext i1 @_ZN4llvm3isaINS_8CallInstEPNS_11InstructionEEEbRKT0_(%"class.llvm::Instruction"** dereferenceable(8) %2)
  br i1 %3, label %4, label %5

; <label>:4:                                      ; preds = %1
  br label %5

; <label>:5:                                      ; preds = %4, %1
  %6 = phi i1 [ false, %1 ], [ true, %4 ]
  %7 = xor i1 %6, true
  br i1 %7, label %8, label %10

; <label>:8:                                      ; preds = %5
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm4castINS_10StructTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 236, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.9, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %11

; <label>:10:                                     ; preds = %5
  br label %11

; <label>:11:                                     ; preds = %10, %9
  %12 = call %"class.llvm::CallInst"* @_ZN4llvm16cast_convert_valINS_8CallInstEPNS_11InstructionES3_E4doitERKS3_(%"class.llvm::Instruction"** dereferenceable(8) %2)
  ret %"class.llvm::CallInst"* %12
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
define linkonce_odr %"class.llvm::CallInst"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_8CallInstEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::CallInst"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %4 = alloca %"class.llvm::IRBuilder"*, align 8
  %5 = alloca %"class.llvm::CallInst"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  %7 = alloca %"class.llvm::ilist_iterator", align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %4, align 8
  store %"class.llvm::CallInst"* %1, %"class.llvm::CallInst"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %8 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %4, align 8
  %9 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderDefaultInserter"*
  %10 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %5, align 8
  %11 = bitcast %"class.llvm::CallInst"* %10 to %"class.llvm::Instruction"*
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
  %21 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %20, align 8
  call void @_ZNK4llvm24IRBuilderDefaultInserter12InsertHelperEPNS_11InstructionERKNS_5TwineEPNS_10BasicBlockENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEE(%"class.llvm::IRBuilderDefaultInserter"* %9, %"class.llvm::Instruction"* %11, %"class.llvm::Twine"* dereferenceable(24) %12, %"class.llvm::BasicBlock"* %15, %"class.llvm::ilist_node_impl.0"* %21)
  %22 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderBase"*
  %23 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %5, align 8
  %24 = bitcast %"class.llvm::CallInst"* %23 to %"class.llvm::Instruction"*
  call void @_ZNK4llvm13IRBuilderBase20SetInstDebugLocationEPNS_11InstructionE(%"class.llvm::IRBuilderBase"* %22, %"class.llvm::Instruction"* %24)
  %25 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %5, align 8
  ret %"class.llvm::CallInst"* %25
}

; Function Attrs: ssp uwtable
define linkonce_odr i32 @_ZN4llvm17OperandBundleUserINS_8CallInstEPNS_3UseEE17CountBundleInputsENS_8ArrayRefINS_17OperandBundleDefTIPNS_5ValueEEEEE(%"class.llvm::OperandBundleDefT"*, i64) #0 align 2 {
  %3 = alloca %"class.llvm::ArrayRef", align 8
  %4 = alloca i32, align 4
  %5 = alloca %"class.llvm::ArrayRef"*, align 8
  %6 = alloca %"class.llvm::OperandBundleDefT"*, align 8
  %7 = alloca %"class.llvm::OperandBundleDefT"*, align 8
  %8 = alloca %"class.llvm::OperandBundleDefT"*, align 8
  %9 = bitcast %"class.llvm::ArrayRef"* %3 to { %"class.llvm::OperandBundleDefT"*, i64 }*
  %10 = getelementptr inbounds { %"class.llvm::OperandBundleDefT"*, i64 }, { %"class.llvm::OperandBundleDefT"*, i64 }* %9, i32 0, i32 0
  store %"class.llvm::OperandBundleDefT"* %0, %"class.llvm::OperandBundleDefT"** %10, align 8
  %11 = getelementptr inbounds { %"class.llvm::OperandBundleDefT"*, i64 }, { %"class.llvm::OperandBundleDefT"*, i64 }* %9, i32 0, i32 1
  store i64 %1, i64* %11, align 8
  store i32 0, i32* %4, align 4
  store %"class.llvm::ArrayRef"* %3, %"class.llvm::ArrayRef"** %5, align 8
  %12 = load %"class.llvm::ArrayRef"*, %"class.llvm::ArrayRef"** %5, align 8
  %13 = call %"class.llvm::OperandBundleDefT"* @_ZNK4llvm8ArrayRefINS_17OperandBundleDefTIPNS_5ValueEEEE5beginEv(%"class.llvm::ArrayRef"* %12)
  store %"class.llvm::OperandBundleDefT"* %13, %"class.llvm::OperandBundleDefT"** %6, align 8
  %14 = load %"class.llvm::ArrayRef"*, %"class.llvm::ArrayRef"** %5, align 8
  %15 = call %"class.llvm::OperandBundleDefT"* @_ZNK4llvm8ArrayRefINS_17OperandBundleDefTIPNS_5ValueEEEE3endEv(%"class.llvm::ArrayRef"* %14)
  store %"class.llvm::OperandBundleDefT"* %15, %"class.llvm::OperandBundleDefT"** %7, align 8
  br label %16

; <label>:16:                                     ; preds = %28, %2
  %17 = load %"class.llvm::OperandBundleDefT"*, %"class.llvm::OperandBundleDefT"** %6, align 8
  %18 = load %"class.llvm::OperandBundleDefT"*, %"class.llvm::OperandBundleDefT"** %7, align 8
  %19 = icmp ne %"class.llvm::OperandBundleDefT"* %17, %18
  br i1 %19, label %20, label %31

; <label>:20:                                     ; preds = %16
  %21 = load %"class.llvm::OperandBundleDefT"*, %"class.llvm::OperandBundleDefT"** %6, align 8
  store %"class.llvm::OperandBundleDefT"* %21, %"class.llvm::OperandBundleDefT"** %8, align 8
  %22 = load %"class.llvm::OperandBundleDefT"*, %"class.llvm::OperandBundleDefT"** %8, align 8
  %23 = call i64 @_ZNK4llvm17OperandBundleDefTIPNS_5ValueEE10input_sizeEv(%"class.llvm::OperandBundleDefT"* %22)
  %24 = load i32, i32* %4, align 4
  %25 = zext i32 %24 to i64
  %26 = add i64 %25, %23
  %27 = trunc i64 %26 to i32
  store i32 %27, i32* %4, align 4
  br label %28

; <label>:28:                                     ; preds = %20
  %29 = load %"class.llvm::OperandBundleDefT"*, %"class.llvm::OperandBundleDefT"** %6, align 8
  %30 = getelementptr inbounds %"class.llvm::OperandBundleDefT", %"class.llvm::OperandBundleDefT"* %29, i32 1
  store %"class.llvm::OperandBundleDefT"* %30, %"class.llvm::OperandBundleDefT"** %6, align 8
  br label %16

; <label>:31:                                     ; preds = %16
  %32 = load i32, i32* %4, align 4
  ret i32 %32
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNK4llvm8ArrayRefINS_17OperandBundleDefTIPNS_5ValueEEEE4sizeEv(%"class.llvm::ArrayRef"*) #4 align 2 {
  %2 = alloca %"class.llvm::ArrayRef"*, align 8
  store %"class.llvm::ArrayRef"* %0, %"class.llvm::ArrayRef"** %2, align 8
  %3 = load %"class.llvm::ArrayRef"*, %"class.llvm::ArrayRef"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::ArrayRef", %"class.llvm::ArrayRef"* %3, i32 0, i32 1
  %5 = load i64, i64* %4, align 8
  ret i64 %5
}

declare i8* @_ZN4llvm4UsernwEmjj(i64, i32, i32) #1

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZN4llvm8CallInstC1EPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS4_EENS5_INS_17OperandBundleDefTIS4_EEEERKNS_5TwineEPNS_11InstructionE(%"class.llvm::CallInst"*, %"class.llvm::FunctionType"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, %"class.llvm::ArrayRef"* byval align 8, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::Instruction"*) unnamed_addr #3 align 2 {
  %9 = alloca %"class.llvm::ArrayRef.36", align 8
  %10 = alloca %"class.llvm::CallInst"*, align 8
  %11 = alloca %"class.llvm::FunctionType"*, align 8
  %12 = alloca %"class.llvm::Value"*, align 8
  %13 = alloca %"class.llvm::Twine"*, align 8
  %14 = alloca %"class.llvm::Instruction"*, align 8
  %15 = bitcast %"class.llvm::ArrayRef.36"* %9 to { %"class.llvm::Value"**, i64 }*
  %16 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %15, i32 0, i32 0
  store %"class.llvm::Value"** %3, %"class.llvm::Value"*** %16, align 8
  %17 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %15, i32 0, i32 1
  store i64 %4, i64* %17, align 8
  store %"class.llvm::CallInst"* %0, %"class.llvm::CallInst"** %10, align 8
  store %"class.llvm::FunctionType"* %1, %"class.llvm::FunctionType"** %11, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %12, align 8
  store %"class.llvm::Twine"* %6, %"class.llvm::Twine"** %13, align 8
  store %"class.llvm::Instruction"* %7, %"class.llvm::Instruction"** %14, align 8
  %18 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %10, align 8
  %19 = load %"class.llvm::FunctionType"*, %"class.llvm::FunctionType"** %11, align 8
  %20 = load %"class.llvm::Value"*, %"class.llvm::Value"** %12, align 8
  %21 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %13, align 8
  %22 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %14, align 8
  %23 = bitcast %"class.llvm::ArrayRef.36"* %9 to { %"class.llvm::Value"**, i64 }*
  %24 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %23, i32 0, i32 0
  %25 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %24, align 8
  %26 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %23, i32 0, i32 1
  %27 = load i64, i64* %26, align 8
  call void @_ZN4llvm8CallInstC2EPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS4_EENS5_INS_17OperandBundleDefTIS4_EEEERKNS_5TwineEPNS_11InstructionE(%"class.llvm::CallInst"* %18, %"class.llvm::FunctionType"* %19, %"class.llvm::Value"* %20, %"class.llvm::Value"** %25, i64 %27, %"class.llvm::ArrayRef"* byval align 8 %5, %"class.llvm::Twine"* dereferenceable(24) %21, %"class.llvm::Instruction"* %22)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::OperandBundleDefT"* @_ZNK4llvm8ArrayRefINS_17OperandBundleDefTIPNS_5ValueEEEE5beginEv(%"class.llvm::ArrayRef"*) #4 align 2 {
  %2 = alloca %"class.llvm::ArrayRef"*, align 8
  store %"class.llvm::ArrayRef"* %0, %"class.llvm::ArrayRef"** %2, align 8
  %3 = load %"class.llvm::ArrayRef"*, %"class.llvm::ArrayRef"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::ArrayRef", %"class.llvm::ArrayRef"* %3, i32 0, i32 0
  %5 = load %"class.llvm::OperandBundleDefT"*, %"class.llvm::OperandBundleDefT"** %4, align 8
  ret %"class.llvm::OperandBundleDefT"* %5
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::OperandBundleDefT"* @_ZNK4llvm8ArrayRefINS_17OperandBundleDefTIPNS_5ValueEEEE3endEv(%"class.llvm::ArrayRef"*) #4 align 2 {
  %2 = alloca %"class.llvm::ArrayRef"*, align 8
  store %"class.llvm::ArrayRef"* %0, %"class.llvm::ArrayRef"** %2, align 8
  %3 = load %"class.llvm::ArrayRef"*, %"class.llvm::ArrayRef"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::ArrayRef", %"class.llvm::ArrayRef"* %3, i32 0, i32 0
  %5 = load %"class.llvm::OperandBundleDefT"*, %"class.llvm::OperandBundleDefT"** %4, align 8
  %6 = getelementptr inbounds %"class.llvm::ArrayRef", %"class.llvm::ArrayRef"* %3, i32 0, i32 1
  %7 = load i64, i64* %6, align 8
  %8 = getelementptr inbounds %"class.llvm::OperandBundleDefT", %"class.llvm::OperandBundleDefT"* %5, i64 %7
  ret %"class.llvm::OperandBundleDefT"* %8
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNK4llvm17OperandBundleDefTIPNS_5ValueEE10input_sizeEv(%"class.llvm::OperandBundleDefT"*) #4 align 2 {
  %2 = alloca %"class.std::__1::vector"*, align 8
  %3 = alloca %"class.llvm::OperandBundleDefT"*, align 8
  store %"class.llvm::OperandBundleDefT"* %0, %"class.llvm::OperandBundleDefT"** %3, align 8
  %4 = load %"class.llvm::OperandBundleDefT"*, %"class.llvm::OperandBundleDefT"** %3, align 8
  %5 = getelementptr inbounds %"class.llvm::OperandBundleDefT", %"class.llvm::OperandBundleDefT"* %4, i32 0, i32 1
  store %"class.std::__1::vector"* %5, %"class.std::__1::vector"** %2, align 8
  %6 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %2, align 8
  %7 = bitcast %"class.std::__1::vector"* %6 to %"class.std::__1::__vector_base"*
  %8 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %7, i32 0, i32 1
  %9 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %8, align 8
  %10 = bitcast %"class.std::__1::vector"* %6 to %"class.std::__1::__vector_base"*
  %11 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %10, i32 0, i32 0
  %12 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %11, align 8
  %13 = ptrtoint %"class.llvm::Value"** %9 to i64
  %14 = ptrtoint %"class.llvm::Value"** %12 to i64
  %15 = sub i64 %13, %14
  %16 = sdiv exact i64 %15, 8
  ret i64 %16
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZN4llvm8CallInstC2EPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS4_EENS5_INS_17OperandBundleDefTIS4_EEEERKNS_5TwineEPNS_11InstructionE(%"class.llvm::CallInst"*, %"class.llvm::FunctionType"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, %"class.llvm::ArrayRef"* byval align 8, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::Instruction"*) unnamed_addr #3 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %9 = alloca %"class.llvm::ArrayRef.36", align 8
  %10 = alloca %"class.llvm::CallInst"*, align 8
  %11 = alloca %"class.llvm::FunctionType"*, align 8
  %12 = alloca %"class.llvm::Value"*, align 8
  %13 = alloca %"class.llvm::Twine"*, align 8
  %14 = alloca %"class.llvm::Instruction"*, align 8
  %15 = alloca %"class.llvm::ArrayRef", align 8
  %16 = alloca %"class.llvm::ArrayRef", align 8
  %17 = alloca i8*
  %18 = alloca i32
  %19 = alloca %"class.llvm::ArrayRef.36", align 8
  %20 = alloca %"class.llvm::ArrayRef", align 8
  %21 = bitcast %"class.llvm::ArrayRef.36"* %9 to { %"class.llvm::Value"**, i64 }*
  %22 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %21, i32 0, i32 0
  store %"class.llvm::Value"** %3, %"class.llvm::Value"*** %22, align 8
  %23 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %21, i32 0, i32 1
  store i64 %4, i64* %23, align 8
  store %"class.llvm::CallInst"* %0, %"class.llvm::CallInst"** %10, align 8
  store %"class.llvm::FunctionType"* %1, %"class.llvm::FunctionType"** %11, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %12, align 8
  store %"class.llvm::Twine"* %6, %"class.llvm::Twine"** %13, align 8
  store %"class.llvm::Instruction"* %7, %"class.llvm::Instruction"** %14, align 8
  %24 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %10, align 8
  %25 = bitcast %"class.llvm::CallInst"* %24 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([5 x i8*], [5 x i8*]* @_ZTVN4llvm8CallInstE, i32 0, i32 2) to i32 (...)**), i32 (...)*** %25, align 8
  %26 = bitcast %"class.llvm::CallInst"* %24 to %"class.llvm::Instruction"*
  %27 = load %"class.llvm::FunctionType"*, %"class.llvm::FunctionType"** %11, align 8
  %28 = call %"class.llvm::Type"* @_ZNK4llvm12FunctionType13getReturnTypeEv(%"class.llvm::FunctionType"* %27)
  %29 = call %"class.llvm::Use"* @_ZN4llvm21VariadicOperandTraitsINS_8CallInstELj1EE6op_endEPS1_(%"class.llvm::CallInst"* %24)
  %30 = call i64 @_ZNK4llvm8ArrayRefIPNS_5ValueEE4sizeEv(%"class.llvm::ArrayRef.36"* %9)
  %31 = bitcast %"class.llvm::ArrayRef"* %15 to i8*
  %32 = bitcast %"class.llvm::ArrayRef"* %5 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %31, i8* %32, i64 16, i32 8, i1 false)
  %33 = bitcast %"class.llvm::ArrayRef"* %15 to { %"class.llvm::OperandBundleDefT"*, i64 }*
  %34 = getelementptr inbounds { %"class.llvm::OperandBundleDefT"*, i64 }, { %"class.llvm::OperandBundleDefT"*, i64 }* %33, i32 0, i32 0
  %35 = load %"class.llvm::OperandBundleDefT"*, %"class.llvm::OperandBundleDefT"** %34, align 8
  %36 = getelementptr inbounds { %"class.llvm::OperandBundleDefT"*, i64 }, { %"class.llvm::OperandBundleDefT"*, i64 }* %33, i32 0, i32 1
  %37 = load i64, i64* %36, align 8
  %38 = call i32 @_ZN4llvm17OperandBundleUserINS_8CallInstEPNS_3UseEE17CountBundleInputsENS_8ArrayRefINS_17OperandBundleDefTIPNS_5ValueEEEEE(%"class.llvm::OperandBundleDefT"* %35, i64 %37)
  %39 = zext i32 %38 to i64
  %40 = add i64 %30, %39
  %41 = add i64 %40, 1
  %42 = sub i64 0, %41
  %43 = getelementptr inbounds %"class.llvm::Use", %"class.llvm::Use"* %29, i64 %42
  %44 = call i64 @_ZNK4llvm8ArrayRefIPNS_5ValueEE4sizeEv(%"class.llvm::ArrayRef.36"* %9)
  %45 = bitcast %"class.llvm::ArrayRef"* %16 to i8*
  %46 = bitcast %"class.llvm::ArrayRef"* %5 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %45, i8* %46, i64 16, i32 8, i1 false)
  %47 = bitcast %"class.llvm::ArrayRef"* %16 to { %"class.llvm::OperandBundleDefT"*, i64 }*
  %48 = getelementptr inbounds { %"class.llvm::OperandBundleDefT"*, i64 }, { %"class.llvm::OperandBundleDefT"*, i64 }* %47, i32 0, i32 0
  %49 = load %"class.llvm::OperandBundleDefT"*, %"class.llvm::OperandBundleDefT"** %48, align 8
  %50 = getelementptr inbounds { %"class.llvm::OperandBundleDefT"*, i64 }, { %"class.llvm::OperandBundleDefT"*, i64 }* %47, i32 0, i32 1
  %51 = load i64, i64* %50, align 8
  %52 = call i32 @_ZN4llvm17OperandBundleUserINS_8CallInstEPNS_3UseEE17CountBundleInputsENS_8ArrayRefINS_17OperandBundleDefTIPNS_5ValueEEEEE(%"class.llvm::OperandBundleDefT"* %49, i64 %51)
  %53 = zext i32 %52 to i64
  %54 = add i64 %44, %53
  %55 = add i64 %54, 1
  %56 = trunc i64 %55 to i32
  %57 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %14, align 8
  call void @_ZN4llvm11InstructionC2EPNS_4TypeEjPNS_3UseEjPS0_(%"class.llvm::Instruction"* %26, %"class.llvm::Type"* %28, i32 54, %"class.llvm::Use"* %43, i32 %56, %"class.llvm::Instruction"* %57)
  %58 = bitcast %"class.llvm::CallInst"* %24 to %"class.llvm::OperandBundleUser"*
  %59 = bitcast %"class.llvm::CallInst"* %24 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([5 x i8*], [5 x i8*]* @_ZTVN4llvm8CallInstE, i32 0, i32 2) to i32 (...)**), i32 (...)*** %59, align 8
  %60 = getelementptr inbounds %"class.llvm::CallInst", %"class.llvm::CallInst"* %24, i32 0, i32 1
  invoke void @_ZN4llvm12AttributeSetC1Ev(%"class.llvm::AttributeSet"* %60)
          to label %61 unwind label %75

; <label>:61:                                     ; preds = %8
  %62 = load %"class.llvm::FunctionType"*, %"class.llvm::FunctionType"** %11, align 8
  %63 = load %"class.llvm::Value"*, %"class.llvm::Value"** %12, align 8
  %64 = bitcast %"class.llvm::ArrayRef.36"* %19 to i8*
  %65 = bitcast %"class.llvm::ArrayRef.36"* %9 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %64, i8* %65, i64 16, i32 8, i1 false)
  %66 = bitcast %"class.llvm::ArrayRef"* %20 to i8*
  %67 = bitcast %"class.llvm::ArrayRef"* %5 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %66, i8* %67, i64 16, i32 8, i1 false)
  %68 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %13, align 8
  %69 = bitcast %"class.llvm::ArrayRef.36"* %19 to { %"class.llvm::Value"**, i64 }*
  %70 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %69, i32 0, i32 0
  %71 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %70, align 8
  %72 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %69, i32 0, i32 1
  %73 = load i64, i64* %72, align 8
  invoke void @_ZN4llvm8CallInst4initEPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS4_EENS5_INS_17OperandBundleDefTIS4_EEEERKNS_5TwineE(%"class.llvm::CallInst"* %24, %"class.llvm::FunctionType"* %62, %"class.llvm::Value"* %63, %"class.llvm::Value"** %71, i64 %73, %"class.llvm::ArrayRef"* byval align 8 %20, %"class.llvm::Twine"* dereferenceable(24) %68)
          to label %74 unwind label %75

; <label>:74:                                     ; preds = %61
  ret void

; <label>:75:                                     ; preds = %61, %8
  %76 = landingpad { i8*, i32 }
          cleanup
  %77 = extractvalue { i8*, i32 } %76, 0
  store i8* %77, i8** %17, align 8
  %78 = extractvalue { i8*, i32 } %76, 1
  store i32 %78, i32* %18, align 4
  %79 = bitcast %"class.llvm::CallInst"* %24 to %"class.llvm::Instruction"*
  call void @_ZN4llvm11InstructionD2Ev(%"class.llvm::Instruction"* %79) #14
  br label %80

; <label>:80:                                     ; preds = %75
  %81 = load i8*, i8** %17, align 8
  %82 = load i32, i32* %18, align 4
  %83 = insertvalue { i8*, i32 } undef, i8* %81, 0
  %84 = insertvalue { i8*, i32 } %83, i32 %82, 1
  resume { i8*, i32 } %84
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::Type"* @_ZNK4llvm12FunctionType13getReturnTypeEv(%"class.llvm::FunctionType"*) #4 align 2 {
  %2 = alloca %"class.llvm::FunctionType"*, align 8
  store %"class.llvm::FunctionType"* %0, %"class.llvm::FunctionType"** %2, align 8
  %3 = load %"class.llvm::FunctionType"*, %"class.llvm::FunctionType"** %2, align 8
  %4 = bitcast %"class.llvm::FunctionType"* %3 to %"class.llvm::Type"*
  %5 = getelementptr inbounds %"class.llvm::Type", %"class.llvm::Type"* %4, i32 0, i32 3
  %6 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %5, align 8
  %7 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %6, i64 0
  %8 = load %"class.llvm::Type"*, %"class.llvm::Type"** %7, align 8
  ret %"class.llvm::Type"* %8
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::Use"* @_ZN4llvm21VariadicOperandTraitsINS_8CallInstELj1EE6op_endEPS1_(%"class.llvm::CallInst"*) #4 align 2 {
  %2 = alloca %"class.llvm::CallInst"*, align 8
  store %"class.llvm::CallInst"* %0, %"class.llvm::CallInst"** %2, align 8
  %3 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %2, align 8
  %4 = bitcast %"class.llvm::CallInst"* %3 to %"class.llvm::Use"*
  ret %"class.llvm::Use"* %4
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm12AttributeSetC1Ev(%"class.llvm::AttributeSet"*) unnamed_addr #0 align 2 {
  %2 = alloca %"class.llvm::AttributeSet"*, align 8
  store %"class.llvm::AttributeSet"* %0, %"class.llvm::AttributeSet"** %2, align 8
  %3 = load %"class.llvm::AttributeSet"*, %"class.llvm::AttributeSet"** %2, align 8
  call void @_ZN4llvm12AttributeSetC2Ev(%"class.llvm::AttributeSet"* %3)
  ret void
}

declare void @_ZN4llvm8CallInst4initEPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS4_EENS5_INS_17OperandBundleDefTIS4_EEEERKNS_5TwineE(%"class.llvm::CallInst"*, %"class.llvm::FunctionType"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, %"class.llvm::ArrayRef"* byval align 8, %"class.llvm::Twine"* dereferenceable(24)) #1

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm12AttributeSetC2Ev(%"class.llvm::AttributeSet"*) unnamed_addr #4 align 2 {
  %2 = alloca %"class.llvm::AttributeSet"*, align 8
  store %"class.llvm::AttributeSet"* %0, %"class.llvm::AttributeSet"** %2, align 8
  %3 = load %"class.llvm::AttributeSet"*, %"class.llvm::AttributeSet"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::AttributeSet", %"class.llvm::AttributeSet"* %3, i32 0, i32 0
  store %"class.llvm::AttributeSetImpl"* null, %"class.llvm::AttributeSetImpl"** %4, align 8
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_14FPMathOperatorEKPNS_8CallInstEPKS2_E4doitERS4_(%"class.llvm::CallInst"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::CallInst"**, align 8
  %3 = alloca %"class.llvm::CallInst"*, align 8
  store %"class.llvm::CallInst"** %0, %"class.llvm::CallInst"*** %2, align 8
  %4 = load %"class.llvm::CallInst"**, %"class.llvm::CallInst"*** %2, align 8
  %5 = call %"class.llvm::CallInst"* @_ZN4llvm13simplify_typeIKPNS_8CallInstEE18getSimplifiedValueERS3_(%"class.llvm::CallInst"** dereferenceable(8) %4)
  store %"class.llvm::CallInst"* %5, %"class.llvm::CallInst"** %3, align 8
  %6 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_14FPMathOperatorEPKNS_8CallInstES4_E4doitERKS4_(%"class.llvm::CallInst"** dereferenceable(8) %3)
  ret i1 %6
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_14FPMathOperatorEPKNS_8CallInstES4_E4doitERKS4_(%"class.llvm::CallInst"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::CallInst"**, align 8
  store %"class.llvm::CallInst"** %0, %"class.llvm::CallInst"*** %2, align 8
  %3 = load %"class.llvm::CallInst"**, %"class.llvm::CallInst"*** %2, align 8
  %4 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %3, align 8
  %5 = call zeroext i1 @_ZN4llvm11isa_impl_clINS_14FPMathOperatorEPKNS_8CallInstEE4doitES4_(%"class.llvm::CallInst"* %4)
  ret i1 %5
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::CallInst"* @_ZN4llvm13simplify_typeIKPNS_8CallInstEE18getSimplifiedValueERS3_(%"class.llvm::CallInst"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::CallInst"**, align 8
  store %"class.llvm::CallInst"** %0, %"class.llvm::CallInst"*** %2, align 8
  %3 = load %"class.llvm::CallInst"**, %"class.llvm::CallInst"*** %2, align 8
  %4 = call dereferenceable(8) %"class.llvm::CallInst"** @_ZN4llvm13simplify_typeIPNS_8CallInstEE18getSimplifiedValueERS2_(%"class.llvm::CallInst"** dereferenceable(8) %3)
  %5 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %4, align 8
  ret %"class.llvm::CallInst"* %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm11isa_impl_clINS_14FPMathOperatorEPKNS_8CallInstEE4doitES4_(%"class.llvm::CallInst"*) #3 align 2 {
  %2 = alloca %"class.llvm::CallInst"*, align 8
  store %"class.llvm::CallInst"* %0, %"class.llvm::CallInst"** %2, align 8
  %3 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %2, align 8
  %4 = icmp ne %"class.llvm::CallInst"* %3, null
  br i1 %4, label %5, label %6

; <label>:5:                                      ; preds = %1
  br label %6

; <label>:6:                                      ; preds = %5, %1
  %7 = phi i1 [ false, %1 ], [ true, %5 ]
  %8 = xor i1 %7, true
  br i1 %8, label %9, label %11

; <label>:9:                                      ; preds = %6
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 95, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.7, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %12

; <label>:11:                                     ; preds = %6
  br label %12

; <label>:12:                                     ; preds = %11, %10
  %13 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %2, align 8
  %14 = call zeroext i1 @_ZN4llvm8isa_implINS_14FPMathOperatorENS_8CallInstEvE4doitERKS2_(%"class.llvm::CallInst"* dereferenceable(80) %13)
  ret i1 %14
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm8isa_implINS_14FPMathOperatorENS_8CallInstEvE4doitERKS2_(%"class.llvm::CallInst"* dereferenceable(80)) #3 align 2 {
  %2 = alloca %"class.llvm::CallInst"*, align 8
  store %"class.llvm::CallInst"* %0, %"class.llvm::CallInst"** %2, align 8
  %3 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %2, align 8
  %4 = bitcast %"class.llvm::CallInst"* %3 to %"class.llvm::Instruction"*
  %5 = call zeroext i1 @_ZN4llvm14FPMathOperator7classofEPKNS_11InstructionE(%"class.llvm::Instruction"* %4)
  ret i1 %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm14FPMathOperator7classofEPKNS_11InstructionE(%"class.llvm::Instruction"*) #3 align 2 {
  %2 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"* %0, %"class.llvm::Instruction"** %2, align 8
  %3 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %2, align 8
  %4 = bitcast %"class.llvm::Instruction"* %3 to %"class.llvm::Value"*
  %5 = call %"class.llvm::Type"* @_ZNK4llvm5Value7getTypeEv(%"class.llvm::Value"* %4)
  %6 = call zeroext i1 @_ZNK4llvm4Type16isFPOrFPVectorTyEv(%"class.llvm::Type"* %5)
  br i1 %6, label %11, label %7

; <label>:7:                                      ; preds = %1
  %8 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %2, align 8
  %9 = call i32 @_ZNK4llvm11Instruction9getOpcodeEv(%"class.llvm::Instruction"* %8)
  %10 = icmp eq i32 %9, 52
  br label %11

; <label>:11:                                     ; preds = %7, %1
  %12 = phi i1 [ true, %1 ], [ %10, %7 ]
  ret i1 %12
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZNK4llvm4Type16isFPOrFPVectorTyEv(%"class.llvm::Type"*) #0 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call %"class.llvm::Type"* @_ZNK4llvm4Type13getScalarTypeEv(%"class.llvm::Type"* %3) #18
  %5 = call zeroext i1 @_ZNK4llvm4Type17isFloatingPointTyEv(%"class.llvm::Type"* %4)
  ret i1 %5
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i32 @_ZNK4llvm11Instruction9getOpcodeEv(%"class.llvm::Instruction"*) #4 align 2 {
  %2 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"* %0, %"class.llvm::Instruction"** %2, align 8
  %3 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %2, align 8
  %4 = bitcast %"class.llvm::Instruction"* %3 to %"class.llvm::Value"*
  %5 = call i32 @_ZNK4llvm5Value10getValueIDEv(%"class.llvm::Value"* %4)
  %6 = sub i32 %5, 24
  ret i32 %6
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZNK4llvm4Type17isFloatingPointTyEv(%"class.llvm::Type"*) #4 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call i32 @_ZNK4llvm4Type9getTypeIDEv(%"class.llvm::Type"* %3)
  %5 = icmp eq i32 %4, 1
  br i1 %5, label %21, label %6

; <label>:6:                                      ; preds = %1
  %7 = call i32 @_ZNK4llvm4Type9getTypeIDEv(%"class.llvm::Type"* %3)
  %8 = icmp eq i32 %7, 2
  br i1 %8, label %21, label %9

; <label>:9:                                      ; preds = %6
  %10 = call i32 @_ZNK4llvm4Type9getTypeIDEv(%"class.llvm::Type"* %3)
  %11 = icmp eq i32 %10, 3
  br i1 %11, label %21, label %12

; <label>:12:                                     ; preds = %9
  %13 = call i32 @_ZNK4llvm4Type9getTypeIDEv(%"class.llvm::Type"* %3)
  %14 = icmp eq i32 %13, 4
  br i1 %14, label %21, label %15

; <label>:15:                                     ; preds = %12
  %16 = call i32 @_ZNK4llvm4Type9getTypeIDEv(%"class.llvm::Type"* %3)
  %17 = icmp eq i32 %16, 5
  br i1 %17, label %21, label %18

; <label>:18:                                     ; preds = %15
  %19 = call i32 @_ZNK4llvm4Type9getTypeIDEv(%"class.llvm::Type"* %3)
  %20 = icmp eq i32 %19, 6
  br label %21

; <label>:21:                                     ; preds = %18, %15, %12, %9, %6, %1
  %22 = phi i1 [ true, %15 ], [ true, %12 ], [ true, %9 ], [ true, %6 ], [ true, %1 ], [ %20, %18 ]
  ret i1 %22
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr dereferenceable(8) %"class.llvm::CallInst"** @_ZN4llvm13simplify_typeIPNS_8CallInstEE18getSimplifiedValueERS2_(%"class.llvm::CallInst"** dereferenceable(8)) #4 align 2 {
  %2 = alloca %"class.llvm::CallInst"**, align 8
  store %"class.llvm::CallInst"** %0, %"class.llvm::CallInst"*** %2, align 8
  %3 = load %"class.llvm::CallInst"**, %"class.llvm::CallInst"*** %2, align 8
  ret %"class.llvm::CallInst"** %3
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm3isaINS_8CallInstEPNS_11InstructionEEEbRKT0_(%"class.llvm::Instruction"** dereferenceable(8)) #3 {
  %2 = alloca %"class.llvm::Instruction"**, align 8
  store %"class.llvm::Instruction"** %0, %"class.llvm::Instruction"*** %2, align 8
  %3 = load %"class.llvm::Instruction"**, %"class.llvm::Instruction"*** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_8CallInstEKPNS_11InstructionEPKS2_E4doitERS4_(%"class.llvm::Instruction"** dereferenceable(8) %3)
  ret i1 %4
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::CallInst"* @_ZN4llvm16cast_convert_valINS_8CallInstEPNS_11InstructionES3_E4doitERKS3_(%"class.llvm::Instruction"** dereferenceable(8)) #4 align 2 {
  %2 = alloca %"class.llvm::Instruction"**, align 8
  %3 = alloca %"class.llvm::CallInst"*, align 8
  store %"class.llvm::Instruction"** %0, %"class.llvm::Instruction"*** %2, align 8
  %4 = load %"class.llvm::Instruction"**, %"class.llvm::Instruction"*** %2, align 8
  %5 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %4, align 8
  %6 = bitcast %"class.llvm::Instruction"* %5 to %"class.llvm::CallInst"*
  store %"class.llvm::CallInst"* %6, %"class.llvm::CallInst"** %3, align 8
  %7 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %3, align 8
  ret %"class.llvm::CallInst"* %7
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_8CallInstEKPNS_11InstructionEPKS2_E4doitERS4_(%"class.llvm::Instruction"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Instruction"**, align 8
  %3 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"** %0, %"class.llvm::Instruction"*** %2, align 8
  %4 = load %"class.llvm::Instruction"**, %"class.llvm::Instruction"*** %2, align 8
  %5 = call %"class.llvm::Instruction"* @_ZN4llvm13simplify_typeIKPNS_11InstructionEE18getSimplifiedValueERS3_(%"class.llvm::Instruction"** dereferenceable(8) %4)
  store %"class.llvm::Instruction"* %5, %"class.llvm::Instruction"** %3, align 8
  %6 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_8CallInstEPKNS_11InstructionES4_E4doitERKS4_(%"class.llvm::Instruction"** dereferenceable(8) %3)
  ret i1 %6
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm13isa_impl_wrapINS_8CallInstEPKNS_11InstructionES4_E4doitERKS4_(%"class.llvm::Instruction"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Instruction"**, align 8
  store %"class.llvm::Instruction"** %0, %"class.llvm::Instruction"*** %2, align 8
  %3 = load %"class.llvm::Instruction"**, %"class.llvm::Instruction"*** %2, align 8
  %4 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %3, align 8
  %5 = call zeroext i1 @_ZN4llvm11isa_impl_clINS_8CallInstEPKNS_11InstructionEE4doitES4_(%"class.llvm::Instruction"* %4)
  ret i1 %5
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Instruction"* @_ZN4llvm13simplify_typeIKPNS_11InstructionEE18getSimplifiedValueERS3_(%"class.llvm::Instruction"** dereferenceable(8)) #0 align 2 {
  %2 = alloca %"class.llvm::Instruction"**, align 8
  store %"class.llvm::Instruction"** %0, %"class.llvm::Instruction"*** %2, align 8
  %3 = load %"class.llvm::Instruction"**, %"class.llvm::Instruction"*** %2, align 8
  %4 = call dereferenceable(8) %"class.llvm::Instruction"** @_ZN4llvm13simplify_typeIPNS_11InstructionEE18getSimplifiedValueERS2_(%"class.llvm::Instruction"** dereferenceable(8) %3)
  %5 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %4, align 8
  ret %"class.llvm::Instruction"* %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm11isa_impl_clINS_8CallInstEPKNS_11InstructionEE4doitES4_(%"class.llvm::Instruction"*) #3 align 2 {
  %2 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"* %0, %"class.llvm::Instruction"** %2, align 8
  %3 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %2, align 8
  %4 = icmp ne %"class.llvm::Instruction"* %3, null
  br i1 %4, label %5, label %6

; <label>:5:                                      ; preds = %1
  br label %6

; <label>:6:                                      ; preds = %5, %1
  %7 = phi i1 [ false, %1 ], [ true, %5 ]
  %8 = xor i1 %7, true
  br i1 %8, label %9, label %11

; <label>:9:                                      ; preds = %6
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.6, i32 0, i32 0), i32 95, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.7, i32 0, i32 0)) #15
  unreachable
                                                  ; No predecessors!
  br label %12

; <label>:11:                                     ; preds = %6
  br label %12

; <label>:12:                                     ; preds = %11, %10
  %13 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %2, align 8
  %14 = call zeroext i1 @_ZN4llvm8isa_implINS_8CallInstENS_11InstructionEvE4doitERKS2_(%"class.llvm::Instruction"* dereferenceable(64) %13)
  ret i1 %14
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm8isa_implINS_8CallInstENS_11InstructionEvE4doitERKS2_(%"class.llvm::Instruction"* dereferenceable(64)) #3 align 2 {
  %2 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"* %0, %"class.llvm::Instruction"** %2, align 8
  %3 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm8CallInst7classofEPKNS_11InstructionE(%"class.llvm::Instruction"* %3)
  ret i1 %4
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm8CallInst7classofEPKNS_11InstructionE(%"class.llvm::Instruction"*) #7 align 2 {
  %2 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"* %0, %"class.llvm::Instruction"** %2, align 8
  %3 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %2, align 8
  %4 = call i32 @_ZNK4llvm11Instruction9getOpcodeEv(%"class.llvm::Instruction"* %3)
  %5 = icmp eq i32 %4, 54
  ret i1 %5
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr dereferenceable(8) %"class.llvm::Instruction"** @_ZN4llvm13simplify_typeIPNS_11InstructionEE18getSimplifiedValueERS2_(%"class.llvm::Instruction"** dereferenceable(8)) #4 align 2 {
  %2 = alloca %"class.llvm::Instruction"**, align 8
  store %"class.llvm::Instruction"** %0, %"class.llvm::Instruction"*** %2, align 8
  %3 = load %"class.llvm::Instruction"**, %"class.llvm::Instruction"*** %2, align 8
  ret %"class.llvm::Instruction"** %3
}

declare void @_ZN4llvm11Instruction11setMetadataEjPNS_6MDNodeE(%"class.llvm::Instruction"*, i32, %"class.llvm::MDNode"*) #1

declare void @_ZN4llvm11Instruction16setFastMathFlagsENS_13FastMathFlagsE(%"class.llvm::Instruction"*, i32) #1

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm8ArrayRefIPNS_5ValueEEC2ERKSt16initializer_listIS2_E(%"class.llvm::ArrayRef.36"*, %"class.std::initializer_list"* dereferenceable(16)) unnamed_addr #4 align 2 {
  %3 = alloca %"class.std::initializer_list"*, align 8
  %4 = alloca %"class.std::initializer_list"*, align 8
  %5 = alloca %"class.std::initializer_list"*, align 8
  %6 = alloca %"class.std::initializer_list"*, align 8
  %7 = alloca %"class.llvm::ArrayRef.36"*, align 8
  %8 = alloca %"class.std::initializer_list"*, align 8
  store %"class.llvm::ArrayRef.36"* %0, %"class.llvm::ArrayRef.36"** %7, align 8
  store %"class.std::initializer_list"* %1, %"class.std::initializer_list"** %8, align 8
  %9 = load %"class.llvm::ArrayRef.36"*, %"class.llvm::ArrayRef.36"** %7, align 8
  %10 = getelementptr inbounds %"class.llvm::ArrayRef.36", %"class.llvm::ArrayRef.36"* %9, i32 0, i32 0
  %11 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %8, align 8
  store %"class.std::initializer_list"* %11, %"class.std::initializer_list"** %6, align 8
  %12 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %6, align 8
  %13 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %12, i32 0, i32 0
  %14 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %13, align 8
  %15 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %8, align 8
  store %"class.std::initializer_list"* %15, %"class.std::initializer_list"** %3, align 8
  %16 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %3, align 8
  %17 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %16, i32 0, i32 0
  %18 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %17, align 8
  %19 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %16, i32 0, i32 1
  %20 = load i64, i64* %19, align 8
  %21 = getelementptr inbounds %"class.llvm::Value"*, %"class.llvm::Value"** %18, i64 %20
  %22 = icmp eq %"class.llvm::Value"** %14, %21
  br i1 %22, label %23, label %24

; <label>:23:                                     ; preds = %2
  br label %29

; <label>:24:                                     ; preds = %2
  %25 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %8, align 8
  store %"class.std::initializer_list"* %25, %"class.std::initializer_list"** %4, align 8
  %26 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %4, align 8
  %27 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %26, i32 0, i32 0
  %28 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %27, align 8
  br label %29

; <label>:29:                                     ; preds = %24, %23
  %30 = phi %"class.llvm::Value"** [ null, %23 ], [ %28, %24 ]
  store %"class.llvm::Value"** %30, %"class.llvm::Value"*** %10, align 8
  %31 = getelementptr inbounds %"class.llvm::ArrayRef.36", %"class.llvm::ArrayRef.36"* %9, i32 0, i32 1
  %32 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %8, align 8
  store %"class.std::initializer_list"* %32, %"class.std::initializer_list"** %5, align 8
  %33 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %5, align 8
  %34 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %33, i32 0, i32 1
  %35 = load i64, i64* %34, align 8
  store i64 %35, i64* %31, align 8
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::AllocaInst"* @_ZNK4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE6InsertINS_10AllocaInstEEEPT_S7_RKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::AllocaInst"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
  %4 = alloca %"class.llvm::IRBuilder"*, align 8
  %5 = alloca %"class.llvm::AllocaInst"*, align 8
  %6 = alloca %"class.llvm::Twine"*, align 8
  %7 = alloca %"class.llvm::ilist_iterator", align 8
  store %"class.llvm::IRBuilder"* %0, %"class.llvm::IRBuilder"** %4, align 8
  store %"class.llvm::AllocaInst"* %1, %"class.llvm::AllocaInst"** %5, align 8
  store %"class.llvm::Twine"* %2, %"class.llvm::Twine"** %6, align 8
  %8 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %4, align 8
  %9 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderDefaultInserter"*
  %10 = load %"class.llvm::AllocaInst"*, %"class.llvm::AllocaInst"** %5, align 8
  %11 = bitcast %"class.llvm::AllocaInst"* %10 to %"class.llvm::Instruction"*
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
  %21 = load %"class.llvm::ilist_node_impl.0"*, %"class.llvm::ilist_node_impl.0"** %20, align 8
  call void @_ZNK4llvm24IRBuilderDefaultInserter12InsertHelperEPNS_11InstructionERKNS_5TwineEPNS_10BasicBlockENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEE(%"class.llvm::IRBuilderDefaultInserter"* %9, %"class.llvm::Instruction"* %11, %"class.llvm::Twine"* dereferenceable(24) %12, %"class.llvm::BasicBlock"* %15, %"class.llvm::ilist_node_impl.0"* %21)
  %22 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderBase"*
  %23 = load %"class.llvm::AllocaInst"*, %"class.llvm::AllocaInst"** %5, align 8
  %24 = bitcast %"class.llvm::AllocaInst"* %23 to %"class.llvm::Instruction"*
  call void @_ZNK4llvm13IRBuilderBase20SetInstDebugLocationEPNS_11InstructionE(%"class.llvm::IRBuilderBase"* %22, %"class.llvm::Instruction"* %24)
  %25 = load %"class.llvm::AllocaInst"*, %"class.llvm::AllocaInst"** %5, align 8
  ret %"class.llvm::AllocaInst"* %25
}

; Function Attrs: ssp uwtable
define linkonce_odr i8* @_ZN4llvm16UnaryInstructionnwEm(i64) #0 align 2 {
  %2 = alloca i64, align 8
  store i64 %0, i64* %2, align 8
  %3 = load i64, i64* %2, align 8
  %4 = call i8* @_ZN4llvm4UsernwEmj(i64 %3, i32 1)
  ret i8* %4
}

declare void @_ZN4llvm10AllocaInstC1EPNS_4TypeEPNS_5ValueERKNS_5TwineEPNS_11InstructionE(%"class.llvm::AllocaInst"*, %"class.llvm::Type"*, %"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::Instruction"*) unnamed_addr #1

attributes #0 = { ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { inlinehint ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { noreturn "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { argmemonly nounwind }
attributes #7 = { inlinehint nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { noinline noreturn nounwind }
attributes #9 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #10 = { nobuiltin "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #11 = { nobuiltin nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #12 = { nounwind readnone }
attributes #13 = { nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #14 = { nounwind }
attributes #15 = { noreturn }
attributes #16 = { noreturn nounwind }
attributes #17 = { builtin nounwind }
attributes #18 = { nounwind readonly }
attributes #19 = { builtin }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"Apple LLVM version 8.1.0 (clang-802.0.42)"}
