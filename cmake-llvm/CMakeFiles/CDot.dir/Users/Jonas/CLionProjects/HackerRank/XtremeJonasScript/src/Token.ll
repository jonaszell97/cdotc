; ModuleID = '/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/Token.cpp'
source_filename = "/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/Token.cpp"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.12.0"

%"class.std::__1::map" = type { %"class.std::__1::__tree" }
%"class.std::__1::__tree" = type { %"class.std::__1::__tree_end_node"*, %"class.std::__1::__compressed_pair", %"class.std::__1::__compressed_pair.0" }
%"class.std::__1::__tree_end_node" = type { %"class.std::__1::__tree_node_base"* }
%"class.std::__1::__tree_node_base" = type <{ %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_end_node"*, i8, [7 x i8] }>
%"class.std::__1::__compressed_pair" = type { %"class.std::__1::__libcpp_compressed_pair_imp" }
%"class.std::__1::__libcpp_compressed_pair_imp" = type { %"class.std::__1::__tree_end_node" }
%"class.std::__1::__compressed_pair.0" = type { %"class.std::__1::__libcpp_compressed_pair_imp.1" }
%"class.std::__1::__libcpp_compressed_pair_imp.1" = type { i64 }
%"class.std::initializer_list" = type { %"struct.std::__1::pair"*, i64 }
%"struct.std::__1::pair" = type { i32, %"class.std::__1::basic_string" }
%"class.std::__1::basic_string" = type { %"class.std::__1::__compressed_pair.2" }
%"class.std::__1::__compressed_pair.2" = type { %"class.std::__1::__libcpp_compressed_pair_imp.3" }
%"class.std::__1::__libcpp_compressed_pair_imp.3" = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" = type { %union.anon }
%union.anon = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" = type { i64, i64, i8* }
%"class.std::__1::__tree_const_iterator" = type { %"class.std::__1::__tree_end_node"* }
%"class.std::__1::__map_const_iterator" = type { %"class.std::__1::__tree_const_iterator" }
%"class.std::__1::__tree_iterator" = type { %"class.std::__1::__tree_end_node"* }
%"class.std::__1::__map_iterator" = type { %"class.std::__1::__tree_iterator" }
%"class.std::__1::__map_value_compare" = type { i8 }
%"struct.std::__1::less" = type { i8 }
%"class.std::__1::allocator.4" = type { i8 }
%"class.std::__1::__basic_string_common" = type { i8 }
%class.Token = type { %struct.Variant, i32, i32, i32, i32, i32, i32 }
%struct.Variant = type <{ %union.anon.7, i8, [7 x i8], %"class.std::__1::basic_string", %struct.TypeSpecifier, i8, i8, i8, [5 x i8] }>
%union.anon.7 = type { i64 }
%struct.TypeSpecifier = type { i32, %"class.std::__1::basic_string", %"class.std::__1::shared_ptr", i8, i8, i8, i8, i64, i32, i8, i8, %"class.std::__1::basic_string", i8, [7 x i8], %"class.std::__1::unordered_map", i8, i8, i8, i8, i8, i8, i8, i32, %"class.std::__1::vector", %"class.std::__1::vector.27", %union.anon.33 }
%"class.std::__1::shared_ptr" = type { %class.Expression*, %"class.std::__1::__shared_weak_count"* }
%class.Expression = type opaque
%"class.std::__1::__shared_weak_count" = type { %"class.std::__1::__shared_count", i64 }
%"class.std::__1::__shared_count" = type { i32 (...)**, i64 }
%"class.std::__1::unordered_map" = type { %"class.std::__1::__hash_table" }
%"class.std::__1::__hash_table" = type <{ %"class.std::__1::unique_ptr", %"class.std::__1::__compressed_pair.14", %"class.std::__1::__compressed_pair.18", %"class.std::__1::__compressed_pair.20", [4 x i8] }>
%"class.std::__1::unique_ptr" = type { %"class.std::__1::__compressed_pair.8" }
%"class.std::__1::__compressed_pair.8" = type { %"class.std::__1::__libcpp_compressed_pair_imp.9" }
%"class.std::__1::__libcpp_compressed_pair_imp.9" = type { %"struct.std::__1::__hash_node_base"**, %"class.std::__1::__bucket_list_deallocator" }
%"struct.std::__1::__hash_node_base" = type { %"struct.std::__1::__hash_node_base"* }
%"class.std::__1::__bucket_list_deallocator" = type { %"class.std::__1::__compressed_pair.10" }
%"class.std::__1::__compressed_pair.10" = type { %"class.std::__1::__libcpp_compressed_pair_imp.11" }
%"class.std::__1::__libcpp_compressed_pair_imp.11" = type { i64 }
%"class.std::__1::__compressed_pair.14" = type { %"class.std::__1::__libcpp_compressed_pair_imp.15" }
%"class.std::__1::__libcpp_compressed_pair_imp.15" = type { %"struct.std::__1::__hash_node_base" }
%"class.std::__1::__compressed_pair.18" = type { %"class.std::__1::__libcpp_compressed_pair_imp.19" }
%"class.std::__1::__libcpp_compressed_pair_imp.19" = type { i64 }
%"class.std::__1::__compressed_pair.20" = type { %"class.std::__1::__libcpp_compressed_pair_imp.21" }
%"class.std::__1::__libcpp_compressed_pair_imp.21" = type { float }
%"class.std::__1::vector" = type { %"class.std::__1::__vector_base" }
%"class.std::__1::__vector_base" = type { %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"*, %"class.std::__1::__compressed_pair.23" }
%"class.std::__1::__compressed_pair.23" = type { %"class.std::__1::__libcpp_compressed_pair_imp.24" }
%"class.std::__1::__libcpp_compressed_pair_imp.24" = type { %"class.std::__1::basic_string"* }
%"class.std::__1::vector.27" = type { %"class.std::__1::__vector_base.28" }
%"class.std::__1::__vector_base.28" = type { %struct.TypeSpecifier*, %struct.TypeSpecifier*, %"class.std::__1::__compressed_pair.29" }
%"class.std::__1::__compressed_pair.29" = type { %"class.std::__1::__libcpp_compressed_pair_imp.30" }
%"class.std::__1::__libcpp_compressed_pair_imp.30" = type { %struct.TypeSpecifier* }
%union.anon.33 = type { %struct.TypeSpecifier* }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short" = type { %union.anon.6, [23 x i8] }
%union.anon.6 = type { i8 }
%"class.std::__1::__tree_node" = type { %"class.std::__1::__tree_node_base.base", %"union.std::__1::__value_type" }
%"class.std::__1::__tree_node_base.base" = type <{ %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_end_node"*, i8 }>
%"union.std::__1::__value_type" = type { %"struct.std::__1::pair" }
%"class.std::__1::allocator" = type { i8 }
%"struct.std::__1::integral_constant" = type { i8 }
%"struct.std::__1::__has_destroy" = type { i8 }
%"class.std::__1::allocator.31" = type { i8 }
%"struct.std::__1::integral_constant.35" = type { i8 }
%"struct.std::__1::__has_destroy.36" = type { i8 }
%"class.std::__1::allocator.25" = type { i8 }
%"struct.std::__1::__has_destroy.37" = type { i8 }
%"class.std::__1::allocator.12" = type { i8 }
%"class.std::__1::allocator.16" = type { i8 }
%"struct.std::__1::pair.38" = type { %"class.std::__1::basic_string", %struct.TypeSpecifier }
%"struct.std::__1::__has_destroy.40" = type { i8 }
%"struct.std::__1::__hash_node" = type { %"struct.std::__1::__hash_node_base", i64, %"union.std::__1::__hash_value_type" }
%"union.std::__1::__hash_value_type" = type { %"struct.std::__1::pair.38" }
%"class.std::__1::__libcpp_compressed_pair_imp.43" = type { %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node_destructor" }
%"class.std::__1::__tree_node_destructor" = type <{ %"class.std::__1::allocator"*, i8, [7 x i8] }>
%"class.std::__1::__compressed_pair.42" = type { %"class.std::__1::__libcpp_compressed_pair_imp.43" }
%"class.std::__1::unique_ptr.41" = type { %"class.std::__1::__compressed_pair.42" }
%"struct.std::__1::bidirectional_iterator_tag" = type { i8 }
%"struct.std::__1::__has_construct" = type { i8 }

@_ZN4llvm24DisableABIBreakingChecksE = external global i32, align 4
@_ZN4llvm30VerifyDisableABIBreakingChecksE = weak hidden global i32* @_ZN4llvm24DisableABIBreakingChecksE, align 8
@_literal_names = global %"class.std::__1::map" zeroinitializer, align 8
@.str = private unnamed_addr constant [14 x i8] c"NumberLiteral\00", align 1
@.str.1 = private unnamed_addr constant [17 x i8] c"CharacterLiteral\00", align 1
@.str.2 = private unnamed_addr constant [15 x i8] c"BooleanLiteral\00", align 1
@.str.3 = private unnamed_addr constant [14 x i8] c"StringLiteral\00", align 1
@.str.4 = private unnamed_addr constant [14 x i8] c"ObjectLiteral\00", align 1
@__dso_handle = external global i8
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_Token.cpp, i8* null }]

; Function Attrs: ssp uwtable
define internal void @__cxx_global_var_init() #0 section "__TEXT,__StaticInit,regular,pure_instructions" personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %1 = alloca %"class.std::initializer_list"*, align 8
  %2 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %3 = alloca %"class.std::__1::__map_const_iterator"*, align 8
  %4 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %5 = alloca %"class.std::__1::__map_const_iterator"*, align 8
  %6 = alloca %"class.std::__1::__tree_iterator", align 8
  %7 = alloca %"class.std::__1::__map_iterator"*, align 8
  %8 = alloca %"class.std::__1::__tree_iterator", align 8
  %9 = alloca %"class.std::__1::__map_iterator"*, align 8
  %10 = alloca %"struct.std::__1::pair"*, align 8
  %11 = alloca %"class.std::__1::__tree_iterator", align 8
  %12 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %13 = alloca %"class.std::__1::__tree"*, align 8
  %14 = alloca %"struct.std::__1::pair"*, align 8
  %15 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %16 = alloca %"class.std::__1::__map_iterator", align 8
  %17 = alloca %"class.std::__1::__map_const_iterator", align 8
  %18 = alloca %"class.std::__1::map"*, align 8
  %19 = alloca %"struct.std::__1::pair"*, align 8
  %20 = alloca %"class.std::__1::__tree_iterator", align 8
  %21 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %22 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %23 = alloca %"class.std::__1::__map_const_iterator"*, align 8
  %24 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %25 = alloca %"class.std::__1::__map_const_iterator"*, align 8
  %26 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %27 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %28 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %29 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %30 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %31 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %32 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %33 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %34 = alloca %"class.std::__1::__tree"*, align 8
  %35 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %36 = alloca %"class.std::__1::__tree"*, align 8
  %37 = alloca %"class.std::__1::__map_const_iterator", align 8
  %38 = alloca %"class.std::__1::map"*, align 8
  %39 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %40 = alloca %"class.std::__1::__map_const_iterator", align 8
  %41 = alloca %"class.std::__1::map"*, align 8
  %42 = alloca %"class.std::__1::map"*, align 8
  %43 = alloca %"struct.std::__1::pair"*, align 8
  %44 = alloca %"struct.std::__1::pair"*, align 8
  %45 = alloca %"class.std::__1::__map_const_iterator", align 8
  %46 = alloca %"class.std::__1::__map_const_iterator", align 8
  %47 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %48 = alloca %"class.std::__1::__map_iterator", align 8
  %49 = alloca %"class.std::initializer_list"*, align 8
  %50 = alloca %"class.std::__1::__map_value_compare"*, align 8
  %51 = alloca %"struct.std::__1::less", align 1
  %52 = alloca %"class.std::__1::__map_value_compare"*, align 8
  %53 = alloca %"class.std::initializer_list", align 8
  %54 = alloca %"class.std::__1::map"*, align 8
  %55 = alloca %"struct.std::__1::less"*, align 8
  %56 = alloca %"class.std::__1::__map_value_compare", align 1
  %57 = alloca %"struct.std::__1::less", align 1
  %58 = alloca i8*
  %59 = alloca i32
  %60 = alloca %"class.std::initializer_list", align 8
  %61 = alloca %"class.std::__1::map"*, align 8
  %62 = alloca %"struct.std::__1::less"*, align 8
  %63 = alloca [14 x i8]*, align 8
  %64 = alloca %"class.std::__1::allocator.4"*, align 8
  %65 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %66 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %67 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %68 = alloca %"class.std::__1::basic_string"*, align 8
  %69 = alloca i8*, align 8
  %70 = alloca %"class.std::__1::basic_string"*, align 8
  %71 = alloca i8*, align 8
  %72 = alloca i32*, align 8
  %73 = alloca %"struct.std::__1::pair"*, align 8
  %74 = alloca i32*, align 8
  %75 = alloca [14 x i8]*, align 8
  %76 = alloca %"struct.std::__1::pair"*, align 8
  %77 = alloca i32*, align 8
  %78 = alloca [14 x i8]*, align 8
  %79 = alloca [14 x i8]*, align 8
  %80 = alloca %"class.std::__1::allocator.4"*, align 8
  %81 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %82 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %83 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %84 = alloca %"class.std::__1::basic_string"*, align 8
  %85 = alloca i8*, align 8
  %86 = alloca %"class.std::__1::basic_string"*, align 8
  %87 = alloca i8*, align 8
  %88 = alloca i32*, align 8
  %89 = alloca %"struct.std::__1::pair"*, align 8
  %90 = alloca i32*, align 8
  %91 = alloca [14 x i8]*, align 8
  %92 = alloca %"struct.std::__1::pair"*, align 8
  %93 = alloca i32*, align 8
  %94 = alloca [14 x i8]*, align 8
  %95 = alloca [15 x i8]*, align 8
  %96 = alloca %"class.std::__1::allocator.4"*, align 8
  %97 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %98 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %99 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %100 = alloca %"class.std::__1::basic_string"*, align 8
  %101 = alloca i8*, align 8
  %102 = alloca %"class.std::__1::basic_string"*, align 8
  %103 = alloca i8*, align 8
  %104 = alloca i32*, align 8
  %105 = alloca %"struct.std::__1::pair"*, align 8
  %106 = alloca i32*, align 8
  %107 = alloca [15 x i8]*, align 8
  %108 = alloca %"struct.std::__1::pair"*, align 8
  %109 = alloca i32*, align 8
  %110 = alloca [15 x i8]*, align 8
  %111 = alloca [17 x i8]*, align 8
  %112 = alloca %"class.std::__1::allocator.4"*, align 8
  %113 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %114 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %115 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %116 = alloca %"class.std::__1::basic_string"*, align 8
  %117 = alloca i8*, align 8
  %118 = alloca %"class.std::__1::basic_string"*, align 8
  %119 = alloca i8*, align 8
  %120 = alloca i32*, align 8
  %121 = alloca %"struct.std::__1::pair"*, align 8
  %122 = alloca i32*, align 8
  %123 = alloca [17 x i8]*, align 8
  %124 = alloca %"struct.std::__1::pair"*, align 8
  %125 = alloca i32*, align 8
  %126 = alloca [17 x i8]*, align 8
  %127 = alloca [14 x i8]*, align 8
  %128 = alloca %"class.std::__1::allocator.4"*, align 8
  %129 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %130 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %131 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %132 = alloca %"class.std::__1::basic_string"*, align 8
  %133 = alloca i8*, align 8
  %134 = alloca %"class.std::__1::basic_string"*, align 8
  %135 = alloca i8*, align 8
  %136 = alloca i32*, align 8
  %137 = alloca %"struct.std::__1::pair"*, align 8
  %138 = alloca i32*, align 8
  %139 = alloca [14 x i8]*, align 8
  %140 = alloca %"struct.std::__1::pair"*, align 8
  %141 = alloca i32*, align 8
  %142 = alloca [14 x i8]*, align 8
  %143 = alloca %"class.std::initializer_list", align 8
  %144 = alloca [5 x %"struct.std::__1::pair"], align 8
  %145 = alloca %"struct.std::__1::pair"*, align 8
  %146 = alloca i32, align 4
  %147 = alloca i8*
  %148 = alloca i32
  %149 = alloca i32, align 4
  %150 = alloca i32, align 4
  %151 = alloca i32, align 4
  %152 = alloca i32, align 4
  %153 = alloca %"struct.std::__1::less", align 1
  %154 = getelementptr inbounds [5 x %"struct.std::__1::pair"], [5 x %"struct.std::__1::pair"]* %144, i64 0, i64 0
  store %"struct.std::__1::pair"* %154, %"struct.std::__1::pair"** %145, align 8
  store i32 1, i32* %146, align 4
  store %"struct.std::__1::pair"* %154, %"struct.std::__1::pair"** %140, align 8
  store i32* %146, i32** %141, align 8
  store [14 x i8]* @.str, [14 x i8]** %142, align 8
  %155 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %140, align 8
  %156 = load i32*, i32** %141, align 8
  %157 = load [14 x i8]*, [14 x i8]** %142, align 8
  store %"struct.std::__1::pair"* %155, %"struct.std::__1::pair"** %137, align 8
  store i32* %156, i32** %138, align 8
  store [14 x i8]* %157, [14 x i8]** %139, align 8
  %158 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %137, align 8
  %159 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %158, i32 0, i32 0
  %160 = load i32*, i32** %138, align 8
  store i32* %160, i32** %136, align 8
  %161 = load i32*, i32** %136, align 8
  %162 = load i32, i32* %161, align 4
  store i32 %162, i32* %159, align 8
  %163 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %158, i32 0, i32 1
  %164 = load [14 x i8]*, [14 x i8]** %139, align 8
  store [14 x i8]* %164, [14 x i8]** %127, align 8
  %165 = load [14 x i8]*, [14 x i8]** %127, align 8
  %166 = getelementptr inbounds [14 x i8], [14 x i8]* %165, i32 0, i32 0
  store %"class.std::__1::basic_string"* %163, %"class.std::__1::basic_string"** %134, align 8
  store i8* %166, i8** %135, align 8
  %167 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %134, align 8
  %168 = load i8*, i8** %135, align 8
  store %"class.std::__1::basic_string"* %167, %"class.std::__1::basic_string"** %132, align 8
  store i8* %168, i8** %133, align 8
  %169 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %132, align 8
  %170 = bitcast %"class.std::__1::basic_string"* %169 to %"class.std::__1::__basic_string_common"*
  %171 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %169, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %171, %"class.std::__1::__compressed_pair.2"** %131, align 8
  %172 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %131, align 8
  store %"class.std::__1::__compressed_pair.2"* %172, %"class.std::__1::__compressed_pair.2"** %130, align 8
  %173 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %130, align 8
  %174 = bitcast %"class.std::__1::__compressed_pair.2"* %173 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %174, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %129, align 8
  %175 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %129, align 8
  %176 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.3"* %175 to %"class.std::__1::allocator.4"*
  store %"class.std::__1::allocator.4"* %176, %"class.std::__1::allocator.4"** %128, align 8
  %177 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %128, align 8
  %178 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %175, i32 0, i32 0
  %179 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %178 to i8*
  call void @llvm.memset.p0i8.i64(i8* %179, i8 0, i64 24, i32 8, i1 false) #2
  %180 = load i8*, i8** %133, align 8
  %181 = load i8*, i8** %133, align 8
  %182 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %181)
          to label %183 unwind label %478

; <label>:183:                                    ; preds = %0
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %169, i8* %180, i64 %182)
          to label %184 unwind label %478

; <label>:184:                                    ; preds = %183
  br label %185

; <label>:185:                                    ; preds = %184
  %186 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %154, i64 1
  store %"struct.std::__1::pair"* %186, %"struct.std::__1::pair"** %145, align 8
  store i32 2, i32* %149, align 4
  store %"struct.std::__1::pair"* %186, %"struct.std::__1::pair"** %124, align 8
  store i32* %149, i32** %125, align 8
  store [17 x i8]* @.str.1, [17 x i8]** %126, align 8
  %187 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %124, align 8
  %188 = load i32*, i32** %125, align 8
  %189 = load [17 x i8]*, [17 x i8]** %126, align 8
  store %"struct.std::__1::pair"* %187, %"struct.std::__1::pair"** %121, align 8
  store i32* %188, i32** %122, align 8
  store [17 x i8]* %189, [17 x i8]** %123, align 8
  %190 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %121, align 8
  %191 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %190, i32 0, i32 0
  %192 = load i32*, i32** %122, align 8
  store i32* %192, i32** %120, align 8
  %193 = load i32*, i32** %120, align 8
  %194 = load i32, i32* %193, align 4
  store i32 %194, i32* %191, align 8
  %195 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %190, i32 0, i32 1
  %196 = load [17 x i8]*, [17 x i8]** %123, align 8
  store [17 x i8]* %196, [17 x i8]** %111, align 8
  %197 = load [17 x i8]*, [17 x i8]** %111, align 8
  %198 = getelementptr inbounds [17 x i8], [17 x i8]* %197, i32 0, i32 0
  store %"class.std::__1::basic_string"* %195, %"class.std::__1::basic_string"** %118, align 8
  store i8* %198, i8** %119, align 8
  %199 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %118, align 8
  %200 = load i8*, i8** %119, align 8
  store %"class.std::__1::basic_string"* %199, %"class.std::__1::basic_string"** %116, align 8
  store i8* %200, i8** %117, align 8
  %201 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %116, align 8
  %202 = bitcast %"class.std::__1::basic_string"* %201 to %"class.std::__1::__basic_string_common"*
  %203 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %201, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %203, %"class.std::__1::__compressed_pair.2"** %115, align 8
  %204 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %115, align 8
  store %"class.std::__1::__compressed_pair.2"* %204, %"class.std::__1::__compressed_pair.2"** %114, align 8
  %205 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %114, align 8
  %206 = bitcast %"class.std::__1::__compressed_pair.2"* %205 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %206, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %113, align 8
  %207 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %113, align 8
  %208 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.3"* %207 to %"class.std::__1::allocator.4"*
  store %"class.std::__1::allocator.4"* %208, %"class.std::__1::allocator.4"** %112, align 8
  %209 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %112, align 8
  %210 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %207, i32 0, i32 0
  %211 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %210 to i8*
  call void @llvm.memset.p0i8.i64(i8* %211, i8 0, i64 24, i32 8, i1 false) #2
  %212 = load i8*, i8** %117, align 8
  %213 = load i8*, i8** %117, align 8
  %214 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %213)
          to label %215 unwind label %478

; <label>:215:                                    ; preds = %185
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %201, i8* %212, i64 %214)
          to label %216 unwind label %478

; <label>:216:                                    ; preds = %215
  br label %217

; <label>:217:                                    ; preds = %216
  %218 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %186, i64 1
  store %"struct.std::__1::pair"* %218, %"struct.std::__1::pair"** %145, align 8
  store i32 3, i32* %150, align 4
  store %"struct.std::__1::pair"* %218, %"struct.std::__1::pair"** %108, align 8
  store i32* %150, i32** %109, align 8
  store [15 x i8]* @.str.2, [15 x i8]** %110, align 8
  %219 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %108, align 8
  %220 = load i32*, i32** %109, align 8
  %221 = load [15 x i8]*, [15 x i8]** %110, align 8
  store %"struct.std::__1::pair"* %219, %"struct.std::__1::pair"** %105, align 8
  store i32* %220, i32** %106, align 8
  store [15 x i8]* %221, [15 x i8]** %107, align 8
  %222 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %105, align 8
  %223 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %222, i32 0, i32 0
  %224 = load i32*, i32** %106, align 8
  store i32* %224, i32** %104, align 8
  %225 = load i32*, i32** %104, align 8
  %226 = load i32, i32* %225, align 4
  store i32 %226, i32* %223, align 8
  %227 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %222, i32 0, i32 1
  %228 = load [15 x i8]*, [15 x i8]** %107, align 8
  store [15 x i8]* %228, [15 x i8]** %95, align 8
  %229 = load [15 x i8]*, [15 x i8]** %95, align 8
  %230 = getelementptr inbounds [15 x i8], [15 x i8]* %229, i32 0, i32 0
  store %"class.std::__1::basic_string"* %227, %"class.std::__1::basic_string"** %102, align 8
  store i8* %230, i8** %103, align 8
  %231 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %102, align 8
  %232 = load i8*, i8** %103, align 8
  store %"class.std::__1::basic_string"* %231, %"class.std::__1::basic_string"** %100, align 8
  store i8* %232, i8** %101, align 8
  %233 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %100, align 8
  %234 = bitcast %"class.std::__1::basic_string"* %233 to %"class.std::__1::__basic_string_common"*
  %235 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %233, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %235, %"class.std::__1::__compressed_pair.2"** %99, align 8
  %236 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %99, align 8
  store %"class.std::__1::__compressed_pair.2"* %236, %"class.std::__1::__compressed_pair.2"** %98, align 8
  %237 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %98, align 8
  %238 = bitcast %"class.std::__1::__compressed_pair.2"* %237 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %238, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %97, align 8
  %239 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %97, align 8
  %240 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.3"* %239 to %"class.std::__1::allocator.4"*
  store %"class.std::__1::allocator.4"* %240, %"class.std::__1::allocator.4"** %96, align 8
  %241 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %96, align 8
  %242 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %239, i32 0, i32 0
  %243 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %242 to i8*
  call void @llvm.memset.p0i8.i64(i8* %243, i8 0, i64 24, i32 8, i1 false) #2
  %244 = load i8*, i8** %101, align 8
  %245 = load i8*, i8** %101, align 8
  %246 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %245)
          to label %247 unwind label %478

; <label>:247:                                    ; preds = %217
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %233, i8* %244, i64 %246)
          to label %248 unwind label %478

; <label>:248:                                    ; preds = %247
  br label %249

; <label>:249:                                    ; preds = %248
  %250 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %218, i64 1
  store %"struct.std::__1::pair"* %250, %"struct.std::__1::pair"** %145, align 8
  store i32 0, i32* %151, align 4
  store %"struct.std::__1::pair"* %250, %"struct.std::__1::pair"** %92, align 8
  store i32* %151, i32** %93, align 8
  store [14 x i8]* @.str.3, [14 x i8]** %94, align 8
  %251 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %92, align 8
  %252 = load i32*, i32** %93, align 8
  %253 = load [14 x i8]*, [14 x i8]** %94, align 8
  store %"struct.std::__1::pair"* %251, %"struct.std::__1::pair"** %89, align 8
  store i32* %252, i32** %90, align 8
  store [14 x i8]* %253, [14 x i8]** %91, align 8
  %254 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %89, align 8
  %255 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %254, i32 0, i32 0
  %256 = load i32*, i32** %90, align 8
  store i32* %256, i32** %88, align 8
  %257 = load i32*, i32** %88, align 8
  %258 = load i32, i32* %257, align 4
  store i32 %258, i32* %255, align 8
  %259 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %254, i32 0, i32 1
  %260 = load [14 x i8]*, [14 x i8]** %91, align 8
  store [14 x i8]* %260, [14 x i8]** %79, align 8
  %261 = load [14 x i8]*, [14 x i8]** %79, align 8
  %262 = getelementptr inbounds [14 x i8], [14 x i8]* %261, i32 0, i32 0
  store %"class.std::__1::basic_string"* %259, %"class.std::__1::basic_string"** %86, align 8
  store i8* %262, i8** %87, align 8
  %263 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %86, align 8
  %264 = load i8*, i8** %87, align 8
  store %"class.std::__1::basic_string"* %263, %"class.std::__1::basic_string"** %84, align 8
  store i8* %264, i8** %85, align 8
  %265 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %84, align 8
  %266 = bitcast %"class.std::__1::basic_string"* %265 to %"class.std::__1::__basic_string_common"*
  %267 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %265, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %267, %"class.std::__1::__compressed_pair.2"** %83, align 8
  %268 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %83, align 8
  store %"class.std::__1::__compressed_pair.2"* %268, %"class.std::__1::__compressed_pair.2"** %82, align 8
  %269 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %82, align 8
  %270 = bitcast %"class.std::__1::__compressed_pair.2"* %269 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %270, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %81, align 8
  %271 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %81, align 8
  %272 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.3"* %271 to %"class.std::__1::allocator.4"*
  store %"class.std::__1::allocator.4"* %272, %"class.std::__1::allocator.4"** %80, align 8
  %273 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %80, align 8
  %274 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %271, i32 0, i32 0
  %275 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %274 to i8*
  call void @llvm.memset.p0i8.i64(i8* %275, i8 0, i64 24, i32 8, i1 false) #2
  %276 = load i8*, i8** %85, align 8
  %277 = load i8*, i8** %85, align 8
  %278 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %277)
          to label %279 unwind label %478

; <label>:279:                                    ; preds = %249
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %265, i8* %276, i64 %278)
          to label %280 unwind label %478

; <label>:280:                                    ; preds = %279
  br label %281

; <label>:281:                                    ; preds = %280
  %282 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %250, i64 1
  store %"struct.std::__1::pair"* %282, %"struct.std::__1::pair"** %145, align 8
  store i32 4, i32* %152, align 4
  store %"struct.std::__1::pair"* %282, %"struct.std::__1::pair"** %76, align 8
  store i32* %152, i32** %77, align 8
  store [14 x i8]* @.str.4, [14 x i8]** %78, align 8
  %283 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %76, align 8
  %284 = load i32*, i32** %77, align 8
  %285 = load [14 x i8]*, [14 x i8]** %78, align 8
  store %"struct.std::__1::pair"* %283, %"struct.std::__1::pair"** %73, align 8
  store i32* %284, i32** %74, align 8
  store [14 x i8]* %285, [14 x i8]** %75, align 8
  %286 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %73, align 8
  %287 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %286, i32 0, i32 0
  %288 = load i32*, i32** %74, align 8
  store i32* %288, i32** %72, align 8
  %289 = load i32*, i32** %72, align 8
  %290 = load i32, i32* %289, align 4
  store i32 %290, i32* %287, align 8
  %291 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %286, i32 0, i32 1
  %292 = load [14 x i8]*, [14 x i8]** %75, align 8
  store [14 x i8]* %292, [14 x i8]** %63, align 8
  %293 = load [14 x i8]*, [14 x i8]** %63, align 8
  %294 = getelementptr inbounds [14 x i8], [14 x i8]* %293, i32 0, i32 0
  store %"class.std::__1::basic_string"* %291, %"class.std::__1::basic_string"** %70, align 8
  store i8* %294, i8** %71, align 8
  %295 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %70, align 8
  %296 = load i8*, i8** %71, align 8
  store %"class.std::__1::basic_string"* %295, %"class.std::__1::basic_string"** %68, align 8
  store i8* %296, i8** %69, align 8
  %297 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8
  %298 = bitcast %"class.std::__1::basic_string"* %297 to %"class.std::__1::__basic_string_common"*
  %299 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %297, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %299, %"class.std::__1::__compressed_pair.2"** %67, align 8
  %300 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %67, align 8
  store %"class.std::__1::__compressed_pair.2"* %300, %"class.std::__1::__compressed_pair.2"** %66, align 8
  %301 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %66, align 8
  %302 = bitcast %"class.std::__1::__compressed_pair.2"* %301 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %302, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %65, align 8
  %303 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %65, align 8
  %304 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.3"* %303 to %"class.std::__1::allocator.4"*
  store %"class.std::__1::allocator.4"* %304, %"class.std::__1::allocator.4"** %64, align 8
  %305 = load %"class.std::__1::allocator.4"*, %"class.std::__1::allocator.4"** %64, align 8
  %306 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %303, i32 0, i32 0
  %307 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %306 to i8*
  call void @llvm.memset.p0i8.i64(i8* %307, i8 0, i64 24, i32 8, i1 false) #2
  %308 = load i8*, i8** %69, align 8
  %309 = load i8*, i8** %69, align 8
  %310 = invoke i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %309)
          to label %311 unwind label %478

; <label>:311:                                    ; preds = %281
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"* %297, i8* %308, i64 %310)
          to label %312 unwind label %478

; <label>:312:                                    ; preds = %311
  br label %313

; <label>:313:                                    ; preds = %312
  %314 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %143, i32 0, i32 0
  %315 = getelementptr inbounds [5 x %"struct.std::__1::pair"], [5 x %"struct.std::__1::pair"]* %144, i64 0, i64 0
  store %"struct.std::__1::pair"* %315, %"struct.std::__1::pair"** %314, align 8
  %316 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %143, i32 0, i32 1
  store i64 5, i64* %316, align 8
  %317 = bitcast %"class.std::initializer_list"* %143 to { %"struct.std::__1::pair"*, i64 }*
  %318 = getelementptr inbounds { %"struct.std::__1::pair"*, i64 }, { %"struct.std::__1::pair"*, i64 }* %317, i32 0, i32 0
  %319 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %318, align 8
  %320 = getelementptr inbounds { %"struct.std::__1::pair"*, i64 }, { %"struct.std::__1::pair"*, i64 }* %317, i32 0, i32 1
  %321 = load i64, i64* %320, align 8
  %322 = bitcast %"class.std::initializer_list"* %60 to { %"struct.std::__1::pair"*, i64 }*
  %323 = getelementptr inbounds { %"struct.std::__1::pair"*, i64 }, { %"struct.std::__1::pair"*, i64 }* %322, i32 0, i32 0
  store %"struct.std::__1::pair"* %319, %"struct.std::__1::pair"** %323, align 8
  %324 = getelementptr inbounds { %"struct.std::__1::pair"*, i64 }, { %"struct.std::__1::pair"*, i64 }* %322, i32 0, i32 1
  store i64 %321, i64* %324, align 8
  store %"class.std::__1::map"* @_literal_names, %"class.std::__1::map"** %61, align 8
  store %"struct.std::__1::less"* %153, %"struct.std::__1::less"** %62, align 8
  %325 = load %"class.std::__1::map"*, %"class.std::__1::map"** %61, align 8
  %326 = load %"struct.std::__1::less"*, %"struct.std::__1::less"** %62, align 8
  %327 = bitcast %"class.std::initializer_list"* %60 to { %"struct.std::__1::pair"*, i64 }*
  %328 = getelementptr inbounds { %"struct.std::__1::pair"*, i64 }, { %"struct.std::__1::pair"*, i64 }* %327, i32 0, i32 0
  %329 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %328, align 8
  %330 = getelementptr inbounds { %"struct.std::__1::pair"*, i64 }, { %"struct.std::__1::pair"*, i64 }* %327, i32 0, i32 1
  %331 = load i64, i64* %330, align 8
  %332 = bitcast %"class.std::initializer_list"* %53 to { %"struct.std::__1::pair"*, i64 }*
  %333 = getelementptr inbounds { %"struct.std::__1::pair"*, i64 }, { %"struct.std::__1::pair"*, i64 }* %332, i32 0, i32 0
  store %"struct.std::__1::pair"* %329, %"struct.std::__1::pair"** %333, align 8
  %334 = getelementptr inbounds { %"struct.std::__1::pair"*, i64 }, { %"struct.std::__1::pair"*, i64 }* %332, i32 0, i32 1
  store i64 %331, i64* %334, align 8
  store %"class.std::__1::map"* %325, %"class.std::__1::map"** %54, align 8
  store %"struct.std::__1::less"* %326, %"struct.std::__1::less"** %55, align 8
  %335 = load %"class.std::__1::map"*, %"class.std::__1::map"** %54, align 8
  %336 = getelementptr inbounds %"class.std::__1::map", %"class.std::__1::map"* %335, i32 0, i32 0
  %337 = load %"struct.std::__1::less"*, %"struct.std::__1::less"** %55, align 8
  store %"class.std::__1::__map_value_compare"* %56, %"class.std::__1::__map_value_compare"** %52, align 8
  %338 = load %"class.std::__1::__map_value_compare"*, %"class.std::__1::__map_value_compare"** %52, align 8
  store %"class.std::__1::__map_value_compare"* %338, %"class.std::__1::__map_value_compare"** %50, align 8
  %339 = load %"class.std::__1::__map_value_compare"*, %"class.std::__1::__map_value_compare"** %50, align 8
  %340 = bitcast %"class.std::__1::__map_value_compare"* %339 to %"struct.std::__1::less"*
  call void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEEC1ERKSD_(%"class.std::__1::__tree"* %336, %"class.std::__1::__map_value_compare"* dereferenceable(1) %56) #2
  store %"class.std::initializer_list"* %53, %"class.std::initializer_list"** %49, align 8
  %341 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %49, align 8
  %342 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %341, i32 0, i32 0
  %343 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %342, align 8
  store %"class.std::initializer_list"* %53, %"class.std::initializer_list"** %1, align 8
  %344 = load %"class.std::initializer_list"*, %"class.std::initializer_list"** %1, align 8
  %345 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %344, i32 0, i32 0
  %346 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %345, align 8
  %347 = getelementptr inbounds %"class.std::initializer_list", %"class.std::initializer_list"* %344, i32 0, i32 1
  %348 = load i64, i64* %347, align 8
  %349 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %346, i64 %348
  store %"class.std::__1::map"* %335, %"class.std::__1::map"** %42, align 8
  store %"struct.std::__1::pair"* %343, %"struct.std::__1::pair"** %43, align 8
  store %"struct.std::__1::pair"* %349, %"struct.std::__1::pair"** %44, align 8
  %350 = load %"class.std::__1::map"*, %"class.std::__1::map"** %42, align 8
  store %"class.std::__1::map"* %350, %"class.std::__1::map"** %41, align 8
  %351 = load %"class.std::__1::map"*, %"class.std::__1::map"** %41, align 8
  store %"class.std::__1::map"* %351, %"class.std::__1::map"** %38, align 8
  %352 = load %"class.std::__1::map"*, %"class.std::__1::map"** %38, align 8
  %353 = getelementptr inbounds %"class.std::__1::map", %"class.std::__1::map"* %352, i32 0, i32 0
  store %"class.std::__1::__tree"* %353, %"class.std::__1::__tree"** %36, align 8
  %354 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %36, align 8
  store %"class.std::__1::__tree"* %354, %"class.std::__1::__tree"** %34, align 8
  %355 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %34, align 8
  %356 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %355, i32 0, i32 1
  store %"class.std::__1::__compressed_pair"* %356, %"class.std::__1::__compressed_pair"** %33, align 8
  %357 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %33, align 8
  %358 = bitcast %"class.std::__1::__compressed_pair"* %357 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %358, %"class.std::__1::__libcpp_compressed_pair_imp"** %32, align 8
  %359 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %32, align 8
  %360 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %359, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %360, %"class.std::__1::__tree_end_node"** %31, align 8
  %361 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %31, align 8
  store %"class.std::__1::__tree_end_node"* %361, %"class.std::__1::__tree_end_node"** %30, align 8
  %362 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %30, align 8
  %363 = bitcast %"class.std::__1::__tree_end_node"* %362 to i8*
  store %"class.std::__1::__tree_const_iterator"* %35, %"class.std::__1::__tree_const_iterator"** %28, align 8
  store %"class.std::__1::__tree_end_node"* %362, %"class.std::__1::__tree_end_node"** %29, align 8
  %364 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %28, align 8
  %365 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %29, align 8
  store %"class.std::__1::__tree_const_iterator"* %364, %"class.std::__1::__tree_const_iterator"** %26, align 8
  store %"class.std::__1::__tree_end_node"* %365, %"class.std::__1::__tree_end_node"** %27, align 8
  %366 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %26, align 8
  %367 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %366, i32 0, i32 0
  %368 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %27, align 8
  store %"class.std::__1::__tree_end_node"* %368, %"class.std::__1::__tree_end_node"** %367, align 8
  %369 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %35, i32 0, i32 0
  %370 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %369, align 8
  %371 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %39, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %370, %"class.std::__1::__tree_end_node"** %371, align 8
  %372 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %39, i32 0, i32 0
  %373 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %372, align 8
  %374 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %24, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %373, %"class.std::__1::__tree_end_node"** %374, align 8
  store %"class.std::__1::__map_const_iterator"* %37, %"class.std::__1::__map_const_iterator"** %25, align 8
  %375 = load %"class.std::__1::__map_const_iterator"*, %"class.std::__1::__map_const_iterator"** %25, align 8
  %376 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %24, i32 0, i32 0
  %377 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %376, align 8
  %378 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %22, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %377, %"class.std::__1::__tree_end_node"** %378, align 8
  store %"class.std::__1::__map_const_iterator"* %375, %"class.std::__1::__map_const_iterator"** %23, align 8
  %379 = load %"class.std::__1::__map_const_iterator"*, %"class.std::__1::__map_const_iterator"** %23, align 8
  %380 = getelementptr inbounds %"class.std::__1::__map_const_iterator", %"class.std::__1::__map_const_iterator"* %379, i32 0, i32 0
  %381 = bitcast %"class.std::__1::__tree_const_iterator"* %380 to i8*
  %382 = bitcast %"class.std::__1::__tree_const_iterator"* %22 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %381, i8* %382, i64 8, i32 8, i1 false) #2
  %383 = getelementptr inbounds %"class.std::__1::__map_const_iterator", %"class.std::__1::__map_const_iterator"* %37, i32 0, i32 0
  %384 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %383, i32 0, i32 0
  %385 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %384, align 8
  %386 = getelementptr inbounds %"class.std::__1::__map_const_iterator", %"class.std::__1::__map_const_iterator"* %40, i32 0, i32 0
  %387 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %386, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %385, %"class.std::__1::__tree_end_node"** %387, align 8
  %388 = getelementptr inbounds %"class.std::__1::__map_const_iterator", %"class.std::__1::__map_const_iterator"* %40, i32 0, i32 0
  %389 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %388, i32 0, i32 0
  %390 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %389, align 8
  %391 = getelementptr inbounds %"class.std::__1::__map_const_iterator", %"class.std::__1::__map_const_iterator"* %45, i32 0, i32 0
  %392 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %391, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %390, %"class.std::__1::__tree_end_node"** %392, align 8
  br label %393

