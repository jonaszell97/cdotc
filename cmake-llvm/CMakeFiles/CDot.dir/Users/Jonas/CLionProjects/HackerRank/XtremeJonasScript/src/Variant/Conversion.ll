; ModuleID = '/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/Variant/Conversion.cpp'
source_filename = "/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/Variant/Conversion.cpp"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.12.0"

%struct.Variant = type <{ %union.anon, i8, [7 x i8], %"class.std::__1::basic_string", %struct.TypeSpecifier, i8, i8, i8, [5 x i8] }>
%union.anon = type { i64 }
%"class.std::__1::basic_string" = type { %"class.std::__1::__compressed_pair" }
%"class.std::__1::__compressed_pair" = type { %"class.std::__1::__libcpp_compressed_pair_imp" }
%"class.std::__1::__libcpp_compressed_pair_imp" = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" = type { %union.anon.0 }
%union.anon.0 = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" = type { i64, i64, i8* }
%struct.TypeSpecifier = type { i32, %"class.std::__1::basic_string", %"class.std::__1::shared_ptr", i8, i8, i8, i8, i64, i32, i8, i8, %"class.std::__1::basic_string", i8, [7 x i8], %"class.std::__1::unordered_map", i8, i8, i8, i8, i8, i8, i8, i32, %"class.std::__1::vector", %"class.std::__1::vector.20", %union.anon.26 }
%"class.std::__1::shared_ptr" = type { %class.Expression*, %"class.std::__1::__shared_weak_count"* }
%class.Expression = type opaque
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
%"class.std::__1::vector" = type { %"class.std::__1::__vector_base" }
%"class.std::__1::__vector_base" = type { %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"*, %"class.std::__1::__compressed_pair.16" }
%"class.std::__1::__compressed_pair.16" = type { %"class.std::__1::__libcpp_compressed_pair_imp.17" }
%"class.std::__1::__libcpp_compressed_pair_imp.17" = type { %"class.std::__1::basic_string"* }
%"class.std::__1::vector.20" = type { %"class.std::__1::__vector_base.21" }
%"class.std::__1::__vector_base.21" = type { %struct.TypeSpecifier*, %struct.TypeSpecifier*, %"class.std::__1::__compressed_pair.22" }
%"class.std::__1::__compressed_pair.22" = type { %"class.std::__1::__libcpp_compressed_pair_imp.23" }
%"class.std::__1::__libcpp_compressed_pair_imp.23" = type { %struct.TypeSpecifier* }
%union.anon.26 = type { %struct.TypeSpecifier* }
%"class.std::__1::allocator" = type { i8 }
%"struct.std::__1::integral_constant" = type { i8 }
%"class.std::invalid_argument" = type { %"class.std::logic_error" }
%"class.std::logic_error" = type { %"class.std::exception", %"class.std::__1::__libcpp_refstring" }
%"class.std::exception" = type { i32 (...)** }
%"class.std::__1::__libcpp_refstring" = type { i8* }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short" = type { %union.anon.1, [23 x i8] }
%union.anon.1 = type { i8 }
%"class.std::__1::__basic_string_common" = type { i8 }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw" = type { [3 x i64] }
%class.AstNode = type opaque

@_ZN4llvm24DisableABIBreakingChecksE = external global i32, align 4
@_ZN4llvm30VerifyDisableABIBreakingChecksE = weak hidden global i32* @_ZN4llvm24DisableABIBreakingChecksE, align 8
@.str = private unnamed_addr constant [21 x i8] c"Cannot cast string '\00", align 1
@.str.1 = private unnamed_addr constant [10 x i8] c"' to char\00", align 1
@_ZTISt16invalid_argument = external constant i8*
@.str.2 = private unnamed_addr constant [12 x i8] c"' to number\00", align 1
@.str.3 = private unnamed_addr constant [6 x i8] c"false\00", align 1
@.str.4 = private unnamed_addr constant [2 x i8] c"0\00", align 1
@.str.5 = private unnamed_addr constant [27 x i8] c"Cannot cast value of type \00", align 1
@.str.6 = private unnamed_addr constant [5 x i8] c" to \00", align 1
@.str.7 = private unnamed_addr constant [7 x i8] c"Value \00", align 1
@.str.8 = private unnamed_addr constant [28 x i8] c" is too large for type char\00", align 1
@.str.9 = private unnamed_addr constant [32 x i8] c"Cannot cast value of type.type \00", align 1
@.str.10 = private unnamed_addr constant [5 x i8] c"true\00", align 1
@_ZTVSt16invalid_argument = external unnamed_addr constant [5 x i8*]

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var9Converter4castE7Variant9ValueType(%struct.Variant* noalias sret, %struct.Variant*, i32) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  %56 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %57 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %58 = alloca %"class.std::__1::basic_string"*, align 8
  %59 = alloca [3 x i64]*, align 8
  %60 = alloca i32, align 4
  %61 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %62 = alloca %"class.std::__1::basic_string"*, align 8
  %63 = alloca %"class.std::__1::basic_string"*, align 8
  %64 = alloca %"class.std::__1::basic_string"*, align 8
  %65 = alloca %"class.std::__1::basic_string"*, align 8
  %66 = alloca %"class.std::__1::basic_string"*, align 8
  %67 = alloca i8*, align 8
  %68 = alloca %"class.std::__1::basic_string"*, align 8
  %69 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %70 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %71 = alloca %"class.std::__1::basic_string"*, align 8
  %72 = alloca [3 x i64]*, align 8
  %73 = alloca i32, align 4
  %74 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %75 = alloca %"class.std::__1::basic_string"*, align 8
  %76 = alloca %"class.std::__1::basic_string"*, align 8
  %77 = alloca %"class.std::__1::basic_string"*, align 8
  %78 = alloca %"class.std::__1::basic_string"*, align 8
  %79 = alloca %"class.std::__1::basic_string"*, align 8
  %80 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %81 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %82 = alloca %"class.std::__1::basic_string"*, align 8
  %83 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %84 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %85 = alloca %"class.std::__1::basic_string"*, align 8
  %86 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %87 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %88 = alloca %"class.std::__1::basic_string"*, align 8
  %89 = alloca %"class.std::__1::basic_string"*, align 8
  %90 = alloca i8*, align 8
  %91 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %92 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %93 = alloca %"class.std::__1::basic_string"*, align 8
  %94 = alloca i8*, align 8
  %95 = alloca i8*, align 8
  %96 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %97 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %98 = alloca %"class.std::__1::basic_string"*, align 8
  %99 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %100 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %101 = alloca %"class.std::__1::basic_string"*, align 8
  %102 = alloca %"class.std::__1::basic_string"*, align 8
  %103 = alloca %"class.std::__1::basic_string"*, align 8
  %104 = alloca %"class.std::__1::basic_string"*, align 8
  %105 = alloca %"class.std::__1::basic_string"*, align 8
  %106 = alloca %"class.std::__1::basic_string"*, align 8
  %107 = alloca %"class.std::__1::basic_string"*, align 8
  %108 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %109 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %110 = alloca %"class.std::__1::basic_string"*, align 8
  %111 = alloca [3 x i64]*, align 8
  %112 = alloca i32, align 4
  %113 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %114 = alloca %"class.std::__1::basic_string"*, align 8
  %115 = alloca %"class.std::__1::basic_string"*, align 8
  %116 = alloca %"class.std::__1::basic_string"*, align 8
  %117 = alloca %"class.std::__1::basic_string"*, align 8
  %118 = alloca %"class.std::__1::basic_string"*, align 8
  %119 = alloca %"class.std::__1::basic_string"*, align 8
  %120 = alloca i8*, align 8
  %121 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %122 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %123 = alloca %"class.std::__1::basic_string"*, align 8
  %124 = alloca [3 x i64]*, align 8
  %125 = alloca i32, align 4
  %126 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %127 = alloca %"class.std::__1::basic_string"*, align 8
  %128 = alloca %"class.std::__1::basic_string"*, align 8
  %129 = alloca %"class.std::__1::basic_string"*, align 8
  %130 = alloca %"class.std::__1::basic_string"*, align 8
  %131 = alloca %"class.std::__1::basic_string"*, align 8
  %132 = alloca i8*, align 8
  %133 = alloca %"class.std::__1::basic_string"*, align 8
  %134 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %135 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %136 = alloca %"class.std::__1::basic_string"*, align 8
  %137 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %138 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %139 = alloca %"class.std::__1::basic_string"*, align 8
  %140 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %141 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %142 = alloca %"class.std::__1::basic_string"*, align 8
  %143 = alloca %"class.std::__1::basic_string"*, align 8
  %144 = alloca %"class.std::__1::basic_string"*, align 8
  %145 = alloca %"class.std::__1::basic_string"*, align 8
  %146 = alloca i8*, align 8
  %147 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %148 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %149 = alloca %"class.std::__1::basic_string"*, align 8
  %150 = alloca [3 x i64]*, align 8
  %151 = alloca i32, align 4
  %152 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %153 = alloca %"class.std::__1::basic_string"*, align 8
  %154 = alloca %"class.std::__1::basic_string"*, align 8
  %155 = alloca %"class.std::__1::basic_string"*, align 8
  %156 = alloca %"class.std::__1::basic_string"*, align 8
  %157 = alloca %"class.std::__1::basic_string"*, align 8
  %158 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %159 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %160 = alloca %"class.std::__1::basic_string"*, align 8
  %161 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %162 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %163 = alloca %"class.std::__1::basic_string"*, align 8
  %164 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %165 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %166 = alloca %"class.std::__1::basic_string"*, align 8
  %167 = alloca %"class.std::__1::basic_string"*, align 8
  %168 = alloca i8*, align 8
  %169 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %170 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %171 = alloca %"class.std::__1::basic_string"*, align 8
  %172 = alloca i8*, align 8
  %173 = alloca i8*, align 8
  %174 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %175 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %176 = alloca %"class.std::__1::basic_string"*, align 8
  %177 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %178 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %179 = alloca %"class.std::__1::basic_string"*, align 8
  %180 = alloca %"class.std::__1::basic_string"*, align 8
  %181 = alloca %"class.std::__1::basic_string"*, align 8
  %182 = alloca %"class.std::__1::basic_string"*, align 8
  %183 = alloca %"class.std::__1::basic_string"*, align 8
  %184 = alloca %"class.std::__1::basic_string"*, align 8
  %185 = alloca %"class.std::__1::basic_string"*, align 8
  %186 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %187 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %188 = alloca %"class.std::__1::basic_string"*, align 8
  %189 = alloca [3 x i64]*, align 8
  %190 = alloca i32, align 4
  %191 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %192 = alloca %"class.std::__1::basic_string"*, align 8
  %193 = alloca %"class.std::__1::basic_string"*, align 8
  %194 = alloca %"class.std::__1::basic_string"*, align 8
  %195 = alloca %"class.std::__1::basic_string"*, align 8
  %196 = alloca %"class.std::__1::basic_string"*, align 8
  %197 = alloca %"class.std::__1::basic_string"*, align 8
  %198 = alloca i8*, align 8
  %199 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %200 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %201 = alloca %"class.std::__1::basic_string"*, align 8
  %202 = alloca [3 x i64]*, align 8
  %203 = alloca i32, align 4
  %204 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %205 = alloca %"class.std::__1::basic_string"*, align 8
  %206 = alloca %"class.std::__1::basic_string"*, align 8
  %207 = alloca %"class.std::__1::basic_string"*, align 8
  %208 = alloca %"class.std::__1::basic_string"*, align 8
  %209 = alloca %"class.std::__1::basic_string"*, align 8
  %210 = alloca i8*, align 8
  %211 = alloca %"class.std::__1::basic_string"*, align 8
  %212 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %213 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %214 = alloca %"class.std::__1::basic_string"*, align 8
  %215 = alloca [3 x i64]*, align 8
  %216 = alloca i32, align 4
  %217 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %218 = alloca %"class.std::__1::basic_string"*, align 8
  %219 = alloca %"class.std::__1::basic_string"*, align 8
  %220 = alloca %"class.std::__1::basic_string"*, align 8
  %221 = alloca %"class.std::__1::basic_string"*, align 8
  %222 = alloca %"class.std::__1::basic_string"*, align 8
  %223 = alloca %"class.std::__1::basic_string"*, align 8
  %224 = alloca i8*, align 8
  %225 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %226 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %227 = alloca %"class.std::__1::basic_string"*, align 8
  %228 = alloca [3 x i64]*, align 8
  %229 = alloca i32, align 4
  %230 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %231 = alloca %"class.std::__1::basic_string"*, align 8
  %232 = alloca %"class.std::__1::basic_string"*, align 8
  %233 = alloca %"class.std::__1::basic_string"*, align 8
  %234 = alloca %"class.std::__1::basic_string"*, align 8
  %235 = alloca %"class.std::__1::basic_string"*, align 8
  %236 = alloca i8*, align 8
  %237 = alloca %"class.std::__1::basic_string"*, align 8
  %238 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %239 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %240 = alloca %"class.std::__1::basic_string"*, align 8
  %241 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %242 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %243 = alloca %"class.std::__1::basic_string"*, align 8
  %244 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %245 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %246 = alloca %"class.std::__1::basic_string"*, align 8
  %247 = alloca %"class.std::__1::basic_string"*, align 8
  %248 = alloca %"class.std::__1::basic_string"*, align 8
  %249 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %250 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %251 = alloca %"class.std::__1::basic_string"*, align 8
  %252 = alloca [3 x i64]*, align 8
  %253 = alloca i32, align 4
  %254 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %255 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %256 = alloca %"class.std::__1::basic_string"*, align 8
  %257 = alloca %"class.std::__1::allocator"*, align 8
  %258 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %259 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %260 = alloca %"class.std::__1::basic_string"*, align 8
  %261 = alloca %"struct.std::__1::integral_constant", align 1
  %262 = alloca %"class.std::__1::basic_string"*, align 8
  %263 = alloca %"class.std::__1::basic_string"*, align 8
  %264 = alloca %"class.std::__1::basic_string"*, align 8
  %265 = alloca %"class.std::__1::basic_string"*, align 8
  %266 = alloca %"struct.std::__1::integral_constant", align 1
  %267 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %268 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %269 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %270 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %271 = alloca %"class.std::__1::basic_string"*, align 8
  %272 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %273 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %274 = alloca %"class.std::__1::basic_string"*, align 8
  %275 = alloca i64, align 8
  %276 = alloca i8*, align 8
  %277 = alloca i8*, align 8
  %278 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %279 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %280 = alloca %"class.std::__1::basic_string"*, align 8
  %281 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %282 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %283 = alloca %"class.std::__1::basic_string"*, align 8
  %284 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %285 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %286 = alloca %"class.std::__1::basic_string"*, align 8
  %287 = alloca i64, align 8
  %288 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %289 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %290 = alloca %"class.std::__1::basic_string"*, align 8
  %291 = alloca %"class.std::__1::basic_string"*, align 8
  %292 = alloca %"class.std::__1::basic_string"*, align 8
  %293 = alloca i8, align 1
  %294 = alloca i8, align 1
  %295 = alloca %"struct.std::__1::integral_constant", align 1
  %296 = alloca %"class.std::__1::basic_string"*, align 8
  %297 = alloca %"class.std::__1::basic_string"*, align 8
  %298 = alloca %"class.std::__1::basic_string"*, align 8
  %299 = alloca %"class.std::__1::basic_string"*, align 8
  %300 = alloca %"struct.std::__1::integral_constant", align 1
  %301 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %302 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %303 = alloca %"class.std::__1::basic_string"*, align 8
  %304 = alloca [3 x i64]*, align 8
  %305 = alloca i32, align 4
  %306 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %307 = alloca %"class.std::__1::basic_string"*, align 8
  %308 = alloca %"class.std::__1::basic_string"*, align 8
  %309 = alloca %"class.std::__1::basic_string"*, align 8
  %310 = alloca %"class.std::__1::basic_string"*, align 8
  %311 = alloca %"class.std::__1::basic_string"*, align 8
  %312 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %313 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %314 = alloca %"class.std::__1::basic_string"*, align 8
  %315 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %316 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %317 = alloca %"class.std::__1::basic_string"*, align 8
  %318 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %319 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %320 = alloca %"class.std::__1::basic_string"*, align 8
  %321 = alloca %"class.std::__1::basic_string"*, align 8
  %322 = alloca i8*, align 8
  %323 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %324 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %325 = alloca %"class.std::__1::basic_string"*, align 8
  %326 = alloca i8*, align 8
  %327 = alloca i8*, align 8
  %328 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %329 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %330 = alloca %"class.std::__1::basic_string"*, align 8
  %331 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %332 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %333 = alloca %"class.std::__1::basic_string"*, align 8
  %334 = alloca %"class.std::__1::basic_string"*, align 8
  %335 = alloca %"class.std::__1::basic_string"*, align 8
  %336 = alloca %"class.std::__1::basic_string"*, align 8
  %337 = alloca %"class.std::__1::basic_string"*, align 8
  %338 = alloca %"class.std::__1::basic_string"*, align 8
  %339 = alloca %"class.std::__1::basic_string"*, align 8
  %340 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %341 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %342 = alloca %"class.std::__1::basic_string"*, align 8
  %343 = alloca [3 x i64]*, align 8
  %344 = alloca i32, align 4
  %345 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %346 = alloca %"class.std::__1::basic_string"*, align 8
  %347 = alloca %"class.std::__1::basic_string"*, align 8
  %348 = alloca %"class.std::__1::basic_string"*, align 8
  %349 = alloca %"class.std::__1::basic_string"*, align 8
  %350 = alloca %"class.std::__1::basic_string"*, align 8
  %351 = alloca %"class.std::__1::basic_string"*, align 8
  %352 = alloca i8*, align 8
  %353 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %354 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %355 = alloca %"class.std::__1::basic_string"*, align 8
  %356 = alloca [3 x i64]*, align 8
  %357 = alloca i32, align 4
  %358 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %359 = alloca %"class.std::__1::basic_string"*, align 8
  %360 = alloca %"class.std::__1::basic_string"*, align 8
  %361 = alloca %"class.std::__1::basic_string"*, align 8
  %362 = alloca %"class.std::__1::basic_string"*, align 8
  %363 = alloca %"class.std::__1::basic_string"*, align 8
  %364 = alloca i8*, align 8
  %365 = alloca %"class.std::__1::basic_string"*, align 8
  %366 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %367 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %368 = alloca %"class.std::__1::basic_string"*, align 8
  %369 = alloca [3 x i64]*, align 8
  %370 = alloca i32, align 4
  %371 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %372 = alloca %"class.std::__1::basic_string"*, align 8
  %373 = alloca %"class.std::__1::basic_string"*, align 8
  %374 = alloca %"class.std::__1::basic_string"*, align 8
  %375 = alloca %"class.std::__1::basic_string"*, align 8
  %376 = alloca %"class.std::__1::basic_string"*, align 8
  %377 = alloca %"class.std::__1::basic_string"*, align 8
  %378 = alloca i8*, align 8
  %379 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %380 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %381 = alloca %"class.std::__1::basic_string"*, align 8
  %382 = alloca [3 x i64]*, align 8
  %383 = alloca i32, align 4
  %384 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %385 = alloca %"class.std::__1::basic_string"*, align 8
  %386 = alloca %"class.std::__1::basic_string"*, align 8
  %387 = alloca %"class.std::__1::basic_string"*, align 8
  %388 = alloca %"class.std::__1::basic_string"*, align 8
  %389 = alloca %"class.std::__1::basic_string"*, align 8
  %390 = alloca i8*, align 8
  %391 = alloca %"class.std::__1::basic_string"*, align 8
  %392 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %393 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %394 = alloca %"class.std::__1::basic_string"*, align 8
  %395 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %396 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %397 = alloca %"class.std::__1::basic_string"*, align 8
  %398 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %399 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %400 = alloca %"class.std::__1::basic_string"*, align 8
  %401 = alloca %"class.std::__1::basic_string"*, align 8
  %402 = alloca %"class.std::__1::basic_string"*, align 8
  %403 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %404 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %405 = alloca %"class.std::__1::basic_string"*, align 8
  %406 = alloca [3 x i64]*, align 8
  %407 = alloca i32, align 4
  %408 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %409 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %410 = alloca %"class.std::__1::basic_string"*, align 8
  %411 = alloca %"class.std::__1::allocator"*, align 8
  %412 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %413 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %414 = alloca %"class.std::__1::basic_string"*, align 8
  %415 = alloca %"struct.std::__1::integral_constant", align 1
  %416 = alloca %"class.std::__1::basic_string"*, align 8
  %417 = alloca %"class.std::__1::basic_string"*, align 8
  %418 = alloca %"class.std::__1::basic_string"*, align 8
  %419 = alloca %"class.std::__1::basic_string"*, align 8
  %420 = alloca %"struct.std::__1::integral_constant", align 1
  %421 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %422 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %423 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %424 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %425 = alloca %"class.std::__1::basic_string"*, align 8
  %426 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %427 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %428 = alloca %"class.std::__1::basic_string"*, align 8
  %429 = alloca i64, align 8
  %430 = alloca i8*, align 8
  %431 = alloca i8*, align 8
  %432 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %433 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %434 = alloca %"class.std::__1::basic_string"*, align 8
  %435 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %436 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %437 = alloca %"class.std::__1::basic_string"*, align 8
  %438 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %439 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %440 = alloca %"class.std::__1::basic_string"*, align 8
  %441 = alloca i64, align 8
  %442 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %443 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %444 = alloca %"class.std::__1::basic_string"*, align 8
  %445 = alloca %"class.std::__1::basic_string"*, align 8
  %446 = alloca %"class.std::__1::basic_string"*, align 8
  %447 = alloca i8, align 1
  %448 = alloca i8, align 1
  %449 = alloca %"struct.std::__1::integral_constant", align 1
  %450 = alloca %"class.std::__1::basic_string"*, align 8
  %451 = alloca %"class.std::__1::basic_string"*, align 8
  %452 = alloca %"class.std::__1::basic_string"*, align 8
  %453 = alloca %"class.std::__1::basic_string"*, align 8
  %454 = alloca %"struct.std::__1::integral_constant", align 1
  %455 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %456 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %457 = alloca %"class.std::__1::basic_string"*, align 8
  %458 = alloca [3 x i64]*, align 8
  %459 = alloca i32, align 4
  %460 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %461 = alloca %"class.std::__1::basic_string"*, align 8
  %462 = alloca %"class.std::__1::basic_string"*, align 8
  %463 = alloca %"class.std::__1::basic_string"*, align 8
  %464 = alloca %"class.std::__1::basic_string"*, align 8
  %465 = alloca %"class.std::__1::basic_string"*, align 8
  %466 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %467 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %468 = alloca %"class.std::__1::basic_string"*, align 8
  %469 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %470 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %471 = alloca %"class.std::__1::basic_string"*, align 8
  %472 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %473 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %474 = alloca %"class.std::__1::basic_string"*, align 8
  %475 = alloca %"class.std::__1::basic_string"*, align 8
  %476 = alloca i8*, align 8
  %477 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %478 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %479 = alloca %"class.std::__1::basic_string"*, align 8
  %480 = alloca i8*, align 8
  %481 = alloca i8*, align 8
  %482 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %483 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %484 = alloca %"class.std::__1::basic_string"*, align 8
  %485 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %486 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %487 = alloca %"class.std::__1::basic_string"*, align 8
  %488 = alloca %"class.std::__1::basic_string"*, align 8
  %489 = alloca %"class.std::__1::basic_string"*, align 8
  %490 = alloca %"class.std::__1::basic_string"*, align 8
  %491 = alloca %"class.std::__1::basic_string"*, align 8
  %492 = alloca %"class.std::__1::basic_string"*, align 8
  %493 = alloca %"class.std::__1::basic_string"*, align 8
  %494 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %495 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %496 = alloca %"class.std::__1::basic_string"*, align 8
  %497 = alloca [3 x i64]*, align 8
  %498 = alloca i32, align 4
  %499 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %500 = alloca %"class.std::__1::basic_string"*, align 8
  %501 = alloca %"class.std::__1::basic_string"*, align 8
  %502 = alloca %"class.std::__1::basic_string"*, align 8
  %503 = alloca %"class.std::__1::basic_string"*, align 8
  %504 = alloca %"class.std::__1::basic_string"*, align 8
  %505 = alloca %"class.std::__1::basic_string"*, align 8
  %506 = alloca i8*, align 8
  %507 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %508 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %509 = alloca %"class.std::__1::basic_string"*, align 8
  %510 = alloca [3 x i64]*, align 8
  %511 = alloca i32, align 4
  %512 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %513 = alloca %"class.std::__1::basic_string"*, align 8
  %514 = alloca %"class.std::__1::basic_string"*, align 8
  %515 = alloca %"class.std::__1::basic_string"*, align 8
  %516 = alloca %"class.std::__1::basic_string"*, align 8
  %517 = alloca %"class.std::__1::basic_string"*, align 8
  %518 = alloca i8*, align 8
  %519 = alloca %"class.std::__1::basic_string"*, align 8
  %520 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %521 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %522 = alloca %"class.std::__1::basic_string"*, align 8
  %523 = alloca [3 x i64]*, align 8
  %524 = alloca i32, align 4
  %525 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %526 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %527 = alloca %"class.std::__1::basic_string"*, align 8
  %528 = alloca %"class.std::__1::allocator"*, align 8
  %529 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %530 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %531 = alloca %"class.std::__1::basic_string"*, align 8
  %532 = alloca %"struct.std::__1::integral_constant", align 1
  %533 = alloca %"class.std::__1::basic_string"*, align 8
  %534 = alloca %"class.std::__1::basic_string"*, align 8
  %535 = alloca %"class.std::__1::basic_string"*, align 8
  %536 = alloca %"class.std::__1::basic_string"*, align 8
  %537 = alloca %"struct.std::__1::integral_constant", align 1
  %538 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %539 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %540 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %541 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %542 = alloca %"class.std::__1::basic_string"*, align 8
  %543 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %544 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %545 = alloca %"class.std::__1::basic_string"*, align 8
  %546 = alloca i64, align 8
  %547 = alloca i8*, align 8
  %548 = alloca i8*, align 8
  %549 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %550 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %551 = alloca %"class.std::__1::basic_string"*, align 8
  %552 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %553 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %554 = alloca %"class.std::__1::basic_string"*, align 8
  %555 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %556 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %557 = alloca %"class.std::__1::basic_string"*, align 8
  %558 = alloca i64, align 8
  %559 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %560 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %561 = alloca %"class.std::__1::basic_string"*, align 8
  %562 = alloca %"class.std::__1::basic_string"*, align 8
  %563 = alloca %"class.std::__1::basic_string"*, align 8
  %564 = alloca i8, align 1
  %565 = alloca i8, align 1
  %566 = alloca %"struct.std::__1::integral_constant", align 1
  %567 = alloca %"class.std::__1::basic_string"*, align 8
  %568 = alloca %"class.std::__1::basic_string"*, align 8
  %569 = alloca %"class.std::__1::basic_string"*, align 8
  %570 = alloca %"class.std::__1::basic_string"*, align 8
  %571 = alloca %"struct.std::__1::integral_constant", align 1
  %572 = alloca %"class.std::__1::allocator"*, align 8
  %573 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %574 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %575 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %576 = alloca %"class.std::__1::basic_string"*, align 8
  %577 = alloca i64, align 8
  %578 = alloca i8, align 1
  %579 = alloca %"class.std::__1::basic_string"*, align 8
  %580 = alloca i64, align 8
  %581 = alloca i8, align 1
  %582 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %583 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %584 = alloca %"class.std::__1::basic_string"*, align 8
  %585 = alloca [3 x i64]*, align 8
  %586 = alloca i32, align 4
  %587 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %588 = alloca %"class.std::__1::basic_string"*, align 8
  %589 = alloca %"class.std::__1::basic_string"*, align 8
  %590 = alloca %"class.std::__1::basic_string"*, align 8
  %591 = alloca %"class.std::__1::basic_string"*, align 8
  %592 = alloca %"class.std::__1::basic_string"*, align 8
  %593 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %594 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %595 = alloca %"class.std::__1::basic_string"*, align 8
  %596 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %597 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %598 = alloca %"class.std::__1::basic_string"*, align 8
  %599 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %600 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %601 = alloca %"class.std::__1::basic_string"*, align 8
  %602 = alloca %"class.std::__1::basic_string"*, align 8
  %603 = alloca i8*, align 8
  %604 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %605 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %606 = alloca %"class.std::__1::basic_string"*, align 8
  %607 = alloca i8*, align 8
  %608 = alloca i8*, align 8
  %609 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %610 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %611 = alloca %"class.std::__1::basic_string"*, align 8
  %612 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %613 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %614 = alloca %"class.std::__1::basic_string"*, align 8
  %615 = alloca %"class.std::__1::basic_string"*, align 8
  %616 = alloca %"class.std::__1::basic_string"*, align 8
  %617 = alloca %"class.std::__1::basic_string"*, align 8
  %618 = alloca %"class.std::__1::basic_string"*, align 8
  %619 = alloca %"class.std::__1::basic_string"*, align 8
  %620 = alloca %"class.std::__1::basic_string"*, align 8
  %621 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %622 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %623 = alloca %"class.std::__1::basic_string"*, align 8
  %624 = alloca [3 x i64]*, align 8
  %625 = alloca i32, align 4
  %626 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %627 = alloca %"class.std::__1::basic_string"*, align 8
  %628 = alloca %"class.std::__1::basic_string"*, align 8
  %629 = alloca %"class.std::__1::basic_string"*, align 8
  %630 = alloca %"class.std::__1::basic_string"*, align 8
  %631 = alloca %"class.std::__1::basic_string"*, align 8
  %632 = alloca %"class.std::__1::basic_string"*, align 8
  %633 = alloca i8*, align 8
  %634 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %635 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %636 = alloca %"class.std::__1::basic_string"*, align 8
  %637 = alloca [3 x i64]*, align 8
  %638 = alloca i32, align 4
  %639 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %640 = alloca %"class.std::__1::basic_string"*, align 8
  %641 = alloca %"class.std::__1::basic_string"*, align 8
  %642 = alloca %"class.std::__1::basic_string"*, align 8
  %643 = alloca %"class.std::__1::basic_string"*, align 8
  %644 = alloca %"class.std::__1::basic_string"*, align 8
  %645 = alloca i8*, align 8
  %646 = alloca %"class.std::__1::basic_string"*, align 8
  %647 = alloca %"class.std::__1::basic_string"*, align 8
  %648 = alloca i8*, align 8
  %649 = alloca %"class.std::__1::basic_string"*, align 8
  %650 = alloca i8*, align 8
  %651 = alloca %"class.std::__1::basic_string"*, align 8
  %652 = alloca i8*, align 8
  %653 = alloca %"class.std::__1::basic_string"*, align 8
  %654 = alloca i8*, align 8
  %655 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %656 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %657 = alloca %"class.std::__1::basic_string"*, align 8
  %658 = alloca [3 x i64]*, align 8
  %659 = alloca i32, align 4
  %660 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %661 = alloca %"class.std::__1::basic_string"*, align 8
  %662 = alloca %"class.std::__1::basic_string"*, align 8
  %663 = alloca %"class.std::__1::basic_string"*, align 8
  %664 = alloca %"class.std::__1::basic_string"*, align 8
  %665 = alloca %"class.std::__1::basic_string"*, align 8
  %666 = alloca %"class.std::__1::basic_string"*, align 8
  %667 = alloca i8*, align 8
  %668 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %669 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %670 = alloca %"class.std::__1::basic_string"*, align 8
  %671 = alloca [3 x i64]*, align 8
  %672 = alloca i32, align 4
  %673 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %674 = alloca %"class.std::__1::basic_string"*, align 8
  %675 = alloca %"class.std::__1::basic_string"*, align 8
  %676 = alloca %"class.std::__1::basic_string"*, align 8
  %677 = alloca %"class.std::__1::basic_string"*, align 8
  %678 = alloca %"class.std::__1::basic_string"*, align 8
  %679 = alloca %"class.std::__1::basic_string"*, align 8
  %680 = alloca i8*, align 8
  %681 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %682 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %683 = alloca %"class.std::__1::basic_string"*, align 8
  %684 = alloca [3 x i64]*, align 8
  %685 = alloca i32, align 4
  %686 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %687 = alloca %"class.std::__1::basic_string"*, align 8
  %688 = alloca %"class.std::__1::basic_string"*, align 8
  %689 = alloca %"class.std::__1::basic_string"*, align 8
  %690 = alloca %"class.std::__1::basic_string"*, align 8
  %691 = alloca %"class.std::__1::basic_string"*, align 8
  %692 = alloca %"class.std::__1::basic_string"*, align 8
  %693 = alloca i8*, align 8
  %694 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %695 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %696 = alloca %"class.std::__1::basic_string"*, align 8
  %697 = alloca [3 x i64]*, align 8
  %698 = alloca i32, align 4
  %699 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %700 = alloca %"class.std::__1::basic_string"*, align 8
  %701 = alloca %"class.std::__1::basic_string"*, align 8
  %702 = alloca %"class.std::__1::basic_string"*, align 8
  %703 = alloca %"class.std::__1::basic_string"*, align 8
  %704 = alloca %"class.std::__1::basic_string"*, align 8
  %705 = alloca %"class.std::__1::basic_string"*, align 8
  %706 = alloca i8*, align 8
  %707 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %708 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %709 = alloca %"class.std::__1::basic_string"*, align 8
  %710 = alloca [3 x i64]*, align 8
  %711 = alloca i32, align 4
  %712 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %713 = alloca %"class.std::__1::basic_string"*, align 8
  %714 = alloca %"class.std::__1::basic_string"*, align 8
  %715 = alloca %"class.std::__1::basic_string"*, align 8
  %716 = alloca %"class.std::__1::basic_string"*, align 8
  %717 = alloca %"class.std::__1::basic_string"*, align 8
  %718 = alloca %"class.std::__1::basic_string"*, align 8
  %719 = alloca i8*, align 8
  %720 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %721 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %722 = alloca %"class.std::__1::basic_string"*, align 8
  %723 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %724 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %725 = alloca %"class.std::__1::basic_string"*, align 8
  %726 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %727 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %728 = alloca %"class.std::__1::basic_string"*, align 8
  %729 = alloca %"class.std::__1::basic_string"*, align 8
  %730 = alloca %"class.std::__1::basic_string"*, align 8
  %731 = alloca i32, align 4
  %732 = alloca %"class.std::__1::basic_string", align 8
  %733 = alloca %"class.std::__1::basic_string", align 8
  %734 = alloca i8*
  %735 = alloca i32
  %736 = alloca %"class.std::invalid_argument", align 8
  %737 = alloca %"class.std::__1::basic_string", align 8
  %738 = alloca %"class.std::__1::basic_string", align 8
  %739 = alloca %"class.std::invalid_argument", align 8
  %740 = alloca %"class.std::__1::basic_string", align 8
  %741 = alloca %"class.std::__1::basic_string", align 8
  %742 = alloca %"class.std::invalid_argument", align 8
  %743 = alloca %"class.std::__1::basic_string", align 8
  %744 = alloca %"class.std::__1::basic_string", align 8
  %745 = alloca %"class.std::invalid_argument", align 8
  %746 = alloca %"class.std::__1::basic_string", align 8
  %747 = alloca %"class.std::__1::basic_string", align 8
  %748 = alloca %"class.std::__1::basic_string", align 8
  %749 = alloca %"class.std::__1::basic_string", align 8
  %750 = alloca %"class.std::__1::basic_string", align 8
  %751 = alloca %"class.std::__1::basic_string", align 8
  %752 = alloca %"class.std::__1::basic_string", align 8
  %753 = alloca %"class.std::__1::basic_string", align 8
  %754 = alloca %"class.std::__1::basic_string", align 8
  %755 = alloca %"class.std::__1::basic_string", align 8
  %756 = alloca %"class.std::__1::basic_string", align 8
  %757 = alloca %"class.std::__1::basic_string", align 8
  %758 = alloca %"class.std::__1::basic_string", align 8
  %759 = alloca %"class.std::__1::basic_string", align 8
  %760 = alloca %"class.std::__1::basic_string", align 8
  %761 = alloca %"class.std::__1::basic_string", align 8
  %762 = alloca %"class.std::__1::basic_string", align 8
  %763 = alloca %"class.std::__1::basic_string", align 8
  %764 = alloca %"class.std::__1::basic_string", align 8
  %765 = alloca %"class.std::__1::basic_string", align 8
  %766 = alloca %"class.std::__1::basic_string", align 8
  %767 = alloca %"class.std::__1::basic_string", align 8
  %768 = alloca %"class.std::__1::basic_string", align 8
  %769 = alloca %"class.std::__1::basic_string", align 8
  %770 = alloca %"class.std::__1::basic_string", align 8
  %771 = alloca %"class.std::__1::basic_string", align 8
  %772 = alloca %"class.std::__1::basic_string", align 8
  %773 = alloca %"class.std::__1::basic_string", align 8
  %774 = alloca %"class.std::__1::basic_string", align 8
  %775 = alloca %"class.std::__1::basic_string", align 8
  %776 = alloca %"class.std::__1::basic_string", align 8
  %777 = alloca %"class.std::__1::basic_string", align 8
  %778 = alloca %"class.std::__1::basic_string", align 8
  %779 = alloca %"class.std::__1::basic_string", align 8
  %780 = alloca %"class.std::__1::basic_string", align 8
  %781 = alloca %"class.std::__1::basic_string", align 8
  %782 = alloca %"class.std::__1::basic_string", align 8
  %783 = alloca %"class.std::__1::basic_string", align 8
  %784 = alloca %"class.std::__1::basic_string", align 8
  %785 = alloca %"class.std::__1::basic_string", align 8
  %786 = alloca %"class.std::__1::basic_string", align 8
  store i32 %2, i32* %731, align 4
  %787 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %788 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %787, i32 0, i32 0
  %789 = load i32, i32* %788, align 8
  %790 = load i32, i32* %731, align 4
  %791 = icmp eq i32 %789, %790
  br i1 %791, label %795, label %792

; <label>:792:                                    ; preds = %3
  %793 = load i32, i32* %731, align 4
  %794 = icmp eq i32 %793, 8
  br i1 %794, label %795, label %796

; <label>:795:                                    ; preds = %792, %3
  call void @_ZN7VariantC1ERKS_(%struct.Variant* %0, %struct.Variant* dereferenceable(264) %1)
  br label %3826

; <label>:796:                                    ; preds = %792
  %797 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %798 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %797, i32 0, i32 0
  %799 = load i32, i32* %798, align 8
  switch i32 %799, label %3591 [
    i32 4, label %800
    i32 6, label %1480
    i32 0, label %1919
    i32 1, label %1919
    i32 2, label %2557
    i32 3, label %2557
    i32 5, label %3239
  ]

; <label>:800:                                    ; preds = %796
  %801 = load i32, i32* %731, align 4
  %802 = icmp eq i32 %801, 6
  br i1 %802, label %803, label %905

; <label>:803:                                    ; preds = %800
  %804 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  store %"class.std::__1::basic_string"* %804, %"class.std::__1::basic_string"** %730, align 8
  %805 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %730, align 8
  store %"class.std::__1::basic_string"* %805, %"class.std::__1::basic_string"** %729, align 8
  %806 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %729, align 8
  store %"class.std::__1::basic_string"* %806, %"class.std::__1::basic_string"** %728, align 8
  %807 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %728, align 8
  %808 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %807, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %808, %"class.std::__1::__compressed_pair"** %727, align 8
  %809 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %727, align 8
  %810 = bitcast %"class.std::__1::__compressed_pair"* %809 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %810, %"class.std::__1::__libcpp_compressed_pair_imp"** %726, align 8
  %811 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %726, align 8
  %812 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %811, i32 0, i32 0
  %813 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %812, i32 0, i32 0
  %814 = bitcast %union.anon.0* %813 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %815 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %814, i32 0, i32 0
  %816 = bitcast %union.anon.1* %815 to i8*
  %817 = load i8, i8* %816, align 8
  %818 = zext i8 %817 to i32
  %819 = and i32 %818, 1
  %820 = icmp ne i32 %819, 0
  br i1 %820, label %821, label %832

; <label>:821:                                    ; preds = %803
  store %"class.std::__1::basic_string"* %806, %"class.std::__1::basic_string"** %722, align 8
  %822 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %722, align 8
  %823 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %822, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %823, %"class.std::__1::__compressed_pair"** %721, align 8
  %824 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %721, align 8
  %825 = bitcast %"class.std::__1::__compressed_pair"* %824 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %825, %"class.std::__1::__libcpp_compressed_pair_imp"** %720, align 8
  %826 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %720, align 8
  %827 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %826, i32 0, i32 0
  %828 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %827, i32 0, i32 0
  %829 = bitcast %union.anon.0* %828 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %830 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %829, i32 0, i32 1
  %831 = load i64, i64* %830, align 8
  br label %847

; <label>:832:                                    ; preds = %803
  store %"class.std::__1::basic_string"* %806, %"class.std::__1::basic_string"** %725, align 8
  %833 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %725, align 8
  %834 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %833, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %834, %"class.std::__1::__compressed_pair"** %724, align 8
  %835 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %724, align 8
  %836 = bitcast %"class.std::__1::__compressed_pair"* %835 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %836, %"class.std::__1::__libcpp_compressed_pair_imp"** %723, align 8
  %837 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %723, align 8
  %838 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %837, i32 0, i32 0
  %839 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %838, i32 0, i32 0
  %840 = bitcast %union.anon.0* %839 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %841 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %840, i32 0, i32 0
  %842 = bitcast %union.anon.1* %841 to i8*
  %843 = load i8, i8* %842, align 8
  %844 = zext i8 %843 to i32
  %845 = ashr i32 %844, 1
  %846 = sext i32 %845 to i64
  br label %847

; <label>:847:                                    ; preds = %821, %832
  %848 = phi i64 [ %831, %821 ], [ %846, %832 ]
  %849 = icmp ugt i64 %848, 1
  br i1 %849, label %850, label %899

; <label>:850:                                    ; preds = %847
  %851 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  call void @_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_RKS9_(%"class.std::__1::basic_string"* sret %733, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0), %"class.std::__1::basic_string"* dereferenceable(24) %851)
  store %"class.std::__1::basic_string"* %733, %"class.std::__1::basic_string"** %718, align 8, !noalias !2
  store i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.1, i32 0, i32 0), i8** %719, align 8, !noalias !2
  %852 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %718, align 8, !noalias !2
  %853 = load i8*, i8** %719, align 8, !noalias !2
  %854 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %852, i8* %853)
          to label %855 unwind label %890

