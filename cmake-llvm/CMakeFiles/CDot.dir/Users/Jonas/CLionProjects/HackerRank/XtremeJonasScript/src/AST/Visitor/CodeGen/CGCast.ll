; ModuleID = '/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/AST/Visitor/CodeGen/CGCast.cpp'
source_filename = "/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/AST/Visitor/CodeGen/CGCast.cpp"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.12.0"

%"class.llvm::IRBuilder" = type <{ %"class.llvm::IRBuilderBase", %"class.llvm::ConstantFolder", [7 x i8] }>
%"class.llvm::IRBuilderBase" = type { %"class.llvm::DebugLoc", %"class.llvm::BasicBlock"*, %"class.llvm::ilist_iterator", %"class.llvm::LLVMContext"*, %"class.llvm::MDNode"*, %"class.llvm::FastMathFlags", %"class.llvm::ArrayRef" }
%"class.llvm::DebugLoc" = type { %"class.llvm::TypedTrackingMDRef" }
%"class.llvm::TypedTrackingMDRef" = type { %"class.llvm::TrackingMDRef" }
%"class.llvm::TrackingMDRef" = type { %"class.llvm::Metadata"* }
%"class.llvm::Metadata" = type { i8, i8, i16, i32 }
%"class.llvm::BasicBlock" = type { %"class.llvm::Value", %"class.llvm::ilist_node_with_parent", %"class.llvm::SymbolTableList.53", %"class.llvm::Function"* }
%"class.llvm::Value" = type { i32 (...)**, %"class.llvm::Type"*, %"class.llvm::Use"*, i8, i8, i16, i32 }
%"class.llvm::Type" = type { %"class.llvm::LLVMContext"*, i32, i32, %"class.llvm::Type"** }
%"class.llvm::Use" = type { %"class.llvm::Value"*, %"class.llvm::Use"*, %"class.llvm::PointerIntPair" }
%"class.llvm::PointerIntPair" = type { i64 }
%"class.llvm::ilist_node_with_parent" = type { %"class.llvm::ilist_node.52" }
%"class.llvm::ilist_node.52" = type { %"class.llvm::ilist_node_impl.28" }
%"class.llvm::ilist_node_impl.28" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::ilist_node_base" = type { %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"* }
%"class.llvm::SymbolTableList.53" = type { %"class.llvm::iplist_impl.54" }
%"class.llvm::iplist_impl.54" = type { %"class.llvm::simple_ilist.57" }
%"class.llvm::simple_ilist.57" = type { %"class.llvm::ilist_sentinel.59" }
%"class.llvm::ilist_sentinel.59" = type { %"class.llvm::ilist_node_impl.60" }
%"class.llvm::ilist_node_impl.60" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::Function" = type { %"class.llvm::GlobalObject", %"class.llvm::ilist_node", %"class.llvm::SymbolTableList", %"class.llvm::SymbolTableList.29", %"class.std::__1::unique_ptr.37", %"class.llvm::AttributeSet" }
%"class.llvm::GlobalObject" = type { %"class.llvm::GlobalValue", %"class.llvm::Comdat"* }
%"class.llvm::GlobalValue" = type { %"class.llvm::Constant", %"class.llvm::Type"*, i32, i32, %"class.llvm::Module"* }
%"class.llvm::Constant" = type { %"class.llvm::User" }
%"class.llvm::User" = type { %"class.llvm::Value" }
%"class.llvm::Module" = type opaque
%"class.llvm::Comdat" = type opaque
%"class.llvm::ilist_node" = type { %"class.llvm::ilist_node_impl" }
%"class.llvm::ilist_node_impl" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::SymbolTableList" = type { %"class.llvm::iplist_impl" }
%"class.llvm::iplist_impl" = type { %"class.llvm::simple_ilist" }
%"class.llvm::simple_ilist" = type { %"class.llvm::ilist_sentinel" }
%"class.llvm::ilist_sentinel" = type { %"class.llvm::ilist_node_impl.28" }
%"class.llvm::SymbolTableList.29" = type { %"class.llvm::iplist_impl.30" }
%"class.llvm::iplist_impl.30" = type { %"class.llvm::simple_ilist.33" }
%"class.llvm::simple_ilist.33" = type { %"class.llvm::ilist_sentinel.35" }
%"class.llvm::ilist_sentinel.35" = type { %"class.llvm::ilist_node_impl.36" }
%"class.llvm::ilist_node_impl.36" = type { %"class.llvm::ilist_node_base" }
%"class.std::__1::unique_ptr.37" = type { %"class.std::__1::__compressed_pair.38" }
%"class.std::__1::__compressed_pair.38" = type { %"class.std::__1::__libcpp_compressed_pair_imp.39" }
%"class.std::__1::__libcpp_compressed_pair_imp.39" = type { %"class.llvm::ValueSymbolTable"* }
%"class.llvm::ValueSymbolTable" = type opaque
%"class.llvm::AttributeSet" = type { %"class.llvm::AttributeSetImpl"* }
%"class.llvm::AttributeSetImpl" = type opaque
%"class.llvm::ilist_iterator" = type { %"class.llvm::ilist_node_impl.60"* }
%"class.llvm::LLVMContext" = type { %"class.llvm::LLVMContextImpl"* }
%"class.llvm::LLVMContextImpl" = type opaque
%"class.llvm::MDNode" = type { %"class.llvm::Metadata", i32, i32, %"class.llvm::ContextAndReplaceableUses" }
%"class.llvm::ContextAndReplaceableUses" = type { %"class.llvm::PointerUnion" }
%"class.llvm::PointerUnion" = type { %"class.llvm::PointerIntPair.202" }
%"class.llvm::PointerIntPair.202" = type { i64 }
%"class.llvm::FastMathFlags" = type { i32 }
%"class.llvm::ArrayRef" = type { %"class.llvm::OperandBundleDefT"*, i64 }
%"class.llvm::OperandBundleDefT" = type { %"class.std::__1::basic_string", %"class.std::__1::vector.203" }
%"class.std::__1::basic_string" = type { %"class.std::__1::__compressed_pair" }
%"class.std::__1::__compressed_pair" = type { %"class.std::__1::__libcpp_compressed_pair_imp" }
%"class.std::__1::__libcpp_compressed_pair_imp" = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" = type { %union.anon }
%union.anon = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" = type { i64, i64, i8* }
%"class.std::__1::vector.203" = type { %"class.std::__1::__vector_base.204" }
%"class.std::__1::__vector_base.204" = type { %"class.llvm::Value"**, %"class.llvm::Value"**, %"class.std::__1::__compressed_pair.205" }
%"class.std::__1::__compressed_pair.205" = type { %"class.std::__1::__libcpp_compressed_pair_imp.206" }
%"class.std::__1::__libcpp_compressed_pair_imp.206" = type { %"class.llvm::Value"** }
%"class.llvm::ConstantFolder" = type { i8 }
%"class.std::__1::unordered_map.212" = type { %"class.std::__1::__hash_table.213" }
%"class.std::__1::__hash_table.213" = type <{ %"class.std::__1::unique_ptr.214", %"class.std::__1::__compressed_pair.223", %"class.std::__1::__compressed_pair.227", %"class.std::__1::__compressed_pair.230", [4 x i8] }>
%"class.std::__1::unique_ptr.214" = type { %"class.std::__1::__compressed_pair.215" }
%"class.std::__1::__compressed_pair.215" = type { %"class.std::__1::__libcpp_compressed_pair_imp.216" }
%"class.std::__1::__libcpp_compressed_pair_imp.216" = type { %"struct.std::__1::__hash_node_base.217"**, %"class.std::__1::__bucket_list_deallocator.218" }
%"struct.std::__1::__hash_node_base.217" = type { %"struct.std::__1::__hash_node_base.217"* }
%"class.std::__1::__bucket_list_deallocator.218" = type { %"class.std::__1::__compressed_pair.219" }
%"class.std::__1::__compressed_pair.219" = type { %"class.std::__1::__libcpp_compressed_pair_imp.220" }
%"class.std::__1::__libcpp_compressed_pair_imp.220" = type { i64 }
%"class.std::__1::__compressed_pair.223" = type { %"class.std::__1::__libcpp_compressed_pair_imp.224" }
%"class.std::__1::__libcpp_compressed_pair_imp.224" = type { %"struct.std::__1::__hash_node_base.217" }
%"class.std::__1::__compressed_pair.227" = type { %"class.std::__1::__libcpp_compressed_pair_imp.228" }
%"class.std::__1::__libcpp_compressed_pair_imp.228" = type { i64 }
%"class.std::__1::__compressed_pair.230" = type { %"class.std::__1::__libcpp_compressed_pair_imp.231" }
%"class.std::__1::__libcpp_compressed_pair_imp.231" = type { float }
%"struct.std::__1::piecewise_construct_t" = type { i8 }
%struct.TypeSpecifier = type { i32, %"class.std::__1::basic_string", %"class.std::__1::shared_ptr", i8, i8, i8, i8, i64, i32, i8, i8, %"class.std::__1::basic_string", i8, [7 x i8], %"class.std::__1::unordered_map", i8, i8, i8, i8, i8, i8, i8, i32, %"class.std::__1::vector", %"class.std::__1::vector.19", %union.anon.25 }
%"class.std::__1::shared_ptr" = type { %class.Expression*, %"class.std::__1::__shared_weak_count"* }
%class.Expression = type opaque
%"class.std::__1::__shared_weak_count" = type { %"class.std::__1::__shared_count", i64 }
%"class.std::__1::__shared_count" = type { i32 (...)**, i64 }
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
%class.CodeGenVisitor = type { i32 (...)**, %"class.std::__1::vector.26", %"class.std::__1::vector.44", %"class.std::__1::vector.50", %"class.std::__1::vector.50", %"class.std::__1::unordered_map.65", %"class.std::__1::unordered_map.87", i8, [7 x i8], %"class.std::__1::unordered_map.109", %"class.std::__1::unordered_map.135", %"class.std::__1::unordered_map.157", %"class.std::__1::unordered_map.157", %"class.llvm::ConstantInt"*, %"class.llvm::ConstantInt"*, %"class.llvm::ConstantInt"*, %"class.llvm::ConstantInt"*, %"class.std::__1::unordered_map.180", %"class.std::__1::vector" }
%"class.std::__1::vector.26" = type { %"class.std::__1::__vector_base.27" }
%"class.std::__1::__vector_base.27" = type { %"class.llvm::Function"**, %"class.llvm::Function"**, %"class.std::__1::__compressed_pair.40" }
%"class.std::__1::__compressed_pair.40" = type { %"class.std::__1::__libcpp_compressed_pair_imp.41" }
%"class.std::__1::__libcpp_compressed_pair_imp.41" = type { %"class.llvm::Function"** }
%"class.std::__1::vector.44" = type { %"class.std::__1::__vector_base.45" }
%"class.std::__1::__vector_base.45" = type { %struct.CGValue**, %struct.CGValue**, %"class.std::__1::__compressed_pair.46" }
%struct.CGValue = type { %"class.llvm::Value"*, i8, i64, i8, i8, i8, i32 }
%"class.std::__1::__compressed_pair.46" = type { %"class.std::__1::__libcpp_compressed_pair_imp.47" }
%"class.std::__1::__libcpp_compressed_pair_imp.47" = type { %struct.CGValue** }
%"class.std::__1::vector.50" = type { %"class.std::__1::__vector_base.51" }
%"class.std::__1::__vector_base.51" = type { %"class.llvm::BasicBlock"**, %"class.llvm::BasicBlock"**, %"class.std::__1::__compressed_pair.61" }
%"class.std::__1::__compressed_pair.61" = type { %"class.std::__1::__libcpp_compressed_pair_imp.62" }
%"class.std::__1::__libcpp_compressed_pair_imp.62" = type { %"class.llvm::BasicBlock"** }
%"class.std::__1::unordered_map.65" = type { %"class.std::__1::__hash_table.66" }
%"class.std::__1::__hash_table.66" = type <{ %"class.std::__1::unique_ptr.67", %"class.std::__1::__compressed_pair.76", %"class.std::__1::__compressed_pair.80", %"class.std::__1::__compressed_pair.83", [4 x i8] }>
%"class.std::__1::unique_ptr.67" = type { %"class.std::__1::__compressed_pair.68" }
%"class.std::__1::__compressed_pair.68" = type { %"class.std::__1::__libcpp_compressed_pair_imp.69" }
%"class.std::__1::__libcpp_compressed_pair_imp.69" = type { %"struct.std::__1::__hash_node_base.70"**, %"class.std::__1::__bucket_list_deallocator.71" }
%"struct.std::__1::__hash_node_base.70" = type { %"struct.std::__1::__hash_node_base.70"* }
%"class.std::__1::__bucket_list_deallocator.71" = type { %"class.std::__1::__compressed_pair.72" }
%"class.std::__1::__compressed_pair.72" = type { %"class.std::__1::__libcpp_compressed_pair_imp.73" }
%"class.std::__1::__libcpp_compressed_pair_imp.73" = type { i64 }
%"class.std::__1::__compressed_pair.76" = type { %"class.std::__1::__libcpp_compressed_pair_imp.77" }
%"class.std::__1::__libcpp_compressed_pair_imp.77" = type { %"struct.std::__1::__hash_node_base.70" }
%"class.std::__1::__compressed_pair.80" = type { %"class.std::__1::__libcpp_compressed_pair_imp.81" }
%"class.std::__1::__libcpp_compressed_pair_imp.81" = type { i64 }
%"class.std::__1::__compressed_pair.83" = type { %"class.std::__1::__libcpp_compressed_pair_imp.84" }
%"class.std::__1::__libcpp_compressed_pair_imp.84" = type { float }
%"class.std::__1::unordered_map.87" = type { %"class.std::__1::__hash_table.88" }
%"class.std::__1::__hash_table.88" = type <{ %"class.std::__1::unique_ptr.89", %"class.std::__1::__compressed_pair.98", %"class.std::__1::__compressed_pair.102", %"class.std::__1::__compressed_pair.105", [4 x i8] }>
%"class.std::__1::unique_ptr.89" = type { %"class.std::__1::__compressed_pair.90" }
%"class.std::__1::__compressed_pair.90" = type { %"class.std::__1::__libcpp_compressed_pair_imp.91" }
%"class.std::__1::__libcpp_compressed_pair_imp.91" = type { %"struct.std::__1::__hash_node_base.92"**, %"class.std::__1::__bucket_list_deallocator.93" }
%"struct.std::__1::__hash_node_base.92" = type { %"struct.std::__1::__hash_node_base.92"* }
%"class.std::__1::__bucket_list_deallocator.93" = type { %"class.std::__1::__compressed_pair.94" }
%"class.std::__1::__compressed_pair.94" = type { %"class.std::__1::__libcpp_compressed_pair_imp.95" }
%"class.std::__1::__libcpp_compressed_pair_imp.95" = type { i64 }
%"class.std::__1::__compressed_pair.98" = type { %"class.std::__1::__libcpp_compressed_pair_imp.99" }
%"class.std::__1::__libcpp_compressed_pair_imp.99" = type { %"struct.std::__1::__hash_node_base.92" }
%"class.std::__1::__compressed_pair.102" = type { %"class.std::__1::__libcpp_compressed_pair_imp.103" }
%"class.std::__1::__libcpp_compressed_pair_imp.103" = type { i64 }
%"class.std::__1::__compressed_pair.105" = type { %"class.std::__1::__libcpp_compressed_pair_imp.106" }
%"class.std::__1::__libcpp_compressed_pair_imp.106" = type { float }
%"class.std::__1::unordered_map.109" = type { %"class.std::__1::__hash_table.110" }
%"class.std::__1::__hash_table.110" = type <{ %"class.std::__1::unique_ptr.111", %"class.std::__1::__compressed_pair.120", %"class.std::__1::__compressed_pair.124", %"class.std::__1::__compressed_pair.129", [4 x i8] }>
%"class.std::__1::unique_ptr.111" = type { %"class.std::__1::__compressed_pair.112" }
%"class.std::__1::__compressed_pair.112" = type { %"class.std::__1::__libcpp_compressed_pair_imp.113" }
%"class.std::__1::__libcpp_compressed_pair_imp.113" = type { %"struct.std::__1::__hash_node_base.114"**, %"class.std::__1::__bucket_list_deallocator.115" }
%"struct.std::__1::__hash_node_base.114" = type { %"struct.std::__1::__hash_node_base.114"* }
%"class.std::__1::__bucket_list_deallocator.115" = type { %"class.std::__1::__compressed_pair.116" }
%"class.std::__1::__compressed_pair.116" = type { %"class.std::__1::__libcpp_compressed_pair_imp.117" }
%"class.std::__1::__libcpp_compressed_pair_imp.117" = type { i64 }
%"class.std::__1::__compressed_pair.120" = type { %"class.std::__1::__libcpp_compressed_pair_imp.121" }
%"class.std::__1::__libcpp_compressed_pair_imp.121" = type { %"struct.std::__1::__hash_node_base.114" }
%"class.std::__1::__compressed_pair.124" = type { %"class.std::__1::__libcpp_compressed_pair_imp.125" }
%"class.std::__1::__libcpp_compressed_pair_imp.125" = type { i64 }
%"class.std::__1::__compressed_pair.129" = type { %"class.std::__1::__libcpp_compressed_pair_imp.130" }
%"class.std::__1::__libcpp_compressed_pair_imp.130" = type { float }
%"class.std::__1::unordered_map.135" = type { %"class.std::__1::__hash_table.136" }
%"class.std::__1::__hash_table.136" = type <{ %"class.std::__1::unique_ptr.137", %"class.std::__1::__compressed_pair.146", %"class.std::__1::__compressed_pair.150", %"class.std::__1::__compressed_pair.153", [4 x i8] }>
%"class.std::__1::unique_ptr.137" = type { %"class.std::__1::__compressed_pair.138" }
%"class.std::__1::__compressed_pair.138" = type { %"class.std::__1::__libcpp_compressed_pair_imp.139" }
%"class.std::__1::__libcpp_compressed_pair_imp.139" = type { %"struct.std::__1::__hash_node_base.140"**, %"class.std::__1::__bucket_list_deallocator.141" }
%"struct.std::__1::__hash_node_base.140" = type { %"struct.std::__1::__hash_node_base.140"* }
%"class.std::__1::__bucket_list_deallocator.141" = type { %"class.std::__1::__compressed_pair.142" }
%"class.std::__1::__compressed_pair.142" = type { %"class.std::__1::__libcpp_compressed_pair_imp.143" }
%"class.std::__1::__libcpp_compressed_pair_imp.143" = type { i64 }
%"class.std::__1::__compressed_pair.146" = type { %"class.std::__1::__libcpp_compressed_pair_imp.147" }
%"class.std::__1::__libcpp_compressed_pair_imp.147" = type { %"struct.std::__1::__hash_node_base.140" }
%"class.std::__1::__compressed_pair.150" = type { %"class.std::__1::__libcpp_compressed_pair_imp.151" }
%"class.std::__1::__libcpp_compressed_pair_imp.151" = type { i64 }
%"class.std::__1::__compressed_pair.153" = type { %"class.std::__1::__libcpp_compressed_pair_imp.154" }
%"class.std::__1::__libcpp_compressed_pair_imp.154" = type { float }
%"class.std::__1::unordered_map.157" = type { %"class.std::__1::__hash_table.158" }
%"class.std::__1::__hash_table.158" = type <{ %"class.std::__1::unique_ptr.159", %"class.std::__1::__compressed_pair.168", %"class.std::__1::__compressed_pair.172", %"class.std::__1::__compressed_pair.175", [4 x i8] }>
%"class.std::__1::unique_ptr.159" = type { %"class.std::__1::__compressed_pair.160" }
%"class.std::__1::__compressed_pair.160" = type { %"class.std::__1::__libcpp_compressed_pair_imp.161" }
%"class.std::__1::__libcpp_compressed_pair_imp.161" = type { %"struct.std::__1::__hash_node_base.162"**, %"class.std::__1::__bucket_list_deallocator.163" }
%"struct.std::__1::__hash_node_base.162" = type { %"struct.std::__1::__hash_node_base.162"* }
%"class.std::__1::__bucket_list_deallocator.163" = type { %"class.std::__1::__compressed_pair.164" }
%"class.std::__1::__compressed_pair.164" = type { %"class.std::__1::__libcpp_compressed_pair_imp.165" }
%"class.std::__1::__libcpp_compressed_pair_imp.165" = type { i64 }
%"class.std::__1::__compressed_pair.168" = type { %"class.std::__1::__libcpp_compressed_pair_imp.169" }
%"class.std::__1::__libcpp_compressed_pair_imp.169" = type { %"struct.std::__1::__hash_node_base.162" }
%"class.std::__1::__compressed_pair.172" = type { %"class.std::__1::__libcpp_compressed_pair_imp.173" }
%"class.std::__1::__libcpp_compressed_pair_imp.173" = type { i64 }
%"class.std::__1::__compressed_pair.175" = type { %"class.std::__1::__libcpp_compressed_pair_imp.176" }
%"class.std::__1::__libcpp_compressed_pair_imp.176" = type { float }
%"class.llvm::ConstantInt" = type { %"class.llvm::ConstantData", %"class.llvm::APInt" }
%"class.llvm::ConstantData" = type { %"class.llvm::Constant" }
%"class.llvm::APInt" = type { i32, %union.anon.179 }
%union.anon.179 = type { i64 }
%"class.std::__1::unordered_map.180" = type { %"class.std::__1::__hash_table.181" }
%"class.std::__1::__hash_table.181" = type <{ %"class.std::__1::unique_ptr.182", %"class.std::__1::__compressed_pair.191", %"class.std::__1::__compressed_pair.195", %"class.std::__1::__compressed_pair.198", [4 x i8] }>
%"class.std::__1::unique_ptr.182" = type { %"class.std::__1::__compressed_pair.183" }
%"class.std::__1::__compressed_pair.183" = type { %"class.std::__1::__libcpp_compressed_pair_imp.184" }
%"class.std::__1::__libcpp_compressed_pair_imp.184" = type { %"struct.std::__1::__hash_node_base.185"**, %"class.std::__1::__bucket_list_deallocator.186" }
%"struct.std::__1::__hash_node_base.185" = type { %"struct.std::__1::__hash_node_base.185"* }
%"class.std::__1::__bucket_list_deallocator.186" = type { %"class.std::__1::__compressed_pair.187" }
%"class.std::__1::__compressed_pair.187" = type { %"class.std::__1::__libcpp_compressed_pair_imp.188" }
%"class.std::__1::__libcpp_compressed_pair_imp.188" = type { i64 }
%"class.std::__1::__compressed_pair.191" = type { %"class.std::__1::__libcpp_compressed_pair_imp.192" }
%"class.std::__1::__libcpp_compressed_pair_imp.192" = type { %"struct.std::__1::__hash_node_base.185" }
%"class.std::__1::__compressed_pair.195" = type { %"class.std::__1::__libcpp_compressed_pair_imp.196" }
%"class.std::__1::__libcpp_compressed_pair_imp.196" = type { i64 }
%"class.std::__1::__compressed_pair.198" = type { %"class.std::__1::__libcpp_compressed_pair_imp.199" }
%"class.std::__1::__libcpp_compressed_pair_imp.199" = type { float }
%"class.std::__1::allocator" = type { i8 }
%"class.llvm::Twine" = type <{ %"union.llvm::Twine::Child", %"union.llvm::Twine::Child", i8, i8, [6 x i8] }>
%"union.llvm::Twine::Child" = type { %"class.llvm::Twine"* }
%"class.llvm::CallInst" = type { %"class.llvm::Instruction", %"class.llvm::AttributeSet", %"class.llvm::FunctionType"* }
%"class.llvm::Instruction" = type { %"class.llvm::User", %"class.llvm::ilist_node_with_parent.209", %"class.llvm::BasicBlock"*, %"class.llvm::DebugLoc" }
%"class.llvm::ilist_node_with_parent.209" = type { %"class.llvm::ilist_node.210" }
%"class.llvm::ilist_node.210" = type { %"class.llvm::ilist_node_impl.60" }
%"class.llvm::FunctionType" = type { %"class.llvm::Type" }
%"class.llvm::ArrayRef.211" = type { %"class.llvm::Value"**, i64 }
%"class.std::initializer_list" = type { %"class.llvm::Value"**, i64 }
%"class.std::__1::__basic_string_common" = type { i8 }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw" = type { [3 x i64] }
%"class.llvm::PointerType" = type { %"class.llvm::Type", %"class.llvm::Type"* }
%"class.llvm::ConstantPointerNull" = type { %"class.llvm::ConstantData" }
%"class.llvm::IntegerType" = type { %"class.llvm::Type" }
%class.AstNode = type opaque
%"class.std::__1::tuple.236" = type { i8 }
%"class.std::__1::__tuple_leaf" = type { %"class.std::__1::basic_string"* }
%"struct.std::__1::__tuple_indices" = type { i8 }
%"struct.std::__1::__tuple_types" = type { i8 }
%"struct.std::__1::__tuple_indices.243" = type { i8 }
%"struct.std::__1::__tuple_types.245" = type { i8 }
%"struct.std::__1::__tuple_impl" = type { %"class.std::__1::__tuple_leaf" }
%"class.std::__1::tuple" = type { %"struct.std::__1::__tuple_impl" }
%"struct.std::__1::equal_to" = type { i8 }
%"class.std::__1::__unordered_map_equal.232" = type { i8 }
%"union.std::__1::__hash_value_type" = type { %"struct.std::__1::pair.237" }
%"struct.std::__1::pair.237" = type { %"class.std::__1::basic_string", %"class.llvm::Constant"* }
%"struct.std::__1::__less" = type { i8 }
%"class.std::__1::__libcpp_compressed_pair_imp.241" = type { %"struct.std::__1::__hash_node"*, %"class.std::__1::__hash_node_destructor" }
%"struct.std::__1::__hash_node" = type { %"struct.std::__1::__hash_node_base.217", i64, %"union.std::__1::__hash_value_type" }
%"class.std::__1::__hash_node_destructor" = type <{ %"class.std::__1::allocator.225"*, i8, [7 x i8] }>
%"class.std::__1::allocator.225" = type { i8 }
%"class.std::__1::__compressed_pair.240" = type { %"class.std::__1::__libcpp_compressed_pair_imp.241" }
%"struct.std::__1::integral_constant.244" = type { i8 }
%"struct.std::__1::__has_destroy" = type { i8 }
%"class.std::__1::unique_ptr.239" = type { %"class.std::__1::__compressed_pair.240" }
%"class.std::__1::__unordered_map_hasher.229" = type { i8 }
%"class.std::__1::__hash_iterator" = type { %"struct.std::__1::__hash_node_base.217"* }
%"struct.std::__1::pair" = type <{ %"class.std::__1::__hash_iterator", i8, [7 x i8] }>
%"struct.std::__1::hash" = type { i8 }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short" = type { %union.anon.0, [23 x i8] }
%union.anon.0 = type { i8 }
%"class.llvm::CastInst" = type { %"class.llvm::UnaryInstruction" }
%"class.llvm::UnaryInstruction" = type { %"class.llvm::Instruction" }
%"class.llvm::IRBuilderDefaultInserter" = type { i8 }
%"class.llvm::SymbolTableListTraits.55" = type { i8 }
%"struct.llvm::ilist_detail::SpecificNodeAccess.58" = type { i8 }
%"class.llvm::PointerUnion.234" = type { %"class.llvm::PointerIntPair.235" }
%"class.llvm::PointerIntPair.235" = type { i64 }
%"struct.std::__1::integral_constant" = type { i8 }
%"struct.std::__1::__has_construct" = type { i8 }
%"struct.std::__1::__murmur2_or_cityhash" = type { i8 }
%"struct.std::__1::pair.242" = type { i64, i64 }
%"class.std::__1::allocator.221" = type { i8 }
%"class.llvm::OperandBundleUser" = type { i8 }

@_ZN4llvm24DisableABIBreakingChecksE = external global i32, align 4
@_ZN4llvm30VerifyDisableABIBreakingChecksE = weak hidden global i32* @_ZN4llvm24DisableABIBreakingChecksE, align 8
@_ZN14CodeGenVisitor7BuilderE = external global %"class.llvm::IRBuilder", align 8
@.str = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.2 = private unnamed_addr constant [3 x i8] c"%F\00", align 1
@.str.3 = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@_ZN14CodeGenVisitor9FunctionsE = external global %"class.std::__1::unordered_map.212", align 8
@.str.4 = private unnamed_addr constant [9 x i8] c"snprintf\00", align 1
@.str.5 = private unnamed_addr constant [8 x i8] c"sprintf\00", align 1
@.str.6 = private unnamed_addr constant [26 x i8] c"Casting from long to long\00", align 1
@.str.7 = private unnamed_addr constant [28 x i8] c"Casting from float to float\00", align 1
@.str.8 = private unnamed_addr constant [30 x i8] c"Casting from double to double\00", align 1
@.str.9 = private unnamed_addr constant [26 x i8] c"Casting from bool to bool\00", align 1
@.str.10 = private unnamed_addr constant [26 x i8] c"Casting from char to char\00", align 1
@.str.11 = private unnamed_addr constant [16 x i8] c"Cannot cast to \00", align 1
@.str.12 = private unnamed_addr constant [15 x i8] c"Invalid twine!\00", align 1
@__func__._ZN4llvm5TwineC2EPKc = private unnamed_addr constant [6 x i8] c"Twine\00", align 1
@.str.13 = private unnamed_addr constant [56 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/ADT/Twine.h\00", align 1
@.str.14 = private unnamed_addr constant [30 x i8] c"isValid() && \22Invalid twine!\22\00", align 1
@.str.15 = private unnamed_addr constant [29 x i8] c"isa<> used on a null pointer\00", align 1
@__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_ = private unnamed_addr constant [5 x i8] c"doit\00", align 1
@.str.16 = private unnamed_addr constant [62 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/Support/Casting.h\00", align 1
@.str.17 = private unnamed_addr constant [38 x i8] c"Val && \22isa<> used on a null pointer\22\00", align 1
@.str.18 = private unnamed_addr constant [42 x i8] c"cast<Ty>() argument of incompatible type!\00", align 1
@__func__._ZN4llvm4castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_ = private unnamed_addr constant [5 x i8] c"cast\00", align 1
@.str.19 = private unnamed_addr constant [59 x i8] c"isa<X>(Val) && \22cast<Ty>() argument of incompatible type!\22\00", align 1
@.str.20 = private unnamed_addr constant [25 x i8] c"Expected values to match\00", align 1
@__func__._ZN4llvm13TrackingMDRef7retrackERS0_ = private unnamed_addr constant [8 x i8] c"retrack\00", align 1
@.str.21 = private unnamed_addr constant [63 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/IR/TrackingMDRef.h\00", align 1
@.str.22 = private unnamed_addr constant [41 x i8] c"MD == X.MD && \22Expected values to match\22\00", align 1
@.str.23 = private unnamed_addr constant [28 x i8] c"Integer too large for field\00", align 1
@__func__._ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE9updateIntEll = private unnamed_addr constant [10 x i8] c"updateInt\00", align 1
@.str.24 = private unnamed_addr constant [65 x i8] c"/usr/local/Cellar/llvm/4.0.0_1/include/llvm/ADT/PointerIntPair.h\00", align 1
@.str.25 = private unnamed_addr constant [59 x i8] c"(IntWord & ~IntMask) == 0 && \22Integer too large for field\22\00", align 1
@.str.26 = private unnamed_addr constant [36 x i8] c"Pointer is not sufficiently aligned\00", align 1
@__func__._ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE13updatePointerElS1_ = private unnamed_addr constant [14 x i8] c"updatePointer\00", align 1
@.str.27 = private unnamed_addr constant [74 x i8] c"(PtrWord & ~PointerBitMask) == 0 && \22Pointer is not sufficiently aligned\22\00", align 1
@_ZNSt3__1L19piecewise_constructE = internal constant %"struct.std::__1::piecewise_construct_t" undef, align 1
@_ZTVN4llvm8CallInstE = external unnamed_addr constant [5 x i8*]

; Function Attrs: ssp uwtable
define %"class.llvm::Value"* @_ZN6CGCast10CreateCastER13TypeSpecifierS1_PN4llvm5ValueER14CodeGenVisitorb(%struct.TypeSpecifier* dereferenceable(216), %struct.TypeSpecifier* dereferenceable(216), %"class.llvm::Value"*, %class.CodeGenVisitor* dereferenceable(448), i1 zeroext) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %8 = alloca %"class.std::__1::basic_string"*, align 8
  %9 = alloca [3 x i64]*, align 8
  %10 = alloca i32, align 4
  %11 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca %"class.std::__1::basic_string"*, align 8
  %14 = alloca %"class.std::__1::basic_string"*, align 8
  %15 = alloca %"class.std::__1::basic_string"*, align 8
  %16 = alloca %"class.std::__1::basic_string"*, align 8
  %17 = alloca i8*, align 8
  %18 = alloca %"class.std::__1::basic_string"*, align 8
  %19 = alloca %"class.std::__1::allocator"*, align 8
  %20 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %21 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %23 = alloca %"class.std::__1::basic_string"*, align 8
  %24 = alloca i8*, align 8
  %25 = alloca %"class.std::__1::basic_string"*, align 8
  %26 = alloca i8*, align 8
  %27 = alloca %"class.std::__1::allocator"*, align 8
  %28 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %29 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %30 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %31 = alloca %"class.std::__1::basic_string"*, align 8
  %32 = alloca i8*, align 8
  %33 = alloca %"class.std::__1::basic_string"*, align 8
  %34 = alloca i8*, align 8
  %35 = alloca %"class.std::__1::allocator"*, align 8
  %36 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %37 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %38 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %39 = alloca %"class.std::__1::basic_string"*, align 8
  %40 = alloca i8*, align 8
  %41 = alloca %"class.std::__1::basic_string"*, align 8
  %42 = alloca i8*, align 8
  %43 = alloca %"class.std::__1::allocator"*, align 8
  %44 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %45 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %46 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %47 = alloca %"class.std::__1::basic_string"*, align 8
  %48 = alloca i8*, align 8
  %49 = alloca %"class.std::__1::basic_string"*, align 8
  %50 = alloca i8*, align 8
  %51 = alloca %"class.std::__1::allocator"*, align 8
  %52 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %53 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %54 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %55 = alloca %"class.std::__1::basic_string"*, align 8
  %56 = alloca i8*, align 8
  %57 = alloca %"class.std::__1::basic_string"*, align 8
  %58 = alloca i8*, align 8
  %59 = alloca %"class.std::__1::allocator"*, align 8
  %60 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %61 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %62 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %63 = alloca %"class.std::__1::basic_string"*, align 8
  %64 = alloca i8*, align 8
  %65 = alloca %"class.std::__1::basic_string"*, align 8
  %66 = alloca i8*, align 8
  %67 = alloca %"class.std::__1::allocator"*, align 8
  %68 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %69 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %70 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %71 = alloca %"class.std::__1::basic_string"*, align 8
  %72 = alloca i8*, align 8
  %73 = alloca %"class.std::__1::basic_string"*, align 8
  %74 = alloca i8*, align 8
  %75 = alloca %"class.std::__1::allocator"*, align 8
  %76 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %77 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %78 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %79 = alloca %"class.std::__1::basic_string"*, align 8
  %80 = alloca i8*, align 8
  %81 = alloca %"class.std::__1::basic_string"*, align 8
  %82 = alloca i8*, align 8
  %83 = alloca %"class.std::__1::basic_string"*, align 8
  %84 = alloca i8*, align 8
  %85 = alloca %"class.std::__1::basic_string"*, align 8
  %86 = alloca i8*, align 8
  %87 = alloca %"class.std::__1::basic_string"*, align 8
  %88 = alloca i8*, align 8
  %89 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %90 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %91 = alloca %"class.std::__1::basic_string"*, align 8
  %92 = alloca [3 x i64]*, align 8
  %93 = alloca i32, align 4
  %94 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %95 = alloca %"class.std::__1::basic_string"*, align 8
  %96 = alloca %"class.std::__1::basic_string"*, align 8
  %97 = alloca %"class.std::__1::basic_string"*, align 8
  %98 = alloca %"class.std::__1::basic_string"*, align 8
  %99 = alloca %"class.std::__1::basic_string"*, align 8
  %100 = alloca i8*, align 8
  %101 = alloca %"class.std::__1::basic_string"*, align 8
  %102 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %103 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %104 = alloca %"class.std::__1::basic_string"*, align 8
  %105 = alloca [3 x i64]*, align 8
  %106 = alloca i32, align 4
  %107 = alloca %"class.std::__1::allocator"*, align 8
  %108 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %109 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %110 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %111 = alloca %"class.std::__1::basic_string"*, align 8
  %112 = alloca %"class.std::__1::basic_string"*, align 8
  %113 = alloca %"class.llvm::Value"*, align 8
  %114 = alloca %struct.TypeSpecifier*, align 8
  %115 = alloca %struct.TypeSpecifier*, align 8
  %116 = alloca %"class.llvm::Value"*, align 8
  %117 = alloca %class.CodeGenVisitor*, align 8
  %118 = alloca i8, align 1
  %119 = alloca %"class.llvm::IRBuilder"*, align 8
  %120 = alloca %"class.llvm::Twine", align 8
  %121 = alloca %"class.std::__1::basic_string", align 8
  %122 = alloca i8*
  %123 = alloca i32
  %124 = alloca i32
  %125 = alloca %"class.llvm::Value"*, align 8
  %126 = alloca %struct.CGValue, align 8
  %127 = alloca %"class.llvm::CallInst"*, align 8
  %128 = alloca %"class.std::__1::basic_string", align 8
  %129 = alloca %"class.llvm::ArrayRef.211", align 8
  %130 = alloca %"class.std::initializer_list", align 8
  %131 = alloca [4 x %"class.llvm::Value"*], align 8
  %132 = alloca %"class.llvm::Twine", align 8
  %133 = alloca %"class.llvm::Value"*, align 8
  %134 = alloca %"class.std::__1::basic_string", align 8
  %135 = alloca %"class.std::__1::basic_string", align 8
  %136 = alloca %"class.llvm::ArrayRef.211", align 8
  %137 = alloca %"class.std::initializer_list", align 8
  %138 = alloca [3 x %"class.llvm::Value"*], align 8
  %139 = alloca %"class.llvm::Twine", align 8
  %140 = alloca %"class.llvm::Type"*, align 8
  %141 = alloca %"class.llvm::Twine", align 8
  %142 = alloca %"class.llvm::Twine", align 8
  %143 = alloca %"class.llvm::Twine", align 8
  %144 = alloca %"class.llvm::Twine", align 8
  %145 = alloca %"class.std::__1::basic_string", align 8
  %146 = alloca %"class.llvm::Twine", align 8
  %147 = alloca %"class.llvm::Twine", align 8
  %148 = alloca %"class.std::__1::basic_string", align 8
  %149 = alloca %"class.llvm::Twine", align 8
  %150 = alloca %"class.llvm::Twine", align 8
  %151 = alloca %"class.std::__1::basic_string", align 8
  %152 = alloca %"class.llvm::Twine", align 8
  %153 = alloca %"class.llvm::Twine", align 8
  %154 = alloca %"class.std::__1::basic_string", align 8
  %155 = alloca %"class.llvm::Twine", align 8
  %156 = alloca %"class.llvm::Twine", align 8
  %157 = alloca %"class.std::__1::basic_string", align 8
  %158 = alloca %"class.llvm::Twine", align 8
  %159 = alloca %"class.llvm::Twine", align 8
  %160 = alloca %"class.llvm::Twine", align 8
  %161 = alloca %"class.std::__1::basic_string", align 8
  %162 = alloca %"class.std::__1::basic_string", align 8
  %163 = alloca %"class.std::__1::basic_string", align 8
  %164 = alloca %"class.std::__1::basic_string", align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %114, align 8
  store %struct.TypeSpecifier* %1, %struct.TypeSpecifier** %115, align 8
  store %"class.llvm::Value"* %2, %"class.llvm::Value"** %116, align 8
  store %class.CodeGenVisitor* %3, %class.CodeGenVisitor** %117, align 8
  %165 = zext i1 %4 to i8
  store i8 %165, i8* %118, align 1
  %166 = load %class.CodeGenVisitor*, %class.CodeGenVisitor** %117, align 8
  store %"class.llvm::IRBuilder"* @_ZN14CodeGenVisitor7BuilderE, %"class.llvm::IRBuilder"** %119, align 8
  %167 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %168 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %167, i32 0, i32 0
  %169 = load i32, i32* %168, align 8
  %170 = icmp eq i32 %169, 7
  br i1 %170, label %171, label %182

; <label>:171:                                    ; preds = %5
  %172 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %114, align 8
  %173 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %172, i32 0, i32 0
  %174 = load i32, i32* %173, align 8
  %175 = icmp eq i32 %174, 7
  br i1 %175, label %176, label %182

; <label>:176:                                    ; preds = %171
  %177 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %178 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %179 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %180 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %179)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %120, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %181 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE13CreateBitCastEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %177, %"class.llvm::Value"* %178, %"class.llvm::Type"* %180, %"class.llvm::Twine"* dereferenceable(24) %120)
  store %"class.llvm::Value"* %181, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:182:                                    ; preds = %171, %5
  %183 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %114, align 8
  %184 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %183, i32 0, i32 0
  %185 = load i32, i32* %184, align 8
  %186 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %187 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %186, i32 0, i32 0
  %188 = load i32, i32* %187, align 8
  %189 = icmp eq i32 %185, %188
  br i1 %189, label %190, label %192

; <label>:190:                                    ; preds = %182
  %191 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  store %"class.llvm::Value"* %191, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:192:                                    ; preds = %182
  %193 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %194 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %193, i32 0, i32 0
  %195 = load i32, i32* %194, align 8
  %196 = icmp eq i32 %195, 4
  br i1 %196, label %197, label %404

; <label>:197:                                    ; preds = %192
  store %"class.std::__1::basic_string"* %121, %"class.std::__1::basic_string"** %112, align 8
  %198 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %112, align 8
  store %"class.std::__1::basic_string"* %198, %"class.std::__1::basic_string"** %111, align 8
  %199 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %111, align 8
  %200 = bitcast %"class.std::__1::basic_string"* %199 to %"class.std::__1::__basic_string_common"*
  %201 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %199, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %201, %"class.std::__1::__compressed_pair"** %110, align 8
  %202 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %110, align 8
  store %"class.std::__1::__compressed_pair"* %202, %"class.std::__1::__compressed_pair"** %109, align 8
  %203 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %109, align 8
  %204 = bitcast %"class.std::__1::__compressed_pair"* %203 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %204, %"class.std::__1::__libcpp_compressed_pair_imp"** %108, align 8
  %205 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %108, align 8
  %206 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %205 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %206, %"class.std::__1::allocator"** %107, align 8
  %207 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %107, align 8
  %208 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %205, i32 0, i32 0
  %209 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %208 to i8*
  call void @llvm.memset.p0i8.i64(i8* %209, i8 0, i64 24, i32 8, i1 false) #14
  store %"class.std::__1::basic_string"* %199, %"class.std::__1::basic_string"** %104, align 8
  %210 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %104, align 8
  %211 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %210, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %211, %"class.std::__1::__compressed_pair"** %103, align 8
  %212 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %103, align 8
  %213 = bitcast %"class.std::__1::__compressed_pair"* %212 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %213, %"class.std::__1::__libcpp_compressed_pair_imp"** %102, align 8
  %214 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %102, align 8
  %215 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %214, i32 0, i32 0
  %216 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %215, i32 0, i32 0
  %217 = bitcast %union.anon* %216 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %218 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %217, i32 0, i32 0
  store [3 x i64]* %218, [3 x i64]** %105, align 8
  store i32 0, i32* %106, align 4
  br label %219

; <label>:219:                                    ; preds = %222, %197
  %220 = load i32, i32* %106, align 4
  %221 = icmp ult i32 %220, 3
  br i1 %221, label %222, label %229

; <label>:222:                                    ; preds = %219
  %223 = load i32, i32* %106, align 4
  %224 = zext i32 %223 to i64
  %225 = load [3 x i64]*, [3 x i64]** %105, align 8
  %226 = getelementptr inbounds [3 x i64], [3 x i64]* %225, i64 0, i64 %224
  store i64 0, i64* %226, align 8
  %227 = load i32, i32* %106, align 4
  %228 = add i32 %227, 1
  store i32 %228, i32* %106, align 4
  br label %219

; <label>:229:                                    ; preds = %219
  %230 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %114, align 8
  %231 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %230, i32 0, i32 0
  %232 = load i32, i32* %231, align 8
  switch i32 %232, label %233 [
    i32 0, label %234
    i32 1, label %234
    i32 5, label %234
    i32 2, label %244
    i32 3, label %244
    i32 6, label %250
    i32 4, label %256
  ]

; <label>:233:                                    ; preds = %229
  br label %234

; <label>:234:                                    ; preds = %229, %229, %229, %233
  store %"class.std::__1::basic_string"* %121, %"class.std::__1::basic_string"** %87, align 8
  store i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i8** %88, align 8
  %235 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %87, align 8
  %236 = load i8*, i8** %88, align 8
  %237 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEPKc(%"class.std::__1::basic_string"* %235, i8* %236)
          to label %238 unwind label %240

; <label>:238:                                    ; preds = %234
  br label %239

; <label>:239:                                    ; preds = %238
  br label %258

; <label>:240:                                    ; preds = %364, %346, %340, %322, %280, %260, %250, %244, %234, %318, %258
  %241 = landingpad { i8*, i32 }
          cleanup
  %242 = extractvalue { i8*, i32 } %241, 0
  store i8* %242, i8** %122, align 8
  %243 = extractvalue { i8*, i32 } %241, 1
  store i32 %243, i32* %123, align 4
  br label %403

; <label>:244:                                    ; preds = %229, %229
  store %"class.std::__1::basic_string"* %121, %"class.std::__1::basic_string"** %85, align 8
  store i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i32 0, i32 0), i8** %86, align 8
  %245 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %85, align 8
  %246 = load i8*, i8** %86, align 8
  %247 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEPKc(%"class.std::__1::basic_string"* %245, i8* %246)
          to label %248 unwind label %240

; <label>:248:                                    ; preds = %244
  br label %249

; <label>:249:                                    ; preds = %248
  br label %258

; <label>:250:                                    ; preds = %229
  store %"class.std::__1::basic_string"* %121, %"class.std::__1::basic_string"** %83, align 8
  store i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.3, i32 0, i32 0), i8** %84, align 8
  %251 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %83, align 8
  %252 = load i8*, i8** %84, align 8
  %253 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEPKc(%"class.std::__1::basic_string"* %251, i8* %252)
          to label %254 unwind label %240

; <label>:254:                                    ; preds = %250
  br label %255

; <label>:255:                                    ; preds = %254
  br label %258

; <label>:256:                                    ; preds = %229
  %257 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  store %"class.llvm::Value"* %257, %"class.llvm::Value"** %113, align 8
  store i32 1, i32* %124, align 4
  br label %402

; <label>:258:                                    ; preds = %255, %249, %239
  %259 = load %class.CodeGenVisitor*, %class.CodeGenVisitor** %117, align 8
  invoke void @_ZN14CodeGenVisitor9GetStringERNSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEEbb(%struct.CGValue* sret %126, %class.CodeGenVisitor* %259, %"class.std::__1::basic_string"* dereferenceable(24) %121, i1 zeroext false, i1 zeroext false)
          to label %260 unwind label %240

; <label>:260:                                    ; preds = %258
  %261 = getelementptr inbounds %struct.CGValue, %struct.CGValue* %126, i32 0, i32 0
  %262 = load %"class.llvm::Value"*, %"class.llvm::Value"** %261, align 8
  store %"class.llvm::Value"* %262, %"class.llvm::Value"** %125, align 8
  %263 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  store %"class.std::__1::basic_string"* %128, %"class.std::__1::basic_string"** %81, align 8
  store i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.4, i32 0, i32 0), i8** %82, align 8
  %264 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %81, align 8
  %265 = load i8*, i8** %82, align 8
  store %"class.std::__1::basic_string"* %264, %"class.std::__1::basic_string"** %79, align 8
  store i8* %265, i8** %80, align 8
  %266 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %79, align 8
  %267 = bitcast %"class.std::__1::basic_string"* %266 to %"class.std::__1::__basic_string_common"*
  %268 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %266, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %268, %"class.std::__1::__compressed_pair"** %78, align 8
  %269 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %78, align 8
  store %"class.std::__1::__compressed_pair"* %269, %"class.std::__1::__compressed_pair"** %77, align 8
  %270 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %77, align 8
  %271 = bitcast %"class.std::__1::__compressed_pair"* %270 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %271, %"class.std::__1::__libcpp_compressed_pair_imp"** %76, align 8
  %272 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %76, align 8
  %273 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %272 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %273, %"class.std::__1::allocator"** %75, align 8
  %274 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %75, align 8
  %275 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %272, i32 0, i32 0
  %276 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %275 to i8*
  call void @llvm.memset.p0i8.i64(i8* %276, i8 0, i64 24, i32 8, i1 false) #14
  %277 = load i8*, i8** %80, align 8
  %278 = load i8*, i8** %80, align 8
  %279 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %278)
          to label %280 unwind label %240

; <label>:280:                                    ; preds = %260
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %266, i8* %277, i64 %279)
          to label %281 unwind label %240

; <label>:281:                                    ; preds = %280
  br label %282

; <label>:282:                                    ; preds = %281
  %283 = invoke dereferenceable(8) %"class.llvm::Constant"** @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantENS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S9_EEEEEixEOS6_(%"class.std::__1::unordered_map.212"* @_ZN14CodeGenVisitor9FunctionsE, %"class.std::__1::basic_string"* dereferenceable(24) %128)
          to label %284 unwind label %390

; <label>:284:                                    ; preds = %282
  %285 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %283, align 8
  %286 = bitcast %"class.llvm::Constant"* %285 to %"class.llvm::Value"*
  %287 = getelementptr inbounds [4 x %"class.llvm::Value"*], [4 x %"class.llvm::Value"*]* %131, i64 0, i64 0
  %288 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %289 = bitcast %"class.llvm::IRBuilder"* %288 to %"class.llvm::IRBuilderBase"*
  %290 = invoke %"class.llvm::PointerType"* @_ZN4llvm13IRBuilderBase12getInt8PtrTyEj(%"class.llvm::IRBuilderBase"* %289, i32 0)
          to label %291 unwind label %390

; <label>:291:                                    ; preds = %284
  %292 = invoke %"class.llvm::ConstantPointerNull"* @_ZN4llvm19ConstantPointerNull3getEPNS_11PointerTypeE(%"class.llvm::PointerType"* %290)
          to label %293 unwind label %390

; <label>:293:                                    ; preds = %291
  %294 = bitcast %"class.llvm::ConstantPointerNull"* %292 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %294, %"class.llvm::Value"** %287, align 8
  %295 = getelementptr inbounds %"class.llvm::Value"*, %"class.llvm::Value"** %287, i64 1
  %296 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %297 = bitcast %"class.llvm::IRBuilder"* %296 to %"class.llvm::IRBuilderBase"*
  %298 = invoke %"class.llvm::IntegerType"* @_ZN4llvm13IRBuilderBase10getInt64TyEv(%"class.llvm::IRBuilderBase"* %297)
          to label %299 unwind label %390

; <label>:299:                                    ; preds = %293
  %300 = invoke %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt3getEPNS_11IntegerTypeEyb(%"class.llvm::IntegerType"* %298, i64 0, i1 zeroext false)
          to label %301 unwind label %390

; <label>:301:                                    ; preds = %299
  %302 = bitcast %"class.llvm::ConstantInt"* %300 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %302, %"class.llvm::Value"** %295, align 8
  %303 = getelementptr inbounds %"class.llvm::Value"*, %"class.llvm::Value"** %295, i64 1
  %304 = load %"class.llvm::Value"*, %"class.llvm::Value"** %125, align 8
  store %"class.llvm::Value"* %304, %"class.llvm::Value"** %303, align 8
  %305 = getelementptr inbounds %"class.llvm::Value"*, %"class.llvm::Value"** %303, i64 1
  %306 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  store %"class.llvm::Value"* %306, %"class.llvm::Value"** %305, align 8
  %307 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %130, i32 0, i32 0
  %308 = getelementptr inbounds [4 x %"class.llvm::Value"*], [4 x %"class.llvm::Value"*]* %131, i64 0, i64 0
  store %"class.llvm::Value"** %308, %"class.llvm::Value"*** %307, align 8
  %309 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %130, i32 0, i32 1
  store i64 4, i64* %309, align 8
  invoke void @_ZN4llvm8ArrayRefIPNS_5ValueEEC1ERKSt16initializer_listIS2_E(%"class.llvm::ArrayRef.211"* %129, %"class.std::initializer_list"* dereferenceable(16) %130)
          to label %310 unwind label %390

; <label>:310:                                    ; preds = %301
  invoke void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %132, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
          to label %311 unwind label %390

; <label>:311:                                    ; preds = %310
  %312 = bitcast %"class.llvm::ArrayRef.211"* %129 to { %"class.llvm::Value"**, i64 }*
  %313 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %312, i32 0, i32 0
  %314 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %313, align 8
  %315 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %312, i32 0, i32 1
  %316 = load i64, i64* %315, align 8
  %317 = invoke %"class.llvm::CallInst"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCallEPNS_5ValueENS_8ArrayRefIS5_EERKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"* %263, %"class.llvm::Value"* %286, %"class.llvm::Value"** %314, i64 %316, %"class.llvm::Twine"* dereferenceable(24) %132, %"class.llvm::MDNode"* null)
          to label %318 unwind label %390

; <label>:318:                                    ; preds = %311
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %128) #14
  store %"class.llvm::CallInst"* %317, %"class.llvm::CallInst"** %127, align 8
  %319 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %320 = bitcast %"class.llvm::IRBuilder"* %319 to %"class.llvm::IRBuilderBase"*
  %321 = invoke %"class.llvm::IntegerType"* @_ZN4llvm13IRBuilderBase9getInt8TyEv(%"class.llvm::IRBuilderBase"* %320)
          to label %322 unwind label %240

; <label>:322:                                    ; preds = %318
  %323 = bitcast %"class.llvm::IntegerType"* %321 to %"class.llvm::Type"*
  store %"class.std::__1::basic_string"* %134, %"class.std::__1::basic_string"** %73, align 8
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0), i8** %74, align 8
  %324 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %73, align 8
  %325 = load i8*, i8** %74, align 8
  store %"class.std::__1::basic_string"* %324, %"class.std::__1::basic_string"** %71, align 8
  store i8* %325, i8** %72, align 8
  %326 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %71, align 8
  %327 = bitcast %"class.std::__1::basic_string"* %326 to %"class.std::__1::__basic_string_common"*
  %328 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %326, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %328, %"class.std::__1::__compressed_pair"** %70, align 8
  %329 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %70, align 8
  store %"class.std::__1::__compressed_pair"* %329, %"class.std::__1::__compressed_pair"** %69, align 8
  %330 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %69, align 8
  %331 = bitcast %"class.std::__1::__compressed_pair"* %330 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %331, %"class.std::__1::__libcpp_compressed_pair_imp"** %68, align 8
  %332 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %68, align 8
  %333 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %332 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %333, %"class.std::__1::allocator"** %67, align 8
  %334 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %67, align 8
  %335 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %332, i32 0, i32 0
  %336 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %335 to i8*
  call void @llvm.memset.p0i8.i64(i8* %336, i8 0, i64 24, i32 8, i1 false) #14
  %337 = load i8*, i8** %72, align 8
  %338 = load i8*, i8** %72, align 8
  %339 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %338)
          to label %340 unwind label %240

; <label>:340:                                    ; preds = %322
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %326, i8* %337, i64 %339)
          to label %341 unwind label %240

; <label>:341:                                    ; preds = %340
  br label %342

; <label>:342:                                    ; preds = %341
  %343 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %127, align 8
  %344 = bitcast %"class.llvm::CallInst"* %343 to %"class.llvm::Value"*
  %345 = invoke %"class.llvm::Value"* @_ZN8CGMemory12CreateAllocaEPN4llvm4TypeENSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEPNS0_5ValueEtbPNS0_10BasicBlockE(%"class.llvm::Type"* %323, %"class.std::__1::basic_string"* %134, %"class.llvm::Value"* %344, i16 zeroext 0, i1 zeroext false, %"class.llvm::BasicBlock"* null)
          to label %346 unwind label %394

; <label>:346:                                    ; preds = %342
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %134) #14
  store %"class.llvm::Value"* %345, %"class.llvm::Value"** %133, align 8
  %347 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  store %"class.std::__1::basic_string"* %135, %"class.std::__1::basic_string"** %65, align 8
  store i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.5, i32 0, i32 0), i8** %66, align 8
  %348 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8
  %349 = load i8*, i8** %66, align 8
  store %"class.std::__1::basic_string"* %348, %"class.std::__1::basic_string"** %63, align 8
  store i8* %349, i8** %64, align 8
  %350 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8
  %351 = bitcast %"class.std::__1::basic_string"* %350 to %"class.std::__1::__basic_string_common"*
  %352 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %350, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %352, %"class.std::__1::__compressed_pair"** %62, align 8
  %353 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %62, align 8
  store %"class.std::__1::__compressed_pair"* %353, %"class.std::__1::__compressed_pair"** %61, align 8
  %354 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8
  %355 = bitcast %"class.std::__1::__compressed_pair"* %354 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %355, %"class.std::__1::__libcpp_compressed_pair_imp"** %60, align 8
  %356 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %60, align 8
  %357 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %356 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %357, %"class.std::__1::allocator"** %59, align 8
  %358 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %59, align 8
  %359 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %356, i32 0, i32 0
  %360 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %359 to i8*
  call void @llvm.memset.p0i8.i64(i8* %360, i8 0, i64 24, i32 8, i1 false) #14
  %361 = load i8*, i8** %64, align 8
  %362 = load i8*, i8** %64, align 8
  %363 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %362)
          to label %364 unwind label %240

; <label>:364:                                    ; preds = %346
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %350, i8* %361, i64 %363)
          to label %365 unwind label %240

; <label>:365:                                    ; preds = %364
  br label %366

; <label>:366:                                    ; preds = %365
  %367 = invoke dereferenceable(8) %"class.llvm::Constant"** @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantENS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S9_EEEEEixEOS6_(%"class.std::__1::unordered_map.212"* @_ZN14CodeGenVisitor9FunctionsE, %"class.std::__1::basic_string"* dereferenceable(24) %135)
          to label %368 unwind label %398

; <label>:368:                                    ; preds = %366
  %369 = load %"class.llvm::Constant"*, %"class.llvm::Constant"** %367, align 8
  %370 = bitcast %"class.llvm::Constant"* %369 to %"class.llvm::Value"*
  %371 = getelementptr inbounds [3 x %"class.llvm::Value"*], [3 x %"class.llvm::Value"*]* %138, i64 0, i64 0
  %372 = load %"class.llvm::Value"*, %"class.llvm::Value"** %133, align 8
  store %"class.llvm::Value"* %372, %"class.llvm::Value"** %371, align 8
  %373 = getelementptr inbounds %"class.llvm::Value"*, %"class.llvm::Value"** %371, i64 1
  %374 = load %"class.llvm::Value"*, %"class.llvm::Value"** %125, align 8
  store %"class.llvm::Value"* %374, %"class.llvm::Value"** %373, align 8
  %375 = getelementptr inbounds %"class.llvm::Value"*, %"class.llvm::Value"** %373, i64 1
  %376 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  store %"class.llvm::Value"* %376, %"class.llvm::Value"** %375, align 8
  %377 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %137, i32 0, i32 0
  %378 = getelementptr inbounds [3 x %"class.llvm::Value"*], [3 x %"class.llvm::Value"*]* %138, i64 0, i64 0
  store %"class.llvm::Value"** %378, %"class.llvm::Value"*** %377, align 8
  %379 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %137, i32 0, i32 1
  store i64 3, i64* %379, align 8
  invoke void @_ZN4llvm8ArrayRefIPNS_5ValueEEC1ERKSt16initializer_listIS2_E(%"class.llvm::ArrayRef.211"* %136, %"class.std::initializer_list"* dereferenceable(16) %137)
          to label %380 unwind label %398

; <label>:380:                                    ; preds = %368
  invoke void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %139, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
          to label %381 unwind label %398

; <label>:381:                                    ; preds = %380
  %382 = bitcast %"class.llvm::ArrayRef.211"* %136 to { %"class.llvm::Value"**, i64 }*
  %383 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %382, i32 0, i32 0
  %384 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %383, align 8
  %385 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %382, i32 0, i32 1
  %386 = load i64, i64* %385, align 8
  %387 = invoke %"class.llvm::CallInst"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCallEPNS_5ValueENS_8ArrayRefIS5_EERKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"* %347, %"class.llvm::Value"* %370, %"class.llvm::Value"** %384, i64 %386, %"class.llvm::Twine"* dereferenceable(24) %139, %"class.llvm::MDNode"* null)
          to label %388 unwind label %398

; <label>:388:                                    ; preds = %381
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %135) #14
  %389 = load %"class.llvm::Value"*, %"class.llvm::Value"** %133, align 8
  store %"class.llvm::Value"* %389, %"class.llvm::Value"** %113, align 8
  store i32 1, i32* %124, align 4
  br label %402

; <label>:390:                                    ; preds = %311, %310, %301, %299, %293, %291, %284, %282
  %391 = landingpad { i8*, i32 }
          cleanup
  %392 = extractvalue { i8*, i32 } %391, 0
  store i8* %392, i8** %122, align 8
  %393 = extractvalue { i8*, i32 } %391, 1
  store i32 %393, i32* %123, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %128) #14
  br label %403

; <label>:394:                                    ; preds = %342
  %395 = landingpad { i8*, i32 }
          cleanup
  %396 = extractvalue { i8*, i32 } %395, 0
  store i8* %396, i8** %122, align 8
  %397 = extractvalue { i8*, i32 } %395, 1
  store i32 %397, i32* %123, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %134) #14
  br label %403

; <label>:398:                                    ; preds = %381, %380, %368, %366
  %399 = landingpad { i8*, i32 }
          cleanup
  %400 = extractvalue { i8*, i32 } %399, 0
  store i8* %400, i8** %122, align 8
  %401 = extractvalue { i8*, i32 } %399, 1
  store i32 %401, i32* %123, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %135) #14
  br label %403

; <label>:402:                                    ; preds = %388, %256
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %121) #14
  br label %770

; <label>:403:                                    ; preds = %398, %394, %390, %240
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %121) #14
  br label %790

; <label>:404:                                    ; preds = %192
  %405 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %114, align 8
  %406 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %405, i32 0, i32 0
  %407 = load i32, i32* %406, align 8
  switch i32 %407, label %690 [
    i32 0, label %408
    i32 1, label %468
    i32 2, label %509
    i32 3, label %550
    i32 5, label %591
    i32 6, label %632
    i32 9, label %679
  ]

; <label>:408:                                    ; preds = %404
  %409 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %410 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %409, i32 0, i32 0
  %411 = load i32, i32* %410, align 8
  switch i32 %411, label %466 [
    i32 0, label %412
    i32 2, label %454
    i32 3, label %454
    i32 5, label %460
    i32 6, label %460
  ]

; <label>:412:                                    ; preds = %408
  %413 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %414 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %413)
  store %"class.llvm::Type"* %414, %"class.llvm::Type"** %140, align 8
  %415 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %114, align 8
  %416 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %415, i32 0, i32 8
  %417 = load i32, i32* %416, align 8
  %418 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %419 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %418, i32 0, i32 8
  %420 = load i32, i32* %419, align 8
  %421 = icmp slt i32 %417, %420
  br i1 %421, label %422, label %427

; <label>:422:                                    ; preds = %412
  %423 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %424 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %425 = load %"class.llvm::Type"*, %"class.llvm::Type"** %140, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %141, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %426 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateSExtEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %423, %"class.llvm::Value"* %424, %"class.llvm::Type"* %425, %"class.llvm::Twine"* dereferenceable(24) %141)
  br label %444

; <label>:427:                                    ; preds = %412
  %428 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %114, align 8
  %429 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %428, i32 0, i32 8
  %430 = load i32, i32* %429, align 8
  %431 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %432 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %431, i32 0, i32 8
  %433 = load i32, i32* %432, align 8
  %434 = icmp sgt i32 %430, %433
  br i1 %434, label %435, label %440

; <label>:435:                                    ; preds = %427
  %436 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %437 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %438 = load %"class.llvm::Type"*, %"class.llvm::Type"** %140, align 8
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %142, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %439 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE11CreateTruncEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %436, %"class.llvm::Value"* %437, %"class.llvm::Type"* %438, %"class.llvm::Twine"* dereferenceable(24) %142)
  br label %442

; <label>:440:                                    ; preds = %427
  %441 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  br label %442

; <label>:442:                                    ; preds = %440, %435
  %443 = phi %"class.llvm::Value"* [ %439, %435 ], [ %441, %440 ]
  br label %444

; <label>:444:                                    ; preds = %442, %422
  %445 = phi %"class.llvm::Value"* [ %426, %422 ], [ %443, %442 ]
  store %"class.llvm::Value"* %445, %"class.llvm::Value"** %116, align 8
  %446 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %447 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %446, i32 0, i32 9
  %448 = load i8, i8* %447, align 4
  %449 = trunc i8 %448 to i1
  %450 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %114, align 8
  %451 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %450, i32 0, i32 9
  %452 = zext i1 %449 to i8
  store i8 %452, i8* %451, align 4
  %453 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  store %"class.llvm::Value"* %453, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:454:                                    ; preds = %408, %408
  %455 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %456 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %457 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %458 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %457)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %143, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %459 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE12CreateSIToFPEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %455, %"class.llvm::Value"* %456, %"class.llvm::Type"* %458, %"class.llvm::Twine"* dereferenceable(24) %143)
  store %"class.llvm::Value"* %459, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:460:                                    ; preds = %408, %408
  %461 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %462 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %463 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %464 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %463)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %144, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %465 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE11CreateTruncEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %461, %"class.llvm::Value"* %462, %"class.llvm::Type"* %464, %"class.llvm::Twine"* dereferenceable(24) %144)
  store %"class.llvm::Value"* %465, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:466:                                    ; preds = %408
  br label %467

; <label>:467:                                    ; preds = %466
  br label %730

; <label>:468:                                    ; preds = %404
  %469 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %470 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %469, i32 0, i32 0
  %471 = load i32, i32* %470, align 8
  switch i32 %471, label %507 [
    i32 1, label %472
    i32 2, label %495
    i32 3, label %495
    i32 5, label %501
    i32 6, label %501
    i32 0, label %501
  ]

; <label>:472:                                    ; preds = %468
  store %"class.std::__1::basic_string"* %145, %"class.std::__1::basic_string"** %57, align 8
  store i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.6, i32 0, i32 0), i8** %58, align 8
  %473 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %57, align 8
  %474 = load i8*, i8** %58, align 8
  store %"class.std::__1::basic_string"* %473, %"class.std::__1::basic_string"** %55, align 8
  store i8* %474, i8** %56, align 8
  %475 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %55, align 8
  %476 = bitcast %"class.std::__1::basic_string"* %475 to %"class.std::__1::__basic_string_common"*
  %477 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %475, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %477, %"class.std::__1::__compressed_pair"** %54, align 8
  %478 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %54, align 8
  store %"class.std::__1::__compressed_pair"* %478, %"class.std::__1::__compressed_pair"** %53, align 8
  %479 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %53, align 8
  %480 = bitcast %"class.std::__1::__compressed_pair"* %479 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %480, %"class.std::__1::__libcpp_compressed_pair_imp"** %52, align 8
  %481 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %52, align 8
  %482 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %481 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %482, %"class.std::__1::allocator"** %51, align 8
  %483 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %51, align 8
  %484 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %481, i32 0, i32 0
  %485 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %484 to i8*
  call void @llvm.memset.p0i8.i64(i8* %485, i8 0, i64 24, i32 8, i1 false) #14
  %486 = load i8*, i8** %56, align 8
  %487 = load i8*, i8** %56, align 8
  %488 = call i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %487)
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %475, i8* %486, i64 %488)
  invoke void @_ZN7Warning5issueENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEEP7AstNode12WarningLevel(%"class.std::__1::basic_string"* %145, %class.AstNode* null, i32 1)
          to label %489 unwind label %491

; <label>:489:                                    ; preds = %472
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %145) #14
  %490 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  store %"class.llvm::Value"* %490, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:491:                                    ; preds = %472
  %492 = landingpad { i8*, i32 }
          cleanup
  %493 = extractvalue { i8*, i32 } %492, 0
  store i8* %493, i8** %122, align 8
  %494 = extractvalue { i8*, i32 } %492, 1
  store i32 %494, i32* %123, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %145) #14
  br label %790

; <label>:495:                                    ; preds = %468, %468
  %496 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %497 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %498 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %499 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %498)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %146, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %500 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE12CreateSIToFPEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %496, %"class.llvm::Value"* %497, %"class.llvm::Type"* %499, %"class.llvm::Twine"* dereferenceable(24) %146)
  store %"class.llvm::Value"* %500, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:501:                                    ; preds = %468, %468, %468
  %502 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %503 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %504 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %505 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %504)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %147, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %506 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE11CreateTruncEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %502, %"class.llvm::Value"* %503, %"class.llvm::Type"* %505, %"class.llvm::Twine"* dereferenceable(24) %147)
  store %"class.llvm::Value"* %506, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:507:                                    ; preds = %468
  br label %508

; <label>:508:                                    ; preds = %507
  br label %730

; <label>:509:                                    ; preds = %404
  %510 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %511 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %510, i32 0, i32 0
  %512 = load i32, i32* %511, align 8
  switch i32 %512, label %548 [
    i32 2, label %513
    i32 1, label %536
    i32 0, label %536
    i32 6, label %536
    i32 5, label %536
    i32 3, label %542
  ]

; <label>:513:                                    ; preds = %509
  store %"class.std::__1::basic_string"* %148, %"class.std::__1::basic_string"** %49, align 8
  store i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.7, i32 0, i32 0), i8** %50, align 8
  %514 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8
  %515 = load i8*, i8** %50, align 8
  store %"class.std::__1::basic_string"* %514, %"class.std::__1::basic_string"** %47, align 8
  store i8* %515, i8** %48, align 8
  %516 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %47, align 8
  %517 = bitcast %"class.std::__1::basic_string"* %516 to %"class.std::__1::__basic_string_common"*
  %518 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %516, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %518, %"class.std::__1::__compressed_pair"** %46, align 8
  %519 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %46, align 8
  store %"class.std::__1::__compressed_pair"* %519, %"class.std::__1::__compressed_pair"** %45, align 8
  %520 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %45, align 8
  %521 = bitcast %"class.std::__1::__compressed_pair"* %520 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %521, %"class.std::__1::__libcpp_compressed_pair_imp"** %44, align 8
  %522 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %44, align 8
  %523 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %522 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %523, %"class.std::__1::allocator"** %43, align 8
  %524 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %43, align 8
  %525 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %522, i32 0, i32 0
  %526 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %525 to i8*
  call void @llvm.memset.p0i8.i64(i8* %526, i8 0, i64 24, i32 8, i1 false) #14
  %527 = load i8*, i8** %48, align 8
  %528 = load i8*, i8** %48, align 8
  %529 = call i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %528)
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %516, i8* %527, i64 %529)
  invoke void @_ZN7Warning5issueENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEEP7AstNode12WarningLevel(%"class.std::__1::basic_string"* %148, %class.AstNode* null, i32 1)
          to label %530 unwind label %532

; <label>:530:                                    ; preds = %513
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %148) #14
  %531 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  store %"class.llvm::Value"* %531, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:532:                                    ; preds = %513
  %533 = landingpad { i8*, i32 }
          cleanup
  %534 = extractvalue { i8*, i32 } %533, 0
  store i8* %534, i8** %122, align 8
  %535 = extractvalue { i8*, i32 } %533, 1
  store i32 %535, i32* %123, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %148) #14
  br label %790

; <label>:536:                                    ; preds = %509, %509, %509, %509
  %537 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %538 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %539 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %540 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %539)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %149, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %541 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE12CreateFPToSIEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %537, %"class.llvm::Value"* %538, %"class.llvm::Type"* %540, %"class.llvm::Twine"* dereferenceable(24) %149)
  store %"class.llvm::Value"* %541, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:542:                                    ; preds = %509
  %543 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %544 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %545 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %546 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %545)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %150, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %547 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE11CreateFPExtEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %543, %"class.llvm::Value"* %544, %"class.llvm::Type"* %546, %"class.llvm::Twine"* dereferenceable(24) %150)
  store %"class.llvm::Value"* %547, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:548:                                    ; preds = %509
  br label %549

; <label>:549:                                    ; preds = %548
  br label %730

; <label>:550:                                    ; preds = %404
  %551 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %552 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %551, i32 0, i32 0
  %553 = load i32, i32* %552, align 8
  switch i32 %553, label %589 [
    i32 3, label %554
    i32 2, label %577
    i32 1, label %583
    i32 0, label %583
    i32 6, label %583
    i32 5, label %583
  ]

; <label>:554:                                    ; preds = %550
  store %"class.std::__1::basic_string"* %151, %"class.std::__1::basic_string"** %41, align 8
  store i8* getelementptr inbounds ([30 x i8], [30 x i8]* @.str.8, i32 0, i32 0), i8** %42, align 8
  %555 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8
  %556 = load i8*, i8** %42, align 8
  store %"class.std::__1::basic_string"* %555, %"class.std::__1::basic_string"** %39, align 8
  store i8* %556, i8** %40, align 8
  %557 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8
  %558 = bitcast %"class.std::__1::basic_string"* %557 to %"class.std::__1::__basic_string_common"*
  %559 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %557, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %559, %"class.std::__1::__compressed_pair"** %38, align 8
  %560 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %38, align 8
  store %"class.std::__1::__compressed_pair"* %560, %"class.std::__1::__compressed_pair"** %37, align 8
  %561 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %37, align 8
  %562 = bitcast %"class.std::__1::__compressed_pair"* %561 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %562, %"class.std::__1::__libcpp_compressed_pair_imp"** %36, align 8
  %563 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %36, align 8
  %564 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %563 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %564, %"class.std::__1::allocator"** %35, align 8
  %565 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %35, align 8
  %566 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %563, i32 0, i32 0
  %567 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %566 to i8*
  call void @llvm.memset.p0i8.i64(i8* %567, i8 0, i64 24, i32 8, i1 false) #14
  %568 = load i8*, i8** %40, align 8
  %569 = load i8*, i8** %40, align 8
  %570 = call i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %569)
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %557, i8* %568, i64 %570)
  invoke void @_ZN7Warning5issueENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEEP7AstNode12WarningLevel(%"class.std::__1::basic_string"* %151, %class.AstNode* null, i32 1)
          to label %571 unwind label %573

; <label>:571:                                    ; preds = %554
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %151) #14
  %572 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  store %"class.llvm::Value"* %572, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:573:                                    ; preds = %554
  %574 = landingpad { i8*, i32 }
          cleanup
  %575 = extractvalue { i8*, i32 } %574, 0
  store i8* %575, i8** %122, align 8
  %576 = extractvalue { i8*, i32 } %574, 1
  store i32 %576, i32* %123, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %151) #14
  br label %790

; <label>:577:                                    ; preds = %550
  %578 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %579 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %580 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %581 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %580)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %152, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %582 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE13CreateFPTruncEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %578, %"class.llvm::Value"* %579, %"class.llvm::Type"* %581, %"class.llvm::Twine"* dereferenceable(24) %152)
  store %"class.llvm::Value"* %582, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:583:                                    ; preds = %550, %550, %550, %550
  %584 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %585 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %586 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %587 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %586)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %153, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %588 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE12CreateFPToSIEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %584, %"class.llvm::Value"* %585, %"class.llvm::Type"* %587, %"class.llvm::Twine"* dereferenceable(24) %153)
  store %"class.llvm::Value"* %588, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:589:                                    ; preds = %550
  br label %590

; <label>:590:                                    ; preds = %589
  br label %730

; <label>:591:                                    ; preds = %404
  %592 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %593 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %592, i32 0, i32 0
  %594 = load i32, i32* %593, align 8
  switch i32 %594, label %630 [
    i32 5, label %595
    i32 2, label %618
    i32 3, label %618
    i32 0, label %624
    i32 1, label %624
    i32 6, label %624
  ]

; <label>:595:                                    ; preds = %591
  store %"class.std::__1::basic_string"* %154, %"class.std::__1::basic_string"** %33, align 8
  store i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.9, i32 0, i32 0), i8** %34, align 8
  %596 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8
  %597 = load i8*, i8** %34, align 8
  store %"class.std::__1::basic_string"* %596, %"class.std::__1::basic_string"** %31, align 8
  store i8* %597, i8** %32, align 8
  %598 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %31, align 8
  %599 = bitcast %"class.std::__1::basic_string"* %598 to %"class.std::__1::__basic_string_common"*
  %600 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %598, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %600, %"class.std::__1::__compressed_pair"** %30, align 8
  %601 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %30, align 8
  store %"class.std::__1::__compressed_pair"* %601, %"class.std::__1::__compressed_pair"** %29, align 8
  %602 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %29, align 8
  %603 = bitcast %"class.std::__1::__compressed_pair"* %602 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %603, %"class.std::__1::__libcpp_compressed_pair_imp"** %28, align 8
  %604 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %28, align 8
  %605 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %604 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %605, %"class.std::__1::allocator"** %27, align 8
  %606 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %27, align 8
  %607 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %604, i32 0, i32 0
  %608 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %607 to i8*
  call void @llvm.memset.p0i8.i64(i8* %608, i8 0, i64 24, i32 8, i1 false) #14
  %609 = load i8*, i8** %32, align 8
  %610 = load i8*, i8** %32, align 8
  %611 = call i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %610)
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %598, i8* %609, i64 %611)
  invoke void @_ZN7Warning5issueENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEEP7AstNode12WarningLevel(%"class.std::__1::basic_string"* %154, %class.AstNode* null, i32 1)
          to label %612 unwind label %614

; <label>:612:                                    ; preds = %595
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %154) #14
  %613 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  store %"class.llvm::Value"* %613, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:614:                                    ; preds = %595
  %615 = landingpad { i8*, i32 }
          cleanup
  %616 = extractvalue { i8*, i32 } %615, 0
  store i8* %616, i8** %122, align 8
  %617 = extractvalue { i8*, i32 } %615, 1
  store i32 %617, i32* %123, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %154) #14
  br label %790

; <label>:618:                                    ; preds = %591, %591
  %619 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %620 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %621 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %622 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %621)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %155, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %623 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE12CreateSIToFPEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %619, %"class.llvm::Value"* %620, %"class.llvm::Type"* %622, %"class.llvm::Twine"* dereferenceable(24) %155)
  store %"class.llvm::Value"* %623, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:624:                                    ; preds = %591, %591, %591
  %625 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %626 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %627 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %628 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %627)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %156, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %629 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateSExtEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %625, %"class.llvm::Value"* %626, %"class.llvm::Type"* %628, %"class.llvm::Twine"* dereferenceable(24) %156)
  store %"class.llvm::Value"* %629, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:630:                                    ; preds = %591
  br label %631

; <label>:631:                                    ; preds = %630
  br label %730

; <label>:632:                                    ; preds = %404
  %633 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %634 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %633, i32 0, i32 0
  %635 = load i32, i32* %634, align 8
  switch i32 %635, label %677 [
    i32 6, label %636
    i32 5, label %659
    i32 0, label %665
    i32 1, label %665
    i32 2, label %671
    i32 3, label %671
  ]

; <label>:636:                                    ; preds = %632
  store %"class.std::__1::basic_string"* %157, %"class.std::__1::basic_string"** %25, align 8
  store i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.10, i32 0, i32 0), i8** %26, align 8
  %637 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %25, align 8
  %638 = load i8*, i8** %26, align 8
  store %"class.std::__1::basic_string"* %637, %"class.std::__1::basic_string"** %23, align 8
  store i8* %638, i8** %24, align 8
  %639 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8
  %640 = bitcast %"class.std::__1::basic_string"* %639 to %"class.std::__1::__basic_string_common"*
  %641 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %639, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %641, %"class.std::__1::__compressed_pair"** %22, align 8
  %642 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8
  store %"class.std::__1::__compressed_pair"* %642, %"class.std::__1::__compressed_pair"** %21, align 8
  %643 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %21, align 8
  %644 = bitcast %"class.std::__1::__compressed_pair"* %643 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %644, %"class.std::__1::__libcpp_compressed_pair_imp"** %20, align 8
  %645 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %20, align 8
  %646 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %645 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %646, %"class.std::__1::allocator"** %19, align 8
  %647 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %19, align 8
  %648 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %645, i32 0, i32 0
  %649 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %648 to i8*
  call void @llvm.memset.p0i8.i64(i8* %649, i8 0, i64 24, i32 8, i1 false) #14
  %650 = load i8*, i8** %24, align 8
  %651 = load i8*, i8** %24, align 8
  %652 = call i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %651)
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %639, i8* %650, i64 %652)
  invoke void @_ZN7Warning5issueENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEEP7AstNode12WarningLevel(%"class.std::__1::basic_string"* %157, %class.AstNode* null, i32 1)
          to label %653 unwind label %655

; <label>:653:                                    ; preds = %636
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %157) #14
  %654 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  store %"class.llvm::Value"* %654, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:655:                                    ; preds = %636
  %656 = landingpad { i8*, i32 }
          cleanup
  %657 = extractvalue { i8*, i32 } %656, 0
  store i8* %657, i8** %122, align 8
  %658 = extractvalue { i8*, i32 } %656, 1
  store i32 %658, i32* %123, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %157) #14
  br label %790

; <label>:659:                                    ; preds = %632
  %660 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %661 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %662 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %663 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %662)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %158, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %664 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE11CreateTruncEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %660, %"class.llvm::Value"* %661, %"class.llvm::Type"* %663, %"class.llvm::Twine"* dereferenceable(24) %158)
  store %"class.llvm::Value"* %664, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:665:                                    ; preds = %632, %632
  %666 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %667 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %668 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %669 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %668)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %159, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %670 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateSExtEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %666, %"class.llvm::Value"* %667, %"class.llvm::Type"* %669, %"class.llvm::Twine"* dereferenceable(24) %159)
  store %"class.llvm::Value"* %670, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:671:                                    ; preds = %632, %632
  %672 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %673 = load %"class.llvm::Value"*, %"class.llvm::Value"** %116, align 8
  %674 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %675 = call %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216) %674)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %160, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %676 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE12CreateSIToFPEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %672, %"class.llvm::Value"* %673, %"class.llvm::Type"* %675, %"class.llvm::Twine"* dereferenceable(24) %160)
  store %"class.llvm::Value"* %676, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:677:                                    ; preds = %632
  br label %678

; <label>:678:                                    ; preds = %677
  br label %730

; <label>:679:                                    ; preds = %404
  %680 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  %681 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %680, i32 0, i32 0
  %682 = load i32, i32* %681, align 8
  switch i32 %682, label %688 [
    i32 5, label %683
  ]

; <label>:683:                                    ; preds = %679
  %684 = load %"class.llvm::IRBuilder"*, %"class.llvm::IRBuilder"** %119, align 8
  %685 = bitcast %"class.llvm::IRBuilder"* %684 to %"class.llvm::IRBuilderBase"*
  %686 = call %"class.llvm::ConstantInt"* @_ZN4llvm13IRBuilderBase8getFalseEv(%"class.llvm::IRBuilderBase"* %685)
  %687 = bitcast %"class.llvm::ConstantInt"* %686 to %"class.llvm::Value"*
  store %"class.llvm::Value"* %687, %"class.llvm::Value"** %113, align 8
  br label %770

; <label>:688:                                    ; preds = %679
  br label %689

; <label>:689:                                    ; preds = %688
  br label %730

; <label>:690:                                    ; preds = %404
  %691 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  call void @_ZNK13TypeSpecifier9to_stringEv(%"class.std::__1::basic_string"* sret %162, %struct.TypeSpecifier* %691)
  store i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.11, i32 0, i32 0), i8** %17, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %162, %"class.std::__1::basic_string"** %18, align 8, !noalias !2
  %692 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %18, align 8, !noalias !2
  %693 = load i8*, i8** %17, align 8, !noalias !2
  %694 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %692, i64 0, i8* %693)
          to label %695 unwind label %772

; <label>:695:                                    ; preds = %690
  store %"class.std::__1::basic_string"* %694, %"class.std::__1::basic_string"** %16, align 8, !noalias !2
  %696 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %16, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %161, %"class.std::__1::basic_string"** %14, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %696, %"class.std::__1::basic_string"** %15, align 8, !noalias !2
  %697 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !2
  %698 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %15, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %697, %"class.std::__1::basic_string"** %12, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %698, %"class.std::__1::basic_string"** %13, align 8, !noalias !2
  %699 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !2
  %700 = bitcast %"class.std::__1::basic_string"* %699 to %"class.std::__1::__basic_string_common"*
  %701 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %699, i32 0, i32 0
  %702 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !2
  %703 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %702, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %703, %"class.std::__1::__compressed_pair"** %11, align 8, !noalias !2
  %704 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %11, align 8, !noalias !2
  %705 = bitcast %"class.std::__1::__compressed_pair"* %701 to i8*
  %706 = bitcast %"class.std::__1::__compressed_pair"* %704 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %705, i8* %706, i64 24, i32 8, i1 false) #14
  %707 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %707, %"class.std::__1::basic_string"** %8, align 8, !noalias !2
  %708 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %8, align 8, !noalias !2
  %709 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %708, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %709, %"class.std::__1::__compressed_pair"** %7, align 8, !noalias !2
  %710 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %7, align 8, !noalias !2
  %711 = bitcast %"class.std::__1::__compressed_pair"* %710 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %711, %"class.std::__1::__libcpp_compressed_pair_imp"** %6, align 8, !noalias !2
  %712 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %6, align 8, !noalias !2
  %713 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %712, i32 0, i32 0
  %714 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %713, i32 0, i32 0
  %715 = bitcast %union.anon* %714 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %716 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %715, i32 0, i32 0
  store [3 x i64]* %716, [3 x i64]** %9, align 8, !noalias !2
  store i32 0, i32* %10, align 4, !noalias !2
  br label %717

; <label>:717:                                    ; preds = %720, %695
  %718 = load i32, i32* %10, align 4, !noalias !2
  %719 = icmp ult i32 %718, 3
  br i1 %719, label %720, label %727

; <label>:720:                                    ; preds = %717
  %721 = load i32, i32* %10, align 4, !noalias !2
  %722 = zext i32 %721 to i64
  %723 = load [3 x i64]*, [3 x i64]** %9, align 8, !noalias !2
  %724 = getelementptr inbounds [3 x i64], [3 x i64]* %723, i64 0, i64 %722
  store i64 0, i64* %724, align 8
  %725 = load i32, i32* %10, align 4, !noalias !2
  %726 = add i32 %725, 1
  store i32 %726, i32* %10, align 4, !noalias !2
  br label %717

; <label>:727:                                    ; preds = %717
  br label %728

; <label>:728:                                    ; preds = %727
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 7, %"class.std::__1::basic_string"* %161, %class.AstNode* null)
          to label %729 unwind label %776

; <label>:729:                                    ; preds = %728
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %161) #14
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %162) #14
  br label %730

; <label>:730:                                    ; preds = %729, %689, %678, %631, %590, %549, %508, %467
  %731 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %115, align 8
  call void @_ZNK13TypeSpecifier9to_stringEv(%"class.std::__1::basic_string"* sret %164, %struct.TypeSpecifier* %731)
  store i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.11, i32 0, i32 0), i8** %100, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %164, %"class.std::__1::basic_string"** %101, align 8, !noalias !5
  %732 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %101, align 8, !noalias !5
  %733 = load i8*, i8** %100, align 8, !noalias !5
  %734 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %732, i64 0, i8* %733)
          to label %735 unwind label %781

; <label>:735:                                    ; preds = %730
  store %"class.std::__1::basic_string"* %734, %"class.std::__1::basic_string"** %99, align 8, !noalias !5
  %736 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %99, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %163, %"class.std::__1::basic_string"** %97, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %736, %"class.std::__1::basic_string"** %98, align 8, !noalias !5
  %737 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %97, align 8, !noalias !5
  %738 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %98, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %737, %"class.std::__1::basic_string"** %95, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %738, %"class.std::__1::basic_string"** %96, align 8, !noalias !5
  %739 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %95, align 8, !noalias !5
  %740 = bitcast %"class.std::__1::basic_string"* %739 to %"class.std::__1::__basic_string_common"*
  %741 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %739, i32 0, i32 0
  %742 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %96, align 8, !noalias !5
  %743 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %742, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %743, %"class.std::__1::__compressed_pair"** %94, align 8, !noalias !5
  %744 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %94, align 8, !noalias !5
  %745 = bitcast %"class.std::__1::__compressed_pair"* %741 to i8*
  %746 = bitcast %"class.std::__1::__compressed_pair"* %744 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %745, i8* %746, i64 24, i32 8, i1 false) #14
  %747 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %96, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %747, %"class.std::__1::basic_string"** %91, align 8, !noalias !5
  %748 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %91, align 8, !noalias !5
  %749 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %748, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %749, %"class.std::__1::__compressed_pair"** %90, align 8, !noalias !5
  %750 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %90, align 8, !noalias !5
  %751 = bitcast %"class.std::__1::__compressed_pair"* %750 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %751, %"class.std::__1::__libcpp_compressed_pair_imp"** %89, align 8, !noalias !5
  %752 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %89, align 8, !noalias !5
  %753 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %752, i32 0, i32 0
  %754 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %753, i32 0, i32 0
  %755 = bitcast %union.anon* %754 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %756 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %755, i32 0, i32 0
  store [3 x i64]* %756, [3 x i64]** %92, align 8, !noalias !5
  store i32 0, i32* %93, align 4, !noalias !5
  br label %757

; <label>:757:                                    ; preds = %760, %735
  %758 = load i32, i32* %93, align 4, !noalias !5
  %759 = icmp ult i32 %758, 3
  br i1 %759, label %760, label %767

; <label>:760:                                    ; preds = %757
  %761 = load i32, i32* %93, align 4, !noalias !5
  %762 = zext i32 %761 to i64
  %763 = load [3 x i64]*, [3 x i64]** %92, align 8, !noalias !5
  %764 = getelementptr inbounds [3 x i64], [3 x i64]* %763, i64 0, i64 %762
  store i64 0, i64* %764, align 8
  %765 = load i32, i32* %93, align 4, !noalias !5
  %766 = add i32 %765, 1
  store i32 %766, i32* %93, align 4, !noalias !5
  br label %757

; <label>:767:                                    ; preds = %757
  br label %768

; <label>:768:                                    ; preds = %767
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 7, %"class.std::__1::basic_string"* %163, %class.AstNode* null)
          to label %769 unwind label %785

; <label>:769:                                    ; preds = %768
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %163) #14
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %164) #14
  br label %770

; <label>:770:                                    ; preds = %769, %683, %671, %665, %659, %653, %624, %618, %612, %583, %577, %571, %542, %536, %530, %501, %495, %489, %460, %454, %444, %402, %190, %176
  %771 = load %"class.llvm::Value"*, %"class.llvm::Value"** %113, align 8
  ret %"class.llvm::Value"* %771

; <label>:772:                                    ; preds = %690
  %773 = landingpad { i8*, i32 }
          cleanup
  %774 = extractvalue { i8*, i32 } %773, 0
  store i8* %774, i8** %122, align 8
  %775 = extractvalue { i8*, i32 } %773, 1
  store i32 %775, i32* %123, align 4
  br label %780

; <label>:776:                                    ; preds = %728
  %777 = landingpad { i8*, i32 }
          cleanup
  %778 = extractvalue { i8*, i32 } %777, 0
  store i8* %778, i8** %122, align 8
  %779 = extractvalue { i8*, i32 } %777, 1
  store i32 %779, i32* %123, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %161) #14
  br label %780

; <label>:780:                                    ; preds = %776, %772
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %162) #14
  br label %790

; <label>:781:                                    ; preds = %730
  %782 = landingpad { i8*, i32 }
          cleanup
  %783 = extractvalue { i8*, i32 } %782, 0
  store i8* %783, i8** %122, align 8
  %784 = extractvalue { i8*, i32 } %782, 1
  store i32 %784, i32* %123, align 4
  br label %789

; <label>:785:                                    ; preds = %768
  %786 = landingpad { i8*, i32 }
          cleanup
  %787 = extractvalue { i8*, i32 } %786, 0
  store i8* %787, i8** %122, align 8
  %788 = extractvalue { i8*, i32 } %786, 1
  store i32 %788, i32* %123, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %163) #14
  br label %789

; <label>:789:                                    ; preds = %785, %781
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %164) #14
  br label %790

; <label>:790:                                    ; preds = %789, %780, %655, %614, %573, %532, %491, %403
  %791 = load i8*, i8** %122, align 8
  %792 = load i32, i32* %123, align 4
  %793 = insertvalue { i8*, i32 } undef, i8* %791, 0
  %794 = insertvalue { i8*, i32 } %793, i32 %792, 1
  resume { i8*, i32 } %794
}

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

declare %"class.llvm::Type"* @_ZN6CGType7getTypeER13TypeSpecifier(%struct.TypeSpecifier* dereferenceable(216)) #1

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

declare i32 @__gxx_personality_v0(...)

declare void @_ZN14CodeGenVisitor9GetStringERNSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEEbb(%struct.CGValue* sret, %class.CodeGenVisitor*, %"class.std::__1::basic_string"* dereferenceable(24), i1 zeroext, i1 zeroext) #1

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::CallInst"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCallEPNS_5ValueENS_8ArrayRefIS5_EERKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::MDNode"*) #0 align 2 {
  %7 = alloca %"class.llvm::ArrayRef.211", align 8
  %8 = alloca %"class.llvm::IRBuilder"*, align 8
  %9 = alloca %"class.llvm::Value"*, align 8
  %10 = alloca %"class.llvm::Twine"*, align 8
  %11 = alloca %"class.llvm::MDNode"*, align 8
  %12 = alloca %"class.llvm::PointerType"*, align 8
  %13 = alloca %"class.llvm::FunctionType"*, align 8
  %14 = alloca %"class.llvm::ArrayRef.211", align 8
  %15 = bitcast %"class.llvm::ArrayRef.211"* %7 to { %"class.llvm::Value"**, i64 }*
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
  %27 = bitcast %"class.llvm::ArrayRef.211"* %14 to i8*
  %28 = bitcast %"class.llvm::ArrayRef.211"* %7 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %27, i8* %28, i64 16, i32 8, i1 false)
  %29 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %10, align 8
  %30 = load %"class.llvm::MDNode"*, %"class.llvm::MDNode"** %11, align 8
  %31 = bitcast %"class.llvm::ArrayRef.211"* %14 to { %"class.llvm::Value"**, i64 }*
  %32 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %31, i32 0, i32 0
  %33 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %32, align 8
  %34 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %31, i32 0, i32 1
  %35 = load i64, i64* %34, align 8
  %36 = call %"class.llvm::CallInst"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCallEPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS7_EERKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"* %18, %"class.llvm::FunctionType"* %25, %"class.llvm::Value"* %26, %"class.llvm::Value"** %33, i64 %35, %"class.llvm::Twine"* dereferenceable(24) %29, %"class.llvm::MDNode"* %30)
  ret %"class.llvm::CallInst"* %36
}

; Function Attrs: ssp uwtable
define linkonce_odr dereferenceable(8) %"class.llvm::Constant"** @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantENS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S9_EEEEEixEOS6_(%"class.std::__1::unordered_map.212"*, %"class.std::__1::basic_string"* dereferenceable(24)) #0 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %3 = alloca %"class.std::__1::tuple.236"*, align 8
  %4 = alloca %"class.std::__1::tuple.236"*, align 8
  %5 = alloca %"class.std::__1::tuple.236", align 1
  %6 = alloca %"class.std::__1::basic_string"*, align 8
  %7 = alloca %"class.std::__1::__tuple_leaf"*, align 8
  %8 = alloca %"class.std::__1::basic_string"*, align 8
  %9 = alloca %"class.std::__1::basic_string"*, align 8
  %10 = alloca %"struct.std::__1::__tuple_indices", align 1
  %11 = alloca %"struct.std::__1::__tuple_types", align 1
  %12 = alloca %"struct.std::__1::__tuple_indices.243", align 1
  %13 = alloca %"struct.std::__1::__tuple_types.245", align 1
  %14 = alloca %"struct.std::__1::__tuple_impl"*, align 8
  %15 = alloca %"class.std::__1::basic_string"*, align 8
  %16 = alloca %"struct.std::__1::__tuple_indices", align 1
  %17 = alloca %"struct.std::__1::__tuple_types", align 1
  %18 = alloca %"struct.std::__1::__tuple_indices.243", align 1
  %19 = alloca %"struct.std::__1::__tuple_types.245", align 1
  %20 = alloca %"struct.std::__1::__tuple_impl"*, align 8
  %21 = alloca %"class.std::__1::basic_string"*, align 8
  %22 = alloca %"class.std::__1::basic_string"*, align 8
  %23 = alloca %"class.std::__1::tuple"*, align 8
  %24 = alloca %"class.std::__1::basic_string"*, align 8
  %25 = alloca %"struct.std::__1::__tuple_indices", align 1
  %26 = alloca %"struct.std::__1::__tuple_types", align 1
  %27 = alloca %"struct.std::__1::__tuple_indices.243", align 1
  %28 = alloca %"struct.std::__1::__tuple_types.245", align 1
  %29 = alloca %"class.std::__1::tuple"*, align 8
  %30 = alloca %"class.std::__1::basic_string"*, align 8
  %31 = alloca %"class.std::__1::basic_string"*, align 8
  %32 = alloca %"class.std::__1::tuple", align 8
  %33 = alloca %"class.std::__1::basic_string"*, align 8
  %34 = alloca i8*, align 8
  %35 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %36 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %37 = alloca %"class.std::__1::basic_string"*, align 8
  %38 = alloca i8*, align 8
  %39 = alloca i8*, align 8
  %40 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %41 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %42 = alloca %"class.std::__1::basic_string"*, align 8
  %43 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %44 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %45 = alloca %"class.std::__1::basic_string"*, align 8
  %46 = alloca %"class.std::__1::basic_string"*, align 8
  %47 = alloca %"class.std::__1::basic_string"*, align 8
  %48 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %49 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %50 = alloca %"class.std::__1::basic_string"*, align 8
  %51 = alloca i8*, align 8
  %52 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %53 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %54 = alloca %"class.std::__1::basic_string"*, align 8
  %55 = alloca i8*, align 8
  %56 = alloca i8*, align 8
  %57 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %58 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %59 = alloca %"class.std::__1::basic_string"*, align 8
  %60 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %61 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %62 = alloca %"class.std::__1::basic_string"*, align 8
  %63 = alloca %"class.std::__1::basic_string"*, align 8
  %64 = alloca %"class.std::__1::basic_string"*, align 8
  %65 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %66 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %67 = alloca %"class.std::__1::basic_string"*, align 8
  %68 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %69 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %70 = alloca %"class.std::__1::basic_string"*, align 8
  %71 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %72 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %73 = alloca %"class.std::__1::basic_string"*, align 8
  %74 = alloca %"class.std::__1::basic_string"*, align 8
  %75 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %76 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %77 = alloca %"class.std::__1::basic_string"*, align 8
  %78 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %79 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %80 = alloca %"class.std::__1::basic_string"*, align 8
  %81 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %82 = alloca %"class.std::__1::__compressed_pair"*, align 8
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
  %94 = alloca %"class.std::__1::__unordered_map_equal.232"*, align 8
  %95 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %96 = alloca %"class.std::__1::basic_string"*, align 8
  %97 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %98 = alloca %"class.std::__1::tuple"*, align 8
  %99 = alloca %"class.std::__1::tuple.236"*, align 8
  %100 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %101 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %102 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %103 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.228"*, align 8
  %104 = alloca %"class.std::__1::__compressed_pair.227"*, align 8
  %105 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %106 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.231"*, align 8
  %107 = alloca %"class.std::__1::__compressed_pair.230"*, align 8
  %108 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %109 = alloca i64, align 8
  %110 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.228"*, align 8
  %111 = alloca %"class.std::__1::__compressed_pair.227"*, align 8
  %112 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %113 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.231"*, align 8
  %114 = alloca %"class.std::__1::__compressed_pair.230"*, align 8
  %115 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %116 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.231"*, align 8
  %117 = alloca %"class.std::__1::__compressed_pair.230"*, align 8
  %118 = alloca %"class.std::__1::__hash_table.213"*, align 8
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
  %131 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.220"*, align 8
  %132 = alloca %"class.std::__1::__compressed_pair.219"*, align 8
  %133 = alloca %"class.std::__1::__bucket_list_deallocator.218"*, align 8
  %134 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %135 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %136 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %137 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %138 = alloca i64, align 8
  %139 = alloca i64, align 8
  %140 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %141 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %142 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %143 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %144 = alloca i64, align 8
  %145 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %146 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %147 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %148 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %149 = alloca i8*, align 8
  %150 = alloca %"class.std::__1::allocator.225"*, align 8
  %151 = alloca %"struct.std::__1::__hash_node"*, align 8
  %152 = alloca i64, align 8
  %153 = alloca %"class.std::__1::allocator.225"*, align 8
  %154 = alloca %"struct.std::__1::__hash_node"*, align 8
  %155 = alloca i64, align 8
  %156 = alloca %"class.std::__1::allocator.225"*, align 8
  %157 = alloca %"struct.std::__1::pair.237"*, align 8
  %158 = alloca %"class.std::__1::allocator.225"*, align 8
  %159 = alloca %"struct.std::__1::pair.237"*, align 8
  %160 = alloca %"struct.std::__1::integral_constant.244", align 1
  %161 = alloca %"struct.std::__1::__has_destroy", align 1
  %162 = alloca %"struct.std::__1::pair.237"*, align 8
  %163 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %164 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %165 = alloca %"struct.std::__1::__hash_node"*, align 8
  %166 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %167 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %168 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %169 = alloca %"struct.std::__1::__hash_node"*, align 8
  %170 = alloca %"struct.std::__1::__hash_node"*, align 8
  %171 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %172 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %173 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %174 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %175 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %176 = alloca i64, align 8
  %177 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.224"*, align 8
  %178 = alloca %"class.std::__1::__compressed_pair.223"*, align 8
  %179 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %180 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %181 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %182 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %183 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %184 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %185 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %186 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %187 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %188 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %189 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %190 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %191 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %192 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %193 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %194 = alloca i64, align 8
  %195 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %196 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %197 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %198 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %199 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %200 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %201 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %202 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %203 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %204 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %205 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %206 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %207 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %208 = alloca i64, align 8
  %209 = alloca i64, align 8
  %210 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %211 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %212 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %213 = alloca i64, align 8
  %214 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %215 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %216 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %217 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %218 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %219 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %220 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %221 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %222 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %223 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %224 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %225 = alloca %"struct.std::__1::__hash_node"*, align 8
  %226 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.228"*, align 8
  %227 = alloca %"class.std::__1::__compressed_pair.227"*, align 8
  %228 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %229 = alloca i64, align 8
  %230 = alloca i64, align 8
  %231 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.220"*, align 8
  %232 = alloca %"class.std::__1::__compressed_pair.219"*, align 8
  %233 = alloca %"class.std::__1::__bucket_list_deallocator.218"*, align 8
  %234 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %235 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %236 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %237 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %238 = alloca %"class.std::__1::__unordered_map_hasher.229"*, align 8
  %239 = alloca %"class.std::__1::basic_string"*, align 8
  %240 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %241 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %242 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %243 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %244 = alloca i8*, align 8
  %245 = alloca %"class.std::__1::allocator.225"*, align 8
  %246 = alloca %"struct.std::__1::__hash_node"*, align 8
  %247 = alloca i64, align 8
  %248 = alloca %"class.std::__1::allocator.225"*, align 8
  %249 = alloca %"struct.std::__1::__hash_node"*, align 8
  %250 = alloca i64, align 8
  %251 = alloca %"class.std::__1::allocator.225"*, align 8
  %252 = alloca %"struct.std::__1::pair.237"*, align 8
  %253 = alloca %"class.std::__1::allocator.225"*, align 8
  %254 = alloca %"struct.std::__1::pair.237"*, align 8
  %255 = alloca %"struct.std::__1::integral_constant.244", align 1
  %256 = alloca %"struct.std::__1::__has_destroy", align 1
  %257 = alloca %"struct.std::__1::pair.237"*, align 8
  %258 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %259 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %260 = alloca %"struct.std::__1::__hash_node"*, align 8
  %261 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %262 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %263 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %264 = alloca %"struct.std::__1::__hash_node"*, align 8
  %265 = alloca %"struct.std::__1::__hash_node"*, align 8
  %266 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %267 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %268 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %269 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %270 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %271 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %272 = alloca i8*, align 8
  %273 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %274 = alloca %"struct.std::__1::pair"*, align 8
  %275 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %276 = alloca i8*, align 8
  %277 = alloca %"struct.std::__1::pair"*, align 8
  %278 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %279 = alloca i8*, align 8
  %280 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.228"*, align 8
  %281 = alloca %"class.std::__1::__compressed_pair.227"*, align 8
  %282 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %283 = alloca %"struct.std::__1::pair", align 8
  %284 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %285 = alloca %"class.std::__1::basic_string"*, align 8
  %286 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %287 = alloca %"class.std::__1::tuple"*, align 8
  %288 = alloca %"class.std::__1::tuple.236"*, align 8
  %289 = alloca i64, align 8
  %290 = alloca i64, align 8
  %291 = alloca i8, align 1
  %292 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %293 = alloca i64, align 8
  %294 = alloca %"class.std::__1::unique_ptr.239", align 8
  %295 = alloca i64, align 8
  %296 = alloca i8*
  %297 = alloca i32
  %298 = alloca i64, align 8
  %299 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %300 = alloca %"class.std::__1::__hash_iterator", align 8
  %301 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %302 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %303 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %304 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %305 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %306 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %307 = alloca %"class.std::__1::basic_string"*, align 8
  %308 = alloca %"class.std::__1::unordered_map.212"*, align 8
  %309 = alloca %"class.std::__1::basic_string"*, align 8
  %310 = alloca %"class.std::__1::tuple", align 8
  %311 = alloca %"class.std::__1::tuple.236", align 1
  %312 = alloca %"class.std::__1::tuple.236", align 1
  %313 = alloca %"struct.std::__1::pair", align 8
  store %"class.std::__1::unordered_map.212"* %0, %"class.std::__1::unordered_map.212"** %308, align 8
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %309, align 8
  %314 = load %"class.std::__1::unordered_map.212"*, %"class.std::__1::unordered_map.212"** %308, align 8
  %315 = getelementptr inbounds %"class.std::__1::unordered_map.212", %"class.std::__1::unordered_map.212"* %314, i32 0, i32 0
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
  store %"class.std::__1::tuple.236"* %5, %"class.std::__1::tuple.236"** %4, align 8
  %344 = load %"class.std::__1::tuple.236"*, %"class.std::__1::tuple.236"** %4, align 8
  store %"class.std::__1::tuple.236"* %344, %"class.std::__1::tuple.236"** %3, align 8
  %345 = load %"class.std::__1::tuple.236"*, %"class.std::__1::tuple.236"** %3, align 8
  store %"class.std::__1::__hash_table.213"* %315, %"class.std::__1::__hash_table.213"** %284, align 8
  store %"class.std::__1::basic_string"* %316, %"class.std::__1::basic_string"** %285, align 8
  store %"struct.std::__1::piecewise_construct_t"* @_ZNSt3__1L19piecewise_constructE, %"struct.std::__1::piecewise_construct_t"** %286, align 8
  store %"class.std::__1::tuple"* %310, %"class.std::__1::tuple"** %287, align 8
  store %"class.std::__1::tuple.236"* %311, %"class.std::__1::tuple.236"** %288, align 8
  %346 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %284, align 8
  store %"class.std::__1::__hash_table.213"* %346, %"class.std::__1::__hash_table.213"** %282, align 8
  %347 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %282, align 8
  %348 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %347, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.227"* %348, %"class.std::__1::__compressed_pair.227"** %281, align 8
  %349 = load %"class.std::__1::__compressed_pair.227"*, %"class.std::__1::__compressed_pair.227"** %281, align 8
  %350 = bitcast %"class.std::__1::__compressed_pair.227"* %349 to %"class.std::__1::__libcpp_compressed_pair_imp.228"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.228"* %350, %"class.std::__1::__libcpp_compressed_pair_imp.228"** %280, align 8
  %351 = load %"class.std::__1::__libcpp_compressed_pair_imp.228"*, %"class.std::__1::__libcpp_compressed_pair_imp.228"** %280, align 8
  %352 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.228"* %351 to %"class.std::__1::__unordered_map_hasher.229"*
  %353 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %285, align 8
  store %"class.std::__1::__unordered_map_hasher.229"* %352, %"class.std::__1::__unordered_map_hasher.229"** %238, align 8
  store %"class.std::__1::basic_string"* %353, %"class.std::__1::basic_string"** %239, align 8
  %354 = load %"class.std::__1::__unordered_map_hasher.229"*, %"class.std::__1::__unordered_map_hasher.229"** %238, align 8
  %355 = bitcast %"class.std::__1::__unordered_map_hasher.229"* %354 to %"struct.std::__1::hash"*
  %356 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %239, align 8
  %357 = call i64 @_ZNKSt3__14hashINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEclERKS6_(%"struct.std::__1::hash"* %355, %"class.std::__1::basic_string"* dereferenceable(24) %356) #14
  store i64 %357, i64* %289, align 8
  store %"class.std::__1::__hash_table.213"* %346, %"class.std::__1::__hash_table.213"** %237, align 8
  %358 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %237, align 8
  %359 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %358, i32 0, i32 0
  store %"class.std::__1::unique_ptr.214"* %359, %"class.std::__1::unique_ptr.214"** %236, align 8
  %360 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %236, align 8
  %361 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %360, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %361, %"class.std::__1::__compressed_pair.215"** %235, align 8
  %362 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %235, align 8
  %363 = bitcast %"class.std::__1::__compressed_pair.215"* %362 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %363, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %234, align 8
  %364 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %234, align 8
  %365 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %364, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator.218"* %365, %"class.std::__1::__bucket_list_deallocator.218"** %233, align 8
  %366 = load %"class.std::__1::__bucket_list_deallocator.218"*, %"class.std::__1::__bucket_list_deallocator.218"** %233, align 8
  %367 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.218", %"class.std::__1::__bucket_list_deallocator.218"* %366, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.219"* %367, %"class.std::__1::__compressed_pair.219"** %232, align 8
  %368 = load %"class.std::__1::__compressed_pair.219"*, %"class.std::__1::__compressed_pair.219"** %232, align 8
  %369 = bitcast %"class.std::__1::__compressed_pair.219"* %368 to %"class.std::__1::__libcpp_compressed_pair_imp.220"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.220"* %369, %"class.std::__1::__libcpp_compressed_pair_imp.220"** %231, align 8
  %370 = load %"class.std::__1::__libcpp_compressed_pair_imp.220"*, %"class.std::__1::__libcpp_compressed_pair_imp.220"** %231, align 8
  %371 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.220", %"class.std::__1::__libcpp_compressed_pair_imp.220"* %370, i32 0, i32 0
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
  %402 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %346, i32 0, i32 0
  %403 = load i64, i64* %293, align 8
  store %"class.std::__1::unique_ptr.214"* %402, %"class.std::__1::unique_ptr.214"** %143, align 8
  store i64 %403, i64* %144, align 8
  %404 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %143, align 8
  %405 = load i64, i64* %144, align 8
  %406 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %404, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %406, %"class.std::__1::__compressed_pair.215"** %142, align 8
  %407 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %142, align 8
  %408 = bitcast %"class.std::__1::__compressed_pair.215"* %407 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %408, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %141, align 8
  %409 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %141, align 8
  %410 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %409, i32 0, i32 0
  %411 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %410, align 8
  %412 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %411, i64 %405
  %413 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %412, align 8
  store %"struct.std::__1::__hash_node_base.217"* %413, %"struct.std::__1::__hash_node_base.217"** %292, align 8
  %414 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %292, align 8
  %415 = icmp ne %"struct.std::__1::__hash_node_base.217"* %414, null
  br i1 %415, label %416, label %716

; <label>:416:                                    ; preds = %400
  %417 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %292, align 8
  %418 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %417, i32 0, i32 0
  %419 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %418, align 8
  store %"struct.std::__1::__hash_node_base.217"* %419, %"struct.std::__1::__hash_node_base.217"** %292, align 8
  br label %420

; <label>:420:                                    ; preds = %711, %416
  %421 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %292, align 8
  %422 = icmp ne %"struct.std::__1::__hash_node_base.217"* %421, null
  br i1 %422, label %423, label %456

; <label>:423:                                    ; preds = %420
  %424 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %292, align 8
  store %"struct.std::__1::__hash_node_base.217"* %424, %"struct.std::__1::__hash_node_base.217"** %140, align 8
  %425 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %140, align 8
  %426 = bitcast %"struct.std::__1::__hash_node_base.217"* %425 to %"struct.std::__1::__hash_node"*
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
  store %"class.std::__1::__hash_table.213"* %346, %"class.std::__1::__hash_table.213"** %118, align 8
  %459 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %118, align 8
  %460 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %459, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.230"* %460, %"class.std::__1::__compressed_pair.230"** %117, align 8
  %461 = load %"class.std::__1::__compressed_pair.230"*, %"class.std::__1::__compressed_pair.230"** %117, align 8
  %462 = bitcast %"class.std::__1::__compressed_pair.230"* %461 to %"class.std::__1::__libcpp_compressed_pair_imp.231"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.231"* %462, %"class.std::__1::__libcpp_compressed_pair_imp.231"** %116, align 8
  %463 = load %"class.std::__1::__libcpp_compressed_pair_imp.231"*, %"class.std::__1::__libcpp_compressed_pair_imp.231"** %116, align 8
  %464 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.231"* %463 to %"class.std::__1::__unordered_map_equal.232"*
  %465 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %292, align 8
  store %"struct.std::__1::__hash_node_base.217"* %465, %"struct.std::__1::__hash_node_base.217"** %102, align 8
  %466 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %102, align 8
  store %"struct.std::__1::__hash_node_base.217"* %466, %"struct.std::__1::__hash_node_base.217"** %101, align 8
  %467 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %101, align 8
  store %"struct.std::__1::__hash_node_base.217"* %467, %"struct.std::__1::__hash_node_base.217"** %100, align 8
  %468 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %100, align 8
  %469 = bitcast %"struct.std::__1::__hash_node_base.217"* %468 to i8*
  %470 = bitcast %"struct.std::__1::__hash_node_base.217"* %468 to %"struct.std::__1::__hash_node"*
  %471 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %470, i32 0, i32 2
  %472 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %285, align 8
  store %"class.std::__1::__unordered_map_equal.232"* %464, %"class.std::__1::__unordered_map_equal.232"** %94, align 8
  store %"union.std::__1::__hash_value_type"* %471, %"union.std::__1::__hash_value_type"** %95, align 8
  store %"class.std::__1::basic_string"* %472, %"class.std::__1::basic_string"** %96, align 8
  %473 = load %"class.std::__1::__unordered_map_equal.232"*, %"class.std::__1::__unordered_map_equal.232"** %94, align 8
  %474 = bitcast %"class.std::__1::__unordered_map_equal.232"* %473 to %"struct.std::__1::equal_to"*
  %475 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %95, align 8
  %476 = bitcast %"union.std::__1::__hash_value_type"* %475 to %"struct.std::__1::pair.237"*
  %477 = getelementptr inbounds %"struct.std::__1::pair.237", %"struct.std::__1::pair.237"* %476, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair"* %485, %"class.std::__1::__compressed_pair"** %82, align 8
  %486 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %82, align 8
  %487 = bitcast %"class.std::__1::__compressed_pair"* %486 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %487, %"class.std::__1::__libcpp_compressed_pair_imp"** %81, align 8
  %488 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %81, align 8
  %489 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %488, i32 0, i32 0
  %490 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %489, i32 0, i32 0
  %491 = bitcast %union.anon* %490 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %492 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %491, i32 0, i32 0
  %493 = bitcast %union.anon.0* %492 to i8*
  %494 = load i8, i8* %493, align 8
  %495 = zext i8 %494 to i32
  %496 = and i32 %495, 1
  %497 = icmp ne i32 %496, 0
  br i1 %497, label %498, label %509

; <label>:498:                                    ; preds = %458
  store %"class.std::__1::basic_string"* %483, %"class.std::__1::basic_string"** %77, align 8
  %499 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %77, align 8
  %500 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %499, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %500, %"class.std::__1::__compressed_pair"** %76, align 8
  %501 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %76, align 8
  %502 = bitcast %"class.std::__1::__compressed_pair"* %501 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %502, %"class.std::__1::__libcpp_compressed_pair_imp"** %75, align 8
  %503 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %75, align 8
  %504 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %503, i32 0, i32 0
  %505 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %504, i32 0, i32 0
  %506 = bitcast %union.anon* %505 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %507 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %506, i32 0, i32 1
  %508 = load i64, i64* %507, align 8
  br label %524

; <label>:509:                                    ; preds = %458
  store %"class.std::__1::basic_string"* %483, %"class.std::__1::basic_string"** %80, align 8
  %510 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %80, align 8
  %511 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %510, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %511, %"class.std::__1::__compressed_pair"** %79, align 8
  %512 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %79, align 8
  %513 = bitcast %"class.std::__1::__compressed_pair"* %512 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %513, %"class.std::__1::__libcpp_compressed_pair_imp"** %78, align 8
  %514 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %78, align 8
  %515 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %514, i32 0, i32 0
  %516 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %515, i32 0, i32 0
  %517 = bitcast %union.anon* %516 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %518 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %517, i32 0, i32 0
  %519 = bitcast %union.anon.0* %518 to i8*
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
  store %"class.std::__1::__compressed_pair"* %530, %"class.std::__1::__compressed_pair"** %72, align 8
  %531 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %72, align 8
  %532 = bitcast %"class.std::__1::__compressed_pair"* %531 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %532, %"class.std::__1::__libcpp_compressed_pair_imp"** %71, align 8
  %533 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %71, align 8
  %534 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %533, i32 0, i32 0
  %535 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %534, i32 0, i32 0
  %536 = bitcast %union.anon* %535 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %537 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %536, i32 0, i32 0
  %538 = bitcast %union.anon.0* %537 to i8*
  %539 = load i8, i8* %538, align 8
  %540 = zext i8 %539 to i32
  %541 = and i32 %540, 1
  %542 = icmp ne i32 %541, 0
  br i1 %542, label %543, label %554

; <label>:543:                                    ; preds = %524
  store %"class.std::__1::basic_string"* %528, %"class.std::__1::basic_string"** %67, align 8
  %544 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %67, align 8
  %545 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %544, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %545, %"class.std::__1::__compressed_pair"** %66, align 8
  %546 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %66, align 8
  %547 = bitcast %"class.std::__1::__compressed_pair"* %546 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %547, %"class.std::__1::__libcpp_compressed_pair_imp"** %65, align 8
  %548 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %65, align 8
  %549 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %548, i32 0, i32 0
  %550 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %549, i32 0, i32 0
  %551 = bitcast %union.anon* %550 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %552 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %551, i32 0, i32 1
  %553 = load i64, i64* %552, align 8
  br label %569

; <label>:554:                                    ; preds = %524
  store %"class.std::__1::basic_string"* %528, %"class.std::__1::basic_string"** %70, align 8
  %555 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %70, align 8
  %556 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %555, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %556, %"class.std::__1::__compressed_pair"** %69, align 8
  %557 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %69, align 8
  %558 = bitcast %"class.std::__1::__compressed_pair"* %557 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %558, %"class.std::__1::__libcpp_compressed_pair_imp"** %68, align 8
  %559 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %68, align 8
  %560 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %559, i32 0, i32 0
  %561 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %560, i32 0, i32 0
  %562 = bitcast %union.anon* %561 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %563 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %562, i32 0, i32 0
  %564 = bitcast %union.anon.0* %563 to i8*
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
  store %"class.std::__1::__compressed_pair"* %578, %"class.std::__1::__compressed_pair"** %61, align 8
  %579 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8
  %580 = bitcast %"class.std::__1::__compressed_pair"* %579 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %580, %"class.std::__1::__libcpp_compressed_pair_imp"** %60, align 8
  %581 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %60, align 8
  %582 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %581, i32 0, i32 0
  %583 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %582, i32 0, i32 0
  %584 = bitcast %union.anon* %583 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %585 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %584, i32 0, i32 0
  %586 = bitcast %union.anon.0* %585 to i8*
  %587 = load i8, i8* %586, align 8
  %588 = zext i8 %587 to i32
  %589 = and i32 %588, 1
  %590 = icmp ne i32 %589, 0
  br i1 %590, label %591, label %602

; <label>:591:                                    ; preds = %573
  store %"class.std::__1::basic_string"* %576, %"class.std::__1::basic_string"** %54, align 8
  %592 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8
  %593 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %592, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %593, %"class.std::__1::__compressed_pair"** %53, align 8
  %594 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %53, align 8
  %595 = bitcast %"class.std::__1::__compressed_pair"* %594 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %595, %"class.std::__1::__libcpp_compressed_pair_imp"** %52, align 8
  %596 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %52, align 8
  %597 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %596, i32 0, i32 0
  %598 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %597, i32 0, i32 0
  %599 = bitcast %union.anon* %598 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %600 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %599, i32 0, i32 2
  %601 = load i8*, i8** %600, align 8
  br label %615

; <label>:602:                                    ; preds = %573
  store %"class.std::__1::basic_string"* %576, %"class.std::__1::basic_string"** %59, align 8
  %603 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %59, align 8
  %604 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %603, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %604, %"class.std::__1::__compressed_pair"** %58, align 8
  %605 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %58, align 8
  %606 = bitcast %"class.std::__1::__compressed_pair"* %605 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %606, %"class.std::__1::__libcpp_compressed_pair_imp"** %57, align 8
  %607 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %57, align 8
  %608 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %607, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair"* %622, %"class.std::__1::__compressed_pair"** %44, align 8
  %623 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8
  %624 = bitcast %"class.std::__1::__compressed_pair"* %623 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %624, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8
  %625 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8
  %626 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %625, i32 0, i32 0
  %627 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %626, i32 0, i32 0
  %628 = bitcast %union.anon* %627 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %629 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %628, i32 0, i32 0
  %630 = bitcast %union.anon.0* %629 to i8*
  %631 = load i8, i8* %630, align 8
  %632 = zext i8 %631 to i32
  %633 = and i32 %632, 1
  %634 = icmp ne i32 %633, 0
  br i1 %634, label %635, label %646

; <label>:635:                                    ; preds = %615
  store %"class.std::__1::basic_string"* %620, %"class.std::__1::basic_string"** %37, align 8
  %636 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8
  %637 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %636, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %637, %"class.std::__1::__compressed_pair"** %36, align 8
  %638 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %36, align 8
  %639 = bitcast %"class.std::__1::__compressed_pair"* %638 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %639, %"class.std::__1::__libcpp_compressed_pair_imp"** %35, align 8
  %640 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %35, align 8
  %641 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %640, i32 0, i32 0
  %642 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %641, i32 0, i32 0
  %643 = bitcast %union.anon* %642 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %644 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %643, i32 0, i32 2
  %645 = load i8*, i8** %644, align 8
  br label %659

; <label>:646:                                    ; preds = %615
  store %"class.std::__1::basic_string"* %620, %"class.std::__1::basic_string"** %42, align 8
  %647 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8
  %648 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %647, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %648, %"class.std::__1::__compressed_pair"** %41, align 8
  %649 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %41, align 8
  %650 = bitcast %"class.std::__1::__compressed_pair"* %649 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %650, %"class.std::__1::__libcpp_compressed_pair_imp"** %40, align 8
  %651 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %40, align 8
  %652 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %651, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair"* %664, %"class.std::__1::__compressed_pair"** %49, align 8
  %665 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %49, align 8
  %666 = bitcast %"class.std::__1::__compressed_pair"* %665 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %666, %"class.std::__1::__libcpp_compressed_pair_imp"** %48, align 8
  %667 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %48, align 8
  %668 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %667, i32 0, i32 0
  %669 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %668, i32 0, i32 0
  %670 = bitcast %union.anon* %669 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %671 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %670, i32 0, i32 0
  %672 = bitcast %union.anon.0* %671 to i8*
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
  call void @__clang_call_terminate(i8* %707) #15
  unreachable

; <label>:708:                                    ; preds = %704, %696, %682, %572
  %709 = load i1, i1* %85, align 1
  br i1 %709, label %710, label %711

; <label>:710:                                    ; preds = %708
  br label %1150

; <label>:711:                                    ; preds = %708
  %712 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %292, align 8
  %713 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %712, i32 0, i32 0
  %714 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %713, align 8
  store %"struct.std::__1::__hash_node_base.217"* %714, %"struct.std::__1::__hash_node_base.217"** %292, align 8
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
  %723 = load %"class.std::__1::tuple.236"*, %"class.std::__1::tuple.236"** %288, align 8
  store %"class.std::__1::tuple.236"* %723, %"class.std::__1::tuple.236"** %99, align 8
  %724 = load %"class.std::__1::tuple.236"*, %"class.std::__1::tuple.236"** %99, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE21__construct_node_hashIRKNS_21piecewise_construct_tEJNS_5tupleIJOS7_EEENSQ_IJEEEEEENS_10unique_ptrINS_11__hash_nodeISB_PvEENS_22__hash_node_destructorINS5_ISX_EEEEEEmOT_DpOT0_(%"class.std::__1::unique_ptr.239"* sret %294, %"class.std::__1::__hash_table.213"* %346, i64 %718, %"struct.std::__1::piecewise_construct_t"* dereferenceable(1) %720, %"class.std::__1::tuple"* dereferenceable(8) %722, %"class.std::__1::tuple.236"* dereferenceable(1) %724)
  store %"class.std::__1::__hash_table.213"* %346, %"class.std::__1::__hash_table.213"** %105, align 8
  %725 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %105, align 8
  %726 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %725, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.227"* %726, %"class.std::__1::__compressed_pair.227"** %104, align 8
  %727 = load %"class.std::__1::__compressed_pair.227"*, %"class.std::__1::__compressed_pair.227"** %104, align 8
  %728 = bitcast %"class.std::__1::__compressed_pair.227"* %727 to %"class.std::__1::__libcpp_compressed_pair_imp.228"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.228"* %728, %"class.std::__1::__libcpp_compressed_pair_imp.228"** %103, align 8
  %729 = load %"class.std::__1::__libcpp_compressed_pair_imp.228"*, %"class.std::__1::__libcpp_compressed_pair_imp.228"** %103, align 8
  %730 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.228", %"class.std::__1::__libcpp_compressed_pair_imp.228"* %729, i32 0, i32 0
  %731 = load i64, i64* %730, align 8
  %732 = add i64 %731, 1
  %733 = uitofp i64 %732 to float
  %734 = load i64, i64* %290, align 8
  %735 = uitofp i64 %734 to float
  store %"class.std::__1::__hash_table.213"* %346, %"class.std::__1::__hash_table.213"** %108, align 8
  %736 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %108, align 8
  %737 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %736, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.230"* %737, %"class.std::__1::__compressed_pair.230"** %107, align 8
  %738 = load %"class.std::__1::__compressed_pair.230"*, %"class.std::__1::__compressed_pair.230"** %107, align 8
  %739 = bitcast %"class.std::__1::__compressed_pair.230"* %738 to %"class.std::__1::__libcpp_compressed_pair_imp.231"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.231"* %739, %"class.std::__1::__libcpp_compressed_pair_imp.231"** %106, align 8
  %740 = load %"class.std::__1::__libcpp_compressed_pair_imp.231"*, %"class.std::__1::__libcpp_compressed_pair_imp.231"** %106, align 8
  %741 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.231", %"class.std::__1::__libcpp_compressed_pair_imp.231"* %740, i32 0, i32 0
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
  store %"class.std::__1::__hash_table.213"* %346, %"class.std::__1::__hash_table.213"** %112, align 8
  %766 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %112, align 8
  %767 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %766, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.227"* %767, %"class.std::__1::__compressed_pair.227"** %111, align 8
  %768 = load %"class.std::__1::__compressed_pair.227"*, %"class.std::__1::__compressed_pair.227"** %111, align 8
  %769 = bitcast %"class.std::__1::__compressed_pair.227"* %768 to %"class.std::__1::__libcpp_compressed_pair_imp.228"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.228"* %769, %"class.std::__1::__libcpp_compressed_pair_imp.228"** %110, align 8
  %770 = load %"class.std::__1::__libcpp_compressed_pair_imp.228"*, %"class.std::__1::__libcpp_compressed_pair_imp.228"** %110, align 8
  %771 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.228", %"class.std::__1::__libcpp_compressed_pair_imp.228"* %770, i32 0, i32 0
  %772 = load i64, i64* %771, align 8
  %773 = add i64 %772, 1
  %774 = uitofp i64 %773 to float
  store %"class.std::__1::__hash_table.213"* %346, %"class.std::__1::__hash_table.213"** %115, align 8
  %775 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %115, align 8
  %776 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %775, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.230"* %776, %"class.std::__1::__compressed_pair.230"** %114, align 8
  %777 = load %"class.std::__1::__compressed_pair.230"*, %"class.std::__1::__compressed_pair.230"** %114, align 8
  %778 = bitcast %"class.std::__1::__compressed_pair.230"* %777 to %"class.std::__1::__libcpp_compressed_pair_imp.231"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.231"* %778, %"class.std::__1::__libcpp_compressed_pair_imp.231"** %113, align 8
  %779 = load %"class.std::__1::__libcpp_compressed_pair_imp.231"*, %"class.std::__1::__libcpp_compressed_pair_imp.231"** %113, align 8
  %780 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.231", %"class.std::__1::__libcpp_compressed_pair_imp.231"* %779, i32 0, i32 0
  %781 = load float, float* %780, align 4
  %782 = fdiv float %774, %781
  store float %782, float* %119, align 4
  %783 = load float, float* %119, align 4
  %784 = call float @ceilf(float %783) #11
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
  invoke void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE6rehashEm(%"class.std::__1::__hash_table.213"* %346, i64 %802)
          to label %803 unwind label %845

; <label>:803:                                    ; preds = %800
  store %"class.std::__1::__hash_table.213"* %346, %"class.std::__1::__hash_table.213"** %137, align 8
  %804 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %137, align 8
  %805 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %804, i32 0, i32 0
  store %"class.std::__1::unique_ptr.214"* %805, %"class.std::__1::unique_ptr.214"** %136, align 8
  %806 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %136, align 8
  %807 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %806, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %807, %"class.std::__1::__compressed_pair.215"** %135, align 8
  %808 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %135, align 8
  %809 = bitcast %"class.std::__1::__compressed_pair.215"* %808 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %809, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %134, align 8
  %810 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %134, align 8
  %811 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %810, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator.218"* %811, %"class.std::__1::__bucket_list_deallocator.218"** %133, align 8
  %812 = load %"class.std::__1::__bucket_list_deallocator.218"*, %"class.std::__1::__bucket_list_deallocator.218"** %133, align 8
  %813 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.218", %"class.std::__1::__bucket_list_deallocator.218"* %812, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.219"* %813, %"class.std::__1::__compressed_pair.219"** %132, align 8
  %814 = load %"class.std::__1::__compressed_pair.219"*, %"class.std::__1::__compressed_pair.219"** %132, align 8
  %815 = bitcast %"class.std::__1::__compressed_pair.219"* %814 to %"class.std::__1::__libcpp_compressed_pair_imp.220"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.220"* %815, %"class.std::__1::__libcpp_compressed_pair_imp.220"** %131, align 8
  %816 = load %"class.std::__1::__libcpp_compressed_pair_imp.220"*, %"class.std::__1::__libcpp_compressed_pair_imp.220"** %131, align 8
  %817 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.220", %"class.std::__1::__libcpp_compressed_pair_imp.220"* %816, i32 0, i32 0
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
  store %"class.std::__1::unique_ptr.239"* %294, %"class.std::__1::unique_ptr.239"** %172, align 8
  %849 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %172, align 8
  store %"class.std::__1::unique_ptr.239"* %849, %"class.std::__1::unique_ptr.239"** %171, align 8
  %850 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %171, align 8
  store %"class.std::__1::unique_ptr.239"* %850, %"class.std::__1::unique_ptr.239"** %168, align 8
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %169, align 8
  %851 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %168, align 8
  %852 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %851, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %852, %"class.std::__1::__compressed_pair.240"** %167, align 8
  %853 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %167, align 8
  %854 = bitcast %"class.std::__1::__compressed_pair.240"* %853 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %854, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %166, align 8
  %855 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %166, align 8
  %856 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %855, i32 0, i32 0
  %857 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %856, align 8
  store %"struct.std::__1::__hash_node"* %857, %"struct.std::__1::__hash_node"** %170, align 8
  %858 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %169, align 8
  %859 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %851, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %859, %"class.std::__1::__compressed_pair.240"** %148, align 8
  %860 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %148, align 8
  %861 = bitcast %"class.std::__1::__compressed_pair.240"* %860 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %861, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %147, align 8
  %862 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %147, align 8
  %863 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %862, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* %858, %"struct.std::__1::__hash_node"** %863, align 8
  %864 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %170, align 8
  %865 = icmp ne %"struct.std::__1::__hash_node"* %864, null
  br i1 %865, label %866, label %905

; <label>:866:                                    ; preds = %845
  %867 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %851, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %867, %"class.std::__1::__compressed_pair.240"** %146, align 8
  %868 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %146, align 8
  %869 = bitcast %"class.std::__1::__compressed_pair.240"* %868 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %869, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %145, align 8
  %870 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %145, align 8
  %871 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %870, i32 0, i32 1
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
  %879 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %878, align 8
  %880 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %165, align 8
  %881 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %880, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %881, %"union.std::__1::__hash_value_type"** %163, align 8
  %882 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %163, align 8
  %883 = bitcast %"union.std::__1::__hash_value_type"* %882 to %"struct.std::__1::pair.237"*
  store %"struct.std::__1::pair.237"* %883, %"struct.std::__1::pair.237"** %162, align 8
  %884 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %162, align 8
  %885 = bitcast %"struct.std::__1::pair.237"* %884 to i8*
  store %"class.std::__1::allocator.225"* %879, %"class.std::__1::allocator.225"** %158, align 8
  store %"struct.std::__1::pair.237"* %884, %"struct.std::__1::pair.237"** %159, align 8
  %886 = bitcast %"struct.std::__1::__has_destroy"* %161 to %"struct.std::__1::integral_constant.244"*
  %887 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %158, align 8
  %888 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %159, align 8
  store %"class.std::__1::allocator.225"* %887, %"class.std::__1::allocator.225"** %156, align 8
  store %"struct.std::__1::pair.237"* %888, %"struct.std::__1::pair.237"** %157, align 8
  %889 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %157, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEED1Ev(%"struct.std::__1::pair.237"* %889) #14
  br label %890

; <label>:890:                                    ; preds = %877, %866
  %891 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %165, align 8
  %892 = icmp ne %"struct.std::__1::__hash_node"* %891, null
  br i1 %892, label %893, label %904

; <label>:893:                                    ; preds = %890
  %894 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %873, i32 0, i32 0
  %895 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %894, align 8
  %896 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %165, align 8
  store %"class.std::__1::allocator.225"* %895, %"class.std::__1::allocator.225"** %153, align 8
  store %"struct.std::__1::__hash_node"* %896, %"struct.std::__1::__hash_node"** %154, align 8
  store i64 1, i64* %155, align 8
  %897 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %153, align 8
  %898 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %154, align 8
  %899 = load i64, i64* %155, align 8
  store %"class.std::__1::allocator.225"* %897, %"class.std::__1::allocator.225"** %150, align 8
  store %"struct.std::__1::__hash_node"* %898, %"struct.std::__1::__hash_node"** %151, align 8
  store i64 %899, i64* %152, align 8
  %900 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %150, align 8
  %901 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %151, align 8
  %902 = bitcast %"struct.std::__1::__hash_node"* %901 to i8*
  store i8* %902, i8** %149, align 8
  %903 = load i8*, i8** %149, align 8
  call void @_ZdlPv(i8* %903) #16
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
  %911 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %346, i32 0, i32 0
  %912 = load i64, i64* %293, align 8
  store %"class.std::__1::unique_ptr.214"* %911, %"class.std::__1::unique_ptr.214"** %175, align 8
  store i64 %912, i64* %176, align 8
  %913 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %175, align 8
  %914 = load i64, i64* %176, align 8
  %915 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %913, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %915, %"class.std::__1::__compressed_pair.215"** %174, align 8
  %916 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %174, align 8
  %917 = bitcast %"class.std::__1::__compressed_pair.215"* %916 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %917, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %173, align 8
  %918 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %173, align 8
  %919 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %918, i32 0, i32 0
  %920 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %919, align 8
  %921 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %920, i64 %914
  %922 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %921, align 8
  store %"struct.std::__1::__hash_node_base.217"* %922, %"struct.std::__1::__hash_node_base.217"** %299, align 8
  %923 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %299, align 8
  %924 = icmp eq %"struct.std::__1::__hash_node_base.217"* %923, null
  br i1 %924, label %925, label %1047

; <label>:925:                                    ; preds = %910
  %926 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %346, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.223"* %926, %"class.std::__1::__compressed_pair.223"** %178, align 8
  %927 = load %"class.std::__1::__compressed_pair.223"*, %"class.std::__1::__compressed_pair.223"** %178, align 8
  %928 = bitcast %"class.std::__1::__compressed_pair.223"* %927 to %"class.std::__1::__libcpp_compressed_pair_imp.224"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.224"* %928, %"class.std::__1::__libcpp_compressed_pair_imp.224"** %177, align 8
  %929 = load %"class.std::__1::__libcpp_compressed_pair_imp.224"*, %"class.std::__1::__libcpp_compressed_pair_imp.224"** %177, align 8
  %930 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.224", %"class.std::__1::__libcpp_compressed_pair_imp.224"* %929, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.217"* %930, %"struct.std::__1::__hash_node_base.217"** %181, align 8
  %931 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %181, align 8
  store %"struct.std::__1::__hash_node_base.217"* %931, %"struct.std::__1::__hash_node_base.217"** %180, align 8
  %932 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %180, align 8
  store %"struct.std::__1::__hash_node_base.217"* %932, %"struct.std::__1::__hash_node_base.217"** %179, align 8
  %933 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %179, align 8
  %934 = bitcast %"struct.std::__1::__hash_node_base.217"* %933 to i8*
  store %"struct.std::__1::__hash_node_base.217"* %933, %"struct.std::__1::__hash_node_base.217"** %299, align 8
  %935 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %299, align 8
  %936 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %935, i32 0, i32 0
  %937 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %936, align 8
  store %"class.std::__1::unique_ptr.239"* %294, %"class.std::__1::unique_ptr.239"** %184, align 8
  %938 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %184, align 8
  %939 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %938, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %939, %"class.std::__1::__compressed_pair.240"** %183, align 8
  %940 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %183, align 8
  %941 = bitcast %"class.std::__1::__compressed_pair.240"* %940 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %941, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %182, align 8
  %942 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %182, align 8
  %943 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %942, i32 0, i32 0
  %944 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %943, align 8
  %945 = bitcast %"struct.std::__1::__hash_node"* %944 to %"struct.std::__1::__hash_node_base.217"*
  %946 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %945, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.217"* %937, %"struct.std::__1::__hash_node_base.217"** %946, align 8
  store %"class.std::__1::unique_ptr.239"* %294, %"class.std::__1::unique_ptr.239"** %187, align 8
  %947 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %187, align 8
  %948 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %947, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %948, %"class.std::__1::__compressed_pair.240"** %186, align 8
  %949 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %186, align 8
  %950 = bitcast %"class.std::__1::__compressed_pair.240"* %949 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %950, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %185, align 8
  %951 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %185, align 8
  %952 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %951, i32 0, i32 0
  %953 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %952, align 8
  %954 = bitcast %"struct.std::__1::__hash_node"* %953 to %"struct.std::__1::__hash_node_base.217"*
  store %"struct.std::__1::__hash_node_base.217"* %954, %"struct.std::__1::__hash_node_base.217"** %190, align 8
  %955 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %190, align 8
  store %"struct.std::__1::__hash_node_base.217"* %955, %"struct.std::__1::__hash_node_base.217"** %189, align 8
  %956 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %189, align 8
  store %"struct.std::__1::__hash_node_base.217"* %956, %"struct.std::__1::__hash_node_base.217"** %188, align 8
  %957 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %188, align 8
  %958 = bitcast %"struct.std::__1::__hash_node_base.217"* %957 to i8*
  %959 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %299, align 8
  %960 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %959, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.217"* %957, %"struct.std::__1::__hash_node_base.217"** %960, align 8
  %961 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %299, align 8
  %962 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %346, i32 0, i32 0
  %963 = load i64, i64* %293, align 8
  store %"class.std::__1::unique_ptr.214"* %962, %"class.std::__1::unique_ptr.214"** %193, align 8
  store i64 %963, i64* %194, align 8
  %964 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %193, align 8
  %965 = load i64, i64* %194, align 8
  %966 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %964, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %966, %"class.std::__1::__compressed_pair.215"** %192, align 8
  %967 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %192, align 8
  %968 = bitcast %"class.std::__1::__compressed_pair.215"* %967 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %968, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %191, align 8
  %969 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %191, align 8
  %970 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %969, i32 0, i32 0
  %971 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %970, align 8
  %972 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %971, i64 %965
  store %"struct.std::__1::__hash_node_base.217"* %961, %"struct.std::__1::__hash_node_base.217"** %972, align 8
  store %"class.std::__1::unique_ptr.239"* %294, %"class.std::__1::unique_ptr.239"** %197, align 8
  %973 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %197, align 8
  %974 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %973, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %974, %"class.std::__1::__compressed_pair.240"** %196, align 8
  %975 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %196, align 8
  %976 = bitcast %"class.std::__1::__compressed_pair.240"* %975 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %976, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %195, align 8
  %977 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %195, align 8
  %978 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %977, i32 0, i32 0
  %979 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %978, align 8
  %980 = bitcast %"struct.std::__1::__hash_node"* %979 to %"struct.std::__1::__hash_node_base.217"*
  %981 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %980, i32 0, i32 0
  %982 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %981, align 8
  %983 = icmp ne %"struct.std::__1::__hash_node_base.217"* %982, null
  br i1 %983, label %984, label %1046

; <label>:984:                                    ; preds = %925
  store %"class.std::__1::unique_ptr.239"* %294, %"class.std::__1::unique_ptr.239"** %200, align 8
  %985 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %200, align 8
  %986 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %985, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %986, %"class.std::__1::__compressed_pair.240"** %199, align 8
  %987 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %199, align 8
  %988 = bitcast %"class.std::__1::__compressed_pair.240"* %987 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %988, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %198, align 8
  %989 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %198, align 8
  %990 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %989, i32 0, i32 0
  %991 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %990, align 8
  %992 = bitcast %"struct.std::__1::__hash_node"* %991 to %"struct.std::__1::__hash_node_base.217"*
  store %"struct.std::__1::__hash_node_base.217"* %992, %"struct.std::__1::__hash_node_base.217"** %203, align 8
  %993 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %203, align 8
  store %"struct.std::__1::__hash_node_base.217"* %993, %"struct.std::__1::__hash_node_base.217"** %202, align 8
  %994 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %202, align 8
  store %"struct.std::__1::__hash_node_base.217"* %994, %"struct.std::__1::__hash_node_base.217"** %201, align 8
  %995 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %201, align 8
  %996 = bitcast %"struct.std::__1::__hash_node_base.217"* %995 to i8*
  %997 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %346, i32 0, i32 0
  store %"class.std::__1::unique_ptr.239"* %294, %"class.std::__1::unique_ptr.239"** %206, align 8
  %998 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %206, align 8
  %999 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %998, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %999, %"class.std::__1::__compressed_pair.240"** %205, align 8
  %1000 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %205, align 8
  %1001 = bitcast %"class.std::__1::__compressed_pair.240"* %1000 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1001, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %204, align 8
  %1002 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %204, align 8
  %1003 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1002, i32 0, i32 0
  %1004 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %1003, align 8
  %1005 = bitcast %"struct.std::__1::__hash_node"* %1004 to %"struct.std::__1::__hash_node_base.217"*
  %1006 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %1005, i32 0, i32 0
  %1007 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %1006, align 8
  store %"struct.std::__1::__hash_node_base.217"* %1007, %"struct.std::__1::__hash_node_base.217"** %207, align 8
  %1008 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %207, align 8
  %1009 = bitcast %"struct.std::__1::__hash_node_base.217"* %1008 to %"struct.std::__1::__hash_node"*
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
  store %"class.std::__1::unique_ptr.214"* %997, %"class.std::__1::unique_ptr.214"** %212, align 8
  store i64 %1036, i64* %213, align 8
  %1037 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %212, align 8
  %1038 = load i64, i64* %213, align 8
  %1039 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %1037, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %1039, %"class.std::__1::__compressed_pair.215"** %211, align 8
  %1040 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %211, align 8
  %1041 = bitcast %"class.std::__1::__compressed_pair.215"* %1040 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %1041, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %210, align 8
  %1042 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %210, align 8
  %1043 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %1042, i32 0, i32 0
  %1044 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %1043, align 8
  %1045 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %1044, i64 %1038
  store %"struct.std::__1::__hash_node_base.217"* %995, %"struct.std::__1::__hash_node_base.217"** %1045, align 8
  br label %1046

; <label>:1046:                                   ; preds = %1035, %925
  br label %1070

; <label>:1047:                                   ; preds = %910
  %1048 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %299, align 8
  %1049 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %1048, i32 0, i32 0
  %1050 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %1049, align 8
  store %"class.std::__1::unique_ptr.239"* %294, %"class.std::__1::unique_ptr.239"** %216, align 8
  %1051 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %216, align 8
  %1052 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %1051, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %1052, %"class.std::__1::__compressed_pair.240"** %215, align 8
  %1053 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %215, align 8
  %1054 = bitcast %"class.std::__1::__compressed_pair.240"* %1053 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1054, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %214, align 8
  %1055 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %214, align 8
  %1056 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1055, i32 0, i32 0
  %1057 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %1056, align 8
  %1058 = bitcast %"struct.std::__1::__hash_node"* %1057 to %"struct.std::__1::__hash_node_base.217"*
  %1059 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %1058, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.217"* %1050, %"struct.std::__1::__hash_node_base.217"** %1059, align 8
  store %"class.std::__1::unique_ptr.239"* %294, %"class.std::__1::unique_ptr.239"** %219, align 8
  %1060 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %219, align 8
  %1061 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %1060, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %1061, %"class.std::__1::__compressed_pair.240"** %218, align 8
  %1062 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %218, align 8
  %1063 = bitcast %"class.std::__1::__compressed_pair.240"* %1062 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1063, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %217, align 8
  %1064 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %217, align 8
  %1065 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1064, i32 0, i32 0
  %1066 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %1065, align 8
  %1067 = bitcast %"struct.std::__1::__hash_node"* %1066 to %"struct.std::__1::__hash_node_base.217"*
  %1068 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %299, align 8
  %1069 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %1068, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.217"* %1067, %"struct.std::__1::__hash_node_base.217"** %1069, align 8
  br label %1070

; <label>:1070:                                   ; preds = %1047, %1046
  store %"class.std::__1::unique_ptr.239"* %294, %"class.std::__1::unique_ptr.239"** %224, align 8
  %1071 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %224, align 8
  %1072 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %1071, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %1072, %"class.std::__1::__compressed_pair.240"** %223, align 8
  %1073 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %223, align 8
  %1074 = bitcast %"class.std::__1::__compressed_pair.240"* %1073 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1074, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %222, align 8
  %1075 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %222, align 8
  %1076 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1075, i32 0, i32 0
  %1077 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %1076, align 8
  store %"struct.std::__1::__hash_node"* %1077, %"struct.std::__1::__hash_node"** %225, align 8
  %1078 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %1071, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %1078, %"class.std::__1::__compressed_pair.240"** %221, align 8
  %1079 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %221, align 8
  %1080 = bitcast %"class.std::__1::__compressed_pair.240"* %1079 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1080, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %220, align 8
  %1081 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %220, align 8
  %1082 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1081, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %1082, align 8
  %1083 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %225, align 8
  %1084 = bitcast %"struct.std::__1::__hash_node"* %1083 to %"struct.std::__1::__hash_node_base.217"*
  store %"struct.std::__1::__hash_node_base.217"* %1084, %"struct.std::__1::__hash_node_base.217"** %292, align 8
  store %"class.std::__1::__hash_table.213"* %346, %"class.std::__1::__hash_table.213"** %228, align 8
  %1085 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %228, align 8
  %1086 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %1085, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.227"* %1086, %"class.std::__1::__compressed_pair.227"** %227, align 8
  %1087 = load %"class.std::__1::__compressed_pair.227"*, %"class.std::__1::__compressed_pair.227"** %227, align 8
  %1088 = bitcast %"class.std::__1::__compressed_pair.227"* %1087 to %"class.std::__1::__libcpp_compressed_pair_imp.228"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.228"* %1088, %"class.std::__1::__libcpp_compressed_pair_imp.228"** %226, align 8
  %1089 = load %"class.std::__1::__libcpp_compressed_pair_imp.228"*, %"class.std::__1::__libcpp_compressed_pair_imp.228"** %226, align 8
  %1090 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.228", %"class.std::__1::__libcpp_compressed_pair_imp.228"* %1089, i32 0, i32 0
  %1091 = load i64, i64* %1090, align 8
  %1092 = add i64 %1091, 1
  store i64 %1092, i64* %1090, align 8
  store i8 1, i8* %291, align 1
  store %"class.std::__1::unique_ptr.239"* %294, %"class.std::__1::unique_ptr.239"** %267, align 8
  %1093 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %267, align 8
  store %"class.std::__1::unique_ptr.239"* %1093, %"class.std::__1::unique_ptr.239"** %266, align 8
  %1094 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %266, align 8
  store %"class.std::__1::unique_ptr.239"* %1094, %"class.std::__1::unique_ptr.239"** %263, align 8
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %264, align 8
  %1095 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %263, align 8
  %1096 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %1095, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %1096, %"class.std::__1::__compressed_pair.240"** %262, align 8
  %1097 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %262, align 8
  %1098 = bitcast %"class.std::__1::__compressed_pair.240"* %1097 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1098, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %261, align 8
  %1099 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %261, align 8
  %1100 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1099, i32 0, i32 0
  %1101 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %1100, align 8
  store %"struct.std::__1::__hash_node"* %1101, %"struct.std::__1::__hash_node"** %265, align 8
  %1102 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %264, align 8
  %1103 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %1095, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %1103, %"class.std::__1::__compressed_pair.240"** %243, align 8
  %1104 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %243, align 8
  %1105 = bitcast %"class.std::__1::__compressed_pair.240"* %1104 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1105, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %242, align 8
  %1106 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %242, align 8
  %1107 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1106, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* %1102, %"struct.std::__1::__hash_node"** %1107, align 8
  %1108 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %265, align 8
  %1109 = icmp ne %"struct.std::__1::__hash_node"* %1108, null
  br i1 %1109, label %1110, label %1149

; <label>:1110:                                   ; preds = %1070
  %1111 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %1095, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %1111, %"class.std::__1::__compressed_pair.240"** %241, align 8
  %1112 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %241, align 8
  %1113 = bitcast %"class.std::__1::__compressed_pair.240"* %1112 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1113, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %240, align 8
  %1114 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %240, align 8
  %1115 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %1114, i32 0, i32 1
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
  %1123 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %1122, align 8
  %1124 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %260, align 8
  %1125 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %1124, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %1125, %"union.std::__1::__hash_value_type"** %258, align 8
  %1126 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %258, align 8
  %1127 = bitcast %"union.std::__1::__hash_value_type"* %1126 to %"struct.std::__1::pair.237"*
  store %"struct.std::__1::pair.237"* %1127, %"struct.std::__1::pair.237"** %257, align 8
  %1128 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %257, align 8
  %1129 = bitcast %"struct.std::__1::pair.237"* %1128 to i8*
  store %"class.std::__1::allocator.225"* %1123, %"class.std::__1::allocator.225"** %253, align 8
  store %"struct.std::__1::pair.237"* %1128, %"struct.std::__1::pair.237"** %254, align 8
  %1130 = bitcast %"struct.std::__1::__has_destroy"* %256 to %"struct.std::__1::integral_constant.244"*
  %1131 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %253, align 8
  %1132 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %254, align 8
  store %"class.std::__1::allocator.225"* %1131, %"class.std::__1::allocator.225"** %251, align 8
  store %"struct.std::__1::pair.237"* %1132, %"struct.std::__1::pair.237"** %252, align 8
  %1133 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %252, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEED1Ev(%"struct.std::__1::pair.237"* %1133) #14
  br label %1134

; <label>:1134:                                   ; preds = %1121, %1110
  %1135 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %260, align 8
  %1136 = icmp ne %"struct.std::__1::__hash_node"* %1135, null
  br i1 %1136, label %1137, label %1148

; <label>:1137:                                   ; preds = %1134
  %1138 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %1117, i32 0, i32 0
  %1139 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %1138, align 8
  %1140 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %260, align 8
  store %"class.std::__1::allocator.225"* %1139, %"class.std::__1::allocator.225"** %248, align 8
  store %"struct.std::__1::__hash_node"* %1140, %"struct.std::__1::__hash_node"** %249, align 8
  store i64 1, i64* %250, align 8
  %1141 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %248, align 8
  %1142 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %249, align 8
  %1143 = load i64, i64* %250, align 8
  store %"class.std::__1::allocator.225"* %1141, %"class.std::__1::allocator.225"** %245, align 8
  store %"struct.std::__1::__hash_node"* %1142, %"struct.std::__1::__hash_node"** %246, align 8
  store i64 %1143, i64* %247, align 8
  %1144 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %245, align 8
  %1145 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %246, align 8
  %1146 = bitcast %"struct.std::__1::__hash_node"* %1145 to i8*
  store i8* %1146, i8** %244, align 8
  %1147 = load i8*, i8** %244, align 8
  call void @_ZdlPv(i8* %1147) #16
  br label %1148

; <label>:1148:                                   ; preds = %1137, %1134
  br label %1149

; <label>:1149:                                   ; preds = %1148, %1070
  br label %1150

; <label>:1150:                                   ; preds = %710, %1149
  %1151 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %292, align 8
  store %"class.std::__1::__hash_iterator"* %300, %"class.std::__1::__hash_iterator"** %270, align 8
  store %"struct.std::__1::__hash_node_base.217"* %1151, %"struct.std::__1::__hash_node_base.217"** %271, align 8
  %1152 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %270, align 8
  %1153 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %271, align 8
  store %"class.std::__1::__hash_iterator"* %1152, %"class.std::__1::__hash_iterator"** %268, align 8
  store %"struct.std::__1::__hash_node_base.217"* %1153, %"struct.std::__1::__hash_node_base.217"** %269, align 8
  %1154 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %268, align 8
  %1155 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %1154, i32 0, i32 0
  %1156 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %269, align 8
  store %"struct.std::__1::__hash_node_base.217"* %1156, %"struct.std::__1::__hash_node_base.217"** %1155, align 8
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
  %1172 = bitcast %"struct.std::__1::pair"* %283 to { %"struct.std::__1::__hash_node_base.217"*, i8 }*
  %1173 = load { %"struct.std::__1::__hash_node_base.217"*, i8 }, { %"struct.std::__1::__hash_node_base.217"*, i8 }* %1172, align 8
  %1174 = bitcast %"struct.std::__1::pair"* %313 to { %"struct.std::__1::__hash_node_base.217"*, i8 }*
  %1175 = getelementptr inbounds { %"struct.std::__1::__hash_node_base.217"*, i8 }, { %"struct.std::__1::__hash_node_base.217"*, i8 }* %1174, i32 0, i32 0
  %1176 = extractvalue { %"struct.std::__1::__hash_node_base.217"*, i8 } %1173, 0
  store %"struct.std::__1::__hash_node_base.217"* %1176, %"struct.std::__1::__hash_node_base.217"** %1175, align 8
  %1177 = getelementptr inbounds { %"struct.std::__1::__hash_node_base.217"*, i8 }, { %"struct.std::__1::__hash_node_base.217"*, i8 }* %1174, i32 0, i32 1
  %1178 = extractvalue { %"struct.std::__1::__hash_node_base.217"*, i8 } %1173, 1
  store i8 %1178, i8* %1177, align 8
  %1179 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %313, i32 0, i32 0
  store %"class.std::__1::__hash_iterator"* %1179, %"class.std::__1::__hash_iterator"** %306, align 8
  %1180 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %306, align 8
  %1181 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %1180, i32 0, i32 0
  %1182 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %1181, align 8
  store %"struct.std::__1::__hash_node_base.217"* %1182, %"struct.std::__1::__hash_node_base.217"** %305, align 8
  %1183 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %305, align 8
  store %"struct.std::__1::__hash_node_base.217"* %1183, %"struct.std::__1::__hash_node_base.217"** %304, align 8
  %1184 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %304, align 8
  store %"struct.std::__1::__hash_node_base.217"* %1184, %"struct.std::__1::__hash_node_base.217"** %303, align 8
  %1185 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %303, align 8
  %1186 = bitcast %"struct.std::__1::__hash_node_base.217"* %1185 to i8*
  %1187 = bitcast %"struct.std::__1::__hash_node_base.217"* %1185 to %"struct.std::__1::__hash_node"*
  %1188 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %1187, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %1188, %"union.std::__1::__hash_value_type"** %302, align 8
  %1189 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %302, align 8
  store %"union.std::__1::__hash_value_type"* %1189, %"union.std::__1::__hash_value_type"** %301, align 8
  %1190 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %301, align 8
  %1191 = bitcast %"union.std::__1::__hash_value_type"* %1190 to i8*
  %1192 = bitcast %"union.std::__1::__hash_value_type"* %1190 to %"struct.std::__1::pair.237"*
  %1193 = getelementptr inbounds %"struct.std::__1::pair.237", %"struct.std::__1::pair.237"* %1192, i32 0, i32 1
  ret %"class.llvm::Constant"** %1193
}

declare %"class.llvm::ConstantPointerNull"* @_ZN4llvm19ConstantPointerNull3getEPNS_11PointerTypeE(%"class.llvm::PointerType"*) #1

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
define linkonce_odr void @_ZN4llvm8ArrayRefIPNS_5ValueEEC1ERKSt16initializer_listIS2_E(%"class.llvm::ArrayRef.211"*, %"class.std::initializer_list"* dereferenceable(16)) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::ArrayRef.211"*, align 8
  %4 = alloca %"class.std::initializer_list"*, align 8
  store %"class.llvm::ArrayRef.211"* %0, %"class.llvm::ArrayRef.211"** %3, align 8
  store %"class.std::initializer_list"* %1, %"class.std::initializer_list"** %4, align 8
  %5 = load %"class.llvm::ArrayRef.211"*, %"class.llvm::ArrayRef.211"** %3, align 8
  %6 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %4, align 8
  call void @_ZN4llvm8ArrayRefIPNS_5ValueEEC2ERKSt16initializer_listIS2_E(%"class.llvm::ArrayRef.211"* %5, %"class.std::initializer_list"* dereferenceable(16) %6)
  ret void
}

; Function Attrs: nounwind
declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"*) unnamed_addr #2

declare %"class.llvm::Value"* @_ZN8CGMemory12CreateAllocaEPN4llvm4TypeENSt3__112basic_stringIcNS3_11char_traitsIcEENS3_9allocatorIcEEEEPNS0_5ValueEtbPNS0_10BasicBlockE(%"class.llvm::Type"*, %"class.std::__1::basic_string"*, %"class.llvm::Value"*, i16 zeroext, i1 zeroext, %"class.llvm::BasicBlock"*) #1

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::IntegerType"* @_ZN4llvm13IRBuilderBase9getInt8TyEv(%"class.llvm::IRBuilderBase"*) #0 align 2 {
  %2 = alloca %"class.llvm::IRBuilderBase"*, align 8
  store %"class.llvm::IRBuilderBase"* %0, %"class.llvm::IRBuilderBase"** %2, align 8
  %3 = load %"class.llvm::IRBuilderBase"*, %"class.llvm::IRBuilderBase"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %3, i32 0, i32 3
  %5 = load %"class.llvm::LLVMContext"*, %"class.llvm::LLVMContext"** %4, align 8
  %6 = call %"class.llvm::IntegerType"* @_ZN4llvm4Type9getInt8TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8) %5)
  ret %"class.llvm::IntegerType"* %6
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateSExtEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Type"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
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
  %13 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCastENS_11Instruction7CastOpsEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %9, i32 38, %"class.llvm::Value"* %10, %"class.llvm::Type"* %11, %"class.llvm::Twine"* dereferenceable(24) %12)
  ret %"class.llvm::Value"* %13
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE11CreateTruncEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Type"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
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
  %13 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCastENS_11Instruction7CastOpsEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %9, i32 36, %"class.llvm::Value"* %10, %"class.llvm::Type"* %11, %"class.llvm::Twine"* dereferenceable(24) %12)
  ret %"class.llvm::Value"* %13
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE12CreateSIToFPEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Type"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
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
  %13 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCastENS_11Instruction7CastOpsEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %9, i32 42, %"class.llvm::Value"* %10, %"class.llvm::Type"* %11, %"class.llvm::Twine"* dereferenceable(24) %12)
  ret %"class.llvm::Value"* %13
}

declare void @_ZN7Warning5issueENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEEP7AstNode12WarningLevel(%"class.std::__1::basic_string"*, %class.AstNode*, i32) #1

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE12CreateFPToSIEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Type"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
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
  %13 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCastENS_11Instruction7CastOpsEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %9, i32 40, %"class.llvm::Value"* %10, %"class.llvm::Type"* %11, %"class.llvm::Twine"* dereferenceable(24) %12)
  ret %"class.llvm::Value"* %13
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE11CreateFPExtEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Type"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
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
  %13 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCastENS_11Instruction7CastOpsEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %9, i32 44, %"class.llvm::Value"* %10, %"class.llvm::Type"* %11, %"class.llvm::Twine"* dereferenceable(24) %12)
  ret %"class.llvm::Value"* %13
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE13CreateFPTruncEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"*, %"class.llvm::Value"*, %"class.llvm::Type"*, %"class.llvm::Twine"* dereferenceable(24)) #0 align 2 {
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
  %13 = call %"class.llvm::Value"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCastENS_11Instruction7CastOpsEPNS_5ValueEPNS_4TypeERKNS_5TwineE(%"class.llvm::IRBuilder"* %9, i32 43, %"class.llvm::Value"* %10, %"class.llvm::Type"* %11, %"class.llvm::Twine"* dereferenceable(24) %12)
  ret %"class.llvm::Value"* %13
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::ConstantInt"* @_ZN4llvm13IRBuilderBase8getFalseEv(%"class.llvm::IRBuilderBase"*) #0 align 2 {
  %2 = alloca %"class.llvm::IRBuilderBase"*, align 8
  store %"class.llvm::IRBuilderBase"* %0, %"class.llvm::IRBuilderBase"** %2, align 8
  %3 = load %"class.llvm::IRBuilderBase"*, %"class.llvm::IRBuilderBase"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %3, i32 0, i32 3
  %5 = load %"class.llvm::LLVMContext"*, %"class.llvm::LLVMContext"** %4, align 8
  %6 = call %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt8getFalseERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8) %5)
  ret %"class.llvm::ConstantInt"* %6
}

declare void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32, %"class.std::__1::basic_string"*, %class.AstNode*) #1

declare void @_ZNK13TypeSpecifier9to_stringEv(%"class.std::__1::basic_string"* sret, %struct.TypeSpecifier*) #1

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
  call void @__assert_rtn(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__._ZN4llvm5TwineC2EPKc, i32 0, i32 0), i8* getelementptr inbounds ([56 x i8], [56 x i8]* @.str.13, i32 0, i32 0), i32 274, i8* getelementptr inbounds ([30 x i8], [30 x i8]* @.str.14, i32 0, i32 0)) #17
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
declare void @__assert_rtn(i8*, i8*, i32, i8*) #3

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

declare %"class.llvm::PointerType"* @_ZN4llvm4Type12getInt8PtrTyERNS_11LLVMContextEj(%"class.llvm::LLVMContext"* dereferenceable(8), i32) #1

declare %"class.llvm::IntegerType"* @_ZN4llvm4Type10getInt64TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8)) #1

declare %"class.llvm::IntegerType"* @_ZN4llvm4Type9getInt8TyERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8)) #1

declare %"class.llvm::ConstantInt"* @_ZN4llvm11ConstantInt8getFalseERNS_11LLVMContextE(%"class.llvm::LLVMContext"* dereferenceable(8)) #1

; Function Attrs: noinline noreturn nounwind
define linkonce_odr hidden void @__clang_call_terminate(i8*) #5 {
  %2 = call i8* @__cxa_begin_catch(i8* %0) #14
  call void @_ZSt9terminatev() #15
  unreachable
}

declare i8* @__cxa_begin_catch(i8*)

declare void @_ZSt9terminatev()

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #6

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
define linkonce_odr %"class.llvm::Constant"* @_ZN4llvm8dyn_castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"*) #7 {
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
  %21 = load %"class.llvm::ilist_node_impl.60"*, %"class.llvm::ilist_node_impl.60"** %20, align 8
  call void @_ZNK4llvm24IRBuilderDefaultInserter12InsertHelperEPNS_11InstructionERKNS_5TwineEPNS_10BasicBlockENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEE(%"class.llvm::IRBuilderDefaultInserter"* %9, %"class.llvm::Instruction"* %11, %"class.llvm::Twine"* dereferenceable(24) %12, %"class.llvm::BasicBlock"* %15, %"class.llvm::ilist_node_impl.60"* %21)
  %22 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderBase"*
  %23 = load %"class.llvm::CastInst"*, %"class.llvm::CastInst"** %5, align 8
  %24 = bitcast %"class.llvm::CastInst"* %23 to %"class.llvm::Instruction"*
  call void @_ZNK4llvm13IRBuilderBase20SetInstDebugLocationEPNS_11InstructionE(%"class.llvm::IRBuilderBase"* %22, %"class.llvm::Instruction"* %24)
  %25 = load %"class.llvm::CastInst"*, %"class.llvm::CastInst"** %5, align 8
  ret %"class.llvm::CastInst"* %25
}

declare %"class.llvm::CastInst"* @_ZN4llvm8CastInst6CreateENS_11Instruction7CastOpsEPNS_5ValueEPNS_4TypeERKNS_5TwineEPS1_(i32, %"class.llvm::Value"*, %"class.llvm::Type"*, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::Instruction"*) #1

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm3isaINS_8ConstantEPNS_5ValueEEEbRKT0_(%"class.llvm::Value"** dereferenceable(8)) #7 {
  %2 = alloca %"class.llvm::Value"**, align 8
  store %"class.llvm::Value"** %0, %"class.llvm::Value"*** %2, align 8
  %3 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_8ConstantEKPNS_5ValueEPKS2_E4doitERS4_(%"class.llvm::Value"** dereferenceable(8) %3)
  ret i1 %4
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr %"class.llvm::Constant"* @_ZN4llvm4castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Value"*) #7 {
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
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm4castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.16, i32 0, i32 0), i32 236, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.19, i32 0, i32 0)) #17
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
define linkonce_odr zeroext i1 @_ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_(%"class.llvm::Value"*) #7 align 2 {
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
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.16, i32 0, i32 0), i32 95, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.17, i32 0, i32 0)) #17
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
define linkonce_odr zeroext i1 @_ZN4llvm8isa_implINS_8ConstantENS_5ValueEvE4doitERKS2_(%"class.llvm::Value"* dereferenceable(32)) #7 align 2 {
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
define linkonce_odr void @_ZNK4llvm24IRBuilderDefaultInserter12InsertHelperEPNS_11InstructionERKNS_5TwineEPNS_10BasicBlockENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEE(%"class.llvm::IRBuilderDefaultInserter"*, %"class.llvm::Instruction"*, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::BasicBlock"*, %"class.llvm::ilist_node_impl.60"*) #0 align 2 {
  %6 = alloca %"class.llvm::ilist_iterator", align 8
  %7 = alloca %"class.llvm::IRBuilderDefaultInserter"*, align 8
  %8 = alloca %"class.llvm::Instruction"*, align 8
  %9 = alloca %"class.llvm::Twine"*, align 8
  %10 = alloca %"class.llvm::BasicBlock"*, align 8
  %11 = alloca %"class.llvm::ilist_iterator", align 8
  %12 = alloca %"class.llvm::ilist_iterator", align 8
  %13 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %6, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.60"* %4, %"class.llvm::ilist_node_impl.60"** %13, align 8
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
  %19 = call dereferenceable(16) %"class.llvm::SymbolTableList.53"* @_ZN4llvm10BasicBlock11getInstListEv(%"class.llvm::BasicBlock"* %18)
  %20 = bitcast %"class.llvm::SymbolTableList.53"* %19 to %"class.llvm::iplist_impl.54"*
  %21 = bitcast %"class.llvm::ilist_iterator"* %11 to i8*
  %22 = bitcast %"class.llvm::ilist_iterator"* %6 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %21, i8* %22, i64 8, i32 8, i1 false)
  %23 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %8, align 8
  %24 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %11, i32 0, i32 0
  %25 = load %"class.llvm::ilist_node_impl.60"*, %"class.llvm::ilist_node_impl.60"** %24, align 8
  %26 = call %"class.llvm::ilist_node_impl.60"* @_ZN4llvm11iplist_implINS_12simple_ilistINS_11InstructionEJEEENS_21SymbolTableListTraitsIS2_EEE6insertENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS2_Lb0ELb0EvEELb0ELb0EEEPS2_(%"class.llvm::iplist_impl.54"* %20, %"class.llvm::ilist_node_impl.60"* %25, %"class.llvm::Instruction"* %23)
  %27 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %12, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.60"* %26, %"class.llvm::ilist_node_impl.60"** %27, align 8
  br label %28

; <label>:28:                                     ; preds = %17, %5
  %29 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %8, align 8
  %30 = bitcast %"class.llvm::Instruction"* %29 to %"class.llvm::Value"*
  %31 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %9, align 8
  call void @_ZN4llvm5Value7setNameERKNS_5TwineE(%"class.llvm::Value"* %30, %"class.llvm::Twine"* dereferenceable(24) %31)
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #6

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
define linkonce_odr dereferenceable(16) %"class.llvm::SymbolTableList.53"* @_ZN4llvm10BasicBlock11getInstListEv(%"class.llvm::BasicBlock"*) #4 align 2 {
  %2 = alloca %"class.llvm::BasicBlock"*, align 8
  store %"class.llvm::BasicBlock"* %0, %"class.llvm::BasicBlock"** %2, align 8
  %3 = load %"class.llvm::BasicBlock"*, %"class.llvm::BasicBlock"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::BasicBlock", %"class.llvm::BasicBlock"* %3, i32 0, i32 2
  ret %"class.llvm::SymbolTableList.53"* %4
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.60"* @_ZN4llvm11iplist_implINS_12simple_ilistINS_11InstructionEJEEENS_21SymbolTableListTraitsIS2_EEE6insertENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS2_Lb0ELb0EvEELb0ELb0EEEPS2_(%"class.llvm::iplist_impl.54"*, %"class.llvm::ilist_node_impl.60"*, %"class.llvm::Instruction"*) #0 align 2 {
  %4 = alloca %"class.llvm::ilist_iterator", align 8
  %5 = alloca %"class.llvm::ilist_iterator", align 8
  %6 = alloca %"class.llvm::iplist_impl.54"*, align 8
  %7 = alloca %"class.llvm::Instruction"*, align 8
  %8 = alloca %"class.llvm::ilist_iterator", align 8
  %9 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %5, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.60"* %1, %"class.llvm::ilist_node_impl.60"** %9, align 8
  store %"class.llvm::iplist_impl.54"* %0, %"class.llvm::iplist_impl.54"** %6, align 8
  store %"class.llvm::Instruction"* %2, %"class.llvm::Instruction"** %7, align 8
  %10 = load %"class.llvm::iplist_impl.54"*, %"class.llvm::iplist_impl.54"** %6, align 8
  %11 = bitcast %"class.llvm::iplist_impl.54"* %10 to %"class.llvm::SymbolTableListTraits.55"*
  %12 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %7, align 8
  call void @_ZN4llvm21SymbolTableListTraitsINS_11InstructionEE13addNodeToListEPS1_(%"class.llvm::SymbolTableListTraits.55"* %11, %"class.llvm::Instruction"* %12)
  %13 = bitcast %"class.llvm::iplist_impl.54"* %10 to %"class.llvm::simple_ilist.57"*
  %14 = bitcast %"class.llvm::ilist_iterator"* %8 to i8*
  %15 = bitcast %"class.llvm::ilist_iterator"* %5 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %14, i8* %15, i64 8, i32 8, i1 false)
  %16 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %7, align 8
  %17 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %8, i32 0, i32 0
  %18 = load %"class.llvm::ilist_node_impl.60"*, %"class.llvm::ilist_node_impl.60"** %17, align 8
  %19 = call %"class.llvm::ilist_node_impl.60"* @_ZN4llvm12simple_ilistINS_11InstructionEJEE6insertENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEERS1_(%"class.llvm::simple_ilist.57"* %13, %"class.llvm::ilist_node_impl.60"* %18, %"class.llvm::Instruction"* dereferenceable(64) %16)
  %20 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %4, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.60"* %19, %"class.llvm::ilist_node_impl.60"** %20, align 8
  %21 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %4, i32 0, i32 0
  %22 = load %"class.llvm::ilist_node_impl.60"*, %"class.llvm::ilist_node_impl.60"** %21, align 8
  ret %"class.llvm::ilist_node_impl.60"* %22
}

declare void @_ZN4llvm5Value7setNameERKNS_5TwineE(%"class.llvm::Value"*, %"class.llvm::Twine"* dereferenceable(24)) #1

declare void @_ZN4llvm21SymbolTableListTraitsINS_11InstructionEE13addNodeToListEPS1_(%"class.llvm::SymbolTableListTraits.55"*, %"class.llvm::Instruction"*) #1

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.60"* @_ZN4llvm12simple_ilistINS_11InstructionEJEE6insertENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEERS1_(%"class.llvm::simple_ilist.57"*, %"class.llvm::ilist_node_impl.60"*, %"class.llvm::Instruction"* dereferenceable(64)) #0 align 2 {
  %4 = alloca %"class.llvm::ilist_iterator", align 8
  %5 = alloca %"class.llvm::ilist_iterator", align 8
  %6 = alloca %"class.llvm::simple_ilist.57"*, align 8
  %7 = alloca %"class.llvm::Instruction"*, align 8
  %8 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %5, i32 0, i32 0
  store %"class.llvm::ilist_node_impl.60"* %1, %"class.llvm::ilist_node_impl.60"** %8, align 8
  store %"class.llvm::simple_ilist.57"* %0, %"class.llvm::simple_ilist.57"** %6, align 8
  store %"class.llvm::Instruction"* %2, %"class.llvm::Instruction"** %7, align 8
  %9 = load %"class.llvm::simple_ilist.57"*, %"class.llvm::simple_ilist.57"** %6, align 8
  %10 = call %"class.llvm::ilist_node_impl.60"* @_ZNK4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EE10getNodePtrEv(%"class.llvm::ilist_iterator"* %5)
  %11 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %7, align 8
  %12 = call %"class.llvm::ilist_node_impl.60"* @_ZN4llvm12ilist_detail18SpecificNodeAccessINS0_12node_optionsINS_11InstructionELb0ELb0EvEEE10getNodePtrEPS3_(%"class.llvm::Instruction"* %11)
  call void @_ZN4llvm10ilist_baseILb0EE12insertBeforeINS_15ilist_node_implINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEEEEEEvRT_SA_(%"class.llvm::ilist_node_impl.60"* dereferenceable(16) %10, %"class.llvm::ilist_node_impl.60"* dereferenceable(16) %12)
  %13 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %7, align 8
  call void @_ZN4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EEC1EPS3_(%"class.llvm::ilist_iterator"* %4, %"class.llvm::Instruction"* %13)
  %14 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %4, i32 0, i32 0
  %15 = load %"class.llvm::ilist_node_impl.60"*, %"class.llvm::ilist_node_impl.60"** %14, align 8
  ret %"class.llvm::ilist_node_impl.60"* %15
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm10ilist_baseILb0EE12insertBeforeINS_15ilist_node_implINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEEEEEEvRT_SA_(%"class.llvm::ilist_node_impl.60"* dereferenceable(16), %"class.llvm::ilist_node_impl.60"* dereferenceable(16)) #0 align 2 {
  %3 = alloca %"class.llvm::ilist_node_impl.60"*, align 8
  %4 = alloca %"class.llvm::ilist_node_impl.60"*, align 8
  store %"class.llvm::ilist_node_impl.60"* %0, %"class.llvm::ilist_node_impl.60"** %3, align 8
  store %"class.llvm::ilist_node_impl.60"* %1, %"class.llvm::ilist_node_impl.60"** %4, align 8
  %5 = load %"class.llvm::ilist_node_impl.60"*, %"class.llvm::ilist_node_impl.60"** %3, align 8
  %6 = bitcast %"class.llvm::ilist_node_impl.60"* %5 to %"class.llvm::ilist_node_base"*
  %7 = load %"class.llvm::ilist_node_impl.60"*, %"class.llvm::ilist_node_impl.60"** %4, align 8
  %8 = bitcast %"class.llvm::ilist_node_impl.60"* %7 to %"class.llvm::ilist_node_base"*
  call void @_ZN4llvm10ilist_baseILb0EE16insertBeforeImplERNS_15ilist_node_baseILb0EEES4_(%"class.llvm::ilist_node_base"* dereferenceable(16) %6, %"class.llvm::ilist_node_base"* dereferenceable(16) %8)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.60"* @_ZNK4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EE10getNodePtrEv(%"class.llvm::ilist_iterator"*) #4 align 2 {
  %2 = alloca %"class.llvm::ilist_iterator"*, align 8
  store %"class.llvm::ilist_iterator"* %0, %"class.llvm::ilist_iterator"** %2, align 8
  %3 = load %"class.llvm::ilist_iterator"*, %"class.llvm::ilist_iterator"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %3, i32 0, i32 0
  %5 = load %"class.llvm::ilist_node_impl.60"*, %"class.llvm::ilist_node_impl.60"** %4, align 8
  ret %"class.llvm::ilist_node_impl.60"* %5
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::ilist_node_impl.60"* @_ZN4llvm12ilist_detail18SpecificNodeAccessINS0_12node_optionsINS_11InstructionELb0ELb0EvEEE10getNodePtrEPS3_(%"class.llvm::Instruction"*) #0 align 2 {
  %2 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"* %0, %"class.llvm::Instruction"** %2, align 8
  %3 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %2, align 8
  %4 = call %"class.llvm::ilist_node_impl.60"* @_ZN4llvm12ilist_detail10NodeAccess10getNodePtrINS0_12node_optionsINS_11InstructionELb0ELb0EvEEEEPNS_15ilist_node_implIT_EENS7_7pointerE(%"class.llvm::Instruction"* %3)
  ret %"class.llvm::ilist_node_impl.60"* %4
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
define linkonce_odr %"class.llvm::ilist_node_impl.60"* @_ZN4llvm12ilist_detail10NodeAccess10getNodePtrINS0_12node_optionsINS_11InstructionELb0ELb0EvEEEEPNS_15ilist_node_implIT_EENS7_7pointerE(%"class.llvm::Instruction"*) #4 align 2 {
  %2 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"* %0, %"class.llvm::Instruction"** %2, align 8
  %3 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %2, align 8
  %4 = icmp eq %"class.llvm::Instruction"* %3, null
  br i1 %4, label %9, label %5

; <label>:5:                                      ; preds = %1
  %6 = bitcast %"class.llvm::Instruction"* %3 to i8*
  %7 = getelementptr inbounds i8, i8* %6, i64 32
  %8 = bitcast i8* %7 to %"class.llvm::ilist_node_impl.60"*
  br label %9

; <label>:9:                                      ; preds = %5, %1
  %10 = phi %"class.llvm::ilist_node_impl.60"* [ %8, %5 ], [ null, %1 ]
  ret %"class.llvm::ilist_node_impl.60"* %10
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm14ilist_iteratorINS_12ilist_detail12node_optionsINS_11InstructionELb0ELb0EvEELb0ELb0EEC2EPS3_(%"class.llvm::ilist_iterator"*, %"class.llvm::Instruction"*) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::ilist_iterator"*, align 8
  %4 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::ilist_iterator"* %0, %"class.llvm::ilist_iterator"** %3, align 8
  store %"class.llvm::Instruction"* %1, %"class.llvm::Instruction"** %4, align 8
  %5 = load %"class.llvm::ilist_iterator"*, %"class.llvm::ilist_iterator"** %3, align 8
  %6 = bitcast %"class.llvm::ilist_iterator"* %5 to %"struct.llvm::ilist_detail::SpecificNodeAccess.58"*
  %7 = getelementptr inbounds %"class.llvm::ilist_iterator", %"class.llvm::ilist_iterator"* %5, i32 0, i32 0
  %8 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %4, align 8
  %9 = call %"class.llvm::ilist_node_impl.60"* @_ZN4llvm12ilist_detail18SpecificNodeAccessINS0_12node_optionsINS_11InstructionELb0ELb0EvEEE10getNodePtrEPS3_(%"class.llvm::Instruction"* %8)
  store %"class.llvm::ilist_node_impl.60"* %9, %"class.llvm::ilist_node_impl.60"** %7, align 8
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
define linkonce_odr void @_ZN4llvm8DebugLocC1ERKS0_(%"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"* dereferenceable(8)) unnamed_addr #7 align 2 {
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
define linkonce_odr void @_ZN4llvm8DebugLocD1Ev(%"class.llvm::DebugLoc"*) unnamed_addr #8 align 2 {
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
define linkonce_odr dereferenceable(8) %"class.llvm::DebugLoc"* @_ZN4llvm8DebugLocaSEOS0_(%"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"* dereferenceable(8)) #7 align 2 {
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
  call void @__assert_rtn(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @__func__._ZN4llvm13TrackingMDRef7retrackERS0_, i32 0, i32 0), i8* getelementptr inbounds ([63 x i8], [63 x i8]* @.str.21, i32 0, i32 0), i32 88, i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str.22, i32 0, i32 0)) #17
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
define linkonce_odr void @_ZN4llvm8DebugLocC2ERKS0_(%"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"* dereferenceable(8)) unnamed_addr #7 align 2 {
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
  %3 = alloca %"class.llvm::PointerUnion.234", align 8
  store %"class.llvm::Metadata"** %0, %"class.llvm::Metadata"*** %2, align 8
  %4 = load %"class.llvm::Metadata"**, %"class.llvm::Metadata"*** %2, align 8
  %5 = bitcast %"class.llvm::Metadata"** %4 to i8*
  %6 = load %"class.llvm::Metadata"**, %"class.llvm::Metadata"*** %2, align 8
  %7 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %6, align 8
  call void @_ZN4llvm12PointerUnionIPNS_15MetadataAsValueEPNS_8MetadataEEC1ES4_(%"class.llvm::PointerUnion.234"* %3, %"class.llvm::Metadata"* null)
  %8 = getelementptr inbounds %"class.llvm::PointerUnion.234", %"class.llvm::PointerUnion.234"* %3, i32 0, i32 0
  %9 = getelementptr inbounds %"class.llvm::PointerIntPair.235", %"class.llvm::PointerIntPair.235"* %8, i32 0, i32 0
  %10 = load i64, i64* %9, align 8
  %11 = call zeroext i1 @_ZN4llvm16MetadataTracking5trackEPvRNS_8MetadataENS_12PointerUnionIPNS_15MetadataAsValueEPS2_EE(i8* %5, %"class.llvm::Metadata"* dereferenceable(8) %7, i64 %10)
  ret i1 %11
}

declare zeroext i1 @_ZN4llvm16MetadataTracking5trackEPvRNS_8MetadataENS_12PointerUnionIPNS_15MetadataAsValueEPS2_EE(i8*, %"class.llvm::Metadata"* dereferenceable(8), i64) #1

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm12PointerUnionIPNS_15MetadataAsValueEPNS_8MetadataEEC1ES4_(%"class.llvm::PointerUnion.234"*, %"class.llvm::Metadata"*) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::PointerUnion.234"*, align 8
  %4 = alloca %"class.llvm::Metadata"*, align 8
  store %"class.llvm::PointerUnion.234"* %0, %"class.llvm::PointerUnion.234"** %3, align 8
  store %"class.llvm::Metadata"* %1, %"class.llvm::Metadata"** %4, align 8
  %5 = load %"class.llvm::PointerUnion.234"*, %"class.llvm::PointerUnion.234"** %3, align 8
  %6 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %4, align 8
  call void @_ZN4llvm12PointerUnionIPNS_15MetadataAsValueEPNS_8MetadataEEC2ES4_(%"class.llvm::PointerUnion.234"* %5, %"class.llvm::Metadata"* %6)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm12PointerUnionIPNS_15MetadataAsValueEPNS_8MetadataEEC2ES4_(%"class.llvm::PointerUnion.234"*, %"class.llvm::Metadata"*) unnamed_addr #0 align 2 {
  %3 = alloca %"class.llvm::PointerUnion.234"*, align 8
  %4 = alloca %"class.llvm::Metadata"*, align 8
  store %"class.llvm::PointerUnion.234"* %0, %"class.llvm::PointerUnion.234"** %3, align 8
  store %"class.llvm::Metadata"* %1, %"class.llvm::Metadata"** %4, align 8
  %5 = load %"class.llvm::PointerUnion.234"*, %"class.llvm::PointerUnion.234"** %3, align 8
  %6 = getelementptr inbounds %"class.llvm::PointerUnion.234", %"class.llvm::PointerUnion.234"* %5, i32 0, i32 0
  %7 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %4, align 8
  %8 = call i8* @_ZN4llvm21PointerLikeTypeTraitsIPNS_8MetadataEE16getAsVoidPointerES2_(%"class.llvm::Metadata"* %7)
  call void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEEC1ES1_b(%"class.llvm::PointerIntPair.235"* %6, i8* %8, i1 zeroext true)
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr i8* @_ZN4llvm21PointerLikeTypeTraitsIPNS_8MetadataEE16getAsVoidPointerES2_(%"class.llvm::Metadata"*) #8 align 2 {
  %2 = alloca %"class.llvm::Metadata"*, align 8
  store %"class.llvm::Metadata"* %0, %"class.llvm::Metadata"** %2, align 8
  %3 = load %"class.llvm::Metadata"*, %"class.llvm::Metadata"** %2, align 8
  %4 = bitcast %"class.llvm::Metadata"* %3 to i8*
  ret i8* %4
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEEC1ES1_b(%"class.llvm::PointerIntPair.235"*, i8*, i1 zeroext) unnamed_addr #0 align 2 {
  %4 = alloca %"class.llvm::PointerIntPair.235"*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i8, align 1
  store %"class.llvm::PointerIntPair.235"* %0, %"class.llvm::PointerIntPair.235"** %4, align 8
  store i8* %1, i8** %5, align 8
  %7 = zext i1 %2 to i8
  store i8 %7, i8* %6, align 1
  %8 = load %"class.llvm::PointerIntPair.235"*, %"class.llvm::PointerIntPair.235"** %4, align 8
  %9 = load i8*, i8** %5, align 8
  %10 = load i8, i8* %6, align 1
  %11 = trunc i8 %10 to i1
  call void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEEC2ES1_b(%"class.llvm::PointerIntPair.235"* %8, i8* %9, i1 zeroext %11)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEEC2ES1_b(%"class.llvm::PointerIntPair.235"*, i8*, i1 zeroext) unnamed_addr #0 align 2 {
  %4 = alloca %"class.llvm::PointerIntPair.235"*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i8, align 1
  store %"class.llvm::PointerIntPair.235"* %0, %"class.llvm::PointerIntPair.235"** %4, align 8
  store i8* %1, i8** %5, align 8
  %7 = zext i1 %2 to i8
  store i8 %7, i8* %6, align 1
  %8 = load %"class.llvm::PointerIntPair.235"*, %"class.llvm::PointerIntPair.235"** %4, align 8
  %9 = load i8*, i8** %5, align 8
  %10 = load i8, i8* %6, align 1
  %11 = trunc i8 %10 to i1
  call void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEE16setPointerAndIntES1_b(%"class.llvm::PointerIntPair.235"* %8, i8* %9, i1 zeroext %11)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm14PointerIntPairIPvLj1EbNS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEENS_18PointerIntPairInfoIS1_Lj1ES7_EEE16setPointerAndIntES1_b(%"class.llvm::PointerIntPair.235"*, i8*, i1 zeroext) #0 align 2 {
  %4 = alloca %"class.llvm::PointerIntPair.235"*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i8, align 1
  store %"class.llvm::PointerIntPair.235"* %0, %"class.llvm::PointerIntPair.235"** %4, align 8
  store i8* %1, i8** %5, align 8
  %7 = zext i1 %2 to i8
  store i8 %7, i8* %6, align 1
  %8 = load %"class.llvm::PointerIntPair.235"*, %"class.llvm::PointerIntPair.235"** %4, align 8
  %9 = load i8*, i8** %5, align 8
  %10 = call i64 @_ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE13updatePointerElS1_(i64 0, i8* %9)
  %11 = load i8, i8* %6, align 1
  %12 = trunc i8 %11 to i1
  %13 = zext i1 %12 to i64
  %14 = call i64 @_ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE9updateIntEll(i64 %10, i64 %13)
  %15 = getelementptr inbounds %"class.llvm::PointerIntPair.235", %"class.llvm::PointerIntPair.235"* %8, i32 0, i32 0
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
  call void @__assert_rtn(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @__func__._ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE9updateIntEll, i32 0, i32 0), i8* getelementptr inbounds ([65 x i8], [65 x i8]* @.str.24, i32 0, i32 0), i32 167, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.25, i32 0, i32 0)) #17
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
  call void @__assert_rtn(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @__func__._ZN4llvm18PointerIntPairInfoIPvLj1ENS_22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEEEE13updatePointerElS1_, i32 0, i32 0), i8* getelementptr inbounds ([65 x i8], [65 x i8]* @.str.24, i32 0, i32 0), i32 160, i8* getelementptr inbounds ([74 x i8], [74 x i8]* @.str.27, i32 0, i32 0)) #17
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
define linkonce_odr i8* @_ZN4llvm22PointerUnionUIntTraitsIPNS_15MetadataAsValueEPNS_8MetadataEE16getAsVoidPointerEPv(i8*) #8 align 2 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = load i8*, i8** %2, align 8
  ret i8* %3
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm8DebugLocD2Ev(%"class.llvm::DebugLoc"*) unnamed_addr #8 align 2 {
  %2 = alloca %"class.llvm::DebugLoc"*, align 8
  store %"class.llvm::DebugLoc"* %0, %"class.llvm::DebugLoc"** %2, align 8
  %3 = load %"class.llvm::DebugLoc"*, %"class.llvm::DebugLoc"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::DebugLoc", %"class.llvm::DebugLoc"* %3, i32 0, i32 0
  call void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEED1Ev(%"class.llvm::TypedTrackingMDRef"* %4) #14
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEED1Ev(%"class.llvm::TypedTrackingMDRef"*) unnamed_addr #8 align 2 {
  %2 = alloca %"class.llvm::TypedTrackingMDRef"*, align 8
  store %"class.llvm::TypedTrackingMDRef"* %0, %"class.llvm::TypedTrackingMDRef"** %2, align 8
  %3 = load %"class.llvm::TypedTrackingMDRef"*, %"class.llvm::TypedTrackingMDRef"** %2, align 8
  call void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEED2Ev(%"class.llvm::TypedTrackingMDRef"* %3) #14
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN4llvm18TypedTrackingMDRefINS_6MDNodeEED2Ev(%"class.llvm::TypedTrackingMDRef"*) unnamed_addr #8 align 2 {
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
  call void @__clang_call_terminate(i8* %7) #15
  unreachable
}

declare dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEPKc(%"class.std::__1::basic_string"*, i8*) #1

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"*, i8*, i64) #1

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8*) #7 align 2 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = load i8*, i8** %2, align 8
  %4 = call i64 @strlen(i8* %3)
  ret i64 %4
}

declare i64 @strlen(i8*) #1

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE21__construct_node_hashIRKNS_21piecewise_construct_tEJNS_5tupleIJOS7_EEENSQ_IJEEEEEENS_10unique_ptrINS_11__hash_nodeISB_PvEENS_22__hash_node_destructorINS5_ISX_EEEEEEmOT_DpOT0_(%"class.std::__1::unique_ptr.239"* noalias sret, %"class.std::__1::__hash_table.213"*, i64, %"struct.std::__1::piecewise_construct_t"* dereferenceable(1), %"class.std::__1::tuple"* dereferenceable(8), %"class.std::__1::tuple.236"* dereferenceable(1)) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %7 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %8 = alloca %"class.std::__1::tuple"*, align 8
  %9 = alloca %"class.std::__1::tuple.236"*, align 8
  %10 = alloca %"class.std::__1::tuple"*, align 8
  %11 = alloca %"class.std::__1::tuple.236"*, align 8
  %12 = alloca %"class.std::__1::tuple"*, align 8
  %13 = alloca %"class.std::__1::tuple.236"*, align 8
  %14 = alloca %"class.std::__1::tuple"*, align 8
  %15 = alloca %"class.std::__1::tuple.236"*, align 8
  %16 = alloca %"class.std::__1::basic_string"*, align 8
  %17 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %18 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %19 = alloca %"class.std::__1::basic_string"*, align 8
  %20 = alloca [3 x i64]*, align 8
  %21 = alloca i32, align 4
  %22 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %23 = alloca %"class.std::__1::basic_string"*, align 8
  %24 = alloca %"class.std::__1::basic_string"*, align 8
  %25 = alloca %"class.std::__1::basic_string"*, align 8
  %26 = alloca %"class.std::__1::basic_string"*, align 8
  %27 = alloca %"class.std::__1::__tuple_leaf"*, align 8
  %28 = alloca %"class.std::__1::tuple"*, align 8
  %29 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %30 = alloca %"struct.std::__1::__tuple_indices", align 1
  %31 = alloca %"struct.std::__1::__tuple_indices.243", align 1
  %32 = alloca %"struct.std::__1::pair.237"*, align 8
  %33 = alloca %"class.std::__1::tuple"*, align 8
  %34 = alloca %"class.std::__1::tuple.236"*, align 8
  %35 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %36 = alloca %"struct.std::__1::__tuple_indices", align 1
  %37 = alloca %"struct.std::__1::__tuple_indices.243", align 1
  %38 = alloca %"struct.std::__1::pair.237"*, align 8
  %39 = alloca %"class.std::__1::tuple"*, align 8
  %40 = alloca %"class.std::__1::tuple.236"*, align 8
  %41 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %42 = alloca %"class.std::__1::tuple", align 8
  %43 = alloca %"class.std::__1::tuple.236", align 1
  %44 = alloca %"struct.std::__1::pair.237"*, align 8
  %45 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %46 = alloca %"struct.std::__1::__tuple_indices", align 1
  %47 = alloca %"struct.std::__1::__tuple_indices.243", align 1
  %48 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %49 = alloca %"class.std::__1::allocator.225"*, align 8
  %50 = alloca %"struct.std::__1::pair.237"*, align 8
  %51 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %52 = alloca %"class.std::__1::tuple"*, align 8
  %53 = alloca %"class.std::__1::tuple.236"*, align 8
  %54 = alloca %"struct.std::__1::piecewise_construct_t", align 1
  %55 = alloca %"class.std::__1::tuple", align 8
  %56 = alloca %"class.std::__1::tuple.236", align 1
  %57 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %58 = alloca %"struct.std::__1::integral_constant", align 1
  %59 = alloca %"class.std::__1::allocator.225"*, align 8
  %60 = alloca %"struct.std::__1::pair.237"*, align 8
  %61 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %62 = alloca %"class.std::__1::tuple"*, align 8
  %63 = alloca %"class.std::__1::tuple.236"*, align 8
  %64 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %65 = alloca %"class.std::__1::allocator.225"*, align 8
  %66 = alloca %"struct.std::__1::pair.237"*, align 8
  %67 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %68 = alloca %"class.std::__1::tuple"*, align 8
  %69 = alloca %"class.std::__1::tuple.236"*, align 8
  %70 = alloca %"struct.std::__1::integral_constant", align 1
  %71 = alloca %"struct.std::__1::__has_construct", align 1
  %72 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %73 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %74 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %75 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %76 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %77 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %78 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %79 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %80 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %81 = alloca %"struct.std::__1::pair.237"*, align 8
  %82 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %83 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %84 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %85 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %86 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %87 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %88 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %89 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %90 = alloca i8*, align 8
  %91 = alloca %"class.std::__1::allocator.225"*, align 8
  %92 = alloca %"struct.std::__1::__hash_node"*, align 8
  %93 = alloca i64, align 8
  %94 = alloca %"class.std::__1::allocator.225"*, align 8
  %95 = alloca %"struct.std::__1::__hash_node"*, align 8
  %96 = alloca i64, align 8
  %97 = alloca %"class.std::__1::allocator.225"*, align 8
  %98 = alloca %"struct.std::__1::pair.237"*, align 8
  %99 = alloca %"class.std::__1::allocator.225"*, align 8
  %100 = alloca %"struct.std::__1::pair.237"*, align 8
  %101 = alloca %"struct.std::__1::integral_constant.244", align 1
  %102 = alloca %"struct.std::__1::__has_destroy", align 1
  %103 = alloca %"struct.std::__1::pair.237"*, align 8
  %104 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %105 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %106 = alloca %"struct.std::__1::__hash_node"*, align 8
  %107 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %108 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %109 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %110 = alloca %"struct.std::__1::__hash_node"*, align 8
  %111 = alloca %"struct.std::__1::__hash_node"*, align 8
  %112 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %113 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %114 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %115 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %116 = alloca %"struct.std::__1::__hash_node"**, align 8
  %117 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %118 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %119 = alloca %"struct.std::__1::__hash_node"*, align 8
  %120 = alloca %"struct.std::__1::__hash_node"**, align 8
  %121 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %122 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %123 = alloca %"struct.std::__1::__hash_node"*, align 8
  %124 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %125 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %126 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %127 = alloca %"struct.std::__1::__hash_node"*, align 8
  %128 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %129 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %130 = alloca %"struct.std::__1::__hash_node"*, align 8
  %131 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %132 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %133 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %134 = alloca %"struct.std::__1::__hash_node"*, align 8
  %135 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %136 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %137 = alloca %"class.std::__1::allocator.225"*, align 8
  %138 = alloca i8, align 1
  %139 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %140 = alloca %"class.std::__1::allocator.225"*, align 8
  %141 = alloca i8, align 1
  %142 = alloca i64, align 8
  %143 = alloca %"class.std::__1::allocator.225"*, align 8
  %144 = alloca i64, align 8
  %145 = alloca i8*, align 8
  %146 = alloca %"class.std::__1::allocator.225"*, align 8
  %147 = alloca i64, align 8
  %148 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %149 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %150 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %151 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %152 = alloca i8*, align 8
  %153 = alloca %"class.std::__1::allocator.225"*, align 8
  %154 = alloca %"struct.std::__1::__hash_node"*, align 8
  %155 = alloca i64, align 8
  %156 = alloca %"class.std::__1::allocator.225"*, align 8
  %157 = alloca %"struct.std::__1::__hash_node"*, align 8
  %158 = alloca i64, align 8
  %159 = alloca %"class.std::__1::allocator.225"*, align 8
  %160 = alloca %"struct.std::__1::pair.237"*, align 8
  %161 = alloca %"class.std::__1::allocator.225"*, align 8
  %162 = alloca %"struct.std::__1::pair.237"*, align 8
  %163 = alloca %"struct.std::__1::integral_constant.244", align 1
  %164 = alloca %"struct.std::__1::__has_destroy", align 1
  %165 = alloca %"struct.std::__1::pair.237"*, align 8
  %166 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %167 = alloca %"class.std::__1::__hash_node_destructor"*, align 8
  %168 = alloca %"struct.std::__1::__hash_node"*, align 8
  %169 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.241"*, align 8
  %170 = alloca %"class.std::__1::__compressed_pair.240"*, align 8
  %171 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %172 = alloca %"struct.std::__1::__hash_node"*, align 8
  %173 = alloca %"struct.std::__1::__hash_node"*, align 8
  %174 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %175 = alloca %"class.std::__1::unique_ptr.239"*, align 8
  %176 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.224"*, align 8
  %177 = alloca %"class.std::__1::__compressed_pair.223"*, align 8
  %178 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %179 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %180 = alloca i64, align 8
  %181 = alloca %"struct.std::__1::piecewise_construct_t"*, align 8
  %182 = alloca %"class.std::__1::tuple"*, align 8
  %183 = alloca %"class.std::__1::tuple.236"*, align 8
  %184 = alloca %"class.std::__1::allocator.225"*, align 8
  %185 = alloca i1, align 1
  %186 = alloca %"class.std::__1::__hash_node_destructor", align 8
  %187 = alloca i8*
  %188 = alloca i32
  store %"class.std::__1::__hash_table.213"* %1, %"class.std::__1::__hash_table.213"** %179, align 8
  store i64 %2, i64* %180, align 8
  store %"struct.std::__1::piecewise_construct_t"* %3, %"struct.std::__1::piecewise_construct_t"** %181, align 8
  store %"class.std::__1::tuple"* %4, %"class.std::__1::tuple"** %182, align 8
  store %"class.std::__1::tuple.236"* %5, %"class.std::__1::tuple.236"** %183, align 8
  %189 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %179, align 8
  store %"class.std::__1::__hash_table.213"* %189, %"class.std::__1::__hash_table.213"** %178, align 8
  %190 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %178, align 8
  %191 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %190, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.223"* %191, %"class.std::__1::__compressed_pair.223"** %177, align 8
  %192 = load %"class.std::__1::__compressed_pair.223"*, %"class.std::__1::__compressed_pair.223"** %177, align 8
  %193 = bitcast %"class.std::__1::__compressed_pair.223"* %192 to %"class.std::__1::__libcpp_compressed_pair_imp.224"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.224"* %193, %"class.std::__1::__libcpp_compressed_pair_imp.224"** %176, align 8
  %194 = load %"class.std::__1::__libcpp_compressed_pair_imp.224"*, %"class.std::__1::__libcpp_compressed_pair_imp.224"** %176, align 8
  %195 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.224"* %194 to %"class.std::__1::allocator.225"*
  store %"class.std::__1::allocator.225"* %195, %"class.std::__1::allocator.225"** %184, align 8
  store i1 false, i1* %185, align 1
  %196 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %184, align 8
  store %"class.std::__1::allocator.225"* %196, %"class.std::__1::allocator.225"** %146, align 8
  store i64 1, i64* %147, align 8
  %197 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %146, align 8
  %198 = load i64, i64* %147, align 8
  store %"class.std::__1::allocator.225"* %197, %"class.std::__1::allocator.225"** %143, align 8
  store i64 %198, i64* %144, align 8
  store i8* null, i8** %145, align 8
  %199 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %143, align 8
  %200 = load i64, i64* %144, align 8
  %201 = mul i64 %200, 48
  store i64 %201, i64* %142, align 8
  %202 = load i64, i64* %142, align 8
  %203 = call i8* @_Znwm(i64 %202) #18
  %204 = bitcast i8* %203 to %"struct.std::__1::__hash_node"*
  %205 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %184, align 8
  store %"class.std::__1::__hash_node_destructor"* %186, %"class.std::__1::__hash_node_destructor"** %139, align 8
  store %"class.std::__1::allocator.225"* %205, %"class.std::__1::allocator.225"** %140, align 8
  store i8 0, i8* %141, align 1
  %206 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %139, align 8
  %207 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %140, align 8
  %208 = load i8, i8* %141, align 1
  %209 = trunc i8 %208 to i1
  store %"class.std::__1::__hash_node_destructor"* %206, %"class.std::__1::__hash_node_destructor"** %136, align 8
  store %"class.std::__1::allocator.225"* %207, %"class.std::__1::allocator.225"** %137, align 8
  %210 = zext i1 %209 to i8
  store i8 %210, i8* %138, align 1
  %211 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %136, align 8
  %212 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %211, i32 0, i32 0
  %213 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %137, align 8
  store %"class.std::__1::allocator.225"* %213, %"class.std::__1::allocator.225"** %212, align 8
  %214 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %211, i32 0, i32 1
  %215 = load i8, i8* %138, align 1
  %216 = trunc i8 %215 to i1
  %217 = zext i1 %216 to i8
  store i8 %217, i8* %214, align 8
  store %"class.std::__1::unique_ptr.239"* %0, %"class.std::__1::unique_ptr.239"** %133, align 8
  store %"struct.std::__1::__hash_node"* %204, %"struct.std::__1::__hash_node"** %134, align 8
  store %"class.std::__1::__hash_node_destructor"* %186, %"class.std::__1::__hash_node_destructor"** %135, align 8
  %218 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %133, align 8
  %219 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %134, align 8
  %220 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %135, align 8
  store %"class.std::__1::unique_ptr.239"* %218, %"class.std::__1::unique_ptr.239"** %129, align 8
  store %"struct.std::__1::__hash_node"* %219, %"struct.std::__1::__hash_node"** %130, align 8
  store %"class.std::__1::__hash_node_destructor"* %220, %"class.std::__1::__hash_node_destructor"** %131, align 8
  %221 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %129, align 8
  %222 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %221, i32 0, i32 0
  %223 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %130, align 8
  %224 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %131, align 8
  store %"class.std::__1::__hash_node_destructor"* %224, %"class.std::__1::__hash_node_destructor"** %128, align 8
  %225 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %128, align 8
  %226 = bitcast %"class.std::__1::__hash_node_destructor"* %132 to i8*
  %227 = bitcast %"class.std::__1::__hash_node_destructor"* %225 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %226, i8* %227, i64 16, i32 8, i1 false) #14
  %228 = bitcast %"class.std::__1::__hash_node_destructor"* %132 to { %"class.std::__1::allocator.225"*, i8 }*
  %229 = getelementptr inbounds { %"class.std::__1::allocator.225"*, i8 }, { %"class.std::__1::allocator.225"*, i8 }* %228, i32 0, i32 0
  %230 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %229, align 8
  %231 = getelementptr inbounds { %"class.std::__1::allocator.225"*, i8 }, { %"class.std::__1::allocator.225"*, i8 }* %228, i32 0, i32 1
  %232 = load i8, i8* %231, align 8
  %233 = bitcast %"class.std::__1::__hash_node_destructor"* %125 to { %"class.std::__1::allocator.225"*, i8 }*
  %234 = getelementptr inbounds { %"class.std::__1::allocator.225"*, i8 }, { %"class.std::__1::allocator.225"*, i8 }* %233, i32 0, i32 0
  store %"class.std::__1::allocator.225"* %230, %"class.std::__1::allocator.225"** %234, align 8
  %235 = getelementptr inbounds { %"class.std::__1::allocator.225"*, i8 }, { %"class.std::__1::allocator.225"*, i8 }* %233, i32 0, i32 1
  store i8 %232, i8* %235, align 8
  store %"class.std::__1::__compressed_pair.240"* %222, %"class.std::__1::__compressed_pair.240"** %126, align 8
  store %"struct.std::__1::__hash_node"* %223, %"struct.std::__1::__hash_node"** %127, align 8
  %236 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %126, align 8
  %237 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %127, align 8
  %238 = bitcast %"class.std::__1::__hash_node_destructor"* %125 to { %"class.std::__1::allocator.225"*, i8 }*
  %239 = getelementptr inbounds { %"class.std::__1::allocator.225"*, i8 }, { %"class.std::__1::allocator.225"*, i8 }* %238, i32 0, i32 0
  %240 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %239, align 8
  %241 = getelementptr inbounds { %"class.std::__1::allocator.225"*, i8 }, { %"class.std::__1::allocator.225"*, i8 }* %238, i32 0, i32 1
  %242 = load i8, i8* %241, align 8
  %243 = bitcast %"class.std::__1::__hash_node_destructor"* %121 to { %"class.std::__1::allocator.225"*, i8 }*
  %244 = getelementptr inbounds { %"class.std::__1::allocator.225"*, i8 }, { %"class.std::__1::allocator.225"*, i8 }* %243, i32 0, i32 0
  store %"class.std::__1::allocator.225"* %240, %"class.std::__1::allocator.225"** %244, align 8
  %245 = getelementptr inbounds { %"class.std::__1::allocator.225"*, i8 }, { %"class.std::__1::allocator.225"*, i8 }* %243, i32 0, i32 1
  store i8 %242, i8* %245, align 8
  store %"class.std::__1::__compressed_pair.240"* %236, %"class.std::__1::__compressed_pair.240"** %122, align 8
  store %"struct.std::__1::__hash_node"* %237, %"struct.std::__1::__hash_node"** %123, align 8
  %246 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %122, align 8
  %247 = bitcast %"class.std::__1::__compressed_pair.240"* %246 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"struct.std::__1::__hash_node"** %123, %"struct.std::__1::__hash_node"*** %120, align 8
  %248 = load %"struct.std::__1::__hash_node"**, %"struct.std::__1::__hash_node"*** %120, align 8
  %249 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %248, align 8
  store %"class.std::__1::__hash_node_destructor"* %121, %"class.std::__1::__hash_node_destructor"** %114, align 8
  %250 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %114, align 8
  %251 = bitcast %"class.std::__1::__hash_node_destructor"* %124 to i8*
  %252 = bitcast %"class.std::__1::__hash_node_destructor"* %250 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %251, i8* %252, i64 16, i32 8, i1 false) #14
  %253 = bitcast %"class.std::__1::__hash_node_destructor"* %124 to { %"class.std::__1::allocator.225"*, i8 }*
  %254 = getelementptr inbounds { %"class.std::__1::allocator.225"*, i8 }, { %"class.std::__1::allocator.225"*, i8 }* %253, i32 0, i32 0
  %255 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %254, align 8
  %256 = getelementptr inbounds { %"class.std::__1::allocator.225"*, i8 }, { %"class.std::__1::allocator.225"*, i8 }* %253, i32 0, i32 1
  %257 = load i8, i8* %256, align 8
  %258 = bitcast %"class.std::__1::__hash_node_destructor"* %117 to { %"class.std::__1::allocator.225"*, i8 }*
  %259 = getelementptr inbounds { %"class.std::__1::allocator.225"*, i8 }, { %"class.std::__1::allocator.225"*, i8 }* %258, i32 0, i32 0
  store %"class.std::__1::allocator.225"* %255, %"class.std::__1::allocator.225"** %259, align 8
  %260 = getelementptr inbounds { %"class.std::__1::allocator.225"*, i8 }, { %"class.std::__1::allocator.225"*, i8 }* %258, i32 0, i32 1
  store i8 %257, i8* %260, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %247, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %118, align 8
  store %"struct.std::__1::__hash_node"* %249, %"struct.std::__1::__hash_node"** %119, align 8
  %261 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %118, align 8
  %262 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %261, i32 0, i32 0
  store %"struct.std::__1::__hash_node"** %119, %"struct.std::__1::__hash_node"*** %116, align 8
  %263 = load %"struct.std::__1::__hash_node"**, %"struct.std::__1::__hash_node"*** %116, align 8
  %264 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %263, align 8
  store %"struct.std::__1::__hash_node"* %264, %"struct.std::__1::__hash_node"** %262, align 8
  %265 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %261, i32 0, i32 1
  store %"class.std::__1::__hash_node_destructor"* %117, %"class.std::__1::__hash_node_destructor"** %115, align 8
  %266 = load %"class.std::__1::__hash_node_destructor"*, %"class.std::__1::__hash_node_destructor"** %115, align 8
  %267 = bitcast %"class.std::__1::__hash_node_destructor"* %265 to i8*
  %268 = bitcast %"class.std::__1::__hash_node_destructor"* %266 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %267, i8* %268, i64 16, i32 8, i1 false) #14
  %269 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %184, align 8
  store %"class.std::__1::unique_ptr.239"* %0, %"class.std::__1::unique_ptr.239"** %85, align 8
  %270 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %85, align 8
  %271 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %270, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %271, %"class.std::__1::__compressed_pair.240"** %84, align 8
  %272 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %84, align 8
  %273 = bitcast %"class.std::__1::__compressed_pair.240"* %272 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %273, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %83, align 8
  %274 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %83, align 8
  %275 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %274, i32 0, i32 0
  %276 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %275, align 8
  %277 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %276, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %277, %"union.std::__1::__hash_value_type"** %82, align 8
  %278 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %82, align 8
  %279 = bitcast %"union.std::__1::__hash_value_type"* %278 to %"struct.std::__1::pair.237"*
  store %"struct.std::__1::pair.237"* %279, %"struct.std::__1::pair.237"** %81, align 8
  %280 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %81, align 8
  %281 = bitcast %"struct.std::__1::pair.237"* %280 to i8*
  br label %282

; <label>:282:                                    ; preds = %6
  %283 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %181, align 8
  store %"struct.std::__1::piecewise_construct_t"* %283, %"struct.std::__1::piecewise_construct_t"** %7, align 8
  %284 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %7, align 8
  %285 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %182, align 8
  store %"class.std::__1::tuple"* %285, %"class.std::__1::tuple"** %8, align 8
  %286 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %8, align 8
  %287 = load %"class.std::__1::tuple.236"*, %"class.std::__1::tuple.236"** %183, align 8
  store %"class.std::__1::tuple.236"* %287, %"class.std::__1::tuple.236"** %9, align 8
  %288 = load %"class.std::__1::tuple.236"*, %"class.std::__1::tuple.236"** %9, align 8
  store %"class.std::__1::allocator.225"* %269, %"class.std::__1::allocator.225"** %65, align 8
  store %"struct.std::__1::pair.237"* %280, %"struct.std::__1::pair.237"** %66, align 8
  store %"struct.std::__1::piecewise_construct_t"* %284, %"struct.std::__1::piecewise_construct_t"** %67, align 8
  store %"class.std::__1::tuple"* %286, %"class.std::__1::tuple"** %68, align 8
  store %"class.std::__1::tuple.236"* %288, %"class.std::__1::tuple.236"** %69, align 8
  %289 = bitcast %"struct.std::__1::__has_construct"* %71 to %"struct.std::__1::integral_constant"*
  %290 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %65, align 8
  %291 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %66, align 8
  %292 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %67, align 8
  store %"struct.std::__1::piecewise_construct_t"* %292, %"struct.std::__1::piecewise_construct_t"** %64, align 8
  %293 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %64, align 8
  %294 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %68, align 8
  store %"class.std::__1::tuple"* %294, %"class.std::__1::tuple"** %10, align 8
  %295 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %10, align 8
  %296 = load %"class.std::__1::tuple.236"*, %"class.std::__1::tuple.236"** %69, align 8
  store %"class.std::__1::tuple.236"* %296, %"class.std::__1::tuple.236"** %11, align 8
  %297 = load %"class.std::__1::tuple.236"*, %"class.std::__1::tuple.236"** %11, align 8
  store %"class.std::__1::allocator.225"* %290, %"class.std::__1::allocator.225"** %59, align 8
  store %"struct.std::__1::pair.237"* %291, %"struct.std::__1::pair.237"** %60, align 8
  store %"struct.std::__1::piecewise_construct_t"* %293, %"struct.std::__1::piecewise_construct_t"** %61, align 8
  store %"class.std::__1::tuple"* %295, %"class.std::__1::tuple"** %62, align 8
  store %"class.std::__1::tuple.236"* %297, %"class.std::__1::tuple.236"** %63, align 8
  %298 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %59, align 8
  %299 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %60, align 8
  %300 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %61, align 8
  store %"struct.std::__1::piecewise_construct_t"* %300, %"struct.std::__1::piecewise_construct_t"** %57, align 8
  %301 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %57, align 8
  %302 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %62, align 8
  store %"class.std::__1::tuple"* %302, %"class.std::__1::tuple"** %12, align 8
  %303 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %12, align 8
  %304 = load %"class.std::__1::tuple.236"*, %"class.std::__1::tuple.236"** %63, align 8
  store %"class.std::__1::tuple.236"* %304, %"class.std::__1::tuple.236"** %13, align 8
  %305 = load %"class.std::__1::tuple.236"*, %"class.std::__1::tuple.236"** %13, align 8
  store %"class.std::__1::allocator.225"* %298, %"class.std::__1::allocator.225"** %49, align 8
  store %"struct.std::__1::pair.237"* %299, %"struct.std::__1::pair.237"** %50, align 8
  store %"struct.std::__1::piecewise_construct_t"* %301, %"struct.std::__1::piecewise_construct_t"** %51, align 8
  store %"class.std::__1::tuple"* %303, %"class.std::__1::tuple"** %52, align 8
  store %"class.std::__1::tuple.236"* %305, %"class.std::__1::tuple.236"** %53, align 8
  %306 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %49, align 8
  %307 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %50, align 8
  %308 = bitcast %"struct.std::__1::pair.237"* %307 to i8*
  %309 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %51, align 8
  store %"struct.std::__1::piecewise_construct_t"* %309, %"struct.std::__1::piecewise_construct_t"** %48, align 8
  %310 = load %"struct.std::__1::piecewise_construct_t"*, %"struct.std::__1::piecewise_construct_t"** %48, align 8
  %311 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %52, align 8
  store %"class.std::__1::tuple"* %311, %"class.std::__1::tuple"** %14, align 8
  %312 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %14, align 8
  %313 = bitcast %"class.std::__1::tuple"* %55 to i8*
  %314 = bitcast %"class.std::__1::tuple"* %312 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %313, i8* %314, i64 8, i32 8, i1 false)
  %315 = load %"class.std::__1::tuple.236"*, %"class.std::__1::tuple.236"** %53, align 8
  store %"class.std::__1::tuple.236"* %315, %"class.std::__1::tuple.236"** %15, align 8
  %316 = load %"class.std::__1::tuple.236"*, %"class.std::__1::tuple.236"** %15, align 8
  %317 = getelementptr inbounds %"class.std::__1::tuple", %"class.std::__1::tuple"* %55, i32 0, i32 0
  %318 = getelementptr inbounds %"struct.std::__1::__tuple_impl", %"struct.std::__1::__tuple_impl"* %317, i32 0, i32 0
  %319 = getelementptr inbounds %"class.std::__1::__tuple_leaf", %"class.std::__1::__tuple_leaf"* %318, i32 0, i32 0
  %320 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %319, align 8
  %321 = getelementptr inbounds %"class.std::__1::tuple", %"class.std::__1::tuple"* %42, i32 0, i32 0
  %322 = getelementptr inbounds %"struct.std::__1::__tuple_impl", %"struct.std::__1::__tuple_impl"* %321, i32 0, i32 0
  %323 = getelementptr inbounds %"class.std::__1::__tuple_leaf", %"class.std::__1::__tuple_leaf"* %322, i32 0, i32 0
  store %"class.std::__1::basic_string"* %320, %"class.std::__1::basic_string"** %323, align 8
  store %"struct.std::__1::pair.237"* %307, %"struct.std::__1::pair.237"** %44, align 8
  %324 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %44, align 8
  store %"struct.std::__1::pair.237"* %324, %"struct.std::__1::pair.237"** %38, align 8
  store %"class.std::__1::tuple"* %42, %"class.std::__1::tuple"** %39, align 8
  store %"class.std::__1::tuple.236"* %43, %"class.std::__1::tuple.236"** %40, align 8
  %325 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %38, align 8
  %326 = load %"class.std::__1::tuple"*, %"class.std::__1::tuple"** %39, align 8
  %327 = load %"class.std::__1::tuple.236"*, %"class.std::__1::tuple.236"** %40, align 8
  store %"struct.std::__1::pair.237"* %325, %"struct.std::__1::pair.237"** %32, align 8
  store %"class.std::__1::tuple"* %326, %"class.std::__1::tuple"** %33, align 8
  store %"class.std::__1::tuple.236"* %327, %"class.std::__1::tuple.236"** %34, align 8
  %328 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %32, align 8
  %329 = getelementptr inbounds %"struct.std::__1::pair.237", %"struct.std::__1::pair.237"* %328, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair"* %344, %"class.std::__1::__compressed_pair"** %22, align 8
  %345 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8
  %346 = bitcast %"class.std::__1::__compressed_pair"* %342 to i8*
  %347 = bitcast %"class.std::__1::__compressed_pair"* %345 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %346, i8* %347, i64 24, i32 8, i1 false) #14
  %348 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8
  store %"class.std::__1::basic_string"* %348, %"class.std::__1::basic_string"** %19, align 8
  %349 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %19, align 8
  %350 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %349, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %350, %"class.std::__1::__compressed_pair"** %18, align 8
  %351 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %18, align 8
  %352 = bitcast %"class.std::__1::__compressed_pair"* %351 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %352, %"class.std::__1::__libcpp_compressed_pair_imp"** %17, align 8
  %353 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %17, align 8
  %354 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %353, i32 0, i32 0
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
  %369 = getelementptr inbounds %"struct.std::__1::pair.237", %"struct.std::__1::pair.237"* %328, i32 0, i32 1
  store %"class.llvm::Constant"* null, %"class.llvm::Constant"** %369, align 8
  br label %370

; <label>:370:                                    ; preds = %368
  store %"class.std::__1::unique_ptr.239"* %0, %"class.std::__1::unique_ptr.239"** %74, align 8
  %371 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %74, align 8
  %372 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %371, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %372, %"class.std::__1::__compressed_pair.240"** %73, align 8
  %373 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %73, align 8
  %374 = bitcast %"class.std::__1::__compressed_pair.240"* %373 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %374, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %72, align 8
  %375 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %72, align 8
  %376 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %375, i32 0, i32 1
  %377 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %376, i32 0, i32 1
  store i8 1, i8* %377, align 8
  %378 = load i64, i64* %180, align 8
  store %"class.std::__1::unique_ptr.239"* %0, %"class.std::__1::unique_ptr.239"** %77, align 8
  %379 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %77, align 8
  %380 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %379, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %380, %"class.std::__1::__compressed_pair.240"** %76, align 8
  %381 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %76, align 8
  %382 = bitcast %"class.std::__1::__compressed_pair.240"* %381 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %382, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %75, align 8
  %383 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %75, align 8
  %384 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %383, i32 0, i32 0
  %385 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %384, align 8
  %386 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %385, i32 0, i32 1
  store i64 %378, i64* %386, align 8
  store %"class.std::__1::unique_ptr.239"* %0, %"class.std::__1::unique_ptr.239"** %80, align 8
  %387 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %80, align 8
  %388 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %387, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %388, %"class.std::__1::__compressed_pair.240"** %79, align 8
  %389 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %79, align 8
  %390 = bitcast %"class.std::__1::__compressed_pair.240"* %389 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %390, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %78, align 8
  %391 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %78, align 8
  %392 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %391, i32 0, i32 0
  %393 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %392, align 8
  %394 = bitcast %"struct.std::__1::__hash_node"* %393 to %"struct.std::__1::__hash_node_base.217"*
  %395 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %394, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.217"* null, %"struct.std::__1::__hash_node_base.217"** %395, align 8
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
  store %"class.std::__1::unique_ptr.239"* %0, %"class.std::__1::unique_ptr.239"** %113, align 8
  %401 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %113, align 8
  store %"class.std::__1::unique_ptr.239"* %401, %"class.std::__1::unique_ptr.239"** %112, align 8
  %402 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %112, align 8
  store %"class.std::__1::unique_ptr.239"* %402, %"class.std::__1::unique_ptr.239"** %109, align 8
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %110, align 8
  %403 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %109, align 8
  %404 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %403, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %404, %"class.std::__1::__compressed_pair.240"** %108, align 8
  %405 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %108, align 8
  %406 = bitcast %"class.std::__1::__compressed_pair.240"* %405 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %406, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %107, align 8
  %407 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %107, align 8
  %408 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %407, i32 0, i32 0
  %409 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %408, align 8
  store %"struct.std::__1::__hash_node"* %409, %"struct.std::__1::__hash_node"** %111, align 8
  %410 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %110, align 8
  %411 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %403, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %411, %"class.std::__1::__compressed_pair.240"** %89, align 8
  %412 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %89, align 8
  %413 = bitcast %"class.std::__1::__compressed_pair.240"* %412 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %413, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %88, align 8
  %414 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %88, align 8
  %415 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %414, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* %410, %"struct.std::__1::__hash_node"** %415, align 8
  %416 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %111, align 8
  %417 = icmp ne %"struct.std::__1::__hash_node"* %416, null
  br i1 %417, label %418, label %457

; <label>:418:                                    ; preds = %397
  %419 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %403, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %419, %"class.std::__1::__compressed_pair.240"** %87, align 8
  %420 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %87, align 8
  %421 = bitcast %"class.std::__1::__compressed_pair.240"* %420 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %421, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %86, align 8
  %422 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %86, align 8
  %423 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %422, i32 0, i32 1
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
  %431 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %430, align 8
  %432 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %106, align 8
  %433 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %432, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %433, %"union.std::__1::__hash_value_type"** %104, align 8
  %434 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %104, align 8
  %435 = bitcast %"union.std::__1::__hash_value_type"* %434 to %"struct.std::__1::pair.237"*
  store %"struct.std::__1::pair.237"* %435, %"struct.std::__1::pair.237"** %103, align 8
  %436 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %103, align 8
  %437 = bitcast %"struct.std::__1::pair.237"* %436 to i8*
  store %"class.std::__1::allocator.225"* %431, %"class.std::__1::allocator.225"** %99, align 8
  store %"struct.std::__1::pair.237"* %436, %"struct.std::__1::pair.237"** %100, align 8
  %438 = bitcast %"struct.std::__1::__has_destroy"* %102 to %"struct.std::__1::integral_constant.244"*
  %439 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %99, align 8
  %440 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %100, align 8
  store %"class.std::__1::allocator.225"* %439, %"class.std::__1::allocator.225"** %97, align 8
  store %"struct.std::__1::pair.237"* %440, %"struct.std::__1::pair.237"** %98, align 8
  %441 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %98, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEED1Ev(%"struct.std::__1::pair.237"* %441) #14
  br label %442

; <label>:442:                                    ; preds = %429, %418
  %443 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %106, align 8
  %444 = icmp ne %"struct.std::__1::__hash_node"* %443, null
  br i1 %444, label %445, label %456

; <label>:445:                                    ; preds = %442
  %446 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %425, i32 0, i32 0
  %447 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %446, align 8
  %448 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %106, align 8
  store %"class.std::__1::allocator.225"* %447, %"class.std::__1::allocator.225"** %94, align 8
  store %"struct.std::__1::__hash_node"* %448, %"struct.std::__1::__hash_node"** %95, align 8
  store i64 1, i64* %96, align 8
  %449 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %94, align 8
  %450 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %95, align 8
  %451 = load i64, i64* %96, align 8
  store %"class.std::__1::allocator.225"* %449, %"class.std::__1::allocator.225"** %91, align 8
  store %"struct.std::__1::__hash_node"* %450, %"struct.std::__1::__hash_node"** %92, align 8
  store i64 %451, i64* %93, align 8
  %452 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %91, align 8
  %453 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %92, align 8
  %454 = bitcast %"struct.std::__1::__hash_node"* %453 to i8*
  store i8* %454, i8** %90, align 8
  %455 = load i8*, i8** %90, align 8
  call void @_ZdlPv(i8* %455) #16
  br label %456

; <label>:456:                                    ; preds = %445, %442
  br label %457

; <label>:457:                                    ; preds = %397, %456
  br label %517

; <label>:458:                                    ; preds = %370
  store %"class.std::__1::unique_ptr.239"* %0, %"class.std::__1::unique_ptr.239"** %175, align 8
  %459 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %175, align 8
  store %"class.std::__1::unique_ptr.239"* %459, %"class.std::__1::unique_ptr.239"** %174, align 8
  %460 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %174, align 8
  store %"class.std::__1::unique_ptr.239"* %460, %"class.std::__1::unique_ptr.239"** %171, align 8
  store %"struct.std::__1::__hash_node"* null, %"struct.std::__1::__hash_node"** %172, align 8
  %461 = load %"class.std::__1::unique_ptr.239"*, %"class.std::__1::unique_ptr.239"** %171, align 8
  %462 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %461, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %462, %"class.std::__1::__compressed_pair.240"** %170, align 8
  %463 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %170, align 8
  %464 = bitcast %"class.std::__1::__compressed_pair.240"* %463 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %464, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %169, align 8
  %465 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %169, align 8
  %466 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %465, i32 0, i32 0
  %467 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %466, align 8
  store %"struct.std::__1::__hash_node"* %467, %"struct.std::__1::__hash_node"** %173, align 8
  %468 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %172, align 8
  %469 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %461, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %469, %"class.std::__1::__compressed_pair.240"** %151, align 8
  %470 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %151, align 8
  %471 = bitcast %"class.std::__1::__compressed_pair.240"* %470 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %471, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %150, align 8
  %472 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %150, align 8
  %473 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %472, i32 0, i32 0
  store %"struct.std::__1::__hash_node"* %468, %"struct.std::__1::__hash_node"** %473, align 8
  %474 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %173, align 8
  %475 = icmp ne %"struct.std::__1::__hash_node"* %474, null
  br i1 %475, label %476, label %515

; <label>:476:                                    ; preds = %458
  %477 = getelementptr inbounds %"class.std::__1::unique_ptr.239", %"class.std::__1::unique_ptr.239"* %461, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.240"* %477, %"class.std::__1::__compressed_pair.240"** %149, align 8
  %478 = load %"class.std::__1::__compressed_pair.240"*, %"class.std::__1::__compressed_pair.240"** %149, align 8
  %479 = bitcast %"class.std::__1::__compressed_pair.240"* %478 to %"class.std::__1::__libcpp_compressed_pair_imp.241"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.241"* %479, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %148, align 8
  %480 = load %"class.std::__1::__libcpp_compressed_pair_imp.241"*, %"class.std::__1::__libcpp_compressed_pair_imp.241"** %148, align 8
  %481 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.241", %"class.std::__1::__libcpp_compressed_pair_imp.241"* %480, i32 0, i32 1
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
  %489 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %488, align 8
  %490 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %168, align 8
  %491 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %490, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %491, %"union.std::__1::__hash_value_type"** %166, align 8
  %492 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %166, align 8
  %493 = bitcast %"union.std::__1::__hash_value_type"* %492 to %"struct.std::__1::pair.237"*
  store %"struct.std::__1::pair.237"* %493, %"struct.std::__1::pair.237"** %165, align 8
  %494 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %165, align 8
  %495 = bitcast %"struct.std::__1::pair.237"* %494 to i8*
  store %"class.std::__1::allocator.225"* %489, %"class.std::__1::allocator.225"** %161, align 8
  store %"struct.std::__1::pair.237"* %494, %"struct.std::__1::pair.237"** %162, align 8
  %496 = bitcast %"struct.std::__1::__has_destroy"* %164 to %"struct.std::__1::integral_constant.244"*
  %497 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %161, align 8
  %498 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %162, align 8
  store %"class.std::__1::allocator.225"* %497, %"class.std::__1::allocator.225"** %159, align 8
  store %"struct.std::__1::pair.237"* %498, %"struct.std::__1::pair.237"** %160, align 8
  %499 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %160, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEED1Ev(%"struct.std::__1::pair.237"* %499) #14
  br label %500

; <label>:500:                                    ; preds = %487, %476
  %501 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %168, align 8
  %502 = icmp ne %"struct.std::__1::__hash_node"* %501, null
  br i1 %502, label %503, label %514

; <label>:503:                                    ; preds = %500
  %504 = getelementptr inbounds %"class.std::__1::__hash_node_destructor", %"class.std::__1::__hash_node_destructor"* %483, i32 0, i32 0
  %505 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %504, align 8
  %506 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %168, align 8
  store %"class.std::__1::allocator.225"* %505, %"class.std::__1::allocator.225"** %156, align 8
  store %"struct.std::__1::__hash_node"* %506, %"struct.std::__1::__hash_node"** %157, align 8
  store i64 1, i64* %158, align 8
  %507 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %156, align 8
  %508 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %157, align 8
  %509 = load i64, i64* %158, align 8
  store %"class.std::__1::allocator.225"* %507, %"class.std::__1::allocator.225"** %153, align 8
  store %"struct.std::__1::__hash_node"* %508, %"struct.std::__1::__hash_node"** %154, align 8
  store i64 %509, i64* %155, align 8
  %510 = load %"class.std::__1::allocator.225"*, %"class.std::__1::allocator.225"** %153, align 8
  %511 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %154, align 8
  %512 = bitcast %"struct.std::__1::__hash_node"* %511 to i8*
  store i8* %512, i8** %152, align 8
  %513 = load i8*, i8** %152, align 8
  call void @_ZdlPv(i8* %513) #16
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
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE6rehashEm(%"class.std::__1::__hash_table.213"*, i64) #0 align 2 {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.231"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair.230"*, align 8
  %5 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %6 = alloca float, align 4
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  %9 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.228"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair.227"*, align 8
  %11 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.231"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair.230"*, align 8
  %14 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %15 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.228"*, align 8
  %16 = alloca %"class.std::__1::__compressed_pair.227"*, align 8
  %17 = alloca %"class.std::__1::__hash_table.213"*, align 8
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
  %29 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.220"*, align 8
  %30 = alloca %"class.std::__1::__compressed_pair.219"*, align 8
  %31 = alloca %"class.std::__1::__bucket_list_deallocator.218"*, align 8
  %32 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %33 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %34 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %35 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %36 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %37 = alloca i64, align 8
  %38 = alloca i64, align 8
  %39 = alloca i64, align 8
  store %"class.std::__1::__hash_table.213"* %0, %"class.std::__1::__hash_table.213"** %36, align 8
  store i64 %1, i64* %37, align 8
  %40 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %36, align 8
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
  store %"class.std::__1::__hash_table.213"* %40, %"class.std::__1::__hash_table.213"** %35, align 8
  %55 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %35, align 8
  %56 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %55, i32 0, i32 0
  store %"class.std::__1::unique_ptr.214"* %56, %"class.std::__1::unique_ptr.214"** %34, align 8
  %57 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %34, align 8
  %58 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %57, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %58, %"class.std::__1::__compressed_pair.215"** %33, align 8
  %59 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %33, align 8
  %60 = bitcast %"class.std::__1::__compressed_pair.215"* %59 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %60, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %32, align 8
  %61 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %32, align 8
  %62 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %61, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator.218"* %62, %"class.std::__1::__bucket_list_deallocator.218"** %31, align 8
  %63 = load %"class.std::__1::__bucket_list_deallocator.218"*, %"class.std::__1::__bucket_list_deallocator.218"** %31, align 8
  %64 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.218", %"class.std::__1::__bucket_list_deallocator.218"* %63, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.219"* %64, %"class.std::__1::__compressed_pair.219"** %30, align 8
  %65 = load %"class.std::__1::__compressed_pair.219"*, %"class.std::__1::__compressed_pair.219"** %30, align 8
  %66 = bitcast %"class.std::__1::__compressed_pair.219"* %65 to %"class.std::__1::__libcpp_compressed_pair_imp.220"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.220"* %66, %"class.std::__1::__libcpp_compressed_pair_imp.220"** %29, align 8
  %67 = load %"class.std::__1::__libcpp_compressed_pair_imp.220"*, %"class.std::__1::__libcpp_compressed_pair_imp.220"** %29, align 8
  %68 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.220", %"class.std::__1::__libcpp_compressed_pair_imp.220"* %67, i32 0, i32 0
  %69 = load i64, i64* %68, align 8
  store i64 %69, i64* %38, align 8
  %70 = load i64, i64* %37, align 8
  %71 = load i64, i64* %38, align 8
  %72 = icmp ugt i64 %70, %71
  br i1 %72, label %73, label %75

; <label>:73:                                     ; preds = %54
  %74 = load i64, i64* %37, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE8__rehashEm(%"class.std::__1::__hash_table.213"* %40, i64 %74)
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
  store %"class.std::__1::__hash_table.213"* %40, %"class.std::__1::__hash_table.213"** %17, align 8
  %93 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %17, align 8
  %94 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %93, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.227"* %94, %"class.std::__1::__compressed_pair.227"** %16, align 8
  %95 = load %"class.std::__1::__compressed_pair.227"*, %"class.std::__1::__compressed_pair.227"** %16, align 8
  %96 = bitcast %"class.std::__1::__compressed_pair.227"* %95 to %"class.std::__1::__libcpp_compressed_pair_imp.228"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.228"* %96, %"class.std::__1::__libcpp_compressed_pair_imp.228"** %15, align 8
  %97 = load %"class.std::__1::__libcpp_compressed_pair_imp.228"*, %"class.std::__1::__libcpp_compressed_pair_imp.228"** %15, align 8
  %98 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.228", %"class.std::__1::__libcpp_compressed_pair_imp.228"* %97, i32 0, i32 0
  %99 = load i64, i64* %98, align 8
  %100 = uitofp i64 %99 to float
  store %"class.std::__1::__hash_table.213"* %40, %"class.std::__1::__hash_table.213"** %5, align 8
  %101 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %5, align 8
  %102 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %101, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.230"* %102, %"class.std::__1::__compressed_pair.230"** %4, align 8
  %103 = load %"class.std::__1::__compressed_pair.230"*, %"class.std::__1::__compressed_pair.230"** %4, align 8
  %104 = bitcast %"class.std::__1::__compressed_pair.230"* %103 to %"class.std::__1::__libcpp_compressed_pair_imp.231"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.231"* %104, %"class.std::__1::__libcpp_compressed_pair_imp.231"** %3, align 8
  %105 = load %"class.std::__1::__libcpp_compressed_pair_imp.231"*, %"class.std::__1::__libcpp_compressed_pair_imp.231"** %3, align 8
  %106 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.231", %"class.std::__1::__libcpp_compressed_pair_imp.231"* %105, i32 0, i32 0
  %107 = load float, float* %106, align 4
  %108 = fdiv float %100, %107
  store float %108, float* %6, align 4
  %109 = load float, float* %6, align 4
  %110 = call float @ceilf(float %109) #11
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
  store %"class.std::__1::__hash_table.213"* %40, %"class.std::__1::__hash_table.213"** %11, align 8
  %121 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %11, align 8
  %122 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %121, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.227"* %122, %"class.std::__1::__compressed_pair.227"** %10, align 8
  %123 = load %"class.std::__1::__compressed_pair.227"*, %"class.std::__1::__compressed_pair.227"** %10, align 8
  %124 = bitcast %"class.std::__1::__compressed_pair.227"* %123 to %"class.std::__1::__libcpp_compressed_pair_imp.228"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.228"* %124, %"class.std::__1::__libcpp_compressed_pair_imp.228"** %9, align 8
  %125 = load %"class.std::__1::__libcpp_compressed_pair_imp.228"*, %"class.std::__1::__libcpp_compressed_pair_imp.228"** %9, align 8
  %126 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.228", %"class.std::__1::__libcpp_compressed_pair_imp.228"* %125, i32 0, i32 0
  %127 = load i64, i64* %126, align 8
  %128 = uitofp i64 %127 to float
  store %"class.std::__1::__hash_table.213"* %40, %"class.std::__1::__hash_table.213"** %14, align 8
  %129 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %14, align 8
  %130 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %129, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.230"* %130, %"class.std::__1::__compressed_pair.230"** %13, align 8
  %131 = load %"class.std::__1::__compressed_pair.230"*, %"class.std::__1::__compressed_pair.230"** %13, align 8
  %132 = bitcast %"class.std::__1::__compressed_pair.230"* %131 to %"class.std::__1::__libcpp_compressed_pair_imp.231"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.231"* %132, %"class.std::__1::__libcpp_compressed_pair_imp.231"** %12, align 8
  %133 = load %"class.std::__1::__libcpp_compressed_pair_imp.231"*, %"class.std::__1::__libcpp_compressed_pair_imp.231"** %12, align 8
  %134 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.231", %"class.std::__1::__libcpp_compressed_pair_imp.231"* %133, i32 0, i32 0
  %135 = load float, float* %134, align 4
  %136 = fdiv float %128, %135
  store float %136, float* %18, align 4
  %137 = load float, float* %18, align 4
  %138 = call float @ceilf(float %137) #11
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
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE8__rehashEm(%"class.std::__1::__hash_table.213"* %40, i64 %164)
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
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %5 = alloca %"class.std::__1::basic_string"*, align 8
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %8 = alloca %"class.std::__1::basic_string"*, align 8
  %9 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca i8*, align 8
  %14 = alloca i8*, align 8
  %15 = alloca %"struct.std::__1::__murmur2_or_cityhash", align 1
  %16 = alloca i8*, align 8
  %17 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %18 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %19 = alloca %"class.std::__1::basic_string"*, align 8
  %20 = alloca i8*, align 8
  %21 = alloca i8*, align 8
  %22 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %23 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %24 = alloca %"class.std::__1::basic_string"*, align 8
  %25 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %26 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %27 = alloca %"class.std::__1::basic_string"*, align 8
  %28 = alloca %"class.std::__1::basic_string"*, align 8
  %29 = alloca %"class.std::__1::basic_string"*, align 8
  %30 = alloca i8*, align 8
  %31 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %32 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %33 = alloca %"class.std::__1::basic_string"*, align 8
  %34 = alloca i8*, align 8
  %35 = alloca i8*, align 8
  %36 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %37 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %38 = alloca %"class.std::__1::basic_string"*, align 8
  %39 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %40 = alloca %"class.std::__1::__compressed_pair"*, align 8
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
  store %"class.std::__1::__compressed_pair"* %51, %"class.std::__1::__compressed_pair"** %40, align 8
  %52 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %40, align 8
  %53 = bitcast %"class.std::__1::__compressed_pair"* %52 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %53, %"class.std::__1::__libcpp_compressed_pair_imp"** %39, align 8
  %54 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %39, align 8
  %55 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %54, i32 0, i32 0
  %56 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %55, i32 0, i32 0
  %57 = bitcast %union.anon* %56 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %58 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %57, i32 0, i32 0
  %59 = bitcast %union.anon.0* %58 to i8*
  %60 = load i8, i8* %59, align 8
  %61 = zext i8 %60 to i32
  %62 = and i32 %61, 1
  %63 = icmp ne i32 %62, 0
  br i1 %63, label %64, label %75

; <label>:64:                                     ; preds = %2
  store %"class.std::__1::basic_string"* %49, %"class.std::__1::basic_string"** %33, align 8
  %65 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8
  %66 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %65, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %66, %"class.std::__1::__compressed_pair"** %32, align 8
  %67 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8
  %68 = bitcast %"class.std::__1::__compressed_pair"* %67 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %68, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8
  %69 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8
  %70 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %69, i32 0, i32 0
  %71 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %70, i32 0, i32 0
  %72 = bitcast %union.anon* %71 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %73 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %72, i32 0, i32 2
  %74 = load i8*, i8** %73, align 8
  br label %88

; <label>:75:                                     ; preds = %2
  store %"class.std::__1::basic_string"* %49, %"class.std::__1::basic_string"** %38, align 8
  %76 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8
  %77 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %76, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %77, %"class.std::__1::__compressed_pair"** %37, align 8
  %78 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %37, align 8
  %79 = bitcast %"class.std::__1::__compressed_pair"* %78 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %79, %"class.std::__1::__libcpp_compressed_pair_imp"** %36, align 8
  %80 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %36, align 8
  %81 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %80, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair"* %95, %"class.std::__1::__compressed_pair"** %26, align 8
  %96 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %26, align 8
  %97 = bitcast %"class.std::__1::__compressed_pair"* %96 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %97, %"class.std::__1::__libcpp_compressed_pair_imp"** %25, align 8
  %98 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %25, align 8
  %99 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %98, i32 0, i32 0
  %100 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %99, i32 0, i32 0
  %101 = bitcast %union.anon* %100 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %102 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %101, i32 0, i32 0
  %103 = bitcast %union.anon.0* %102 to i8*
  %104 = load i8, i8* %103, align 8
  %105 = zext i8 %104 to i32
  %106 = and i32 %105, 1
  %107 = icmp ne i32 %106, 0
  br i1 %107, label %108, label %119

; <label>:108:                                    ; preds = %88
  store %"class.std::__1::basic_string"* %93, %"class.std::__1::basic_string"** %19, align 8
  %109 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %19, align 8
  %110 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %109, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %110, %"class.std::__1::__compressed_pair"** %18, align 8
  %111 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %18, align 8
  %112 = bitcast %"class.std::__1::__compressed_pair"* %111 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %112, %"class.std::__1::__libcpp_compressed_pair_imp"** %17, align 8
  %113 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %17, align 8
  %114 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %113, i32 0, i32 0
  %115 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %114, i32 0, i32 0
  %116 = bitcast %union.anon* %115 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %117 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %116, i32 0, i32 2
  %118 = load i8*, i8** %117, align 8
  br label %132

; <label>:119:                                    ; preds = %88
  store %"class.std::__1::basic_string"* %93, %"class.std::__1::basic_string"** %24, align 8
  %120 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8
  %121 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %120, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %121, %"class.std::__1::__compressed_pair"** %23, align 8
  %122 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %23, align 8
  %123 = bitcast %"class.std::__1::__compressed_pair"* %122 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %123, %"class.std::__1::__libcpp_compressed_pair_imp"** %22, align 8
  %124 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %22, align 8
  %125 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %124, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair"* %138, %"class.std::__1::__compressed_pair"** %10, align 8
  %139 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %10, align 8
  %140 = bitcast %"class.std::__1::__compressed_pair"* %139 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %140, %"class.std::__1::__libcpp_compressed_pair_imp"** %9, align 8
  %141 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %9, align 8
  %142 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %141, i32 0, i32 0
  %143 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %142, i32 0, i32 0
  %144 = bitcast %union.anon* %143 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %145 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %144, i32 0, i32 0
  %146 = bitcast %union.anon.0* %145 to i8*
  %147 = load i8, i8* %146, align 8
  %148 = zext i8 %147 to i32
  %149 = and i32 %148, 1
  %150 = icmp ne i32 %149, 0
  br i1 %150, label %151, label %162

; <label>:151:                                    ; preds = %132
  store %"class.std::__1::basic_string"* %136, %"class.std::__1::basic_string"** %5, align 8
  %152 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %5, align 8
  %153 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %152, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %153, %"class.std::__1::__compressed_pair"** %4, align 8
  %154 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %4, align 8
  %155 = bitcast %"class.std::__1::__compressed_pair"* %154 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %155, %"class.std::__1::__libcpp_compressed_pair_imp"** %3, align 8
  %156 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %3, align 8
  %157 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %156, i32 0, i32 0
  %158 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %157, i32 0, i32 0
  %159 = bitcast %union.anon* %158 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %160 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %159, i32 0, i32 1
  %161 = load i64, i64* %160, align 8
  br label %177

; <label>:162:                                    ; preds = %132
  store %"class.std::__1::basic_string"* %136, %"class.std::__1::basic_string"** %8, align 8
  %163 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %8, align 8
  %164 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %163, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %164, %"class.std::__1::__compressed_pair"** %7, align 8
  %165 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %7, align 8
  %166 = bitcast %"class.std::__1::__compressed_pair"* %165 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %166, %"class.std::__1::__libcpp_compressed_pair_imp"** %6, align 8
  %167 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %6, align 8
  %168 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %167, i32 0, i32 0
  %169 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %168, i32 0, i32 0
  %170 = bitcast %union.anon* %169 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %171 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %170, i32 0, i32 0
  %172 = bitcast %union.anon.0* %171 to i8*
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
  %12 = alloca %"struct.std::__1::pair.242"*, align 8
  %13 = alloca %"struct.std::__1::pair.242"*, align 8
  %14 = alloca i8*, align 8
  %15 = alloca i64, align 8
  %16 = alloca i8*, align 8
  %17 = alloca i64, align 8
  %18 = alloca i64*, align 8
  %19 = alloca i64*, align 8
  %20 = alloca %"struct.std::__1::pair.242"*, align 8
  %21 = alloca %"struct.std::__1::pair.242"*, align 8
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
  %46 = alloca %"struct.std::__1::pair.242", align 8
  %47 = alloca %"struct.std::__1::pair.242", align 8
  %48 = alloca %"struct.std::__1::pair.242", align 8
  %49 = alloca %"struct.std::__1::pair.242", align 8
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
  %120 = bitcast %"struct.std::__1::pair.242"* %46 to { i64, i64 }*
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
  %133 = bitcast %"struct.std::__1::pair.242"* %47 to { i64, i64 }*
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
  %152 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %46, i32 0, i32 0
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
  %164 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %46, i32 0, i32 1
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
  %175 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %47, i32 0, i32 1
  %176 = load i64, i64* %175, align 8
  %177 = load i64, i64* %43, align 8
  %178 = xor i64 %177, %176
  store i64 %178, i64* %43, align 8
  %179 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %46, i32 0, i32 0
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
  %190 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %47, i32 0, i32 0
  %191 = load i64, i64* %190, align 8
  %192 = add i64 %189, %191
  %193 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE8__rotateEmi(i64 %192, i32 33)
  %194 = mul i64 %193, -5435081209227447693
  store i64 %194, i64* %45, align 8
  %195 = load i8*, i8** %42, align 8
  %196 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %46, i32 0, i32 1
  %197 = load i64, i64* %196, align 8
  %198 = mul i64 %197, -5435081209227447693
  %199 = load i64, i64* %43, align 8
  %200 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %47, i32 0, i32 0
  %201 = load i64, i64* %200, align 8
  %202 = add i64 %199, %201
  %203 = call { i64, i64 } @_ZNSt3__121__murmur2_or_cityhashImLm64EE29__weak_hash_len_32_with_seedsEPKcmm(i8* %195, i64 %198, i64 %202)
  %204 = bitcast %"struct.std::__1::pair.242"* %48 to { i64, i64 }*
  %205 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %204, i32 0, i32 0
  %206 = extractvalue { i64, i64 } %203, 0
  store i64 %206, i64* %205, align 8
  %207 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %204, i32 0, i32 1
  %208 = extractvalue { i64, i64 } %203, 1
  store i64 %208, i64* %207, align 8
  store %"struct.std::__1::pair.242"* %46, %"struct.std::__1::pair.242"** %12, align 8
  store %"struct.std::__1::pair.242"* %48, %"struct.std::__1::pair.242"** %13, align 8
  %209 = load %"struct.std::__1::pair.242"*, %"struct.std::__1::pair.242"** %12, align 8
  %210 = load %"struct.std::__1::pair.242"*, %"struct.std::__1::pair.242"** %13, align 8
  %211 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %210, i32 0, i32 0
  store i64* %211, i64** %11, align 8
  %212 = load i64*, i64** %11, align 8
  %213 = load i64, i64* %212, align 8
  %214 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %209, i32 0, i32 0
  store i64 %213, i64* %214, align 8
  %215 = load %"struct.std::__1::pair.242"*, %"struct.std::__1::pair.242"** %13, align 8
  %216 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %215, i32 0, i32 1
  store i64* %216, i64** %10, align 8
  %217 = load i64*, i64** %10, align 8
  %218 = load i64, i64* %217, align 8
  %219 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %209, i32 0, i32 1
  store i64 %218, i64* %219, align 8
  %220 = load i8*, i8** %42, align 8
  %221 = getelementptr inbounds i8, i8* %220, i64 32
  %222 = load i64, i64* %45, align 8
  %223 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %47, i32 0, i32 1
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
  %234 = bitcast %"struct.std::__1::pair.242"* %49 to { i64, i64 }*
  %235 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %234, i32 0, i32 0
  %236 = extractvalue { i64, i64 } %233, 0
  store i64 %236, i64* %235, align 8
  %237 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %234, i32 0, i32 1
  %238 = extractvalue { i64, i64 } %233, 1
  store i64 %238, i64* %237, align 8
  store %"struct.std::__1::pair.242"* %47, %"struct.std::__1::pair.242"** %20, align 8
  store %"struct.std::__1::pair.242"* %49, %"struct.std::__1::pair.242"** %21, align 8
  %239 = load %"struct.std::__1::pair.242"*, %"struct.std::__1::pair.242"** %20, align 8
  %240 = load %"struct.std::__1::pair.242"*, %"struct.std::__1::pair.242"** %21, align 8
  %241 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %240, i32 0, i32 0
  store i64* %241, i64** %19, align 8
  %242 = load i64*, i64** %19, align 8
  %243 = load i64, i64* %242, align 8
  %244 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %239, i32 0, i32 0
  store i64 %243, i64* %244, align 8
  %245 = load %"struct.std::__1::pair.242"*, %"struct.std::__1::pair.242"** %21, align 8
  %246 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %245, i32 0, i32 1
  store i64* %246, i64** %18, align 8
  %247 = load i64*, i64** %18, align 8
  %248 = load i64, i64* %247, align 8
  %249 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %239, i32 0, i32 1
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
  %268 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %46, i32 0, i32 0
  %269 = load i64, i64* %268, align 8
  %270 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %47, i32 0, i32 0
  %271 = load i64, i64* %270, align 8
  %272 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE13__hash_len_16Emm(i64 %269, i64 %271)
  %273 = load i64, i64* %44, align 8
  %274 = call i64 @_ZNSt3__121__murmur2_or_cityhashImLm64EE11__shift_mixEm(i64 %273)
  %275 = mul i64 %274, -5435081209227447693
  %276 = add i64 %272, %275
  %277 = load i64, i64* %45, align 8
  %278 = add i64 %276, %277
  %279 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %46, i32 0, i32 1
  %280 = load i64, i64* %279, align 8
  %281 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %47, i32 0, i32 1
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
  %12 = alloca %"struct.std::__1::pair.242", align 8
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
  %38 = bitcast %"struct.std::__1::pair.242"* %12 to { i64, i64 }*
  %39 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %38, i32 0, i32 0
  %40 = extractvalue { i64, i64 } %37, 0
  store i64 %40, i64* %39, align 8
  %41 = getelementptr inbounds { i64, i64 }, { i64, i64 }* %38, i32 0, i32 1
  %42 = extractvalue { i64, i64 } %37, 1
  store i64 %42, i64* %41, align 8
  %43 = bitcast %"struct.std::__1::pair.242"* %12 to { i64, i64 }*
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
  %9 = alloca %"struct.std::__1::pair.242"*, align 8
  %10 = alloca i64*, align 8
  %11 = alloca i64*, align 8
  %12 = alloca %"struct.std::__1::pair.242"*, align 8
  %13 = alloca i64*, align 8
  %14 = alloca i64*, align 8
  %15 = alloca %"struct.std::__1::pair.242", align 8
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
  store %"struct.std::__1::pair.242"* %15, %"struct.std::__1::pair.242"** %12, align 8
  store i64* %23, i64** %13, align 8
  store i64* %24, i64** %14, align 8
  %51 = load %"struct.std::__1::pair.242"*, %"struct.std::__1::pair.242"** %12, align 8
  %52 = load i64*, i64** %13, align 8
  %53 = load i64*, i64** %14, align 8
  store %"struct.std::__1::pair.242"* %51, %"struct.std::__1::pair.242"** %9, align 8
  store i64* %52, i64** %10, align 8
  store i64* %53, i64** %11, align 8
  %54 = load %"struct.std::__1::pair.242"*, %"struct.std::__1::pair.242"** %9, align 8
  %55 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %54, i32 0, i32 0
  %56 = load i64*, i64** %10, align 8
  store i64* %56, i64** %8, align 8
  %57 = load i64*, i64** %8, align 8
  %58 = load i64, i64* %57, align 8
  store i64 %58, i64* %55, align 8
  %59 = getelementptr inbounds %"struct.std::__1::pair.242", %"struct.std::__1::pair.242"* %54, i32 0, i32 1
  %60 = load i64*, i64** %11, align 8
  store i64* %60, i64** %7, align 8
  %61 = load i64*, i64** %7, align 8
  %62 = load i64, i64* %61, align 8
  store i64 %62, i64* %59, align 8
  %63 = bitcast %"struct.std::__1::pair.242"* %15 to { i64, i64 }*
  %64 = load { i64, i64 }, { i64, i64 }* %63, align 8
  ret { i64, i64 } %64
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8*, i8*, i64) #7 align 2 {
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
declare noalias i8* @_Znwm(i64) #9

declare i64 @_ZNSt3__112__next_primeEm(i64) #1

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEEENS_22__unordered_map_hasherIS7_SB_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_SB_NS_8equal_toIS7_EELb1EEENS5_ISB_EEE8__rehashEm(%"class.std::__1::__hash_table.213"*, i64) #0 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.220"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair.219"*, align 8
  %5 = alloca %"class.std::__1::__bucket_list_deallocator.218"*, align 8
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %8 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %9 = alloca i64, align 8
  %10 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.224"*, align 8
  %11 = alloca %"class.std::__1::__compressed_pair.223"*, align 8
  %12 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %13 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %14 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %15 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %16 = alloca i64, align 8
  %17 = alloca i64, align 8
  %18 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %19 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %20 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %21 = alloca i64, align 8
  %22 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %23 = alloca i64, align 8
  %24 = alloca i64, align 8
  %25 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %26 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %27 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %28 = alloca i64, align 8
  %29 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %30 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %31 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %32 = alloca i64, align 8
  %33 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.231"*, align 8
  %34 = alloca %"class.std::__1::__compressed_pair.230"*, align 8
  %35 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %36 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %37 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %38 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %39 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %40 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %41 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %42 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %43 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %44 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %45 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %46 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %47 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %48 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %49 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.220"*, align 8
  %50 = alloca %"class.std::__1::__compressed_pair.219"*, align 8
  %51 = alloca %"class.std::__1::__bucket_list_deallocator.218"*, align 8
  %52 = alloca i8*, align 8
  %53 = alloca %"class.std::__1::allocator.221"*, align 8
  %54 = alloca %"struct.std::__1::__hash_node_base.217"**, align 8
  %55 = alloca i64, align 8
  %56 = alloca %"class.std::__1::allocator.221"*, align 8
  %57 = alloca %"struct.std::__1::__hash_node_base.217"**, align 8
  %58 = alloca i64, align 8
  %59 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.220"*, align 8
  %60 = alloca %"class.std::__1::__compressed_pair.219"*, align 8
  %61 = alloca %"class.std::__1::__bucket_list_deallocator.218"*, align 8
  %62 = alloca %"class.std::__1::__bucket_list_deallocator.218"*, align 8
  %63 = alloca %"struct.std::__1::__hash_node_base.217"**, align 8
  %64 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %65 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %66 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %67 = alloca %"struct.std::__1::__hash_node_base.217"**, align 8
  %68 = alloca %"struct.std::__1::__hash_node_base.217"**, align 8
  %69 = alloca i64, align 8
  %70 = alloca %"class.std::__1::allocator.221"*, align 8
  %71 = alloca i64, align 8
  %72 = alloca i8*, align 8
  %73 = alloca %"class.std::__1::allocator.221"*, align 8
  %74 = alloca i64, align 8
  %75 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.220"*, align 8
  %76 = alloca %"class.std::__1::__compressed_pair.219"*, align 8
  %77 = alloca %"class.std::__1::__bucket_list_deallocator.218"*, align 8
  %78 = alloca i8*, align 8
  %79 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %80 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %81 = alloca %"class.std::__1::basic_string"*, align 8
  %82 = alloca i8*, align 8
  %83 = alloca i8*, align 8
  %84 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %85 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %86 = alloca %"class.std::__1::basic_string"*, align 8
  %87 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %88 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %89 = alloca %"class.std::__1::basic_string"*, align 8
  %90 = alloca %"class.std::__1::basic_string"*, align 8
  %91 = alloca %"class.std::__1::basic_string"*, align 8
  %92 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %93 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %94 = alloca %"class.std::__1::basic_string"*, align 8
  %95 = alloca i8*, align 8
  %96 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %97 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %98 = alloca %"class.std::__1::basic_string"*, align 8
  %99 = alloca i8*, align 8
  %100 = alloca i8*, align 8
  %101 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %102 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %103 = alloca %"class.std::__1::basic_string"*, align 8
  %104 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %105 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %106 = alloca %"class.std::__1::basic_string"*, align 8
  %107 = alloca %"class.std::__1::basic_string"*, align 8
  %108 = alloca %"class.std::__1::basic_string"*, align 8
  %109 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %110 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %111 = alloca %"class.std::__1::basic_string"*, align 8
  %112 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %113 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %114 = alloca %"class.std::__1::basic_string"*, align 8
  %115 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %116 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %117 = alloca %"class.std::__1::basic_string"*, align 8
  %118 = alloca %"class.std::__1::basic_string"*, align 8
  %119 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %120 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %121 = alloca %"class.std::__1::basic_string"*, align 8
  %122 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %123 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %124 = alloca %"class.std::__1::basic_string"*, align 8
  %125 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %126 = alloca %"class.std::__1::__compressed_pair"*, align 8
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
  %138 = alloca %"class.std::__1::__unordered_map_equal.232"*, align 8
  %139 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %140 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %141 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %142 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %143 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %144 = alloca i64, align 8
  %145 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %146 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %147 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %148 = alloca i64, align 8
  %149 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.216"*, align 8
  %150 = alloca %"class.std::__1::__compressed_pair.215"*, align 8
  %151 = alloca %"class.std::__1::unique_ptr.214"*, align 8
  %152 = alloca %"class.std::__1::__hash_table.213"*, align 8
  %153 = alloca i64, align 8
  %154 = alloca %"class.std::__1::allocator.221"*, align 8
  %155 = alloca i64, align 8
  %156 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %157 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  %158 = alloca i64, align 8
  %159 = alloca i64, align 8
  %160 = alloca %"struct.std::__1::__hash_node_base.217"*, align 8
  store %"class.std::__1::__hash_table.213"* %0, %"class.std::__1::__hash_table.213"** %152, align 8
  store i64 %1, i64* %153, align 8
  %161 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %152, align 8
  %162 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %161, i32 0, i32 0
  store %"class.std::__1::unique_ptr.214"* %162, %"class.std::__1::unique_ptr.214"** %151, align 8
  %163 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %151, align 8
  %164 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %163, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %164, %"class.std::__1::__compressed_pair.215"** %150, align 8
  %165 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %150, align 8
  %166 = bitcast %"class.std::__1::__compressed_pair.215"* %165 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %166, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %149, align 8
  %167 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %149, align 8
  %168 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %167, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator.218"* %168, %"class.std::__1::__bucket_list_deallocator.218"** %77, align 8
  %169 = load %"class.std::__1::__bucket_list_deallocator.218"*, %"class.std::__1::__bucket_list_deallocator.218"** %77, align 8
  %170 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.218", %"class.std::__1::__bucket_list_deallocator.218"* %169, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.219"* %170, %"class.std::__1::__compressed_pair.219"** %76, align 8
  %171 = load %"class.std::__1::__compressed_pair.219"*, %"class.std::__1::__compressed_pair.219"** %76, align 8
  %172 = bitcast %"class.std::__1::__compressed_pair.219"* %171 to %"class.std::__1::__libcpp_compressed_pair_imp.220"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.220"* %172, %"class.std::__1::__libcpp_compressed_pair_imp.220"** %75, align 8
  %173 = load %"class.std::__1::__libcpp_compressed_pair_imp.220"*, %"class.std::__1::__libcpp_compressed_pair_imp.220"** %75, align 8
  %174 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.220"* %173 to %"class.std::__1::allocator.221"*
  store %"class.std::__1::allocator.221"* %174, %"class.std::__1::allocator.221"** %154, align 8
  %175 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %161, i32 0, i32 0
  %176 = load i64, i64* %153, align 8
  %177 = icmp ugt i64 %176, 0
  br i1 %177, label %178, label %189

; <label>:178:                                    ; preds = %2
  %179 = load %"class.std::__1::allocator.221"*, %"class.std::__1::allocator.221"** %154, align 8
  %180 = load i64, i64* %153, align 8
  store %"class.std::__1::allocator.221"* %179, %"class.std::__1::allocator.221"** %73, align 8
  store i64 %180, i64* %74, align 8
  %181 = load %"class.std::__1::allocator.221"*, %"class.std::__1::allocator.221"** %73, align 8
  %182 = load i64, i64* %74, align 8
  store %"class.std::__1::allocator.221"* %181, %"class.std::__1::allocator.221"** %70, align 8
  store i64 %182, i64* %71, align 8
  store i8* null, i8** %72, align 8
  %183 = load %"class.std::__1::allocator.221"*, %"class.std::__1::allocator.221"** %70, align 8
  %184 = load i64, i64* %71, align 8
  %185 = mul i64 %184, 8
  store i64 %185, i64* %69, align 8
  %186 = load i64, i64* %69, align 8
  %187 = call i8* @_Znwm(i64 %186) #18
  %188 = bitcast i8* %187 to %"struct.std::__1::__hash_node_base.217"**
  br label %190

; <label>:189:                                    ; preds = %2
  br label %190

; <label>:190:                                    ; preds = %189, %178
  %191 = phi %"struct.std::__1::__hash_node_base.217"** [ %188, %178 ], [ null, %189 ]
  store %"class.std::__1::unique_ptr.214"* %175, %"class.std::__1::unique_ptr.214"** %66, align 8
  store %"struct.std::__1::__hash_node_base.217"** %191, %"struct.std::__1::__hash_node_base.217"*** %67, align 8
  %192 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %66, align 8
  %193 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %192, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %193, %"class.std::__1::__compressed_pair.215"** %65, align 8
  %194 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %65, align 8
  %195 = bitcast %"class.std::__1::__compressed_pair.215"* %194 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %195, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %64, align 8
  %196 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %64, align 8
  %197 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %196, i32 0, i32 0
  %198 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %197, align 8
  store %"struct.std::__1::__hash_node_base.217"** %198, %"struct.std::__1::__hash_node_base.217"*** %68, align 8
  %199 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %67, align 8
  %200 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %192, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %200, %"class.std::__1::__compressed_pair.215"** %48, align 8
  %201 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %48, align 8
  %202 = bitcast %"class.std::__1::__compressed_pair.215"* %201 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %202, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %47, align 8
  %203 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %47, align 8
  %204 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %203, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.217"** %199, %"struct.std::__1::__hash_node_base.217"*** %204, align 8
  %205 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %68, align 8
  %206 = icmp ne %"struct.std::__1::__hash_node_base.217"** %205, null
  br i1 %206, label %207, label %236

; <label>:207:                                    ; preds = %190
  %208 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %192, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %208, %"class.std::__1::__compressed_pair.215"** %46, align 8
  %209 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %46, align 8
  %210 = bitcast %"class.std::__1::__compressed_pair.215"* %209 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %210, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %45, align 8
  %211 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %45, align 8
  %212 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %211, i32 0, i32 1
  %213 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %68, align 8
  store %"class.std::__1::__bucket_list_deallocator.218"* %212, %"class.std::__1::__bucket_list_deallocator.218"** %62, align 8
  store %"struct.std::__1::__hash_node_base.217"** %213, %"struct.std::__1::__hash_node_base.217"*** %63, align 8
  %214 = load %"class.std::__1::__bucket_list_deallocator.218"*, %"class.std::__1::__bucket_list_deallocator.218"** %62, align 8
  store %"class.std::__1::__bucket_list_deallocator.218"* %214, %"class.std::__1::__bucket_list_deallocator.218"** %61, align 8
  %215 = load %"class.std::__1::__bucket_list_deallocator.218"*, %"class.std::__1::__bucket_list_deallocator.218"** %61, align 8
  %216 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.218", %"class.std::__1::__bucket_list_deallocator.218"* %215, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.219"* %216, %"class.std::__1::__compressed_pair.219"** %60, align 8
  %217 = load %"class.std::__1::__compressed_pair.219"*, %"class.std::__1::__compressed_pair.219"** %60, align 8
  %218 = bitcast %"class.std::__1::__compressed_pair.219"* %217 to %"class.std::__1::__libcpp_compressed_pair_imp.220"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.220"* %218, %"class.std::__1::__libcpp_compressed_pair_imp.220"** %59, align 8
  %219 = load %"class.std::__1::__libcpp_compressed_pair_imp.220"*, %"class.std::__1::__libcpp_compressed_pair_imp.220"** %59, align 8
  %220 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.220"* %219 to %"class.std::__1::allocator.221"*
  %221 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %63, align 8
  store %"class.std::__1::__bucket_list_deallocator.218"* %214, %"class.std::__1::__bucket_list_deallocator.218"** %51, align 8
  %222 = load %"class.std::__1::__bucket_list_deallocator.218"*, %"class.std::__1::__bucket_list_deallocator.218"** %51, align 8
  %223 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.218", %"class.std::__1::__bucket_list_deallocator.218"* %222, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.219"* %223, %"class.std::__1::__compressed_pair.219"** %50, align 8
  %224 = load %"class.std::__1::__compressed_pair.219"*, %"class.std::__1::__compressed_pair.219"** %50, align 8
  %225 = bitcast %"class.std::__1::__compressed_pair.219"* %224 to %"class.std::__1::__libcpp_compressed_pair_imp.220"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.220"* %225, %"class.std::__1::__libcpp_compressed_pair_imp.220"** %49, align 8
  %226 = load %"class.std::__1::__libcpp_compressed_pair_imp.220"*, %"class.std::__1::__libcpp_compressed_pair_imp.220"** %49, align 8
  %227 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.220", %"class.std::__1::__libcpp_compressed_pair_imp.220"* %226, i32 0, i32 0
  %228 = load i64, i64* %227, align 8
  store %"class.std::__1::allocator.221"* %220, %"class.std::__1::allocator.221"** %56, align 8
  store %"struct.std::__1::__hash_node_base.217"** %221, %"struct.std::__1::__hash_node_base.217"*** %57, align 8
  store i64 %228, i64* %58, align 8
  %229 = load %"class.std::__1::allocator.221"*, %"class.std::__1::allocator.221"** %56, align 8
  %230 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %57, align 8
  %231 = load i64, i64* %58, align 8
  store %"class.std::__1::allocator.221"* %229, %"class.std::__1::allocator.221"** %53, align 8
  store %"struct.std::__1::__hash_node_base.217"** %230, %"struct.std::__1::__hash_node_base.217"*** %54, align 8
  store i64 %231, i64* %55, align 8
  %232 = load %"class.std::__1::allocator.221"*, %"class.std::__1::allocator.221"** %53, align 8
  %233 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %54, align 8
  %234 = bitcast %"struct.std::__1::__hash_node_base.217"** %233 to i8*
  store i8* %234, i8** %52, align 8
  %235 = load i8*, i8** %52, align 8
  call void @_ZdlPv(i8* %235) #16
  br label %236

; <label>:236:                                    ; preds = %190, %207
  %237 = load i64, i64* %153, align 8
  %238 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %161, i32 0, i32 0
  store %"class.std::__1::unique_ptr.214"* %238, %"class.std::__1::unique_ptr.214"** %44, align 8
  %239 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %44, align 8
  %240 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %239, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %240, %"class.std::__1::__compressed_pair.215"** %43, align 8
  %241 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %43, align 8
  %242 = bitcast %"class.std::__1::__compressed_pair.215"* %241 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %242, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %42, align 8
  %243 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %42, align 8
  %244 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %243, i32 0, i32 1
  store %"class.std::__1::__bucket_list_deallocator.218"* %244, %"class.std::__1::__bucket_list_deallocator.218"** %5, align 8
  %245 = load %"class.std::__1::__bucket_list_deallocator.218"*, %"class.std::__1::__bucket_list_deallocator.218"** %5, align 8
  %246 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator.218", %"class.std::__1::__bucket_list_deallocator.218"* %245, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.219"* %246, %"class.std::__1::__compressed_pair.219"** %4, align 8
  %247 = load %"class.std::__1::__compressed_pair.219"*, %"class.std::__1::__compressed_pair.219"** %4, align 8
  %248 = bitcast %"class.std::__1::__compressed_pair.219"* %247 to %"class.std::__1::__libcpp_compressed_pair_imp.220"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.220"* %248, %"class.std::__1::__libcpp_compressed_pair_imp.220"** %3, align 8
  %249 = load %"class.std::__1::__libcpp_compressed_pair_imp.220"*, %"class.std::__1::__libcpp_compressed_pair_imp.220"** %3, align 8
  %250 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.220", %"class.std::__1::__libcpp_compressed_pair_imp.220"* %249, i32 0, i32 0
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
  %259 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %161, i32 0, i32 0
  %260 = load i64, i64* %155, align 8
  store %"class.std::__1::unique_ptr.214"* %259, %"class.std::__1::unique_ptr.214"** %8, align 8
  store i64 %260, i64* %9, align 8
  %261 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %8, align 8
  %262 = load i64, i64* %9, align 8
  %263 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %261, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %263, %"class.std::__1::__compressed_pair.215"** %7, align 8
  %264 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %7, align 8
  %265 = bitcast %"class.std::__1::__compressed_pair.215"* %264 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %265, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %6, align 8
  %266 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %6, align 8
  %267 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %266, i32 0, i32 0
  %268 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %267, align 8
  %269 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %268, i64 %262
  store %"struct.std::__1::__hash_node_base.217"* null, %"struct.std::__1::__hash_node_base.217"** %269, align 8
  br label %270

; <label>:270:                                    ; preds = %258
  %271 = load i64, i64* %155, align 8
  %272 = add i64 %271, 1
  store i64 %272, i64* %155, align 8
  br label %254

; <label>:273:                                    ; preds = %254
  %274 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %161, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.223"* %274, %"class.std::__1::__compressed_pair.223"** %11, align 8
  %275 = load %"class.std::__1::__compressed_pair.223"*, %"class.std::__1::__compressed_pair.223"** %11, align 8
  %276 = bitcast %"class.std::__1::__compressed_pair.223"* %275 to %"class.std::__1::__libcpp_compressed_pair_imp.224"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.224"* %276, %"class.std::__1::__libcpp_compressed_pair_imp.224"** %10, align 8
  %277 = load %"class.std::__1::__libcpp_compressed_pair_imp.224"*, %"class.std::__1::__libcpp_compressed_pair_imp.224"** %10, align 8
  %278 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.224", %"class.std::__1::__libcpp_compressed_pair_imp.224"* %277, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.217"* %278, %"struct.std::__1::__hash_node_base.217"** %14, align 8
  %279 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %14, align 8
  store %"struct.std::__1::__hash_node_base.217"* %279, %"struct.std::__1::__hash_node_base.217"** %13, align 8
  %280 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %13, align 8
  store %"struct.std::__1::__hash_node_base.217"* %280, %"struct.std::__1::__hash_node_base.217"** %12, align 8
  %281 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %12, align 8
  %282 = bitcast %"struct.std::__1::__hash_node_base.217"* %281 to i8*
  store %"struct.std::__1::__hash_node_base.217"* %281, %"struct.std::__1::__hash_node_base.217"** %156, align 8
  %283 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %156, align 8
  %284 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %283, i32 0, i32 0
  %285 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %284, align 8
  store %"struct.std::__1::__hash_node_base.217"* %285, %"struct.std::__1::__hash_node_base.217"** %157, align 8
  %286 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %157, align 8
  %287 = icmp ne %"struct.std::__1::__hash_node_base.217"* %286, null
  br i1 %287, label %288, label %723

; <label>:288:                                    ; preds = %273
  %289 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %157, align 8
  store %"struct.std::__1::__hash_node_base.217"* %289, %"struct.std::__1::__hash_node_base.217"** %15, align 8
  %290 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %15, align 8
  %291 = bitcast %"struct.std::__1::__hash_node_base.217"* %290 to %"struct.std::__1::__hash_node"*
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
  %319 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %156, align 8
  %320 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %161, i32 0, i32 0
  %321 = load i64, i64* %158, align 8
  store %"class.std::__1::unique_ptr.214"* %320, %"class.std::__1::unique_ptr.214"** %20, align 8
  store i64 %321, i64* %21, align 8
  %322 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %20, align 8
  %323 = load i64, i64* %21, align 8
  %324 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %322, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %324, %"class.std::__1::__compressed_pair.215"** %19, align 8
  %325 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %19, align 8
  %326 = bitcast %"class.std::__1::__compressed_pair.215"* %325 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %326, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %18, align 8
  %327 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %18, align 8
  %328 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %327, i32 0, i32 0
  %329 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %328, align 8
  %330 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %329, i64 %323
  store %"struct.std::__1::__hash_node_base.217"* %319, %"struct.std::__1::__hash_node_base.217"** %330, align 8
  %331 = load i64, i64* %158, align 8
  store i64 %331, i64* %159, align 8
  %332 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %157, align 8
  store %"struct.std::__1::__hash_node_base.217"* %332, %"struct.std::__1::__hash_node_base.217"** %156, align 8
  %333 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %157, align 8
  %334 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %333, i32 0, i32 0
  %335 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %334, align 8
  store %"struct.std::__1::__hash_node_base.217"* %335, %"struct.std::__1::__hash_node_base.217"** %157, align 8
  br label %336

; <label>:336:                                    ; preds = %718, %317
  %337 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %157, align 8
  %338 = icmp ne %"struct.std::__1::__hash_node_base.217"* %337, null
  br i1 %338, label %339, label %722

; <label>:339:                                    ; preds = %336
  %340 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %157, align 8
  store %"struct.std::__1::__hash_node_base.217"* %340, %"struct.std::__1::__hash_node_base.217"** %22, align 8
  %341 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %22, align 8
  %342 = bitcast %"struct.std::__1::__hash_node_base.217"* %341 to %"struct.std::__1::__hash_node"*
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
  %374 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %157, align 8
  store %"struct.std::__1::__hash_node_base.217"* %374, %"struct.std::__1::__hash_node_base.217"** %156, align 8
  br label %717

; <label>:375:                                    ; preds = %368
  %376 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %161, i32 0, i32 0
  %377 = load i64, i64* %158, align 8
  store %"class.std::__1::unique_ptr.214"* %376, %"class.std::__1::unique_ptr.214"** %27, align 8
  store i64 %377, i64* %28, align 8
  %378 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %27, align 8
  %379 = load i64, i64* %28, align 8
  %380 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %378, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %380, %"class.std::__1::__compressed_pair.215"** %26, align 8
  %381 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %26, align 8
  %382 = bitcast %"class.std::__1::__compressed_pair.215"* %381 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %382, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %25, align 8
  %383 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %25, align 8
  %384 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %383, i32 0, i32 0
  %385 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %384, align 8
  %386 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %385, i64 %379
  %387 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %386, align 8
  %388 = icmp eq %"struct.std::__1::__hash_node_base.217"* %387, null
  br i1 %388, label %389, label %404

; <label>:389:                                    ; preds = %375
  %390 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %156, align 8
  %391 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %161, i32 0, i32 0
  %392 = load i64, i64* %158, align 8
  store %"class.std::__1::unique_ptr.214"* %391, %"class.std::__1::unique_ptr.214"** %31, align 8
  store i64 %392, i64* %32, align 8
  %393 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %31, align 8
  %394 = load i64, i64* %32, align 8
  %395 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %393, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %395, %"class.std::__1::__compressed_pair.215"** %30, align 8
  %396 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %30, align 8
  %397 = bitcast %"class.std::__1::__compressed_pair.215"* %396 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %397, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %29, align 8
  %398 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %29, align 8
  %399 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %398, i32 0, i32 0
  %400 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %399, align 8
  %401 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %400, i64 %394
  store %"struct.std::__1::__hash_node_base.217"* %390, %"struct.std::__1::__hash_node_base.217"** %401, align 8
  %402 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %157, align 8
  store %"struct.std::__1::__hash_node_base.217"* %402, %"struct.std::__1::__hash_node_base.217"** %156, align 8
  %403 = load i64, i64* %158, align 8
  store i64 %403, i64* %159, align 8
  br label %716

; <label>:404:                                    ; preds = %375
  %405 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %157, align 8
  store %"struct.std::__1::__hash_node_base.217"* %405, %"struct.std::__1::__hash_node_base.217"** %160, align 8
  br label %406

; <label>:406:                                    ; preds = %676, %404
  %407 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %160, align 8
  %408 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %407, i32 0, i32 0
  %409 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %408, align 8
  %410 = icmp ne %"struct.std::__1::__hash_node_base.217"* %409, null
  br i1 %410, label %411, label %673

; <label>:411:                                    ; preds = %406
  store %"class.std::__1::__hash_table.213"* %161, %"class.std::__1::__hash_table.213"** %35, align 8
  %412 = load %"class.std::__1::__hash_table.213"*, %"class.std::__1::__hash_table.213"** %35, align 8
  %413 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %412, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.230"* %413, %"class.std::__1::__compressed_pair.230"** %34, align 8
  %414 = load %"class.std::__1::__compressed_pair.230"*, %"class.std::__1::__compressed_pair.230"** %34, align 8
  %415 = bitcast %"class.std::__1::__compressed_pair.230"* %414 to %"class.std::__1::__libcpp_compressed_pair_imp.231"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.231"* %415, %"class.std::__1::__libcpp_compressed_pair_imp.231"** %33, align 8
  %416 = load %"class.std::__1::__libcpp_compressed_pair_imp.231"*, %"class.std::__1::__libcpp_compressed_pair_imp.231"** %33, align 8
  %417 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.231"* %416 to %"class.std::__1::__unordered_map_equal.232"*
  %418 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %157, align 8
  store %"struct.std::__1::__hash_node_base.217"* %418, %"struct.std::__1::__hash_node_base.217"** %38, align 8
  %419 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %38, align 8
  store %"struct.std::__1::__hash_node_base.217"* %419, %"struct.std::__1::__hash_node_base.217"** %37, align 8
  %420 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %37, align 8
  store %"struct.std::__1::__hash_node_base.217"* %420, %"struct.std::__1::__hash_node_base.217"** %36, align 8
  %421 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %36, align 8
  %422 = bitcast %"struct.std::__1::__hash_node_base.217"* %421 to i8*
  %423 = bitcast %"struct.std::__1::__hash_node_base.217"* %421 to %"struct.std::__1::__hash_node"*
  %424 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %423, i32 0, i32 2
  %425 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %160, align 8
  %426 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %425, i32 0, i32 0
  %427 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %426, align 8
  store %"struct.std::__1::__hash_node_base.217"* %427, %"struct.std::__1::__hash_node_base.217"** %41, align 8
  %428 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %41, align 8
  store %"struct.std::__1::__hash_node_base.217"* %428, %"struct.std::__1::__hash_node_base.217"** %40, align 8
  %429 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %40, align 8
  store %"struct.std::__1::__hash_node_base.217"* %429, %"struct.std::__1::__hash_node_base.217"** %39, align 8
  %430 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %39, align 8
  %431 = bitcast %"struct.std::__1::__hash_node_base.217"* %430 to i8*
  %432 = bitcast %"struct.std::__1::__hash_node_base.217"* %430 to %"struct.std::__1::__hash_node"*
  %433 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %432, i32 0, i32 2
  store %"class.std::__1::__unordered_map_equal.232"* %417, %"class.std::__1::__unordered_map_equal.232"** %138, align 8
  store %"union.std::__1::__hash_value_type"* %424, %"union.std::__1::__hash_value_type"** %139, align 8
  store %"union.std::__1::__hash_value_type"* %433, %"union.std::__1::__hash_value_type"** %140, align 8
  %434 = load %"class.std::__1::__unordered_map_equal.232"*, %"class.std::__1::__unordered_map_equal.232"** %138, align 8
  %435 = bitcast %"class.std::__1::__unordered_map_equal.232"* %434 to %"struct.std::__1::equal_to"*
  %436 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %139, align 8
  %437 = bitcast %"union.std::__1::__hash_value_type"* %436 to %"struct.std::__1::pair.237"*
  %438 = getelementptr inbounds %"struct.std::__1::pair.237", %"struct.std::__1::pair.237"* %437, i32 0, i32 0
  %439 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %140, align 8
  %440 = bitcast %"union.std::__1::__hash_value_type"* %439 to %"struct.std::__1::pair.237"*
  %441 = getelementptr inbounds %"struct.std::__1::pair.237", %"struct.std::__1::pair.237"* %440, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair"* %448, %"class.std::__1::__compressed_pair"** %126, align 8
  %449 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %126, align 8
  %450 = bitcast %"class.std::__1::__compressed_pair"* %449 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %450, %"class.std::__1::__libcpp_compressed_pair_imp"** %125, align 8
  %451 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %125, align 8
  %452 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %451, i32 0, i32 0
  %453 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %452, i32 0, i32 0
  %454 = bitcast %union.anon* %453 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %455 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %454, i32 0, i32 0
  %456 = bitcast %union.anon.0* %455 to i8*
  %457 = load i8, i8* %456, align 8
  %458 = zext i8 %457 to i32
  %459 = and i32 %458, 1
  %460 = icmp ne i32 %459, 0
  br i1 %460, label %461, label %472

; <label>:461:                                    ; preds = %411
  store %"class.std::__1::basic_string"* %446, %"class.std::__1::basic_string"** %121, align 8
  %462 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %121, align 8
  %463 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %462, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %463, %"class.std::__1::__compressed_pair"** %120, align 8
  %464 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %120, align 8
  %465 = bitcast %"class.std::__1::__compressed_pair"* %464 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %465, %"class.std::__1::__libcpp_compressed_pair_imp"** %119, align 8
  %466 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %119, align 8
  %467 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %466, i32 0, i32 0
  %468 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %467, i32 0, i32 0
  %469 = bitcast %union.anon* %468 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %470 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %469, i32 0, i32 1
  %471 = load i64, i64* %470, align 8
  br label %487

; <label>:472:                                    ; preds = %411
  store %"class.std::__1::basic_string"* %446, %"class.std::__1::basic_string"** %124, align 8
  %473 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %124, align 8
  %474 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %473, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %474, %"class.std::__1::__compressed_pair"** %123, align 8
  %475 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %123, align 8
  %476 = bitcast %"class.std::__1::__compressed_pair"* %475 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %476, %"class.std::__1::__libcpp_compressed_pair_imp"** %122, align 8
  %477 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %122, align 8
  %478 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %477, i32 0, i32 0
  %479 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %478, i32 0, i32 0
  %480 = bitcast %union.anon* %479 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %481 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %480, i32 0, i32 0
  %482 = bitcast %union.anon.0* %481 to i8*
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
  store %"class.std::__1::__compressed_pair"* %493, %"class.std::__1::__compressed_pair"** %116, align 8
  %494 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %116, align 8
  %495 = bitcast %"class.std::__1::__compressed_pair"* %494 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %495, %"class.std::__1::__libcpp_compressed_pair_imp"** %115, align 8
  %496 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %115, align 8
  %497 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %496, i32 0, i32 0
  %498 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %497, i32 0, i32 0
  %499 = bitcast %union.anon* %498 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %500 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %499, i32 0, i32 0
  %501 = bitcast %union.anon.0* %500 to i8*
  %502 = load i8, i8* %501, align 8
  %503 = zext i8 %502 to i32
  %504 = and i32 %503, 1
  %505 = icmp ne i32 %504, 0
  br i1 %505, label %506, label %517

; <label>:506:                                    ; preds = %487
  store %"class.std::__1::basic_string"* %491, %"class.std::__1::basic_string"** %111, align 8
  %507 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %111, align 8
  %508 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %507, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %508, %"class.std::__1::__compressed_pair"** %110, align 8
  %509 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %110, align 8
  %510 = bitcast %"class.std::__1::__compressed_pair"* %509 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %510, %"class.std::__1::__libcpp_compressed_pair_imp"** %109, align 8
  %511 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %109, align 8
  %512 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %511, i32 0, i32 0
  %513 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %512, i32 0, i32 0
  %514 = bitcast %union.anon* %513 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %515 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %514, i32 0, i32 1
  %516 = load i64, i64* %515, align 8
  br label %532

; <label>:517:                                    ; preds = %487
  store %"class.std::__1::basic_string"* %491, %"class.std::__1::basic_string"** %114, align 8
  %518 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %114, align 8
  %519 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %518, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %519, %"class.std::__1::__compressed_pair"** %113, align 8
  %520 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %113, align 8
  %521 = bitcast %"class.std::__1::__compressed_pair"* %520 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %521, %"class.std::__1::__libcpp_compressed_pair_imp"** %112, align 8
  %522 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %112, align 8
  %523 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %522, i32 0, i32 0
  %524 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %523, i32 0, i32 0
  %525 = bitcast %union.anon* %524 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %526 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %525, i32 0, i32 0
  %527 = bitcast %union.anon.0* %526 to i8*
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
  store %"class.std::__1::__compressed_pair"* %541, %"class.std::__1::__compressed_pair"** %105, align 8
  %542 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %105, align 8
  %543 = bitcast %"class.std::__1::__compressed_pair"* %542 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %543, %"class.std::__1::__libcpp_compressed_pair_imp"** %104, align 8
  %544 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %104, align 8
  %545 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %544, i32 0, i32 0
  %546 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %545, i32 0, i32 0
  %547 = bitcast %union.anon* %546 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %548 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %547, i32 0, i32 0
  %549 = bitcast %union.anon.0* %548 to i8*
  %550 = load i8, i8* %549, align 8
  %551 = zext i8 %550 to i32
  %552 = and i32 %551, 1
  %553 = icmp ne i32 %552, 0
  br i1 %553, label %554, label %565

; <label>:554:                                    ; preds = %536
  store %"class.std::__1::basic_string"* %539, %"class.std::__1::basic_string"** %98, align 8
  %555 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %98, align 8
  %556 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %555, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %556, %"class.std::__1::__compressed_pair"** %97, align 8
  %557 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %97, align 8
  %558 = bitcast %"class.std::__1::__compressed_pair"* %557 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %558, %"class.std::__1::__libcpp_compressed_pair_imp"** %96, align 8
  %559 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %96, align 8
  %560 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %559, i32 0, i32 0
  %561 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %560, i32 0, i32 0
  %562 = bitcast %union.anon* %561 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %563 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %562, i32 0, i32 2
  %564 = load i8*, i8** %563, align 8
  br label %578

; <label>:565:                                    ; preds = %536
  store %"class.std::__1::basic_string"* %539, %"class.std::__1::basic_string"** %103, align 8
  %566 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %103, align 8
  %567 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %566, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %567, %"class.std::__1::__compressed_pair"** %102, align 8
  %568 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %102, align 8
  %569 = bitcast %"class.std::__1::__compressed_pair"* %568 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %569, %"class.std::__1::__libcpp_compressed_pair_imp"** %101, align 8
  %570 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %101, align 8
  %571 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %570, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair"* %585, %"class.std::__1::__compressed_pair"** %88, align 8
  %586 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %88, align 8
  %587 = bitcast %"class.std::__1::__compressed_pair"* %586 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %587, %"class.std::__1::__libcpp_compressed_pair_imp"** %87, align 8
  %588 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %87, align 8
  %589 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %588, i32 0, i32 0
  %590 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %589, i32 0, i32 0
  %591 = bitcast %union.anon* %590 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %592 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %591, i32 0, i32 0
  %593 = bitcast %union.anon.0* %592 to i8*
  %594 = load i8, i8* %593, align 8
  %595 = zext i8 %594 to i32
  %596 = and i32 %595, 1
  %597 = icmp ne i32 %596, 0
  br i1 %597, label %598, label %609

; <label>:598:                                    ; preds = %578
  store %"class.std::__1::basic_string"* %583, %"class.std::__1::basic_string"** %81, align 8
  %599 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %81, align 8
  %600 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %599, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %600, %"class.std::__1::__compressed_pair"** %80, align 8
  %601 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %80, align 8
  %602 = bitcast %"class.std::__1::__compressed_pair"* %601 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %602, %"class.std::__1::__libcpp_compressed_pair_imp"** %79, align 8
  %603 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %79, align 8
  %604 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %603, i32 0, i32 0
  %605 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %604, i32 0, i32 0
  %606 = bitcast %union.anon* %605 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %607 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %606, i32 0, i32 2
  %608 = load i8*, i8** %607, align 8
  br label %622

; <label>:609:                                    ; preds = %578
  store %"class.std::__1::basic_string"* %583, %"class.std::__1::basic_string"** %86, align 8
  %610 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %86, align 8
  %611 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %610, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %611, %"class.std::__1::__compressed_pair"** %85, align 8
  %612 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %85, align 8
  %613 = bitcast %"class.std::__1::__compressed_pair"* %612 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %613, %"class.std::__1::__libcpp_compressed_pair_imp"** %84, align 8
  %614 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %84, align 8
  %615 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %614, i32 0, i32 0
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
  store %"class.std::__1::__compressed_pair"* %627, %"class.std::__1::__compressed_pair"** %93, align 8
  %628 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %93, align 8
  %629 = bitcast %"class.std::__1::__compressed_pair"* %628 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %629, %"class.std::__1::__libcpp_compressed_pair_imp"** %92, align 8
  %630 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %92, align 8
  %631 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %630, i32 0, i32 0
  %632 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %631, i32 0, i32 0
  %633 = bitcast %union.anon* %632 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %634 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %633, i32 0, i32 0
  %635 = bitcast %union.anon.0* %634 to i8*
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
  %677 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %160, align 8
  %678 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %677, i32 0, i32 0
  %679 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %678, align 8
  store %"struct.std::__1::__hash_node_base.217"* %679, %"struct.std::__1::__hash_node_base.217"** %160, align 8
  br label %406

; <label>:680:                                    ; preds = %673
  %681 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %160, align 8
  %682 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %681, i32 0, i32 0
  %683 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %682, align 8
  %684 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %156, align 8
  %685 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %684, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.217"* %683, %"struct.std::__1::__hash_node_base.217"** %685, align 8
  %686 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %161, i32 0, i32 0
  %687 = load i64, i64* %158, align 8
  store %"class.std::__1::unique_ptr.214"* %686, %"class.std::__1::unique_ptr.214"** %143, align 8
  store i64 %687, i64* %144, align 8
  %688 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %143, align 8
  %689 = load i64, i64* %144, align 8
  %690 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %688, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %690, %"class.std::__1::__compressed_pair.215"** %142, align 8
  %691 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %142, align 8
  %692 = bitcast %"class.std::__1::__compressed_pair.215"* %691 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %692, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %141, align 8
  %693 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %141, align 8
  %694 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %693, i32 0, i32 0
  %695 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %694, align 8
  %696 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %695, i64 %689
  %697 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %696, align 8
  %698 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %697, i32 0, i32 0
  %699 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %698, align 8
  %700 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %160, align 8
  %701 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %700, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.217"* %699, %"struct.std::__1::__hash_node_base.217"** %701, align 8
  %702 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %157, align 8
  %703 = getelementptr inbounds %"class.std::__1::__hash_table.213", %"class.std::__1::__hash_table.213"* %161, i32 0, i32 0
  %704 = load i64, i64* %158, align 8
  store %"class.std::__1::unique_ptr.214"* %703, %"class.std::__1::unique_ptr.214"** %147, align 8
  store i64 %704, i64* %148, align 8
  %705 = load %"class.std::__1::unique_ptr.214"*, %"class.std::__1::unique_ptr.214"** %147, align 8
  %706 = load i64, i64* %148, align 8
  %707 = getelementptr inbounds %"class.std::__1::unique_ptr.214", %"class.std::__1::unique_ptr.214"* %705, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.215"* %707, %"class.std::__1::__compressed_pair.215"** %146, align 8
  %708 = load %"class.std::__1::__compressed_pair.215"*, %"class.std::__1::__compressed_pair.215"** %146, align 8
  %709 = bitcast %"class.std::__1::__compressed_pair.215"* %708 to %"class.std::__1::__libcpp_compressed_pair_imp.216"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.216"* %709, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %145, align 8
  %710 = load %"class.std::__1::__libcpp_compressed_pair_imp.216"*, %"class.std::__1::__libcpp_compressed_pair_imp.216"** %145, align 8
  %711 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.216", %"class.std::__1::__libcpp_compressed_pair_imp.216"* %710, i32 0, i32 0
  %712 = load %"struct.std::__1::__hash_node_base.217"**, %"struct.std::__1::__hash_node_base.217"*** %711, align 8
  %713 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %712, i64 %706
  %714 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %713, align 8
  %715 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %714, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.217"* %702, %"struct.std::__1::__hash_node_base.217"** %715, align 8
  br label %716

; <label>:716:                                    ; preds = %680, %389
  br label %717

; <label>:717:                                    ; preds = %716, %373
  br label %718

; <label>:718:                                    ; preds = %717
  %719 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %156, align 8
  %720 = getelementptr inbounds %"struct.std::__1::__hash_node_base.217", %"struct.std::__1::__hash_node_base.217"* %719, i32 0, i32 0
  %721 = load %"struct.std::__1::__hash_node_base.217"*, %"struct.std::__1::__hash_node_base.217"** %720, align 8
  store %"struct.std::__1::__hash_node_base.217"* %721, %"struct.std::__1::__hash_node_base.217"** %157, align 8
  br label %336

; <label>:722:                                    ; preds = %336
  br label %723

; <label>:723:                                    ; preds = %722, %273
  br label %724

; <label>:724:                                    ; preds = %723, %236
  ret void
}

; Function Attrs: nobuiltin nounwind
declare void @_ZdlPv(i8*) #10

; Function Attrs: nounwind readnone
declare i64 @llvm.ctlz.i64(i64, i1) #11

; Function Attrs: nounwind readnone
declare float @ceilf(float) #12

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEED1Ev(%"struct.std::__1::pair.237"*) unnamed_addr #8 align 2 {
  %2 = alloca %"struct.std::__1::pair.237"*, align 8
  store %"struct.std::__1::pair.237"* %0, %"struct.std::__1::pair.237"** %2, align 8
  %3 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %2, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEED2Ev(%"struct.std::__1::pair.237"* %3) #14
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPN4llvm8ConstantEED2Ev(%"struct.std::__1::pair.237"*) unnamed_addr #8 align 2 {
  %2 = alloca %"struct.std::__1::pair.237"*, align 8
  store %"struct.std::__1::pair.237"* %0, %"struct.std::__1::pair.237"** %2, align 8
  %3 = load %"struct.std::__1::pair.237"*, %"struct.std::__1::pair.237"** %2, align 8
  %4 = getelementptr inbounds %"struct.std::__1::pair.237", %"struct.std::__1::pair.237"* %3, i32 0, i32 0
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %4) #14
  ret void
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr %"class.llvm::PointerType"* @_ZN4llvm4castINS_11PointerTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Type"*) #7 {
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
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm4castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.16, i32 0, i32 0), i32 236, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.19, i32 0, i32 0)) #17
  unreachable
                                                  ; No predecessors!
  br label %11

; <label>:10:                                     ; preds = %5
  br label %11

; <label>:11:                                     ; preds = %10, %9
  %12 = call %"class.llvm::PointerType"* @_ZN4llvm16cast_convert_valINS_11PointerTypeEPNS_4TypeES3_E4doitERKS3_(%"class.llvm::Type"** dereferenceable(8) %2)
  ret %"class.llvm::PointerType"* %12
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr %"class.llvm::FunctionType"* @_ZN4llvm4castINS_12FunctionTypeENS_4TypeEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Type"*) #7 {
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
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm4castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.16, i32 0, i32 0), i32 236, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.19, i32 0, i32 0)) #17
  unreachable
                                                  ; No predecessors!
  br label %11

; <label>:10:                                     ; preds = %5
  br label %11

; <label>:11:                                     ; preds = %10, %9
  %12 = call %"class.llvm::FunctionType"* @_ZN4llvm16cast_convert_valINS_12FunctionTypeEPNS_4TypeES3_E4doitERKS3_(%"class.llvm::Type"** dereferenceable(8) %2)
  ret %"class.llvm::FunctionType"* %12
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.llvm::Type"* @_ZNK4llvm11PointerType14getElementTypeEv(%"class.llvm::PointerType"*) #4 align 2 {
  %2 = alloca %"class.llvm::PointerType"*, align 8
  store %"class.llvm::PointerType"* %0, %"class.llvm::PointerType"** %2, align 8
  %3 = load %"class.llvm::PointerType"*, %"class.llvm::PointerType"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::PointerType", %"class.llvm::PointerType"* %3, i32 0, i32 1
  %5 = load %"class.llvm::Type"*, %"class.llvm::Type"** %4, align 8
  ret %"class.llvm::Type"* %5
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::CallInst"* @_ZN4llvm9IRBuilderINS_14ConstantFolderENS_24IRBuilderDefaultInserterEE10CreateCallEPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS7_EERKNS_5TwineEPNS_6MDNodeE(%"class.llvm::IRBuilder"*, %"class.llvm::FunctionType"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::MDNode"*) #0 align 2 {
  %8 = alloca %"class.llvm::ArrayRef.211", align 8
  %9 = alloca %"class.llvm::IRBuilder"*, align 8
  %10 = alloca %"class.llvm::FunctionType"*, align 8
  %11 = alloca %"class.llvm::Value"*, align 8
  %12 = alloca %"class.llvm::Twine"*, align 8
  %13 = alloca %"class.llvm::MDNode"*, align 8
  %14 = alloca %"class.llvm::CallInst"*, align 8
  %15 = alloca %"class.llvm::ArrayRef.211", align 8
  %16 = alloca %"class.llvm::ArrayRef", align 8
  %17 = alloca %"class.llvm::Twine", align 8
  %18 = alloca %"class.llvm::FastMathFlags", align 4
  %19 = bitcast %"class.llvm::ArrayRef.211"* %8 to { %"class.llvm::Value"**, i64 }*
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
  %25 = bitcast %"class.llvm::ArrayRef.211"* %15 to i8*
  %26 = bitcast %"class.llvm::ArrayRef.211"* %8 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %25, i8* %26, i64 16, i32 8, i1 false)
  %27 = bitcast %"class.llvm::IRBuilder"* %22 to %"class.llvm::IRBuilderBase"*
  %28 = getelementptr inbounds %"class.llvm::IRBuilderBase", %"class.llvm::IRBuilderBase"* %27, i32 0, i32 6
  %29 = bitcast %"class.llvm::ArrayRef"* %16 to i8*
  %30 = bitcast %"class.llvm::ArrayRef"* %28 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %29, i8* %30, i64 16, i32 8, i1 false)
  call void @_ZN4llvm5TwineC1EPKc(%"class.llvm::Twine"* %17, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i32 0, i32 0))
  %31 = bitcast %"class.llvm::ArrayRef.211"* %15 to { %"class.llvm::Value"**, i64 }*
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
define linkonce_odr zeroext i1 @_ZN4llvm3isaINS_11PointerTypeEPNS_4TypeEEEbRKT0_(%"class.llvm::Type"** dereferenceable(8)) #7 {
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
define linkonce_odr zeroext i1 @_ZN4llvm11isa_impl_clINS_11PointerTypeEPKNS_4TypeEE4doitES4_(%"class.llvm::Type"*) #7 align 2 {
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
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.16, i32 0, i32 0), i32 95, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.17, i32 0, i32 0)) #17
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

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm8isa_implINS_11PointerTypeENS_4TypeEvE4doitERKS2_(%"class.llvm::Type"* dereferenceable(24)) #7 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call i32 @_ZNK4llvm4Type9getTypeIDEv(%"class.llvm::Type"* %3)
  %5 = icmp eq i32 %4, 15
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

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm3isaINS_12FunctionTypeEPNS_4TypeEEEbRKT0_(%"class.llvm::Type"** dereferenceable(8)) #7 {
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
define linkonce_odr zeroext i1 @_ZN4llvm11isa_impl_clINS_12FunctionTypeEPKNS_4TypeEE4doitES4_(%"class.llvm::Type"*) #7 align 2 {
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
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.16, i32 0, i32 0), i32 95, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.17, i32 0, i32 0)) #17
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
define linkonce_odr zeroext i1 @_ZN4llvm8isa_implINS_12FunctionTypeENS_4TypeEvE4doitERKS2_(%"class.llvm::Type"* dereferenceable(24)) #7 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm12FunctionType7classofEPKNS_4TypeE(%"class.llvm::Type"* %3)
  ret i1 %4
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm12FunctionType7classofEPKNS_4TypeE(%"class.llvm::Type"*) #8 align 2 {
  %2 = alloca %"class.llvm::Type"*, align 8
  store %"class.llvm::Type"* %0, %"class.llvm::Type"** %2, align 8
  %3 = load %"class.llvm::Type"*, %"class.llvm::Type"** %2, align 8
  %4 = call i32 @_ZNK4llvm4Type9getTypeIDEv(%"class.llvm::Type"* %3)
  %5 = icmp eq i32 %4, 12
  ret i1 %5
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.llvm::CallInst"* @_ZN4llvm8CallInst6CreateEPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS4_EENS5_INS_17OperandBundleDefTIS4_EEEERKNS_5TwineEPNS_11InstructionE(%"class.llvm::FunctionType"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, %"class.llvm::OperandBundleDefT"*, i64, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::Instruction"*) #0 align 2 {
  %9 = alloca %"class.llvm::ArrayRef.211", align 8
  %10 = alloca %"class.llvm::ArrayRef", align 8
  %11 = alloca %"class.llvm::FunctionType"*, align 8
  %12 = alloca %"class.llvm::Value"*, align 8
  %13 = alloca %"class.llvm::Twine"*, align 8
  %14 = alloca %"class.llvm::Instruction"*, align 8
  %15 = alloca i32, align 4
  %16 = alloca %"class.llvm::ArrayRef", align 8
  %17 = alloca i32, align 4
  %18 = alloca %"class.llvm::ArrayRef.211", align 8
  %19 = alloca %"class.llvm::ArrayRef", align 8
  %20 = bitcast %"class.llvm::ArrayRef.211"* %9 to { %"class.llvm::Value"**, i64 }*
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
  %26 = call i64 @_ZNK4llvm8ArrayRefIPNS_5ValueEE4sizeEv(%"class.llvm::ArrayRef.211"* %9)
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
  %47 = bitcast %"class.llvm::ArrayRef.211"* %18 to i8*
  %48 = bitcast %"class.llvm::ArrayRef.211"* %9 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %47, i8* %48, i64 16, i32 8, i1 false)
  %49 = bitcast %"class.llvm::ArrayRef"* %19 to i8*
  %50 = bitcast %"class.llvm::ArrayRef"* %10 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %49, i8* %50, i64 16, i32 8, i1 false)
  %51 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %13, align 8
  %52 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %14, align 8
  %53 = bitcast %"class.llvm::ArrayRef.211"* %18 to { %"class.llvm::Value"**, i64 }*
  %54 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %53, i32 0, i32 0
  %55 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %54, align 8
  %56 = getelementptr inbounds { %"class.llvm::Value"**, i64 }, { %"class.llvm::Value"**, i64 }* %53, i32 0, i32 1
  %57 = load i64, i64* %56, align 8
  call void @_ZN4llvm8CallInstC1EPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS4_EENS5_INS_17OperandBundleDefTIS4_EEEERKNS_5TwineEPNS_11InstructionE(%"class.llvm::CallInst"* %44, %"class.llvm::FunctionType"* %45, %"class.llvm::Value"* %46, %"class.llvm::Value"** %55, i64 %57, %"class.llvm::ArrayRef"* byval align 8 %19, %"class.llvm::Twine"* dereferenceable(24) %51, %"class.llvm::Instruction"* %52)
  ret %"class.llvm::CallInst"* %44
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm3isaINS_14FPMathOperatorEPNS_8CallInstEEEbRKT0_(%"class.llvm::CallInst"** dereferenceable(8)) #7 {
  %2 = alloca %"class.llvm::CallInst"**, align 8
  store %"class.llvm::CallInst"** %0, %"class.llvm::CallInst"*** %2, align 8
  %3 = load %"class.llvm::CallInst"**, %"class.llvm::CallInst"*** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm13isa_impl_wrapINS_14FPMathOperatorEKPNS_8CallInstEPKS2_E4doitERS4_(%"class.llvm::CallInst"** dereferenceable(8) %3)
  ret i1 %4
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr %"class.llvm::CallInst"* @_ZN4llvm4castINS_8CallInstENS_11InstructionEEENS_10cast_rettyIT_PT0_E8ret_typeES6_(%"class.llvm::Instruction"*) #7 {
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
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm4castINS_8ConstantENS_5ValueEEENS_10cast_rettyIT_PT0_E8ret_typeES6_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.16, i32 0, i32 0), i32 236, i8* getelementptr inbounds ([59 x i8], [59 x i8]* @.str.19, i32 0, i32 0)) #17
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
  %21 = load %"class.llvm::ilist_node_impl.60"*, %"class.llvm::ilist_node_impl.60"** %20, align 8
  call void @_ZNK4llvm24IRBuilderDefaultInserter12InsertHelperEPNS_11InstructionERKNS_5TwineEPNS_10BasicBlockENS_14ilist_iteratorINS_12ilist_detail12node_optionsIS1_Lb0ELb0EvEELb0ELb0EEE(%"class.llvm::IRBuilderDefaultInserter"* %9, %"class.llvm::Instruction"* %11, %"class.llvm::Twine"* dereferenceable(24) %12, %"class.llvm::BasicBlock"* %15, %"class.llvm::ilist_node_impl.60"* %21)
  %22 = bitcast %"class.llvm::IRBuilder"* %8 to %"class.llvm::IRBuilderBase"*
  %23 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %5, align 8
  %24 = bitcast %"class.llvm::CallInst"* %23 to %"class.llvm::Instruction"*
  call void @_ZNK4llvm13IRBuilderBase20SetInstDebugLocationEPNS_11InstructionE(%"class.llvm::IRBuilderBase"* %22, %"class.llvm::Instruction"* %24)
  %25 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %5, align 8
  ret %"class.llvm::CallInst"* %25
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNK4llvm8ArrayRefIPNS_5ValueEE4sizeEv(%"class.llvm::ArrayRef.211"*) #4 align 2 {
  %2 = alloca %"class.llvm::ArrayRef.211"*, align 8
  store %"class.llvm::ArrayRef.211"* %0, %"class.llvm::ArrayRef.211"** %2, align 8
  %3 = load %"class.llvm::ArrayRef.211"*, %"class.llvm::ArrayRef.211"** %2, align 8
  %4 = getelementptr inbounds %"class.llvm::ArrayRef.211", %"class.llvm::ArrayRef.211"* %3, i32 0, i32 1
  %5 = load i64, i64* %4, align 8
  ret i64 %5
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
define linkonce_odr void @_ZN4llvm8CallInstC1EPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS4_EENS5_INS_17OperandBundleDefTIS4_EEEERKNS_5TwineEPNS_11InstructionE(%"class.llvm::CallInst"*, %"class.llvm::FunctionType"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, %"class.llvm::ArrayRef"* byval align 8, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::Instruction"*) unnamed_addr #7 align 2 {
  %9 = alloca %"class.llvm::ArrayRef.211", align 8
  %10 = alloca %"class.llvm::CallInst"*, align 8
  %11 = alloca %"class.llvm::FunctionType"*, align 8
  %12 = alloca %"class.llvm::Value"*, align 8
  %13 = alloca %"class.llvm::Twine"*, align 8
  %14 = alloca %"class.llvm::Instruction"*, align 8
  %15 = bitcast %"class.llvm::ArrayRef.211"* %9 to { %"class.llvm::Value"**, i64 }*
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
  %23 = bitcast %"class.llvm::ArrayRef.211"* %9 to { %"class.llvm::Value"**, i64 }*
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
  %2 = alloca %"class.std::__1::vector.203"*, align 8
  %3 = alloca %"class.llvm::OperandBundleDefT"*, align 8
  store %"class.llvm::OperandBundleDefT"* %0, %"class.llvm::OperandBundleDefT"** %3, align 8
  %4 = load %"class.llvm::OperandBundleDefT"*, %"class.llvm::OperandBundleDefT"** %3, align 8
  %5 = getelementptr inbounds %"class.llvm::OperandBundleDefT", %"class.llvm::OperandBundleDefT"* %4, i32 0, i32 1
  store %"class.std::__1::vector.203"* %5, %"class.std::__1::vector.203"** %2, align 8
  %6 = load %"class.std::__1::vector.203"*, %"class.std::__1::vector.203"** %2, align 8
  %7 = bitcast %"class.std::__1::vector.203"* %6 to %"class.std::__1::__vector_base.204"*
  %8 = getelementptr inbounds %"class.std::__1::__vector_base.204", %"class.std::__1::__vector_base.204"* %7, i32 0, i32 1
  %9 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %8, align 8
  %10 = bitcast %"class.std::__1::vector.203"* %6 to %"class.std::__1::__vector_base.204"*
  %11 = getelementptr inbounds %"class.std::__1::__vector_base.204", %"class.std::__1::__vector_base.204"* %10, i32 0, i32 0
  %12 = load %"class.llvm::Value"**, %"class.llvm::Value"*** %11, align 8
  %13 = ptrtoint %"class.llvm::Value"** %9 to i64
  %14 = ptrtoint %"class.llvm::Value"** %12 to i64
  %15 = sub i64 %13, %14
  %16 = sdiv exact i64 %15, 8
  ret i64 %16
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZN4llvm8CallInstC2EPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS4_EENS5_INS_17OperandBundleDefTIS4_EEEERKNS_5TwineEPNS_11InstructionE(%"class.llvm::CallInst"*, %"class.llvm::FunctionType"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, %"class.llvm::ArrayRef"* byval align 8, %"class.llvm::Twine"* dereferenceable(24), %"class.llvm::Instruction"*) unnamed_addr #7 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %9 = alloca %"class.llvm::ArrayRef.211", align 8
  %10 = alloca %"class.llvm::CallInst"*, align 8
  %11 = alloca %"class.llvm::FunctionType"*, align 8
  %12 = alloca %"class.llvm::Value"*, align 8
  %13 = alloca %"class.llvm::Twine"*, align 8
  %14 = alloca %"class.llvm::Instruction"*, align 8
  %15 = alloca %"class.llvm::ArrayRef", align 8
  %16 = alloca %"class.llvm::ArrayRef", align 8
  %17 = alloca i8*
  %18 = alloca i32
  %19 = alloca %"class.llvm::ArrayRef.211", align 8
  %20 = alloca %"class.llvm::ArrayRef", align 8
  %21 = bitcast %"class.llvm::ArrayRef.211"* %9 to { %"class.llvm::Value"**, i64 }*
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
  %30 = call i64 @_ZNK4llvm8ArrayRefIPNS_5ValueEE4sizeEv(%"class.llvm::ArrayRef.211"* %9)
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
  %44 = call i64 @_ZNK4llvm8ArrayRefIPNS_5ValueEE4sizeEv(%"class.llvm::ArrayRef.211"* %9)
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
  %64 = bitcast %"class.llvm::ArrayRef.211"* %19 to i8*
  %65 = bitcast %"class.llvm::ArrayRef.211"* %9 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %64, i8* %65, i64 16, i32 8, i1 false)
  %66 = bitcast %"class.llvm::ArrayRef"* %20 to i8*
  %67 = bitcast %"class.llvm::ArrayRef"* %5 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %66, i8* %67, i64 16, i32 8, i1 false)
  %68 = load %"class.llvm::Twine"*, %"class.llvm::Twine"** %13, align 8
  %69 = bitcast %"class.llvm::ArrayRef.211"* %19 to { %"class.llvm::Value"**, i64 }*
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

declare void @_ZN4llvm11InstructionC2EPNS_4TypeEjPNS_3UseEjPS0_(%"class.llvm::Instruction"*, %"class.llvm::Type"*, i32, %"class.llvm::Use"*, i32, %"class.llvm::Instruction"*) unnamed_addr #1

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZN4llvm12AttributeSetC1Ev(%"class.llvm::AttributeSet"*) unnamed_addr #0 align 2 {
  %2 = alloca %"class.llvm::AttributeSet"*, align 8
  store %"class.llvm::AttributeSet"* %0, %"class.llvm::AttributeSet"** %2, align 8
  %3 = load %"class.llvm::AttributeSet"*, %"class.llvm::AttributeSet"** %2, align 8
  call void @_ZN4llvm12AttributeSetC2Ev(%"class.llvm::AttributeSet"* %3)
  ret void
}

declare void @_ZN4llvm8CallInst4initEPNS_12FunctionTypeEPNS_5ValueENS_8ArrayRefIS4_EENS5_INS_17OperandBundleDefTIS4_EEEERKNS_5TwineE(%"class.llvm::CallInst"*, %"class.llvm::FunctionType"*, %"class.llvm::Value"*, %"class.llvm::Value"**, i64, %"class.llvm::ArrayRef"* byval align 8, %"class.llvm::Twine"* dereferenceable(24)) #1

; Function Attrs: nounwind
declare void @_ZN4llvm11InstructionD2Ev(%"class.llvm::Instruction"*) unnamed_addr #2

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
define linkonce_odr zeroext i1 @_ZN4llvm11isa_impl_clINS_14FPMathOperatorEPKNS_8CallInstEE4doitES4_(%"class.llvm::CallInst"*) #7 align 2 {
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
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.16, i32 0, i32 0), i32 95, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.17, i32 0, i32 0)) #17
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
define linkonce_odr zeroext i1 @_ZN4llvm8isa_implINS_14FPMathOperatorENS_8CallInstEvE4doitERKS2_(%"class.llvm::CallInst"* dereferenceable(80)) #7 align 2 {
  %2 = alloca %"class.llvm::CallInst"*, align 8
  store %"class.llvm::CallInst"* %0, %"class.llvm::CallInst"** %2, align 8
  %3 = load %"class.llvm::CallInst"*, %"class.llvm::CallInst"** %2, align 8
  %4 = bitcast %"class.llvm::CallInst"* %3 to %"class.llvm::Instruction"*
  %5 = call zeroext i1 @_ZN4llvm14FPMathOperator7classofEPKNS_11InstructionE(%"class.llvm::Instruction"* %4)
  ret i1 %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm14FPMathOperator7classofEPKNS_11InstructionE(%"class.llvm::Instruction"*) #7 align 2 {
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
  %4 = call %"class.llvm::Type"* @_ZNK4llvm4Type13getScalarTypeEv(%"class.llvm::Type"* %3) #19
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

; Function Attrs: nounwind readonly
declare %"class.llvm::Type"* @_ZNK4llvm4Type13getScalarTypeEv(%"class.llvm::Type"*) #13

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
define linkonce_odr zeroext i1 @_ZN4llvm3isaINS_8CallInstEPNS_11InstructionEEEbRKT0_(%"class.llvm::Instruction"** dereferenceable(8)) #7 {
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
define linkonce_odr zeroext i1 @_ZN4llvm11isa_impl_clINS_8CallInstEPKNS_11InstructionEE4doitES4_(%"class.llvm::Instruction"*) #7 align 2 {
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
  call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__._ZN4llvm11isa_impl_clINS_8ConstantEPKNS_5ValueEE4doitES4_, i32 0, i32 0), i8* getelementptr inbounds ([62 x i8], [62 x i8]* @.str.16, i32 0, i32 0), i32 95, i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.17, i32 0, i32 0)) #17
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
define linkonce_odr zeroext i1 @_ZN4llvm8isa_implINS_8CallInstENS_11InstructionEvE4doitERKS2_(%"class.llvm::Instruction"* dereferenceable(64)) #7 align 2 {
  %2 = alloca %"class.llvm::Instruction"*, align 8
  store %"class.llvm::Instruction"* %0, %"class.llvm::Instruction"** %2, align 8
  %3 = load %"class.llvm::Instruction"*, %"class.llvm::Instruction"** %2, align 8
  %4 = call zeroext i1 @_ZN4llvm8CallInst7classofEPKNS_11InstructionE(%"class.llvm::Instruction"* %3)
  ret i1 %4
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZN4llvm8CallInst7classofEPKNS_11InstructionE(%"class.llvm::Instruction"*) #8 align 2 {
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
define linkonce_odr void @_ZN4llvm8ArrayRefIPNS_5ValueEEC2ERKSt16initializer_listIS2_E(%"class.llvm::ArrayRef.211"*, %"class.std::initializer_list"* dereferenceable(16)) unnamed_addr #4 align 2 {
  %3 = alloca %"class.std::initializer_list"*, align 8
  %4 = alloca %"class.std::initializer_list"*, align 8
  %5 = alloca %"class.std::initializer_list"*, align 8
  %6 = alloca %"class.std::initializer_list"*, align 8
  %7 = alloca %"class.llvm::ArrayRef.211"*, align 8
  %8 = alloca %"class.std::initializer_list"*, align 8
  store %"class.llvm::ArrayRef.211"* %0, %"class.llvm::ArrayRef.211"** %7, align 8
  store %"class.std::initializer_list"* %1, %"class.std::initializer_list"** %8, align 8
  %9 = load %"class.llvm::ArrayRef.211"*, %"class.llvm::ArrayRef.211"** %7, align 8
  %10 = getelementptr inbounds %"class.llvm::ArrayRef.211", %"class.llvm::ArrayRef.211"* %9, i32 0, i32 0
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
  %31 = getelementptr inbounds %"class.llvm::ArrayRef.211", %"class.llvm::ArrayRef.211"* %9, i32 0, i32 1
  %32 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %8, align 8
  store %"class.std::initializer_list"* %32, %"class.std::initializer_list"** %5, align 8
  %33 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %5, align 8
  %34 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %33, i32 0, i32 1
  %35 = load i64, i64* %34, align 8
  store i64 %35, i64* %31, align 8
  ret void
}

declare dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"*, i64, i8*) #1

attributes #0 = { ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { noreturn "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { noinline noreturn nounwind }
attributes #6 = { argmemonly nounwind }
attributes #7 = { inlinehint ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { inlinehint nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #9 = { nobuiltin "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #10 = { nobuiltin nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #11 = { nounwind readnone }
attributes #12 = { nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #13 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #14 = { nounwind }
attributes #15 = { noreturn nounwind }
attributes #16 = { builtin nounwind }
attributes #17 = { noreturn }
attributes #18 = { builtin }
attributes #19 = { nounwind readonly }

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