; <label>:393:                                    ; preds = %437, %313
  %394 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %43, align 8
  %395 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %44, align 8
  %396 = icmp ne %"struct.std::__1::pair"* %394, %395
  br i1 %396, label %397, label %468

; <label>:397:                                    ; preds = %393
  %398 = getelementptr inbounds %"class.std::__1::__map_const_iterator", %"class.std::__1::__map_const_iterator"* %45, i32 0, i32 0
  %399 = bitcast %"class.std::__1::__tree_const_iterator"* %47 to i8*
  %400 = bitcast %"class.std::__1::__tree_const_iterator"* %398 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %399, i8* %400, i64 8, i32 8, i1 false)
  %401 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %47, i32 0, i32 0
  %402 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %401, align 8
  %403 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %4, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %402, %"class.std::__1::__tree_end_node"** %403, align 8
  store %"class.std::__1::__map_const_iterator"* %46, %"class.std::__1::__map_const_iterator"** %5, align 8
  %404 = load %"class.std::__1::__map_const_iterator"*, %"class.std::__1::__map_const_iterator"** %5, align 8
  %405 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %4, i32 0, i32 0
  %406 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %405, align 8
  %407 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %2, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %406, %"class.std::__1::__tree_end_node"** %407, align 8
  store %"class.std::__1::__map_const_iterator"* %404, %"class.std::__1::__map_const_iterator"** %3, align 8
  %408 = load %"class.std::__1::__map_const_iterator"*, %"class.std::__1::__map_const_iterator"** %3, align 8
  %409 = getelementptr inbounds %"class.std::__1::__map_const_iterator", %"class.std::__1::__map_const_iterator"* %408, i32 0, i32 0
  %410 = bitcast %"class.std::__1::__tree_const_iterator"* %409 to i8*
  %411 = bitcast %"class.std::__1::__tree_const_iterator"* %2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %410, i8* %411, i64 8, i32 8, i1 false) #2
  %412 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %43, align 8
  %413 = getelementptr inbounds %"class.std::__1::__map_const_iterator", %"class.std::__1::__map_const_iterator"* %46, i32 0, i32 0
  %414 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %413, i32 0, i32 0
  %415 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %414, align 8
  %416 = getelementptr inbounds %"class.std::__1::__map_const_iterator", %"class.std::__1::__map_const_iterator"* %17, i32 0, i32 0
  %417 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %416, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %415, %"class.std::__1::__tree_end_node"** %417, align 8
  store %"class.std::__1::map"* %350, %"class.std::__1::map"** %18, align 8
  store %"struct.std::__1::pair"* %412, %"struct.std::__1::pair"** %19, align 8
  %418 = load %"class.std::__1::map"*, %"class.std::__1::map"** %18, align 8
  %419 = getelementptr inbounds %"class.std::__1::map", %"class.std::__1::map"* %418, i32 0, i32 0
  %420 = getelementptr inbounds %"class.std::__1::__map_const_iterator", %"class.std::__1::__map_const_iterator"* %17, i32 0, i32 0
  %421 = bitcast %"class.std::__1::__tree_const_iterator"* %21 to i8*
  %422 = bitcast %"class.std::__1::__tree_const_iterator"* %420 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %421, i8* %422, i64 8, i32 8, i1 false)
  %423 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %19, align 8
  %424 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %21, i32 0, i32 0
  %425 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %424, align 8
  %426 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %12, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %425, %"class.std::__1::__tree_end_node"** %426, align 8
  store %"class.std::__1::__tree"* %419, %"class.std::__1::__tree"** %13, align 8
  store %"struct.std::__1::pair"* %423, %"struct.std::__1::pair"** %14, align 8
  %427 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %13, align 8
  %428 = bitcast %"class.std::__1::__tree_const_iterator"* %15 to i8*
  %429 = bitcast %"class.std::__1::__tree_const_iterator"* %12 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %428, i8* %429, i64 8, i32 8, i1 false)
  %430 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %14, align 8
  store %"struct.std::__1::pair"* %430, %"struct.std::__1::pair"** %10, align 8
  %431 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %10, align 8
  %432 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %431, i32 0, i32 0
  %433 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %14, align 8
  %434 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %15, i32 0, i32 0
  %435 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %434, align 8
  %436 = invoke %"class.std::__1::__tree_end_node"* @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE30__emplace_hint_unique_key_argsIS2_JRKNS_4pairIKS2_S8_EEEEENS_15__tree_iteratorIS9_PNS_11__tree_nodeIS9_PvEElEENS_21__tree_const_iteratorIS9_SQ_lEERKT_DpOT0_(%"class.std::__1::__tree"* %427, %"class.std::__1::__tree_end_node"* %435, i32* dereferenceable(4) %432, %"struct.std::__1::pair"* dereferenceable(32) %433)
          to label %437 unwind label %460

; <label>:437:                                    ; preds = %397
  %438 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %11, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %436, %"class.std::__1::__tree_end_node"** %438, align 8
  %439 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %11, i32 0, i32 0
  %440 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %439, align 8
  %441 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %20, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %440, %"class.std::__1::__tree_end_node"** %441, align 8
  %442 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %20, i32 0, i32 0
  %443 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %442, align 8
  %444 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %8, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %443, %"class.std::__1::__tree_end_node"** %444, align 8
  store %"class.std::__1::__map_iterator"* %16, %"class.std::__1::__map_iterator"** %9, align 8
  %445 = load %"class.std::__1::__map_iterator"*, %"class.std::__1::__map_iterator"** %9, align 8
  %446 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %8, i32 0, i32 0
  %447 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %446, align 8
  %448 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %6, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %447, %"class.std::__1::__tree_end_node"** %448, align 8
  store %"class.std::__1::__map_iterator"* %445, %"class.std::__1::__map_iterator"** %7, align 8
  %449 = load %"class.std::__1::__map_iterator"*, %"class.std::__1::__map_iterator"** %7, align 8
  %450 = getelementptr inbounds %"class.std::__1::__map_iterator", %"class.std::__1::__map_iterator"* %449, i32 0, i32 0
  %451 = bitcast %"class.std::__1::__tree_iterator"* %450 to i8*
  %452 = bitcast %"class.std::__1::__tree_iterator"* %6 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %451, i8* %452, i64 8, i32 8, i1 false) #2
  %453 = getelementptr inbounds %"class.std::__1::__map_iterator", %"class.std::__1::__map_iterator"* %16, i32 0, i32 0
  %454 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %453, i32 0, i32 0
  %455 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %454, align 8
  %456 = getelementptr inbounds %"class.std::__1::__map_iterator", %"class.std::__1::__map_iterator"* %48, i32 0, i32 0
  %457 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %456, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %455, %"class.std::__1::__tree_end_node"** %457, align 8
  %458 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %43, align 8
  %459 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %458, i32 1
  store %"struct.std::__1::pair"* %459, %"struct.std::__1::pair"** %43, align 8
  br label %393

; <label>:460:                                    ; preds = %397
  %461 = landingpad { i8*, i32 }
          cleanup
  %462 = extractvalue { i8*, i32 } %461, 0
  store i8* %462, i8** %58, align 8
  %463 = extractvalue { i8*, i32 } %461, 1
  store i32 %463, i32* %59, align 4
  call void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEED1Ev(%"class.std::__1::__tree"* %336) #2
  %464 = load i8*, i8** %58, align 8
  %465 = load i32, i32* %59, align 4
  %466 = insertvalue { i8*, i32 } undef, i8* %464, 0
  %467 = insertvalue { i8*, i32 } %466, i32 %465, 1
  br label %491

; <label>:468:                                    ; preds = %393
  br label %469

; <label>:469:                                    ; preds = %468
  %470 = getelementptr inbounds [5 x %"struct.std::__1::pair"], [5 x %"struct.std::__1::pair"]* %144, i32 0, i32 0
  %471 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %470, i64 5
  br label %472

; <label>:472:                                    ; preds = %472, %469
  %473 = phi %"struct.std::__1::pair"* [ %471, %469 ], [ %474, %472 ]
  %474 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %473, i64 -1
  call void @_ZNSt3__14pairIK11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEED1Ev(%"struct.std::__1::pair"* %474) #2
  %475 = icmp eq %"struct.std::__1::pair"* %474, %470
  br i1 %475, label %476, label %472

; <label>:476:                                    ; preds = %472
  %477 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::__1::map"*)* @_ZNSt3__13mapI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_4lessIS1_EENS5_INS_4pairIKS1_S7_EEEEED1Ev to void (i8*)*), i8* bitcast (%"class.std::__1::map"* @_literal_names to i8*), i8* @__dso_handle) #2
  ret void

; <label>:478:                                    ; preds = %311, %281, %279, %249, %247, %217, %215, %185, %183, %0
  %479 = landingpad { i8*, i32 }
          cleanup
  %480 = extractvalue { i8*, i32 } %479, 0
  store i8* %480, i8** %147, align 8
  %481 = extractvalue { i8*, i32 } %479, 1
  store i32 %481, i32* %148, align 4
  %482 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %145, align 8
  %483 = icmp eq %"struct.std::__1::pair"* %154, %482
  br i1 %483, label %488, label %484

; <label>:484:                                    ; preds = %484, %478
  %485 = phi %"struct.std::__1::pair"* [ %482, %478 ], [ %486, %484 ]
  %486 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %485, i64 -1
  call void @_ZNSt3__14pairIK11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEED1Ev(%"struct.std::__1::pair"* %486) #2
  %487 = icmp eq %"struct.std::__1::pair"* %486, %154
  br i1 %487, label %488, label %484

; <label>:488:                                    ; preds = %484, %478
  br label %502
                                                  ; No predecessors!
  %490 = landingpad { i8*, i32 }
          cleanup
  br label %491

; <label>:491:                                    ; preds = %460, %489
  %492 = phi { i8*, i32 } [ %490, %489 ], [ %467, %460 ]
  %493 = extractvalue { i8*, i32 } %492, 0
  store i8* %493, i8** %147, align 8
  %494 = extractvalue { i8*, i32 } %492, 1
  store i32 %494, i32* %148, align 4
  %495 = getelementptr inbounds [5 x %"struct.std::__1::pair"], [5 x %"struct.std::__1::pair"]* %144, i32 0, i32 0
  %496 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %495, i64 5
  br label %497

; <label>:497:                                    ; preds = %497, %491
  %498 = phi %"struct.std::__1::pair"* [ %496, %491 ], [ %499, %497 ]
  %499 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %498, i64 -1
  call void @_ZNSt3__14pairIK11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEED1Ev(%"struct.std::__1::pair"* %499) #2
  %500 = icmp eq %"struct.std::__1::pair"* %499, %495
  br i1 %500, label %501, label %497

; <label>:501:                                    ; preds = %497
  br label %502

; <label>:502:                                    ; preds = %501, %488
  %503 = load i8*, i8** %147, align 8
  %504 = load i32, i32* %148, align 4
  %505 = insertvalue { i8*, i32 } undef, i8* %503, 0
  %506 = insertvalue { i8*, i32 } %505, i32 %504, 1
  resume { i8*, i32 } %506
}

declare i32 @__gxx_personality_v0(...)

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIK11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEED1Ev(%"struct.std::__1::pair"*) unnamed_addr #1 align 2 {
  %2 = alloca %"struct.std::__1::pair"*, align 8
  store %"struct.std::__1::pair"* %0, %"struct.std::__1::pair"** %2, align 8
  %3 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %2, align 8
  call void @_ZNSt3__14pairIK11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEED2Ev(%"struct.std::__1::pair"* %3) #2
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__13mapI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_4lessIS1_EENS5_INS_4pairIKS1_S7_EEEEED1Ev(%"class.std::__1::map"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::map"*, align 8
  store %"class.std::__1::map"* %0, %"class.std::__1::map"** %2, align 8
  %3 = load %"class.std::__1::map"*, %"class.std::__1::map"** %2, align 8
  call void @_ZNSt3__13mapI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_4lessIS1_EENS5_INS_4pairIKS1_S7_EEEEED2Ev(%"class.std::__1::map"* %3) #2
  ret void
}

; Function Attrs: nounwind
declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) #2

; Function Attrs: ssp uwtable
define void @_ZN5TokenC2E9TokenType7Variantiii(%class.Token*, i32, %struct.Variant*, i32, i32, i32) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %7 = alloca %class.Token*, align 8
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i8*
  %13 = alloca i32
  store %class.Token* %0, %class.Token** %7, align 8
  store i32 %1, i32* %8, align 4
  store i32 %3, i32* %9, align 4
  store i32 %4, i32* %10, align 4
  store i32 %5, i32* %11, align 4
  %14 = load %class.Token*, %class.Token** %7, align 8
  %15 = getelementptr inbounds %class.Token, %class.Token* %14, i32 0, i32 0
  call void @_ZN7VariantC1Ev(%struct.Variant* %15)
  %16 = getelementptr inbounds %class.Token, %class.Token* %14, i32 0, i32 2
  %17 = load i32, i32* %9, align 4
  store i32 %17, i32* %16, align 4
  %18 = getelementptr inbounds %class.Token, %class.Token* %14, i32 0, i32 3
  %19 = load i32, i32* %10, align 4
  store i32 %19, i32* %18, align 8
  %20 = getelementptr inbounds %class.Token, %class.Token* %14, i32 0, i32 4
  %21 = load i32, i32* %11, align 4
  store i32 %21, i32* %20, align 4
  %22 = load i32, i32* %8, align 4
  %23 = getelementptr inbounds %class.Token, %class.Token* %14, i32 0, i32 1
  store i32 %22, i32* %23, align 8
  %24 = getelementptr inbounds %class.Token, %class.Token* %14, i32 0, i32 0
  %25 = invoke dereferenceable(264) %struct.Variant* @_ZN7VariantaSERKS_(%struct.Variant* %24, %struct.Variant* dereferenceable(264) %2)
          to label %26 unwind label %27

; <label>:26:                                     ; preds = %6
  ret void

; <label>:27:                                     ; preds = %6
  %28 = landingpad { i8*, i32 }
          cleanup
  %29 = extractvalue { i8*, i32 } %28, 0
  store i8* %29, i8** %12, align 8
  %30 = extractvalue { i8*, i32 } %28, 1
  store i32 %30, i32* %13, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %15) #2
  br label %31

; <label>:31:                                     ; preds = %27
  %32 = load i8*, i8** %12, align 8
  %33 = load i32, i32* %13, align 4
  %34 = insertvalue { i8*, i32 } undef, i8* %32, 0
  %35 = insertvalue { i8*, i32 } %34, i32 %33, 1
  resume { i8*, i32 } %35
}

declare void @_ZN7VariantC1Ev(%struct.Variant*) unnamed_addr #3

declare dereferenceable(264) %struct.Variant* @_ZN7VariantaSERKS_(%struct.Variant*, %struct.Variant* dereferenceable(264)) #3

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN7VariantD1Ev(%struct.Variant*) unnamed_addr #1 align 2 {
  %2 = alloca %struct.Variant*, align 8
  store %struct.Variant* %0, %struct.Variant** %2, align 8
  %3 = load %struct.Variant*, %struct.Variant** %2, align 8
  call void @_ZN7VariantD2Ev(%struct.Variant* %3) #2
  ret void
}

; Function Attrs: ssp uwtable
define void @_ZN5TokenC1E9TokenType7Variantiii(%class.Token*, i32, %struct.Variant*, i32, i32, i32) unnamed_addr #0 align 2 {
  %7 = alloca %class.Token*, align 8
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  store %class.Token* %0, %class.Token** %7, align 8
  store i32 %1, i32* %8, align 4
  store i32 %3, i32* %9, align 4
  store i32 %4, i32* %10, align 4
  store i32 %5, i32* %11, align 4
  %12 = load %class.Token*, %class.Token** %7, align 8
  %13 = load i32, i32* %8, align 4
  %14 = load i32, i32* %9, align 4
  %15 = load i32, i32* %10, align 4
  %16 = load i32, i32* %11, align 4
  call void @_ZN5TokenC2E9TokenType7Variantiii(%class.Token* %12, i32 %13, %struct.Variant* %2, i32 %14, i32 %15, i32 %16)
  ret void
}

; Function Attrs: ssp uwtable
define void @_ZN5Token9get_valueEv(%struct.Variant* noalias sret, %class.Token*) #0 align 2 {
  %3 = alloca %class.Token*, align 8
  store %class.Token* %1, %class.Token** %3, align 8
  %4 = load %class.Token*, %class.Token** %3, align 8
  %5 = getelementptr inbounds %class.Token, %class.Token* %4, i32 0, i32 0
  call void @_ZN7VariantC1ERKS_(%struct.Variant* %0, %struct.Variant* dereferenceable(264) %5)
  ret void
}

declare void @_ZN7VariantC1ERKS_(%struct.Variant*, %struct.Variant* dereferenceable(264)) unnamed_addr #3

; Function Attrs: nounwind ssp uwtable
define i32 @_ZN5Token8get_typeEv(%class.Token*) #4 align 2 {
  %2 = alloca %class.Token*, align 8
  store %class.Token* %0, %class.Token** %2, align 8
  %3 = load %class.Token*, %class.Token** %2, align 8
  %4 = getelementptr inbounds %class.Token, %class.Token* %3, i32 0, i32 1
  %5 = load i32, i32* %4, align 8
  ret i32 %5
}

; Function Attrs: ssp uwtable
define zeroext i1 @_ZN5Token13is_punctuatorEc(%class.Token*, i8 signext) #0 align 2 {
  %3 = alloca %class.Token*, align 8
  %4 = alloca i8, align 1
  store %class.Token* %0, %class.Token** %3, align 8
  store i8 %1, i8* %4, align 1
  %5 = load %class.Token*, %class.Token** %3, align 8
  %6 = getelementptr inbounds %class.Token, %class.Token* %5, i32 0, i32 1
  %7 = load i32, i32* %6, align 8
  %8 = icmp eq i32 %7, 3
  br i1 %8, label %9, label %16

; <label>:9:                                      ; preds = %2
  %10 = getelementptr inbounds %class.Token, %class.Token* %5, i32 0, i32 0
  %11 = call signext i8 @_ZN7Variant3getIcEET_v(%struct.Variant* %10)
  %12 = sext i8 %11 to i32
  %13 = load i8, i8* %4, align 1
  %14 = sext i8 %13 to i32
  %15 = icmp eq i32 %12, %14
  br label %16

; <label>:16:                                     ; preds = %9, %2
  %17 = phi i1 [ false, %2 ], [ %15, %9 ]
  ret i1 %17
}

declare signext i8 @_ZN7Variant3getIcEET_v(%struct.Variant*) #3

; Function Attrs: ssp uwtable
define signext i8 @_ZN5Token14get_punctuatorEv(%class.Token*) #0 align 2 {
  %2 = alloca %class.Token*, align 8
  store %class.Token* %0, %class.Token** %2, align 8
  %3 = load %class.Token*, %class.Token** %2, align 8
  %4 = getelementptr inbounds %class.Token, %class.Token* %3, i32 0, i32 0
  %5 = call signext i8 @_ZN7Variant3getIcEET_v(%struct.Variant* %4)
  ret i8 %5
}

; Function Attrs: ssp uwtable
define zeroext i1 @_ZN5Token10is_keywordENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Token*, %"class.std::__1::basic_string"*) #0 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %3 = alloca i8*, align 8
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %6 = alloca %"class.std::__1::basic_string"*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i8*, align 8
  %9 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %14 = alloca %"class.std::__1::basic_string"*, align 8
  %15 = alloca %"class.std::__1::basic_string"*, align 8
  %16 = alloca %"class.std::__1::basic_string"*, align 8
  %17 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %18 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %19 = alloca %"class.std::__1::basic_string"*, align 8
  %20 = alloca i8*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %23 = alloca %"class.std::__1::basic_string"*, align 8
  %24 = alloca i8*, align 8
  %25 = alloca i8*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %28 = alloca %"class.std::__1::basic_string"*, align 8
  %29 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %30 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %31 = alloca %"class.std::__1::basic_string"*, align 8
  %32 = alloca %"class.std::__1::basic_string"*, align 8
  %33 = alloca %"class.std::__1::basic_string"*, align 8
  %34 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %35 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %36 = alloca %"class.std::__1::basic_string"*, align 8
  %37 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %38 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %39 = alloca %"class.std::__1::basic_string"*, align 8
  %40 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %41 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %42 = alloca %"class.std::__1::basic_string"*, align 8
  %43 = alloca %"class.std::__1::basic_string"*, align 8
  %44 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %45 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %46 = alloca %"class.std::__1::basic_string"*, align 8
  %47 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %48 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %49 = alloca %"class.std::__1::basic_string"*, align 8
  %50 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %51 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %52 = alloca %"class.std::__1::basic_string"*, align 8
  %53 = alloca %"class.std::__1::basic_string"*, align 8
  %54 = alloca i1, align 1
  %55 = alloca %"class.std::__1::basic_string"*, align 8
  %56 = alloca %"class.std::__1::basic_string"*, align 8
  %57 = alloca i64, align 8
  %58 = alloca i8*, align 8
  %59 = alloca i8*, align 8
  %60 = alloca i1, align 1
  %61 = alloca %class.Token*, align 8
  %62 = alloca %"class.std::__1::basic_string", align 8
  %63 = alloca i1, align 1
  store %class.Token* %0, %class.Token** %61, align 8
  %64 = load %class.Token*, %class.Token** %61, align 8
  %65 = getelementptr inbounds %class.Token, %class.Token* %64, i32 0, i32 1
  %66 = load i32, i32* %65, align 8
  %67 = icmp eq i32 %66, 0
  store i1 false, i1* %63, align 1
  br i1 %67, label %68, label %298

; <label>:68:                                     ; preds = %2
  %69 = getelementptr inbounds %class.Token, %class.Token* %64, i32 0, i32 0
  call void @_ZN7Variant3getINSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEEET_v(%"class.std::__1::basic_string"* sret %62, %struct.Variant* %69)
  store i1 true, i1* %63, align 1
  store %"class.std::__1::basic_string"* %62, %"class.std::__1::basic_string"** %55, align 8
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %56, align 8
  %70 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %55, align 8
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %53, align 8
  %71 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %52, align 8
  %72 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8
  %73 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %72, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %73, %"class.std::__1::__compressed_pair.2"** %51, align 8
  %74 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %51, align 8
  %75 = bitcast %"class.std::__1::__compressed_pair.2"* %74 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %75, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %50, align 8
  %76 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %50, align 8
  %77 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %76, i32 0, i32 0
  %78 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %77, i32 0, i32 0
  %79 = bitcast %union.anon* %78 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %80 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %79, i32 0, i32 0
  %81 = bitcast %union.anon.6* %80 to i8*
  %82 = load i8, i8* %81, align 8
  %83 = zext i8 %82 to i32
  %84 = and i32 %83, 1
  %85 = icmp ne i32 %84, 0
  br i1 %85, label %86, label %97

; <label>:86:                                     ; preds = %68
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %46, align 8
  %87 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %46, align 8
  %88 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %87, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %88, %"class.std::__1::__compressed_pair.2"** %45, align 8
  %89 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %45, align 8
  %90 = bitcast %"class.std::__1::__compressed_pair.2"* %89 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %90, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %44, align 8
  %91 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %44, align 8
  %92 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %91, i32 0, i32 0
  %93 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %92, i32 0, i32 0
  %94 = bitcast %union.anon* %93 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %95 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %94, i32 0, i32 1
  %96 = load i64, i64* %95, align 8
  br label %112

; <label>:97:                                     ; preds = %68
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %49, align 8
  %98 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8
  %99 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %98, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %99, %"class.std::__1::__compressed_pair.2"** %48, align 8
  %100 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %48, align 8
  %101 = bitcast %"class.std::__1::__compressed_pair.2"* %100 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %101, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %47, align 8
  %102 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %47, align 8
  %103 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %102, i32 0, i32 0
  %104 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %103, i32 0, i32 0
  %105 = bitcast %union.anon* %104 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %106 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %105, i32 0, i32 0
  %107 = bitcast %union.anon.6* %106 to i8*
  %108 = load i8, i8* %107, align 8
  %109 = zext i8 %108 to i32
  %110 = ashr i32 %109, 1
  %111 = sext i32 %110 to i64
  br label %112

; <label>:112:                                    ; preds = %97, %86
  %113 = phi i64 [ %96, %86 ], [ %111, %97 ]
  store i64 %113, i64* %57, align 8
  %114 = load i64, i64* %57, align 8
  %115 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %56, align 8
  store %"class.std::__1::basic_string"* %115, %"class.std::__1::basic_string"** %43, align 8
  %116 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %43, align 8
  store %"class.std::__1::basic_string"* %116, %"class.std::__1::basic_string"** %42, align 8
  %117 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8
  %118 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %117, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %118, %"class.std::__1::__compressed_pair.2"** %41, align 8
  %119 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %41, align 8
  %120 = bitcast %"class.std::__1::__compressed_pair.2"* %119 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %120, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %40, align 8
  %121 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %40, align 8
  %122 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %121, i32 0, i32 0
  %123 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %122, i32 0, i32 0
  %124 = bitcast %union.anon* %123 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %125 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %124, i32 0, i32 0
  %126 = bitcast %union.anon.6* %125 to i8*
  %127 = load i8, i8* %126, align 8
  %128 = zext i8 %127 to i32
  %129 = and i32 %128, 1
  %130 = icmp ne i32 %129, 0
  br i1 %130, label %131, label %142

; <label>:131:                                    ; preds = %112
  store %"class.std::__1::basic_string"* %116, %"class.std::__1::basic_string"** %36, align 8
  %132 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8
  %133 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %132, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %133, %"class.std::__1::__compressed_pair.2"** %35, align 8
  %134 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %35, align 8
  %135 = bitcast %"class.std::__1::__compressed_pair.2"* %134 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %135, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %34, align 8
  %136 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %34, align 8
  %137 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %136, i32 0, i32 0
  %138 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %137, i32 0, i32 0
  %139 = bitcast %union.anon* %138 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %140 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %139, i32 0, i32 1
  %141 = load i64, i64* %140, align 8
  br label %157

; <label>:142:                                    ; preds = %112
  store %"class.std::__1::basic_string"* %116, %"class.std::__1::basic_string"** %39, align 8
  %143 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8
  %144 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %143, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %144, %"class.std::__1::__compressed_pair.2"** %38, align 8
  %145 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %38, align 8
  %146 = bitcast %"class.std::__1::__compressed_pair.2"* %145 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %146, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %37, align 8
  %147 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %37, align 8
  %148 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %147, i32 0, i32 0
  %149 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %148, i32 0, i32 0
  %150 = bitcast %union.anon* %149 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %151 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %150, i32 0, i32 0
  %152 = bitcast %union.anon.6* %151 to i8*
  %153 = load i8, i8* %152, align 8
  %154 = zext i8 %153 to i32
  %155 = ashr i32 %154, 1
  %156 = sext i32 %155 to i64
  br label %157

; <label>:157:                                    ; preds = %142, %131
  %158 = phi i64 [ %141, %131 ], [ %156, %142 ]
  %159 = icmp ne i64 %114, %158
  br i1 %159, label %160, label %161

; <label>:160:                                    ; preds = %157
  store i1 false, i1* %54, align 1
  br label %296

; <label>:161:                                    ; preds = %157
  %162 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %55, align 8
  store %"class.std::__1::basic_string"* %162, %"class.std::__1::basic_string"** %33, align 8
  %163 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8
  store %"class.std::__1::basic_string"* %163, %"class.std::__1::basic_string"** %32, align 8
  %164 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %32, align 8
  store %"class.std::__1::basic_string"* %164, %"class.std::__1::basic_string"** %31, align 8
  %165 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %31, align 8
  %166 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %165, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %166, %"class.std::__1::__compressed_pair.2"** %30, align 8
  %167 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %30, align 8
  %168 = bitcast %"class.std::__1::__compressed_pair.2"* %167 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %168, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %29, align 8
  %169 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %29, align 8
  %170 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %169, i32 0, i32 0
  %171 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %170, i32 0, i32 0
  %172 = bitcast %union.anon* %171 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %173 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %172, i32 0, i32 0
  %174 = bitcast %union.anon.6* %173 to i8*
  %175 = load i8, i8* %174, align 8
  %176 = zext i8 %175 to i32
  %177 = and i32 %176, 1
  %178 = icmp ne i32 %177, 0
  br i1 %178, label %179, label %190

; <label>:179:                                    ; preds = %161
  store %"class.std::__1::basic_string"* %164, %"class.std::__1::basic_string"** %23, align 8
  %180 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8
  %181 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %180, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %181, %"class.std::__1::__compressed_pair.2"** %22, align 8
  %182 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %22, align 8
  %183 = bitcast %"class.std::__1::__compressed_pair.2"* %182 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %183, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %21, align 8
  %184 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %21, align 8
  %185 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %184, i32 0, i32 0
  %186 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %185, i32 0, i32 0
  %187 = bitcast %union.anon* %186 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %188 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %187, i32 0, i32 2
  %189 = load i8*, i8** %188, align 8
  br label %203

; <label>:190:                                    ; preds = %161
  store %"class.std::__1::basic_string"* %164, %"class.std::__1::basic_string"** %28, align 8
  %191 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8
  %192 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %191, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %192, %"class.std::__1::__compressed_pair.2"** %27, align 8
  %193 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %27, align 8
  %194 = bitcast %"class.std::__1::__compressed_pair.2"* %193 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %194, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %26, align 8
  %195 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %26, align 8
  %196 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %195, i32 0, i32 0
  %197 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %196, i32 0, i32 0
  %198 = bitcast %union.anon* %197 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %199 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %198, i32 0, i32 1
  %200 = getelementptr inbounds [23 x i8], [23 x i8]* %199, i64 0, i64 0
  store i8* %200, i8** %25, align 8
  %201 = load i8*, i8** %25, align 8
  store i8* %201, i8** %24, align 8
  %202 = load i8*, i8** %24, align 8
  br label %203

; <label>:203:                                    ; preds = %190, %179
  %204 = phi i8* [ %189, %179 ], [ %202, %190 ]
  store i8* %204, i8** %20, align 8
  %205 = load i8*, i8** %20, align 8
  store i8* %205, i8** %58, align 8
  %206 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %56, align 8
  store %"class.std::__1::basic_string"* %206, %"class.std::__1::basic_string"** %16, align 8
  %207 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %16, align 8
  store %"class.std::__1::basic_string"* %207, %"class.std::__1::basic_string"** %15, align 8
  %208 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %15, align 8
  store %"class.std::__1::basic_string"* %208, %"class.std::__1::basic_string"** %14, align 8
  %209 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8
  %210 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %209, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %210, %"class.std::__1::__compressed_pair.2"** %13, align 8
  %211 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %13, align 8
  %212 = bitcast %"class.std::__1::__compressed_pair.2"* %211 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %212, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %12, align 8
  %213 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %12, align 8
  %214 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %213, i32 0, i32 0
  %215 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %214, i32 0, i32 0
  %216 = bitcast %union.anon* %215 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %217 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %216, i32 0, i32 0
  %218 = bitcast %union.anon.6* %217 to i8*
  %219 = load i8, i8* %218, align 8
  %220 = zext i8 %219 to i32
  %221 = and i32 %220, 1
  %222 = icmp ne i32 %221, 0
  br i1 %222, label %223, label %234

; <label>:223:                                    ; preds = %203
  store %"class.std::__1::basic_string"* %208, %"class.std::__1::basic_string"** %6, align 8
  %224 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8
  %225 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %224, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %225, %"class.std::__1::__compressed_pair.2"** %5, align 8
  %226 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %5, align 8
  %227 = bitcast %"class.std::__1::__compressed_pair.2"* %226 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %227, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %4, align 8
  %228 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %4, align 8
  %229 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %228, i32 0, i32 0
  %230 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %229, i32 0, i32 0
  %231 = bitcast %union.anon* %230 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %232 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %231, i32 0, i32 2
  %233 = load i8*, i8** %232, align 8
  br label %247

; <label>:234:                                    ; preds = %203
  store %"class.std::__1::basic_string"* %208, %"class.std::__1::basic_string"** %11, align 8
  %235 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8
  %236 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %235, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %236, %"class.std::__1::__compressed_pair.2"** %10, align 8
  %237 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %10, align 8
  %238 = bitcast %"class.std::__1::__compressed_pair.2"* %237 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %238, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %9, align 8
  %239 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %9, align 8
  %240 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %239, i32 0, i32 0
  %241 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %240, i32 0, i32 0
  %242 = bitcast %union.anon* %241 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %243 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %242, i32 0, i32 1
  %244 = getelementptr inbounds [23 x i8], [23 x i8]* %243, i64 0, i64 0
  store i8* %244, i8** %8, align 8
  %245 = load i8*, i8** %8, align 8
  store i8* %245, i8** %7, align 8
  %246 = load i8*, i8** %7, align 8
  br label %247

; <label>:247:                                    ; preds = %234, %223
  %248 = phi i8* [ %233, %223 ], [ %246, %234 ]
  store i8* %248, i8** %3, align 8
  %249 = load i8*, i8** %3, align 8
  store i8* %249, i8** %59, align 8
  %250 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %55, align 8
  store %"class.std::__1::basic_string"* %250, %"class.std::__1::basic_string"** %19, align 8
  %251 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %19, align 8
  %252 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %251, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %252, %"class.std::__1::__compressed_pair.2"** %18, align 8
  %253 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %18, align 8
  %254 = bitcast %"class.std::__1::__compressed_pair.2"* %253 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %254, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %17, align 8
  %255 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %17, align 8
  %256 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %255, i32 0, i32 0
  %257 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %256, i32 0, i32 0
  %258 = bitcast %union.anon* %257 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %259 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %258, i32 0, i32 0
  %260 = bitcast %union.anon.6* %259 to i8*
  %261 = load i8, i8* %260, align 8
  %262 = zext i8 %261 to i32
  %263 = and i32 %262, 1
  %264 = icmp ne i32 %263, 0
  br i1 %264, label %265, label %272

; <label>:265:                                    ; preds = %247
  %266 = load i8*, i8** %58, align 8
  %267 = load i8*, i8** %59, align 8
  %268 = load i64, i64* %57, align 8
  %269 = invoke i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8* %266, i8* %267, i64 %268)
          to label %270 unwind label %293

