; ModuleID = '/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/Lexer.cpp'
source_filename = "/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/Lexer.cpp"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.12.0"

%"class.std::__1::basic_string" = type { %"class.std::__1::__compressed_pair" }
%"class.std::__1::__compressed_pair" = type { %"class.std::__1::__libcpp_compressed_pair_imp" }
%"class.std::__1::__libcpp_compressed_pair_imp" = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" = type { %union.anon.0 }
%union.anon.0 = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" = type { i64, i64, i8* }
%"class.std::__1::vector" = type { %"class.std::__1::__vector_base" }
%"class.std::__1::__vector_base" = type { %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"*, %"class.std::__1::__compressed_pair.16" }
%"class.std::__1::__compressed_pair.16" = type { %"class.std::__1::__libcpp_compressed_pair_imp.17" }
%"class.std::__1::__libcpp_compressed_pair_imp.17" = type { %"class.std::__1::basic_string"* }
%"class.std::__1::vector.47" = type { %"class.std::__1::__vector_base.48" }
%"class.std::__1::__vector_base.48" = type { i8*, i8*, %"class.std::__1::__compressed_pair.49" }
%"class.std::__1::__compressed_pair.49" = type { %"class.std::__1::__libcpp_compressed_pair_imp.50" }
%"class.std::__1::__libcpp_compressed_pair_imp.50" = type { i8* }
%"class.std::__1::unordered_map.51" = type { %"class.std::__1::__hash_table.52" }
%"class.std::__1::__hash_table.52" = type <{ %"class.std::__1::unique_ptr.53", %"class.std::__1::__compressed_pair.62", %"class.std::__1::__compressed_pair.66", %"class.std::__1::__compressed_pair.69", [4 x i8] }>
%"class.std::__1::unique_ptr.53" = type { %"class.std::__1::__compressed_pair.54" }
%"class.std::__1::__compressed_pair.54" = type { %"class.std::__1::__libcpp_compressed_pair_imp.55" }
%"class.std::__1::__libcpp_compressed_pair_imp.55" = type { %"struct.std::__1::__hash_node_base.56"**, %"class.std::__1::__bucket_list_deallocator.57" }
%"struct.std::__1::__hash_node_base.56" = type { %"struct.std::__1::__hash_node_base.56"* }
%"class.std::__1::__bucket_list_deallocator.57" = type { %"class.std::__1::__compressed_pair.58" }
%"class.std::__1::__compressed_pair.58" = type { %"class.std::__1::__libcpp_compressed_pair_imp.59" }
%"class.std::__1::__libcpp_compressed_pair_imp.59" = type { i64 }
%"class.std::__1::__compressed_pair.62" = type { %"class.std::__1::__libcpp_compressed_pair_imp.63" }
%"class.std::__1::__libcpp_compressed_pair_imp.63" = type { %"struct.std::__1::__hash_node_base.56" }
%"class.std::__1::__compressed_pair.66" = type { %"class.std::__1::__libcpp_compressed_pair_imp.67" }
%"class.std::__1::__libcpp_compressed_pair_imp.67" = type { i64 }
%"class.std::__1::__compressed_pair.69" = type { %"class.std::__1::__libcpp_compressed_pair_imp.70" }
%"class.std::__1::__libcpp_compressed_pair_imp.70" = type { float }
%class.Lexer = type { i32, i32, i32, i32, i32, [4 x i8], %class.Token, %"class.std::__1::vector.27", %"class.std::__1::basic_string" }
%class.Token = type { %struct.Variant, i32, i32, i32, i32, i32, i32 }
%struct.Variant = type <{ %union.anon, i8, [7 x i8], %"class.std::__1::basic_string", %struct.TypeSpecifier, i8, i8, i8, [5 x i8] }>
%union.anon = type { i64 }
%struct.TypeSpecifier = type { i32, %"class.std::__1::basic_string", %"class.std::__1::shared_ptr", i8, i8, i8, i8, i64, i32, i8, i8, %"class.std::__1::basic_string", i8, [7 x i8], %"class.std::__1::unordered_map", i8, i8, i8, i8, i8, i8, i8, i32, %"class.std::__1::vector", %"class.std::__1::vector.20", %union.anon.26 }
%"class.std::__1::shared_ptr" = type { %class.Expression*, %"class.std::__1::__shared_weak_count"* }
%class.Expression = type { %class.Statement.base, %"class.std::__1::shared_ptr", i8, %"class.std::__1::shared_ptr", i8, %"class.std::__1::unique_ptr.33", i8, %struct.TypeSpecifier }
%class.Statement.base = type { %class.AstNode.base }
%class.AstNode.base = type <{ i32 (...)**, i32, i32, i64, %class.AstNode*, %class.DeclStmt*, i8, [3 x i8], i32, %"class.std::__1::vector", %struct.TypeSpecifier, %struct.TypeSpecifier, i8, [7 x i8], %"class.std::__1::basic_string", i8, i8 }>
%class.AstNode = type <{ i32 (...)**, i32, i32, i64, %class.AstNode*, %class.DeclStmt*, i8, [3 x i8], i32, %"class.std::__1::vector", %struct.TypeSpecifier, %struct.TypeSpecifier, i8, [7 x i8], %"class.std::__1::basic_string", i8, i8, [6 x i8] }>
%class.DeclStmt = type <{ %class.Statement.base, [6 x i8], %"class.std::__1::vector", %"class.std::__1::vector.36", %"class.std::__1::vector", %"class.std::__1::vector.42", i8, i8, [6 x i8] }>
%"class.std::__1::vector.36" = type { %"class.std::__1::__vector_base.37" }
%"class.std::__1::__vector_base.37" = type { %"struct.std::__1::pair"*, %"struct.std::__1::pair"*, %"class.std::__1::__compressed_pair.38" }
%"struct.std::__1::pair" = type opaque
%"class.std::__1::__compressed_pair.38" = type { %"class.std::__1::__libcpp_compressed_pair_imp.39" }
%"class.std::__1::__libcpp_compressed_pair_imp.39" = type { %"struct.std::__1::pair"* }
%"class.std::__1::vector.42" = type { i64*, i64, %"class.std::__1::__compressed_pair.43" }
%"class.std::__1::__compressed_pair.43" = type { %"class.std::__1::__libcpp_compressed_pair_imp.44" }
%"class.std::__1::__libcpp_compressed_pair_imp.44" = type { i64 }
%"class.std::__1::unique_ptr.33" = type { %"class.std::__1::__compressed_pair.34" }
%"class.std::__1::__compressed_pair.34" = type { %"class.std::__1::__libcpp_compressed_pair_imp.35" }
%"class.std::__1::__libcpp_compressed_pair_imp.35" = type { %struct.Variant* }
%"class.std::__1::__shared_weak_count" = type { %"class.std::__1::__shared_count", i64 }
%"class.std::__1::__shared_count" = type { i32 (...)**, i64 }
%"class.std::__1::unordered_map" = type { %"class.std::__1::__hash_table" }
%"class.std::__1::__hash_table" = type <{ %"class.std::__1::unique_ptr", %"class.std::__1::__compressed_pair.8", %"class.std::__1::__compressed_pair.12", %"class.std::__1::__compressed_pair.14", [4 x i8] }>
%"class.std::__1::unique_ptr" = type { %"class.std::__1::__compressed_pair.2" }
%"class.std::__1::__compressed_pair.2" = type { %"class.std::__1::__libcpp_compressed_pair_imp.3" }
%"class.std::__1::__libcpp_compressed_pair_imp.3" = type { %"struct.std::__1::__hash_node_base"**, %"class.std::__1::__bucket_list_deallocator" }
%"struct.std::__1::__hash_node_base" = type { %"struct.std::__1::__hash_node_base"* }
%"class.std::__1::__bucket_list_deallocator" = type { %"class.std::__1::__compressed_pair.4" }
%"class.std::__1::__compressed_pair.4" = type { %"class.std::__1::__libcpp_compressed_pair_imp.5" }
%"class.std::__1::__libcpp_compressed_pair_imp.5" = type { i64 }
%"class.std::__1::__compressed_pair.8" = type { %"class.std::__1::__libcpp_compressed_pair_imp.9" }
%"class.std::__1::__libcpp_compressed_pair_imp.9" = type { %"struct.std::__1::__hash_node_base" }
%"class.std::__1::__compressed_pair.12" = type { %"class.std::__1::__libcpp_compressed_pair_imp.13" }
%"class.std::__1::__libcpp_compressed_pair_imp.13" = type { i64 }
%"class.std::__1::__compressed_pair.14" = type { %"class.std::__1::__libcpp_compressed_pair_imp.15" }
%"class.std::__1::__libcpp_compressed_pair_imp.15" = type { float }
%"class.std::__1::vector.20" = type { %"class.std::__1::__vector_base.21" }
%"class.std::__1::__vector_base.21" = type { %struct.TypeSpecifier*, %struct.TypeSpecifier*, %"class.std::__1::__compressed_pair.22" }
%"class.std::__1::__compressed_pair.22" = type { %"class.std::__1::__libcpp_compressed_pair_imp.23" }
%"class.std::__1::__libcpp_compressed_pair_imp.23" = type { %struct.TypeSpecifier* }
%union.anon.26 = type { %struct.TypeSpecifier* }
%"class.std::__1::vector.27" = type { %"class.std::__1::__vector_base.28" }
%"class.std::__1::__vector_base.28" = type { %class.Token*, %class.Token*, %"class.std::__1::__compressed_pair.29" }
%"class.std::__1::__compressed_pair.29" = type { %"class.std::__1::__libcpp_compressed_pair_imp.30" }
%"class.std::__1::__libcpp_compressed_pair_imp.30" = type { %class.Token* }
%"class.std::__1::allocator.31" = type { i8 }
%"struct.std::__1::integral_constant" = type { i8 }
%"struct.std::__1::__has_construct" = type { i8 }
%"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator" = type { i8 }
%"class.std::__1::__vector_base_common" = type { i8 }
%"class.std::__1::allocator" = type { i8 }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short" = type { %union.anon.1, [23 x i8] }
%union.anon.1 = type { i8 }
%"class.std::__1::__basic_string_common" = type { i8 }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw" = type { [3 x i64] }
%"struct.std::__1::pair.73" = type <{ %"class.std::__1::basic_string", i32, [4 x i8] }>
%"union.std::__1::__hash_value_type.82" = type { %"struct.std::__1::pair.73" }
%"class.std::__1::__hash_iterator" = type { %"struct.std::__1::__hash_node_base.56"* }
%"class.std::__1::__hash_map_iterator" = type { %"class.std::__1::__hash_iterator" }
%"struct.std::__1::__hash_node.85" = type { %"struct.std::__1::__hash_node_base.56", i64, %"union.std::__1::__hash_value_type.82" }
%"class.std::__1::allocator.24" = type { i8 }
%"struct.std::__1::__has_destroy" = type { i8 }
%"class.std::__1::allocator.18" = type { i8 }
%"struct.std::__1::__has_destroy.74" = type { i8 }
%"class.std::__1::allocator.6" = type { i8 }
%"class.std::__1::allocator.10" = type { i8 }
%"struct.std::__1::pair.75" = type { %"class.std::__1::basic_string", %struct.TypeSpecifier }
%"struct.std::__1::integral_constant.77" = type { i8 }
%"struct.std::__1::__has_destroy.78" = type { i8 }
%"struct.std::__1::__hash_node" = type { %"struct.std::__1::__hash_node_base", i64, %"union.std::__1::__hash_value_type" }
%"union.std::__1::__hash_value_type" = type { %"struct.std::__1::pair.75" }
%"struct.std::__1::__has_destroy.79" = type { i8 }
%"struct.std::__1::__less" = type { i8 }
%"struct.std::__1::__split_buffer" = type { %class.Token*, %class.Token*, %class.Token*, %"class.std::__1::__compressed_pair.80" }
%"class.std::__1::__compressed_pair.80" = type { %"class.std::__1::__libcpp_compressed_pair_imp.81" }
%"class.std::__1::__libcpp_compressed_pair_imp.81" = type { %class.Token*, %"class.std::__1::allocator.31"* }
%"struct.std::__1::__has_max_size" = type { i8 }
%"class.std::__1::__split_buffer_common" = type { i8 }

@_ZN4llvm24DisableABIBreakingChecksE = external global i32, align 4
@_ZN4llvm30VerifyDisableABIBreakingChecksE = weak hidden global i32* @_ZN4llvm24DisableABIBreakingChecksE, align 8
@.str = private unnamed_addr constant [7 x i8] c"String\00", align 1
@.str.1 = private unnamed_addr constant [18 x i8] c"Unexpected token \00", align 1
@_ZN4util11token_namesE = external global [0 x %"class.std::__1::basic_string"], align 8
@_ZN4util16binary_operatorsE = external global %"class.std::__1::vector", align 8
@_ZN4util15unary_operatorsE = external global %"class.std::__1::vector", align 8
@_ZN4util18tertiary_operatorsE = external global %"class.std::__1::vector", align 8
@_ZN4util14operator_charsE = external global %"class.std::__1::vector.47", align 8
@_ZN4util8keywordsE = external global %"class.std::__1::vector", align 8
@_ZN4util7typemapE = external global %"class.std::__1::unordered_map.51", align 8
@_ZN4util11punctuatorsE = external global %"class.std::__1::vector.47", align 8
@.str.2 = private unnamed_addr constant [5 x i8] c"true\00", align 1
@.str.3 = private unnamed_addr constant [6 x i8] c"false\00", align 1
@.str.4 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@.str.5 = private unnamed_addr constant [37 x i8] c"Expected \22'\22 after character literal\00", align 1
@.str.6 = private unnamed_addr constant [3 x i8] c"0x\00", align 1
@.str.7 = private unnamed_addr constant [22 x i8] c"Invalid octal literal\00", align 1
@.str.8 = private unnamed_addr constant [7 x i8] c"typeof\00", align 1
@.str.9 = private unnamed_addr constant [4 x i8] c"new\00", align 1
@.str.10 = private unnamed_addr constant [5 x i8] c"null\00", align 1
@.str.11 = private unnamed_addr constant [2 x i8] c"_\00", align 1
@.str.12 = private unnamed_addr constant [29 x i8] c"'_' is a reserved identifier\00", align 1
@.str.13 = private unnamed_addr constant [2 x i8] c".\00", align 1
@.str.14 = private unnamed_addr constant [22 x i8] c"Unexpected character \00", align 1
@.str.15 = private unnamed_addr constant [10 x i8] c"Expected \00", align 1
@.str.16 = private unnamed_addr constant [10 x i8] c" but got \00", align 1

; Function Attrs: ssp uwtable
define void @_ZN5LexerC2ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Lexer*, %"class.std::__1::basic_string"*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::vector.27"*, align 8
  %4 = alloca %class.Token*, align 8
  %5 = alloca %"class.std::__1::allocator.31"*, align 8
  %6 = alloca %class.Token*, align 8
  %7 = alloca %class.Token*, align 8
  %8 = alloca %class.Token*, align 8
  %9 = alloca %"struct.std::__1::integral_constant", align 1
  %10 = alloca %"class.std::__1::allocator.31"*, align 8
  %11 = alloca %class.Token*, align 8
  %12 = alloca %class.Token*, align 8
  %13 = alloca %class.Token*, align 8
  %14 = alloca %"class.std::__1::allocator.31"*, align 8
  %15 = alloca %class.Token*, align 8
  %16 = alloca %class.Token*, align 8
  %17 = alloca %"struct.std::__1::integral_constant", align 1
  %18 = alloca %"struct.std::__1::__has_construct", align 1
  %19 = alloca %class.Token*, align 8
  %20 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %21 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %22 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %23 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %24 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %25 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %26 = alloca %"class.std::__1::vector.27"*, align 8
  %27 = alloca %class.Token*, align 8
  %28 = alloca %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator", align 1
  %29 = alloca %class.Token**, align 8
  %30 = alloca %"class.std::__1::allocator.31"*, align 8
  %31 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %32 = alloca %class.Token*, align 8
  %33 = alloca %class.Token**, align 8
  %34 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %35 = alloca %class.Token*, align 8
  %36 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %37 = alloca %class.Token*, align 8
  %38 = alloca %"class.std::__1::__vector_base_common"*, align 8
  %39 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %40 = alloca %"class.std::__1::vector.27"*, align 8
  %41 = alloca %"class.std::__1::vector.27"*, align 8
  %42 = alloca %class.Lexer*, align 8
  %43 = alloca %struct.Variant, align 8
  %44 = alloca i8*
  %45 = alloca i32
  %46 = alloca %class.Token, align 8
  store %class.Lexer* %0, %class.Lexer** %42, align 8
  %47 = load %class.Lexer*, %class.Lexer** %42, align 8
  %48 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 0
  store i32 0, i32* %48, align 8
  %49 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 1
  store i32 0, i32* %49, align 4
  %50 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 2
  store i32 0, i32* %50, align 8
  %51 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 4
  store i32 0, i32* %51, align 8
  %52 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 6
  call void @_ZN7VariantC1Ev(%struct.Variant* %43)
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %52, i32 6, %struct.Variant* %43, i32 0, i32 0, i32 0)
          to label %53 unwind label %141

; <label>:53:                                     ; preds = %2
  call void @_ZN7VariantD1Ev(%struct.Variant* %43) #11
  %54 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 7
  store %"class.std::__1::vector.27"* %54, %"class.std::__1::vector.27"** %41, align 8
  %55 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %41, align 8
  store %"class.std::__1::vector.27"* %55, %"class.std::__1::vector.27"** %40, align 8
  %56 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %40, align 8
  %57 = bitcast %"class.std::__1::vector.27"* %56 to %"class.std::__1::__vector_base.28"*
  store %"class.std::__1::__vector_base.28"* %57, %"class.std::__1::__vector_base.28"** %39, align 8
  %58 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %39, align 8
  %59 = bitcast %"class.std::__1::__vector_base.28"* %58 to %"class.std::__1::__vector_base_common"*
  store %"class.std::__1::__vector_base_common"* %59, %"class.std::__1::__vector_base_common"** %38, align 8
  %60 = load %"class.std::__1::__vector_base_common"*, %"class.std::__1::__vector_base_common"** %38, align 8
  %61 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %58, i32 0, i32 0
  store %class.Token* null, %class.Token** %61, align 8
  %62 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %58, i32 0, i32 1
  store %class.Token* null, %class.Token** %62, align 8
  %63 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %58, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %63, %"class.std::__1::__compressed_pair.29"** %36, align 8
  store %class.Token* null, %class.Token** %37, align 8
  %64 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %36, align 8
  %65 = load %class.Token*, %class.Token** %37, align 8
  store %"class.std::__1::__compressed_pair.29"* %64, %"class.std::__1::__compressed_pair.29"** %34, align 8
  store %class.Token* %65, %class.Token** %35, align 8
  %66 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %34, align 8
  %67 = bitcast %"class.std::__1::__compressed_pair.29"* %66 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %class.Token** %35, %class.Token*** %33, align 8
  %68 = load %class.Token**, %class.Token*** %33, align 8
  %69 = load %class.Token*, %class.Token** %68, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %67, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %31, align 8
  store %class.Token* %69, %class.Token** %32, align 8
  %70 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %31, align 8
  %71 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.30"* %70 to %"class.std::__1::allocator.31"*
  store %"class.std::__1::allocator.31"* %71, %"class.std::__1::allocator.31"** %30, align 8
  %72 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %30, align 8
  %73 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.30", %"class.std::__1::__libcpp_compressed_pair_imp.30"* %70, i32 0, i32 0
  store %class.Token** %32, %class.Token*** %29, align 8
  %74 = load %class.Token**, %class.Token*** %29, align 8
  %75 = load %class.Token*, %class.Token** %74, align 8
  store %class.Token* %75, %class.Token** %73, align 8
  %76 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 8
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %76, %"class.std::__1::basic_string"* dereferenceable(24) %1)
          to label %77 unwind label %145

; <label>:77:                                     ; preds = %53
  br label %78

; <label>:78:                                     ; preds = %140, %77
  %79 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 6
  %80 = invoke i32 @_ZN5Token8get_typeEv(%class.Token* %79)
          to label %81 unwind label %149

; <label>:81:                                     ; preds = %78
  %82 = icmp ne i32 %80, 7
  br i1 %82, label %83, label %157

; <label>:83:                                     ; preds = %81
  %84 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 6
  invoke void @_ZN5Lexer15_get_next_tokenEb(%class.Token* sret %46, %class.Lexer* %47, i1 zeroext false)
          to label %85 unwind label %149

; <label>:85:                                     ; preds = %83
  %86 = invoke dereferenceable(288) %class.Token* @_ZN5TokenaSEOS_(%class.Token* %84, %class.Token* dereferenceable(288) %46)
          to label %87 unwind label %153

; <label>:87:                                     ; preds = %85
  call void @_ZN5TokenD1Ev(%class.Token* %46) #11
  %88 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 7
  %89 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 6
  store %"class.std::__1::vector.27"* %88, %"class.std::__1::vector.27"** %26, align 8
  store %class.Token* %89, %class.Token** %27, align 8
  %90 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %26, align 8
  %91 = bitcast %"class.std::__1::vector.27"* %90 to %"class.std::__1::__vector_base.28"*
  %92 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %91, i32 0, i32 1
  %93 = load %class.Token*, %class.Token** %92, align 8
  %94 = bitcast %"class.std::__1::vector.27"* %90 to %"class.std::__1::__vector_base.28"*
  store %"class.std::__1::__vector_base.28"* %94, %"class.std::__1::__vector_base.28"** %25, align 8
  %95 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %25, align 8
  %96 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %95, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %96, %"class.std::__1::__compressed_pair.29"** %24, align 8
  %97 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %24, align 8
  %98 = bitcast %"class.std::__1::__compressed_pair.29"* %97 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %98, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %23, align 8
  %99 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %23, align 8
  %100 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.30", %"class.std::__1::__libcpp_compressed_pair_imp.30"* %99, i32 0, i32 0
  %101 = load %class.Token*, %class.Token** %100, align 8
  %102 = icmp ne %class.Token* %93, %101
  br i1 %102, label %103, label %136

; <label>:103:                                    ; preds = %87
  invoke void @_ZNSt3__16vectorI5TokenNS_9allocatorIS1_EEE24__RAII_IncreaseAnnotatorC1ERKS4_m(%"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"* %28, %"class.std::__1::vector.27"* dereferenceable(24) %90, i64 1)
          to label %104 unwind label %149

; <label>:104:                                    ; preds = %103
  %105 = bitcast %"class.std::__1::vector.27"* %90 to %"class.std::__1::__vector_base.28"*
  store %"class.std::__1::__vector_base.28"* %105, %"class.std::__1::__vector_base.28"** %22, align 8
  %106 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %22, align 8
  %107 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %106, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %107, %"class.std::__1::__compressed_pair.29"** %21, align 8
  %108 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %21, align 8
  %109 = bitcast %"class.std::__1::__compressed_pair.29"* %108 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %109, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %20, align 8
  %110 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %20, align 8
  %111 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.30"* %110 to %"class.std::__1::allocator.31"*
  %112 = bitcast %"class.std::__1::vector.27"* %90 to %"class.std::__1::__vector_base.28"*
  %113 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %112, i32 0, i32 1
  %114 = load %class.Token*, %class.Token** %113, align 8
  store %class.Token* %114, %class.Token** %19, align 8
  %115 = load %class.Token*, %class.Token** %19, align 8
  %116 = load %class.Token*, %class.Token** %27, align 8
  store %"class.std::__1::allocator.31"* %111, %"class.std::__1::allocator.31"** %14, align 8
  store %class.Token* %115, %class.Token** %15, align 8
  store %class.Token* %116, %class.Token** %16, align 8
  %117 = bitcast %"struct.std::__1::__has_construct"* %18 to %"struct.std::__1::integral_constant"*
  %118 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %14, align 8
  %119 = load %class.Token*, %class.Token** %15, align 8
  %120 = load %class.Token*, %class.Token** %16, align 8
  store %class.Token* %120, %class.Token** %13, align 8
  %121 = load %class.Token*, %class.Token** %13, align 8
  store %"class.std::__1::allocator.31"* %118, %"class.std::__1::allocator.31"** %10, align 8
  store %class.Token* %119, %class.Token** %11, align 8
  store %class.Token* %121, %class.Token** %12, align 8
  %122 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %10, align 8
  %123 = load %class.Token*, %class.Token** %11, align 8
  %124 = load %class.Token*, %class.Token** %12, align 8
  store %class.Token* %124, %class.Token** %8, align 8
  %125 = load %class.Token*, %class.Token** %8, align 8
  store %"class.std::__1::allocator.31"* %122, %"class.std::__1::allocator.31"** %5, align 8
  store %class.Token* %123, %class.Token** %6, align 8
  store %class.Token* %125, %class.Token** %7, align 8
  %126 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %5, align 8
  %127 = load %class.Token*, %class.Token** %6, align 8
  %128 = bitcast %class.Token* %127 to i8*
  %129 = load %class.Token*, %class.Token** %7, align 8
  store %class.Token* %129, %class.Token** %4, align 8
  %130 = load %class.Token*, %class.Token** %4, align 8
  invoke void @_ZN5TokenC1ERKS_(%class.Token* %127, %class.Token* dereferenceable(288) %130)
          to label %131 unwind label %149

; <label>:131:                                    ; preds = %104
  call void @_ZNSt3__16vectorI5TokenNS_9allocatorIS1_EEE24__RAII_IncreaseAnnotator6__doneEv(%"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"* %28)
  %132 = bitcast %"class.std::__1::vector.27"* %90 to %"class.std::__1::__vector_base.28"*
  %133 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %132, i32 0, i32 1
  %134 = load %class.Token*, %class.Token** %133, align 8
  %135 = getelementptr inbounds %class.Token, %class.Token* %134, i32 1
  store %class.Token* %135, %class.Token** %133, align 8
  br label %139

; <label>:136:                                    ; preds = %87
  %137 = load %class.Token*, %class.Token** %27, align 8
  invoke void @_ZNSt3__16vectorI5TokenNS_9allocatorIS1_EEE21__push_back_slow_pathIRKS1_EEvOT_(%"class.std::__1::vector.27"* %90, %class.Token* dereferenceable(288) %137)
          to label %138 unwind label %149

; <label>:138:                                    ; preds = %136
  br label %139

; <label>:139:                                    ; preds = %131, %138
  br label %140

; <label>:140:                                    ; preds = %139
  br label %78

; <label>:141:                                    ; preds = %2
  %142 = landingpad { i8*, i32 }
          cleanup
  %143 = extractvalue { i8*, i32 } %142, 0
  store i8* %143, i8** %44, align 8
  %144 = extractvalue { i8*, i32 } %142, 1
  store i32 %144, i32* %45, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %43) #11
  br label %175

; <label>:145:                                    ; preds = %53
  %146 = landingpad { i8*, i32 }
          cleanup
  %147 = extractvalue { i8*, i32 } %146, 0
  store i8* %147, i8** %44, align 8
  %148 = extractvalue { i8*, i32 } %146, 1
  store i32 %148, i32* %45, align 4
  br label %174

; <label>:149:                                    ; preds = %136, %104, %103, %167, %165, %83, %78
  %150 = landingpad { i8*, i32 }
          cleanup
  %151 = extractvalue { i8*, i32 } %150, 0
  store i8* %151, i8** %44, align 8
  %152 = extractvalue { i8*, i32 } %150, 1
  store i32 %152, i32* %45, align 4
  br label %173

; <label>:153:                                    ; preds = %85
  %154 = landingpad { i8*, i32 }
          cleanup
  %155 = extractvalue { i8*, i32 } %154, 0
  store i8* %155, i8** %44, align 8
  %156 = extractvalue { i8*, i32 } %154, 1
  store i32 %156, i32* %45, align 4
  call void @_ZN5TokenD1Ev(%class.Token* %46) #11
  br label %173

; <label>:157:                                    ; preds = %81
  %158 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 4
  store i32 0, i32* %158, align 8
  %159 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 6
  %160 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 7
  store %"class.std::__1::vector.27"* %160, %"class.std::__1::vector.27"** %3, align 8
  %161 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %3, align 8
  %162 = bitcast %"class.std::__1::vector.27"* %161 to %"class.std::__1::__vector_base.28"*
  %163 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %162, i32 0, i32 0
  %164 = load %class.Token*, %class.Token** %163, align 8
  br label %165

; <label>:165:                                    ; preds = %157
  %166 = invoke dereferenceable(288) %class.Token* @_ZN5TokenaSERKS_(%class.Token* %159, %class.Token* dereferenceable(288) %164)
          to label %167 unwind label %149

; <label>:167:                                    ; preds = %165
  %168 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 3
  store i32 0, i32* %168, align 4
  %169 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 6
  %170 = invoke i32 @_ZNK5Token9get_startEv(%class.Token* %169)
          to label %171 unwind label %149

; <label>:171:                                    ; preds = %167
  %172 = getelementptr inbounds %class.Lexer, %class.Lexer* %47, i32 0, i32 0
  store i32 %170, i32* %172, align 8
  ret void

; <label>:173:                                    ; preds = %153, %149
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %76) #11
  br label %174

; <label>:174:                                    ; preds = %173, %145
  call void @_ZNSt3__16vectorI5TokenNS_9allocatorIS1_EEED1Ev(%"class.std::__1::vector.27"* %54) #11
  call void @_ZN5TokenD1Ev(%class.Token* %52) #11
  br label %175

; <label>:175:                                    ; preds = %174, %141
  %176 = load i8*, i8** %44, align 8
  %177 = load i32, i32* %45, align 4
  %178 = insertvalue { i8*, i32 } undef, i8* %176, 0
  %179 = insertvalue { i8*, i32 } %178, i32 %177, 1
  resume { i8*, i32 } %179
}

declare void @_ZN7VariantC1Ev(%struct.Variant*) unnamed_addr #1

declare void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token*, i32, %struct.Variant*, i32, i32, i32) unnamed_addr #1

declare i32 @__gxx_personality_v0(...)

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN7VariantD1Ev(%struct.Variant*) unnamed_addr #2 align 2 {
  %2 = alloca %struct.Variant*, align 8
  store %struct.Variant* %0, %struct.Variant** %2, align 8
  %3 = load %struct.Variant*, %struct.Variant** %2, align 8
  call void @_ZN7VariantD2Ev(%struct.Variant* %3) #11
  ret void
}

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"*, %"class.std::__1::basic_string"* dereferenceable(24)) unnamed_addr #1

declare i32 @_ZN5Token8get_typeEv(%class.Token*) #1

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr dereferenceable(288) %class.Token* @_ZN5TokenaSEOS_(%class.Token*, %class.Token* dereferenceable(288)) #3 align 2 {
  %3 = alloca %class.Token*, align 8
  %4 = alloca %class.Token*, align 8
  store %class.Token* %0, %class.Token** %3, align 8
  store %class.Token* %1, %class.Token** %4, align 8
  %5 = load %class.Token*, %class.Token** %3, align 8
  %6 = getelementptr inbounds %class.Token, %class.Token* %5, i32 0, i32 0
  %7 = load %class.Token*, %class.Token** %4, align 8
  %8 = getelementptr inbounds %class.Token, %class.Token* %7, i32 0, i32 0
  %9 = call dereferenceable(264) %struct.Variant* @_ZN7VariantaSERKS_(%struct.Variant* %6, %struct.Variant* dereferenceable(264) %8)
  %10 = getelementptr inbounds %class.Token, %class.Token* %5, i32 0, i32 1
  %11 = load %class.Token*, %class.Token** %4, align 8
  %12 = getelementptr inbounds %class.Token, %class.Token* %11, i32 0, i32 1
  %13 = bitcast i32* %10 to i8*
  %14 = bitcast i32* %12 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %13, i8* %14, i64 24, i32 8, i1 false)
  ret %class.Token* %5
}

; Function Attrs: ssp uwtable
define void @_ZN5Lexer15_get_next_tokenEb(%class.Token* noalias sret, %class.Lexer*, i1 zeroext) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %6 = alloca %"class.std::__1::basic_string"*, align 8
  %7 = alloca [3 x i64]*, align 8
  %8 = alloca i32, align 4
  %9 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %10 = alloca %"class.std::__1::basic_string"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca %"class.std::__1::basic_string"*, align 8
  %14 = alloca %"class.std::__1::basic_string"*, align 8
  %15 = alloca i8*, align 8
  %16 = alloca %"class.std::__1::basic_string"*, align 8
  %17 = alloca %"class.std::__1::allocator"*, align 8
  %18 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %19 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %20 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %21 = alloca %"class.std::__1::basic_string"*, align 8
  %22 = alloca i64, align 8
  %23 = alloca i8, align 1
  %24 = alloca %"class.std::__1::basic_string"*, align 8
  %25 = alloca i64, align 8
  %26 = alloca i8, align 1
  %27 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %28 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %29 = alloca %"class.std::__1::basic_string"*, align 8
  %30 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %31 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %32 = alloca %"class.std::__1::basic_string"*, align 8
  %33 = alloca i64, align 8
  %34 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %35 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %36 = alloca %"class.std::__1::basic_string"*, align 8
  %37 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %38 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %39 = alloca %"class.std::__1::basic_string"*, align 8
  %40 = alloca i64, align 8
  %41 = alloca i8*, align 8
  %42 = alloca i8*, align 8
  %43 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %44 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %45 = alloca %"class.std::__1::basic_string"*, align 8
  %46 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %47 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %48 = alloca %"class.std::__1::basic_string"*, align 8
  %49 = alloca %"class.std::__1::basic_string"*, align 8
  %50 = alloca i64, align 8
  %51 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %52 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %53 = alloca %"class.std::__1::basic_string"*, align 8
  %54 = alloca %"class.std::__1::basic_string"*, align 8
  %55 = alloca i64, align 8
  %56 = alloca i8, align 1
  %57 = alloca i8, align 1
  %58 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %59 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %60 = alloca %"class.std::__1::basic_string"*, align 8
  %61 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %62 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %63 = alloca %"class.std::__1::basic_string"*, align 8
  %64 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %65 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %66 = alloca %"class.std::__1::basic_string"*, align 8
  %67 = alloca %"class.std::__1::basic_string"*, align 8
  %68 = alloca %"class.std::__1::basic_string"*, align 8
  %69 = alloca %"class.std::__1::basic_string"*, align 8
  %70 = alloca i8*, align 8
  %71 = alloca %"class.std::__1::basic_string"*, align 8
  %72 = alloca i8, align 1
  %73 = alloca %"class.std::__1::allocator"*, align 8
  %74 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %75 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %76 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %77 = alloca %"class.std::__1::basic_string"*, align 8
  %78 = alloca i8*, align 8
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
  %89 = alloca %"class.std::__1::basic_string"*, align 8
  %90 = alloca i8*, align 8
  %91 = alloca %"class.std::__1::basic_string"*, align 8
  %92 = alloca i8, align 1
  %93 = alloca %"class.std::__1::basic_string"*, align 8
  %94 = alloca i8, align 1
  %95 = alloca %"class.std::__1::allocator"*, align 8
  %96 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %97 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %98 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %99 = alloca %"class.std::__1::basic_string"*, align 8
  %100 = alloca i8*, align 8
  %101 = alloca %"class.std::__1::basic_string"*, align 8
  %102 = alloca i8*, align 8
  %103 = alloca %"class.std::__1::allocator"*, align 8
  %104 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %105 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %106 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %107 = alloca %"class.std::__1::basic_string"*, align 8
  %108 = alloca i8*, align 8
  %109 = alloca %"class.std::__1::basic_string"*, align 8
  %110 = alloca i8*, align 8
  %111 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %112 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %113 = alloca %"class.std::__1::basic_string"*, align 8
  %114 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %115 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %116 = alloca %"class.std::__1::basic_string"*, align 8
  %117 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %118 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %119 = alloca %"class.std::__1::basic_string"*, align 8
  %120 = alloca %"class.std::__1::basic_string"*, align 8
  %121 = alloca %"class.std::__1::basic_string"*, align 8
  %122 = alloca %"class.std::__1::basic_string"*, align 8
  %123 = alloca i8, align 1
  %124 = alloca %"class.std::__1::basic_string"*, align 8
  %125 = alloca i8, align 1
  %126 = alloca %"class.std::__1::allocator"*, align 8
  %127 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %128 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %129 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %130 = alloca %"class.std::__1::basic_string"*, align 8
  %131 = alloca i8*, align 8
  %132 = alloca %"class.std::__1::basic_string"*, align 8
  %133 = alloca i8*, align 8
  %134 = alloca %"class.std::__1::basic_string"*, align 8
  %135 = alloca i8, align 1
  %136 = alloca %"class.std::__1::allocator"*, align 8
  %137 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %138 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %139 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %140 = alloca %"class.std::__1::basic_string"*, align 8
  %141 = alloca i8*, align 8
  %142 = alloca %"class.std::__1::basic_string"*, align 8
  %143 = alloca i8*, align 8
  %144 = alloca %"class.std::__1::basic_string"*, align 8
  %145 = alloca i8, align 1
  %146 = alloca %"class.std::__1::basic_string"*, align 8
  %147 = alloca i8, align 1
  %148 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %149 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %150 = alloca %"class.std::__1::basic_string"*, align 8
  %151 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %152 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %153 = alloca %"class.std::__1::basic_string"*, align 8
  %154 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %155 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %156 = alloca %"class.std::__1::basic_string"*, align 8
  %157 = alloca %"class.std::__1::basic_string"*, align 8
  %158 = alloca %"class.std::__1::basic_string"*, align 8
  %159 = alloca %"class.std::__1::allocator"*, align 8
  %160 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %161 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %162 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %163 = alloca %"class.std::__1::basic_string"*, align 8
  %164 = alloca i8*, align 8
  %165 = alloca %"class.std::__1::basic_string"*, align 8
  %166 = alloca i8*, align 8
  %167 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %168 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %169 = alloca %"class.std::__1::basic_string"*, align 8
  %170 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %171 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %172 = alloca %"class.std::__1::basic_string"*, align 8
  %173 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %174 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %175 = alloca %"class.std::__1::basic_string"*, align 8
  %176 = alloca %"class.std::__1::basic_string"*, align 8
  %177 = alloca %"class.std::__1::basic_string"*, align 8
  %178 = alloca %"class.std::__1::allocator"*, align 8
  %179 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %180 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %181 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %182 = alloca %"class.std::__1::basic_string"*, align 8
  %183 = alloca i8*, align 8
  %184 = alloca %"class.std::__1::basic_string"*, align 8
  %185 = alloca i8*, align 8
  %186 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %187 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %188 = alloca %"class.std::__1::basic_string"*, align 8
  %189 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %190 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %191 = alloca %"class.std::__1::basic_string"*, align 8
  %192 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %193 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %194 = alloca %"class.std::__1::basic_string"*, align 8
  %195 = alloca %"class.std::__1::basic_string"*, align 8
  %196 = alloca %"class.std::__1::basic_string"*, align 8
  %197 = alloca %class.Lexer*, align 8
  %198 = alloca i8, align 1
  %199 = alloca i32, align 4
  %200 = alloca %struct.Variant, align 8
  %201 = alloca i8*
  %202 = alloca i32
  %203 = alloca %"class.std::__1::basic_string", align 8
  %204 = alloca i32, align 4
  %205 = alloca i8, align 1
  %206 = alloca %struct.Variant, align 8
  %207 = alloca i32
  %208 = alloca i8, align 1
  %209 = alloca i8, align 1
  %210 = alloca %"class.std::__1::basic_string", align 8
  %211 = alloca %struct.Variant, align 8
  %212 = alloca %struct.Variant, align 8
  %213 = alloca %"class.std::__1::basic_string", align 8
  %214 = alloca i8, align 1
  %215 = alloca %"class.std::__1::basic_string", align 8
  %216 = alloca %struct.Variant, align 8
  %217 = alloca %"class.std::__1::basic_string", align 8
  %218 = alloca %struct.Variant, align 8
  %219 = alloca i8, align 1
  %220 = alloca %struct.Variant, align 8
  %221 = alloca %"class.std::__1::basic_string", align 8
  %222 = alloca %"class.std::__1::basic_string", align 8
  %223 = alloca double, align 8
  %224 = alloca double, align 8
  %225 = alloca %struct.Variant, align 8
  %226 = alloca %struct.Variant, align 8
  %227 = alloca %struct.Variant, align 8
  %228 = alloca %struct.Variant, align 8
  %229 = alloca %struct.Variant, align 8
  %230 = alloca %struct.Variant, align 8
  %231 = alloca %struct.Variant, align 8
  %232 = alloca %"class.std::__1::basic_string", align 8
  %233 = alloca %struct.Variant, align 8
  %234 = alloca %"class.std::__1::basic_string", align 8
  %235 = alloca %struct.Variant, align 8
  %236 = alloca %"class.std::__1::basic_string", align 8
  %237 = alloca %struct.Variant, align 8
  %238 = alloca %"class.std::__1::basic_string", align 8
  %239 = alloca %"class.std::__1::basic_string", align 8
  %240 = alloca %struct.Variant, align 8
  %241 = alloca %"class.std::__1::basic_string", align 8
  %242 = alloca %"class.std::__1::basic_string", align 8
  %243 = alloca %struct.Variant, align 8
  %244 = alloca %"class.std::__1::basic_string", align 8
  %245 = alloca %struct.Variant, align 8
  %246 = alloca %"class.std::__1::basic_string", align 8
  %247 = alloca %struct.Variant, align 8
  %248 = alloca %"class.std::__1::basic_string", align 8
  %249 = alloca %struct.Variant, align 8
  %250 = alloca %"class.std::__1::basic_string", align 8
  %251 = alloca %struct.Variant, align 8
  %252 = alloca %"class.std::__1::basic_string", align 8
  %253 = alloca %"class.std::__1::basic_string", align 8
  %254 = alloca %struct.Variant, align 8
  store %class.Lexer* %1, %class.Lexer** %197, align 8
  %255 = zext i1 %2 to i8
  store i8 %255, i8* %198, align 1
  %256 = load %class.Lexer*, %class.Lexer** %197, align 8
  %257 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 6
  %258 = call i32 @_ZN5Token8get_typeEv(%class.Token* %257)
  %259 = icmp eq i32 %258, 7
  br i1 %259, label %260, label %262

; <label>:260:                                    ; preds = %3
  %261 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 6
  call void @_ZN5TokenC1ERKS_(%class.Token* %0, %class.Token* dereferenceable(288) %261)
  br label %1622

; <label>:262:                                    ; preds = %3
  %263 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %264 = load i32, i32* %263, align 8
  %265 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 3
  store i32 %264, i32* %265, align 4
  %266 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 1
  %267 = load i32, i32* %266, align 4
  store i32 %267, i32* %199, align 4
  %268 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %269 = load i32, i32* %268, align 8
  %270 = sext i32 %269 to i64
  %271 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 8
  store %"class.std::__1::basic_string"* %271, %"class.std::__1::basic_string"** %196, align 8
  %272 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %196, align 8
  store %"class.std::__1::basic_string"* %272, %"class.std::__1::basic_string"** %195, align 8
  %273 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %195, align 8
  store %"class.std::__1::basic_string"* %273, %"class.std::__1::basic_string"** %194, align 8
  %274 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %194, align 8
  %275 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %274, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %275, %"class.std::__1::__compressed_pair"** %193, align 8
  %276 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %193, align 8
  %277 = bitcast %"class.std::__1::__compressed_pair"* %276 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %277, %"class.std::__1::__libcpp_compressed_pair_imp"** %192, align 8
  %278 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %192, align 8
  %279 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %278, i32 0, i32 0
  %280 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %279, i32 0, i32 0
  %281 = bitcast %union.anon.0* %280 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %282 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %281, i32 0, i32 0
  %283 = bitcast %union.anon.1* %282 to i8*
  %284 = load i8, i8* %283, align 8
  %285 = zext i8 %284 to i32
  %286 = and i32 %285, 1
  %287 = icmp ne i32 %286, 0
  br i1 %287, label %288, label %299

; <label>:288:                                    ; preds = %262
  store %"class.std::__1::basic_string"* %273, %"class.std::__1::basic_string"** %188, align 8
  %289 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %188, align 8
  %290 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %289, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %290, %"class.std::__1::__compressed_pair"** %187, align 8
  %291 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %187, align 8
  %292 = bitcast %"class.std::__1::__compressed_pair"* %291 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %292, %"class.std::__1::__libcpp_compressed_pair_imp"** %186, align 8
  %293 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %186, align 8
  %294 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %293, i32 0, i32 0
  %295 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %294, i32 0, i32 0
  %296 = bitcast %union.anon.0* %295 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %297 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %296, i32 0, i32 1
  %298 = load i64, i64* %297, align 8
  br label %314

; <label>:299:                                    ; preds = %262
  store %"class.std::__1::basic_string"* %273, %"class.std::__1::basic_string"** %191, align 8
  %300 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %191, align 8
  %301 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %300, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %301, %"class.std::__1::__compressed_pair"** %190, align 8
  %302 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %190, align 8
  %303 = bitcast %"class.std::__1::__compressed_pair"* %302 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %303, %"class.std::__1::__libcpp_compressed_pair_imp"** %189, align 8
  %304 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %189, align 8
  %305 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %304, i32 0, i32 0
  %306 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %305, i32 0, i32 0
  %307 = bitcast %union.anon.0* %306 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %308 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %307, i32 0, i32 0
  %309 = bitcast %union.anon.1* %308 to i8*
  %310 = load i8, i8* %309, align 8
  %311 = zext i8 %310 to i32
  %312 = ashr i32 %311, 1
  %313 = sext i32 %312 to i64
  br label %314

; <label>:314:                                    ; preds = %288, %299
  %315 = phi i64 [ %298, %288 ], [ %313, %299 ]
  %316 = icmp uge i64 %270, %315
  br i1 %316, label %317, label %328

; <label>:317:                                    ; preds = %314
  call void @_ZN7VariantC1Ev(%struct.Variant* %200)
  %318 = load i32, i32* %199, align 4
  %319 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %320 = load i32, i32* %319, align 8
  %321 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %322 = load i32, i32* %321, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 7, %struct.Variant* %200, i32 %318, i32 %320, i32 %322)
          to label %323 unwind label %324

; <label>:323:                                    ; preds = %317
  call void @_ZN7VariantD1Ev(%struct.Variant* %200) #11
  br label %1622

; <label>:324:                                    ; preds = %317
  %325 = landingpad { i8*, i32 }
          cleanup
  %326 = extractvalue { i8*, i32 } %325, 0
  store i8* %326, i8** %201, align 8
  %327 = extractvalue { i8*, i32 } %325, 1
  store i32 %327, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %200) #11
  br label %1623

; <label>:328:                                    ; preds = %314
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %184, align 8
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.4, i32 0, i32 0), i8** %185, align 8
  %329 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %184, align 8
  %330 = load i8*, i8** %185, align 8
  store %"class.std::__1::basic_string"* %329, %"class.std::__1::basic_string"** %182, align 8
  store i8* %330, i8** %183, align 8
  %331 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %182, align 8
  %332 = bitcast %"class.std::__1::basic_string"* %331 to %"class.std::__1::__basic_string_common"*
  %333 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %331, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %333, %"class.std::__1::__compressed_pair"** %181, align 8
  %334 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %181, align 8
  store %"class.std::__1::__compressed_pair"* %334, %"class.std::__1::__compressed_pair"** %180, align 8
  %335 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %180, align 8
  %336 = bitcast %"class.std::__1::__compressed_pair"* %335 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %336, %"class.std::__1::__libcpp_compressed_pair_imp"** %179, align 8
  %337 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %179, align 8
  %338 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %337 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %338, %"class.std::__1::allocator"** %178, align 8
  %339 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %178, align 8
  %340 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %337, i32 0, i32 0
  %341 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %340 to i8*
  call void @llvm.memset.p0i8.i64(i8* %341, i8 0, i64 24, i32 8, i1 false) #11
  %342 = load i8*, i8** %183, align 8
  %343 = load i8*, i8** %183, align 8
  %344 = call i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %343)
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %331, i8* %342, i64 %344)
  %345 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %346 = load i32, i32* %345, align 8
  store i32 %346, i32* %204, align 4
  %347 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %348 unwind label %358

; <label>:348:                                    ; preds = %328
  store i8 %347, i8* %205, align 1
  br label %349

; <label>:349:                                    ; preds = %355, %348
  %350 = load i8, i8* %205, align 1
  %351 = sext i8 %350 to i32
  %352 = icmp eq i32 %351, 32
  br i1 %352, label %353, label %362

; <label>:353:                                    ; preds = %349
  %354 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %355 unwind label %358

; <label>:355:                                    ; preds = %353
  store i8 %354, i8* %205, align 1
  %356 = load i32, i32* %204, align 4
  %357 = add nsw i32 %356, 1
  store i32 %357, i32* %204, align 4
  br label %349

; <label>:358:                                    ; preds = %1536, %1321, %1280, %1263, %1116, %942, %925, %802, %716, %699, %651, %634, %590, %585, %493, %476, %1608, %1524, %1520, %1503, %1497, %1347, %1335, %1329, %1326, %1317, %1312, %1288, %1239, %1235, %1214, %1210, %1189, %1185, %1174, %1152, %1130, %1124, %1121, %1112, %1106, %1095, %1093, %1089, %1070, %1068, %1066, %1056, %1045, %1043, %1032, %1030, %1028, %1009, %1007, %1005, %995, %991, %904, %808, %791, %786, %758, %620, %616, %599, %596, %582, %580, %517, %501, %470, %467, %465, %459, %452, %448, %447, %441, %433, %432, %416, %353, %328
  %359 = landingpad { i8*, i32 }
          cleanup
  %360 = extractvalue { i8*, i32 } %359, 0
  store i8* %360, i8** %201, align 8
  %361 = extractvalue { i8*, i32 } %359, 1
  store i32 %361, i32* %202, align 4
  br label %1621

; <label>:362:                                    ; preds = %349
  %363 = load i8, i8* %205, align 1
  %364 = sext i8 %363 to i32
  %365 = icmp eq i32 %364, 0
  br i1 %365, label %366, label %428

; <label>:366:                                    ; preds = %362
  %367 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %368 = load i32, i32* %367, align 8
  %369 = sext i32 %368 to i64
  %370 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 8
  store %"class.std::__1::basic_string"* %370, %"class.std::__1::basic_string"** %177, align 8
  %371 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %177, align 8
  store %"class.std::__1::basic_string"* %371, %"class.std::__1::basic_string"** %176, align 8
  %372 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %176, align 8
  store %"class.std::__1::basic_string"* %372, %"class.std::__1::basic_string"** %175, align 8
  %373 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %175, align 8
  %374 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %373, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %374, %"class.std::__1::__compressed_pair"** %174, align 8
  %375 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %174, align 8
  %376 = bitcast %"class.std::__1::__compressed_pair"* %375 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %376, %"class.std::__1::__libcpp_compressed_pair_imp"** %173, align 8
  %377 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %173, align 8
  %378 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %377, i32 0, i32 0
  %379 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %378, i32 0, i32 0
  %380 = bitcast %union.anon.0* %379 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %381 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %380, i32 0, i32 0
  %382 = bitcast %union.anon.1* %381 to i8*
  %383 = load i8, i8* %382, align 8
  %384 = zext i8 %383 to i32
  %385 = and i32 %384, 1
  %386 = icmp ne i32 %385, 0
  br i1 %386, label %387, label %398

; <label>:387:                                    ; preds = %366
  store %"class.std::__1::basic_string"* %372, %"class.std::__1::basic_string"** %169, align 8
  %388 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %169, align 8
  %389 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %388, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %389, %"class.std::__1::__compressed_pair"** %168, align 8
  %390 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %168, align 8
  %391 = bitcast %"class.std::__1::__compressed_pair"* %390 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %391, %"class.std::__1::__libcpp_compressed_pair_imp"** %167, align 8
  %392 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %167, align 8
  %393 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %392, i32 0, i32 0
  %394 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %393, i32 0, i32 0
  %395 = bitcast %union.anon.0* %394 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %396 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %395, i32 0, i32 1
  %397 = load i64, i64* %396, align 8
  br label %413

; <label>:398:                                    ; preds = %366
  store %"class.std::__1::basic_string"* %372, %"class.std::__1::basic_string"** %172, align 8
  %399 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %172, align 8
  %400 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %399, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %400, %"class.std::__1::__compressed_pair"** %171, align 8
  %401 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %171, align 8
  %402 = bitcast %"class.std::__1::__compressed_pair"* %401 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %402, %"class.std::__1::__libcpp_compressed_pair_imp"** %170, align 8
  %403 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %170, align 8
  %404 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %403, i32 0, i32 0
  %405 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %404, i32 0, i32 0
  %406 = bitcast %union.anon.0* %405 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %407 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %406, i32 0, i32 0
  %408 = bitcast %union.anon.1* %407 to i8*
  %409 = load i8, i8* %408, align 8
  %410 = zext i8 %409 to i32
  %411 = ashr i32 %410, 1
  %412 = sext i32 %411 to i64
  br label %413

; <label>:413:                                    ; preds = %387, %398
  %414 = phi i64 [ %397, %387 ], [ %412, %398 ]
  %415 = icmp uge i64 %369, %414
  br i1 %415, label %416, label %428

; <label>:416:                                    ; preds = %413
  invoke void @_ZN7VariantC1Ev(%struct.Variant* %206)
          to label %417 unwind label %358

; <label>:417:                                    ; preds = %416
  %418 = load i32, i32* %199, align 4
  %419 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %420 = load i32, i32* %419, align 8
  %421 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %422 = load i32, i32* %421, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 7, %struct.Variant* %206, i32 %418, i32 %420, i32 %422)
          to label %423 unwind label %424

; <label>:423:                                    ; preds = %417
  call void @_ZN7VariantD1Ev(%struct.Variant* %206) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:424:                                    ; preds = %417
  %425 = landingpad { i8*, i32 }
          cleanup
  %426 = extractvalue { i8*, i32 } %425, 0
  store i8* %426, i8** %201, align 8
  %427 = extractvalue { i8*, i32 } %425, 1
  store i32 %427, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %206) #11
  br label %1621

; <label>:428:                                    ; preds = %413, %362
  %429 = load i8, i8* %205, align 1
  %430 = sext i8 %429 to i32
  %431 = icmp eq i32 %430, 35
  br i1 %431, label %432, label %437

; <label>:432:                                    ; preds = %428
  invoke void @_ZN5Lexer14ignore_commentEv(%class.Lexer* %256)
          to label %433 unwind label %358

; <label>:433:                                    ; preds = %432
  %434 = load i8, i8* %198, align 1
  %435 = trunc i8 %434 to i1
  invoke void @_ZN5Lexer15_get_next_tokenEb(%class.Token* sret %0, %class.Lexer* %256, i1 zeroext %435)
          to label %436 unwind label %358

; <label>:436:                                    ; preds = %433
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:437:                                    ; preds = %428
  %438 = load i8, i8* %205, align 1
  %439 = sext i8 %438 to i32
  %440 = icmp eq i32 %439, 47
  br i1 %440, label %441, label %455

; <label>:441:                                    ; preds = %437
  %442 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %443 unwind label %358

; <label>:443:                                    ; preds = %441
  store i8 %442, i8* %208, align 1
  %444 = load i8, i8* %208, align 1
  %445 = sext i8 %444 to i32
  %446 = icmp eq i32 %445, 42
  br i1 %446, label %447, label %452

; <label>:447:                                    ; preds = %443
  invoke void @_ZN5Lexer17ignore_ml_commentEv(%class.Lexer* %256)
          to label %448 unwind label %358

; <label>:448:                                    ; preds = %447
  %449 = load i8, i8* %198, align 1
  %450 = trunc i8 %449 to i1
  invoke void @_ZN5Lexer15_get_next_tokenEb(%class.Token* sret %0, %class.Lexer* %256, i1 zeroext %450)
          to label %451 unwind label %358

; <label>:451:                                    ; preds = %448
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:452:                                    ; preds = %443
  invoke void @_ZN5Lexer11backtrack_cEi(%class.Lexer* %256, i32 1)
          to label %453 unwind label %358

; <label>:453:                                    ; preds = %452
  br label %454

; <label>:454:                                    ; preds = %453
  br label %455

; <label>:455:                                    ; preds = %454, %437
  %456 = load i8, i8* %205, align 1
  %457 = sext i8 %456 to i32
  %458 = icmp eq i32 %457, 39
  br i1 %458, label %459, label %513

; <label>:459:                                    ; preds = %455
  %460 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %461 unwind label %358

; <label>:461:                                    ; preds = %459
  store i8 %460, i8* %208, align 1
  %462 = load i8, i8* %208, align 1
  %463 = sext i8 %462 to i32
  %464 = icmp eq i32 %463, 92
  br i1 %464, label %465, label %470

; <label>:465:                                    ; preds = %461
  %466 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %467 unwind label %358

; <label>:467:                                    ; preds = %465
  %468 = invoke signext i8 @_ZN5Lexer11escape_charEc(%class.Lexer* %256, i8 signext %466)
          to label %469 unwind label %358

; <label>:469:                                    ; preds = %467
  store i8 %468, i8* %208, align 1
  br label %470

; <label>:470:                                    ; preds = %469, %461
  %471 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %472 unwind label %358

; <label>:472:                                    ; preds = %470
  store i8 %471, i8* %209, align 1
  %473 = load i8, i8* %209, align 1
  %474 = sext i8 %473 to i32
  %475 = icmp ne i32 %474, 39
  br i1 %475, label %476, label %501

; <label>:476:                                    ; preds = %472
  store %"class.std::__1::basic_string"* %210, %"class.std::__1::basic_string"** %165, align 8
  store i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.5, i32 0, i32 0), i8** %166, align 8
  %477 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %165, align 8
  %478 = load i8*, i8** %166, align 8
  store %"class.std::__1::basic_string"* %477, %"class.std::__1::basic_string"** %163, align 8
  store i8* %478, i8** %164, align 8
  %479 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %163, align 8
  %480 = bitcast %"class.std::__1::basic_string"* %479 to %"class.std::__1::__basic_string_common"*
  %481 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %479, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %481, %"class.std::__1::__compressed_pair"** %162, align 8
  %482 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %162, align 8
  store %"class.std::__1::__compressed_pair"* %482, %"class.std::__1::__compressed_pair"** %161, align 8
  %483 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %161, align 8
  %484 = bitcast %"class.std::__1::__compressed_pair"* %483 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %484, %"class.std::__1::__libcpp_compressed_pair_imp"** %160, align 8
  %485 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %160, align 8
  %486 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %485 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %486, %"class.std::__1::allocator"** %159, align 8
  %487 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %159, align 8
  %488 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %485, i32 0, i32 0
  %489 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %488 to i8*
  call void @llvm.memset.p0i8.i64(i8* %489, i8 0, i64 24, i32 8, i1 false) #11
  %490 = load i8*, i8** %164, align 8
  %491 = load i8*, i8** %164, align 8
  %492 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %491)
          to label %493 unwind label %358

; <label>:493:                                    ; preds = %476
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %479, i8* %490, i64 %492)
          to label %494 unwind label %358

; <label>:494:                                    ; preds = %493
  br label %495

; <label>:495:                                    ; preds = %494
  invoke void @_ZN10ParseError5raiseE11ParseErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP5Lexer(i32 1, %"class.std::__1::basic_string"* %210, %class.Lexer* %256)
          to label %496 unwind label %497

; <label>:496:                                    ; preds = %495
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %210) #11
  br label %501

; <label>:497:                                    ; preds = %495
  %498 = landingpad { i8*, i32 }
          cleanup
  %499 = extractvalue { i8*, i32 } %498, 0
  store i8* %499, i8** %201, align 8
  %500 = extractvalue { i8*, i32 } %498, 1
  store i32 %500, i32* %202, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %210) #11
  br label %1621

; <label>:501:                                    ; preds = %496, %472
  %502 = load i8, i8* %208, align 1
  invoke void @_ZN7VariantC1Ec(%struct.Variant* %211, i8 signext %502)
          to label %503 unwind label %358

; <label>:503:                                    ; preds = %501
  %504 = load i32, i32* %199, align 4
  %505 = load i32, i32* %204, align 4
  %506 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %507 = load i32, i32* %506, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 5, %struct.Variant* %211, i32 %504, i32 %505, i32 %507)
          to label %508 unwind label %509

; <label>:508:                                    ; preds = %503
  call void @_ZN7VariantD1Ev(%struct.Variant* %211) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:509:                                    ; preds = %503
  %510 = landingpad { i8*, i32 }
          cleanup
  %511 = extractvalue { i8*, i32 } %510, 0
  store i8* %511, i8** %201, align 8
  %512 = extractvalue { i8*, i32 } %510, 1
  store i32 %512, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %211) #11
  br label %1621

; <label>:513:                                    ; preds = %455
  %514 = load i8, i8* %205, align 1
  %515 = sext i8 %514 to i32
  %516 = icmp eq i32 %515, 34
  br i1 %516, label %517, label %616

; <label>:517:                                    ; preds = %513
  %518 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %519 unwind label %358

; <label>:519:                                    ; preds = %517
  store i8 %518, i8* %208, align 1
  br label %520

; <label>:520:                                    ; preds = %598, %519
  %521 = load i8, i8* %208, align 1
  %522 = sext i8 %521 to i32
  %523 = icmp ne i32 %522, 34
  br i1 %523, label %524, label %574

; <label>:524:                                    ; preds = %520
  %525 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %526 = load i32, i32* %525, align 8
  %527 = sext i32 %526 to i64
  %528 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 8
  store %"class.std::__1::basic_string"* %528, %"class.std::__1::basic_string"** %158, align 8
  %529 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %158, align 8
  store %"class.std::__1::basic_string"* %529, %"class.std::__1::basic_string"** %157, align 8
  %530 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %157, align 8
  store %"class.std::__1::basic_string"* %530, %"class.std::__1::basic_string"** %156, align 8
  %531 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %156, align 8
  %532 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %531, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %532, %"class.std::__1::__compressed_pair"** %155, align 8
  %533 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %155, align 8
  %534 = bitcast %"class.std::__1::__compressed_pair"* %533 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %534, %"class.std::__1::__libcpp_compressed_pair_imp"** %154, align 8
  %535 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %154, align 8
  %536 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %535, i32 0, i32 0
  %537 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %536, i32 0, i32 0
  %538 = bitcast %union.anon.0* %537 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %539 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %538, i32 0, i32 0
  %540 = bitcast %union.anon.1* %539 to i8*
  %541 = load i8, i8* %540, align 8
  %542 = zext i8 %541 to i32
  %543 = and i32 %542, 1
  %544 = icmp ne i32 %543, 0
  br i1 %544, label %545, label %556

; <label>:545:                                    ; preds = %524
  store %"class.std::__1::basic_string"* %530, %"class.std::__1::basic_string"** %150, align 8
  %546 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %150, align 8
  %547 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %546, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %547, %"class.std::__1::__compressed_pair"** %149, align 8
  %548 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %149, align 8
  %549 = bitcast %"class.std::__1::__compressed_pair"* %548 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %549, %"class.std::__1::__libcpp_compressed_pair_imp"** %148, align 8
  %550 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %148, align 8
  %551 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %550, i32 0, i32 0
  %552 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %551, i32 0, i32 0
  %553 = bitcast %union.anon.0* %552 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %554 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %553, i32 0, i32 1
  %555 = load i64, i64* %554, align 8
  br label %571

; <label>:556:                                    ; preds = %524
  store %"class.std::__1::basic_string"* %530, %"class.std::__1::basic_string"** %153, align 8
  %557 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %153, align 8
  %558 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %557, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %558, %"class.std::__1::__compressed_pair"** %152, align 8
  %559 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %152, align 8
  %560 = bitcast %"class.std::__1::__compressed_pair"* %559 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %560, %"class.std::__1::__libcpp_compressed_pair_imp"** %151, align 8
  %561 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %151, align 8
  %562 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %561, i32 0, i32 0
  %563 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %562, i32 0, i32 0
  %564 = bitcast %union.anon.0* %563 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %565 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %564, i32 0, i32 0
  %566 = bitcast %union.anon.1* %565 to i8*
  %567 = load i8, i8* %566, align 8
  %568 = zext i8 %567 to i32
  %569 = ashr i32 %568, 1
  %570 = sext i32 %569 to i64
  br label %571

; <label>:571:                                    ; preds = %545, %556
  %572 = phi i64 [ %555, %545 ], [ %570, %556 ]
  %573 = icmp ult i64 %527, %572
  br label %574

; <label>:574:                                    ; preds = %571, %520
  %575 = phi i1 [ false, %520 ], [ %573, %571 ]
  br i1 %575, label %576, label %599

; <label>:576:                                    ; preds = %574
  %577 = load i8, i8* %208, align 1
  %578 = sext i8 %577 to i32
  %579 = icmp eq i32 %578, 92
  br i1 %579, label %580, label %590

; <label>:580:                                    ; preds = %576
  %581 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %582 unwind label %358

; <label>:582:                                    ; preds = %580
  store i8 %581, i8* %208, align 1
  %583 = load i8, i8* %208, align 1
  %584 = invoke signext i8 @_ZN5Lexer11escape_charEc(%class.Lexer* %256, i8 signext %583)
          to label %585 unwind label %358

; <label>:585:                                    ; preds = %582
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %146, align 8
  store i8 %584, i8* %147, align 1
  %586 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %146, align 8
  %587 = load i8, i8* %147, align 1
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE9push_backEc(%"class.std::__1::basic_string"* %586, i8 signext %587)
          to label %588 unwind label %358

; <label>:588:                                    ; preds = %585
  br label %589

; <label>:589:                                    ; preds = %588
  br label %596

; <label>:590:                                    ; preds = %576
  %591 = load i8, i8* %208, align 1
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %144, align 8
  store i8 %591, i8* %145, align 1
  %592 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %144, align 8
  %593 = load i8, i8* %145, align 1
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE9push_backEc(%"class.std::__1::basic_string"* %592, i8 signext %593)
          to label %594 unwind label %358

; <label>:594:                                    ; preds = %590
  br label %595

; <label>:595:                                    ; preds = %594
  br label %596

; <label>:596:                                    ; preds = %595, %589
  %597 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %598 unwind label %358

; <label>:598:                                    ; preds = %596
  store i8 %597, i8* %208, align 1
  br label %520

; <label>:599:                                    ; preds = %574
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %213, %"class.std::__1::basic_string"* dereferenceable(24) %203)
          to label %600 unwind label %358

; <label>:600:                                    ; preds = %599
  invoke void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant* %212, %"class.std::__1::basic_string"* %213)
          to label %601 unwind label %607

; <label>:601:                                    ; preds = %600
  %602 = load i32, i32* %199, align 4
  %603 = load i32, i32* %204, align 4
  %604 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %605 = load i32, i32* %604, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 5, %struct.Variant* %212, i32 %602, i32 %603, i32 %605)
          to label %606 unwind label %611

; <label>:606:                                    ; preds = %601
  call void @_ZN7VariantD1Ev(%struct.Variant* %212) #11
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %213) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:607:                                    ; preds = %600
  %608 = landingpad { i8*, i32 }
          cleanup
  %609 = extractvalue { i8*, i32 } %608, 0
  store i8* %609, i8** %201, align 8
  %610 = extractvalue { i8*, i32 } %608, 1
  store i32 %610, i32* %202, align 4
  br label %615

; <label>:611:                                    ; preds = %601
  %612 = landingpad { i8*, i32 }
          cleanup
  %613 = extractvalue { i8*, i32 } %612, 0
  store i8* %613, i8** %201, align 8
  %614 = extractvalue { i8*, i32 } %612, 1
  store i32 %614, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %212) #11
  br label %615

; <label>:615:                                    ; preds = %611, %607
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %213) #11
  br label %1621

; <label>:616:                                    ; preds = %513
  %617 = load i8, i8* %205, align 1
  %618 = invoke zeroext i1 @_ZN5Lexer9is_numberEcb(%class.Lexer* %256, i8 signext %617, i1 zeroext false)
          to label %619 unwind label %358

; <label>:619:                                    ; preds = %616
  br i1 %618, label %620, label %1106

; <label>:620:                                    ; preds = %619
  %621 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %622 unwind label %358

; <label>:622:                                    ; preds = %620
  store i8 %621, i8* %214, align 1
  %623 = load i8, i8* %205, align 1
  %624 = sext i8 %623 to i32
  %625 = icmp eq i32 %624, 48
  br i1 %625, label %626, label %687

; <label>:626:                                    ; preds = %622
  %627 = load i8, i8* %214, align 1
  %628 = sext i8 %627 to i32
  %629 = icmp eq i32 %628, 120
  br i1 %629, label %634, label %630

; <label>:630:                                    ; preds = %626
  %631 = load i8, i8* %214, align 1
  %632 = sext i8 %631 to i32
  %633 = icmp eq i32 %632, 88
  br i1 %633, label %634, label %687

; <label>:634:                                    ; preds = %630, %626
  store %"class.std::__1::basic_string"* %215, %"class.std::__1::basic_string"** %142, align 8
  store i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.6, i32 0, i32 0), i8** %143, align 8
  %635 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %142, align 8
  %636 = load i8*, i8** %143, align 8
  store %"class.std::__1::basic_string"* %635, %"class.std::__1::basic_string"** %140, align 8
  store i8* %636, i8** %141, align 8
  %637 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %140, align 8
  %638 = bitcast %"class.std::__1::basic_string"* %637 to %"class.std::__1::__basic_string_common"*
  %639 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %637, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %639, %"class.std::__1::__compressed_pair"** %139, align 8
  %640 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %139, align 8
  store %"class.std::__1::__compressed_pair"* %640, %"class.std::__1::__compressed_pair"** %138, align 8
  %641 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %138, align 8
  %642 = bitcast %"class.std::__1::__compressed_pair"* %641 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %642, %"class.std::__1::__libcpp_compressed_pair_imp"** %137, align 8
  %643 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %137, align 8
  %644 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %643 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %644, %"class.std::__1::allocator"** %136, align 8
  %645 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %136, align 8
  %646 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %643, i32 0, i32 0
  %647 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %646 to i8*
  call void @llvm.memset.p0i8.i64(i8* %647, i8 0, i64 24, i32 8, i1 false) #11
  %648 = load i8*, i8** %141, align 8
  %649 = load i8*, i8** %141, align 8
  %650 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %649)
          to label %651 unwind label %358

; <label>:651:                                    ; preds = %634
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %637, i8* %648, i64 %650)
          to label %652 unwind label %358

; <label>:652:                                    ; preds = %651
  br label %653

; <label>:653:                                    ; preds = %652
  %654 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %655 unwind label %668

; <label>:655:                                    ; preds = %653
  store i8 %654, i8* %208, align 1
  br label %656

; <label>:656:                                    ; preds = %667, %655
  %657 = load i8, i8* %208, align 1
  %658 = invoke zeroext i1 @_ZN5Lexer6is_hexEc(%class.Lexer* %256, i8 signext %657)
          to label %659 unwind label %668

; <label>:659:                                    ; preds = %656
  br i1 %658, label %660, label %672

; <label>:660:                                    ; preds = %659
  %661 = load i8, i8* %208, align 1
  store %"class.std::__1::basic_string"* %215, %"class.std::__1::basic_string"** %134, align 8
  store i8 %661, i8* %135, align 1
  %662 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %134, align 8
  %663 = load i8, i8* %135, align 1
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE9push_backEc(%"class.std::__1::basic_string"* %662, i8 signext %663)
          to label %664 unwind label %668

; <label>:664:                                    ; preds = %660
  br label %665

; <label>:665:                                    ; preds = %664
  %666 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %667 unwind label %668

; <label>:667:                                    ; preds = %665
  store i8 %666, i8* %208, align 1
  br label %656

; <label>:668:                                    ; preds = %660, %675, %673, %672, %665, %656, %653
  %669 = landingpad { i8*, i32 }
          cleanup
  %670 = extractvalue { i8*, i32 } %669, 0
  store i8* %670, i8** %201, align 8
  %671 = extractvalue { i8*, i32 } %669, 1
  store i32 %671, i32* %202, align 4
  br label %686

; <label>:672:                                    ; preds = %659
  invoke void @_ZN5Lexer11backtrack_cEi(%class.Lexer* %256, i32 1)
          to label %673 unwind label %668

; <label>:673:                                    ; preds = %672
  %674 = invoke i64 @_ZNSt3__15stoulERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPmi(%"class.std::__1::basic_string"* dereferenceable(24) %215, i64* null, i32 16)
          to label %675 unwind label %668

; <label>:675:                                    ; preds = %673
  invoke void @_ZN7VariantC1El(%struct.Variant* %216, i64 %674)
          to label %676 unwind label %668

; <label>:676:                                    ; preds = %675
  %677 = load i32, i32* %199, align 4
  %678 = load i32, i32* %204, align 4
  %679 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %680 = load i32, i32* %679, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 5, %struct.Variant* %216, i32 %677, i32 %678, i32 %680)
          to label %681 unwind label %682

; <label>:681:                                    ; preds = %676
  call void @_ZN7VariantD1Ev(%struct.Variant* %216) #11
  store i32 1, i32* %207, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %215) #11
  br label %1620

; <label>:682:                                    ; preds = %676
  %683 = landingpad { i8*, i32 }
          cleanup
  %684 = extractvalue { i8*, i32 } %683, 0
  store i8* %684, i8** %201, align 8
  %685 = extractvalue { i8*, i32 } %683, 1
  store i32 %685, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %216) #11
  br label %686

; <label>:686:                                    ; preds = %682, %668
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %215) #11
  br label %1621

; <label>:687:                                    ; preds = %630, %622
  %688 = load i8, i8* %205, align 1
  %689 = sext i8 %688 to i32
  %690 = icmp eq i32 %689, 48
  br i1 %690, label %691, label %758

; <label>:691:                                    ; preds = %687
  %692 = load i8, i8* %214, align 1
  %693 = sext i8 %692 to i32
  %694 = icmp eq i32 %693, 98
  br i1 %694, label %699, label %695

; <label>:695:                                    ; preds = %691
  %696 = load i8, i8* %214, align 1
  %697 = sext i8 %696 to i32
  %698 = icmp eq i32 %697, 66
  br i1 %698, label %699, label %758

; <label>:699:                                    ; preds = %695, %691
  store %"class.std::__1::basic_string"* %217, %"class.std::__1::basic_string"** %132, align 8
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.4, i32 0, i32 0), i8** %133, align 8
  %700 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %132, align 8
  %701 = load i8*, i8** %133, align 8
  store %"class.std::__1::basic_string"* %700, %"class.std::__1::basic_string"** %130, align 8
  store i8* %701, i8** %131, align 8
  %702 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %130, align 8
  %703 = bitcast %"class.std::__1::basic_string"* %702 to %"class.std::__1::__basic_string_common"*
  %704 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %702, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %704, %"class.std::__1::__compressed_pair"** %129, align 8
  %705 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %129, align 8
  store %"class.std::__1::__compressed_pair"* %705, %"class.std::__1::__compressed_pair"** %128, align 8
  %706 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %128, align 8
  %707 = bitcast %"class.std::__1::__compressed_pair"* %706 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %707, %"class.std::__1::__libcpp_compressed_pair_imp"** %127, align 8
  %708 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %127, align 8
  %709 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %708 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %709, %"class.std::__1::allocator"** %126, align 8
  %710 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %126, align 8
  %711 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %708, i32 0, i32 0
  %712 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %711 to i8*
  call void @llvm.memset.p0i8.i64(i8* %712, i8 0, i64 24, i32 8, i1 false) #11
  %713 = load i8*, i8** %131, align 8
  %714 = load i8*, i8** %131, align 8
  %715 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %714)
          to label %716 unwind label %358

; <label>:716:                                    ; preds = %699
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %702, i8* %713, i64 %715)
          to label %717 unwind label %358

; <label>:717:                                    ; preds = %716
  br label %718

; <label>:718:                                    ; preds = %717
  %719 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %720 unwind label %739

; <label>:720:                                    ; preds = %718
  store i8 %719, i8* %208, align 1
  br label %721

; <label>:721:                                    ; preds = %738, %720
  %722 = load i8, i8* %208, align 1
  %723 = sext i8 %722 to i32
  %724 = icmp eq i32 %723, 48
  br i1 %724, label %729, label %725

; <label>:725:                                    ; preds = %721
  %726 = load i8, i8* %208, align 1
  %727 = sext i8 %726 to i32
  %728 = icmp eq i32 %727, 49
  br label %729

; <label>:729:                                    ; preds = %725, %721
  %730 = phi i1 [ true, %721 ], [ %728, %725 ]
  br i1 %730, label %731, label %743

; <label>:731:                                    ; preds = %729
  %732 = load i8, i8* %208, align 1
  store %"class.std::__1::basic_string"* %217, %"class.std::__1::basic_string"** %124, align 8
  store i8 %732, i8* %125, align 1
  %733 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %124, align 8
  %734 = load i8, i8* %125, align 1
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE9push_backEc(%"class.std::__1::basic_string"* %733, i8 signext %734)
          to label %735 unwind label %739

; <label>:735:                                    ; preds = %731
  br label %736

; <label>:736:                                    ; preds = %735
  %737 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %738 unwind label %739

; <label>:738:                                    ; preds = %736
  store i8 %737, i8* %208, align 1
  br label %721

; <label>:739:                                    ; preds = %731, %746, %744, %743, %736, %718
  %740 = landingpad { i8*, i32 }
          cleanup
  %741 = extractvalue { i8*, i32 } %740, 0
  store i8* %741, i8** %201, align 8
  %742 = extractvalue { i8*, i32 } %740, 1
  store i32 %742, i32* %202, align 4
  br label %757

; <label>:743:                                    ; preds = %729
  invoke void @_ZN5Lexer11backtrack_cEi(%class.Lexer* %256, i32 1)
          to label %744 unwind label %739

; <label>:744:                                    ; preds = %743
  %745 = invoke i64 @_ZNSt3__15stoulERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPmi(%"class.std::__1::basic_string"* dereferenceable(24) %217, i64* null, i32 2)
          to label %746 unwind label %739

; <label>:746:                                    ; preds = %744
  invoke void @_ZN7VariantC1El(%struct.Variant* %218, i64 %745)
          to label %747 unwind label %739

; <label>:747:                                    ; preds = %746
  %748 = load i32, i32* %199, align 4
  %749 = load i32, i32* %204, align 4
  %750 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %751 = load i32, i32* %750, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 5, %struct.Variant* %218, i32 %748, i32 %749, i32 %751)
          to label %752 unwind label %753

; <label>:752:                                    ; preds = %747
  call void @_ZN7VariantD1Ev(%struct.Variant* %218) #11
  store i32 1, i32* %207, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %217) #11
  br label %1620

; <label>:753:                                    ; preds = %747
  %754 = landingpad { i8*, i32 }
          cleanup
  %755 = extractvalue { i8*, i32 } %754, 0
  store i8* %755, i8** %201, align 8
  %756 = extractvalue { i8*, i32 } %754, 1
  store i32 %756, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %218) #11
  br label %757

; <label>:757:                                    ; preds = %753, %739
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %217) #11
  br label %1621

; <label>:758:                                    ; preds = %695, %687
  invoke void @_ZN5Lexer11backtrack_cEi(%class.Lexer* %256, i32 1)
          to label %759 unwind label %358

; <label>:759:                                    ; preds = %758
  %760 = load i8, i8* %205, align 1
  store i8 %760, i8* %208, align 1
  store i8 0, i8* %219, align 1
  br label %761

; <label>:761:                                    ; preds = %810, %759
  %762 = load i8, i8* %208, align 1
  %763 = sext i8 %762 to i32
  %764 = icmp sge i32 %763, 48
  br i1 %764, label %765, label %769

; <label>:765:                                    ; preds = %761
  %766 = load i8, i8* %208, align 1
  %767 = sext i8 %766 to i32
  %768 = icmp sle i32 %767, 57
  br i1 %768, label %777, label %769

; <label>:769:                                    ; preds = %765, %761
  %770 = load i8, i8* %208, align 1
  %771 = sext i8 %770 to i32
  %772 = icmp eq i32 %771, 95
  br i1 %772, label %777, label %773

; <label>:773:                                    ; preds = %769
  %774 = load i8, i8* %208, align 1
  %775 = sext i8 %774 to i32
  %776 = icmp eq i32 %775, 46
  br label %777

; <label>:777:                                    ; preds = %773, %769, %765
  %778 = phi i1 [ true, %769 ], [ true, %765 ], [ %776, %773 ]
  br i1 %778, label %779, label %811

; <label>:779:                                    ; preds = %777
  %780 = load i8, i8* %208, align 1
  %781 = sext i8 %780 to i32
  %782 = icmp eq i32 %781, 46
  br i1 %782, label %783, label %798

; <label>:783:                                    ; preds = %779
  %784 = load i8, i8* %219, align 1
  %785 = trunc i8 %784 to i1
  br i1 %785, label %798, label %786

; <label>:786:                                    ; preds = %783
  %787 = invoke signext i8 @_ZN5Lexer14char_lookaheadEv(%class.Lexer* %256)
          to label %788 unwind label %358

; <label>:788:                                    ; preds = %786
  %789 = sext i8 %787 to i32
  %790 = icmp slt i32 %789, 48
  br i1 %790, label %796, label %791

; <label>:791:                                    ; preds = %788
  %792 = invoke signext i8 @_ZN5Lexer14char_lookaheadEv(%class.Lexer* %256)
          to label %793 unwind label %358

; <label>:793:                                    ; preds = %791
  %794 = sext i8 %792 to i32
  %795 = icmp sgt i32 %794, 57
  br i1 %795, label %796, label %797

; <label>:796:                                    ; preds = %793, %788
  br label %811

; <label>:797:                                    ; preds = %793
  store i8 1, i8* %219, align 1
  br label %798

; <label>:798:                                    ; preds = %797, %783, %779
  %799 = load i8, i8* %208, align 1
  %800 = sext i8 %799 to i32
  %801 = icmp ne i32 %800, 95
  br i1 %801, label %802, label %808

; <label>:802:                                    ; preds = %798
  %803 = load i8, i8* %208, align 1
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %122, align 8
  store i8 %803, i8* %123, align 1
  %804 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %122, align 8
  %805 = load i8, i8* %123, align 1
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE9push_backEc(%"class.std::__1::basic_string"* %804, i8 signext %805)
          to label %806 unwind label %358

; <label>:806:                                    ; preds = %802
  br label %807

; <label>:807:                                    ; preds = %806
  br label %808

; <label>:808:                                    ; preds = %807, %798
  %809 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %810 unwind label %358

; <label>:810:                                    ; preds = %808
  store i8 %809, i8* %208, align 1
  br label %761

; <label>:811:                                    ; preds = %796, %777
  %812 = load i8, i8* %205, align 1
  %813 = sext i8 %812 to i32
  %814 = icmp eq i32 %813, 48
  br i1 %814, label %815, label %917

; <label>:815:                                    ; preds = %811
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %121, align 8
  %816 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %121, align 8
  store %"class.std::__1::basic_string"* %816, %"class.std::__1::basic_string"** %120, align 8
  %817 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %120, align 8
  store %"class.std::__1::basic_string"* %817, %"class.std::__1::basic_string"** %119, align 8
  %818 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %119, align 8
  %819 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %818, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %819, %"class.std::__1::__compressed_pair"** %118, align 8
  %820 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %118, align 8
  %821 = bitcast %"class.std::__1::__compressed_pair"* %820 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %821, %"class.std::__1::__libcpp_compressed_pair_imp"** %117, align 8
  %822 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %117, align 8
  %823 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %822, i32 0, i32 0
  %824 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %823, i32 0, i32 0
  %825 = bitcast %union.anon.0* %824 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %826 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %825, i32 0, i32 0
  %827 = bitcast %union.anon.1* %826 to i8*
  %828 = load i8, i8* %827, align 8
  %829 = zext i8 %828 to i32
  %830 = and i32 %829, 1
  %831 = icmp ne i32 %830, 0
  br i1 %831, label %832, label %843

; <label>:832:                                    ; preds = %815
  store %"class.std::__1::basic_string"* %817, %"class.std::__1::basic_string"** %113, align 8
  %833 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %113, align 8
  %834 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %833, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %834, %"class.std::__1::__compressed_pair"** %112, align 8
  %835 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %112, align 8
  %836 = bitcast %"class.std::__1::__compressed_pair"* %835 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %836, %"class.std::__1::__libcpp_compressed_pair_imp"** %111, align 8
  %837 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %111, align 8
  %838 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %837, i32 0, i32 0
  %839 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %838, i32 0, i32 0
  %840 = bitcast %union.anon.0* %839 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %841 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %840, i32 0, i32 1
  %842 = load i64, i64* %841, align 8
  br label %858

; <label>:843:                                    ; preds = %815
  store %"class.std::__1::basic_string"* %817, %"class.std::__1::basic_string"** %116, align 8
  %844 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %116, align 8
  %845 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %844, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %845, %"class.std::__1::__compressed_pair"** %115, align 8
  %846 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %115, align 8
  %847 = bitcast %"class.std::__1::__compressed_pair"* %846 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %847, %"class.std::__1::__libcpp_compressed_pair_imp"** %114, align 8
  %848 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %114, align 8
  %849 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %848, i32 0, i32 0
  %850 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %849, i32 0, i32 0
  %851 = bitcast %union.anon.0* %850 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %852 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %851, i32 0, i32 0
  %853 = bitcast %union.anon.1* %852 to i8*
  %854 = load i8, i8* %853, align 8
  %855 = zext i8 %854 to i32
  %856 = ashr i32 %855, 1
  %857 = sext i32 %856 to i64
  br label %858

; <label>:858:                                    ; preds = %832, %843
  %859 = phi i64 [ %842, %832 ], [ %857, %843 ]
  %860 = icmp ne i64 %859, 1
  br i1 %860, label %861, label %917

; <label>:861:                                    ; preds = %858
  %862 = load i8, i8* %219, align 1
  %863 = trunc i8 %862 to i1
  br i1 %863, label %917, label %864

; <label>:864:                                    ; preds = %861
  invoke void @_ZN5Lexer11backtrack_cEi(%class.Lexer* %256, i32 1)
          to label %865 unwind label %874

; <label>:865:                                    ; preds = %864
  %866 = invoke i32 @_ZNSt3__14stoiERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPmi(%"class.std::__1::basic_string"* dereferenceable(24) %203, i64* null, i32 8)
          to label %867 unwind label %874

; <label>:867:                                    ; preds = %865
  invoke void @_ZN7VariantC1Ei(%struct.Variant* %220, i32 %866)
          to label %868 unwind label %874

; <label>:868:                                    ; preds = %867
  %869 = load i32, i32* %199, align 4
  %870 = load i32, i32* %204, align 4
  %871 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %872 = load i32, i32* %871, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 5, %struct.Variant* %220, i32 %869, i32 %870, i32 %872)
          to label %873 unwind label %878

; <label>:873:                                    ; preds = %868
  call void @_ZN7VariantD1Ev(%struct.Variant* %220) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:874:                                    ; preds = %867, %865, %864
  %875 = landingpad { i8*, i32 }
          catch i8* null
  %876 = extractvalue { i8*, i32 } %875, 0
  store i8* %876, i8** %201, align 8
  %877 = extractvalue { i8*, i32 } %875, 1
  store i32 %877, i32* %202, align 4
  br label %882

; <label>:878:                                    ; preds = %868
  %879 = landingpad { i8*, i32 }
          catch i8* null
  %880 = extractvalue { i8*, i32 } %879, 0
  store i8* %880, i8** %201, align 8
  %881 = extractvalue { i8*, i32 } %879, 1
  store i32 %881, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %220) #11
  br label %882

; <label>:882:                                    ; preds = %878, %874
  %883 = load i8*, i8** %201, align 8
  %884 = call i8* @__cxa_begin_catch(i8* %883) #11
  store %"class.std::__1::basic_string"* %221, %"class.std::__1::basic_string"** %109, align 8
  store i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.7, i32 0, i32 0), i8** %110, align 8
  %885 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %109, align 8
  %886 = load i8*, i8** %110, align 8
  store %"class.std::__1::basic_string"* %885, %"class.std::__1::basic_string"** %107, align 8
  store i8* %886, i8** %108, align 8
  %887 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %107, align 8
  %888 = bitcast %"class.std::__1::basic_string"* %887 to %"class.std::__1::__basic_string_common"*
  %889 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %887, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %889, %"class.std::__1::__compressed_pair"** %106, align 8
  %890 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %106, align 8
  store %"class.std::__1::__compressed_pair"* %890, %"class.std::__1::__compressed_pair"** %105, align 8
  %891 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %105, align 8
  %892 = bitcast %"class.std::__1::__compressed_pair"* %891 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %892, %"class.std::__1::__libcpp_compressed_pair_imp"** %104, align 8
  %893 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %104, align 8
  %894 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %893 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %894, %"class.std::__1::allocator"** %103, align 8
  %895 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %103, align 8
  %896 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %893, i32 0, i32 0
  %897 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %896 to i8*
  call void @llvm.memset.p0i8.i64(i8* %897, i8 0, i64 24, i32 8, i1 false) #11
  %898 = load i8*, i8** %108, align 8
  %899 = load i8*, i8** %108, align 8
  %900 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %899)
          to label %901 unwind label %907

; <label>:901:                                    ; preds = %882
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %887, i8* %898, i64 %900)
          to label %902 unwind label %907

; <label>:902:                                    ; preds = %901
  br label %903

; <label>:903:                                    ; preds = %902
  invoke void @_ZN10ParseError5raiseE11ParseErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP5Lexer(i32 1, %"class.std::__1::basic_string"* %221, %class.Lexer* %256)
          to label %904 unwind label %911

; <label>:904:                                    ; preds = %903
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %221) #11
  invoke void @__cxa_end_catch()
          to label %905 unwind label %358

; <label>:905:                                    ; preds = %904
  br label %906

; <label>:906:                                    ; preds = %905
  br label %917

; <label>:907:                                    ; preds = %901, %882
  %908 = landingpad { i8*, i32 }
          cleanup
  %909 = extractvalue { i8*, i32 } %908, 0
  store i8* %909, i8** %201, align 8
  %910 = extractvalue { i8*, i32 } %908, 1
  store i32 %910, i32* %202, align 4
  br label %915

; <label>:911:                                    ; preds = %903
  %912 = landingpad { i8*, i32 }
          cleanup
  %913 = extractvalue { i8*, i32 } %912, 0
  store i8* %913, i8** %201, align 8
  %914 = extractvalue { i8*, i32 } %912, 1
  store i32 %914, i32* %202, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %221) #11
  br label %915

; <label>:915:                                    ; preds = %911, %907
  invoke void @__cxa_end_catch()
          to label %916 unwind label %1628

; <label>:916:                                    ; preds = %915
  br label %1621

; <label>:917:                                    ; preds = %906, %861, %858, %811
  %918 = load i8, i8* %208, align 1
  %919 = sext i8 %918 to i32
  %920 = icmp eq i32 %919, 101
  br i1 %920, label %925, label %921

; <label>:921:                                    ; preds = %917
  %922 = load i8, i8* %208, align 1
  %923 = sext i8 %922 to i32
  %924 = icmp eq i32 %923, 69
  br i1 %924, label %925, label %991

; <label>:925:                                    ; preds = %921, %917
  store %"class.std::__1::basic_string"* %222, %"class.std::__1::basic_string"** %101, align 8
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.4, i32 0, i32 0), i8** %102, align 8
  %926 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %101, align 8
  %927 = load i8*, i8** %102, align 8
  store %"class.std::__1::basic_string"* %926, %"class.std::__1::basic_string"** %99, align 8
  store i8* %927, i8** %100, align 8
  %928 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %99, align 8
  %929 = bitcast %"class.std::__1::basic_string"* %928 to %"class.std::__1::__basic_string_common"*
  %930 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %928, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %930, %"class.std::__1::__compressed_pair"** %98, align 8
  %931 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %98, align 8
  store %"class.std::__1::__compressed_pair"* %931, %"class.std::__1::__compressed_pair"** %97, align 8
  %932 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %97, align 8
  %933 = bitcast %"class.std::__1::__compressed_pair"* %932 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %933, %"class.std::__1::__libcpp_compressed_pair_imp"** %96, align 8
  %934 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %96, align 8
  %935 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %934 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %935, %"class.std::__1::allocator"** %95, align 8
  %936 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %95, align 8
  %937 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %934, i32 0, i32 0
  %938 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %937 to i8*
  call void @llvm.memset.p0i8.i64(i8* %938, i8 0, i64 24, i32 8, i1 false) #11
  %939 = load i8*, i8** %100, align 8
  %940 = load i8*, i8** %100, align 8
  %941 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %940)
          to label %942 unwind label %358

; <label>:942:                                    ; preds = %925
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %928, i8* %939, i64 %941)
          to label %943 unwind label %358

; <label>:943:                                    ; preds = %942
  br label %944

; <label>:944:                                    ; preds = %943
  %945 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %946 unwind label %965

; <label>:946:                                    ; preds = %944
  store i8 %945, i8* %208, align 1
  br label %947

; <label>:947:                                    ; preds = %964, %946
  %948 = load i8, i8* %208, align 1
  %949 = sext i8 %948 to i32
  %950 = icmp eq i32 %949, 45
  br i1 %950, label %955, label %951

; <label>:951:                                    ; preds = %947
  %952 = load i8, i8* %208, align 1
  %953 = invoke zeroext i1 @_ZN5Lexer9is_numberEcb(%class.Lexer* %256, i8 signext %952, i1 zeroext false)
          to label %954 unwind label %965

; <label>:954:                                    ; preds = %951
  br label %955

; <label>:955:                                    ; preds = %954, %947
  %956 = phi i1 [ true, %947 ], [ %953, %954 ]
  br i1 %956, label %957, label %969

; <label>:957:                                    ; preds = %955
  %958 = load i8, i8* %208, align 1
  store %"class.std::__1::basic_string"* %222, %"class.std::__1::basic_string"** %93, align 8
  store i8 %958, i8* %94, align 1
  %959 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %93, align 8
  %960 = load i8, i8* %94, align 1
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE9push_backEc(%"class.std::__1::basic_string"* %959, i8 signext %960)
          to label %961 unwind label %965

; <label>:961:                                    ; preds = %957
  br label %962

; <label>:962:                                    ; preds = %961
  %963 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %964 unwind label %965

; <label>:964:                                    ; preds = %962
  store i8 %963, i8* %208, align 1
  br label %947

; <label>:965:                                    ; preds = %957, %974, %972, %970, %969, %962, %951, %944
  %966 = landingpad { i8*, i32 }
          cleanup
  %967 = extractvalue { i8*, i32 } %966, 0
  store i8* %967, i8** %201, align 8
  %968 = extractvalue { i8*, i32 } %966, 1
  store i32 %968, i32* %202, align 4
  br label %990

; <label>:969:                                    ; preds = %955
  invoke void @_ZN5Lexer11backtrack_cEi(%class.Lexer* %256, i32 1)
          to label %970 unwind label %965

; <label>:970:                                    ; preds = %969
  %971 = invoke double @_ZNSt3__14stodERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPm(%"class.std::__1::basic_string"* dereferenceable(24) %203, i64* null)
          to label %972 unwind label %965

; <label>:972:                                    ; preds = %970
  store double %971, double* %223, align 8
  %973 = invoke i32 @_ZNSt3__14stoiERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPmi(%"class.std::__1::basic_string"* dereferenceable(24) %222, i64* null, i32 10)
          to label %974 unwind label %965

; <label>:974:                                    ; preds = %972
  %975 = sitofp i32 %973 to double
  %976 = call double @llvm.pow.f64(double 1.000000e+01, double %975)
  store double %976, double* %224, align 8
  %977 = load double, double* %223, align 8
  %978 = load double, double* %224, align 8
  %979 = fmul double %977, %978
  invoke void @_ZN7VariantC1Ed(%struct.Variant* %225, double %979)
          to label %980 unwind label %965

; <label>:980:                                    ; preds = %974
  %981 = load i32, i32* %199, align 4
  %982 = load i32, i32* %204, align 4
  %983 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %984 = load i32, i32* %983, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 5, %struct.Variant* %225, i32 %981, i32 %982, i32 %984)
          to label %985 unwind label %986

; <label>:985:                                    ; preds = %980
  call void @_ZN7VariantD1Ev(%struct.Variant* %225) #11
  store i32 1, i32* %207, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %222) #11
  br label %1620

; <label>:986:                                    ; preds = %980
  %987 = landingpad { i8*, i32 }
          cleanup
  %988 = extractvalue { i8*, i32 } %987, 0
  store i8* %988, i8** %201, align 8
  %989 = extractvalue { i8*, i32 } %987, 1
  store i32 %989, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %225) #11
  br label %990

; <label>:990:                                    ; preds = %986, %965
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %222) #11
  br label %1621

; <label>:991:                                    ; preds = %921
  invoke void @_ZN5Lexer11backtrack_cEi(%class.Lexer* %256, i32 1)
          to label %992 unwind label %358

; <label>:992:                                    ; preds = %991
  %993 = load i8, i8* %219, align 1
  %994 = trunc i8 %993 to i1
  br i1 %994, label %1056, label %995

; <label>:995:                                    ; preds = %992
  %996 = invoke signext i8 @_ZN5Lexer14char_lookaheadEv(%class.Lexer* %256)
          to label %997 unwind label %358

; <label>:997:                                    ; preds = %995
  store i8 %996, i8* %208, align 1
  %998 = load i8, i8* %208, align 1
  %999 = sext i8 %998 to i32
  %1000 = icmp eq i32 %999, 108
  br i1 %1000, label %1005, label %1001

; <label>:1001:                                   ; preds = %997
  %1002 = load i8, i8* %208, align 1
  %1003 = sext i8 %1002 to i32
  %1004 = icmp eq i32 %1003, 76
  br i1 %1004, label %1005, label %1020

; <label>:1005:                                   ; preds = %1001, %997
  %1006 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %1007 unwind label %358

; <label>:1007:                                   ; preds = %1005
  %1008 = invoke i64 @_ZNSt3__14stolERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPmi(%"class.std::__1::basic_string"* dereferenceable(24) %203, i64* null, i32 10)
          to label %1009 unwind label %358

; <label>:1009:                                   ; preds = %1007
  invoke void @_ZN7VariantC1El(%struct.Variant* %226, i64 %1008)
          to label %1010 unwind label %358

; <label>:1010:                                   ; preds = %1009
  %1011 = load i32, i32* %199, align 4
  %1012 = load i32, i32* %204, align 4
  %1013 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1014 = load i32, i32* %1013, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 5, %struct.Variant* %226, i32 %1011, i32 %1012, i32 %1014)
          to label %1015 unwind label %1016

; <label>:1015:                                   ; preds = %1010
  call void @_ZN7VariantD1Ev(%struct.Variant* %226) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1016:                                   ; preds = %1010
  %1017 = landingpad { i8*, i32 }
          cleanup
  %1018 = extractvalue { i8*, i32 } %1017, 0
  store i8* %1018, i8** %201, align 8
  %1019 = extractvalue { i8*, i32 } %1017, 1
  store i32 %1019, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %226) #11
  br label %1621

; <label>:1020:                                   ; preds = %1001
  %1021 = load i8, i8* %208, align 1
  %1022 = sext i8 %1021 to i32
  %1023 = icmp eq i32 %1022, 100
  br i1 %1023, label %1028, label %1024

; <label>:1024:                                   ; preds = %1020
  %1025 = load i8, i8* %208, align 1
  %1026 = sext i8 %1025 to i32
  %1027 = icmp eq i32 %1026, 68
  br i1 %1027, label %1028, label %1043

; <label>:1028:                                   ; preds = %1024, %1020
  %1029 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %1030 unwind label %358

; <label>:1030:                                   ; preds = %1028
  %1031 = invoke double @_ZNSt3__14stodERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPm(%"class.std::__1::basic_string"* dereferenceable(24) %203, i64* null)
          to label %1032 unwind label %358

; <label>:1032:                                   ; preds = %1030
  invoke void @_ZN7VariantC1Ed(%struct.Variant* %227, double %1031)
          to label %1033 unwind label %358

; <label>:1033:                                   ; preds = %1032
  %1034 = load i32, i32* %199, align 4
  %1035 = load i32, i32* %204, align 4
  %1036 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1037 = load i32, i32* %1036, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 5, %struct.Variant* %227, i32 %1034, i32 %1035, i32 %1037)
          to label %1038 unwind label %1039

; <label>:1038:                                   ; preds = %1033
  call void @_ZN7VariantD1Ev(%struct.Variant* %227) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1039:                                   ; preds = %1033
  %1040 = landingpad { i8*, i32 }
          cleanup
  %1041 = extractvalue { i8*, i32 } %1040, 0
  store i8* %1041, i8** %201, align 8
  %1042 = extractvalue { i8*, i32 } %1040, 1
  store i32 %1042, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %227) #11
  br label %1621

; <label>:1043:                                   ; preds = %1024
  %1044 = invoke i32 @_ZNSt3__14stoiERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPmi(%"class.std::__1::basic_string"* dereferenceable(24) %203, i64* null, i32 10)
          to label %1045 unwind label %358

; <label>:1045:                                   ; preds = %1043
  invoke void @_ZN7VariantC1Ei(%struct.Variant* %228, i32 %1044)
          to label %1046 unwind label %358

; <label>:1046:                                   ; preds = %1045
  %1047 = load i32, i32* %199, align 4
  %1048 = load i32, i32* %204, align 4
  %1049 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1050 = load i32, i32* %1049, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 5, %struct.Variant* %228, i32 %1047, i32 %1048, i32 %1050)
          to label %1051 unwind label %1052

; <label>:1051:                                   ; preds = %1046
  call void @_ZN7VariantD1Ev(%struct.Variant* %228) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1052:                                   ; preds = %1046
  %1053 = landingpad { i8*, i32 }
          cleanup
  %1054 = extractvalue { i8*, i32 } %1053, 0
  store i8* %1054, i8** %201, align 8
  %1055 = extractvalue { i8*, i32 } %1053, 1
  store i32 %1055, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %228) #11
  br label %1621

; <label>:1056:                                   ; preds = %992
  %1057 = invoke signext i8 @_ZN5Lexer14char_lookaheadEv(%class.Lexer* %256)
          to label %1058 unwind label %358

; <label>:1058:                                   ; preds = %1056
  store i8 %1057, i8* %208, align 1
  %1059 = load i8, i8* %208, align 1
  %1060 = sext i8 %1059 to i32
  %1061 = icmp eq i32 %1060, 102
  br i1 %1061, label %1066, label %1062

; <label>:1062:                                   ; preds = %1058
  %1063 = load i8, i8* %208, align 1
  %1064 = sext i8 %1063 to i32
  %1065 = icmp eq i32 %1064, 70
  br i1 %1065, label %1066, label %1081

; <label>:1066:                                   ; preds = %1062, %1058
  %1067 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %1068 unwind label %358

; <label>:1068:                                   ; preds = %1066
  %1069 = invoke float @_ZNSt3__14stofERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPm(%"class.std::__1::basic_string"* dereferenceable(24) %203, i64* null)
          to label %1070 unwind label %358

; <label>:1070:                                   ; preds = %1068
  invoke void @_ZN7VariantC1Ef(%struct.Variant* %229, float %1069)
          to label %1071 unwind label %358

; <label>:1071:                                   ; preds = %1070
  %1072 = load i32, i32* %199, align 4
  %1073 = load i32, i32* %204, align 4
  %1074 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1075 = load i32, i32* %1074, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 5, %struct.Variant* %229, i32 %1072, i32 %1073, i32 %1075)
          to label %1076 unwind label %1077

; <label>:1076:                                   ; preds = %1071
  call void @_ZN7VariantD1Ev(%struct.Variant* %229) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1077:                                   ; preds = %1071
  %1078 = landingpad { i8*, i32 }
          cleanup
  %1079 = extractvalue { i8*, i32 } %1078, 0
  store i8* %1079, i8** %201, align 8
  %1080 = extractvalue { i8*, i32 } %1078, 1
  store i32 %1080, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %229) #11
  br label %1621

; <label>:1081:                                   ; preds = %1062
  %1082 = load i8, i8* %208, align 1
  %1083 = sext i8 %1082 to i32
  %1084 = icmp eq i32 %1083, 100
  br i1 %1084, label %1089, label %1085

; <label>:1085:                                   ; preds = %1081
  %1086 = load i8, i8* %208, align 1
  %1087 = sext i8 %1086 to i32
  %1088 = icmp eq i32 %1087, 68
  br i1 %1088, label %1089, label %1092

; <label>:1089:                                   ; preds = %1085, %1081
  %1090 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %1091 unwind label %358

; <label>:1091:                                   ; preds = %1089
  br label %1092

; <label>:1092:                                   ; preds = %1091, %1085
  br label %1093

; <label>:1093:                                   ; preds = %1092
  %1094 = invoke double @_ZNSt3__14stodERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPm(%"class.std::__1::basic_string"* dereferenceable(24) %203, i64* null)
          to label %1095 unwind label %358

; <label>:1095:                                   ; preds = %1093
  invoke void @_ZN7VariantC1Ed(%struct.Variant* %230, double %1094)
          to label %1096 unwind label %358

; <label>:1096:                                   ; preds = %1095
  %1097 = load i32, i32* %199, align 4
  %1098 = load i32, i32* %204, align 4
  %1099 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1100 = load i32, i32* %1099, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 5, %struct.Variant* %230, i32 %1097, i32 %1098, i32 %1100)
          to label %1101 unwind label %1102

; <label>:1101:                                   ; preds = %1096
  call void @_ZN7VariantD1Ev(%struct.Variant* %230) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1102:                                   ; preds = %1096
  %1103 = landingpad { i8*, i32 }
          cleanup
  %1104 = extractvalue { i8*, i32 } %1103, 0
  store i8* %1104, i8** %201, align 8
  %1105 = extractvalue { i8*, i32 } %1103, 1
  store i32 %1105, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %230) #11
  br label %1621

; <label>:1106:                                   ; preds = %619
  %1107 = load i8, i8* %205, align 1
  %1108 = invoke zeroext i1 @_ZN5Lexer18is_identifier_charEc(%class.Lexer* %256, i8 signext %1107)
          to label %1109 unwind label %358

; <label>:1109:                                   ; preds = %1106
  br i1 %1108, label %1110, label %1312

; <label>:1110:                                   ; preds = %1109
  %1111 = load i8, i8* %205, align 1
  store i8 %1111, i8* %208, align 1
  br label %1112

; <label>:1112:                                   ; preds = %1123, %1110
  %1113 = load i8, i8* %208, align 1
  %1114 = invoke zeroext i1 @_ZN5Lexer18is_identifier_charEc(%class.Lexer* %256, i8 signext %1113)
          to label %1115 unwind label %358

; <label>:1115:                                   ; preds = %1112
  br i1 %1114, label %1116, label %1124

; <label>:1116:                                   ; preds = %1115
  %1117 = load i8, i8* %208, align 1
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %91, align 8
  store i8 %1117, i8* %92, align 1
  %1118 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %91, align 8
  %1119 = load i8, i8* %92, align 1
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE9push_backEc(%"class.std::__1::basic_string"* %1118, i8 signext %1119)
          to label %1120 unwind label %358

; <label>:1120:                                   ; preds = %1116
  br label %1121

; <label>:1121:                                   ; preds = %1120
  %1122 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %1123 unwind label %358

; <label>:1123:                                   ; preds = %1121
  store i8 %1122, i8* %208, align 1
  br label %1112

; <label>:1124:                                   ; preds = %1115
  invoke void @_ZN5Lexer11backtrack_cEi(%class.Lexer* %256, i32 1)
          to label %1125 unwind label %358

; <label>:1125:                                   ; preds = %1124
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %89, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.8, i32 0, i32 0), i8** %90, align 8
  %1126 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %89, align 8
  %1127 = load i8*, i8** %90, align 8
  %1128 = call i32 @_ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7compareEPKc(%"class.std::__1::basic_string"* %1126, i8* %1127) #11
  %1129 = icmp eq i32 %1128, 0
  br i1 %1129, label %1130, label %1147

; <label>:1130:                                   ; preds = %1125
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %232, %"class.std::__1::basic_string"* dereferenceable(24) %203)
          to label %1131 unwind label %358

; <label>:1131:                                   ; preds = %1130
  invoke void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant* %231, %"class.std::__1::basic_string"* %232)
          to label %1132 unwind label %1138

; <label>:1132:                                   ; preds = %1131
  %1133 = load i32, i32* %199, align 4
  %1134 = load i32, i32* %204, align 4
  %1135 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1136 = load i32, i32* %1135, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 4, %struct.Variant* %231, i32 %1133, i32 %1134, i32 %1136)
          to label %1137 unwind label %1142

; <label>:1137:                                   ; preds = %1132
  call void @_ZN7VariantD1Ev(%struct.Variant* %231) #11
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %232) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1138:                                   ; preds = %1131
  %1139 = landingpad { i8*, i32 }
          cleanup
  %1140 = extractvalue { i8*, i32 } %1139, 0
  store i8* %1140, i8** %201, align 8
  %1141 = extractvalue { i8*, i32 } %1139, 1
  store i32 %1141, i32* %202, align 4
  br label %1146

; <label>:1142:                                   ; preds = %1132
  %1143 = landingpad { i8*, i32 }
          cleanup
  %1144 = extractvalue { i8*, i32 } %1143, 0
  store i8* %1144, i8** %201, align 8
  %1145 = extractvalue { i8*, i32 } %1143, 1
  store i32 %1145, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %231) #11
  br label %1146

; <label>:1146:                                   ; preds = %1142, %1138
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %232) #11
  br label %1621

; <label>:1147:                                   ; preds = %1125
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %87, align 8
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.9, i32 0, i32 0), i8** %88, align 8
  %1148 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %87, align 8
  %1149 = load i8*, i8** %88, align 8
  %1150 = call i32 @_ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7compareEPKc(%"class.std::__1::basic_string"* %1148, i8* %1149) #11
  %1151 = icmp eq i32 %1150, 0
  br i1 %1151, label %1152, label %1169

; <label>:1152:                                   ; preds = %1147
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %234, %"class.std::__1::basic_string"* dereferenceable(24) %203)
          to label %1153 unwind label %358

; <label>:1153:                                   ; preds = %1152
  invoke void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant* %233, %"class.std::__1::basic_string"* %234)
          to label %1154 unwind label %1160

; <label>:1154:                                   ; preds = %1153
  %1155 = load i32, i32* %199, align 4
  %1156 = load i32, i32* %204, align 4
  %1157 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1158 = load i32, i32* %1157, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 4, %struct.Variant* %233, i32 %1155, i32 %1156, i32 %1158)
          to label %1159 unwind label %1164

; <label>:1159:                                   ; preds = %1154
  call void @_ZN7VariantD1Ev(%struct.Variant* %233) #11
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %234) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1160:                                   ; preds = %1153
  %1161 = landingpad { i8*, i32 }
          cleanup
  %1162 = extractvalue { i8*, i32 } %1161, 0
  store i8* %1162, i8** %201, align 8
  %1163 = extractvalue { i8*, i32 } %1161, 1
  store i32 %1163, i32* %202, align 4
  br label %1168

; <label>:1164:                                   ; preds = %1154
  %1165 = landingpad { i8*, i32 }
          cleanup
  %1166 = extractvalue { i8*, i32 } %1165, 0
  store i8* %1166, i8** %201, align 8
  %1167 = extractvalue { i8*, i32 } %1165, 1
  store i32 %1167, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %233) #11
  br label %1168

; <label>:1168:                                   ; preds = %1164, %1160
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %234) #11
  br label %1621

; <label>:1169:                                   ; preds = %1147
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %85, align 8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.10, i32 0, i32 0), i8** %86, align 8
  %1170 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %85, align 8
  %1171 = load i8*, i8** %86, align 8
  %1172 = call i32 @_ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7compareEPKc(%"class.std::__1::basic_string"* %1170, i8* %1171) #11
  %1173 = icmp eq i32 %1172, 0
  br i1 %1173, label %1174, label %1185

; <label>:1174:                                   ; preds = %1169
  invoke void @_ZN7VariantC1Ev(%struct.Variant* %235)
          to label %1175 unwind label %358

; <label>:1175:                                   ; preds = %1174
  %1176 = load i32, i32* %199, align 4
  %1177 = load i32, i32* %204, align 4
  %1178 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1179 = load i32, i32* %1178, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 5, %struct.Variant* %235, i32 %1176, i32 %1177, i32 %1179)
          to label %1180 unwind label %1181

; <label>:1180:                                   ; preds = %1175
  call void @_ZN7VariantD1Ev(%struct.Variant* %235) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1181:                                   ; preds = %1175
  %1182 = landingpad { i8*, i32 }
          cleanup
  %1183 = extractvalue { i8*, i32 } %1182, 0
  store i8* %1183, i8** %201, align 8
  %1184 = extractvalue { i8*, i32 } %1182, 1
  store i32 %1184, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %235) #11
  br label %1621

; <label>:1185:                                   ; preds = %1169
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %236, %"class.std::__1::basic_string"* dereferenceable(24) %203)
          to label %1186 unwind label %358

; <label>:1186:                                   ; preds = %1185
  %1187 = invoke zeroext i1 @_ZN5Lexer10is_keywordENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Lexer* %256, %"class.std::__1::basic_string"* %236)
          to label %1188 unwind label %1197

; <label>:1188:                                   ; preds = %1186
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %236) #11
  br i1 %1187, label %1189, label %1210

; <label>:1189:                                   ; preds = %1188
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %238, %"class.std::__1::basic_string"* dereferenceable(24) %203)
          to label %1190 unwind label %358

; <label>:1190:                                   ; preds = %1189
  invoke void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant* %237, %"class.std::__1::basic_string"* %238)
          to label %1191 unwind label %1201

; <label>:1191:                                   ; preds = %1190
  %1192 = load i32, i32* %199, align 4
  %1193 = load i32, i32* %204, align 4
  %1194 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1195 = load i32, i32* %1194, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 0, %struct.Variant* %237, i32 %1192, i32 %1193, i32 %1195)
          to label %1196 unwind label %1205

; <label>:1196:                                   ; preds = %1191
  call void @_ZN7VariantD1Ev(%struct.Variant* %237) #11
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %238) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1197:                                   ; preds = %1186
  %1198 = landingpad { i8*, i32 }
          cleanup
  %1199 = extractvalue { i8*, i32 } %1198, 0
  store i8* %1199, i8** %201, align 8
  %1200 = extractvalue { i8*, i32 } %1198, 1
  store i32 %1200, i32* %202, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %236) #11
  br label %1621

; <label>:1201:                                   ; preds = %1190
  %1202 = landingpad { i8*, i32 }
          cleanup
  %1203 = extractvalue { i8*, i32 } %1202, 0
  store i8* %1203, i8** %201, align 8
  %1204 = extractvalue { i8*, i32 } %1202, 1
  store i32 %1204, i32* %202, align 4
  br label %1209

; <label>:1205:                                   ; preds = %1191
  %1206 = landingpad { i8*, i32 }
          cleanup
  %1207 = extractvalue { i8*, i32 } %1206, 0
  store i8* %1207, i8** %201, align 8
  %1208 = extractvalue { i8*, i32 } %1206, 1
  store i32 %1208, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %237) #11
  br label %1209

; <label>:1209:                                   ; preds = %1205, %1201
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %238) #11
  br label %1621

; <label>:1210:                                   ; preds = %1188
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %239, %"class.std::__1::basic_string"* dereferenceable(24) %203)
          to label %1211 unwind label %358

; <label>:1211:                                   ; preds = %1210
  %1212 = invoke zeroext i1 @_ZN5Lexer15is_type_keywordENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Lexer* %256, %"class.std::__1::basic_string"* %239)
          to label %1213 unwind label %1222

; <label>:1213:                                   ; preds = %1211
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %239) #11
  br i1 %1212, label %1214, label %1235

; <label>:1214:                                   ; preds = %1213
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %241, %"class.std::__1::basic_string"* dereferenceable(24) %203)
          to label %1215 unwind label %358

; <label>:1215:                                   ; preds = %1214
  invoke void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant* %240, %"class.std::__1::basic_string"* %241)
          to label %1216 unwind label %1226

; <label>:1216:                                   ; preds = %1215
  %1217 = load i32, i32* %199, align 4
  %1218 = load i32, i32* %204, align 4
  %1219 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1220 = load i32, i32* %1219, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 2, %struct.Variant* %240, i32 %1217, i32 %1218, i32 %1220)
          to label %1221 unwind label %1230

; <label>:1221:                                   ; preds = %1216
  call void @_ZN7VariantD1Ev(%struct.Variant* %240) #11
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %241) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1222:                                   ; preds = %1211
  %1223 = landingpad { i8*, i32 }
          cleanup
  %1224 = extractvalue { i8*, i32 } %1223, 0
  store i8* %1224, i8** %201, align 8
  %1225 = extractvalue { i8*, i32 } %1223, 1
  store i32 %1225, i32* %202, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %239) #11
  br label %1621

; <label>:1226:                                   ; preds = %1215
  %1227 = landingpad { i8*, i32 }
          cleanup
  %1228 = extractvalue { i8*, i32 } %1227, 0
  store i8* %1228, i8** %201, align 8
  %1229 = extractvalue { i8*, i32 } %1227, 1
  store i32 %1229, i32* %202, align 4
  br label %1234

; <label>:1230:                                   ; preds = %1216
  %1231 = landingpad { i8*, i32 }
          cleanup
  %1232 = extractvalue { i8*, i32 } %1231, 0
  store i8* %1232, i8** %201, align 8
  %1233 = extractvalue { i8*, i32 } %1231, 1
  store i32 %1233, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %240) #11
  br label %1234

; <label>:1234:                                   ; preds = %1230, %1226
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %241) #11
  br label %1621

; <label>:1235:                                   ; preds = %1213
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %242, %"class.std::__1::basic_string"* dereferenceable(24) %203)
          to label %1236 unwind label %358

; <label>:1236:                                   ; preds = %1235
  %1237 = invoke zeroext i1 @_ZN5Lexer15is_bool_literalENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Lexer* %256, %"class.std::__1::basic_string"* %242)
          to label %1238 unwind label %1250

; <label>:1238:                                   ; preds = %1236
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %242) #11
  br i1 %1237, label %1239, label %1258

; <label>:1239:                                   ; preds = %1238
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %83, align 8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.2, i32 0, i32 0), i8** %84, align 8
  %1240 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %83, align 8
  %1241 = load i8*, i8** %84, align 8
  %1242 = call i32 @_ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7compareEPKc(%"class.std::__1::basic_string"* %1240, i8* %1241) #11
  %1243 = icmp eq i32 %1242, 0
  invoke void @_ZN7VariantC1Eb(%struct.Variant* %243, i1 zeroext %1243)
          to label %1244 unwind label %358

; <label>:1244:                                   ; preds = %1239
  %1245 = load i32, i32* %199, align 4
  %1246 = load i32, i32* %204, align 4
  %1247 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1248 = load i32, i32* %1247, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 5, %struct.Variant* %243, i32 %1245, i32 %1246, i32 %1248)
          to label %1249 unwind label %1254

; <label>:1249:                                   ; preds = %1244
  call void @_ZN7VariantD1Ev(%struct.Variant* %243) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1250:                                   ; preds = %1236
  %1251 = landingpad { i8*, i32 }
          cleanup
  %1252 = extractvalue { i8*, i32 } %1251, 0
  store i8* %1252, i8** %201, align 8
  %1253 = extractvalue { i8*, i32 } %1251, 1
  store i32 %1253, i32* %202, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %242) #11
  br label %1621

; <label>:1254:                                   ; preds = %1244
  %1255 = landingpad { i8*, i32 }
          cleanup
  %1256 = extractvalue { i8*, i32 } %1255, 0
  store i8* %1256, i8** %201, align 8
  %1257 = extractvalue { i8*, i32 } %1255, 1
  store i32 %1257, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %243) #11
  br label %1621

; <label>:1258:                                   ; preds = %1238
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %81, align 8
  store i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.11, i32 0, i32 0), i8** %82, align 8
  %1259 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %81, align 8
  %1260 = load i8*, i8** %82, align 8
  %1261 = call i32 @_ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7compareEPKc(%"class.std::__1::basic_string"* %1259, i8* %1260) #11
  %1262 = icmp eq i32 %1261, 0
  br i1 %1262, label %1263, label %1288

; <label>:1263:                                   ; preds = %1258
  store %"class.std::__1::basic_string"* %244, %"class.std::__1::basic_string"** %79, align 8
  store i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.12, i32 0, i32 0), i8** %80, align 8
  %1264 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %79, align 8
  %1265 = load i8*, i8** %80, align 8
  store %"class.std::__1::basic_string"* %1264, %"class.std::__1::basic_string"** %77, align 8
  store i8* %1265, i8** %78, align 8
  %1266 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %77, align 8
  %1267 = bitcast %"class.std::__1::basic_string"* %1266 to %"class.std::__1::__basic_string_common"*
  %1268 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1266, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1268, %"class.std::__1::__compressed_pair"** %76, align 8
  %1269 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %76, align 8
  store %"class.std::__1::__compressed_pair"* %1269, %"class.std::__1::__compressed_pair"** %75, align 8
  %1270 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %75, align 8
  %1271 = bitcast %"class.std::__1::__compressed_pair"* %1270 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1271, %"class.std::__1::__libcpp_compressed_pair_imp"** %74, align 8
  %1272 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %74, align 8
  %1273 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %1272 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %1273, %"class.std::__1::allocator"** %73, align 8
  %1274 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %73, align 8
  %1275 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1272, i32 0, i32 0
  %1276 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1275 to i8*
  call void @llvm.memset.p0i8.i64(i8* %1276, i8 0, i64 24, i32 8, i1 false) #11
  %1277 = load i8*, i8** %78, align 8
  %1278 = load i8*, i8** %78, align 8
  %1279 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %1278)
          to label %1280 unwind label %358

; <label>:1280:                                   ; preds = %1263
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %1266, i8* %1277, i64 %1279)
          to label %1281 unwind label %358

; <label>:1281:                                   ; preds = %1280
  br label %1282

; <label>:1282:                                   ; preds = %1281
  invoke void @_ZN10ParseError5raiseE11ParseErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP5Lexer(i32 0, %"class.std::__1::basic_string"* %244, %class.Lexer* %256)
          to label %1283 unwind label %1284

; <label>:1283:                                   ; preds = %1282
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %244) #11
  br label %1305

; <label>:1284:                                   ; preds = %1282
  %1285 = landingpad { i8*, i32 }
          cleanup
  %1286 = extractvalue { i8*, i32 } %1285, 0
  store i8* %1286, i8** %201, align 8
  %1287 = extractvalue { i8*, i32 } %1285, 1
  store i32 %1287, i32* %202, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %244) #11
  br label %1621

; <label>:1288:                                   ; preds = %1258
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %246, %"class.std::__1::basic_string"* dereferenceable(24) %203)
          to label %1289 unwind label %358

; <label>:1289:                                   ; preds = %1288
  invoke void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant* %245, %"class.std::__1::basic_string"* %246)
          to label %1290 unwind label %1296

; <label>:1290:                                   ; preds = %1289
  %1291 = load i32, i32* %199, align 4
  %1292 = load i32, i32* %204, align 4
  %1293 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1294 = load i32, i32* %1293, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 2, %struct.Variant* %245, i32 %1291, i32 %1292, i32 %1294)
          to label %1295 unwind label %1300

; <label>:1295:                                   ; preds = %1290
  call void @_ZN7VariantD1Ev(%struct.Variant* %245) #11
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %246) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1296:                                   ; preds = %1289
  %1297 = landingpad { i8*, i32 }
          cleanup
  %1298 = extractvalue { i8*, i32 } %1297, 0
  store i8* %1298, i8** %201, align 8
  %1299 = extractvalue { i8*, i32 } %1297, 1
  store i32 %1299, i32* %202, align 4
  br label %1304

; <label>:1300:                                   ; preds = %1290
  %1301 = landingpad { i8*, i32 }
          cleanup
  %1302 = extractvalue { i8*, i32 } %1301, 0
  store i8* %1302, i8** %201, align 8
  %1303 = extractvalue { i8*, i32 } %1301, 1
  store i32 %1303, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %245) #11
  br label %1304

; <label>:1304:                                   ; preds = %1300, %1296
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %246) #11
  br label %1621

; <label>:1305:                                   ; preds = %1283
  br label %1306

; <label>:1306:                                   ; preds = %1305
  br label %1307

; <label>:1307:                                   ; preds = %1306
  br label %1308

; <label>:1308:                                   ; preds = %1307
  br label %1309

; <label>:1309:                                   ; preds = %1308
  br label %1310

; <label>:1310:                                   ; preds = %1309
  br label %1311

; <label>:1311:                                   ; preds = %1310
  br label %1605

; <label>:1312:                                   ; preds = %1109
  %1313 = load i8, i8* %205, align 1
  %1314 = invoke zeroext i1 @_ZN5Lexer16is_operator_charEc(%class.Lexer* %256, i8 signext %1313)
          to label %1315 unwind label %358

; <label>:1315:                                   ; preds = %1312
  br i1 %1314, label %1316, label %1520

; <label>:1316:                                   ; preds = %1315
  br label %1317

; <label>:1317:                                   ; preds = %1328, %1316
  %1318 = load i8, i8* %205, align 1
  %1319 = invoke zeroext i1 @_ZN5Lexer16is_operator_charEc(%class.Lexer* %256, i8 signext %1318)
          to label %1320 unwind label %358

; <label>:1320:                                   ; preds = %1317
  br i1 %1319, label %1321, label %1329

; <label>:1321:                                   ; preds = %1320
  %1322 = load i8, i8* %205, align 1
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %71, align 8
  store i8 %1322, i8* %72, align 1
  %1323 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %71, align 8
  %1324 = load i8, i8* %72, align 1
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE9push_backEc(%"class.std::__1::basic_string"* %1323, i8 signext %1324)
          to label %1325 unwind label %358

; <label>:1325:                                   ; preds = %1321
  br label %1326

; <label>:1326:                                   ; preds = %1325
  %1327 = invoke signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %256)
          to label %1328 unwind label %358

; <label>:1328:                                   ; preds = %1326
  store i8 %1327, i8* %205, align 1
  br label %1317

; <label>:1329:                                   ; preds = %1320
  invoke void @_ZN5Lexer11backtrack_cEi(%class.Lexer* %256, i32 1)
          to label %1330 unwind label %358

; <label>:1330:                                   ; preds = %1329
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %69, align 8
  store i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.13, i32 0, i32 0), i8** %70, align 8
  %1331 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %69, align 8
  %1332 = load i8*, i8** %70, align 8
  %1333 = call i32 @_ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7compareEPKc(%"class.std::__1::basic_string"* %1331, i8* %1332) #11
  %1334 = icmp eq i32 %1333, 0
  br i1 %1334, label %1335, label %1346

; <label>:1335:                                   ; preds = %1330
  invoke void @_ZN7VariantC1Ec(%struct.Variant* %247, i8 signext 46)
          to label %1336 unwind label %358

; <label>:1336:                                   ; preds = %1335
  %1337 = load i32, i32* %199, align 4
  %1338 = load i32, i32* %204, align 4
  %1339 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1340 = load i32, i32* %1339, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 3, %struct.Variant* %247, i32 %1337, i32 %1338, i32 %1340)
          to label %1341 unwind label %1342

; <label>:1341:                                   ; preds = %1336
  call void @_ZN7VariantD1Ev(%struct.Variant* %247) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1342:                                   ; preds = %1336
  %1343 = landingpad { i8*, i32 }
          cleanup
  %1344 = extractvalue { i8*, i32 } %1343, 0
  store i8* %1344, i8** %201, align 8
  %1345 = extractvalue { i8*, i32 } %1343, 1
  store i32 %1345, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %247) #11
  br label %1621

; <label>:1346:                                   ; preds = %1330
  br label %1347

; <label>:1347:                                   ; preds = %1498, %1346
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %248, %"class.std::__1::basic_string"* dereferenceable(24) %203)
          to label %1348 unwind label %358

; <label>:1348:                                   ; preds = %1347
  %1349 = invoke zeroext i1 @_ZN5Lexer11is_operatorENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Lexer* %256, %"class.std::__1::basic_string"* %248)
          to label %1350 unwind label %1499

; <label>:1350:                                   ; preds = %1348
  br i1 %1349, label %1397, label %1351

; <label>:1351:                                   ; preds = %1350
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %68, align 8
  %1352 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8
  store %"class.std::__1::basic_string"* %1352, %"class.std::__1::basic_string"** %67, align 8
  %1353 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %67, align 8
  store %"class.std::__1::basic_string"* %1353, %"class.std::__1::basic_string"** %66, align 8
  %1354 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8
  %1355 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1354, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1355, %"class.std::__1::__compressed_pair"** %65, align 8
  %1356 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %65, align 8
  %1357 = bitcast %"class.std::__1::__compressed_pair"* %1356 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1357, %"class.std::__1::__libcpp_compressed_pair_imp"** %64, align 8
  %1358 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %64, align 8
  %1359 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1358, i32 0, i32 0
  %1360 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1359, i32 0, i32 0
  %1361 = bitcast %union.anon.0* %1360 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1362 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1361, i32 0, i32 0
  %1363 = bitcast %union.anon.1* %1362 to i8*
  %1364 = load i8, i8* %1363, align 8
  %1365 = zext i8 %1364 to i32
  %1366 = and i32 %1365, 1
  %1367 = icmp ne i32 %1366, 0
  br i1 %1367, label %1368, label %1379

; <label>:1368:                                   ; preds = %1351
  store %"class.std::__1::basic_string"* %1353, %"class.std::__1::basic_string"** %60, align 8
  %1369 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %60, align 8
  %1370 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1369, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1370, %"class.std::__1::__compressed_pair"** %59, align 8
  %1371 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %59, align 8
  %1372 = bitcast %"class.std::__1::__compressed_pair"* %1371 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1372, %"class.std::__1::__libcpp_compressed_pair_imp"** %58, align 8
  %1373 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %58, align 8
  %1374 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1373, i32 0, i32 0
  %1375 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1374, i32 0, i32 0
  %1376 = bitcast %union.anon.0* %1375 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1377 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1376, i32 0, i32 1
  %1378 = load i64, i64* %1377, align 8
  br label %1394

; <label>:1379:                                   ; preds = %1351
  store %"class.std::__1::basic_string"* %1353, %"class.std::__1::basic_string"** %63, align 8
  %1380 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8
  %1381 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1380, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1381, %"class.std::__1::__compressed_pair"** %62, align 8
  %1382 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %62, align 8
  %1383 = bitcast %"class.std::__1::__compressed_pair"* %1382 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1383, %"class.std::__1::__libcpp_compressed_pair_imp"** %61, align 8
  %1384 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %61, align 8
  %1385 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1384, i32 0, i32 0
  %1386 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1385, i32 0, i32 0
  %1387 = bitcast %union.anon.0* %1386 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1388 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1387, i32 0, i32 0
  %1389 = bitcast %union.anon.1* %1388 to i8*
  %1390 = load i8, i8* %1389, align 8
  %1391 = zext i8 %1390 to i32
  %1392 = ashr i32 %1391, 1
  %1393 = sext i32 %1392 to i64
  br label %1394

; <label>:1394:                                   ; preds = %1368, %1379
  %1395 = phi i64 [ %1378, %1368 ], [ %1393, %1379 ]
  %1396 = icmp ugt i64 %1395, 0
  br label %1397

; <label>:1397:                                   ; preds = %1394, %1350
  %1398 = phi i1 [ false, %1350 ], [ %1396, %1394 ]
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %248) #11
  br i1 %1398, label %1399, label %1503

; <label>:1399:                                   ; preds = %1397
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %54, align 8
  %1400 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8
  store %"class.std::__1::basic_string"* %1400, %"class.std::__1::basic_string"** %53, align 8
  %1401 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8
  %1402 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1401, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1402, %"class.std::__1::__compressed_pair"** %52, align 8
  %1403 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %52, align 8
  %1404 = bitcast %"class.std::__1::__compressed_pair"* %1403 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1404, %"class.std::__1::__libcpp_compressed_pair_imp"** %51, align 8
  %1405 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %51, align 8
  %1406 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1405, i32 0, i32 0
  %1407 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1406, i32 0, i32 0
  %1408 = bitcast %union.anon.0* %1407 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1409 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1408, i32 0, i32 0
  %1410 = bitcast %union.anon.1* %1409 to i8*
  %1411 = load i8, i8* %1410, align 8
  %1412 = zext i8 %1411 to i32
  %1413 = and i32 %1412, 1
  %1414 = icmp ne i32 %1413, 0
  br i1 %1414, label %1415, label %1450

; <label>:1415:                                   ; preds = %1399
  store %"class.std::__1::basic_string"* %1400, %"class.std::__1::basic_string"** %48, align 8
  %1416 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %48, align 8
  %1417 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1416, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1417, %"class.std::__1::__compressed_pair"** %47, align 8
  %1418 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %47, align 8
  %1419 = bitcast %"class.std::__1::__compressed_pair"* %1418 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1419, %"class.std::__1::__libcpp_compressed_pair_imp"** %46, align 8
  %1420 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %46, align 8
  %1421 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1420, i32 0, i32 0
  %1422 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1421, i32 0, i32 0
  %1423 = bitcast %union.anon.0* %1422 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1424 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1423, i32 0, i32 1
  %1425 = load i64, i64* %1424, align 8
  %1426 = sub i64 %1425, 1
  store i64 %1426, i64* %55, align 8
  %1427 = load i64, i64* %55, align 8
  store %"class.std::__1::basic_string"* %1400, %"class.std::__1::basic_string"** %32, align 8
  store i64 %1427, i64* %33, align 8
  %1428 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %32, align 8
  %1429 = load i64, i64* %33, align 8
  %1430 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1428, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1430, %"class.std::__1::__compressed_pair"** %31, align 8
  %1431 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %31, align 8
  %1432 = bitcast %"class.std::__1::__compressed_pair"* %1431 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1432, %"class.std::__1::__libcpp_compressed_pair_imp"** %30, align 8
  %1433 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %30, align 8
  %1434 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1433, i32 0, i32 0
  %1435 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1434, i32 0, i32 0
  %1436 = bitcast %union.anon.0* %1435 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1437 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1436, i32 0, i32 1
  store i64 %1429, i64* %1437, align 8
  store %"class.std::__1::basic_string"* %1400, %"class.std::__1::basic_string"** %29, align 8
  %1438 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %29, align 8
  %1439 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1438, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1439, %"class.std::__1::__compressed_pair"** %28, align 8
  %1440 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %28, align 8
  %1441 = bitcast %"class.std::__1::__compressed_pair"* %1440 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1441, %"class.std::__1::__libcpp_compressed_pair_imp"** %27, align 8
  %1442 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %27, align 8
  %1443 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1442, i32 0, i32 0
  %1444 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1443, i32 0, i32 0
  %1445 = bitcast %union.anon.0* %1444 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1446 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1445, i32 0, i32 2
  %1447 = load i8*, i8** %1446, align 8
  %1448 = load i64, i64* %55, align 8
  %1449 = getelementptr inbounds i8, i8* %1447, i64 %1448
  store i8 0, i8* %56, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %1449, i8* dereferenceable(1) %56) #11
  br label %1494

; <label>:1450:                                   ; preds = %1399
  store %"class.std::__1::basic_string"* %1400, %"class.std::__1::basic_string"** %36, align 8
  %1451 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8
  %1452 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1451, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1452, %"class.std::__1::__compressed_pair"** %35, align 8
  %1453 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8
  %1454 = bitcast %"class.std::__1::__compressed_pair"* %1453 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1454, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8
  %1455 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8
  %1456 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1455, i32 0, i32 0
  %1457 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1456, i32 0, i32 0
  %1458 = bitcast %union.anon.0* %1457 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1459 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1458, i32 0, i32 0
  %1460 = bitcast %union.anon.1* %1459 to i8*
  %1461 = load i8, i8* %1460, align 8
  %1462 = zext i8 %1461 to i32
  %1463 = ashr i32 %1462, 1
  %1464 = sext i32 %1463 to i64
  %1465 = sub i64 %1464, 1
  store i64 %1465, i64* %55, align 8
  %1466 = load i64, i64* %55, align 8
  store %"class.std::__1::basic_string"* %1400, %"class.std::__1::basic_string"** %39, align 8
  store i64 %1466, i64* %40, align 8
  %1467 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8
  %1468 = load i64, i64* %40, align 8
  %1469 = shl i64 %1468, 1
  %1470 = trunc i64 %1469 to i8
  %1471 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1467, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1471, %"class.std::__1::__compressed_pair"** %38, align 8
  %1472 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %38, align 8
  %1473 = bitcast %"class.std::__1::__compressed_pair"* %1472 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1473, %"class.std::__1::__libcpp_compressed_pair_imp"** %37, align 8
  %1474 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %37, align 8
  %1475 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1474, i32 0, i32 0
  %1476 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1475, i32 0, i32 0
  %1477 = bitcast %union.anon.0* %1476 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1478 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1477, i32 0, i32 0
  %1479 = bitcast %union.anon.1* %1478 to i8*
  store i8 %1470, i8* %1479, align 8
  store %"class.std::__1::basic_string"* %1400, %"class.std::__1::basic_string"** %45, align 8
  %1480 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8
  %1481 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1480, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1481, %"class.std::__1::__compressed_pair"** %44, align 8
  %1482 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8
  %1483 = bitcast %"class.std::__1::__compressed_pair"* %1482 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1483, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8
  %1484 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8
  %1485 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1484, i32 0, i32 0
  %1486 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1485, i32 0, i32 0
  %1487 = bitcast %union.anon.0* %1486 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1488 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1487, i32 0, i32 1
  %1489 = getelementptr inbounds [23 x i8], [23 x i8]* %1488, i64 0, i64 0
  store i8* %1489, i8** %42, align 8
  %1490 = load i8*, i8** %42, align 8
  store i8* %1490, i8** %41, align 8
  %1491 = load i8*, i8** %41, align 8
  %1492 = load i64, i64* %55, align 8
  %1493 = getelementptr inbounds i8, i8* %1491, i64 %1492
  store i8 0, i8* %57, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %1493, i8* dereferenceable(1) %57) #11
  br label %1494

; <label>:1494:                                   ; preds = %1415, %1450
  %1495 = load i64, i64* %55, align 8
  store %"class.std::__1::basic_string"* %1400, %"class.std::__1::basic_string"** %49, align 8
  store i64 %1495, i64* %50, align 8
  %1496 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8
  br label %1497

; <label>:1497:                                   ; preds = %1494
  invoke void @_ZN5Lexer11backtrack_cEi(%class.Lexer* %256, i32 1)
          to label %1498 unwind label %358

; <label>:1498:                                   ; preds = %1497
  br label %1347

; <label>:1499:                                   ; preds = %1348
  %1500 = landingpad { i8*, i32 }
          cleanup
  %1501 = extractvalue { i8*, i32 } %1500, 0
  store i8* %1501, i8** %201, align 8
  %1502 = extractvalue { i8*, i32 } %1500, 1
  store i32 %1502, i32* %202, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %248) #11
  br label %1621

; <label>:1503:                                   ; preds = %1397
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %250, %"class.std::__1::basic_string"* dereferenceable(24) %203)
          to label %1504 unwind label %358

; <label>:1504:                                   ; preds = %1503
  invoke void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant* %249, %"class.std::__1::basic_string"* %250)
          to label %1505 unwind label %1511

; <label>:1505:                                   ; preds = %1504
  %1506 = load i32, i32* %199, align 4
  %1507 = load i32, i32* %204, align 4
  %1508 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1509 = load i32, i32* %1508, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 4, %struct.Variant* %249, i32 %1506, i32 %1507, i32 %1509)
          to label %1510 unwind label %1515

; <label>:1510:                                   ; preds = %1505
  call void @_ZN7VariantD1Ev(%struct.Variant* %249) #11
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %250) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1511:                                   ; preds = %1504
  %1512 = landingpad { i8*, i32 }
          cleanup
  %1513 = extractvalue { i8*, i32 } %1512, 0
  store i8* %1513, i8** %201, align 8
  %1514 = extractvalue { i8*, i32 } %1512, 1
  store i32 %1514, i32* %202, align 4
  br label %1519

; <label>:1515:                                   ; preds = %1505
  %1516 = landingpad { i8*, i32 }
          cleanup
  %1517 = extractvalue { i8*, i32 } %1516, 0
  store i8* %1517, i8** %201, align 8
  %1518 = extractvalue { i8*, i32 } %1516, 1
  store i32 %1518, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %249) #11
  br label %1519

; <label>:1519:                                   ; preds = %1515, %1511
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %250) #11
  br label %1621

; <label>:1520:                                   ; preds = %1315
  %1521 = load i8, i8* %205, align 1
  %1522 = invoke zeroext i1 @_ZN5Lexer13is_punctuatorEc(%class.Lexer* %256, i8 signext %1521)
          to label %1523 unwind label %358

; <label>:1523:                                   ; preds = %1520
  br i1 %1522, label %1524, label %1536

; <label>:1524:                                   ; preds = %1523
  %1525 = load i8, i8* %205, align 1
  invoke void @_ZN7VariantC1Ec(%struct.Variant* %251, i8 signext %1525)
          to label %1526 unwind label %358

; <label>:1526:                                   ; preds = %1524
  %1527 = load i32, i32* %199, align 4
  %1528 = load i32, i32* %204, align 4
  %1529 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1530 = load i32, i32* %1529, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 3, %struct.Variant* %251, i32 %1527, i32 %1528, i32 %1530)
          to label %1531 unwind label %1532

; <label>:1531:                                   ; preds = %1526
  call void @_ZN7VariantD1Ev(%struct.Variant* %251) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1532:                                   ; preds = %1526
  %1533 = landingpad { i8*, i32 }
          cleanup
  %1534 = extractvalue { i8*, i32 } %1533, 0
  store i8* %1534, i8** %201, align 8
  %1535 = extractvalue { i8*, i32 } %1533, 1
  store i32 %1535, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %251) #11
  br label %1621

; <label>:1536:                                   ; preds = %1523
  %1537 = load i8, i8* %205, align 1
  store %"class.std::__1::basic_string"* %253, %"class.std::__1::basic_string"** %24, align 8
  store i64 1, i64* %25, align 8
  store i8 %1537, i8* %26, align 1
  %1538 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8
  %1539 = load i64, i64* %25, align 8
  %1540 = load i8, i8* %26, align 1
  store %"class.std::__1::basic_string"* %1538, %"class.std::__1::basic_string"** %21, align 8
  store i64 %1539, i64* %22, align 8
  store i8 %1540, i8* %23, align 1
  %1541 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %21, align 8
  %1542 = bitcast %"class.std::__1::basic_string"* %1541 to %"class.std::__1::__basic_string_common"*
  %1543 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1541, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1543, %"class.std::__1::__compressed_pair"** %20, align 8
  %1544 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %20, align 8
  store %"class.std::__1::__compressed_pair"* %1544, %"class.std::__1::__compressed_pair"** %19, align 8
  %1545 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8
  %1546 = bitcast %"class.std::__1::__compressed_pair"* %1545 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1546, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8
  %1547 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8
  %1548 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %1547 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %1548, %"class.std::__1::allocator"** %17, align 8
  %1549 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %17, align 8
  %1550 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1547, i32 0, i32 0
  %1551 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1550 to i8*
  call void @llvm.memset.p0i8.i64(i8* %1551, i8 0, i64 24, i32 8, i1 false) #11
  %1552 = load i64, i64* %22, align 8
  %1553 = load i8, i8* %23, align 1
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEmc(%"class.std::__1::basic_string"* %1541, i64 %1552, i8 signext %1553)
          to label %1554 unwind label %358

; <label>:1554:                                   ; preds = %1536
  br label %1555

; <label>:1555:                                   ; preds = %1554
  store i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.14, i32 0, i32 0), i8** %15, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %253, %"class.std::__1::basic_string"** %16, align 8, !noalias !2
  %1556 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %16, align 8, !noalias !2
  %1557 = load i8*, i8** %15, align 8, !noalias !2
  %1558 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %1556, i64 0, i8* %1557)
          to label %1559 unwind label %1594

; <label>:1559:                                   ; preds = %1555
  store %"class.std::__1::basic_string"* %1558, %"class.std::__1::basic_string"** %14, align 8, !noalias !2
  %1560 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %252, %"class.std::__1::basic_string"** %12, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %1560, %"class.std::__1::basic_string"** %13, align 8, !noalias !2
  %1561 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !2
  %1562 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %1561, %"class.std::__1::basic_string"** %10, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %1562, %"class.std::__1::basic_string"** %11, align 8, !noalias !2
  %1563 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !2
  %1564 = bitcast %"class.std::__1::basic_string"* %1563 to %"class.std::__1::__basic_string_common"*
  %1565 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1563, i32 0, i32 0
  %1566 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !2
  %1567 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1566, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1567, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !2
  %1568 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !2
  %1569 = bitcast %"class.std::__1::__compressed_pair"* %1565 to i8*
  %1570 = bitcast %"class.std::__1::__compressed_pair"* %1568 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1569, i8* %1570, i64 24, i32 8, i1 false) #11
  %1571 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %1571, %"class.std::__1::basic_string"** %6, align 8, !noalias !2
  %1572 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !2
  %1573 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1572, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1573, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !2
  %1574 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !2
  %1575 = bitcast %"class.std::__1::__compressed_pair"* %1574 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1575, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !2
  %1576 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !2
  %1577 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1576, i32 0, i32 0
  %1578 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1577, i32 0, i32 0
  %1579 = bitcast %union.anon.0* %1578 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %1580 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %1579, i32 0, i32 0
  store [3 x i64]* %1580, [3 x i64]** %7, align 8, !noalias !2
  store i32 0, i32* %8, align 4, !noalias !2
  br label %1581

; <label>:1581:                                   ; preds = %1584, %1559
  %1582 = load i32, i32* %8, align 4, !noalias !2
  %1583 = icmp ult i32 %1582, 3
  br i1 %1583, label %1584, label %1591

; <label>:1584:                                   ; preds = %1581
  %1585 = load i32, i32* %8, align 4, !noalias !2
  %1586 = zext i32 %1585 to i64
  %1587 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !2
  %1588 = getelementptr inbounds [3 x i64], [3 x i64]* %1587, i64 0, i64 %1586
  store i64 0, i64* %1588, align 8
  %1589 = load i32, i32* %8, align 4, !noalias !2
  %1590 = add i32 %1589, 1
  store i32 %1590, i32* %8, align 4, !noalias !2
  br label %1581

; <label>:1591:                                   ; preds = %1581
  br label %1592

; <label>:1592:                                   ; preds = %1591
  invoke void @_ZN10ParseError5raiseE11ParseErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP5Lexer(i32 1, %"class.std::__1::basic_string"* %252, %class.Lexer* %256)
          to label %1593 unwind label %1598

; <label>:1593:                                   ; preds = %1592
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %252) #11
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %253) #11
  br label %1603

; <label>:1594:                                   ; preds = %1555
  %1595 = landingpad { i8*, i32 }
          cleanup
  %1596 = extractvalue { i8*, i32 } %1595, 0
  store i8* %1596, i8** %201, align 8
  %1597 = extractvalue { i8*, i32 } %1595, 1
  store i32 %1597, i32* %202, align 4
  br label %1602

; <label>:1598:                                   ; preds = %1592
  %1599 = landingpad { i8*, i32 }
          cleanup
  %1600 = extractvalue { i8*, i32 } %1599, 0
  store i8* %1600, i8** %201, align 8
  %1601 = extractvalue { i8*, i32 } %1599, 1
  store i32 %1601, i32* %202, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %252) #11
  br label %1602

; <label>:1602:                                   ; preds = %1598, %1594
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %253) #11
  br label %1621

; <label>:1603:                                   ; preds = %1593
  br label %1604

; <label>:1604:                                   ; preds = %1603
  br label %1605

; <label>:1605:                                   ; preds = %1604, %1311
  br label %1606

; <label>:1606:                                   ; preds = %1605
  br label %1607

; <label>:1607:                                   ; preds = %1606
  br label %1608

; <label>:1608:                                   ; preds = %1607
  invoke void @_ZN7VariantC1Ev(%struct.Variant* %254)
          to label %1609 unwind label %358

; <label>:1609:                                   ; preds = %1608
  %1610 = load i32, i32* %199, align 4
  %1611 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1612 = load i32, i32* %1611, align 8
  %1613 = getelementptr inbounds %class.Lexer, %class.Lexer* %256, i32 0, i32 0
  %1614 = load i32, i32* %1613, align 8
  invoke void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token* %0, i32 7, %struct.Variant* %254, i32 %1610, i32 %1612, i32 %1614)
          to label %1615 unwind label %1616

; <label>:1615:                                   ; preds = %1609
  call void @_ZN7VariantD1Ev(%struct.Variant* %254) #11
  store i32 1, i32* %207, align 4
  br label %1620

; <label>:1616:                                   ; preds = %1609
  %1617 = landingpad { i8*, i32 }
          cleanup
  %1618 = extractvalue { i8*, i32 } %1617, 0
  store i8* %1618, i8** %201, align 8
  %1619 = extractvalue { i8*, i32 } %1617, 1
  store i32 %1619, i32* %202, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %254) #11
  br label %1621

; <label>:1620:                                   ; preds = %1615, %1531, %1510, %1341, %1295, %1249, %1221, %1196, %1180, %1159, %1137, %1101, %1076, %1051, %1038, %1015, %985, %873, %752, %681, %606, %508, %451, %436, %423
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %203) #11
  br label %1622

; <label>:1621:                                   ; preds = %1616, %1602, %1532, %1519, %1499, %1342, %1304, %1284, %1254, %1250, %1234, %1222, %1209, %1197, %1181, %1168, %1146, %1102, %1077, %1052, %1039, %1016, %990, %916, %757, %686, %615, %509, %497, %424, %358
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %203) #11
  br label %1623

; <label>:1622:                                   ; preds = %1620, %323, %260
  ret void

; <label>:1623:                                   ; preds = %1621, %324
  %1624 = load i8*, i8** %201, align 8
  %1625 = load i32, i32* %202, align 4
  %1626 = insertvalue { i8*, i32 } undef, i8* %1624, 0
  %1627 = insertvalue { i8*, i32 } %1626, i32 %1625, 1
  resume { i8*, i32 } %1627

; <label>:1628:                                   ; preds = %915
  %1629 = landingpad { i8*, i32 }
          catch i8* null
  %1630 = extractvalue { i8*, i32 } %1629, 0
  call void @__clang_call_terminate(i8* %1630) #12
  unreachable
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN5TokenD1Ev(%class.Token*) unnamed_addr #2 align 2 {
  %2 = alloca %class.Token*, align 8
  store %class.Token* %0, %class.Token** %2, align 8
  %3 = load %class.Token*, %class.Token** %2, align 8
  call void @_ZN5TokenD2Ev(%class.Token* %3) #11
  ret void
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr dereferenceable(288) %class.Token* @_ZN5TokenaSERKS_(%class.Token*, %class.Token* dereferenceable(288)) #3 align 2 {
  %3 = alloca %class.Token*, align 8
  %4 = alloca %class.Token*, align 8
  store %class.Token* %0, %class.Token** %3, align 8
  store %class.Token* %1, %class.Token** %4, align 8
  %5 = load %class.Token*, %class.Token** %3, align 8
  %6 = getelementptr inbounds %class.Token, %class.Token* %5, i32 0, i32 0
  %7 = load %class.Token*, %class.Token** %4, align 8
  %8 = getelementptr inbounds %class.Token, %class.Token* %7, i32 0, i32 0
  %9 = call dereferenceable(264) %struct.Variant* @_ZN7VariantaSERKS_(%struct.Variant* %6, %struct.Variant* dereferenceable(264) %8)
  %10 = getelementptr inbounds %class.Token, %class.Token* %5, i32 0, i32 1
  %11 = load %class.Token*, %class.Token** %4, align 8
  %12 = getelementptr inbounds %class.Token, %class.Token* %11, i32 0, i32 1
  %13 = bitcast i32* %10 to i8*
  %14 = bitcast i32* %12 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %13, i8* %14, i64 24, i32 8, i1 false)
  ret %class.Token* %5
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr i32 @_ZNK5Token9get_startEv(%class.Token*) #2 align 2 {
  %2 = alloca %class.Token*, align 8
  store %class.Token* %0, %class.Token** %2, align 8
  %3 = load %class.Token*, %class.Token** %2, align 8
  %4 = getelementptr inbounds %class.Token, %class.Token* %3, i32 0, i32 3
  %5 = load i32, i32* %4, align 8
  ret i32 %5
}

; Function Attrs: nounwind
declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"*) unnamed_addr #4

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI5TokenNS_9allocatorIS1_EEED1Ev(%"class.std::__1::vector.27"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::vector.27"*, align 8
  store %"class.std::__1::vector.27"* %0, %"class.std::__1::vector.27"** %2, align 8
  %3 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %2, align 8
  call void @_ZNSt3__16vectorI5TokenNS_9allocatorIS1_EEED2Ev(%"class.std::__1::vector.27"* %3) #11
  ret void
}

; Function Attrs: ssp uwtable
define void @_ZN5LexerC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Lexer*, %"class.std::__1::basic_string"*) unnamed_addr #0 align 2 {
  %3 = alloca %class.Lexer*, align 8
  store %class.Lexer* %0, %class.Lexer** %3, align 8
  %4 = load %class.Lexer*, %class.Lexer** %3, align 8
  call void @_ZN5LexerC2ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Lexer* %4, %"class.std::__1::basic_string"* %1)
  ret void
}

; Function Attrs: ssp uwtable
define void @_ZN5Lexer5s_valEv(%"class.std::__1::basic_string"* noalias sret, %class.Lexer*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::basic_string"*, align 8
  %4 = alloca i8*, align 8
  %5 = alloca %"class.std::__1::basic_string"*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %class.Lexer*, align 8
  %8 = alloca %struct.Variant, align 8
  %9 = alloca i8*
  %10 = alloca i32
  %11 = alloca %"class.std::__1::basic_string", align 8
  %12 = alloca %struct.Variant, align 8
  store %class.Lexer* %1, %class.Lexer** %7, align 8
  %13 = load %class.Lexer*, %class.Lexer** %7, align 8
  %14 = getelementptr inbounds %class.Lexer, %class.Lexer* %13, i32 0, i32 6
  call void @_ZN5Token9get_valueEv(%struct.Variant* sret %8, %class.Token* %14)
  %15 = invoke dereferenceable(216) %struct.TypeSpecifier* @_ZN7Variant8get_typeEv(%struct.Variant* %8)
          to label %16 unwind label %31

; <label>:16:                                     ; preds = %2
  %17 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %15, i32 0, i32 1
  store %"class.std::__1::basic_string"* %17, %"class.std::__1::basic_string"** %5, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str, i32 0, i32 0), i8** %6, align 8
  %18 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %5, align 8
  %19 = load i8*, i8** %6, align 8
  store %"class.std::__1::basic_string"* %18, %"class.std::__1::basic_string"** %3, align 8
  store i8* %19, i8** %4, align 8
  %20 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %3, align 8
  %21 = load i8*, i8** %4, align 8
  %22 = call i32 @_ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7compareEPKc(%"class.std::__1::basic_string"* %20, i8* %21) #11
  %23 = icmp eq i32 %22, 0
  %24 = xor i1 %23, true
  call void @_ZN7VariantD1Ev(%struct.Variant* %8) #11
  br i1 %24, label %25, label %39

; <label>:25:                                     ; preds = %16
  %26 = getelementptr inbounds %class.Lexer, %class.Lexer* %13, i32 0, i32 6
  %27 = call i32 @_ZN5Token8get_typeEv(%class.Token* %26)
  %28 = zext i32 %27 to i64
  %29 = getelementptr inbounds [0 x %"class.std::__1::basic_string"], [0 x %"class.std::__1::basic_string"]* @_ZN4util11token_namesE, i64 0, i64 %28
  call void @_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_RKS9_(%"class.std::__1::basic_string"* sret %11, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str.1, i32 0, i32 0), %"class.std::__1::basic_string"* dereferenceable(24) %29)
  invoke void @_ZN10ParseError5raiseE11ParseErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP5Lexer(i32 0, %"class.std::__1::basic_string"* %11, %class.Lexer* %13)
          to label %30 unwind label %35

; <label>:30:                                     ; preds = %25
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %11) #11
  br label %39

; <label>:31:                                     ; preds = %2
  %32 = landingpad { i8*, i32 }
          cleanup
  %33 = extractvalue { i8*, i32 } %32, 0
  store i8* %33, i8** %9, align 8
  %34 = extractvalue { i8*, i32 } %32, 1
  store i32 %34, i32* %10, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %8) #11
  br label %46

; <label>:35:                                     ; preds = %25
  %36 = landingpad { i8*, i32 }
          cleanup
  %37 = extractvalue { i8*, i32 } %36, 0
  store i8* %37, i8** %9, align 8
  %38 = extractvalue { i8*, i32 } %36, 1
  store i32 %38, i32* %10, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %11) #11
  br label %46

; <label>:39:                                     ; preds = %30, %16
  %40 = getelementptr inbounds %class.Lexer, %class.Lexer* %13, i32 0, i32 6
  call void @_ZN5Token9get_valueEv(%struct.Variant* sret %12, %class.Token* %40)
  invoke void @_ZN7Variant3getINSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEEET_v(%"class.std::__1::basic_string"* sret %0, %struct.Variant* %12)
          to label %41 unwind label %42

; <label>:41:                                     ; preds = %39
  call void @_ZN7VariantD1Ev(%struct.Variant* %12) #11
  ret void

; <label>:42:                                     ; preds = %39
  %43 = landingpad { i8*, i32 }
          cleanup
  %44 = extractvalue { i8*, i32 } %43, 0
  store i8* %44, i8** %9, align 8
  %45 = extractvalue { i8*, i32 } %43, 1
  store i32 %45, i32* %10, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %12) #11
  br label %46

; <label>:46:                                     ; preds = %42, %35, %31
  %47 = load i8*, i8** %9, align 8
  %48 = load i32, i32* %10, align 4
  %49 = insertvalue { i8*, i32 } undef, i8* %47, 0
  %50 = insertvalue { i8*, i32 } %49, i32 %48, 1
  resume { i8*, i32 } %50
}

declare void @_ZN5Token9get_valueEv(%struct.Variant* sret, %class.Token*) #1

declare dereferenceable(216) %struct.TypeSpecifier* @_ZN7Variant8get_typeEv(%struct.Variant*) #1

declare void @_ZN10ParseError5raiseE11ParseErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP5Lexer(i32, %"class.std::__1::basic_string"*, %class.Lexer*) #1

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_RKS9_(%"class.std::__1::basic_string"* noalias sret, i8*, %"class.std::__1::basic_string"* dereferenceable(24)) #0 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %6 = alloca %"class.std::__1::basic_string"*, align 8
  %7 = alloca [3 x i64]*, align 8
  %8 = alloca i32, align 4
  %9 = alloca %"class.std::__1::allocator"*, align 8
  %10 = alloca %"class.std::__1::allocator", align 1
  %11 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %12 = alloca %"class.std::__1::allocator"*, align 8
  %13 = alloca %"class.std::__1::allocator", align 1
  %14 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %15 = alloca %"class.std::__1::allocator", align 1
  %16 = alloca %"class.std::__1::allocator", align 1
  %17 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %18 = alloca %"class.std::__1::basic_string"*, align 8
  %19 = alloca %"class.std::__1::allocator"*, align 8
  %20 = alloca %"class.std::__1::allocator", align 1
  %21 = alloca %"class.std::__1::basic_string"*, align 8
  %22 = alloca %"class.std::__1::allocator"*, align 8
  %23 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %24 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %25 = alloca %"class.std::__1::basic_string"*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %28 = alloca %"class.std::__1::basic_string"*, align 8
  %29 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %30 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %31 = alloca %"class.std::__1::basic_string"*, align 8
  %32 = alloca %"class.std::__1::basic_string"*, align 8
  %33 = alloca i8*, align 8
  %34 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %35 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %36 = alloca %"class.std::__1::basic_string"*, align 8
  %37 = alloca i8*, align 8
  %38 = alloca i8*, align 8
  %39 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %40 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %41 = alloca %"class.std::__1::basic_string"*, align 8
  %42 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %43 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %44 = alloca %"class.std::__1::basic_string"*, align 8
  %45 = alloca %"class.std::__1::basic_string"*, align 8
  %46 = alloca %"class.std::__1::basic_string"*, align 8
  %47 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %48 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %49 = alloca %"class.std::__1::basic_string"*, align 8
  %50 = alloca %"class.std::__1::allocator", align 1
  %51 = alloca %"class.std::__1::basic_string"*, align 8
  %52 = alloca i8*, align 8
  %53 = alloca %"class.std::__1::basic_string"*, align 8
  %54 = alloca i1, align 1
  %55 = alloca %"class.std::__1::allocator", align 1
  %56 = alloca %"class.std::__1::allocator", align 1
  %57 = alloca i64, align 8
  %58 = alloca i8*
  %59 = alloca i32
  %60 = alloca i64, align 8
  store i8* %1, i8** %52, align 8
  store %"class.std::__1::basic_string"* %2, %"class.std::__1::basic_string"** %53, align 8
  store i1 false, i1* %54, align 1
  %61 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8
  store %"class.std::__1::basic_string"* %61, %"class.std::__1::basic_string"** %51, align 8
  %62 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8
  store %"class.std::__1::basic_string"* %62, %"class.std::__1::basic_string"** %49, align 8
  %63 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8
  %64 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %63, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %64, %"class.std::__1::__compressed_pair"** %48, align 8
  %65 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8
  %66 = bitcast %"class.std::__1::__compressed_pair"* %65 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %66, %"class.std::__1::__libcpp_compressed_pair_imp"** %47, align 8
  %67 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %47, align 8
  %68 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %67 to %"class.std::__1::allocator"*
  store %"class.std::__1::basic_string"* %0, %"class.std::__1::basic_string"** %21, align 8
  store %"class.std::__1::allocator"* %55, %"class.std::__1::allocator"** %22, align 8
  %69 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %21, align 8
  %70 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %22, align 8
  store %"class.std::__1::basic_string"* %69, %"class.std::__1::basic_string"** %18, align 8
  store %"class.std::__1::allocator"* %70, %"class.std::__1::allocator"** %19, align 8
  %71 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %18, align 8
  %72 = bitcast %"class.std::__1::basic_string"* %71 to %"class.std::__1::__basic_string_common"*
  %73 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %71, i32 0, i32 0
  %74 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %19, align 8
  store %"class.std::__1::__compressed_pair"* %73, %"class.std::__1::__compressed_pair"** %17, align 8
  %75 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %17, align 8
  store %"class.std::__1::__compressed_pair"* %75, %"class.std::__1::__compressed_pair"** %14, align 8
  %76 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %14, align 8
  %77 = bitcast %"class.std::__1::__compressed_pair"* %76 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::allocator"* %13, %"class.std::__1::allocator"** %12, align 8
  %78 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %12, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %77, %"class.std::__1::__libcpp_compressed_pair_imp"** %11, align 8
  %79 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %11, align 8
  %80 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %79 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %10, %"class.std::__1::allocator"** %9, align 8
  %81 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %9, align 8
  %82 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %79, i32 0, i32 0
  %83 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %82 to i8*
  call void @llvm.memset.p0i8.i64(i8* %83, i8 0, i64 24, i32 8, i1 false) #11
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %6, align 8
  %84 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8
  %85 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %84, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %85, %"class.std::__1::__compressed_pair"** %5, align 8
  %86 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8
  %87 = bitcast %"class.std::__1::__compressed_pair"* %86 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %87, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8
  %88 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8
  %89 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %88, i32 0, i32 0
  %90 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %89, i32 0, i32 0
  %91 = bitcast %union.anon.0* %90 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %92 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %91, i32 0, i32 0
  store [3 x i64]* %92, [3 x i64]** %7, align 8
  store i32 0, i32* %8, align 4
  br label %93

; <label>:93:                                     ; preds = %96, %3
  %94 = load i32, i32* %8, align 4
  %95 = icmp ult i32 %94, 3
  br i1 %95, label %96, label %103

; <label>:96:                                     ; preds = %93
  %97 = load i32, i32* %8, align 4
  %98 = zext i32 %97 to i64
  %99 = load [3 x i64]*, [3 x i64]** %7, align 8
  %100 = getelementptr inbounds [3 x i64], [3 x i64]* %99, i64 0, i64 %98
  store i64 0, i64* %100, align 8
  %101 = load i32, i32* %8, align 4
  %102 = add i32 %101, 1
  store i32 %102, i32* %8, align 4
  br label %93

; <label>:103:                                    ; preds = %93
  %104 = load i8*, i8** %52, align 8
  %105 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %104)
          to label %106 unwind label %205

; <label>:106:                                    ; preds = %103
  store i64 %105, i64* %57, align 8
  %107 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8
  store %"class.std::__1::basic_string"* %107, %"class.std::__1::basic_string"** %32, align 8
  %108 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %32, align 8
  store %"class.std::__1::basic_string"* %108, %"class.std::__1::basic_string"** %31, align 8
  %109 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %31, align 8
  %110 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %109, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %110, %"class.std::__1::__compressed_pair"** %30, align 8
  %111 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %30, align 8
  %112 = bitcast %"class.std::__1::__compressed_pair"* %111 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %112, %"class.std::__1::__libcpp_compressed_pair_imp"** %29, align 8
  %113 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %29, align 8
  %114 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %113, i32 0, i32 0
  %115 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %114, i32 0, i32 0
  %116 = bitcast %union.anon.0* %115 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %117 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %116, i32 0, i32 0
  %118 = bitcast %union.anon.1* %117 to i8*
  %119 = load i8, i8* %118, align 8
  %120 = zext i8 %119 to i32
  %121 = and i32 %120, 1
  %122 = icmp ne i32 %121, 0
  br i1 %122, label %123, label %134

; <label>:123:                                    ; preds = %106
  store %"class.std::__1::basic_string"* %108, %"class.std::__1::basic_string"** %25, align 8
  %124 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %25, align 8
  %125 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %124, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %125, %"class.std::__1::__compressed_pair"** %24, align 8
  %126 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %24, align 8
  %127 = bitcast %"class.std::__1::__compressed_pair"* %126 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %127, %"class.std::__1::__libcpp_compressed_pair_imp"** %23, align 8
  %128 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %23, align 8
  %129 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %128, i32 0, i32 0
  %130 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %129, i32 0, i32 0
  %131 = bitcast %union.anon.0* %130 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %132 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %131, i32 0, i32 1
  %133 = load i64, i64* %132, align 8
  br label %149

; <label>:134:                                    ; preds = %106
  store %"class.std::__1::basic_string"* %108, %"class.std::__1::basic_string"** %28, align 8
  %135 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8
  %136 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %135, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %136, %"class.std::__1::__compressed_pair"** %27, align 8
  %137 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8
  %138 = bitcast %"class.std::__1::__compressed_pair"* %137 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %138, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8
  %139 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8
  %140 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %139, i32 0, i32 0
  %141 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %140, i32 0, i32 0
  %142 = bitcast %union.anon.0* %141 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %143 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %142, i32 0, i32 0
  %144 = bitcast %union.anon.1* %143 to i8*
  %145 = load i8, i8* %144, align 8
  %146 = zext i8 %145 to i32
  %147 = ashr i32 %146, 1
  %148 = sext i32 %147 to i64
  br label %149

; <label>:149:                                    ; preds = %123, %134
  %150 = phi i64 [ %133, %123 ], [ %148, %134 ]
  store i64 %150, i64* %60, align 8
  %151 = load i8*, i8** %52, align 8
  %152 = load i64, i64* %57, align 8
  %153 = load i64, i64* %57, align 8
  %154 = load i64, i64* %60, align 8
  %155 = add i64 %153, %154
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcmm(%"class.std::__1::basic_string"* %0, i8* %151, i64 %152, i64 %155)
          to label %156 unwind label %205

; <label>:156:                                    ; preds = %149
  %157 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8
  store %"class.std::__1::basic_string"* %157, %"class.std::__1::basic_string"** %46, align 8
  %158 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %46, align 8
  store %"class.std::__1::basic_string"* %158, %"class.std::__1::basic_string"** %45, align 8
  %159 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8
  store %"class.std::__1::basic_string"* %159, %"class.std::__1::basic_string"** %44, align 8
  %160 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %44, align 8
  %161 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %160, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %161, %"class.std::__1::__compressed_pair"** %43, align 8
  %162 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %43, align 8
  %163 = bitcast %"class.std::__1::__compressed_pair"* %162 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %163, %"class.std::__1::__libcpp_compressed_pair_imp"** %42, align 8
  %164 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %42, align 8
  %165 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %164, i32 0, i32 0
  %166 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %165, i32 0, i32 0
  %167 = bitcast %union.anon.0* %166 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %168 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %167, i32 0, i32 0
  %169 = bitcast %union.anon.1* %168 to i8*
  %170 = load i8, i8* %169, align 8
  %171 = zext i8 %170 to i32
  %172 = and i32 %171, 1
  %173 = icmp ne i32 %172, 0
  br i1 %173, label %174, label %185

; <label>:174:                                    ; preds = %156
  store %"class.std::__1::basic_string"* %159, %"class.std::__1::basic_string"** %36, align 8
  %175 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8
  %176 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %175, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %176, %"class.std::__1::__compressed_pair"** %35, align 8
  %177 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8
  %178 = bitcast %"class.std::__1::__compressed_pair"* %177 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %178, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8
  %179 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8
  %180 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %179, i32 0, i32 0
  %181 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %180, i32 0, i32 0
  %182 = bitcast %union.anon.0* %181 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %183 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %182, i32 0, i32 2
  %184 = load i8*, i8** %183, align 8
  br label %198

; <label>:185:                                    ; preds = %156
  store %"class.std::__1::basic_string"* %159, %"class.std::__1::basic_string"** %41, align 8
  %186 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8
  %187 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %186, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %187, %"class.std::__1::__compressed_pair"** %40, align 8
  %188 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %40, align 8
  %189 = bitcast %"class.std::__1::__compressed_pair"* %188 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %189, %"class.std::__1::__libcpp_compressed_pair_imp"** %39, align 8
  %190 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %39, align 8
  %191 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %190, i32 0, i32 0
  %192 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %191, i32 0, i32 0
  %193 = bitcast %union.anon.0* %192 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %194 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %193, i32 0, i32 1
  %195 = getelementptr inbounds [23 x i8], [23 x i8]* %194, i64 0, i64 0
  store i8* %195, i8** %38, align 8
  %196 = load i8*, i8** %38, align 8
  store i8* %196, i8** %37, align 8
  %197 = load i8*, i8** %37, align 8
  br label %198

; <label>:198:                                    ; preds = %174, %185
  %199 = phi i8* [ %184, %174 ], [ %197, %185 ]
  store i8* %199, i8** %33, align 8
  %200 = load i8*, i8** %33, align 8
  %201 = load i64, i64* %60, align 8
  %202 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %0, i8* %200, i64 %201)
          to label %203 unwind label %205

; <label>:203:                                    ; preds = %198
  store i1 true, i1* %54, align 1
  %204 = load i1, i1* %54, align 1
  br i1 %204, label %210, label %209

; <label>:205:                                    ; preds = %198, %149, %103
  %206 = landingpad { i8*, i32 }
          cleanup
  %207 = extractvalue { i8*, i32 } %206, 0
  store i8* %207, i8** %58, align 8
  %208 = extractvalue { i8*, i32 } %206, 1
  store i32 %208, i32* %59, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %0) #11
  br label %211

; <label>:209:                                    ; preds = %203
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %0) #11
  br label %210

; <label>:210:                                    ; preds = %209, %203
  ret void

; <label>:211:                                    ; preds = %205
  %212 = load i8*, i8** %58, align 8
  %213 = load i32, i32* %59, align 4
  %214 = insertvalue { i8*, i32 } undef, i8* %212, 0
  %215 = insertvalue { i8*, i32 } %214, i32 %213, 1
  resume { i8*, i32 } %215
}

declare void @_ZN7Variant3getINSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEEET_v(%"class.std::__1::basic_string"* sret, %struct.Variant*) #1

; Function Attrs: nounwind ssp uwtable
define zeroext i1 @_ZN5Lexer9is_numberEcb(%class.Lexer*, i8 signext, i1 zeroext) #5 align 2 {
  %4 = alloca %class.Lexer*, align 8
  %5 = alloca i8, align 1
  %6 = alloca i8, align 1
  %7 = alloca i32, align 4
  store %class.Lexer* %0, %class.Lexer** %4, align 8
  store i8 %1, i8* %5, align 1
  %8 = zext i1 %2 to i8
  store i8 %8, i8* %6, align 1
  %9 = load %class.Lexer*, %class.Lexer** %4, align 8
  %10 = load i8, i8* %5, align 1
  %11 = sext i8 %10 to i32
  store i32 %11, i32* %7, align 4
  %12 = load i32, i32* %7, align 4
  %13 = icmp sge i32 %12, 48
  br i1 %13, label %14, label %17

; <label>:14:                                     ; preds = %3
  %15 = load i32, i32* %7, align 4
  %16 = icmp sle i32 %15, 57
  br i1 %16, label %25, label %17

; <label>:17:                                     ; preds = %14, %3
  %18 = load i8, i8* %6, align 1
  %19 = trunc i8 %18 to i1
  br i1 %19, label %20, label %23

; <label>:20:                                     ; preds = %17
  %21 = load i32, i32* %7, align 4
  %22 = icmp eq i32 %21, 46
  br label %23

; <label>:23:                                     ; preds = %20, %17
  %24 = phi i1 [ false, %17 ], [ %22, %20 ]
  br label %25

; <label>:25:                                     ; preds = %23, %14
  %26 = phi i1 [ true, %14 ], [ %24, %23 ]
  ret i1 %26
}

; Function Attrs: nounwind ssp uwtable
define zeroext i1 @_ZN5Lexer6is_hexEc(%class.Lexer*, i8 signext) #5 align 2 {
  %3 = alloca %class.Lexer*, align 8
  %4 = alloca i8, align 1
  %5 = alloca i32, align 4
  store %class.Lexer* %0, %class.Lexer** %3, align 8
  store i8 %1, i8* %4, align 1
  %6 = load %class.Lexer*, %class.Lexer** %3, align 8
  %7 = load i8, i8* %4, align 1
  %8 = sext i8 %7 to i32
  store i32 %8, i32* %5, align 4
  %9 = load i32, i32* %5, align 4
  %10 = icmp sge i32 %9, 65
  br i1 %10, label %11, label %14

; <label>:11:                                     ; preds = %2
  %12 = load i32, i32* %5, align 4
  %13 = icmp sle i32 %12, 70
  br i1 %13, label %28, label %14

; <label>:14:                                     ; preds = %11, %2
  %15 = load i32, i32* %5, align 4
  %16 = icmp sge i32 %15, 97
  br i1 %16, label %17, label %20

; <label>:17:                                     ; preds = %14
  %18 = load i32, i32* %5, align 4
  %19 = icmp sle i32 %18, 102
  br i1 %19, label %28, label %20

; <label>:20:                                     ; preds = %17, %14
  %21 = load i32, i32* %5, align 4
  %22 = icmp sge i32 %21, 48
  br i1 %22, label %23, label %26

; <label>:23:                                     ; preds = %20
  %24 = load i32, i32* %5, align 4
  %25 = icmp sle i32 %24, 57
  br label %26

; <label>:26:                                     ; preds = %23, %20
  %27 = phi i1 [ false, %20 ], [ %25, %23 ]
  br label %28

; <label>:28:                                     ; preds = %26, %17, %11
  %29 = phi i1 [ true, %17 ], [ true, %11 ], [ %27, %26 ]
  ret i1 %29
}

; Function Attrs: ssp uwtable
define zeroext i1 @_ZN5Lexer18is_identifier_charEc(%class.Lexer*, i8 signext) #0 align 2 {
  %3 = alloca %class.Lexer*, align 8
  %4 = alloca i8, align 1
  %5 = alloca i32, align 4
  store %class.Lexer* %0, %class.Lexer** %3, align 8
  store i8 %1, i8* %4, align 1
  %6 = load %class.Lexer*, %class.Lexer** %3, align 8
  %7 = load i8, i8* %4, align 1
  %8 = sext i8 %7 to i32
  store i32 %8, i32* %5, align 4
  %9 = load i32, i32* %5, align 4
  %10 = trunc i32 %9 to i8
  %11 = call zeroext i1 @_ZN5Lexer16is_operator_charEc(%class.Lexer* %6, i8 signext %10)
  br i1 %11, label %22, label %12

; <label>:12:                                     ; preds = %2
  %13 = load i32, i32* %5, align 4
  %14 = trunc i32 %13 to i8
  %15 = call zeroext i1 @_ZN5Lexer13is_punctuatorEc(%class.Lexer* %6, i8 signext %14)
  br i1 %15, label %22, label %16

; <label>:16:                                     ; preds = %12
  %17 = load i32, i32* %5, align 4
  %18 = icmp ne i32 %17, 32
  br i1 %18, label %19, label %22

; <label>:19:                                     ; preds = %16
  %20 = load i32, i32* %5, align 4
  %21 = icmp ne i32 %20, 0
  br label %22

; <label>:22:                                     ; preds = %19, %16, %12, %2
  %23 = phi i1 [ false, %16 ], [ false, %12 ], [ false, %2 ], [ %21, %19 ]
  ret i1 %23
}

; Function Attrs: ssp uwtable
define zeroext i1 @_ZN5Lexer16is_operator_charEc(%class.Lexer*, i8 signext) #0 align 2 {
  %3 = alloca %"class.std::__1::vector.47"*, align 8
  %4 = alloca i64, align 8
  %5 = alloca %"class.std::__1::vector.47"*, align 8
  %6 = alloca i1, align 1
  %7 = alloca %class.Lexer*, align 8
  %8 = alloca i8, align 1
  %9 = alloca i32, align 4
  store %class.Lexer* %0, %class.Lexer** %7, align 8
  store i8 %1, i8* %8, align 1
  %10 = load %class.Lexer*, %class.Lexer** %7, align 8
  store i32 0, i32* %9, align 4
  br label %11

; <label>:11:                                     ; preds = %41, %2
  %12 = load i32, i32* %9, align 4
  %13 = sext i32 %12 to i64
  store %"class.std::__1::vector.47"* @_ZN4util14operator_charsE, %"class.std::__1::vector.47"** %5, align 8
  %14 = load %"class.std::__1::vector.47"*, %"class.std::__1::vector.47"** %5, align 8
  %15 = bitcast %"class.std::__1::vector.47"* %14 to %"class.std::__1::__vector_base.48"*
  %16 = getelementptr inbounds %"class.std::__1::__vector_base.48", %"class.std::__1::__vector_base.48"* %15, i32 0, i32 1
  %17 = load i8*, i8** %16, align 8
  %18 = bitcast %"class.std::__1::vector.47"* %14 to %"class.std::__1::__vector_base.48"*
  %19 = getelementptr inbounds %"class.std::__1::__vector_base.48", %"class.std::__1::__vector_base.48"* %18, i32 0, i32 0
  %20 = load i8*, i8** %19, align 8
  %21 = ptrtoint i8* %17 to i64
  %22 = ptrtoint i8* %20 to i64
  %23 = sub i64 %21, %22
  %24 = icmp ult i64 %13, %23
  br i1 %24, label %25, label %44

; <label>:25:                                     ; preds = %11
  %26 = load i8, i8* %8, align 1
  %27 = sext i8 %26 to i32
  %28 = load i32, i32* %9, align 4
  %29 = sext i32 %28 to i64
  store %"class.std::__1::vector.47"* @_ZN4util14operator_charsE, %"class.std::__1::vector.47"** %3, align 8
  store i64 %29, i64* %4, align 8
  %30 = load %"class.std::__1::vector.47"*, %"class.std::__1::vector.47"** %3, align 8
  %31 = load i64, i64* %4, align 8
  %32 = bitcast %"class.std::__1::vector.47"* %30 to %"class.std::__1::__vector_base.48"*
  %33 = getelementptr inbounds %"class.std::__1::__vector_base.48", %"class.std::__1::__vector_base.48"* %32, i32 0, i32 0
  %34 = load i8*, i8** %33, align 8
  %35 = getelementptr inbounds i8, i8* %34, i64 %31
  %36 = load i8, i8* %35, align 1
  %37 = sext i8 %36 to i32
  %38 = icmp eq i32 %27, %37
  br i1 %38, label %39, label %40

; <label>:39:                                     ; preds = %25
  store i1 true, i1* %6, align 1
  br label %45

; <label>:40:                                     ; preds = %25
  br label %41

; <label>:41:                                     ; preds = %40
  %42 = load i32, i32* %9, align 4
  %43 = add nsw i32 %42, 1
  store i32 %43, i32* %9, align 4
  br label %11

; <label>:44:                                     ; preds = %11
  store i1 false, i1* %6, align 1
  br label %45

; <label>:45:                                     ; preds = %44, %39
  %46 = load i1, i1* %6, align 1
  ret i1 %46
}

; Function Attrs: ssp uwtable
define zeroext i1 @_ZN5Lexer13is_punctuatorEc(%class.Lexer*, i8 signext) #0 align 2 {
  %3 = alloca %"class.std::__1::vector.47"*, align 8
  %4 = alloca i64, align 8
  %5 = alloca %"class.std::__1::vector.47"*, align 8
  %6 = alloca i1, align 1
  %7 = alloca %class.Lexer*, align 8
  %8 = alloca i8, align 1
  %9 = alloca i32, align 4
  store %class.Lexer* %0, %class.Lexer** %7, align 8
  store i8 %1, i8* %8, align 1
  %10 = load %class.Lexer*, %class.Lexer** %7, align 8
  store i32 0, i32* %9, align 4
  br label %11

; <label>:11:                                     ; preds = %41, %2
  %12 = load i32, i32* %9, align 4
  %13 = sext i32 %12 to i64
  store %"class.std::__1::vector.47"* @_ZN4util11punctuatorsE, %"class.std::__1::vector.47"** %5, align 8
  %14 = load %"class.std::__1::vector.47"*, %"class.std::__1::vector.47"** %5, align 8
  %15 = bitcast %"class.std::__1::vector.47"* %14 to %"class.std::__1::__vector_base.48"*
  %16 = getelementptr inbounds %"class.std::__1::__vector_base.48", %"class.std::__1::__vector_base.48"* %15, i32 0, i32 1
  %17 = load i8*, i8** %16, align 8
  %18 = bitcast %"class.std::__1::vector.47"* %14 to %"class.std::__1::__vector_base.48"*
  %19 = getelementptr inbounds %"class.std::__1::__vector_base.48", %"class.std::__1::__vector_base.48"* %18, i32 0, i32 0
  %20 = load i8*, i8** %19, align 8
  %21 = ptrtoint i8* %17 to i64
  %22 = ptrtoint i8* %20 to i64
  %23 = sub i64 %21, %22
  %24 = icmp ult i64 %13, %23
  br i1 %24, label %25, label %44

; <label>:25:                                     ; preds = %11
  %26 = load i8, i8* %8, align 1
  %27 = sext i8 %26 to i32
  %28 = load i32, i32* %9, align 4
  %29 = sext i32 %28 to i64
  store %"class.std::__1::vector.47"* @_ZN4util11punctuatorsE, %"class.std::__1::vector.47"** %3, align 8
  store i64 %29, i64* %4, align 8
  %30 = load %"class.std::__1::vector.47"*, %"class.std::__1::vector.47"** %3, align 8
  %31 = load i64, i64* %4, align 8
  %32 = bitcast %"class.std::__1::vector.47"* %30 to %"class.std::__1::__vector_base.48"*
  %33 = getelementptr inbounds %"class.std::__1::__vector_base.48", %"class.std::__1::__vector_base.48"* %32, i32 0, i32 0
  %34 = load i8*, i8** %33, align 8
  %35 = getelementptr inbounds i8, i8* %34, i64 %31
  %36 = load i8, i8* %35, align 1
  %37 = sext i8 %36 to i32
  %38 = icmp eq i32 %27, %37
  br i1 %38, label %39, label %40

; <label>:39:                                     ; preds = %25
  store i1 true, i1* %6, align 1
  br label %45

; <label>:40:                                     ; preds = %25
  br label %41

; <label>:41:                                     ; preds = %40
  %42 = load i32, i32* %9, align 4
  %43 = add nsw i32 %42, 1
  store i32 %43, i32* %9, align 4
  br label %11

; <label>:44:                                     ; preds = %11
  store i1 false, i1* %6, align 1
  br label %45

; <label>:45:                                     ; preds = %44, %39
  %46 = load i1, i1* %6, align 1
  ret i1 %46
}

; Function Attrs: ssp uwtable
define zeroext i1 @_ZN5Lexer11is_operatorENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Lexer*, %"class.std::__1::basic_string"*) #0 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %3 = alloca i8*, align 8
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %6 = alloca %"class.std::__1::basic_string"*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i8*, align 8
  %9 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %14 = alloca %"class.std::__1::basic_string"*, align 8
  %15 = alloca %"class.std::__1::basic_string"*, align 8
  %16 = alloca %"class.std::__1::basic_string"*, align 8
  %17 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %18 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %19 = alloca %"class.std::__1::basic_string"*, align 8
  %20 = alloca i8*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %23 = alloca %"class.std::__1::basic_string"*, align 8
  %24 = alloca i8*, align 8
  %25 = alloca i8*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %28 = alloca %"class.std::__1::basic_string"*, align 8
  %29 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %30 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %31 = alloca %"class.std::__1::basic_string"*, align 8
  %32 = alloca %"class.std::__1::basic_string"*, align 8
  %33 = alloca %"class.std::__1::basic_string"*, align 8
  %34 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %35 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %36 = alloca %"class.std::__1::basic_string"*, align 8
  %37 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %38 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %39 = alloca %"class.std::__1::basic_string"*, align 8
  %40 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %41 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %42 = alloca %"class.std::__1::basic_string"*, align 8
  %43 = alloca %"class.std::__1::basic_string"*, align 8
  %44 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %45 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %46 = alloca %"class.std::__1::basic_string"*, align 8
  %47 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %48 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %49 = alloca %"class.std::__1::basic_string"*, align 8
  %50 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %51 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %52 = alloca %"class.std::__1::basic_string"*, align 8
  %53 = alloca %"class.std::__1::basic_string"*, align 8
  %54 = alloca i1, align 1
  %55 = alloca %"class.std::__1::basic_string"*, align 8
  %56 = alloca %"class.std::__1::basic_string"*, align 8
  %57 = alloca i64, align 8
  %58 = alloca i8*, align 8
  %59 = alloca i8*, align 8
  %60 = alloca %"class.std::__1::vector"*, align 8
  %61 = alloca %"class.std::__1::vector"*, align 8
  %62 = alloca i64, align 8
  %63 = alloca %"class.std::__1::vector"*, align 8
  %64 = alloca i64, align 8
  %65 = alloca %"class.std::__1::vector"*, align 8
  %66 = alloca i8*, align 8
  %67 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %68 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %69 = alloca %"class.std::__1::basic_string"*, align 8
  %70 = alloca i8*, align 8
  %71 = alloca i8*, align 8
  %72 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %73 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %74 = alloca %"class.std::__1::basic_string"*, align 8
  %75 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %76 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %77 = alloca %"class.std::__1::basic_string"*, align 8
  %78 = alloca %"class.std::__1::basic_string"*, align 8
  %79 = alloca %"class.std::__1::basic_string"*, align 8
  %80 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %81 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %82 = alloca %"class.std::__1::basic_string"*, align 8
  %83 = alloca i8*, align 8
  %84 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %85 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %86 = alloca %"class.std::__1::basic_string"*, align 8
  %87 = alloca i8*, align 8
  %88 = alloca i8*, align 8
  %89 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %90 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %91 = alloca %"class.std::__1::basic_string"*, align 8
  %92 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %93 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %94 = alloca %"class.std::__1::basic_string"*, align 8
  %95 = alloca %"class.std::__1::basic_string"*, align 8
  %96 = alloca %"class.std::__1::basic_string"*, align 8
  %97 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %98 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %99 = alloca %"class.std::__1::basic_string"*, align 8
  %100 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %101 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %102 = alloca %"class.std::__1::basic_string"*, align 8
  %103 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %104 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %105 = alloca %"class.std::__1::basic_string"*, align 8
  %106 = alloca %"class.std::__1::basic_string"*, align 8
  %107 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %108 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %109 = alloca %"class.std::__1::basic_string"*, align 8
  %110 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %111 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %112 = alloca %"class.std::__1::basic_string"*, align 8
  %113 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %114 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %115 = alloca %"class.std::__1::basic_string"*, align 8
  %116 = alloca %"class.std::__1::basic_string"*, align 8
  %117 = alloca i1, align 1
  %118 = alloca %"class.std::__1::basic_string"*, align 8
  %119 = alloca %"class.std::__1::basic_string"*, align 8
  %120 = alloca i64, align 8
  %121 = alloca i8*, align 8
  %122 = alloca i8*, align 8
  %123 = alloca %"class.std::__1::vector"*, align 8
  %124 = alloca i64, align 8
  %125 = alloca i8*, align 8
  %126 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %127 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %128 = alloca %"class.std::__1::basic_string"*, align 8
  %129 = alloca i8*, align 8
  %130 = alloca i8*, align 8
  %131 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %132 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %133 = alloca %"class.std::__1::basic_string"*, align 8
  %134 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %135 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %136 = alloca %"class.std::__1::basic_string"*, align 8
  %137 = alloca %"class.std::__1::basic_string"*, align 8
  %138 = alloca %"class.std::__1::basic_string"*, align 8
  %139 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %140 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %141 = alloca %"class.std::__1::basic_string"*, align 8
  %142 = alloca i8*, align 8
  %143 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %144 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %145 = alloca %"class.std::__1::basic_string"*, align 8
  %146 = alloca i8*, align 8
  %147 = alloca i8*, align 8
  %148 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %149 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %150 = alloca %"class.std::__1::basic_string"*, align 8
  %151 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %152 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %153 = alloca %"class.std::__1::basic_string"*, align 8
  %154 = alloca %"class.std::__1::basic_string"*, align 8
  %155 = alloca %"class.std::__1::basic_string"*, align 8
  %156 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %157 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %158 = alloca %"class.std::__1::basic_string"*, align 8
  %159 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %160 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %161 = alloca %"class.std::__1::basic_string"*, align 8
  %162 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %163 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %164 = alloca %"class.std::__1::basic_string"*, align 8
  %165 = alloca %"class.std::__1::basic_string"*, align 8
  %166 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %167 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %168 = alloca %"class.std::__1::basic_string"*, align 8
  %169 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %170 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %171 = alloca %"class.std::__1::basic_string"*, align 8
  %172 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %173 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %174 = alloca %"class.std::__1::basic_string"*, align 8
  %175 = alloca %"class.std::__1::basic_string"*, align 8
  %176 = alloca i1, align 1
  %177 = alloca %"class.std::__1::basic_string"*, align 8
  %178 = alloca %"class.std::__1::basic_string"*, align 8
  %179 = alloca i64, align 8
  %180 = alloca i8*, align 8
  %181 = alloca i8*, align 8
  %182 = alloca %"class.std::__1::vector"*, align 8
  %183 = alloca i1, align 1
  %184 = alloca %class.Lexer*, align 8
  %185 = alloca i32, align 4
  %186 = alloca i32, align 4
  %187 = alloca i32, align 4
  store %class.Lexer* %0, %class.Lexer** %184, align 8
  %188 = load %class.Lexer*, %class.Lexer** %184, align 8
  store i32 0, i32* %185, align 4
  br label %189

; <label>:189:                                    ; preds = %443, %2
  %190 = load i32, i32* %185, align 4
  %191 = sext i32 %190 to i64
  store %"class.std::__1::vector"* @_ZN4util16binary_operatorsE, %"class.std::__1::vector"** %182, align 8
  %192 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %182, align 8
  %193 = bitcast %"class.std::__1::vector"* %192 to %"class.std::__1::__vector_base"*
  %194 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %193, i32 0, i32 1
  %195 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %194, align 8
  %196 = bitcast %"class.std::__1::vector"* %192 to %"class.std::__1::__vector_base"*
  %197 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %196, i32 0, i32 0
  %198 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %197, align 8
  %199 = ptrtoint %"class.std::__1::basic_string"* %195 to i64
  %200 = ptrtoint %"class.std::__1::basic_string"* %198 to i64
  %201 = sub i64 %199, %200
  %202 = sdiv exact i64 %201, 24
  %203 = icmp ult i64 %191, %202
  br i1 %203, label %204, label %446

; <label>:204:                                    ; preds = %189
  %205 = load i32, i32* %185, align 4
  %206 = sext i32 %205 to i64
  store %"class.std::__1::vector"* @_ZN4util16binary_operatorsE, %"class.std::__1::vector"** %123, align 8
  store i64 %206, i64* %124, align 8
  %207 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %123, align 8
  %208 = load i64, i64* %124, align 8
  %209 = bitcast %"class.std::__1::vector"* %207 to %"class.std::__1::__vector_base"*
  %210 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %209, i32 0, i32 0
  %211 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %210, align 8
  %212 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %211, i64 %208
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %118, align 8
  store %"class.std::__1::basic_string"* %212, %"class.std::__1::basic_string"** %119, align 8
  %213 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %118, align 8
  store %"class.std::__1::basic_string"* %213, %"class.std::__1::basic_string"** %116, align 8
  %214 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %116, align 8
  store %"class.std::__1::basic_string"* %214, %"class.std::__1::basic_string"** %115, align 8
  %215 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %115, align 8
  %216 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %215, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %216, %"class.std::__1::__compressed_pair"** %114, align 8
  %217 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %114, align 8
  %218 = bitcast %"class.std::__1::__compressed_pair"* %217 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %218, %"class.std::__1::__libcpp_compressed_pair_imp"** %113, align 8
  %219 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %113, align 8
  %220 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %219, i32 0, i32 0
  %221 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %220, i32 0, i32 0
  %222 = bitcast %union.anon.0* %221 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %223 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %222, i32 0, i32 0
  %224 = bitcast %union.anon.1* %223 to i8*
  %225 = load i8, i8* %224, align 8
  %226 = zext i8 %225 to i32
  %227 = and i32 %226, 1
  %228 = icmp ne i32 %227, 0
  br i1 %228, label %229, label %240

; <label>:229:                                    ; preds = %204
  store %"class.std::__1::basic_string"* %214, %"class.std::__1::basic_string"** %109, align 8
  %230 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %109, align 8
  %231 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %230, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %231, %"class.std::__1::__compressed_pair"** %108, align 8
  %232 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %108, align 8
  %233 = bitcast %"class.std::__1::__compressed_pair"* %232 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %233, %"class.std::__1::__libcpp_compressed_pair_imp"** %107, align 8
  %234 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %107, align 8
  %235 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %234, i32 0, i32 0
  %236 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %235, i32 0, i32 0
  %237 = bitcast %union.anon.0* %236 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %238 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %237, i32 0, i32 1
  %239 = load i64, i64* %238, align 8
  br label %255

; <label>:240:                                    ; preds = %204
  store %"class.std::__1::basic_string"* %214, %"class.std::__1::basic_string"** %112, align 8
  %241 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %112, align 8
  %242 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %241, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %242, %"class.std::__1::__compressed_pair"** %111, align 8
  %243 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %111, align 8
  %244 = bitcast %"class.std::__1::__compressed_pair"* %243 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %244, %"class.std::__1::__libcpp_compressed_pair_imp"** %110, align 8
  %245 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %110, align 8
  %246 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %245, i32 0, i32 0
  %247 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %246, i32 0, i32 0
  %248 = bitcast %union.anon.0* %247 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %249 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %248, i32 0, i32 0
  %250 = bitcast %union.anon.1* %249 to i8*
  %251 = load i8, i8* %250, align 8
  %252 = zext i8 %251 to i32
  %253 = ashr i32 %252, 1
  %254 = sext i32 %253 to i64
  br label %255

; <label>:255:                                    ; preds = %240, %229
  %256 = phi i64 [ %239, %229 ], [ %254, %240 ]
  store i64 %256, i64* %120, align 8
  %257 = load i64, i64* %120, align 8
  %258 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %119, align 8
  store %"class.std::__1::basic_string"* %258, %"class.std::__1::basic_string"** %106, align 8
  %259 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %106, align 8
  store %"class.std::__1::basic_string"* %259, %"class.std::__1::basic_string"** %105, align 8
  %260 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %105, align 8
  %261 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %260, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %261, %"class.std::__1::__compressed_pair"** %104, align 8
  %262 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %104, align 8
  %263 = bitcast %"class.std::__1::__compressed_pair"* %262 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %263, %"class.std::__1::__libcpp_compressed_pair_imp"** %103, align 8
  %264 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %103, align 8
  %265 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %264, i32 0, i32 0
  %266 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %265, i32 0, i32 0
  %267 = bitcast %union.anon.0* %266 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %268 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %267, i32 0, i32 0
  %269 = bitcast %union.anon.1* %268 to i8*
  %270 = load i8, i8* %269, align 8
  %271 = zext i8 %270 to i32
  %272 = and i32 %271, 1
  %273 = icmp ne i32 %272, 0
  br i1 %273, label %274, label %285

; <label>:274:                                    ; preds = %255
  store %"class.std::__1::basic_string"* %259, %"class.std::__1::basic_string"** %99, align 8
  %275 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %99, align 8
  %276 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %275, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %276, %"class.std::__1::__compressed_pair"** %98, align 8
  %277 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %98, align 8
  %278 = bitcast %"class.std::__1::__compressed_pair"* %277 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %278, %"class.std::__1::__libcpp_compressed_pair_imp"** %97, align 8
  %279 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %97, align 8
  %280 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %279, i32 0, i32 0
  %281 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %280, i32 0, i32 0
  %282 = bitcast %union.anon.0* %281 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %283 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %282, i32 0, i32 1
  %284 = load i64, i64* %283, align 8
  br label %300

; <label>:285:                                    ; preds = %255
  store %"class.std::__1::basic_string"* %259, %"class.std::__1::basic_string"** %102, align 8
  %286 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %102, align 8
  %287 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %286, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %287, %"class.std::__1::__compressed_pair"** %101, align 8
  %288 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %101, align 8
  %289 = bitcast %"class.std::__1::__compressed_pair"* %288 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %289, %"class.std::__1::__libcpp_compressed_pair_imp"** %100, align 8
  %290 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %100, align 8
  %291 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %290, i32 0, i32 0
  %292 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %291, i32 0, i32 0
  %293 = bitcast %union.anon.0* %292 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %294 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %293, i32 0, i32 0
  %295 = bitcast %union.anon.1* %294 to i8*
  %296 = load i8, i8* %295, align 8
  %297 = zext i8 %296 to i32
  %298 = ashr i32 %297, 1
  %299 = sext i32 %298 to i64
  br label %300

; <label>:300:                                    ; preds = %285, %274
  %301 = phi i64 [ %284, %274 ], [ %299, %285 ]
  %302 = icmp ne i64 %257, %301
  br i1 %302, label %303, label %304

; <label>:303:                                    ; preds = %300
  store i1 false, i1* %117, align 1
  br label %439

; <label>:304:                                    ; preds = %300
  %305 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %118, align 8
  store %"class.std::__1::basic_string"* %305, %"class.std::__1::basic_string"** %96, align 8
  %306 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %96, align 8
  store %"class.std::__1::basic_string"* %306, %"class.std::__1::basic_string"** %95, align 8
  %307 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %95, align 8
  store %"class.std::__1::basic_string"* %307, %"class.std::__1::basic_string"** %94, align 8
  %308 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %94, align 8
  %309 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %308, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %309, %"class.std::__1::__compressed_pair"** %93, align 8
  %310 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %93, align 8
  %311 = bitcast %"class.std::__1::__compressed_pair"* %310 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %311, %"class.std::__1::__libcpp_compressed_pair_imp"** %92, align 8
  %312 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %92, align 8
  %313 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %312, i32 0, i32 0
  %314 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %313, i32 0, i32 0
  %315 = bitcast %union.anon.0* %314 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %316 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %315, i32 0, i32 0
  %317 = bitcast %union.anon.1* %316 to i8*
  %318 = load i8, i8* %317, align 8
  %319 = zext i8 %318 to i32
  %320 = and i32 %319, 1
  %321 = icmp ne i32 %320, 0
  br i1 %321, label %322, label %333

; <label>:322:                                    ; preds = %304
  store %"class.std::__1::basic_string"* %307, %"class.std::__1::basic_string"** %86, align 8
  %323 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %86, align 8
  %324 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %323, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %324, %"class.std::__1::__compressed_pair"** %85, align 8
  %325 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %85, align 8
  %326 = bitcast %"class.std::__1::__compressed_pair"* %325 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %326, %"class.std::__1::__libcpp_compressed_pair_imp"** %84, align 8
  %327 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %84, align 8
  %328 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %327, i32 0, i32 0
  %329 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %328, i32 0, i32 0
  %330 = bitcast %union.anon.0* %329 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %331 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %330, i32 0, i32 2
  %332 = load i8*, i8** %331, align 8
  br label %346

; <label>:333:                                    ; preds = %304
  store %"class.std::__1::basic_string"* %307, %"class.std::__1::basic_string"** %91, align 8
  %334 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %91, align 8
  %335 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %334, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %335, %"class.std::__1::__compressed_pair"** %90, align 8
  %336 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %90, align 8
  %337 = bitcast %"class.std::__1::__compressed_pair"* %336 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %337, %"class.std::__1::__libcpp_compressed_pair_imp"** %89, align 8
  %338 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %89, align 8
  %339 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %338, i32 0, i32 0
  %340 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %339, i32 0, i32 0
  %341 = bitcast %union.anon.0* %340 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %342 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %341, i32 0, i32 1
  %343 = getelementptr inbounds [23 x i8], [23 x i8]* %342, i64 0, i64 0
  store i8* %343, i8** %88, align 8
  %344 = load i8*, i8** %88, align 8
  store i8* %344, i8** %87, align 8
  %345 = load i8*, i8** %87, align 8
  br label %346

; <label>:346:                                    ; preds = %333, %322
  %347 = phi i8* [ %332, %322 ], [ %345, %333 ]
  store i8* %347, i8** %83, align 8
  %348 = load i8*, i8** %83, align 8
  store i8* %348, i8** %121, align 8
  %349 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %119, align 8
  store %"class.std::__1::basic_string"* %349, %"class.std::__1::basic_string"** %79, align 8
  %350 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %79, align 8
  store %"class.std::__1::basic_string"* %350, %"class.std::__1::basic_string"** %78, align 8
  %351 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %78, align 8
  store %"class.std::__1::basic_string"* %351, %"class.std::__1::basic_string"** %77, align 8
  %352 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %77, align 8
  %353 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %352, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %353, %"class.std::__1::__compressed_pair"** %76, align 8
  %354 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %76, align 8
  %355 = bitcast %"class.std::__1::__compressed_pair"* %354 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %355, %"class.std::__1::__libcpp_compressed_pair_imp"** %75, align 8
  %356 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %75, align 8
  %357 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %356, i32 0, i32 0
  %358 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %357, i32 0, i32 0
  %359 = bitcast %union.anon.0* %358 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %360 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %359, i32 0, i32 0
  %361 = bitcast %union.anon.1* %360 to i8*
  %362 = load i8, i8* %361, align 8
  %363 = zext i8 %362 to i32
  %364 = and i32 %363, 1
  %365 = icmp ne i32 %364, 0
  br i1 %365, label %366, label %377

; <label>:366:                                    ; preds = %346
  store %"class.std::__1::basic_string"* %351, %"class.std::__1::basic_string"** %69, align 8
  %367 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %69, align 8
  %368 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %367, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %368, %"class.std::__1::__compressed_pair"** %68, align 8
  %369 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %68, align 8
  %370 = bitcast %"class.std::__1::__compressed_pair"* %369 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %370, %"class.std::__1::__libcpp_compressed_pair_imp"** %67, align 8
  %371 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %67, align 8
  %372 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %371, i32 0, i32 0
  %373 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %372, i32 0, i32 0
  %374 = bitcast %union.anon.0* %373 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %375 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %374, i32 0, i32 2
  %376 = load i8*, i8** %375, align 8
  br label %390

; <label>:377:                                    ; preds = %346
  store %"class.std::__1::basic_string"* %351, %"class.std::__1::basic_string"** %74, align 8
  %378 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %74, align 8
  %379 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %378, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %379, %"class.std::__1::__compressed_pair"** %73, align 8
  %380 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %73, align 8
  %381 = bitcast %"class.std::__1::__compressed_pair"* %380 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %381, %"class.std::__1::__libcpp_compressed_pair_imp"** %72, align 8
  %382 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %72, align 8
  %383 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %382, i32 0, i32 0
  %384 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %383, i32 0, i32 0
  %385 = bitcast %union.anon.0* %384 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %386 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %385, i32 0, i32 1
  %387 = getelementptr inbounds [23 x i8], [23 x i8]* %386, i64 0, i64 0
  store i8* %387, i8** %71, align 8
  %388 = load i8*, i8** %71, align 8
  store i8* %388, i8** %70, align 8
  %389 = load i8*, i8** %70, align 8
  br label %390

; <label>:390:                                    ; preds = %377, %366
  %391 = phi i8* [ %376, %366 ], [ %389, %377 ]
  store i8* %391, i8** %66, align 8
  %392 = load i8*, i8** %66, align 8
  store i8* %392, i8** %122, align 8
  %393 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %118, align 8
  store %"class.std::__1::basic_string"* %393, %"class.std::__1::basic_string"** %82, align 8
  %394 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %82, align 8
  %395 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %394, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %395, %"class.std::__1::__compressed_pair"** %81, align 8
  %396 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %81, align 8
  %397 = bitcast %"class.std::__1::__compressed_pair"* %396 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %397, %"class.std::__1::__libcpp_compressed_pair_imp"** %80, align 8
  %398 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %80, align 8
  %399 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %398, i32 0, i32 0
  %400 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %399, i32 0, i32 0
  %401 = bitcast %union.anon.0* %400 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %402 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %401, i32 0, i32 0
  %403 = bitcast %union.anon.1* %402 to i8*
  %404 = load i8, i8* %403, align 8
  %405 = zext i8 %404 to i32
  %406 = and i32 %405, 1
  %407 = icmp ne i32 %406, 0
  br i1 %407, label %408, label %415

; <label>:408:                                    ; preds = %390
  %409 = load i8*, i8** %121, align 8
  %410 = load i8*, i8** %122, align 8
  %411 = load i64, i64* %120, align 8
  %412 = invoke i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8* %409, i8* %410, i64 %411)
          to label %413 unwind label %436

; <label>:413:                                    ; preds = %408
  %414 = icmp eq i32 %412, 0
  store i1 %414, i1* %117, align 1
  br label %439

; <label>:415:                                    ; preds = %390
  br label %416

; <label>:416:                                    ; preds = %428, %415
  %417 = load i64, i64* %120, align 8
  %418 = icmp ne i64 %417, 0
  br i1 %418, label %419, label %435

; <label>:419:                                    ; preds = %416
  %420 = load i8*, i8** %121, align 8
  %421 = load i8, i8* %420, align 1
  %422 = sext i8 %421 to i32
  %423 = load i8*, i8** %122, align 8
  %424 = load i8, i8* %423, align 1
  %425 = sext i8 %424 to i32
  %426 = icmp ne i32 %422, %425
  br i1 %426, label %427, label %428

; <label>:427:                                    ; preds = %419
  store i1 false, i1* %117, align 1
  br label %439

; <label>:428:                                    ; preds = %419
  %429 = load i64, i64* %120, align 8
  %430 = add i64 %429, -1
  store i64 %430, i64* %120, align 8
  %431 = load i8*, i8** %121, align 8
  %432 = getelementptr inbounds i8, i8* %431, i32 1
  store i8* %432, i8** %121, align 8
  %433 = load i8*, i8** %122, align 8
  %434 = getelementptr inbounds i8, i8* %433, i32 1
  store i8* %434, i8** %122, align 8
  br label %416

; <label>:435:                                    ; preds = %416
  store i1 true, i1* %117, align 1
  br label %439

; <label>:436:                                    ; preds = %408
  %437 = landingpad { i8*, i32 }
          catch i8* null
  %438 = extractvalue { i8*, i32 } %437, 0
  call void @__clang_call_terminate(i8* %438) #12
  unreachable

; <label>:439:                                    ; preds = %303, %413, %427, %435
  %440 = load i1, i1* %117, align 1
  br i1 %440, label %441, label %442

; <label>:441:                                    ; preds = %439
  store i1 true, i1* %183, align 1
  br label %963

; <label>:442:                                    ; preds = %439
  br label %443

; <label>:443:                                    ; preds = %442
  %444 = load i32, i32* %185, align 4
  %445 = add nsw i32 %444, 1
  store i32 %445, i32* %185, align 4
  br label %189

; <label>:446:                                    ; preds = %189
  store i32 0, i32* %186, align 4
  br label %447

; <label>:447:                                    ; preds = %701, %446
  %448 = load i32, i32* %186, align 4
  %449 = sext i32 %448 to i64
  store %"class.std::__1::vector"* @_ZN4util15unary_operatorsE, %"class.std::__1::vector"** %65, align 8
  %450 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %65, align 8
  %451 = bitcast %"class.std::__1::vector"* %450 to %"class.std::__1::__vector_base"*
  %452 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %451, i32 0, i32 1
  %453 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %452, align 8
  %454 = bitcast %"class.std::__1::vector"* %450 to %"class.std::__1::__vector_base"*
  %455 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %454, i32 0, i32 0
  %456 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %455, align 8
  %457 = ptrtoint %"class.std::__1::basic_string"* %453 to i64
  %458 = ptrtoint %"class.std::__1::basic_string"* %456 to i64
  %459 = sub i64 %457, %458
  %460 = sdiv exact i64 %459, 24
  %461 = icmp ult i64 %449, %460
  br i1 %461, label %462, label %704

; <label>:462:                                    ; preds = %447
  %463 = load i32, i32* %186, align 4
  %464 = sext i32 %463 to i64
  store %"class.std::__1::vector"* @_ZN4util15unary_operatorsE, %"class.std::__1::vector"** %63, align 8
  store i64 %464, i64* %64, align 8
  %465 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %63, align 8
  %466 = load i64, i64* %64, align 8
  %467 = bitcast %"class.std::__1::vector"* %465 to %"class.std::__1::__vector_base"*
  %468 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %467, i32 0, i32 0
  %469 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %468, align 8
  %470 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %469, i64 %466
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %55, align 8
  store %"class.std::__1::basic_string"* %470, %"class.std::__1::basic_string"** %56, align 8
  %471 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %55, align 8
  store %"class.std::__1::basic_string"* %471, %"class.std::__1::basic_string"** %53, align 8
  %472 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8
  store %"class.std::__1::basic_string"* %472, %"class.std::__1::basic_string"** %52, align 8
  %473 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8
  %474 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %473, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %474, %"class.std::__1::__compressed_pair"** %51, align 8
  %475 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %51, align 8
  %476 = bitcast %"class.std::__1::__compressed_pair"* %475 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %476, %"class.std::__1::__libcpp_compressed_pair_imp"** %50, align 8
  %477 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %50, align 8
  %478 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %477, i32 0, i32 0
  %479 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %478, i32 0, i32 0
  %480 = bitcast %union.anon.0* %479 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %481 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %480, i32 0, i32 0
  %482 = bitcast %union.anon.1* %481 to i8*
  %483 = load i8, i8* %482, align 8
  %484 = zext i8 %483 to i32
  %485 = and i32 %484, 1
  %486 = icmp ne i32 %485, 0
  br i1 %486, label %487, label %498

; <label>:487:                                    ; preds = %462
  store %"class.std::__1::basic_string"* %472, %"class.std::__1::basic_string"** %46, align 8
  %488 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %46, align 8
  %489 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %488, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %489, %"class.std::__1::__compressed_pair"** %45, align 8
  %490 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %45, align 8
  %491 = bitcast %"class.std::__1::__compressed_pair"* %490 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %491, %"class.std::__1::__libcpp_compressed_pair_imp"** %44, align 8
  %492 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %44, align 8
  %493 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %492, i32 0, i32 0
  %494 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %493, i32 0, i32 0
  %495 = bitcast %union.anon.0* %494 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %496 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %495, i32 0, i32 1
  %497 = load i64, i64* %496, align 8
  br label %513

; <label>:498:                                    ; preds = %462
  store %"class.std::__1::basic_string"* %472, %"class.std::__1::basic_string"** %49, align 8
  %499 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8
  %500 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %499, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %500, %"class.std::__1::__compressed_pair"** %48, align 8
  %501 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8
  %502 = bitcast %"class.std::__1::__compressed_pair"* %501 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %502, %"class.std::__1::__libcpp_compressed_pair_imp"** %47, align 8
  %503 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %47, align 8
  %504 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %503, i32 0, i32 0
  %505 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %504, i32 0, i32 0
  %506 = bitcast %union.anon.0* %505 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %507 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %506, i32 0, i32 0
  %508 = bitcast %union.anon.1* %507 to i8*
  %509 = load i8, i8* %508, align 8
  %510 = zext i8 %509 to i32
  %511 = ashr i32 %510, 1
  %512 = sext i32 %511 to i64
  br label %513

; <label>:513:                                    ; preds = %498, %487
  %514 = phi i64 [ %497, %487 ], [ %512, %498 ]
  store i64 %514, i64* %57, align 8
  %515 = load i64, i64* %57, align 8
  %516 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %56, align 8
  store %"class.std::__1::basic_string"* %516, %"class.std::__1::basic_string"** %43, align 8
  %517 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %43, align 8
  store %"class.std::__1::basic_string"* %517, %"class.std::__1::basic_string"** %42, align 8
  %518 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8
  %519 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %518, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %519, %"class.std::__1::__compressed_pair"** %41, align 8
  %520 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %41, align 8
  %521 = bitcast %"class.std::__1::__compressed_pair"* %520 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %521, %"class.std::__1::__libcpp_compressed_pair_imp"** %40, align 8
  %522 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %40, align 8
  %523 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %522, i32 0, i32 0
  %524 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %523, i32 0, i32 0
  %525 = bitcast %union.anon.0* %524 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %526 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %525, i32 0, i32 0
  %527 = bitcast %union.anon.1* %526 to i8*
  %528 = load i8, i8* %527, align 8
  %529 = zext i8 %528 to i32
  %530 = and i32 %529, 1
  %531 = icmp ne i32 %530, 0
  br i1 %531, label %532, label %543

; <label>:532:                                    ; preds = %513
  store %"class.std::__1::basic_string"* %517, %"class.std::__1::basic_string"** %36, align 8
  %533 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8
  %534 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %533, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %534, %"class.std::__1::__compressed_pair"** %35, align 8
  %535 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8
  %536 = bitcast %"class.std::__1::__compressed_pair"* %535 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %536, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8
  %537 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8
  %538 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %537, i32 0, i32 0
  %539 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %538, i32 0, i32 0
  %540 = bitcast %union.anon.0* %539 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %541 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %540, i32 0, i32 1
  %542 = load i64, i64* %541, align 8
  br label %558

; <label>:543:                                    ; preds = %513
  store %"class.std::__1::basic_string"* %517, %"class.std::__1::basic_string"** %39, align 8
  %544 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8
  %545 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %544, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %545, %"class.std::__1::__compressed_pair"** %38, align 8
  %546 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %38, align 8
  %547 = bitcast %"class.std::__1::__compressed_pair"* %546 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %547, %"class.std::__1::__libcpp_compressed_pair_imp"** %37, align 8
  %548 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %37, align 8
  %549 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %548, i32 0, i32 0
  %550 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %549, i32 0, i32 0
  %551 = bitcast %union.anon.0* %550 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %552 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %551, i32 0, i32 0
  %553 = bitcast %union.anon.1* %552 to i8*
  %554 = load i8, i8* %553, align 8
  %555 = zext i8 %554 to i32
  %556 = ashr i32 %555, 1
  %557 = sext i32 %556 to i64
  br label %558

; <label>:558:                                    ; preds = %543, %532
  %559 = phi i64 [ %542, %532 ], [ %557, %543 ]
  %560 = icmp ne i64 %515, %559
  br i1 %560, label %561, label %562

; <label>:561:                                    ; preds = %558
  store i1 false, i1* %54, align 1
  br label %697

; <label>:562:                                    ; preds = %558
  %563 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %55, align 8
  store %"class.std::__1::basic_string"* %563, %"class.std::__1::basic_string"** %33, align 8
  %564 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8
  store %"class.std::__1::basic_string"* %564, %"class.std::__1::basic_string"** %32, align 8
  %565 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %32, align 8
  store %"class.std::__1::basic_string"* %565, %"class.std::__1::basic_string"** %31, align 8
  %566 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %31, align 8
  %567 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %566, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %567, %"class.std::__1::__compressed_pair"** %30, align 8
  %568 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %30, align 8
  %569 = bitcast %"class.std::__1::__compressed_pair"* %568 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %569, %"class.std::__1::__libcpp_compressed_pair_imp"** %29, align 8
  %570 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %29, align 8
  %571 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %570, i32 0, i32 0
  %572 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %571, i32 0, i32 0
  %573 = bitcast %union.anon.0* %572 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %574 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %573, i32 0, i32 0
  %575 = bitcast %union.anon.1* %574 to i8*
  %576 = load i8, i8* %575, align 8
  %577 = zext i8 %576 to i32
  %578 = and i32 %577, 1
  %579 = icmp ne i32 %578, 0
  br i1 %579, label %580, label %591

; <label>:580:                                    ; preds = %562
  store %"class.std::__1::basic_string"* %565, %"class.std::__1::basic_string"** %23, align 8
  %581 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8
  %582 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %581, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %582, %"class.std::__1::__compressed_pair"** %22, align 8
  %583 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8
  %584 = bitcast %"class.std::__1::__compressed_pair"* %583 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %584, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8
  %585 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8
  %586 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %585, i32 0, i32 0
  %587 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %586, i32 0, i32 0
  %588 = bitcast %union.anon.0* %587 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %589 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %588, i32 0, i32 2
  %590 = load i8*, i8** %589, align 8
  br label %604

; <label>:591:                                    ; preds = %562
  store %"class.std::__1::basic_string"* %565, %"class.std::__1::basic_string"** %28, align 8
  %592 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8
  %593 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %592, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %593, %"class.std::__1::__compressed_pair"** %27, align 8
  %594 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8
  %595 = bitcast %"class.std::__1::__compressed_pair"* %594 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %595, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8
  %596 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8
  %597 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %596, i32 0, i32 0
  %598 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %597, i32 0, i32 0
  %599 = bitcast %union.anon.0* %598 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %600 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %599, i32 0, i32 1
  %601 = getelementptr inbounds [23 x i8], [23 x i8]* %600, i64 0, i64 0
  store i8* %601, i8** %25, align 8
  %602 = load i8*, i8** %25, align 8
  store i8* %602, i8** %24, align 8
  %603 = load i8*, i8** %24, align 8
  br label %604

; <label>:604:                                    ; preds = %591, %580
  %605 = phi i8* [ %590, %580 ], [ %603, %591 ]
  store i8* %605, i8** %20, align 8
  %606 = load i8*, i8** %20, align 8
  store i8* %606, i8** %58, align 8
  %607 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %56, align 8
  store %"class.std::__1::basic_string"* %607, %"class.std::__1::basic_string"** %16, align 8
  %608 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %16, align 8
  store %"class.std::__1::basic_string"* %608, %"class.std::__1::basic_string"** %15, align 8
  %609 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %15, align 8
  store %"class.std::__1::basic_string"* %609, %"class.std::__1::basic_string"** %14, align 8
  %610 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8
  %611 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %610, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %611, %"class.std::__1::__compressed_pair"** %13, align 8
  %612 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %13, align 8
  %613 = bitcast %"class.std::__1::__compressed_pair"* %612 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %613, %"class.std::__1::__libcpp_compressed_pair_imp"** %12, align 8
  %614 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %12, align 8
  %615 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %614, i32 0, i32 0
  %616 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %615, i32 0, i32 0
  %617 = bitcast %union.anon.0* %616 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %618 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %617, i32 0, i32 0
  %619 = bitcast %union.anon.1* %618 to i8*
  %620 = load i8, i8* %619, align 8
  %621 = zext i8 %620 to i32
  %622 = and i32 %621, 1
  %623 = icmp ne i32 %622, 0
  br i1 %623, label %624, label %635

; <label>:624:                                    ; preds = %604
  store %"class.std::__1::basic_string"* %609, %"class.std::__1::basic_string"** %6, align 8
  %625 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8
  %626 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %625, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %626, %"class.std::__1::__compressed_pair"** %5, align 8
  %627 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8
  %628 = bitcast %"class.std::__1::__compressed_pair"* %627 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %628, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8
  %629 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8
  %630 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %629, i32 0, i32 0
  %631 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %630, i32 0, i32 0
  %632 = bitcast %union.anon.0* %631 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %633 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %632, i32 0, i32 2
  %634 = load i8*, i8** %633, align 8
  br label %648

; <label>:635:                                    ; preds = %604
  store %"class.std::__1::basic_string"* %609, %"class.std::__1::basic_string"** %11, align 8
  %636 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8
  %637 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %636, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %637, %"class.std::__1::__compressed_pair"** %10, align 8
  %638 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %10, align 8
  %639 = bitcast %"class.std::__1::__compressed_pair"* %638 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %639, %"class.std::__1::__libcpp_compressed_pair_imp"** %9, align 8
  %640 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %9, align 8
  %641 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %640, i32 0, i32 0
  %642 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %641, i32 0, i32 0
  %643 = bitcast %union.anon.0* %642 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %644 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %643, i32 0, i32 1
  %645 = getelementptr inbounds [23 x i8], [23 x i8]* %644, i64 0, i64 0
  store i8* %645, i8** %8, align 8
  %646 = load i8*, i8** %8, align 8
  store i8* %646, i8** %7, align 8
  %647 = load i8*, i8** %7, align 8
  br label %648

; <label>:648:                                    ; preds = %635, %624
  %649 = phi i8* [ %634, %624 ], [ %647, %635 ]
  store i8* %649, i8** %3, align 8
  %650 = load i8*, i8** %3, align 8
  store i8* %650, i8** %59, align 8
  %651 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %55, align 8
  store %"class.std::__1::basic_string"* %651, %"class.std::__1::basic_string"** %19, align 8
  %652 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %19, align 8
  %653 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %652, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %653, %"class.std::__1::__compressed_pair"** %18, align 8
  %654 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %18, align 8
  %655 = bitcast %"class.std::__1::__compressed_pair"* %654 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %655, %"class.std::__1::__libcpp_compressed_pair_imp"** %17, align 8
  %656 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %17, align 8
  %657 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %656, i32 0, i32 0
  %658 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %657, i32 0, i32 0
  %659 = bitcast %union.anon.0* %658 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %660 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %659, i32 0, i32 0
  %661 = bitcast %union.anon.1* %660 to i8*
  %662 = load i8, i8* %661, align 8
  %663 = zext i8 %662 to i32
  %664 = and i32 %663, 1
  %665 = icmp ne i32 %664, 0
  br i1 %665, label %666, label %673

; <label>:666:                                    ; preds = %648
  %667 = load i8*, i8** %58, align 8
  %668 = load i8*, i8** %59, align 8
  %669 = load i64, i64* %57, align 8
  %670 = invoke i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8* %667, i8* %668, i64 %669)
          to label %671 unwind label %694

; <label>:671:                                    ; preds = %666
  %672 = icmp eq i32 %670, 0
  store i1 %672, i1* %54, align 1
  br label %697

; <label>:673:                                    ; preds = %648
  br label %674

; <label>:674:                                    ; preds = %686, %673
  %675 = load i64, i64* %57, align 8
  %676 = icmp ne i64 %675, 0
  br i1 %676, label %677, label %693

; <label>:677:                                    ; preds = %674
  %678 = load i8*, i8** %58, align 8
  %679 = load i8, i8* %678, align 1
  %680 = sext i8 %679 to i32
  %681 = load i8*, i8** %59, align 8
  %682 = load i8, i8* %681, align 1
  %683 = sext i8 %682 to i32
  %684 = icmp ne i32 %680, %683
  br i1 %684, label %685, label %686

; <label>:685:                                    ; preds = %677
  store i1 false, i1* %54, align 1
  br label %697

; <label>:686:                                    ; preds = %677
  %687 = load i64, i64* %57, align 8
  %688 = add i64 %687, -1
  store i64 %688, i64* %57, align 8
  %689 = load i8*, i8** %58, align 8
  %690 = getelementptr inbounds i8, i8* %689, i32 1
  store i8* %690, i8** %58, align 8
  %691 = load i8*, i8** %59, align 8
  %692 = getelementptr inbounds i8, i8* %691, i32 1
  store i8* %692, i8** %59, align 8
  br label %674

; <label>:693:                                    ; preds = %674
  store i1 true, i1* %54, align 1
  br label %697

; <label>:694:                                    ; preds = %666
  %695 = landingpad { i8*, i32 }
          catch i8* null
  %696 = extractvalue { i8*, i32 } %695, 0
  call void @__clang_call_terminate(i8* %696) #12
  unreachable

; <label>:697:                                    ; preds = %561, %671, %685, %693
  %698 = load i1, i1* %54, align 1
  br i1 %698, label %699, label %700

; <label>:699:                                    ; preds = %697
  store i1 true, i1* %183, align 1
  br label %963

; <label>:700:                                    ; preds = %697
  br label %701

; <label>:701:                                    ; preds = %700
  %702 = load i32, i32* %186, align 4
  %703 = add nsw i32 %702, 1
  store i32 %703, i32* %186, align 4
  br label %447

; <label>:704:                                    ; preds = %447
  store i32 0, i32* %187, align 4
  br label %705

; <label>:705:                                    ; preds = %959, %704
  %706 = load i32, i32* %187, align 4
  %707 = sext i32 %706 to i64
  store %"class.std::__1::vector"* @_ZN4util18tertiary_operatorsE, %"class.std::__1::vector"** %60, align 8
  %708 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %60, align 8
  %709 = bitcast %"class.std::__1::vector"* %708 to %"class.std::__1::__vector_base"*
  %710 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %709, i32 0, i32 1
  %711 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %710, align 8
  %712 = bitcast %"class.std::__1::vector"* %708 to %"class.std::__1::__vector_base"*
  %713 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %712, i32 0, i32 0
  %714 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %713, align 8
  %715 = ptrtoint %"class.std::__1::basic_string"* %711 to i64
  %716 = ptrtoint %"class.std::__1::basic_string"* %714 to i64
  %717 = sub i64 %715, %716
  %718 = sdiv exact i64 %717, 24
  %719 = icmp ult i64 %707, %718
  br i1 %719, label %720, label %962

; <label>:720:                                    ; preds = %705
  %721 = load i32, i32* %187, align 4
  %722 = sext i32 %721 to i64
  store %"class.std::__1::vector"* @_ZN4util18tertiary_operatorsE, %"class.std::__1::vector"** %61, align 8
  store i64 %722, i64* %62, align 8
  %723 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %61, align 8
  %724 = load i64, i64* %62, align 8
  %725 = bitcast %"class.std::__1::vector"* %723 to %"class.std::__1::__vector_base"*
  %726 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %725, i32 0, i32 0
  %727 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %726, align 8
  %728 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %727, i64 %724
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %177, align 8
  store %"class.std::__1::basic_string"* %728, %"class.std::__1::basic_string"** %178, align 8
  %729 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %177, align 8
  store %"class.std::__1::basic_string"* %729, %"class.std::__1::basic_string"** %175, align 8
  %730 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %175, align 8
  store %"class.std::__1::basic_string"* %730, %"class.std::__1::basic_string"** %174, align 8
  %731 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %174, align 8
  %732 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %731, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %732, %"class.std::__1::__compressed_pair"** %173, align 8
  %733 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %173, align 8
  %734 = bitcast %"class.std::__1::__compressed_pair"* %733 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %734, %"class.std::__1::__libcpp_compressed_pair_imp"** %172, align 8
  %735 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %172, align 8
  %736 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %735, i32 0, i32 0
  %737 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %736, i32 0, i32 0
  %738 = bitcast %union.anon.0* %737 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %739 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %738, i32 0, i32 0
  %740 = bitcast %union.anon.1* %739 to i8*
  %741 = load i8, i8* %740, align 8
  %742 = zext i8 %741 to i32
  %743 = and i32 %742, 1
  %744 = icmp ne i32 %743, 0
  br i1 %744, label %745, label %756

; <label>:745:                                    ; preds = %720
  store %"class.std::__1::basic_string"* %730, %"class.std::__1::basic_string"** %168, align 8
  %746 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %168, align 8
  %747 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %746, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %747, %"class.std::__1::__compressed_pair"** %167, align 8
  %748 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %167, align 8
  %749 = bitcast %"class.std::__1::__compressed_pair"* %748 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %749, %"class.std::__1::__libcpp_compressed_pair_imp"** %166, align 8
  %750 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %166, align 8
  %751 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %750, i32 0, i32 0
  %752 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %751, i32 0, i32 0
  %753 = bitcast %union.anon.0* %752 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %754 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %753, i32 0, i32 1
  %755 = load i64, i64* %754, align 8
  br label %771

; <label>:756:                                    ; preds = %720
  store %"class.std::__1::basic_string"* %730, %"class.std::__1::basic_string"** %171, align 8
  %757 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %171, align 8
  %758 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %757, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %758, %"class.std::__1::__compressed_pair"** %170, align 8
  %759 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %170, align 8
  %760 = bitcast %"class.std::__1::__compressed_pair"* %759 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %760, %"class.std::__1::__libcpp_compressed_pair_imp"** %169, align 8
  %761 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %169, align 8
  %762 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %761, i32 0, i32 0
  %763 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %762, i32 0, i32 0
  %764 = bitcast %union.anon.0* %763 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %765 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %764, i32 0, i32 0
  %766 = bitcast %union.anon.1* %765 to i8*
  %767 = load i8, i8* %766, align 8
  %768 = zext i8 %767 to i32
  %769 = ashr i32 %768, 1
  %770 = sext i32 %769 to i64
  br label %771

; <label>:771:                                    ; preds = %756, %745
  %772 = phi i64 [ %755, %745 ], [ %770, %756 ]
  store i64 %772, i64* %179, align 8
  %773 = load i64, i64* %179, align 8
  %774 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %178, align 8
  store %"class.std::__1::basic_string"* %774, %"class.std::__1::basic_string"** %165, align 8
  %775 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %165, align 8
  store %"class.std::__1::basic_string"* %775, %"class.std::__1::basic_string"** %164, align 8
  %776 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %164, align 8
  %777 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %776, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %777, %"class.std::__1::__compressed_pair"** %163, align 8
  %778 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %163, align 8
  %779 = bitcast %"class.std::__1::__compressed_pair"* %778 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %779, %"class.std::__1::__libcpp_compressed_pair_imp"** %162, align 8
  %780 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %162, align 8
  %781 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %780, i32 0, i32 0
  %782 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %781, i32 0, i32 0
  %783 = bitcast %union.anon.0* %782 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %784 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %783, i32 0, i32 0
  %785 = bitcast %union.anon.1* %784 to i8*
  %786 = load i8, i8* %785, align 8
  %787 = zext i8 %786 to i32
  %788 = and i32 %787, 1
  %789 = icmp ne i32 %788, 0
  br i1 %789, label %790, label %801

; <label>:790:                                    ; preds = %771
  store %"class.std::__1::basic_string"* %775, %"class.std::__1::basic_string"** %158, align 8
  %791 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %158, align 8
  %792 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %791, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %792, %"class.std::__1::__compressed_pair"** %157, align 8
  %793 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %157, align 8
  %794 = bitcast %"class.std::__1::__compressed_pair"* %793 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %794, %"class.std::__1::__libcpp_compressed_pair_imp"** %156, align 8
  %795 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %156, align 8
  %796 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %795, i32 0, i32 0
  %797 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %796, i32 0, i32 0
  %798 = bitcast %union.anon.0* %797 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %799 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %798, i32 0, i32 1
  %800 = load i64, i64* %799, align 8
  br label %816

; <label>:801:                                    ; preds = %771
  store %"class.std::__1::basic_string"* %775, %"class.std::__1::basic_string"** %161, align 8
  %802 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %161, align 8
  %803 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %802, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %803, %"class.std::__1::__compressed_pair"** %160, align 8
  %804 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %160, align 8
  %805 = bitcast %"class.std::__1::__compressed_pair"* %804 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %805, %"class.std::__1::__libcpp_compressed_pair_imp"** %159, align 8
  %806 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %159, align 8
  %807 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %806, i32 0, i32 0
  %808 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %807, i32 0, i32 0
  %809 = bitcast %union.anon.0* %808 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %810 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %809, i32 0, i32 0
  %811 = bitcast %union.anon.1* %810 to i8*
  %812 = load i8, i8* %811, align 8
  %813 = zext i8 %812 to i32
  %814 = ashr i32 %813, 1
  %815 = sext i32 %814 to i64
  br label %816

; <label>:816:                                    ; preds = %801, %790
  %817 = phi i64 [ %800, %790 ], [ %815, %801 ]
  %818 = icmp ne i64 %773, %817
  br i1 %818, label %819, label %820

; <label>:819:                                    ; preds = %816
  store i1 false, i1* %176, align 1
  br label %955

; <label>:820:                                    ; preds = %816
  %821 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %177, align 8
  store %"class.std::__1::basic_string"* %821, %"class.std::__1::basic_string"** %155, align 8
  %822 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %155, align 8
  store %"class.std::__1::basic_string"* %822, %"class.std::__1::basic_string"** %154, align 8
  %823 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %154, align 8
  store %"class.std::__1::basic_string"* %823, %"class.std::__1::basic_string"** %153, align 8
  %824 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %153, align 8
  %825 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %824, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %825, %"class.std::__1::__compressed_pair"** %152, align 8
  %826 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %152, align 8
  %827 = bitcast %"class.std::__1::__compressed_pair"* %826 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %827, %"class.std::__1::__libcpp_compressed_pair_imp"** %151, align 8
  %828 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %151, align 8
  %829 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %828, i32 0, i32 0
  %830 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %829, i32 0, i32 0
  %831 = bitcast %union.anon.0* %830 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %832 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %831, i32 0, i32 0
  %833 = bitcast %union.anon.1* %832 to i8*
  %834 = load i8, i8* %833, align 8
  %835 = zext i8 %834 to i32
  %836 = and i32 %835, 1
  %837 = icmp ne i32 %836, 0
  br i1 %837, label %838, label %849

; <label>:838:                                    ; preds = %820
  store %"class.std::__1::basic_string"* %823, %"class.std::__1::basic_string"** %145, align 8
  %839 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %145, align 8
  %840 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %839, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %840, %"class.std::__1::__compressed_pair"** %144, align 8
  %841 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %144, align 8
  %842 = bitcast %"class.std::__1::__compressed_pair"* %841 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %842, %"class.std::__1::__libcpp_compressed_pair_imp"** %143, align 8
  %843 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %143, align 8
  %844 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %843, i32 0, i32 0
  %845 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %844, i32 0, i32 0
  %846 = bitcast %union.anon.0* %845 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %847 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %846, i32 0, i32 2
  %848 = load i8*, i8** %847, align 8
  br label %862

; <label>:849:                                    ; preds = %820
  store %"class.std::__1::basic_string"* %823, %"class.std::__1::basic_string"** %150, align 8
  %850 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %150, align 8
  %851 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %850, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %851, %"class.std::__1::__compressed_pair"** %149, align 8
  %852 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %149, align 8
  %853 = bitcast %"class.std::__1::__compressed_pair"* %852 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %853, %"class.std::__1::__libcpp_compressed_pair_imp"** %148, align 8
  %854 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %148, align 8
  %855 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %854, i32 0, i32 0
  %856 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %855, i32 0, i32 0
  %857 = bitcast %union.anon.0* %856 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %858 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %857, i32 0, i32 1
  %859 = getelementptr inbounds [23 x i8], [23 x i8]* %858, i64 0, i64 0
  store i8* %859, i8** %147, align 8
  %860 = load i8*, i8** %147, align 8
  store i8* %860, i8** %146, align 8
  %861 = load i8*, i8** %146, align 8
  br label %862

; <label>:862:                                    ; preds = %849, %838
  %863 = phi i8* [ %848, %838 ], [ %861, %849 ]
  store i8* %863, i8** %142, align 8
  %864 = load i8*, i8** %142, align 8
  store i8* %864, i8** %180, align 8
  %865 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %178, align 8
  store %"class.std::__1::basic_string"* %865, %"class.std::__1::basic_string"** %138, align 8
  %866 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %138, align 8
  store %"class.std::__1::basic_string"* %866, %"class.std::__1::basic_string"** %137, align 8
  %867 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %137, align 8
  store %"class.std::__1::basic_string"* %867, %"class.std::__1::basic_string"** %136, align 8
  %868 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %136, align 8
  %869 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %868, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %869, %"class.std::__1::__compressed_pair"** %135, align 8
  %870 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %135, align 8
  %871 = bitcast %"class.std::__1::__compressed_pair"* %870 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %871, %"class.std::__1::__libcpp_compressed_pair_imp"** %134, align 8
  %872 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %134, align 8
  %873 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %872, i32 0, i32 0
  %874 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %873, i32 0, i32 0
  %875 = bitcast %union.anon.0* %874 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %876 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %875, i32 0, i32 0
  %877 = bitcast %union.anon.1* %876 to i8*
  %878 = load i8, i8* %877, align 8
  %879 = zext i8 %878 to i32
  %880 = and i32 %879, 1
  %881 = icmp ne i32 %880, 0
  br i1 %881, label %882, label %893

; <label>:882:                                    ; preds = %862
  store %"class.std::__1::basic_string"* %867, %"class.std::__1::basic_string"** %128, align 8
  %883 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %128, align 8
  %884 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %883, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %884, %"class.std::__1::__compressed_pair"** %127, align 8
  %885 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %127, align 8
  %886 = bitcast %"class.std::__1::__compressed_pair"* %885 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %886, %"class.std::__1::__libcpp_compressed_pair_imp"** %126, align 8
  %887 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %126, align 8
  %888 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %887, i32 0, i32 0
  %889 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %888, i32 0, i32 0
  %890 = bitcast %union.anon.0* %889 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %891 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %890, i32 0, i32 2
  %892 = load i8*, i8** %891, align 8
  br label %906

; <label>:893:                                    ; preds = %862
  store %"class.std::__1::basic_string"* %867, %"class.std::__1::basic_string"** %133, align 8
  %894 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %133, align 8
  %895 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %894, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %895, %"class.std::__1::__compressed_pair"** %132, align 8
  %896 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %132, align 8
  %897 = bitcast %"class.std::__1::__compressed_pair"* %896 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %897, %"class.std::__1::__libcpp_compressed_pair_imp"** %131, align 8
  %898 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %131, align 8
  %899 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %898, i32 0, i32 0
  %900 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %899, i32 0, i32 0
  %901 = bitcast %union.anon.0* %900 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %902 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %901, i32 0, i32 1
  %903 = getelementptr inbounds [23 x i8], [23 x i8]* %902, i64 0, i64 0
  store i8* %903, i8** %130, align 8
  %904 = load i8*, i8** %130, align 8
  store i8* %904, i8** %129, align 8
  %905 = load i8*, i8** %129, align 8
  br label %906

; <label>:906:                                    ; preds = %893, %882
  %907 = phi i8* [ %892, %882 ], [ %905, %893 ]
  store i8* %907, i8** %125, align 8
  %908 = load i8*, i8** %125, align 8
  store i8* %908, i8** %181, align 8
  %909 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %177, align 8
  store %"class.std::__1::basic_string"* %909, %"class.std::__1::basic_string"** %141, align 8
  %910 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %141, align 8
  %911 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %910, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %911, %"class.std::__1::__compressed_pair"** %140, align 8
  %912 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %140, align 8
  %913 = bitcast %"class.std::__1::__compressed_pair"* %912 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %913, %"class.std::__1::__libcpp_compressed_pair_imp"** %139, align 8
  %914 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %139, align 8
  %915 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %914, i32 0, i32 0
  %916 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %915, i32 0, i32 0
  %917 = bitcast %union.anon.0* %916 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %918 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %917, i32 0, i32 0
  %919 = bitcast %union.anon.1* %918 to i8*
  %920 = load i8, i8* %919, align 8
  %921 = zext i8 %920 to i32
  %922 = and i32 %921, 1
  %923 = icmp ne i32 %922, 0
  br i1 %923, label %924, label %931

; <label>:924:                                    ; preds = %906
  %925 = load i8*, i8** %180, align 8
  %926 = load i8*, i8** %181, align 8
  %927 = load i64, i64* %179, align 8
  %928 = invoke i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8* %925, i8* %926, i64 %927)
          to label %929 unwind label %952

; <label>:929:                                    ; preds = %924
  %930 = icmp eq i32 %928, 0
  store i1 %930, i1* %176, align 1
  br label %955

; <label>:931:                                    ; preds = %906
  br label %932

; <label>:932:                                    ; preds = %944, %931
  %933 = load i64, i64* %179, align 8
  %934 = icmp ne i64 %933, 0
  br i1 %934, label %935, label %951

; <label>:935:                                    ; preds = %932
  %936 = load i8*, i8** %180, align 8
  %937 = load i8, i8* %936, align 1
  %938 = sext i8 %937 to i32
  %939 = load i8*, i8** %181, align 8
  %940 = load i8, i8* %939, align 1
  %941 = sext i8 %940 to i32
  %942 = icmp ne i32 %938, %941
  br i1 %942, label %943, label %944

; <label>:943:                                    ; preds = %935
  store i1 false, i1* %176, align 1
  br label %955

; <label>:944:                                    ; preds = %935
  %945 = load i64, i64* %179, align 8
  %946 = add i64 %945, -1
  store i64 %946, i64* %179, align 8
  %947 = load i8*, i8** %180, align 8
  %948 = getelementptr inbounds i8, i8* %947, i32 1
  store i8* %948, i8** %180, align 8
  %949 = load i8*, i8** %181, align 8
  %950 = getelementptr inbounds i8, i8* %949, i32 1
  store i8* %950, i8** %181, align 8
  br label %932

; <label>:951:                                    ; preds = %932
  store i1 true, i1* %176, align 1
  br label %955

; <label>:952:                                    ; preds = %924
  %953 = landingpad { i8*, i32 }
          catch i8* null
  %954 = extractvalue { i8*, i32 } %953, 0
  call void @__clang_call_terminate(i8* %954) #12
  unreachable

; <label>:955:                                    ; preds = %819, %929, %943, %951
  %956 = load i1, i1* %176, align 1
  br i1 %956, label %957, label %958

; <label>:957:                                    ; preds = %955
  store i1 true, i1* %183, align 1
  br label %963

; <label>:958:                                    ; preds = %955
  br label %959

; <label>:959:                                    ; preds = %958
  %960 = load i32, i32* %187, align 4
  %961 = add nsw i32 %960, 1
  store i32 %961, i32* %187, align 4
  br label %705

; <label>:962:                                    ; preds = %705
  store i1 false, i1* %183, align 1
  br label %963

; <label>:963:                                    ; preds = %962, %957, %699, %441
  %964 = load i1, i1* %183, align 1
  ret i1 %964
}

; Function Attrs: ssp uwtable
define zeroext i1 @_ZN5Lexer10is_keywordENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Lexer*, %"class.std::__1::basic_string"*) #0 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %3 = alloca %"class.std::__1::vector"*, align 8
  %4 = alloca i64, align 8
  %5 = alloca i8*, align 8
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %8 = alloca %"class.std::__1::basic_string"*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca i8*, align 8
  %11 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %12 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %13 = alloca %"class.std::__1::basic_string"*, align 8
  %14 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %15 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %16 = alloca %"class.std::__1::basic_string"*, align 8
  %17 = alloca %"class.std::__1::basic_string"*, align 8
  %18 = alloca %"class.std::__1::basic_string"*, align 8
  %19 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %20 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %21 = alloca %"class.std::__1::basic_string"*, align 8
  %22 = alloca i8*, align 8
  %23 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %24 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %25 = alloca %"class.std::__1::basic_string"*, align 8
  %26 = alloca i8*, align 8
  %27 = alloca i8*, align 8
  %28 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %29 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %30 = alloca %"class.std::__1::basic_string"*, align 8
  %31 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %32 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %33 = alloca %"class.std::__1::basic_string"*, align 8
  %34 = alloca %"class.std::__1::basic_string"*, align 8
  %35 = alloca %"class.std::__1::basic_string"*, align 8
  %36 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %37 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %38 = alloca %"class.std::__1::basic_string"*, align 8
  %39 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %40 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %41 = alloca %"class.std::__1::basic_string"*, align 8
  %42 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %43 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %44 = alloca %"class.std::__1::basic_string"*, align 8
  %45 = alloca %"class.std::__1::basic_string"*, align 8
  %46 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %47 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %48 = alloca %"class.std::__1::basic_string"*, align 8
  %49 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %50 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %51 = alloca %"class.std::__1::basic_string"*, align 8
  %52 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %53 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %54 = alloca %"class.std::__1::basic_string"*, align 8
  %55 = alloca %"class.std::__1::basic_string"*, align 8
  %56 = alloca i1, align 1
  %57 = alloca %"class.std::__1::basic_string"*, align 8
  %58 = alloca %"class.std::__1::basic_string"*, align 8
  %59 = alloca i64, align 8
  %60 = alloca i8*, align 8
  %61 = alloca i8*, align 8
  %62 = alloca %"class.std::__1::vector"*, align 8
  %63 = alloca i1, align 1
  %64 = alloca %class.Lexer*, align 8
  %65 = alloca i32, align 4
  store %class.Lexer* %0, %class.Lexer** %64, align 8
  %66 = load %class.Lexer*, %class.Lexer** %64, align 8
  store i32 0, i32* %65, align 4
  br label %67

; <label>:67:                                     ; preds = %321, %2
  %68 = load i32, i32* %65, align 4
  %69 = sext i32 %68 to i64
  store %"class.std::__1::vector"* @_ZN4util8keywordsE, %"class.std::__1::vector"** %62, align 8
  %70 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %62, align 8
  %71 = bitcast %"class.std::__1::vector"* %70 to %"class.std::__1::__vector_base"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %71, i32 0, i32 1
  %73 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %72, align 8
  %74 = bitcast %"class.std::__1::vector"* %70 to %"class.std::__1::__vector_base"*
  %75 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %74, i32 0, i32 0
  %76 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %75, align 8
  %77 = ptrtoint %"class.std::__1::basic_string"* %73 to i64
  %78 = ptrtoint %"class.std::__1::basic_string"* %76 to i64
  %79 = sub i64 %77, %78
  %80 = sdiv exact i64 %79, 24
  %81 = icmp ult i64 %69, %80
  br i1 %81, label %82, label %324

; <label>:82:                                     ; preds = %67
  %83 = load i32, i32* %65, align 4
  %84 = sext i32 %83 to i64
  store %"class.std::__1::vector"* @_ZN4util8keywordsE, %"class.std::__1::vector"** %3, align 8
  store i64 %84, i64* %4, align 8
  %85 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %3, align 8
  %86 = load i64, i64* %4, align 8
  %87 = bitcast %"class.std::__1::vector"* %85 to %"class.std::__1::__vector_base"*
  %88 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %87, i32 0, i32 0
  %89 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %88, align 8
  %90 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %89, i64 %86
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %57, align 8
  store %"class.std::__1::basic_string"* %90, %"class.std::__1::basic_string"** %58, align 8
  %91 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %57, align 8
  store %"class.std::__1::basic_string"* %91, %"class.std::__1::basic_string"** %55, align 8
  %92 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %55, align 8
  store %"class.std::__1::basic_string"* %92, %"class.std::__1::basic_string"** %54, align 8
  %93 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8
  %94 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %93, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %94, %"class.std::__1::__compressed_pair"** %53, align 8
  %95 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %53, align 8
  %96 = bitcast %"class.std::__1::__compressed_pair"* %95 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %96, %"class.std::__1::__libcpp_compressed_pair_imp"** %52, align 8
  %97 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %52, align 8
  %98 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %97, i32 0, i32 0
  %99 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %98, i32 0, i32 0
  %100 = bitcast %union.anon.0* %99 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %101 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %100, i32 0, i32 0
  %102 = bitcast %union.anon.1* %101 to i8*
  %103 = load i8, i8* %102, align 8
  %104 = zext i8 %103 to i32
  %105 = and i32 %104, 1
  %106 = icmp ne i32 %105, 0
  br i1 %106, label %107, label %118

; <label>:107:                                    ; preds = %82
  store %"class.std::__1::basic_string"* %92, %"class.std::__1::basic_string"** %48, align 8
  %108 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %48, align 8
  %109 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %108, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %109, %"class.std::__1::__compressed_pair"** %47, align 8
  %110 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %47, align 8
  %111 = bitcast %"class.std::__1::__compressed_pair"* %110 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %111, %"class.std::__1::__libcpp_compressed_pair_imp"** %46, align 8
  %112 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %46, align 8
  %113 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %112, i32 0, i32 0
  %114 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %113, i32 0, i32 0
  %115 = bitcast %union.anon.0* %114 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %116 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %115, i32 0, i32 1
  %117 = load i64, i64* %116, align 8
  br label %133

; <label>:118:                                    ; preds = %82
  store %"class.std::__1::basic_string"* %92, %"class.std::__1::basic_string"** %51, align 8
  %119 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8
  %120 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %119, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %120, %"class.std::__1::__compressed_pair"** %50, align 8
  %121 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %50, align 8
  %122 = bitcast %"class.std::__1::__compressed_pair"* %121 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %122, %"class.std::__1::__libcpp_compressed_pair_imp"** %49, align 8
  %123 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %49, align 8
  %124 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %123, i32 0, i32 0
  %125 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %124, i32 0, i32 0
  %126 = bitcast %union.anon.0* %125 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %127 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %126, i32 0, i32 0
  %128 = bitcast %union.anon.1* %127 to i8*
  %129 = load i8, i8* %128, align 8
  %130 = zext i8 %129 to i32
  %131 = ashr i32 %130, 1
  %132 = sext i32 %131 to i64
  br label %133

; <label>:133:                                    ; preds = %118, %107
  %134 = phi i64 [ %117, %107 ], [ %132, %118 ]
  store i64 %134, i64* %59, align 8
  %135 = load i64, i64* %59, align 8
  %136 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8
  store %"class.std::__1::basic_string"* %136, %"class.std::__1::basic_string"** %45, align 8
  %137 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8
  store %"class.std::__1::basic_string"* %137, %"class.std::__1::basic_string"** %44, align 8
  %138 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %44, align 8
  %139 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %138, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %139, %"class.std::__1::__compressed_pair"** %43, align 8
  %140 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %43, align 8
  %141 = bitcast %"class.std::__1::__compressed_pair"* %140 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %141, %"class.std::__1::__libcpp_compressed_pair_imp"** %42, align 8
  %142 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %42, align 8
  %143 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %142, i32 0, i32 0
  %144 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %143, i32 0, i32 0
  %145 = bitcast %union.anon.0* %144 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %146 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %145, i32 0, i32 0
  %147 = bitcast %union.anon.1* %146 to i8*
  %148 = load i8, i8* %147, align 8
  %149 = zext i8 %148 to i32
  %150 = and i32 %149, 1
  %151 = icmp ne i32 %150, 0
  br i1 %151, label %152, label %163

; <label>:152:                                    ; preds = %133
  store %"class.std::__1::basic_string"* %137, %"class.std::__1::basic_string"** %38, align 8
  %153 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8
  %154 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %153, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %154, %"class.std::__1::__compressed_pair"** %37, align 8
  %155 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %37, align 8
  %156 = bitcast %"class.std::__1::__compressed_pair"* %155 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %156, %"class.std::__1::__libcpp_compressed_pair_imp"** %36, align 8
  %157 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %36, align 8
  %158 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %157, i32 0, i32 0
  %159 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %158, i32 0, i32 0
  %160 = bitcast %union.anon.0* %159 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %161 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %160, i32 0, i32 1
  %162 = load i64, i64* %161, align 8
  br label %178

; <label>:163:                                    ; preds = %133
  store %"class.std::__1::basic_string"* %137, %"class.std::__1::basic_string"** %41, align 8
  %164 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8
  %165 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %164, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %165, %"class.std::__1::__compressed_pair"** %40, align 8
  %166 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %40, align 8
  %167 = bitcast %"class.std::__1::__compressed_pair"* %166 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %167, %"class.std::__1::__libcpp_compressed_pair_imp"** %39, align 8
  %168 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %39, align 8
  %169 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %168, i32 0, i32 0
  %170 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %169, i32 0, i32 0
  %171 = bitcast %union.anon.0* %170 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %172 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %171, i32 0, i32 0
  %173 = bitcast %union.anon.1* %172 to i8*
  %174 = load i8, i8* %173, align 8
  %175 = zext i8 %174 to i32
  %176 = ashr i32 %175, 1
  %177 = sext i32 %176 to i64
  br label %178

; <label>:178:                                    ; preds = %163, %152
  %179 = phi i64 [ %162, %152 ], [ %177, %163 ]
  %180 = icmp ne i64 %135, %179
  br i1 %180, label %181, label %182

; <label>:181:                                    ; preds = %178
  store i1 false, i1* %56, align 1
  br label %317

; <label>:182:                                    ; preds = %178
  %183 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %57, align 8
  store %"class.std::__1::basic_string"* %183, %"class.std::__1::basic_string"** %35, align 8
  %184 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %35, align 8
  store %"class.std::__1::basic_string"* %184, %"class.std::__1::basic_string"** %34, align 8
  %185 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %34, align 8
  store %"class.std::__1::basic_string"* %185, %"class.std::__1::basic_string"** %33, align 8
  %186 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8
  %187 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %186, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %187, %"class.std::__1::__compressed_pair"** %32, align 8
  %188 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8
  %189 = bitcast %"class.std::__1::__compressed_pair"* %188 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %189, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8
  %190 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8
  %191 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %190, i32 0, i32 0
  %192 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %191, i32 0, i32 0
  %193 = bitcast %union.anon.0* %192 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %194 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %193, i32 0, i32 0
  %195 = bitcast %union.anon.1* %194 to i8*
  %196 = load i8, i8* %195, align 8
  %197 = zext i8 %196 to i32
  %198 = and i32 %197, 1
  %199 = icmp ne i32 %198, 0
  br i1 %199, label %200, label %211

; <label>:200:                                    ; preds = %182
  store %"class.std::__1::basic_string"* %185, %"class.std::__1::basic_string"** %25, align 8
  %201 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %25, align 8
  %202 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %201, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %202, %"class.std::__1::__compressed_pair"** %24, align 8
  %203 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %24, align 8
  %204 = bitcast %"class.std::__1::__compressed_pair"* %203 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %204, %"class.std::__1::__libcpp_compressed_pair_imp"** %23, align 8
  %205 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %23, align 8
  %206 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %205, i32 0, i32 0
  %207 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %206, i32 0, i32 0
  %208 = bitcast %union.anon.0* %207 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %209 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %208, i32 0, i32 2
  %210 = load i8*, i8** %209, align 8
  br label %224

; <label>:211:                                    ; preds = %182
  store %"class.std::__1::basic_string"* %185, %"class.std::__1::basic_string"** %30, align 8
  %212 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %30, align 8
  %213 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %212, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %213, %"class.std::__1::__compressed_pair"** %29, align 8
  %214 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %29, align 8
  %215 = bitcast %"class.std::__1::__compressed_pair"* %214 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %215, %"class.std::__1::__libcpp_compressed_pair_imp"** %28, align 8
  %216 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %28, align 8
  %217 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %216, i32 0, i32 0
  %218 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %217, i32 0, i32 0
  %219 = bitcast %union.anon.0* %218 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %220 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %219, i32 0, i32 1
  %221 = getelementptr inbounds [23 x i8], [23 x i8]* %220, i64 0, i64 0
  store i8* %221, i8** %27, align 8
  %222 = load i8*, i8** %27, align 8
  store i8* %222, i8** %26, align 8
  %223 = load i8*, i8** %26, align 8
  br label %224

; <label>:224:                                    ; preds = %211, %200
  %225 = phi i8* [ %210, %200 ], [ %223, %211 ]
  store i8* %225, i8** %22, align 8
  %226 = load i8*, i8** %22, align 8
  store i8* %226, i8** %60, align 8
  %227 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8
  store %"class.std::__1::basic_string"* %227, %"class.std::__1::basic_string"** %18, align 8
  %228 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %18, align 8
  store %"class.std::__1::basic_string"* %228, %"class.std::__1::basic_string"** %17, align 8
  %229 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8
  store %"class.std::__1::basic_string"* %229, %"class.std::__1::basic_string"** %16, align 8
  %230 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %16, align 8
  %231 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %230, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %231, %"class.std::__1::__compressed_pair"** %15, align 8
  %232 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %15, align 8
  %233 = bitcast %"class.std::__1::__compressed_pair"* %232 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %233, %"class.std::__1::__libcpp_compressed_pair_imp"** %14, align 8
  %234 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %14, align 8
  %235 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %234, i32 0, i32 0
  %236 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %235, i32 0, i32 0
  %237 = bitcast %union.anon.0* %236 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %238 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %237, i32 0, i32 0
  %239 = bitcast %union.anon.1* %238 to i8*
  %240 = load i8, i8* %239, align 8
  %241 = zext i8 %240 to i32
  %242 = and i32 %241, 1
  %243 = icmp ne i32 %242, 0
  br i1 %243, label %244, label %255

; <label>:244:                                    ; preds = %224
  store %"class.std::__1::basic_string"* %229, %"class.std::__1::basic_string"** %8, align 8
  %245 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %8, align 8
  %246 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %245, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %246, %"class.std::__1::__compressed_pair"** %7, align 8
  %247 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %7, align 8
  %248 = bitcast %"class.std::__1::__compressed_pair"* %247 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %248, %"class.std::__1::__libcpp_compressed_pair_imp"** %6, align 8
  %249 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %6, align 8
  %250 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %249, i32 0, i32 0
  %251 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %250, i32 0, i32 0
  %252 = bitcast %union.anon.0* %251 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %253 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %252, i32 0, i32 2
  %254 = load i8*, i8** %253, align 8
  br label %268

; <label>:255:                                    ; preds = %224
  store %"class.std::__1::basic_string"* %229, %"class.std::__1::basic_string"** %13, align 8
  %256 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8
  %257 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %256, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %257, %"class.std::__1::__compressed_pair"** %12, align 8
  %258 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %12, align 8
  %259 = bitcast %"class.std::__1::__compressed_pair"* %258 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %259, %"class.std::__1::__libcpp_compressed_pair_imp"** %11, align 8
  %260 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %11, align 8
  %261 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %260, i32 0, i32 0
  %262 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %261, i32 0, i32 0
  %263 = bitcast %union.anon.0* %262 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %264 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %263, i32 0, i32 1
  %265 = getelementptr inbounds [23 x i8], [23 x i8]* %264, i64 0, i64 0
  store i8* %265, i8** %10, align 8
  %266 = load i8*, i8** %10, align 8
  store i8* %266, i8** %9, align 8
  %267 = load i8*, i8** %9, align 8
  br label %268

; <label>:268:                                    ; preds = %255, %244
  %269 = phi i8* [ %254, %244 ], [ %267, %255 ]
  store i8* %269, i8** %5, align 8
  %270 = load i8*, i8** %5, align 8
  store i8* %270, i8** %61, align 8
  %271 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %57, align 8
  store %"class.std::__1::basic_string"* %271, %"class.std::__1::basic_string"** %21, align 8
  %272 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %21, align 8
  %273 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %272, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %273, %"class.std::__1::__compressed_pair"** %20, align 8
  %274 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %20, align 8
  %275 = bitcast %"class.std::__1::__compressed_pair"* %274 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %275, %"class.std::__1::__libcpp_compressed_pair_imp"** %19, align 8
  %276 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %19, align 8
  %277 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %276, i32 0, i32 0
  %278 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %277, i32 0, i32 0
  %279 = bitcast %union.anon.0* %278 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %280 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %279, i32 0, i32 0
  %281 = bitcast %union.anon.1* %280 to i8*
  %282 = load i8, i8* %281, align 8
  %283 = zext i8 %282 to i32
  %284 = and i32 %283, 1
  %285 = icmp ne i32 %284, 0
  br i1 %285, label %286, label %293

; <label>:286:                                    ; preds = %268
  %287 = load i8*, i8** %60, align 8
  %288 = load i8*, i8** %61, align 8
  %289 = load i64, i64* %59, align 8
  %290 = invoke i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8* %287, i8* %288, i64 %289)
          to label %291 unwind label %314

; <label>:291:                                    ; preds = %286
  %292 = icmp eq i32 %290, 0
  store i1 %292, i1* %56, align 1
  br label %317

; <label>:293:                                    ; preds = %268
  br label %294

; <label>:294:                                    ; preds = %306, %293
  %295 = load i64, i64* %59, align 8
  %296 = icmp ne i64 %295, 0
  br i1 %296, label %297, label %313

; <label>:297:                                    ; preds = %294
  %298 = load i8*, i8** %60, align 8
  %299 = load i8, i8* %298, align 1
  %300 = sext i8 %299 to i32
  %301 = load i8*, i8** %61, align 8
  %302 = load i8, i8* %301, align 1
  %303 = sext i8 %302 to i32
  %304 = icmp ne i32 %300, %303
  br i1 %304, label %305, label %306

; <label>:305:                                    ; preds = %297
  store i1 false, i1* %56, align 1
  br label %317

; <label>:306:                                    ; preds = %297
  %307 = load i64, i64* %59, align 8
  %308 = add i64 %307, -1
  store i64 %308, i64* %59, align 8
  %309 = load i8*, i8** %60, align 8
  %310 = getelementptr inbounds i8, i8* %309, i32 1
  store i8* %310, i8** %60, align 8
  %311 = load i8*, i8** %61, align 8
  %312 = getelementptr inbounds i8, i8* %311, i32 1
  store i8* %312, i8** %61, align 8
  br label %294

; <label>:313:                                    ; preds = %294
  store i1 true, i1* %56, align 1
  br label %317

; <label>:314:                                    ; preds = %286
  %315 = landingpad { i8*, i32 }
          catch i8* null
  %316 = extractvalue { i8*, i32 } %315, 0
  call void @__clang_call_terminate(i8* %316) #12
  unreachable

; <label>:317:                                    ; preds = %181, %291, %305, %313
  %318 = load i1, i1* %56, align 1
  br i1 %318, label %319, label %320

; <label>:319:                                    ; preds = %317
  store i1 true, i1* %63, align 1
  br label %325

; <label>:320:                                    ; preds = %317
  br label %321

; <label>:321:                                    ; preds = %320
  %322 = load i32, i32* %65, align 4
  %323 = add nsw i32 %322, 1
  store i32 %323, i32* %65, align 4
  br label %67

; <label>:324:                                    ; preds = %67
  store i1 false, i1* %63, align 1
  br label %325

; <label>:325:                                    ; preds = %324, %319
  %326 = load i1, i1* %63, align 1
  ret i1 %326
}

; Function Attrs: ssp uwtable
define zeroext i1 @_ZN5Lexer15is_type_keywordENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Lexer*, %"class.std::__1::basic_string"*) #0 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %3 = alloca %"class.std::__1::basic_string"*, align 8
  %4 = alloca %"struct.std::__1::pair.73"*, align 8
  %5 = alloca %"struct.std::__1::pair.73"*, align 8
  %6 = alloca i32*, align 8
  %7 = alloca %"struct.std::__1::pair.73"*, align 8
  %8 = alloca %"struct.std::__1::pair.73"*, align 8
  %9 = alloca %"struct.std::__1::pair.73"*, align 8
  %10 = alloca %"struct.std::__1::pair.73"*, align 8
  %11 = alloca %"struct.std::__1::pair.73"*, align 8
  %12 = alloca %"struct.std::__1::pair.73"*, align 8
  %13 = alloca %"union.std::__1::__hash_value_type.82"*, align 8
  %14 = alloca %"union.std::__1::__hash_value_type.82"*, align 8
  %15 = alloca %"struct.std::__1::__hash_node_base.56"*, align 8
  %16 = alloca %"struct.std::__1::__hash_node_base.56"*, align 8
  %17 = alloca %"struct.std::__1::__hash_node_base.56"*, align 8
  %18 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %19 = alloca %"class.std::__1::__hash_map_iterator"*, align 8
  %20 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %21 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %22 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %23 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %24 = alloca %"class.std::__1::__hash_map_iterator"*, align 8
  %25 = alloca %"class.std::__1::__hash_map_iterator"*, align 8
  %26 = alloca %"class.std::__1::__hash_iterator", align 8
  %27 = alloca %"class.std::__1::__hash_map_iterator"*, align 8
  %28 = alloca %"class.std::__1::__hash_iterator", align 8
  %29 = alloca %"class.std::__1::__hash_map_iterator"*, align 8
  %30 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %31 = alloca %"struct.std::__1::__hash_node_base.56"*, align 8
  %32 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %33 = alloca %"struct.std::__1::__hash_node_base.56"*, align 8
  %34 = alloca %"class.std::__1::__hash_iterator", align 8
  %35 = alloca %"class.std::__1::__hash_table.52"*, align 8
  %36 = alloca %"class.std::__1::__hash_map_iterator", align 8
  %37 = alloca %"class.std::__1::unordered_map.51"*, align 8
  %38 = alloca %"class.std::__1::__hash_iterator", align 8
  %39 = alloca %"class.std::__1::__hash_iterator", align 8
  %40 = alloca %"class.std::__1::__hash_map_iterator"*, align 8
  %41 = alloca %"class.std::__1::__hash_iterator", align 8
  %42 = alloca %"class.std::__1::__hash_map_iterator"*, align 8
  %43 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %44 = alloca %"struct.std::__1::__hash_node_base.56"*, align 8
  %45 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %46 = alloca %"struct.std::__1::__hash_node_base.56"*, align 8
  %47 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.63"*, align 8
  %48 = alloca %"class.std::__1::__compressed_pair.62"*, align 8
  %49 = alloca %"class.std::__1::__hash_iterator", align 8
  %50 = alloca %"class.std::__1::__hash_table.52"*, align 8
  %51 = alloca %"class.std::__1::__hash_map_iterator", align 8
  %52 = alloca %"class.std::__1::unordered_map.51"*, align 8
  %53 = alloca %"class.std::__1::__hash_iterator", align 8
  %54 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %55 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %56 = alloca %"class.std::__1::basic_string"*, align 8
  %57 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %58 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %59 = alloca %"class.std::__1::basic_string"*, align 8
  %60 = alloca i64, align 8
  %61 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %62 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %63 = alloca %"class.std::__1::basic_string"*, align 8
  %64 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %65 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %66 = alloca %"class.std::__1::basic_string"*, align 8
  %67 = alloca i64, align 8
  %68 = alloca i8*, align 8
  %69 = alloca i8*, align 8
  %70 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %71 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %72 = alloca %"class.std::__1::basic_string"*, align 8
  %73 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %74 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %75 = alloca %"class.std::__1::basic_string"*, align 8
  %76 = alloca %"class.std::__1::basic_string"*, align 8
  %77 = alloca i64, align 8
  %78 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %79 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %80 = alloca %"class.std::__1::basic_string"*, align 8
  %81 = alloca %"class.std::__1::basic_string"*, align 8
  %82 = alloca i64, align 8
  %83 = alloca i8, align 1
  %84 = alloca i8, align 1
  %85 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %86 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %87 = alloca %"class.std::__1::basic_string"*, align 8
  %88 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %89 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %90 = alloca %"class.std::__1::basic_string"*, align 8
  %91 = alloca i64, align 8
  %92 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %93 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %94 = alloca %"class.std::__1::basic_string"*, align 8
  %95 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %96 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %97 = alloca %"class.std::__1::basic_string"*, align 8
  %98 = alloca i64, align 8
  %99 = alloca i8*, align 8
  %100 = alloca i8*, align 8
  %101 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %102 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %103 = alloca %"class.std::__1::basic_string"*, align 8
  %104 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %105 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %106 = alloca %"class.std::__1::basic_string"*, align 8
  %107 = alloca %"class.std::__1::basic_string"*, align 8
  %108 = alloca i64, align 8
  %109 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %110 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %111 = alloca %"class.std::__1::basic_string"*, align 8
  %112 = alloca %"class.std::__1::basic_string"*, align 8
  %113 = alloca i64, align 8
  %114 = alloca i8, align 1
  %115 = alloca i8, align 1
  %116 = alloca i8*, align 8
  %117 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %118 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %119 = alloca %"class.std::__1::basic_string"*, align 8
  %120 = alloca i8*, align 8
  %121 = alloca i8*, align 8
  %122 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %123 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %124 = alloca %"class.std::__1::basic_string"*, align 8
  %125 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %126 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %127 = alloca %"class.std::__1::basic_string"*, align 8
  %128 = alloca %"class.std::__1::basic_string"*, align 8
  %129 = alloca %"class.std::__1::basic_string"*, align 8
  %130 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %131 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %132 = alloca %"class.std::__1::basic_string"*, align 8
  %133 = alloca i8*, align 8
  %134 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %135 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %136 = alloca %"class.std::__1::basic_string"*, align 8
  %137 = alloca i8*, align 8
  %138 = alloca i8*, align 8
  %139 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %140 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %141 = alloca %"class.std::__1::basic_string"*, align 8
  %142 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %143 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %144 = alloca %"class.std::__1::basic_string"*, align 8
  %145 = alloca %"class.std::__1::basic_string"*, align 8
  %146 = alloca %"class.std::__1::basic_string"*, align 8
  %147 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %148 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %149 = alloca %"class.std::__1::basic_string"*, align 8
  %150 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %151 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %152 = alloca %"class.std::__1::basic_string"*, align 8
  %153 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %154 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %155 = alloca %"class.std::__1::basic_string"*, align 8
  %156 = alloca %"class.std::__1::basic_string"*, align 8
  %157 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %158 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %159 = alloca %"class.std::__1::basic_string"*, align 8
  %160 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %161 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %162 = alloca %"class.std::__1::basic_string"*, align 8
  %163 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %164 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %165 = alloca %"class.std::__1::basic_string"*, align 8
  %166 = alloca %"class.std::__1::basic_string"*, align 8
  %167 = alloca i1, align 1
  %168 = alloca %"class.std::__1::basic_string"*, align 8
  %169 = alloca %"class.std::__1::basic_string"*, align 8
  %170 = alloca i64, align 8
  %171 = alloca i8*, align 8
  %172 = alloca i8*, align 8
  %173 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %174 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %175 = alloca %"class.std::__1::basic_string"*, align 8
  %176 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %177 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %178 = alloca %"class.std::__1::basic_string"*, align 8
  %179 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %180 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %181 = alloca %"class.std::__1::basic_string"*, align 8
  %182 = alloca %"class.std::__1::basic_string"*, align 8
  %183 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %184 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %185 = alloca %"class.std::__1::basic_string"*, align 8
  %186 = alloca i8*, align 8
  %187 = alloca i8*, align 8
  %188 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %189 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %190 = alloca %"class.std::__1::basic_string"*, align 8
  %191 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %192 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %193 = alloca %"class.std::__1::basic_string"*, align 8
  %194 = alloca %"class.std::__1::basic_string"*, align 8
  %195 = alloca %"class.std::__1::basic_string"*, align 8
  %196 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %197 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %198 = alloca %"class.std::__1::basic_string"*, align 8
  %199 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %200 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %201 = alloca %"class.std::__1::basic_string"*, align 8
  %202 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %203 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %204 = alloca %"class.std::__1::basic_string"*, align 8
  %205 = alloca %"class.std::__1::basic_string"*, align 8
  %206 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %207 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %208 = alloca %"class.std::__1::basic_string"*, align 8
  %209 = alloca i8*, align 8
  %210 = alloca i8*, align 8
  %211 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %212 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %213 = alloca %"class.std::__1::basic_string"*, align 8
  %214 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %215 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %216 = alloca %"class.std::__1::basic_string"*, align 8
  %217 = alloca %"class.std::__1::basic_string"*, align 8
  %218 = alloca %"class.std::__1::basic_string"*, align 8
  %219 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %220 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %221 = alloca %"class.std::__1::basic_string"*, align 8
  %222 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %223 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %224 = alloca %"class.std::__1::basic_string"*, align 8
  %225 = alloca i64, align 8
  %226 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %227 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %228 = alloca %"class.std::__1::basic_string"*, align 8
  %229 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %230 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %231 = alloca %"class.std::__1::basic_string"*, align 8
  %232 = alloca i64, align 8
  %233 = alloca i8*, align 8
  %234 = alloca i8*, align 8
  %235 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %236 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %237 = alloca %"class.std::__1::basic_string"*, align 8
  %238 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %239 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %240 = alloca %"class.std::__1::basic_string"*, align 8
  %241 = alloca %"class.std::__1::basic_string"*, align 8
  %242 = alloca i64, align 8
  %243 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %244 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %245 = alloca %"class.std::__1::basic_string"*, align 8
  %246 = alloca %"class.std::__1::basic_string"*, align 8
  %247 = alloca i64, align 8
  %248 = alloca i8, align 1
  %249 = alloca i8, align 1
  %250 = alloca %"class.std::__1::__hash_iterator"*, align 8
  %251 = alloca %"class.std::__1::__hash_map_iterator"*, align 8
  %252 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %253 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %254 = alloca %"class.std::__1::basic_string"*, align 8
  %255 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %256 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %257 = alloca %"class.std::__1::basic_string"*, align 8
  %258 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %259 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %260 = alloca %"class.std::__1::basic_string"*, align 8
  %261 = alloca %"class.std::__1::basic_string"*, align 8
  %262 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %263 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %264 = alloca %"class.std::__1::basic_string"*, align 8
  %265 = alloca i8*, align 8
  %266 = alloca i8*, align 8
  %267 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %268 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %269 = alloca %"class.std::__1::basic_string"*, align 8
  %270 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %271 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %272 = alloca %"class.std::__1::basic_string"*, align 8
  %273 = alloca %"class.std::__1::basic_string"*, align 8
  %274 = alloca %"class.std::__1::basic_string"*, align 8
  %275 = alloca i1, align 1
  %276 = alloca %class.Lexer*, align 8
  %277 = alloca %"class.std::__1::unordered_map.51"*, align 8
  %278 = alloca %"class.std::__1::__hash_map_iterator", align 8
  %279 = alloca %"class.std::__1::__hash_map_iterator", align 8
  %280 = alloca %"struct.std::__1::pair.73", align 8
  %281 = alloca i32
  store %class.Lexer* %0, %class.Lexer** %276, align 8
  %282 = load %class.Lexer*, %class.Lexer** %276, align 8
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %274, align 8
  %283 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %274, align 8
  store %"class.std::__1::basic_string"* %283, %"class.std::__1::basic_string"** %273, align 8
  %284 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %273, align 8
  store %"class.std::__1::basic_string"* %284, %"class.std::__1::basic_string"** %272, align 8
  %285 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %272, align 8
  %286 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %285, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %286, %"class.std::__1::__compressed_pair"** %271, align 8
  %287 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %271, align 8
  %288 = bitcast %"class.std::__1::__compressed_pair"* %287 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %288, %"class.std::__1::__libcpp_compressed_pair_imp"** %270, align 8
  %289 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %270, align 8
  %290 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %289, i32 0, i32 0
  %291 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %290, i32 0, i32 0
  %292 = bitcast %union.anon.0* %291 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %293 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %292, i32 0, i32 0
  %294 = bitcast %union.anon.1* %293 to i8*
  %295 = load i8, i8* %294, align 8
  %296 = zext i8 %295 to i32
  %297 = and i32 %296, 1
  %298 = icmp ne i32 %297, 0
  br i1 %298, label %299, label %310

; <label>:299:                                    ; preds = %2
  store %"class.std::__1::basic_string"* %284, %"class.std::__1::basic_string"** %264, align 8
  %300 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %264, align 8
  %301 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %300, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %301, %"class.std::__1::__compressed_pair"** %263, align 8
  %302 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %263, align 8
  %303 = bitcast %"class.std::__1::__compressed_pair"* %302 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %303, %"class.std::__1::__libcpp_compressed_pair_imp"** %262, align 8
  %304 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %262, align 8
  %305 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %304, i32 0, i32 0
  %306 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %305, i32 0, i32 0
  %307 = bitcast %union.anon.0* %306 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %308 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %307, i32 0, i32 2
  %309 = load i8*, i8** %308, align 8
  br label %323

; <label>:310:                                    ; preds = %2
  store %"class.std::__1::basic_string"* %284, %"class.std::__1::basic_string"** %269, align 8
  %311 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %269, align 8
  %312 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %311, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %312, %"class.std::__1::__compressed_pair"** %268, align 8
  %313 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %268, align 8
  %314 = bitcast %"class.std::__1::__compressed_pair"* %313 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %314, %"class.std::__1::__libcpp_compressed_pair_imp"** %267, align 8
  %315 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %267, align 8
  %316 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %315, i32 0, i32 0
  %317 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %316, i32 0, i32 0
  %318 = bitcast %union.anon.0* %317 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %319 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %318, i32 0, i32 1
  %320 = getelementptr inbounds [23 x i8], [23 x i8]* %319, i64 0, i64 0
  store i8* %320, i8** %266, align 8
  %321 = load i8*, i8** %266, align 8
  store i8* %321, i8** %265, align 8
  %322 = load i8*, i8** %265, align 8
  br label %323

; <label>:323:                                    ; preds = %310, %299
  %324 = phi i8* [ %309, %299 ], [ %322, %310 ]
  store %"class.std::__1::basic_string"* %283, %"class.std::__1::basic_string"** %261, align 8
  %325 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %261, align 8
  store %"class.std::__1::basic_string"* %325, %"class.std::__1::basic_string"** %260, align 8
  %326 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %260, align 8
  %327 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %326, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %327, %"class.std::__1::__compressed_pair"** %259, align 8
  %328 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %259, align 8
  %329 = bitcast %"class.std::__1::__compressed_pair"* %328 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %329, %"class.std::__1::__libcpp_compressed_pair_imp"** %258, align 8
  %330 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %258, align 8
  %331 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %330, i32 0, i32 0
  %332 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %331, i32 0, i32 0
  %333 = bitcast %union.anon.0* %332 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %334 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %333, i32 0, i32 0
  %335 = bitcast %union.anon.1* %334 to i8*
  %336 = load i8, i8* %335, align 8
  %337 = zext i8 %336 to i32
  %338 = and i32 %337, 1
  %339 = icmp ne i32 %338, 0
  br i1 %339, label %340, label %351

; <label>:340:                                    ; preds = %323
  store %"class.std::__1::basic_string"* %325, %"class.std::__1::basic_string"** %254, align 8
  %341 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %254, align 8
  %342 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %341, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %342, %"class.std::__1::__compressed_pair"** %253, align 8
  %343 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %253, align 8
  %344 = bitcast %"class.std::__1::__compressed_pair"* %343 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %344, %"class.std::__1::__libcpp_compressed_pair_imp"** %252, align 8
  %345 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %252, align 8
  %346 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %345, i32 0, i32 0
  %347 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %346, i32 0, i32 0
  %348 = bitcast %union.anon.0* %347 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %349 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %348, i32 0, i32 1
  %350 = load i64, i64* %349, align 8
  br label %366

; <label>:351:                                    ; preds = %323
  store %"class.std::__1::basic_string"* %325, %"class.std::__1::basic_string"** %257, align 8
  %352 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %257, align 8
  %353 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %352, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %353, %"class.std::__1::__compressed_pair"** %256, align 8
  %354 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %256, align 8
  %355 = bitcast %"class.std::__1::__compressed_pair"* %354 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %355, %"class.std::__1::__libcpp_compressed_pair_imp"** %255, align 8
  %356 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %255, align 8
  %357 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %356, i32 0, i32 0
  %358 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %357, i32 0, i32 0
  %359 = bitcast %union.anon.0* %358 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %360 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %359, i32 0, i32 0
  %361 = bitcast %union.anon.1* %360 to i8*
  %362 = load i8, i8* %361, align 8
  %363 = zext i8 %362 to i32
  %364 = ashr i32 %363, 1
  %365 = sext i32 %364 to i64
  br label %366

; <label>:366:                                    ; preds = %340, %351
  %367 = phi i64 [ %350, %340 ], [ %365, %351 ]
  %368 = getelementptr inbounds i8, i8* %324, i64 %367
  %369 = getelementptr inbounds i8, i8* %368, i64 -1
  %370 = load i8, i8* %369, align 1
  %371 = sext i8 %370 to i32
  %372 = icmp eq i32 %371, 93
  br i1 %372, label %373, label %851

; <label>:373:                                    ; preds = %366
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %246, align 8
  %374 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %246, align 8
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %245, align 8
  %375 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %245, align 8
  %376 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %375, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %376, %"class.std::__1::__compressed_pair"** %244, align 8
  %377 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %244, align 8
  %378 = bitcast %"class.std::__1::__compressed_pair"* %377 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %378, %"class.std::__1::__libcpp_compressed_pair_imp"** %243, align 8
  %379 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %243, align 8
  %380 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %379, i32 0, i32 0
  %381 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %380, i32 0, i32 0
  %382 = bitcast %union.anon.0* %381 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %383 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %382, i32 0, i32 0
  %384 = bitcast %union.anon.1* %383 to i8*
  %385 = load i8, i8* %384, align 8
  %386 = zext i8 %385 to i32
  %387 = and i32 %386, 1
  %388 = icmp ne i32 %387, 0
  br i1 %388, label %389, label %424

; <label>:389:                                    ; preds = %373
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %240, align 8
  %390 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %240, align 8
  %391 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %390, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %391, %"class.std::__1::__compressed_pair"** %239, align 8
  %392 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %239, align 8
  %393 = bitcast %"class.std::__1::__compressed_pair"* %392 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %393, %"class.std::__1::__libcpp_compressed_pair_imp"** %238, align 8
  %394 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %238, align 8
  %395 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %394, i32 0, i32 0
  %396 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %395, i32 0, i32 0
  %397 = bitcast %union.anon.0* %396 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %398 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %397, i32 0, i32 1
  %399 = load i64, i64* %398, align 8
  %400 = sub i64 %399, 1
  store i64 %400, i64* %247, align 8
  %401 = load i64, i64* %247, align 8
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %224, align 8
  store i64 %401, i64* %225, align 8
  %402 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %224, align 8
  %403 = load i64, i64* %225, align 8
  %404 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %402, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %404, %"class.std::__1::__compressed_pair"** %223, align 8
  %405 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %223, align 8
  %406 = bitcast %"class.std::__1::__compressed_pair"* %405 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %406, %"class.std::__1::__libcpp_compressed_pair_imp"** %222, align 8
  %407 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %222, align 8
  %408 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %407, i32 0, i32 0
  %409 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %408, i32 0, i32 0
  %410 = bitcast %union.anon.0* %409 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %411 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %410, i32 0, i32 1
  store i64 %403, i64* %411, align 8
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %221, align 8
  %412 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %221, align 8
  %413 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %412, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %413, %"class.std::__1::__compressed_pair"** %220, align 8
  %414 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %220, align 8
  %415 = bitcast %"class.std::__1::__compressed_pair"* %414 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %415, %"class.std::__1::__libcpp_compressed_pair_imp"** %219, align 8
  %416 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %219, align 8
  %417 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %416, i32 0, i32 0
  %418 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %417, i32 0, i32 0
  %419 = bitcast %union.anon.0* %418 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %420 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %419, i32 0, i32 2
  %421 = load i8*, i8** %420, align 8
  %422 = load i64, i64* %247, align 8
  %423 = getelementptr inbounds i8, i8* %421, i64 %422
  store i8 0, i8* %248, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %423, i8* dereferenceable(1) %248) #11
  br label %468

; <label>:424:                                    ; preds = %373
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %228, align 8
  %425 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %228, align 8
  %426 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %425, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %426, %"class.std::__1::__compressed_pair"** %227, align 8
  %427 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %227, align 8
  %428 = bitcast %"class.std::__1::__compressed_pair"* %427 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %428, %"class.std::__1::__libcpp_compressed_pair_imp"** %226, align 8
  %429 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %226, align 8
  %430 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %429, i32 0, i32 0
  %431 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %430, i32 0, i32 0
  %432 = bitcast %union.anon.0* %431 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %433 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %432, i32 0, i32 0
  %434 = bitcast %union.anon.1* %433 to i8*
  %435 = load i8, i8* %434, align 8
  %436 = zext i8 %435 to i32
  %437 = ashr i32 %436, 1
  %438 = sext i32 %437 to i64
  %439 = sub i64 %438, 1
  store i64 %439, i64* %247, align 8
  %440 = load i64, i64* %247, align 8
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %231, align 8
  store i64 %440, i64* %232, align 8
  %441 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %231, align 8
  %442 = load i64, i64* %232, align 8
  %443 = shl i64 %442, 1
  %444 = trunc i64 %443 to i8
  %445 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %441, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %445, %"class.std::__1::__compressed_pair"** %230, align 8
  %446 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %230, align 8
  %447 = bitcast %"class.std::__1::__compressed_pair"* %446 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %447, %"class.std::__1::__libcpp_compressed_pair_imp"** %229, align 8
  %448 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %229, align 8
  %449 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %448, i32 0, i32 0
  %450 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %449, i32 0, i32 0
  %451 = bitcast %union.anon.0* %450 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %452 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %451, i32 0, i32 0
  %453 = bitcast %union.anon.1* %452 to i8*
  store i8 %444, i8* %453, align 8
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %237, align 8
  %454 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %237, align 8
  %455 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %454, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %455, %"class.std::__1::__compressed_pair"** %236, align 8
  %456 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %236, align 8
  %457 = bitcast %"class.std::__1::__compressed_pair"* %456 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %457, %"class.std::__1::__libcpp_compressed_pair_imp"** %235, align 8
  %458 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %235, align 8
  %459 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %458, i32 0, i32 0
  %460 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %459, i32 0, i32 0
  %461 = bitcast %union.anon.0* %460 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %462 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %461, i32 0, i32 1
  %463 = getelementptr inbounds [23 x i8], [23 x i8]* %462, i64 0, i64 0
  store i8* %463, i8** %234, align 8
  %464 = load i8*, i8** %234, align 8
  store i8* %464, i8** %233, align 8
  %465 = load i8*, i8** %233, align 8
  %466 = load i64, i64* %247, align 8
  %467 = getelementptr inbounds i8, i8* %465, i64 %466
  store i8 0, i8* %249, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %467, i8* dereferenceable(1) %249) #11
  br label %468

; <label>:468:                                    ; preds = %389, %424
  %469 = load i64, i64* %247, align 8
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %241, align 8
  store i64 %469, i64* %242, align 8
  %470 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %241, align 8
  br label %471

; <label>:471:                                    ; preds = %750, %468
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %218, align 8
  %472 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %218, align 8
  store %"class.std::__1::basic_string"* %472, %"class.std::__1::basic_string"** %217, align 8
  %473 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %217, align 8
  store %"class.std::__1::basic_string"* %473, %"class.std::__1::basic_string"** %216, align 8
  %474 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %216, align 8
  %475 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %474, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %475, %"class.std::__1::__compressed_pair"** %215, align 8
  %476 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %215, align 8
  %477 = bitcast %"class.std::__1::__compressed_pair"* %476 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %477, %"class.std::__1::__libcpp_compressed_pair_imp"** %214, align 8
  %478 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %214, align 8
  %479 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %478, i32 0, i32 0
  %480 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %479, i32 0, i32 0
  %481 = bitcast %union.anon.0* %480 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %482 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %481, i32 0, i32 0
  %483 = bitcast %union.anon.1* %482 to i8*
  %484 = load i8, i8* %483, align 8
  %485 = zext i8 %484 to i32
  %486 = and i32 %485, 1
  %487 = icmp ne i32 %486, 0
  br i1 %487, label %488, label %499

; <label>:488:                                    ; preds = %471
  store %"class.std::__1::basic_string"* %473, %"class.std::__1::basic_string"** %208, align 8
  %489 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %208, align 8
  %490 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %489, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %490, %"class.std::__1::__compressed_pair"** %207, align 8
  %491 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %207, align 8
  %492 = bitcast %"class.std::__1::__compressed_pair"* %491 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %492, %"class.std::__1::__libcpp_compressed_pair_imp"** %206, align 8
  %493 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %206, align 8
  %494 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %493, i32 0, i32 0
  %495 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %494, i32 0, i32 0
  %496 = bitcast %union.anon.0* %495 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %497 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %496, i32 0, i32 2
  %498 = load i8*, i8** %497, align 8
  br label %512

; <label>:499:                                    ; preds = %471
  store %"class.std::__1::basic_string"* %473, %"class.std::__1::basic_string"** %213, align 8
  %500 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %213, align 8
  %501 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %500, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %501, %"class.std::__1::__compressed_pair"** %212, align 8
  %502 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %212, align 8
  %503 = bitcast %"class.std::__1::__compressed_pair"* %502 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %503, %"class.std::__1::__libcpp_compressed_pair_imp"** %211, align 8
  %504 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %211, align 8
  %505 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %504, i32 0, i32 0
  %506 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %505, i32 0, i32 0
  %507 = bitcast %union.anon.0* %506 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %508 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %507, i32 0, i32 1
  %509 = getelementptr inbounds [23 x i8], [23 x i8]* %508, i64 0, i64 0
  store i8* %509, i8** %210, align 8
  %510 = load i8*, i8** %210, align 8
  store i8* %510, i8** %209, align 8
  %511 = load i8*, i8** %209, align 8
  br label %512

; <label>:512:                                    ; preds = %499, %488
  %513 = phi i8* [ %498, %488 ], [ %511, %499 ]
  store %"class.std::__1::basic_string"* %472, %"class.std::__1::basic_string"** %205, align 8
  %514 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %205, align 8
  store %"class.std::__1::basic_string"* %514, %"class.std::__1::basic_string"** %204, align 8
  %515 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %204, align 8
  %516 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %515, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %516, %"class.std::__1::__compressed_pair"** %203, align 8
  %517 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %203, align 8
  %518 = bitcast %"class.std::__1::__compressed_pair"* %517 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %518, %"class.std::__1::__libcpp_compressed_pair_imp"** %202, align 8
  %519 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %202, align 8
  %520 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %519, i32 0, i32 0
  %521 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %520, i32 0, i32 0
  %522 = bitcast %union.anon.0* %521 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %523 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %522, i32 0, i32 0
  %524 = bitcast %union.anon.1* %523 to i8*
  %525 = load i8, i8* %524, align 8
  %526 = zext i8 %525 to i32
  %527 = and i32 %526, 1
  %528 = icmp ne i32 %527, 0
  br i1 %528, label %529, label %540

; <label>:529:                                    ; preds = %512
  store %"class.std::__1::basic_string"* %514, %"class.std::__1::basic_string"** %198, align 8
  %530 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %198, align 8
  %531 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %530, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %531, %"class.std::__1::__compressed_pair"** %197, align 8
  %532 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %197, align 8
  %533 = bitcast %"class.std::__1::__compressed_pair"* %532 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %533, %"class.std::__1::__libcpp_compressed_pair_imp"** %196, align 8
  %534 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %196, align 8
  %535 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %534, i32 0, i32 0
  %536 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %535, i32 0, i32 0
  %537 = bitcast %union.anon.0* %536 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %538 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %537, i32 0, i32 1
  %539 = load i64, i64* %538, align 8
  br label %555

; <label>:540:                                    ; preds = %512
  store %"class.std::__1::basic_string"* %514, %"class.std::__1::basic_string"** %201, align 8
  %541 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %201, align 8
  %542 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %541, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %542, %"class.std::__1::__compressed_pair"** %200, align 8
  %543 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %200, align 8
  %544 = bitcast %"class.std::__1::__compressed_pair"* %543 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %544, %"class.std::__1::__libcpp_compressed_pair_imp"** %199, align 8
  %545 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %199, align 8
  %546 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %545, i32 0, i32 0
  %547 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %546, i32 0, i32 0
  %548 = bitcast %union.anon.0* %547 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %549 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %548, i32 0, i32 0
  %550 = bitcast %union.anon.1* %549 to i8*
  %551 = load i8, i8* %550, align 8
  %552 = zext i8 %551 to i32
  %553 = ashr i32 %552, 1
  %554 = sext i32 %553 to i64
  br label %555

; <label>:555:                                    ; preds = %529, %540
  %556 = phi i64 [ %539, %529 ], [ %554, %540 ]
  %557 = getelementptr inbounds i8, i8* %513, i64 %556
  %558 = getelementptr inbounds i8, i8* %557, i64 -1
  %559 = load i8, i8* %558, align 1
  %560 = sext i8 %559 to i32
  %561 = icmp sgt i32 %560, 48
  br i1 %561, label %562, label %653

; <label>:562:                                    ; preds = %555
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %195, align 8
  %563 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %195, align 8
  store %"class.std::__1::basic_string"* %563, %"class.std::__1::basic_string"** %194, align 8
  %564 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %194, align 8
  store %"class.std::__1::basic_string"* %564, %"class.std::__1::basic_string"** %193, align 8
  %565 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %193, align 8
  %566 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %565, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %566, %"class.std::__1::__compressed_pair"** %192, align 8
  %567 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %192, align 8
  %568 = bitcast %"class.std::__1::__compressed_pair"* %567 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %568, %"class.std::__1::__libcpp_compressed_pair_imp"** %191, align 8
  %569 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %191, align 8
  %570 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %569, i32 0, i32 0
  %571 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %570, i32 0, i32 0
  %572 = bitcast %union.anon.0* %571 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %573 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %572, i32 0, i32 0
  %574 = bitcast %union.anon.1* %573 to i8*
  %575 = load i8, i8* %574, align 8
  %576 = zext i8 %575 to i32
  %577 = and i32 %576, 1
  %578 = icmp ne i32 %577, 0
  br i1 %578, label %579, label %590

; <label>:579:                                    ; preds = %562
  store %"class.std::__1::basic_string"* %564, %"class.std::__1::basic_string"** %185, align 8
  %580 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %185, align 8
  %581 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %580, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %581, %"class.std::__1::__compressed_pair"** %184, align 8
  %582 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %184, align 8
  %583 = bitcast %"class.std::__1::__compressed_pair"* %582 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %583, %"class.std::__1::__libcpp_compressed_pair_imp"** %183, align 8
  %584 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %183, align 8
  %585 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %584, i32 0, i32 0
  %586 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %585, i32 0, i32 0
  %587 = bitcast %union.anon.0* %586 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %588 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %587, i32 0, i32 2
  %589 = load i8*, i8** %588, align 8
  br label %603

; <label>:590:                                    ; preds = %562
  store %"class.std::__1::basic_string"* %564, %"class.std::__1::basic_string"** %190, align 8
  %591 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %190, align 8
  %592 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %591, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %592, %"class.std::__1::__compressed_pair"** %189, align 8
  %593 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %189, align 8
  %594 = bitcast %"class.std::__1::__compressed_pair"* %593 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %594, %"class.std::__1::__libcpp_compressed_pair_imp"** %188, align 8
  %595 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %188, align 8
  %596 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %595, i32 0, i32 0
  %597 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %596, i32 0, i32 0
  %598 = bitcast %union.anon.0* %597 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %599 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %598, i32 0, i32 1
  %600 = getelementptr inbounds [23 x i8], [23 x i8]* %599, i64 0, i64 0
  store i8* %600, i8** %187, align 8
  %601 = load i8*, i8** %187, align 8
  store i8* %601, i8** %186, align 8
  %602 = load i8*, i8** %186, align 8
  br label %603

; <label>:603:                                    ; preds = %590, %579
  %604 = phi i8* [ %589, %579 ], [ %602, %590 ]
  store %"class.std::__1::basic_string"* %563, %"class.std::__1::basic_string"** %182, align 8
  %605 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %182, align 8
  store %"class.std::__1::basic_string"* %605, %"class.std::__1::basic_string"** %181, align 8
  %606 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %181, align 8
  %607 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %606, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %607, %"class.std::__1::__compressed_pair"** %180, align 8
  %608 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %180, align 8
  %609 = bitcast %"class.std::__1::__compressed_pair"* %608 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %609, %"class.std::__1::__libcpp_compressed_pair_imp"** %179, align 8
  %610 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %179, align 8
  %611 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %610, i32 0, i32 0
  %612 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %611, i32 0, i32 0
  %613 = bitcast %union.anon.0* %612 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %614 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %613, i32 0, i32 0
  %615 = bitcast %union.anon.1* %614 to i8*
  %616 = load i8, i8* %615, align 8
  %617 = zext i8 %616 to i32
  %618 = and i32 %617, 1
  %619 = icmp ne i32 %618, 0
  br i1 %619, label %620, label %631

; <label>:620:                                    ; preds = %603
  store %"class.std::__1::basic_string"* %605, %"class.std::__1::basic_string"** %175, align 8
  %621 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %175, align 8
  %622 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %621, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %622, %"class.std::__1::__compressed_pair"** %174, align 8
  %623 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %174, align 8
  %624 = bitcast %"class.std::__1::__compressed_pair"* %623 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %624, %"class.std::__1::__libcpp_compressed_pair_imp"** %173, align 8
  %625 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %173, align 8
  %626 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %625, i32 0, i32 0
  %627 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %626, i32 0, i32 0
  %628 = bitcast %union.anon.0* %627 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %629 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %628, i32 0, i32 1
  %630 = load i64, i64* %629, align 8
  br label %646

; <label>:631:                                    ; preds = %603
  store %"class.std::__1::basic_string"* %605, %"class.std::__1::basic_string"** %178, align 8
  %632 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %178, align 8
  %633 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %632, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %633, %"class.std::__1::__compressed_pair"** %177, align 8
  %634 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %177, align 8
  %635 = bitcast %"class.std::__1::__compressed_pair"* %634 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %635, %"class.std::__1::__libcpp_compressed_pair_imp"** %176, align 8
  %636 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %176, align 8
  %637 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %636, i32 0, i32 0
  %638 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %637, i32 0, i32 0
  %639 = bitcast %union.anon.0* %638 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %640 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %639, i32 0, i32 0
  %641 = bitcast %union.anon.1* %640 to i8*
  %642 = load i8, i8* %641, align 8
  %643 = zext i8 %642 to i32
  %644 = ashr i32 %643, 1
  %645 = sext i32 %644 to i64
  br label %646

; <label>:646:                                    ; preds = %620, %631
  %647 = phi i64 [ %630, %620 ], [ %645, %631 ]
  %648 = getelementptr inbounds i8, i8* %604, i64 %647
  %649 = getelementptr inbounds i8, i8* %648, i64 -1
  %650 = load i8, i8* %649, align 1
  %651 = sext i8 %650 to i32
  %652 = icmp slt i32 %651, 57
  br label %653

; <label>:653:                                    ; preds = %646, %555
  %654 = phi i1 [ false, %555 ], [ %652, %646 ]
  br i1 %654, label %655, label %753

; <label>:655:                                    ; preds = %653
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %112, align 8
  %656 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %112, align 8
  store %"class.std::__1::basic_string"* %656, %"class.std::__1::basic_string"** %111, align 8
  %657 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %111, align 8
  %658 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %657, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %658, %"class.std::__1::__compressed_pair"** %110, align 8
  %659 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %110, align 8
  %660 = bitcast %"class.std::__1::__compressed_pair"* %659 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %660, %"class.std::__1::__libcpp_compressed_pair_imp"** %109, align 8
  %661 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %109, align 8
  %662 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %661, i32 0, i32 0
  %663 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %662, i32 0, i32 0
  %664 = bitcast %union.anon.0* %663 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %665 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %664, i32 0, i32 0
  %666 = bitcast %union.anon.1* %665 to i8*
  %667 = load i8, i8* %666, align 8
  %668 = zext i8 %667 to i32
  %669 = and i32 %668, 1
  %670 = icmp ne i32 %669, 0
  br i1 %670, label %671, label %706

; <label>:671:                                    ; preds = %655
  store %"class.std::__1::basic_string"* %656, %"class.std::__1::basic_string"** %106, align 8
  %672 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %106, align 8
  %673 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %672, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %673, %"class.std::__1::__compressed_pair"** %105, align 8
  %674 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %105, align 8
  %675 = bitcast %"class.std::__1::__compressed_pair"* %674 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %675, %"class.std::__1::__libcpp_compressed_pair_imp"** %104, align 8
  %676 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %104, align 8
  %677 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %676, i32 0, i32 0
  %678 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %677, i32 0, i32 0
  %679 = bitcast %union.anon.0* %678 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %680 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %679, i32 0, i32 1
  %681 = load i64, i64* %680, align 8
  %682 = sub i64 %681, 1
  store i64 %682, i64* %113, align 8
  %683 = load i64, i64* %113, align 8
  store %"class.std::__1::basic_string"* %656, %"class.std::__1::basic_string"** %90, align 8
  store i64 %683, i64* %91, align 8
  %684 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %90, align 8
  %685 = load i64, i64* %91, align 8
  %686 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %684, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %686, %"class.std::__1::__compressed_pair"** %89, align 8
  %687 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %89, align 8
  %688 = bitcast %"class.std::__1::__compressed_pair"* %687 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %688, %"class.std::__1::__libcpp_compressed_pair_imp"** %88, align 8
  %689 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %88, align 8
  %690 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %689, i32 0, i32 0
  %691 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %690, i32 0, i32 0
  %692 = bitcast %union.anon.0* %691 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %693 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %692, i32 0, i32 1
  store i64 %685, i64* %693, align 8
  store %"class.std::__1::basic_string"* %656, %"class.std::__1::basic_string"** %87, align 8
  %694 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %87, align 8
  %695 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %694, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %695, %"class.std::__1::__compressed_pair"** %86, align 8
  %696 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %86, align 8
  %697 = bitcast %"class.std::__1::__compressed_pair"* %696 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %697, %"class.std::__1::__libcpp_compressed_pair_imp"** %85, align 8
  %698 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %85, align 8
  %699 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %698, i32 0, i32 0
  %700 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %699, i32 0, i32 0
  %701 = bitcast %union.anon.0* %700 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %702 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %701, i32 0, i32 2
  %703 = load i8*, i8** %702, align 8
  %704 = load i64, i64* %113, align 8
  %705 = getelementptr inbounds i8, i8* %703, i64 %704
  store i8 0, i8* %114, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %705, i8* dereferenceable(1) %114) #11
  br label %750

; <label>:706:                                    ; preds = %655
  store %"class.std::__1::basic_string"* %656, %"class.std::__1::basic_string"** %94, align 8
  %707 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %94, align 8
  %708 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %707, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %708, %"class.std::__1::__compressed_pair"** %93, align 8
  %709 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %93, align 8
  %710 = bitcast %"class.std::__1::__compressed_pair"* %709 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %710, %"class.std::__1::__libcpp_compressed_pair_imp"** %92, align 8
  %711 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %92, align 8
  %712 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %711, i32 0, i32 0
  %713 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %712, i32 0, i32 0
  %714 = bitcast %union.anon.0* %713 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %715 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %714, i32 0, i32 0
  %716 = bitcast %union.anon.1* %715 to i8*
  %717 = load i8, i8* %716, align 8
  %718 = zext i8 %717 to i32
  %719 = ashr i32 %718, 1
  %720 = sext i32 %719 to i64
  %721 = sub i64 %720, 1
  store i64 %721, i64* %113, align 8
  %722 = load i64, i64* %113, align 8
  store %"class.std::__1::basic_string"* %656, %"class.std::__1::basic_string"** %97, align 8
  store i64 %722, i64* %98, align 8
  %723 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %97, align 8
  %724 = load i64, i64* %98, align 8
  %725 = shl i64 %724, 1
  %726 = trunc i64 %725 to i8
  %727 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %723, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %727, %"class.std::__1::__compressed_pair"** %96, align 8
  %728 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %96, align 8
  %729 = bitcast %"class.std::__1::__compressed_pair"* %728 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %729, %"class.std::__1::__libcpp_compressed_pair_imp"** %95, align 8
  %730 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %95, align 8
  %731 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %730, i32 0, i32 0
  %732 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %731, i32 0, i32 0
  %733 = bitcast %union.anon.0* %732 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %734 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %733, i32 0, i32 0
  %735 = bitcast %union.anon.1* %734 to i8*
  store i8 %726, i8* %735, align 8
  store %"class.std::__1::basic_string"* %656, %"class.std::__1::basic_string"** %103, align 8
  %736 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %103, align 8
  %737 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %736, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %737, %"class.std::__1::__compressed_pair"** %102, align 8
  %738 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %102, align 8
  %739 = bitcast %"class.std::__1::__compressed_pair"* %738 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %739, %"class.std::__1::__libcpp_compressed_pair_imp"** %101, align 8
  %740 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %101, align 8
  %741 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %740, i32 0, i32 0
  %742 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %741, i32 0, i32 0
  %743 = bitcast %union.anon.0* %742 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %744 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %743, i32 0, i32 1
  %745 = getelementptr inbounds [23 x i8], [23 x i8]* %744, i64 0, i64 0
  store i8* %745, i8** %100, align 8
  %746 = load i8*, i8** %100, align 8
  store i8* %746, i8** %99, align 8
  %747 = load i8*, i8** %99, align 8
  %748 = load i64, i64* %113, align 8
  %749 = getelementptr inbounds i8, i8* %747, i64 %748
  store i8 0, i8* %115, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %749, i8* dereferenceable(1) %115) #11
  br label %750

; <label>:750:                                    ; preds = %671, %706
  %751 = load i64, i64* %113, align 8
  store %"class.std::__1::basic_string"* %656, %"class.std::__1::basic_string"** %107, align 8
  store i64 %751, i64* %108, align 8
  %752 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %107, align 8
  br label %471

; <label>:753:                                    ; preds = %653
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %81, align 8
  %754 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %81, align 8
  store %"class.std::__1::basic_string"* %754, %"class.std::__1::basic_string"** %80, align 8
  %755 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %80, align 8
  %756 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %755, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %756, %"class.std::__1::__compressed_pair"** %79, align 8
  %757 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %79, align 8
  %758 = bitcast %"class.std::__1::__compressed_pair"* %757 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %758, %"class.std::__1::__libcpp_compressed_pair_imp"** %78, align 8
  %759 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %78, align 8
  %760 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %759, i32 0, i32 0
  %761 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %760, i32 0, i32 0
  %762 = bitcast %union.anon.0* %761 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %763 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %762, i32 0, i32 0
  %764 = bitcast %union.anon.1* %763 to i8*
  %765 = load i8, i8* %764, align 8
  %766 = zext i8 %765 to i32
  %767 = and i32 %766, 1
  %768 = icmp ne i32 %767, 0
  br i1 %768, label %769, label %804

; <label>:769:                                    ; preds = %753
  store %"class.std::__1::basic_string"* %754, %"class.std::__1::basic_string"** %75, align 8
  %770 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %75, align 8
  %771 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %770, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %771, %"class.std::__1::__compressed_pair"** %74, align 8
  %772 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %74, align 8
  %773 = bitcast %"class.std::__1::__compressed_pair"* %772 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %773, %"class.std::__1::__libcpp_compressed_pair_imp"** %73, align 8
  %774 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %73, align 8
  %775 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %774, i32 0, i32 0
  %776 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %775, i32 0, i32 0
  %777 = bitcast %union.anon.0* %776 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %778 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %777, i32 0, i32 1
  %779 = load i64, i64* %778, align 8
  %780 = sub i64 %779, 1
  store i64 %780, i64* %82, align 8
  %781 = load i64, i64* %82, align 8
  store %"class.std::__1::basic_string"* %754, %"class.std::__1::basic_string"** %59, align 8
  store i64 %781, i64* %60, align 8
  %782 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %59, align 8
  %783 = load i64, i64* %60, align 8
  %784 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %782, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %784, %"class.std::__1::__compressed_pair"** %58, align 8
  %785 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %58, align 8
  %786 = bitcast %"class.std::__1::__compressed_pair"* %785 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %786, %"class.std::__1::__libcpp_compressed_pair_imp"** %57, align 8
  %787 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %57, align 8
  %788 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %787, i32 0, i32 0
  %789 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %788, i32 0, i32 0
  %790 = bitcast %union.anon.0* %789 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %791 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %790, i32 0, i32 1
  store i64 %783, i64* %791, align 8
  store %"class.std::__1::basic_string"* %754, %"class.std::__1::basic_string"** %56, align 8
  %792 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %56, align 8
  %793 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %792, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %793, %"class.std::__1::__compressed_pair"** %55, align 8
  %794 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %55, align 8
  %795 = bitcast %"class.std::__1::__compressed_pair"* %794 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %795, %"class.std::__1::__libcpp_compressed_pair_imp"** %54, align 8
  %796 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %54, align 8
  %797 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %796, i32 0, i32 0
  %798 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %797, i32 0, i32 0
  %799 = bitcast %union.anon.0* %798 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %800 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %799, i32 0, i32 2
  %801 = load i8*, i8** %800, align 8
  %802 = load i64, i64* %82, align 8
  %803 = getelementptr inbounds i8, i8* %801, i64 %802
  store i8 0, i8* %83, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %803, i8* dereferenceable(1) %83) #11
  br label %848

; <label>:804:                                    ; preds = %753
  store %"class.std::__1::basic_string"* %754, %"class.std::__1::basic_string"** %63, align 8
  %805 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8
  %806 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %805, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %806, %"class.std::__1::__compressed_pair"** %62, align 8
  %807 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %62, align 8
  %808 = bitcast %"class.std::__1::__compressed_pair"* %807 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %808, %"class.std::__1::__libcpp_compressed_pair_imp"** %61, align 8
  %809 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %61, align 8
  %810 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %809, i32 0, i32 0
  %811 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %810, i32 0, i32 0
  %812 = bitcast %union.anon.0* %811 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %813 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %812, i32 0, i32 0
  %814 = bitcast %union.anon.1* %813 to i8*
  %815 = load i8, i8* %814, align 8
  %816 = zext i8 %815 to i32
  %817 = ashr i32 %816, 1
  %818 = sext i32 %817 to i64
  %819 = sub i64 %818, 1
  store i64 %819, i64* %82, align 8
  %820 = load i64, i64* %82, align 8
  store %"class.std::__1::basic_string"* %754, %"class.std::__1::basic_string"** %66, align 8
  store i64 %820, i64* %67, align 8
  %821 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8
  %822 = load i64, i64* %67, align 8
  %823 = shl i64 %822, 1
  %824 = trunc i64 %823 to i8
  %825 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %821, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %825, %"class.std::__1::__compressed_pair"** %65, align 8
  %826 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %65, align 8
  %827 = bitcast %"class.std::__1::__compressed_pair"* %826 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %827, %"class.std::__1::__libcpp_compressed_pair_imp"** %64, align 8
  %828 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %64, align 8
  %829 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %828, i32 0, i32 0
  %830 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %829, i32 0, i32 0
  %831 = bitcast %union.anon.0* %830 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %832 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %831, i32 0, i32 0
  %833 = bitcast %union.anon.1* %832 to i8*
  store i8 %824, i8* %833, align 8
  store %"class.std::__1::basic_string"* %754, %"class.std::__1::basic_string"** %72, align 8
  %834 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %72, align 8
  %835 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %834, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %835, %"class.std::__1::__compressed_pair"** %71, align 8
  %836 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %71, align 8
  %837 = bitcast %"class.std::__1::__compressed_pair"* %836 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %837, %"class.std::__1::__libcpp_compressed_pair_imp"** %70, align 8
  %838 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %70, align 8
  %839 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %838, i32 0, i32 0
  %840 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %839, i32 0, i32 0
  %841 = bitcast %union.anon.0* %840 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %842 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %841, i32 0, i32 1
  %843 = getelementptr inbounds [23 x i8], [23 x i8]* %842, i64 0, i64 0
  store i8* %843, i8** %69, align 8
  %844 = load i8*, i8** %69, align 8
  store i8* %844, i8** %68, align 8
  %845 = load i8*, i8** %68, align 8
  %846 = load i64, i64* %82, align 8
  %847 = getelementptr inbounds i8, i8* %845, i64 %846
  store i8 0, i8* %84, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %847, i8* dereferenceable(1) %84) #11
  br label %848

; <label>:848:                                    ; preds = %769, %804
  %849 = load i64, i64* %82, align 8
  store %"class.std::__1::basic_string"* %754, %"class.std::__1::basic_string"** %76, align 8
  store i64 %849, i64* %77, align 8
  %850 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %76, align 8
  br label %851

; <label>:851:                                    ; preds = %848, %366
  store %"class.std::__1::unordered_map.51"* @_ZN4util7typemapE, %"class.std::__1::unordered_map.51"** %277, align 8
  store %"class.std::__1::unordered_map.51"* @_ZN4util7typemapE, %"class.std::__1::unordered_map.51"** %52, align 8
  %852 = load %"class.std::__1::unordered_map.51"*, %"class.std::__1::unordered_map.51"** %52, align 8
  %853 = getelementptr inbounds %"class.std::__1::unordered_map.51", %"class.std::__1::unordered_map.51"* %852, i32 0, i32 0
  store %"class.std::__1::__hash_table.52"* %853, %"class.std::__1::__hash_table.52"** %50, align 8
  %854 = load %"class.std::__1::__hash_table.52"*, %"class.std::__1::__hash_table.52"** %50, align 8
  %855 = getelementptr inbounds %"class.std::__1::__hash_table.52", %"class.std::__1::__hash_table.52"* %854, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.62"* %855, %"class.std::__1::__compressed_pair.62"** %48, align 8
  %856 = load %"class.std::__1::__compressed_pair.62"*, %"class.std::__1::__compressed_pair.62"** %48, align 8
  %857 = bitcast %"class.std::__1::__compressed_pair.62"* %856 to %"class.std::__1::__libcpp_compressed_pair_imp.63"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.63"* %857, %"class.std::__1::__libcpp_compressed_pair_imp.63"** %47, align 8
  %858 = load %"class.std::__1::__libcpp_compressed_pair_imp.63"*, %"class.std::__1::__libcpp_compressed_pair_imp.63"** %47, align 8
  %859 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.63", %"class.std::__1::__libcpp_compressed_pair_imp.63"* %858, i32 0, i32 0
  %860 = getelementptr inbounds %"struct.std::__1::__hash_node_base.56", %"struct.std::__1::__hash_node_base.56"* %859, i32 0, i32 0
  %861 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %860, align 8
  store %"class.std::__1::__hash_iterator"* %49, %"class.std::__1::__hash_iterator"** %45, align 8
  store %"struct.std::__1::__hash_node_base.56"* %861, %"struct.std::__1::__hash_node_base.56"** %46, align 8
  %862 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %45, align 8
  %863 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %46, align 8
  store %"class.std::__1::__hash_iterator"* %862, %"class.std::__1::__hash_iterator"** %43, align 8
  store %"struct.std::__1::__hash_node_base.56"* %863, %"struct.std::__1::__hash_node_base.56"** %44, align 8
  %864 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %43, align 8
  %865 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %864, i32 0, i32 0
  %866 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %44, align 8
  store %"struct.std::__1::__hash_node_base.56"* %866, %"struct.std::__1::__hash_node_base.56"** %865, align 8
  %867 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %49, i32 0, i32 0
  %868 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %867, align 8
  %869 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %53, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.56"* %868, %"struct.std::__1::__hash_node_base.56"** %869, align 8
  %870 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %53, i32 0, i32 0
  %871 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %870, align 8
  %872 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %41, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.56"* %871, %"struct.std::__1::__hash_node_base.56"** %872, align 8
  store %"class.std::__1::__hash_map_iterator"* %51, %"class.std::__1::__hash_map_iterator"** %42, align 8
  %873 = load %"class.std::__1::__hash_map_iterator"*, %"class.std::__1::__hash_map_iterator"** %42, align 8
  %874 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %41, i32 0, i32 0
  %875 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %874, align 8
  %876 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %39, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.56"* %875, %"struct.std::__1::__hash_node_base.56"** %876, align 8
  store %"class.std::__1::__hash_map_iterator"* %873, %"class.std::__1::__hash_map_iterator"** %40, align 8
  %877 = load %"class.std::__1::__hash_map_iterator"*, %"class.std::__1::__hash_map_iterator"** %40, align 8
  %878 = getelementptr inbounds %"class.std::__1::__hash_map_iterator", %"class.std::__1::__hash_map_iterator"* %877, i32 0, i32 0
  %879 = bitcast %"class.std::__1::__hash_iterator"* %878 to i8*
  %880 = bitcast %"class.std::__1::__hash_iterator"* %39 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %879, i8* %880, i64 8, i32 8, i1 false) #11
  %881 = getelementptr inbounds %"class.std::__1::__hash_map_iterator", %"class.std::__1::__hash_map_iterator"* %51, i32 0, i32 0
  %882 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %881, i32 0, i32 0
  %883 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %882, align 8
  %884 = getelementptr inbounds %"class.std::__1::__hash_map_iterator", %"class.std::__1::__hash_map_iterator"* %278, i32 0, i32 0
  %885 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %884, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.56"* %883, %"struct.std::__1::__hash_node_base.56"** %885, align 8
  store %"class.std::__1::unordered_map.51"* @_ZN4util7typemapE, %"class.std::__1::unordered_map.51"** %37, align 8
  %886 = load %"class.std::__1::unordered_map.51"*, %"class.std::__1::unordered_map.51"** %37, align 8
  %887 = getelementptr inbounds %"class.std::__1::unordered_map.51", %"class.std::__1::unordered_map.51"* %886, i32 0, i32 0
  store %"class.std::__1::__hash_table.52"* %887, %"class.std::__1::__hash_table.52"** %35, align 8
  %888 = load %"class.std::__1::__hash_table.52"*, %"class.std::__1::__hash_table.52"** %35, align 8
  store %"class.std::__1::__hash_iterator"* %34, %"class.std::__1::__hash_iterator"** %32, align 8
  store %"struct.std::__1::__hash_node_base.56"* null, %"struct.std::__1::__hash_node_base.56"** %33, align 8
  %889 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %32, align 8
  %890 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %33, align 8
  store %"class.std::__1::__hash_iterator"* %889, %"class.std::__1::__hash_iterator"** %30, align 8
  store %"struct.std::__1::__hash_node_base.56"* %890, %"struct.std::__1::__hash_node_base.56"** %31, align 8
  %891 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %30, align 8
  %892 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %891, i32 0, i32 0
  %893 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %31, align 8
  store %"struct.std::__1::__hash_node_base.56"* %893, %"struct.std::__1::__hash_node_base.56"** %892, align 8
  %894 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %34, i32 0, i32 0
  %895 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %894, align 8
  %896 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %38, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.56"* %895, %"struct.std::__1::__hash_node_base.56"** %896, align 8
  %897 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %38, i32 0, i32 0
  %898 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %897, align 8
  %899 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %28, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.56"* %898, %"struct.std::__1::__hash_node_base.56"** %899, align 8
  store %"class.std::__1::__hash_map_iterator"* %36, %"class.std::__1::__hash_map_iterator"** %29, align 8
  %900 = load %"class.std::__1::__hash_map_iterator"*, %"class.std::__1::__hash_map_iterator"** %29, align 8
  %901 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %28, i32 0, i32 0
  %902 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %901, align 8
  %903 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %26, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.56"* %902, %"struct.std::__1::__hash_node_base.56"** %903, align 8
  store %"class.std::__1::__hash_map_iterator"* %900, %"class.std::__1::__hash_map_iterator"** %27, align 8
  %904 = load %"class.std::__1::__hash_map_iterator"*, %"class.std::__1::__hash_map_iterator"** %27, align 8
  %905 = getelementptr inbounds %"class.std::__1::__hash_map_iterator", %"class.std::__1::__hash_map_iterator"* %904, i32 0, i32 0
  %906 = bitcast %"class.std::__1::__hash_iterator"* %905 to i8*
  %907 = bitcast %"class.std::__1::__hash_iterator"* %26 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %906, i8* %907, i64 8, i32 8, i1 false) #11
  %908 = getelementptr inbounds %"class.std::__1::__hash_map_iterator", %"class.std::__1::__hash_map_iterator"* %36, i32 0, i32 0
  %909 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %908, i32 0, i32 0
  %910 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %909, align 8
  %911 = getelementptr inbounds %"class.std::__1::__hash_map_iterator", %"class.std::__1::__hash_map_iterator"* %279, i32 0, i32 0
  %912 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %911, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.56"* %910, %"struct.std::__1::__hash_node_base.56"** %912, align 8
  br label %913

; <label>:913:                                    ; preds = %1194, %851
  store %"class.std::__1::__hash_map_iterator"* %278, %"class.std::__1::__hash_map_iterator"** %24, align 8
  store %"class.std::__1::__hash_map_iterator"* %279, %"class.std::__1::__hash_map_iterator"** %25, align 8
  %914 = load %"class.std::__1::__hash_map_iterator"*, %"class.std::__1::__hash_map_iterator"** %24, align 8
  %915 = getelementptr inbounds %"class.std::__1::__hash_map_iterator", %"class.std::__1::__hash_map_iterator"* %914, i32 0, i32 0
  %916 = load %"class.std::__1::__hash_map_iterator"*, %"class.std::__1::__hash_map_iterator"** %25, align 8
  %917 = getelementptr inbounds %"class.std::__1::__hash_map_iterator", %"class.std::__1::__hash_map_iterator"* %916, i32 0, i32 0
  store %"class.std::__1::__hash_iterator"* %915, %"class.std::__1::__hash_iterator"** %22, align 8
  store %"class.std::__1::__hash_iterator"* %917, %"class.std::__1::__hash_iterator"** %23, align 8
  %918 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %22, align 8
  %919 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %23, align 8
  store %"class.std::__1::__hash_iterator"* %918, %"class.std::__1::__hash_iterator"** %20, align 8
  store %"class.std::__1::__hash_iterator"* %919, %"class.std::__1::__hash_iterator"** %21, align 8
  %920 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %20, align 8
  %921 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %920, i32 0, i32 0
  %922 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %921, align 8
  %923 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %21, align 8
  %924 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %923, i32 0, i32 0
  %925 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %924, align 8
  %926 = icmp eq %"struct.std::__1::__hash_node_base.56"* %922, %925
  %927 = xor i1 %926, true
  br i1 %927, label %928, label %1203

; <label>:928:                                    ; preds = %913
  store %"class.std::__1::__hash_map_iterator"* %278, %"class.std::__1::__hash_map_iterator"** %19, align 8
  %929 = load %"class.std::__1::__hash_map_iterator"*, %"class.std::__1::__hash_map_iterator"** %19, align 8
  %930 = getelementptr inbounds %"class.std::__1::__hash_map_iterator", %"class.std::__1::__hash_map_iterator"* %929, i32 0, i32 0
  store %"class.std::__1::__hash_iterator"* %930, %"class.std::__1::__hash_iterator"** %18, align 8
  %931 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %18, align 8
  %932 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %931, i32 0, i32 0
  %933 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %932, align 8
  store %"struct.std::__1::__hash_node_base.56"* %933, %"struct.std::__1::__hash_node_base.56"** %17, align 8
  %934 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %17, align 8
  store %"struct.std::__1::__hash_node_base.56"* %934, %"struct.std::__1::__hash_node_base.56"** %16, align 8
  %935 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %16, align 8
  store %"struct.std::__1::__hash_node_base.56"* %935, %"struct.std::__1::__hash_node_base.56"** %15, align 8
  %936 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %15, align 8
  %937 = bitcast %"struct.std::__1::__hash_node_base.56"* %936 to i8*
  %938 = bitcast %"struct.std::__1::__hash_node_base.56"* %936 to %"struct.std::__1::__hash_node.85"*
  %939 = getelementptr inbounds %"struct.std::__1::__hash_node.85", %"struct.std::__1::__hash_node.85"* %938, i32 0, i32 2
  store %"union.std::__1::__hash_value_type.82"* %939, %"union.std::__1::__hash_value_type.82"** %14, align 8
  %940 = load %"union.std::__1::__hash_value_type.82"*, %"union.std::__1::__hash_value_type.82"** %14, align 8
  store %"union.std::__1::__hash_value_type.82"* %940, %"union.std::__1::__hash_value_type.82"** %13, align 8
  %941 = load %"union.std::__1::__hash_value_type.82"*, %"union.std::__1::__hash_value_type.82"** %13, align 8
  %942 = bitcast %"union.std::__1::__hash_value_type.82"* %941 to i8*
  %943 = bitcast %"union.std::__1::__hash_value_type.82"* %941 to %"struct.std::__1::pair.73"*
  store %"struct.std::__1::pair.73"* %280, %"struct.std::__1::pair.73"** %11, align 8
  store %"struct.std::__1::pair.73"* %943, %"struct.std::__1::pair.73"** %12, align 8
  %944 = load %"struct.std::__1::pair.73"*, %"struct.std::__1::pair.73"** %11, align 8
  %945 = load %"struct.std::__1::pair.73"*, %"struct.std::__1::pair.73"** %12, align 8
  store %"struct.std::__1::pair.73"* %944, %"struct.std::__1::pair.73"** %9, align 8
  store %"struct.std::__1::pair.73"* %945, %"struct.std::__1::pair.73"** %10, align 8
  %946 = load %"struct.std::__1::pair.73"*, %"struct.std::__1::pair.73"** %9, align 8
  %947 = getelementptr inbounds %"struct.std::__1::pair.73", %"struct.std::__1::pair.73"* %946, i32 0, i32 0
  %948 = load %"struct.std::__1::pair.73"*, %"struct.std::__1::pair.73"** %10, align 8
  store %"struct.std::__1::pair.73"* %948, %"struct.std::__1::pair.73"** %8, align 8
  %949 = load %"struct.std::__1::pair.73"*, %"struct.std::__1::pair.73"** %8, align 8
  store %"struct.std::__1::pair.73"* %949, %"struct.std::__1::pair.73"** %7, align 8
  %950 = load %"struct.std::__1::pair.73"*, %"struct.std::__1::pair.73"** %7, align 8
  %951 = getelementptr inbounds %"struct.std::__1::pair.73", %"struct.std::__1::pair.73"* %950, i32 0, i32 0
  store %"class.std::__1::basic_string"* %951, %"class.std::__1::basic_string"** %3, align 8
  %952 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %3, align 8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %947, %"class.std::__1::basic_string"* dereferenceable(24) %952)
  %953 = getelementptr inbounds %"struct.std::__1::pair.73", %"struct.std::__1::pair.73"* %946, i32 0, i32 1
  %954 = load %"struct.std::__1::pair.73"*, %"struct.std::__1::pair.73"** %10, align 8
  store %"struct.std::__1::pair.73"* %954, %"struct.std::__1::pair.73"** %5, align 8
  %955 = load %"struct.std::__1::pair.73"*, %"struct.std::__1::pair.73"** %5, align 8
  store %"struct.std::__1::pair.73"* %955, %"struct.std::__1::pair.73"** %4, align 8
  %956 = load %"struct.std::__1::pair.73"*, %"struct.std::__1::pair.73"** %4, align 8
  %957 = getelementptr inbounds %"struct.std::__1::pair.73", %"struct.std::__1::pair.73"* %956, i32 0, i32 1
  store i32* %957, i32** %6, align 8
  %958 = load i32*, i32** %6, align 8
  %959 = load i32, i32* %958, align 4
  store i32 %959, i32* %953, align 8
  %960 = getelementptr inbounds %"struct.std::__1::pair.73", %"struct.std::__1::pair.73"* %280, i32 0, i32 0
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %168, align 8
  store %"class.std::__1::basic_string"* %960, %"class.std::__1::basic_string"** %169, align 8
  %961 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %168, align 8
  store %"class.std::__1::basic_string"* %961, %"class.std::__1::basic_string"** %166, align 8
  %962 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %166, align 8
  store %"class.std::__1::basic_string"* %962, %"class.std::__1::basic_string"** %165, align 8
  %963 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %165, align 8
  %964 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %963, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %964, %"class.std::__1::__compressed_pair"** %164, align 8
  %965 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %164, align 8
  %966 = bitcast %"class.std::__1::__compressed_pair"* %965 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %966, %"class.std::__1::__libcpp_compressed_pair_imp"** %163, align 8
  %967 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %163, align 8
  %968 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %967, i32 0, i32 0
  %969 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %968, i32 0, i32 0
  %970 = bitcast %union.anon.0* %969 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %971 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %970, i32 0, i32 0
  %972 = bitcast %union.anon.1* %971 to i8*
  %973 = load i8, i8* %972, align 8
  %974 = zext i8 %973 to i32
  %975 = and i32 %974, 1
  %976 = icmp ne i32 %975, 0
  br i1 %976, label %977, label %988

; <label>:977:                                    ; preds = %928
  store %"class.std::__1::basic_string"* %962, %"class.std::__1::basic_string"** %159, align 8
  %978 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %159, align 8
  %979 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %978, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %979, %"class.std::__1::__compressed_pair"** %158, align 8
  %980 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %158, align 8
  %981 = bitcast %"class.std::__1::__compressed_pair"* %980 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %981, %"class.std::__1::__libcpp_compressed_pair_imp"** %157, align 8
  %982 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %157, align 8
  %983 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %982, i32 0, i32 0
  %984 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %983, i32 0, i32 0
  %985 = bitcast %union.anon.0* %984 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %986 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %985, i32 0, i32 1
  %987 = load i64, i64* %986, align 8
  br label %1003

; <label>:988:                                    ; preds = %928
  store %"class.std::__1::basic_string"* %962, %"class.std::__1::basic_string"** %162, align 8
  %989 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %162, align 8
  %990 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %989, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %990, %"class.std::__1::__compressed_pair"** %161, align 8
  %991 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %161, align 8
  %992 = bitcast %"class.std::__1::__compressed_pair"* %991 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %992, %"class.std::__1::__libcpp_compressed_pair_imp"** %160, align 8
  %993 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %160, align 8
  %994 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %993, i32 0, i32 0
  %995 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %994, i32 0, i32 0
  %996 = bitcast %union.anon.0* %995 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %997 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %996, i32 0, i32 0
  %998 = bitcast %union.anon.1* %997 to i8*
  %999 = load i8, i8* %998, align 8
  %1000 = zext i8 %999 to i32
  %1001 = ashr i32 %1000, 1
  %1002 = sext i32 %1001 to i64
  br label %1003

; <label>:1003:                                   ; preds = %988, %977
  %1004 = phi i64 [ %987, %977 ], [ %1002, %988 ]
  store i64 %1004, i64* %170, align 8
  %1005 = load i64, i64* %170, align 8
  %1006 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %169, align 8
  store %"class.std::__1::basic_string"* %1006, %"class.std::__1::basic_string"** %156, align 8
  %1007 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %156, align 8
  store %"class.std::__1::basic_string"* %1007, %"class.std::__1::basic_string"** %155, align 8
  %1008 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %155, align 8
  %1009 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1008, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1009, %"class.std::__1::__compressed_pair"** %154, align 8
  %1010 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %154, align 8
  %1011 = bitcast %"class.std::__1::__compressed_pair"* %1010 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1011, %"class.std::__1::__libcpp_compressed_pair_imp"** %153, align 8
  %1012 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %153, align 8
  %1013 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1012, i32 0, i32 0
  %1014 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1013, i32 0, i32 0
  %1015 = bitcast %union.anon.0* %1014 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1016 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1015, i32 0, i32 0
  %1017 = bitcast %union.anon.1* %1016 to i8*
  %1018 = load i8, i8* %1017, align 8
  %1019 = zext i8 %1018 to i32
  %1020 = and i32 %1019, 1
  %1021 = icmp ne i32 %1020, 0
  br i1 %1021, label %1022, label %1033

; <label>:1022:                                   ; preds = %1003
  store %"class.std::__1::basic_string"* %1007, %"class.std::__1::basic_string"** %149, align 8
  %1023 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %149, align 8
  %1024 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1023, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1024, %"class.std::__1::__compressed_pair"** %148, align 8
  %1025 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %148, align 8
  %1026 = bitcast %"class.std::__1::__compressed_pair"* %1025 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1026, %"class.std::__1::__libcpp_compressed_pair_imp"** %147, align 8
  %1027 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %147, align 8
  %1028 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1027, i32 0, i32 0
  %1029 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1028, i32 0, i32 0
  %1030 = bitcast %union.anon.0* %1029 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1031 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1030, i32 0, i32 1
  %1032 = load i64, i64* %1031, align 8
  br label %1048

; <label>:1033:                                   ; preds = %1003
  store %"class.std::__1::basic_string"* %1007, %"class.std::__1::basic_string"** %152, align 8
  %1034 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %152, align 8
  %1035 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1034, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1035, %"class.std::__1::__compressed_pair"** %151, align 8
  %1036 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %151, align 8
  %1037 = bitcast %"class.std::__1::__compressed_pair"* %1036 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1037, %"class.std::__1::__libcpp_compressed_pair_imp"** %150, align 8
  %1038 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %150, align 8
  %1039 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1038, i32 0, i32 0
  %1040 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1039, i32 0, i32 0
  %1041 = bitcast %union.anon.0* %1040 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1042 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1041, i32 0, i32 0
  %1043 = bitcast %union.anon.1* %1042 to i8*
  %1044 = load i8, i8* %1043, align 8
  %1045 = zext i8 %1044 to i32
  %1046 = ashr i32 %1045, 1
  %1047 = sext i32 %1046 to i64
  br label %1048

; <label>:1048:                                   ; preds = %1033, %1022
  %1049 = phi i64 [ %1032, %1022 ], [ %1047, %1033 ]
  %1050 = icmp ne i64 %1005, %1049
  br i1 %1050, label %1051, label %1052

; <label>:1051:                                   ; preds = %1048
  store i1 false, i1* %167, align 1
  br label %1187

; <label>:1052:                                   ; preds = %1048
  %1053 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %168, align 8
  store %"class.std::__1::basic_string"* %1053, %"class.std::__1::basic_string"** %146, align 8
  %1054 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %146, align 8
  store %"class.std::__1::basic_string"* %1054, %"class.std::__1::basic_string"** %145, align 8
  %1055 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %145, align 8
  store %"class.std::__1::basic_string"* %1055, %"class.std::__1::basic_string"** %144, align 8
  %1056 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %144, align 8
  %1057 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1056, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1057, %"class.std::__1::__compressed_pair"** %143, align 8
  %1058 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %143, align 8
  %1059 = bitcast %"class.std::__1::__compressed_pair"* %1058 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1059, %"class.std::__1::__libcpp_compressed_pair_imp"** %142, align 8
  %1060 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %142, align 8
  %1061 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1060, i32 0, i32 0
  %1062 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1061, i32 0, i32 0
  %1063 = bitcast %union.anon.0* %1062 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1064 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1063, i32 0, i32 0
  %1065 = bitcast %union.anon.1* %1064 to i8*
  %1066 = load i8, i8* %1065, align 8
  %1067 = zext i8 %1066 to i32
  %1068 = and i32 %1067, 1
  %1069 = icmp ne i32 %1068, 0
  br i1 %1069, label %1070, label %1081

; <label>:1070:                                   ; preds = %1052
  store %"class.std::__1::basic_string"* %1055, %"class.std::__1::basic_string"** %136, align 8
  %1071 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %136, align 8
  %1072 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1071, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1072, %"class.std::__1::__compressed_pair"** %135, align 8
  %1073 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %135, align 8
  %1074 = bitcast %"class.std::__1::__compressed_pair"* %1073 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1074, %"class.std::__1::__libcpp_compressed_pair_imp"** %134, align 8
  %1075 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %134, align 8
  %1076 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1075, i32 0, i32 0
  %1077 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1076, i32 0, i32 0
  %1078 = bitcast %union.anon.0* %1077 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1079 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1078, i32 0, i32 2
  %1080 = load i8*, i8** %1079, align 8
  br label %1094

; <label>:1081:                                   ; preds = %1052
  store %"class.std::__1::basic_string"* %1055, %"class.std::__1::basic_string"** %141, align 8
  %1082 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %141, align 8
  %1083 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1082, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1083, %"class.std::__1::__compressed_pair"** %140, align 8
  %1084 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %140, align 8
  %1085 = bitcast %"class.std::__1::__compressed_pair"* %1084 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1085, %"class.std::__1::__libcpp_compressed_pair_imp"** %139, align 8
  %1086 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %139, align 8
  %1087 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1086, i32 0, i32 0
  %1088 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1087, i32 0, i32 0
  %1089 = bitcast %union.anon.0* %1088 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1090 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1089, i32 0, i32 1
  %1091 = getelementptr inbounds [23 x i8], [23 x i8]* %1090, i64 0, i64 0
  store i8* %1091, i8** %138, align 8
  %1092 = load i8*, i8** %138, align 8
  store i8* %1092, i8** %137, align 8
  %1093 = load i8*, i8** %137, align 8
  br label %1094

; <label>:1094:                                   ; preds = %1081, %1070
  %1095 = phi i8* [ %1080, %1070 ], [ %1093, %1081 ]
  store i8* %1095, i8** %133, align 8
  %1096 = load i8*, i8** %133, align 8
  store i8* %1096, i8** %171, align 8
  %1097 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %169, align 8
  store %"class.std::__1::basic_string"* %1097, %"class.std::__1::basic_string"** %129, align 8
  %1098 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %129, align 8
  store %"class.std::__1::basic_string"* %1098, %"class.std::__1::basic_string"** %128, align 8
  %1099 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %128, align 8
  store %"class.std::__1::basic_string"* %1099, %"class.std::__1::basic_string"** %127, align 8
  %1100 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %127, align 8
  %1101 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1100, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1101, %"class.std::__1::__compressed_pair"** %126, align 8
  %1102 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %126, align 8
  %1103 = bitcast %"class.std::__1::__compressed_pair"* %1102 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1103, %"class.std::__1::__libcpp_compressed_pair_imp"** %125, align 8
  %1104 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %125, align 8
  %1105 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1104, i32 0, i32 0
  %1106 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1105, i32 0, i32 0
  %1107 = bitcast %union.anon.0* %1106 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1108 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1107, i32 0, i32 0
  %1109 = bitcast %union.anon.1* %1108 to i8*
  %1110 = load i8, i8* %1109, align 8
  %1111 = zext i8 %1110 to i32
  %1112 = and i32 %1111, 1
  %1113 = icmp ne i32 %1112, 0
  br i1 %1113, label %1114, label %1125

; <label>:1114:                                   ; preds = %1094
  store %"class.std::__1::basic_string"* %1099, %"class.std::__1::basic_string"** %119, align 8
  %1115 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %119, align 8
  %1116 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1115, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1116, %"class.std::__1::__compressed_pair"** %118, align 8
  %1117 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %118, align 8
  %1118 = bitcast %"class.std::__1::__compressed_pair"* %1117 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1118, %"class.std::__1::__libcpp_compressed_pair_imp"** %117, align 8
  %1119 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %117, align 8
  %1120 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1119, i32 0, i32 0
  %1121 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1120, i32 0, i32 0
  %1122 = bitcast %union.anon.0* %1121 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1123 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1122, i32 0, i32 2
  %1124 = load i8*, i8** %1123, align 8
  br label %1138

; <label>:1125:                                   ; preds = %1094
  store %"class.std::__1::basic_string"* %1099, %"class.std::__1::basic_string"** %124, align 8
  %1126 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %124, align 8
  %1127 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1126, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1127, %"class.std::__1::__compressed_pair"** %123, align 8
  %1128 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %123, align 8
  %1129 = bitcast %"class.std::__1::__compressed_pair"* %1128 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1129, %"class.std::__1::__libcpp_compressed_pair_imp"** %122, align 8
  %1130 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %122, align 8
  %1131 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1130, i32 0, i32 0
  %1132 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1131, i32 0, i32 0
  %1133 = bitcast %union.anon.0* %1132 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1134 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1133, i32 0, i32 1
  %1135 = getelementptr inbounds [23 x i8], [23 x i8]* %1134, i64 0, i64 0
  store i8* %1135, i8** %121, align 8
  %1136 = load i8*, i8** %121, align 8
  store i8* %1136, i8** %120, align 8
  %1137 = load i8*, i8** %120, align 8
  br label %1138

; <label>:1138:                                   ; preds = %1125, %1114
  %1139 = phi i8* [ %1124, %1114 ], [ %1137, %1125 ]
  store i8* %1139, i8** %116, align 8
  %1140 = load i8*, i8** %116, align 8
  store i8* %1140, i8** %172, align 8
  %1141 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %168, align 8
  store %"class.std::__1::basic_string"* %1141, %"class.std::__1::basic_string"** %132, align 8
  %1142 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %132, align 8
  %1143 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1142, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1143, %"class.std::__1::__compressed_pair"** %131, align 8
  %1144 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %131, align 8
  %1145 = bitcast %"class.std::__1::__compressed_pair"* %1144 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1145, %"class.std::__1::__libcpp_compressed_pair_imp"** %130, align 8
  %1146 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %130, align 8
  %1147 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1146, i32 0, i32 0
  %1148 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1147, i32 0, i32 0
  %1149 = bitcast %union.anon.0* %1148 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1150 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1149, i32 0, i32 0
  %1151 = bitcast %union.anon.1* %1150 to i8*
  %1152 = load i8, i8* %1151, align 8
  %1153 = zext i8 %1152 to i32
  %1154 = and i32 %1153, 1
  %1155 = icmp ne i32 %1154, 0
  br i1 %1155, label %1156, label %1163

; <label>:1156:                                   ; preds = %1138
  %1157 = load i8*, i8** %171, align 8
  %1158 = load i8*, i8** %172, align 8
  %1159 = load i64, i64* %170, align 8
  %1160 = invoke i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8* %1157, i8* %1158, i64 %1159)
          to label %1161 unwind label %1184

; <label>:1161:                                   ; preds = %1156
  %1162 = icmp eq i32 %1160, 0
  store i1 %1162, i1* %167, align 1
  br label %1187

; <label>:1163:                                   ; preds = %1138
  br label %1164

; <label>:1164:                                   ; preds = %1176, %1163
  %1165 = load i64, i64* %170, align 8
  %1166 = icmp ne i64 %1165, 0
  br i1 %1166, label %1167, label %1183

; <label>:1167:                                   ; preds = %1164
  %1168 = load i8*, i8** %171, align 8
  %1169 = load i8, i8* %1168, align 1
  %1170 = sext i8 %1169 to i32
  %1171 = load i8*, i8** %172, align 8
  %1172 = load i8, i8* %1171, align 1
  %1173 = sext i8 %1172 to i32
  %1174 = icmp ne i32 %1170, %1173
  br i1 %1174, label %1175, label %1176

; <label>:1175:                                   ; preds = %1167
  store i1 false, i1* %167, align 1
  br label %1187

; <label>:1176:                                   ; preds = %1167
  %1177 = load i64, i64* %170, align 8
  %1178 = add i64 %1177, -1
  store i64 %1178, i64* %170, align 8
  %1179 = load i8*, i8** %171, align 8
  %1180 = getelementptr inbounds i8, i8* %1179, i32 1
  store i8* %1180, i8** %171, align 8
  %1181 = load i8*, i8** %172, align 8
  %1182 = getelementptr inbounds i8, i8* %1181, i32 1
  store i8* %1182, i8** %172, align 8
  br label %1164

; <label>:1183:                                   ; preds = %1164
  store i1 true, i1* %167, align 1
  br label %1187

; <label>:1184:                                   ; preds = %1156
  %1185 = landingpad { i8*, i32 }
          catch i8* null
  %1186 = extractvalue { i8*, i32 } %1185, 0
  call void @__clang_call_terminate(i8* %1186) #12
  unreachable

; <label>:1187:                                   ; preds = %1051, %1161, %1175, %1183
  %1188 = load i1, i1* %167, align 1
  br i1 %1188, label %1189, label %1190

; <label>:1189:                                   ; preds = %1187
  store i1 true, i1* %275, align 1
  store i32 1, i32* %281, align 4
  br label %1191

; <label>:1190:                                   ; preds = %1187
  store i32 0, i32* %281, align 4
  br label %1191

; <label>:1191:                                   ; preds = %1190, %1189
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE9ValueTypeED1Ev(%"struct.std::__1::pair.73"* %280) #11
  %1192 = load i32, i32* %281, align 4
  switch i32 %1192, label %1206 [
    i32 0, label %1193
    i32 1, label %1204
  ]

; <label>:1193:                                   ; preds = %1191
  br label %1194

; <label>:1194:                                   ; preds = %1193
  store %"class.std::__1::__hash_map_iterator"* %278, %"class.std::__1::__hash_map_iterator"** %251, align 8
  %1195 = load %"class.std::__1::__hash_map_iterator"*, %"class.std::__1::__hash_map_iterator"** %251, align 8
  %1196 = getelementptr inbounds %"class.std::__1::__hash_map_iterator", %"class.std::__1::__hash_map_iterator"* %1195, i32 0, i32 0
  store %"class.std::__1::__hash_iterator"* %1196, %"class.std::__1::__hash_iterator"** %250, align 8
  %1197 = load %"class.std::__1::__hash_iterator"*, %"class.std::__1::__hash_iterator"** %250, align 8
  %1198 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %1197, i32 0, i32 0
  %1199 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %1198, align 8
  %1200 = getelementptr inbounds %"struct.std::__1::__hash_node_base.56", %"struct.std::__1::__hash_node_base.56"* %1199, i32 0, i32 0
  %1201 = load %"struct.std::__1::__hash_node_base.56"*, %"struct.std::__1::__hash_node_base.56"** %1200, align 8
  %1202 = getelementptr inbounds %"class.std::__1::__hash_iterator", %"class.std::__1::__hash_iterator"* %1197, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base.56"* %1201, %"struct.std::__1::__hash_node_base.56"** %1202, align 8
  br label %913

; <label>:1203:                                   ; preds = %913
  store i1 false, i1* %275, align 1
  br label %1204

; <label>:1204:                                   ; preds = %1203, %1191
  %1205 = load i1, i1* %275, align 1
  ret i1 %1205

; <label>:1206:                                   ; preds = %1191
  unreachable
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE9ValueTypeED1Ev(%"struct.std::__1::pair.73"*) unnamed_addr #2 align 2 {
  %2 = alloca %"struct.std::__1::pair.73"*, align 8
  store %"struct.std::__1::pair.73"* %0, %"struct.std::__1::pair.73"** %2, align 8
  %3 = load %"struct.std::__1::pair.73"*, %"struct.std::__1::pair.73"** %2, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE9ValueTypeED2Ev(%"struct.std::__1::pair.73"* %3) #11
  ret void
}

; Function Attrs: nounwind ssp uwtable
define zeroext i1 @_ZN5Lexer15is_bool_literalENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Lexer*, %"class.std::__1::basic_string"*) #5 align 2 {
  %3 = alloca %"class.std::__1::basic_string"*, align 8
  %4 = alloca i8*, align 8
  %5 = alloca %"class.std::__1::basic_string"*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %class.Lexer*, align 8
  store %class.Lexer* %0, %class.Lexer** %7, align 8
  %8 = load %class.Lexer*, %class.Lexer** %7, align 8
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %5, align 8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.2, i32 0, i32 0), i8** %6, align 8
  %9 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %5, align 8
  %10 = load i8*, i8** %6, align 8
  %11 = call i32 @_ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7compareEPKc(%"class.std::__1::basic_string"* %9, i8* %10) #11
  %12 = icmp eq i32 %11, 0
  br i1 %12, label %18, label %13

; <label>:13:                                     ; preds = %2
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %3, align 8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.3, i32 0, i32 0), i8** %4, align 8
  %14 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %3, align 8
  %15 = load i8*, i8** %4, align 8
  %16 = call i32 @_ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7compareEPKc(%"class.std::__1::basic_string"* %14, i8* %15) #11
  %17 = icmp eq i32 %16, 0
  br label %18

; <label>:18:                                     ; preds = %13, %2
  %19 = phi i1 [ true, %2 ], [ %17, %13 ]
  ret i1 %19
}

; Function Attrs: ssp uwtable
define signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer*) #0 align 2 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %4 = alloca %"class.std::__1::basic_string"*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %8 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %9 = alloca %"class.std::__1::basic_string"*, align 8
  %10 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %11 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca %"class.std::__1::basic_string"*, align 8
  %14 = alloca %"class.std::__1::basic_string"*, align 8
  %15 = alloca i64, align 8
  %16 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %17 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %18 = alloca %"class.std::__1::basic_string"*, align 8
  %19 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %20 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %21 = alloca %"class.std::__1::basic_string"*, align 8
  %22 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %23 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %24 = alloca %"class.std::__1::basic_string"*, align 8
  %25 = alloca %"class.std::__1::basic_string"*, align 8
  %26 = alloca %"class.std::__1::basic_string"*, align 8
  %27 = alloca i8, align 1
  %28 = alloca %class.Lexer*, align 8
  %29 = alloca i8, align 1
  store %class.Lexer* %0, %class.Lexer** %28, align 8
  %30 = load %class.Lexer*, %class.Lexer** %28, align 8
  %31 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 0
  %32 = load i32, i32* %31, align 8
  %33 = sext i32 %32 to i64
  %34 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 8
  store %"class.std::__1::basic_string"* %34, %"class.std::__1::basic_string"** %26, align 8
  %35 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %26, align 8
  store %"class.std::__1::basic_string"* %35, %"class.std::__1::basic_string"** %25, align 8
  %36 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %25, align 8
  store %"class.std::__1::basic_string"* %36, %"class.std::__1::basic_string"** %24, align 8
  %37 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8
  %38 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %37, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %38, %"class.std::__1::__compressed_pair"** %23, align 8
  %39 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %23, align 8
  %40 = bitcast %"class.std::__1::__compressed_pair"* %39 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %40, %"class.std::__1::__libcpp_compressed_pair_imp"** %22, align 8
  %41 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %22, align 8
  %42 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %41, i32 0, i32 0
  %43 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %42, i32 0, i32 0
  %44 = bitcast %union.anon.0* %43 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %45 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %44, i32 0, i32 0
  %46 = bitcast %union.anon.1* %45 to i8*
  %47 = load i8, i8* %46, align 8
  %48 = zext i8 %47 to i32
  %49 = and i32 %48, 1
  %50 = icmp ne i32 %49, 0
  br i1 %50, label %51, label %62

; <label>:51:                                     ; preds = %1
  store %"class.std::__1::basic_string"* %36, %"class.std::__1::basic_string"** %18, align 8
  %52 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %18, align 8
  %53 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %52, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %53, %"class.std::__1::__compressed_pair"** %17, align 8
  %54 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %17, align 8
  %55 = bitcast %"class.std::__1::__compressed_pair"* %54 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %55, %"class.std::__1::__libcpp_compressed_pair_imp"** %16, align 8
  %56 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %16, align 8
  %57 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %56, i32 0, i32 0
  %58 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %57, i32 0, i32 0
  %59 = bitcast %union.anon.0* %58 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %60 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %59, i32 0, i32 1
  %61 = load i64, i64* %60, align 8
  br label %77

; <label>:62:                                     ; preds = %1
  store %"class.std::__1::basic_string"* %36, %"class.std::__1::basic_string"** %21, align 8
  %63 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %21, align 8
  %64 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %63, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %64, %"class.std::__1::__compressed_pair"** %20, align 8
  %65 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %20, align 8
  %66 = bitcast %"class.std::__1::__compressed_pair"* %65 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %66, %"class.std::__1::__libcpp_compressed_pair_imp"** %19, align 8
  %67 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %19, align 8
  %68 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %67, i32 0, i32 0
  %69 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %68, i32 0, i32 0
  %70 = bitcast %union.anon.0* %69 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %71 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %70, i32 0, i32 0
  %72 = bitcast %union.anon.1* %71 to i8*
  %73 = load i8, i8* %72, align 8
  %74 = zext i8 %73 to i32
  %75 = ashr i32 %74, 1
  %76 = sext i32 %75 to i64
  br label %77

; <label>:77:                                     ; preds = %51, %62
  %78 = phi i64 [ %61, %51 ], [ %76, %62 ]
  %79 = icmp uge i64 %33, %78
  br i1 %79, label %80, label %84

; <label>:80:                                     ; preds = %77
  %81 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 0
  %82 = load i32, i32* %81, align 8
  %83 = add nsw i32 %82, 1
  store i32 %83, i32* %81, align 8
  store i8 0, i8* %27, align 1
  br label %148

; <label>:84:                                     ; preds = %77
  %85 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 8
  %86 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 0
  %87 = load i32, i32* %86, align 8
  %88 = add nsw i32 %87, 1
  store i32 %88, i32* %86, align 8
  %89 = sext i32 %87 to i64
  store %"class.std::__1::basic_string"* %85, %"class.std::__1::basic_string"** %14, align 8
  store i64 %89, i64* %15, align 8
  %90 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8
  store %"class.std::__1::basic_string"* %90, %"class.std::__1::basic_string"** %13, align 8
  %91 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8
  store %"class.std::__1::basic_string"* %91, %"class.std::__1::basic_string"** %12, align 8
  %92 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8
  %93 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %92, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %93, %"class.std::__1::__compressed_pair"** %11, align 8
  %94 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %11, align 8
  %95 = bitcast %"class.std::__1::__compressed_pair"* %94 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %95, %"class.std::__1::__libcpp_compressed_pair_imp"** %10, align 8
  %96 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %10, align 8
  %97 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %96, i32 0, i32 0
  %98 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %97, i32 0, i32 0
  %99 = bitcast %union.anon.0* %98 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %100 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %99, i32 0, i32 0
  %101 = bitcast %union.anon.1* %100 to i8*
  %102 = load i8, i8* %101, align 8
  %103 = zext i8 %102 to i32
  %104 = and i32 %103, 1
  %105 = icmp ne i32 %104, 0
  br i1 %105, label %106, label %117

; <label>:106:                                    ; preds = %84
  store %"class.std::__1::basic_string"* %91, %"class.std::__1::basic_string"** %4, align 8
  %107 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %4, align 8
  %108 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %107, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %108, %"class.std::__1::__compressed_pair"** %3, align 8
  %109 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %3, align 8
  %110 = bitcast %"class.std::__1::__compressed_pair"* %109 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %110, %"class.std::__1::__libcpp_compressed_pair_imp"** %2, align 8
  %111 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %2, align 8
  %112 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %111, i32 0, i32 0
  %113 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %112, i32 0, i32 0
  %114 = bitcast %union.anon.0* %113 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %115 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %114, i32 0, i32 2
  %116 = load i8*, i8** %115, align 8
  br label %130

; <label>:117:                                    ; preds = %84
  store %"class.std::__1::basic_string"* %91, %"class.std::__1::basic_string"** %9, align 8
  %118 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %9, align 8
  %119 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %118, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %119, %"class.std::__1::__compressed_pair"** %8, align 8
  %120 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %8, align 8
  %121 = bitcast %"class.std::__1::__compressed_pair"* %120 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %121, %"class.std::__1::__libcpp_compressed_pair_imp"** %7, align 8
  %122 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %7, align 8
  %123 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %122, i32 0, i32 0
  %124 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %123, i32 0, i32 0
  %125 = bitcast %union.anon.0* %124 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %126 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %125, i32 0, i32 1
  %127 = getelementptr inbounds [23 x i8], [23 x i8]* %126, i64 0, i64 0
  store i8* %127, i8** %6, align 8
  %128 = load i8*, i8** %6, align 8
  store i8* %128, i8** %5, align 8
  %129 = load i8*, i8** %5, align 8
  br label %130

; <label>:130:                                    ; preds = %106, %117
  %131 = phi i8* [ %116, %106 ], [ %129, %117 ]
  %132 = load i64, i64* %15, align 8
  %133 = getelementptr inbounds i8, i8* %131, i64 %132
  %134 = load i8, i8* %133, align 1
  store i8 %134, i8* %29, align 1
  %135 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 2
  %136 = load i32, i32* %135, align 8
  %137 = add nsw i32 %136, 1
  store i32 %137, i32* %135, align 8
  %138 = load i8, i8* %29, align 1
  %139 = sext i8 %138 to i32
  %140 = icmp eq i32 %139, 10
  br i1 %140, label %141, label %146

; <label>:141:                                    ; preds = %130
  %142 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 1
  %143 = load i32, i32* %142, align 4
  %144 = add nsw i32 %143, 1
  store i32 %144, i32* %142, align 4
  %145 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 2
  store i32 0, i32* %145, align 8
  br label %146

; <label>:146:                                    ; preds = %141, %130
  %147 = load i8, i8* %29, align 1
  store i8 %147, i8* %27, align 1
  br label %148

; <label>:148:                                    ; preds = %146, %80
  %149 = load i8, i8* %27, align 1
  ret i8 %149
}

; Function Attrs: ssp uwtable
define signext i8 @_ZN5Lexer14char_lookaheadEv(%class.Lexer*) #0 align 2 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %4 = alloca %"class.std::__1::basic_string"*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %8 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %9 = alloca %"class.std::__1::basic_string"*, align 8
  %10 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %11 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca %"class.std::__1::basic_string"*, align 8
  %14 = alloca %"class.std::__1::basic_string"*, align 8
  %15 = alloca i64, align 8
  %16 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %17 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %18 = alloca %"class.std::__1::basic_string"*, align 8
  %19 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %20 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %21 = alloca %"class.std::__1::basic_string"*, align 8
  %22 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %23 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %24 = alloca %"class.std::__1::basic_string"*, align 8
  %25 = alloca %"class.std::__1::basic_string"*, align 8
  %26 = alloca %"class.std::__1::basic_string"*, align 8
  %27 = alloca i8, align 1
  %28 = alloca %class.Lexer*, align 8
  %29 = alloca i8, align 1
  store %class.Lexer* %0, %class.Lexer** %28, align 8
  %30 = load %class.Lexer*, %class.Lexer** %28, align 8
  %31 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 0
  %32 = load i32, i32* %31, align 8
  %33 = sext i32 %32 to i64
  %34 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 8
  store %"class.std::__1::basic_string"* %34, %"class.std::__1::basic_string"** %26, align 8
  %35 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %26, align 8
  store %"class.std::__1::basic_string"* %35, %"class.std::__1::basic_string"** %25, align 8
  %36 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %25, align 8
  store %"class.std::__1::basic_string"* %36, %"class.std::__1::basic_string"** %24, align 8
  %37 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8
  %38 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %37, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %38, %"class.std::__1::__compressed_pair"** %23, align 8
  %39 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %23, align 8
  %40 = bitcast %"class.std::__1::__compressed_pair"* %39 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %40, %"class.std::__1::__libcpp_compressed_pair_imp"** %22, align 8
  %41 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %22, align 8
  %42 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %41, i32 0, i32 0
  %43 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %42, i32 0, i32 0
  %44 = bitcast %union.anon.0* %43 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %45 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %44, i32 0, i32 0
  %46 = bitcast %union.anon.1* %45 to i8*
  %47 = load i8, i8* %46, align 8
  %48 = zext i8 %47 to i32
  %49 = and i32 %48, 1
  %50 = icmp ne i32 %49, 0
  br i1 %50, label %51, label %62

; <label>:51:                                     ; preds = %1
  store %"class.std::__1::basic_string"* %36, %"class.std::__1::basic_string"** %18, align 8
  %52 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %18, align 8
  %53 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %52, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %53, %"class.std::__1::__compressed_pair"** %17, align 8
  %54 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %17, align 8
  %55 = bitcast %"class.std::__1::__compressed_pair"* %54 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %55, %"class.std::__1::__libcpp_compressed_pair_imp"** %16, align 8
  %56 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %16, align 8
  %57 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %56, i32 0, i32 0
  %58 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %57, i32 0, i32 0
  %59 = bitcast %union.anon.0* %58 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %60 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %59, i32 0, i32 1
  %61 = load i64, i64* %60, align 8
  br label %77

; <label>:62:                                     ; preds = %1
  store %"class.std::__1::basic_string"* %36, %"class.std::__1::basic_string"** %21, align 8
  %63 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %21, align 8
  %64 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %63, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %64, %"class.std::__1::__compressed_pair"** %20, align 8
  %65 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %20, align 8
  %66 = bitcast %"class.std::__1::__compressed_pair"* %65 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %66, %"class.std::__1::__libcpp_compressed_pair_imp"** %19, align 8
  %67 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %19, align 8
  %68 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %67, i32 0, i32 0
  %69 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %68, i32 0, i32 0
  %70 = bitcast %union.anon.0* %69 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %71 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %70, i32 0, i32 0
  %72 = bitcast %union.anon.1* %71 to i8*
  %73 = load i8, i8* %72, align 8
  %74 = zext i8 %73 to i32
  %75 = ashr i32 %74, 1
  %76 = sext i32 %75 to i64
  br label %77

; <label>:77:                                     ; preds = %51, %62
  %78 = phi i64 [ %61, %51 ], [ %76, %62 ]
  %79 = icmp uge i64 %33, %78
  br i1 %79, label %80, label %84

; <label>:80:                                     ; preds = %77
  %81 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 0
  %82 = load i32, i32* %81, align 8
  %83 = add nsw i32 %82, 1
  store i32 %83, i32* %81, align 8
  store i8 0, i8* %27, align 1
  br label %147

; <label>:84:                                     ; preds = %77
  %85 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 8
  %86 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 0
  %87 = load i32, i32* %86, align 8
  %88 = sext i32 %87 to i64
  store %"class.std::__1::basic_string"* %85, %"class.std::__1::basic_string"** %14, align 8
  store i64 %88, i64* %15, align 8
  %89 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8
  store %"class.std::__1::basic_string"* %89, %"class.std::__1::basic_string"** %13, align 8
  %90 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8
  store %"class.std::__1::basic_string"* %90, %"class.std::__1::basic_string"** %12, align 8
  %91 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8
  %92 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %91, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %92, %"class.std::__1::__compressed_pair"** %11, align 8
  %93 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %11, align 8
  %94 = bitcast %"class.std::__1::__compressed_pair"* %93 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %94, %"class.std::__1::__libcpp_compressed_pair_imp"** %10, align 8
  %95 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %10, align 8
  %96 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %95, i32 0, i32 0
  %97 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %96, i32 0, i32 0
  %98 = bitcast %union.anon.0* %97 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %99 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %98, i32 0, i32 0
  %100 = bitcast %union.anon.1* %99 to i8*
  %101 = load i8, i8* %100, align 8
  %102 = zext i8 %101 to i32
  %103 = and i32 %102, 1
  %104 = icmp ne i32 %103, 0
  br i1 %104, label %105, label %116

; <label>:105:                                    ; preds = %84
  store %"class.std::__1::basic_string"* %90, %"class.std::__1::basic_string"** %4, align 8
  %106 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %4, align 8
  %107 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %106, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %107, %"class.std::__1::__compressed_pair"** %3, align 8
  %108 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %3, align 8
  %109 = bitcast %"class.std::__1::__compressed_pair"* %108 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %109, %"class.std::__1::__libcpp_compressed_pair_imp"** %2, align 8
  %110 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %2, align 8
  %111 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %110, i32 0, i32 0
  %112 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %111, i32 0, i32 0
  %113 = bitcast %union.anon.0* %112 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %114 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %113, i32 0, i32 2
  %115 = load i8*, i8** %114, align 8
  br label %129

; <label>:116:                                    ; preds = %84
  store %"class.std::__1::basic_string"* %90, %"class.std::__1::basic_string"** %9, align 8
  %117 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %9, align 8
  %118 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %117, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %118, %"class.std::__1::__compressed_pair"** %8, align 8
  %119 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %8, align 8
  %120 = bitcast %"class.std::__1::__compressed_pair"* %119 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %120, %"class.std::__1::__libcpp_compressed_pair_imp"** %7, align 8
  %121 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %7, align 8
  %122 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %121, i32 0, i32 0
  %123 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %122, i32 0, i32 0
  %124 = bitcast %union.anon.0* %123 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %125 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %124, i32 0, i32 1
  %126 = getelementptr inbounds [23 x i8], [23 x i8]* %125, i64 0, i64 0
  store i8* %126, i8** %6, align 8
  %127 = load i8*, i8** %6, align 8
  store i8* %127, i8** %5, align 8
  %128 = load i8*, i8** %5, align 8
  br label %129

; <label>:129:                                    ; preds = %105, %116
  %130 = phi i8* [ %115, %105 ], [ %128, %116 ]
  %131 = load i64, i64* %15, align 8
  %132 = getelementptr inbounds i8, i8* %130, i64 %131
  %133 = load i8, i8* %132, align 1
  store i8 %133, i8* %29, align 1
  %134 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 2
  %135 = load i32, i32* %134, align 8
  %136 = add nsw i32 %135, 1
  store i32 %136, i32* %134, align 8
  %137 = load i8, i8* %29, align 1
  %138 = sext i8 %137 to i32
  %139 = icmp eq i32 %138, 10
  br i1 %139, label %140, label %145

; <label>:140:                                    ; preds = %129
  %141 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 1
  %142 = load i32, i32* %141, align 4
  %143 = add nsw i32 %142, 1
  store i32 %143, i32* %141, align 4
  %144 = getelementptr inbounds %class.Lexer, %class.Lexer* %30, i32 0, i32 2
  store i32 0, i32* %144, align 8
  br label %145

; <label>:145:                                    ; preds = %140, %129
  %146 = load i8, i8* %29, align 1
  store i8 %146, i8* %27, align 1
  br label %147

; <label>:147:                                    ; preds = %145, %80
  %148 = load i8, i8* %27, align 1
  ret i8 %148
}

; Function Attrs: nounwind ssp uwtable
define void @_ZN5Lexer9backtrackEv(%class.Lexer*) #5 align 2 {
  %2 = alloca %class.Lexer*, align 8
  store %class.Lexer* %0, %class.Lexer** %2, align 8
  %3 = load %class.Lexer*, %class.Lexer** %2, align 8
  %4 = getelementptr inbounds %class.Lexer, %class.Lexer* %3, i32 0, i32 4
  %5 = load i32, i32* %4, align 8
  %6 = add nsw i32 %5, -1
  store i32 %6, i32* %4, align 8
  %7 = getelementptr inbounds %class.Lexer, %class.Lexer* %3, i32 0, i32 3
  %8 = load i32, i32* %7, align 4
  %9 = getelementptr inbounds %class.Lexer, %class.Lexer* %3, i32 0, i32 0
  store i32 %8, i32* %9, align 8
  ret void
}

; Function Attrs: nounwind ssp uwtable
define void @_ZN5Lexer11backtrack_cEi(%class.Lexer*, i32) #5 align 2 {
  %3 = alloca %class.Lexer*, align 8
  %4 = alloca i32, align 4
  store %class.Lexer* %0, %class.Lexer** %3, align 8
  store i32 %1, i32* %4, align 4
  %5 = load %class.Lexer*, %class.Lexer** %3, align 8
  %6 = load i32, i32* %4, align 4
  %7 = getelementptr inbounds %class.Lexer, %class.Lexer* %5, i32 0, i32 0
  %8 = load i32, i32* %7, align 8
  %9 = sub nsw i32 %8, %6
  store i32 %9, i32* %7, align 8
  ret void
}

; Function Attrs: ssp uwtable
define void @_ZN5Lexer14get_next_tokenEb(%class.Token* noalias sret, %class.Lexer*, i1 zeroext) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::vector.27"*, align 8
  %5 = alloca i64, align 8
  %6 = alloca %"class.std::__1::vector.27"*, align 8
  %7 = alloca %"class.std::__1::vector.27"*, align 8
  %8 = alloca %class.Lexer*, align 8
  %9 = alloca i8, align 1
  %10 = alloca %class.Token, align 8
  %11 = alloca i8*
  %12 = alloca i32
  %13 = alloca i32
  store %class.Lexer* %1, %class.Lexer** %8, align 8
  %14 = zext i1 %2 to i8
  store i8 %14, i8* %9, align 1
  %15 = load %class.Lexer*, %class.Lexer** %8, align 8
  %16 = getelementptr inbounds %class.Lexer, %class.Lexer* %15, i32 0, i32 4
  %17 = load i32, i32* %16, align 8
  %18 = sext i32 %17 to i64
  %19 = getelementptr inbounds %class.Lexer, %class.Lexer* %15, i32 0, i32 7
  store %"class.std::__1::vector.27"* %19, %"class.std::__1::vector.27"** %7, align 8
  %20 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %7, align 8
  %21 = bitcast %"class.std::__1::vector.27"* %20 to %"class.std::__1::__vector_base.28"*
  %22 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %21, i32 0, i32 1
  %23 = load %class.Token*, %class.Token** %22, align 8
  %24 = bitcast %"class.std::__1::vector.27"* %20 to %"class.std::__1::__vector_base.28"*
  %25 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %24, i32 0, i32 0
  %26 = load %class.Token*, %class.Token** %25, align 8
  %27 = ptrtoint %class.Token* %23 to i64
  %28 = ptrtoint %class.Token* %26 to i64
  %29 = sub i64 %27, %28
  %30 = sdiv exact i64 %29, 288
  %31 = sub i64 %30, 1
  %32 = icmp uge i64 %18, %31
  br i1 %32, label %33, label %40

; <label>:33:                                     ; preds = %3
  %34 = getelementptr inbounds %class.Lexer, %class.Lexer* %15, i32 0, i32 7
  store %"class.std::__1::vector.27"* %34, %"class.std::__1::vector.27"** %6, align 8
  %35 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %6, align 8
  %36 = bitcast %"class.std::__1::vector.27"* %35 to %"class.std::__1::__vector_base.28"*
  %37 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %36, i32 0, i32 1
  %38 = load %class.Token*, %class.Token** %37, align 8
  %39 = getelementptr inbounds %class.Token, %class.Token* %38, i64 -1
  call void @_ZN5TokenC1ERKS_(%class.Token* %0, %class.Token* dereferenceable(288) %39)
  br label %72

; <label>:40:                                     ; preds = %3
  %41 = getelementptr inbounds %class.Lexer, %class.Lexer* %15, i32 0, i32 7
  %42 = getelementptr inbounds %class.Lexer, %class.Lexer* %15, i32 0, i32 4
  %43 = load i32, i32* %42, align 8
  %44 = add nsw i32 %43, 1
  store i32 %44, i32* %42, align 8
  %45 = sext i32 %43 to i64
  store %"class.std::__1::vector.27"* %41, %"class.std::__1::vector.27"** %4, align 8
  store i64 %45, i64* %5, align 8
  %46 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %4, align 8
  %47 = load i64, i64* %5, align 8
  %48 = bitcast %"class.std::__1::vector.27"* %46 to %"class.std::__1::__vector_base.28"*
  %49 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %48, i32 0, i32 0
  %50 = load %class.Token*, %class.Token** %49, align 8
  %51 = getelementptr inbounds %class.Token, %class.Token* %50, i64 %47
  call void @_ZN5TokenC1ERKS_(%class.Token* %10, %class.Token* dereferenceable(288) %51)
  %52 = load i8, i8* %9, align 1
  %53 = trunc i8 %52 to i1
  br i1 %53, label %54, label %63

; <label>:54:                                     ; preds = %40
  %55 = invoke zeroext i1 @_ZN5Token13is_punctuatorEc(%class.Token* %10, i8 signext 10)
          to label %56 unwind label %59

; <label>:56:                                     ; preds = %54
  br i1 %55, label %57, label %63

; <label>:57:                                     ; preds = %56
  invoke void @_ZN5Lexer14get_next_tokenEb(%class.Token* sret %0, %class.Lexer* %15, i1 zeroext true)
          to label %58 unwind label %59

; <label>:58:                                     ; preds = %57
  store i32 1, i32* %13, align 4
  br label %71

; <label>:59:                                     ; preds = %68, %65, %63, %57, %54
  %60 = landingpad { i8*, i32 }
          cleanup
  %61 = extractvalue { i8*, i32 } %60, 0
  store i8* %61, i8** %11, align 8
  %62 = extractvalue { i8*, i32 } %60, 1
  store i32 %62, i32* %12, align 4
  call void @_ZN5TokenD1Ev(%class.Token* %10) #11
  br label %73

; <label>:63:                                     ; preds = %56, %40
  %64 = invoke i32 @_ZNK5Token9get_startEv(%class.Token* %10)
          to label %65 unwind label %59

; <label>:65:                                     ; preds = %63
  %66 = getelementptr inbounds %class.Lexer, %class.Lexer* %15, i32 0, i32 3
  store i32 %64, i32* %66, align 4
  %67 = invoke i32 @_ZNK5Token7get_endEv(%class.Token* %10)
          to label %68 unwind label %59

; <label>:68:                                     ; preds = %65
  %69 = getelementptr inbounds %class.Lexer, %class.Lexer* %15, i32 0, i32 0
  store i32 %67, i32* %69, align 8
  invoke void @_ZN5TokenC1EOS_(%class.Token* %0, %class.Token* dereferenceable(288) %10)
          to label %70 unwind label %59

; <label>:70:                                     ; preds = %68
  store i32 1, i32* %13, align 4
  br label %71

; <label>:71:                                     ; preds = %70, %58
  call void @_ZN5TokenD1Ev(%class.Token* %10) #11
  br label %72

; <label>:72:                                     ; preds = %71, %33
  ret void

; <label>:73:                                     ; preds = %59
  %74 = load i8*, i8** %11, align 8
  %75 = load i32, i32* %12, align 4
  %76 = insertvalue { i8*, i32 } undef, i8* %74, 0
  %77 = insertvalue { i8*, i32 } %76, i32 %75, 1
  resume { i8*, i32 } %77
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZN5TokenC1ERKS_(%class.Token*, %class.Token* dereferenceable(288)) unnamed_addr #3 align 2 {
  %3 = alloca %class.Token*, align 8
  %4 = alloca %class.Token*, align 8
  store %class.Token* %0, %class.Token** %3, align 8
  store %class.Token* %1, %class.Token** %4, align 8
  %5 = load %class.Token*, %class.Token** %3, align 8
  %6 = load %class.Token*, %class.Token** %4, align 8
  call void @_ZN5TokenC2ERKS_(%class.Token* %5, %class.Token* dereferenceable(288) %6)
  ret void
}

declare zeroext i1 @_ZN5Token13is_punctuatorEc(%class.Token*, i8 signext) #1

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr i32 @_ZNK5Token7get_endEv(%class.Token*) #2 align 2 {
  %2 = alloca %class.Token*, align 8
  store %class.Token* %0, %class.Token** %2, align 8
  %3 = load %class.Token*, %class.Token** %2, align 8
  %4 = getelementptr inbounds %class.Token, %class.Token* %3, i32 0, i32 4
  %5 = load i32, i32* %4, align 4
  ret i32 %5
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZN5TokenC1EOS_(%class.Token*, %class.Token* dereferenceable(288)) unnamed_addr #3 align 2 {
  %3 = alloca %class.Token*, align 8
  %4 = alloca %class.Token*, align 8
  store %class.Token* %0, %class.Token** %3, align 8
  store %class.Token* %1, %class.Token** %4, align 8
  %5 = load %class.Token*, %class.Token** %3, align 8
  %6 = load %class.Token*, %class.Token** %4, align 8
  call void @_ZN5TokenC2EOS_(%class.Token* %5, %class.Token* dereferenceable(288) %6)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define signext i8 @_ZN5Lexer11escape_charEc(%class.Lexer*, i8 signext) #5 align 2 {
  %3 = alloca i8, align 1
  %4 = alloca %class.Lexer*, align 8
  %5 = alloca i8, align 1
  store %class.Lexer* %0, %class.Lexer** %4, align 8
  store i8 %1, i8* %5, align 1
  %6 = load %class.Lexer*, %class.Lexer** %4, align 8
  %7 = load i8, i8* %5, align 1
  %8 = sext i8 %7 to i32
  switch i32 %8, label %18 [
    i32 110, label %9
    i32 97, label %10
    i32 114, label %11
    i32 118, label %12
    i32 116, label %13
    i32 98, label %14
    i32 34, label %15
    i32 39, label %16
    i32 48, label %17
  ]

; <label>:9:                                      ; preds = %2
  store i8 10, i8* %3, align 1
  br label %20

; <label>:10:                                     ; preds = %2
  store i8 7, i8* %3, align 1
  br label %20

; <label>:11:                                     ; preds = %2
  store i8 13, i8* %3, align 1
  br label %20

; <label>:12:                                     ; preds = %2
  store i8 11, i8* %3, align 1
  br label %20

; <label>:13:                                     ; preds = %2
  store i8 9, i8* %3, align 1
  br label %20

; <label>:14:                                     ; preds = %2
  store i8 8, i8* %3, align 1
  br label %20

; <label>:15:                                     ; preds = %2
  store i8 34, i8* %3, align 1
  br label %20

; <label>:16:                                     ; preds = %2
  store i8 39, i8* %3, align 1
  br label %20

; <label>:17:                                     ; preds = %2
  store i8 0, i8* %3, align 1
  br label %20

; <label>:18:                                     ; preds = %2
  %19 = load i8, i8* %5, align 1
  store i8 %19, i8* %3, align 1
  br label %20

; <label>:20:                                     ; preds = %18, %17, %16, %15, %14, %13, %12, %11, %10, %9
  %21 = load i8, i8* %3, align 1
  ret i8 %21
}

; Function Attrs: ssp uwtable
define void @_ZN5Lexer14ignore_commentEv(%class.Lexer*) #0 align 2 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %4 = alloca %"class.std::__1::basic_string"*, align 8
  %5 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %6 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %7 = alloca %"class.std::__1::basic_string"*, align 8
  %8 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %9 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %10 = alloca %"class.std::__1::basic_string"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca %class.Lexer*, align 8
  %14 = alloca i8, align 1
  store %class.Lexer* %0, %class.Lexer** %13, align 8
  %15 = load %class.Lexer*, %class.Lexer** %13, align 8
  %16 = call signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %15)
  store i8 %16, i8* %14, align 1
  br label %17

; <label>:17:                                     ; preds = %72, %1
  %18 = load i8, i8* %14, align 1
  %19 = sext i8 %18 to i32
  %20 = icmp ne i32 %19, 10
  br i1 %20, label %21, label %74

; <label>:21:                                     ; preds = %17
  %22 = getelementptr inbounds %class.Lexer, %class.Lexer* %15, i32 0, i32 0
  %23 = load i32, i32* %22, align 8
  %24 = sext i32 %23 to i64
  %25 = getelementptr inbounds %class.Lexer, %class.Lexer* %15, i32 0, i32 8
  store %"class.std::__1::basic_string"* %25, %"class.std::__1::basic_string"** %12, align 8
  %26 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8
  store %"class.std::__1::basic_string"* %26, %"class.std::__1::basic_string"** %11, align 8
  %27 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8
  store %"class.std::__1::basic_string"* %27, %"class.std::__1::basic_string"** %10, align 8
  %28 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8
  %29 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %28, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %29, %"class.std::__1::__compressed_pair"** %9, align 8
  %30 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8
  %31 = bitcast %"class.std::__1::__compressed_pair"* %30 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %31, %"class.std::__1::__libcpp_compressed_pair_imp"** %8, align 8
  %32 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %8, align 8
  %33 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %32, i32 0, i32 0
  %34 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %33, i32 0, i32 0
  %35 = bitcast %union.anon.0* %34 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %36 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %35, i32 0, i32 0
  %37 = bitcast %union.anon.1* %36 to i8*
  %38 = load i8, i8* %37, align 8
  %39 = zext i8 %38 to i32
  %40 = and i32 %39, 1
  %41 = icmp ne i32 %40, 0
  br i1 %41, label %42, label %53

; <label>:42:                                     ; preds = %21
  store %"class.std::__1::basic_string"* %27, %"class.std::__1::basic_string"** %4, align 8
  %43 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %4, align 8
  %44 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %43, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %44, %"class.std::__1::__compressed_pair"** %3, align 8
  %45 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %3, align 8
  %46 = bitcast %"class.std::__1::__compressed_pair"* %45 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %46, %"class.std::__1::__libcpp_compressed_pair_imp"** %2, align 8
  %47 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %2, align 8
  %48 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %47, i32 0, i32 0
  %49 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %48, i32 0, i32 0
  %50 = bitcast %union.anon.0* %49 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %51 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %50, i32 0, i32 1
  %52 = load i64, i64* %51, align 8
  br label %68

; <label>:53:                                     ; preds = %21
  store %"class.std::__1::basic_string"* %27, %"class.std::__1::basic_string"** %7, align 8
  %54 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %7, align 8
  %55 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %54, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %55, %"class.std::__1::__compressed_pair"** %6, align 8
  %56 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %6, align 8
  %57 = bitcast %"class.std::__1::__compressed_pair"* %56 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %57, %"class.std::__1::__libcpp_compressed_pair_imp"** %5, align 8
  %58 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %5, align 8
  %59 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %58, i32 0, i32 0
  %60 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %59, i32 0, i32 0
  %61 = bitcast %union.anon.0* %60 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %62 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %61, i32 0, i32 0
  %63 = bitcast %union.anon.1* %62 to i8*
  %64 = load i8, i8* %63, align 8
  %65 = zext i8 %64 to i32
  %66 = ashr i32 %65, 1
  %67 = sext i32 %66 to i64
  br label %68

; <label>:68:                                     ; preds = %42, %53
  %69 = phi i64 [ %52, %42 ], [ %67, %53 ]
  %70 = icmp uge i64 %24, %69
  br i1 %70, label %71, label %72

; <label>:71:                                     ; preds = %68
  br label %74

; <label>:72:                                     ; preds = %68
  %73 = call signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %15)
  store i8 %73, i8* %14, align 1
  br label %17

; <label>:74:                                     ; preds = %71, %17
  ret void
}

; Function Attrs: ssp uwtable
define void @_ZN5Lexer17ignore_ml_commentEv(%class.Lexer*) #0 align 2 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %4 = alloca %"class.std::__1::basic_string"*, align 8
  %5 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %6 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %7 = alloca %"class.std::__1::basic_string"*, align 8
  %8 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %9 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %10 = alloca %"class.std::__1::basic_string"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca %class.Lexer*, align 8
  %14 = alloca i8, align 1
  store %class.Lexer* %0, %class.Lexer** %13, align 8
  %15 = load %class.Lexer*, %class.Lexer** %13, align 8
  %16 = call signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %15)
  store i8 %16, i8* %14, align 1
  br label %17

; <label>:17:                                     ; preds = %80, %1
  br label %18

; <label>:18:                                     ; preds = %73, %17
  %19 = load i8, i8* %14, align 1
  %20 = sext i8 %19 to i32
  %21 = icmp ne i32 %20, 42
  br i1 %21, label %22, label %75

; <label>:22:                                     ; preds = %18
  %23 = getelementptr inbounds %class.Lexer, %class.Lexer* %15, i32 0, i32 0
  %24 = load i32, i32* %23, align 8
  %25 = sext i32 %24 to i64
  %26 = getelementptr inbounds %class.Lexer, %class.Lexer* %15, i32 0, i32 8
  store %"class.std::__1::basic_string"* %26, %"class.std::__1::basic_string"** %12, align 8
  %27 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8
  store %"class.std::__1::basic_string"* %27, %"class.std::__1::basic_string"** %11, align 8
  %28 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8
  store %"class.std::__1::basic_string"* %28, %"class.std::__1::basic_string"** %10, align 8
  %29 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8
  %30 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %29, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %30, %"class.std::__1::__compressed_pair"** %9, align 8
  %31 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8
  %32 = bitcast %"class.std::__1::__compressed_pair"* %31 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %32, %"class.std::__1::__libcpp_compressed_pair_imp"** %8, align 8
  %33 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %8, align 8
  %34 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %33, i32 0, i32 0
  %35 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %34, i32 0, i32 0
  %36 = bitcast %union.anon.0* %35 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %37 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %36, i32 0, i32 0
  %38 = bitcast %union.anon.1* %37 to i8*
  %39 = load i8, i8* %38, align 8
  %40 = zext i8 %39 to i32
  %41 = and i32 %40, 1
  %42 = icmp ne i32 %41, 0
  br i1 %42, label %43, label %54

; <label>:43:                                     ; preds = %22
  store %"class.std::__1::basic_string"* %28, %"class.std::__1::basic_string"** %4, align 8
  %44 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %4, align 8
  %45 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %44, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %45, %"class.std::__1::__compressed_pair"** %3, align 8
  %46 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %3, align 8
  %47 = bitcast %"class.std::__1::__compressed_pair"* %46 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %47, %"class.std::__1::__libcpp_compressed_pair_imp"** %2, align 8
  %48 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %2, align 8
  %49 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %48, i32 0, i32 0
  %50 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %49, i32 0, i32 0
  %51 = bitcast %union.anon.0* %50 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %52 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %51, i32 0, i32 1
  %53 = load i64, i64* %52, align 8
  br label %69

; <label>:54:                                     ; preds = %22
  store %"class.std::__1::basic_string"* %28, %"class.std::__1::basic_string"** %7, align 8
  %55 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %7, align 8
  %56 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %55, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %56, %"class.std::__1::__compressed_pair"** %6, align 8
  %57 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %6, align 8
  %58 = bitcast %"class.std::__1::__compressed_pair"* %57 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %58, %"class.std::__1::__libcpp_compressed_pair_imp"** %5, align 8
  %59 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %5, align 8
  %60 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %59, i32 0, i32 0
  %61 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %60, i32 0, i32 0
  %62 = bitcast %union.anon.0* %61 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %63 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %62, i32 0, i32 0
  %64 = bitcast %union.anon.1* %63 to i8*
  %65 = load i8, i8* %64, align 8
  %66 = zext i8 %65 to i32
  %67 = ashr i32 %66, 1
  %68 = sext i32 %67 to i64
  br label %69

; <label>:69:                                     ; preds = %43, %54
  %70 = phi i64 [ %53, %43 ], [ %68, %54 ]
  %71 = icmp uge i64 %25, %70
  br i1 %71, label %72, label %73

; <label>:72:                                     ; preds = %69
  br label %81

; <label>:73:                                     ; preds = %69
  %74 = call signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %15)
  store i8 %74, i8* %14, align 1
  br label %18

; <label>:75:                                     ; preds = %18
  %76 = call signext i8 @_ZN5Lexer13get_next_charEv(%class.Lexer* %15)
  store i8 %76, i8* %14, align 1
  %77 = load i8, i8* %14, align 1
  %78 = sext i8 %77 to i32
  %79 = icmp ne i32 %78, 47
  br i1 %79, label %80, label %81

; <label>:80:                                     ; preds = %75
  br label %17

; <label>:81:                                     ; preds = %72, %75
  ret void
}

declare void @_ZN7VariantC1Ec(%struct.Variant*, i8 signext) unnamed_addr #1

declare void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant*, %"class.std::__1::basic_string"*) unnamed_addr #1

declare i64 @_ZNSt3__15stoulERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPmi(%"class.std::__1::basic_string"* dereferenceable(24), i64*, i32) #1

declare void @_ZN7VariantC1El(%struct.Variant*, i64) unnamed_addr #1

declare i32 @_ZNSt3__14stoiERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPmi(%"class.std::__1::basic_string"* dereferenceable(24), i64*, i32) #1

declare void @_ZN7VariantC1Ei(%struct.Variant*, i32) unnamed_addr #1

declare i8* @__cxa_begin_catch(i8*)

declare void @__cxa_end_catch()

; Function Attrs: noinline noreturn nounwind
define linkonce_odr hidden void @__clang_call_terminate(i8*) #6 {
  %2 = call i8* @__cxa_begin_catch(i8* %0) #11
  call void @_ZSt9terminatev() #12
  unreachable
}

declare void @_ZSt9terminatev()

declare double @_ZNSt3__14stodERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPm(%"class.std::__1::basic_string"* dereferenceable(24), i64*) #1

; Function Attrs: nounwind readnone
declare double @llvm.pow.f64(double, double) #7

declare void @_ZN7VariantC1Ed(%struct.Variant*, double) unnamed_addr #1

declare i64 @_ZNSt3__14stolERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPmi(%"class.std::__1::basic_string"* dereferenceable(24), i64*, i32) #1

declare float @_ZNSt3__14stofERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPm(%"class.std::__1::basic_string"* dereferenceable(24), i64*) #1

declare void @_ZN7VariantC1Ef(%struct.Variant*, float) unnamed_addr #1

declare void @_ZN7VariantC1Eb(%struct.Variant*, i1 zeroext) unnamed_addr #1

; Function Attrs: ssp uwtable
define void @_ZN5Lexer7advanceE9TokenTypeb(%class.Lexer*, i32, i1 zeroext) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %6 = alloca %"class.std::__1::basic_string"*, align 8
  %7 = alloca [3 x i64]*, align 8
  %8 = alloca i32, align 4
  %9 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %10 = alloca %"class.std::__1::basic_string"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca %"class.std::__1::basic_string"*, align 8
  %14 = alloca %"class.std::__1::basic_string"*, align 8
  %15 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %16 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %17 = alloca %"class.std::__1::basic_string"*, align 8
  %18 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %19 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %20 = alloca %"class.std::__1::basic_string"*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %23 = alloca %"class.std::__1::basic_string"*, align 8
  %24 = alloca %"class.std::__1::basic_string"*, align 8
  %25 = alloca i8*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %28 = alloca %"class.std::__1::basic_string"*, align 8
  %29 = alloca i8*, align 8
  %30 = alloca i8*, align 8
  %31 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %32 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %33 = alloca %"class.std::__1::basic_string"*, align 8
  %34 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %35 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %36 = alloca %"class.std::__1::basic_string"*, align 8
  %37 = alloca %"class.std::__1::basic_string"*, align 8
  %38 = alloca %"class.std::__1::basic_string"*, align 8
  %39 = alloca %"class.std::__1::basic_string"*, align 8
  %40 = alloca %"class.std::__1::basic_string"*, align 8
  %41 = alloca %"class.std::__1::basic_string"*, align 8
  %42 = alloca %"class.std::__1::basic_string"*, align 8
  %43 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %44 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %45 = alloca %"class.std::__1::basic_string"*, align 8
  %46 = alloca [3 x i64]*, align 8
  %47 = alloca i32, align 4
  %48 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %49 = alloca %"class.std::__1::basic_string"*, align 8
  %50 = alloca %"class.std::__1::basic_string"*, align 8
  %51 = alloca %"class.std::__1::basic_string"*, align 8
  %52 = alloca %"class.std::__1::basic_string"*, align 8
  %53 = alloca %"class.std::__1::basic_string"*, align 8
  %54 = alloca %"class.std::__1::basic_string"*, align 8
  %55 = alloca i8*, align 8
  %56 = alloca %class.Lexer*, align 8
  %57 = alloca i32, align 4
  %58 = alloca i8, align 1
  %59 = alloca %class.Token, align 8
  %60 = alloca i8*
  %61 = alloca i32
  %62 = alloca %"class.std::__1::basic_string", align 8
  %63 = alloca %"class.std::__1::basic_string", align 8
  %64 = alloca %"class.std::__1::basic_string", align 8
  store %class.Lexer* %0, %class.Lexer** %56, align 8
  store i32 %1, i32* %57, align 4
  %65 = zext i1 %2 to i8
  store i8 %65, i8* %58, align 1
  %66 = load %class.Lexer*, %class.Lexer** %56, align 8
  %67 = getelementptr inbounds %class.Lexer, %class.Lexer* %66, i32 0, i32 6
  %68 = load i8, i8* %58, align 1
  %69 = trunc i8 %68 to i1
  call void @_ZN5Lexer14get_next_tokenEb(%class.Token* sret %59, %class.Lexer* %66, i1 zeroext %69)
  %70 = invoke dereferenceable(288) %class.Token* @_ZN5TokenaSEOS_(%class.Token* %67, %class.Token* dereferenceable(288) %59)
          to label %71 unwind label %249

; <label>:71:                                     ; preds = %3
  call void @_ZN5TokenD1Ev(%class.Token* %59) #11
  %72 = getelementptr inbounds %class.Lexer, %class.Lexer* %66, i32 0, i32 6
  %73 = call i32 @_ZN5Token8get_typeEv(%class.Token* %72)
  %74 = load i32, i32* %57, align 4
  %75 = icmp ne i32 %73, %74
  br i1 %75, label %76, label %267

; <label>:76:                                     ; preds = %71
  %77 = load i32, i32* %57, align 4
  %78 = zext i32 %77 to i64
  %79 = getelementptr inbounds [0 x %"class.std::__1::basic_string"], [0 x %"class.std::__1::basic_string"]* @_ZN4util11token_namesE, i64 0, i64 %78
  call void @_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_RKS9_(%"class.std::__1::basic_string"* sret %64, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.15, i32 0, i32 0), %"class.std::__1::basic_string"* dereferenceable(24) %79)
  store %"class.std::__1::basic_string"* %64, %"class.std::__1::basic_string"** %54, align 8, !noalias !5
  store i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.16, i32 0, i32 0), i8** %55, align 8, !noalias !5
  %80 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !5
  %81 = load i8*, i8** %55, align 8, !noalias !5
  %82 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %80, i8* %81)
          to label %83 unwind label %253

; <label>:83:                                     ; preds = %76
  store %"class.std::__1::basic_string"* %82, %"class.std::__1::basic_string"** %53, align 8, !noalias !5
  %84 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %63, %"class.std::__1::basic_string"** %51, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %84, %"class.std::__1::basic_string"** %52, align 8, !noalias !5
  %85 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !5
  %86 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %85, %"class.std::__1::basic_string"** %49, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %86, %"class.std::__1::basic_string"** %50, align 8, !noalias !5
  %87 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !5
  %88 = bitcast %"class.std::__1::basic_string"* %87 to %"class.std::__1::__basic_string_common"*
  %89 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %87, i32 0, i32 0
  %90 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !5
  %91 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %90, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %91, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !5
  %92 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !5
  %93 = bitcast %"class.std::__1::__compressed_pair"* %89 to i8*
  %94 = bitcast %"class.std::__1::__compressed_pair"* %92 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %93, i8* %94, i64 24, i32 8, i1 false) #11
  %95 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %95, %"class.std::__1::basic_string"** %45, align 8, !noalias !5
  %96 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !5
  %97 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %96, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %97, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !5
  %98 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !5
  %99 = bitcast %"class.std::__1::__compressed_pair"* %98 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %99, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !5
  %100 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !5
  %101 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %100, i32 0, i32 0
  %102 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %101, i32 0, i32 0
  %103 = bitcast %union.anon.0* %102 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %104 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %103, i32 0, i32 0
  store [3 x i64]* %104, [3 x i64]** %46, align 8, !noalias !5
  store i32 0, i32* %47, align 4, !noalias !5
  br label %105

; <label>:105:                                    ; preds = %108, %83
  %106 = load i32, i32* %47, align 4, !noalias !5
  %107 = icmp ult i32 %106, 3
  br i1 %107, label %108, label %115

; <label>:108:                                    ; preds = %105
  %109 = load i32, i32* %47, align 4, !noalias !5
  %110 = zext i32 %109 to i64
  %111 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !5
  %112 = getelementptr inbounds [3 x i64], [3 x i64]* %111, i64 0, i64 %110
  store i64 0, i64* %112, align 8
  %113 = load i32, i32* %47, align 4, !noalias !5
  %114 = add i32 %113, 1
  store i32 %114, i32* %47, align 4, !noalias !5
  br label %105

; <label>:115:                                    ; preds = %105
  br label %116

; <label>:116:                                    ; preds = %115
  %117 = getelementptr inbounds %class.Lexer, %class.Lexer* %66, i32 0, i32 6
  %118 = invoke i32 @_ZN5Token8get_typeEv(%class.Token* %117)
          to label %119 unwind label %257

; <label>:119:                                    ; preds = %116
  %120 = zext i32 %118 to i64
  %121 = getelementptr inbounds [0 x %"class.std::__1::basic_string"], [0 x %"class.std::__1::basic_string"]* @_ZN4util11token_namesE, i64 0, i64 %120
  store %"class.std::__1::basic_string"* %63, %"class.std::__1::basic_string"** %41, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %121, %"class.std::__1::basic_string"** %42, align 8, !noalias !8
  %122 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !8
  %123 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %122, %"class.std::__1::basic_string"** %39, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %123, %"class.std::__1::basic_string"** %40, align 8, !noalias !8
  %124 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !8
  %125 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %125, %"class.std::__1::basic_string"** %38, align 8, !noalias !8
  %126 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %126, %"class.std::__1::basic_string"** %37, align 8, !noalias !8
  %127 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %127, %"class.std::__1::basic_string"** %36, align 8, !noalias !8
  %128 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !8
  %129 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %128, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %129, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !8
  %130 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !8
  %131 = bitcast %"class.std::__1::__compressed_pair"* %130 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %131, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !8
  %132 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !8
  %133 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %132, i32 0, i32 0
  %134 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %133, i32 0, i32 0
  %135 = bitcast %union.anon.0* %134 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %136 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %135, i32 0, i32 0
  %137 = bitcast %union.anon.1* %136 to i8*
  %138 = load i8, i8* %137, align 8, !noalias !8
  %139 = zext i8 %138 to i32
  %140 = and i32 %139, 1
  %141 = icmp ne i32 %140, 0
  br i1 %141, label %142, label %153

; <label>:142:                                    ; preds = %119
  store %"class.std::__1::basic_string"* %127, %"class.std::__1::basic_string"** %28, align 8, !noalias !8
  %143 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !8
  %144 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %143, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %144, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !8
  %145 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !8
  %146 = bitcast %"class.std::__1::__compressed_pair"* %145 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %146, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !8
  %147 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !8
  %148 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %147, i32 0, i32 0
  %149 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %148, i32 0, i32 0
  %150 = bitcast %union.anon.0* %149 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %151 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %150, i32 0, i32 2
  %152 = load i8*, i8** %151, align 8
  br label %166

; <label>:153:                                    ; preds = %119
  store %"class.std::__1::basic_string"* %127, %"class.std::__1::basic_string"** %33, align 8, !noalias !8
  %154 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !8
  %155 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %154, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %155, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !8
  %156 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !8
  %157 = bitcast %"class.std::__1::__compressed_pair"* %156 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %157, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !8
  %158 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !8
  %159 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %158, i32 0, i32 0
  %160 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %159, i32 0, i32 0
  %161 = bitcast %union.anon.0* %160 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %162 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %161, i32 0, i32 1
  %163 = getelementptr inbounds [23 x i8], [23 x i8]* %162, i64 0, i64 0
  store i8* %163, i8** %30, align 8, !noalias !8
  %164 = load i8*, i8** %30, align 8, !noalias !8
  store i8* %164, i8** %29, align 8, !noalias !8
  %165 = load i8*, i8** %29, align 8, !noalias !8
  br label %166

; <label>:166:                                    ; preds = %153, %142
  %167 = phi i8* [ %152, %142 ], [ %165, %153 ]
  store i8* %167, i8** %25, align 8, !noalias !8
  %168 = load i8*, i8** %25, align 8, !noalias !8
  %169 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %169, %"class.std::__1::basic_string"** %24, align 8, !noalias !8
  %170 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %170, %"class.std::__1::basic_string"** %23, align 8, !noalias !8
  %171 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !8
  %172 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %171, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %172, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !8
  %173 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !8
  %174 = bitcast %"class.std::__1::__compressed_pair"* %173 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %174, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !8
  %175 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !8
  %176 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %175, i32 0, i32 0
  %177 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %176, i32 0, i32 0
  %178 = bitcast %union.anon.0* %177 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %179 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %178, i32 0, i32 0
  %180 = bitcast %union.anon.1* %179 to i8*
  %181 = load i8, i8* %180, align 8, !noalias !8
  %182 = zext i8 %181 to i32
  %183 = and i32 %182, 1
  %184 = icmp ne i32 %183, 0
  br i1 %184, label %185, label %196

; <label>:185:                                    ; preds = %166
  store %"class.std::__1::basic_string"* %170, %"class.std::__1::basic_string"** %17, align 8, !noalias !8
  %186 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !8
  %187 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %186, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %187, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !8
  %188 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !8
  %189 = bitcast %"class.std::__1::__compressed_pair"* %188 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %189, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !8
  %190 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !8
  %191 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %190, i32 0, i32 0
  %192 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %191, i32 0, i32 0
  %193 = bitcast %union.anon.0* %192 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %194 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %193, i32 0, i32 1
  %195 = load i64, i64* %194, align 8
  br label %211

; <label>:196:                                    ; preds = %166
  store %"class.std::__1::basic_string"* %170, %"class.std::__1::basic_string"** %20, align 8, !noalias !8
  %197 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !8
  %198 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %197, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %198, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !8
  %199 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !8
  %200 = bitcast %"class.std::__1::__compressed_pair"* %199 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %200, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !8
  %201 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !8
  %202 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %201, i32 0, i32 0
  %203 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %202, i32 0, i32 0
  %204 = bitcast %union.anon.0* %203 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %205 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %204, i32 0, i32 0
  %206 = bitcast %union.anon.1* %205 to i8*
  %207 = load i8, i8* %206, align 8
  %208 = zext i8 %207 to i32
  %209 = ashr i32 %208, 1
  %210 = sext i32 %209 to i64
  br label %211

; <label>:211:                                    ; preds = %196, %185
  %212 = phi i64 [ %195, %185 ], [ %210, %196 ]
  %213 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %124, i8* %168, i64 %212)
          to label %214 unwind label %257

; <label>:214:                                    ; preds = %211
  store %"class.std::__1::basic_string"* %213, %"class.std::__1::basic_string"** %14, align 8, !noalias !8
  %215 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %62, %"class.std::__1::basic_string"** %12, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %215, %"class.std::__1::basic_string"** %13, align 8, !noalias !8
  %216 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !8
  %217 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %216, %"class.std::__1::basic_string"** %10, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %217, %"class.std::__1::basic_string"** %11, align 8, !noalias !8
  %218 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !8
  %219 = bitcast %"class.std::__1::basic_string"* %218 to %"class.std::__1::__basic_string_common"*
  %220 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %218, i32 0, i32 0
  %221 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !8
  %222 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %221, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %222, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !8
  %223 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !8
  %224 = bitcast %"class.std::__1::__compressed_pair"* %220 to i8*
  %225 = bitcast %"class.std::__1::__compressed_pair"* %223 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %224, i8* %225, i64 24, i32 8, i1 false) #11
  %226 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %226, %"class.std::__1::basic_string"** %6, align 8, !noalias !8
  %227 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !8
  %228 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %227, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %228, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !8
  %229 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !8
  %230 = bitcast %"class.std::__1::__compressed_pair"* %229 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %230, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !8
  %231 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !8
  %232 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %231, i32 0, i32 0
  %233 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %232, i32 0, i32 0
  %234 = bitcast %union.anon.0* %233 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %235 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %234, i32 0, i32 0
  store [3 x i64]* %235, [3 x i64]** %7, align 8, !noalias !8
  store i32 0, i32* %8, align 4, !noalias !8
  br label %236

; <label>:236:                                    ; preds = %239, %214
  %237 = load i32, i32* %8, align 4, !noalias !8
  %238 = icmp ult i32 %237, 3
  br i1 %238, label %239, label %246

; <label>:239:                                    ; preds = %236
  %240 = load i32, i32* %8, align 4, !noalias !8
  %241 = zext i32 %240 to i64
  %242 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !8
  %243 = getelementptr inbounds [3 x i64], [3 x i64]* %242, i64 0, i64 %241
  store i64 0, i64* %243, align 8
  %244 = load i32, i32* %8, align 4, !noalias !8
  %245 = add i32 %244, 1
  store i32 %245, i32* %8, align 4, !noalias !8
  br label %236

; <label>:246:                                    ; preds = %236
  br label %247

; <label>:247:                                    ; preds = %246
  invoke void @_ZN10ParseError5raiseE11ParseErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP5Lexer(i32 0, %"class.std::__1::basic_string"* %62, %class.Lexer* %66)
          to label %248 unwind label %261

; <label>:248:                                    ; preds = %247
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %62) #11
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %63) #11
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %64) #11
  br label %267

; <label>:249:                                    ; preds = %3
  %250 = landingpad { i8*, i32 }
          cleanup
  %251 = extractvalue { i8*, i32 } %250, 0
  store i8* %251, i8** %60, align 8
  %252 = extractvalue { i8*, i32 } %250, 1
  store i32 %252, i32* %61, align 4
  call void @_ZN5TokenD1Ev(%class.Token* %59) #11
  br label %268

; <label>:253:                                    ; preds = %76
  %254 = landingpad { i8*, i32 }
          cleanup
  %255 = extractvalue { i8*, i32 } %254, 0
  store i8* %255, i8** %60, align 8
  %256 = extractvalue { i8*, i32 } %254, 1
  store i32 %256, i32* %61, align 4
  br label %266

; <label>:257:                                    ; preds = %211, %116
  %258 = landingpad { i8*, i32 }
          cleanup
  %259 = extractvalue { i8*, i32 } %258, 0
  store i8* %259, i8** %60, align 8
  %260 = extractvalue { i8*, i32 } %258, 1
  store i32 %260, i32* %61, align 4
  br label %265

; <label>:261:                                    ; preds = %247
  %262 = landingpad { i8*, i32 }
          cleanup
  %263 = extractvalue { i8*, i32 } %262, 0
  store i8* %263, i8** %60, align 8
  %264 = extractvalue { i8*, i32 } %262, 1
  store i32 %264, i32* %61, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %62) #11
  br label %265

; <label>:265:                                    ; preds = %261, %257
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %63) #11
  br label %266

; <label>:266:                                    ; preds = %265, %253
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %64) #11
  br label %268

; <label>:267:                                    ; preds = %248, %71
  ret void

; <label>:268:                                    ; preds = %266, %249
  %269 = load i8*, i8** %60, align 8
  %270 = load i32, i32* %61, align 4
  %271 = insertvalue { i8*, i32 } undef, i8* %269, 0
  %272 = insertvalue { i8*, i32 } %271, i32 %270, 1
  resume { i8*, i32 } %272
}

; Function Attrs: ssp uwtable
define void @_ZN5Lexer7advanceEb(%class.Lexer*, i1 zeroext) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %class.Lexer*, align 8
  %4 = alloca i8, align 1
  %5 = alloca %class.Token, align 8
  %6 = alloca i8*
  %7 = alloca i32
  store %class.Lexer* %0, %class.Lexer** %3, align 8
  %8 = zext i1 %1 to i8
  store i8 %8, i8* %4, align 1
  %9 = load %class.Lexer*, %class.Lexer** %3, align 8
  %10 = getelementptr inbounds %class.Lexer, %class.Lexer* %9, i32 0, i32 6
  %11 = load i8, i8* %4, align 1
  %12 = trunc i8 %11 to i1
  call void @_ZN5Lexer14get_next_tokenEb(%class.Token* sret %5, %class.Lexer* %9, i1 zeroext %12)
  %13 = invoke dereferenceable(288) %class.Token* @_ZN5TokenaSEOS_(%class.Token* %10, %class.Token* dereferenceable(288) %5)
          to label %14 unwind label %15

; <label>:14:                                     ; preds = %2
  call void @_ZN5TokenD1Ev(%class.Token* %5) #11
  ret void

; <label>:15:                                     ; preds = %2
  %16 = landingpad { i8*, i32 }
          cleanup
  %17 = extractvalue { i8*, i32 } %16, 0
  store i8* %17, i8** %6, align 8
  %18 = extractvalue { i8*, i32 } %16, 1
  store i32 %18, i32* %7, align 4
  call void @_ZN5TokenD1Ev(%class.Token* %5) #11
  br label %19

; <label>:19:                                     ; preds = %15
  %20 = load i8*, i8** %6, align 8
  %21 = load i32, i32* %7, align 4
  %22 = insertvalue { i8*, i32 } undef, i8* %20, 0
  %23 = insertvalue { i8*, i32 } %22, i32 %21, 1
  resume { i8*, i32 } %23
}

; Function Attrs: ssp uwtable
define void @_ZN5Lexer9lookaheadEb(%class.Token* noalias sret, %class.Lexer*, i1 zeroext) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::vector.27"*, align 8
  %5 = alloca i64, align 8
  %6 = alloca %"class.std::__1::vector.27"*, align 8
  %7 = alloca i64, align 8
  %8 = alloca %"class.std::__1::vector.27"*, align 8
  %9 = alloca %"class.std::__1::vector.27"*, align 8
  %10 = alloca %class.Lexer*, align 8
  %11 = alloca i8, align 1
  %12 = alloca %class.Token, align 8
  %13 = alloca i32, align 4
  %14 = alloca i8*
  %15 = alloca i32
  store %class.Lexer* %1, %class.Lexer** %10, align 8
  %16 = zext i1 %2 to i8
  store i8 %16, i8* %11, align 1
  %17 = load %class.Lexer*, %class.Lexer** %10, align 8
  %18 = getelementptr inbounds %class.Lexer, %class.Lexer* %17, i32 0, i32 4
  %19 = load i32, i32* %18, align 8
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds %class.Lexer, %class.Lexer* %17, i32 0, i32 7
  store %"class.std::__1::vector.27"* %21, %"class.std::__1::vector.27"** %9, align 8
  %22 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %9, align 8
  %23 = bitcast %"class.std::__1::vector.27"* %22 to %"class.std::__1::__vector_base.28"*
  %24 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %23, i32 0, i32 1
  %25 = load %class.Token*, %class.Token** %24, align 8
  %26 = bitcast %"class.std::__1::vector.27"* %22 to %"class.std::__1::__vector_base.28"*
  %27 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %26, i32 0, i32 0
  %28 = load %class.Token*, %class.Token** %27, align 8
  %29 = ptrtoint %class.Token* %25 to i64
  %30 = ptrtoint %class.Token* %28 to i64
  %31 = sub i64 %29, %30
  %32 = sdiv exact i64 %31, 288
  %33 = sub i64 %32, 1
  %34 = icmp uge i64 %20, %33
  br i1 %34, label %35, label %42

; <label>:35:                                     ; preds = %3
  %36 = getelementptr inbounds %class.Lexer, %class.Lexer* %17, i32 0, i32 7
  store %"class.std::__1::vector.27"* %36, %"class.std::__1::vector.27"** %8, align 8
  %37 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %8, align 8
  %38 = bitcast %"class.std::__1::vector.27"* %37 to %"class.std::__1::__vector_base.28"*
  %39 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %38, i32 0, i32 1
  %40 = load %class.Token*, %class.Token** %39, align 8
  %41 = getelementptr inbounds %class.Token, %class.Token* %40, i64 -1
  call void @_ZN5TokenC1ERKS_(%class.Token* %0, %class.Token* dereferenceable(288) %41)
  br label %84

; <label>:42:                                     ; preds = %3
  %43 = getelementptr inbounds %class.Lexer, %class.Lexer* %17, i32 0, i32 7
  %44 = getelementptr inbounds %class.Lexer, %class.Lexer* %17, i32 0, i32 4
  %45 = load i32, i32* %44, align 8
  %46 = sext i32 %45 to i64
  store %"class.std::__1::vector.27"* %43, %"class.std::__1::vector.27"** %6, align 8
  store i64 %46, i64* %7, align 8
  %47 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %6, align 8
  %48 = load i64, i64* %7, align 8
  %49 = bitcast %"class.std::__1::vector.27"* %47 to %"class.std::__1::__vector_base.28"*
  %50 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %49, i32 0, i32 0
  %51 = load %class.Token*, %class.Token** %50, align 8
  %52 = getelementptr inbounds %class.Token, %class.Token* %51, i64 %48
  call void @_ZN5TokenC1ERKS_(%class.Token* %12, %class.Token* dereferenceable(288) %52)
  store i32 1, i32* %13, align 4
  br label %53

; <label>:53:                                     ; preds = %77, %42
  %54 = load i8, i8* %11, align 1
  %55 = trunc i8 %54 to i1
  br i1 %55, label %56, label %59

; <label>:56:                                     ; preds = %53
  %57 = invoke zeroext i1 @_ZN5Token13is_punctuatorEc(%class.Token* %12, i8 signext 10)
          to label %58 unwind label %78

; <label>:58:                                     ; preds = %56
  br label %59

; <label>:59:                                     ; preds = %58, %53
  %60 = phi i1 [ false, %53 ], [ %57, %58 ]
  br i1 %60, label %61, label %82

; <label>:61:                                     ; preds = %59
  %62 = getelementptr inbounds %class.Lexer, %class.Lexer* %17, i32 0, i32 7
  %63 = getelementptr inbounds %class.Lexer, %class.Lexer* %17, i32 0, i32 4
  %64 = load i32, i32* %63, align 8
  %65 = load i32, i32* %13, align 4
  %66 = add nsw i32 %65, 1
  store i32 %66, i32* %13, align 4
  %67 = add nsw i32 %64, %65
  %68 = sext i32 %67 to i64
  store %"class.std::__1::vector.27"* %62, %"class.std::__1::vector.27"** %4, align 8
  store i64 %68, i64* %5, align 8
  %69 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %4, align 8
  %70 = load i64, i64* %5, align 8
  %71 = bitcast %"class.std::__1::vector.27"* %69 to %"class.std::__1::__vector_base.28"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %71, i32 0, i32 0
  %73 = load %class.Token*, %class.Token** %72, align 8
  %74 = getelementptr inbounds %class.Token, %class.Token* %73, i64 %70
  br label %75

; <label>:75:                                     ; preds = %61
  %76 = invoke dereferenceable(288) %class.Token* @_ZN5TokenaSERKS_(%class.Token* %12, %class.Token* dereferenceable(288) %74)
          to label %77 unwind label %78

; <label>:77:                                     ; preds = %75
  br label %53

; <label>:78:                                     ; preds = %82, %75, %56
  %79 = landingpad { i8*, i32 }
          cleanup
  %80 = extractvalue { i8*, i32 } %79, 0
  store i8* %80, i8** %14, align 8
  %81 = extractvalue { i8*, i32 } %79, 1
  store i32 %81, i32* %15, align 4
  call void @_ZN5TokenD1Ev(%class.Token* %12) #11
  br label %85

; <label>:82:                                     ; preds = %59
  invoke void @_ZN5TokenC1EOS_(%class.Token* %0, %class.Token* dereferenceable(288) %12)
          to label %83 unwind label %78

; <label>:83:                                     ; preds = %82
  call void @_ZN5TokenD1Ev(%class.Token* %12) #11
  br label %84

; <label>:84:                                     ; preds = %83, %35
  ret void

; <label>:85:                                     ; preds = %78
  %86 = load i8*, i8** %14, align 8
  %87 = load i32, i32* %15, align 4
  %88 = insertvalue { i8*, i32 } undef, i8* %86, 0
  %89 = insertvalue { i8*, i32 } %88, i32 %87, 1
  resume { i8*, i32 } %89
}

; Function Attrs: ssp uwtable
define void @_ZN5Lexer10lookbehindEv(%class.Token* noalias sret, %class.Lexer*) #0 align 2 {
  %3 = alloca %"class.std::__1::vector.27"*, align 8
  %4 = alloca i64, align 8
  %5 = alloca %"class.std::__1::vector.27"*, align 8
  %6 = alloca %class.Lexer*, align 8
  store %class.Lexer* %1, %class.Lexer** %6, align 8
  %7 = load %class.Lexer*, %class.Lexer** %6, align 8
  %8 = getelementptr inbounds %class.Lexer, %class.Lexer* %7, i32 0, i32 4
  %9 = load i32, i32* %8, align 8
  %10 = icmp eq i32 %9, 0
  br i1 %10, label %11, label %17

; <label>:11:                                     ; preds = %2
  %12 = getelementptr inbounds %class.Lexer, %class.Lexer* %7, i32 0, i32 7
  store %"class.std::__1::vector.27"* %12, %"class.std::__1::vector.27"** %5, align 8
  %13 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %5, align 8
  %14 = bitcast %"class.std::__1::vector.27"* %13 to %"class.std::__1::__vector_base.28"*
  %15 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %14, i32 0, i32 0
  %16 = load %class.Token*, %class.Token** %15, align 8
  call void @_ZN5TokenC1ERKS_(%class.Token* %0, %class.Token* dereferenceable(288) %16)
  br label %29

; <label>:17:                                     ; preds = %2
  %18 = getelementptr inbounds %class.Lexer, %class.Lexer* %7, i32 0, i32 7
  %19 = getelementptr inbounds %class.Lexer, %class.Lexer* %7, i32 0, i32 4
  %20 = load i32, i32* %19, align 8
  %21 = sub nsw i32 %20, 1
  %22 = sext i32 %21 to i64
  store %"class.std::__1::vector.27"* %18, %"class.std::__1::vector.27"** %3, align 8
  store i64 %22, i64* %4, align 8
  %23 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %3, align 8
  %24 = load i64, i64* %4, align 8
  %25 = bitcast %"class.std::__1::vector.27"* %23 to %"class.std::__1::__vector_base.28"*
  %26 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %25, i32 0, i32 0
  %27 = load %class.Token*, %class.Token** %26, align 8
  %28 = getelementptr inbounds %class.Token, %class.Token* %27, i64 %24
  call void @_ZN5TokenC1ERKS_(%class.Token* %0, %class.Token* dereferenceable(288) %28)
  br label %29

; <label>:29:                                     ; preds = %17, %11
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN7VariantD2Ev(%struct.Variant*) unnamed_addr #2 align 2 {
  %2 = alloca %struct.Variant*, align 8
  store %struct.Variant* %0, %struct.Variant** %2, align 8
  %3 = load %struct.Variant*, %struct.Variant** %2, align 8
  %4 = getelementptr inbounds %struct.Variant, %struct.Variant* %3, i32 0, i32 4
  call void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier* %4) #11
  %5 = getelementptr inbounds %struct.Variant, %struct.Variant* %3, i32 0, i32 3
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %5) #11
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier*) unnamed_addr #2 align 2 {
  %2 = alloca %struct.TypeSpecifier*, align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %2, align 8
  %3 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %2, align 8
  call void @_ZN13TypeSpecifierD2Ev(%struct.TypeSpecifier* %3) #11
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN13TypeSpecifierD2Ev(%struct.TypeSpecifier*) unnamed_addr #2 align 2 {
  %2 = alloca %struct.TypeSpecifier*, align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %2, align 8
  %3 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %2, align 8
  %4 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 24
  call void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED1Ev(%"class.std::__1::vector.20"* %4) #11
  %5 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 23
  call void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED1Ev(%"class.std::__1::vector"* %5) #11
  %6 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 14
  call void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED1Ev(%"class.std::__1::unordered_map"* %6) #11
  %7 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 11
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %7) #11
  %8 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 2
  call void @_ZNSt3__110shared_ptrI10ExpressionED1Ev(%"class.std::__1::shared_ptr"* %8) #11
  %9 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 1
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %9) #11
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED1Ev(%"class.std::__1::vector.20"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::vector.20"*, align 8
  store %"class.std::__1::vector.20"* %0, %"class.std::__1::vector.20"** %2, align 8
  %3 = load %"class.std::__1::vector.20"*, %"class.std::__1::vector.20"** %2, align 8
  call void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::vector.20"* %3) #11
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED1Ev(%"class.std::__1::vector"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::vector"*, align 8
  store %"class.std::__1::vector"* %0, %"class.std::__1::vector"** %2, align 8
  %3 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %2, align 8
  call void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::vector"* %3) #11
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED1Ev(%"class.std::__1::unordered_map"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::unordered_map"*, align 8
  store %"class.std::__1::unordered_map"* %0, %"class.std::__1::unordered_map"** %2, align 8
  %3 = load %"class.std::__1::unordered_map"*, %"class.std::__1::unordered_map"** %2, align 8
  call void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED2Ev(%"class.std::__1::unordered_map"* %3) #11
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__110shared_ptrI10ExpressionED1Ev(%"class.std::__1::shared_ptr"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.std::__1::shared_ptr"*, align 8
  store %"class.std::__1::shared_ptr"* %0, %"class.std::__1::shared_ptr"** %2, align 8
  %3 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %2, align 8
  call void @_ZNSt3__110shared_ptrI10ExpressionED2Ev(%"class.std::__1::shared_ptr"* %3) #11
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::vector.20"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::vector.20"*, align 8
  store %"class.std::__1::vector.20"* %0, %"class.std::__1::vector.20"** %2, align 8
  %3 = load %"class.std::__1::vector.20"*, %"class.std::__1::vector.20"** %2, align 8
  %4 = bitcast %"class.std::__1::vector.20"* %3 to %"class.std::__1::__vector_base.21"*
  call void @_ZNSt3__113__vector_baseI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::__vector_base.21"* %4) #11
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113__vector_baseI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::__vector_base.21"*) unnamed_addr #5 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::allocator.24"*, align 8
  %3 = alloca %struct.TypeSpecifier*, align 8
  %4 = alloca %"struct.std::__1::integral_constant", align 1
  %5 = alloca %"class.std::__1::allocator.24"*, align 8
  %6 = alloca %struct.TypeSpecifier*, align 8
  %7 = alloca %"class.std::__1::allocator.24"*, align 8
  %8 = alloca %struct.TypeSpecifier*, align 8
  %9 = alloca %"struct.std::__1::integral_constant", align 1
  %10 = alloca %"struct.std::__1::__has_destroy", align 1
  %11 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.23"*, align 8
  %12 = alloca %"class.std::__1::__compressed_pair.22"*, align 8
  %13 = alloca %"class.std::__1::__vector_base.21"*, align 8
  %14 = alloca %struct.TypeSpecifier*, align 8
  %15 = alloca %"class.std::__1::__vector_base.21"*, align 8
  %16 = alloca %struct.TypeSpecifier*, align 8
  %17 = alloca %"class.std::__1::__vector_base.21"*, align 8
  %18 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.23"*, align 8
  %19 = alloca %"class.std::__1::__compressed_pair.22"*, align 8
  %20 = alloca %"class.std::__1::__vector_base.21"*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.23"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.22"*, align 8
  %23 = alloca %"class.std::__1::__vector_base.21"*, align 8
  %24 = alloca %"class.std::__1::__vector_base.21"*, align 8
  %25 = alloca i8*, align 8
  %26 = alloca %"class.std::__1::allocator.24"*, align 8
  %27 = alloca %struct.TypeSpecifier*, align 8
  %28 = alloca i64, align 8
  %29 = alloca %"class.std::__1::allocator.24"*, align 8
  %30 = alloca %struct.TypeSpecifier*, align 8
  %31 = alloca i64, align 8
  %32 = alloca %"class.std::__1::__vector_base.21"*, align 8
  store %"class.std::__1::__vector_base.21"* %0, %"class.std::__1::__vector_base.21"** %32, align 8
  %33 = load %"class.std::__1::__vector_base.21"*, %"class.std::__1::__vector_base.21"** %32, align 8
  %34 = getelementptr inbounds %"class.std::__1::__vector_base.21", %"class.std::__1::__vector_base.21"* %33, i32 0, i32 0
  %35 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %34, align 8
  %36 = icmp ne %struct.TypeSpecifier* %35, null
  br i1 %36, label %37, label %95

; <label>:37:                                     ; preds = %1
  store %"class.std::__1::__vector_base.21"* %33, %"class.std::__1::__vector_base.21"** %17, align 8
  %38 = load %"class.std::__1::__vector_base.21"*, %"class.std::__1::__vector_base.21"** %17, align 8
  %39 = getelementptr inbounds %"class.std::__1::__vector_base.21", %"class.std::__1::__vector_base.21"* %38, i32 0, i32 0
  %40 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %39, align 8
  store %"class.std::__1::__vector_base.21"* %38, %"class.std::__1::__vector_base.21"** %15, align 8
  store %struct.TypeSpecifier* %40, %struct.TypeSpecifier** %16, align 8
  %41 = load %"class.std::__1::__vector_base.21"*, %"class.std::__1::__vector_base.21"** %15, align 8
  br label %42

; <label>:42:                                     ; preds = %47, %37
  %43 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %16, align 8
  %44 = getelementptr inbounds %"class.std::__1::__vector_base.21", %"class.std::__1::__vector_base.21"* %41, i32 0, i32 1
  %45 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %44, align 8
  %46 = icmp ne %struct.TypeSpecifier* %43, %45
  br i1 %46, label %47, label %65

; <label>:47:                                     ; preds = %42
  store %"class.std::__1::__vector_base.21"* %41, %"class.std::__1::__vector_base.21"** %13, align 8
  %48 = load %"class.std::__1::__vector_base.21"*, %"class.std::__1::__vector_base.21"** %13, align 8
  %49 = getelementptr inbounds %"class.std::__1::__vector_base.21", %"class.std::__1::__vector_base.21"* %48, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.22"* %49, %"class.std::__1::__compressed_pair.22"** %12, align 8
  %50 = load %"class.std::__1::__compressed_pair.22"*, %"class.std::__1::__compressed_pair.22"** %12, align 8
  %51 = bitcast %"class.std::__1::__compressed_pair.22"* %50 to %"class.std::__1::__libcpp_compressed_pair_imp.23"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.23"* %51, %"class.std::__1::__libcpp_compressed_pair_imp.23"** %11, align 8
  %52 = load %"class.std::__1::__libcpp_compressed_pair_imp.23"*, %"class.std::__1::__libcpp_compressed_pair_imp.23"** %11, align 8
  %53 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.23"* %52 to %"class.std::__1::allocator.24"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.21", %"class.std::__1::__vector_base.21"* %41, i32 0, i32 1
  %55 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %54, align 8
  %56 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %55, i32 -1
  store %struct.TypeSpecifier* %56, %struct.TypeSpecifier** %54, align 8
  store %struct.TypeSpecifier* %56, %struct.TypeSpecifier** %14, align 8
  %57 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %14, align 8
  store %"class.std::__1::allocator.24"* %53, %"class.std::__1::allocator.24"** %7, align 8
  store %struct.TypeSpecifier* %57, %struct.TypeSpecifier** %8, align 8
  %58 = bitcast %"struct.std::__1::__has_destroy"* %10 to %"struct.std::__1::integral_constant"*
  %59 = load %"class.std::__1::allocator.24"*, %"class.std::__1::allocator.24"** %7, align 8
  %60 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %8, align 8
  store %"class.std::__1::allocator.24"* %59, %"class.std::__1::allocator.24"** %5, align 8
  store %struct.TypeSpecifier* %60, %struct.TypeSpecifier** %6, align 8
  %61 = load %"class.std::__1::allocator.24"*, %"class.std::__1::allocator.24"** %5, align 8
  %62 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %6, align 8
  store %"class.std::__1::allocator.24"* %61, %"class.std::__1::allocator.24"** %2, align 8
  store %struct.TypeSpecifier* %62, %struct.TypeSpecifier** %3, align 8
  %63 = load %"class.std::__1::allocator.24"*, %"class.std::__1::allocator.24"** %2, align 8
  %64 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  call void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier* %64) #11
  br label %42

; <label>:65:                                     ; preds = %42
  store %"class.std::__1::__vector_base.21"* %33, %"class.std::__1::__vector_base.21"** %20, align 8
  %66 = load %"class.std::__1::__vector_base.21"*, %"class.std::__1::__vector_base.21"** %20, align 8
  %67 = getelementptr inbounds %"class.std::__1::__vector_base.21", %"class.std::__1::__vector_base.21"* %66, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.22"* %67, %"class.std::__1::__compressed_pair.22"** %19, align 8
  %68 = load %"class.std::__1::__compressed_pair.22"*, %"class.std::__1::__compressed_pair.22"** %19, align 8
  %69 = bitcast %"class.std::__1::__compressed_pair.22"* %68 to %"class.std::__1::__libcpp_compressed_pair_imp.23"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.23"* %69, %"class.std::__1::__libcpp_compressed_pair_imp.23"** %18, align 8
  %70 = load %"class.std::__1::__libcpp_compressed_pair_imp.23"*, %"class.std::__1::__libcpp_compressed_pair_imp.23"** %18, align 8
  %71 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.23"* %70 to %"class.std::__1::allocator.24"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.21", %"class.std::__1::__vector_base.21"* %33, i32 0, i32 0
  %73 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %72, align 8
  store %"class.std::__1::__vector_base.21"* %33, %"class.std::__1::__vector_base.21"** %24, align 8
  %74 = load %"class.std::__1::__vector_base.21"*, %"class.std::__1::__vector_base.21"** %24, align 8
  store %"class.std::__1::__vector_base.21"* %74, %"class.std::__1::__vector_base.21"** %23, align 8
  %75 = load %"class.std::__1::__vector_base.21"*, %"class.std::__1::__vector_base.21"** %23, align 8
  %76 = getelementptr inbounds %"class.std::__1::__vector_base.21", %"class.std::__1::__vector_base.21"* %75, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.22"* %76, %"class.std::__1::__compressed_pair.22"** %22, align 8
  %77 = load %"class.std::__1::__compressed_pair.22"*, %"class.std::__1::__compressed_pair.22"** %22, align 8
  %78 = bitcast %"class.std::__1::__compressed_pair.22"* %77 to %"class.std::__1::__libcpp_compressed_pair_imp.23"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.23"* %78, %"class.std::__1::__libcpp_compressed_pair_imp.23"** %21, align 8
  %79 = load %"class.std::__1::__libcpp_compressed_pair_imp.23"*, %"class.std::__1::__libcpp_compressed_pair_imp.23"** %21, align 8
  %80 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.23", %"class.std::__1::__libcpp_compressed_pair_imp.23"* %79, i32 0, i32 0
  %81 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %80, align 8
  %82 = getelementptr inbounds %"class.std::__1::__vector_base.21", %"class.std::__1::__vector_base.21"* %74, i32 0, i32 0
  %83 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %82, align 8
  %84 = ptrtoint %struct.TypeSpecifier* %81 to i64
  %85 = ptrtoint %struct.TypeSpecifier* %83 to i64
  %86 = sub i64 %84, %85
  %87 = sdiv exact i64 %86, 216
  store %"class.std::__1::allocator.24"* %71, %"class.std::__1::allocator.24"** %29, align 8
  store %struct.TypeSpecifier* %73, %struct.TypeSpecifier** %30, align 8
  store i64 %87, i64* %31, align 8
  %88 = load %"class.std::__1::allocator.24"*, %"class.std::__1::allocator.24"** %29, align 8
  %89 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %30, align 8
  %90 = load i64, i64* %31, align 8
  store %"class.std::__1::allocator.24"* %88, %"class.std::__1::allocator.24"** %26, align 8
  store %struct.TypeSpecifier* %89, %struct.TypeSpecifier** %27, align 8
  store i64 %90, i64* %28, align 8
  %91 = load %"class.std::__1::allocator.24"*, %"class.std::__1::allocator.24"** %26, align 8
  %92 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %27, align 8
  %93 = bitcast %struct.TypeSpecifier* %92 to i8*
  store i8* %93, i8** %25, align 8
  %94 = load i8*, i8** %25, align 8
  call void @_ZdlPv(i8* %94) #13
  br label %95

; <label>:95:                                     ; preds = %65, %1
  ret void
}

; Function Attrs: nobuiltin nounwind
declare void @_ZdlPv(i8*) #8

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::vector"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::vector"*, align 8
  store %"class.std::__1::vector"* %0, %"class.std::__1::vector"** %2, align 8
  %3 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %2, align 8
  %4 = bitcast %"class.std::__1::vector"* %3 to %"class.std::__1::__vector_base"*
  call void @_ZNSt3__113__vector_baseINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::__vector_base"* %4) #11
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113__vector_baseINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::__vector_base"*) unnamed_addr #5 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.17"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.16"*, align 8
  %4 = alloca %"class.std::__1::__vector_base"*, align 8
  %5 = alloca %"class.std::__1::__vector_base"*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %"class.std::__1::allocator.18"*, align 8
  %8 = alloca %"class.std::__1::basic_string"*, align 8
  %9 = alloca i64, align 8
  %10 = alloca %"class.std::__1::allocator.18"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.17"*, align 8
  %14 = alloca %"class.std::__1::__compressed_pair.16"*, align 8
  %15 = alloca %"class.std::__1::__vector_base"*, align 8
  %16 = alloca %"class.std::__1::allocator.18"*, align 8
  %17 = alloca %"class.std::__1::basic_string"*, align 8
  %18 = alloca %"struct.std::__1::integral_constant", align 1
  %19 = alloca %"class.std::__1::allocator.18"*, align 8
  %20 = alloca %"class.std::__1::basic_string"*, align 8
  %21 = alloca %"class.std::__1::allocator.18"*, align 8
  %22 = alloca %"class.std::__1::basic_string"*, align 8
  %23 = alloca %"struct.std::__1::integral_constant", align 1
  %24 = alloca %"struct.std::__1::__has_destroy.74", align 1
  %25 = alloca %"class.std::__1::basic_string"*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.17"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.16"*, align 8
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
  store %"class.std::__1::__compressed_pair.16"* %49, %"class.std::__1::__compressed_pair.16"** %27, align 8
  %50 = load %"class.std::__1::__compressed_pair.16"*, %"class.std::__1::__compressed_pair.16"** %27, align 8
  %51 = bitcast %"class.std::__1::__compressed_pair.16"* %50 to %"class.std::__1::__libcpp_compressed_pair_imp.17"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.17"* %51, %"class.std::__1::__libcpp_compressed_pair_imp.17"** %26, align 8
  %52 = load %"class.std::__1::__libcpp_compressed_pair_imp.17"*, %"class.std::__1::__libcpp_compressed_pair_imp.17"** %26, align 8
  %53 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.17"* %52 to %"class.std::__1::allocator.18"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %41, i32 0, i32 1
  %55 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8
  %56 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %55, i32 -1
  store %"class.std::__1::basic_string"* %56, %"class.std::__1::basic_string"** %54, align 8
  store %"class.std::__1::basic_string"* %56, %"class.std::__1::basic_string"** %25, align 8
  %57 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %25, align 8
  store %"class.std::__1::allocator.18"* %53, %"class.std::__1::allocator.18"** %21, align 8
  store %"class.std::__1::basic_string"* %57, %"class.std::__1::basic_string"** %22, align 8
  %58 = bitcast %"struct.std::__1::__has_destroy.74"* %24 to %"struct.std::__1::integral_constant"*
  %59 = load %"class.std::__1::allocator.18"*, %"class.std::__1::allocator.18"** %21, align 8
  %60 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %22, align 8
  store %"class.std::__1::allocator.18"* %59, %"class.std::__1::allocator.18"** %19, align 8
  store %"class.std::__1::basic_string"* %60, %"class.std::__1::basic_string"** %20, align 8
  %61 = load %"class.std::__1::allocator.18"*, %"class.std::__1::allocator.18"** %19, align 8
  %62 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8
  store %"class.std::__1::allocator.18"* %61, %"class.std::__1::allocator.18"** %16, align 8
  store %"class.std::__1::basic_string"* %62, %"class.std::__1::basic_string"** %17, align 8
  %63 = load %"class.std::__1::allocator.18"*, %"class.std::__1::allocator.18"** %16, align 8
  %64 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %64) #11
  br label %42

; <label>:65:                                     ; preds = %42
  store %"class.std::__1::__vector_base"* %33, %"class.std::__1::__vector_base"** %15, align 8
  %66 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %15, align 8
  %67 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %66, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.16"* %67, %"class.std::__1::__compressed_pair.16"** %14, align 8
  %68 = load %"class.std::__1::__compressed_pair.16"*, %"class.std::__1::__compressed_pair.16"** %14, align 8
  %69 = bitcast %"class.std::__1::__compressed_pair.16"* %68 to %"class.std::__1::__libcpp_compressed_pair_imp.17"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.17"* %69, %"class.std::__1::__libcpp_compressed_pair_imp.17"** %13, align 8
  %70 = load %"class.std::__1::__libcpp_compressed_pair_imp.17"*, %"class.std::__1::__libcpp_compressed_pair_imp.17"** %13, align 8
  %71 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.17"* %70 to %"class.std::__1::allocator.18"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %33, i32 0, i32 0
  %73 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %72, align 8
  store %"class.std::__1::__vector_base"* %33, %"class.std::__1::__vector_base"** %5, align 8
  %74 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %5, align 8
  store %"class.std::__1::__vector_base"* %74, %"class.std::__1::__vector_base"** %4, align 8
  %75 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %4, align 8
  %76 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %75, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.16"* %76, %"class.std::__1::__compressed_pair.16"** %3, align 8
  %77 = load %"class.std::__1::__compressed_pair.16"*, %"class.std::__1::__compressed_pair.16"** %3, align 8
  %78 = bitcast %"class.std::__1::__compressed_pair.16"* %77 to %"class.std::__1::__libcpp_compressed_pair_imp.17"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.17"* %78, %"class.std::__1::__libcpp_compressed_pair_imp.17"** %2, align 8
  %79 = load %"class.std::__1::__libcpp_compressed_pair_imp.17"*, %"class.std::__1::__libcpp_compressed_pair_imp.17"** %2, align 8
  %80 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.17", %"class.std::__1::__libcpp_compressed_pair_imp.17"* %79, i32 0, i32 0
  %81 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %80, align 8
  %82 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %74, i32 0, i32 0
  %83 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %82, align 8
  %84 = ptrtoint %"class.std::__1::basic_string"* %81 to i64
  %85 = ptrtoint %"class.std::__1::basic_string"* %83 to i64
  %86 = sub i64 %84, %85
  %87 = sdiv exact i64 %86, 24
  store %"class.std::__1::allocator.18"* %71, %"class.std::__1::allocator.18"** %10, align 8
  store %"class.std::__1::basic_string"* %73, %"class.std::__1::basic_string"** %11, align 8
  store i64 %87, i64* %12, align 8
  %88 = load %"class.std::__1::allocator.18"*, %"class.std::__1::allocator.18"** %10, align 8
  %89 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8
  %90 = load i64, i64* %12, align 8
  store %"class.std::__1::allocator.18"* %88, %"class.std::__1::allocator.18"** %7, align 8
  store %"class.std::__1::basic_string"* %89, %"class.std::__1::basic_string"** %8, align 8
  store i64 %90, i64* %9, align 8
  %91 = load %"class.std::__1::allocator.18"*, %"class.std::__1::allocator.18"** %7, align 8
  %92 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %8, align 8
  %93 = bitcast %"class.std::__1::basic_string"* %92 to i8*
  store i8* %93, i8** %6, align 8
  %94 = load i8*, i8** %6, align 8
  call void @_ZdlPv(i8* %94) #13
  br label %95

; <label>:95:                                     ; preds = %65, %1
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED2Ev(%"class.std::__1::unordered_map"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::unordered_map"*, align 8
  store %"class.std::__1::unordered_map"* %0, %"class.std::__1::unordered_map"** %2, align 8
  %3 = load %"class.std::__1::unordered_map"*, %"class.std::__1::unordered_map"** %2, align 8
  %4 = getelementptr inbounds %"class.std::__1::unordered_map", %"class.std::__1::unordered_map"* %3, i32 0, i32 0
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED1Ev(%"class.std::__1::__hash_table"* %4) #11
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED1Ev(%"class.std::__1::__hash_table"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.std::__1::__hash_table"*, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %2, align 8
  %3 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %2, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED2Ev(%"class.std::__1::__hash_table"* %3) #11
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED2Ev(%"class.std::__1::__hash_table"*) unnamed_addr #5 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.5"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair.4"*, align 8
  %8 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca %"class.std::__1::allocator.6"*, align 8
  %11 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::allocator.6"*, align 8
  %14 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %15 = alloca i64, align 8
  %16 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.5"*, align 8
  %17 = alloca %"class.std::__1::__compressed_pair.4"*, align 8
  %18 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %19 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %20 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %23 = alloca %"class.std::__1::unique_ptr"*, align 8
  %24 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %25 = alloca %"class.std::__1::unique_ptr"*, align 8
  %26 = alloca %"class.std::__1::unique_ptr"*, align 8
  %27 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.9"*, align 8
  %28 = alloca %"class.std::__1::__compressed_pair.8"*, align 8
  %29 = alloca %"class.std::__1::__hash_table"*, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %29, align 8
  %30 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %29, align 8
  %31 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %30, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.8"* %31, %"class.std::__1::__compressed_pair.8"** %28, align 8
  %32 = load %"class.std::__1::__compressed_pair.8"*, %"class.std::__1::__compressed_pair.8"** %28, align 8
  %33 = bitcast %"class.std::__1::__compressed_pair.8"* %32 to %"class.std::__1::__libcpp_compressed_pair_imp.9"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.9"* %33, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %27, align 8
  %34 = load %"class.std::__1::__libcpp_compressed_pair_imp.9"*, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %27, align 8
  %35 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.9", %"class.std::__1::__libcpp_compressed_pair_imp.9"* %34, i32 0, i32 0
  %36 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %35, i32 0, i32 0
  %37 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %36, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEE12__deallocateEPNS_16__hash_node_baseIPNS_11__hash_nodeIS9_PvEEEE(%"class.std::__1::__hash_table"* %30, %"struct.std::__1::__hash_node_base"* %37) #11
  %38 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %30, i32 0, i32 0
  store %"class.std::__1::unique_ptr"* %38, %"class.std::__1::unique_ptr"** %26, align 8
  %39 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %26, align 8
  store %"class.std::__1::unique_ptr"* %39, %"class.std::__1::unique_ptr"** %25, align 8
  %40 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %25, align 8
  store %"class.std::__1::unique_ptr"* %40, %"class.std::__1::unique_ptr"** %23, align 8
  %41 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %23, align 8
  %42 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %42, %"class.std::__1::__compressed_pair.2"** %22, align 8
  %43 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %22, align 8
  %44 = bitcast %"class.std::__1::__compressed_pair.2"* %43 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %44, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %21, align 8
  %45 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %21, align 8
  %46 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %45, i32 0, i32 0
  %47 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %46, align 8
  store %"struct.std::__1::__hash_node_base"** %47, %"struct.std::__1::__hash_node_base"*** %24, align 8
  %48 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %48, %"class.std::__1::__compressed_pair.2"** %5, align 8
  %49 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %5, align 8
  %50 = bitcast %"class.std::__1::__compressed_pair.2"* %49 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %50, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %4, align 8
  %51 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %4, align 8
  %52 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %51, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"** null, %"struct.std::__1::__hash_node_base"*** %52, align 8
  %53 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %24, align 8
  %54 = icmp ne %"struct.std::__1::__hash_node_base"** %53, null
  br i1 %54, label %55, label %84

; <label>:55:                                     ; preds = %1
  %56 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %56, %"class.std::__1::__compressed_pair.2"** %3, align 8
  %57 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %3, align 8
  %58 = bitcast %"class.std::__1::__compressed_pair.2"* %57 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %58, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %2, align 8
  %59 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %2, align 8
  %60 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %59, i32 0, i32 1
  %61 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %24, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %60, %"class.std::__1::__bucket_list_deallocator"** %19, align 8
  store %"struct.std::__1::__hash_node_base"** %61, %"struct.std::__1::__hash_node_base"*** %20, align 8
  %62 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %19, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %62, %"class.std::__1::__bucket_list_deallocator"** %18, align 8
  %63 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %18, align 8
  %64 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %63, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.4"* %64, %"class.std::__1::__compressed_pair.4"** %17, align 8
  %65 = load %"class.std::__1::__compressed_pair.4"*, %"class.std::__1::__compressed_pair.4"** %17, align 8
  %66 = bitcast %"class.std::__1::__compressed_pair.4"* %65 to %"class.std::__1::__libcpp_compressed_pair_imp.5"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.5"* %66, %"class.std::__1::__libcpp_compressed_pair_imp.5"** %16, align 8
  %67 = load %"class.std::__1::__libcpp_compressed_pair_imp.5"*, %"class.std::__1::__libcpp_compressed_pair_imp.5"** %16, align 8
  %68 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.5"* %67 to %"class.std::__1::allocator.6"*
  %69 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %20, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %62, %"class.std::__1::__bucket_list_deallocator"** %8, align 8
  %70 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %8, align 8
  %71 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %70, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.4"* %71, %"class.std::__1::__compressed_pair.4"** %7, align 8
  %72 = load %"class.std::__1::__compressed_pair.4"*, %"class.std::__1::__compressed_pair.4"** %7, align 8
  %73 = bitcast %"class.std::__1::__compressed_pair.4"* %72 to %"class.std::__1::__libcpp_compressed_pair_imp.5"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.5"* %73, %"class.std::__1::__libcpp_compressed_pair_imp.5"** %6, align 8
  %74 = load %"class.std::__1::__libcpp_compressed_pair_imp.5"*, %"class.std::__1::__libcpp_compressed_pair_imp.5"** %6, align 8
  %75 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.5", %"class.std::__1::__libcpp_compressed_pair_imp.5"* %74, i32 0, i32 0
  %76 = load i64, i64* %75, align 8
  store %"class.std::__1::allocator.6"* %68, %"class.std::__1::allocator.6"** %13, align 8
  store %"struct.std::__1::__hash_node_base"** %69, %"struct.std::__1::__hash_node_base"*** %14, align 8
  store i64 %76, i64* %15, align 8
  %77 = load %"class.std::__1::allocator.6"*, %"class.std::__1::allocator.6"** %13, align 8
  %78 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %14, align 8
  %79 = load i64, i64* %15, align 8
  store %"class.std::__1::allocator.6"* %77, %"class.std::__1::allocator.6"** %10, align 8
  store %"struct.std::__1::__hash_node_base"** %78, %"struct.std::__1::__hash_node_base"*** %11, align 8
  store i64 %79, i64* %12, align 8
  %80 = load %"class.std::__1::allocator.6"*, %"class.std::__1::allocator.6"** %10, align 8
  %81 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %11, align 8
  %82 = bitcast %"struct.std::__1::__hash_node_base"** %81 to i8*
  store i8* %82, i8** %9, align 8
  %83 = load i8*, i8** %9, align 8
  call void @_ZdlPv(i8* %83) #13
  br label %84

; <label>:84:                                     ; preds = %1, %55
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEE12__deallocateEPNS_16__hash_node_baseIPNS_11__hash_nodeIS9_PvEEEE(%"class.std::__1::__hash_table"*, %"struct.std::__1::__hash_node_base"*) #5 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::allocator.10"*, align 8
  %4 = alloca %"struct.std::__1::pair.75"*, align 8
  %5 = alloca %"class.std::__1::allocator.10"*, align 8
  %6 = alloca %"struct.std::__1::pair.75"*, align 8
  %7 = alloca %"struct.std::__1::integral_constant.77", align 1
  %8 = alloca %"struct.std::__1::__has_destroy.78", align 1
  %9 = alloca i8*, align 8
  %10 = alloca %"class.std::__1::allocator.10"*, align 8
  %11 = alloca %"struct.std::__1::__hash_node"*, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::allocator.10"*, align 8
  %14 = alloca %"struct.std::__1::__hash_node"*, align 8
  %15 = alloca i64, align 8
  %16 = alloca %"struct.std::__1::pair.75"*, align 8
  %17 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %18 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %19 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %20 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.9"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.8"*, align 8
  %23 = alloca %"class.std::__1::__hash_table"*, align 8
  %24 = alloca %"class.std::__1::__hash_table"*, align 8
  %25 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %26 = alloca %"class.std::__1::allocator.10"*, align 8
  %27 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %28 = alloca %"struct.std::__1::__hash_node"*, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %24, align 8
  store %"struct.std::__1::__hash_node_base"* %1, %"struct.std::__1::__hash_node_base"** %25, align 8
  %29 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %24, align 8
  store %"class.std::__1::__hash_table"* %29, %"class.std::__1::__hash_table"** %23, align 8
  %30 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %23, align 8
  %31 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %30, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.8"* %31, %"class.std::__1::__compressed_pair.8"** %22, align 8
  %32 = load %"class.std::__1::__compressed_pair.8"*, %"class.std::__1::__compressed_pair.8"** %22, align 8
  %33 = bitcast %"class.std::__1::__compressed_pair.8"* %32 to %"class.std::__1::__libcpp_compressed_pair_imp.9"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.9"* %33, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %21, align 8
  %34 = load %"class.std::__1::__libcpp_compressed_pair_imp.9"*, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %21, align 8
  %35 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.9"* %34 to %"class.std::__1::allocator.10"*
  store %"class.std::__1::allocator.10"* %35, %"class.std::__1::allocator.10"** %26, align 8
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
  %49 = load %"class.std::__1::allocator.10"*, %"class.std::__1::allocator.10"** %26, align 8
  %50 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %28, align 8
  %51 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %50, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %51, %"union.std::__1::__hash_value_type"** %17, align 8
  %52 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %17, align 8
  %53 = bitcast %"union.std::__1::__hash_value_type"* %52 to %"struct.std::__1::pair.75"*
  store %"struct.std::__1::pair.75"* %53, %"struct.std::__1::pair.75"** %16, align 8
  %54 = load %"struct.std::__1::pair.75"*, %"struct.std::__1::pair.75"** %16, align 8
  %55 = bitcast %"struct.std::__1::pair.75"* %54 to i8*
  br label %56

; <label>:56:                                     ; preds = %39
  store %"class.std::__1::allocator.10"* %49, %"class.std::__1::allocator.10"** %5, align 8
  store %"struct.std::__1::pair.75"* %54, %"struct.std::__1::pair.75"** %6, align 8
  %57 = bitcast %"struct.std::__1::__has_destroy.78"* %8 to %"struct.std::__1::integral_constant.77"*
  %58 = load %"class.std::__1::allocator.10"*, %"class.std::__1::allocator.10"** %5, align 8
  %59 = load %"struct.std::__1::pair.75"*, %"struct.std::__1::pair.75"** %6, align 8
  store %"class.std::__1::allocator.10"* %58, %"class.std::__1::allocator.10"** %3, align 8
  store %"struct.std::__1::pair.75"* %59, %"struct.std::__1::pair.75"** %4, align 8
  %60 = load %"struct.std::__1::pair.75"*, %"struct.std::__1::pair.75"** %4, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED1Ev(%"struct.std::__1::pair.75"* %60) #11
  br label %61

; <label>:61:                                     ; preds = %56
  %62 = load %"class.std::__1::allocator.10"*, %"class.std::__1::allocator.10"** %26, align 8
  %63 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %28, align 8
  store %"class.std::__1::allocator.10"* %62, %"class.std::__1::allocator.10"** %13, align 8
  store %"struct.std::__1::__hash_node"* %63, %"struct.std::__1::__hash_node"** %14, align 8
  store i64 1, i64* %15, align 8
  %64 = load %"class.std::__1::allocator.10"*, %"class.std::__1::allocator.10"** %13, align 8
  %65 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %14, align 8
  %66 = load i64, i64* %15, align 8
  store %"class.std::__1::allocator.10"* %64, %"class.std::__1::allocator.10"** %10, align 8
  store %"struct.std::__1::__hash_node"* %65, %"struct.std::__1::__hash_node"** %11, align 8
  store i64 %66, i64* %12, align 8
  %67 = load %"class.std::__1::allocator.10"*, %"class.std::__1::allocator.10"** %10, align 8
  %68 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %11, align 8
  %69 = bitcast %"struct.std::__1::__hash_node"* %68 to i8*
  store i8* %69, i8** %9, align 8
  %70 = load i8*, i8** %9, align 8
  call void @_ZdlPv(i8* %70) #13
  %71 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %27, align 8
  store %"struct.std::__1::__hash_node_base"* %71, %"struct.std::__1::__hash_node_base"** %25, align 8
  br label %36

; <label>:72:                                     ; preds = %36
  ret void
                                                  ; No predecessors!
  %74 = landingpad { i8*, i32 }
          catch i8* null
  %75 = extractvalue { i8*, i32 } %74, 0
  call void @__clang_call_terminate(i8* %75) #12
  unreachable
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED1Ev(%"struct.std::__1::pair.75"*) unnamed_addr #2 align 2 {
  %2 = alloca %"struct.std::__1::pair.75"*, align 8
  store %"struct.std::__1::pair.75"* %0, %"struct.std::__1::pair.75"** %2, align 8
  %3 = load %"struct.std::__1::pair.75"*, %"struct.std::__1::pair.75"** %2, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED2Ev(%"struct.std::__1::pair.75"* %3) #11
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED2Ev(%"struct.std::__1::pair.75"*) unnamed_addr #2 align 2 {
  %2 = alloca %"struct.std::__1::pair.75"*, align 8
  store %"struct.std::__1::pair.75"* %0, %"struct.std::__1::pair.75"** %2, align 8
  %3 = load %"struct.std::__1::pair.75"*, %"struct.std::__1::pair.75"** %2, align 8
  %4 = getelementptr inbounds %"struct.std::__1::pair.75", %"struct.std::__1::pair.75"* %3, i32 0, i32 1
  call void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier* %4) #11
  %5 = getelementptr inbounds %"struct.std::__1::pair.75", %"struct.std::__1::pair.75"* %3, i32 0, i32 0
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %5) #11
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__110shared_ptrI10ExpressionED2Ev(%"class.std::__1::shared_ptr"*) unnamed_addr #5 align 2 {
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
  call void @_ZNSt3__119__shared_weak_count16__release_sharedEv(%"class.std::__1::__shared_weak_count"* %9) #11
  br label %10

; <label>:10:                                     ; preds = %7, %1
  ret void
}

; Function Attrs: nounwind
declare void @_ZNSt3__119__shared_weak_count16__release_sharedEv(%"class.std::__1::__shared_weak_count"*) #4

declare dereferenceable(264) %struct.Variant* @_ZN7VariantaSERKS_(%struct.Variant*, %struct.Variant* dereferenceable(264)) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #9

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN5TokenD2Ev(%class.Token*) unnamed_addr #2 align 2 {
  %2 = alloca %class.Token*, align 8
  store %class.Token* %0, %class.Token** %2, align 8
  %3 = load %class.Token*, %class.Token** %2, align 8
  %4 = getelementptr inbounds %class.Token, %class.Token* %3, i32 0, i32 0
  call void @_ZN7VariantD1Ev(%struct.Variant* %4) #11
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI5TokenNS_9allocatorIS1_EEED2Ev(%"class.std::__1::vector.27"*) unnamed_addr #2 align 2 {
  %2 = alloca %"class.std::__1::vector.27"*, align 8
  store %"class.std::__1::vector.27"* %0, %"class.std::__1::vector.27"** %2, align 8
  %3 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %2, align 8
  %4 = bitcast %"class.std::__1::vector.27"* %3 to %"class.std::__1::__vector_base.28"*
  call void @_ZNSt3__113__vector_baseI5TokenNS_9allocatorIS1_EEED2Ev(%"class.std::__1::__vector_base.28"* %4) #11
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113__vector_baseI5TokenNS_9allocatorIS1_EEED2Ev(%"class.std::__1::__vector_base.28"*) unnamed_addr #5 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %4 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %5 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %"class.std::__1::allocator.31"*, align 8
  %8 = alloca %class.Token*, align 8
  %9 = alloca i64, align 8
  %10 = alloca %"class.std::__1::allocator.31"*, align 8
  %11 = alloca %class.Token*, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %14 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %15 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %16 = alloca %"class.std::__1::allocator.31"*, align 8
  %17 = alloca %class.Token*, align 8
  %18 = alloca %"struct.std::__1::integral_constant", align 1
  %19 = alloca %"class.std::__1::allocator.31"*, align 8
  %20 = alloca %class.Token*, align 8
  %21 = alloca %"class.std::__1::allocator.31"*, align 8
  %22 = alloca %class.Token*, align 8
  %23 = alloca %"struct.std::__1::integral_constant", align 1
  %24 = alloca %"struct.std::__1::__has_destroy.79", align 1
  %25 = alloca %class.Token*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %28 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %29 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %30 = alloca %class.Token*, align 8
  %31 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %32 = alloca %"class.std::__1::__vector_base.28"*, align 8
  store %"class.std::__1::__vector_base.28"* %0, %"class.std::__1::__vector_base.28"** %32, align 8
  %33 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %32, align 8
  %34 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %33, i32 0, i32 0
  %35 = load %class.Token*, %class.Token** %34, align 8
  %36 = icmp ne %class.Token* %35, null
  br i1 %36, label %37, label %95

; <label>:37:                                     ; preds = %1
  store %"class.std::__1::__vector_base.28"* %33, %"class.std::__1::__vector_base.28"** %31, align 8
  %38 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %31, align 8
  %39 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %38, i32 0, i32 0
  %40 = load %class.Token*, %class.Token** %39, align 8
  store %"class.std::__1::__vector_base.28"* %38, %"class.std::__1::__vector_base.28"** %29, align 8
  store %class.Token* %40, %class.Token** %30, align 8
  %41 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %29, align 8
  br label %42

; <label>:42:                                     ; preds = %47, %37
  %43 = load %class.Token*, %class.Token** %30, align 8
  %44 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %41, i32 0, i32 1
  %45 = load %class.Token*, %class.Token** %44, align 8
  %46 = icmp ne %class.Token* %43, %45
  br i1 %46, label %47, label %65

; <label>:47:                                     ; preds = %42
  store %"class.std::__1::__vector_base.28"* %41, %"class.std::__1::__vector_base.28"** %28, align 8
  %48 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %28, align 8
  %49 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %48, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %49, %"class.std::__1::__compressed_pair.29"** %27, align 8
  %50 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %27, align 8
  %51 = bitcast %"class.std::__1::__compressed_pair.29"* %50 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %51, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %26, align 8
  %52 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %26, align 8
  %53 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.30"* %52 to %"class.std::__1::allocator.31"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %41, i32 0, i32 1
  %55 = load %class.Token*, %class.Token** %54, align 8
  %56 = getelementptr inbounds %class.Token, %class.Token* %55, i32 -1
  store %class.Token* %56, %class.Token** %54, align 8
  store %class.Token* %56, %class.Token** %25, align 8
  %57 = load %class.Token*, %class.Token** %25, align 8
  store %"class.std::__1::allocator.31"* %53, %"class.std::__1::allocator.31"** %21, align 8
  store %class.Token* %57, %class.Token** %22, align 8
  %58 = bitcast %"struct.std::__1::__has_destroy.79"* %24 to %"struct.std::__1::integral_constant"*
  %59 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %21, align 8
  %60 = load %class.Token*, %class.Token** %22, align 8
  store %"class.std::__1::allocator.31"* %59, %"class.std::__1::allocator.31"** %19, align 8
  store %class.Token* %60, %class.Token** %20, align 8
  %61 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %19, align 8
  %62 = load %class.Token*, %class.Token** %20, align 8
  store %"class.std::__1::allocator.31"* %61, %"class.std::__1::allocator.31"** %16, align 8
  store %class.Token* %62, %class.Token** %17, align 8
  %63 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %16, align 8
  %64 = load %class.Token*, %class.Token** %17, align 8
  call void @_ZN5TokenD1Ev(%class.Token* %64) #11
  br label %42

; <label>:65:                                     ; preds = %42
  store %"class.std::__1::__vector_base.28"* %33, %"class.std::__1::__vector_base.28"** %15, align 8
  %66 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %15, align 8
  %67 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %66, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %67, %"class.std::__1::__compressed_pair.29"** %14, align 8
  %68 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %14, align 8
  %69 = bitcast %"class.std::__1::__compressed_pair.29"* %68 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %69, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %13, align 8
  %70 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %13, align 8
  %71 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.30"* %70 to %"class.std::__1::allocator.31"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %33, i32 0, i32 0
  %73 = load %class.Token*, %class.Token** %72, align 8
  store %"class.std::__1::__vector_base.28"* %33, %"class.std::__1::__vector_base.28"** %5, align 8
  %74 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %5, align 8
  store %"class.std::__1::__vector_base.28"* %74, %"class.std::__1::__vector_base.28"** %4, align 8
  %75 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %4, align 8
  %76 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %75, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %76, %"class.std::__1::__compressed_pair.29"** %3, align 8
  %77 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %3, align 8
  %78 = bitcast %"class.std::__1::__compressed_pair.29"* %77 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %78, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %2, align 8
  %79 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %2, align 8
  %80 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.30", %"class.std::__1::__libcpp_compressed_pair_imp.30"* %79, i32 0, i32 0
  %81 = load %class.Token*, %class.Token** %80, align 8
  %82 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %74, i32 0, i32 0
  %83 = load %class.Token*, %class.Token** %82, align 8
  %84 = ptrtoint %class.Token* %81 to i64
  %85 = ptrtoint %class.Token* %83 to i64
  %86 = sub i64 %84, %85
  %87 = sdiv exact i64 %86, 288
  store %"class.std::__1::allocator.31"* %71, %"class.std::__1::allocator.31"** %10, align 8
  store %class.Token* %73, %class.Token** %11, align 8
  store i64 %87, i64* %12, align 8
  %88 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %10, align 8
  %89 = load %class.Token*, %class.Token** %11, align 8
  %90 = load i64, i64* %12, align 8
  store %"class.std::__1::allocator.31"* %88, %"class.std::__1::allocator.31"** %7, align 8
  store %class.Token* %89, %class.Token** %8, align 8
  store i64 %90, i64* %9, align 8
  %91 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %7, align 8
  %92 = load %class.Token*, %class.Token** %8, align 8
  %93 = bitcast %class.Token* %92 to i8*
  store i8* %93, i8** %6, align 8
  %94 = load i8*, i8** %6, align 8
  call void @_ZdlPv(i8* %94) #13
  br label %95

; <label>:95:                                     ; preds = %65, %1
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE9ValueTypeED2Ev(%"struct.std::__1::pair.73"*) unnamed_addr #2 align 2 {
  %2 = alloca %"struct.std::__1::pair.73"*, align 8
  store %"struct.std::__1::pair.73"* %0, %"struct.std::__1::pair.73"** %2, align 8
  %3 = load %"struct.std::__1::pair.73"*, %"struct.std::__1::pair.73"** %2, align 8
  %4 = getelementptr inbounds %"struct.std::__1::pair.73", %"struct.std::__1::pair.73"* %3, i32 0, i32 0
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %4) #11
  ret void
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZN5TokenC2ERKS_(%class.Token*, %class.Token* dereferenceable(288)) unnamed_addr #3 align 2 {
  %3 = alloca %class.Token*, align 8
  %4 = alloca %class.Token*, align 8
  store %class.Token* %0, %class.Token** %3, align 8
  store %class.Token* %1, %class.Token** %4, align 8
  %5 = load %class.Token*, %class.Token** %3, align 8
  %6 = getelementptr inbounds %class.Token, %class.Token* %5, i32 0, i32 0
  %7 = load %class.Token*, %class.Token** %4, align 8
  %8 = getelementptr inbounds %class.Token, %class.Token* %7, i32 0, i32 0
  call void @_ZN7VariantC1ERKS_(%struct.Variant* %6, %struct.Variant* dereferenceable(264) %8)
  %9 = getelementptr inbounds %class.Token, %class.Token* %5, i32 0, i32 1
  %10 = load %class.Token*, %class.Token** %4, align 8
  %11 = getelementptr inbounds %class.Token, %class.Token* %10, i32 0, i32 1
  %12 = bitcast i32* %9 to i8*
  %13 = bitcast i32* %11 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %12, i8* %13, i64 24, i32 8, i1 false)
  ret void
}

declare void @_ZN7VariantC1ERKS_(%struct.Variant*, %struct.Variant* dereferenceable(264)) unnamed_addr #1

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZN5TokenC2EOS_(%class.Token*, %class.Token* dereferenceable(288)) unnamed_addr #3 align 2 {
  %3 = alloca %class.Token*, align 8
  %4 = alloca %class.Token*, align 8
  store %class.Token* %0, %class.Token** %3, align 8
  store %class.Token* %1, %class.Token** %4, align 8
  %5 = load %class.Token*, %class.Token** %3, align 8
  %6 = getelementptr inbounds %class.Token, %class.Token* %5, i32 0, i32 0
  %7 = load %class.Token*, %class.Token** %4, align 8
  %8 = getelementptr inbounds %class.Token, %class.Token* %7, i32 0, i32 0
  call void @_ZN7VariantC1ERKS_(%struct.Variant* %6, %struct.Variant* dereferenceable(264) %8)
  %9 = getelementptr inbounds %class.Token, %class.Token* %5, i32 0, i32 1
  %10 = load %class.Token*, %class.Token** %4, align 8
  %11 = getelementptr inbounds %class.Token, %class.Token* %10, i32 0, i32 1
  %12 = bitcast i32* %9 to i8*
  %13 = bitcast i32* %11 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %12, i8* %13, i64 24, i32 8, i1 false)
  ret void
}

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEmc(%"class.std::__1::basic_string"*, i64, i8 signext) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #9

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"*, i8*, i64) #1

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8*) #3 align 2 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = load i8*, i8** %2, align 8
  %4 = call i64 @strlen(i8* %3)
  ret i64 %4
}

declare i64 @strlen(i8*) #1

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI5TokenNS_9allocatorIS1_EEE24__RAII_IncreaseAnnotatorC1ERKS4_m(%"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"*, %"class.std::__1::vector.27"* dereferenceable(24), i64) unnamed_addr #3 align 2 {
  %4 = alloca %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"*, align 8
  %5 = alloca %"class.std::__1::vector.27"*, align 8
  %6 = alloca i64, align 8
  store %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"* %0, %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"** %4, align 8
  store %"class.std::__1::vector.27"* %1, %"class.std::__1::vector.27"** %5, align 8
  store i64 %2, i64* %6, align 8
  %7 = load %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"*, %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"** %4, align 8
  %8 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %5, align 8
  %9 = load i64, i64* %6, align 8
  call void @_ZNSt3__16vectorI5TokenNS_9allocatorIS1_EEE24__RAII_IncreaseAnnotatorC2ERKS4_m(%"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"* %7, %"class.std::__1::vector.27"* dereferenceable(24) %8, i64 %9)
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI5TokenNS_9allocatorIS1_EEE24__RAII_IncreaseAnnotator6__doneEv(%"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"*) #2 align 2 {
  %2 = alloca %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"*, align 8
  store %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"* %0, %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"** %2, align 8
  %3 = load %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"*, %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"** %2, align 8
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI5TokenNS_9allocatorIS1_EEE21__push_back_slow_pathIRKS1_EEvOT_(%"class.std::__1::vector.27"*, %class.Token* dereferenceable(288)) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %class.Token*, align 8
  %4 = alloca %"class.std::__1::allocator.31"*, align 8
  %5 = alloca %class.Token*, align 8
  %6 = alloca %class.Token*, align 8
  %7 = alloca %class.Token*, align 8
  %8 = alloca %"struct.std::__1::integral_constant", align 1
  %9 = alloca %"class.std::__1::allocator.31"*, align 8
  %10 = alloca %class.Token*, align 8
  %11 = alloca %class.Token*, align 8
  %12 = alloca %class.Token*, align 8
  %13 = alloca %"class.std::__1::allocator.31"*, align 8
  %14 = alloca %class.Token*, align 8
  %15 = alloca %class.Token*, align 8
  %16 = alloca %"struct.std::__1::integral_constant", align 1
  %17 = alloca %"struct.std::__1::__has_construct", align 1
  %18 = alloca %class.Token*, align 8
  %19 = alloca %class.Token*, align 8
  %20 = alloca %"class.std::__1::vector.27"*, align 8
  %21 = alloca %"struct.std::__1::__less"*, align 8
  %22 = alloca i64*, align 8
  %23 = alloca i64*, align 8
  %24 = alloca %"struct.std::__1::__less", align 1
  %25 = alloca i64*, align 8
  %26 = alloca i64*, align 8
  %27 = alloca i64*, align 8
  %28 = alloca i64*, align 8
  %29 = alloca %"struct.std::__1::__less", align 1
  %30 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %31 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %32 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %33 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %34 = alloca %"class.std::__1::vector.27"*, align 8
  %35 = alloca i64, align 8
  %36 = alloca %"class.std::__1::vector.27"*, align 8
  %37 = alloca i64, align 8
  %38 = alloca i64, align 8
  %39 = alloca i64, align 8
  %40 = alloca i64, align 8
  %41 = alloca %"class.std::__1::vector.27"*, align 8
  %42 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %43 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %44 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %45 = alloca %"class.std::__1::vector.27"*, align 8
  %46 = alloca %class.Token*, align 8
  %47 = alloca %"class.std::__1::allocator.31"*, align 8
  %48 = alloca %"struct.std::__1::__split_buffer", align 8
  %49 = alloca i8*
  %50 = alloca i32
  store %"class.std::__1::vector.27"* %0, %"class.std::__1::vector.27"** %45, align 8
  store %class.Token* %1, %class.Token** %46, align 8
  %51 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %45, align 8
  %52 = bitcast %"class.std::__1::vector.27"* %51 to %"class.std::__1::__vector_base.28"*
  store %"class.std::__1::__vector_base.28"* %52, %"class.std::__1::__vector_base.28"** %44, align 8
  %53 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %44, align 8
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %53, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %54, %"class.std::__1::__compressed_pair.29"** %43, align 8
  %55 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %43, align 8
  %56 = bitcast %"class.std::__1::__compressed_pair.29"* %55 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %56, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %42, align 8
  %57 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %42, align 8
  %58 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.30"* %57 to %"class.std::__1::allocator.31"*
  store %"class.std::__1::allocator.31"* %58, %"class.std::__1::allocator.31"** %47, align 8
  store %"class.std::__1::vector.27"* %51, %"class.std::__1::vector.27"** %41, align 8
  %59 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %41, align 8
  %60 = bitcast %"class.std::__1::vector.27"* %59 to %"class.std::__1::__vector_base.28"*
  %61 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %60, i32 0, i32 1
  %62 = load %class.Token*, %class.Token** %61, align 8
  %63 = bitcast %"class.std::__1::vector.27"* %59 to %"class.std::__1::__vector_base.28"*
  %64 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %63, i32 0, i32 0
  %65 = load %class.Token*, %class.Token** %64, align 8
  %66 = ptrtoint %class.Token* %62 to i64
  %67 = ptrtoint %class.Token* %65 to i64
  %68 = sub i64 %66, %67
  %69 = sdiv exact i64 %68, 288
  %70 = add i64 %69, 1
  store %"class.std::__1::vector.27"* %51, %"class.std::__1::vector.27"** %36, align 8
  store i64 %70, i64* %37, align 8
  %71 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %36, align 8
  %72 = call i64 @_ZNKSt3__16vectorI5TokenNS_9allocatorIS1_EEE8max_sizeEv(%"class.std::__1::vector.27"* %71) #11
  store i64 %72, i64* %38, align 8
  %73 = load i64, i64* %37, align 8
  %74 = load i64, i64* %38, align 8
  %75 = icmp ugt i64 %73, %74
  br i1 %75, label %76, label %78

; <label>:76:                                     ; preds = %2
  %77 = bitcast %"class.std::__1::vector.27"* %71 to %"class.std::__1::__vector_base_common"*
  call void @_ZNKSt3__120__vector_base_commonILb1EE20__throw_length_errorEv(%"class.std::__1::__vector_base_common"* %77)
  br label %78

; <label>:78:                                     ; preds = %76, %2
  store %"class.std::__1::vector.27"* %71, %"class.std::__1::vector.27"** %34, align 8
  %79 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %34, align 8
  %80 = bitcast %"class.std::__1::vector.27"* %79 to %"class.std::__1::__vector_base.28"*
  store %"class.std::__1::__vector_base.28"* %80, %"class.std::__1::__vector_base.28"** %33, align 8
  %81 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %33, align 8
  store %"class.std::__1::__vector_base.28"* %81, %"class.std::__1::__vector_base.28"** %32, align 8
  %82 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %32, align 8
  %83 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %82, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %83, %"class.std::__1::__compressed_pair.29"** %31, align 8
  %84 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %31, align 8
  %85 = bitcast %"class.std::__1::__compressed_pair.29"* %84 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %85, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %30, align 8
  %86 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %30, align 8
  %87 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.30", %"class.std::__1::__libcpp_compressed_pair_imp.30"* %86, i32 0, i32 0
  %88 = load %class.Token*, %class.Token** %87, align 8
  %89 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %81, i32 0, i32 0
  %90 = load %class.Token*, %class.Token** %89, align 8
  %91 = ptrtoint %class.Token* %88 to i64
  %92 = ptrtoint %class.Token* %90 to i64
  %93 = sub i64 %91, %92
  %94 = sdiv exact i64 %93, 288
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
  store %"class.std::__1::vector.27"* %51, %"class.std::__1::vector.27"** %20, align 8
  %123 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %20, align 8
  %124 = bitcast %"class.std::__1::vector.27"* %123 to %"class.std::__1::__vector_base.28"*
  %125 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %124, i32 0, i32 1
  %126 = load %class.Token*, %class.Token** %125, align 8
  %127 = bitcast %"class.std::__1::vector.27"* %123 to %"class.std::__1::__vector_base.28"*
  %128 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %127, i32 0, i32 0
  %129 = load %class.Token*, %class.Token** %128, align 8
  %130 = ptrtoint %class.Token* %126 to i64
  %131 = ptrtoint %class.Token* %129 to i64
  %132 = sub i64 %130, %131
  %133 = sdiv exact i64 %132, 288
  %134 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %47, align 8
  call void @_ZNSt3__114__split_bufferI5TokenRNS_9allocatorIS1_EEEC1EmmS4_(%"struct.std::__1::__split_buffer"* %48, i64 %122, i64 %133, %"class.std::__1::allocator.31"* dereferenceable(1) %134)
  %135 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %47, align 8
  %136 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %48, i32 0, i32 2
  %137 = load %class.Token*, %class.Token** %136, align 8
  store %class.Token* %137, %class.Token** %19, align 8
  %138 = load %class.Token*, %class.Token** %19, align 8
  %139 = load %class.Token*, %class.Token** %46, align 8
  store %class.Token* %139, %class.Token** %18, align 8
  %140 = load %class.Token*, %class.Token** %18, align 8
  store %"class.std::__1::allocator.31"* %135, %"class.std::__1::allocator.31"** %13, align 8
  store %class.Token* %138, %class.Token** %14, align 8
  store %class.Token* %140, %class.Token** %15, align 8
  %141 = bitcast %"struct.std::__1::__has_construct"* %17 to %"struct.std::__1::integral_constant"*
  %142 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %13, align 8
  %143 = load %class.Token*, %class.Token** %14, align 8
  %144 = load %class.Token*, %class.Token** %15, align 8
  store %class.Token* %144, %class.Token** %12, align 8
  %145 = load %class.Token*, %class.Token** %12, align 8
  store %"class.std::__1::allocator.31"* %142, %"class.std::__1::allocator.31"** %9, align 8
  store %class.Token* %143, %class.Token** %10, align 8
  store %class.Token* %145, %class.Token** %11, align 8
  %146 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %9, align 8
  %147 = load %class.Token*, %class.Token** %10, align 8
  %148 = load %class.Token*, %class.Token** %11, align 8
  store %class.Token* %148, %class.Token** %7, align 8
  %149 = load %class.Token*, %class.Token** %7, align 8
  store %"class.std::__1::allocator.31"* %146, %"class.std::__1::allocator.31"** %4, align 8
  store %class.Token* %147, %class.Token** %5, align 8
  store %class.Token* %149, %class.Token** %6, align 8
  %150 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %4, align 8
  %151 = load %class.Token*, %class.Token** %5, align 8
  %152 = bitcast %class.Token* %151 to i8*
  %153 = load %class.Token*, %class.Token** %6, align 8
  store %class.Token* %153, %class.Token** %3, align 8
  %154 = load %class.Token*, %class.Token** %3, align 8
  invoke void @_ZN5TokenC1ERKS_(%class.Token* %151, %class.Token* dereferenceable(288) %154)
          to label %155 unwind label %161

; <label>:155:                                    ; preds = %121
  br label %156

; <label>:156:                                    ; preds = %155
  %157 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %48, i32 0, i32 2
  %158 = load %class.Token*, %class.Token** %157, align 8
  %159 = getelementptr inbounds %class.Token, %class.Token* %158, i32 1
  store %class.Token* %159, %class.Token** %157, align 8
  invoke void @_ZNSt3__16vectorI5TokenNS_9allocatorIS1_EEE26__swap_out_circular_bufferERNS_14__split_bufferIS1_RS3_EE(%"class.std::__1::vector.27"* %51, %"struct.std::__1::__split_buffer"* dereferenceable(40) %48)
          to label %160 unwind label %161

; <label>:160:                                    ; preds = %156
  call void @_ZNSt3__114__split_bufferI5TokenRNS_9allocatorIS1_EEED1Ev(%"struct.std::__1::__split_buffer"* %48) #11
  ret void

; <label>:161:                                    ; preds = %121, %156
  %162 = landingpad { i8*, i32 }
          cleanup
  %163 = extractvalue { i8*, i32 } %162, 0
  store i8* %163, i8** %49, align 8
  %164 = extractvalue { i8*, i32 } %162, 1
  store i32 %164, i32* %50, align 4
  call void @_ZNSt3__114__split_bufferI5TokenRNS_9allocatorIS1_EEED1Ev(%"struct.std::__1::__split_buffer"* %48) #11
  br label %165

; <label>:165:                                    ; preds = %161
  %166 = load i8*, i8** %49, align 8
  %167 = load i32, i32* %50, align 4
  %168 = insertvalue { i8*, i32 } undef, i8* %166, 0
  %169 = insertvalue { i8*, i32 } %168, i32 %167, 1
  resume { i8*, i32 } %169
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI5TokenNS_9allocatorIS1_EEE24__RAII_IncreaseAnnotatorC2ERKS4_m(%"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"*, %"class.std::__1::vector.27"* dereferenceable(24), i64) unnamed_addr #2 align 2 {
  %4 = alloca %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"*, align 8
  %5 = alloca %"class.std::__1::vector.27"*, align 8
  %6 = alloca i64, align 8
  store %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"* %0, %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"** %4, align 8
  store %"class.std::__1::vector.27"* %1, %"class.std::__1::vector.27"** %5, align 8
  store i64 %2, i64* %6, align 8
  %7 = load %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"*, %"struct.std::__1::vector<Token, std::__1::allocator<Token> >::__RAII_IncreaseAnnotator"** %4, align 8
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__114__split_bufferI5TokenRNS_9allocatorIS1_EEEC1EmmS4_(%"struct.std::__1::__split_buffer"*, i64, i64, %"class.std::__1::allocator.31"* dereferenceable(1)) unnamed_addr #0 align 2 {
  %5 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  %8 = alloca %"class.std::__1::allocator.31"*, align 8
  store %"struct.std::__1::__split_buffer"* %0, %"struct.std::__1::__split_buffer"** %5, align 8
  store i64 %1, i64* %6, align 8
  store i64 %2, i64* %7, align 8
  store %"class.std::__1::allocator.31"* %3, %"class.std::__1::allocator.31"** %8, align 8
  %9 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %5, align 8
  %10 = load i64, i64* %6, align 8
  %11 = load i64, i64* %7, align 8
  %12 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %8, align 8
  call void @_ZNSt3__114__split_bufferI5TokenRNS_9allocatorIS1_EEEC2EmmS4_(%"struct.std::__1::__split_buffer"* %9, i64 %10, i64 %11, %"class.std::__1::allocator.31"* dereferenceable(1) %12)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI5TokenNS_9allocatorIS1_EEE26__swap_out_circular_bufferERNS_14__split_bufferIS1_RS3_EE(%"class.std::__1::vector.27"*, %"struct.std::__1::__split_buffer"* dereferenceable(40)) #0 align 2 {
  %3 = alloca %class.Token**, align 8
  %4 = alloca %class.Token**, align 8
  %5 = alloca %class.Token**, align 8
  %6 = alloca %class.Token**, align 8
  %7 = alloca %class.Token**, align 8
  %8 = alloca %class.Token*, align 8
  %9 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %11 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %14 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %15 = alloca %class.Token**, align 8
  %16 = alloca %class.Token**, align 8
  %17 = alloca %class.Token**, align 8
  %18 = alloca %class.Token**, align 8
  %19 = alloca %class.Token**, align 8
  %20 = alloca %class.Token*, align 8
  %21 = alloca %"class.std::__1::vector.27"*, align 8
  %22 = alloca %class.Token**, align 8
  %23 = alloca %class.Token**, align 8
  %24 = alloca %class.Token**, align 8
  %25 = alloca %class.Token**, align 8
  %26 = alloca %class.Token**, align 8
  %27 = alloca %class.Token*, align 8
  %28 = alloca %class.Token*, align 8
  %29 = alloca %class.Token*, align 8
  %30 = alloca %class.Token*, align 8
  %31 = alloca %"class.std::__1::allocator.31"*, align 8
  %32 = alloca %class.Token*, align 8
  %33 = alloca %class.Token*, align 8
  %34 = alloca %class.Token*, align 8
  %35 = alloca %"struct.std::__1::integral_constant", align 1
  %36 = alloca %"class.std::__1::allocator.31"*, align 8
  %37 = alloca %class.Token*, align 8
  %38 = alloca %class.Token*, align 8
  %39 = alloca %class.Token*, align 8
  %40 = alloca %"class.std::__1::allocator.31"*, align 8
  %41 = alloca %class.Token*, align 8
  %42 = alloca %class.Token*, align 8
  %43 = alloca %"struct.std::__1::integral_constant", align 1
  %44 = alloca %"struct.std::__1::__has_construct", align 1
  %45 = alloca %class.Token*, align 8
  %46 = alloca %"class.std::__1::allocator.31"*, align 8
  %47 = alloca %class.Token*, align 8
  %48 = alloca %class.Token*, align 8
  %49 = alloca %class.Token**, align 8
  %50 = alloca %"class.std::__1::vector.27"*, align 8
  %51 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %52 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %53 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %54 = alloca %"class.std::__1::vector.27"*, align 8
  %55 = alloca %"struct.std::__1::__split_buffer"*, align 8
  store %"class.std::__1::vector.27"* %0, %"class.std::__1::vector.27"** %54, align 8
  store %"struct.std::__1::__split_buffer"* %1, %"struct.std::__1::__split_buffer"** %55, align 8
  %56 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %54, align 8
  call void @_ZNKSt3__16vectorI5TokenNS_9allocatorIS1_EEE17__annotate_deleteEv(%"class.std::__1::vector.27"* %56)
  %57 = bitcast %"class.std::__1::vector.27"* %56 to %"class.std::__1::__vector_base.28"*
  store %"class.std::__1::__vector_base.28"* %57, %"class.std::__1::__vector_base.28"** %53, align 8
  %58 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %53, align 8
  %59 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %58, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %59, %"class.std::__1::__compressed_pair.29"** %52, align 8
  %60 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %52, align 8
  %61 = bitcast %"class.std::__1::__compressed_pair.29"* %60 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %61, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %51, align 8
  %62 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %51, align 8
  %63 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.30"* %62 to %"class.std::__1::allocator.31"*
  %64 = bitcast %"class.std::__1::vector.27"* %56 to %"class.std::__1::__vector_base.28"*
  %65 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %64, i32 0, i32 0
  %66 = load %class.Token*, %class.Token** %65, align 8
  %67 = bitcast %"class.std::__1::vector.27"* %56 to %"class.std::__1::__vector_base.28"*
  %68 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %67, i32 0, i32 1
  %69 = load %class.Token*, %class.Token** %68, align 8
  %70 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %55, align 8
  %71 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %70, i32 0, i32 1
  store %"class.std::__1::allocator.31"* %63, %"class.std::__1::allocator.31"** %46, align 8
  store %class.Token* %66, %class.Token** %47, align 8
  store %class.Token* %69, %class.Token** %48, align 8
  store %class.Token** %71, %class.Token*** %49, align 8
  br label %72

; <label>:72:                                     ; preds = %76, %2
  %73 = load %class.Token*, %class.Token** %48, align 8
  %74 = load %class.Token*, %class.Token** %47, align 8
  %75 = icmp ne %class.Token* %73, %74
  br i1 %75, label %76, label %103

; <label>:76:                                     ; preds = %72
  %77 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %46, align 8
  %78 = load %class.Token**, %class.Token*** %49, align 8
  %79 = load %class.Token*, %class.Token** %78, align 8
  %80 = getelementptr inbounds %class.Token, %class.Token* %79, i64 -1
  store %class.Token* %80, %class.Token** %45, align 8
  %81 = load %class.Token*, %class.Token** %45, align 8
  %82 = load %class.Token*, %class.Token** %48, align 8
  %83 = getelementptr inbounds %class.Token, %class.Token* %82, i32 -1
  store %class.Token* %83, %class.Token** %48, align 8
  store %class.Token* %83, %class.Token** %29, align 8
  %84 = load %class.Token*, %class.Token** %29, align 8
  store %class.Token* %84, %class.Token** %28, align 8
  %85 = load %class.Token*, %class.Token** %28, align 8
  store %"class.std::__1::allocator.31"* %77, %"class.std::__1::allocator.31"** %40, align 8
  store %class.Token* %81, %class.Token** %41, align 8
  store %class.Token* %85, %class.Token** %42, align 8
  %86 = bitcast %"struct.std::__1::__has_construct"* %44 to %"struct.std::__1::integral_constant"*
  %87 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %40, align 8
  %88 = load %class.Token*, %class.Token** %41, align 8
  %89 = load %class.Token*, %class.Token** %42, align 8
  store %class.Token* %89, %class.Token** %39, align 8
  %90 = load %class.Token*, %class.Token** %39, align 8
  store %"class.std::__1::allocator.31"* %87, %"class.std::__1::allocator.31"** %36, align 8
  store %class.Token* %88, %class.Token** %37, align 8
  store %class.Token* %90, %class.Token** %38, align 8
  %91 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %36, align 8
  %92 = load %class.Token*, %class.Token** %37, align 8
  %93 = load %class.Token*, %class.Token** %38, align 8
  store %class.Token* %93, %class.Token** %34, align 8
  %94 = load %class.Token*, %class.Token** %34, align 8
  store %"class.std::__1::allocator.31"* %91, %"class.std::__1::allocator.31"** %31, align 8
  store %class.Token* %92, %class.Token** %32, align 8
  store %class.Token* %94, %class.Token** %33, align 8
  %95 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %31, align 8
  %96 = load %class.Token*, %class.Token** %32, align 8
  %97 = bitcast %class.Token* %96 to i8*
  %98 = load %class.Token*, %class.Token** %33, align 8
  store %class.Token* %98, %class.Token** %30, align 8
  %99 = load %class.Token*, %class.Token** %30, align 8
  call void @_ZN5TokenC1ERKS_(%class.Token* %96, %class.Token* dereferenceable(288) %99)
  %100 = load %class.Token**, %class.Token*** %49, align 8
  %101 = load %class.Token*, %class.Token** %100, align 8
  %102 = getelementptr inbounds %class.Token, %class.Token* %101, i32 -1
  store %class.Token* %102, %class.Token** %100, align 8
  br label %72

; <label>:103:                                    ; preds = %72
  %104 = bitcast %"class.std::__1::vector.27"* %56 to %"class.std::__1::__vector_base.28"*
  %105 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %104, i32 0, i32 0
  %106 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %55, align 8
  %107 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %106, i32 0, i32 1
  store %class.Token** %105, %class.Token*** %25, align 8
  store %class.Token** %107, %class.Token*** %26, align 8
  %108 = load %class.Token**, %class.Token*** %25, align 8
  store %class.Token** %108, %class.Token*** %24, align 8
  %109 = load %class.Token**, %class.Token*** %24, align 8
  %110 = load %class.Token*, %class.Token** %109, align 8
  store %class.Token* %110, %class.Token** %27, align 8
  %111 = load %class.Token**, %class.Token*** %26, align 8
  store %class.Token** %111, %class.Token*** %22, align 8
  %112 = load %class.Token**, %class.Token*** %22, align 8
  %113 = load %class.Token*, %class.Token** %112, align 8
  %114 = load %class.Token**, %class.Token*** %25, align 8
  store %class.Token* %113, %class.Token** %114, align 8
  store %class.Token** %27, %class.Token*** %23, align 8
  %115 = load %class.Token**, %class.Token*** %23, align 8
  %116 = load %class.Token*, %class.Token** %115, align 8
  %117 = load %class.Token**, %class.Token*** %26, align 8
  store %class.Token* %116, %class.Token** %117, align 8
  %118 = bitcast %"class.std::__1::vector.27"* %56 to %"class.std::__1::__vector_base.28"*
  %119 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %118, i32 0, i32 1
  %120 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %55, align 8
  %121 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %120, i32 0, i32 2
  store %class.Token** %119, %class.Token*** %6, align 8
  store %class.Token** %121, %class.Token*** %7, align 8
  %122 = load %class.Token**, %class.Token*** %6, align 8
  store %class.Token** %122, %class.Token*** %5, align 8
  %123 = load %class.Token**, %class.Token*** %5, align 8
  %124 = load %class.Token*, %class.Token** %123, align 8
  store %class.Token* %124, %class.Token** %8, align 8
  %125 = load %class.Token**, %class.Token*** %7, align 8
  store %class.Token** %125, %class.Token*** %3, align 8
  %126 = load %class.Token**, %class.Token*** %3, align 8
  %127 = load %class.Token*, %class.Token** %126, align 8
  %128 = load %class.Token**, %class.Token*** %6, align 8
  store %class.Token* %127, %class.Token** %128, align 8
  store %class.Token** %8, %class.Token*** %4, align 8
  %129 = load %class.Token**, %class.Token*** %4, align 8
  %130 = load %class.Token*, %class.Token** %129, align 8
  %131 = load %class.Token**, %class.Token*** %7, align 8
  store %class.Token* %130, %class.Token** %131, align 8
  %132 = bitcast %"class.std::__1::vector.27"* %56 to %"class.std::__1::__vector_base.28"*
  store %"class.std::__1::__vector_base.28"* %132, %"class.std::__1::__vector_base.28"** %11, align 8
  %133 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %11, align 8
  %134 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %133, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %134, %"class.std::__1::__compressed_pair.29"** %10, align 8
  %135 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %10, align 8
  %136 = bitcast %"class.std::__1::__compressed_pair.29"* %135 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %136, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %9, align 8
  %137 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %9, align 8
  %138 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.30", %"class.std::__1::__libcpp_compressed_pair_imp.30"* %137, i32 0, i32 0
  %139 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %55, align 8
  store %"struct.std::__1::__split_buffer"* %139, %"struct.std::__1::__split_buffer"** %14, align 8
  %140 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %14, align 8
  %141 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %140, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.80"* %141, %"class.std::__1::__compressed_pair.80"** %13, align 8
  %142 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %13, align 8
  %143 = bitcast %"class.std::__1::__compressed_pair.80"* %142 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %143, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %12, align 8
  %144 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %12, align 8
  %145 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %144, i32 0, i32 0
  store %class.Token** %138, %class.Token*** %18, align 8
  store %class.Token** %145, %class.Token*** %19, align 8
  %146 = load %class.Token**, %class.Token*** %18, align 8
  store %class.Token** %146, %class.Token*** %17, align 8
  %147 = load %class.Token**, %class.Token*** %17, align 8
  %148 = load %class.Token*, %class.Token** %147, align 8
  store %class.Token* %148, %class.Token** %20, align 8
  %149 = load %class.Token**, %class.Token*** %19, align 8
  store %class.Token** %149, %class.Token*** %15, align 8
  %150 = load %class.Token**, %class.Token*** %15, align 8
  %151 = load %class.Token*, %class.Token** %150, align 8
  %152 = load %class.Token**, %class.Token*** %18, align 8
  store %class.Token* %151, %class.Token** %152, align 8
  store %class.Token** %20, %class.Token*** %16, align 8
  %153 = load %class.Token**, %class.Token*** %16, align 8
  %154 = load %class.Token*, %class.Token** %153, align 8
  %155 = load %class.Token**, %class.Token*** %19, align 8
  store %class.Token* %154, %class.Token** %155, align 8
  %156 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %55, align 8
  %157 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %156, i32 0, i32 1
  %158 = load %class.Token*, %class.Token** %157, align 8
  %159 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %55, align 8
  %160 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %159, i32 0, i32 0
  store %class.Token* %158, %class.Token** %160, align 8
  store %"class.std::__1::vector.27"* %56, %"class.std::__1::vector.27"** %21, align 8
  %161 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %21, align 8
  %162 = bitcast %"class.std::__1::vector.27"* %161 to %"class.std::__1::__vector_base.28"*
  %163 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %162, i32 0, i32 1
  %164 = load %class.Token*, %class.Token** %163, align 8
  %165 = bitcast %"class.std::__1::vector.27"* %161 to %"class.std::__1::__vector_base.28"*
  %166 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %165, i32 0, i32 0
  %167 = load %class.Token*, %class.Token** %166, align 8
  %168 = ptrtoint %class.Token* %164 to i64
  %169 = ptrtoint %class.Token* %167 to i64
  %170 = sub i64 %168, %169
  %171 = sdiv exact i64 %170, 288
  call void @_ZNKSt3__16vectorI5TokenNS_9allocatorIS1_EEE14__annotate_newEm(%"class.std::__1::vector.27"* %56, i64 %171)
  store %"class.std::__1::vector.27"* %56, %"class.std::__1::vector.27"** %50, align 8
  %172 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %50, align 8
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__114__split_bufferI5TokenRNS_9allocatorIS1_EEED1Ev(%"struct.std::__1::__split_buffer"*) unnamed_addr #5 align 2 {
  %2 = alloca %"struct.std::__1::__split_buffer"*, align 8
  store %"struct.std::__1::__split_buffer"* %0, %"struct.std::__1::__split_buffer"** %2, align 8
  %3 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %2, align 8
  call void @_ZNSt3__114__split_bufferI5TokenRNS_9allocatorIS1_EEED2Ev(%"struct.std::__1::__split_buffer"* %3) #11
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr i64 @_ZNKSt3__16vectorI5TokenNS_9allocatorIS1_EEE8max_sizeEv(%"class.std::__1::vector.27"*) #5 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"struct.std::__1::__less"*, align 8
  %3 = alloca i64*, align 8
  %4 = alloca i64*, align 8
  %5 = alloca %"struct.std::__1::__less", align 1
  %6 = alloca i64*, align 8
  %7 = alloca i64*, align 8
  %8 = alloca i64*, align 8
  %9 = alloca i64*, align 8
  %10 = alloca %"struct.std::__1::__less", align 1
  %11 = alloca %"class.std::__1::allocator.31"*, align 8
  %12 = alloca %"struct.std::__1::integral_constant", align 1
  %13 = alloca %"class.std::__1::allocator.31"*, align 8
  %14 = alloca %"class.std::__1::allocator.31"*, align 8
  %15 = alloca %"struct.std::__1::integral_constant", align 1
  %16 = alloca %"struct.std::__1::__has_max_size", align 1
  %17 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %18 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %19 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %20 = alloca %"class.std::__1::vector.27"*, align 8
  %21 = alloca i64, align 8
  %22 = alloca i64, align 8
  store %"class.std::__1::vector.27"* %0, %"class.std::__1::vector.27"** %20, align 8
  %23 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %20, align 8
  %24 = bitcast %"class.std::__1::vector.27"* %23 to %"class.std::__1::__vector_base.28"*
  store %"class.std::__1::__vector_base.28"* %24, %"class.std::__1::__vector_base.28"** %19, align 8
  %25 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %19, align 8
  %26 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %25, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %26, %"class.std::__1::__compressed_pair.29"** %18, align 8
  %27 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %18, align 8
  %28 = bitcast %"class.std::__1::__compressed_pair.29"* %27 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %28, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %17, align 8
  %29 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %17, align 8
  %30 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.30"* %29 to %"class.std::__1::allocator.31"*
  store %"class.std::__1::allocator.31"* %30, %"class.std::__1::allocator.31"** %14, align 8
  %31 = bitcast %"struct.std::__1::__has_max_size"* %16 to %"struct.std::__1::integral_constant"*
  %32 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %14, align 8
  store %"class.std::__1::allocator.31"* %32, %"class.std::__1::allocator.31"** %13, align 8
  %33 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %13, align 8
  store %"class.std::__1::allocator.31"* %33, %"class.std::__1::allocator.31"** %11, align 8
  %34 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %11, align 8
  store i64 64051194700380387, i64* %21, align 8
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
  call void @__clang_call_terminate(i8* %56) #12
  unreachable
}

declare void @_ZNKSt3__120__vector_base_commonILb1EE20__throw_length_errorEv(%"class.std::__1::__vector_base_common"*) #1

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__114__split_bufferI5TokenRNS_9allocatorIS1_EEEC2EmmS4_(%"struct.std::__1::__split_buffer"*, i64, i64, %"class.std::__1::allocator.31"* dereferenceable(1)) unnamed_addr #0 align 2 {
  %5 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %6 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %7 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %8 = alloca i64, align 8
  %9 = alloca %"class.std::__1::allocator.31"*, align 8
  %10 = alloca i64, align 8
  %11 = alloca i8*, align 8
  %12 = alloca %"class.std::__1::allocator.31"*, align 8
  %13 = alloca i64, align 8
  %14 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %15 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %16 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %17 = alloca %"class.std::__1::allocator.31"*, align 8
  %18 = alloca %"class.std::__1::allocator.31"*, align 8
  %19 = alloca %class.Token**, align 8
  %20 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %21 = alloca %class.Token*, align 8
  %22 = alloca %"class.std::__1::allocator.31"*, align 8
  %23 = alloca %class.Token**, align 8
  %24 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %25 = alloca %class.Token*, align 8
  %26 = alloca %"class.std::__1::allocator.31"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %28 = alloca %class.Token*, align 8
  %29 = alloca %"class.std::__1::allocator.31"*, align 8
  %30 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %31 = alloca i64, align 8
  %32 = alloca i64, align 8
  %33 = alloca %"class.std::__1::allocator.31"*, align 8
  store %"struct.std::__1::__split_buffer"* %0, %"struct.std::__1::__split_buffer"** %30, align 8
  store i64 %1, i64* %31, align 8
  store i64 %2, i64* %32, align 8
  store %"class.std::__1::allocator.31"* %3, %"class.std::__1::allocator.31"** %33, align 8
  %34 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %30, align 8
  %35 = bitcast %"struct.std::__1::__split_buffer"* %34 to %"class.std::__1::__split_buffer_common"*
  %36 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %34, i32 0, i32 3
  %37 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %33, align 8
  store %"class.std::__1::__compressed_pair.80"* %36, %"class.std::__1::__compressed_pair.80"** %27, align 8
  store %class.Token* null, %class.Token** %28, align 8
  store %"class.std::__1::allocator.31"* %37, %"class.std::__1::allocator.31"** %29, align 8
  %38 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %27, align 8
  %39 = load %class.Token*, %class.Token** %28, align 8
  %40 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %29, align 8
  store %"class.std::__1::__compressed_pair.80"* %38, %"class.std::__1::__compressed_pair.80"** %24, align 8
  store %class.Token* %39, %class.Token** %25, align 8
  store %"class.std::__1::allocator.31"* %40, %"class.std::__1::allocator.31"** %26, align 8
  %41 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %24, align 8
  %42 = bitcast %"class.std::__1::__compressed_pair.80"* %41 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %class.Token** %25, %class.Token*** %23, align 8
  %43 = load %class.Token**, %class.Token*** %23, align 8
  %44 = load %class.Token*, %class.Token** %43, align 8
  %45 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %26, align 8
  store %"class.std::__1::allocator.31"* %45, %"class.std::__1::allocator.31"** %17, align 8
  %46 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %17, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %42, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %20, align 8
  store %class.Token* %44, %class.Token** %21, align 8
  store %"class.std::__1::allocator.31"* %46, %"class.std::__1::allocator.31"** %22, align 8
  %47 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %20, align 8
  %48 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %47, i32 0, i32 0
  store %class.Token** %21, %class.Token*** %19, align 8
  %49 = load %class.Token**, %class.Token*** %19, align 8
  %50 = load %class.Token*, %class.Token** %49, align 8
  store %class.Token* %50, %class.Token** %48, align 8
  %51 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %47, i32 0, i32 1
  %52 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %22, align 8
  store %"class.std::__1::allocator.31"* %52, %"class.std::__1::allocator.31"** %18, align 8
  %53 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %18, align 8
  store %"class.std::__1::allocator.31"* %53, %"class.std::__1::allocator.31"** %51, align 8
  %54 = load i64, i64* %31, align 8
  %55 = icmp ne i64 %54, 0
  br i1 %55, label %56, label %73

; <label>:56:                                     ; preds = %4
  store %"struct.std::__1::__split_buffer"* %34, %"struct.std::__1::__split_buffer"** %7, align 8
  %57 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %7, align 8
  %58 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %57, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.80"* %58, %"class.std::__1::__compressed_pair.80"** %6, align 8
  %59 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %6, align 8
  %60 = bitcast %"class.std::__1::__compressed_pair.80"* %59 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %60, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %5, align 8
  %61 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %5, align 8
  %62 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %61, i32 0, i32 1
  %63 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %62, align 8
  %64 = load i64, i64* %31, align 8
  store %"class.std::__1::allocator.31"* %63, %"class.std::__1::allocator.31"** %12, align 8
  store i64 %64, i64* %13, align 8
  %65 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %12, align 8
  %66 = load i64, i64* %13, align 8
  store %"class.std::__1::allocator.31"* %65, %"class.std::__1::allocator.31"** %9, align 8
  store i64 %66, i64* %10, align 8
  store i8* null, i8** %11, align 8
  %67 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %9, align 8
  %68 = load i64, i64* %10, align 8
  %69 = mul i64 %68, 288
  store i64 %69, i64* %8, align 8
  %70 = load i64, i64* %8, align 8
  %71 = call i8* @_Znwm(i64 %70) #14
  %72 = bitcast i8* %71 to %class.Token*
  br label %74

; <label>:73:                                     ; preds = %4
  br label %74

; <label>:74:                                     ; preds = %73, %56
  %75 = phi %class.Token* [ %72, %56 ], [ null, %73 ]
  %76 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %34, i32 0, i32 0
  store %class.Token* %75, %class.Token** %76, align 8
  %77 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %34, i32 0, i32 0
  %78 = load %class.Token*, %class.Token** %77, align 8
  %79 = load i64, i64* %32, align 8
  %80 = getelementptr inbounds %class.Token, %class.Token* %78, i64 %79
  %81 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %34, i32 0, i32 2
  store %class.Token* %80, %class.Token** %81, align 8
  %82 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %34, i32 0, i32 1
  store %class.Token* %80, %class.Token** %82, align 8
  %83 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %34, i32 0, i32 0
  %84 = load %class.Token*, %class.Token** %83, align 8
  %85 = load i64, i64* %31, align 8
  %86 = getelementptr inbounds %class.Token, %class.Token* %84, i64 %85
  store %"struct.std::__1::__split_buffer"* %34, %"struct.std::__1::__split_buffer"** %16, align 8
  %87 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %16, align 8
  %88 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %87, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.80"* %88, %"class.std::__1::__compressed_pair.80"** %15, align 8
  %89 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %15, align 8
  %90 = bitcast %"class.std::__1::__compressed_pair.80"* %89 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %90, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %14, align 8
  %91 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %14, align 8
  %92 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %91, i32 0, i32 0
  store %class.Token* %86, %class.Token** %92, align 8
  ret void
}

; Function Attrs: nobuiltin
declare noalias i8* @_Znwm(i64) #10

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNKSt3__16vectorI5TokenNS_9allocatorIS1_EEE17__annotate_deleteEv(%"class.std::__1::vector.27"*) #0 align 2 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %4 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %5 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %6 = alloca %"class.std::__1::vector.27"*, align 8
  %7 = alloca %class.Token*, align 8
  %8 = alloca %"class.std::__1::vector.27"*, align 8
  %9 = alloca %"class.std::__1::vector.27"*, align 8
  %10 = alloca %class.Token*, align 8
  %11 = alloca %"class.std::__1::vector.27"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %14 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %15 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %16 = alloca %"class.std::__1::vector.27"*, align 8
  %17 = alloca %class.Token*, align 8
  %18 = alloca %"class.std::__1::vector.27"*, align 8
  %19 = alloca %class.Token*, align 8
  %20 = alloca %"class.std::__1::vector.27"*, align 8
  %21 = alloca %"class.std::__1::vector.27"*, align 8
  store %"class.std::__1::vector.27"* %0, %"class.std::__1::vector.27"** %21, align 8
  %22 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %21, align 8
  store %"class.std::__1::vector.27"* %22, %"class.std::__1::vector.27"** %20, align 8
  %23 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %20, align 8
  %24 = bitcast %"class.std::__1::vector.27"* %23 to %"class.std::__1::__vector_base.28"*
  %25 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %24, i32 0, i32 0
  %26 = load %class.Token*, %class.Token** %25, align 8
  store %class.Token* %26, %class.Token** %19, align 8
  %27 = load %class.Token*, %class.Token** %19, align 8
  %28 = bitcast %class.Token* %27 to i8*
  store %"class.std::__1::vector.27"* %22, %"class.std::__1::vector.27"** %18, align 8
  %29 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %18, align 8
  %30 = bitcast %"class.std::__1::vector.27"* %29 to %"class.std::__1::__vector_base.28"*
  %31 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %30, i32 0, i32 0
  %32 = load %class.Token*, %class.Token** %31, align 8
  store %class.Token* %32, %class.Token** %17, align 8
  %33 = load %class.Token*, %class.Token** %17, align 8
  store %"class.std::__1::vector.27"* %22, %"class.std::__1::vector.27"** %6, align 8
  %34 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %6, align 8
  %35 = bitcast %"class.std::__1::vector.27"* %34 to %"class.std::__1::__vector_base.28"*
  store %"class.std::__1::__vector_base.28"* %35, %"class.std::__1::__vector_base.28"** %5, align 8
  %36 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %5, align 8
  store %"class.std::__1::__vector_base.28"* %36, %"class.std::__1::__vector_base.28"** %4, align 8
  %37 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %4, align 8
  %38 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %37, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %38, %"class.std::__1::__compressed_pair.29"** %3, align 8
  %39 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %3, align 8
  %40 = bitcast %"class.std::__1::__compressed_pair.29"* %39 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %40, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %2, align 8
  %41 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %2, align 8
  %42 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.30", %"class.std::__1::__libcpp_compressed_pair_imp.30"* %41, i32 0, i32 0
  %43 = load %class.Token*, %class.Token** %42, align 8
  %44 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %36, i32 0, i32 0
  %45 = load %class.Token*, %class.Token** %44, align 8
  %46 = ptrtoint %class.Token* %43 to i64
  %47 = ptrtoint %class.Token* %45 to i64
  %48 = sub i64 %46, %47
  %49 = sdiv exact i64 %48, 288
  %50 = getelementptr inbounds %class.Token, %class.Token* %33, i64 %49
  %51 = bitcast %class.Token* %50 to i8*
  store %"class.std::__1::vector.27"* %22, %"class.std::__1::vector.27"** %8, align 8
  %52 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %8, align 8
  %53 = bitcast %"class.std::__1::vector.27"* %52 to %"class.std::__1::__vector_base.28"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %53, i32 0, i32 0
  %55 = load %class.Token*, %class.Token** %54, align 8
  store %class.Token* %55, %class.Token** %7, align 8
  %56 = load %class.Token*, %class.Token** %7, align 8
  store %"class.std::__1::vector.27"* %22, %"class.std::__1::vector.27"** %9, align 8
  %57 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %9, align 8
  %58 = bitcast %"class.std::__1::vector.27"* %57 to %"class.std::__1::__vector_base.28"*
  %59 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %58, i32 0, i32 1
  %60 = load %class.Token*, %class.Token** %59, align 8
  %61 = bitcast %"class.std::__1::vector.27"* %57 to %"class.std::__1::__vector_base.28"*
  %62 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %61, i32 0, i32 0
  %63 = load %class.Token*, %class.Token** %62, align 8
  %64 = ptrtoint %class.Token* %60 to i64
  %65 = ptrtoint %class.Token* %63 to i64
  %66 = sub i64 %64, %65
  %67 = sdiv exact i64 %66, 288
  %68 = getelementptr inbounds %class.Token, %class.Token* %56, i64 %67
  %69 = bitcast %class.Token* %68 to i8*
  store %"class.std::__1::vector.27"* %22, %"class.std::__1::vector.27"** %11, align 8
  %70 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %11, align 8
  %71 = bitcast %"class.std::__1::vector.27"* %70 to %"class.std::__1::__vector_base.28"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %71, i32 0, i32 0
  %73 = load %class.Token*, %class.Token** %72, align 8
  store %class.Token* %73, %class.Token** %10, align 8
  %74 = load %class.Token*, %class.Token** %10, align 8
  store %"class.std::__1::vector.27"* %22, %"class.std::__1::vector.27"** %16, align 8
  %75 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %16, align 8
  %76 = bitcast %"class.std::__1::vector.27"* %75 to %"class.std::__1::__vector_base.28"*
  store %"class.std::__1::__vector_base.28"* %76, %"class.std::__1::__vector_base.28"** %15, align 8
  %77 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %15, align 8
  store %"class.std::__1::__vector_base.28"* %77, %"class.std::__1::__vector_base.28"** %14, align 8
  %78 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %14, align 8
  %79 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %78, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %79, %"class.std::__1::__compressed_pair.29"** %13, align 8
  %80 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %13, align 8
  %81 = bitcast %"class.std::__1::__compressed_pair.29"* %80 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %81, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %12, align 8
  %82 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %12, align 8
  %83 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.30", %"class.std::__1::__libcpp_compressed_pair_imp.30"* %82, i32 0, i32 0
  %84 = load %class.Token*, %class.Token** %83, align 8
  %85 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %77, i32 0, i32 0
  %86 = load %class.Token*, %class.Token** %85, align 8
  %87 = ptrtoint %class.Token* %84 to i64
  %88 = ptrtoint %class.Token* %86 to i64
  %89 = sub i64 %87, %88
  %90 = sdiv exact i64 %89, 288
  %91 = getelementptr inbounds %class.Token, %class.Token* %74, i64 %90
  %92 = bitcast %class.Token* %91 to i8*
  call void @_ZNKSt3__16vectorI5TokenNS_9allocatorIS1_EEE31__annotate_contiguous_containerEPKvS6_S6_S6_(%"class.std::__1::vector.27"* %22, i8* %28, i8* %51, i8* %69, i8* %92)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNKSt3__16vectorI5TokenNS_9allocatorIS1_EEE14__annotate_newEm(%"class.std::__1::vector.27"*, i64) #5 align 2 {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %5 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %6 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %7 = alloca %"class.std::__1::vector.27"*, align 8
  %8 = alloca %class.Token*, align 8
  %9 = alloca %"class.std::__1::vector.27"*, align 8
  %10 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %11 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %12 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %13 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %14 = alloca %"class.std::__1::vector.27"*, align 8
  %15 = alloca %class.Token*, align 8
  %16 = alloca %"class.std::__1::vector.27"*, align 8
  %17 = alloca %class.Token*, align 8
  %18 = alloca %"class.std::__1::vector.27"*, align 8
  %19 = alloca %class.Token*, align 8
  %20 = alloca %"class.std::__1::vector.27"*, align 8
  %21 = alloca %"class.std::__1::vector.27"*, align 8
  %22 = alloca i64, align 8
  store %"class.std::__1::vector.27"* %0, %"class.std::__1::vector.27"** %21, align 8
  store i64 %1, i64* %22, align 8
  %23 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %21, align 8
  store %"class.std::__1::vector.27"* %23, %"class.std::__1::vector.27"** %20, align 8
  %24 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %20, align 8
  %25 = bitcast %"class.std::__1::vector.27"* %24 to %"class.std::__1::__vector_base.28"*
  %26 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %25, i32 0, i32 0
  %27 = load %class.Token*, %class.Token** %26, align 8
  store %class.Token* %27, %class.Token** %19, align 8
  %28 = load %class.Token*, %class.Token** %19, align 8
  %29 = bitcast %class.Token* %28 to i8*
  store %"class.std::__1::vector.27"* %23, %"class.std::__1::vector.27"** %18, align 8
  %30 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %18, align 8
  %31 = bitcast %"class.std::__1::vector.27"* %30 to %"class.std::__1::__vector_base.28"*
  %32 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %31, i32 0, i32 0
  %33 = load %class.Token*, %class.Token** %32, align 8
  store %class.Token* %33, %class.Token** %17, align 8
  %34 = load %class.Token*, %class.Token** %17, align 8
  store %"class.std::__1::vector.27"* %23, %"class.std::__1::vector.27"** %7, align 8
  %35 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %7, align 8
  %36 = bitcast %"class.std::__1::vector.27"* %35 to %"class.std::__1::__vector_base.28"*
  store %"class.std::__1::__vector_base.28"* %36, %"class.std::__1::__vector_base.28"** %6, align 8
  %37 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %6, align 8
  store %"class.std::__1::__vector_base.28"* %37, %"class.std::__1::__vector_base.28"** %5, align 8
  %38 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %5, align 8
  %39 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %38, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %39, %"class.std::__1::__compressed_pair.29"** %4, align 8
  %40 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %4, align 8
  %41 = bitcast %"class.std::__1::__compressed_pair.29"* %40 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %41, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %3, align 8
  %42 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %3, align 8
  %43 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.30", %"class.std::__1::__libcpp_compressed_pair_imp.30"* %42, i32 0, i32 0
  %44 = load %class.Token*, %class.Token** %43, align 8
  %45 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %37, i32 0, i32 0
  %46 = load %class.Token*, %class.Token** %45, align 8
  %47 = ptrtoint %class.Token* %44 to i64
  %48 = ptrtoint %class.Token* %46 to i64
  %49 = sub i64 %47, %48
  %50 = sdiv exact i64 %49, 288
  %51 = getelementptr inbounds %class.Token, %class.Token* %34, i64 %50
  %52 = bitcast %class.Token* %51 to i8*
  store %"class.std::__1::vector.27"* %23, %"class.std::__1::vector.27"** %9, align 8
  %53 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %9, align 8
  %54 = bitcast %"class.std::__1::vector.27"* %53 to %"class.std::__1::__vector_base.28"*
  %55 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %54, i32 0, i32 0
  %56 = load %class.Token*, %class.Token** %55, align 8
  store %class.Token* %56, %class.Token** %8, align 8
  %57 = load %class.Token*, %class.Token** %8, align 8
  store %"class.std::__1::vector.27"* %23, %"class.std::__1::vector.27"** %14, align 8
  %58 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %14, align 8
  %59 = bitcast %"class.std::__1::vector.27"* %58 to %"class.std::__1::__vector_base.28"*
  store %"class.std::__1::__vector_base.28"* %59, %"class.std::__1::__vector_base.28"** %13, align 8
  %60 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %13, align 8
  store %"class.std::__1::__vector_base.28"* %60, %"class.std::__1::__vector_base.28"** %12, align 8
  %61 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %12, align 8
  %62 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %61, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %62, %"class.std::__1::__compressed_pair.29"** %11, align 8
  %63 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %11, align 8
  %64 = bitcast %"class.std::__1::__compressed_pair.29"* %63 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %64, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %10, align 8
  %65 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %10, align 8
  %66 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.30", %"class.std::__1::__libcpp_compressed_pair_imp.30"* %65, i32 0, i32 0
  %67 = load %class.Token*, %class.Token** %66, align 8
  %68 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %60, i32 0, i32 0
  %69 = load %class.Token*, %class.Token** %68, align 8
  %70 = ptrtoint %class.Token* %67 to i64
  %71 = ptrtoint %class.Token* %69 to i64
  %72 = sub i64 %70, %71
  %73 = sdiv exact i64 %72, 288
  %74 = getelementptr inbounds %class.Token, %class.Token* %57, i64 %73
  %75 = bitcast %class.Token* %74 to i8*
  store %"class.std::__1::vector.27"* %23, %"class.std::__1::vector.27"** %16, align 8
  %76 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %16, align 8
  %77 = bitcast %"class.std::__1::vector.27"* %76 to %"class.std::__1::__vector_base.28"*
  %78 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %77, i32 0, i32 0
  %79 = load %class.Token*, %class.Token** %78, align 8
  store %class.Token* %79, %class.Token** %15, align 8
  %80 = load %class.Token*, %class.Token** %15, align 8
  %81 = load i64, i64* %22, align 8
  %82 = getelementptr inbounds %class.Token, %class.Token* %80, i64 %81
  %83 = bitcast %class.Token* %82 to i8*
  call void @_ZNKSt3__16vectorI5TokenNS_9allocatorIS1_EEE31__annotate_contiguous_containerEPKvS6_S6_S6_(%"class.std::__1::vector.27"* %23, i8* %29, i8* %52, i8* %75, i8* %83)
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNKSt3__16vectorI5TokenNS_9allocatorIS1_EEE31__annotate_contiguous_containerEPKvS6_S6_S6_(%"class.std::__1::vector.27"*, i8*, i8*, i8*, i8*) #5 align 2 {
  %6 = alloca %"class.std::__1::vector.27"*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i8*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca i8*, align 8
  store %"class.std::__1::vector.27"* %0, %"class.std::__1::vector.27"** %6, align 8
  store i8* %1, i8** %7, align 8
  store i8* %2, i8** %8, align 8
  store i8* %3, i8** %9, align 8
  store i8* %4, i8** %10, align 8
  %11 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %6, align 8
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__114__split_bufferI5TokenRNS_9allocatorIS1_EEED2Ev(%"struct.std::__1::__split_buffer"*) unnamed_addr #5 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca i8*, align 8
  %3 = alloca %"class.std::__1::allocator.31"*, align 8
  %4 = alloca %class.Token*, align 8
  %5 = alloca i64, align 8
  %6 = alloca %"class.std::__1::allocator.31"*, align 8
  %7 = alloca %class.Token*, align 8
  %8 = alloca i64, align 8
  %9 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %11 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %12 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %13 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %14 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %15 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %16 = alloca %"class.std::__1::allocator.31"*, align 8
  %17 = alloca %class.Token*, align 8
  %18 = alloca %"struct.std::__1::integral_constant", align 1
  %19 = alloca %"class.std::__1::allocator.31"*, align 8
  %20 = alloca %class.Token*, align 8
  %21 = alloca %"class.std::__1::allocator.31"*, align 8
  %22 = alloca %class.Token*, align 8
  %23 = alloca %"struct.std::__1::integral_constant", align 1
  %24 = alloca %"struct.std::__1::__has_destroy.79", align 1
  %25 = alloca %class.Token*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.81"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.80"*, align 8
  %28 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %29 = alloca %"struct.std::__1::integral_constant.77", align 1
  %30 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %31 = alloca %class.Token*, align 8
  %32 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %33 = alloca %class.Token*, align 8
  %34 = alloca %"struct.std::__1::integral_constant.77", align 1
  %35 = alloca %"struct.std::__1::__split_buffer"*, align 8
  %36 = alloca %"struct.std::__1::__split_buffer"*, align 8
  store %"struct.std::__1::__split_buffer"* %0, %"struct.std::__1::__split_buffer"** %36, align 8
  %37 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %36, align 8
  store %"struct.std::__1::__split_buffer"* %37, %"struct.std::__1::__split_buffer"** %35, align 8
  %38 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %35, align 8
  %39 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %38, i32 0, i32 1
  %40 = load %class.Token*, %class.Token** %39, align 8
  store %"struct.std::__1::__split_buffer"* %38, %"struct.std::__1::__split_buffer"** %32, align 8
  store %class.Token* %40, %class.Token** %33, align 8
  %41 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %32, align 8
  %42 = load %class.Token*, %class.Token** %33, align 8
  store %"struct.std::__1::__split_buffer"* %41, %"struct.std::__1::__split_buffer"** %30, align 8
  store %class.Token* %42, %class.Token** %31, align 8
  %43 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %30, align 8
  br label %44

; <label>:44:                                     ; preds = %49, %1
  %45 = load %class.Token*, %class.Token** %31, align 8
  %46 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %43, i32 0, i32 2
  %47 = load %class.Token*, %class.Token** %46, align 8
  %48 = icmp ne %class.Token* %45, %47
  br i1 %48, label %49, label %68

; <label>:49:                                     ; preds = %44
  store %"struct.std::__1::__split_buffer"* %43, %"struct.std::__1::__split_buffer"** %28, align 8
  %50 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %28, align 8
  %51 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %50, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.80"* %51, %"class.std::__1::__compressed_pair.80"** %27, align 8
  %52 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %27, align 8
  %53 = bitcast %"class.std::__1::__compressed_pair.80"* %52 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %53, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %26, align 8
  %54 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %26, align 8
  %55 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %54, i32 0, i32 1
  %56 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %55, align 8
  %57 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %43, i32 0, i32 2
  %58 = load %class.Token*, %class.Token** %57, align 8
  %59 = getelementptr inbounds %class.Token, %class.Token* %58, i32 -1
  store %class.Token* %59, %class.Token** %57, align 8
  store %class.Token* %59, %class.Token** %25, align 8
  %60 = load %class.Token*, %class.Token** %25, align 8
  store %"class.std::__1::allocator.31"* %56, %"class.std::__1::allocator.31"** %21, align 8
  store %class.Token* %60, %class.Token** %22, align 8
  %61 = bitcast %"struct.std::__1::__has_destroy.79"* %24 to %"struct.std::__1::integral_constant"*
  %62 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %21, align 8
  %63 = load %class.Token*, %class.Token** %22, align 8
  store %"class.std::__1::allocator.31"* %62, %"class.std::__1::allocator.31"** %19, align 8
  store %class.Token* %63, %class.Token** %20, align 8
  %64 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %19, align 8
  %65 = load %class.Token*, %class.Token** %20, align 8
  store %"class.std::__1::allocator.31"* %64, %"class.std::__1::allocator.31"** %16, align 8
  store %class.Token* %65, %class.Token** %17, align 8
  %66 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %16, align 8
  %67 = load %class.Token*, %class.Token** %17, align 8
  call void @_ZN5TokenD1Ev(%class.Token* %67) #11
  br label %44

; <label>:68:                                     ; preds = %44
  %69 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %37, i32 0, i32 0
  %70 = load %class.Token*, %class.Token** %69, align 8
  %71 = icmp ne %class.Token* %70, null
  br i1 %71, label %72, label %104

; <label>:72:                                     ; preds = %68
  store %"struct.std::__1::__split_buffer"* %37, %"struct.std::__1::__split_buffer"** %15, align 8
  %73 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %15, align 8
  %74 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %73, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.80"* %74, %"class.std::__1::__compressed_pair.80"** %14, align 8
  %75 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %14, align 8
  %76 = bitcast %"class.std::__1::__compressed_pair.80"* %75 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %76, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %13, align 8
  %77 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %13, align 8
  %78 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %77, i32 0, i32 1
  %79 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %78, align 8
  %80 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %37, i32 0, i32 0
  %81 = load %class.Token*, %class.Token** %80, align 8
  store %"struct.std::__1::__split_buffer"* %37, %"struct.std::__1::__split_buffer"** %12, align 8
  %82 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %12, align 8
  store %"struct.std::__1::__split_buffer"* %82, %"struct.std::__1::__split_buffer"** %11, align 8
  %83 = load %"struct.std::__1::__split_buffer"*, %"struct.std::__1::__split_buffer"** %11, align 8
  %84 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %83, i32 0, i32 3
  store %"class.std::__1::__compressed_pair.80"* %84, %"class.std::__1::__compressed_pair.80"** %10, align 8
  %85 = load %"class.std::__1::__compressed_pair.80"*, %"class.std::__1::__compressed_pair.80"** %10, align 8
  %86 = bitcast %"class.std::__1::__compressed_pair.80"* %85 to %"class.std::__1::__libcpp_compressed_pair_imp.81"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.81"* %86, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %9, align 8
  %87 = load %"class.std::__1::__libcpp_compressed_pair_imp.81"*, %"class.std::__1::__libcpp_compressed_pair_imp.81"** %9, align 8
  %88 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.81", %"class.std::__1::__libcpp_compressed_pair_imp.81"* %87, i32 0, i32 0
  %89 = load %class.Token*, %class.Token** %88, align 8
  %90 = getelementptr inbounds %"struct.std::__1::__split_buffer", %"struct.std::__1::__split_buffer"* %82, i32 0, i32 0
  %91 = load %class.Token*, %class.Token** %90, align 8
  %92 = ptrtoint %class.Token* %89 to i64
  %93 = ptrtoint %class.Token* %91 to i64
  %94 = sub i64 %92, %93
  %95 = sdiv exact i64 %94, 288
  br label %96

; <label>:96:                                     ; preds = %72
  store %"class.std::__1::allocator.31"* %79, %"class.std::__1::allocator.31"** %6, align 8
  store %class.Token* %81, %class.Token** %7, align 8
  store i64 %95, i64* %8, align 8
  %97 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %6, align 8
  %98 = load %class.Token*, %class.Token** %7, align 8
  %99 = load i64, i64* %8, align 8
  store %"class.std::__1::allocator.31"* %97, %"class.std::__1::allocator.31"** %3, align 8
  store %class.Token* %98, %class.Token** %4, align 8
  store i64 %99, i64* %5, align 8
  %100 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %3, align 8
  %101 = load %class.Token*, %class.Token** %4, align 8
  %102 = bitcast %class.Token* %101 to i8*
  store i8* %102, i8** %2, align 8
  %103 = load i8*, i8** %2, align 8
  call void @_ZdlPv(i8* %103) #13
  br label %104

; <label>:104:                                    ; preds = %96, %68
  ret void
                                                  ; No predecessors!
  %106 = landingpad { i8*, i32 }
          catch i8* null
  %107 = extractvalue { i8*, i32 } %106, 0
  call void @__clang_call_terminate(i8* %107) #12
  unreachable
}

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcmm(%"class.std::__1::basic_string"*, i8*, i64, i64) #1

declare dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"*, i8*, i64) #1

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

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1), i8* dereferenceable(1)) #2 align 2 {
  %3 = alloca i8*, align 8
  %4 = alloca i8*, align 8
  store i8* %0, i8** %3, align 8
  store i8* %1, i8** %4, align 8
  %5 = load i8*, i8** %4, align 8
  %6 = load i8, i8* %5, align 1
  %7 = load i8*, i8** %3, align 8
  store i8 %6, i8* %7, align 1
  ret void
}

; Function Attrs: nounwind
declare i32 @_ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7compareEPKc(%"class.std::__1::basic_string"*, i8*) #4

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE9push_backEc(%"class.std::__1::basic_string"*, i8 signext) #1

declare dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"*, i64, i8*) #1

declare dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"*, i8*) #1

attributes #0 = { ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { inlinehint nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { inlinehint ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { noinline noreturn nounwind }
attributes #7 = { nounwind readnone }
attributes #8 = { nobuiltin nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #9 = { argmemonly nounwind }
attributes #10 = { nobuiltin "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #11 = { nounwind }
attributes #12 = { noreturn nounwind }
attributes #13 = { builtin nounwind }
attributes #14 = { builtin }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"Apple LLVM version 8.1.0 (clang-802.0.42)"}
!2 = !{!3}
!3 = distinct !{!3, !4, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!4 = distinct !{!4, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!5 = !{!6}
!6 = distinct !{!6, !7, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!7 = distinct !{!7, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!8 = !{!9}
!9 = distinct !{!9, !10, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_RKS9_: argument 0"}
!10 = distinct !{!10, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_RKS9_"}