; <label>:855:                                    ; preds = %850
  store %"class.std::__1::basic_string"* %854, %"class.std::__1::basic_string"** %717, align 8, !noalias !2
  %856 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %717, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %732, %"class.std::__1::basic_string"** %715, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %856, %"class.std::__1::basic_string"** %716, align 8, !noalias !2
  %857 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %715, align 8, !noalias !2
  %858 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %716, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %857, %"class.std::__1::basic_string"** %713, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %858, %"class.std::__1::basic_string"** %714, align 8, !noalias !2
  %859 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %713, align 8, !noalias !2
  %860 = bitcast %"class.std::__1::basic_string"* %859 to %"class.std::__1::__basic_string_common"*
  %861 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %859, i32 0, i32 0
  %862 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %714, align 8, !noalias !2
  %863 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %862, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %863, %"class.std::__1::__compressed_pair"** %712, align 8, !noalias !2
  %864 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %712, align 8, !noalias !2
  %865 = bitcast %"class.std::__1::__compressed_pair"* %861 to i8*
  %866 = bitcast %"class.std::__1::__compressed_pair"* %864 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %865, i8* %866, i64 24, i32 8, i1 false) #8
  %867 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %714, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %867, %"class.std::__1::basic_string"** %709, align 8, !noalias !2
  %868 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %709, align 8, !noalias !2
  %869 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %868, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %869, %"class.std::__1::__compressed_pair"** %708, align 8, !noalias !2
  %870 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %708, align 8, !noalias !2
  %871 = bitcast %"class.std::__1::__compressed_pair"* %870 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %871, %"class.std::__1::__libcpp_compressed_pair_imp"** %707, align 8, !noalias !2
  %872 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %707, align 8, !noalias !2
  %873 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %872, i32 0, i32 0
  %874 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %873, i32 0, i32 0
  %875 = bitcast %union.anon.0* %874 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %876 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %875, i32 0, i32 0
  store [3 x i64]* %876, [3 x i64]** %710, align 8, !noalias !2
  store i32 0, i32* %711, align 4, !noalias !2
  br label %877

; <label>:877:                                    ; preds = %880, %855
  %878 = load i32, i32* %711, align 4, !noalias !2
  %879 = icmp ult i32 %878, 3
  br i1 %879, label %880, label %887

; <label>:880:                                    ; preds = %877
  %881 = load i32, i32* %711, align 4, !noalias !2
  %882 = zext i32 %881 to i64
  %883 = load [3 x i64]*, [3 x i64]** %710, align 8, !noalias !2
  %884 = getelementptr inbounds [3 x i64], [3 x i64]* %883, i64 0, i64 %882
  store i64 0, i64* %884, align 8
  %885 = load i32, i32* %711, align 4, !noalias !2
  %886 = add i32 %885, 1
  store i32 %886, i32* %711, align 4, !noalias !2
  br label %877

; <label>:887:                                    ; preds = %877
  br label %888

; <label>:888:                                    ; preds = %887
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %732, %class.AstNode* null)
          to label %889 unwind label %894

; <label>:889:                                    ; preds = %888
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %732) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %733) #8
  br label %899

; <label>:890:                                    ; preds = %850
  %891 = landingpad { i8*, i32 }
          cleanup
  %892 = extractvalue { i8*, i32 } %891, 0
  store i8* %892, i8** %734, align 8
  %893 = extractvalue { i8*, i32 } %891, 1
  store i32 %893, i32* %735, align 4
  br label %898

; <label>:894:                                    ; preds = %888
  %895 = landingpad { i8*, i32 }
          cleanup
  %896 = extractvalue { i8*, i32 } %895, 0
  store i8* %896, i8** %734, align 8
  %897 = extractvalue { i8*, i32 } %895, 1
  store i32 %897, i32* %735, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %732) #8
  br label %898

; <label>:898:                                    ; preds = %894, %890
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %733) #8
  br label %3827

; <label>:899:                                    ; preds = %889, %847
  %900 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  %901 = call dereferenceable(1) i8* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE2atEm(%"class.std::__1::basic_string"* %900, i64 0)
  %902 = load i8, i8* %901, align 1
  %903 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %904 = bitcast %union.anon* %903 to i8*
  store i8 %902, i8* %904, align 8
  br label %1479

; <label>:905:                                    ; preds = %800
  %906 = load i32, i32* %731, align 4
  %907 = icmp eq i32 %906, 0
  br i1 %907, label %908, label %983

; <label>:908:                                    ; preds = %905
  %909 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  %910 = invoke i32 @_ZNSt3__14stoiERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPmi(%"class.std::__1::basic_string"* dereferenceable(24) %909, i64* null, i32 10)
          to label %911 unwind label %914

; <label>:911:                                    ; preds = %908
  %912 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %913 = bitcast %union.anon* %912 to i32*
  store i32 %910, i32* %913, align 8
  br label %967

; <label>:914:                                    ; preds = %908
  %915 = landingpad { i8*, i32 }
          catch i8* bitcast (i8** @_ZTISt16invalid_argument to i8*)
  %916 = extractvalue { i8*, i32 } %915, 0
  store i8* %916, i8** %734, align 8
  %917 = extractvalue { i8*, i32 } %915, 1
  store i32 %917, i32* %735, align 4
  br label %918

; <label>:918:                                    ; preds = %914
  %919 = load i32, i32* %735, align 4
  %920 = call i32 @llvm.eh.typeid.for(i8* bitcast (i8** @_ZTISt16invalid_argument to i8*)) #8
  %921 = icmp eq i32 %919, %920
  br i1 %921, label %922, label %3827

; <label>:922:                                    ; preds = %918
  %923 = load i8*, i8** %734, align 8
  %924 = call i8* @__cxa_get_exception_ptr(i8* %923) #8
  %925 = bitcast i8* %924 to %"class.std::invalid_argument"*
  call void @_ZNSt16invalid_argumentC1ERKS_(%"class.std::invalid_argument"* %736, %"class.std::invalid_argument"* dereferenceable(16) %925) #8
  %926 = call i8* @__cxa_begin_catch(i8* %923) #8
  %927 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  invoke void @_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_RKS9_(%"class.std::__1::basic_string"* sret %738, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0), %"class.std::__1::basic_string"* dereferenceable(24) %927)
          to label %928 unwind label %968

; <label>:928:                                    ; preds = %922
  store %"class.std::__1::basic_string"* %738, %"class.std::__1::basic_string"** %705, align 8, !noalias !5
  store i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.2, i32 0, i32 0), i8** %706, align 8, !noalias !5
  %929 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %705, align 8, !noalias !5
  %930 = load i8*, i8** %706, align 8, !noalias !5
  %931 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %929, i8* %930)
          to label %932 unwind label %972

; <label>:932:                                    ; preds = %928
  store %"class.std::__1::basic_string"* %931, %"class.std::__1::basic_string"** %704, align 8, !noalias !5
  %933 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %704, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %737, %"class.std::__1::basic_string"** %702, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %933, %"class.std::__1::basic_string"** %703, align 8, !noalias !5
  %934 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %702, align 8, !noalias !5
  %935 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %703, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %934, %"class.std::__1::basic_string"** %700, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %935, %"class.std::__1::basic_string"** %701, align 8, !noalias !5
  %936 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %700, align 8, !noalias !5
  %937 = bitcast %"class.std::__1::basic_string"* %936 to %"class.std::__1::__basic_string_common"*
  %938 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %936, i32 0, i32 0
  %939 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %701, align 8, !noalias !5
  %940 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %939, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %940, %"class.std::__1::__compressed_pair"** %699, align 8, !noalias !5
  %941 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %699, align 8, !noalias !5
  %942 = bitcast %"class.std::__1::__compressed_pair"* %938 to i8*
  %943 = bitcast %"class.std::__1::__compressed_pair"* %941 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %942, i8* %943, i64 24, i32 8, i1 false) #8
  %944 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %701, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %944, %"class.std::__1::basic_string"** %696, align 8, !noalias !5
  %945 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %696, align 8, !noalias !5
  %946 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %945, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %946, %"class.std::__1::__compressed_pair"** %695, align 8, !noalias !5
  %947 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %695, align 8, !noalias !5
  %948 = bitcast %"class.std::__1::__compressed_pair"* %947 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %948, %"class.std::__1::__libcpp_compressed_pair_imp"** %694, align 8, !noalias !5
  %949 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %694, align 8, !noalias !5
  %950 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %949, i32 0, i32 0
  %951 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %950, i32 0, i32 0
  %952 = bitcast %union.anon.0* %951 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %953 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %952, i32 0, i32 0
  store [3 x i64]* %953, [3 x i64]** %697, align 8, !noalias !5
  store i32 0, i32* %698, align 4, !noalias !5
  br label %954

; <label>:954:                                    ; preds = %957, %932
  %955 = load i32, i32* %698, align 4, !noalias !5
  %956 = icmp ult i32 %955, 3
  br i1 %956, label %957, label %964

; <label>:957:                                    ; preds = %954
  %958 = load i32, i32* %698, align 4, !noalias !5
  %959 = zext i32 %958 to i64
  %960 = load [3 x i64]*, [3 x i64]** %697, align 8, !noalias !5
  %961 = getelementptr inbounds [3 x i64], [3 x i64]* %960, i64 0, i64 %959
  store i64 0, i64* %961, align 8
  %962 = load i32, i32* %698, align 4, !noalias !5
  %963 = add i32 %962, 1
  store i32 %963, i32* %698, align 4, !noalias !5
  br label %954

; <label>:964:                                    ; preds = %954
  br label %965

; <label>:965:                                    ; preds = %964
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %737, %class.AstNode* null)
          to label %966 unwind label %976

; <label>:966:                                    ; preds = %965
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %737) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %738) #8
  call void @_ZNSt16invalid_argumentD1Ev(%"class.std::invalid_argument"* %736) #8
  call void @__cxa_end_catch()
  br label %967

; <label>:967:                                    ; preds = %966, %911
  br label %1478

; <label>:968:                                    ; preds = %922
  %969 = landingpad { i8*, i32 }
          cleanup
  %970 = extractvalue { i8*, i32 } %969, 0
  store i8* %970, i8** %734, align 8
  %971 = extractvalue { i8*, i32 } %969, 1
  store i32 %971, i32* %735, align 4
  br label %981

; <label>:972:                                    ; preds = %928
  %973 = landingpad { i8*, i32 }
          cleanup
  %974 = extractvalue { i8*, i32 } %973, 0
  store i8* %974, i8** %734, align 8
  %975 = extractvalue { i8*, i32 } %973, 1
  store i32 %975, i32* %735, align 4
  br label %980

; <label>:976:                                    ; preds = %965
  %977 = landingpad { i8*, i32 }
          cleanup
  %978 = extractvalue { i8*, i32 } %977, 0
  store i8* %978, i8** %734, align 8
  %979 = extractvalue { i8*, i32 } %977, 1
  store i32 %979, i32* %735, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %737) #8
  br label %980

; <label>:980:                                    ; preds = %976, %972
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %738) #8
  br label %981

; <label>:981:                                    ; preds = %980, %968
  call void @_ZNSt16invalid_argumentD1Ev(%"class.std::invalid_argument"* %736) #8
  invoke void @__cxa_end_catch()
          to label %982 unwind label %3832

; <label>:982:                                    ; preds = %981
  br label %3827

; <label>:983:                                    ; preds = %905
  %984 = load i32, i32* %731, align 4
  %985 = icmp eq i32 %984, 1
  br i1 %985, label %986, label %1061

; <label>:986:                                    ; preds = %983
  %987 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  %988 = invoke i64 @_ZNSt3__14stolERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPmi(%"class.std::__1::basic_string"* dereferenceable(24) %987, i64* null, i32 10)
          to label %989 unwind label %992

; <label>:989:                                    ; preds = %986
  %990 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %991 = bitcast %union.anon* %990 to i64*
  store i64 %988, i64* %991, align 8
  br label %1045

; <label>:992:                                    ; preds = %986
  %993 = landingpad { i8*, i32 }
          catch i8* bitcast (i8** @_ZTISt16invalid_argument to i8*)
  %994 = extractvalue { i8*, i32 } %993, 0
  store i8* %994, i8** %734, align 8
  %995 = extractvalue { i8*, i32 } %993, 1
  store i32 %995, i32* %735, align 4
  br label %996

; <label>:996:                                    ; preds = %992
  %997 = load i32, i32* %735, align 4
  %998 = call i32 @llvm.eh.typeid.for(i8* bitcast (i8** @_ZTISt16invalid_argument to i8*)) #8
  %999 = icmp eq i32 %997, %998
  br i1 %999, label %1000, label %3827

; <label>:1000:                                   ; preds = %996
  %1001 = load i8*, i8** %734, align 8
  %1002 = call i8* @__cxa_get_exception_ptr(i8* %1001) #8
  %1003 = bitcast i8* %1002 to %"class.std::invalid_argument"*
  call void @_ZNSt16invalid_argumentC1ERKS_(%"class.std::invalid_argument"* %739, %"class.std::invalid_argument"* dereferenceable(16) %1003) #8
  %1004 = call i8* @__cxa_begin_catch(i8* %1001) #8
  %1005 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  invoke void @_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_RKS9_(%"class.std::__1::basic_string"* sret %741, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0), %"class.std::__1::basic_string"* dereferenceable(24) %1005)
          to label %1006 unwind label %1046

; <label>:1006:                                   ; preds = %1000
  store %"class.std::__1::basic_string"* %741, %"class.std::__1::basic_string"** %692, align 8, !noalias !8
  store i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.2, i32 0, i32 0), i8** %693, align 8, !noalias !8
  %1007 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %692, align 8, !noalias !8
  %1008 = load i8*, i8** %693, align 8, !noalias !8
  %1009 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %1007, i8* %1008)
          to label %1010 unwind label %1050

; <label>:1010:                                   ; preds = %1006
  store %"class.std::__1::basic_string"* %1009, %"class.std::__1::basic_string"** %691, align 8, !noalias !8
  %1011 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %691, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %740, %"class.std::__1::basic_string"** %689, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %1011, %"class.std::__1::basic_string"** %690, align 8, !noalias !8
  %1012 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %689, align 8, !noalias !8
  %1013 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %690, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %1012, %"class.std::__1::basic_string"** %687, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %1013, %"class.std::__1::basic_string"** %688, align 8, !noalias !8
  %1014 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %687, align 8, !noalias !8
  %1015 = bitcast %"class.std::__1::basic_string"* %1014 to %"class.std::__1::__basic_string_common"*
  %1016 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1014, i32 0, i32 0
  %1017 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %688, align 8, !noalias !8
  %1018 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1017, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1018, %"class.std::__1::__compressed_pair"** %686, align 8, !noalias !8
  %1019 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %686, align 8, !noalias !8
  %1020 = bitcast %"class.std::__1::__compressed_pair"* %1016 to i8*
  %1021 = bitcast %"class.std::__1::__compressed_pair"* %1019 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1020, i8* %1021, i64 24, i32 8, i1 false) #8
  %1022 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %688, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %1022, %"class.std::__1::basic_string"** %683, align 8, !noalias !8
  %1023 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %683, align 8, !noalias !8
  %1024 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1023, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1024, %"class.std::__1::__compressed_pair"** %682, align 8, !noalias !8
  %1025 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %682, align 8, !noalias !8
  %1026 = bitcast %"class.std::__1::__compressed_pair"* %1025 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1026, %"class.std::__1::__libcpp_compressed_pair_imp"** %681, align 8, !noalias !8
  %1027 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %681, align 8, !noalias !8
  %1028 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1027, i32 0, i32 0
  %1029 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1028, i32 0, i32 0
  %1030 = bitcast %union.anon.0* %1029 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %1031 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %1030, i32 0, i32 0
  store [3 x i64]* %1031, [3 x i64]** %684, align 8, !noalias !8
  store i32 0, i32* %685, align 4, !noalias !8
  br label %1032

; <label>:1032:                                   ; preds = %1035, %1010
  %1033 = load i32, i32* %685, align 4, !noalias !8
  %1034 = icmp ult i32 %1033, 3
  br i1 %1034, label %1035, label %1042

; <label>:1035:                                   ; preds = %1032
  %1036 = load i32, i32* %685, align 4, !noalias !8
  %1037 = zext i32 %1036 to i64
  %1038 = load [3 x i64]*, [3 x i64]** %684, align 8, !noalias !8
  %1039 = getelementptr inbounds [3 x i64], [3 x i64]* %1038, i64 0, i64 %1037
  store i64 0, i64* %1039, align 8
  %1040 = load i32, i32* %685, align 4, !noalias !8
  %1041 = add i32 %1040, 1
  store i32 %1041, i32* %685, align 4, !noalias !8
  br label %1032

; <label>:1042:                                   ; preds = %1032
  br label %1043

; <label>:1043:                                   ; preds = %1042
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %740, %class.AstNode* null)
          to label %1044 unwind label %1054

; <label>:1044:                                   ; preds = %1043
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %740) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %741) #8
  call void @_ZNSt16invalid_argumentD1Ev(%"class.std::invalid_argument"* %739) #8
  call void @__cxa_end_catch()
  br label %1045

; <label>:1045:                                   ; preds = %1044, %989
  br label %1477

; <label>:1046:                                   ; preds = %1000
  %1047 = landingpad { i8*, i32 }
          cleanup
  %1048 = extractvalue { i8*, i32 } %1047, 0
  store i8* %1048, i8** %734, align 8
  %1049 = extractvalue { i8*, i32 } %1047, 1
  store i32 %1049, i32* %735, align 4
  br label %1059

; <label>:1050:                                   ; preds = %1006
  %1051 = landingpad { i8*, i32 }
          cleanup
  %1052 = extractvalue { i8*, i32 } %1051, 0
  store i8* %1052, i8** %734, align 8
  %1053 = extractvalue { i8*, i32 } %1051, 1
  store i32 %1053, i32* %735, align 4
  br label %1058

; <label>:1054:                                   ; preds = %1043
  %1055 = landingpad { i8*, i32 }
          cleanup
  %1056 = extractvalue { i8*, i32 } %1055, 0
  store i8* %1056, i8** %734, align 8
  %1057 = extractvalue { i8*, i32 } %1055, 1
  store i32 %1057, i32* %735, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %740) #8
  br label %1058

; <label>:1058:                                   ; preds = %1054, %1050
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %741) #8
  br label %1059

; <label>:1059:                                   ; preds = %1058, %1046
  call void @_ZNSt16invalid_argumentD1Ev(%"class.std::invalid_argument"* %739) #8
  invoke void @__cxa_end_catch()
          to label %1060 unwind label %3832

; <label>:1060:                                   ; preds = %1059
  br label %3827

; <label>:1061:                                   ; preds = %983
  %1062 = load i32, i32* %731, align 4
  %1063 = icmp eq i32 %1062, 2
  br i1 %1063, label %1064, label %1139

; <label>:1064:                                   ; preds = %1061
  %1065 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  %1066 = invoke float @_ZNSt3__14stofERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPm(%"class.std::__1::basic_string"* dereferenceable(24) %1065, i64* null)
          to label %1067 unwind label %1070

; <label>:1067:                                   ; preds = %1064
  %1068 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1069 = bitcast %union.anon* %1068 to float*
  store float %1066, float* %1069, align 8
  br label %1123

; <label>:1070:                                   ; preds = %1064
  %1071 = landingpad { i8*, i32 }
          catch i8* bitcast (i8** @_ZTISt16invalid_argument to i8*)
  %1072 = extractvalue { i8*, i32 } %1071, 0
  store i8* %1072, i8** %734, align 8
  %1073 = extractvalue { i8*, i32 } %1071, 1
  store i32 %1073, i32* %735, align 4
  br label %1074

; <label>:1074:                                   ; preds = %1070
  %1075 = load i32, i32* %735, align 4
  %1076 = call i32 @llvm.eh.typeid.for(i8* bitcast (i8** @_ZTISt16invalid_argument to i8*)) #8
  %1077 = icmp eq i32 %1075, %1076
  br i1 %1077, label %1078, label %3827

; <label>:1078:                                   ; preds = %1074
  %1079 = load i8*, i8** %734, align 8
  %1080 = call i8* @__cxa_get_exception_ptr(i8* %1079) #8
  %1081 = bitcast i8* %1080 to %"class.std::invalid_argument"*
  call void @_ZNSt16invalid_argumentC1ERKS_(%"class.std::invalid_argument"* %742, %"class.std::invalid_argument"* dereferenceable(16) %1081) #8
  %1082 = call i8* @__cxa_begin_catch(i8* %1079) #8
  %1083 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  invoke void @_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_RKS9_(%"class.std::__1::basic_string"* sret %744, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0), %"class.std::__1::basic_string"* dereferenceable(24) %1083)
          to label %1084 unwind label %1124

; <label>:1084:                                   ; preds = %1078
  store %"class.std::__1::basic_string"* %744, %"class.std::__1::basic_string"** %679, align 8, !noalias !11
  store i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.2, i32 0, i32 0), i8** %680, align 8, !noalias !11
  %1085 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %679, align 8, !noalias !11
  %1086 = load i8*, i8** %680, align 8, !noalias !11
  %1087 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %1085, i8* %1086)
          to label %1088 unwind label %1128

; <label>:1088:                                   ; preds = %1084
  store %"class.std::__1::basic_string"* %1087, %"class.std::__1::basic_string"** %678, align 8, !noalias !11
  %1089 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %678, align 8, !noalias !11
  store %"class.std::__1::basic_string"* %743, %"class.std::__1::basic_string"** %676, align 8, !noalias !11
  store %"class.std::__1::basic_string"* %1089, %"class.std::__1::basic_string"** %677, align 8, !noalias !11
  %1090 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %676, align 8, !noalias !11
  %1091 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %677, align 8, !noalias !11
  store %"class.std::__1::basic_string"* %1090, %"class.std::__1::basic_string"** %674, align 8, !noalias !11
  store %"class.std::__1::basic_string"* %1091, %"class.std::__1::basic_string"** %675, align 8, !noalias !11
  %1092 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %674, align 8, !noalias !11
  %1093 = bitcast %"class.std::__1::basic_string"* %1092 to %"class.std::__1::__basic_string_common"*
  %1094 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1092, i32 0, i32 0
  %1095 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %675, align 8, !noalias !11
  %1096 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1095, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1096, %"class.std::__1::__compressed_pair"** %673, align 8, !noalias !11
  %1097 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %673, align 8, !noalias !11
  %1098 = bitcast %"class.std::__1::__compressed_pair"* %1094 to i8*
  %1099 = bitcast %"class.std::__1::__compressed_pair"* %1097 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1098, i8* %1099, i64 24, i32 8, i1 false) #8
  %1100 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %675, align 8, !noalias !11
  store %"class.std::__1::basic_string"* %1100, %"class.std::__1::basic_string"** %670, align 8, !noalias !11
  %1101 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %670, align 8, !noalias !11
  %1102 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1101, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1102, %"class.std::__1::__compressed_pair"** %669, align 8, !noalias !11
  %1103 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %669, align 8, !noalias !11
  %1104 = bitcast %"class.std::__1::__compressed_pair"* %1103 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1104, %"class.std::__1::__libcpp_compressed_pair_imp"** %668, align 8, !noalias !11
  %1105 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %668, align 8, !noalias !11
  %1106 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1105, i32 0, i32 0
  %1107 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1106, i32 0, i32 0
  %1108 = bitcast %union.anon.0* %1107 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %1109 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %1108, i32 0, i32 0
  store [3 x i64]* %1109, [3 x i64]** %671, align 8, !noalias !11
  store i32 0, i32* %672, align 4, !noalias !11
  br label %1110

; <label>:1110:                                   ; preds = %1113, %1088
  %1111 = load i32, i32* %672, align 4, !noalias !11
  %1112 = icmp ult i32 %1111, 3
  br i1 %1112, label %1113, label %1120

; <label>:1113:                                   ; preds = %1110
  %1114 = load i32, i32* %672, align 4, !noalias !11
  %1115 = zext i32 %1114 to i64
  %1116 = load [3 x i64]*, [3 x i64]** %671, align 8, !noalias !11
  %1117 = getelementptr inbounds [3 x i64], [3 x i64]* %1116, i64 0, i64 %1115
  store i64 0, i64* %1117, align 8
  %1118 = load i32, i32* %672, align 4, !noalias !11
  %1119 = add i32 %1118, 1
  store i32 %1119, i32* %672, align 4, !noalias !11
  br label %1110

; <label>:1120:                                   ; preds = %1110
  br label %1121

; <label>:1121:                                   ; preds = %1120
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %743, %class.AstNode* null)
          to label %1122 unwind label %1132

; <label>:1122:                                   ; preds = %1121
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %743) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %744) #8
  call void @_ZNSt16invalid_argumentD1Ev(%"class.std::invalid_argument"* %742) #8
  call void @__cxa_end_catch()
  br label %1123

; <label>:1123:                                   ; preds = %1122, %1067
  br label %1476

; <label>:1124:                                   ; preds = %1078
  %1125 = landingpad { i8*, i32 }
          cleanup
  %1126 = extractvalue { i8*, i32 } %1125, 0
  store i8* %1126, i8** %734, align 8
  %1127 = extractvalue { i8*, i32 } %1125, 1
  store i32 %1127, i32* %735, align 4
  br label %1137

; <label>:1128:                                   ; preds = %1084
  %1129 = landingpad { i8*, i32 }
          cleanup
  %1130 = extractvalue { i8*, i32 } %1129, 0
  store i8* %1130, i8** %734, align 8
  %1131 = extractvalue { i8*, i32 } %1129, 1
  store i32 %1131, i32* %735, align 4
  br label %1136

; <label>:1132:                                   ; preds = %1121
  %1133 = landingpad { i8*, i32 }
          cleanup
  %1134 = extractvalue { i8*, i32 } %1133, 0
  store i8* %1134, i8** %734, align 8
  %1135 = extractvalue { i8*, i32 } %1133, 1
  store i32 %1135, i32* %735, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %743) #8
  br label %1136

; <label>:1136:                                   ; preds = %1132, %1128
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %744) #8
  br label %1137

; <label>:1137:                                   ; preds = %1136, %1124
  call void @_ZNSt16invalid_argumentD1Ev(%"class.std::invalid_argument"* %742) #8
  invoke void @__cxa_end_catch()
          to label %1138 unwind label %3832

; <label>:1138:                                   ; preds = %1137
  br label %3827

; <label>:1139:                                   ; preds = %1061
  %1140 = load i32, i32* %731, align 4
  %1141 = icmp eq i32 %1140, 3
  br i1 %1141, label %1142, label %1217

; <label>:1142:                                   ; preds = %1139
  %1143 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  %1144 = invoke double @_ZNSt3__14stodERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPm(%"class.std::__1::basic_string"* dereferenceable(24) %1143, i64* null)
          to label %1145 unwind label %1148

; <label>:1145:                                   ; preds = %1142
  %1146 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1147 = bitcast %union.anon* %1146 to double*
  store double %1144, double* %1147, align 8
  br label %1201

; <label>:1148:                                   ; preds = %1142
  %1149 = landingpad { i8*, i32 }
          catch i8* bitcast (i8** @_ZTISt16invalid_argument to i8*)
  %1150 = extractvalue { i8*, i32 } %1149, 0
  store i8* %1150, i8** %734, align 8
  %1151 = extractvalue { i8*, i32 } %1149, 1
  store i32 %1151, i32* %735, align 4
  br label %1152

; <label>:1152:                                   ; preds = %1148
  %1153 = load i32, i32* %735, align 4
  %1154 = call i32 @llvm.eh.typeid.for(i8* bitcast (i8** @_ZTISt16invalid_argument to i8*)) #8
  %1155 = icmp eq i32 %1153, %1154
  br i1 %1155, label %1156, label %3827

; <label>:1156:                                   ; preds = %1152
  %1157 = load i8*, i8** %734, align 8
  %1158 = call i8* @__cxa_get_exception_ptr(i8* %1157) #8
  %1159 = bitcast i8* %1158 to %"class.std::invalid_argument"*
  call void @_ZNSt16invalid_argumentC1ERKS_(%"class.std::invalid_argument"* %745, %"class.std::invalid_argument"* dereferenceable(16) %1159) #8
  %1160 = call i8* @__cxa_begin_catch(i8* %1157) #8
  %1161 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  invoke void @_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_RKS9_(%"class.std::__1::basic_string"* sret %747, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0), %"class.std::__1::basic_string"* dereferenceable(24) %1161)
          to label %1162 unwind label %1202

; <label>:1162:                                   ; preds = %1156
  store %"class.std::__1::basic_string"* %747, %"class.std::__1::basic_string"** %666, align 8, !noalias !14
  store i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.2, i32 0, i32 0), i8** %667, align 8, !noalias !14
  %1163 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %666, align 8, !noalias !14
  %1164 = load i8*, i8** %667, align 8, !noalias !14
  %1165 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %1163, i8* %1164)
          to label %1166 unwind label %1206

; <label>:1166:                                   ; preds = %1162
  store %"class.std::__1::basic_string"* %1165, %"class.std::__1::basic_string"** %665, align 8, !noalias !14
  %1167 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %665, align 8, !noalias !14
  store %"class.std::__1::basic_string"* %746, %"class.std::__1::basic_string"** %663, align 8, !noalias !14
  store %"class.std::__1::basic_string"* %1167, %"class.std::__1::basic_string"** %664, align 8, !noalias !14
  %1168 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %663, align 8, !noalias !14
  %1169 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %664, align 8, !noalias !14
  store %"class.std::__1::basic_string"* %1168, %"class.std::__1::basic_string"** %661, align 8, !noalias !14
  store %"class.std::__1::basic_string"* %1169, %"class.std::__1::basic_string"** %662, align 8, !noalias !14
  %1170 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %661, align 8, !noalias !14
  %1171 = bitcast %"class.std::__1::basic_string"* %1170 to %"class.std::__1::__basic_string_common"*
  %1172 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1170, i32 0, i32 0
  %1173 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %662, align 8, !noalias !14
  %1174 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1173, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1174, %"class.std::__1::__compressed_pair"** %660, align 8, !noalias !14
  %1175 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %660, align 8, !noalias !14
  %1176 = bitcast %"class.std::__1::__compressed_pair"* %1172 to i8*
  %1177 = bitcast %"class.std::__1::__compressed_pair"* %1175 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1176, i8* %1177, i64 24, i32 8, i1 false) #8
  %1178 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %662, align 8, !noalias !14
  store %"class.std::__1::basic_string"* %1178, %"class.std::__1::basic_string"** %657, align 8, !noalias !14
  %1179 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %657, align 8, !noalias !14
  %1180 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1179, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1180, %"class.std::__1::__compressed_pair"** %656, align 8, !noalias !14
  %1181 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %656, align 8, !noalias !14
  %1182 = bitcast %"class.std::__1::__compressed_pair"* %1181 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1182, %"class.std::__1::__libcpp_compressed_pair_imp"** %655, align 8, !noalias !14
  %1183 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %655, align 8, !noalias !14
  %1184 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1183, i32 0, i32 0
  %1185 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1184, i32 0, i32 0
  %1186 = bitcast %union.anon.0* %1185 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %1187 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %1186, i32 0, i32 0
  store [3 x i64]* %1187, [3 x i64]** %658, align 8, !noalias !14
  store i32 0, i32* %659, align 4, !noalias !14
  br label %1188