; <label>:270:                                    ; preds = %265
  %271 = icmp eq i32 %269, 0
  store i1 %271, i1* %54, align 1
  br label %296

; <label>:272:                                    ; preds = %247
  br label %273

; <label>:273:                                    ; preds = %285, %272
  %274 = load i64, i64* %57, align 8
  %275 = icmp ne i64 %274, 0
  br i1 %275, label %276, label %292

; <label>:276:                                    ; preds = %273
  %277 = load i8*, i8** %58, align 8
  %278 = load i8, i8* %277, align 1
  %279 = sext i8 %278 to i32
  %280 = load i8*, i8** %59, align 8
  %281 = load i8, i8* %280, align 1
  %282 = sext i8 %281 to i32
  %283 = icmp ne i32 %279, %282
  br i1 %283, label %284, label %285

; <label>:284:                                    ; preds = %276
  store i1 false, i1* %54, align 1
  br label %296

; <label>:285:                                    ; preds = %276
  %286 = load i64, i64* %57, align 8
  %287 = add i64 %286, -1
  store i64 %287, i64* %57, align 8
  %288 = load i8*, i8** %58, align 8
  %289 = getelementptr inbounds i8, i8* %288, i32 1
  store i8* %289, i8** %58, align 8
  %290 = load i8*, i8** %59, align 8
  %291 = getelementptr inbounds i8, i8* %290, i32 1
  store i8* %291, i8** %59, align 8
  br label %273

; <label>:292:                                    ; preds = %273
  store i1 true, i1* %54, align 1
  br label %296

; <label>:293:                                    ; preds = %265
  %294 = landingpad { i8*, i32 }
          catch i8* null
  %295 = extractvalue { i8*, i32 } %294, 0
  call void @__clang_call_terminate(i8* %295) #11
  unreachable

; <label>:296:                                    ; preds = %160, %270, %284, %292
  %297 = load i1, i1* %54, align 1
  br label %298

; <label>:298:                                    ; preds = %296, %2
  %299 = phi i1 [ false, %2 ], [ %297, %296 ]
  store i1 %299, i1* %60, align 1
  %300 = load i1, i1* %63, align 1
  br i1 %300, label %301, label %302

; <label>:301:                                    ; preds = %298
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %62) #2
  br label %302

; <label>:302:                                    ; preds = %301, %298
  %303 = load i1, i1* %60, align 1
  ret i1 %303
}

declare void @_ZN7Variant3getINSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEEET_v(%"class.std::__1::basic_string"* sret, %struct.Variant*) #3

; Function Attrs: nounwind
declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"*) unnamed_addr #5

; Function Attrs: nounwind ssp uwtable
define zeroext i1 @_ZN5Token10is_literalE11LiteralType(%class.Token*, i32) #4 align 2 {
  %3 = alloca %class.Token*, align 8
  %4 = alloca i32, align 4
  store %class.Token* %0, %class.Token** %3, align 8
  store i32 %1, i32* %4, align 4
  %5 = load %class.Token*, %class.Token** %3, align 8
  %6 = getelementptr inbounds %class.Token, %class.Token* %5, i32 0, i32 1
  %7 = load i32, i32* %6, align 8
  %8 = icmp eq i32 %7, 5
  br i1 %8, label %9, label %14

; <label>:9:                                      ; preds = %2
  %10 = getelementptr inbounds %class.Token, %class.Token* %5, i32 0, i32 6
  %11 = load i32, i32* %10, align 4
  %12 = load i32, i32* %4, align 4
  %13 = icmp eq i32 %11, %12
  br label %14

; <label>:14:                                     ; preds = %9, %2
  %15 = phi i1 [ false, %2 ], [ %13, %9 ]
  ret i1 %15
}

; Function Attrs: ssp uwtable
define zeroext i1 @_ZN5Token11is_operatorENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Token*, %"class.std::__1::basic_string"*) #0 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %3 = alloca i8*, align 8
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %6 = alloca %"class.std::__1::basic_string"*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i8*, align 8
  %9 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %10 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %14 = alloca %"class.std::__1::basic_string"*, align 8
  %15 = alloca %"class.std::__1::basic_string"*, align 8
  %16 = alloca %"class.std::__1::basic_string"*, align 8
  %17 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %18 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %19 = alloca %"class.std::__1::basic_string"*, align 8
  %20 = alloca i8*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %23 = alloca %"class.std::__1::basic_string"*, align 8
  %24 = alloca i8*, align 8
  %25 = alloca i8*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %28 = alloca %"class.std::__1::basic_string"*, align 8
  %29 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %30 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %31 = alloca %"class.std::__1::basic_string"*, align 8
  %32 = alloca %"class.std::__1::basic_string"*, align 8
  %33 = alloca %"class.std::__1::basic_string"*, align 8
  %34 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %35 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %36 = alloca %"class.std::__1::basic_string"*, align 8
  %37 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %38 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %39 = alloca %"class.std::__1::basic_string"*, align 8
  %40 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %41 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %42 = alloca %"class.std::__1::basic_string"*, align 8
  %43 = alloca %"class.std::__1::basic_string"*, align 8
  %44 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %45 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %46 = alloca %"class.std::__1::basic_string"*, align 8
  %47 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %48 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %49 = alloca %"class.std::__1::basic_string"*, align 8
  %50 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.3"*, align 8
  %51 = alloca %"class.std::__1::__compressed_pair.2"*, align 8
  %52 = alloca %"class.std::__1::basic_string"*, align 8
  %53 = alloca %"class.std::__1::basic_string"*, align 8
  %54 = alloca i1, align 1
  %55 = alloca %"class.std::__1::basic_string"*, align 8
  %56 = alloca %"class.std::__1::basic_string"*, align 8
  %57 = alloca i64, align 8
  %58 = alloca i8*, align 8
  %59 = alloca i8*, align 8
  %60 = alloca i1, align 1
  %61 = alloca %class.Token*, align 8
  %62 = alloca %"class.std::__1::basic_string", align 8
  %63 = alloca i1, align 1
  store %class.Token* %0, %class.Token** %61, align 8
  %64 = load %class.Token*, %class.Token** %61, align 8
  %65 = getelementptr inbounds %class.Token, %class.Token* %64, i32 0, i32 1
  %66 = load i32, i32* %65, align 8
  %67 = icmp eq i32 %66, 4
  store i1 false, i1* %63, align 1
  br i1 %67, label %68, label %298

; <label>:68:                                     ; preds = %2
  %69 = getelementptr inbounds %class.Token, %class.Token* %64, i32 0, i32 0
  call void @_ZN7Variant3getINSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEEET_v(%"class.std::__1::basic_string"* sret %62, %struct.Variant* %69)
  store i1 true, i1* %63, align 1
  store %"class.std::__1::basic_string"* %62, %"class.std::__1::basic_string"** %55, align 8
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %56, align 8
  %70 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %55, align 8
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %53, align 8
  %71 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %52, align 8
  %72 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8
  %73 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %72, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %73, %"class.std::__1::__compressed_pair.2"** %51, align 8
  %74 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %51, align 8
  %75 = bitcast %"class.std::__1::__compressed_pair.2"* %74 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %75, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %50, align 8
  %76 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %50, align 8
  %77 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %76, i32 0, i32 0
  %78 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %77, i32 0, i32 0
  %79 = bitcast %union.anon* %78 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %80 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %79, i32 0, i32 0
  %81 = bitcast %union.anon.6* %80 to i8*
  %82 = load i8, i8* %81, align 8
  %83 = zext i8 %82 to i32
  %84 = and i32 %83, 1
  %85 = icmp ne i32 %84, 0
  br i1 %85, label %86, label %97

; <label>:86:                                     ; preds = %68
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %46, align 8
  %87 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %46, align 8
  %88 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %87, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %88, %"class.std::__1::__compressed_pair.2"** %45, align 8
  %89 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %45, align 8
  %90 = bitcast %"class.std::__1::__compressed_pair.2"* %89 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %90, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %44, align 8
  %91 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %44, align 8
  %92 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %91, i32 0, i32 0
  %93 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %92, i32 0, i32 0
  %94 = bitcast %union.anon* %93 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %95 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %94, i32 0, i32 1
  %96 = load i64, i64* %95, align 8
  br label %112

; <label>:97:                                     ; preds = %68
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %49, align 8
  %98 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8
  %99 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %98, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %99, %"class.std::__1::__compressed_pair.2"** %48, align 8
  %100 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %48, align 8
  %101 = bitcast %"class.std::__1::__compressed_pair.2"* %100 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %101, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %47, align 8
  %102 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %47, align 8
  %103 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %102, i32 0, i32 0
  %104 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %103, i32 0, i32 0
  %105 = bitcast %union.anon* %104 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %106 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %105, i32 0, i32 0
  %107 = bitcast %union.anon.6* %106 to i8*
  %108 = load i8, i8* %107, align 8
  %109 = zext i8 %108 to i32
  %110 = ashr i32 %109, 1
  %111 = sext i32 %110 to i64
  br label %112

; <label>:112:                                    ; preds = %97, %86
  %113 = phi i64 [ %96, %86 ], [ %111, %97 ]
  store i64 %113, i64* %57, align 8
  %114 = load i64, i64* %57, align 8
  %115 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %56, align 8
  store %"class.std::__1::basic_string"* %115, %"class.std::__1::basic_string"** %43, align 8
  %116 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %43, align 8
  store %"class.std::__1::basic_string"* %116, %"class.std::__1::basic_string"** %42, align 8
  %117 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8
  %118 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %117, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %118, %"class.std::__1::__compressed_pair.2"** %41, align 8
  %119 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %41, align 8
  %120 = bitcast %"class.std::__1::__compressed_pair.2"* %119 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %120, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %40, align 8
  %121 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %40, align 8
  %122 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %121, i32 0, i32 0
  %123 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %122, i32 0, i32 0
  %124 = bitcast %union.anon* %123 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %125 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %124, i32 0, i32 0
  %126 = bitcast %union.anon.6* %125 to i8*
  %127 = load i8, i8* %126, align 8
  %128 = zext i8 %127 to i32
  %129 = and i32 %128, 1
  %130 = icmp ne i32 %129, 0
  br i1 %130, label %131, label %142

; <label>:131:                                    ; preds = %112
  store %"class.std::__1::basic_string"* %116, %"class.std::__1::basic_string"** %36, align 8
  %132 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8
  %133 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %132, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %133, %"class.std::__1::__compressed_pair.2"** %35, align 8
  %134 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %35, align 8
  %135 = bitcast %"class.std::__1::__compressed_pair.2"* %134 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %135, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %34, align 8
  %136 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %34, align 8
  %137 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %136, i32 0, i32 0
  %138 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %137, i32 0, i32 0
  %139 = bitcast %union.anon* %138 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %140 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %139, i32 0, i32 1
  %141 = load i64, i64* %140, align 8
  br label %157

; <label>:142:                                    ; preds = %112
  store %"class.std::__1::basic_string"* %116, %"class.std::__1::basic_string"** %39, align 8
  %143 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8
  %144 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %143, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %144, %"class.std::__1::__compressed_pair.2"** %38, align 8
  %145 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %38, align 8
  %146 = bitcast %"class.std::__1::__compressed_pair.2"* %145 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %146, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %37, align 8
  %147 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %37, align 8
  %148 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %147, i32 0, i32 0
  %149 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %148, i32 0, i32 0
  %150 = bitcast %union.anon* %149 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %151 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %150, i32 0, i32 0
  %152 = bitcast %union.anon.6* %151 to i8*
  %153 = load i8, i8* %152, align 8
  %154 = zext i8 %153 to i32
  %155 = ashr i32 %154, 1
  %156 = sext i32 %155 to i64
  br label %157

; <label>:157:                                    ; preds = %142, %131
  %158 = phi i64 [ %141, %131 ], [ %156, %142 ]
  %159 = icmp ne i64 %114, %158
  br i1 %159, label %160, label %161

; <label>:160:                                    ; preds = %157
  store i1 false, i1* %54, align 1
  br label %296

; <label>:161:                                    ; preds = %157
  %162 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %55, align 8
  store %"class.std::__1::basic_string"* %162, %"class.std::__1::basic_string"** %33, align 8
  %163 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8
  store %"class.std::__1::basic_string"* %163, %"class.std::__1::basic_string"** %32, align 8
  %164 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %32, align 8
  store %"class.std::__1::basic_string"* %164, %"class.std::__1::basic_string"** %31, align 8
  %165 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %31, align 8
  %166 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %165, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %166, %"class.std::__1::__compressed_pair.2"** %30, align 8
  %167 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %30, align 8
  %168 = bitcast %"class.std::__1::__compressed_pair.2"* %167 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %168, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %29, align 8
  %169 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %29, align 8
  %170 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %169, i32 0, i32 0
  %171 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %170, i32 0, i32 0
  %172 = bitcast %union.anon* %171 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %173 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %172, i32 0, i32 0
  %174 = bitcast %union.anon.6* %173 to i8*
  %175 = load i8, i8* %174, align 8
  %176 = zext i8 %175 to i32
  %177 = and i32 %176, 1
  %178 = icmp ne i32 %177, 0
  br i1 %178, label %179, label %190

; <label>:179:                                    ; preds = %161
  store %"class.std::__1::basic_string"* %164, %"class.std::__1::basic_string"** %23, align 8
  %180 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8
  %181 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %180, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %181, %"class.std::__1::__compressed_pair.2"** %22, align 8
  %182 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %22, align 8
  %183 = bitcast %"class.std::__1::__compressed_pair.2"* %182 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %183, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %21, align 8
  %184 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %21, align 8
  %185 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %184, i32 0, i32 0
  %186 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %185, i32 0, i32 0
  %187 = bitcast %union.anon* %186 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %188 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %187, i32 0, i32 2
  %189 = load i8*, i8** %188, align 8
  br label %203

; <label>:190:                                    ; preds = %161
  store %"class.std::__1::basic_string"* %164, %"class.std::__1::basic_string"** %28, align 8
  %191 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8
  %192 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %191, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %192, %"class.std::__1::__compressed_pair.2"** %27, align 8
  %193 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %27, align 8
  %194 = bitcast %"class.std::__1::__compressed_pair.2"* %193 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %194, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %26, align 8
  %195 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %26, align 8
  %196 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %195, i32 0, i32 0
  %197 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %196, i32 0, i32 0
  %198 = bitcast %union.anon* %197 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %199 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %198, i32 0, i32 1
  %200 = getelementptr inbounds [23 x i8], [23 x i8]* %199, i64 0, i64 0
  store i8* %200, i8** %25, align 8
  %201 = load i8*, i8** %25, align 8
  store i8* %201, i8** %24, align 8
  %202 = load i8*, i8** %24, align 8
  br label %203

; <label>:203:                                    ; preds = %190, %179
  %204 = phi i8* [ %189, %179 ], [ %202, %190 ]
  store i8* %204, i8** %20, align 8
  %205 = load i8*, i8** %20, align 8
  store i8* %205, i8** %58, align 8
  %206 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %56, align 8
  store %"class.std::__1::basic_string"* %206, %"class.std::__1::basic_string"** %16, align 8
  %207 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %16, align 8
  store %"class.std::__1::basic_string"* %207, %"class.std::__1::basic_string"** %15, align 8
  %208 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %15, align 8
  store %"class.std::__1::basic_string"* %208, %"class.std::__1::basic_string"** %14, align 8
  %209 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8
  %210 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %209, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %210, %"class.std::__1::__compressed_pair.2"** %13, align 8
  %211 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %13, align 8
  %212 = bitcast %"class.std::__1::__compressed_pair.2"* %211 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %212, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %12, align 8
  %213 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %12, align 8
  %214 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %213, i32 0, i32 0
  %215 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %214, i32 0, i32 0
  %216 = bitcast %union.anon* %215 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %217 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %216, i32 0, i32 0
  %218 = bitcast %union.anon.6* %217 to i8*
  %219 = load i8, i8* %218, align 8
  %220 = zext i8 %219 to i32
  %221 = and i32 %220, 1
  %222 = icmp ne i32 %221, 0
  br i1 %222, label %223, label %234

; <label>:223:                                    ; preds = %203
  store %"class.std::__1::basic_string"* %208, %"class.std::__1::basic_string"** %6, align 8
  %224 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8
  %225 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %224, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %225, %"class.std::__1::__compressed_pair.2"** %5, align 8
  %226 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %5, align 8
  %227 = bitcast %"class.std::__1::__compressed_pair.2"* %226 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %227, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %4, align 8
  %228 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %4, align 8
  %229 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %228, i32 0, i32 0
  %230 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %229, i32 0, i32 0
  %231 = bitcast %union.anon* %230 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %232 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %231, i32 0, i32 2
  %233 = load i8*, i8** %232, align 8
  br label %247

; <label>:234:                                    ; preds = %203
  store %"class.std::__1::basic_string"* %208, %"class.std::__1::basic_string"** %11, align 8
  %235 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8
  %236 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %235, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %236, %"class.std::__1::__compressed_pair.2"** %10, align 8
  %237 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %10, align 8
  %238 = bitcast %"class.std::__1::__compressed_pair.2"* %237 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %238, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %9, align 8
  %239 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %9, align 8
  %240 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %239, i32 0, i32 0
  %241 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %240, i32 0, i32 0
  %242 = bitcast %union.anon* %241 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %243 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %242, i32 0, i32 1
  %244 = getelementptr inbounds [23 x i8], [23 x i8]* %243, i64 0, i64 0
  store i8* %244, i8** %8, align 8
  %245 = load i8*, i8** %8, align 8
  store i8* %245, i8** %7, align 8
  %246 = load i8*, i8** %7, align 8
  br label %247

; <label>:247:                                    ; preds = %234, %223
  %248 = phi i8* [ %233, %223 ], [ %246, %234 ]
  store i8* %248, i8** %3, align 8
  %249 = load i8*, i8** %3, align 8
  store i8* %249, i8** %59, align 8
  %250 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %55, align 8
  store %"class.std::__1::basic_string"* %250, %"class.std::__1::basic_string"** %19, align 8
  %251 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %19, align 8
  %252 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %251, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.2"* %252, %"class.std::__1::__compressed_pair.2"** %18, align 8
  %253 = load %"class.std::__1::__compressed_pair.2"*, %"class.std::__1::__compressed_pair.2"** %18, align 8
  %254 = bitcast %"class.std::__1::__compressed_pair.2"* %253 to %"class.std::__1::__libcpp_compressed_pair_imp.3"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.3"* %254, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %17, align 8
  %255 = load %"class.std::__1::__libcpp_compressed_pair_imp.3"*, %"class.std::__1::__libcpp_compressed_pair_imp.3"** %17, align 8
  %256 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.3", %"class.std::__1::__libcpp_compressed_pair_imp.3"* %255, i32 0, i32 0
  %257 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %256, i32 0, i32 0
  %258 = bitcast %union.anon* %257 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %259 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %258, i32 0, i32 0
  %260 = bitcast %union.anon.6* %259 to i8*
  %261 = load i8, i8* %260, align 8
  %262 = zext i8 %261 to i32
  %263 = and i32 %262, 1
  %264 = icmp ne i32 %263, 0
  br i1 %264, label %265, label %272

; <label>:265:                                    ; preds = %247
  %266 = load i8*, i8** %58, align 8
  %267 = load i8*, i8** %59, align 8
  %268 = load i64, i64* %57, align 8
  %269 = invoke i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8* %266, i8* %267, i64 %268)
          to label %270 unwind label %293

; <label>:270:                                    ; preds = %265
  %271 = icmp eq i32 %269, 0
  store i1 %271, i1* %54, align 1
  br label %296

; <label>:272:                                    ; preds = %247
  br label %273

; <label>:273:                                    ; preds = %285, %272
  %274 = load i64, i64* %57, align 8
  %275 = icmp ne i64 %274, 0
  br i1 %275, label %276, label %292

; <label>:276:                                    ; preds = %273
  %277 = load i8*, i8** %58, align 8
  %278 = load i8, i8* %277, align 1
  %279 = sext i8 %278 to i32
  %280 = load i8*, i8** %59, align 8
  %281 = load i8, i8* %280, align 1
  %282 = sext i8 %281 to i32
  %283 = icmp ne i32 %279, %282
  br i1 %283, label %284, label %285

; <label>:284:                                    ; preds = %276
  store i1 false, i1* %54, align 1
  br label %296

; <label>:285:                                    ; preds = %276
  %286 = load i64, i64* %57, align 8
  %287 = add i64 %286, -1
  store i64 %287, i64* %57, align 8
  %288 = load i8*, i8** %58, align 8
  %289 = getelementptr inbounds i8, i8* %288, i32 1
  store i8* %289, i8** %58, align 8
  %290 = load i8*, i8** %59, align 8
  %291 = getelementptr inbounds i8, i8* %290, i32 1
  store i8* %291, i8** %59, align 8
  br label %273

; <label>:292:                                    ; preds = %273
  store i1 true, i1* %54, align 1
  br label %296

; <label>:293:                                    ; preds = %265
  %294 = landingpad { i8*, i32 }
          catch i8* null
  %295 = extractvalue { i8*, i32 } %294, 0
  call void @__clang_call_terminate(i8* %295) #11
  unreachable

; <label>:296:                                    ; preds = %160, %270, %284, %292
  %297 = load i1, i1* %54, align 1
  br label %298

; <label>:298:                                    ; preds = %296, %2
  %299 = phi i1 [ false, %2 ], [ %297, %296 ]
  store i1 %299, i1* %60, align 1
  %300 = load i1, i1* %63, align 1
  br i1 %300, label %301, label %302

; <label>:301:                                    ; preds = %298
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %62) #2
  br label %302

; <label>:302:                                    ; preds = %301, %298
  %303 = load i1, i1* %60, align 1
  ret i1 %303
}

; Function Attrs: ssp uwtable
define zeroext i1 @_ZN5Token12is_separatorEv(%class.Token*) #0 align 2 {
  %2 = alloca %class.Token*, align 8
  store %class.Token* %0, %class.Token** %2, align 8
  %3 = load %class.Token*, %class.Token** %2, align 8
  %4 = getelementptr inbounds %class.Token, %class.Token* %3, i32 0, i32 1
  %5 = load i32, i32* %4, align 8
  %6 = icmp eq i32 %5, 3
  br i1 %6, label %7, label %19

; <label>:7:                                      ; preds = %1
  %8 = getelementptr inbounds %class.Token, %class.Token* %3, i32 0, i32 0
  %9 = call signext i8 @_ZN7Variant3getIcEET_v(%struct.Variant* %8)
  %10 = sext i8 %9 to i32
  %11 = icmp eq i32 %10, 10
  br i1 %11, label %17, label %12

; <label>:12:                                     ; preds = %7
  %13 = getelementptr inbounds %class.Token, %class.Token* %3, i32 0, i32 0
  %14 = call signext i8 @_ZN7Variant3getIcEET_v(%struct.Variant* %13)
  %15 = sext i8 %14 to i32
  %16 = icmp eq i32 %15, 59
  br label %17

; <label>:17:                                     ; preds = %12, %7
  %18 = phi i1 [ true, %7 ], [ %16, %12 ]
  br label %19

; <label>:19:                                     ; preds = %17, %1
  %20 = phi i1 [ false, %1 ], [ %18, %17 ]
  ret i1 %20
}

; Function Attrs: ssp uwtable
define void @_ZN5Token16set_literal_typeE11LiteralType(%class.Token* noalias sret, %class.Token*, i32) #0 align 2 {
  %4 = alloca %class.Token*, align 8
  %5 = alloca i32, align 4
  store %class.Token* %1, %class.Token** %4, align 8
  store i32 %2, i32* %5, align 4
  %6 = load %class.Token*, %class.Token** %4, align 8
  %7 = load i32, i32* %5, align 4
  %8 = getelementptr inbounds %class.Token, %class.Token* %6, i32 0, i32 6
  store i32 %7, i32* %8, align 4
  call void @_ZN5TokenC1ERKS_(%class.Token* %0, %class.Token* dereferenceable(288) %6)
  ret void
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZN5TokenC1ERKS_(%class.Token*, %class.Token* dereferenceable(288)) unnamed_addr #6 align 2 {
  %3 = alloca %class.Token*, align 8
  %4 = alloca %class.Token*, align 8
  store %class.Token* %0, %class.Token** %3, align 8
  store %class.Token* %1, %class.Token** %4, align 8
  %5 = load %class.Token*, %class.Token** %3, align 8
  %6 = load %class.Token*, %class.Token** %4, align 8
  call void @_ZN5TokenC2ERKS_(%class.Token* %5, %class.Token* dereferenceable(288) %6)
  ret void
}

; Function Attrs: ssp uwtable
define void @_ZN5Token17set_operator_typeE12OperatorType(%class.Token* noalias sret, %class.Token*, i32) #0 align 2 {
  %4 = alloca %class.Token*, align 8
  %5 = alloca i32, align 4
  store %class.Token* %1, %class.Token** %4, align 8
  store i32 %2, i32* %5, align 4
  %6 = load %class.Token*, %class.Token** %4, align 8
  %7 = load i32, i32* %5, align 4
  %8 = getelementptr inbounds %class.Token, %class.Token* %6, i32 0, i32 5
  store i32 %7, i32* %8, align 8
  call void @_ZN5TokenC1ERKS_(%class.Token* %0, %class.Token* dereferenceable(288) %6)
  ret void
}

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcm(%"class.std::__1::basic_string"*, i8*, i64) #3

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8*) #6 align 2 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = load i8*, i8** %2, align 8
  %4 = call i64 @strlen(i8* %3)
  ret i64 %4
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #7

declare i64 @strlen(i8*) #3

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIK11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEED2Ev(%"struct.std::__1::pair"*) unnamed_addr #1 align 2 {
  %2 = alloca %"struct.std::__1::pair"*, align 8
  store %"struct.std::__1::pair"* %0, %"struct.std::__1::pair"** %2, align 8
  %3 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %2, align 8
  %4 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %3, i32 0, i32 1
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %4) #2
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__13mapI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_4lessIS1_EENS5_INS_4pairIKS1_S7_EEEEED2Ev(%"class.std::__1::map"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::map"*, align 8
  store %"class.std::__1::map"* %0, %"class.std::__1::map"** %2, align 8
  %3 = load %"class.std::__1::map"*, %"class.std::__1::map"** %2, align 8
  %4 = getelementptr inbounds %"class.std::__1::map", %"class.std::__1::map"* %3, i32 0, i32 0
  call void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEED1Ev(%"class.std::__1::__tree"* %4) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEED1Ev(%"class.std::__1::__tree"*) unnamed_addr #4 align 2 {
  %2 = alloca %"class.std::__1::__tree"*, align 8
  store %"class.std::__1::__tree"* %0, %"class.std::__1::__tree"** %2, align 8
  %3 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %2, align 8
  call void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEED2Ev(%"class.std::__1::__tree"* %3) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEED2Ev(%"class.std::__1::__tree"*) unnamed_addr #4 align 2 {
  %2 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %3 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %6 = alloca %"class.std::__1::__tree"*, align 8
  %7 = alloca %"class.std::__1::__tree"*, align 8
  %8 = alloca %"class.std::__1::__tree"*, align 8
  store %"class.std::__1::__tree"* %0, %"class.std::__1::__tree"** %8, align 8
  %9 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %8, align 8
  store %"class.std::__1::__tree"* %9, %"class.std::__1::__tree"** %7, align 8
  %10 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %7, align 8
  store %"class.std::__1::__tree"* %10, %"class.std::__1::__tree"** %6, align 8
  %11 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %6, align 8
  %12 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %11, i32 0, i32 1
  store %"class.std::__1::__compressed_pair"* %12, %"class.std::__1::__compressed_pair"** %5, align 8
  %13 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8
  %14 = bitcast %"class.std::__1::__compressed_pair"* %13 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %14, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8
  %15 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8
  %16 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %15, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %16, %"class.std::__1::__tree_end_node"** %3, align 8
  %17 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %3, align 8
  store %"class.std::__1::__tree_end_node"* %17, %"class.std::__1::__tree_end_node"** %2, align 8
  %18 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %2, align 8
  %19 = bitcast %"class.std::__1::__tree_end_node"* %18 to i8*
  %20 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %18, i32 0, i32 0
  %21 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %20, align 8
  %22 = bitcast %"class.std::__1::__tree_node_base"* %21 to %"class.std::__1::__tree_node"*
  call void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE7destroyEPNS_11__tree_nodeIS9_PvEE(%"class.std::__1::__tree"* %9, %"class.std::__1::__tree_node"* %22) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE7destroyEPNS_11__tree_nodeIS9_PvEE(%"class.std::__1::__tree"*, %"class.std::__1::__tree_node"*) #4 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::allocator"*, align 8
  %4 = alloca %"struct.std::__1::pair"*, align 8
  %5 = alloca %"class.std::__1::allocator"*, align 8
  %6 = alloca %"struct.std::__1::pair"*, align 8
  %7 = alloca %"struct.std::__1::integral_constant", align 1
  %8 = alloca %"struct.std::__1::__has_destroy", align 1
  %9 = alloca %"struct.std::__1::pair"*, align 8
  %10 = alloca %"union.std::__1::__value_type"*, align 8
  %11 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %12 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %13 = alloca %"class.std::__1::__tree"*, align 8
  %14 = alloca i8*, align 8
  %15 = alloca %"class.std::__1::allocator"*, align 8
  %16 = alloca %"class.std::__1::__tree_node"*, align 8
  %17 = alloca i64, align 8
  %18 = alloca %"class.std::__1::allocator"*, align 8
  %19 = alloca %"class.std::__1::__tree_node"*, align 8
  %20 = alloca i64, align 8
  %21 = alloca %"class.std::__1::__tree"*, align 8
  %22 = alloca %"class.std::__1::__tree_node"*, align 8
  %23 = alloca %"class.std::__1::allocator"*, align 8
  store %"class.std::__1::__tree"* %0, %"class.std::__1::__tree"** %21, align 8
  store %"class.std::__1::__tree_node"* %1, %"class.std::__1::__tree_node"** %22, align 8
  %24 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %21, align 8
  %25 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %22, align 8
  %26 = icmp ne %"class.std::__1::__tree_node"* %25, null
  br i1 %26, label %27, label %66

; <label>:27:                                     ; preds = %2
  %28 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %22, align 8
  %29 = bitcast %"class.std::__1::__tree_node"* %28 to %"class.std::__1::__tree_end_node"*
  %30 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %29, i32 0, i32 0
  %31 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %30, align 8
  %32 = bitcast %"class.std::__1::__tree_node_base"* %31 to %"class.std::__1::__tree_node"*
  call void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE7destroyEPNS_11__tree_nodeIS9_PvEE(%"class.std::__1::__tree"* %24, %"class.std::__1::__tree_node"* %32) #2
  %33 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %22, align 8
  %34 = bitcast %"class.std::__1::__tree_node"* %33 to %"class.std::__1::__tree_node_base"*
  %35 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %34, i32 0, i32 1
  %36 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %35, align 8
  %37 = bitcast %"class.std::__1::__tree_node_base"* %36 to %"class.std::__1::__tree_node"*
  call void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE7destroyEPNS_11__tree_nodeIS9_PvEE(%"class.std::__1::__tree"* %24, %"class.std::__1::__tree_node"* %37) #2
  store %"class.std::__1::__tree"* %24, %"class.std::__1::__tree"** %13, align 8
  %38 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %13, align 8
  %39 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %38, i32 0, i32 1
  store %"class.std::__1::__compressed_pair"* %39, %"class.std::__1::__compressed_pair"** %12, align 8
  %40 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %12, align 8
  %41 = bitcast %"class.std::__1::__compressed_pair"* %40 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %41, %"class.std::__1::__libcpp_compressed_pair_imp"** %11, align 8
  %42 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %11, align 8
  %43 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %42 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %43, %"class.std::__1::allocator"** %23, align 8
  %44 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %23, align 8
  %45 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %22, align 8
  %46 = getelementptr inbounds %"class.std::__1::__tree_node", %"class.std::__1::__tree_node"* %45, i32 0, i32 1
  store %"union.std::__1::__value_type"* %46, %"union.std::__1::__value_type"** %10, align 8
  %47 = load %"union.std::__1::__value_type"*, %"union.std::__1::__value_type"** %10, align 8
  %48 = bitcast %"union.std::__1::__value_type"* %47 to %"struct.std::__1::pair"*
  store %"struct.std::__1::pair"* %48, %"struct.std::__1::pair"** %9, align 8
  %49 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %9, align 8
  %50 = bitcast %"struct.std::__1::pair"* %49 to i8*
  br label %51

; <label>:51:                                     ; preds = %27
  store %"class.std::__1::allocator"* %44, %"class.std::__1::allocator"** %5, align 8
  store %"struct.std::__1::pair"* %49, %"struct.std::__1::pair"** %6, align 8
  %52 = bitcast %"struct.std::__1::__has_destroy"* %8 to %"struct.std::__1::integral_constant"*
  %53 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %5, align 8
  %54 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %6, align 8
  store %"class.std::__1::allocator"* %53, %"class.std::__1::allocator"** %3, align 8
  store %"struct.std::__1::pair"* %54, %"struct.std::__1::pair"** %4, align 8
  %55 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %4, align 8
  call void @_ZNSt3__14pairIK11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEED1Ev(%"struct.std::__1::pair"* %55) #2
  br label %56

; <label>:56:                                     ; preds = %51
  %57 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %23, align 8
  %58 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %22, align 8
  store %"class.std::__1::allocator"* %57, %"class.std::__1::allocator"** %18, align 8
  store %"class.std::__1::__tree_node"* %58, %"class.std::__1::__tree_node"** %19, align 8
  store i64 1, i64* %20, align 8
  %59 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %18, align 8
  %60 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %19, align 8
  %61 = load i64, i64* %20, align 8
  store %"class.std::__1::allocator"* %59, %"class.std::__1::allocator"** %15, align 8
  store %"class.std::__1::__tree_node"* %60, %"class.std::__1::__tree_node"** %16, align 8
  store i64 %61, i64* %17, align 8
  %62 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %15, align 8
  %63 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %16, align 8
  %64 = bitcast %"class.std::__1::__tree_node"* %63 to i8*
  store i8* %64, i8** %14, align 8
  %65 = load i8*, i8** %14, align 8
  call void @_ZdlPv(i8* %65) #12
  br label %66

; <label>:66:                                     ; preds = %56, %2
  ret void
                                                  ; No predecessors!
  %68 = landingpad { i8*, i32 }
          catch i8* null
  %69 = extractvalue { i8*, i32 } %68, 0
  call void @__clang_call_terminate(i8* %69) #11
  unreachable
}

; Function Attrs: noinline noreturn nounwind
define linkonce_odr hidden void @__clang_call_terminate(i8*) #8 {
  %2 = call i8* @__cxa_begin_catch(i8* %0) #2
  call void @_ZSt9terminatev() #11
  unreachable
}

declare i8* @__cxa_begin_catch(i8*)

declare void @_ZSt9terminatev()

