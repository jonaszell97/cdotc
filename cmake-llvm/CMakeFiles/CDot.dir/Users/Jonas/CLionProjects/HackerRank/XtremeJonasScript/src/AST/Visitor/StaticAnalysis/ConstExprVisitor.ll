; ModuleID = '/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/AST/Visitor/StaticAnalysis/ConstExprVisitor.cpp'
source_filename = "/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/AST/Visitor/StaticAnalysis/ConstExprVisitor.cpp"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.12.0"

%struct.Variant = type <{ %union.anon.44, i8, [7 x i8], %"class.std::__1::basic_string", %struct.TypeSpecifier, i8, i8, i8, [5 x i8] }>
%union.anon.44 = type { i64 }
%"class.std::__1::basic_string" = type { %"class.std::__1::__compressed_pair" }
%"class.std::__1::__compressed_pair" = type { %"class.std::__1::__libcpp_compressed_pair_imp" }
%"class.std::__1::__libcpp_compressed_pair_imp" = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" = type { %union.anon }
%union.anon = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" = type { i64, i64, i8* }
%struct.TypeSpecifier = type { i32, %"class.std::__1::basic_string", %"class.std::__1::shared_ptr", i8, i8, i8, i8, i64, i32, i8, i8, %"class.std::__1::basic_string", i8, [7 x i8], %"class.std::__1::unordered_map", i8, i8, i8, i8, i8, i8, i8, i32, %"class.std::__1::vector", %"class.std::__1::vector.19", %union.anon.25 }
%"class.std::__1::shared_ptr" = type { %class.Expression*, %"class.std::__1::__shared_weak_count"* }
%class.Expression = type { %class.Statement.base, %"class.std::__1::shared_ptr", i8, %"class.std::__1::shared_ptr", i8, %"class.std::__1::unique_ptr.41", i8, %struct.TypeSpecifier }
%class.Statement.base = type { %class.AstNode.base }
%class.AstNode.base = type <{ i32 (...)**, i32, i32, i64, %class.AstNode*, %class.DeclStmt*, i8, [3 x i8], i32, %"class.std::__1::vector", %struct.TypeSpecifier, %struct.TypeSpecifier, i8, [7 x i8], %"class.std::__1::basic_string", i8, i8 }>
%class.AstNode = type <{ i32 (...)**, i32, i32, i64, %class.AstNode*, %class.DeclStmt*, i8, [3 x i8], i32, %"class.std::__1::vector", %struct.TypeSpecifier, %struct.TypeSpecifier, i8, [7 x i8], %"class.std::__1::basic_string", i8, i8, [6 x i8] }>
%class.DeclStmt = type <{ %class.Statement.base, [6 x i8], %"class.std::__1::vector", %"class.std::__1::vector.45", %"class.std::__1::vector", %"class.std::__1::vector.51", i8, i8, [6 x i8] }>
%"class.std::__1::vector.45" = type { %"class.std::__1::__vector_base.46" }
%"class.std::__1::__vector_base.46" = type { %"struct.std::__1::pair"*, %"struct.std::__1::pair"*, %"class.std::__1::__compressed_pair.47" }
%"struct.std::__1::pair" = type opaque
%"class.std::__1::__compressed_pair.47" = type { %"class.std::__1::__libcpp_compressed_pair_imp.48" }
%"class.std::__1::__libcpp_compressed_pair_imp.48" = type { %"struct.std::__1::pair"* }
%"class.std::__1::vector.51" = type { i64*, i64, %"class.std::__1::__compressed_pair.52" }
%"class.std::__1::__compressed_pair.52" = type { %"class.std::__1::__libcpp_compressed_pair_imp.53" }
%"class.std::__1::__libcpp_compressed_pair_imp.53" = type { i64 }
%"class.std::__1::unique_ptr.41" = type { %"class.std::__1::__compressed_pair.42" }
%"class.std::__1::__compressed_pair.42" = type { %"class.std::__1::__libcpp_compressed_pair_imp.43" }
%"class.std::__1::__libcpp_compressed_pair_imp.43" = type { %struct.Variant* }
%"class.std::__1::__shared_weak_count" = type { %"class.std::__1::__shared_count", i64 }
%"class.std::__1::__shared_count" = type { i32 (...)**, i64 }
%"class.std::__1::unordered_map" = type { %"class.std::__1::__hash_table" }
%"class.std::__1::__hash_table" = type <{ %"class.std::__1::unique_ptr", %"class.std::__1::__compressed_pair.11", %"class.std::__1::__compressed_pair.15", %"class.std::__1::__compressed_pair.17", [4 x i8] }>
%"class.std::__1::unique_ptr" = type { %"class.std::__1::__compressed_pair.5" }
%"class.std::__1::__compressed_pair.5" = type { %"class.std::__1::__libcpp_compressed_pair_imp.6" }
%"class.std::__1::__libcpp_compressed_pair_imp.6" = type { %"struct.std::__1::__hash_node_base"**, %"class.std::__1::__bucket_list_deallocator" }
%"struct.std::__1::__hash_node_base" = type { %"struct.std::__1::__hash_node_base"* }
%"class.std::__1::__bucket_list_deallocator" = type { %"class.std::__1::__compressed_pair.7" }
%"class.std::__1::__compressed_pair.7" = type { %"class.std::__1::__libcpp_compressed_pair_imp.8" }
%"class.std::__1::__libcpp_compressed_pair_imp.8" = type { i64 }
%"class.std::__1::__compressed_pair.11" = type { %"class.std::__1::__libcpp_compressed_pair_imp.12" }
%"class.std::__1::__libcpp_compressed_pair_imp.12" = type { %"struct.std::__1::__hash_node_base" }
%"class.std::__1::__compressed_pair.15" = type { %"class.std::__1::__libcpp_compressed_pair_imp.16" }
%"class.std::__1::__libcpp_compressed_pair_imp.16" = type { i64 }
%"class.std::__1::__compressed_pair.17" = type { %"class.std::__1::__libcpp_compressed_pair_imp.18" }
%"class.std::__1::__libcpp_compressed_pair_imp.18" = type { float }
%"class.std::__1::vector" = type { %"class.std::__1::__vector_base" }
%"class.std::__1::__vector_base" = type { %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"*, %"class.std::__1::__compressed_pair.1" }
%"class.std::__1::__compressed_pair.1" = type { %"class.std::__1::__libcpp_compressed_pair_imp.2" }
%"class.std::__1::__libcpp_compressed_pair_imp.2" = type { %"class.std::__1::basic_string"* }
%"class.std::__1::vector.19" = type { %"class.std::__1::__vector_base.20" }
%"class.std::__1::__vector_base.20" = type { %struct.TypeSpecifier*, %struct.TypeSpecifier*, %"class.std::__1::__compressed_pair.21" }
%"class.std::__1::__compressed_pair.21" = type { %"class.std::__1::__libcpp_compressed_pair_imp.22" }
%"class.std::__1::__libcpp_compressed_pair_imp.22" = type { %struct.TypeSpecifier* }
%union.anon.25 = type { %struct.TypeSpecifier* }
%class.ConstExprVisitor = type { %class.Visitor }
%class.Visitor = type { i32 (...)** }
%class.NamespaceDecl = type { %class.Statement.base, %"class.std::__1::basic_string", %"class.std::__1::shared_ptr.26" }
%"class.std::__1::shared_ptr.26" = type { %class.CompoundStmt*, %"class.std::__1::__shared_weak_count"* }
%class.CompoundStmt = type <{ %class.Statement.base, [6 x i8], %"class.std::__1::enable_shared_from_this", i8, i8, [6 x i8], %"class.std::__1::vector.27", %"class.std::__1::vector.34", i32, [4 x i8] }>
%"class.std::__1::enable_shared_from_this" = type { %"class.std::__1::weak_ptr" }
%"class.std::__1::weak_ptr" = type { %class.CompoundStmt*, %"class.std::__1::__shared_weak_count"* }
%"class.std::__1::vector.27" = type { %"class.std::__1::__vector_base.28" }
%"class.std::__1::__vector_base.28" = type { %"class.std::__1::shared_ptr.29"*, %"class.std::__1::shared_ptr.29"*, %"class.std::__1::__compressed_pair.30" }
%"class.std::__1::shared_ptr.29" = type { %class.Statement*, %"class.std::__1::__shared_weak_count"* }
%class.Statement = type { %class.AstNode.base, [6 x i8] }
%"class.std::__1::__compressed_pair.30" = type { %"class.std::__1::__libcpp_compressed_pair_imp.31" }
%"class.std::__1::__libcpp_compressed_pair_imp.31" = type { %"class.std::__1::shared_ptr.29"* }
%"class.std::__1::vector.34" = type { %"class.std::__1::__vector_base.35" }
%"class.std::__1::__vector_base.35" = type { %"class.std::__1::shared_ptr.36"*, %"class.std::__1::shared_ptr.36"*, %"class.std::__1::__compressed_pair.37" }
%"class.std::__1::shared_ptr.36" = type { %class.ImportStmt*, %"class.std::__1::__shared_weak_count"* }
%class.ImportStmt = type { %class.Statement.base, %"class.std::__1::vector" }
%"class.std::__1::__compressed_pair.37" = type { %"class.std::__1::__libcpp_compressed_pair_imp.38" }
%"class.std::__1::__libcpp_compressed_pair_imp.38" = type { %"class.std::__1::shared_ptr.36"* }
%"class.std::__1::shared_ptr.58" = type { %class.AstNode*, %"class.std::__1::__shared_weak_count"* }
%"class.std::__1::__wrap_iter" = type { %"class.std::__1::shared_ptr.58"* }
%"class.std::__1::vector.56" = type { %"class.std::__1::__vector_base.57" }
%"class.std::__1::__vector_base.57" = type { %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"*, %"class.std::__1::__compressed_pair.59" }
%"class.std::__1::__compressed_pair.59" = type { %"class.std::__1::__libcpp_compressed_pair_imp.60" }
%"class.std::__1::__libcpp_compressed_pair_imp.60" = type { %"class.std::__1::shared_ptr.58"* }
%class.FunctionDecl = type { %class.Statement.base, %"class.std::__1::basic_string", %"class.std::__1::shared_ptr.63", %"class.std::__1::vector.64", %"class.std::__1::shared_ptr.26", %"class.std::__1::vector.71", %"class.std::__1::vector.19", %"class.std::__1::vector" }
%"class.std::__1::shared_ptr.63" = type { %class.TypeRef*, %"class.std::__1::__shared_weak_count"* }
%class.TypeRef = type { %class.Statement.base, %struct.TypeSpecifier }
%"class.std::__1::vector.64" = type { %"class.std::__1::__vector_base.65" }
%"class.std::__1::__vector_base.65" = type { %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"*, %"class.std::__1::__compressed_pair.67" }
%"class.std::__1::shared_ptr.66" = type { %class.FuncArgDecl*, %"class.std::__1::__shared_weak_count"* }
%class.FuncArgDecl = type { %class.Expression, %"class.std::__1::basic_string", %"class.std::__1::shared_ptr.63", %"class.std::__1::shared_ptr" }
%"class.std::__1::__compressed_pair.67" = type { %"class.std::__1::__libcpp_compressed_pair_imp.68" }
%"class.std::__1::__libcpp_compressed_pair_imp.68" = type { %"class.std::__1::shared_ptr.66"* }
%"class.std::__1::vector.71" = type { %"class.std::__1::__vector_base.72" }
%"class.std::__1::__vector_base.72" = type { %"struct.std::__1::pair.73"*, %"struct.std::__1::pair.73"*, %"class.std::__1::__compressed_pair.74" }
%"struct.std::__1::pair.73" = type { %"class.std::__1::basic_string", %"class.std::__1::basic_string" }
%"class.std::__1::__compressed_pair.74" = type { %"class.std::__1::__libcpp_compressed_pair_imp.75" }
%"class.std::__1::__libcpp_compressed_pair_imp.75" = type { %"struct.std::__1::pair.73"* }
%"class.std::__1::__wrap_iter.78" = type { %"class.std::__1::shared_ptr.66"* }
%class.IdentifierRefExpr = type <{ %class.Expression, %"class.std::__1::basic_string", %struct.TypeSpecifier, i8, i8, i8, i8, [4 x i8] }>
%class.ForStmt = type { %class.Statement.base, i8, %"class.std::__1::shared_ptr.79", %"class.std::__1::shared_ptr", %"class.std::__1::shared_ptr.29", %"class.std::__1::shared_ptr.29", %"class.std::__1::shared_ptr.29", %"class.std::__1::shared_ptr.26" }
%"class.std::__1::shared_ptr.79" = type { %class.IdentifierRefExpr*, %"class.std::__1::__shared_weak_count"* }
%class.WhileStmt = type { %class.Statement.base, %"class.std::__1::shared_ptr", %"class.std::__1::shared_ptr.26" }
%class.ArrayLiteral = type { %class.Expression, %"class.std::__1::vector.80", %"class.std::__1::shared_ptr", %struct.TypeSpecifier, i32, i8, %"class.llvm::Value"* }
%"class.std::__1::vector.80" = type { %"class.std::__1::__vector_base.81" }
%"class.std::__1::__vector_base.81" = type { %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"*, %"class.std::__1::__compressed_pair.82" }
%"class.std::__1::__compressed_pair.82" = type { %"class.std::__1::__libcpp_compressed_pair_imp.83" }
%"class.std::__1::__libcpp_compressed_pair_imp.83" = type { %"class.std::__1::shared_ptr"* }
%"class.llvm::Value" = type { i32 (...)**, %"class.llvm::Type"*, %"class.llvm::Use"*, i8, i8, i16, i32 }
%"class.llvm::Type" = type { %"class.llvm::LLVMContext"*, i32, i32, %"class.llvm::Type"** }
%"class.llvm::LLVMContext" = type { %"class.llvm::LLVMContextImpl"* }
%"class.llvm::LLVMContextImpl" = type opaque
%"class.llvm::Use" = type { %"class.llvm::Value"*, %"class.llvm::Use"*, %"class.llvm::PointerIntPair" }
%"class.llvm::PointerIntPair" = type { i64 }
%class.LiteralExpr = type { %class.Expression, %struct.Variant, %struct.TypeSpecifier, %"class.std::__1::basic_string", i8, %"class.std::__1::basic_string" }
%class.StringLiteral = type { %class.Expression, i8, %"class.std::__1::basic_string" }
%class.ArrayAccessExpr = type { %class.Expression, %"class.std::__1::shared_ptr", %struct.TypeSpecifier, i8, %"class.std::__1::shared_ptr.86" }
%"class.std::__1::shared_ptr.86" = type { %class.CallExpr*, %"class.std::__1::__shared_weak_count"* }
%class.CallExpr = type <{ %class.Expression, i32, [4 x i8], %"class.std::__1::basic_string", %"class.std::__1::vector.80", %struct.TypeSpecifier, i8, [7 x i8], %"class.std::__1::basic_string", i8, i8, [6 x i8], %struct.TypeSpecifier, i8, [3 x i8], i32, %"class.std::__1::basic_string", %"struct.cdot::cl::Method"*, %struct.TypeSpecifier, i8, [7 x i8], %"class.std::__1::vector.19", i8, [7 x i8], %"class.std::__1::basic_string", i8, [7 x i8], %"class.std::__1::basic_string", i8, [7 x i8] }>
%"struct.cdot::cl::Method" = type { %"class.std::__1::basic_string", %"class.std::__1::basic_string", %struct.TypeSpecifier, %"class.std::__1::vector", %"class.std::__1::vector.19", %"class.std::__1::vector.80", i32, %struct.TypeSpecifier*, %"class.llvm::Function"* }
%"class.llvm::Function" = type { %"class.llvm::GlobalObject", %"class.llvm::ilist_node", %"class.llvm::SymbolTableList", %"class.llvm::SymbolTableList.88", %"class.std::__1::unique_ptr.96", %"class.llvm::AttributeSet" }
%"class.llvm::GlobalObject" = type { %"class.llvm::GlobalValue", %"class.llvm::Comdat"* }
%"class.llvm::GlobalValue" = type { %"class.llvm::Constant", %"class.llvm::Type"*, i32, i32, %"class.llvm::Module"* }
%"class.llvm::Constant" = type { %"class.llvm::User" }
%"class.llvm::User" = type { %"class.llvm::Value" }
%"class.llvm::Module" = type opaque
%"class.llvm::Comdat" = type opaque
%"class.llvm::ilist_node" = type { %"class.llvm::ilist_node_impl" }
%"class.llvm::ilist_node_impl" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::ilist_node_base" = type { %"class.llvm::ilist_node_base"*, %"class.llvm::ilist_node_base"* }
%"class.llvm::SymbolTableList" = type { %"class.llvm::iplist_impl" }
%"class.llvm::iplist_impl" = type { %"class.llvm::simple_ilist" }
%"class.llvm::simple_ilist" = type { %"class.llvm::ilist_sentinel" }
%"class.llvm::ilist_sentinel" = type { %"class.llvm::ilist_node_impl.87" }
%"class.llvm::ilist_node_impl.87" = type { %"class.llvm::ilist_node_base" }
%"class.llvm::SymbolTableList.88" = type { %"class.llvm::iplist_impl.89" }
%"class.llvm::iplist_impl.89" = type { %"class.llvm::simple_ilist.92" }
%"class.llvm::simple_ilist.92" = type { %"class.llvm::ilist_sentinel.94" }
%"class.llvm::ilist_sentinel.94" = type { %"class.llvm::ilist_node_impl.95" }
%"class.llvm::ilist_node_impl.95" = type { %"class.llvm::ilist_node_base" }
%"class.std::__1::unique_ptr.96" = type { %"class.std::__1::__compressed_pair.97" }
%"class.std::__1::__compressed_pair.97" = type { %"class.std::__1::__libcpp_compressed_pair_imp.98" }
%"class.std::__1::__libcpp_compressed_pair_imp.98" = type { %"class.llvm::ValueSymbolTable"* }
%"class.llvm::ValueSymbolTable" = type opaque
%"class.llvm::AttributeSet" = type { %"class.llvm::AttributeSetImpl"* }
%"class.llvm::AttributeSetImpl" = type opaque
%class.MemberRefExpr = type <{ %class.Expression, %"class.std::__1::basic_string", %"class.std::__1::basic_string", %struct.TypeSpecifier, i8, i8, [6 x i8], %struct.TypeSpecifier, i8, [7 x i8] }>
%class.BinaryOperator = type { %class.Operator, %"class.std::__1::shared_ptr", %"class.std::__1::shared_ptr", %struct.TypeSpecifier, %"class.std::__1::basic_string", i64, i64, i8, %"class.std::__1::shared_ptr.86", i8, %"class.std::__1::basic_string", %"struct.cdot::cl::Method"* }
%class.Operator = type { %class.Expression, %"class.std::__1::basic_string", %"class.std::__1::basic_string" }
%class.ExplicitCastExpr = type { %class.Operator, %struct.TypeSpecifier, %struct.TypeSpecifier, %"class.std::__1::shared_ptr" }
%class.TertiaryOperator = type { %class.Operator, %"class.std::__1::shared_ptr", %"class.std::__1::shared_ptr", %"class.std::__1::shared_ptr", %struct.TypeSpecifier }
%class.UnaryOperator = type { %class.Operator, %"class.std::__1::basic_string", %"class.std::__1::basic_string", %struct.TypeSpecifier, %struct.TypeSpecifier, %"class.std::__1::shared_ptr", i8, i8, %"class.std::__1::shared_ptr.86", i8, %"struct.cdot::cl::Method"*, %"class.std::__1::basic_string" }
%class.BreakStmt = type { %class.Statement.base, %"class.std::__1::basic_string", %class.Statement* }
%class.ContinueStmt = type { %class.Statement.base, %"class.std::__1::basic_string", %class.Statement* }
%class.IfStmt = type { %class.Statement.base, %"class.std::__1::shared_ptr", %"class.std::__1::shared_ptr.29", %"class.std::__1::shared_ptr.29", %"class.std::__1::basic_string" }
%class.SwitchStmt = type { %class.Statement.base, %"class.std::__1::shared_ptr", %"class.std::__1::vector.101", i8, i32 }
%"class.std::__1::vector.101" = type { %"class.std::__1::__vector_base.102" }
%"class.std::__1::__vector_base.102" = type { %"class.std::__1::shared_ptr.103"*, %"class.std::__1::shared_ptr.103"*, %"class.std::__1::__compressed_pair.104" }
%"class.std::__1::shared_ptr.103" = type { %class.CaseStmt*, %"class.std::__1::__shared_weak_count"* }
%class.CaseStmt = type <{ %class.LabelStmt, i8, [7 x i8], %"class.std::__1::shared_ptr", i8, [7 x i8] }>
%class.LabelStmt = type { %class.CompoundStmt.base, %"class.std::__1::basic_string", %class.CompoundStmt*, %class.EvaluatingVisitor* }
%class.CompoundStmt.base = type <{ %class.Statement.base, [6 x i8], %"class.std::__1::enable_shared_from_this", i8, i8, [6 x i8], %"class.std::__1::vector.27", %"class.std::__1::vector.34", i32 }>
%class.EvaluatingVisitor = type opaque
%"class.std::__1::__compressed_pair.104" = type { %"class.std::__1::__libcpp_compressed_pair_imp.105" }
%"class.std::__1::__libcpp_compressed_pair_imp.105" = type { %"class.std::__1::shared_ptr.103"* }
%class.GotoStmt = type { %class.Statement.base, %"class.std::__1::basic_string" }
%class.ReturnStmt = type { %class.Statement.base, %class.CompoundStmt*, %"class.std::__1::shared_ptr", %struct.TypeSpecifier }
%class.InputStmt = type { %class.Statement.base, %"class.std::__1::basic_string" }
%class.OutputStmt = type { %class.Statement.base, %"class.std::__1::shared_ptr", i8, %"class.std::__1::basic_string" }
%class.ClassDecl = type { %class.Expression, %"class.std::__1::basic_string", %"class.std::__1::vector", i8, i32, %"class.std::__1::basic_string", %"class.std::__1::vector.108", %"class.std::__1::vector.115", %"class.std::__1::vector.122", %"class.std::__1::vector.129", %"class.std::__1::vector.136", %"class.std::__1::basic_string", i8, %"class.cdot::cl::Class"* }
%"class.std::__1::vector.108" = type { %"class.std::__1::__vector_base.109" }
%"class.std::__1::__vector_base.109" = type { %"class.std::__1::shared_ptr.110"*, %"class.std::__1::shared_ptr.110"*, %"class.std::__1::__compressed_pair.111" }
%"class.std::__1::shared_ptr.110" = type opaque
%"class.std::__1::__compressed_pair.111" = type { %"class.std::__1::__libcpp_compressed_pair_imp.112" }
%"class.std::__1::__libcpp_compressed_pair_imp.112" = type { %"class.std::__1::shared_ptr.110"* }
%"class.std::__1::vector.115" = type { %"class.std::__1::__vector_base.116" }
%"class.std::__1::__vector_base.116" = type { %"class.std::__1::shared_ptr.117"*, %"class.std::__1::shared_ptr.117"*, %"class.std::__1::__compressed_pair.118" }
%"class.std::__1::shared_ptr.117" = type opaque
%"class.std::__1::__compressed_pair.118" = type { %"class.std::__1::__libcpp_compressed_pair_imp.119" }
%"class.std::__1::__libcpp_compressed_pair_imp.119" = type { %"class.std::__1::shared_ptr.117"* }
%"class.std::__1::vector.122" = type { %"class.std::__1::__vector_base.123" }
%"class.std::__1::__vector_base.123" = type { %"class.std::__1::shared_ptr.124"*, %"class.std::__1::shared_ptr.124"*, %"class.std::__1::__compressed_pair.125" }
%"class.std::__1::shared_ptr.124" = type opaque
%"class.std::__1::__compressed_pair.125" = type { %"class.std::__1::__libcpp_compressed_pair_imp.126" }
%"class.std::__1::__libcpp_compressed_pair_imp.126" = type { %"class.std::__1::shared_ptr.124"* }
%"class.std::__1::vector.129" = type { %"class.std::__1::__vector_base.130" }
%"class.std::__1::__vector_base.130" = type { %"class.std::__1::shared_ptr.131"*, %"class.std::__1::shared_ptr.131"*, %"class.std::__1::__compressed_pair.132" }
%"class.std::__1::shared_ptr.131" = type opaque
%"class.std::__1::__compressed_pair.132" = type { %"class.std::__1::__libcpp_compressed_pair_imp.133" }
%"class.std::__1::__libcpp_compressed_pair_imp.133" = type { %"class.std::__1::shared_ptr.131"* }
%"class.std::__1::vector.136" = type { %"class.std::__1::__vector_base.137" }
%"class.std::__1::__vector_base.137" = type { %"struct.std::__1::pair.138"*, %"struct.std::__1::pair.138"*, %"class.std::__1::__compressed_pair.139" }
%"struct.std::__1::pair.138" = type { %"class.std::__1::basic_string", %struct.TypeSpecifier }
%"class.std::__1::__compressed_pair.139" = type { %"class.std::__1::__libcpp_compressed_pair_imp.140" }
%"class.std::__1::__libcpp_compressed_pair_imp.140" = type { %"struct.std::__1::pair.138"* }
%"class.cdot::cl::Class" = type { i32, %"class.std::__1::basic_string", %"class.cdot::cl::Class"*, %"class.std::__1::vector.143", %class.ClassDecl*, %"class.std::__1::vector", %"class.std::__1::map", %"class.std::__1::vector", %"class.std::__1::vector.155", %"class.std::__1::unordered_multimap", %"class.std::__1::vector", %"class.std::__1::vector.183", %"class.std::__1::unordered_map", %"class.std::__1::vector.190", %"class.std::__1::vector", %"class.std::__1::vector", %"class.std::__1::unordered_map.196", %"class.std::__1::unordered_map.218", %class.Namespace*, %"class.std::__1::vector.136", i8, %"class.std::__1::vector.71" }
%"class.std::__1::vector.143" = type { %"class.std::__1::__vector_base.144" }
%"class.std::__1::__vector_base.144" = type { %"class.cdot::cl::Class"**, %"class.cdot::cl::Class"**, %"class.std::__1::__compressed_pair.145" }
%"class.std::__1::__compressed_pair.145" = type { %"class.std::__1::__libcpp_compressed_pair_imp.146" }
%"class.std::__1::__libcpp_compressed_pair_imp.146" = type { %"class.cdot::cl::Class"** }
%"class.std::__1::map" = type { %"class.std::__1::__tree" }
%"class.std::__1::__tree" = type { %"class.std::__1::__tree_end_node"*, %"class.std::__1::__compressed_pair.149", %"class.std::__1::__compressed_pair.153" }
%"class.std::__1::__tree_end_node" = type { %"class.std::__1::__tree_node_base"* }
%"class.std::__1::__tree_node_base" = type <{ %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_end_node"*, i8, [7 x i8] }>
%"class.std::__1::__compressed_pair.149" = type { %"class.std::__1::__libcpp_compressed_pair_imp.150" }
%"class.std::__1::__libcpp_compressed_pair_imp.150" = type { %"class.std::__1::__tree_end_node" }
%"class.std::__1::__compressed_pair.153" = type { %"class.std::__1::__libcpp_compressed_pair_imp.154" }
%"class.std::__1::__libcpp_compressed_pair_imp.154" = type { i64 }
%"class.std::__1::vector.155" = type { %"class.std::__1::__vector_base.156" }
%"class.std::__1::__vector_base.156" = type { %"struct.std::__1::pair.157"*, %"struct.std::__1::pair.157"*, %"class.std::__1::__compressed_pair.158" }
%"struct.std::__1::pair.157" = type { %"class.std::__1::basic_string", %"struct.cdot::cl::Field"* }
%"struct.cdot::cl::Field" = type { %"class.std::__1::basic_string", %"class.std::__1::basic_string", %struct.TypeSpecifier, i32, %"class.std::__1::shared_ptr", %"class.llvm::Type"* }
%"class.std::__1::__compressed_pair.158" = type { %"class.std::__1::__libcpp_compressed_pair_imp.159" }
%"class.std::__1::__libcpp_compressed_pair_imp.159" = type { %"struct.std::__1::pair.157"* }
%"class.std::__1::unordered_multimap" = type { %"class.std::__1::__hash_table.162" }
%"class.std::__1::__hash_table.162" = type <{ %"class.std::__1::unique_ptr.163", %"class.std::__1::__compressed_pair.172", %"class.std::__1::__compressed_pair.176", %"class.std::__1::__compressed_pair.179", [4 x i8] }>
%"class.std::__1::unique_ptr.163" = type { %"class.std::__1::__compressed_pair.164" }
%"class.std::__1::__compressed_pair.164" = type { %"class.std::__1::__libcpp_compressed_pair_imp.165" }
%"class.std::__1::__libcpp_compressed_pair_imp.165" = type { %"struct.std::__1::__hash_node_base.166"**, %"class.std::__1::__bucket_list_deallocator.167" }
%"struct.std::__1::__hash_node_base.166" = type { %"struct.std::__1::__hash_node_base.166"* }
%"class.std::__1::__bucket_list_deallocator.167" = type { %"class.std::__1::__compressed_pair.168" }
%"class.std::__1::__compressed_pair.168" = type { %"class.std::__1::__libcpp_compressed_pair_imp.169" }
%"class.std::__1::__libcpp_compressed_pair_imp.169" = type { i64 }
%"class.std::__1::__compressed_pair.172" = type { %"class.std::__1::__libcpp_compressed_pair_imp.173" }
%"class.std::__1::__libcpp_compressed_pair_imp.173" = type { %"struct.std::__1::__hash_node_base.166" }
%"class.std::__1::__compressed_pair.176" = type { %"class.std::__1::__libcpp_compressed_pair_imp.177" }
%"class.std::__1::__libcpp_compressed_pair_imp.177" = type { i64 }
%"class.std::__1::__compressed_pair.179" = type { %"class.std::__1::__libcpp_compressed_pair_imp.180" }
%"class.std::__1::__libcpp_compressed_pair_imp.180" = type { float }
%"class.std::__1::vector.183" = type { %"class.std::__1::__vector_base.184" }
%"class.std::__1::__vector_base.184" = type { %"struct.std::__1::pair.185"*, %"struct.std::__1::pair.185"*, %"class.std::__1::__compressed_pair.186" }
%"struct.std::__1::pair.185" = type opaque
%"class.std::__1::__compressed_pair.186" = type { %"class.std::__1::__libcpp_compressed_pair_imp.187" }
%"class.std::__1::__libcpp_compressed_pair_imp.187" = type { %"struct.std::__1::pair.185"* }
%"class.std::__1::vector.190" = type { %"class.std::__1::__vector_base.191" }
%"class.std::__1::__vector_base.191" = type { %"struct.cdot::cl::Method"**, %"struct.cdot::cl::Method"**, %"class.std::__1::__compressed_pair.192" }
%"class.std::__1::__compressed_pair.192" = type { %"class.std::__1::__libcpp_compressed_pair_imp.193" }
%"class.std::__1::__libcpp_compressed_pair_imp.193" = type { %"struct.cdot::cl::Method"** }
%"class.std::__1::unordered_map.196" = type { %"class.std::__1::__hash_table.197" }
%"class.std::__1::__hash_table.197" = type <{ %"class.std::__1::unique_ptr.198", %"class.std::__1::__compressed_pair.207", %"class.std::__1::__compressed_pair.211", %"class.std::__1::__compressed_pair.214", [4 x i8] }>
%"class.std::__1::unique_ptr.198" = type { %"class.std::__1::__compressed_pair.199" }
%"class.std::__1::__compressed_pair.199" = type { %"class.std::__1::__libcpp_compressed_pair_imp.200" }
%"class.std::__1::__libcpp_compressed_pair_imp.200" = type { %"struct.std::__1::__hash_node_base.201"**, %"class.std::__1::__bucket_list_deallocator.202" }
%"struct.std::__1::__hash_node_base.201" = type { %"struct.std::__1::__hash_node_base.201"* }
%"class.std::__1::__bucket_list_deallocator.202" = type { %"class.std::__1::__compressed_pair.203" }
%"class.std::__1::__compressed_pair.203" = type { %"class.std::__1::__libcpp_compressed_pair_imp.204" }
%"class.std::__1::__libcpp_compressed_pair_imp.204" = type { i64 }
%"class.std::__1::__compressed_pair.207" = type { %"class.std::__1::__libcpp_compressed_pair_imp.208" }
%"class.std::__1::__libcpp_compressed_pair_imp.208" = type { %"struct.std::__1::__hash_node_base.201" }
%"class.std::__1::__compressed_pair.211" = type { %"class.std::__1::__libcpp_compressed_pair_imp.212" }
%"class.std::__1::__libcpp_compressed_pair_imp.212" = type { i64 }
%"class.std::__1::__compressed_pair.214" = type { %"class.std::__1::__libcpp_compressed_pair_imp.215" }
%"class.std::__1::__libcpp_compressed_pair_imp.215" = type { float }
%"class.std::__1::unordered_map.218" = type { %"class.std::__1::__hash_table.219" }
%"class.std::__1::__hash_table.219" = type <{ %"class.std::__1::unique_ptr.220", %"class.std::__1::__compressed_pair.229", %"class.std::__1::__compressed_pair.233", %"class.std::__1::__compressed_pair.236", [4 x i8] }>
%"class.std::__1::unique_ptr.220" = type { %"class.std::__1::__compressed_pair.221" }
%"class.std::__1::__compressed_pair.221" = type { %"class.std::__1::__libcpp_compressed_pair_imp.222" }
%"class.std::__1::__libcpp_compressed_pair_imp.222" = type { %"struct.std::__1::__hash_node_base.223"**, %"class.std::__1::__bucket_list_deallocator.224" }
%"struct.std::__1::__hash_node_base.223" = type { %"struct.std::__1::__hash_node_base.223"* }
%"class.std::__1::__bucket_list_deallocator.224" = type { %"class.std::__1::__compressed_pair.225" }
%"class.std::__1::__compressed_pair.225" = type { %"class.std::__1::__libcpp_compressed_pair_imp.226" }
%"class.std::__1::__libcpp_compressed_pair_imp.226" = type { i64 }
%"class.std::__1::__compressed_pair.229" = type { %"class.std::__1::__libcpp_compressed_pair_imp.230" }
%"class.std::__1::__libcpp_compressed_pair_imp.230" = type { %"struct.std::__1::__hash_node_base.223" }
%"class.std::__1::__compressed_pair.233" = type { %"class.std::__1::__libcpp_compressed_pair_imp.234" }
%"class.std::__1::__libcpp_compressed_pair_imp.234" = type { i64 }
%"class.std::__1::__compressed_pair.236" = type { %"class.std::__1::__libcpp_compressed_pair_imp.237" }
%"class.std::__1::__libcpp_compressed_pair_imp.237" = type { float }
%class.Namespace = type { %"class.std::__1::unordered_map.240", %"class.std::__1::basic_string", %class.Namespace*, %"class.std::__1::unordered_map.262", %"class.std::__1::unordered_map.284", %"class.std::__1::unordered_map.306", %"class.std::__1::unordered_map", %"class.std::__1::unordered_map" }
%"class.std::__1::unordered_map.240" = type { %"class.std::__1::__hash_table.241" }
%"class.std::__1::__hash_table.241" = type <{ %"class.std::__1::unique_ptr.242", %"class.std::__1::__compressed_pair.251", %"class.std::__1::__compressed_pair.255", %"class.std::__1::__compressed_pair.258", [4 x i8] }>
%"class.std::__1::unique_ptr.242" = type { %"class.std::__1::__compressed_pair.243" }
%"class.std::__1::__compressed_pair.243" = type { %"class.std::__1::__libcpp_compressed_pair_imp.244" }
%"class.std::__1::__libcpp_compressed_pair_imp.244" = type { %"struct.std::__1::__hash_node_base.245"**, %"class.std::__1::__bucket_list_deallocator.246" }
%"struct.std::__1::__hash_node_base.245" = type { %"struct.std::__1::__hash_node_base.245"* }
%"class.std::__1::__bucket_list_deallocator.246" = type { %"class.std::__1::__compressed_pair.247" }
%"class.std::__1::__compressed_pair.247" = type { %"class.std::__1::__libcpp_compressed_pair_imp.248" }
%"class.std::__1::__libcpp_compressed_pair_imp.248" = type { i64 }
%"class.std::__1::__compressed_pair.251" = type { %"class.std::__1::__libcpp_compressed_pair_imp.252" }
%"class.std::__1::__libcpp_compressed_pair_imp.252" = type { %"struct.std::__1::__hash_node_base.245" }
%"class.std::__1::__compressed_pair.255" = type { %"class.std::__1::__libcpp_compressed_pair_imp.256" }
%"class.std::__1::__libcpp_compressed_pair_imp.256" = type { i64 }
%"class.std::__1::__compressed_pair.258" = type { %"class.std::__1::__libcpp_compressed_pair_imp.259" }
%"class.std::__1::__libcpp_compressed_pair_imp.259" = type { float }
%"class.std::__1::unordered_map.262" = type { %"class.std::__1::__hash_table.263" }
%"class.std::__1::__hash_table.263" = type <{ %"class.std::__1::unique_ptr.264", %"class.std::__1::__compressed_pair.273", %"class.std::__1::__compressed_pair.277", %"class.std::__1::__compressed_pair.280", [4 x i8] }>
%"class.std::__1::unique_ptr.264" = type { %"class.std::__1::__compressed_pair.265" }
%"class.std::__1::__compressed_pair.265" = type { %"class.std::__1::__libcpp_compressed_pair_imp.266" }
%"class.std::__1::__libcpp_compressed_pair_imp.266" = type { %"struct.std::__1::__hash_node_base.267"**, %"class.std::__1::__bucket_list_deallocator.268" }
%"struct.std::__1::__hash_node_base.267" = type { %"struct.std::__1::__hash_node_base.267"* }
%"class.std::__1::__bucket_list_deallocator.268" = type { %"class.std::__1::__compressed_pair.269" }
%"class.std::__1::__compressed_pair.269" = type { %"class.std::__1::__libcpp_compressed_pair_imp.270" }
%"class.std::__1::__libcpp_compressed_pair_imp.270" = type { i64 }
%"class.std::__1::__compressed_pair.273" = type { %"class.std::__1::__libcpp_compressed_pair_imp.274" }
%"class.std::__1::__libcpp_compressed_pair_imp.274" = type { %"struct.std::__1::__hash_node_base.267" }
%"class.std::__1::__compressed_pair.277" = type { %"class.std::__1::__libcpp_compressed_pair_imp.278" }
%"class.std::__1::__libcpp_compressed_pair_imp.278" = type { i64 }
%"class.std::__1::__compressed_pair.280" = type { %"class.std::__1::__libcpp_compressed_pair_imp.281" }
%"class.std::__1::__libcpp_compressed_pair_imp.281" = type { float }
%"class.std::__1::unordered_map.284" = type { %"class.std::__1::__hash_table.285" }
%"class.std::__1::__hash_table.285" = type <{ %"class.std::__1::unique_ptr.286", %"class.std::__1::__compressed_pair.295", %"class.std::__1::__compressed_pair.299", %"class.std::__1::__compressed_pair.302", [4 x i8] }>
%"class.std::__1::unique_ptr.286" = type { %"class.std::__1::__compressed_pair.287" }
%"class.std::__1::__compressed_pair.287" = type { %"class.std::__1::__libcpp_compressed_pair_imp.288" }
%"class.std::__1::__libcpp_compressed_pair_imp.288" = type { %"struct.std::__1::__hash_node_base.289"**, %"class.std::__1::__bucket_list_deallocator.290" }
%"struct.std::__1::__hash_node_base.289" = type { %"struct.std::__1::__hash_node_base.289"* }
%"class.std::__1::__bucket_list_deallocator.290" = type { %"class.std::__1::__compressed_pair.291" }
%"class.std::__1::__compressed_pair.291" = type { %"class.std::__1::__libcpp_compressed_pair_imp.292" }
%"class.std::__1::__libcpp_compressed_pair_imp.292" = type { i64 }
%"class.std::__1::__compressed_pair.295" = type { %"class.std::__1::__libcpp_compressed_pair_imp.296" }
%"class.std::__1::__libcpp_compressed_pair_imp.296" = type { %"struct.std::__1::__hash_node_base.289" }
%"class.std::__1::__compressed_pair.299" = type { %"class.std::__1::__libcpp_compressed_pair_imp.300" }
%"class.std::__1::__libcpp_compressed_pair_imp.300" = type { i64 }
%"class.std::__1::__compressed_pair.302" = type { %"class.std::__1::__libcpp_compressed_pair_imp.303" }
%"class.std::__1::__libcpp_compressed_pair_imp.303" = type { float }
%"class.std::__1::unordered_map.306" = type { %"class.std::__1::__hash_table.307" }
%"class.std::__1::__hash_table.307" = type <{ %"class.std::__1::unique_ptr.308", %"class.std::__1::__compressed_pair.317", %"class.std::__1::__compressed_pair.321", %"class.std::__1::__compressed_pair.324", [4 x i8] }>
%"class.std::__1::unique_ptr.308" = type { %"class.std::__1::__compressed_pair.309" }
%"class.std::__1::__compressed_pair.309" = type { %"class.std::__1::__libcpp_compressed_pair_imp.310" }
%"class.std::__1::__libcpp_compressed_pair_imp.310" = type { %"struct.std::__1::__hash_node_base.311"**, %"class.std::__1::__bucket_list_deallocator.312" }
%"struct.std::__1::__hash_node_base.311" = type { %"struct.std::__1::__hash_node_base.311"* }
%"class.std::__1::__bucket_list_deallocator.312" = type { %"class.std::__1::__compressed_pair.313" }
%"class.std::__1::__compressed_pair.313" = type { %"class.std::__1::__libcpp_compressed_pair_imp.314" }
%"class.std::__1::__libcpp_compressed_pair_imp.314" = type { i64 }
%"class.std::__1::__compressed_pair.317" = type { %"class.std::__1::__libcpp_compressed_pair_imp.318" }
%"class.std::__1::__libcpp_compressed_pair_imp.318" = type { %"struct.std::__1::__hash_node_base.311" }
%"class.std::__1::__compressed_pair.321" = type { %"class.std::__1::__libcpp_compressed_pair_imp.322" }
%"class.std::__1::__libcpp_compressed_pair_imp.322" = type { i64 }
%"class.std::__1::__compressed_pair.324" = type { %"class.std::__1::__libcpp_compressed_pair_imp.325" }
%"class.std::__1::__libcpp_compressed_pair_imp.325" = type { float }
%class.ConstrDecl = type { %class.Statement.base, i8, i32, %"class.std::__1::vector.64", %"class.std::__1::shared_ptr.26", i8, %"class.std::__1::basic_string", %"class.std::__1::basic_string", %"struct.cdot::cl::Method"* }
%class.FieldDecl = type { %class.Statement.base, i8, i8, i8, i32, %"class.std::__1::shared_ptr.63", %"class.std::__1::basic_string", %"class.std::__1::shared_ptr", %"class.std::__1::basic_string", %"class.std::__1::basic_string", %"class.std::__1::basic_string", %"struct.cdot::cl::Method"*, %"struct.cdot::cl::Method"* }
%class.MethodDecl = type { %class.Statement.base, i8, i8, i32, %"class.std::__1::basic_string", %"class.std::__1::shared_ptr.63", %"class.std::__1::vector.64", %"class.std::__1::shared_ptr.26", i8, %"class.std::__1::basic_string", %"class.std::__1::basic_string", i8, %"class.llvm::Function"*, %"struct.cdot::cl::Method"* }
%class.LambdaExpr = type { %class.Expression, %struct.TypeSpecifier, %"class.std::__1::vector.64", %"class.std::__1::shared_ptr.29", %"class.std::__1::vector.71", %"class.std::__1::vector.19", %"class.std::__1::basic_string", i32, %"class.std::__1::basic_string", i8, %"class.llvm::StructType"* }
%"class.llvm::StructType" = type { %"class.llvm::CompositeType", i8* }
%"class.llvm::CompositeType" = type { %"class.llvm::Type" }
%class.OperatorDecl = type { %class.Statement.base, i8, %struct.TypeSpecifier, i32, %"class.std::__1::basic_string", %"class.std::__1::vector.64", %"class.std::__1::shared_ptr.26" }
%class.ExportStmt = type { %class.Statement.base, %"class.std::__1::basic_string" }
%class.InterfaceDecl = type opaque
%class.ImplicitCastExpr = type { %class.Expression, %struct.TypeSpecifier, %struct.TypeSpecifier, %"class.std::__1::shared_ptr" }
%class.ExtendStmt = type opaque
%class.TypedefDecl = type { %class.Statement.base, %"class.std::__1::basic_string", %"class.std::__1::shared_ptr.63" }
%"class.std::__1::allocator.23" = type { i8 }
%"struct.std::__1::integral_constant" = type { i8 }
%"struct.std::__1::__has_destroy" = type { i8 }
%"class.std::__1::allocator.3" = type { i8 }
%"struct.std::__1::__has_destroy.328" = type { i8 }
%"class.std::__1::allocator.9" = type { i8 }
%"class.std::__1::allocator.13" = type { i8 }
%"struct.std::__1::pair.329" = type { %"class.std::__1::basic_string", %struct.TypeSpecifier }
%"struct.std::__1::integral_constant.330" = type { i8 }
%"struct.std::__1::__has_destroy.331" = type { i8 }
%"struct.std::__1::__hash_node" = type { %"struct.std::__1::__hash_node_base", i64, %"union.std::__1::__hash_value_type" }
%"union.std::__1::__hash_value_type" = type { %"struct.std::__1::pair.329" }
%"class.std::__1::allocator.61" = type { i8 }
%"struct.std::__1::__has_destroy.332" = type { i8 }

@_ZN4llvm24DisableABIBreakingChecksE = external global i32, align 4
@_ZN4llvm30VerifyDisableABIBreakingChecksE = weak hidden global i32* @_ZN4llvm24DisableABIBreakingChecksE, align 8
@_ZTV16ConstExprVisitor = unnamed_addr constant [44 x i8*] [i8* null, i8* bitcast ({ i8*, i8*, i8* }* @_ZTI16ConstExprVisitor to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.NamespaceDecl*)* @_ZN16ConstExprVisitor5visitEP13NamespaceDecl to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.FunctionDecl*)* @_ZN16ConstExprVisitor5visitEP12FunctionDecl to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.CompoundStmt*)* @_ZN16ConstExprVisitor5visitEP12CompoundStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.IdentifierRefExpr*)* @_ZN16ConstExprVisitor5visitEP17IdentifierRefExpr to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.DeclStmt*)* @_ZN16ConstExprVisitor5visitEP8DeclStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.ForStmt*)* @_ZN16ConstExprVisitor5visitEP7ForStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.WhileStmt*)* @_ZN16ConstExprVisitor5visitEP9WhileStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.ImportStmt*)* @_ZN16ConstExprVisitor5visitEP10ImportStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.ExportStmt*)* @_ZN16ConstExprVisitor5visitEP10ExportStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.ArrayLiteral*)* @_ZN16ConstExprVisitor5visitEP12ArrayLiteral to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.LiteralExpr*)* @_ZN16ConstExprVisitor5visitEP11LiteralExpr to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.StringLiteral*)* @_ZN16ConstExprVisitor5visitEP13StringLiteral to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.ArrayAccessExpr*)* @_ZN16ConstExprVisitor5visitEP15ArrayAccessExpr to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.CallExpr*)* @_ZN16ConstExprVisitor5visitEP8CallExpr to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.MemberRefExpr*)* @_ZN16ConstExprVisitor5visitEP13MemberRefExpr to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.BinaryOperator*)* @_ZN16ConstExprVisitor5visitEP14BinaryOperator to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.ExplicitCastExpr*)* @_ZN16ConstExprVisitor5visitEP16ExplicitCastExpr to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.TertiaryOperator*)* @_ZN16ConstExprVisitor5visitEP16TertiaryOperator to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.UnaryOperator*)* @_ZN16ConstExprVisitor5visitEP13UnaryOperator to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.BreakStmt*)* @_ZN16ConstExprVisitor5visitEP9BreakStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.ContinueStmt*)* @_ZN16ConstExprVisitor5visitEP12ContinueStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.IfStmt*)* @_ZN16ConstExprVisitor5visitEP6IfStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.SwitchStmt*)* @_ZN16ConstExprVisitor5visitEP10SwitchStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.CaseStmt*)* @_ZN16ConstExprVisitor5visitEP8CaseStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.LabelStmt*)* @_ZN16ConstExprVisitor5visitEP9LabelStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.GotoStmt*)* @_ZN16ConstExprVisitor5visitEP8GotoStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.FuncArgDecl*)* @_ZN16ConstExprVisitor5visitEP11FuncArgDecl to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.ReturnStmt*)* @_ZN16ConstExprVisitor5visitEP10ReturnStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.InputStmt*)* @_ZN16ConstExprVisitor5visitEP9InputStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.OutputStmt*)* @_ZN16ConstExprVisitor5visitEP10OutputStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.Expression*)* @_ZN16ConstExprVisitor5visitEP10Expression to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.ClassDecl*)* @_ZN16ConstExprVisitor5visitEP9ClassDecl to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.MethodDecl*)* @_ZN16ConstExprVisitor5visitEP10MethodDecl to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.FieldDecl*)* @_ZN16ConstExprVisitor5visitEP9FieldDecl to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.ConstrDecl*)* @_ZN16ConstExprVisitor5visitEP10ConstrDecl to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.LambdaExpr*)* @_ZN16ConstExprVisitor5visitEP10LambdaExpr to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.OperatorDecl*)* @_ZN16ConstExprVisitor5visitEP12OperatorDecl to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.InterfaceDecl*)* @_ZN16ConstExprVisitor5visitEP13InterfaceDecl to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.ImplicitCastExpr*)* @_ZN16ConstExprVisitor5visitEP16ImplicitCastExpr to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.ExtendStmt*)* @_ZN16ConstExprVisitor5visitEP10ExtendStmt to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.TypedefDecl*)* @_ZN16ConstExprVisitor5visitEP11TypedefDecl to i8*), i8* bitcast (void (%struct.Variant*, %class.ConstExprVisitor*, %class.TypeRef*)* @_ZN16ConstExprVisitor5visitEP7TypeRef to i8*)], align 8
@_ZTVN10__cxxabiv120__si_class_type_infoE = external global i8*
@_ZTS16ConstExprVisitor = constant [19 x i8] c"16ConstExprVisitor\00"
@_ZTVN10__cxxabiv117__class_type_infoE = external global i8*
@_ZTS7Visitor = linkonce_odr constant [9 x i8] c"7Visitor\00"
@_ZTI7Visitor = linkonce_odr constant { i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv117__class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @_ZTS7Visitor, i32 0, i32 0) }
@_ZTI16ConstExprVisitor = constant { i8*, i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv120__si_class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @_ZTS16ConstExprVisitor, i32 0, i32 0), i8* bitcast ({ i8*, i8* }* @_ZTI7Visitor to i8*) }

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP13NamespaceDecl(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.NamespaceDecl*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.NamespaceDecl*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.NamespaceDecl* %2, %class.NamespaceDecl** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.NamespaceDecl*, %class.NamespaceDecl** %36, align 8
  %47 = bitcast %class.NamespaceDecl* %46 to void (%"class.std::__1::vector.56"*, %class.NamespaceDecl*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.NamespaceDecl*)**, void (%"class.std::__1::vector.56"*, %class.NamespaceDecl*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.NamespaceDecl*)*, void (%"class.std::__1::vector.56"*, %class.NamespaceDecl*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.NamespaceDecl*)*, void (%"class.std::__1::vector.56"*, %class.NamespaceDecl*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.NamespaceDecl* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

declare i32 @__gxx_personality_v0(...)

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN7VariantD1Ev(%struct.Variant*) unnamed_addr #1 align 2 {
  %2 = alloca %struct.Variant*, align 8
  store %struct.Variant* %0, %struct.Variant** %2, align 8
  %3 = load %struct.Variant*, %struct.Variant** %2, align 8
  call void @_ZN7VariantD2Ev(%struct.Variant* %3) #7
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  store %"class.std::__1::shared_ptr.58"* %0, %"class.std::__1::shared_ptr.58"** %2, align 8
  %3 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %2, align 8
  call void @_ZNSt3__110shared_ptrI7AstNodeED2Ev(%"class.std::__1::shared_ptr.58"* %3) #7
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::vector.56"*, align 8
  store %"class.std::__1::vector.56"* %0, %"class.std::__1::vector.56"** %2, align 8
  %3 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %2, align 8
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED2Ev(%"class.std::__1::vector.56"* %3) #7
  ret void
}

declare void @_ZN7VariantC1Ev(%struct.Variant*) unnamed_addr #3

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP12CompoundStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.CompoundStmt*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.CompoundStmt*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.CompoundStmt* %2, %class.CompoundStmt** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.CompoundStmt*, %class.CompoundStmt** %36, align 8
  %47 = bitcast %class.CompoundStmt* %46 to void (%"class.std::__1::vector.56"*, %class.CompoundStmt*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.CompoundStmt*)**, void (%"class.std::__1::vector.56"*, %class.CompoundStmt*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.CompoundStmt*)*, void (%"class.std::__1::vector.56"*, %class.CompoundStmt*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.CompoundStmt*)*, void (%"class.std::__1::vector.56"*, %class.CompoundStmt*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.CompoundStmt* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP12FunctionDecl(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.FunctionDecl*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.66"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.66"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.66"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.66"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.66"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter.78"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter.78"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter.78"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter.78"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter.78"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter.78"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter.78"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter.78"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter.78"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.66"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter.78"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.66"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter.78", align 8
  %22 = alloca %"class.std::__1::vector.64"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.66"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter.78", align 8
  %25 = alloca %"class.std::__1::vector.64"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter.78"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.66"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter.78"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.66"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter.78", align 8
  %31 = alloca %"class.std::__1::vector.64"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.66"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter.78", align 8
  %34 = alloca %"class.std::__1::vector.64"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.FunctionDecl*, align 8
  %37 = alloca %"class.std::__1::vector.64"*, align 8
  %38 = alloca %"class.std::__1::__wrap_iter.78", align 8
  %39 = alloca %"class.std::__1::__wrap_iter.78", align 8
  %40 = alloca %"class.std::__1::shared_ptr.66", align 8
  %41 = alloca %struct.Variant, align 8
  %42 = alloca i8*
  %43 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.FunctionDecl* %2, %class.FunctionDecl** %36, align 8
  %44 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %45 = load %class.FunctionDecl*, %class.FunctionDecl** %36, align 8
  %46 = getelementptr inbounds %class.FunctionDecl, %class.FunctionDecl* %45, i32 0, i32 3
  store %"class.std::__1::vector.64"* %46, %"class.std::__1::vector.64"** %37, align 8
  %47 = load %"class.std::__1::vector.64"*, %"class.std::__1::vector.64"** %37, align 8
  store %"class.std::__1::vector.64"* %47, %"class.std::__1::vector.64"** %34, align 8
  %48 = load %"class.std::__1::vector.64"*, %"class.std::__1::vector.64"** %34, align 8
  %49 = bitcast %"class.std::__1::vector.64"* %48 to %"class.std::__1::__vector_base.65"*
  %50 = getelementptr inbounds %"class.std::__1::__vector_base.65", %"class.std::__1::__vector_base.65"* %49, i32 0, i32 0
  %51 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %50, align 8
  store %"class.std::__1::vector.64"* %48, %"class.std::__1::vector.64"** %31, align 8
  store %"class.std::__1::shared_ptr.66"* %51, %"class.std::__1::shared_ptr.66"** %32, align 8
  %52 = load %"class.std::__1::vector.64"*, %"class.std::__1::vector.64"** %31, align 8
  %53 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %32, align 8
  store %"class.std::__1::__wrap_iter.78"* %30, %"class.std::__1::__wrap_iter.78"** %28, align 8
  store %"class.std::__1::shared_ptr.66"* %53, %"class.std::__1::shared_ptr.66"** %29, align 8
  %54 = load %"class.std::__1::__wrap_iter.78"*, %"class.std::__1::__wrap_iter.78"** %28, align 8
  %55 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %29, align 8
  store %"class.std::__1::__wrap_iter.78"* %54, %"class.std::__1::__wrap_iter.78"** %26, align 8
  store %"class.std::__1::shared_ptr.66"* %55, %"class.std::__1::shared_ptr.66"** %27, align 8
  %56 = load %"class.std::__1::__wrap_iter.78"*, %"class.std::__1::__wrap_iter.78"** %26, align 8
  %57 = getelementptr inbounds %"class.std::__1::__wrap_iter.78", %"class.std::__1::__wrap_iter.78"* %56, i32 0, i32 0
  %58 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %27, align 8
  store %"class.std::__1::shared_ptr.66"* %58, %"class.std::__1::shared_ptr.66"** %57, align 8
  %59 = getelementptr inbounds %"class.std::__1::__wrap_iter.78", %"class.std::__1::__wrap_iter.78"* %30, i32 0, i32 0
  %60 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %59, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter.78", %"class.std::__1::__wrap_iter.78"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.66"* %60, %"class.std::__1::shared_ptr.66"** %61, align 8
  %62 = getelementptr inbounds %"class.std::__1::__wrap_iter.78", %"class.std::__1::__wrap_iter.78"* %33, i32 0, i32 0
  %63 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %62, align 8
  %64 = getelementptr inbounds %"class.std::__1::__wrap_iter.78", %"class.std::__1::__wrap_iter.78"* %38, i32 0, i32 0
  store %"class.std::__1::shared_ptr.66"* %63, %"class.std::__1::shared_ptr.66"** %64, align 8
  %65 = load %"class.std::__1::vector.64"*, %"class.std::__1::vector.64"** %37, align 8
  store %"class.std::__1::vector.64"* %65, %"class.std::__1::vector.64"** %25, align 8
  %66 = load %"class.std::__1::vector.64"*, %"class.std::__1::vector.64"** %25, align 8
  %67 = bitcast %"class.std::__1::vector.64"* %66 to %"class.std::__1::__vector_base.65"*
  %68 = getelementptr inbounds %"class.std::__1::__vector_base.65", %"class.std::__1::__vector_base.65"* %67, i32 0, i32 1
  %69 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %68, align 8
  store %"class.std::__1::vector.64"* %66, %"class.std::__1::vector.64"** %22, align 8
  store %"class.std::__1::shared_ptr.66"* %69, %"class.std::__1::shared_ptr.66"** %23, align 8
  %70 = load %"class.std::__1::vector.64"*, %"class.std::__1::vector.64"** %22, align 8
  %71 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %23, align 8
  store %"class.std::__1::__wrap_iter.78"* %21, %"class.std::__1::__wrap_iter.78"** %19, align 8
  store %"class.std::__1::shared_ptr.66"* %71, %"class.std::__1::shared_ptr.66"** %20, align 8
  %72 = load %"class.std::__1::__wrap_iter.78"*, %"class.std::__1::__wrap_iter.78"** %19, align 8
  %73 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %20, align 8
  store %"class.std::__1::__wrap_iter.78"* %72, %"class.std::__1::__wrap_iter.78"** %17, align 8
  store %"class.std::__1::shared_ptr.66"* %73, %"class.std::__1::shared_ptr.66"** %18, align 8
  %74 = load %"class.std::__1::__wrap_iter.78"*, %"class.std::__1::__wrap_iter.78"** %17, align 8
  %75 = getelementptr inbounds %"class.std::__1::__wrap_iter.78", %"class.std::__1::__wrap_iter.78"* %74, i32 0, i32 0
  %76 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %18, align 8
  store %"class.std::__1::shared_ptr.66"* %76, %"class.std::__1::shared_ptr.66"** %75, align 8
  %77 = getelementptr inbounds %"class.std::__1::__wrap_iter.78", %"class.std::__1::__wrap_iter.78"* %21, i32 0, i32 0
  %78 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %77, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter.78", %"class.std::__1::__wrap_iter.78"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.66"* %78, %"class.std::__1::shared_ptr.66"** %79, align 8
  %80 = getelementptr inbounds %"class.std::__1::__wrap_iter.78", %"class.std::__1::__wrap_iter.78"* %24, i32 0, i32 0
  %81 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %80, align 8
  %82 = getelementptr inbounds %"class.std::__1::__wrap_iter.78", %"class.std::__1::__wrap_iter.78"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.66"* %81, %"class.std::__1::shared_ptr.66"** %82, align 8
  br label %83

; <label>:83:                                     ; preds = %127, %3
  store %"class.std::__1::__wrap_iter.78"* %38, %"class.std::__1::__wrap_iter.78"** %14, align 8
  store %"class.std::__1::__wrap_iter.78"* %39, %"class.std::__1::__wrap_iter.78"** %15, align 8
  %84 = load %"class.std::__1::__wrap_iter.78"*, %"class.std::__1::__wrap_iter.78"** %14, align 8
  %85 = load %"class.std::__1::__wrap_iter.78"*, %"class.std::__1::__wrap_iter.78"** %15, align 8
  store %"class.std::__1::__wrap_iter.78"* %84, %"class.std::__1::__wrap_iter.78"** %12, align 8
  store %"class.std::__1::__wrap_iter.78"* %85, %"class.std::__1::__wrap_iter.78"** %13, align 8
  %86 = load %"class.std::__1::__wrap_iter.78"*, %"class.std::__1::__wrap_iter.78"** %12, align 8
  store %"class.std::__1::__wrap_iter.78"* %86, %"class.std::__1::__wrap_iter.78"** %11, align 8
  %87 = load %"class.std::__1::__wrap_iter.78"*, %"class.std::__1::__wrap_iter.78"** %11, align 8
  %88 = getelementptr inbounds %"class.std::__1::__wrap_iter.78", %"class.std::__1::__wrap_iter.78"* %87, i32 0, i32 0
  %89 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %88, align 8
  %90 = load %"class.std::__1::__wrap_iter.78"*, %"class.std::__1::__wrap_iter.78"** %13, align 8
  store %"class.std::__1::__wrap_iter.78"* %90, %"class.std::__1::__wrap_iter.78"** %10, align 8
  %91 = load %"class.std::__1::__wrap_iter.78"*, %"class.std::__1::__wrap_iter.78"** %10, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter.78", %"class.std::__1::__wrap_iter.78"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %92, align 8
  %94 = icmp eq %"class.std::__1::shared_ptr.66"* %89, %93
  %95 = xor i1 %94, true
  br i1 %95, label %96, label %136

; <label>:96:                                     ; preds = %83
  store %"class.std::__1::__wrap_iter.78"* %38, %"class.std::__1::__wrap_iter.78"** %9, align 8
  %97 = load %"class.std::__1::__wrap_iter.78"*, %"class.std::__1::__wrap_iter.78"** %9, align 8
  %98 = getelementptr inbounds %"class.std::__1::__wrap_iter.78", %"class.std::__1::__wrap_iter.78"* %97, i32 0, i32 0
  %99 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %98, align 8
  store %"class.std::__1::shared_ptr.66"* %40, %"class.std::__1::shared_ptr.66"** %7, align 8
  store %"class.std::__1::shared_ptr.66"* %99, %"class.std::__1::shared_ptr.66"** %8, align 8
  %100 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %7, align 8
  %101 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %8, align 8
  store %"class.std::__1::shared_ptr.66"* %100, %"class.std::__1::shared_ptr.66"** %5, align 8
  store %"class.std::__1::shared_ptr.66"* %101, %"class.std::__1::shared_ptr.66"** %6, align 8
  %102 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %5, align 8
  %103 = getelementptr inbounds %"class.std::__1::shared_ptr.66", %"class.std::__1::shared_ptr.66"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %6, align 8
  %105 = getelementptr inbounds %"class.std::__1::shared_ptr.66", %"class.std::__1::shared_ptr.66"* %104, i32 0, i32 0
  %106 = load %class.FuncArgDecl*, %class.FuncArgDecl** %105, align 8
  store %class.FuncArgDecl* %106, %class.FuncArgDecl** %103, align 8
  %107 = getelementptr inbounds %"class.std::__1::shared_ptr.66", %"class.std::__1::shared_ptr.66"* %102, i32 0, i32 1
  %108 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %6, align 8
  %109 = getelementptr inbounds %"class.std::__1::shared_ptr.66", %"class.std::__1::shared_ptr.66"* %108, i32 0, i32 1
  %110 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %109, align 8
  store %"class.std::__1::__shared_weak_count"* %110, %"class.std::__1::__shared_weak_count"** %107, align 8
  %111 = getelementptr inbounds %"class.std::__1::shared_ptr.66", %"class.std::__1::shared_ptr.66"* %102, i32 0, i32 1
  %112 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %111, align 8
  %113 = icmp ne %"class.std::__1::__shared_weak_count"* %112, null
  br i1 %113, label %114, label %117

; <label>:114:                                    ; preds = %96
  %115 = getelementptr inbounds %"class.std::__1::shared_ptr.66", %"class.std::__1::shared_ptr.66"* %102, i32 0, i32 1
  %116 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %115, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %116) #7
  br label %117

; <label>:117:                                    ; preds = %96, %114
  store %"class.std::__1::shared_ptr.66"* %40, %"class.std::__1::shared_ptr.66"** %4, align 8
  %118 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %4, align 8
  %119 = getelementptr inbounds %"class.std::__1::shared_ptr.66", %"class.std::__1::shared_ptr.66"* %118, i32 0, i32 0
  %120 = load %class.FuncArgDecl*, %class.FuncArgDecl** %119, align 8
  %121 = bitcast %class.FuncArgDecl* %120 to void (%struct.Variant*, %class.FuncArgDecl*, %class.Visitor*)***
  %122 = load void (%struct.Variant*, %class.FuncArgDecl*, %class.Visitor*)**, void (%struct.Variant*, %class.FuncArgDecl*, %class.Visitor*)*** %121, align 8
  %123 = getelementptr inbounds void (%struct.Variant*, %class.FuncArgDecl*, %class.Visitor*)*, void (%struct.Variant*, %class.FuncArgDecl*, %class.Visitor*)** %122, i64 16
  %124 = load void (%struct.Variant*, %class.FuncArgDecl*, %class.Visitor*)*, void (%struct.Variant*, %class.FuncArgDecl*, %class.Visitor*)** %123, align 8
  %125 = bitcast %class.ConstExprVisitor* %44 to %class.Visitor*
  invoke void %124(%struct.Variant* sret %41, %class.FuncArgDecl* %120, %class.Visitor* dereferenceable(8) %125)
          to label %126 unwind label %132

; <label>:126:                                    ; preds = %117
  call void @_ZN7VariantD1Ev(%struct.Variant* %41) #7
  call void @_ZNSt3__110shared_ptrI11FuncArgDeclED1Ev(%"class.std::__1::shared_ptr.66"* %40) #7
  br label %127

; <label>:127:                                    ; preds = %126
  store %"class.std::__1::__wrap_iter.78"* %38, %"class.std::__1::__wrap_iter.78"** %16, align 8
  %128 = load %"class.std::__1::__wrap_iter.78"*, %"class.std::__1::__wrap_iter.78"** %16, align 8
  %129 = getelementptr inbounds %"class.std::__1::__wrap_iter.78", %"class.std::__1::__wrap_iter.78"* %128, i32 0, i32 0
  %130 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %129, align 8
  %131 = getelementptr inbounds %"class.std::__1::shared_ptr.66", %"class.std::__1::shared_ptr.66"* %130, i32 1
  store %"class.std::__1::shared_ptr.66"* %131, %"class.std::__1::shared_ptr.66"** %129, align 8
  br label %83

; <label>:132:                                    ; preds = %117
  %133 = landingpad { i8*, i32 }
          cleanup
  %134 = extractvalue { i8*, i32 } %133, 0
  store i8* %134, i8** %42, align 8
  %135 = extractvalue { i8*, i32 } %133, 1
  store i32 %135, i32* %43, align 4
  call void @_ZNSt3__110shared_ptrI11FuncArgDeclED1Ev(%"class.std::__1::shared_ptr.66"* %40) #7
  br label %137

; <label>:136:                                    ; preds = %83
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:137:                                    ; preds = %132
  %138 = load i8*, i8** %42, align 8
  %139 = load i32, i32* %43, align 4
  %140 = insertvalue { i8*, i32 } undef, i8* %138, 0
  %141 = insertvalue { i8*, i32 } %140, i32 %139, 1
  resume { i8*, i32 } %141
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__110shared_ptrI11FuncArgDeclED1Ev(%"class.std::__1::shared_ptr.66"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::shared_ptr.66"*, align 8
  store %"class.std::__1::shared_ptr.66"* %0, %"class.std::__1::shared_ptr.66"** %2, align 8
  %3 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %2, align 8
  call void @_ZNSt3__110shared_ptrI11FuncArgDeclED2Ev(%"class.std::__1::shared_ptr.66"* %3) #7
  ret void
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP17IdentifierRefExpr(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.IdentifierRefExpr*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.IdentifierRefExpr*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.IdentifierRefExpr* %2, %class.IdentifierRefExpr** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.IdentifierRefExpr*, %class.IdentifierRefExpr** %36, align 8
  %47 = bitcast %class.IdentifierRefExpr* %46 to void (%"class.std::__1::vector.56"*, %class.IdentifierRefExpr*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.IdentifierRefExpr*)**, void (%"class.std::__1::vector.56"*, %class.IdentifierRefExpr*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.IdentifierRefExpr*)*, void (%"class.std::__1::vector.56"*, %class.IdentifierRefExpr*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.IdentifierRefExpr*)*, void (%"class.std::__1::vector.56"*, %class.IdentifierRefExpr*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.IdentifierRefExpr* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP8DeclStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.DeclStmt*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.DeclStmt*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.DeclStmt* %2, %class.DeclStmt** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.DeclStmt*, %class.DeclStmt** %36, align 8
  %47 = bitcast %class.DeclStmt* %46 to void (%"class.std::__1::vector.56"*, %class.DeclStmt*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.DeclStmt*)**, void (%"class.std::__1::vector.56"*, %class.DeclStmt*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.DeclStmt*)*, void (%"class.std::__1::vector.56"*, %class.DeclStmt*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.DeclStmt*)*, void (%"class.std::__1::vector.56"*, %class.DeclStmt*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.DeclStmt* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP7ForStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.ForStmt*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.ForStmt*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.ForStmt* %2, %class.ForStmt** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.ForStmt*, %class.ForStmt** %36, align 8
  %47 = bitcast %class.ForStmt* %46 to void (%"class.std::__1::vector.56"*, %class.ForStmt*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.ForStmt*)**, void (%"class.std::__1::vector.56"*, %class.ForStmt*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.ForStmt*)*, void (%"class.std::__1::vector.56"*, %class.ForStmt*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.ForStmt*)*, void (%"class.std::__1::vector.56"*, %class.ForStmt*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.ForStmt* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP9WhileStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.WhileStmt*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.WhileStmt*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.WhileStmt* %2, %class.WhileStmt** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.WhileStmt*, %class.WhileStmt** %36, align 8
  %47 = bitcast %class.WhileStmt* %46 to void (%"class.std::__1::vector.56"*, %class.WhileStmt*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.WhileStmt*)**, void (%"class.std::__1::vector.56"*, %class.WhileStmt*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.WhileStmt*)*, void (%"class.std::__1::vector.56"*, %class.WhileStmt*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.WhileStmt*)*, void (%"class.std::__1::vector.56"*, %class.WhileStmt*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.WhileStmt* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP12ArrayLiteral(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.ArrayLiteral*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.ArrayLiteral*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.ArrayLiteral* %2, %class.ArrayLiteral** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.ArrayLiteral*, %class.ArrayLiteral** %36, align 8
  %47 = bitcast %class.ArrayLiteral* %46 to void (%"class.std::__1::vector.56"*, %class.ArrayLiteral*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.ArrayLiteral*)**, void (%"class.std::__1::vector.56"*, %class.ArrayLiteral*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.ArrayLiteral*)*, void (%"class.std::__1::vector.56"*, %class.ArrayLiteral*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.ArrayLiteral*)*, void (%"class.std::__1::vector.56"*, %class.ArrayLiteral*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.ArrayLiteral* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP11LiteralExpr(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.LiteralExpr*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.LiteralExpr*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.LiteralExpr* %2, %class.LiteralExpr** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.LiteralExpr*, %class.LiteralExpr** %36, align 8
  %47 = bitcast %class.LiteralExpr* %46 to void (%"class.std::__1::vector.56"*, %class.LiteralExpr*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.LiteralExpr*)**, void (%"class.std::__1::vector.56"*, %class.LiteralExpr*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.LiteralExpr*)*, void (%"class.std::__1::vector.56"*, %class.LiteralExpr*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.LiteralExpr*)*, void (%"class.std::__1::vector.56"*, %class.LiteralExpr*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.LiteralExpr* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP13StringLiteral(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.StringLiteral*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %class.ConstExprVisitor*, align 8
  %5 = alloca %class.StringLiteral*, align 8
  %6 = alloca %"class.std::__1::basic_string", align 8
  %7 = alloca i8*
  %8 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %4, align 8
  store %class.StringLiteral* %2, %class.StringLiteral** %5, align 8
  %9 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %4, align 8
  %10 = load %class.StringLiteral*, %class.StringLiteral** %5, align 8
  %11 = getelementptr inbounds %class.StringLiteral, %class.StringLiteral* %10, i32 0, i32 2
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %6, %"class.std::__1::basic_string"* dereferenceable(24) %11)
  invoke void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant* %0, %"class.std::__1::basic_string"* %6)
          to label %12 unwind label %13

; <label>:12:                                     ; preds = %3
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %6) #7
  ret void

; <label>:13:                                     ; preds = %3
  %14 = landingpad { i8*, i32 }
          cleanup
  %15 = extractvalue { i8*, i32 } %14, 0
  store i8* %15, i8** %7, align 8
  %16 = extractvalue { i8*, i32 } %14, 1
  store i32 %16, i32* %8, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %6) #7
  br label %17

; <label>:17:                                     ; preds = %13
  %18 = load i8*, i8** %7, align 8
  %19 = load i32, i32* %8, align 4
  %20 = insertvalue { i8*, i32 } undef, i8* %18, 0
  %21 = insertvalue { i8*, i32 } %20, i32 %19, 1
  resume { i8*, i32 } %21
}

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"*, %"class.std::__1::basic_string"* dereferenceable(24)) unnamed_addr #3

declare void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant*, %"class.std::__1::basic_string"*) unnamed_addr #3

; Function Attrs: nounwind
declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"*) unnamed_addr #4

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP15ArrayAccessExpr(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.ArrayAccessExpr*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.ArrayAccessExpr*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.ArrayAccessExpr* %2, %class.ArrayAccessExpr** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.ArrayAccessExpr*, %class.ArrayAccessExpr** %36, align 8
  %47 = bitcast %class.ArrayAccessExpr* %46 to void (%"class.std::__1::vector.56"*, %class.ArrayAccessExpr*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.ArrayAccessExpr*)**, void (%"class.std::__1::vector.56"*, %class.ArrayAccessExpr*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.ArrayAccessExpr*)*, void (%"class.std::__1::vector.56"*, %class.ArrayAccessExpr*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.ArrayAccessExpr*)*, void (%"class.std::__1::vector.56"*, %class.ArrayAccessExpr*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.ArrayAccessExpr* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP8CallExpr(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.CallExpr*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.CallExpr*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.CallExpr* %2, %class.CallExpr** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.CallExpr*, %class.CallExpr** %36, align 8
  %47 = bitcast %class.CallExpr* %46 to void (%"class.std::__1::vector.56"*, %class.CallExpr*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.CallExpr*)**, void (%"class.std::__1::vector.56"*, %class.CallExpr*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.CallExpr*)*, void (%"class.std::__1::vector.56"*, %class.CallExpr*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.CallExpr*)*, void (%"class.std::__1::vector.56"*, %class.CallExpr*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.CallExpr* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP13MemberRefExpr(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.MemberRefExpr*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.MemberRefExpr*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.MemberRefExpr* %2, %class.MemberRefExpr** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.MemberRefExpr*, %class.MemberRefExpr** %36, align 8
  %47 = bitcast %class.MemberRefExpr* %46 to void (%"class.std::__1::vector.56"*, %class.MemberRefExpr*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.MemberRefExpr*)**, void (%"class.std::__1::vector.56"*, %class.MemberRefExpr*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.MemberRefExpr*)*, void (%"class.std::__1::vector.56"*, %class.MemberRefExpr*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.MemberRefExpr*)*, void (%"class.std::__1::vector.56"*, %class.MemberRefExpr*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.MemberRefExpr* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP14BinaryOperator(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.BinaryOperator*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr"*, align 8
  %6 = alloca %class.ConstExprVisitor*, align 8
  %7 = alloca %class.BinaryOperator*, align 8
  %8 = alloca %struct.Variant, align 8
  %9 = alloca i8*
  %10 = alloca i32
  %11 = alloca %struct.Variant, align 8
  %12 = alloca i1, align 1
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %6, align 8
  store %class.BinaryOperator* %2, %class.BinaryOperator** %7, align 8
  %13 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %6, align 8
  %14 = load %class.BinaryOperator*, %class.BinaryOperator** %7, align 8
  %15 = getelementptr inbounds %class.BinaryOperator, %class.BinaryOperator* %14, i32 0, i32 1
  store %"class.std::__1::shared_ptr"* %15, %"class.std::__1::shared_ptr"** %5, align 8
  %16 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %5, align 8
  %17 = getelementptr inbounds %"class.std::__1::shared_ptr", %"class.std::__1::shared_ptr"* %16, i32 0, i32 0
  %18 = load %class.Expression*, %class.Expression** %17, align 8
  %19 = bitcast %class.Expression* %18 to void (%struct.Variant*, %class.Expression*, %class.Visitor*)***
  %20 = load void (%struct.Variant*, %class.Expression*, %class.Visitor*)**, void (%struct.Variant*, %class.Expression*, %class.Visitor*)*** %19, align 8
  %21 = getelementptr inbounds void (%struct.Variant*, %class.Expression*, %class.Visitor*)*, void (%struct.Variant*, %class.Expression*, %class.Visitor*)** %20, i64 16
  %22 = load void (%struct.Variant*, %class.Expression*, %class.Visitor*)*, void (%struct.Variant*, %class.Expression*, %class.Visitor*)** %21, align 8
  %23 = bitcast %class.ConstExprVisitor* %13 to %class.Visitor*
  call void %22(%struct.Variant* sret %8, %class.Expression* %18, %class.Visitor* dereferenceable(8) %23)
  store i1 false, i1* %12, align 1
  %24 = invoke dereferenceable(216) %struct.TypeSpecifier* @_ZN7Variant8get_typeEv(%struct.Variant* %8)
          to label %25 unwind label %52

; <label>:25:                                     ; preds = %3
  %26 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %24, i32 0, i32 16
  %27 = load i8, i8* %26, align 1
  %28 = trunc i8 %27 to i1
  br i1 %28, label %29, label %46

; <label>:29:                                     ; preds = %25
  %30 = load %class.BinaryOperator*, %class.BinaryOperator** %7, align 8
  %31 = getelementptr inbounds %class.BinaryOperator, %class.BinaryOperator* %30, i32 0, i32 2
  store %"class.std::__1::shared_ptr"* %31, %"class.std::__1::shared_ptr"** %4, align 8
  %32 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %4, align 8
  %33 = getelementptr inbounds %"class.std::__1::shared_ptr", %"class.std::__1::shared_ptr"* %32, i32 0, i32 0
  %34 = load %class.Expression*, %class.Expression** %33, align 8
  %35 = bitcast %class.Expression* %34 to void (%struct.Variant*, %class.Expression*, %class.Visitor*)***
  %36 = load void (%struct.Variant*, %class.Expression*, %class.Visitor*)**, void (%struct.Variant*, %class.Expression*, %class.Visitor*)*** %35, align 8
  %37 = getelementptr inbounds void (%struct.Variant*, %class.Expression*, %class.Visitor*)*, void (%struct.Variant*, %class.Expression*, %class.Visitor*)** %36, i64 16
  %38 = load void (%struct.Variant*, %class.Expression*, %class.Visitor*)*, void (%struct.Variant*, %class.Expression*, %class.Visitor*)** %37, align 8
  %39 = bitcast %class.ConstExprVisitor* %13 to %class.Visitor*
  invoke void %38(%struct.Variant* sret %11, %class.Expression* %34, %class.Visitor* dereferenceable(8) %39)
          to label %40 unwind label %52

; <label>:40:                                     ; preds = %29
  store i1 true, i1* %12, align 1
  %41 = invoke dereferenceable(216) %struct.TypeSpecifier* @_ZN7Variant8get_typeEv(%struct.Variant* %11)
          to label %42 unwind label %56

; <label>:42:                                     ; preds = %40
  %43 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %41, i32 0, i32 16
  %44 = load i8, i8* %43, align 1
  %45 = trunc i8 %44 to i1
  br label %46

; <label>:46:                                     ; preds = %42, %25
  %47 = phi i1 [ false, %25 ], [ %45, %42 ]
  %48 = load i1, i1* %12, align 1
  br i1 %48, label %49, label %50

; <label>:49:                                     ; preds = %46
  call void @_ZN7VariantD1Ev(%struct.Variant* %11) #7
  br label %50

; <label>:50:                                     ; preds = %49, %46
  call void @_ZN7VariantD1Ev(%struct.Variant* %8) #7
  br i1 %47, label %51, label %64

; <label>:51:                                     ; preds = %50
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  br label %64

; <label>:52:                                     ; preds = %29, %3
  %53 = landingpad { i8*, i32 }
          cleanup
  %54 = extractvalue { i8*, i32 } %53, 0
  store i8* %54, i8** %9, align 8
  %55 = extractvalue { i8*, i32 } %53, 1
  store i32 %55, i32* %10, align 4
  br label %63

; <label>:56:                                     ; preds = %40
  %57 = landingpad { i8*, i32 }
          cleanup
  %58 = extractvalue { i8*, i32 } %57, 0
  store i8* %58, i8** %9, align 8
  %59 = extractvalue { i8*, i32 } %57, 1
  store i32 %59, i32* %10, align 4
  %60 = load i1, i1* %12, align 1
  br i1 %60, label %61, label %62

; <label>:61:                                     ; preds = %56
  call void @_ZN7VariantD1Ev(%struct.Variant* %11) #7
  br label %62

; <label>:62:                                     ; preds = %61, %56
  br label %63

; <label>:63:                                     ; preds = %62, %52
  call void @_ZN7VariantD1Ev(%struct.Variant* %8) #7
  br label %65

; <label>:64:                                     ; preds = %51, %50
  ret void

; <label>:65:                                     ; preds = %63
  %66 = load i8*, i8** %9, align 8
  %67 = load i32, i32* %10, align 4
  %68 = insertvalue { i8*, i32 } undef, i8* %66, 0
  %69 = insertvalue { i8*, i32 } %68, i32 %67, 1
  resume { i8*, i32 } %69
}

declare dereferenceable(216) %struct.TypeSpecifier* @_ZN7Variant8get_typeEv(%struct.Variant*) #3

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP16ExplicitCastExpr(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.ExplicitCastExpr*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.ExplicitCastExpr*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.ExplicitCastExpr* %2, %class.ExplicitCastExpr** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.ExplicitCastExpr*, %class.ExplicitCastExpr** %36, align 8
  %47 = bitcast %class.ExplicitCastExpr* %46 to void (%"class.std::__1::vector.56"*, %class.ExplicitCastExpr*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.ExplicitCastExpr*)**, void (%"class.std::__1::vector.56"*, %class.ExplicitCastExpr*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.ExplicitCastExpr*)*, void (%"class.std::__1::vector.56"*, %class.ExplicitCastExpr*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.ExplicitCastExpr*)*, void (%"class.std::__1::vector.56"*, %class.ExplicitCastExpr*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.ExplicitCastExpr* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP16TertiaryOperator(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.TertiaryOperator*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.TertiaryOperator*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.TertiaryOperator* %2, %class.TertiaryOperator** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.TertiaryOperator*, %class.TertiaryOperator** %36, align 8
  %47 = bitcast %class.TertiaryOperator* %46 to void (%"class.std::__1::vector.56"*, %class.TertiaryOperator*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.TertiaryOperator*)**, void (%"class.std::__1::vector.56"*, %class.TertiaryOperator*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.TertiaryOperator*)*, void (%"class.std::__1::vector.56"*, %class.TertiaryOperator*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.TertiaryOperator*)*, void (%"class.std::__1::vector.56"*, %class.TertiaryOperator*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.TertiaryOperator* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP13UnaryOperator(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.UnaryOperator*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.UnaryOperator*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.UnaryOperator* %2, %class.UnaryOperator** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.UnaryOperator*, %class.UnaryOperator** %36, align 8
  %47 = bitcast %class.UnaryOperator* %46 to void (%"class.std::__1::vector.56"*, %class.UnaryOperator*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.UnaryOperator*)**, void (%"class.std::__1::vector.56"*, %class.UnaryOperator*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.UnaryOperator*)*, void (%"class.std::__1::vector.56"*, %class.UnaryOperator*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.UnaryOperator*)*, void (%"class.std::__1::vector.56"*, %class.UnaryOperator*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.UnaryOperator* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP9BreakStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.BreakStmt*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.BreakStmt*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.BreakStmt* %2, %class.BreakStmt** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.BreakStmt*, %class.BreakStmt** %36, align 8
  %47 = bitcast %class.BreakStmt* %46 to void (%"class.std::__1::vector.56"*, %class.BreakStmt*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.BreakStmt*)**, void (%"class.std::__1::vector.56"*, %class.BreakStmt*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.BreakStmt*)*, void (%"class.std::__1::vector.56"*, %class.BreakStmt*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.BreakStmt*)*, void (%"class.std::__1::vector.56"*, %class.BreakStmt*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.BreakStmt* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP12ContinueStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.ContinueStmt*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.ContinueStmt*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.ContinueStmt* %2, %class.ContinueStmt** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.ContinueStmt*, %class.ContinueStmt** %36, align 8
  %47 = bitcast %class.ContinueStmt* %46 to void (%"class.std::__1::vector.56"*, %class.ContinueStmt*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.ContinueStmt*)**, void (%"class.std::__1::vector.56"*, %class.ContinueStmt*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.ContinueStmt*)*, void (%"class.std::__1::vector.56"*, %class.ContinueStmt*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.ContinueStmt*)*, void (%"class.std::__1::vector.56"*, %class.ContinueStmt*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.ContinueStmt* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP6IfStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.IfStmt*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.IfStmt*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.IfStmt* %2, %class.IfStmt** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.IfStmt*, %class.IfStmt** %36, align 8
  %47 = bitcast %class.IfStmt* %46 to void (%"class.std::__1::vector.56"*, %class.IfStmt*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.IfStmt*)**, void (%"class.std::__1::vector.56"*, %class.IfStmt*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.IfStmt*)*, void (%"class.std::__1::vector.56"*, %class.IfStmt*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.IfStmt*)*, void (%"class.std::__1::vector.56"*, %class.IfStmt*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.IfStmt* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP10SwitchStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.SwitchStmt*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.SwitchStmt*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.SwitchStmt* %2, %class.SwitchStmt** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.SwitchStmt*, %class.SwitchStmt** %36, align 8
  %47 = bitcast %class.SwitchStmt* %46 to void (%"class.std::__1::vector.56"*, %class.SwitchStmt*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.SwitchStmt*)**, void (%"class.std::__1::vector.56"*, %class.SwitchStmt*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.SwitchStmt*)*, void (%"class.std::__1::vector.56"*, %class.SwitchStmt*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.SwitchStmt*)*, void (%"class.std::__1::vector.56"*, %class.SwitchStmt*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.SwitchStmt* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP8CaseStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.CaseStmt*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.CaseStmt*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.CaseStmt* %2, %class.CaseStmt** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.CaseStmt*, %class.CaseStmt** %36, align 8
  %47 = bitcast %class.CaseStmt* %46 to void (%"class.std::__1::vector.56"*, %class.CaseStmt*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.CaseStmt*)**, void (%"class.std::__1::vector.56"*, %class.CaseStmt*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.CaseStmt*)*, void (%"class.std::__1::vector.56"*, %class.CaseStmt*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.CaseStmt*)*, void (%"class.std::__1::vector.56"*, %class.CaseStmt*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.CaseStmt* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP9LabelStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.LabelStmt*) unnamed_addr #0 align 2 {
  %4 = alloca %class.ConstExprVisitor*, align 8
  %5 = alloca %class.LabelStmt*, align 8
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %4, align 8
  store %class.LabelStmt* %2, %class.LabelStmt** %5, align 8
  %6 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %4, align 8
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP8GotoStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.GotoStmt*) unnamed_addr #0 align 2 {
  %4 = alloca %class.ConstExprVisitor*, align 8
  %5 = alloca %class.GotoStmt*, align 8
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %4, align 8
  store %class.GotoStmt* %2, %class.GotoStmt** %5, align 8
  %6 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %4, align 8
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP11FuncArgDecl(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.FuncArgDecl*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.FuncArgDecl*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.FuncArgDecl* %2, %class.FuncArgDecl** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.FuncArgDecl*, %class.FuncArgDecl** %36, align 8
  %47 = bitcast %class.FuncArgDecl* %46 to void (%"class.std::__1::vector.56"*, %class.FuncArgDecl*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.FuncArgDecl*)**, void (%"class.std::__1::vector.56"*, %class.FuncArgDecl*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.FuncArgDecl*)*, void (%"class.std::__1::vector.56"*, %class.FuncArgDecl*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.FuncArgDecl*)*, void (%"class.std::__1::vector.56"*, %class.FuncArgDecl*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.FuncArgDecl* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP10ReturnStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.ReturnStmt*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.ReturnStmt*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.ReturnStmt* %2, %class.ReturnStmt** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.ReturnStmt*, %class.ReturnStmt** %36, align 8
  %47 = bitcast %class.ReturnStmt* %46 to void (%"class.std::__1::vector.56"*, %class.ReturnStmt*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.ReturnStmt*)**, void (%"class.std::__1::vector.56"*, %class.ReturnStmt*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.ReturnStmt*)*, void (%"class.std::__1::vector.56"*, %class.ReturnStmt*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.ReturnStmt*)*, void (%"class.std::__1::vector.56"*, %class.ReturnStmt*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.ReturnStmt* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP9InputStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.InputStmt*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.InputStmt*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.InputStmt* %2, %class.InputStmt** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.InputStmt*, %class.InputStmt** %36, align 8
  %47 = bitcast %class.InputStmt* %46 to void (%"class.std::__1::vector.56"*, %class.InputStmt*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.InputStmt*)**, void (%"class.std::__1::vector.56"*, %class.InputStmt*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.InputStmt*)*, void (%"class.std::__1::vector.56"*, %class.InputStmt*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.InputStmt*)*, void (%"class.std::__1::vector.56"*, %class.InputStmt*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.InputStmt* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP10OutputStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.OutputStmt*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.OutputStmt*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.OutputStmt* %2, %class.OutputStmt** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.OutputStmt*, %class.OutputStmt** %36, align 8
  %47 = bitcast %class.OutputStmt* %46 to void (%"class.std::__1::vector.56"*, %class.OutputStmt*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.OutputStmt*)**, void (%"class.std::__1::vector.56"*, %class.OutputStmt*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.OutputStmt*)*, void (%"class.std::__1::vector.56"*, %class.OutputStmt*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.OutputStmt*)*, void (%"class.std::__1::vector.56"*, %class.OutputStmt*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.OutputStmt* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP10Expression(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.Expression*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.Expression*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.Expression* %2, %class.Expression** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.Expression*, %class.Expression** %36, align 8
  %47 = bitcast %class.Expression* %46 to void (%"class.std::__1::vector.56"*, %class.Expression*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.Expression*)**, void (%"class.std::__1::vector.56"*, %class.Expression*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.Expression*)*, void (%"class.std::__1::vector.56"*, %class.Expression*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.Expression*)*, void (%"class.std::__1::vector.56"*, %class.Expression*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.Expression* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP9ClassDecl(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.ClassDecl*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.ClassDecl*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.ClassDecl* %2, %class.ClassDecl** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.ClassDecl*, %class.ClassDecl** %36, align 8
  %47 = bitcast %class.ClassDecl* %46 to void (%"class.std::__1::vector.56"*, %class.ClassDecl*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.ClassDecl*)**, void (%"class.std::__1::vector.56"*, %class.ClassDecl*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.ClassDecl*)*, void (%"class.std::__1::vector.56"*, %class.ClassDecl*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.ClassDecl*)*, void (%"class.std::__1::vector.56"*, %class.ClassDecl*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.ClassDecl* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP10ConstrDecl(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.ConstrDecl*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.ConstrDecl*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.ConstrDecl* %2, %class.ConstrDecl** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.ConstrDecl*, %class.ConstrDecl** %36, align 8
  %47 = bitcast %class.ConstrDecl* %46 to void (%"class.std::__1::vector.56"*, %class.ConstrDecl*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.ConstrDecl*)**, void (%"class.std::__1::vector.56"*, %class.ConstrDecl*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.ConstrDecl*)*, void (%"class.std::__1::vector.56"*, %class.ConstrDecl*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.ConstrDecl*)*, void (%"class.std::__1::vector.56"*, %class.ConstrDecl*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.ConstrDecl* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP9FieldDecl(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.FieldDecl*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.FieldDecl*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.FieldDecl* %2, %class.FieldDecl** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.FieldDecl*, %class.FieldDecl** %36, align 8
  %47 = bitcast %class.FieldDecl* %46 to void (%"class.std::__1::vector.56"*, %class.FieldDecl*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.FieldDecl*)**, void (%"class.std::__1::vector.56"*, %class.FieldDecl*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.FieldDecl*)*, void (%"class.std::__1::vector.56"*, %class.FieldDecl*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.FieldDecl*)*, void (%"class.std::__1::vector.56"*, %class.FieldDecl*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.FieldDecl* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP10MethodDecl(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.MethodDecl*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.MethodDecl*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.MethodDecl* %2, %class.MethodDecl** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.MethodDecl*, %class.MethodDecl** %36, align 8
  %47 = bitcast %class.MethodDecl* %46 to void (%"class.std::__1::vector.56"*, %class.MethodDecl*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.MethodDecl*)**, void (%"class.std::__1::vector.56"*, %class.MethodDecl*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.MethodDecl*)*, void (%"class.std::__1::vector.56"*, %class.MethodDecl*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.MethodDecl*)*, void (%"class.std::__1::vector.56"*, %class.MethodDecl*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.MethodDecl* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP10LambdaExpr(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.LambdaExpr*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.LambdaExpr*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.LambdaExpr* %2, %class.LambdaExpr** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.LambdaExpr*, %class.LambdaExpr** %36, align 8
  %47 = bitcast %class.LambdaExpr* %46 to void (%"class.std::__1::vector.56"*, %class.LambdaExpr*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.LambdaExpr*)**, void (%"class.std::__1::vector.56"*, %class.LambdaExpr*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.LambdaExpr*)*, void (%"class.std::__1::vector.56"*, %class.LambdaExpr*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.LambdaExpr*)*, void (%"class.std::__1::vector.56"*, %class.LambdaExpr*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.LambdaExpr* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP12OperatorDecl(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.OperatorDecl*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.OperatorDecl*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.OperatorDecl* %2, %class.OperatorDecl** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.OperatorDecl*, %class.OperatorDecl** %36, align 8
  %47 = bitcast %class.OperatorDecl* %46 to void (%"class.std::__1::vector.56"*, %class.OperatorDecl*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.OperatorDecl*)**, void (%"class.std::__1::vector.56"*, %class.OperatorDecl*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.OperatorDecl*)*, void (%"class.std::__1::vector.56"*, %class.OperatorDecl*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.OperatorDecl*)*, void (%"class.std::__1::vector.56"*, %class.OperatorDecl*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.OperatorDecl* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP10ImportStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.ImportStmt*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.ImportStmt*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.ImportStmt* %2, %class.ImportStmt** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.ImportStmt*, %class.ImportStmt** %36, align 8
  %47 = bitcast %class.ImportStmt* %46 to void (%"class.std::__1::vector.56"*, %class.ImportStmt*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.ImportStmt*)**, void (%"class.std::__1::vector.56"*, %class.ImportStmt*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.ImportStmt*)*, void (%"class.std::__1::vector.56"*, %class.ImportStmt*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.ImportStmt*)*, void (%"class.std::__1::vector.56"*, %class.ImportStmt*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.ImportStmt* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP10ExportStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.ExportStmt*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %5 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %6 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %7 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %10 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %11 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %12 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %13 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %14 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %15 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %16 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %17 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %18 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %19 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::__wrap_iter", align 8
  %22 = alloca %"class.std::__1::vector.56"*, align 8
  %23 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %24 = alloca %"class.std::__1::__wrap_iter", align 8
  %25 = alloca %"class.std::__1::vector.56"*, align 8
  %26 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %27 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %28 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %29 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %30 = alloca %"class.std::__1::__wrap_iter", align 8
  %31 = alloca %"class.std::__1::vector.56"*, align 8
  %32 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %33 = alloca %"class.std::__1::__wrap_iter", align 8
  %34 = alloca %"class.std::__1::vector.56"*, align 8
  %35 = alloca %class.ConstExprVisitor*, align 8
  %36 = alloca %class.ExportStmt*, align 8
  %37 = alloca %"class.std::__1::vector.56"*, align 8
  %38 = alloca %"class.std::__1::vector.56", align 8
  %39 = alloca %"class.std::__1::__wrap_iter", align 8
  %40 = alloca %"class.std::__1::__wrap_iter", align 8
  %41 = alloca %"class.std::__1::shared_ptr.58", align 8
  %42 = alloca %struct.Variant, align 8
  %43 = alloca i8*
  %44 = alloca i32
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %35, align 8
  store %class.ExportStmt* %2, %class.ExportStmt** %36, align 8
  %45 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %35, align 8
  %46 = load %class.ExportStmt*, %class.ExportStmt** %36, align 8
  %47 = bitcast %class.ExportStmt* %46 to void (%"class.std::__1::vector.56"*, %class.ExportStmt*)***
  %48 = load void (%"class.std::__1::vector.56"*, %class.ExportStmt*)**, void (%"class.std::__1::vector.56"*, %class.ExportStmt*)*** %47, align 8
  %49 = getelementptr inbounds void (%"class.std::__1::vector.56"*, %class.ExportStmt*)*, void (%"class.std::__1::vector.56"*, %class.ExportStmt*)** %48, i64 2
  %50 = load void (%"class.std::__1::vector.56"*, %class.ExportStmt*)*, void (%"class.std::__1::vector.56"*, %class.ExportStmt*)** %49, align 8
  call void %50(%"class.std::__1::vector.56"* sret %38, %class.ExportStmt* %46)
  store %"class.std::__1::vector.56"* %38, %"class.std::__1::vector.56"** %37, align 8
  %51 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %51, %"class.std::__1::vector.56"** %34, align 8
  %52 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %34, align 8
  %53 = bitcast %"class.std::__1::vector.56"* %52 to %"class.std::__1::__vector_base.57"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %53, i32 0, i32 0
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::vector.56"* %52, %"class.std::__1::vector.56"** %31, align 8
  store %"class.std::__1::shared_ptr.58"* %55, %"class.std::__1::shared_ptr.58"** %32, align 8
  %56 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %31, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %32, align 8
  store %"class.std::__1::__wrap_iter"* %30, %"class.std::__1::__wrap_iter"** %28, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %29, align 8
  %58 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %28, align 8
  %59 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %29, align 8
  store %"class.std::__1::__wrap_iter"* %58, %"class.std::__1::__wrap_iter"** %26, align 8
  store %"class.std::__1::shared_ptr.58"* %59, %"class.std::__1::shared_ptr.58"** %27, align 8
  %60 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %26, align 8
  %61 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %60, i32 0, i32 0
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %27, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %30, i32 0, i32 0
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %63, align 8
  %65 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %64, %"class.std::__1::shared_ptr.58"** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %33, i32 0, i32 0
  %67 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %39, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %67, %"class.std::__1::shared_ptr.58"** %68, align 8
  %69 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %37, align 8
  store %"class.std::__1::vector.56"* %69, %"class.std::__1::vector.56"** %25, align 8
  %70 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %25, align 8
  %71 = bitcast %"class.std::__1::vector.56"* %70 to %"class.std::__1::__vector_base.57"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::vector.56"* %70, %"class.std::__1::vector.56"** %22, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %23, align 8
  %74 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %22, align 8
  %75 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %23, align 8
  store %"class.std::__1::__wrap_iter"* %21, %"class.std::__1::__wrap_iter"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %75, %"class.std::__1::shared_ptr.58"** %20, align 8
  %76 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %19, align 8
  %77 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::__wrap_iter"* %76, %"class.std::__1::__wrap_iter"** %17, align 8
  store %"class.std::__1::shared_ptr.58"* %77, %"class.std::__1::shared_ptr.58"** %18, align 8
  %78 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %17, align 8
  %79 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %78, i32 0, i32 0
  %80 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %18, align 8
  store %"class.std::__1::shared_ptr.58"* %80, %"class.std::__1::shared_ptr.58"** %79, align 8
  %81 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %21, i32 0, i32 0
  %82 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %82, %"class.std::__1::shared_ptr.58"** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %24, i32 0, i32 0
  %85 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %40, i32 0, i32 0
  store %"class.std::__1::shared_ptr.58"* %85, %"class.std::__1::shared_ptr.58"** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %132, %3
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %15, align 8
  store %"class.std::__1::__wrap_iter"* %40, %"class.std::__1::__wrap_iter"** %16, align 8
  %88 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %15, align 8
  %89 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %16, align 8
  store %"class.std::__1::__wrap_iter"* %88, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %89, %"class.std::__1::__wrap_iter"** %14, align 8
  %90 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %13, align 8
  store %"class.std::__1::__wrap_iter"* %90, %"class.std::__1::__wrap_iter"** %12, align 8
  %91 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %91, i32 0, i32 0
  %93 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %92, align 8
  %94 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %14, align 8
  store %"class.std::__1::__wrap_iter"* %94, %"class.std::__1::__wrap_iter"** %11, align 8
  %95 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %11, align 8
  %96 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %95, i32 0, i32 0
  %97 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %96, align 8
  %98 = icmp eq %"class.std::__1::shared_ptr.58"* %93, %97
  %99 = xor i1 %98, true
  br i1 %99, label %101, label %100

; <label>:100:                                    ; preds = %87
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %141

; <label>:101:                                    ; preds = %87
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %9, align 8
  %102 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %9, align 8
  %103 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %102, i32 0, i32 0
  %104 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %103, align 8
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %104, %"class.std::__1::shared_ptr.58"** %8, align 8
  %105 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %7, align 8
  %106 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  store %"class.std::__1::shared_ptr.58"* %105, %"class.std::__1::shared_ptr.58"** %5, align 8
  store %"class.std::__1::shared_ptr.58"* %106, %"class.std::__1::shared_ptr.58"** %6, align 8
  %107 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %5, align 8
  %108 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 0
  %109 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %110 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %109, i32 0, i32 0
  %111 = load %class.AstNode*, %class.AstNode** %110, align 8
  store %class.AstNode* %111, %class.AstNode** %108, align 8
  %112 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %113 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %6, align 8
  %114 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %113, i32 0, i32 1
  %115 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %114, align 8
  store %"class.std::__1::__shared_weak_count"* %115, %"class.std::__1::__shared_weak_count"** %112, align 8
  %116 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %117 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %116, align 8
  %118 = icmp ne %"class.std::__1::__shared_weak_count"* %117, null
  br i1 %118, label %119, label %122

; <label>:119:                                    ; preds = %101
  %120 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %107, i32 0, i32 1
  %121 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %120, align 8
  call void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"* %121) #7
  br label %122

; <label>:122:                                    ; preds = %101, %119
  store %"class.std::__1::shared_ptr.58"* %41, %"class.std::__1::shared_ptr.58"** %4, align 8
  %123 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %4, align 8
  %124 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %123, i32 0, i32 0
  %125 = load %class.AstNode*, %class.AstNode** %124, align 8
  %126 = bitcast %class.AstNode* %125 to void (%struct.Variant*, %class.AstNode*, %class.Visitor*)***
  %127 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)**, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*** %126, align 8
  %128 = getelementptr inbounds void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %127, i64 16
  %129 = load void (%struct.Variant*, %class.AstNode*, %class.Visitor*)*, void (%struct.Variant*, %class.AstNode*, %class.Visitor*)** %128, align 8
  %130 = bitcast %class.ConstExprVisitor* %45 to %class.Visitor*
  invoke void %129(%struct.Variant* sret %42, %class.AstNode* %125, %class.Visitor* dereferenceable(8) %130)
          to label %131 unwind label %137

; <label>:131:                                    ; preds = %122
  call void @_ZN7VariantD1Ev(%struct.Variant* %42) #7
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  br label %132

; <label>:132:                                    ; preds = %131
  store %"class.std::__1::__wrap_iter"* %39, %"class.std::__1::__wrap_iter"** %10, align 8
  %133 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %10, align 8
  %134 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %133, i32 0, i32 0
  %135 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %134, align 8
  %136 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %135, i32 1
  store %"class.std::__1::shared_ptr.58"* %136, %"class.std::__1::shared_ptr.58"** %134, align 8
  br label %87

; <label>:137:                                    ; preds = %122
  %138 = landingpad { i8*, i32 }
          cleanup
  %139 = extractvalue { i8*, i32 } %138, 0
  store i8* %139, i8** %43, align 8
  %140 = extractvalue { i8*, i32 } %138, 1
  store i32 %140, i32* %44, align 4
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %41) #7
  call void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector.56"* %38) #7
  br label %142

; <label>:141:                                    ; preds = %100
  call void @_ZN7VariantC1Ev(%struct.Variant* %0)
  ret void

; <label>:142:                                    ; preds = %137
  %143 = load i8*, i8** %43, align 8
  %144 = load i32, i32* %44, align 4
  %145 = insertvalue { i8*, i32 } undef, i8* %143, 0
  %146 = insertvalue { i8*, i32 } %145, i32 %144, 1
  resume { i8*, i32 } %146
}

; Function Attrs: nounwind ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP13InterfaceDecl(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.InterfaceDecl*) unnamed_addr #2 align 2 {
  %4 = alloca %class.ConstExprVisitor*, align 8
  %5 = alloca %class.InterfaceDecl*, align 8
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %4, align 8
  store %class.InterfaceDecl* %2, %class.InterfaceDecl** %5, align 8
  %6 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %4, align 8
  ret void
}

; Function Attrs: nounwind ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP16ImplicitCastExpr(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.ImplicitCastExpr*) unnamed_addr #2 align 2 {
  %4 = alloca %class.ConstExprVisitor*, align 8
  %5 = alloca %class.ImplicitCastExpr*, align 8
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %4, align 8
  store %class.ImplicitCastExpr* %2, %class.ImplicitCastExpr** %5, align 8
  %6 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %4, align 8
  ret void
}

; Function Attrs: nounwind ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP10ExtendStmt(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.ExtendStmt*) unnamed_addr #2 align 2 {
  %4 = alloca %class.ConstExprVisitor*, align 8
  %5 = alloca %class.ExtendStmt*, align 8
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %4, align 8
  store %class.ExtendStmt* %2, %class.ExtendStmt** %5, align 8
  %6 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %4, align 8
  ret void
}

; Function Attrs: nounwind ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP11TypedefDecl(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.TypedefDecl*) unnamed_addr #2 align 2 {
  %4 = alloca %class.ConstExprVisitor*, align 8
  %5 = alloca %class.TypedefDecl*, align 8
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %4, align 8
  store %class.TypedefDecl* %2, %class.TypedefDecl** %5, align 8
  %6 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %4, align 8
  ret void
}

; Function Attrs: nounwind ssp uwtable
define void @_ZN16ConstExprVisitor5visitEP7TypeRef(%struct.Variant* noalias sret, %class.ConstExprVisitor*, %class.TypeRef*) unnamed_addr #2 align 2 {
  %4 = alloca %class.ConstExprVisitor*, align 8
  %5 = alloca %class.TypeRef*, align 8
  store %class.ConstExprVisitor* %1, %class.ConstExprVisitor** %4, align 8
  store %class.TypeRef* %2, %class.TypeRef** %5, align 8
  %6 = load %class.ConstExprVisitor*, %class.ConstExprVisitor** %4, align 8
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN7VariantD2Ev(%struct.Variant*) unnamed_addr #1 align 2 {
  %2 = alloca %struct.Variant*, align 8
  store %struct.Variant* %0, %struct.Variant** %2, align 8
  %3 = load %struct.Variant*, %struct.Variant** %2, align 8
  %4 = getelementptr inbounds %struct.Variant, %struct.Variant* %3, i32 0, i32 4
  call void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier* %4) #7
  %5 = getelementptr inbounds %struct.Variant, %struct.Variant* %3, i32 0, i32 3
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %5) #7
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier*) unnamed_addr #1 align 2 {
  %2 = alloca %struct.TypeSpecifier*, align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %2, align 8
  %3 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %2, align 8
  call void @_ZN13TypeSpecifierD2Ev(%struct.TypeSpecifier* %3) #7
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN13TypeSpecifierD2Ev(%struct.TypeSpecifier*) unnamed_addr #1 align 2 {
  %2 = alloca %struct.TypeSpecifier*, align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %2, align 8
  %3 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %2, align 8
  %4 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 24
  call void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED1Ev(%"class.std::__1::vector.19"* %4) #7
  %5 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 23
  call void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED1Ev(%"class.std::__1::vector"* %5) #7
  %6 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 14
  call void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED1Ev(%"class.std::__1::unordered_map"* %6) #7
  %7 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 11
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %7) #7
  %8 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 2
  call void @_ZNSt3__110shared_ptrI10ExpressionED1Ev(%"class.std::__1::shared_ptr"* %8) #7
  %9 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 1
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %9) #7
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED1Ev(%"class.std::__1::vector.19"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::vector.19"*, align 8
  store %"class.std::__1::vector.19"* %0, %"class.std::__1::vector.19"** %2, align 8
  %3 = load %"class.std::__1::vector.19"*, %"class.std::__1::vector.19"** %2, align 8
  call void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::vector.19"* %3) #7
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED1Ev(%"class.std::__1::vector"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::vector"*, align 8
  store %"class.std::__1::vector"* %0, %"class.std::__1::vector"** %2, align 8
  %3 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %2, align 8
  call void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::vector"* %3) #7
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED1Ev(%"class.std::__1::unordered_map"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::unordered_map"*, align 8
  store %"class.std::__1::unordered_map"* %0, %"class.std::__1::unordered_map"** %2, align 8
  %3 = load %"class.std::__1::unordered_map"*, %"class.std::__1::unordered_map"** %2, align 8
  call void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED2Ev(%"class.std::__1::unordered_map"* %3) #7
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__110shared_ptrI10ExpressionED1Ev(%"class.std::__1::shared_ptr"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::shared_ptr"*, align 8
  store %"class.std::__1::shared_ptr"* %0, %"class.std::__1::shared_ptr"** %2, align 8
  %3 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %2, align 8
  call void @_ZNSt3__110shared_ptrI10ExpressionED2Ev(%"class.std::__1::shared_ptr"* %3) #7
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::vector.19"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::vector.19"*, align 8
  store %"class.std::__1::vector.19"* %0, %"class.std::__1::vector.19"** %2, align 8
  %3 = load %"class.std::__1::vector.19"*, %"class.std::__1::vector.19"** %2, align 8
  %4 = bitcast %"class.std::__1::vector.19"* %3 to %"class.std::__1::__vector_base.20"*
  call void @_ZNSt3__113__vector_baseI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::__vector_base.20"* %4) #7
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113__vector_baseI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::__vector_base.20"*) unnamed_addr #2 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::allocator.23"*, align 8
  %3 = alloca %struct.TypeSpecifier*, align 8
  %4 = alloca %"struct.std::__1::integral_constant", align 1
  %5 = alloca %"class.std::__1::allocator.23"*, align 8
  %6 = alloca %struct.TypeSpecifier*, align 8
  %7 = alloca %"class.std::__1::allocator.23"*, align 8
  %8 = alloca %struct.TypeSpecifier*, align 8
  %9 = alloca %"struct.std::__1::integral_constant", align 1
  %10 = alloca %"struct.std::__1::__has_destroy", align 1
  %11 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.22"*, align 8
  %12 = alloca %"class.std::__1::__compressed_pair.21"*, align 8
  %13 = alloca %"class.std::__1::__vector_base.20"*, align 8
  %14 = alloca %struct.TypeSpecifier*, align 8
  %15 = alloca %"class.std::__1::__vector_base.20"*, align 8
  %16 = alloca %struct.TypeSpecifier*, align 8
  %17 = alloca %"class.std::__1::__vector_base.20"*, align 8
  %18 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.22"*, align 8
  %19 = alloca %"class.std::__1::__compressed_pair.21"*, align 8
  %20 = alloca %"class.std::__1::__vector_base.20"*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.22"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.21"*, align 8
  %23 = alloca %"class.std::__1::__vector_base.20"*, align 8
  %24 = alloca %"class.std::__1::__vector_base.20"*, align 8
  %25 = alloca i8*, align 8
  %26 = alloca %"class.std::__1::allocator.23"*, align 8
  %27 = alloca %struct.TypeSpecifier*, align 8
  %28 = alloca i64, align 8
  %29 = alloca %"class.std::__1::allocator.23"*, align 8
  %30 = alloca %struct.TypeSpecifier*, align 8
  %31 = alloca i64, align 8
  %32 = alloca %"class.std::__1::__vector_base.20"*, align 8
  store %"class.std::__1::__vector_base.20"* %0, %"class.std::__1::__vector_base.20"** %32, align 8
  %33 = load %"class.std::__1::__vector_base.20"*, %"class.std::__1::__vector_base.20"** %32, align 8
  %34 = getelementptr inbounds %"class.std::__1::__vector_base.20", %"class.std::__1::__vector_base.20"* %33, i32 0, i32 0
  %35 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %34, align 8
  %36 = icmp ne %struct.TypeSpecifier* %35, null
  br i1 %36, label %37, label %95

; <label>:37:                                     ; preds = %1
  store %"class.std::__1::__vector_base.20"* %33, %"class.std::__1::__vector_base.20"** %17, align 8
  %38 = load %"class.std::__1::__vector_base.20"*, %"class.std::__1::__vector_base.20"** %17, align 8
  %39 = getelementptr inbounds %"class.std::__1::__vector_base.20", %"class.std::__1::__vector_base.20"* %38, i32 0, i32 0
  %40 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %39, align 8
  store %"class.std::__1::__vector_base.20"* %38, %"class.std::__1::__vector_base.20"** %15, align 8
  store %struct.TypeSpecifier* %40, %struct.TypeSpecifier** %16, align 8
  %41 = load %"class.std::__1::__vector_base.20"*, %"class.std::__1::__vector_base.20"** %15, align 8
  br label %42

; <label>:42:                                     ; preds = %47, %37
  %43 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %16, align 8
  %44 = getelementptr inbounds %"class.std::__1::__vector_base.20", %"class.std::__1::__vector_base.20"* %41, i32 0, i32 1
  %45 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %44, align 8
  %46 = icmp ne %struct.TypeSpecifier* %43, %45
  br i1 %46, label %47, label %65

; <label>:47:                                     ; preds = %42
  store %"class.std::__1::__vector_base.20"* %41, %"class.std::__1::__vector_base.20"** %13, align 8
  %48 = load %"class.std::__1::__vector_base.20"*, %"class.std::__1::__vector_base.20"** %13, align 8
  %49 = getelementptr inbounds %"class.std::__1::__vector_base.20", %"class.std::__1::__vector_base.20"* %48, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.21"* %49, %"class.std::__1::__compressed_pair.21"** %12, align 8
  %50 = load %"class.std::__1::__compressed_pair.21"*, %"class.std::__1::__compressed_pair.21"** %12, align 8
  %51 = bitcast %"class.std::__1::__compressed_pair.21"* %50 to %"class.std::__1::__libcpp_compressed_pair_imp.22"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.22"* %51, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %11, align 8
  %52 = load %"class.std::__1::__libcpp_compressed_pair_imp.22"*, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %11, align 8
  %53 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.22"* %52 to %"class.std::__1::allocator.23"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.20", %"class.std::__1::__vector_base.20"* %41, i32 0, i32 1
  %55 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %54, align 8
  %56 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %55, i32 -1
  store %struct.TypeSpecifier* %56, %struct.TypeSpecifier** %54, align 8
  store %struct.TypeSpecifier* %56, %struct.TypeSpecifier** %14, align 8
  %57 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %14, align 8
  store %"class.std::__1::allocator.23"* %53, %"class.std::__1::allocator.23"** %7, align 8
  store %struct.TypeSpecifier* %57, %struct.TypeSpecifier** %8, align 8
  %58 = bitcast %"struct.std::__1::__has_destroy"* %10 to %"struct.std::__1::integral_constant"*
  %59 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %7, align 8
  %60 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %8, align 8
  store %"class.std::__1::allocator.23"* %59, %"class.std::__1::allocator.23"** %5, align 8
  store %struct.TypeSpecifier* %60, %struct.TypeSpecifier** %6, align 8
  %61 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %5, align 8
  %62 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %6, align 8
  store %"class.std::__1::allocator.23"* %61, %"class.std::__1::allocator.23"** %2, align 8
  store %struct.TypeSpecifier* %62, %struct.TypeSpecifier** %3, align 8
  %63 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %2, align 8
  %64 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  call void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier* %64) #7
  br label %42

; <label>:65:                                     ; preds = %42
  store %"class.std::__1::__vector_base.20"* %33, %"class.std::__1::__vector_base.20"** %20, align 8
  %66 = load %"class.std::__1::__vector_base.20"*, %"class.std::__1::__vector_base.20"** %20, align 8
  %67 = getelementptr inbounds %"class.std::__1::__vector_base.20", %"class.std::__1::__vector_base.20"* %66, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.21"* %67, %"class.std::__1::__compressed_pair.21"** %19, align 8
  %68 = load %"class.std::__1::__compressed_pair.21"*, %"class.std::__1::__compressed_pair.21"** %19, align 8
  %69 = bitcast %"class.std::__1::__compressed_pair.21"* %68 to %"class.std::__1::__libcpp_compressed_pair_imp.22"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.22"* %69, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %18, align 8
  %70 = load %"class.std::__1::__libcpp_compressed_pair_imp.22"*, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %18, align 8
  %71 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.22"* %70 to %"class.std::__1::allocator.23"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.20", %"class.std::__1::__vector_base.20"* %33, i32 0, i32 0
  %73 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %72, align 8
  store %"class.std::__1::__vector_base.20"* %33, %"class.std::__1::__vector_base.20"** %24, align 8
  %74 = load %"class.std::__1::__vector_base.20"*, %"class.std::__1::__vector_base.20"** %24, align 8
  store %"class.std::__1::__vector_base.20"* %74, %"class.std::__1::__vector_base.20"** %23, align 8
  %75 = load %"class.std::__1::__vector_base.20"*, %"class.std::__1::__vector_base.20"** %23, align 8
  %76 = getelementptr inbounds %"class.std::__1::__vector_base.20", %"class.std::__1::__vector_base.20"* %75, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.21"* %76, %"class.std::__1::__compressed_pair.21"** %22, align 8
  %77 = load %"class.std::__1::__compressed_pair.21"*, %"class.std::__1::__compressed_pair.21"** %22, align 8
  %78 = bitcast %"class.std::__1::__compressed_pair.21"* %77 to %"class.std::__1::__libcpp_compressed_pair_imp.22"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.22"* %78, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %21, align 8
  %79 = load %"class.std::__1::__libcpp_compressed_pair_imp.22"*, %"class.std::__1::__libcpp_compressed_pair_imp.22"** %21, align 8
  %80 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.22", %"class.std::__1::__libcpp_compressed_pair_imp.22"* %79, i32 0, i32 0
  %81 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %80, align 8
  %82 = getelementptr inbounds %"class.std::__1::__vector_base.20", %"class.std::__1::__vector_base.20"* %74, i32 0, i32 0
  %83 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %82, align 8
  %84 = ptrtoint %struct.TypeSpecifier* %81 to i64
  %85 = ptrtoint %struct.TypeSpecifier* %83 to i64
  %86 = sub i64 %84, %85
  %87 = sdiv exact i64 %86, 216
  store %"class.std::__1::allocator.23"* %71, %"class.std::__1::allocator.23"** %29, align 8
  store %struct.TypeSpecifier* %73, %struct.TypeSpecifier** %30, align 8
  store i64 %87, i64* %31, align 8
  %88 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %29, align 8
  %89 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %30, align 8
  %90 = load i64, i64* %31, align 8
  store %"class.std::__1::allocator.23"* %88, %"class.std::__1::allocator.23"** %26, align 8
  store %struct.TypeSpecifier* %89, %struct.TypeSpecifier** %27, align 8
  store i64 %90, i64* %28, align 8
  %91 = load %"class.std::__1::allocator.23"*, %"class.std::__1::allocator.23"** %26, align 8
  %92 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %27, align 8
  %93 = bitcast %struct.TypeSpecifier* %92 to i8*
  store i8* %93, i8** %25, align 8
  %94 = load i8*, i8** %25, align 8
  call void @_ZdlPv(i8* %94) #8
  br label %95

; <label>:95:                                     ; preds = %65, %1
  ret void
}

; Function Attrs: noinline noreturn nounwind
define linkonce_odr hidden void @__clang_call_terminate(i8*) #5 {
  %2 = call i8* @__cxa_begin_catch(i8* %0) #7
  call void @_ZSt9terminatev() #9
  unreachable
}

declare i8* @__cxa_begin_catch(i8*)

declare void @_ZSt9terminatev()

; Function Attrs: nobuiltin nounwind
declare void @_ZdlPv(i8*) #6

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::vector"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::vector"*, align 8
  store %"class.std::__1::vector"* %0, %"class.std::__1::vector"** %2, align 8
  %3 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %2, align 8
  %4 = bitcast %"class.std::__1::vector"* %3 to %"class.std::__1::__vector_base"*
  call void @_ZNSt3__113__vector_baseINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::__vector_base"* %4) #7
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113__vector_baseINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::__vector_base"*) unnamed_addr #2 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.2"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.1"*, align 8
  %4 = alloca %"class.std::__1::__vector_base"*, align 8
  %5 = alloca %"class.std::__1::__vector_base"*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %"class.std::__1::allocator.3"*, align 8
  %8 = alloca %"class.std::__1::basic_string"*, align 8
  %9 = alloca i64, align 8
  %10 = alloca %"class.std::__1::allocator.3"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.2"*, align 8
  %14 = alloca %"class.std::__1::__compressed_pair.1"*, align 8
  %15 = alloca %"class.std::__1::__vector_base"*, align 8
  %16 = alloca %"class.std::__1::allocator.3"*, align 8
  %17 = alloca %"class.std::__1::basic_string"*, align 8
  %18 = alloca %"struct.std::__1::integral_constant", align 1
  %19 = alloca %"class.std::__1::allocator.3"*, align 8
  %20 = alloca %"class.std::__1::basic_string"*, align 8
  %21 = alloca %"class.std::__1::allocator.3"*, align 8
  %22 = alloca %"class.std::__1::basic_string"*, align 8
  %23 = alloca %"struct.std::__1::integral_constant", align 1
  %24 = alloca %"struct.std::__1::__has_destroy.328", align 1
  %25 = alloca %"class.std::__1::basic_string"*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.2"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.1"*, align 8
  %28 = alloca %"class.std::__1::__vector_base"*, align 8
  %29 = alloca %"class.std::__1::__vector_base"*, align 8
  %30 = alloca %"class.std::__1::basic_string"*, align 8
  %31 = alloca %"class.std::__1::__vector_base"*, align 8
  %32 = alloca %"class.std::__1::__vector_base"*, align 8
  store %"class.std::__1::__vector_base"* %0, %"class.std::__1::__vector_base"** %32, align 8
  %33 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %32, align 8
  %34 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %33, i32 0, i32 0
  %35 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %34, align 8
  %36 = icmp ne %"class.std::__1::basic_string"* %35, null
  br i1 %36, label %37, label %95

; <label>:37:                                     ; preds = %1
  store %"class.std::__1::__vector_base"* %33, %"class.std::__1::__vector_base"** %31, align 8
  %38 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %31, align 8
  %39 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %38, i32 0, i32 0
  %40 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8
  store %"class.std::__1::__vector_base"* %38, %"class.std::__1::__vector_base"** %29, align 8
  store %"class.std::__1::basic_string"* %40, %"class.std::__1::basic_string"** %30, align 8
  %41 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %29, align 8
  br label %42

; <label>:42:                                     ; preds = %47, %37
  %43 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %30, align 8
  %44 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %41, i32 0, i32 1
  %45 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %44, align 8
  %46 = icmp ne %"class.std::__1::basic_string"* %43, %45
  br i1 %46, label %47, label %65

; <label>:47:                                     ; preds = %42
  store %"class.std::__1::__vector_base"* %41, %"class.std::__1::__vector_base"** %28, align 8
  %48 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %28, align 8
  %49 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %48, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.1"* %49, %"class.std::__1::__compressed_pair.1"** %27, align 8
  %50 = load %"class.std::__1::__compressed_pair.1"*, %"class.std::__1::__compressed_pair.1"** %27, align 8
  %51 = bitcast %"class.std::__1::__compressed_pair.1"* %50 to %"class.std::__1::__libcpp_compressed_pair_imp.2"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.2"* %51, %"class.std::__1::__libcpp_compressed_pair_imp.2"** %26, align 8
  %52 = load %"class.std::__1::__libcpp_compressed_pair_imp.2"*, %"class.std::__1::__libcpp_compressed_pair_imp.2"** %26, align 8
  %53 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.2"* %52 to %"class.std::__1::allocator.3"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %41, i32 0, i32 1
  %55 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8
  %56 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %55, i32 -1
  store %"class.std::__1::basic_string"* %56, %"class.std::__1::basic_string"** %54, align 8
  store %"class.std::__1::basic_string"* %56, %"class.std::__1::basic_string"** %25, align 8
  %57 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %25, align 8
  store %"class.std::__1::allocator.3"* %53, %"class.std::__1::allocator.3"** %21, align 8
  store %"class.std::__1::basic_string"* %57, %"class.std::__1::basic_string"** %22, align 8
  %58 = bitcast %"struct.std::__1::__has_destroy.328"* %24 to %"struct.std::__1::integral_constant"*
  %59 = load %"class.std::__1::allocator.3"*, %"class.std::__1::allocator.3"** %21, align 8
  %60 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %22, align 8
  store %"class.std::__1::allocator.3"* %59, %"class.std::__1::allocator.3"** %19, align 8
  store %"class.std::__1::basic_string"* %60, %"class.std::__1::basic_string"** %20, align 8
  %61 = load %"class.std::__1::allocator.3"*, %"class.std::__1::allocator.3"** %19, align 8
  %62 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8
  store %"class.std::__1::allocator.3"* %61, %"class.std::__1::allocator.3"** %16, align 8
  store %"class.std::__1::basic_string"* %62, %"class.std::__1::basic_string"** %17, align 8
  %63 = load %"class.std::__1::allocator.3"*, %"class.std::__1::allocator.3"** %16, align 8
  %64 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %64) #7
  br label %42

; <label>:65:                                     ; preds = %42
  store %"class.std::__1::__vector_base"* %33, %"class.std::__1::__vector_base"** %15, align 8
  %66 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %15, align 8
  %67 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %66, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.1"* %67, %"class.std::__1::__compressed_pair.1"** %14, align 8
  %68 = load %"class.std::__1::__compressed_pair.1"*, %"class.std::__1::__compressed_pair.1"** %14, align 8
  %69 = bitcast %"class.std::__1::__compressed_pair.1"* %68 to %"class.std::__1::__libcpp_compressed_pair_imp.2"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.2"* %69, %"class.std::__1::__libcpp_compressed_pair_imp.2"** %13, align 8
  %70 = load %"class.std::__1::__libcpp_compressed_pair_imp.2"*, %"class.std::__1::__libcpp_compressed_pair_imp.2"** %13, align 8
  %71 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.2"* %70 to %"class.std::__1::allocator.3"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %33, i32 0, i32 0
  %73 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %72, align 8
  store %"class.std::__1::__vector_base"* %33, %"class.std::__1::__vector_base"** %5, align 8
  %74 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %5, align 8
  store %"class.std::__1::__vector_base"* %74, %"class.std::__1::__vector_base"** %4, align 8
  %75 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %4, align 8
  %76 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %75, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.1"* %76, %"class.std::__1::__compressed_pair.1"** %3, align 8
  %77 = load %"class.std::__1::__compressed_pair.1"*, %"class.std::__1::__compressed_pair.1"** %3, align 8
  %78 = bitcast %"class.std::__1::__compressed_pair.1"* %77 to %"class.std::__1::__libcpp_compressed_pair_imp.2"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.2"* %78, %"class.std::__1::__libcpp_compressed_pair_imp.2"** %2, align 8
  %79 = load %"class.std::__1::__libcpp_compressed_pair_imp.2"*, %"class.std::__1::__libcpp_compressed_pair_imp.2"** %2, align 8
  %80 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.2", %"class.std::__1::__libcpp_compressed_pair_imp.2"* %79, i32 0, i32 0
  %81 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %80, align 8
  %82 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %74, i32 0, i32 0
  %83 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %82, align 8
  %84 = ptrtoint %"class.std::__1::basic_string"* %81 to i64
  %85 = ptrtoint %"class.std::__1::basic_string"* %83 to i64
  %86 = sub i64 %84, %85
  %87 = sdiv exact i64 %86, 24
  store %"class.std::__1::allocator.3"* %71, %"class.std::__1::allocator.3"** %10, align 8
  store %"class.std::__1::basic_string"* %73, %"class.std::__1::basic_string"** %11, align 8
  store i64 %87, i64* %12, align 8
  %88 = load %"class.std::__1::allocator.3"*, %"class.std::__1::allocator.3"** %10, align 8
  %89 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8
  %90 = load i64, i64* %12, align 8
  store %"class.std::__1::allocator.3"* %88, %"class.std::__1::allocator.3"** %7, align 8
  store %"class.std::__1::basic_string"* %89, %"class.std::__1::basic_string"** %8, align 8
  store i64 %90, i64* %9, align 8
  %91 = load %"class.std::__1::allocator.3"*, %"class.std::__1::allocator.3"** %7, align 8
  %92 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %8, align 8
  %93 = bitcast %"class.std::__1::basic_string"* %92 to i8*
  store i8* %93, i8** %6, align 8
  %94 = load i8*, i8** %6, align 8
  call void @_ZdlPv(i8* %94) #8
  br label %95

; <label>:95:                                     ; preds = %65, %1
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED2Ev(%"class.std::__1::unordered_map"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::unordered_map"*, align 8
  store %"class.std::__1::unordered_map"* %0, %"class.std::__1::unordered_map"** %2, align 8
  %3 = load %"class.std::__1::unordered_map"*, %"class.std::__1::unordered_map"** %2, align 8
  %4 = getelementptr inbounds %"class.std::__1::unordered_map", %"class.std::__1::unordered_map"* %3, i32 0, i32 0
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED1Ev(%"class.std::__1::__hash_table"* %4) #7
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED1Ev(%"class.std::__1::__hash_table"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::__hash_table"*, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %2, align 8
  %3 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %2, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED2Ev(%"class.std::__1::__hash_table"* %3) #7
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED2Ev(%"class.std::__1::__hash_table"*) unnamed_addr #2 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.6"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.5"*, align 8
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.6"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair.5"*, align 8
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.8"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair.7"*, align 8
  %8 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca %"class.std::__1::allocator.9"*, align 8
  %11 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::allocator.9"*, align 8
  %14 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %15 = alloca i64, align 8
  %16 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.8"*, align 8
  %17 = alloca %"class.std::__1::__compressed_pair.7"*, align 8
  %18 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %19 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %20 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.6"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.5"*, align 8
  %23 = alloca %"class.std::__1::unique_ptr"*, align 8
  %24 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %25 = alloca %"class.std::__1::unique_ptr"*, align 8
  %26 = alloca %"class.std::__1::unique_ptr"*, align 8
  %27 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.12"*, align 8
  %28 = alloca %"class.std::__1::__compressed_pair.11"*, align 8
  %29 = alloca %"class.std::__1::__hash_table"*, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %29, align 8
  %30 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %29, align 8
  %31 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %30, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.11"* %31, %"class.std::__1::__compressed_pair.11"** %28, align 8
  %32 = load %"class.std::__1::__compressed_pair.11"*, %"class.std::__1::__compressed_pair.11"** %28, align 8
  %33 = bitcast %"class.std::__1::__compressed_pair.11"* %32 to %"class.std::__1::__libcpp_compressed_pair_imp.12"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.12"* %33, %"class.std::__1::__libcpp_compressed_pair_imp.12"** %27, align 8
  %34 = load %"class.std::__1::__libcpp_compressed_pair_imp.12"*, %"class.std::__1::__libcpp_compressed_pair_imp.12"** %27, align 8
  %35 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.12", %"class.std::__1::__libcpp_compressed_pair_imp.12"* %34, i32 0, i32 0
  %36 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %35, i32 0, i32 0
  %37 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %36, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEE12__deallocateEPNS_16__hash_node_baseIPNS_11__hash_nodeIS9_PvEEEE(%"class.std::__1::__hash_table"* %30, %"struct.std::__1::__hash_node_base"* %37) #7
  %38 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %30, i32 0, i32 0
  store %"class.std::__1::unique_ptr"* %38, %"class.std::__1::unique_ptr"** %26, align 8
  %39 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %26, align 8
  store %"class.std::__1::unique_ptr"* %39, %"class.std::__1::unique_ptr"** %25, align 8
  %40 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %25, align 8
  store %"class.std::__1::unique_ptr"* %40, %"class.std::__1::unique_ptr"** %23, align 8
  %41 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %23, align 8
  %42 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.5"* %42, %"class.std::__1::__compressed_pair.5"** %22, align 8
  %43 = load %"class.std::__1::__compressed_pair.5"*, %"class.std::__1::__compressed_pair.5"** %22, align 8
  %44 = bitcast %"class.std::__1::__compressed_pair.5"* %43 to %"class.std::__1::__libcpp_compressed_pair_imp.6"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.6"* %44, %"class.std::__1::__libcpp_compressed_pair_imp.6"** %21, align 8
  %45 = load %"class.std::__1::__libcpp_compressed_pair_imp.6"*, %"class.std::__1::__libcpp_compressed_pair_imp.6"** %21, align 8
  %46 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.6", %"class.std::__1::__libcpp_compressed_pair_imp.6"* %45, i32 0, i32 0
  %47 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %46, align 8
  store %"struct.std::__1::__hash_node_base"** %47, %"struct.std::__1::__hash_node_base"*** %24, align 8
  %48 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.5"* %48, %"class.std::__1::__compressed_pair.5"** %5, align 8
  %49 = load %"class.std::__1::__compressed_pair.5"*, %"class.std::__1::__compressed_pair.5"** %5, align 8
  %50 = bitcast %"class.std::__1::__compressed_pair.5"* %49 to %"class.std::__1::__libcpp_compressed_pair_imp.6"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.6"* %50, %"class.std::__1::__libcpp_compressed_pair_imp.6"** %4, align 8
  %51 = load %"class.std::__1::__libcpp_compressed_pair_imp.6"*, %"class.std::__1::__libcpp_compressed_pair_imp.6"** %4, align 8
  %52 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.6", %"class.std::__1::__libcpp_compressed_pair_imp.6"* %51, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"** null, %"struct.std::__1::__hash_node_base"*** %52, align 8
  %53 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %24, align 8
  %54 = icmp ne %"struct.std::__1::__hash_node_base"** %53, null
  br i1 %54, label %55, label %84

; <label>:55:                                     ; preds = %1
  %56 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.5"* %56, %"class.std::__1::__compressed_pair.5"** %3, align 8
  %57 = load %"class.std::__1::__compressed_pair.5"*, %"class.std::__1::__compressed_pair.5"** %3, align 8
  %58 = bitcast %"class.std::__1::__compressed_pair.5"* %57 to %"class.std::__1::__libcpp_compressed_pair_imp.6"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.6"* %58, %"class.std::__1::__libcpp_compressed_pair_imp.6"** %2, align 8
  %59 = load %"class.std::__1::__libcpp_compressed_pair_imp.6"*, %"class.std::__1::__libcpp_compressed_pair_imp.6"** %2, align 8
  %60 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.6", %"class.std::__1::__libcpp_compressed_pair_imp.6"* %59, i32 0, i32 1
  %61 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %24, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %60, %"class.std::__1::__bucket_list_deallocator"** %19, align 8
  store %"struct.std::__1::__hash_node_base"** %61, %"struct.std::__1::__hash_node_base"*** %20, align 8
  %62 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %19, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %62, %"class.std::__1::__bucket_list_deallocator"** %18, align 8
  %63 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %18, align 8
  %64 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %63, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.7"* %64, %"class.std::__1::__compressed_pair.7"** %17, align 8
  %65 = load %"class.std::__1::__compressed_pair.7"*, %"class.std::__1::__compressed_pair.7"** %17, align 8
  %66 = bitcast %"class.std::__1::__compressed_pair.7"* %65 to %"class.std::__1::__libcpp_compressed_pair_imp.8"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.8"* %66, %"class.std::__1::__libcpp_compressed_pair_imp.8"** %16, align 8
  %67 = load %"class.std::__1::__libcpp_compressed_pair_imp.8"*, %"class.std::__1::__libcpp_compressed_pair_imp.8"** %16, align 8
  %68 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.8"* %67 to %"class.std::__1::allocator.9"*
  %69 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %20, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %62, %"class.std::__1::__bucket_list_deallocator"** %8, align 8
  %70 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %8, align 8
  %71 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %70, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.7"* %71, %"class.std::__1::__compressed_pair.7"** %7, align 8
  %72 = load %"class.std::__1::__compressed_pair.7"*, %"class.std::__1::__compressed_pair.7"** %7, align 8
  %73 = bitcast %"class.std::__1::__compressed_pair.7"* %72 to %"class.std::__1::__libcpp_compressed_pair_imp.8"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.8"* %73, %"class.std::__1::__libcpp_compressed_pair_imp.8"** %6, align 8
  %74 = load %"class.std::__1::__libcpp_compressed_pair_imp.8"*, %"class.std::__1::__libcpp_compressed_pair_imp.8"** %6, align 8
  %75 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.8", %"class.std::__1::__libcpp_compressed_pair_imp.8"* %74, i32 0, i32 0
  %76 = load i64, i64* %75, align 8
  store %"class.std::__1::allocator.9"* %68, %"class.std::__1::allocator.9"** %13, align 8
  store %"struct.std::__1::__hash_node_base"** %69, %"struct.std::__1::__hash_node_base"*** %14, align 8
  store i64 %76, i64* %15, align 8
  %77 = load %"class.std::__1::allocator.9"*, %"class.std::__1::allocator.9"** %13, align 8
  %78 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %14, align 8
  %79 = load i64, i64* %15, align 8
  store %"class.std::__1::allocator.9"* %77, %"class.std::__1::allocator.9"** %10, align 8
  store %"struct.std::__1::__hash_node_base"** %78, %"struct.std::__1::__hash_node_base"*** %11, align 8
  store i64 %79, i64* %12, align 8
  %80 = load %"class.std::__1::allocator.9"*, %"class.std::__1::allocator.9"** %10, align 8
  %81 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %11, align 8
  %82 = bitcast %"struct.std::__1::__hash_node_base"** %81 to i8*
  store i8* %82, i8** %9, align 8
  %83 = load i8*, i8** %9, align 8
  call void @_ZdlPv(i8* %83) #8
  br label %84

; <label>:84:                                     ; preds = %1, %55
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEE12__deallocateEPNS_16__hash_node_baseIPNS_11__hash_nodeIS9_PvEEEE(%"class.std::__1::__hash_table"*, %"struct.std::__1::__hash_node_base"*) #2 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::allocator.13"*, align 8
  %4 = alloca %"struct.std::__1::pair.329"*, align 8
  %5 = alloca %"class.std::__1::allocator.13"*, align 8
  %6 = alloca %"struct.std::__1::pair.329"*, align 8
  %7 = alloca %"struct.std::__1::integral_constant.330", align 1
  %8 = alloca %"struct.std::__1::__has_destroy.331", align 1
  %9 = alloca i8*, align 8
  %10 = alloca %"class.std::__1::allocator.13"*, align 8
  %11 = alloca %"struct.std::__1::__hash_node"*, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::allocator.13"*, align 8
  %14 = alloca %"struct.std::__1::__hash_node"*, align 8
  %15 = alloca i64, align 8
  %16 = alloca %"struct.std::__1::pair.329"*, align 8
  %17 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %18 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %19 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %20 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.12"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.11"*, align 8
  %23 = alloca %"class.std::__1::__hash_table"*, align 8
  %24 = alloca %"class.std::__1::__hash_table"*, align 8
  %25 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %26 = alloca %"class.std::__1::allocator.13"*, align 8
  %27 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %28 = alloca %"struct.std::__1::__hash_node"*, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %24, align 8
  store %"struct.std::__1::__hash_node_base"* %1, %"struct.std::__1::__hash_node_base"** %25, align 8
  %29 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %24, align 8
  store %"class.std::__1::__hash_table"* %29, %"class.std::__1::__hash_table"** %23, align 8
  %30 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %23, align 8
  %31 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %30, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.11"* %31, %"class.std::__1::__compressed_pair.11"** %22, align 8
  %32 = load %"class.std::__1::__compressed_pair.11"*, %"class.std::__1::__compressed_pair.11"** %22, align 8
  %33 = bitcast %"class.std::__1::__compressed_pair.11"* %32 to %"class.std::__1::__libcpp_compressed_pair_imp.12"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.12"* %33, %"class.std::__1::__libcpp_compressed_pair_imp.12"** %21, align 8
  %34 = load %"class.std::__1::__libcpp_compressed_pair_imp.12"*, %"class.std::__1::__libcpp_compressed_pair_imp.12"** %21, align 8
  %35 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.12"* %34 to %"class.std::__1::allocator.13"*
  store %"class.std::__1::allocator.13"* %35, %"class.std::__1::allocator.13"** %26, align 8
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
  %49 = load %"class.std::__1::allocator.13"*, %"class.std::__1::allocator.13"** %26, align 8
  %50 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %28, align 8
  %51 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %50, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %51, %"union.std::__1::__hash_value_type"** %17, align 8
  %52 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %17, align 8
  %53 = bitcast %"union.std::__1::__hash_value_type"* %52 to %"struct.std::__1::pair.329"*
  store %"struct.std::__1::pair.329"* %53, %"struct.std::__1::pair.329"** %16, align 8
  %54 = load %"struct.std::__1::pair.329"*, %"struct.std::__1::pair.329"** %16, align 8
  %55 = bitcast %"struct.std::__1::pair.329"* %54 to i8*
  br label %56

; <label>:56:                                     ; preds = %39
  store %"class.std::__1::allocator.13"* %49, %"class.std::__1::allocator.13"** %5, align 8
  store %"struct.std::__1::pair.329"* %54, %"struct.std::__1::pair.329"** %6, align 8
  %57 = bitcast %"struct.std::__1::__has_destroy.331"* %8 to %"struct.std::__1::integral_constant.330"*
  %58 = load %"class.std::__1::allocator.13"*, %"class.std::__1::allocator.13"** %5, align 8
  %59 = load %"struct.std::__1::pair.329"*, %"struct.std::__1::pair.329"** %6, align 8
  store %"class.std::__1::allocator.13"* %58, %"class.std::__1::allocator.13"** %3, align 8
  store %"struct.std::__1::pair.329"* %59, %"struct.std::__1::pair.329"** %4, align 8
  %60 = load %"struct.std::__1::pair.329"*, %"struct.std::__1::pair.329"** %4, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED1Ev(%"struct.std::__1::pair.329"* %60) #7
  br label %61

; <label>:61:                                     ; preds = %56
  %62 = load %"class.std::__1::allocator.13"*, %"class.std::__1::allocator.13"** %26, align 8
  %63 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %28, align 8
  store %"class.std::__1::allocator.13"* %62, %"class.std::__1::allocator.13"** %13, align 8
  store %"struct.std::__1::__hash_node"* %63, %"struct.std::__1::__hash_node"** %14, align 8
  store i64 1, i64* %15, align 8
  %64 = load %"class.std::__1::allocator.13"*, %"class.std::__1::allocator.13"** %13, align 8
  %65 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %14, align 8
  %66 = load i64, i64* %15, align 8
  store %"class.std::__1::allocator.13"* %64, %"class.std::__1::allocator.13"** %10, align 8
  store %"struct.std::__1::__hash_node"* %65, %"struct.std::__1::__hash_node"** %11, align 8
  store i64 %66, i64* %12, align 8
  %67 = load %"class.std::__1::allocator.13"*, %"class.std::__1::allocator.13"** %10, align 8
  %68 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %11, align 8
  %69 = bitcast %"struct.std::__1::__hash_node"* %68 to i8*
  store i8* %69, i8** %9, align 8
  %70 = load i8*, i8** %9, align 8
  call void @_ZdlPv(i8* %70) #8
  %71 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %27, align 8
  store %"struct.std::__1::__hash_node_base"* %71, %"struct.std::__1::__hash_node_base"** %25, align 8
  br label %36

; <label>:72:                                     ; preds = %36
  ret void
                                                  ; No predecessors!
  %74 = landingpad { i8*, i32 }
          catch i8* null
  %75 = extractvalue { i8*, i32 } %74, 0
  call void @__clang_call_terminate(i8* %75) #9
  unreachable
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED1Ev(%"struct.std::__1::pair.329"*) unnamed_addr #1 align 2 {
  %2 = alloca %"struct.std::__1::pair.329"*, align 8
  store %"struct.std::__1::pair.329"* %0, %"struct.std::__1::pair.329"** %2, align 8
  %3 = load %"struct.std::__1::pair.329"*, %"struct.std::__1::pair.329"** %2, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED2Ev(%"struct.std::__1::pair.329"* %3) #7
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED2Ev(%"struct.std::__1::pair.329"*) unnamed_addr #1 align 2 {
  %2 = alloca %"struct.std::__1::pair.329"*, align 8
  store %"struct.std::__1::pair.329"* %0, %"struct.std::__1::pair.329"** %2, align 8
  %3 = load %"struct.std::__1::pair.329"*, %"struct.std::__1::pair.329"** %2, align 8
  %4 = getelementptr inbounds %"struct.std::__1::pair.329", %"struct.std::__1::pair.329"* %3, i32 0, i32 1
  call void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier* %4) #7
  %5 = getelementptr inbounds %"struct.std::__1::pair.329", %"struct.std::__1::pair.329"* %3, i32 0, i32 0
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %5) #7
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__110shared_ptrI10ExpressionED2Ev(%"class.std::__1::shared_ptr"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::shared_ptr"*, align 8
  store %"class.std::__1::shared_ptr"* %0, %"class.std::__1::shared_ptr"** %2, align 8
  %3 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %2, align 8
  %4 = getelementptr inbounds %"class.std::__1::shared_ptr", %"class.std::__1::shared_ptr"* %3, i32 0, i32 1
  %5 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %4, align 8
  %6 = icmp ne %"class.std::__1::__shared_weak_count"* %5, null
  br i1 %6, label %7, label %10

; <label>:7:                                      ; preds = %1
  %8 = getelementptr inbounds %"class.std::__1::shared_ptr", %"class.std::__1::shared_ptr"* %3, i32 0, i32 1
  %9 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %8, align 8
  call void @_ZNSt3__119__shared_weak_count16__release_sharedEv(%"class.std::__1::__shared_weak_count"* %9) #7
  br label %10

; <label>:10:                                     ; preds = %7, %1
  ret void
}

; Function Attrs: nounwind
declare void @_ZNSt3__119__shared_weak_count16__release_sharedEv(%"class.std::__1::__shared_weak_count"*) #4

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED2Ev(%"class.std::__1::vector.56"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::vector.56"*, align 8
  store %"class.std::__1::vector.56"* %0, %"class.std::__1::vector.56"** %2, align 8
  %3 = load %"class.std::__1::vector.56"*, %"class.std::__1::vector.56"** %2, align 8
  %4 = bitcast %"class.std::__1::vector.56"* %3 to %"class.std::__1::__vector_base.57"*
  call void @_ZNSt3__113__vector_baseINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED2Ev(%"class.std::__1::__vector_base.57"* %4) #7
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113__vector_baseINS_10shared_ptrI7AstNodeEENS_9allocatorIS3_EEED2Ev(%"class.std::__1::__vector_base.57"*) unnamed_addr #2 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.60"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.59"*, align 8
  %4 = alloca %"class.std::__1::__vector_base.57"*, align 8
  %5 = alloca %"class.std::__1::__vector_base.57"*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %"class.std::__1::allocator.61"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %9 = alloca i64, align 8
  %10 = alloca %"class.std::__1::allocator.61"*, align 8
  %11 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.60"*, align 8
  %14 = alloca %"class.std::__1::__compressed_pair.59"*, align 8
  %15 = alloca %"class.std::__1::__vector_base.57"*, align 8
  %16 = alloca %"class.std::__1::allocator.61"*, align 8
  %17 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %18 = alloca %"struct.std::__1::integral_constant", align 1
  %19 = alloca %"class.std::__1::allocator.61"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %21 = alloca %"class.std::__1::allocator.61"*, align 8
  %22 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %23 = alloca %"struct.std::__1::integral_constant", align 1
  %24 = alloca %"struct.std::__1::__has_destroy.332", align 1
  %25 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.60"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.59"*, align 8
  %28 = alloca %"class.std::__1::__vector_base.57"*, align 8
  %29 = alloca %"class.std::__1::__vector_base.57"*, align 8
  %30 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  %31 = alloca %"class.std::__1::__vector_base.57"*, align 8
  %32 = alloca %"class.std::__1::__vector_base.57"*, align 8
  store %"class.std::__1::__vector_base.57"* %0, %"class.std::__1::__vector_base.57"** %32, align 8
  %33 = load %"class.std::__1::__vector_base.57"*, %"class.std::__1::__vector_base.57"** %32, align 8
  %34 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %33, i32 0, i32 0
  %35 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %34, align 8
  %36 = icmp ne %"class.std::__1::shared_ptr.58"* %35, null
  br i1 %36, label %37, label %95

; <label>:37:                                     ; preds = %1
  store %"class.std::__1::__vector_base.57"* %33, %"class.std::__1::__vector_base.57"** %31, align 8
  %38 = load %"class.std::__1::__vector_base.57"*, %"class.std::__1::__vector_base.57"** %31, align 8
  %39 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %38, i32 0, i32 0
  %40 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %39, align 8
  store %"class.std::__1::__vector_base.57"* %38, %"class.std::__1::__vector_base.57"** %29, align 8
  store %"class.std::__1::shared_ptr.58"* %40, %"class.std::__1::shared_ptr.58"** %30, align 8
  %41 = load %"class.std::__1::__vector_base.57"*, %"class.std::__1::__vector_base.57"** %29, align 8
  br label %42

; <label>:42:                                     ; preds = %47, %37
  %43 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %30, align 8
  %44 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %41, i32 0, i32 1
  %45 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %44, align 8
  %46 = icmp ne %"class.std::__1::shared_ptr.58"* %43, %45
  br i1 %46, label %47, label %65

; <label>:47:                                     ; preds = %42
  store %"class.std::__1::__vector_base.57"* %41, %"class.std::__1::__vector_base.57"** %28, align 8
  %48 = load %"class.std::__1::__vector_base.57"*, %"class.std::__1::__vector_base.57"** %28, align 8
  %49 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %48, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.59"* %49, %"class.std::__1::__compressed_pair.59"** %27, align 8
  %50 = load %"class.std::__1::__compressed_pair.59"*, %"class.std::__1::__compressed_pair.59"** %27, align 8
  %51 = bitcast %"class.std::__1::__compressed_pair.59"* %50 to %"class.std::__1::__libcpp_compressed_pair_imp.60"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.60"* %51, %"class.std::__1::__libcpp_compressed_pair_imp.60"** %26, align 8
  %52 = load %"class.std::__1::__libcpp_compressed_pair_imp.60"*, %"class.std::__1::__libcpp_compressed_pair_imp.60"** %26, align 8
  %53 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.60"* %52 to %"class.std::__1::allocator.61"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %41, i32 0, i32 1
  %55 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %54, align 8
  %56 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %55, i32 -1
  store %"class.std::__1::shared_ptr.58"* %56, %"class.std::__1::shared_ptr.58"** %54, align 8
  store %"class.std::__1::shared_ptr.58"* %56, %"class.std::__1::shared_ptr.58"** %25, align 8
  %57 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %25, align 8
  store %"class.std::__1::allocator.61"* %53, %"class.std::__1::allocator.61"** %21, align 8
  store %"class.std::__1::shared_ptr.58"* %57, %"class.std::__1::shared_ptr.58"** %22, align 8
  %58 = bitcast %"struct.std::__1::__has_destroy.332"* %24 to %"struct.std::__1::integral_constant"*
  %59 = load %"class.std::__1::allocator.61"*, %"class.std::__1::allocator.61"** %21, align 8
  %60 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %22, align 8
  store %"class.std::__1::allocator.61"* %59, %"class.std::__1::allocator.61"** %19, align 8
  store %"class.std::__1::shared_ptr.58"* %60, %"class.std::__1::shared_ptr.58"** %20, align 8
  %61 = load %"class.std::__1::allocator.61"*, %"class.std::__1::allocator.61"** %19, align 8
  %62 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %20, align 8
  store %"class.std::__1::allocator.61"* %61, %"class.std::__1::allocator.61"** %16, align 8
  store %"class.std::__1::shared_ptr.58"* %62, %"class.std::__1::shared_ptr.58"** %17, align 8
  %63 = load %"class.std::__1::allocator.61"*, %"class.std::__1::allocator.61"** %16, align 8
  %64 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %17, align 8
  call void @_ZNSt3__110shared_ptrI7AstNodeED1Ev(%"class.std::__1::shared_ptr.58"* %64) #7
  br label %42

; <label>:65:                                     ; preds = %42
  store %"class.std::__1::__vector_base.57"* %33, %"class.std::__1::__vector_base.57"** %15, align 8
  %66 = load %"class.std::__1::__vector_base.57"*, %"class.std::__1::__vector_base.57"** %15, align 8
  %67 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %66, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.59"* %67, %"class.std::__1::__compressed_pair.59"** %14, align 8
  %68 = load %"class.std::__1::__compressed_pair.59"*, %"class.std::__1::__compressed_pair.59"** %14, align 8
  %69 = bitcast %"class.std::__1::__compressed_pair.59"* %68 to %"class.std::__1::__libcpp_compressed_pair_imp.60"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.60"* %69, %"class.std::__1::__libcpp_compressed_pair_imp.60"** %13, align 8
  %70 = load %"class.std::__1::__libcpp_compressed_pair_imp.60"*, %"class.std::__1::__libcpp_compressed_pair_imp.60"** %13, align 8
  %71 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.60"* %70 to %"class.std::__1::allocator.61"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %33, i32 0, i32 0
  %73 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %72, align 8
  store %"class.std::__1::__vector_base.57"* %33, %"class.std::__1::__vector_base.57"** %5, align 8
  %74 = load %"class.std::__1::__vector_base.57"*, %"class.std::__1::__vector_base.57"** %5, align 8
  store %"class.std::__1::__vector_base.57"* %74, %"class.std::__1::__vector_base.57"** %4, align 8
  %75 = load %"class.std::__1::__vector_base.57"*, %"class.std::__1::__vector_base.57"** %4, align 8
  %76 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %75, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.59"* %76, %"class.std::__1::__compressed_pair.59"** %3, align 8
  %77 = load %"class.std::__1::__compressed_pair.59"*, %"class.std::__1::__compressed_pair.59"** %3, align 8
  %78 = bitcast %"class.std::__1::__compressed_pair.59"* %77 to %"class.std::__1::__libcpp_compressed_pair_imp.60"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.60"* %78, %"class.std::__1::__libcpp_compressed_pair_imp.60"** %2, align 8
  %79 = load %"class.std::__1::__libcpp_compressed_pair_imp.60"*, %"class.std::__1::__libcpp_compressed_pair_imp.60"** %2, align 8
  %80 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.60", %"class.std::__1::__libcpp_compressed_pair_imp.60"* %79, i32 0, i32 0
  %81 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %80, align 8
  %82 = getelementptr inbounds %"class.std::__1::__vector_base.57", %"class.std::__1::__vector_base.57"* %74, i32 0, i32 0
  %83 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %82, align 8
  %84 = ptrtoint %"class.std::__1::shared_ptr.58"* %81 to i64
  %85 = ptrtoint %"class.std::__1::shared_ptr.58"* %83 to i64
  %86 = sub i64 %84, %85
  %87 = sdiv exact i64 %86, 16
  store %"class.std::__1::allocator.61"* %71, %"class.std::__1::allocator.61"** %10, align 8
  store %"class.std::__1::shared_ptr.58"* %73, %"class.std::__1::shared_ptr.58"** %11, align 8
  store i64 %87, i64* %12, align 8
  %88 = load %"class.std::__1::allocator.61"*, %"class.std::__1::allocator.61"** %10, align 8
  %89 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %11, align 8
  %90 = load i64, i64* %12, align 8
  store %"class.std::__1::allocator.61"* %88, %"class.std::__1::allocator.61"** %7, align 8
  store %"class.std::__1::shared_ptr.58"* %89, %"class.std::__1::shared_ptr.58"** %8, align 8
  store i64 %90, i64* %9, align 8
  %91 = load %"class.std::__1::allocator.61"*, %"class.std::__1::allocator.61"** %7, align 8
  %92 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %8, align 8
  %93 = bitcast %"class.std::__1::shared_ptr.58"* %92 to i8*
  store i8* %93, i8** %6, align 8
  %94 = load i8*, i8** %6, align 8
  call void @_ZdlPv(i8* %94) #8
  br label %95

; <label>:95:                                     ; preds = %65, %1
  ret void
}

; Function Attrs: nounwind
declare void @_ZNSt3__119__shared_weak_count12__add_sharedEv(%"class.std::__1::__shared_weak_count"*) #4

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__110shared_ptrI7AstNodeED2Ev(%"class.std::__1::shared_ptr.58"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::shared_ptr.58"*, align 8
  store %"class.std::__1::shared_ptr.58"* %0, %"class.std::__1::shared_ptr.58"** %2, align 8
  %3 = load %"class.std::__1::shared_ptr.58"*, %"class.std::__1::shared_ptr.58"** %2, align 8
  %4 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %3, i32 0, i32 1
  %5 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %4, align 8
  %6 = icmp ne %"class.std::__1::__shared_weak_count"* %5, null
  br i1 %6, label %7, label %10

; <label>:7:                                      ; preds = %1
  %8 = getelementptr inbounds %"class.std::__1::shared_ptr.58", %"class.std::__1::shared_ptr.58"* %3, i32 0, i32 1
  %9 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %8, align 8
  call void @_ZNSt3__119__shared_weak_count16__release_sharedEv(%"class.std::__1::__shared_weak_count"* %9) #7
  br label %10

; <label>:10:                                     ; preds = %7, %1
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__110shared_ptrI11FuncArgDeclED2Ev(%"class.std::__1::shared_ptr.66"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::shared_ptr.66"*, align 8
  store %"class.std::__1::shared_ptr.66"* %0, %"class.std::__1::shared_ptr.66"** %2, align 8
  %3 = load %"class.std::__1::shared_ptr.66"*, %"class.std::__1::shared_ptr.66"** %2, align 8
  %4 = getelementptr inbounds %"class.std::__1::shared_ptr.66", %"class.std::__1::shared_ptr.66"* %3, i32 0, i32 1
  %5 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %4, align 8
  %6 = icmp ne %"class.std::__1::__shared_weak_count"* %5, null
  br i1 %6, label %7, label %10

; <label>:7:                                      ; preds = %1
  %8 = getelementptr inbounds %"class.std::__1::shared_ptr.66", %"class.std::__1::shared_ptr.66"* %3, i32 0, i32 1
  %9 = load %"class.std::__1::__shared_weak_count"*, %"class.std::__1::__shared_weak_count"** %8, align 8
  call void @_ZNSt3__119__shared_weak_count16__release_sharedEv(%"class.std::__1::__shared_weak_count"* %9) #7
  br label %10

; <label>:10:                                     ; preds = %7, %1
  ret void
}

attributes #0 = { ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { inlinehint nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { noinline noreturn nounwind }
attributes #6 = { nobuiltin nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { nounwind }
attributes #8 = { builtin nounwind }
attributes #9 = { noreturn nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"Apple LLVM version 8.1.0 (clang-802.0.42)"}