; <label>:1188:                                   ; preds = %1191, %1166
  %1189 = load i32, i32* %659, align 4, !noalias !14
  %1190 = icmp ult i32 %1189, 3
  br i1 %1190, label %1191, label %1198

; <label>:1191:                                   ; preds = %1188
  %1192 = load i32, i32* %659, align 4, !noalias !14
  %1193 = zext i32 %1192 to i64
  %1194 = load [3 x i64]*, [3 x i64]** %658, align 8, !noalias !14
  %1195 = getelementptr inbounds [3 x i64], [3 x i64]* %1194, i64 0, i64 %1193
  store i64 0, i64* %1195, align 8
  %1196 = load i32, i32* %659, align 4, !noalias !14
  %1197 = add i32 %1196, 1
  store i32 %1197, i32* %659, align 4, !noalias !14
  br label %1188

; <label>:1198:                                   ; preds = %1188
  br label %1199

; <label>:1199:                                   ; preds = %1198
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %746, %class.AstNode* null)
          to label %1200 unwind label %1210

; <label>:1200:                                   ; preds = %1199
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %746) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %747) #8
  call void @_ZNSt16invalid_argumentD1Ev(%"class.std::invalid_argument"* %745) #8
  call void @__cxa_end_catch()
  br label %1201

; <label>:1201:                                   ; preds = %1200, %1145
  br label %1475

; <label>:1202:                                   ; preds = %1156
  %1203 = landingpad { i8*, i32 }
          cleanup
  %1204 = extractvalue { i8*, i32 } %1203, 0
  store i8* %1204, i8** %734, align 8
  %1205 = extractvalue { i8*, i32 } %1203, 1
  store i32 %1205, i32* %735, align 4
  br label %1215

; <label>:1206:                                   ; preds = %1162
  %1207 = landingpad { i8*, i32 }
          cleanup
  %1208 = extractvalue { i8*, i32 } %1207, 0
  store i8* %1208, i8** %734, align 8
  %1209 = extractvalue { i8*, i32 } %1207, 1
  store i32 %1209, i32* %735, align 4
  br label %1214

; <label>:1210:                                   ; preds = %1199
  %1211 = landingpad { i8*, i32 }
          cleanup
  %1212 = extractvalue { i8*, i32 } %1211, 0
  store i8* %1212, i8** %734, align 8
  %1213 = extractvalue { i8*, i32 } %1211, 1
  store i32 %1213, i32* %735, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %746) #8
  br label %1214

; <label>:1214:                                   ; preds = %1210, %1206
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %747) #8
  br label %1215

; <label>:1215:                                   ; preds = %1214, %1202
  call void @_ZNSt16invalid_argumentD1Ev(%"class.std::invalid_argument"* %745) #8
  invoke void @__cxa_end_catch()
          to label %1216 unwind label %3832

; <label>:1216:                                   ; preds = %1215
  br label %3827

; <label>:1217:                                   ; preds = %1139
  %1218 = load i32, i32* %731, align 4
  %1219 = icmp eq i32 %1218, 5
  br i1 %1219, label %1220, label %1243

; <label>:1220:                                   ; preds = %1217
  %1221 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  store %"class.std::__1::basic_string"* %1221, %"class.std::__1::basic_string"** %653, align 8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.3, i32 0, i32 0), i8** %654, align 8
  %1222 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %653, align 8
  %1223 = load i8*, i8** %654, align 8
  store %"class.std::__1::basic_string"* %1222, %"class.std::__1::basic_string"** %651, align 8
  store i8* %1223, i8** %652, align 8
  %1224 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %651, align 8
  %1225 = load i8*, i8** %652, align 8
  %1226 = call i32 @_ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7compareEPKc(%"class.std::__1::basic_string"* %1224, i8* %1225) #8
  %1227 = icmp eq i32 %1226, 0
  %1228 = xor i1 %1227, true
  br i1 %1228, label %1229, label %1238

; <label>:1229:                                   ; preds = %1220
  %1230 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  store %"class.std::__1::basic_string"* %1230, %"class.std::__1::basic_string"** %649, align 8
  store i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.4, i32 0, i32 0), i8** %650, align 8
  %1231 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %649, align 8
  %1232 = load i8*, i8** %650, align 8
  store %"class.std::__1::basic_string"* %1231, %"class.std::__1::basic_string"** %647, align 8
  store i8* %1232, i8** %648, align 8
  %1233 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %647, align 8
  %1234 = load i8*, i8** %648, align 8
  %1235 = call i32 @_ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7compareEPKc(%"class.std::__1::basic_string"* %1233, i8* %1234) #8
  %1236 = icmp eq i32 %1235, 0
  %1237 = xor i1 %1236, true
  br label %1238

; <label>:1238:                                   ; preds = %1229, %1220
  %1239 = phi i1 [ false, %1220 ], [ %1237, %1229 ]
  %1240 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1241 = bitcast %union.anon* %1240 to i8*
  %1242 = zext i1 %1239 to i8
  store i8 %1242, i8* %1241, align 8
  br label %1474

; <label>:1243:                                   ; preds = %1217
  %1244 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %1245 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %1244, i32 0, i32 0
  %1246 = load i32, i32* %1245, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %751, i32 %1246)
  store i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str.5, i32 0, i32 0), i8** %645, align 8, !noalias !17
  store %"class.std::__1::basic_string"* %751, %"class.std::__1::basic_string"** %646, align 8, !noalias !17
  %1247 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %646, align 8, !noalias !17
  %1248 = load i8*, i8** %645, align 8, !noalias !17
  %1249 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %1247, i64 0, i8* %1248)
          to label %1250 unwind label %1450

; <label>:1250:                                   ; preds = %1243
  store %"class.std::__1::basic_string"* %1249, %"class.std::__1::basic_string"** %644, align 8, !noalias !17
  %1251 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %644, align 8, !noalias !17
  store %"class.std::__1::basic_string"* %750, %"class.std::__1::basic_string"** %642, align 8, !noalias !17
  store %"class.std::__1::basic_string"* %1251, %"class.std::__1::basic_string"** %643, align 8, !noalias !17
  %1252 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %642, align 8, !noalias !17
  %1253 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %643, align 8, !noalias !17
  store %"class.std::__1::basic_string"* %1252, %"class.std::__1::basic_string"** %640, align 8, !noalias !17
  store %"class.std::__1::basic_string"* %1253, %"class.std::__1::basic_string"** %641, align 8, !noalias !17
  %1254 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %640, align 8, !noalias !17
  %1255 = bitcast %"class.std::__1::basic_string"* %1254 to %"class.std::__1::__basic_string_common"*
  %1256 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1254, i32 0, i32 0
  %1257 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %641, align 8, !noalias !17
  %1258 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1257, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1258, %"class.std::__1::__compressed_pair"** %639, align 8, !noalias !17
  %1259 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %639, align 8, !noalias !17
  %1260 = bitcast %"class.std::__1::__compressed_pair"* %1256 to i8*
  %1261 = bitcast %"class.std::__1::__compressed_pair"* %1259 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1260, i8* %1261, i64 24, i32 8, i1 false) #8
  %1262 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %641, align 8, !noalias !17
  store %"class.std::__1::basic_string"* %1262, %"class.std::__1::basic_string"** %636, align 8, !noalias !17
  %1263 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %636, align 8, !noalias !17
  %1264 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1263, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1264, %"class.std::__1::__compressed_pair"** %635, align 8, !noalias !17
  %1265 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %635, align 8, !noalias !17
  %1266 = bitcast %"class.std::__1::__compressed_pair"* %1265 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1266, %"class.std::__1::__libcpp_compressed_pair_imp"** %634, align 8, !noalias !17
  %1267 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %634, align 8, !noalias !17
  %1268 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1267, i32 0, i32 0
  %1269 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1268, i32 0, i32 0
  %1270 = bitcast %union.anon.0* %1269 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %1271 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %1270, i32 0, i32 0
  store [3 x i64]* %1271, [3 x i64]** %637, align 8, !noalias !17
  store i32 0, i32* %638, align 4, !noalias !17
  br label %1272

; <label>:1272:                                   ; preds = %1275, %1250
  %1273 = load i32, i32* %638, align 4, !noalias !17
  %1274 = icmp ult i32 %1273, 3
  br i1 %1274, label %1275, label %1282

; <label>:1275:                                   ; preds = %1272
  %1276 = load i32, i32* %638, align 4, !noalias !17
  %1277 = zext i32 %1276 to i64
  %1278 = load [3 x i64]*, [3 x i64]** %637, align 8, !noalias !17
  %1279 = getelementptr inbounds [3 x i64], [3 x i64]* %1278, i64 0, i64 %1277
  store i64 0, i64* %1279, align 8
  %1280 = load i32, i32* %638, align 4, !noalias !17
  %1281 = add i32 %1280, 1
  store i32 %1281, i32* %638, align 4, !noalias !17
  br label %1272

; <label>:1282:                                   ; preds = %1272
  br label %1283

; <label>:1283:                                   ; preds = %1282
  store %"class.std::__1::basic_string"* %750, %"class.std::__1::basic_string"** %632, align 8, !noalias !20
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.6, i32 0, i32 0), i8** %633, align 8, !noalias !20
  %1284 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %632, align 8, !noalias !20
  %1285 = load i8*, i8** %633, align 8, !noalias !20
  %1286 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %1284, i8* %1285)
          to label %1287 unwind label %1454

; <label>:1287:                                   ; preds = %1283
  store %"class.std::__1::basic_string"* %1286, %"class.std::__1::basic_string"** %631, align 8, !noalias !20
  %1288 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %631, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %749, %"class.std::__1::basic_string"** %629, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %1288, %"class.std::__1::basic_string"** %630, align 8, !noalias !20
  %1289 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %629, align 8, !noalias !20
  %1290 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %630, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %1289, %"class.std::__1::basic_string"** %627, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %1290, %"class.std::__1::basic_string"** %628, align 8, !noalias !20
  %1291 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %627, align 8, !noalias !20
  %1292 = bitcast %"class.std::__1::basic_string"* %1291 to %"class.std::__1::__basic_string_common"*
  %1293 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1291, i32 0, i32 0
  %1294 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %628, align 8, !noalias !20
  %1295 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1294, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1295, %"class.std::__1::__compressed_pair"** %626, align 8, !noalias !20
  %1296 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %626, align 8, !noalias !20
  %1297 = bitcast %"class.std::__1::__compressed_pair"* %1293 to i8*
  %1298 = bitcast %"class.std::__1::__compressed_pair"* %1296 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1297, i8* %1298, i64 24, i32 8, i1 false) #8
  %1299 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %628, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %1299, %"class.std::__1::basic_string"** %623, align 8, !noalias !20
  %1300 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %623, align 8, !noalias !20
  %1301 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1300, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1301, %"class.std::__1::__compressed_pair"** %622, align 8, !noalias !20
  %1302 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %622, align 8, !noalias !20
  %1303 = bitcast %"class.std::__1::__compressed_pair"* %1302 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1303, %"class.std::__1::__libcpp_compressed_pair_imp"** %621, align 8, !noalias !20
  %1304 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %621, align 8, !noalias !20
  %1305 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1304, i32 0, i32 0
  %1306 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1305, i32 0, i32 0
  %1307 = bitcast %union.anon.0* %1306 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %1308 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %1307, i32 0, i32 0
  store [3 x i64]* %1308, [3 x i64]** %624, align 8, !noalias !20
  store i32 0, i32* %625, align 4, !noalias !20
  br label %1309

; <label>:1309:                                   ; preds = %1312, %1287
  %1310 = load i32, i32* %625, align 4, !noalias !20
  %1311 = icmp ult i32 %1310, 3
  br i1 %1311, label %1312, label %1319

; <label>:1312:                                   ; preds = %1309
  %1313 = load i32, i32* %625, align 4, !noalias !20
  %1314 = zext i32 %1313 to i64
  %1315 = load [3 x i64]*, [3 x i64]** %624, align 8, !noalias !20
  %1316 = getelementptr inbounds [3 x i64], [3 x i64]* %1315, i64 0, i64 %1314
  store i64 0, i64* %1316, align 8
  %1317 = load i32, i32* %625, align 4, !noalias !20
  %1318 = add i32 %1317, 1
  store i32 %1318, i32* %625, align 4, !noalias !20
  br label %1309

; <label>:1319:                                   ; preds = %1309
  br label %1320

; <label>:1320:                                   ; preds = %1319
  %1321 = load i32, i32* %731, align 4
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %752, i32 %1321)
          to label %1322 unwind label %1458

; <label>:1322:                                   ; preds = %1320
  store %"class.std::__1::basic_string"* %749, %"class.std::__1::basic_string"** %619, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %752, %"class.std::__1::basic_string"** %620, align 8, !noalias !23
  %1323 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %619, align 8, !noalias !23
  %1324 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %620, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %1323, %"class.std::__1::basic_string"** %617, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %1324, %"class.std::__1::basic_string"** %618, align 8, !noalias !23
  %1325 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %617, align 8, !noalias !23
  %1326 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %618, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %1326, %"class.std::__1::basic_string"** %616, align 8, !noalias !23
  %1327 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %616, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %1327, %"class.std::__1::basic_string"** %615, align 8, !noalias !23
  %1328 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %615, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %1328, %"class.std::__1::basic_string"** %614, align 8, !noalias !23
  %1329 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %614, align 8, !noalias !23
  %1330 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1329, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1330, %"class.std::__1::__compressed_pair"** %613, align 8, !noalias !23
  %1331 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %613, align 8, !noalias !23
  %1332 = bitcast %"class.std::__1::__compressed_pair"* %1331 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1332, %"class.std::__1::__libcpp_compressed_pair_imp"** %612, align 8, !noalias !23
  %1333 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %612, align 8, !noalias !23
  %1334 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1333, i32 0, i32 0
  %1335 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1334, i32 0, i32 0
  %1336 = bitcast %union.anon.0* %1335 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1337 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1336, i32 0, i32 0
  %1338 = bitcast %union.anon.1* %1337 to i8*
  %1339 = load i8, i8* %1338, align 8, !noalias !23
  %1340 = zext i8 %1339 to i32
  %1341 = and i32 %1340, 1
  %1342 = icmp ne i32 %1341, 0
  br i1 %1342, label %1343, label %1354

; <label>:1343:                                   ; preds = %1322
  store %"class.std::__1::basic_string"* %1328, %"class.std::__1::basic_string"** %606, align 8, !noalias !23
  %1344 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %606, align 8, !noalias !23
  %1345 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1344, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1345, %"class.std::__1::__compressed_pair"** %605, align 8, !noalias !23
  %1346 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %605, align 8, !noalias !23
  %1347 = bitcast %"class.std::__1::__compressed_pair"* %1346 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1347, %"class.std::__1::__libcpp_compressed_pair_imp"** %604, align 8, !noalias !23
  %1348 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %604, align 8, !noalias !23
  %1349 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1348, i32 0, i32 0
  %1350 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1349, i32 0, i32 0
  %1351 = bitcast %union.anon.0* %1350 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1352 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1351, i32 0, i32 2
  %1353 = load i8*, i8** %1352, align 8
  br label %1367

; <label>:1354:                                   ; preds = %1322
  store %"class.std::__1::basic_string"* %1328, %"class.std::__1::basic_string"** %611, align 8, !noalias !23
  %1355 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %611, align 8, !noalias !23
  %1356 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1355, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1356, %"class.std::__1::__compressed_pair"** %610, align 8, !noalias !23
  %1357 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %610, align 8, !noalias !23
  %1358 = bitcast %"class.std::__1::__compressed_pair"* %1357 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1358, %"class.std::__1::__libcpp_compressed_pair_imp"** %609, align 8, !noalias !23
  %1359 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %609, align 8, !noalias !23
  %1360 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1359, i32 0, i32 0
  %1361 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1360, i32 0, i32 0
  %1362 = bitcast %union.anon.0* %1361 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1363 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1362, i32 0, i32 1
  %1364 = getelementptr inbounds [23 x i8], [23 x i8]* %1363, i64 0, i64 0
  store i8* %1364, i8** %608, align 8, !noalias !23
  %1365 = load i8*, i8** %608, align 8, !noalias !23
  store i8* %1365, i8** %607, align 8, !noalias !23
  %1366 = load i8*, i8** %607, align 8, !noalias !23
  br label %1367

; <label>:1367:                                   ; preds = %1354, %1343
  %1368 = phi i8* [ %1353, %1343 ], [ %1366, %1354 ]
  store i8* %1368, i8** %603, align 8, !noalias !23
  %1369 = load i8*, i8** %603, align 8, !noalias !23
  %1370 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %618, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %1370, %"class.std::__1::basic_string"** %602, align 8, !noalias !23
  %1371 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %602, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %1371, %"class.std::__1::basic_string"** %601, align 8, !noalias !23
  %1372 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %601, align 8, !noalias !23
  %1373 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1372, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1373, %"class.std::__1::__compressed_pair"** %600, align 8, !noalias !23
  %1374 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %600, align 8, !noalias !23
  %1375 = bitcast %"class.std::__1::__compressed_pair"* %1374 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1375, %"class.std::__1::__libcpp_compressed_pair_imp"** %599, align 8, !noalias !23
  %1376 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %599, align 8, !noalias !23
  %1377 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1376, i32 0, i32 0
  %1378 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1377, i32 0, i32 0
  %1379 = bitcast %union.anon.0* %1378 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1380 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1379, i32 0, i32 0
  %1381 = bitcast %union.anon.1* %1380 to i8*
  %1382 = load i8, i8* %1381, align 8, !noalias !23
  %1383 = zext i8 %1382 to i32
  %1384 = and i32 %1383, 1
  %1385 = icmp ne i32 %1384, 0
  br i1 %1385, label %1386, label %1397

; <label>:1386:                                   ; preds = %1367
  store %"class.std::__1::basic_string"* %1371, %"class.std::__1::basic_string"** %595, align 8, !noalias !23
  %1387 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %595, align 8, !noalias !23
  %1388 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1387, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1388, %"class.std::__1::__compressed_pair"** %594, align 8, !noalias !23
  %1389 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %594, align 8, !noalias !23
  %1390 = bitcast %"class.std::__1::__compressed_pair"* %1389 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1390, %"class.std::__1::__libcpp_compressed_pair_imp"** %593, align 8, !noalias !23
  %1391 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %593, align 8, !noalias !23
  %1392 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1391, i32 0, i32 0
  %1393 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1392, i32 0, i32 0
  %1394 = bitcast %union.anon.0* %1393 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1395 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1394, i32 0, i32 1
  %1396 = load i64, i64* %1395, align 8
  br label %1412

; <label>:1397:                                   ; preds = %1367
  store %"class.std::__1::basic_string"* %1371, %"class.std::__1::basic_string"** %598, align 8, !noalias !23
  %1398 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %598, align 8, !noalias !23
  %1399 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1398, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1399, %"class.std::__1::__compressed_pair"** %597, align 8, !noalias !23
  %1400 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %597, align 8, !noalias !23
  %1401 = bitcast %"class.std::__1::__compressed_pair"* %1400 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1401, %"class.std::__1::__libcpp_compressed_pair_imp"** %596, align 8, !noalias !23
  %1402 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %596, align 8, !noalias !23
  %1403 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1402, i32 0, i32 0
  %1404 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1403, i32 0, i32 0
  %1405 = bitcast %union.anon.0* %1404 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1406 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1405, i32 0, i32 0
  %1407 = bitcast %union.anon.1* %1406 to i8*
  %1408 = load i8, i8* %1407, align 8
  %1409 = zext i8 %1408 to i32
  %1410 = ashr i32 %1409, 1
  %1411 = sext i32 %1410 to i64
  br label %1412

; <label>:1412:                                   ; preds = %1397, %1386
  %1413 = phi i64 [ %1396, %1386 ], [ %1411, %1397 ]
  %1414 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %1325, i8* %1369, i64 %1413)
          to label %1415 unwind label %1462

; <label>:1415:                                   ; preds = %1412
  store %"class.std::__1::basic_string"* %1414, %"class.std::__1::basic_string"** %592, align 8, !noalias !23
  %1416 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %592, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %748, %"class.std::__1::basic_string"** %590, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %1416, %"class.std::__1::basic_string"** %591, align 8, !noalias !23
  %1417 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %590, align 8, !noalias !23
  %1418 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %591, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %1417, %"class.std::__1::basic_string"** %588, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %1418, %"class.std::__1::basic_string"** %589, align 8, !noalias !23
  %1419 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %588, align 8, !noalias !23
  %1420 = bitcast %"class.std::__1::basic_string"* %1419 to %"class.std::__1::__basic_string_common"*
  %1421 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1419, i32 0, i32 0
  %1422 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %589, align 8, !noalias !23
  %1423 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1422, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1423, %"class.std::__1::__compressed_pair"** %587, align 8, !noalias !23
  %1424 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %587, align 8, !noalias !23
  %1425 = bitcast %"class.std::__1::__compressed_pair"* %1421 to i8*
  %1426 = bitcast %"class.std::__1::__compressed_pair"* %1424 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1425, i8* %1426, i64 24, i32 8, i1 false) #8
  %1427 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %589, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %1427, %"class.std::__1::basic_string"** %584, align 8, !noalias !23
  %1428 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %584, align 8, !noalias !23
  %1429 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1428, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1429, %"class.std::__1::__compressed_pair"** %583, align 8, !noalias !23
  %1430 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %583, align 8, !noalias !23
  %1431 = bitcast %"class.std::__1::__compressed_pair"* %1430 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1431, %"class.std::__1::__libcpp_compressed_pair_imp"** %582, align 8, !noalias !23
  %1432 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %582, align 8, !noalias !23
  %1433 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1432, i32 0, i32 0
  %1434 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1433, i32 0, i32 0
  %1435 = bitcast %union.anon.0* %1434 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %1436 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %1435, i32 0, i32 0
  store [3 x i64]* %1436, [3 x i64]** %585, align 8, !noalias !23
  store i32 0, i32* %586, align 4, !noalias !23
  br label %1437

; <label>:1437:                                   ; preds = %1440, %1415
  %1438 = load i32, i32* %586, align 4, !noalias !23
  %1439 = icmp ult i32 %1438, 3
  br i1 %1439, label %1440, label %1447

; <label>:1440:                                   ; preds = %1437
  %1441 = load i32, i32* %586, align 4, !noalias !23
  %1442 = zext i32 %1441 to i64
  %1443 = load [3 x i64]*, [3 x i64]** %585, align 8, !noalias !23
  %1444 = getelementptr inbounds [3 x i64], [3 x i64]* %1443, i64 0, i64 %1442
  store i64 0, i64* %1444, align 8
  %1445 = load i32, i32* %586, align 4, !noalias !23
  %1446 = add i32 %1445, 1
  store i32 %1446, i32* %586, align 4, !noalias !23
  br label %1437

; <label>:1447:                                   ; preds = %1437
  br label %1448

; <label>:1448:                                   ; preds = %1447
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %748, %class.AstNode* null)
          to label %1449 unwind label %1466

; <label>:1449:                                   ; preds = %1448
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %748) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %752) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %749) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %750) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %751) #8
  br label %1474

; <label>:1450:                                   ; preds = %1243
  %1451 = landingpad { i8*, i32 }
          cleanup
  %1452 = extractvalue { i8*, i32 } %1451, 0
  store i8* %1452, i8** %734, align 8
  %1453 = extractvalue { i8*, i32 } %1451, 1
  store i32 %1453, i32* %735, align 4
  br label %1473

; <label>:1454:                                   ; preds = %1283
  %1455 = landingpad { i8*, i32 }
          cleanup
  %1456 = extractvalue { i8*, i32 } %1455, 0
  store i8* %1456, i8** %734, align 8
  %1457 = extractvalue { i8*, i32 } %1455, 1
  store i32 %1457, i32* %735, align 4
  br label %1472

; <label>:1458:                                   ; preds = %1320
  %1459 = landingpad { i8*, i32 }
          cleanup
  %1460 = extractvalue { i8*, i32 } %1459, 0
  store i8* %1460, i8** %734, align 8
  %1461 = extractvalue { i8*, i32 } %1459, 1
  store i32 %1461, i32* %735, align 4
  br label %1471

; <label>:1462:                                   ; preds = %1412
  %1463 = landingpad { i8*, i32 }
          cleanup
  %1464 = extractvalue { i8*, i32 } %1463, 0
  store i8* %1464, i8** %734, align 8
  %1465 = extractvalue { i8*, i32 } %1463, 1
  store i32 %1465, i32* %735, align 4
  br label %1470

; <label>:1466:                                   ; preds = %1448
  %1467 = landingpad { i8*, i32 }
          cleanup
  %1468 = extractvalue { i8*, i32 } %1467, 0
  store i8* %1468, i8** %734, align 8
  %1469 = extractvalue { i8*, i32 } %1467, 1
  store i32 %1469, i32* %735, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %748) #8
  br label %1470

; <label>:1470:                                   ; preds = %1466, %1462
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %752) #8
  br label %1471

; <label>:1471:                                   ; preds = %1470, %1458
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %749) #8
  br label %1472

; <label>:1472:                                   ; preds = %1471, %1454
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %750) #8
  br label %1473

; <label>:1473:                                   ; preds = %1472, %1450
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %751) #8
  br label %3827

; <label>:1474:                                   ; preds = %1449, %1238
  br label %1475

; <label>:1475:                                   ; preds = %1474, %1201
  br label %1476

; <label>:1476:                                   ; preds = %1475, %1123
  br label %1477

; <label>:1477:                                   ; preds = %1476, %1045
  br label %1478

; <label>:1478:                                   ; preds = %1477, %967
  br label %1479

; <label>:1479:                                   ; preds = %1478, %899
  br label %3798

; <label>:1480:                                   ; preds = %796
  %1481 = load i32, i32* %731, align 4
  %1482 = icmp eq i32 %1481, 4
  br i1 %1482, label %1483, label %1630

; <label>:1483:                                   ; preds = %1480
  %1484 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %1485 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %1484, i32 0, i32 7
  store i64 1, i64* %1485, align 8
  %1486 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  %1487 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1488 = bitcast %union.anon* %1487 to i8*
  %1489 = load i8, i8* %1488, align 8
  store %"class.std::__1::basic_string"* %753, %"class.std::__1::basic_string"** %579, align 8
  store i64 1, i64* %580, align 8
  store i8 %1489, i8* %581, align 1
  %1490 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %579, align 8
  %1491 = load i64, i64* %580, align 8
  %1492 = load i8, i8* %581, align 1
  store %"class.std::__1::basic_string"* %1490, %"class.std::__1::basic_string"** %576, align 8
  store i64 %1491, i64* %577, align 8
  store i8 %1492, i8* %578, align 1
  %1493 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %576, align 8
  %1494 = bitcast %"class.std::__1::basic_string"* %1493 to %"class.std::__1::__basic_string_common"*
  %1495 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1493, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1495, %"class.std::__1::__compressed_pair"** %575, align 8
  %1496 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %575, align 8
  store %"class.std::__1::__compressed_pair"* %1496, %"class.std::__1::__compressed_pair"** %574, align 8
  %1497 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %574, align 8
  %1498 = bitcast %"class.std::__1::__compressed_pair"* %1497 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1498, %"class.std::__1::__libcpp_compressed_pair_imp"** %573, align 8
  %1499 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %573, align 8
  %1500 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %1499 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %1500, %"class.std::__1::allocator"** %572, align 8
  %1501 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %572, align 8
  %1502 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1499, i32 0, i32 0
  %1503 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1502 to i8*
  call void @llvm.memset.p0i8.i64(i8* %1503, i8 0, i64 24, i32 8, i1 false) #8
  %1504 = load i64, i64* %577, align 8
  %1505 = load i8, i8* %578, align 1
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEmc(%"class.std::__1::basic_string"* %1493, i64 %1504, i8 signext %1505)
  store %"class.std::__1::basic_string"* %1486, %"class.std::__1::basic_string"** %569, align 8
  store %"class.std::__1::basic_string"* %753, %"class.std::__1::basic_string"** %570, align 8
  %1506 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %569, align 8
  %1507 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %570, align 8
  store %"class.std::__1::basic_string"* %1506, %"class.std::__1::basic_string"** %567, align 8
  store %"class.std::__1::basic_string"* %1507, %"class.std::__1::basic_string"** %568, align 8
  %1508 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %567, align 8
  store %"class.std::__1::basic_string"* %1508, %"class.std::__1::basic_string"** %563, align 8
  %1509 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %563, align 8
  store %"class.std::__1::basic_string"* %1509, %"class.std::__1::basic_string"** %562, align 8
  %1510 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %562, align 8
  store %"class.std::__1::basic_string"* %1509, %"class.std::__1::basic_string"** %561, align 8
  %1511 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %561, align 8
  %1512 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1511, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1512, %"class.std::__1::__compressed_pair"** %560, align 8
  %1513 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %560, align 8
  %1514 = bitcast %"class.std::__1::__compressed_pair"* %1513 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1514, %"class.std::__1::__libcpp_compressed_pair_imp"** %559, align 8
  %1515 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %559, align 8
  %1516 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1515, i32 0, i32 0
  %1517 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1516, i32 0, i32 0
  %1518 = bitcast %union.anon.0* %1517 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1519 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1518, i32 0, i32 0
  %1520 = bitcast %union.anon.1* %1519 to i8*
  %1521 = load i8, i8* %1520, align 8
  %1522 = zext i8 %1521 to i32
  %1523 = and i32 %1522, 1
  %1524 = icmp ne i32 %1523, 0
  br i1 %1524, label %1525, label %1546

; <label>:1525:                                   ; preds = %1483
  store %"class.std::__1::basic_string"* %1509, %"class.std::__1::basic_string"** %554, align 8
  %1526 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %554, align 8
  %1527 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1526, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1527, %"class.std::__1::__compressed_pair"** %553, align 8
  %1528 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %553, align 8
  %1529 = bitcast %"class.std::__1::__compressed_pair"* %1528 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1529, %"class.std::__1::__libcpp_compressed_pair_imp"** %552, align 8
  %1530 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %552, align 8
  %1531 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1530, i32 0, i32 0
  %1532 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1531, i32 0, i32 0
  %1533 = bitcast %union.anon.0* %1532 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1534 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1533, i32 0, i32 2
  %1535 = load i8*, i8** %1534, align 8
  store i8 0, i8* %564, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %1535, i8* dereferenceable(1) %564) #8
  store %"class.std::__1::basic_string"* %1509, %"class.std::__1::basic_string"** %545, align 8
  store i64 0, i64* %546, align 8
  %1536 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %545, align 8
  %1537 = load i64, i64* %546, align 8
  %1538 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1536, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1538, %"class.std::__1::__compressed_pair"** %544, align 8
  %1539 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %544, align 8
  %1540 = bitcast %"class.std::__1::__compressed_pair"* %1539 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1540, %"class.std::__1::__libcpp_compressed_pair_imp"** %543, align 8
  %1541 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %543, align 8
  %1542 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1541, i32 0, i32 0
  %1543 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1542, i32 0, i32 0
  %1544 = bitcast %union.anon.0* %1543 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1545 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1544, i32 0, i32 1
  store i64 %1537, i64* %1545, align 8
  br label %1572

; <label>:1546:                                   ; preds = %1483
  store %"class.std::__1::basic_string"* %1509, %"class.std::__1::basic_string"** %551, align 8
  %1547 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %551, align 8
  %1548 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1547, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1548, %"class.std::__1::__compressed_pair"** %550, align 8
  %1549 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %550, align 8
  %1550 = bitcast %"class.std::__1::__compressed_pair"* %1549 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1550, %"class.std::__1::__libcpp_compressed_pair_imp"** %549, align 8
  %1551 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %549, align 8
  %1552 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1551, i32 0, i32 0
  %1553 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1552, i32 0, i32 0
  %1554 = bitcast %union.anon.0* %1553 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1555 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1554, i32 0, i32 1
  %1556 = getelementptr inbounds [23 x i8], [23 x i8]* %1555, i64 0, i64 0
  store i8* %1556, i8** %548, align 8
  %1557 = load i8*, i8** %548, align 8
  store i8* %1557, i8** %547, align 8
  %1558 = load i8*, i8** %547, align 8
  store i8 0, i8* %565, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %1558, i8* dereferenceable(1) %565) #8
  store %"class.std::__1::basic_string"* %1509, %"class.std::__1::basic_string"** %557, align 8
  store i64 0, i64* %558, align 8
  %1559 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %557, align 8
  %1560 = load i64, i64* %558, align 8
  %1561 = shl i64 %1560, 1
  %1562 = trunc i64 %1561 to i8
  %1563 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1559, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1563, %"class.std::__1::__compressed_pair"** %556, align 8
  %1564 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %556, align 8
  %1565 = bitcast %"class.std::__1::__compressed_pair"* %1564 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1565, %"class.std::__1::__libcpp_compressed_pair_imp"** %555, align 8
  %1566 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %555, align 8
  %1567 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1566, i32 0, i32 0
  %1568 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1567, i32 0, i32 0
  %1569 = bitcast %union.anon.0* %1568 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1570 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1569, i32 0, i32 0
  %1571 = bitcast %union.anon.1* %1570 to i8*
  store i8 %1562, i8* %1571, align 8
  br label %1572

; <label>:1572:                                   ; preds = %1546, %1525
  store %"class.std::__1::basic_string"* %1508, %"class.std::__1::basic_string"** %542, align 8
  %1573 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %542, align 8
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7reserveEm(%"class.std::__1::basic_string"* %1573, i64 0)
          to label %1577 unwind label %1574

; <label>:1574:                                   ; preds = %1572
  %1575 = landingpad { i8*, i32 }
          catch i8* null
  %1576 = extractvalue { i8*, i32 } %1575, 0
  call void @__clang_call_terminate(i8* %1576) #9
  unreachable

; <label>:1577:                                   ; preds = %1572
  %1578 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1508, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1578, %"class.std::__1::__compressed_pair"** %541, align 8
  %1579 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %541, align 8
  %1580 = bitcast %"class.std::__1::__compressed_pair"* %1579 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1580, %"class.std::__1::__libcpp_compressed_pair_imp"** %540, align 8
  %1581 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %540, align 8
  %1582 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1581, i32 0, i32 0
  %1583 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %568, align 8
  %1584 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1583, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1584, %"class.std::__1::__compressed_pair"** %539, align 8
  %1585 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %539, align 8
  %1586 = bitcast %"class.std::__1::__compressed_pair"* %1585 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1586, %"class.std::__1::__libcpp_compressed_pair_imp"** %538, align 8
  %1587 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %538, align 8
  %1588 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1587, i32 0, i32 0
  %1589 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1582 to i8*
  %1590 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1588 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1589, i8* %1590, i64 24, i32 8, i1 false) #8
  %1591 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %568, align 8
  store %"class.std::__1::basic_string"* %1508, %"class.std::__1::basic_string"** %535, align 8
  store %"class.std::__1::basic_string"* %1591, %"class.std::__1::basic_string"** %536, align 8
  %1592 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %535, align 8
  %1593 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %536, align 8
  store %"class.std::__1::basic_string"* %1592, %"class.std::__1::basic_string"** %533, align 8
  store %"class.std::__1::basic_string"* %1593, %"class.std::__1::basic_string"** %534, align 8
  %1594 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %533, align 8
  store %"class.std::__1::basic_string"* %1594, %"class.std::__1::basic_string"** %531, align 8
  %1595 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %531, align 8
  %1596 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1595, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1596, %"class.std::__1::__compressed_pair"** %530, align 8
  %1597 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %530, align 8
  %1598 = bitcast %"class.std::__1::__compressed_pair"* %1597 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1598, %"class.std::__1::__libcpp_compressed_pair_imp"** %529, align 8
  %1599 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %529, align 8
  %1600 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %1599 to %"class.std::__1::allocator"*
  %1601 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %534, align 8
  store %"class.std::__1::basic_string"* %1601, %"class.std::__1::basic_string"** %527, align 8
  %1602 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %527, align 8
  %1603 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1602, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1603, %"class.std::__1::__compressed_pair"** %526, align 8
  %1604 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %526, align 8
  %1605 = bitcast %"class.std::__1::__compressed_pair"* %1604 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1605, %"class.std::__1::__libcpp_compressed_pair_imp"** %525, align 8
  %1606 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %525, align 8
  %1607 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %1606 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %1607, %"class.std::__1::allocator"** %528, align 8
  %1608 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %528, align 8
  %1609 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %568, align 8
  store %"class.std::__1::basic_string"* %1609, %"class.std::__1::basic_string"** %522, align 8
  %1610 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %522, align 8
  %1611 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1610, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1611, %"class.std::__1::__compressed_pair"** %521, align 8
  %1612 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %521, align 8
  %1613 = bitcast %"class.std::__1::__compressed_pair"* %1612 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1613, %"class.std::__1::__libcpp_compressed_pair_imp"** %520, align 8
  %1614 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %520, align 8
  %1615 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1614, i32 0, i32 0
  %1616 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1615, i32 0, i32 0
  %1617 = bitcast %union.anon.0* %1616 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %1618 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %1617, i32 0, i32 0
  store [3 x i64]* %1618, [3 x i64]** %523, align 8
  store i32 0, i32* %524, align 4
  br label %1619