; Function Attrs: nobuiltin nounwind
declare void @_ZdlPv(i8*) #9

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN7VariantD2Ev(%struct.Variant*) unnamed_addr #1 align 2 {
  %2 = alloca %struct.Variant*, align 8
  store %struct.Variant* %0, %struct.Variant** %2, align 8
  %3 = load %struct.Variant*, %struct.Variant** %2, align 8
  %4 = getelementptr inbounds %struct.Variant, %struct.Variant* %3, i32 0, i32 4
  call void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier* %4) #2
  %5 = getelementptr inbounds %struct.Variant, %struct.Variant* %3, i32 0, i32 3
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %5) #2
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier*) unnamed_addr #1 align 2 {
  %2 = alloca %struct.TypeSpecifier*, align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %2, align 8
  %3 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %2, align 8
  call void @_ZN13TypeSpecifierD2Ev(%struct.TypeSpecifier* %3) #2
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN13TypeSpecifierD2Ev(%struct.TypeSpecifier*) unnamed_addr #1 align 2 {
  %2 = alloca %struct.TypeSpecifier*, align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %2, align 8
  %3 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %2, align 8
  %4 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 24
  call void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED1Ev(%"class.std::__1::vector.27"* %4) #2
  %5 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 23
  call void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED1Ev(%"class.std::__1::vector"* %5) #2
  %6 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 14
  call void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED1Ev(%"class.std::__1::unordered_map"* %6) #2
  %7 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 11
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %7) #2
  %8 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 2
  call void @_ZNSt3__110shared_ptrI10ExpressionED1Ev(%"class.std::__1::shared_ptr"* %8) #2
  %9 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 1
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %9) #2
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED1Ev(%"class.std::__1::vector.27"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::vector.27"*, align 8
  store %"class.std::__1::vector.27"* %0, %"class.std::__1::vector.27"** %2, align 8
  %3 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %2, align 8
  call void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::vector.27"* %3) #2
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED1Ev(%"class.std::__1::vector"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::vector"*, align 8
  store %"class.std::__1::vector"* %0, %"class.std::__1::vector"** %2, align 8
  %3 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %2, align 8
  call void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::vector"* %3) #2
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED1Ev(%"class.std::__1::unordered_map"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::unordered_map"*, align 8
  store %"class.std::__1::unordered_map"* %0, %"class.std::__1::unordered_map"** %2, align 8
  %3 = load %"class.std::__1::unordered_map"*, %"class.std::__1::unordered_map"** %2, align 8
  call void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED2Ev(%"class.std::__1::unordered_map"* %3) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__110shared_ptrI10ExpressionED1Ev(%"class.std::__1::shared_ptr"*) unnamed_addr #4 align 2 {
  %2 = alloca %"class.std::__1::shared_ptr"*, align 8
  store %"class.std::__1::shared_ptr"* %0, %"class.std::__1::shared_ptr"** %2, align 8
  %3 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %2, align 8
  call void @_ZNSt3__110shared_ptrI10ExpressionED2Ev(%"class.std::__1::shared_ptr"* %3) #2
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::vector.27"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::vector.27"*, align 8
  store %"class.std::__1::vector.27"* %0, %"class.std::__1::vector.27"** %2, align 8
  %3 = load %"class.std::__1::vector.27"*, %"class.std::__1::vector.27"** %2, align 8
  %4 = bitcast %"class.std::__1::vector.27"* %3 to %"class.std::__1::__vector_base.28"*
  call void @_ZNSt3__113__vector_baseI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::__vector_base.28"* %4) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113__vector_baseI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::__vector_base.28"*) unnamed_addr #4 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::allocator.31"*, align 8
  %3 = alloca %struct.TypeSpecifier*, align 8
  %4 = alloca %"struct.std::__1::integral_constant.35", align 1
  %5 = alloca %"class.std::__1::allocator.31"*, align 8
  %6 = alloca %struct.TypeSpecifier*, align 8
  %7 = alloca %"class.std::__1::allocator.31"*, align 8
  %8 = alloca %struct.TypeSpecifier*, align 8
  %9 = alloca %"struct.std::__1::integral_constant.35", align 1
  %10 = alloca %"struct.std::__1::__has_destroy.36", align 1
  %11 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %12 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %13 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %14 = alloca %struct.TypeSpecifier*, align 8
  %15 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %16 = alloca %struct.TypeSpecifier*, align 8
  %17 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %18 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %19 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %20 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %23 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %24 = alloca %"class.std::__1::__vector_base.28"*, align 8
  %25 = alloca i8*, align 8
  %26 = alloca %"class.std::__1::allocator.31"*, align 8
  %27 = alloca %struct.TypeSpecifier*, align 8
  %28 = alloca i64, align 8
  %29 = alloca %"class.std::__1::allocator.31"*, align 8
  %30 = alloca %struct.TypeSpecifier*, align 8
  %31 = alloca i64, align 8
  %32 = alloca %"class.std::__1::__vector_base.28"*, align 8
  store %"class.std::__1::__vector_base.28"* %0, %"class.std::__1::__vector_base.28"** %32, align 8
  %33 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %32, align 8
  %34 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %33, i32 0, i32 0
  %35 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %34, align 8
  %36 = icmp ne %struct.TypeSpecifier* %35, null
  br i1 %36, label %37, label %95

; <label>:37:                                     ; preds = %1
  store %"class.std::__1::__vector_base.28"* %33, %"class.std::__1::__vector_base.28"** %17, align 8
  %38 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %17, align 8
  %39 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %38, i32 0, i32 0
  %40 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %39, align 8
  store %"class.std::__1::__vector_base.28"* %38, %"class.std::__1::__vector_base.28"** %15, align 8
  store %struct.TypeSpecifier* %40, %struct.TypeSpecifier** %16, align 8
  %41 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %15, align 8
  br label %42

; <label>:42:                                     ; preds = %47, %37
  %43 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %16, align 8
  %44 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %41, i32 0, i32 1
  %45 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %44, align 8
  %46 = icmp ne %struct.TypeSpecifier* %43, %45
  br i1 %46, label %47, label %65

; <label>:47:                                     ; preds = %42
  store %"class.std::__1::__vector_base.28"* %41, %"class.std::__1::__vector_base.28"** %13, align 8
  %48 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %13, align 8
  %49 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %48, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %49, %"class.std::__1::__compressed_pair.29"** %12, align 8
  %50 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %12, align 8
  %51 = bitcast %"class.std::__1::__compressed_pair.29"* %50 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %51, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %11, align 8
  %52 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %11, align 8
  %53 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.30"* %52 to %"class.std::__1::allocator.31"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %41, i32 0, i32 1
  %55 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %54, align 8
  %56 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %55, i32 -1
  store %struct.TypeSpecifier* %56, %struct.TypeSpecifier** %54, align 8
  store %struct.TypeSpecifier* %56, %struct.TypeSpecifier** %14, align 8
  %57 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %14, align 8
  store %"class.std::__1::allocator.31"* %53, %"class.std::__1::allocator.31"** %7, align 8
  store %struct.TypeSpecifier* %57, %struct.TypeSpecifier** %8, align 8
  %58 = bitcast %"struct.std::__1::__has_destroy.36"* %10 to %"struct.std::__1::integral_constant.35"*
  %59 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %7, align 8
  %60 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %8, align 8
  store %"class.std::__1::allocator.31"* %59, %"class.std::__1::allocator.31"** %5, align 8
  store %struct.TypeSpecifier* %60, %struct.TypeSpecifier** %6, align 8
  %61 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %5, align 8
  %62 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %6, align 8
  store %"class.std::__1::allocator.31"* %61, %"class.std::__1::allocator.31"** %2, align 8
  store %struct.TypeSpecifier* %62, %struct.TypeSpecifier** %3, align 8
  %63 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %2, align 8
  %64 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %3, align 8
  call void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier* %64) #2
  br label %42

; <label>:65:                                     ; preds = %42
  store %"class.std::__1::__vector_base.28"* %33, %"class.std::__1::__vector_base.28"** %20, align 8
  %66 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %20, align 8
  %67 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %66, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %67, %"class.std::__1::__compressed_pair.29"** %19, align 8
  %68 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %19, align 8
  %69 = bitcast %"class.std::__1::__compressed_pair.29"* %68 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %69, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %18, align 8
  %70 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %18, align 8
  %71 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.30"* %70 to %"class.std::__1::allocator.31"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %33, i32 0, i32 0
  %73 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %72, align 8
  store %"class.std::__1::__vector_base.28"* %33, %"class.std::__1::__vector_base.28"** %24, align 8
  %74 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %24, align 8
  store %"class.std::__1::__vector_base.28"* %74, %"class.std::__1::__vector_base.28"** %23, align 8
  %75 = load %"class.std::__1::__vector_base.28"*, %"class.std::__1::__vector_base.28"** %23, align 8
  %76 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %75, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %76, %"class.std::__1::__compressed_pair.29"** %22, align 8
  %77 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %22, align 8
  %78 = bitcast %"class.std::__1::__compressed_pair.29"* %77 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %78, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %21, align 8
  %79 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %21, align 8
  %80 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.30", %"class.std::__1::__libcpp_compressed_pair_imp.30"* %79, i32 0, i32 0
  %81 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %80, align 8
  %82 = getelementptr inbounds %"class.std::__1::__vector_base.28", %"class.std::__1::__vector_base.28"* %74, i32 0, i32 0
  %83 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %82, align 8
  %84 = ptrtoint %struct.TypeSpecifier* %81 to i64
  %85 = ptrtoint %struct.TypeSpecifier* %83 to i64
  %86 = sub i64 %84, %85
  %87 = sdiv exact i64 %86, 216
  store %"class.std::__1::allocator.31"* %71, %"class.std::__1::allocator.31"** %29, align 8
  store %struct.TypeSpecifier* %73, %struct.TypeSpecifier** %30, align 8
  store i64 %87, i64* %31, align 8
  %88 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %29, align 8
  %89 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %30, align 8
  %90 = load i64, i64* %31, align 8
  store %"class.std::__1::allocator.31"* %88, %"class.std::__1::allocator.31"** %26, align 8
  store %struct.TypeSpecifier* %89, %struct.TypeSpecifier** %27, align 8
  store i64 %90, i64* %28, align 8
  %91 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %26, align 8
  %92 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %27, align 8
  %93 = bitcast %struct.TypeSpecifier* %92 to i8*
  store i8* %93, i8** %25, align 8
  %94 = load i8*, i8** %25, align 8
  call void @_ZdlPv(i8* %94) #12
  br label %95

; <label>:95:                                     ; preds = %65, %1
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::vector"*) unnamed_addr #1 align 2 {
  %2 = alloca %"class.std::__1::vector"*, align 8
  store %"class.std::__1::vector"* %0, %"class.std::__1::vector"** %2, align 8
  %3 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %2, align 8
  %4 = bitcast %"class.std::__1::vector"* %3 to %"class.std::__1::__vector_base"*
  call void @_ZNSt3__113__vector_baseINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::__vector_base"* %4) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113__vector_baseINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::__vector_base"*) unnamed_addr #4 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.24"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.23"*, align 8
  %4 = alloca %"class.std::__1::__vector_base"*, align 8
  %5 = alloca %"class.std::__1::__vector_base"*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %"class.std::__1::allocator.25"*, align 8
  %8 = alloca %"class.std::__1::basic_string"*, align 8
  %9 = alloca i64, align 8
  %10 = alloca %"class.std::__1::allocator.25"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.24"*, align 8
  %14 = alloca %"class.std::__1::__compressed_pair.23"*, align 8
  %15 = alloca %"class.std::__1::__vector_base"*, align 8
  %16 = alloca %"class.std::__1::allocator.25"*, align 8
  %17 = alloca %"class.std::__1::basic_string"*, align 8
  %18 = alloca %"struct.std::__1::integral_constant.35", align 1
  %19 = alloca %"class.std::__1::allocator.25"*, align 8
  %20 = alloca %"class.std::__1::basic_string"*, align 8
  %21 = alloca %"class.std::__1::allocator.25"*, align 8
  %22 = alloca %"class.std::__1::basic_string"*, align 8
  %23 = alloca %"struct.std::__1::integral_constant.35", align 1
  %24 = alloca %"struct.std::__1::__has_destroy.37", align 1
  %25 = alloca %"class.std::__1::basic_string"*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.24"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.23"*, align 8
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
  store %"class.std::__1::__compressed_pair.23"* %49, %"class.std::__1::__compressed_pair.23"** %27, align 8
  %50 = load %"class.std::__1::__compressed_pair.23"*, %"class.std::__1::__compressed_pair.23"** %27, align 8
  %51 = bitcast %"class.std::__1::__compressed_pair.23"* %50 to %"class.std::__1::__libcpp_compressed_pair_imp.24"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.24"* %51, %"class.std::__1::__libcpp_compressed_pair_imp.24"** %26, align 8
  %52 = load %"class.std::__1::__libcpp_compressed_pair_imp.24"*, %"class.std::__1::__libcpp_compressed_pair_imp.24"** %26, align 8
  %53 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.24"* %52 to %"class.std::__1::allocator.25"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %41, i32 0, i32 1
  %55 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8
  %56 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %55, i32 -1
  store %"class.std::__1::basic_string"* %56, %"class.std::__1::basic_string"** %54, align 8
  store %"class.std::__1::basic_string"* %56, %"class.std::__1::basic_string"** %25, align 8
  %57 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %25, align 8
  store %"class.std::__1::allocator.25"* %53, %"class.std::__1::allocator.25"** %21, align 8
  store %"class.std::__1::basic_string"* %57, %"class.std::__1::basic_string"** %22, align 8
  %58 = bitcast %"struct.std::__1::__has_destroy.37"* %24 to %"struct.std::__1::integral_constant.35"*
  %59 = load %"class.std::__1::allocator.25"*, %"class.std::__1::allocator.25"** %21, align 8
  %60 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %22, align 8
  store %"class.std::__1::allocator.25"* %59, %"class.std::__1::allocator.25"** %19, align 8
  store %"class.std::__1::basic_string"* %60, %"class.std::__1::basic_string"** %20, align 8
  %61 = load %"class.std::__1::allocator.25"*, %"class.std::__1::allocator.25"** %19, align 8
  %62 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8
  store %"class.std::__1::allocator.25"* %61, %"class.std::__1::allocator.25"** %16, align 8
  store %"class.std::__1::basic_string"* %62, %"class.std::__1::basic_string"** %17, align 8
  %63 = load %"class.std::__1::allocator.25"*, %"class.std::__1::allocator.25"** %16, align 8
  %64 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %64) #2
  br label %42

; <label>:65:                                     ; preds = %42
  store %"class.std::__1::__vector_base"* %33, %"class.std::__1::__vector_base"** %15, align 8
  %66 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %15, align 8
  %67 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %66, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.23"* %67, %"class.std::__1::__compressed_pair.23"** %14, align 8
  %68 = load %"class.std::__1::__compressed_pair.23"*, %"class.std::__1::__compressed_pair.23"** %14, align 8
  %69 = bitcast %"class.std::__1::__compressed_pair.23"* %68 to %"class.std::__1::__libcpp_compressed_pair_imp.24"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.24"* %69, %"class.std::__1::__libcpp_compressed_pair_imp.24"** %13, align 8
  %70 = load %"class.std::__1::__libcpp_compressed_pair_imp.24"*, %"class.std::__1::__libcpp_compressed_pair_imp.24"** %13, align 8
  %71 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.24"* %70 to %"class.std::__1::allocator.25"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %33, i32 0, i32 0
  %73 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %72, align 8
  store %"class.std::__1::__vector_base"* %33, %"class.std::__1::__vector_base"** %5, align 8
  %74 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %5, align 8
  store %"class.std::__1::__vector_base"* %74, %"class.std::__1::__vector_base"** %4, align 8
  %75 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %4, align 8
  %76 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %75, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.23"* %76, %"class.std::__1::__compressed_pair.23"** %3, align 8
  %77 = load %"class.std::__1::__compressed_pair.23"*, %"class.std::__1::__compressed_pair.23"** %3, align 8
  %78 = bitcast %"class.std::__1::__compressed_pair.23"* %77 to %"class.std::__1::__libcpp_compressed_pair_imp.24"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.24"* %78, %"class.std::__1::__libcpp_compressed_pair_imp.24"** %2, align 8
  %79 = load %"class.std::__1::__libcpp_compressed_pair_imp.24"*, %"class.std::__1::__libcpp_compressed_pair_imp.24"** %2, align 8
  %80 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.24", %"class.std::__1::__libcpp_compressed_pair_imp.24"* %79, i32 0, i32 0
  %81 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %80, align 8
  %82 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %74, i32 0, i32 0
  %83 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %82, align 8
  %84 = ptrtoint %"class.std::__1::basic_string"* %81 to i64
  %85 = ptrtoint %"class.std::__1::basic_string"* %83 to i64
  %86 = sub i64 %84, %85
  %87 = sdiv exact i64 %86, 24
  store %"class.std::__1::allocator.25"* %71, %"class.std::__1::allocator.25"** %10, align 8
  store %"class.std::__1::basic_string"* %73, %"class.std::__1::basic_string"** %11, align 8
  store i64 %87, i64* %12, align 8
  %88 = load %"class.std::__1::allocator.25"*, %"class.std::__1::allocator.25"** %10, align 8
  %89 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8
  %90 = load i64, i64* %12, align 8
  store %"class.std::__1::allocator.25"* %88, %"class.std::__1::allocator.25"** %7, align 8
  store %"class.std::__1::basic_string"* %89, %"class.std::__1::basic_string"** %8, align 8
  store i64 %90, i64* %9, align 8
  %91 = load %"class.std::__1::allocator.25"*, %"class.std::__1::allocator.25"** %7, align 8
  %92 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %8, align 8
  %93 = bitcast %"class.std::__1::basic_string"* %92 to i8*
  store i8* %93, i8** %6, align 8
  %94 = load i8*, i8** %6, align 8
  call void @_ZdlPv(i8* %94) #12
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
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED1Ev(%"class.std::__1::__hash_table"* %4) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED1Ev(%"class.std::__1::__hash_table"*) unnamed_addr #4 align 2 {
  %2 = alloca %"class.std::__1::__hash_table"*, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %2, align 8
  %3 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %2, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED2Ev(%"class.std::__1::__hash_table"* %3) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED2Ev(%"class.std::__1::__hash_table"*) unnamed_addr #4 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.9"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.8"*, align 8
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.9"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair.8"*, align 8
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.11"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair.10"*, align 8
  %8 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca %"class.std::__1::allocator.12"*, align 8
  %11 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::allocator.12"*, align 8
  %14 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %15 = alloca i64, align 8
  %16 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.11"*, align 8
  %17 = alloca %"class.std::__1::__compressed_pair.10"*, align 8
  %18 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %19 = alloca %"class.std::__1::__bucket_list_deallocator"*, align 8
  %20 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.9"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.8"*, align 8
  %23 = alloca %"class.std::__1::unique_ptr"*, align 8
  %24 = alloca %"struct.std::__1::__hash_node_base"**, align 8
  %25 = alloca %"class.std::__1::unique_ptr"*, align 8
  %26 = alloca %"class.std::__1::unique_ptr"*, align 8
  %27 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.15"*, align 8
  %28 = alloca %"class.std::__1::__compressed_pair.14"*, align 8
  %29 = alloca %"class.std::__1::__hash_table"*, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %29, align 8
  %30 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %29, align 8
  %31 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %30, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.14"* %31, %"class.std::__1::__compressed_pair.14"** %28, align 8
  %32 = load %"class.std::__1::__compressed_pair.14"*, %"class.std::__1::__compressed_pair.14"** %28, align 8
  %33 = bitcast %"class.std::__1::__compressed_pair.14"* %32 to %"class.std::__1::__libcpp_compressed_pair_imp.15"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.15"* %33, %"class.std::__1::__libcpp_compressed_pair_imp.15"** %27, align 8
  %34 = load %"class.std::__1::__libcpp_compressed_pair_imp.15"*, %"class.std::__1::__libcpp_compressed_pair_imp.15"** %27, align 8
  %35 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.15", %"class.std::__1::__libcpp_compressed_pair_imp.15"* %34, i32 0, i32 0
  %36 = getelementptr inbounds %"struct.std::__1::__hash_node_base", %"struct.std::__1::__hash_node_base"* %35, i32 0, i32 0
  %37 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %36, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEE12__deallocateEPNS_16__hash_node_baseIPNS_11__hash_nodeIS9_PvEEEE(%"class.std::__1::__hash_table"* %30, %"struct.std::__1::__hash_node_base"* %37) #2
  %38 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %30, i32 0, i32 0
  store %"class.std::__1::unique_ptr"* %38, %"class.std::__1::unique_ptr"** %26, align 8
  %39 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %26, align 8
  store %"class.std::__1::unique_ptr"* %39, %"class.std::__1::unique_ptr"** %25, align 8
  %40 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %25, align 8
  store %"class.std::__1::unique_ptr"* %40, %"class.std::__1::unique_ptr"** %23, align 8
  %41 = load %"class.std::__1::unique_ptr"*, %"class.std::__1::unique_ptr"** %23, align 8
  %42 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.8"* %42, %"class.std::__1::__compressed_pair.8"** %22, align 8
  %43 = load %"class.std::__1::__compressed_pair.8"*, %"class.std::__1::__compressed_pair.8"** %22, align 8
  %44 = bitcast %"class.std::__1::__compressed_pair.8"* %43 to %"class.std::__1::__libcpp_compressed_pair_imp.9"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.9"* %44, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %21, align 8
  %45 = load %"class.std::__1::__libcpp_compressed_pair_imp.9"*, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %21, align 8
  %46 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.9", %"class.std::__1::__libcpp_compressed_pair_imp.9"* %45, i32 0, i32 0
  %47 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %46, align 8
  store %"struct.std::__1::__hash_node_base"** %47, %"struct.std::__1::__hash_node_base"*** %24, align 8
  %48 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.8"* %48, %"class.std::__1::__compressed_pair.8"** %5, align 8
  %49 = load %"class.std::__1::__compressed_pair.8"*, %"class.std::__1::__compressed_pair.8"** %5, align 8
  %50 = bitcast %"class.std::__1::__compressed_pair.8"* %49 to %"class.std::__1::__libcpp_compressed_pair_imp.9"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.9"* %50, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %4, align 8
  %51 = load %"class.std::__1::__libcpp_compressed_pair_imp.9"*, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %4, align 8
  %52 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.9", %"class.std::__1::__libcpp_compressed_pair_imp.9"* %51, i32 0, i32 0
  store %"struct.std::__1::__hash_node_base"** null, %"struct.std::__1::__hash_node_base"*** %52, align 8
  %53 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %24, align 8
  %54 = icmp ne %"struct.std::__1::__hash_node_base"** %53, null
  br i1 %54, label %55, label %84

; <label>:55:                                     ; preds = %1
  %56 = getelementptr inbounds %"class.std::__1::unique_ptr", %"class.std::__1::unique_ptr"* %41, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.8"* %56, %"class.std::__1::__compressed_pair.8"** %3, align 8
  %57 = load %"class.std::__1::__compressed_pair.8"*, %"class.std::__1::__compressed_pair.8"** %3, align 8
  %58 = bitcast %"class.std::__1::__compressed_pair.8"* %57 to %"class.std::__1::__libcpp_compressed_pair_imp.9"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.9"* %58, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %2, align 8
  %59 = load %"class.std::__1::__libcpp_compressed_pair_imp.9"*, %"class.std::__1::__libcpp_compressed_pair_imp.9"** %2, align 8
  %60 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.9", %"class.std::__1::__libcpp_compressed_pair_imp.9"* %59, i32 0, i32 1
  %61 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %24, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %60, %"class.std::__1::__bucket_list_deallocator"** %19, align 8
  store %"struct.std::__1::__hash_node_base"** %61, %"struct.std::__1::__hash_node_base"*** %20, align 8
  %62 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %19, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %62, %"class.std::__1::__bucket_list_deallocator"** %18, align 8
  %63 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %18, align 8
  %64 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %63, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.10"* %64, %"class.std::__1::__compressed_pair.10"** %17, align 8
  %65 = load %"class.std::__1::__compressed_pair.10"*, %"class.std::__1::__compressed_pair.10"** %17, align 8
  %66 = bitcast %"class.std::__1::__compressed_pair.10"* %65 to %"class.std::__1::__libcpp_compressed_pair_imp.11"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.11"* %66, %"class.std::__1::__libcpp_compressed_pair_imp.11"** %16, align 8
  %67 = load %"class.std::__1::__libcpp_compressed_pair_imp.11"*, %"class.std::__1::__libcpp_compressed_pair_imp.11"** %16, align 8
  %68 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.11"* %67 to %"class.std::__1::allocator.12"*
  %69 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %20, align 8
  store %"class.std::__1::__bucket_list_deallocator"* %62, %"class.std::__1::__bucket_list_deallocator"** %8, align 8
  %70 = load %"class.std::__1::__bucket_list_deallocator"*, %"class.std::__1::__bucket_list_deallocator"** %8, align 8
  %71 = getelementptr inbounds %"class.std::__1::__bucket_list_deallocator", %"class.std::__1::__bucket_list_deallocator"* %70, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.10"* %71, %"class.std::__1::__compressed_pair.10"** %7, align 8
  %72 = load %"class.std::__1::__compressed_pair.10"*, %"class.std::__1::__compressed_pair.10"** %7, align 8
  %73 = bitcast %"class.std::__1::__compressed_pair.10"* %72 to %"class.std::__1::__libcpp_compressed_pair_imp.11"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.11"* %73, %"class.std::__1::__libcpp_compressed_pair_imp.11"** %6, align 8
  %74 = load %"class.std::__1::__libcpp_compressed_pair_imp.11"*, %"class.std::__1::__libcpp_compressed_pair_imp.11"** %6, align 8
  %75 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.11", %"class.std::__1::__libcpp_compressed_pair_imp.11"* %74, i32 0, i32 0
  %76 = load i64, i64* %75, align 8
  store %"class.std::__1::allocator.12"* %68, %"class.std::__1::allocator.12"** %13, align 8
  store %"struct.std::__1::__hash_node_base"** %69, %"struct.std::__1::__hash_node_base"*** %14, align 8
  store i64 %76, i64* %15, align 8
  %77 = load %"class.std::__1::allocator.12"*, %"class.std::__1::allocator.12"** %13, align 8
  %78 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %14, align 8
  %79 = load i64, i64* %15, align 8
  store %"class.std::__1::allocator.12"* %77, %"class.std::__1::allocator.12"** %10, align 8
  store %"struct.std::__1::__hash_node_base"** %78, %"struct.std::__1::__hash_node_base"*** %11, align 8
  store i64 %79, i64* %12, align 8
  %80 = load %"class.std::__1::allocator.12"*, %"class.std::__1::allocator.12"** %10, align 8
  %81 = load %"struct.std::__1::__hash_node_base"**, %"struct.std::__1::__hash_node_base"*** %11, align 8
  %82 = bitcast %"struct.std::__1::__hash_node_base"** %81 to i8*
  store i8* %82, i8** %9, align 8
  %83 = load i8*, i8** %9, align 8
  call void @_ZdlPv(i8* %83) #12
  br label %84

; <label>:84:                                     ; preds = %1, %55
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEE12__deallocateEPNS_16__hash_node_baseIPNS_11__hash_nodeIS9_PvEEEE(%"class.std::__1::__hash_table"*, %"struct.std::__1::__hash_node_base"*) #4 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::allocator.16"*, align 8
  %4 = alloca %"struct.std::__1::pair.38"*, align 8
  %5 = alloca %"class.std::__1::allocator.16"*, align 8
  %6 = alloca %"struct.std::__1::pair.38"*, align 8
  %7 = alloca %"struct.std::__1::integral_constant", align 1
  %8 = alloca %"struct.std::__1::__has_destroy.40", align 1
  %9 = alloca i8*, align 8
  %10 = alloca %"class.std::__1::allocator.16"*, align 8
  %11 = alloca %"struct.std::__1::__hash_node"*, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::allocator.16"*, align 8
  %14 = alloca %"struct.std::__1::__hash_node"*, align 8
  %15 = alloca i64, align 8
  %16 = alloca %"struct.std::__1::pair.38"*, align 8
  %17 = alloca %"union.std::__1::__hash_value_type"*, align 8
  %18 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %19 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %20 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.15"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.14"*, align 8
  %23 = alloca %"class.std::__1::__hash_table"*, align 8
  %24 = alloca %"class.std::__1::__hash_table"*, align 8
  %25 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %26 = alloca %"class.std::__1::allocator.16"*, align 8
  %27 = alloca %"struct.std::__1::__hash_node_base"*, align 8
  %28 = alloca %"struct.std::__1::__hash_node"*, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %24, align 8
  store %"struct.std::__1::__hash_node_base"* %1, %"struct.std::__1::__hash_node_base"** %25, align 8
  %29 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %24, align 8
  store %"class.std::__1::__hash_table"* %29, %"class.std::__1::__hash_table"** %23, align 8
  %30 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %23, align 8
  %31 = getelementptr inbounds %"class.std::__1::__hash_table", %"class.std::__1::__hash_table"* %30, i32 0, i32 1
  store %"class.std::__1::__compressed_pair.14"* %31, %"class.std::__1::__compressed_pair.14"** %22, align 8
  %32 = load %"class.std::__1::__compressed_pair.14"*, %"class.std::__1::__compressed_pair.14"** %22, align 8
  %33 = bitcast %"class.std::__1::__compressed_pair.14"* %32 to %"class.std::__1::__libcpp_compressed_pair_imp.15"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.15"* %33, %"class.std::__1::__libcpp_compressed_pair_imp.15"** %21, align 8
  %34 = load %"class.std::__1::__libcpp_compressed_pair_imp.15"*, %"class.std::__1::__libcpp_compressed_pair_imp.15"** %21, align 8
  %35 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.15"* %34 to %"class.std::__1::allocator.16"*
  store %"class.std::__1::allocator.16"* %35, %"class.std::__1::allocator.16"** %26, align 8
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
  %49 = load %"class.std::__1::allocator.16"*, %"class.std::__1::allocator.16"** %26, align 8
  %50 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %28, align 8
  %51 = getelementptr inbounds %"struct.std::__1::__hash_node", %"struct.std::__1::__hash_node"* %50, i32 0, i32 2
  store %"union.std::__1::__hash_value_type"* %51, %"union.std::__1::__hash_value_type"** %17, align 8
  %52 = load %"union.std::__1::__hash_value_type"*, %"union.std::__1::__hash_value_type"** %17, align 8
  %53 = bitcast %"union.std::__1::__hash_value_type"* %52 to %"struct.std::__1::pair.38"*
  store %"struct.std::__1::pair.38"* %53, %"struct.std::__1::pair.38"** %16, align 8
  %54 = load %"struct.std::__1::pair.38"*, %"struct.std::__1::pair.38"** %16, align 8
  %55 = bitcast %"struct.std::__1::pair.38"* %54 to i8*
  br label %56

; <label>:56:                                     ; preds = %39
  store %"class.std::__1::allocator.16"* %49, %"class.std::__1::allocator.16"** %5, align 8
  store %"struct.std::__1::pair.38"* %54, %"struct.std::__1::pair.38"** %6, align 8
  %57 = bitcast %"struct.std::__1::__has_destroy.40"* %8 to %"struct.std::__1::integral_constant"*
  %58 = load %"class.std::__1::allocator.16"*, %"class.std::__1::allocator.16"** %5, align 8
  %59 = load %"struct.std::__1::pair.38"*, %"struct.std::__1::pair.38"** %6, align 8
  store %"class.std::__1::allocator.16"* %58, %"class.std::__1::allocator.16"** %3, align 8
  store %"struct.std::__1::pair.38"* %59, %"struct.std::__1::pair.38"** %4, align 8
  %60 = load %"struct.std::__1::pair.38"*, %"struct.std::__1::pair.38"** %4, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED1Ev(%"struct.std::__1::pair.38"* %60) #2
  br label %61

; <label>:61:                                     ; preds = %56
  %62 = load %"class.std::__1::allocator.16"*, %"class.std::__1::allocator.16"** %26, align 8
  %63 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %28, align 8
  store %"class.std::__1::allocator.16"* %62, %"class.std::__1::allocator.16"** %13, align 8
  store %"struct.std::__1::__hash_node"* %63, %"struct.std::__1::__hash_node"** %14, align 8
  store i64 1, i64* %15, align 8
  %64 = load %"class.std::__1::allocator.16"*, %"class.std::__1::allocator.16"** %13, align 8
  %65 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %14, align 8
  %66 = load i64, i64* %15, align 8
  store %"class.std::__1::allocator.16"* %64, %"class.std::__1::allocator.16"** %10, align 8
  store %"struct.std::__1::__hash_node"* %65, %"struct.std::__1::__hash_node"** %11, align 8
  store i64 %66, i64* %12, align 8
  %67 = load %"class.std::__1::allocator.16"*, %"class.std::__1::allocator.16"** %10, align 8
  %68 = load %"struct.std::__1::__hash_node"*, %"struct.std::__1::__hash_node"** %11, align 8
  %69 = bitcast %"struct.std::__1::__hash_node"* %68 to i8*
  store i8* %69, i8** %9, align 8
  %70 = load i8*, i8** %9, align 8
  call void @_ZdlPv(i8* %70) #12
  %71 = load %"struct.std::__1::__hash_node_base"*, %"struct.std::__1::__hash_node_base"** %27, align 8
  store %"struct.std::__1::__hash_node_base"* %71, %"struct.std::__1::__hash_node_base"** %25, align 8
  br label %36

; <label>:72:                                     ; preds = %36
  ret void
                                                  ; No predecessors!
  %74 = landingpad { i8*, i32 }
          catch i8* null
  %75 = extractvalue { i8*, i32 } %74, 0
  call void @__clang_call_terminate(i8* %75) #11
  unreachable
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED1Ev(%"struct.std::__1::pair.38"*) unnamed_addr #1 align 2 {
  %2 = alloca %"struct.std::__1::pair.38"*, align 8
  store %"struct.std::__1::pair.38"* %0, %"struct.std::__1::pair.38"** %2, align 8
  %3 = load %"struct.std::__1::pair.38"*, %"struct.std::__1::pair.38"** %2, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED2Ev(%"struct.std::__1::pair.38"* %3) #2
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED2Ev(%"struct.std::__1::pair.38"*) unnamed_addr #1 align 2 {
  %2 = alloca %"struct.std::__1::pair.38"*, align 8
  store %"struct.std::__1::pair.38"* %0, %"struct.std::__1::pair.38"** %2, align 8
  %3 = load %"struct.std::__1::pair.38"*, %"struct.std::__1::pair.38"** %2, align 8
  %4 = getelementptr inbounds %"struct.std::__1::pair.38", %"struct.std::__1::pair.38"* %3, i32 0, i32 1
  call void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier* %4) #2
  %5 = getelementptr inbounds %"struct.std::__1::pair.38", %"struct.std::__1::pair.38"* %3, i32 0, i32 0
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %5) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__110shared_ptrI10ExpressionED2Ev(%"class.std::__1::shared_ptr"*) unnamed_addr #4 align 2 {
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
  call void @_ZNSt3__119__shared_weak_count16__release_sharedEv(%"class.std::__1::__shared_weak_count"* %9) #2
  br label %10

; <label>:10:                                     ; preds = %7, %1
  ret void
}

; Function Attrs: nounwind
declare void @_ZNSt3__119__shared_weak_count16__release_sharedEv(%"class.std::__1::__shared_weak_count"*) #5

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr void @_ZN5TokenC2ERKS_(%class.Token*, %class.Token* dereferenceable(288)) unnamed_addr #6 align 2 {
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

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #7

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEEC1ERKSD_(%"class.std::__1::__tree"*, %"class.std::__1::__map_value_compare"* dereferenceable(1)) unnamed_addr #4 align 2 {
  %3 = alloca %"class.std::__1::__tree"*, align 8
  %4 = alloca %"class.std::__1::__map_value_compare"*, align 8
  store %"class.std::__1::__tree"* %0, %"class.std::__1::__tree"** %3, align 8
  store %"class.std::__1::__map_value_compare"* %1, %"class.std::__1::__map_value_compare"** %4, align 8
  %5 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %3, align 8
  %6 = load %"class.std::__1::__map_value_compare"*, %"class.std::__1::__map_value_compare"** %4, align 8
  call void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEEC2ERKSD_(%"class.std::__1::__tree"* %5, %"class.std::__1::__map_value_compare"* dereferenceable(1) %6) #2
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEEC2ERKSD_(%"class.std::__1::__tree"*, %"class.std::__1::__map_value_compare"* dereferenceable(1)) unnamed_addr #4 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %4 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %5 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %6 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %7 = alloca %"class.std::__1::__tree"*, align 8
  %8 = alloca %"class.std::__1::__tree"*, align 8
  %9 = alloca %"class.std::__1::__map_value_compare"*, align 8
  %10 = alloca i64*, align 8
  %11 = alloca %"class.std::__1::__map_value_compare"*, align 8
  %12 = alloca %"class.std::__1::__map_value_compare", align 1
  %13 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %14 = alloca i64, align 8
  %15 = alloca i64*, align 8
  %16 = alloca %"class.std::__1::__map_value_compare", align 1
  %17 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %18 = alloca i64, align 8
  %19 = alloca %"class.std::__1::__map_value_compare", align 1
  %20 = alloca %"class.std::__1::__map_value_compare", align 1
  %21 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %22 = alloca i64, align 8
  %23 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %24 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %25 = alloca %"class.std::__1::allocator"*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %28 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %29 = alloca %"class.std::__1::__tree"*, align 8
  %30 = alloca %"class.std::__1::__map_value_compare"*, align 8
  %31 = alloca %"class.std::__1::__map_value_compare", align 1
  store %"class.std::__1::__tree"* %0, %"class.std::__1::__tree"** %29, align 8
  store %"class.std::__1::__map_value_compare"* %1, %"class.std::__1::__map_value_compare"** %30, align 8
  %32 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %29, align 8
  %33 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %32, i32 0, i32 1
  store %"class.std::__1::__compressed_pair"* %33, %"class.std::__1::__compressed_pair"** %28, align 8
  %34 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %28, align 8
  store %"class.std::__1::__compressed_pair"* %34, %"class.std::__1::__compressed_pair"** %27, align 8
  %35 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8
  %36 = bitcast %"class.std::__1::__compressed_pair"* %35 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %36, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8
  %37 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8
  %38 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %37 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %38, %"class.std::__1::allocator"** %25, align 8
  %39 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %25, align 8
  %40 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %37, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %40, %"class.std::__1::__tree_end_node"** %24, align 8
  %41 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %24, align 8
  store %"class.std::__1::__tree_end_node"* %41, %"class.std::__1::__tree_end_node"** %23, align 8
  %42 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %23, align 8
  %43 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %42, i32 0, i32 0
  store %"class.std::__1::__tree_node_base"* null, %"class.std::__1::__tree_node_base"** %43, align 8
  br label %44

; <label>:44:                                     ; preds = %2
  %45 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %32, i32 0, i32 2
  %46 = load %"class.std::__1::__map_value_compare"*, %"class.std::__1::__map_value_compare"** %30, align 8
  store %"class.std::__1::__compressed_pair.0"* %45, %"class.std::__1::__compressed_pair.0"** %21, align 8
  store i64 0, i64* %22, align 8
  %47 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %21, align 8
  %48 = load i64, i64* %22, align 8
  store %"class.std::__1::__compressed_pair.0"* %47, %"class.std::__1::__compressed_pair.0"** %17, align 8
  store i64 %48, i64* %18, align 8
  %49 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %17, align 8
  %50 = bitcast %"class.std::__1::__compressed_pair.0"* %49 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store i64* %18, i64** %15, align 8
  %51 = load i64*, i64** %15, align 8
  %52 = load i64, i64* %51, align 8
  store %"class.std::__1::__map_value_compare"* %16, %"class.std::__1::__map_value_compare"** %9, align 8
  %53 = load %"class.std::__1::__map_value_compare"*, %"class.std::__1::__map_value_compare"** %9, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %50, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %13, align 8
  store i64 %52, i64* %14, align 8
  %54 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %13, align 8
  %55 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.1"* %54 to %"class.std::__1::__map_value_compare"*
  store %"class.std::__1::__map_value_compare"* %12, %"class.std::__1::__map_value_compare"** %11, align 8
  %56 = load %"class.std::__1::__map_value_compare"*, %"class.std::__1::__map_value_compare"** %11, align 8
  %57 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.1", %"class.std::__1::__libcpp_compressed_pair_imp.1"* %54, i32 0, i32 0
  store i64* %14, i64** %10, align 8
  %58 = load i64*, i64** %10, align 8
  %59 = load i64, i64* %58, align 8
  store i64 %59, i64* %57, align 8
  br label %60

