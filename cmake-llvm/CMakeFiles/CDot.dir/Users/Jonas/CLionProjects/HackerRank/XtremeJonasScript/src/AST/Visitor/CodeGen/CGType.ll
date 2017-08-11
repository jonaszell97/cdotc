; ModuleID = '/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/AST/Visitor/CodeGen/CGType.cpp'
source_filename = "/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/AST/Visitor/CodeGen/CGType.cpp"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.12.0"

%"class.std::__1::unordered_map" = type { %"class.std::__1::__hash_table" }
%"class.std::__1::__hash_table" = type <{ %"class.std::__1::unique_ptr", %"class.std::__1::__compressed_pair.2", %"class.std::__1::__compressed_pair.6", %"class.std::__1::__compressed_pair.8", [4 x i8] }>
%"class.std::__1::unique_ptr" = type { %"class.std::__1::__compressed_pair" }
%"class.std::__1::__compressed_pair" = type { %"class.std::__1::__libcpp_compressed_pair_imp" }
%"class.std::__1::__libcpp_compressed_pair_imp" = type { %"struct.std::__1::__hash_node_base"**, %"class.std::__1::__bucket_list_deallocator" }
%"struct.std::__1::__hash_node_base" = type { %"struct.std::__1::__hash_node_base"* }
%"class.std::__1::__bucket_list_deallocator" = type { %"class.std::__1::__compressed_pair.0" }
%"class.std::__1::__compressed_pair.0" = type { %"class.std::__1::__libcpp_compressed_pair_imp.1" }
%"class.std::__1::__libcpp_compressed_pair_imp.1" = type { i64 }
%"class.std::__1::__compressed_pair.2" = type { %"class.std::__1::__libcpp_compressed_pair_imp.3" }
%"class.std::__1::__libcpp_compressed_pair_imp.3" = type { %"struct.std::__1::__hash_node_base" }
%"class.std::__1::__compressed_pair.6" = type { %"class.std::__1::__libcpp_compressed_pair_imp.7" }
%"class.std::__1::__libcpp_compressed_pair_imp.7" = type { i64 }
%"class.std::__1::__compressed_pair.8" = type { %"class.std::__1::__libcpp_compressed_pair_imp.9" }
%"class.std::__1::__libcpp_compressed_pair_imp.9" = type { float }
%"class.std::__1::unordered_map.10" = type { %"class.std::__1::__hash_table.11" }
%"class.std::__1::__hash_table.11" = type <{ %"class.std::__1::unique_ptr.12", %"class.std::__1::__compressed_pair.21", %"class.std::__1::__compressed_pair.25", %"class.std::__1::__compressed_pair.30", [4 x i8] }>
%"class.std::__1::unique_ptr.12" = type { %"class.std::__1::__compressed_pair.13" }
%"class.std::__1::__compressed_pair.13" = type { %"class.std::__1::__libcpp_compressed_pair_imp.14" }
%"class.std::__1::__libcpp_compressed_pair_imp.14" = type { %"struct.std::__1::__hash_node_base.15"**, %"class.std::__1::__bucket_list_deallocator.16" }
%"struct.std::__1::__hash_node_base.15" = type { %"struct.std::__1::__hash_node_base.15"* }
%"class.std::__1::__bucket_list_deallocator.16" = type { %"class.std::__1::__compressed_pair.17" }
%"class.std::__1::__compressed_pair.17" = type { %"class.std::__1::__libcpp_compressed_pair_imp.18" }
%"class.std::__1::__libcpp_compressed_pair_imp.18" = type { i64 }
%"class.std::__1::__compressed_pair.21" = type { %"class.std::__1::__libcpp_compressed_pair_imp.22" }
%"class.std::__1::__libcpp_compressed_pair_imp.22" = type { %"struct.std::__1::__hash_node_base.15" }
%"class.std::__1::__compressed_pair.25" = type { %"class.std::__1::__libcpp_compressed_pair_imp.26" }
%"class.std::__1::__libcpp_compressed_pair_imp.26" = type { i64 }
%"class.std::__1::__compressed_pair.30" = type { %"class.std::__1::__libcpp_compressed_pair_imp.31" }
%"class.std::__1::__libcpp_compressed_pair_imp.31" = type { float }
%"class.llvm::IRBuilder" = type <{ %"class.llvm::IRBuilderBase", %"class.llvm::ConstantFolder", [7 x i8] }>
%"class.llvm::IRBuilderBase" = type { %"class.llvm::DebugLoc", %"class.llvm::BasicBlock"*, %"class.llvm::ilist_iterator", %"class.llvm::LLVMContext"*, %"class.llvm::MDNode"*, %"class.llvm::FastMathFlags", %"class.llvm::ArrayRef.108" }
%"class.llvm::DebugLoc" = type { %"class.llvm::TypedTrackingMDRef" }
%"class.llvm::TypedTrackingMDRef" = type { %"class.llvm::TrackingMDRef" }
%"class.llvm::TrackingMDRef" = type { %"class.llvm::Metadata"* }
%"class.llvm::Metadata" = type { i8, i8, i16, i32 }
%"class.llvm::BasicBlock" = type { %"class.llvm::Value", %"class.llvm::ilist_node_with_parent", %"class.llvm::SymbolTableList", %"class.llvm::Function"* }
%"class.llvm::Value" = type { i32 (...)**, %"class.llvm::Type"*, %"class.llvm::Use"*, i8, i8, i16, i32 }
%"class.llvm::Type" = type { %"class.llvm::LLVMContext"*, i32, i32, %"class.llvm::Type"** }
%"class.llvm::Use" = type { %"class.llvm::Value"*, %"class.llvm::Use"*, %"class.llvm::PointerIntPair" }
%"class.llvm::PointerIntPair" = type { i64 }
%"class.llvm::ilist_node_with_parent" = type { %"class.llvm::ilist_node" }
%"class.llvm::ilist_node" = type { %"class.llvm::ilist_node_impl" }
%"class.llvm::ilist_node_impl" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::ilist_node_base" = type { %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"* }
%"class.llvm::SymbolTableList" = type { %"class.llvm::iplist_impl" }
%"class.llvm::iplist_impl" = type { %"class.llvm::simple_ilist" }
%"class.llvm::simple_ilist" = type { %"class.llvm::ilist_sentinel" }
%"class.llvm::ilist_sentinel" = type { %"class.llvm::ilist_node_impl.84" }
%"class.llvm::ilist_node_impl.84" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::Function" = type { %"class.llvm::GlobalObject", %"class.llvm::ilist_node.85", %"class.llvm::SymbolTableList.87", %"class.llvm::SymbolTableList.94", %"class.std::__1::unique_ptr.102", %"class.llvm::AttributeSet" }
%"class.llvm::GlobalObject" = type { %"class.llvm::GlobalValue", %"class.llvm::Comdat"* }
%"class.llvm::GlobalValue" = type { %"class.llvm::Constant", %"class.llvm::Type"*, i32, i32, %"class.llvm::Module"* }
%"class.llvm::Constant" = type { %"class.llvm::User" }
%"class.llvm::User" = type { %"class.llvm::Value" }
%"class.llvm::Module" = type opaque
%"class.llvm::Comdat" = type opaque
%"class.llvm::ilist_node.85" = type { %"class.llvm::ilist_node_impl.86" }
%"class.llvm::ilist_node_impl.86" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::SymbolTableList.87" = type { %"class.llvm::iplist_impl.88" }
%"class.llvm::iplist_impl.88" = type { %"class.llvm::simple_ilist.91" }
%"class.llvm::simple_ilist.91" = type { %"class.llvm::ilist_sentinel.93" }
%"class.llvm::ilist_sentinel.93" = type { %"class.llvm::ilist_node_impl" }
%"class.llvm::SymbolTableList.94" = type { %"class.llvm::iplist_impl.95" }
%"class.llvm::iplist_impl.95" = type { %"class.llvm::simple_ilist.98" }
%"class.llvm::simple_ilist.98" = type { %"class.llvm::ilist_sentinel.100" }
%"class.llvm::ilist_sentinel.100" = type { %"class.llvm::ilist_node_impl.101" }
%"class.llvm::ilist_node_impl.101" = type { %"class.llvm::ilist_node_base" }
%"class.std::__1::unique_ptr.102" = type { %"class.std::__1::__compressed_pair.103" }
%"class.std::__1::__compressed_pair.103" = type { %"class.std::__1::__libcpp_compressed_pair_imp.104" }
%"class.std::__1::__libcpp_compressed_pair_imp.104" = type { %"class.llvm::ValueSymbolTable"* }
%"class.llvm::ValueSymbolTable" = type opaque
%"class.llvm::AttributeSet" = type { %"class.llvm::AttributeSetImpl"* }
%"class.llvm::AttributeSetImpl" = type opaque
%"class.llvm::ilist_iterator" = type { %"class.llvm::ilist_node_impl.84"* }
%"class.llvm::LLVMContext" = type { %"class.llvm::LLVMContextImpl"* }
%"class.llvm::LLVMContextImpl" = type opaque
%"class.llvm::MDNode" = type { %"class.llvm::Metadata", i32, i32, %"class.llvm::ContextAndReplaceableUses" }
%"class.llvm::ContextAndReplaceableUses" = type { %"class.llvm::PointerUnion" }
%"class.llvm::PointerUnion" = type { %"class.llvm::PointerIntPair.107" }
%"class.llvm::PointerIntPair.107" = type { i64 }
%"class.llvm::FastMathFlags" = type { i32 }
%"class.llvm::ArrayRef.108" = type { %"class.llvm::OperandBundleDefT"*, i64 }
%"class.llvm::OperandBundleDefT" = type { %"class.std::__1::basic_string", %"class.std::__1::vector.109" }
%"class.std::__1::basic_string" = type { %"class.std::__1::__compressed_pair.36" }
%"class.std::__1::__compressed_pair.36" = type { %"class.std::__1::__libcpp_compressed_pair_imp.37" }
%"class.std::__1::__libcpp_compressed_pair_imp.37" = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" = type { %union.anon }
%union.anon = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" = type { i64, i64, i8* }
%"class.std::__1::vector.109" = type { %"class.std::__1::__vector_base.110" }
%"class.std::__1::__vector_base.110" = type { %"class.llvm::Value"**, %"class.llvm::Value"**, %"class.std::__1::__compressed_pair.111" }
%"class.std::__1::__compressed_pair.111" = type { %"class.std::__1::__libcpp_compressed_pair_imp.112" }
%"class.std::__1::__libcpp_compressed_pair_imp.112" = type { %"class.llvm::Value"** }
%"class.llvm::ConstantFolder" = type { i8 }
%"struct.std::__1::piecewise_construct_t" = type { i8 }
%"class.std::__1::__unordered_map_equal" = type { i8 }
%"class.std::__1::__unordered_map_hasher" = type { i8 }
%"class.std::__1::allocator" = type { i8 }
%"class.std::__1::allocator.4" = type { i8 }
%"struct.std::__1::hash" = type { i8 }
%"struct.std::__1::equal_to" = type { i8 }
%"class.std::__1::__unordered_map_equal.32" = type { i8 }
%"class.std::__1::__unordered_map_hasher.27" = type { i8 }
%"class.std::__1::allocator.19" = type { i8 }
%"class.std::__1::allocator.23" = type { i8 }
%"struct.std::__1::hash.28" = type { i8 }
%"struct.std::__1::equal_to.33" = type { i8 }
%"class.llvm::FunctionType" = type { %"class.llvm::Type" }
%struct.TypeSpecifier = type { i32, %"class.std::__1::basic_string", %"class.std::__1::shared_ptr", i8, i8, i8, i8, i64, i32, i8, i8, %"class.std::__1::basic_string", i8, [7 x i8], %"class.std::__1::unordered_map.41", i8, i8, i8, i8, i8, i8, i8, i32, %"class.std::__1::vector", %"class.std::__1::vector.67", %union.anon.73 }
%"class.std::__1::shared_ptr" = type { %class.Expression*, %"class.std::__1::__shared_weak_count"* }
%class.Expression = type { %class.Statement.base, %"class.std::__1::shared_ptr", i8, %"class.std::__1::shared_ptr", i8, %"class.std::__1::unique_ptr.74", i8, %struct.TypeSpecifier }
%class.Statement.base = type { %class.AstNode.base }
%class.AstNode.base = type <{ i32 (...)**, i32, i32, i64, %class.AstNode*, %class.DeclStmt*, i8, [3 x i8], i32, %"class.std::__1::vector", %struct.TypeSpecifier, %struct.TypeSpecifier, i8, [7 x i8], %"class.std::__1::basic_string", i8, i8 }>
%class.AstNode = type <{ i32 (...)**, i32, i32, i64, %class.AstNode*, %class.DeclStmt*, i8, [3 x i8], i32, %"class.std::__1::vector", %struct.TypeSpecifier, %struct.TypeSpecifier, i8, [7 x i8], %"class.std::__1::basic_string", i8, i8, [6 x i8] }>
%class.DeclStmt = type opaque
%"class.std::__1::unique_ptr.74" = type { %"class.std::__1::__compressed_pair.75" }
%"class.std::__1::__compressed_pair.75" = type { %"class.std::__1::__libcpp_compressed_pair_imp.76" }
%"class.std::__1::__libcpp_compressed_pair_imp.76" = type { %struct.Variant* }
%struct.Variant = type <{ %union.anon.77, i8, [7 x i8], %"class.std::__1::basic_string", %struct.TypeSpecifier, i8, i8, i8, [5 x i8] }>
%union.anon.77 = type { i64 }
%"class.std::__1::__shared_weak_count" = type { %"class.std::__1::__shared_count", i64 }
%"class.std::__1::__shared_count" = type { i32 (...)**, i64 }
%"class.std::__1::unordered_map.41" = type { %"class.std::__1::__hash_table.42" }
%"class.std::__1::__hash_table.42" = type <{ %"class.std::__1::unique_ptr.43", %"class.std::__1::__compressed_pair.52", %"class.std::__1::__compressed_pair.56", %"class.std::__1::__compressed_pair.59", [4 x i8] }>
%"class.std::__1::unique_ptr.43" = type { %"class.std::__1::__compressed_pair.44" }
%"class.std::__1::__compressed_pair.44" = type { %"class.std::__1::__libcpp_compressed_pair_imp.45" }
%"class.std::__1::__libcpp_compressed_pair_imp.45" = type { %"struct.std::__1::__hash_node_base.46"**, %"class.std::__1::__bucket_list_deallocator.47" }
%"struct.std::__1::__hash_node_base.46" = type { %"struct.std::__1::__hash_node_base.46"* }
%"class.std::__1::__bucket_list_deallocator.47" = type { %"class.std::__1::__compressed_pair.48" }
%"class.std::__1::__compressed_pair.48" = type { %"class.std::__1::__libcpp_compressed_pair_imp.49" }
%"class.std::__1::__libcpp_compressed_pair_imp.49" = type { i64 }
%"class.std::__1::__compressed_pair.52" = type { %"class.std::__1::__libcpp_compressed_pair_imp.53" }
%"class.std::__1::__libcpp_compressed_pair_imp.53" = type { %"struct.std::__1::__hash_node_base.46" }
%"class.std::__1::__compressed_pair.56" = type { %"class.std::__1::__libcpp_compressed_pair_imp.57" }
%"class.std::__1::__libcpp_compressed_pair_imp.57" = type { i64 }
%"class.std::__1::__compressed_pair.59" = type { %"class.std::__1::__libcpp_compressed_pair_imp.60" }
%"class.std::__1::__libcpp_compressed_pair_imp.60" = type { float }
%"class.std::__1::vector" = type { %"class.std::__1::__vector_base" }
%"class.std::__1::__vector_base" = type { %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"*, %"class.std::__1::__compressed_pair.63" }
%"class.std::__1::__compressed_pair.63" = type { %"class.std::__1::__libcpp_compressed_pair_imp.64" }
%"class.std::__1::__libcpp_compressed_pair_imp.64" = type { %"class.std::__1::basic_string"* }
%"class.std::__1::vector.67" = type { %"class.std::__1::__vector_base.68" }
%"class.std::__1::__vector_base.68" = type { %struct.TypeSpecifier*, %struct.TypeSpecifier*, %"class.std::__1::__compressed_pair.69" }
%"class.std::__1::__compressed_pair.69" = type { %"class.std::__1::__libcpp_compressed_pair_imp.70" }
%"class.std::__1::__libcpp_compressed_pair_imp.70" = type { %struct.TypeSpecifier* }
%union.anon.73 = type { %struct.TypeSpecifier* }
%"class.std::__1::vector.78" = type { %"class.std::__1::__vector_base.79" }
%"class.std::__1::__vector_base.79" = type { %"class.llvm::Type"**, %"class.llvm::Type"**, %"class.std::__1::__compressed_pair.80" }
%"class.std::__1::__compressed_pair.80" = type { %"class.std::__1::__libcpp_compressed_pair_imp.81" }
%"class.std::__1::__libcpp_compressed_pair_imp.81" = type { %"class.llvm::Type"** }
%"class.llvm::ArrayRef" = type { %"class.llvm::Type"**, i64 }
%"class.llvm::PointerType" = type { %"class.llvm::Type", %"class.llvm::Type"* }
%"class.std::__1::__wrap_iter" = type { %struct.TypeSpecifier* }
%"class.std::__1::allocator.82" = type { i8 }
%"struct.std::__1::integral_constant.128" = type { i8 }
%"struct.std::__1::__has_construct.154" = type { i8 }
%"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator" = type { i8 }
%"class.std::initializer_list" = type { %"class.llvm::Type"**, i64 }
%"class.std::__1::__vector_base_common" = type { i8 }
%"class.llvm::StructType" = type { %"class.llvm::CompositeType", i8* }
%"class.llvm::CompositeType" = type { %"class.llvm::Type" }
%"class.llvm::IntegerType" = type { %"class.llvm::Type" }
%"class.std::__1::__basic_string_common" = type { i8 }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw" = type { [3 x i64] }
%"class.std::__1::tuple.131" = type { i8 }
%"union.std::__1::__hash_value_type.124" = type { %"struct.std::__1::pair.125" }
%"struct.std::__1::pair.125" = type { i32, %"class.llvm::StructType"* }
%"class.std::__1::tuple.141" = type { %"struct.std::__1::__tuple_impl.142" }
%"struct.std::__1::__tuple_impl.142" = type { %"class.std::__1::__tuple_leaf.143" }
%"class.std::__1::__tuple_leaf.143" = type { i32* }
%"struct.std::__1::__less" = type { i8 }
%"class.std::__1::__libcpp_compressed_pair_imp.146" = type { %"struct.std::__1::__hash_node.123"*, %"class.std::__1::__hash_node_destructor.147" }
%"struct.std::__1::__hash_node.123" = type { %"struct.std::__1::__hash_node_base.15", i64, %"union.std::__1::__hash_value_type.124" }
%"class.std::__1::__hash_node_destructor.147" = type <{ %"class.std::__1::allocator.23"*, i8, [7 x i8] }>
%"class.std::__1::__compressed_pair.145" = type { %"class.std::__1::__libcpp_compressed_pair_imp.146" }
%"struct.std::__1::integral_constant" = type { i8 }
%"struct.std::__1::__has_destroy.127" = type { i8 }
%"class.std::__1::unique_ptr.144" = type { %"class.std::__1::__compressed_pair.145" }
%"class.std::__1::__hash_iterator.139" = type { %"struct.std::__1::__hash_node_base.15"* }
%"struct.std::__1::pair.138" = type <{ %"class.std::__1::__hash_iterator.139", i8, [7 x i8] }>
%"struct.std::__1::__tuple_indices" = type { i8 }
%"struct.std::__1::__tuple_types.150" = type { i8 }
%"struct.std::__1::__tuple_indices.136" = type { i8 }
%"struct.std::__1::__tuple_types.137" = type { i8 }
%"struct.std::__1::__split_buffer" = type { %"class.llvm::Type"**, %"class.llvm::Type"**, %"class.llvm::Type"**, %"class.std::__1::__compressed_pair.152" }
%"class.std::__1::__compressed_pair.152" = type { %"class.std::__1::__libcpp_compressed_pair_imp.153" }
%"class.std::__1::__libcpp_compressed_pair_imp.153" = type { %"class.llvm::Type"**, %"class.std::__1::allocator.82"* }
%"union.std::__1::__hash_value_type" = type { %"struct.std::__1::pair" }
%"struct.std::__1::pair" = type { %"class.std::__1::basic_string", %"class.llvm::StructType"* }
%"class.std::__1::tuple" = type { %"struct.std::__1::__tuple_impl" }
%"struct.std::__1::__tuple_impl" = type { %"class.std::__1::__tuple_leaf" }
%"class.std::__1::__tuple_leaf" = type { %"class.std::__1::basic_string"* }
%"class.std::__1::__libcpp_compressed_pair_imp.134" = type { %"struct.std::__1::__hash_node"*, %"class.std::__1::__hash_node_destructor" }
%"struct.std::__1::__hash_node" = type { %"struct.std::__1::__hash_node_base", i64, %"union.std::__1::__hash_value_type" }
%"class.std::__1::__hash_node_destructor" = type <{ %"class.std::__1::allocator.4"*, i8, [7 x i8] }>
%"class.std::__1::__compressed_pair.133" = type { %"class.std::__1::__libcpp_compressed_pair_imp.134" }
%"struct.std::__1::__has_destroy" = type { i8 }
%"class.std::__1::unique_ptr.132" = type { %"class.std::__1::__compressed_pair.133" }
%"class.std::__1::__hash_iterator" = type { %"struct.std::__1::__hash_node_base"* }
%"struct.std::__1::pair.130" = type <{ %"class.std::__1::__hash_iterator", i8, [7 x i8] }>
%"struct.std::__1::__tuple_types" = type { i8 }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short" = type { %union.anon.40, [23 x i8] }
%union.anon.40 = type { i8 }
%"class.llvm::APInt" = type { i32, %union.anon.115 }
%union.anon.115 = type { i64 }
%"class.llvm::APFloat" = type { [8 x i8], %"union.llvm::APFloat::Storage" }
%"union.llvm::APFloat::Storage" = type { %"class.llvm::detail::DoubleAPFloat", [8 x i8] }
%"class.llvm::detail::DoubleAPFloat" = type { %"struct.llvm::fltSemantics"*, %"class.std::__1::unique_ptr.116" }
%"struct.llvm::fltSemantics" = type opaque
%"class.std::__1::unique_ptr.116" = type { %"class.std::__1::__compressed_pair.117" }
%"class.std::__1::__compressed_pair.117" = type { %"class.std::__1::__libcpp_compressed_pair_imp.118" }
%"class.std::__1::__libcpp_compressed_pair_imp.118" = type { %"class.llvm::APFloat"* }
%"class.llvm::ConstantInt" = type { %"class.llvm::ConstantData", %"class.llvm::APInt" }
%"class.llvm::ConstantData" = type { %"class.llvm::Constant" }
%"class.llvm::ConstantFP" = type { %"class.llvm::ConstantData", %"class.llvm::APFloat" }
%"class.llvm::ConstantPointerNull" = type { %"class.llvm::ConstantData" }
%"class.llvm::ArrayRef.121" = type { %"class.llvm::Constant"**, i64 }
%"class.llvm::ArrayType" = type { %"class.llvm::SequentialType" }
%"class.llvm::SequentialType" = type { %"class.llvm::CompositeType", %"class.llvm::Type"*, i64 }
%"struct.std::__1::__has_destroy.129" = type { i8 }
%"class.llvm::detail::IEEEFloat" = type <{ %"struct.llvm::fltSemantics"*, %"union.llvm::detail::IEEEFloat::Significand", i16, i8, [5 x i8] }>
%"union.llvm::detail::IEEEFloat::Significand" = type { i64 }
%"struct.llvm::APFloatBase" = type { i8 }
%"struct.std::__1::default_delete.119" = type { i8 }
%"struct.std::__1::__has_construct" = type { i8 }
%"struct.std::__1::__murmur2_or_cityhash" = type { i8 }
%"struct.std::__1::pair.135" = type { i64, i64 }
%"struct.std::__1::__has_construct.149" = type { i8 }
%"struct.std::__1::__has_construct.151" = type { i8 }
%"struct.std::__1::__has_max_size" = type { i8 }
%"class.std::__1::__split_buffer_common" = type { i8 }

@_ZN4llvm24DisableABIBreakingChecksE = external global i32, align 4
@_ZN4llvm30VerifyDisableABIBreakingChecksE = weak hidden global i32* @_ZN4llvm24DisableABIBreakingChecksE, align 8
@_ZN6CGType14StructureTypesE = global %"class.std::__1::unordered_map" zeroinitializer, align 8
@__dso_handle = external global i8
@_ZN6CGType8EnvTypesE = global %"class.std::__1::unordered_map.10" zeroinitializer, align 8
@_ZN14CodeGenVisitor7BuilderE = external global %"class.llvm::IRBuilder", align 8
@_ZN14CodeGenVisitor7ContextE = external global %"class.llvm::LLVMContext", align 8
@.str = private unnamed_addr constant [18 x i8] c"Unsupported type \00", align 1
@.str.2 = private unnamed_addr constant [26 x i8] c"Unsupported literal type \00", align 1
@.str.3 = private unnamed_addr constant [19 x i8] c"bitwidth too small\00", align 1
@__func__._ZN4llvm5APIntC2Ejyb = private unnamed_addr constant [6 x i8] c"APInt\00", align 1
@.str.4 = private unnamed_addr constant [56 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/ADT/APInt.h\00", align 1
@.str.5 = private unnamed_addr constant [33 x i8] c"BitWidth && \22bitwidth too small\22\00", align 1
@.str.6 = private unnamed_addr constant [21 x i8] c"Unexpected semantics\00", align 1
@.str.7 = private unnamed_addr constant [58 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/ADT/APFloat.h\00", align 1
@_ZNSt3__1L19piecewise_constructE = internal constant %"struct.std::__1::piecewise_construct_t" undef, align 1
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_CGType.cpp, i8* null }]

; Function Attrs: ssp uwtable
define internal void @__cxx_global_var_init() #0 section "__TEXT,__StaticInit,regular,pure_instructions" personality i32 (...)* @__gxx_personality_v0 {
  %1 = alloca float*, align 8
  %2 = alloca %"class.std::__1::__unordered_map_equal"*, align 8
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.9"*, align 8
  %4 = alloca float, align 4
  %5 = alloca float*, align 8
  %6 = alloca %"class.std::__1::__compressed_pair.8"*, align 8
  %7 = alloca float, align 4
  %8 = alloca %"class.std::__1::__compressed_pair.8"*, align 8
  %9 = alloca float, align 4
  %10 = alloca i64*, align 8
  %11 = alloca %"class.std::__1::__unordered_map_hasher"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.7"*, align 8
  %13 = alloca i64, align 8
  %14 = alloca i64*, align 8
  %15 = alloca %"class.std::__1::__compressed_pair.6"*, align 8
  %16 = alloca i64, align 8
  %17 = alloca %"class.std::__1::__compressed_pair.6"*, align 8
  %18 = alloca i64, align 8
  %19 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %20 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %23 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %24 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %25 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %26 = alloca i8*, align 8
  %27 = alloca %"class.std::__1::allocator"*, align 8
  %28 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %29 = alloca i64, align 8
  %30 = alloca %"class.std::__1::allocator"*, align 8
  %31 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %32 = alloca i64, align 8
  %33 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %34 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %35 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %36 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %37 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %38 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %39 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %40 = alloca %"class.std::__1::unique_ptr"*, align 8
  %41 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %42 = alloca %"class.std::__1::unique_ptr"*, align 8
  %43 = alloca %"class.std::__1::unique_ptr"*, align 8
  %44 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %45 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %46 = alloca %"class.std::__1::allocator.4"*, align 8
  %47 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %48 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %49 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %50 = alloca i64*, align 8
  %51 = alloca %"class.std::__1::allocator"*, align 8
  %52 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %53 = alloca i64, align 8
  %54 = alloca i64*, align 8
  %55 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %56 = alloca i64, align 8
  %57 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %58 = alloca i64, align 8
  %59 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %60 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %61 = alloca %"struct.std::__1::__hash_node_base"***, align 8
  %62 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %63 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %64 = alloca %"struct.std::__1::__hash_node_base"***, align 8
  %65 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %66 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %67 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %68 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %69 = alloca %"class.std::__1::unique_ptr"*, align 8
  %70 = alloca %"class.std::__1::unique_ptr"*, align 8
  %71 = alloca %"class.std::__1::__hash_table"*, align 8
  %72 = alloca i8*
  %73 = alloca i32
  %74 = alloca %"class.std::__1::__hash_table"*, align 8
  %75 = alloca %"class.std::__1::unordered_map"*, align 8
  %76 = alloca %"class.std::__1::unordered_map"*, align 8
  store %"class.std::__1::unordered_map"* @_ZN6CGType14StructureTypesE, %"class.std::__1::unordered_map"** %76, align 8
  %77 = load %"class.std::__1::unordered_map"*, %"class.std::__1::unordered_map"** %76, align 8
  store %"class.std::__1::unordered_map"* %77, %"class.std::__1::unordered_map"** %75, align 8
  %78 = load %"class.std::__1::unordered_map"*, %"class.std::__1::unordered_map"** %75, align 8
  %79 = getelementptr inbounds %"class.std::__1::unordered_map", %"class.std::__1::unordered_map"* %78, i32 0, i32 0
  store %"class.std::__1::__hash_table"* %79, %"class.std::__1::__hash_table"** %74, align 8
  %80 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %74, align 8
  store %"class.std::__1::__hash_table"* %80, %"class.std::__1::__hash_table"** %71, align 8
  %81 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %71, align 8
  %82 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %81, i32 0, i32 0
  store %"class.std::__1::unique_ptr"* %82, %"class.std::__1::unique_ptr"** %70, align 8
  %83 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %70, align 8
  store %"class.std::__1::unique_ptr"* %83, %"class.std::__1::unique_ptr"** %69, align 8
  %84 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %69, align 8
  %85 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %84, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %85, %"class.std::__1::__compressed_pair"** %67, align 8
  store %"struct.std::__1::__hash_node_base"** null, %"struct.std::__1::__hash_node_base"*** %68, align 8
  %86 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %67, align 8
  %87 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %68, align 8
  store %"class.std::__1::__compressed_pair"* %86, %"class.std::__1::__compressed_pair"** %65, align 8
  store %"struct.std::__1::__hash_node_base"** %87, %"struct.std::__1::__hash_node_base"*** %66, align 8
  %88 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %65, align 8
  %89 = bitcast %"class.std::__1::__compressed_pair"* %88 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"struct.std::__1::__hash_node_base"*** %66, %"struct.std::__1::__hash_node_base"**** %64, align 8
  %90 = load %"struct.std::__1::__hash_node_base"***, %"struct.std::__1::__hash_node_base"**** %64, align 8
  %91 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %90, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %89, %"class.std::__1::__libcpp_compressed_pair_imp"** %62, align 8
  store %"struct.std::__1::__hash_node_base"** %91, %"struct.std::__1::__hash_node_base"*** %63, align 8
  %92 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %62, align 8
  %93 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %92, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"*** %63, %"struct.std::__1::__hash_node_base"**** %61, align 8
  %94 = load %"struct.std::__1::__hash_node_base"***, %"struct.std::__1::__hash_node_base"**** %61, align 8
  %95 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %94, align 8
  store %"struct.std::__1::__hash_node_base"** %95, %"struct.std::__1::__hash_node_base"*** %93, align 8
  %96 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %92, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator"* %96, %"class.std::__1::__bucket_list_deallocator"** %60, align 8
  %97 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %60, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %97, %"class.std::__1::__bucket_list_deallocator"** %59, align 8
  %98 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %59, align 8
  %99 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %98, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.0"* %99, %"class.std::__1::__compressed_pair.0"** %57, align 8
  store i64 0, i64* %58, align 8
  %100 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %57, align 8
  %101 = load i64, i64* %58, align 8
  store %"class.std::__1::__compressed_pair.0"* %100, %"class.std::__1::__compressed_pair.0"** %55, align 8
  store i64 %101, i64* %56, align 8
  %102 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %55, align 8
  %103 = bitcast %"class.std::__1::__compressed_pair.0"* %102 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store i64* %56, i64** %54, align 8
  %104 = load i64*, i64** %54, align 8
  %105 = load i64, i64* %104, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %103, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %52, align 8
  store i64 %105, i64* %53, align 8
  %106 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %52, align 8
  %107 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.1"* %106 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %107, %"class.std::__1::allocator"** %51, align 8
  %108 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %51, align 8
  %109 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.1", %"class.std::__1::__libcpp_compressed_pair_imp.1"* %106, i32 0, i32 0
  store i64* %53, i64** %50, align 8
  %110 = load i64*, i64** %50, align 8
  %111 = load i64, i64* %110, align 8
  store i64 %111, i64* %109, align 8
  %112 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %81, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.2"* %112, %"class.std::__1::__compressed_pair.2"** %49, align 8
  %113 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %49, align 8
  store %"class.std::__1::__compressed_pair.2"* %113, %"class.std::__1::__compressed_pair.2"** %48, align 8
  %114 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %48, align 8
  %115 = bitcast %"class.std::__1::__compressed_pair.2"* %114 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %115, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %47, align 8
  %116 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %47, align 8
  %117 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.3"* %116 to %"class.std::__1::allocator.4"*
  store %"class.std::__1::allocator.4"* %117, %"class.std::__1::allocator.4"** %46, align 8
  %118 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %46, align 8
  %119 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %116, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %119, %"struct.std::__1::__hash_node_base"** %45, align 8
  %120 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %45, align 8
  store %"struct.std::__1::__hash_node_base"* %120, %"struct.std::__1::__hash_node_base"** %44, align 8
  %121 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %44, align 8
  %122 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %121, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* null, %"struct.std::__1::__hash_node_base"** %122, align 8
  %123 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %81, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.6"* %123, %"class.std::__1::__compressed_pair.6"** %17, align 8
  store i64 0, i64* %18, align 8
  %124 = load %"class.std::__1::__compressed_pair.6"*, %"class.std::__1::__compressed_pair.6"** %17, align 8
  %125 = load i64, i64* %18, align 8
  store %"class.std::__1::__compressed_pair.6"* %124, %"class.std::__1::__compressed_pair.6"** %15, align 8
  store i64 %125, i64* %16, align 8
  %126 = load %"class.std::__1::__compressed_pair.6"*, %"class.std::__1::__compressed_pair.6"** %15, align 8
  %127 = bitcast %"class.std::__1::__compressed_pair.6"* %126 to %"class.std::__1::__libcpp_compressed_pair_imp.7"*
  store i64* %16, i64** %14, align 8
  %128 = load i64*, i64** %14, align 8
  %129 = load i64, i64* %128, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.7"* %127, %"class.std::__1::__libcpp_compressed_pair_imp.7"** %12, align 8
  store i64 %129, i64* %13, align 8
  %130 = load %"class.std::__1::__libcpp_compressed_pair_imp.7"*, %"class.std::__1::__libcpp_compressed_pair_imp.7"** %12, align 8
  %131 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.7"* %130 to %"class.std::__1::__unordered_map_hasher"*
  store %"class.std::__1::__unordered_map_hasher"* %131, %"class.std::__1::__unordered_map_hasher"** %11, align 8
  %132 = load %"class.std::__1::__unordered_map_hasher"*, %"class.std::__1::__unordered_map_hasher"** %11, align 8
  %133 = bitcast %"class.std::__1::__unordered_map_hasher"* %132 to %"struct.std::__1::hash"*
  %134 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.7", %"class.std::__1::__libcpp_compressed_pair_imp.7"* %130, i32 0, i32 0
  store i64* %13, i64** %10, align 8
  %135 = load i64*, i64** %10, align 8
  %136 = load i64, i64* %135, align 8
  store i64 %136, i64* %134, align 8
  %137 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %81, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.8"* %137, %"class.std::__1::__compressed_pair.8"** %8, align 8
  store float 1.000000e+00, float* %9, align 4
  %138 = load %"class.std::__1::__compressed_pair.8"*, %"class.std::__1::__compressed_pair.8"** %8, align 8
  %139 = load float, float* %9, align 4
  store %"class.std::__1::__compressed_pair.8"* %138, %"class.std::__1::__compressed_pair.8"** %6, align 8
  store float %139, float* %7, align 4
  %140 = load %"class.std::__1::__compressed_pair.8"*, %"class.std::__1::__compressed_pair.8"** %6, align 8
  %141 = bitcast %"class.std::__1::__compressed_pair.8"* %140 to %"class.std::__1::__libcpp_compressed_pair_imp.9"*
  store float* %7, float** %5, align 8
  %142 = load float*, float** %5, align 8
  %143 = load float, float* %142, align 4
  store %"class.std::__1::__libcpp_compressed_pair_imp.9"* %141, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %3, align 8
  store float %143, float* %4, align 4
  %144 = load %"class.std::__1::__libcpp_compressed_pair_imp.9"*, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %3, align 8
  %145 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.9"* %144 to %"class.std::__1::__unordered_map_equal"*
  store %"class.std::__1::__unordered_map_equal"* %145, %"class.std::__1::__unordered_map_equal"** %2, align 8
  %146 = load %"class.std::__1::__unordered_map_equal"*, %"class.std::__1::__unordered_map_equal"** %2, align 8
  %147 = bitcast %"class.std::__1::__unordered_map_equal"* %146 to %"struct.std::__1::equal_to"*
  %148 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.9", %"class.std::__1::__libcpp_compressed_pair_imp.9"* %144, i32 0, i32 0
  store float* %4, float** %1, align 8
  %149 = load float*, float** %1, align 8
  %150 = load float, float* %149, align 4
  store float %150, float* %148, align 4
  %151 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::__1::unordered_map"*)* @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeENS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S9_EEEEED1Ev to void (i8*)*), i8* bitcast (%"class.std::__1::unordered_map"* @_ZN6CGType14StructureTypesE to i8*), i8* @__dso_handle) #2
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeENS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S9_EEEEED1Ev(%"class.std::__1::unordered_map"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::unordered_map"*, align 8
  store %"class.std::__1::unordered_map"* %0, %"class.std::__1::unordered_map"** %2, align 8
  %3 = load %"class.std::__1::unordered_map"*, %"class.std::__1::unordered_map"** %2, align 8
  call void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeENS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S9_EEEEED2Ev(%"class.std::__1::unordered_map"* %3) #2
  ret void
}

; Function Attrs: nounwind
declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) #2

; Function Attrs: ssp uwtable
define internal void @__cxx_global_var_init.1() #0 section "__TEXT,__StaticInit,regular,pure_instructions" personality i32 (...)* @__gxx_personality_v0 {
  %1 = alloca float*, align 8
  %2 = alloca %"class.std::__1::__unordered_map_equal.32"*, align 8
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.31"*, align 8
  %4 = alloca float, align 4
  %5 = alloca float*, align 8
  %6 = alloca %"class.std::__1::__compressed_pair.30"*, align 8
  %7 = alloca float, align 4
  %8 = alloca %"class.std::__1::__compressed_pair.30"*, align 8
  %9 = alloca float, align 4
  %10 = alloca i64*, align 8
  %11 = alloca %"class.std::__1::__unordered_map_hasher.27"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.26"*, align 8
  %13 = alloca i64, align 8
  %14 = alloca i64*, align 8
  %15 = alloca %"class.std::__1::__compressed_pair.25"*, align 8
  %16 = alloca i64, align 8
  %17 = alloca %"class.std::__1::__compressed_pair.25"*, align 8
  %18 = alloca i64, align 8
  %19 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %20 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %23 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.18"*, align 8
  %24 = alloca %"class.std::__1::__compressed_pair.17"*, align 8
  %25 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %26 = alloca i8*, align 8
  %27 = alloca %"class.std::__1::allocator.19"*, align 8
  %28 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %29 = alloca i64, align 8
  %30 = alloca %"class.std::__1::allocator.19"*, align 8
  %31 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %32 = alloca i64, align 8
  %33 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.18"*, align 8
  %34 = alloca %"class.std::__1::__compressed_pair.17"*, align 8
  %35 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %36 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %37 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %38 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %39 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %40 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %41 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %42 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %43 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %44 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %45 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %46 = alloca %"class.std::__1::allocator.23"*, align 8
  %47 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.22"*, align 8
  %48 = alloca %"class.std::__1::__compressed_pair.21"*, align 8
  %49 = alloca %"class.std::__1::__compressed_pair.21"*, align 8
  %50 = alloca i64*, align 8
  %51 = alloca %"class.std::__1::allocator.19"*, align 8
  %52 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.18"*, align 8
  %53 = alloca i64, align 8
  %54 = alloca i64*, align 8
  %55 = alloca %"class.std::__1::__compressed_pair.17"*, align 8
  %56 = alloca i64, align 8
  %57 = alloca %"class.std::__1::__compressed_pair.17"*, align 8
  %58 = alloca i64, align 8
  %59 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %60 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %61 = alloca %"struct.std::__1::__hash_node_base.15"***, align 8
  %62 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %63 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %64 = alloca %"struct.std::__1::__hash_node_base.15"***, align 8
  %65 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %66 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %67 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %68 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %69 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %70 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %71 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %72 = alloca i8*
  %73 = alloca i32
  %74 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %75 = alloca %"class.std::__1::unordered_map.10"*, align 8
  %76 = alloca %"class.std::__1::unordered_map.10"*, align 8
  store %"class.std::__1::unordered_map.10"* @_ZN6CGType8EnvTypesE, %"class.std::__1::unordered_map.10"** %76, align 8
  %77 = load %"class.std::__1::unordered_map.10"*, %"class.std::__1::unordered_map.10"** %76, align 8
  store %"class.std::__1::unordered_map.10"* %77, %"class.std::__1::unordered_map.10"** %75, align 8
  %78 = load %"class.std::__1::unordered_map.10"*, %"class.std::__1::unordered_map.10"** %75, align 8
  %79 = getelementptr inbounds %"class.std::__1::unordered_map.10", %"class.std::__1::unordered_map.10"* %78, i32 0, i32 0
  store %"class.std::__1::__hash_table.11"* %79, %"class.std::__1::__hash_table.11"** %74, align 8
  %80 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %74, align 8
  store %"class.std::__1::__hash_table.11"* %80, %"class.std::__1::__hash_table.11"** %71, align 8
  %81 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %71, align 8
  %82 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %81, i32 0, i32 0
  store %"class.std::__1::unique_ptr.12"* %82, %"class.std::__1::unique_ptr.12"** %70, align 8
  %83 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %70, align 8
  store %"class.std::__1::unique_ptr.12"* %83, %"class.std::__1::unique_ptr.12"** %69, align 8
  %84 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %69, align 8
  %85 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %84, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %85, %"class.std::__1::__compressed_pair.13"** %67, align 8
  store %"struct.std::__1::__hash_node_base.15"** null, %"struct.std::__1::__hash_node_base.15"*** %68, align 8
  %86 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %67, align 8
  %87 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %68, align 8
  store %"class.std::__1::__compressed_pair.13"* %86, %"class.std::__1::__compressed_pair.13"** %65, align 8
  store %"struct.std::__1::__hash_node_base.15"** %87, %"struct.std::__1::__hash_node_base.15"*** %66, align 8
  %88 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %65, align 8
  %89 = bitcast %"class.std::__1::__compressed_pair.13"* %88 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"struct.std::__1::__hash_node_base.15"*** %66, %"struct.std::__1::__hash_node_base.15"**** %64, align 8
  %90 = load %"struct.std::__1::__hash_node_base.15"***, %"struct.std::__1::__hash_node_base.15"**** %64, align 8
  %91 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %90, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %89, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %62, align 8
  store %"struct.std::__1::__hash_node_base.15"** %91, %"struct.std::__1::__hash_node_base.15"*** %63, align 8
  %92 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %62, align 8
  %93 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %92, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"*** %63, %"struct.std::__1::__hash_node_base.15"**** %61, align 8
  %94 = load %"struct.std::__1::__hash_node_base.15"***, %"struct.std::__1::__hash_node_base.15"**** %61, align 8
  %95 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %94, align 8
  store %"struct.std::__1::__hash_node_base.15"** %95, %"struct.std::__1::__hash_node_base.15"*** %93, align 8
  %96 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %92, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator.16"* %96, %"class.std::__1::__bucket_list_deallocator.16"** %60, align 8
  %97 = load %"class.std::__1::__bucket_list_deallocator.16"*, %"class.std::__1::__bucket_list_deallocator.16"** %60, align 8
  store %"class.std::__1::__bucket_list_deallocator.16"* %97, %"class.std::__1::__bucket_list_deallocator.16"** %59, align 8
  %98 = load %"class.std::__1::__bucket_list_deallocator.16"*, %"class.std::__1::__bucket_list_deallocator.16"** %59, align 8
  %99 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.16", %"class.std::__1::__bucket_list_deallocator.16"* %98, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.17"* %99, %"class.std::__1::__compressed_pair.17"** %57, align 8
  store i64 0, i64* %58, align 8
  %100 = load %"class.std::__1::__compressed_pair.17"*, %"class.std::__1::__compressed_pair.17"** %57, align 8
  %101 = load i64, i64* %58, align 8
  store %"class.std::__1::__compressed_pair.17"* %100, %"class.std::__1::__compressed_pair.17"** %55, align 8
  store i64 %101, i64* %56, align 8
  %102 = load %"class.std::__1::__compressed_pair.17"*, %"class.std::__1::__compressed_pair.17"** %55, align 8
  %103 = bitcast %"class.std::__1::__compressed_pair.17"* %102 to %"class.std::__1::__libcpp_compressed_pair_imp.18"*
  store i64* %56, i64** %54, align 8
  %104 = load i64*, i64** %54, align 8
  %105 = load i64, i64* %104, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.18"* %103, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %52, align 8
  store i64 %105, i64* %53, align 8
  %106 = load %"class.std::__1::__libcpp_compressed_pair_imp.18"*, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %52, align 8
  %107 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.18"* %106 to %"class.std::__1::allocator.19"*
  store %"class.std::__1::allocator.19"* %107, %"class.std::__1::allocator.19"** %51, align 8
  %108 = load %"class.std::__1::allocator.19"*, %"class.std::__1::allocator.19"** %51, align 8
  %109 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.18", %"class.std::__1::__libcpp_compressed_pair_imp.18"* %106, i32 0, i32 0
  store i64* %53, i64** %50, align 8
  %110 = load i64*, i64** %50, align 8
  %111 = load i64, i64* %110, align 8
  store i64 %111, i64* %109, align 8
  %112 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %81, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.21"* %112, %"class.std::__1::__compressed_pair.21"** %49, align 8
  %113 = load %"class.std::__1::__compressed_pair.21"*, %"class.std::__1::__compressed_pair.21"** %49, align 8
  store %"class.std::__1::__compressed_pair.21"* %113, %"class.std::__1::__compressed_pair.21"** %48, align 8
  %114 = load %"class.std::__1::__compressed_pair.21"*, %"class.std::__1::__compressed_pair.21"** %48, align 8
  %115 = bitcast %"class.std::__1::__compressed_pair.21"* %114 to %"class.std::__1::__libcpp_compressed_pair_imp.22"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.22"* %115, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %47, align 8
  %116 = load %"class.std::__1::__libcpp_compressed_pair_imp.22"*, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %47, align 8
  %117 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.22"* %116 to %"class.std::__1::allocator.23"*
  store %"class.std::__1::allocator.23"* %117, %"class.std::__1::allocator.23"** %46, align 8
  %118 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %46, align 8
  %119 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.22", %"class.std::__1::__libcpp_compressed_pair_imp.22"* %116, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"* %119, %"struct.std::__1::__hash_node_base.15"** %45, align 8
  %120 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %45, align 8
  store %"struct.std::__1::__hash_node_base.15"* %120, %"struct.std::__1::__hash_node_base.15"** %44, align 8
  %121 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %44, align 8
  %122 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %121, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"* null, %"struct.std::__1::__hash_node_base.15"** %122, align 8
  %123 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %81, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.25"* %123, %"class.std::__1::__compressed_pair.25"** %17, align 8
  store i64 0, i64* %18, align 8
  %124 = load %"class.std::__1::__compressed_pair.25"*, %"class.std::__1::__compressed_pair.25"** %17, align 8
  %125 = load i64, i64* %18, align 8
  store %"class.std::__1::__compressed_pair.25"* %124, %"class.std::__1::__compressed_pair.25"** %15, align 8
  store i64 %125, i64* %16, align 8
  %126 = load %"class.std::__1::__compressed_pair.25"*, %"class.std::__1::__compressed_pair.25"** %15, align 8
  %127 = bitcast %"class.std::__1::__compressed_pair.25"* %126 to %"class.std::__1::__libcpp_compressed_pair_imp.26"*
  store i64* %16, i64** %14, align 8
  %128 = load i64*, i64** %14, align 8
  %129 = load i64, i64* %128, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.26"* %127, %"class.std::__1::__libcpp_compressed_pair_imp.26"** %12, align 8
  store i64 %129, i64* %13, align 8
  %130 = load %"class.std::__1::__libcpp_compressed_pair_imp.26"*, %"class.std::__1::__libcpp_compressed_pair_imp.26"** %12, align 8
  %131 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.26"* %130 to %"class.std::__1::__unordered_map_hasher.27"*
  store %"class.std::__1::__unordered_map_hasher.27"* %131, %"class.std::__1::__unordered_map_hasher.27"** %11, align 8
  %132 = load %"class.std::__1::__unordered_map_hasher.27"*, %"class.std::__1::__unordered_map_hasher.27"** %11, align 8
  %133 = bitcast %"class.std::__1::__unordered_map_hasher.27"* %132 to %"struct.std::__1::hash.28"*
  %134 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.26", %"class.std::__1::__libcpp_compressed_pair_imp.26"* %130, i32 0, i32 0
  store i64* %13, i64** %10, align 8
  %135 = load i64*, i64** %10, align 8
  %136 = load i64, i64* %135, align 8
  store i64 %136, i64* %134, align 8
  %137 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %81, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.30"* %137, %"class.std::__1::__compressed_pair.30"** %8, align 8
  store float 1.000000e+00, float* %9, align 4
  %138 = load %"class.std::__1::__compressed_pair.30"*, %"class.std::__1::__compressed_pair.30"** %8, align 8
  %139 = load float, float* %9, align 4
  store %"class.std::__1::__compressed_pair.30"* %138, %"class.std::__1::__compressed_pair.30"** %6, align 8
  store float %139, float* %7, align 4
  %140 = load %"class.std::__1::__compressed_pair.30"*, %"class.std::__1::__compressed_pair.30"** %6, align 8
  %141 = bitcast %"class.std::__1::__compressed_pair.30"* %140 to %"class.std::__1::__libcpp_compressed_pair_imp.31"*
  store float* %7, float** %5, align 8
  %142 = load float*, float** %5, align 8
  %143 = load float, float* %142, align 4
  store %"class.std::__1::__libcpp_compressed_pair_imp.31"* %141, %"class.std::__1::__libcpp_compressed_pair_imp.31"** %3, align 8
  store float %143, float* %4, align 4
  %144 = load %"class.std::__1::__libcpp_compressed_pair_imp.31"*, %"class.std::__1::__libcpp_compressed_pair_imp.31"** %3, align 8
  %145 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.31"* %144 to %"class.std::__1::__unordered_map_equal.32"*
  store %"class.std::__1::__unordered_map_equal.32"* %145, %"class.std::__1::__unordered_map_equal.32"** %2, align 8
  %146 = load %"class.std::__1::__unordered_map_equal.32"*, %"class.std::__1::__unordered_map_equal.32"** %2, align 8
  %147 = bitcast %"class.std::__1::__unordered_map_equal.32"* %146 to %"struct.std::__1::equal_to.33"*
  %148 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.31", %"class.std::__1::__libcpp_compressed_pair_imp.31"* %144, i32 0, i32 0
  store float* %4, float** %1, align 8
  %149 = load float*, float** %1, align 8
  %150 = load float, float* %149, align 4
  store float %150, float* %148, align 4
  %151 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::__1::unordered_map.10"*)* @_ZNSt3__113unordered_mapIjPN4llvm10StructTypeENS_4hashIjEENS_8equal_toIjEENS_9allocatorINS_4pairIKjS3_EEEEED1Ev to void (i8*)*), i8* bitcast (%"class.std::__1::unordered_map.10"* @_ZN6CGType8EnvTypesE to i8*), i8* @__dso_handle) #2
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113unordered_mapIjPN4llvm10StructTypeENS_4hashIjEENS_8equal_toIjEENS_9allocatorINS_4pairIKjS3_EEEEED1Ev(%"class.std::__1::unordered_map.10"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::unordered_map.10"*, align 8
  store %"class.std::__1::unordered_map.10"* %0, %"class.std::__1::unordered_map.10"** %2, align 8
  %3 = load %"class.std::__1::unordered_map.10"*, %"class.std::__1::unordered_map.10"** %2, align 8
  call void @_ZNSt3__113unordered_mapIjPN4llvm10StructTypeENS_4hashIjEENS_8equal_toIjEENS_9allocatorINS_4pairIKjS3_EEEEED2Ev(%"class.std::__1::unordered_map.10"* %3) #2
  ret void
}

; Function Attrs: ssp uwtable
define %"class.llvm::FunctionType"* @_ZN6CGType11getFuncTypeER13TypeSpecifierNSt3__16vectorIPN4llvm4TypeENS2_9allocatorIS6_EEEE(%struct.TypeSpecifier* dereferenceable(216), %"class.std::__1::vector.78"*) #0 align 2 {
  %3 = alloca %struct.TypeSpecifier*, align 8
  %4 = alloca %"class.llvm::Type"*, align 8
  %5 = alloca %"class.llvm::ArrayRef", align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %3, align 8
  %6 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  %7 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %6)
  store %"class.llvm::Type"* %7, %"class.llvm::Type"** %4, align 8
  %8 = load %"class.llvm::Type"*, %"class.llvm::Type"** %4, align 8
  call void @_ZN4llvm8ArrayRefIPNS_4TypeEEC1INSt3__19allocatorIS2_EEEERKNS5_6vectorIS2_T_EE(%"class.llvm::ArrayRef"* %5, %"class.std::__1::vector.78"* dereferenceable(24) %1)
  %9 = bitcast %"class.llvm::ArrayRef"* %5 to { %"class.llvm::Type"**, i64 }*
  %10 = getelementptr inbounds { %"class.llvm::Type"**, i64 }, { %"class.llvm::Type"**, i64 }* %9, i32 0, i32 0
  %11 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %10, align 8
  %12 = getelementptr inbounds { %"class.llvm::Type"**, i64 }, { %"class.llvm::Type"**, i64 }* %9, i32 0, i32 1
  %13 = load i64, i64* %12, align 8
  %14 = call %"class.llvm::FunctionType"* @_ZN4llvm12FunctionType3getEPNS_4TypeENS_8ArrayRefIS2_EEb(%"class.llvm::Type"* %8, %"class.llvm::Type"** %11, i64 %13, i1 zeroext false)
  ret %"class.llvm::FunctionType"* %14
}

; Function Attrs: ssp uwtable
define %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216)) #0 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  %3 = alloca %struct.TypeSpecifier*, align 8
  %4 = alloca %"class.llvm::Type"*, align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %3, align 8
  %5 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  %6 = call %"class.llvm::Type"* @_ZN6CGType8_getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %5)
  store %"class.llvm::Type"* %6, %"class.llvm::Type"** %4, align 8
  %7 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  %8 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %7, i32 0, i32 19
  %9 = load i8, i8* %8, align 4
  %10 = trunc i8 %9 to i1
  br i1 %10, label %16, label %11

; <label>:11:                                     ; preds = %1
  %12 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  %13 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %12, i32 0, i32 20
  %14 = load i8, i8* %13, align 1
  %15 = trunc i8 %14 to i1
  br i1 %15, label %16, label %20

; <label>:16:                                     ; preds = %11, %1
  %17 = load %"class.llvm::Type"*, %"class.llvm::Type"** %4, align 8
  %18 = call %"class.llvm::PointerType"* @_ZNK4llvm4Type12getPointerToEj(%"class.llvm::Type"* %17, i32 0)
  %19 = bitcast %"class.llvm::PointerType"* %18 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %19, %"class.llvm::Type"** %2, align 8
  br label %22

; <label>:20:                                     ; preds = %11
  %21 = load %"class.llvm::Type"*, %"class.llvm::Type"** %4, align 8
  store %"class.llvm::Type"* %21, %"class.llvm::Type"** %2, align 8
  br label %22

; <label>:22:                                     ; preds = %20, %16
  %23 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  ret %"class.llvm::Type"* %23
}

declare %"class.llvm::FunctionType"* @_ZN4llvm12FunctionType3getEPNS_4TypeENS_8ArrayRefIS2_EEb(%"class.llvm::Type"*, %"class.llvm::Type"**, i64, i1 zeroext) #3

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm8ArrayRefIPNS_4TypeEEC1INSt3__19allocatorIS2_EEEERKNS5_6vectorIS2_T_EE(%"class.llvm::ArrayRef"*, %"class.std::__1::vector.78"* dereferenceable(24)) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::ArrayRef"*, align 8
  %4 = alloca %"class.std::__1::vector.78"*, align 8
  store %"class.llvm::ArrayRef"* %0, %"class.llvm::ArrayRef"** %3, align 8
  store %"class.std::__1::vector.78"* %1, %"class.std::__1::vector.78"** %4, align 8
  %5 = load %"class.llvm::ArrayRef"*, %"class.llvm::ArrayRef"** %3, align 8
  %6 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %4, align 8
  call void @_ZN4llvm8ArrayRefIPNS_4TypeEEC2INSt3__19allocatorIS2_EEEERKNS5_6vectorIS2_T_EE(%"class.llvm::ArrayRef"* %5, %"class.std::__1::vector.78"* dereferenceable(24) %6)
  ret void
}

; Function Attrs: ssp uwtable
define %"class.llvm::Type"* @_ZN6CGType8_getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216)) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %3 = alloca %"class.llvm::Type"**, align 8
  %4 = alloca %"class.llvm::Type"**, align 8
  %5 = alloca %"class.std::__1::allocator.82"*, align 8
  %6 = alloca %"class.llvm::Type"**, align 8
  %7 = alloca %"class.llvm::Type"**, align 8
  %8 = alloca %"class.llvm::Type"**, align 8
  %9 = alloca %"struct.std::__1::integral_constant.128", align 1
  %10 = alloca %"class.std::__1::allocator.82"*, align 8
  %11 = alloca %"class.llvm::Type"**, align 8
  %12 = alloca %"class.llvm::Type"**, align 8
  %13 = alloca %"class.llvm::Type"**, align 8
  %14 = alloca %"class.std::__1::allocator.82"*, align 8
  %15 = alloca %"class.llvm::Type"**, align 8
  %16 = alloca %"class.llvm::Type"**, align 8
  %17 = alloca %"struct.std::__1::integral_constant.128", align 1
  %18 = alloca %"struct.std::__1::__has_construct.154", align 1
  %19 = alloca %"class.llvm::Type"**, align 8
  %20 = alloca %"class.llvm::Type"**, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %23 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %24 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %25 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %26 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %27 = alloca %"class.std::__1::vector.78"*, align 8
  %28 = alloca %"class.llvm::Type"**, align 8
  %29 = alloca %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator", align 1
  %30 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %31 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %32 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %34 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %35 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %36 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %37 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %38 = alloca %struct.TypeSpecifier*, align 8
  %39 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %40 = alloca %struct.TypeSpecifier*, align 8
  %41 = alloca %"class.std::__1::__wrap_iter", align 8
  %42 = alloca %"class.std::__1::vector.67"*, align 8
  %43 = alloca %struct.TypeSpecifier*, align 8
  %44 = alloca %"class.std::__1::__wrap_iter", align 8
  %45 = alloca %"class.std::__1::vector.67"*, align 8
  %46 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %47 = alloca %struct.TypeSpecifier*, align 8
  %48 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %49 = alloca %struct.TypeSpecifier*, align 8
  %50 = alloca %"class.std::__1::__wrap_iter", align 8
  %51 = alloca %"class.std::__1::vector.67"*, align 8
  %52 = alloca %struct.TypeSpecifier*, align 8
  %53 = alloca %"class.std::__1::__wrap_iter", align 8
  %54 = alloca %"class.std::__1::vector.67"*, align 8
  %55 = alloca %"class.std::__1::vector.67"*, align 8
  %56 = alloca %"class.std::initializer_list"*, align 8
  %57 = alloca %"class.std::initializer_list"*, align 8
  %58 = alloca %"class.std::initializer_list"*, align 8
  %59 = alloca %"class.std::initializer_list"*, align 8
  %60 = alloca %"class.std::initializer_list"*, align 8
  %61 = alloca %"class.llvm::Type"***, align 8
  %62 = alloca %"class.std::__1::allocator.82"*, align 8
  %63 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %64 = alloca %"class.llvm::Type"**, align 8
  %65 = alloca %"class.llvm::Type"***, align 8
  %66 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %67 = alloca %"class.llvm::Type"**, align 8
  %68 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %69 = alloca %"class.llvm::Type"**, align 8
  %70 = alloca %"class.std::__1::__vector_base_common"*, align 8
  %71 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %72 = alloca %"class.std::initializer_list", align 8
  %73 = alloca %"class.std::__1::vector.78"*, align 8
  %74 = alloca i8*
  %75 = alloca i32
  %76 = alloca %"class.std::initializer_list", align 8
  %77 = alloca %"class.std::__1::vector.78"*, align 8
  %78 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %79 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %80 = alloca %"class.std::__1::basic_string"*, align 8
  %81 = alloca [3 x i64]*, align 8
  %82 = alloca i32, align 4
  %83 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %84 = alloca %"class.std::__1::basic_string"*, align 8
  %85 = alloca %"class.std::__1::basic_string"*, align 8
  %86 = alloca %"class.std::__1::basic_string"*, align 8
  %87 = alloca %"class.std::__1::basic_string"*, align 8
  %88 = alloca %"class.std::__1::basic_string"*, align 8
  %89 = alloca i8*, align 8
  %90 = alloca %"class.std::__1::basic_string"*, align 8
  %91 = alloca %"class.llvm::Type"*, align 8
  %92 = alloca %struct.TypeSpecifier*, align 8
  %93 = alloca %"class.std::__1::vector.78", align 8
  %94 = alloca %"class.std::initializer_list", align 8
  %95 = alloca [1 x %"class.llvm::Type"*], align 8
  %96 = alloca i8*
  %97 = alloca i32
  %98 = alloca %"class.std::__1::vector.67"*, align 8
  %99 = alloca %"class.std::__1::__wrap_iter", align 8
  %100 = alloca %"class.std::__1::__wrap_iter", align 8
  %101 = alloca %struct.TypeSpecifier*, align 8
  %102 = alloca %"class.llvm::Type"*, align 8
  %103 = alloca %"class.llvm::ArrayRef", align 8
  %104 = alloca %"class.std::__1::basic_string", align 8
  %105 = alloca %"class.std::__1::basic_string", align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %92, align 8
  %106 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %92, align 8
  %107 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %106, i32 0, i32 4
  %108 = load i8, i8* %107, align 1
  %109 = trunc i8 %108 to i1
  br i1 %109, label %110, label %118

; <label>:110:                                    ; preds = %1
  %111 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %92, align 8
  %112 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %111, i32 0, i32 25
  %113 = bitcast %union.anon.73* %112 to %struct.TypeSpecifier**
  %114 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %113, align 8
  %115 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %114)
  %116 = call %"class.llvm::PointerType"* @_ZNK4llvm4Type12getPointerToEj(%"class.llvm::Type"* %115, i32 0)
  %117 = bitcast %"class.llvm::PointerType"* %116 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %117, %"class.llvm::Type"** %91, align 8
  br label %457

; <label>:118:                                    ; preds = %1
  %119 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %92, align 8
  %120 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %119, i32 0, i32 21
  %121 = load i8, i8* %120, align 2
  %122 = trunc i8 %121 to i1
  br i1 %122, label %123, label %131

; <label>:123:                                    ; preds = %118
  %124 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %92, align 8
  %125 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %124, i32 0, i32 22
  %126 = call dereferenceable(8) %"class.llvm::StructType"** @_ZNSt3__113unordered_mapIjPN4llvm10StructTypeENS_4hashIjEENS_8equal_toIjEENS_9allocatorINS_4pairIKjS3_EEEEEixERSA_(%"class.std::__1::unordered_map.10"* @_ZN6CGType8EnvTypesE, i32* dereferenceable(4) %125)
  %127 = load %"class.llvm::StructType"*, %"class.llvm::StructType"** %126, align 8
  %128 = bitcast %"class.llvm::StructType"* %127 to %"class.llvm::Type"*
  %129 = call %"class.llvm::PointerType"* @_ZNK4llvm4Type12getPointerToEj(%"class.llvm::Type"* %128, i32 0)
  %130 = bitcast %"class.llvm::PointerType"* %129 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %130, %"class.llvm::Type"** %91, align 8
  br label %457

; <label>:131:                                    ; preds = %118
  %132 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %92, align 8
  %133 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %132, i32 0, i32 5
  %134 = load i8, i8* %133, align 2
  %135 = trunc i8 %134 to i1
  br i1 %135, label %136, label %139

; <label>:136:                                    ; preds = %131
  %137 = call %"class.llvm::PointerType"* @_ZN4llvm13IRBuilderBase12getInt8PtrTyEj(%"class.llvm::IRBuilderBase"* getelementptr inbounds (%"class.llvm::IRBuilder", %"class.llvm::IRBuilder"* @_ZN14CodeGenVisitor7BuilderE, i32 0, i32 0), i32 0)
  %138 = bitcast %"class.llvm::PointerType"* %137 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %138, %"class.llvm::Type"** %91, align 8
  br label %457

; <label>:139:                                    ; preds = %131
  %140 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %92, align 8
  %141 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %140, i32 0, i32 18
  %142 = load i8, i8* %141, align 1
  %143 = trunc i8 %142 to i1
  br i1 %143, label %144, label %373

; <label>:144:                                    ; preds = %139
  %145 = getelementptr inbounds [1 x %"class.llvm::Type"*], [1 x %"class.llvm::Type"*]* %95, i64 0, i64 0
  %146 = call %"class.llvm::PointerType"* @_ZN4llvm4Type12getInt8PtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, i32 0)
  %147 = bitcast %"class.llvm::PointerType"* %146 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %147, %"class.llvm::Type"** %145, align 8
  %148 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %94, i32 0, i32 0
  %149 = getelementptr inbounds [1 x %"class.llvm::Type"*], [1 x %"class.llvm::Type"*]* %95, i64 0, i64 0
  store %"class.llvm::Type"** %149, %"class.llvm::Type"*** %148, align 8
  %150 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %94, i32 0, i32 1
  store i64 1, i64* %150, align 8
  %151 = bitcast %"class.std::initializer_list"* %94 to { %"class.llvm::Type"**, i64 }*
  %152 = getelementptr inbounds { %"class.llvm::Type"**, i64 }, { %"class.llvm::Type"**, i64 }* %151, i32 0, i32 0
  %153 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %152, align 8
  %154 = getelementptr inbounds { %"class.llvm::Type"**, i64 }, { %"class.llvm::Type"**, i64 }* %151, i32 0, i32 1
  %155 = load i64, i64* %154, align 8
  %156 = bitcast %"class.std::initializer_list"* %76 to { %"class.llvm::Type"**, i64 }*
  %157 = getelementptr inbounds { %"class.llvm::Type"**, i64 }, { %"class.llvm::Type"**, i64 }* %156, i32 0, i32 0
  store %"class.llvm::Type"** %153, %"class.llvm::Type"*** %157, align 8
  %158 = getelementptr inbounds { %"class.llvm::Type"**, i64 }, { %"class.llvm::Type"**, i64 }* %156, i32 0, i32 1
  store i64 %155, i64* %158, align 8
  store %"class.std::__1::vector.78"* %93, %"class.std::__1::vector.78"** %77, align 8
  %159 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %77, align 8
  %160 = bitcast %"class.std::initializer_list"* %76 to { %"class.llvm::Type"**, i64 }*
  %161 = getelementptr inbounds { %"class.llvm::Type"**, i64 }, { %"class.llvm::Type"**, i64 }* %160, i32 0, i32 0
  %162 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %161, align 8
  %163 = getelementptr inbounds { %"class.llvm::Type"**, i64 }, { %"class.llvm::Type"**, i64 }* %160, i32 0, i32 1
  %164 = load i64, i64* %163, align 8
  %165 = bitcast %"class.std::initializer_list"* %72 to { %"class.llvm::Type"**, i64 }*
  %166 = getelementptr inbounds { %"class.llvm::Type"**, i64 }, { %"class.llvm::Type"**, i64 }* %165, i32 0, i32 0
  store %"class.llvm::Type"** %162, %"class.llvm::Type"*** %166, align 8
  %167 = getelementptr inbounds { %"class.llvm::Type"**, i64 }, { %"class.llvm::Type"**, i64 }* %165, i32 0, i32 1
  store i64 %164, i64* %167, align 8
  store %"class.std::__1::vector.78"* %159, %"class.std::__1::vector.78"** %73, align 8
  %168 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %73, align 8
  %169 = bitcast %"class.std::__1::vector.78"* %168 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %169, %"class.std::__1::__vector_base.79"** %71, align 8
  %170 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %71, align 8
  %171 = bitcast %"class.std::__1::__vector_base.79"* %170 to %"class.std::__1::__vector_base_common"*
  store %"class.std::__1::__vector_base_common"* %171, %"class.std::__1::__vector_base_common"** %70, align 8
  %172 = load %"class.std::__1::__vector_base_common"*, %"class.std::__1::__vector_base_common"** %70, align 8
  %173 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %170, i32 0, i32 0
  store %"class.llvm::Type"** null, %"class.llvm::Type"*** %173, align 8
  %174 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %170, i32 0, i32 1
  store %"class.llvm::Type"** null, %"class.llvm::Type"*** %174, align 8
  %175 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %170, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %175, %"class.std::__1::__compressed_pair.80"** %68, align 8
  store %"class.llvm::Type"** null, %"class.llvm::Type"*** %69, align 8
  %176 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %68, align 8
  %177 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %69, align 8
  store %"class.std::__1::__compressed_pair.80"* %176, %"class.std::__1::__compressed_pair.80"** %66, align 8
  store %"class.llvm::Type"** %177, %"class.llvm::Type"*** %67, align 8
  %178 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %66, align 8
  %179 = bitcast %"class.std::__1::__compressed_pair.80"* %178 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.llvm::Type"*** %67, %"class.llvm::Type"**** %65, align 8
  %180 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %65, align 8
  %181 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %180, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %179, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %63, align 8
  store %"class.llvm::Type"** %181, %"class.llvm::Type"*** %64, align 8
  %182 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %63, align 8
  %183 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.81"* %182 to %"class.std::__1::allocator.82"*
  store %"class.std::__1::allocator.82"* %183, %"class.std::__1::allocator.82"** %62, align 8
  %184 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %62, align 8
  %185 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %182, i32 0, i32 0
  store %"class.llvm::Type"*** %64, %"class.llvm::Type"**** %61, align 8
  %186 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %61, align 8
  %187 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %186, align 8
  store %"class.llvm::Type"** %187, %"class.llvm::Type"*** %185, align 8
  store %"class.std::initializer_list"* %72, %"class.std::initializer_list"** %60, align 8
  %188 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %60, align 8
  %189 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %188, i32 0, i32 1
  %190 = load i64, i64* %189, align 8
  %191 = icmp ugt i64 %190, 0
  br i1 %191, label %192, label %219

; <label>:192:                                    ; preds = %144
  store %"class.std::initializer_list"* %72, %"class.std::initializer_list"** %59, align 8
  %193 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %59, align 8
  %194 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %193, i32 0, i32 1
  %195 = load i64, i64* %194, align 8
  invoke void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE8allocateEm(%"class.std::__1::vector.78"* %168, i64 %195)
          to label %196 unwind label %210

; <label>:196:                                    ; preds = %192
  store %"class.std::initializer_list"* %72, %"class.std::initializer_list"** %56, align 8
  %197 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %56, align 8
  %198 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %197, i32 0, i32 0
  %199 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %198, align 8
  store %"class.std::initializer_list"* %72, %"class.std::initializer_list"** %57, align 8
  %200 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %57, align 8
  %201 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %200, i32 0, i32 0
  %202 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %201, align 8
  %203 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %200, i32 0, i32 1
  %204 = load i64, i64* %203, align 8
  %205 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %202, i64 %204
  store %"class.std::initializer_list"* %72, %"class.std::initializer_list"** %58, align 8
  %206 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %58, align 8
  %207 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %206, i32 0, i32 1
  %208 = load i64, i64* %207, align 8
  invoke void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE18__construct_at_endIPKS3_EENS_9enable_ifIXsr21__is_forward_iteratorIT_EE5valueEvE4typeESB_SB_m(%"class.std::__1::vector.78"* %168, %"class.llvm::Type"** %199, %"class.llvm::Type"** %205, i64 %208)
          to label %209 unwind label %210

; <label>:209:                                    ; preds = %196
  br label %219

; <label>:210:                                    ; preds = %196, %192
  %211 = landingpad { i8*, i32 }
          cleanup
  %212 = extractvalue { i8*, i32 } %211, 0
  store i8* %212, i8** %74, align 8
  %213 = extractvalue { i8*, i32 } %211, 1
  store i32 %213, i32* %75, align 4
  %214 = bitcast %"class.std::__1::vector.78"* %168 to %"class.std::__1::__vector_base.79"*
  call void @_ZNSt3__113__vector_baseIPN4llvm4TypeENS_9allocatorIS3_EEED2Ev(%"class.std::__1::__vector_base.79"* %214) #2
  %215 = load i8*, i8** %74, align 8
  %216 = load i32, i32* %75, align 4
  %217 = insertvalue { i8*, i32 } undef, i8* %215, 0
  %218 = insertvalue { i8*, i32 } %217, i32 %216, 1
  resume { i8*, i32 } %218

; <label>:219:                                    ; preds = %144, %209
  %220 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %92, align 8
  %221 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %220, i32 0, i32 24
  store %"class.std::__1::vector.67"* %221, %"class.std::__1::vector.67"** %55, align 8
  %222 = load %"class.std::__1::vector.67"*, %"class.std::__1::vector.67"** %55, align 8
  %223 = bitcast %"class.std::__1::vector.67"* %222 to %"class.std::__1::__vector_base.68"*
  %224 = getelementptr inbounds %"class.std::__1::__vector_base.68", %"class.std::__1::__vector_base.68"* %223, i32 0, i32 1
  %225 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %224, align 8
  %226 = bitcast %"class.std::__1::vector.67"* %222 to %"class.std::__1::__vector_base.68"*
  %227 = getelementptr inbounds %"class.std::__1::__vector_base.68", %"class.std::__1::__vector_base.68"* %226, i32 0, i32 0
  %228 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %227, align 8
  %229 = ptrtoint %struct.TypeSpecifier* %225 to i64
  %230 = ptrtoint %struct.TypeSpecifier* %228 to i64
  %231 = sub i64 %229, %230
  %232 = sdiv exact i64 %231, 216
  invoke void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE7reserveEm(%"class.std::__1::vector.78"* %93, i64 %232)
          to label %233 unwind label %350

; <label>:233:                                    ; preds = %219
  %234 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %92, align 8
  %235 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %234, i32 0, i32 24
  store %"class.std::__1::vector.67"* %235, %"class.std::__1::vector.67"** %98, align 8
  %236 = load %"class.std::__1::vector.67"*, %"class.std::__1::vector.67"** %98, align 8
  store %"class.std::__1::vector.67"* %236, %"class.std::__1::vector.67"** %54, align 8
  %237 = load %"class.std::__1::vector.67"*, %"class.std::__1::vector.67"** %54, align 8
  %238 = bitcast %"class.std::__1::vector.67"* %237 to %"class.std::__1::__vector_base.68"*
  %239 = getelementptr inbounds %"class.std::__1::__vector_base.68", %"class.std::__1::__vector_base.68"* %238, i32 0, i32 0
  %240 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %239, align 8
  store %"class.std::__1::vector.67"* %237, %"class.std::__1::vector.67"** %51, align 8
  store %struct.TypeSpecifier* %240, %struct.TypeSpecifier** %52, align 8
  %241 = load %"class.std::__1::vector.67"*, %"class.std::__1::vector.67"** %51, align 8
  %242 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %52, align 8
  store %"class.std::__1::__wrap_iter"* %50, %"class.std::__1::__wrap_iter"** %48, align 8
  store %struct.TypeSpecifier* %242, %struct.TypeSpecifier** %49, align 8
  %243 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %48, align 8
  %244 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %49, align 8
  store %"class.std::__1::__wrap_iter"* %243, %"class.std::__1::__wrap_iter"** %46, align 8
  store %struct.TypeSpecifier* %244, %struct.TypeSpecifier** %47, align 8
  %245 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %46, align 8
  %246 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %245, i32 0, i32 0
  %247 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %47, align 8
  store %struct.TypeSpecifier* %247, %struct.TypeSpecifier** %246, align 8
  %248 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %50, i32 0, i32 0
  %249 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %248, align 8
  %250 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %53, i32 0, i32 0
  store %struct.TypeSpecifier* %249, %struct.TypeSpecifier** %250, align 8
  %251 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %53, i32 0, i32 0
  %252 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %251, align 8
  %253 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %99, i32 0, i32 0
  store %struct.TypeSpecifier* %252, %struct.TypeSpecifier** %253, align 8
  %254 = load %"class.std::__1::vector.67"*, %"class.std::__1::vector.67"** %98, align 8
  store %"class.std::__1::vector.67"* %254, %"class.std::__1::vector.67"** %45, align 8
  %255 = load %"class.std::__1::vector.67"*, %"class.std::__1::vector.67"** %45, align 8
  %256 = bitcast %"class.std::__1::vector.67"* %255 to %"class.std::__1::__vector_base.68"*
  %257 = getelementptr inbounds %"class.std::__1::__vector_base.68", %"class.std::__1::__vector_base.68"* %256, i32 0, i32 1
  %258 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %257, align 8
  store %"class.std::__1::vector.67"* %255, %"class.std::__1::vector.67"** %42, align 8
  store %struct.TypeSpecifier* %258, %struct.TypeSpecifier** %43, align 8
  %259 = load %"class.std::__1::vector.67"*, %"class.std::__1::vector.67"** %42, align 8
  %260 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %43, align 8
  store %"class.std::__1::__wrap_iter"* %41, %"class.std::__1::__wrap_iter"** %39, align 8
  store %struct.TypeSpecifier* %260, %struct.TypeSpecifier** %40, align 8
  %261 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %39, align 8
  %262 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %40, align 8
  store %"class.std::__1::__wrap_iter"* %261, %"class.std::__1::__wrap_iter"** %37, align 8
  store %struct.TypeSpecifier* %262, %struct.TypeSpecifier** %38, align 8
  %263 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %37, align 8
  %264 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %263, i32 0, i32 0
  %265 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %38, align 8
  store %struct.TypeSpecifier* %265, %struct.TypeSpecifier** %264, align 8
  %266 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %41, i32 0, i32 0
  %267 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %266, align 8
  %268 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %44, i32 0, i32 0
  store %struct.TypeSpecifier* %267, %struct.TypeSpecifier** %268, align 8
  %269 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %44, i32 0, i32 0
  %270 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %269, align 8
  %271 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %100, i32 0, i32 0
  store %struct.TypeSpecifier* %270, %struct.TypeSpecifier** %271, align 8
  br label %272

; <label>:272:                                    ; preds = %345, %233
  store %"class.std::__1::__wrap_iter"* %99, %"class.std::__1::__wrap_iter"** %35, align 8
  store %"class.std::__1::__wrap_iter"* %100, %"class.std::__1::__wrap_iter"** %36, align 8
  %273 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %35, align 8
  %274 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %36, align 8
  store %"class.std::__1::__wrap_iter"* %273, %"class.std::__1::__wrap_iter"** %33, align 8
  store %"class.std::__1::__wrap_iter"* %274, %"class.std::__1::__wrap_iter"** %34, align 8
  %275 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %33, align 8
  store %"class.std::__1::__wrap_iter"* %275, %"class.std::__1::__wrap_iter"** %32, align 8
  %276 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %32, align 8
  %277 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %276, i32 0, i32 0
  %278 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %277, align 8
  %279 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %34, align 8
  store %"class.std::__1::__wrap_iter"* %279, %"class.std::__1::__wrap_iter"** %31, align 8
  %280 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %31, align 8
  %281 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %280, i32 0, i32 0
  %282 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %281, align 8
  %283 = icmp eq %struct.TypeSpecifier* %278, %282
  %284 = xor i1 %283, true
  br i1 %284, label %285, label %354

; <label>:285:                                    ; preds = %272
  store %"class.std::__1::__wrap_iter"* %99, %"class.std::__1::__wrap_iter"** %30, align 8
  %286 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %30, align 8
  %287 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %286, i32 0, i32 0
  %288 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %287, align 8
  store %struct.TypeSpecifier* %288, %struct.TypeSpecifier** %101, align 8
  %289 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %101, align 8
  %290 = invoke %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %289)
          to label %291 unwind label %350

; <label>:291:                                    ; preds = %285
  store %"class.llvm::Type"* %290, %"class.llvm::Type"** %102, align 8
  store %"class.std::__1::vector.78"* %93, %"class.std::__1::vector.78"** %27, align 8
  store %"class.llvm::Type"** %102, %"class.llvm::Type"*** %28, align 8
  %292 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %27, align 8
  %293 = bitcast %"class.std::__1::vector.78"* %292 to %"class.std::__1::__vector_base.79"*
  %294 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %293, i32 0, i32 1
  %295 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %294, align 8
  %296 = bitcast %"class.std::__1::vector.78"* %292 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %296, %"class.std::__1::__vector_base.79"** %26, align 8
  %297 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %26, align 8
  %298 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %297, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %298, %"class.std::__1::__compressed_pair.80"** %25, align 8
  %299 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %25, align 8
  %300 = bitcast %"class.std::__1::__compressed_pair.80"* %299 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %300, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %24, align 8
  %301 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %24, align 8
  %302 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %301, i32 0, i32 0
  %303 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %302, align 8
  %304 = icmp ult %"class.llvm::Type"** %295, %303
  br i1 %304, label %305, label %339

; <label>:305:                                    ; preds = %291
  invoke void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE24__RAII_IncreaseAnnotatorC1ERKS6_m(%"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"* %29, %"class.std::__1::vector.78"* dereferenceable(24) %292, i64 1)
          to label %306 unwind label %350

; <label>:306:                                    ; preds = %305
  %307 = bitcast %"class.std::__1::vector.78"* %292 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %307, %"class.std::__1::__vector_base.79"** %23, align 8
  %308 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %23, align 8
  %309 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %308, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %309, %"class.std::__1::__compressed_pair.80"** %22, align 8
  %310 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %22, align 8
  %311 = bitcast %"class.std::__1::__compressed_pair.80"* %310 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %311, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %21, align 8
  %312 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %21, align 8
  %313 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.81"* %312 to %"class.std::__1::allocator.82"*
  %314 = bitcast %"class.std::__1::vector.78"* %292 to %"class.std::__1::__vector_base.79"*
  %315 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %314, i32 0, i32 1
  %316 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %315, align 8
  store %"class.llvm::Type"** %316, %"class.llvm::Type"*** %19, align 8
  %317 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %19, align 8
  %318 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %28, align 8
  store %"class.llvm::Type"** %318, %"class.llvm::Type"*** %3, align 8
  %319 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %3, align 8
  store %"class.std::__1::allocator.82"* %313, %"class.std::__1::allocator.82"** %14, align 8
  store %"class.llvm::Type"** %317, %"class.llvm::Type"*** %15, align 8
  store %"class.llvm::Type"** %319, %"class.llvm::Type"*** %16, align 8
  %320 = bitcast %"struct.std::__1::__has_construct.154"* %18 to %"struct.std::__1::integral_constant.128"*
  %321 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %14, align 8
  %322 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %15, align 8
  %323 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %16, align 8
  store %"class.llvm::Type"** %323, %"class.llvm::Type"*** %13, align 8
  %324 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %13, align 8
  store %"class.std::__1::allocator.82"* %321, %"class.std::__1::allocator.82"** %10, align 8
  store %"class.llvm::Type"** %322, %"class.llvm::Type"*** %11, align 8
  store %"class.llvm::Type"** %324, %"class.llvm::Type"*** %12, align 8
  %325 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %10, align 8
  %326 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %11, align 8
  %327 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %12, align 8
  store %"class.llvm::Type"** %327, %"class.llvm::Type"*** %8, align 8
  %328 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %8, align 8
  store %"class.std::__1::allocator.82"* %325, %"class.std::__1::allocator.82"** %5, align 8
  store %"class.llvm::Type"** %326, %"class.llvm::Type"*** %6, align 8
  store %"class.llvm::Type"** %328, %"class.llvm::Type"*** %7, align 8
  %329 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %5, align 8
  %330 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %6, align 8
  %331 = bitcast %"class.llvm::Type"** %330 to i8*
  %332 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %7, align 8
  store %"class.llvm::Type"** %332, %"class.llvm::Type"*** %4, align 8
  %333 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %4, align 8
  %334 = load %"class.llvm::Type"*, %"class.llvm::Type"** %333, align 8
  store %"class.llvm::Type"* %334, %"class.llvm::Type"** %330, align 8
  call void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE24__RAII_IncreaseAnnotator6__doneEv(%"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"* %29)
  %335 = bitcast %"class.std::__1::vector.78"* %292 to %"class.std::__1::__vector_base.79"*
  %336 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %335, i32 0, i32 1
  %337 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %336, align 8
  %338 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %337, i32 1
  store %"class.llvm::Type"** %338, %"class.llvm::Type"*** %336, align 8
  br label %343

; <label>:339:                                    ; preds = %291
  %340 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %28, align 8
  store %"class.llvm::Type"** %340, %"class.llvm::Type"*** %20, align 8
  %341 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %20, align 8
  invoke void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE21__push_back_slow_pathIS3_EEvOT_(%"class.std::__1::vector.78"* %292, %"class.llvm::Type"** dereferenceable(8) %341)
          to label %342 unwind label %350

; <label>:342:                                    ; preds = %339
  br label %343

; <label>:343:                                    ; preds = %306, %342
  br label %344

; <label>:344:                                    ; preds = %343
  br label %345

; <label>:345:                                    ; preds = %344
  store %"class.std::__1::__wrap_iter"* %99, %"class.std::__1::__wrap_iter"** %2, align 8
  %346 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %2, align 8
  %347 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %346, i32 0, i32 0
  %348 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %347, align 8
  %349 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %348, i32 1
  store %struct.TypeSpecifier* %349, %struct.TypeSpecifier** %347, align 8
  br label %272

; <label>:350:                                    ; preds = %339, %305, %368, %361, %360, %354, %285, %219
  %351 = landingpad { i8*, i32 }
          cleanup
  %352 = extractvalue { i8*, i32 } %351, 0
  store i8* %352, i8** %96, align 8
  %353 = extractvalue { i8*, i32 } %351, 1
  store i32 %353, i32* %97, align 4
  call void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.78"* %93) #2
  br label %468

; <label>:354:                                    ; preds = %272
  %355 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %92, align 8
  %356 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %355, i32 0, i32 25
  %357 = bitcast %union.anon.73* %356 to %struct.TypeSpecifier**
  %358 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %357, align 8
  %359 = invoke %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %358)
          to label %360 unwind label %350

; <label>:360:                                    ; preds = %354
  invoke void @_ZN4llvm8ArrayRefIPNS_4TypeEEC1INSt3__19allocatorIS2_EEEERKNS5_6vectorIS2_T_EE(%"class.llvm::ArrayRef"* %103, %"class.std::__1::vector.78"* dereferenceable(24) %93)
          to label %361 unwind label %350

; <label>:361:                                    ; preds = %360
  %362 = bitcast %"class.llvm::ArrayRef"* %103 to { %"class.llvm::Type"**, i64 }*
  %363 = getelementptr inbounds { %"class.llvm::Type"**, i64 }, { %"class.llvm::Type"**, i64 }* %362, i32 0, i32 0
  %364 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %363, align 8
  %365 = getelementptr inbounds { %"class.llvm::Type"**, i64 }, { %"class.llvm::Type"**, i64 }* %362, i32 0, i32 1
  %366 = load i64, i64* %365, align 8
  %367 = invoke %"class.llvm::FunctionType"* @_ZN4llvm12FunctionType3getEPNS_4TypeENS_8ArrayRefIS2_EEb(%"class.llvm::Type"* %359, %"class.llvm::Type"** %364, i64 %366, i1 zeroext false)
          to label %368 unwind label %350

; <label>:368:                                    ; preds = %361
  %369 = bitcast %"class.llvm::FunctionType"* %367 to %"class.llvm::Type"*
  %370 = invoke %"class.llvm::PointerType"* @_ZNK4llvm4Type12getPointerToEj(%"class.llvm::Type"* %369, i32 0)
          to label %371 unwind label %350

; <label>:371:                                    ; preds = %368
  %372 = bitcast %"class.llvm::PointerType"* %370 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %372, %"class.llvm::Type"** %91, align 8
  call void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.78"* %93) #2
  br label %457

; <label>:373:                                    ; preds = %139
  %374 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %92, align 8
  %375 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %374, i32 0, i32 0
  %376 = load i32, i32* %375, align 8
  switch i32 %376, label %417 [
    i32 0, label %377
    i32 1, label %394
    i32 2, label %397
    i32 3, label %399
    i32 5, label %401
    i32 6, label %404
    i32 9, label %407
    i32 7, label %409
    i32 12, label %409
  ]

; <label>:377:                                    ; preds = %373
  %378 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %92, align 8
  %379 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %378, i32 0, i32 8
  %380 = load i32, i32* %379, align 8
  switch i32 %380, label %388 [
    i32 8, label %381
    i32 16, label %384
    i32 32, label %387
    i32 64, label %391
  ]

; <label>:381:                                    ; preds = %377
  %382 = call %"class.llvm::IntegerType"* @_ZN4llvm4Type9getInt8TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE)
  %383 = bitcast %"class.llvm::IntegerType"* %382 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %383, %"class.llvm::Type"** %91, align 8
  br label %457

; <label>:384:                                    ; preds = %377
  %385 = call %"class.llvm::IntegerType"* @_ZN4llvm4Type10getInt16TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE)
  %386 = bitcast %"class.llvm::IntegerType"* %385 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %386, %"class.llvm::Type"** %91, align 8
  br label %457

; <label>:387:                                    ; preds = %377
  br label %388

; <label>:388:                                    ; preds = %377, %387
  %389 = call %"class.llvm::IntegerType"* @_ZN4llvm4Type10getInt32TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE)
  %390 = bitcast %"class.llvm::IntegerType"* %389 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %390, %"class.llvm::Type"** %91, align 8
  br label %457

; <label>:391:                                    ; preds = %377
  %392 = call %"class.llvm::IntegerType"* @_ZN4llvm4Type10getInt64TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE)
  %393 = bitcast %"class.llvm::IntegerType"* %392 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %393, %"class.llvm::Type"** %91, align 8
  br label %457

; <label>:394:                                    ; preds = %373
  %395 = call %"class.llvm::IntegerType"* @_ZN4llvm4Type10getInt64TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE)
  %396 = bitcast %"class.llvm::IntegerType"* %395 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %396, %"class.llvm::Type"** %91, align 8
  br label %457

; <label>:397:                                    ; preds = %373
  %398 = call %"class.llvm::Type"* @_ZN4llvm4Type10getFloatTyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE)
  store %"class.llvm::Type"* %398, %"class.llvm::Type"** %91, align 8
  br label %457

; <label>:399:                                    ; preds = %373
  %400 = call %"class.llvm::Type"* @_ZN4llvm4Type11getDoubleTyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE)
  store %"class.llvm::Type"* %400, %"class.llvm::Type"** %91, align 8
  br label %457

; <label>:401:                                    ; preds = %373
  %402 = call %"class.llvm::IntegerType"* @_ZN4llvm4Type9getInt1TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE)
  %403 = bitcast %"class.llvm::IntegerType"* %402 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %403, %"class.llvm::Type"** %91, align 8
  br label %457

; <label>:404:                                    ; preds = %373
  %405 = call %"class.llvm::IntegerType"* @_ZN4llvm4Type9getInt8TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE)
  %406 = bitcast %"class.llvm::IntegerType"* %405 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %406, %"class.llvm::Type"** %91, align 8
  br label %457

; <label>:407:                                    ; preds = %373
  %408 = call %"class.llvm::Type"* @_ZN4llvm4Type9getVoidTyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE)
  store %"class.llvm::Type"* %408, %"class.llvm::Type"** %91, align 8
  br label %457

; <label>:409:                                    ; preds = %373, %373
  %410 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %92, align 8
  %411 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %410, i32 0, i32 1
  %412 = call dereferenceable(8) %"class.llvm::StructType"** @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeENS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S9_EEEEEixERSF_(%"class.std::__1::unordered_map"* @_ZN6CGType14StructureTypesE, %"class.std::__1::basic_string"* dereferenceable(24) %411)
  %413 = load %"class.llvm::StructType"*, %"class.llvm::StructType"** %412, align 8
  %414 = bitcast %"class.llvm::StructType"* %413 to %"class.llvm::Type"*
  %415 = call %"class.llvm::PointerType"* @_ZNK4llvm4Type12getPointerToEj(%"class.llvm::Type"* %414, i32 0)
  %416 = bitcast %"class.llvm::PointerType"* %415 to %"class.llvm::Type"*
  store %"class.llvm::Type"* %416, %"class.llvm::Type"** %91, align 8
  br label %457

; <label>:417:                                    ; preds = %373
  %418 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %92, align 8
  call void @_ZNK13TypeSpecifier9to_stringEv(%"class.std::__1::basic_string"* sret %105, %struct.TypeSpecifier* %418)
  store i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str, i32 0, i32 0), i8** %89, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %105, %"class.std::__1::basic_string"** %90, align 8, !noalias !2
  %419 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %90, align 8, !noalias !2
  %420 = load i8*, i8** %89, align 8, !noalias !2
  %421 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %419, i64 0, i8* %420)
          to label %422 unwind label %459

; <label>:422:                                    ; preds = %417
  store %"class.std::__1::basic_string"* %421, %"class.std::__1::basic_string"** %88, align 8, !noalias !2
  %423 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %88, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %104, %"class.std::__1::basic_string"** %86, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %423, %"class.std::__1::basic_string"** %87, align 8, !noalias !2
  %424 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %86, align 8, !noalias !2
  %425 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %87, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %424, %"class.std::__1::basic_string"** %84, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %425, %"class.std::__1::basic_string"** %85, align 8, !noalias !2
  %426 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %84, align 8, !noalias !2
  %427 = bitcast %"class.std::__1::basic_string"* %426 to %"class.std::__1::__basic_string_common"*
  %428 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %426, i32 0, i32 0
  %429 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %85, align 8, !noalias !2
  %430 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %429, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %430, %"class.std::__1::__compressed_pair.36"** %83, align 8, !noalias !2
  %431 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %83, align 8, !noalias !2
  %432 = bitcast %"class.std::__1::__compressed_pair.36"* %428 to i8*
  %433 = bitcast %"class.std::__1::__compressed_pair.36"* %431 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %432, i8* %433, i64 24, i32 8, i1 false) #2
  %434 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %85, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %434, %"class.std::__1::basic_string"** %80, align 8, !noalias !2
  %435 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %80, align 8, !noalias !2
  %436 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %435, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %436, %"class.std::__1::__compressed_pair.36"** %79, align 8, !noalias !2
  %437 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %79, align 8, !noalias !2
  %438 = bitcast %"class.std::__1::__compressed_pair.36"* %437 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %438, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %78, align 8, !noalias !2
  %439 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %78, align 8, !noalias !2
  %440 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %439, i32 0, i32 0
  %441 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %440, i32 0, i32 0
  %442 = bitcast %union.anon* %441 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %443 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %442, i32 0, i32 0
  store [3 x i64]* %443, [3 x i64]** %81, align 8, !noalias !2
  store i32 0, i32* %82, align 4, !noalias !2
  br label %444

; <label>:444:                                    ; preds = %447, %422
  %445 = load i32, i32* %82, align 4, !noalias !2
  %446 = icmp ult i32 %445, 3
  br i1 %446, label %447, label %454

; <label>:447:                                    ; preds = %444
  %448 = load i32, i32* %82, align 4, !noalias !2
  %449 = zext i32 %448 to i64
  %450 = load [3 x i64]*, [3 x i64]** %81, align 8, !noalias !2
  %451 = getelementptr inbounds [3 x i64], [3 x i64]* %450, i64 0, i64 %449
  store i64 0, i64* %451, align 8
  %452 = load i32, i32* %82, align 4, !noalias !2
  %453 = add i32 %452, 1
  store i32 %453, i32* %82, align 4, !noalias !2
  br label %444

; <label>:454:                                    ; preds = %444
  br label %455

; <label>:455:                                    ; preds = %454
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 7, %"class.std::__1::basic_string"* %104, %class.AstNode* null)
          to label %456 unwind label %463

; <label>:456:                                    ; preds = %455
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %104) #2
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %105) #2
  br label %457

; <label>:457:                                    ; preds = %110, %123, %136, %371, %381, %384, %388, %391, %394, %397, %399, %401, %404, %407, %409, %456
  %458 = load %"class.llvm::Type"*, %"class.llvm::Type"** %91, align 8
  ret %"class.llvm::Type"* %458

; <label>:459:                                    ; preds = %417
  %460 = landingpad { i8*, i32 }
          cleanup
  %461 = extractvalue { i8*, i32 } %460, 0
  store i8* %461, i8** %96, align 8
  %462 = extractvalue { i8*, i32 } %460, 1
  store i32 %462, i32* %97, align 4
  br label %467

; <label>:463:                                    ; preds = %455
  %464 = landingpad { i8*, i32 }
          cleanup
  %465 = extractvalue { i8*, i32 } %464, 0
  store i8* %465, i8** %96, align 8
  %466 = extractvalue { i8*, i32 } %464, 1
  store i32 %466, i32* %97, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %104) #2
  br label %467

; <label>:467:                                    ; preds = %463, %459
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %105) #2
  br label %468

; <label>:468:                                    ; preds = %467, %350
  %469 = load i8*, i8** %96, align 8
  %470 = load i32, i32* %97, align 4
  %471 = insertvalue { i8*, i32 } undef, i8* %469, 0
  %472 = insertvalue { i8*, i32 } %471, i32 %470, 1
  resume { i8*, i32 } %472
}

declare %"class.llvm::PointerType"* @_ZNK4llvm4Type12getPointerToEj(%"class.llvm::Type"*, i32) #3

; Function Attrs: ssp uwtable
define linkonce_odr dereferenceable(8) %"class.llvm::StructType"** @_ZNSt3__113unordered_mapIjPN4llvm10StructTypeENS_4hashIjEENS_8equal_toIjEENS_9allocatorINS_4pairIKjS3_EEEEEixERSA_(%"class.std::__1::unordered_map.10"*, i32* dereferenceable(4)) #0 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %3 = alloca %"class.std::__1::tuple.131"*, align 8
  %4 = alloca %"class.std::__1::tuple.131"*, align 8
  %5 = alloca %"class.std::__1::tuple.131", align 1
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  %8 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.31"*, align 8
  %9 = alloca %"class.std::__1::__compressed_pair.30"*, align 8
  %10 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %11 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %12 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %13 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %14 = alloca %"struct.std::__1::equal_to.33"*, align 8
  %15 = alloca i32*, align 8
  %16 = alloca i32*, align 8
  %17 = alloca %"class.std::__1::__unordered_map_equal.32"*, align 8
  %18 = alloca %"union.std::__1::__hash_value_type.124"*, align 8
  %19 = alloca i32*, align 8
  %20 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %21 = alloca %"class.std::__1::tuple.141"*, align 8
  %22 = alloca %"class.std::__1::tuple.131"*, align 8
  %23 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %24 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.26"*, align 8
  %25 = alloca %"class.std::__1::__compressed_pair.25"*, align 8
  %26 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %27 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.31"*, align 8
  %28 = alloca %"class.std::__1::__compressed_pair.30"*, align 8
  %29 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %30 = alloca i64, align 8
  %31 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.26"*, align 8
  %32 = alloca %"class.std::__1::__compressed_pair.25"*, align 8
  %33 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %34 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.31"*, align 8
  %35 = alloca %"class.std::__1::__compressed_pair.30"*, align 8
  %36 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %37 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %38 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %39 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %40 = alloca i64, align 8
  %41 = alloca float, align 4
  %42 = alloca %"struct.std::__1::__less"*, align 8
  %43 = alloca i64*, align 8
  %44 = alloca i64*, align 8
  %45 = alloca %"struct.std::__1::__less", align 1
  %46 = alloca i64*, align 8
  %47 = alloca i64*, align 8
  %48 = alloca i64*, align 8
  %49 = alloca i64*, align 8
  %50 = alloca %"struct.std::__1::__less", align 1
  %51 = alloca i64, align 8
  %52 = alloca i64, align 8
  %53 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.18"*, align 8
  %54 = alloca %"class.std::__1::__compressed_pair.17"*, align 8
  %55 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %56 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %57 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %58 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %59 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %60 = alloca i64, align 8
  %61 = alloca i64, align 8
  %62 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.18"*, align 8
  %63 = alloca %"class.std::__1::__compressed_pair.17"*, align 8
  %64 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %65 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %66 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %67 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %68 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %69 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %70 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %71 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %72 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %73 = alloca i8*, align 8
  %74 = alloca %"class.std::__1::allocator.23"*, align 8
  %75 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %76 = alloca i64, align 8
  %77 = alloca %"class.std::__1::allocator.23"*, align 8
  %78 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %79 = alloca i64, align 8
  %80 = alloca %"class.std::__1::allocator.23"*, align 8
  %81 = alloca %"struct.std::__1::pair.125"*, align 8
  %82 = alloca %"class.std::__1::allocator.23"*, align 8
  %83 = alloca %"struct.std::__1::pair.125"*, align 8
  %84 = alloca %"struct.std::__1::integral_constant", align 1
  %85 = alloca %"struct.std::__1::__has_destroy.127", align 1
  %86 = alloca %"struct.std::__1::pair.125"*, align 8
  %87 = alloca %"union.std::__1::__hash_value_type.124"*, align 8
  %88 = alloca %"class.std::__1::__hash_node_destructor.147"*, align 8
  %89 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %90 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %91 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %92 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %93 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %94 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %95 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %96 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %97 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %98 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %99 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %100 = alloca i64, align 8
  %101 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.22"*, align 8
  %102 = alloca %"class.std::__1::__compressed_pair.21"*, align 8
  %103 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %104 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %105 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %106 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %107 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %108 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %109 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %110 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %111 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %112 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %113 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %114 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %115 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %116 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %117 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %118 = alloca i64, align 8
  %119 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %120 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %121 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %122 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %123 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %124 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %125 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %126 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %127 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %128 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %129 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %130 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %131 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %132 = alloca i64, align 8
  %133 = alloca i64, align 8
  %134 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %135 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %136 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %137 = alloca i64, align 8
  %138 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %139 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %140 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %141 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %142 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %143 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %144 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %145 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %146 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %147 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %148 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %149 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %150 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.26"*, align 8
  %151 = alloca %"class.std::__1::__compressed_pair.25"*, align 8
  %152 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %153 = alloca %"struct.std::__1::hash.28"*, align 8
  %154 = alloca i32, align 4
  %155 = alloca %"class.std::__1::__unordered_map_hasher.27"*, align 8
  %156 = alloca i32*, align 8
  %157 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %158 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %159 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %160 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %161 = alloca i8*, align 8
  %162 = alloca %"class.std::__1::allocator.23"*, align 8
  %163 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %164 = alloca i64, align 8
  %165 = alloca %"class.std::__1::allocator.23"*, align 8
  %166 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %167 = alloca i64, align 8
  %168 = alloca %"class.std::__1::allocator.23"*, align 8
  %169 = alloca %"struct.std::__1::pair.125"*, align 8
  %170 = alloca %"class.std::__1::allocator.23"*, align 8
  %171 = alloca %"struct.std::__1::pair.125"*, align 8
  %172 = alloca %"struct.std::__1::integral_constant", align 1
  %173 = alloca %"struct.std::__1::__has_destroy.127", align 1
  %174 = alloca %"struct.std::__1::pair.125"*, align 8
  %175 = alloca %"union.std::__1::__hash_value_type.124"*, align 8
  %176 = alloca %"class.std::__1::__hash_node_destructor.147"*, align 8
  %177 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %178 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %179 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %180 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %181 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %182 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %183 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %184 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %185 = alloca %"class.std::__1::__hash_iterator.139"*, align 8
  %186 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %187 = alloca %"class.std::__1::__hash_iterator.139"*, align 8
  %188 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %189 = alloca i8*, align 8
  %190 = alloca %"class.std::__1::__hash_iterator.139"*, align 8
  %191 = alloca %"struct.std::__1::pair.138"*, align 8
  %192 = alloca %"class.std::__1::__hash_iterator.139"*, align 8
  %193 = alloca i8*, align 8
  %194 = alloca %"struct.std::__1::pair.138"*, align 8
  %195 = alloca %"class.std::__1::__hash_iterator.139"*, align 8
  %196 = alloca i8*, align 8
  %197 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.26"*, align 8
  %198 = alloca %"class.std::__1::__compressed_pair.25"*, align 8
  %199 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %200 = alloca %"struct.std::__1::pair.138", align 8
  %201 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %202 = alloca i32*, align 8
  %203 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %204 = alloca %"class.std::__1::tuple.141"*, align 8
  %205 = alloca %"class.std::__1::tuple.131"*, align 8
  %206 = alloca i64, align 8
  %207 = alloca i64, align 8
  %208 = alloca i8, align 1
  %209 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %210 = alloca i64, align 8
  %211 = alloca %"class.std::__1::unique_ptr.144", align 8
  %212 = alloca i64, align 8
  %213 = alloca i8*
  %214 = alloca i32
  %215 = alloca i64, align 8
  %216 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %217 = alloca %"class.std::__1::__hash_iterator.139", align 8
  %218 = alloca %"union.std::__1::__hash_value_type.124"*, align 8
  %219 = alloca %"union.std::__1::__hash_value_type.124"*, align 8
  %220 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %221 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %222 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %223 = alloca %"class.std::__1::__hash_iterator.139"*, align 8
  %224 = alloca i32*, align 8
  %225 = alloca %"class.std::__1::__tuple_leaf.143"*, align 8
  %226 = alloca i32*, align 8
  %227 = alloca i32*, align 8
  %228 = alloca %"struct.std::__1::__tuple_indices", align 1
  %229 = alloca %"struct.std::__1::__tuple_types.150", align 1
  %230 = alloca %"struct.std::__1::__tuple_indices.136", align 1
  %231 = alloca %"struct.std::__1::__tuple_types.137", align 1
  %232 = alloca %"struct.std::__1::__tuple_impl.142"*, align 8
  %233 = alloca i32*, align 8
  %234 = alloca %"struct.std::__1::__tuple_indices", align 1
  %235 = alloca %"struct.std::__1::__tuple_types.150", align 1
  %236 = alloca %"struct.std::__1::__tuple_indices.136", align 1
  %237 = alloca %"struct.std::__1::__tuple_types.137", align 1
  %238 = alloca %"struct.std::__1::__tuple_impl.142"*, align 8
  %239 = alloca i32*, align 8
  %240 = alloca %"class.std::__1::tuple.141"*, align 8
  %241 = alloca i32*, align 8
  %242 = alloca %"struct.std::__1::__tuple_indices", align 1
  %243 = alloca %"struct.std::__1::__tuple_types.150", align 1
  %244 = alloca %"struct.std::__1::__tuple_indices.136", align 1
  %245 = alloca %"struct.std::__1::__tuple_types.137", align 1
  %246 = alloca %"class.std::__1::tuple.141"*, align 8
  %247 = alloca i32*, align 8
  %248 = alloca i32*, align 8
  %249 = alloca %"class.std::__1::tuple.141", align 8
  %250 = alloca i32*, align 8
  %251 = alloca %"class.std::__1::unordered_map.10"*, align 8
  %252 = alloca i32*, align 8
  %253 = alloca %"class.std::__1::tuple.141", align 8
  %254 = alloca %"class.std::__1::tuple.131", align 1
  %255 = alloca %"class.std::__1::tuple.131", align 1
  %256 = alloca %"struct.std::__1::pair.138", align 8
  store %"class.std::__1::unordered_map.10"* %0, %"class.std::__1::unordered_map.10"** %251, align 8
  store i32* %1, i32** %252, align 8
  %257 = load %"class.std::__1::unordered_map.10"*, %"class.std::__1::unordered_map.10"** %251, align 8
  %258 = getelementptr inbounds %"class.std::__1::unordered_map.10", %"class.std::__1::unordered_map.10"* %257, i32 0, i32 0
  %259 = load i32*, i32** %252, align 8
  %260 = load i32*, i32** %252, align 8
  store i32* %260, i32** %250, align 8
  %261 = load i32*, i32** %250, align 8
  store i32* %261, i32** %248, align 8
  %262 = load i32*, i32** %248, align 8
  store %"class.std::__1::tuple.141"* %249, %"class.std::__1::tuple.141"** %246, align 8
  store i32* %262, i32** %247, align 8
  %263 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %246, align 8
  %264 = load i32*, i32** %247, align 8
  store %"class.std::__1::tuple.141"* %263, %"class.std::__1::tuple.141"** %240, align 8
  store i32* %264, i32** %241, align 8
  %265 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %240, align 8
  %266 = getelementptr inbounds %"class.std::__1::tuple.141", %"class.std::__1::tuple.141"* %265, i32 0, i32 0
  %267 = load i32*, i32** %241, align 8
  store %"struct.std::__1::__tuple_impl.142"* %266, %"struct.std::__1::__tuple_impl.142"** %238, align 8
  store i32* %267, i32** %239, align 8
  %268 = load %"struct.std::__1::__tuple_impl.142"*, %"struct.std::__1::__tuple_impl.142"** %238, align 8
  %269 = load i32*, i32** %239, align 8
  store %"struct.std::__1::__tuple_impl.142"* %268, %"struct.std::__1::__tuple_impl.142"** %232, align 8
  store i32* %269, i32** %233, align 8
  %270 = load %"struct.std::__1::__tuple_impl.142"*, %"struct.std::__1::__tuple_impl.142"** %232, align 8
  %271 = bitcast %"struct.std::__1::__tuple_impl.142"* %270 to %"class.std::__1::__tuple_leaf.143"*
  %272 = load i32*, i32** %233, align 8
  store i32* %272, i32** %227, align 8
  %273 = load i32*, i32** %227, align 8
  store %"class.std::__1::__tuple_leaf.143"* %271, %"class.std::__1::__tuple_leaf.143"** %225, align 8
  store i32* %273, i32** %226, align 8
  %274 = load %"class.std::__1::__tuple_leaf.143"*, %"class.std::__1::__tuple_leaf.143"** %225, align 8
  %275 = getelementptr inbounds %"class.std::__1::__tuple_leaf.143", %"class.std::__1::__tuple_leaf.143"* %274, i32 0, i32 0
  %276 = load i32*, i32** %226, align 8
  store i32* %276, i32** %224, align 8
  %277 = load i32*, i32** %224, align 8
  store i32* %277, i32** %275, align 8
  %278 = getelementptr inbounds %"class.std::__1::tuple.141", %"class.std::__1::tuple.141"* %249, i32 0, i32 0
  %279 = getelementptr inbounds %"struct.std::__1::__tuple_impl.142", %"struct.std::__1::__tuple_impl.142"* %278, i32 0, i32 0
  %280 = getelementptr inbounds %"class.std::__1::__tuple_leaf.143", %"class.std::__1::__tuple_leaf.143"* %279, i32 0, i32 0
  %281 = load i32*, i32** %280, align 8
  %282 = getelementptr inbounds %"class.std::__1::tuple.141", %"class.std::__1::tuple.141"* %253, i32 0, i32 0
  %283 = getelementptr inbounds %"struct.std::__1::__tuple_impl.142", %"struct.std::__1::__tuple_impl.142"* %282, i32 0, i32 0
  %284 = getelementptr inbounds %"class.std::__1::__tuple_leaf.143", %"class.std::__1::__tuple_leaf.143"* %283, i32 0, i32 0
  store i32* %281, i32** %284, align 8
  store %"class.std::__1::tuple.131"* %5, %"class.std::__1::tuple.131"** %4, align 8
  %285 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %4, align 8
  store %"class.std::__1::tuple.131"* %285, %"class.std::__1::tuple.131"** %3, align 8
  %286 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %3, align 8
  store %"class.std::__1::__hash_table.11"* %258, %"class.std::__1::__hash_table.11"** %201, align 8
  store i32* %259, i32** %202, align 8
  store %"struct.std::__1::piecewise_construct_t"* @_ZNSt3__1L19piecewise_constructE, %"struct.std::__1::piecewise_construct_t"** %203, align 8
  store %"class.std::__1::tuple.141"* %253, %"class.std::__1::tuple.141"** %204, align 8
  store %"class.std::__1::tuple.131"* %254, %"class.std::__1::tuple.131"** %205, align 8
  %287 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %201, align 8
  store %"class.std::__1::__hash_table.11"* %287, %"class.std::__1::__hash_table.11"** %199, align 8
  %288 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %199, align 8
  %289 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %288, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.25"* %289, %"class.std::__1::__compressed_pair.25"** %198, align 8
  %290 = load %"class.std::__1::__compressed_pair.25"*, %"class.std::__1::__compressed_pair.25"** %198, align 8
  %291 = bitcast %"class.std::__1::__compressed_pair.25"* %290 to %"class.std::__1::__libcpp_compressed_pair_imp.26"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.26"* %291, %"class.std::__1::__libcpp_compressed_pair_imp.26"** %197, align 8
  %292 = load %"class.std::__1::__libcpp_compressed_pair_imp.26"*, %"class.std::__1::__libcpp_compressed_pair_imp.26"** %197, align 8
  %293 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.26"* %292 to %"class.std::__1::__unordered_map_hasher.27"*
  %294 = load i32*, i32** %202, align 8
  store %"class.std::__1::__unordered_map_hasher.27"* %293, %"class.std::__1::__unordered_map_hasher.27"** %155, align 8
  store i32* %294, i32** %156, align 8
  %295 = load %"class.std::__1::__unordered_map_hasher.27"*, %"class.std::__1::__unordered_map_hasher.27"** %155, align 8
  %296 = bitcast %"class.std::__1::__unordered_map_hasher.27"* %295 to %"struct.std::__1::hash.28"*
  %297 = load i32*, i32** %156, align 8
  %298 = load i32, i32* %297, align 4
  store %"struct.std::__1::hash.28"* %296, %"struct.std::__1::hash.28"** %153, align 8
  store i32 %298, i32* %154, align 4
  %299 = load %"struct.std::__1::hash.28"*, %"struct.std::__1::hash.28"** %153, align 8
  %300 = load i32, i32* %154, align 4
  %301 = zext i32 %300 to i64
  store i64 %301, i64* %206, align 8
  store %"class.std::__1::__hash_table.11"* %287, %"class.std::__1::__hash_table.11"** %68, align 8
  %302 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %68, align 8
  %303 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %302, i32 0, i32 0
  store %"class.std::__1::unique_ptr.12"* %303, %"class.std::__1::unique_ptr.12"** %67, align 8
  %304 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %67, align 8
  %305 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %304, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %305, %"class.std::__1::__compressed_pair.13"** %66, align 8
  %306 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %66, align 8
  %307 = bitcast %"class.std::__1::__compressed_pair.13"* %306 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %307, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %65, align 8
  %308 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %65, align 8
  %309 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %308, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator.16"* %309, %"class.std::__1::__bucket_list_deallocator.16"** %64, align 8
  %310 = load %"class.std::__1::__bucket_list_deallocator.16"*, %"class.std::__1::__bucket_list_deallocator.16"** %64, align 8
  %311 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.16", %"class.std::__1::__bucket_list_deallocator.16"* %310, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.17"* %311, %"class.std::__1::__compressed_pair.17"** %63, align 8
  %312 = load %"class.std::__1::__compressed_pair.17"*, %"class.std::__1::__compressed_pair.17"** %63, align 8
  %313 = bitcast %"class.std::__1::__compressed_pair.17"* %312 to %"class.std::__1::__libcpp_compressed_pair_imp.18"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.18"* %313, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %62, align 8
  %314 = load %"class.std::__1::__libcpp_compressed_pair_imp.18"*, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %62, align 8
  %315 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.18", %"class.std::__1::__libcpp_compressed_pair_imp.18"* %314, i32 0, i32 0
  %316 = load i64, i64* %315, align 8
  store i64 %316, i64* %207, align 8
  store i8 0, i8* %208, align 1
  %317 = load i64, i64* %207, align 8
  %318 = icmp ne i64 %317, 0
  br i1 %318, label %319, label %436

; <label>:319:                                    ; preds = %2
  %320 = load i64, i64* %206, align 8
  %321 = load i64, i64* %207, align 8
  store i64 %320, i64* %51, align 8
  store i64 %321, i64* %52, align 8
  %322 = load i64, i64* %52, align 8
  %323 = load i64, i64* %52, align 8
  %324 = sub i64 %323, 1
  %325 = and i64 %322, %324
  %326 = icmp ne i64 %325, 0
  br i1 %326, label %332, label %327

; <label>:327:                                    ; preds = %319
  %328 = load i64, i64* %51, align 8
  %329 = load i64, i64* %52, align 8
  %330 = sub i64 %329, 1
  %331 = and i64 %328, %330
  br label %344

; <label>:332:                                    ; preds = %319
  %333 = load i64, i64* %51, align 8
  %334 = load i64, i64* %52, align 8
  %335 = icmp ult i64 %333, %334
  br i1 %335, label %336, label %338

; <label>:336:                                    ; preds = %332
  %337 = load i64, i64* %51, align 8
  br label %342

; <label>:338:                                    ; preds = %332
  %339 = load i64, i64* %51, align 8
  %340 = load i64, i64* %52, align 8
  %341 = urem i64 %339, %340
  br label %342

; <label>:342:                                    ; preds = %338, %336
  %343 = phi i64 [ %337, %336 ], [ %341, %338 ]
  br label %344

; <label>:344:                                    ; preds = %342, %327
  %345 = phi i64 [ %331, %327 ], [ %343, %342 ]
  store i64 %345, i64* %210, align 8
  %346 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %287, i32 0, i32 0
  %347 = load i64, i64* %210, align 8
  store %"class.std::__1::unique_ptr.12"* %346, %"class.std::__1::unique_ptr.12"** %39, align 8
  store i64 %347, i64* %40, align 8
  %348 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %39, align 8
  %349 = load i64, i64* %40, align 8
  %350 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %348, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %350, %"class.std::__1::__compressed_pair.13"** %38, align 8
  %351 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %38, align 8
  %352 = bitcast %"class.std::__1::__compressed_pair.13"* %351 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %352, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %37, align 8
  %353 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %37, align 8
  %354 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %353, i32 0, i32 0
  %355 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %354, align 8
  %356 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %355, i64 %349
  %357 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %356, align 8
  store %"struct.std::__1::__hash_node_base.15"* %357, %"struct.std::__1::__hash_node_base.15"** %209, align 8
  %358 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %209, align 8
  %359 = icmp ne %"struct.std::__1::__hash_node_base.15"* %358, null
  br i1 %359, label %360, label %435

; <label>:360:                                    ; preds = %344
  %361 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %209, align 8
  %362 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %361, i32 0, i32 0
  %363 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %362, align 8
  store %"struct.std::__1::__hash_node_base.15"* %363, %"struct.std::__1::__hash_node_base.15"** %209, align 8
  br label %364

; <label>:364:                                    ; preds = %430, %360
  %365 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %209, align 8
  %366 = icmp ne %"struct.std::__1::__hash_node_base.15"* %365, null
  br i1 %366, label %367, label %400

; <label>:367:                                    ; preds = %364
  %368 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %209, align 8
  store %"struct.std::__1::__hash_node_base.15"* %368, %"struct.std::__1::__hash_node_base.15"** %23, align 8
  %369 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %23, align 8
  %370 = bitcast %"struct.std::__1::__hash_node_base.15"* %369 to %"struct.std::__1::__hash_node.123"*
  %371 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %370, i32 0, i32 1
  %372 = load i64, i64* %371, align 8
  %373 = load i64, i64* %207, align 8
  store i64 %372, i64* %6, align 8
  store i64 %373, i64* %7, align 8
  %374 = load i64, i64* %7, align 8
  %375 = load i64, i64* %7, align 8
  %376 = sub i64 %375, 1
  %377 = and i64 %374, %376
  %378 = icmp ne i64 %377, 0
  br i1 %378, label %384, label %379

; <label>:379:                                    ; preds = %367
  %380 = load i64, i64* %6, align 8
  %381 = load i64, i64* %7, align 8
  %382 = sub i64 %381, 1
  %383 = and i64 %380, %382
  br label %396

; <label>:384:                                    ; preds = %367
  %385 = load i64, i64* %6, align 8
  %386 = load i64, i64* %7, align 8
  %387 = icmp ult i64 %385, %386
  br i1 %387, label %388, label %390

; <label>:388:                                    ; preds = %384
  %389 = load i64, i64* %6, align 8
  br label %394

; <label>:390:                                    ; preds = %384
  %391 = load i64, i64* %6, align 8
  %392 = load i64, i64* %7, align 8
  %393 = urem i64 %391, %392
  br label %394

; <label>:394:                                    ; preds = %390, %388
  %395 = phi i64 [ %389, %388 ], [ %393, %390 ]
  br label %396

; <label>:396:                                    ; preds = %394, %379
  %397 = phi i64 [ %383, %379 ], [ %395, %394 ]
  %398 = load i64, i64* %210, align 8
  %399 = icmp eq i64 %397, %398
  br label %400

; <label>:400:                                    ; preds = %396, %364
  %401 = phi i1 [ false, %364 ], [ %399, %396 ]
  br i1 %401, label %402, label %434

; <label>:402:                                    ; preds = %400
  store %"class.std::__1::__hash_table.11"* %287, %"class.std::__1::__hash_table.11"** %10, align 8
  %403 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %10, align 8
  %404 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %403, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.30"* %404, %"class.std::__1::__compressed_pair.30"** %9, align 8
  %405 = load %"class.std::__1::__compressed_pair.30"*, %"class.std::__1::__compressed_pair.30"** %9, align 8
  %406 = bitcast %"class.std::__1::__compressed_pair.30"* %405 to %"class.std::__1::__libcpp_compressed_pair_imp.31"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.31"* %406, %"class.std::__1::__libcpp_compressed_pair_imp.31"** %8, align 8
  %407 = load %"class.std::__1::__libcpp_compressed_pair_imp.31"*, %"class.std::__1::__libcpp_compressed_pair_imp.31"** %8, align 8
  %408 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.31"* %407 to %"class.std::__1::__unordered_map_equal.32"*
  %409 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %209, align 8
  store %"struct.std::__1::__hash_node_base.15"* %409, %"struct.std::__1::__hash_node_base.15"** %13, align 8
  %410 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %13, align 8
  store %"struct.std::__1::__hash_node_base.15"* %410, %"struct.std::__1::__hash_node_base.15"** %12, align 8
  %411 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %12, align 8
  store %"struct.std::__1::__hash_node_base.15"* %411, %"struct.std::__1::__hash_node_base.15"** %11, align 8
  %412 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %11, align 8
  %413 = bitcast %"struct.std::__1::__hash_node_base.15"* %412 to i8*
  %414 = bitcast %"struct.std::__1::__hash_node_base.15"* %412 to %"struct.std::__1::__hash_node.123"*
  %415 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %414, i32 0, i32 2
  %416 = load i32*, i32** %202, align 8
  store %"class.std::__1::__unordered_map_equal.32"* %408, %"class.std::__1::__unordered_map_equal.32"** %17, align 8
  store %"union.std::__1::__hash_value_type.124"* %415, %"union.std::__1::__hash_value_type.124"** %18, align 8
  store i32* %416, i32** %19, align 8
  %417 = load %"class.std::__1::__unordered_map_equal.32"*, %"class.std::__1::__unordered_map_equal.32"** %17, align 8
  %418 = bitcast %"class.std::__1::__unordered_map_equal.32"* %417 to %"struct.std::__1::equal_to.33"*
  %419 = load %"union.std::__1::__hash_value_type.124"*, %"union.std::__1::__hash_value_type.124"** %18, align 8
  %420 = bitcast %"union.std::__1::__hash_value_type.124"* %419 to %"struct.std::__1::pair.125"*
  %421 = getelementptr inbounds %"struct.std::__1::pair.125", %"struct.std::__1::pair.125"* %420, i32 0, i32 0
  %422 = load i32*, i32** %19, align 8
  store %"struct.std::__1::equal_to.33"* %418, %"struct.std::__1::equal_to.33"** %14, align 8
  store i32* %421, i32** %15, align 8
  store i32* %422, i32** %16, align 8
  %423 = load %"struct.std::__1::equal_to.33"*, %"struct.std::__1::equal_to.33"** %14, align 8
  %424 = load i32*, i32** %15, align 8
  %425 = load i32, i32* %424, align 4
  %426 = load i32*, i32** %16, align 8
  %427 = load i32, i32* %426, align 4
  %428 = icmp eq i32 %425, %427
  br i1 %428, label %429, label %430

; <label>:429:                                    ; preds = %402
  br label %869

; <label>:430:                                    ; preds = %402
  %431 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %209, align 8
  %432 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %431, i32 0, i32 0
  %433 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %432, align 8
  store %"struct.std::__1::__hash_node_base.15"* %433, %"struct.std::__1::__hash_node_base.15"** %209, align 8
  br label %364

; <label>:434:                                    ; preds = %400
  br label %435

; <label>:435:                                    ; preds = %434, %344
  br label %436

; <label>:436:                                    ; preds = %435, %2
  %437 = load i64, i64* %206, align 8
  %438 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %203, align 8
  store %"struct.std::__1::piecewise_construct_t"* %438, %"struct.std::__1::piecewise_construct_t"** %20, align 8
  %439 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %20, align 8
  %440 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %204, align 8
  store %"class.std::__1::tuple.141"* %440, %"class.std::__1::tuple.141"** %21, align 8
  %441 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %21, align 8
  %442 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %205, align 8
  store %"class.std::__1::tuple.131"* %442, %"class.std::__1::tuple.131"** %22, align 8
  %443 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %22, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeIjPN4llvm10StructTypeEEENS_22__unordered_map_hasherIjS5_NS_4hashIjEELb1EEENS_21__unordered_map_equalIjS5_NS_8equal_toIjEELb1EEENS_9allocatorIS5_EEE21__construct_node_hashIRKNS_21piecewise_construct_tEJNS_5tupleIJRKjEEENSL_IJEEEEEENS_10unique_ptrINS_11__hash_nodeIS5_PvEENS_22__hash_node_destructorINSE_IST_EEEEEEmOT_DpOT0_(%"class.std::__1::unique_ptr.144"* sret %211, %"class.std::__1::__hash_table.11"* %287, i64 %437, %"struct.std::__1::piecewise_construct_t"* dereferenceable(1) %439, %"class.std::__1::tuple.141"* dereferenceable(8) %441, %"class.std::__1::tuple.131"* dereferenceable(1) %443)
  store %"class.std::__1::__hash_table.11"* %287, %"class.std::__1::__hash_table.11"** %26, align 8
  %444 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %26, align 8
  %445 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %444, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.25"* %445, %"class.std::__1::__compressed_pair.25"** %25, align 8
  %446 = load %"class.std::__1::__compressed_pair.25"*, %"class.std::__1::__compressed_pair.25"** %25, align 8
  %447 = bitcast %"class.std::__1::__compressed_pair.25"* %446 to %"class.std::__1::__libcpp_compressed_pair_imp.26"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.26"* %447, %"class.std::__1::__libcpp_compressed_pair_imp.26"** %24, align 8
  %448 = load %"class.std::__1::__libcpp_compressed_pair_imp.26"*, %"class.std::__1::__libcpp_compressed_pair_imp.26"** %24, align 8
  %449 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.26", %"class.std::__1::__libcpp_compressed_pair_imp.26"* %448, i32 0, i32 0
  %450 = load i64, i64* %449, align 8
  %451 = add i64 %450, 1
  %452 = uitofp i64 %451 to float
  %453 = load i64, i64* %207, align 8
  %454 = uitofp i64 %453 to float
  store %"class.std::__1::__hash_table.11"* %287, %"class.std::__1::__hash_table.11"** %29, align 8
  %455 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %29, align 8
  %456 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %455, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.30"* %456, %"class.std::__1::__compressed_pair.30"** %28, align 8
  %457 = load %"class.std::__1::__compressed_pair.30"*, %"class.std::__1::__compressed_pair.30"** %28, align 8
  %458 = bitcast %"class.std::__1::__compressed_pair.30"* %457 to %"class.std::__1::__libcpp_compressed_pair_imp.31"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.31"* %458, %"class.std::__1::__libcpp_compressed_pair_imp.31"** %27, align 8
  %459 = load %"class.std::__1::__libcpp_compressed_pair_imp.31"*, %"class.std::__1::__libcpp_compressed_pair_imp.31"** %27, align 8
  %460 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.31", %"class.std::__1::__libcpp_compressed_pair_imp.31"* %459, i32 0, i32 0
  %461 = load float, float* %460, align 4
  %462 = fmul float %454, %461
  %463 = fcmp ogt float %452, %462
  br i1 %463, label %467, label %464

; <label>:464:                                    ; preds = %436
  %465 = load i64, i64* %207, align 8
  %466 = icmp eq i64 %465, 0
  br i1 %466, label %467, label %629

; <label>:467:                                    ; preds = %464, %436
  %468 = load i64, i64* %207, align 8
  %469 = mul i64 2, %468
  %470 = load i64, i64* %207, align 8
  store i64 %470, i64* %30, align 8
  %471 = load i64, i64* %30, align 8
  %472 = icmp ugt i64 %471, 2
  br i1 %472, label %473, label %480

; <label>:473:                                    ; preds = %467
  %474 = load i64, i64* %30, align 8
  %475 = load i64, i64* %30, align 8
  %476 = sub i64 %475, 1
  %477 = and i64 %474, %476
  %478 = icmp ne i64 %477, 0
  %479 = xor i1 %478, true
  br label %480

; <label>:480:                                    ; preds = %473, %467
  %481 = phi i1 [ false, %467 ], [ %479, %473 ]
  %482 = xor i1 %481, true
  %483 = zext i1 %482 to i64
  %484 = add i64 %469, %483
  store i64 %484, i64* %212, align 8
  store %"class.std::__1::__hash_table.11"* %287, %"class.std::__1::__hash_table.11"** %33, align 8
  %485 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %33, align 8
  %486 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %485, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.25"* %486, %"class.std::__1::__compressed_pair.25"** %32, align 8
  %487 = load %"class.std::__1::__compressed_pair.25"*, %"class.std::__1::__compressed_pair.25"** %32, align 8
  %488 = bitcast %"class.std::__1::__compressed_pair.25"* %487 to %"class.std::__1::__libcpp_compressed_pair_imp.26"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.26"* %488, %"class.std::__1::__libcpp_compressed_pair_imp.26"** %31, align 8
  %489 = load %"class.std::__1::__libcpp_compressed_pair_imp.26"*, %"class.std::__1::__libcpp_compressed_pair_imp.26"** %31, align 8
  %490 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.26", %"class.std::__1::__libcpp_compressed_pair_imp.26"* %489, i32 0, i32 0
  %491 = load i64, i64* %490, align 8
  %492 = add i64 %491, 1
  %493 = uitofp i64 %492 to float
  store %"class.std::__1::__hash_table.11"* %287, %"class.std::__1::__hash_table.11"** %36, align 8
  %494 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %36, align 8
  %495 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %494, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.30"* %495, %"class.std::__1::__compressed_pair.30"** %35, align 8
  %496 = load %"class.std::__1::__compressed_pair.30"*, %"class.std::__1::__compressed_pair.30"** %35, align 8
  %497 = bitcast %"class.std::__1::__compressed_pair.30"* %496 to %"class.std::__1::__libcpp_compressed_pair_imp.31"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.31"* %497, %"class.std::__1::__libcpp_compressed_pair_imp.31"** %34, align 8
  %498 = load %"class.std::__1::__libcpp_compressed_pair_imp.31"*, %"class.std::__1::__libcpp_compressed_pair_imp.31"** %34, align 8
  %499 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.31", %"class.std::__1::__libcpp_compressed_pair_imp.31"* %498, i32 0, i32 0
  %500 = load float, float* %499, align 4
  %501 = fdiv float %493, %500
  store float %501, float* %41, align 4
  %502 = load float, float* %41, align 4
  %503 = call float @ceilf(float %502) #12
  %504 = fptoui float %503 to i64
  store i64 %504, i64* %215, align 8
  store i64* %212, i64** %48, align 8
  store i64* %215, i64** %49, align 8
  %505 = load i64*, i64** %48, align 8
  %506 = load i64*, i64** %49, align 8
  store i64* %505, i64** %46, align 8
  store i64* %506, i64** %47, align 8
  %507 = load i64*, i64** %46, align 8
  %508 = load i64*, i64** %47, align 8
  store %"struct.std::__1::__less"* %45, %"struct.std::__1::__less"** %42, align 8
  store i64* %507, i64** %43, align 8
  store i64* %508, i64** %44, align 8
  %509 = load %"struct.std::__1::__less"*, %"struct.std::__1::__less"** %42, align 8
  %510 = load i64*, i64** %43, align 8
  %511 = load i64, i64* %510, align 8
  %512 = load i64*, i64** %44, align 8
  %513 = load i64, i64* %512, align 8
  %514 = icmp ult i64 %511, %513
  br i1 %514, label %515, label %517

; <label>:515:                                    ; preds = %480
  %516 = load i64*, i64** %47, align 8
  br label %519

; <label>:517:                                    ; preds = %480
  %518 = load i64*, i64** %46, align 8
  br label %519

; <label>:519:                                    ; preds = %517, %515
  %520 = phi i64* [ %516, %515 ], [ %518, %517 ]
  %521 = load i64, i64* %520, align 8
  invoke void @_ZNSt3__112__hash_tableINS_17__hash_value_typeIjPN4llvm10StructTypeEEENS_22__unordered_map_hasherIjS5_NS_4hashIjEELb1EEENS_21__unordered_map_equalIjS5_NS_8equal_toIjEELb1EEENS_9allocatorIS5_EEE6rehashEm(%"class.std::__1::__hash_table.11"* %287, i64 %521)
          to label %522 unwind label %564

; <label>:522:                                    ; preds = %519
  store %"class.std::__1::__hash_table.11"* %287, %"class.std::__1::__hash_table.11"** %59, align 8
  %523 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %59, align 8
  %524 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %523, i32 0, i32 0
  store %"class.std::__1::unique_ptr.12"* %524, %"class.std::__1::unique_ptr.12"** %58, align 8
  %525 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %58, align 8
  %526 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %525, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %526, %"class.std::__1::__compressed_pair.13"** %57, align 8
  %527 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %57, align 8
  %528 = bitcast %"class.std::__1::__compressed_pair.13"* %527 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %528, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %56, align 8
  %529 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %56, align 8
  %530 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %529, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator.16"* %530, %"class.std::__1::__bucket_list_deallocator.16"** %55, align 8
  %531 = load %"class.std::__1::__bucket_list_deallocator.16"*, %"class.std::__1::__bucket_list_deallocator.16"** %55, align 8
  %532 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.16", %"class.std::__1::__bucket_list_deallocator.16"* %531, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.17"* %532, %"class.std::__1::__compressed_pair.17"** %54, align 8
  %533 = load %"class.std::__1::__compressed_pair.17"*, %"class.std::__1::__compressed_pair.17"** %54, align 8
  %534 = bitcast %"class.std::__1::__compressed_pair.17"* %533 to %"class.std::__1::__libcpp_compressed_pair_imp.18"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.18"* %534, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %53, align 8
  %535 = load %"class.std::__1::__libcpp_compressed_pair_imp.18"*, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %53, align 8
  %536 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.18", %"class.std::__1::__libcpp_compressed_pair_imp.18"* %535, i32 0, i32 0
  %537 = load i64, i64* %536, align 8
  store i64 %537, i64* %207, align 8
  %538 = load i64, i64* %206, align 8
  %539 = load i64, i64* %207, align 8
  store i64 %538, i64* %60, align 8
  store i64 %539, i64* %61, align 8
  %540 = load i64, i64* %61, align 8
  %541 = load i64, i64* %61, align 8
  %542 = sub i64 %541, 1
  %543 = and i64 %540, %542
  %544 = icmp ne i64 %543, 0
  br i1 %544, label %550, label %545

; <label>:545:                                    ; preds = %522
  %546 = load i64, i64* %60, align 8
  %547 = load i64, i64* %61, align 8
  %548 = sub i64 %547, 1
  %549 = and i64 %546, %548
  br label %562

; <label>:550:                                    ; preds = %522
  %551 = load i64, i64* %60, align 8
  %552 = load i64, i64* %61, align 8
  %553 = icmp ult i64 %551, %552
  br i1 %553, label %554, label %556

; <label>:554:                                    ; preds = %550
  %555 = load i64, i64* %60, align 8
  br label %560

; <label>:556:                                    ; preds = %550
  %557 = load i64, i64* %60, align 8
  %558 = load i64, i64* %61, align 8
  %559 = urem i64 %557, %558
  br label %560

; <label>:560:                                    ; preds = %556, %554
  %561 = phi i64 [ %555, %554 ], [ %559, %556 ]
  br label %562

; <label>:562:                                    ; preds = %560, %545
  %563 = phi i64 [ %549, %545 ], [ %561, %560 ]
  store i64 %563, i64* %210, align 8
  br label %629

; <label>:564:                                    ; preds = %519
  %565 = landingpad { i8*, i32 }
          cleanup
  %566 = extractvalue { i8*, i32 } %565, 0
  store i8* %566, i8** %213, align 8
  %567 = extractvalue { i8*, i32 } %565, 1
  store i32 %567, i32* %214, align 4
  store %"class.std::__1::unique_ptr.144"* %211, %"class.std::__1::unique_ptr.144"** %96, align 8
  %568 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %96, align 8
  store %"class.std::__1::unique_ptr.144"* %568, %"class.std::__1::unique_ptr.144"** %95, align 8
  %569 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %95, align 8
  store %"class.std::__1::unique_ptr.144"* %569, %"class.std::__1::unique_ptr.144"** %92, align 8
  store %"struct.std::__1::__hash_node.123"* null, %"struct.std::__1::__hash_node.123"** %93, align 8
  %570 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %92, align 8
  %571 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %570, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %571, %"class.std::__1::__compressed_pair.145"** %91, align 8
  %572 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %91, align 8
  %573 = bitcast %"class.std::__1::__compressed_pair.145"* %572 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %573, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %90, align 8
  %574 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %90, align 8
  %575 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %574, i32 0, i32 0
  %576 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %575, align 8
  store %"struct.std::__1::__hash_node.123"* %576, %"struct.std::__1::__hash_node.123"** %94, align 8
  %577 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %93, align 8
  %578 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %570, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %578, %"class.std::__1::__compressed_pair.145"** %72, align 8
  %579 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %72, align 8
  %580 = bitcast %"class.std::__1::__compressed_pair.145"* %579 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %580, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %71, align 8
  %581 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %71, align 8
  %582 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %581, i32 0, i32 0
  store %"struct.std::__1::__hash_node.123"* %577, %"struct.std::__1::__hash_node.123"** %582, align 8
  %583 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %94, align 8
  %584 = icmp ne %"struct.std::__1::__hash_node.123"* %583, null
  br i1 %584, label %585, label %624

; <label>:585:                                    ; preds = %564
  %586 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %570, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %586, %"class.std::__1::__compressed_pair.145"** %70, align 8
  %587 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %70, align 8
  %588 = bitcast %"class.std::__1::__compressed_pair.145"* %587 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %588, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %69, align 8
  %589 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %69, align 8
  %590 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %589, i32 0, i32 1
  %591 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %94, align 8
  store %"class.std::__1::__hash_node_destructor.147"* %590, %"class.std::__1::__hash_node_destructor.147"** %88, align 8
  store %"struct.std::__1::__hash_node.123"* %591, %"struct.std::__1::__hash_node.123"** %89, align 8
  %592 = load %"class.std::__1::__hash_node_destructor.147"*, %"class.std::__1::__hash_node_destructor.147"** %88, align 8
  %593 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %592, i32 0, i32 1
  %594 = load i8, i8* %593, align 8
  %595 = trunc i8 %594 to i1
  br i1 %595, label %596, label %609

; <label>:596:                                    ; preds = %585
  %597 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %592, i32 0, i32 0
  %598 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %597, align 8
  %599 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %89, align 8
  %600 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %599, i32 0, i32 2
  store %"union.std::__1::__hash_value_type.124"* %600, %"union.std::__1::__hash_value_type.124"** %87, align 8
  %601 = load %"union.std::__1::__hash_value_type.124"*, %"union.std::__1::__hash_value_type.124"** %87, align 8
  %602 = bitcast %"union.std::__1::__hash_value_type.124"* %601 to %"struct.std::__1::pair.125"*
  store %"struct.std::__1::pair.125"* %602, %"struct.std::__1::pair.125"** %86, align 8
  %603 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %86, align 8
  %604 = bitcast %"struct.std::__1::pair.125"* %603 to i8*
  store %"class.std::__1::allocator.23"* %598, %"class.std::__1::allocator.23"** %82, align 8
  store %"struct.std::__1::pair.125"* %603, %"struct.std::__1::pair.125"** %83, align 8
  %605 = bitcast %"struct.std::__1::__has_destroy.127"* %85 to %"struct.std::__1::integral_constant"*
  %606 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %82, align 8
  %607 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %83, align 8
  store %"class.std::__1::allocator.23"* %606, %"class.std::__1::allocator.23"** %80, align 8
  store %"struct.std::__1::pair.125"* %607, %"struct.std::__1::pair.125"** %81, align 8
  %608 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %81, align 8
  br label %609

; <label>:609:                                    ; preds = %596, %585
  %610 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %89, align 8
  %611 = icmp ne %"struct.std::__1::__hash_node.123"* %610, null
  br i1 %611, label %612, label %623

; <label>:612:                                    ; preds = %609
  %613 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %592, i32 0, i32 0
  %614 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %613, align 8
  %615 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %89, align 8
  store %"class.std::__1::allocator.23"* %614, %"class.std::__1::allocator.23"** %77, align 8
  store %"struct.std::__1::__hash_node.123"* %615, %"struct.std::__1::__hash_node.123"** %78, align 8
  store i64 1, i64* %79, align 8
  %616 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %77, align 8
  %617 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %78, align 8
  %618 = load i64, i64* %79, align 8
  store %"class.std::__1::allocator.23"* %616, %"class.std::__1::allocator.23"** %74, align 8
  store %"struct.std::__1::__hash_node.123"* %617, %"struct.std::__1::__hash_node.123"** %75, align 8
  store i64 %618, i64* %76, align 8
  %619 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %74, align 8
  %620 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %75, align 8
  %621 = bitcast %"struct.std::__1::__hash_node.123"* %620 to i8*
  store i8* %621, i8** %73, align 8
  %622 = load i8*, i8** %73, align 8
  call void @_ZdlPv(i8* %622) #14
  br label %623

; <label>:623:                                    ; preds = %612, %609
  br label %624

; <label>:624:                                    ; preds = %623, %564
  %625 = load i8*, i8** %213, align 8
  %626 = load i32, i32* %214, align 4
  %627 = insertvalue { i8*, i32 } undef, i8* %625, 0
  %628 = insertvalue { i8*, i32 } %627, i32 %626, 1
  resume { i8*, i32 } %628

; <label>:629:                                    ; preds = %562, %464
  %630 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %287, i32 0, i32 0
  %631 = load i64, i64* %210, align 8
  store %"class.std::__1::unique_ptr.12"* %630, %"class.std::__1::unique_ptr.12"** %99, align 8
  store i64 %631, i64* %100, align 8
  %632 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %99, align 8
  %633 = load i64, i64* %100, align 8
  %634 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %632, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %634, %"class.std::__1::__compressed_pair.13"** %98, align 8
  %635 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %98, align 8
  %636 = bitcast %"class.std::__1::__compressed_pair.13"* %635 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %636, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %97, align 8
  %637 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %97, align 8
  %638 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %637, i32 0, i32 0
  %639 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %638, align 8
  %640 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %639, i64 %633
  %641 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %640, align 8
  store %"struct.std::__1::__hash_node_base.15"* %641, %"struct.std::__1::__hash_node_base.15"** %216, align 8
  %642 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %216, align 8
  %643 = icmp eq %"struct.std::__1::__hash_node_base.15"* %642, null
  br i1 %643, label %644, label %766

; <label>:644:                                    ; preds = %629
  %645 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %287, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.21"* %645, %"class.std::__1::__compressed_pair.21"** %102, align 8
  %646 = load %"class.std::__1::__compressed_pair.21"*, %"class.std::__1::__compressed_pair.21"** %102, align 8
  %647 = bitcast %"class.std::__1::__compressed_pair.21"* %646 to %"class.std::__1::__libcpp_compressed_pair_imp.22"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.22"* %647, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %101, align 8
  %648 = load %"class.std::__1::__libcpp_compressed_pair_imp.22"*, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %101, align 8
  %649 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.22", %"class.std::__1::__libcpp_compressed_pair_imp.22"* %648, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"* %649, %"struct.std::__1::__hash_node_base.15"** %105, align 8
  %650 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %105, align 8
  store %"struct.std::__1::__hash_node_base.15"* %650, %"struct.std::__1::__hash_node_base.15"** %104, align 8
  %651 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %104, align 8
  store %"struct.std::__1::__hash_node_base.15"* %651, %"struct.std::__1::__hash_node_base.15"** %103, align 8
  %652 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %103, align 8
  %653 = bitcast %"struct.std::__1::__hash_node_base.15"* %652 to i8*
  store %"struct.std::__1::__hash_node_base.15"* %652, %"struct.std::__1::__hash_node_base.15"** %216, align 8
  %654 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %216, align 8
  %655 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %654, i32 0, i32 0
  %656 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %655, align 8
  store %"class.std::__1::unique_ptr.144"* %211, %"class.std::__1::unique_ptr.144"** %108, align 8
  %657 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %108, align 8
  %658 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %657, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %658, %"class.std::__1::__compressed_pair.145"** %107, align 8
  %659 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %107, align 8
  %660 = bitcast %"class.std::__1::__compressed_pair.145"* %659 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %660, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %106, align 8
  %661 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %106, align 8
  %662 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %661, i32 0, i32 0
  %663 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %662, align 8
  %664 = bitcast %"struct.std::__1::__hash_node.123"* %663 to %"struct.std::__1::__hash_node_base.15"*
  %665 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %664, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"* %656, %"struct.std::__1::__hash_node_base.15"** %665, align 8
  store %"class.std::__1::unique_ptr.144"* %211, %"class.std::__1::unique_ptr.144"** %111, align 8
  %666 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %111, align 8
  %667 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %666, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %667, %"class.std::__1::__compressed_pair.145"** %110, align 8
  %668 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %110, align 8
  %669 = bitcast %"class.std::__1::__compressed_pair.145"* %668 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %669, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %109, align 8
  %670 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %109, align 8
  %671 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %670, i32 0, i32 0
  %672 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %671, align 8
  %673 = bitcast %"struct.std::__1::__hash_node.123"* %672 to %"struct.std::__1::__hash_node_base.15"*
  store %"struct.std::__1::__hash_node_base.15"* %673, %"struct.std::__1::__hash_node_base.15"** %114, align 8
  %674 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %114, align 8
  store %"struct.std::__1::__hash_node_base.15"* %674, %"struct.std::__1::__hash_node_base.15"** %113, align 8
  %675 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %113, align 8
  store %"struct.std::__1::__hash_node_base.15"* %675, %"struct.std::__1::__hash_node_base.15"** %112, align 8
  %676 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %112, align 8
  %677 = bitcast %"struct.std::__1::__hash_node_base.15"* %676 to i8*
  %678 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %216, align 8
  %679 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %678, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"* %676, %"struct.std::__1::__hash_node_base.15"** %679, align 8
  %680 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %216, align 8
  %681 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %287, i32 0, i32 0
  %682 = load i64, i64* %210, align 8
  store %"class.std::__1::unique_ptr.12"* %681, %"class.std::__1::unique_ptr.12"** %117, align 8
  store i64 %682, i64* %118, align 8
  %683 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %117, align 8
  %684 = load i64, i64* %118, align 8
  %685 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %683, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %685, %"class.std::__1::__compressed_pair.13"** %116, align 8
  %686 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %116, align 8
  %687 = bitcast %"class.std::__1::__compressed_pair.13"* %686 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %687, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %115, align 8
  %688 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %115, align 8
  %689 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %688, i32 0, i32 0
  %690 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %689, align 8
  %691 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %690, i64 %684
  store %"struct.std::__1::__hash_node_base.15"* %680, %"struct.std::__1::__hash_node_base.15"** %691, align 8
  store %"class.std::__1::unique_ptr.144"* %211, %"class.std::__1::unique_ptr.144"** %121, align 8
  %692 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %121, align 8
  %693 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %692, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %693, %"class.std::__1::__compressed_pair.145"** %120, align 8
  %694 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %120, align 8
  %695 = bitcast %"class.std::__1::__compressed_pair.145"* %694 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %695, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %119, align 8
  %696 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %119, align 8
  %697 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %696, i32 0, i32 0
  %698 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %697, align 8
  %699 = bitcast %"struct.std::__1::__hash_node.123"* %698 to %"struct.std::__1::__hash_node_base.15"*
  %700 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %699, i32 0, i32 0
  %701 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %700, align 8
  %702 = icmp ne %"struct.std::__1::__hash_node_base.15"* %701, null
  br i1 %702, label %703, label %765

; <label>:703:                                    ; preds = %644
  store %"class.std::__1::unique_ptr.144"* %211, %"class.std::__1::unique_ptr.144"** %124, align 8
  %704 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %124, align 8
  %705 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %704, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %705, %"class.std::__1::__compressed_pair.145"** %123, align 8
  %706 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %123, align 8
  %707 = bitcast %"class.std::__1::__compressed_pair.145"* %706 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %707, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %122, align 8
  %708 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %122, align 8
  %709 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %708, i32 0, i32 0
  %710 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %709, align 8
  %711 = bitcast %"struct.std::__1::__hash_node.123"* %710 to %"struct.std::__1::__hash_node_base.15"*
  store %"struct.std::__1::__hash_node_base.15"* %711, %"struct.std::__1::__hash_node_base.15"** %127, align 8
  %712 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %127, align 8
  store %"struct.std::__1::__hash_node_base.15"* %712, %"struct.std::__1::__hash_node_base.15"** %126, align 8
  %713 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %126, align 8
  store %"struct.std::__1::__hash_node_base.15"* %713, %"struct.std::__1::__hash_node_base.15"** %125, align 8
  %714 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %125, align 8
  %715 = bitcast %"struct.std::__1::__hash_node_base.15"* %714 to i8*
  %716 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %287, i32 0, i32 0
  store %"class.std::__1::unique_ptr.144"* %211, %"class.std::__1::unique_ptr.144"** %130, align 8
  %717 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %130, align 8
  %718 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %717, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %718, %"class.std::__1::__compressed_pair.145"** %129, align 8
  %719 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %129, align 8
  %720 = bitcast %"class.std::__1::__compressed_pair.145"* %719 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %720, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %128, align 8
  %721 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %128, align 8
  %722 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %721, i32 0, i32 0
  %723 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %722, align 8
  %724 = bitcast %"struct.std::__1::__hash_node.123"* %723 to %"struct.std::__1::__hash_node_base.15"*
  %725 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %724, i32 0, i32 0
  %726 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %725, align 8
  store %"struct.std::__1::__hash_node_base.15"* %726, %"struct.std::__1::__hash_node_base.15"** %131, align 8
  %727 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %131, align 8
  %728 = bitcast %"struct.std::__1::__hash_node_base.15"* %727 to %"struct.std::__1::__hash_node.123"*
  %729 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %728, i32 0, i32 1
  %730 = load i64, i64* %729, align 8
  %731 = load i64, i64* %207, align 8
  store i64 %730, i64* %132, align 8
  store i64 %731, i64* %133, align 8
  %732 = load i64, i64* %133, align 8
  %733 = load i64, i64* %133, align 8
  %734 = sub i64 %733, 1
  %735 = and i64 %732, %734
  %736 = icmp ne i64 %735, 0
  br i1 %736, label %742, label %737

; <label>:737:                                    ; preds = %703
  %738 = load i64, i64* %132, align 8
  %739 = load i64, i64* %133, align 8
  %740 = sub i64 %739, 1
  %741 = and i64 %738, %740
  br label %754

; <label>:742:                                    ; preds = %703
  %743 = load i64, i64* %132, align 8
  %744 = load i64, i64* %133, align 8
  %745 = icmp ult i64 %743, %744
  br i1 %745, label %746, label %748

; <label>:746:                                    ; preds = %742
  %747 = load i64, i64* %132, align 8
  br label %752

; <label>:748:                                    ; preds = %742
  %749 = load i64, i64* %132, align 8
  %750 = load i64, i64* %133, align 8
  %751 = urem i64 %749, %750
  br label %752

; <label>:752:                                    ; preds = %748, %746
  %753 = phi i64 [ %747, %746 ], [ %751, %748 ]
  br label %754

; <label>:754:                                    ; preds = %752, %737
  %755 = phi i64 [ %741, %737 ], [ %753, %752 ]
  store %"class.std::__1::unique_ptr.12"* %716, %"class.std::__1::unique_ptr.12"** %136, align 8
  store i64 %755, i64* %137, align 8
  %756 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %136, align 8
  %757 = load i64, i64* %137, align 8
  %758 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %756, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %758, %"class.std::__1::__compressed_pair.13"** %135, align 8
  %759 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %135, align 8
  %760 = bitcast %"class.std::__1::__compressed_pair.13"* %759 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %760, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %134, align 8
  %761 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %134, align 8
  %762 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %761, i32 0, i32 0
  %763 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %762, align 8
  %764 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %763, i64 %757
  store %"struct.std::__1::__hash_node_base.15"* %714, %"struct.std::__1::__hash_node_base.15"** %764, align 8
  br label %765

; <label>:765:                                    ; preds = %754, %644
  br label %789

; <label>:766:                                    ; preds = %629
  %767 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %216, align 8
  %768 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %767, i32 0, i32 0
  %769 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %768, align 8
  store %"class.std::__1::unique_ptr.144"* %211, %"class.std::__1::unique_ptr.144"** %140, align 8
  %770 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %140, align 8
  %771 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %770, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %771, %"class.std::__1::__compressed_pair.145"** %139, align 8
  %772 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %139, align 8
  %773 = bitcast %"class.std::__1::__compressed_pair.145"* %772 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %773, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %138, align 8
  %774 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %138, align 8
  %775 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %774, i32 0, i32 0
  %776 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %775, align 8
  %777 = bitcast %"struct.std::__1::__hash_node.123"* %776 to %"struct.std::__1::__hash_node_base.15"*
  %778 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %777, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"* %769, %"struct.std::__1::__hash_node_base.15"** %778, align 8
  store %"class.std::__1::unique_ptr.144"* %211, %"class.std::__1::unique_ptr.144"** %143, align 8
  %779 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %143, align 8
  %780 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %779, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %780, %"class.std::__1::__compressed_pair.145"** %142, align 8
  %781 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %142, align 8
  %782 = bitcast %"class.std::__1::__compressed_pair.145"* %781 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %782, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %141, align 8
  %783 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %141, align 8
  %784 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %783, i32 0, i32 0
  %785 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %784, align 8
  %786 = bitcast %"struct.std::__1::__hash_node.123"* %785 to %"struct.std::__1::__hash_node_base.15"*
  %787 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %216, align 8
  %788 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %787, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"* %786, %"struct.std::__1::__hash_node_base.15"** %788, align 8
  br label %789

; <label>:789:                                    ; preds = %766, %765
  store %"class.std::__1::unique_ptr.144"* %211, %"class.std::__1::unique_ptr.144"** %148, align 8
  %790 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %148, align 8
  %791 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %790, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %791, %"class.std::__1::__compressed_pair.145"** %147, align 8
  %792 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %147, align 8
  %793 = bitcast %"class.std::__1::__compressed_pair.145"* %792 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %793, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %146, align 8
  %794 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %146, align 8
  %795 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %794, i32 0, i32 0
  %796 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %795, align 8
  store %"struct.std::__1::__hash_node.123"* %796, %"struct.std::__1::__hash_node.123"** %149, align 8
  %797 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %790, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %797, %"class.std::__1::__compressed_pair.145"** %145, align 8
  %798 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %145, align 8
  %799 = bitcast %"class.std::__1::__compressed_pair.145"* %798 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %799, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %144, align 8
  %800 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %144, align 8
  %801 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %800, i32 0, i32 0
  store %"struct.std::__1::__hash_node.123"* null, %"struct.std::__1::__hash_node.123"** %801, align 8
  %802 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %149, align 8
  %803 = bitcast %"struct.std::__1::__hash_node.123"* %802 to %"struct.std::__1::__hash_node_base.15"*
  store %"struct.std::__1::__hash_node_base.15"* %803, %"struct.std::__1::__hash_node_base.15"** %209, align 8
  store %"class.std::__1::__hash_table.11"* %287, %"class.std::__1::__hash_table.11"** %152, align 8
  %804 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %152, align 8
  %805 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %804, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.25"* %805, %"class.std::__1::__compressed_pair.25"** %151, align 8
  %806 = load %"class.std::__1::__compressed_pair.25"*, %"class.std::__1::__compressed_pair.25"** %151, align 8
  %807 = bitcast %"class.std::__1::__compressed_pair.25"* %806 to %"class.std::__1::__libcpp_compressed_pair_imp.26"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.26"* %807, %"class.std::__1::__libcpp_compressed_pair_imp.26"** %150, align 8
  %808 = load %"class.std::__1::__libcpp_compressed_pair_imp.26"*, %"class.std::__1::__libcpp_compressed_pair_imp.26"** %150, align 8
  %809 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.26", %"class.std::__1::__libcpp_compressed_pair_imp.26"* %808, i32 0, i32 0
  %810 = load i64, i64* %809, align 8
  %811 = add i64 %810, 1
  store i64 %811, i64* %809, align 8
  store i8 1, i8* %208, align 1
  store %"class.std::__1::unique_ptr.144"* %211, %"class.std::__1::unique_ptr.144"** %184, align 8
  %812 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %184, align 8
  store %"class.std::__1::unique_ptr.144"* %812, %"class.std::__1::unique_ptr.144"** %183, align 8
  %813 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %183, align 8
  store %"class.std::__1::unique_ptr.144"* %813, %"class.std::__1::unique_ptr.144"** %180, align 8
  store %"struct.std::__1::__hash_node.123"* null, %"struct.std::__1::__hash_node.123"** %181, align 8
  %814 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %180, align 8
  %815 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %814, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %815, %"class.std::__1::__compressed_pair.145"** %179, align 8
  %816 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %179, align 8
  %817 = bitcast %"class.std::__1::__compressed_pair.145"* %816 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %817, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %178, align 8
  %818 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %178, align 8
  %819 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %818, i32 0, i32 0
  %820 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %819, align 8
  store %"struct.std::__1::__hash_node.123"* %820, %"struct.std::__1::__hash_node.123"** %182, align 8
  %821 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %181, align 8
  %822 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %814, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %822, %"class.std::__1::__compressed_pair.145"** %160, align 8
  %823 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %160, align 8
  %824 = bitcast %"class.std::__1::__compressed_pair.145"* %823 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %824, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %159, align 8
  %825 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %159, align 8
  %826 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %825, i32 0, i32 0
  store %"struct.std::__1::__hash_node.123"* %821, %"struct.std::__1::__hash_node.123"** %826, align 8
  %827 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %182, align 8
  %828 = icmp ne %"struct.std::__1::__hash_node.123"* %827, null
  br i1 %828, label %829, label %868

; <label>:829:                                    ; preds = %789
  %830 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %814, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %830, %"class.std::__1::__compressed_pair.145"** %158, align 8
  %831 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %158, align 8
  %832 = bitcast %"class.std::__1::__compressed_pair.145"* %831 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %832, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %157, align 8
  %833 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %157, align 8
  %834 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %833, i32 0, i32 1
  %835 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %182, align 8
  store %"class.std::__1::__hash_node_destructor.147"* %834, %"class.std::__1::__hash_node_destructor.147"** %176, align 8
  store %"struct.std::__1::__hash_node.123"* %835, %"struct.std::__1::__hash_node.123"** %177, align 8
  %836 = load %"class.std::__1::__hash_node_destructor.147"*, %"class.std::__1::__hash_node_destructor.147"** %176, align 8
  %837 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %836, i32 0, i32 1
  %838 = load i8, i8* %837, align 8
  %839 = trunc i8 %838 to i1
  br i1 %839, label %840, label %853

; <label>:840:                                    ; preds = %829
  %841 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %836, i32 0, i32 0
  %842 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %841, align 8
  %843 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %177, align 8
  %844 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %843, i32 0, i32 2
  store %"union.std::__1::__hash_value_type.124"* %844, %"union.std::__1::__hash_value_type.124"** %175, align 8
  %845 = load %"union.std::__1::__hash_value_type.124"*, %"union.std::__1::__hash_value_type.124"** %175, align 8
  %846 = bitcast %"union.std::__1::__hash_value_type.124"* %845 to %"struct.std::__1::pair.125"*
  store %"struct.std::__1::pair.125"* %846, %"struct.std::__1::pair.125"** %174, align 8
  %847 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %174, align 8
  %848 = bitcast %"struct.std::__1::pair.125"* %847 to i8*
  store %"class.std::__1::allocator.23"* %842, %"class.std::__1::allocator.23"** %170, align 8
  store %"struct.std::__1::pair.125"* %847, %"struct.std::__1::pair.125"** %171, align 8
  %849 = bitcast %"struct.std::__1::__has_destroy.127"* %173 to %"struct.std::__1::integral_constant"*
  %850 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %170, align 8
  %851 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %171, align 8
  store %"class.std::__1::allocator.23"* %850, %"class.std::__1::allocator.23"** %168, align 8
  store %"struct.std::__1::pair.125"* %851, %"struct.std::__1::pair.125"** %169, align 8
  %852 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %169, align 8
  br label %853

; <label>:853:                                    ; preds = %840, %829
  %854 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %177, align 8
  %855 = icmp ne %"struct.std::__1::__hash_node.123"* %854, null
  br i1 %855, label %856, label %867

; <label>:856:                                    ; preds = %853
  %857 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %836, i32 0, i32 0
  %858 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %857, align 8
  %859 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %177, align 8
  store %"class.std::__1::allocator.23"* %858, %"class.std::__1::allocator.23"** %165, align 8
  store %"struct.std::__1::__hash_node.123"* %859, %"struct.std::__1::__hash_node.123"** %166, align 8
  store i64 1, i64* %167, align 8
  %860 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %165, align 8
  %861 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %166, align 8
  %862 = load i64, i64* %167, align 8
  store %"class.std::__1::allocator.23"* %860, %"class.std::__1::allocator.23"** %162, align 8
  store %"struct.std::__1::__hash_node.123"* %861, %"struct.std::__1::__hash_node.123"** %163, align 8
  store i64 %862, i64* %164, align 8
  %863 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %162, align 8
  %864 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %163, align 8
  %865 = bitcast %"struct.std::__1::__hash_node.123"* %864 to i8*
  store i8* %865, i8** %161, align 8
  %866 = load i8*, i8** %161, align 8
  call void @_ZdlPv(i8* %866) #14
  br label %867

; <label>:867:                                    ; preds = %856, %853
  br label %868

; <label>:868:                                    ; preds = %867, %789
  br label %869

; <label>:869:                                    ; preds = %429, %868
  %870 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %209, align 8
  store %"class.std::__1::__hash_iterator.139"* %217, %"class.std::__1::__hash_iterator.139"** %187, align 8
  store %"struct.std::__1::__hash_node_base.15"* %870, %"struct.std::__1::__hash_node_base.15"** %188, align 8
  %871 = load %"class.std::__1::__hash_iterator.139"*, %"class.std::__1::__hash_iterator.139"** %187, align 8
  %872 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %188, align 8
  store %"class.std::__1::__hash_iterator.139"* %871, %"class.std::__1::__hash_iterator.139"** %185, align 8
  store %"struct.std::__1::__hash_node_base.15"* %872, %"struct.std::__1::__hash_node_base.15"** %186, align 8
  %873 = load %"class.std::__1::__hash_iterator.139"*, %"class.std::__1::__hash_iterator.139"** %185, align 8
  %874 = getelementptr inbounds %"class.std::__1::__hash_iterator.139", %"class.std::__1::__hash_iterator.139"* %873, i32 0, i32 0
  %875 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %186, align 8
  store %"struct.std::__1::__hash_node_base.15"* %875, %"struct.std::__1::__hash_node_base.15"** %874, align 8
  store %"struct.std::__1::pair.138"* %200, %"struct.std::__1::pair.138"** %194, align 8
  store %"class.std::__1::__hash_iterator.139"* %217, %"class.std::__1::__hash_iterator.139"** %195, align 8
  store i8* %208, i8** %196, align 8
  %876 = load %"struct.std::__1::pair.138"*, %"struct.std::__1::pair.138"** %194, align 8
  %877 = load %"class.std::__1::__hash_iterator.139"*, %"class.std::__1::__hash_iterator.139"** %195, align 8
  %878 = load i8*, i8** %196, align 8
  store %"struct.std::__1::pair.138"* %876, %"struct.std::__1::pair.138"** %191, align 8
  store %"class.std::__1::__hash_iterator.139"* %877, %"class.std::__1::__hash_iterator.139"** %192, align 8
  store i8* %878, i8** %193, align 8
  %879 = load %"struct.std::__1::pair.138"*, %"struct.std::__1::pair.138"** %191, align 8
  %880 = getelementptr inbounds %"struct.std::__1::pair.138", %"struct.std::__1::pair.138"* %879, i32 0, i32 0
  %881 = load %"class.std::__1::__hash_iterator.139"*, %"class.std::__1::__hash_iterator.139"** %192, align 8
  store %"class.std::__1::__hash_iterator.139"* %881, %"class.std::__1::__hash_iterator.139"** %190, align 8
  %882 = load %"class.std::__1::__hash_iterator.139"*, %"class.std::__1::__hash_iterator.139"** %190, align 8
  %883 = bitcast %"class.std::__1::__hash_iterator.139"* %880 to i8*
  %884 = bitcast %"class.std::__1::__hash_iterator.139"* %882 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %883, i8* %884, i64 8, i32 8, i1 false) #2
  %885 = getelementptr inbounds %"struct.std::__1::pair.138", %"struct.std::__1::pair.138"* %879, i32 0, i32 1
  %886 = load i8*, i8** %193, align 8
  store i8* %886, i8** %189, align 8
  %887 = load i8*, i8** %189, align 8
  %888 = load i8, i8* %887, align 1
  %889 = trunc i8 %888 to i1
  %890 = zext i1 %889 to i8
  store i8 %890, i8* %885, align 8
  %891 = bitcast %"struct.std::__1::pair.138"* %200 to { %"struct.std::__1::__hash_node_base.15"*, i8 }*
  %892 = load { %"struct.std::__1::__hash_node_base.15"*, i8 }, { %"struct.std::__1::__hash_node_base.15"*, i8 }* %891, align 8
  %893 = bitcast %"struct.std::__1::pair.138"* %256 to { %"struct.std::__1::__hash_node_base.15"*, i8 }*
  %894 = getelementptr inbounds { %"struct.std::__1::__hash_node_base.15"*, i8 }, { %"struct.std::__1::__hash_node_base.15"*, i8 }* %893, i32 0, i32 0
  %895 = extractvalue { %"struct.std::__1::__hash_node_base.15"*, i8 } %892, 0
  store %"struct.std::__1::__hash_node_base.15"* %895, %"struct.std::__1::__hash_node_base.15"** %894, align 8
  %896 = getelementptr inbounds { %"struct.std::__1::__hash_node_base.15"*, i8 }, { %"struct.std::__1::__hash_node_base.15"*, i8 }* %893, i32 0, i32 1
  %897 = extractvalue { %"struct.std::__1::__hash_node_base.15"*, i8 } %892, 1
  store i8 %897, i8* %896, align 8
  %898 = getelementptr inbounds %"struct.std::__1::pair.138", %"struct.std::__1::pair.138"* %256, i32 0, i32 0
  store %"class.std::__1::__hash_iterator.139"* %898, %"class.std::__1::__hash_iterator.139"** %223, align 8
  %899 = load %"class.std::__1::__hash_iterator.139"*, %"class.std::__1::__hash_iterator.139"** %223, align 8
  %900 = getelementptr inbounds %"class.std::__1::__hash_iterator.139", %"class.std::__1::__hash_iterator.139"* %899, i32 0, i32 0
  %901 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %900, align 8
  store %"struct.std::__1::__hash_node_base.15"* %901, %"struct.std::__1::__hash_node_base.15"** %222, align 8
  %902 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %222, align 8
  store %"struct.std::__1::__hash_node_base.15"* %902, %"struct.std::__1::__hash_node_base.15"** %221, align 8
  %903 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %221, align 8
  store %"struct.std::__1::__hash_node_base.15"* %903, %"struct.std::__1::__hash_node_base.15"** %220, align 8
  %904 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %220, align 8
  %905 = bitcast %"struct.std::__1::__hash_node_base.15"* %904 to i8*
  %906 = bitcast %"struct.std::__1::__hash_node_base.15"* %904 to %"struct.std::__1::__hash_node.123"*
  %907 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %906, i32 0, i32 2
  store %"union.std::__1::__hash_value_type.124"* %907, %"union.std::__1::__hash_value_type.124"** %219, align 8
  %908 = load %"union.std::__1::__hash_value_type.124"*, %"union.std::__1::__hash_value_type.124"** %219, align 8
  store %"union.std::__1::__hash_value_type.124"* %908, %"union.std::__1::__hash_value_type.124"** %218, align 8
  %909 = load %"union.std::__1::__hash_value_type.124"*, %"union.std::__1::__hash_value_type.124"** %218, align 8
  %910 = bitcast %"union.std::__1::__hash_value_type.124"* %909 to i8*
  %911 = bitcast %"union.std::__1::__hash_value_type.124"* %909 to %"struct.std::__1::pair.125"*
  %912 = getelementptr inbounds %"struct.std::__1::pair.125", %"struct.std::__1::pair.125"* %911, i32 0, i32 1
  ret %"class.llvm::StructType"** %912
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::PointerType"* @_ZN4llvm13IRBuilderBase12getInt8PtrTyEj(%"class.llvm::IRBuilderBase"*, i32) #0 align 2 {
  %3 = alloca %"class.llvm::IRBuilderBase"*, align 8
  %4 = alloca i32, align 4
  store %"class.llvm::IRBuilderBase"* %0, %"class.llvm::IRBuilderBase"** %3, align 8
  store i32 %1, i32* %4, align 4
  %5 = load %"class.llvm::IRBuilderBase"*, %"class.llvm::IRBuilderBase"** %3, align 8
  %6 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %5, i32 0, i32 3
  %7 = load %"class.llvm::LLVMContext"*, %"class.llvm::LLVMContext"** %6, align 8
  %8 = load i32, i32* %4, align 4
  %9 = call %"class.llvm::PointerType"* @_ZN4llvm4Type12getInt8PtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8) %7, i32 %8)
  ret %"class.llvm::PointerType"* %9
}

declare %"class.llvm::PointerType"* @_ZN4llvm4Type12getInt8PtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8), i32) #3

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE7reserveEm(%"class.std::__1::vector.78"*, i64) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::vector.78"*, align 8
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %6 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %7 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %8 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %9 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %10 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %11 = alloca %"class.std::__1::vector.78"*, align 8
  %12 = alloca %"class.std::__1::vector.78"*, align 8
  %13 = alloca i64, align 8
  %14 = alloca %"class.std::__1::allocator.82"*, align 8
  %15 = alloca %"struct.std::__1::__split_buffer", align 8
  %16 = alloca i8*
  %17 = alloca i32
  store %"class.std::__1::vector.78"* %0, %"class.std::__1::vector.78"** %12, align 8
  store i64 %1, i64* %13, align 8
  %18 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %12, align 8
  %19 = load i64, i64* %13, align 8
  store %"class.std::__1::vector.78"* %18, %"class.std::__1::vector.78"** %11, align 8
  %20 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %11, align 8
  %21 = bitcast %"class.std::__1::vector.78"* %20 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %21, %"class.std::__1::__vector_base.79"** %10, align 8
  %22 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %10, align 8
  store %"class.std::__1::__vector_base.79"* %22, %"class.std::__1::__vector_base.79"** %9, align 8
  %23 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %9, align 8
  %24 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %23, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %24, %"class.std::__1::__compressed_pair.80"** %8, align 8
  %25 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %8, align 8
  %26 = bitcast %"class.std::__1::__compressed_pair.80"* %25 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %26, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %7, align 8
  %27 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %7, align 8
  %28 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %27, i32 0, i32 0
  %29 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %28, align 8
  %30 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %22, i32 0, i32 0
  %31 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %30, align 8
  %32 = ptrtoint %"class.llvm::Type"** %29 to i64
  %33 = ptrtoint %"class.llvm::Type"** %31 to i64
  %34 = sub i64 %32, %33
  %35 = sdiv exact i64 %34, 8
  %36 = icmp ugt i64 %19, %35
  br i1 %36, label %37, label %63

; <label>:37:                                     ; preds = %2
  %38 = bitcast %"class.std::__1::vector.78"* %18 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %38, %"class.std::__1::__vector_base.79"** %6, align 8
  %39 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %6, align 8
  %40 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %39, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %40, %"class.std::__1::__compressed_pair.80"** %5, align 8
  %41 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %5, align 8
  %42 = bitcast %"class.std::__1::__compressed_pair.80"* %41 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %42, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %4, align 8
  %43 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %4, align 8
  %44 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.81"* %43 to %"class.std::__1::allocator.82"*
  store %"class.std::__1::allocator.82"* %44, %"class.std::__1::allocator.82"** %14, align 8
  %45 = load i64, i64* %13, align 8
  store %"class.std::__1::vector.78"* %18, %"class.std::__1::vector.78"** %3, align 8
  %46 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %3, align 8
  %47 = bitcast %"class.std::__1::vector.78"* %46 to %"class.std::__1::__vector_base.79"*
  %48 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %47, i32 0, i32 1
  %49 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %48, align 8
  %50 = bitcast %"class.std::__1::vector.78"* %46 to %"class.std::__1::__vector_base.79"*
  %51 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %50, i32 0, i32 0
  %52 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %51, align 8
  %53 = ptrtoint %"class.llvm::Type"** %49 to i64
  %54 = ptrtoint %"class.llvm::Type"** %52 to i64
  %55 = sub i64 %53, %54
  %56 = sdiv exact i64 %55, 8
  %57 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %14, align 8
  call void @_ZNSt3__114__split_bufferIPN4llvm4TypeERNS_9allocatorIS3_EEEC1EmmS6_(%"struct.std::__1::__split_buffer"* %15, i64 %45, i64 %56, %"class.std::__1::allocator.82"* dereferenceable(1) %57)
  invoke void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE26__swap_out_circular_bufferERNS_14__split_bufferIS3_RS5_EE(%"class.std::__1::vector.78"* %18, %"struct.std::__1::__split_buffer"* dereferenceable(40) %15)
          to label %58 unwind label %59

; <label>:58:                                     ; preds = %37
  call void @_ZNSt3__114__split_bufferIPN4llvm4TypeERNS_9allocatorIS3_EEED1Ev(%"struct.std::__1::__split_buffer"* %15) #2
  br label %63

; <label>:59:                                     ; preds = %37
  %60 = landingpad { i8*, i32 }
          cleanup
  %61 = extractvalue { i8*, i32 } %60, 0
  store i8* %61, i8** %16, align 8
  %62 = extractvalue { i8*, i32 } %60, 1
  store i32 %62, i32* %17, align 4
  call void @_ZNSt3__114__split_bufferIPN4llvm4TypeERNS_9allocatorIS3_EEED1Ev(%"struct.std::__1::__split_buffer"* %15) #2
  br label %64

; <label>:63:                                     ; preds = %58, %2
  ret void

; <label>:64:                                     ; preds = %59
  %65 = load i8*, i8** %16, align 8
  %66 = load i32, i32* %17, align 4
  %67 = insertvalue { i8*, i32 } undef, i8* %65, 0
  %68 = insertvalue { i8*, i32 } %67, i32 %66, 1
  resume { i8*, i32 } %68
}

declare i32 @__gxx_personality_v0(...)

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.78"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::vector.78"*, align 8
  store %"class.std::__1::vector.78"* %0, %"class.std::__1::vector.78"** %2, align 8
  %3 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %2, align 8
  call void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEED2Ev(%"class.std::__1::vector.78"* %3) #2
  ret void
}

declare %"class.llvm::IntegerType"* @_ZN4llvm4Type9getInt8TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8)) #3

declare %"class.llvm::IntegerType"* @_ZN4llvm4Type10getInt16TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8)) #3

declare %"class.llvm::IntegerType"* @_ZN4llvm4Type10getInt32TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8)) #3

declare %"class.llvm::IntegerType"* @_ZN4llvm4Type10getInt64TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8)) #3

declare %"class.llvm::Type"* @_ZN4llvm4Type10getFloatTyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8)) #3

declare %"class.llvm::Type"* @_ZN4llvm4Type11getDoubleTyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8)) #3

declare %"class.llvm::IntegerType"* @_ZN4llvm4Type9getInt1TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8)) #3

declare %"class.llvm::Type"* @_ZN4llvm4Type9getVoidTyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8)) #3

; Function Attrs: ssp uwtable
define linkonce_odr dereferenceable(8) %"class.llvm::StructType"** @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeENS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S9_EEEEEixERSF_(%"class.std::__1::unordered_map"*, %"class.std::__1::basic_string"* dereferenceable(24)) #0 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %3 = alloca %"class.std::__1::tuple.131"*, align 8
  %4 = alloca %"class.std::__1::tuple.131"*, align 8
  %5 = alloca %"class.std::__1::tuple.131", align 1
  %6 = alloca i8*, align 8
  %7 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %8 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %9 = alloca %"class.std::__1::basic_string"*, align 8
  %10 = alloca i8*, align 8
  %11 = alloca i8*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %14 = alloca %"class.std::__1::basic_string"*, align 8
  %15 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %16 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %17 = alloca %"class.std::__1::basic_string"*, align 8
  %18 = alloca %"class.std::__1::basic_string"*, align 8
  %19 = alloca %"class.std::__1::basic_string"*, align 8
  %20 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %21 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %22 = alloca %"class.std::__1::basic_string"*, align 8
  %23 = alloca i8*, align 8
  %24 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %25 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %26 = alloca %"class.std::__1::basic_string"*, align 8
  %27 = alloca i8*, align 8
  %28 = alloca i8*, align 8
  %29 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %30 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %31 = alloca %"class.std::__1::basic_string"*, align 8
  %32 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %33 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %34 = alloca %"class.std::__1::basic_string"*, align 8
  %35 = alloca %"class.std::__1::basic_string"*, align 8
  %36 = alloca %"class.std::__1::basic_string"*, align 8
  %37 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %38 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %39 = alloca %"class.std::__1::basic_string"*, align 8
  %40 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %41 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %42 = alloca %"class.std::__1::basic_string"*, align 8
  %43 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %44 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %45 = alloca %"class.std::__1::basic_string"*, align 8
  %46 = alloca %"class.std::__1::basic_string"*, align 8
  %47 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %48 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %49 = alloca %"class.std::__1::basic_string"*, align 8
  %50 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %51 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %52 = alloca %"class.std::__1::basic_string"*, align 8
  %53 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %54 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %55 = alloca %"class.std::__1::basic_string"*, align 8
  %56 = alloca %"class.std::__1::basic_string"*, align 8
  %57 = alloca i1, align 1
  %58 = alloca %"class.std::__1::basic_string"*, align 8
  %59 = alloca %"class.std::__1::basic_string"*, align 8
  %60 = alloca i64, align 8
  %61 = alloca i8*, align 8
  %62 = alloca i8*, align 8
  %63 = alloca %"struct.std::__1::equal_to"*, align 8
  %64 = alloca %"class.std::__1::basic_string"*, align 8
  %65 = alloca %"class.std::__1::basic_string"*, align 8
  %66 = alloca %"class.std::__1::__unordered_map_equal"*, align 8
  %67 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %68 = alloca %"class.std::__1::basic_string"*, align 8
  %69 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %70 = alloca %"class.std::__1::tuple"*, align 8
  %71 = alloca %"class.std::__1::tuple.131"*, align 8
  %72 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %73 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %74 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %75 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.7"*, align 8
  %76 = alloca %"class.std::__1::__compressed_pair.6"*, align 8
  %77 = alloca %"class.std::__1::__hash_table"*, align 8
  %78 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.9"*, align 8
  %79 = alloca %"class.std::__1::__compressed_pair.8"*, align 8
  %80 = alloca %"class.std::__1::__hash_table"*, align 8
  %81 = alloca i64, align 8
  %82 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.7"*, align 8
  %83 = alloca %"class.std::__1::__compressed_pair.6"*, align 8
  %84 = alloca %"class.std::__1::__hash_table"*, align 8
  %85 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.9"*, align 8
  %86 = alloca %"class.std::__1::__compressed_pair.8"*, align 8
  %87 = alloca %"class.std::__1::__hash_table"*, align 8
  %88 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.9"*, align 8
  %89 = alloca %"class.std::__1::__compressed_pair.8"*, align 8
  %90 = alloca %"class.std::__1::__hash_table"*, align 8
  %91 = alloca float, align 4
  %92 = alloca %"struct.std::__1::__less"*, align 8
  %93 = alloca i64*, align 8
  %94 = alloca i64*, align 8
  %95 = alloca %"struct.std::__1::__less", align 1
  %96 = alloca i64*, align 8
  %97 = alloca i64*, align 8
  %98 = alloca i64*, align 8
  %99 = alloca i64*, align 8
  %100 = alloca %"struct.std::__1::__less", align 1
  %101 = alloca i64, align 8
  %102 = alloca i64, align 8
  %103 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %104 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %105 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %106 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %107 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %108 = alloca %"class.std::__1::unique_ptr"*, align 8
  %109 = alloca %"class.std::__1::__hash_table"*, align 8
  %110 = alloca i64, align 8
  %111 = alloca i64, align 8
  %112 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %113 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %114 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %115 = alloca %"class.std::__1::unique_ptr"*, align 8
  %116 = alloca i64, align 8
  %117 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %118 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %119 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %120 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %121 = alloca i8*, align 8
  %122 = alloca %"class.std::__1::allocator.4"*, align 8
  %123 = alloca %"struct.std::__1::__hash_node"*, align 8
  %124 = alloca i64, align 8
  %125 = alloca %"class.std::__1::allocator.4"*, align 8
  %126 = alloca %"struct.std::__1::__hash_node"*, align 8
  %127 = alloca i64, align 8
  %128 = alloca %"class.std::__1::allocator.4"*, align 8
  %129 = alloca %"struct.std::__1::pair"*, align 8
  %130 = alloca %"class.std::__1::allocator.4"*, align 8
  %131 = alloca %"struct.std::__1::pair"*, align 8
  %132 = alloca %"struct.std::__1::integral_constant", align 1
  %133 = alloca %"struct.std::__1::__has_destroy", align 1
  %134 = alloca %"struct.std::__1::pair"*, align 8
  %135 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %136 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %137 = alloca %"struct.std::__1::__hash_node"*, align 8
  %138 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %139 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %140 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %141 = alloca %"struct.std::__1::__hash_node"*, align 8
  %142 = alloca %"struct.std::__1::__hash_node"*, align 8
  %143 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %144 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %145 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %146 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %147 = alloca %"class.std::__1::unique_ptr"*, align 8
  %148 = alloca i64, align 8
  %149 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %150 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %151 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %152 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %153 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %154 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %155 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %156 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %157 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %158 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %159 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %160 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %161 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %162 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %163 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %164 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %165 = alloca %"class.std::__1::unique_ptr"*, align 8
  %166 = alloca i64, align 8
  %167 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %168 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %169 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %170 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %171 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %172 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %173 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %174 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %175 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %176 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %177 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %178 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %179 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %180 = alloca i64, align 8
  %181 = alloca i64, align 8
  %182 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %183 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %184 = alloca %"class.std::__1::unique_ptr"*, align 8
  %185 = alloca i64, align 8
  %186 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %187 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %188 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %189 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %190 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %191 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %192 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %193 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %194 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %195 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %196 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %197 = alloca %"struct.std::__1::__hash_node"*, align 8
  %198 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.7"*, align 8
  %199 = alloca %"class.std::__1::__compressed_pair.6"*, align 8
  %200 = alloca %"class.std::__1::__hash_table"*, align 8
  %201 = alloca i64, align 8
  %202 = alloca i64, align 8
  %203 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %204 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %205 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %206 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %207 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %208 = alloca %"class.std::__1::unique_ptr"*, align 8
  %209 = alloca %"class.std::__1::__hash_table"*, align 8
  %210 = alloca %"class.std::__1::__unordered_map_hasher"*, align 8
  %211 = alloca %"class.std::__1::basic_string"*, align 8
  %212 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %213 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %214 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %215 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %216 = alloca i8*, align 8
  %217 = alloca %"class.std::__1::allocator.4"*, align 8
  %218 = alloca %"struct.std::__1::__hash_node"*, align 8
  %219 = alloca i64, align 8
  %220 = alloca %"class.std::__1::allocator.4"*, align 8
  %221 = alloca %"struct.std::__1::__hash_node"*, align 8
  %222 = alloca i64, align 8
  %223 = alloca %"class.std::__1::allocator.4"*, align 8
  %224 = alloca %"struct.std::__1::pair"*, align 8
  %225 = alloca %"class.std::__1::allocator.4"*, align 8
  %226 = alloca %"struct.std::__1::pair"*, align 8
  %227 = alloca %"struct.std::__1::integral_constant", align 1
  %228 = alloca %"struct.std::__1::__has_destroy", align 1
  %229 = alloca %"struct.std::__1::pair"*, align 8
  %230 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %231 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %232 = alloca %"struct.std::__1::__hash_node"*, align 8
  %233 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %234 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %235 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %236 = alloca %"struct.std::__1::__hash_node"*, align 8
  %237 = alloca %"struct.std::__1::__hash_node"*, align 8
  %238 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %239 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %240 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %241 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %242 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %243 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %244 = alloca i8*, align 8
  %245 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %246 = alloca %"struct.std::__1::pair.130"*, align 8
  %247 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %248 = alloca i8*, align 8
  %249 = alloca %"struct.std::__1::pair.130"*, align 8
  %250 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %251 = alloca i8*, align 8
  %252 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.7"*, align 8
  %253 = alloca %"class.std::__1::__compressed_pair.6"*, align 8
  %254 = alloca %"class.std::__1::__hash_table"*, align 8
  %255 = alloca %"struct.std::__1::pair.130", align 8
  %256 = alloca %"class.std::__1::__hash_table"*, align 8
  %257 = alloca %"class.std::__1::basic_string"*, align 8
  %258 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %259 = alloca %"class.std::__1::tuple"*, align 8
  %260 = alloca %"class.std::__1::tuple.131"*, align 8
  %261 = alloca i64, align 8
  %262 = alloca i64, align 8
  %263 = alloca i8, align 1
  %264 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %265 = alloca i64, align 8
  %266 = alloca %"class.std::__1::unique_ptr.132", align 8
  %267 = alloca i64, align 8
  %268 = alloca i8*
  %269 = alloca i32
  %270 = alloca i64, align 8
  %271 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %272 = alloca %"class.std::__1::__hash_iterator", align 8
  %273 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %274 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %275 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %276 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %277 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %278 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %279 = alloca %"class.std::__1::basic_string"*, align 8
  %280 = alloca %"class.std::__1::__tuple_leaf"*, align 8
  %281 = alloca %"class.std::__1::basic_string"*, align 8
  %282 = alloca %"class.std::__1::basic_string"*, align 8
  %283 = alloca %"struct.std::__1::__tuple_indices", align 1
  %284 = alloca %"struct.std::__1::__tuple_types", align 1
  %285 = alloca %"struct.std::__1::__tuple_indices.136", align 1
  %286 = alloca %"struct.std::__1::__tuple_types.137", align 1
  %287 = alloca %"struct.std::__1::__tuple_impl"*, align 8
  %288 = alloca %"class.std::__1::basic_string"*, align 8
  %289 = alloca %"struct.std::__1::__tuple_indices", align 1
  %290 = alloca %"struct.std::__1::__tuple_types", align 1
  %291 = alloca %"struct.std::__1::__tuple_indices.136", align 1
  %292 = alloca %"struct.std::__1::__tuple_types.137", align 1
  %293 = alloca %"struct.std::__1::__tuple_impl"*, align 8
  %294 = alloca %"class.std::__1::basic_string"*, align 8
  %295 = alloca %"class.std::__1::tuple"*, align 8
  %296 = alloca %"class.std::__1::basic_string"*, align 8
  %297 = alloca %"struct.std::__1::__tuple_indices", align 1
  %298 = alloca %"struct.std::__1::__tuple_types", align 1
  %299 = alloca %"struct.std::__1::__tuple_indices.136", align 1
  %300 = alloca %"struct.std::__1::__tuple_types.137", align 1
  %301 = alloca %"class.std::__1::tuple"*, align 8
  %302 = alloca %"class.std::__1::basic_string"*, align 8
  %303 = alloca %"class.std::__1::basic_string"*, align 8
  %304 = alloca %"class.std::__1::tuple", align 8
  %305 = alloca %"class.std::__1::basic_string"*, align 8
  %306 = alloca %"class.std::__1::unordered_map"*, align 8
  %307 = alloca %"class.std::__1::basic_string"*, align 8
  %308 = alloca %"class.std::__1::tuple", align 8
  %309 = alloca %"class.std::__1::tuple.131", align 1
  %310 = alloca %"class.std::__1::tuple.131", align 1
  %311 = alloca %"struct.std::__1::pair.130", align 8
  store %"class.std::__1::unordered_map"* %0, %"class.std::__1::unordered_map"** %306, align 8
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %307, align 8
  %312 = load %"class.std::__1::unordered_map"*, %"class.std::__1::unordered_map"** %306, align 8
  %313 = getelementptr inbounds %"class.std::__1::unordered_map", %"class.std::__1::unordered_map"* %312, i32 0, i32 0
  %314 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %307, align 8
  %315 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %307, align 8
  store %"class.std::__1::basic_string"* %315, %"class.std::__1::basic_string"** %305, align 8
  %316 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %305, align 8
  store %"class.std::__1::basic_string"* %316, %"class.std::__1::basic_string"** %303, align 8
  %317 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %303, align 8
  store %"class.std::__1::tuple"* %304, %"class.std::__1::tuple"** %301, align 8
  store %"class.std::__1::basic_string"* %317, %"class.std::__1::basic_string"** %302, align 8
  %318 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %301, align 8
  %319 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %302, align 8
  store %"class.std::__1::tuple"* %318, %"class.std::__1::tuple"** %295, align 8
  store %"class.std::__1::basic_string"* %319, %"class.std::__1::basic_string"** %296, align 8
  %320 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %295, align 8
  %321 = getelementptr inbounds %"class.std::__1::tuple", %"class.std::__1::tuple"* %320, i32 0, i32 0
  %322 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %296, align 8
  store %"struct.std::__1::__tuple_impl"* %321, %"struct.std::__1::__tuple_impl"** %293, align 8
  store %"class.std::__1::basic_string"* %322, %"class.std::__1::basic_string"** %294, align 8
  %323 = load %"struct.std::__1::__tuple_impl"*, %"struct.std::__1::__tuple_impl"** %293, align 8
  %324 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %294, align 8
  store %"struct.std::__1::__tuple_impl"* %323, %"struct.std::__1::__tuple_impl"** %287, align 8
  store %"class.std::__1::basic_string"* %324, %"class.std::__1::basic_string"** %288, align 8
  %325 = load %"struct.std::__1::__tuple_impl"*, %"struct.std::__1::__tuple_impl"** %287, align 8
  %326 = bitcast %"struct.std::__1::__tuple_impl"* %325 to %"class.std::__1::__tuple_leaf"*
  %327 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %288, align 8
  store %"class.std::__1::basic_string"* %327, %"class.std::__1::basic_string"** %282, align 8
  %328 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %282, align 8
  store %"class.std::__1::__tuple_leaf"* %326, %"class.std::__1::__tuple_leaf"** %280, align 8
  store %"class.std::__1::basic_string"* %328, %"class.std::__1::basic_string"** %281, align 8
  %329 = load %"class.std::__1::__tuple_leaf"*, %"class.std::__1::__tuple_leaf"** %280, align 8
  %330 = getelementptr inbounds %"class.std::__1::__tuple_leaf", %"class.std::__1::__tuple_leaf"* %329, i32 0, i32 0
  %331 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %281, align 8
  store %"class.std::__1::basic_string"* %331, %"class.std::__1::basic_string"** %279, align 8
  %332 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %279, align 8
  store %"class.std::__1::basic_string"* %332, %"class.std::__1::basic_string"** %330, align 8
  %333 = getelementptr inbounds %"class.std::__1::tuple", %"class.std::__1::tuple"* %304, i32 0, i32 0
  %334 = getelementptr inbounds %"struct.std::__1::__tuple_impl", %"struct.std::__1::__tuple_impl"* %333, i32 0, i32 0
  %335 = getelementptr inbounds %"class.std::__1::__tuple_leaf", %"class.std::__1::__tuple_leaf"* %334, i32 0, i32 0
  %336 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %335, align 8
  %337 = getelementptr inbounds %"class.std::__1::tuple", %"class.std::__1::tuple"* %308, i32 0, i32 0
  %338 = getelementptr inbounds %"struct.std::__1::__tuple_impl", %"struct.std::__1::__tuple_impl"* %337, i32 0, i32 0
  %339 = getelementptr inbounds %"class.std::__1::__tuple_leaf", %"class.std::__1::__tuple_leaf"* %338, i32 0, i32 0
  store %"class.std::__1::basic_string"* %336, %"class.std::__1::basic_string"** %339, align 8
  store %"class.std::__1::tuple.131"* %5, %"class.std::__1::tuple.131"** %4, align 8
  %340 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %4, align 8
  store %"class.std::__1::tuple.131"* %340, %"class.std::__1::tuple.131"** %3, align 8
  %341 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %3, align 8
  store %"class.std::__1::__hash_table"* %313, %"class.std::__1::__hash_table"** %256, align 8
  store %"class.std::__1::basic_string"* %314, %"class.std::__1::basic_string"** %257, align 8
  store %"struct.std::__1::piecewise_construct_t"* @_ZNSt3__1L19piecewise_constructE, %"struct.std::__1::piecewise_construct_t"** %258, align 8
  store %"class.std::__1::tuple"* %308, %"class.std::__1::tuple"** %259, align 8
  store %"class.std::__1::tuple.131"* %309, %"class.std::__1::tuple.131"** %260, align 8
  %342 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %256, align 8
  store %"class.std::__1::__hash_table"* %342, %"class.std::__1::__hash_table"** %254, align 8
  %343 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %254, align 8
  %344 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %343, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.6"* %344, %"class.std::__1::__compressed_pair.6"** %253, align 8
  %345 = load %"class.std::__1::__compressed_pair.6"*, %"class.std::__1::__compressed_pair.6"** %253, align 8
  %346 = bitcast %"class.std::__1::__compressed_pair.6"* %345 to %"class.std::__1::__libcpp_compressed_pair_imp.7"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.7"* %346, %"class.std::__1::__libcpp_compressed_pair_imp.7"** %252, align 8
  %347 = load %"class.std::__1::__libcpp_compressed_pair_imp.7"*, %"class.std::__1::__libcpp_compressed_pair_imp.7"** %252, align 8
  %348 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.7"* %347 to %"class.std::__1::__unordered_map_hasher"*
  %349 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %257, align 8
  store %"class.std::__1::__unordered_map_hasher"* %348, %"class.std::__1::__unordered_map_hasher"** %210, align 8
  store %"class.std::__1::basic_string"* %349, %"class.std::__1::basic_string"** %211, align 8
  %350 = load %"class.std::__1::__unordered_map_hasher"*, %"class.std::__1::__unordered_map_hasher"** %210, align 8
  %351 = bitcast %"class.std::__1::__unordered_map_hasher"* %350 to %"struct.std::__1::hash"*
  %352 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %211, align 8
  %353 = call i64 @_ZNKSt3__14hashINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEclERKS6_(%"struct.std::__1::hash"* %351, %"class.std::__1::basic_string"* dereferenceable(24) %352) #2
  store i64 %353, i64* %261, align 8
  store %"class.std::__1::__hash_table"* %342, %"class.std::__1::__hash_table"** %209, align 8
  %354 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %209, align 8
  %355 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %354, i32 0, i32 0
  store %"class.std::__1::unique_ptr"* %355, %"class.std::__1::unique_ptr"** %208, align 8
  %356 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %208, align 8
  %357 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %356, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %357, %"class.std::__1::__compressed_pair"** %207, align 8
  %358 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %207, align 8
  %359 = bitcast %"class.std::__1::__compressed_pair"* %358 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %359, %"class.std::__1::__libcpp_compressed_pair_imp"** %206, align 8
  %360 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %206, align 8
  %361 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %360, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator"* %361, %"class.std::__1::__bucket_list_deallocator"** %205, align 8
  %362 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %205, align 8
  %363 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %362, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.0"* %363, %"class.std::__1::__compressed_pair.0"** %204, align 8
  %364 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %204, align 8
  %365 = bitcast %"class.std::__1::__compressed_pair.0"* %364 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %365, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %203, align 8
  %366 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %203, align 8
  %367 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.1", %"class.std::__1::__libcpp_compressed_pair_imp.1"* %366, i32 0, i32 0
  %368 = load i64, i64* %367, align 8
  store i64 %368, i64* %262, align 8
  store i8 0, i8* %263, align 1
  %369 = load i64, i64* %262, align 8
  %370 = icmp ne i64 %369, 0
  br i1 %370, label %371, label %713

; <label>:371:                                    ; preds = %2
  %372 = load i64, i64* %261, align 8
  %373 = load i64, i64* %262, align 8
  store i64 %372, i64* %201, align 8
  store i64 %373, i64* %202, align 8
  %374 = load i64, i64* %202, align 8
  %375 = load i64, i64* %202, align 8
  %376 = sub i64 %375, 1
  %377 = and i64 %374, %376
  %378 = icmp ne i64 %377, 0
  br i1 %378, label %384, label %379

; <label>:379:                                    ; preds = %371
  %380 = load i64, i64* %201, align 8
  %381 = load i64, i64* %202, align 8
  %382 = sub i64 %381, 1
  %383 = and i64 %380, %382
  br label %396

; <label>:384:                                    ; preds = %371
  %385 = load i64, i64* %201, align 8
  %386 = load i64, i64* %202, align 8
  %387 = icmp ult i64 %385, %386
  br i1 %387, label %388, label %390

; <label>:388:                                    ; preds = %384
  %389 = load i64, i64* %201, align 8
  br label %394

; <label>:390:                                    ; preds = %384
  %391 = load i64, i64* %201, align 8
  %392 = load i64, i64* %202, align 8
  %393 = urem i64 %391, %392
  br label %394

; <label>:394:                                    ; preds = %390, %388
  %395 = phi i64 [ %389, %388 ], [ %393, %390 ]
  br label %396

; <label>:396:                                    ; preds = %394, %379
  %397 = phi i64 [ %383, %379 ], [ %395, %394 ]
  store i64 %397, i64* %265, align 8
  %398 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %342, i32 0, i32 0
  %399 = load i64, i64* %265, align 8
  store %"class.std::__1::unique_ptr"* %398, %"class.std::__1::unique_ptr"** %115, align 8
  store i64 %399, i64* %116, align 8
  %400 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %115, align 8
  %401 = load i64, i64* %116, align 8
  %402 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %400, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %402, %"class.std::__1::__compressed_pair"** %114, align 8
  %403 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %114, align 8
  %404 = bitcast %"class.std::__1::__compressed_pair"* %403 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %404, %"class.std::__1::__libcpp_compressed_pair_imp"** %113, align 8
  %405 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %113, align 8
  %406 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %405, i32 0, i32 0
  %407 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %406, align 8
  %408 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %407, i64 %401
  %409 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %408, align 8
  store %"struct.std::__1::__hash_node_base"* %409, %"struct.std::__1::__hash_node_base"** %264, align 8
  %410 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %264, align 8
  %411 = icmp ne %"struct.std::__1::__hash_node_base"* %410, null
  br i1 %411, label %412, label %712

; <label>:412:                                    ; preds = %396
  %413 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %264, align 8
  %414 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %413, i32 0, i32 0
  %415 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %414, align 8
  store %"struct.std::__1::__hash_node_base"* %415, %"struct.std::__1::__hash_node_base"** %264, align 8
  br label %416

; <label>:416:                                    ; preds = %707, %412
  %417 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %264, align 8
  %418 = icmp ne %"struct.std::__1::__hash_node_base"* %417, null
  br i1 %418, label %419, label %452

; <label>:419:                                    ; preds = %416
  %420 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %264, align 8
  store %"struct.std::__1::__hash_node_base"* %420, %"struct.std::__1::__hash_node_base"** %112, align 8
  %421 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %112, align 8
  %422 = bitcast %"struct.std::__1::__hash_node_base"* %421 to %"struct.std::__1::__hash_node"*
  %423 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %422, i32 0, i32 1
  %424 = load i64, i64* %423, align 8
  %425 = load i64, i64* %262, align 8
  store i64 %424, i64* %101, align 8
  store i64 %425, i64* %102, align 8
  %426 = load i64, i64* %102, align 8
  %427 = load i64, i64* %102, align 8
  %428 = sub i64 %427, 1
  %429 = and i64 %426, %428
  %430 = icmp ne i64 %429, 0
  br i1 %430, label %436, label %431

; <label>:431:                                    ; preds = %419
  %432 = load i64, i64* %101, align 8
  %433 = load i64, i64* %102, align 8
  %434 = sub i64 %433, 1
  %435 = and i64 %432, %434
  br label %448

; <label>:436:                                    ; preds = %419
  %437 = load i64, i64* %101, align 8
  %438 = load i64, i64* %102, align 8
  %439 = icmp ult i64 %437, %438
  br i1 %439, label %440, label %442

; <label>:440:                                    ; preds = %436
  %441 = load i64, i64* %101, align 8
  br label %446

; <label>:442:                                    ; preds = %436
  %443 = load i64, i64* %101, align 8
  %444 = load i64, i64* %102, align 8
  %445 = urem i64 %443, %444
  br label %446

; <label>:446:                                    ; preds = %442, %440
  %447 = phi i64 [ %441, %440 ], [ %445, %442 ]
  br label %448

; <label>:448:                                    ; preds = %446, %431
  %449 = phi i64 [ %435, %431 ], [ %447, %446 ]
  %450 = load i64, i64* %265, align 8
  %451 = icmp eq i64 %449, %450
  br label %452

; <label>:452:                                    ; preds = %448, %416
  %453 = phi i1 [ false, %416 ], [ %451, %448 ]
  br i1 %453, label %454, label %711

; <label>:454:                                    ; preds = %452
  store %"class.std::__1::__hash_table"* %342, %"class.std::__1::__hash_table"** %90, align 8
  %455 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %90, align 8
  %456 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %455, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.8"* %456, %"class.std::__1::__compressed_pair.8"** %89, align 8
  %457 = load %"class.std::__1::__compressed_pair.8"*, %"class.std::__1::__compressed_pair.8"** %89, align 8
  %458 = bitcast %"class.std::__1::__compressed_pair.8"* %457 to %"class.std::__1::__libcpp_compressed_pair_imp.9"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.9"* %458, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %88, align 8
  %459 = load %"class.std::__1::__libcpp_compressed_pair_imp.9"*, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %88, align 8
  %460 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.9"* %459 to %"class.std::__1::__unordered_map_equal"*
  %461 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %264, align 8
  store %"struct.std::__1::__hash_node_base"* %461, %"struct.std::__1::__hash_node_base"** %74, align 8
  %462 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %74, align 8
  store %"struct.std::__1::__hash_node_base"* %462, %"struct.std::__1::__hash_node_base"** %73, align 8
  %463 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %73, align 8
  store %"struct.std::__1::__hash_node_base"* %463, %"struct.std::__1::__hash_node_base"** %72, align 8
  %464 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %72, align 8
  %465 = bitcast %"struct.std::__1::__hash_node_base"* %464 to i8*
  %466 = bitcast %"struct.std::__1::__hash_node_base"* %464 to %"struct.std::__1::__hash_node"*
  %467 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %466, i32 0, i32 2
  %468 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %257, align 8
  store %"class.std::__1::__unordered_map_equal"* %460, %"class.std::__1::__unordered_map_equal"** %66, align 8
  store %"union.std::__1::__hash_value_type"* %467, %"union.std::__1::__hash_value_type"** %67, align 8
  store %"class.std::__1::basic_string"* %468, %"class.std::__1::basic_string"** %68, align 8
  %469 = load %"class.std::__1::__unordered_map_equal"*, %"class.std::__1::__unordered_map_equal"** %66, align 8
  %470 = bitcast %"class.std::__1::__unordered_map_equal"* %469 to %"struct.std::__1::equal_to"*
  %471 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %67, align 8
  %472 = bitcast %"union.std::__1::__hash_value_type"* %471 to %"struct.std::__1::pair"*
  %473 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %472, i32 0, i32 0
  %474 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8
  store %"struct.std::__1::equal_to"* %470, %"struct.std::__1::equal_to"** %63, align 8
  store %"class.std::__1::basic_string"* %473, %"class.std::__1::basic_string"** %64, align 8
  store %"class.std::__1::basic_string"* %474, %"class.std::__1::basic_string"** %65, align 8
  %475 = load %"struct.std::__1::equal_to"*, %"struct.std::__1::equal_to"** %63, align 8
  %476 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8
  %477 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8
  store %"class.std::__1::basic_string"* %476, %"class.std::__1::basic_string"** %58, align 8
  store %"class.std::__1::basic_string"* %477, %"class.std::__1::basic_string"** %59, align 8
  %478 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8
  store %"class.std::__1::basic_string"* %478, %"class.std::__1::basic_string"** %56, align 8
  %479 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %56, align 8
  store %"class.std::__1::basic_string"* %479, %"class.std::__1::basic_string"** %55, align 8
  %480 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %55, align 8
  %481 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %480, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %481, %"class.std::__1::__compressed_pair.36"** %54, align 8
  %482 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %54, align 8
  %483 = bitcast %"class.std::__1::__compressed_pair.36"* %482 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %483, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %53, align 8
  %484 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %53, align 8
  %485 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %484, i32 0, i32 0
  %486 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %485, i32 0, i32 0
  %487 = bitcast %union.anon* %486 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %488 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %487, i32 0, i32 0
  %489 = bitcast %union.anon.40* %488 to i8*
  %490 = load i8, i8* %489, align 8
  %491 = zext i8 %490 to i32
  %492 = and i32 %491, 1
  %493 = icmp ne i32 %492, 0
  br i1 %493, label %494, label %505

; <label>:494:                                    ; preds = %454
  store %"class.std::__1::basic_string"* %479, %"class.std::__1::basic_string"** %49, align 8
  %495 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8
  %496 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %495, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %496, %"class.std::__1::__compressed_pair.36"** %48, align 8
  %497 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %48, align 8
  %498 = bitcast %"class.std::__1::__compressed_pair.36"* %497 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %498, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %47, align 8
  %499 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %47, align 8
  %500 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %499, i32 0, i32 0
  %501 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %500, i32 0, i32 0
  %502 = bitcast %union.anon* %501 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %503 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %502, i32 0, i32 1
  %504 = load i64, i64* %503, align 8
  br label %520

; <label>:505:                                    ; preds = %454
  store %"class.std::__1::basic_string"* %479, %"class.std::__1::basic_string"** %52, align 8
  %506 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8
  %507 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %506, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %507, %"class.std::__1::__compressed_pair.36"** %51, align 8
  %508 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %51, align 8
  %509 = bitcast %"class.std::__1::__compressed_pair.36"* %508 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %509, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %50, align 8
  %510 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %50, align 8
  %511 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %510, i32 0, i32 0
  %512 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %511, i32 0, i32 0
  %513 = bitcast %union.anon* %512 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %514 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %513, i32 0, i32 0
  %515 = bitcast %union.anon.40* %514 to i8*
  %516 = load i8, i8* %515, align 8
  %517 = zext i8 %516 to i32
  %518 = ashr i32 %517, 1
  %519 = sext i32 %518 to i64
  br label %520

; <label>:520:                                    ; preds = %505, %494
  %521 = phi i64 [ %504, %494 ], [ %519, %505 ]
  store i64 %521, i64* %60, align 8
  %522 = load i64, i64* %60, align 8
  %523 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %59, align 8
  store %"class.std::__1::basic_string"* %523, %"class.std::__1::basic_string"** %46, align 8
  %524 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %46, align 8
  store %"class.std::__1::basic_string"* %524, %"class.std::__1::basic_string"** %45, align 8
  %525 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8
  %526 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %525, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %526, %"class.std::__1::__compressed_pair.36"** %44, align 8
  %527 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %44, align 8
  %528 = bitcast %"class.std::__1::__compressed_pair.36"* %527 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %528, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %43, align 8
  %529 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %43, align 8
  %530 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %529, i32 0, i32 0
  %531 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %530, i32 0, i32 0
  %532 = bitcast %union.anon* %531 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %533 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %532, i32 0, i32 0
  %534 = bitcast %union.anon.40* %533 to i8*
  %535 = load i8, i8* %534, align 8
  %536 = zext i8 %535 to i32
  %537 = and i32 %536, 1
  %538 = icmp ne i32 %537, 0
  br i1 %538, label %539, label %550

; <label>:539:                                    ; preds = %520
  store %"class.std::__1::basic_string"* %524, %"class.std::__1::basic_string"** %39, align 8
  %540 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8
  %541 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %540, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %541, %"class.std::__1::__compressed_pair.36"** %38, align 8
  %542 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %38, align 8
  %543 = bitcast %"class.std::__1::__compressed_pair.36"* %542 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %543, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %37, align 8
  %544 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %37, align 8
  %545 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %544, i32 0, i32 0
  %546 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %545, i32 0, i32 0
  %547 = bitcast %union.anon* %546 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %548 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %547, i32 0, i32 1
  %549 = load i64, i64* %548, align 8
  br label %565

; <label>:550:                                    ; preds = %520
  store %"class.std::__1::basic_string"* %524, %"class.std::__1::basic_string"** %42, align 8
  %551 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8
  %552 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %551, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %552, %"class.std::__1::__compressed_pair.36"** %41, align 8
  %553 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %41, align 8
  %554 = bitcast %"class.std::__1::__compressed_pair.36"* %553 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %554, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %40, align 8
  %555 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %40, align 8
  %556 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %555, i32 0, i32 0
  %557 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %556, i32 0, i32 0
  %558 = bitcast %union.anon* %557 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %559 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %558, i32 0, i32 0
  %560 = bitcast %union.anon.40* %559 to i8*
  %561 = load i8, i8* %560, align 8
  %562 = zext i8 %561 to i32
  %563 = ashr i32 %562, 1
  %564 = sext i32 %563 to i64
  br label %565

; <label>:565:                                    ; preds = %550, %539
  %566 = phi i64 [ %549, %539 ], [ %564, %550 ]
  %567 = icmp ne i64 %522, %566
  br i1 %567, label %568, label %569

; <label>:568:                                    ; preds = %565
  store i1 false, i1* %57, align 1
  br label %704

; <label>:569:                                    ; preds = %565
  %570 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8
  store %"class.std::__1::basic_string"* %570, %"class.std::__1::basic_string"** %36, align 8
  %571 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8
  store %"class.std::__1::basic_string"* %571, %"class.std::__1::basic_string"** %35, align 8
  %572 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %35, align 8
  store %"class.std::__1::basic_string"* %572, %"class.std::__1::basic_string"** %34, align 8
  %573 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %34, align 8
  %574 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %573, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %574, %"class.std::__1::__compressed_pair.36"** %33, align 8
  %575 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %33, align 8
  %576 = bitcast %"class.std::__1::__compressed_pair.36"* %575 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %576, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %32, align 8
  %577 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %32, align 8
  %578 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %577, i32 0, i32 0
  %579 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %578, i32 0, i32 0
  %580 = bitcast %union.anon* %579 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %581 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %580, i32 0, i32 0
  %582 = bitcast %union.anon.40* %581 to i8*
  %583 = load i8, i8* %582, align 8
  %584 = zext i8 %583 to i32
  %585 = and i32 %584, 1
  %586 = icmp ne i32 %585, 0
  br i1 %586, label %587, label %598

; <label>:587:                                    ; preds = %569
  store %"class.std::__1::basic_string"* %572, %"class.std::__1::basic_string"** %26, align 8
  %588 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %26, align 8
  %589 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %588, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %589, %"class.std::__1::__compressed_pair.36"** %25, align 8
  %590 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %25, align 8
  %591 = bitcast %"class.std::__1::__compressed_pair.36"* %590 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %591, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %24, align 8
  %592 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %24, align 8
  %593 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %592, i32 0, i32 0
  %594 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %593, i32 0, i32 0
  %595 = bitcast %union.anon* %594 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %596 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %595, i32 0, i32 2
  %597 = load i8*, i8** %596, align 8
  br label %611

; <label>:598:                                    ; preds = %569
  store %"class.std::__1::basic_string"* %572, %"class.std::__1::basic_string"** %31, align 8
  %599 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %31, align 8
  %600 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %599, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %600, %"class.std::__1::__compressed_pair.36"** %30, align 8
  %601 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %30, align 8
  %602 = bitcast %"class.std::__1::__compressed_pair.36"* %601 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %602, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %29, align 8
  %603 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %29, align 8
  %604 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %603, i32 0, i32 0
  %605 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %604, i32 0, i32 0
  %606 = bitcast %union.anon* %605 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %607 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %606, i32 0, i32 1
  %608 = getelementptr inbounds [23 x i8], [23 x i8]* %607, i64 0, i64 0
  store i8* %608, i8** %28, align 8
  %609 = load i8*, i8** %28, align 8
  store i8* %609, i8** %27, align 8
  %610 = load i8*, i8** %27, align 8
  br label %611

; <label>:611:                                    ; preds = %598, %587
  %612 = phi i8* [ %597, %587 ], [ %610, %598 ]
  store i8* %612, i8** %23, align 8
  %613 = load i8*, i8** %23, align 8
  store i8* %613, i8** %61, align 8
  %614 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %59, align 8
  store %"class.std::__1::basic_string"* %614, %"class.std::__1::basic_string"** %19, align 8
  %615 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %19, align 8
  store %"class.std::__1::basic_string"* %615, %"class.std::__1::basic_string"** %18, align 8
  %616 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %18, align 8
  store %"class.std::__1::basic_string"* %616, %"class.std::__1::basic_string"** %17, align 8
  %617 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8
  %618 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %617, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %618, %"class.std::__1::__compressed_pair.36"** %16, align 8
  %619 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %16, align 8
  %620 = bitcast %"class.std::__1::__compressed_pair.36"* %619 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %620, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %15, align 8
  %621 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %15, align 8
  %622 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %621, i32 0, i32 0
  %623 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %622, i32 0, i32 0
  %624 = bitcast %union.anon* %623 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %625 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %624, i32 0, i32 0
  %626 = bitcast %union.anon.40* %625 to i8*
  %627 = load i8, i8* %626, align 8
  %628 = zext i8 %627 to i32
  %629 = and i32 %628, 1
  %630 = icmp ne i32 %629, 0
  br i1 %630, label %631, label %642

; <label>:631:                                    ; preds = %611
  store %"class.std::__1::basic_string"* %616, %"class.std::__1::basic_string"** %9, align 8
  %632 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %9, align 8
  %633 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %632, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %633, %"class.std::__1::__compressed_pair.36"** %8, align 8
  %634 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %8, align 8
  %635 = bitcast %"class.std::__1::__compressed_pair.36"* %634 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %635, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %7, align 8
  %636 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %7, align 8
  %637 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %636, i32 0, i32 0
  %638 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %637, i32 0, i32 0
  %639 = bitcast %union.anon* %638 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %640 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %639, i32 0, i32 2
  %641 = load i8*, i8** %640, align 8
  br label %655

; <label>:642:                                    ; preds = %611
  store %"class.std::__1::basic_string"* %616, %"class.std::__1::basic_string"** %14, align 8
  %643 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8
  %644 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %643, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %644, %"class.std::__1::__compressed_pair.36"** %13, align 8
  %645 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %13, align 8
  %646 = bitcast %"class.std::__1::__compressed_pair.36"* %645 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %646, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %12, align 8
  %647 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %12, align 8
  %648 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %647, i32 0, i32 0
  %649 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %648, i32 0, i32 0
  %650 = bitcast %union.anon* %649 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %651 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %650, i32 0, i32 1
  %652 = getelementptr inbounds [23 x i8], [23 x i8]* %651, i64 0, i64 0
  store i8* %652, i8** %11, align 8
  %653 = load i8*, i8** %11, align 8
  store i8* %653, i8** %10, align 8
  %654 = load i8*, i8** %10, align 8
  br label %655

; <label>:655:                                    ; preds = %642, %631
  %656 = phi i8* [ %641, %631 ], [ %654, %642 ]
  store i8* %656, i8** %6, align 8
  %657 = load i8*, i8** %6, align 8
  store i8* %657, i8** %62, align 8
  %658 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8
  store %"class.std::__1::basic_string"* %658, %"class.std::__1::basic_string"** %22, align 8
  %659 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %22, align 8
  %660 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %659, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %660, %"class.std::__1::__compressed_pair.36"** %21, align 8
  %661 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %21, align 8
  %662 = bitcast %"class.std::__1::__compressed_pair.36"* %661 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %662, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %20, align 8
  %663 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %20, align 8
  %664 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %663, i32 0, i32 0
  %665 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %664, i32 0, i32 0
  %666 = bitcast %union.anon* %665 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %667 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %666, i32 0, i32 0
  %668 = bitcast %union.anon.40* %667 to i8*
  %669 = load i8, i8* %668, align 8
  %670 = zext i8 %669 to i32
  %671 = and i32 %670, 1
  %672 = icmp ne i32 %671, 0
  br i1 %672, label %673, label %680

; <label>:673:                                    ; preds = %655
  %674 = load i8*, i8** %61, align 8
  %675 = load i8*, i8** %62, align 8
  %676 = load i64, i64* %60, align 8
  %677 = invoke i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8* %674, i8* %675, i64 %676)
          to label %678 unwind label %701

; <label>:678:                                    ; preds = %673
  %679 = icmp eq i32 %677, 0
  store i1 %679, i1* %57, align 1
  br label %704

; <label>:680:                                    ; preds = %655
  br label %681

; <label>:681:                                    ; preds = %693, %680
  %682 = load i64, i64* %60, align 8
  %683 = icmp ne i64 %682, 0
  br i1 %683, label %684, label %700

; <label>:684:                                    ; preds = %681
  %685 = load i8*, i8** %61, align 8
  %686 = load i8, i8* %685, align 1
  %687 = sext i8 %686 to i32
  %688 = load i8*, i8** %62, align 8
  %689 = load i8, i8* %688, align 1
  %690 = sext i8 %689 to i32
  %691 = icmp ne i32 %687, %690
  br i1 %691, label %692, label %693

; <label>:692:                                    ; preds = %684
  store i1 false, i1* %57, align 1
  br label %704

; <label>:693:                                    ; preds = %684
  %694 = load i64, i64* %60, align 8
  %695 = add i64 %694, -1
  store i64 %695, i64* %60, align 8
  %696 = load i8*, i8** %61, align 8
  %697 = getelementptr inbounds i8, i8* %696, i32 1
  store i8* %697, i8** %61, align 8
  %698 = load i8*, i8** %62, align 8
  %699 = getelementptr inbounds i8, i8* %698, i32 1
  store i8* %699, i8** %62, align 8
  br label %681

; <label>:700:                                    ; preds = %681
  store i1 true, i1* %57, align 1
  br label %704

; <label>:701:                                    ; preds = %673
  %702 = landingpad { i8*, i32 }
          catch i8* null
  %703 = extractvalue { i8*, i32 } %702, 0
  call void @__clang_call_terminate(i8* %703) #15
  unreachable

; <label>:704:                                    ; preds = %700, %692, %678, %568
  %705 = load i1, i1* %57, align 1
  br i1 %705, label %706, label %707

; <label>:706:                                    ; preds = %704
  br label %1146

; <label>:707:                                    ; preds = %704
  %708 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %264, align 8
  %709 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %708, i32 0, i32 0
  %710 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %709, align 8
  store %"struct.std::__1::__hash_node_base"* %710, %"struct.std::__1::__hash_node_base"** %264, align 8
  br label %416

; <label>:711:                                    ; preds = %452
  br label %712

; <label>:712:                                    ; preds = %711, %396
  br label %713

; <label>:713:                                    ; preds = %712, %2
  %714 = load i64, i64* %261, align 8
  %715 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %258, align 8
  store %"struct.std::__1::piecewise_construct_t"* %715, %"struct.std::__1::piecewise_construct_t"** %69, align 8
  %716 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %69, align 8
  %717 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %259, align 8
  store %"class.std::__1::tuple"* %717, %"class.std::__1::tuple"** %70, align 8
  %718 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %70, align 8
  %719 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %260, align 8
  store %"class.std::__1::tuple.131"* %719, %"class.std::__1::tuple.131"** %71, align 8
  %720 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %71, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE21__construct_node_hashIRKNS_21piecewise_construct_tEJNS_5tupleIJRKS7_EEENSQ_IJEEEEEENS_10unique_ptrINS_11__hash_nodeISB_PvEENS_22__hash_node_destructorINS5_ISY_EEEEEEmOT_DpOT0_(%"class.std::__1::unique_ptr.132"* sret %266, %"class.std::__1::__hash_table"* %342, i64 %714, %"struct.std::__1::piecewise_construct_t"* dereferenceable(1) %716, %"class.std::__1::tuple"* dereferenceable(8) %718, %"class.std::__1::tuple.131"* dereferenceable(1) %720)
  store %"class.std::__1::__hash_table"* %342, %"class.std::__1::__hash_table"** %77, align 8
  %721 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %77, align 8
  %722 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %721, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.6"* %722, %"class.std::__1::__compressed_pair.6"** %76, align 8
  %723 = load %"class.std::__1::__compressed_pair.6"*, %"class.std::__1::__compressed_pair.6"** %76, align 8
  %724 = bitcast %"class.std::__1::__compressed_pair.6"* %723 to %"class.std::__1::__libcpp_compressed_pair_imp.7"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.7"* %724, %"class.std::__1::__libcpp_compressed_pair_imp.7"** %75, align 8
  %725 = load %"class.std::__1::__libcpp_compressed_pair_imp.7"*, %"class.std::__1::__libcpp_compressed_pair_imp.7"** %75, align 8
  %726 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.7", %"class.std::__1::__libcpp_compressed_pair_imp.7"* %725, i32 0, i32 0
  %727 = load i64, i64* %726, align 8
  %728 = add i64 %727, 1
  %729 = uitofp i64 %728 to float
  %730 = load i64, i64* %262, align 8
  %731 = uitofp i64 %730 to float
  store %"class.std::__1::__hash_table"* %342, %"class.std::__1::__hash_table"** %80, align 8
  %732 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %80, align 8
  %733 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %732, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.8"* %733, %"class.std::__1::__compressed_pair.8"** %79, align 8
  %734 = load %"class.std::__1::__compressed_pair.8"*, %"class.std::__1::__compressed_pair.8"** %79, align 8
  %735 = bitcast %"class.std::__1::__compressed_pair.8"* %734 to %"class.std::__1::__libcpp_compressed_pair_imp.9"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.9"* %735, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %78, align 8
  %736 = load %"class.std::__1::__libcpp_compressed_pair_imp.9"*, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %78, align 8
  %737 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.9", %"class.std::__1::__libcpp_compressed_pair_imp.9"* %736, i32 0, i32 0
  %738 = load float, float* %737, align 4
  %739 = fmul float %731, %738
  %740 = fcmp ogt float %729, %739
  br i1 %740, label %744, label %741

; <label>:741:                                    ; preds = %713
  %742 = load i64, i64* %262, align 8
  %743 = icmp eq i64 %742, 0
  br i1 %743, label %744, label %906

; <label>:744:                                    ; preds = %741, %713
  %745 = load i64, i64* %262, align 8
  %746 = mul i64 2, %745
  %747 = load i64, i64* %262, align 8
  store i64 %747, i64* %81, align 8
  %748 = load i64, i64* %81, align 8
  %749 = icmp ugt i64 %748, 2
  br i1 %749, label %750, label %757

; <label>:750:                                    ; preds = %744
  %751 = load i64, i64* %81, align 8
  %752 = load i64, i64* %81, align 8
  %753 = sub i64 %752, 1
  %754 = and i64 %751, %753
  %755 = icmp ne i64 %754, 0
  %756 = xor i1 %755, true
  br label %757

; <label>:757:                                    ; preds = %750, %744
  %758 = phi i1 [ false, %744 ], [ %756, %750 ]
  %759 = xor i1 %758, true
  %760 = zext i1 %759 to i64
  %761 = add i64 %746, %760
  store i64 %761, i64* %267, align 8
  store %"class.std::__1::__hash_table"* %342, %"class.std::__1::__hash_table"** %84, align 8
  %762 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %84, align 8
  %763 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %762, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.6"* %763, %"class.std::__1::__compressed_pair.6"** %83, align 8
  %764 = load %"class.std::__1::__compressed_pair.6"*, %"class.std::__1::__compressed_pair.6"** %83, align 8
  %765 = bitcast %"class.std::__1::__compressed_pair.6"* %764 to %"class.std::__1::__libcpp_compressed_pair_imp.7"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.7"* %765, %"class.std::__1::__libcpp_compressed_pair_imp.7"** %82, align 8
  %766 = load %"class.std::__1::__libcpp_compressed_pair_imp.7"*, %"class.std::__1::__libcpp_compressed_pair_imp.7"** %82, align 8
  %767 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.7", %"class.std::__1::__libcpp_compressed_pair_imp.7"* %766, i32 0, i32 0
  %768 = load i64, i64* %767, align 8
  %769 = add i64 %768, 1
  %770 = uitofp i64 %769 to float
  store %"class.std::__1::__hash_table"* %342, %"class.std::__1::__hash_table"** %87, align 8
  %771 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %87, align 8
  %772 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %771, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.8"* %772, %"class.std::__1::__compressed_pair.8"** %86, align 8
  %773 = load %"class.std::__1::__compressed_pair.8"*, %"class.std::__1::__compressed_pair.8"** %86, align 8
  %774 = bitcast %"class.std::__1::__compressed_pair.8"* %773 to %"class.std::__1::__libcpp_compressed_pair_imp.9"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.9"* %774, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %85, align 8
  %775 = load %"class.std::__1::__libcpp_compressed_pair_imp.9"*, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %85, align 8
  %776 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.9", %"class.std::__1::__libcpp_compressed_pair_imp.9"* %775, i32 0, i32 0
  %777 = load float, float* %776, align 4
  %778 = fdiv float %770, %777
  store float %778, float* %91, align 4
  %779 = load float, float* %91, align 4
  %780 = call float @ceilf(float %779) #12
  %781 = fptoui float %780 to i64
  store i64 %781, i64* %270, align 8
  store i64* %267, i64** %98, align 8
  store i64* %270, i64** %99, align 8
  %782 = load i64*, i64** %98, align 8
  %783 = load i64*, i64** %99, align 8
  store i64* %782, i64** %96, align 8
  store i64* %783, i64** %97, align 8
  %784 = load i64*, i64** %96, align 8
  %785 = load i64*, i64** %97, align 8
  store %"struct.std::__1::__less"* %95, %"struct.std::__1::__less"** %92, align 8
  store i64* %784, i64** %93, align 8
  store i64* %785, i64** %94, align 8
  %786 = load %"struct.std::__1::__less"*, %"struct.std::__1::__less"** %92, align 8
  %787 = load i64*, i64** %93, align 8
  %788 = load i64, i64* %787, align 8
  %789 = load i64*, i64** %94, align 8
  %790 = load i64, i64* %789, align 8
  %791 = icmp ult i64 %788, %790
  br i1 %791, label %792, label %794

; <label>:792:                                    ; preds = %757
  %793 = load i64*, i64** %97, align 8
  br label %796

; <label>:794:                                    ; preds = %757
  %795 = load i64*, i64** %96, align 8
  br label %796

; <label>:796:                                    ; preds = %794, %792
  %797 = phi i64* [ %793, %792 ], [ %795, %794 ]
  %798 = load i64, i64* %797, align 8
  invoke void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE6rehashEm(%"class.std::__1::__hash_table"* %342, i64 %798)
          to label %799 unwind label %841

; <label>:799:                                    ; preds = %796
  store %"class.std::__1::__hash_table"* %342, %"class.std::__1::__hash_table"** %109, align 8
  %800 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %109, align 8
  %801 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %800, i32 0, i32 0
  store %"class.std::__1::unique_ptr"* %801, %"class.std::__1::unique_ptr"** %108, align 8
  %802 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %108, align 8
  %803 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %802, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %803, %"class.std::__1::__compressed_pair"** %107, align 8
  %804 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %107, align 8
  %805 = bitcast %"class.std::__1::__compressed_pair"* %804 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %805, %"class.std::__1::__libcpp_compressed_pair_imp"** %106, align 8
  %806 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %106, align 8
  %807 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %806, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator"* %807, %"class.std::__1::__bucket_list_deallocator"** %105, align 8
  %808 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %105, align 8
  %809 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %808, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.0"* %809, %"class.std::__1::__compressed_pair.0"** %104, align 8
  %810 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %104, align 8
  %811 = bitcast %"class.std::__1::__compressed_pair.0"* %810 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %811, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %103, align 8
  %812 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %103, align 8
  %813 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.1", %"class.std::__1::__libcpp_compressed_pair_imp.1"* %812, i32 0, i32 0
  %814 = load i64, i64* %813, align 8
  store i64 %814, i64* %262, align 8
  %815 = load i64, i64* %261, align 8
  %816 = load i64, i64* %262, align 8
  store i64 %815, i64* %110, align 8
  store i64 %816, i64* %111, align 8
  %817 = load i64, i64* %111, align 8
  %818 = load i64, i64* %111, align 8
  %819 = sub i64 %818, 1
  %820 = and i64 %817, %819
  %821 = icmp ne i64 %820, 0
  br i1 %821, label %827, label %822

; <label>:822:                                    ; preds = %799
  %823 = load i64, i64* %110, align 8
  %824 = load i64, i64* %111, align 8
  %825 = sub i64 %824, 1
  %826 = and i64 %823, %825
  br label %839

; <label>:827:                                    ; preds = %799
  %828 = load i64, i64* %110, align 8
  %829 = load i64, i64* %111, align 8
  %830 = icmp ult i64 %828, %829
  br i1 %830, label %831, label %833

; <label>:831:                                    ; preds = %827
  %832 = load i64, i64* %110, align 8
  br label %837

; <label>:833:                                    ; preds = %827
  %834 = load i64, i64* %110, align 8
  %835 = load i64, i64* %111, align 8
  %836 = urem i64 %834, %835
  br label %837

; <label>:837:                                    ; preds = %833, %831
  %838 = phi i64 [ %832, %831 ], [ %836, %833 ]
  br label %839

; <label>:839:                                    ; preds = %837, %822
  %840 = phi i64 [ %826, %822 ], [ %838, %837 ]
  store i64 %840, i64* %265, align 8
  br label %906

; <label>:841:                                    ; preds = %796
  %842 = landingpad { i8*, i32 }
          cleanup
  %843 = extractvalue { i8*, i32 } %842, 0
  store i8* %843, i8** %268, align 8
  %844 = extractvalue { i8*, i32 } %842, 1
  store i32 %844, i32* %269, align 4
  store %"class.std::__1::unique_ptr.132"* %266, %"class.std::__1::unique_ptr.132"** %144, align 8
  %845 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %144, align 8
  store %"class.std::__1::unique_ptr.132"* %845, %"class.std::__1::unique_ptr.132"** %143, align 8
  %846 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %143, align 8
  store %"class.std::__1::unique_ptr.132"* %846, %"class.std::__1::unique_ptr.132"** %140, align 8
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %141, align 8
  %847 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %140, align 8
  %848 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %847, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %848, %"class.std::__1::__compressed_pair.133"** %139, align 8
  %849 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %139, align 8
  %850 = bitcast %"class.std::__1::__compressed_pair.133"* %849 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %850, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %138, align 8
  %851 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %138, align 8
  %852 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %851, i32 0, i32 0
  %853 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %852, align 8
  store %"struct.std::__1::__hash_node"* %853, %"struct.std::__1::__hash_node"** %142, align 8
  %854 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %141, align 8
  %855 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %847, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %855, %"class.std::__1::__compressed_pair.133"** %120, align 8
  %856 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %120, align 8
  %857 = bitcast %"class.std::__1::__compressed_pair.133"* %856 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %857, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %119, align 8
  %858 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %119, align 8
  %859 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %858, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* %854, %"struct.std::__1::__hash_node"** %859, align 8
  %860 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %142, align 8
  %861 = icmp ne %"struct.std::__1::__hash_node"* %860, null
  br i1 %861, label %862, label %901

; <label>:862:                                    ; preds = %841
  %863 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %847, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %863, %"class.std::__1::__compressed_pair.133"** %118, align 8
  %864 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %118, align 8
  %865 = bitcast %"class.std::__1::__compressed_pair.133"* %864 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %865, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %117, align 8
  %866 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %117, align 8
  %867 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %866, i32 0, i32 1
  %868 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %142, align 8
  store %"class.std::__1::__hash_node_destructor"* %867, %"class.std::__1::__hash_node_destructor"** %136, align 8
  store %"struct.std::__1::__hash_node"* %868, %"struct.std::__1::__hash_node"** %137, align 8
  %869 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %136, align 8
  %870 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %869, i32 0, i32 1
  %871 = load i8, i8* %870, align 8
  %872 = trunc i8 %871 to i1
  br i1 %872, label %873, label %886

; <label>:873:                                    ; preds = %862
  %874 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %869, i32 0, i32 0
  %875 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %874, align 8
  %876 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %137, align 8
  %877 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %876, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %877, %"union.std::__1::__hash_value_type"** %135, align 8
  %878 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %135, align 8
  %879 = bitcast %"union.std::__1::__hash_value_type"* %878 to %"struct.std::__1::pair"*
  store %"struct.std::__1::pair"* %879, %"struct.std::__1::pair"** %134, align 8
  %880 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %134, align 8
  %881 = bitcast %"struct.std::__1::pair"* %880 to i8*
  store %"class.std::__1::allocator.4"* %875, %"class.std::__1::allocator.4"** %130, align 8
  store %"struct.std::__1::pair"* %880, %"struct.std::__1::pair"** %131, align 8
  %882 = bitcast %"struct.std::__1::__has_destroy"* %133 to %"struct.std::__1::integral_constant"*
  %883 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %130, align 8
  %884 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %131, align 8
  store %"class.std::__1::allocator.4"* %883, %"class.std::__1::allocator.4"** %128, align 8
  store %"struct.std::__1::pair"* %884, %"struct.std::__1::pair"** %129, align 8
  %885 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %129, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEED1Ev(%"struct.std::__1::pair"* %885) #2
  br label %886

; <label>:886:                                    ; preds = %873, %862
  %887 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %137, align 8
  %888 = icmp ne %"struct.std::__1::__hash_node"* %887, null
  br i1 %888, label %889, label %900

; <label>:889:                                    ; preds = %886
  %890 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %869, i32 0, i32 0
  %891 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %890, align 8
  %892 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %137, align 8
  store %"class.std::__1::allocator.4"* %891, %"class.std::__1::allocator.4"** %125, align 8
  store %"struct.std::__1::__hash_node"* %892, %"struct.std::__1::__hash_node"** %126, align 8
  store i64 1, i64* %127, align 8
  %893 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %125, align 8
  %894 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %126, align 8
  %895 = load i64, i64* %127, align 8
  store %"class.std::__1::allocator.4"* %893, %"class.std::__1::allocator.4"** %122, align 8
  store %"struct.std::__1::__hash_node"* %894, %"struct.std::__1::__hash_node"** %123, align 8
  store i64 %895, i64* %124, align 8
  %896 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %122, align 8
  %897 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %123, align 8
  %898 = bitcast %"struct.std::__1::__hash_node"* %897 to i8*
  store i8* %898, i8** %121, align 8
  %899 = load i8*, i8** %121, align 8
  call void @_ZdlPv(i8* %899) #14
  br label %900

; <label>:900:                                    ; preds = %889, %886
  br label %901

; <label>:901:                                    ; preds = %900, %841
  %902 = load i8*, i8** %268, align 8
  %903 = load i32, i32* %269, align 4
  %904 = insertvalue { i8*, i32 } undef, i8* %902, 0
  %905 = insertvalue { i8*, i32 } %904, i32 %903, 1
  resume { i8*, i32 } %905

; <label>:906:                                    ; preds = %839, %741
  %907 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %342, i32 0, i32 0
  %908 = load i64, i64* %265, align 8
  store %"class.std::__1::unique_ptr"* %907, %"class.std::__1::unique_ptr"** %147, align 8
  store i64 %908, i64* %148, align 8
  %909 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %147, align 8
  %910 = load i64, i64* %148, align 8
  %911 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %909, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %911, %"class.std::__1::__compressed_pair"** %146, align 8
  %912 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %146, align 8
  %913 = bitcast %"class.std::__1::__compressed_pair"* %912 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %913, %"class.std::__1::__libcpp_compressed_pair_imp"** %145, align 8
  %914 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %145, align 8
  %915 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %914, i32 0, i32 0
  %916 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %915, align 8
  %917 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %916, i64 %910
  %918 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %917, align 8
  store %"struct.std::__1::__hash_node_base"* %918, %"struct.std::__1::__hash_node_base"** %271, align 8
  %919 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %271, align 8
  %920 = icmp eq %"struct.std::__1::__hash_node_base"* %919, null
  br i1 %920, label %921, label %1043

; <label>:921:                                    ; preds = %906
  %922 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %342, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.2"* %922, %"class.std::__1::__compressed_pair.2"** %150, align 8
  %923 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %150, align 8
  %924 = bitcast %"class.std::__1::__compressed_pair.2"* %923 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %924, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %149, align 8
  %925 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %149, align 8
  %926 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %925, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %926, %"struct.std::__1::__hash_node_base"** %153, align 8
  %927 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %153, align 8
  store %"struct.std::__1::__hash_node_base"* %927, %"struct.std::__1::__hash_node_base"** %152, align 8
  %928 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %152, align 8
  store %"struct.std::__1::__hash_node_base"* %928, %"struct.std::__1::__hash_node_base"** %151, align 8
  %929 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %151, align 8
  %930 = bitcast %"struct.std::__1::__hash_node_base"* %929 to i8*
  store %"struct.std::__1::__hash_node_base"* %929, %"struct.std::__1::__hash_node_base"** %271, align 8
  %931 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %271, align 8
  %932 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %931, i32 0, i32 0
  %933 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %932, align 8
  store %"class.std::__1::unique_ptr.132"* %266, %"class.std::__1::unique_ptr.132"** %156, align 8
  %934 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %156, align 8
  %935 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %934, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %935, %"class.std::__1::__compressed_pair.133"** %155, align 8
  %936 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %155, align 8
  %937 = bitcast %"class.std::__1::__compressed_pair.133"* %936 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %937, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %154, align 8
  %938 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %154, align 8
  %939 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %938, i32 0, i32 0
  %940 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %939, align 8
  %941 = bitcast %"struct.std::__1::__hash_node"* %940 to %"struct.std::__1::__hash_node_base"*
  %942 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %941, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %933, %"struct.std::__1::__hash_node_base"** %942, align 8
  store %"class.std::__1::unique_ptr.132"* %266, %"class.std::__1::unique_ptr.132"** %159, align 8
  %943 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %159, align 8
  %944 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %943, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %944, %"class.std::__1::__compressed_pair.133"** %158, align 8
  %945 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %158, align 8
  %946 = bitcast %"class.std::__1::__compressed_pair.133"* %945 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %946, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %157, align 8
  %947 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %157, align 8
  %948 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %947, i32 0, i32 0
  %949 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %948, align 8
  %950 = bitcast %"struct.std::__1::__hash_node"* %949 to %"struct.std::__1::__hash_node_base"*
  store %"struct.std::__1::__hash_node_base"* %950, %"struct.std::__1::__hash_node_base"** %162, align 8
  %951 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %162, align 8
  store %"struct.std::__1::__hash_node_base"* %951, %"struct.std::__1::__hash_node_base"** %161, align 8
  %952 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %161, align 8
  store %"struct.std::__1::__hash_node_base"* %952, %"struct.std::__1::__hash_node_base"** %160, align 8
  %953 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %160, align 8
  %954 = bitcast %"struct.std::__1::__hash_node_base"* %953 to i8*
  %955 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %271, align 8
  %956 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %955, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %953, %"struct.std::__1::__hash_node_base"** %956, align 8
  %957 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %271, align 8
  %958 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %342, i32 0, i32 0
  %959 = load i64, i64* %265, align 8
  store %"class.std::__1::unique_ptr"* %958, %"class.std::__1::unique_ptr"** %165, align 8
  store i64 %959, i64* %166, align 8
  %960 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %165, align 8
  %961 = load i64, i64* %166, align 8
  %962 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %960, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %962, %"class.std::__1::__compressed_pair"** %164, align 8
  %963 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %164, align 8
  %964 = bitcast %"class.std::__1::__compressed_pair"* %963 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %964, %"class.std::__1::__libcpp_compressed_pair_imp"** %163, align 8
  %965 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %163, align 8
  %966 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %965, i32 0, i32 0
  %967 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %966, align 8
  %968 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %967, i64 %961
  store %"struct.std::__1::__hash_node_base"* %957, %"struct.std::__1::__hash_node_base"** %968, align 8
  store %"class.std::__1::unique_ptr.132"* %266, %"class.std::__1::unique_ptr.132"** %169, align 8
  %969 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %169, align 8
  %970 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %969, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %970, %"class.std::__1::__compressed_pair.133"** %168, align 8
  %971 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %168, align 8
  %972 = bitcast %"class.std::__1::__compressed_pair.133"* %971 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %972, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %167, align 8
  %973 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %167, align 8
  %974 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %973, i32 0, i32 0
  %975 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %974, align 8
  %976 = bitcast %"struct.std::__1::__hash_node"* %975 to %"struct.std::__1::__hash_node_base"*
  %977 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %976, i32 0, i32 0
  %978 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %977, align 8
  %979 = icmp ne %"struct.std::__1::__hash_node_base"* %978, null
  br i1 %979, label %980, label %1042

; <label>:980:                                    ; preds = %921
  store %"class.std::__1::unique_ptr.132"* %266, %"class.std::__1::unique_ptr.132"** %172, align 8
  %981 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %172, align 8
  %982 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %981, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %982, %"class.std::__1::__compressed_pair.133"** %171, align 8
  %983 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %171, align 8
  %984 = bitcast %"class.std::__1::__compressed_pair.133"* %983 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %984, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %170, align 8
  %985 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %170, align 8
  %986 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %985, i32 0, i32 0
  %987 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %986, align 8
  %988 = bitcast %"struct.std::__1::__hash_node"* %987 to %"struct.std::__1::__hash_node_base"*
  store %"struct.std::__1::__hash_node_base"* %988, %"struct.std::__1::__hash_node_base"** %175, align 8
  %989 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %175, align 8
  store %"struct.std::__1::__hash_node_base"* %989, %"struct.std::__1::__hash_node_base"** %174, align 8
  %990 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %174, align 8
  store %"struct.std::__1::__hash_node_base"* %990, %"struct.std::__1::__hash_node_base"** %173, align 8
  %991 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %173, align 8
  %992 = bitcast %"struct.std::__1::__hash_node_base"* %991 to i8*
  %993 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %342, i32 0, i32 0
  store %"class.std::__1::unique_ptr.132"* %266, %"class.std::__1::unique_ptr.132"** %178, align 8
  %994 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %178, align 8
  %995 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %994, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %995, %"class.std::__1::__compressed_pair.133"** %177, align 8
  %996 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %177, align 8
  %997 = bitcast %"class.std::__1::__compressed_pair.133"* %996 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %997, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %176, align 8
  %998 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %176, align 8
  %999 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %998, i32 0, i32 0
  %1000 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %999, align 8
  %1001 = bitcast %"struct.std::__1::__hash_node"* %1000 to %"struct.std::__1::__hash_node_base"*
  %1002 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %1001, i32 0, i32 0
  %1003 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %1002, align 8
  store %"struct.std::__1::__hash_node_base"* %1003, %"struct.std::__1::__hash_node_base"** %179, align 8
  %1004 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %179, align 8
  %1005 = bitcast %"struct.std::__1::__hash_node_base"* %1004 to %"struct.std::__1::__hash_node"*
  %1006 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %1005, i32 0, i32 1
  %1007 = load i64, i64* %1006, align 8
  %1008 = load i64, i64* %262, align 8
  store i64 %1007, i64* %180, align 8
  store i64 %1008, i64* %181, align 8
  %1009 = load i64, i64* %181, align 8
  %1010 = load i64, i64* %181, align 8
  %1011 = sub i64 %1010, 1
  %1012 = and i64 %1009, %1011
  %1013 = icmp ne i64 %1012, 0
  br i1 %1013, label %1019, label %1014

; <label>:1014:                                   ; preds = %980
  %1015 = load i64, i64* %180, align 8
  %1016 = load i64, i64* %181, align 8
  %1017 = sub i64 %1016, 1
  %1018 = and i64 %1015, %1017
  br label %1031

; <label>:1019:                                   ; preds = %980
  %1020 = load i64, i64* %180, align 8
  %1021 = load i64, i64* %181, align 8
  %1022 = icmp ult i64 %1020, %1021
  br i1 %1022, label %1023, label %1025

; <label>:1023:                                   ; preds = %1019
  %1024 = load i64, i64* %180, align 8
  br label %1029

; <label>:1025:                                   ; preds = %1019
  %1026 = load i64, i64* %180, align 8
  %1027 = load i64, i64* %181, align 8
  %1028 = urem i64 %1026, %1027
  br label %1029

; <label>:1029:                                   ; preds = %1025, %1023
  %1030 = phi i64 [ %1024, %1023 ], [ %1028, %1025 ]
  br label %1031

; <label>:1031:                                   ; preds = %1029, %1014
  %1032 = phi i64 [ %1018, %1014 ], [ %1030, %1029 ]
  store %"class.std::__1::unique_ptr"* %993, %"class.std::__1::unique_ptr"** %184, align 8
  store i64 %1032, i64* %185, align 8
  %1033 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %184, align 8
  %1034 = load i64, i64* %185, align 8
  %1035 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %1033, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1035, %"class.std::__1::__compressed_pair"** %183, align 8
  %1036 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %183, align 8
  %1037 = bitcast %"class.std::__1::__compressed_pair"* %1036 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1037, %"class.std::__1::__libcpp_compressed_pair_imp"** %182, align 8
  %1038 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %182, align 8
  %1039 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1038, i32 0, i32 0
  %1040 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %1039, align 8
  %1041 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %1040, i64 %1034
  store %"struct.std::__1::__hash_node_base"* %991, %"struct.std::__1::__hash_node_base"** %1041, align 8
  br label %1042

; <label>:1042:                                   ; preds = %1031, %921
  br label %1066

; <label>:1043:                                   ; preds = %906
  %1044 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %271, align 8
  %1045 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %1044, i32 0, i32 0
  %1046 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %1045, align 8
  store %"class.std::__1::unique_ptr.132"* %266, %"class.std::__1::unique_ptr.132"** %188, align 8
  %1047 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %188, align 8
  %1048 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %1047, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %1048, %"class.std::__1::__compressed_pair.133"** %187, align 8
  %1049 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %187, align 8
  %1050 = bitcast %"class.std::__1::__compressed_pair.133"* %1049 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %1050, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %186, align 8
  %1051 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %186, align 8
  %1052 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %1051, i32 0, i32 0
  %1053 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %1052, align 8
  %1054 = bitcast %"struct.std::__1::__hash_node"* %1053 to %"struct.std::__1::__hash_node_base"*
  %1055 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %1054, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %1046, %"struct.std::__1::__hash_node_base"** %1055, align 8
  store %"class.std::__1::unique_ptr.132"* %266, %"class.std::__1::unique_ptr.132"** %191, align 8
  %1056 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %191, align 8
  %1057 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %1056, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %1057, %"class.std::__1::__compressed_pair.133"** %190, align 8
  %1058 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %190, align 8
  %1059 = bitcast %"class.std::__1::__compressed_pair.133"* %1058 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %1059, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %189, align 8
  %1060 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %189, align 8
  %1061 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %1060, i32 0, i32 0
  %1062 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %1061, align 8
  %1063 = bitcast %"struct.std::__1::__hash_node"* %1062 to %"struct.std::__1::__hash_node_base"*
  %1064 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %271, align 8
  %1065 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %1064, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* %1063, %"struct.std::__1::__hash_node_base"** %1065, align 8
  br label %1066

; <label>:1066:                                   ; preds = %1043, %1042
  store %"class.std::__1::unique_ptr.132"* %266, %"class.std::__1::unique_ptr.132"** %196, align 8
  %1067 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %196, align 8
  %1068 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %1067, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %1068, %"class.std::__1::__compressed_pair.133"** %195, align 8
  %1069 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %195, align 8
  %1070 = bitcast %"class.std::__1::__compressed_pair.133"* %1069 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %1070, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %194, align 8
  %1071 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %194, align 8
  %1072 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %1071, i32 0, i32 0
  %1073 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %1072, align 8
  store %"struct.std::__1::__hash_node"* %1073, %"struct.std::__1::__hash_node"** %197, align 8
  %1074 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %1067, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %1074, %"class.std::__1::__compressed_pair.133"** %193, align 8
  %1075 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %193, align 8
  %1076 = bitcast %"class.std::__1::__compressed_pair.133"* %1075 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %1076, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %192, align 8
  %1077 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %192, align 8
  %1078 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %1077, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %1078, align 8
  %1079 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %197, align 8
  %1080 = bitcast %"struct.std::__1::__hash_node"* %1079 to %"struct.std::__1::__hash_node_base"*
  store %"struct.std::__1::__hash_node_base"* %1080, %"struct.std::__1::__hash_node_base"** %264, align 8
  store %"class.std::__1::__hash_table"* %342, %"class.std::__1::__hash_table"** %200, align 8
  %1081 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %200, align 8
  %1082 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %1081, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.6"* %1082, %"class.std::__1::__compressed_pair.6"** %199, align 8
  %1083 = load %"class.std::__1::__compressed_pair.6"*, %"class.std::__1::__compressed_pair.6"** %199, align 8
  %1084 = bitcast %"class.std::__1::__compressed_pair.6"* %1083 to %"class.std::__1::__libcpp_compressed_pair_imp.7"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.7"* %1084, %"class.std::__1::__libcpp_compressed_pair_imp.7"** %198, align 8
  %1085 = load %"class.std::__1::__libcpp_compressed_pair_imp.7"*, %"class.std::__1::__libcpp_compressed_pair_imp.7"** %198, align 8
  %1086 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.7", %"class.std::__1::__libcpp_compressed_pair_imp.7"* %1085, i32 0, i32 0
  %1087 = load i64, i64* %1086, align 8
  %1088 = add i64 %1087, 1
  store i64 %1088, i64* %1086, align 8
  store i8 1, i8* %263, align 1
  store %"class.std::__1::unique_ptr.132"* %266, %"class.std::__1::unique_ptr.132"** %239, align 8
  %1089 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %239, align 8
  store %"class.std::__1::unique_ptr.132"* %1089, %"class.std::__1::unique_ptr.132"** %238, align 8
  %1090 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %238, align 8
  store %"class.std::__1::unique_ptr.132"* %1090, %"class.std::__1::unique_ptr.132"** %235, align 8
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %236, align 8
  %1091 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %235, align 8
  %1092 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %1091, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %1092, %"class.std::__1::__compressed_pair.133"** %234, align 8
  %1093 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %234, align 8
  %1094 = bitcast %"class.std::__1::__compressed_pair.133"* %1093 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %1094, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %233, align 8
  %1095 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %233, align 8
  %1096 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %1095, i32 0, i32 0
  %1097 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %1096, align 8
  store %"struct.std::__1::__hash_node"* %1097, %"struct.std::__1::__hash_node"** %237, align 8
  %1098 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %236, align 8
  %1099 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %1091, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %1099, %"class.std::__1::__compressed_pair.133"** %215, align 8
  %1100 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %215, align 8
  %1101 = bitcast %"class.std::__1::__compressed_pair.133"* %1100 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %1101, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %214, align 8
  %1102 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %214, align 8
  %1103 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %1102, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* %1098, %"struct.std::__1::__hash_node"** %1103, align 8
  %1104 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %237, align 8
  %1105 = icmp ne %"struct.std::__1::__hash_node"* %1104, null
  br i1 %1105, label %1106, label %1145

; <label>:1106:                                   ; preds = %1066
  %1107 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %1091, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %1107, %"class.std::__1::__compressed_pair.133"** %213, align 8
  %1108 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %213, align 8
  %1109 = bitcast %"class.std::__1::__compressed_pair.133"* %1108 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %1109, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %212, align 8
  %1110 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %212, align 8
  %1111 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %1110, i32 0, i32 1
  %1112 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %237, align 8
  store %"class.std::__1::__hash_node_destructor"* %1111, %"class.std::__1::__hash_node_destructor"** %231, align 8
  store %"struct.std::__1::__hash_node"* %1112, %"struct.std::__1::__hash_node"** %232, align 8
  %1113 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %231, align 8
  %1114 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %1113, i32 0, i32 1
  %1115 = load i8, i8* %1114, align 8
  %1116 = trunc i8 %1115 to i1
  br i1 %1116, label %1117, label %1130

; <label>:1117:                                   ; preds = %1106
  %1118 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %1113, i32 0, i32 0
  %1119 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %1118, align 8
  %1120 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %232, align 8
  %1121 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %1120, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %1121, %"union.std::__1::__hash_value_type"** %230, align 8
  %1122 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %230, align 8
  %1123 = bitcast %"union.std::__1::__hash_value_type"* %1122 to %"struct.std::__1::pair"*
  store %"struct.std::__1::pair"* %1123, %"struct.std::__1::pair"** %229, align 8
  %1124 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %229, align 8
  %1125 = bitcast %"struct.std::__1::pair"* %1124 to i8*
  store %"class.std::__1::allocator.4"* %1119, %"class.std::__1::allocator.4"** %225, align 8
  store %"struct.std::__1::pair"* %1124, %"struct.std::__1::pair"** %226, align 8
  %1126 = bitcast %"struct.std::__1::__has_destroy"* %228 to %"struct.std::__1::integral_constant"*
  %1127 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %225, align 8
  %1128 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %226, align 8
  store %"class.std::__1::allocator.4"* %1127, %"class.std::__1::allocator.4"** %223, align 8
  store %"struct.std::__1::pair"* %1128, %"struct.std::__1::pair"** %224, align 8
  %1129 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %224, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEED1Ev(%"struct.std::__1::pair"* %1129) #2
  br label %1130

; <label>:1130:                                   ; preds = %1117, %1106
  %1131 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %232, align 8
  %1132 = icmp ne %"struct.std::__1::__hash_node"* %1131, null
  br i1 %1132, label %1133, label %1144

; <label>:1133:                                   ; preds = %1130
  %1134 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %1113, i32 0, i32 0
  %1135 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %1134, align 8
  %1136 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %232, align 8
  store %"class.std::__1::allocator.4"* %1135, %"class.std::__1::allocator.4"** %220, align 8
  store %"struct.std::__1::__hash_node"* %1136, %"struct.std::__1::__hash_node"** %221, align 8
  store i64 1, i64* %222, align 8
  %1137 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %220, align 8
  %1138 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %221, align 8
  %1139 = load i64, i64* %222, align 8
  store %"class.std::__1::allocator.4"* %1137, %"class.std::__1::allocator.4"** %217, align 8
  store %"struct.std::__1::__hash_node"* %1138, %"struct.std::__1::__hash_node"** %218, align 8
  store i64 %1139, i64* %219, align 8
  %1140 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %217, align 8
  %1141 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %218, align 8
  %1142 = bitcast %"struct.std::__1::__hash_node"* %1141 to i8*
  store i8* %1142, i8** %216, align 8
  %1143 = load i8*, i8** %216, align 8
  call void @_ZdlPv(i8* %1143) #14
  br label %1144

; <label>:1144:                                   ; preds = %1133, %1130
  br label %1145

; <label>:1145:                                   ; preds = %1144, %1066
  br label %1146

; <label>:1146:                                   ; preds = %706, %1145
  %1147 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %264, align 8
  store %"class.std::__1::__hash_iterator"* %272, %"class.std::__1::__hash_iterator"** %242, align 8
  store %"struct.std::__1::__hash_node_base"* %1147, %"struct.std::__1::__hash_node_base"** %243, align 8
  %1148 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %242, align 8
  %1149 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %243, align 8
  store %"class.std::__1::__hash_iterator"* %1148, %"class.std::__1::__hash_iterator"** %240, align 8
  store %"struct.std::__1::__hash_node_base"* %1149, %"struct.std::__1::__hash_node_base"** %241, align 8
  %1150 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %240, align 8
  %1151 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %1150, i32 0, i32 0
  %1152 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %241, align 8
  store %"struct.std::__1::__hash_node_base"* %1152, %"struct.std::__1::__hash_node_base"** %1151, align 8
  store %"struct.std::__1::pair.130"* %255, %"struct.std::__1::pair.130"** %249, align 8
  store %"class.std::__1::__hash_iterator"* %272, %"class.std::__1::__hash_iterator"** %250, align 8
  store i8* %263, i8** %251, align 8
  %1153 = load %"struct.std::__1::pair.130"*, %"struct.std::__1::pair.130"** %249, align 8
  %1154 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %250, align 8
  %1155 = load i8*, i8** %251, align 8
  store %"struct.std::__1::pair.130"* %1153, %"struct.std::__1::pair.130"** %246, align 8
  store %"class.std::__1::__hash_iterator"* %1154, %"class.std::__1::__hash_iterator"** %247, align 8
  store i8* %1155, i8** %248, align 8
  %1156 = load %"struct.std::__1::pair.130"*, %"struct.std::__1::pair.130"** %246, align 8
  %1157 = getelementptr inbounds %"struct.std::__1::pair.130", %"struct.std::__1::pair.130"* %1156, i32 0, i32 0
  %1158 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %247, align 8
  store %"class.std::__1::__hash_iterator"* %1158, %"class.std::__1::__hash_iterator"** %245, align 8
  %1159 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %245, align 8
  %1160 = bitcast %"class.std::__1::__hash_iterator"* %1157 to i8*
  %1161 = bitcast %"class.std::__1::__hash_iterator"* %1159 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1160, i8* %1161, i64 8, i32 8, i1 false) #2
  %1162 = getelementptr inbounds %"struct.std::__1::pair.130", %"struct.std::__1::pair.130"* %1156, i32 0, i32 1
  %1163 = load i8*, i8** %248, align 8
  store i8* %1163, i8** %244, align 8
  %1164 = load i8*, i8** %244, align 8
  %1165 = load i8, i8* %1164, align 1
  %1166 = trunc i8 %1165 to i1
  %1167 = zext i1 %1166 to i8
  store i8 %1167, i8* %1162, align 8
  %1168 = bitcast %"struct.std::__1::pair.130"* %255 to { %"struct.std::__1::__hash_node_base"*, i8 }*
  %1169 = load { %"struct.std::__1::__hash_node_base"*, i8 }, { %"struct.std::__1::__hash_node_base"*, i8 }* %1168, align 8
  %1170 = bitcast %"struct.std::__1::pair.130"* %311 to { %"struct.std::__1::__hash_node_base"*, i8 }*
  %1171 = getelementptr inbounds { %"struct.std::__1::__hash_node_base"*, i8 }, { %"struct.std::__1::__hash_node_base"*, i8 }* %1170, i32 0, i32 0
  %1172 = extractvalue { %"struct.std::__1::__hash_node_base"*, i8 } %1169, 0
  store %"struct.std::__1::__hash_node_base"* %1172, %"struct.std::__1::__hash_node_base"** %1171, align 8
  %1173 = getelementptr inbounds { %"struct.std::__1::__hash_node_base"*, i8 }, { %"struct.std::__1::__hash_node_base"*, i8 }* %1170, i32 0, i32 1
  %1174 = extractvalue { %"struct.std::__1::__hash_node_base"*, i8 } %1169, 1
  store i8 %1174, i8* %1173, align 8
  %1175 = getelementptr inbounds %"struct.std::__1::pair.130", %"struct.std::__1::pair.130"* %311, i32 0, i32 0
  store %"class.std::__1::__hash_iterator"* %1175, %"class.std::__1::__hash_iterator"** %278, align 8
  %1176 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %278, align 8
  %1177 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %1176, i32 0, i32 0
  %1178 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %1177, align 8
  store %"struct.std::__1::__hash_node_base"* %1178, %"struct.std::__1::__hash_node_base"** %277, align 8
  %1179 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %277, align 8
  store %"struct.std::__1::__hash_node_base"* %1179, %"struct.std::__1::__hash_node_base"** %276, align 8
  %1180 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %276, align 8
  store %"struct.std::__1::__hash_node_base"* %1180, %"struct.std::__1::__hash_node_base"** %275, align 8
  %1181 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %275, align 8
  %1182 = bitcast %"struct.std::__1::__hash_node_base"* %1181 to i8*
  %1183 = bitcast %"struct.std::__1::__hash_node_base"* %1181 to %"struct.std::__1::__hash_node"*
  %1184 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %1183, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %1184, %"union.std::__1::__hash_value_type"** %274, align 8
  %1185 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %274, align 8
  store %"union.std::__1::__hash_value_type"* %1185, %"union.std::__1::__hash_value_type"** %273, align 8
  %1186 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %273, align 8
  %1187 = bitcast %"union.std::__1::__hash_value_type"* %1186 to i8*
  %1188 = bitcast %"union.std::__1::__hash_value_type"* %1186 to %"struct.std::__1::pair"*
  %1189 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %1188, i32 0, i32 1
  ret %"class.llvm::StructType"** %1189
}

declare void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32, %"class.std::__1::basic_string"*, %class.AstNode*) #3

declare void @_ZNK13TypeSpecifier9to_stringEv(%"class.std::__1::basic_string"* sret, %struct.TypeSpecifier*) #3

; Function Attrs: nounwind
declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"*) unnamed_addr #4

; Function Attrs: ssp uwtable
define %"class.llvm::PointerType"* @_ZN6CGType10getPtrTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216)) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %4 = alloca %"class.std::__1::basic_string"*, align 8
  %5 = alloca [3 x i64]*, align 8
  %6 = alloca i32, align 4
  %7 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %8 = alloca %"class.std::__1::basic_string"*, align 8
  %9 = alloca %"class.std::__1::basic_string"*, align 8
  %10 = alloca %"class.std::__1::basic_string"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca i8*, align 8
  %14 = alloca %"class.std::__1::basic_string"*, align 8
  %15 = alloca %"class.llvm::PointerType"*, align 8
  %16 = alloca %struct.TypeSpecifier*, align 8
  %17 = alloca %"class.std::__1::basic_string", align 8
  %18 = alloca %"class.std::__1::basic_string", align 8
  %19 = alloca i8*
  %20 = alloca i32
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %16, align 8
  %21 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %16, align 8
  %22 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %21, i32 0, i32 0
  %23 = load i32, i32* %22, align 8
  switch i32 %23, label %43 [
    i32 0, label %24
    i32 1, label %26
    i32 2, label %28
    i32 3, label %30
    i32 5, label %32
    i32 6, label %34
    i32 7, label %36
  ]

; <label>:24:                                     ; preds = %1
  %25 = call %"class.llvm::PointerType"* @_ZN4llvm4Type13getInt32PtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, i32 0)
  store %"class.llvm::PointerType"* %25, %"class.llvm::PointerType"** %15, align 8
  br label %83

; <label>:26:                                     ; preds = %1
  %27 = call %"class.llvm::PointerType"* @_ZN4llvm4Type13getInt64PtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, i32 0)
  store %"class.llvm::PointerType"* %27, %"class.llvm::PointerType"** %15, align 8
  br label %83

; <label>:28:                                     ; preds = %1
  %29 = call %"class.llvm::PointerType"* @_ZN4llvm4Type13getFloatPtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, i32 0)
  store %"class.llvm::PointerType"* %29, %"class.llvm::PointerType"** %15, align 8
  br label %83

; <label>:30:                                     ; preds = %1
  %31 = call %"class.llvm::PointerType"* @_ZN4llvm4Type14getDoublePtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, i32 0)
  store %"class.llvm::PointerType"* %31, %"class.llvm::PointerType"** %15, align 8
  br label %83

; <label>:32:                                     ; preds = %1
  %33 = call %"class.llvm::PointerType"* @_ZN4llvm4Type12getInt1PtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, i32 0)
  store %"class.llvm::PointerType"* %33, %"class.llvm::PointerType"** %15, align 8
  br label %83

; <label>:34:                                     ; preds = %1
  %35 = call %"class.llvm::PointerType"* @_ZN4llvm4Type12getInt8PtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, i32 0)
  store %"class.llvm::PointerType"* %35, %"class.llvm::PointerType"** %15, align 8
  br label %83

; <label>:36:                                     ; preds = %1
  %37 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %16, align 8
  %38 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %37, i32 0, i32 1
  %39 = call dereferenceable(8) %"class.llvm::StructType"** @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeENS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S9_EEEEEixERSF_(%"class.std::__1::unordered_map"* @_ZN6CGType14StructureTypesE, %"class.std::__1::basic_string"* dereferenceable(24) %38)
  %40 = load %"class.llvm::StructType"*, %"class.llvm::StructType"** %39, align 8
  %41 = bitcast %"class.llvm::StructType"* %40 to %"class.llvm::Type"*
  %42 = call %"class.llvm::PointerType"* @_ZNK4llvm4Type12getPointerToEj(%"class.llvm::Type"* %41, i32 0)
  store %"class.llvm::PointerType"* %42, %"class.llvm::PointerType"** %15, align 8
  br label %83

; <label>:43:                                     ; preds = %1
  %44 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %16, align 8
  call void @_ZNK13TypeSpecifier9to_stringEv(%"class.std::__1::basic_string"* sret %18, %struct.TypeSpecifier* %44)
  store i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str, i32 0, i32 0), i8** %13, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %18, %"class.std::__1::basic_string"** %14, align 8, !noalias !5
  %45 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !5
  %46 = load i8*, i8** %13, align 8, !noalias !5
  %47 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %45, i64 0, i8* %46)
          to label %48 unwind label %85

; <label>:48:                                     ; preds = %43
  store %"class.std::__1::basic_string"* %47, %"class.std::__1::basic_string"** %12, align 8, !noalias !5
  %49 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %17, %"class.std::__1::basic_string"** %10, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %49, %"class.std::__1::basic_string"** %11, align 8, !noalias !5
  %50 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !5
  %51 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %50, %"class.std::__1::basic_string"** %8, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %51, %"class.std::__1::basic_string"** %9, align 8, !noalias !5
  %52 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %8, align 8, !noalias !5
  %53 = bitcast %"class.std::__1::basic_string"* %52 to %"class.std::__1::__basic_string_common"*
  %54 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %52, i32 0, i32 0
  %55 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %9, align 8, !noalias !5
  %56 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %55, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %56, %"class.std::__1::__compressed_pair.36"** %7, align 8, !noalias !5
  %57 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %7, align 8, !noalias !5
  %58 = bitcast %"class.std::__1::__compressed_pair.36"* %54 to i8*
  %59 = bitcast %"class.std::__1::__compressed_pair.36"* %57 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %58, i8* %59, i64 24, i32 8, i1 false) #2
  %60 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %9, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %60, %"class.std::__1::basic_string"** %4, align 8, !noalias !5
  %61 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %4, align 8, !noalias !5
  %62 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %61, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %62, %"class.std::__1::__compressed_pair.36"** %3, align 8, !noalias !5
  %63 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %3, align 8, !noalias !5
  %64 = bitcast %"class.std::__1::__compressed_pair.36"* %63 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %64, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %2, align 8, !noalias !5
  %65 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %2, align 8, !noalias !5
  %66 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %65, i32 0, i32 0
  %67 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %66, i32 0, i32 0
  %68 = bitcast %union.anon* %67 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %69 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %68, i32 0, i32 0
  store [3 x i64]* %69, [3 x i64]** %5, align 8, !noalias !5
  store i32 0, i32* %6, align 4, !noalias !5
  br label %70

; <label>:70:                                     ; preds = %73, %48
  %71 = load i32, i32* %6, align 4, !noalias !5
  %72 = icmp ult i32 %71, 3
  br i1 %72, label %73, label %80

; <label>:73:                                     ; preds = %70
  %74 = load i32, i32* %6, align 4, !noalias !5
  %75 = zext i32 %74 to i64
  %76 = load [3 x i64]*, [3 x i64]** %5, align 8, !noalias !5
  %77 = getelementptr inbounds [3 x i64], [3 x i64]* %76, i64 0, i64 %75
  store i64 0, i64* %77, align 8
  %78 = load i32, i32* %6, align 4, !noalias !5
  %79 = add i32 %78, 1
  store i32 %79, i32* %6, align 4, !noalias !5
  br label %70

; <label>:80:                                     ; preds = %70
  br label %81

; <label>:81:                                     ; preds = %80
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 7, %"class.std::__1::basic_string"* %17, %class.AstNode* null)
          to label %82 unwind label %89

; <label>:82:                                     ; preds = %81
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %17) #2
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %18) #2
  br label %83

; <label>:83:                                     ; preds = %24, %26, %28, %30, %32, %34, %36, %82
  %84 = load %"class.llvm::PointerType"*, %"class.llvm::PointerType"** %15, align 8
  ret %"class.llvm::PointerType"* %84

; <label>:85:                                     ; preds = %43
  %86 = landingpad { i8*, i32 }
          cleanup
  %87 = extractvalue { i8*, i32 } %86, 0
  store i8* %87, i8** %19, align 8
  %88 = extractvalue { i8*, i32 } %86, 1
  store i32 %88, i32* %20, align 4
  br label %93

; <label>:89:                                     ; preds = %81
  %90 = landingpad { i8*, i32 }
          cleanup
  %91 = extractvalue { i8*, i32 } %90, 0
  store i8* %91, i8** %19, align 8
  %92 = extractvalue { i8*, i32 } %90, 1
  store i32 %92, i32* %20, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %17) #2
  br label %93

; <label>:93:                                     ; preds = %89, %85
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %18) #2
  br label %94

; <label>:94:                                     ; preds = %93
  %95 = load i8*, i8** %19, align 8
  %96 = load i32, i32* %20, align 4
  %97 = insertvalue { i8*, i32 } undef, i8* %95, 0
  %98 = insertvalue { i8*, i32 } %97, i32 %96, 1
  resume { i8*, i32 } %98
}

declare %"class.llvm::PointerType"* @_ZN4llvm4Type13getInt32PtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8), i32) #3

declare %"class.llvm::PointerType"* @_ZN4llvm4Type13getInt64PtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8), i32) #3

declare %"class.llvm::PointerType"* @_ZN4llvm4Type13getFloatPtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8), i32) #3

declare %"class.llvm::PointerType"* @_ZN4llvm4Type14getDoublePtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8), i32) #3

declare %"class.llvm::PointerType"* @_ZN4llvm4Type12getInt1PtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8), i32) #3

; Function Attrs: nounwind ssp uwtable
define zeroext i16 @_ZN6CGType12getAlignmentER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216)) #5 align 2 {
  %2 = alloca i16, align 2
  %3 = alloca %struct.TypeSpecifier*, align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %3, align 8
  %4 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  %5 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %4, i32 0, i32 0
  %6 = load i32, i32* %5, align 8
  %7 = icmp eq i32 %6, 4
  br i1 %7, label %8, label %9

; <label>:8:                                      ; preds = %1
  store i16 1, i16* %2, align 2
  br label %28

; <label>:9:                                      ; preds = %1
  %10 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  %11 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %10, i32 0, i32 6
  %12 = load i8, i8* %11, align 1
  %13 = trunc i8 %12 to i1
  br i1 %13, label %14, label %19

; <label>:14:                                     ; preds = %9
  %15 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  %16 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %15, i32 0, i32 19
  %17 = load i8, i8* %16, align 4
  %18 = trunc i8 %17 to i1
  br i1 %18, label %19, label %20

; <label>:19:                                     ; preds = %14, %9
  store i16 8, i16* %2, align 2
  br label %28

; <label>:20:                                     ; preds = %14
  %21 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  %22 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %21, i32 0, i32 0
  %23 = load i32, i32* %22, align 8
  switch i32 %23, label %27 [
    i32 0, label %24
    i32 2, label %24
    i32 1, label %25
    i32 3, label %25
    i32 6, label %26
    i32 5, label %26
  ]

; <label>:24:                                     ; preds = %20, %20
  store i16 4, i16* %2, align 2
  br label %28

; <label>:25:                                     ; preds = %20, %20
  store i16 8, i16* %2, align 2
  br label %28

; <label>:26:                                     ; preds = %20, %20
  store i16 1, i16* %2, align 2
  br label %28

; <label>:27:                                     ; preds = %20
  store i16 4, i16* %2, align 2
  br label %28

; <label>:28:                                     ; preds = %27, %26, %25, %24, %19, %8
  %29 = load i16, i16* %2, align 2
  ret i16 %29
}

; Function Attrs: ssp uwtable
define %"class.llvm::Constant"* @_ZN6CGType14getConstantValER7Variant(%struct.Variant* dereferenceable(264)) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %4 = alloca %"class.std::__1::basic_string"*, align 8
  %5 = alloca [3 x i64]*, align 8
  %6 = alloca i32, align 4
  %7 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %8 = alloca %"class.std::__1::basic_string"*, align 8
  %9 = alloca %"class.std::__1::basic_string"*, align 8
  %10 = alloca %"class.std::__1::basic_string"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca i8*, align 8
  %14 = alloca %"class.std::__1::basic_string"*, align 8
  %15 = alloca %"class.llvm::Constant"*, align 8
  %16 = alloca %struct.Variant*, align 8
  %17 = alloca %"class.llvm::APInt", align 8
  %18 = alloca i8*
  %19 = alloca i32
  %20 = alloca %"class.llvm::APInt", align 8
  %21 = alloca %"class.llvm::APInt", align 8
  %22 = alloca %"class.llvm::APInt", align 8
  %23 = alloca %"class.llvm::APFloat", align 8
  %24 = alloca %"class.llvm::APFloat", align 8
  %25 = alloca %"class.std::__1::basic_string", align 8
  %26 = alloca %"class.std::__1::basic_string", align 8
  store %struct.Variant* %0, %struct.Variant** %16, align 8
  %27 = load %struct.Variant*, %struct.Variant** %16, align 8
  %28 = call dereferenceable(216) %struct.TypeSpecifier* @_ZN7Variant8get_typeEv(%struct.Variant* %27)
  %29 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %28, i32 0, i32 0
  %30 = load i32, i32* %29, align 8
  switch i32 %30, label %108 [
    i32 0, label %31
    i32 1, label %52
    i32 5, label %62
    i32 6, label %73
    i32 2, label %84
    i32 3, label %94
    i32 9, label %104
  ]

; <label>:31:                                     ; preds = %1
  %32 = load %struct.Variant*, %struct.Variant** %16, align 8
  %33 = call dereferenceable(216) %struct.TypeSpecifier* @_ZN7Variant8get_typeEv(%struct.Variant* %32)
  %34 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %33, i32 0, i32 8
  %35 = load i32, i32* %34, align 8
  %36 = load %struct.Variant*, %struct.Variant** %16, align 8
  %37 = call i32 @_ZN7Variant3getIiEET_v(%struct.Variant* %36)
  %38 = sext i32 %37 to i64
  %39 = load %struct.Variant*, %struct.Variant** %16, align 8
  %40 = call dereferenceable(216) %struct.TypeSpecifier* @_ZN7Variant8get_typeEv(%struct.Variant* %39)
  %41 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %40, i32 0, i32 9
  %42 = load i8, i8* %41, align 4
  %43 = trunc i8 %42 to i1
  %44 = xor i1 %43, true
  call void @_ZN4llvm5APIntC1Ejyb(%"class.llvm::APInt"* %17, i32 %35, i64 %38, i1 zeroext %44)
  %45 = invoke %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt3getERNS_11LLVMContextERKNS_5APIntE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, %"class.llvm::APInt"* dereferenceable(16) %17)
          to label %46 unwind label %48

; <label>:46:                                     ; preds = %31
  %47 = bitcast %"class.llvm::ConstantInt"* %45 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %47, %"class.llvm::Constant"** %15, align 8
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %17) #2
  br label %149

; <label>:48:                                     ; preds = %31
  %49 = landingpad { i8*, i32 }
          cleanup
  %50 = extractvalue { i8*, i32 } %49, 0
  store i8* %50, i8** %18, align 8
  %51 = extractvalue { i8*, i32 } %49, 1
  store i32 %51, i32* %19, align 4
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %17) #2
  br label %160

; <label>:52:                                     ; preds = %1
  %53 = load %struct.Variant*, %struct.Variant** %16, align 8
  %54 = call i64 @_ZN7Variant3getIlEET_v(%struct.Variant* %53)
  call void @_ZN4llvm5APIntC1Ejyb(%"class.llvm::APInt"* %20, i32 64, i64 %54, i1 zeroext true)
  %55 = invoke %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt3getERNS_11LLVMContextERKNS_5APIntE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, %"class.llvm::APInt"* dereferenceable(16) %20)
          to label %56 unwind label %58

; <label>:56:                                     ; preds = %52
  %57 = bitcast %"class.llvm::ConstantInt"* %55 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %57, %"class.llvm::Constant"** %15, align 8
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %20) #2
  br label %149

; <label>:58:                                     ; preds = %52
  %59 = landingpad { i8*, i32 }
          cleanup
  %60 = extractvalue { i8*, i32 } %59, 0
  store i8* %60, i8** %18, align 8
  %61 = extractvalue { i8*, i32 } %59, 1
  store i32 %61, i32* %19, align 4
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %20) #2
  br label %160

; <label>:62:                                     ; preds = %1
  %63 = load %struct.Variant*, %struct.Variant** %16, align 8
  %64 = call zeroext i1 @_ZN7Variant3getIbEET_v(%struct.Variant* %63)
  %65 = zext i1 %64 to i64
  call void @_ZN4llvm5APIntC1Ejyb(%"class.llvm::APInt"* %21, i32 1, i64 %65, i1 zeroext true)
  %66 = invoke %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt3getERNS_11LLVMContextERKNS_5APIntE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, %"class.llvm::APInt"* dereferenceable(16) %21)
          to label %67 unwind label %69

; <label>:67:                                     ; preds = %62
  %68 = bitcast %"class.llvm::ConstantInt"* %66 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %68, %"class.llvm::Constant"** %15, align 8
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %21) #2
  br label %149

; <label>:69:                                     ; preds = %62
  %70 = landingpad { i8*, i32 }
          cleanup
  %71 = extractvalue { i8*, i32 } %70, 0
  store i8* %71, i8** %18, align 8
  %72 = extractvalue { i8*, i32 } %70, 1
  store i32 %72, i32* %19, align 4
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %21) #2
  br label %160

; <label>:73:                                     ; preds = %1
  %74 = load %struct.Variant*, %struct.Variant** %16, align 8
  %75 = call signext i8 @_ZN7Variant3getIcEET_v(%struct.Variant* %74)
  %76 = sext i8 %75 to i64
  call void @_ZN4llvm5APIntC1Ejyb(%"class.llvm::APInt"* %22, i32 8, i64 %76, i1 zeroext true)
  %77 = invoke %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt3getERNS_11LLVMContextERKNS_5APIntE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, %"class.llvm::APInt"* dereferenceable(16) %22)
          to label %78 unwind label %80

; <label>:78:                                     ; preds = %73
  %79 = bitcast %"class.llvm::ConstantInt"* %77 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %79, %"class.llvm::Constant"** %15, align 8
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %22) #2
  br label %149

; <label>:80:                                     ; preds = %73
  %81 = landingpad { i8*, i32 }
          cleanup
  %82 = extractvalue { i8*, i32 } %81, 0
  store i8* %82, i8** %18, align 8
  %83 = extractvalue { i8*, i32 } %81, 1
  store i32 %83, i32* %19, align 4
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %22) #2
  br label %160

; <label>:84:                                     ; preds = %1
  %85 = load %struct.Variant*, %struct.Variant** %16, align 8
  %86 = call float @_ZN7Variant3getIfEET_v(%struct.Variant* %85)
  call void @_ZN4llvm7APFloatC1Ef(%"class.llvm::APFloat"* %23, float %86)
  %87 = invoke %"class.llvm::ConstantFP"* @_ZN4llvm10ConstantFP3getERNS_11LLVMContextERKNS_7APFloatE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, %"class.llvm::APFloat"* dereferenceable(32) %23)
          to label %88 unwind label %90

; <label>:88:                                     ; preds = %84
  %89 = bitcast %"class.llvm::ConstantFP"* %87 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %89, %"class.llvm::Constant"** %15, align 8
  call void @_ZN4llvm7APFloatD1Ev(%"class.llvm::APFloat"* %23) #2
  br label %149

; <label>:90:                                     ; preds = %84
  %91 = landingpad { i8*, i32 }
          cleanup
  %92 = extractvalue { i8*, i32 } %91, 0
  store i8* %92, i8** %18, align 8
  %93 = extractvalue { i8*, i32 } %91, 1
  store i32 %93, i32* %19, align 4
  call void @_ZN4llvm7APFloatD1Ev(%"class.llvm::APFloat"* %23) #2
  br label %160

; <label>:94:                                     ; preds = %1
  %95 = load %struct.Variant*, %struct.Variant** %16, align 8
  %96 = call double @_ZN7Variant3getIdEET_v(%struct.Variant* %95)
  call void @_ZN4llvm7APFloatC1Ed(%"class.llvm::APFloat"* %24, double %96)
  %97 = invoke %"class.llvm::ConstantFP"* @_ZN4llvm10ConstantFP3getERNS_11LLVMContextERKNS_7APFloatE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, %"class.llvm::APFloat"* dereferenceable(32) %24)
          to label %98 unwind label %100

; <label>:98:                                     ; preds = %94
  %99 = bitcast %"class.llvm::ConstantFP"* %97 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %99, %"class.llvm::Constant"** %15, align 8
  call void @_ZN4llvm7APFloatD1Ev(%"class.llvm::APFloat"* %24) #2
  br label %149

; <label>:100:                                    ; preds = %94
  %101 = landingpad { i8*, i32 }
          cleanup
  %102 = extractvalue { i8*, i32 } %101, 0
  store i8* %102, i8** %18, align 8
  %103 = extractvalue { i8*, i32 } %101, 1
  store i32 %103, i32* %19, align 4
  call void @_ZN4llvm7APFloatD1Ev(%"class.llvm::APFloat"* %24) #2
  br label %160

; <label>:104:                                    ; preds = %1
  %105 = call %"class.llvm::PointerType"* @_ZN4llvm4Type12getInt8PtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, i32 0)
  %106 = call %"class.llvm::ConstantPointerNull"* @_ZN4llvm19ConstantPointerNull3getEPNS_11PointerTypeE(%"class.llvm::PointerType"* %105)
  %107 = bitcast %"class.llvm::ConstantPointerNull"* %106 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %107, %"class.llvm::Constant"** %15, align 8
  br label %149

; <label>:108:                                    ; preds = %1
  %109 = load %struct.Variant*, %struct.Variant** %16, align 8
  %110 = call dereferenceable(216) %struct.TypeSpecifier* @_ZN7Variant8get_typeEv(%struct.Variant* %109)
  call void @_ZNK13TypeSpecifier9to_stringEv(%"class.std::__1::basic_string"* sret %26, %struct.TypeSpecifier* %110)
  store i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.2, i32 0, i32 0), i8** %13, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %26, %"class.std::__1::basic_string"** %14, align 8, !noalias !8
  %111 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !8
  %112 = load i8*, i8** %13, align 8, !noalias !8
  %113 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %111, i64 0, i8* %112)
          to label %114 unwind label %151

; <label>:114:                                    ; preds = %108
  store %"class.std::__1::basic_string"* %113, %"class.std::__1::basic_string"** %12, align 8, !noalias !8
  %115 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %25, %"class.std::__1::basic_string"** %10, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %115, %"class.std::__1::basic_string"** %11, align 8, !noalias !8
  %116 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !8
  %117 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %116, %"class.std::__1::basic_string"** %8, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %117, %"class.std::__1::basic_string"** %9, align 8, !noalias !8
  %118 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %8, align 8, !noalias !8
  %119 = bitcast %"class.std::__1::basic_string"* %118 to %"class.std::__1::__basic_string_common"*
  %120 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %118, i32 0, i32 0
  %121 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %9, align 8, !noalias !8
  %122 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %121, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %122, %"class.std::__1::__compressed_pair.36"** %7, align 8, !noalias !8
  %123 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %7, align 8, !noalias !8
  %124 = bitcast %"class.std::__1::__compressed_pair.36"* %120 to i8*
  %125 = bitcast %"class.std::__1::__compressed_pair.36"* %123 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %124, i8* %125, i64 24, i32 8, i1 false) #2
  %126 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %9, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %126, %"class.std::__1::basic_string"** %4, align 8, !noalias !8
  %127 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %4, align 8, !noalias !8
  %128 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %127, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %128, %"class.std::__1::__compressed_pair.36"** %3, align 8, !noalias !8
  %129 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %3, align 8, !noalias !8
  %130 = bitcast %"class.std::__1::__compressed_pair.36"* %129 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %130, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %2, align 8, !noalias !8
  %131 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %2, align 8, !noalias !8
  %132 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %131, i32 0, i32 0
  %133 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %132, i32 0, i32 0
  %134 = bitcast %union.anon* %133 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %135 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %134, i32 0, i32 0
  store [3 x i64]* %135, [3 x i64]** %5, align 8, !noalias !8
  store i32 0, i32* %6, align 4, !noalias !8
  br label %136

; <label>:136:                                    ; preds = %139, %114
  %137 = load i32, i32* %6, align 4, !noalias !8
  %138 = icmp ult i32 %137, 3
  br i1 %138, label %139, label %146

; <label>:139:                                    ; preds = %136
  %140 = load i32, i32* %6, align 4, !noalias !8
  %141 = zext i32 %140 to i64
  %142 = load [3 x i64]*, [3 x i64]** %5, align 8, !noalias !8
  %143 = getelementptr inbounds [3 x i64], [3 x i64]* %142, i64 0, i64 %141
  store i64 0, i64* %143, align 8
  %144 = load i32, i32* %6, align 4, !noalias !8
  %145 = add i32 %144, 1
  store i32 %145, i32* %6, align 4, !noalias !8
  br label %136

; <label>:146:                                    ; preds = %136
  br label %147

; <label>:147:                                    ; preds = %146
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 7, %"class.std::__1::basic_string"* %25, %class.AstNode* null)
          to label %148 unwind label %155

; <label>:148:                                    ; preds = %147
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %25) #2
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %26) #2
  br label %149

; <label>:149:                                    ; preds = %46, %56, %67, %78, %88, %98, %104, %148
  %150 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %15, align 8
  ret %"class.llvm::Constant"* %150

; <label>:151:                                    ; preds = %108
  %152 = landingpad { i8*, i32 }
          cleanup
  %153 = extractvalue { i8*, i32 } %152, 0
  store i8* %153, i8** %18, align 8
  %154 = extractvalue { i8*, i32 } %152, 1
  store i32 %154, i32* %19, align 4
  br label %159

; <label>:155:                                    ; preds = %147
  %156 = landingpad { i8*, i32 }
          cleanup
  %157 = extractvalue { i8*, i32 } %156, 0
  store i8* %157, i8** %18, align 8
  %158 = extractvalue { i8*, i32 } %156, 1
  store i32 %158, i32* %19, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %25) #2
  br label %159

; <label>:159:                                    ; preds = %155, %151
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %26) #2
  br label %160

; <label>:160:                                    ; preds = %159, %100, %90, %80, %69, %58, %48
  %161 = load i8*, i8** %18, align 8
  %162 = load i32, i32* %19, align 4
  %163 = insertvalue { i8*, i32 } undef, i8* %161, 0
  %164 = insertvalue { i8*, i32 } %163, i32 %162, 1
  resume { i8*, i32 } %164
}

declare dereferenceable(216) %struct.TypeSpecifier* @_ZN7Variant8get_typeEv(%struct.Variant*) #3

declare %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt3getERNS_11LLVMContextERKNS_5APIntE(%"class.llvm::LLVMContext"* dereferenceable(8), %"class.llvm::APInt"* dereferenceable(16)) #3

declare i32 @_ZN7Variant3getIiEET_v(%struct.Variant*) #3

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm5APIntC1Ejyb(%"class.llvm::APInt"*, i32, i64, i1 zeroext) unnamed_addr #0 align 2 {
  %5 = alloca %"class.llvm::APInt"*, align 8
  %6 = alloca i32, align 4
  %7 = alloca i64, align 8
  %8 = alloca i8, align 1
  store %"class.llvm::APInt"* %0, %"class.llvm::APInt"** %5, align 8
  store i32 %1, i32* %6, align 4
  store i64 %2, i64* %7, align 8
  %9 = zext i1 %3 to i8
  store i8 %9, i8* %8, align 1
  %10 = load %"class.llvm::APInt"*, %"class.llvm::APInt"** %5, align 8
  %11 = load i32, i32* %6, align 4
  %12 = load i64, i64* %7, align 8
  %13 = load i8, i8* %8, align 1
  %14 = trunc i8 %13 to i1
  call void @_ZN4llvm5APIntC2Ejyb(%"class.llvm::APInt"* %10, i32 %11, i64 %12, i1 zeroext %14)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.llvm::APInt"*, align 8
  store %"class.llvm::APInt"* %0, %"class.llvm::APInt"** %2, align 8
  %3 = load %"class.llvm::APInt"*, %"class.llvm::APInt"** %2, align 8
  call void @_ZN4llvm5APIntD2Ev(%"class.llvm::APInt"* %3) #2
  ret void
}

declare i64 @_ZN7Variant3getIlEET_v(%struct.Variant*) #3

declare zeroext i1 @_ZN7Variant3getIbEET_v(%struct.Variant*) #3

declare signext i8 @_ZN7Variant3getIcEET_v(%struct.Variant*) #3

declare %"class.llvm::ConstantFP"* @_ZN4llvm10ConstantFP3getERNS_11LLVMContextERKNS_7APFloatE(%"class.llvm::LLVMContext"* dereferenceable(8), %"class.llvm::APFloat"* dereferenceable(32)) #3

declare float @_ZN7Variant3getIfEET_v(%struct.Variant*) #3

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm7APFloatC1Ef(%"class.llvm::APFloat"*, float) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::APFloat"*, align 8
  %4 = alloca float, align 4
  store %"class.llvm::APFloat"* %0, %"class.llvm::APFloat"** %3, align 8
  store float %1, float* %4, align 4
  %5 = load %"class.llvm::APFloat"*, %"class.llvm::APFloat"** %3, align 8
  %6 = load float, float* %4, align 4
  call void @_ZN4llvm7APFloatC2Ef(%"class.llvm::APFloat"* %5, float %6)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm7APFloatD1Ev(%"class.llvm::APFloat"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.llvm::APFloat"*, align 8
  store %"class.llvm::APFloat"* %0, %"class.llvm::APFloat"** %2, align 8
  %3 = load %"class.llvm::APFloat"*, %"class.llvm::APFloat"** %2, align 8
  call void @_ZN4llvm7APFloatD2Ev(%"class.llvm::APFloat"* %3) #2
  ret void
}

declare double @_ZN7Variant3getIdEET_v(%struct.Variant*) #3

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm7APFloatC1Ed(%"class.llvm::APFloat"*, double) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::APFloat"*, align 8
  %4 = alloca double, align 8
  store %"class.llvm::APFloat"* %0, %"class.llvm::APFloat"** %3, align 8
  store double %1, double* %4, align 8
  %5 = load %"class.llvm::APFloat"*, %"class.llvm::APFloat"** %3, align 8
  %6 = load double, double* %4, align 8
  call void @_ZN4llvm7APFloatC2Ed(%"class.llvm::APFloat"* %5, double %6)
  ret void
}

declare %"class.llvm::ConstantPointerNull"* @_ZN4llvm19ConstantPointerNull3getEPNS_11PointerTypeE(%"class.llvm::PointerType"*) #3

; Function Attrs: ssp uwtable
define %"class.llvm::Constant"* @_ZN6CGType13getDefaultValER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216)) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"class.llvm::Constant"*, align 8
  %3 = alloca %struct.TypeSpecifier*, align 8
  %4 = alloca %"class.llvm::IRBuilder"*, align 8
  %5 = alloca %"class.llvm::ArrayRef.121", align 8
  %6 = alloca %"class.llvm::APInt", align 8
  %7 = alloca i8*
  %8 = alloca i32
  %9 = alloca %"class.llvm::APInt", align 8
  %10 = alloca %"class.llvm::APInt", align 8
  %11 = alloca %"class.llvm::APInt", align 8
  %12 = alloca %"class.llvm::APFloat", align 8
  %13 = alloca %"class.llvm::APFloat", align 8
  %14 = alloca %"class.llvm::APInt", align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %3, align 8
  store %"class.llvm::IRBuilder"* @_ZN14CodeGenVisitor7BuilderE, %"class.llvm::IRBuilder"** %4, align 8
  %15 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  %16 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %15, i32 0, i32 4
  %17 = load i8, i8* %16, align 1
  %18 = trunc i8 %17 to i1
  br i1 %18, label %19, label %32

; <label>:19:                                     ; preds = %1
  %20 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  %21 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %20, i32 0, i32 25
  %22 = bitcast %union.anon.73* %21 to %struct.TypeSpecifier**
  %23 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %22, align 8
  %24 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %23)
  %25 = call %"class.llvm::ArrayType"* @_ZN4llvm9ArrayType3getEPNS_4TypeEy(%"class.llvm::Type"* %24, i64 0)
  call void @_ZN4llvm8ArrayRefIPNS_8ConstantEEC1Ev(%"class.llvm::ArrayRef.121"* %5)
  %26 = bitcast %"class.llvm::ArrayRef.121"* %5 to { %"class.llvm::Constant"**, i64 }*
  %27 = getelementptr inbounds { %"class.llvm::Constant"**, i64 }, { %"class.llvm::Constant"**, i64 }* %26, i32 0, i32 0
  %28 = load %"class.llvm::Constant"**, %"class.llvm::Constant"*** %27, align 8
  %29 = getelementptr inbounds { %"class.llvm::Constant"**, i64 }, { %"class.llvm::Constant"**, i64 }* %26, i32 0, i32 1
  %30 = load i64, i64* %29, align 8
  %31 = call %"class.llvm::Constant"* @_ZN4llvm13ConstantArray3getEPNS_9ArrayTypeENS_8ArrayRefIPNS_8ConstantEEE(%"class.llvm::ArrayType"* %25, %"class.llvm::Constant"** %28, i64 %30)
  store %"class.llvm::Constant"* %31, %"class.llvm::Constant"** %2, align 8
  br label %105

; <label>:32:                                     ; preds = %1
  %33 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  %34 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %33, i32 0, i32 0
  %35 = load i32, i32* %34, align 8
  switch i32 %35, label %97 [
    i32 0, label %36
    i32 1, label %52
    i32 5, label %60
    i32 6, label %68
    i32 2, label %76
    i32 3, label %84
    i32 9, label %92
  ]

; <label>:36:                                     ; preds = %32
  %37 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  %38 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %37, i32 0, i32 8
  %39 = load i32, i32* %38, align 8
  %40 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  %41 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %40, i32 0, i32 9
  %42 = load i8, i8* %41, align 4
  %43 = trunc i8 %42 to i1
  %44 = xor i1 %43, true
  call void @_ZN4llvm5APIntC1Ejyb(%"class.llvm::APInt"* %6, i32 %39, i64 0, i1 zeroext %44)
  %45 = invoke %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt3getERNS_11LLVMContextERKNS_5APIntE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, %"class.llvm::APInt"* dereferenceable(16) %6)
          to label %46 unwind label %48

; <label>:46:                                     ; preds = %36
  %47 = bitcast %"class.llvm::ConstantInt"* %45 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %47, %"class.llvm::Constant"** %2, align 8
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %6) #2
  br label %105

; <label>:48:                                     ; preds = %36
  %49 = landingpad { i8*, i32 }
          cleanup
  %50 = extractvalue { i8*, i32 } %49, 0
  store i8* %50, i8** %7, align 8
  %51 = extractvalue { i8*, i32 } %49, 1
  store i32 %51, i32* %8, align 4
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %6) #2
  br label %107

; <label>:52:                                     ; preds = %32
  call void @_ZN4llvm5APIntC1Ejyb(%"class.llvm::APInt"* %9, i32 64, i64 0, i1 zeroext true)
  %53 = invoke %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt3getERNS_11LLVMContextERKNS_5APIntE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, %"class.llvm::APInt"* dereferenceable(16) %9)
          to label %54 unwind label %56

; <label>:54:                                     ; preds = %52
  %55 = bitcast %"class.llvm::ConstantInt"* %53 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %55, %"class.llvm::Constant"** %2, align 8
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %9) #2
  br label %105

; <label>:56:                                     ; preds = %52
  %57 = landingpad { i8*, i32 }
          cleanup
  %58 = extractvalue { i8*, i32 } %57, 0
  store i8* %58, i8** %7, align 8
  %59 = extractvalue { i8*, i32 } %57, 1
  store i32 %59, i32* %8, align 4
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %9) #2
  br label %107

; <label>:60:                                     ; preds = %32
  call void @_ZN4llvm5APIntC1Ejyb(%"class.llvm::APInt"* %10, i32 1, i64 0, i1 zeroext true)
  %61 = invoke %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt3getERNS_11LLVMContextERKNS_5APIntE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, %"class.llvm::APInt"* dereferenceable(16) %10)
          to label %62 unwind label %64

; <label>:62:                                     ; preds = %60
  %63 = bitcast %"class.llvm::ConstantInt"* %61 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %63, %"class.llvm::Constant"** %2, align 8
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %10) #2
  br label %105

; <label>:64:                                     ; preds = %60
  %65 = landingpad { i8*, i32 }
          cleanup
  %66 = extractvalue { i8*, i32 } %65, 0
  store i8* %66, i8** %7, align 8
  %67 = extractvalue { i8*, i32 } %65, 1
  store i32 %67, i32* %8, align 4
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %10) #2
  br label %107

; <label>:68:                                     ; preds = %32
  call void @_ZN4llvm5APIntC1Ejyb(%"class.llvm::APInt"* %11, i32 8, i64 0, i1 zeroext true)
  %69 = invoke %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt3getERNS_11LLVMContextERKNS_5APIntE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, %"class.llvm::APInt"* dereferenceable(16) %11)
          to label %70 unwind label %72

; <label>:70:                                     ; preds = %68
  %71 = bitcast %"class.llvm::ConstantInt"* %69 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %71, %"class.llvm::Constant"** %2, align 8
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %11) #2
  br label %105

; <label>:72:                                     ; preds = %68
  %73 = landingpad { i8*, i32 }
          cleanup
  %74 = extractvalue { i8*, i32 } %73, 0
  store i8* %74, i8** %7, align 8
  %75 = extractvalue { i8*, i32 } %73, 1
  store i32 %75, i32* %8, align 4
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %11) #2
  br label %107

; <label>:76:                                     ; preds = %32
  call void @_ZN4llvm7APFloatC1Ef(%"class.llvm::APFloat"* %12, float 0.000000e+00)
  %77 = invoke %"class.llvm::ConstantFP"* @_ZN4llvm10ConstantFP3getERNS_11LLVMContextERKNS_7APFloatE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, %"class.llvm::APFloat"* dereferenceable(32) %12)
          to label %78 unwind label %80

; <label>:78:                                     ; preds = %76
  %79 = bitcast %"class.llvm::ConstantFP"* %77 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %79, %"class.llvm::Constant"** %2, align 8
  call void @_ZN4llvm7APFloatD1Ev(%"class.llvm::APFloat"* %12) #2
  br label %105

; <label>:80:                                     ; preds = %76
  %81 = landingpad { i8*, i32 }
          cleanup
  %82 = extractvalue { i8*, i32 } %81, 0
  store i8* %82, i8** %7, align 8
  %83 = extractvalue { i8*, i32 } %81, 1
  store i32 %83, i32* %8, align 4
  call void @_ZN4llvm7APFloatD1Ev(%"class.llvm::APFloat"* %12) #2
  br label %107

; <label>:84:                                     ; preds = %32
  call void @_ZN4llvm7APFloatC1Ed(%"class.llvm::APFloat"* %13, double 0.000000e+00)
  %85 = invoke %"class.llvm::ConstantFP"* @_ZN4llvm10ConstantFP3getERNS_11LLVMContextERKNS_7APFloatE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, %"class.llvm::APFloat"* dereferenceable(32) %13)
          to label %86 unwind label %88

; <label>:86:                                     ; preds = %84
  %87 = bitcast %"class.llvm::ConstantFP"* %85 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %87, %"class.llvm::Constant"** %2, align 8
  call void @_ZN4llvm7APFloatD1Ev(%"class.llvm::APFloat"* %13) #2
  br label %105

; <label>:88:                                     ; preds = %84
  %89 = landingpad { i8*, i32 }
          cleanup
  %90 = extractvalue { i8*, i32 } %89, 0
  store i8* %90, i8** %7, align 8
  %91 = extractvalue { i8*, i32 } %89, 1
  store i32 %91, i32* %8, align 4
  call void @_ZN4llvm7APFloatD1Ev(%"class.llvm::APFloat"* %13) #2
  br label %107

; <label>:92:                                     ; preds = %32
  %93 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  %94 = call %"class.llvm::PointerType"* @_ZN6CGType10getPtrTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %93)
  %95 = call %"class.llvm::ConstantPointerNull"* @_ZN4llvm19ConstantPointerNull3getEPNS_11PointerTypeE(%"class.llvm::PointerType"* %94)
  %96 = bitcast %"class.llvm::ConstantPointerNull"* %95 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %96, %"class.llvm::Constant"** %2, align 8
  br label %105

; <label>:97:                                     ; preds = %32
  call void @_ZN4llvm5APIntC1Ejyb(%"class.llvm::APInt"* %14, i32 32, i64 0, i1 zeroext true)
  %98 = invoke %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt3getERNS_11LLVMContextERKNS_5APIntE(%"class.llvm::LLVMContext"* dereferenceable(8) @_ZN14CodeGenVisitor7ContextE, %"class.llvm::APInt"* dereferenceable(16) %14)
          to label %99 unwind label %101

; <label>:99:                                     ; preds = %97
  %100 = bitcast %"class.llvm::ConstantInt"* %98 to %"class.llvm::Constant"*
  store %"class.llvm::Constant"* %100, %"class.llvm::Constant"** %2, align 8
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %14) #2
  br label %105

; <label>:101:                                    ; preds = %97
  %102 = landingpad { i8*, i32 }
          cleanup
  %103 = extractvalue { i8*, i32 } %102, 0
  store i8* %103, i8** %7, align 8
  %104 = extractvalue { i8*, i32 } %102, 1
  store i32 %104, i32* %8, align 4
  call void @_ZN4llvm5APIntD1Ev(%"class.llvm::APInt"* %14) #2
  br label %107

; <label>:105:                                    ; preds = %99, %92, %86, %78, %70, %62, %54, %46, %19
  %106 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %2, align 8
  ret %"class.llvm::Constant"* %106

; <label>:107:                                    ; preds = %101, %88, %80, %72, %64, %56, %48
  %108 = load i8*, i8** %7, align 8
  %109 = load i32, i32* %8, align 4
  %110 = insertvalue { i8*, i32 } undef, i8* %108, 0
  %111 = insertvalue { i8*, i32 } %110, i32 %109, 1
  resume { i8*, i32 } %111
}

declare %"class.llvm::Constant"* @_ZN4llvm13ConstantArray3getEPNS_9ArrayTypeENS_8ArrayRefIPNS_8ConstantEEE(%"class.llvm::ArrayType"*, %"class.llvm::Constant"**, i64) #3

declare %"class.llvm::ArrayType"* @_ZN4llvm9ArrayType3getEPNS_4TypeEy(%"class.llvm::Type"*, i64) #3

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm8ArrayRefIPNS_8ConstantEEC1Ev(%"class.llvm::ArrayRef.121"*) unnamed_addr #0 align 2 {
  %2 = alloca %"class.llvm::ArrayRef.121"*, align 8
  store %"class.llvm::ArrayRef.121"* %0, %"class.llvm::ArrayRef.121"** %2, align 8
  %3 = load %"class.llvm::ArrayRef.121"*, %"class.llvm::ArrayRef.121"** %2, align 8
  call void @_ZN4llvm8ArrayRefIPNS_8ConstantEEC2Ev(%"class.llvm::ArrayRef.121"* %3)
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeENS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S9_EEEEED2Ev(%"class.std::__1::unordered_map"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::unordered_map"*, align 8
  store %"class.std::__1::unordered_map"* %0, %"class.std::__1::unordered_map"** %2, align 8
  %3 = load %"class.std::__1::unordered_map"*, %"class.std::__1::unordered_map"** %2, align 8
  %4 = getelementptr inbounds %"class.std::__1::unordered_map", %"class.std::__1::unordered_map"* %3, i32 0, i32 0
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEED1Ev(%"class.std::__1::__hash_table"* %4) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEED1Ev(%"class.std::__1::__hash_table"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.std::__1::__hash_table"*, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %2, align 8
  %3 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %2, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEED2Ev(%"class.std::__1::__hash_table"* %3) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEED2Ev(%"class.std::__1::__hash_table"*) unnamed_addr #5 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %8 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca %"class.std::__1::allocator"*, align 8
  %11 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::allocator"*, align 8
  %14 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %15 = alloca i64, align 8
  %16 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %17 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %18 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %19 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %20 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %23 = alloca %"class.std::__1::unique_ptr"*, align 8
  %24 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %25 = alloca %"class.std::__1::unique_ptr"*, align 8
  %26 = alloca %"class.std::__1::unique_ptr"*, align 8
  %27 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %28 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %29 = alloca %"class.std::__1::__hash_table"*, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %29, align 8
  %30 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %29, align 8
  %31 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %30, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.2"* %31, %"class.std::__1::__compressed_pair.2"** %28, align 8
  %32 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %28, align 8
  %33 = bitcast %"class.std::__1::__compressed_pair.2"* %32 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %33, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %27, align 8
  %34 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %27, align 8
  %35 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %34, i32 0, i32 0
  %36 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %35, i32 0, i32 0
  %37 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %36, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE12__deallocateEPNS_16__hash_node_baseIPNS_11__hash_nodeISB_PvEEEE(%"class.std::__1::__hash_table"* %30, %"struct.std::__1::__hash_node_base"* %37) #2
  %38 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %30, i32 0, i32 0
  store %"class.std::__1::unique_ptr"* %38, %"class.std::__1::unique_ptr"** %26, align 8
  %39 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %26, align 8
  store %"class.std::__1::unique_ptr"* %39, %"class.std::__1::unique_ptr"** %25, align 8
  %40 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %25, align 8
  store %"class.std::__1::unique_ptr"* %40, %"class.std::__1::unique_ptr"** %23, align 8
  %41 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %23, align 8
  %42 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %42, %"class.std::__1::__compressed_pair"** %22, align 8
  %43 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8
  %44 = bitcast %"class.std::__1::__compressed_pair"* %43 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %44, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8
  %45 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8
  %46 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %45, i32 0, i32 0
  %47 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %46, align 8
  store %"struct.std::__1::__hash_node_base"** %47, %"struct.std::__1::__hash_node_base"*** %24, align 8
  %48 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %48, %"class.std::__1::__compressed_pair"** %5, align 8
  %49 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8
  %50 = bitcast %"class.std::__1::__compressed_pair"* %49 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %50, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8
  %51 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8
  %52 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %51, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"** null, %"struct.std::__1::__hash_node_base"*** %52, align 8
  %53 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %24, align 8
  %54 = icmp ne %"struct.std::__1::__hash_node_base"** %53, null
  br i1 %54, label %55, label %84

; <label>:55:                                     ; preds = %1
  %56 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %56, %"class.std::__1::__compressed_pair"** %3, align 8
  %57 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %3, align 8
  %58 = bitcast %"class.std::__1::__compressed_pair"* %57 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %58, %"class.std::__1::__libcpp_compressed_pair_imp"** %2, align 8
  %59 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %2, align 8
  %60 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %59, i32 0, i32 1
  %61 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %24, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %60, %"class.std::__1::__bucket_list_deallocator"** %19, align 8
  store %"struct.std::__1::__hash_node_base"** %61, %"struct.std::__1::__hash_node_base"*** %20, align 8
  %62 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %19, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %62, %"class.std::__1::__bucket_list_deallocator"** %18, align 8
  %63 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %18, align 8
  %64 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %63, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.0"* %64, %"class.std::__1::__compressed_pair.0"** %17, align 8
  %65 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %17, align 8
  %66 = bitcast %"class.std::__1::__compressed_pair.0"* %65 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %66, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %16, align 8
  %67 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %16, align 8
  %68 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.1"* %67 to %"class.std::__1::allocator"*
  %69 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %20, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %62, %"class.std::__1::__bucket_list_deallocator"** %8, align 8
  %70 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %8, align 8
  %71 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %70, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.0"* %71, %"class.std::__1::__compressed_pair.0"** %7, align 8
  %72 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %7, align 8
  %73 = bitcast %"class.std::__1::__compressed_pair.0"* %72 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %73, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %6, align 8
  %74 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %6, align 8
  %75 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.1", %"class.std::__1::__libcpp_compressed_pair_imp.1"* %74, i32 0, i32 0
  %76 = load i64, i64* %75, align 8
  store %"class.std::__1::allocator"* %68, %"class.std::__1::allocator"** %13, align 8
  store %"struct.std::__1::__hash_node_base"** %69, %"struct.std::__1::__hash_node_base"*** %14, align 8
  store i64 %76, i64* %15, align 8
  %77 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %13, align 8
  %78 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %14, align 8
  %79 = load i64, i64* %15, align 8
  store %"class.std::__1::allocator"* %77, %"class.std::__1::allocator"** %10, align 8
  store %"struct.std::__1::__hash_node_base"** %78, %"struct.std::__1::__hash_node_base"*** %11, align 8
  store i64 %79, i64* %12, align 8
  %80 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %10, align 8
  %81 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %11, align 8
  %82 = bitcast %"struct.std::__1::__hash_node_base"** %81 to i8*
  store i8* %82, i8** %9, align 8
  %83 = load i8*, i8** %9, align 8
  call void @_ZdlPv(i8* %83) #14
  br label %84

; <label>:84:                                     ; preds = %1, %55
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE12__deallocateEPNS_16__hash_node_baseIPNS_11__hash_nodeISB_PvEEEE(%"class.std::__1::__hash_table"*, %"struct.std::__1::__hash_node_base"*) #5 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::allocator.4"*, align 8
  %4 = alloca %"struct.std::__1::pair"*, align 8
  %5 = alloca %"class.std::__1::allocator.4"*, align 8
  %6 = alloca %"struct.std::__1::pair"*, align 8
  %7 = alloca %"struct.std::__1::integral_constant", align 1
  %8 = alloca %"struct.std::__1::__has_destroy", align 1
  %9 = alloca %"struct.std::__1::pair"*, align 8
  %10 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %11 = alloca i8*, align 8
  %12 = alloca %"class.std::__1::allocator.4"*, align 8
  %13 = alloca %"struct.std::__1::__hash_node"*, align 8
  %14 = alloca i64, align 8
  %15 = alloca %"class.std::__1::allocator.4"*, align 8
  %16 = alloca %"struct.std::__1::__hash_node"*, align 8
  %17 = alloca i64, align 8
  %18 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %19 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %20 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %23 = alloca %"class.std::__1::__hash_table"*, align 8
  %24 = alloca %"class.std::__1::__hash_table"*, align 8
  %25 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %26 = alloca %"class.std::__1::allocator.4"*, align 8
  %27 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %28 = alloca %"struct.std::__1::__hash_node"*, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %24, align 8
  store %"struct.std::__1::__hash_node_base"* %1, %"struct.std::__1::__hash_node_base"** %25, align 8
  %29 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %24, align 8
  store %"class.std::__1::__hash_table"* %29, %"class.std::__1::__hash_table"** %23, align 8
  %30 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %23, align 8
  %31 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %30, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.2"* %31, %"class.std::__1::__compressed_pair.2"** %22, align 8
  %32 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %22, align 8
  %33 = bitcast %"class.std::__1::__compressed_pair.2"* %32 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %33, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %21, align 8
  %34 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %21, align 8
  %35 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.3"* %34 to %"class.std::__1::allocator.4"*
  store %"class.std::__1::allocator.4"* %35, %"class.std::__1::allocator.4"** %26, align 8
  br label %36

; <label>:36:                                     ; preds = %61, %2
  %37 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %25, align 8
  %38 = icmp ne %"struct.std::__1::__hash_node_base"* %37, null
  br i1 %38, label %39, label %72

; <label>:39:                                     ; preds = %36
  %40 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %25, align 8
  %41 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %40, i32 0, i32 0
  %42 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %41, align 8
  store %"struct.std::__1::__hash_node_base"* %42, %"struct.std::__1::__hash_node_base"** %27, align 8
  %43 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %25, align 8
  store %"struct.std::__1::__hash_node_base"* %43, %"struct.std::__1::__hash_node_base"** %20, align 8
  %44 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %20, align 8
  store %"struct.std::__1::__hash_node_base"* %44, %"struct.std::__1::__hash_node_base"** %19, align 8
  %45 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %19, align 8
  store %"struct.std::__1::__hash_node_base"* %45, %"struct.std::__1::__hash_node_base"** %18, align 8
  %46 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %18, align 8
  %47 = bitcast %"struct.std::__1::__hash_node_base"* %46 to i8*
  %48 = bitcast %"struct.std::__1::__hash_node_base"* %46 to %"struct.std::__1::__hash_node"*
  store %"struct.std::__1::__hash_node"* %48, %"struct.std::__1::__hash_node"** %28, align 8
  %49 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %26, align 8
  %50 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %28, align 8
  %51 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %50, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %51, %"union.std::__1::__hash_value_type"** %10, align 8
  %52 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %10, align 8
  %53 = bitcast %"union.std::__1::__hash_value_type"* %52 to %"struct.std::__1::pair"*
  store %"struct.std::__1::pair"* %53, %"struct.std::__1::pair"** %9, align 8
  %54 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %9, align 8
  %55 = bitcast %"struct.std::__1::pair"* %54 to i8*
  br label %56

; <label>:56:                                     ; preds = %39
  store %"class.std::__1::allocator.4"* %49, %"class.std::__1::allocator.4"** %5, align 8
  store %"struct.std::__1::pair"* %54, %"struct.std::__1::pair"** %6, align 8
  %57 = bitcast %"struct.std::__1::__has_destroy"* %8 to %"struct.std::__1::integral_constant"*
  %58 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %5, align 8
  %59 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %6, align 8
  store %"class.std::__1::allocator.4"* %58, %"class.std::__1::allocator.4"** %3, align 8
  store %"struct.std::__1::pair"* %59, %"struct.std::__1::pair"** %4, align 8
  %60 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %4, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEED1Ev(%"struct.std::__1::pair"* %60) #2
  br label %61

; <label>:61:                                     ; preds = %56
  %62 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %26, align 8
  %63 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %28, align 8
  store %"class.std::__1::allocator.4"* %62, %"class.std::__1::allocator.4"** %15, align 8
  store %"struct.std::__1::__hash_node"* %63, %"struct.std::__1::__hash_node"** %16, align 8
  store i64 1, i64* %17, align 8
  %64 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %15, align 8
  %65 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %16, align 8
  %66 = load i64, i64* %17, align 8
  store %"class.std::__1::allocator.4"* %64, %"class.std::__1::allocator.4"** %12, align 8
  store %"struct.std::__1::__hash_node"* %65, %"struct.std::__1::__hash_node"** %13, align 8
  store i64 %66, i64* %14, align 8
  %67 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %12, align 8
  %68 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %13, align 8
  %69 = bitcast %"struct.std::__1::__hash_node"* %68 to i8*
  store i8* %69, i8** %11, align 8
  %70 = load i8*, i8** %11, align 8
  call void @_ZdlPv(i8* %70) #14
  %71 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %27, align 8
  store %"struct.std::__1::__hash_node_base"* %71, %"struct.std::__1::__hash_node_base"** %25, align 8
  br label %36

; <label>:72:                                     ; preds = %36
  ret void
                                                  ; No predecessors!
  %74 = landingpad { i8*, i32 }
          catch i8* null
  %75 = extractvalue { i8*, i32 } %74, 0
  call void @__clang_call_terminate(i8* %75) #15
  unreachable
}

; Function Attrs: noinline noreturn nounwind
define linkonce_odr hidden void @__clang_call_terminate(i8*) #6 {
  %2 = call i8* @__cxa_begin_catch(i8* %0) #2
  call void @_ZSt9terminatev() #15
  unreachable
}

declare i8* @__cxa_begin_catch(i8*)

declare void @_ZSt9terminatev()

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEED1Ev(%"struct.std::__1::pair"*) unnamed_addr #1 align 2 {
  %2 = alloca %"struct.std::__1::pair"*, align 8
  store %"struct.std::__1::pair"* %0, %"struct.std::__1::pair"** %2, align 8
  %3 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %2, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEED2Ev(%"struct.std::__1::pair"* %3) #2
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEED2Ev(%"struct.std::__1::pair"*) unnamed_addr #1 align 2 {
  %2 = alloca %"struct.std::__1::pair"*, align 8
  store %"struct.std::__1::pair"* %0, %"struct.std::__1::pair"** %2, align 8
  %3 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %2, align 8
  %4 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %3, i32 0, i32 0
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %4) #2
  ret void
}

; Function Attrs: nobuiltin nounwind
declare void @_ZdlPv(i8*) #7

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113unordered_mapIjPN4llvm10StructTypeENS_4hashIjEENS_8equal_toIjEENS_9allocatorINS_4pairIKjS3_EEEEED2Ev(%"class.std::__1::unordered_map.10"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::unordered_map.10"*, align 8
  store %"class.std::__1::unordered_map.10"* %0, %"class.std::__1::unordered_map.10"** %2, align 8
  %3 = load %"class.std::__1::unordered_map.10"*, %"class.std::__1::unordered_map.10"** %2, align 8
  %4 = getelementptr inbounds %"class.std::__1::unordered_map.10", %"class.std::__1::unordered_map.10"* %3, i32 0, i32 0
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeIjPN4llvm10StructTypeEEENS_22__unordered_map_hasherIjS5_NS_4hashIjEELb1EEENS_21__unordered_map_equalIjS5_NS_8equal_toIjEELb1EEENS_9allocatorIS5_EEED1Ev(%"class.std::__1::__hash_table.11"* %4) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeIjPN4llvm10StructTypeEEENS_22__unordered_map_hasherIjS5_NS_4hashIjEELb1EEENS_21__unordered_map_equalIjS5_NS_8equal_toIjEELb1EEENS_9allocatorIS5_EEED1Ev(%"class.std::__1::__hash_table.11"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.std::__1::__hash_table.11"*, align 8
  store %"class.std::__1::__hash_table.11"* %0, %"class.std::__1::__hash_table.11"** %2, align 8
  %3 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %2, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeIjPN4llvm10StructTypeEEENS_22__unordered_map_hasherIjS5_NS_4hashIjEELb1EEENS_21__unordered_map_equalIjS5_NS_8equal_toIjEELb1EEENS_9allocatorIS5_EEED2Ev(%"class.std::__1::__hash_table.11"* %3) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeIjPN4llvm10StructTypeEEENS_22__unordered_map_hasherIjS5_NS_4hashIjEELb1EEENS_21__unordered_map_equalIjS5_NS_8equal_toIjEELb1EEENS_9allocatorIS5_EEED2Ev(%"class.std::__1::__hash_table.11"*) unnamed_addr #5 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.18"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair.17"*, align 8
  %8 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca %"class.std::__1::allocator.19"*, align 8
  %11 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::allocator.19"*, align 8
  %14 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %15 = alloca i64, align 8
  %16 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.18"*, align 8
  %17 = alloca %"class.std::__1::__compressed_pair.17"*, align 8
  %18 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %19 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %20 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %23 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %24 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %25 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %26 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %27 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.22"*, align 8
  %28 = alloca %"class.std::__1::__compressed_pair.21"*, align 8
  %29 = alloca %"class.std::__1::__hash_table.11"*, align 8
  store %"class.std::__1::__hash_table.11"* %0, %"class.std::__1::__hash_table.11"** %29, align 8
  %30 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %29, align 8
  %31 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %30, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.21"* %31, %"class.std::__1::__compressed_pair.21"** %28, align 8
  %32 = load %"class.std::__1::__compressed_pair.21"*, %"class.std::__1::__compressed_pair.21"** %28, align 8
  %33 = bitcast %"class.std::__1::__compressed_pair.21"* %32 to %"class.std::__1::__libcpp_compressed_pair_imp.22"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.22"* %33, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %27, align 8
  %34 = load %"class.std::__1::__libcpp_compressed_pair_imp.22"*, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %27, align 8
  %35 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.22", %"class.std::__1::__libcpp_compressed_pair_imp.22"* %34, i32 0, i32 0
  %36 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %35, i32 0, i32 0
  %37 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %36, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeIjPN4llvm10StructTypeEEENS_22__unordered_map_hasherIjS5_NS_4hashIjEELb1EEENS_21__unordered_map_equalIjS5_NS_8equal_toIjEELb1EEENS_9allocatorIS5_EEE12__deallocateEPNS_16__hash_node_baseIPNS_11__hash_nodeIS5_PvEEEE(%"class.std::__1::__hash_table.11"* %30, %"struct.std::__1::__hash_node_base.15"* %37) #2
  %38 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %30, i32 0, i32 0
  store %"class.std::__1::unique_ptr.12"* %38, %"class.std::__1::unique_ptr.12"** %26, align 8
  %39 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %26, align 8
  store %"class.std::__1::unique_ptr.12"* %39, %"class.std::__1::unique_ptr.12"** %25, align 8
  %40 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %25, align 8
  store %"class.std::__1::unique_ptr.12"* %40, %"class.std::__1::unique_ptr.12"** %23, align 8
  %41 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %23, align 8
  %42 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %42, %"class.std::__1::__compressed_pair.13"** %22, align 8
  %43 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %22, align 8
  %44 = bitcast %"class.std::__1::__compressed_pair.13"* %43 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %44, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %21, align 8
  %45 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %21, align 8
  %46 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %45, i32 0, i32 0
  %47 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %46, align 8
  store %"struct.std::__1::__hash_node_base.15"** %47, %"struct.std::__1::__hash_node_base.15"*** %24, align 8
  %48 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %48, %"class.std::__1::__compressed_pair.13"** %5, align 8
  %49 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %5, align 8
  %50 = bitcast %"class.std::__1::__compressed_pair.13"* %49 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %50, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %4, align 8
  %51 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %4, align 8
  %52 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %51, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"** null, %"struct.std::__1::__hash_node_base.15"*** %52, align 8
  %53 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %24, align 8
  %54 = icmp ne %"struct.std::__1::__hash_node_base.15"** %53, null
  br i1 %54, label %55, label %84

; <label>:55:                                     ; preds = %1
  %56 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %56, %"class.std::__1::__compressed_pair.13"** %3, align 8
  %57 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %3, align 8
  %58 = bitcast %"class.std::__1::__compressed_pair.13"* %57 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %58, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %2, align 8
  %59 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %2, align 8
  %60 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %59, i32 0, i32 1
  %61 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %24, align 8
  store %"class.std::__1::__bucket_list_deallocator.16"* %60, %"class.std::__1::__bucket_list_deallocator.16"** %19, align 8
  store %"struct.std::__1::__hash_node_base.15"** %61, %"struct.std::__1::__hash_node_base.15"*** %20, align 8
  %62 = load %"class.std::__1::__bucket_list_deallocator.16"*, %"class.std::__1::__bucket_list_deallocator.16"** %19, align 8
  store %"class.std::__1::__bucket_list_deallocator.16"* %62, %"class.std::__1::__bucket_list_deallocator.16"** %18, align 8
  %63 = load %"class.std::__1::__bucket_list_deallocator.16"*, %"class.std::__1::__bucket_list_deallocator.16"** %18, align 8
  %64 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.16", %"class.std::__1::__bucket_list_deallocator.16"* %63, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.17"* %64, %"class.std::__1::__compressed_pair.17"** %17, align 8
  %65 = load %"class.std::__1::__compressed_pair.17"*, %"class.std::__1::__compressed_pair.17"** %17, align 8
  %66 = bitcast %"class.std::__1::__compressed_pair.17"* %65 to %"class.std::__1::__libcpp_compressed_pair_imp.18"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.18"* %66, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %16, align 8
  %67 = load %"class.std::__1::__libcpp_compressed_pair_imp.18"*, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %16, align 8
  %68 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.18"* %67 to %"class.std::__1::allocator.19"*
  %69 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %20, align 8
  store %"class.std::__1::__bucket_list_deallocator.16"* %62, %"class.std::__1::__bucket_list_deallocator.16"** %8, align 8
  %70 = load %"class.std::__1::__bucket_list_deallocator.16"*, %"class.std::__1::__bucket_list_deallocator.16"** %8, align 8
  %71 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.16", %"class.std::__1::__bucket_list_deallocator.16"* %70, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.17"* %71, %"class.std::__1::__compressed_pair.17"** %7, align 8
  %72 = load %"class.std::__1::__compressed_pair.17"*, %"class.std::__1::__compressed_pair.17"** %7, align 8
  %73 = bitcast %"class.std::__1::__compressed_pair.17"* %72 to %"class.std::__1::__libcpp_compressed_pair_imp.18"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.18"* %73, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %6, align 8
  %74 = load %"class.std::__1::__libcpp_compressed_pair_imp.18"*, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %6, align 8
  %75 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.18", %"class.std::__1::__libcpp_compressed_pair_imp.18"* %74, i32 0, i32 0
  %76 = load i64, i64* %75, align 8
  store %"class.std::__1::allocator.19"* %68, %"class.std::__1::allocator.19"** %13, align 8
  store %"struct.std::__1::__hash_node_base.15"** %69, %"struct.std::__1::__hash_node_base.15"*** %14, align 8
  store i64 %76, i64* %15, align 8
  %77 = load %"class.std::__1::allocator.19"*, %"class.std::__1::allocator.19"** %13, align 8
  %78 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %14, align 8
  %79 = load i64, i64* %15, align 8
  store %"class.std::__1::allocator.19"* %77, %"class.std::__1::allocator.19"** %10, align 8
  store %"struct.std::__1::__hash_node_base.15"** %78, %"struct.std::__1::__hash_node_base.15"*** %11, align 8
  store i64 %79, i64* %12, align 8
  %80 = load %"class.std::__1::allocator.19"*, %"class.std::__1::allocator.19"** %10, align 8
  %81 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %11, align 8
  %82 = bitcast %"struct.std::__1::__hash_node_base.15"** %81 to i8*
  store i8* %82, i8** %9, align 8
  %83 = load i8*, i8** %9, align 8
  call void @_ZdlPv(i8* %83) #14
  br label %84

; <label>:84:                                     ; preds = %1, %55
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeIjPN4llvm10StructTypeEEENS_22__unordered_map_hasherIjS5_NS_4hashIjEELb1EEENS_21__unordered_map_equalIjS5_NS_8equal_toIjEELb1EEENS_9allocatorIS5_EEE12__deallocateEPNS_16__hash_node_baseIPNS_11__hash_nodeIS5_PvEEEE(%"class.std::__1::__hash_table.11"*, %"struct.std::__1::__hash_node_base.15"*) #5 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::allocator.23"*, align 8
  %4 = alloca %"struct.std::__1::pair.125"*, align 8
  %5 = alloca %"class.std::__1::allocator.23"*, align 8
  %6 = alloca %"struct.std::__1::pair.125"*, align 8
  %7 = alloca %"struct.std::__1::integral_constant", align 1
  %8 = alloca %"struct.std::__1::__has_destroy.127", align 1
  %9 = alloca %"struct.std::__1::pair.125"*, align 8
  %10 = alloca %"union.std::__1::__hash_value_type.124"*, align 8
  %11 = alloca i8*, align 8
  %12 = alloca %"class.std::__1::allocator.23"*, align 8
  %13 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %14 = alloca i64, align 8
  %15 = alloca %"class.std::__1::allocator.23"*, align 8
  %16 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %17 = alloca i64, align 8
  %18 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %19 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %20 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.22"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.21"*, align 8
  %23 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %24 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %25 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %26 = alloca %"class.std::__1::allocator.23"*, align 8
  %27 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %28 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  store %"class.std::__1::__hash_table.11"* %0, %"class.std::__1::__hash_table.11"** %24, align 8
  store %"struct.std::__1::__hash_node_base.15"* %1, %"struct.std::__1::__hash_node_base.15"** %25, align 8
  %29 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %24, align 8
  store %"class.std::__1::__hash_table.11"* %29, %"class.std::__1::__hash_table.11"** %23, align 8
  %30 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %23, align 8
  %31 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %30, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.21"* %31, %"class.std::__1::__compressed_pair.21"** %22, align 8
  %32 = load %"class.std::__1::__compressed_pair.21"*, %"class.std::__1::__compressed_pair.21"** %22, align 8
  %33 = bitcast %"class.std::__1::__compressed_pair.21"* %32 to %"class.std::__1::__libcpp_compressed_pair_imp.22"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.22"* %33, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %21, align 8
  %34 = load %"class.std::__1::__libcpp_compressed_pair_imp.22"*, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %21, align 8
  %35 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.22"* %34 to %"class.std::__1::allocator.23"*
  store %"class.std::__1::allocator.23"* %35, %"class.std::__1::allocator.23"** %26, align 8
  br label %36

; <label>:36:                                     ; preds = %61, %2
  %37 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %25, align 8
  %38 = icmp ne %"struct.std::__1::__hash_node_base.15"* %37, null
  br i1 %38, label %39, label %72

; <label>:39:                                     ; preds = %36
  %40 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %25, align 8
  %41 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %40, i32 0, i32 0
  %42 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %41, align 8
  store %"struct.std::__1::__hash_node_base.15"* %42, %"struct.std::__1::__hash_node_base.15"** %27, align 8
  %43 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %25, align 8
  store %"struct.std::__1::__hash_node_base.15"* %43, %"struct.std::__1::__hash_node_base.15"** %20, align 8
  %44 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %20, align 8
  store %"struct.std::__1::__hash_node_base.15"* %44, %"struct.std::__1::__hash_node_base.15"** %19, align 8
  %45 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %19, align 8
  store %"struct.std::__1::__hash_node_base.15"* %45, %"struct.std::__1::__hash_node_base.15"** %18, align 8
  %46 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %18, align 8
  %47 = bitcast %"struct.std::__1::__hash_node_base.15"* %46 to i8*
  %48 = bitcast %"struct.std::__1::__hash_node_base.15"* %46 to %"struct.std::__1::__hash_node.123"*
  store %"struct.std::__1::__hash_node.123"* %48, %"struct.std::__1::__hash_node.123"** %28, align 8
  %49 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %26, align 8
  %50 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %28, align 8
  %51 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %50, i32 0, i32 2
  store %"union.std::__1::__hash_value_type.124"* %51, %"union.std::__1::__hash_value_type.124"** %10, align 8
  %52 = load %"union.std::__1::__hash_value_type.124"*, %"union.std::__1::__hash_value_type.124"** %10, align 8
  %53 = bitcast %"union.std::__1::__hash_value_type.124"* %52 to %"struct.std::__1::pair.125"*
  store %"struct.std::__1::pair.125"* %53, %"struct.std::__1::pair.125"** %9, align 8
  %54 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %9, align 8
  %55 = bitcast %"struct.std::__1::pair.125"* %54 to i8*
  br label %56

; <label>:56:                                     ; preds = %39
  store %"class.std::__1::allocator.23"* %49, %"class.std::__1::allocator.23"** %5, align 8
  store %"struct.std::__1::pair.125"* %54, %"struct.std::__1::pair.125"** %6, align 8
  %57 = bitcast %"struct.std::__1::__has_destroy.127"* %8 to %"struct.std::__1::integral_constant"*
  %58 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %5, align 8
  %59 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %6, align 8
  store %"class.std::__1::allocator.23"* %58, %"class.std::__1::allocator.23"** %3, align 8
  store %"struct.std::__1::pair.125"* %59, %"struct.std::__1::pair.125"** %4, align 8
  %60 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %4, align 8
  br label %61

; <label>:61:                                     ; preds = %56
  %62 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %26, align 8
  %63 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %28, align 8
  store %"class.std::__1::allocator.23"* %62, %"class.std::__1::allocator.23"** %15, align 8
  store %"struct.std::__1::__hash_node.123"* %63, %"struct.std::__1::__hash_node.123"** %16, align 8
  store i64 1, i64* %17, align 8
  %64 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %15, align 8
  %65 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %16, align 8
  %66 = load i64, i64* %17, align 8
  store %"class.std::__1::allocator.23"* %64, %"class.std::__1::allocator.23"** %12, align 8
  store %"struct.std::__1::__hash_node.123"* %65, %"struct.std::__1::__hash_node.123"** %13, align 8
  store i64 %66, i64* %14, align 8
  %67 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %12, align 8
  %68 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %13, align 8
  %69 = bitcast %"struct.std::__1::__hash_node.123"* %68 to i8*
  store i8* %69, i8** %11, align 8
  %70 = load i8*, i8** %11, align 8
  call void @_ZdlPv(i8* %70) #14
  %71 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %27, align 8
  store %"struct.std::__1::__hash_node_base.15"* %71, %"struct.std::__1::__hash_node_base.15"** %25, align 8
  br label %36

; <label>:72:                                     ; preds = %36
  ret void
                                                  ; No predecessors!
  %74 = landingpad { i8*, i32 }
          catch i8* null
  %75 = extractvalue { i8*, i32 } %74, 0
  call void @__clang_call_terminate(i8* %75) #15
  unreachable
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEED2Ev(%"class.std::__1::vector.78"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::vector.78"*, align 8
  store %"class.std::__1::vector.78"* %0, %"class.std::__1::vector.78"** %2, align 8
  %3 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %2, align 8
  %4 = bitcast %"class.std::__1::vector.78"* %3 to %"class.std::__1::__vector_base.79"*
  call void @_ZNSt3__113__vector_baseIPN4llvm4TypeENS_9allocatorIS3_EEED2Ev(%"class.std::__1::__vector_base.79"* %4) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113__vector_baseIPN4llvm4TypeENS_9allocatorIS3_EEED2Ev(%"class.std::__1::__vector_base.79"*) unnamed_addr #5 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %4 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %5 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %8 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca %"class.std::__1::allocator.82"*, align 8
  %11 = alloca %"class.llvm::Type"**, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::allocator.82"*, align 8
  %14 = alloca %"class.llvm::Type"**, align 8
  %15 = alloca i64, align 8
  %16 = alloca %"class.std::__1::allocator.82"*, align 8
  %17 = alloca %"class.llvm::Type"**, align 8
  %18 = alloca %"struct.std::__1::integral_constant.128", align 1
  %19 = alloca %"class.std::__1::allocator.82"*, align 8
  %20 = alloca %"class.llvm::Type"**, align 8
  %21 = alloca %"class.std::__1::allocator.82"*, align 8
  %22 = alloca %"class.llvm::Type"**, align 8
  %23 = alloca %"struct.std::__1::integral_constant.128", align 1
  %24 = alloca %"struct.std::__1::__has_destroy.129", align 1
  %25 = alloca %"class.llvm::Type"**, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %28 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %29 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %30 = alloca %"class.llvm::Type"**, align 8
  %31 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %32 = alloca %"class.std::__1::__vector_base.79"*, align 8
  store %"class.std::__1::__vector_base.79"* %0, %"class.std::__1::__vector_base.79"** %32, align 8
  %33 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %32, align 8
  %34 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %33, i32 0, i32 0
  %35 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %34, align 8
  %36 = icmp ne %"class.llvm::Type"** %35, null
  br i1 %36, label %37, label %95

; <label>:37:                                     ; preds = %1
  store %"class.std::__1::__vector_base.79"* %33, %"class.std::__1::__vector_base.79"** %31, align 8
  %38 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %31, align 8
  %39 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %38, i32 0, i32 0
  %40 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %39, align 8
  store %"class.std::__1::__vector_base.79"* %38, %"class.std::__1::__vector_base.79"** %29, align 8
  store %"class.llvm::Type"** %40, %"class.llvm::Type"*** %30, align 8
  %41 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %29, align 8
  br label %42

; <label>:42:                                     ; preds = %47, %37
  %43 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %30, align 8
  %44 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %41, i32 0, i32 1
  %45 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %44, align 8
  %46 = icmp ne %"class.llvm::Type"** %43, %45
  br i1 %46, label %47, label %65

; <label>:47:                                     ; preds = %42
  store %"class.std::__1::__vector_base.79"* %41, %"class.std::__1::__vector_base.79"** %28, align 8
  %48 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %28, align 8
  %49 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %48, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %49, %"class.std::__1::__compressed_pair.80"** %27, align 8
  %50 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %27, align 8
  %51 = bitcast %"class.std::__1::__compressed_pair.80"* %50 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %51, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %26, align 8
  %52 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %26, align 8
  %53 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.81"* %52 to %"class.std::__1::allocator.82"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %41, i32 0, i32 1
  %55 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %54, align 8
  %56 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %55, i32 -1
  store %"class.llvm::Type"** %56, %"class.llvm::Type"*** %54, align 8
  store %"class.llvm::Type"** %56, %"class.llvm::Type"*** %25, align 8
  %57 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %25, align 8
  store %"class.std::__1::allocator.82"* %53, %"class.std::__1::allocator.82"** %21, align 8
  store %"class.llvm::Type"** %57, %"class.llvm::Type"*** %22, align 8
  %58 = bitcast %"struct.std::__1::__has_destroy.129"* %24 to %"struct.std::__1::integral_constant.128"*
  %59 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %21, align 8
  %60 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %22, align 8
  store %"class.std::__1::allocator.82"* %59, %"class.std::__1::allocator.82"** %19, align 8
  store %"class.llvm::Type"** %60, %"class.llvm::Type"*** %20, align 8
  %61 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %19, align 8
  %62 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %20, align 8
  store %"class.std::__1::allocator.82"* %61, %"class.std::__1::allocator.82"** %16, align 8
  store %"class.llvm::Type"** %62, %"class.llvm::Type"*** %17, align 8
  %63 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %16, align 8
  %64 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %17, align 8
  br label %42

; <label>:65:                                     ; preds = %42
  store %"class.std::__1::__vector_base.79"* %33, %"class.std::__1::__vector_base.79"** %8, align 8
  %66 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %8, align 8
  %67 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %66, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %67, %"class.std::__1::__compressed_pair.80"** %7, align 8
  %68 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %7, align 8
  %69 = bitcast %"class.std::__1::__compressed_pair.80"* %68 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %69, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %6, align 8
  %70 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %6, align 8
  %71 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.81"* %70 to %"class.std::__1::allocator.82"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %33, i32 0, i32 0
  %73 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %72, align 8
  store %"class.std::__1::__vector_base.79"* %33, %"class.std::__1::__vector_base.79"** %5, align 8
  %74 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %5, align 8
  store %"class.std::__1::__vector_base.79"* %74, %"class.std::__1::__vector_base.79"** %4, align 8
  %75 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %4, align 8
  %76 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %75, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %76, %"class.std::__1::__compressed_pair.80"** %3, align 8
  %77 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %3, align 8
  %78 = bitcast %"class.std::__1::__compressed_pair.80"* %77 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %78, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %2, align 8
  %79 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %2, align 8
  %80 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %79, i32 0, i32 0
  %81 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %80, align 8
  %82 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %74, i32 0, i32 0
  %83 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %82, align 8
  %84 = ptrtoint %"class.llvm::Type"** %81 to i64
  %85 = ptrtoint %"class.llvm::Type"** %83 to i64
  %86 = sub i64 %84, %85
  %87 = sdiv exact i64 %86, 8
  store %"class.std::__1::allocator.82"* %71, %"class.std::__1::allocator.82"** %13, align 8
  store %"class.llvm::Type"** %73, %"class.llvm::Type"*** %14, align 8
  store i64 %87, i64* %15, align 8
  %88 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %13, align 8
  %89 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %14, align 8
  %90 = load i64, i64* %15, align 8
  store %"class.std::__1::allocator.82"* %88, %"class.std::__1::allocator.82"** %10, align 8
  store %"class.llvm::Type"** %89, %"class.llvm::Type"*** %11, align 8
  store i64 %90, i64* %12, align 8
  %91 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %10, align 8
  %92 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %11, align 8
  %93 = bitcast %"class.llvm::Type"** %92 to i8*
  store i8* %93, i8** %9, align 8
  %94 = load i8*, i8** %9, align 8
  call void @_ZdlPv(i8* %94) #14
  br label %95

; <label>:95:                                     ; preds = %65, %1
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm5APIntC2Ejyb(%"class.llvm::APInt"*, i32, i64, i1 zeroext) unnamed_addr #0 align 2 {
  %5 = alloca %"class.llvm::APInt"*, align 8
  %6 = alloca i32, align 4
  %7 = alloca i64, align 8
  %8 = alloca i8, align 1
  store %"class.llvm::APInt"* %0, %"class.llvm::APInt"** %5, align 8
  store i32 %1, i32* %6, align 4
  store i64 %2, i64* %7, align 8
  %9 = zext i1 %3 to i8
  store i8 %9, i8* %8, align 1
  %10 = load %"class.llvm::APInt"*, %"class.llvm::APInt"** %5, align 8
  %11 = getelementptr inbounds %"class.llvm::APInt", %"class.llvm::APInt"* %10, i32 0, i32 0
  %12 = load i32, i32* %6, align 4
  store i32 %12, i32* %11, align 8
  %13 = getelementptr inbounds %"class.llvm::APInt", %"class.llvm::APInt"* %10, i32 0, i32 1
  %14 = bitcast %union.anon.115* %13 to i64*
  store i64 0, i64* %14, align 8
  %15 = getelementptr inbounds %"class.llvm::APInt", %"class.llvm::APInt"* %10, i32 0, i32 0
  %16 = load i32, i32* %15, align 8
  %17 = icmp ne i32 %16, 0
  br i1 %17, label %18, label %19

; <label>:18:                                     ; preds = %4
  br label %19

; <label>:19:                                     ; preds = %18, %4
  %20 = phi i1 [ false, %4 ], [ true, %18 ]
  %21 = xor i1 %20, true
  br i1 %21, label %22, label %24

; <label>:22:                                     ; preds = %19
  call void @__assert_rtn(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__._ZN4llvm5APIntC2Ejyb, i32 0, i32 0), i8* getelementptr inbounds ([56 x i8], [56 x i8]* @.str.4, i32 0, i32 0), i32 242, i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.5, i32 0, i32 0)) #16
  unreachable
                                                  ; No predecessors!
  br label %25

; <label>:24:                                     ; preds = %19
  br label %25

; <label>:25:                                     ; preds = %24, %23
  %26 = call zeroext i1 @_ZNK4llvm5APInt12isSingleWordEv(%"class.llvm::APInt"* %10)
  br i1 %26, label %27, label %31

; <label>:27:                                     ; preds = %25
  %28 = load i64, i64* %7, align 8
  %29 = getelementptr inbounds %"class.llvm::APInt", %"class.llvm::APInt"* %10, i32 0, i32 1
  %30 = bitcast %union.anon.115* %29 to i64*
  store i64 %28, i64* %30, align 8
  br label %35

; <label>:31:                                     ; preds = %25
  %32 = load i64, i64* %7, align 8
  %33 = load i8, i8* %8, align 1
  %34 = trunc i8 %33 to i1
  call void @_ZN4llvm5APInt12initSlowCaseEyb(%"class.llvm::APInt"* %10, i64 %32, i1 zeroext %34)
  br label %35

; <label>:35:                                     ; preds = %31, %27
  %36 = call dereferenceable(16) %"class.llvm::APInt"* @_ZN4llvm5APInt15clearUnusedBitsEv(%"class.llvm::APInt"* %10)
  ret void
}

; Function Attrs: noreturn
declare void @__assert_rtn(i8*, i8*, i32, i8*) #8

; Function Attrs: nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZNK4llvm5APInt12isSingleWordEv(%"class.llvm::APInt"*) #5 align 2 {
  %2 = alloca %"class.llvm::APInt"*, align 8
  store %"class.llvm::APInt"* %0, %"class.llvm::APInt"** %2, align 8
  %3 = load %"class.llvm::APInt"*, %"class.llvm::APInt"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::APInt", %"class.llvm::APInt"* %3, i32 0, i32 0
  %5 = load i32, i32* %4, align 8
  %6 = icmp ule i32 %5, 64
  ret i1 %6
}

declare void @_ZN4llvm5APInt12initSlowCaseEyb(%"class.llvm::APInt"*, i64, i1 zeroext) #3

; Function Attrs: ssp uwtable
define linkonce_odr dereferenceable(16) %"class.llvm::APInt"* @_ZN4llvm5APInt15clearUnusedBitsEv(%"class.llvm::APInt"*) #0 align 2 {
  %2 = alloca %"class.llvm::APInt"*, align 8
  %3 = alloca %"class.llvm::APInt"*, align 8
  %4 = alloca i32, align 4
  %5 = alloca i64, align 8
  store %"class.llvm::APInt"* %0, %"class.llvm::APInt"** %3, align 8
  %6 = load %"class.llvm::APInt"*, %"class.llvm::APInt"** %3, align 8
  %7 = getelementptr inbounds %"class.llvm::APInt", %"class.llvm::APInt"* %6, i32 0, i32 0
  %8 = load i32, i32* %7, align 8
  %9 = urem i32 %8, 64
  store i32 %9, i32* %4, align 4
  %10 = load i32, i32* %4, align 4
  %11 = icmp eq i32 %10, 0
  br i1 %11, label %12, label %13

; <label>:12:                                     ; preds = %1
  store %"class.llvm::APInt"* %6, %"class.llvm::APInt"** %2, align 8
  br label %37

; <label>:13:                                     ; preds = %1
  %14 = load i32, i32* %4, align 4
  %15 = sub i32 64, %14
  %16 = zext i32 %15 to i64
  %17 = lshr i64 -1, %16
  store i64 %17, i64* %5, align 8
  %18 = call zeroext i1 @_ZNK4llvm5APInt12isSingleWordEv(%"class.llvm::APInt"* %6)
  br i1 %18, label %19, label %25

; <label>:19:                                     ; preds = %13
  %20 = load i64, i64* %5, align 8
  %21 = getelementptr inbounds %"class.llvm::APInt", %"class.llvm::APInt"* %6, i32 0, i32 1
  %22 = bitcast %union.anon.115* %21 to i64*
  %23 = load i64, i64* %22, align 8
  %24 = and i64 %23, %20
  store i64 %24, i64* %22, align 8
  br label %36

; <label>:25:                                     ; preds = %13
  %26 = load i64, i64* %5, align 8
  %27 = call i32 @_ZNK4llvm5APInt11getNumWordsEv(%"class.llvm::APInt"* %6)
  %28 = sub i32 %27, 1
  %29 = zext i32 %28 to i64
  %30 = getelementptr inbounds %"class.llvm::APInt", %"class.llvm::APInt"* %6, i32 0, i32 1
  %31 = bitcast %union.anon.115* %30 to i64**
  %32 = load i64*, i64** %31, align 8
  %33 = getelementptr inbounds i64, i64* %32, i64 %29
  %34 = load i64, i64* %33, align 8
  %35 = and i64 %34, %26
  store i64 %35, i64* %33, align 8
  br label %36

; <label>:36:                                     ; preds = %25, %19
  store %"class.llvm::APInt"* %6, %"class.llvm::APInt"** %2, align 8
  br label %37

; <label>:37:                                     ; preds = %36, %12
  %38 = load %"class.llvm::APInt"*, %"class.llvm::APInt"** %2, align 8
  ret %"class.llvm::APInt"* %38
}

; Function Attrs: ssp uwtable
define linkonce_odr i32 @_ZNK4llvm5APInt11getNumWordsEv(%"class.llvm::APInt"*) #0 align 2 {
  %2 = alloca %"class.llvm::APInt"*, align 8
  store %"class.llvm::APInt"* %0, %"class.llvm::APInt"** %2, align 8
  %3 = load %"class.llvm::APInt"*, %"class.llvm::APInt"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::APInt", %"class.llvm::APInt"* %3, i32 0, i32 0
  %5 = load i32, i32* %4, align 8
  %6 = call i32 @_ZN4llvm5APInt11getNumWordsEj(i32 %5)
  ret i32 %6
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i32 @_ZN4llvm5APInt11getNumWordsEj(i32) #5 align 2 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = zext i32 %3 to i64
  %5 = add i64 %4, 64
  %6 = sub i64 %5, 1
  %7 = udiv i64 %6, 64
  %8 = trunc i64 %7 to i32
  ret i32 %8
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm5APIntD2Ev(%"class.llvm::APInt"*) unnamed_addr #5 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"class.llvm::APInt"*, align 8
  store %"class.llvm::APInt"* %0, %"class.llvm::APInt"** %2, align 8
  %3 = load %"class.llvm::APInt"*, %"class.llvm::APInt"** %2, align 8
  %4 = invoke zeroext i1 @_ZNK4llvm5APInt12needsCleanupEv(%"class.llvm::APInt"* %3)
          to label %5 unwind label %15

; <label>:5:                                      ; preds = %1
  br i1 %4, label %6, label %14

; <label>:6:                                      ; preds = %5
  %7 = getelementptr inbounds %"class.llvm::APInt", %"class.llvm::APInt"* %3, i32 0, i32 1
  %8 = bitcast %union.anon.115* %7 to i64**
  %9 = load i64*, i64** %8, align 8
  %10 = icmp eq i64* %9, null
  br i1 %10, label %13, label %11

; <label>:11:                                     ; preds = %6
  %12 = bitcast i64* %9 to i8*
  call void @_ZdaPv(i8* %12) #14
  br label %13

; <label>:13:                                     ; preds = %11, %6
  br label %14

; <label>:14:                                     ; preds = %13, %5
  ret void

; <label>:15:                                     ; preds = %1
  %16 = landingpad { i8*, i32 }
          catch i8* null
  %17 = extractvalue { i8*, i32 } %16, 0
  call void @__clang_call_terminate(i8* %17) #15
  unreachable
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZNK4llvm5APInt12needsCleanupEv(%"class.llvm::APInt"*) #5 align 2 {
  %2 = alloca %"class.llvm::APInt"*, align 8
  store %"class.llvm::APInt"* %0, %"class.llvm::APInt"** %2, align 8
  %3 = load %"class.llvm::APInt"*, %"class.llvm::APInt"** %2, align 8
  %4 = call zeroext i1 @_ZNK4llvm5APInt12isSingleWordEv(%"class.llvm::APInt"* %3)
  %5 = xor i1 %4, true
  ret i1 %5
}

; Function Attrs: nobuiltin nounwind
declare void @_ZdaPv(i8*) #7

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm7APFloatC2Ef(%"class.llvm::APFloat"*, float) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.llvm::APFloat"*, align 8
  %4 = alloca float, align 4
  %5 = alloca %"class.llvm::detail::IEEEFloat", align 8
  %6 = alloca i8*
  %7 = alloca i32
  store %"class.llvm::APFloat"* %0, %"class.llvm::APFloat"** %3, align 8
  store float %1, float* %4, align 4
  %8 = load %"class.llvm::APFloat"*, %"class.llvm::APFloat"** %3, align 8
  %9 = bitcast %"class.llvm::APFloat"* %8 to %"struct.llvm::APFloatBase"*
  %10 = getelementptr inbounds %"class.llvm::APFloat", %"class.llvm::APFloat"* %8, i32 0, i32 1
  %11 = load float, float* %4, align 4
  call void @_ZN4llvm6detail9IEEEFloatC1Ef(%"class.llvm::detail::IEEEFloat"* %5, float %11)
  %12 = invoke nonnull %"struct.llvm::fltSemantics"* @_ZN4llvm11APFloatBase10IEEEsingleEv()
          to label %13 unwind label %15

; <label>:13:                                     ; preds = %2
  invoke void @_ZN4llvm7APFloat7StorageC1ENS_6detail9IEEEFloatERKNS_12fltSemanticsE(%"union.llvm::APFloat::Storage"* %10, %"class.llvm::detail::IEEEFloat"* %5, %"struct.llvm::fltSemantics"* nonnull %12)
          to label %14 unwind label %15

; <label>:14:                                     ; preds = %13
  call void @_ZN4llvm6detail9IEEEFloatD1Ev(%"class.llvm::detail::IEEEFloat"* %5) #2
  ret void

; <label>:15:                                     ; preds = %13, %2
  %16 = landingpad { i8*, i32 }
          cleanup
  %17 = extractvalue { i8*, i32 } %16, 0
  store i8* %17, i8** %6, align 8
  %18 = extractvalue { i8*, i32 } %16, 1
  store i32 %18, i32* %7, align 4
  call void @_ZN4llvm6detail9IEEEFloatD1Ev(%"class.llvm::detail::IEEEFloat"* %5) #2
  br label %19

; <label>:19:                                     ; preds = %15
  %20 = load i8*, i8** %6, align 8
  %21 = load i32, i32* %7, align 4
  %22 = insertvalue { i8*, i32 } undef, i8* %20, 0
  %23 = insertvalue { i8*, i32 } %22, i32 %21, 1
  resume { i8*, i32 } %23
}

declare void @_ZN4llvm6detail9IEEEFloatC1Ef(%"class.llvm::detail::IEEEFloat"*, float) unnamed_addr #3

declare nonnull %"struct.llvm::fltSemantics"* @_ZN4llvm11APFloatBase10IEEEsingleEv() #3

declare void @_ZN4llvm7APFloat7StorageC1ENS_6detail9IEEEFloatERKNS_12fltSemanticsE(%"union.llvm::APFloat::Storage"*, %"class.llvm::detail::IEEEFloat"*, %"struct.llvm::fltSemantics"* nonnull) unnamed_addr #3

; Function Attrs: nounwind
declare void @_ZN4llvm6detail9IEEEFloatD1Ev(%"class.llvm::detail::IEEEFloat"*) unnamed_addr #4

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm7APFloatD2Ev(%"class.llvm::APFloat"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.llvm::APFloat"*, align 8
  store %"class.llvm::APFloat"* %0, %"class.llvm::APFloat"** %2, align 8
  %3 = load %"class.llvm::APFloat"*, %"class.llvm::APFloat"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::APFloat", %"class.llvm::APFloat"* %3, i32 0, i32 1
  call void @_ZN4llvm7APFloat7StorageD1Ev(%"union.llvm::APFloat::Storage"* %4) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm7APFloat7StorageD1Ev(%"union.llvm::APFloat::Storage"*) unnamed_addr #5 align 2 {
  %2 = alloca %"union.llvm::APFloat::Storage"*, align 8
  store %"union.llvm::APFloat::Storage"* %0, %"union.llvm::APFloat::Storage"** %2, align 8
  %3 = load %"union.llvm::APFloat::Storage"*, %"union.llvm::APFloat::Storage"** %2, align 8
  call void @_ZN4llvm7APFloat7StorageD2Ev(%"union.llvm::APFloat::Storage"* %3) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm7APFloat7StorageD2Ev(%"union.llvm::APFloat::Storage"*) unnamed_addr #5 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"union.llvm::APFloat::Storage"*, align 8
  store %"union.llvm::APFloat::Storage"* %0, %"union.llvm::APFloat::Storage"** %2, align 8
  %3 = load %"union.llvm::APFloat::Storage"*, %"union.llvm::APFloat::Storage"** %2, align 8
  %4 = bitcast %"union.llvm::APFloat::Storage"* %3 to %"struct.llvm::fltSemantics"**
  %5 = load %"struct.llvm::fltSemantics"*, %"struct.llvm::fltSemantics"** %4, align 8
  %6 = invoke zeroext i1 @_ZN4llvm7APFloat10usesLayoutINS_6detail9IEEEFloatEEEbRKNS_12fltSemanticsE(%"struct.llvm::fltSemantics"* nonnull %5)
          to label %7 unwind label %20

; <label>:7:                                      ; preds = %1
  br i1 %6, label %8, label %10

; <label>:8:                                      ; preds = %7
  %9 = bitcast %"union.llvm::APFloat::Storage"* %3 to %"class.llvm::detail::IEEEFloat"*
  call void @_ZN4llvm6detail9IEEEFloatD1Ev(%"class.llvm::detail::IEEEFloat"* %9) #2
  br label %19

; <label>:10:                                     ; preds = %7
  %11 = bitcast %"union.llvm::APFloat::Storage"* %3 to %"struct.llvm::fltSemantics"**
  %12 = load %"struct.llvm::fltSemantics"*, %"struct.llvm::fltSemantics"** %11, align 8
  %13 = invoke zeroext i1 @_ZN4llvm7APFloat10usesLayoutINS_6detail13DoubleAPFloatEEEbRKNS_12fltSemanticsE(%"struct.llvm::fltSemantics"* nonnull %12)
          to label %14 unwind label %20

; <label>:14:                                     ; preds = %10
  br i1 %13, label %15, label %17

; <label>:15:                                     ; preds = %14
  %16 = bitcast %"union.llvm::APFloat::Storage"* %3 to %"class.llvm::detail::DoubleAPFloat"*
  call void @_ZN4llvm6detail13DoubleAPFloatD1Ev(%"class.llvm::detail::DoubleAPFloat"* %16) #2
  br label %19

; <label>:17:                                     ; preds = %14
  invoke void @_ZN4llvm25llvm_unreachable_internalEPKcS1_j(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.6, i32 0, i32 0), i8* getelementptr inbounds ([58 x i8], [58 x i8]* @.str.7, i32 0, i32 0), i32 692) #16
          to label %18 unwind label %20

; <label>:18:                                     ; preds = %17
  unreachable

; <label>:19:                                     ; preds = %15, %8
  ret void

; <label>:20:                                     ; preds = %17, %10, %1
  %21 = landingpad { i8*, i32 }
          catch i8* null
  %22 = extractvalue { i8*, i32 } %21, 0
  call void @__clang_call_terminate(i8* %22) #15
  unreachable
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm7APFloat10usesLayoutINS_6detail9IEEEFloatEEEbRKNS_12fltSemanticsE(%"struct.llvm::fltSemantics"* nonnull) #0 align 2 {
  %2 = alloca %"struct.llvm::fltSemantics"*, align 8
  store %"struct.llvm::fltSemantics"* %0, %"struct.llvm::fltSemantics"** %2, align 8
  %3 = load %"struct.llvm::fltSemantics"*, %"struct.llvm::fltSemantics"** %2, align 8
  %4 = call nonnull %"struct.llvm::fltSemantics"* @_ZN4llvm11APFloatBase15PPCDoubleDoubleEv()
  %5 = icmp ne %"struct.llvm::fltSemantics"* %3, %4
  ret i1 %5
}

; Function Attrs: ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm7APFloat10usesLayoutINS_6detail13DoubleAPFloatEEEbRKNS_12fltSemanticsE(%"struct.llvm::fltSemantics"* nonnull) #0 align 2 {
  %2 = alloca %"struct.llvm::fltSemantics"*, align 8
  store %"struct.llvm::fltSemantics"* %0, %"struct.llvm::fltSemantics"** %2, align 8
  %3 = load %"struct.llvm::fltSemantics"*, %"struct.llvm::fltSemantics"** %2, align 8
  %4 = call nonnull %"struct.llvm::fltSemantics"* @_ZN4llvm11APFloatBase15PPCDoubleDoubleEv()
  %5 = icmp eq %"struct.llvm::fltSemantics"* %3, %4
  ret i1 %5
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm6detail13DoubleAPFloatD1Ev(%"class.llvm::detail::DoubleAPFloat"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.llvm::detail::DoubleAPFloat"*, align 8
  store %"class.llvm::detail::DoubleAPFloat"* %0, %"class.llvm::detail::DoubleAPFloat"** %2, align 8
  %3 = load %"class.llvm::detail::DoubleAPFloat"*, %"class.llvm::detail::DoubleAPFloat"** %2, align 8
  call void @_ZN4llvm6detail13DoubleAPFloatD2Ev(%"class.llvm::detail::DoubleAPFloat"* %3) #2
  ret void
}

; Function Attrs: noreturn
declare void @_ZN4llvm25llvm_unreachable_internalEPKcS1_j(i8*, i8*, i32) #8

declare nonnull %"struct.llvm::fltSemantics"* @_ZN4llvm11APFloatBase15PPCDoubleDoubleEv() #3

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm6detail13DoubleAPFloatD2Ev(%"class.llvm::detail::DoubleAPFloat"*) unnamed_addr #1 align 2 {
  %2 = alloca %"struct.std::__1::default_delete.119"*, align 8
  %3 = alloca %"class.llvm::APFloat"*, align 8
  %4 = alloca i8*, align 8
  %5 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.118"*, align 8
  %6 = alloca %"class.std::__1::__compressed_pair.117"*, align 8
  %7 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.118"*, align 8
  %8 = alloca %"class.std::__1::__compressed_pair.117"*, align 8
  %9 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.118"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair.117"*, align 8
  %11 = alloca %"class.std::__1::unique_ptr.116"*, align 8
  %12 = alloca %"class.llvm::APFloat"*, align 8
  %13 = alloca %"class.std::__1::unique_ptr.116"*, align 8
  %14 = alloca %"class.std::__1::unique_ptr.116"*, align 8
  %15 = alloca %"class.llvm::detail::DoubleAPFloat"*, align 8
  store %"class.llvm::detail::DoubleAPFloat"* %0, %"class.llvm::detail::DoubleAPFloat"** %15, align 8
  %16 = load %"class.llvm::detail::DoubleAPFloat"*, %"class.llvm::detail::DoubleAPFloat"** %15, align 8
  %17 = getelementptr inbounds %"class.llvm::detail::DoubleAPFloat", %"class.llvm::detail::DoubleAPFloat"* %16, i32 0, i32 1
  store %"class.std::__1::unique_ptr.116"* %17, %"class.std::__1::unique_ptr.116"** %14, align 8
  %18 = load %"class.std::__1::unique_ptr.116"*, %"class.std::__1::unique_ptr.116"** %14, align 8
  store %"class.std::__1::unique_ptr.116"* %18, %"class.std::__1::unique_ptr.116"** %13, align 8
  %19 = load %"class.std::__1::unique_ptr.116"*, %"class.std::__1::unique_ptr.116"** %13, align 8
  store %"class.std::__1::unique_ptr.116"* %19, %"class.std::__1::unique_ptr.116"** %11, align 8
  %20 = load %"class.std::__1::unique_ptr.116"*, %"class.std::__1::unique_ptr.116"** %11, align 8
  %21 = getelementptr inbounds %"class.std::__1::unique_ptr.116", %"class.std::__1::unique_ptr.116"* %20, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.117"* %21, %"class.std::__1::__compressed_pair.117"** %10, align 8
  %22 = load %"class.std::__1::__compressed_pair.117"*, %"class.std::__1::__compressed_pair.117"** %10, align 8
  %23 = bitcast %"class.std::__1::__compressed_pair.117"* %22 to %"class.std::__1::__libcpp_compressed_pair_imp.118"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.118"* %23, %"class.std::__1::__libcpp_compressed_pair_imp.118"** %9, align 8
  %24 = load %"class.std::__1::__libcpp_compressed_pair_imp.118"*, %"class.std::__1::__libcpp_compressed_pair_imp.118"** %9, align 8
  %25 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.118", %"class.std::__1::__libcpp_compressed_pair_imp.118"* %24, i32 0, i32 0
  %26 = load %"class.llvm::APFloat"*, %"class.llvm::APFloat"** %25, align 8
  store %"class.llvm::APFloat"* %26, %"class.llvm::APFloat"** %12, align 8
  %27 = getelementptr inbounds %"class.std::__1::unique_ptr.116", %"class.std::__1::unique_ptr.116"* %20, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.117"* %27, %"class.std::__1::__compressed_pair.117"** %8, align 8
  %28 = load %"class.std::__1::__compressed_pair.117"*, %"class.std::__1::__compressed_pair.117"** %8, align 8
  %29 = bitcast %"class.std::__1::__compressed_pair.117"* %28 to %"class.std::__1::__libcpp_compressed_pair_imp.118"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.118"* %29, %"class.std::__1::__libcpp_compressed_pair_imp.118"** %7, align 8
  %30 = load %"class.std::__1::__libcpp_compressed_pair_imp.118"*, %"class.std::__1::__libcpp_compressed_pair_imp.118"** %7, align 8
  %31 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.118", %"class.std::__1::__libcpp_compressed_pair_imp.118"* %30, i32 0, i32 0
  store %"class.llvm::APFloat"* null, %"class.llvm::APFloat"** %31, align 8
  %32 = load %"class.llvm::APFloat"*, %"class.llvm::APFloat"** %12, align 8
  %33 = icmp ne %"class.llvm::APFloat"* %32, null
  br i1 %33, label %34, label %57

; <label>:34:                                     ; preds = %1
  %35 = getelementptr inbounds %"class.std::__1::unique_ptr.116", %"class.std::__1::unique_ptr.116"* %20, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.117"* %35, %"class.std::__1::__compressed_pair.117"** %6, align 8
  %36 = load %"class.std::__1::__compressed_pair.117"*, %"class.std::__1::__compressed_pair.117"** %6, align 8
  %37 = bitcast %"class.std::__1::__compressed_pair.117"* %36 to %"class.std::__1::__libcpp_compressed_pair_imp.118"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.118"* %37, %"class.std::__1::__libcpp_compressed_pair_imp.118"** %5, align 8
  %38 = load %"class.std::__1::__libcpp_compressed_pair_imp.118"*, %"class.std::__1::__libcpp_compressed_pair_imp.118"** %5, align 8
  %39 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.118"* %38 to %"struct.std::__1::default_delete.119"*
  %40 = load %"class.llvm::APFloat"*, %"class.llvm::APFloat"** %12, align 8
  store %"struct.std::__1::default_delete.119"* %39, %"struct.std::__1::default_delete.119"** %2, align 8
  store %"class.llvm::APFloat"* %40, %"class.llvm::APFloat"** %3, align 8
  store i8* null, i8** %4, align 8
  %41 = load %"struct.std::__1::default_delete.119"*, %"struct.std::__1::default_delete.119"** %2, align 8
  %42 = load %"class.llvm::APFloat"*, %"class.llvm::APFloat"** %3, align 8
  %43 = icmp eq %"class.llvm::APFloat"* %42, null
  br i1 %43, label %56, label %44

; <label>:44:                                     ; preds = %34
  %45 = bitcast %"class.llvm::APFloat"* %42 to i8*
  %46 = getelementptr inbounds i8, i8* %45, i64 -8
  %47 = bitcast i8* %46 to i64*
  %48 = load i64, i64* %47, align 8
  %49 = getelementptr inbounds %"class.llvm::APFloat", %"class.llvm::APFloat"* %42, i64 %48
  %50 = icmp eq %"class.llvm::APFloat"* %42, %49
  br i1 %50, label %55, label %51

; <label>:51:                                     ; preds = %51, %44
  %52 = phi %"class.llvm::APFloat"* [ %49, %44 ], [ %53, %51 ]
  %53 = getelementptr inbounds %"class.llvm::APFloat", %"class.llvm::APFloat"* %52, i64 -1
  call void @_ZN4llvm7APFloatD1Ev(%"class.llvm::APFloat"* %53) #2
  %54 = icmp eq %"class.llvm::APFloat"* %53, %42
  br i1 %54, label %55, label %51

; <label>:55:                                     ; preds = %51, %44
  call void @_ZdaPv(i8* %46) #14
  br label %56

; <label>:56:                                     ; preds = %34, %55
  br label %57

; <label>:57:                                     ; preds = %1, %56
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm7APFloatC2Ed(%"class.llvm::APFloat"*, double) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.llvm::APFloat"*, align 8
  %4 = alloca double, align 8
  %5 = alloca %"class.llvm::detail::IEEEFloat", align 8
  %6 = alloca i8*
  %7 = alloca i32
  store %"class.llvm::APFloat"* %0, %"class.llvm::APFloat"** %3, align 8
  store double %1, double* %4, align 8
  %8 = load %"class.llvm::APFloat"*, %"class.llvm::APFloat"** %3, align 8
  %9 = bitcast %"class.llvm::APFloat"* %8 to %"struct.llvm::APFloatBase"*
  %10 = getelementptr inbounds %"class.llvm::APFloat", %"class.llvm::APFloat"* %8, i32 0, i32 1
  %11 = load double, double* %4, align 8
  call void @_ZN4llvm6detail9IEEEFloatC1Ed(%"class.llvm::detail::IEEEFloat"* %5, double %11)
  %12 = invoke nonnull %"struct.llvm::fltSemantics"* @_ZN4llvm11APFloatBase10IEEEdoubleEv()
          to label %13 unwind label %15

; <label>:13:                                     ; preds = %2
  invoke void @_ZN4llvm7APFloat7StorageC1ENS_6detail9IEEEFloatERKNS_12fltSemanticsE(%"union.llvm::APFloat::Storage"* %10, %"class.llvm::detail::IEEEFloat"* %5, %"struct.llvm::fltSemantics"* nonnull %12)
          to label %14 unwind label %15

; <label>:14:                                     ; preds = %13
  call void @_ZN4llvm6detail9IEEEFloatD1Ev(%"class.llvm::detail::IEEEFloat"* %5) #2
  ret void

; <label>:15:                                     ; preds = %13, %2
  %16 = landingpad { i8*, i32 }
          cleanup
  %17 = extractvalue { i8*, i32 } %16, 0
  store i8* %17, i8** %6, align 8
  %18 = extractvalue { i8*, i32 } %16, 1
  store i32 %18, i32* %7, align 4
  call void @_ZN4llvm6detail9IEEEFloatD1Ev(%"class.llvm::detail::IEEEFloat"* %5) #2
  br label %19

; <label>:19:                                     ; preds = %15
  %20 = load i8*, i8** %6, align 8
  %21 = load i32, i32* %7, align 4
  %22 = insertvalue { i8*, i32 } undef, i8* %20, 0
  %23 = insertvalue { i8*, i32 } %22, i32 %21, 1
  resume { i8*, i32 } %23
}

declare void @_ZN4llvm6detail9IEEEFloatC1Ed(%"class.llvm::detail::IEEEFloat"*, double) unnamed_addr #3

declare nonnull %"struct.llvm::fltSemantics"* @_ZN4llvm11APFloatBase10IEEEdoubleEv() #3

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE21__construct_node_hashIRKNS_21piecewise_construct_tEJNS_5tupleIJRKS7_EEENSQ_IJEEEEEENS_10unique_ptrINS_11__hash_nodeISB_PvEENS_22__hash_node_destructorINS5_ISY_EEEEEEmOT_DpOT0_(%"class.std::__1::unique_ptr.132"* noalias sret, %"class.std::__1::__hash_table"*, i64, %"struct.std::__1::piecewise_construct_t"* dereferenceable(1), %"class.std::__1::tuple"* dereferenceable(8), %"class.std::__1::tuple.131"* dereferenceable(1)) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %7 = alloca %"class.std::__1::tuple"*, align 8
  %8 = alloca %"class.std::__1::tuple.131"*, align 8
  %9 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %10 = alloca %"class.std::__1::tuple.131"*, align 8
  %11 = alloca %"class.std::__1::tuple"*, align 8
  %12 = alloca %"class.std::__1::tuple.131"*, align 8
  %13 = alloca %"class.std::__1::tuple"*, align 8
  %14 = alloca %"class.std::__1::tuple.131"*, align 8
  %15 = alloca %"class.std::__1::tuple"*, align 8
  %16 = alloca %"class.std::__1::basic_string"*, align 8
  %17 = alloca %"class.std::__1::__tuple_leaf"*, align 8
  %18 = alloca %"class.std::__1::tuple"*, align 8
  %19 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %20 = alloca %"struct.std::__1::__tuple_indices", align 1
  %21 = alloca %"struct.std::__1::__tuple_indices.136", align 1
  %22 = alloca %"struct.std::__1::pair"*, align 8
  %23 = alloca %"class.std::__1::tuple"*, align 8
  %24 = alloca %"class.std::__1::tuple.131"*, align 8
  %25 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %26 = alloca %"struct.std::__1::__tuple_indices", align 1
  %27 = alloca %"struct.std::__1::__tuple_indices.136", align 1
  %28 = alloca %"struct.std::__1::pair"*, align 8
  %29 = alloca %"class.std::__1::tuple"*, align 8
  %30 = alloca %"class.std::__1::tuple.131"*, align 8
  %31 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %32 = alloca %"class.std::__1::tuple", align 8
  %33 = alloca %"class.std::__1::tuple.131", align 1
  %34 = alloca %"struct.std::__1::pair"*, align 8
  %35 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %36 = alloca %"struct.std::__1::__tuple_indices", align 1
  %37 = alloca %"struct.std::__1::__tuple_indices.136", align 1
  %38 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %39 = alloca %"class.std::__1::allocator.4"*, align 8
  %40 = alloca %"struct.std::__1::pair"*, align 8
  %41 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %42 = alloca %"class.std::__1::tuple"*, align 8
  %43 = alloca %"class.std::__1::tuple.131"*, align 8
  %44 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %45 = alloca %"class.std::__1::tuple", align 8
  %46 = alloca %"class.std::__1::tuple.131", align 1
  %47 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %48 = alloca %"struct.std::__1::integral_constant.128", align 1
  %49 = alloca %"class.std::__1::allocator.4"*, align 8
  %50 = alloca %"struct.std::__1::pair"*, align 8
  %51 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %52 = alloca %"class.std::__1::tuple"*, align 8
  %53 = alloca %"class.std::__1::tuple.131"*, align 8
  %54 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %55 = alloca %"class.std::__1::allocator.4"*, align 8
  %56 = alloca %"struct.std::__1::pair"*, align 8
  %57 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %58 = alloca %"class.std::__1::tuple"*, align 8
  %59 = alloca %"class.std::__1::tuple.131"*, align 8
  %60 = alloca %"struct.std::__1::integral_constant.128", align 1
  %61 = alloca %"struct.std::__1::__has_construct", align 1
  %62 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %63 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %64 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %65 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %66 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %67 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %68 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %69 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %70 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %71 = alloca %"struct.std::__1::pair"*, align 8
  %72 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %73 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %74 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %75 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %76 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %77 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %78 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %79 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %80 = alloca i8*, align 8
  %81 = alloca %"class.std::__1::allocator.4"*, align 8
  %82 = alloca %"struct.std::__1::__hash_node"*, align 8
  %83 = alloca i64, align 8
  %84 = alloca %"class.std::__1::allocator.4"*, align 8
  %85 = alloca %"struct.std::__1::__hash_node"*, align 8
  %86 = alloca i64, align 8
  %87 = alloca %"class.std::__1::allocator.4"*, align 8
  %88 = alloca %"struct.std::__1::pair"*, align 8
  %89 = alloca %"class.std::__1::allocator.4"*, align 8
  %90 = alloca %"struct.std::__1::pair"*, align 8
  %91 = alloca %"struct.std::__1::integral_constant", align 1
  %92 = alloca %"struct.std::__1::__has_destroy", align 1
  %93 = alloca %"struct.std::__1::pair"*, align 8
  %94 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %95 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %96 = alloca %"struct.std::__1::__hash_node"*, align 8
  %97 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %98 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %99 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %100 = alloca %"struct.std::__1::__hash_node"*, align 8
  %101 = alloca %"struct.std::__1::__hash_node"*, align 8
  %102 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %103 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %104 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %105 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %106 = alloca %"struct.std::__1::__hash_node"**, align 8
  %107 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %108 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %109 = alloca %"struct.std::__1::__hash_node"*, align 8
  %110 = alloca %"struct.std::__1::__hash_node"**, align 8
  %111 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %112 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %113 = alloca %"struct.std::__1::__hash_node"*, align 8
  %114 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %115 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %116 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %117 = alloca %"struct.std::__1::__hash_node"*, align 8
  %118 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %119 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %120 = alloca %"struct.std::__1::__hash_node"*, align 8
  %121 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %122 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %123 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %124 = alloca %"struct.std::__1::__hash_node"*, align 8
  %125 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %126 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %127 = alloca %"class.std::__1::allocator.4"*, align 8
  %128 = alloca i8, align 1
  %129 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %130 = alloca %"class.std::__1::allocator.4"*, align 8
  %131 = alloca i8, align 1
  %132 = alloca i64, align 8
  %133 = alloca %"class.std::__1::allocator.4"*, align 8
  %134 = alloca i64, align 8
  %135 = alloca i8*, align 8
  %136 = alloca %"class.std::__1::allocator.4"*, align 8
  %137 = alloca i64, align 8
  %138 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %139 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %140 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %141 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %142 = alloca i8*, align 8
  %143 = alloca %"class.std::__1::allocator.4"*, align 8
  %144 = alloca %"struct.std::__1::__hash_node"*, align 8
  %145 = alloca i64, align 8
  %146 = alloca %"class.std::__1::allocator.4"*, align 8
  %147 = alloca %"struct.std::__1::__hash_node"*, align 8
  %148 = alloca i64, align 8
  %149 = alloca %"class.std::__1::allocator.4"*, align 8
  %150 = alloca %"struct.std::__1::pair"*, align 8
  %151 = alloca %"class.std::__1::allocator.4"*, align 8
  %152 = alloca %"struct.std::__1::pair"*, align 8
  %153 = alloca %"struct.std::__1::integral_constant", align 1
  %154 = alloca %"struct.std::__1::__has_destroy", align 1
  %155 = alloca %"struct.std::__1::pair"*, align 8
  %156 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %157 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %158 = alloca %"struct.std::__1::__hash_node"*, align 8
  %159 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.134"*, align 8
  %160 = alloca %"class.std::__1::__compressed_pair.133"*, align 8
  %161 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %162 = alloca %"struct.std::__1::__hash_node"*, align 8
  %163 = alloca %"struct.std::__1::__hash_node"*, align 8
  %164 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %165 = alloca %"class.std::__1::unique_ptr.132"*, align 8
  %166 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %167 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %168 = alloca %"class.std::__1::__hash_table"*, align 8
  %169 = alloca %"class.std::__1::__hash_table"*, align 8
  %170 = alloca i64, align 8
  %171 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %172 = alloca %"class.std::__1::tuple"*, align 8
  %173 = alloca %"class.std::__1::tuple.131"*, align 8
  %174 = alloca %"class.std::__1::allocator.4"*, align 8
  %175 = alloca i1, align 1
  %176 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %177 = alloca i8*
  %178 = alloca i32
  store %"class.std::__1::__hash_table"* %1, %"class.std::__1::__hash_table"** %169, align 8
  store i64 %2, i64* %170, align 8
  store %"struct.std::__1::piecewise_construct_t"* %3, %"struct.std::__1::piecewise_construct_t"** %171, align 8
  store %"class.std::__1::tuple"* %4, %"class.std::__1::tuple"** %172, align 8
  store %"class.std::__1::tuple.131"* %5, %"class.std::__1::tuple.131"** %173, align 8
  %179 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %169, align 8
  store %"class.std::__1::__hash_table"* %179, %"class.std::__1::__hash_table"** %168, align 8
  %180 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %168, align 8
  %181 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %180, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.2"* %181, %"class.std::__1::__compressed_pair.2"** %167, align 8
  %182 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %167, align 8
  %183 = bitcast %"class.std::__1::__compressed_pair.2"* %182 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %183, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %166, align 8
  %184 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %166, align 8
  %185 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.3"* %184 to %"class.std::__1::allocator.4"*
  store %"class.std::__1::allocator.4"* %185, %"class.std::__1::allocator.4"** %174, align 8
  store i1 false, i1* %175, align 1
  %186 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %174, align 8
  store %"class.std::__1::allocator.4"* %186, %"class.std::__1::allocator.4"** %136, align 8
  store i64 1, i64* %137, align 8
  %187 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %136, align 8
  %188 = load i64, i64* %137, align 8
  store %"class.std::__1::allocator.4"* %187, %"class.std::__1::allocator.4"** %133, align 8
  store i64 %188, i64* %134, align 8
  store i8* null, i8** %135, align 8
  %189 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %133, align 8
  %190 = load i64, i64* %134, align 8
  %191 = mul i64 %190, 48
  store i64 %191, i64* %132, align 8
  %192 = load i64, i64* %132, align 8
  %193 = call i8* @_Znwm(i64 %192) #17
  %194 = bitcast i8* %193 to %"struct.std::__1::__hash_node"*
  %195 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %174, align 8
  store %"class.std::__1::__hash_node_destructor"* %176, %"class.std::__1::__hash_node_destructor"** %129, align 8
  store %"class.std::__1::allocator.4"* %195, %"class.std::__1::allocator.4"** %130, align 8
  store i8 0, i8* %131, align 1
  %196 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %129, align 8
  %197 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %130, align 8
  %198 = load i8, i8* %131, align 1
  %199 = trunc i8 %198 to i1
  store %"class.std::__1::__hash_node_destructor"* %196, %"class.std::__1::__hash_node_destructor"** %126, align 8
  store %"class.std::__1::allocator.4"* %197, %"class.std::__1::allocator.4"** %127, align 8
  %200 = zext i1 %199 to i8
  store i8 %200, i8* %128, align 1
  %201 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %126, align 8
  %202 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %201, i32 0, i32 0
  %203 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %127, align 8
  store %"class.std::__1::allocator.4"* %203, %"class.std::__1::allocator.4"** %202, align 8
  %204 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %201, i32 0, i32 1
  %205 = load i8, i8* %128, align 1
  %206 = trunc i8 %205 to i1
  %207 = zext i1 %206 to i8
  store i8 %207, i8* %204, align 8
  store %"class.std::__1::unique_ptr.132"* %0, %"class.std::__1::unique_ptr.132"** %123, align 8
  store %"struct.std::__1::__hash_node"* %194, %"struct.std::__1::__hash_node"** %124, align 8
  store %"class.std::__1::__hash_node_destructor"* %176, %"class.std::__1::__hash_node_destructor"** %125, align 8
  %208 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %123, align 8
  %209 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %124, align 8
  %210 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %125, align 8
  store %"class.std::__1::unique_ptr.132"* %208, %"class.std::__1::unique_ptr.132"** %119, align 8
  store %"struct.std::__1::__hash_node"* %209, %"struct.std::__1::__hash_node"** %120, align 8
  store %"class.std::__1::__hash_node_destructor"* %210, %"class.std::__1::__hash_node_destructor"** %121, align 8
  %211 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %119, align 8
  %212 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %211, i32 0, i32 0
  %213 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %120, align 8
  %214 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %121, align 8
  store %"class.std::__1::__hash_node_destructor"* %214, %"class.std::__1::__hash_node_destructor"** %118, align 8
  %215 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %118, align 8
  %216 = bitcast %"class.std::__1::__hash_node_destructor"* %122 to i8*
  %217 = bitcast %"class.std::__1::__hash_node_destructor"* %215 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %216, i8* %217, i64 16, i32 8, i1 false) #2
  %218 = bitcast %"class.std::__1::__hash_node_destructor"* %122 to { %"class.std::__1::allocator.4"*, i8 }*
  %219 = getelementptr inbounds { %"class.std::__1::allocator.4"*, i8 }, { %"class.std::__1::allocator.4"*, i8 }* %218, i32 0, i32 0
  %220 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %219, align 8
  %221 = getelementptr inbounds { %"class.std::__1::allocator.4"*, i8 }, { %"class.std::__1::allocator.4"*, i8 }* %218, i32 0, i32 1
  %222 = load i8, i8* %221, align 8
  %223 = bitcast %"class.std::__1::__hash_node_destructor"* %115 to { %"class.std::__1::allocator.4"*, i8 }*
  %224 = getelementptr inbounds { %"class.std::__1::allocator.4"*, i8 }, { %"class.std::__1::allocator.4"*, i8 }* %223, i32 0, i32 0
  store %"class.std::__1::allocator.4"* %220, %"class.std::__1::allocator.4"** %224, align 8
  %225 = getelementptr inbounds { %"class.std::__1::allocator.4"*, i8 }, { %"class.std::__1::allocator.4"*, i8 }* %223, i32 0, i32 1
  store i8 %222, i8* %225, align 8
  store %"class.std::__1::__compressed_pair.133"* %212, %"class.std::__1::__compressed_pair.133"** %116, align 8
  store %"struct.std::__1::__hash_node"* %213, %"struct.std::__1::__hash_node"** %117, align 8
  %226 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %116, align 8
  %227 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %117, align 8
  %228 = bitcast %"class.std::__1::__hash_node_destructor"* %115 to { %"class.std::__1::allocator.4"*, i8 }*
  %229 = getelementptr inbounds { %"class.std::__1::allocator.4"*, i8 }, { %"class.std::__1::allocator.4"*, i8 }* %228, i32 0, i32 0
  %230 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %229, align 8
  %231 = getelementptr inbounds { %"class.std::__1::allocator.4"*, i8 }, { %"class.std::__1::allocator.4"*, i8 }* %228, i32 0, i32 1
  %232 = load i8, i8* %231, align 8
  %233 = bitcast %"class.std::__1::__hash_node_destructor"* %111 to { %"class.std::__1::allocator.4"*, i8 }*
  %234 = getelementptr inbounds { %"class.std::__1::allocator.4"*, i8 }, { %"class.std::__1::allocator.4"*, i8 }* %233, i32 0, i32 0
  store %"class.std::__1::allocator.4"* %230, %"class.std::__1::allocator.4"** %234, align 8
  %235 = getelementptr inbounds { %"class.std::__1::allocator.4"*, i8 }, { %"class.std::__1::allocator.4"*, i8 }* %233, i32 0, i32 1
  store i8 %232, i8* %235, align 8
  store %"class.std::__1::__compressed_pair.133"* %226, %"class.std::__1::__compressed_pair.133"** %112, align 8
  store %"struct.std::__1::__hash_node"* %227, %"struct.std::__1::__hash_node"** %113, align 8
  %236 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %112, align 8
  %237 = bitcast %"class.std::__1::__compressed_pair.133"* %236 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"struct.std::__1::__hash_node"** %113, %"struct.std::__1::__hash_node"*** %110, align 8
  %238 = load %"struct.std::__1::__hash_node"**, %"struct.std::__1::__hash_node"*** %110, align 8
  %239 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %238, align 8
  store %"class.std::__1::__hash_node_destructor"* %111, %"class.std::__1::__hash_node_destructor"** %104, align 8
  %240 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %104, align 8
  %241 = bitcast %"class.std::__1::__hash_node_destructor"* %114 to i8*
  %242 = bitcast %"class.std::__1::__hash_node_destructor"* %240 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %241, i8* %242, i64 16, i32 8, i1 false) #2
  %243 = bitcast %"class.std::__1::__hash_node_destructor"* %114 to { %"class.std::__1::allocator.4"*, i8 }*
  %244 = getelementptr inbounds { %"class.std::__1::allocator.4"*, i8 }, { %"class.std::__1::allocator.4"*, i8 }* %243, i32 0, i32 0
  %245 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %244, align 8
  %246 = getelementptr inbounds { %"class.std::__1::allocator.4"*, i8 }, { %"class.std::__1::allocator.4"*, i8 }* %243, i32 0, i32 1
  %247 = load i8, i8* %246, align 8
  %248 = bitcast %"class.std::__1::__hash_node_destructor"* %107 to { %"class.std::__1::allocator.4"*, i8 }*
  %249 = getelementptr inbounds { %"class.std::__1::allocator.4"*, i8 }, { %"class.std::__1::allocator.4"*, i8 }* %248, i32 0, i32 0
  store %"class.std::__1::allocator.4"* %245, %"class.std::__1::allocator.4"** %249, align 8
  %250 = getelementptr inbounds { %"class.std::__1::allocator.4"*, i8 }, { %"class.std::__1::allocator.4"*, i8 }* %248, i32 0, i32 1
  store i8 %247, i8* %250, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %237, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %108, align 8
  store %"struct.std::__1::__hash_node"* %239, %"struct.std::__1::__hash_node"** %109, align 8
  %251 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %108, align 8
  %252 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %251, i32 0, i32 0
  store %"struct.std::__1::__hash_node"** %109, %"struct.std::__1::__hash_node"*** %106, align 8
  %253 = load %"struct.std::__1::__hash_node"**, %"struct.std::__1::__hash_node"*** %106, align 8
  %254 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %253, align 8
  store %"struct.std::__1::__hash_node"* %254, %"struct.std::__1::__hash_node"** %252, align 8
  %255 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %251, i32 0, i32 1
  store %"class.std::__1::__hash_node_destructor"* %107, %"class.std::__1::__hash_node_destructor"** %105, align 8
  %256 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %105, align 8
  %257 = bitcast %"class.std::__1::__hash_node_destructor"* %255 to i8*
  %258 = bitcast %"class.std::__1::__hash_node_destructor"* %256 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %257, i8* %258, i64 16, i32 8, i1 false) #2
  %259 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %174, align 8
  store %"class.std::__1::unique_ptr.132"* %0, %"class.std::__1::unique_ptr.132"** %75, align 8
  %260 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %75, align 8
  %261 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %260, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %261, %"class.std::__1::__compressed_pair.133"** %74, align 8
  %262 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %74, align 8
  %263 = bitcast %"class.std::__1::__compressed_pair.133"* %262 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %263, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %73, align 8
  %264 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %73, align 8
  %265 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %264, i32 0, i32 0
  %266 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %265, align 8
  %267 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %266, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %267, %"union.std::__1::__hash_value_type"** %72, align 8
  %268 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %72, align 8
  %269 = bitcast %"union.std::__1::__hash_value_type"* %268 to %"struct.std::__1::pair"*
  store %"struct.std::__1::pair"* %269, %"struct.std::__1::pair"** %71, align 8
  %270 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %71, align 8
  %271 = bitcast %"struct.std::__1::pair"* %270 to i8*
  br label %272

; <label>:272:                                    ; preds = %6
  %273 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %171, align 8
  store %"struct.std::__1::piecewise_construct_t"* %273, %"struct.std::__1::piecewise_construct_t"** %9, align 8
  %274 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %9, align 8
  %275 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %172, align 8
  store %"class.std::__1::tuple"* %275, %"class.std::__1::tuple"** %7, align 8
  %276 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %7, align 8
  %277 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %173, align 8
  store %"class.std::__1::tuple.131"* %277, %"class.std::__1::tuple.131"** %8, align 8
  %278 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %8, align 8
  store %"class.std::__1::allocator.4"* %259, %"class.std::__1::allocator.4"** %55, align 8
  store %"struct.std::__1::pair"* %270, %"struct.std::__1::pair"** %56, align 8
  store %"struct.std::__1::piecewise_construct_t"* %274, %"struct.std::__1::piecewise_construct_t"** %57, align 8
  store %"class.std::__1::tuple"* %276, %"class.std::__1::tuple"** %58, align 8
  store %"class.std::__1::tuple.131"* %278, %"class.std::__1::tuple.131"** %59, align 8
  %279 = bitcast %"struct.std::__1::__has_construct"* %61 to %"struct.std::__1::integral_constant.128"*
  %280 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %55, align 8
  %281 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %56, align 8
  %282 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %57, align 8
  store %"struct.std::__1::piecewise_construct_t"* %282, %"struct.std::__1::piecewise_construct_t"** %54, align 8
  %283 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %54, align 8
  %284 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %58, align 8
  store %"class.std::__1::tuple"* %284, %"class.std::__1::tuple"** %11, align 8
  %285 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %11, align 8
  %286 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %59, align 8
  store %"class.std::__1::tuple.131"* %286, %"class.std::__1::tuple.131"** %10, align 8
  %287 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %10, align 8
  store %"class.std::__1::allocator.4"* %280, %"class.std::__1::allocator.4"** %49, align 8
  store %"struct.std::__1::pair"* %281, %"struct.std::__1::pair"** %50, align 8
  store %"struct.std::__1::piecewise_construct_t"* %283, %"struct.std::__1::piecewise_construct_t"** %51, align 8
  store %"class.std::__1::tuple"* %285, %"class.std::__1::tuple"** %52, align 8
  store %"class.std::__1::tuple.131"* %287, %"class.std::__1::tuple.131"** %53, align 8
  %288 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %49, align 8
  %289 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %50, align 8
  %290 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %51, align 8
  store %"struct.std::__1::piecewise_construct_t"* %290, %"struct.std::__1::piecewise_construct_t"** %47, align 8
  %291 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %47, align 8
  %292 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %52, align 8
  store %"class.std::__1::tuple"* %292, %"class.std::__1::tuple"** %13, align 8
  %293 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %13, align 8
  %294 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %53, align 8
  store %"class.std::__1::tuple.131"* %294, %"class.std::__1::tuple.131"** %12, align 8
  %295 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %12, align 8
  store %"class.std::__1::allocator.4"* %288, %"class.std::__1::allocator.4"** %39, align 8
  store %"struct.std::__1::pair"* %289, %"struct.std::__1::pair"** %40, align 8
  store %"struct.std::__1::piecewise_construct_t"* %291, %"struct.std::__1::piecewise_construct_t"** %41, align 8
  store %"class.std::__1::tuple"* %293, %"class.std::__1::tuple"** %42, align 8
  store %"class.std::__1::tuple.131"* %295, %"class.std::__1::tuple.131"** %43, align 8
  %296 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %39, align 8
  %297 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %40, align 8
  %298 = bitcast %"struct.std::__1::pair"* %297 to i8*
  %299 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %41, align 8
  store %"struct.std::__1::piecewise_construct_t"* %299, %"struct.std::__1::piecewise_construct_t"** %38, align 8
  %300 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %38, align 8
  %301 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %42, align 8
  store %"class.std::__1::tuple"* %301, %"class.std::__1::tuple"** %15, align 8
  %302 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %15, align 8
  %303 = bitcast %"class.std::__1::tuple"* %45 to i8*
  %304 = bitcast %"class.std::__1::tuple"* %302 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %303, i8* %304, i64 8, i32 8, i1 false)
  %305 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %43, align 8
  store %"class.std::__1::tuple.131"* %305, %"class.std::__1::tuple.131"** %14, align 8
  %306 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %14, align 8
  %307 = getelementptr inbounds %"class.std::__1::tuple", %"class.std::__1::tuple"* %45, i32 0, i32 0
  %308 = getelementptr inbounds %"struct.std::__1::__tuple_impl", %"struct.std::__1::__tuple_impl"* %307, i32 0, i32 0
  %309 = getelementptr inbounds %"class.std::__1::__tuple_leaf", %"class.std::__1::__tuple_leaf"* %308, i32 0, i32 0
  %310 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %309, align 8
  %311 = getelementptr inbounds %"class.std::__1::tuple", %"class.std::__1::tuple"* %32, i32 0, i32 0
  %312 = getelementptr inbounds %"struct.std::__1::__tuple_impl", %"struct.std::__1::__tuple_impl"* %311, i32 0, i32 0
  %313 = getelementptr inbounds %"class.std::__1::__tuple_leaf", %"class.std::__1::__tuple_leaf"* %312, i32 0, i32 0
  store %"class.std::__1::basic_string"* %310, %"class.std::__1::basic_string"** %313, align 8
  store %"struct.std::__1::pair"* %297, %"struct.std::__1::pair"** %34, align 8
  %314 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %34, align 8
  store %"struct.std::__1::pair"* %314, %"struct.std::__1::pair"** %28, align 8
  store %"class.std::__1::tuple"* %32, %"class.std::__1::tuple"** %29, align 8
  store %"class.std::__1::tuple.131"* %33, %"class.std::__1::tuple.131"** %30, align 8
  %315 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %28, align 8
  %316 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %29, align 8
  %317 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %30, align 8
  store %"struct.std::__1::pair"* %315, %"struct.std::__1::pair"** %22, align 8
  store %"class.std::__1::tuple"* %316, %"class.std::__1::tuple"** %23, align 8
  store %"class.std::__1::tuple.131"* %317, %"class.std::__1::tuple.131"** %24, align 8
  %318 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %22, align 8
  %319 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %318, i32 0, i32 0
  %320 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %23, align 8
  store %"class.std::__1::tuple"* %320, %"class.std::__1::tuple"** %18, align 8
  %321 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %18, align 8
  %322 = getelementptr inbounds %"class.std::__1::tuple", %"class.std::__1::tuple"* %321, i32 0, i32 0
  %323 = bitcast %"struct.std::__1::__tuple_impl"* %322 to %"class.std::__1::__tuple_leaf"*
  store %"class.std::__1::__tuple_leaf"* %323, %"class.std::__1::__tuple_leaf"** %17, align 8
  %324 = load %"class.std::__1::__tuple_leaf"*, %"class.std::__1::__tuple_leaf"** %17, align 8
  %325 = getelementptr inbounds %"class.std::__1::__tuple_leaf", %"class.std::__1::__tuple_leaf"* %324, i32 0, i32 0
  %326 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %325, align 8
  store %"class.std::__1::basic_string"* %326, %"class.std::__1::basic_string"** %16, align 8
  %327 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %16, align 8
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %319, %"class.std::__1::basic_string"* dereferenceable(24) %327)
          to label %328 unwind label %357

; <label>:328:                                    ; preds = %272
  %329 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %318, i32 0, i32 1
  store %"class.llvm::StructType"* null, %"class.llvm::StructType"** %329, align 8
  br label %330

; <label>:330:                                    ; preds = %328
  store %"class.std::__1::unique_ptr.132"* %0, %"class.std::__1::unique_ptr.132"** %64, align 8
  %331 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %64, align 8
  %332 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %331, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %332, %"class.std::__1::__compressed_pair.133"** %63, align 8
  %333 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %63, align 8
  %334 = bitcast %"class.std::__1::__compressed_pair.133"* %333 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %334, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %62, align 8
  %335 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %62, align 8
  %336 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %335, i32 0, i32 1
  %337 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %336, i32 0, i32 1
  store i8 1, i8* %337, align 8
  %338 = load i64, i64* %170, align 8
  store %"class.std::__1::unique_ptr.132"* %0, %"class.std::__1::unique_ptr.132"** %67, align 8
  %339 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %67, align 8
  %340 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %339, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %340, %"class.std::__1::__compressed_pair.133"** %66, align 8
  %341 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %66, align 8
  %342 = bitcast %"class.std::__1::__compressed_pair.133"* %341 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %342, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %65, align 8
  %343 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %65, align 8
  %344 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %343, i32 0, i32 0
  %345 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %344, align 8
  %346 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %345, i32 0, i32 1
  store i64 %338, i64* %346, align 8
  store %"class.std::__1::unique_ptr.132"* %0, %"class.std::__1::unique_ptr.132"** %70, align 8
  %347 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %70, align 8
  %348 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %347, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %348, %"class.std::__1::__compressed_pair.133"** %69, align 8
  %349 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %69, align 8
  %350 = bitcast %"class.std::__1::__compressed_pair.133"* %349 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %350, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %68, align 8
  %351 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %68, align 8
  %352 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %351, i32 0, i32 0
  %353 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %352, align 8
  %354 = bitcast %"struct.std::__1::__hash_node"* %353 to %"struct.std::__1::__hash_node_base"*
  %355 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %354, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"* null, %"struct.std::__1::__hash_node_base"** %355, align 8
  store i1 true, i1* %175, align 1
  %356 = load i1, i1* %175, align 1
  br i1 %356, label %476, label %418

; <label>:357:                                    ; preds = %272
  %358 = landingpad { i8*, i32 }
          cleanup
  %359 = extractvalue { i8*, i32 } %358, 0
  store i8* %359, i8** %177, align 8
  %360 = extractvalue { i8*, i32 } %358, 1
  store i32 %360, i32* %178, align 4
  store %"class.std::__1::unique_ptr.132"* %0, %"class.std::__1::unique_ptr.132"** %103, align 8
  %361 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %103, align 8
  store %"class.std::__1::unique_ptr.132"* %361, %"class.std::__1::unique_ptr.132"** %102, align 8
  %362 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %102, align 8
  store %"class.std::__1::unique_ptr.132"* %362, %"class.std::__1::unique_ptr.132"** %99, align 8
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %100, align 8
  %363 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %99, align 8
  %364 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %363, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %364, %"class.std::__1::__compressed_pair.133"** %98, align 8
  %365 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %98, align 8
  %366 = bitcast %"class.std::__1::__compressed_pair.133"* %365 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %366, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %97, align 8
  %367 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %97, align 8
  %368 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %367, i32 0, i32 0
  %369 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %368, align 8
  store %"struct.std::__1::__hash_node"* %369, %"struct.std::__1::__hash_node"** %101, align 8
  %370 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %100, align 8
  %371 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %363, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %371, %"class.std::__1::__compressed_pair.133"** %79, align 8
  %372 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %79, align 8
  %373 = bitcast %"class.std::__1::__compressed_pair.133"* %372 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %373, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %78, align 8
  %374 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %78, align 8
  %375 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %374, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* %370, %"struct.std::__1::__hash_node"** %375, align 8
  %376 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %101, align 8
  %377 = icmp ne %"struct.std::__1::__hash_node"* %376, null
  br i1 %377, label %378, label %417

; <label>:378:                                    ; preds = %357
  %379 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %363, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %379, %"class.std::__1::__compressed_pair.133"** %77, align 8
  %380 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %77, align 8
  %381 = bitcast %"class.std::__1::__compressed_pair.133"* %380 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %381, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %76, align 8
  %382 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %76, align 8
  %383 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %382, i32 0, i32 1
  %384 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %101, align 8
  store %"class.std::__1::__hash_node_destructor"* %383, %"class.std::__1::__hash_node_destructor"** %95, align 8
  store %"struct.std::__1::__hash_node"* %384, %"struct.std::__1::__hash_node"** %96, align 8
  %385 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %95, align 8
  %386 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %385, i32 0, i32 1
  %387 = load i8, i8* %386, align 8
  %388 = trunc i8 %387 to i1
  br i1 %388, label %389, label %402

; <label>:389:                                    ; preds = %378
  %390 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %385, i32 0, i32 0
  %391 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %390, align 8
  %392 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %96, align 8
  %393 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %392, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %393, %"union.std::__1::__hash_value_type"** %94, align 8
  %394 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %94, align 8
  %395 = bitcast %"union.std::__1::__hash_value_type"* %394 to %"struct.std::__1::pair"*
  store %"struct.std::__1::pair"* %395, %"struct.std::__1::pair"** %93, align 8
  %396 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %93, align 8
  %397 = bitcast %"struct.std::__1::pair"* %396 to i8*
  store %"class.std::__1::allocator.4"* %391, %"class.std::__1::allocator.4"** %89, align 8
  store %"struct.std::__1::pair"* %396, %"struct.std::__1::pair"** %90, align 8
  %398 = bitcast %"struct.std::__1::__has_destroy"* %92 to %"struct.std::__1::integral_constant"*
  %399 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %89, align 8
  %400 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %90, align 8
  store %"class.std::__1::allocator.4"* %399, %"class.std::__1::allocator.4"** %87, align 8
  store %"struct.std::__1::pair"* %400, %"struct.std::__1::pair"** %88, align 8
  %401 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %88, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEED1Ev(%"struct.std::__1::pair"* %401) #2
  br label %402

; <label>:402:                                    ; preds = %389, %378
  %403 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %96, align 8
  %404 = icmp ne %"struct.std::__1::__hash_node"* %403, null
  br i1 %404, label %405, label %416

; <label>:405:                                    ; preds = %402
  %406 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %385, i32 0, i32 0
  %407 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %406, align 8
  %408 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %96, align 8
  store %"class.std::__1::allocator.4"* %407, %"class.std::__1::allocator.4"** %84, align 8
  store %"struct.std::__1::__hash_node"* %408, %"struct.std::__1::__hash_node"** %85, align 8
  store i64 1, i64* %86, align 8
  %409 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %84, align 8
  %410 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %85, align 8
  %411 = load i64, i64* %86, align 8
  store %"class.std::__1::allocator.4"* %409, %"class.std::__1::allocator.4"** %81, align 8
  store %"struct.std::__1::__hash_node"* %410, %"struct.std::__1::__hash_node"** %82, align 8
  store i64 %411, i64* %83, align 8
  %412 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %81, align 8
  %413 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %82, align 8
  %414 = bitcast %"struct.std::__1::__hash_node"* %413 to i8*
  store i8* %414, i8** %80, align 8
  %415 = load i8*, i8** %80, align 8
  call void @_ZdlPv(i8* %415) #14
  br label %416

; <label>:416:                                    ; preds = %405, %402
  br label %417

; <label>:417:                                    ; preds = %357, %416
  br label %477

; <label>:418:                                    ; preds = %330
  store %"class.std::__1::unique_ptr.132"* %0, %"class.std::__1::unique_ptr.132"** %165, align 8
  %419 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %165, align 8
  store %"class.std::__1::unique_ptr.132"* %419, %"class.std::__1::unique_ptr.132"** %164, align 8
  %420 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %164, align 8
  store %"class.std::__1::unique_ptr.132"* %420, %"class.std::__1::unique_ptr.132"** %161, align 8
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %162, align 8
  %421 = load %"class.std::__1::unique_ptr.132"*, %"class.std::__1::unique_ptr.132"** %161, align 8
  %422 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %421, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %422, %"class.std::__1::__compressed_pair.133"** %160, align 8
  %423 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %160, align 8
  %424 = bitcast %"class.std::__1::__compressed_pair.133"* %423 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %424, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %159, align 8
  %425 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %159, align 8
  %426 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %425, i32 0, i32 0
  %427 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %426, align 8
  store %"struct.std::__1::__hash_node"* %427, %"struct.std::__1::__hash_node"** %163, align 8
  %428 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %162, align 8
  %429 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %421, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %429, %"class.std::__1::__compressed_pair.133"** %141, align 8
  %430 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %141, align 8
  %431 = bitcast %"class.std::__1::__compressed_pair.133"* %430 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %431, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %140, align 8
  %432 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %140, align 8
  %433 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %432, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* %428, %"struct.std::__1::__hash_node"** %433, align 8
  %434 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %163, align 8
  %435 = icmp ne %"struct.std::__1::__hash_node"* %434, null
  br i1 %435, label %436, label %475

; <label>:436:                                    ; preds = %418
  %437 = getelementptr inbounds %"class.std::__1::unique_ptr.132", %"class.std::__1::unique_ptr.132"* %421, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.133"* %437, %"class.std::__1::__compressed_pair.133"** %139, align 8
  %438 = load %"class.std::__1::__compressed_pair.133"*, %"class.std::__1::__compressed_pair.133"** %139, align 8
  %439 = bitcast %"class.std::__1::__compressed_pair.133"* %438 to %"class.std::__1::__libcpp_compressed_pair_imp.134"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.134"* %439, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %138, align 8
  %440 = load %"class.std::__1::__libcpp_compressed_pair_imp.134"*, %"class.std::__1::__libcpp_compressed_pair_imp.134"** %138, align 8
  %441 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.134", %"class.std::__1::__libcpp_compressed_pair_imp.134"* %440, i32 0, i32 1
  %442 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %163, align 8
  store %"class.std::__1::__hash_node_destructor"* %441, %"class.std::__1::__hash_node_destructor"** %157, align 8
  store %"struct.std::__1::__hash_node"* %442, %"struct.std::__1::__hash_node"** %158, align 8
  %443 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %157, align 8
  %444 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %443, i32 0, i32 1
  %445 = load i8, i8* %444, align 8
  %446 = trunc i8 %445 to i1
  br i1 %446, label %447, label %460

; <label>:447:                                    ; preds = %436
  %448 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %443, i32 0, i32 0
  %449 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %448, align 8
  %450 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %158, align 8
  %451 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %450, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %451, %"union.std::__1::__hash_value_type"** %156, align 8
  %452 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %156, align 8
  %453 = bitcast %"union.std::__1::__hash_value_type"* %452 to %"struct.std::__1::pair"*
  store %"struct.std::__1::pair"* %453, %"struct.std::__1::pair"** %155, align 8
  %454 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %155, align 8
  %455 = bitcast %"struct.std::__1::pair"* %454 to i8*
  store %"class.std::__1::allocator.4"* %449, %"class.std::__1::allocator.4"** %151, align 8
  store %"struct.std::__1::pair"* %454, %"struct.std::__1::pair"** %152, align 8
  %456 = bitcast %"struct.std::__1::__has_destroy"* %154 to %"struct.std::__1::integral_constant"*
  %457 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %151, align 8
  %458 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %152, align 8
  store %"class.std::__1::allocator.4"* %457, %"class.std::__1::allocator.4"** %149, align 8
  store %"struct.std::__1::pair"* %458, %"struct.std::__1::pair"** %150, align 8
  %459 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %150, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEED1Ev(%"struct.std::__1::pair"* %459) #2
  br label %460

; <label>:460:                                    ; preds = %447, %436
  %461 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %158, align 8
  %462 = icmp ne %"struct.std::__1::__hash_node"* %461, null
  br i1 %462, label %463, label %474

; <label>:463:                                    ; preds = %460
  %464 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %443, i32 0, i32 0
  %465 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %464, align 8
  %466 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %158, align 8
  store %"class.std::__1::allocator.4"* %465, %"class.std::__1::allocator.4"** %146, align 8
  store %"struct.std::__1::__hash_node"* %466, %"struct.std::__1::__hash_node"** %147, align 8
  store i64 1, i64* %148, align 8
  %467 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %146, align 8
  %468 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %147, align 8
  %469 = load i64, i64* %148, align 8
  store %"class.std::__1::allocator.4"* %467, %"class.std::__1::allocator.4"** %143, align 8
  store %"struct.std::__1::__hash_node"* %468, %"struct.std::__1::__hash_node"** %144, align 8
  store i64 %469, i64* %145, align 8
  %470 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %143, align 8
  %471 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %144, align 8
  %472 = bitcast %"struct.std::__1::__hash_node"* %471 to i8*
  store i8* %472, i8** %142, align 8
  %473 = load i8*, i8** %142, align 8
  call void @_ZdlPv(i8* %473) #14
  br label %474

; <label>:474:                                    ; preds = %463, %460
  br label %475

; <label>:475:                                    ; preds = %418, %474
  br label %476

; <label>:476:                                    ; preds = %475, %330
  ret void

; <label>:477:                                    ; preds = %417
  %478 = load i8*, i8** %177, align 8
  %479 = load i32, i32* %178, align 4
  %480 = insertvalue { i8*, i32 } undef, i8* %478, 0
  %481 = insertvalue { i8*, i32 } %480, i32 %479, 1
  resume { i8*, i32 } %481
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE6rehashEm(%"class.std::__1::__hash_table"*, i64) #0 align 2 {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.9"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair.8"*, align 8
  %5 = alloca %"class.std::__1::__hash_table"*, align 8
  %6 = alloca float, align 4
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  %9 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.7"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair.6"*, align 8
  %11 = alloca %"class.std::__1::__hash_table"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.9"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair.8"*, align 8
  %14 = alloca %"class.std::__1::__hash_table"*, align 8
  %15 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.7"*, align 8
  %16 = alloca %"class.std::__1::__compressed_pair.6"*, align 8
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
  %29 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %30 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %31 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %32 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %33 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %34 = alloca %"class.std::__1::unique_ptr"*, align 8
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
  store %"class.std::__1::unique_ptr"* %56, %"class.std::__1::unique_ptr"** %34, align 8
  %57 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %34, align 8
  %58 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %57, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %58, %"class.std::__1::__compressed_pair"** %33, align 8
  %59 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %33, align 8
  %60 = bitcast %"class.std::__1::__compressed_pair"* %59 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %60, %"class.std::__1::__libcpp_compressed_pair_imp"** %32, align 8
  %61 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %32, align 8
  %62 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %61, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator"* %62, %"class.std::__1::__bucket_list_deallocator"** %31, align 8
  %63 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %31, align 8
  %64 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %63, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.0"* %64, %"class.std::__1::__compressed_pair.0"** %30, align 8
  %65 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %30, align 8
  %66 = bitcast %"class.std::__1::__compressed_pair.0"* %65 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %66, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %29, align 8
  %67 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %29, align 8
  %68 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.1", %"class.std::__1::__libcpp_compressed_pair_imp.1"* %67, i32 0, i32 0
  %69 = load i64, i64* %68, align 8
  store i64 %69, i64* %38, align 8
  %70 = load i64, i64* %37, align 8
  %71 = load i64, i64* %38, align 8
  %72 = icmp ugt i64 %70, %71
  br i1 %72, label %73, label %75

; <label>:73:                                     ; preds = %54
  %74 = load i64, i64* %37, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE8__rehashEm(%"class.std::__1::__hash_table"* %40, i64 %74)
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
  store %"class.std::__1::__compressed_pair.6"* %94, %"class.std::__1::__compressed_pair.6"** %16, align 8
  %95 = load %"class.std::__1::__compressed_pair.6"*, %"class.std::__1::__compressed_pair.6"** %16, align 8
  %96 = bitcast %"class.std::__1::__compressed_pair.6"* %95 to %"class.std::__1::__libcpp_compressed_pair_imp.7"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.7"* %96, %"class.std::__1::__libcpp_compressed_pair_imp.7"** %15, align 8
  %97 = load %"class.std::__1::__libcpp_compressed_pair_imp.7"*, %"class.std::__1::__libcpp_compressed_pair_imp.7"** %15, align 8
  %98 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.7", %"class.std::__1::__libcpp_compressed_pair_imp.7"* %97, i32 0, i32 0
  %99 = load i64, i64* %98, align 8
  %100 = uitofp i64 %99 to float
  store %"class.std::__1::__hash_table"* %40, %"class.std::__1::__hash_table"** %5, align 8
  %101 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %5, align 8
  %102 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %101, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.8"* %102, %"class.std::__1::__compressed_pair.8"** %4, align 8
  %103 = load %"class.std::__1::__compressed_pair.8"*, %"class.std::__1::__compressed_pair.8"** %4, align 8
  %104 = bitcast %"class.std::__1::__compressed_pair.8"* %103 to %"class.std::__1::__libcpp_compressed_pair_imp.9"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.9"* %104, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %3, align 8
  %105 = load %"class.std::__1::__libcpp_compressed_pair_imp.9"*, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %3, align 8
  %106 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.9", %"class.std::__1::__libcpp_compressed_pair_imp.9"* %105, i32 0, i32 0
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
  %115 = call i64 @llvm.ctlz.i64(i64 %114, i1 true) #2
  %116 = trunc i64 %115 to i32
  %117 = sext i32 %116 to i64
  %118 = sub i64 64, %117
  %119 = shl i64 1, %118
  br label %141

; <label>:120:                                    ; preds = %90
  store %"class.std::__1::__hash_table"* %40, %"class.std::__1::__hash_table"** %11, align 8
  %121 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %11, align 8
  %122 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %121, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.6"* %122, %"class.std::__1::__compressed_pair.6"** %10, align 8
  %123 = load %"class.std::__1::__compressed_pair.6"*, %"class.std::__1::__compressed_pair.6"** %10, align 8
  %124 = bitcast %"class.std::__1::__compressed_pair.6"* %123 to %"class.std::__1::__libcpp_compressed_pair_imp.7"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.7"* %124, %"class.std::__1::__libcpp_compressed_pair_imp.7"** %9, align 8
  %125 = load %"class.std::__1::__libcpp_compressed_pair_imp.7"*, %"class.std::__1::__libcpp_compressed_pair_imp.7"** %9, align 8
  %126 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.7", %"class.std::__1::__libcpp_compressed_pair_imp.7"* %125, i32 0, i32 0
  %127 = load i64, i64* %126, align 8
  %128 = uitofp i64 %127 to float
  store %"class.std::__1::__hash_table"* %40, %"class.std::__1::__hash_table"** %14, align 8
  %129 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %14, align 8
  %130 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %129, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.8"* %130, %"class.std::__1::__compressed_pair.8"** %13, align 8
  %131 = load %"class.std::__1::__compressed_pair.8"*, %"class.std::__1::__compressed_pair.8"** %13, align 8
  %132 = bitcast %"class.std::__1::__compressed_pair.8"* %131 to %"class.std::__1::__libcpp_compressed_pair_imp.9"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.9"* %132, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %12, align 8
  %133 = load %"class.std::__1::__libcpp_compressed_pair_imp.9"*, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %12, align 8
  %134 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.9", %"class.std::__1::__libcpp_compressed_pair_imp.9"* %133, i32 0, i32 0
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
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE8__rehashEm(%"class.std::__1::__hash_table"* %40, i64 %164)
  br label %165

; <label>:165:                                    ; preds = %163, %157
  br label %166

; <label>:166:                                    ; preds = %165, %75
  br label %167

; <label>:167:                                    ; preds = %166, %73
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNKSt3__14hashINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEclERKS6_(%"struct.std::__1::hash"*, %"class.std::__1::basic_string"* dereferenceable(24)) #5 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %5 = alloca %"class.std::__1::basic_string"*, align 8
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %8 = alloca %"class.std::__1::basic_string"*, align 8
  %9 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca i8*, align 8
  %14 = alloca i8*, align 8
  %15 = alloca %"struct.std::__1::__murmur2_or_cityhash", align 1
  %16 = alloca i8*, align 8
  %17 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %18 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %19 = alloca %"class.std::__1::basic_string"*, align 8
  %20 = alloca i8*, align 8
  %21 = alloca i8*, align 8
  %22 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %23 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %24 = alloca %"class.std::__1::basic_string"*, align 8
  %25 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %26 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %27 = alloca %"class.std::__1::basic_string"*, align 8
  %28 = alloca %"class.std::__1::basic_string"*, align 8
  %29 = alloca %"class.std::__1::basic_string"*, align 8
  %30 = alloca i8*, align 8
  %31 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %32 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %33 = alloca %"class.std::__1::basic_string"*, align 8
  %34 = alloca i8*, align 8
  %35 = alloca i8*, align 8
  %36 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %37 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %38 = alloca %"class.std::__1::basic_string"*, align 8
  %39 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %40 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
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
  store %"class.std::__1::__compressed_pair.36"* %51, %"class.std::__1::__compressed_pair.36"** %40, align 8
  %52 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %40, align 8
  %53 = bitcast %"class.std::__1::__compressed_pair.36"* %52 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %53, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %39, align 8
  %54 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %39, align 8
  %55 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %54, i32 0, i32 0
  %56 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %55, i32 0, i32 0
  %57 = bitcast %union.anon* %56 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %58 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %57, i32 0, i32 0
  %59 = bitcast %union.anon.40* %58 to i8*
  %60 = load i8, i8* %59, align 8
  %61 = zext i8 %60 to i32
  %62 = and i32 %61, 1
  %63 = icmp ne i32 %62, 0
  br i1 %63, label %64, label %75

; <label>:64:                                     ; preds = %2
  store %"class.std::__1::basic_string"* %49, %"class.std::__1::basic_string"** %33, align 8
  %65 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8
  %66 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %65, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %66, %"class.std::__1::__compressed_pair.36"** %32, align 8
  %67 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %32, align 8
  %68 = bitcast %"class.std::__1::__compressed_pair.36"* %67 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %68, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %31, align 8
  %69 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %31, align 8
  %70 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %69, i32 0, i32 0
  %71 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %70, i32 0, i32 0
  %72 = bitcast %union.anon* %71 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %73 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %72, i32 0, i32 2
  %74 = load i8*, i8** %73, align 8
  br label %88

; <label>:75:                                     ; preds = %2
  store %"class.std::__1::basic_string"* %49, %"class.std::__1::basic_string"** %38, align 8
  %76 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8
  %77 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %76, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %77, %"class.std::__1::__compressed_pair.36"** %37, align 8
  %78 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %37, align 8
  %79 = bitcast %"class.std::__1::__compressed_pair.36"* %78 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %79, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %36, align 8
  %80 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %36, align 8
  %81 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %80, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair.36"* %95, %"class.std::__1::__compressed_pair.36"** %26, align 8
  %96 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %26, align 8
  %97 = bitcast %"class.std::__1::__compressed_pair.36"* %96 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %97, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %25, align 8
  %98 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %25, align 8
  %99 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %98, i32 0, i32 0
  %100 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %99, i32 0, i32 0
  %101 = bitcast %union.anon* %100 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %102 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %101, i32 0, i32 0
  %103 = bitcast %union.anon.40* %102 to i8*
  %104 = load i8, i8* %103, align 8
  %105 = zext i8 %104 to i32
  %106 = and i32 %105, 1
  %107 = icmp ne i32 %106, 0
  br i1 %107, label %108, label %119

; <label>:108:                                    ; preds = %88
  store %"class.std::__1::basic_string"* %93, %"class.std::__1::basic_string"** %19, align 8
  %109 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %19, align 8
  %110 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %109, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %110, %"class.std::__1::__compressed_pair.36"** %18, align 8
  %111 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %18, align 8
  %112 = bitcast %"class.std::__1::__compressed_pair.36"* %111 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %112, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %17, align 8
  %113 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %17, align 8
  %114 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %113, i32 0, i32 0
  %115 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %114, i32 0, i32 0
  %116 = bitcast %union.anon* %115 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %117 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %116, i32 0, i32 2
  %118 = load i8*, i8** %117, align 8
  br label %132

; <label>:119:                                    ; preds = %88
  store %"class.std::__1::basic_string"* %93, %"class.std::__1::basic_string"** %24, align 8
  %120 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8
  %121 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %120, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %121, %"class.std::__1::__compressed_pair.36"** %23, align 8
  %122 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %23, align 8
  %123 = bitcast %"class.std::__1::__compressed_pair.36"* %122 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %123, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %22, align 8
  %124 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %22, align 8
  %125 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %124, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair.36"* %138, %"class.std::__1::__compressed_pair.36"** %10, align 8
  %139 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %10, align 8
  %140 = bitcast %"class.std::__1::__compressed_pair.36"* %139 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %140, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %9, align 8
  %141 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %9, align 8
  %142 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %141, i32 0, i32 0
  %143 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %142, i32 0, i32 0
  %144 = bitcast %union.anon* %143 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %145 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %144, i32 0, i32 0
  %146 = bitcast %union.anon.40* %145 to i8*
  %147 = load i8, i8* %146, align 8
  %148 = zext i8 %147 to i32
  %149 = and i32 %148, 1
  %150 = icmp ne i32 %149, 0
  br i1 %150, label %151, label %162

; <label>:151:                                    ; preds = %132
  store %"class.std::__1::basic_string"* %136, %"class.std::__1::basic_string"** %5, align 8
  %152 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %5, align 8
  %153 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %152, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %153, %"class.std::__1::__compressed_pair.36"** %4, align 8
  %154 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %4, align 8
  %155 = bitcast %"class.std::__1::__compressed_pair.36"* %154 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %155, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %3, align 8
  %156 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %3, align 8
  %157 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %156, i32 0, i32 0
  %158 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %157, i32 0, i32 0
  %159 = bitcast %union.anon* %158 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %160 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %159, i32 0, i32 1
  %161 = load i64, i64* %160, align 8
  br label %177

; <label>:162:                                    ; preds = %132
  store %"class.std::__1::basic_string"* %136, %"class.std::__1::basic_string"** %8, align 8
  %163 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %8, align 8
  %164 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %163, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %164, %"class.std::__1::__compressed_pair.36"** %7, align 8
  %165 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %7, align 8
  %166 = bitcast %"class.std::__1::__compressed_pair.36"* %165 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %166, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %6, align 8
  %167 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %6, align 8
  %168 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %167, i32 0, i32 0
  %169 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %168, i32 0, i32 0
  %170 = bitcast %union.anon* %169 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %171 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %170, i32 0, i32 0
  %172 = bitcast %union.anon.40* %171 to i8*
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
  call void @__clang_call_terminate(i8* %191) #15
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
  %12 = alloca %"struct.std::__1::pair.135"*, align 8
  %13 = alloca %"struct.std::__1::pair.135"*, align 8
  %14 = alloca i8*, align 8
  %15 = alloca i64, align 8
  %16 = alloca i8*, align 8
  %17 = alloca i64, align 8
  %18 = alloca i64*, align 8
  %19 = alloca i64*, align 8
  %20 = alloca %"struct.std::__1::pair.135"*, align 8
  %21 = alloca %"struct.std::__1::pair.135"*, align 8
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
  %46 = alloca %"struct.std::__1::pair.135", align 8
  %47 = alloca %"struct.std::__1::pair.135", align 8
  %48 = alloca %"struct.std::__1::pair.135", align 8
  %49 = alloca %"struct.std::__1::pair.135", align 8
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %78, i8* %79, i64 8, i32 1, i1 false) #2
  %80 = load i64, i64* %37, align 8
  store i64 %80, i64* %43, align 8
  %81 = load i8*, i8** %42, align 8
  %82 = load i64, i64* %41, align 8
  %83 = getelementptr inbounds i8, i8* %81, i64 %82
  %84 = getelementptr inbounds i8, i8* %83, i64 -16
  store i8* %84, i8** %34, align 8
  %85 = bitcast i64* %35 to i8*
  %86 = load i8*, i8** %34, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %85, i8* %86, i64 8, i32 1, i1 false) #2
  %87 = load i64, i64* %35, align 8
  %88 = load i8*, i8** %42, align 8
  %89 = load i64, i64* %41, align 8
  %90 = getelementptr inbounds i8, i8* %88, i64 %89
  %91 = getelementptr inbounds i8, i8* %90, i64 -56
  store i8* %91, i8** %32, align 8
  %92 = bitcast i64* %33 to i8*
  %93 = load i8*, i8** %32, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %92, i8* %93, i64 8, i32 1, i1 false) #2
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %100, i8* %101, i64 8, i32 1, i1 false) #2
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %109, i8* %110, i64 8, i32 1, i1 false) #2
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
  %120 = bitcast %"struct.std::__1::pair.135"* %46 to { i64, i64 }*
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
  %133 = bitcast %"struct.std::__1::pair.135"* %47 to { i64, i64 }*
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %141, i8* %142, i64 8, i32 1, i1 false) #2
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
  %152 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %46, i32 0, i32 0
  %153 = load i64, i64* %152, align 8
  %154 = add i64 %151, %153
  %155 = load i8*, i8** %42, align 8
  %156 = getelementptr inbounds i8, i8* %155, i64 8
  store i8* %156, i8** %8, align 8
  %157 = bitcast i64* %9 to i8*
  %158 = load i8*, i8** %8, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %157, i8* %158, i64 8, i32 1, i1 false) #2
  %159 = load i64, i64* %9, align 8
  %160 = add i64 %154, %159
  %161 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %160, i32 37)
  %162 = mul i64 %161, -5435081209227447693
  store i64 %162, i64* %43, align 8
  %163 = load i64, i64* %44, align 8
  %164 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %46, i32 0, i32 1
  %165 = load i64, i64* %164, align 8
  %166 = add i64 %163, %165
  %167 = load i8*, i8** %42, align 8
  %168 = getelementptr inbounds i8, i8* %167, i64 48
  store i8* %168, i8** %6, align 8
  %169 = bitcast i64* %7 to i8*
  %170 = load i8*, i8** %6, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %169, i8* %170, i64 8, i32 1, i1 false) #2
  %171 = load i64, i64* %7, align 8
  %172 = add i64 %166, %171
  %173 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %172, i32 42)
  %174 = mul i64 %173, -5435081209227447693
  store i64 %174, i64* %44, align 8
  %175 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %47, i32 0, i32 1
  %176 = load i64, i64* %175, align 8
  %177 = load i64, i64* %43, align 8
  %178 = xor i64 %177, %176
  store i64 %178, i64* %43, align 8
  %179 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %46, i32 0, i32 0
  %180 = load i64, i64* %179, align 8
  %181 = load i8*, i8** %42, align 8
  %182 = getelementptr inbounds i8, i8* %181, i64 40
  store i8* %182, i8** %4, align 8
  %183 = bitcast i64* %5 to i8*
  %184 = load i8*, i8** %4, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %183, i8* %184, i64 8, i32 1, i1 false) #2
  %185 = load i64, i64* %5, align 8
  %186 = add i64 %180, %185
  %187 = load i64, i64* %44, align 8
  %188 = add i64 %187, %186
  store i64 %188, i64* %44, align 8
  %189 = load i64, i64* %45, align 8
  %190 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %47, i32 0, i32 0
  %191 = load i64, i64* %190, align 8
  %192 = add i64 %189, %191
  %193 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %192, i32 33)
  %194 = mul i64 %193, -5435081209227447693
  store i64 %194, i64* %45, align 8
  %195 = load i8*, i8** %42, align 8
  %196 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %46, i32 0, i32 1
  %197 = load i64, i64* %196, align 8
  %198 = mul i64 %197, -5435081209227447693
  %199 = load i64, i64* %43, align 8
  %200 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %47, i32 0, i32 0
  %201 = load i64, i64* %200, align 8
  %202 = add i64 %199, %201
  %203 = call { i64, i64 } @_ZNSt3__121__murmur2_or_cityhashImLm64EE29__weak_hash_len_32_with_seedsEPKcmm(i8* %195, i64 %198, i64 %202)
  %204 = bitcast %"struct.std::__1::pair.135"* %48 to { i64, i64 }*
  %205 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %204, i32 0, i32 0
  %206 = extractvalue { i64, i64 } %203, 0
  store i64 %206, i64* %205, align 8
  %207 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %204, i32 0, i32 1
  %208 = extractvalue { i64, i64 } %203, 1
  store i64 %208, i64* %207, align 8
  store %"struct.std::__1::pair.135"* %46, %"struct.std::__1::pair.135"** %12, align 8
  store %"struct.std::__1::pair.135"* %48, %"struct.std::__1::pair.135"** %13, align 8
  %209 = load %"struct.std::__1::pair.135"*, %"struct.std::__1::pair.135"** %12, align 8
  %210 = load %"struct.std::__1::pair.135"*, %"struct.std::__1::pair.135"** %13, align 8
  %211 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %210, i32 0, i32 0
  store i64* %211, i64** %11, align 8
  %212 = load i64*, i64** %11, align 8
  %213 = load i64, i64* %212, align 8
  %214 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %209, i32 0, i32 0
  store i64 %213, i64* %214, align 8
  %215 = load %"struct.std::__1::pair.135"*, %"struct.std::__1::pair.135"** %13, align 8
  %216 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %215, i32 0, i32 1
  store i64* %216, i64** %10, align 8
  %217 = load i64*, i64** %10, align 8
  %218 = load i64, i64* %217, align 8
  %219 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %209, i32 0, i32 1
  store i64 %218, i64* %219, align 8
  %220 = load i8*, i8** %42, align 8
  %221 = getelementptr inbounds i8, i8* %220, i64 32
  %222 = load i64, i64* %45, align 8
  %223 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %47, i32 0, i32 1
  %224 = load i64, i64* %223, align 8
  %225 = add i64 %222, %224
  %226 = load i64, i64* %44, align 8
  %227 = load i8*, i8** %42, align 8
  %228 = getelementptr inbounds i8, i8* %227, i64 16
  store i8* %228, i8** %14, align 8
  %229 = bitcast i64* %15 to i8*
  %230 = load i8*, i8** %14, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %229, i8* %230, i64 8, i32 1, i1 false) #2
  %231 = load i64, i64* %15, align 8
  %232 = add i64 %226, %231
  %233 = call { i64, i64 } @_ZNSt3__121__murmur2_or_cityhashImLm64EE29__weak_hash_len_32_with_seedsEPKcmm(i8* %221, i64 %225, i64 %232)
  %234 = bitcast %"struct.std::__1::pair.135"* %49 to { i64, i64 }*
  %235 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %234, i32 0, i32 0
  %236 = extractvalue { i64, i64 } %233, 0
  store i64 %236, i64* %235, align 8
  %237 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %234, i32 0, i32 1
  %238 = extractvalue { i64, i64 } %233, 1
  store i64 %238, i64* %237, align 8
  store %"struct.std::__1::pair.135"* %47, %"struct.std::__1::pair.135"** %20, align 8
  store %"struct.std::__1::pair.135"* %49, %"struct.std::__1::pair.135"** %21, align 8
  %239 = load %"struct.std::__1::pair.135"*, %"struct.std::__1::pair.135"** %20, align 8
  %240 = load %"struct.std::__1::pair.135"*, %"struct.std::__1::pair.135"** %21, align 8
  %241 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %240, i32 0, i32 0
  store i64* %241, i64** %19, align 8
  %242 = load i64*, i64** %19, align 8
  %243 = load i64, i64* %242, align 8
  %244 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %239, i32 0, i32 0
  store i64 %243, i64* %244, align 8
  %245 = load %"struct.std::__1::pair.135"*, %"struct.std::__1::pair.135"** %21, align 8
  %246 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %245, i32 0, i32 1
  store i64* %246, i64** %18, align 8
  %247 = load i64*, i64** %18, align 8
  %248 = load i64, i64* %247, align 8
  %249 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %239, i32 0, i32 1
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
  %268 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %46, i32 0, i32 0
  %269 = load i64, i64* %268, align 8
  %270 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %47, i32 0, i32 0
  %271 = load i64, i64* %270, align 8
  %272 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE13__hash_len_16Emm(i64 %269, i64 %271)
  %273 = load i64, i64* %44, align 8
  %274 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE11__shift_mixEm(i64 %273)
  %275 = mul i64 %274, -5435081209227447693
  %276 = add i64 %272, %275
  %277 = load i64, i64* %45, align 8
  %278 = add i64 %276, %277
  %279 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %46, i32 0, i32 1
  %280 = load i64, i64* %279, align 8
  %281 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %47, i32 0, i32 1
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %27, i8* %28, i64 8, i32 1, i1 false) #2
  %29 = load i64, i64* %10, align 8
  store i64 %29, i64* %14, align 8
  %30 = load i8*, i8** %12, align 8
  %31 = load i64, i64* %13, align 8
  %32 = getelementptr inbounds i8, i8* %30, i64 %31
  %33 = getelementptr inbounds i8, i8* %32, i64 -8
  store i8* %33, i8** %7, align 8
  %34 = bitcast i64* %8 to i8*
  %35 = load i8*, i8** %7, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %34, i8* %35, i64 8, i32 1, i1 false) #2
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %52, i8* %53, i64 4, i32 1, i1 false) #2
  %54 = load i32, i32* %6, align 4
  store i32 %54, i32* %16, align 4
  %55 = load i8*, i8** %12, align 8
  %56 = load i64, i64* %13, align 8
  %57 = getelementptr inbounds i8, i8* %55, i64 %56
  %58 = getelementptr inbounds i8, i8* %57, i64 -4
  store i8* %58, i8** %3, align 8
  %59 = bitcast i32* %4 to i8*
  %60 = load i8*, i8** %3, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %59, i8* %60, i64 4, i32 1, i1 false) #2
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %18, i8* %19, i64 8, i32 1, i1 false) #2
  %20 = load i64, i64* %10, align 8
  %21 = mul i64 %20, -5435081209227447693
  store i64 %21, i64* %13, align 8
  %22 = load i8*, i8** %11, align 8
  %23 = getelementptr inbounds i8, i8* %22, i64 8
  store i8* %23, i8** %7, align 8
  %24 = bitcast i64* %8 to i8*
  %25 = load i8*, i8** %7, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %24, i8* %25, i64 8, i32 1, i1 false) #2
  %26 = load i64, i64* %8, align 8
  store i64 %26, i64* %14, align 8
  %27 = load i8*, i8** %11, align 8
  %28 = load i64, i64* %12, align 8
  %29 = getelementptr inbounds i8, i8* %27, i64 %28
  %30 = getelementptr inbounds i8, i8* %29, i64 -8
  store i8* %30, i8** %5, align 8
  %31 = bitcast i64* %6 to i8*
  %32 = load i8*, i8** %5, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %31, i8* %32, i64 8, i32 1, i1 false) #2
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %39, i8* %40, i64 8, i32 1, i1 false) #2
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %36, i8* %37, i64 8, i32 1, i1 false) #2
  %38 = load i64, i64* %22, align 8
  store i64 %38, i64* %25, align 8
  %39 = load i8*, i8** %23, align 8
  store i8* %39, i8** %19, align 8
  %40 = bitcast i64* %20 to i8*
  %41 = load i8*, i8** %19, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %40, i8* %41, i64 8, i32 1, i1 false) #2
  %42 = load i64, i64* %20, align 8
  %43 = load i64, i64* %24, align 8
  %44 = load i8*, i8** %23, align 8
  %45 = load i64, i64* %24, align 8
  %46 = getelementptr inbounds i8, i8* %44, i64 %45
  %47 = getelementptr inbounds i8, i8* %46, i64 -16
  store i8* %47, i8** %17, align 8
  %48 = bitcast i64* %18 to i8*
  %49 = load i8*, i8** %17, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %48, i8* %49, i64 8, i32 1, i1 false) #2
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %62, i8* %63, i64 8, i32 1, i1 false) #2
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %73, i8* %74, i64 8, i32 1, i1 false) #2
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %89, i8* %90, i64 8, i32 1, i1 false) #2
  %91 = load i64, i64* %8, align 8
  %92 = load i8*, i8** %23, align 8
  %93 = load i64, i64* %24, align 8
  %94 = getelementptr inbounds i8, i8* %92, i64 %93
  %95 = getelementptr inbounds i8, i8* %94, i64 -32
  store i8* %95, i8** %5, align 8
  %96 = bitcast i64* %6 to i8*
  %97 = load i8*, i8** %5, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %96, i8* %97, i64 8, i32 1, i1 false) #2
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %104, i8* %105, i64 8, i32 1, i1 false) #2
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %119, i8* %120, i64 8, i32 1, i1 false) #2
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %132, i8* %133, i64 8, i32 1, i1 false) #2
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
define linkonce_odr i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE13__hash_len_16Emm(i64, i64) #5 align 2 {
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
  %12 = alloca %"struct.std::__1::pair.135", align 8
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
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %17, i8* %18, i64 8, i32 1, i1 false) #2
  %19 = load i64, i64* %11, align 8
  %20 = load i8*, i8** %13, align 8
  %21 = getelementptr inbounds i8, i8* %20, i64 8
  store i8* %21, i8** %8, align 8
  %22 = bitcast i64* %9 to i8*
  %23 = load i8*, i8** %8, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %22, i8* %23, i64 8, i32 1, i1 false) #2
  %24 = load i64, i64* %9, align 8
  %25 = load i8*, i8** %13, align 8
  %26 = getelementptr inbounds i8, i8* %25, i64 16
  store i8* %26, i8** %4, align 8
  %27 = bitcast i64* %5 to i8*
  %28 = load i8*, i8** %4, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %27, i8* %28, i64 8, i32 1, i1 false) #2
  %29 = load i64, i64* %5, align 8
  %30 = load i8*, i8** %13, align 8
  %31 = getelementptr inbounds i8, i8* %30, i64 24
  store i8* %31, i8** %6, align 8
  %32 = bitcast i64* %7 to i8*
  %33 = load i8*, i8** %6, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %32, i8* %33, i64 8, i32 1, i1 false) #2
  %34 = load i64, i64* %7, align 8
  %35 = load i64, i64* %14, align 8
  %36 = load i64, i64* %15, align 8
  %37 = call { i64, i64 } @_ZNSt3__121__murmur2_or_cityhashImLm64EE29__weak_hash_len_32_with_seedsEmmmmmm(i64 %19, i64 %24, i64 %29, i64 %34, i64 %35, i64 %36)
  %38 = bitcast %"struct.std::__1::pair.135"* %12 to { i64, i64 }*
  %39 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %38, i32 0, i32 0
  %40 = extractvalue { i64, i64 } %37, 0
  store i64 %40, i64* %39, align 8
  %41 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %38, i32 0, i32 1
  %42 = extractvalue { i64, i64 } %37, 1
  store i64 %42, i64* %41, align 8
  %43 = bitcast %"struct.std::__1::pair.135"* %12 to { i64, i64 }*
  %44 = load { i64, i64 }, { i64, i64 }* %43, align 8
  ret { i64, i64 } %44
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64, i32) #5 align 2 {
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
define linkonce_odr i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE11__shift_mixEm(i64) #5 align 2 {
  %2 = alloca i64, align 8
  store i64 %0, i64* %2, align 8
  %3 = load i64, i64* %2, align 8
  %4 = load i64, i64* %2, align 8
  %5 = lshr i64 %4, 47
  %6 = xor i64 %3, %5
  ret i64 %6
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE22__rotate_by_at_least_1Emi(i64, i32) #5 align 2 {
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

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #9

; Function Attrs: ssp uwtable
define linkonce_odr { i64, i64 } @_ZNSt3__121__murmur2_or_cityhashImLm64EE29__weak_hash_len_32_with_seedsEmmmmmm(i64, i64, i64, i64, i64, i64) #0 align 2 {
  %7 = alloca i64*, align 8
  %8 = alloca i64*, align 8
  %9 = alloca %"struct.std::__1::pair.135"*, align 8
  %10 = alloca i64*, align 8
  %11 = alloca i64*, align 8
  %12 = alloca %"struct.std::__1::pair.135"*, align 8
  %13 = alloca i64*, align 8
  %14 = alloca i64*, align 8
  %15 = alloca %"struct.std::__1::pair.135", align 8
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
  store %"struct.std::__1::pair.135"* %15, %"struct.std::__1::pair.135"** %12, align 8
  store i64* %23, i64** %13, align 8
  store i64* %24, i64** %14, align 8
  %51 = load %"struct.std::__1::pair.135"*, %"struct.std::__1::pair.135"** %12, align 8
  %52 = load i64*, i64** %13, align 8
  %53 = load i64*, i64** %14, align 8
  store %"struct.std::__1::pair.135"* %51, %"struct.std::__1::pair.135"** %9, align 8
  store i64* %52, i64** %10, align 8
  store i64* %53, i64** %11, align 8
  %54 = load %"struct.std::__1::pair.135"*, %"struct.std::__1::pair.135"** %9, align 8
  %55 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %54, i32 0, i32 0
  %56 = load i64*, i64** %10, align 8
  store i64* %56, i64** %8, align 8
  %57 = load i64*, i64** %8, align 8
  %58 = load i64, i64* %57, align 8
  store i64 %58, i64* %55, align 8
  %59 = getelementptr inbounds %"struct.std::__1::pair.135", %"struct.std::__1::pair.135"* %54, i32 0, i32 1
  %60 = load i64*, i64** %11, align 8
  store i64* %60, i64** %7, align 8
  %61 = load i64*, i64** %7, align 8
  %62 = load i64, i64* %61, align 8
  store i64 %62, i64* %59, align 8
  %63 = bitcast %"struct.std::__1::pair.135"* %15 to { i64, i64 }*
  %64 = load { i64, i64 }, { i64, i64 }* %63, align 8
  ret { i64, i64 } %64
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8*, i8*, i64) #10 align 2 {
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

declare i32 @memcmp(i8*, i8*, i64) #3

; Function Attrs: nobuiltin
declare noalias i8* @_Znwm(i64) #11

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"*, %"class.std::__1::basic_string"* dereferenceable(24)) unnamed_addr #3

declare i64 @_ZNSt3__112__next_primeEm(i64) #3

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm10StructTypeEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE8__rehashEm(%"class.std::__1::__hash_table"*, i64) #0 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %5 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %8 = alloca %"class.std::__1::unique_ptr"*, align 8
  %9 = alloca i64, align 8
  %10 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %11 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %12 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %13 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %14 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %15 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %16 = alloca i64, align 8
  %17 = alloca i64, align 8
  %18 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %19 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %20 = alloca %"class.std::__1::unique_ptr"*, align 8
  %21 = alloca i64, align 8
  %22 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %23 = alloca i64, align 8
  %24 = alloca i64, align 8
  %25 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %26 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %27 = alloca %"class.std::__1::unique_ptr"*, align 8
  %28 = alloca i64, align 8
  %29 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %30 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %31 = alloca %"class.std::__1::unique_ptr"*, align 8
  %32 = alloca i64, align 8
  %33 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.9"*, align 8
  %34 = alloca %"class.std::__1::__compressed_pair.8"*, align 8
  %35 = alloca %"class.std::__1::__hash_table"*, align 8
  %36 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %37 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %38 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %39 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %40 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %41 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %42 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %43 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %44 = alloca %"class.std::__1::unique_ptr"*, align 8
  %45 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %46 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %47 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %48 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %49 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %50 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %51 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %52 = alloca i8*, align 8
  %53 = alloca %"class.std::__1::allocator"*, align 8
  %54 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %55 = alloca i64, align 8
  %56 = alloca %"class.std::__1::allocator"*, align 8
  %57 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %58 = alloca i64, align 8
  %59 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %60 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %61 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %62 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %63 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %64 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %65 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %66 = alloca %"class.std::__1::unique_ptr"*, align 8
  %67 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %68 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %69 = alloca i64, align 8
  %70 = alloca %"class.std::__1::allocator"*, align 8
  %71 = alloca i64, align 8
  %72 = alloca i8*, align 8
  %73 = alloca %"class.std::__1::allocator"*, align 8
  %74 = alloca i64, align 8
  %75 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %76 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %77 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %78 = alloca i8*, align 8
  %79 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %80 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %81 = alloca %"class.std::__1::basic_string"*, align 8
  %82 = alloca i8*, align 8
  %83 = alloca i8*, align 8
  %84 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %85 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %86 = alloca %"class.std::__1::basic_string"*, align 8
  %87 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %88 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %89 = alloca %"class.std::__1::basic_string"*, align 8
  %90 = alloca %"class.std::__1::basic_string"*, align 8
  %91 = alloca %"class.std::__1::basic_string"*, align 8
  %92 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %93 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %94 = alloca %"class.std::__1::basic_string"*, align 8
  %95 = alloca i8*, align 8
  %96 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %97 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %98 = alloca %"class.std::__1::basic_string"*, align 8
  %99 = alloca i8*, align 8
  %100 = alloca i8*, align 8
  %101 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %102 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %103 = alloca %"class.std::__1::basic_string"*, align 8
  %104 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %105 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %106 = alloca %"class.std::__1::basic_string"*, align 8
  %107 = alloca %"class.std::__1::basic_string"*, align 8
  %108 = alloca %"class.std::__1::basic_string"*, align 8
  %109 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %110 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %111 = alloca %"class.std::__1::basic_string"*, align 8
  %112 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %113 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %114 = alloca %"class.std::__1::basic_string"*, align 8
  %115 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %116 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %117 = alloca %"class.std::__1::basic_string"*, align 8
  %118 = alloca %"class.std::__1::basic_string"*, align 8
  %119 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %120 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %121 = alloca %"class.std::__1::basic_string"*, align 8
  %122 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %123 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
  %124 = alloca %"class.std::__1::basic_string"*, align 8
  %125 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.37"*, align 8
  %126 = alloca %"class.std::__1::__compressed_pair.36"*, align 8
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
  %141 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %142 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %143 = alloca %"class.std::__1::unique_ptr"*, align 8
  %144 = alloca i64, align 8
  %145 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %146 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %147 = alloca %"class.std::__1::unique_ptr"*, align 8
  %148 = alloca i64, align 8
  %149 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %150 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %151 = alloca %"class.std::__1::unique_ptr"*, align 8
  %152 = alloca %"class.std::__1::__hash_table"*, align 8
  %153 = alloca i64, align 8
  %154 = alloca %"class.std::__1::allocator"*, align 8
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
  store %"class.std::__1::unique_ptr"* %162, %"class.std::__1::unique_ptr"** %151, align 8
  %163 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %151, align 8
  %164 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %163, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %164, %"class.std::__1::__compressed_pair"** %150, align 8
  %165 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %150, align 8
  %166 = bitcast %"class.std::__1::__compressed_pair"* %165 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %166, %"class.std::__1::__libcpp_compressed_pair_imp"** %149, align 8
  %167 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %149, align 8
  %168 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %167, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator"* %168, %"class.std::__1::__bucket_list_deallocator"** %77, align 8
  %169 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %77, align 8
  %170 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %169, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.0"* %170, %"class.std::__1::__compressed_pair.0"** %76, align 8
  %171 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %76, align 8
  %172 = bitcast %"class.std::__1::__compressed_pair.0"* %171 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %172, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %75, align 8
  %173 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %75, align 8
  %174 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.1"* %173 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %174, %"class.std::__1::allocator"** %154, align 8
  %175 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %161, i32 0, i32 0
  %176 = load i64, i64* %153, align 8
  %177 = icmp ugt i64 %176, 0
  br i1 %177, label %178, label %189

; <label>:178:                                    ; preds = %2
  %179 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %154, align 8
  %180 = load i64, i64* %153, align 8
  store %"class.std::__1::allocator"* %179, %"class.std::__1::allocator"** %73, align 8
  store i64 %180, i64* %74, align 8
  %181 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %73, align 8
  %182 = load i64, i64* %74, align 8
  store %"class.std::__1::allocator"* %181, %"class.std::__1::allocator"** %70, align 8
  store i64 %182, i64* %71, align 8
  store i8* null, i8** %72, align 8
  %183 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %70, align 8
  %184 = load i64, i64* %71, align 8
  %185 = mul i64 %184, 8
  store i64 %185, i64* %69, align 8
  %186 = load i64, i64* %69, align 8
  %187 = call i8* @_Znwm(i64 %186) #17
  %188 = bitcast i8* %187 to %"struct.std::__1::__hash_node_base"**
  br label %190

; <label>:189:                                    ; preds = %2
  br label %190

; <label>:190:                                    ; preds = %189, %178
  %191 = phi %"struct.std::__1::__hash_node_base"** [ %188, %178 ], [ null, %189 ]
  store %"class.std::__1::unique_ptr"* %175, %"class.std::__1::unique_ptr"** %66, align 8
  store %"struct.std::__1::__hash_node_base"** %191, %"struct.std::__1::__hash_node_base"*** %67, align 8
  %192 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %66, align 8
  %193 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %192, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %193, %"class.std::__1::__compressed_pair"** %65, align 8
  %194 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %65, align 8
  %195 = bitcast %"class.std::__1::__compressed_pair"* %194 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %195, %"class.std::__1::__libcpp_compressed_pair_imp"** %64, align 8
  %196 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %64, align 8
  %197 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %196, i32 0, i32 0
  %198 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %197, align 8
  store %"struct.std::__1::__hash_node_base"** %198, %"struct.std::__1::__hash_node_base"*** %68, align 8
  %199 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %67, align 8
  %200 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %192, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %200, %"class.std::__1::__compressed_pair"** %48, align 8
  %201 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8
  %202 = bitcast %"class.std::__1::__compressed_pair"* %201 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %202, %"class.std::__1::__libcpp_compressed_pair_imp"** %47, align 8
  %203 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %47, align 8
  %204 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %203, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"** %199, %"struct.std::__1::__hash_node_base"*** %204, align 8
  %205 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %68, align 8
  %206 = icmp ne %"struct.std::__1::__hash_node_base"** %205, null
  br i1 %206, label %207, label %236

; <label>:207:                                    ; preds = %190
  %208 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %192, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %208, %"class.std::__1::__compressed_pair"** %46, align 8
  %209 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %46, align 8
  %210 = bitcast %"class.std::__1::__compressed_pair"* %209 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %210, %"class.std::__1::__libcpp_compressed_pair_imp"** %45, align 8
  %211 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %45, align 8
  %212 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %211, i32 0, i32 1
  %213 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %68, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %212, %"class.std::__1::__bucket_list_deallocator"** %62, align 8
  store %"struct.std::__1::__hash_node_base"** %213, %"struct.std::__1::__hash_node_base"*** %63, align 8
  %214 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %62, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %214, %"class.std::__1::__bucket_list_deallocator"** %61, align 8
  %215 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %61, align 8
  %216 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %215, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.0"* %216, %"class.std::__1::__compressed_pair.0"** %60, align 8
  %217 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %60, align 8
  %218 = bitcast %"class.std::__1::__compressed_pair.0"* %217 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %218, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %59, align 8
  %219 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %59, align 8
  %220 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.1"* %219 to %"class.std::__1::allocator"*
  %221 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %63, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %214, %"class.std::__1::__bucket_list_deallocator"** %51, align 8
  %222 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %51, align 8
  %223 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %222, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.0"* %223, %"class.std::__1::__compressed_pair.0"** %50, align 8
  %224 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %50, align 8
  %225 = bitcast %"class.std::__1::__compressed_pair.0"* %224 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %225, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %49, align 8
  %226 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %49, align 8
  %227 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.1", %"class.std::__1::__libcpp_compressed_pair_imp.1"* %226, i32 0, i32 0
  %228 = load i64, i64* %227, align 8
  store %"class.std::__1::allocator"* %220, %"class.std::__1::allocator"** %56, align 8
  store %"struct.std::__1::__hash_node_base"** %221, %"struct.std::__1::__hash_node_base"*** %57, align 8
  store i64 %228, i64* %58, align 8
  %229 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %56, align 8
  %230 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %57, align 8
  %231 = load i64, i64* %58, align 8
  store %"class.std::__1::allocator"* %229, %"class.std::__1::allocator"** %53, align 8
  store %"struct.std::__1::__hash_node_base"** %230, %"struct.std::__1::__hash_node_base"*** %54, align 8
  store i64 %231, i64* %55, align 8
  %232 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %53, align 8
  %233 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %54, align 8
  %234 = bitcast %"struct.std::__1::__hash_node_base"** %233 to i8*
  store i8* %234, i8** %52, align 8
  %235 = load i8*, i8** %52, align 8
  call void @_ZdlPv(i8* %235) #14
  br label %236

; <label>:236:                                    ; preds = %190, %207
  %237 = load i64, i64* %153, align 8
  %238 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %161, i32 0, i32 0
  store %"class.std::__1::unique_ptr"* %238, %"class.std::__1::unique_ptr"** %44, align 8
  %239 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %44, align 8
  %240 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %239, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %240, %"class.std::__1::__compressed_pair"** %43, align 8
  %241 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %43, align 8
  %242 = bitcast %"class.std::__1::__compressed_pair"* %241 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %242, %"class.std::__1::__libcpp_compressed_pair_imp"** %42, align 8
  %243 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %42, align 8
  %244 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %243, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator"* %244, %"class.std::__1::__bucket_list_deallocator"** %5, align 8
  %245 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %5, align 8
  %246 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %245, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.0"* %246, %"class.std::__1::__compressed_pair.0"** %4, align 8
  %247 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %4, align 8
  %248 = bitcast %"class.std::__1::__compressed_pair.0"* %247 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %248, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %3, align 8
  %249 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %3, align 8
  %250 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.1", %"class.std::__1::__libcpp_compressed_pair_imp.1"* %249, i32 0, i32 0
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
  store %"class.std::__1::unique_ptr"* %259, %"class.std::__1::unique_ptr"** %8, align 8
  store i64 %260, i64* %9, align 8
  %261 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %8, align 8
  %262 = load i64, i64* %9, align 8
  %263 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %261, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %263, %"class.std::__1::__compressed_pair"** %7, align 8
  %264 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %7, align 8
  %265 = bitcast %"class.std::__1::__compressed_pair"* %264 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %265, %"class.std::__1::__libcpp_compressed_pair_imp"** %6, align 8
  %266 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %6, align 8
  %267 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %266, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair.2"* %274, %"class.std::__1::__compressed_pair.2"** %11, align 8
  %275 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %11, align 8
  %276 = bitcast %"class.std::__1::__compressed_pair.2"* %275 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %276, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %10, align 8
  %277 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %10, align 8
  %278 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %277, i32 0, i32 0
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
  store %"class.std::__1::unique_ptr"* %320, %"class.std::__1::unique_ptr"** %20, align 8
  store i64 %321, i64* %21, align 8
  %322 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %20, align 8
  %323 = load i64, i64* %21, align 8
  %324 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %322, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %324, %"class.std::__1::__compressed_pair"** %19, align 8
  %325 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8
  %326 = bitcast %"class.std::__1::__compressed_pair"* %325 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %326, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8
  %327 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8
  %328 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %327, i32 0, i32 0
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
  store %"class.std::__1::unique_ptr"* %376, %"class.std::__1::unique_ptr"** %27, align 8
  store i64 %377, i64* %28, align 8
  %378 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %27, align 8
  %379 = load i64, i64* %28, align 8
  %380 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %378, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %380, %"class.std::__1::__compressed_pair"** %26, align 8
  %381 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %26, align 8
  %382 = bitcast %"class.std::__1::__compressed_pair"* %381 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %382, %"class.std::__1::__libcpp_compressed_pair_imp"** %25, align 8
  %383 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %25, align 8
  %384 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %383, i32 0, i32 0
  %385 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %384, align 8
  %386 = getelementptr inbounds %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %385, i64 %379
  %387 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %386, align 8
  %388 = icmp eq %"struct.std::__1::__hash_node_base"* %387, null
  br i1 %388, label %389, label %404

; <label>:389:                                    ; preds = %375
  %390 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %156, align 8
  %391 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %161, i32 0, i32 0
  %392 = load i64, i64* %158, align 8
  store %"class.std::__1::unique_ptr"* %391, %"class.std::__1::unique_ptr"** %31, align 8
  store i64 %392, i64* %32, align 8
  %393 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %31, align 8
  %394 = load i64, i64* %32, align 8
  %395 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %393, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %395, %"class.std::__1::__compressed_pair"** %30, align 8
  %396 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %30, align 8
  %397 = bitcast %"class.std::__1::__compressed_pair"* %396 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %397, %"class.std::__1::__libcpp_compressed_pair_imp"** %29, align 8
  %398 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %29, align 8
  %399 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %398, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair.8"* %413, %"class.std::__1::__compressed_pair.8"** %34, align 8
  %414 = load %"class.std::__1::__compressed_pair.8"*, %"class.std::__1::__compressed_pair.8"** %34, align 8
  %415 = bitcast %"class.std::__1::__compressed_pair.8"* %414 to %"class.std::__1::__libcpp_compressed_pair_imp.9"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.9"* %415, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %33, align 8
  %416 = load %"class.std::__1::__libcpp_compressed_pair_imp.9"*, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %33, align 8
  %417 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.9"* %416 to %"class.std::__1::__unordered_map_equal"*
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
  %437 = bitcast %"union.std::__1::__hash_value_type"* %436 to %"struct.std::__1::pair"*
  %438 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %437, i32 0, i32 0
  %439 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %140, align 8
  %440 = bitcast %"union.std::__1::__hash_value_type"* %439 to %"struct.std::__1::pair"*
  %441 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %440, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair.36"* %448, %"class.std::__1::__compressed_pair.36"** %126, align 8
  %449 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %126, align 8
  %450 = bitcast %"class.std::__1::__compressed_pair.36"* %449 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %450, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %125, align 8
  %451 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %125, align 8
  %452 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %451, i32 0, i32 0
  %453 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %452, i32 0, i32 0
  %454 = bitcast %union.anon* %453 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %455 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %454, i32 0, i32 0
  %456 = bitcast %union.anon.40* %455 to i8*
  %457 = load i8, i8* %456, align 8
  %458 = zext i8 %457 to i32
  %459 = and i32 %458, 1
  %460 = icmp ne i32 %459, 0
  br i1 %460, label %461, label %472

; <label>:461:                                    ; preds = %411
  store %"class.std::__1::basic_string"* %446, %"class.std::__1::basic_string"** %121, align 8
  %462 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %121, align 8
  %463 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %462, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %463, %"class.std::__1::__compressed_pair.36"** %120, align 8
  %464 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %120, align 8
  %465 = bitcast %"class.std::__1::__compressed_pair.36"* %464 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %465, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %119, align 8
  %466 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %119, align 8
  %467 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %466, i32 0, i32 0
  %468 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %467, i32 0, i32 0
  %469 = bitcast %union.anon* %468 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %470 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %469, i32 0, i32 1
  %471 = load i64, i64* %470, align 8
  br label %487

; <label>:472:                                    ; preds = %411
  store %"class.std::__1::basic_string"* %446, %"class.std::__1::basic_string"** %124, align 8
  %473 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %124, align 8
  %474 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %473, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %474, %"class.std::__1::__compressed_pair.36"** %123, align 8
  %475 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %123, align 8
  %476 = bitcast %"class.std::__1::__compressed_pair.36"* %475 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %476, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %122, align 8
  %477 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %122, align 8
  %478 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %477, i32 0, i32 0
  %479 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %478, i32 0, i32 0
  %480 = bitcast %union.anon* %479 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %481 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %480, i32 0, i32 0
  %482 = bitcast %union.anon.40* %481 to i8*
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
  store %"class.std::__1::__compressed_pair.36"* %493, %"class.std::__1::__compressed_pair.36"** %116, align 8
  %494 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %116, align 8
  %495 = bitcast %"class.std::__1::__compressed_pair.36"* %494 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %495, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %115, align 8
  %496 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %115, align 8
  %497 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %496, i32 0, i32 0
  %498 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %497, i32 0, i32 0
  %499 = bitcast %union.anon* %498 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %500 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %499, i32 0, i32 0
  %501 = bitcast %union.anon.40* %500 to i8*
  %502 = load i8, i8* %501, align 8
  %503 = zext i8 %502 to i32
  %504 = and i32 %503, 1
  %505 = icmp ne i32 %504, 0
  br i1 %505, label %506, label %517

; <label>:506:                                    ; preds = %487
  store %"class.std::__1::basic_string"* %491, %"class.std::__1::basic_string"** %111, align 8
  %507 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %111, align 8
  %508 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %507, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %508, %"class.std::__1::__compressed_pair.36"** %110, align 8
  %509 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %110, align 8
  %510 = bitcast %"class.std::__1::__compressed_pair.36"* %509 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %510, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %109, align 8
  %511 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %109, align 8
  %512 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %511, i32 0, i32 0
  %513 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %512, i32 0, i32 0
  %514 = bitcast %union.anon* %513 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %515 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %514, i32 0, i32 1
  %516 = load i64, i64* %515, align 8
  br label %532

; <label>:517:                                    ; preds = %487
  store %"class.std::__1::basic_string"* %491, %"class.std::__1::basic_string"** %114, align 8
  %518 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %114, align 8
  %519 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %518, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %519, %"class.std::__1::__compressed_pair.36"** %113, align 8
  %520 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %113, align 8
  %521 = bitcast %"class.std::__1::__compressed_pair.36"* %520 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %521, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %112, align 8
  %522 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %112, align 8
  %523 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %522, i32 0, i32 0
  %524 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %523, i32 0, i32 0
  %525 = bitcast %union.anon* %524 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %526 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %525, i32 0, i32 0
  %527 = bitcast %union.anon.40* %526 to i8*
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
  store %"class.std::__1::__compressed_pair.36"* %541, %"class.std::__1::__compressed_pair.36"** %105, align 8
  %542 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %105, align 8
  %543 = bitcast %"class.std::__1::__compressed_pair.36"* %542 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %543, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %104, align 8
  %544 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %104, align 8
  %545 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %544, i32 0, i32 0
  %546 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %545, i32 0, i32 0
  %547 = bitcast %union.anon* %546 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %548 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %547, i32 0, i32 0
  %549 = bitcast %union.anon.40* %548 to i8*
  %550 = load i8, i8* %549, align 8
  %551 = zext i8 %550 to i32
  %552 = and i32 %551, 1
  %553 = icmp ne i32 %552, 0
  br i1 %553, label %554, label %565

; <label>:554:                                    ; preds = %536
  store %"class.std::__1::basic_string"* %539, %"class.std::__1::basic_string"** %98, align 8
  %555 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %98, align 8
  %556 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %555, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %556, %"class.std::__1::__compressed_pair.36"** %97, align 8
  %557 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %97, align 8
  %558 = bitcast %"class.std::__1::__compressed_pair.36"* %557 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %558, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %96, align 8
  %559 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %96, align 8
  %560 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %559, i32 0, i32 0
  %561 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %560, i32 0, i32 0
  %562 = bitcast %union.anon* %561 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %563 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %562, i32 0, i32 2
  %564 = load i8*, i8** %563, align 8
  br label %578

; <label>:565:                                    ; preds = %536
  store %"class.std::__1::basic_string"* %539, %"class.std::__1::basic_string"** %103, align 8
  %566 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %103, align 8
  %567 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %566, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %567, %"class.std::__1::__compressed_pair.36"** %102, align 8
  %568 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %102, align 8
  %569 = bitcast %"class.std::__1::__compressed_pair.36"* %568 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %569, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %101, align 8
  %570 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %101, align 8
  %571 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %570, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair.36"* %585, %"class.std::__1::__compressed_pair.36"** %88, align 8
  %586 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %88, align 8
  %587 = bitcast %"class.std::__1::__compressed_pair.36"* %586 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %587, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %87, align 8
  %588 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %87, align 8
  %589 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %588, i32 0, i32 0
  %590 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %589, i32 0, i32 0
  %591 = bitcast %union.anon* %590 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %592 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %591, i32 0, i32 0
  %593 = bitcast %union.anon.40* %592 to i8*
  %594 = load i8, i8* %593, align 8
  %595 = zext i8 %594 to i32
  %596 = and i32 %595, 1
  %597 = icmp ne i32 %596, 0
  br i1 %597, label %598, label %609

; <label>:598:                                    ; preds = %578
  store %"class.std::__1::basic_string"* %583, %"class.std::__1::basic_string"** %81, align 8
  %599 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %81, align 8
  %600 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %599, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %600, %"class.std::__1::__compressed_pair.36"** %80, align 8
  %601 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %80, align 8
  %602 = bitcast %"class.std::__1::__compressed_pair.36"* %601 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %602, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %79, align 8
  %603 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %79, align 8
  %604 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %603, i32 0, i32 0
  %605 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %604, i32 0, i32 0
  %606 = bitcast %union.anon* %605 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %607 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %606, i32 0, i32 2
  %608 = load i8*, i8** %607, align 8
  br label %622

; <label>:609:                                    ; preds = %578
  store %"class.std::__1::basic_string"* %583, %"class.std::__1::basic_string"** %86, align 8
  %610 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %86, align 8
  %611 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %610, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.36"* %611, %"class.std::__1::__compressed_pair.36"** %85, align 8
  %612 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %85, align 8
  %613 = bitcast %"class.std::__1::__compressed_pair.36"* %612 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %613, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %84, align 8
  %614 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %84, align 8
  %615 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %614, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair.36"* %627, %"class.std::__1::__compressed_pair.36"** %93, align 8
  %628 = load %"class.std::__1::__compressed_pair.36"*, %"class.std::__1::__compressed_pair.36"** %93, align 8
  %629 = bitcast %"class.std::__1::__compressed_pair.36"* %628 to %"class.std::__1::__libcpp_compressed_pair_imp.37"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.37"* %629, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %92, align 8
  %630 = load %"class.std::__1::__libcpp_compressed_pair_imp.37"*, %"class.std::__1::__libcpp_compressed_pair_imp.37"** %92, align 8
  %631 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.37", %"class.std::__1::__libcpp_compressed_pair_imp.37"* %630, i32 0, i32 0
  %632 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %631, i32 0, i32 0
  %633 = bitcast %union.anon* %632 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %634 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %633, i32 0, i32 0
  %635 = bitcast %union.anon.40* %634 to i8*
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
  call void @__clang_call_terminate(i8* %670) #15
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
  store %"class.std::__1::unique_ptr"* %686, %"class.std::__1::unique_ptr"** %143, align 8
  store i64 %687, i64* %144, align 8
  %688 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %143, align 8
  %689 = load i64, i64* %144, align 8
  %690 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %688, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %690, %"class.std::__1::__compressed_pair"** %142, align 8
  %691 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %142, align 8
  %692 = bitcast %"class.std::__1::__compressed_pair"* %691 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %692, %"class.std::__1::__libcpp_compressed_pair_imp"** %141, align 8
  %693 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %141, align 8
  %694 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %693, i32 0, i32 0
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
  store %"class.std::__1::unique_ptr"* %703, %"class.std::__1::unique_ptr"** %147, align 8
  store i64 %704, i64* %148, align 8
  %705 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %147, align 8
  %706 = load i64, i64* %148, align 8
  %707 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %705, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %707, %"class.std::__1::__compressed_pair"** %146, align 8
  %708 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %146, align 8
  %709 = bitcast %"class.std::__1::__compressed_pair"* %708 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %709, %"class.std::__1::__libcpp_compressed_pair_imp"** %145, align 8
  %710 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %145, align 8
  %711 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %710, i32 0, i32 0
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

; Function Attrs: nounwind readnone
declare i64 @llvm.ctlz.i64(i64, i1) #12

; Function Attrs: nounwind readnone
declare float @ceilf(float) #13

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm8ArrayRefIPNS_4TypeEEC2INSt3__19allocatorIS2_EEEERKNS5_6vectorIS2_T_EE(%"class.llvm::ArrayRef"*, %"class.std::__1::vector.78"* dereferenceable(24)) unnamed_addr #5 align 2 {
  %3 = alloca %"class.std::__1::vector.78"*, align 8
  %4 = alloca %"class.llvm::Type"**, align 8
  %5 = alloca %"class.std::__1::vector.78"*, align 8
  %6 = alloca %"class.llvm::ArrayRef"*, align 8
  %7 = alloca %"class.std::__1::vector.78"*, align 8
  store %"class.llvm::ArrayRef"* %0, %"class.llvm::ArrayRef"** %6, align 8
  store %"class.std::__1::vector.78"* %1, %"class.std::__1::vector.78"** %7, align 8
  %8 = load %"class.llvm::ArrayRef"*, %"class.llvm::ArrayRef"** %6, align 8
  %9 = getelementptr inbounds %"class.llvm::ArrayRef", %"class.llvm::ArrayRef"* %8, i32 0, i32 0
  %10 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %7, align 8
  store %"class.std::__1::vector.78"* %10, %"class.std::__1::vector.78"** %5, align 8
  %11 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %5, align 8
  %12 = bitcast %"class.std::__1::vector.78"* %11 to %"class.std::__1::__vector_base.79"*
  %13 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %12, i32 0, i32 0
  %14 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %13, align 8
  store %"class.llvm::Type"** %14, %"class.llvm::Type"*** %4, align 8
  %15 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %4, align 8
  store %"class.llvm::Type"** %15, %"class.llvm::Type"*** %9, align 8
  %16 = getelementptr inbounds %"class.llvm::ArrayRef", %"class.llvm::ArrayRef"* %8, i32 0, i32 1
  %17 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %7, align 8
  store %"class.std::__1::vector.78"* %17, %"class.std::__1::vector.78"** %3, align 8
  %18 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %3, align 8
  %19 = bitcast %"class.std::__1::vector.78"* %18 to %"class.std::__1::__vector_base.79"*
  %20 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %19, i32 0, i32 1
  %21 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %20, align 8
  %22 = bitcast %"class.std::__1::vector.78"* %18 to %"class.std::__1::__vector_base.79"*
  %23 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %22, i32 0, i32 0
  %24 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %23, align 8
  %25 = ptrtoint %"class.llvm::Type"** %21 to i64
  %26 = ptrtoint %"class.llvm::Type"** %24 to i64
  %27 = sub i64 %25, %26
  %28 = sdiv exact i64 %27, 8
  store i64 %28, i64* %16, align 8
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeIjPN4llvm10StructTypeEEENS_22__unordered_map_hasherIjS5_NS_4hashIjEELb1EEENS_21__unordered_map_equalIjS5_NS_8equal_toIjEELb1EEENS_9allocatorIS5_EEE21__construct_node_hashIRKNS_21piecewise_construct_tEJNS_5tupleIJRKjEEENSL_IJEEEEEENS_10unique_ptrINS_11__hash_nodeIS5_PvEENS_22__hash_node_destructorINSE_IST_EEEEEEmOT_DpOT0_(%"class.std::__1::unique_ptr.144"* noalias sret, %"class.std::__1::__hash_table.11"*, i64, %"struct.std::__1::piecewise_construct_t"* dereferenceable(1), %"class.std::__1::tuple.141"* dereferenceable(8), %"class.std::__1::tuple.131"* dereferenceable(1)) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %7 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %8 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %9 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %10 = alloca %"struct.std::__1::pair.125"*, align 8
  %11 = alloca %"union.std::__1::__hash_value_type.124"*, align 8
  %12 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %13 = alloca %"class.std::__1::tuple.141"*, align 8
  %14 = alloca %"class.std::__1::tuple.131"*, align 8
  %15 = alloca %"class.std::__1::tuple.141"*, align 8
  %16 = alloca %"class.std::__1::tuple.131"*, align 8
  %17 = alloca %"class.std::__1::tuple.141"*, align 8
  %18 = alloca %"class.std::__1::tuple.131"*, align 8
  %19 = alloca %"class.std::__1::tuple.141"*, align 8
  %20 = alloca %"class.std::__1::tuple.131"*, align 8
  %21 = alloca i32*, align 8
  %22 = alloca %"class.std::__1::__tuple_leaf.143"*, align 8
  %23 = alloca %"class.std::__1::tuple.141"*, align 8
  %24 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %25 = alloca %"struct.std::__1::__tuple_indices", align 1
  %26 = alloca %"struct.std::__1::__tuple_indices.136", align 1
  %27 = alloca %"struct.std::__1::pair.125"*, align 8
  %28 = alloca %"class.std::__1::tuple.141"*, align 8
  %29 = alloca %"class.std::__1::tuple.131"*, align 8
  %30 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %31 = alloca %"struct.std::__1::__tuple_indices", align 1
  %32 = alloca %"struct.std::__1::__tuple_indices.136", align 1
  %33 = alloca %"struct.std::__1::pair.125"*, align 8
  %34 = alloca %"class.std::__1::tuple.141"*, align 8
  %35 = alloca %"class.std::__1::tuple.131"*, align 8
  %36 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %37 = alloca %"class.std::__1::tuple.141", align 8
  %38 = alloca %"class.std::__1::tuple.131", align 1
  %39 = alloca %"struct.std::__1::pair.125"*, align 8
  %40 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %41 = alloca %"struct.std::__1::__tuple_indices", align 1
  %42 = alloca %"struct.std::__1::__tuple_indices.136", align 1
  %43 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %44 = alloca %"class.std::__1::allocator.23"*, align 8
  %45 = alloca %"struct.std::__1::pair.125"*, align 8
  %46 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %47 = alloca %"class.std::__1::tuple.141"*, align 8
  %48 = alloca %"class.std::__1::tuple.131"*, align 8
  %49 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %50 = alloca %"class.std::__1::tuple.141", align 8
  %51 = alloca %"class.std::__1::tuple.131", align 1
  %52 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %53 = alloca %"struct.std::__1::integral_constant.128", align 1
  %54 = alloca %"class.std::__1::allocator.23"*, align 8
  %55 = alloca %"struct.std::__1::pair.125"*, align 8
  %56 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %57 = alloca %"class.std::__1::tuple.141"*, align 8
  %58 = alloca %"class.std::__1::tuple.131"*, align 8
  %59 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %60 = alloca %"class.std::__1::allocator.23"*, align 8
  %61 = alloca %"struct.std::__1::pair.125"*, align 8
  %62 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %63 = alloca %"class.std::__1::tuple.141"*, align 8
  %64 = alloca %"class.std::__1::tuple.131"*, align 8
  %65 = alloca %"struct.std::__1::integral_constant.128", align 1
  %66 = alloca %"struct.std::__1::__has_construct.149", align 1
  %67 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %68 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %69 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %70 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %71 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %72 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %73 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %74 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %75 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %76 = alloca %"class.std::__1::__hash_node_destructor.147"*, align 8
  %77 = alloca %"class.std::__1::__hash_node_destructor.147"*, align 8
  %78 = alloca %"struct.std::__1::__hash_node.123"**, align 8
  %79 = alloca %"class.std::__1::__hash_node_destructor.147", align 8
  %80 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %81 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %82 = alloca %"struct.std::__1::__hash_node.123"**, align 8
  %83 = alloca %"class.std::__1::__hash_node_destructor.147", align 8
  %84 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %85 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %86 = alloca %"class.std::__1::__hash_node_destructor.147", align 8
  %87 = alloca %"class.std::__1::__hash_node_destructor.147", align 8
  %88 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %89 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %90 = alloca %"class.std::__1::__hash_node_destructor.147"*, align 8
  %91 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %92 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %93 = alloca %"class.std::__1::__hash_node_destructor.147"*, align 8
  %94 = alloca %"class.std::__1::__hash_node_destructor.147", align 8
  %95 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %96 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %97 = alloca %"class.std::__1::__hash_node_destructor.147"*, align 8
  %98 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %99 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %100 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %101 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %102 = alloca i8*, align 8
  %103 = alloca %"class.std::__1::allocator.23"*, align 8
  %104 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %105 = alloca i64, align 8
  %106 = alloca %"class.std::__1::allocator.23"*, align 8
  %107 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %108 = alloca i64, align 8
  %109 = alloca %"class.std::__1::allocator.23"*, align 8
  %110 = alloca %"struct.std::__1::pair.125"*, align 8
  %111 = alloca %"class.std::__1::allocator.23"*, align 8
  %112 = alloca %"struct.std::__1::pair.125"*, align 8
  %113 = alloca %"struct.std::__1::integral_constant", align 1
  %114 = alloca %"struct.std::__1::__has_destroy.127", align 1
  %115 = alloca %"struct.std::__1::pair.125"*, align 8
  %116 = alloca %"union.std::__1::__hash_value_type.124"*, align 8
  %117 = alloca %"class.std::__1::__hash_node_destructor.147"*, align 8
  %118 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %119 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %120 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %121 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %122 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %123 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %124 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %125 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %126 = alloca %"class.std::__1::__hash_node_destructor.147"*, align 8
  %127 = alloca %"class.std::__1::allocator.23"*, align 8
  %128 = alloca i8, align 1
  %129 = alloca %"class.std::__1::__hash_node_destructor.147"*, align 8
  %130 = alloca %"class.std::__1::allocator.23"*, align 8
  %131 = alloca i8, align 1
  %132 = alloca i64, align 8
  %133 = alloca %"class.std::__1::allocator.23"*, align 8
  %134 = alloca i64, align 8
  %135 = alloca i8*, align 8
  %136 = alloca %"class.std::__1::allocator.23"*, align 8
  %137 = alloca i64, align 8
  %138 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %139 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %140 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %141 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %142 = alloca i8*, align 8
  %143 = alloca %"class.std::__1::allocator.23"*, align 8
  %144 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %145 = alloca i64, align 8
  %146 = alloca %"class.std::__1::allocator.23"*, align 8
  %147 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %148 = alloca i64, align 8
  %149 = alloca %"class.std::__1::allocator.23"*, align 8
  %150 = alloca %"struct.std::__1::pair.125"*, align 8
  %151 = alloca %"class.std::__1::allocator.23"*, align 8
  %152 = alloca %"struct.std::__1::pair.125"*, align 8
  %153 = alloca %"struct.std::__1::integral_constant", align 1
  %154 = alloca %"struct.std::__1::__has_destroy.127", align 1
  %155 = alloca %"struct.std::__1::pair.125"*, align 8
  %156 = alloca %"union.std::__1::__hash_value_type.124"*, align 8
  %157 = alloca %"class.std::__1::__hash_node_destructor.147"*, align 8
  %158 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %159 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.146"*, align 8
  %160 = alloca %"class.std::__1::__compressed_pair.145"*, align 8
  %161 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %162 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %163 = alloca %"struct.std::__1::__hash_node.123"*, align 8
  %164 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %165 = alloca %"class.std::__1::unique_ptr.144"*, align 8
  %166 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.22"*, align 8
  %167 = alloca %"class.std::__1::__compressed_pair.21"*, align 8
  %168 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %169 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %170 = alloca i64, align 8
  %171 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %172 = alloca %"class.std::__1::tuple.141"*, align 8
  %173 = alloca %"class.std::__1::tuple.131"*, align 8
  %174 = alloca %"class.std::__1::allocator.23"*, align 8
  %175 = alloca i1, align 1
  %176 = alloca %"class.std::__1::__hash_node_destructor.147", align 8
  %177 = alloca i8*
  %178 = alloca i32
  store %"class.std::__1::__hash_table.11"* %1, %"class.std::__1::__hash_table.11"** %169, align 8
  store i64 %2, i64* %170, align 8
  store %"struct.std::__1::piecewise_construct_t"* %3, %"struct.std::__1::piecewise_construct_t"** %171, align 8
  store %"class.std::__1::tuple.141"* %4, %"class.std::__1::tuple.141"** %172, align 8
  store %"class.std::__1::tuple.131"* %5, %"class.std::__1::tuple.131"** %173, align 8
  %179 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %169, align 8
  store %"class.std::__1::__hash_table.11"* %179, %"class.std::__1::__hash_table.11"** %168, align 8
  %180 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %168, align 8
  %181 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %180, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.21"* %181, %"class.std::__1::__compressed_pair.21"** %167, align 8
  %182 = load %"class.std::__1::__compressed_pair.21"*, %"class.std::__1::__compressed_pair.21"** %167, align 8
  %183 = bitcast %"class.std::__1::__compressed_pair.21"* %182 to %"class.std::__1::__libcpp_compressed_pair_imp.22"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.22"* %183, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %166, align 8
  %184 = load %"class.std::__1::__libcpp_compressed_pair_imp.22"*, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %166, align 8
  %185 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.22"* %184 to %"class.std::__1::allocator.23"*
  store %"class.std::__1::allocator.23"* %185, %"class.std::__1::allocator.23"** %174, align 8
  store i1 false, i1* %175, align 1
  %186 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %174, align 8
  store %"class.std::__1::allocator.23"* %186, %"class.std::__1::allocator.23"** %136, align 8
  store i64 1, i64* %137, align 8
  %187 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %136, align 8
  %188 = load i64, i64* %137, align 8
  store %"class.std::__1::allocator.23"* %187, %"class.std::__1::allocator.23"** %133, align 8
  store i64 %188, i64* %134, align 8
  store i8* null, i8** %135, align 8
  %189 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %133, align 8
  %190 = load i64, i64* %134, align 8
  %191 = mul i64 %190, 32
  store i64 %191, i64* %132, align 8
  %192 = load i64, i64* %132, align 8
  %193 = call i8* @_Znwm(i64 %192) #17
  %194 = bitcast i8* %193 to %"struct.std::__1::__hash_node.123"*
  %195 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %174, align 8
  store %"class.std::__1::__hash_node_destructor.147"* %176, %"class.std::__1::__hash_node_destructor.147"** %129, align 8
  store %"class.std::__1::allocator.23"* %195, %"class.std::__1::allocator.23"** %130, align 8
  store i8 0, i8* %131, align 1
  %196 = load %"class.std::__1::__hash_node_destructor.147"*, %"class.std::__1::__hash_node_destructor.147"** %129, align 8
  %197 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %130, align 8
  %198 = load i8, i8* %131, align 1
  %199 = trunc i8 %198 to i1
  store %"class.std::__1::__hash_node_destructor.147"* %196, %"class.std::__1::__hash_node_destructor.147"** %126, align 8
  store %"class.std::__1::allocator.23"* %197, %"class.std::__1::allocator.23"** %127, align 8
  %200 = zext i1 %199 to i8
  store i8 %200, i8* %128, align 1
  %201 = load %"class.std::__1::__hash_node_destructor.147"*, %"class.std::__1::__hash_node_destructor.147"** %126, align 8
  %202 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %201, i32 0, i32 0
  %203 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %127, align 8
  store %"class.std::__1::allocator.23"* %203, %"class.std::__1::allocator.23"** %202, align 8
  %204 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %201, i32 0, i32 1
  %205 = load i8, i8* %128, align 1
  %206 = trunc i8 %205 to i1
  %207 = zext i1 %206 to i8
  store i8 %207, i8* %204, align 8
  store %"class.std::__1::unique_ptr.144"* %0, %"class.std::__1::unique_ptr.144"** %95, align 8
  store %"struct.std::__1::__hash_node.123"* %194, %"struct.std::__1::__hash_node.123"** %96, align 8
  store %"class.std::__1::__hash_node_destructor.147"* %176, %"class.std::__1::__hash_node_destructor.147"** %97, align 8
  %208 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %95, align 8
  %209 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %96, align 8
  %210 = load %"class.std::__1::__hash_node_destructor.147"*, %"class.std::__1::__hash_node_destructor.147"** %97, align 8
  store %"class.std::__1::unique_ptr.144"* %208, %"class.std::__1::unique_ptr.144"** %91, align 8
  store %"struct.std::__1::__hash_node.123"* %209, %"struct.std::__1::__hash_node.123"** %92, align 8
  store %"class.std::__1::__hash_node_destructor.147"* %210, %"class.std::__1::__hash_node_destructor.147"** %93, align 8
  %211 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %91, align 8
  %212 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %211, i32 0, i32 0
  %213 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %92, align 8
  %214 = load %"class.std::__1::__hash_node_destructor.147"*, %"class.std::__1::__hash_node_destructor.147"** %93, align 8
  store %"class.std::__1::__hash_node_destructor.147"* %214, %"class.std::__1::__hash_node_destructor.147"** %90, align 8
  %215 = load %"class.std::__1::__hash_node_destructor.147"*, %"class.std::__1::__hash_node_destructor.147"** %90, align 8
  %216 = bitcast %"class.std::__1::__hash_node_destructor.147"* %94 to i8*
  %217 = bitcast %"class.std::__1::__hash_node_destructor.147"* %215 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %216, i8* %217, i64 16, i32 8, i1 false) #2
  %218 = bitcast %"class.std::__1::__hash_node_destructor.147"* %94 to { %"class.std::__1::allocator.23"*, i8 }*
  %219 = getelementptr inbounds { %"class.std::__1::allocator.23"*, i8 }, { %"class.std::__1::allocator.23"*, i8 }* %218, i32 0, i32 0
  %220 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %219, align 8
  %221 = getelementptr inbounds { %"class.std::__1::allocator.23"*, i8 }, { %"class.std::__1::allocator.23"*, i8 }* %218, i32 0, i32 1
  %222 = load i8, i8* %221, align 8
  %223 = bitcast %"class.std::__1::__hash_node_destructor.147"* %87 to { %"class.std::__1::allocator.23"*, i8 }*
  %224 = getelementptr inbounds { %"class.std::__1::allocator.23"*, i8 }, { %"class.std::__1::allocator.23"*, i8 }* %223, i32 0, i32 0
  store %"class.std::__1::allocator.23"* %220, %"class.std::__1::allocator.23"** %224, align 8
  %225 = getelementptr inbounds { %"class.std::__1::allocator.23"*, i8 }, { %"class.std::__1::allocator.23"*, i8 }* %223, i32 0, i32 1
  store i8 %222, i8* %225, align 8
  store %"class.std::__1::__compressed_pair.145"* %212, %"class.std::__1::__compressed_pair.145"** %88, align 8
  store %"struct.std::__1::__hash_node.123"* %213, %"struct.std::__1::__hash_node.123"** %89, align 8
  %226 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %88, align 8
  %227 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %89, align 8
  %228 = bitcast %"class.std::__1::__hash_node_destructor.147"* %87 to { %"class.std::__1::allocator.23"*, i8 }*
  %229 = getelementptr inbounds { %"class.std::__1::allocator.23"*, i8 }, { %"class.std::__1::allocator.23"*, i8 }* %228, i32 0, i32 0
  %230 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %229, align 8
  %231 = getelementptr inbounds { %"class.std::__1::allocator.23"*, i8 }, { %"class.std::__1::allocator.23"*, i8 }* %228, i32 0, i32 1
  %232 = load i8, i8* %231, align 8
  %233 = bitcast %"class.std::__1::__hash_node_destructor.147"* %83 to { %"class.std::__1::allocator.23"*, i8 }*
  %234 = getelementptr inbounds { %"class.std::__1::allocator.23"*, i8 }, { %"class.std::__1::allocator.23"*, i8 }* %233, i32 0, i32 0
  store %"class.std::__1::allocator.23"* %230, %"class.std::__1::allocator.23"** %234, align 8
  %235 = getelementptr inbounds { %"class.std::__1::allocator.23"*, i8 }, { %"class.std::__1::allocator.23"*, i8 }* %233, i32 0, i32 1
  store i8 %232, i8* %235, align 8
  store %"class.std::__1::__compressed_pair.145"* %226, %"class.std::__1::__compressed_pair.145"** %84, align 8
  store %"struct.std::__1::__hash_node.123"* %227, %"struct.std::__1::__hash_node.123"** %85, align 8
  %236 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %84, align 8
  %237 = bitcast %"class.std::__1::__compressed_pair.145"* %236 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"struct.std::__1::__hash_node.123"** %85, %"struct.std::__1::__hash_node.123"*** %82, align 8
  %238 = load %"struct.std::__1::__hash_node.123"**, %"struct.std::__1::__hash_node.123"*** %82, align 8
  %239 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %238, align 8
  store %"class.std::__1::__hash_node_destructor.147"* %83, %"class.std::__1::__hash_node_destructor.147"** %76, align 8
  %240 = load %"class.std::__1::__hash_node_destructor.147"*, %"class.std::__1::__hash_node_destructor.147"** %76, align 8
  %241 = bitcast %"class.std::__1::__hash_node_destructor.147"* %86 to i8*
  %242 = bitcast %"class.std::__1::__hash_node_destructor.147"* %240 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %241, i8* %242, i64 16, i32 8, i1 false) #2
  %243 = bitcast %"class.std::__1::__hash_node_destructor.147"* %86 to { %"class.std::__1::allocator.23"*, i8 }*
  %244 = getelementptr inbounds { %"class.std::__1::allocator.23"*, i8 }, { %"class.std::__1::allocator.23"*, i8 }* %243, i32 0, i32 0
  %245 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %244, align 8
  %246 = getelementptr inbounds { %"class.std::__1::allocator.23"*, i8 }, { %"class.std::__1::allocator.23"*, i8 }* %243, i32 0, i32 1
  %247 = load i8, i8* %246, align 8
  %248 = bitcast %"class.std::__1::__hash_node_destructor.147"* %79 to { %"class.std::__1::allocator.23"*, i8 }*
  %249 = getelementptr inbounds { %"class.std::__1::allocator.23"*, i8 }, { %"class.std::__1::allocator.23"*, i8 }* %248, i32 0, i32 0
  store %"class.std::__1::allocator.23"* %245, %"class.std::__1::allocator.23"** %249, align 8
  %250 = getelementptr inbounds { %"class.std::__1::allocator.23"*, i8 }, { %"class.std::__1::allocator.23"*, i8 }* %248, i32 0, i32 1
  store i8 %247, i8* %250, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %237, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %80, align 8
  store %"struct.std::__1::__hash_node.123"* %239, %"struct.std::__1::__hash_node.123"** %81, align 8
  %251 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %80, align 8
  %252 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %251, i32 0, i32 0
  store %"struct.std::__1::__hash_node.123"** %81, %"struct.std::__1::__hash_node.123"*** %78, align 8
  %253 = load %"struct.std::__1::__hash_node.123"**, %"struct.std::__1::__hash_node.123"*** %78, align 8
  %254 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %253, align 8
  store %"struct.std::__1::__hash_node.123"* %254, %"struct.std::__1::__hash_node.123"** %252, align 8
  %255 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %251, i32 0, i32 1
  store %"class.std::__1::__hash_node_destructor.147"* %79, %"class.std::__1::__hash_node_destructor.147"** %77, align 8
  %256 = load %"class.std::__1::__hash_node_destructor.147"*, %"class.std::__1::__hash_node_destructor.147"** %77, align 8
  %257 = bitcast %"class.std::__1::__hash_node_destructor.147"* %255 to i8*
  %258 = bitcast %"class.std::__1::__hash_node_destructor.147"* %256 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %257, i8* %258, i64 16, i32 8, i1 false) #2
  %259 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %174, align 8
  store %"class.std::__1::unique_ptr.144"* %0, %"class.std::__1::unique_ptr.144"** %9, align 8
  %260 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %9, align 8
  %261 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %260, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %261, %"class.std::__1::__compressed_pair.145"** %8, align 8
  %262 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %8, align 8
  %263 = bitcast %"class.std::__1::__compressed_pair.145"* %262 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %263, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %7, align 8
  %264 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %7, align 8
  %265 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %264, i32 0, i32 0
  %266 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %265, align 8
  %267 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %266, i32 0, i32 2
  store %"union.std::__1::__hash_value_type.124"* %267, %"union.std::__1::__hash_value_type.124"** %11, align 8
  %268 = load %"union.std::__1::__hash_value_type.124"*, %"union.std::__1::__hash_value_type.124"** %11, align 8
  %269 = bitcast %"union.std::__1::__hash_value_type.124"* %268 to %"struct.std::__1::pair.125"*
  store %"struct.std::__1::pair.125"* %269, %"struct.std::__1::pair.125"** %10, align 8
  %270 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %10, align 8
  %271 = bitcast %"struct.std::__1::pair.125"* %270 to i8*
  br label %272

; <label>:272:                                    ; preds = %6
  %273 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %171, align 8
  store %"struct.std::__1::piecewise_construct_t"* %273, %"struct.std::__1::piecewise_construct_t"** %12, align 8
  %274 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %12, align 8
  %275 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %172, align 8
  store %"class.std::__1::tuple.141"* %275, %"class.std::__1::tuple.141"** %13, align 8
  %276 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %13, align 8
  %277 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %173, align 8
  store %"class.std::__1::tuple.131"* %277, %"class.std::__1::tuple.131"** %14, align 8
  %278 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %14, align 8
  store %"class.std::__1::allocator.23"* %259, %"class.std::__1::allocator.23"** %60, align 8
  store %"struct.std::__1::pair.125"* %270, %"struct.std::__1::pair.125"** %61, align 8
  store %"struct.std::__1::piecewise_construct_t"* %274, %"struct.std::__1::piecewise_construct_t"** %62, align 8
  store %"class.std::__1::tuple.141"* %276, %"class.std::__1::tuple.141"** %63, align 8
  store %"class.std::__1::tuple.131"* %278, %"class.std::__1::tuple.131"** %64, align 8
  %279 = bitcast %"struct.std::__1::__has_construct.149"* %66 to %"struct.std::__1::integral_constant.128"*
  %280 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %60, align 8
  %281 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %61, align 8
  %282 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %62, align 8
  store %"struct.std::__1::piecewise_construct_t"* %282, %"struct.std::__1::piecewise_construct_t"** %59, align 8
  %283 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %59, align 8
  %284 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %63, align 8
  store %"class.std::__1::tuple.141"* %284, %"class.std::__1::tuple.141"** %15, align 8
  %285 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %15, align 8
  %286 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %64, align 8
  store %"class.std::__1::tuple.131"* %286, %"class.std::__1::tuple.131"** %16, align 8
  %287 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %16, align 8
  store %"class.std::__1::allocator.23"* %280, %"class.std::__1::allocator.23"** %54, align 8
  store %"struct.std::__1::pair.125"* %281, %"struct.std::__1::pair.125"** %55, align 8
  store %"struct.std::__1::piecewise_construct_t"* %283, %"struct.std::__1::piecewise_construct_t"** %56, align 8
  store %"class.std::__1::tuple.141"* %285, %"class.std::__1::tuple.141"** %57, align 8
  store %"class.std::__1::tuple.131"* %287, %"class.std::__1::tuple.131"** %58, align 8
  %288 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %54, align 8
  %289 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %55, align 8
  %290 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %56, align 8
  store %"struct.std::__1::piecewise_construct_t"* %290, %"struct.std::__1::piecewise_construct_t"** %52, align 8
  %291 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %52, align 8
  %292 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %57, align 8
  store %"class.std::__1::tuple.141"* %292, %"class.std::__1::tuple.141"** %17, align 8
  %293 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %17, align 8
  %294 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %58, align 8
  store %"class.std::__1::tuple.131"* %294, %"class.std::__1::tuple.131"** %18, align 8
  %295 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %18, align 8
  store %"class.std::__1::allocator.23"* %288, %"class.std::__1::allocator.23"** %44, align 8
  store %"struct.std::__1::pair.125"* %289, %"struct.std::__1::pair.125"** %45, align 8
  store %"struct.std::__1::piecewise_construct_t"* %291, %"struct.std::__1::piecewise_construct_t"** %46, align 8
  store %"class.std::__1::tuple.141"* %293, %"class.std::__1::tuple.141"** %47, align 8
  store %"class.std::__1::tuple.131"* %295, %"class.std::__1::tuple.131"** %48, align 8
  %296 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %44, align 8
  %297 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %45, align 8
  %298 = bitcast %"struct.std::__1::pair.125"* %297 to i8*
  %299 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %46, align 8
  store %"struct.std::__1::piecewise_construct_t"* %299, %"struct.std::__1::piecewise_construct_t"** %43, align 8
  %300 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %43, align 8
  %301 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %47, align 8
  store %"class.std::__1::tuple.141"* %301, %"class.std::__1::tuple.141"** %19, align 8
  %302 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %19, align 8
  %303 = bitcast %"class.std::__1::tuple.141"* %50 to i8*
  %304 = bitcast %"class.std::__1::tuple.141"* %302 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %303, i8* %304, i64 8, i32 8, i1 false)
  %305 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %48, align 8
  store %"class.std::__1::tuple.131"* %305, %"class.std::__1::tuple.131"** %20, align 8
  %306 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %20, align 8
  %307 = getelementptr inbounds %"class.std::__1::tuple.141", %"class.std::__1::tuple.141"* %50, i32 0, i32 0
  %308 = getelementptr inbounds %"struct.std::__1::__tuple_impl.142", %"struct.std::__1::__tuple_impl.142"* %307, i32 0, i32 0
  %309 = getelementptr inbounds %"class.std::__1::__tuple_leaf.143", %"class.std::__1::__tuple_leaf.143"* %308, i32 0, i32 0
  %310 = load i32*, i32** %309, align 8
  %311 = getelementptr inbounds %"class.std::__1::tuple.141", %"class.std::__1::tuple.141"* %37, i32 0, i32 0
  %312 = getelementptr inbounds %"struct.std::__1::__tuple_impl.142", %"struct.std::__1::__tuple_impl.142"* %311, i32 0, i32 0
  %313 = getelementptr inbounds %"class.std::__1::__tuple_leaf.143", %"class.std::__1::__tuple_leaf.143"* %312, i32 0, i32 0
  store i32* %310, i32** %313, align 8
  store %"struct.std::__1::pair.125"* %297, %"struct.std::__1::pair.125"** %39, align 8
  %314 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %39, align 8
  store %"struct.std::__1::pair.125"* %314, %"struct.std::__1::pair.125"** %33, align 8
  store %"class.std::__1::tuple.141"* %37, %"class.std::__1::tuple.141"** %34, align 8
  store %"class.std::__1::tuple.131"* %38, %"class.std::__1::tuple.131"** %35, align 8
  %315 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %33, align 8
  %316 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %34, align 8
  %317 = load %"class.std::__1::tuple.131"*, %"class.std::__1::tuple.131"** %35, align 8
  store %"struct.std::__1::pair.125"* %315, %"struct.std::__1::pair.125"** %27, align 8
  store %"class.std::__1::tuple.141"* %316, %"class.std::__1::tuple.141"** %28, align 8
  store %"class.std::__1::tuple.131"* %317, %"class.std::__1::tuple.131"** %29, align 8
  %318 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %27, align 8
  %319 = getelementptr inbounds %"struct.std::__1::pair.125", %"struct.std::__1::pair.125"* %318, i32 0, i32 0
  %320 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %28, align 8
  store %"class.std::__1::tuple.141"* %320, %"class.std::__1::tuple.141"** %23, align 8
  %321 = load %"class.std::__1::tuple.141"*, %"class.std::__1::tuple.141"** %23, align 8
  %322 = getelementptr inbounds %"class.std::__1::tuple.141", %"class.std::__1::tuple.141"* %321, i32 0, i32 0
  %323 = bitcast %"struct.std::__1::__tuple_impl.142"* %322 to %"class.std::__1::__tuple_leaf.143"*
  store %"class.std::__1::__tuple_leaf.143"* %323, %"class.std::__1::__tuple_leaf.143"** %22, align 8
  %324 = load %"class.std::__1::__tuple_leaf.143"*, %"class.std::__1::__tuple_leaf.143"** %22, align 8
  %325 = getelementptr inbounds %"class.std::__1::__tuple_leaf.143", %"class.std::__1::__tuple_leaf.143"* %324, i32 0, i32 0
  %326 = load i32*, i32** %325, align 8
  store i32* %326, i32** %21, align 8
  %327 = load i32*, i32** %21, align 8
  %328 = load i32, i32* %327, align 4
  store i32 %328, i32* %319, align 8
  %329 = getelementptr inbounds %"struct.std::__1::pair.125", %"struct.std::__1::pair.125"* %318, i32 0, i32 1
  store %"class.llvm::StructType"* null, %"class.llvm::StructType"** %329, align 8
  br label %330

; <label>:330:                                    ; preds = %272
  store %"class.std::__1::unique_ptr.144"* %0, %"class.std::__1::unique_ptr.144"** %69, align 8
  %331 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %69, align 8
  %332 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %331, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %332, %"class.std::__1::__compressed_pair.145"** %68, align 8
  %333 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %68, align 8
  %334 = bitcast %"class.std::__1::__compressed_pair.145"* %333 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %334, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %67, align 8
  %335 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %67, align 8
  %336 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %335, i32 0, i32 1
  %337 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %336, i32 0, i32 1
  store i8 1, i8* %337, align 8
  %338 = load i64, i64* %170, align 8
  store %"class.std::__1::unique_ptr.144"* %0, %"class.std::__1::unique_ptr.144"** %72, align 8
  %339 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %72, align 8
  %340 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %339, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %340, %"class.std::__1::__compressed_pair.145"** %71, align 8
  %341 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %71, align 8
  %342 = bitcast %"class.std::__1::__compressed_pair.145"* %341 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %342, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %70, align 8
  %343 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %70, align 8
  %344 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %343, i32 0, i32 0
  %345 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %344, align 8
  %346 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %345, i32 0, i32 1
  store i64 %338, i64* %346, align 8
  store %"class.std::__1::unique_ptr.144"* %0, %"class.std::__1::unique_ptr.144"** %75, align 8
  %347 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %75, align 8
  %348 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %347, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %348, %"class.std::__1::__compressed_pair.145"** %74, align 8
  %349 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %74, align 8
  %350 = bitcast %"class.std::__1::__compressed_pair.145"* %349 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %350, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %73, align 8
  %351 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %73, align 8
  %352 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %351, i32 0, i32 0
  %353 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %352, align 8
  %354 = bitcast %"struct.std::__1::__hash_node.123"* %353 to %"struct.std::__1::__hash_node_base.15"*
  %355 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %354, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"* null, %"struct.std::__1::__hash_node_base.15"** %355, align 8
  store i1 true, i1* %175, align 1
  %356 = load i1, i1* %175, align 1
  br i1 %356, label %476, label %418
                                                  ; No predecessors!
  %358 = landingpad { i8*, i32 }
          cleanup
  %359 = extractvalue { i8*, i32 } %358, 0
  store i8* %359, i8** %177, align 8
  %360 = extractvalue { i8*, i32 } %358, 1
  store i32 %360, i32* %178, align 4
  store %"class.std::__1::unique_ptr.144"* %0, %"class.std::__1::unique_ptr.144"** %125, align 8
  %361 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %125, align 8
  store %"class.std::__1::unique_ptr.144"* %361, %"class.std::__1::unique_ptr.144"** %124, align 8
  %362 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %124, align 8
  store %"class.std::__1::unique_ptr.144"* %362, %"class.std::__1::unique_ptr.144"** %121, align 8
  store %"struct.std::__1::__hash_node.123"* null, %"struct.std::__1::__hash_node.123"** %122, align 8
  %363 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %121, align 8
  %364 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %363, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %364, %"class.std::__1::__compressed_pair.145"** %120, align 8
  %365 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %120, align 8
  %366 = bitcast %"class.std::__1::__compressed_pair.145"* %365 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %366, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %119, align 8
  %367 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %119, align 8
  %368 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %367, i32 0, i32 0
  %369 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %368, align 8
  store %"struct.std::__1::__hash_node.123"* %369, %"struct.std::__1::__hash_node.123"** %123, align 8
  %370 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %122, align 8
  %371 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %363, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %371, %"class.std::__1::__compressed_pair.145"** %101, align 8
  %372 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %101, align 8
  %373 = bitcast %"class.std::__1::__compressed_pair.145"* %372 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %373, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %100, align 8
  %374 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %100, align 8
  %375 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %374, i32 0, i32 0
  store %"struct.std::__1::__hash_node.123"* %370, %"struct.std::__1::__hash_node.123"** %375, align 8
  %376 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %123, align 8
  %377 = icmp ne %"struct.std::__1::__hash_node.123"* %376, null
  br i1 %377, label %378, label %417

; <label>:378:                                    ; preds = %357
  %379 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %363, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %379, %"class.std::__1::__compressed_pair.145"** %99, align 8
  %380 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %99, align 8
  %381 = bitcast %"class.std::__1::__compressed_pair.145"* %380 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %381, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %98, align 8
  %382 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %98, align 8
  %383 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %382, i32 0, i32 1
  %384 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %123, align 8
  store %"class.std::__1::__hash_node_destructor.147"* %383, %"class.std::__1::__hash_node_destructor.147"** %117, align 8
  store %"struct.std::__1::__hash_node.123"* %384, %"struct.std::__1::__hash_node.123"** %118, align 8
  %385 = load %"class.std::__1::__hash_node_destructor.147"*, %"class.std::__1::__hash_node_destructor.147"** %117, align 8
  %386 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %385, i32 0, i32 1
  %387 = load i8, i8* %386, align 8
  %388 = trunc i8 %387 to i1
  br i1 %388, label %389, label %402

; <label>:389:                                    ; preds = %378
  %390 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %385, i32 0, i32 0
  %391 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %390, align 8
  %392 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %118, align 8
  %393 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %392, i32 0, i32 2
  store %"union.std::__1::__hash_value_type.124"* %393, %"union.std::__1::__hash_value_type.124"** %116, align 8
  %394 = load %"union.std::__1::__hash_value_type.124"*, %"union.std::__1::__hash_value_type.124"** %116, align 8
  %395 = bitcast %"union.std::__1::__hash_value_type.124"* %394 to %"struct.std::__1::pair.125"*
  store %"struct.std::__1::pair.125"* %395, %"struct.std::__1::pair.125"** %115, align 8
  %396 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %115, align 8
  %397 = bitcast %"struct.std::__1::pair.125"* %396 to i8*
  store %"class.std::__1::allocator.23"* %391, %"class.std::__1::allocator.23"** %111, align 8
  store %"struct.std::__1::pair.125"* %396, %"struct.std::__1::pair.125"** %112, align 8
  %398 = bitcast %"struct.std::__1::__has_destroy.127"* %114 to %"struct.std::__1::integral_constant"*
  %399 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %111, align 8
  %400 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %112, align 8
  store %"class.std::__1::allocator.23"* %399, %"class.std::__1::allocator.23"** %109, align 8
  store %"struct.std::__1::pair.125"* %400, %"struct.std::__1::pair.125"** %110, align 8
  %401 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %110, align 8
  br label %402

; <label>:402:                                    ; preds = %389, %378
  %403 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %118, align 8
  %404 = icmp ne %"struct.std::__1::__hash_node.123"* %403, null
  br i1 %404, label %405, label %416

; <label>:405:                                    ; preds = %402
  %406 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %385, i32 0, i32 0
  %407 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %406, align 8
  %408 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %118, align 8
  store %"class.std::__1::allocator.23"* %407, %"class.std::__1::allocator.23"** %106, align 8
  store %"struct.std::__1::__hash_node.123"* %408, %"struct.std::__1::__hash_node.123"** %107, align 8
  store i64 1, i64* %108, align 8
  %409 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %106, align 8
  %410 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %107, align 8
  %411 = load i64, i64* %108, align 8
  store %"class.std::__1::allocator.23"* %409, %"class.std::__1::allocator.23"** %103, align 8
  store %"struct.std::__1::__hash_node.123"* %410, %"struct.std::__1::__hash_node.123"** %104, align 8
  store i64 %411, i64* %105, align 8
  %412 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %103, align 8
  %413 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %104, align 8
  %414 = bitcast %"struct.std::__1::__hash_node.123"* %413 to i8*
  store i8* %414, i8** %102, align 8
  %415 = load i8*, i8** %102, align 8
  call void @_ZdlPv(i8* %415) #14
  br label %416

; <label>:416:                                    ; preds = %405, %402
  br label %417

; <label>:417:                                    ; preds = %357, %416
  br label %477

; <label>:418:                                    ; preds = %330
  store %"class.std::__1::unique_ptr.144"* %0, %"class.std::__1::unique_ptr.144"** %165, align 8
  %419 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %165, align 8
  store %"class.std::__1::unique_ptr.144"* %419, %"class.std::__1::unique_ptr.144"** %164, align 8
  %420 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %164, align 8
  store %"class.std::__1::unique_ptr.144"* %420, %"class.std::__1::unique_ptr.144"** %161, align 8
  store %"struct.std::__1::__hash_node.123"* null, %"struct.std::__1::__hash_node.123"** %162, align 8
  %421 = load %"class.std::__1::unique_ptr.144"*, %"class.std::__1::unique_ptr.144"** %161, align 8
  %422 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %421, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %422, %"class.std::__1::__compressed_pair.145"** %160, align 8
  %423 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %160, align 8
  %424 = bitcast %"class.std::__1::__compressed_pair.145"* %423 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %424, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %159, align 8
  %425 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %159, align 8
  %426 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %425, i32 0, i32 0
  %427 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %426, align 8
  store %"struct.std::__1::__hash_node.123"* %427, %"struct.std::__1::__hash_node.123"** %163, align 8
  %428 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %162, align 8
  %429 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %421, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %429, %"class.std::__1::__compressed_pair.145"** %141, align 8
  %430 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %141, align 8
  %431 = bitcast %"class.std::__1::__compressed_pair.145"* %430 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %431, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %140, align 8
  %432 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %140, align 8
  %433 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %432, i32 0, i32 0
  store %"struct.std::__1::__hash_node.123"* %428, %"struct.std::__1::__hash_node.123"** %433, align 8
  %434 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %163, align 8
  %435 = icmp ne %"struct.std::__1::__hash_node.123"* %434, null
  br i1 %435, label %436, label %475

; <label>:436:                                    ; preds = %418
  %437 = getelementptr inbounds %"class.std::__1::unique_ptr.144", %"class.std::__1::unique_ptr.144"* %421, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.145"* %437, %"class.std::__1::__compressed_pair.145"** %139, align 8
  %438 = load %"class.std::__1::__compressed_pair.145"*, %"class.std::__1::__compressed_pair.145"** %139, align 8
  %439 = bitcast %"class.std::__1::__compressed_pair.145"* %438 to %"class.std::__1::__libcpp_compressed_pair_imp.146"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.146"* %439, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %138, align 8
  %440 = load %"class.std::__1::__libcpp_compressed_pair_imp.146"*, %"class.std::__1::__libcpp_compressed_pair_imp.146"** %138, align 8
  %441 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.146", %"class.std::__1::__libcpp_compressed_pair_imp.146"* %440, i32 0, i32 1
  %442 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %163, align 8
  store %"class.std::__1::__hash_node_destructor.147"* %441, %"class.std::__1::__hash_node_destructor.147"** %157, align 8
  store %"struct.std::__1::__hash_node.123"* %442, %"struct.std::__1::__hash_node.123"** %158, align 8
  %443 = load %"class.std::__1::__hash_node_destructor.147"*, %"class.std::__1::__hash_node_destructor.147"** %157, align 8
  %444 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %443, i32 0, i32 1
  %445 = load i8, i8* %444, align 8
  %446 = trunc i8 %445 to i1
  br i1 %446, label %447, label %460

; <label>:447:                                    ; preds = %436
  %448 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %443, i32 0, i32 0
  %449 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %448, align 8
  %450 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %158, align 8
  %451 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %450, i32 0, i32 2
  store %"union.std::__1::__hash_value_type.124"* %451, %"union.std::__1::__hash_value_type.124"** %156, align 8
  %452 = load %"union.std::__1::__hash_value_type.124"*, %"union.std::__1::__hash_value_type.124"** %156, align 8
  %453 = bitcast %"union.std::__1::__hash_value_type.124"* %452 to %"struct.std::__1::pair.125"*
  store %"struct.std::__1::pair.125"* %453, %"struct.std::__1::pair.125"** %155, align 8
  %454 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %155, align 8
  %455 = bitcast %"struct.std::__1::pair.125"* %454 to i8*
  store %"class.std::__1::allocator.23"* %449, %"class.std::__1::allocator.23"** %151, align 8
  store %"struct.std::__1::pair.125"* %454, %"struct.std::__1::pair.125"** %152, align 8
  %456 = bitcast %"struct.std::__1::__has_destroy.127"* %154 to %"struct.std::__1::integral_constant"*
  %457 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %151, align 8
  %458 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %152, align 8
  store %"class.std::__1::allocator.23"* %457, %"class.std::__1::allocator.23"** %149, align 8
  store %"struct.std::__1::pair.125"* %458, %"struct.std::__1::pair.125"** %150, align 8
  %459 = load %"struct.std::__1::pair.125"*, %"struct.std::__1::pair.125"** %150, align 8
  br label %460

; <label>:460:                                    ; preds = %447, %436
  %461 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %158, align 8
  %462 = icmp ne %"struct.std::__1::__hash_node.123"* %461, null
  br i1 %462, label %463, label %474

; <label>:463:                                    ; preds = %460
  %464 = getelementptr inbounds %"class.std::__1::__hash_node_destructor.147", %"class.std::__1::__hash_node_destructor.147"* %443, i32 0, i32 0
  %465 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %464, align 8
  %466 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %158, align 8
  store %"class.std::__1::allocator.23"* %465, %"class.std::__1::allocator.23"** %146, align 8
  store %"struct.std::__1::__hash_node.123"* %466, %"struct.std::__1::__hash_node.123"** %147, align 8
  store i64 1, i64* %148, align 8
  %467 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %146, align 8
  %468 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %147, align 8
  %469 = load i64, i64* %148, align 8
  store %"class.std::__1::allocator.23"* %467, %"class.std::__1::allocator.23"** %143, align 8
  store %"struct.std::__1::__hash_node.123"* %468, %"struct.std::__1::__hash_node.123"** %144, align 8
  store i64 %469, i64* %145, align 8
  %470 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %143, align 8
  %471 = load %"struct.std::__1::__hash_node.123"*, %"struct.std::__1::__hash_node.123"** %144, align 8
  %472 = bitcast %"struct.std::__1::__hash_node.123"* %471 to i8*
  store i8* %472, i8** %142, align 8
  %473 = load i8*, i8** %142, align 8
  call void @_ZdlPv(i8* %473) #14
  br label %474

; <label>:474:                                    ; preds = %463, %460
  br label %475

; <label>:475:                                    ; preds = %418, %474
  br label %476

; <label>:476:                                    ; preds = %475, %330
  ret void

; <label>:477:                                    ; preds = %417
  %478 = load i8*, i8** %177, align 8
  %479 = load i32, i32* %178, align 4
  %480 = insertvalue { i8*, i32 } undef, i8* %478, 0
  %481 = insertvalue { i8*, i32 } %480, i32 %479, 1
  resume { i8*, i32 } %481
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeIjPN4llvm10StructTypeEEENS_22__unordered_map_hasherIjS5_NS_4hashIjEELb1EEENS_21__unordered_map_equalIjS5_NS_8equal_toIjEELb1EEENS_9allocatorIS5_EEE6rehashEm(%"class.std::__1::__hash_table.11"*, i64) #0 align 2 {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.31"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair.30"*, align 8
  %5 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %6 = alloca float, align 4
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  %9 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.26"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair.25"*, align 8
  %11 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.31"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair.30"*, align 8
  %14 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %15 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.26"*, align 8
  %16 = alloca %"class.std::__1::__compressed_pair.25"*, align 8
  %17 = alloca %"class.std::__1::__hash_table.11"*, align 8
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
  %29 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.18"*, align 8
  %30 = alloca %"class.std::__1::__compressed_pair.17"*, align 8
  %31 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %32 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %33 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %34 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %35 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %36 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %37 = alloca i64, align 8
  %38 = alloca i64, align 8
  %39 = alloca i64, align 8
  store %"class.std::__1::__hash_table.11"* %0, %"class.std::__1::__hash_table.11"** %36, align 8
  store i64 %1, i64* %37, align 8
  %40 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %36, align 8
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
  store %"class.std::__1::__hash_table.11"* %40, %"class.std::__1::__hash_table.11"** %35, align 8
  %55 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %35, align 8
  %56 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %55, i32 0, i32 0
  store %"class.std::__1::unique_ptr.12"* %56, %"class.std::__1::unique_ptr.12"** %34, align 8
  %57 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %34, align 8
  %58 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %57, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %58, %"class.std::__1::__compressed_pair.13"** %33, align 8
  %59 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %33, align 8
  %60 = bitcast %"class.std::__1::__compressed_pair.13"* %59 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %60, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %32, align 8
  %61 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %32, align 8
  %62 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %61, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator.16"* %62, %"class.std::__1::__bucket_list_deallocator.16"** %31, align 8
  %63 = load %"class.std::__1::__bucket_list_deallocator.16"*, %"class.std::__1::__bucket_list_deallocator.16"** %31, align 8
  %64 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.16", %"class.std::__1::__bucket_list_deallocator.16"* %63, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.17"* %64, %"class.std::__1::__compressed_pair.17"** %30, align 8
  %65 = load %"class.std::__1::__compressed_pair.17"*, %"class.std::__1::__compressed_pair.17"** %30, align 8
  %66 = bitcast %"class.std::__1::__compressed_pair.17"* %65 to %"class.std::__1::__libcpp_compressed_pair_imp.18"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.18"* %66, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %29, align 8
  %67 = load %"class.std::__1::__libcpp_compressed_pair_imp.18"*, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %29, align 8
  %68 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.18", %"class.std::__1::__libcpp_compressed_pair_imp.18"* %67, i32 0, i32 0
  %69 = load i64, i64* %68, align 8
  store i64 %69, i64* %38, align 8
  %70 = load i64, i64* %37, align 8
  %71 = load i64, i64* %38, align 8
  %72 = icmp ugt i64 %70, %71
  br i1 %72, label %73, label %75

; <label>:73:                                     ; preds = %54
  %74 = load i64, i64* %37, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeIjPN4llvm10StructTypeEEENS_22__unordered_map_hasherIjS5_NS_4hashIjEELb1EEENS_21__unordered_map_equalIjS5_NS_8equal_toIjEELb1EEENS_9allocatorIS5_EEE8__rehashEm(%"class.std::__1::__hash_table.11"* %40, i64 %74)
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
  store %"class.std::__1::__hash_table.11"* %40, %"class.std::__1::__hash_table.11"** %17, align 8
  %93 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %17, align 8
  %94 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %93, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.25"* %94, %"class.std::__1::__compressed_pair.25"** %16, align 8
  %95 = load %"class.std::__1::__compressed_pair.25"*, %"class.std::__1::__compressed_pair.25"** %16, align 8
  %96 = bitcast %"class.std::__1::__compressed_pair.25"* %95 to %"class.std::__1::__libcpp_compressed_pair_imp.26"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.26"* %96, %"class.std::__1::__libcpp_compressed_pair_imp.26"** %15, align 8
  %97 = load %"class.std::__1::__libcpp_compressed_pair_imp.26"*, %"class.std::__1::__libcpp_compressed_pair_imp.26"** %15, align 8
  %98 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.26", %"class.std::__1::__libcpp_compressed_pair_imp.26"* %97, i32 0, i32 0
  %99 = load i64, i64* %98, align 8
  %100 = uitofp i64 %99 to float
  store %"class.std::__1::__hash_table.11"* %40, %"class.std::__1::__hash_table.11"** %5, align 8
  %101 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %5, align 8
  %102 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %101, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.30"* %102, %"class.std::__1::__compressed_pair.30"** %4, align 8
  %103 = load %"class.std::__1::__compressed_pair.30"*, %"class.std::__1::__compressed_pair.30"** %4, align 8
  %104 = bitcast %"class.std::__1::__compressed_pair.30"* %103 to %"class.std::__1::__libcpp_compressed_pair_imp.31"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.31"* %104, %"class.std::__1::__libcpp_compressed_pair_imp.31"** %3, align 8
  %105 = load %"class.std::__1::__libcpp_compressed_pair_imp.31"*, %"class.std::__1::__libcpp_compressed_pair_imp.31"** %3, align 8
  %106 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.31", %"class.std::__1::__libcpp_compressed_pair_imp.31"* %105, i32 0, i32 0
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
  %115 = call i64 @llvm.ctlz.i64(i64 %114, i1 true) #2
  %116 = trunc i64 %115 to i32
  %117 = sext i32 %116 to i64
  %118 = sub i64 64, %117
  %119 = shl i64 1, %118
  br label %141

; <label>:120:                                    ; preds = %90
  store %"class.std::__1::__hash_table.11"* %40, %"class.std::__1::__hash_table.11"** %11, align 8
  %121 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %11, align 8
  %122 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %121, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.25"* %122, %"class.std::__1::__compressed_pair.25"** %10, align 8
  %123 = load %"class.std::__1::__compressed_pair.25"*, %"class.std::__1::__compressed_pair.25"** %10, align 8
  %124 = bitcast %"class.std::__1::__compressed_pair.25"* %123 to %"class.std::__1::__libcpp_compressed_pair_imp.26"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.26"* %124, %"class.std::__1::__libcpp_compressed_pair_imp.26"** %9, align 8
  %125 = load %"class.std::__1::__libcpp_compressed_pair_imp.26"*, %"class.std::__1::__libcpp_compressed_pair_imp.26"** %9, align 8
  %126 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.26", %"class.std::__1::__libcpp_compressed_pair_imp.26"* %125, i32 0, i32 0
  %127 = load i64, i64* %126, align 8
  %128 = uitofp i64 %127 to float
  store %"class.std::__1::__hash_table.11"* %40, %"class.std::__1::__hash_table.11"** %14, align 8
  %129 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %14, align 8
  %130 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %129, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.30"* %130, %"class.std::__1::__compressed_pair.30"** %13, align 8
  %131 = load %"class.std::__1::__compressed_pair.30"*, %"class.std::__1::__compressed_pair.30"** %13, align 8
  %132 = bitcast %"class.std::__1::__compressed_pair.30"* %131 to %"class.std::__1::__libcpp_compressed_pair_imp.31"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.31"* %132, %"class.std::__1::__libcpp_compressed_pair_imp.31"** %12, align 8
  %133 = load %"class.std::__1::__libcpp_compressed_pair_imp.31"*, %"class.std::__1::__libcpp_compressed_pair_imp.31"** %12, align 8
  %134 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.31", %"class.std::__1::__libcpp_compressed_pair_imp.31"* %133, i32 0, i32 0
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
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeIjPN4llvm10StructTypeEEENS_22__unordered_map_hasherIjS5_NS_4hashIjEELb1EEENS_21__unordered_map_equalIjS5_NS_8equal_toIjEELb1EEENS_9allocatorIS5_EEE8__rehashEm(%"class.std::__1::__hash_table.11"* %40, i64 %164)
  br label %165

; <label>:165:                                    ; preds = %163, %157
  br label %166

; <label>:166:                                    ; preds = %165, %75
  br label %167

; <label>:167:                                    ; preds = %166, %73
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeIjPN4llvm10StructTypeEEENS_22__unordered_map_hasherIjS5_NS_4hashIjEELb1EEENS_21__unordered_map_equalIjS5_NS_8equal_toIjEELb1EEENS_9allocatorIS5_EEE8__rehashEm(%"class.std::__1::__hash_table.11"*, i64) #0 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %3 = alloca i64, align 8
  %4 = alloca %"class.std::__1::allocator.19"*, align 8
  %5 = alloca i64, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %"class.std::__1::allocator.19"*, align 8
  %8 = alloca i64, align 8
  %9 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.18"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair.17"*, align 8
  %11 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %14 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %15 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %16 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.18"*, align 8
  %17 = alloca %"class.std::__1::__compressed_pair.17"*, align 8
  %18 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %19 = alloca i8*, align 8
  %20 = alloca %"class.std::__1::allocator.19"*, align 8
  %21 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %22 = alloca i64, align 8
  %23 = alloca %"class.std::__1::allocator.19"*, align 8
  %24 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %25 = alloca i64, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.18"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.17"*, align 8
  %28 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %29 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %30 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %31 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %32 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %33 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %34 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %35 = alloca %"struct.std::__1::__hash_node_base.15"**, align 8
  %36 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %37 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %38 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %39 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.18"*, align 8
  %40 = alloca %"class.std::__1::__compressed_pair.17"*, align 8
  %41 = alloca %"class.std::__1::__bucket_list_deallocator.16"*, align 8
  %42 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %43 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %44 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %45 = alloca i64, align 8
  %46 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.22"*, align 8
  %47 = alloca %"class.std::__1::__compressed_pair.21"*, align 8
  %48 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %49 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %50 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %51 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %52 = alloca i64, align 8
  %53 = alloca i64, align 8
  %54 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %55 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %56 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %57 = alloca i64, align 8
  %58 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %59 = alloca i64, align 8
  %60 = alloca i64, align 8
  %61 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %62 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %63 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %64 = alloca i64, align 8
  %65 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %66 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %67 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %68 = alloca i64, align 8
  %69 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.31"*, align 8
  %70 = alloca %"class.std::__1::__compressed_pair.30"*, align 8
  %71 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %72 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %73 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %74 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %75 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %76 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %77 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %78 = alloca %"struct.std::__1::equal_to.33"*, align 8
  %79 = alloca i32*, align 8
  %80 = alloca i32*, align 8
  %81 = alloca %"class.std::__1::__unordered_map_equal.32"*, align 8
  %82 = alloca %"union.std::__1::__hash_value_type.124"*, align 8
  %83 = alloca %"union.std::__1::__hash_value_type.124"*, align 8
  %84 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %85 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %86 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %87 = alloca i64, align 8
  %88 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %89 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %90 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %91 = alloca i64, align 8
  %92 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.14"*, align 8
  %93 = alloca %"class.std::__1::__compressed_pair.13"*, align 8
  %94 = alloca %"class.std::__1::unique_ptr.12"*, align 8
  %95 = alloca %"class.std::__1::__hash_table.11"*, align 8
  %96 = alloca i64, align 8
  %97 = alloca %"class.std::__1::allocator.19"*, align 8
  %98 = alloca i64, align 8
  %99 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %100 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  %101 = alloca i64, align 8
  %102 = alloca i64, align 8
  %103 = alloca %"struct.std::__1::__hash_node_base.15"*, align 8
  store %"class.std::__1::__hash_table.11"* %0, %"class.std::__1::__hash_table.11"** %95, align 8
  store i64 %1, i64* %96, align 8
  %104 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %95, align 8
  %105 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %104, i32 0, i32 0
  store %"class.std::__1::unique_ptr.12"* %105, %"class.std::__1::unique_ptr.12"** %94, align 8
  %106 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %94, align 8
  %107 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %106, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %107, %"class.std::__1::__compressed_pair.13"** %93, align 8
  %108 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %93, align 8
  %109 = bitcast %"class.std::__1::__compressed_pair.13"* %108 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %109, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %92, align 8
  %110 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %92, align 8
  %111 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %110, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator.16"* %111, %"class.std::__1::__bucket_list_deallocator.16"** %11, align 8
  %112 = load %"class.std::__1::__bucket_list_deallocator.16"*, %"class.std::__1::__bucket_list_deallocator.16"** %11, align 8
  %113 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.16", %"class.std::__1::__bucket_list_deallocator.16"* %112, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.17"* %113, %"class.std::__1::__compressed_pair.17"** %10, align 8
  %114 = load %"class.std::__1::__compressed_pair.17"*, %"class.std::__1::__compressed_pair.17"** %10, align 8
  %115 = bitcast %"class.std::__1::__compressed_pair.17"* %114 to %"class.std::__1::__libcpp_compressed_pair_imp.18"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.18"* %115, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %9, align 8
  %116 = load %"class.std::__1::__libcpp_compressed_pair_imp.18"*, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %9, align 8
  %117 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.18"* %116 to %"class.std::__1::allocator.19"*
  store %"class.std::__1::allocator.19"* %117, %"class.std::__1::allocator.19"** %97, align 8
  %118 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %104, i32 0, i32 0
  %119 = load i64, i64* %96, align 8
  %120 = icmp ugt i64 %119, 0
  br i1 %120, label %121, label %132

; <label>:121:                                    ; preds = %2
  %122 = load %"class.std::__1::allocator.19"*, %"class.std::__1::allocator.19"** %97, align 8
  %123 = load i64, i64* %96, align 8
  store %"class.std::__1::allocator.19"* %122, %"class.std::__1::allocator.19"** %7, align 8
  store i64 %123, i64* %8, align 8
  %124 = load %"class.std::__1::allocator.19"*, %"class.std::__1::allocator.19"** %7, align 8
  %125 = load i64, i64* %8, align 8
  store %"class.std::__1::allocator.19"* %124, %"class.std::__1::allocator.19"** %4, align 8
  store i64 %125, i64* %5, align 8
  store i8* null, i8** %6, align 8
  %126 = load %"class.std::__1::allocator.19"*, %"class.std::__1::allocator.19"** %4, align 8
  %127 = load i64, i64* %5, align 8
  %128 = mul i64 %127, 8
  store i64 %128, i64* %3, align 8
  %129 = load i64, i64* %3, align 8
  %130 = call i8* @_Znwm(i64 %129) #17
  %131 = bitcast i8* %130 to %"struct.std::__1::__hash_node_base.15"**
  br label %133

; <label>:132:                                    ; preds = %2
  br label %133

; <label>:133:                                    ; preds = %132, %121
  %134 = phi %"struct.std::__1::__hash_node_base.15"** [ %131, %121 ], [ null, %132 ]
  store %"class.std::__1::unique_ptr.12"* %118, %"class.std::__1::unique_ptr.12"** %33, align 8
  store %"struct.std::__1::__hash_node_base.15"** %134, %"struct.std::__1::__hash_node_base.15"*** %34, align 8
  %135 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %33, align 8
  %136 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %135, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %136, %"class.std::__1::__compressed_pair.13"** %32, align 8
  %137 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %32, align 8
  %138 = bitcast %"class.std::__1::__compressed_pair.13"* %137 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %138, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %31, align 8
  %139 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %31, align 8
  %140 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %139, i32 0, i32 0
  %141 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %140, align 8
  store %"struct.std::__1::__hash_node_base.15"** %141, %"struct.std::__1::__hash_node_base.15"*** %35, align 8
  %142 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %34, align 8
  %143 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %135, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %143, %"class.std::__1::__compressed_pair.13"** %15, align 8
  %144 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %15, align 8
  %145 = bitcast %"class.std::__1::__compressed_pair.13"* %144 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %145, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %14, align 8
  %146 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %14, align 8
  %147 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %146, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"** %142, %"struct.std::__1::__hash_node_base.15"*** %147, align 8
  %148 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %35, align 8
  %149 = icmp ne %"struct.std::__1::__hash_node_base.15"** %148, null
  br i1 %149, label %150, label %179

; <label>:150:                                    ; preds = %133
  %151 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %135, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %151, %"class.std::__1::__compressed_pair.13"** %13, align 8
  %152 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %13, align 8
  %153 = bitcast %"class.std::__1::__compressed_pair.13"* %152 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %153, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %12, align 8
  %154 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %12, align 8
  %155 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %154, i32 0, i32 1
  %156 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %35, align 8
  store %"class.std::__1::__bucket_list_deallocator.16"* %155, %"class.std::__1::__bucket_list_deallocator.16"** %29, align 8
  store %"struct.std::__1::__hash_node_base.15"** %156, %"struct.std::__1::__hash_node_base.15"*** %30, align 8
  %157 = load %"class.std::__1::__bucket_list_deallocator.16"*, %"class.std::__1::__bucket_list_deallocator.16"** %29, align 8
  store %"class.std::__1::__bucket_list_deallocator.16"* %157, %"class.std::__1::__bucket_list_deallocator.16"** %28, align 8
  %158 = load %"class.std::__1::__bucket_list_deallocator.16"*, %"class.std::__1::__bucket_list_deallocator.16"** %28, align 8
  %159 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.16", %"class.std::__1::__bucket_list_deallocator.16"* %158, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.17"* %159, %"class.std::__1::__compressed_pair.17"** %27, align 8
  %160 = load %"class.std::__1::__compressed_pair.17"*, %"class.std::__1::__compressed_pair.17"** %27, align 8
  %161 = bitcast %"class.std::__1::__compressed_pair.17"* %160 to %"class.std::__1::__libcpp_compressed_pair_imp.18"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.18"* %161, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %26, align 8
  %162 = load %"class.std::__1::__libcpp_compressed_pair_imp.18"*, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %26, align 8
  %163 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.18"* %162 to %"class.std::__1::allocator.19"*
  %164 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %30, align 8
  store %"class.std::__1::__bucket_list_deallocator.16"* %157, %"class.std::__1::__bucket_list_deallocator.16"** %18, align 8
  %165 = load %"class.std::__1::__bucket_list_deallocator.16"*, %"class.std::__1::__bucket_list_deallocator.16"** %18, align 8
  %166 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.16", %"class.std::__1::__bucket_list_deallocator.16"* %165, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.17"* %166, %"class.std::__1::__compressed_pair.17"** %17, align 8
  %167 = load %"class.std::__1::__compressed_pair.17"*, %"class.std::__1::__compressed_pair.17"** %17, align 8
  %168 = bitcast %"class.std::__1::__compressed_pair.17"* %167 to %"class.std::__1::__libcpp_compressed_pair_imp.18"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.18"* %168, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %16, align 8
  %169 = load %"class.std::__1::__libcpp_compressed_pair_imp.18"*, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %16, align 8
  %170 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.18", %"class.std::__1::__libcpp_compressed_pair_imp.18"* %169, i32 0, i32 0
  %171 = load i64, i64* %170, align 8
  store %"class.std::__1::allocator.19"* %163, %"class.std::__1::allocator.19"** %23, align 8
  store %"struct.std::__1::__hash_node_base.15"** %164, %"struct.std::__1::__hash_node_base.15"*** %24, align 8
  store i64 %171, i64* %25, align 8
  %172 = load %"class.std::__1::allocator.19"*, %"class.std::__1::allocator.19"** %23, align 8
  %173 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %24, align 8
  %174 = load i64, i64* %25, align 8
  store %"class.std::__1::allocator.19"* %172, %"class.std::__1::allocator.19"** %20, align 8
  store %"struct.std::__1::__hash_node_base.15"** %173, %"struct.std::__1::__hash_node_base.15"*** %21, align 8
  store i64 %174, i64* %22, align 8
  %175 = load %"class.std::__1::allocator.19"*, %"class.std::__1::allocator.19"** %20, align 8
  %176 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %21, align 8
  %177 = bitcast %"struct.std::__1::__hash_node_base.15"** %176 to i8*
  store i8* %177, i8** %19, align 8
  %178 = load i8*, i8** %19, align 8
  call void @_ZdlPv(i8* %178) #14
  br label %179

; <label>:179:                                    ; preds = %133, %150
  %180 = load i64, i64* %96, align 8
  %181 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %104, i32 0, i32 0
  store %"class.std::__1::unique_ptr.12"* %181, %"class.std::__1::unique_ptr.12"** %38, align 8
  %182 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %38, align 8
  %183 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %182, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %183, %"class.std::__1::__compressed_pair.13"** %37, align 8
  %184 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %37, align 8
  %185 = bitcast %"class.std::__1::__compressed_pair.13"* %184 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %185, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %36, align 8
  %186 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %36, align 8
  %187 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %186, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator.16"* %187, %"class.std::__1::__bucket_list_deallocator.16"** %41, align 8
  %188 = load %"class.std::__1::__bucket_list_deallocator.16"*, %"class.std::__1::__bucket_list_deallocator.16"** %41, align 8
  %189 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.16", %"class.std::__1::__bucket_list_deallocator.16"* %188, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.17"* %189, %"class.std::__1::__compressed_pair.17"** %40, align 8
  %190 = load %"class.std::__1::__compressed_pair.17"*, %"class.std::__1::__compressed_pair.17"** %40, align 8
  %191 = bitcast %"class.std::__1::__compressed_pair.17"* %190 to %"class.std::__1::__libcpp_compressed_pair_imp.18"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.18"* %191, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %39, align 8
  %192 = load %"class.std::__1::__libcpp_compressed_pair_imp.18"*, %"class.std::__1::__libcpp_compressed_pair_imp.18"** %39, align 8
  %193 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.18", %"class.std::__1::__libcpp_compressed_pair_imp.18"* %192, i32 0, i32 0
  store i64 %180, i64* %193, align 8
  %194 = load i64, i64* %96, align 8
  %195 = icmp ugt i64 %194, 0
  br i1 %195, label %196, label %442

; <label>:196:                                    ; preds = %179
  store i64 0, i64* %98, align 8
  br label %197

; <label>:197:                                    ; preds = %213, %196
  %198 = load i64, i64* %98, align 8
  %199 = load i64, i64* %96, align 8
  %200 = icmp ult i64 %198, %199
  br i1 %200, label %201, label %216

; <label>:201:                                    ; preds = %197
  %202 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %104, i32 0, i32 0
  %203 = load i64, i64* %98, align 8
  store %"class.std::__1::unique_ptr.12"* %202, %"class.std::__1::unique_ptr.12"** %44, align 8
  store i64 %203, i64* %45, align 8
  %204 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %44, align 8
  %205 = load i64, i64* %45, align 8
  %206 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %204, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %206, %"class.std::__1::__compressed_pair.13"** %43, align 8
  %207 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %43, align 8
  %208 = bitcast %"class.std::__1::__compressed_pair.13"* %207 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %208, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %42, align 8
  %209 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %42, align 8
  %210 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %209, i32 0, i32 0
  %211 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %210, align 8
  %212 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %211, i64 %205
  store %"struct.std::__1::__hash_node_base.15"* null, %"struct.std::__1::__hash_node_base.15"** %212, align 8
  br label %213

; <label>:213:                                    ; preds = %201
  %214 = load i64, i64* %98, align 8
  %215 = add i64 %214, 1
  store i64 %215, i64* %98, align 8
  br label %197

; <label>:216:                                    ; preds = %197
  %217 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %104, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.21"* %217, %"class.std::__1::__compressed_pair.21"** %47, align 8
  %218 = load %"class.std::__1::__compressed_pair.21"*, %"class.std::__1::__compressed_pair.21"** %47, align 8
  %219 = bitcast %"class.std::__1::__compressed_pair.21"* %218 to %"class.std::__1::__libcpp_compressed_pair_imp.22"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.22"* %219, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %46, align 8
  %220 = load %"class.std::__1::__libcpp_compressed_pair_imp.22"*, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %46, align 8
  %221 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.22", %"class.std::__1::__libcpp_compressed_pair_imp.22"* %220, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"* %221, %"struct.std::__1::__hash_node_base.15"** %50, align 8
  %222 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %50, align 8
  store %"struct.std::__1::__hash_node_base.15"* %222, %"struct.std::__1::__hash_node_base.15"** %49, align 8
  %223 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %49, align 8
  store %"struct.std::__1::__hash_node_base.15"* %223, %"struct.std::__1::__hash_node_base.15"** %48, align 8
  %224 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %48, align 8
  %225 = bitcast %"struct.std::__1::__hash_node_base.15"* %224 to i8*
  store %"struct.std::__1::__hash_node_base.15"* %224, %"struct.std::__1::__hash_node_base.15"** %99, align 8
  %226 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %99, align 8
  %227 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %226, i32 0, i32 0
  %228 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %227, align 8
  store %"struct.std::__1::__hash_node_base.15"* %228, %"struct.std::__1::__hash_node_base.15"** %100, align 8
  %229 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %100, align 8
  %230 = icmp ne %"struct.std::__1::__hash_node_base.15"* %229, null
  br i1 %230, label %231, label %441

; <label>:231:                                    ; preds = %216
  %232 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %100, align 8
  store %"struct.std::__1::__hash_node_base.15"* %232, %"struct.std::__1::__hash_node_base.15"** %51, align 8
  %233 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %51, align 8
  %234 = bitcast %"struct.std::__1::__hash_node_base.15"* %233 to %"struct.std::__1::__hash_node.123"*
  %235 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %234, i32 0, i32 1
  %236 = load i64, i64* %235, align 8
  %237 = load i64, i64* %96, align 8
  store i64 %236, i64* %52, align 8
  store i64 %237, i64* %53, align 8
  %238 = load i64, i64* %53, align 8
  %239 = load i64, i64* %53, align 8
  %240 = sub i64 %239, 1
  %241 = and i64 %238, %240
  %242 = icmp ne i64 %241, 0
  br i1 %242, label %248, label %243

; <label>:243:                                    ; preds = %231
  %244 = load i64, i64* %52, align 8
  %245 = load i64, i64* %53, align 8
  %246 = sub i64 %245, 1
  %247 = and i64 %244, %246
  br label %260

; <label>:248:                                    ; preds = %231
  %249 = load i64, i64* %52, align 8
  %250 = load i64, i64* %53, align 8
  %251 = icmp ult i64 %249, %250
  br i1 %251, label %252, label %254

; <label>:252:                                    ; preds = %248
  %253 = load i64, i64* %52, align 8
  br label %258

; <label>:254:                                    ; preds = %248
  %255 = load i64, i64* %52, align 8
  %256 = load i64, i64* %53, align 8
  %257 = urem i64 %255, %256
  br label %258

; <label>:258:                                    ; preds = %254, %252
  %259 = phi i64 [ %253, %252 ], [ %257, %254 ]
  br label %260

; <label>:260:                                    ; preds = %243, %258
  %261 = phi i64 [ %247, %243 ], [ %259, %258 ]
  store i64 %261, i64* %101, align 8
  %262 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %99, align 8
  %263 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %104, i32 0, i32 0
  %264 = load i64, i64* %101, align 8
  store %"class.std::__1::unique_ptr.12"* %263, %"class.std::__1::unique_ptr.12"** %56, align 8
  store i64 %264, i64* %57, align 8
  %265 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %56, align 8
  %266 = load i64, i64* %57, align 8
  %267 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %265, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %267, %"class.std::__1::__compressed_pair.13"** %55, align 8
  %268 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %55, align 8
  %269 = bitcast %"class.std::__1::__compressed_pair.13"* %268 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %269, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %54, align 8
  %270 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %54, align 8
  %271 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %270, i32 0, i32 0
  %272 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %271, align 8
  %273 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %272, i64 %266
  store %"struct.std::__1::__hash_node_base.15"* %262, %"struct.std::__1::__hash_node_base.15"** %273, align 8
  %274 = load i64, i64* %101, align 8
  store i64 %274, i64* %102, align 8
  %275 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %100, align 8
  store %"struct.std::__1::__hash_node_base.15"* %275, %"struct.std::__1::__hash_node_base.15"** %99, align 8
  %276 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %100, align 8
  %277 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %276, i32 0, i32 0
  %278 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %277, align 8
  store %"struct.std::__1::__hash_node_base.15"* %278, %"struct.std::__1::__hash_node_base.15"** %100, align 8
  br label %279

; <label>:279:                                    ; preds = %436, %260
  %280 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %100, align 8
  %281 = icmp ne %"struct.std::__1::__hash_node_base.15"* %280, null
  br i1 %281, label %282, label %440

; <label>:282:                                    ; preds = %279
  %283 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %100, align 8
  store %"struct.std::__1::__hash_node_base.15"* %283, %"struct.std::__1::__hash_node_base.15"** %58, align 8
  %284 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %58, align 8
  %285 = bitcast %"struct.std::__1::__hash_node_base.15"* %284 to %"struct.std::__1::__hash_node.123"*
  %286 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %285, i32 0, i32 1
  %287 = load i64, i64* %286, align 8
  %288 = load i64, i64* %96, align 8
  store i64 %287, i64* %59, align 8
  store i64 %288, i64* %60, align 8
  %289 = load i64, i64* %60, align 8
  %290 = load i64, i64* %60, align 8
  %291 = sub i64 %290, 1
  %292 = and i64 %289, %291
  %293 = icmp ne i64 %292, 0
  br i1 %293, label %299, label %294

; <label>:294:                                    ; preds = %282
  %295 = load i64, i64* %59, align 8
  %296 = load i64, i64* %60, align 8
  %297 = sub i64 %296, 1
  %298 = and i64 %295, %297
  br label %311

; <label>:299:                                    ; preds = %282
  %300 = load i64, i64* %59, align 8
  %301 = load i64, i64* %60, align 8
  %302 = icmp ult i64 %300, %301
  br i1 %302, label %303, label %305

; <label>:303:                                    ; preds = %299
  %304 = load i64, i64* %59, align 8
  br label %309

; <label>:305:                                    ; preds = %299
  %306 = load i64, i64* %59, align 8
  %307 = load i64, i64* %60, align 8
  %308 = urem i64 %306, %307
  br label %309

; <label>:309:                                    ; preds = %305, %303
  %310 = phi i64 [ %304, %303 ], [ %308, %305 ]
  br label %311

; <label>:311:                                    ; preds = %294, %309
  %312 = phi i64 [ %298, %294 ], [ %310, %309 ]
  store i64 %312, i64* %101, align 8
  %313 = load i64, i64* %101, align 8
  %314 = load i64, i64* %102, align 8
  %315 = icmp eq i64 %313, %314
  br i1 %315, label %316, label %318

; <label>:316:                                    ; preds = %311
  %317 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %100, align 8
  store %"struct.std::__1::__hash_node_base.15"* %317, %"struct.std::__1::__hash_node_base.15"** %99, align 8
  br label %435

; <label>:318:                                    ; preds = %311
  %319 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %104, i32 0, i32 0
  %320 = load i64, i64* %101, align 8
  store %"class.std::__1::unique_ptr.12"* %319, %"class.std::__1::unique_ptr.12"** %63, align 8
  store i64 %320, i64* %64, align 8
  %321 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %63, align 8
  %322 = load i64, i64* %64, align 8
  %323 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %321, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %323, %"class.std::__1::__compressed_pair.13"** %62, align 8
  %324 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %62, align 8
  %325 = bitcast %"class.std::__1::__compressed_pair.13"* %324 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %325, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %61, align 8
  %326 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %61, align 8
  %327 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %326, i32 0, i32 0
  %328 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %327, align 8
  %329 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %328, i64 %322
  %330 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %329, align 8
  %331 = icmp eq %"struct.std::__1::__hash_node_base.15"* %330, null
  br i1 %331, label %332, label %347

; <label>:332:                                    ; preds = %318
  %333 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %99, align 8
  %334 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %104, i32 0, i32 0
  %335 = load i64, i64* %101, align 8
  store %"class.std::__1::unique_ptr.12"* %334, %"class.std::__1::unique_ptr.12"** %67, align 8
  store i64 %335, i64* %68, align 8
  %336 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %67, align 8
  %337 = load i64, i64* %68, align 8
  %338 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %336, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %338, %"class.std::__1::__compressed_pair.13"** %66, align 8
  %339 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %66, align 8
  %340 = bitcast %"class.std::__1::__compressed_pair.13"* %339 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %340, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %65, align 8
  %341 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %65, align 8
  %342 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %341, i32 0, i32 0
  %343 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %342, align 8
  %344 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %343, i64 %337
  store %"struct.std::__1::__hash_node_base.15"* %333, %"struct.std::__1::__hash_node_base.15"** %344, align 8
  %345 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %100, align 8
  store %"struct.std::__1::__hash_node_base.15"* %345, %"struct.std::__1::__hash_node_base.15"** %99, align 8
  %346 = load i64, i64* %101, align 8
  store i64 %346, i64* %102, align 8
  br label %434

; <label>:347:                                    ; preds = %318
  %348 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %100, align 8
  store %"struct.std::__1::__hash_node_base.15"* %348, %"struct.std::__1::__hash_node_base.15"** %103, align 8
  br label %349

; <label>:349:                                    ; preds = %394, %347
  %350 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %103, align 8
  %351 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %350, i32 0, i32 0
  %352 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %351, align 8
  %353 = icmp ne %"struct.std::__1::__hash_node_base.15"* %352, null
  br i1 %353, label %354, label %391

; <label>:354:                                    ; preds = %349
  store %"class.std::__1::__hash_table.11"* %104, %"class.std::__1::__hash_table.11"** %71, align 8
  %355 = load %"class.std::__1::__hash_table.11"*, %"class.std::__1::__hash_table.11"** %71, align 8
  %356 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %355, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.30"* %356, %"class.std::__1::__compressed_pair.30"** %70, align 8
  %357 = load %"class.std::__1::__compressed_pair.30"*, %"class.std::__1::__compressed_pair.30"** %70, align 8
  %358 = bitcast %"class.std::__1::__compressed_pair.30"* %357 to %"class.std::__1::__libcpp_compressed_pair_imp.31"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.31"* %358, %"class.std::__1::__libcpp_compressed_pair_imp.31"** %69, align 8
  %359 = load %"class.std::__1::__libcpp_compressed_pair_imp.31"*, %"class.std::__1::__libcpp_compressed_pair_imp.31"** %69, align 8
  %360 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.31"* %359 to %"class.std::__1::__unordered_map_equal.32"*
  %361 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %100, align 8
  store %"struct.std::__1::__hash_node_base.15"* %361, %"struct.std::__1::__hash_node_base.15"** %74, align 8
  %362 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %74, align 8
  store %"struct.std::__1::__hash_node_base.15"* %362, %"struct.std::__1::__hash_node_base.15"** %73, align 8
  %363 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %73, align 8
  store %"struct.std::__1::__hash_node_base.15"* %363, %"struct.std::__1::__hash_node_base.15"** %72, align 8
  %364 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %72, align 8
  %365 = bitcast %"struct.std::__1::__hash_node_base.15"* %364 to i8*
  %366 = bitcast %"struct.std::__1::__hash_node_base.15"* %364 to %"struct.std::__1::__hash_node.123"*
  %367 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %366, i32 0, i32 2
  %368 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %103, align 8
  %369 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %368, i32 0, i32 0
  %370 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %369, align 8
  store %"struct.std::__1::__hash_node_base.15"* %370, %"struct.std::__1::__hash_node_base.15"** %77, align 8
  %371 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %77, align 8
  store %"struct.std::__1::__hash_node_base.15"* %371, %"struct.std::__1::__hash_node_base.15"** %76, align 8
  %372 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %76, align 8
  store %"struct.std::__1::__hash_node_base.15"* %372, %"struct.std::__1::__hash_node_base.15"** %75, align 8
  %373 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %75, align 8
  %374 = bitcast %"struct.std::__1::__hash_node_base.15"* %373 to i8*
  %375 = bitcast %"struct.std::__1::__hash_node_base.15"* %373 to %"struct.std::__1::__hash_node.123"*
  %376 = getelementptr inbounds %"struct.std::__1::__hash_node.123", %"struct.std::__1::__hash_node.123"* %375, i32 0, i32 2
  store %"class.std::__1::__unordered_map_equal.32"* %360, %"class.std::__1::__unordered_map_equal.32"** %81, align 8
  store %"union.std::__1::__hash_value_type.124"* %367, %"union.std::__1::__hash_value_type.124"** %82, align 8
  store %"union.std::__1::__hash_value_type.124"* %376, %"union.std::__1::__hash_value_type.124"** %83, align 8
  %377 = load %"class.std::__1::__unordered_map_equal.32"*, %"class.std::__1::__unordered_map_equal.32"** %81, align 8
  %378 = bitcast %"class.std::__1::__unordered_map_equal.32"* %377 to %"struct.std::__1::equal_to.33"*
  %379 = load %"union.std::__1::__hash_value_type.124"*, %"union.std::__1::__hash_value_type.124"** %82, align 8
  %380 = bitcast %"union.std::__1::__hash_value_type.124"* %379 to %"struct.std::__1::pair.125"*
  %381 = getelementptr inbounds %"struct.std::__1::pair.125", %"struct.std::__1::pair.125"* %380, i32 0, i32 0
  %382 = load %"union.std::__1::__hash_value_type.124"*, %"union.std::__1::__hash_value_type.124"** %83, align 8
  %383 = bitcast %"union.std::__1::__hash_value_type.124"* %382 to %"struct.std::__1::pair.125"*
  %384 = getelementptr inbounds %"struct.std::__1::pair.125", %"struct.std::__1::pair.125"* %383, i32 0, i32 0
  store %"struct.std::__1::equal_to.33"* %378, %"struct.std::__1::equal_to.33"** %78, align 8
  store i32* %381, i32** %79, align 8
  store i32* %384, i32** %80, align 8
  %385 = load %"struct.std::__1::equal_to.33"*, %"struct.std::__1::equal_to.33"** %78, align 8
  %386 = load i32*, i32** %79, align 8
  %387 = load i32, i32* %386, align 4
  %388 = load i32*, i32** %80, align 8
  %389 = load i32, i32* %388, align 4
  %390 = icmp eq i32 %387, %389
  br label %391

; <label>:391:                                    ; preds = %354, %349
  %392 = phi i1 [ false, %349 ], [ %390, %354 ]
  br i1 %392, label %393, label %398

; <label>:393:                                    ; preds = %391
  br label %394

; <label>:394:                                    ; preds = %393
  %395 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %103, align 8
  %396 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %395, i32 0, i32 0
  %397 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %396, align 8
  store %"struct.std::__1::__hash_node_base.15"* %397, %"struct.std::__1::__hash_node_base.15"** %103, align 8
  br label %349

; <label>:398:                                    ; preds = %391
  %399 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %103, align 8
  %400 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %399, i32 0, i32 0
  %401 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %400, align 8
  %402 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %99, align 8
  %403 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %402, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"* %401, %"struct.std::__1::__hash_node_base.15"** %403, align 8
  %404 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %104, i32 0, i32 0
  %405 = load i64, i64* %101, align 8
  store %"class.std::__1::unique_ptr.12"* %404, %"class.std::__1::unique_ptr.12"** %86, align 8
  store i64 %405, i64* %87, align 8
  %406 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %86, align 8
  %407 = load i64, i64* %87, align 8
  %408 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %406, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %408, %"class.std::__1::__compressed_pair.13"** %85, align 8
  %409 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %85, align 8
  %410 = bitcast %"class.std::__1::__compressed_pair.13"* %409 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %410, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %84, align 8
  %411 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %84, align 8
  %412 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %411, i32 0, i32 0
  %413 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %412, align 8
  %414 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %413, i64 %407
  %415 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %414, align 8
  %416 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %415, i32 0, i32 0
  %417 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %416, align 8
  %418 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %103, align 8
  %419 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %418, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"* %417, %"struct.std::__1::__hash_node_base.15"** %419, align 8
  %420 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %100, align 8
  %421 = getelementptr inbounds %"class.std::__1::__hash_table.11", %"class.std::__1::__hash_table.11"* %104, i32 0, i32 0
  %422 = load i64, i64* %101, align 8
  store %"class.std::__1::unique_ptr.12"* %421, %"class.std::__1::unique_ptr.12"** %90, align 8
  store i64 %422, i64* %91, align 8
  %423 = load %"class.std::__1::unique_ptr.12"*, %"class.std::__1::unique_ptr.12"** %90, align 8
  %424 = load i64, i64* %91, align 8
  %425 = getelementptr inbounds %"class.std::__1::unique_ptr.12", %"class.std::__1::unique_ptr.12"* %423, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.13"* %425, %"class.std::__1::__compressed_pair.13"** %89, align 8
  %426 = load %"class.std::__1::__compressed_pair.13"*, %"class.std::__1::__compressed_pair.13"** %89, align 8
  %427 = bitcast %"class.std::__1::__compressed_pair.13"* %426 to %"class.std::__1::__libcpp_compressed_pair_imp.14"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.14"* %427, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %88, align 8
  %428 = load %"class.std::__1::__libcpp_compressed_pair_imp.14"*, %"class.std::__1::__libcpp_compressed_pair_imp.14"** %88, align 8
  %429 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.14", %"class.std::__1::__libcpp_compressed_pair_imp.14"* %428, i32 0, i32 0
  %430 = load %"struct.std::__1::__hash_node_base.15"**, %"struct.std::__1::__hash_node_base.15"*** %429, align 8
  %431 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %430, i64 %424
  %432 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %431, align 8
  %433 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %432, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.15"* %420, %"struct.std::__1::__hash_node_base.15"** %433, align 8
  br label %434

; <label>:434:                                    ; preds = %398, %332
  br label %435

; <label>:435:                                    ; preds = %434, %316
  br label %436

; <label>:436:                                    ; preds = %435
  %437 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %99, align 8
  %438 = getelementptr inbounds %"struct.std::__1::__hash_node_base.15", %"struct.std::__1::__hash_node_base.15"* %437, i32 0, i32 0
  %439 = load %"struct.std::__1::__hash_node_base.15"*, %"struct.std::__1::__hash_node_base.15"** %438, align 8
  store %"struct.std::__1::__hash_node_base.15"* %439, %"struct.std::__1::__hash_node_base.15"** %100, align 8
  br label %279

; <label>:440:                                    ; preds = %279
  br label %441

; <label>:441:                                    ; preds = %440, %216
  br label %442

; <label>:442:                                    ; preds = %441, %179
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE8allocateEm(%"class.std::__1::vector.78"*, i64) #0 align 2 {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %5 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %6 = alloca i64, align 8
  %7 = alloca %"class.std::__1::allocator.82"*, align 8
  %8 = alloca i64, align 8
  %9 = alloca i8*, align 8
  %10 = alloca %"class.std::__1::allocator.82"*, align 8
  %11 = alloca i64, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %14 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %15 = alloca %"class.std::__1::vector.78"*, align 8
  %16 = alloca i64, align 8
  store %"class.std::__1::vector.78"* %0, %"class.std::__1::vector.78"** %15, align 8
  store i64 %1, i64* %16, align 8
  %17 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %15, align 8
  %18 = load i64, i64* %16, align 8
  %19 = call i64 @_ZNKSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE8max_sizeEv(%"class.std::__1::vector.78"* %17) #2
  %20 = icmp ugt i64 %18, %19
  br i1 %20, label %21, label %23

; <label>:21:                                     ; preds = %2
  %22 = bitcast %"class.std::__1::vector.78"* %17 to %"class.std::__1::__vector_base_common"*
  call void @_ZNKSt3__120__vector_base_commonILb1EE20__throw_length_errorEv(%"class.std::__1::__vector_base_common"* %22)
  br label %23

; <label>:23:                                     ; preds = %21, %2
  %24 = bitcast %"class.std::__1::vector.78"* %17 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %24, %"class.std::__1::__vector_base.79"** %14, align 8
  %25 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %14, align 8
  %26 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %25, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %26, %"class.std::__1::__compressed_pair.80"** %13, align 8
  %27 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %13, align 8
  %28 = bitcast %"class.std::__1::__compressed_pair.80"* %27 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %28, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %12, align 8
  %29 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %12, align 8
  %30 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.81"* %29 to %"class.std::__1::allocator.82"*
  %31 = load i64, i64* %16, align 8
  store %"class.std::__1::allocator.82"* %30, %"class.std::__1::allocator.82"** %10, align 8
  store i64 %31, i64* %11, align 8
  %32 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %10, align 8
  %33 = load i64, i64* %11, align 8
  store %"class.std::__1::allocator.82"* %32, %"class.std::__1::allocator.82"** %7, align 8
  store i64 %33, i64* %8, align 8
  store i8* null, i8** %9, align 8
  %34 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %7, align 8
  %35 = load i64, i64* %8, align 8
  %36 = mul i64 %35, 8
  store i64 %36, i64* %6, align 8
  %37 = load i64, i64* %6, align 8
  %38 = call i8* @_Znwm(i64 %37) #17
  %39 = bitcast i8* %38 to %"class.llvm::Type"**
  %40 = bitcast %"class.std::__1::vector.78"* %17 to %"class.std::__1::__vector_base.79"*
  %41 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %40, i32 0, i32 1
  store %"class.llvm::Type"** %39, %"class.llvm::Type"*** %41, align 8
  %42 = bitcast %"class.std::__1::vector.78"* %17 to %"class.std::__1::__vector_base.79"*
  %43 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %42, i32 0, i32 0
  store %"class.llvm::Type"** %39, %"class.llvm::Type"*** %43, align 8
  %44 = bitcast %"class.std::__1::vector.78"* %17 to %"class.std::__1::__vector_base.79"*
  %45 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %44, i32 0, i32 0
  %46 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %45, align 8
  %47 = load i64, i64* %16, align 8
  %48 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %46, i64 %47
  %49 = bitcast %"class.std::__1::vector.78"* %17 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %49, %"class.std::__1::__vector_base.79"** %5, align 8
  %50 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %5, align 8
  %51 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %50, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %51, %"class.std::__1::__compressed_pair.80"** %4, align 8
  %52 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %4, align 8
  %53 = bitcast %"class.std::__1::__compressed_pair.80"* %52 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %53, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %3, align 8
  %54 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %3, align 8
  %55 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %54, i32 0, i32 0
  store %"class.llvm::Type"** %48, %"class.llvm::Type"*** %55, align 8
  call void @_ZNKSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE14__annotate_newEm(%"class.std::__1::vector.78"* %17, i64 0)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE18__construct_at_endIPKS3_EENS_9enable_ifIXsr21__is_forward_iteratorIT_EE5valueEvE4typeESB_SB_m(%"class.std::__1::vector.78"*, %"class.llvm::Type"**, %"class.llvm::Type"**, i64) #0 align 2 {
  %5 = alloca %"class.llvm::Type"**, align 8
  %6 = alloca %"class.std::__1::allocator.82"*, align 8
  %7 = alloca %"class.llvm::Type"**, align 8
  %8 = alloca %"class.llvm::Type"**, align 8
  %9 = alloca %"class.llvm::Type"**, align 8
  %10 = alloca %"struct.std::__1::integral_constant.128", align 1
  %11 = alloca %"class.std::__1::allocator.82"*, align 8
  %12 = alloca %"class.llvm::Type"**, align 8
  %13 = alloca %"class.llvm::Type"**, align 8
  %14 = alloca %"class.llvm::Type"**, align 8
  %15 = alloca %"class.std::__1::allocator.82"*, align 8
  %16 = alloca %"class.llvm::Type"**, align 8
  %17 = alloca %"class.llvm::Type"**, align 8
  %18 = alloca %"struct.std::__1::integral_constant.128", align 1
  %19 = alloca %"struct.std::__1::__has_construct.151", align 1
  %20 = alloca %"class.llvm::Type"**, align 8
  %21 = alloca %"class.std::__1::allocator.82"*, align 8
  %22 = alloca %"class.llvm::Type"**, align 8
  %23 = alloca %"class.llvm::Type"**, align 8
  %24 = alloca %"class.llvm::Type"***, align 8
  %25 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %26 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %27 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %28 = alloca %"class.std::__1::vector.78"*, align 8
  %29 = alloca %"class.llvm::Type"**, align 8
  %30 = alloca %"class.llvm::Type"**, align 8
  %31 = alloca i64, align 8
  %32 = alloca %"class.std::__1::allocator.82"*, align 8
  %33 = alloca %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator", align 1
  store %"class.std::__1::vector.78"* %0, %"class.std::__1::vector.78"** %28, align 8
  store %"class.llvm::Type"** %1, %"class.llvm::Type"*** %29, align 8
  store %"class.llvm::Type"** %2, %"class.llvm::Type"*** %30, align 8
  store i64 %3, i64* %31, align 8
  %34 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %28, align 8
  %35 = bitcast %"class.std::__1::vector.78"* %34 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %35, %"class.std::__1::__vector_base.79"** %27, align 8
  %36 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %27, align 8
  %37 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %36, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %37, %"class.std::__1::__compressed_pair.80"** %26, align 8
  %38 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %26, align 8
  %39 = bitcast %"class.std::__1::__compressed_pair.80"* %38 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %39, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %25, align 8
  %40 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %25, align 8
  %41 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.81"* %40 to %"class.std::__1::allocator.82"*
  store %"class.std::__1::allocator.82"* %41, %"class.std::__1::allocator.82"** %32, align 8
  %42 = load i64, i64* %31, align 8
  call void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE24__RAII_IncreaseAnnotatorC1ERKS6_m(%"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"* %33, %"class.std::__1::vector.78"* dereferenceable(24) %34, i64 %42)
  %43 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %32, align 8
  %44 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %29, align 8
  %45 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %30, align 8
  %46 = bitcast %"class.std::__1::vector.78"* %34 to %"class.std::__1::__vector_base.79"*
  %47 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %46, i32 0, i32 1
  store %"class.std::__1::allocator.82"* %43, %"class.std::__1::allocator.82"** %21, align 8
  store %"class.llvm::Type"** %44, %"class.llvm::Type"*** %22, align 8
  store %"class.llvm::Type"** %45, %"class.llvm::Type"*** %23, align 8
  store %"class.llvm::Type"*** %47, %"class.llvm::Type"**** %24, align 8
  br label %48

; <label>:48:                                     ; preds = %52, %4
  %49 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %22, align 8
  %50 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %23, align 8
  %51 = icmp ne %"class.llvm::Type"** %49, %50
  br i1 %51, label %52, label %78

; <label>:52:                                     ; preds = %48
  %53 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %21, align 8
  %54 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %24, align 8
  %55 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %54, align 8
  store %"class.llvm::Type"** %55, %"class.llvm::Type"*** %20, align 8
  %56 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %20, align 8
  %57 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %22, align 8
  store %"class.std::__1::allocator.82"* %53, %"class.std::__1::allocator.82"** %15, align 8
  store %"class.llvm::Type"** %56, %"class.llvm::Type"*** %16, align 8
  store %"class.llvm::Type"** %57, %"class.llvm::Type"*** %17, align 8
  %58 = bitcast %"struct.std::__1::__has_construct.151"* %19 to %"struct.std::__1::integral_constant.128"*
  %59 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %15, align 8
  %60 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %16, align 8
  %61 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %17, align 8
  store %"class.llvm::Type"** %61, %"class.llvm::Type"*** %14, align 8
  %62 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %14, align 8
  store %"class.std::__1::allocator.82"* %59, %"class.std::__1::allocator.82"** %11, align 8
  store %"class.llvm::Type"** %60, %"class.llvm::Type"*** %12, align 8
  store %"class.llvm::Type"** %62, %"class.llvm::Type"*** %13, align 8
  %63 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %11, align 8
  %64 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %12, align 8
  %65 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %13, align 8
  store %"class.llvm::Type"** %65, %"class.llvm::Type"*** %9, align 8
  %66 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %9, align 8
  store %"class.std::__1::allocator.82"* %63, %"class.std::__1::allocator.82"** %6, align 8
  store %"class.llvm::Type"** %64, %"class.llvm::Type"*** %7, align 8
  store %"class.llvm::Type"** %66, %"class.llvm::Type"*** %8, align 8
  %67 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %6, align 8
  %68 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %7, align 8
  %69 = bitcast %"class.llvm::Type"** %68 to i8*
  %70 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %8, align 8
  store %"class.llvm::Type"** %70, %"class.llvm::Type"*** %5, align 8
  %71 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %5, align 8
  %72 = load %"class.llvm::Type"*, %"class.llvm::Type"** %71, align 8
  store %"class.llvm::Type"* %72, %"class.llvm::Type"** %68, align 8
  %73 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %22, align 8
  %74 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %73, i32 1
  store %"class.llvm::Type"** %74, %"class.llvm::Type"*** %22, align 8
  %75 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %24, align 8
  %76 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %75, align 8
  %77 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %76, i32 1
  store %"class.llvm::Type"** %77, %"class.llvm::Type"*** %75, align 8
  br label %48

; <label>:78:                                     ; preds = %48
  call void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE24__RAII_IncreaseAnnotator6__doneEv(%"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"* %33)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNKSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE8max_sizeEv(%"class.std::__1::vector.78"*) #5 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"struct.std::__1::__less"*, align 8
  %3 = alloca i64*, align 8
  %4 = alloca i64*, align 8
  %5 = alloca %"struct.std::__1::__less", align 1
  %6 = alloca i64*, align 8
  %7 = alloca i64*, align 8
  %8 = alloca i64*, align 8
  %9 = alloca i64*, align 8
  %10 = alloca %"struct.std::__1::__less", align 1
  %11 = alloca %"class.std::__1::allocator.82"*, align 8
  %12 = alloca %"struct.std::__1::integral_constant.128", align 1
  %13 = alloca %"class.std::__1::allocator.82"*, align 8
  %14 = alloca %"class.std::__1::allocator.82"*, align 8
  %15 = alloca %"struct.std::__1::integral_constant.128", align 1
  %16 = alloca %"struct.std::__1::__has_max_size", align 1
  %17 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %18 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %19 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %20 = alloca %"class.std::__1::vector.78"*, align 8
  %21 = alloca i64, align 8
  %22 = alloca i64, align 8
  store %"class.std::__1::vector.78"* %0, %"class.std::__1::vector.78"** %20, align 8
  %23 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %20, align 8
  %24 = bitcast %"class.std::__1::vector.78"* %23 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %24, %"class.std::__1::__vector_base.79"** %19, align 8
  %25 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %19, align 8
  %26 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %25, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %26, %"class.std::__1::__compressed_pair.80"** %18, align 8
  %27 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %18, align 8
  %28 = bitcast %"class.std::__1::__compressed_pair.80"* %27 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %28, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %17, align 8
  %29 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %17, align 8
  %30 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.81"* %29 to %"class.std::__1::allocator.82"*
  store %"class.std::__1::allocator.82"* %30, %"class.std::__1::allocator.82"** %14, align 8
  %31 = bitcast %"struct.std::__1::__has_max_size"* %16 to %"struct.std::__1::integral_constant.128"*
  %32 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %14, align 8
  store %"class.std::__1::allocator.82"* %32, %"class.std::__1::allocator.82"** %13, align 8
  %33 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %13, align 8
  store %"class.std::__1::allocator.82"* %33, %"class.std::__1::allocator.82"** %11, align 8
  %34 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %11, align 8
  store i64 2305843009213693951, i64* %21, align 8
  %35 = udiv i64 -1, 2
  store i64 %35, i64* %22, align 8
  store i64* %21, i64** %8, align 8
  store i64* %22, i64** %9, align 8
  %36 = load i64*, i64** %8, align 8
  %37 = load i64*, i64** %9, align 8
  store i64* %36, i64** %6, align 8
  store i64* %37, i64** %7, align 8
  %38 = load i64*, i64** %7, align 8
  %39 = load i64*, i64** %6, align 8
  store %"struct.std::__1::__less"* %5, %"struct.std::__1::__less"** %2, align 8
  store i64* %38, i64** %3, align 8
  store i64* %39, i64** %4, align 8
  %40 = load %"struct.std::__1::__less"*, %"struct.std::__1::__less"** %2, align 8
  %41 = load i64*, i64** %3, align 8
  %42 = load i64, i64* %41, align 8
  %43 = load i64*, i64** %4, align 8
  %44 = load i64, i64* %43, align 8
  %45 = icmp ult i64 %42, %44
  br i1 %45, label %46, label %48

; <label>:46:                                     ; preds = %1
  %47 = load i64*, i64** %7, align 8
  br label %50

; <label>:48:                                     ; preds = %1
  %49 = load i64*, i64** %6, align 8
  br label %50

; <label>:50:                                     ; preds = %46, %48
  %51 = phi i64* [ %47, %46 ], [ %49, %48 ]
  br label %52

; <label>:52:                                     ; preds = %50
  %53 = load i64, i64* %51, align 8
  ret i64 %53
                                                  ; No predecessors!
  %55 = landingpad { i8*, i32 }
          catch i8* null
  %56 = extractvalue { i8*, i32 } %55, 0
  call void @__clang_call_terminate(i8* %56) #15
  unreachable
}

declare void @_ZNKSt3__120__vector_base_commonILb1EE20__throw_length_errorEv(%"class.std::__1::__vector_base_common"*) #3

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNKSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE14__annotate_newEm(%"class.std::__1::vector.78"*, i64) #0 align 2 {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %5 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %6 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %7 = alloca %"class.std::__1::vector.78"*, align 8
  %8 = alloca %"class.llvm::Type"**, align 8
  %9 = alloca %"class.std::__1::vector.78"*, align 8
  %10 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %11 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %12 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %13 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %14 = alloca %"class.std::__1::vector.78"*, align 8
  %15 = alloca %"class.llvm::Type"**, align 8
  %16 = alloca %"class.std::__1::vector.78"*, align 8
  %17 = alloca %"class.llvm::Type"**, align 8
  %18 = alloca %"class.std::__1::vector.78"*, align 8
  %19 = alloca %"class.llvm::Type"**, align 8
  %20 = alloca %"class.std::__1::vector.78"*, align 8
  %21 = alloca %"class.std::__1::vector.78"*, align 8
  %22 = alloca i64, align 8
  store %"class.std::__1::vector.78"* %0, %"class.std::__1::vector.78"** %21, align 8
  store i64 %1, i64* %22, align 8
  %23 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %21, align 8
  store %"class.std::__1::vector.78"* %23, %"class.std::__1::vector.78"** %20, align 8
  %24 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %20, align 8
  %25 = bitcast %"class.std::__1::vector.78"* %24 to %"class.std::__1::__vector_base.79"*
  %26 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %25, i32 0, i32 0
  %27 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %26, align 8
  store %"class.llvm::Type"** %27, %"class.llvm::Type"*** %19, align 8
  %28 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %19, align 8
  %29 = bitcast %"class.llvm::Type"** %28 to i8*
  store %"class.std::__1::vector.78"* %23, %"class.std::__1::vector.78"** %18, align 8
  %30 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %18, align 8
  %31 = bitcast %"class.std::__1::vector.78"* %30 to %"class.std::__1::__vector_base.79"*
  %32 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %31, i32 0, i32 0
  %33 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %32, align 8
  store %"class.llvm::Type"** %33, %"class.llvm::Type"*** %17, align 8
  %34 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %17, align 8
  store %"class.std::__1::vector.78"* %23, %"class.std::__1::vector.78"** %7, align 8
  %35 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %7, align 8
  %36 = bitcast %"class.std::__1::vector.78"* %35 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %36, %"class.std::__1::__vector_base.79"** %6, align 8
  %37 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %6, align 8
  store %"class.std::__1::__vector_base.79"* %37, %"class.std::__1::__vector_base.79"** %5, align 8
  %38 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %5, align 8
  %39 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %38, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %39, %"class.std::__1::__compressed_pair.80"** %4, align 8
  %40 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %4, align 8
  %41 = bitcast %"class.std::__1::__compressed_pair.80"* %40 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %41, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %3, align 8
  %42 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %3, align 8
  %43 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %42, i32 0, i32 0
  %44 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %43, align 8
  %45 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %37, i32 0, i32 0
  %46 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %45, align 8
  %47 = ptrtoint %"class.llvm::Type"** %44 to i64
  %48 = ptrtoint %"class.llvm::Type"** %46 to i64
  %49 = sub i64 %47, %48
  %50 = sdiv exact i64 %49, 8
  %51 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %34, i64 %50
  %52 = bitcast %"class.llvm::Type"** %51 to i8*
  store %"class.std::__1::vector.78"* %23, %"class.std::__1::vector.78"** %9, align 8
  %53 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %9, align 8
  %54 = bitcast %"class.std::__1::vector.78"* %53 to %"class.std::__1::__vector_base.79"*
  %55 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %54, i32 0, i32 0
  %56 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %55, align 8
  store %"class.llvm::Type"** %56, %"class.llvm::Type"*** %8, align 8
  %57 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %8, align 8
  store %"class.std::__1::vector.78"* %23, %"class.std::__1::vector.78"** %14, align 8
  %58 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %14, align 8
  %59 = bitcast %"class.std::__1::vector.78"* %58 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %59, %"class.std::__1::__vector_base.79"** %13, align 8
  %60 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %13, align 8
  store %"class.std::__1::__vector_base.79"* %60, %"class.std::__1::__vector_base.79"** %12, align 8
  %61 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %12, align 8
  %62 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %61, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %62, %"class.std::__1::__compressed_pair.80"** %11, align 8
  %63 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %11, align 8
  %64 = bitcast %"class.std::__1::__compressed_pair.80"* %63 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %64, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %10, align 8
  %65 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %10, align 8
  %66 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %65, i32 0, i32 0
  %67 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %60, i32 0, i32 0
  %69 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %68, align 8
  %70 = ptrtoint %"class.llvm::Type"** %67 to i64
  %71 = ptrtoint %"class.llvm::Type"** %69 to i64
  %72 = sub i64 %70, %71
  %73 = sdiv exact i64 %72, 8
  %74 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %57, i64 %73
  %75 = bitcast %"class.llvm::Type"** %74 to i8*
  store %"class.std::__1::vector.78"* %23, %"class.std::__1::vector.78"** %16, align 8
  %76 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %16, align 8
  %77 = bitcast %"class.std::__1::vector.78"* %76 to %"class.std::__1::__vector_base.79"*
  %78 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %77, i32 0, i32 0
  %79 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %78, align 8
  store %"class.llvm::Type"** %79, %"class.llvm::Type"*** %15, align 8
  %80 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %15, align 8
  %81 = load i64, i64* %22, align 8
  %82 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %80, i64 %81
  %83 = bitcast %"class.llvm::Type"** %82 to i8*
  call void @_ZNKSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE31__annotate_contiguous_containerEPKvS8_S8_S8_(%"class.std::__1::vector.78"* %23, i8* %29, i8* %52, i8* %75, i8* %83)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNKSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE31__annotate_contiguous_containerEPKvS8_S8_S8_(%"class.std::__1::vector.78"*, i8*, i8*, i8*, i8*) #5 align 2 {
  %6 = alloca %"class.std::__1::vector.78"*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i8*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca i8*, align 8
  store %"class.std::__1::vector.78"* %0, %"class.std::__1::vector.78"** %6, align 8
  store i8* %1, i8** %7, align 8
  store i8* %2, i8** %8, align 8
  store i8* %3, i8** %9, align 8
  store i8* %4, i8** %10, align 8
  %11 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %6, align 8
  ret void
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE24__RAII_IncreaseAnnotatorC1ERKS6_m(%"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"*, %"class.std::__1::vector.78"* dereferenceable(24), i64) unnamed_addr #10 align 2 {
  %4 = alloca %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"*, align 8
  %5 = alloca %"class.std::__1::vector.78"*, align 8
  %6 = alloca i64, align 8
  store %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"* %0, %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"** %4, align 8
  store %"class.std::__1::vector.78"* %1, %"class.std::__1::vector.78"** %5, align 8
  store i64 %2, i64* %6, align 8
  %7 = load %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"*, %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"** %4, align 8
  %8 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %5, align 8
  %9 = load i64, i64* %6, align 8
  call void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE24__RAII_IncreaseAnnotatorC2ERKS6_m(%"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"* %7, %"class.std::__1::vector.78"* dereferenceable(24) %8, i64 %9)
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE24__RAII_IncreaseAnnotator6__doneEv(%"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"*) #1 align 2 {
  %2 = alloca %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"*, align 8
  store %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"* %0, %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"** %2, align 8
  %3 = load %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"*, %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"** %2, align 8
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE24__RAII_IncreaseAnnotatorC2ERKS6_m(%"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"*, %"class.std::__1::vector.78"* dereferenceable(24), i64) unnamed_addr #1 align 2 {
  %4 = alloca %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"*, align 8
  %5 = alloca %"class.std::__1::vector.78"*, align 8
  %6 = alloca i64, align 8
  store %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"* %0, %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"** %4, align 8
  store %"class.std::__1::vector.78"* %1, %"class.std::__1::vector.78"** %5, align 8
  store i64 %2, i64* %6, align 8
  %7 = load %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"*, %"struct.std::__1::vector<llvm::Type *, std::__1::allocator<llvm::Type *> >::__RAII_IncreaseAnnotator"** %4, align 8
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__114__split_bufferIPN4llvm4TypeERNS_9allocatorIS3_EEEC1EmmS6_(%"struct.std::__1::__split_buffer"*, i64, i64, %"class.std::__1::allocator.82"* dereferenceable(1)) unnamed_addr #0 align 2 {
  %5 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  %8 = alloca %"class.std::__1::allocator.82"*, align 8
  store %"struct.std::__1::__split_buffer"* %0, %"struct.std::__1::__split_buffer"** %5, align 8
  store i64 %1, i64* %6, align 8
  store i64 %2, i64* %7, align 8
  store %"class.std::__1::allocator.82"* %3, %"class.std::__1::allocator.82"** %8, align 8
  %9 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %5, align 8
  %10 = load i64, i64* %6, align 8
  %11 = load i64, i64* %7, align 8
  %12 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %8, align 8
  call void @_ZNSt3__114__split_bufferIPN4llvm4TypeERNS_9allocatorIS3_EEEC2EmmS6_(%"struct.std::__1::__split_buffer"* %9, i64 %10, i64 %11, %"class.std::__1::allocator.82"* dereferenceable(1) %12)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE26__swap_out_circular_bufferERNS_14__split_bufferIS3_RS5_EE(%"class.std::__1::vector.78"*, %"struct.std::__1::__split_buffer"* dereferenceable(40)) #0 align 2 {
  %3 = alloca %"class.llvm::Type"***, align 8
  %4 = alloca %"class.llvm::Type"***, align 8
  %5 = alloca %"class.llvm::Type"***, align 8
  %6 = alloca %"class.llvm::Type"***, align 8
  %7 = alloca %"class.llvm::Type"***, align 8
  %8 = alloca %"class.llvm::Type"**, align 8
  %9 = alloca %"class.llvm::Type"***, align 8
  %10 = alloca %"class.llvm::Type"***, align 8
  %11 = alloca %"class.llvm::Type"***, align 8
  %12 = alloca %"class.llvm::Type"***, align 8
  %13 = alloca %"class.llvm::Type"***, align 8
  %14 = alloca %"class.llvm::Type"**, align 8
  %15 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %16 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %17 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %18 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.153"*, align 8
  %19 = alloca %"class.std::__1::__compressed_pair.152"*, align 8
  %20 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %21 = alloca %"class.llvm::Type"***, align 8
  %22 = alloca %"class.llvm::Type"***, align 8
  %23 = alloca %"class.llvm::Type"***, align 8
  %24 = alloca %"class.llvm::Type"***, align 8
  %25 = alloca %"class.llvm::Type"***, align 8
  %26 = alloca %"class.llvm::Type"**, align 8
  %27 = alloca %"class.std::__1::vector.78"*, align 8
  %28 = alloca %"class.std::__1::allocator.82"*, align 8
  %29 = alloca %"class.llvm::Type"**, align 8
  %30 = alloca %"class.llvm::Type"**, align 8
  %31 = alloca %"class.llvm::Type"***, align 8
  %32 = alloca i64, align 8
  %33 = alloca %"class.std::__1::vector.78"*, align 8
  %34 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %35 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %36 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %37 = alloca %"class.std::__1::vector.78"*, align 8
  %38 = alloca %"struct.std::__1::__split_buffer"*, align 8
  store %"class.std::__1::vector.78"* %0, %"class.std::__1::vector.78"** %37, align 8
  store %"struct.std::__1::__split_buffer"* %1, %"struct.std::__1::__split_buffer"** %38, align 8
  %39 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %37, align 8
  call void @_ZNKSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE17__annotate_deleteEv(%"class.std::__1::vector.78"* %39)
  %40 = bitcast %"class.std::__1::vector.78"* %39 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %40, %"class.std::__1::__vector_base.79"** %36, align 8
  %41 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %36, align 8
  %42 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %41, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %42, %"class.std::__1::__compressed_pair.80"** %35, align 8
  %43 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %35, align 8
  %44 = bitcast %"class.std::__1::__compressed_pair.80"* %43 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %44, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %34, align 8
  %45 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %34, align 8
  %46 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.81"* %45 to %"class.std::__1::allocator.82"*
  %47 = bitcast %"class.std::__1::vector.78"* %39 to %"class.std::__1::__vector_base.79"*
  %48 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %47, i32 0, i32 0
  %49 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %48, align 8
  %50 = bitcast %"class.std::__1::vector.78"* %39 to %"class.std::__1::__vector_base.79"*
  %51 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %50, i32 0, i32 1
  %52 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %51, align 8
  %53 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %38, align 8
  %54 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %53, i32 0, i32 1
  store %"class.std::__1::allocator.82"* %46, %"class.std::__1::allocator.82"** %28, align 8
  store %"class.llvm::Type"** %49, %"class.llvm::Type"*** %29, align 8
  store %"class.llvm::Type"** %52, %"class.llvm::Type"*** %30, align 8
  store %"class.llvm::Type"*** %54, %"class.llvm::Type"**** %31, align 8
  %55 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %30, align 8
  %56 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %29, align 8
  %57 = ptrtoint %"class.llvm::Type"** %55 to i64
  %58 = ptrtoint %"class.llvm::Type"** %56 to i64
  %59 = sub i64 %57, %58
  %60 = sdiv exact i64 %59, 8
  store i64 %60, i64* %32, align 8
  %61 = load i64, i64* %32, align 8
  %62 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %31, align 8
  %63 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %62, align 8
  %64 = sub i64 0, %61
  %65 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %63, i64 %64
  store %"class.llvm::Type"** %65, %"class.llvm::Type"*** %62, align 8
  %66 = load i64, i64* %32, align 8
  %67 = icmp sgt i64 %66, 0
  br i1 %67, label %68, label %76

; <label>:68:                                     ; preds = %2
  %69 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %31, align 8
  %70 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %69, align 8
  %71 = bitcast %"class.llvm::Type"** %70 to i8*
  %72 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %29, align 8
  %73 = bitcast %"class.llvm::Type"** %72 to i8*
  %74 = load i64, i64* %32, align 8
  %75 = mul i64 %74, 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %71, i8* %73, i64 %75, i32 8, i1 false) #2
  br label %76

; <label>:76:                                     ; preds = %2, %68
  %77 = bitcast %"class.std::__1::vector.78"* %39 to %"class.std::__1::__vector_base.79"*
  %78 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %77, i32 0, i32 0
  %79 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %38, align 8
  %80 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %79, i32 0, i32 1
  store %"class.llvm::Type"*** %78, %"class.llvm::Type"**** %6, align 8
  store %"class.llvm::Type"*** %80, %"class.llvm::Type"**** %7, align 8
  %81 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %6, align 8
  store %"class.llvm::Type"*** %81, %"class.llvm::Type"**** %5, align 8
  %82 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %5, align 8
  %83 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %82, align 8
  store %"class.llvm::Type"** %83, %"class.llvm::Type"*** %8, align 8
  %84 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %7, align 8
  store %"class.llvm::Type"*** %84, %"class.llvm::Type"**** %3, align 8
  %85 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %3, align 8
  %86 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %85, align 8
  %87 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %6, align 8
  store %"class.llvm::Type"** %86, %"class.llvm::Type"*** %87, align 8
  store %"class.llvm::Type"*** %8, %"class.llvm::Type"**** %4, align 8
  %88 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %4, align 8
  %89 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %88, align 8
  %90 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %7, align 8
  store %"class.llvm::Type"** %89, %"class.llvm::Type"*** %90, align 8
  %91 = bitcast %"class.std::__1::vector.78"* %39 to %"class.std::__1::__vector_base.79"*
  %92 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %91, i32 0, i32 1
  %93 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %38, align 8
  %94 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %93, i32 0, i32 2
  store %"class.llvm::Type"*** %92, %"class.llvm::Type"**** %12, align 8
  store %"class.llvm::Type"*** %94, %"class.llvm::Type"**** %13, align 8
  %95 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %12, align 8
  store %"class.llvm::Type"*** %95, %"class.llvm::Type"**** %11, align 8
  %96 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %11, align 8
  %97 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %96, align 8
  store %"class.llvm::Type"** %97, %"class.llvm::Type"*** %14, align 8
  %98 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %13, align 8
  store %"class.llvm::Type"*** %98, %"class.llvm::Type"**** %9, align 8
  %99 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %9, align 8
  %100 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %99, align 8
  %101 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %12, align 8
  store %"class.llvm::Type"** %100, %"class.llvm::Type"*** %101, align 8
  store %"class.llvm::Type"*** %14, %"class.llvm::Type"**** %10, align 8
  %102 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %10, align 8
  %103 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %102, align 8
  %104 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %13, align 8
  store %"class.llvm::Type"** %103, %"class.llvm::Type"*** %104, align 8
  %105 = bitcast %"class.std::__1::vector.78"* %39 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %105, %"class.std::__1::__vector_base.79"** %17, align 8
  %106 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %17, align 8
  %107 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %106, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %107, %"class.std::__1::__compressed_pair.80"** %16, align 8
  %108 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %16, align 8
  %109 = bitcast %"class.std::__1::__compressed_pair.80"* %108 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %109, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %15, align 8
  %110 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %15, align 8
  %111 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %110, i32 0, i32 0
  %112 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %38, align 8
  store %"struct.std::__1::__split_buffer"* %112, %"struct.std::__1::__split_buffer"** %20, align 8
  %113 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %20, align 8
  %114 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %113, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.152"* %114, %"class.std::__1::__compressed_pair.152"** %19, align 8
  %115 = load %"class.std::__1::__compressed_pair.152"*, %"class.std::__1::__compressed_pair.152"** %19, align 8
  %116 = bitcast %"class.std::__1::__compressed_pair.152"* %115 to %"class.std::__1::__libcpp_compressed_pair_imp.153"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.153"* %116, %"class.std::__1::__libcpp_compressed_pair_imp.153"** %18, align 8
  %117 = load %"class.std::__1::__libcpp_compressed_pair_imp.153"*, %"class.std::__1::__libcpp_compressed_pair_imp.153"** %18, align 8
  %118 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.153", %"class.std::__1::__libcpp_compressed_pair_imp.153"* %117, i32 0, i32 0
  store %"class.llvm::Type"*** %111, %"class.llvm::Type"**** %24, align 8
  store %"class.llvm::Type"*** %118, %"class.llvm::Type"**** %25, align 8
  %119 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %24, align 8
  store %"class.llvm::Type"*** %119, %"class.llvm::Type"**** %23, align 8
  %120 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %23, align 8
  %121 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %120, align 8
  store %"class.llvm::Type"** %121, %"class.llvm::Type"*** %26, align 8
  %122 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %25, align 8
  store %"class.llvm::Type"*** %122, %"class.llvm::Type"**** %21, align 8
  %123 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %21, align 8
  %124 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %123, align 8
  %125 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %24, align 8
  store %"class.llvm::Type"** %124, %"class.llvm::Type"*** %125, align 8
  store %"class.llvm::Type"*** %26, %"class.llvm::Type"**** %22, align 8
  %126 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %22, align 8
  %127 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %126, align 8
  %128 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %25, align 8
  store %"class.llvm::Type"** %127, %"class.llvm::Type"*** %128, align 8
  %129 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %38, align 8
  %130 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %129, i32 0, i32 1
  %131 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %130, align 8
  %132 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %38, align 8
  %133 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %132, i32 0, i32 0
  store %"class.llvm::Type"** %131, %"class.llvm::Type"*** %133, align 8
  store %"class.std::__1::vector.78"* %39, %"class.std::__1::vector.78"** %27, align 8
  %134 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %27, align 8
  %135 = bitcast %"class.std::__1::vector.78"* %134 to %"class.std::__1::__vector_base.79"*
  %136 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %135, i32 0, i32 1
  %137 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %136, align 8
  %138 = bitcast %"class.std::__1::vector.78"* %134 to %"class.std::__1::__vector_base.79"*
  %139 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %138, i32 0, i32 0
  %140 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %139, align 8
  %141 = ptrtoint %"class.llvm::Type"** %137 to i64
  %142 = ptrtoint %"class.llvm::Type"** %140 to i64
  %143 = sub i64 %141, %142
  %144 = sdiv exact i64 %143, 8
  call void @_ZNKSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE14__annotate_newEm(%"class.std::__1::vector.78"* %39, i64 %144)
  store %"class.std::__1::vector.78"* %39, %"class.std::__1::vector.78"** %33, align 8
  %145 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %33, align 8
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__114__split_bufferIPN4llvm4TypeERNS_9allocatorIS3_EEED1Ev(%"struct.std::__1::__split_buffer"*) unnamed_addr #5 align 2 {
  %2 = alloca %"struct.std::__1::__split_buffer"*, align 8
  store %"struct.std::__1::__split_buffer"* %0, %"struct.std::__1::__split_buffer"** %2, align 8
  %3 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %2, align 8
  call void @_ZNSt3__114__split_bufferIPN4llvm4TypeERNS_9allocatorIS3_EEED2Ev(%"struct.std::__1::__split_buffer"* %3) #2
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__114__split_bufferIPN4llvm4TypeERNS_9allocatorIS3_EEEC2EmmS6_(%"struct.std::__1::__split_buffer"*, i64, i64, %"class.std::__1::allocator.82"* dereferenceable(1)) unnamed_addr #0 align 2 {
  %5 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.153"*, align 8
  %6 = alloca %"class.std::__1::__compressed_pair.152"*, align 8
  %7 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %8 = alloca i64, align 8
  %9 = alloca %"class.std::__1::allocator.82"*, align 8
  %10 = alloca i64, align 8
  %11 = alloca i8*, align 8
  %12 = alloca %"class.std::__1::allocator.82"*, align 8
  %13 = alloca i64, align 8
  %14 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.153"*, align 8
  %15 = alloca %"class.std::__1::__compressed_pair.152"*, align 8
  %16 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %17 = alloca %"class.std::__1::allocator.82"*, align 8
  %18 = alloca %"class.std::__1::allocator.82"*, align 8
  %19 = alloca %"class.llvm::Type"***, align 8
  %20 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.153"*, align 8
  %21 = alloca %"class.llvm::Type"**, align 8
  %22 = alloca %"class.std::__1::allocator.82"*, align 8
  %23 = alloca %"class.llvm::Type"***, align 8
  %24 = alloca %"class.std::__1::__compressed_pair.152"*, align 8
  %25 = alloca %"class.llvm::Type"**, align 8
  %26 = alloca %"class.std::__1::allocator.82"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.152"*, align 8
  %28 = alloca %"class.llvm::Type"**, align 8
  %29 = alloca %"class.std::__1::allocator.82"*, align 8
  %30 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %31 = alloca i64, align 8
  %32 = alloca i64, align 8
  %33 = alloca %"class.std::__1::allocator.82"*, align 8
  store %"struct.std::__1::__split_buffer"* %0, %"struct.std::__1::__split_buffer"** %30, align 8
  store i64 %1, i64* %31, align 8
  store i64 %2, i64* %32, align 8
  store %"class.std::__1::allocator.82"* %3, %"class.std::__1::allocator.82"** %33, align 8
  %34 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %30, align 8
  %35 = bitcast %"struct.std::__1::__split_buffer"* %34 to %"class.std::__1::__split_buffer_common"*
  %36 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %34, i32 0, i32 3
  %37 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %33, align 8
  store %"class.std::__1::__compressed_pair.152"* %36, %"class.std::__1::__compressed_pair.152"** %27, align 8
  store %"class.llvm::Type"** null, %"class.llvm::Type"*** %28, align 8
  store %"class.std::__1::allocator.82"* %37, %"class.std::__1::allocator.82"** %29, align 8
  %38 = load %"class.std::__1::__compressed_pair.152"*, %"class.std::__1::__compressed_pair.152"** %27, align 8
  %39 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %28, align 8
  %40 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %29, align 8
  store %"class.std::__1::__compressed_pair.152"* %38, %"class.std::__1::__compressed_pair.152"** %24, align 8
  store %"class.llvm::Type"** %39, %"class.llvm::Type"*** %25, align 8
  store %"class.std::__1::allocator.82"* %40, %"class.std::__1::allocator.82"** %26, align 8
  %41 = load %"class.std::__1::__compressed_pair.152"*, %"class.std::__1::__compressed_pair.152"** %24, align 8
  %42 = bitcast %"class.std::__1::__compressed_pair.152"* %41 to %"class.std::__1::__libcpp_compressed_pair_imp.153"*
  store %"class.llvm::Type"*** %25, %"class.llvm::Type"**** %23, align 8
  %43 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %23, align 8
  %44 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %43, align 8
  %45 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %26, align 8
  store %"class.std::__1::allocator.82"* %45, %"class.std::__1::allocator.82"** %17, align 8
  %46 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %17, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.153"* %42, %"class.std::__1::__libcpp_compressed_pair_imp.153"** %20, align 8
  store %"class.llvm::Type"** %44, %"class.llvm::Type"*** %21, align 8
  store %"class.std::__1::allocator.82"* %46, %"class.std::__1::allocator.82"** %22, align 8
  %47 = load %"class.std::__1::__libcpp_compressed_pair_imp.153"*, %"class.std::__1::__libcpp_compressed_pair_imp.153"** %20, align 8
  %48 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.153", %"class.std::__1::__libcpp_compressed_pair_imp.153"* %47, i32 0, i32 0
  store %"class.llvm::Type"*** %21, %"class.llvm::Type"**** %19, align 8
  %49 = load %"class.llvm::Type"***, %"class.llvm::Type"**** %19, align 8
  %50 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %49, align 8
  store %"class.llvm::Type"** %50, %"class.llvm::Type"*** %48, align 8
  %51 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.153", %"class.std::__1::__libcpp_compressed_pair_imp.153"* %47, i32 0, i32 1
  %52 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %22, align 8
  store %"class.std::__1::allocator.82"* %52, %"class.std::__1::allocator.82"** %18, align 8
  %53 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %18, align 8
  store %"class.std::__1::allocator.82"* %53, %"class.std::__1::allocator.82"** %51, align 8
  %54 = load i64, i64* %31, align 8
  %55 = icmp ne i64 %54, 0
  br i1 %55, label %56, label %73

; <label>:56:                                     ; preds = %4
  store %"struct.std::__1::__split_buffer"* %34, %"struct.std::__1::__split_buffer"** %7, align 8
  %57 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %7, align 8
  %58 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %57, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.152"* %58, %"class.std::__1::__compressed_pair.152"** %6, align 8
  %59 = load %"class.std::__1::__compressed_pair.152"*, %"class.std::__1::__compressed_pair.152"** %6, align 8
  %60 = bitcast %"class.std::__1::__compressed_pair.152"* %59 to %"class.std::__1::__libcpp_compressed_pair_imp.153"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.153"* %60, %"class.std::__1::__libcpp_compressed_pair_imp.153"** %5, align 8
  %61 = load %"class.std::__1::__libcpp_compressed_pair_imp.153"*, %"class.std::__1::__libcpp_compressed_pair_imp.153"** %5, align 8
  %62 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.153", %"class.std::__1::__libcpp_compressed_pair_imp.153"* %61, i32 0, i32 1
  %63 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %62, align 8
  %64 = load i64, i64* %31, align 8
  store %"class.std::__1::allocator.82"* %63, %"class.std::__1::allocator.82"** %12, align 8
  store i64 %64, i64* %13, align 8
  %65 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %12, align 8
  %66 = load i64, i64* %13, align 8
  store %"class.std::__1::allocator.82"* %65, %"class.std::__1::allocator.82"** %9, align 8
  store i64 %66, i64* %10, align 8
  store i8* null, i8** %11, align 8
  %67 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %9, align 8
  %68 = load i64, i64* %10, align 8
  %69 = mul i64 %68, 8
  store i64 %69, i64* %8, align 8
  %70 = load i64, i64* %8, align 8
  %71 = call i8* @_Znwm(i64 %70) #17
  %72 = bitcast i8* %71 to %"class.llvm::Type"**
  br label %74

; <label>:73:                                     ; preds = %4
  br label %74

; <label>:74:                                     ; preds = %73, %56
  %75 = phi %"class.llvm::Type"** [ %72, %56 ], [ null, %73 ]
  %76 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %34, i32 0, i32 0
  store %"class.llvm::Type"** %75, %"class.llvm::Type"*** %76, align 8
  %77 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %34, i32 0, i32 0
  %78 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %77, align 8
  %79 = load i64, i64* %32, align 8
  %80 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %78, i64 %79
  %81 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %34, i32 0, i32 2
  store %"class.llvm::Type"** %80, %"class.llvm::Type"*** %81, align 8
  %82 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %34, i32 0, i32 1
  store %"class.llvm::Type"** %80, %"class.llvm::Type"*** %82, align 8
  %83 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %34, i32 0, i32 0
  %84 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %83, align 8
  %85 = load i64, i64* %31, align 8
  %86 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %84, i64 %85
  store %"struct.std::__1::__split_buffer"* %34, %"struct.std::__1::__split_buffer"** %16, align 8
  %87 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %16, align 8
  %88 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %87, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.152"* %88, %"class.std::__1::__compressed_pair.152"** %15, align 8
  %89 = load %"class.std::__1::__compressed_pair.152"*, %"class.std::__1::__compressed_pair.152"** %15, align 8
  %90 = bitcast %"class.std::__1::__compressed_pair.152"* %89 to %"class.std::__1::__libcpp_compressed_pair_imp.153"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.153"* %90, %"class.std::__1::__libcpp_compressed_pair_imp.153"** %14, align 8
  %91 = load %"class.std::__1::__libcpp_compressed_pair_imp.153"*, %"class.std::__1::__libcpp_compressed_pair_imp.153"** %14, align 8
  %92 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.153", %"class.std::__1::__libcpp_compressed_pair_imp.153"* %91, i32 0, i32 0
  store %"class.llvm::Type"** %86, %"class.llvm::Type"*** %92, align 8
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNKSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE17__annotate_deleteEv(%"class.std::__1::vector.78"*) #5 align 2 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %4 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %5 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %6 = alloca %"class.std::__1::vector.78"*, align 8
  %7 = alloca %"class.llvm::Type"**, align 8
  %8 = alloca %"class.std::__1::vector.78"*, align 8
  %9 = alloca %"class.std::__1::vector.78"*, align 8
  %10 = alloca %"class.llvm::Type"**, align 8
  %11 = alloca %"class.std::__1::vector.78"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %14 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %15 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %16 = alloca %"class.std::__1::vector.78"*, align 8
  %17 = alloca %"class.llvm::Type"**, align 8
  %18 = alloca %"class.std::__1::vector.78"*, align 8
  %19 = alloca %"class.llvm::Type"**, align 8
  %20 = alloca %"class.std::__1::vector.78"*, align 8
  %21 = alloca %"class.std::__1::vector.78"*, align 8
  store %"class.std::__1::vector.78"* %0, %"class.std::__1::vector.78"** %21, align 8
  %22 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %21, align 8
  store %"class.std::__1::vector.78"* %22, %"class.std::__1::vector.78"** %20, align 8
  %23 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %20, align 8
  %24 = bitcast %"class.std::__1::vector.78"* %23 to %"class.std::__1::__vector_base.79"*
  %25 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %24, i32 0, i32 0
  %26 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %25, align 8
  store %"class.llvm::Type"** %26, %"class.llvm::Type"*** %19, align 8
  %27 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %19, align 8
  %28 = bitcast %"class.llvm::Type"** %27 to i8*
  store %"class.std::__1::vector.78"* %22, %"class.std::__1::vector.78"** %18, align 8
  %29 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %18, align 8
  %30 = bitcast %"class.std::__1::vector.78"* %29 to %"class.std::__1::__vector_base.79"*
  %31 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %30, i32 0, i32 0
  %32 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %31, align 8
  store %"class.llvm::Type"** %32, %"class.llvm::Type"*** %17, align 8
  %33 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %17, align 8
  store %"class.std::__1::vector.78"* %22, %"class.std::__1::vector.78"** %6, align 8
  %34 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %6, align 8
  %35 = bitcast %"class.std::__1::vector.78"* %34 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %35, %"class.std::__1::__vector_base.79"** %5, align 8
  %36 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %5, align 8
  store %"class.std::__1::__vector_base.79"* %36, %"class.std::__1::__vector_base.79"** %4, align 8
  %37 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %4, align 8
  %38 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %37, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %38, %"class.std::__1::__compressed_pair.80"** %3, align 8
  %39 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %3, align 8
  %40 = bitcast %"class.std::__1::__compressed_pair.80"* %39 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %40, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %2, align 8
  %41 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %2, align 8
  %42 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %41, i32 0, i32 0
  %43 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %42, align 8
  %44 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %36, i32 0, i32 0
  %45 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %44, align 8
  %46 = ptrtoint %"class.llvm::Type"** %43 to i64
  %47 = ptrtoint %"class.llvm::Type"** %45 to i64
  %48 = sub i64 %46, %47
  %49 = sdiv exact i64 %48, 8
  %50 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %33, i64 %49
  %51 = bitcast %"class.llvm::Type"** %50 to i8*
  store %"class.std::__1::vector.78"* %22, %"class.std::__1::vector.78"** %8, align 8
  %52 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %8, align 8
  %53 = bitcast %"class.std::__1::vector.78"* %52 to %"class.std::__1::__vector_base.79"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %53, i32 0, i32 0
  %55 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %54, align 8
  store %"class.llvm::Type"** %55, %"class.llvm::Type"*** %7, align 8
  %56 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %7, align 8
  store %"class.std::__1::vector.78"* %22, %"class.std::__1::vector.78"** %9, align 8
  %57 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %9, align 8
  %58 = bitcast %"class.std::__1::vector.78"* %57 to %"class.std::__1::__vector_base.79"*
  %59 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %58, i32 0, i32 1
  %60 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %59, align 8
  %61 = bitcast %"class.std::__1::vector.78"* %57 to %"class.std::__1::__vector_base.79"*
  %62 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %61, i32 0, i32 0
  %63 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %62, align 8
  %64 = ptrtoint %"class.llvm::Type"** %60 to i64
  %65 = ptrtoint %"class.llvm::Type"** %63 to i64
  %66 = sub i64 %64, %65
  %67 = sdiv exact i64 %66, 8
  %68 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %56, i64 %67
  %69 = bitcast %"class.llvm::Type"** %68 to i8*
  store %"class.std::__1::vector.78"* %22, %"class.std::__1::vector.78"** %11, align 8
  %70 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %11, align 8
  %71 = bitcast %"class.std::__1::vector.78"* %70 to %"class.std::__1::__vector_base.79"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %71, i32 0, i32 0
  %73 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %72, align 8
  store %"class.llvm::Type"** %73, %"class.llvm::Type"*** %10, align 8
  %74 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %10, align 8
  store %"class.std::__1::vector.78"* %22, %"class.std::__1::vector.78"** %16, align 8
  %75 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %16, align 8
  %76 = bitcast %"class.std::__1::vector.78"* %75 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %76, %"class.std::__1::__vector_base.79"** %15, align 8
  %77 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %15, align 8
  store %"class.std::__1::__vector_base.79"* %77, %"class.std::__1::__vector_base.79"** %14, align 8
  %78 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %14, align 8
  %79 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %78, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %79, %"class.std::__1::__compressed_pair.80"** %13, align 8
  %80 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %13, align 8
  %81 = bitcast %"class.std::__1::__compressed_pair.80"* %80 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %81, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %12, align 8
  %82 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %12, align 8
  %83 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %82, i32 0, i32 0
  %84 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %83, align 8
  %85 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %77, i32 0, i32 0
  %86 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %85, align 8
  %87 = ptrtoint %"class.llvm::Type"** %84 to i64
  %88 = ptrtoint %"class.llvm::Type"** %86 to i64
  %89 = sub i64 %87, %88
  %90 = sdiv exact i64 %89, 8
  %91 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %74, i64 %90
  %92 = bitcast %"class.llvm::Type"** %91 to i8*
  call void @_ZNKSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE31__annotate_contiguous_containerEPKvS8_S8_S8_(%"class.std::__1::vector.78"* %22, i8* %28, i8* %51, i8* %69, i8* %92)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__114__split_bufferIPN4llvm4TypeERNS_9allocatorIS3_EEED2Ev(%"struct.std::__1::__split_buffer"*) unnamed_addr #5 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.153"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.152"*, align 8
  %4 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %5 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %"class.std::__1::allocator.82"*, align 8
  %8 = alloca %"class.llvm::Type"**, align 8
  %9 = alloca i64, align 8
  %10 = alloca %"class.std::__1::allocator.82"*, align 8
  %11 = alloca %"class.llvm::Type"**, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.153"*, align 8
  %14 = alloca %"class.std::__1::__compressed_pair.152"*, align 8
  %15 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %16 = alloca %"class.std::__1::allocator.82"*, align 8
  %17 = alloca %"class.llvm::Type"**, align 8
  %18 = alloca %"struct.std::__1::integral_constant.128", align 1
  %19 = alloca %"class.std::__1::allocator.82"*, align 8
  %20 = alloca %"class.llvm::Type"**, align 8
  %21 = alloca %"class.std::__1::allocator.82"*, align 8
  %22 = alloca %"class.llvm::Type"**, align 8
  %23 = alloca %"struct.std::__1::integral_constant.128", align 1
  %24 = alloca %"struct.std::__1::__has_destroy.129", align 1
  %25 = alloca %"class.llvm::Type"**, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.153"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.152"*, align 8
  %28 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %29 = alloca %"struct.std::__1::integral_constant", align 1
  %30 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %31 = alloca %"class.llvm::Type"**, align 8
  %32 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %33 = alloca %"class.llvm::Type"**, align 8
  %34 = alloca %"struct.std::__1::integral_constant", align 1
  %35 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %36 = alloca %"struct.std::__1::__split_buffer"*, align 8
  store %"struct.std::__1::__split_buffer"* %0, %"struct.std::__1::__split_buffer"** %36, align 8
  %37 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %36, align 8
  store %"struct.std::__1::__split_buffer"* %37, %"struct.std::__1::__split_buffer"** %35, align 8
  %38 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %35, align 8
  %39 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %38, i32 0, i32 1
  %40 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %39, align 8
  store %"struct.std::__1::__split_buffer"* %38, %"struct.std::__1::__split_buffer"** %32, align 8
  store %"class.llvm::Type"** %40, %"class.llvm::Type"*** %33, align 8
  %41 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %32, align 8
  %42 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %33, align 8
  store %"struct.std::__1::__split_buffer"* %41, %"struct.std::__1::__split_buffer"** %30, align 8
  store %"class.llvm::Type"** %42, %"class.llvm::Type"*** %31, align 8
  %43 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %30, align 8
  br label %44

; <label>:44:                                     ; preds = %49, %1
  %45 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %31, align 8
  %46 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %43, i32 0, i32 2
  %47 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %46, align 8
  %48 = icmp ne %"class.llvm::Type"** %45, %47
  br i1 %48, label %49, label %68

; <label>:49:                                     ; preds = %44
  store %"struct.std::__1::__split_buffer"* %43, %"struct.std::__1::__split_buffer"** %28, align 8
  %50 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %28, align 8
  %51 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %50, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.152"* %51, %"class.std::__1::__compressed_pair.152"** %27, align 8
  %52 = load %"class.std::__1::__compressed_pair.152"*, %"class.std::__1::__compressed_pair.152"** %27, align 8
  %53 = bitcast %"class.std::__1::__compressed_pair.152"* %52 to %"class.std::__1::__libcpp_compressed_pair_imp.153"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.153"* %53, %"class.std::__1::__libcpp_compressed_pair_imp.153"** %26, align 8
  %54 = load %"class.std::__1::__libcpp_compressed_pair_imp.153"*, %"class.std::__1::__libcpp_compressed_pair_imp.153"** %26, align 8
  %55 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.153", %"class.std::__1::__libcpp_compressed_pair_imp.153"* %54, i32 0, i32 1
  %56 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %55, align 8
  %57 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %43, i32 0, i32 2
  %58 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %57, align 8
  %59 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %58, i32 -1
  store %"class.llvm::Type"** %59, %"class.llvm::Type"*** %57, align 8
  store %"class.llvm::Type"** %59, %"class.llvm::Type"*** %25, align 8
  %60 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %25, align 8
  store %"class.std::__1::allocator.82"* %56, %"class.std::__1::allocator.82"** %21, align 8
  store %"class.llvm::Type"** %60, %"class.llvm::Type"*** %22, align 8
  %61 = bitcast %"struct.std::__1::__has_destroy.129"* %24 to %"struct.std::__1::integral_constant.128"*
  %62 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %21, align 8
  %63 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %22, align 8
  store %"class.std::__1::allocator.82"* %62, %"class.std::__1::allocator.82"** %19, align 8
  store %"class.llvm::Type"** %63, %"class.llvm::Type"*** %20, align 8
  %64 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %19, align 8
  %65 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %20, align 8
  store %"class.std::__1::allocator.82"* %64, %"class.std::__1::allocator.82"** %16, align 8
  store %"class.llvm::Type"** %65, %"class.llvm::Type"*** %17, align 8
  %66 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %16, align 8
  %67 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %17, align 8
  br label %44

; <label>:68:                                     ; preds = %44
  %69 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %37, i32 0, i32 0
  %70 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %69, align 8
  %71 = icmp ne %"class.llvm::Type"** %70, null
  br i1 %71, label %72, label %104

; <label>:72:                                     ; preds = %68
  store %"struct.std::__1::__split_buffer"* %37, %"struct.std::__1::__split_buffer"** %15, align 8
  %73 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %15, align 8
  %74 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %73, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.152"* %74, %"class.std::__1::__compressed_pair.152"** %14, align 8
  %75 = load %"class.std::__1::__compressed_pair.152"*, %"class.std::__1::__compressed_pair.152"** %14, align 8
  %76 = bitcast %"class.std::__1::__compressed_pair.152"* %75 to %"class.std::__1::__libcpp_compressed_pair_imp.153"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.153"* %76, %"class.std::__1::__libcpp_compressed_pair_imp.153"** %13, align 8
  %77 = load %"class.std::__1::__libcpp_compressed_pair_imp.153"*, %"class.std::__1::__libcpp_compressed_pair_imp.153"** %13, align 8
  %78 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.153", %"class.std::__1::__libcpp_compressed_pair_imp.153"* %77, i32 0, i32 1
  %79 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %78, align 8
  %80 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %37, i32 0, i32 0
  %81 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %80, align 8
  store %"struct.std::__1::__split_buffer"* %37, %"struct.std::__1::__split_buffer"** %5, align 8
  %82 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %5, align 8
  store %"struct.std::__1::__split_buffer"* %82, %"struct.std::__1::__split_buffer"** %4, align 8
  %83 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %4, align 8
  %84 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %83, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.152"* %84, %"class.std::__1::__compressed_pair.152"** %3, align 8
  %85 = load %"class.std::__1::__compressed_pair.152"*, %"class.std::__1::__compressed_pair.152"** %3, align 8
  %86 = bitcast %"class.std::__1::__compressed_pair.152"* %85 to %"class.std::__1::__libcpp_compressed_pair_imp.153"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.153"* %86, %"class.std::__1::__libcpp_compressed_pair_imp.153"** %2, align 8
  %87 = load %"class.std::__1::__libcpp_compressed_pair_imp.153"*, %"class.std::__1::__libcpp_compressed_pair_imp.153"** %2, align 8
  %88 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.153", %"class.std::__1::__libcpp_compressed_pair_imp.153"* %87, i32 0, i32 0
  %89 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %88, align 8
  %90 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %82, i32 0, i32 0
  %91 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %90, align 8
  %92 = ptrtoint %"class.llvm::Type"** %89 to i64
  %93 = ptrtoint %"class.llvm::Type"** %91 to i64
  %94 = sub i64 %92, %93
  %95 = sdiv exact i64 %94, 8
  br label %96

; <label>:96:                                     ; preds = %72
  store %"class.std::__1::allocator.82"* %79, %"class.std::__1::allocator.82"** %10, align 8
  store %"class.llvm::Type"** %81, %"class.llvm::Type"*** %11, align 8
  store i64 %95, i64* %12, align 8
  %97 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %10, align 8
  %98 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %11, align 8
  %99 = load i64, i64* %12, align 8
  store %"class.std::__1::allocator.82"* %97, %"class.std::__1::allocator.82"** %7, align 8
  store %"class.llvm::Type"** %98, %"class.llvm::Type"*** %8, align 8
  store i64 %99, i64* %9, align 8
  %100 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %7, align 8
  %101 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %8, align 8
  %102 = bitcast %"class.llvm::Type"** %101 to i8*
  store i8* %102, i8** %6, align 8
  %103 = load i8*, i8** %6, align 8
  call void @_ZdlPv(i8* %103) #14
  br label %104

; <label>:104:                                    ; preds = %96, %68
  ret void
                                                  ; No predecessors!
  %106 = landingpad { i8*, i32 }
          catch i8* null
  %107 = extractvalue { i8*, i32 } %106, 0
  call void @__clang_call_terminate(i8* %107) #15
  unreachable
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE21__push_back_slow_pathIS3_EEvOT_(%"class.std::__1::vector.78"*, %"class.llvm::Type"** dereferenceable(8)) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.llvm::Type"**, align 8
  %4 = alloca %"class.std::__1::allocator.82"*, align 8
  %5 = alloca %"class.llvm::Type"**, align 8
  %6 = alloca %"class.llvm::Type"**, align 8
  %7 = alloca %"class.llvm::Type"**, align 8
  %8 = alloca %"struct.std::__1::integral_constant.128", align 1
  %9 = alloca %"class.std::__1::allocator.82"*, align 8
  %10 = alloca %"class.llvm::Type"**, align 8
  %11 = alloca %"class.llvm::Type"**, align 8
  %12 = alloca %"class.llvm::Type"**, align 8
  %13 = alloca %"class.std::__1::allocator.82"*, align 8
  %14 = alloca %"class.llvm::Type"**, align 8
  %15 = alloca %"class.llvm::Type"**, align 8
  %16 = alloca %"struct.std::__1::integral_constant.128", align 1
  %17 = alloca %"struct.std::__1::__has_construct.154", align 1
  %18 = alloca %"class.llvm::Type"**, align 8
  %19 = alloca %"class.llvm::Type"**, align 8
  %20 = alloca %"class.std::__1::vector.78"*, align 8
  %21 = alloca %"struct.std::__1::__less"*, align 8
  %22 = alloca i64*, align 8
  %23 = alloca i64*, align 8
  %24 = alloca %"struct.std::__1::__less", align 1
  %25 = alloca i64*, align 8
  %26 = alloca i64*, align 8
  %27 = alloca i64*, align 8
  %28 = alloca i64*, align 8
  %29 = alloca %"struct.std::__1::__less", align 1
  %30 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %31 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %32 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %33 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %34 = alloca %"class.std::__1::vector.78"*, align 8
  %35 = alloca i64, align 8
  %36 = alloca %"class.std::__1::vector.78"*, align 8
  %37 = alloca i64, align 8
  %38 = alloca i64, align 8
  %39 = alloca i64, align 8
  %40 = alloca i64, align 8
  %41 = alloca %"class.std::__1::vector.78"*, align 8
  %42 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %43 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %44 = alloca %"class.std::__1::__vector_base.79"*, align 8
  %45 = alloca %"class.std::__1::vector.78"*, align 8
  %46 = alloca %"class.llvm::Type"**, align 8
  %47 = alloca %"class.std::__1::allocator.82"*, align 8
  %48 = alloca %"struct.std::__1::__split_buffer", align 8
  %49 = alloca i8*
  %50 = alloca i32
  store %"class.std::__1::vector.78"* %0, %"class.std::__1::vector.78"** %45, align 8
  store %"class.llvm::Type"** %1, %"class.llvm::Type"*** %46, align 8
  %51 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %45, align 8
  %52 = bitcast %"class.std::__1::vector.78"* %51 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %52, %"class.std::__1::__vector_base.79"** %44, align 8
  %53 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %44, align 8
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %53, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %54, %"class.std::__1::__compressed_pair.80"** %43, align 8
  %55 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %43, align 8
  %56 = bitcast %"class.std::__1::__compressed_pair.80"* %55 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %56, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %42, align 8
  %57 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %42, align 8
  %58 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.81"* %57 to %"class.std::__1::allocator.82"*
  store %"class.std::__1::allocator.82"* %58, %"class.std::__1::allocator.82"** %47, align 8
  store %"class.std::__1::vector.78"* %51, %"class.std::__1::vector.78"** %41, align 8
  %59 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %41, align 8
  %60 = bitcast %"class.std::__1::vector.78"* %59 to %"class.std::__1::__vector_base.79"*
  %61 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %60, i32 0, i32 1
  %62 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %61, align 8
  %63 = bitcast %"class.std::__1::vector.78"* %59 to %"class.std::__1::__vector_base.79"*
  %64 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %63, i32 0, i32 0
  %65 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %64, align 8
  %66 = ptrtoint %"class.llvm::Type"** %62 to i64
  %67 = ptrtoint %"class.llvm::Type"** %65 to i64
  %68 = sub i64 %66, %67
  %69 = sdiv exact i64 %68, 8
  %70 = add i64 %69, 1
  store %"class.std::__1::vector.78"* %51, %"class.std::__1::vector.78"** %36, align 8
  store i64 %70, i64* %37, align 8
  %71 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %36, align 8
  %72 = call i64 @_ZNKSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE8max_sizeEv(%"class.std::__1::vector.78"* %71) #2
  store i64 %72, i64* %38, align 8
  %73 = load i64, i64* %37, align 8
  %74 = load i64, i64* %38, align 8
  %75 = icmp ugt i64 %73, %74
  br i1 %75, label %76, label %78

; <label>:76:                                     ; preds = %2
  %77 = bitcast %"class.std::__1::vector.78"* %71 to %"class.std::__1::__vector_base_common"*
  call void @_ZNKSt3__120__vector_base_commonILb1EE20__throw_length_errorEv(%"class.std::__1::__vector_base_common"* %77)
  br label %78

; <label>:78:                                     ; preds = %76, %2
  store %"class.std::__1::vector.78"* %71, %"class.std::__1::vector.78"** %34, align 8
  %79 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %34, align 8
  %80 = bitcast %"class.std::__1::vector.78"* %79 to %"class.std::__1::__vector_base.79"*
  store %"class.std::__1::__vector_base.79"* %80, %"class.std::__1::__vector_base.79"** %33, align 8
  %81 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %33, align 8
  store %"class.std::__1::__vector_base.79"* %81, %"class.std::__1::__vector_base.79"** %32, align 8
  %82 = load %"class.std::__1::__vector_base.79"*, %"class.std::__1::__vector_base.79"** %32, align 8
  %83 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %82, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.80"* %83, %"class.std::__1::__compressed_pair.80"** %31, align 8
  %84 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %31, align 8
  %85 = bitcast %"class.std::__1::__compressed_pair.80"* %84 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %85, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %30, align 8
  %86 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %30, align 8
  %87 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %86, i32 0, i32 0
  %88 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %87, align 8
  %89 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %81, i32 0, i32 0
  %90 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %89, align 8
  %91 = ptrtoint %"class.llvm::Type"** %88 to i64
  %92 = ptrtoint %"class.llvm::Type"** %90 to i64
  %93 = sub i64 %91, %92
  %94 = sdiv exact i64 %93, 8
  store i64 %94, i64* %39, align 8
  %95 = load i64, i64* %39, align 8
  %96 = load i64, i64* %38, align 8
  %97 = udiv i64 %96, 2
  %98 = icmp uge i64 %95, %97
  br i1 %98, label %99, label %101

; <label>:99:                                     ; preds = %78
  %100 = load i64, i64* %38, align 8
  store i64 %100, i64* %35, align 8
  br label %121

; <label>:101:                                    ; preds = %78
  %102 = load i64, i64* %39, align 8
  %103 = mul i64 2, %102
  store i64 %103, i64* %40, align 8
  store i64* %40, i64** %27, align 8
  store i64* %37, i64** %28, align 8
  %104 = load i64*, i64** %27, align 8
  %105 = load i64*, i64** %28, align 8
  store i64* %104, i64** %25, align 8
  store i64* %105, i64** %26, align 8
  %106 = load i64*, i64** %25, align 8
  %107 = load i64*, i64** %26, align 8
  store %"struct.std::__1::__less"* %24, %"struct.std::__1::__less"** %21, align 8
  store i64* %106, i64** %22, align 8
  store i64* %107, i64** %23, align 8
  %108 = load %"struct.std::__1::__less"*, %"struct.std::__1::__less"** %21, align 8
  %109 = load i64*, i64** %22, align 8
  %110 = load i64, i64* %109, align 8
  %111 = load i64*, i64** %23, align 8
  %112 = load i64, i64* %111, align 8
  %113 = icmp ult i64 %110, %112
  br i1 %113, label %114, label %116

; <label>:114:                                    ; preds = %101
  %115 = load i64*, i64** %26, align 8
  br label %118

; <label>:116:                                    ; preds = %101
  %117 = load i64*, i64** %25, align 8
  br label %118

; <label>:118:                                    ; preds = %116, %114
  %119 = phi i64* [ %115, %114 ], [ %117, %116 ]
  %120 = load i64, i64* %119, align 8
  store i64 %120, i64* %35, align 8
  br label %121

; <label>:121:                                    ; preds = %99, %118
  %122 = load i64, i64* %35, align 8
  store %"class.std::__1::vector.78"* %51, %"class.std::__1::vector.78"** %20, align 8
  %123 = load %"class.std::__1::vector.78"*, %"class.std::__1::vector.78"** %20, align 8
  %124 = bitcast %"class.std::__1::vector.78"* %123 to %"class.std::__1::__vector_base.79"*
  %125 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %124, i32 0, i32 1
  %126 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %125, align 8
  %127 = bitcast %"class.std::__1::vector.78"* %123 to %"class.std::__1::__vector_base.79"*
  %128 = getelementptr inbounds %"class.std::__1::__vector_base.79", %"class.std::__1::__vector_base.79"* %127, i32 0, i32 0
  %129 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %128, align 8
  %130 = ptrtoint %"class.llvm::Type"** %126 to i64
  %131 = ptrtoint %"class.llvm::Type"** %129 to i64
  %132 = sub i64 %130, %131
  %133 = sdiv exact i64 %132, 8
  %134 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %47, align 8
  call void @_ZNSt3__114__split_bufferIPN4llvm4TypeERNS_9allocatorIS3_EEEC1EmmS6_(%"struct.std::__1::__split_buffer"* %48, i64 %122, i64 %133, %"class.std::__1::allocator.82"* dereferenceable(1) %134)
  %135 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %47, align 8
  %136 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %48, i32 0, i32 2
  %137 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %136, align 8
  store %"class.llvm::Type"** %137, %"class.llvm::Type"*** %19, align 8
  %138 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %19, align 8
  %139 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %46, align 8
  store %"class.llvm::Type"** %139, %"class.llvm::Type"*** %18, align 8
  %140 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %18, align 8
  store %"class.std::__1::allocator.82"* %135, %"class.std::__1::allocator.82"** %13, align 8
  store %"class.llvm::Type"** %138, %"class.llvm::Type"*** %14, align 8
  store %"class.llvm::Type"** %140, %"class.llvm::Type"*** %15, align 8
  %141 = bitcast %"struct.std::__1::__has_construct.154"* %17 to %"struct.std::__1::integral_constant.128"*
  %142 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %13, align 8
  %143 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %14, align 8
  %144 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %15, align 8
  store %"class.llvm::Type"** %144, %"class.llvm::Type"*** %12, align 8
  %145 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %12, align 8
  store %"class.std::__1::allocator.82"* %142, %"class.std::__1::allocator.82"** %9, align 8
  store %"class.llvm::Type"** %143, %"class.llvm::Type"*** %10, align 8
  store %"class.llvm::Type"** %145, %"class.llvm::Type"*** %11, align 8
  %146 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %9, align 8
  %147 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %10, align 8
  %148 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %11, align 8
  store %"class.llvm::Type"** %148, %"class.llvm::Type"*** %7, align 8
  %149 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %7, align 8
  store %"class.std::__1::allocator.82"* %146, %"class.std::__1::allocator.82"** %4, align 8
  store %"class.llvm::Type"** %147, %"class.llvm::Type"*** %5, align 8
  store %"class.llvm::Type"** %149, %"class.llvm::Type"*** %6, align 8
  %150 = load %"class.std::__1::allocator.82"*, %"class.std::__1::allocator.82"** %4, align 8
  %151 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %5, align 8
  %152 = bitcast %"class.llvm::Type"** %151 to i8*
  %153 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %6, align 8
  store %"class.llvm::Type"** %153, %"class.llvm::Type"*** %3, align 8
  %154 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %3, align 8
  %155 = load %"class.llvm::Type"*, %"class.llvm::Type"** %154, align 8
  store %"class.llvm::Type"* %155, %"class.llvm::Type"** %151, align 8
  br label %156

; <label>:156:                                    ; preds = %121
  %157 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %48, i32 0, i32 2
  %158 = load %"class.llvm::Type"**, %"class.llvm::Type"*** %157, align 8
  %159 = getelementptr inbounds %"class.llvm::Type"*, %"class.llvm::Type"** %158, i32 1
  store %"class.llvm::Type"** %159, %"class.llvm::Type"*** %157, align 8
  invoke void @_ZNSt3__16vectorIPN4llvm4TypeENS_9allocatorIS3_EEE26__swap_out_circular_bufferERNS_14__split_bufferIS3_RS5_EE(%"class.std::__1::vector.78"* %51, %"struct.std::__1::__split_buffer"* dereferenceable(40) %48)
          to label %160 unwind label %161

; <label>:160:                                    ; preds = %156
  call void @_ZNSt3__114__split_bufferIPN4llvm4TypeERNS_9allocatorIS3_EEED1Ev(%"struct.std::__1::__split_buffer"* %48) #2
  ret void

; <label>:161:                                    ; preds = %156
  %162 = landingpad { i8*, i32 }
          cleanup
  %163 = extractvalue { i8*, i32 } %162, 0
  store i8* %163, i8** %49, align 8
  %164 = extractvalue { i8*, i32 } %162, 1
  store i32 %164, i32* %50, align 4
  call void @_ZNSt3__114__split_bufferIPN4llvm4TypeERNS_9allocatorIS3_EEED1Ev(%"struct.std::__1::__split_buffer"* %48) #2
  br label %165

; <label>:165:                                    ; preds = %161
  %166 = load i8*, i8** %49, align 8
  %167 = load i32, i32* %50, align 4
  %168 = insertvalue { i8*, i32 } undef, i8* %166, 0
  %169 = insertvalue { i8*, i32 } %168, i32 %167, 1
  resume { i8*, i32 } %169
}

declare dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"*, i64, i8*) #3

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm8ArrayRefIPNS_8ConstantEEC2Ev(%"class.llvm::ArrayRef.121"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.llvm::ArrayRef.121"*, align 8
  store %"class.llvm::ArrayRef.121"* %0, %"class.llvm::ArrayRef.121"** %2, align 8
  %3 = load %"class.llvm::ArrayRef.121"*, %"class.llvm::ArrayRef.121"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::ArrayRef.121", %"class.llvm::ArrayRef.121"* %3, i32 0, i32 0
  store %"class.llvm::Constant"** null, %"class.llvm::Constant"*** %4, align 8
  %5 = getelementptr inbounds %"class.llvm::ArrayRef.121", %"class.llvm::ArrayRef.121"* %3, i32 0, i32 1
  store i64 0, i64* %5, align 8
  ret void
}

; Function Attrs: ssp uwtable
define internal void @_GLOBAL__sub_I_CGType.cpp() #0 section "__TEXT,__StaticInit,regular,pure_instructions" {
  call void @__cxx_global_var_init()
  call void @__cxx_global_var_init.1()
  ret void
}

attributes #0 = { ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { inlinehint nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { noinline noreturn nounwind }
attributes #7 = { nobuiltin nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { noreturn "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #9 = { argmemonly nounwind }
attributes #10 = { inlinehint ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #11 = { nobuiltin "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #12 = { nounwind readnone }
attributes #13 = { nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #14 = { builtin nounwind }
attributes #15 = { noreturn nounwind }
attributes #16 = { noreturn }
attributes #17 = { builtin }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"Apple LLVM version 8.1.0 (clang-802.0.42)"}
!2 = !{!3}
!3 = distinct !{!3, !4, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!4 = distinct !{!4, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!5 = !{!6}
!6 = distinct !{!6, !7, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!7 = distinct !{!7, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!8 = !{!9}
!9 = distinct !{!9, !10, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!10 = distinct !{!10, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