; <label>:1619:                                   ; preds = %1622, %1577
  %1620 = load i32, i32* %524, align 4
  %1621 = icmp ult i32 %1620, 3
  br i1 %1621, label %1622, label %1629

; <label>:1622:                                   ; preds = %1619
  %1623 = load i32, i32* %524, align 4
  %1624 = zext i32 %1623 to i64
  %1625 = load [3 x i64]*, [3 x i64]** %523, align 8
  %1626 = getelementptr inbounds [3 x i64], [3 x i64]* %1625, i64 0, i64 %1624
  store i64 0, i64* %1626, align 8
  %1627 = load i32, i32* %524, align 4
  %1628 = add i32 %1627, 1
  store i32 %1628, i32* %524, align 4
  br label %1619

; <label>:1629:                                   ; preds = %1619
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %753) #8
  br label %1918

; <label>:1630:                                   ; preds = %1480
  %1631 = load i32, i32* %731, align 4
  %1632 = icmp eq i32 %1631, 0
  br i1 %1632, label %1633, label %1640

; <label>:1633:                                   ; preds = %1630
  %1634 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1635 = bitcast %union.anon* %1634 to i8*
  %1636 = load i8, i8* %1635, align 8
  %1637 = sext i8 %1636 to i32
  %1638 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1639 = bitcast %union.anon* %1638 to i32*
  store i32 %1637, i32* %1639, align 8
  br label %1917

; <label>:1640:                                   ; preds = %1630
  %1641 = load i32, i32* %731, align 4
  %1642 = icmp eq i32 %1641, 1
  br i1 %1642, label %1643, label %1650

; <label>:1643:                                   ; preds = %1640
  %1644 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1645 = bitcast %union.anon* %1644 to i8*
  %1646 = load i8, i8* %1645, align 8
  %1647 = sext i8 %1646 to i64
  %1648 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1649 = bitcast %union.anon* %1648 to i64*
  store i64 %1647, i64* %1649, align 8
  br label %1916

; <label>:1650:                                   ; preds = %1640
  %1651 = load i32, i32* %731, align 4
  %1652 = icmp eq i32 %1651, 2
  br i1 %1652, label %1653, label %1660

; <label>:1653:                                   ; preds = %1650
  %1654 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1655 = bitcast %union.anon* %1654 to i8*
  %1656 = load i8, i8* %1655, align 8
  %1657 = sitofp i8 %1656 to float
  %1658 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1659 = bitcast %union.anon* %1658 to float*
  store float %1657, float* %1659, align 8
  br label %1915

; <label>:1660:                                   ; preds = %1650
  %1661 = load i32, i32* %731, align 4
  %1662 = icmp eq i32 %1661, 3
  br i1 %1662, label %1663, label %1670

; <label>:1663:                                   ; preds = %1660
  %1664 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1665 = bitcast %union.anon* %1664 to i8*
  %1666 = load i8, i8* %1665, align 8
  %1667 = sitofp i8 %1666 to double
  %1668 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1669 = bitcast %union.anon* %1668 to double*
  store double %1667, double* %1669, align 8
  br label %1914

; <label>:1670:                                   ; preds = %1660
  %1671 = load i32, i32* %731, align 4
  %1672 = icmp eq i32 %1671, 5
  br i1 %1672, label %1673, label %1682

; <label>:1673:                                   ; preds = %1670
  %1674 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1675 = bitcast %union.anon* %1674 to i8*
  %1676 = load i8, i8* %1675, align 8
  %1677 = sext i8 %1676 to i32
  %1678 = icmp ne i32 %1677, 48
  %1679 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1680 = bitcast %union.anon* %1679 to i8*
  %1681 = zext i1 %1678 to i8
  store i8 %1681, i8* %1680, align 8
  br label %1913

; <label>:1682:                                   ; preds = %1670
  %1683 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %1684 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %1683, i32 0, i32 0
  %1685 = load i32, i32* %1684, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %757, i32 %1685)
  store i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str.5, i32 0, i32 0), i8** %518, align 8, !noalias !26
  store %"class.std::__1::basic_string"* %757, %"class.std::__1::basic_string"** %519, align 8, !noalias !26
  %1686 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %519, align 8, !noalias !26
  %1687 = load i8*, i8** %518, align 8, !noalias !26
  %1688 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %1686, i64 0, i8* %1687)
          to label %1689 unwind label %1889

; <label>:1689:                                   ; preds = %1682
  store %"class.std::__1::basic_string"* %1688, %"class.std::__1::basic_string"** %517, align 8, !noalias !26
  %1690 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %517, align 8, !noalias !26
  store %"class.std::__1::basic_string"* %756, %"class.std::__1::basic_string"** %515, align 8, !noalias !26
  store %"class.std::__1::basic_string"* %1690, %"class.std::__1::basic_string"** %516, align 8, !noalias !26
  %1691 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %515, align 8, !noalias !26
  %1692 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %516, align 8, !noalias !26
  store %"class.std::__1::basic_string"* %1691, %"class.std::__1::basic_string"** %513, align 8, !noalias !26
  store %"class.std::__1::basic_string"* %1692, %"class.std::__1::basic_string"** %514, align 8, !noalias !26
  %1693 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %513, align 8, !noalias !26
  %1694 = bitcast %"class.std::__1::basic_string"* %1693 to %"class.std::__1::__basic_string_common"*
  %1695 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1693, i32 0, i32 0
  %1696 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %514, align 8, !noalias !26
  %1697 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1696, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1697, %"class.std::__1::__compressed_pair"** %512, align 8, !noalias !26
  %1698 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %512, align 8, !noalias !26
  %1699 = bitcast %"class.std::__1::__compressed_pair"* %1695 to i8*
  %1700 = bitcast %"class.std::__1::__compressed_pair"* %1698 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1699, i8* %1700, i64 24, i32 8, i1 false) #8
  %1701 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %514, align 8, !noalias !26
  store %"class.std::__1::basic_string"* %1701, %"class.std::__1::basic_string"** %509, align 8, !noalias !26
  %1702 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %509, align 8, !noalias !26
  %1703 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1702, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1703, %"class.std::__1::__compressed_pair"** %508, align 8, !noalias !26
  %1704 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %508, align 8, !noalias !26
  %1705 = bitcast %"class.std::__1::__compressed_pair"* %1704 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1705, %"class.std::__1::__libcpp_compressed_pair_imp"** %507, align 8, !noalias !26
  %1706 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %507, align 8, !noalias !26
  %1707 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1706, i32 0, i32 0
  %1708 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1707, i32 0, i32 0
  %1709 = bitcast %union.anon.0* %1708 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %1710 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %1709, i32 0, i32 0
  store [3 x i64]* %1710, [3 x i64]** %510, align 8, !noalias !26
  store i32 0, i32* %511, align 4, !noalias !26
  br label %1711

; <label>:1711:                                   ; preds = %1714, %1689
  %1712 = load i32, i32* %511, align 4, !noalias !26
  %1713 = icmp ult i32 %1712, 3
  br i1 %1713, label %1714, label %1721

; <label>:1714:                                   ; preds = %1711
  %1715 = load i32, i32* %511, align 4, !noalias !26
  %1716 = zext i32 %1715 to i64
  %1717 = load [3 x i64]*, [3 x i64]** %510, align 8, !noalias !26
  %1718 = getelementptr inbounds [3 x i64], [3 x i64]* %1717, i64 0, i64 %1716
  store i64 0, i64* %1718, align 8
  %1719 = load i32, i32* %511, align 4, !noalias !26
  %1720 = add i32 %1719, 1
  store i32 %1720, i32* %511, align 4, !noalias !26
  br label %1711

; <label>:1721:                                   ; preds = %1711
  br label %1722

; <label>:1722:                                   ; preds = %1721
  store %"class.std::__1::basic_string"* %756, %"class.std::__1::basic_string"** %505, align 8, !noalias !29
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.6, i32 0, i32 0), i8** %506, align 8, !noalias !29
  %1723 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %505, align 8, !noalias !29
  %1724 = load i8*, i8** %506, align 8, !noalias !29
  %1725 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %1723, i8* %1724)
          to label %1726 unwind label %1893

; <label>:1726:                                   ; preds = %1722
  store %"class.std::__1::basic_string"* %1725, %"class.std::__1::basic_string"** %504, align 8, !noalias !29
  %1727 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %504, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %755, %"class.std::__1::basic_string"** %502, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %1727, %"class.std::__1::basic_string"** %503, align 8, !noalias !29
  %1728 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %502, align 8, !noalias !29
  %1729 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %503, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %1728, %"class.std::__1::basic_string"** %500, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %1729, %"class.std::__1::basic_string"** %501, align 8, !noalias !29
  %1730 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %500, align 8, !noalias !29
  %1731 = bitcast %"class.std::__1::basic_string"* %1730 to %"class.std::__1::__basic_string_common"*
  %1732 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1730, i32 0, i32 0
  %1733 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %501, align 8, !noalias !29
  %1734 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1733, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1734, %"class.std::__1::__compressed_pair"** %499, align 8, !noalias !29
  %1735 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %499, align 8, !noalias !29
  %1736 = bitcast %"class.std::__1::__compressed_pair"* %1732 to i8*
  %1737 = bitcast %"class.std::__1::__compressed_pair"* %1735 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1736, i8* %1737, i64 24, i32 8, i1 false) #8
  %1738 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %501, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %1738, %"class.std::__1::basic_string"** %496, align 8, !noalias !29
  %1739 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %496, align 8, !noalias !29
  %1740 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1739, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1740, %"class.std::__1::__compressed_pair"** %495, align 8, !noalias !29
  %1741 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %495, align 8, !noalias !29
  %1742 = bitcast %"class.std::__1::__compressed_pair"* %1741 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1742, %"class.std::__1::__libcpp_compressed_pair_imp"** %494, align 8, !noalias !29
  %1743 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %494, align 8, !noalias !29
  %1744 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1743, i32 0, i32 0
  %1745 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1744, i32 0, i32 0
  %1746 = bitcast %union.anon.0* %1745 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %1747 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %1746, i32 0, i32 0
  store [3 x i64]* %1747, [3 x i64]** %497, align 8, !noalias !29
  store i32 0, i32* %498, align 4, !noalias !29
  br label %1748

; <label>:1748:                                   ; preds = %1751, %1726
  %1749 = load i32, i32* %498, align 4, !noalias !29
  %1750 = icmp ult i32 %1749, 3
  br i1 %1750, label %1751, label %1758

; <label>:1751:                                   ; preds = %1748
  %1752 = load i32, i32* %498, align 4, !noalias !29
  %1753 = zext i32 %1752 to i64
  %1754 = load [3 x i64]*, [3 x i64]** %497, align 8, !noalias !29
  %1755 = getelementptr inbounds [3 x i64], [3 x i64]* %1754, i64 0, i64 %1753
  store i64 0, i64* %1755, align 8
  %1756 = load i32, i32* %498, align 4, !noalias !29
  %1757 = add i32 %1756, 1
  store i32 %1757, i32* %498, align 4, !noalias !29
  br label %1748

; <label>:1758:                                   ; preds = %1748
  br label %1759

; <label>:1759:                                   ; preds = %1758
  %1760 = load i32, i32* %731, align 4
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %758, i32 %1760)
          to label %1761 unwind label %1897

; <label>:1761:                                   ; preds = %1759
  store %"class.std::__1::basic_string"* %755, %"class.std::__1::basic_string"** %492, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %758, %"class.std::__1::basic_string"** %493, align 8, !noalias !32
  %1762 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %492, align 8, !noalias !32
  %1763 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %493, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %1762, %"class.std::__1::basic_string"** %490, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %1763, %"class.std::__1::basic_string"** %491, align 8, !noalias !32
  %1764 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %490, align 8, !noalias !32
  %1765 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %491, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %1765, %"class.std::__1::basic_string"** %489, align 8, !noalias !32
  %1766 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %489, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %1766, %"class.std::__1::basic_string"** %488, align 8, !noalias !32
  %1767 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %488, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %1767, %"class.std::__1::basic_string"** %487, align 8, !noalias !32
  %1768 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %487, align 8, !noalias !32
  %1769 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1768, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1769, %"class.std::__1::__compressed_pair"** %486, align 8, !noalias !32
  %1770 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %486, align 8, !noalias !32
  %1771 = bitcast %"class.std::__1::__compressed_pair"* %1770 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1771, %"class.std::__1::__libcpp_compressed_pair_imp"** %485, align 8, !noalias !32
  %1772 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %485, align 8, !noalias !32
  %1773 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1772, i32 0, i32 0
  %1774 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1773, i32 0, i32 0
  %1775 = bitcast %union.anon.0* %1774 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1776 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1775, i32 0, i32 0
  %1777 = bitcast %union.anon.1* %1776 to i8*
  %1778 = load i8, i8* %1777, align 8, !noalias !32
  %1779 = zext i8 %1778 to i32
  %1780 = and i32 %1779, 1
  %1781 = icmp ne i32 %1780, 0
  br i1 %1781, label %1782, label %1793

; <label>:1782:                                   ; preds = %1761
  store %"class.std::__1::basic_string"* %1767, %"class.std::__1::basic_string"** %479, align 8, !noalias !32
  %1783 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %479, align 8, !noalias !32
  %1784 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1783, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1784, %"class.std::__1::__compressed_pair"** %478, align 8, !noalias !32
  %1785 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %478, align 8, !noalias !32
  %1786 = bitcast %"class.std::__1::__compressed_pair"* %1785 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1786, %"class.std::__1::__libcpp_compressed_pair_imp"** %477, align 8, !noalias !32
  %1787 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %477, align 8, !noalias !32
  %1788 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1787, i32 0, i32 0
  %1789 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1788, i32 0, i32 0
  %1790 = bitcast %union.anon.0* %1789 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1791 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1790, i32 0, i32 2
  %1792 = load i8*, i8** %1791, align 8
  br label %1806

; <label>:1793:                                   ; preds = %1761
  store %"class.std::__1::basic_string"* %1767, %"class.std::__1::basic_string"** %484, align 8, !noalias !32
  %1794 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %484, align 8, !noalias !32
  %1795 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1794, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1795, %"class.std::__1::__compressed_pair"** %483, align 8, !noalias !32
  %1796 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %483, align 8, !noalias !32
  %1797 = bitcast %"class.std::__1::__compressed_pair"* %1796 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1797, %"class.std::__1::__libcpp_compressed_pair_imp"** %482, align 8, !noalias !32
  %1798 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %482, align 8, !noalias !32
  %1799 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1798, i32 0, i32 0
  %1800 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1799, i32 0, i32 0
  %1801 = bitcast %union.anon.0* %1800 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1802 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1801, i32 0, i32 1
  %1803 = getelementptr inbounds [23 x i8], [23 x i8]* %1802, i64 0, i64 0
  store i8* %1803, i8** %481, align 8, !noalias !32
  %1804 = load i8*, i8** %481, align 8, !noalias !32
  store i8* %1804, i8** %480, align 8, !noalias !32
  %1805 = load i8*, i8** %480, align 8, !noalias !32
  br label %1806

; <label>:1806:                                   ; preds = %1793, %1782
  %1807 = phi i8* [ %1792, %1782 ], [ %1805, %1793 ]
  store i8* %1807, i8** %476, align 8, !noalias !32
  %1808 = load i8*, i8** %476, align 8, !noalias !32
  %1809 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %491, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %1809, %"class.std::__1::basic_string"** %475, align 8, !noalias !32
  %1810 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %475, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %1810, %"class.std::__1::basic_string"** %474, align 8, !noalias !32
  %1811 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %474, align 8, !noalias !32
  %1812 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1811, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1812, %"class.std::__1::__compressed_pair"** %473, align 8, !noalias !32
  %1813 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %473, align 8, !noalias !32
  %1814 = bitcast %"class.std::__1::__compressed_pair"* %1813 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1814, %"class.std::__1::__libcpp_compressed_pair_imp"** %472, align 8, !noalias !32
  %1815 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %472, align 8, !noalias !32
  %1816 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1815, i32 0, i32 0
  %1817 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1816, i32 0, i32 0
  %1818 = bitcast %union.anon.0* %1817 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1819 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1818, i32 0, i32 0
  %1820 = bitcast %union.anon.1* %1819 to i8*
  %1821 = load i8, i8* %1820, align 8, !noalias !32
  %1822 = zext i8 %1821 to i32
  %1823 = and i32 %1822, 1
  %1824 = icmp ne i32 %1823, 0
  br i1 %1824, label %1825, label %1836

; <label>:1825:                                   ; preds = %1806
  store %"class.std::__1::basic_string"* %1810, %"class.std::__1::basic_string"** %468, align 8, !noalias !32
  %1826 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %468, align 8, !noalias !32
  %1827 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1826, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1827, %"class.std::__1::__compressed_pair"** %467, align 8, !noalias !32
  %1828 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %467, align 8, !noalias !32
  %1829 = bitcast %"class.std::__1::__compressed_pair"* %1828 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1829, %"class.std::__1::__libcpp_compressed_pair_imp"** %466, align 8, !noalias !32
  %1830 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %466, align 8, !noalias !32
  %1831 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1830, i32 0, i32 0
  %1832 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1831, i32 0, i32 0
  %1833 = bitcast %union.anon.0* %1832 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1834 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1833, i32 0, i32 1
  %1835 = load i64, i64* %1834, align 8
  br label %1851

; <label>:1836:                                   ; preds = %1806
  store %"class.std::__1::basic_string"* %1810, %"class.std::__1::basic_string"** %471, align 8, !noalias !32
  %1837 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %471, align 8, !noalias !32
  %1838 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1837, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1838, %"class.std::__1::__compressed_pair"** %470, align 8, !noalias !32
  %1839 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %470, align 8, !noalias !32
  %1840 = bitcast %"class.std::__1::__compressed_pair"* %1839 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1840, %"class.std::__1::__libcpp_compressed_pair_imp"** %469, align 8, !noalias !32
  %1841 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %469, align 8, !noalias !32
  %1842 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1841, i32 0, i32 0
  %1843 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1842, i32 0, i32 0
  %1844 = bitcast %union.anon.0* %1843 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1845 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1844, i32 0, i32 0
  %1846 = bitcast %union.anon.1* %1845 to i8*
  %1847 = load i8, i8* %1846, align 8
  %1848 = zext i8 %1847 to i32
  %1849 = ashr i32 %1848, 1
  %1850 = sext i32 %1849 to i64
  br label %1851

; <label>:1851:                                   ; preds = %1836, %1825
  %1852 = phi i64 [ %1835, %1825 ], [ %1850, %1836 ]
  %1853 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %1764, i8* %1808, i64 %1852)
          to label %1854 unwind label %1901

; <label>:1854:                                   ; preds = %1851
  store %"class.std::__1::basic_string"* %1853, %"class.std::__1::basic_string"** %465, align 8, !noalias !32
  %1855 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %465, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %754, %"class.std::__1::basic_string"** %463, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %1855, %"class.std::__1::basic_string"** %464, align 8, !noalias !32
  %1856 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %463, align 8, !noalias !32
  %1857 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %464, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %1856, %"class.std::__1::basic_string"** %461, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %1857, %"class.std::__1::basic_string"** %462, align 8, !noalias !32
  %1858 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %461, align 8, !noalias !32
  %1859 = bitcast %"class.std::__1::basic_string"* %1858 to %"class.std::__1::__basic_string_common"*
  %1860 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1858, i32 0, i32 0
  %1861 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %462, align 8, !noalias !32
  %1862 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1861, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1862, %"class.std::__1::__compressed_pair"** %460, align 8, !noalias !32
  %1863 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %460, align 8, !noalias !32
  %1864 = bitcast %"class.std::__1::__compressed_pair"* %1860 to i8*
  %1865 = bitcast %"class.std::__1::__compressed_pair"* %1863 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1864, i8* %1865, i64 24, i32 8, i1 false) #8
  %1866 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %462, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %1866, %"class.std::__1::basic_string"** %457, align 8, !noalias !32
  %1867 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %457, align 8, !noalias !32
  %1868 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1867, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1868, %"class.std::__1::__compressed_pair"** %456, align 8, !noalias !32
  %1869 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %456, align 8, !noalias !32
  %1870 = bitcast %"class.std::__1::__compressed_pair"* %1869 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1870, %"class.std::__1::__libcpp_compressed_pair_imp"** %455, align 8, !noalias !32
  %1871 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %455, align 8, !noalias !32
  %1872 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1871, i32 0, i32 0
  %1873 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1872, i32 0, i32 0
  %1874 = bitcast %union.anon.0* %1873 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %1875 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %1874, i32 0, i32 0
  store [3 x i64]* %1875, [3 x i64]** %458, align 8, !noalias !32
  store i32 0, i32* %459, align 4, !noalias !32
  br label %1876

; <label>:1876:                                   ; preds = %1879, %1854
  %1877 = load i32, i32* %459, align 4, !noalias !32
  %1878 = icmp ult i32 %1877, 3
  br i1 %1878, label %1879, label %1886

; <label>:1879:                                   ; preds = %1876
  %1880 = load i32, i32* %459, align 4, !noalias !32
  %1881 = zext i32 %1880 to i64
  %1882 = load [3 x i64]*, [3 x i64]** %458, align 8, !noalias !32
  %1883 = getelementptr inbounds [3 x i64], [3 x i64]* %1882, i64 0, i64 %1881
  store i64 0, i64* %1883, align 8
  %1884 = load i32, i32* %459, align 4, !noalias !32
  %1885 = add i32 %1884, 1
  store i32 %1885, i32* %459, align 4, !noalias !32
  br label %1876

; <label>:1886:                                   ; preds = %1876
  br label %1887

; <label>:1887:                                   ; preds = %1886
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %754, %class.AstNode* null)
          to label %1888 unwind label %1905

; <label>:1888:                                   ; preds = %1887
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %754) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %758) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %755) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %756) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %757) #8
  br label %1913

; <label>:1889:                                   ; preds = %1682
  %1890 = landingpad { i8*, i32 }
          cleanup
  %1891 = extractvalue { i8*, i32 } %1890, 0
  store i8* %1891, i8** %734, align 8
  %1892 = extractvalue { i8*, i32 } %1890, 1
  store i32 %1892, i32* %735, align 4
  br label %1912

; <label>:1893:                                   ; preds = %1722
  %1894 = landingpad { i8*, i32 }
          cleanup
  %1895 = extractvalue { i8*, i32 } %1894, 0
  store i8* %1895, i8** %734, align 8
  %1896 = extractvalue { i8*, i32 } %1894, 1
  store i32 %1896, i32* %735, align 4
  br label %1911

; <label>:1897:                                   ; preds = %1759
  %1898 = landingpad { i8*, i32 }
          cleanup
  %1899 = extractvalue { i8*, i32 } %1898, 0
  store i8* %1899, i8** %734, align 8
  %1900 = extractvalue { i8*, i32 } %1898, 1
  store i32 %1900, i32* %735, align 4
  br label %1910

; <label>:1901:                                   ; preds = %1851
  %1902 = landingpad { i8*, i32 }
          cleanup
  %1903 = extractvalue { i8*, i32 } %1902, 0
  store i8* %1903, i8** %734, align 8
  %1904 = extractvalue { i8*, i32 } %1902, 1
  store i32 %1904, i32* %735, align 4
  br label %1909

; <label>:1905:                                   ; preds = %1887
  %1906 = landingpad { i8*, i32 }
          cleanup
  %1907 = extractvalue { i8*, i32 } %1906, 0
  store i8* %1907, i8** %734, align 8
  %1908 = extractvalue { i8*, i32 } %1906, 1
  store i32 %1908, i32* %735, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %754) #8
  br label %1909

; <label>:1909:                                   ; preds = %1905, %1901
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %758) #8
  br label %1910

; <label>:1910:                                   ; preds = %1909, %1897
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %755) #8
  br label %1911

; <label>:1911:                                   ; preds = %1910, %1893
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %756) #8
  br label %1912

; <label>:1912:                                   ; preds = %1911, %1889
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %757) #8
  br label %3827

; <label>:1913:                                   ; preds = %1888, %1673
  br label %1914

; <label>:1914:                                   ; preds = %1913, %1663
  br label %1915

; <label>:1915:                                   ; preds = %1914, %1653
  br label %1916

; <label>:1916:                                   ; preds = %1915, %1643
  br label %1917

; <label>:1917:                                   ; preds = %1916, %1633
  br label %1918

; <label>:1918:                                   ; preds = %1917, %1629
  br label %3798

; <label>:1919:                                   ; preds = %796, %796
  %1920 = load i32, i32* %731, align 4
  %1921 = icmp eq i32 %1920, 4
  br i1 %1921, label %1922, label %2110

; <label>:1922:                                   ; preds = %1919
  %1923 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  %1924 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %1925 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %1924, i32 0, i32 0
  %1926 = load i32, i32* %1925, align 8
  %1927 = icmp eq i32 %1926, 0
  br i1 %1927, label %1928, label %1933

; <label>:1928:                                   ; preds = %1922
  %1929 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1930 = bitcast %union.anon* %1929 to i32*
  %1931 = load i32, i32* %1930, align 8
  %1932 = sext i32 %1931 to i64
  br label %1937

; <label>:1933:                                   ; preds = %1922
  %1934 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %1935 = bitcast %union.anon* %1934 to i64*
  %1936 = load i64, i64* %1935, align 8
  br label %1937

; <label>:1937:                                   ; preds = %1933, %1928
  %1938 = phi i64 [ %1932, %1928 ], [ %1936, %1933 ]
  call void @_ZNSt3__19to_stringEl(%"class.std::__1::basic_string"* sret %759, i64 %1938)
  store %"class.std::__1::basic_string"* %1923, %"class.std::__1::basic_string"** %452, align 8
  store %"class.std::__1::basic_string"* %759, %"class.std::__1::basic_string"** %453, align 8
  %1939 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %452, align 8
  %1940 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %453, align 8
  store %"class.std::__1::basic_string"* %1939, %"class.std::__1::basic_string"** %450, align 8
  store %"class.std::__1::basic_string"* %1940, %"class.std::__1::basic_string"** %451, align 8
  %1941 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %450, align 8
  store %"class.std::__1::basic_string"* %1941, %"class.std::__1::basic_string"** %446, align 8
  %1942 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %446, align 8
  store %"class.std::__1::basic_string"* %1942, %"class.std::__1::basic_string"** %445, align 8
  %1943 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %445, align 8
  store %"class.std::__1::basic_string"* %1942, %"class.std::__1::basic_string"** %444, align 8
  %1944 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %444, align 8
  %1945 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1944, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1945, %"class.std::__1::__compressed_pair"** %443, align 8
  %1946 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %443, align 8
  %1947 = bitcast %"class.std::__1::__compressed_pair"* %1946 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1947, %"class.std::__1::__libcpp_compressed_pair_imp"** %442, align 8
  %1948 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %442, align 8
  %1949 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1948, i32 0, i32 0
  %1950 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1949, i32 0, i32 0
  %1951 = bitcast %union.anon.0* %1950 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1952 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1951, i32 0, i32 0
  %1953 = bitcast %union.anon.1* %1952 to i8*
  %1954 = load i8, i8* %1953, align 8
  %1955 = zext i8 %1954 to i32
  %1956 = and i32 %1955, 1
  %1957 = icmp ne i32 %1956, 0
  br i1 %1957, label %1958, label %1979

; <label>:1958:                                   ; preds = %1937
  store %"class.std::__1::basic_string"* %1942, %"class.std::__1::basic_string"** %437, align 8
  %1959 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %437, align 8
  %1960 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1959, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1960, %"class.std::__1::__compressed_pair"** %436, align 8
  %1961 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %436, align 8
  %1962 = bitcast %"class.std::__1::__compressed_pair"* %1961 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1962, %"class.std::__1::__libcpp_compressed_pair_imp"** %435, align 8
  %1963 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %435, align 8
  %1964 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1963, i32 0, i32 0
  %1965 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1964, i32 0, i32 0
  %1966 = bitcast %union.anon.0* %1965 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1967 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1966, i32 0, i32 2
  %1968 = load i8*, i8** %1967, align 8
  store i8 0, i8* %447, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %1968, i8* dereferenceable(1) %447) #8
  store %"class.std::__1::basic_string"* %1942, %"class.std::__1::basic_string"** %428, align 8
  store i64 0, i64* %429, align 8
  %1969 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %428, align 8
  %1970 = load i64, i64* %429, align 8
  %1971 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1969, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1971, %"class.std::__1::__compressed_pair"** %427, align 8
  %1972 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %427, align 8
  %1973 = bitcast %"class.std::__1::__compressed_pair"* %1972 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1973, %"class.std::__1::__libcpp_compressed_pair_imp"** %426, align 8
  %1974 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %426, align 8
  %1975 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1974, i32 0, i32 0
  %1976 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1975, i32 0, i32 0
  %1977 = bitcast %union.anon.0* %1976 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1978 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1977, i32 0, i32 1
  store i64 %1970, i64* %1978, align 8
  br label %2005

; <label>:1979:                                   ; preds = %1937
  store %"class.std::__1::basic_string"* %1942, %"class.std::__1::basic_string"** %434, align 8
  %1980 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %434, align 8
  %1981 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1980, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1981, %"class.std::__1::__compressed_pair"** %433, align 8
  %1982 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %433, align 8
  %1983 = bitcast %"class.std::__1::__compressed_pair"* %1982 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1983, %"class.std::__1::__libcpp_compressed_pair_imp"** %432, align 8
  %1984 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %432, align 8
  %1985 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1984, i32 0, i32 0
  %1986 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1985, i32 0, i32 0
  %1987 = bitcast %union.anon.0* %1986 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1988 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1987, i32 0, i32 1
  %1989 = getelementptr inbounds [23 x i8], [23 x i8]* %1988, i64 0, i64 0
  store i8* %1989, i8** %431, align 8
  %1990 = load i8*, i8** %431, align 8
  store i8* %1990, i8** %430, align 8
  %1991 = load i8*, i8** %430, align 8
  store i8 0, i8* %448, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %1991, i8* dereferenceable(1) %448) #8
  store %"class.std::__1::basic_string"* %1942, %"class.std::__1::basic_string"** %440, align 8
  store i64 0, i64* %441, align 8
  %1992 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %440, align 8
  %1993 = load i64, i64* %441, align 8
  %1994 = shl i64 %1993, 1
  %1995 = trunc i64 %1994 to i8
  %1996 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1992, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1996, %"class.std::__1::__compressed_pair"** %439, align 8
  %1997 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %439, align 8
  %1998 = bitcast %"class.std::__1::__compressed_pair"* %1997 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1998, %"class.std::__1::__libcpp_compressed_pair_imp"** %438, align 8
  %1999 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %438, align 8
  %2000 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1999, i32 0, i32 0
  %2001 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2000, i32 0, i32 0
  %2002 = bitcast %union.anon.0* %2001 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %2003 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %2002, i32 0, i32 0
  %2004 = bitcast %union.anon.1* %2003 to i8*
  store i8 %1995, i8* %2004, align 8
  br label %2005

; <label>:2005:                                   ; preds = %1979, %1958
  store %"class.std::__1::basic_string"* %1941, %"class.std::__1::basic_string"** %425, align 8
  %2006 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %425, align 8
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7reserveEm(%"class.std::__1::basic_string"* %2006, i64 0)
          to label %2010 unwind label %2007

; <label>:2007:                                   ; preds = %2005
  %2008 = landingpad { i8*, i32 }
          catch i8* null
  %2009 = extractvalue { i8*, i32 } %2008, 0
  call void @__clang_call_terminate(i8* %2009) #9
  unreachable

; <label>:2010:                                   ; preds = %2005
  %2011 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1941, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2011, %"class.std::__1::__compressed_pair"** %424, align 8
  %2012 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %424, align 8
  %2013 = bitcast %"class.std::__1::__compressed_pair"* %2012 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2013, %"class.std::__1::__libcpp_compressed_pair_imp"** %423, align 8
  %2014 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %423, align 8
  %2015 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2014, i32 0, i32 0
  %2016 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %451, align 8
  %2017 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2016, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2017, %"class.std::__1::__compressed_pair"** %422, align 8
  %2018 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %422, align 8
  %2019 = bitcast %"class.std::__1::__compressed_pair"* %2018 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2019, %"class.std::__1::__libcpp_compressed_pair_imp"** %421, align 8
  %2020 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %421, align 8
  %2021 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2020, i32 0, i32 0
  %2022 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2015 to i8*
  %2023 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2021 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2022, i8* %2023, i64 24, i32 8, i1 false) #8
  %2024 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %451, align 8
  store %"class.std::__1::basic_string"* %1941, %"class.std::__1::basic_string"** %418, align 8
  store %"class.std::__1::basic_string"* %2024, %"class.std::__1::basic_string"** %419, align 8
  %2025 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %418, align 8
  %2026 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %419, align 8
  store %"class.std::__1::basic_string"* %2025, %"class.std::__1::basic_string"** %416, align 8
  store %"class.std::__1::basic_string"* %2026, %"class.std::__1::basic_string"** %417, align 8
  %2027 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %416, align 8
  store %"class.std::__1::basic_string"* %2027, %"class.std::__1::basic_string"** %414, align 8
  %2028 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %414, align 8
  %2029 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2028, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2029, %"class.std::__1::__compressed_pair"** %413, align 8
  %2030 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %413, align 8
  %2031 = bitcast %"class.std::__1::__compressed_pair"* %2030 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2031, %"class.std::__1::__libcpp_compressed_pair_imp"** %412, align 8
  %2032 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %412, align 8
  %2033 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %2032 to %"class.std::__1::allocator"*
  %2034 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %417, align 8
  store %"class.std::__1::basic_string"* %2034, %"class.std::__1::basic_string"** %410, align 8
  %2035 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %410, align 8
  %2036 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2035, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2036, %"class.std::__1::__compressed_pair"** %409, align 8
  %2037 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %409, align 8
  %2038 = bitcast %"class.std::__1::__compressed_pair"* %2037 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2038, %"class.std::__1::__libcpp_compressed_pair_imp"** %408, align 8
  %2039 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %408, align 8
  %2040 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %2039 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %2040, %"class.std::__1::allocator"** %411, align 8
  %2041 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %411, align 8
  %2042 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %451, align 8
  store %"class.std::__1::basic_string"* %2042, %"class.std::__1::basic_string"** %405, align 8
  %2043 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %405, align 8
  %2044 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2043, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2044, %"class.std::__1::__compressed_pair"** %404, align 8
  %2045 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %404, align 8
  %2046 = bitcast %"class.std::__1::__compressed_pair"* %2045 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2046, %"class.std::__1::__libcpp_compressed_pair_imp"** %403, align 8
  %2047 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %403, align 8
  %2048 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2047, i32 0, i32 0
  %2049 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2048, i32 0, i32 0
  %2050 = bitcast %union.anon.0* %2049 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %2051 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %2050, i32 0, i32 0
  store [3 x i64]* %2051, [3 x i64]** %406, align 8
  store i32 0, i32* %407, align 4
  br label %2052

; <label>:2052:                                   ; preds = %2055, %2010
  %2053 = load i32, i32* %407, align 4
  %2054 = icmp ult i32 %2053, 3
  br i1 %2054, label %2055, label %2062

; <label>:2055:                                   ; preds = %2052
  %2056 = load i32, i32* %407, align 4
  %2057 = zext i32 %2056 to i64
  %2058 = load [3 x i64]*, [3 x i64]** %406, align 8
  %2059 = getelementptr inbounds [3 x i64], [3 x i64]* %2058, i64 0, i64 %2057
  store i64 0, i64* %2059, align 8
  %2060 = load i32, i32* %407, align 4
  %2061 = add i32 %2060, 1
  store i32 %2061, i32* %407, align 4
  br label %2052

