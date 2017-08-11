; ModuleID = '/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/Variant/Arithmetic.cpp'
source_filename = "/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/Variant/Arithmetic.cpp"
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
%"class.std::__1::__wrap_iter" = type { i8* }
%"class.std::__1::__wrap_iter.31" = type { i8* }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short" = type { %union.anon.1, [23 x i8] }
%union.anon.1 = type { i8 }
%"class.std::__1::__basic_string_common" = type { i8 }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw" = type { [3 x i64] }
%class.AstNode = type opaque
%"class.std::__1::allocator.24" = type { i8 }
%"struct.std::__1::integral_constant" = type { i8 }
%"struct.std::__1::__has_destroy" = type { i8 }
%"class.std::__1::allocator.18" = type { i8 }
%"struct.std::__1::__has_destroy.27" = type { i8 }
%"class.std::__1::allocator.6" = type { i8 }
%"class.std::__1::allocator.10" = type { i8 }
%"struct.std::__1::pair" = type { %"class.std::__1::basic_string", %struct.TypeSpecifier }
%"struct.std::__1::integral_constant.29" = type { i8 }
%"struct.std::__1::__has_destroy.30" = type { i8 }
%"struct.std::__1::__hash_node" = type { %"struct.std::__1::__hash_node_base", i64, %"union.std::__1::__hash_value_type" }
%"union.std::__1::__hash_value_type" = type { %"struct.std::__1::pair" }

@_ZN4llvm24DisableABIBreakingChecksE = external global i32, align 4
@_ZN4llvm30VerifyDisableABIBreakingChecksE = weak hidden global i32* @_ZN4llvm24DisableABIBreakingChecksE, align 8
@.str = private unnamed_addr constant [27 x i8] c"Cannot add values of type \00", align 1
@.str.1 = private unnamed_addr constant [6 x i8] c" and \00", align 1
@.str.2 = private unnamed_addr constant [39 x i8] c"Cannot apply modulo to values of type \00", align 1
@.str.3 = private unnamed_addr constant [29 x i8] c"Cannot negate value of type \00", align 1
@.str.4 = private unnamed_addr constant [48 x i8] c"Cannot apply bitwise negation to value of type \00", align 1
@.str.5 = private unnamed_addr constant [31 x i8] c"Cannot compare values of type \00", align 1
@.str.6 = private unnamed_addr constant [56 x i8] c"Cannot determine larger element between values of type \00", align 1
@.str.7 = private unnamed_addr constant [44 x i8] c"Cannot apply logical and to values of type \00", align 1
@.str.8 = private unnamed_addr constant [43 x i8] c"Cannot apply logical or to values of type \00", align 1
@.str.9 = private unnamed_addr constant [44 x i8] c"Cannot apply bitwise and to values of type \00", align 1
@.str.10 = private unnamed_addr constant [43 x i8] c"Cannot apply bitwise or to values of type \00", align 1
@.str.11 = private unnamed_addr constant [46 x i8] c"Cannot apply left bitshift to values of type \00", align 1
@.str.12 = private unnamed_addr constant [47 x i8] c"Cannot apply right bitshift to values of type \00", align 1

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic3addE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  %80 = alloca %"class.std::__1::basic_string"*, align 8
  %81 = alloca i8, align 1
  %82 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %83 = alloca %"class.std::__1::__wrap_iter.31"*, align 8
  %84 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %85 = alloca i8*, align 8
  %86 = alloca %"class.std::__1::__wrap_iter.31"*, align 8
  %87 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %88 = alloca i8*, align 8
  %89 = alloca %"class.std::__1::basic_string"*, align 8
  %90 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %91 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %92 = alloca %"class.std::__1::basic_string"*, align 8
  %93 = alloca [3 x i64]*, align 8
  %94 = alloca i32, align 4
  %95 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %96 = alloca %"class.std::__1::basic_string"*, align 8
  %97 = alloca %"class.std::__1::basic_string"*, align 8
  %98 = alloca %"class.std::__1::basic_string"*, align 8
  %99 = alloca %"class.std::__1::basic_string"*, align 8
  %100 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %101 = alloca i8*, align 8
  %102 = alloca %"class.std::__1::__wrap_iter"*, align 8
  %103 = alloca i8*, align 8
  %104 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %105 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %106 = alloca %"class.std::__1::basic_string"*, align 8
  %107 = alloca i8*, align 8
  %108 = alloca i8*, align 8
  %109 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %110 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %111 = alloca %"class.std::__1::basic_string"*, align 8
  %112 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %113 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %114 = alloca %"class.std::__1::basic_string"*, align 8
  %115 = alloca %"class.std::__1::basic_string"*, align 8
  %116 = alloca %"class.std::__1::__wrap_iter", align 8
  %117 = alloca %"class.std::__1::basic_string"*, align 8
  %118 = alloca i8, align 1
  %119 = alloca %"class.std::__1::basic_string"*, align 8
  %120 = alloca %"class.std::__1::__wrap_iter.31", align 8
  %121 = alloca %"class.std::__1::__wrap_iter", align 8
  %122 = alloca %"class.std::__1::__wrap_iter", align 8
  %123 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %124 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %125 = alloca %"class.std::__1::basic_string"*, align 8
  %126 = alloca [3 x i64]*, align 8
  %127 = alloca i32, align 4
  %128 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %129 = alloca %"class.std::__1::basic_string"*, align 8
  %130 = alloca %"class.std::__1::basic_string"*, align 8
  %131 = alloca %"class.std::__1::basic_string"*, align 8
  %132 = alloca %"class.std::__1::basic_string"*, align 8
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
  %144 = alloca i8*, align 8
  %145 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %146 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %147 = alloca %"class.std::__1::basic_string"*, align 8
  %148 = alloca i8*, align 8
  %149 = alloca i8*, align 8
  %150 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %151 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %152 = alloca %"class.std::__1::basic_string"*, align 8
  %153 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %154 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %155 = alloca %"class.std::__1::basic_string"*, align 8
  %156 = alloca %"class.std::__1::basic_string"*, align 8
  %157 = alloca %"class.std::__1::basic_string"*, align 8
  %158 = alloca %"class.std::__1::basic_string"*, align 8
  %159 = alloca %"class.std::__1::basic_string"*, align 8
  %160 = alloca %"class.std::__1::basic_string"*, align 8
  %161 = alloca %"class.std::__1::basic_string"*, align 8
  %162 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %163 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %164 = alloca %"class.std::__1::basic_string"*, align 8
  %165 = alloca [3 x i64]*, align 8
  %166 = alloca i32, align 4
  %167 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %168 = alloca %"class.std::__1::basic_string"*, align 8
  %169 = alloca %"class.std::__1::basic_string"*, align 8
  %170 = alloca %"class.std::__1::basic_string"*, align 8
  %171 = alloca %"class.std::__1::basic_string"*, align 8
  %172 = alloca %"class.std::__1::basic_string"*, align 8
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
  %183 = alloca i8*, align 8
  %184 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %185 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %186 = alloca %"class.std::__1::basic_string"*, align 8
  %187 = alloca i8*, align 8
  %188 = alloca i8*, align 8
  %189 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %190 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %191 = alloca %"class.std::__1::basic_string"*, align 8
  %192 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %193 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %194 = alloca %"class.std::__1::basic_string"*, align 8
  %195 = alloca %"class.std::__1::basic_string"*, align 8
  %196 = alloca %"class.std::__1::basic_string"*, align 8
  %197 = alloca %"class.std::__1::basic_string"*, align 8
  %198 = alloca i64, align 8
  %199 = alloca %"class.std::__1::basic_string"*, align 8
  %200 = alloca %"class.std::__1::basic_string"*, align 8
  %201 = alloca %"class.std::__1::basic_string"*, align 8
  %202 = alloca %"class.std::__1::basic_string", align 8
  %203 = alloca %"class.std::__1::basic_string", align 8
  %204 = alloca i8*
  %205 = alloca i32
  %206 = alloca %"class.std::__1::basic_string", align 8
  %207 = alloca %"class.std::__1::basic_string", align 8
  %208 = alloca %"class.std::__1::basic_string", align 8
  %209 = alloca %"class.std::__1::basic_string", align 8
  %210 = alloca %"class.std::__1::basic_string", align 8
  %211 = alloca %"class.std::__1::basic_string", align 8
  %212 = alloca %"class.std::__1::basic_string", align 8
  %213 = alloca %"class.std::__1::basic_string", align 8
  %214 = alloca %"class.std::__1::basic_string", align 8
  %215 = alloca %"class.std::__1::basic_string", align 8
  %216 = alloca %"class.std::__1::basic_string", align 8
  %217 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %218 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %217, i32 0, i32 0
  %219 = load i32, i32* %218, align 8
  %220 = icmp eq i32 %219, 4
  br i1 %220, label %221, label %360

; <label>:221:                                    ; preds = %3
  %222 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  call void @_ZN7Variant9to_stringEb(%"class.std::__1::basic_string"* sret %203, %struct.Variant* %2, i1 zeroext false)
  store %"class.std::__1::basic_string"* %222, %"class.std::__1::basic_string"** %200, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %203, %"class.std::__1::basic_string"** %201, align 8, !noalias !2
  %223 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %201, align 8, !noalias !2
  %224 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %200, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %223, %"class.std::__1::basic_string"** %197, align 8, !noalias !2
  store i64 0, i64* %198, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %224, %"class.std::__1::basic_string"** %199, align 8, !noalias !2
  %225 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %197, align 8, !noalias !2
  %226 = load i64, i64* %198, align 8, !noalias !2
  %227 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %199, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %227, %"class.std::__1::basic_string"** %196, align 8, !noalias !2
  %228 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %196, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %228, %"class.std::__1::basic_string"** %195, align 8, !noalias !2
  %229 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %195, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %229, %"class.std::__1::basic_string"** %194, align 8, !noalias !2
  %230 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %194, align 8, !noalias !2
  %231 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %230, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %231, %"class.std::__1::__compressed_pair"** %193, align 8, !noalias !2
  %232 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %193, align 8, !noalias !2
  %233 = bitcast %"class.std::__1::__compressed_pair"* %232 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %233, %"class.std::__1::__libcpp_compressed_pair_imp"** %192, align 8, !noalias !2
  %234 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %192, align 8, !noalias !2
  %235 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %234, i32 0, i32 0
  %236 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %235, i32 0, i32 0
  %237 = bitcast %union.anon.0* %236 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %238 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %237, i32 0, i32 0
  %239 = bitcast %union.anon.1* %238 to i8*
  %240 = load i8, i8* %239, align 8, !noalias !2
  %241 = zext i8 %240 to i32
  %242 = and i32 %241, 1
  %243 = icmp ne i32 %242, 0
  br i1 %243, label %244, label %255

; <label>:244:                                    ; preds = %221
  store %"class.std::__1::basic_string"* %229, %"class.std::__1::basic_string"** %186, align 8, !noalias !2
  %245 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %186, align 8, !noalias !2
  %246 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %245, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %246, %"class.std::__1::__compressed_pair"** %185, align 8, !noalias !2
  %247 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %185, align 8, !noalias !2
  %248 = bitcast %"class.std::__1::__compressed_pair"* %247 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %248, %"class.std::__1::__libcpp_compressed_pair_imp"** %184, align 8, !noalias !2
  %249 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %184, align 8, !noalias !2
  %250 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %249, i32 0, i32 0
  %251 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %250, i32 0, i32 0
  %252 = bitcast %union.anon.0* %251 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %253 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %252, i32 0, i32 2
  %254 = load i8*, i8** %253, align 8
  br label %268

; <label>:255:                                    ; preds = %221
  store %"class.std::__1::basic_string"* %229, %"class.std::__1::basic_string"** %191, align 8, !noalias !2
  %256 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %191, align 8, !noalias !2
  %257 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %256, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %257, %"class.std::__1::__compressed_pair"** %190, align 8, !noalias !2
  %258 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %190, align 8, !noalias !2
  %259 = bitcast %"class.std::__1::__compressed_pair"* %258 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %259, %"class.std::__1::__libcpp_compressed_pair_imp"** %189, align 8, !noalias !2
  %260 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %189, align 8, !noalias !2
  %261 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %260, i32 0, i32 0
  %262 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %261, i32 0, i32 0
  %263 = bitcast %union.anon.0* %262 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %264 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %263, i32 0, i32 1
  %265 = getelementptr inbounds [23 x i8], [23 x i8]* %264, i64 0, i64 0
  store i8* %265, i8** %188, align 8, !noalias !2
  %266 = load i8*, i8** %188, align 8, !noalias !2
  store i8* %266, i8** %187, align 8, !noalias !2
  %267 = load i8*, i8** %187, align 8, !noalias !2
  br label %268

; <label>:268:                                    ; preds = %255, %244
  %269 = phi i8* [ %254, %244 ], [ %267, %255 ]
  store i8* %269, i8** %183, align 8, !noalias !2
  %270 = load i8*, i8** %183, align 8, !noalias !2
  %271 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %199, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %271, %"class.std::__1::basic_string"** %182, align 8, !noalias !2
  %272 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %182, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %272, %"class.std::__1::basic_string"** %181, align 8, !noalias !2
  %273 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %181, align 8, !noalias !2
  %274 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %273, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %274, %"class.std::__1::__compressed_pair"** %180, align 8, !noalias !2
  %275 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %180, align 8, !noalias !2
  %276 = bitcast %"class.std::__1::__compressed_pair"* %275 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %276, %"class.std::__1::__libcpp_compressed_pair_imp"** %179, align 8, !noalias !2
  %277 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %179, align 8, !noalias !2
  %278 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %277, i32 0, i32 0
  %279 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %278, i32 0, i32 0
  %280 = bitcast %union.anon.0* %279 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %281 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %280, i32 0, i32 0
  %282 = bitcast %union.anon.1* %281 to i8*
  %283 = load i8, i8* %282, align 8, !noalias !2
  %284 = zext i8 %283 to i32
  %285 = and i32 %284, 1
  %286 = icmp ne i32 %285, 0
  br i1 %286, label %287, label %298

; <label>:287:                                    ; preds = %268
  store %"class.std::__1::basic_string"* %272, %"class.std::__1::basic_string"** %175, align 8, !noalias !2
  %288 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %175, align 8, !noalias !2
  %289 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %288, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %289, %"class.std::__1::__compressed_pair"** %174, align 8, !noalias !2
  %290 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %174, align 8, !noalias !2
  %291 = bitcast %"class.std::__1::__compressed_pair"* %290 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %291, %"class.std::__1::__libcpp_compressed_pair_imp"** %173, align 8, !noalias !2
  %292 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %173, align 8, !noalias !2
  %293 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %292, i32 0, i32 0
  %294 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %293, i32 0, i32 0
  %295 = bitcast %union.anon.0* %294 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %296 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %295, i32 0, i32 1
  %297 = load i64, i64* %296, align 8
  br label %313

; <label>:298:                                    ; preds = %268
  store %"class.std::__1::basic_string"* %272, %"class.std::__1::basic_string"** %178, align 8, !noalias !2
  %299 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %178, align 8, !noalias !2
  %300 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %299, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %300, %"class.std::__1::__compressed_pair"** %177, align 8, !noalias !2
  %301 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %177, align 8, !noalias !2
  %302 = bitcast %"class.std::__1::__compressed_pair"* %301 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %302, %"class.std::__1::__libcpp_compressed_pair_imp"** %176, align 8, !noalias !2
  %303 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %176, align 8, !noalias !2
  %304 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %303, i32 0, i32 0
  %305 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %304, i32 0, i32 0
  %306 = bitcast %union.anon.0* %305 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %307 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %306, i32 0, i32 0
  %308 = bitcast %union.anon.1* %307 to i8*
  %309 = load i8, i8* %308, align 8
  %310 = zext i8 %309 to i32
  %311 = ashr i32 %310, 1
  %312 = sext i32 %311 to i64
  br label %313

; <label>:313:                                    ; preds = %298, %287
  %314 = phi i64 [ %297, %287 ], [ %312, %298 ]
  %315 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKcm(%"class.std::__1::basic_string"* %225, i64 %226, i8* %270, i64 %314)
          to label %316 unwind label %351

; <label>:316:                                    ; preds = %313
  store %"class.std::__1::basic_string"* %315, %"class.std::__1::basic_string"** %172, align 8, !noalias !2
  %317 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %172, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %202, %"class.std::__1::basic_string"** %170, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %317, %"class.std::__1::basic_string"** %171, align 8, !noalias !2
  %318 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %170, align 8, !noalias !2
  %319 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %171, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %318, %"class.std::__1::basic_string"** %168, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %319, %"class.std::__1::basic_string"** %169, align 8, !noalias !2
  %320 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %168, align 8, !noalias !2
  %321 = bitcast %"class.std::__1::basic_string"* %320 to %"class.std::__1::__basic_string_common"*
  %322 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %320, i32 0, i32 0
  %323 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %169, align 8, !noalias !2
  %324 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %323, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %324, %"class.std::__1::__compressed_pair"** %167, align 8, !noalias !2
  %325 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %167, align 8, !noalias !2
  %326 = bitcast %"class.std::__1::__compressed_pair"* %322 to i8*
  %327 = bitcast %"class.std::__1::__compressed_pair"* %325 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %326, i8* %327, i64 24, i32 8, i1 false) #10
  %328 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %169, align 8, !noalias !2
  store %"class.std::__1::basic_string"* %328, %"class.std::__1::basic_string"** %164, align 8, !noalias !2
  %329 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %164, align 8, !noalias !2
  %330 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %329, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %330, %"class.std::__1::__compressed_pair"** %163, align 8, !noalias !2
  %331 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %163, align 8, !noalias !2
  %332 = bitcast %"class.std::__1::__compressed_pair"* %331 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %332, %"class.std::__1::__libcpp_compressed_pair_imp"** %162, align 8, !noalias !2
  %333 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %162, align 8, !noalias !2
  %334 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %333, i32 0, i32 0
  %335 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %334, i32 0, i32 0
  %336 = bitcast %union.anon.0* %335 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %337 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %336, i32 0, i32 0
  store [3 x i64]* %337, [3 x i64]** %165, align 8, !noalias !2
  store i32 0, i32* %166, align 4, !noalias !2
  br label %338

; <label>:338:                                    ; preds = %341, %316
  %339 = load i32, i32* %166, align 4, !noalias !2
  %340 = icmp ult i32 %339, 3
  br i1 %340, label %341, label %348

; <label>:341:                                    ; preds = %338
  %342 = load i32, i32* %166, align 4, !noalias !2
  %343 = zext i32 %342 to i64
  %344 = load [3 x i64]*, [3 x i64]** %165, align 8, !noalias !2
  %345 = getelementptr inbounds [3 x i64], [3 x i64]* %344, i64 0, i64 %343
  store i64 0, i64* %345, align 8
  %346 = load i32, i32* %166, align 4, !noalias !2
  %347 = add i32 %346, 1
  store i32 %347, i32* %166, align 4, !noalias !2
  br label %338

; <label>:348:                                    ; preds = %338
  br label %349

; <label>:349:                                    ; preds = %348
  invoke void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant* %0, %"class.std::__1::basic_string"* %202)
          to label %350 unwind label %355

; <label>:350:                                    ; preds = %349
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %202) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %203) #10
  br label %1057

; <label>:351:                                    ; preds = %313
  %352 = landingpad { i8*, i32 }
          cleanup
  %353 = extractvalue { i8*, i32 } %352, 0
  store i8* %353, i8** %204, align 8
  %354 = extractvalue { i8*, i32 } %352, 1
  store i32 %354, i32* %205, align 4
  br label %359

; <label>:355:                                    ; preds = %349
  %356 = landingpad { i8*, i32 }
          cleanup
  %357 = extractvalue { i8*, i32 } %356, 0
  store i8* %357, i8** %204, align 8
  %358 = extractvalue { i8*, i32 } %356, 1
  store i32 %358, i32* %205, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %202) #10
  br label %359

; <label>:359:                                    ; preds = %355, %351
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %203) #10
  br label %1082

; <label>:360:                                    ; preds = %3
  %361 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %362 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %361, i32 0, i32 0
  %363 = load i32, i32* %362, align 8
  %364 = icmp eq i32 %363, 4
  br i1 %364, label %365, label %503

; <label>:365:                                    ; preds = %360
  call void @_ZN7Variant9to_stringEb(%"class.std::__1::basic_string"* sret %207, %struct.Variant* %1, i1 zeroext false)
  %366 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 3
  store %"class.std::__1::basic_string"* %207, %"class.std::__1::basic_string"** %160, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %366, %"class.std::__1::basic_string"** %161, align 8, !noalias !5
  %367 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %160, align 8, !noalias !5
  %368 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %161, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %367, %"class.std::__1::basic_string"** %158, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %368, %"class.std::__1::basic_string"** %159, align 8, !noalias !5
  %369 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %158, align 8, !noalias !5
  %370 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %159, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %370, %"class.std::__1::basic_string"** %157, align 8, !noalias !5
  %371 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %157, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %371, %"class.std::__1::basic_string"** %156, align 8, !noalias !5
  %372 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %156, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %372, %"class.std::__1::basic_string"** %155, align 8, !noalias !5
  %373 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %155, align 8, !noalias !5
  %374 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %373, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %374, %"class.std::__1::__compressed_pair"** %154, align 8, !noalias !5
  %375 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %154, align 8, !noalias !5
  %376 = bitcast %"class.std::__1::__compressed_pair"* %375 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %376, %"class.std::__1::__libcpp_compressed_pair_imp"** %153, align 8, !noalias !5
  %377 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %153, align 8, !noalias !5
  %378 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %377, i32 0, i32 0
  %379 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %378, i32 0, i32 0
  %380 = bitcast %union.anon.0* %379 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %381 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %380, i32 0, i32 0
  %382 = bitcast %union.anon.1* %381 to i8*
  %383 = load i8, i8* %382, align 8, !noalias !5
  %384 = zext i8 %383 to i32
  %385 = and i32 %384, 1
  %386 = icmp ne i32 %385, 0
  br i1 %386, label %387, label %398

; <label>:387:                                    ; preds = %365
  store %"class.std::__1::basic_string"* %372, %"class.std::__1::basic_string"** %147, align 8, !noalias !5
  %388 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %147, align 8, !noalias !5
  %389 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %388, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %389, %"class.std::__1::__compressed_pair"** %146, align 8, !noalias !5
  %390 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %146, align 8, !noalias !5
  %391 = bitcast %"class.std::__1::__compressed_pair"* %390 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %391, %"class.std::__1::__libcpp_compressed_pair_imp"** %145, align 8, !noalias !5
  %392 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %145, align 8, !noalias !5
  %393 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %392, i32 0, i32 0
  %394 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %393, i32 0, i32 0
  %395 = bitcast %union.anon.0* %394 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %396 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %395, i32 0, i32 2
  %397 = load i8*, i8** %396, align 8
  br label %411

; <label>:398:                                    ; preds = %365
  store %"class.std::__1::basic_string"* %372, %"class.std::__1::basic_string"** %152, align 8, !noalias !5
  %399 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %152, align 8, !noalias !5
  %400 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %399, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %400, %"class.std::__1::__compressed_pair"** %151, align 8, !noalias !5
  %401 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %151, align 8, !noalias !5
  %402 = bitcast %"class.std::__1::__compressed_pair"* %401 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %402, %"class.std::__1::__libcpp_compressed_pair_imp"** %150, align 8, !noalias !5
  %403 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %150, align 8, !noalias !5
  %404 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %403, i32 0, i32 0
  %405 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %404, i32 0, i32 0
  %406 = bitcast %union.anon.0* %405 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %407 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %406, i32 0, i32 1
  %408 = getelementptr inbounds [23 x i8], [23 x i8]* %407, i64 0, i64 0
  store i8* %408, i8** %149, align 8, !noalias !5
  %409 = load i8*, i8** %149, align 8, !noalias !5
  store i8* %409, i8** %148, align 8, !noalias !5
  %410 = load i8*, i8** %148, align 8, !noalias !5
  br label %411

; <label>:411:                                    ; preds = %398, %387
  %412 = phi i8* [ %397, %387 ], [ %410, %398 ]
  store i8* %412, i8** %144, align 8, !noalias !5
  %413 = load i8*, i8** %144, align 8, !noalias !5
  %414 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %159, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %414, %"class.std::__1::basic_string"** %143, align 8, !noalias !5
  %415 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %143, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %415, %"class.std::__1::basic_string"** %142, align 8, !noalias !5
  %416 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %142, align 8, !noalias !5
  %417 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %416, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %417, %"class.std::__1::__compressed_pair"** %141, align 8, !noalias !5
  %418 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %141, align 8, !noalias !5
  %419 = bitcast %"class.std::__1::__compressed_pair"* %418 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %419, %"class.std::__1::__libcpp_compressed_pair_imp"** %140, align 8, !noalias !5
  %420 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %140, align 8, !noalias !5
  %421 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %420, i32 0, i32 0
  %422 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %421, i32 0, i32 0
  %423 = bitcast %union.anon.0* %422 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %424 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %423, i32 0, i32 0
  %425 = bitcast %union.anon.1* %424 to i8*
  %426 = load i8, i8* %425, align 8, !noalias !5
  %427 = zext i8 %426 to i32
  %428 = and i32 %427, 1
  %429 = icmp ne i32 %428, 0
  br i1 %429, label %430, label %441

; <label>:430:                                    ; preds = %411
  store %"class.std::__1::basic_string"* %415, %"class.std::__1::basic_string"** %136, align 8, !noalias !5
  %431 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %136, align 8, !noalias !5
  %432 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %431, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %432, %"class.std::__1::__compressed_pair"** %135, align 8, !noalias !5
  %433 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %135, align 8, !noalias !5
  %434 = bitcast %"class.std::__1::__compressed_pair"* %433 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %434, %"class.std::__1::__libcpp_compressed_pair_imp"** %134, align 8, !noalias !5
  %435 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %134, align 8, !noalias !5
  %436 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %435, i32 0, i32 0
  %437 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %436, i32 0, i32 0
  %438 = bitcast %union.anon.0* %437 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %439 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %438, i32 0, i32 1
  %440 = load i64, i64* %439, align 8
  br label %456

; <label>:441:                                    ; preds = %411
  store %"class.std::__1::basic_string"* %415, %"class.std::__1::basic_string"** %139, align 8, !noalias !5
  %442 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %139, align 8, !noalias !5
  %443 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %442, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %443, %"class.std::__1::__compressed_pair"** %138, align 8, !noalias !5
  %444 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %138, align 8, !noalias !5
  %445 = bitcast %"class.std::__1::__compressed_pair"* %444 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %445, %"class.std::__1::__libcpp_compressed_pair_imp"** %137, align 8, !noalias !5
  %446 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %137, align 8, !noalias !5
  %447 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %446, i32 0, i32 0
  %448 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %447, i32 0, i32 0
  %449 = bitcast %union.anon.0* %448 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %450 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %449, i32 0, i32 0
  %451 = bitcast %union.anon.1* %450 to i8*
  %452 = load i8, i8* %451, align 8
  %453 = zext i8 %452 to i32
  %454 = ashr i32 %453, 1
  %455 = sext i32 %454 to i64
  br label %456

; <label>:456:                                    ; preds = %441, %430
  %457 = phi i64 [ %440, %430 ], [ %455, %441 ]
  %458 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %369, i8* %413, i64 %457)
          to label %459 unwind label %494

; <label>:459:                                    ; preds = %456
  store %"class.std::__1::basic_string"* %458, %"class.std::__1::basic_string"** %133, align 8, !noalias !5
  %460 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %133, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %206, %"class.std::__1::basic_string"** %131, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %460, %"class.std::__1::basic_string"** %132, align 8, !noalias !5
  %461 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %131, align 8, !noalias !5
  %462 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %132, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %461, %"class.std::__1::basic_string"** %129, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %462, %"class.std::__1::basic_string"** %130, align 8, !noalias !5
  %463 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %129, align 8, !noalias !5
  %464 = bitcast %"class.std::__1::basic_string"* %463 to %"class.std::__1::__basic_string_common"*
  %465 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %463, i32 0, i32 0
  %466 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %130, align 8, !noalias !5
  %467 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %466, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %467, %"class.std::__1::__compressed_pair"** %128, align 8, !noalias !5
  %468 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %128, align 8, !noalias !5
  %469 = bitcast %"class.std::__1::__compressed_pair"* %465 to i8*
  %470 = bitcast %"class.std::__1::__compressed_pair"* %468 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %469, i8* %470, i64 24, i32 8, i1 false) #10
  %471 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %130, align 8, !noalias !5
  store %"class.std::__1::basic_string"* %471, %"class.std::__1::basic_string"** %125, align 8, !noalias !5
  %472 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %125, align 8, !noalias !5
  %473 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %472, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %473, %"class.std::__1::__compressed_pair"** %124, align 8, !noalias !5
  %474 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %124, align 8, !noalias !5
  %475 = bitcast %"class.std::__1::__compressed_pair"* %474 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %475, %"class.std::__1::__libcpp_compressed_pair_imp"** %123, align 8, !noalias !5
  %476 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %123, align 8, !noalias !5
  %477 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %476, i32 0, i32 0
  %478 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %477, i32 0, i32 0
  %479 = bitcast %union.anon.0* %478 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %480 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %479, i32 0, i32 0
  store [3 x i64]* %480, [3 x i64]** %126, align 8, !noalias !5
  store i32 0, i32* %127, align 4, !noalias !5
  br label %481

; <label>:481:                                    ; preds = %484, %459
  %482 = load i32, i32* %127, align 4, !noalias !5
  %483 = icmp ult i32 %482, 3
  br i1 %483, label %484, label %491

; <label>:484:                                    ; preds = %481
  %485 = load i32, i32* %127, align 4, !noalias !5
  %486 = zext i32 %485 to i64
  %487 = load [3 x i64]*, [3 x i64]** %126, align 8, !noalias !5
  %488 = getelementptr inbounds [3 x i64], [3 x i64]* %487, i64 0, i64 %486
  store i64 0, i64* %488, align 8
  %489 = load i32, i32* %127, align 4, !noalias !5
  %490 = add i32 %489, 1
  store i32 %490, i32* %127, align 4, !noalias !5
  br label %481

; <label>:491:                                    ; preds = %481
  br label %492

; <label>:492:                                    ; preds = %491
  invoke void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant* %0, %"class.std::__1::basic_string"* %206)
          to label %493 unwind label %498

; <label>:493:                                    ; preds = %492
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %206) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %207) #10
  br label %1057

; <label>:494:                                    ; preds = %456
  %495 = landingpad { i8*, i32 }
          cleanup
  %496 = extractvalue { i8*, i32 } %495, 0
  store i8* %496, i8** %204, align 8
  %497 = extractvalue { i8*, i32 } %495, 1
  store i32 %497, i32* %205, align 4
  br label %502

; <label>:498:                                    ; preds = %492
  %499 = landingpad { i8*, i32 }
          cleanup
  %500 = extractvalue { i8*, i32 } %499, 0
  store i8* %500, i8** %204, align 8
  %501 = extractvalue { i8*, i32 } %499, 1
  store i32 %501, i32* %205, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %206) #10
  br label %502

; <label>:502:                                    ; preds = %498, %494
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %207) #10
  br label %1082

; <label>:503:                                    ; preds = %360
  %504 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %505 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %504, i32 0, i32 0
  %506 = load i32, i32* %505, align 8
  %507 = icmp eq i32 %506, 6
  br i1 %507, label %508, label %623

; <label>:508:                                    ; preds = %503
  %509 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %510 = bitcast %union.anon* %509 to i8*
  %511 = load i8, i8* %510, align 8
  call void @_ZN7Variant9to_stringEb(%"class.std::__1::basic_string"* sret %209, %struct.Variant* %2, i1 zeroext false)
  store i8 %511, i8* %118, align 1, !noalias !8
  store %"class.std::__1::basic_string"* %209, %"class.std::__1::basic_string"** %119, align 8, !noalias !8
  %512 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %119, align 8, !noalias !8
  %513 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %119, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %513, %"class.std::__1::basic_string"** %117, align 8, !noalias !8
  %514 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %117, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %514, %"class.std::__1::basic_string"** %115, align 8, !noalias !8
  %515 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %115, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %515, %"class.std::__1::basic_string"** %114, align 8, !noalias !8
  %516 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %114, align 8, !noalias !8
  %517 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %516, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %517, %"class.std::__1::__compressed_pair"** %113, align 8, !noalias !8
  %518 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %113, align 8, !noalias !8
  %519 = bitcast %"class.std::__1::__compressed_pair"* %518 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %519, %"class.std::__1::__libcpp_compressed_pair_imp"** %112, align 8, !noalias !8
  %520 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %112, align 8, !noalias !8
  %521 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %520, i32 0, i32 0
  %522 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %521, i32 0, i32 0
  %523 = bitcast %union.anon.0* %522 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %524 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %523, i32 0, i32 0
  %525 = bitcast %union.anon.1* %524 to i8*
  %526 = load i8, i8* %525, align 8, !noalias !8
  %527 = zext i8 %526 to i32
  %528 = and i32 %527, 1
  %529 = icmp ne i32 %528, 0
  br i1 %529, label %530, label %541

; <label>:530:                                    ; preds = %508
  store %"class.std::__1::basic_string"* %515, %"class.std::__1::basic_string"** %106, align 8, !noalias !8
  %531 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %106, align 8, !noalias !8
  %532 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %531, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %532, %"class.std::__1::__compressed_pair"** %105, align 8, !noalias !8
  %533 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %105, align 8, !noalias !8
  %534 = bitcast %"class.std::__1::__compressed_pair"* %533 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %534, %"class.std::__1::__libcpp_compressed_pair_imp"** %104, align 8, !noalias !8
  %535 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %104, align 8, !noalias !8
  %536 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %535, i32 0, i32 0
  %537 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %536, i32 0, i32 0
  %538 = bitcast %union.anon.0* %537 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %539 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %538, i32 0, i32 2
  %540 = load i8*, i8** %539, align 8
  br label %554

; <label>:541:                                    ; preds = %508
  store %"class.std::__1::basic_string"* %515, %"class.std::__1::basic_string"** %111, align 8, !noalias !8
  %542 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %111, align 8, !noalias !8
  %543 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %542, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %543, %"class.std::__1::__compressed_pair"** %110, align 8, !noalias !8
  %544 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %110, align 8, !noalias !8
  %545 = bitcast %"class.std::__1::__compressed_pair"* %544 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %545, %"class.std::__1::__libcpp_compressed_pair_imp"** %109, align 8, !noalias !8
  %546 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %109, align 8, !noalias !8
  %547 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %546, i32 0, i32 0
  %548 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %547, i32 0, i32 0
  %549 = bitcast %union.anon.0* %548 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %550 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %549, i32 0, i32 1
  %551 = getelementptr inbounds [23 x i8], [23 x i8]* %550, i64 0, i64 0
  store i8* %551, i8** %108, align 8, !noalias !8
  %552 = load i8*, i8** %108, align 8, !noalias !8
  store i8* %552, i8** %107, align 8, !noalias !8
  %553 = load i8*, i8** %107, align 8, !noalias !8
  br label %554

; <label>:554:                                    ; preds = %541, %530
  %555 = phi i8* [ %540, %530 ], [ %553, %541 ]
  store %"class.std::__1::__wrap_iter"* %116, %"class.std::__1::__wrap_iter"** %102, align 8, !noalias !8
  store i8* %555, i8** %103, align 8, !noalias !8
  %556 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %102, align 8, !noalias !8
  %557 = load i8*, i8** %103, align 8, !noalias !8
  store %"class.std::__1::__wrap_iter"* %556, %"class.std::__1::__wrap_iter"** %100, align 8, !noalias !8
  store i8* %557, i8** %101, align 8, !noalias !8
  %558 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %100, align 8, !noalias !8
  %559 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %558, i32 0, i32 0
  %560 = load i8*, i8** %101, align 8, !noalias !8
  store i8* %560, i8** %559, align 8, !noalias !8
  %561 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %116, i32 0, i32 0
  %562 = load i8*, i8** %561, align 8, !noalias !8
  %563 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %121, i32 0, i32 0
  store i8* %562, i8** %563, align 8, !noalias !8
  store %"class.std::__1::__wrap_iter.31"* %120, %"class.std::__1::__wrap_iter.31"** %86, align 8, !noalias !8
  store %"class.std::__1::__wrap_iter"* %121, %"class.std::__1::__wrap_iter"** %87, align 8, !noalias !8
  store i8* null, i8** %88, align 8, !noalias !8
  %564 = load %"class.std::__1::__wrap_iter.31"*, %"class.std::__1::__wrap_iter.31"** %86, align 8, !noalias !8
  %565 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %87, align 8, !noalias !8
  %566 = load i8*, i8** %88, align 8, !noalias !8
  store %"class.std::__1::__wrap_iter.31"* %564, %"class.std::__1::__wrap_iter.31"** %83, align 8, !noalias !8
  store %"class.std::__1::__wrap_iter"* %565, %"class.std::__1::__wrap_iter"** %84, align 8, !noalias !8
  store i8* %566, i8** %85, align 8, !noalias !8
  %567 = load %"class.std::__1::__wrap_iter.31"*, %"class.std::__1::__wrap_iter.31"** %83, align 8, !noalias !8
  %568 = getelementptr inbounds %"class.std::__1::__wrap_iter.31", %"class.std::__1::__wrap_iter.31"* %567, i32 0, i32 0
  %569 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %84, align 8, !noalias !8
  store %"class.std::__1::__wrap_iter"* %569, %"class.std::__1::__wrap_iter"** %82, align 8, !noalias !8
  %570 = load %"class.std::__1::__wrap_iter"*, %"class.std::__1::__wrap_iter"** %82, align 8, !noalias !8
  %571 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %570, i32 0, i32 0
  %572 = load i8*, i8** %571, align 8, !noalias !8
  store i8* %572, i8** %568, align 8, !noalias !8
  %573 = load i8, i8* %118, align 1, !noalias !8
  %574 = getelementptr inbounds %"class.std::__1::__wrap_iter.31", %"class.std::__1::__wrap_iter.31"* %120, i32 0, i32 0
  %575 = load i8*, i8** %574, align 8, !noalias !8
  %576 = invoke i8* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertENS_11__wrap_iterIPKcEEc(%"class.std::__1::basic_string"* %512, i8* %575, i8 signext %573)
          to label %577 unwind label %614

; <label>:577:                                    ; preds = %554
  %578 = getelementptr inbounds %"class.std::__1::__wrap_iter", %"class.std::__1::__wrap_iter"* %122, i32 0, i32 0
  store i8* %576, i8** %578, align 8, !noalias !8
  %579 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %119, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %579, %"class.std::__1::basic_string"** %89, align 8, !noalias !8
  %580 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %89, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %208, %"class.std::__1::basic_string"** %98, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %580, %"class.std::__1::basic_string"** %99, align 8, !noalias !8
  %581 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %98, align 8, !noalias !8
  %582 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %99, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %581, %"class.std::__1::basic_string"** %96, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %582, %"class.std::__1::basic_string"** %97, align 8, !noalias !8
  %583 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %96, align 8, !noalias !8
  %584 = bitcast %"class.std::__1::basic_string"* %583 to %"class.std::__1::__basic_string_common"*
  %585 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %583, i32 0, i32 0
  %586 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %97, align 8, !noalias !8
  %587 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %586, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %587, %"class.std::__1::__compressed_pair"** %95, align 8, !noalias !8
  %588 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %95, align 8, !noalias !8
  %589 = bitcast %"class.std::__1::__compressed_pair"* %585 to i8*
  %590 = bitcast %"class.std::__1::__compressed_pair"* %588 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %589, i8* %590, i64 24, i32 8, i1 false) #10
  %591 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %97, align 8, !noalias !8
  store %"class.std::__1::basic_string"* %591, %"class.std::__1::basic_string"** %92, align 8, !noalias !8
  %592 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %92, align 8, !noalias !8
  %593 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %592, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %593, %"class.std::__1::__compressed_pair"** %91, align 8, !noalias !8
  %594 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %91, align 8, !noalias !8
  %595 = bitcast %"class.std::__1::__compressed_pair"* %594 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %595, %"class.std::__1::__libcpp_compressed_pair_imp"** %90, align 8, !noalias !8
  %596 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %90, align 8, !noalias !8
  %597 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %596, i32 0, i32 0
  %598 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %597, i32 0, i32 0
  %599 = bitcast %union.anon.0* %598 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %600 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %599, i32 0, i32 0
  store [3 x i64]* %600, [3 x i64]** %93, align 8, !noalias !8
  store i32 0, i32* %94, align 4, !noalias !8
  br label %601

; <label>:601:                                    ; preds = %604, %577
  %602 = load i32, i32* %94, align 4, !noalias !8
  %603 = icmp ult i32 %602, 3
  br i1 %603, label %604, label %611

; <label>:604:                                    ; preds = %601
  %605 = load i32, i32* %94, align 4, !noalias !8
  %606 = zext i32 %605 to i64
  %607 = load [3 x i64]*, [3 x i64]** %93, align 8, !noalias !8
  %608 = getelementptr inbounds [3 x i64], [3 x i64]* %607, i64 0, i64 %606
  store i64 0, i64* %608, align 8
  %609 = load i32, i32* %94, align 4, !noalias !8
  %610 = add i32 %609, 1
  store i32 %610, i32* %94, align 4, !noalias !8
  br label %601

; <label>:611:                                    ; preds = %601
  br label %612

; <label>:612:                                    ; preds = %611
  invoke void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant* %0, %"class.std::__1::basic_string"* %208)
          to label %613 unwind label %618

; <label>:613:                                    ; preds = %612
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %208) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %209) #10
  br label %1057

; <label>:614:                                    ; preds = %554
  %615 = landingpad { i8*, i32 }
          cleanup
  %616 = extractvalue { i8*, i32 } %615, 0
  store i8* %616, i8** %204, align 8
  %617 = extractvalue { i8*, i32 } %615, 1
  store i32 %617, i32* %205, align 4
  br label %622

; <label>:618:                                    ; preds = %612
  %619 = landingpad { i8*, i32 }
          cleanup
  %620 = extractvalue { i8*, i32 } %619, 0
  store i8* %620, i8** %204, align 8
  %621 = extractvalue { i8*, i32 } %619, 1
  store i32 %621, i32* %205, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %208) #10
  br label %622

; <label>:622:                                    ; preds = %618, %614
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %209) #10
  br label %1082

; <label>:623:                                    ; preds = %503
  %624 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %625 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %624, i32 0, i32 0
  %626 = load i32, i32* %625, align 8
  %627 = icmp eq i32 %626, 6
  br i1 %627, label %628, label %679

; <label>:628:                                    ; preds = %623
  call void @_ZN7Variant9to_stringEb(%"class.std::__1::basic_string"* sret %211, %struct.Variant* %1, i1 zeroext false)
  %629 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %630 = bitcast %union.anon* %629 to i8*
  %631 = load i8, i8* %630, align 8
  store %"class.std::__1::basic_string"* %211, %"class.std::__1::basic_string"** %80, align 8, !noalias !11
  store i8 %631, i8* %81, align 1, !noalias !11
  %632 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %80, align 8, !noalias !11
  %633 = load i8, i8* %81, align 1, !noalias !11
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE9push_backEc(%"class.std::__1::basic_string"* %632, i8 signext %633)
          to label %634 unwind label %670

; <label>:634:                                    ; preds = %628
  %635 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %80, align 8, !noalias !11
  store %"class.std::__1::basic_string"* %635, %"class.std::__1::basic_string"** %79, align 8, !noalias !11
  %636 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %79, align 8, !noalias !11
  store %"class.std::__1::basic_string"* %210, %"class.std::__1::basic_string"** %77, align 8, !noalias !11
  store %"class.std::__1::basic_string"* %636, %"class.std::__1::basic_string"** %78, align 8, !noalias !11
  %637 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %77, align 8, !noalias !11
  %638 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %78, align 8, !noalias !11
  store %"class.std::__1::basic_string"* %637, %"class.std::__1::basic_string"** %75, align 8, !noalias !11
  store %"class.std::__1::basic_string"* %638, %"class.std::__1::basic_string"** %76, align 8, !noalias !11
  %639 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %75, align 8, !noalias !11
  %640 = bitcast %"class.std::__1::basic_string"* %639 to %"class.std::__1::__basic_string_common"*
  %641 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %639, i32 0, i32 0
  %642 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %76, align 8, !noalias !11
  %643 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %642, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %643, %"class.std::__1::__compressed_pair"** %74, align 8, !noalias !11
  %644 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %74, align 8, !noalias !11
  %645 = bitcast %"class.std::__1::__compressed_pair"* %641 to i8*
  %646 = bitcast %"class.std::__1::__compressed_pair"* %644 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %645, i8* %646, i64 24, i32 8, i1 false) #10
  %647 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %76, align 8, !noalias !11
  store %"class.std::__1::basic_string"* %647, %"class.std::__1::basic_string"** %71, align 8, !noalias !11
  %648 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %71, align 8, !noalias !11
  %649 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %648, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %649, %"class.std::__1::__compressed_pair"** %70, align 8, !noalias !11
  %650 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %70, align 8, !noalias !11
  %651 = bitcast %"class.std::__1::__compressed_pair"* %650 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %651, %"class.std::__1::__libcpp_compressed_pair_imp"** %69, align 8, !noalias !11
  %652 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %69, align 8, !noalias !11
  %653 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %652, i32 0, i32 0
  %654 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %653, i32 0, i32 0
  %655 = bitcast %union.anon.0* %654 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %656 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %655, i32 0, i32 0
  store [3 x i64]* %656, [3 x i64]** %72, align 8, !noalias !11
  store i32 0, i32* %73, align 4, !noalias !11
  br label %657

; <label>:657:                                    ; preds = %660, %634
  %658 = load i32, i32* %73, align 4, !noalias !11
  %659 = icmp ult i32 %658, 3
  br i1 %659, label %660, label %667

; <label>:660:                                    ; preds = %657
  %661 = load i32, i32* %73, align 4, !noalias !11
  %662 = zext i32 %661 to i64
  %663 = load [3 x i64]*, [3 x i64]** %72, align 8, !noalias !11
  %664 = getelementptr inbounds [3 x i64], [3 x i64]* %663, i64 0, i64 %662
  store i64 0, i64* %664, align 8
  %665 = load i32, i32* %73, align 4, !noalias !11
  %666 = add i32 %665, 1
  store i32 %666, i32* %73, align 4, !noalias !11
  br label %657

; <label>:667:                                    ; preds = %657
  br label %668

; <label>:668:                                    ; preds = %667
  invoke void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant* %0, %"class.std::__1::basic_string"* %210)
          to label %669 unwind label %674

; <label>:669:                                    ; preds = %668
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %210) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %211) #10
  br label %1057

; <label>:670:                                    ; preds = %628
  %671 = landingpad { i8*, i32 }
          cleanup
  %672 = extractvalue { i8*, i32 } %671, 0
  store i8* %672, i8** %204, align 8
  %673 = extractvalue { i8*, i32 } %671, 1
  store i32 %673, i32* %205, align 4
  br label %678

; <label>:674:                                    ; preds = %668
  %675 = landingpad { i8*, i32 }
          cleanup
  %676 = extractvalue { i8*, i32 } %675, 0
  store i8* %676, i8** %204, align 8
  %677 = extractvalue { i8*, i32 } %675, 1
  store i32 %677, i32* %205, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %210) #10
  br label %678

; <label>:678:                                    ; preds = %674, %670
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %211) #10
  br label %1082

; <label>:679:                                    ; preds = %623
  %680 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %681 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %680, i32 0, i32 0
  %682 = load i32, i32* %681, align 8
  switch i32 %682, label %847 [
    i32 0, label %683
    i32 1, label %724
    i32 2, label %765
    i32 3, label %806
  ]

; <label>:683:                                    ; preds = %679
  %684 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %685 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %684, i32 0, i32 0
  %686 = load i32, i32* %685, align 8
  switch i32 %686, label %722 [
    i32 0, label %687
    i32 1, label %695
    i32 2, label %704
    i32 3, label %713
  ]

; <label>:687:                                    ; preds = %683
  %688 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %689 = bitcast %union.anon* %688 to i32*
  %690 = load i32, i32* %689, align 8
  %691 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %692 = bitcast %union.anon* %691 to i32*
  %693 = load i32, i32* %692, align 8
  %694 = add nsw i32 %690, %693
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %694)
  br label %1057

; <label>:695:                                    ; preds = %683
  %696 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %697 = bitcast %union.anon* %696 to i32*
  %698 = load i32, i32* %697, align 8
  %699 = sext i32 %698 to i64
  %700 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %701 = bitcast %union.anon* %700 to i64*
  %702 = load i64, i64* %701, align 8
  %703 = add nsw i64 %699, %702
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %703)
  br label %1057

; <label>:704:                                    ; preds = %683
  %705 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %706 = bitcast %union.anon* %705 to i32*
  %707 = load i32, i32* %706, align 8
  %708 = sitofp i32 %707 to float
  %709 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %710 = bitcast %union.anon* %709 to float*
  %711 = load float, float* %710, align 8
  %712 = fadd float %708, %711
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %712)
  br label %1057

; <label>:713:                                    ; preds = %683
  %714 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %715 = bitcast %union.anon* %714 to i32*
  %716 = load i32, i32* %715, align 8
  %717 = sitofp i32 %716 to double
  %718 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %719 = bitcast %union.anon* %718 to double*
  %720 = load double, double* %719, align 8
  %721 = fadd double %717, %720
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %721)
  br label %1057

; <label>:722:                                    ; preds = %683
  br label %723

; <label>:723:                                    ; preds = %722
  br label %848

; <label>:724:                                    ; preds = %679
  %725 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %726 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %725, i32 0, i32 0
  %727 = load i32, i32* %726, align 8
  switch i32 %727, label %763 [
    i32 0, label %728
    i32 1, label %737
    i32 2, label %745
    i32 3, label %754
  ]

; <label>:728:                                    ; preds = %724
  %729 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %730 = bitcast %union.anon* %729 to i64*
  %731 = load i64, i64* %730, align 8
  %732 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %733 = bitcast %union.anon* %732 to i32*
  %734 = load i32, i32* %733, align 8
  %735 = sext i32 %734 to i64
  %736 = add nsw i64 %731, %735
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %736)
  br label %1057

; <label>:737:                                    ; preds = %724
  %738 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %739 = bitcast %union.anon* %738 to i64*
  %740 = load i64, i64* %739, align 8
  %741 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %742 = bitcast %union.anon* %741 to i64*
  %743 = load i64, i64* %742, align 8
  %744 = add nsw i64 %740, %743
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %744)
  br label %1057

; <label>:745:                                    ; preds = %724
  %746 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %747 = bitcast %union.anon* %746 to i64*
  %748 = load i64, i64* %747, align 8
  %749 = sitofp i64 %748 to float
  %750 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %751 = bitcast %union.anon* %750 to float*
  %752 = load float, float* %751, align 8
  %753 = fadd float %749, %752
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %753)
  br label %1057

; <label>:754:                                    ; preds = %724
  %755 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %756 = bitcast %union.anon* %755 to i64*
  %757 = load i64, i64* %756, align 8
  %758 = sitofp i64 %757 to double
  %759 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %760 = bitcast %union.anon* %759 to double*
  %761 = load double, double* %760, align 8
  %762 = fadd double %758, %761
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %762)
  br label %1057

; <label>:763:                                    ; preds = %724
  br label %764

; <label>:764:                                    ; preds = %763
  br label %848

; <label>:765:                                    ; preds = %679
  %766 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %767 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %766, i32 0, i32 0
  %768 = load i32, i32* %767, align 8
  switch i32 %768, label %804 [
    i32 0, label %769
    i32 1, label %778
    i32 2, label %787
    i32 3, label %795
  ]

; <label>:769:                                    ; preds = %765
  %770 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %771 = bitcast %union.anon* %770 to float*
  %772 = load float, float* %771, align 8
  %773 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %774 = bitcast %union.anon* %773 to i32*
  %775 = load i32, i32* %774, align 8
  %776 = sitofp i32 %775 to float
  %777 = fadd float %772, %776
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %777)
  br label %1057

; <label>:778:                                    ; preds = %765
  %779 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %780 = bitcast %union.anon* %779 to float*
  %781 = load float, float* %780, align 8
  %782 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %783 = bitcast %union.anon* %782 to i64*
  %784 = load i64, i64* %783, align 8
  %785 = sitofp i64 %784 to float
  %786 = fadd float %781, %785
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %786)
  br label %1057

; <label>:787:                                    ; preds = %765
  %788 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %789 = bitcast %union.anon* %788 to float*
  %790 = load float, float* %789, align 8
  %791 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %792 = bitcast %union.anon* %791 to float*
  %793 = load float, float* %792, align 8
  %794 = fadd float %790, %793
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %794)
  br label %1057

; <label>:795:                                    ; preds = %765
  %796 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %797 = bitcast %union.anon* %796 to float*
  %798 = load float, float* %797, align 8
  %799 = fpext float %798 to double
  %800 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %801 = bitcast %union.anon* %800 to double*
  %802 = load double, double* %801, align 8
  %803 = fadd double %799, %802
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %803)
  br label %1057

; <label>:804:                                    ; preds = %765
  br label %805

; <label>:805:                                    ; preds = %804
  br label %848

; <label>:806:                                    ; preds = %679
  %807 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %808 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %807, i32 0, i32 0
  %809 = load i32, i32* %808, align 8
  switch i32 %809, label %845 [
    i32 0, label %810
    i32 1, label %819
    i32 2, label %828
    i32 3, label %837
  ]

; <label>:810:                                    ; preds = %806
  %811 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %812 = bitcast %union.anon* %811 to double*
  %813 = load double, double* %812, align 8
  %814 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %815 = bitcast %union.anon* %814 to i32*
  %816 = load i32, i32* %815, align 8
  %817 = sitofp i32 %816 to double
  %818 = fadd double %813, %817
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %818)
  br label %1057

; <label>:819:                                    ; preds = %806
  %820 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %821 = bitcast %union.anon* %820 to double*
  %822 = load double, double* %821, align 8
  %823 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %824 = bitcast %union.anon* %823 to i64*
  %825 = load i64, i64* %824, align 8
  %826 = sitofp i64 %825 to double
  %827 = fadd double %822, %826
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %827)
  br label %1057

; <label>:828:                                    ; preds = %806
  %829 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %830 = bitcast %union.anon* %829 to double*
  %831 = load double, double* %830, align 8
  %832 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %833 = bitcast %union.anon* %832 to float*
  %834 = load float, float* %833, align 8
  %835 = fpext float %834 to double
  %836 = fadd double %831, %835
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %836)
  br label %1057

; <label>:837:                                    ; preds = %806
  %838 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %839 = bitcast %union.anon* %838 to double*
  %840 = load double, double* %839, align 8
  %841 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %842 = bitcast %union.anon* %841 to double*
  %843 = load double, double* %842, align 8
  %844 = fadd double %840, %843
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %844)
  br label %1057

; <label>:845:                                    ; preds = %806
  br label %846

; <label>:846:                                    ; preds = %845
  br label %848

; <label>:847:                                    ; preds = %679
  br label %848

; <label>:848:                                    ; preds = %847, %846, %805, %764, %723
  %849 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %850 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %849, i32 0, i32 0
  %851 = load i32, i32* %850, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %215, i32 %851)
  store i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str, i32 0, i32 0), i8** %67, align 8, !noalias !14
  store %"class.std::__1::basic_string"* %215, %"class.std::__1::basic_string"** %68, align 8, !noalias !14
  %852 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !14
  %853 = load i8*, i8** %67, align 8, !noalias !14
  %854 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %852, i64 0, i8* %853)
          to label %855 unwind label %1058

; <label>:855:                                    ; preds = %848
  store %"class.std::__1::basic_string"* %854, %"class.std::__1::basic_string"** %66, align 8, !noalias !14
  %856 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !14
  store %"class.std::__1::basic_string"* %214, %"class.std::__1::basic_string"** %64, align 8, !noalias !14
  store %"class.std::__1::basic_string"* %856, %"class.std::__1::basic_string"** %65, align 8, !noalias !14
  %857 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !14
  %858 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !14
  store %"class.std::__1::basic_string"* %857, %"class.std::__1::basic_string"** %62, align 8, !noalias !14
  store %"class.std::__1::basic_string"* %858, %"class.std::__1::basic_string"** %63, align 8, !noalias !14
  %859 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8, !noalias !14
  %860 = bitcast %"class.std::__1::basic_string"* %859 to %"class.std::__1::__basic_string_common"*
  %861 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %859, i32 0, i32 0
  %862 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !14
  %863 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %862, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %863, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !14
  %864 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !14
  %865 = bitcast %"class.std::__1::__compressed_pair"* %861 to i8*
  %866 = bitcast %"class.std::__1::__compressed_pair"* %864 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %865, i8* %866, i64 24, i32 8, i1 false) #10
  %867 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !14
  store %"class.std::__1::basic_string"* %867, %"class.std::__1::basic_string"** %58, align 8, !noalias !14
  %868 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8, !noalias !14
  %869 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %868, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %869, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !14
  %870 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !14
  %871 = bitcast %"class.std::__1::__compressed_pair"* %870 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %871, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !14
  %872 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !14
  %873 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %872, i32 0, i32 0
  %874 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %873, i32 0, i32 0
  %875 = bitcast %union.anon.0* %874 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %876 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %875, i32 0, i32 0
  store [3 x i64]* %876, [3 x i64]** %59, align 8, !noalias !14
  store i32 0, i32* %60, align 4, !noalias !14
  br label %877

; <label>:877:                                    ; preds = %880, %855
  %878 = load i32, i32* %60, align 4, !noalias !14
  %879 = icmp ult i32 %878, 3
  br i1 %879, label %880, label %887

; <label>:880:                                    ; preds = %877
  %881 = load i32, i32* %60, align 4, !noalias !14
  %882 = zext i32 %881 to i64
  %883 = load [3 x i64]*, [3 x i64]** %59, align 8, !noalias !14
  %884 = getelementptr inbounds [3 x i64], [3 x i64]* %883, i64 0, i64 %882
  store i64 0, i64* %884, align 8
  %885 = load i32, i32* %60, align 4, !noalias !14
  %886 = add i32 %885, 1
  store i32 %886, i32* %60, align 4, !noalias !14
  br label %877

; <label>:887:                                    ; preds = %877
  br label %888

; <label>:888:                                    ; preds = %887
  store %"class.std::__1::basic_string"* %214, %"class.std::__1::basic_string"** %54, align 8, !noalias !17
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %55, align 8, !noalias !17
  %889 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !17
  %890 = load i8*, i8** %55, align 8, !noalias !17
  %891 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %889, i8* %890)
          to label %892 unwind label %1062

; <label>:892:                                    ; preds = %888
  store %"class.std::__1::basic_string"* %891, %"class.std::__1::basic_string"** %53, align 8, !noalias !17
  %893 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !17
  store %"class.std::__1::basic_string"* %213, %"class.std::__1::basic_string"** %51, align 8, !noalias !17
  store %"class.std::__1::basic_string"* %893, %"class.std::__1::basic_string"** %52, align 8, !noalias !17
  %894 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !17
  %895 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !17
  store %"class.std::__1::basic_string"* %894, %"class.std::__1::basic_string"** %49, align 8, !noalias !17
  store %"class.std::__1::basic_string"* %895, %"class.std::__1::basic_string"** %50, align 8, !noalias !17
  %896 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !17
  %897 = bitcast %"class.std::__1::basic_string"* %896 to %"class.std::__1::__basic_string_common"*
  %898 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %896, i32 0, i32 0
  %899 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !17
  %900 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %899, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %900, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !17
  %901 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !17
  %902 = bitcast %"class.std::__1::__compressed_pair"* %898 to i8*
  %903 = bitcast %"class.std::__1::__compressed_pair"* %901 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %902, i8* %903, i64 24, i32 8, i1 false) #10
  %904 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !17
  store %"class.std::__1::basic_string"* %904, %"class.std::__1::basic_string"** %45, align 8, !noalias !17
  %905 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !17
  %906 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %905, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %906, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !17
  %907 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !17
  %908 = bitcast %"class.std::__1::__compressed_pair"* %907 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %908, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !17
  %909 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !17
  %910 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %909, i32 0, i32 0
  %911 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %910, i32 0, i32 0
  %912 = bitcast %union.anon.0* %911 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %913 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %912, i32 0, i32 0
  store [3 x i64]* %913, [3 x i64]** %46, align 8, !noalias !17
  store i32 0, i32* %47, align 4, !noalias !17
  br label %914

; <label>:914:                                    ; preds = %917, %892
  %915 = load i32, i32* %47, align 4, !noalias !17
  %916 = icmp ult i32 %915, 3
  br i1 %916, label %917, label %924

; <label>:917:                                    ; preds = %914
  %918 = load i32, i32* %47, align 4, !noalias !17
  %919 = zext i32 %918 to i64
  %920 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !17
  %921 = getelementptr inbounds [3 x i64], [3 x i64]* %920, i64 0, i64 %919
  store i64 0, i64* %921, align 8
  %922 = load i32, i32* %47, align 4, !noalias !17
  %923 = add i32 %922, 1
  store i32 %923, i32* %47, align 4, !noalias !17
  br label %914

; <label>:924:                                    ; preds = %914
  br label %925

; <label>:925:                                    ; preds = %924
  %926 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %927 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %926, i32 0, i32 0
  %928 = load i32, i32* %927, align 8
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %216, i32 %928)
          to label %929 unwind label %1066

; <label>:929:                                    ; preds = %925
  store %"class.std::__1::basic_string"* %213, %"class.std::__1::basic_string"** %41, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %216, %"class.std::__1::basic_string"** %42, align 8, !noalias !20
  %930 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !20
  %931 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %930, %"class.std::__1::basic_string"** %39, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %931, %"class.std::__1::basic_string"** %40, align 8, !noalias !20
  %932 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !20
  %933 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %933, %"class.std::__1::basic_string"** %38, align 8, !noalias !20
  %934 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %934, %"class.std::__1::basic_string"** %37, align 8, !noalias !20
  %935 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %935, %"class.std::__1::basic_string"** %36, align 8, !noalias !20
  %936 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !20
  %937 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %936, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %937, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !20
  %938 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !20
  %939 = bitcast %"class.std::__1::__compressed_pair"* %938 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %939, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !20
  %940 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !20
  %941 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %940, i32 0, i32 0
  %942 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %941, i32 0, i32 0
  %943 = bitcast %union.anon.0* %942 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %944 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %943, i32 0, i32 0
  %945 = bitcast %union.anon.1* %944 to i8*
  %946 = load i8, i8* %945, align 8, !noalias !20
  %947 = zext i8 %946 to i32
  %948 = and i32 %947, 1
  %949 = icmp ne i32 %948, 0
  br i1 %949, label %950, label %961

; <label>:950:                                    ; preds = %929
  store %"class.std::__1::basic_string"* %935, %"class.std::__1::basic_string"** %28, align 8, !noalias !20
  %951 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !20
  %952 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %951, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %952, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !20
  %953 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !20
  %954 = bitcast %"class.std::__1::__compressed_pair"* %953 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %954, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !20
  %955 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !20
  %956 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %955, i32 0, i32 0
  %957 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %956, i32 0, i32 0
  %958 = bitcast %union.anon.0* %957 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %959 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %958, i32 0, i32 2
  %960 = load i8*, i8** %959, align 8
  br label %974

; <label>:961:                                    ; preds = %929
  store %"class.std::__1::basic_string"* %935, %"class.std::__1::basic_string"** %33, align 8, !noalias !20
  %962 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !20
  %963 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %962, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %963, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !20
  %964 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !20
  %965 = bitcast %"class.std::__1::__compressed_pair"* %964 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %965, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !20
  %966 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !20
  %967 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %966, i32 0, i32 0
  %968 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %967, i32 0, i32 0
  %969 = bitcast %union.anon.0* %968 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %970 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %969, i32 0, i32 1
  %971 = getelementptr inbounds [23 x i8], [23 x i8]* %970, i64 0, i64 0
  store i8* %971, i8** %30, align 8, !noalias !20
  %972 = load i8*, i8** %30, align 8, !noalias !20
  store i8* %972, i8** %29, align 8, !noalias !20
  %973 = load i8*, i8** %29, align 8, !noalias !20
  br label %974

; <label>:974:                                    ; preds = %961, %950
  %975 = phi i8* [ %960, %950 ], [ %973, %961 ]
  store i8* %975, i8** %25, align 8, !noalias !20
  %976 = load i8*, i8** %25, align 8, !noalias !20
  %977 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %977, %"class.std::__1::basic_string"** %24, align 8, !noalias !20
  %978 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %978, %"class.std::__1::basic_string"** %23, align 8, !noalias !20
  %979 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !20
  %980 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %979, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %980, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !20
  %981 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !20
  %982 = bitcast %"class.std::__1::__compressed_pair"* %981 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %982, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !20
  %983 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !20
  %984 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %983, i32 0, i32 0
  %985 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %984, i32 0, i32 0
  %986 = bitcast %union.anon.0* %985 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %987 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %986, i32 0, i32 0
  %988 = bitcast %union.anon.1* %987 to i8*
  %989 = load i8, i8* %988, align 8, !noalias !20
  %990 = zext i8 %989 to i32
  %991 = and i32 %990, 1
  %992 = icmp ne i32 %991, 0
  br i1 %992, label %993, label %1004

; <label>:993:                                    ; preds = %974
  store %"class.std::__1::basic_string"* %978, %"class.std::__1::basic_string"** %17, align 8, !noalias !20
  %994 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !20
  %995 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %994, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %995, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !20
  %996 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !20
  %997 = bitcast %"class.std::__1::__compressed_pair"* %996 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %997, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !20
  %998 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !20
  %999 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %998, i32 0, i32 0
  %1000 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %999, i32 0, i32 0
  %1001 = bitcast %union.anon.0* %1000 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %1002 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %1001, i32 0, i32 1
  %1003 = load i64, i64* %1002, align 8
  br label %1019

; <label>:1004:                                   ; preds = %974
  store %"class.std::__1::basic_string"* %978, %"class.std::__1::basic_string"** %20, align 8, !noalias !20
  %1005 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !20
  %1006 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1005, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1006, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !20
  %1007 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !20
  %1008 = bitcast %"class.std::__1::__compressed_pair"* %1007 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1008, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !20
  %1009 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !20
  %1010 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1009, i32 0, i32 0
  %1011 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1010, i32 0, i32 0
  %1012 = bitcast %union.anon.0* %1011 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %1013 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %1012, i32 0, i32 0
  %1014 = bitcast %union.anon.1* %1013 to i8*
  %1015 = load i8, i8* %1014, align 8
  %1016 = zext i8 %1015 to i32
  %1017 = ashr i32 %1016, 1
  %1018 = sext i32 %1017 to i64
  br label %1019

; <label>:1019:                                   ; preds = %1004, %993
  %1020 = phi i64 [ %1003, %993 ], [ %1018, %1004 ]
  %1021 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %932, i8* %976, i64 %1020)
          to label %1022 unwind label %1070

; <label>:1022:                                   ; preds = %1019
  store %"class.std::__1::basic_string"* %1021, %"class.std::__1::basic_string"** %14, align 8, !noalias !20
  %1023 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %212, %"class.std::__1::basic_string"** %12, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %1023, %"class.std::__1::basic_string"** %13, align 8, !noalias !20
  %1024 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !20
  %1025 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %1024, %"class.std::__1::basic_string"** %10, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %1025, %"class.std::__1::basic_string"** %11, align 8, !noalias !20
  %1026 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !20
  %1027 = bitcast %"class.std::__1::basic_string"* %1026 to %"class.std::__1::__basic_string_common"*
  %1028 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1026, i32 0, i32 0
  %1029 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !20
  %1030 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1029, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1030, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !20
  %1031 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !20
  %1032 = bitcast %"class.std::__1::__compressed_pair"* %1028 to i8*
  %1033 = bitcast %"class.std::__1::__compressed_pair"* %1031 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1032, i8* %1033, i64 24, i32 8, i1 false) #10
  %1034 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !20
  store %"class.std::__1::basic_string"* %1034, %"class.std::__1::basic_string"** %6, align 8, !noalias !20
  %1035 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !20
  %1036 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %1035, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %1036, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !20
  %1037 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !20
  %1038 = bitcast %"class.std::__1::__compressed_pair"* %1037 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %1038, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !20
  %1039 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !20
  %1040 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %1039, i32 0, i32 0
  %1041 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %1040, i32 0, i32 0
  %1042 = bitcast %union.anon.0* %1041 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %1043 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %1042, i32 0, i32 0
  store [3 x i64]* %1043, [3 x i64]** %7, align 8, !noalias !20
  store i32 0, i32* %8, align 4, !noalias !20
  br label %1044

; <label>:1044:                                   ; preds = %1047, %1022
  %1045 = load i32, i32* %8, align 4, !noalias !20
  %1046 = icmp ult i32 %1045, 3
  br i1 %1046, label %1047, label %1054

; <label>:1047:                                   ; preds = %1044
  %1048 = load i32, i32* %8, align 4, !noalias !20
  %1049 = zext i32 %1048 to i64
  %1050 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !20
  %1051 = getelementptr inbounds [3 x i64], [3 x i64]* %1050, i64 0, i64 %1049
  store i64 0, i64* %1051, align 8
  %1052 = load i32, i32* %8, align 4, !noalias !20
  %1053 = add i32 %1052, 1
  store i32 %1053, i32* %8, align 4, !noalias !20
  br label %1044

; <label>:1054:                                   ; preds = %1044
  br label %1055

; <label>:1055:                                   ; preds = %1054
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %212, %class.AstNode* null)
          to label %1056 unwind label %1074

; <label>:1056:                                   ; preds = %1055
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %212) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %216) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %213) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %214) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %215) #10
  br label %1057

; <label>:1057:                                   ; preds = %1056, %837, %828, %819, %810, %795, %787, %778, %769, %754, %745, %737, %728, %713, %704, %695, %687, %669, %613, %493, %350
  ret void

; <label>:1058:                                   ; preds = %848
  %1059 = landingpad { i8*, i32 }
          cleanup
  %1060 = extractvalue { i8*, i32 } %1059, 0
  store i8* %1060, i8** %204, align 8
  %1061 = extractvalue { i8*, i32 } %1059, 1
  store i32 %1061, i32* %205, align 4
  br label %1081

; <label>:1062:                                   ; preds = %888
  %1063 = landingpad { i8*, i32 }
          cleanup
  %1064 = extractvalue { i8*, i32 } %1063, 0
  store i8* %1064, i8** %204, align 8
  %1065 = extractvalue { i8*, i32 } %1063, 1
  store i32 %1065, i32* %205, align 4
  br label %1080

; <label>:1066:                                   ; preds = %925
  %1067 = landingpad { i8*, i32 }
          cleanup
  %1068 = extractvalue { i8*, i32 } %1067, 0
  store i8* %1068, i8** %204, align 8
  %1069 = extractvalue { i8*, i32 } %1067, 1
  store i32 %1069, i32* %205, align 4
  br label %1079

; <label>:1070:                                   ; preds = %1019
  %1071 = landingpad { i8*, i32 }
          cleanup
  %1072 = extractvalue { i8*, i32 } %1071, 0
  store i8* %1072, i8** %204, align 8
  %1073 = extractvalue { i8*, i32 } %1071, 1
  store i32 %1073, i32* %205, align 4
  br label %1078

; <label>:1074:                                   ; preds = %1055
  %1075 = landingpad { i8*, i32 }
          cleanup
  %1076 = extractvalue { i8*, i32 } %1075, 0
  store i8* %1076, i8** %204, align 8
  %1077 = extractvalue { i8*, i32 } %1075, 1
  store i32 %1077, i32* %205, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %212) #10
  br label %1078

; <label>:1078:                                   ; preds = %1074, %1070
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %216) #10
  br label %1079

; <label>:1079:                                   ; preds = %1078, %1066
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %213) #10
  br label %1080

; <label>:1080:                                   ; preds = %1079, %1062
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %214) #10
  br label %1081

; <label>:1081:                                   ; preds = %1080, %1058
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %215) #10
  br label %1082

; <label>:1082:                                   ; preds = %1081, %678, %622, %502, %359
  %1083 = load i8*, i8** %204, align 8
  %1084 = load i32, i32* %205, align 4
  %1085 = insertvalue { i8*, i32 } undef, i8* %1083, 0
  %1086 = insertvalue { i8*, i32 } %1085, i32 %1084, 1
  resume { i8*, i32 } %1086
}

declare void @_ZN7Variant9to_stringEb(%"class.std::__1::basic_string"* sret, %struct.Variant*, i1 zeroext) #1

declare i32 @__gxx_personality_v0(...)

declare void @_ZN7VariantC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%struct.Variant*, %"class.std::__1::basic_string"*) unnamed_addr #1

; Function Attrs: nounwind
declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"*) unnamed_addr #2

declare void @_ZN7VariantC1Ei(%struct.Variant*, i32) unnamed_addr #1

declare void @_ZN7VariantC1El(%struct.Variant*, i64) unnamed_addr #1

declare void @_ZN7VariantC1Ef(%struct.Variant*, float) unnamed_addr #1

declare void @_ZN7VariantC1Ed(%struct.Variant*, double) unnamed_addr #1

declare void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32, %"class.std::__1::basic_string"*, %class.AstNode*) #1

declare void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret, i32) #1

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic5minusE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %struct.Variant, align 8
  %5 = alloca %struct.Variant, align 8
  %6 = alloca i8*
  %7 = alloca i32
  call void @_ZN7VariantC1ERKS_(%struct.Variant* %4, %struct.Variant* dereferenceable(264) %1)
  invoke void @_ZN7VariantngEv(%struct.Variant* sret %5, %struct.Variant* %2)
          to label %8 unwind label %10

; <label>:8:                                      ; preds = %3
  invoke void @_ZN4cdot3var10Arithmetic3addE7VariantS2_(%struct.Variant* sret %0, %struct.Variant* %4, %struct.Variant* %5)
          to label %9 unwind label %14

; <label>:9:                                      ; preds = %8
  call void @_ZN7VariantD1Ev(%struct.Variant* %5) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %4) #10
  ret void

; <label>:10:                                     ; preds = %3
  %11 = landingpad { i8*, i32 }
          cleanup
  %12 = extractvalue { i8*, i32 } %11, 0
  store i8* %12, i8** %6, align 8
  %13 = extractvalue { i8*, i32 } %11, 1
  store i32 %13, i32* %7, align 4
  br label %18

; <label>:14:                                     ; preds = %8
  %15 = landingpad { i8*, i32 }
          cleanup
  %16 = extractvalue { i8*, i32 } %15, 0
  store i8* %16, i8** %6, align 8
  %17 = extractvalue { i8*, i32 } %15, 1
  store i32 %17, i32* %7, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %5) #10
  br label %18

; <label>:18:                                     ; preds = %14, %10
  call void @_ZN7VariantD1Ev(%struct.Variant* %4) #10
  br label %19

; <label>:19:                                     ; preds = %18
  %20 = load i8*, i8** %6, align 8
  %21 = load i32, i32* %7, align 4
  %22 = insertvalue { i8*, i32 } undef, i8* %20, 0
  %23 = insertvalue { i8*, i32 } %22, i32 %21, 1
  resume { i8*, i32 } %23
}

declare void @_ZN7VariantC1ERKS_(%struct.Variant*, %struct.Variant* dereferenceable(264)) unnamed_addr #1

declare void @_ZN7VariantngEv(%struct.Variant* sret, %struct.Variant*) #1

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN7VariantD1Ev(%struct.Variant*) unnamed_addr #3 align 2 {
  %2 = alloca %struct.Variant*, align 8
  store %struct.Variant* %0, %struct.Variant** %2, align 8
  %3 = load %struct.Variant*, %struct.Variant** %2, align 8
  call void @_ZN7VariantD2Ev(%struct.Variant* %3) #10
  ret void
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic8multiplyE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  %69 = alloca %"class.std::__1::basic_string", align 8
  %70 = alloca %"class.std::__1::basic_string", align 8
  %71 = alloca %"class.std::__1::basic_string", align 8
  %72 = alloca %"class.std::__1::basic_string", align 8
  %73 = alloca i8*
  %74 = alloca i32
  %75 = alloca %"class.std::__1::basic_string", align 8
  %76 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %77 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %76, i32 0, i32 0
  %78 = load i32, i32* %77, align 8
  switch i32 %78, label %243 [
    i32 0, label %79
    i32 1, label %120
    i32 2, label %161
    i32 3, label %202
  ]

; <label>:79:                                     ; preds = %3
  %80 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %81 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %80, i32 0, i32 0
  %82 = load i32, i32* %81, align 8
  switch i32 %82, label %118 [
    i32 0, label %83
    i32 1, label %91
    i32 2, label %100
    i32 3, label %109
  ]

; <label>:83:                                     ; preds = %79
  %84 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %85 = bitcast %union.anon* %84 to i32*
  %86 = load i32, i32* %85, align 8
  %87 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %88 = bitcast %union.anon* %87 to i32*
  %89 = load i32, i32* %88, align 8
  %90 = mul nsw i32 %86, %89
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %90)
  br label %453

; <label>:91:                                     ; preds = %79
  %92 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %93 = bitcast %union.anon* %92 to i32*
  %94 = load i32, i32* %93, align 8
  %95 = sext i32 %94 to i64
  %96 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %97 = bitcast %union.anon* %96 to i64*
  %98 = load i64, i64* %97, align 8
  %99 = mul nsw i64 %95, %98
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %99)
  br label %453

; <label>:100:                                    ; preds = %79
  %101 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %102 = bitcast %union.anon* %101 to i32*
  %103 = load i32, i32* %102, align 8
  %104 = sitofp i32 %103 to float
  %105 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %106 = bitcast %union.anon* %105 to float*
  %107 = load float, float* %106, align 8
  %108 = fmul float %104, %107
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %108)
  br label %453

; <label>:109:                                    ; preds = %79
  %110 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %111 = bitcast %union.anon* %110 to i32*
  %112 = load i32, i32* %111, align 8
  %113 = sitofp i32 %112 to double
  %114 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %115 = bitcast %union.anon* %114 to double*
  %116 = load double, double* %115, align 8
  %117 = fmul double %113, %116
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %117)
  br label %453

; <label>:118:                                    ; preds = %79
  br label %119

; <label>:119:                                    ; preds = %118
  br label %244

; <label>:120:                                    ; preds = %3
  %121 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %122 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %121, i32 0, i32 0
  %123 = load i32, i32* %122, align 8
  switch i32 %123, label %159 [
    i32 0, label %124
    i32 1, label %133
    i32 2, label %141
    i32 3, label %150
  ]

; <label>:124:                                    ; preds = %120
  %125 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %126 = bitcast %union.anon* %125 to i64*
  %127 = load i64, i64* %126, align 8
  %128 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %129 = bitcast %union.anon* %128 to i32*
  %130 = load i32, i32* %129, align 8
  %131 = sext i32 %130 to i64
  %132 = mul nsw i64 %127, %131
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %132)
  br label %453

; <label>:133:                                    ; preds = %120
  %134 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %135 = bitcast %union.anon* %134 to i64*
  %136 = load i64, i64* %135, align 8
  %137 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %138 = bitcast %union.anon* %137 to i64*
  %139 = load i64, i64* %138, align 8
  %140 = mul nsw i64 %136, %139
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %140)
  br label %453

; <label>:141:                                    ; preds = %120
  %142 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %143 = bitcast %union.anon* %142 to i64*
  %144 = load i64, i64* %143, align 8
  %145 = sitofp i64 %144 to float
  %146 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %147 = bitcast %union.anon* %146 to float*
  %148 = load float, float* %147, align 8
  %149 = fmul float %145, %148
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %149)
  br label %453

; <label>:150:                                    ; preds = %120
  %151 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %152 = bitcast %union.anon* %151 to i64*
  %153 = load i64, i64* %152, align 8
  %154 = sitofp i64 %153 to double
  %155 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %156 = bitcast %union.anon* %155 to double*
  %157 = load double, double* %156, align 8
  %158 = fmul double %154, %157
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %158)
  br label %453

; <label>:159:                                    ; preds = %120
  br label %160

; <label>:160:                                    ; preds = %159
  br label %244

; <label>:161:                                    ; preds = %3
  %162 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %163 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %162, i32 0, i32 0
  %164 = load i32, i32* %163, align 8
  switch i32 %164, label %200 [
    i32 0, label %165
    i32 1, label %174
    i32 2, label %183
    i32 3, label %191
  ]

; <label>:165:                                    ; preds = %161
  %166 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %167 = bitcast %union.anon* %166 to float*
  %168 = load float, float* %167, align 8
  %169 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %170 = bitcast %union.anon* %169 to i32*
  %171 = load i32, i32* %170, align 8
  %172 = sitofp i32 %171 to float
  %173 = fmul float %168, %172
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %173)
  br label %453

; <label>:174:                                    ; preds = %161
  %175 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %176 = bitcast %union.anon* %175 to float*
  %177 = load float, float* %176, align 8
  %178 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %179 = bitcast %union.anon* %178 to i64*
  %180 = load i64, i64* %179, align 8
  %181 = sitofp i64 %180 to float
  %182 = fmul float %177, %181
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %182)
  br label %453

; <label>:183:                                    ; preds = %161
  %184 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %185 = bitcast %union.anon* %184 to float*
  %186 = load float, float* %185, align 8
  %187 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %188 = bitcast %union.anon* %187 to float*
  %189 = load float, float* %188, align 8
  %190 = fmul float %186, %189
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %190)
  br label %453

; <label>:191:                                    ; preds = %161
  %192 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %193 = bitcast %union.anon* %192 to float*
  %194 = load float, float* %193, align 8
  %195 = fpext float %194 to double
  %196 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %197 = bitcast %union.anon* %196 to double*
  %198 = load double, double* %197, align 8
  %199 = fmul double %195, %198
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %199)
  br label %453

; <label>:200:                                    ; preds = %161
  br label %201

; <label>:201:                                    ; preds = %200
  br label %244

; <label>:202:                                    ; preds = %3
  %203 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %204 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %203, i32 0, i32 0
  %205 = load i32, i32* %204, align 8
  switch i32 %205, label %241 [
    i32 0, label %206
    i32 1, label %215
    i32 2, label %224
    i32 3, label %233
  ]

; <label>:206:                                    ; preds = %202
  %207 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %208 = bitcast %union.anon* %207 to double*
  %209 = load double, double* %208, align 8
  %210 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %211 = bitcast %union.anon* %210 to i32*
  %212 = load i32, i32* %211, align 8
  %213 = sitofp i32 %212 to double
  %214 = fmul double %209, %213
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %214)
  br label %453

; <label>:215:                                    ; preds = %202
  %216 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %217 = bitcast %union.anon* %216 to double*
  %218 = load double, double* %217, align 8
  %219 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %220 = bitcast %union.anon* %219 to i64*
  %221 = load i64, i64* %220, align 8
  %222 = sitofp i64 %221 to double
  %223 = fmul double %218, %222
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %223)
  br label %453

; <label>:224:                                    ; preds = %202
  %225 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %226 = bitcast %union.anon* %225 to double*
  %227 = load double, double* %226, align 8
  %228 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %229 = bitcast %union.anon* %228 to float*
  %230 = load float, float* %229, align 8
  %231 = fpext float %230 to double
  %232 = fmul double %227, %231
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %232)
  br label %453

; <label>:233:                                    ; preds = %202
  %234 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %235 = bitcast %union.anon* %234 to double*
  %236 = load double, double* %235, align 8
  %237 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %238 = bitcast %union.anon* %237 to double*
  %239 = load double, double* %238, align 8
  %240 = fmul double %236, %239
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %240)
  br label %453

; <label>:241:                                    ; preds = %202
  br label %242

; <label>:242:                                    ; preds = %241
  br label %244

; <label>:243:                                    ; preds = %3
  br label %244

; <label>:244:                                    ; preds = %243, %242, %201, %160, %119
  %245 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %246 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %245, i32 0, i32 0
  %247 = load i32, i32* %246, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %72, i32 %247)
  store i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str, i32 0, i32 0), i8** %67, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %72, %"class.std::__1::basic_string"** %68, align 8, !noalias !23
  %248 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !23
  %249 = load i8*, i8** %67, align 8, !noalias !23
  %250 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %248, i64 0, i8* %249)
          to label %251 unwind label %454

; <label>:251:                                    ; preds = %244
  store %"class.std::__1::basic_string"* %250, %"class.std::__1::basic_string"** %66, align 8, !noalias !23
  %252 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %64, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %252, %"class.std::__1::basic_string"** %65, align 8, !noalias !23
  %253 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !23
  %254 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %253, %"class.std::__1::basic_string"** %62, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %254, %"class.std::__1::basic_string"** %63, align 8, !noalias !23
  %255 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8, !noalias !23
  %256 = bitcast %"class.std::__1::basic_string"* %255 to %"class.std::__1::__basic_string_common"*
  %257 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %255, i32 0, i32 0
  %258 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !23
  %259 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %258, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %259, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !23
  %260 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !23
  %261 = bitcast %"class.std::__1::__compressed_pair"* %257 to i8*
  %262 = bitcast %"class.std::__1::__compressed_pair"* %260 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %261, i8* %262, i64 24, i32 8, i1 false) #10
  %263 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !23
  store %"class.std::__1::basic_string"* %263, %"class.std::__1::basic_string"** %58, align 8, !noalias !23
  %264 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8, !noalias !23
  %265 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %264, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %265, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !23
  %266 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !23
  %267 = bitcast %"class.std::__1::__compressed_pair"* %266 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %267, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !23
  %268 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !23
  %269 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %268, i32 0, i32 0
  %270 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %269, i32 0, i32 0
  %271 = bitcast %union.anon.0* %270 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %272 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %271, i32 0, i32 0
  store [3 x i64]* %272, [3 x i64]** %59, align 8, !noalias !23
  store i32 0, i32* %60, align 4, !noalias !23
  br label %273

; <label>:273:                                    ; preds = %276, %251
  %274 = load i32, i32* %60, align 4, !noalias !23
  %275 = icmp ult i32 %274, 3
  br i1 %275, label %276, label %283

; <label>:276:                                    ; preds = %273
  %277 = load i32, i32* %60, align 4, !noalias !23
  %278 = zext i32 %277 to i64
  %279 = load [3 x i64]*, [3 x i64]** %59, align 8, !noalias !23
  %280 = getelementptr inbounds [3 x i64], [3 x i64]* %279, i64 0, i64 %278
  store i64 0, i64* %280, align 8
  %281 = load i32, i32* %60, align 4, !noalias !23
  %282 = add i32 %281, 1
  store i32 %282, i32* %60, align 4, !noalias !23
  br label %273

; <label>:283:                                    ; preds = %273
  br label %284

; <label>:284:                                    ; preds = %283
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %54, align 8, !noalias !26
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %55, align 8, !noalias !26
  %285 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !26
  %286 = load i8*, i8** %55, align 8, !noalias !26
  %287 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %285, i8* %286)
          to label %288 unwind label %458

; <label>:288:                                    ; preds = %284
  store %"class.std::__1::basic_string"* %287, %"class.std::__1::basic_string"** %53, align 8, !noalias !26
  %289 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !26
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %51, align 8, !noalias !26
  store %"class.std::__1::basic_string"* %289, %"class.std::__1::basic_string"** %52, align 8, !noalias !26
  %290 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !26
  %291 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !26
  store %"class.std::__1::basic_string"* %290, %"class.std::__1::basic_string"** %49, align 8, !noalias !26
  store %"class.std::__1::basic_string"* %291, %"class.std::__1::basic_string"** %50, align 8, !noalias !26
  %292 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !26
  %293 = bitcast %"class.std::__1::basic_string"* %292 to %"class.std::__1::__basic_string_common"*
  %294 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %292, i32 0, i32 0
  %295 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !26
  %296 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %295, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %296, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !26
  %297 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !26
  %298 = bitcast %"class.std::__1::__compressed_pair"* %294 to i8*
  %299 = bitcast %"class.std::__1::__compressed_pair"* %297 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %298, i8* %299, i64 24, i32 8, i1 false) #10
  %300 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !26
  store %"class.std::__1::basic_string"* %300, %"class.std::__1::basic_string"** %45, align 8, !noalias !26
  %301 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !26
  %302 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %301, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %302, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !26
  %303 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !26
  %304 = bitcast %"class.std::__1::__compressed_pair"* %303 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %304, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !26
  %305 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !26
  %306 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %305, i32 0, i32 0
  %307 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %306, i32 0, i32 0
  %308 = bitcast %union.anon.0* %307 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %309 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %308, i32 0, i32 0
  store [3 x i64]* %309, [3 x i64]** %46, align 8, !noalias !26
  store i32 0, i32* %47, align 4, !noalias !26
  br label %310

; <label>:310:                                    ; preds = %313, %288
  %311 = load i32, i32* %47, align 4, !noalias !26
  %312 = icmp ult i32 %311, 3
  br i1 %312, label %313, label %320

; <label>:313:                                    ; preds = %310
  %314 = load i32, i32* %47, align 4, !noalias !26
  %315 = zext i32 %314 to i64
  %316 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !26
  %317 = getelementptr inbounds [3 x i64], [3 x i64]* %316, i64 0, i64 %315
  store i64 0, i64* %317, align 8
  %318 = load i32, i32* %47, align 4, !noalias !26
  %319 = add i32 %318, 1
  store i32 %319, i32* %47, align 4, !noalias !26
  br label %310

; <label>:320:                                    ; preds = %310
  br label %321

; <label>:321:                                    ; preds = %320
  %322 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %323 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %322, i32 0, i32 0
  %324 = load i32, i32* %323, align 8
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %75, i32 %324)
          to label %325 unwind label %462

; <label>:325:                                    ; preds = %321
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %41, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %75, %"class.std::__1::basic_string"** %42, align 8, !noalias !29
  %326 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !29
  %327 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %326, %"class.std::__1::basic_string"** %39, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %327, %"class.std::__1::basic_string"** %40, align 8, !noalias !29
  %328 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !29
  %329 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %329, %"class.std::__1::basic_string"** %38, align 8, !noalias !29
  %330 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %330, %"class.std::__1::basic_string"** %37, align 8, !noalias !29
  %331 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %331, %"class.std::__1::basic_string"** %36, align 8, !noalias !29
  %332 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !29
  %333 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %332, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %333, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !29
  %334 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !29
  %335 = bitcast %"class.std::__1::__compressed_pair"* %334 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %335, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !29
  %336 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !29
  %337 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %336, i32 0, i32 0
  %338 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %337, i32 0, i32 0
  %339 = bitcast %union.anon.0* %338 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %340 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %339, i32 0, i32 0
  %341 = bitcast %union.anon.1* %340 to i8*
  %342 = load i8, i8* %341, align 8, !noalias !29
  %343 = zext i8 %342 to i32
  %344 = and i32 %343, 1
  %345 = icmp ne i32 %344, 0
  br i1 %345, label %346, label %357

; <label>:346:                                    ; preds = %325
  store %"class.std::__1::basic_string"* %331, %"class.std::__1::basic_string"** %28, align 8, !noalias !29
  %347 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !29
  %348 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %347, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %348, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !29
  %349 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !29
  %350 = bitcast %"class.std::__1::__compressed_pair"* %349 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %350, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !29
  %351 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !29
  %352 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %351, i32 0, i32 0
  %353 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %352, i32 0, i32 0
  %354 = bitcast %union.anon.0* %353 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %355 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %354, i32 0, i32 2
  %356 = load i8*, i8** %355, align 8
  br label %370

; <label>:357:                                    ; preds = %325
  store %"class.std::__1::basic_string"* %331, %"class.std::__1::basic_string"** %33, align 8, !noalias !29
  %358 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !29
  %359 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %358, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %359, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !29
  %360 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !29
  %361 = bitcast %"class.std::__1::__compressed_pair"* %360 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %361, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !29
  %362 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !29
  %363 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %362, i32 0, i32 0
  %364 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %363, i32 0, i32 0
  %365 = bitcast %union.anon.0* %364 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %366 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %365, i32 0, i32 1
  %367 = getelementptr inbounds [23 x i8], [23 x i8]* %366, i64 0, i64 0
  store i8* %367, i8** %30, align 8, !noalias !29
  %368 = load i8*, i8** %30, align 8, !noalias !29
  store i8* %368, i8** %29, align 8, !noalias !29
  %369 = load i8*, i8** %29, align 8, !noalias !29
  br label %370

; <label>:370:                                    ; preds = %357, %346
  %371 = phi i8* [ %356, %346 ], [ %369, %357 ]
  store i8* %371, i8** %25, align 8, !noalias !29
  %372 = load i8*, i8** %25, align 8, !noalias !29
  %373 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %373, %"class.std::__1::basic_string"** %24, align 8, !noalias !29
  %374 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %23, align 8, !noalias !29
  %375 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !29
  %376 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %375, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %376, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !29
  %377 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !29
  %378 = bitcast %"class.std::__1::__compressed_pair"* %377 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %378, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !29
  %379 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !29
  %380 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %379, i32 0, i32 0
  %381 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %380, i32 0, i32 0
  %382 = bitcast %union.anon.0* %381 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %383 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %382, i32 0, i32 0
  %384 = bitcast %union.anon.1* %383 to i8*
  %385 = load i8, i8* %384, align 8, !noalias !29
  %386 = zext i8 %385 to i32
  %387 = and i32 %386, 1
  %388 = icmp ne i32 %387, 0
  br i1 %388, label %389, label %400

; <label>:389:                                    ; preds = %370
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %17, align 8, !noalias !29
  %390 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !29
  %391 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %390, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %391, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !29
  %392 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !29
  %393 = bitcast %"class.std::__1::__compressed_pair"* %392 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %393, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !29
  %394 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !29
  %395 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %394, i32 0, i32 0
  %396 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %395, i32 0, i32 0
  %397 = bitcast %union.anon.0* %396 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %398 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %397, i32 0, i32 1
  %399 = load i64, i64* %398, align 8
  br label %415

; <label>:400:                                    ; preds = %370
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %20, align 8, !noalias !29
  %401 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !29
  %402 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %401, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %402, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !29
  %403 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !29
  %404 = bitcast %"class.std::__1::__compressed_pair"* %403 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %404, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !29
  %405 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !29
  %406 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %405, i32 0, i32 0
  %407 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %406, i32 0, i32 0
  %408 = bitcast %union.anon.0* %407 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %409 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %408, i32 0, i32 0
  %410 = bitcast %union.anon.1* %409 to i8*
  %411 = load i8, i8* %410, align 8
  %412 = zext i8 %411 to i32
  %413 = ashr i32 %412, 1
  %414 = sext i32 %413 to i64
  br label %415

; <label>:415:                                    ; preds = %400, %389
  %416 = phi i64 [ %399, %389 ], [ %414, %400 ]
  %417 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %328, i8* %372, i64 %416)
          to label %418 unwind label %466

; <label>:418:                                    ; preds = %415
  store %"class.std::__1::basic_string"* %417, %"class.std::__1::basic_string"** %14, align 8, !noalias !29
  %419 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %69, %"class.std::__1::basic_string"** %12, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %419, %"class.std::__1::basic_string"** %13, align 8, !noalias !29
  %420 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !29
  %421 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %420, %"class.std::__1::basic_string"** %10, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %421, %"class.std::__1::basic_string"** %11, align 8, !noalias !29
  %422 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !29
  %423 = bitcast %"class.std::__1::basic_string"* %422 to %"class.std::__1::__basic_string_common"*
  %424 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %422, i32 0, i32 0
  %425 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !29
  %426 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %425, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %426, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !29
  %427 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !29
  %428 = bitcast %"class.std::__1::__compressed_pair"* %424 to i8*
  %429 = bitcast %"class.std::__1::__compressed_pair"* %427 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %428, i8* %429, i64 24, i32 8, i1 false) #10
  %430 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !29
  store %"class.std::__1::basic_string"* %430, %"class.std::__1::basic_string"** %6, align 8, !noalias !29
  %431 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !29
  %432 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %431, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %432, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !29
  %433 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !29
  %434 = bitcast %"class.std::__1::__compressed_pair"* %433 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %434, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !29
  %435 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !29
  %436 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %435, i32 0, i32 0
  %437 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %436, i32 0, i32 0
  %438 = bitcast %union.anon.0* %437 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %439 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %438, i32 0, i32 0
  store [3 x i64]* %439, [3 x i64]** %7, align 8, !noalias !29
  store i32 0, i32* %8, align 4, !noalias !29
  br label %440

; <label>:440:                                    ; preds = %443, %418
  %441 = load i32, i32* %8, align 4, !noalias !29
  %442 = icmp ult i32 %441, 3
  br i1 %442, label %443, label %450

; <label>:443:                                    ; preds = %440
  %444 = load i32, i32* %8, align 4, !noalias !29
  %445 = zext i32 %444 to i64
  %446 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !29
  %447 = getelementptr inbounds [3 x i64], [3 x i64]* %446, i64 0, i64 %445
  store i64 0, i64* %447, align 8
  %448 = load i32, i32* %8, align 4, !noalias !29
  %449 = add i32 %448, 1
  store i32 %449, i32* %8, align 4, !noalias !29
  br label %440

; <label>:450:                                    ; preds = %440
  br label %451

; <label>:451:                                    ; preds = %450
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %69, %class.AstNode* null)
          to label %452 unwind label %470

; <label>:452:                                    ; preds = %451
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %453

; <label>:453:                                    ; preds = %452, %233, %224, %215, %206, %191, %183, %174, %165, %150, %141, %133, %124, %109, %100, %91, %83
  ret void

; <label>:454:                                    ; preds = %244
  %455 = landingpad { i8*, i32 }
          cleanup
  %456 = extractvalue { i8*, i32 } %455, 0
  store i8* %456, i8** %73, align 8
  %457 = extractvalue { i8*, i32 } %455, 1
  store i32 %457, i32* %74, align 4
  br label %477

; <label>:458:                                    ; preds = %284
  %459 = landingpad { i8*, i32 }
          cleanup
  %460 = extractvalue { i8*, i32 } %459, 0
  store i8* %460, i8** %73, align 8
  %461 = extractvalue { i8*, i32 } %459, 1
  store i32 %461, i32* %74, align 4
  br label %476

; <label>:462:                                    ; preds = %321
  %463 = landingpad { i8*, i32 }
          cleanup
  %464 = extractvalue { i8*, i32 } %463, 0
  store i8* %464, i8** %73, align 8
  %465 = extractvalue { i8*, i32 } %463, 1
  store i32 %465, i32* %74, align 4
  br label %475

; <label>:466:                                    ; preds = %415
  %467 = landingpad { i8*, i32 }
          cleanup
  %468 = extractvalue { i8*, i32 } %467, 0
  store i8* %468, i8** %73, align 8
  %469 = extractvalue { i8*, i32 } %467, 1
  store i32 %469, i32* %74, align 4
  br label %474

; <label>:470:                                    ; preds = %451
  %471 = landingpad { i8*, i32 }
          cleanup
  %472 = extractvalue { i8*, i32 } %471, 0
  store i8* %472, i8** %73, align 8
  %473 = extractvalue { i8*, i32 } %471, 1
  store i32 %473, i32* %74, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  br label %474

; <label>:474:                                    ; preds = %470, %466
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  br label %475

; <label>:475:                                    ; preds = %474, %462
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  br label %476

; <label>:476:                                    ; preds = %475, %458
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  br label %477

; <label>:477:                                    ; preds = %476, %454
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %478

; <label>:478:                                    ; preds = %477
  %479 = load i8*, i8** %73, align 8
  %480 = load i32, i32* %74, align 4
  %481 = insertvalue { i8*, i32 } undef, i8* %479, 0
  %482 = insertvalue { i8*, i32 } %481, i32 %480, 1
  resume { i8*, i32 } %482
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic6divideE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  %69 = alloca %"class.std::__1::basic_string", align 8
  %70 = alloca %"class.std::__1::basic_string", align 8
  %71 = alloca %"class.std::__1::basic_string", align 8
  %72 = alloca %"class.std::__1::basic_string", align 8
  %73 = alloca i8*
  %74 = alloca i32
  %75 = alloca %"class.std::__1::basic_string", align 8
  %76 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %77 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %76, i32 0, i32 0
  %78 = load i32, i32* %77, align 8
  switch i32 %78, label %243 [
    i32 0, label %79
    i32 1, label %120
    i32 2, label %161
    i32 3, label %202
  ]

; <label>:79:                                     ; preds = %3
  %80 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %81 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %80, i32 0, i32 0
  %82 = load i32, i32* %81, align 8
  switch i32 %82, label %118 [
    i32 0, label %83
    i32 1, label %91
    i32 2, label %100
    i32 3, label %109
  ]

; <label>:83:                                     ; preds = %79
  %84 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %85 = bitcast %union.anon* %84 to i32*
  %86 = load i32, i32* %85, align 8
  %87 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %88 = bitcast %union.anon* %87 to i32*
  %89 = load i32, i32* %88, align 8
  %90 = sdiv i32 %86, %89
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %90)
  br label %453

; <label>:91:                                     ; preds = %79
  %92 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %93 = bitcast %union.anon* %92 to i32*
  %94 = load i32, i32* %93, align 8
  %95 = sext i32 %94 to i64
  %96 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %97 = bitcast %union.anon* %96 to i64*
  %98 = load i64, i64* %97, align 8
  %99 = sdiv i64 %95, %98
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %99)
  br label %453

; <label>:100:                                    ; preds = %79
  %101 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %102 = bitcast %union.anon* %101 to i32*
  %103 = load i32, i32* %102, align 8
  %104 = sitofp i32 %103 to float
  %105 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %106 = bitcast %union.anon* %105 to float*
  %107 = load float, float* %106, align 8
  %108 = fdiv float %104, %107
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %108)
  br label %453

; <label>:109:                                    ; preds = %79
  %110 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %111 = bitcast %union.anon* %110 to i32*
  %112 = load i32, i32* %111, align 8
  %113 = sitofp i32 %112 to double
  %114 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %115 = bitcast %union.anon* %114 to double*
  %116 = load double, double* %115, align 8
  %117 = fdiv double %113, %116
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %117)
  br label %453

; <label>:118:                                    ; preds = %79
  br label %119

; <label>:119:                                    ; preds = %118
  br label %244

; <label>:120:                                    ; preds = %3
  %121 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %122 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %121, i32 0, i32 0
  %123 = load i32, i32* %122, align 8
  switch i32 %123, label %159 [
    i32 0, label %124
    i32 1, label %133
    i32 2, label %141
    i32 3, label %150
  ]

; <label>:124:                                    ; preds = %120
  %125 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %126 = bitcast %union.anon* %125 to i64*
  %127 = load i64, i64* %126, align 8
  %128 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %129 = bitcast %union.anon* %128 to i32*
  %130 = load i32, i32* %129, align 8
  %131 = sext i32 %130 to i64
  %132 = sdiv i64 %127, %131
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %132)
  br label %453

; <label>:133:                                    ; preds = %120
  %134 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %135 = bitcast %union.anon* %134 to i64*
  %136 = load i64, i64* %135, align 8
  %137 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %138 = bitcast %union.anon* %137 to i64*
  %139 = load i64, i64* %138, align 8
  %140 = sdiv i64 %136, %139
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %140)
  br label %453

; <label>:141:                                    ; preds = %120
  %142 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %143 = bitcast %union.anon* %142 to i64*
  %144 = load i64, i64* %143, align 8
  %145 = sitofp i64 %144 to float
  %146 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %147 = bitcast %union.anon* %146 to float*
  %148 = load float, float* %147, align 8
  %149 = fdiv float %145, %148
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %149)
  br label %453

; <label>:150:                                    ; preds = %120
  %151 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %152 = bitcast %union.anon* %151 to i64*
  %153 = load i64, i64* %152, align 8
  %154 = sitofp i64 %153 to double
  %155 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %156 = bitcast %union.anon* %155 to double*
  %157 = load double, double* %156, align 8
  %158 = fdiv double %154, %157
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %158)
  br label %453

; <label>:159:                                    ; preds = %120
  br label %160

; <label>:160:                                    ; preds = %159
  br label %244

; <label>:161:                                    ; preds = %3
  %162 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %163 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %162, i32 0, i32 0
  %164 = load i32, i32* %163, align 8
  switch i32 %164, label %200 [
    i32 0, label %165
    i32 1, label %174
    i32 2, label %183
    i32 3, label %191
  ]

; <label>:165:                                    ; preds = %161
  %166 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %167 = bitcast %union.anon* %166 to float*
  %168 = load float, float* %167, align 8
  %169 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %170 = bitcast %union.anon* %169 to i32*
  %171 = load i32, i32* %170, align 8
  %172 = sitofp i32 %171 to float
  %173 = fdiv float %168, %172
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %173)
  br label %453

; <label>:174:                                    ; preds = %161
  %175 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %176 = bitcast %union.anon* %175 to float*
  %177 = load float, float* %176, align 8
  %178 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %179 = bitcast %union.anon* %178 to i64*
  %180 = load i64, i64* %179, align 8
  %181 = sitofp i64 %180 to float
  %182 = fdiv float %177, %181
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %182)
  br label %453

; <label>:183:                                    ; preds = %161
  %184 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %185 = bitcast %union.anon* %184 to float*
  %186 = load float, float* %185, align 8
  %187 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %188 = bitcast %union.anon* %187 to float*
  %189 = load float, float* %188, align 8
  %190 = fdiv float %186, %189
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %190)
  br label %453

; <label>:191:                                    ; preds = %161
  %192 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %193 = bitcast %union.anon* %192 to float*
  %194 = load float, float* %193, align 8
  %195 = fpext float %194 to double
  %196 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %197 = bitcast %union.anon* %196 to double*
  %198 = load double, double* %197, align 8
  %199 = fdiv double %195, %198
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %199)
  br label %453

; <label>:200:                                    ; preds = %161
  br label %201

; <label>:201:                                    ; preds = %200
  br label %244

; <label>:202:                                    ; preds = %3
  %203 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %204 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %203, i32 0, i32 0
  %205 = load i32, i32* %204, align 8
  switch i32 %205, label %241 [
    i32 0, label %206
    i32 1, label %215
    i32 2, label %224
    i32 3, label %233
  ]

; <label>:206:                                    ; preds = %202
  %207 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %208 = bitcast %union.anon* %207 to double*
  %209 = load double, double* %208, align 8
  %210 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %211 = bitcast %union.anon* %210 to i32*
  %212 = load i32, i32* %211, align 8
  %213 = sitofp i32 %212 to double
  %214 = fdiv double %209, %213
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %214)
  br label %453

; <label>:215:                                    ; preds = %202
  %216 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %217 = bitcast %union.anon* %216 to double*
  %218 = load double, double* %217, align 8
  %219 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %220 = bitcast %union.anon* %219 to i64*
  %221 = load i64, i64* %220, align 8
  %222 = sitofp i64 %221 to double
  %223 = fdiv double %218, %222
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %223)
  br label %453

; <label>:224:                                    ; preds = %202
  %225 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %226 = bitcast %union.anon* %225 to double*
  %227 = load double, double* %226, align 8
  %228 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %229 = bitcast %union.anon* %228 to float*
  %230 = load float, float* %229, align 8
  %231 = fpext float %230 to double
  %232 = fdiv double %227, %231
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %232)
  br label %453

; <label>:233:                                    ; preds = %202
  %234 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %235 = bitcast %union.anon* %234 to double*
  %236 = load double, double* %235, align 8
  %237 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %238 = bitcast %union.anon* %237 to double*
  %239 = load double, double* %238, align 8
  %240 = fdiv double %236, %239
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %240)
  br label %453

; <label>:241:                                    ; preds = %202
  br label %242

; <label>:242:                                    ; preds = %241
  br label %244

; <label>:243:                                    ; preds = %3
  br label %244

; <label>:244:                                    ; preds = %243, %242, %201, %160, %119
  %245 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %246 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %245, i32 0, i32 0
  %247 = load i32, i32* %246, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %72, i32 %247)
  store i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str, i32 0, i32 0), i8** %67, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %72, %"class.std::__1::basic_string"** %68, align 8, !noalias !32
  %248 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !32
  %249 = load i8*, i8** %67, align 8, !noalias !32
  %250 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %248, i64 0, i8* %249)
          to label %251 unwind label %454

; <label>:251:                                    ; preds = %244
  store %"class.std::__1::basic_string"* %250, %"class.std::__1::basic_string"** %66, align 8, !noalias !32
  %252 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %64, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %252, %"class.std::__1::basic_string"** %65, align 8, !noalias !32
  %253 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !32
  %254 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %253, %"class.std::__1::basic_string"** %62, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %254, %"class.std::__1::basic_string"** %63, align 8, !noalias !32
  %255 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8, !noalias !32
  %256 = bitcast %"class.std::__1::basic_string"* %255 to %"class.std::__1::__basic_string_common"*
  %257 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %255, i32 0, i32 0
  %258 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !32
  %259 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %258, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %259, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !32
  %260 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !32
  %261 = bitcast %"class.std::__1::__compressed_pair"* %257 to i8*
  %262 = bitcast %"class.std::__1::__compressed_pair"* %260 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %261, i8* %262, i64 24, i32 8, i1 false) #10
  %263 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !32
  store %"class.std::__1::basic_string"* %263, %"class.std::__1::basic_string"** %58, align 8, !noalias !32
  %264 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8, !noalias !32
  %265 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %264, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %265, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !32
  %266 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !32
  %267 = bitcast %"class.std::__1::__compressed_pair"* %266 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %267, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !32
  %268 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !32
  %269 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %268, i32 0, i32 0
  %270 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %269, i32 0, i32 0
  %271 = bitcast %union.anon.0* %270 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %272 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %271, i32 0, i32 0
  store [3 x i64]* %272, [3 x i64]** %59, align 8, !noalias !32
  store i32 0, i32* %60, align 4, !noalias !32
  br label %273

; <label>:273:                                    ; preds = %276, %251
  %274 = load i32, i32* %60, align 4, !noalias !32
  %275 = icmp ult i32 %274, 3
  br i1 %275, label %276, label %283

; <label>:276:                                    ; preds = %273
  %277 = load i32, i32* %60, align 4, !noalias !32
  %278 = zext i32 %277 to i64
  %279 = load [3 x i64]*, [3 x i64]** %59, align 8, !noalias !32
  %280 = getelementptr inbounds [3 x i64], [3 x i64]* %279, i64 0, i64 %278
  store i64 0, i64* %280, align 8
  %281 = load i32, i32* %60, align 4, !noalias !32
  %282 = add i32 %281, 1
  store i32 %282, i32* %60, align 4, !noalias !32
  br label %273

; <label>:283:                                    ; preds = %273
  br label %284

; <label>:284:                                    ; preds = %283
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %54, align 8, !noalias !35
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %55, align 8, !noalias !35
  %285 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !35
  %286 = load i8*, i8** %55, align 8, !noalias !35
  %287 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %285, i8* %286)
          to label %288 unwind label %458

; <label>:288:                                    ; preds = %284
  store %"class.std::__1::basic_string"* %287, %"class.std::__1::basic_string"** %53, align 8, !noalias !35
  %289 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !35
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %51, align 8, !noalias !35
  store %"class.std::__1::basic_string"* %289, %"class.std::__1::basic_string"** %52, align 8, !noalias !35
  %290 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !35
  %291 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !35
  store %"class.std::__1::basic_string"* %290, %"class.std::__1::basic_string"** %49, align 8, !noalias !35
  store %"class.std::__1::basic_string"* %291, %"class.std::__1::basic_string"** %50, align 8, !noalias !35
  %292 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !35
  %293 = bitcast %"class.std::__1::basic_string"* %292 to %"class.std::__1::__basic_string_common"*
  %294 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %292, i32 0, i32 0
  %295 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !35
  %296 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %295, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %296, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !35
  %297 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !35
  %298 = bitcast %"class.std::__1::__compressed_pair"* %294 to i8*
  %299 = bitcast %"class.std::__1::__compressed_pair"* %297 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %298, i8* %299, i64 24, i32 8, i1 false) #10
  %300 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !35
  store %"class.std::__1::basic_string"* %300, %"class.std::__1::basic_string"** %45, align 8, !noalias !35
  %301 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !35
  %302 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %301, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %302, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !35
  %303 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !35
  %304 = bitcast %"class.std::__1::__compressed_pair"* %303 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %304, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !35
  %305 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !35
  %306 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %305, i32 0, i32 0
  %307 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %306, i32 0, i32 0
  %308 = bitcast %union.anon.0* %307 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %309 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %308, i32 0, i32 0
  store [3 x i64]* %309, [3 x i64]** %46, align 8, !noalias !35
  store i32 0, i32* %47, align 4, !noalias !35
  br label %310

; <label>:310:                                    ; preds = %313, %288
  %311 = load i32, i32* %47, align 4, !noalias !35
  %312 = icmp ult i32 %311, 3
  br i1 %312, label %313, label %320

; <label>:313:                                    ; preds = %310
  %314 = load i32, i32* %47, align 4, !noalias !35
  %315 = zext i32 %314 to i64
  %316 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !35
  %317 = getelementptr inbounds [3 x i64], [3 x i64]* %316, i64 0, i64 %315
  store i64 0, i64* %317, align 8
  %318 = load i32, i32* %47, align 4, !noalias !35
  %319 = add i32 %318, 1
  store i32 %319, i32* %47, align 4, !noalias !35
  br label %310

; <label>:320:                                    ; preds = %310
  br label %321

; <label>:321:                                    ; preds = %320
  %322 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %323 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %322, i32 0, i32 0
  %324 = load i32, i32* %323, align 8
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %75, i32 %324)
          to label %325 unwind label %462

; <label>:325:                                    ; preds = %321
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %41, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %75, %"class.std::__1::basic_string"** %42, align 8, !noalias !38
  %326 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !38
  %327 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %326, %"class.std::__1::basic_string"** %39, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %327, %"class.std::__1::basic_string"** %40, align 8, !noalias !38
  %328 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !38
  %329 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %329, %"class.std::__1::basic_string"** %38, align 8, !noalias !38
  %330 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %330, %"class.std::__1::basic_string"** %37, align 8, !noalias !38
  %331 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %331, %"class.std::__1::basic_string"** %36, align 8, !noalias !38
  %332 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !38
  %333 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %332, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %333, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !38
  %334 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !38
  %335 = bitcast %"class.std::__1::__compressed_pair"* %334 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %335, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !38
  %336 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !38
  %337 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %336, i32 0, i32 0
  %338 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %337, i32 0, i32 0
  %339 = bitcast %union.anon.0* %338 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %340 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %339, i32 0, i32 0
  %341 = bitcast %union.anon.1* %340 to i8*
  %342 = load i8, i8* %341, align 8, !noalias !38
  %343 = zext i8 %342 to i32
  %344 = and i32 %343, 1
  %345 = icmp ne i32 %344, 0
  br i1 %345, label %346, label %357

; <label>:346:                                    ; preds = %325
  store %"class.std::__1::basic_string"* %331, %"class.std::__1::basic_string"** %28, align 8, !noalias !38
  %347 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !38
  %348 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %347, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %348, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !38
  %349 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !38
  %350 = bitcast %"class.std::__1::__compressed_pair"* %349 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %350, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !38
  %351 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !38
  %352 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %351, i32 0, i32 0
  %353 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %352, i32 0, i32 0
  %354 = bitcast %union.anon.0* %353 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %355 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %354, i32 0, i32 2
  %356 = load i8*, i8** %355, align 8
  br label %370

; <label>:357:                                    ; preds = %325
  store %"class.std::__1::basic_string"* %331, %"class.std::__1::basic_string"** %33, align 8, !noalias !38
  %358 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !38
  %359 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %358, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %359, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !38
  %360 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !38
  %361 = bitcast %"class.std::__1::__compressed_pair"* %360 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %361, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !38
  %362 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !38
  %363 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %362, i32 0, i32 0
  %364 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %363, i32 0, i32 0
  %365 = bitcast %union.anon.0* %364 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %366 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %365, i32 0, i32 1
  %367 = getelementptr inbounds [23 x i8], [23 x i8]* %366, i64 0, i64 0
  store i8* %367, i8** %30, align 8, !noalias !38
  %368 = load i8*, i8** %30, align 8, !noalias !38
  store i8* %368, i8** %29, align 8, !noalias !38
  %369 = load i8*, i8** %29, align 8, !noalias !38
  br label %370

; <label>:370:                                    ; preds = %357, %346
  %371 = phi i8* [ %356, %346 ], [ %369, %357 ]
  store i8* %371, i8** %25, align 8, !noalias !38
  %372 = load i8*, i8** %25, align 8, !noalias !38
  %373 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %373, %"class.std::__1::basic_string"** %24, align 8, !noalias !38
  %374 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %23, align 8, !noalias !38
  %375 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !38
  %376 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %375, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %376, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !38
  %377 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !38
  %378 = bitcast %"class.std::__1::__compressed_pair"* %377 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %378, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !38
  %379 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !38
  %380 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %379, i32 0, i32 0
  %381 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %380, i32 0, i32 0
  %382 = bitcast %union.anon.0* %381 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %383 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %382, i32 0, i32 0
  %384 = bitcast %union.anon.1* %383 to i8*
  %385 = load i8, i8* %384, align 8, !noalias !38
  %386 = zext i8 %385 to i32
  %387 = and i32 %386, 1
  %388 = icmp ne i32 %387, 0
  br i1 %388, label %389, label %400

; <label>:389:                                    ; preds = %370
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %17, align 8, !noalias !38
  %390 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !38
  %391 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %390, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %391, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !38
  %392 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !38
  %393 = bitcast %"class.std::__1::__compressed_pair"* %392 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %393, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !38
  %394 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !38
  %395 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %394, i32 0, i32 0
  %396 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %395, i32 0, i32 0
  %397 = bitcast %union.anon.0* %396 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %398 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %397, i32 0, i32 1
  %399 = load i64, i64* %398, align 8
  br label %415

; <label>:400:                                    ; preds = %370
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %20, align 8, !noalias !38
  %401 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !38
  %402 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %401, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %402, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !38
  %403 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !38
  %404 = bitcast %"class.std::__1::__compressed_pair"* %403 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %404, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !38
  %405 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !38
  %406 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %405, i32 0, i32 0
  %407 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %406, i32 0, i32 0
  %408 = bitcast %union.anon.0* %407 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %409 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %408, i32 0, i32 0
  %410 = bitcast %union.anon.1* %409 to i8*
  %411 = load i8, i8* %410, align 8
  %412 = zext i8 %411 to i32
  %413 = ashr i32 %412, 1
  %414 = sext i32 %413 to i64
  br label %415

; <label>:415:                                    ; preds = %400, %389
  %416 = phi i64 [ %399, %389 ], [ %414, %400 ]
  %417 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %328, i8* %372, i64 %416)
          to label %418 unwind label %466

; <label>:418:                                    ; preds = %415
  store %"class.std::__1::basic_string"* %417, %"class.std::__1::basic_string"** %14, align 8, !noalias !38
  %419 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %69, %"class.std::__1::basic_string"** %12, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %419, %"class.std::__1::basic_string"** %13, align 8, !noalias !38
  %420 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !38
  %421 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %420, %"class.std::__1::basic_string"** %10, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %421, %"class.std::__1::basic_string"** %11, align 8, !noalias !38
  %422 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !38
  %423 = bitcast %"class.std::__1::basic_string"* %422 to %"class.std::__1::__basic_string_common"*
  %424 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %422, i32 0, i32 0
  %425 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !38
  %426 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %425, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %426, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !38
  %427 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !38
  %428 = bitcast %"class.std::__1::__compressed_pair"* %424 to i8*
  %429 = bitcast %"class.std::__1::__compressed_pair"* %427 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %428, i8* %429, i64 24, i32 8, i1 false) #10
  %430 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !38
  store %"class.std::__1::basic_string"* %430, %"class.std::__1::basic_string"** %6, align 8, !noalias !38
  %431 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !38
  %432 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %431, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %432, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !38
  %433 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !38
  %434 = bitcast %"class.std::__1::__compressed_pair"* %433 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %434, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !38
  %435 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !38
  %436 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %435, i32 0, i32 0
  %437 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %436, i32 0, i32 0
  %438 = bitcast %union.anon.0* %437 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %439 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %438, i32 0, i32 0
  store [3 x i64]* %439, [3 x i64]** %7, align 8, !noalias !38
  store i32 0, i32* %8, align 4, !noalias !38
  br label %440

; <label>:440:                                    ; preds = %443, %418
  %441 = load i32, i32* %8, align 4, !noalias !38
  %442 = icmp ult i32 %441, 3
  br i1 %442, label %443, label %450

; <label>:443:                                    ; preds = %440
  %444 = load i32, i32* %8, align 4, !noalias !38
  %445 = zext i32 %444 to i64
  %446 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !38
  %447 = getelementptr inbounds [3 x i64], [3 x i64]* %446, i64 0, i64 %445
  store i64 0, i64* %447, align 8
  %448 = load i32, i32* %8, align 4, !noalias !38
  %449 = add i32 %448, 1
  store i32 %449, i32* %8, align 4, !noalias !38
  br label %440

; <label>:450:                                    ; preds = %440
  br label %451

; <label>:451:                                    ; preds = %450
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %69, %class.AstNode* null)
          to label %452 unwind label %470

; <label>:452:                                    ; preds = %451
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %453

; <label>:453:                                    ; preds = %452, %233, %224, %215, %206, %191, %183, %174, %165, %150, %141, %133, %124, %109, %100, %91, %83
  ret void

; <label>:454:                                    ; preds = %244
  %455 = landingpad { i8*, i32 }
          cleanup
  %456 = extractvalue { i8*, i32 } %455, 0
  store i8* %456, i8** %73, align 8
  %457 = extractvalue { i8*, i32 } %455, 1
  store i32 %457, i32* %74, align 4
  br label %477

; <label>:458:                                    ; preds = %284
  %459 = landingpad { i8*, i32 }
          cleanup
  %460 = extractvalue { i8*, i32 } %459, 0
  store i8* %460, i8** %73, align 8
  %461 = extractvalue { i8*, i32 } %459, 1
  store i32 %461, i32* %74, align 4
  br label %476

; <label>:462:                                    ; preds = %321
  %463 = landingpad { i8*, i32 }
          cleanup
  %464 = extractvalue { i8*, i32 } %463, 0
  store i8* %464, i8** %73, align 8
  %465 = extractvalue { i8*, i32 } %463, 1
  store i32 %465, i32* %74, align 4
  br label %475

; <label>:466:                                    ; preds = %415
  %467 = landingpad { i8*, i32 }
          cleanup
  %468 = extractvalue { i8*, i32 } %467, 0
  store i8* %468, i8** %73, align 8
  %469 = extractvalue { i8*, i32 } %467, 1
  store i32 %469, i32* %74, align 4
  br label %474

; <label>:470:                                    ; preds = %451
  %471 = landingpad { i8*, i32 }
          cleanup
  %472 = extractvalue { i8*, i32 } %471, 0
  store i8* %472, i8** %73, align 8
  %473 = extractvalue { i8*, i32 } %471, 1
  store i32 %473, i32* %74, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  br label %474

; <label>:474:                                    ; preds = %470, %466
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  br label %475

; <label>:475:                                    ; preds = %474, %462
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  br label %476

; <label>:476:                                    ; preds = %475, %458
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  br label %477

; <label>:477:                                    ; preds = %476, %454
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %478

; <label>:478:                                    ; preds = %477
  %479 = load i8*, i8** %73, align 8
  %480 = load i32, i32* %74, align 4
  %481 = insertvalue { i8*, i32 } undef, i8* %479, 0
  %482 = insertvalue { i8*, i32 } %481, i32 %480, 1
  resume { i8*, i32 } %482
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic6moduloE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  %69 = alloca %"class.std::__1::basic_string", align 8
  %70 = alloca %"class.std::__1::basic_string", align 8
  %71 = alloca %"class.std::__1::basic_string", align 8
  %72 = alloca %"class.std::__1::basic_string", align 8
  %73 = alloca i8*
  %74 = alloca i32
  %75 = alloca %"class.std::__1::basic_string", align 8
  %76 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %77 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %76, i32 0, i32 0
  %78 = load i32, i32* %77, align 8
  switch i32 %78, label %125 [
    i32 0, label %79
    i32 1, label %102
  ]

; <label>:79:                                     ; preds = %3
  %80 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %81 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %80, i32 0, i32 0
  %82 = load i32, i32* %81, align 8
  switch i32 %82, label %100 [
    i32 0, label %83
    i32 1, label %91
  ]

; <label>:83:                                     ; preds = %79
  %84 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %85 = bitcast %union.anon* %84 to i32*
  %86 = load i32, i32* %85, align 8
  %87 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %88 = bitcast %union.anon* %87 to i32*
  %89 = load i32, i32* %88, align 8
  %90 = srem i32 %86, %89
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %90)
  br label %334

; <label>:91:                                     ; preds = %79
  %92 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %93 = bitcast %union.anon* %92 to i32*
  %94 = load i32, i32* %93, align 8
  %95 = sext i32 %94 to i64
  %96 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %97 = bitcast %union.anon* %96 to i64*
  %98 = load i64, i64* %97, align 8
  %99 = srem i64 %95, %98
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %99)
  br label %334

; <label>:100:                                    ; preds = %79
  br label %101

; <label>:101:                                    ; preds = %100
  br label %125

; <label>:102:                                    ; preds = %3
  %103 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %104 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %103, i32 0, i32 0
  %105 = load i32, i32* %104, align 8
  switch i32 %105, label %123 [
    i32 0, label %106
    i32 1, label %115
  ]

; <label>:106:                                    ; preds = %102
  %107 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %108 = bitcast %union.anon* %107 to i64*
  %109 = load i64, i64* %108, align 8
  %110 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %111 = bitcast %union.anon* %110 to i32*
  %112 = load i32, i32* %111, align 8
  %113 = sext i32 %112 to i64
  %114 = srem i64 %109, %113
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %114)
  br label %334

; <label>:115:                                    ; preds = %102
  %116 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %117 = bitcast %union.anon* %116 to i64*
  %118 = load i64, i64* %117, align 8
  %119 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %120 = bitcast %union.anon* %119 to i64*
  %121 = load i64, i64* %120, align 8
  %122 = srem i64 %118, %121
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %122)
  br label %334

; <label>:123:                                    ; preds = %102
  br label %124

; <label>:124:                                    ; preds = %123
  br label %125

; <label>:125:                                    ; preds = %3, %124, %101
  %126 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %127 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %126, i32 0, i32 0
  %128 = load i32, i32* %127, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %72, i32 %128)
  store i8* getelementptr inbounds ([39 x i8], [39 x i8]* @.str.2, i32 0, i32 0), i8** %67, align 8, !noalias !41
  store %"class.std::__1::basic_string"* %72, %"class.std::__1::basic_string"** %68, align 8, !noalias !41
  %129 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !41
  %130 = load i8*, i8** %67, align 8, !noalias !41
  %131 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %129, i64 0, i8* %130)
          to label %132 unwind label %335

; <label>:132:                                    ; preds = %125
  store %"class.std::__1::basic_string"* %131, %"class.std::__1::basic_string"** %66, align 8, !noalias !41
  %133 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !41
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %64, align 8, !noalias !41
  store %"class.std::__1::basic_string"* %133, %"class.std::__1::basic_string"** %65, align 8, !noalias !41
  %134 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !41
  %135 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !41
  store %"class.std::__1::basic_string"* %134, %"class.std::__1::basic_string"** %62, align 8, !noalias !41
  store %"class.std::__1::basic_string"* %135, %"class.std::__1::basic_string"** %63, align 8, !noalias !41
  %136 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8, !noalias !41
  %137 = bitcast %"class.std::__1::basic_string"* %136 to %"class.std::__1::__basic_string_common"*
  %138 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %136, i32 0, i32 0
  %139 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !41
  %140 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %139, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %140, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !41
  %141 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !41
  %142 = bitcast %"class.std::__1::__compressed_pair"* %138 to i8*
  %143 = bitcast %"class.std::__1::__compressed_pair"* %141 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %142, i8* %143, i64 24, i32 8, i1 false) #10
  %144 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !41
  store %"class.std::__1::basic_string"* %144, %"class.std::__1::basic_string"** %58, align 8, !noalias !41
  %145 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8, !noalias !41
  %146 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %145, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %146, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !41
  %147 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !41
  %148 = bitcast %"class.std::__1::__compressed_pair"* %147 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %148, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !41
  %149 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !41
  %150 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %149, i32 0, i32 0
  %151 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %150, i32 0, i32 0
  %152 = bitcast %union.anon.0* %151 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %153 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %152, i32 0, i32 0
  store [3 x i64]* %153, [3 x i64]** %59, align 8, !noalias !41
  store i32 0, i32* %60, align 4, !noalias !41
  br label %154

; <label>:154:                                    ; preds = %157, %132
  %155 = load i32, i32* %60, align 4, !noalias !41
  %156 = icmp ult i32 %155, 3
  br i1 %156, label %157, label %164

; <label>:157:                                    ; preds = %154
  %158 = load i32, i32* %60, align 4, !noalias !41
  %159 = zext i32 %158 to i64
  %160 = load [3 x i64]*, [3 x i64]** %59, align 8, !noalias !41
  %161 = getelementptr inbounds [3 x i64], [3 x i64]* %160, i64 0, i64 %159
  store i64 0, i64* %161, align 8
  %162 = load i32, i32* %60, align 4, !noalias !41
  %163 = add i32 %162, 1
  store i32 %163, i32* %60, align 4, !noalias !41
  br label %154

; <label>:164:                                    ; preds = %154
  br label %165

; <label>:165:                                    ; preds = %164
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %54, align 8, !noalias !44
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %55, align 8, !noalias !44
  %166 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !44
  %167 = load i8*, i8** %55, align 8, !noalias !44
  %168 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %166, i8* %167)
          to label %169 unwind label %339

; <label>:169:                                    ; preds = %165
  store %"class.std::__1::basic_string"* %168, %"class.std::__1::basic_string"** %53, align 8, !noalias !44
  %170 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !44
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %51, align 8, !noalias !44
  store %"class.std::__1::basic_string"* %170, %"class.std::__1::basic_string"** %52, align 8, !noalias !44
  %171 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !44
  %172 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !44
  store %"class.std::__1::basic_string"* %171, %"class.std::__1::basic_string"** %49, align 8, !noalias !44
  store %"class.std::__1::basic_string"* %172, %"class.std::__1::basic_string"** %50, align 8, !noalias !44
  %173 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !44
  %174 = bitcast %"class.std::__1::basic_string"* %173 to %"class.std::__1::__basic_string_common"*
  %175 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %173, i32 0, i32 0
  %176 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !44
  %177 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %176, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %177, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !44
  %178 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !44
  %179 = bitcast %"class.std::__1::__compressed_pair"* %175 to i8*
  %180 = bitcast %"class.std::__1::__compressed_pair"* %178 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %179, i8* %180, i64 24, i32 8, i1 false) #10
  %181 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !44
  store %"class.std::__1::basic_string"* %181, %"class.std::__1::basic_string"** %45, align 8, !noalias !44
  %182 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !44
  %183 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %182, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %183, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !44
  %184 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !44
  %185 = bitcast %"class.std::__1::__compressed_pair"* %184 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %185, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !44
  %186 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !44
  %187 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %186, i32 0, i32 0
  %188 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %187, i32 0, i32 0
  %189 = bitcast %union.anon.0* %188 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %190 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %189, i32 0, i32 0
  store [3 x i64]* %190, [3 x i64]** %46, align 8, !noalias !44
  store i32 0, i32* %47, align 4, !noalias !44
  br label %191

; <label>:191:                                    ; preds = %194, %169
  %192 = load i32, i32* %47, align 4, !noalias !44
  %193 = icmp ult i32 %192, 3
  br i1 %193, label %194, label %201

; <label>:194:                                    ; preds = %191
  %195 = load i32, i32* %47, align 4, !noalias !44
  %196 = zext i32 %195 to i64
  %197 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !44
  %198 = getelementptr inbounds [3 x i64], [3 x i64]* %197, i64 0, i64 %196
  store i64 0, i64* %198, align 8
  %199 = load i32, i32* %47, align 4, !noalias !44
  %200 = add i32 %199, 1
  store i32 %200, i32* %47, align 4, !noalias !44
  br label %191

; <label>:201:                                    ; preds = %191
  br label %202

; <label>:202:                                    ; preds = %201
  %203 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %204 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %203, i32 0, i32 0
  %205 = load i32, i32* %204, align 8
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %75, i32 %205)
          to label %206 unwind label %343

; <label>:206:                                    ; preds = %202
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %41, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %75, %"class.std::__1::basic_string"** %42, align 8, !noalias !47
  %207 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !47
  %208 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %207, %"class.std::__1::basic_string"** %39, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %208, %"class.std::__1::basic_string"** %40, align 8, !noalias !47
  %209 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !47
  %210 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %210, %"class.std::__1::basic_string"** %38, align 8, !noalias !47
  %211 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %211, %"class.std::__1::basic_string"** %37, align 8, !noalias !47
  %212 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %212, %"class.std::__1::basic_string"** %36, align 8, !noalias !47
  %213 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !47
  %214 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %213, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %214, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !47
  %215 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !47
  %216 = bitcast %"class.std::__1::__compressed_pair"* %215 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %216, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !47
  %217 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !47
  %218 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %217, i32 0, i32 0
  %219 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %218, i32 0, i32 0
  %220 = bitcast %union.anon.0* %219 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %221 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %220, i32 0, i32 0
  %222 = bitcast %union.anon.1* %221 to i8*
  %223 = load i8, i8* %222, align 8, !noalias !47
  %224 = zext i8 %223 to i32
  %225 = and i32 %224, 1
  %226 = icmp ne i32 %225, 0
  br i1 %226, label %227, label %238

; <label>:227:                                    ; preds = %206
  store %"class.std::__1::basic_string"* %212, %"class.std::__1::basic_string"** %28, align 8, !noalias !47
  %228 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !47
  %229 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %228, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %229, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !47
  %230 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !47
  %231 = bitcast %"class.std::__1::__compressed_pair"* %230 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %231, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !47
  %232 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !47
  %233 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %232, i32 0, i32 0
  %234 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %233, i32 0, i32 0
  %235 = bitcast %union.anon.0* %234 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %236 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %235, i32 0, i32 2
  %237 = load i8*, i8** %236, align 8
  br label %251

; <label>:238:                                    ; preds = %206
  store %"class.std::__1::basic_string"* %212, %"class.std::__1::basic_string"** %33, align 8, !noalias !47
  %239 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !47
  %240 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %239, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %240, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !47
  %241 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !47
  %242 = bitcast %"class.std::__1::__compressed_pair"* %241 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %242, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !47
  %243 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !47
  %244 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %243, i32 0, i32 0
  %245 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %244, i32 0, i32 0
  %246 = bitcast %union.anon.0* %245 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %247 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %246, i32 0, i32 1
  %248 = getelementptr inbounds [23 x i8], [23 x i8]* %247, i64 0, i64 0
  store i8* %248, i8** %30, align 8, !noalias !47
  %249 = load i8*, i8** %30, align 8, !noalias !47
  store i8* %249, i8** %29, align 8, !noalias !47
  %250 = load i8*, i8** %29, align 8, !noalias !47
  br label %251

; <label>:251:                                    ; preds = %238, %227
  %252 = phi i8* [ %237, %227 ], [ %250, %238 ]
  store i8* %252, i8** %25, align 8, !noalias !47
  %253 = load i8*, i8** %25, align 8, !noalias !47
  %254 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %254, %"class.std::__1::basic_string"** %24, align 8, !noalias !47
  %255 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %255, %"class.std::__1::basic_string"** %23, align 8, !noalias !47
  %256 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !47
  %257 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %256, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %257, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !47
  %258 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !47
  %259 = bitcast %"class.std::__1::__compressed_pair"* %258 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %259, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !47
  %260 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !47
  %261 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %260, i32 0, i32 0
  %262 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %261, i32 0, i32 0
  %263 = bitcast %union.anon.0* %262 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %264 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %263, i32 0, i32 0
  %265 = bitcast %union.anon.1* %264 to i8*
  %266 = load i8, i8* %265, align 8, !noalias !47
  %267 = zext i8 %266 to i32
  %268 = and i32 %267, 1
  %269 = icmp ne i32 %268, 0
  br i1 %269, label %270, label %281

; <label>:270:                                    ; preds = %251
  store %"class.std::__1::basic_string"* %255, %"class.std::__1::basic_string"** %17, align 8, !noalias !47
  %271 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !47
  %272 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %271, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %272, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !47
  %273 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !47
  %274 = bitcast %"class.std::__1::__compressed_pair"* %273 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %274, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !47
  %275 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !47
  %276 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %275, i32 0, i32 0
  %277 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %276, i32 0, i32 0
  %278 = bitcast %union.anon.0* %277 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %279 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %278, i32 0, i32 1
  %280 = load i64, i64* %279, align 8
  br label %296

; <label>:281:                                    ; preds = %251
  store %"class.std::__1::basic_string"* %255, %"class.std::__1::basic_string"** %20, align 8, !noalias !47
  %282 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !47
  %283 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %282, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %283, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !47
  %284 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !47
  %285 = bitcast %"class.std::__1::__compressed_pair"* %284 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %285, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !47
  %286 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !47
  %287 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %286, i32 0, i32 0
  %288 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %287, i32 0, i32 0
  %289 = bitcast %union.anon.0* %288 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %290 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %289, i32 0, i32 0
  %291 = bitcast %union.anon.1* %290 to i8*
  %292 = load i8, i8* %291, align 8
  %293 = zext i8 %292 to i32
  %294 = ashr i32 %293, 1
  %295 = sext i32 %294 to i64
  br label %296

; <label>:296:                                    ; preds = %281, %270
  %297 = phi i64 [ %280, %270 ], [ %295, %281 ]
  %298 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %209, i8* %253, i64 %297)
          to label %299 unwind label %347

; <label>:299:                                    ; preds = %296
  store %"class.std::__1::basic_string"* %298, %"class.std::__1::basic_string"** %14, align 8, !noalias !47
  %300 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %69, %"class.std::__1::basic_string"** %12, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %300, %"class.std::__1::basic_string"** %13, align 8, !noalias !47
  %301 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !47
  %302 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %301, %"class.std::__1::basic_string"** %10, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %302, %"class.std::__1::basic_string"** %11, align 8, !noalias !47
  %303 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !47
  %304 = bitcast %"class.std::__1::basic_string"* %303 to %"class.std::__1::__basic_string_common"*
  %305 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %303, i32 0, i32 0
  %306 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !47
  %307 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %306, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %307, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !47
  %308 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !47
  %309 = bitcast %"class.std::__1::__compressed_pair"* %305 to i8*
  %310 = bitcast %"class.std::__1::__compressed_pair"* %308 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %309, i8* %310, i64 24, i32 8, i1 false) #10
  %311 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !47
  store %"class.std::__1::basic_string"* %311, %"class.std::__1::basic_string"** %6, align 8, !noalias !47
  %312 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !47
  %313 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %312, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %313, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !47
  %314 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !47
  %315 = bitcast %"class.std::__1::__compressed_pair"* %314 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %315, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !47
  %316 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !47
  %317 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %316, i32 0, i32 0
  %318 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %317, i32 0, i32 0
  %319 = bitcast %union.anon.0* %318 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %320 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %319, i32 0, i32 0
  store [3 x i64]* %320, [3 x i64]** %7, align 8, !noalias !47
  store i32 0, i32* %8, align 4, !noalias !47
  br label %321

; <label>:321:                                    ; preds = %324, %299
  %322 = load i32, i32* %8, align 4, !noalias !47
  %323 = icmp ult i32 %322, 3
  br i1 %323, label %324, label %331

; <label>:324:                                    ; preds = %321
  %325 = load i32, i32* %8, align 4, !noalias !47
  %326 = zext i32 %325 to i64
  %327 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !47
  %328 = getelementptr inbounds [3 x i64], [3 x i64]* %327, i64 0, i64 %326
  store i64 0, i64* %328, align 8
  %329 = load i32, i32* %8, align 4, !noalias !47
  %330 = add i32 %329, 1
  store i32 %330, i32* %8, align 4, !noalias !47
  br label %321

; <label>:331:                                    ; preds = %321
  br label %332

; <label>:332:                                    ; preds = %331
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %69, %class.AstNode* null)
          to label %333 unwind label %351

; <label>:333:                                    ; preds = %332
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %334

; <label>:334:                                    ; preds = %333, %115, %106, %91, %83
  ret void

; <label>:335:                                    ; preds = %125
  %336 = landingpad { i8*, i32 }
          cleanup
  %337 = extractvalue { i8*, i32 } %336, 0
  store i8* %337, i8** %73, align 8
  %338 = extractvalue { i8*, i32 } %336, 1
  store i32 %338, i32* %74, align 4
  br label %358

; <label>:339:                                    ; preds = %165
  %340 = landingpad { i8*, i32 }
          cleanup
  %341 = extractvalue { i8*, i32 } %340, 0
  store i8* %341, i8** %73, align 8
  %342 = extractvalue { i8*, i32 } %340, 1
  store i32 %342, i32* %74, align 4
  br label %357

; <label>:343:                                    ; preds = %202
  %344 = landingpad { i8*, i32 }
          cleanup
  %345 = extractvalue { i8*, i32 } %344, 0
  store i8* %345, i8** %73, align 8
  %346 = extractvalue { i8*, i32 } %344, 1
  store i32 %346, i32* %74, align 4
  br label %356

; <label>:347:                                    ; preds = %296
  %348 = landingpad { i8*, i32 }
          cleanup
  %349 = extractvalue { i8*, i32 } %348, 0
  store i8* %349, i8** %73, align 8
  %350 = extractvalue { i8*, i32 } %348, 1
  store i32 %350, i32* %74, align 4
  br label %355

; <label>:351:                                    ; preds = %332
  %352 = landingpad { i8*, i32 }
          cleanup
  %353 = extractvalue { i8*, i32 } %352, 0
  store i8* %353, i8** %73, align 8
  %354 = extractvalue { i8*, i32 } %352, 1
  store i32 %354, i32* %74, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  br label %355

; <label>:355:                                    ; preds = %351, %347
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  br label %356

; <label>:356:                                    ; preds = %355, %343
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  br label %357

; <label>:357:                                    ; preds = %356, %339
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  br label %358

; <label>:358:                                    ; preds = %357, %335
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %359

; <label>:359:                                    ; preds = %358
  %360 = load i8*, i8** %73, align 8
  %361 = load i32, i32* %74, align 4
  %362 = insertvalue { i8*, i32 } undef, i8* %360, 0
  %363 = insertvalue { i8*, i32 } %362, i32 %361, 1
  resume { i8*, i32 } %363
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic3powE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca i64, align 8
  %5 = alloca i32, align 4
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  %9 = alloca float, align 4
  %10 = alloca i64, align 8
  %11 = alloca double, align 8
  %12 = alloca float, align 4
  %13 = alloca i32, align 4
  %14 = alloca float, align 4
  %15 = alloca i64, align 8
  %16 = alloca float, align 4
  %17 = alloca float, align 4
  %18 = alloca float, align 4
  %19 = alloca double, align 8
  %20 = alloca double, align 8
  %21 = alloca i32, align 4
  %22 = alloca double, align 8
  %23 = alloca i64, align 8
  %24 = alloca double, align 8
  %25 = alloca float, align 4
  %26 = alloca i32, align 4
  %27 = alloca double, align 8
  %28 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %29 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %30 = alloca %"class.std::__1::basic_string"*, align 8
  %31 = alloca [3 x i64]*, align 8
  %32 = alloca i32, align 4
  %33 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %34 = alloca %"class.std::__1::basic_string"*, align 8
  %35 = alloca %"class.std::__1::basic_string"*, align 8
  %36 = alloca %"class.std::__1::basic_string"*, align 8
  %37 = alloca %"class.std::__1::basic_string"*, align 8
  %38 = alloca %"class.std::__1::basic_string"*, align 8
  %39 = alloca i8*, align 8
  %40 = alloca %"class.std::__1::basic_string"*, align 8
  %41 = alloca i32, align 4
  %42 = alloca float, align 4
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
  %56 = alloca i32, align 4
  %57 = alloca i64, align 8
  %58 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %59 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %60 = alloca %"class.std::__1::basic_string"*, align 8
  %61 = alloca [3 x i64]*, align 8
  %62 = alloca i32, align 4
  %63 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %64 = alloca %"class.std::__1::basic_string"*, align 8
  %65 = alloca %"class.std::__1::basic_string"*, align 8
  %66 = alloca %"class.std::__1::basic_string"*, align 8
  %67 = alloca %"class.std::__1::basic_string"*, align 8
  %68 = alloca %"class.std::__1::basic_string"*, align 8
  %69 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %70 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %71 = alloca %"class.std::__1::basic_string"*, align 8
  %72 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %73 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %74 = alloca %"class.std::__1::basic_string"*, align 8
  %75 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %76 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %77 = alloca %"class.std::__1::basic_string"*, align 8
  %78 = alloca %"class.std::__1::basic_string"*, align 8
  %79 = alloca i8*, align 8
  %80 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %81 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %82 = alloca %"class.std::__1::basic_string"*, align 8
  %83 = alloca i8*, align 8
  %84 = alloca i8*, align 8
  %85 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %86 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %87 = alloca %"class.std::__1::basic_string"*, align 8
  %88 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %89 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %90 = alloca %"class.std::__1::basic_string"*, align 8
  %91 = alloca %"class.std::__1::basic_string"*, align 8
  %92 = alloca %"class.std::__1::basic_string"*, align 8
  %93 = alloca %"class.std::__1::basic_string"*, align 8
  %94 = alloca %"class.std::__1::basic_string"*, align 8
  %95 = alloca %"class.std::__1::basic_string"*, align 8
  %96 = alloca %"class.std::__1::basic_string"*, align 8
  %97 = alloca i32, align 4
  %98 = alloca i32, align 4
  %99 = alloca %"class.std::__1::basic_string", align 8
  %100 = alloca %"class.std::__1::basic_string", align 8
  %101 = alloca %"class.std::__1::basic_string", align 8
  %102 = alloca %"class.std::__1::basic_string", align 8
  %103 = alloca i8*
  %104 = alloca i32
  %105 = alloca %"class.std::__1::basic_string", align 8
  %106 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %107 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %106, i32 0, i32 0
  %108 = load i32, i32* %107, align 8
  switch i32 %108, label %317 [
    i32 0, label %109
    i32 1, label %164
    i32 2, label %219
    i32 3, label %270
  ]

; <label>:109:                                    ; preds = %3
  %110 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %111 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %110, i32 0, i32 0
  %112 = load i32, i32* %111, align 8
  switch i32 %112, label %162 [
    i32 0, label %113
    i32 1, label %126
    i32 2, label %139
    i32 3, label %151
  ]

; <label>:113:                                    ; preds = %109
  %114 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %115 = bitcast %union.anon* %114 to i32*
  %116 = load i32, i32* %115, align 8
  %117 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %118 = bitcast %union.anon* %117 to i32*
  %119 = load i32, i32* %118, align 8
  store i32 %116, i32* %97, align 4
  store i32 %119, i32* %98, align 4
  %120 = load i32, i32* %97, align 4
  %121 = sitofp i32 %120 to double
  %122 = load i32, i32* %98, align 4
  %123 = sitofp i32 %122 to double
  %124 = call double @llvm.pow.f64(double %121, double %123) #10
  %125 = fptosi double %124 to i32
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %125)
  br label %527

; <label>:126:                                    ; preds = %109
  %127 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %128 = bitcast %union.anon* %127 to i32*
  %129 = load i32, i32* %128, align 8
  %130 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %131 = bitcast %union.anon* %130 to i64*
  %132 = load i64, i64* %131, align 8
  store i32 %129, i32* %56, align 4
  store i64 %132, i64* %57, align 8
  %133 = load i32, i32* %56, align 4
  %134 = sitofp i32 %133 to double
  %135 = load i64, i64* %57, align 8
  %136 = sitofp i64 %135 to double
  %137 = call double @llvm.pow.f64(double %134, double %136) #10
  %138 = fptosi double %137 to i32
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %138)
  br label %527

; <label>:139:                                    ; preds = %109
  %140 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %141 = bitcast %union.anon* %140 to i32*
  %142 = load i32, i32* %141, align 8
  %143 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %144 = bitcast %union.anon* %143 to float*
  %145 = load float, float* %144, align 8
  store i32 %142, i32* %41, align 4
  store float %145, float* %42, align 4
  %146 = load i32, i32* %41, align 4
  %147 = sitofp i32 %146 to double
  %148 = load float, float* %42, align 4
  %149 = fpext float %148 to double
  %150 = call double @llvm.pow.f64(double %147, double %149) #10
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %150)
  br label %527

; <label>:151:                                    ; preds = %109
  %152 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %153 = bitcast %union.anon* %152 to i32*
  %154 = load i32, i32* %153, align 8
  %155 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %156 = bitcast %union.anon* %155 to double*
  %157 = load double, double* %156, align 8
  store i32 %154, i32* %26, align 4
  store double %157, double* %27, align 8
  %158 = load i32, i32* %26, align 4
  %159 = sitofp i32 %158 to double
  %160 = load double, double* %27, align 8
  %161 = call double @llvm.pow.f64(double %159, double %160) #10
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %161)
  br label %527

; <label>:162:                                    ; preds = %109
  br label %163

; <label>:163:                                    ; preds = %162
  br label %318

; <label>:164:                                    ; preds = %3
  %165 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %166 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %165, i32 0, i32 0
  %167 = load i32, i32* %166, align 8
  switch i32 %167, label %217 [
    i32 0, label %168
    i32 1, label %181
    i32 2, label %194
    i32 3, label %206
  ]

; <label>:168:                                    ; preds = %164
  %169 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %170 = bitcast %union.anon* %169 to i64*
  %171 = load i64, i64* %170, align 8
  %172 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %173 = bitcast %union.anon* %172 to i32*
  %174 = load i32, i32* %173, align 8
  store i64 %171, i64* %4, align 8
  store i32 %174, i32* %5, align 4
  %175 = load i64, i64* %4, align 8
  %176 = sitofp i64 %175 to double
  %177 = load i32, i32* %5, align 4
  %178 = sitofp i32 %177 to double
  %179 = call double @llvm.pow.f64(double %176, double %178) #10
  %180 = fptosi double %179 to i32
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %180)
  br label %527

; <label>:181:                                    ; preds = %164
  %182 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %183 = bitcast %union.anon* %182 to i64*
  %184 = load i64, i64* %183, align 8
  %185 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %186 = bitcast %union.anon* %185 to i64*
  %187 = load i64, i64* %186, align 8
  store i64 %184, i64* %6, align 8
  store i64 %187, i64* %7, align 8
  %188 = load i64, i64* %6, align 8
  %189 = sitofp i64 %188 to double
  %190 = load i64, i64* %7, align 8
  %191 = sitofp i64 %190 to double
  %192 = call double @llvm.pow.f64(double %189, double %191) #10
  %193 = fptosi double %192 to i32
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %193)
  br label %527

; <label>:194:                                    ; preds = %164
  %195 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %196 = bitcast %union.anon* %195 to i64*
  %197 = load i64, i64* %196, align 8
  %198 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %199 = bitcast %union.anon* %198 to float*
  %200 = load float, float* %199, align 8
  store i64 %197, i64* %8, align 8
  store float %200, float* %9, align 4
  %201 = load i64, i64* %8, align 8
  %202 = sitofp i64 %201 to double
  %203 = load float, float* %9, align 4
  %204 = fpext float %203 to double
  %205 = call double @llvm.pow.f64(double %202, double %204) #10
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %205)
  br label %527

; <label>:206:                                    ; preds = %164
  %207 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %208 = bitcast %union.anon* %207 to i64*
  %209 = load i64, i64* %208, align 8
  %210 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %211 = bitcast %union.anon* %210 to double*
  %212 = load double, double* %211, align 8
  store i64 %209, i64* %10, align 8
  store double %212, double* %11, align 8
  %213 = load i64, i64* %10, align 8
  %214 = sitofp i64 %213 to double
  %215 = load double, double* %11, align 8
  %216 = call double @llvm.pow.f64(double %214, double %215) #10
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %216)
  br label %527

; <label>:217:                                    ; preds = %164
  br label %218

; <label>:218:                                    ; preds = %217
  br label %318

; <label>:219:                                    ; preds = %3
  %220 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %221 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %220, i32 0, i32 0
  %222 = load i32, i32* %221, align 8
  switch i32 %222, label %268 [
    i32 0, label %223
    i32 1, label %235
    i32 2, label %247
    i32 3, label %257
  ]

; <label>:223:                                    ; preds = %219
  %224 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %225 = bitcast %union.anon* %224 to float*
  %226 = load float, float* %225, align 8
  %227 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %228 = bitcast %union.anon* %227 to i32*
  %229 = load i32, i32* %228, align 8
  store float %226, float* %12, align 4
  store i32 %229, i32* %13, align 4
  %230 = load float, float* %12, align 4
  %231 = fpext float %230 to double
  %232 = load i32, i32* %13, align 4
  %233 = sitofp i32 %232 to double
  %234 = call double @llvm.pow.f64(double %231, double %233) #10
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %234)
  br label %527

; <label>:235:                                    ; preds = %219
  %236 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %237 = bitcast %union.anon* %236 to float*
  %238 = load float, float* %237, align 8
  %239 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %240 = bitcast %union.anon* %239 to i64*
  %241 = load i64, i64* %240, align 8
  store float %238, float* %14, align 4
  store i64 %241, i64* %15, align 8
  %242 = load float, float* %14, align 4
  %243 = fpext float %242 to double
  %244 = load i64, i64* %15, align 8
  %245 = sitofp i64 %244 to double
  %246 = call double @llvm.pow.f64(double %243, double %245) #10
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %246)
  br label %527

; <label>:247:                                    ; preds = %219
  %248 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %249 = bitcast %union.anon* %248 to float*
  %250 = load float, float* %249, align 8
  %251 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %252 = bitcast %union.anon* %251 to float*
  %253 = load float, float* %252, align 8
  store float %250, float* %16, align 4
  store float %253, float* %17, align 4
  %254 = load float, float* %16, align 4
  %255 = load float, float* %17, align 4
  %256 = call float @llvm.pow.f32(float %254, float %255) #10
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %256)
  br label %527

; <label>:257:                                    ; preds = %219
  %258 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %259 = bitcast %union.anon* %258 to float*
  %260 = load float, float* %259, align 8
  %261 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %262 = bitcast %union.anon* %261 to double*
  %263 = load double, double* %262, align 8
  store float %260, float* %18, align 4
  store double %263, double* %19, align 8
  %264 = load float, float* %18, align 4
  %265 = fpext float %264 to double
  %266 = load double, double* %19, align 8
  %267 = call double @llvm.pow.f64(double %265, double %266) #10
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %267)
  br label %527

; <label>:268:                                    ; preds = %219
  br label %269

; <label>:269:                                    ; preds = %268
  br label %318

; <label>:270:                                    ; preds = %3
  %271 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %272 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %271, i32 0, i32 0
  %273 = load i32, i32* %272, align 8
  switch i32 %273, label %315 [
    i32 0, label %274
    i32 1, label %285
    i32 2, label %296
    i32 3, label %307
  ]

; <label>:274:                                    ; preds = %270
  %275 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %276 = bitcast %union.anon* %275 to double*
  %277 = load double, double* %276, align 8
  %278 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %279 = bitcast %union.anon* %278 to i32*
  %280 = load i32, i32* %279, align 8
  store double %277, double* %20, align 8
  store i32 %280, i32* %21, align 4
  %281 = load double, double* %20, align 8
  %282 = load i32, i32* %21, align 4
  %283 = sitofp i32 %282 to double
  %284 = call double @llvm.pow.f64(double %281, double %283) #10
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %284)
  br label %527

; <label>:285:                                    ; preds = %270
  %286 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %287 = bitcast %union.anon* %286 to double*
  %288 = load double, double* %287, align 8
  %289 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %290 = bitcast %union.anon* %289 to i64*
  %291 = load i64, i64* %290, align 8
  store double %288, double* %22, align 8
  store i64 %291, i64* %23, align 8
  %292 = load double, double* %22, align 8
  %293 = load i64, i64* %23, align 8
  %294 = sitofp i64 %293 to double
  %295 = call double @llvm.pow.f64(double %292, double %294) #10
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %295)
  br label %527

; <label>:296:                                    ; preds = %270
  %297 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %298 = bitcast %union.anon* %297 to double*
  %299 = load double, double* %298, align 8
  %300 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %301 = bitcast %union.anon* %300 to float*
  %302 = load float, float* %301, align 8
  store double %299, double* %24, align 8
  store float %302, float* %25, align 4
  %303 = load double, double* %24, align 8
  %304 = load float, float* %25, align 4
  %305 = fpext float %304 to double
  %306 = call double @llvm.pow.f64(double %303, double %305) #10
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %306)
  br label %527

; <label>:307:                                    ; preds = %270
  %308 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %309 = bitcast %union.anon* %308 to double*
  %310 = load double, double* %309, align 8
  %311 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %312 = bitcast %union.anon* %311 to double*
  %313 = load double, double* %312, align 8
  %314 = call double @llvm.pow.f64(double %310, double %313)
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %314)
  br label %527

; <label>:315:                                    ; preds = %270
  br label %316

; <label>:316:                                    ; preds = %315
  br label %318

; <label>:317:                                    ; preds = %3
  br label %318

; <label>:318:                                    ; preds = %317, %316, %269, %218, %163
  %319 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %320 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %319, i32 0, i32 0
  %321 = load i32, i32* %320, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %102, i32 %321)
  store i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str, i32 0, i32 0), i8** %39, align 8, !noalias !50
  store %"class.std::__1::basic_string"* %102, %"class.std::__1::basic_string"** %40, align 8, !noalias !50
  %322 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !50
  %323 = load i8*, i8** %39, align 8, !noalias !50
  %324 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %322, i64 0, i8* %323)
          to label %325 unwind label %528

; <label>:325:                                    ; preds = %318
  store %"class.std::__1::basic_string"* %324, %"class.std::__1::basic_string"** %38, align 8, !noalias !50
  %326 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !50
  store %"class.std::__1::basic_string"* %101, %"class.std::__1::basic_string"** %36, align 8, !noalias !50
  store %"class.std::__1::basic_string"* %326, %"class.std::__1::basic_string"** %37, align 8, !noalias !50
  %327 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !50
  %328 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !50
  store %"class.std::__1::basic_string"* %327, %"class.std::__1::basic_string"** %34, align 8, !noalias !50
  store %"class.std::__1::basic_string"* %328, %"class.std::__1::basic_string"** %35, align 8, !noalias !50
  %329 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %34, align 8, !noalias !50
  %330 = bitcast %"class.std::__1::basic_string"* %329 to %"class.std::__1::__basic_string_common"*
  %331 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %329, i32 0, i32 0
  %332 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %35, align 8, !noalias !50
  %333 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %332, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %333, %"class.std::__1::__compressed_pair"** %33, align 8, !noalias !50
  %334 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %33, align 8, !noalias !50
  %335 = bitcast %"class.std::__1::__compressed_pair"* %331 to i8*
  %336 = bitcast %"class.std::__1::__compressed_pair"* %334 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %335, i8* %336, i64 24, i32 8, i1 false) #10
  %337 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %35, align 8, !noalias !50
  store %"class.std::__1::basic_string"* %337, %"class.std::__1::basic_string"** %30, align 8, !noalias !50
  %338 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %30, align 8, !noalias !50
  %339 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %338, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %339, %"class.std::__1::__compressed_pair"** %29, align 8, !noalias !50
  %340 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %29, align 8, !noalias !50
  %341 = bitcast %"class.std::__1::__compressed_pair"* %340 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %341, %"class.std::__1::__libcpp_compressed_pair_imp"** %28, align 8, !noalias !50
  %342 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %28, align 8, !noalias !50
  %343 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %342, i32 0, i32 0
  %344 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %343, i32 0, i32 0
  %345 = bitcast %union.anon.0* %344 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %346 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %345, i32 0, i32 0
  store [3 x i64]* %346, [3 x i64]** %31, align 8, !noalias !50
  store i32 0, i32* %32, align 4, !noalias !50
  br label %347

; <label>:347:                                    ; preds = %350, %325
  %348 = load i32, i32* %32, align 4, !noalias !50
  %349 = icmp ult i32 %348, 3
  br i1 %349, label %350, label %357

; <label>:350:                                    ; preds = %347
  %351 = load i32, i32* %32, align 4, !noalias !50
  %352 = zext i32 %351 to i64
  %353 = load [3 x i64]*, [3 x i64]** %31, align 8, !noalias !50
  %354 = getelementptr inbounds [3 x i64], [3 x i64]* %353, i64 0, i64 %352
  store i64 0, i64* %354, align 8
  %355 = load i32, i32* %32, align 4, !noalias !50
  %356 = add i32 %355, 1
  store i32 %356, i32* %32, align 4, !noalias !50
  br label %347

; <label>:357:                                    ; preds = %347
  br label %358

; <label>:358:                                    ; preds = %357
  store %"class.std::__1::basic_string"* %101, %"class.std::__1::basic_string"** %54, align 8, !noalias !53
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %55, align 8, !noalias !53
  %359 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !53
  %360 = load i8*, i8** %55, align 8, !noalias !53
  %361 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %359, i8* %360)
          to label %362 unwind label %532

; <label>:362:                                    ; preds = %358
  store %"class.std::__1::basic_string"* %361, %"class.std::__1::basic_string"** %53, align 8, !noalias !53
  %363 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !53
  store %"class.std::__1::basic_string"* %100, %"class.std::__1::basic_string"** %51, align 8, !noalias !53
  store %"class.std::__1::basic_string"* %363, %"class.std::__1::basic_string"** %52, align 8, !noalias !53
  %364 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !53
  %365 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !53
  store %"class.std::__1::basic_string"* %364, %"class.std::__1::basic_string"** %49, align 8, !noalias !53
  store %"class.std::__1::basic_string"* %365, %"class.std::__1::basic_string"** %50, align 8, !noalias !53
  %366 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !53
  %367 = bitcast %"class.std::__1::basic_string"* %366 to %"class.std::__1::__basic_string_common"*
  %368 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %366, i32 0, i32 0
  %369 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !53
  %370 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %369, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %370, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !53
  %371 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !53
  %372 = bitcast %"class.std::__1::__compressed_pair"* %368 to i8*
  %373 = bitcast %"class.std::__1::__compressed_pair"* %371 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %372, i8* %373, i64 24, i32 8, i1 false) #10
  %374 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !53
  store %"class.std::__1::basic_string"* %374, %"class.std::__1::basic_string"** %45, align 8, !noalias !53
  %375 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !53
  %376 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %375, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %376, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !53
  %377 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !53
  %378 = bitcast %"class.std::__1::__compressed_pair"* %377 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %378, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !53
  %379 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !53
  %380 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %379, i32 0, i32 0
  %381 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %380, i32 0, i32 0
  %382 = bitcast %union.anon.0* %381 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %383 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %382, i32 0, i32 0
  store [3 x i64]* %383, [3 x i64]** %46, align 8, !noalias !53
  store i32 0, i32* %47, align 4, !noalias !53
  br label %384

; <label>:384:                                    ; preds = %387, %362
  %385 = load i32, i32* %47, align 4, !noalias !53
  %386 = icmp ult i32 %385, 3
  br i1 %386, label %387, label %394

; <label>:387:                                    ; preds = %384
  %388 = load i32, i32* %47, align 4, !noalias !53
  %389 = zext i32 %388 to i64
  %390 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !53
  %391 = getelementptr inbounds [3 x i64], [3 x i64]* %390, i64 0, i64 %389
  store i64 0, i64* %391, align 8
  %392 = load i32, i32* %47, align 4, !noalias !53
  %393 = add i32 %392, 1
  store i32 %393, i32* %47, align 4, !noalias !53
  br label %384

; <label>:394:                                    ; preds = %384
  br label %395

; <label>:395:                                    ; preds = %394
  %396 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %397 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %396, i32 0, i32 0
  %398 = load i32, i32* %397, align 8
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %105, i32 %398)
          to label %399 unwind label %536

; <label>:399:                                    ; preds = %395
  store %"class.std::__1::basic_string"* %100, %"class.std::__1::basic_string"** %95, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %105, %"class.std::__1::basic_string"** %96, align 8, !noalias !56
  %400 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %95, align 8, !noalias !56
  %401 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %96, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %400, %"class.std::__1::basic_string"** %93, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %401, %"class.std::__1::basic_string"** %94, align 8, !noalias !56
  %402 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %93, align 8, !noalias !56
  %403 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %94, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %403, %"class.std::__1::basic_string"** %92, align 8, !noalias !56
  %404 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %92, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %404, %"class.std::__1::basic_string"** %91, align 8, !noalias !56
  %405 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %91, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %405, %"class.std::__1::basic_string"** %90, align 8, !noalias !56
  %406 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %90, align 8, !noalias !56
  %407 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %406, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %407, %"class.std::__1::__compressed_pair"** %89, align 8, !noalias !56
  %408 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %89, align 8, !noalias !56
  %409 = bitcast %"class.std::__1::__compressed_pair"* %408 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %409, %"class.std::__1::__libcpp_compressed_pair_imp"** %88, align 8, !noalias !56
  %410 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %88, align 8, !noalias !56
  %411 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %410, i32 0, i32 0
  %412 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %411, i32 0, i32 0
  %413 = bitcast %union.anon.0* %412 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %414 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %413, i32 0, i32 0
  %415 = bitcast %union.anon.1* %414 to i8*
  %416 = load i8, i8* %415, align 8, !noalias !56
  %417 = zext i8 %416 to i32
  %418 = and i32 %417, 1
  %419 = icmp ne i32 %418, 0
  br i1 %419, label %420, label %431

; <label>:420:                                    ; preds = %399
  store %"class.std::__1::basic_string"* %405, %"class.std::__1::basic_string"** %82, align 8, !noalias !56
  %421 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %82, align 8, !noalias !56
  %422 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %421, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %422, %"class.std::__1::__compressed_pair"** %81, align 8, !noalias !56
  %423 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %81, align 8, !noalias !56
  %424 = bitcast %"class.std::__1::__compressed_pair"* %423 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %424, %"class.std::__1::__libcpp_compressed_pair_imp"** %80, align 8, !noalias !56
  %425 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %80, align 8, !noalias !56
  %426 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %425, i32 0, i32 0
  %427 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %426, i32 0, i32 0
  %428 = bitcast %union.anon.0* %427 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %429 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %428, i32 0, i32 2
  %430 = load i8*, i8** %429, align 8
  br label %444

; <label>:431:                                    ; preds = %399
  store %"class.std::__1::basic_string"* %405, %"class.std::__1::basic_string"** %87, align 8, !noalias !56
  %432 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %87, align 8, !noalias !56
  %433 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %432, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %433, %"class.std::__1::__compressed_pair"** %86, align 8, !noalias !56
  %434 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %86, align 8, !noalias !56
  %435 = bitcast %"class.std::__1::__compressed_pair"* %434 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %435, %"class.std::__1::__libcpp_compressed_pair_imp"** %85, align 8, !noalias !56
  %436 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %85, align 8, !noalias !56
  %437 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %436, i32 0, i32 0
  %438 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %437, i32 0, i32 0
  %439 = bitcast %union.anon.0* %438 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %440 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %439, i32 0, i32 1
  %441 = getelementptr inbounds [23 x i8], [23 x i8]* %440, i64 0, i64 0
  store i8* %441, i8** %84, align 8, !noalias !56
  %442 = load i8*, i8** %84, align 8, !noalias !56
  store i8* %442, i8** %83, align 8, !noalias !56
  %443 = load i8*, i8** %83, align 8, !noalias !56
  br label %444

; <label>:444:                                    ; preds = %431, %420
  %445 = phi i8* [ %430, %420 ], [ %443, %431 ]
  store i8* %445, i8** %79, align 8, !noalias !56
  %446 = load i8*, i8** %79, align 8, !noalias !56
  %447 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %94, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %447, %"class.std::__1::basic_string"** %78, align 8, !noalias !56
  %448 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %78, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %448, %"class.std::__1::basic_string"** %77, align 8, !noalias !56
  %449 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %77, align 8, !noalias !56
  %450 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %449, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %450, %"class.std::__1::__compressed_pair"** %76, align 8, !noalias !56
  %451 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %76, align 8, !noalias !56
  %452 = bitcast %"class.std::__1::__compressed_pair"* %451 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %452, %"class.std::__1::__libcpp_compressed_pair_imp"** %75, align 8, !noalias !56
  %453 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %75, align 8, !noalias !56
  %454 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %453, i32 0, i32 0
  %455 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %454, i32 0, i32 0
  %456 = bitcast %union.anon.0* %455 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %457 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %456, i32 0, i32 0
  %458 = bitcast %union.anon.1* %457 to i8*
  %459 = load i8, i8* %458, align 8, !noalias !56
  %460 = zext i8 %459 to i32
  %461 = and i32 %460, 1
  %462 = icmp ne i32 %461, 0
  br i1 %462, label %463, label %474

; <label>:463:                                    ; preds = %444
  store %"class.std::__1::basic_string"* %448, %"class.std::__1::basic_string"** %71, align 8, !noalias !56
  %464 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %71, align 8, !noalias !56
  %465 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %464, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %465, %"class.std::__1::__compressed_pair"** %70, align 8, !noalias !56
  %466 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %70, align 8, !noalias !56
  %467 = bitcast %"class.std::__1::__compressed_pair"* %466 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %467, %"class.std::__1::__libcpp_compressed_pair_imp"** %69, align 8, !noalias !56
  %468 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %69, align 8, !noalias !56
  %469 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %468, i32 0, i32 0
  %470 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %469, i32 0, i32 0
  %471 = bitcast %union.anon.0* %470 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %472 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %471, i32 0, i32 1
  %473 = load i64, i64* %472, align 8
  br label %489

; <label>:474:                                    ; preds = %444
  store %"class.std::__1::basic_string"* %448, %"class.std::__1::basic_string"** %74, align 8, !noalias !56
  %475 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %74, align 8, !noalias !56
  %476 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %475, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %476, %"class.std::__1::__compressed_pair"** %73, align 8, !noalias !56
  %477 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %73, align 8, !noalias !56
  %478 = bitcast %"class.std::__1::__compressed_pair"* %477 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %478, %"class.std::__1::__libcpp_compressed_pair_imp"** %72, align 8, !noalias !56
  %479 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %72, align 8, !noalias !56
  %480 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %479, i32 0, i32 0
  %481 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %480, i32 0, i32 0
  %482 = bitcast %union.anon.0* %481 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %483 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %482, i32 0, i32 0
  %484 = bitcast %union.anon.1* %483 to i8*
  %485 = load i8, i8* %484, align 8
  %486 = zext i8 %485 to i32
  %487 = ashr i32 %486, 1
  %488 = sext i32 %487 to i64
  br label %489

; <label>:489:                                    ; preds = %474, %463
  %490 = phi i64 [ %473, %463 ], [ %488, %474 ]
  %491 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %402, i8* %446, i64 %490)
          to label %492 unwind label %540

; <label>:492:                                    ; preds = %489
  store %"class.std::__1::basic_string"* %491, %"class.std::__1::basic_string"** %68, align 8, !noalias !56
  %493 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %99, %"class.std::__1::basic_string"** %66, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %493, %"class.std::__1::basic_string"** %67, align 8, !noalias !56
  %494 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !56
  %495 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %67, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %494, %"class.std::__1::basic_string"** %64, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %495, %"class.std::__1::basic_string"** %65, align 8, !noalias !56
  %496 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !56
  %497 = bitcast %"class.std::__1::basic_string"* %496 to %"class.std::__1::__basic_string_common"*
  %498 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %496, i32 0, i32 0
  %499 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !56
  %500 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %499, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %500, %"class.std::__1::__compressed_pair"** %63, align 8, !noalias !56
  %501 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %63, align 8, !noalias !56
  %502 = bitcast %"class.std::__1::__compressed_pair"* %498 to i8*
  %503 = bitcast %"class.std::__1::__compressed_pair"* %501 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %502, i8* %503, i64 24, i32 8, i1 false) #10
  %504 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !56
  store %"class.std::__1::basic_string"* %504, %"class.std::__1::basic_string"** %60, align 8, !noalias !56
  %505 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %60, align 8, !noalias !56
  %506 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %505, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %506, %"class.std::__1::__compressed_pair"** %59, align 8, !noalias !56
  %507 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %59, align 8, !noalias !56
  %508 = bitcast %"class.std::__1::__compressed_pair"* %507 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %508, %"class.std::__1::__libcpp_compressed_pair_imp"** %58, align 8, !noalias !56
  %509 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %58, align 8, !noalias !56
  %510 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %509, i32 0, i32 0
  %511 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %510, i32 0, i32 0
  %512 = bitcast %union.anon.0* %511 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %513 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %512, i32 0, i32 0
  store [3 x i64]* %513, [3 x i64]** %61, align 8, !noalias !56
  store i32 0, i32* %62, align 4, !noalias !56
  br label %514

; <label>:514:                                    ; preds = %517, %492
  %515 = load i32, i32* %62, align 4, !noalias !56
  %516 = icmp ult i32 %515, 3
  br i1 %516, label %517, label %524

; <label>:517:                                    ; preds = %514
  %518 = load i32, i32* %62, align 4, !noalias !56
  %519 = zext i32 %518 to i64
  %520 = load [3 x i64]*, [3 x i64]** %61, align 8, !noalias !56
  %521 = getelementptr inbounds [3 x i64], [3 x i64]* %520, i64 0, i64 %519
  store i64 0, i64* %521, align 8
  %522 = load i32, i32* %62, align 4, !noalias !56
  %523 = add i32 %522, 1
  store i32 %523, i32* %62, align 4, !noalias !56
  br label %514

; <label>:524:                                    ; preds = %514
  br label %525

; <label>:525:                                    ; preds = %524
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %99, %class.AstNode* null)
          to label %526 unwind label %544

; <label>:526:                                    ; preds = %525
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %99) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %105) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %100) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %101) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %102) #10
  br label %527

; <label>:527:                                    ; preds = %526, %307, %296, %285, %274, %257, %247, %235, %223, %206, %194, %181, %168, %151, %139, %126, %113
  ret void

; <label>:528:                                    ; preds = %318
  %529 = landingpad { i8*, i32 }
          cleanup
  %530 = extractvalue { i8*, i32 } %529, 0
  store i8* %530, i8** %103, align 8
  %531 = extractvalue { i8*, i32 } %529, 1
  store i32 %531, i32* %104, align 4
  br label %551

; <label>:532:                                    ; preds = %358
  %533 = landingpad { i8*, i32 }
          cleanup
  %534 = extractvalue { i8*, i32 } %533, 0
  store i8* %534, i8** %103, align 8
  %535 = extractvalue { i8*, i32 } %533, 1
  store i32 %535, i32* %104, align 4
  br label %550

; <label>:536:                                    ; preds = %395
  %537 = landingpad { i8*, i32 }
          cleanup
  %538 = extractvalue { i8*, i32 } %537, 0
  store i8* %538, i8** %103, align 8
  %539 = extractvalue { i8*, i32 } %537, 1
  store i32 %539, i32* %104, align 4
  br label %549

; <label>:540:                                    ; preds = %489
  %541 = landingpad { i8*, i32 }
          cleanup
  %542 = extractvalue { i8*, i32 } %541, 0
  store i8* %542, i8** %103, align 8
  %543 = extractvalue { i8*, i32 } %541, 1
  store i32 %543, i32* %104, align 4
  br label %548

; <label>:544:                                    ; preds = %525
  %545 = landingpad { i8*, i32 }
          cleanup
  %546 = extractvalue { i8*, i32 } %545, 0
  store i8* %546, i8** %103, align 8
  %547 = extractvalue { i8*, i32 } %545, 1
  store i32 %547, i32* %104, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %99) #10
  br label %548

; <label>:548:                                    ; preds = %544, %540
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %105) #10
  br label %549

; <label>:549:                                    ; preds = %548, %536
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %100) #10
  br label %550

; <label>:550:                                    ; preds = %549, %532
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %101) #10
  br label %551

; <label>:551:                                    ; preds = %550, %528
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %102) #10
  br label %552

; <label>:552:                                    ; preds = %551
  %553 = load i8*, i8** %103, align 8
  %554 = load i32, i32* %104, align 4
  %555 = insertvalue { i8*, i32 } undef, i8* %553, 0
  %556 = insertvalue { i8*, i32 } %555, i32 %554, 1
  resume { i8*, i32 } %556
}

; Function Attrs: nounwind readnone
declare double @llvm.pow.f64(double, double) #4

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic11unary_minusE7Variant(%struct.Variant* noalias sret, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %5 = alloca %"class.std::__1::basic_string"*, align 8
  %6 = alloca [3 x i64]*, align 8
  %7 = alloca i32, align 4
  %8 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %9 = alloca %"class.std::__1::basic_string"*, align 8
  %10 = alloca %"class.std::__1::basic_string"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca %"class.std::__1::basic_string"*, align 8
  %14 = alloca i8*, align 8
  %15 = alloca %"class.std::__1::basic_string"*, align 8
  %16 = alloca %"class.std::__1::basic_string", align 8
  %17 = alloca %"class.std::__1::basic_string", align 8
  %18 = alloca i8*
  %19 = alloca i32
  %20 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %21 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %20, i32 0, i32 0
  %22 = load i32, i32* %21, align 8
  switch i32 %22, label %43 [
    i32 0, label %23
    i32 1, label %28
    i32 2, label %33
    i32 3, label %38
  ]

; <label>:23:                                     ; preds = %2
  %24 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %25 = bitcast %union.anon* %24 to i32*
  %26 = load i32, i32* %25, align 8
  %27 = sub nsw i32 0, %26
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %27)
  br label %86

; <label>:28:                                     ; preds = %2
  %29 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %30 = bitcast %union.anon* %29 to i64*
  %31 = load i64, i64* %30, align 8
  %32 = sub nsw i64 0, %31
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %32)
  br label %86

; <label>:33:                                     ; preds = %2
  %34 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %35 = bitcast %union.anon* %34 to float*
  %36 = load float, float* %35, align 8
  %37 = fsub float -0.000000e+00, %36
  call void @_ZN7VariantC1Ef(%struct.Variant* %0, float %37)
  br label %86

; <label>:38:                                     ; preds = %2
  %39 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %40 = bitcast %union.anon* %39 to double*
  %41 = load double, double* %40, align 8
  %42 = fsub double -0.000000e+00, %41
  call void @_ZN7VariantC1Ed(%struct.Variant* %0, double %42)
  br label %86

; <label>:43:                                     ; preds = %2
  br label %44

; <label>:44:                                     ; preds = %43
  %45 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %46 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %45, i32 0, i32 0
  %47 = load i32, i32* %46, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %17, i32 %47)
  store i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.3, i32 0, i32 0), i8** %14, align 8, !noalias !59
  store %"class.std::__1::basic_string"* %17, %"class.std::__1::basic_string"** %15, align 8, !noalias !59
  %48 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %15, align 8, !noalias !59
  %49 = load i8*, i8** %14, align 8, !noalias !59
  %50 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %48, i64 0, i8* %49)
          to label %51 unwind label %87

; <label>:51:                                     ; preds = %44
  store %"class.std::__1::basic_string"* %50, %"class.std::__1::basic_string"** %13, align 8, !noalias !59
  %52 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !59
  store %"class.std::__1::basic_string"* %16, %"class.std::__1::basic_string"** %11, align 8, !noalias !59
  store %"class.std::__1::basic_string"* %52, %"class.std::__1::basic_string"** %12, align 8, !noalias !59
  %53 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !59
  %54 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !59
  store %"class.std::__1::basic_string"* %53, %"class.std::__1::basic_string"** %9, align 8, !noalias !59
  store %"class.std::__1::basic_string"* %54, %"class.std::__1::basic_string"** %10, align 8, !noalias !59
  %55 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %9, align 8, !noalias !59
  %56 = bitcast %"class.std::__1::basic_string"* %55 to %"class.std::__1::__basic_string_common"*
  %57 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %55, i32 0, i32 0
  %58 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !59
  %59 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %58, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %59, %"class.std::__1::__compressed_pair"** %8, align 8, !noalias !59
  %60 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %8, align 8, !noalias !59
  %61 = bitcast %"class.std::__1::__compressed_pair"* %57 to i8*
  %62 = bitcast %"class.std::__1::__compressed_pair"* %60 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %61, i8* %62, i64 24, i32 8, i1 false) #10
  %63 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !59
  store %"class.std::__1::basic_string"* %63, %"class.std::__1::basic_string"** %5, align 8, !noalias !59
  %64 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %5, align 8, !noalias !59
  %65 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %64, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %65, %"class.std::__1::__compressed_pair"** %4, align 8, !noalias !59
  %66 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %4, align 8, !noalias !59
  %67 = bitcast %"class.std::__1::__compressed_pair"* %66 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %67, %"class.std::__1::__libcpp_compressed_pair_imp"** %3, align 8, !noalias !59
  %68 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %3, align 8, !noalias !59
  %69 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %68, i32 0, i32 0
  %70 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %69, i32 0, i32 0
  %71 = bitcast %union.anon.0* %70 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %72 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %71, i32 0, i32 0
  store [3 x i64]* %72, [3 x i64]** %6, align 8, !noalias !59
  store i32 0, i32* %7, align 4, !noalias !59
  br label %73

; <label>:73:                                     ; preds = %76, %51
  %74 = load i32, i32* %7, align 4, !noalias !59
  %75 = icmp ult i32 %74, 3
  br i1 %75, label %76, label %83

; <label>:76:                                     ; preds = %73
  %77 = load i32, i32* %7, align 4, !noalias !59
  %78 = zext i32 %77 to i64
  %79 = load [3 x i64]*, [3 x i64]** %6, align 8, !noalias !59
  %80 = getelementptr inbounds [3 x i64], [3 x i64]* %79, i64 0, i64 %78
  store i64 0, i64* %80, align 8
  %81 = load i32, i32* %7, align 4, !noalias !59
  %82 = add i32 %81, 1
  store i32 %82, i32* %7, align 4, !noalias !59
  br label %73

; <label>:83:                                     ; preds = %73
  br label %84

; <label>:84:                                     ; preds = %83
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %16, %class.AstNode* null)
          to label %85 unwind label %91

; <label>:85:                                     ; preds = %84
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %16) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %17) #10
  br label %86

; <label>:86:                                     ; preds = %85, %38, %33, %28, %23
  ret void

; <label>:87:                                     ; preds = %44
  %88 = landingpad { i8*, i32 }
          cleanup
  %89 = extractvalue { i8*, i32 } %88, 0
  store i8* %89, i8** %18, align 8
  %90 = extractvalue { i8*, i32 } %88, 1
  store i32 %90, i32* %19, align 4
  br label %95

; <label>:91:                                     ; preds = %84
  %92 = landingpad { i8*, i32 }
          cleanup
  %93 = extractvalue { i8*, i32 } %92, 0
  store i8* %93, i8** %18, align 8
  %94 = extractvalue { i8*, i32 } %92, 1
  store i32 %94, i32* %19, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %16) #10
  br label %95

; <label>:95:                                     ; preds = %91, %87
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %17) #10
  br label %96

; <label>:96:                                     ; preds = %95
  %97 = load i8*, i8** %18, align 8
  %98 = load i32, i32* %19, align 4
  %99 = insertvalue { i8*, i32 } undef, i8* %97, 0
  %100 = insertvalue { i8*, i32 } %99, i32 %98, 1
  resume { i8*, i32 } %100
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic9unary_notE7Variant(%struct.Variant* noalias sret, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %5 = alloca %"class.std::__1::basic_string"*, align 8
  %6 = alloca [3 x i64]*, align 8
  %7 = alloca i32, align 4
  %8 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %9 = alloca %"class.std::__1::basic_string"*, align 8
  %10 = alloca %"class.std::__1::basic_string"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca %"class.std::__1::basic_string"*, align 8
  %14 = alloca i8*, align 8
  %15 = alloca %"class.std::__1::basic_string"*, align 8
  %16 = alloca %"class.std::__1::basic_string", align 8
  %17 = alloca %"class.std::__1::basic_string", align 8
  %18 = alloca i8*
  %19 = alloca i32
  %20 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %21 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %20, i32 0, i32 0
  %22 = load i32, i32* %21, align 8
  switch i32 %22, label %53 [
    i32 0, label %23
    i32 1, label %29
    i32 2, label %35
    i32 3, label %41
    i32 5, label %47
  ]

; <label>:23:                                     ; preds = %2
  %24 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %25 = bitcast %union.anon* %24 to i32*
  %26 = load i32, i32* %25, align 8
  %27 = icmp ne i32 %26, 0
  %28 = xor i1 %27, true
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %28)
  br label %96

; <label>:29:                                     ; preds = %2
  %30 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %31 = bitcast %union.anon* %30 to i64*
  %32 = load i64, i64* %31, align 8
  %33 = icmp ne i64 %32, 0
  %34 = xor i1 %33, true
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %34)
  br label %96

; <label>:35:                                     ; preds = %2
  %36 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %37 = bitcast %union.anon* %36 to float*
  %38 = load float, float* %37, align 8
  %39 = fcmp une float %38, 0.000000e+00
  %40 = xor i1 %39, true
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %40)
  br label %96

; <label>:41:                                     ; preds = %2
  %42 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %43 = bitcast %union.anon* %42 to double*
  %44 = load double, double* %43, align 8
  %45 = fcmp une double %44, 0.000000e+00
  %46 = xor i1 %45, true
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %46)
  br label %96

; <label>:47:                                     ; preds = %2
  %48 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %49 = bitcast %union.anon* %48 to i8*
  %50 = load i8, i8* %49, align 8
  %51 = trunc i8 %50 to i1
  %52 = xor i1 %51, true
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %52)
  br label %96

; <label>:53:                                     ; preds = %2
  br label %54

; <label>:54:                                     ; preds = %53
  %55 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %56 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %55, i32 0, i32 0
  %57 = load i32, i32* %56, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %17, i32 %57)
  store i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.3, i32 0, i32 0), i8** %14, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %17, %"class.std::__1::basic_string"** %15, align 8, !noalias !62
  %58 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %15, align 8, !noalias !62
  %59 = load i8*, i8** %14, align 8, !noalias !62
  %60 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %58, i64 0, i8* %59)
          to label %61 unwind label %97

; <label>:61:                                     ; preds = %54
  store %"class.std::__1::basic_string"* %60, %"class.std::__1::basic_string"** %13, align 8, !noalias !62
  %62 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %16, %"class.std::__1::basic_string"** %11, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %62, %"class.std::__1::basic_string"** %12, align 8, !noalias !62
  %63 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !62
  %64 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %63, %"class.std::__1::basic_string"** %9, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %64, %"class.std::__1::basic_string"** %10, align 8, !noalias !62
  %65 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %9, align 8, !noalias !62
  %66 = bitcast %"class.std::__1::basic_string"* %65 to %"class.std::__1::__basic_string_common"*
  %67 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %65, i32 0, i32 0
  %68 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !62
  %69 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %68, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %69, %"class.std::__1::__compressed_pair"** %8, align 8, !noalias !62
  %70 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %8, align 8, !noalias !62
  %71 = bitcast %"class.std::__1::__compressed_pair"* %67 to i8*
  %72 = bitcast %"class.std::__1::__compressed_pair"* %70 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %71, i8* %72, i64 24, i32 8, i1 false) #10
  %73 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !62
  store %"class.std::__1::basic_string"* %73, %"class.std::__1::basic_string"** %5, align 8, !noalias !62
  %74 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %5, align 8, !noalias !62
  %75 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %74, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %75, %"class.std::__1::__compressed_pair"** %4, align 8, !noalias !62
  %76 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %4, align 8, !noalias !62
  %77 = bitcast %"class.std::__1::__compressed_pair"* %76 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %77, %"class.std::__1::__libcpp_compressed_pair_imp"** %3, align 8, !noalias !62
  %78 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %3, align 8, !noalias !62
  %79 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %78, i32 0, i32 0
  %80 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %79, i32 0, i32 0
  %81 = bitcast %union.anon.0* %80 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %82 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %81, i32 0, i32 0
  store [3 x i64]* %82, [3 x i64]** %6, align 8, !noalias !62
  store i32 0, i32* %7, align 4, !noalias !62
  br label %83

; <label>:83:                                     ; preds = %86, %61
  %84 = load i32, i32* %7, align 4, !noalias !62
  %85 = icmp ult i32 %84, 3
  br i1 %85, label %86, label %93

; <label>:86:                                     ; preds = %83
  %87 = load i32, i32* %7, align 4, !noalias !62
  %88 = zext i32 %87 to i64
  %89 = load [3 x i64]*, [3 x i64]** %6, align 8, !noalias !62
  %90 = getelementptr inbounds [3 x i64], [3 x i64]* %89, i64 0, i64 %88
  store i64 0, i64* %90, align 8
  %91 = load i32, i32* %7, align 4, !noalias !62
  %92 = add i32 %91, 1
  store i32 %92, i32* %7, align 4, !noalias !62
  br label %83

; <label>:93:                                     ; preds = %83
  br label %94

; <label>:94:                                     ; preds = %93
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %16, %class.AstNode* null)
          to label %95 unwind label %101

; <label>:95:                                     ; preds = %94
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %16) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %17) #10
  br label %96

; <label>:96:                                     ; preds = %95, %47, %41, %35, %29, %23
  ret void

; <label>:97:                                     ; preds = %54
  %98 = landingpad { i8*, i32 }
          cleanup
  %99 = extractvalue { i8*, i32 } %98, 0
  store i8* %99, i8** %18, align 8
  %100 = extractvalue { i8*, i32 } %98, 1
  store i32 %100, i32* %19, align 4
  br label %105

; <label>:101:                                    ; preds = %94
  %102 = landingpad { i8*, i32 }
          cleanup
  %103 = extractvalue { i8*, i32 } %102, 0
  store i8* %103, i8** %18, align 8
  %104 = extractvalue { i8*, i32 } %102, 1
  store i32 %104, i32* %19, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %16) #10
  br label %105

; <label>:105:                                    ; preds = %101, %97
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %17) #10
  br label %106

; <label>:106:                                    ; preds = %105
  %107 = load i8*, i8** %18, align 8
  %108 = load i32, i32* %19, align 4
  %109 = insertvalue { i8*, i32 } undef, i8* %107, 0
  %110 = insertvalue { i8*, i32 } %109, i32 %108, 1
  resume { i8*, i32 } %110
}

declare void @_ZN7VariantC1Eb(%struct.Variant*, i1 zeroext) unnamed_addr #1

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic14bitwise_negateE7Variant(%struct.Variant* noalias sret, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %4 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %5 = alloca %"class.std::__1::basic_string"*, align 8
  %6 = alloca [3 x i64]*, align 8
  %7 = alloca i32, align 4
  %8 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %9 = alloca %"class.std::__1::basic_string"*, align 8
  %10 = alloca %"class.std::__1::basic_string"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::basic_string"*, align 8
  %13 = alloca %"class.std::__1::basic_string"*, align 8
  %14 = alloca i8*, align 8
  %15 = alloca %"class.std::__1::basic_string"*, align 8
  %16 = alloca %"class.std::__1::basic_string", align 8
  %17 = alloca %"class.std::__1::basic_string", align 8
  %18 = alloca i8*
  %19 = alloca i32
  %20 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %21 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %20, i32 0, i32 0
  %22 = load i32, i32* %21, align 8
  switch i32 %22, label %33 [
    i32 0, label %23
    i32 1, label %28
  ]

; <label>:23:                                     ; preds = %2
  %24 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %25 = bitcast %union.anon* %24 to i32*
  %26 = load i32, i32* %25, align 8
  %27 = xor i32 %26, -1
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %27)
  br label %76

; <label>:28:                                     ; preds = %2
  %29 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %30 = bitcast %union.anon* %29 to i64*
  %31 = load i64, i64* %30, align 8
  %32 = xor i64 %31, -1
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %32)
  br label %76

; <label>:33:                                     ; preds = %2
  br label %34

; <label>:34:                                     ; preds = %33
  %35 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %36 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %35, i32 0, i32 0
  %37 = load i32, i32* %36, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %17, i32 %37)
  store i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str.4, i32 0, i32 0), i8** %14, align 8, !noalias !65
  store %"class.std::__1::basic_string"* %17, %"class.std::__1::basic_string"** %15, align 8, !noalias !65
  %38 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %15, align 8, !noalias !65
  %39 = load i8*, i8** %14, align 8, !noalias !65
  %40 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %38, i64 0, i8* %39)
          to label %41 unwind label %77

; <label>:41:                                     ; preds = %34
  store %"class.std::__1::basic_string"* %40, %"class.std::__1::basic_string"** %13, align 8, !noalias !65
  %42 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !65
  store %"class.std::__1::basic_string"* %16, %"class.std::__1::basic_string"** %11, align 8, !noalias !65
  store %"class.std::__1::basic_string"* %42, %"class.std::__1::basic_string"** %12, align 8, !noalias !65
  %43 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !65
  %44 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !65
  store %"class.std::__1::basic_string"* %43, %"class.std::__1::basic_string"** %9, align 8, !noalias !65
  store %"class.std::__1::basic_string"* %44, %"class.std::__1::basic_string"** %10, align 8, !noalias !65
  %45 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %9, align 8, !noalias !65
  %46 = bitcast %"class.std::__1::basic_string"* %45 to %"class.std::__1::__basic_string_common"*
  %47 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %45, i32 0, i32 0
  %48 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !65
  %49 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %48, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %49, %"class.std::__1::__compressed_pair"** %8, align 8, !noalias !65
  %50 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %8, align 8, !noalias !65
  %51 = bitcast %"class.std::__1::__compressed_pair"* %47 to i8*
  %52 = bitcast %"class.std::__1::__compressed_pair"* %50 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %51, i8* %52, i64 24, i32 8, i1 false) #10
  %53 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !65
  store %"class.std::__1::basic_string"* %53, %"class.std::__1::basic_string"** %5, align 8, !noalias !65
  %54 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %5, align 8, !noalias !65
  %55 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %54, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %55, %"class.std::__1::__compressed_pair"** %4, align 8, !noalias !65
  %56 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %4, align 8, !noalias !65
  %57 = bitcast %"class.std::__1::__compressed_pair"* %56 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %57, %"class.std::__1::__libcpp_compressed_pair_imp"** %3, align 8, !noalias !65
  %58 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %3, align 8, !noalias !65
  %59 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %58, i32 0, i32 0
  %60 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %59, i32 0, i32 0
  %61 = bitcast %union.anon.0* %60 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %62 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %61, i32 0, i32 0
  store [3 x i64]* %62, [3 x i64]** %6, align 8, !noalias !65
  store i32 0, i32* %7, align 4, !noalias !65
  br label %63

; <label>:63:                                     ; preds = %66, %41
  %64 = load i32, i32* %7, align 4, !noalias !65
  %65 = icmp ult i32 %64, 3
  br i1 %65, label %66, label %73

; <label>:66:                                     ; preds = %63
  %67 = load i32, i32* %7, align 4, !noalias !65
  %68 = zext i32 %67 to i64
  %69 = load [3 x i64]*, [3 x i64]** %6, align 8, !noalias !65
  %70 = getelementptr inbounds [3 x i64], [3 x i64]* %69, i64 0, i64 %68
  store i64 0, i64* %70, align 8
  %71 = load i32, i32* %7, align 4, !noalias !65
  %72 = add i32 %71, 1
  store i32 %72, i32* %7, align 4, !noalias !65
  br label %63

; <label>:73:                                     ; preds = %63
  br label %74

; <label>:74:                                     ; preds = %73
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %16, %class.AstNode* null)
          to label %75 unwind label %81

; <label>:75:                                     ; preds = %74
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %16) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %17) #10
  br label %76

; <label>:76:                                     ; preds = %75, %28, %23
  ret void

; <label>:77:                                     ; preds = %34
  %78 = landingpad { i8*, i32 }
          cleanup
  %79 = extractvalue { i8*, i32 } %78, 0
  store i8* %79, i8** %18, align 8
  %80 = extractvalue { i8*, i32 } %78, 1
  store i32 %80, i32* %19, align 4
  br label %85

; <label>:81:                                     ; preds = %74
  %82 = landingpad { i8*, i32 }
          cleanup
  %83 = extractvalue { i8*, i32 } %82, 0
  store i8* %83, i8** %18, align 8
  %84 = extractvalue { i8*, i32 } %82, 1
  store i32 %84, i32* %19, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %16) #10
  br label %85

; <label>:85:                                     ; preds = %81, %77
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %17) #10
  br label %86

; <label>:86:                                     ; preds = %85
  %87 = load i8*, i8** %18, align 8
  %88 = load i32, i32* %19, align 4
  %89 = insertvalue { i8*, i32 } undef, i8* %87, 0
  %90 = insertvalue { i8*, i32 } %89, i32 %88, 1
  resume { i8*, i32 } %90
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic5equalE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  %69 = alloca i8*, align 8
  %70 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %71 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %72 = alloca %"class.std::__1::basic_string"*, align 8
  %73 = alloca i8*, align 8
  %74 = alloca i8*, align 8
  %75 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %76 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %77 = alloca %"class.std::__1::basic_string"*, align 8
  %78 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %79 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %80 = alloca %"class.std::__1::basic_string"*, align 8
  %81 = alloca %"class.std::__1::basic_string"*, align 8
  %82 = alloca %"class.std::__1::basic_string"*, align 8
  %83 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %84 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %85 = alloca %"class.std::__1::basic_string"*, align 8
  %86 = alloca i8*, align 8
  %87 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %88 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %89 = alloca %"class.std::__1::basic_string"*, align 8
  %90 = alloca i8*, align 8
  %91 = alloca i8*, align 8
  %92 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %93 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %94 = alloca %"class.std::__1::basic_string"*, align 8
  %95 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %96 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %97 = alloca %"class.std::__1::basic_string"*, align 8
  %98 = alloca %"class.std::__1::basic_string"*, align 8
  %99 = alloca %"class.std::__1::basic_string"*, align 8
  %100 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %101 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %102 = alloca %"class.std::__1::basic_string"*, align 8
  %103 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %104 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %105 = alloca %"class.std::__1::basic_string"*, align 8
  %106 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %107 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %108 = alloca %"class.std::__1::basic_string"*, align 8
  %109 = alloca %"class.std::__1::basic_string"*, align 8
  %110 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %111 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %112 = alloca %"class.std::__1::basic_string"*, align 8
  %113 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %114 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %115 = alloca %"class.std::__1::basic_string"*, align 8
  %116 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %117 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %118 = alloca %"class.std::__1::basic_string"*, align 8
  %119 = alloca %"class.std::__1::basic_string"*, align 8
  %120 = alloca i1, align 1
  %121 = alloca %"class.std::__1::basic_string"*, align 8
  %122 = alloca %"class.std::__1::basic_string"*, align 8
  %123 = alloca i64, align 8
  %124 = alloca i8*, align 8
  %125 = alloca i8*, align 8
  %126 = alloca %"class.std::__1::basic_string", align 8
  %127 = alloca %"class.std::__1::basic_string", align 8
  %128 = alloca %"class.std::__1::basic_string", align 8
  %129 = alloca %"class.std::__1::basic_string", align 8
  %130 = alloca i8*
  %131 = alloca i32
  %132 = alloca %"class.std::__1::basic_string", align 8
  %133 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %134 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %133, i32 0, i32 0
  %135 = load i32, i32* %134, align 8
  %136 = icmp eq i32 %135, 3
  br i1 %136, label %137, label %151

; <label>:137:                                    ; preds = %3
  %138 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %139 = bitcast %union.anon* %138 to double*
  %140 = load double, double* %139, align 8
  %141 = fcmp olt double %140, 0x41DFFFFFFFC00000
  br i1 %141, label %142, label %151

; <label>:142:                                    ; preds = %137
  %143 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %144 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %143, i32 0, i32 0
  store i32 0, i32* %144, align 8
  %145 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %146 = bitcast %union.anon* %145 to double*
  %147 = load double, double* %146, align 8
  %148 = fptosi double %147 to i32
  %149 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %150 = bitcast %union.anon* %149 to i32*
  store i32 %148, i32* %150, align 8
  br label %151

; <label>:151:                                    ; preds = %142, %137, %3
  %152 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %153 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %152, i32 0, i32 0
  %154 = load i32, i32* %153, align 8
  %155 = icmp eq i32 %154, 3
  br i1 %155, label %156, label %170

; <label>:156:                                    ; preds = %151
  %157 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %158 = bitcast %union.anon* %157 to double*
  %159 = load double, double* %158, align 8
  %160 = fcmp olt double %159, 0x41DFFFFFFFC00000
  br i1 %160, label %161, label %170

; <label>:161:                                    ; preds = %156
  %162 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %163 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %162, i32 0, i32 0
  store i32 0, i32* %163, align 8
  %164 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %165 = bitcast %union.anon* %164 to double*
  %166 = load double, double* %165, align 8
  %167 = fptosi double %166 to i32
  %168 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %169 = bitcast %union.anon* %168 to i32*
  store i32 %167, i32* %169, align 8
  br label %170

; <label>:170:                                    ; preds = %161, %156, %151
  %171 = call zeroext i1 @_ZNK7Variant7is_nullEv(%struct.Variant* %1)
  br i1 %171, label %172, label %175

; <label>:172:                                    ; preds = %170
  %173 = call zeroext i1 @_ZNK7Variant7is_nullEv(%struct.Variant* %2)
  br i1 %173, label %174, label %175

; <label>:174:                                    ; preds = %172
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext true)
  br label %683

; <label>:175:                                    ; preds = %172, %170
  %176 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %177 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %176, i32 0, i32 0
  %178 = load i32, i32* %177, align 8
  %179 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %180 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %179, i32 0, i32 0
  %181 = load i32, i32* %180, align 8
  %182 = icmp ne i32 %178, %181
  br i1 %182, label %183, label %184

; <label>:183:                                    ; preds = %175
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext false)
  br label %683

; <label>:184:                                    ; preds = %175
  %185 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %186 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %185, i32 0, i32 0
  %187 = load i32, i32* %186, align 8
  switch i32 %187, label %473 [
    i32 0, label %188
    i32 1, label %196
    i32 2, label %204
    i32 3, label %212
    i32 4, label %220
    i32 6, label %451
    i32 5, label %461
  ]

; <label>:188:                                    ; preds = %184
  %189 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %190 = bitcast %union.anon* %189 to i32*
  %191 = load i32, i32* %190, align 8
  %192 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %193 = bitcast %union.anon* %192 to i32*
  %194 = load i32, i32* %193, align 8
  %195 = icmp eq i32 %191, %194
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %195)
  br label %683

; <label>:196:                                    ; preds = %184
  %197 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %198 = bitcast %union.anon* %197 to i64*
  %199 = load i64, i64* %198, align 8
  %200 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %201 = bitcast %union.anon* %200 to i64*
  %202 = load i64, i64* %201, align 8
  %203 = icmp eq i64 %199, %202
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %203)
  br label %683

; <label>:204:                                    ; preds = %184
  %205 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %206 = bitcast %union.anon* %205 to float*
  %207 = load float, float* %206, align 8
  %208 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %209 = bitcast %union.anon* %208 to float*
  %210 = load float, float* %209, align 8
  %211 = fcmp oeq float %207, %210
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %211)
  br label %683

; <label>:212:                                    ; preds = %184
  %213 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %214 = bitcast %union.anon* %213 to double*
  %215 = load double, double* %214, align 8
  %216 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %217 = bitcast %union.anon* %216 to double*
  %218 = load double, double* %217, align 8
  %219 = fcmp oeq double %215, %218
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %219)
  br label %683

; <label>:220:                                    ; preds = %184
  %221 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 3
  %222 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 3
  store %"class.std::__1::basic_string"* %221, %"class.std::__1::basic_string"** %121, align 8
  store %"class.std::__1::basic_string"* %222, %"class.std::__1::basic_string"** %122, align 8
  %223 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %121, align 8
  store %"class.std::__1::basic_string"* %223, %"class.std::__1::basic_string"** %119, align 8
  %224 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %119, align 8
  store %"class.std::__1::basic_string"* %224, %"class.std::__1::basic_string"** %118, align 8
  %225 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %118, align 8
  %226 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %225, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %226, %"class.std::__1::__compressed_pair"** %117, align 8
  %227 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %117, align 8
  %228 = bitcast %"class.std::__1::__compressed_pair"* %227 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %228, %"class.std::__1::__libcpp_compressed_pair_imp"** %116, align 8
  %229 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %116, align 8
  %230 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %229, i32 0, i32 0
  %231 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %230, i32 0, i32 0
  %232 = bitcast %union.anon.0* %231 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %233 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %232, i32 0, i32 0
  %234 = bitcast %union.anon.1* %233 to i8*
  %235 = load i8, i8* %234, align 8
  %236 = zext i8 %235 to i32
  %237 = and i32 %236, 1
  %238 = icmp ne i32 %237, 0
  br i1 %238, label %239, label %250

; <label>:239:                                    ; preds = %220
  store %"class.std::__1::basic_string"* %224, %"class.std::__1::basic_string"** %112, align 8
  %240 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %112, align 8
  %241 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %240, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %241, %"class.std::__1::__compressed_pair"** %111, align 8
  %242 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %111, align 8
  %243 = bitcast %"class.std::__1::__compressed_pair"* %242 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %243, %"class.std::__1::__libcpp_compressed_pair_imp"** %110, align 8
  %244 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %110, align 8
  %245 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %244, i32 0, i32 0
  %246 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %245, i32 0, i32 0
  %247 = bitcast %union.anon.0* %246 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %248 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %247, i32 0, i32 1
  %249 = load i64, i64* %248, align 8
  br label %265

; <label>:250:                                    ; preds = %220
  store %"class.std::__1::basic_string"* %224, %"class.std::__1::basic_string"** %115, align 8
  %251 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %115, align 8
  %252 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %251, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %252, %"class.std::__1::__compressed_pair"** %114, align 8
  %253 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %114, align 8
  %254 = bitcast %"class.std::__1::__compressed_pair"* %253 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %254, %"class.std::__1::__libcpp_compressed_pair_imp"** %113, align 8
  %255 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %113, align 8
  %256 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %255, i32 0, i32 0
  %257 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %256, i32 0, i32 0
  %258 = bitcast %union.anon.0* %257 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %259 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %258, i32 0, i32 0
  %260 = bitcast %union.anon.1* %259 to i8*
  %261 = load i8, i8* %260, align 8
  %262 = zext i8 %261 to i32
  %263 = ashr i32 %262, 1
  %264 = sext i32 %263 to i64
  br label %265

; <label>:265:                                    ; preds = %250, %239
  %266 = phi i64 [ %249, %239 ], [ %264, %250 ]
  store i64 %266, i64* %123, align 8
  %267 = load i64, i64* %123, align 8
  %268 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %122, align 8
  store %"class.std::__1::basic_string"* %268, %"class.std::__1::basic_string"** %109, align 8
  %269 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %109, align 8
  store %"class.std::__1::basic_string"* %269, %"class.std::__1::basic_string"** %108, align 8
  %270 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %108, align 8
  %271 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %270, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %271, %"class.std::__1::__compressed_pair"** %107, align 8
  %272 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %107, align 8
  %273 = bitcast %"class.std::__1::__compressed_pair"* %272 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %273, %"class.std::__1::__libcpp_compressed_pair_imp"** %106, align 8
  %274 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %106, align 8
  %275 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %274, i32 0, i32 0
  %276 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %275, i32 0, i32 0
  %277 = bitcast %union.anon.0* %276 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %278 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %277, i32 0, i32 0
  %279 = bitcast %union.anon.1* %278 to i8*
  %280 = load i8, i8* %279, align 8
  %281 = zext i8 %280 to i32
  %282 = and i32 %281, 1
  %283 = icmp ne i32 %282, 0
  br i1 %283, label %284, label %295

; <label>:284:                                    ; preds = %265
  store %"class.std::__1::basic_string"* %269, %"class.std::__1::basic_string"** %102, align 8
  %285 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %102, align 8
  %286 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %285, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %286, %"class.std::__1::__compressed_pair"** %101, align 8
  %287 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %101, align 8
  %288 = bitcast %"class.std::__1::__compressed_pair"* %287 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %288, %"class.std::__1::__libcpp_compressed_pair_imp"** %100, align 8
  %289 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %100, align 8
  %290 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %289, i32 0, i32 0
  %291 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %290, i32 0, i32 0
  %292 = bitcast %union.anon.0* %291 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %293 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %292, i32 0, i32 1
  %294 = load i64, i64* %293, align 8
  br label %310

; <label>:295:                                    ; preds = %265
  store %"class.std::__1::basic_string"* %269, %"class.std::__1::basic_string"** %105, align 8
  %296 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %105, align 8
  %297 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %296, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %297, %"class.std::__1::__compressed_pair"** %104, align 8
  %298 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %104, align 8
  %299 = bitcast %"class.std::__1::__compressed_pair"* %298 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %299, %"class.std::__1::__libcpp_compressed_pair_imp"** %103, align 8
  %300 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %103, align 8
  %301 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %300, i32 0, i32 0
  %302 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %301, i32 0, i32 0
  %303 = bitcast %union.anon.0* %302 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %304 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %303, i32 0, i32 0
  %305 = bitcast %union.anon.1* %304 to i8*
  %306 = load i8, i8* %305, align 8
  %307 = zext i8 %306 to i32
  %308 = ashr i32 %307, 1
  %309 = sext i32 %308 to i64
  br label %310

; <label>:310:                                    ; preds = %295, %284
  %311 = phi i64 [ %294, %284 ], [ %309, %295 ]
  %312 = icmp ne i64 %267, %311
  br i1 %312, label %313, label %314

; <label>:313:                                    ; preds = %310
  store i1 false, i1* %120, align 1
  br label %449

; <label>:314:                                    ; preds = %310
  %315 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %121, align 8
  store %"class.std::__1::basic_string"* %315, %"class.std::__1::basic_string"** %99, align 8
  %316 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %99, align 8
  store %"class.std::__1::basic_string"* %316, %"class.std::__1::basic_string"** %98, align 8
  %317 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %98, align 8
  store %"class.std::__1::basic_string"* %317, %"class.std::__1::basic_string"** %97, align 8
  %318 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %97, align 8
  %319 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %318, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %319, %"class.std::__1::__compressed_pair"** %96, align 8
  %320 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %96, align 8
  %321 = bitcast %"class.std::__1::__compressed_pair"* %320 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %321, %"class.std::__1::__libcpp_compressed_pair_imp"** %95, align 8
  %322 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %95, align 8
  %323 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %322, i32 0, i32 0
  %324 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %323, i32 0, i32 0
  %325 = bitcast %union.anon.0* %324 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %326 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %325, i32 0, i32 0
  %327 = bitcast %union.anon.1* %326 to i8*
  %328 = load i8, i8* %327, align 8
  %329 = zext i8 %328 to i32
  %330 = and i32 %329, 1
  %331 = icmp ne i32 %330, 0
  br i1 %331, label %332, label %343

; <label>:332:                                    ; preds = %314
  store %"class.std::__1::basic_string"* %317, %"class.std::__1::basic_string"** %89, align 8
  %333 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %89, align 8
  %334 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %333, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %334, %"class.std::__1::__compressed_pair"** %88, align 8
  %335 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %88, align 8
  %336 = bitcast %"class.std::__1::__compressed_pair"* %335 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %336, %"class.std::__1::__libcpp_compressed_pair_imp"** %87, align 8
  %337 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %87, align 8
  %338 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %337, i32 0, i32 0
  %339 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %338, i32 0, i32 0
  %340 = bitcast %union.anon.0* %339 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %341 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %340, i32 0, i32 2
  %342 = load i8*, i8** %341, align 8
  br label %356

; <label>:343:                                    ; preds = %314
  store %"class.std::__1::basic_string"* %317, %"class.std::__1::basic_string"** %94, align 8
  %344 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %94, align 8
  %345 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %344, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %345, %"class.std::__1::__compressed_pair"** %93, align 8
  %346 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %93, align 8
  %347 = bitcast %"class.std::__1::__compressed_pair"* %346 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %347, %"class.std::__1::__libcpp_compressed_pair_imp"** %92, align 8
  %348 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %92, align 8
  %349 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %348, i32 0, i32 0
  %350 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %349, i32 0, i32 0
  %351 = bitcast %union.anon.0* %350 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %352 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %351, i32 0, i32 1
  %353 = getelementptr inbounds [23 x i8], [23 x i8]* %352, i64 0, i64 0
  store i8* %353, i8** %91, align 8
  %354 = load i8*, i8** %91, align 8
  store i8* %354, i8** %90, align 8
  %355 = load i8*, i8** %90, align 8
  br label %356

; <label>:356:                                    ; preds = %343, %332
  %357 = phi i8* [ %342, %332 ], [ %355, %343 ]
  store i8* %357, i8** %86, align 8
  %358 = load i8*, i8** %86, align 8
  store i8* %358, i8** %124, align 8
  %359 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %122, align 8
  store %"class.std::__1::basic_string"* %359, %"class.std::__1::basic_string"** %82, align 8
  %360 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %82, align 8
  store %"class.std::__1::basic_string"* %360, %"class.std::__1::basic_string"** %81, align 8
  %361 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %81, align 8
  store %"class.std::__1::basic_string"* %361, %"class.std::__1::basic_string"** %80, align 8
  %362 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %80, align 8
  %363 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %362, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %363, %"class.std::__1::__compressed_pair"** %79, align 8
  %364 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %79, align 8
  %365 = bitcast %"class.std::__1::__compressed_pair"* %364 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %365, %"class.std::__1::__libcpp_compressed_pair_imp"** %78, align 8
  %366 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %78, align 8
  %367 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %366, i32 0, i32 0
  %368 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %367, i32 0, i32 0
  %369 = bitcast %union.anon.0* %368 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %370 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %369, i32 0, i32 0
  %371 = bitcast %union.anon.1* %370 to i8*
  %372 = load i8, i8* %371, align 8
  %373 = zext i8 %372 to i32
  %374 = and i32 %373, 1
  %375 = icmp ne i32 %374, 0
  br i1 %375, label %376, label %387

; <label>:376:                                    ; preds = %356
  store %"class.std::__1::basic_string"* %361, %"class.std::__1::basic_string"** %72, align 8
  %377 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %72, align 8
  %378 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %377, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %378, %"class.std::__1::__compressed_pair"** %71, align 8
  %379 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %71, align 8
  %380 = bitcast %"class.std::__1::__compressed_pair"* %379 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %380, %"class.std::__1::__libcpp_compressed_pair_imp"** %70, align 8
  %381 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %70, align 8
  %382 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %381, i32 0, i32 0
  %383 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %382, i32 0, i32 0
  %384 = bitcast %union.anon.0* %383 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %385 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %384, i32 0, i32 2
  %386 = load i8*, i8** %385, align 8
  br label %400

; <label>:387:                                    ; preds = %356
  store %"class.std::__1::basic_string"* %361, %"class.std::__1::basic_string"** %77, align 8
  %388 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %77, align 8
  %389 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %388, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %389, %"class.std::__1::__compressed_pair"** %76, align 8
  %390 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %76, align 8
  %391 = bitcast %"class.std::__1::__compressed_pair"* %390 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %391, %"class.std::__1::__libcpp_compressed_pair_imp"** %75, align 8
  %392 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %75, align 8
  %393 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %392, i32 0, i32 0
  %394 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %393, i32 0, i32 0
  %395 = bitcast %union.anon.0* %394 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %396 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %395, i32 0, i32 1
  %397 = getelementptr inbounds [23 x i8], [23 x i8]* %396, i64 0, i64 0
  store i8* %397, i8** %74, align 8
  %398 = load i8*, i8** %74, align 8
  store i8* %398, i8** %73, align 8
  %399 = load i8*, i8** %73, align 8
  br label %400

; <label>:400:                                    ; preds = %387, %376
  %401 = phi i8* [ %386, %376 ], [ %399, %387 ]
  store i8* %401, i8** %69, align 8
  %402 = load i8*, i8** %69, align 8
  store i8* %402, i8** %125, align 8
  %403 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %121, align 8
  store %"class.std::__1::basic_string"* %403, %"class.std::__1::basic_string"** %85, align 8
  %404 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %85, align 8
  %405 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %404, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %405, %"class.std::__1::__compressed_pair"** %84, align 8
  %406 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %84, align 8
  %407 = bitcast %"class.std::__1::__compressed_pair"* %406 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %407, %"class.std::__1::__libcpp_compressed_pair_imp"** %83, align 8
  %408 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %83, align 8
  %409 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %408, i32 0, i32 0
  %410 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %409, i32 0, i32 0
  %411 = bitcast %union.anon.0* %410 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %412 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %411, i32 0, i32 0
  %413 = bitcast %union.anon.1* %412 to i8*
  %414 = load i8, i8* %413, align 8
  %415 = zext i8 %414 to i32
  %416 = and i32 %415, 1
  %417 = icmp ne i32 %416, 0
  br i1 %417, label %418, label %425

; <label>:418:                                    ; preds = %400
  %419 = load i8*, i8** %124, align 8
  %420 = load i8*, i8** %125, align 8
  %421 = load i64, i64* %123, align 8
  %422 = invoke i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8* %419, i8* %420, i64 %421)
          to label %423 unwind label %446

; <label>:423:                                    ; preds = %418
  %424 = icmp eq i32 %422, 0
  store i1 %424, i1* %120, align 1
  br label %449

; <label>:425:                                    ; preds = %400
  br label %426

; <label>:426:                                    ; preds = %438, %425
  %427 = load i64, i64* %123, align 8
  %428 = icmp ne i64 %427, 0
  br i1 %428, label %429, label %445

; <label>:429:                                    ; preds = %426
  %430 = load i8*, i8** %124, align 8
  %431 = load i8, i8* %430, align 1
  %432 = sext i8 %431 to i32
  %433 = load i8*, i8** %125, align 8
  %434 = load i8, i8* %433, align 1
  %435 = sext i8 %434 to i32
  %436 = icmp ne i32 %432, %435
  br i1 %436, label %437, label %438

; <label>:437:                                    ; preds = %429
  store i1 false, i1* %120, align 1
  br label %449

; <label>:438:                                    ; preds = %429
  %439 = load i64, i64* %123, align 8
  %440 = add i64 %439, -1
  store i64 %440, i64* %123, align 8
  %441 = load i8*, i8** %124, align 8
  %442 = getelementptr inbounds i8, i8* %441, i32 1
  store i8* %442, i8** %124, align 8
  %443 = load i8*, i8** %125, align 8
  %444 = getelementptr inbounds i8, i8* %443, i32 1
  store i8* %444, i8** %125, align 8
  br label %426

; <label>:445:                                    ; preds = %426
  store i1 true, i1* %120, align 1
  br label %449

; <label>:446:                                    ; preds = %418
  %447 = landingpad { i8*, i32 }
          catch i8* null
  %448 = extractvalue { i8*, i32 } %447, 0
  call void @__clang_call_terminate(i8* %448) #11
  unreachable

; <label>:449:                                    ; preds = %313, %423, %437, %445
  %450 = load i1, i1* %120, align 1
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %450)
  br label %683

; <label>:451:                                    ; preds = %184
  %452 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %453 = bitcast %union.anon* %452 to i8*
  %454 = load i8, i8* %453, align 8
  %455 = sext i8 %454 to i32
  %456 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %457 = bitcast %union.anon* %456 to i8*
  %458 = load i8, i8* %457, align 8
  %459 = sext i8 %458 to i32
  %460 = icmp eq i32 %455, %459
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %460)
  br label %683

; <label>:461:                                    ; preds = %184
  %462 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %463 = bitcast %union.anon* %462 to i8*
  %464 = load i8, i8* %463, align 8
  %465 = trunc i8 %464 to i1
  %466 = zext i1 %465 to i32
  %467 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %468 = bitcast %union.anon* %467 to i8*
  %469 = load i8, i8* %468, align 8
  %470 = trunc i8 %469 to i1
  %471 = zext i1 %470 to i32
  %472 = icmp eq i32 %466, %471
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %472)
  br label %683

; <label>:473:                                    ; preds = %184
  br label %474

; <label>:474:                                    ; preds = %473
  %475 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %476 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %475, i32 0, i32 0
  %477 = load i32, i32* %476, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %129, i32 %477)
  store i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.5, i32 0, i32 0), i8** %67, align 8, !noalias !68
  store %"class.std::__1::basic_string"* %129, %"class.std::__1::basic_string"** %68, align 8, !noalias !68
  %478 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !68
  %479 = load i8*, i8** %67, align 8, !noalias !68
  %480 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %478, i64 0, i8* %479)
          to label %481 unwind label %684

; <label>:481:                                    ; preds = %474
  store %"class.std::__1::basic_string"* %480, %"class.std::__1::basic_string"** %66, align 8, !noalias !68
  %482 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !68
  store %"class.std::__1::basic_string"* %128, %"class.std::__1::basic_string"** %64, align 8, !noalias !68
  store %"class.std::__1::basic_string"* %482, %"class.std::__1::basic_string"** %65, align 8, !noalias !68
  %483 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !68
  %484 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !68
  store %"class.std::__1::basic_string"* %483, %"class.std::__1::basic_string"** %62, align 8, !noalias !68
  store %"class.std::__1::basic_string"* %484, %"class.std::__1::basic_string"** %63, align 8, !noalias !68
  %485 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8, !noalias !68
  %486 = bitcast %"class.std::__1::basic_string"* %485 to %"class.std::__1::__basic_string_common"*
  %487 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %485, i32 0, i32 0
  %488 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !68
  %489 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %488, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %489, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !68
  %490 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !68
  %491 = bitcast %"class.std::__1::__compressed_pair"* %487 to i8*
  %492 = bitcast %"class.std::__1::__compressed_pair"* %490 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %491, i8* %492, i64 24, i32 8, i1 false) #10
  %493 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !68
  store %"class.std::__1::basic_string"* %493, %"class.std::__1::basic_string"** %58, align 8, !noalias !68
  %494 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8, !noalias !68
  %495 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %494, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %495, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !68
  %496 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !68
  %497 = bitcast %"class.std::__1::__compressed_pair"* %496 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %497, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !68
  %498 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !68
  %499 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %498, i32 0, i32 0
  %500 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %499, i32 0, i32 0
  %501 = bitcast %union.anon.0* %500 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %502 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %501, i32 0, i32 0
  store [3 x i64]* %502, [3 x i64]** %59, align 8, !noalias !68
  store i32 0, i32* %60, align 4, !noalias !68
  br label %503

; <label>:503:                                    ; preds = %506, %481
  %504 = load i32, i32* %60, align 4, !noalias !68
  %505 = icmp ult i32 %504, 3
  br i1 %505, label %506, label %513

; <label>:506:                                    ; preds = %503
  %507 = load i32, i32* %60, align 4, !noalias !68
  %508 = zext i32 %507 to i64
  %509 = load [3 x i64]*, [3 x i64]** %59, align 8, !noalias !68
  %510 = getelementptr inbounds [3 x i64], [3 x i64]* %509, i64 0, i64 %508
  store i64 0, i64* %510, align 8
  %511 = load i32, i32* %60, align 4, !noalias !68
  %512 = add i32 %511, 1
  store i32 %512, i32* %60, align 4, !noalias !68
  br label %503

; <label>:513:                                    ; preds = %503
  br label %514

; <label>:514:                                    ; preds = %513
  store %"class.std::__1::basic_string"* %128, %"class.std::__1::basic_string"** %54, align 8, !noalias !71
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %55, align 8, !noalias !71
  %515 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !71
  %516 = load i8*, i8** %55, align 8, !noalias !71
  %517 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %515, i8* %516)
          to label %518 unwind label %688

; <label>:518:                                    ; preds = %514
  store %"class.std::__1::basic_string"* %517, %"class.std::__1::basic_string"** %53, align 8, !noalias !71
  %519 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %127, %"class.std::__1::basic_string"** %51, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %519, %"class.std::__1::basic_string"** %52, align 8, !noalias !71
  %520 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !71
  %521 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %520, %"class.std::__1::basic_string"** %49, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %521, %"class.std::__1::basic_string"** %50, align 8, !noalias !71
  %522 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !71
  %523 = bitcast %"class.std::__1::basic_string"* %522 to %"class.std::__1::__basic_string_common"*
  %524 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %522, i32 0, i32 0
  %525 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !71
  %526 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %525, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %526, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !71
  %527 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !71
  %528 = bitcast %"class.std::__1::__compressed_pair"* %524 to i8*
  %529 = bitcast %"class.std::__1::__compressed_pair"* %527 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %528, i8* %529, i64 24, i32 8, i1 false) #10
  %530 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !71
  store %"class.std::__1::basic_string"* %530, %"class.std::__1::basic_string"** %45, align 8, !noalias !71
  %531 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !71
  %532 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %531, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %532, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !71
  %533 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !71
  %534 = bitcast %"class.std::__1::__compressed_pair"* %533 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %534, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !71
  %535 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !71
  %536 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %535, i32 0, i32 0
  %537 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %536, i32 0, i32 0
  %538 = bitcast %union.anon.0* %537 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %539 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %538, i32 0, i32 0
  store [3 x i64]* %539, [3 x i64]** %46, align 8, !noalias !71
  store i32 0, i32* %47, align 4, !noalias !71
  br label %540

; <label>:540:                                    ; preds = %543, %518
  %541 = load i32, i32* %47, align 4, !noalias !71
  %542 = icmp ult i32 %541, 3
  br i1 %542, label %543, label %550

; <label>:543:                                    ; preds = %540
  %544 = load i32, i32* %47, align 4, !noalias !71
  %545 = zext i32 %544 to i64
  %546 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !71
  %547 = getelementptr inbounds [3 x i64], [3 x i64]* %546, i64 0, i64 %545
  store i64 0, i64* %547, align 8
  %548 = load i32, i32* %47, align 4, !noalias !71
  %549 = add i32 %548, 1
  store i32 %549, i32* %47, align 4, !noalias !71
  br label %540

; <label>:550:                                    ; preds = %540
  br label %551

; <label>:551:                                    ; preds = %550
  %552 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %553 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %552, i32 0, i32 0
  %554 = load i32, i32* %553, align 8
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %132, i32 %554)
          to label %555 unwind label %692

; <label>:555:                                    ; preds = %551
  store %"class.std::__1::basic_string"* %127, %"class.std::__1::basic_string"** %41, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %132, %"class.std::__1::basic_string"** %42, align 8, !noalias !74
  %556 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !74
  %557 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %556, %"class.std::__1::basic_string"** %39, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %557, %"class.std::__1::basic_string"** %40, align 8, !noalias !74
  %558 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !74
  %559 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %559, %"class.std::__1::basic_string"** %38, align 8, !noalias !74
  %560 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %560, %"class.std::__1::basic_string"** %37, align 8, !noalias !74
  %561 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %561, %"class.std::__1::basic_string"** %36, align 8, !noalias !74
  %562 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !74
  %563 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %562, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %563, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !74
  %564 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !74
  %565 = bitcast %"class.std::__1::__compressed_pair"* %564 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %565, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !74
  %566 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !74
  %567 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %566, i32 0, i32 0
  %568 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %567, i32 0, i32 0
  %569 = bitcast %union.anon.0* %568 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %570 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %569, i32 0, i32 0
  %571 = bitcast %union.anon.1* %570 to i8*
  %572 = load i8, i8* %571, align 8, !noalias !74
  %573 = zext i8 %572 to i32
  %574 = and i32 %573, 1
  %575 = icmp ne i32 %574, 0
  br i1 %575, label %576, label %587

; <label>:576:                                    ; preds = %555
  store %"class.std::__1::basic_string"* %561, %"class.std::__1::basic_string"** %28, align 8, !noalias !74
  %577 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !74
  %578 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %577, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %578, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !74
  %579 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !74
  %580 = bitcast %"class.std::__1::__compressed_pair"* %579 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %580, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !74
  %581 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !74
  %582 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %581, i32 0, i32 0
  %583 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %582, i32 0, i32 0
  %584 = bitcast %union.anon.0* %583 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %585 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %584, i32 0, i32 2
  %586 = load i8*, i8** %585, align 8
  br label %600

; <label>:587:                                    ; preds = %555
  store %"class.std::__1::basic_string"* %561, %"class.std::__1::basic_string"** %33, align 8, !noalias !74
  %588 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !74
  %589 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %588, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %589, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !74
  %590 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !74
  %591 = bitcast %"class.std::__1::__compressed_pair"* %590 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %591, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !74
  %592 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !74
  %593 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %592, i32 0, i32 0
  %594 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %593, i32 0, i32 0
  %595 = bitcast %union.anon.0* %594 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %596 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %595, i32 0, i32 1
  %597 = getelementptr inbounds [23 x i8], [23 x i8]* %596, i64 0, i64 0
  store i8* %597, i8** %30, align 8, !noalias !74
  %598 = load i8*, i8** %30, align 8, !noalias !74
  store i8* %598, i8** %29, align 8, !noalias !74
  %599 = load i8*, i8** %29, align 8, !noalias !74
  br label %600

; <label>:600:                                    ; preds = %587, %576
  %601 = phi i8* [ %586, %576 ], [ %599, %587 ]
  store i8* %601, i8** %25, align 8, !noalias !74
  %602 = load i8*, i8** %25, align 8, !noalias !74
  %603 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %603, %"class.std::__1::basic_string"** %24, align 8, !noalias !74
  %604 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %604, %"class.std::__1::basic_string"** %23, align 8, !noalias !74
  %605 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !74
  %606 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %605, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %606, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !74
  %607 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !74
  %608 = bitcast %"class.std::__1::__compressed_pair"* %607 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %608, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !74
  %609 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !74
  %610 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %609, i32 0, i32 0
  %611 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %610, i32 0, i32 0
  %612 = bitcast %union.anon.0* %611 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %613 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %612, i32 0, i32 0
  %614 = bitcast %union.anon.1* %613 to i8*
  %615 = load i8, i8* %614, align 8, !noalias !74
  %616 = zext i8 %615 to i32
  %617 = and i32 %616, 1
  %618 = icmp ne i32 %617, 0
  br i1 %618, label %619, label %630

; <label>:619:                                    ; preds = %600
  store %"class.std::__1::basic_string"* %604, %"class.std::__1::basic_string"** %17, align 8, !noalias !74
  %620 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !74
  %621 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %620, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %621, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !74
  %622 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !74
  %623 = bitcast %"class.std::__1::__compressed_pair"* %622 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %623, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !74
  %624 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !74
  %625 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %624, i32 0, i32 0
  %626 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %625, i32 0, i32 0
  %627 = bitcast %union.anon.0* %626 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %628 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %627, i32 0, i32 1
  %629 = load i64, i64* %628, align 8
  br label %645

; <label>:630:                                    ; preds = %600
  store %"class.std::__1::basic_string"* %604, %"class.std::__1::basic_string"** %20, align 8, !noalias !74
  %631 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !74
  %632 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %631, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %632, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !74
  %633 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !74
  %634 = bitcast %"class.std::__1::__compressed_pair"* %633 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %634, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !74
  %635 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !74
  %636 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %635, i32 0, i32 0
  %637 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %636, i32 0, i32 0
  %638 = bitcast %union.anon.0* %637 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %639 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %638, i32 0, i32 0
  %640 = bitcast %union.anon.1* %639 to i8*
  %641 = load i8, i8* %640, align 8
  %642 = zext i8 %641 to i32
  %643 = ashr i32 %642, 1
  %644 = sext i32 %643 to i64
  br label %645

; <label>:645:                                    ; preds = %630, %619
  %646 = phi i64 [ %629, %619 ], [ %644, %630 ]
  %647 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %558, i8* %602, i64 %646)
          to label %648 unwind label %696

; <label>:648:                                    ; preds = %645
  store %"class.std::__1::basic_string"* %647, %"class.std::__1::basic_string"** %14, align 8, !noalias !74
  %649 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %126, %"class.std::__1::basic_string"** %12, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %649, %"class.std::__1::basic_string"** %13, align 8, !noalias !74
  %650 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !74
  %651 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %650, %"class.std::__1::basic_string"** %10, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %651, %"class.std::__1::basic_string"** %11, align 8, !noalias !74
  %652 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !74
  %653 = bitcast %"class.std::__1::basic_string"* %652 to %"class.std::__1::__basic_string_common"*
  %654 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %652, i32 0, i32 0
  %655 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !74
  %656 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %655, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %656, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !74
  %657 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !74
  %658 = bitcast %"class.std::__1::__compressed_pair"* %654 to i8*
  %659 = bitcast %"class.std::__1::__compressed_pair"* %657 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %658, i8* %659, i64 24, i32 8, i1 false) #10
  %660 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !74
  store %"class.std::__1::basic_string"* %660, %"class.std::__1::basic_string"** %6, align 8, !noalias !74
  %661 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !74
  %662 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %661, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %662, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !74
  %663 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !74
  %664 = bitcast %"class.std::__1::__compressed_pair"* %663 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %664, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !74
  %665 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !74
  %666 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %665, i32 0, i32 0
  %667 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %666, i32 0, i32 0
  %668 = bitcast %union.anon.0* %667 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %669 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %668, i32 0, i32 0
  store [3 x i64]* %669, [3 x i64]** %7, align 8, !noalias !74
  store i32 0, i32* %8, align 4, !noalias !74
  br label %670

; <label>:670:                                    ; preds = %673, %648
  %671 = load i32, i32* %8, align 4, !noalias !74
  %672 = icmp ult i32 %671, 3
  br i1 %672, label %673, label %680

; <label>:673:                                    ; preds = %670
  %674 = load i32, i32* %8, align 4, !noalias !74
  %675 = zext i32 %674 to i64
  %676 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !74
  %677 = getelementptr inbounds [3 x i64], [3 x i64]* %676, i64 0, i64 %675
  store i64 0, i64* %677, align 8
  %678 = load i32, i32* %8, align 4, !noalias !74
  %679 = add i32 %678, 1
  store i32 %679, i32* %8, align 4, !noalias !74
  br label %670

; <label>:680:                                    ; preds = %670
  br label %681

; <label>:681:                                    ; preds = %680
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %126, %class.AstNode* null)
          to label %682 unwind label %700

; <label>:682:                                    ; preds = %681
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %126) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %132) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %127) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %128) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %129) #10
  br label %683

; <label>:683:                                    ; preds = %682, %461, %451, %449, %212, %204, %196, %188, %183, %174
  ret void

; <label>:684:                                    ; preds = %474
  %685 = landingpad { i8*, i32 }
          cleanup
  %686 = extractvalue { i8*, i32 } %685, 0
  store i8* %686, i8** %130, align 8
  %687 = extractvalue { i8*, i32 } %685, 1
  store i32 %687, i32* %131, align 4
  br label %707

; <label>:688:                                    ; preds = %514
  %689 = landingpad { i8*, i32 }
          cleanup
  %690 = extractvalue { i8*, i32 } %689, 0
  store i8* %690, i8** %130, align 8
  %691 = extractvalue { i8*, i32 } %689, 1
  store i32 %691, i32* %131, align 4
  br label %706

; <label>:692:                                    ; preds = %551
  %693 = landingpad { i8*, i32 }
          cleanup
  %694 = extractvalue { i8*, i32 } %693, 0
  store i8* %694, i8** %130, align 8
  %695 = extractvalue { i8*, i32 } %693, 1
  store i32 %695, i32* %131, align 4
  br label %705

; <label>:696:                                    ; preds = %645
  %697 = landingpad { i8*, i32 }
          cleanup
  %698 = extractvalue { i8*, i32 } %697, 0
  store i8* %698, i8** %130, align 8
  %699 = extractvalue { i8*, i32 } %697, 1
  store i32 %699, i32* %131, align 4
  br label %704

; <label>:700:                                    ; preds = %681
  %701 = landingpad { i8*, i32 }
          cleanup
  %702 = extractvalue { i8*, i32 } %701, 0
  store i8* %702, i8** %130, align 8
  %703 = extractvalue { i8*, i32 } %701, 1
  store i32 %703, i32* %131, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %126) #10
  br label %704

; <label>:704:                                    ; preds = %700, %696
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %132) #10
  br label %705

; <label>:705:                                    ; preds = %704, %692
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %127) #10
  br label %706

; <label>:706:                                    ; preds = %705, %688
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %128) #10
  br label %707

; <label>:707:                                    ; preds = %706, %684
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %129) #10
  br label %708

; <label>:708:                                    ; preds = %707
  %709 = load i8*, i8** %130, align 8
  %710 = load i32, i32* %131, align 4
  %711 = insertvalue { i8*, i32 } undef, i8* %709, 0
  %712 = insertvalue { i8*, i32 } %711, i32 %710, 1
  resume { i8*, i32 } %712
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZNK7Variant7is_nullEv(%struct.Variant*) #3 align 2 {
  %2 = alloca %struct.Variant*, align 8
  store %struct.Variant* %0, %struct.Variant** %2, align 8
  %3 = load %struct.Variant*, %struct.Variant** %2, align 8
  %4 = getelementptr inbounds %struct.Variant, %struct.Variant* %3, i32 0, i32 5
  %5 = load i8, i8* %4, align 8
  %6 = trunc i8 %5 to i1
  ret i1 %6
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic7greaterE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  %69 = alloca %"class.std::__1::basic_string", align 8
  %70 = alloca %"class.std::__1::basic_string", align 8
  %71 = alloca %"class.std::__1::basic_string", align 8
  %72 = alloca %"class.std::__1::basic_string", align 8
  %73 = alloca i8*
  %74 = alloca i32
  %75 = alloca %"class.std::__1::basic_string", align 8
  %76 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %77 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %76, i32 0, i32 0
  %78 = load i32, i32* %77, align 8
  %79 = icmp eq i32 %78, 3
  br i1 %79, label %80, label %94

; <label>:80:                                     ; preds = %3
  %81 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %82 = bitcast %union.anon* %81 to double*
  %83 = load double, double* %82, align 8
  %84 = fcmp olt double %83, 0x41DFFFFFFFC00000
  br i1 %84, label %85, label %94

; <label>:85:                                     ; preds = %80
  %86 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %87 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %86, i32 0, i32 0
  store i32 0, i32* %87, align 8
  %88 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %89 = bitcast %union.anon* %88 to double*
  %90 = load double, double* %89, align 8
  %91 = fptosi double %90 to i32
  %92 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %93 = bitcast %union.anon* %92 to i32*
  store i32 %91, i32* %93, align 8
  br label %94

; <label>:94:                                     ; preds = %85, %80, %3
  %95 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %96 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %95, i32 0, i32 0
  %97 = load i32, i32* %96, align 8
  %98 = icmp eq i32 %97, 3
  br i1 %98, label %99, label %113

; <label>:99:                                     ; preds = %94
  %100 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %101 = bitcast %union.anon* %100 to double*
  %102 = load double, double* %101, align 8
  %103 = fcmp olt double %102, 0x41DFFFFFFFC00000
  br i1 %103, label %104, label %113

; <label>:104:                                    ; preds = %99
  %105 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %106 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %105, i32 0, i32 0
  store i32 0, i32* %106, align 8
  %107 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %108 = bitcast %union.anon* %107 to double*
  %109 = load double, double* %108, align 8
  %110 = fptosi double %109 to i32
  %111 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %112 = bitcast %union.anon* %111 to i32*
  store i32 %110, i32* %112, align 8
  br label %113

; <label>:113:                                    ; preds = %104, %99, %94
  %114 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %115 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %114, i32 0, i32 0
  %116 = load i32, i32* %115, align 8
  %117 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %118 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %117, i32 0, i32 0
  %119 = load i32, i32* %118, align 8
  %120 = icmp ne i32 %116, %119
  br i1 %120, label %121, label %122

; <label>:121:                                    ; preds = %113
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext false)
  br label %378

; <label>:122:                                    ; preds = %113
  %123 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %124 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %123, i32 0, i32 0
  %125 = load i32, i32* %124, align 8
  switch i32 %125, label %168 [
    i32 0, label %126
    i32 1, label %134
    i32 2, label %142
    i32 3, label %150
    i32 6, label %158
  ]

; <label>:126:                                    ; preds = %122
  %127 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %128 = bitcast %union.anon* %127 to i32*
  %129 = load i32, i32* %128, align 8
  %130 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %131 = bitcast %union.anon* %130 to i32*
  %132 = load i32, i32* %131, align 8
  %133 = icmp sgt i32 %129, %132
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %133)
  br label %378

; <label>:134:                                    ; preds = %122
  %135 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %136 = bitcast %union.anon* %135 to i64*
  %137 = load i64, i64* %136, align 8
  %138 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %139 = bitcast %union.anon* %138 to i64*
  %140 = load i64, i64* %139, align 8
  %141 = icmp sgt i64 %137, %140
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %141)
  br label %378

; <label>:142:                                    ; preds = %122
  %143 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %144 = bitcast %union.anon* %143 to float*
  %145 = load float, float* %144, align 8
  %146 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %147 = bitcast %union.anon* %146 to float*
  %148 = load float, float* %147, align 8
  %149 = fcmp ogt float %145, %148
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %149)
  br label %378

; <label>:150:                                    ; preds = %122
  %151 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %152 = bitcast %union.anon* %151 to double*
  %153 = load double, double* %152, align 8
  %154 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %155 = bitcast %union.anon* %154 to double*
  %156 = load double, double* %155, align 8
  %157 = fcmp ogt double %153, %156
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %157)
  br label %378

; <label>:158:                                    ; preds = %122
  %159 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %160 = bitcast %union.anon* %159 to i8*
  %161 = load i8, i8* %160, align 8
  %162 = sext i8 %161 to i32
  %163 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %164 = bitcast %union.anon* %163 to i8*
  %165 = load i8, i8* %164, align 8
  %166 = sext i8 %165 to i32
  %167 = icmp sgt i32 %162, %166
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %167)
  br label %378

; <label>:168:                                    ; preds = %122
  br label %169

; <label>:169:                                    ; preds = %168
  %170 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %171 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %170, i32 0, i32 0
  %172 = load i32, i32* %171, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %72, i32 %172)
  store i8* getelementptr inbounds ([56 x i8], [56 x i8]* @.str.6, i32 0, i32 0), i8** %67, align 8, !noalias !77
  store %"class.std::__1::basic_string"* %72, %"class.std::__1::basic_string"** %68, align 8, !noalias !77
  %173 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !77
  %174 = load i8*, i8** %67, align 8, !noalias !77
  %175 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %173, i64 0, i8* %174)
          to label %176 unwind label %379

; <label>:176:                                    ; preds = %169
  store %"class.std::__1::basic_string"* %175, %"class.std::__1::basic_string"** %66, align 8, !noalias !77
  %177 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !77
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %64, align 8, !noalias !77
  store %"class.std::__1::basic_string"* %177, %"class.std::__1::basic_string"** %65, align 8, !noalias !77
  %178 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !77
  %179 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !77
  store %"class.std::__1::basic_string"* %178, %"class.std::__1::basic_string"** %62, align 8, !noalias !77
  store %"class.std::__1::basic_string"* %179, %"class.std::__1::basic_string"** %63, align 8, !noalias !77
  %180 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8, !noalias !77
  %181 = bitcast %"class.std::__1::basic_string"* %180 to %"class.std::__1::__basic_string_common"*
  %182 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %180, i32 0, i32 0
  %183 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !77
  %184 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %183, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %184, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !77
  %185 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !77
  %186 = bitcast %"class.std::__1::__compressed_pair"* %182 to i8*
  %187 = bitcast %"class.std::__1::__compressed_pair"* %185 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %186, i8* %187, i64 24, i32 8, i1 false) #10
  %188 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !77
  store %"class.std::__1::basic_string"* %188, %"class.std::__1::basic_string"** %58, align 8, !noalias !77
  %189 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8, !noalias !77
  %190 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %189, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %190, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !77
  %191 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !77
  %192 = bitcast %"class.std::__1::__compressed_pair"* %191 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %192, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !77
  %193 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !77
  %194 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %193, i32 0, i32 0
  %195 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %194, i32 0, i32 0
  %196 = bitcast %union.anon.0* %195 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %197 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %196, i32 0, i32 0
  store [3 x i64]* %197, [3 x i64]** %59, align 8, !noalias !77
  store i32 0, i32* %60, align 4, !noalias !77
  br label %198

; <label>:198:                                    ; preds = %201, %176
  %199 = load i32, i32* %60, align 4, !noalias !77
  %200 = icmp ult i32 %199, 3
  br i1 %200, label %201, label %208

; <label>:201:                                    ; preds = %198
  %202 = load i32, i32* %60, align 4, !noalias !77
  %203 = zext i32 %202 to i64
  %204 = load [3 x i64]*, [3 x i64]** %59, align 8, !noalias !77
  %205 = getelementptr inbounds [3 x i64], [3 x i64]* %204, i64 0, i64 %203
  store i64 0, i64* %205, align 8
  %206 = load i32, i32* %60, align 4, !noalias !77
  %207 = add i32 %206, 1
  store i32 %207, i32* %60, align 4, !noalias !77
  br label %198

; <label>:208:                                    ; preds = %198
  br label %209

; <label>:209:                                    ; preds = %208
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %54, align 8, !noalias !80
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %55, align 8, !noalias !80
  %210 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !80
  %211 = load i8*, i8** %55, align 8, !noalias !80
  %212 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %210, i8* %211)
          to label %213 unwind label %383

; <label>:213:                                    ; preds = %209
  store %"class.std::__1::basic_string"* %212, %"class.std::__1::basic_string"** %53, align 8, !noalias !80
  %214 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %51, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %214, %"class.std::__1::basic_string"** %52, align 8, !noalias !80
  %215 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !80
  %216 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %215, %"class.std::__1::basic_string"** %49, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %216, %"class.std::__1::basic_string"** %50, align 8, !noalias !80
  %217 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !80
  %218 = bitcast %"class.std::__1::basic_string"* %217 to %"class.std::__1::__basic_string_common"*
  %219 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %217, i32 0, i32 0
  %220 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !80
  %221 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %220, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %221, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !80
  %222 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !80
  %223 = bitcast %"class.std::__1::__compressed_pair"* %219 to i8*
  %224 = bitcast %"class.std::__1::__compressed_pair"* %222 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %223, i8* %224, i64 24, i32 8, i1 false) #10
  %225 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !80
  store %"class.std::__1::basic_string"* %225, %"class.std::__1::basic_string"** %45, align 8, !noalias !80
  %226 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !80
  %227 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %226, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %227, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !80
  %228 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !80
  %229 = bitcast %"class.std::__1::__compressed_pair"* %228 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %229, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !80
  %230 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !80
  %231 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %230, i32 0, i32 0
  %232 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %231, i32 0, i32 0
  %233 = bitcast %union.anon.0* %232 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %234 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %233, i32 0, i32 0
  store [3 x i64]* %234, [3 x i64]** %46, align 8, !noalias !80
  store i32 0, i32* %47, align 4, !noalias !80
  br label %235

; <label>:235:                                    ; preds = %238, %213
  %236 = load i32, i32* %47, align 4, !noalias !80
  %237 = icmp ult i32 %236, 3
  br i1 %237, label %238, label %245

; <label>:238:                                    ; preds = %235
  %239 = load i32, i32* %47, align 4, !noalias !80
  %240 = zext i32 %239 to i64
  %241 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !80
  %242 = getelementptr inbounds [3 x i64], [3 x i64]* %241, i64 0, i64 %240
  store i64 0, i64* %242, align 8
  %243 = load i32, i32* %47, align 4, !noalias !80
  %244 = add i32 %243, 1
  store i32 %244, i32* %47, align 4, !noalias !80
  br label %235

; <label>:245:                                    ; preds = %235
  br label %246

; <label>:246:                                    ; preds = %245
  %247 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %248 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %247, i32 0, i32 0
  %249 = load i32, i32* %248, align 8
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %75, i32 %249)
          to label %250 unwind label %387

; <label>:250:                                    ; preds = %246
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %41, align 8, !noalias !83
  store %"class.std::__1::basic_string"* %75, %"class.std::__1::basic_string"** %42, align 8, !noalias !83
  %251 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !83
  %252 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !83
  store %"class.std::__1::basic_string"* %251, %"class.std::__1::basic_string"** %39, align 8, !noalias !83
  store %"class.std::__1::basic_string"* %252, %"class.std::__1::basic_string"** %40, align 8, !noalias !83
  %253 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !83
  %254 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !83
  store %"class.std::__1::basic_string"* %254, %"class.std::__1::basic_string"** %38, align 8, !noalias !83
  %255 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !83
  store %"class.std::__1::basic_string"* %255, %"class.std::__1::basic_string"** %37, align 8, !noalias !83
  %256 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !83
  store %"class.std::__1::basic_string"* %256, %"class.std::__1::basic_string"** %36, align 8, !noalias !83
  %257 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !83
  %258 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %257, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %258, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !83
  %259 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !83
  %260 = bitcast %"class.std::__1::__compressed_pair"* %259 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %260, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !83
  %261 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !83
  %262 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %261, i32 0, i32 0
  %263 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %262, i32 0, i32 0
  %264 = bitcast %union.anon.0* %263 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %265 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %264, i32 0, i32 0
  %266 = bitcast %union.anon.1* %265 to i8*
  %267 = load i8, i8* %266, align 8, !noalias !83
  %268 = zext i8 %267 to i32
  %269 = and i32 %268, 1
  %270 = icmp ne i32 %269, 0
  br i1 %270, label %271, label %282

; <label>:271:                                    ; preds = %250
  store %"class.std::__1::basic_string"* %256, %"class.std::__1::basic_string"** %28, align 8, !noalias !83
  %272 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !83
  %273 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %272, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %273, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !83
  %274 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !83
  %275 = bitcast %"class.std::__1::__compressed_pair"* %274 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %275, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !83
  %276 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !83
  %277 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %276, i32 0, i32 0
  %278 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %277, i32 0, i32 0
  %279 = bitcast %union.anon.0* %278 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %280 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %279, i32 0, i32 2
  %281 = load i8*, i8** %280, align 8
  br label %295

; <label>:282:                                    ; preds = %250
  store %"class.std::__1::basic_string"* %256, %"class.std::__1::basic_string"** %33, align 8, !noalias !83
  %283 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !83
  %284 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %283, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %284, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !83
  %285 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !83
  %286 = bitcast %"class.std::__1::__compressed_pair"* %285 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %286, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !83
  %287 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !83
  %288 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %287, i32 0, i32 0
  %289 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %288, i32 0, i32 0
  %290 = bitcast %union.anon.0* %289 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %291 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %290, i32 0, i32 1
  %292 = getelementptr inbounds [23 x i8], [23 x i8]* %291, i64 0, i64 0
  store i8* %292, i8** %30, align 8, !noalias !83
  %293 = load i8*, i8** %30, align 8, !noalias !83
  store i8* %293, i8** %29, align 8, !noalias !83
  %294 = load i8*, i8** %29, align 8, !noalias !83
  br label %295

; <label>:295:                                    ; preds = %282, %271
  %296 = phi i8* [ %281, %271 ], [ %294, %282 ]
  store i8* %296, i8** %25, align 8, !noalias !83
  %297 = load i8*, i8** %25, align 8, !noalias !83
  %298 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !83
  store %"class.std::__1::basic_string"* %298, %"class.std::__1::basic_string"** %24, align 8, !noalias !83
  %299 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !83
  store %"class.std::__1::basic_string"* %299, %"class.std::__1::basic_string"** %23, align 8, !noalias !83
  %300 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !83
  %301 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %300, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %301, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !83
  %302 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !83
  %303 = bitcast %"class.std::__1::__compressed_pair"* %302 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %303, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !83
  %304 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !83
  %305 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %304, i32 0, i32 0
  %306 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %305, i32 0, i32 0
  %307 = bitcast %union.anon.0* %306 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %308 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %307, i32 0, i32 0
  %309 = bitcast %union.anon.1* %308 to i8*
  %310 = load i8, i8* %309, align 8, !noalias !83
  %311 = zext i8 %310 to i32
  %312 = and i32 %311, 1
  %313 = icmp ne i32 %312, 0
  br i1 %313, label %314, label %325

; <label>:314:                                    ; preds = %295
  store %"class.std::__1::basic_string"* %299, %"class.std::__1::basic_string"** %17, align 8, !noalias !83
  %315 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !83
  %316 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %315, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %316, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !83
  %317 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !83
  %318 = bitcast %"class.std::__1::__compressed_pair"* %317 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %318, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !83
  %319 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !83
  %320 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %319, i32 0, i32 0
  %321 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %320, i32 0, i32 0
  %322 = bitcast %union.anon.0* %321 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %323 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %322, i32 0, i32 1
  %324 = load i64, i64* %323, align 8
  br label %340

; <label>:325:                                    ; preds = %295
  store %"class.std::__1::basic_string"* %299, %"class.std::__1::basic_string"** %20, align 8, !noalias !83
  %326 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !83
  %327 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %326, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %327, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !83
  %328 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !83
  %329 = bitcast %"class.std::__1::__compressed_pair"* %328 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %329, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !83
  %330 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !83
  %331 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %330, i32 0, i32 0
  %332 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %331, i32 0, i32 0
  %333 = bitcast %union.anon.0* %332 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %334 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %333, i32 0, i32 0
  %335 = bitcast %union.anon.1* %334 to i8*
  %336 = load i8, i8* %335, align 8
  %337 = zext i8 %336 to i32
  %338 = ashr i32 %337, 1
  %339 = sext i32 %338 to i64
  br label %340

; <label>:340:                                    ; preds = %325, %314
  %341 = phi i64 [ %324, %314 ], [ %339, %325 ]
  %342 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %253, i8* %297, i64 %341)
          to label %343 unwind label %391

; <label>:343:                                    ; preds = %340
  store %"class.std::__1::basic_string"* %342, %"class.std::__1::basic_string"** %14, align 8, !noalias !83
  %344 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !83
  store %"class.std::__1::basic_string"* %69, %"class.std::__1::basic_string"** %12, align 8, !noalias !83
  store %"class.std::__1::basic_string"* %344, %"class.std::__1::basic_string"** %13, align 8, !noalias !83
  %345 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !83
  %346 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !83
  store %"class.std::__1::basic_string"* %345, %"class.std::__1::basic_string"** %10, align 8, !noalias !83
  store %"class.std::__1::basic_string"* %346, %"class.std::__1::basic_string"** %11, align 8, !noalias !83
  %347 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !83
  %348 = bitcast %"class.std::__1::basic_string"* %347 to %"class.std::__1::__basic_string_common"*
  %349 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %347, i32 0, i32 0
  %350 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !83
  %351 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %350, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %351, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !83
  %352 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !83
  %353 = bitcast %"class.std::__1::__compressed_pair"* %349 to i8*
  %354 = bitcast %"class.std::__1::__compressed_pair"* %352 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %353, i8* %354, i64 24, i32 8, i1 false) #10
  %355 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !83
  store %"class.std::__1::basic_string"* %355, %"class.std::__1::basic_string"** %6, align 8, !noalias !83
  %356 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !83
  %357 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %356, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %357, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !83
  %358 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !83
  %359 = bitcast %"class.std::__1::__compressed_pair"* %358 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %359, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !83
  %360 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !83
  %361 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %360, i32 0, i32 0
  %362 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %361, i32 0, i32 0
  %363 = bitcast %union.anon.0* %362 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %364 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %363, i32 0, i32 0
  store [3 x i64]* %364, [3 x i64]** %7, align 8, !noalias !83
  store i32 0, i32* %8, align 4, !noalias !83
  br label %365

; <label>:365:                                    ; preds = %368, %343
  %366 = load i32, i32* %8, align 4, !noalias !83
  %367 = icmp ult i32 %366, 3
  br i1 %367, label %368, label %375

; <label>:368:                                    ; preds = %365
  %369 = load i32, i32* %8, align 4, !noalias !83
  %370 = zext i32 %369 to i64
  %371 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !83
  %372 = getelementptr inbounds [3 x i64], [3 x i64]* %371, i64 0, i64 %370
  store i64 0, i64* %372, align 8
  %373 = load i32, i32* %8, align 4, !noalias !83
  %374 = add i32 %373, 1
  store i32 %374, i32* %8, align 4, !noalias !83
  br label %365

; <label>:375:                                    ; preds = %365
  br label %376

; <label>:376:                                    ; preds = %375
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %69, %class.AstNode* null)
          to label %377 unwind label %395

; <label>:377:                                    ; preds = %376
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %378

; <label>:378:                                    ; preds = %377, %158, %150, %142, %134, %126, %121
  ret void

; <label>:379:                                    ; preds = %169
  %380 = landingpad { i8*, i32 }
          cleanup
  %381 = extractvalue { i8*, i32 } %380, 0
  store i8* %381, i8** %73, align 8
  %382 = extractvalue { i8*, i32 } %380, 1
  store i32 %382, i32* %74, align 4
  br label %402

; <label>:383:                                    ; preds = %209
  %384 = landingpad { i8*, i32 }
          cleanup
  %385 = extractvalue { i8*, i32 } %384, 0
  store i8* %385, i8** %73, align 8
  %386 = extractvalue { i8*, i32 } %384, 1
  store i32 %386, i32* %74, align 4
  br label %401

; <label>:387:                                    ; preds = %246
  %388 = landingpad { i8*, i32 }
          cleanup
  %389 = extractvalue { i8*, i32 } %388, 0
  store i8* %389, i8** %73, align 8
  %390 = extractvalue { i8*, i32 } %388, 1
  store i32 %390, i32* %74, align 4
  br label %400

; <label>:391:                                    ; preds = %340
  %392 = landingpad { i8*, i32 }
          cleanup
  %393 = extractvalue { i8*, i32 } %392, 0
  store i8* %393, i8** %73, align 8
  %394 = extractvalue { i8*, i32 } %392, 1
  store i32 %394, i32* %74, align 4
  br label %399

; <label>:395:                                    ; preds = %376
  %396 = landingpad { i8*, i32 }
          cleanup
  %397 = extractvalue { i8*, i32 } %396, 0
  store i8* %397, i8** %73, align 8
  %398 = extractvalue { i8*, i32 } %396, 1
  store i32 %398, i32* %74, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  br label %399

; <label>:399:                                    ; preds = %395, %391
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  br label %400

; <label>:400:                                    ; preds = %399, %387
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  br label %401

; <label>:401:                                    ; preds = %400, %383
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  br label %402

; <label>:402:                                    ; preds = %401, %379
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %403

; <label>:403:                                    ; preds = %402
  %404 = load i8*, i8** %73, align 8
  %405 = load i32, i32* %74, align 4
  %406 = insertvalue { i8*, i32 } undef, i8* %404, 0
  %407 = insertvalue { i8*, i32 } %406, i32 %405, 1
  resume { i8*, i32 } %407
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic9not_equalE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %struct.Variant, align 8
  %5 = alloca %struct.Variant, align 8
  %6 = alloca %struct.Variant, align 8
  %7 = alloca i8*
  %8 = alloca i32
  call void @_ZN7VariantC1ERKS_(%struct.Variant* %5, %struct.Variant* dereferenceable(264) %1)
  invoke void @_ZN7VariantC1ERKS_(%struct.Variant* %6, %struct.Variant* dereferenceable(264) %2)
          to label %9 unwind label %12

; <label>:9:                                      ; preds = %3
  invoke void @_ZN4cdot3var10Arithmetic5equalE7VariantS2_(%struct.Variant* sret %4, %struct.Variant* %5, %struct.Variant* %6)
          to label %10 unwind label %16

; <label>:10:                                     ; preds = %9
  invoke void @_ZN7VariantntEv(%struct.Variant* sret %0, %struct.Variant* %4)
          to label %11 unwind label %20

; <label>:11:                                     ; preds = %10
  call void @_ZN7VariantD1Ev(%struct.Variant* %4) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %6) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %5) #10
  ret void

; <label>:12:                                     ; preds = %3
  %13 = landingpad { i8*, i32 }
          cleanup
  %14 = extractvalue { i8*, i32 } %13, 0
  store i8* %14, i8** %7, align 8
  %15 = extractvalue { i8*, i32 } %13, 1
  store i32 %15, i32* %8, align 4
  br label %25

; <label>:16:                                     ; preds = %9
  %17 = landingpad { i8*, i32 }
          cleanup
  %18 = extractvalue { i8*, i32 } %17, 0
  store i8* %18, i8** %7, align 8
  %19 = extractvalue { i8*, i32 } %17, 1
  store i32 %19, i32* %8, align 4
  br label %24

; <label>:20:                                     ; preds = %10
  %21 = landingpad { i8*, i32 }
          cleanup
  %22 = extractvalue { i8*, i32 } %21, 0
  store i8* %22, i8** %7, align 8
  %23 = extractvalue { i8*, i32 } %21, 1
  store i32 %23, i32* %8, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %4) #10
  br label %24

; <label>:24:                                     ; preds = %20, %16
  call void @_ZN7VariantD1Ev(%struct.Variant* %6) #10
  br label %25

; <label>:25:                                     ; preds = %24, %12
  call void @_ZN7VariantD1Ev(%struct.Variant* %5) #10
  br label %26

; <label>:26:                                     ; preds = %25
  %27 = load i8*, i8** %7, align 8
  %28 = load i32, i32* %8, align 4
  %29 = insertvalue { i8*, i32 } undef, i8* %27, 0
  %30 = insertvalue { i8*, i32 } %29, i32 %28, 1
  resume { i8*, i32 } %30
}

declare void @_ZN7VariantntEv(%struct.Variant* sret, %struct.Variant*) #1

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic13greater_equalE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %struct.Variant, align 8
  %5 = alloca %struct.Variant, align 8
  %6 = alloca %struct.Variant, align 8
  %7 = alloca i8*
  %8 = alloca i32
  %9 = alloca %struct.Variant, align 8
  %10 = alloca %struct.Variant, align 8
  %11 = alloca %struct.Variant, align 8
  call void @_ZN7VariantC1ERKS_(%struct.Variant* %5, %struct.Variant* dereferenceable(264) %1)
  invoke void @_ZN7VariantC1ERKS_(%struct.Variant* %6, %struct.Variant* dereferenceable(264) %2)
          to label %12 unwind label %18

; <label>:12:                                     ; preds = %3
  invoke void @_ZN4cdot3var10Arithmetic5equalE7VariantS2_(%struct.Variant* sret %4, %struct.Variant* %5, %struct.Variant* %6)
          to label %13 unwind label %22

; <label>:13:                                     ; preds = %12
  invoke void @_ZN7VariantC1ERKS_(%struct.Variant* %10, %struct.Variant* dereferenceable(264) %1)
          to label %14 unwind label %26

; <label>:14:                                     ; preds = %13
  invoke void @_ZN7VariantC1ERKS_(%struct.Variant* %11, %struct.Variant* dereferenceable(264) %2)
          to label %15 unwind label %30

; <label>:15:                                     ; preds = %14
  invoke void @_ZN4cdot3var10Arithmetic7greaterE7VariantS2_(%struct.Variant* sret %9, %struct.Variant* %10, %struct.Variant* %11)
          to label %16 unwind label %34

; <label>:16:                                     ; preds = %15
  invoke void @_ZN7VariantooES_(%struct.Variant* sret %0, %struct.Variant* %4, %struct.Variant* %9)
          to label %17 unwind label %38

; <label>:17:                                     ; preds = %16
  call void @_ZN7VariantD1Ev(%struct.Variant* %9) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %11) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %10) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %4) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %6) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %5) #10
  ret void

; <label>:18:                                     ; preds = %3
  %19 = landingpad { i8*, i32 }
          cleanup
  %20 = extractvalue { i8*, i32 } %19, 0
  store i8* %20, i8** %7, align 8
  %21 = extractvalue { i8*, i32 } %19, 1
  store i32 %21, i32* %8, align 4
  br label %46

; <label>:22:                                     ; preds = %12
  %23 = landingpad { i8*, i32 }
          cleanup
  %24 = extractvalue { i8*, i32 } %23, 0
  store i8* %24, i8** %7, align 8
  %25 = extractvalue { i8*, i32 } %23, 1
  store i32 %25, i32* %8, align 4
  br label %45

; <label>:26:                                     ; preds = %13
  %27 = landingpad { i8*, i32 }
          cleanup
  %28 = extractvalue { i8*, i32 } %27, 0
  store i8* %28, i8** %7, align 8
  %29 = extractvalue { i8*, i32 } %27, 1
  store i32 %29, i32* %8, align 4
  br label %44

; <label>:30:                                     ; preds = %14
  %31 = landingpad { i8*, i32 }
          cleanup
  %32 = extractvalue { i8*, i32 } %31, 0
  store i8* %32, i8** %7, align 8
  %33 = extractvalue { i8*, i32 } %31, 1
  store i32 %33, i32* %8, align 4
  br label %43

; <label>:34:                                     ; preds = %15
  %35 = landingpad { i8*, i32 }
          cleanup
  %36 = extractvalue { i8*, i32 } %35, 0
  store i8* %36, i8** %7, align 8
  %37 = extractvalue { i8*, i32 } %35, 1
  store i32 %37, i32* %8, align 4
  br label %42

; <label>:38:                                     ; preds = %16
  %39 = landingpad { i8*, i32 }
          cleanup
  %40 = extractvalue { i8*, i32 } %39, 0
  store i8* %40, i8** %7, align 8
  %41 = extractvalue { i8*, i32 } %39, 1
  store i32 %41, i32* %8, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %9) #10
  br label %42

; <label>:42:                                     ; preds = %38, %34
  call void @_ZN7VariantD1Ev(%struct.Variant* %11) #10
  br label %43

; <label>:43:                                     ; preds = %42, %30
  call void @_ZN7VariantD1Ev(%struct.Variant* %10) #10
  br label %44

; <label>:44:                                     ; preds = %43, %26
  call void @_ZN7VariantD1Ev(%struct.Variant* %4) #10
  br label %45

; <label>:45:                                     ; preds = %44, %22
  call void @_ZN7VariantD1Ev(%struct.Variant* %6) #10
  br label %46

; <label>:46:                                     ; preds = %45, %18
  call void @_ZN7VariantD1Ev(%struct.Variant* %5) #10
  br label %47

; <label>:47:                                     ; preds = %46
  %48 = load i8*, i8** %7, align 8
  %49 = load i32, i32* %8, align 4
  %50 = insertvalue { i8*, i32 } undef, i8* %48, 0
  %51 = insertvalue { i8*, i32 } %50, i32 %49, 1
  resume { i8*, i32 } %51
}

declare void @_ZN7VariantooES_(%struct.Variant* sret, %struct.Variant*, %struct.Variant*) #1

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic5lowerE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %struct.Variant, align 8
  %5 = alloca %struct.Variant, align 8
  %6 = alloca %struct.Variant, align 8
  %7 = alloca %struct.Variant, align 8
  %8 = alloca i8*
  %9 = alloca i32
  %10 = alloca %struct.Variant, align 8
  %11 = alloca %struct.Variant, align 8
  %12 = alloca %struct.Variant, align 8
  %13 = alloca %struct.Variant, align 8
  call void @_ZN7VariantC1ERKS_(%struct.Variant* %6, %struct.Variant* dereferenceable(264) %1)
  invoke void @_ZN7VariantC1ERKS_(%struct.Variant* %7, %struct.Variant* dereferenceable(264) %2)
          to label %14 unwind label %22

; <label>:14:                                     ; preds = %3
  invoke void @_ZN4cdot3var10Arithmetic7greaterE7VariantS2_(%struct.Variant* sret %5, %struct.Variant* %6, %struct.Variant* %7)
          to label %15 unwind label %26

; <label>:15:                                     ; preds = %14
  invoke void @_ZN7VariantntEv(%struct.Variant* sret %4, %struct.Variant* %5)
          to label %16 unwind label %30

; <label>:16:                                     ; preds = %15
  invoke void @_ZN7VariantC1ERKS_(%struct.Variant* %12, %struct.Variant* dereferenceable(264) %1)
          to label %17 unwind label %34

; <label>:17:                                     ; preds = %16
  invoke void @_ZN7VariantC1ERKS_(%struct.Variant* %13, %struct.Variant* dereferenceable(264) %2)
          to label %18 unwind label %38

; <label>:18:                                     ; preds = %17
  invoke void @_ZN4cdot3var10Arithmetic5equalE7VariantS2_(%struct.Variant* sret %11, %struct.Variant* %12, %struct.Variant* %13)
          to label %19 unwind label %42

; <label>:19:                                     ; preds = %18
  invoke void @_ZN7VariantntEv(%struct.Variant* sret %10, %struct.Variant* %11)
          to label %20 unwind label %46

; <label>:20:                                     ; preds = %19
  invoke void @_ZN7VariantaaES_(%struct.Variant* sret %0, %struct.Variant* %4, %struct.Variant* %10)
          to label %21 unwind label %50

; <label>:21:                                     ; preds = %20
  call void @_ZN7VariantD1Ev(%struct.Variant* %10) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %11) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %13) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %12) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %4) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %5) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %7) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %6) #10
  ret void

; <label>:22:                                     ; preds = %3
  %23 = landingpad { i8*, i32 }
          cleanup
  %24 = extractvalue { i8*, i32 } %23, 0
  store i8* %24, i8** %8, align 8
  %25 = extractvalue { i8*, i32 } %23, 1
  store i32 %25, i32* %9, align 4
  br label %60

; <label>:26:                                     ; preds = %14
  %27 = landingpad { i8*, i32 }
          cleanup
  %28 = extractvalue { i8*, i32 } %27, 0
  store i8* %28, i8** %8, align 8
  %29 = extractvalue { i8*, i32 } %27, 1
  store i32 %29, i32* %9, align 4
  br label %59

; <label>:30:                                     ; preds = %15
  %31 = landingpad { i8*, i32 }
          cleanup
  %32 = extractvalue { i8*, i32 } %31, 0
  store i8* %32, i8** %8, align 8
  %33 = extractvalue { i8*, i32 } %31, 1
  store i32 %33, i32* %9, align 4
  br label %58

; <label>:34:                                     ; preds = %16
  %35 = landingpad { i8*, i32 }
          cleanup
  %36 = extractvalue { i8*, i32 } %35, 0
  store i8* %36, i8** %8, align 8
  %37 = extractvalue { i8*, i32 } %35, 1
  store i32 %37, i32* %9, align 4
  br label %57

; <label>:38:                                     ; preds = %17
  %39 = landingpad { i8*, i32 }
          cleanup
  %40 = extractvalue { i8*, i32 } %39, 0
  store i8* %40, i8** %8, align 8
  %41 = extractvalue { i8*, i32 } %39, 1
  store i32 %41, i32* %9, align 4
  br label %56

; <label>:42:                                     ; preds = %18
  %43 = landingpad { i8*, i32 }
          cleanup
  %44 = extractvalue { i8*, i32 } %43, 0
  store i8* %44, i8** %8, align 8
  %45 = extractvalue { i8*, i32 } %43, 1
  store i32 %45, i32* %9, align 4
  br label %55

; <label>:46:                                     ; preds = %19
  %47 = landingpad { i8*, i32 }
          cleanup
  %48 = extractvalue { i8*, i32 } %47, 0
  store i8* %48, i8** %8, align 8
  %49 = extractvalue { i8*, i32 } %47, 1
  store i32 %49, i32* %9, align 4
  br label %54

; <label>:50:                                     ; preds = %20
  %51 = landingpad { i8*, i32 }
          cleanup
  %52 = extractvalue { i8*, i32 } %51, 0
  store i8* %52, i8** %8, align 8
  %53 = extractvalue { i8*, i32 } %51, 1
  store i32 %53, i32* %9, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %10) #10
  br label %54

; <label>:54:                                     ; preds = %50, %46
  call void @_ZN7VariantD1Ev(%struct.Variant* %11) #10
  br label %55

; <label>:55:                                     ; preds = %54, %42
  call void @_ZN7VariantD1Ev(%struct.Variant* %13) #10
  br label %56

; <label>:56:                                     ; preds = %55, %38
  call void @_ZN7VariantD1Ev(%struct.Variant* %12) #10
  br label %57

; <label>:57:                                     ; preds = %56, %34
  call void @_ZN7VariantD1Ev(%struct.Variant* %4) #10
  br label %58

; <label>:58:                                     ; preds = %57, %30
  call void @_ZN7VariantD1Ev(%struct.Variant* %5) #10
  br label %59

; <label>:59:                                     ; preds = %58, %26
  call void @_ZN7VariantD1Ev(%struct.Variant* %7) #10
  br label %60

; <label>:60:                                     ; preds = %59, %22
  call void @_ZN7VariantD1Ev(%struct.Variant* %6) #10
  br label %61

; <label>:61:                                     ; preds = %60
  %62 = load i8*, i8** %8, align 8
  %63 = load i32, i32* %9, align 4
  %64 = insertvalue { i8*, i32 } undef, i8* %62, 0
  %65 = insertvalue { i8*, i32 } %64, i32 %63, 1
  resume { i8*, i32 } %65
}

declare void @_ZN7VariantaaES_(%struct.Variant* sret, %struct.Variant*, %struct.Variant*) #1

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic11lower_equalE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %struct.Variant, align 8
  %5 = alloca %struct.Variant, align 8
  %6 = alloca %struct.Variant, align 8
  %7 = alloca i8*
  %8 = alloca i32
  call void @_ZN7VariantC1ERKS_(%struct.Variant* %5, %struct.Variant* dereferenceable(264) %1)
  invoke void @_ZN7VariantC1ERKS_(%struct.Variant* %6, %struct.Variant* dereferenceable(264) %2)
          to label %9 unwind label %12

; <label>:9:                                      ; preds = %3
  invoke void @_ZN4cdot3var10Arithmetic7greaterE7VariantS2_(%struct.Variant* sret %4, %struct.Variant* %5, %struct.Variant* %6)
          to label %10 unwind label %16

; <label>:10:                                     ; preds = %9
  invoke void @_ZN7VariantntEv(%struct.Variant* sret %0, %struct.Variant* %4)
          to label %11 unwind label %20

; <label>:11:                                     ; preds = %10
  call void @_ZN7VariantD1Ev(%struct.Variant* %4) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %6) #10
  call void @_ZN7VariantD1Ev(%struct.Variant* %5) #10
  ret void

; <label>:12:                                     ; preds = %3
  %13 = landingpad { i8*, i32 }
          cleanup
  %14 = extractvalue { i8*, i32 } %13, 0
  store i8* %14, i8** %7, align 8
  %15 = extractvalue { i8*, i32 } %13, 1
  store i32 %15, i32* %8, align 4
  br label %25

; <label>:16:                                     ; preds = %9
  %17 = landingpad { i8*, i32 }
          cleanup
  %18 = extractvalue { i8*, i32 } %17, 0
  store i8* %18, i8** %7, align 8
  %19 = extractvalue { i8*, i32 } %17, 1
  store i32 %19, i32* %8, align 4
  br label %24

; <label>:20:                                     ; preds = %10
  %21 = landingpad { i8*, i32 }
          cleanup
  %22 = extractvalue { i8*, i32 } %21, 0
  store i8* %22, i8** %7, align 8
  %23 = extractvalue { i8*, i32 } %21, 1
  store i32 %23, i32* %8, align 4
  call void @_ZN7VariantD1Ev(%struct.Variant* %4) #10
  br label %24

; <label>:24:                                     ; preds = %20, %16
  call void @_ZN7VariantD1Ev(%struct.Variant* %6) #10
  br label %25

; <label>:25:                                     ; preds = %24, %12
  call void @_ZN7VariantD1Ev(%struct.Variant* %5) #10
  br label %26

; <label>:26:                                     ; preds = %25
  %27 = load i8*, i8** %7, align 8
  %28 = load i32, i32* %8, align 4
  %29 = insertvalue { i8*, i32 } undef, i8* %27, 0
  %30 = insertvalue { i8*, i32 } %29, i32 %28, 1
  resume { i8*, i32 } %30
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic11logical_andE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  %69 = alloca %"class.std::__1::basic_string", align 8
  %70 = alloca %"class.std::__1::basic_string", align 8
  %71 = alloca %"class.std::__1::basic_string", align 8
  %72 = alloca %"class.std::__1::basic_string", align 8
  %73 = alloca i8*
  %74 = alloca i32
  %75 = alloca %"class.std::__1::basic_string", align 8
  %76 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %77 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %76, i32 0, i32 0
  %78 = load i32, i32* %77, align 8
  %79 = icmp eq i32 %78, 5
  br i1 %79, label %80, label %97

; <label>:80:                                     ; preds = %3
  %81 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %82 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %81, i32 0, i32 0
  %83 = load i32, i32* %82, align 8
  %84 = icmp eq i32 %83, 5
  br i1 %84, label %85, label %97

; <label>:85:                                     ; preds = %80
  %86 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %87 = bitcast %union.anon* %86 to i8*
  %88 = load i8, i8* %87, align 8
  %89 = trunc i8 %88 to i1
  br i1 %89, label %90, label %95

; <label>:90:                                     ; preds = %85
  %91 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %92 = bitcast %union.anon* %91 to i8*
  %93 = load i8, i8* %92, align 8
  %94 = trunc i8 %93 to i1
  br label %95

; <label>:95:                                     ; preds = %90, %85
  %96 = phi i1 [ false, %85 ], [ %94, %90 ]
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %96)
  br label %306

; <label>:97:                                     ; preds = %80, %3
  %98 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %99 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %98, i32 0, i32 0
  %100 = load i32, i32* %99, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %72, i32 %100)
  store i8* getelementptr inbounds ([44 x i8], [44 x i8]* @.str.7, i32 0, i32 0), i8** %67, align 8, !noalias !86
  store %"class.std::__1::basic_string"* %72, %"class.std::__1::basic_string"** %68, align 8, !noalias !86
  %101 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !86
  %102 = load i8*, i8** %67, align 8, !noalias !86
  %103 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %101, i64 0, i8* %102)
          to label %104 unwind label %307

; <label>:104:                                    ; preds = %97
  store %"class.std::__1::basic_string"* %103, %"class.std::__1::basic_string"** %66, align 8, !noalias !86
  %105 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !86
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %64, align 8, !noalias !86
  store %"class.std::__1::basic_string"* %105, %"class.std::__1::basic_string"** %65, align 8, !noalias !86
  %106 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !86
  %107 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !86
  store %"class.std::__1::basic_string"* %106, %"class.std::__1::basic_string"** %62, align 8, !noalias !86
  store %"class.std::__1::basic_string"* %107, %"class.std::__1::basic_string"** %63, align 8, !noalias !86
  %108 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8, !noalias !86
  %109 = bitcast %"class.std::__1::basic_string"* %108 to %"class.std::__1::__basic_string_common"*
  %110 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %108, i32 0, i32 0
  %111 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !86
  %112 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %111, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %112, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !86
  %113 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !86
  %114 = bitcast %"class.std::__1::__compressed_pair"* %110 to i8*
  %115 = bitcast %"class.std::__1::__compressed_pair"* %113 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %114, i8* %115, i64 24, i32 8, i1 false) #10
  %116 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !86
  store %"class.std::__1::basic_string"* %116, %"class.std::__1::basic_string"** %58, align 8, !noalias !86
  %117 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8, !noalias !86
  %118 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %117, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %118, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !86
  %119 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !86
  %120 = bitcast %"class.std::__1::__compressed_pair"* %119 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %120, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !86
  %121 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !86
  %122 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %121, i32 0, i32 0
  %123 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %122, i32 0, i32 0
  %124 = bitcast %union.anon.0* %123 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %125 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %124, i32 0, i32 0
  store [3 x i64]* %125, [3 x i64]** %59, align 8, !noalias !86
  store i32 0, i32* %60, align 4, !noalias !86
  br label %126

; <label>:126:                                    ; preds = %129, %104
  %127 = load i32, i32* %60, align 4, !noalias !86
  %128 = icmp ult i32 %127, 3
  br i1 %128, label %129, label %136

; <label>:129:                                    ; preds = %126
  %130 = load i32, i32* %60, align 4, !noalias !86
  %131 = zext i32 %130 to i64
  %132 = load [3 x i64]*, [3 x i64]** %59, align 8, !noalias !86
  %133 = getelementptr inbounds [3 x i64], [3 x i64]* %132, i64 0, i64 %131
  store i64 0, i64* %133, align 8
  %134 = load i32, i32* %60, align 4, !noalias !86
  %135 = add i32 %134, 1
  store i32 %135, i32* %60, align 4, !noalias !86
  br label %126

; <label>:136:                                    ; preds = %126
  br label %137

; <label>:137:                                    ; preds = %136
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %54, align 8, !noalias !89
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %55, align 8, !noalias !89
  %138 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !89
  %139 = load i8*, i8** %55, align 8, !noalias !89
  %140 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %138, i8* %139)
          to label %141 unwind label %311

; <label>:141:                                    ; preds = %137
  store %"class.std::__1::basic_string"* %140, %"class.std::__1::basic_string"** %53, align 8, !noalias !89
  %142 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !89
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %51, align 8, !noalias !89
  store %"class.std::__1::basic_string"* %142, %"class.std::__1::basic_string"** %52, align 8, !noalias !89
  %143 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !89
  %144 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !89
  store %"class.std::__1::basic_string"* %143, %"class.std::__1::basic_string"** %49, align 8, !noalias !89
  store %"class.std::__1::basic_string"* %144, %"class.std::__1::basic_string"** %50, align 8, !noalias !89
  %145 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !89
  %146 = bitcast %"class.std::__1::basic_string"* %145 to %"class.std::__1::__basic_string_common"*
  %147 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %145, i32 0, i32 0
  %148 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !89
  %149 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %148, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %149, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !89
  %150 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !89
  %151 = bitcast %"class.std::__1::__compressed_pair"* %147 to i8*
  %152 = bitcast %"class.std::__1::__compressed_pair"* %150 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %151, i8* %152, i64 24, i32 8, i1 false) #10
  %153 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !89
  store %"class.std::__1::basic_string"* %153, %"class.std::__1::basic_string"** %45, align 8, !noalias !89
  %154 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !89
  %155 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %154, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %155, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !89
  %156 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !89
  %157 = bitcast %"class.std::__1::__compressed_pair"* %156 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %157, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !89
  %158 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !89
  %159 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %158, i32 0, i32 0
  %160 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %159, i32 0, i32 0
  %161 = bitcast %union.anon.0* %160 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %162 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %161, i32 0, i32 0
  store [3 x i64]* %162, [3 x i64]** %46, align 8, !noalias !89
  store i32 0, i32* %47, align 4, !noalias !89
  br label %163

; <label>:163:                                    ; preds = %166, %141
  %164 = load i32, i32* %47, align 4, !noalias !89
  %165 = icmp ult i32 %164, 3
  br i1 %165, label %166, label %173

; <label>:166:                                    ; preds = %163
  %167 = load i32, i32* %47, align 4, !noalias !89
  %168 = zext i32 %167 to i64
  %169 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !89
  %170 = getelementptr inbounds [3 x i64], [3 x i64]* %169, i64 0, i64 %168
  store i64 0, i64* %170, align 8
  %171 = load i32, i32* %47, align 4, !noalias !89
  %172 = add i32 %171, 1
  store i32 %172, i32* %47, align 4, !noalias !89
  br label %163

; <label>:173:                                    ; preds = %163
  br label %174

; <label>:174:                                    ; preds = %173
  %175 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %176 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %175, i32 0, i32 0
  %177 = load i32, i32* %176, align 8
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %75, i32 %177)
          to label %178 unwind label %315

; <label>:178:                                    ; preds = %174
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %41, align 8, !noalias !92
  store %"class.std::__1::basic_string"* %75, %"class.std::__1::basic_string"** %42, align 8, !noalias !92
  %179 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !92
  %180 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !92
  store %"class.std::__1::basic_string"* %179, %"class.std::__1::basic_string"** %39, align 8, !noalias !92
  store %"class.std::__1::basic_string"* %180, %"class.std::__1::basic_string"** %40, align 8, !noalias !92
  %181 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !92
  %182 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !92
  store %"class.std::__1::basic_string"* %182, %"class.std::__1::basic_string"** %38, align 8, !noalias !92
  %183 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !92
  store %"class.std::__1::basic_string"* %183, %"class.std::__1::basic_string"** %37, align 8, !noalias !92
  %184 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !92
  store %"class.std::__1::basic_string"* %184, %"class.std::__1::basic_string"** %36, align 8, !noalias !92
  %185 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !92
  %186 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %185, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %186, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !92
  %187 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !92
  %188 = bitcast %"class.std::__1::__compressed_pair"* %187 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %188, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !92
  %189 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !92
  %190 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %189, i32 0, i32 0
  %191 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %190, i32 0, i32 0
  %192 = bitcast %union.anon.0* %191 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %193 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %192, i32 0, i32 0
  %194 = bitcast %union.anon.1* %193 to i8*
  %195 = load i8, i8* %194, align 8, !noalias !92
  %196 = zext i8 %195 to i32
  %197 = and i32 %196, 1
  %198 = icmp ne i32 %197, 0
  br i1 %198, label %199, label %210

; <label>:199:                                    ; preds = %178
  store %"class.std::__1::basic_string"* %184, %"class.std::__1::basic_string"** %28, align 8, !noalias !92
  %200 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !92
  %201 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %200, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %201, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !92
  %202 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !92
  %203 = bitcast %"class.std::__1::__compressed_pair"* %202 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %203, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !92
  %204 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !92
  %205 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %204, i32 0, i32 0
  %206 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %205, i32 0, i32 0
  %207 = bitcast %union.anon.0* %206 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %208 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %207, i32 0, i32 2
  %209 = load i8*, i8** %208, align 8
  br label %223

; <label>:210:                                    ; preds = %178
  store %"class.std::__1::basic_string"* %184, %"class.std::__1::basic_string"** %33, align 8, !noalias !92
  %211 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !92
  %212 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %211, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %212, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !92
  %213 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !92
  %214 = bitcast %"class.std::__1::__compressed_pair"* %213 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %214, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !92
  %215 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !92
  %216 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %215, i32 0, i32 0
  %217 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %216, i32 0, i32 0
  %218 = bitcast %union.anon.0* %217 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %219 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %218, i32 0, i32 1
  %220 = getelementptr inbounds [23 x i8], [23 x i8]* %219, i64 0, i64 0
  store i8* %220, i8** %30, align 8, !noalias !92
  %221 = load i8*, i8** %30, align 8, !noalias !92
  store i8* %221, i8** %29, align 8, !noalias !92
  %222 = load i8*, i8** %29, align 8, !noalias !92
  br label %223

; <label>:223:                                    ; preds = %210, %199
  %224 = phi i8* [ %209, %199 ], [ %222, %210 ]
  store i8* %224, i8** %25, align 8, !noalias !92
  %225 = load i8*, i8** %25, align 8, !noalias !92
  %226 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !92
  store %"class.std::__1::basic_string"* %226, %"class.std::__1::basic_string"** %24, align 8, !noalias !92
  %227 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !92
  store %"class.std::__1::basic_string"* %227, %"class.std::__1::basic_string"** %23, align 8, !noalias !92
  %228 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !92
  %229 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %228, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %229, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !92
  %230 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !92
  %231 = bitcast %"class.std::__1::__compressed_pair"* %230 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %231, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !92
  %232 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !92
  %233 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %232, i32 0, i32 0
  %234 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %233, i32 0, i32 0
  %235 = bitcast %union.anon.0* %234 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %236 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %235, i32 0, i32 0
  %237 = bitcast %union.anon.1* %236 to i8*
  %238 = load i8, i8* %237, align 8, !noalias !92
  %239 = zext i8 %238 to i32
  %240 = and i32 %239, 1
  %241 = icmp ne i32 %240, 0
  br i1 %241, label %242, label %253

; <label>:242:                                    ; preds = %223
  store %"class.std::__1::basic_string"* %227, %"class.std::__1::basic_string"** %17, align 8, !noalias !92
  %243 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !92
  %244 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %243, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %244, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !92
  %245 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !92
  %246 = bitcast %"class.std::__1::__compressed_pair"* %245 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %246, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !92
  %247 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !92
  %248 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %247, i32 0, i32 0
  %249 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %248, i32 0, i32 0
  %250 = bitcast %union.anon.0* %249 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %251 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %250, i32 0, i32 1
  %252 = load i64, i64* %251, align 8
  br label %268

; <label>:253:                                    ; preds = %223
  store %"class.std::__1::basic_string"* %227, %"class.std::__1::basic_string"** %20, align 8, !noalias !92
  %254 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !92
  %255 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %254, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %255, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !92
  %256 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !92
  %257 = bitcast %"class.std::__1::__compressed_pair"* %256 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %257, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !92
  %258 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !92
  %259 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %258, i32 0, i32 0
  %260 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %259, i32 0, i32 0
  %261 = bitcast %union.anon.0* %260 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %262 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %261, i32 0, i32 0
  %263 = bitcast %union.anon.1* %262 to i8*
  %264 = load i8, i8* %263, align 8
  %265 = zext i8 %264 to i32
  %266 = ashr i32 %265, 1
  %267 = sext i32 %266 to i64
  br label %268

; <label>:268:                                    ; preds = %253, %242
  %269 = phi i64 [ %252, %242 ], [ %267, %253 ]
  %270 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %181, i8* %225, i64 %269)
          to label %271 unwind label %319

; <label>:271:                                    ; preds = %268
  store %"class.std::__1::basic_string"* %270, %"class.std::__1::basic_string"** %14, align 8, !noalias !92
  %272 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !92
  store %"class.std::__1::basic_string"* %69, %"class.std::__1::basic_string"** %12, align 8, !noalias !92
  store %"class.std::__1::basic_string"* %272, %"class.std::__1::basic_string"** %13, align 8, !noalias !92
  %273 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !92
  %274 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !92
  store %"class.std::__1::basic_string"* %273, %"class.std::__1::basic_string"** %10, align 8, !noalias !92
  store %"class.std::__1::basic_string"* %274, %"class.std::__1::basic_string"** %11, align 8, !noalias !92
  %275 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !92
  %276 = bitcast %"class.std::__1::basic_string"* %275 to %"class.std::__1::__basic_string_common"*
  %277 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %275, i32 0, i32 0
  %278 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !92
  %279 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %278, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %279, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !92
  %280 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !92
  %281 = bitcast %"class.std::__1::__compressed_pair"* %277 to i8*
  %282 = bitcast %"class.std::__1::__compressed_pair"* %280 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %281, i8* %282, i64 24, i32 8, i1 false) #10
  %283 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !92
  store %"class.std::__1::basic_string"* %283, %"class.std::__1::basic_string"** %6, align 8, !noalias !92
  %284 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !92
  %285 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %284, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %285, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !92
  %286 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !92
  %287 = bitcast %"class.std::__1::__compressed_pair"* %286 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %287, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !92
  %288 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !92
  %289 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %288, i32 0, i32 0
  %290 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %289, i32 0, i32 0
  %291 = bitcast %union.anon.0* %290 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %292 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %291, i32 0, i32 0
  store [3 x i64]* %292, [3 x i64]** %7, align 8, !noalias !92
  store i32 0, i32* %8, align 4, !noalias !92
  br label %293

; <label>:293:                                    ; preds = %296, %271
  %294 = load i32, i32* %8, align 4, !noalias !92
  %295 = icmp ult i32 %294, 3
  br i1 %295, label %296, label %303

; <label>:296:                                    ; preds = %293
  %297 = load i32, i32* %8, align 4, !noalias !92
  %298 = zext i32 %297 to i64
  %299 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !92
  %300 = getelementptr inbounds [3 x i64], [3 x i64]* %299, i64 0, i64 %298
  store i64 0, i64* %300, align 8
  %301 = load i32, i32* %8, align 4, !noalias !92
  %302 = add i32 %301, 1
  store i32 %302, i32* %8, align 4, !noalias !92
  br label %293

; <label>:303:                                    ; preds = %293
  br label %304

; <label>:304:                                    ; preds = %303
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %69, %class.AstNode* null)
          to label %305 unwind label %323

; <label>:305:                                    ; preds = %304
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %306

; <label>:306:                                    ; preds = %305, %95
  ret void

; <label>:307:                                    ; preds = %97
  %308 = landingpad { i8*, i32 }
          cleanup
  %309 = extractvalue { i8*, i32 } %308, 0
  store i8* %309, i8** %73, align 8
  %310 = extractvalue { i8*, i32 } %308, 1
  store i32 %310, i32* %74, align 4
  br label %330

; <label>:311:                                    ; preds = %137
  %312 = landingpad { i8*, i32 }
          cleanup
  %313 = extractvalue { i8*, i32 } %312, 0
  store i8* %313, i8** %73, align 8
  %314 = extractvalue { i8*, i32 } %312, 1
  store i32 %314, i32* %74, align 4
  br label %329

; <label>:315:                                    ; preds = %174
  %316 = landingpad { i8*, i32 }
          cleanup
  %317 = extractvalue { i8*, i32 } %316, 0
  store i8* %317, i8** %73, align 8
  %318 = extractvalue { i8*, i32 } %316, 1
  store i32 %318, i32* %74, align 4
  br label %328

; <label>:319:                                    ; preds = %268
  %320 = landingpad { i8*, i32 }
          cleanup
  %321 = extractvalue { i8*, i32 } %320, 0
  store i8* %321, i8** %73, align 8
  %322 = extractvalue { i8*, i32 } %320, 1
  store i32 %322, i32* %74, align 4
  br label %327

; <label>:323:                                    ; preds = %304
  %324 = landingpad { i8*, i32 }
          cleanup
  %325 = extractvalue { i8*, i32 } %324, 0
  store i8* %325, i8** %73, align 8
  %326 = extractvalue { i8*, i32 } %324, 1
  store i32 %326, i32* %74, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  br label %327

; <label>:327:                                    ; preds = %323, %319
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  br label %328

; <label>:328:                                    ; preds = %327, %315
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  br label %329

; <label>:329:                                    ; preds = %328, %311
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  br label %330

; <label>:330:                                    ; preds = %329, %307
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %331

; <label>:331:                                    ; preds = %330
  %332 = load i8*, i8** %73, align 8
  %333 = load i32, i32* %74, align 4
  %334 = insertvalue { i8*, i32 } undef, i8* %332, 0
  %335 = insertvalue { i8*, i32 } %334, i32 %333, 1
  resume { i8*, i32 } %335
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic10logical_orE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  %69 = alloca %"class.std::__1::basic_string", align 8
  %70 = alloca %"class.std::__1::basic_string", align 8
  %71 = alloca %"class.std::__1::basic_string", align 8
  %72 = alloca %"class.std::__1::basic_string", align 8
  %73 = alloca i8*
  %74 = alloca i32
  %75 = alloca %"class.std::__1::basic_string", align 8
  %76 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %77 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %76, i32 0, i32 0
  %78 = load i32, i32* %77, align 8
  %79 = icmp eq i32 %78, 5
  br i1 %79, label %80, label %97

; <label>:80:                                     ; preds = %3
  %81 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %82 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %81, i32 0, i32 0
  %83 = load i32, i32* %82, align 8
  %84 = icmp eq i32 %83, 5
  br i1 %84, label %85, label %97

; <label>:85:                                     ; preds = %80
  %86 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %87 = bitcast %union.anon* %86 to i8*
  %88 = load i8, i8* %87, align 8
  %89 = trunc i8 %88 to i1
  br i1 %89, label %95, label %90

; <label>:90:                                     ; preds = %85
  %91 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %92 = bitcast %union.anon* %91 to i8*
  %93 = load i8, i8* %92, align 8
  %94 = trunc i8 %93 to i1
  br label %95

; <label>:95:                                     ; preds = %90, %85
  %96 = phi i1 [ true, %85 ], [ %94, %90 ]
  call void @_ZN7VariantC1Eb(%struct.Variant* %0, i1 zeroext %96)
  br label %306

; <label>:97:                                     ; preds = %80, %3
  %98 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %99 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %98, i32 0, i32 0
  %100 = load i32, i32* %99, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %72, i32 %100)
  store i8* getelementptr inbounds ([43 x i8], [43 x i8]* @.str.8, i32 0, i32 0), i8** %67, align 8, !noalias !95
  store %"class.std::__1::basic_string"* %72, %"class.std::__1::basic_string"** %68, align 8, !noalias !95
  %101 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !95
  %102 = load i8*, i8** %67, align 8, !noalias !95
  %103 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %101, i64 0, i8* %102)
          to label %104 unwind label %307

; <label>:104:                                    ; preds = %97
  store %"class.std::__1::basic_string"* %103, %"class.std::__1::basic_string"** %66, align 8, !noalias !95
  %105 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !95
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %64, align 8, !noalias !95
  store %"class.std::__1::basic_string"* %105, %"class.std::__1::basic_string"** %65, align 8, !noalias !95
  %106 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !95
  %107 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !95
  store %"class.std::__1::basic_string"* %106, %"class.std::__1::basic_string"** %62, align 8, !noalias !95
  store %"class.std::__1::basic_string"* %107, %"class.std::__1::basic_string"** %63, align 8, !noalias !95
  %108 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8, !noalias !95
  %109 = bitcast %"class.std::__1::basic_string"* %108 to %"class.std::__1::__basic_string_common"*
  %110 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %108, i32 0, i32 0
  %111 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !95
  %112 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %111, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %112, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !95
  %113 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !95
  %114 = bitcast %"class.std::__1::__compressed_pair"* %110 to i8*
  %115 = bitcast %"class.std::__1::__compressed_pair"* %113 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %114, i8* %115, i64 24, i32 8, i1 false) #10
  %116 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !95
  store %"class.std::__1::basic_string"* %116, %"class.std::__1::basic_string"** %58, align 8, !noalias !95
  %117 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8, !noalias !95
  %118 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %117, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %118, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !95
  %119 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !95
  %120 = bitcast %"class.std::__1::__compressed_pair"* %119 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %120, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !95
  %121 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !95
  %122 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %121, i32 0, i32 0
  %123 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %122, i32 0, i32 0
  %124 = bitcast %union.anon.0* %123 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %125 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %124, i32 0, i32 0
  store [3 x i64]* %125, [3 x i64]** %59, align 8, !noalias !95
  store i32 0, i32* %60, align 4, !noalias !95
  br label %126

; <label>:126:                                    ; preds = %129, %104
  %127 = load i32, i32* %60, align 4, !noalias !95
  %128 = icmp ult i32 %127, 3
  br i1 %128, label %129, label %136

; <label>:129:                                    ; preds = %126
  %130 = load i32, i32* %60, align 4, !noalias !95
  %131 = zext i32 %130 to i64
  %132 = load [3 x i64]*, [3 x i64]** %59, align 8, !noalias !95
  %133 = getelementptr inbounds [3 x i64], [3 x i64]* %132, i64 0, i64 %131
  store i64 0, i64* %133, align 8
  %134 = load i32, i32* %60, align 4, !noalias !95
  %135 = add i32 %134, 1
  store i32 %135, i32* %60, align 4, !noalias !95
  br label %126

; <label>:136:                                    ; preds = %126
  br label %137

; <label>:137:                                    ; preds = %136
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %54, align 8, !noalias !98
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %55, align 8, !noalias !98
  %138 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !98
  %139 = load i8*, i8** %55, align 8, !noalias !98
  %140 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %138, i8* %139)
          to label %141 unwind label %311

; <label>:141:                                    ; preds = %137
  store %"class.std::__1::basic_string"* %140, %"class.std::__1::basic_string"** %53, align 8, !noalias !98
  %142 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !98
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %51, align 8, !noalias !98
  store %"class.std::__1::basic_string"* %142, %"class.std::__1::basic_string"** %52, align 8, !noalias !98
  %143 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !98
  %144 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !98
  store %"class.std::__1::basic_string"* %143, %"class.std::__1::basic_string"** %49, align 8, !noalias !98
  store %"class.std::__1::basic_string"* %144, %"class.std::__1::basic_string"** %50, align 8, !noalias !98
  %145 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !98
  %146 = bitcast %"class.std::__1::basic_string"* %145 to %"class.std::__1::__basic_string_common"*
  %147 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %145, i32 0, i32 0
  %148 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !98
  %149 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %148, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %149, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !98
  %150 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !98
  %151 = bitcast %"class.std::__1::__compressed_pair"* %147 to i8*
  %152 = bitcast %"class.std::__1::__compressed_pair"* %150 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %151, i8* %152, i64 24, i32 8, i1 false) #10
  %153 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !98
  store %"class.std::__1::basic_string"* %153, %"class.std::__1::basic_string"** %45, align 8, !noalias !98
  %154 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !98
  %155 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %154, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %155, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !98
  %156 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !98
  %157 = bitcast %"class.std::__1::__compressed_pair"* %156 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %157, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !98
  %158 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !98
  %159 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %158, i32 0, i32 0
  %160 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %159, i32 0, i32 0
  %161 = bitcast %union.anon.0* %160 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %162 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %161, i32 0, i32 0
  store [3 x i64]* %162, [3 x i64]** %46, align 8, !noalias !98
  store i32 0, i32* %47, align 4, !noalias !98
  br label %163

; <label>:163:                                    ; preds = %166, %141
  %164 = load i32, i32* %47, align 4, !noalias !98
  %165 = icmp ult i32 %164, 3
  br i1 %165, label %166, label %173

; <label>:166:                                    ; preds = %163
  %167 = load i32, i32* %47, align 4, !noalias !98
  %168 = zext i32 %167 to i64
  %169 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !98
  %170 = getelementptr inbounds [3 x i64], [3 x i64]* %169, i64 0, i64 %168
  store i64 0, i64* %170, align 8
  %171 = load i32, i32* %47, align 4, !noalias !98
  %172 = add i32 %171, 1
  store i32 %172, i32* %47, align 4, !noalias !98
  br label %163

; <label>:173:                                    ; preds = %163
  br label %174

; <label>:174:                                    ; preds = %173
  %175 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %176 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %175, i32 0, i32 0
  %177 = load i32, i32* %176, align 8
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %75, i32 %177)
          to label %178 unwind label %315

; <label>:178:                                    ; preds = %174
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %41, align 8, !noalias !101
  store %"class.std::__1::basic_string"* %75, %"class.std::__1::basic_string"** %42, align 8, !noalias !101
  %179 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !101
  %180 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !101
  store %"class.std::__1::basic_string"* %179, %"class.std::__1::basic_string"** %39, align 8, !noalias !101
  store %"class.std::__1::basic_string"* %180, %"class.std::__1::basic_string"** %40, align 8, !noalias !101
  %181 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !101
  %182 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !101
  store %"class.std::__1::basic_string"* %182, %"class.std::__1::basic_string"** %38, align 8, !noalias !101
  %183 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !101
  store %"class.std::__1::basic_string"* %183, %"class.std::__1::basic_string"** %37, align 8, !noalias !101
  %184 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !101
  store %"class.std::__1::basic_string"* %184, %"class.std::__1::basic_string"** %36, align 8, !noalias !101
  %185 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !101
  %186 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %185, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %186, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !101
  %187 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !101
  %188 = bitcast %"class.std::__1::__compressed_pair"* %187 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %188, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !101
  %189 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !101
  %190 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %189, i32 0, i32 0
  %191 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %190, i32 0, i32 0
  %192 = bitcast %union.anon.0* %191 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %193 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %192, i32 0, i32 0
  %194 = bitcast %union.anon.1* %193 to i8*
  %195 = load i8, i8* %194, align 8, !noalias !101
  %196 = zext i8 %195 to i32
  %197 = and i32 %196, 1
  %198 = icmp ne i32 %197, 0
  br i1 %198, label %199, label %210

; <label>:199:                                    ; preds = %178
  store %"class.std::__1::basic_string"* %184, %"class.std::__1::basic_string"** %28, align 8, !noalias !101
  %200 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !101
  %201 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %200, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %201, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !101
  %202 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !101
  %203 = bitcast %"class.std::__1::__compressed_pair"* %202 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %203, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !101
  %204 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !101
  %205 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %204, i32 0, i32 0
  %206 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %205, i32 0, i32 0
  %207 = bitcast %union.anon.0* %206 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %208 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %207, i32 0, i32 2
  %209 = load i8*, i8** %208, align 8
  br label %223

; <label>:210:                                    ; preds = %178
  store %"class.std::__1::basic_string"* %184, %"class.std::__1::basic_string"** %33, align 8, !noalias !101
  %211 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !101
  %212 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %211, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %212, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !101
  %213 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !101
  %214 = bitcast %"class.std::__1::__compressed_pair"* %213 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %214, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !101
  %215 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !101
  %216 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %215, i32 0, i32 0
  %217 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %216, i32 0, i32 0
  %218 = bitcast %union.anon.0* %217 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %219 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %218, i32 0, i32 1
  %220 = getelementptr inbounds [23 x i8], [23 x i8]* %219, i64 0, i64 0
  store i8* %220, i8** %30, align 8, !noalias !101
  %221 = load i8*, i8** %30, align 8, !noalias !101
  store i8* %221, i8** %29, align 8, !noalias !101
  %222 = load i8*, i8** %29, align 8, !noalias !101
  br label %223

; <label>:223:                                    ; preds = %210, %199
  %224 = phi i8* [ %209, %199 ], [ %222, %210 ]
  store i8* %224, i8** %25, align 8, !noalias !101
  %225 = load i8*, i8** %25, align 8, !noalias !101
  %226 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !101
  store %"class.std::__1::basic_string"* %226, %"class.std::__1::basic_string"** %24, align 8, !noalias !101
  %227 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !101
  store %"class.std::__1::basic_string"* %227, %"class.std::__1::basic_string"** %23, align 8, !noalias !101
  %228 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !101
  %229 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %228, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %229, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !101
  %230 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !101
  %231 = bitcast %"class.std::__1::__compressed_pair"* %230 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %231, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !101
  %232 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !101
  %233 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %232, i32 0, i32 0
  %234 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %233, i32 0, i32 0
  %235 = bitcast %union.anon.0* %234 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %236 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %235, i32 0, i32 0
  %237 = bitcast %union.anon.1* %236 to i8*
  %238 = load i8, i8* %237, align 8, !noalias !101
  %239 = zext i8 %238 to i32
  %240 = and i32 %239, 1
  %241 = icmp ne i32 %240, 0
  br i1 %241, label %242, label %253

; <label>:242:                                    ; preds = %223
  store %"class.std::__1::basic_string"* %227, %"class.std::__1::basic_string"** %17, align 8, !noalias !101
  %243 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !101
  %244 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %243, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %244, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !101
  %245 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !101
  %246 = bitcast %"class.std::__1::__compressed_pair"* %245 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %246, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !101
  %247 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !101
  %248 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %247, i32 0, i32 0
  %249 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %248, i32 0, i32 0
  %250 = bitcast %union.anon.0* %249 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %251 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %250, i32 0, i32 1
  %252 = load i64, i64* %251, align 8
  br label %268

; <label>:253:                                    ; preds = %223
  store %"class.std::__1::basic_string"* %227, %"class.std::__1::basic_string"** %20, align 8, !noalias !101
  %254 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !101
  %255 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %254, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %255, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !101
  %256 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !101
  %257 = bitcast %"class.std::__1::__compressed_pair"* %256 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %257, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !101
  %258 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !101
  %259 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %258, i32 0, i32 0
  %260 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %259, i32 0, i32 0
  %261 = bitcast %union.anon.0* %260 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %262 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %261, i32 0, i32 0
  %263 = bitcast %union.anon.1* %262 to i8*
  %264 = load i8, i8* %263, align 8
  %265 = zext i8 %264 to i32
  %266 = ashr i32 %265, 1
  %267 = sext i32 %266 to i64
  br label %268

; <label>:268:                                    ; preds = %253, %242
  %269 = phi i64 [ %252, %242 ], [ %267, %253 ]
  %270 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %181, i8* %225, i64 %269)
          to label %271 unwind label %319

; <label>:271:                                    ; preds = %268
  store %"class.std::__1::basic_string"* %270, %"class.std::__1::basic_string"** %14, align 8, !noalias !101
  %272 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !101
  store %"class.std::__1::basic_string"* %69, %"class.std::__1::basic_string"** %12, align 8, !noalias !101
  store %"class.std::__1::basic_string"* %272, %"class.std::__1::basic_string"** %13, align 8, !noalias !101
  %273 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !101
  %274 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !101
  store %"class.std::__1::basic_string"* %273, %"class.std::__1::basic_string"** %10, align 8, !noalias !101
  store %"class.std::__1::basic_string"* %274, %"class.std::__1::basic_string"** %11, align 8, !noalias !101
  %275 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !101
  %276 = bitcast %"class.std::__1::basic_string"* %275 to %"class.std::__1::__basic_string_common"*
  %277 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %275, i32 0, i32 0
  %278 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !101
  %279 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %278, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %279, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !101
  %280 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !101
  %281 = bitcast %"class.std::__1::__compressed_pair"* %277 to i8*
  %282 = bitcast %"class.std::__1::__compressed_pair"* %280 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %281, i8* %282, i64 24, i32 8, i1 false) #10
  %283 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !101
  store %"class.std::__1::basic_string"* %283, %"class.std::__1::basic_string"** %6, align 8, !noalias !101
  %284 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !101
  %285 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %284, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %285, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !101
  %286 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !101
  %287 = bitcast %"class.std::__1::__compressed_pair"* %286 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %287, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !101
  %288 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !101
  %289 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %288, i32 0, i32 0
  %290 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %289, i32 0, i32 0
  %291 = bitcast %union.anon.0* %290 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %292 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %291, i32 0, i32 0
  store [3 x i64]* %292, [3 x i64]** %7, align 8, !noalias !101
  store i32 0, i32* %8, align 4, !noalias !101
  br label %293

; <label>:293:                                    ; preds = %296, %271
  %294 = load i32, i32* %8, align 4, !noalias !101
  %295 = icmp ult i32 %294, 3
  br i1 %295, label %296, label %303

; <label>:296:                                    ; preds = %293
  %297 = load i32, i32* %8, align 4, !noalias !101
  %298 = zext i32 %297 to i64
  %299 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !101
  %300 = getelementptr inbounds [3 x i64], [3 x i64]* %299, i64 0, i64 %298
  store i64 0, i64* %300, align 8
  %301 = load i32, i32* %8, align 4, !noalias !101
  %302 = add i32 %301, 1
  store i32 %302, i32* %8, align 4, !noalias !101
  br label %293

; <label>:303:                                    ; preds = %293
  br label %304

; <label>:304:                                    ; preds = %303
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %69, %class.AstNode* null)
          to label %305 unwind label %323

; <label>:305:                                    ; preds = %304
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %306

; <label>:306:                                    ; preds = %305, %95
  ret void

; <label>:307:                                    ; preds = %97
  %308 = landingpad { i8*, i32 }
          cleanup
  %309 = extractvalue { i8*, i32 } %308, 0
  store i8* %309, i8** %73, align 8
  %310 = extractvalue { i8*, i32 } %308, 1
  store i32 %310, i32* %74, align 4
  br label %330

; <label>:311:                                    ; preds = %137
  %312 = landingpad { i8*, i32 }
          cleanup
  %313 = extractvalue { i8*, i32 } %312, 0
  store i8* %313, i8** %73, align 8
  %314 = extractvalue { i8*, i32 } %312, 1
  store i32 %314, i32* %74, align 4
  br label %329

; <label>:315:                                    ; preds = %174
  %316 = landingpad { i8*, i32 }
          cleanup
  %317 = extractvalue { i8*, i32 } %316, 0
  store i8* %317, i8** %73, align 8
  %318 = extractvalue { i8*, i32 } %316, 1
  store i32 %318, i32* %74, align 4
  br label %328

; <label>:319:                                    ; preds = %268
  %320 = landingpad { i8*, i32 }
          cleanup
  %321 = extractvalue { i8*, i32 } %320, 0
  store i8* %321, i8** %73, align 8
  %322 = extractvalue { i8*, i32 } %320, 1
  store i32 %322, i32* %74, align 4
  br label %327

; <label>:323:                                    ; preds = %304
  %324 = landingpad { i8*, i32 }
          cleanup
  %325 = extractvalue { i8*, i32 } %324, 0
  store i8* %325, i8** %73, align 8
  %326 = extractvalue { i8*, i32 } %324, 1
  store i32 %326, i32* %74, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  br label %327

; <label>:327:                                    ; preds = %323, %319
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  br label %328

; <label>:328:                                    ; preds = %327, %315
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  br label %329

; <label>:329:                                    ; preds = %328, %311
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  br label %330

; <label>:330:                                    ; preds = %329, %307
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %331

; <label>:331:                                    ; preds = %330
  %332 = load i8*, i8** %73, align 8
  %333 = load i32, i32* %74, align 4
  %334 = insertvalue { i8*, i32 } undef, i8* %332, 0
  %335 = insertvalue { i8*, i32 } %334, i32 %333, 1
  resume { i8*, i32 } %335
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic11bitwise_andE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  %69 = alloca %"class.std::__1::basic_string", align 8
  %70 = alloca %"class.std::__1::basic_string", align 8
  %71 = alloca %"class.std::__1::basic_string", align 8
  %72 = alloca %"class.std::__1::basic_string", align 8
  %73 = alloca i8*
  %74 = alloca i32
  %75 = alloca %"class.std::__1::basic_string", align 8
  %76 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %77 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %76, i32 0, i32 0
  %78 = load i32, i32* %77, align 8
  switch i32 %78, label %257 [
    i32 0, label %79
    i32 1, label %121
    i32 6, label %163
    i32 5, label %208
  ]

; <label>:79:                                     ; preds = %3
  %80 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %81 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %80, i32 0, i32 0
  %82 = load i32, i32* %81, align 8
  switch i32 %82, label %119 [
    i32 0, label %83
    i32 1, label %91
    i32 6, label %100
    i32 5, label %109
  ]

; <label>:83:                                     ; preds = %79
  %84 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %85 = bitcast %union.anon* %84 to i32*
  %86 = load i32, i32* %85, align 8
  %87 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %88 = bitcast %union.anon* %87 to i32*
  %89 = load i32, i32* %88, align 8
  %90 = and i32 %86, %89
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %90)
  br label %467

; <label>:91:                                     ; preds = %79
  %92 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %93 = bitcast %union.anon* %92 to i32*
  %94 = load i32, i32* %93, align 8
  %95 = sext i32 %94 to i64
  %96 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %97 = bitcast %union.anon* %96 to i64*
  %98 = load i64, i64* %97, align 8
  %99 = and i64 %95, %98
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %99)
  br label %467

; <label>:100:                                    ; preds = %79
  %101 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %102 = bitcast %union.anon* %101 to i32*
  %103 = load i32, i32* %102, align 8
  %104 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %105 = bitcast %union.anon* %104 to i8*
  %106 = load i8, i8* %105, align 8
  %107 = sext i8 %106 to i32
  %108 = and i32 %103, %107
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %108)
  br label %467

; <label>:109:                                    ; preds = %79
  %110 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %111 = bitcast %union.anon* %110 to i32*
  %112 = load i32, i32* %111, align 8
  %113 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %114 = bitcast %union.anon* %113 to i8*
  %115 = load i8, i8* %114, align 8
  %116 = trunc i8 %115 to i1
  %117 = zext i1 %116 to i32
  %118 = and i32 %112, %117
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %118)
  br label %467

; <label>:119:                                    ; preds = %79
  br label %120

; <label>:120:                                    ; preds = %119
  br label %258

; <label>:121:                                    ; preds = %3
  %122 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %123 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %122, i32 0, i32 0
  %124 = load i32, i32* %123, align 8
  switch i32 %124, label %161 [
    i32 0, label %125
    i32 1, label %134
    i32 6, label %142
    i32 5, label %151
  ]

; <label>:125:                                    ; preds = %121
  %126 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %127 = bitcast %union.anon* %126 to i64*
  %128 = load i64, i64* %127, align 8
  %129 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %130 = bitcast %union.anon* %129 to i32*
  %131 = load i32, i32* %130, align 8
  %132 = sext i32 %131 to i64
  %133 = and i64 %128, %132
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %133)
  br label %467

; <label>:134:                                    ; preds = %121
  %135 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %136 = bitcast %union.anon* %135 to i64*
  %137 = load i64, i64* %136, align 8
  %138 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %139 = bitcast %union.anon* %138 to i64*
  %140 = load i64, i64* %139, align 8
  %141 = and i64 %137, %140
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %141)
  br label %467

; <label>:142:                                    ; preds = %121
  %143 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %144 = bitcast %union.anon* %143 to i64*
  %145 = load i64, i64* %144, align 8
  %146 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %147 = bitcast %union.anon* %146 to i8*
  %148 = load i8, i8* %147, align 8
  %149 = sext i8 %148 to i64
  %150 = and i64 %145, %149
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %150)
  br label %467

; <label>:151:                                    ; preds = %121
  %152 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %153 = bitcast %union.anon* %152 to i64*
  %154 = load i64, i64* %153, align 8
  %155 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %156 = bitcast %union.anon* %155 to i8*
  %157 = load i8, i8* %156, align 8
  %158 = trunc i8 %157 to i1
  %159 = zext i1 %158 to i64
  %160 = and i64 %154, %159
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %160)
  br label %467

; <label>:161:                                    ; preds = %121
  br label %162

; <label>:162:                                    ; preds = %161
  br label %258

; <label>:163:                                    ; preds = %3
  %164 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %165 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %164, i32 0, i32 0
  %166 = load i32, i32* %165, align 8
  switch i32 %166, label %206 [
    i32 0, label %167
    i32 1, label %176
    i32 6, label %185
    i32 5, label %195
  ]

; <label>:167:                                    ; preds = %163
  %168 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %169 = bitcast %union.anon* %168 to i8*
  %170 = load i8, i8* %169, align 8
  %171 = sext i8 %170 to i32
  %172 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %173 = bitcast %union.anon* %172 to i32*
  %174 = load i32, i32* %173, align 8
  %175 = and i32 %171, %174
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %175)
  br label %467

; <label>:176:                                    ; preds = %163
  %177 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %178 = bitcast %union.anon* %177 to i8*
  %179 = load i8, i8* %178, align 8
  %180 = sext i8 %179 to i64
  %181 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %182 = bitcast %union.anon* %181 to i64*
  %183 = load i64, i64* %182, align 8
  %184 = and i64 %180, %183
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %184)
  br label %467

; <label>:185:                                    ; preds = %163
  %186 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %187 = bitcast %union.anon* %186 to i8*
  %188 = load i8, i8* %187, align 8
  %189 = sext i8 %188 to i32
  %190 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %191 = bitcast %union.anon* %190 to i8*
  %192 = load i8, i8* %191, align 8
  %193 = sext i8 %192 to i32
  %194 = and i32 %189, %193
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %194)
  br label %467

; <label>:195:                                    ; preds = %163
  %196 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %197 = bitcast %union.anon* %196 to i8*
  %198 = load i8, i8* %197, align 8
  %199 = sext i8 %198 to i32
  %200 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %201 = bitcast %union.anon* %200 to i8*
  %202 = load i8, i8* %201, align 8
  %203 = trunc i8 %202 to i1
  %204 = zext i1 %203 to i32
  %205 = and i32 %199, %204
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %205)
  br label %467

; <label>:206:                                    ; preds = %163
  br label %207

; <label>:207:                                    ; preds = %206
  br label %258

; <label>:208:                                    ; preds = %3
  %209 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %210 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %209, i32 0, i32 0
  %211 = load i32, i32* %210, align 8
  switch i32 %211, label %255 [
    i32 0, label %212
    i32 1, label %222
    i32 6, label %232
    i32 5, label %243
  ]

; <label>:212:                                    ; preds = %208
  %213 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %214 = bitcast %union.anon* %213 to i8*
  %215 = load i8, i8* %214, align 8
  %216 = trunc i8 %215 to i1
  %217 = zext i1 %216 to i32
  %218 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %219 = bitcast %union.anon* %218 to i32*
  %220 = load i32, i32* %219, align 8
  %221 = and i32 %217, %220
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %221)
  br label %467

; <label>:222:                                    ; preds = %208
  %223 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %224 = bitcast %union.anon* %223 to i8*
  %225 = load i8, i8* %224, align 8
  %226 = trunc i8 %225 to i1
  %227 = zext i1 %226 to i64
  %228 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %229 = bitcast %union.anon* %228 to i64*
  %230 = load i64, i64* %229, align 8
  %231 = and i64 %227, %230
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %231)
  br label %467

; <label>:232:                                    ; preds = %208
  %233 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %234 = bitcast %union.anon* %233 to i8*
  %235 = load i8, i8* %234, align 8
  %236 = trunc i8 %235 to i1
  %237 = zext i1 %236 to i32
  %238 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %239 = bitcast %union.anon* %238 to i8*
  %240 = load i8, i8* %239, align 8
  %241 = sext i8 %240 to i32
  %242 = and i32 %237, %241
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %242)
  br label %467

; <label>:243:                                    ; preds = %208
  %244 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %245 = bitcast %union.anon* %244 to i8*
  %246 = load i8, i8* %245, align 8
  %247 = trunc i8 %246 to i1
  %248 = zext i1 %247 to i32
  %249 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %250 = bitcast %union.anon* %249 to i8*
  %251 = load i8, i8* %250, align 8
  %252 = trunc i8 %251 to i1
  %253 = zext i1 %252 to i32
  %254 = and i32 %248, %253
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %254)
  br label %467

; <label>:255:                                    ; preds = %208
  br label %256

; <label>:256:                                    ; preds = %255
  br label %258

; <label>:257:                                    ; preds = %3
  br label %258

; <label>:258:                                    ; preds = %257, %256, %207, %162, %120
  %259 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %260 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %259, i32 0, i32 0
  %261 = load i32, i32* %260, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %72, i32 %261)
  store i8* getelementptr inbounds ([44 x i8], [44 x i8]* @.str.9, i32 0, i32 0), i8** %67, align 8, !noalias !104
  store %"class.std::__1::basic_string"* %72, %"class.std::__1::basic_string"** %68, align 8, !noalias !104
  %262 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !104
  %263 = load i8*, i8** %67, align 8, !noalias !104
  %264 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %262, i64 0, i8* %263)
          to label %265 unwind label %468

; <label>:265:                                    ; preds = %258
  store %"class.std::__1::basic_string"* %264, %"class.std::__1::basic_string"** %66, align 8, !noalias !104
  %266 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !104
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %64, align 8, !noalias !104
  store %"class.std::__1::basic_string"* %266, %"class.std::__1::basic_string"** %65, align 8, !noalias !104
  %267 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !104
  %268 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !104
  store %"class.std::__1::basic_string"* %267, %"class.std::__1::basic_string"** %62, align 8, !noalias !104
  store %"class.std::__1::basic_string"* %268, %"class.std::__1::basic_string"** %63, align 8, !noalias !104
  %269 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8, !noalias !104
  %270 = bitcast %"class.std::__1::basic_string"* %269 to %"class.std::__1::__basic_string_common"*
  %271 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %269, i32 0, i32 0
  %272 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !104
  %273 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %272, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %273, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !104
  %274 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !104
  %275 = bitcast %"class.std::__1::__compressed_pair"* %271 to i8*
  %276 = bitcast %"class.std::__1::__compressed_pair"* %274 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %275, i8* %276, i64 24, i32 8, i1 false) #10
  %277 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !104
  store %"class.std::__1::basic_string"* %277, %"class.std::__1::basic_string"** %58, align 8, !noalias !104
  %278 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8, !noalias !104
  %279 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %278, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %279, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !104
  %280 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !104
  %281 = bitcast %"class.std::__1::__compressed_pair"* %280 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %281, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !104
  %282 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !104
  %283 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %282, i32 0, i32 0
  %284 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %283, i32 0, i32 0
  %285 = bitcast %union.anon.0* %284 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %286 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %285, i32 0, i32 0
  store [3 x i64]* %286, [3 x i64]** %59, align 8, !noalias !104
  store i32 0, i32* %60, align 4, !noalias !104
  br label %287

; <label>:287:                                    ; preds = %290, %265
  %288 = load i32, i32* %60, align 4, !noalias !104
  %289 = icmp ult i32 %288, 3
  br i1 %289, label %290, label %297

; <label>:290:                                    ; preds = %287
  %291 = load i32, i32* %60, align 4, !noalias !104
  %292 = zext i32 %291 to i64
  %293 = load [3 x i64]*, [3 x i64]** %59, align 8, !noalias !104
  %294 = getelementptr inbounds [3 x i64], [3 x i64]* %293, i64 0, i64 %292
  store i64 0, i64* %294, align 8
  %295 = load i32, i32* %60, align 4, !noalias !104
  %296 = add i32 %295, 1
  store i32 %296, i32* %60, align 4, !noalias !104
  br label %287

; <label>:297:                                    ; preds = %287
  br label %298

; <label>:298:                                    ; preds = %297
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %54, align 8, !noalias !107
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %55, align 8, !noalias !107
  %299 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !107
  %300 = load i8*, i8** %55, align 8, !noalias !107
  %301 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %299, i8* %300)
          to label %302 unwind label %472

; <label>:302:                                    ; preds = %298
  store %"class.std::__1::basic_string"* %301, %"class.std::__1::basic_string"** %53, align 8, !noalias !107
  %303 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !107
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %51, align 8, !noalias !107
  store %"class.std::__1::basic_string"* %303, %"class.std::__1::basic_string"** %52, align 8, !noalias !107
  %304 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !107
  %305 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !107
  store %"class.std::__1::basic_string"* %304, %"class.std::__1::basic_string"** %49, align 8, !noalias !107
  store %"class.std::__1::basic_string"* %305, %"class.std::__1::basic_string"** %50, align 8, !noalias !107
  %306 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !107
  %307 = bitcast %"class.std::__1::basic_string"* %306 to %"class.std::__1::__basic_string_common"*
  %308 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %306, i32 0, i32 0
  %309 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !107
  %310 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %309, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %310, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !107
  %311 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !107
  %312 = bitcast %"class.std::__1::__compressed_pair"* %308 to i8*
  %313 = bitcast %"class.std::__1::__compressed_pair"* %311 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %312, i8* %313, i64 24, i32 8, i1 false) #10
  %314 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !107
  store %"class.std::__1::basic_string"* %314, %"class.std::__1::basic_string"** %45, align 8, !noalias !107
  %315 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !107
  %316 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %315, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %316, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !107
  %317 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !107
  %318 = bitcast %"class.std::__1::__compressed_pair"* %317 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %318, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !107
  %319 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !107
  %320 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %319, i32 0, i32 0
  %321 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %320, i32 0, i32 0
  %322 = bitcast %union.anon.0* %321 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %323 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %322, i32 0, i32 0
  store [3 x i64]* %323, [3 x i64]** %46, align 8, !noalias !107
  store i32 0, i32* %47, align 4, !noalias !107
  br label %324

; <label>:324:                                    ; preds = %327, %302
  %325 = load i32, i32* %47, align 4, !noalias !107
  %326 = icmp ult i32 %325, 3
  br i1 %326, label %327, label %334

; <label>:327:                                    ; preds = %324
  %328 = load i32, i32* %47, align 4, !noalias !107
  %329 = zext i32 %328 to i64
  %330 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !107
  %331 = getelementptr inbounds [3 x i64], [3 x i64]* %330, i64 0, i64 %329
  store i64 0, i64* %331, align 8
  %332 = load i32, i32* %47, align 4, !noalias !107
  %333 = add i32 %332, 1
  store i32 %333, i32* %47, align 4, !noalias !107
  br label %324

; <label>:334:                                    ; preds = %324
  br label %335

; <label>:335:                                    ; preds = %334
  %336 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %337 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %336, i32 0, i32 0
  %338 = load i32, i32* %337, align 8
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %75, i32 %338)
          to label %339 unwind label %476

; <label>:339:                                    ; preds = %335
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %41, align 8, !noalias !110
  store %"class.std::__1::basic_string"* %75, %"class.std::__1::basic_string"** %42, align 8, !noalias !110
  %340 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !110
  %341 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !110
  store %"class.std::__1::basic_string"* %340, %"class.std::__1::basic_string"** %39, align 8, !noalias !110
  store %"class.std::__1::basic_string"* %341, %"class.std::__1::basic_string"** %40, align 8, !noalias !110
  %342 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !110
  %343 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !110
  store %"class.std::__1::basic_string"* %343, %"class.std::__1::basic_string"** %38, align 8, !noalias !110
  %344 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !110
  store %"class.std::__1::basic_string"* %344, %"class.std::__1::basic_string"** %37, align 8, !noalias !110
  %345 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !110
  store %"class.std::__1::basic_string"* %345, %"class.std::__1::basic_string"** %36, align 8, !noalias !110
  %346 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !110
  %347 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %346, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %347, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !110
  %348 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !110
  %349 = bitcast %"class.std::__1::__compressed_pair"* %348 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %349, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !110
  %350 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !110
  %351 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %350, i32 0, i32 0
  %352 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %351, i32 0, i32 0
  %353 = bitcast %union.anon.0* %352 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %354 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %353, i32 0, i32 0
  %355 = bitcast %union.anon.1* %354 to i8*
  %356 = load i8, i8* %355, align 8, !noalias !110
  %357 = zext i8 %356 to i32
  %358 = and i32 %357, 1
  %359 = icmp ne i32 %358, 0
  br i1 %359, label %360, label %371

; <label>:360:                                    ; preds = %339
  store %"class.std::__1::basic_string"* %345, %"class.std::__1::basic_string"** %28, align 8, !noalias !110
  %361 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !110
  %362 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %361, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %362, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !110
  %363 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !110
  %364 = bitcast %"class.std::__1::__compressed_pair"* %363 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %364, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !110
  %365 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !110
  %366 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %365, i32 0, i32 0
  %367 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %366, i32 0, i32 0
  %368 = bitcast %union.anon.0* %367 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %369 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %368, i32 0, i32 2
  %370 = load i8*, i8** %369, align 8
  br label %384

; <label>:371:                                    ; preds = %339
  store %"class.std::__1::basic_string"* %345, %"class.std::__1::basic_string"** %33, align 8, !noalias !110
  %372 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !110
  %373 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %372, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %373, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !110
  %374 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !110
  %375 = bitcast %"class.std::__1::__compressed_pair"* %374 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %375, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !110
  %376 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !110
  %377 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %376, i32 0, i32 0
  %378 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %377, i32 0, i32 0
  %379 = bitcast %union.anon.0* %378 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %380 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %379, i32 0, i32 1
  %381 = getelementptr inbounds [23 x i8], [23 x i8]* %380, i64 0, i64 0
  store i8* %381, i8** %30, align 8, !noalias !110
  %382 = load i8*, i8** %30, align 8, !noalias !110
  store i8* %382, i8** %29, align 8, !noalias !110
  %383 = load i8*, i8** %29, align 8, !noalias !110
  br label %384

; <label>:384:                                    ; preds = %371, %360
  %385 = phi i8* [ %370, %360 ], [ %383, %371 ]
  store i8* %385, i8** %25, align 8, !noalias !110
  %386 = load i8*, i8** %25, align 8, !noalias !110
  %387 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !110
  store %"class.std::__1::basic_string"* %387, %"class.std::__1::basic_string"** %24, align 8, !noalias !110
  %388 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !110
  store %"class.std::__1::basic_string"* %388, %"class.std::__1::basic_string"** %23, align 8, !noalias !110
  %389 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !110
  %390 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %389, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %390, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !110
  %391 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !110
  %392 = bitcast %"class.std::__1::__compressed_pair"* %391 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %392, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !110
  %393 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !110
  %394 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %393, i32 0, i32 0
  %395 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %394, i32 0, i32 0
  %396 = bitcast %union.anon.0* %395 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %397 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %396, i32 0, i32 0
  %398 = bitcast %union.anon.1* %397 to i8*
  %399 = load i8, i8* %398, align 8, !noalias !110
  %400 = zext i8 %399 to i32
  %401 = and i32 %400, 1
  %402 = icmp ne i32 %401, 0
  br i1 %402, label %403, label %414

; <label>:403:                                    ; preds = %384
  store %"class.std::__1::basic_string"* %388, %"class.std::__1::basic_string"** %17, align 8, !noalias !110
  %404 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !110
  %405 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %404, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %405, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !110
  %406 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !110
  %407 = bitcast %"class.std::__1::__compressed_pair"* %406 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %407, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !110
  %408 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !110
  %409 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %408, i32 0, i32 0
  %410 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %409, i32 0, i32 0
  %411 = bitcast %union.anon.0* %410 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %412 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %411, i32 0, i32 1
  %413 = load i64, i64* %412, align 8
  br label %429

; <label>:414:                                    ; preds = %384
  store %"class.std::__1::basic_string"* %388, %"class.std::__1::basic_string"** %20, align 8, !noalias !110
  %415 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !110
  %416 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %415, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %416, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !110
  %417 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !110
  %418 = bitcast %"class.std::__1::__compressed_pair"* %417 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %418, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !110
  %419 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !110
  %420 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %419, i32 0, i32 0
  %421 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %420, i32 0, i32 0
  %422 = bitcast %union.anon.0* %421 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %423 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %422, i32 0, i32 0
  %424 = bitcast %union.anon.1* %423 to i8*
  %425 = load i8, i8* %424, align 8
  %426 = zext i8 %425 to i32
  %427 = ashr i32 %426, 1
  %428 = sext i32 %427 to i64
  br label %429

; <label>:429:                                    ; preds = %414, %403
  %430 = phi i64 [ %413, %403 ], [ %428, %414 ]
  %431 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %342, i8* %386, i64 %430)
          to label %432 unwind label %480

; <label>:432:                                    ; preds = %429
  store %"class.std::__1::basic_string"* %431, %"class.std::__1::basic_string"** %14, align 8, !noalias !110
  %433 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !110
  store %"class.std::__1::basic_string"* %69, %"class.std::__1::basic_string"** %12, align 8, !noalias !110
  store %"class.std::__1::basic_string"* %433, %"class.std::__1::basic_string"** %13, align 8, !noalias !110
  %434 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !110
  %435 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !110
  store %"class.std::__1::basic_string"* %434, %"class.std::__1::basic_string"** %10, align 8, !noalias !110
  store %"class.std::__1::basic_string"* %435, %"class.std::__1::basic_string"** %11, align 8, !noalias !110
  %436 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !110
  %437 = bitcast %"class.std::__1::basic_string"* %436 to %"class.std::__1::__basic_string_common"*
  %438 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %436, i32 0, i32 0
  %439 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !110
  %440 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %439, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %440, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !110
  %441 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !110
  %442 = bitcast %"class.std::__1::__compressed_pair"* %438 to i8*
  %443 = bitcast %"class.std::__1::__compressed_pair"* %441 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %442, i8* %443, i64 24, i32 8, i1 false) #10
  %444 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !110
  store %"class.std::__1::basic_string"* %444, %"class.std::__1::basic_string"** %6, align 8, !noalias !110
  %445 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !110
  %446 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %445, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %446, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !110
  %447 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !110
  %448 = bitcast %"class.std::__1::__compressed_pair"* %447 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %448, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !110
  %449 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !110
  %450 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %449, i32 0, i32 0
  %451 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %450, i32 0, i32 0
  %452 = bitcast %union.anon.0* %451 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %453 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %452, i32 0, i32 0
  store [3 x i64]* %453, [3 x i64]** %7, align 8, !noalias !110
  store i32 0, i32* %8, align 4, !noalias !110
  br label %454

; <label>:454:                                    ; preds = %457, %432
  %455 = load i32, i32* %8, align 4, !noalias !110
  %456 = icmp ult i32 %455, 3
  br i1 %456, label %457, label %464

; <label>:457:                                    ; preds = %454
  %458 = load i32, i32* %8, align 4, !noalias !110
  %459 = zext i32 %458 to i64
  %460 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !110
  %461 = getelementptr inbounds [3 x i64], [3 x i64]* %460, i64 0, i64 %459
  store i64 0, i64* %461, align 8
  %462 = load i32, i32* %8, align 4, !noalias !110
  %463 = add i32 %462, 1
  store i32 %463, i32* %8, align 4, !noalias !110
  br label %454

; <label>:464:                                    ; preds = %454
  br label %465

; <label>:465:                                    ; preds = %464
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %69, %class.AstNode* null)
          to label %466 unwind label %484

; <label>:466:                                    ; preds = %465
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %467

; <label>:467:                                    ; preds = %466, %243, %232, %222, %212, %195, %185, %176, %167, %151, %142, %134, %125, %109, %100, %91, %83
  ret void

; <label>:468:                                    ; preds = %258
  %469 = landingpad { i8*, i32 }
          cleanup
  %470 = extractvalue { i8*, i32 } %469, 0
  store i8* %470, i8** %73, align 8
  %471 = extractvalue { i8*, i32 } %469, 1
  store i32 %471, i32* %74, align 4
  br label %491

; <label>:472:                                    ; preds = %298
  %473 = landingpad { i8*, i32 }
          cleanup
  %474 = extractvalue { i8*, i32 } %473, 0
  store i8* %474, i8** %73, align 8
  %475 = extractvalue { i8*, i32 } %473, 1
  store i32 %475, i32* %74, align 4
  br label %490

; <label>:476:                                    ; preds = %335
  %477 = landingpad { i8*, i32 }
          cleanup
  %478 = extractvalue { i8*, i32 } %477, 0
  store i8* %478, i8** %73, align 8
  %479 = extractvalue { i8*, i32 } %477, 1
  store i32 %479, i32* %74, align 4
  br label %489

; <label>:480:                                    ; preds = %429
  %481 = landingpad { i8*, i32 }
          cleanup
  %482 = extractvalue { i8*, i32 } %481, 0
  store i8* %482, i8** %73, align 8
  %483 = extractvalue { i8*, i32 } %481, 1
  store i32 %483, i32* %74, align 4
  br label %488

; <label>:484:                                    ; preds = %465
  %485 = landingpad { i8*, i32 }
          cleanup
  %486 = extractvalue { i8*, i32 } %485, 0
  store i8* %486, i8** %73, align 8
  %487 = extractvalue { i8*, i32 } %485, 1
  store i32 %487, i32* %74, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  br label %488

; <label>:488:                                    ; preds = %484, %480
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  br label %489

; <label>:489:                                    ; preds = %488, %476
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  br label %490

; <label>:490:                                    ; preds = %489, %472
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  br label %491

; <label>:491:                                    ; preds = %490, %468
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %492

; <label>:492:                                    ; preds = %491
  %493 = load i8*, i8** %73, align 8
  %494 = load i32, i32* %74, align 4
  %495 = insertvalue { i8*, i32 } undef, i8* %493, 0
  %496 = insertvalue { i8*, i32 } %495, i32 %494, 1
  resume { i8*, i32 } %496
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic10bitwise_orE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  %69 = alloca %"class.std::__1::basic_string", align 8
  %70 = alloca %"class.std::__1::basic_string", align 8
  %71 = alloca %"class.std::__1::basic_string", align 8
  %72 = alloca %"class.std::__1::basic_string", align 8
  %73 = alloca i8*
  %74 = alloca i32
  %75 = alloca %"class.std::__1::basic_string", align 8
  %76 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %77 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %76, i32 0, i32 0
  %78 = load i32, i32* %77, align 8
  switch i32 %78, label %257 [
    i32 0, label %79
    i32 1, label %121
    i32 6, label %163
    i32 5, label %208
  ]

; <label>:79:                                     ; preds = %3
  %80 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %81 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %80, i32 0, i32 0
  %82 = load i32, i32* %81, align 8
  switch i32 %82, label %119 [
    i32 0, label %83
    i32 1, label %91
    i32 6, label %100
    i32 5, label %109
  ]

; <label>:83:                                     ; preds = %79
  %84 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %85 = bitcast %union.anon* %84 to i32*
  %86 = load i32, i32* %85, align 8
  %87 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %88 = bitcast %union.anon* %87 to i32*
  %89 = load i32, i32* %88, align 8
  %90 = or i32 %86, %89
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %90)
  br label %467

; <label>:91:                                     ; preds = %79
  %92 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %93 = bitcast %union.anon* %92 to i32*
  %94 = load i32, i32* %93, align 8
  %95 = sext i32 %94 to i64
  %96 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %97 = bitcast %union.anon* %96 to i64*
  %98 = load i64, i64* %97, align 8
  %99 = or i64 %95, %98
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %99)
  br label %467

; <label>:100:                                    ; preds = %79
  %101 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %102 = bitcast %union.anon* %101 to i32*
  %103 = load i32, i32* %102, align 8
  %104 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %105 = bitcast %union.anon* %104 to i8*
  %106 = load i8, i8* %105, align 8
  %107 = sext i8 %106 to i32
  %108 = or i32 %103, %107
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %108)
  br label %467

; <label>:109:                                    ; preds = %79
  %110 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %111 = bitcast %union.anon* %110 to i32*
  %112 = load i32, i32* %111, align 8
  %113 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %114 = bitcast %union.anon* %113 to i8*
  %115 = load i8, i8* %114, align 8
  %116 = trunc i8 %115 to i1
  %117 = zext i1 %116 to i32
  %118 = or i32 %112, %117
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %118)
  br label %467

; <label>:119:                                    ; preds = %79
  br label %120

; <label>:120:                                    ; preds = %119
  br label %258

; <label>:121:                                    ; preds = %3
  %122 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %123 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %122, i32 0, i32 0
  %124 = load i32, i32* %123, align 8
  switch i32 %124, label %161 [
    i32 0, label %125
    i32 1, label %134
    i32 6, label %142
    i32 5, label %151
  ]

; <label>:125:                                    ; preds = %121
  %126 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %127 = bitcast %union.anon* %126 to i64*
  %128 = load i64, i64* %127, align 8
  %129 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %130 = bitcast %union.anon* %129 to i32*
  %131 = load i32, i32* %130, align 8
  %132 = sext i32 %131 to i64
  %133 = or i64 %128, %132
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %133)
  br label %467

; <label>:134:                                    ; preds = %121
  %135 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %136 = bitcast %union.anon* %135 to i64*
  %137 = load i64, i64* %136, align 8
  %138 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %139 = bitcast %union.anon* %138 to i64*
  %140 = load i64, i64* %139, align 8
  %141 = or i64 %137, %140
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %141)
  br label %467

; <label>:142:                                    ; preds = %121
  %143 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %144 = bitcast %union.anon* %143 to i64*
  %145 = load i64, i64* %144, align 8
  %146 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %147 = bitcast %union.anon* %146 to i8*
  %148 = load i8, i8* %147, align 8
  %149 = sext i8 %148 to i64
  %150 = or i64 %145, %149
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %150)
  br label %467

; <label>:151:                                    ; preds = %121
  %152 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %153 = bitcast %union.anon* %152 to i64*
  %154 = load i64, i64* %153, align 8
  %155 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %156 = bitcast %union.anon* %155 to i8*
  %157 = load i8, i8* %156, align 8
  %158 = trunc i8 %157 to i1
  %159 = zext i1 %158 to i64
  %160 = or i64 %154, %159
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %160)
  br label %467

; <label>:161:                                    ; preds = %121
  br label %162

; <label>:162:                                    ; preds = %161
  br label %258

; <label>:163:                                    ; preds = %3
  %164 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %165 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %164, i32 0, i32 0
  %166 = load i32, i32* %165, align 8
  switch i32 %166, label %206 [
    i32 0, label %167
    i32 1, label %176
    i32 6, label %185
    i32 5, label %195
  ]

; <label>:167:                                    ; preds = %163
  %168 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %169 = bitcast %union.anon* %168 to i8*
  %170 = load i8, i8* %169, align 8
  %171 = sext i8 %170 to i32
  %172 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %173 = bitcast %union.anon* %172 to i32*
  %174 = load i32, i32* %173, align 8
  %175 = or i32 %171, %174
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %175)
  br label %467

; <label>:176:                                    ; preds = %163
  %177 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %178 = bitcast %union.anon* %177 to i8*
  %179 = load i8, i8* %178, align 8
  %180 = sext i8 %179 to i64
  %181 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %182 = bitcast %union.anon* %181 to i64*
  %183 = load i64, i64* %182, align 8
  %184 = or i64 %180, %183
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %184)
  br label %467

; <label>:185:                                    ; preds = %163
  %186 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %187 = bitcast %union.anon* %186 to i8*
  %188 = load i8, i8* %187, align 8
  %189 = sext i8 %188 to i32
  %190 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %191 = bitcast %union.anon* %190 to i8*
  %192 = load i8, i8* %191, align 8
  %193 = sext i8 %192 to i32
  %194 = or i32 %189, %193
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %194)
  br label %467

; <label>:195:                                    ; preds = %163
  %196 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %197 = bitcast %union.anon* %196 to i8*
  %198 = load i8, i8* %197, align 8
  %199 = sext i8 %198 to i32
  %200 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %201 = bitcast %union.anon* %200 to i8*
  %202 = load i8, i8* %201, align 8
  %203 = trunc i8 %202 to i1
  %204 = zext i1 %203 to i32
  %205 = or i32 %199, %204
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %205)
  br label %467

; <label>:206:                                    ; preds = %163
  br label %207

; <label>:207:                                    ; preds = %206
  br label %258

; <label>:208:                                    ; preds = %3
  %209 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %210 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %209, i32 0, i32 0
  %211 = load i32, i32* %210, align 8
  switch i32 %211, label %255 [
    i32 0, label %212
    i32 1, label %222
    i32 6, label %232
    i32 5, label %243
  ]

; <label>:212:                                    ; preds = %208
  %213 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %214 = bitcast %union.anon* %213 to i8*
  %215 = load i8, i8* %214, align 8
  %216 = trunc i8 %215 to i1
  %217 = zext i1 %216 to i32
  %218 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %219 = bitcast %union.anon* %218 to i32*
  %220 = load i32, i32* %219, align 8
  %221 = or i32 %217, %220
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %221)
  br label %467

; <label>:222:                                    ; preds = %208
  %223 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %224 = bitcast %union.anon* %223 to i8*
  %225 = load i8, i8* %224, align 8
  %226 = trunc i8 %225 to i1
  %227 = zext i1 %226 to i64
  %228 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %229 = bitcast %union.anon* %228 to i64*
  %230 = load i64, i64* %229, align 8
  %231 = or i64 %227, %230
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %231)
  br label %467

; <label>:232:                                    ; preds = %208
  %233 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %234 = bitcast %union.anon* %233 to i8*
  %235 = load i8, i8* %234, align 8
  %236 = trunc i8 %235 to i1
  %237 = zext i1 %236 to i32
  %238 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %239 = bitcast %union.anon* %238 to i8*
  %240 = load i8, i8* %239, align 8
  %241 = sext i8 %240 to i32
  %242 = or i32 %237, %241
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %242)
  br label %467

; <label>:243:                                    ; preds = %208
  %244 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %245 = bitcast %union.anon* %244 to i8*
  %246 = load i8, i8* %245, align 8
  %247 = trunc i8 %246 to i1
  %248 = zext i1 %247 to i32
  %249 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %250 = bitcast %union.anon* %249 to i8*
  %251 = load i8, i8* %250, align 8
  %252 = trunc i8 %251 to i1
  %253 = zext i1 %252 to i32
  %254 = or i32 %248, %253
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %254)
  br label %467

; <label>:255:                                    ; preds = %208
  br label %256

; <label>:256:                                    ; preds = %255
  br label %258

; <label>:257:                                    ; preds = %3
  br label %258

; <label>:258:                                    ; preds = %257, %256, %207, %162, %120
  %259 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %260 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %259, i32 0, i32 0
  %261 = load i32, i32* %260, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %72, i32 %261)
  store i8* getelementptr inbounds ([43 x i8], [43 x i8]* @.str.10, i32 0, i32 0), i8** %67, align 8, !noalias !113
  store %"class.std::__1::basic_string"* %72, %"class.std::__1::basic_string"** %68, align 8, !noalias !113
  %262 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !113
  %263 = load i8*, i8** %67, align 8, !noalias !113
  %264 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %262, i64 0, i8* %263)
          to label %265 unwind label %468

; <label>:265:                                    ; preds = %258
  store %"class.std::__1::basic_string"* %264, %"class.std::__1::basic_string"** %66, align 8, !noalias !113
  %266 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !113
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %64, align 8, !noalias !113
  store %"class.std::__1::basic_string"* %266, %"class.std::__1::basic_string"** %65, align 8, !noalias !113
  %267 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !113
  %268 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !113
  store %"class.std::__1::basic_string"* %267, %"class.std::__1::basic_string"** %62, align 8, !noalias !113
  store %"class.std::__1::basic_string"* %268, %"class.std::__1::basic_string"** %63, align 8, !noalias !113
  %269 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8, !noalias !113
  %270 = bitcast %"class.std::__1::basic_string"* %269 to %"class.std::__1::__basic_string_common"*
  %271 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %269, i32 0, i32 0
  %272 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !113
  %273 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %272, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %273, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !113
  %274 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !113
  %275 = bitcast %"class.std::__1::__compressed_pair"* %271 to i8*
  %276 = bitcast %"class.std::__1::__compressed_pair"* %274 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %275, i8* %276, i64 24, i32 8, i1 false) #10
  %277 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !113
  store %"class.std::__1::basic_string"* %277, %"class.std::__1::basic_string"** %58, align 8, !noalias !113
  %278 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8, !noalias !113
  %279 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %278, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %279, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !113
  %280 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !113
  %281 = bitcast %"class.std::__1::__compressed_pair"* %280 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %281, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !113
  %282 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !113
  %283 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %282, i32 0, i32 0
  %284 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %283, i32 0, i32 0
  %285 = bitcast %union.anon.0* %284 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %286 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %285, i32 0, i32 0
  store [3 x i64]* %286, [3 x i64]** %59, align 8, !noalias !113
  store i32 0, i32* %60, align 4, !noalias !113
  br label %287

; <label>:287:                                    ; preds = %290, %265
  %288 = load i32, i32* %60, align 4, !noalias !113
  %289 = icmp ult i32 %288, 3
  br i1 %289, label %290, label %297

; <label>:290:                                    ; preds = %287
  %291 = load i32, i32* %60, align 4, !noalias !113
  %292 = zext i32 %291 to i64
  %293 = load [3 x i64]*, [3 x i64]** %59, align 8, !noalias !113
  %294 = getelementptr inbounds [3 x i64], [3 x i64]* %293, i64 0, i64 %292
  store i64 0, i64* %294, align 8
  %295 = load i32, i32* %60, align 4, !noalias !113
  %296 = add i32 %295, 1
  store i32 %296, i32* %60, align 4, !noalias !113
  br label %287

; <label>:297:                                    ; preds = %287
  br label %298

; <label>:298:                                    ; preds = %297
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %54, align 8, !noalias !116
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %55, align 8, !noalias !116
  %299 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !116
  %300 = load i8*, i8** %55, align 8, !noalias !116
  %301 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %299, i8* %300)
          to label %302 unwind label %472

; <label>:302:                                    ; preds = %298
  store %"class.std::__1::basic_string"* %301, %"class.std::__1::basic_string"** %53, align 8, !noalias !116
  %303 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !116
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %51, align 8, !noalias !116
  store %"class.std::__1::basic_string"* %303, %"class.std::__1::basic_string"** %52, align 8, !noalias !116
  %304 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !116
  %305 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !116
  store %"class.std::__1::basic_string"* %304, %"class.std::__1::basic_string"** %49, align 8, !noalias !116
  store %"class.std::__1::basic_string"* %305, %"class.std::__1::basic_string"** %50, align 8, !noalias !116
  %306 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !116
  %307 = bitcast %"class.std::__1::basic_string"* %306 to %"class.std::__1::__basic_string_common"*
  %308 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %306, i32 0, i32 0
  %309 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !116
  %310 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %309, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %310, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !116
  %311 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !116
  %312 = bitcast %"class.std::__1::__compressed_pair"* %308 to i8*
  %313 = bitcast %"class.std::__1::__compressed_pair"* %311 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %312, i8* %313, i64 24, i32 8, i1 false) #10
  %314 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !116
  store %"class.std::__1::basic_string"* %314, %"class.std::__1::basic_string"** %45, align 8, !noalias !116
  %315 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !116
  %316 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %315, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %316, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !116
  %317 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !116
  %318 = bitcast %"class.std::__1::__compressed_pair"* %317 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %318, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !116
  %319 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !116
  %320 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %319, i32 0, i32 0
  %321 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %320, i32 0, i32 0
  %322 = bitcast %union.anon.0* %321 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %323 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %322, i32 0, i32 0
  store [3 x i64]* %323, [3 x i64]** %46, align 8, !noalias !116
  store i32 0, i32* %47, align 4, !noalias !116
  br label %324

; <label>:324:                                    ; preds = %327, %302
  %325 = load i32, i32* %47, align 4, !noalias !116
  %326 = icmp ult i32 %325, 3
  br i1 %326, label %327, label %334

; <label>:327:                                    ; preds = %324
  %328 = load i32, i32* %47, align 4, !noalias !116
  %329 = zext i32 %328 to i64
  %330 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !116
  %331 = getelementptr inbounds [3 x i64], [3 x i64]* %330, i64 0, i64 %329
  store i64 0, i64* %331, align 8
  %332 = load i32, i32* %47, align 4, !noalias !116
  %333 = add i32 %332, 1
  store i32 %333, i32* %47, align 4, !noalias !116
  br label %324

; <label>:334:                                    ; preds = %324
  br label %335

; <label>:335:                                    ; preds = %334
  %336 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %337 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %336, i32 0, i32 0
  %338 = load i32, i32* %337, align 8
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %75, i32 %338)
          to label %339 unwind label %476

; <label>:339:                                    ; preds = %335
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %41, align 8, !noalias !119
  store %"class.std::__1::basic_string"* %75, %"class.std::__1::basic_string"** %42, align 8, !noalias !119
  %340 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !119
  %341 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !119
  store %"class.std::__1::basic_string"* %340, %"class.std::__1::basic_string"** %39, align 8, !noalias !119
  store %"class.std::__1::basic_string"* %341, %"class.std::__1::basic_string"** %40, align 8, !noalias !119
  %342 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !119
  %343 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !119
  store %"class.std::__1::basic_string"* %343, %"class.std::__1::basic_string"** %38, align 8, !noalias !119
  %344 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !119
  store %"class.std::__1::basic_string"* %344, %"class.std::__1::basic_string"** %37, align 8, !noalias !119
  %345 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !119
  store %"class.std::__1::basic_string"* %345, %"class.std::__1::basic_string"** %36, align 8, !noalias !119
  %346 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !119
  %347 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %346, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %347, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !119
  %348 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !119
  %349 = bitcast %"class.std::__1::__compressed_pair"* %348 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %349, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !119
  %350 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !119
  %351 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %350, i32 0, i32 0
  %352 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %351, i32 0, i32 0
  %353 = bitcast %union.anon.0* %352 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %354 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %353, i32 0, i32 0
  %355 = bitcast %union.anon.1* %354 to i8*
  %356 = load i8, i8* %355, align 8, !noalias !119
  %357 = zext i8 %356 to i32
  %358 = and i32 %357, 1
  %359 = icmp ne i32 %358, 0
  br i1 %359, label %360, label %371

; <label>:360:                                    ; preds = %339
  store %"class.std::__1::basic_string"* %345, %"class.std::__1::basic_string"** %28, align 8, !noalias !119
  %361 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !119
  %362 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %361, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %362, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !119
  %363 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !119
  %364 = bitcast %"class.std::__1::__compressed_pair"* %363 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %364, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !119
  %365 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !119
  %366 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %365, i32 0, i32 0
  %367 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %366, i32 0, i32 0
  %368 = bitcast %union.anon.0* %367 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %369 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %368, i32 0, i32 2
  %370 = load i8*, i8** %369, align 8
  br label %384

; <label>:371:                                    ; preds = %339
  store %"class.std::__1::basic_string"* %345, %"class.std::__1::basic_string"** %33, align 8, !noalias !119
  %372 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !119
  %373 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %372, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %373, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !119
  %374 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !119
  %375 = bitcast %"class.std::__1::__compressed_pair"* %374 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %375, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !119
  %376 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !119
  %377 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %376, i32 0, i32 0
  %378 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %377, i32 0, i32 0
  %379 = bitcast %union.anon.0* %378 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %380 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %379, i32 0, i32 1
  %381 = getelementptr inbounds [23 x i8], [23 x i8]* %380, i64 0, i64 0
  store i8* %381, i8** %30, align 8, !noalias !119
  %382 = load i8*, i8** %30, align 8, !noalias !119
  store i8* %382, i8** %29, align 8, !noalias !119
  %383 = load i8*, i8** %29, align 8, !noalias !119
  br label %384

; <label>:384:                                    ; preds = %371, %360
  %385 = phi i8* [ %370, %360 ], [ %383, %371 ]
  store i8* %385, i8** %25, align 8, !noalias !119
  %386 = load i8*, i8** %25, align 8, !noalias !119
  %387 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !119
  store %"class.std::__1::basic_string"* %387, %"class.std::__1::basic_string"** %24, align 8, !noalias !119
  %388 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !119
  store %"class.std::__1::basic_string"* %388, %"class.std::__1::basic_string"** %23, align 8, !noalias !119
  %389 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !119
  %390 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %389, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %390, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !119
  %391 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !119
  %392 = bitcast %"class.std::__1::__compressed_pair"* %391 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %392, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !119
  %393 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !119
  %394 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %393, i32 0, i32 0
  %395 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %394, i32 0, i32 0
  %396 = bitcast %union.anon.0* %395 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %397 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %396, i32 0, i32 0
  %398 = bitcast %union.anon.1* %397 to i8*
  %399 = load i8, i8* %398, align 8, !noalias !119
  %400 = zext i8 %399 to i32
  %401 = and i32 %400, 1
  %402 = icmp ne i32 %401, 0
  br i1 %402, label %403, label %414

; <label>:403:                                    ; preds = %384
  store %"class.std::__1::basic_string"* %388, %"class.std::__1::basic_string"** %17, align 8, !noalias !119
  %404 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !119
  %405 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %404, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %405, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !119
  %406 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !119
  %407 = bitcast %"class.std::__1::__compressed_pair"* %406 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %407, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !119
  %408 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !119
  %409 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %408, i32 0, i32 0
  %410 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %409, i32 0, i32 0
  %411 = bitcast %union.anon.0* %410 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %412 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %411, i32 0, i32 1
  %413 = load i64, i64* %412, align 8
  br label %429

; <label>:414:                                    ; preds = %384
  store %"class.std::__1::basic_string"* %388, %"class.std::__1::basic_string"** %20, align 8, !noalias !119
  %415 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !119
  %416 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %415, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %416, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !119
  %417 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !119
  %418 = bitcast %"class.std::__1::__compressed_pair"* %417 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %418, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !119
  %419 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !119
  %420 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %419, i32 0, i32 0
  %421 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %420, i32 0, i32 0
  %422 = bitcast %union.anon.0* %421 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %423 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %422, i32 0, i32 0
  %424 = bitcast %union.anon.1* %423 to i8*
  %425 = load i8, i8* %424, align 8
  %426 = zext i8 %425 to i32
  %427 = ashr i32 %426, 1
  %428 = sext i32 %427 to i64
  br label %429

; <label>:429:                                    ; preds = %414, %403
  %430 = phi i64 [ %413, %403 ], [ %428, %414 ]
  %431 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %342, i8* %386, i64 %430)
          to label %432 unwind label %480

; <label>:432:                                    ; preds = %429
  store %"class.std::__1::basic_string"* %431, %"class.std::__1::basic_string"** %14, align 8, !noalias !119
  %433 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !119
  store %"class.std::__1::basic_string"* %69, %"class.std::__1::basic_string"** %12, align 8, !noalias !119
  store %"class.std::__1::basic_string"* %433, %"class.std::__1::basic_string"** %13, align 8, !noalias !119
  %434 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !119
  %435 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !119
  store %"class.std::__1::basic_string"* %434, %"class.std::__1::basic_string"** %10, align 8, !noalias !119
  store %"class.std::__1::basic_string"* %435, %"class.std::__1::basic_string"** %11, align 8, !noalias !119
  %436 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !119
  %437 = bitcast %"class.std::__1::basic_string"* %436 to %"class.std::__1::__basic_string_common"*
  %438 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %436, i32 0, i32 0
  %439 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !119
  %440 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %439, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %440, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !119
  %441 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !119
  %442 = bitcast %"class.std::__1::__compressed_pair"* %438 to i8*
  %443 = bitcast %"class.std::__1::__compressed_pair"* %441 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %442, i8* %443, i64 24, i32 8, i1 false) #10
  %444 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !119
  store %"class.std::__1::basic_string"* %444, %"class.std::__1::basic_string"** %6, align 8, !noalias !119
  %445 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !119
  %446 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %445, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %446, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !119
  %447 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !119
  %448 = bitcast %"class.std::__1::__compressed_pair"* %447 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %448, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !119
  %449 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !119
  %450 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %449, i32 0, i32 0
  %451 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %450, i32 0, i32 0
  %452 = bitcast %union.anon.0* %451 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %453 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %452, i32 0, i32 0
  store [3 x i64]* %453, [3 x i64]** %7, align 8, !noalias !119
  store i32 0, i32* %8, align 4, !noalias !119
  br label %454

; <label>:454:                                    ; preds = %457, %432
  %455 = load i32, i32* %8, align 4, !noalias !119
  %456 = icmp ult i32 %455, 3
  br i1 %456, label %457, label %464

; <label>:457:                                    ; preds = %454
  %458 = load i32, i32* %8, align 4, !noalias !119
  %459 = zext i32 %458 to i64
  %460 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !119
  %461 = getelementptr inbounds [3 x i64], [3 x i64]* %460, i64 0, i64 %459
  store i64 0, i64* %461, align 8
  %462 = load i32, i32* %8, align 4, !noalias !119
  %463 = add i32 %462, 1
  store i32 %463, i32* %8, align 4, !noalias !119
  br label %454

; <label>:464:                                    ; preds = %454
  br label %465

; <label>:465:                                    ; preds = %464
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %69, %class.AstNode* null)
          to label %466 unwind label %484

; <label>:466:                                    ; preds = %465
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %467

; <label>:467:                                    ; preds = %466, %243, %232, %222, %212, %195, %185, %176, %167, %151, %142, %134, %125, %109, %100, %91, %83
  ret void

; <label>:468:                                    ; preds = %258
  %469 = landingpad { i8*, i32 }
          cleanup
  %470 = extractvalue { i8*, i32 } %469, 0
  store i8* %470, i8** %73, align 8
  %471 = extractvalue { i8*, i32 } %469, 1
  store i32 %471, i32* %74, align 4
  br label %491

; <label>:472:                                    ; preds = %298
  %473 = landingpad { i8*, i32 }
          cleanup
  %474 = extractvalue { i8*, i32 } %473, 0
  store i8* %474, i8** %73, align 8
  %475 = extractvalue { i8*, i32 } %473, 1
  store i32 %475, i32* %74, align 4
  br label %490

; <label>:476:                                    ; preds = %335
  %477 = landingpad { i8*, i32 }
          cleanup
  %478 = extractvalue { i8*, i32 } %477, 0
  store i8* %478, i8** %73, align 8
  %479 = extractvalue { i8*, i32 } %477, 1
  store i32 %479, i32* %74, align 4
  br label %489

; <label>:480:                                    ; preds = %429
  %481 = landingpad { i8*, i32 }
          cleanup
  %482 = extractvalue { i8*, i32 } %481, 0
  store i8* %482, i8** %73, align 8
  %483 = extractvalue { i8*, i32 } %481, 1
  store i32 %483, i32* %74, align 4
  br label %488

; <label>:484:                                    ; preds = %465
  %485 = landingpad { i8*, i32 }
          cleanup
  %486 = extractvalue { i8*, i32 } %485, 0
  store i8* %486, i8** %73, align 8
  %487 = extractvalue { i8*, i32 } %485, 1
  store i32 %487, i32* %74, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  br label %488

; <label>:488:                                    ; preds = %484, %480
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  br label %489

; <label>:489:                                    ; preds = %488, %476
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  br label %490

; <label>:490:                                    ; preds = %489, %472
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  br label %491

; <label>:491:                                    ; preds = %490, %468
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %492

; <label>:492:                                    ; preds = %491
  %493 = load i8*, i8** %73, align 8
  %494 = load i32, i32* %74, align 4
  %495 = insertvalue { i8*, i32 } undef, i8* %493, 0
  %496 = insertvalue { i8*, i32 } %495, i32 %494, 1
  resume { i8*, i32 } %496
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic11bitwise_xorE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  %69 = alloca %"class.std::__1::basic_string", align 8
  %70 = alloca %"class.std::__1::basic_string", align 8
  %71 = alloca %"class.std::__1::basic_string", align 8
  %72 = alloca %"class.std::__1::basic_string", align 8
  %73 = alloca i8*
  %74 = alloca i32
  %75 = alloca %"class.std::__1::basic_string", align 8
  %76 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %77 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %76, i32 0, i32 0
  %78 = load i32, i32* %77, align 8
  switch i32 %78, label %257 [
    i32 0, label %79
    i32 1, label %121
    i32 6, label %163
    i32 5, label %208
  ]

; <label>:79:                                     ; preds = %3
  %80 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %81 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %80, i32 0, i32 0
  %82 = load i32, i32* %81, align 8
  switch i32 %82, label %119 [
    i32 0, label %83
    i32 1, label %91
    i32 6, label %100
    i32 5, label %109
  ]

; <label>:83:                                     ; preds = %79
  %84 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %85 = bitcast %union.anon* %84 to i32*
  %86 = load i32, i32* %85, align 8
  %87 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %88 = bitcast %union.anon* %87 to i32*
  %89 = load i32, i32* %88, align 8
  %90 = xor i32 %86, %89
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %90)
  br label %467

; <label>:91:                                     ; preds = %79
  %92 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %93 = bitcast %union.anon* %92 to i32*
  %94 = load i32, i32* %93, align 8
  %95 = sext i32 %94 to i64
  %96 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %97 = bitcast %union.anon* %96 to i64*
  %98 = load i64, i64* %97, align 8
  %99 = xor i64 %95, %98
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %99)
  br label %467

; <label>:100:                                    ; preds = %79
  %101 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %102 = bitcast %union.anon* %101 to i32*
  %103 = load i32, i32* %102, align 8
  %104 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %105 = bitcast %union.anon* %104 to i8*
  %106 = load i8, i8* %105, align 8
  %107 = sext i8 %106 to i32
  %108 = xor i32 %103, %107
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %108)
  br label %467

; <label>:109:                                    ; preds = %79
  %110 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %111 = bitcast %union.anon* %110 to i32*
  %112 = load i32, i32* %111, align 8
  %113 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %114 = bitcast %union.anon* %113 to i8*
  %115 = load i8, i8* %114, align 8
  %116 = trunc i8 %115 to i1
  %117 = zext i1 %116 to i32
  %118 = xor i32 %112, %117
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %118)
  br label %467

; <label>:119:                                    ; preds = %79
  br label %120

; <label>:120:                                    ; preds = %119
  br label %258

; <label>:121:                                    ; preds = %3
  %122 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %123 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %122, i32 0, i32 0
  %124 = load i32, i32* %123, align 8
  switch i32 %124, label %161 [
    i32 0, label %125
    i32 1, label %134
    i32 6, label %142
    i32 5, label %151
  ]

; <label>:125:                                    ; preds = %121
  %126 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %127 = bitcast %union.anon* %126 to i64*
  %128 = load i64, i64* %127, align 8
  %129 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %130 = bitcast %union.anon* %129 to i32*
  %131 = load i32, i32* %130, align 8
  %132 = sext i32 %131 to i64
  %133 = xor i64 %128, %132
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %133)
  br label %467

; <label>:134:                                    ; preds = %121
  %135 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %136 = bitcast %union.anon* %135 to i64*
  %137 = load i64, i64* %136, align 8
  %138 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %139 = bitcast %union.anon* %138 to i64*
  %140 = load i64, i64* %139, align 8
  %141 = xor i64 %137, %140
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %141)
  br label %467

; <label>:142:                                    ; preds = %121
  %143 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %144 = bitcast %union.anon* %143 to i64*
  %145 = load i64, i64* %144, align 8
  %146 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %147 = bitcast %union.anon* %146 to i8*
  %148 = load i8, i8* %147, align 8
  %149 = sext i8 %148 to i64
  %150 = xor i64 %145, %149
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %150)
  br label %467

; <label>:151:                                    ; preds = %121
  %152 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %153 = bitcast %union.anon* %152 to i64*
  %154 = load i64, i64* %153, align 8
  %155 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %156 = bitcast %union.anon* %155 to i8*
  %157 = load i8, i8* %156, align 8
  %158 = trunc i8 %157 to i1
  %159 = zext i1 %158 to i64
  %160 = xor i64 %154, %159
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %160)
  br label %467

; <label>:161:                                    ; preds = %121
  br label %162

; <label>:162:                                    ; preds = %161
  br label %258

; <label>:163:                                    ; preds = %3
  %164 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %165 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %164, i32 0, i32 0
  %166 = load i32, i32* %165, align 8
  switch i32 %166, label %206 [
    i32 0, label %167
    i32 1, label %176
    i32 6, label %185
    i32 5, label %195
  ]

; <label>:167:                                    ; preds = %163
  %168 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %169 = bitcast %union.anon* %168 to i8*
  %170 = load i8, i8* %169, align 8
  %171 = sext i8 %170 to i32
  %172 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %173 = bitcast %union.anon* %172 to i32*
  %174 = load i32, i32* %173, align 8
  %175 = xor i32 %171, %174
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %175)
  br label %467

; <label>:176:                                    ; preds = %163
  %177 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %178 = bitcast %union.anon* %177 to i8*
  %179 = load i8, i8* %178, align 8
  %180 = sext i8 %179 to i64
  %181 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %182 = bitcast %union.anon* %181 to i64*
  %183 = load i64, i64* %182, align 8
  %184 = xor i64 %180, %183
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %184)
  br label %467

; <label>:185:                                    ; preds = %163
  %186 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %187 = bitcast %union.anon* %186 to i8*
  %188 = load i8, i8* %187, align 8
  %189 = sext i8 %188 to i32
  %190 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %191 = bitcast %union.anon* %190 to i8*
  %192 = load i8, i8* %191, align 8
  %193 = sext i8 %192 to i32
  %194 = xor i32 %189, %193
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %194)
  br label %467

; <label>:195:                                    ; preds = %163
  %196 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %197 = bitcast %union.anon* %196 to i8*
  %198 = load i8, i8* %197, align 8
  %199 = sext i8 %198 to i32
  %200 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %201 = bitcast %union.anon* %200 to i8*
  %202 = load i8, i8* %201, align 8
  %203 = trunc i8 %202 to i1
  %204 = zext i1 %203 to i32
  %205 = xor i32 %199, %204
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %205)
  br label %467

; <label>:206:                                    ; preds = %163
  br label %207

; <label>:207:                                    ; preds = %206
  br label %258

; <label>:208:                                    ; preds = %3
  %209 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %210 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %209, i32 0, i32 0
  %211 = load i32, i32* %210, align 8
  switch i32 %211, label %255 [
    i32 0, label %212
    i32 1, label %222
    i32 6, label %232
    i32 5, label %243
  ]

; <label>:212:                                    ; preds = %208
  %213 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %214 = bitcast %union.anon* %213 to i8*
  %215 = load i8, i8* %214, align 8
  %216 = trunc i8 %215 to i1
  %217 = zext i1 %216 to i32
  %218 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %219 = bitcast %union.anon* %218 to i32*
  %220 = load i32, i32* %219, align 8
  %221 = xor i32 %217, %220
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %221)
  br label %467

; <label>:222:                                    ; preds = %208
  %223 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %224 = bitcast %union.anon* %223 to i8*
  %225 = load i8, i8* %224, align 8
  %226 = trunc i8 %225 to i1
  %227 = zext i1 %226 to i64
  %228 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %229 = bitcast %union.anon* %228 to i64*
  %230 = load i64, i64* %229, align 8
  %231 = xor i64 %227, %230
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %231)
  br label %467

; <label>:232:                                    ; preds = %208
  %233 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %234 = bitcast %union.anon* %233 to i8*
  %235 = load i8, i8* %234, align 8
  %236 = trunc i8 %235 to i1
  %237 = zext i1 %236 to i32
  %238 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %239 = bitcast %union.anon* %238 to i8*
  %240 = load i8, i8* %239, align 8
  %241 = sext i8 %240 to i32
  %242 = xor i32 %237, %241
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %242)
  br label %467

; <label>:243:                                    ; preds = %208
  %244 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %245 = bitcast %union.anon* %244 to i8*
  %246 = load i8, i8* %245, align 8
  %247 = trunc i8 %246 to i1
  %248 = zext i1 %247 to i32
  %249 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %250 = bitcast %union.anon* %249 to i8*
  %251 = load i8, i8* %250, align 8
  %252 = trunc i8 %251 to i1
  %253 = zext i1 %252 to i32
  %254 = xor i32 %248, %253
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %254)
  br label %467

; <label>:255:                                    ; preds = %208
  br label %256

; <label>:256:                                    ; preds = %255
  br label %258

; <label>:257:                                    ; preds = %3
  br label %258

; <label>:258:                                    ; preds = %257, %256, %207, %162, %120
  %259 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %260 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %259, i32 0, i32 0
  %261 = load i32, i32* %260, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %72, i32 %261)
  store i8* getelementptr inbounds ([44 x i8], [44 x i8]* @.str.9, i32 0, i32 0), i8** %67, align 8, !noalias !122
  store %"class.std::__1::basic_string"* %72, %"class.std::__1::basic_string"** %68, align 8, !noalias !122
  %262 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !122
  %263 = load i8*, i8** %67, align 8, !noalias !122
  %264 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %262, i64 0, i8* %263)
          to label %265 unwind label %468

; <label>:265:                                    ; preds = %258
  store %"class.std::__1::basic_string"* %264, %"class.std::__1::basic_string"** %66, align 8, !noalias !122
  %266 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !122
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %64, align 8, !noalias !122
  store %"class.std::__1::basic_string"* %266, %"class.std::__1::basic_string"** %65, align 8, !noalias !122
  %267 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !122
  %268 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !122
  store %"class.std::__1::basic_string"* %267, %"class.std::__1::basic_string"** %62, align 8, !noalias !122
  store %"class.std::__1::basic_string"* %268, %"class.std::__1::basic_string"** %63, align 8, !noalias !122
  %269 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8, !noalias !122
  %270 = bitcast %"class.std::__1::basic_string"* %269 to %"class.std::__1::__basic_string_common"*
  %271 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %269, i32 0, i32 0
  %272 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !122
  %273 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %272, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %273, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !122
  %274 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !122
  %275 = bitcast %"class.std::__1::__compressed_pair"* %271 to i8*
  %276 = bitcast %"class.std::__1::__compressed_pair"* %274 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %275, i8* %276, i64 24, i32 8, i1 false) #10
  %277 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !122
  store %"class.std::__1::basic_string"* %277, %"class.std::__1::basic_string"** %58, align 8, !noalias !122
  %278 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8, !noalias !122
  %279 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %278, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %279, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !122
  %280 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !122
  %281 = bitcast %"class.std::__1::__compressed_pair"* %280 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %281, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !122
  %282 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !122
  %283 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %282, i32 0, i32 0
  %284 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %283, i32 0, i32 0
  %285 = bitcast %union.anon.0* %284 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %286 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %285, i32 0, i32 0
  store [3 x i64]* %286, [3 x i64]** %59, align 8, !noalias !122
  store i32 0, i32* %60, align 4, !noalias !122
  br label %287

; <label>:287:                                    ; preds = %290, %265
  %288 = load i32, i32* %60, align 4, !noalias !122
  %289 = icmp ult i32 %288, 3
  br i1 %289, label %290, label %297

; <label>:290:                                    ; preds = %287
  %291 = load i32, i32* %60, align 4, !noalias !122
  %292 = zext i32 %291 to i64
  %293 = load [3 x i64]*, [3 x i64]** %59, align 8, !noalias !122
  %294 = getelementptr inbounds [3 x i64], [3 x i64]* %293, i64 0, i64 %292
  store i64 0, i64* %294, align 8
  %295 = load i32, i32* %60, align 4, !noalias !122
  %296 = add i32 %295, 1
  store i32 %296, i32* %60, align 4, !noalias !122
  br label %287

; <label>:297:                                    ; preds = %287
  br label %298

; <label>:298:                                    ; preds = %297
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %54, align 8, !noalias !125
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %55, align 8, !noalias !125
  %299 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !125
  %300 = load i8*, i8** %55, align 8, !noalias !125
  %301 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %299, i8* %300)
          to label %302 unwind label %472

; <label>:302:                                    ; preds = %298
  store %"class.std::__1::basic_string"* %301, %"class.std::__1::basic_string"** %53, align 8, !noalias !125
  %303 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !125
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %51, align 8, !noalias !125
  store %"class.std::__1::basic_string"* %303, %"class.std::__1::basic_string"** %52, align 8, !noalias !125
  %304 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !125
  %305 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !125
  store %"class.std::__1::basic_string"* %304, %"class.std::__1::basic_string"** %49, align 8, !noalias !125
  store %"class.std::__1::basic_string"* %305, %"class.std::__1::basic_string"** %50, align 8, !noalias !125
  %306 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !125
  %307 = bitcast %"class.std::__1::basic_string"* %306 to %"class.std::__1::__basic_string_common"*
  %308 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %306, i32 0, i32 0
  %309 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !125
  %310 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %309, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %310, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !125
  %311 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !125
  %312 = bitcast %"class.std::__1::__compressed_pair"* %308 to i8*
  %313 = bitcast %"class.std::__1::__compressed_pair"* %311 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %312, i8* %313, i64 24, i32 8, i1 false) #10
  %314 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !125
  store %"class.std::__1::basic_string"* %314, %"class.std::__1::basic_string"** %45, align 8, !noalias !125
  %315 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !125
  %316 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %315, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %316, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !125
  %317 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !125
  %318 = bitcast %"class.std::__1::__compressed_pair"* %317 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %318, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !125
  %319 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !125
  %320 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %319, i32 0, i32 0
  %321 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %320, i32 0, i32 0
  %322 = bitcast %union.anon.0* %321 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %323 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %322, i32 0, i32 0
  store [3 x i64]* %323, [3 x i64]** %46, align 8, !noalias !125
  store i32 0, i32* %47, align 4, !noalias !125
  br label %324

; <label>:324:                                    ; preds = %327, %302
  %325 = load i32, i32* %47, align 4, !noalias !125
  %326 = icmp ult i32 %325, 3
  br i1 %326, label %327, label %334

; <label>:327:                                    ; preds = %324
  %328 = load i32, i32* %47, align 4, !noalias !125
  %329 = zext i32 %328 to i64
  %330 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !125
  %331 = getelementptr inbounds [3 x i64], [3 x i64]* %330, i64 0, i64 %329
  store i64 0, i64* %331, align 8
  %332 = load i32, i32* %47, align 4, !noalias !125
  %333 = add i32 %332, 1
  store i32 %333, i32* %47, align 4, !noalias !125
  br label %324

; <label>:334:                                    ; preds = %324
  br label %335

; <label>:335:                                    ; preds = %334
  %336 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %337 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %336, i32 0, i32 0
  %338 = load i32, i32* %337, align 8
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %75, i32 %338)
          to label %339 unwind label %476

; <label>:339:                                    ; preds = %335
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %41, align 8, !noalias !128
  store %"class.std::__1::basic_string"* %75, %"class.std::__1::basic_string"** %42, align 8, !noalias !128
  %340 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !128
  %341 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !128
  store %"class.std::__1::basic_string"* %340, %"class.std::__1::basic_string"** %39, align 8, !noalias !128
  store %"class.std::__1::basic_string"* %341, %"class.std::__1::basic_string"** %40, align 8, !noalias !128
  %342 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !128
  %343 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !128
  store %"class.std::__1::basic_string"* %343, %"class.std::__1::basic_string"** %38, align 8, !noalias !128
  %344 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !128
  store %"class.std::__1::basic_string"* %344, %"class.std::__1::basic_string"** %37, align 8, !noalias !128
  %345 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !128
  store %"class.std::__1::basic_string"* %345, %"class.std::__1::basic_string"** %36, align 8, !noalias !128
  %346 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !128
  %347 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %346, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %347, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !128
  %348 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !128
  %349 = bitcast %"class.std::__1::__compressed_pair"* %348 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %349, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !128
  %350 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !128
  %351 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %350, i32 0, i32 0
  %352 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %351, i32 0, i32 0
  %353 = bitcast %union.anon.0* %352 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %354 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %353, i32 0, i32 0
  %355 = bitcast %union.anon.1* %354 to i8*
  %356 = load i8, i8* %355, align 8, !noalias !128
  %357 = zext i8 %356 to i32
  %358 = and i32 %357, 1
  %359 = icmp ne i32 %358, 0
  br i1 %359, label %360, label %371

; <label>:360:                                    ; preds = %339
  store %"class.std::__1::basic_string"* %345, %"class.std::__1::basic_string"** %28, align 8, !noalias !128
  %361 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !128
  %362 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %361, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %362, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !128
  %363 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !128
  %364 = bitcast %"class.std::__1::__compressed_pair"* %363 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %364, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !128
  %365 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !128
  %366 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %365, i32 0, i32 0
  %367 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %366, i32 0, i32 0
  %368 = bitcast %union.anon.0* %367 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %369 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %368, i32 0, i32 2
  %370 = load i8*, i8** %369, align 8
  br label %384

; <label>:371:                                    ; preds = %339
  store %"class.std::__1::basic_string"* %345, %"class.std::__1::basic_string"** %33, align 8, !noalias !128
  %372 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !128
  %373 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %372, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %373, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !128
  %374 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !128
  %375 = bitcast %"class.std::__1::__compressed_pair"* %374 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %375, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !128
  %376 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !128
  %377 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %376, i32 0, i32 0
  %378 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %377, i32 0, i32 0
  %379 = bitcast %union.anon.0* %378 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %380 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %379, i32 0, i32 1
  %381 = getelementptr inbounds [23 x i8], [23 x i8]* %380, i64 0, i64 0
  store i8* %381, i8** %30, align 8, !noalias !128
  %382 = load i8*, i8** %30, align 8, !noalias !128
  store i8* %382, i8** %29, align 8, !noalias !128
  %383 = load i8*, i8** %29, align 8, !noalias !128
  br label %384

; <label>:384:                                    ; preds = %371, %360
  %385 = phi i8* [ %370, %360 ], [ %383, %371 ]
  store i8* %385, i8** %25, align 8, !noalias !128
  %386 = load i8*, i8** %25, align 8, !noalias !128
  %387 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !128
  store %"class.std::__1::basic_string"* %387, %"class.std::__1::basic_string"** %24, align 8, !noalias !128
  %388 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !128
  store %"class.std::__1::basic_string"* %388, %"class.std::__1::basic_string"** %23, align 8, !noalias !128
  %389 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !128
  %390 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %389, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %390, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !128
  %391 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !128
  %392 = bitcast %"class.std::__1::__compressed_pair"* %391 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %392, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !128
  %393 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !128
  %394 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %393, i32 0, i32 0
  %395 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %394, i32 0, i32 0
  %396 = bitcast %union.anon.0* %395 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %397 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %396, i32 0, i32 0
  %398 = bitcast %union.anon.1* %397 to i8*
  %399 = load i8, i8* %398, align 8, !noalias !128
  %400 = zext i8 %399 to i32
  %401 = and i32 %400, 1
  %402 = icmp ne i32 %401, 0
  br i1 %402, label %403, label %414

; <label>:403:                                    ; preds = %384
  store %"class.std::__1::basic_string"* %388, %"class.std::__1::basic_string"** %17, align 8, !noalias !128
  %404 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !128
  %405 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %404, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %405, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !128
  %406 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !128
  %407 = bitcast %"class.std::__1::__compressed_pair"* %406 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %407, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !128
  %408 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !128
  %409 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %408, i32 0, i32 0
  %410 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %409, i32 0, i32 0
  %411 = bitcast %union.anon.0* %410 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %412 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %411, i32 0, i32 1
  %413 = load i64, i64* %412, align 8
  br label %429

; <label>:414:                                    ; preds = %384
  store %"class.std::__1::basic_string"* %388, %"class.std::__1::basic_string"** %20, align 8, !noalias !128
  %415 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !128
  %416 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %415, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %416, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !128
  %417 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !128
  %418 = bitcast %"class.std::__1::__compressed_pair"* %417 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %418, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !128
  %419 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !128
  %420 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %419, i32 0, i32 0
  %421 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %420, i32 0, i32 0
  %422 = bitcast %union.anon.0* %421 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %423 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %422, i32 0, i32 0
  %424 = bitcast %union.anon.1* %423 to i8*
  %425 = load i8, i8* %424, align 8
  %426 = zext i8 %425 to i32
  %427 = ashr i32 %426, 1
  %428 = sext i32 %427 to i64
  br label %429

; <label>:429:                                    ; preds = %414, %403
  %430 = phi i64 [ %413, %403 ], [ %428, %414 ]
  %431 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %342, i8* %386, i64 %430)
          to label %432 unwind label %480

; <label>:432:                                    ; preds = %429
  store %"class.std::__1::basic_string"* %431, %"class.std::__1::basic_string"** %14, align 8, !noalias !128
  %433 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !128
  store %"class.std::__1::basic_string"* %69, %"class.std::__1::basic_string"** %12, align 8, !noalias !128
  store %"class.std::__1::basic_string"* %433, %"class.std::__1::basic_string"** %13, align 8, !noalias !128
  %434 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !128
  %435 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !128
  store %"class.std::__1::basic_string"* %434, %"class.std::__1::basic_string"** %10, align 8, !noalias !128
  store %"class.std::__1::basic_string"* %435, %"class.std::__1::basic_string"** %11, align 8, !noalias !128
  %436 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !128
  %437 = bitcast %"class.std::__1::basic_string"* %436 to %"class.std::__1::__basic_string_common"*
  %438 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %436, i32 0, i32 0
  %439 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !128
  %440 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %439, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %440, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !128
  %441 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !128
  %442 = bitcast %"class.std::__1::__compressed_pair"* %438 to i8*
  %443 = bitcast %"class.std::__1::__compressed_pair"* %441 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %442, i8* %443, i64 24, i32 8, i1 false) #10
  %444 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !128
  store %"class.std::__1::basic_string"* %444, %"class.std::__1::basic_string"** %6, align 8, !noalias !128
  %445 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !128
  %446 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %445, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %446, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !128
  %447 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !128
  %448 = bitcast %"class.std::__1::__compressed_pair"* %447 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %448, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !128
  %449 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !128
  %450 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %449, i32 0, i32 0
  %451 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %450, i32 0, i32 0
  %452 = bitcast %union.anon.0* %451 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %453 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %452, i32 0, i32 0
  store [3 x i64]* %453, [3 x i64]** %7, align 8, !noalias !128
  store i32 0, i32* %8, align 4, !noalias !128
  br label %454

; <label>:454:                                    ; preds = %457, %432
  %455 = load i32, i32* %8, align 4, !noalias !128
  %456 = icmp ult i32 %455, 3
  br i1 %456, label %457, label %464

; <label>:457:                                    ; preds = %454
  %458 = load i32, i32* %8, align 4, !noalias !128
  %459 = zext i32 %458 to i64
  %460 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !128
  %461 = getelementptr inbounds [3 x i64], [3 x i64]* %460, i64 0, i64 %459
  store i64 0, i64* %461, align 8
  %462 = load i32, i32* %8, align 4, !noalias !128
  %463 = add i32 %462, 1
  store i32 %463, i32* %8, align 4, !noalias !128
  br label %454

; <label>:464:                                    ; preds = %454
  br label %465

; <label>:465:                                    ; preds = %464
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %69, %class.AstNode* null)
          to label %466 unwind label %484

; <label>:466:                                    ; preds = %465
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %467

; <label>:467:                                    ; preds = %466, %243, %232, %222, %212, %195, %185, %176, %167, %151, %142, %134, %125, %109, %100, %91, %83
  ret void

; <label>:468:                                    ; preds = %258
  %469 = landingpad { i8*, i32 }
          cleanup
  %470 = extractvalue { i8*, i32 } %469, 0
  store i8* %470, i8** %73, align 8
  %471 = extractvalue { i8*, i32 } %469, 1
  store i32 %471, i32* %74, align 4
  br label %491

; <label>:472:                                    ; preds = %298
  %473 = landingpad { i8*, i32 }
          cleanup
  %474 = extractvalue { i8*, i32 } %473, 0
  store i8* %474, i8** %73, align 8
  %475 = extractvalue { i8*, i32 } %473, 1
  store i32 %475, i32* %74, align 4
  br label %490

; <label>:476:                                    ; preds = %335
  %477 = landingpad { i8*, i32 }
          cleanup
  %478 = extractvalue { i8*, i32 } %477, 0
  store i8* %478, i8** %73, align 8
  %479 = extractvalue { i8*, i32 } %477, 1
  store i32 %479, i32* %74, align 4
  br label %489

; <label>:480:                                    ; preds = %429
  %481 = landingpad { i8*, i32 }
          cleanup
  %482 = extractvalue { i8*, i32 } %481, 0
  store i8* %482, i8** %73, align 8
  %483 = extractvalue { i8*, i32 } %481, 1
  store i32 %483, i32* %74, align 4
  br label %488

; <label>:484:                                    ; preds = %465
  %485 = landingpad { i8*, i32 }
          cleanup
  %486 = extractvalue { i8*, i32 } %485, 0
  store i8* %486, i8** %73, align 8
  %487 = extractvalue { i8*, i32 } %485, 1
  store i32 %487, i32* %74, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  br label %488

; <label>:488:                                    ; preds = %484, %480
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  br label %489

; <label>:489:                                    ; preds = %488, %476
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  br label %490

; <label>:490:                                    ; preds = %489, %472
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  br label %491

; <label>:491:                                    ; preds = %490, %468
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %492

; <label>:492:                                    ; preds = %491
  %493 = load i8*, i8** %73, align 8
  %494 = load i32, i32* %74, align 4
  %495 = insertvalue { i8*, i32 } undef, i8* %493, 0
  %496 = insertvalue { i8*, i32 } %495, i32 %494, 1
  resume { i8*, i32 } %496
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic13bitshift_leftE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  %69 = alloca %"class.std::__1::basic_string", align 8
  %70 = alloca %"class.std::__1::basic_string", align 8
  %71 = alloca %"class.std::__1::basic_string", align 8
  %72 = alloca %"class.std::__1::basic_string", align 8
  %73 = alloca i8*
  %74 = alloca i32
  %75 = alloca %"class.std::__1::basic_string", align 8
  %76 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %77 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %76, i32 0, i32 0
  %78 = load i32, i32* %77, align 8
  switch i32 %78, label %261 [
    i32 0, label %79
    i32 1, label %121
    i32 6, label %165
    i32 5, label %211
  ]

; <label>:79:                                     ; preds = %3
  %80 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %81 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %80, i32 0, i32 0
  %82 = load i32, i32* %81, align 8
  switch i32 %82, label %119 [
    i32 0, label %83
    i32 1, label %91
    i32 6, label %100
    i32 5, label %109
  ]

; <label>:83:                                     ; preds = %79
  %84 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %85 = bitcast %union.anon* %84 to i32*
  %86 = load i32, i32* %85, align 8
  %87 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %88 = bitcast %union.anon* %87 to i32*
  %89 = load i32, i32* %88, align 8
  %90 = shl i32 %86, %89
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %90)
  br label %471

; <label>:91:                                     ; preds = %79
  %92 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %93 = bitcast %union.anon* %92 to i32*
  %94 = load i32, i32* %93, align 8
  %95 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %96 = bitcast %union.anon* %95 to i64*
  %97 = load i64, i64* %96, align 8
  %98 = trunc i64 %97 to i32
  %99 = shl i32 %94, %98
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %99)
  br label %471

; <label>:100:                                    ; preds = %79
  %101 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %102 = bitcast %union.anon* %101 to i32*
  %103 = load i32, i32* %102, align 8
  %104 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %105 = bitcast %union.anon* %104 to i8*
  %106 = load i8, i8* %105, align 8
  %107 = sext i8 %106 to i32
  %108 = shl i32 %103, %107
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %108)
  br label %471

; <label>:109:                                    ; preds = %79
  %110 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %111 = bitcast %union.anon* %110 to i32*
  %112 = load i32, i32* %111, align 8
  %113 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %114 = bitcast %union.anon* %113 to i8*
  %115 = load i8, i8* %114, align 8
  %116 = trunc i8 %115 to i1
  %117 = zext i1 %116 to i32
  %118 = shl i32 %112, %117
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %118)
  br label %471

; <label>:119:                                    ; preds = %79
  br label %120

; <label>:120:                                    ; preds = %119
  br label %262

; <label>:121:                                    ; preds = %3
  %122 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %123 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %122, i32 0, i32 0
  %124 = load i32, i32* %123, align 8
  switch i32 %124, label %163 [
    i32 0, label %125
    i32 1, label %134
    i32 6, label %142
    i32 5, label %152
  ]

; <label>:125:                                    ; preds = %121
  %126 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %127 = bitcast %union.anon* %126 to i64*
  %128 = load i64, i64* %127, align 8
  %129 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %130 = bitcast %union.anon* %129 to i32*
  %131 = load i32, i32* %130, align 8
  %132 = zext i32 %131 to i64
  %133 = shl i64 %128, %132
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %133)
  br label %471

; <label>:134:                                    ; preds = %121
  %135 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %136 = bitcast %union.anon* %135 to i64*
  %137 = load i64, i64* %136, align 8
  %138 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %139 = bitcast %union.anon* %138 to i64*
  %140 = load i64, i64* %139, align 8
  %141 = shl i64 %137, %140
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %141)
  br label %471

; <label>:142:                                    ; preds = %121
  %143 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %144 = bitcast %union.anon* %143 to i64*
  %145 = load i64, i64* %144, align 8
  %146 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %147 = bitcast %union.anon* %146 to i8*
  %148 = load i8, i8* %147, align 8
  %149 = sext i8 %148 to i32
  %150 = zext i32 %149 to i64
  %151 = shl i64 %145, %150
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %151)
  br label %471

; <label>:152:                                    ; preds = %121
  %153 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %154 = bitcast %union.anon* %153 to i64*
  %155 = load i64, i64* %154, align 8
  %156 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %157 = bitcast %union.anon* %156 to i8*
  %158 = load i8, i8* %157, align 8
  %159 = trunc i8 %158 to i1
  %160 = zext i1 %159 to i32
  %161 = zext i32 %160 to i64
  %162 = shl i64 %155, %161
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %162)
  br label %471

; <label>:163:                                    ; preds = %121
  br label %164

; <label>:164:                                    ; preds = %163
  br label %262

; <label>:165:                                    ; preds = %3
  %166 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %167 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %166, i32 0, i32 0
  %168 = load i32, i32* %167, align 8
  switch i32 %168, label %209 [
    i32 0, label %169
    i32 1, label %178
    i32 6, label %188
    i32 5, label %198
  ]

; <label>:169:                                    ; preds = %165
  %170 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %171 = bitcast %union.anon* %170 to i8*
  %172 = load i8, i8* %171, align 8
  %173 = sext i8 %172 to i32
  %174 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %175 = bitcast %union.anon* %174 to i32*
  %176 = load i32, i32* %175, align 8
  %177 = shl i32 %173, %176
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %177)
  br label %471

; <label>:178:                                    ; preds = %165
  %179 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %180 = bitcast %union.anon* %179 to i8*
  %181 = load i8, i8* %180, align 8
  %182 = sext i8 %181 to i32
  %183 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %184 = bitcast %union.anon* %183 to i64*
  %185 = load i64, i64* %184, align 8
  %186 = trunc i64 %185 to i32
  %187 = shl i32 %182, %186
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %187)
  br label %471

; <label>:188:                                    ; preds = %165
  %189 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %190 = bitcast %union.anon* %189 to i8*
  %191 = load i8, i8* %190, align 8
  %192 = sext i8 %191 to i32
  %193 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %194 = bitcast %union.anon* %193 to i8*
  %195 = load i8, i8* %194, align 8
  %196 = sext i8 %195 to i32
  %197 = shl i32 %192, %196
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %197)
  br label %471

; <label>:198:                                    ; preds = %165
  %199 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %200 = bitcast %union.anon* %199 to i8*
  %201 = load i8, i8* %200, align 8
  %202 = sext i8 %201 to i32
  %203 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %204 = bitcast %union.anon* %203 to i8*
  %205 = load i8, i8* %204, align 8
  %206 = trunc i8 %205 to i1
  %207 = zext i1 %206 to i32
  %208 = shl i32 %202, %207
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %208)
  br label %471

; <label>:209:                                    ; preds = %165
  br label %210

; <label>:210:                                    ; preds = %209
  br label %262

; <label>:211:                                    ; preds = %3
  %212 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %213 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %212, i32 0, i32 0
  %214 = load i32, i32* %213, align 8
  switch i32 %214, label %259 [
    i32 0, label %215
    i32 1, label %225
    i32 6, label %236
    i32 5, label %247
  ]

; <label>:215:                                    ; preds = %211
  %216 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %217 = bitcast %union.anon* %216 to i8*
  %218 = load i8, i8* %217, align 8
  %219 = trunc i8 %218 to i1
  %220 = zext i1 %219 to i32
  %221 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %222 = bitcast %union.anon* %221 to i32*
  %223 = load i32, i32* %222, align 8
  %224 = shl i32 %220, %223
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %224)
  br label %471

; <label>:225:                                    ; preds = %211
  %226 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %227 = bitcast %union.anon* %226 to i8*
  %228 = load i8, i8* %227, align 8
  %229 = trunc i8 %228 to i1
  %230 = zext i1 %229 to i32
  %231 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %232 = bitcast %union.anon* %231 to i64*
  %233 = load i64, i64* %232, align 8
  %234 = trunc i64 %233 to i32
  %235 = shl i32 %230, %234
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %235)
  br label %471

; <label>:236:                                    ; preds = %211
  %237 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %238 = bitcast %union.anon* %237 to i8*
  %239 = load i8, i8* %238, align 8
  %240 = trunc i8 %239 to i1
  %241 = zext i1 %240 to i32
  %242 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %243 = bitcast %union.anon* %242 to i8*
  %244 = load i8, i8* %243, align 8
  %245 = sext i8 %244 to i32
  %246 = shl i32 %241, %245
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %246)
  br label %471

; <label>:247:                                    ; preds = %211
  %248 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %249 = bitcast %union.anon* %248 to i8*
  %250 = load i8, i8* %249, align 8
  %251 = trunc i8 %250 to i1
  %252 = zext i1 %251 to i32
  %253 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %254 = bitcast %union.anon* %253 to i8*
  %255 = load i8, i8* %254, align 8
  %256 = trunc i8 %255 to i1
  %257 = zext i1 %256 to i32
  %258 = shl i32 %252, %257
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %258)
  br label %471

; <label>:259:                                    ; preds = %211
  br label %260

; <label>:260:                                    ; preds = %259
  br label %262

; <label>:261:                                    ; preds = %3
  br label %262

; <label>:262:                                    ; preds = %261, %260, %210, %164, %120
  %263 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %264 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %263, i32 0, i32 0
  %265 = load i32, i32* %264, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %72, i32 %265)
  store i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.11, i32 0, i32 0), i8** %67, align 8, !noalias !131
  store %"class.std::__1::basic_string"* %72, %"class.std::__1::basic_string"** %68, align 8, !noalias !131
  %266 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !131
  %267 = load i8*, i8** %67, align 8, !noalias !131
  %268 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %266, i64 0, i8* %267)
          to label %269 unwind label %472

; <label>:269:                                    ; preds = %262
  store %"class.std::__1::basic_string"* %268, %"class.std::__1::basic_string"** %66, align 8, !noalias !131
  %270 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !131
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %64, align 8, !noalias !131
  store %"class.std::__1::basic_string"* %270, %"class.std::__1::basic_string"** %65, align 8, !noalias !131
  %271 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !131
  %272 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !131
  store %"class.std::__1::basic_string"* %271, %"class.std::__1::basic_string"** %62, align 8, !noalias !131
  store %"class.std::__1::basic_string"* %272, %"class.std::__1::basic_string"** %63, align 8, !noalias !131
  %273 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8, !noalias !131
  %274 = bitcast %"class.std::__1::basic_string"* %273 to %"class.std::__1::__basic_string_common"*
  %275 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %273, i32 0, i32 0
  %276 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !131
  %277 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %276, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %277, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !131
  %278 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !131
  %279 = bitcast %"class.std::__1::__compressed_pair"* %275 to i8*
  %280 = bitcast %"class.std::__1::__compressed_pair"* %278 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %279, i8* %280, i64 24, i32 8, i1 false) #10
  %281 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !131
  store %"class.std::__1::basic_string"* %281, %"class.std::__1::basic_string"** %58, align 8, !noalias !131
  %282 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8, !noalias !131
  %283 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %282, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %283, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !131
  %284 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !131
  %285 = bitcast %"class.std::__1::__compressed_pair"* %284 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %285, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !131
  %286 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !131
  %287 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %286, i32 0, i32 0
  %288 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %287, i32 0, i32 0
  %289 = bitcast %union.anon.0* %288 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %290 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %289, i32 0, i32 0
  store [3 x i64]* %290, [3 x i64]** %59, align 8, !noalias !131
  store i32 0, i32* %60, align 4, !noalias !131
  br label %291

; <label>:291:                                    ; preds = %294, %269
  %292 = load i32, i32* %60, align 4, !noalias !131
  %293 = icmp ult i32 %292, 3
  br i1 %293, label %294, label %301

; <label>:294:                                    ; preds = %291
  %295 = load i32, i32* %60, align 4, !noalias !131
  %296 = zext i32 %295 to i64
  %297 = load [3 x i64]*, [3 x i64]** %59, align 8, !noalias !131
  %298 = getelementptr inbounds [3 x i64], [3 x i64]* %297, i64 0, i64 %296
  store i64 0, i64* %298, align 8
  %299 = load i32, i32* %60, align 4, !noalias !131
  %300 = add i32 %299, 1
  store i32 %300, i32* %60, align 4, !noalias !131
  br label %291

; <label>:301:                                    ; preds = %291
  br label %302

; <label>:302:                                    ; preds = %301
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %54, align 8, !noalias !134
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %55, align 8, !noalias !134
  %303 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !134
  %304 = load i8*, i8** %55, align 8, !noalias !134
  %305 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %303, i8* %304)
          to label %306 unwind label %476

; <label>:306:                                    ; preds = %302
  store %"class.std::__1::basic_string"* %305, %"class.std::__1::basic_string"** %53, align 8, !noalias !134
  %307 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !134
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %51, align 8, !noalias !134
  store %"class.std::__1::basic_string"* %307, %"class.std::__1::basic_string"** %52, align 8, !noalias !134
  %308 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !134
  %309 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !134
  store %"class.std::__1::basic_string"* %308, %"class.std::__1::basic_string"** %49, align 8, !noalias !134
  store %"class.std::__1::basic_string"* %309, %"class.std::__1::basic_string"** %50, align 8, !noalias !134
  %310 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !134
  %311 = bitcast %"class.std::__1::basic_string"* %310 to %"class.std::__1::__basic_string_common"*
  %312 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %310, i32 0, i32 0
  %313 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !134
  %314 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %313, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %314, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !134
  %315 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !134
  %316 = bitcast %"class.std::__1::__compressed_pair"* %312 to i8*
  %317 = bitcast %"class.std::__1::__compressed_pair"* %315 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %316, i8* %317, i64 24, i32 8, i1 false) #10
  %318 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !134
  store %"class.std::__1::basic_string"* %318, %"class.std::__1::basic_string"** %45, align 8, !noalias !134
  %319 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !134
  %320 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %319, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %320, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !134
  %321 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !134
  %322 = bitcast %"class.std::__1::__compressed_pair"* %321 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %322, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !134
  %323 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !134
  %324 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %323, i32 0, i32 0
  %325 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %324, i32 0, i32 0
  %326 = bitcast %union.anon.0* %325 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %327 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %326, i32 0, i32 0
  store [3 x i64]* %327, [3 x i64]** %46, align 8, !noalias !134
  store i32 0, i32* %47, align 4, !noalias !134
  br label %328

; <label>:328:                                    ; preds = %331, %306
  %329 = load i32, i32* %47, align 4, !noalias !134
  %330 = icmp ult i32 %329, 3
  br i1 %330, label %331, label %338

; <label>:331:                                    ; preds = %328
  %332 = load i32, i32* %47, align 4, !noalias !134
  %333 = zext i32 %332 to i64
  %334 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !134
  %335 = getelementptr inbounds [3 x i64], [3 x i64]* %334, i64 0, i64 %333
  store i64 0, i64* %335, align 8
  %336 = load i32, i32* %47, align 4, !noalias !134
  %337 = add i32 %336, 1
  store i32 %337, i32* %47, align 4, !noalias !134
  br label %328

; <label>:338:                                    ; preds = %328
  br label %339

; <label>:339:                                    ; preds = %338
  %340 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %341 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %340, i32 0, i32 0
  %342 = load i32, i32* %341, align 8
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %75, i32 %342)
          to label %343 unwind label %480

; <label>:343:                                    ; preds = %339
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %41, align 8, !noalias !137
  store %"class.std::__1::basic_string"* %75, %"class.std::__1::basic_string"** %42, align 8, !noalias !137
  %344 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !137
  %345 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !137
  store %"class.std::__1::basic_string"* %344, %"class.std::__1::basic_string"** %39, align 8, !noalias !137
  store %"class.std::__1::basic_string"* %345, %"class.std::__1::basic_string"** %40, align 8, !noalias !137
  %346 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !137
  %347 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !137
  store %"class.std::__1::basic_string"* %347, %"class.std::__1::basic_string"** %38, align 8, !noalias !137
  %348 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !137
  store %"class.std::__1::basic_string"* %348, %"class.std::__1::basic_string"** %37, align 8, !noalias !137
  %349 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !137
  store %"class.std::__1::basic_string"* %349, %"class.std::__1::basic_string"** %36, align 8, !noalias !137
  %350 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !137
  %351 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %350, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %351, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !137
  %352 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !137
  %353 = bitcast %"class.std::__1::__compressed_pair"* %352 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %353, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !137
  %354 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !137
  %355 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %354, i32 0, i32 0
  %356 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %355, i32 0, i32 0
  %357 = bitcast %union.anon.0* %356 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %358 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %357, i32 0, i32 0
  %359 = bitcast %union.anon.1* %358 to i8*
  %360 = load i8, i8* %359, align 8, !noalias !137
  %361 = zext i8 %360 to i32
  %362 = and i32 %361, 1
  %363 = icmp ne i32 %362, 0
  br i1 %363, label %364, label %375

; <label>:364:                                    ; preds = %343
  store %"class.std::__1::basic_string"* %349, %"class.std::__1::basic_string"** %28, align 8, !noalias !137
  %365 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !137
  %366 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %365, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %366, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !137
  %367 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !137
  %368 = bitcast %"class.std::__1::__compressed_pair"* %367 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %368, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !137
  %369 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !137
  %370 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %369, i32 0, i32 0
  %371 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %370, i32 0, i32 0
  %372 = bitcast %union.anon.0* %371 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %373 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %372, i32 0, i32 2
  %374 = load i8*, i8** %373, align 8
  br label %388

; <label>:375:                                    ; preds = %343
  store %"class.std::__1::basic_string"* %349, %"class.std::__1::basic_string"** %33, align 8, !noalias !137
  %376 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !137
  %377 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %376, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %377, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !137
  %378 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !137
  %379 = bitcast %"class.std::__1::__compressed_pair"* %378 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %379, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !137
  %380 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !137
  %381 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %380, i32 0, i32 0
  %382 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %381, i32 0, i32 0
  %383 = bitcast %union.anon.0* %382 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %384 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %383, i32 0, i32 1
  %385 = getelementptr inbounds [23 x i8], [23 x i8]* %384, i64 0, i64 0
  store i8* %385, i8** %30, align 8, !noalias !137
  %386 = load i8*, i8** %30, align 8, !noalias !137
  store i8* %386, i8** %29, align 8, !noalias !137
  %387 = load i8*, i8** %29, align 8, !noalias !137
  br label %388

; <label>:388:                                    ; preds = %375, %364
  %389 = phi i8* [ %374, %364 ], [ %387, %375 ]
  store i8* %389, i8** %25, align 8, !noalias !137
  %390 = load i8*, i8** %25, align 8, !noalias !137
  %391 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !137
  store %"class.std::__1::basic_string"* %391, %"class.std::__1::basic_string"** %24, align 8, !noalias !137
  %392 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !137
  store %"class.std::__1::basic_string"* %392, %"class.std::__1::basic_string"** %23, align 8, !noalias !137
  %393 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !137
  %394 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %393, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %394, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !137
  %395 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !137
  %396 = bitcast %"class.std::__1::__compressed_pair"* %395 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %396, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !137
  %397 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !137
  %398 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %397, i32 0, i32 0
  %399 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %398, i32 0, i32 0
  %400 = bitcast %union.anon.0* %399 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %401 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %400, i32 0, i32 0
  %402 = bitcast %union.anon.1* %401 to i8*
  %403 = load i8, i8* %402, align 8, !noalias !137
  %404 = zext i8 %403 to i32
  %405 = and i32 %404, 1
  %406 = icmp ne i32 %405, 0
  br i1 %406, label %407, label %418

; <label>:407:                                    ; preds = %388
  store %"class.std::__1::basic_string"* %392, %"class.std::__1::basic_string"** %17, align 8, !noalias !137
  %408 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !137
  %409 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %408, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %409, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !137
  %410 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !137
  %411 = bitcast %"class.std::__1::__compressed_pair"* %410 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %411, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !137
  %412 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !137
  %413 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %412, i32 0, i32 0
  %414 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %413, i32 0, i32 0
  %415 = bitcast %union.anon.0* %414 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %416 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %415, i32 0, i32 1
  %417 = load i64, i64* %416, align 8
  br label %433

; <label>:418:                                    ; preds = %388
  store %"class.std::__1::basic_string"* %392, %"class.std::__1::basic_string"** %20, align 8, !noalias !137
  %419 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !137
  %420 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %419, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %420, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !137
  %421 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !137
  %422 = bitcast %"class.std::__1::__compressed_pair"* %421 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %422, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !137
  %423 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !137
  %424 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %423, i32 0, i32 0
  %425 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %424, i32 0, i32 0
  %426 = bitcast %union.anon.0* %425 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %427 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %426, i32 0, i32 0
  %428 = bitcast %union.anon.1* %427 to i8*
  %429 = load i8, i8* %428, align 8
  %430 = zext i8 %429 to i32
  %431 = ashr i32 %430, 1
  %432 = sext i32 %431 to i64
  br label %433

; <label>:433:                                    ; preds = %418, %407
  %434 = phi i64 [ %417, %407 ], [ %432, %418 ]
  %435 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %346, i8* %390, i64 %434)
          to label %436 unwind label %484

; <label>:436:                                    ; preds = %433
  store %"class.std::__1::basic_string"* %435, %"class.std::__1::basic_string"** %14, align 8, !noalias !137
  %437 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !137
  store %"class.std::__1::basic_string"* %69, %"class.std::__1::basic_string"** %12, align 8, !noalias !137
  store %"class.std::__1::basic_string"* %437, %"class.std::__1::basic_string"** %13, align 8, !noalias !137
  %438 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !137
  %439 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !137
  store %"class.std::__1::basic_string"* %438, %"class.std::__1::basic_string"** %10, align 8, !noalias !137
  store %"class.std::__1::basic_string"* %439, %"class.std::__1::basic_string"** %11, align 8, !noalias !137
  %440 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !137
  %441 = bitcast %"class.std::__1::basic_string"* %440 to %"class.std::__1::__basic_string_common"*
  %442 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %440, i32 0, i32 0
  %443 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !137
  %444 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %443, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %444, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !137
  %445 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !137
  %446 = bitcast %"class.std::__1::__compressed_pair"* %442 to i8*
  %447 = bitcast %"class.std::__1::__compressed_pair"* %445 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %446, i8* %447, i64 24, i32 8, i1 false) #10
  %448 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !137
  store %"class.std::__1::basic_string"* %448, %"class.std::__1::basic_string"** %6, align 8, !noalias !137
  %449 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !137
  %450 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %449, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %450, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !137
  %451 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !137
  %452 = bitcast %"class.std::__1::__compressed_pair"* %451 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %452, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !137
  %453 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !137
  %454 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %453, i32 0, i32 0
  %455 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %454, i32 0, i32 0
  %456 = bitcast %union.anon.0* %455 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %457 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %456, i32 0, i32 0
  store [3 x i64]* %457, [3 x i64]** %7, align 8, !noalias !137
  store i32 0, i32* %8, align 4, !noalias !137
  br label %458

; <label>:458:                                    ; preds = %461, %436
  %459 = load i32, i32* %8, align 4, !noalias !137
  %460 = icmp ult i32 %459, 3
  br i1 %460, label %461, label %468

; <label>:461:                                    ; preds = %458
  %462 = load i32, i32* %8, align 4, !noalias !137
  %463 = zext i32 %462 to i64
  %464 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !137
  %465 = getelementptr inbounds [3 x i64], [3 x i64]* %464, i64 0, i64 %463
  store i64 0, i64* %465, align 8
  %466 = load i32, i32* %8, align 4, !noalias !137
  %467 = add i32 %466, 1
  store i32 %467, i32* %8, align 4, !noalias !137
  br label %458

; <label>:468:                                    ; preds = %458
  br label %469

; <label>:469:                                    ; preds = %468
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %69, %class.AstNode* null)
          to label %470 unwind label %488

; <label>:470:                                    ; preds = %469
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %471

; <label>:471:                                    ; preds = %470, %247, %236, %225, %215, %198, %188, %178, %169, %152, %142, %134, %125, %109, %100, %91, %83
  ret void

; <label>:472:                                    ; preds = %262
  %473 = landingpad { i8*, i32 }
          cleanup
  %474 = extractvalue { i8*, i32 } %473, 0
  store i8* %474, i8** %73, align 8
  %475 = extractvalue { i8*, i32 } %473, 1
  store i32 %475, i32* %74, align 4
  br label %495

; <label>:476:                                    ; preds = %302
  %477 = landingpad { i8*, i32 }
          cleanup
  %478 = extractvalue { i8*, i32 } %477, 0
  store i8* %478, i8** %73, align 8
  %479 = extractvalue { i8*, i32 } %477, 1
  store i32 %479, i32* %74, align 4
  br label %494

; <label>:480:                                    ; preds = %339
  %481 = landingpad { i8*, i32 }
          cleanup
  %482 = extractvalue { i8*, i32 } %481, 0
  store i8* %482, i8** %73, align 8
  %483 = extractvalue { i8*, i32 } %481, 1
  store i32 %483, i32* %74, align 4
  br label %493

; <label>:484:                                    ; preds = %433
  %485 = landingpad { i8*, i32 }
          cleanup
  %486 = extractvalue { i8*, i32 } %485, 0
  store i8* %486, i8** %73, align 8
  %487 = extractvalue { i8*, i32 } %485, 1
  store i32 %487, i32* %74, align 4
  br label %492

; <label>:488:                                    ; preds = %469
  %489 = landingpad { i8*, i32 }
          cleanup
  %490 = extractvalue { i8*, i32 } %489, 0
  store i8* %490, i8** %73, align 8
  %491 = extractvalue { i8*, i32 } %489, 1
  store i32 %491, i32* %74, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  br label %492

; <label>:492:                                    ; preds = %488, %484
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  br label %493

; <label>:493:                                    ; preds = %492, %480
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  br label %494

; <label>:494:                                    ; preds = %493, %476
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  br label %495

; <label>:495:                                    ; preds = %494, %472
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %496

; <label>:496:                                    ; preds = %495
  %497 = load i8*, i8** %73, align 8
  %498 = load i32, i32* %74, align 4
  %499 = insertvalue { i8*, i32 } undef, i8* %497, 0
  %500 = insertvalue { i8*, i32 } %499, i32 %498, 1
  resume { i8*, i32 } %500
}

; Function Attrs: ssp uwtable
define void @_ZN4cdot3var10Arithmetic14bitshift_rightE7VariantS2_(%struct.Variant* noalias sret, %struct.Variant*, %struct.Variant*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
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
  %69 = alloca %"class.std::__1::basic_string", align 8
  %70 = alloca %"class.std::__1::basic_string", align 8
  %71 = alloca %"class.std::__1::basic_string", align 8
  %72 = alloca %"class.std::__1::basic_string", align 8
  %73 = alloca i8*
  %74 = alloca i32
  %75 = alloca %"class.std::__1::basic_string", align 8
  %76 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %77 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %76, i32 0, i32 0
  %78 = load i32, i32* %77, align 8
  switch i32 %78, label %261 [
    i32 0, label %79
    i32 1, label %121
    i32 6, label %165
    i32 5, label %211
  ]

; <label>:79:                                     ; preds = %3
  %80 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %81 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %80, i32 0, i32 0
  %82 = load i32, i32* %81, align 8
  switch i32 %82, label %119 [
    i32 0, label %83
    i32 1, label %91
    i32 6, label %100
    i32 5, label %109
  ]

; <label>:83:                                     ; preds = %79
  %84 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %85 = bitcast %union.anon* %84 to i32*
  %86 = load i32, i32* %85, align 8
  %87 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %88 = bitcast %union.anon* %87 to i32*
  %89 = load i32, i32* %88, align 8
  %90 = ashr i32 %86, %89
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %90)
  br label %471

; <label>:91:                                     ; preds = %79
  %92 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %93 = bitcast %union.anon* %92 to i32*
  %94 = load i32, i32* %93, align 8
  %95 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %96 = bitcast %union.anon* %95 to i64*
  %97 = load i64, i64* %96, align 8
  %98 = trunc i64 %97 to i32
  %99 = ashr i32 %94, %98
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %99)
  br label %471

; <label>:100:                                    ; preds = %79
  %101 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %102 = bitcast %union.anon* %101 to i32*
  %103 = load i32, i32* %102, align 8
  %104 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %105 = bitcast %union.anon* %104 to i8*
  %106 = load i8, i8* %105, align 8
  %107 = sext i8 %106 to i32
  %108 = ashr i32 %103, %107
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %108)
  br label %471

; <label>:109:                                    ; preds = %79
  %110 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %111 = bitcast %union.anon* %110 to i32*
  %112 = load i32, i32* %111, align 8
  %113 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %114 = bitcast %union.anon* %113 to i8*
  %115 = load i8, i8* %114, align 8
  %116 = trunc i8 %115 to i1
  %117 = zext i1 %116 to i32
  %118 = ashr i32 %112, %117
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %118)
  br label %471

; <label>:119:                                    ; preds = %79
  br label %120

; <label>:120:                                    ; preds = %119
  br label %262

; <label>:121:                                    ; preds = %3
  %122 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %123 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %122, i32 0, i32 0
  %124 = load i32, i32* %123, align 8
  switch i32 %124, label %163 [
    i32 0, label %125
    i32 1, label %134
    i32 6, label %142
    i32 5, label %152
  ]

; <label>:125:                                    ; preds = %121
  %126 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %127 = bitcast %union.anon* %126 to i64*
  %128 = load i64, i64* %127, align 8
  %129 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %130 = bitcast %union.anon* %129 to i32*
  %131 = load i32, i32* %130, align 8
  %132 = zext i32 %131 to i64
  %133 = ashr i64 %128, %132
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %133)
  br label %471

; <label>:134:                                    ; preds = %121
  %135 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %136 = bitcast %union.anon* %135 to i64*
  %137 = load i64, i64* %136, align 8
  %138 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %139 = bitcast %union.anon* %138 to i64*
  %140 = load i64, i64* %139, align 8
  %141 = ashr i64 %137, %140
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %141)
  br label %471

; <label>:142:                                    ; preds = %121
  %143 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %144 = bitcast %union.anon* %143 to i64*
  %145 = load i64, i64* %144, align 8
  %146 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %147 = bitcast %union.anon* %146 to i8*
  %148 = load i8, i8* %147, align 8
  %149 = sext i8 %148 to i32
  %150 = zext i32 %149 to i64
  %151 = ashr i64 %145, %150
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %151)
  br label %471

; <label>:152:                                    ; preds = %121
  %153 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %154 = bitcast %union.anon* %153 to i64*
  %155 = load i64, i64* %154, align 8
  %156 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %157 = bitcast %union.anon* %156 to i8*
  %158 = load i8, i8* %157, align 8
  %159 = trunc i8 %158 to i1
  %160 = zext i1 %159 to i32
  %161 = zext i32 %160 to i64
  %162 = ashr i64 %155, %161
  call void @_ZN7VariantC1El(%struct.Variant* %0, i64 %162)
  br label %471

; <label>:163:                                    ; preds = %121
  br label %164

; <label>:164:                                    ; preds = %163
  br label %262

; <label>:165:                                    ; preds = %3
  %166 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %167 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %166, i32 0, i32 0
  %168 = load i32, i32* %167, align 8
  switch i32 %168, label %209 [
    i32 0, label %169
    i32 1, label %178
    i32 6, label %188
    i32 5, label %198
  ]

; <label>:169:                                    ; preds = %165
  %170 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %171 = bitcast %union.anon* %170 to i8*
  %172 = load i8, i8* %171, align 8
  %173 = sext i8 %172 to i32
  %174 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %175 = bitcast %union.anon* %174 to i32*
  %176 = load i32, i32* %175, align 8
  %177 = ashr i32 %173, %176
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %177)
  br label %471

; <label>:178:                                    ; preds = %165
  %179 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %180 = bitcast %union.anon* %179 to i8*
  %181 = load i8, i8* %180, align 8
  %182 = sext i8 %181 to i32
  %183 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %184 = bitcast %union.anon* %183 to i64*
  %185 = load i64, i64* %184, align 8
  %186 = trunc i64 %185 to i32
  %187 = ashr i32 %182, %186
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %187)
  br label %471

; <label>:188:                                    ; preds = %165
  %189 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %190 = bitcast %union.anon* %189 to i8*
  %191 = load i8, i8* %190, align 8
  %192 = sext i8 %191 to i32
  %193 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %194 = bitcast %union.anon* %193 to i8*
  %195 = load i8, i8* %194, align 8
  %196 = sext i8 %195 to i32
  %197 = ashr i32 %192, %196
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %197)
  br label %471

; <label>:198:                                    ; preds = %165
  %199 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %200 = bitcast %union.anon* %199 to i8*
  %201 = load i8, i8* %200, align 8
  %202 = sext i8 %201 to i32
  %203 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %204 = bitcast %union.anon* %203 to i8*
  %205 = load i8, i8* %204, align 8
  %206 = trunc i8 %205 to i1
  %207 = zext i1 %206 to i32
  %208 = ashr i32 %202, %207
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %208)
  br label %471

; <label>:209:                                    ; preds = %165
  br label %210

; <label>:210:                                    ; preds = %209
  br label %262

; <label>:211:                                    ; preds = %3
  %212 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %213 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %212, i32 0, i32 0
  %214 = load i32, i32* %213, align 8
  switch i32 %214, label %259 [
    i32 0, label %215
    i32 1, label %225
    i32 6, label %236
    i32 5, label %247
  ]

; <label>:215:                                    ; preds = %211
  %216 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %217 = bitcast %union.anon* %216 to i8*
  %218 = load i8, i8* %217, align 8
  %219 = trunc i8 %218 to i1
  %220 = zext i1 %219 to i32
  %221 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %222 = bitcast %union.anon* %221 to i32*
  %223 = load i32, i32* %222, align 8
  %224 = ashr i32 %220, %223
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %224)
  br label %471

; <label>:225:                                    ; preds = %211
  %226 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %227 = bitcast %union.anon* %226 to i8*
  %228 = load i8, i8* %227, align 8
  %229 = trunc i8 %228 to i1
  %230 = zext i1 %229 to i32
  %231 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %232 = bitcast %union.anon* %231 to i64*
  %233 = load i64, i64* %232, align 8
  %234 = trunc i64 %233 to i32
  %235 = ashr i32 %230, %234
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %235)
  br label %471

; <label>:236:                                    ; preds = %211
  %237 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %238 = bitcast %union.anon* %237 to i8*
  %239 = load i8, i8* %238, align 8
  %240 = trunc i8 %239 to i1
  %241 = zext i1 %240 to i32
  %242 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %243 = bitcast %union.anon* %242 to i8*
  %244 = load i8, i8* %243, align 8
  %245 = sext i8 %244 to i32
  %246 = ashr i32 %241, %245
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %246)
  br label %471

; <label>:247:                                    ; preds = %211
  %248 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 0
  %249 = bitcast %union.anon* %248 to i8*
  %250 = load i8, i8* %249, align 8
  %251 = trunc i8 %250 to i1
  %252 = zext i1 %251 to i32
  %253 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 0
  %254 = bitcast %union.anon* %253 to i8*
  %255 = load i8, i8* %254, align 8
  %256 = trunc i8 %255 to i1
  %257 = zext i1 %256 to i32
  %258 = ashr i32 %252, %257
  call void @_ZN7VariantC1Ei(%struct.Variant* %0, i32 %258)
  br label %471

; <label>:259:                                    ; preds = %211
  br label %260

; <label>:260:                                    ; preds = %259
  br label %262

; <label>:261:                                    ; preds = %3
  br label %262

; <label>:262:                                    ; preds = %261, %260, %210, %164, %120
  %263 = getelementptr inbounds %struct.Variant, %struct.Variant* %1, i32 0, i32 4
  %264 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %263, i32 0, i32 0
  %265 = load i32, i32* %264, align 8
  call void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %72, i32 %265)
  store i8* getelementptr inbounds ([47 x i8], [47 x i8]* @.str.12, i32 0, i32 0), i8** %67, align 8, !noalias !140
  store %"class.std::__1::basic_string"* %72, %"class.std::__1::basic_string"** %68, align 8, !noalias !140
  %266 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %68, align 8, !noalias !140
  %267 = load i8*, i8** %67, align 8, !noalias !140
  %268 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"* %266, i64 0, i8* %267)
          to label %269 unwind label %472

; <label>:269:                                    ; preds = %262
  store %"class.std::__1::basic_string"* %268, %"class.std::__1::basic_string"** %66, align 8, !noalias !140
  %270 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %66, align 8, !noalias !140
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %64, align 8, !noalias !140
  store %"class.std::__1::basic_string"* %270, %"class.std::__1::basic_string"** %65, align 8, !noalias !140
  %271 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %64, align 8, !noalias !140
  %272 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %65, align 8, !noalias !140
  store %"class.std::__1::basic_string"* %271, %"class.std::__1::basic_string"** %62, align 8, !noalias !140
  store %"class.std::__1::basic_string"* %272, %"class.std::__1::basic_string"** %63, align 8, !noalias !140
  %273 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %62, align 8, !noalias !140
  %274 = bitcast %"class.std::__1::basic_string"* %273 to %"class.std::__1::__basic_string_common"*
  %275 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %273, i32 0, i32 0
  %276 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !140
  %277 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %276, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %277, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !140
  %278 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %61, align 8, !noalias !140
  %279 = bitcast %"class.std::__1::__compressed_pair"* %275 to i8*
  %280 = bitcast %"class.std::__1::__compressed_pair"* %278 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %279, i8* %280, i64 24, i32 8, i1 false) #10
  %281 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %63, align 8, !noalias !140
  store %"class.std::__1::basic_string"* %281, %"class.std::__1::basic_string"** %58, align 8, !noalias !140
  %282 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8, !noalias !140
  %283 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %282, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %283, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !140
  %284 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %57, align 8, !noalias !140
  %285 = bitcast %"class.std::__1::__compressed_pair"* %284 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %285, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !140
  %286 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %56, align 8, !noalias !140
  %287 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %286, i32 0, i32 0
  %288 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %287, i32 0, i32 0
  %289 = bitcast %union.anon.0* %288 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %290 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %289, i32 0, i32 0
  store [3 x i64]* %290, [3 x i64]** %59, align 8, !noalias !140
  store i32 0, i32* %60, align 4, !noalias !140
  br label %291

; <label>:291:                                    ; preds = %294, %269
  %292 = load i32, i32* %60, align 4, !noalias !140
  %293 = icmp ult i32 %292, 3
  br i1 %293, label %294, label %301

; <label>:294:                                    ; preds = %291
  %295 = load i32, i32* %60, align 4, !noalias !140
  %296 = zext i32 %295 to i64
  %297 = load [3 x i64]*, [3 x i64]** %59, align 8, !noalias !140
  %298 = getelementptr inbounds [3 x i64], [3 x i64]* %297, i64 0, i64 %296
  store i64 0, i64* %298, align 8
  %299 = load i32, i32* %60, align 4, !noalias !140
  %300 = add i32 %299, 1
  store i32 %300, i32* %60, align 4, !noalias !140
  br label %291

; <label>:301:                                    ; preds = %291
  br label %302

; <label>:302:                                    ; preds = %301
  store %"class.std::__1::basic_string"* %71, %"class.std::__1::basic_string"** %54, align 8, !noalias !143
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %55, align 8, !noalias !143
  %303 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8, !noalias !143
  %304 = load i8*, i8** %55, align 8, !noalias !143
  %305 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"* %303, i8* %304)
          to label %306 unwind label %476

; <label>:306:                                    ; preds = %302
  store %"class.std::__1::basic_string"* %305, %"class.std::__1::basic_string"** %53, align 8, !noalias !143
  %307 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8, !noalias !143
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %51, align 8, !noalias !143
  store %"class.std::__1::basic_string"* %307, %"class.std::__1::basic_string"** %52, align 8, !noalias !143
  %308 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8, !noalias !143
  %309 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8, !noalias !143
  store %"class.std::__1::basic_string"* %308, %"class.std::__1::basic_string"** %49, align 8, !noalias !143
  store %"class.std::__1::basic_string"* %309, %"class.std::__1::basic_string"** %50, align 8, !noalias !143
  %310 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %49, align 8, !noalias !143
  %311 = bitcast %"class.std::__1::basic_string"* %310 to %"class.std::__1::__basic_string_common"*
  %312 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %310, i32 0, i32 0
  %313 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !143
  %314 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %313, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %314, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !143
  %315 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %48, align 8, !noalias !143
  %316 = bitcast %"class.std::__1::__compressed_pair"* %312 to i8*
  %317 = bitcast %"class.std::__1::__compressed_pair"* %315 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %316, i8* %317, i64 24, i32 8, i1 false) #10
  %318 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %50, align 8, !noalias !143
  store %"class.std::__1::basic_string"* %318, %"class.std::__1::basic_string"** %45, align 8, !noalias !143
  %319 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %45, align 8, !noalias !143
  %320 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %319, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %320, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !143
  %321 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %44, align 8, !noalias !143
  %322 = bitcast %"class.std::__1::__compressed_pair"* %321 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %322, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !143
  %323 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %43, align 8, !noalias !143
  %324 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %323, i32 0, i32 0
  %325 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %324, i32 0, i32 0
  %326 = bitcast %union.anon.0* %325 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %327 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %326, i32 0, i32 0
  store [3 x i64]* %327, [3 x i64]** %46, align 8, !noalias !143
  store i32 0, i32* %47, align 4, !noalias !143
  br label %328

; <label>:328:                                    ; preds = %331, %306
  %329 = load i32, i32* %47, align 4, !noalias !143
  %330 = icmp ult i32 %329, 3
  br i1 %330, label %331, label %338

; <label>:331:                                    ; preds = %328
  %332 = load i32, i32* %47, align 4, !noalias !143
  %333 = zext i32 %332 to i64
  %334 = load [3 x i64]*, [3 x i64]** %46, align 8, !noalias !143
  %335 = getelementptr inbounds [3 x i64], [3 x i64]* %334, i64 0, i64 %333
  store i64 0, i64* %335, align 8
  %336 = load i32, i32* %47, align 4, !noalias !143
  %337 = add i32 %336, 1
  store i32 %337, i32* %47, align 4, !noalias !143
  br label %328

; <label>:338:                                    ; preds = %328
  br label %339

; <label>:339:                                    ; preds = %338
  %340 = getelementptr inbounds %struct.Variant, %struct.Variant* %2, i32 0, i32 4
  %341 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %340, i32 0, i32 0
  %342 = load i32, i32* %341, align 8
  invoke void @_ZN3val9typetostrE9ValueType(%"class.std::__1::basic_string"* sret %75, i32 %342)
          to label %343 unwind label %480

; <label>:343:                                    ; preds = %339
  store %"class.std::__1::basic_string"* %70, %"class.std::__1::basic_string"** %41, align 8, !noalias !146
  store %"class.std::__1::basic_string"* %75, %"class.std::__1::basic_string"** %42, align 8, !noalias !146
  %344 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8, !noalias !146
  %345 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %42, align 8, !noalias !146
  store %"class.std::__1::basic_string"* %344, %"class.std::__1::basic_string"** %39, align 8, !noalias !146
  store %"class.std::__1::basic_string"* %345, %"class.std::__1::basic_string"** %40, align 8, !noalias !146
  %346 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8, !noalias !146
  %347 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !146
  store %"class.std::__1::basic_string"* %347, %"class.std::__1::basic_string"** %38, align 8, !noalias !146
  %348 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %38, align 8, !noalias !146
  store %"class.std::__1::basic_string"* %348, %"class.std::__1::basic_string"** %37, align 8, !noalias !146
  %349 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %37, align 8, !noalias !146
  store %"class.std::__1::basic_string"* %349, %"class.std::__1::basic_string"** %36, align 8, !noalias !146
  %350 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %36, align 8, !noalias !146
  %351 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %350, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %351, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !146
  %352 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %35, align 8, !noalias !146
  %353 = bitcast %"class.std::__1::__compressed_pair"* %352 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %353, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !146
  %354 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %34, align 8, !noalias !146
  %355 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %354, i32 0, i32 0
  %356 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %355, i32 0, i32 0
  %357 = bitcast %union.anon.0* %356 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %358 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %357, i32 0, i32 0
  %359 = bitcast %union.anon.1* %358 to i8*
  %360 = load i8, i8* %359, align 8, !noalias !146
  %361 = zext i8 %360 to i32
  %362 = and i32 %361, 1
  %363 = icmp ne i32 %362, 0
  br i1 %363, label %364, label %375

; <label>:364:                                    ; preds = %343
  store %"class.std::__1::basic_string"* %349, %"class.std::__1::basic_string"** %28, align 8, !noalias !146
  %365 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8, !noalias !146
  %366 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %365, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %366, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !146
  %367 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8, !noalias !146
  %368 = bitcast %"class.std::__1::__compressed_pair"* %367 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %368, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !146
  %369 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %26, align 8, !noalias !146
  %370 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %369, i32 0, i32 0
  %371 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %370, i32 0, i32 0
  %372 = bitcast %union.anon.0* %371 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %373 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %372, i32 0, i32 2
  %374 = load i8*, i8** %373, align 8
  br label %388

; <label>:375:                                    ; preds = %343
  store %"class.std::__1::basic_string"* %349, %"class.std::__1::basic_string"** %33, align 8, !noalias !146
  %376 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %33, align 8, !noalias !146
  %377 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %376, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %377, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !146
  %378 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %32, align 8, !noalias !146
  %379 = bitcast %"class.std::__1::__compressed_pair"* %378 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %379, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !146
  %380 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %31, align 8, !noalias !146
  %381 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %380, i32 0, i32 0
  %382 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %381, i32 0, i32 0
  %383 = bitcast %union.anon.0* %382 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %384 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %383, i32 0, i32 1
  %385 = getelementptr inbounds [23 x i8], [23 x i8]* %384, i64 0, i64 0
  store i8* %385, i8** %30, align 8, !noalias !146
  %386 = load i8*, i8** %30, align 8, !noalias !146
  store i8* %386, i8** %29, align 8, !noalias !146
  %387 = load i8*, i8** %29, align 8, !noalias !146
  br label %388

; <label>:388:                                    ; preds = %375, %364
  %389 = phi i8* [ %374, %364 ], [ %387, %375 ]
  store i8* %389, i8** %25, align 8, !noalias !146
  %390 = load i8*, i8** %25, align 8, !noalias !146
  %391 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %40, align 8, !noalias !146
  store %"class.std::__1::basic_string"* %391, %"class.std::__1::basic_string"** %24, align 8, !noalias !146
  %392 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8, !noalias !146
  store %"class.std::__1::basic_string"* %392, %"class.std::__1::basic_string"** %23, align 8, !noalias !146
  %393 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8, !noalias !146
  %394 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %393, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %394, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !146
  %395 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %22, align 8, !noalias !146
  %396 = bitcast %"class.std::__1::__compressed_pair"* %395 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %396, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !146
  %397 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %21, align 8, !noalias !146
  %398 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %397, i32 0, i32 0
  %399 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %398, i32 0, i32 0
  %400 = bitcast %union.anon.0* %399 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %401 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %400, i32 0, i32 0
  %402 = bitcast %union.anon.1* %401 to i8*
  %403 = load i8, i8* %402, align 8, !noalias !146
  %404 = zext i8 %403 to i32
  %405 = and i32 %404, 1
  %406 = icmp ne i32 %405, 0
  br i1 %406, label %407, label %418

; <label>:407:                                    ; preds = %388
  store %"class.std::__1::basic_string"* %392, %"class.std::__1::basic_string"** %17, align 8, !noalias !146
  %408 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8, !noalias !146
  %409 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %408, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %409, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !146
  %410 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %16, align 8, !noalias !146
  %411 = bitcast %"class.std::__1::__compressed_pair"* %410 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %411, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !146
  %412 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %15, align 8, !noalias !146
  %413 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %412, i32 0, i32 0
  %414 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %413, i32 0, i32 0
  %415 = bitcast %union.anon.0* %414 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %416 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %415, i32 0, i32 1
  %417 = load i64, i64* %416, align 8
  br label %433

; <label>:418:                                    ; preds = %388
  store %"class.std::__1::basic_string"* %392, %"class.std::__1::basic_string"** %20, align 8, !noalias !146
  %419 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8, !noalias !146
  %420 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %419, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %420, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !146
  %421 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8, !noalias !146
  %422 = bitcast %"class.std::__1::__compressed_pair"* %421 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %422, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !146
  %423 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8, !noalias !146
  %424 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %423, i32 0, i32 0
  %425 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %424, i32 0, i32 0
  %426 = bitcast %union.anon.0* %425 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %427 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %426, i32 0, i32 0
  %428 = bitcast %union.anon.1* %427 to i8*
  %429 = load i8, i8* %428, align 8
  %430 = zext i8 %429 to i32
  %431 = ashr i32 %430, 1
  %432 = sext i32 %431 to i64
  br label %433

; <label>:433:                                    ; preds = %418, %407
  %434 = phi i64 [ %417, %407 ], [ %432, %418 ]
  %435 = invoke dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"* %346, i8* %390, i64 %434)
          to label %436 unwind label %484

; <label>:436:                                    ; preds = %433
  store %"class.std::__1::basic_string"* %435, %"class.std::__1::basic_string"** %14, align 8, !noalias !146
  %437 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8, !noalias !146
  store %"class.std::__1::basic_string"* %69, %"class.std::__1::basic_string"** %12, align 8, !noalias !146
  store %"class.std::__1::basic_string"* %437, %"class.std::__1::basic_string"** %13, align 8, !noalias !146
  %438 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %12, align 8, !noalias !146
  %439 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8, !noalias !146
  store %"class.std::__1::basic_string"* %438, %"class.std::__1::basic_string"** %10, align 8, !noalias !146
  store %"class.std::__1::basic_string"* %439, %"class.std::__1::basic_string"** %11, align 8, !noalias !146
  %440 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %10, align 8, !noalias !146
  %441 = bitcast %"class.std::__1::basic_string"* %440 to %"class.std::__1::__basic_string_common"*
  %442 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %440, i32 0, i32 0
  %443 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !146
  %444 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %443, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %444, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !146
  %445 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %9, align 8, !noalias !146
  %446 = bitcast %"class.std::__1::__compressed_pair"* %442 to i8*
  %447 = bitcast %"class.std::__1::__compressed_pair"* %445 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %446, i8* %447, i64 24, i32 8, i1 false) #10
  %448 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8, !noalias !146
  store %"class.std::__1::basic_string"* %448, %"class.std::__1::basic_string"** %6, align 8, !noalias !146
  %449 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %6, align 8, !noalias !146
  %450 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %449, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %450, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !146
  %451 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %5, align 8, !noalias !146
  %452 = bitcast %"class.std::__1::__compressed_pair"* %451 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %452, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !146
  %453 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %4, align 8, !noalias !146
  %454 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %453, i32 0, i32 0
  %455 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %454, i32 0, i32 0
  %456 = bitcast %union.anon.0* %455 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %457 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %456, i32 0, i32 0
  store [3 x i64]* %457, [3 x i64]** %7, align 8, !noalias !146
  store i32 0, i32* %8, align 4, !noalias !146
  br label %458

; <label>:458:                                    ; preds = %461, %436
  %459 = load i32, i32* %8, align 4, !noalias !146
  %460 = icmp ult i32 %459, 3
  br i1 %460, label %461, label %468

; <label>:461:                                    ; preds = %458
  %462 = load i32, i32* %8, align 4, !noalias !146
  %463 = zext i32 %462 to i64
  %464 = load [3 x i64]*, [3 x i64]** %7, align 8, !noalias !146
  %465 = getelementptr inbounds [3 x i64], [3 x i64]* %464, i64 0, i64 %463
  store i64 0, i64* %465, align 8
  %466 = load i32, i32* %8, align 4, !noalias !146
  %467 = add i32 %466, 1
  store i32 %467, i32* %8, align 4, !noalias !146
  br label %458

; <label>:468:                                    ; preds = %458
  br label %469

; <label>:469:                                    ; preds = %468
  invoke void @_ZN12RuntimeError5raiseE13RuntimeErrorsNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEP7AstNode(i32 2, %"class.std::__1::basic_string"* %69, %class.AstNode* null)
          to label %470 unwind label %488

; <label>:470:                                    ; preds = %469
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %471

; <label>:471:                                    ; preds = %470, %247, %236, %225, %215, %198, %188, %178, %169, %152, %142, %134, %125, %109, %100, %91, %83
  ret void

; <label>:472:                                    ; preds = %262
  %473 = landingpad { i8*, i32 }
          cleanup
  %474 = extractvalue { i8*, i32 } %473, 0
  store i8* %474, i8** %73, align 8
  %475 = extractvalue { i8*, i32 } %473, 1
  store i32 %475, i32* %74, align 4
  br label %495

; <label>:476:                                    ; preds = %302
  %477 = landingpad { i8*, i32 }
          cleanup
  %478 = extractvalue { i8*, i32 } %477, 0
  store i8* %478, i8** %73, align 8
  %479 = extractvalue { i8*, i32 } %477, 1
  store i32 %479, i32* %74, align 4
  br label %494

; <label>:480:                                    ; preds = %339
  %481 = landingpad { i8*, i32 }
          cleanup
  %482 = extractvalue { i8*, i32 } %481, 0
  store i8* %482, i8** %73, align 8
  %483 = extractvalue { i8*, i32 } %481, 1
  store i32 %483, i32* %74, align 4
  br label %493

; <label>:484:                                    ; preds = %433
  %485 = landingpad { i8*, i32 }
          cleanup
  %486 = extractvalue { i8*, i32 } %485, 0
  store i8* %486, i8** %73, align 8
  %487 = extractvalue { i8*, i32 } %485, 1
  store i32 %487, i32* %74, align 4
  br label %492

; <label>:488:                                    ; preds = %469
  %489 = landingpad { i8*, i32 }
          cleanup
  %490 = extractvalue { i8*, i32 } %489, 0
  store i8* %490, i8** %73, align 8
  %491 = extractvalue { i8*, i32 } %489, 1
  store i32 %491, i32* %74, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %69) #10
  br label %492

; <label>:492:                                    ; preds = %488, %484
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %75) #10
  br label %493

; <label>:493:                                    ; preds = %492, %480
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %70) #10
  br label %494

; <label>:494:                                    ; preds = %493, %476
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %71) #10
  br label %495

; <label>:495:                                    ; preds = %494, %472
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %72) #10
  br label %496

; <label>:496:                                    ; preds = %495
  %497 = load i8*, i8** %73, align 8
  %498 = load i32, i32* %74, align 4
  %499 = insertvalue { i8*, i32 } undef, i8* %497, 0
  %500 = insertvalue { i8*, i32 } %499, i32 %498, 1
  resume { i8*, i32 } %500
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN7VariantD2Ev(%struct.Variant*) unnamed_addr #3 align 2 {
  %2 = alloca %struct.Variant*, align 8
  store %struct.Variant* %0, %struct.Variant** %2, align 8
  %3 = load %struct.Variant*, %struct.Variant** %2, align 8
  %4 = getelementptr inbounds %struct.Variant, %struct.Variant* %3, i32 0, i32 4
  call void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier* %4) #10
  %5 = getelementptr inbounds %struct.Variant, %struct.Variant* %3, i32 0, i32 3
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %5) #10
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier*) unnamed_addr #3 align 2 {
  %2 = alloca %struct.TypeSpecifier*, align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %2, align 8
  %3 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %2, align 8
  call void @_ZN13TypeSpecifierD2Ev(%struct.TypeSpecifier* %3) #10
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN13TypeSpecifierD2Ev(%struct.TypeSpecifier*) unnamed_addr #3 align 2 {
  %2 = alloca %struct.TypeSpecifier*, align 8
  store %struct.TypeSpecifier* %0, %struct.TypeSpecifier** %2, align 8
  %3 = load %struct.TypeSpecifier*, %struct.TypeSpecifier** %2, align 8
  %4 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 24
  call void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED1Ev(%"class.std::__1::vector.20"* %4) #10
  %5 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 23
  call void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED1Ev(%"class.std::__1::vector"* %5) #10
  %6 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 14
  call void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED1Ev(%"class.std::__1::unordered_map"* %6) #10
  %7 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 11
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %7) #10
  %8 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 2
  call void @_ZNSt3__110shared_ptrI10ExpressionED1Ev(%"class.std::__1::shared_ptr"* %8) #10
  %9 = getelementptr inbounds %struct.TypeSpecifier, %struct.TypeSpecifier* %3, i32 0, i32 1
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %9) #10
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED1Ev(%"class.std::__1::vector.20"*) unnamed_addr #3 align 2 {
  %2 = alloca %"class.std::__1::vector.20"*, align 8
  store %"class.std::__1::vector.20"* %0, %"class.std::__1::vector.20"** %2, align 8
  %3 = load %"class.std::__1::vector.20"*, %"class.std::__1::vector.20"** %2, align 8
  call void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::vector.20"* %3) #10
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED1Ev(%"class.std::__1::vector"*) unnamed_addr #3 align 2 {
  %2 = alloca %"class.std::__1::vector"*, align 8
  store %"class.std::__1::vector"* %0, %"class.std::__1::vector"** %2, align 8
  %3 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %2, align 8
  call void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::vector"* %3) #10
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED1Ev(%"class.std::__1::unordered_map"*) unnamed_addr #3 align 2 {
  %2 = alloca %"class.std::__1::unordered_map"*, align 8
  store %"class.std::__1::unordered_map"* %0, %"class.std::__1::unordered_map"** %2, align 8
  %3 = load %"class.std::__1::unordered_map"*, %"class.std::__1::unordered_map"** %2, align 8
  call void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED2Ev(%"class.std::__1::unordered_map"* %3) #10
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__110shared_ptrI10ExpressionED1Ev(%"class.std::__1::shared_ptr"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.std::__1::shared_ptr"*, align 8
  store %"class.std::__1::shared_ptr"* %0, %"class.std::__1::shared_ptr"** %2, align 8
  %3 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %2, align 8
  call void @_ZNSt3__110shared_ptrI10ExpressionED2Ev(%"class.std::__1::shared_ptr"* %3) #10
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::vector.20"*) unnamed_addr #3 align 2 {
  %2 = alloca %"class.std::__1::vector.20"*, align 8
  store %"class.std::__1::vector.20"* %0, %"class.std::__1::vector.20"** %2, align 8
  %3 = load %"class.std::__1::vector.20"*, %"class.std::__1::vector.20"** %2, align 8
  %4 = bitcast %"class.std::__1::vector.20"* %3 to %"class.std::__1::__vector_base.21"*
  call void @_ZNSt3__113__vector_baseI13TypeSpecifierNS_9allocatorIS1_EEED2Ev(%"class.std::__1::__vector_base.21"* %4) #10
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
  call void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier* %64) #10
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
  call void @_ZdlPv(i8* %94) #12
  br label %95

; <label>:95:                                     ; preds = %65, %1
  ret void
}

; Function Attrs: noinline noreturn nounwind
define linkonce_odr hidden void @__clang_call_terminate(i8*) #6 {
  %2 = call i8* @__cxa_begin_catch(i8* %0) #10
  call void @_ZSt9terminatev() #11
  unreachable
}

declare i8* @__cxa_begin_catch(i8*)

declare void @_ZSt9terminatev()

; Function Attrs: nobuiltin nounwind
declare void @_ZdlPv(i8*) #7

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::vector"*) unnamed_addr #3 align 2 {
  %2 = alloca %"class.std::__1::vector"*, align 8
  store %"class.std::__1::vector"* %0, %"class.std::__1::vector"** %2, align 8
  %3 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %2, align 8
  %4 = bitcast %"class.std::__1::vector"* %3 to %"class.std::__1::__vector_base"*
  call void @_ZNSt3__113__vector_baseINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::__vector_base"* %4) #10
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
  %24 = alloca %"struct.std::__1::__has_destroy.27", align 1
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
  %58 = bitcast %"struct.std::__1::__has_destroy.27"* %24 to %"struct.std::__1::integral_constant"*
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
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %64) #10
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
  call void @_ZdlPv(i8* %94) #12
  br label %95

; <label>:95:                                     ; preds = %65, %1
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierNS_4hashIS6_EENS_8equal_toIS6_EENS4_INS_4pairIKS6_S7_EEEEED2Ev(%"class.std::__1::unordered_map"*) unnamed_addr #3 align 2 {
  %2 = alloca %"class.std::__1::unordered_map"*, align 8
  store %"class.std::__1::unordered_map"* %0, %"class.std::__1::unordered_map"** %2, align 8
  %3 = load %"class.std::__1::unordered_map"*, %"class.std::__1::unordered_map"** %2, align 8
  %4 = getelementptr inbounds %"class.std::__1::unordered_map", %"class.std::__1::unordered_map"* %3, i32 0, i32 0
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED1Ev(%"class.std::__1::__hash_table"* %4) #10
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED1Ev(%"class.std::__1::__hash_table"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.std::__1::__hash_table"*, align 8
  store %"class.std::__1::__hash_table"* %0, %"class.std::__1::__hash_table"** %2, align 8
  %3 = load %"class.std::__1::__hash_table"*, %"class.std::__1::__hash_table"** %2, align 8
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEED2Ev(%"class.std::__1::__hash_table"* %3) #10
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
  call void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEE12__deallocateEPNS_16__hash_node_baseIPNS_11__hash_nodeIS9_PvEEEE(%"class.std::__1::__hash_table"* %30, %"struct.std::__1::__hash_node_base"* %37) #10
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
  call void @_ZdlPv(i8* %83) #12
  br label %84

; <label>:84:                                     ; preds = %1, %55
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__112__hash_tableINS_17__hash_value_typeINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierEENS_22__unordered_map_hasherIS7_S9_NS_4hashIS7_EELb1EEENS_21__unordered_map_equalIS7_S9_NS_8equal_toIS7_EELb1EEENS5_IS9_EEE12__deallocateEPNS_16__hash_node_baseIPNS_11__hash_nodeIS9_PvEEEE(%"class.std::__1::__hash_table"*, %"struct.std::__1::__hash_node_base"*) #5 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %3 = alloca %"class.std::__1::allocator.10"*, align 8
  %4 = alloca %"struct.std::__1::pair"*, align 8
  %5 = alloca %"class.std::__1::allocator.10"*, align 8
  %6 = alloca %"struct.std::__1::pair"*, align 8
  %7 = alloca %"struct.std::__1::integral_constant.29", align 1
  %8 = alloca %"struct.std::__1::__has_destroy.30", align 1
  %9 = alloca i8*, align 8
  %10 = alloca %"class.std::__1::allocator.10"*, align 8
  %11 = alloca %"struct.std::__1::__hash_node"*, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::allocator.10"*, align 8
  %14 = alloca %"struct.std::__1::__hash_node"*, align 8
  %15 = alloca i64, align 8
  %16 = alloca %"struct.std::__1::pair"*, align 8
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
  %53 = bitcast %"union.std::__1::__hash_value_type"* %52 to %"struct.std::__1::pair"*
  store %"struct.std::__1::pair"* %53, %"struct.std::__1::pair"** %16, align 8
  %54 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %16, align 8
  %55 = bitcast %"struct.std::__1::pair"* %54 to i8*
  br label %56

; <label>:56:                                     ; preds = %39
  store %"class.std::__1::allocator.10"* %49, %"class.std::__1::allocator.10"** %5, align 8
  store %"struct.std::__1::pair"* %54, %"struct.std::__1::pair"** %6, align 8
  %57 = bitcast %"struct.std::__1::__has_destroy.30"* %8 to %"struct.std::__1::integral_constant.29"*
  %58 = load %"class.std::__1::allocator.10"*, %"class.std::__1::allocator.10"** %5, align 8
  %59 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %6, align 8
  store %"class.std::__1::allocator.10"* %58, %"class.std::__1::allocator.10"** %3, align 8
  store %"struct.std::__1::pair"* %59, %"struct.std::__1::pair"** %4, align 8
  %60 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %4, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED1Ev(%"struct.std::__1::pair"* %60) #10
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
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED1Ev(%"struct.std::__1::pair"*) unnamed_addr #3 align 2 {
  %2 = alloca %"struct.std::__1::pair"*, align 8
  store %"struct.std::__1::pair"* %0, %"struct.std::__1::pair"** %2, align 8
  %3 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %2, align 8
  call void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED2Ev(%"struct.std::__1::pair"* %3) #10
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__14pairIKNS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE13TypeSpecifierED2Ev(%"struct.std::__1::pair"*) unnamed_addr #3 align 2 {
  %2 = alloca %"struct.std::__1::pair"*, align 8
  store %"struct.std::__1::pair"* %0, %"struct.std::__1::pair"** %2, align 8
  %3 = load %"struct.std::__1::pair"*, %"struct.std::__1::pair"** %2, align 8
  %4 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %3, i32 0, i32 1
  call void @_ZN13TypeSpecifierD1Ev(%struct.TypeSpecifier* %4) #10
  %5 = getelementptr inbounds %"struct.std::__1::pair", %"struct.std::__1::pair"* %3, i32 0, i32 0
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %5) #10
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
  call void @_ZNSt3__119__shared_weak_count16__release_sharedEv(%"class.std::__1::__shared_weak_count"* %9) #10
  br label %10

; <label>:10:                                     ; preds = %7, %1
  ret void
}

; Function Attrs: nounwind
declare void @_ZNSt3__119__shared_weak_count16__release_sharedEv(%"class.std::__1::__shared_weak_count"*) #2

; Function Attrs: nounwind readnone
declare float @llvm.pow.f32(float, float) #4

declare dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKcm(%"class.std::__1::basic_string"*, i64, i8*, i64) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #8

declare dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm(%"class.std::__1::basic_string"*, i8*, i64) #1

declare i8* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertENS_11__wrap_iterIPKcEEc(%"class.std::__1::basic_string"*, i8*, i8 signext) #1

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE9push_backEc(%"class.std::__1::basic_string"*, i8 signext) #1

declare dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc(%"class.std::__1::basic_string"*, i64, i8*) #1

declare dereferenceable(24) %"class.std::__1::basic_string"* @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc(%"class.std::__1::basic_string"*, i8*) #1

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr i32 @_ZNSt3__111char_traitsIcE7compareEPKcS3_m(i8*, i8*, i64) #9 align 2 {
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

attributes #0 = { ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { inlinehint nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind readnone }
attributes #5 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { noinline noreturn nounwind }
attributes #7 = { nobuiltin nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { argmemonly nounwind }
attributes #9 = { inlinehint ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #10 = { nounwind }
attributes #11 = { noreturn nounwind }
attributes #12 = { builtin nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"Apple LLVM version 8.1.0 (clang-802.0.42)"}
!2 = !{!3}
!3 = distinct !{!3, !4, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EERKS9_OS9_: argument 0"}
!4 = distinct !{!4, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EERKS9_OS9_"}
!5 = !{!6}
!6 = distinct !{!6, !7, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_RKS9_: argument 0"}
!7 = distinct !{!7, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_RKS9_"}
!8 = !{!9}
!9 = distinct !{!9, !10, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EES6_OS9_: argument 0"}
!10 = distinct !{!10, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EES6_OS9_"}
!11 = !{!12}
!12 = distinct !{!12, !13, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_S6_: argument 0"}
!13 = distinct !{!13, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_S6_"}
!14 = !{!15}
!15 = distinct !{!15, !16, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!16 = distinct !{!16, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!17 = !{!18}
!18 = distinct !{!18, !19, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!19 = distinct !{!19, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!20 = !{!21}
!21 = distinct !{!21, !22, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!22 = distinct !{!22, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!23 = !{!24}
!24 = distinct !{!24, !25, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!25 = distinct !{!25, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!26 = !{!27}
!27 = distinct !{!27, !28, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!28 = distinct !{!28, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!29 = !{!30}
!30 = distinct !{!30, !31, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!31 = distinct !{!31, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!32 = !{!33}
!33 = distinct !{!33, !34, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!34 = distinct !{!34, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!35 = !{!36}
!36 = distinct !{!36, !37, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!37 = distinct !{!37, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!38 = !{!39}
!39 = distinct !{!39, !40, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!40 = distinct !{!40, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
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
!57 = distinct !{!57, !58, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!58 = distinct !{!58, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!59 = !{!60}
!60 = distinct !{!60, !61, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!61 = distinct !{!61, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!62 = !{!63}
!63 = distinct !{!63, !64, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!64 = distinct !{!64, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!65 = !{!66}
!66 = distinct !{!66, !67, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!67 = distinct !{!67, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!68 = !{!69}
!69 = distinct !{!69, !70, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!70 = distinct !{!70, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!71 = !{!72}
!72 = distinct !{!72, !73, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!73 = distinct !{!73, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!74 = !{!75}
!75 = distinct !{!75, !76, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!76 = distinct !{!76, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!77 = !{!78}
!78 = distinct !{!78, !79, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!79 = distinct !{!79, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!80 = !{!81}
!81 = distinct !{!81, !82, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!82 = distinct !{!82, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!83 = !{!84}
!84 = distinct !{!84, !85, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!85 = distinct !{!85, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!86 = !{!87}
!87 = distinct !{!87, !88, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!88 = distinct !{!88, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!89 = !{!90}
!90 = distinct !{!90, !91, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!91 = distinct !{!91, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!92 = !{!93}
!93 = distinct !{!93, !94, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!94 = distinct !{!94, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!95 = !{!96}
!96 = distinct !{!96, !97, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!97 = distinct !{!97, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!98 = !{!99}
!99 = distinct !{!99, !100, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!100 = distinct !{!100, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!101 = !{!102}
!102 = distinct !{!102, !103, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!103 = distinct !{!103, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!104 = !{!105}
!105 = distinct !{!105, !106, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!106 = distinct !{!106, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!107 = !{!108}
!108 = distinct !{!108, !109, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!109 = distinct !{!109, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!110 = !{!111}
!111 = distinct !{!111, !112, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!112 = distinct !{!112, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!113 = !{!114}
!114 = distinct !{!114, !115, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!115 = distinct !{!115, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!116 = !{!117}
!117 = distinct !{!117, !118, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!118 = distinct !{!118, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!119 = !{!120}
!120 = distinct !{!120, !121, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!121 = distinct !{!121, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!122 = !{!123}
!123 = distinct !{!123, !124, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!124 = distinct !{!124, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!125 = !{!126}
!126 = distinct !{!126, !127, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!127 = distinct !{!127, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!128 = !{!129}
!129 = distinct !{!129, !130, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!130 = distinct !{!130, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!131 = !{!132}
!132 = distinct !{!132, !133, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!133 = distinct !{!133, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!134 = !{!135}
!135 = distinct !{!135, !136, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!136 = distinct !{!136, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!137 = !{!138}
!138 = distinct !{!138, !139, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!139 = distinct !{!139, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
!140 = !{!141}
!141 = distinct !{!141, !142, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_: argument 0"}
!142 = distinct !{!142, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEPKS6_OS9_"}
!143 = !{!144}
!144 = distinct !{!144, !145, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_: argument 0"}
!145 = distinct !{!145, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_PKS6_"}
!146 = !{!147}
!147 = distinct !{!147, !148, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_: argument 0"}
!148 = distinct !{!148, !"_ZNSt3__1plIcNS_11char_traitsIcEENS_9allocatorIcEEEENS_12basic_stringIT_T0_T1_EEOS9_SA_"}