; <label>:60:                                     ; preds = %44
  store %"class.std::__1::__tree"* %32, %"class.std::__1::__tree"** %7, align 8
  %61 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %7, align 8
  %62 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %61, i32 0, i32 1
  store %"class.std::__1::__compressed_pair"* %62, %"class.std::__1::__compressed_pair"** %6, align 8
  %63 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %6, align 8
  %64 = bitcast %"class.std::__1::__compressed_pair"* %63 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %64, %"class.std::__1::__libcpp_compressed_pair_imp"** %5, align 8
  %65 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %5, align 8
  %66 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %65, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %66, %"class.std::__1::__tree_end_node"** %4, align 8
  %67 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %4, align 8
  store %"class.std::__1::__tree_end_node"* %67, %"class.std::__1::__tree_end_node"** %3, align 8
  %68 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %3, align 8
  %69 = bitcast %"class.std::__1::__tree_end_node"* %68 to i8*
  store %"class.std::__1::__tree"* %32, %"class.std::__1::__tree"** %8, align 8
  %70 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %8, align 8
  %71 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %70, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %68, %"class.std::__1::__tree_end_node"** %71, align 8
  ret void
                                                  ; No predecessors!
  %73 = landingpad { i8*, i32 }
          catch i8* null
  %74 = extractvalue { i8*, i32 } %73, 0
  call void @__clang_call_terminate(i8* %74) #11
  unreachable
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.std::__1::__tree_end_node"* @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE30__emplace_hint_unique_key_argsIS2_JRKNS_4pairIKS2_S8_EEEEENS_15__tree_iteratorIS9_PNS_11__tree_nodeIS9_PvEElEENS_21__tree_const_iteratorIS9_SQ_lEERKT_DpOT0_(%"class.std::__1::__tree"*, %"class.std::__1::__tree_end_node"*, i32* dereferenceable(4), %"struct.std::__1::pair"* dereferenceable(32)) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %5 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %6 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %7 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %8 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca %"class.std::__1::allocator"*, align 8
  %11 = alloca %"class.std::__1::__tree_node"*, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::allocator"*, align 8
  %14 = alloca %"class.std::__1::__tree_node"*, align 8
  %15 = alloca i64, align 8
  %16 = alloca %"class.std::__1::allocator"*, align 8
  %17 = alloca %"struct.std::__1::pair"*, align 8
  %18 = alloca %"class.std::__1::allocator"*, align 8
  %19 = alloca %"struct.std::__1::pair"*, align 8
  %20 = alloca %"struct.std::__1::integral_constant", align 1
  %21 = alloca %"struct.std::__1::__has_destroy", align 1
  %22 = alloca %"struct.std::__1::pair"*, align 8
  %23 = alloca %"union.std::__1::__value_type"*, align 8
  %24 = alloca %"class.std::__1::__tree_node_destructor"*, align 8
  %25 = alloca %"class.std::__1::__tree_node"*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %28 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %29 = alloca %"class.std::__1::__tree_node"*, align 8
  %30 = alloca %"class.std::__1::__tree_node"*, align 8
  %31 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %32 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %33 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %34 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %35 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %36 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %37 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %38 = alloca %"class.std::__1::__tree_node"*, align 8
  %39 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %40 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %41 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %42 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %43 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %44 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %45 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %46 = alloca i8*, align 8
  %47 = alloca %"class.std::__1::allocator"*, align 8
  %48 = alloca %"class.std::__1::__tree_node"*, align 8
  %49 = alloca i64, align 8
  %50 = alloca %"class.std::__1::allocator"*, align 8
  %51 = alloca %"class.std::__1::__tree_node"*, align 8
  %52 = alloca i64, align 8
  %53 = alloca %"class.std::__1::allocator"*, align 8
  %54 = alloca %"struct.std::__1::pair"*, align 8
  %55 = alloca %"class.std::__1::allocator"*, align 8
  %56 = alloca %"struct.std::__1::pair"*, align 8
  %57 = alloca %"struct.std::__1::integral_constant", align 1
  %58 = alloca %"struct.std::__1::__has_destroy", align 1
  %59 = alloca %"struct.std::__1::pair"*, align 8
  %60 = alloca %"union.std::__1::__value_type"*, align 8
  %61 = alloca %"class.std::__1::__tree_node_destructor"*, align 8
  %62 = alloca %"class.std::__1::__tree_node"*, align 8
  %63 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %64 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %65 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %66 = alloca %"class.std::__1::__tree_node"*, align 8
  %67 = alloca %"class.std::__1::__tree_node"*, align 8
  %68 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %69 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %70 = alloca %"class.std::__1::__tree_iterator"*, align 8
  %71 = alloca %"class.std::__1::__tree_node"*, align 8
  %72 = alloca %"class.std::__1::__tree_iterator"*, align 8
  %73 = alloca %"class.std::__1::__tree_node"*, align 8
  %74 = alloca %"struct.std::__1::pair"*, align 8
  %75 = alloca %"class.std::__1::__tree_iterator", align 8
  %76 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %77 = alloca %"class.std::__1::__tree"*, align 8
  %78 = alloca i32*, align 8
  %79 = alloca %"struct.std::__1::pair"*, align 8
  %80 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %81 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %82 = alloca %"class.std::__1::__tree_node_base"**, align 8
  %83 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %84 = alloca %"class.std::__1::__tree_node"*, align 8
  %85 = alloca %"class.std::__1::unique_ptr.41", align 8
  %86 = alloca i8*
  %87 = alloca i32
  %88 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %76, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %1, %"class.std::__1::__tree_end_node"** %88, align 8
  store %"class.std::__1::__tree"* %0, %"class.std::__1::__tree"** %77, align 8
  store i32* %2, i32** %78, align 8
  store %"struct.std::__1::pair"* %3, %"struct.std::__1::pair"** %79, align 8
  %89 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %77, align 8
  %90 = bitcast %"class.std::__1::__tree_const_iterator"* %83 to i8*
  %91 = bitcast %"class.std::__1::__tree_const_iterator"* %76 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %90, i8* %91, i64 8, i32 8, i1 false)
  %92 = load i32*, i32** %78, align 8
  %93 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %83, i32 0, i32 0
  %94 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %93, align 8
  %95 = call dereferenceable(8) %"class.std::__1::__tree_node_base"** @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE12__find_equalIS2_EERPNS_16__tree_node_baseIPvEENS_21__tree_const_iteratorIS9_PNS_11__tree_nodeIS9_SI_EElEERPNS_15__tree_end_nodeISK_EESL_RKT_(%"class.std::__1::__tree"* %89, %"class.std::__1::__tree_end_node"* %94, %"class.std::__1::__tree_end_node"** dereferenceable(8) %80, %"class.std::__1::__tree_node_base"** dereferenceable(8) %81, i32* dereferenceable(4) %92)
  store %"class.std::__1::__tree_node_base"** %95, %"class.std::__1::__tree_node_base"*** %82, align 8
  %96 = load %"class.std::__1::__tree_node_base"**, %"class.std::__1::__tree_node_base"*** %82, align 8
  %97 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %96, align 8
  %98 = bitcast %"class.std::__1::__tree_node_base"* %97 to %"class.std::__1::__tree_node"*
  store %"class.std::__1::__tree_node"* %98, %"class.std::__1::__tree_node"** %84, align 8
  %99 = load %"class.std::__1::__tree_node_base"**, %"class.std::__1::__tree_node_base"*** %82, align 8
  %100 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %99, align 8
  %101 = icmp eq %"class.std::__1::__tree_node_base"* %100, null
  br i1 %101, label %102, label %247

; <label>:102:                                    ; preds = %4
  %103 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %79, align 8
  store %"struct.std::__1::pair"* %103, %"struct.std::__1::pair"** %74, align 8
  %104 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %74, align 8
  call void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE16__construct_nodeIJRKNS_4pairIKS2_S8_EEEEENS_10unique_ptrINS_11__tree_nodeIS9_PvEENS_22__tree_node_destructorINS6_ISP_EEEEEEDpOT_(%"class.std::__1::unique_ptr.41"* sret %85, %"class.std::__1::__tree"* %89, %"struct.std::__1::pair"* dereferenceable(32) %104)
  %105 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %80, align 8
  %106 = load %"class.std::__1::__tree_node_base"**, %"class.std::__1::__tree_node_base"*** %82, align 8
  store %"class.std::__1::unique_ptr.41"* %85, %"class.std::__1::unique_ptr.41"** %41, align 8
  %107 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %41, align 8
  %108 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %107, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %108, %"class.std::__1::__compressed_pair.42"** %40, align 8
  %109 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %40, align 8
  %110 = bitcast %"class.std::__1::__compressed_pair.42"* %109 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %110, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %39, align 8
  %111 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %39, align 8
  %112 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %111, i32 0, i32 0
  %113 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %112, align 8
  %114 = bitcast %"class.std::__1::__tree_node"* %113 to %"class.std::__1::__tree_node_base"*
  invoke void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE16__insert_node_atEPNS_15__tree_end_nodeIPNS_16__tree_node_baseIPvEEEERSK_SK_(%"class.std::__1::__tree"* %89, %"class.std::__1::__tree_end_node"* %105, %"class.std::__1::__tree_node_base"** dereferenceable(8) %106, %"class.std::__1::__tree_node_base"* %114)
          to label %115 unwind label %186

; <label>:115:                                    ; preds = %102
  store %"class.std::__1::unique_ptr.41"* %85, %"class.std::__1::unique_ptr.41"** %37, align 8
  %116 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %37, align 8
  %117 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %116, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %117, %"class.std::__1::__compressed_pair.42"** %36, align 8
  %118 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %36, align 8
  %119 = bitcast %"class.std::__1::__compressed_pair.42"* %118 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %119, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %35, align 8
  %120 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %35, align 8
  %121 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %120, i32 0, i32 0
  %122 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %121, align 8
  store %"class.std::__1::__tree_node"* %122, %"class.std::__1::__tree_node"** %38, align 8
  %123 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %116, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %123, %"class.std::__1::__compressed_pair.42"** %34, align 8
  %124 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %34, align 8
  %125 = bitcast %"class.std::__1::__compressed_pair.42"* %124 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %125, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %33, align 8
  %126 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %33, align 8
  %127 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %126, i32 0, i32 0
  store %"class.std::__1::__tree_node"* null, %"class.std::__1::__tree_node"** %127, align 8
  %128 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %38, align 8
  store %"class.std::__1::__tree_node"* %128, %"class.std::__1::__tree_node"** %84, align 8
  store %"class.std::__1::unique_ptr.41"* %85, %"class.std::__1::unique_ptr.41"** %32, align 8
  %129 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %32, align 8
  store %"class.std::__1::unique_ptr.41"* %129, %"class.std::__1::unique_ptr.41"** %31, align 8
  %130 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %31, align 8
  store %"class.std::__1::unique_ptr.41"* %130, %"class.std::__1::unique_ptr.41"** %28, align 8
  store %"class.std::__1::__tree_node"* null, %"class.std::__1::__tree_node"** %29, align 8
  %131 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %28, align 8
  %132 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %131, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %132, %"class.std::__1::__compressed_pair.42"** %27, align 8
  %133 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %27, align 8
  %134 = bitcast %"class.std::__1::__compressed_pair.42"* %133 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %134, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %26, align 8
  %135 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %26, align 8
  %136 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %135, i32 0, i32 0
  %137 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %136, align 8
  store %"class.std::__1::__tree_node"* %137, %"class.std::__1::__tree_node"** %30, align 8
  %138 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %29, align 8
  %139 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %131, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %139, %"class.std::__1::__compressed_pair.42"** %8, align 8
  %140 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %8, align 8
  %141 = bitcast %"class.std::__1::__compressed_pair.42"* %140 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %141, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %7, align 8
  %142 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %7, align 8
  %143 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %142, i32 0, i32 0
  store %"class.std::__1::__tree_node"* %138, %"class.std::__1::__tree_node"** %143, align 8
  %144 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %30, align 8
  %145 = icmp ne %"class.std::__1::__tree_node"* %144, null
  br i1 %145, label %146, label %185

; <label>:146:                                    ; preds = %115
  %147 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %131, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %147, %"class.std::__1::__compressed_pair.42"** %6, align 8
  %148 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %6, align 8
  %149 = bitcast %"class.std::__1::__compressed_pair.42"* %148 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %149, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %5, align 8
  %150 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %5, align 8
  %151 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %150, i32 0, i32 1
  %152 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %30, align 8
  store %"class.std::__1::__tree_node_destructor"* %151, %"class.std::__1::__tree_node_destructor"** %24, align 8
  store %"class.std::__1::__tree_node"* %152, %"class.std::__1::__tree_node"** %25, align 8
  %153 = load %"class.std::__1::__tree_node_destructor"*, %"class.std::__1::__tree_node_destructor"** %24, align 8
  %154 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %153, i32 0, i32 1
  %155 = load i8, i8* %154, align 8
  %156 = trunc i8 %155 to i1
  br i1 %156, label %157, label %170

; <label>:157:                                    ; preds = %146
  %158 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %153, i32 0, i32 0
  %159 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %158, align 8
  %160 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %25, align 8
  %161 = getelementptr inbounds %"class.std::__1::__tree_node", %"class.std::__1::__tree_node"* %160, i32 0, i32 1
  store %"union.std::__1::__value_type"* %161, %"union.std::__1::__value_type"** %23, align 8
  %162 = load %"union.std::__1::__value_type"*, %"union.std::__1::__value_type"** %23, align 8
  %163 = bitcast %"union.std::__1::__value_type"* %162 to %"struct.std::__1::pair"*
  store %"struct.std::__1::pair"* %163, %"struct.std::__1::pair"** %22, align 8
  %164 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %22, align 8
  %165 = bitcast %"struct.std::__1::pair"* %164 to i8*
  store %"class.std::__1::allocator"* %159, %"class.std::__1::allocator"** %18, align 8
  store %"struct.std::__1::pair"* %164, %"struct.std::__1::pair"** %19, align 8
  %166 = bitcast %"struct.std::__1::__has_destroy"* %21 to %"struct.std::__1::integral_constant"*
  %167 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %18, align 8
  %168 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %19, align 8
  store %"class.std::__1::allocator"* %167, %"class.std::__1::allocator"** %16, align 8
  store %"struct.std::__1::pair"* %168, %"struct.std::__1::pair"** %17, align 8
  %169 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %17, align 8
  call void @_ZNSt3__14pairIK11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEED1Ev(%"struct.std::__1::pair"* %169) #2
  br label %170

; <label>:170:                                    ; preds = %157, %146
  %171 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %25, align 8
  %172 = icmp ne %"class.std::__1::__tree_node"* %171, null
  br i1 %172, label %173, label %184

; <label>:173:                                    ; preds = %170
  %174 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %153, i32 0, i32 0
  %175 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %174, align 8
  %176 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %25, align 8
  store %"class.std::__1::allocator"* %175, %"class.std::__1::allocator"** %13, align 8
  store %"class.std::__1::__tree_node"* %176, %"class.std::__1::__tree_node"** %14, align 8
  store i64 1, i64* %15, align 8
  %177 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %13, align 8
  %178 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %14, align 8
  %179 = load i64, i64* %15, align 8
  store %"class.std::__1::allocator"* %177, %"class.std::__1::allocator"** %10, align 8
  store %"class.std::__1::__tree_node"* %178, %"class.std::__1::__tree_node"** %11, align 8
  store i64 %179, i64* %12, align 8
  %180 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %10, align 8
  %181 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %11, align 8
  %182 = bitcast %"class.std::__1::__tree_node"* %181 to i8*
  store i8* %182, i8** %9, align 8
  %183 = load i8*, i8** %9, align 8
  call void @_ZdlPv(i8* %183) #12
  br label %184

; <label>:184:                                    ; preds = %173, %170
  br label %185

; <label>:185:                                    ; preds = %115, %184
  br label %247

; <label>:186:                                    ; preds = %102
  %187 = landingpad { i8*, i32 }
          cleanup
  %188 = extractvalue { i8*, i32 } %187, 0
  store i8* %188, i8** %86, align 8
  %189 = extractvalue { i8*, i32 } %187, 1
  store i32 %189, i32* %87, align 4
  store %"class.std::__1::unique_ptr.41"* %85, %"class.std::__1::unique_ptr.41"** %69, align 8
  %190 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %69, align 8
  store %"class.std::__1::unique_ptr.41"* %190, %"class.std::__1::unique_ptr.41"** %68, align 8
  %191 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %68, align 8
  store %"class.std::__1::unique_ptr.41"* %191, %"class.std::__1::unique_ptr.41"** %65, align 8
  store %"class.std::__1::__tree_node"* null, %"class.std::__1::__tree_node"** %66, align 8
  %192 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %65, align 8
  %193 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %192, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %193, %"class.std::__1::__compressed_pair.42"** %64, align 8
  %194 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %64, align 8
  %195 = bitcast %"class.std::__1::__compressed_pair.42"* %194 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %195, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %63, align 8
  %196 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %63, align 8
  %197 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %196, i32 0, i32 0
  %198 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %197, align 8
  store %"class.std::__1::__tree_node"* %198, %"class.std::__1::__tree_node"** %67, align 8
  %199 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %66, align 8
  %200 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %192, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %200, %"class.std::__1::__compressed_pair.42"** %45, align 8
  %201 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %45, align 8
  %202 = bitcast %"class.std::__1::__compressed_pair.42"* %201 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %202, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %44, align 8
  %203 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %44, align 8
  %204 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %203, i32 0, i32 0
  store %"class.std::__1::__tree_node"* %199, %"class.std::__1::__tree_node"** %204, align 8
  %205 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %67, align 8
  %206 = icmp ne %"class.std::__1::__tree_node"* %205, null
  br i1 %206, label %207, label %246

; <label>:207:                                    ; preds = %186
  %208 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %192, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %208, %"class.std::__1::__compressed_pair.42"** %43, align 8
  %209 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %43, align 8
  %210 = bitcast %"class.std::__1::__compressed_pair.42"* %209 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %210, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %42, align 8
  %211 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %42, align 8
  %212 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %211, i32 0, i32 1
  %213 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %67, align 8
  store %"class.std::__1::__tree_node_destructor"* %212, %"class.std::__1::__tree_node_destructor"** %61, align 8
  store %"class.std::__1::__tree_node"* %213, %"class.std::__1::__tree_node"** %62, align 8
  %214 = load %"class.std::__1::__tree_node_destructor"*, %"class.std::__1::__tree_node_destructor"** %61, align 8
  %215 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %214, i32 0, i32 1
  %216 = load i8, i8* %215, align 8
  %217 = trunc i8 %216 to i1
  br i1 %217, label %218, label %231

; <label>:218:                                    ; preds = %207
  %219 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %214, i32 0, i32 0
  %220 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %219, align 8
  %221 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %62, align 8
  %222 = getelementptr inbounds %"class.std::__1::__tree_node", %"class.std::__1::__tree_node"* %221, i32 0, i32 1
  store %"union.std::__1::__value_type"* %222, %"union.std::__1::__value_type"** %60, align 8
  %223 = load %"union.std::__1::__value_type"*, %"union.std::__1::__value_type"** %60, align 8
  %224 = bitcast %"union.std::__1::__value_type"* %223 to %"struct.std::__1::pair"*
  store %"struct.std::__1::pair"* %224, %"struct.std::__1::pair"** %59, align 8
  %225 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %59, align 8
  %226 = bitcast %"struct.std::__1::pair"* %225 to i8*
  store %"class.std::__1::allocator"* %220, %"class.std::__1::allocator"** %55, align 8
  store %"struct.std::__1::pair"* %225, %"struct.std::__1::pair"** %56, align 8
  %227 = bitcast %"struct.std::__1::__has_destroy"* %58 to %"struct.std::__1::integral_constant"*
  %228 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %55, align 8
  %229 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %56, align 8
  store %"class.std::__1::allocator"* %228, %"class.std::__1::allocator"** %53, align 8
  store %"struct.std::__1::pair"* %229, %"struct.std::__1::pair"** %54, align 8
  %230 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %54, align 8
  call void @_ZNSt3__14pairIK11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEED1Ev(%"struct.std::__1::pair"* %230) #2
  br label %231

; <label>:231:                                    ; preds = %218, %207
  %232 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %62, align 8
  %233 = icmp ne %"class.std::__1::__tree_node"* %232, null
  br i1 %233, label %234, label %245

; <label>:234:                                    ; preds = %231
  %235 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %214, i32 0, i32 0
  %236 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %235, align 8
  %237 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %62, align 8
  store %"class.std::__1::allocator"* %236, %"class.std::__1::allocator"** %50, align 8
  store %"class.std::__1::__tree_node"* %237, %"class.std::__1::__tree_node"** %51, align 8
  store i64 1, i64* %52, align 8
  %238 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %50, align 8
  %239 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %51, align 8
  %240 = load i64, i64* %52, align 8
  store %"class.std::__1::allocator"* %238, %"class.std::__1::allocator"** %47, align 8
  store %"class.std::__1::__tree_node"* %239, %"class.std::__1::__tree_node"** %48, align 8
  store i64 %240, i64* %49, align 8
  %241 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %47, align 8
  %242 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %48, align 8
  %243 = bitcast %"class.std::__1::__tree_node"* %242 to i8*
  store i8* %243, i8** %46, align 8
  %244 = load i8*, i8** %46, align 8
  call void @_ZdlPv(i8* %244) #12
  br label %245

; <label>:245:                                    ; preds = %234, %231
  br label %246

; <label>:246:                                    ; preds = %186, %245
  br label %257

; <label>:247:                                    ; preds = %185, %4
  %248 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %84, align 8
  store %"class.std::__1::__tree_iterator"* %75, %"class.std::__1::__tree_iterator"** %72, align 8
  store %"class.std::__1::__tree_node"* %248, %"class.std::__1::__tree_node"** %73, align 8
  %249 = load %"class.std::__1::__tree_iterator"*, %"class.std::__1::__tree_iterator"** %72, align 8
  %250 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %73, align 8
  store %"class.std::__1::__tree_iterator"* %249, %"class.std::__1::__tree_iterator"** %70, align 8
  store %"class.std::__1::__tree_node"* %250, %"class.std::__1::__tree_node"** %71, align 8
  %251 = load %"class.std::__1::__tree_iterator"*, %"class.std::__1::__tree_iterator"** %70, align 8
  %252 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %251, i32 0, i32 0
  %253 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %71, align 8
  %254 = bitcast %"class.std::__1::__tree_node"* %253 to %"class.std::__1::__tree_end_node"*
  store %"class.std::__1::__tree_end_node"* %254, %"class.std::__1::__tree_end_node"** %252, align 8
  %255 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %75, i32 0, i32 0
  %256 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %255, align 8
  ret %"class.std::__1::__tree_end_node"* %256

; <label>:257:                                    ; preds = %246
  %258 = load i8*, i8** %86, align 8
  %259 = load i32, i32* %87, align 4
  %260 = insertvalue { i8*, i32 } undef, i8* %258, 0
  %261 = insertvalue { i8*, i32 } %260, i32 %259, 1
  resume { i8*, i32 } %261
}

; Function Attrs: ssp uwtable
define linkonce_odr dereferenceable(8) %"class.std::__1::__tree_node_base"** @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE12__find_equalIS2_EERPNS_16__tree_node_baseIPvEENS_21__tree_const_iteratorIS9_PNS_11__tree_nodeIS9_SI_EElEERPNS_15__tree_end_nodeISK_EESL_RKT_(%"class.std::__1::__tree"*, %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** dereferenceable(8), %"class.std::__1::__tree_node_base"** dereferenceable(8), i32* dereferenceable(4)) #0 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %8 = alloca %"class.std::__1::__tree"*, align 8
  %9 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %10 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %11 = alloca %"struct.std::__1::less"*, align 8
  %12 = alloca i32*, align 8
  %13 = alloca i32*, align 8
  %14 = alloca %"class.std::__1::__map_value_compare"*, align 8
  %15 = alloca i32*, align 8
  %16 = alloca %"union.std::__1::__value_type"*, align 8
  %17 = alloca %"class.std::__1::__tree_iterator"*, align 8
  %18 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %19 = alloca %"class.std::__1::__tree_iterator"*, align 8
  %20 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %21 = alloca %"class.std::__1::__tree"*, align 8
  %22 = alloca %"class.std::__1::__tree_iterator", align 8
  %23 = alloca %"class.std::__1::__tree"*, align 8
  %24 = alloca %"class.std::__1::__tree_iterator", align 8
  %25 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %26 = alloca %"class.std::__1::__tree_iterator", align 8
  %27 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %28 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %29 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %30 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %31 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %32 = alloca %"class.std::__1::__tree"*, align 8
  %33 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %34 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %35 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %36 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %37 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %38 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %39 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %40 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %41 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %42 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %43 = alloca %"struct.std::__1::less"*, align 8
  %44 = alloca i32*, align 8
  %45 = alloca i32*, align 8
  %46 = alloca %"class.std::__1::__map_value_compare"*, align 8
  %47 = alloca %"union.std::__1::__value_type"*, align 8
  %48 = alloca i32*, align 8
  %49 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %50 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %51 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %52 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %53 = alloca %"class.std::__1::__tree"*, align 8
  %54 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %55 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %56 = alloca %"struct.std::__1::less"*, align 8
  %57 = alloca i32*, align 8
  %58 = alloca i32*, align 8
  %59 = alloca %"class.std::__1::__map_value_compare"*, align 8
  %60 = alloca %"union.std::__1::__value_type"*, align 8
  %61 = alloca i32*, align 8
  %62 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %63 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %64 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %65 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %66 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %67 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %68 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %69 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %70 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %71 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %72 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %73 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %74 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %75 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %76 = alloca %"struct.std::__1::bidirectional_iterator_tag", align 1
  %77 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %78 = alloca i64, align 8
  %79 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %80 = alloca i64, align 8
  %81 = alloca %"struct.std::__1::bidirectional_iterator_tag", align 1
  %82 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %83 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %84 = alloca i64, align 8
  %85 = alloca i8*, align 8
  %86 = alloca %"class.std::__1::__tree_iterator"*, align 8
  %87 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %88 = alloca %"class.std::__1::__tree_iterator"*, align 8
  %89 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %90 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %91 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %92 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %93 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %94 = alloca %"class.std::__1::__tree"*, align 8
  %95 = alloca %"class.std::__1::__tree_iterator", align 8
  %96 = alloca %"class.std::__1::__tree"*, align 8
  %97 = alloca %"class.std::__1::__tree_iterator", align 8
  %98 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %99 = alloca %"class.std::__1::__tree_iterator", align 8
  %100 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %101 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %102 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %103 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %104 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %105 = alloca %"class.std::__1::__tree"*, align 8
  %106 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %107 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %108 = alloca %"struct.std::__1::less"*, align 8
  %109 = alloca i32*, align 8
  %110 = alloca i32*, align 8
  %111 = alloca %"class.std::__1::__map_value_compare"*, align 8
  %112 = alloca i32*, align 8
  %113 = alloca %"union.std::__1::__value_type"*, align 8
  %114 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %115 = alloca %"class.std::__1::__tree_iterator", align 8
  %116 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %117 = alloca %"class.std::__1::__tree_iterator", align 8
  %118 = alloca %"class.std::__1::__tree_const_iterator"*, align 8
  %119 = alloca %"class.std::__1::__tree_iterator"*, align 8
  %120 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %121 = alloca %"class.std::__1::__tree_iterator"*, align 8
  %122 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %123 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %124 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %125 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %126 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %127 = alloca %"class.std::__1::__tree"*, align 8
  %128 = alloca %"class.std::__1::__tree_iterator", align 8
  %129 = alloca %"class.std::__1::__tree"*, align 8
  %130 = alloca %"class.std::__1::__tree_node_base"**, align 8
  %131 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %132 = alloca %"class.std::__1::__tree"*, align 8
  %133 = alloca %"class.std::__1::__tree_end_node"**, align 8
  %134 = alloca %"class.std::__1::__tree_node_base"**, align 8
  %135 = alloca i32*, align 8
  %136 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %137 = alloca %"class.std::__1::__tree_iterator", align 8
  %138 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %139 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %140 = alloca %"class.std::__1::__tree_iterator", align 8
  %141 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %142 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %143 = alloca %"class.std::__1::__tree_const_iterator", align 8
  %144 = alloca %"class.std::__1::__tree_iterator", align 8
  %145 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %131, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %1, %"class.std::__1::__tree_end_node"** %145, align 8
  store %"class.std::__1::__tree"* %0, %"class.std::__1::__tree"** %132, align 8
  store %"class.std::__1::__tree_end_node"** %2, %"class.std::__1::__tree_end_node"*** %133, align 8
  store %"class.std::__1::__tree_node_base"** %3, %"class.std::__1::__tree_node_base"*** %134, align 8
  store i32* %4, i32** %135, align 8
  %146 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %132, align 8
  store %"class.std::__1::__tree"* %146, %"class.std::__1::__tree"** %129, align 8
  %147 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %129, align 8
  store %"class.std::__1::__tree"* %147, %"class.std::__1::__tree"** %127, align 8
  %148 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %127, align 8
  %149 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %148, i32 0, i32 1
  store %"class.std::__1::__compressed_pair"* %149, %"class.std::__1::__compressed_pair"** %126, align 8
  %150 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %126, align 8
  %151 = bitcast %"class.std::__1::__compressed_pair"* %150 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %151, %"class.std::__1::__libcpp_compressed_pair_imp"** %125, align 8
  %152 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %125, align 8
  %153 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %152, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %153, %"class.std::__1::__tree_end_node"** %124, align 8
  %154 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %124, align 8
  store %"class.std::__1::__tree_end_node"* %154, %"class.std::__1::__tree_end_node"** %123, align 8
  %155 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %123, align 8
  %156 = bitcast %"class.std::__1::__tree_end_node"* %155 to i8*
  store %"class.std::__1::__tree_iterator"* %128, %"class.std::__1::__tree_iterator"** %121, align 8
  store %"class.std::__1::__tree_end_node"* %155, %"class.std::__1::__tree_end_node"** %122, align 8
  %157 = load %"class.std::__1::__tree_iterator"*, %"class.std::__1::__tree_iterator"** %121, align 8
  %158 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %122, align 8
  store %"class.std::__1::__tree_iterator"* %157, %"class.std::__1::__tree_iterator"** %119, align 8
  store %"class.std::__1::__tree_end_node"* %158, %"class.std::__1::__tree_end_node"** %120, align 8
  %159 = load %"class.std::__1::__tree_iterator"*, %"class.std::__1::__tree_iterator"** %119, align 8
  %160 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %159, i32 0, i32 0
  %161 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %120, align 8
  store %"class.std::__1::__tree_end_node"* %161, %"class.std::__1::__tree_end_node"** %160, align 8
  %162 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %128, i32 0, i32 0
  %163 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %162, align 8
  %164 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %137, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %163, %"class.std::__1::__tree_end_node"** %164, align 8
  %165 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %137, i32 0, i32 0
  %166 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %165, align 8
  %167 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %117, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %166, %"class.std::__1::__tree_end_node"** %167, align 8
  store %"class.std::__1::__tree_const_iterator"* %136, %"class.std::__1::__tree_const_iterator"** %118, align 8
  %168 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %118, align 8
  %169 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %117, i32 0, i32 0
  %170 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %169, align 8
  %171 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %115, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %170, %"class.std::__1::__tree_end_node"** %171, align 8
  store %"class.std::__1::__tree_const_iterator"* %168, %"class.std::__1::__tree_const_iterator"** %116, align 8
  %172 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %116, align 8
  %173 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %172, i32 0, i32 0
  %174 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %115, i32 0, i32 0
  %175 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %174, align 8
  store %"class.std::__1::__tree_end_node"* %175, %"class.std::__1::__tree_end_node"** %173, align 8
  store %"class.std::__1::__tree_const_iterator"* %131, %"class.std::__1::__tree_const_iterator"** %49, align 8
  store %"class.std::__1::__tree_const_iterator"* %136, %"class.std::__1::__tree_const_iterator"** %50, align 8
  %176 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %49, align 8
  %177 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %176, i32 0, i32 0
  %178 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %177, align 8
  %179 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %50, align 8
  %180 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %179, i32 0, i32 0
  %181 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %180, align 8
  %182 = icmp eq %"class.std::__1::__tree_end_node"* %178, %181
  br i1 %182, label %209, label %183

; <label>:183:                                    ; preds = %5
  store %"class.std::__1::__tree"* %146, %"class.std::__1::__tree"** %8, align 8
  %184 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %8, align 8
  %185 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %184, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.0"* %185, %"class.std::__1::__compressed_pair.0"** %7, align 8
  %186 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %7, align 8
  %187 = bitcast %"class.std::__1::__compressed_pair.0"* %186 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %187, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %6, align 8
  %188 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %6, align 8
  %189 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.1"* %188 to %"class.std::__1::__map_value_compare"*
  %190 = load i32*, i32** %135, align 8
  store %"class.std::__1::__tree_const_iterator"* %131, %"class.std::__1::__tree_const_iterator"** %10, align 8
  %191 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %10, align 8
  store %"class.std::__1::__tree_const_iterator"* %191, %"class.std::__1::__tree_const_iterator"** %9, align 8
  %192 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %9, align 8
  %193 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %192, i32 0, i32 0
  %194 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %193, align 8
  %195 = bitcast %"class.std::__1::__tree_end_node"* %194 to %"class.std::__1::__tree_node"*
  %196 = getelementptr inbounds %"class.std::__1::__tree_node", %"class.std::__1::__tree_node"* %195, i32 0, i32 1
  store %"class.std::__1::__map_value_compare"* %189, %"class.std::__1::__map_value_compare"** %14, align 8
  store i32* %190, i32** %15, align 8
  store %"union.std::__1::__value_type"* %196, %"union.std::__1::__value_type"** %16, align 8
  %197 = load %"class.std::__1::__map_value_compare"*, %"class.std::__1::__map_value_compare"** %14, align 8
  %198 = bitcast %"class.std::__1::__map_value_compare"* %197 to %"struct.std::__1::less"*
  %199 = load i32*, i32** %15, align 8
  %200 = load %"union.std::__1::__value_type"*, %"union.std::__1::__value_type"** %16, align 8
  %201 = bitcast %"union.std::__1::__value_type"* %200 to %"struct.std::__1::pair"*
  %202 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %201, i32 0, i32 0
  store %"struct.std::__1::less"* %198, %"struct.std::__1::less"** %11, align 8
  store i32* %199, i32** %12, align 8
  store i32* %202, i32** %13, align 8
  %203 = load %"struct.std::__1::less"*, %"struct.std::__1::less"** %11, align 8
  %204 = load i32*, i32** %12, align 8
  %205 = load i32, i32* %204, align 4
  %206 = load i32*, i32** %13, align 8
  %207 = load i32, i32* %206, align 4
  %208 = icmp slt i32 %205, %207
  br label %209

; <label>:209:                                    ; preds = %183, %5
  %210 = phi i1 [ true, %5 ], [ %208, %183 ]
  br i1 %210, label %211, label %347

; <label>:211:                                    ; preds = %209
  %212 = bitcast %"class.std::__1::__tree_const_iterator"* %138 to i8*
  %213 = bitcast %"class.std::__1::__tree_const_iterator"* %131 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %212, i8* %213, i64 8, i32 8, i1 false)
  store %"class.std::__1::__tree"* %146, %"class.std::__1::__tree"** %23, align 8
  %214 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %23, align 8
  store %"class.std::__1::__tree"* %214, %"class.std::__1::__tree"** %21, align 8
  %215 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %21, align 8
  %216 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %215, i32 0, i32 0
  %217 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %216, align 8
  store %"class.std::__1::__tree_iterator"* %22, %"class.std::__1::__tree_iterator"** %19, align 8
  store %"class.std::__1::__tree_end_node"* %217, %"class.std::__1::__tree_end_node"** %20, align 8
  %218 = load %"class.std::__1::__tree_iterator"*, %"class.std::__1::__tree_iterator"** %19, align 8
  %219 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %20, align 8
  store %"class.std::__1::__tree_iterator"* %218, %"class.std::__1::__tree_iterator"** %17, align 8
  store %"class.std::__1::__tree_end_node"* %219, %"class.std::__1::__tree_end_node"** %18, align 8
  %220 = load %"class.std::__1::__tree_iterator"*, %"class.std::__1::__tree_iterator"** %17, align 8
  %221 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %220, i32 0, i32 0
  %222 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %18, align 8
  store %"class.std::__1::__tree_end_node"* %222, %"class.std::__1::__tree_end_node"** %221, align 8
  %223 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %22, i32 0, i32 0
  %224 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %223, align 8
  %225 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %140, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %224, %"class.std::__1::__tree_end_node"** %225, align 8
  %226 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %140, i32 0, i32 0
  %227 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %226, align 8
  %228 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %26, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %227, %"class.std::__1::__tree_end_node"** %228, align 8
  store %"class.std::__1::__tree_const_iterator"* %139, %"class.std::__1::__tree_const_iterator"** %27, align 8
  %229 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %27, align 8
  %230 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %26, i32 0, i32 0
  %231 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %230, align 8
  %232 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %24, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %231, %"class.std::__1::__tree_end_node"** %232, align 8
  store %"class.std::__1::__tree_const_iterator"* %229, %"class.std::__1::__tree_const_iterator"** %25, align 8
  %233 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %25, align 8
  %234 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %233, i32 0, i32 0
  %235 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %24, i32 0, i32 0
  %236 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %235, align 8
  store %"class.std::__1::__tree_end_node"* %236, %"class.std::__1::__tree_end_node"** %234, align 8
  store %"class.std::__1::__tree_const_iterator"* %138, %"class.std::__1::__tree_const_iterator"** %28, align 8
  store %"class.std::__1::__tree_const_iterator"* %139, %"class.std::__1::__tree_const_iterator"** %29, align 8
  %237 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %28, align 8
  %238 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %237, i32 0, i32 0
  %239 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %238, align 8
  %240 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %29, align 8
  %241 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %240, i32 0, i32 0
  %242 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %241, align 8
  %243 = icmp eq %"class.std::__1::__tree_end_node"* %239, %242
  br i1 %243, label %320, label %244