; <label>:2062:                                   ; preds = %2052
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %759) #8
  %2063 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  store %"class.std::__1::basic_string"* %2063, %"class.std::__1::basic_string"** %402, align 8
  %2064 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %402, align 8
  store %"class.std::__1::basic_string"* %2064, %"class.std::__1::basic_string"** %401, align 8
  %2065 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %401, align 8
  store %"class.std::__1::basic_string"* %2065, %"class.std::__1::basic_string"** %400, align 8
  %2066 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %400, align 8
  %2067 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2066, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2067, %"class.std::__1::__compressed_pair"** %399, align 8
  %2068 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %399, align 8
  %2069 = bitcast %"class.std::__1::__compressed_pair"* %2068 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2069, %"class.std::__1::__libcpp_compressed_pair_imp"** %398, align 8
  %2070 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %398, align 8
  %2071 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2070, i32 0, i32 0
  %2072 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2071, i32 0, i32 0
  %2073 = bitcast %union.anon.0* %2072 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %2074 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %2073, i32 0, i32 0
  %2075 = bitcast %union.anon.1* %2074 to i8*
  %2076 = load i8, i8* %2075, align 8
  %2077 = zext i8 %2076 to i32
  %2078 = and i32 %2077, 1
  %2079 = icmp ne i32 %2078, 0
  br i1 %2079, label %2080, label %2091

; <label>:2080:                                   ; preds = %2062
  store %"class.std::__1::basic_string"* %2065, %"class.std::__1::basic_string"** %394, align 8
  %2081 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %394, align 8
  %2082 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2081, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2082, %"class.std::__1::__compressed_pair"** %393, align 8
  %2083 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %393, align 8
  %2084 = bitcast %"class.std::__1::__compressed_pair"* %2083 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2084, %"class.std::__1::__libcpp_compressed_pair_imp"** %392, align 8
  %2085 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %392, align 8
  %2086 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2085, i32 0, i32 0
  %2087 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2086, i32 0, i32 0
  %2088 = bitcast %union.anon.0* %2087 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %2089 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %2088, i32 0, i32 1
  %2090 = load i64, i64* %2089, align 8
  br label %2106

; <label>:2091:                                   ; preds = %2062
  store %"class.std::__1::basic_string"* %2065, %"class.std::__1::basic_string"** %397, align 8
  %2092 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %397, align 8
  %2093 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2092, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2093, %"class.std::__1::__compressed_pair"** %396, align 8
  %2094 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %396, align 8
  %2095 = bitcast %"class.std::__1::__compressed_pair"* %2094 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2095, %"class.std::__1::__libcpp_compressed_pair_imp"** %395, align 8
  %2096 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %395, align 8
  %2097 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2096, i32 0, i32 0
  %2098 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2097, i32 0, i32 0
  %2099 = bitcast %union.anon.0* %2098 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %2100 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %2099, i32 0, i32 0
  %2101 = bitcast %union.anon.1* %2100 to i8*
  %2102 = load i8, i8* %2101, align 8
  %2103 = zext i8 %2102 to i32
  %2104 = ashr i32 %2103, 1
  %2105 = sext i32 %2104 to i64
  br label %2106

; <label>:2106:                                   ; preds = %2080, %2091
  %2107 = phi i64 [ %2090, %2080 ], [ %2105, %2091 ]
  %2108 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2109 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2108, i32 0, i32 7
  store i64 %2107, i64* %2109, align 8
  br label %2556

; <label>:2110:                                   ; preds = %1919
  %2111 = load i32, i32* %731, align 4
  %2112 = icmp eq i32 %2111, 6
  br i1 %2112, label %2113, label %2254

; <label>:2113:                                   ; preds = %2110
  %2114 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2115 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2114, i32 0, i32 0
  %2116 = load i32, i32* %2115, align 8
  %2117 = icmp eq i32 %2116, 0
  br i1 %2117, label %2118, label %2123

; <label>:2118:                                   ; preds = %2113
  %2119 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2120 = bitcast %union.anon* %2119 to i32*
  %2121 = load i32, i32* %2120, align 8
  %2122 = sext i32 %2121 to i64
  br label %2127

; <label>:2123:                                   ; preds = %2113
  %2124 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2125 = bitcast %union.anon* %2124 to i64*
  %2126 = load i64, i64* %2125, align 8
  br label %2127

; <label>:2127:                                   ; preds = %2123, %2118
  %2128 = phi i64 [ %2122, %2118 ], [ %2126, %2123 ]
  %2129 = icmp sgt i64 %2128, 255
  br i1 %2129, label %2130, label %2235

; <label>:2130:                                   ; preds = %2127
  %2131 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2132 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2131, i32 0, i32 0
  %2133 = load i32, i32* %2132, align 8
  %2134 = icmp eq i32 %2133, 0
  br i1 %2134, label %2135, label %2140

; <label>:2135:                                   ; preds = %2130
  %2136 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2137 = bitcast %union.anon* %2136 to i32*
  %2138 = load i32, i32* %2137, align 8
  %2139 = sext i32 %2138 to i64
  br label %2144

; <label>:2140:                                   ; preds = %2130
  %2141 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2142 = bitcast %union.anon* %2141 to i64*
  %2143 = load i64, i64* %2142, align 8
  br label %2144

; <label>:2144:                                   ; preds = %2140, %2135
  %2145 = phi i64 [ %2139, %2135 ], [ %2143, %2140 ]
  call void @_ZNSt3__19to_stringEl(%"class.std::__1::basic_string"* sret %762, i64 %2145)
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.7, i32 0, i32 0), i8** %390, align 8, !noalias !35
  store %"class.std::__1::basic_string"* %762, %"class.std::__1::basic_string"** %391, align 8, !noalias !35
  %2146 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %391, align 8, !noalias !35
  %2147 = load i8*, i8** %390, align 8, !noalias !35
  %2148 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %2146, i64 0, i8* %2147)
          to label %2149 unwind label %2221

; <label>:2149:                                   ; preds = %2144
  store %"class.std::__1::basic_string"* %2148, %"class.std::__1::basic_string"** %389, align 8, !noalias !35
  %2150 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %389, align 8, !noalias !35
  store %"class.std::__1::basic_string"* %761, %"class.std::__1::basic_string"** %387, align 8, !noalias !35
  store %"class.std::__1::basic_string"* %2150, %"class.std::__1::basic_string"** %388, align 8, !noalias !35
  %2151 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %387, align 8, !noalias !35
  %2152 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %388, align 8, !noalias !35
  store %"class.std::__1::basic_string"* %2151, %"class.std::__1::basic_string"** %385, align 8, !noalias !35
  store %"class.std::__1::basic_string"* %2152, %"class.std::__1::basic_string"** %386, align 8, !noalias !35
  %2153 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %385, align 8, !noalias !35
  %2154 = bitcast %"class.std::__1::basic_string"* %2153 to %"class.std::__1::__basic_string_common"*
  %2155 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2153, i32 0, i32 0
  %2156 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %386, align 8, !noalias !35
  %2157 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2156, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2157, %"class.std::__1::__compressed_pair"** %384, align 8, !noalias !35
  %2158 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %384, align 8, !noalias !35
  %2159 = bitcast %"class.std::__1::__compressed_pair"* %2155 to i8*
  %2160 = bitcast %"class.std::__1::__compressed_pair"* %2158 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2159, i8* %2160, i64 24, i32 8, i1 false) #8
  %2161 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %386, align 8, !noalias !35
  store %"class.std::__1::basic_string"* %2161, %"class.std::__1::basic_string"** %381, align 8, !noalias !35
  %2162 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %381, align 8, !noalias !35
  %2163 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2162, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2163, %"class.std::__1::__compressed_pair"** %380, align 8, !noalias !35
  %2164 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %380, align 8, !noalias !35
  %2165 = bitcast %"class.std::__1::__compressed_pair"* %2164 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2165, %"class.std::__1::__libcpp_compressed_pair_imp"** %379, align 8, !noalias !35
  %2166 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %379, align 8, !noalias !35
  %2167 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2166, i32 0, i32 0
  %2168 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2167, i32 0, i32 0
  %2169 = bitcast %union.anon.0* %2168 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %2170 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %2169, i32 0, i32 0
  store [3 x i64]* %2170, [3 x i64]** %382, align 8, !noalias !35
  store i32 0, i32* %383, align 4, !noalias !35
  br label %2171

; <label>:2171:                                   ; preds = %2174, %2149
  %2172 = load i32, i32* %383, align 4, !noalias !35
  %2173 = icmp ult i32 %2172, 3
  br i1 %2173, label %2174, label %2181

; <label>:2174:                                   ; preds = %2171
  %2175 = load i32, i32* %383, align 4, !noalias !35
  %2176 = zext i32 %2175 to i64
  %2177 = load [3 x i64]*, [3 x i64]** %382, align 8, !noalias !35
  %2178 = getelementptr inbounds [3 x i64], [3 x i64]* %2177, i64 0, i64 %2176
  store i64 0, i64* %2178, align 8
  %2179 = load i32, i32* %383, align 4, !noalias !35
  %2180 = add i32 %2179, 1
  store i32 %2180, i32* %383, align 4, !noalias !35
  br label %2171

; <label>:2181:                                   ; preds = %2171
  br label %2182

; <label>:2182:                                   ; preds = %2181
  store %"class.std::__1::basic_string"* %761, %"class.std::__1::basic_string"** %377, align 8, !noalias !38
  store i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.8, i32 0, i32 0), i8** %378, align 8, !noalias !38
  %2183 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %377, align 8, !noalias !38
  %2184 = load i8*, i8** %378, align 8, !noalias !38
  %2185 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %2183, i8* %2184)
          to label %2186 unwind label %2225

; <label>:2186:                                   ; preds = %2182
  store %"class.std::__1::basic_string"* %2185, %"class.std::__1::basic_string"** %376, align 8, !noalias !38
  %2187 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %376, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %760, %"class.std::__1::basic_string"** %374, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %2187, %"class.std::__1::basic_string"** %375, align 8, !noalias !38
  %2188 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %374, align 8, !noalias !38
  %2189 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %375, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %2188, %"class.std::__1::basic_string"** %372, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %2189, %"class.std::__1::basic_string"** %373, align 8, !noalias !38
  %2190 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %372, align 8, !noalias !38
  %2191 = bitcast %"class.std::__1::basic_string"* %2190 to %"class.std::__1::__basic_string_common"*
  %2192 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2190, i32 0, i32 0
  %2193 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %373, align 8, !noalias !38
  %2194 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2193, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2194, %"class.std::__1::__compressed_pair"** %371, align 8, !noalias !38
  %2195 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %371, align 8, !noalias !38
  %2196 = bitcast %"class.std::__1::__compressed_pair"* %2192 to i8*
  %2197 = bitcast %"class.std::__1::__compressed_pair"* %2195 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2196, i8* %2197, i64 24, i32 8, i1 false) #8
  %2198 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %373, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %2198, %"class.std::__1::basic_string"** %368, align 8, !noalias !38
  %2199 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %368, align 8, !noalias !38
  %2200 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2199, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2200, %"class.std::__1::__compressed_pair"** %367, align 8, !noalias !38
  %2201 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %367, align 8, !noalias !38
  %2202 = bitcast %"class.std::__1::__compressed_pair"* %2201 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2202, %"class.std::__1::__libcpp_compressed_pair_imp"** %366, align 8, !noalias !38
  %2203 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %366, align 8, !noalias !38
  %2204 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2203, i32 0, i32 0
  %2205 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2204, i32 0, i32 0
  %2206 = bitcast %union.anon.0* %2205 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %2207 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %2206, i32 0, i32 0
  store [3 x i64]* %2207, [3 x i64]** %369, align 8, !noalias !38
  store i32 0, i32* %370, align 4, !noalias !38
  br label %2208

; <label>:2208:                                   ; preds = %2211, %2186
  %2209 = load i32, i32* %370, align 4, !noalias !38
  %2210 = icmp ult i32 %2209, 3
  br i1 %2210, label %2211, label %2218

; <label>:2211:                                   ; preds = %2208
  %2212 = load i32, i32* %370, align 4, !noalias !38
  %2213 = zext i32 %2212 to i64
  %2214 = load [3 x i64]*, [3 x i64]** %369, align 8, !noalias !38
  %2215 = getelementptr inbounds [3 x i64], [3 x i64]* %2214, i64 0, i64 %2213
  store i64 0, i64* %2215, align 8
  %2216 = load i32, i32* %370, align 4, !noalias !38
  %2217 = add i32 %2216, 1
  store i32 %2217, i32* %370, align 4, !noalias !38
  br label %2208

; <label>:2218:                                   ; preds = %2208
  br label %2219

; <label>:2219:                                   ; preds = %2218
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 8, %"class.std::__1::basic_string"* %760, %class.AstNode* null)
          to label %2220 unwind label %2229

; <label>:2220:                                   ; preds = %2219
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %760) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %761) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %762) #8
  br label %2235

; <label>:2221:                                   ; preds = %2144
  %2222 = landingpad { i8*, i32 }
          cleanup
  %2223 = extractvalue { i8*, i32 } %2222, 0
  store i8* %2223, i8** %734, align 8
  %2224 = extractvalue { i8*, i32 } %2222, 1
  store i32 %2224, i32* %735, align 4
  br label %2234

; <label>:2225:                                   ; preds = %2182
  %2226 = landingpad { i8*, i32 }
          cleanup
  %2227 = extractvalue { i8*, i32 } %2226, 0
  store i8* %2227, i8** %734, align 8
  %2228 = extractvalue { i8*, i32 } %2226, 1
  store i32 %2228, i32* %735, align 4
  br label %2233

; <label>:2229:                                   ; preds = %2219
  %2230 = landingpad { i8*, i32 }
          cleanup
  %2231 = extractvalue { i8*, i32 } %2230, 0
  store i8* %2231, i8** %734, align 8
  %2232 = extractvalue { i8*, i32 } %2230, 1
  store i32 %2232, i32* %735, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %760) #8
  br label %2233

; <label>:2233:                                   ; preds = %2229, %2225
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %761) #8
  br label %2234

; <label>:2234:                                   ; preds = %2233, %2221
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %762) #8
  br label %3827

; <label>:2235:                                   ; preds = %2220, %2127
  %2236 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2237 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2236, i32 0, i32 0
  %2238 = load i32, i32* %2237, align 8
  %2239 = icmp eq i32 %2238, 0
  br i1 %2239, label %2240, label %2245

; <label>:2240:                                   ; preds = %2235
  %2241 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2242 = bitcast %union.anon* %2241 to i32*
  %2243 = load i32, i32* %2242, align 8
  %2244 = sext i32 %2243 to i64
  br label %2249

; <label>:2245:                                   ; preds = %2235
  %2246 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2247 = bitcast %union.anon* %2246 to i64*
  %2248 = load i64, i64* %2247, align 8
  br label %2249

; <label>:2249:                                   ; preds = %2245, %2240
  %2250 = phi i64 [ %2244, %2240 ], [ %2248, %2245 ]
  %2251 = trunc i64 %2250 to i8
  %2252 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2253 = bitcast %union.anon* %2252 to i8*
  store i8 %2251, i8* %2253, align 8
  br label %2555

; <label>:2254:                                   ; preds = %2110
  %2255 = load i32, i32* %731, align 4
  %2256 = icmp eq i32 %2255, 5
  br i1 %2256, label %2257, label %2277

; <label>:2257:                                   ; preds = %2254
  %2258 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2259 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2258, i32 0, i32 0
  %2260 = load i32, i32* %2259, align 8
  %2261 = icmp eq i32 %2260, 0
  br i1 %2261, label %2262, label %2267

; <label>:2262:                                   ; preds = %2257
  %2263 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2264 = bitcast %union.anon* %2263 to i32*
  %2265 = load i32, i32* %2264, align 8
  %2266 = sext i32 %2265 to i64
  br label %2271

; <label>:2267:                                   ; preds = %2257
  %2268 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2269 = bitcast %union.anon* %2268 to i64*
  %2270 = load i64, i64* %2269, align 8
  br label %2271

; <label>:2271:                                   ; preds = %2267, %2262
  %2272 = phi i64 [ %2266, %2262 ], [ %2270, %2267 ]
  %2273 = icmp ne i64 %2272, 0
  %2274 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2275 = bitcast %union.anon* %2274 to i8*
  %2276 = zext i1 %2273 to i8
  store i8 %2276, i8* %2275, align 8
  br label %2554

; <label>:2277:                                   ; preds = %2254
  %2278 = load i32, i32* %731, align 4
  %2279 = icmp eq i32 %2278, 2
  br i1 %2279, label %2280, label %2299

; <label>:2280:                                   ; preds = %2277
  %2281 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2282 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2281, i32 0, i32 0
  %2283 = load i32, i32* %2282, align 8
  %2284 = icmp eq i32 %2283, 0
  br i1 %2284, label %2285, label %2290

; <label>:2285:                                   ; preds = %2280
  %2286 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2287 = bitcast %union.anon* %2286 to i32*
  %2288 = load i32, i32* %2287, align 8
  %2289 = sext i32 %2288 to i64
  br label %2294

; <label>:2290:                                   ; preds = %2280
  %2291 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2292 = bitcast %union.anon* %2291 to i64*
  %2293 = load i64, i64* %2292, align 8
  br label %2294

; <label>:2294:                                   ; preds = %2290, %2285
  %2295 = phi i64 [ %2289, %2285 ], [ %2293, %2290 ]
  %2296 = sitofp i64 %2295 to float
  %2297 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2298 = bitcast %union.anon* %2297 to float*
  store float %2296, float* %2298, align 8
  br label %2553

; <label>:2299:                                   ; preds = %2277
  %2300 = load i32, i32* %731, align 4
  %2301 = icmp eq i32 %2300, 3
  br i1 %2301, label %2302, label %2321

; <label>:2302:                                   ; preds = %2299
  %2303 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2304 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2303, i32 0, i32 0
  %2305 = load i32, i32* %2304, align 8
  %2306 = icmp eq i32 %2305, 0
  br i1 %2306, label %2307, label %2312

; <label>:2307:                                   ; preds = %2302
  %2308 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2309 = bitcast %union.anon* %2308 to i32*
  %2310 = load i32, i32* %2309, align 8
  %2311 = sext i32 %2310 to i64
  br label %2316

; <label>:2312:                                   ; preds = %2302
  %2313 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2314 = bitcast %union.anon* %2313 to i64*
  %2315 = load i64, i64* %2314, align 8
  br label %2316

; <label>:2316:                                   ; preds = %2312, %2307
  %2317 = phi i64 [ %2311, %2307 ], [ %2315, %2312 ]
  %2318 = sitofp i64 %2317 to double
  %2319 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2320 = bitcast %union.anon* %2319 to double*
  store double %2318, double* %2320, align 8
  br label %2552

; <label>:2321:                                   ; preds = %2299
  %2322 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2323 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2322, i32 0, i32 0
  %2324 = load i32, i32* %2323, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %766, i32 %2324)
  store i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.9, i32 0, i32 0), i8** %364, align 8, !noalias !41
  store %"class.std::__1::basic_string"* %766, %"class.std::__1::basic_string"** %365, align 8, !noalias !41
  %2325 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %365, align 8, !noalias !41
  %2326 = load i8*, i8** %364, align 8, !noalias !41
  %2327 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %2325, i64 0, i8* %2326)
          to label %2328 unwind label %2528

; <label>:2328:                                   ; preds = %2321
  store %"class.std::__1::basic_string"* %2327, %"class.std::__1::basic_string"** %363, align 8, !noalias !41
  %2329 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %363, align 8, !noalias !41
  store %"class.std::__1::basic_string"* %765, %"class.std::__1::basic_string"** %361, align 8, !noalias !41
  store %"class.std::__1::basic_string"* %2329, %"class.std::__1::basic_string"** %362, align 8, !noalias !41
  %2330 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %361, align 8, !noalias !41
  %2331 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %362, align 8, !noalias !41
  store %"class.std::__1::basic_string"* %2330, %"class.std::__1::basic_string"** %359, align 8, !noalias !41
  store %"class.std::__1::basic_string"* %2331, %"class.std::__1::basic_string"** %360, align 8, !noalias !41
  %2332 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %359, align 8, !noalias !41
  %2333 = bitcast %"class.std::__1::basic_string"* %2332 to %"class.std::__1::__basic_string_common"*
  %2334 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2332, i32 0, i32 0
  %2335 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %360, align 8, !noalias !41
  %2336 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2335, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2336, %"class.std::__1::__compressed_pair"** %358, align 8, !noalias !41
  %2337 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %358, align 8, !noalias !41
  %2338 = bitcast %"class.std::__1::__compressed_pair"* %2334 to i8*
  %2339 = bitcast %"class.std::__1::__compressed_pair"* %2337 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2338, i8* %2339, i64 24, i32 8, i1 false) #8
  %2340 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %360, align 8, !noalias !41
  store %"class.std::__1::basic_string"* %2340, %"class.std::__1::basic_string"** %355, align 8, !noalias !41
  %2341 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %355, align 8, !noalias !41
  %2342 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2341, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2342, %"class.std::__1::__compressed_pair"** %354, align 8, !noalias !41
  %2343 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %354, align 8, !noalias !41
  %2344 = bitcast %"class.std::__1::__compressed_pair"* %2343 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2344, %"class.std::__1::__libcpp_compressed_pair_imp"** %353, align 8, !noalias !41
  %2345 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %353, align 8, !noalias !41
  %2346 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2345, i32 0, i32 0
  %2347 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2346, i32 0, i32 0
  %2348 = bitcast %union.anon.0* %2347 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %2349 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %2348, i32 0, i32 0
  store [3 x i64]* %2349, [3 x i64]** %356, align 8, !noalias !41
  store i32 0, i32* %357, align 4, !noalias !41
  br label %2350

; <label>:2350:                                   ; preds = %2353, %2328
  %2351 = load i32, i32* %357, align 4, !noalias !41
  %2352 = icmp ult i32 %2351, 3
  br i1 %2352, label %2353, label %2360

; <label>:2353:                                   ; preds = %2350
  %2354 = load i32, i32* %357, align 4, !noalias !41
  %2355 = zext i32 %2354 to i64
  %2356 = load [3 x i64]*, [3 x i64]** %356, align 8, !noalias !41
  %2357 = getelementptr inbounds [3 x i64], [3 x i64]* %2356, i64 0, i64 %2355
  store i64 0, i64* %2357, align 8
  %2358 = load i32, i32* %357, align 4, !noalias !41
  %2359 = add i32 %2358, 1
  store i32 %2359, i32* %357, align 4, !noalias !41
  br label %2350

; <label>:2360:                                   ; preds = %2350
  br label %2361

; <label>:2361:                                   ; preds = %2360
  store %"class.std::__1::basic_string"* %765, %"class.std::__1::basic_string"** %351, align 8, !noalias !44
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.6, i32 0, i32 0), i8** %352, align 8, !noalias !44
  %2362 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %351, align 8, !noalias !44
  %2363 = load i8*, i8** %352, align 8, !noalias !44
  %2364 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %2362, i8* %2363)
          to label %2365 unwind label %2532

; <label>:2365:                                   ; preds = %2361
  store %"class.std::__1::basic_string"* %2364, %"class.std::__1::basic_string"** %350, align 8, !noalias !44
  %2366 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %350, align 8, !noalias !44
  store %"class.std::__1::basic_string"* %764, %"class.std::__1::basic_string"** %348, align 8, !noalias !44
  store %"class.std::__1::basic_string"* %2366, %"class.std::__1::basic_string"** %349, align 8, !noalias !44
  %2367 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %348, align 8, !noalias !44
  %2368 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %349, align 8, !noalias !44
  store %"class.std::__1::basic_string"* %2367, %"class.std::__1::basic_string"** %346, align 8, !noalias !44
  store %"class.std::__1::basic_string"* %2368, %"class.std::__1::basic_string"** %347, align 8, !noalias !44
  %2369 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %346, align 8, !noalias !44
  %2370 = bitcast %"class.std::__1::basic_string"* %2369 to %"class.std::__1::__basic_string_common"*
  %2371 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2369, i32 0, i32 0
  %2372 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %347, align 8, !noalias !44
  %2373 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2372, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2373, %"class.std::__1::__compressed_pair"** %345, align 8, !noalias !44
  %2374 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %345, align 8, !noalias !44
  %2375 = bitcast %"class.std::__1::__compressed_pair"* %2371 to i8*
  %2376 = bitcast %"class.std::__1::__compressed_pair"* %2374 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2375, i8* %2376, i64 24, i32 8, i1 false) #8
  %2377 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %347, align 8, !noalias !44
  store %"class.std::__1::basic_string"* %2377, %"class.std::__1::basic_string"** %342, align 8, !noalias !44
  %2378 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %342, align 8, !noalias !44
  %2379 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2378, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2379, %"class.std::__1::__compressed_pair"** %341, align 8, !noalias !44
  %2380 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %341, align 8, !noalias !44
  %2381 = bitcast %"class.std::__1::__compressed_pair"* %2380 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2381, %"class.std::__1::__libcpp_compressed_pair_imp"** %340, align 8, !noalias !44
  %2382 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %340, align 8, !noalias !44
  %2383 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2382, i32 0, i32 0
  %2384 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2383, i32 0, i32 0
  %2385 = bitcast %union.anon.0* %2384 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %2386 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %2385, i32 0, i32 0
  store [3 x i64]* %2386, [3 x i64]** %343, align 8, !noalias !44
  store i32 0, i32* %344, align 4, !noalias !44
  br label %2387

; <label>:2387:                                   ; preds = %2390, %2365
  %2388 = load i32, i32* %344, align 4, !noalias !44
  %2389 = icmp ult i32 %2388, 3
  br i1 %2389, label %2390, label %2397

; <label>:2390:                                   ; preds = %2387
  %2391 = load i32, i32* %344, align 4, !noalias !44
  %2392 = zext i32 %2391 to i64
  %2393 = load [3 x i64]*, [3 x i64]** %343, align 8, !noalias !44
  %2394 = getelementptr inbounds [3 x i64], [3 x i64]* %2393, i64 0, i64 %2392
  store i64 0, i64* %2394, align 8
  %2395 = load i32, i32* %344, align 4, !noalias !44
  %2396 = add i32 %2395, 1
  store i32 %2396, i32* %344, align 4, !noalias !44
  br label %2387

; <label>:2397:                                   ; preds = %2387
  br label %2398

; <label>:2398:                                   ; preds = %2397
  %2399 = load i32, i32* %731, align 4
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %767, i32 %2399)
          to label %2400 unwind label %2536

; <label>:2400:                                   ; preds = %2398
  store %"class.std::__1::basic_string"* %764, %"class.std::__1::basic_string"** %338, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %767, %"class.std::__1::basic_string"** %339, align 8, !noalias !47
  %2401 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %338, align 8, !noalias !47
  %2402 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %339, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %2401, %"class.std::__1::basic_string"** %336, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %2402, %"class.std::__1::basic_string"** %337, align 8, !noalias !47
  %2403 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %336, align 8, !noalias !47
  %2404 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %337, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %2404, %"class.std::__1::basic_string"** %335, align 8, !noalias !47
  %2405 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %335, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %2405, %"class.std::__1::basic_string"** %334, align 8, !noalias !47
  %2406 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %334, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %2406, %"class.std::__1::basic_string"** %333, align 8, !noalias !47
  %2407 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %333, align 8, !noalias !47
  %2408 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2407, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2408, %"class.std::__1::__compressed_pair"** %332, align 8, !noalias !47
  %2409 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %332, align 8, !noalias !47
  %2410 = bitcast %"class.std::__1::__compressed_pair"* %2409 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2410, %"class.std::__1::__libcpp_compressed_pair_imp"** %331, align 8, !noalias !47
  %2411 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %331, align 8, !noalias !47
  %2412 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2411, i32 0, i32 0
  %2413 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2412, i32 0, i32 0
  %2414 = bitcast %union.anon.0* %2413 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %2415 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %2414, i32 0, i32 0
  %2416 = bitcast %union.anon.1* %2415 to i8*
  %2417 = load i8, i8* %2416, align 8, !noalias !47
  %2418 = zext i8 %2417 to i32
  %2419 = and i32 %2418, 1
  %2420 = icmp ne i32 %2419, 0
  br i1 %2420, label %2421, label %2432

; <label>:2421:                                   ; preds = %2400
  store %"class.std::__1::basic_string"* %2406, %"class.std::__1::basic_string"** %325, align 8, !noalias !47
  %2422 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %325, align 8, !noalias !47
  %2423 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2422, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2423, %"class.std::__1::__compressed_pair"** %324, align 8, !noalias !47
  %2424 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %324, align 8, !noalias !47
  %2425 = bitcast %"class.std::__1::__compressed_pair"* %2424 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2425, %"class.std::__1::__libcpp_compressed_pair_imp"** %323, align 8, !noalias !47
  %2426 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %323, align 8, !noalias !47
  %2427 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2426, i32 0, i32 0
  %2428 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2427, i32 0, i32 0
  %2429 = bitcast %union.anon.0* %2428 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %2430 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %2429, i32 0, i32 2
  %2431 = load i8*, i8** %2430, align 8
  br label %2445

; <label>:2432:                                   ; preds = %2400
  store %"class.std::__1::basic_string"* %2406, %"class.std::__1::basic_string"** %330, align 8, !noalias !47
  %2433 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %330, align 8, !noalias !47
  %2434 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2433, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2434, %"class.std::__1::__compressed_pair"** %329, align 8, !noalias !47
  %2435 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %329, align 8, !noalias !47
  %2436 = bitcast %"class.std::__1::__compressed_pair"* %2435 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2436, %"class.std::__1::__libcpp_compressed_pair_imp"** %328, align 8, !noalias !47
  %2437 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %328, align 8, !noalias !47
  %2438 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2437, i32 0, i32 0
  %2439 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2438, i32 0, i32 0
  %2440 = bitcast %union.anon.0* %2439 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %2441 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %2440, i32 0, i32 1
  %2442 = getelementptr inbounds [23 x i8], [23 x i8]* %2441, i64 0, i64 0
  store i8* %2442, i8** %327, align 8, !noalias !47
  %2443 = load i8*, i8** %327, align 8, !noalias !47
  store i8* %2443, i8** %326, align 8, !noalias !47
  %2444 = load i8*, i8** %326, align 8, !noalias !47
  br label %2445

; <label>:2445:                                   ; preds = %2432, %2421
  %2446 = phi i8* [ %2431, %2421 ], [ %2444, %2432 ]
  store i8* %2446, i8** %322, align 8, !noalias !47
  %2447 = load i8*, i8** %322, align 8, !noalias !47
  %2448 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %337, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %2448, %"class.std::__1::basic_string"** %321, align 8, !noalias !47
  %2449 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %321, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %2449, %"class.std::__1::basic_string"** %320, align 8, !noalias !47
  %2450 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %320, align 8, !noalias !47
  %2451 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2450, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2451, %"class.std::__1::__compressed_pair"** %319, align 8, !noalias !47
  %2452 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %319, align 8, !noalias !47
  %2453 = bitcast %"class.std::__1::__compressed_pair"* %2452 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2453, %"class.std::__1::__libcpp_compressed_pair_imp"** %318, align 8, !noalias !47
  %2454 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %318, align 8, !noalias !47
  %2455 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2454, i32 0, i32 0
  %2456 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2455, i32 0, i32 0
  %2457 = bitcast %union.anon.0* %2456 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %2458 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %2457, i32 0, i32 0
  %2459 = bitcast %union.anon.1* %2458 to i8*
  %2460 = load i8, i8* %2459, align 8, !noalias !47
  %2461 = zext i8 %2460 to i32
  %2462 = and i32 %2461, 1
  %2463 = icmp ne i32 %2462, 0
  br i1 %2463, label %2464, label %2475

; <label>:2464:                                   ; preds = %2445
  store %"class.std::__1::basic_string"* %2449, %"class.std::__1::basic_string"** %314, align 8, !noalias !47
  %2465 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %314, align 8, !noalias !47
  %2466 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2465, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2466, %"class.std::__1::__compressed_pair"** %313, align 8, !noalias !47
  %2467 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %313, align 8, !noalias !47
  %2468 = bitcast %"class.std::__1::__compressed_pair"* %2467 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2468, %"class.std::__1::__libcpp_compressed_pair_imp"** %312, align 8, !noalias !47
  %2469 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %312, align 8, !noalias !47
  %2470 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2469, i32 0, i32 0
  %2471 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2470, i32 0, i32 0
  %2472 = bitcast %union.anon.0* %2471 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %2473 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %2472, i32 0, i32 1
  %2474 = load i64, i64* %2473, align 8
  br label %2490

; <label>:2475:                                   ; preds = %2445
  store %"class.std::__1::basic_string"* %2449, %"class.std::__1::basic_string"** %317, align 8, !noalias !47
  %2476 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %317, align 8, !noalias !47
  %2477 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2476, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2477, %"class.std::__1::__compressed_pair"** %316, align 8, !noalias !47
  %2478 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %316, align 8, !noalias !47
  %2479 = bitcast %"class.std::__1::__compressed_pair"* %2478 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2479, %"class.std::__1::__libcpp_compressed_pair_imp"** %315, align 8, !noalias !47
  %2480 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %315, align 8, !noalias !47
  %2481 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2480, i32 0, i32 0
  %2482 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2481, i32 0, i32 0
  %2483 = bitcast %union.anon.0* %2482 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %2484 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %2483, i32 0, i32 0
  %2485 = bitcast %union.anon.1* %2484 to i8*
  %2486 = load i8, i8* %2485, align 8
  %2487 = zext i8 %2486 to i32
  %2488 = ashr i32 %2487, 1
  %2489 = sext i32 %2488 to i64
  br label %2490

; <label>:2490:                                   ; preds = %2475, %2464
  %2491 = phi i64 [ %2474, %2464 ], [ %2489, %2475 ]
  %2492 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %2403, i8* %2447, i64 %2491)
          to label %2493 unwind label %2540

; <label>:2493:                                   ; preds = %2490
  store %"class.std::__1::basic_string"* %2492, %"class.std::__1::basic_string"** %311, align 8, !noalias !47
  %2494 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %311, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %763, %"class.std::__1::basic_string"** %309, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %2494, %"class.std::__1::basic_string"** %310, align 8, !noalias !47
  %2495 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %309, align 8, !noalias !47
  %2496 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %310, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %2495, %"class.std::__1::basic_string"** %307, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %2496, %"class.std::__1::basic_string"** %308, align 8, !noalias !47
  %2497 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %307, align 8, !noalias !47
  %2498 = bitcast %"class.std::__1::basic_string"* %2497 to %"class.std::__1::__basic_string_common"*
  %2499 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2497, i32 0, i32 0
  %2500 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %308, align 8, !noalias !47
  %2501 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2500, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2501, %"class.std::__1::__compressed_pair"** %306, align 8, !noalias !47
  %2502 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %306, align 8, !noalias !47
  %2503 = bitcast %"class.std::__1::__compressed_pair"* %2499 to i8*
  %2504 = bitcast %"class.std::__1::__compressed_pair"* %2502 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2503, i8* %2504, i64 24, i32 8, i1 false) #8
  %2505 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %308, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %2505, %"class.std::__1::basic_string"** %303, align 8, !noalias !47
  %2506 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %303, align 8, !noalias !47
  %2507 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2506, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2507, %"class.std::__1::__compressed_pair"** %302, align 8, !noalias !47
  %2508 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %302, align 8, !noalias !47
  %2509 = bitcast %"class.std::__1::__compressed_pair"* %2508 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2509, %"class.std::__1::__libcpp_compressed_pair_imp"** %301, align 8, !noalias !47
  %2510 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %301, align 8, !noalias !47
  %2511 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2510, i32 0, i32 0
  %2512 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2511, i32 0, i32 0
  %2513 = bitcast %union.anon.0* %2512 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %2514 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %2513, i32 0, i32 0
  store [3 x i64]* %2514, [3 x i64]** %304, align 8, !noalias !47
  store i32 0, i32* %305, align 4, !noalias !47
  br label %2515

; <label>:2515:                                   ; preds = %2518, %2493
  %2516 = load i32, i32* %305, align 4, !noalias !47
  %2517 = icmp ult i32 %2516, 3
  br i1 %2517, label %2518, label %2525

; <label>:2518:                                   ; preds = %2515
  %2519 = load i32, i32* %305, align 4, !noalias !47
  %2520 = zext i32 %2519 to i64
  %2521 = load [3 x i64]*, [3 x i64]** %304, align 8, !noalias !47
  %2522 = getelementptr inbounds [3 x i64], [3 x i64]* %2521, i64 0, i64 %2520
  store i64 0, i64* %2522, align 8
  %2523 = load i32, i32* %305, align 4, !noalias !47
  %2524 = add i32 %2523, 1
  store i32 %2524, i32* %305, align 4, !noalias !47
  br label %2515

; <label>:2525:                                   ; preds = %2515
  br label %2526

; <label>:2526:                                   ; preds = %2525
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %763, %class.AstNode* null)
          to label %2527 unwind label %2544

; <label>:2527:                                   ; preds = %2526
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %763) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %767) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %764) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %765) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %766) #8
  br label %2552

; <label>:2528:                                   ; preds = %2321
  %2529 = landingpad { i8*, i32 }
          cleanup
  %2530 = extractvalue { i8*, i32 } %2529, 0
  store i8* %2530, i8** %734, align 8
  %2531 = extractvalue { i8*, i32 } %2529, 1
  store i32 %2531, i32* %735, align 4
  br label %2551