; <label>:244:                                    ; preds = %211
  store %"class.std::__1::__tree"* %146, %"class.std::__1::__tree"** %32, align 8
  %245 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %32, align 8
  %246 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %245, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.0"* %246, %"class.std::__1::__compressed_pair.0"** %31, align 8
  %247 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %31, align 8
  %248 = bitcast %"class.std::__1::__compressed_pair.0"* %247 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %248, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %30, align 8
  %249 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %30, align 8
  %250 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.1"* %249 to %"class.std::__1::__map_value_compare"*
  store %"class.std::__1::__tree_const_iterator"* %138, %"class.std::__1::__tree_const_iterator"** %40, align 8
  %251 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %40, align 8
  %252 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %251, i32 0, i32 0
  %253 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %252, align 8
  store %"class.std::__1::__tree_end_node"* %253, %"class.std::__1::__tree_end_node"** %38, align 8
  %254 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %38, align 8
  %255 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %254, i32 0, i32 0
  %256 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %255, align 8
  %257 = icmp ne %"class.std::__1::__tree_node_base"* %256, null
  br i1 %257, label %258, label %273

; <label>:258:                                    ; preds = %244
  %259 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %38, align 8
  %260 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %259, i32 0, i32 0
  %261 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %260, align 8
  store %"class.std::__1::__tree_node_base"* %261, %"class.std::__1::__tree_node_base"** %36, align 8
  br label %262

; <label>:262:                                    ; preds = %267, %258
  %263 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %36, align 8
  %264 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %263, i32 0, i32 1
  %265 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %264, align 8
  %266 = icmp ne %"class.std::__1::__tree_node_base"* %265, null
  br i1 %266, label %267, label %271

; <label>:267:                                    ; preds = %262
  %268 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %36, align 8
  %269 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %268, i32 0, i32 1
  %270 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %269, align 8
  store %"class.std::__1::__tree_node_base"* %270, %"class.std::__1::__tree_node_base"** %36, align 8
  br label %262

; <label>:271:                                    ; preds = %262
  %272 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %36, align 8
  store %"class.std::__1::__tree_node_base"* %272, %"class.std::__1::__tree_node_base"** %37, align 8
  br label %297

; <label>:273:                                    ; preds = %244
  %274 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %38, align 8
  %275 = bitcast %"class.std::__1::__tree_end_node"* %274 to %"class.std::__1::__tree_node_base"*
  store %"class.std::__1::__tree_node_base"* %275, %"class.std::__1::__tree_node_base"** %39, align 8
  br label %276

; <label>:276:                                    ; preds = %285, %273
  %277 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %39, align 8
  store %"class.std::__1::__tree_node_base"* %277, %"class.std::__1::__tree_node_base"** %35, align 8
  %278 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %35, align 8
  %279 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %35, align 8
  %280 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %279, i32 0, i32 2
  %281 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %280, align 8
  %282 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %281, i32 0, i32 0
  %283 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %282, align 8
  %284 = icmp eq %"class.std::__1::__tree_node_base"* %278, %283
  br i1 %284, label %285, label %291

; <label>:285:                                    ; preds = %276
  %286 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %39, align 8
  store %"class.std::__1::__tree_node_base"* %286, %"class.std::__1::__tree_node_base"** %33, align 8
  %287 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %33, align 8
  %288 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %287, i32 0, i32 2
  %289 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %288, align 8
  %290 = bitcast %"class.std::__1::__tree_end_node"* %289 to %"class.std::__1::__tree_node_base"*
  store %"class.std::__1::__tree_node_base"* %290, %"class.std::__1::__tree_node_base"** %39, align 8
  br label %276

; <label>:291:                                    ; preds = %276
  %292 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %39, align 8
  store %"class.std::__1::__tree_node_base"* %292, %"class.std::__1::__tree_node_base"** %34, align 8
  %293 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %34, align 8
  %294 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %293, i32 0, i32 2
  %295 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %294, align 8
  %296 = bitcast %"class.std::__1::__tree_end_node"* %295 to %"class.std::__1::__tree_node_base"*
  store %"class.std::__1::__tree_node_base"* %296, %"class.std::__1::__tree_node_base"** %37, align 8
  br label %297

; <label>:297:                                    ; preds = %271, %291
  %298 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %37, align 8
  %299 = bitcast %"class.std::__1::__tree_node_base"* %298 to %"class.std::__1::__tree_end_node"*
  %300 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %251, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %299, %"class.std::__1::__tree_end_node"** %300, align 8
  store %"class.std::__1::__tree_const_iterator"* %251, %"class.std::__1::__tree_const_iterator"** %42, align 8
  %301 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %42, align 8
  store %"class.std::__1::__tree_const_iterator"* %301, %"class.std::__1::__tree_const_iterator"** %41, align 8
  %302 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %41, align 8
  %303 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %302, i32 0, i32 0
  %304 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %303, align 8
  %305 = bitcast %"class.std::__1::__tree_end_node"* %304 to %"class.std::__1::__tree_node"*
  %306 = getelementptr inbounds %"class.std::__1::__tree_node", %"class.std::__1::__tree_node"* %305, i32 0, i32 1
  %307 = load i32*, i32** %135, align 8
  store %"class.std::__1::__map_value_compare"* %250, %"class.std::__1::__map_value_compare"** %46, align 8
  store %"union.std::__1::__value_type"* %306, %"union.std::__1::__value_type"** %47, align 8
  store i32* %307, i32** %48, align 8
  %308 = load %"class.std::__1::__map_value_compare"*, %"class.std::__1::__map_value_compare"** %46, align 8
  %309 = bitcast %"class.std::__1::__map_value_compare"* %308 to %"struct.std::__1::less"*
  %310 = load %"union.std::__1::__value_type"*, %"union.std::__1::__value_type"** %47, align 8
  %311 = bitcast %"union.std::__1::__value_type"* %310 to %"struct.std::__1::pair"*
  %312 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %311, i32 0, i32 0
  %313 = load i32*, i32** %48, align 8
  store %"struct.std::__1::less"* %309, %"struct.std::__1::less"** %43, align 8
  store i32* %312, i32** %44, align 8
  store i32* %313, i32** %45, align 8
  %314 = load %"struct.std::__1::less"*, %"struct.std::__1::less"** %43, align 8
  %315 = load i32*, i32** %44, align 8
  %316 = load i32, i32* %315, align 4
  %317 = load i32*, i32** %45, align 8
  %318 = load i32, i32* %317, align 4
  %319 = icmp slt i32 %316, %318
  br label %320

; <label>:320:                                    ; preds = %297, %211
  %321 = phi i1 [ true, %211 ], [ %319, %297 ]
  br i1 %321, label %322, label %343

; <label>:322:                                    ; preds = %320
  %323 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %131, i32 0, i32 0
  %324 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %323, align 8
  %325 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %324, i32 0, i32 0
  %326 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %325, align 8
  %327 = icmp eq %"class.std::__1::__tree_node_base"* %326, null
  br i1 %327, label %328, label %335

; <label>:328:                                    ; preds = %322
  %329 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %131, i32 0, i32 0
  %330 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %329, align 8
  %331 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %133, align 8
  store %"class.std::__1::__tree_end_node"* %330, %"class.std::__1::__tree_end_node"** %331, align 8
  %332 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %133, align 8
  %333 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %332, align 8
  %334 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %333, i32 0, i32 0
  store %"class.std::__1::__tree_node_base"** %334, %"class.std::__1::__tree_node_base"*** %130, align 8
  br label %610

; <label>:335:                                    ; preds = %322
  %336 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %138, i32 0, i32 0
  %337 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %336, align 8
  %338 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %133, align 8
  store %"class.std::__1::__tree_end_node"* %337, %"class.std::__1::__tree_end_node"** %338, align 8
  %339 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %138, i32 0, i32 0
  %340 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %339, align 8
  %341 = bitcast %"class.std::__1::__tree_end_node"* %340 to %"class.std::__1::__tree_node_base"*
  %342 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %341, i32 0, i32 1
  store %"class.std::__1::__tree_node_base"** %342, %"class.std::__1::__tree_node_base"*** %130, align 8
  br label %610

; <label>:343:                                    ; preds = %320
  %344 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %133, align 8
  %345 = load i32*, i32** %135, align 8
  %346 = call dereferenceable(8) %"class.std::__1::__tree_node_base"** @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE12__find_equalIS2_EERPNS_16__tree_node_baseIPvEERPNS_15__tree_end_nodeISK_EERKT_(%"class.std::__1::__tree"* %146, %"class.std::__1::__tree_end_node"** dereferenceable(8) %344, i32* dereferenceable(4) %345)
  store %"class.std::__1::__tree_node_base"** %346, %"class.std::__1::__tree_node_base"*** %130, align 8
  br label %610

; <label>:347:                                    ; preds = %209
  store %"class.std::__1::__tree"* %146, %"class.std::__1::__tree"** %53, align 8
  %348 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %53, align 8
  %349 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %348, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.0"* %349, %"class.std::__1::__compressed_pair.0"** %52, align 8
  %350 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %52, align 8
  %351 = bitcast %"class.std::__1::__compressed_pair.0"* %350 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %351, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %51, align 8
  %352 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %51, align 8
  %353 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.1"* %352 to %"class.std::__1::__map_value_compare"*
  store %"class.std::__1::__tree_const_iterator"* %131, %"class.std::__1::__tree_const_iterator"** %55, align 8
  %354 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %55, align 8
  store %"class.std::__1::__tree_const_iterator"* %354, %"class.std::__1::__tree_const_iterator"** %54, align 8
  %355 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %54, align 8
  %356 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %355, i32 0, i32 0
  %357 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %356, align 8
  %358 = bitcast %"class.std::__1::__tree_end_node"* %357 to %"class.std::__1::__tree_node"*
  %359 = getelementptr inbounds %"class.std::__1::__tree_node", %"class.std::__1::__tree_node"* %358, i32 0, i32 1
  %360 = load i32*, i32** %135, align 8
  store %"class.std::__1::__map_value_compare"* %353, %"class.std::__1::__map_value_compare"** %59, align 8
  store %"union.std::__1::__value_type"* %359, %"union.std::__1::__value_type"** %60, align 8
  store i32* %360, i32** %61, align 8
  %361 = load %"class.std::__1::__map_value_compare"*, %"class.std::__1::__map_value_compare"** %59, align 8
  %362 = bitcast %"class.std::__1::__map_value_compare"* %361 to %"struct.std::__1::less"*
  %363 = load %"union.std::__1::__value_type"*, %"union.std::__1::__value_type"** %60, align 8
  %364 = bitcast %"union.std::__1::__value_type"* %363 to %"struct.std::__1::pair"*
  %365 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %364, i32 0, i32 0
  %366 = load i32*, i32** %61, align 8
  store %"struct.std::__1::less"* %362, %"struct.std::__1::less"** %56, align 8
  store i32* %365, i32** %57, align 8
  store i32* %366, i32** %58, align 8
  %367 = load %"struct.std::__1::less"*, %"struct.std::__1::less"** %56, align 8
  %368 = load i32*, i32** %57, align 8
  %369 = load i32, i32* %368, align 4
  %370 = load i32*, i32** %58, align 8
  %371 = load i32, i32* %370, align 4
  %372 = icmp slt i32 %369, %371
  br i1 %372, label %373, label %600

; <label>:373:                                    ; preds = %347
  %374 = bitcast %"class.std::__1::__tree_const_iterator"* %142 to i8*
  %375 = bitcast %"class.std::__1::__tree_const_iterator"* %131 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %374, i8* %375, i64 8, i32 8, i1 false)
  %376 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %142, i32 0, i32 0
  %377 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %376, align 8
  %378 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %83, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %377, %"class.std::__1::__tree_end_node"** %378, align 8
  store i64 1, i64* %84, align 8
  store i8* null, i8** %85, align 8
  %379 = load i64, i64* %84, align 8
  store %"class.std::__1::__tree_const_iterator"* %83, %"class.std::__1::__tree_const_iterator"** %79, align 8
  store i64 %379, i64* %80, align 8
  %380 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %79, align 8
  %381 = load i64, i64* %80, align 8
  store %"class.std::__1::__tree_const_iterator"* %380, %"class.std::__1::__tree_const_iterator"** %77, align 8
  store i64 %381, i64* %78, align 8
  %382 = load i64, i64* %78, align 8
  %383 = icmp sge i64 %382, 0
  br i1 %383, label %384, label %443

; <label>:384:                                    ; preds = %373
  br label %385

; <label>:385:                                    ; preds = %437, %384
  %386 = load i64, i64* %78, align 8
  %387 = icmp sgt i64 %386, 0
  br i1 %387, label %388, label %442

; <label>:388:                                    ; preds = %385
  %389 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %77, align 8
  store %"class.std::__1::__tree_const_iterator"* %389, %"class.std::__1::__tree_const_iterator"** %75, align 8
  %390 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %75, align 8
  %391 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %390, i32 0, i32 0
  %392 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %391, align 8
  %393 = bitcast %"class.std::__1::__tree_end_node"* %392 to %"class.std::__1::__tree_node_base"*
  store %"class.std::__1::__tree_node_base"* %393, %"class.std::__1::__tree_node_base"** %74, align 8
  %394 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %74, align 8
  %395 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %394, i32 0, i32 1
  %396 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %395, align 8
  %397 = icmp ne %"class.std::__1::__tree_node_base"* %396, null
  br i1 %397, label %398, label %416

; <label>:398:                                    ; preds = %388
  %399 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %74, align 8
  %400 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %399, i32 0, i32 1
  %401 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %400, align 8
  store %"class.std::__1::__tree_node_base"* %401, %"class.std::__1::__tree_node_base"** %72, align 8
  br label %402

; <label>:402:                                    ; preds = %408, %398
  %403 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %72, align 8
  %404 = bitcast %"class.std::__1::__tree_node_base"* %403 to %"class.std::__1::__tree_end_node"*
  %405 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %404, i32 0, i32 0
  %406 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %405, align 8
  %407 = icmp ne %"class.std::__1::__tree_node_base"* %406, null
  br i1 %407, label %408, label %413

; <label>:408:                                    ; preds = %402
  %409 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %72, align 8
  %410 = bitcast %"class.std::__1::__tree_node_base"* %409 to %"class.std::__1::__tree_end_node"*
  %411 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %410, i32 0, i32 0
  %412 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %411, align 8
  store %"class.std::__1::__tree_node_base"* %412, %"class.std::__1::__tree_node_base"** %72, align 8
  br label %402

; <label>:413:                                    ; preds = %402
  %414 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %72, align 8
  %415 = bitcast %"class.std::__1::__tree_node_base"* %414 to %"class.std::__1::__tree_end_node"*
  store %"class.std::__1::__tree_end_node"* %415, %"class.std::__1::__tree_end_node"** %73, align 8
  br label %437

; <label>:416:                                    ; preds = %388
  br label %417

; <label>:417:                                    ; preds = %427, %416
  %418 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %74, align 8
  store %"class.std::__1::__tree_node_base"* %418, %"class.std::__1::__tree_node_base"** %71, align 8
  %419 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %71, align 8
  %420 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %71, align 8
  %421 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %420, i32 0, i32 2
  %422 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %421, align 8
  %423 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %422, i32 0, i32 0
  %424 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %423, align 8
  %425 = icmp eq %"class.std::__1::__tree_node_base"* %419, %424
  %426 = xor i1 %425, true
  br i1 %426, label %427, label %433

; <label>:427:                                    ; preds = %417
  %428 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %74, align 8
  store %"class.std::__1::__tree_node_base"* %428, %"class.std::__1::__tree_node_base"** %70, align 8
  %429 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %70, align 8
  %430 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %429, i32 0, i32 2
  %431 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %430, align 8
  %432 = bitcast %"class.std::__1::__tree_end_node"* %431 to %"class.std::__1::__tree_node_base"*
  store %"class.std::__1::__tree_node_base"* %432, %"class.std::__1::__tree_node_base"** %74, align 8
  br label %417

; <label>:433:                                    ; preds = %417
  %434 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %74, align 8
  %435 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %434, i32 0, i32 2
  %436 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %435, align 8
  store %"class.std::__1::__tree_end_node"* %436, %"class.std::__1::__tree_end_node"** %73, align 8
  br label %437

; <label>:437:                                    ; preds = %433, %413
  %438 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %73, align 8
  %439 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %390, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %438, %"class.std::__1::__tree_end_node"** %439, align 8
  %440 = load i64, i64* %78, align 8
  %441 = add nsw i64 %440, -1
  store i64 %441, i64* %78, align 8
  br label %385

; <label>:442:                                    ; preds = %385
  br label %502

; <label>:443:                                    ; preds = %373
  br label %444

; <label>:444:                                    ; preds = %495, %443
  %445 = load i64, i64* %78, align 8
  %446 = icmp slt i64 %445, 0
  br i1 %446, label %447, label %501

; <label>:447:                                    ; preds = %444
  %448 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %77, align 8
  store %"class.std::__1::__tree_const_iterator"* %448, %"class.std::__1::__tree_const_iterator"** %69, align 8
  %449 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %69, align 8
  %450 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %449, i32 0, i32 0
  %451 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %450, align 8
  store %"class.std::__1::__tree_end_node"* %451, %"class.std::__1::__tree_end_node"** %67, align 8
  %452 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %67, align 8
  %453 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %452, i32 0, i32 0
  %454 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %453, align 8
  %455 = icmp ne %"class.std::__1::__tree_node_base"* %454, null
  br i1 %455, label %456, label %471

; <label>:456:                                    ; preds = %447
  %457 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %67, align 8
  %458 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %457, i32 0, i32 0
  %459 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %458, align 8
  store %"class.std::__1::__tree_node_base"* %459, %"class.std::__1::__tree_node_base"** %65, align 8
  br label %460

; <label>:460:                                    ; preds = %465, %456
  %461 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %65, align 8
  %462 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %461, i32 0, i32 1
  %463 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %462, align 8
  %464 = icmp ne %"class.std::__1::__tree_node_base"* %463, null
  br i1 %464, label %465, label %469

; <label>:465:                                    ; preds = %460
  %466 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %65, align 8
  %467 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %466, i32 0, i32 1
  %468 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %467, align 8
  store %"class.std::__1::__tree_node_base"* %468, %"class.std::__1::__tree_node_base"** %65, align 8
  br label %460

; <label>:469:                                    ; preds = %460
  %470 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %65, align 8
  store %"class.std::__1::__tree_node_base"* %470, %"class.std::__1::__tree_node_base"** %66, align 8
  br label %495

; <label>:471:                                    ; preds = %447
  %472 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %67, align 8
  %473 = bitcast %"class.std::__1::__tree_end_node"* %472 to %"class.std::__1::__tree_node_base"*
  store %"class.std::__1::__tree_node_base"* %473, %"class.std::__1::__tree_node_base"** %68, align 8
  br label %474

; <label>:474:                                    ; preds = %483, %471
  %475 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %68, align 8
  store %"class.std::__1::__tree_node_base"* %475, %"class.std::__1::__tree_node_base"** %64, align 8
  %476 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %64, align 8
  %477 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %64, align 8
  %478 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %477, i32 0, i32 2
  %479 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %478, align 8
  %480 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %479, i32 0, i32 0
  %481 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %480, align 8
  %482 = icmp eq %"class.std::__1::__tree_node_base"* %476, %481
  br i1 %482, label %483, label %489

; <label>:483:                                    ; preds = %474
  %484 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %68, align 8
  store %"class.std::__1::__tree_node_base"* %484, %"class.std::__1::__tree_node_base"** %62, align 8
  %485 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %62, align 8
  %486 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %485, i32 0, i32 2
  %487 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %486, align 8
  %488 = bitcast %"class.std::__1::__tree_end_node"* %487 to %"class.std::__1::__tree_node_base"*
  store %"class.std::__1::__tree_node_base"* %488, %"class.std::__1::__tree_node_base"** %68, align 8
  br label %474

; <label>:489:                                    ; preds = %474
  %490 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %68, align 8
  store %"class.std::__1::__tree_node_base"* %490, %"class.std::__1::__tree_node_base"** %63, align 8
  %491 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %63, align 8
  %492 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %491, i32 0, i32 2
  %493 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %492, align 8
  %494 = bitcast %"class.std::__1::__tree_end_node"* %493 to %"class.std::__1::__tree_node_base"*
  store %"class.std::__1::__tree_node_base"* %494, %"class.std::__1::__tree_node_base"** %66, align 8
  br label %495

; <label>:495:                                    ; preds = %489, %469
  %496 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %66, align 8
  %497 = bitcast %"class.std::__1::__tree_node_base"* %496 to %"class.std::__1::__tree_end_node"*
  %498 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %449, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %497, %"class.std::__1::__tree_end_node"** %498, align 8
  %499 = load i64, i64* %78, align 8
  %500 = add nsw i64 %499, 1
  store i64 %500, i64* %78, align 8
  br label %444

; <label>:501:                                    ; preds = %444
  br label %502

; <label>:502:                                    ; preds = %442, %501
  %503 = bitcast %"class.std::__1::__tree_const_iterator"* %82 to i8*
  %504 = bitcast %"class.std::__1::__tree_const_iterator"* %83 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %503, i8* %504, i64 8, i32 8, i1 false)
  %505 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %82, i32 0, i32 0
  %506 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %505, align 8
  %507 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %141, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %506, %"class.std::__1::__tree_end_node"** %507, align 8
  store %"class.std::__1::__tree"* %146, %"class.std::__1::__tree"** %96, align 8
  %508 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %96, align 8
  store %"class.std::__1::__tree"* %508, %"class.std::__1::__tree"** %94, align 8
  %509 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %94, align 8
  %510 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %509, i32 0, i32 1
  store %"class.std::__1::__compressed_pair"* %510, %"class.std::__1::__compressed_pair"** %93, align 8
  %511 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %93, align 8
  %512 = bitcast %"class.std::__1::__compressed_pair"* %511 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %512, %"class.std::__1::__libcpp_compressed_pair_imp"** %92, align 8
  %513 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %92, align 8
  %514 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %513, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %514, %"class.std::__1::__tree_end_node"** %91, align 8
  %515 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %91, align 8
  store %"class.std::__1::__tree_end_node"* %515, %"class.std::__1::__tree_end_node"** %90, align 8
  %516 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %90, align 8
  %517 = bitcast %"class.std::__1::__tree_end_node"* %516 to i8*
  store %"class.std::__1::__tree_iterator"* %95, %"class.std::__1::__tree_iterator"** %88, align 8
  store %"class.std::__1::__tree_end_node"* %516, %"class.std::__1::__tree_end_node"** %89, align 8
  %518 = load %"class.std::__1::__tree_iterator"*, %"class.std::__1::__tree_iterator"** %88, align 8
  %519 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %89, align 8
  store %"class.std::__1::__tree_iterator"* %518, %"class.std::__1::__tree_iterator"** %86, align 8
  store %"class.std::__1::__tree_end_node"* %519, %"class.std::__1::__tree_end_node"** %87, align 8
  %520 = load %"class.std::__1::__tree_iterator"*, %"class.std::__1::__tree_iterator"** %86, align 8
  %521 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %520, i32 0, i32 0
  %522 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %87, align 8
  store %"class.std::__1::__tree_end_node"* %522, %"class.std::__1::__tree_end_node"** %521, align 8
  %523 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %95, i32 0, i32 0
  %524 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %523, align 8
  %525 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %144, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %524, %"class.std::__1::__tree_end_node"** %525, align 8
  %526 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %144, i32 0, i32 0
  %527 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %526, align 8
  %528 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %99, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %527, %"class.std::__1::__tree_end_node"** %528, align 8
  store %"class.std::__1::__tree_const_iterator"* %143, %"class.std::__1::__tree_const_iterator"** %100, align 8
  %529 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %100, align 8
  %530 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %99, i32 0, i32 0
  %531 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %530, align 8
  %532 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %97, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %531, %"class.std::__1::__tree_end_node"** %532, align 8
  store %"class.std::__1::__tree_const_iterator"* %529, %"class.std::__1::__tree_const_iterator"** %98, align 8
  %533 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %98, align 8
  %534 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %533, i32 0, i32 0
  %535 = getelementptr inbounds %"class.std::__1::__tree_iterator", %"class.std::__1::__tree_iterator"* %97, i32 0, i32 0
  %536 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %535, align 8
  store %"class.std::__1::__tree_end_node"* %536, %"class.std::__1::__tree_end_node"** %534, align 8
  store %"class.std::__1::__tree_const_iterator"* %141, %"class.std::__1::__tree_const_iterator"** %101, align 8
  store %"class.std::__1::__tree_const_iterator"* %143, %"class.std::__1::__tree_const_iterator"** %102, align 8
  %537 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %101, align 8
  %538 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %537, i32 0, i32 0
  %539 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %538, align 8
  %540 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %102, align 8
  %541 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %540, i32 0, i32 0
  %542 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %541, align 8
  %543 = icmp eq %"class.std::__1::__tree_end_node"* %539, %542
  br i1 %543, label %570, label %544

; <label>:544:                                    ; preds = %502
  store %"class.std::__1::__tree"* %146, %"class.std::__1::__tree"** %105, align 8
  %545 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %105, align 8
  %546 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %545, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.0"* %546, %"class.std::__1::__compressed_pair.0"** %104, align 8
  %547 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %104, align 8
  %548 = bitcast %"class.std::__1::__compressed_pair.0"* %547 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %548, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %103, align 8
  %549 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %103, align 8
  %550 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.1"* %549 to %"class.std::__1::__map_value_compare"*
  %551 = load i32*, i32** %135, align 8
  store %"class.std::__1::__tree_const_iterator"* %141, %"class.std::__1::__tree_const_iterator"** %107, align 8
  %552 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %107, align 8
  store %"class.std::__1::__tree_const_iterator"* %552, %"class.std::__1::__tree_const_iterator"** %106, align 8
  %553 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %106, align 8
  %554 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %553, i32 0, i32 0
  %555 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %554, align 8
  %556 = bitcast %"class.std::__1::__tree_end_node"* %555 to %"class.std::__1::__tree_node"*
  %557 = getelementptr inbounds %"class.std::__1::__tree_node", %"class.std::__1::__tree_node"* %556, i32 0, i32 1
  store %"class.std::__1::__map_value_compare"* %550, %"class.std::__1::__map_value_compare"** %111, align 8
  store i32* %551, i32** %112, align 8
  store %"union.std::__1::__value_type"* %557, %"union.std::__1::__value_type"** %113, align 8
  %558 = load %"class.std::__1::__map_value_compare"*, %"class.std::__1::__map_value_compare"** %111, align 8
  %559 = bitcast %"class.std::__1::__map_value_compare"* %558 to %"struct.std::__1::less"*
  %560 = load i32*, i32** %112, align 8
  %561 = load %"union.std::__1::__value_type"*, %"union.std::__1::__value_type"** %113, align 8
  %562 = bitcast %"union.std::__1::__value_type"* %561 to %"struct.std::__1::pair"*
  %563 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %562, i32 0, i32 0
  store %"struct.std::__1::less"* %559, %"struct.std::__1::less"** %108, align 8
  store i32* %560, i32** %109, align 8
  store i32* %563, i32** %110, align 8
  %564 = load %"struct.std::__1::less"*, %"struct.std::__1::less"** %108, align 8
  %565 = load i32*, i32** %109, align 8
  %566 = load i32, i32* %565, align 4
  %567 = load i32*, i32** %110, align 8
  %568 = load i32, i32* %567, align 4
  %569 = icmp slt i32 %566, %568
  br label %570

; <label>:570:                                    ; preds = %544, %502
  %571 = phi i1 [ true, %502 ], [ %569, %544 ]
  br i1 %571, label %572, label %596

; <label>:572:                                    ; preds = %570
  store %"class.std::__1::__tree_const_iterator"* %131, %"class.std::__1::__tree_const_iterator"** %114, align 8
  %573 = load %"class.std::__1::__tree_const_iterator"*, %"class.std::__1::__tree_const_iterator"** %114, align 8
  %574 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %573, i32 0, i32 0
  %575 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %574, align 8
  %576 = bitcast %"class.std::__1::__tree_end_node"* %575 to %"class.std::__1::__tree_node"*
  %577 = bitcast %"class.std::__1::__tree_node"* %576 to %"class.std::__1::__tree_node_base"*
  %578 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %577, i32 0, i32 1
  %579 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %578, align 8
  %580 = icmp eq %"class.std::__1::__tree_node_base"* %579, null
  br i1 %580, label %581, label %589

; <label>:581:                                    ; preds = %572
  %582 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %131, i32 0, i32 0
  %583 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %582, align 8
  %584 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %133, align 8
  store %"class.std::__1::__tree_end_node"* %583, %"class.std::__1::__tree_end_node"** %584, align 8
  %585 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %131, i32 0, i32 0
  %586 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %585, align 8
  %587 = bitcast %"class.std::__1::__tree_end_node"* %586 to %"class.std::__1::__tree_node_base"*
  %588 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %587, i32 0, i32 1
  store %"class.std::__1::__tree_node_base"** %588, %"class.std::__1::__tree_node_base"*** %130, align 8
  br label %610

; <label>:589:                                    ; preds = %572
  %590 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %141, i32 0, i32 0
  %591 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %590, align 8
  %592 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %133, align 8
  store %"class.std::__1::__tree_end_node"* %591, %"class.std::__1::__tree_end_node"** %592, align 8
  %593 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %133, align 8
  %594 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %593, align 8
  %595 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %594, i32 0, i32 0
  store %"class.std::__1::__tree_node_base"** %595, %"class.std::__1::__tree_node_base"*** %130, align 8
  br label %610

; <label>:596:                                    ; preds = %570
  %597 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %133, align 8
  %598 = load i32*, i32** %135, align 8
  %599 = call dereferenceable(8) %"class.std::__1::__tree_node_base"** @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE12__find_equalIS2_EERPNS_16__tree_node_baseIPvEERPNS_15__tree_end_nodeISK_EERKT_(%"class.std::__1::__tree"* %146, %"class.std::__1::__tree_end_node"** dereferenceable(8) %597, i32* dereferenceable(4) %598)
  store %"class.std::__1::__tree_node_base"** %599, %"class.std::__1::__tree_node_base"*** %130, align 8
  br label %610

; <label>:600:                                    ; preds = %347
  br label %601

; <label>:601:                                    ; preds = %600
  %602 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %131, i32 0, i32 0
  %603 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %602, align 8
  %604 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %133, align 8
  store %"class.std::__1::__tree_end_node"* %603, %"class.std::__1::__tree_end_node"** %604, align 8
  %605 = getelementptr inbounds %"class.std::__1::__tree_const_iterator", %"class.std::__1::__tree_const_iterator"* %131, i32 0, i32 0
  %606 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %605, align 8
  %607 = bitcast %"class.std::__1::__tree_end_node"* %606 to %"class.std::__1::__tree_node_base"*
  %608 = load %"class.std::__1::__tree_node_base"**, %"class.std::__1::__tree_node_base"*** %134, align 8
  store %"class.std::__1::__tree_node_base"* %607, %"class.std::__1::__tree_node_base"** %608, align 8
  %609 = load %"class.std::__1::__tree_node_base"**, %"class.std::__1::__tree_node_base"*** %134, align 8
  store %"class.std::__1::__tree_node_base"** %609, %"class.std::__1::__tree_node_base"*** %130, align 8
  br label %610