; <label>:2532:                                   ; preds = %2361
  %2533 = landingpad { i8*, i32 }
          cleanup
  %2534 = extractvalue { i8*, i32 } %2533, 0
  store i8* %2534, i8** %734, align 8
  %2535 = extractvalue { i8*, i32 } %2533, 1
  store i32 %2535, i32* %735, align 4
  br label %2550

; <label>:2536:                                   ; preds = %2398
  %2537 = landingpad { i8*, i32 }
          cleanup
  %2538 = extractvalue { i8*, i32 } %2537, 0
  store i8* %2538, i8** %734, align 8
  %2539 = extractvalue { i8*, i32 } %2537, 1
  store i32 %2539, i32* %735, align 4
  br label %2549

; <label>:2540:                                   ; preds = %2490
  %2541 = landingpad { i8*, i32 }
          cleanup
  %2542 = extractvalue { i8*, i32 } %2541, 0
  store i8* %2542, i8** %734, align 8
  %2543 = extractvalue { i8*, i32 } %2541, 1
  store i32 %2543, i32* %735, align 4
  br label %2548

; <label>:2544:                                   ; preds = %2526
  %2545 = landingpad { i8*, i32 }
          cleanup
  %2546 = extractvalue { i8*, i32 } %2545, 0
  store i8* %2546, i8** %734, align 8
  %2547 = extractvalue { i8*, i32 } %2545, 1
  store i32 %2547, i32* %735, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %763) #8
  br label %2548

; <label>:2548:                                   ; preds = %2544, %2540
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %767) #8
  br label %2549

; <label>:2549:                                   ; preds = %2548, %2536
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %764) #8
  br label %2550

; <label>:2550:                                   ; preds = %2549, %2532
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %765) #8
  br label %2551

; <label>:2551:                                   ; preds = %2550, %2528
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %766) #8
  br label %3827

; <label>:2552:                                   ; preds = %2527, %2316
  br label %2553

; <label>:2553:                                   ; preds = %2552, %2294
  br label %2554

; <label>:2554:                                   ; preds = %2553, %2271
  br label %2555

; <label>:2555:                                   ; preds = %2554, %2249
  br label %2556

; <label>:2556:                                   ; preds = %2555, %2106
  br label %3798

; <label>:2557:                                   ; preds = %796, %796
  %2558 = load i32, i32* %731, align 4
  %2559 = icmp eq i32 %2558, 4
  br i1 %2559, label %2560, label %2748

; <label>:2560:                                   ; preds = %2557
  %2561 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  %2562 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2563 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2562, i32 0, i32 0
  %2564 = load i32, i32* %2563, align 8
  %2565 = icmp eq i32 %2564, 2
  br i1 %2565, label %2566, label %2571

; <label>:2566:                                   ; preds = %2560
  %2567 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2568 = bitcast %union.anon* %2567 to float*
  %2569 = load float, float* %2568, align 8
  %2570 = fpext float %2569 to double
  br label %2575

; <label>:2571:                                   ; preds = %2560
  %2572 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2573 = bitcast %union.anon* %2572 to double*
  %2574 = load double, double* %2573, align 8
  br label %2575

; <label>:2575:                                   ; preds = %2571, %2566
  %2576 = phi double [ %2570, %2566 ], [ %2574, %2571 ]
  call void @_ZNSt3__19to_stringEd(%"class.std::__1::basic_string"* sret %768, double %2576)
  store %"class.std::__1::basic_string"* %2561, %"class.std::__1::basic_string"** %298, align 8
  store %"class.std::__1::basic_string"* %768, %"class.std::__1::basic_string"** %299, align 8
  %2577 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %298, align 8
  %2578 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %299, align 8
  store %"class.std::__1::basic_string"* %2577, %"class.std::__1::basic_string"** %296, align 8
  store %"class.std::__1::basic_string"* %2578, %"class.std::__1::basic_string"** %297, align 8
  %2579 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %296, align 8
  store %"class.std::__1::basic_string"* %2579, %"class.std::__1::basic_string"** %292, align 8
  %2580 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %292, align 8
  store %"class.std::__1::basic_string"* %2580, %"class.std::__1::basic_string"** %291, align 8
  %2581 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %291, align 8
  store %"class.std::__1::basic_string"* %2580, %"class.std::__1::basic_string"** %290, align 8
  %2582 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %290, align 8
  %2583 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2582, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2583, %"class.std::__1::__compressed_pair"** %289, align 8
  %2584 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %289, align 8
  %2585 = bitcast %"class.std::__1::__compressed_pair"* %2584 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2585, %"class.std::__1::__libcpp_compressed_pair_imp"** %288, align 8
  %2586 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %288, align 8
  %2587 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2586, i32 0, i32 0
  %2588 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2587, i32 0, i32 0
  %2589 = bitcast %union.anon.0* %2588 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %2590 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %2589, i32 0, i32 0
  %2591 = bitcast %union.anon.1* %2590 to i8*
  %2592 = load i8, i8* %2591, align 8
  %2593 = zext i8 %2592 to i32
  %2594 = and i32 %2593, 1
  %2595 = icmp ne i32 %2594, 0
  br i1 %2595, label %2596, label %2617

; <label>:2596:                                   ; preds = %2575
  store %"class.std::__1::basic_string"* %2580, %"class.std::__1::basic_string"** %283, align 8
  %2597 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %283, align 8
  %2598 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2597, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2598, %"class.std::__1::__compressed_pair"** %282, align 8
  %2599 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %282, align 8
  %2600 = bitcast %"class.std::__1::__compressed_pair"* %2599 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2600, %"class.std::__1::__libcpp_compressed_pair_imp"** %281, align 8
  %2601 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %281, align 8
  %2602 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2601, i32 0, i32 0
  %2603 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2602, i32 0, i32 0
  %2604 = bitcast %union.anon.0* %2603 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %2605 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %2604, i32 0, i32 2
  %2606 = load i8*, i8** %2605, align 8
  store i8 0, i8* %293, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %2606, i8* dereferenceable(1) %293) #8
  store %"class.std::__1::basic_string"* %2580, %"class.std::__1::basic_string"** %274, align 8
  store i64 0, i64* %275, align 8
  %2607 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %274, align 8
  %2608 = load i64, i64* %275, align 8
  %2609 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2607, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2609, %"class.std::__1::__compressed_pair"** %273, align 8
  %2610 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %273, align 8
  %2611 = bitcast %"class.std::__1::__compressed_pair"* %2610 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2611, %"class.std::__1::__libcpp_compressed_pair_imp"** %272, align 8
  %2612 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %272, align 8
  %2613 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2612, i32 0, i32 0
  %2614 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2613, i32 0, i32 0
  %2615 = bitcast %union.anon.0* %2614 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %2616 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %2615, i32 0, i32 1
  store i64 %2608, i64* %2616, align 8
  br label %2643

; <label>:2617:                                   ; preds = %2575
  store %"class.std::__1::basic_string"* %2580, %"class.std::__1::basic_string"** %280, align 8
  %2618 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %280, align 8
  %2619 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2618, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2619, %"class.std::__1::__compressed_pair"** %279, align 8
  %2620 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %279, align 8
  %2621 = bitcast %"class.std::__1::__compressed_pair"* %2620 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2621, %"class.std::__1::__libcpp_compressed_pair_imp"** %278, align 8
  %2622 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %278, align 8
  %2623 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2622, i32 0, i32 0
  %2624 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2623, i32 0, i32 0
  %2625 = bitcast %union.anon.0* %2624 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %2626 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %2625, i32 0, i32 1
  %2627 = getelementptr inbounds [23 x i8], [23 x i8]* %2626, i64 0, i64 0
  store i8* %2627, i8** %277, align 8
  %2628 = load i8*, i8** %277, align 8
  store i8* %2628, i8** %276, align 8
  %2629 = load i8*, i8** %276, align 8
  store i8 0, i8* %294, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %2629, i8* dereferenceable(1) %294) #8
  store %"class.std::__1::basic_string"* %2580, %"class.std::__1::basic_string"** %286, align 8
  store i64 0, i64* %287, align 8
  %2630 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %286, align 8
  %2631 = load i64, i64* %287, align 8
  %2632 = shl i64 %2631, 1
  %2633 = trunc i64 %2632 to i8
  %2634 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2630, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2634, %"class.std::__1::__compressed_pair"** %285, align 8
  %2635 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %285, align 8
  %2636 = bitcast %"class.std::__1::__compressed_pair"* %2635 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2636, %"class.std::__1::__libcpp_compressed_pair_imp"** %284, align 8
  %2637 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %284, align 8
  %2638 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2637, i32 0, i32 0
  %2639 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2638, i32 0, i32 0
  %2640 = bitcast %union.anon.0* %2639 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %2641 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %2640, i32 0, i32 0
  %2642 = bitcast %union.anon.1* %2641 to i8*
  store i8 %2633, i8* %2642, align 8
  br label %2643

; <label>:2643:                                   ; preds = %2617, %2596
  store %"class.std::__1::basic_string"* %2579, %"class.std::__1::basic_string"** %271, align 8
  %2644 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %271, align 8
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7reserveEm(%"class.std::__1::basic_string"* %2644, i64 0)
          to label %2648 unwind label %2645

; <label>:2645:                                   ; preds = %2643
  %2646 = landingpad { i8*, i32 }
          catch i8* null
  %2647 = extractvalue { i8*, i32 } %2646, 0
  call void @__clang_call_terminate(i8* %2647) #9
  unreachable

; <label>:2648:                                   ; preds = %2643
  %2649 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2579, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2649, %"class.std::__1::__compressed_pair"** %270, align 8
  %2650 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %270, align 8
  %2651 = bitcast %"class.std::__1::__compressed_pair"* %2650 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2651, %"class.std::__1::__libcpp_compressed_pair_imp"** %269, align 8
  %2652 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %269, align 8
  %2653 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2652, i32 0, i32 0
  %2654 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %297, align 8
  %2655 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2654, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2655, %"class.std::__1::__compressed_pair"** %268, align 8
  %2656 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %268, align 8
  %2657 = bitcast %"class.std::__1::__compressed_pair"* %2656 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2657, %"class.std::__1::__libcpp_compressed_pair_imp"** %267, align 8
  %2658 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %267, align 8
  %2659 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2658, i32 0, i32 0
  %2660 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2653 to i8*
  %2661 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2659 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2660, i8* %2661, i64 24, i32 8, i1 false) #8
  %2662 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %297, align 8
  store %"class.std::__1::basic_string"* %2579, %"class.std::__1::basic_string"** %264, align 8
  store %"class.std::__1::basic_string"* %2662, %"class.std::__1::basic_string"** %265, align 8
  %2663 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %264, align 8
  %2664 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %265, align 8
  store %"class.std::__1::basic_string"* %2663, %"class.std::__1::basic_string"** %262, align 8
  store %"class.std::__1::basic_string"* %2664, %"class.std::__1::basic_string"** %263, align 8
  %2665 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %262, align 8
  store %"class.std::__1::basic_string"* %2665, %"class.std::__1::basic_string"** %260, align 8
  %2666 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %260, align 8
  %2667 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2666, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2667, %"class.std::__1::__compressed_pair"** %259, align 8
  %2668 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %259, align 8
  %2669 = bitcast %"class.std::__1::__compressed_pair"* %2668 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2669, %"class.std::__1::__libcpp_compressed_pair_imp"** %258, align 8
  %2670 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %258, align 8
  %2671 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %2670 to %"class.std::__1::allocator"*
  %2672 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %263, align 8
  store %"class.std::__1::basic_string"* %2672, %"class.std::__1::basic_string"** %256, align 8
  %2673 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %256, align 8
  %2674 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2673, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2674, %"class.std::__1::__compressed_pair"** %255, align 8
  %2675 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %255, align 8
  %2676 = bitcast %"class.std::__1::__compressed_pair"* %2675 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2676, %"class.std::__1::__libcpp_compressed_pair_imp"** %254, align 8
  %2677 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %254, align 8
  %2678 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %2677 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %2678, %"class.std::__1::allocator"** %257, align 8
  %2679 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %257, align 8
  %2680 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %297, align 8
  store %"class.std::__1::basic_string"* %2680, %"class.std::__1::basic_string"** %251, align 8
  %2681 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %251, align 8
  %2682 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2681, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2682, %"class.std::__1::__compressed_pair"** %250, align 8
  %2683 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %250, align 8
  %2684 = bitcast %"class.std::__1::__compressed_pair"* %2683 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2684, %"class.std::__1::__libcpp_compressed_pair_imp"** %249, align 8
  %2685 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %249, align 8
  %2686 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2685, i32 0, i32 0
  %2687 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2686, i32 0, i32 0
  %2688 = bitcast %union.anon.0* %2687 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %2689 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %2688, i32 0, i32 0
  store [3 x i64]* %2689, [3 x i64]** %252, align 8
  store i32 0, i32* %253, align 4
  br label %2690

; <label>:2690:                                   ; preds = %2693, %2648
  %2691 = load i32, i32* %253, align 4
  %2692 = icmp ult i32 %2691, 3
  br i1 %2692, label %2693, label %2700

; <label>:2693:                                   ; preds = %2690
  %2694 = load i32, i32* %253, align 4
  %2695 = zext i32 %2694 to i64
  %2696 = load [3 x i64]*, [3 x i64]** %252, align 8
  %2697 = getelementptr inbounds [3 x i64], [3 x i64]* %2696, i64 0, i64 %2695
  store i64 0, i64* %2697, align 8
  %2698 = load i32, i32* %253, align 4
  %2699 = add i32 %2698, 1
  store i32 %2699, i32* %253, align 4
  br label %2690

; <label>:2700:                                   ; preds = %2690
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %768) #8
  %2701 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  store %"class.std::__1::basic_string"* %2701, %"class.std::__1::basic_string"** %248, align 8
  %2702 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %248, align 8
  store %"class.std::__1::basic_string"* %2702, %"class.std::__1::basic_string"** %247, align 8
  %2703 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %247, align 8
  store %"class.std::__1::basic_string"* %2703, %"class.std::__1::basic_string"** %246, align 8
  %2704 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %246, align 8
  %2705 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2704, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2705, %"class.std::__1::__compressed_pair"** %245, align 8
  %2706 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %245, align 8
  %2707 = bitcast %"class.std::__1::__compressed_pair"* %2706 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2707, %"class.std::__1::__libcpp_compressed_pair_imp"** %244, align 8
  %2708 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %244, align 8
  %2709 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2708, i32 0, i32 0
  %2710 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2709, i32 0, i32 0
  %2711 = bitcast %union.anon.0* %2710 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %2712 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %2711, i32 0, i32 0
  %2713 = bitcast %union.anon.1* %2712 to i8*
  %2714 = load i8, i8* %2713, align 8
  %2715 = zext i8 %2714 to i32
  %2716 = and i32 %2715, 1
  %2717 = icmp ne i32 %2716, 0
  br i1 %2717, label %2718, label %2729

; <label>:2718:                                   ; preds = %2700
  store %"class.std::__1::basic_string"* %2703, %"class.std::__1::basic_string"** %240, align 8
  %2719 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %240, align 8
  %2720 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2719, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2720, %"class.std::__1::__compressed_pair"** %239, align 8
  %2721 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %239, align 8
  %2722 = bitcast %"class.std::__1::__compressed_pair"* %2721 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2722, %"class.std::__1::__libcpp_compressed_pair_imp"** %238, align 8
  %2723 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %238, align 8
  %2724 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2723, i32 0, i32 0
  %2725 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2724, i32 0, i32 0
  %2726 = bitcast %union.anon.0* %2725 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %2727 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %2726, i32 0, i32 1
  %2728 = load i64, i64* %2727, align 8
  br label %2744

; <label>:2729:                                   ; preds = %2700
  store %"class.std::__1::basic_string"* %2703, %"class.std::__1::basic_string"** %243, align 8
  %2730 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %243, align 8
  %2731 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2730, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2731, %"class.std::__1::__compressed_pair"** %242, align 8
  %2732 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %242, align 8
  %2733 = bitcast %"class.std::__1::__compressed_pair"* %2732 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2733, %"class.std::__1::__libcpp_compressed_pair_imp"** %241, align 8
  %2734 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %241, align 8
  %2735 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2734, i32 0, i32 0
  %2736 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2735, i32 0, i32 0
  %2737 = bitcast %union.anon.0* %2736 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %2738 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %2737, i32 0, i32 0
  %2739 = bitcast %union.anon.1* %2738 to i8*
  %2740 = load i8, i8* %2739, align 8
  %2741 = zext i8 %2740 to i32
  %2742 = ashr i32 %2741, 1
  %2743 = sext i32 %2742 to i64
  br label %2744

; <label>:2744:                                   ; preds = %2718, %2729
  %2745 = phi i64 [ %2728, %2718 ], [ %2743, %2729 ]
  %2746 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2747 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2746, i32 0, i32 7
  store i64 %2745, i64* %2747, align 8
  br label %3238

; <label>:2748:                                   ; preds = %2557
  %2749 = load i32, i32* %731, align 4
  %2750 = icmp eq i32 %2749, 6
  br i1 %2750, label %2751, label %2892

; <label>:2751:                                   ; preds = %2748
  %2752 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2753 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2752, i32 0, i32 0
  %2754 = load i32, i32* %2753, align 8
  %2755 = icmp eq i32 %2754, 2
  br i1 %2755, label %2756, label %2761

; <label>:2756:                                   ; preds = %2751
  %2757 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2758 = bitcast %union.anon* %2757 to float*
  %2759 = load float, float* %2758, align 8
  %2760 = fpext float %2759 to double
  br label %2765

; <label>:2761:                                   ; preds = %2751
  %2762 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2763 = bitcast %union.anon* %2762 to double*
  %2764 = load double, double* %2763, align 8
  br label %2765

; <label>:2765:                                   ; preds = %2761, %2756
  %2766 = phi double [ %2760, %2756 ], [ %2764, %2761 ]
  %2767 = fcmp ogt double %2766, 2.550000e+02
  br i1 %2767, label %2768, label %2873

; <label>:2768:                                   ; preds = %2765
  %2769 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2770 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2769, i32 0, i32 0
  %2771 = load i32, i32* %2770, align 8
  %2772 = icmp eq i32 %2771, 2
  br i1 %2772, label %2773, label %2778

; <label>:2773:                                   ; preds = %2768
  %2774 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2775 = bitcast %union.anon* %2774 to float*
  %2776 = load float, float* %2775, align 8
  %2777 = fpext float %2776 to double
  br label %2782

; <label>:2778:                                   ; preds = %2768
  %2779 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2780 = bitcast %union.anon* %2779 to double*
  %2781 = load double, double* %2780, align 8
  br label %2782

; <label>:2782:                                   ; preds = %2778, %2773
  %2783 = phi double [ %2777, %2773 ], [ %2781, %2778 ]
  call void @_ZNSt3__19to_stringEd(%"class.std::__1::basic_string"* sret %771, double %2783)
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.7, i32 0, i32 0), i8** %236, align 8, !noalias !50
  store %"class.std::__1::basic_string"* %771, %"class.std::__1::basic_string"** %237, align 8, !noalias !50
  %2784 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %237, align 8, !noalias !50
  %2785 = load i8*, i8** %236, align 8, !noalias !50
  %2786 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %2784, i64 0, i8* %2785)
          to label %2787 unwind label %2859

; <label>:2787:                                   ; preds = %2782
  store %"class.std::__1::basic_string"* %2786, %"class.std::__1::basic_string"** %235, align 8, !noalias !50
  %2788 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %235, align 8, !noalias !50
  store %"class.std::__1::basic_string"* %770, %"class.std::__1::basic_string"** %233, align 8, !noalias !50
  store %"class.std::__1::basic_string"* %2788, %"class.std::__1::basic_string"** %234, align 8, !noalias !50
  %2789 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %233, align 8, !noalias !50
  %2790 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %234, align 8, !noalias !50
  store %"class.std::__1::basic_string"* %2789, %"class.std::__1::basic_string"** %231, align 8, !noalias !50
  store %"class.std::__1::basic_string"* %2790, %"class.std::__1::basic_string"** %232, align 8, !noalias !50
  %2791 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %231, align 8, !noalias !50
  %2792 = bitcast %"class.std::__1::basic_string"* %2791 to %"class.std::__1::__basic_string_common"*
  %2793 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2791, i32 0, i32 0
  %2794 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %232, align 8, !noalias !50
  %2795 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2794, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2795, %"class.std::__1::__compressed_pair"** %230, align 8, !noalias !50
  %2796 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %230, align 8, !noalias !50
  %2797 = bitcast %"class.std::__1::__compressed_pair"* %2793 to i8*
  %2798 = bitcast %"class.std::__1::__compressed_pair"* %2796 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2797, i8* %2798, i64 24, i32 8, i1 false) #8
  %2799 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %232, align 8, !noalias !50
  store %"class.std::__1::basic_string"* %2799, %"class.std::__1::basic_string"** %227, align 8, !noalias !50
  %2800 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %227, align 8, !noalias !50
  %2801 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2800, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2801, %"class.std::__1::__compressed_pair"** %226, align 8, !noalias !50
  %2802 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %226, align 8, !noalias !50
  %2803 = bitcast %"class.std::__1::__compressed_pair"* %2802 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2803, %"class.std::__1::__libcpp_compressed_pair_imp"** %225, align 8, !noalias !50
  %2804 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %225, align 8, !noalias !50
  %2805 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2804, i32 0, i32 0
  %2806 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2805, i32 0, i32 0
  %2807 = bitcast %union.anon.0* %2806 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %2808 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %2807, i32 0, i32 0
  store [3 x i64]* %2808, [3 x i64]** %228, align 8, !noalias !50
  store i32 0, i32* %229, align 4, !noalias !50
  br label %2809

; <label>:2809:                                   ; preds = %2812, %2787
  %2810 = load i32, i32* %229, align 4, !noalias !50
  %2811 = icmp ult i32 %2810, 3
  br i1 %2811, label %2812, label %2819

; <label>:2812:                                   ; preds = %2809
  %2813 = load i32, i32* %229, align 4, !noalias !50
  %2814 = zext i32 %2813 to i64
  %2815 = load [3 x i64]*, [3 x i64]** %228, align 8, !noalias !50
  %2816 = getelementptr inbounds [3 x i64], [3 x i64]* %2815, i64 0, i64 %2814
  store i64 0, i64* %2816, align 8
  %2817 = load i32, i32* %229, align 4, !noalias !50
  %2818 = add i32 %2817, 1
  store i32 %2818, i32* %229, align 4, !noalias !50
  br label %2809

; <label>:2819:                                   ; preds = %2809
  br label %2820

; <label>:2820:                                   ; preds = %2819
  store %"class.std::__1::basic_string"* %770, %"class.std::__1::basic_string"** %223, align 8, !noalias !53
  store i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.8, i32 0, i32 0), i8** %224, align 8, !noalias !53
  %2821 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %223, align 8, !noalias !53
  %2822 = load i8*, i8** %224, align 8, !noalias !53
  %2823 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %2821, i8* %2822)
          to label %2824 unwind label %2863

; <label>:2824:                                   ; preds = %2820
  store %"class.std::__1::basic_string"* %2823, %"class.std::__1::basic_string"** %222, align 8, !noalias !53
  %2825 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %222, align 8, !noalias !53
  store %"class.std::__1::basic_string"* %769, %"class.std::__1::basic_string"** %220, align 8, !noalias !53
  store %"class.std::__1::basic_string"* %2825, %"class.std::__1::basic_string"** %221, align 8, !noalias !53
  %2826 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %220, align 8, !noalias !53
  %2827 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %221, align 8, !noalias !53
  store %"class.std::__1::basic_string"* %2826, %"class.std::__1::basic_string"** %218, align 8, !noalias !53
  store %"class.std::__1::basic_string"* %2827, %"class.std::__1::basic_string"** %219, align 8, !noalias !53
  %2828 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %218, align 8, !noalias !53
  %2829 = bitcast %"class.std::__1::basic_string"* %2828 to %"class.std::__1::__basic_string_common"*
  %2830 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2828, i32 0, i32 0
  %2831 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %219, align 8, !noalias !53
  %2832 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2831, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2832, %"class.std::__1::__compressed_pair"** %217, align 8, !noalias !53
  %2833 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %217, align 8, !noalias !53
  %2834 = bitcast %"class.std::__1::__compressed_pair"* %2830 to i8*
  %2835 = bitcast %"class.std::__1::__compressed_pair"* %2833 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2834, i8* %2835, i64 24, i32 8, i1 false) #8
  %2836 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %219, align 8, !noalias !53
  store %"class.std::__1::basic_string"* %2836, %"class.std::__1::basic_string"** %214, align 8, !noalias !53
  %2837 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %214, align 8, !noalias !53
  %2838 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %2837, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %2838, %"class.std::__1::__compressed_pair"** %213, align 8, !noalias !53
  %2839 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %213, align 8, !noalias !53
  %2840 = bitcast %"class.std::__1::__compressed_pair"* %2839 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %2840, %"class.std::__1::__libcpp_compressed_pair_imp"** %212, align 8, !noalias !53
  %2841 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %212, align 8, !noalias !53
  %2842 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %2841, i32 0, i32 0
  %2843 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %2842, i32 0, i32 0
  %2844 = bitcast %union.anon.0* %2843 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %2845 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %2844, i32 0, i32 0
  store [3 x i64]* %2845, [3 x i64]** %215, align 8, !noalias !53
  store i32 0, i32* %216, align 4, !noalias !53
  br label %2846

; <label>:2846:                                   ; preds = %2849, %2824
  %2847 = load i32, i32* %216, align 4, !noalias !53
  %2848 = icmp ult i32 %2847, 3
  br i1 %2848, label %2849, label %2856

; <label>:2849:                                   ; preds = %2846
  %2850 = load i32, i32* %216, align 4, !noalias !53
  %2851 = zext i32 %2850 to i64
  %2852 = load [3 x i64]*, [3 x i64]** %215, align 8, !noalias !53
  %2853 = getelementptr inbounds [3 x i64], [3 x i64]* %2852, i64 0, i64 %2851
  store i64 0, i64* %2853, align 8
  %2854 = load i32, i32* %216, align 4, !noalias !53
  %2855 = add i32 %2854, 1
  store i32 %2855, i32* %216, align 4, !noalias !53
  br label %2846

; <label>:2856:                                   ; preds = %2846
  br label %2857

; <label>:2857:                                   ; preds = %2856
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 8, %"class.std::__1::basic_string"* %769, %class.AstNode* null)
          to label %2858 unwind label %2867

; <label>:2858:                                   ; preds = %2857
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %769) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %770) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %771) #8
  br label %2873

; <label>:2859:                                   ; preds = %2782
  %2860 = landingpad { i8*, i32 }
          cleanup
  %2861 = extractvalue { i8*, i32 } %2860, 0
  store i8* %2861, i8** %734, align 8
  %2862 = extractvalue { i8*, i32 } %2860, 1
  store i32 %2862, i32* %735, align 4
  br label %2872

; <label>:2863:                                   ; preds = %2820
  %2864 = landingpad { i8*, i32 }
          cleanup
  %2865 = extractvalue { i8*, i32 } %2864, 0
  store i8* %2865, i8** %734, align 8
  %2866 = extractvalue { i8*, i32 } %2864, 1
  store i32 %2866, i32* %735, align 4
  br label %2871

; <label>:2867:                                   ; preds = %2857
  %2868 = landingpad { i8*, i32 }
          cleanup
  %2869 = extractvalue { i8*, i32 } %2868, 0
  store i8* %2869, i8** %734, align 8
  %2870 = extractvalue { i8*, i32 } %2868, 1
  store i32 %2870, i32* %735, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %769) #8
  br label %2871

; <label>:2871:                                   ; preds = %2867, %2863
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %770) #8
  br label %2872

; <label>:2872:                                   ; preds = %2871, %2859
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %771) #8
  br label %3827

; <label>:2873:                                   ; preds = %2858, %2765
  %2874 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2875 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2874, i32 0, i32 0
  %2876 = load i32, i32* %2875, align 8
  %2877 = icmp eq i32 %2876, 2
  br i1 %2877, label %2878, label %2883

; <label>:2878:                                   ; preds = %2873
  %2879 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2880 = bitcast %union.anon* %2879 to float*
  %2881 = load float, float* %2880, align 8
  %2882 = fpext float %2881 to double
  br label %2887

; <label>:2883:                                   ; preds = %2873
  %2884 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2885 = bitcast %union.anon* %2884 to double*
  %2886 = load double, double* %2885, align 8
  br label %2887

; <label>:2887:                                   ; preds = %2883, %2878
  %2888 = phi double [ %2882, %2878 ], [ %2886, %2883 ]
  %2889 = fptosi double %2888 to i8
  %2890 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2891 = bitcast %union.anon* %2890 to i8*
  store i8 %2889, i8* %2891, align 8
  br label %3237

; <label>:2892:                                   ; preds = %2748
  %2893 = load i32, i32* %731, align 4
  %2894 = icmp eq i32 %2893, 5
  br i1 %2894, label %2895, label %2915

; <label>:2895:                                   ; preds = %2892
  %2896 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2897 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2896, i32 0, i32 0
  %2898 = load i32, i32* %2897, align 8
  %2899 = icmp eq i32 %2898, 2
  br i1 %2899, label %2900, label %2905

; <label>:2900:                                   ; preds = %2895
  %2901 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2902 = bitcast %union.anon* %2901 to float*
  %2903 = load float, float* %2902, align 8
  %2904 = fpext float %2903 to double
  br label %2909

; <label>:2905:                                   ; preds = %2895
  %2906 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2907 = bitcast %union.anon* %2906 to double*
  %2908 = load double, double* %2907, align 8
  br label %2909

; <label>:2909:                                   ; preds = %2905, %2900
  %2910 = phi double [ %2904, %2900 ], [ %2908, %2905 ]
  %2911 = fcmp une double %2910, 0.000000e+00
  %2912 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2913 = bitcast %union.anon* %2912 to i8*
  %2914 = zext i1 %2911 to i8
  store i8 %2914, i8* %2913, align 8
  br label %3236

; <label>:2915:                                   ; preds = %2892
  %2916 = load i32, i32* %731, align 4
  %2917 = icmp eq i32 %2916, 2
  br i1 %2917, label %2918, label %2936

; <label>:2918:                                   ; preds = %2915
  %2919 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2920 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2919, i32 0, i32 0
  %2921 = load i32, i32* %2920, align 8
  %2922 = icmp eq i32 %2921, 2
  br i1 %2922, label %2923, label %2927

; <label>:2923:                                   ; preds = %2918
  %2924 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2925 = bitcast %union.anon* %2924 to float*
  %2926 = load float, float* %2925, align 8
  br label %2932

; <label>:2927:                                   ; preds = %2918
  %2928 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2929 = bitcast %union.anon* %2928 to double*
  %2930 = load double, double* %2929, align 8
  %2931 = fptrunc double %2930 to float
  br label %2932

; <label>:2932:                                   ; preds = %2927, %2923
  %2933 = phi float [ %2926, %2923 ], [ %2931, %2927 ]
  %2934 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2935 = bitcast %union.anon* %2934 to float*
  store float %2933, float* %2935, align 8
  br label %3235

; <label>:2936:                                   ; preds = %2915
  %2937 = load i32, i32* %731, align 4
  %2938 = icmp eq i32 %2937, 0
  br i1 %2938, label %2939, label %2958

; <label>:2939:                                   ; preds = %2936
  %2940 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2941 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2940, i32 0, i32 0
  %2942 = load i32, i32* %2941, align 8
  %2943 = icmp eq i32 %2942, 2
  br i1 %2943, label %2944, label %2949

; <label>:2944:                                   ; preds = %2939
  %2945 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2946 = bitcast %union.anon* %2945 to float*
  %2947 = load float, float* %2946, align 8
  %2948 = fpext float %2947 to double
  br label %2953

; <label>:2949:                                   ; preds = %2939
  %2950 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2951 = bitcast %union.anon* %2950 to double*
  %2952 = load double, double* %2951, align 8
  br label %2953

; <label>:2953:                                   ; preds = %2949, %2944
  %2954 = phi double [ %2948, %2944 ], [ %2952, %2949 ]
  %2955 = fptosi double %2954 to i32
  %2956 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2957 = bitcast %union.anon* %2956 to i32*
  store i32 %2955, i32* %2957, align 8
  br label %3234

; <label>:2958:                                   ; preds = %2936
  %2959 = load i32, i32* %731, align 4
  %2960 = icmp eq i32 %2959, 1
  br i1 %2960, label %2961, label %2980

; <label>:2961:                                   ; preds = %2958
  %2962 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2963 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2962, i32 0, i32 0
  %2964 = load i32, i32* %2963, align 8
  %2965 = icmp eq i32 %2964, 2
  br i1 %2965, label %2966, label %2971

; <label>:2966:                                   ; preds = %2961
  %2967 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2968 = bitcast %union.anon* %2967 to float*
  %2969 = load float, float* %2968, align 8
  %2970 = fpext float %2969 to double
  br label %2975

; <label>:2971:                                   ; preds = %2961
  %2972 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2973 = bitcast %union.anon* %2972 to double*
  %2974 = load double, double* %2973, align 8
  br label %2975

; <label>:2975:                                   ; preds = %2971, %2966
  %2976 = phi double [ %2970, %2966 ], [ %2974, %2971 ]
  %2977 = fptosi double %2976 to i64
  %2978 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2979 = bitcast %union.anon* %2978 to i64*
  store i64 %2977, i64* %2979, align 8
  br label %3233

; <label>:2980:                                   ; preds = %2958
  %2981 = load i32, i32* %731, align 4
  %2982 = icmp eq i32 %2981, 3
  br i1 %2982, label %2983, label %3001

; <label>:2983:                                   ; preds = %2980
  %2984 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %2985 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %2984, i32 0, i32 0
  %2986 = load i32, i32* %2985, align 8
  %2987 = icmp eq i32 %2986, 2
  br i1 %2987, label %2988, label %2993

; <label>:2988:                                   ; preds = %2983
  %2989 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2990 = bitcast %union.anon* %2989 to float*
  %2991 = load float, float* %2990, align 8
  %2992 = fpext float %2991 to double
  br label %2997

; <label>:2993:                                   ; preds = %2983
  %2994 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %2995 = bitcast %union.anon* %2994 to double*
  %2996 = load double, double* %2995, align 8
  br label %2997

; <label>:2997:                                   ; preds = %2993, %2988
  %2998 = phi double [ %2992, %2988 ], [ %2996, %2993 ]
  %2999 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %3000 = bitcast %union.anon* %2999 to double*
  store double %2998, double* %3000, align 8
  br label %3232

; <label>:3001:                                   ; preds = %2980
  %3002 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %3003 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3002, i32 0, i32 0
  %3004 = load i32, i32* %3003, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %775, i32 %3004)
  store i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str.5, i32 0, i32 0), i8** %210, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %775, %"class.std::__1::basic_string"** %211, align 8, !noalias !56
  %3005 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %211, align 8, !noalias !56
  %3006 = load i8*, i8** %210, align 8, !noalias !56
  %3007 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %3005, i64 0, i8* %3006)
          to label %3008 unwind label %3208

; <label>:3008:                                   ; preds = %3001
  store %"class.std::__1::basic_string"* %3007, %"class.std::__1::basic_string"** %209, align 8, !noalias !56
  %3009 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %209, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %774, %"class.std::__1::basic_string"** %207, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %3009, %"class.std::__1::basic_string"** %208, align 8, !noalias !56
  %3010 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %207, align 8, !noalias !56
  %3011 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %208, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %3010, %"class.std::__1::basic_string"** %205, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %3011, %"class.std::__1::basic_string"** %206, align 8, !noalias !56
  %3012 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %205, align 8, !noalias !56
  %3013 = bitcast %"class.std::__1::basic_string"* %3012 to %"class.std::__1::__basic_string_common"*
  %3014 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3012, i32 0, i32 0
  %3015 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %206, align 8, !noalias !56
  %3016 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3015, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3016, %"class.std::__1::__compressed_pair"** %204, align 8, !noalias !56
  %3017 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %204, align 8, !noalias !56
  %3018 = bitcast %"class.std::__1::__compressed_pair"* %3014 to i8*
  %3019 = bitcast %"class.std::__1::__compressed_pair"* %3017 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %3018, i8* %3019, i64 24, i32 8, i1 false) #8
  %3020 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %206, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %3020, %"class.std::__1::basic_string"** %201, align 8, !noalias !56
  %3021 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %201, align 8, !noalias !56
  %3022 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3021, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3022, %"class.std::__1::__compressed_pair"** %200, align 8, !noalias !56
  %3023 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %200, align 8, !noalias !56
  %3024 = bitcast %"class.std::__1::__compressed_pair"* %3023 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3024, %"class.std::__1::__libcpp_compressed_pair_imp"** %199, align 8, !noalias !56
  %3025 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %199, align 8, !noalias !56
  %3026 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3025, i32 0, i32 0
  %3027 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3026, i32 0, i32 0
  %3028 = bitcast %union.anon.0* %3027 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %3029 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %3028, i32 0, i32 0
  store [3 x i64]* %3029, [3 x i64]** %202, align 8, !noalias !56
  store i32 0, i32* %203, align 4, !noalias !56
  br label %3030

; <label>:3030:                                   ; preds = %3033, %3008
  %3031 = load i32, i32* %203, align 4, !noalias !56
  %3032 = icmp ult i32 %3031, 3
  br i1 %3032, label %3033, label %3040

; <label>:3033:                                   ; preds = %3030
  %3034 = load i32, i32* %203, align 4, !noalias !56
  %3035 = zext i32 %3034 to i64
  %3036 = load [3 x i64]*, [3 x i64]** %202, align 8, !noalias !56
  %3037 = getelementptr inbounds [3 x i64], [3 x i64]* %3036, i64 0, i64 %3035
  store i64 0, i64* %3037, align 8
  %3038 = load i32, i32* %203, align 4, !noalias !56
  %3039 = add i32 %3038, 1
  store i32 %3039, i32* %203, align 4, !noalias !56
  br label %3030

; <label>:3040:                                   ; preds = %3030
  br label %3041

; <label>:3041:                                   ; preds = %3040
  store %"class.std::__1::basic_string"* %774, %"class.std::__1::basic_string"** %197, align 8, !noalias !59
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.6, i32 0, i32 0), i8** %198, align 8, !noalias !59
  %3042 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %197, align 8, !noalias !59
  %3043 = load i8*, i8** %198, align 8, !noalias !59
  %3044 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %3042, i8* %3043)
          to label %3045 unwind label %3212

; <label>:3045:                                   ; preds = %3041
  store %"class.std::__1::basic_string"* %3044, %"class.std::__1::basic_string"** %196, align 8, !noalias !59
  %3046 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %196, align 8, !noalias !59
  store %"class.std::__1::basic_string"* %773, %"class.std::__1::basic_string"** %194, align 8, !noalias !59
  store %"class.std::__1::basic_string"* %3046, %"class.std::__1::basic_string"** %195, align 8, !noalias !59
  %3047 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %194, align 8, !noalias !59
  %3048 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %195, align 8, !noalias !59
  store %"class.std::__1::basic_string"* %3047, %"class.std::__1::basic_string"** %192, align 8, !noalias !59
  store %"class.std::__1::basic_string"* %3048, %"class.std::__1::basic_string"** %193, align 8, !noalias !59
  %3049 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %192, align 8, !noalias !59
  %3050 = bitcast %"class.std::__1::basic_string"* %3049 to %"class.std::__1::__basic_string_common"*
  %3051 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3049, i32 0, i32 0
  %3052 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %193, align 8, !noalias !59
  %3053 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3052, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3053, %"class.std::__1::__compressed_pair"** %191, align 8, !noalias !59
  %3054 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %191, align 8, !noalias !59
  %3055 = bitcast %"class.std::__1::__compressed_pair"* %3051 to i8*
  %3056 = bitcast %"class.std::__1::__compressed_pair"* %3054 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %3055, i8* %3056, i64 24, i32 8, i1 false) #8
  %3057 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %193, align 8, !noalias !59
  store %"class.std::__1::basic_string"* %3057, %"class.std::__1::basic_string"** %188, align 8, !noalias !59
  %3058 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %188, align 8, !noalias !59
  %3059 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3058, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3059, %"class.std::__1::__compressed_pair"** %187, align 8, !noalias !59
  %3060 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %187, align 8, !noalias !59
  %3061 = bitcast %"class.std::__1::__compressed_pair"* %3060 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3061, %"class.std::__1::__libcpp_compressed_pair_imp"** %186, align 8, !noalias !59
  %3062 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %186, align 8, !noalias !59
  %3063 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3062, i32 0, i32 0
  %3064 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3063, i32 0, i32 0
  %3065 = bitcast %union.anon.0* %3064 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %3066 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %3065, i32 0, i32 0
  store [3 x i64]* %3066, [3 x i64]** %189, align 8, !noalias !59
  store i32 0, i32* %190, align 4, !noalias !59
  br label %3067

; <label>:3067:                                   ; preds = %3070, %3045
  %3068 = load i32, i32* %190, align 4, !noalias !59
  %3069 = icmp ult i32 %3068, 3
  br i1 %3069, label %3070, label %3077

; <label>:3070:                                   ; preds = %3067
  %3071 = load i32, i32* %190, align 4, !noalias !59
  %3072 = zext i32 %3071 to i64
  %3073 = load [3 x i64]*, [3 x i64]** %189, align 8, !noalias !59
  %3074 = getelementptr inbounds [3 x i64], [3 x i64]* %3073, i64 0, i64 %3072
  store i64 0, i64* %3074, align 8
  %3075 = load i32, i32* %190, align 4, !noalias !59
  %3076 = add i32 %3075, 1
  store i32 %3076, i32* %190, align 4, !noalias !59
  br label %3067

; <label>:3077:                                   ; preds = %3067
  br label %3078

; <label>:3078:                                   ; preds = %3077
  %3079 = load i32, i32* %731, align 4
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %776, i32 %3079)
          to label %3080 unwind label %3216

; <label>:3080:                                   ; preds = %3078
  store %"class.std::__1::basic_string"* %773, %"class.std::__1::basic_string"** %184, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %776, %"class.std::__1::basic_string"** %185, align 8, !noalias !62
  %3081 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %184, align 8, !noalias !62
  %3082 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %185, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %3081, %"class.std::__1::basic_string"** %182, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %3082, %"class.std::__1::basic_string"** %183, align 8, !noalias !62
  %3083 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %182, align 8, !noalias !62
  %3084 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %183, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %3084, %"class.std::__1::basic_string"** %181, align 8, !noalias !62
  %3085 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %181, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %3085, %"class.std::__1::basic_string"** %180, align 8, !noalias !62
  %3086 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %180, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %3086, %"class.std::__1::basic_string"** %179, align 8, !noalias !62
  %3087 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %179, align 8, !noalias !62
  %3088 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3087, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3088, %"class.std::__1::__compressed_pair"** %178, align 8, !noalias !62
  %3089 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %178, align 8, !noalias !62
  %3090 = bitcast %"class.std::__1::__compressed_pair"* %3089 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3090, %"class.std::__1::__libcpp_compressed_pair_imp"** %177, align 8, !noalias !62
  %3091 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %177, align 8, !noalias !62
  %3092 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3091, i32 0, i32 0
  %3093 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3092, i32 0, i32 0
  %3094 = bitcast %union.anon.0* %3093 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %3095 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %3094, i32 0, i32 0
  %3096 = bitcast %union.anon.1* %3095 to i8*
  %3097 = load i8, i8* %3096, align 8, !noalias !62
  %3098 = zext i8 %3097 to i32
  %3099 = and i32 %3098, 1
  %3100 = icmp ne i32 %3099, 0
  br i1 %3100, label %3101, label %3112

; <label>:3101:                                   ; preds = %3080
  store %"class.std::__1::basic_string"* %3086, %"class.std::__1::basic_string"** %171, align 8, !noalias !62
  %3102 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %171, align 8, !noalias !62
  %3103 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3102, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3103, %"class.std::__1::__compressed_pair"** %170, align 8, !noalias !62
  %3104 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %170, align 8, !noalias !62
  %3105 = bitcast %"class.std::__1::__compressed_pair"* %3104 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3105, %"class.std::__1::__libcpp_compressed_pair_imp"** %169, align 8, !noalias !62
  %3106 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %169, align 8, !noalias !62
  %3107 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3106, i32 0, i32 0
  %3108 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3107, i32 0, i32 0
  %3109 = bitcast %union.anon.0* %3108 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %3110 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %3109, i32 0, i32 2
  %3111 = load i8*, i8** %3110, align 8
  br label %3125

; <label>:3112:                                   ; preds = %3080
  store %"class.std::__1::basic_string"* %3086, %"class.std::__1::basic_string"** %176, align 8, !noalias !62
  %3113 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %176, align 8, !noalias !62
  %3114 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3113, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3114, %"class.std::__1::__compressed_pair"** %175, align 8, !noalias !62
  %3115 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %175, align 8, !noalias !62
  %3116 = bitcast %"class.std::__1::__compressed_pair"* %3115 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3116, %"class.std::__1::__libcpp_compressed_pair_imp"** %174, align 8, !noalias !62
  %3117 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %174, align 8, !noalias !62
  %3118 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3117, i32 0, i32 0
  %3119 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3118, i32 0, i32 0
  %3120 = bitcast %union.anon.0* %3119 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %3121 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %3120, i32 0, i32 1
  %3122 = getelementptr inbounds [23 x i8], [23 x i8]* %3121, i64 0, i64 0
  store i8* %3122, i8** %173, align 8, !noalias !62
  %3123 = load i8*, i8** %173, align 8, !noalias !62
  store i8* %3123, i8** %172, align 8, !noalias !62
  %3124 = load i8*, i8** %172, align 8, !noalias !62
  br label %3125

; <label>:3125:                                   ; preds = %3112, %3101
  %3126 = phi i8* [ %3111, %3101 ], [ %3124, %3112 ]
  store i8* %3126, i8** %168, align 8, !noalias !62
  %3127 = load i8*, i8** %168, align 8, !noalias !62
  %3128 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %183, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %3128, %"class.std::__1::basic_string"** %167, align 8, !noalias !62
  %3129 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %167, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %3129, %"class.std::__1::basic_string"** %166, align 8, !noalias !62
  %3130 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %166, align 8, !noalias !62
  %3131 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3130, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3131, %"class.std::__1::__compressed_pair"** %165, align 8, !noalias !62
  %3132 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %165, align 8, !noalias !62
  %3133 = bitcast %"class.std::__1::__compressed_pair"* %3132 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3133, %"class.std::__1::__libcpp_compressed_pair_imp"** %164, align 8, !noalias !62
  %3134 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %164, align 8, !noalias !62
  %3135 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3134, i32 0, i32 0
  %3136 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3135, i32 0, i32 0
  %3137 = bitcast %union.anon.0* %3136 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %3138 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %3137, i32 0, i32 0
  %3139 = bitcast %union.anon.1* %3138 to i8*
  %3140 = load i8, i8* %3139, align 8, !noalias !62
  %3141 = zext i8 %3140 to i32
  %3142 = and i32 %3141, 1
  %3143 = icmp ne i32 %3142, 0
  br i1 %3143, label %3144, label %3155

; <label>:3144:                                   ; preds = %3125
  store %"class.std::__1::basic_string"* %3129, %"class.std::__1::basic_string"** %160, align 8, !noalias !62
  %3145 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %160, align 8, !noalias !62
  %3146 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3145, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3146, %"class.std::__1::__compressed_pair"** %159, align 8, !noalias !62
  %3147 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %159, align 8, !noalias !62
  %3148 = bitcast %"class.std::__1::__compressed_pair"* %3147 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3148, %"class.std::__1::__libcpp_compressed_pair_imp"** %158, align 8, !noalias !62
  %3149 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %158, align 8, !noalias !62
  %3150 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3149, i32 0, i32 0
  %3151 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3150, i32 0, i32 0
  %3152 = bitcast %union.anon.0* %3151 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %3153 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %3152, i32 0, i32 1
  %3154 = load i64, i64* %3153, align 8
  br label %3170

; <label>:3155:                                   ; preds = %3125
  store %"class.std::__1::basic_string"* %3129, %"class.std::__1::basic_string"** %163, align 8, !noalias !62
  %3156 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %163, align 8, !noalias !62
  %3157 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3156, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3157, %"class.std::__1::__compressed_pair"** %162, align 8, !noalias !62
  %3158 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %162, align 8, !noalias !62
  %3159 = bitcast %"class.std::__1::__compressed_pair"* %3158 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3159, %"class.std::__1::__libcpp_compressed_pair_imp"** %161, align 8, !noalias !62
  %3160 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %161, align 8, !noalias !62
  %3161 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3160, i32 0, i32 0
  %3162 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3161, i32 0, i32 0
  %3163 = bitcast %union.anon.0* %3162 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %3164 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %3163, i32 0, i32 0
  %3165 = bitcast %union.anon.1* %3164 to i8*
  %3166 = load i8, i8* %3165, align 8
  %3167 = zext i8 %3166 to i32
  %3168 = ashr i32 %3167, 1
  %3169 = sext i32 %3168 to i64
  br label %3170

; <label>:3170:                                   ; preds = %3155, %3144
  %3171 = phi i64 [ %3154, %3144 ], [ %3169, %3155 ]
  %3172 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %3083, i8* %3127, i64 %3171)
          to label %3173 unwind label %3220

; <label>:3173:                                   ; preds = %3170
  store %"class.std::__1::basic_string"* %3172, %"class.std::__1::basic_string"** %157, align 8, !noalias !62
  %3174 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %157, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %772, %"class.std::__1::basic_string"** %155, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %3174, %"class.std::__1::basic_string"** %156, align 8, !noalias !62
  %3175 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %155, align 8, !noalias !62
  %3176 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %156, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %3175, %"class.std::__1::basic_string"** %153, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %3176, %"class.std::__1::basic_string"** %154, align 8, !noalias !62
  %3177 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %153, align 8, !noalias !62
  %3178 = bitcast %"class.std::__1::basic_string"* %3177 to %"class.std::__1::__basic_string_common"*
  %3179 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3177, i32 0, i32 0
  %3180 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %154, align 8, !noalias !62
  %3181 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3180, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3181, %"class.std::__1::__compressed_pair"** %152, align 8, !noalias !62
  %3182 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %152, align 8, !noalias !62
  %3183 = bitcast %"class.std::__1::__compressed_pair"* %3179 to i8*
  %3184 = bitcast %"class.std::__1::__compressed_pair"* %3182 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %3183, i8* %3184, i64 24, i32 8, i1 false) #8
  %3185 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %154, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %3185, %"class.std::__1::basic_string"** %149, align 8, !noalias !62
  %3186 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %149, align 8, !noalias !62
  %3187 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3186, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3187, %"class.std::__1::__compressed_pair"** %148, align 8, !noalias !62
  %3188 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %148, align 8, !noalias !62
  %3189 = bitcast %"class.std::__1::__compressed_pair"* %3188 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3189, %"class.std::__1::__libcpp_compressed_pair_imp"** %147, align 8, !noalias !62
  %3190 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %147, align 8, !noalias !62
  %3191 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3190, i32 0, i32 0
  %3192 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3191, i32 0, i32 0
  %3193 = bitcast %union.anon.0* %3192 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %3194 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %3193, i32 0, i32 0
  store [3 x i64]* %3194, [3 x i64]** %150, align 8, !noalias !62
  store i32 0, i32* %151, align 4, !noalias !62
  br label %3195

; <label>:3195:                                   ; preds = %3198, %3173
  %3196 = load i32, i32* %151, align 4, !noalias !62
  %3197 = icmp ult i32 %3196, 3
  br i1 %3197, label %3198, label %3205

; <label>:3198:                                   ; preds = %3195
  %3199 = load i32, i32* %151, align 4, !noalias !62
  %3200 = zext i32 %3199 to i64
  %3201 = load [3 x i64]*, [3 x i64]** %150, align 8, !noalias !62
  %3202 = getelementptr inbounds [3 x i64], [3 x i64]* %3201, i64 0, i64 %3200
  store i64 0, i64* %3202, align 8
  %3203 = load i32, i32* %151, align 4, !noalias !62
  %3204 = add i32 %3203, 1
  store i32 %3204, i32* %151, align 4, !noalias !62
  br label %3195

; <label>:3205:                                   ; preds = %3195
  br label %3206

; <label>:3206:                                   ; preds = %3205
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %772, %class.AstNode* null)
          to label %3207 unwind label %3224

; <label>:3207:                                   ; preds = %3206
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %772) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %776) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %773) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %774) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %775) #8
  br label %3232

; <label>:3208:                                   ; preds = %3001
  %3209 = landingpad { i8*, i32 }
          cleanup
  %3210 = extractvalue { i8*, i32 } %3209, 0
  store i8* %3210, i8** %734, align 8
  %3211 = extractvalue { i8*, i32 } %3209, 1
  store i32 %3211, i32* %735, align 4
  br label %3231

; <label>:3212:                                   ; preds = %3041
  %3213 = landingpad { i8*, i32 }
          cleanup
  %3214 = extractvalue { i8*, i32 } %3213, 0
  store i8* %3214, i8** %734, align 8
  %3215 = extractvalue { i8*, i32 } %3213, 1
  store i32 %3215, i32* %735, align 4
  br label %3230

; <label>:3216:                                   ; preds = %3078
  %3217 = landingpad { i8*, i32 }
          cleanup
  %3218 = extractvalue { i8*, i32 } %3217, 0
  store i8* %3218, i8** %734, align 8
  %3219 = extractvalue { i8*, i32 } %3217, 1
  store i32 %3219, i32* %735, align 4
  br label %3229

; <label>:3220:                                   ; preds = %3170
  %3221 = landingpad { i8*, i32 }
          cleanup
  %3222 = extractvalue { i8*, i32 } %3221, 0
  store i8* %3222, i8** %734, align 8
  %3223 = extractvalue { i8*, i32 } %3221, 1
  store i32 %3223, i32* %735, align 4
  br label %3228

; <label>:3224:                                   ; preds = %3206
  %3225 = landingpad { i8*, i32 }
          cleanup
  %3226 = extractvalue { i8*, i32 } %3225, 0
  store i8* %3226, i8** %734, align 8
  %3227 = extractvalue { i8*, i32 } %3225, 1
  store i32 %3227, i32* %735, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %772) #8
  br label %3228

; <label>:3228:                                   ; preds = %3224, %3220
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %776) #8
  br label %3229

; <label>:3229:                                   ; preds = %3228, %3216
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %773) #8
  br label %3230

; <label>:3230:                                   ; preds = %3229, %3212
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %774) #8
  br label %3231

; <label>:3231:                                   ; preds = %3230, %3208
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %775) #8
  br label %3827

; <label>:3232:                                   ; preds = %3207, %2997
  br label %3233

; <label>:3233:                                   ; preds = %3232, %2975
  br label %3234

; <label>:3234:                                   ; preds = %3233, %2953
  br label %3235

; <label>:3235:                                   ; preds = %3234, %2932
  br label %3236

; <label>:3236:                                   ; preds = %3235, %2909
  br label %3237

; <label>:3237:                                   ; preds = %3236, %2887
  br label %3238

; <label>:3238:                                   ; preds = %3237, %2744
  br label %3798

; <label>:3239:                                   ; preds = %796
  %3240 = load i32, i32* %731, align 4
  %3241 = icmp eq i32 %3240, 4
  br i1 %3241, label %3242, label %3299

; <label>:3242:                                   ; preds = %3239
  %3243 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  %3244 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %3245 = bitcast %union.anon* %3244 to i8*
  %3246 = load i8, i8* %3245, align 8
  %3247 = trunc i8 %3246 to i1
  %3248 = select i1 %3247, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.10, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.3, i32 0, i32 0)
  store %"class.std::__1::basic_string"* %3243, %"class.std::__1::basic_string"** %145, align 8
  store i8* %3248, i8** %146, align 8
  %3249 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %145, align 8
  %3250 = load i8*, i8** %146, align 8
  %3251 = call dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEPKc(%"class.std::__1::basic_string"* %3249, i8* %3250)
  %3252 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  store %"class.std::__1::basic_string"* %3252, %"class.std::__1::basic_string"** %144, align 8
  %3253 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %144, align 8
  store %"class.std::__1::basic_string"* %3253, %"class.std::__1::basic_string"** %143, align 8
  %3254 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %143, align 8
  store %"class.std::__1::basic_string"* %3254, %"class.std::__1::basic_string"** %142, align 8
  %3255 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %142, align 8
  %3256 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3255, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3256, %"class.std::__1::__compressed_pair"** %141, align 8
  %3257 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %141, align 8
  %3258 = bitcast %"class.std::__1::__compressed_pair"* %3257 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3258, %"class.std::__1::__libcpp_compressed_pair_imp"** %140, align 8
  %3259 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %140, align 8
  %3260 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3259, i32 0, i32 0
  %3261 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3260, i32 0, i32 0
  %3262 = bitcast %union.anon.0* %3261 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %3263 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %3262, i32 0, i32 0
  %3264 = bitcast %union.anon.1* %3263 to i8*
  %3265 = load i8, i8* %3264, align 8
  %3266 = zext i8 %3265 to i32
  %3267 = and i32 %3266, 1
  %3268 = icmp ne i32 %3267, 0
  br i1 %3268, label %3269, label %3280

; <label>:3269:                                   ; preds = %3242
  store %"class.std::__1::basic_string"* %3254, %"class.std::__1::basic_string"** %136, align 8
  %3270 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %136, align 8
  %3271 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3270, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3271, %"class.std::__1::__compressed_pair"** %135, align 8
  %3272 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %135, align 8
  %3273 = bitcast %"class.std::__1::__compressed_pair"* %3272 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3273, %"class.std::__1::__libcpp_compressed_pair_imp"** %134, align 8
  %3274 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %134, align 8
  %3275 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3274, i32 0, i32 0
  %3276 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3275, i32 0, i32 0
  %3277 = bitcast %union.anon.0* %3276 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %3278 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %3277, i32 0, i32 1
  %3279 = load i64, i64* %3278, align 8
  br label %3295

; <label>:3280:                                   ; preds = %3242
  store %"class.std::__1::basic_string"* %3254, %"class.std::__1::basic_string"** %139, align 8
  %3281 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %139, align 8
  %3282 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3281, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3282, %"class.std::__1::__compressed_pair"** %138, align 8
  %3283 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %138, align 8
  %3284 = bitcast %"class.std::__1::__compressed_pair"* %3283 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3284, %"class.std::__1::__libcpp_compressed_pair_imp"** %137, align 8
  %3285 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %137, align 8
  %3286 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3285, i32 0, i32 0
  %3287 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3286, i32 0, i32 0
  %3288 = bitcast %union.anon.0* %3287 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %3289 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %3288, i32 0, i32 0
  %3290 = bitcast %union.anon.1* %3289 to i8*
  %3291 = load i8, i8* %3290, align 8
  %3292 = zext i8 %3291 to i32
  %3293 = ashr i32 %3292, 1
  %3294 = sext i32 %3293 to i64
  br label %3295

; <label>:3295:                                   ; preds = %3269, %3280
  %3296 = phi i64 [ %3279, %3269 ], [ %3294, %3280 ]
  %3297 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %3298 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3297, i32 0, i32 7
  store i64 %3296, i64* %3298, align 8
  br label %3590

; <label>:3299:                                   ; preds = %3239
  %3300 = load i32, i32* %731, align 4
  %3301 = icmp eq i32 %3300, 6
  br i1 %3301, label %3302, label %3310

; <label>:3302:                                   ; preds = %3299
  %3303 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %3304 = bitcast %union.anon* %3303 to i8*
  %3305 = load i8, i8* %3304, align 8
  %3306 = trunc i8 %3305 to i1
  %3307 = select i1 %3306, i8 49, i8 48
  %3308 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %3309 = bitcast %union.anon* %3308 to i8*
  store i8 %3307, i8* %3309, align 8
  br label %3589

; <label>:3310:                                   ; preds = %3299
  %3311 = load i32, i32* %731, align 4
  %3312 = icmp eq i32 %3311, 0
  br i1 %3312, label %3313, label %3321

; <label>:3313:                                   ; preds = %3310
  %3314 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %3315 = bitcast %union.anon* %3314 to i8*
  %3316 = load i8, i8* %3315, align 8
  %3317 = trunc i8 %3316 to i1
  %3318 = zext i1 %3317 to i32
  %3319 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %3320 = bitcast %union.anon* %3319 to i32*
  store i32 %3318, i32* %3320, align 8
  br label %3588

; <label>:3321:                                   ; preds = %3310
  %3322 = load i32, i32* %731, align 4
  %3323 = icmp eq i32 %3322, 1
  br i1 %3323, label %3324, label %3332

; <label>:3324:                                   ; preds = %3321
  %3325 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %3326 = bitcast %union.anon* %3325 to i8*
  %3327 = load i8, i8* %3326, align 8
  %3328 = trunc i8 %3327 to i1
  %3329 = zext i1 %3328 to i64
  %3330 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %3331 = bitcast %union.anon* %3330 to i64*
  store i64 %3329, i64* %3331, align 8
  br label %3587

; <label>:3332:                                   ; preds = %3321
  %3333 = load i32, i32* %731, align 4
  %3334 = icmp eq i32 %3333, 2
  br i1 %3334, label %3335, label %3343

; <label>:3335:                                   ; preds = %3332
  %3336 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %3337 = bitcast %union.anon* %3336 to i8*
  %3338 = load i8, i8* %3337, align 8
  %3339 = trunc i8 %3338 to i1
  %3340 = uitofp i1 %3339 to float
  %3341 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %3342 = bitcast %union.anon* %3341 to float*
  store float %3340, float* %3342, align 8
  br label %3586

; <label>:3343:                                   ; preds = %3332
  %3344 = load i32, i32* %731, align 4
  %3345 = icmp eq i32 %3344, 3
  br i1 %3345, label %3346, label %3354

; <label>:3346:                                   ; preds = %3343
  %3347 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %3348 = bitcast %union.anon* %3347 to i8*
  %3349 = load i8, i8* %3348, align 8
  %3350 = trunc i8 %3349 to i1
  %3351 = uitofp i1 %3350 to double
  %3352 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %3353 = bitcast %union.anon* %3352 to double*
  store double %3351, double* %3353, align 8
  br label %3585

; <label>:3354:                                   ; preds = %3343
  %3355 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %3356 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3355, i32 0, i32 0
  %3357 = load i32, i32* %3356, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %780, i32 %3357)
  store i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str.5, i32 0, i32 0), i8** %132, align 8, !noalias !65
  store %"class.std::__1::basic_string"* %780, %"class.std::__1::basic_string"** %133, align 8, !noalias !65
  %3358 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %133, align 8, !noalias !65
  %3359 = load i8*, i8** %132, align 8, !noalias !65
  %3360 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %3358, i64 0, i8* %3359)
          to label %3361 unwind label %3561

; <label>:3361:                                   ; preds = %3354
  store %"class.std::__1::basic_string"* %3360, %"class.std::__1::basic_string"** %131, align 8, !noalias !65
  %3362 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %131, align 8, !noalias !65
  store %"class.std::__1::basic_string"* %779, %"class.std::__1::basic_string"** %129, align 8, !noalias !65
  store %"class.std::__1::basic_string"* %3362, %"class.std::__1::basic_string"** %130, align 8, !noalias !65
  %3363 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %129, align 8, !noalias !65
  %3364 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %130, align 8, !noalias !65
  store %"class.std::__1::basic_string"* %3363, %"class.std::__1::basic_string"** %127, align 8, !noalias !65
  store %"class.std::__1::basic_string"* %3364, %"class.std::__1::basic_string"** %128, align 8, !noalias !65
  %3365 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %127, align 8, !noalias !65
  %3366 = bitcast %"class.std::__1::basic_string"* %3365 to %"class.std::__1::__basic_string_common"*
  %3367 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3365, i32 0, i32 0
  %3368 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %128, align 8, !noalias !65
  %3369 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3368, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3369, %"class.std::__1::__compressed_pair"** %126, align 8, !noalias !65
  %3370 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %126, align 8, !noalias !65
  %3371 = bitcast %"class.std::__1::__compressed_pair"* %3367 to i8*
  %3372 = bitcast %"class.std::__1::__compressed_pair"* %3370 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %3371, i8* %3372, i64 24, i32 8, i1 false) #8
  %3373 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %128, align 8, !noalias !65
  store %"class.std::__1::basic_string"* %3373, %"class.std::__1::basic_string"** %123, align 8, !noalias !65
  %3374 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %123, align 8, !noalias !65
  %3375 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3374, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3375, %"class.std::__1::__compressed_pair"** %122, align 8, !noalias !65
  %3376 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %122, align 8, !noalias !65
  %3377 = bitcast %"class.std::__1::__compressed_pair"* %3376 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3377, %"class.std::__1::__libcpp_compressed_pair_imp"** %121, align 8, !noalias !65
  %3378 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %121, align 8, !noalias !65
  %3379 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3378, i32 0, i32 0
  %3380 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3379, i32 0, i32 0
  %3381 = bitcast %union.anon.0* %3380 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %3382 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %3381, i32 0, i32 0
  store [3 x i64]* %3382, [3 x i64]** %124, align 8, !noalias !65
  store i32 0, i32* %125, align 4, !noalias !65
  br label %3383

; <label>:3383:                                   ; preds = %3386, %3361
  %3384 = load i32, i32* %125, align 4, !noalias !65
  %3385 = icmp ult i32 %3384, 3
  br i1 %3385, label %3386, label %3393

; <label>:3386:                                   ; preds = %3383
  %3387 = load i32, i32* %125, align 4, !noalias !65
  %3388 = zext i32 %3387 to i64
  %3389 = load [3 x i64]*, [3 x i64]** %124, align 8, !noalias !65
  %3390 = getelementptr inbounds [3 x i64], [3 x i64]* %3389, i64 0, i64 %3388
  store i64 0, i64* %3390, align 8
  %3391 = load i32, i32* %125, align 4, !noalias !65
  %3392 = add i32 %3391, 1
  store i32 %3392, i32* %125, align 4, !noalias !65
  br label %3383

; <label>:3393:                                   ; preds = %3383
  br label %3394

; <label>:3394:                                   ; preds = %3393
  store %"class.std::__1::basic_string"* %779, %"class.std::__1::basic_string"** %119, align 8, !noalias !68
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.6, i32 0, i32 0), i8** %120, align 8, !noalias !68
  %3395 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %119, align 8, !noalias !68
  %3396 = load i8*, i8** %120, align 8, !noalias !68
  %3397 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %3395, i8* %3396)
          to label %3398 unwind label %3565

; <label>:3398:                                   ; preds = %3394
  store %"class.std::__1::basic_string"* %3397, %"class.std::__1::basic_string"** %118, align 8, !noalias !68
  %3399 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %118, align 8, !noalias !68
  store %"class.std::__1::basic_string"* %778, %"class.std::__1::basic_string"** %116, align 8, !noalias !68
  store %"class.std::__1::basic_string"* %3399, %"class.std::__1::basic_string"** %117, align 8, !noalias !68
  %3400 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %116, align 8, !noalias !68
  %3401 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %117, align 8, !noalias !68
  store %"class.std::__1::basic_string"* %3400, %"class.std::__1::basic_string"** %114, align 8, !noalias !68
  store %"class.std::__1::basic_string"* %3401, %"class.std::__1::basic_string"** %115, align 8, !noalias !68
  %3402 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %114, align 8, !noalias !68
  %3403 = bitcast %"class.std::__1::basic_string"* %3402 to %"class.std::__1::__basic_string_common"*
  %3404 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3402, i32 0, i32 0
  %3405 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %115, align 8, !noalias !68
  %3406 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3405, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3406, %"class.std::__1::__compressed_pair"** %113, align 8, !noalias !68
  %3407 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %113, align 8, !noalias !68
  %3408 = bitcast %"class.std::__1::__compressed_pair"* %3404 to i8*
  %3409 = bitcast %"class.std::__1::__compressed_pair"* %3407 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %3408, i8* %3409, i64 24, i32 8, i1 false) #8
  %3410 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %115, align 8, !noalias !68
  store %"class.std::__1::basic_string"* %3410, %"class.std::__1::basic_string"** %110, align 8, !noalias !68
  %3411 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %110, align 8, !noalias !68
  %3412 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3411, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3412, %"class.std::__1::__compressed_pair"** %109, align 8, !noalias !68
  %3413 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %109, align 8, !noalias !68
  %3414 = bitcast %"class.std::__1::__compressed_pair"* %3413 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3414, %"class.std::__1::__libcpp_compressed_pair_imp"** %108, align 8, !noalias !68
  %3415 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %108, align 8, !noalias !68
  %3416 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3415, i32 0, i32 0
  %3417 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3416, i32 0, i32 0
  %3418 = bitcast %union.anon.0* %3417 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %3419 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %3418, i32 0, i32 0
  store [3 x i64]* %3419, [3 x i64]** %111, align 8, !noalias !68
  store i32 0, i32* %112, align 4, !noalias !68
  br label %3420

; <label>:3420:                                   ; preds = %3423, %3398
  %3421 = load i32, i32* %112, align 4, !noalias !68
  %3422 = icmp ult i32 %3421, 3
  br i1 %3422, label %3423, label %3430

; <label>:3423:                                   ; preds = %3420
  %3424 = load i32, i32* %112, align 4, !noalias !68
  %3425 = zext i32 %3424 to i64
  %3426 = load [3 x i64]*, [3 x i64]** %111, align 8, !noalias !68
  %3427 = getelementptr inbounds [3 x i64], [3 x i64]* %3426, i64 0, i64 %3425
  store i64 0, i64* %3427, align 8
  %3428 = load i32, i32* %112, align 4, !noalias !68
  %3429 = add i32 %3428, 1
  store i32 %3429, i32* %112, align 4, !noalias !68
  br label %3420

; <label>:3430:                                   ; preds = %3420
  br label %3431

; <label>:3431:                                   ; preds = %3430
  %3432 = load i32, i32* %731, align 4
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %781, i32 %3432)
          to label %3433 unwind label %3569

; <label>:3433:                                   ; preds = %3431
  store %"class.std::__1::basic_string"* %778, %"class.std::__1::basic_string"** %106, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %781, %"class.std::__1::basic_string"** %107, align 8, !noalias !71
  %3434 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %106, align 8, !noalias !71
  %3435 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %107, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %3434, %"class.std::__1::basic_string"** %104, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %3435, %"class.std::__1::basic_string"** %105, align 8, !noalias !71
  %3436 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %104, align 8, !noalias !71
  %3437 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %105, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %3437, %"class.std::__1::basic_string"** %103, align 8, !noalias !71
  %3438 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %103, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %3438, %"class.std::__1::basic_string"** %102, align 8, !noalias !71
  %3439 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %102, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %3439, %"class.std::__1::basic_string"** %101, align 8, !noalias !71
  %3440 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %101, align 8, !noalias !71
  %3441 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3440, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3441, %"class.std::__1::__compressed_pair"** %100, align 8, !noalias !71
  %3442 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %100, align 8, !noalias !71
  %3443 = bitcast %"class.std::__1::__compressed_pair"* %3442 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3443, %"class.std::__1::__libcpp_compressed_pair_imp"** %99, align 8, !noalias !71
  %3444 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %99, align 8, !noalias !71
  %3445 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3444, i32 0, i32 0
  %3446 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3445, i32 0, i32 0
  %3447 = bitcast %union.anon.0* %3446 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %3448 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %3447, i32 0, i32 0
  %3449 = bitcast %union.anon.1* %3448 to i8*
  %3450 = load i8, i8* %3449, align 8, !noalias !71
  %3451 = zext i8 %3450 to i32
  %3452 = and i32 %3451, 1
  %3453 = icmp ne i32 %3452, 0
  br i1 %3453, label %3454, label %3465

; <label>:3454:                                   ; preds = %3433
  store %"class.std::__1::basic_string"* %3439, %"class.std::__1::basic_string"** %93, align 8, !noalias !71
  %3455 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %93, align 8, !noalias !71
  %3456 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3455, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3456, %"class.std::__1::__compressed_pair"** %92, align 8, !noalias !71
  %3457 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %92, align 8, !noalias !71
  %3458 = bitcast %"class.std::__1::__compressed_pair"* %3457 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3458, %"class.std::__1::__libcpp_compressed_pair_imp"** %91, align 8, !noalias !71
  %3459 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %91, align 8, !noalias !71
  %3460 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3459, i32 0, i32 0
  %3461 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3460, i32 0, i32 0
  %3462 = bitcast %union.anon.0* %3461 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %3463 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %3462, i32 0, i32 2
  %3464 = load i8*, i8** %3463, align 8
  br label %3478

; <label>:3465:                                   ; preds = %3433
  store %"class.std::__1::basic_string"* %3439, %"class.std::__1::basic_string"** %98, align 8, !noalias !71
  %3466 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %98, align 8, !noalias !71
  %3467 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3466, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3467, %"class.std::__1::__compressed_pair"** %97, align 8, !noalias !71
  %3468 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %97, align 8, !noalias !71
  %3469 = bitcast %"class.std::__1::__compressed_pair"* %3468 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3469, %"class.std::__1::__libcpp_compressed_pair_imp"** %96, align 8, !noalias !71
  %3470 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %96, align 8, !noalias !71
  %3471 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3470, i32 0, i32 0
  %3472 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3471, i32 0, i32 0
  %3473 = bitcast %union.anon.0* %3472 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %3474 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %3473, i32 0, i32 1
  %3475 = getelementptr inbounds [23 x i8], [23 x i8]* %3474, i64 0, i64 0
  store i8* %3475, i8** %95, align 8, !noalias !71
  %3476 = load i8*, i8** %95, align 8, !noalias !71
  store i8* %3476, i8** %94, align 8, !noalias !71
  %3477 = load i8*, i8** %94, align 8, !noalias !71
  br label %3478