; <label>:610:                                    ; preds = %601, %596, %589, %581, %343, %335, %328
  %611 = load %"class.std::__1::__tree_node_base"**, %"class.std::__1::__tree_node_base"*** %130, align 8
  ret %"class.std::__1::__tree_node_base"** %611
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE16__construct_nodeIJRKNS_4pairIKS2_S8_EEEEENS_10unique_ptrINS_11__tree_nodeIS9_PvEENS_22__tree_node_destructorINS6_ISP_EEEEEEDpOT_(%"class.std::__1::unique_ptr.41"* noalias sret, %"class.std::__1::__tree"*, %"struct.std::__1::pair"* dereferenceable(32)) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"struct.std::__1::pair"*, align 8
  %5 = alloca %"struct.std::__1::pair"*, align 8
  %6 = alloca %"struct.std::__1::pair"*, align 8
  %7 = alloca %"struct.std::__1::pair"*, align 8
  %8 = alloca %"struct.std::__1::pair"*, align 8
  %9 = alloca %"struct.std::__1::pair"*, align 8
  %10 = alloca %"class.std::__1::allocator"*, align 8
  %11 = alloca %"struct.std::__1::pair"*, align 8
  %12 = alloca %"struct.std::__1::pair"*, align 8
  %13 = alloca %"struct.std::__1::pair"*, align 8
  %14 = alloca %"struct.std::__1::integral_constant.35", align 1
  %15 = alloca %"class.std::__1::allocator"*, align 8
  %16 = alloca %"struct.std::__1::pair"*, align 8
  %17 = alloca %"struct.std::__1::pair"*, align 8
  %18 = alloca %"struct.std::__1::pair"*, align 8
  %19 = alloca %"class.std::__1::allocator"*, align 8
  %20 = alloca %"struct.std::__1::pair"*, align 8
  %21 = alloca %"struct.std::__1::pair"*, align 8
  %22 = alloca %"struct.std::__1::integral_constant.35", align 1
  %23 = alloca %"struct.std::__1::__has_construct", align 1
  %24 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %25 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %26 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %27 = alloca %"struct.std::__1::pair"*, align 8
  %28 = alloca %"union.std::__1::__value_type"*, align 8
  %29 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %30 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %31 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %32 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %33 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %34 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %35 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %36 = alloca i8*, align 8
  %37 = alloca %"class.std::__1::allocator"*, align 8
  %38 = alloca %"class.std::__1::__tree_node"*, align 8
  %39 = alloca i64, align 8
  %40 = alloca %"class.std::__1::allocator"*, align 8
  %41 = alloca %"class.std::__1::__tree_node"*, align 8
  %42 = alloca i64, align 8
  %43 = alloca %"class.std::__1::allocator"*, align 8
  %44 = alloca %"struct.std::__1::pair"*, align 8
  %45 = alloca %"class.std::__1::allocator"*, align 8
  %46 = alloca %"struct.std::__1::pair"*, align 8
  %47 = alloca %"struct.std::__1::integral_constant", align 1
  %48 = alloca %"struct.std::__1::__has_destroy", align 1
  %49 = alloca %"struct.std::__1::pair"*, align 8
  %50 = alloca %"union.std::__1::__value_type"*, align 8
  %51 = alloca %"class.std::__1::__tree_node_destructor"*, align 8
  %52 = alloca %"class.std::__1::__tree_node"*, align 8
  %53 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %54 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %55 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %56 = alloca %"class.std::__1::__tree_node"*, align 8
  %57 = alloca %"class.std::__1::__tree_node"*, align 8
  %58 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %59 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %60 = alloca %"class.std::__1::__tree_node_destructor"*, align 8
  %61 = alloca %"class.std::__1::__tree_node_destructor"*, align 8
  %62 = alloca %"class.std::__1::__tree_node"**, align 8
  %63 = alloca %"class.std::__1::__tree_node_destructor", align 8
  %64 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %65 = alloca %"class.std::__1::__tree_node"*, align 8
  %66 = alloca %"class.std::__1::__tree_node"**, align 8
  %67 = alloca %"class.std::__1::__tree_node_destructor", align 8
  %68 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %69 = alloca %"class.std::__1::__tree_node"*, align 8
  %70 = alloca %"class.std::__1::__tree_node_destructor", align 8
  %71 = alloca %"class.std::__1::__tree_node_destructor", align 8
  %72 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %73 = alloca %"class.std::__1::__tree_node"*, align 8
  %74 = alloca %"class.std::__1::__tree_node_destructor"*, align 8
  %75 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %76 = alloca %"class.std::__1::__tree_node"*, align 8
  %77 = alloca %"class.std::__1::__tree_node_destructor"*, align 8
  %78 = alloca %"class.std::__1::__tree_node_destructor", align 8
  %79 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %80 = alloca %"class.std::__1::__tree_node"*, align 8
  %81 = alloca %"class.std::__1::__tree_node_destructor"*, align 8
  %82 = alloca %"class.std::__1::__tree_node_destructor"*, align 8
  %83 = alloca %"class.std::__1::allocator"*, align 8
  %84 = alloca i8, align 1
  %85 = alloca %"class.std::__1::__tree_node_destructor"*, align 8
  %86 = alloca %"class.std::__1::allocator"*, align 8
  %87 = alloca i8, align 1
  %88 = alloca i64, align 8
  %89 = alloca %"class.std::__1::allocator"*, align 8
  %90 = alloca i64, align 8
  %91 = alloca i8*, align 8
  %92 = alloca %"class.std::__1::allocator"*, align 8
  %93 = alloca i64, align 8
  %94 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %95 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %96 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %97 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %98 = alloca i8*, align 8
  %99 = alloca %"class.std::__1::allocator"*, align 8
  %100 = alloca %"class.std::__1::__tree_node"*, align 8
  %101 = alloca i64, align 8
  %102 = alloca %"class.std::__1::allocator"*, align 8
  %103 = alloca %"class.std::__1::__tree_node"*, align 8
  %104 = alloca i64, align 8
  %105 = alloca %"class.std::__1::allocator"*, align 8
  %106 = alloca %"struct.std::__1::pair"*, align 8
  %107 = alloca %"class.std::__1::allocator"*, align 8
  %108 = alloca %"struct.std::__1::pair"*, align 8
  %109 = alloca %"struct.std::__1::integral_constant", align 1
  %110 = alloca %"struct.std::__1::__has_destroy", align 1
  %111 = alloca %"struct.std::__1::pair"*, align 8
  %112 = alloca %"union.std::__1::__value_type"*, align 8
  %113 = alloca %"class.std::__1::__tree_node_destructor"*, align 8
  %114 = alloca %"class.std::__1::__tree_node"*, align 8
  %115 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.43"*, align 8
  %116 = alloca %"class.std::__1::__compressed_pair.42"*, align 8
  %117 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %118 = alloca %"class.std::__1::__tree_node"*, align 8
  %119 = alloca %"class.std::__1::__tree_node"*, align 8
  %120 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %121 = alloca %"class.std::__1::unique_ptr.41"*, align 8
  %122 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %123 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %124 = alloca %"class.std::__1::__tree"*, align 8
  %125 = alloca %"class.std::__1::__tree"*, align 8
  %126 = alloca %"struct.std::__1::pair"*, align 8
  %127 = alloca %"class.std::__1::allocator"*, align 8
  %128 = alloca i1, align 1
  %129 = alloca %"class.std::__1::__tree_node_destructor", align 8
  %130 = alloca i8*
  %131 = alloca i32
  store %"class.std::__1::__tree"* %1, %"class.std::__1::__tree"** %125, align 8
  store %"struct.std::__1::pair"* %2, %"struct.std::__1::pair"** %126, align 8
  %132 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %125, align 8
  store %"class.std::__1::__tree"* %132, %"class.std::__1::__tree"** %124, align 8
  %133 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %124, align 8
  %134 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %133, i32 0, i32 1
  store %"class.std::__1::__compressed_pair"* %134, %"class.std::__1::__compressed_pair"** %123, align 8
  %135 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %123, align 8
  %136 = bitcast %"class.std::__1::__compressed_pair"* %135 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %136, %"class.std::__1::__libcpp_compressed_pair_imp"** %122, align 8
  %137 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %122, align 8
  %138 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %137 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %138, %"class.std::__1::allocator"** %127, align 8
  store i1 false, i1* %128, align 1
  %139 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %127, align 8
  store %"class.std::__1::allocator"* %139, %"class.std::__1::allocator"** %92, align 8
  store i64 1, i64* %93, align 8
  %140 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %92, align 8
  %141 = load i64, i64* %93, align 8
  store %"class.std::__1::allocator"* %140, %"class.std::__1::allocator"** %89, align 8
  store i64 %141, i64* %90, align 8
  store i8* null, i8** %91, align 8
  %142 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %89, align 8
  %143 = load i64, i64* %90, align 8
  %144 = mul i64 %143, 64
  store i64 %144, i64* %88, align 8
  %145 = load i64, i64* %88, align 8
  %146 = call i8* @_Znwm(i64 %145) #13
  %147 = bitcast i8* %146 to %"class.std::__1::__tree_node"*
  %148 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %127, align 8
  store %"class.std::__1::__tree_node_destructor"* %129, %"class.std::__1::__tree_node_destructor"** %85, align 8
  store %"class.std::__1::allocator"* %148, %"class.std::__1::allocator"** %86, align 8
  store i8 0, i8* %87, align 1
  %149 = load %"class.std::__1::__tree_node_destructor"*, %"class.std::__1::__tree_node_destructor"** %85, align 8
  %150 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %86, align 8
  %151 = load i8, i8* %87, align 1
  %152 = trunc i8 %151 to i1
  store %"class.std::__1::__tree_node_destructor"* %149, %"class.std::__1::__tree_node_destructor"** %82, align 8
  store %"class.std::__1::allocator"* %150, %"class.std::__1::allocator"** %83, align 8
  %153 = zext i1 %152 to i8
  store i8 %153, i8* %84, align 1
  %154 = load %"class.std::__1::__tree_node_destructor"*, %"class.std::__1::__tree_node_destructor"** %82, align 8
  %155 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %154, i32 0, i32 0
  %156 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %83, align 8
  store %"class.std::__1::allocator"* %156, %"class.std::__1::allocator"** %155, align 8
  %157 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %154, i32 0, i32 1
  %158 = load i8, i8* %84, align 1
  %159 = trunc i8 %158 to i1
  %160 = zext i1 %159 to i8
  store i8 %160, i8* %157, align 8
  store %"class.std::__1::unique_ptr.41"* %0, %"class.std::__1::unique_ptr.41"** %79, align 8
  store %"class.std::__1::__tree_node"* %147, %"class.std::__1::__tree_node"** %80, align 8
  store %"class.std::__1::__tree_node_destructor"* %129, %"class.std::__1::__tree_node_destructor"** %81, align 8
  %161 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %79, align 8
  %162 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %80, align 8
  %163 = load %"class.std::__1::__tree_node_destructor"*, %"class.std::__1::__tree_node_destructor"** %81, align 8
  store %"class.std::__1::unique_ptr.41"* %161, %"class.std::__1::unique_ptr.41"** %75, align 8
  store %"class.std::__1::__tree_node"* %162, %"class.std::__1::__tree_node"** %76, align 8
  store %"class.std::__1::__tree_node_destructor"* %163, %"class.std::__1::__tree_node_destructor"** %77, align 8
  %164 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %75, align 8
  %165 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %164, i32 0, i32 0
  %166 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %76, align 8
  %167 = load %"class.std::__1::__tree_node_destructor"*, %"class.std::__1::__tree_node_destructor"** %77, align 8
  store %"class.std::__1::__tree_node_destructor"* %167, %"class.std::__1::__tree_node_destructor"** %74, align 8
  %168 = load %"class.std::__1::__tree_node_destructor"*, %"class.std::__1::__tree_node_destructor"** %74, align 8
  %169 = bitcast %"class.std::__1::__tree_node_destructor"* %78 to i8*
  %170 = bitcast %"class.std::__1::__tree_node_destructor"* %168 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %169, i8* %170, i64 16, i32 8, i1 false) #2
  %171 = bitcast %"class.std::__1::__tree_node_destructor"* %78 to { %"class.std::__1::allocator"*, i8 }*
  %172 = getelementptr inbounds { %"class.std::__1::allocator"*, i8 }, { %"class.std::__1::allocator"*, i8 }* %171, i32 0, i32 0
  %173 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %172, align 8
  %174 = getelementptr inbounds { %"class.std::__1::allocator"*, i8 }, { %"class.std::__1::allocator"*, i8 }* %171, i32 0, i32 1
  %175 = load i8, i8* %174, align 8
  %176 = bitcast %"class.std::__1::__tree_node_destructor"* %71 to { %"class.std::__1::allocator"*, i8 }*
  %177 = getelementptr inbounds { %"class.std::__1::allocator"*, i8 }, { %"class.std::__1::allocator"*, i8 }* %176, i32 0, i32 0
  store %"class.std::__1::allocator"* %173, %"class.std::__1::allocator"** %177, align 8
  %178 = getelementptr inbounds { %"class.std::__1::allocator"*, i8 }, { %"class.std::__1::allocator"*, i8 }* %176, i32 0, i32 1
  store i8 %175, i8* %178, align 8
  store %"class.std::__1::__compressed_pair.42"* %165, %"class.std::__1::__compressed_pair.42"** %72, align 8
  store %"class.std::__1::__tree_node"* %166, %"class.std::__1::__tree_node"** %73, align 8
  %179 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %72, align 8
  %180 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %73, align 8
  %181 = bitcast %"class.std::__1::__tree_node_destructor"* %71 to { %"class.std::__1::allocator"*, i8 }*
  %182 = getelementptr inbounds { %"class.std::__1::allocator"*, i8 }, { %"class.std::__1::allocator"*, i8 }* %181, i32 0, i32 0
  %183 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %182, align 8
  %184 = getelementptr inbounds { %"class.std::__1::allocator"*, i8 }, { %"class.std::__1::allocator"*, i8 }* %181, i32 0, i32 1
  %185 = load i8, i8* %184, align 8
  %186 = bitcast %"class.std::__1::__tree_node_destructor"* %67 to { %"class.std::__1::allocator"*, i8 }*
  %187 = getelementptr inbounds { %"class.std::__1::allocator"*, i8 }, { %"class.std::__1::allocator"*, i8 }* %186, i32 0, i32 0
  store %"class.std::__1::allocator"* %183, %"class.std::__1::allocator"** %187, align 8
  %188 = getelementptr inbounds { %"class.std::__1::allocator"*, i8 }, { %"class.std::__1::allocator"*, i8 }* %186, i32 0, i32 1
  store i8 %185, i8* %188, align 8
  store %"class.std::__1::__compressed_pair.42"* %179, %"class.std::__1::__compressed_pair.42"** %68, align 8
  store %"class.std::__1::__tree_node"* %180, %"class.std::__1::__tree_node"** %69, align 8
  %189 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %68, align 8
  %190 = bitcast %"class.std::__1::__compressed_pair.42"* %189 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__tree_node"** %69, %"class.std::__1::__tree_node"*** %66, align 8
  %191 = load %"class.std::__1::__tree_node"**, %"class.std::__1::__tree_node"*** %66, align 8
  %192 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %191, align 8
  store %"class.std::__1::__tree_node_destructor"* %67, %"class.std::__1::__tree_node_destructor"** %60, align 8
  %193 = load %"class.std::__1::__tree_node_destructor"*, %"class.std::__1::__tree_node_destructor"** %60, align 8
  %194 = bitcast %"class.std::__1::__tree_node_destructor"* %70 to i8*
  %195 = bitcast %"class.std::__1::__tree_node_destructor"* %193 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %194, i8* %195, i64 16, i32 8, i1 false) #2
  %196 = bitcast %"class.std::__1::__tree_node_destructor"* %70 to { %"class.std::__1::allocator"*, i8 }*
  %197 = getelementptr inbounds { %"class.std::__1::allocator"*, i8 }, { %"class.std::__1::allocator"*, i8 }* %196, i32 0, i32 0
  %198 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %197, align 8
  %199 = getelementptr inbounds { %"class.std::__1::allocator"*, i8 }, { %"class.std::__1::allocator"*, i8 }* %196, i32 0, i32 1
  %200 = load i8, i8* %199, align 8
  %201 = bitcast %"class.std::__1::__tree_node_destructor"* %63 to { %"class.std::__1::allocator"*, i8 }*
  %202 = getelementptr inbounds { %"class.std::__1::allocator"*, i8 }, { %"class.std::__1::allocator"*, i8 }* %201, i32 0, i32 0
  store %"class.std::__1::allocator"* %198, %"class.std::__1::allocator"** %202, align 8
  %203 = getelementptr inbounds { %"class.std::__1::allocator"*, i8 }, { %"class.std::__1::allocator"*, i8 }* %201, i32 0, i32 1
  store i8 %200, i8* %203, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %190, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %64, align 8
  store %"class.std::__1::__tree_node"* %192, %"class.std::__1::__tree_node"** %65, align 8
  %204 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %64, align 8
  %205 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %204, i32 0, i32 0
  store %"class.std::__1::__tree_node"** %65, %"class.std::__1::__tree_node"*** %62, align 8
  %206 = load %"class.std::__1::__tree_node"**, %"class.std::__1::__tree_node"*** %62, align 8
  %207 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %206, align 8
  store %"class.std::__1::__tree_node"* %207, %"class.std::__1::__tree_node"** %205, align 8
  %208 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %204, i32 0, i32 1
  store %"class.std::__1::__tree_node_destructor"* %63, %"class.std::__1::__tree_node_destructor"** %61, align 8
  %209 = load %"class.std::__1::__tree_node_destructor"*, %"class.std::__1::__tree_node_destructor"** %61, align 8
  %210 = bitcast %"class.std::__1::__tree_node_destructor"* %208 to i8*
  %211 = bitcast %"class.std::__1::__tree_node_destructor"* %209 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %210, i8* %211, i64 16, i32 8, i1 false) #2
  %212 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %127, align 8
  store %"class.std::__1::unique_ptr.41"* %0, %"class.std::__1::unique_ptr.41"** %31, align 8
  %213 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %31, align 8
  %214 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %213, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %214, %"class.std::__1::__compressed_pair.42"** %30, align 8
  %215 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %30, align 8
  %216 = bitcast %"class.std::__1::__compressed_pair.42"* %215 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %216, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %29, align 8
  %217 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %29, align 8
  %218 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %217, i32 0, i32 0
  %219 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %218, align 8
  %220 = getelementptr inbounds %"class.std::__1::__tree_node", %"class.std::__1::__tree_node"* %219, i32 0, i32 1
  store %"union.std::__1::__value_type"* %220, %"union.std::__1::__value_type"** %28, align 8
  %221 = load %"union.std::__1::__value_type"*, %"union.std::__1::__value_type"** %28, align 8
  %222 = bitcast %"union.std::__1::__value_type"* %221 to %"struct.std::__1::pair"*
  store %"struct.std::__1::pair"* %222, %"struct.std::__1::pair"** %27, align 8
  %223 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %27, align 8
  %224 = bitcast %"struct.std::__1::pair"* %223 to i8*
  br label %225

; <label>:225:                                    ; preds = %3
  %226 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %126, align 8
  store %"struct.std::__1::pair"* %226, %"struct.std::__1::pair"** %4, align 8
  %227 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %4, align 8
  store %"class.std::__1::allocator"* %212, %"class.std::__1::allocator"** %19, align 8
  store %"struct.std::__1::pair"* %223, %"struct.std::__1::pair"** %20, align 8
  store %"struct.std::__1::pair"* %227, %"struct.std::__1::pair"** %21, align 8
  %228 = bitcast %"struct.std::__1::__has_construct"* %23 to %"struct.std::__1::integral_constant.35"*
  %229 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %19, align 8
  %230 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %20, align 8
  %231 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %21, align 8
  store %"struct.std::__1::pair"* %231, %"struct.std::__1::pair"** %18, align 8
  %232 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %18, align 8
  store %"class.std::__1::allocator"* %229, %"class.std::__1::allocator"** %15, align 8
  store %"struct.std::__1::pair"* %230, %"struct.std::__1::pair"** %16, align 8
  store %"struct.std::__1::pair"* %232, %"struct.std::__1::pair"** %17, align 8
  %233 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %15, align 8
  %234 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %16, align 8
  %235 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %17, align 8
  store %"struct.std::__1::pair"* %235, %"struct.std::__1::pair"** %13, align 8
  %236 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %13, align 8
  store %"class.std::__1::allocator"* %233, %"class.std::__1::allocator"** %10, align 8
  store %"struct.std::__1::pair"* %234, %"struct.std::__1::pair"** %11, align 8
  store %"struct.std::__1::pair"* %236, %"struct.std::__1::pair"** %12, align 8
  %237 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %10, align 8
  %238 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %11, align 8
  %239 = bitcast %"struct.std::__1::pair"* %238 to i8*
  %240 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %12, align 8
  store %"struct.std::__1::pair"* %240, %"struct.std::__1::pair"** %9, align 8
  %241 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %9, align 8
  store %"struct.std::__1::pair"* %238, %"struct.std::__1::pair"** %7, align 8
  store %"struct.std::__1::pair"* %241, %"struct.std::__1::pair"** %8, align 8
  %242 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %7, align 8
  %243 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %8, align 8
  store %"struct.std::__1::pair"* %242, %"struct.std::__1::pair"** %5, align 8
  store %"struct.std::__1::pair"* %243, %"struct.std::__1::pair"** %6, align 8
  %244 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %5, align 8
  %245 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %244, i32 0, i32 0
  %246 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %6, align 8
  %247 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %246, i32 0, i32 0
  %248 = load i32, i32* %247, align 8
  store i32 %248, i32* %245, align 8
  %249 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %244, i32 0, i32 1
  %250 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %6, align 8
  %251 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %250, i32 0, i32 1
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %249, %"class.std::__1::basic_string"* dereferenceable(24) %251)
          to label %252 unwind label %262

; <label>:252:                                    ; preds = %225
  br label %253

; <label>:253:                                    ; preds = %252
  store %"class.std::__1::unique_ptr.41"* %0, %"class.std::__1::unique_ptr.41"** %26, align 8
  %254 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %26, align 8
  %255 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %254, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %255, %"class.std::__1::__compressed_pair.42"** %25, align 8
  %256 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %25, align 8
  %257 = bitcast %"class.std::__1::__compressed_pair.42"* %256 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %257, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %24, align 8
  %258 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %24, align 8
  %259 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %258, i32 0, i32 1
  %260 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %259, i32 0, i32 1
  store i8 1, i8* %260, align 8
  store i1 true, i1* %128, align 1
  %261 = load i1, i1* %128, align 1
  br i1 %261, label %381, label %323

; <label>:262:                                    ; preds = %225
  %263 = landingpad { i8*, i32 }
          cleanup
  %264 = extractvalue { i8*, i32 } %263, 0
  store i8* %264, i8** %130, align 8
  %265 = extractvalue { i8*, i32 } %263, 1
  store i32 %265, i32* %131, align 4
  store %"class.std::__1::unique_ptr.41"* %0, %"class.std::__1::unique_ptr.41"** %59, align 8
  %266 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %59, align 8
  store %"class.std::__1::unique_ptr.41"* %266, %"class.std::__1::unique_ptr.41"** %58, align 8
  %267 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %58, align 8
  store %"class.std::__1::unique_ptr.41"* %267, %"class.std::__1::unique_ptr.41"** %55, align 8
  store %"class.std::__1::__tree_node"* null, %"class.std::__1::__tree_node"** %56, align 8
  %268 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %55, align 8
  %269 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %268, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %269, %"class.std::__1::__compressed_pair.42"** %54, align 8
  %270 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %54, align 8
  %271 = bitcast %"class.std::__1::__compressed_pair.42"* %270 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %271, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %53, align 8
  %272 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %53, align 8
  %273 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %272, i32 0, i32 0
  %274 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %273, align 8
  store %"class.std::__1::__tree_node"* %274, %"class.std::__1::__tree_node"** %57, align 8
  %275 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %56, align 8
  %276 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %268, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %276, %"class.std::__1::__compressed_pair.42"** %35, align 8
  %277 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %35, align 8
  %278 = bitcast %"class.std::__1::__compressed_pair.42"* %277 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %278, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %34, align 8
  %279 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %34, align 8
  %280 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %279, i32 0, i32 0
  store %"class.std::__1::__tree_node"* %275, %"class.std::__1::__tree_node"** %280, align 8
  %281 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %57, align 8
  %282 = icmp ne %"class.std::__1::__tree_node"* %281, null
  br i1 %282, label %283, label %322

; <label>:283:                                    ; preds = %262
  %284 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %268, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %284, %"class.std::__1::__compressed_pair.42"** %33, align 8
  %285 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %33, align 8
  %286 = bitcast %"class.std::__1::__compressed_pair.42"* %285 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %286, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %32, align 8
  %287 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %32, align 8
  %288 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %287, i32 0, i32 1
  %289 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %57, align 8
  store %"class.std::__1::__tree_node_destructor"* %288, %"class.std::__1::__tree_node_destructor"** %51, align 8
  store %"class.std::__1::__tree_node"* %289, %"class.std::__1::__tree_node"** %52, align 8
  %290 = load %"class.std::__1::__tree_node_destructor"*, %"class.std::__1::__tree_node_destructor"** %51, align 8
  %291 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %290, i32 0, i32 1
  %292 = load i8, i8* %291, align 8
  %293 = trunc i8 %292 to i1
  br i1 %293, label %294, label %307

; <label>:294:                                    ; preds = %283
  %295 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %290, i32 0, i32 0
  %296 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %295, align 8
  %297 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %52, align 8
  %298 = getelementptr inbounds %"class.std::__1::__tree_node", %"class.std::__1::__tree_node"* %297, i32 0, i32 1
  store %"union.std::__1::__value_type"* %298, %"union.std::__1::__value_type"** %50, align 8
  %299 = load %"union.std::__1::__value_type"*, %"union.std::__1::__value_type"** %50, align 8
  %300 = bitcast %"union.std::__1::__value_type"* %299 to %"struct.std::__1::pair"*
  store %"struct.std::__1::pair"* %300, %"struct.std::__1::pair"** %49, align 8
  %301 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %49, align 8
  %302 = bitcast %"struct.std::__1::pair"* %301 to i8*
  store %"class.std::__1::allocator"* %296, %"class.std::__1::allocator"** %45, align 8
  store %"struct.std::__1::pair"* %301, %"struct.std::__1::pair"** %46, align 8
  %303 = bitcast %"struct.std::__1::__has_destroy"* %48 to %"struct.std::__1::integral_constant"*
  %304 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %45, align 8
  %305 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %46, align 8
  store %"class.std::__1::allocator"* %304, %"class.std::__1::allocator"** %43, align 8
  store %"struct.std::__1::pair"* %305, %"struct.std::__1::pair"** %44, align 8
  %306 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %44, align 8
  call void @_ZNSt3__14pairIK11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEED1Ev(%"struct.std::__1::pair"* %306) #2
  br label %307

; <label>:307:                                    ; preds = %294, %283
  %308 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %52, align 8
  %309 = icmp ne %"class.std::__1::__tree_node"* %308, null
  br i1 %309, label %310, label %321

; <label>:310:                                    ; preds = %307
  %311 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %290, i32 0, i32 0
  %312 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %311, align 8
  %313 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %52, align 8
  store %"class.std::__1::allocator"* %312, %"class.std::__1::allocator"** %40, align 8
  store %"class.std::__1::__tree_node"* %313, %"class.std::__1::__tree_node"** %41, align 8
  store i64 1, i64* %42, align 8
  %314 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %40, align 8
  %315 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %41, align 8
  %316 = load i64, i64* %42, align 8
  store %"class.std::__1::allocator"* %314, %"class.std::__1::allocator"** %37, align 8
  store %"class.std::__1::__tree_node"* %315, %"class.std::__1::__tree_node"** %38, align 8
  store i64 %316, i64* %39, align 8
  %317 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %37, align 8
  %318 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %38, align 8
  %319 = bitcast %"class.std::__1::__tree_node"* %318 to i8*
  store i8* %319, i8** %36, align 8
  %320 = load i8*, i8** %36, align 8
  call void @_ZdlPv(i8* %320) #12
  br label %321

; <label>:321:                                    ; preds = %310, %307
  br label %322

; <label>:322:                                    ; preds = %262, %321
  br label %382

; <label>:323:                                    ; preds = %253
  store %"class.std::__1::unique_ptr.41"* %0, %"class.std::__1::unique_ptr.41"** %121, align 8
  %324 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %121, align 8
  store %"class.std::__1::unique_ptr.41"* %324, %"class.std::__1::unique_ptr.41"** %120, align 8
  %325 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %120, align 8
  store %"class.std::__1::unique_ptr.41"* %325, %"class.std::__1::unique_ptr.41"** %117, align 8
  store %"class.std::__1::__tree_node"* null, %"class.std::__1::__tree_node"** %118, align 8
  %326 = load %"class.std::__1::unique_ptr.41"*, %"class.std::__1::unique_ptr.41"** %117, align 8
  %327 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %326, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %327, %"class.std::__1::__compressed_pair.42"** %116, align 8
  %328 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %116, align 8
  %329 = bitcast %"class.std::__1::__compressed_pair.42"* %328 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %329, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %115, align 8
  %330 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %115, align 8
  %331 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %330, i32 0, i32 0
  %332 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %331, align 8
  store %"class.std::__1::__tree_node"* %332, %"class.std::__1::__tree_node"** %119, align 8
  %333 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %118, align 8
  %334 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %326, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %334, %"class.std::__1::__compressed_pair.42"** %97, align 8
  %335 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %97, align 8
  %336 = bitcast %"class.std::__1::__compressed_pair.42"* %335 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %336, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %96, align 8
  %337 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %96, align 8
  %338 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %337, i32 0, i32 0
  store %"class.std::__1::__tree_node"* %333, %"class.std::__1::__tree_node"** %338, align 8
  %339 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %119, align 8
  %340 = icmp ne %"class.std::__1::__tree_node"* %339, null
  br i1 %340, label %341, label %380

; <label>:341:                                    ; preds = %323
  %342 = getelementptr inbounds %"class.std::__1::unique_ptr.41", %"class.std::__1::unique_ptr.41"* %326, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.42"* %342, %"class.std::__1::__compressed_pair.42"** %95, align 8
  %343 = load %"class.std::__1::__compressed_pair.42"*, %"class.std::__1::__compressed_pair.42"** %95, align 8
  %344 = bitcast %"class.std::__1::__compressed_pair.42"* %343 to %"class.std::__1::__libcpp_compressed_pair_imp.43"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.43"* %344, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %94, align 8
  %345 = load %"class.std::__1::__libcpp_compressed_pair_imp.43"*, %"class.std::__1::__libcpp_compressed_pair_imp.43"** %94, align 8
  %346 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.43", %"class.std::__1::__libcpp_compressed_pair_imp.43"* %345, i32 0, i32 1
  %347 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %119, align 8
  store %"class.std::__1::__tree_node_destructor"* %346, %"class.std::__1::__tree_node_destructor"** %113, align 8
  store %"class.std::__1::__tree_node"* %347, %"class.std::__1::__tree_node"** %114, align 8
  %348 = load %"class.std::__1::__tree_node_destructor"*, %"class.std::__1::__tree_node_destructor"** %113, align 8
  %349 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %348, i32 0, i32 1
  %350 = load i8, i8* %349, align 8
  %351 = trunc i8 %350 to i1
  br i1 %351, label %352, label %365

; <label>:352:                                    ; preds = %341
  %353 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %348, i32 0, i32 0
  %354 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %353, align 8
  %355 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %114, align 8
  %356 = getelementptr inbounds %"class.std::__1::__tree_node", %"class.std::__1::__tree_node"* %355, i32 0, i32 1
  store %"union.std::__1::__value_type"* %356, %"union.std::__1::__value_type"** %112, align 8
  %357 = load %"union.std::__1::__value_type"*, %"union.std::__1::__value_type"** %112, align 8
  %358 = bitcast %"union.std::__1::__value_type"* %357 to %"struct.std::__1::pair"*
  store %"struct.std::__1::pair"* %358, %"struct.std::__1::pair"** %111, align 8
  %359 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %111, align 8
  %360 = bitcast %"struct.std::__1::pair"* %359 to i8*
  store %"class.std::__1::allocator"* %354, %"class.std::__1::allocator"** %107, align 8
  store %"struct.std::__1::pair"* %359, %"struct.std::__1::pair"** %108, align 8
  %361 = bitcast %"struct.std::__1::__has_destroy"* %110 to %"struct.std::__1::integral_constant"*
  %362 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %107, align 8
  %363 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %108, align 8
  store %"class.std::__1::allocator"* %362, %"class.std::__1::allocator"** %105, align 8
  store %"struct.std::__1::pair"* %363, %"struct.std::__1::pair"** %106, align 8
  %364 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %106, align 8
  call void @_ZNSt3__14pairIK11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEED1Ev(%"struct.std::__1::pair"* %364) #2
  br label %365

; <label>:365:                                    ; preds = %352, %341
  %366 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %114, align 8
  %367 = icmp ne %"class.std::__1::__tree_node"* %366, null
  br i1 %367, label %368, label %379

; <label>:368:                                    ; preds = %365
  %369 = getelementptr inbounds %"class.std::__1::__tree_node_destructor", %"class.std::__1::__tree_node_destructor"* %348, i32 0, i32 0
  %370 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %369, align 8
  %371 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %114, align 8
  store %"class.std::__1::allocator"* %370, %"class.std::__1::allocator"** %102, align 8
  store %"class.std::__1::__tree_node"* %371, %"class.std::__1::__tree_node"** %103, align 8
  store i64 1, i64* %104, align 8
  %372 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %102, align 8
  %373 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %103, align 8
  %374 = load i64, i64* %104, align 8
  store %"class.std::__1::allocator"* %372, %"class.std::__1::allocator"** %99, align 8
  store %"class.std::__1::__tree_node"* %373, %"class.std::__1::__tree_node"** %100, align 8
  store i64 %374, i64* %101, align 8
  %375 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %99, align 8
  %376 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %100, align 8
  %377 = bitcast %"class.std::__1::__tree_node"* %376 to i8*
  store i8* %377, i8** %98, align 8
  %378 = load i8*, i8** %98, align 8
  call void @_ZdlPv(i8* %378) #12
  br label %379

; <label>:379:                                    ; preds = %368, %365
  br label %380

; <label>:380:                                    ; preds = %323, %379
  br label %381

; <label>:381:                                    ; preds = %380, %253
  ret void