; <label>:3478:                                   ; preds = %3465, %3454
  %3479 = phi i8* [ %3464, %3454 ], [ %3477, %3465 ]
  store i8* %3479, i8** %90, align 8, !noalias !71
  %3480 = load i8*, i8** %90, align 8, !noalias !71
  %3481 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %105, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %3481, %"class.std::__1::basic_string"** %89, align 8, !noalias !71
  %3482 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %89, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %3482, %"class.std::__1::basic_string"** %88, align 8, !noalias !71
  %3483 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %88, align 8, !noalias !71
  %3484 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3483, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3484, %"class.std::__1::__compressed_pair"** %87, align 8, !noalias !71
  %3485 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %87, align 8, !noalias !71
  %3486 = bitcast %"class.std::__1::__compressed_pair"* %3485 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3486, %"class.std::__1::__libcpp_compressed_pair_imp"** %86, align 8, !noalias !71
  %3487 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %86, align 8, !noalias !71
  %3488 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3487, i32 0, i32 0
  %3489 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3488, i32 0, i32 0
  %3490 = bitcast %union.anon.0* %3489 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %3491 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %3490, i32 0, i32 0
  %3492 = bitcast %union.anon.1* %3491 to i8*
  %3493 = load i8, i8* %3492, align 8, !noalias !71
  %3494 = zext i8 %3493 to i32
  %3495 = and i32 %3494, 1
  %3496 = icmp ne i32 %3495, 0
  br i1 %3496, label %3497, label %3508

; <label>:3497:                                   ; preds = %3478
  store %"class.std::__1::basic_string"* %3482, %"class.std::__1::basic_string"** %82, align 8, !noalias !71
  %3498 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %82, align 8, !noalias !71
  %3499 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3498, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3499, %"class.std::__1::__compressed_pair"** %81, align 8, !noalias !71
  %3500 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %81, align 8, !noalias !71
  %3501 = bitcast %"class.std::__1::__compressed_pair"* %3500 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3501, %"class.std::__1::__libcpp_compressed_pair_imp"** %80, align 8, !noalias !71
  %3502 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %80, align 8, !noalias !71
  %3503 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3502, i32 0, i32 0
  %3504 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3503, i32 0, i32 0
  %3505 = bitcast %union.anon.0* %3504 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %3506 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %3505, i32 0, i32 1
  %3507 = load i64, i64* %3506, align 8
  br label %3523

; <label>:3508:                                   ; preds = %3478
  store %"class.std::__1::basic_string"* %3482, %"class.std::__1::basic_string"** %85, align 8, !noalias !71
  %3509 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %85, align 8, !noalias !71
  %3510 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3509, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3510, %"class.std::__1::__compressed_pair"** %84, align 8, !noalias !71
  %3511 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %84, align 8, !noalias !71
  %3512 = bitcast %"class.std::__1::__compressed_pair"* %3511 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3512, %"class.std::__1::__libcpp_compressed_pair_imp"** %83, align 8, !noalias !71
  %3513 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %83, align 8, !noalias !71
  %3514 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3513, i32 0, i32 0
  %3515 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3514, i32 0, i32 0
  %3516 = bitcast %union.anon.0* %3515 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %3517 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %3516, i32 0, i32 0
  %3518 = bitcast %union.anon.1* %3517 to i8*
  %3519 = load i8, i8* %3518, align 8
  %3520 = zext i8 %3519 to i32
  %3521 = ashr i32 %3520, 1
  %3522 = sext i32 %3521 to i64
  br label %3523

; <label>:3523:                                   ; preds = %3508, %3497
  %3524 = phi i64 [ %3507, %3497 ], [ %3522, %3508 ]
  %3525 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %3436, i8* %3480, i64 %3524)
          to label %3526 unwind label %3573

; <label>:3526:                                   ; preds = %3523
  store %"class.std::__1::basic_string"* %3525, %"class.std::__1::basic_string"** %79, align 8, !noalias !71
  %3527 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %79, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %777, %"class.std::__1::basic_string"** %77, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %3527, %"class.std::__1::basic_string"** %78, align 8, !noalias !71
  %3528 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %77, align 8, !noalias !71
  %3529 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %78, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %3528, %"class.std::__1::basic_string"** %75, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %3529, %"class.std::__1::basic_string"** %76, align 8, !noalias !71
  %3530 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %75, align 8, !noalias !71
  %3531 = bitcast %"class.std::__1::basic_string"* %3530 to %"class.std::__1::__basic_string_common"*
  %3532 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3530, i32 0, i32 0
  %3533 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %76, align 8, !noalias !71
  %3534 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3533, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3534, %"class.std::__1::__compressed_pair"** %74, align 8, !noalias !71
  %3535 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %74, align 8, !noalias !71
  %3536 = bitcast %"class.std::__1::__compressed_pair"* %3532 to i8*
  %3537 = bitcast %"class.std::__1::__compressed_pair"* %3535 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %3536, i8* %3537, i64 24, i32 8, i1 false) #8
  %3538 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %76, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %3538, %"class.std::__1::basic_string"** %71, align 8, !noalias !71
  %3539 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %71, align 8, !noalias !71
  %3540 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3539, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3540, %"class.std::__1::__compressed_pair"** %70, align 8, !noalias !71
  %3541 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %70, align 8, !noalias !71
  %3542 = bitcast %"class.std::__1::__compressed_pair"* %3541 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3542, %"class.std::__1::__libcpp_compressed_pair_imp"** %69, align 8, !noalias !71
  %3543 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %69, align 8, !noalias !71
  %3544 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3543, i32 0, i32 0
  %3545 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3544, i32 0, i32 0
  %3546 = bitcast %union.anon.0* %3545 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %3547 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %3546, i32 0, i32 0
  store [3 x i64]* %3547, [3 x i64]** %72, align 8, !noalias !71
  store i32 0, i32* %73, align 4, !noalias !71
  br label %3548

; <label>:3548:                                   ; preds = %3551, %3526
  %3549 = load i32, i32* %73, align 4, !noalias !71
  %3550 = icmp ult i32 %3549, 3
  br i1 %3550, label %3551, label %3558

; <label>:3551:                                   ; preds = %3548
  %3552 = load i32, i32* %73, align 4, !noalias !71
  %3553 = zext i32 %3552 to i64
  %3554 = load [3 x i64]*, [3 x i64]** %72, align 8, !noalias !71
  %3555 = getelementptr inbounds [3 x i64], [3 x i64]* %3554, i64 0, i64 %3553
  store i64 0, i64* %3555, align 8
  %3556 = load i32, i32* %73, align 4, !noalias !71
  %3557 = add i32 %3556, 1
  store i32 %3557, i32* %73, align 4, !noalias !71
  br label %3548

; <label>:3558:                                   ; preds = %3548
  br label %3559

; <label>:3559:                                   ; preds = %3558
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %777, %class.AstNode* null)
          to label %3560 unwind label %3577

; <label>:3560:                                   ; preds = %3559
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %777) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %781) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %778) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %779) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %780) #8
  br label %3585

; <label>:3561:                                   ; preds = %3354
  %3562 = landingpad { i8*, i32 }
          cleanup
  %3563 = extractvalue { i8*, i32 } %3562, 0
  store i8* %3563, i8** %734, align 8
  %3564 = extractvalue { i8*, i32 } %3562, 1
  store i32 %3564, i32* %735, align 4
  br label %3584

; <label>:3565:                                   ; preds = %3394
  %3566 = landingpad { i8*, i32 }
          cleanup
  %3567 = extractvalue { i8*, i32 } %3566, 0
  store i8* %3567, i8** %734, align 8
  %3568 = extractvalue { i8*, i32 } %3566, 1
  store i32 %3568, i32* %735, align 4
  br label %3583

; <label>:3569:                                   ; preds = %3431
  %3570 = landingpad { i8*, i32 }
          cleanup
  %3571 = extractvalue { i8*, i32 } %3570, 0
  store i8* %3571, i8** %734, align 8
  %3572 = extractvalue { i8*, i32 } %3570, 1
  store i32 %3572, i32* %735, align 4
  br label %3582

; <label>:3573:                                   ; preds = %3523
  %3574 = landingpad { i8*, i32 }
          cleanup
  %3575 = extractvalue { i8*, i32 } %3574, 0
  store i8* %3575, i8** %734, align 8
  %3576 = extractvalue { i8*, i32 } %3574, 1
  store i32 %3576, i32* %735, align 4
  br label %3581

; <label>:3577:                                   ; preds = %3559
  %3578 = landingpad { i8*, i32 }
          cleanup
  %3579 = extractvalue { i8*, i32 } %3578, 0
  store i8* %3579, i8** %734, align 8
  %3580 = extractvalue { i8*, i32 } %3578, 1
  store i32 %3580, i32* %735, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %777) #8
  br label %3581

; <label>:3581:                                   ; preds = %3577, %3573
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %781) #8
  br label %3582

; <label>:3582:                                   ; preds = %3581, %3569
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %778) #8
  br label %3583

; <label>:3583:                                   ; preds = %3582, %3565
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %779) #8
  br label %3584

; <label>:3584:                                   ; preds = %3583, %3561
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %780) #8
  br label %3827

; <label>:3585:                                   ; preds = %3560, %3346
  br label %3586

; <label>:3586:                                   ; preds = %3585, %3335
  br label %3587

; <label>:3587:                                   ; preds = %3586, %3324
  br label %3588

; <label>:3588:                                   ; preds = %3587, %3313
  br label %3589

; <label>:3589:                                   ; preds = %3588, %3302
  br label %3590

; <label>:3590:                                   ; preds = %3589, %3295
  br label %3798

; <label>:3591:                                   ; preds = %796
  %3592 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %3593 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3592, i32 0, i32 0
  %3594 = load i32, i32* %3593, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %785, i32 %3594)
  store i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str.5, i32 0, i32 0), i8** %67, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %785, %"class.std::__1::basic_string"** %68, align 8, !noalias !74
  %3595 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !74
  %3596 = load i8*, i8** %67, align 8, !noalias !74
  %3597 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %3595, i64 0, i8* %3596)
          to label %3598 unwind label %3802

; <label>:3598:                                   ; preds = %3591
  store %"class.std::__1::basic_string"* %3597, %"class.std::__1::basic_string"** %66, align 8, !noalias !74
  %3599 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %784, %"class.std::__1::basic_string"** %64, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %3599, %"class.std::__1::basic_string"** %65, align 8, !noalias !74
  %3600 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !74
  %3601 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %3600, %"class.std::__1::basic_string"** %62, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %3601, %"class.std::__1::basic_string"** %63, align 8, !noalias !74
  %3602 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8, !noalias !74
  %3603 = bitcast %"class.std::__1::basic_string"* %3602 to %"class.std::__1::__basic_string_common"*
  %3604 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3602, i32 0, i32 0
  %3605 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !74
  %3606 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3605, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3606, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !74
  %3607 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !74
  %3608 = bitcast %"class.std::__1::__compressed_pair"* %3604 to i8*
  %3609 = bitcast %"class.std::__1::__compressed_pair"* %3607 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %3608, i8* %3609, i64 24, i32 8, i1 false) #8
  %3610 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %3610, %"class.std::__1::basic_string"** %58, align 8, !noalias !74
  %3611 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8, !noalias !74
  %3612 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3611, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3612, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !74
  %3613 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !74
  %3614 = bitcast %"class.std::__1::__compressed_pair"* %3613 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3614, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !74
  %3615 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !74
  %3616 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3615, i32 0, i32 0
  %3617 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3616, i32 0, i32 0
  %3618 = bitcast %union.anon.0* %3617 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %3619 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %3618, i32 0, i32 0
  store [3 x i64]* %3619, [3 x i64]** %59, align 8, !noalias !74
  store i32 0, i32* %60, align 4, !noalias !74
  br label %3620

; <label>:3620:                                   ; preds = %3623, %3598
  %3621 = load i32, i32* %60, align 4, !noalias !74
  %3622 = icmp ult i32 %3621, 3
  br i1 %3622, label %3623, label %3630

; <label>:3623:                                   ; preds = %3620
  %3624 = load i32, i32* %60, align 4, !noalias !74
  %3625 = zext i32 %3624 to i64
  %3626 = load [3 x i64]*, [3 x i64]** %59, align 8, !noalias !74
  %3627 = getelementptr inbounds [3 x i64], [3 x i64]* %3626, i64 0, i64 %3625
  store i64 0, i64* %3627, align 8
  %3628 = load i32, i32* %60, align 4, !noalias !74
  %3629 = add i32 %3628, 1
  store i32 %3629, i32* %60, align 4, !noalias !74
  br label %3620

; <label>:3630:                                   ; preds = %3620
  br label %3631

; <label>:3631:                                   ; preds = %3630
  store %"class.std::__1::basic_string"* %784, %"class.std::__1::basic_string"** %54, align 8, !noalias !77
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.6, i32 0, i32 0), i8** %55, align 8, !noalias !77
  %3632 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !77
  %3633 = load i8*, i8** %55, align 8, !noalias !77
  %3634 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %3632, i8* %3633)
          to label %3635 unwind label %3806

; <label>:3635:                                   ; preds = %3631
  store %"class.std::__1::basic_string"* %3634, %"class.std::__1::basic_string"** %53, align 8, !noalias !77
  %3636 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !77
  store %"class.std::__1::basic_string"* %783, %"class.std::__1::basic_string"** %51, align 8, !noalias !77
  store %"class.std::__1::basic_string"* %3636, %"class.std::__1::basic_string"** %52, align 8, !noalias !77
  %3637 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !77
  %3638 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !77
  store %"class.std::__1::basic_string"* %3637, %"class.std::__1::basic_string"** %49, align 8, !noalias !77
  store %"class.std::__1::basic_string"* %3638, %"class.std::__1::basic_string"** %50, align 8, !noalias !77
  %3639 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !77
  %3640 = bitcast %"class.std::__1::basic_string"* %3639 to %"class.std::__1::__basic_string_common"*
  %3641 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3639, i32 0, i32 0
  %3642 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !77
  %3643 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3642, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3643, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !77
  %3644 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !77
  %3645 = bitcast %"class.std::__1::__compressed_pair"* %3641 to i8*
  %3646 = bitcast %"class.std::__1::__compressed_pair"* %3644 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %3645, i8* %3646, i64 24, i32 8, i1 false) #8
  %3647 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !77
  store %"class.std::__1::basic_string"* %3647, %"class.std::__1::basic_string"** %45, align 8, !noalias !77
  %3648 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !77
  %3649 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3648, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3649, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !77
  %3650 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !77
  %3651 = bitcast %"class.std::__1::__compressed_pair"* %3650 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3651, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !77
  %3652 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !77
  %3653 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3652, i32 0, i32 0
  %3654 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3653, i32 0, i32 0
  %3655 = bitcast %union.anon.0* %3654 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %3656 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %3655, i32 0, i32 0
  store [3 x i64]* %3656, [3 x i64]** %46, align 8, !noalias !77
  store i32 0, i32* %47, align 4, !noalias !77
  br label %3657

; <label>:3657:                                   ; preds = %3660, %3635
  %3658 = load i32, i32* %47, align 4, !noalias !77
  %3659 = icmp ult i32 %3658, 3
  br i1 %3659, label %3660, label %3667

; <label>:3660:                                   ; preds = %3657
  %3661 = load i32, i32* %47, align 4, !noalias !77
  %3662 = zext i32 %3661 to i64
  %3663 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !77
  %3664 = getelementptr inbounds [3 x i64], [3 x i64]* %3663, i64 0, i64 %3662
  store i64 0, i64* %3664, align 8
  %3665 = load i32, i32* %47, align 4, !noalias !77
  %3666 = add i32 %3665, 1
  store i32 %3666, i32* %47, align 4, !noalias !77
  br label %3657

; <label>:3667:                                   ; preds = %3657
  br label %3668

; <label>:3668:                                   ; preds = %3667
  %3669 = load i32, i32* %731, align 4
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %786, i32 %3669)
          to label %3670 unwind label %3810

; <label>:3670:                                   ; preds = %3668
  store %"class.std::__1::basic_string"* %783, %"class.std::__1::basic_string"** %41, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %786, %"class.std::__1::basic_string"** %42, align 8, !noalias !80
  %3671 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !80
  %3672 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %3671, %"class.std::__1::basic_string"** %39, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %3672, %"class.std::__1::basic_string"** %40, align 8, !noalias !80
  %3673 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !80
  %3674 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %3674, %"class.std::__1::basic_string"** %38, align 8, !noalias !80
  %3675 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %3675, %"class.std::__1::basic_string"** %37, align 8, !noalias !80
  %3676 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %3676, %"class.std::__1::basic_string"** %36, align 8, !noalias !80
  %3677 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !80
  %3678 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3677, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3678, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !80
  %3679 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !80
  %3680 = bitcast %"class.std::__1::__compressed_pair"* %3679 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3680, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !80
  %3681 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !80
  %3682 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3681, i32 0, i32 0
  %3683 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3682, i32 0, i32 0
  %3684 = bitcast %union.anon.0* %3683 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %3685 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %3684, i32 0, i32 0
  %3686 = bitcast %union.anon.1* %3685 to i8*
  %3687 = load i8, i8* %3686, align 8, !noalias !80
  %3688 = zext i8 %3687 to i32
  %3689 = and i32 %3688, 1
  %3690 = icmp ne i32 %3689, 0
  br i1 %3690, label %3691, label %3702

; <label>:3691:                                   ; preds = %3670
  store %"class.std::__1::basic_string"* %3676, %"class.std::__1::basic_string"** %28, align 8, !noalias !80
  %3692 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !80
  %3693 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3692, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3693, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !80
  %3694 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !80
  %3695 = bitcast %"class.std::__1::__compressed_pair"* %3694 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3695, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !80
  %3696 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !80
  %3697 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3696, i32 0, i32 0
  %3698 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3697, i32 0, i32 0
  %3699 = bitcast %union.anon.0* %3698 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %3700 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %3699, i32 0, i32 2
  %3701 = load i8*, i8** %3700, align 8
  br label %3715

; <label>:3702:                                   ; preds = %3670
  store %"class.std::__1::basic_string"* %3676, %"class.std::__1::basic_string"** %33, align 8, !noalias !80
  %3703 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !80
  %3704 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3703, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3704, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !80
  %3705 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !80
  %3706 = bitcast %"class.std::__1::__compressed_pair"* %3705 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3706, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !80
  %3707 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !80
  %3708 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3707, i32 0, i32 0
  %3709 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3708, i32 0, i32 0
  %3710 = bitcast %union.anon.0* %3709 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %3711 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %3710, i32 0, i32 1
  %3712 = getelementptr inbounds [23 x i8], [23 x i8]* %3711, i64 0, i64 0
  store i8* %3712, i8** %30, align 8, !noalias !80
  %3713 = load i8*, i8** %30, align 8, !noalias !80
  store i8* %3713, i8** %29, align 8, !noalias !80
  %3714 = load i8*, i8** %29, align 8, !noalias !80
  br label %3715

; <label>:3715:                                   ; preds = %3702, %3691
  %3716 = phi i8* [ %3701, %3691 ], [ %3714, %3702 ]
  store i8* %3716, i8** %25, align 8, !noalias !80
  %3717 = load i8*, i8** %25, align 8, !noalias !80
  %3718 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %3718, %"class.std::__1::basic_string"** %24, align 8, !noalias !80
  %3719 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %3719, %"class.std::__1::basic_string"** %23, align 8, !noalias !80
  %3720 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !80
  %3721 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3720, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3721, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !80
  %3722 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !80
  %3723 = bitcast %"class.std::__1::__compressed_pair"* %3722 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3723, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !80
  %3724 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !80
  %3725 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3724, i32 0, i32 0
  %3726 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3725, i32 0, i32 0
  %3727 = bitcast %union.anon.0* %3726 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %3728 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %3727, i32 0, i32 0
  %3729 = bitcast %union.anon.1* %3728 to i8*
  %3730 = load i8, i8* %3729, align 8, !noalias !80
  %3731 = zext i8 %3730 to i32
  %3732 = and i32 %3731, 1
  %3733 = icmp ne i32 %3732, 0
  br i1 %3733, label %3734, label %3745

; <label>:3734:                                   ; preds = %3715
  store %"class.std::__1::basic_string"* %3719, %"class.std::__1::basic_string"** %17, align 8, !noalias !80
  %3735 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !80
  %3736 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3735, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3736, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !80
  %3737 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !80
  %3738 = bitcast %"class.std::__1::__compressed_pair"* %3737 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3738, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !80
  %3739 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !80
  %3740 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3739, i32 0, i32 0
  %3741 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3740, i32 0, i32 0
  %3742 = bitcast %union.anon.0* %3741 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %3743 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %3742, i32 0, i32 1
  %3744 = load i64, i64* %3743, align 8
  br label %3760

; <label>:3745:                                   ; preds = %3715
  store %"class.std::__1::basic_string"* %3719, %"class.std::__1::basic_string"** %20, align 8, !noalias !80
  %3746 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !80
  %3747 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3746, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3747, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !80
  %3748 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !80
  %3749 = bitcast %"class.std::__1::__compressed_pair"* %3748 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3749, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !80
  %3750 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !80
  %3751 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3750, i32 0, i32 0
  %3752 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3751, i32 0, i32 0
  %3753 = bitcast %union.anon.0* %3752 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %3754 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %3753, i32 0, i32 0
  %3755 = bitcast %union.anon.1* %3754 to i8*
  %3756 = load i8, i8* %3755, align 8
  %3757 = zext i8 %3756 to i32
  %3758 = ashr i32 %3757, 1
  %3759 = sext i32 %3758 to i64
  br label %3760

; <label>:3760:                                   ; preds = %3745, %3734
  %3761 = phi i64 [ %3744, %3734 ], [ %3759, %3745 ]
  %3762 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %3673, i8* %3717, i64 %3761)
          to label %3763 unwind label %3814

; <label>:3763:                                   ; preds = %3760
  store %"class.std::__1::basic_string"* %3762, %"class.std::__1::basic_string"** %14, align 8, !noalias !80
  %3764 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %782, %"class.std::__1::basic_string"** %12, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %3764, %"class.std::__1::basic_string"** %13, align 8, !noalias !80
  %3765 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !80
  %3766 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %3765, %"class.std::__1::basic_string"** %10, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %3766, %"class.std::__1::basic_string"** %11, align 8, !noalias !80
  %3767 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !80
  %3768 = bitcast %"class.std::__1::basic_string"* %3767 to %"class.std::__1::__basic_string_common"*
  %3769 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3767, i32 0, i32 0
  %3770 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !80
  %3771 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3770, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3771, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !80
  %3772 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !80
  %3773 = bitcast %"class.std::__1::__compressed_pair"* %3769 to i8*
  %3774 = bitcast %"class.std::__1::__compressed_pair"* %3772 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %3773, i8* %3774, i64 24, i32 8, i1 false) #8
  %3775 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %3775, %"class.std::__1::basic_string"** %6, align 8, !noalias !80
  %3776 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !80
  %3777 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %3776, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %3777, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !80
  %3778 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !80
  %3779 = bitcast %"class.std::__1::__compressed_pair"* %3778 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %3779, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !80
  %3780 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !80
  %3781 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %3780, i32 0, i32 0
  %3782 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %3781, i32 0, i32 0
  %3783 = bitcast %union.anon.0* %3782 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %3784 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %3783, i32 0, i32 0
  store [3 x i64]* %3784, [3 x i64]** %7, align 8, !noalias !80
  store i32 0, i32* %8, align 4, !noalias !80
  br label %3785

; <label>:3785:                                   ; preds = %3788, %3763
  %3786 = load i32, i32* %8, align 4, !noalias !80
  %3787 = icmp ult i32 %3786, 3
  br i1 %3787, label %3788, label %3795

; <label>:3788:                                   ; preds = %3785
  %3789 = load i32, i32* %8, align 4, !noalias !80
  %3790 = zext i32 %3789 to i64
  %3791 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !80
  %3792 = getelementptr inbounds [3 x i64], [3 x i64]* %3791, i64 0, i64 %3790
  store i64 0, i64* %3792, align 8
  %3793 = load i32, i32* %8, align 4, !noalias !80
  %3794 = add i32 %3793, 1
  store i32 %3794, i32* %8, align 4, !noalias !80
  br label %3785

; <label>:3795:                                   ; preds = %3785
  br label %3796

; <label>:3796:                                   ; preds = %3795
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %782, %class.AstNode* null)
          to label %3797 unwind label %3818

; <label>:3797:                                   ; preds = %3796
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %782) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %786) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %783) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %784) #8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %785) #8
  br label %3798

; <label>:3798:                                   ; preds = %3797, %3590, %3238, %2556, %1918, %1479
  %3799 = load i32, i32* %731, align 4
  %3800 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %3801 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3800, i32 0, i32 0
  store i32 %3799, i32* %3801, align 8
  call void @_ZN7Variant13check_numericEv(%struct.Variant* %1)
  call void @_ZN7VariantC1ERKS_(%struct.Variant* %0, %struct.Variant* dereferenceable(264) %1)
  br label %3826

; <label>:3802:                                   ; preds = %3591
  %3803 = landingpad { i8*, i32 }
          cleanup
  %3804 = extractvalue { i8*, i32 } %3803, 0
  store i8* %3804, i8** %734, align 8
  %3805 = extractvalue { i8*, i32 } %3803, 1
  store i32 %3805, i32* %735, align 4
  br label %3825

; <label>:3806:                                   ; preds = %3631
  %3807 = landingpad { i8*, i32 }
          cleanup
  %3808 = extractvalue { i8*, i32 } %3807, 0
  store i8* %3808, i8** %734, align 8
  %3809 = extractvalue { i8*, i32 } %3807, 1
  store i32 %3809, i32* %735, align 4
  br label %3824

; <label>:3810:                                   ; preds = %3668
  %3811 = landingpad { i8*, i32 }
          cleanup
  %3812 = extractvalue { i8*, i32 } %3811, 0
  store i8* %3812, i8** %734, align 8
  %3813 = extractvalue { i8*, i32 } %3811, 1
  store i32 %3813, i32* %735, align 4
  br label %3823

; <label>:3814:                                   ; preds = %3760
  %3815 = landingpad { i8*, i32 }
          cleanup
  %3816 = extractvalue { i8*, i32 } %3815, 0
  store i8* %3816, i8** %734, align 8
  %3817 = extractvalue { i8*, i32 } %3815, 1
  store i32 %3817, i32* %735, align 4
  br label %3822

; <label>:3818:                                   ; preds = %3796
  %3819 = landingpad { i8*, i32 }
          cleanup
  %3820 = extractvalue { i8*, i32 } %3819, 0
  store i8* %3820, i8** %734, align 8
  %3821 = extractvalue { i8*, i32 } %3819, 1
  store i32 %3821, i32* %735, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %782) #8
  br label %3822

; <label>:3822:                                   ; preds = %3818, %3814
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %786) #8
  br label %3823

; <label>:3823:                                   ; preds = %3822, %3810
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %783) #8
  br label %3824

; <label>:3824:                                   ; preds = %3823, %3806
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %784) #8
  br label %3825

; <label>:3825:                                   ; preds = %3824, %3802
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %785) #8
  br label %3827

; <label>:3826:                                   ; preds = %3798, %795
  ret void

; <label>:3827:                                   ; preds = %3825, %3584, %3231, %2872, %2551, %2234, %1912, %1473, %1216, %1152, %1138, %1074, %1060, %996, %982, %918, %898
  %3828 = load i8*, i8** %734, align 8
  %3829 = load i32, i32* %735, align 4
  %3830 = insertvalue { i8*, i32 } undef, i8* %3828, 0
  %3831 = insertvalue { i8*, i32 } %3830, i32 %3829, 1
  resume { i8*, i32 } %3831

; <label>:3832:                                   ; preds = %1215, %1137, %1059, %981
  %3833 = landingpad { i8*, i32 }
          catch i8* null
  %3834 = extractvalue { i8*, i32 } %3833, 0
  call void @__clang_call_terminate(i8* %3834) #9
  unreachable
}

declare void @_ZN7VariantC1ERKS_(%struct.Variant*, %struct.Variant* dereferenceable(264)) unnamed_addr #1

declare void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32, %"class.std::__1::basic_string"*, %class.AstNode*) #1

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
  call void @llvm.memset.p0i8.i64(i8* %83, i8 0, i64 24, i32 8, i1 false) #8
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
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %0) #8
  br label %211

; <label>:209:                                    ; preds = %203
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %0) #8
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

declare i32 @__gxx_personality_v0(...)

; Function Attrs: nounwind
declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"*) unnamed_addr #2

declare dereferenceable(1) i8* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE2atEm(%"class.std::__1::basic_string"*, i64) #1

declare i32 @_ZNSt3__14stoiERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPmi(%"class.std::__1::basic_string"* dereferenceable(24), i64*, i32) #1

; Function Attrs: nounwind readnone
declare i32 @llvm.eh.typeid.for(i8*) #3

declare i8* @__cxa_get_exception_ptr(i8*)

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt16invalid_argumentC1ERKS_(%"class.std::invalid_argument"*, %"class.std::invalid_argument"* dereferenceable(16)) unnamed_addr #4 align 2 {
  %3 = alloca %"class.std::invalid_argument"*, align 8
  %4 = alloca %"class.std::invalid_argument"*, align 8
  store %"class.std::invalid_argument"* %0, %"class.std::invalid_argument"** %3, align 8
  store %"class.std::invalid_argument"* %1, %"class.std::invalid_argument"** %4, align 8
  %5 = load %"class.std::invalid_argument"*, %"class.std::invalid_argument"** %3, align 8
  %6 = load %"class.std::invalid_argument"*, %"class.std::invalid_argument"** %4, align 8
  call void @_ZNSt16invalid_argumentC2ERKS_(%"class.std::invalid_argument"* %5, %"class.std::invalid_argument"* dereferenceable(16) %6) #8
  ret void
}

declare i8* @__cxa_begin_catch(i8*)

; Function Attrs: nounwind
declare void @_ZNSt16invalid_argumentD1Ev(%"class.std::invalid_argument"*) unnamed_addr #2

declare void @__cxa_end_catch()

; Function Attrs: noinline noreturn nounwind
define linkonce_odr hidden void @__clang_call_terminate(i8*) #5 {
  %2 = call i8* @__cxa_begin_catch(i8* %0) #8
  call void @_ZSt9terminatev() #9
  unreachable
}

declare void @_ZSt9terminatev()

declare i64 @_ZNSt3__14stolERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPmi(%"class.std::__1::basic_string"* dereferenceable(24), i64*, i32) #1

declare float @_ZNSt3__14stofERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPm(%"class.std::__1::basic_string"* dereferenceable(24), i64*) #1

declare double @_ZNSt3__14stodERKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEPm(%"class.std::__1::basic_string"* dereferenceable(24), i64*) #1

declare void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret, i32) #1

declare void @_ZNSt3__19to_stringEl(%"class.std::__1::basic_string"* sret, i64) #1

declare void @_ZNSt3__19to_stringEd(%"class.std::__1::basic_string"* sret, double) #1

declare void @_ZN7Variant13check_numericEv(%struct.Variant*) #1

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt16invalid_argumentC2ERKS_(%"class.std::invalid_argument"*, %"class.std::invalid_argument"* dereferenceable(16)) unnamed_addr #4 align 2 {
  %3 = alloca %"class.std::invalid_argument"*, align 8
  %4 = alloca %"class.std::invalid_argument"*, align 8
  store %"class.std::invalid_argument"* %0, %"class.std::invalid_argument"** %3, align 8
  store %"class.std::invalid_argument"* %1, %"class.std::invalid_argument"** %4, align 8
  %5 = load %"class.std::invalid_argument"*, %"class.std::invalid_argument"** %3, align 8
  %6 = bitcast %"class.std::invalid_argument"* %5 to %"class.std::logic_error"*
  %7 = load %"class.std::invalid_argument"*, %"class.std::invalid_argument"** %4, align 8
  %8 = bitcast %"class.std::invalid_argument"* %7 to %"class.std::logic_error"*
  call void @_ZNSt11logic_errorC2ERKS_(%"class.std::logic_error"* %6, %"class.std::logic_error"* dereferenceable(16) %8) #8
  %9 = bitcast %"class.std::invalid_argument"* %5 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([5 x i8*], [5 x i8*]* @_ZTVSt16invalid_argument, i32 0, i32 2) to i32 (...)**), i32 (...)*** %9, align 8
  ret void
}

; Function Attrs: nounwind
declare void @_ZNSt11logic_errorC2ERKS_(%"class.std::logic_error"*, %"class.std::logic_error"* dereferenceable(16)) unnamed_addr #2

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8*) #6 align 2 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = load i8*, i8** %2, align 8
  %4 = call i64 @strlen(i8* %3)
  ret i64 %4
}

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEPKcmm(%"class.std::__1::basic_string"*, i8*, i64, i64) #1

declare dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"*, i8*, i64) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #7

declare i64 @strlen(i8*) #1

declare dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"*, i8*) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #7

; Function Attrs: nounwind
declare i32 @_ZNKSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7compareEPKc(%"class.std::__1::basic_string"*, i8*) #2

declare dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"*, i64, i8*) #1

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEmc(%"class.std::__1::basic_string"*, i64, i8 signext) #1

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1), i8* dereferenceable(1)) #4 align 2 {
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

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7reserveEm(%"class.std::__1::basic_string"*, i64) #1

declare dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEPKc(%"class.std::__1::basic_string"*, i8*) #1

attributes #0 = { ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind readnone }
attributes #4 = { inlinehint nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { noinline noreturn nounwind }
attributes #6 = { inlinehint ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { argmemonly nounwind }
attributes #8 = { nounwind }
attributes #9 = { noreturn nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"Apple LLVM version 8.1.0 (clang-802.0.42)"}
!2 = !{!3}
!3 = distinct !{!3, !4, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!4 = distinct !{!4, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!5 = !{!6}
!6 = distinct !{!6, !7, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!7 = distinct !{!7, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!8 = !{!9}
!9 = distinct !{!9, !10, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!10 = distinct !{!10, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!11 = !{!12}
!12 = distinct !{!12, !13, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!13 = distinct !{!13, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!14 = !{!15}
!15 = distinct !{!15, !16, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!16 = distinct !{!16, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!17 = !{!18}
!18 = distinct !{!18, !19, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!19 = distinct !{!19, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!20 = !{!21}
!21 = distinct !{!21, !22, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!22 = distinct !{!22, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!23 = !{!24}
!24 = distinct !{!24, !25, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!25 = distinct !{!25, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!26 = !{!27}
!27 = distinct !{!27, !28, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!28 = distinct !{!28, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!29 = !{!30}
!30 = distinct !{!30, !31, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!31 = distinct !{!31, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!32 = !{!33}
!33 = distinct !{!33, !34, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!34 = distinct !{!34, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!35 = !{!36}
!36 = distinct !{!36, !37, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!37 = distinct !{!37, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!38 = !{!39}
!39 = distinct !{!39, !40, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!40 = distinct !{!40, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!41 = !{!42}
!42 = distinct !{!42, !43, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!43 = distinct !{!43, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!44 = !{!45}
!45 = distinct !{!45, !46, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!46 = distinct !{!46, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!47 = !{!48}
!48 = distinct !{!48, !49, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!49 = distinct !{!49, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!50 = !{!51}
!51 = distinct !{!51, !52, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!52 = distinct !{!52, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!53 = !{!54}
!54 = distinct !{!54, !55, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!55 = distinct !{!55, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!56 = !{!57}
!57 = distinct !{!57, !58, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!58 = distinct !{!58, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!59 = !{!60}
!60 = distinct !{!60, !61, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!61 = distinct !{!61, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!62 = !{!63}
!63 = distinct !{!63, !64, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!64 = distinct !{!64, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!65 = !{!66}
!66 = distinct !{!66, !67, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!67 = distinct !{!67, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!68 = !{!69}
!69 = distinct !{!69, !70, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!70 = distinct !{!70, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!71 = !{!72}
!72 = distinct !{!72, !73, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!73 = distinct !{!73, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!74 = !{!75}
!75 = distinct !{!75, !76, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!76 = distinct !{!76, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!77 = !{!78}
!78 = distinct !{!78, !79, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!79 = distinct !{!79, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!80 = !{!81}
!81 = distinct !{!81, !82, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!82 = distinct !{!82, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