; <label>:382:                                    ; preds = %322
  %383 = load i8*, i8** %130, align 8
  %384 = load i32, i32* %131, align 4
  %385 = insertvalue { i8*, i32 } undef, i8* %383, 0
  %386 = insertvalue { i8*, i32 } %385, i32 %384, 1
  resume { i8*, i32 } %386
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE16__insert_node_atEPNS_15__tree_end_nodeIPNS_16__tree_node_baseIPvEEEERSK_SK_(%"class.std::__1::__tree"*, %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_node_base"** dereferenceable(8), %"class.std::__1::__tree_node_base"*) #4 align 2 {
  %5 = alloca %"class.std::__1::__tree"*, align 8
  %6 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %7 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %8 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %9 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %10 = alloca %"class.std::__1::__tree"*, align 8
  %11 = alloca %"class.std::__1::__tree"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %14 = alloca %"class.std::__1::__tree"*, align 8
  %15 = alloca %"class.std::__1::__tree"*, align 8
  %16 = alloca %"class.std::__1::__tree"*, align 8
  %17 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %18 = alloca %"class.std::__1::__tree_node_base"**, align 8
  %19 = alloca %"class.std::__1::__tree_node_base"*, align 8
  store %"class.std::__1::__tree"* %0, %"class.std::__1::__tree"** %16, align 8
  store %"class.std::__1::__tree_end_node"* %1, %"class.std::__1::__tree_end_node"** %17, align 8
  store %"class.std::__1::__tree_node_base"** %2, %"class.std::__1::__tree_node_base"*** %18, align 8
  store %"class.std::__1::__tree_node_base"* %3, %"class.std::__1::__tree_node_base"** %19, align 8
  %20 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %16, align 8
  %21 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %19, align 8
  %22 = bitcast %"class.std::__1::__tree_node_base"* %21 to %"class.std::__1::__tree_end_node"*
  %23 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %22, i32 0, i32 0
  store %"class.std::__1::__tree_node_base"* null, %"class.std::__1::__tree_node_base"** %23, align 8
  %24 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %19, align 8
  %25 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %24, i32 0, i32 1
  store %"class.std::__1::__tree_node_base"* null, %"class.std::__1::__tree_node_base"** %25, align 8
  %26 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %17, align 8
  %27 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %19, align 8
  %28 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %27, i32 0, i32 2
  store %"class.std::__1::__tree_end_node"* %26, %"class.std::__1::__tree_end_node"** %28, align 8
  %29 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %19, align 8
  %30 = load %"class.std::__1::__tree_node_base"**, %"class.std::__1::__tree_node_base"*** %18, align 8
  store %"class.std::__1::__tree_node_base"* %29, %"class.std::__1::__tree_node_base"** %30, align 8
  store %"class.std::__1::__tree"* %20, %"class.std::__1::__tree"** %15, align 8
  %31 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %15, align 8
  %32 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %31, i32 0, i32 0
  %33 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %32, align 8
  %34 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %33, i32 0, i32 0
  %35 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %34, align 8
  %36 = icmp ne %"class.std::__1::__tree_node_base"* %35, null
  br i1 %36, label %37, label %46

; <label>:37:                                     ; preds = %4
  store %"class.std::__1::__tree"* %20, %"class.std::__1::__tree"** %11, align 8
  %38 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %11, align 8
  %39 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %38, i32 0, i32 0
  %40 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %39, align 8
  %41 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %40, i32 0, i32 0
  %42 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %41, align 8
  %43 = bitcast %"class.std::__1::__tree_node_base"* %42 to %"class.std::__1::__tree_end_node"*
  store %"class.std::__1::__tree"* %20, %"class.std::__1::__tree"** %5, align 8
  %44 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %5, align 8
  %45 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %44, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %43, %"class.std::__1::__tree_end_node"** %45, align 8
  br label %46

; <label>:46:                                     ; preds = %37, %4
  store %"class.std::__1::__tree"* %20, %"class.std::__1::__tree"** %10, align 8
  %47 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %10, align 8
  %48 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %47, i32 0, i32 1
  store %"class.std::__1::__compressed_pair"* %48, %"class.std::__1::__compressed_pair"** %9, align 8
  %49 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8
  %50 = bitcast %"class.std::__1::__compressed_pair"* %49 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %50, %"class.std::__1::__libcpp_compressed_pair_imp"** %8, align 8
  %51 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %8, align 8
  %52 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %51, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %52, %"class.std::__1::__tree_end_node"** %7, align 8
  %53 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %7, align 8
  store %"class.std::__1::__tree_end_node"* %53, %"class.std::__1::__tree_end_node"** %6, align 8
  %54 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %6, align 8
  %55 = bitcast %"class.std::__1::__tree_end_node"* %54 to i8*
  %56 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %54, i32 0, i32 0
  %57 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %56, align 8
  %58 = load %"class.std::__1::__tree_node_base"**, %"class.std::__1::__tree_node_base"*** %18, align 8
  %59 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %58, align 8
  call void @_ZNSt3__127__tree_balance_after_insertIPNS_16__tree_node_baseIPvEEEEvT_S5_(%"class.std::__1::__tree_node_base"* %57, %"class.std::__1::__tree_node_base"* %59) #2
  store %"class.std::__1::__tree"* %20, %"class.std::__1::__tree"** %14, align 8
  %60 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %14, align 8
  %61 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %60, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.0"* %61, %"class.std::__1::__compressed_pair.0"** %13, align 8
  %62 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %13, align 8
  %63 = bitcast %"class.std::__1::__compressed_pair.0"* %62 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %63, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %12, align 8
  %64 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %12, align 8
  %65 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.1", %"class.std::__1::__libcpp_compressed_pair_imp.1"* %64, i32 0, i32 0
  %66 = load i64, i64* %65, align 8
  %67 = add i64 %66, 1
  store i64 %67, i64* %65, align 8
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr dereferenceable(8) %"class.std::__1::__tree_node_base"** @_ZNSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE12__find_equalIS2_EERPNS_16__tree_node_baseIPvEERPNS_15__tree_end_nodeISK_EERKT_(%"class.std::__1::__tree"*, %"class.std::__1::__tree_end_node"** dereferenceable(8), i32* dereferenceable(4)) #0 align 2 {
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %6 = alloca %"class.std::__1::__tree"*, align 8
  %7 = alloca %"struct.std::__1::less"*, align 8
  %8 = alloca i32*, align 8
  %9 = alloca i32*, align 8
  %10 = alloca %"class.std::__1::__map_value_compare"*, align 8
  %11 = alloca i32*, align 8
  %12 = alloca %"union.std::__1::__value_type"*, align 8
  %13 = alloca %"class.std::__1::__tree_node_base"**, align 8
  %14 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.1"*, align 8
  %15 = alloca %"class.std::__1::__compressed_pair.0"*, align 8
  %16 = alloca %"class.std::__1::__tree"*, align 8
  %17 = alloca %"struct.std::__1::less"*, align 8
  %18 = alloca i32*, align 8
  %19 = alloca i32*, align 8
  %20 = alloca %"class.std::__1::__map_value_compare"*, align 8
  %21 = alloca %"union.std::__1::__value_type"*, align 8
  %22 = alloca i32*, align 8
  %23 = alloca %"class.std::__1::__tree_node_base"**, align 8
  %24 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %25 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %28 = alloca %"class.std::__1::__tree"*, align 8
  %29 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %30 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %31 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %32 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %33 = alloca %"class.std::__1::__tree"*, align 8
  %34 = alloca %"class.std::__1::__tree"*, align 8
  %35 = alloca %"class.std::__1::__tree_node_base"**, align 8
  %36 = alloca %"class.std::__1::__tree"*, align 8
  %37 = alloca %"class.std::__1::__tree_end_node"**, align 8
  %38 = alloca i32*, align 8
  %39 = alloca %"class.std::__1::__tree_node"*, align 8
  %40 = alloca %"class.std::__1::__tree_node_base"**, align 8
  store %"class.std::__1::__tree"* %0, %"class.std::__1::__tree"** %36, align 8
  store %"class.std::__1::__tree_end_node"** %1, %"class.std::__1::__tree_end_node"*** %37, align 8
  store i32* %2, i32** %38, align 8
  %41 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %36, align 8
  store %"class.std::__1::__tree"* %41, %"class.std::__1::__tree"** %34, align 8
  %42 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %34, align 8
  store %"class.std::__1::__tree"* %42, %"class.std::__1::__tree"** %33, align 8
  %43 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %33, align 8
  %44 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %43, i32 0, i32 1
  store %"class.std::__1::__compressed_pair"* %44, %"class.std::__1::__compressed_pair"** %32, align 8
  %45 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8
  %46 = bitcast %"class.std::__1::__compressed_pair"* %45 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %46, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8
  %47 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8
  %48 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %47, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %48, %"class.std::__1::__tree_end_node"** %30, align 8
  %49 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %30, align 8
  store %"class.std::__1::__tree_end_node"* %49, %"class.std::__1::__tree_end_node"** %29, align 8
  %50 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %29, align 8
  %51 = bitcast %"class.std::__1::__tree_end_node"* %50 to i8*
  %52 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %50, i32 0, i32 0
  %53 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %52, align 8
  %54 = bitcast %"class.std::__1::__tree_node_base"* %53 to %"class.std::__1::__tree_node"*
  store %"class.std::__1::__tree_node"* %54, %"class.std::__1::__tree_node"** %39, align 8
  %55 = call %"class.std::__1::__tree_node_base"** @_ZNKSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE10__root_ptrEv(%"class.std::__1::__tree"* %41) #2
  store %"class.std::__1::__tree_node_base"** %55, %"class.std::__1::__tree_node_base"*** %40, align 8
  %56 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %39, align 8
  %57 = icmp ne %"class.std::__1::__tree_node"* %56, null
  br i1 %57, label %58, label %160

; <label>:58:                                     ; preds = %3
  br label %59

; <label>:59:                                     ; preds = %58, %159
  store %"class.std::__1::__tree"* %41, %"class.std::__1::__tree"** %6, align 8
  %60 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %6, align 8
  %61 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %60, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.0"* %61, %"class.std::__1::__compressed_pair.0"** %5, align 8
  %62 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %5, align 8
  %63 = bitcast %"class.std::__1::__compressed_pair.0"* %62 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %63, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %4, align 8
  %64 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %4, align 8
  %65 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.1"* %64 to %"class.std::__1::__map_value_compare"*
  %66 = load i32*, i32** %38, align 8
  %67 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %39, align 8
  %68 = getelementptr inbounds %"class.std::__1::__tree_node", %"class.std::__1::__tree_node"* %67, i32 0, i32 1
  store %"class.std::__1::__map_value_compare"* %65, %"class.std::__1::__map_value_compare"** %10, align 8
  store i32* %66, i32** %11, align 8
  store %"union.std::__1::__value_type"* %68, %"union.std::__1::__value_type"** %12, align 8
  %69 = load %"class.std::__1::__map_value_compare"*, %"class.std::__1::__map_value_compare"** %10, align 8
  %70 = bitcast %"class.std::__1::__map_value_compare"* %69 to %"struct.std::__1::less"*
  %71 = load i32*, i32** %11, align 8
  %72 = load %"union.std::__1::__value_type"*, %"union.std::__1::__value_type"** %12, align 8
  %73 = bitcast %"union.std::__1::__value_type"* %72 to %"struct.std::__1::pair"*
  %74 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %73, i32 0, i32 0
  store %"struct.std::__1::less"* %70, %"struct.std::__1::less"** %7, align 8
  store i32* %71, i32** %8, align 8
  store i32* %74, i32** %9, align 8
  %75 = load %"struct.std::__1::less"*, %"struct.std::__1::less"** %7, align 8
  %76 = load i32*, i32** %8, align 8
  %77 = load i32, i32* %76, align 4
  %78 = load i32*, i32** %9, align 8
  %79 = load i32, i32* %78, align 4
  %80 = icmp slt i32 %77, %79
  br i1 %80, label %81, label %106

; <label>:81:                                     ; preds = %59
  %82 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %39, align 8
  %83 = bitcast %"class.std::__1::__tree_node"* %82 to %"class.std::__1::__tree_end_node"*
  %84 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %83, i32 0, i32 0
  %85 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %84, align 8
  %86 = icmp ne %"class.std::__1::__tree_node_base"* %85, null
  br i1 %86, label %87, label %98

; <label>:87:                                     ; preds = %81
  %88 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %39, align 8
  %89 = bitcast %"class.std::__1::__tree_node"* %88 to %"class.std::__1::__tree_end_node"*
  %90 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %89, i32 0, i32 0
  store %"class.std::__1::__tree_node_base"** %90, %"class.std::__1::__tree_node_base"*** %13, align 8
  %91 = load %"class.std::__1::__tree_node_base"**, %"class.std::__1::__tree_node_base"*** %13, align 8
  %92 = bitcast %"class.std::__1::__tree_node_base"** %91 to i8*
  store %"class.std::__1::__tree_node_base"** %91, %"class.std::__1::__tree_node_base"*** %40, align 8
  %93 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %39, align 8
  %94 = bitcast %"class.std::__1::__tree_node"* %93 to %"class.std::__1::__tree_end_node"*
  %95 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %94, i32 0, i32 0
  %96 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %95, align 8
  %97 = bitcast %"class.std::__1::__tree_node_base"* %96 to %"class.std::__1::__tree_node"*
  store %"class.std::__1::__tree_node"* %97, %"class.std::__1::__tree_node"** %39, align 8
  br label %105

; <label>:98:                                     ; preds = %81
  %99 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %39, align 8
  %100 = bitcast %"class.std::__1::__tree_node"* %99 to %"class.std::__1::__tree_end_node"*
  %101 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %37, align 8
  store %"class.std::__1::__tree_end_node"* %100, %"class.std::__1::__tree_end_node"** %101, align 8
  %102 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %37, align 8
  %103 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %102, align 8
  %104 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %103, i32 0, i32 0
  store %"class.std::__1::__tree_node_base"** %104, %"class.std::__1::__tree_node_base"*** %35, align 8
  br label %174

; <label>:105:                                    ; preds = %87
  br label %159

; <label>:106:                                    ; preds = %59
  store %"class.std::__1::__tree"* %41, %"class.std::__1::__tree"** %16, align 8
  %107 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %16, align 8
  %108 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %107, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.0"* %108, %"class.std::__1::__compressed_pair.0"** %15, align 8
  %109 = load %"class.std::__1::__compressed_pair.0"*, %"class.std::__1::__compressed_pair.0"** %15, align 8
  %110 = bitcast %"class.std::__1::__compressed_pair.0"* %109 to %"class.std::__1::__libcpp_compressed_pair_imp.1"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.1"* %110, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %14, align 8
  %111 = load %"class.std::__1::__libcpp_compressed_pair_imp.1"*, %"class.std::__1::__libcpp_compressed_pair_imp.1"** %14, align 8
  %112 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.1"* %111 to %"class.std::__1::__map_value_compare"*
  %113 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %39, align 8
  %114 = getelementptr inbounds %"class.std::__1::__tree_node", %"class.std::__1::__tree_node"* %113, i32 0, i32 1
  %115 = load i32*, i32** %38, align 8
  store %"class.std::__1::__map_value_compare"* %112, %"class.std::__1::__map_value_compare"** %20, align 8
  store %"union.std::__1::__value_type"* %114, %"union.std::__1::__value_type"** %21, align 8
  store i32* %115, i32** %22, align 8
  %116 = load %"class.std::__1::__map_value_compare"*, %"class.std::__1::__map_value_compare"** %20, align 8
  %117 = bitcast %"class.std::__1::__map_value_compare"* %116 to %"struct.std::__1::less"*
  %118 = load %"union.std::__1::__value_type"*, %"union.std::__1::__value_type"** %21, align 8
  %119 = bitcast %"union.std::__1::__value_type"* %118 to %"struct.std::__1::pair"*
  %120 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %119, i32 0, i32 0
  %121 = load i32*, i32** %22, align 8
  store %"struct.std::__1::less"* %117, %"struct.std::__1::less"** %17, align 8
  store i32* %120, i32** %18, align 8
  store i32* %121, i32** %19, align 8
  %122 = load %"struct.std::__1::less"*, %"struct.std::__1::less"** %17, align 8
  %123 = load i32*, i32** %18, align 8
  %124 = load i32, i32* %123, align 4
  %125 = load i32*, i32** %19, align 8
  %126 = load i32, i32* %125, align 4
  %127 = icmp slt i32 %124, %126
  br i1 %127, label %128, label %153

; <label>:128:                                    ; preds = %106
  %129 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %39, align 8
  %130 = bitcast %"class.std::__1::__tree_node"* %129 to %"class.std::__1::__tree_node_base"*
  %131 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %130, i32 0, i32 1
  %132 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %131, align 8
  %133 = icmp ne %"class.std::__1::__tree_node_base"* %132, null
  br i1 %133, label %134, label %145

; <label>:134:                                    ; preds = %128
  %135 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %39, align 8
  %136 = bitcast %"class.std::__1::__tree_node"* %135 to %"class.std::__1::__tree_node_base"*
  %137 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %136, i32 0, i32 1
  store %"class.std::__1::__tree_node_base"** %137, %"class.std::__1::__tree_node_base"*** %23, align 8
  %138 = load %"class.std::__1::__tree_node_base"**, %"class.std::__1::__tree_node_base"*** %23, align 8
  %139 = bitcast %"class.std::__1::__tree_node_base"** %138 to i8*
  store %"class.std::__1::__tree_node_base"** %138, %"class.std::__1::__tree_node_base"*** %40, align 8
  %140 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %39, align 8
  %141 = bitcast %"class.std::__1::__tree_node"* %140 to %"class.std::__1::__tree_node_base"*
  %142 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %141, i32 0, i32 1
  %143 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %142, align 8
  %144 = bitcast %"class.std::__1::__tree_node_base"* %143 to %"class.std::__1::__tree_node"*
  store %"class.std::__1::__tree_node"* %144, %"class.std::__1::__tree_node"** %39, align 8
  br label %152

; <label>:145:                                    ; preds = %128
  %146 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %39, align 8
  %147 = bitcast %"class.std::__1::__tree_node"* %146 to %"class.std::__1::__tree_end_node"*
  %148 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %37, align 8
  store %"class.std::__1::__tree_end_node"* %147, %"class.std::__1::__tree_end_node"** %148, align 8
  %149 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %39, align 8
  %150 = bitcast %"class.std::__1::__tree_node"* %149 to %"class.std::__1::__tree_node_base"*
  %151 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %150, i32 0, i32 1
  store %"class.std::__1::__tree_node_base"** %151, %"class.std::__1::__tree_node_base"*** %35, align 8
  br label %174

; <label>:152:                                    ; preds = %134
  br label %158

; <label>:153:                                    ; preds = %106
  %154 = load %"class.std::__1::__tree_node"*, %"class.std::__1::__tree_node"** %39, align 8
  %155 = bitcast %"class.std::__1::__tree_node"* %154 to %"class.std::__1::__tree_end_node"*
  %156 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %37, align 8
  store %"class.std::__1::__tree_end_node"* %155, %"class.std::__1::__tree_end_node"** %156, align 8
  %157 = load %"class.std::__1::__tree_node_base"**, %"class.std::__1::__tree_node_base"*** %40, align 8
  store %"class.std::__1::__tree_node_base"** %157, %"class.std::__1::__tree_node_base"*** %35, align 8
  br label %174

; <label>:158:                                    ; preds = %152
  br label %159

; <label>:159:                                    ; preds = %158, %105
  br label %59

; <label>:160:                                    ; preds = %3
  store %"class.std::__1::__tree"* %41, %"class.std::__1::__tree"** %28, align 8
  %161 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %28, align 8
  %162 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %161, i32 0, i32 1
  store %"class.std::__1::__compressed_pair"* %162, %"class.std::__1::__compressed_pair"** %27, align 8
  %163 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8
  %164 = bitcast %"class.std::__1::__compressed_pair"* %163 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %164, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8
  %165 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8
  %166 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %165, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %166, %"class.std::__1::__tree_end_node"** %25, align 8
  %167 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %25, align 8
  store %"class.std::__1::__tree_end_node"* %167, %"class.std::__1::__tree_end_node"** %24, align 8
  %168 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %24, align 8
  %169 = bitcast %"class.std::__1::__tree_end_node"* %168 to i8*
  %170 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %37, align 8
  store %"class.std::__1::__tree_end_node"* %168, %"class.std::__1::__tree_end_node"** %170, align 8
  %171 = load %"class.std::__1::__tree_end_node"**, %"class.std::__1::__tree_end_node"*** %37, align 8
  %172 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %171, align 8
  %173 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %172, i32 0, i32 0
  store %"class.std::__1::__tree_node_base"** %173, %"class.std::__1::__tree_node_base"*** %35, align 8
  br label %174

; <label>:174:                                    ; preds = %160, %153, %145, %98
  %175 = load %"class.std::__1::__tree_node_base"**, %"class.std::__1::__tree_node_base"*** %35, align 8
  ret %"class.std::__1::__tree_node_base"** %175
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr %"class.std::__1::__tree_node_base"** @_ZNKSt3__16__treeINS_12__value_typeI11LiteralTypeNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEEENS_19__map_value_compareIS2_S9_NS_4lessIS2_EELb1EEENS6_IS9_EEE10__root_ptrEv(%"class.std::__1::__tree"*) #4 align 2 {
  %2 = alloca %"class.std::__1::__tree_node_base"**, align 8
  %3 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %4 = alloca %"class.std::__1::__tree_end_node"*, align 8
  %5 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %6 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %7 = alloca %"class.std::__1::__tree"*, align 8
  %8 = alloca %"class.std::__1::__tree"*, align 8
  store %"class.std::__1::__tree"* %0, %"class.std::__1::__tree"** %8, align 8
  %9 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %8, align 8
  store %"class.std::__1::__tree"* %9, %"class.std::__1::__tree"** %7, align 8
  %10 = load %"class.std::__1::__tree"*, %"class.std::__1::__tree"** %7, align 8
  %11 = getelementptr inbounds %"class.std::__1::__tree", %"class.std::__1::__tree"* %10, i32 0, i32 1
  store %"class.std::__1::__compressed_pair"* %11, %"class.std::__1::__compressed_pair"** %6, align 8
  %12 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %6, align 8
  %13 = bitcast %"class.std::__1::__compressed_pair"* %12 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %13, %"class.std::__1::__libcpp_compressed_pair_imp"** %5, align 8
  %14 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %5, align 8
  %15 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %14, i32 0, i32 0
  store %"class.std::__1::__tree_end_node"* %15, %"class.std::__1::__tree_end_node"** %4, align 8
  %16 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %4, align 8
  store %"class.std::__1::__tree_end_node"* %16, %"class.std::__1::__tree_end_node"** %3, align 8
  %17 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %3, align 8
  %18 = bitcast %"class.std::__1::__tree_end_node"* %17 to i8*
  %19 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %17, i32 0, i32 0
  store %"class.std::__1::__tree_node_base"** %19, %"class.std::__1::__tree_node_base"*** %2, align 8
  %20 = load %"class.std::__1::__tree_node_base"**, %"class.std::__1::__tree_node_base"*** %2, align 8
  %21 = bitcast %"class.std::__1::__tree_node_base"** %20 to i8*
  ret %"class.std::__1::__tree_node_base"** %20
}

; Function Attrs: nobuiltin
declare noalias i8* @_Znwm(i64) #10

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"*, %"class.std::__1::basic_string"* dereferenceable(24)) unnamed_addr #3

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__127__tree_balance_after_insertIPNS_16__tree_node_baseIPvEEEEvT_S5_(%"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"*) #4 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %4 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %5 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %6 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %7 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %8 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %9 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %10 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %11 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %12 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %13 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %14 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %15 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %16 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %17 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %18 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %19 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %20 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %21 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %22 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %23 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %24 = alloca %"class.std::__1::__tree_node_base"*, align 8
  store %"class.std::__1::__tree_node_base"* %0, %"class.std::__1::__tree_node_base"** %21, align 8
  store %"class.std::__1::__tree_node_base"* %1, %"class.std::__1::__tree_node_base"** %22, align 8
  %25 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  %26 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %21, align 8
  %27 = icmp eq %"class.std::__1::__tree_node_base"* %25, %26
  %28 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  %29 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %28, i32 0, i32 3
  %30 = zext i1 %27 to i8
  store i8 %30, i8* %29, align 8
  br label %31

; <label>:31:                                     ; preds = %219, %2
  %32 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  %33 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %21, align 8
  %34 = icmp ne %"class.std::__1::__tree_node_base"* %32, %33
  br i1 %34, label %35, label %46

; <label>:35:                                     ; preds = %31
  %36 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %36, %"class.std::__1::__tree_node_base"** %20, align 8
  %37 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %20, align 8
  %38 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %37, i32 0, i32 2
  %39 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %38, align 8
  %40 = bitcast %"class.std::__1::__tree_end_node"* %39 to %"class.std::__1::__tree_node_base"*
  br label %41

; <label>:41:                                     ; preds = %35
  %42 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %40, i32 0, i32 3
  %43 = load i8, i8* %42, align 8
  %44 = trunc i8 %43 to i1
  %45 = xor i1 %44, true
  br label %46

; <label>:46:                                     ; preds = %41, %31
  %47 = phi i1 [ false, %31 ], [ %45, %41 ]
  br i1 %47, label %48, label %220

; <label>:48:                                     ; preds = %46
  %49 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %49, %"class.std::__1::__tree_node_base"** %19, align 8
  %50 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %19, align 8
  %51 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %50, i32 0, i32 2
  %52 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %51, align 8
  %53 = bitcast %"class.std::__1::__tree_end_node"* %52 to %"class.std::__1::__tree_node_base"*
  br label %54

; <label>:54:                                     ; preds = %48
  store %"class.std::__1::__tree_node_base"* %53, %"class.std::__1::__tree_node_base"** %18, align 8
  %55 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %18, align 8
  %56 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %18, align 8
  %57 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %56, i32 0, i32 2
  %58 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %57, align 8
  %59 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %58, i32 0, i32 0
  %60 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %59, align 8
  %61 = icmp eq %"class.std::__1::__tree_node_base"* %55, %60
  br i1 %61, label %62, label %142

; <label>:62:                                     ; preds = %54
  %63 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %63, %"class.std::__1::__tree_node_base"** %15, align 8
  %64 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %15, align 8
  %65 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %64, i32 0, i32 2
  %66 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %65, align 8
  %67 = bitcast %"class.std::__1::__tree_end_node"* %66 to %"class.std::__1::__tree_node_base"*
  br label %68

; <label>:68:                                     ; preds = %62
  store %"class.std::__1::__tree_node_base"* %67, %"class.std::__1::__tree_node_base"** %9, align 8
  %69 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %9, align 8
  %70 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %69, i32 0, i32 2
  %71 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %70, align 8
  %72 = bitcast %"class.std::__1::__tree_end_node"* %71 to %"class.std::__1::__tree_node_base"*
  br label %73

; <label>:73:                                     ; preds = %68
  %74 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %72, i32 0, i32 1
  %75 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %74, align 8
  store %"class.std::__1::__tree_node_base"* %75, %"class.std::__1::__tree_node_base"** %23, align 8
  %76 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %23, align 8
  %77 = icmp ne %"class.std::__1::__tree_node_base"* %76, null
  br i1 %77, label %78, label %106

; <label>:78:                                     ; preds = %73
  %79 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %23, align 8
  %80 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %79, i32 0, i32 3
  %81 = load i8, i8* %80, align 8
  %82 = trunc i8 %81 to i1
  br i1 %82, label %106, label %83

; <label>:83:                                     ; preds = %78
  %84 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %84, %"class.std::__1::__tree_node_base"** %6, align 8
  %85 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %6, align 8
  %86 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %85, i32 0, i32 2
  %87 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %86, align 8
  %88 = bitcast %"class.std::__1::__tree_end_node"* %87 to %"class.std::__1::__tree_node_base"*
  br label %89

; <label>:89:                                     ; preds = %83
  store %"class.std::__1::__tree_node_base"* %88, %"class.std::__1::__tree_node_base"** %22, align 8
  %90 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  %91 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %90, i32 0, i32 3
  store i8 1, i8* %91, align 8
  %92 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %92, %"class.std::__1::__tree_node_base"** %3, align 8
  %93 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %3, align 8
  %94 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %93, i32 0, i32 2
  %95 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %94, align 8
  %96 = bitcast %"class.std::__1::__tree_end_node"* %95 to %"class.std::__1::__tree_node_base"*
  br label %97

; <label>:97:                                     ; preds = %89
  store %"class.std::__1::__tree_node_base"* %96, %"class.std::__1::__tree_node_base"** %22, align 8
  %98 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  %99 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %21, align 8
  %100 = icmp eq %"class.std::__1::__tree_node_base"* %98, %99
  %101 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  %102 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %101, i32 0, i32 3
  %103 = zext i1 %100 to i8
  store i8 %103, i8* %102, align 8
  %104 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %23, align 8
  %105 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %104, i32 0, i32 3
  store i8 1, i8* %105, align 8
  br label %141

; <label>:106:                                    ; preds = %78, %73
  %107 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %107, %"class.std::__1::__tree_node_base"** %4, align 8
  %108 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %4, align 8
  %109 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %4, align 8
  %110 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %109, i32 0, i32 2
  %111 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %110, align 8
  %112 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %111, i32 0, i32 0
  %113 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %112, align 8
  %114 = icmp eq %"class.std::__1::__tree_node_base"* %108, %113
  br i1 %114, label %123, label %115

; <label>:115:                                    ; preds = %106
  %116 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %116, %"class.std::__1::__tree_node_base"** %5, align 8
  %117 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %5, align 8
  %118 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %117, i32 0, i32 2
  %119 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %118, align 8
  %120 = bitcast %"class.std::__1::__tree_end_node"* %119 to %"class.std::__1::__tree_node_base"*
  br label %121

; <label>:121:                                    ; preds = %115
  store %"class.std::__1::__tree_node_base"* %120, %"class.std::__1::__tree_node_base"** %22, align 8
  %122 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  call void @_ZNSt3__118__tree_left_rotateIPNS_16__tree_node_baseIPvEEEEvT_(%"class.std::__1::__tree_node_base"* %122) #2
  br label %123

; <label>:123:                                    ; preds = %121, %106
  %124 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %124, %"class.std::__1::__tree_node_base"** %7, align 8
  %125 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %7, align 8
  %126 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %125, i32 0, i32 2
  %127 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %126, align 8
  %128 = bitcast %"class.std::__1::__tree_end_node"* %127 to %"class.std::__1::__tree_node_base"*
  br label %129

; <label>:129:                                    ; preds = %123
  store %"class.std::__1::__tree_node_base"* %128, %"class.std::__1::__tree_node_base"** %22, align 8
  %130 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  %131 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %130, i32 0, i32 3
  store i8 1, i8* %131, align 8
  %132 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %132, %"class.std::__1::__tree_node_base"** %8, align 8
  %133 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %134 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %133, i32 0, i32 2
  %135 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %134, align 8
  %136 = bitcast %"class.std::__1::__tree_end_node"* %135 to %"class.std::__1::__tree_node_base"*
  br label %137

; <label>:137:                                    ; preds = %129
  store %"class.std::__1::__tree_node_base"* %136, %"class.std::__1::__tree_node_base"** %22, align 8
  %138 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  %139 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %138, i32 0, i32 3
  store i8 0, i8* %139, align 8
  %140 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  call void @_ZNSt3__119__tree_right_rotateIPNS_16__tree_node_baseIPvEEEEvT_(%"class.std::__1::__tree_node_base"* %140) #2
  br label %220

; <label>:141:                                    ; preds = %97
  br label %219

; <label>:142:                                    ; preds = %54
  %143 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %143, %"class.std::__1::__tree_node_base"** %10, align 8
  %144 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %10, align 8
  %145 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %144, i32 0, i32 2
  %146 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %145, align 8
  %147 = bitcast %"class.std::__1::__tree_end_node"* %146 to %"class.std::__1::__tree_node_base"*
  br label %148

; <label>:148:                                    ; preds = %142
  %149 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %147, i32 0, i32 2
  %150 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %149, align 8
  %151 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %150, i32 0, i32 0
  %152 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %151, align 8
  store %"class.std::__1::__tree_node_base"* %152, %"class.std::__1::__tree_node_base"** %24, align 8
  %153 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %24, align 8
  %154 = icmp ne %"class.std::__1::__tree_node_base"* %153, null
  br i1 %154, label %155, label %183

; <label>:155:                                    ; preds = %148
  %156 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %24, align 8
  %157 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %156, i32 0, i32 3
  %158 = load i8, i8* %157, align 8
  %159 = trunc i8 %158 to i1
  br i1 %159, label %183, label %160

; <label>:160:                                    ; preds = %155
  %161 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %161, %"class.std::__1::__tree_node_base"** %11, align 8
  %162 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %11, align 8
  %163 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %162, i32 0, i32 2
  %164 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %163, align 8
  %165 = bitcast %"class.std::__1::__tree_end_node"* %164 to %"class.std::__1::__tree_node_base"*
  br label %166

; <label>:166:                                    ; preds = %160
  store %"class.std::__1::__tree_node_base"* %165, %"class.std::__1::__tree_node_base"** %22, align 8
  %167 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  %168 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %167, i32 0, i32 3
  store i8 1, i8* %168, align 8
  %169 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %169, %"class.std::__1::__tree_node_base"** %12, align 8
  %170 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %12, align 8
  %171 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %170, i32 0, i32 2
  %172 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %171, align 8
  %173 = bitcast %"class.std::__1::__tree_end_node"* %172 to %"class.std::__1::__tree_node_base"*
  br label %174

; <label>:174:                                    ; preds = %166
  store %"class.std::__1::__tree_node_base"* %173, %"class.std::__1::__tree_node_base"** %22, align 8
  %175 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  %176 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %21, align 8
  %177 = icmp eq %"class.std::__1::__tree_node_base"* %175, %176
  %178 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  %179 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %178, i32 0, i32 3
  %180 = zext i1 %177 to i8
  store i8 %180, i8* %179, align 8
  %181 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %24, align 8
  %182 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %181, i32 0, i32 3
  store i8 1, i8* %182, align 8
  br label %218

; <label>:183:                                    ; preds = %155, %148
  %184 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %184, %"class.std::__1::__tree_node_base"** %13, align 8
  %185 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %13, align 8
  %186 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %13, align 8
  %187 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %186, i32 0, i32 2
  %188 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %187, align 8
  %189 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %188, i32 0, i32 0
  %190 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %189, align 8
  %191 = icmp eq %"class.std::__1::__tree_node_base"* %185, %190
  br i1 %191, label %192, label %200

; <label>:192:                                    ; preds = %183
  %193 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %193, %"class.std::__1::__tree_node_base"** %14, align 8
  %194 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %14, align 8
  %195 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %194, i32 0, i32 2
  %196 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %195, align 8
  %197 = bitcast %"class.std::__1::__tree_end_node"* %196 to %"class.std::__1::__tree_node_base"*
  br label %198

; <label>:198:                                    ; preds = %192
  store %"class.std::__1::__tree_node_base"* %197, %"class.std::__1::__tree_node_base"** %22, align 8
  %199 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  call void @_ZNSt3__119__tree_right_rotateIPNS_16__tree_node_baseIPvEEEEvT_(%"class.std::__1::__tree_node_base"* %199) #2
  br label %200

; <label>:200:                                    ; preds = %198, %183
  %201 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %201, %"class.std::__1::__tree_node_base"** %16, align 8
  %202 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %16, align 8
  %203 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %202, i32 0, i32 2
  %204 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %203, align 8
  %205 = bitcast %"class.std::__1::__tree_end_node"* %204 to %"class.std::__1::__tree_node_base"*
  br label %206

; <label>:206:                                    ; preds = %200
  store %"class.std::__1::__tree_node_base"* %205, %"class.std::__1::__tree_node_base"** %22, align 8
  %207 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  %208 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %207, i32 0, i32 3
  store i8 1, i8* %208, align 8
  %209 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  store %"class.std::__1::__tree_node_base"* %209, %"class.std::__1::__tree_node_base"** %17, align 8
  %210 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %17, align 8
  %211 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %210, i32 0, i32 2
  %212 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %211, align 8
  %213 = bitcast %"class.std::__1::__tree_end_node"* %212 to %"class.std::__1::__tree_node_base"*
  br label %214

; <label>:214:                                    ; preds = %206
  store %"class.std::__1::__tree_node_base"* %213, %"class.std::__1::__tree_node_base"** %22, align 8
  %215 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  %216 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %215, i32 0, i32 3
  store i8 0, i8* %216, align 8
  %217 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  call void @_ZNSt3__118__tree_left_rotateIPNS_16__tree_node_baseIPvEEEEvT_(%"class.std::__1::__tree_node_base"* %217) #2
  br label %220

; <label>:218:                                    ; preds = %174
  br label %219

; <label>:219:                                    ; preds = %218, %141
  br label %31

; <label>:220:                                    ; preds = %214, %137, %46
  ret void
                                                  ; No predecessors!
  %222 = landingpad { i8*, i32 }
          catch i8* null
  %223 = extractvalue { i8*, i32 } %222, 0
  call void @__clang_call_terminate(i8* %223) #11
  unreachable
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__118__tree_left_rotateIPNS_16__tree_node_baseIPvEEEEvT_(%"class.std::__1::__tree_node_base"*) #4 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %3 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %4 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %5 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %6 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %7 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %8 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %9 = alloca %"class.std::__1::__tree_node_base"*, align 8
  store %"class.std::__1::__tree_node_base"* %0, %"class.std::__1::__tree_node_base"** %8, align 8
  %10 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %11 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %10, i32 0, i32 1
  %12 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %11, align 8
  store %"class.std::__1::__tree_node_base"* %12, %"class.std::__1::__tree_node_base"** %9, align 8
  %13 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %9, align 8
  %14 = bitcast %"class.std::__1::__tree_node_base"* %13 to %"class.std::__1::__tree_end_node"*
  %15 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %14, i32 0, i32 0
  %16 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %15, align 8
  %17 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %18 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %17, i32 0, i32 1
  store %"class.std::__1::__tree_node_base"* %16, %"class.std::__1::__tree_node_base"** %18, align 8
  %19 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %20 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %19, i32 0, i32 1
  %21 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %20, align 8
  %22 = icmp ne %"class.std::__1::__tree_node_base"* %21, null
  br i1 %22, label %23, label %33

; <label>:23:                                     ; preds = %1
  %24 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %25 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %24, i32 0, i32 1
  %26 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %25, align 8
  %27 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  store %"class.std::__1::__tree_node_base"* %26, %"class.std::__1::__tree_node_base"** %6, align 8
  store %"class.std::__1::__tree_node_base"* %27, %"class.std::__1::__tree_node_base"** %7, align 8
  %28 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %6, align 8
  %29 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %7, align 8
  %30 = bitcast %"class.std::__1::__tree_node_base"* %29 to %"class.std::__1::__tree_end_node"*
  %31 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %28, i32 0, i32 2
  store %"class.std::__1::__tree_end_node"* %30, %"class.std::__1::__tree_end_node"** %31, align 8
  br label %32

; <label>:32:                                     ; preds = %23
  br label %33

; <label>:33:                                     ; preds = %32, %1
  %34 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %35 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %34, i32 0, i32 2
  %36 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %35, align 8
  %37 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %9, align 8
  %38 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %37, i32 0, i32 2
  store %"class.std::__1::__tree_end_node"* %36, %"class.std::__1::__tree_end_node"** %38, align 8
  %39 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  store %"class.std::__1::__tree_node_base"* %39, %"class.std::__1::__tree_node_base"** %5, align 8
  %40 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %5, align 8
  %41 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %5, align 8
  %42 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %41, i32 0, i32 2
  %43 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %42, align 8
  %44 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %43, i32 0, i32 0
  %45 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %44, align 8
  %46 = icmp eq %"class.std::__1::__tree_node_base"* %40, %45
  br i1 %46, label %47, label %53

; <label>:47:                                     ; preds = %33
  %48 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %9, align 8
  %49 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %50 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %49, i32 0, i32 2
  %51 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %50, align 8
  %52 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %51, i32 0, i32 0
  store %"class.std::__1::__tree_node_base"* %48, %"class.std::__1::__tree_node_base"** %52, align 8
  br label %62

; <label>:53:                                     ; preds = %33
  %54 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %9, align 8
  %55 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  store %"class.std::__1::__tree_node_base"* %55, %"class.std::__1::__tree_node_base"** %2, align 8
  %56 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %2, align 8
  %57 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %56, i32 0, i32 2
  %58 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %57, align 8
  %59 = bitcast %"class.std::__1::__tree_end_node"* %58 to %"class.std::__1::__tree_node_base"*
  br label %60

; <label>:60:                                     ; preds = %53
  %61 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %59, i32 0, i32 1
  store %"class.std::__1::__tree_node_base"* %54, %"class.std::__1::__tree_node_base"** %61, align 8
  br label %62

; <label>:62:                                     ; preds = %60, %47
  %63 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %64 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %9, align 8
  %65 = bitcast %"class.std::__1::__tree_node_base"* %64 to %"class.std::__1::__tree_end_node"*
  %66 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %65, i32 0, i32 0
  store %"class.std::__1::__tree_node_base"* %63, %"class.std::__1::__tree_node_base"** %66, align 8
  %67 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %68 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %9, align 8
  store %"class.std::__1::__tree_node_base"* %67, %"class.std::__1::__tree_node_base"** %3, align 8
  store %"class.std::__1::__tree_node_base"* %68, %"class.std::__1::__tree_node_base"** %4, align 8
  %69 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %3, align 8
  %70 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %4, align 8
  %71 = bitcast %"class.std::__1::__tree_node_base"* %70 to %"class.std::__1::__tree_end_node"*
  %72 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %69, i32 0, i32 2
  store %"class.std::__1::__tree_end_node"* %71, %"class.std::__1::__tree_end_node"** %72, align 8
  br label %73

; <label>:73:                                     ; preds = %62
  ret void
                                                  ; No predecessors!
  %75 = landingpad { i8*, i32 }
          catch i8* null
  %76 = extractvalue { i8*, i32 } %75, 0
  call void @__clang_call_terminate(i8* %76) #11
  unreachable
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__119__tree_right_rotateIPNS_16__tree_node_baseIPvEEEEvT_(%"class.std::__1::__tree_node_base"*) #4 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %3 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %4 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %5 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %6 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %7 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %8 = alloca %"class.std::__1::__tree_node_base"*, align 8
  %9 = alloca %"class.std::__1::__tree_node_base"*, align 8
  store %"class.std::__1::__tree_node_base"* %0, %"class.std::__1::__tree_node_base"** %8, align 8
  %10 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %11 = bitcast %"class.std::__1::__tree_node_base"* %10 to %"class.std::__1::__tree_end_node"*
  %12 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %11, i32 0, i32 0
  %13 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %12, align 8
  store %"class.std::__1::__tree_node_base"* %13, %"class.std::__1::__tree_node_base"** %9, align 8
  %14 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %9, align 8
  %15 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %14, i32 0, i32 1
  %16 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %15, align 8
  %17 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %18 = bitcast %"class.std::__1::__tree_node_base"* %17 to %"class.std::__1::__tree_end_node"*
  %19 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %18, i32 0, i32 0
  store %"class.std::__1::__tree_node_base"* %16, %"class.std::__1::__tree_node_base"** %19, align 8
  %20 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %21 = bitcast %"class.std::__1::__tree_node_base"* %20 to %"class.std::__1::__tree_end_node"*
  %22 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %21, i32 0, i32 0
  %23 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %22, align 8
  %24 = icmp ne %"class.std::__1::__tree_node_base"* %23, null
  br i1 %24, label %25, label %36

; <label>:25:                                     ; preds = %1
  %26 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %27 = bitcast %"class.std::__1::__tree_node_base"* %26 to %"class.std::__1::__tree_end_node"*
  %28 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %27, i32 0, i32 0
  %29 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %28, align 8
  %30 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  store %"class.std::__1::__tree_node_base"* %29, %"class.std::__1::__tree_node_base"** %6, align 8
  store %"class.std::__1::__tree_node_base"* %30, %"class.std::__1::__tree_node_base"** %7, align 8
  %31 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %6, align 8
  %32 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %7, align 8
  %33 = bitcast %"class.std::__1::__tree_node_base"* %32 to %"class.std::__1::__tree_end_node"*
  %34 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %31, i32 0, i32 2
  store %"class.std::__1::__tree_end_node"* %33, %"class.std::__1::__tree_end_node"** %34, align 8
  br label %35

; <label>:35:                                     ; preds = %25
  br label %36

; <label>:36:                                     ; preds = %35, %1
  %37 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %38 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %37, i32 0, i32 2
  %39 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %38, align 8
  %40 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %9, align 8
  %41 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %40, i32 0, i32 2
  store %"class.std::__1::__tree_end_node"* %39, %"class.std::__1::__tree_end_node"** %41, align 8
  %42 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  store %"class.std::__1::__tree_node_base"* %42, %"class.std::__1::__tree_node_base"** %5, align 8
  %43 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %5, align 8
  %44 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %5, align 8
  %45 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %44, i32 0, i32 2
  %46 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %45, align 8
  %47 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %46, i32 0, i32 0
  %48 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %47, align 8
  %49 = icmp eq %"class.std::__1::__tree_node_base"* %43, %48
  br i1 %49, label %50, label %56

; <label>:50:                                     ; preds = %36
  %51 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %9, align 8
  %52 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %53 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %52, i32 0, i32 2
  %54 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %53, align 8
  %55 = getelementptr inbounds %"class.std::__1::__tree_end_node", %"class.std::__1::__tree_end_node"* %54, i32 0, i32 0
  store %"class.std::__1::__tree_node_base"* %51, %"class.std::__1::__tree_node_base"** %55, align 8
  br label %65

; <label>:56:                                     ; preds = %36
  %57 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %9, align 8
  %58 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  store %"class.std::__1::__tree_node_base"* %58, %"class.std::__1::__tree_node_base"** %2, align 8
  %59 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %2, align 8
  %60 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %59, i32 0, i32 2
  %61 = load %"class.std::__1::__tree_end_node"*, %"class.std::__1::__tree_end_node"** %60, align 8
  %62 = bitcast %"class.std::__1::__tree_end_node"* %61 to %"class.std::__1::__tree_node_base"*
  br label %63

; <label>:63:                                     ; preds = %56
  %64 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %62, i32 0, i32 1
  store %"class.std::__1::__tree_node_base"* %57, %"class.std::__1::__tree_node_base"** %64, align 8
  br label %65

; <label>:65:                                     ; preds = %63, %50
  %66 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %67 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %9, align 8
  %68 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %67, i32 0, i32 1
  store %"class.std::__1::__tree_node_base"* %66, %"class.std::__1::__tree_node_base"** %68, align 8
  %69 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %8, align 8
  %70 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %9, align 8
  store %"class.std::__1::__tree_node_base"* %69, %"class.std::__1::__tree_node_base"** %3, align 8
  store %"class.std::__1::__tree_node_base"* %70, %"class.std::__1::__tree_node_base"** %4, align 8
  %71 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %3, align 8
  %72 = load %"class.std::__1::__tree_node_base"*, %"class.std::__1::__tree_node_base"** %4, align 8
  %73 = bitcast %"class.std::__1::__tree_node_base"* %72 to %"class.std::__1::__tree_end_node"*
  %74 = getelementptr inbounds %"class.std::__1::__tree_node_base", %"class.std::__1::__tree_node_base"* %71, i32 0, i32 2
  store %"class.std::__1::__tree_end_node"* %73, %"class.std::__1::__tree_end_node"** %74, align 8
  br label %75

; <label>:75:                                     ; preds = %65
  ret void
                                                  ; No predecessors!
  %77 = landingpad { i8*, i32 }
          catch i8* null
  %78 = extractvalue { i8*, i32 } %77, 0
  call void @__clang_call_terminate(i8* %78) #11
  unreachable
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8*, i8*, i64) #6 align 2 {
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

; Function Attrs: ssp uwtable
define internal void @_GLOBAL__sub_I_Token.cpp() #0 section "__TEXT,__StaticInit,regular,pure_instructions" {
  call void @__cxx_global_var_init()
  ret void
}

attributes #0 = { ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { inlinehint nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { inlinehint ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { argmemonly nounwind }
attributes #8 = { noinline noreturn nounwind }
attributes #9 = { nobuiltin nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #10 = { nobuiltin "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #11 = { noreturn nounwind }
attributes #12 = { builtin nounwind }
attributes #13 = { builtin }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"Apple LLVM version 8.1.0 (clang-802.0.42)"}
