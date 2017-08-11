; ModuleID = '/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/main.cpp'
source_filename = "/Users/Jonas/CLionProjects/HackerRank/XtremeJonasScript/src/main.cpp"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.12.0"

%"class.std::__1::basic_ostream" = type { i32 (...)**, %"class.std::__1::basic_ios.base" }
%"class.std::__1::basic_ios.base" = type <{ %"class.std::__1::ios_base", %"class.std::__1::basic_ostream"*, i32 }>
%"class.std::__1::ios_base" = type { i32 (...)**, i32, i64, i64, i32, i32, i8*, i8*, void (i32, %"class.std::__1::ios_base"*, i32)**, i32*, i64, i64, i64*, i64, i64, i8**, i64, i64 }
%"class.std::__1::basic_istream" = type { i32 (...)**, i64, %"class.std::__1::basic_ios.base" }
%"class.std::__1::locale::id" = type <{ %"struct.std::__1::once_flag", i32, [4 x i8] }>
%"struct.std::__1::once_flag" = type { i64 }
%"class.std::__1::basic_string" = type { %"class.std::__1::__compressed_pair" }
%"class.std::__1::__compressed_pair" = type { %"class.std::__1::__libcpp_compressed_pair_imp" }
%"class.std::__1::__libcpp_compressed_pair_imp" = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep" = type { %union.anon }
%union.anon = type { %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long" = type { i64, i64, i8* }
%"class.std::__1::ctype" = type <{ %"class.std::__1::locale::facet", i32*, i8, [7 x i8] }>
%"class.std::__1::locale::facet" = type { %"class.std::__1::__shared_count" }
%"class.std::__1::__shared_count" = type { i32 (...)**, i64 }
%"class.std::__1::locale" = type { %"class.std::__1::locale::__imp"* }
%"class.std::__1::locale::__imp" = type opaque
%"class.std::__1::basic_ios" = type <{ %"class.std::__1::ios_base", %"class.std::__1::basic_ostream"*, i32, [4 x i8] }>
%"class.std::__1::allocator" = type { i8 }
%"class.std::__1::__basic_string_common" = type { i8 }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw" = type { [3 x i64] }
%"class.std::__1::istreambuf_iterator" = type { %"class.std::__1::basic_streambuf"* }
%"class.std::__1::basic_streambuf" = type { i32 (...)**, %"class.std::__1::locale", i8*, i8*, i8*, i8*, i8*, i8* }
%"class.std::__1::basic_ifstream" = type { %"class.std::__1::basic_istream.base", %"class.std::__1::basic_filebuf", %"class.std::__1::basic_ios.base" }
%"class.std::__1::basic_istream.base" = type { i32 (...)**, i64 }
%"class.std::__1::basic_filebuf" = type <{ %"class.std::__1::basic_streambuf", i8*, i8*, i8*, [8 x i8], i64, i8*, i64, %struct.__sFILE*, %"class.std::__1::codecvt"*, %union.__mbstate_t, %union.__mbstate_t, i32, i32, i8, i8, i8, [5 x i8] }>
%struct.__sFILE = type { i8*, i32, i32, i16, i16, %struct.__sbuf, i32, i8*, i32 (i8*)*, i32 (i8*, i8*, i32)*, i64 (i8*, i64, i32)*, i32 (i8*, i8*, i32)*, %struct.__sbuf, %struct.__sFILEX*, i32, [3 x i8], [1 x i8], %struct.__sbuf, i32, i64 }
%struct.__sFILEX = type opaque
%struct.__sbuf = type { i8*, i32 }
%"class.std::__1::codecvt" = type { %"class.std::__1::locale::facet" }
%union.__mbstate_t = type { i64, [120 x i8] }
%class.Parser = type { %"class.std::__1::vector", i64, %"class.std::__1::vector.1", i8, i8, %class.Lexer* }
%"class.std::__1::vector" = type { %"class.std::__1::__vector_base" }
%"class.std::__1::__vector_base" = type { %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"*, %"class.std::__1::__compressed_pair.29" }
%"class.std::__1::shared_ptr" = type { %class.Statement*, %"class.std::__1::__shared_weak_count"* }
%class.Statement = type { %class.AstNode.base, [6 x i8] }
%class.AstNode.base = type <{ i32 (...)**, i32, i32, i64, %class.AstNode*, %class.DeclStmt*, i8, [3 x i8], i32, %"class.std::__1::vector.1", %struct.TypeSpecifier, %struct.TypeSpecifier, i8, [7 x i8], %"class.std::__1::basic_string", i8, i8 }>
%class.AstNode = type <{ i32 (...)**, i32, i32, i64, %class.AstNode*, %class.DeclStmt*, i8, [3 x i8], i32, %"class.std::__1::vector.1", %struct.TypeSpecifier, %struct.TypeSpecifier, i8, [7 x i8], %"class.std::__1::basic_string", i8, i8, [6 x i8] }>
%class.DeclStmt = type <{ %class.Statement.base, [6 x i8], %"class.std::__1::vector.1", %"class.std::__1::vector.43", %"class.std::__1::vector.1", %"class.std::__1::vector.49", i8, i8, [6 x i8] }>
%class.Statement.base = type { %class.AstNode.base }
%"class.std::__1::vector.43" = type { %"class.std::__1::__vector_base.44" }
%"class.std::__1::__vector_base.44" = type { %"struct.std::__1::pair"*, %"struct.std::__1::pair"*, %"class.std::__1::__compressed_pair.45" }
%"struct.std::__1::pair" = type opaque
%"class.std::__1::__compressed_pair.45" = type { %"class.std::__1::__libcpp_compressed_pair_imp.46" }
%"class.std::__1::__libcpp_compressed_pair_imp.46" = type { %"struct.std::__1::pair"* }
%"class.std::__1::vector.49" = type { i64*, i64, %"class.std::__1::__compressed_pair.50" }
%"class.std::__1::__compressed_pair.50" = type { %"class.std::__1::__libcpp_compressed_pair_imp.51" }
%"class.std::__1::__libcpp_compressed_pair_imp.51" = type { i64 }
%struct.TypeSpecifier = type { i32, %"class.std::__1::basic_string", %"class.std::__1::shared_ptr.7", i8, i8, i8, i8, i64, i32, i8, i8, %"class.std::__1::basic_string", i8, [7 x i8], %"class.std::__1::unordered_map", i8, i8, i8, i8, i8, i8, i8, i32, %"class.std::__1::vector.1", %"class.std::__1::vector.22", %union.anon.28 }
%"class.std::__1::shared_ptr.7" = type { %class.Expression*, %"class.std::__1::__shared_weak_count"* }
%class.Expression = type { %class.Statement.base, %"class.std::__1::shared_ptr.7", i8, %"class.std::__1::shared_ptr.7", i8, %"class.std::__1::unique_ptr.40", i8, %struct.TypeSpecifier }
%"class.std::__1::unique_ptr.40" = type { %"class.std::__1::__compressed_pair.41" }
%"class.std::__1::__compressed_pair.41" = type { %"class.std::__1::__libcpp_compressed_pair_imp.42" }
%"class.std::__1::__libcpp_compressed_pair_imp.42" = type { %struct.Variant* }
%struct.Variant = type <{ %union.anon.33, i8, [7 x i8], %"class.std::__1::basic_string", %struct.TypeSpecifier, i8, i8, i8, [5 x i8] }>
%union.anon.33 = type { i64 }
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
%"class.std::__1::vector.22" = type { %"class.std::__1::__vector_base.23" }
%"class.std::__1::__vector_base.23" = type { %struct.TypeSpecifier*, %struct.TypeSpecifier*, %"class.std::__1::__compressed_pair.24" }
%"class.std::__1::__compressed_pair.24" = type { %"class.std::__1::__libcpp_compressed_pair_imp.25" }
%"class.std::__1::__libcpp_compressed_pair_imp.25" = type { %struct.TypeSpecifier* }
%union.anon.28 = type { %struct.TypeSpecifier* }
%"class.std::__1::__shared_weak_count" = type { %"class.std::__1::__shared_count", i64 }
%"class.std::__1::__compressed_pair.29" = type { %"class.std::__1::__libcpp_compressed_pair_imp.30" }
%"class.std::__1::__libcpp_compressed_pair_imp.30" = type { %"class.std::__1::shared_ptr"* }
%"class.std::__1::vector.1" = type { %"class.std::__1::__vector_base.2" }
%"class.std::__1::__vector_base.2" = type { %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"*, %"class.std::__1::__compressed_pair.3" }
%"class.std::__1::__compressed_pair.3" = type { %"class.std::__1::__libcpp_compressed_pair_imp.4" }
%"class.std::__1::__libcpp_compressed_pair_imp.4" = type { %"class.std::__1::basic_string"* }
%class.Lexer = type { i32, i32, i32, i32, i32, [4 x i8], %class.Token, %"class.std::__1::vector.34", %"class.std::__1::basic_string" }
%class.Token = type { %struct.Variant, i32, i32, i32, i32, i32, i32 }
%"class.std::__1::vector.34" = type { %"class.std::__1::__vector_base.35" }
%"class.std::__1::__vector_base.35" = type { %class.Token*, %class.Token*, %"class.std::__1::__compressed_pair.36" }
%"class.std::__1::__compressed_pair.36" = type { %"class.std::__1::__libcpp_compressed_pair_imp.37" }
%"class.std::__1::__libcpp_compressed_pair_imp.37" = type { %class.Token* }
%class.RuntimeError = type { %"class.std::exception", %"class.std::__1::basic_string" }
%"class.std::exception" = type { i32 (...)** }
%class.ParseError = type { %"class.std::exception", %"class.std::__1::basic_string" }
%"struct.std::__1::iterator" = type { i8 }
%"class.std::__1::allocator.5" = type { i8 }
%"struct.std::__1::integral_constant" = type { i8 }
%"struct.std::__1::__has_destroy" = type { i8 }
%"class.std::__1::allocator.31" = type { i8 }
%"struct.std::__1::__has_destroy.54" = type { i8 }
%"class.std::__1::__libcpp_compressed_pair_imp.57" = type { %struct.__sFILE*, i32 (%struct.__sFILE*)* }
%"class.std::__1::__compressed_pair.56" = type { %"class.std::__1::__libcpp_compressed_pair_imp.57" }
%"class.std::__1::unique_ptr.55" = type { %"class.std::__1::__compressed_pair.56" }
%"struct.std::__1::__less" = type { i8 }
%"class.std::__1::fpos" = type { %union.__mbstate_t, i64 }
%"class.std::bad_cast" = type { %"class.std::exception" }
%"struct.std::__1::__less.58" = type { i8 }
%"class.std::__1::ostreambuf_iterator" = type { %"class.std::__1::basic_streambuf"* }
%"class.std::__1::basic_ostream<char, std::__1::char_traits<char> >::sentry" = type { i8, %"class.std::__1::basic_ostream"* }
%"struct.std::__1::iterator.59" = type { i8 }
%"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short" = type { %union.anon.0, [23 x i8] }
%union.anon.0 = type { i8 }
%"struct.std::__1::__has_max_size" = type { i8 }
%"class.std::__1::basic_istream<char, std::__1::char_traits<char> >::sentry" = type { i8 }

@_ZN4llvm24DisableABIBreakingChecksE = external global i32, align 4
@_ZN4llvm30VerifyDisableABIBreakingChecksE = weak hidden global i32* @_ZN4llvm24DisableABIBreakingChecksE, align 8
@_ZNSt3__14coutE = external global %"class.std::__1::basic_ostream", align 8
@.str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@_ZNSt3__13cinE = external global %"class.std::__1::basic_istream", align 8
@.str.1 = private unnamed_addr constant [38 x i8] c"/Users/Jonas/CDotProjects/ex/main.dot\00", align 1
@_ZTVN10__cxxabiv120__si_class_type_infoE = external global i8*
@_ZTS10ParseError = linkonce_odr constant [13 x i8] c"10ParseError\00"
@_ZTISt9exception = external constant i8*
@_ZTI10ParseError = linkonce_odr constant { i8*, i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv120__si_class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @_ZTS10ParseError, i32 0, i32 0), i8* bitcast (i8** @_ZTISt9exception to i8*) }
@_ZTS12RuntimeError = linkonce_odr constant [15 x i8] c"12RuntimeError\00"
@_ZTI12RuntimeError = linkonce_odr constant { i8*, i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv120__si_class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @_ZTS12RuntimeError, i32 0, i32 0), i8* bitcast (i8** @_ZTISt9exception to i8*) }
@_ZTVNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE = linkonce_odr unnamed_addr constant [10 x i8*] [i8* inttoptr (i64 424 to i8*), i8* null, i8* bitcast ({ i8*, i8*, i8* }* @_ZTINSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE to i8*), i8* bitcast (void (%"class.std::__1::basic_ifstream"*)* @_ZNSt3__114basic_ifstreamIcNS_11char_traitsIcEEED1Ev to i8*), i8* bitcast (void (%"class.std::__1::basic_ifstream"*)* @_ZNSt3__114basic_ifstreamIcNS_11char_traitsIcEEED0Ev to i8*), i8* inttoptr (i64 -424 to i8*), i8* inttoptr (i64 -424 to i8*), i8* bitcast ({ i8*, i8*, i8* }* @_ZTINSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE to i8*), i8* bitcast (void (%"class.std::__1::basic_ifstream"*)* @_ZTv0_n24_NSt3__114basic_ifstreamIcNS_11char_traitsIcEEED1Ev to i8*), i8* bitcast (void (%"class.std::__1::basic_ifstream"*)* @_ZTv0_n24_NSt3__114basic_ifstreamIcNS_11char_traitsIcEEED0Ev to i8*)], align 8
@_ZTTNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE = linkonce_odr unnamed_addr constant [4 x i8*] [i8* bitcast (i8** getelementptr inbounds ([10 x i8*], [10 x i8*]* @_ZTVNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE, i32 0, i32 3) to i8*), i8* bitcast (i8** getelementptr inbounds ([10 x i8*], [10 x i8*]* @_ZTCNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE0_NS_13basic_istreamIcS2_EE, i32 0, i32 3) to i8*), i8* bitcast (i8** getelementptr inbounds ([10 x i8*], [10 x i8*]* @_ZTCNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE0_NS_13basic_istreamIcS2_EE, i32 0, i32 8) to i8*), i8* bitcast (i8** getelementptr inbounds ([10 x i8*], [10 x i8*]* @_ZTVNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE, i32 0, i32 8) to i8*)]
@_ZTCNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE0_NS_13basic_istreamIcS2_EE = linkonce_odr unnamed_addr constant [10 x i8*] [i8* inttoptr (i64 424 to i8*), i8* null, i8* bitcast (i8** @_ZTINSt3__113basic_istreamIcNS_11char_traitsIcEEEE to i8*), i8* bitcast (void (%"class.std::__1::basic_istream"*)* @_ZNSt3__113basic_istreamIcNS_11char_traitsIcEEED1Ev to i8*), i8* bitcast (void (%"class.std::__1::basic_istream"*)* @_ZNSt3__113basic_istreamIcNS_11char_traitsIcEEED0Ev to i8*), i8* inttoptr (i64 -424 to i8*), i8* inttoptr (i64 -424 to i8*), i8* bitcast (i8** @_ZTINSt3__113basic_istreamIcNS_11char_traitsIcEEEE to i8*), i8* bitcast (void (%"class.std::__1::basic_istream"*)* @_ZTv0_n24_NSt3__113basic_istreamIcNS_11char_traitsIcEEED1Ev to i8*), i8* bitcast (void (%"class.std::__1::basic_istream"*)* @_ZTv0_n24_NSt3__113basic_istreamIcNS_11char_traitsIcEEED0Ev to i8*)]
@_ZTINSt3__113basic_istreamIcNS_11char_traitsIcEEEE = external constant i8*
@_ZTSNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE = linkonce_odr constant [48 x i8] c"NSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE\00"
@_ZTINSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE = linkonce_odr constant { i8*, i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv120__si_class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([48 x i8], [48 x i8]* @_ZTSNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE, i32 0, i32 0), i8* bitcast (i8** @_ZTINSt3__113basic_istreamIcNS_11char_traitsIcEEEE to i8*) }
@_ZTVNSt3__113basic_filebufIcNS_11char_traitsIcEEEE = linkonce_odr unnamed_addr constant [16 x i8*] [i8* null, i8* bitcast ({ i8*, i8*, i8* }* @_ZTINSt3__113basic_filebufIcNS_11char_traitsIcEEEE to i8*), i8* bitcast (void (%"class.std::__1::basic_filebuf"*)* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEED1Ev to i8*), i8* bitcast (void (%"class.std::__1::basic_filebuf"*)* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEED0Ev to i8*), i8* bitcast (void (%"class.std::__1::basic_filebuf"*, %"class.std::__1::locale"*)* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE5imbueERKNS_6localeE to i8*), i8* bitcast (%"class.std::__1::basic_streambuf"* (%"class.std::__1::basic_filebuf"*, i8*, i64)* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE6setbufEPcl to i8*), i8* bitcast (void (%"class.std::__1::fpos"*, %"class.std::__1::basic_filebuf"*, i64, i32, i32)* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE7seekoffExNS_8ios_base7seekdirEj to i8*), i8* bitcast (void (%"class.std::__1::fpos"*, %"class.std::__1::basic_filebuf"*, %"class.std::__1::fpos"*, i32)* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE7seekposENS_4fposI11__mbstate_tEEj to i8*), i8* bitcast (i32 (%"class.std::__1::basic_filebuf"*)* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE4syncEv to i8*), i8* bitcast (i64 (%"class.std::__1::basic_streambuf"*)* @_ZNSt3__115basic_streambufIcNS_11char_traitsIcEEE9showmanycEv to i8*), i8* bitcast (i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)* @_ZNSt3__115basic_streambufIcNS_11char_traitsIcEEE6xsgetnEPcl to i8*), i8* bitcast (i32 (%"class.std::__1::basic_filebuf"*)* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE9underflowEv to i8*), i8* bitcast (i32 (%"class.std::__1::basic_streambuf"*)* @_ZNSt3__115basic_streambufIcNS_11char_traitsIcEEE5uflowEv to i8*), i8* bitcast (i32 (%"class.std::__1::basic_filebuf"*, i32)* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE9pbackfailEi to i8*), i8* bitcast (i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)* @_ZNSt3__115basic_streambufIcNS_11char_traitsIcEEE6xsputnEPKcl to i8*), i8* bitcast (i32 (%"class.std::__1::basic_filebuf"*, i32)* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE8overflowEi to i8*)], align 8
@_ZTSNSt3__113basic_filebufIcNS_11char_traitsIcEEEE = linkonce_odr constant [47 x i8] c"NSt3__113basic_filebufIcNS_11char_traitsIcEEEE\00"
@_ZTINSt3__115basic_streambufIcNS_11char_traitsIcEEEE = external constant i8*
@_ZTINSt3__113basic_filebufIcNS_11char_traitsIcEEEE = linkonce_odr constant { i8*, i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv120__si_class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([47 x i8], [47 x i8]* @_ZTSNSt3__113basic_filebufIcNS_11char_traitsIcEEEE, i32 0, i32 0), i8* bitcast (i8** @_ZTINSt3__115basic_streambufIcNS_11char_traitsIcEEEE to i8*) }
@_ZNSt3__17codecvtIcc11__mbstate_tE2idE = external global %"class.std::__1::locale::id", align 8
@_ZTISt8bad_cast = external constant i8*
@_ZNSt3__15ctypeIcE2idE = external global %"class.std::__1::locale::id", align 8
@_ZTVNSt3__19basic_iosIcNS_11char_traitsIcEEEE = external unnamed_addr constant [4 x i8*]
@_ZTVNSt3__18ios_baseE = external unnamed_addr constant [4 x i8*]
@.str.2 = private unnamed_addr constant [2 x i8] c"w\00", align 1
@.str.3 = private unnamed_addr constant [2 x i8] c"a\00", align 1
@.str.4 = private unnamed_addr constant [2 x i8] c"r\00", align 1
@.str.5 = private unnamed_addr constant [3 x i8] c"r+\00", align 1
@.str.6 = private unnamed_addr constant [3 x i8] c"w+\00", align 1
@.str.7 = private unnamed_addr constant [3 x i8] c"a+\00", align 1
@.str.8 = private unnamed_addr constant [3 x i8] c"wb\00", align 1
@.str.9 = private unnamed_addr constant [3 x i8] c"ab\00", align 1
@.str.10 = private unnamed_addr constant [3 x i8] c"rb\00", align 1
@.str.11 = private unnamed_addr constant [4 x i8] c"r+b\00", align 1
@.str.12 = private unnamed_addr constant [4 x i8] c"w+b\00", align 1
@.str.13 = private unnamed_addr constant [4 x i8] c"a+b\00", align 1

; Function Attrs: ssp uwtable
define void @_Z7handlerv() #0 {
  %1 = alloca [20 x i8*], align 16
  %2 = alloca i32, align 4
  %3 = alloca i8**, align 8
  %4 = alloca i32, align 4
  %5 = getelementptr inbounds [20 x i8*], [20 x i8*]* %1, i32 0, i32 0
  %6 = call i32 @backtrace(i8** %5, i32 20)
  store i32 %6, i32* %2, align 4
  %7 = getelementptr inbounds [20 x i8*], [20 x i8*]* %1, i32 0, i32 0
  %8 = load i32, i32* %2, align 4
  %9 = call i8** @backtrace_symbols(i8** %7, i32 %8)
  store i8** %9, i8*** %3, align 8
  store i32 0, i32* %4, align 4
  br label %10

; <label>:10:                                     ; preds = %22, %0
  %11 = load i32, i32* %4, align 4
  %12 = load i32, i32* %2, align 4
  %13 = icmp slt i32 %11, %12
  br i1 %13, label %14, label %25

; <label>:14:                                     ; preds = %10
  %15 = load i32, i32* %4, align 4
  %16 = sext i32 %15 to i64
  %17 = load i8**, i8*** %3, align 8
  %18 = getelementptr inbounds i8*, i8** %17, i64 %16
  %19 = load i8*, i8** %18, align 8
  %20 = call dereferenceable(160) %"class.std::__1::basic_ostream"* @_ZNSt3__1lsINS_11char_traitsIcEEEERNS_13basic_ostreamIcT_EES6_PKc(%"class.std::__1::basic_ostream"* dereferenceable(160) @_ZNSt3__14coutE, i8* %19)
  %21 = call dereferenceable(160) %"class.std::__1::basic_ostream"* @_ZNSt3__1lsINS_11char_traitsIcEEEERNS_13basic_ostreamIcT_EES6_PKc(%"class.std::__1::basic_ostream"* dereferenceable(160) %20, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0))
  br label %22

; <label>:22:                                     ; preds = %14
  %23 = load i32, i32* %4, align 4
  %24 = add nsw i32 %23, 1
  store i32 %24, i32* %4, align 4
  br label %10

; <label>:25:                                     ; preds = %10
  %26 = load i8**, i8*** %3, align 8
  %27 = bitcast i8** %26 to i8*
  call void @free(i8* %27)
  call void @exit(i32 1) #14
  unreachable
                                                  ; No predecessors!
  ret void
}

declare i32 @backtrace(i8**, i32) #1

declare i8** @backtrace_symbols(i8**, i32) #1

; Function Attrs: ssp uwtable
define linkonce_odr dereferenceable(160) %"class.std::__1::basic_ostream"* @_ZNSt3__1lsINS_11char_traitsIcEEEERNS_13basic_ostreamIcT_EES6_PKc(%"class.std::__1::basic_ostream"* dereferenceable(160), i8*) #0 {
  %3 = alloca %"class.std::__1::basic_ostream"*, align 8
  %4 = alloca i8*, align 8
  store %"class.std::__1::basic_ostream"* %0, %"class.std::__1::basic_ostream"** %3, align 8
  store i8* %1, i8** %4, align 8
  %5 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %3, align 8
  %6 = load i8*, i8** %4, align 8
  %7 = load i8*, i8** %4, align 8
  %8 = call i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8* %7)
  %9 = call dereferenceable(160) %"class.std::__1::basic_ostream"* @_ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m(%"class.std::__1::basic_ostream"* dereferenceable(160) %5, i8* %6, i64 %8)
  ret %"class.std::__1::basic_ostream"* %9
}

declare void @free(i8*) #1

; Function Attrs: noreturn
declare void @exit(i32) #2

; Function Attrs: ssp uwtable
define void @_Z12cin_readlinev(%"class.std::__1::basic_string"* noalias sret) #0 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"class.std::__1::ctype"*, align 8
  %3 = alloca i8, align 1
  %4 = alloca %"class.std::__1::locale"*, align 8
  %5 = alloca %"class.std::__1::basic_ios"*, align 8
  %6 = alloca i8, align 1
  %7 = alloca %"class.std::__1::locale", align 8
  %8 = alloca i8*
  %9 = alloca i32
  %10 = alloca %"class.std::__1::basic_istream"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %14 = alloca %"class.std::__1::basic_string"*, align 8
  %15 = alloca [3 x i64]*, align 8
  %16 = alloca i32, align 4
  %17 = alloca %"class.std::__1::allocator"*, align 8
  %18 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %19 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %20 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %21 = alloca %"class.std::__1::basic_string"*, align 8
  %22 = alloca %"class.std::__1::basic_string"*, align 8
  %23 = alloca i1, align 1
  %24 = alloca i8*
  %25 = alloca i32
  store i1 false, i1* %23, align 1
  store %"class.std::__1::basic_string"* %0, %"class.std::__1::basic_string"** %22, align 8
  %26 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %22, align 8
  store %"class.std::__1::basic_string"* %26, %"class.std::__1::basic_string"** %21, align 8
  %27 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %21, align 8
  %28 = bitcast %"class.std::__1::basic_string"* %27 to %"class.std::__1::__basic_string_common"*
  %29 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %27, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %29, %"class.std::__1::__compressed_pair"** %20, align 8
  %30 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %20, align 8
  store %"class.std::__1::__compressed_pair"* %30, %"class.std::__1::__compressed_pair"** %19, align 8
  %31 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %19, align 8
  %32 = bitcast %"class.std::__1::__compressed_pair"* %31 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %32, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8
  %33 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %18, align 8
  %34 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %33 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %34, %"class.std::__1::allocator"** %17, align 8
  %35 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %17, align 8
  %36 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %33, i32 0, i32 0
  %37 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %36 to i8*
  call void @llvm.memset.p0i8.i64(i8* %37, i8 0, i64 24, i32 8, i1 false) #15
  store %"class.std::__1::basic_string"* %27, %"class.std::__1::basic_string"** %14, align 8
  %38 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %14, align 8
  %39 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %38, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %39, %"class.std::__1::__compressed_pair"** %13, align 8
  %40 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %13, align 8
  %41 = bitcast %"class.std::__1::__compressed_pair"* %40 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %41, %"class.std::__1::__libcpp_compressed_pair_imp"** %12, align 8
  %42 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %12, align 8
  %43 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %42, i32 0, i32 0
  %44 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %43, i32 0, i32 0
  %45 = bitcast %union.anon* %44 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %46 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %45, i32 0, i32 0
  store [3 x i64]* %46, [3 x i64]** %15, align 8
  store i32 0, i32* %16, align 4
  br label %47

; <label>:47:                                     ; preds = %50, %1
  %48 = load i32, i32* %16, align 4
  %49 = icmp ult i32 %48, 3
  br i1 %49, label %50, label %57

; <label>:50:                                     ; preds = %47
  %51 = load i32, i32* %16, align 4
  %52 = zext i32 %51 to i64
  %53 = load [3 x i64]*, [3 x i64]** %15, align 8
  %54 = getelementptr inbounds [3 x i64], [3 x i64]* %53, i64 0, i64 %52
  store i64 0, i64* %54, align 8
  %55 = load i32, i32* %16, align 4
  %56 = add i32 %55, 1
  store i32 %56, i32* %16, align 4
  br label %47

; <label>:57:                                     ; preds = %47
  store %"class.std::__1::basic_istream"* @_ZNSt3__13cinE, %"class.std::__1::basic_istream"** %10, align 8
  store %"class.std::__1::basic_string"* %0, %"class.std::__1::basic_string"** %11, align 8
  %58 = load %"class.std::__1::basic_istream"*, %"class.std::__1::basic_istream"** %10, align 8
  %59 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8
  %60 = load %"class.std::__1::basic_istream"*, %"class.std::__1::basic_istream"** %10, align 8
  %61 = bitcast %"class.std::__1::basic_istream"* %60 to i8**
  %62 = load i8*, i8** %61, align 8
  %63 = getelementptr i8, i8* %62, i64 -24
  %64 = bitcast i8* %63 to i64*
  %65 = load i64, i64* %64, align 8
  %66 = bitcast %"class.std::__1::basic_istream"* %60 to i8*
  %67 = getelementptr inbounds i8, i8* %66, i64 %65
  %68 = bitcast i8* %67 to %"class.std::__1::basic_ios"*
  store %"class.std::__1::basic_ios"* %68, %"class.std::__1::basic_ios"** %5, align 8
  store i8 10, i8* %6, align 1
  %69 = load %"class.std::__1::basic_ios"*, %"class.std::__1::basic_ios"** %5, align 8
  %70 = bitcast %"class.std::__1::basic_ios"* %69 to %"class.std::__1::ios_base"*
  invoke void @_ZNKSt3__18ios_base6getlocEv(%"class.std::__1::locale"* sret %7, %"class.std::__1::ios_base"* %70)
          to label %71 unwind label %97

; <label>:71:                                     ; preds = %57
  store %"class.std::__1::locale"* %7, %"class.std::__1::locale"** %4, align 8
  %72 = load %"class.std::__1::locale"*, %"class.std::__1::locale"** %4, align 8
  %73 = invoke %"class.std::__1::locale::facet"* @_ZNKSt3__16locale9use_facetERNS0_2idE(%"class.std::__1::locale"* %72, %"class.std::__1::locale::id"* dereferenceable(16) @_ZNSt3__15ctypeIcE2idE)
          to label %74 unwind label %84

; <label>:74:                                     ; preds = %71
  %75 = bitcast %"class.std::__1::locale::facet"* %73 to %"class.std::__1::ctype"*
  %76 = load i8, i8* %6, align 1
  store %"class.std::__1::ctype"* %75, %"class.std::__1::ctype"** %2, align 8
  store i8 %76, i8* %3, align 1
  %77 = load %"class.std::__1::ctype"*, %"class.std::__1::ctype"** %2, align 8
  %78 = bitcast %"class.std::__1::ctype"* %77 to i8 (%"class.std::__1::ctype"*, i8)***
  %79 = load i8 (%"class.std::__1::ctype"*, i8)**, i8 (%"class.std::__1::ctype"*, i8)*** %78, align 8
  %80 = getelementptr inbounds i8 (%"class.std::__1::ctype"*, i8)*, i8 (%"class.std::__1::ctype"*, i8)** %79, i64 7
  %81 = load i8 (%"class.std::__1::ctype"*, i8)*, i8 (%"class.std::__1::ctype"*, i8)** %80, align 8
  %82 = load i8, i8* %3, align 1
  %83 = invoke signext i8 %81(%"class.std::__1::ctype"* %77, i8 signext %82)
          to label %92 unwind label %84

; <label>:84:                                     ; preds = %74, %71
  %85 = landingpad { i8*, i32 }
          cleanup
  %86 = extractvalue { i8*, i32 } %85, 0
  store i8* %86, i8** %8, align 8
  %87 = extractvalue { i8*, i32 } %85, 1
  store i32 %87, i32* %9, align 4
  call void @_ZNSt3__16localeD1Ev(%"class.std::__1::locale"* %7) #15
  %88 = load i8*, i8** %8, align 8
  %89 = load i32, i32* %9, align 4
  %90 = insertvalue { i8*, i32 } undef, i8* %88, 0
  %91 = insertvalue { i8*, i32 } %90, i32 %89, 1
  br label %99

; <label>:92:                                     ; preds = %74
  call void @_ZNSt3__16localeD1Ev(%"class.std::__1::locale"* %7) #15
  %93 = invoke dereferenceable(168) %"class.std::__1::basic_istream"* @_ZNSt3__17getlineIcNS_11char_traitsIcEENS_9allocatorIcEEEERNS_13basic_istreamIT_T0_EES9_RNS_12basic_stringIS6_S7_T1_EES6_(%"class.std::__1::basic_istream"* dereferenceable(168) %58, %"class.std::__1::basic_string"* dereferenceable(24) %59, i8 signext %83)
          to label %94 unwind label %97

; <label>:94:                                     ; preds = %92
  br label %95

; <label>:95:                                     ; preds = %94
  store i1 true, i1* %23, align 1
  %96 = load i1, i1* %23, align 1
  br i1 %96, label %104, label %103

; <label>:97:                                     ; preds = %92, %57
  %98 = landingpad { i8*, i32 }
          cleanup
  br label %99

; <label>:99:                                     ; preds = %84, %97
  %100 = phi { i8*, i32 } [ %98, %97 ], [ %91, %84 ]
  %101 = extractvalue { i8*, i32 } %100, 0
  store i8* %101, i8** %24, align 8
  %102 = extractvalue { i8*, i32 } %100, 1
  store i32 %102, i32* %25, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %0) #15
  br label %105

; <label>:103:                                    ; preds = %95
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %0) #15
  br label %104

; <label>:104:                                    ; preds = %103, %95
  ret void

; <label>:105:                                    ; preds = %99
  %106 = load i8*, i8** %24, align 8
  %107 = load i32, i32* %25, align 4
  %108 = insertvalue { i8*, i32 } undef, i8* %106, 0
  %109 = insertvalue { i8*, i32 } %108, i32 %107, 1
  resume { i8*, i32 } %109
}

declare i32 @__gxx_personality_v0(...)

; Function Attrs: nounwind
declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"*) unnamed_addr #3

; Function Attrs: norecurse ssp uwtable
define i32 @main() #4 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %1 = alloca %"class.std::__1::basic_ostream"*, align 8
  %2 = alloca %"class.std::__1::basic_ostream"* (%"class.std::__1::basic_ostream"*)*, align 8
  %3 = alloca %"class.std::__1::basic_ostream"*, align 8
  %4 = alloca %"class.std::__1::basic_ostream"* (%"class.std::__1::basic_ostream"*)*, align 8
  %5 = alloca %"class.std::__1::allocator"*, align 8
  %6 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %7 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %8 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %9 = alloca %"class.std::__1::istreambuf_iterator", align 8
  %10 = alloca %"class.std::__1::istreambuf_iterator", align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca %"class.std::__1::istreambuf_iterator", align 8
  %13 = alloca %"class.std::__1::istreambuf_iterator", align 8
  %14 = alloca %"class.std::__1::istreambuf_iterator", align 8
  %15 = alloca %"class.std::__1::istreambuf_iterator", align 8
  %16 = alloca %"class.std::__1::basic_string"*, align 8
  %17 = alloca %"class.std::__1::istreambuf_iterator"*, align 8
  %18 = alloca %"class.std::__1::istreambuf_iterator"*, align 8
  %19 = alloca %"class.std::__1::ios_base"*, align 8
  %20 = alloca %"class.std::__1::basic_ios"*, align 8
  %21 = alloca %"class.std::__1::istreambuf_iterator"*, align 8
  %22 = alloca %"class.std::__1::basic_istream"*, align 8
  %23 = alloca %"class.std::__1::istreambuf_iterator"*, align 8
  %24 = alloca %"class.std::__1::basic_istream"*, align 8
  %25 = alloca %"class.std::__1::ios_base"*, align 8
  %26 = alloca i32, align 4
  %27 = alloca %"class.std::__1::basic_ios"*, align 8
  %28 = alloca i32, align 4
  %29 = alloca %"class.std::__1::basic_ios"*, align 8
  %30 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %31 = alloca %"class.std::__1::basic_istream"*, align 8
  %32 = alloca i8**, align 8
  %33 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %34 = alloca %"class.std::__1::ios_base"*, align 8
  %35 = alloca %"class.std::__1::basic_ios"*, align 8
  %36 = alloca %"class.std::__1::basic_ifstream"*, align 8
  %37 = alloca i8*, align 8
  %38 = alloca i32, align 4
  %39 = alloca i8*
  %40 = alloca i32
  %41 = alloca i32, align 4
  %42 = alloca %"class.std::__1::basic_ifstream", align 8
  %43 = alloca %"class.std::__1::basic_string", align 8
  %44 = alloca %"class.std::__1::istreambuf_iterator", align 8
  %45 = alloca %"class.std::__1::istreambuf_iterator", align 8
  %46 = alloca i8*
  %47 = alloca i32
  %48 = alloca %class.Parser, align 8
  %49 = alloca %"class.std::__1::basic_string", align 8
  %50 = alloca %class.RuntimeError*, align 8
  %51 = alloca %class.ParseError*, align 8
  store i32 0, i32* %41, align 4
  %52 = call void ()* @_ZSt13set_terminatePFvvE(void ()* @_Z7handlerv) #15
  store %"class.std::__1::basic_ifstream"* %42, %"class.std::__1::basic_ifstream"** %36, align 8
  store i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.1, i32 0, i32 0), i8** %37, align 8
  store i32 8, i32* %38, align 4
  %53 = load %"class.std::__1::basic_ifstream"*, %"class.std::__1::basic_ifstream"** %36, align 8
  %54 = bitcast %"class.std::__1::basic_ifstream"* %53 to i8*
  %55 = getelementptr inbounds i8, i8* %54, i64 424
  %56 = bitcast i8* %55 to %"class.std::__1::basic_ios"*
  store %"class.std::__1::basic_ios"* %56, %"class.std::__1::basic_ios"** %35, align 8
  %57 = load %"class.std::__1::basic_ios"*, %"class.std::__1::basic_ios"** %35, align 8
  %58 = bitcast %"class.std::__1::basic_ios"* %57 to %"class.std::__1::ios_base"*
  store %"class.std::__1::ios_base"* %58, %"class.std::__1::ios_base"** %34, align 8
  %59 = load %"class.std::__1::ios_base"*, %"class.std::__1::ios_base"** %34, align 8
  %60 = bitcast %"class.std::__1::ios_base"* %59 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([4 x i8*], [4 x i8*]* @_ZTVNSt3__18ios_baseE, i32 0, i32 2) to i32 (...)**), i32 (...)*** %60, align 8
  %61 = bitcast %"class.std::__1::basic_ios"* %57 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([4 x i8*], [4 x i8*]* @_ZTVNSt3__19basic_iosIcNS_11char_traitsIcEEEE, i32 0, i32 2) to i32 (...)**), i32 (...)*** %61, align 8
  %62 = bitcast %"class.std::__1::basic_ifstream"* %53 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([10 x i8*], [10 x i8*]* @_ZTVNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE, i32 0, i32 3) to i32 (...)**), i32 (...)*** %62, align 8
  %63 = bitcast %"class.std::__1::basic_ifstream"* %53 to i8*
  %64 = getelementptr inbounds i8, i8* %63, i64 424
  %65 = bitcast i8* %64 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([10 x i8*], [10 x i8*]* @_ZTVNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE, i32 0, i32 8) to i32 (...)**), i32 (...)*** %65, align 8
  %66 = bitcast %"class.std::__1::basic_ifstream"* %53 to %"class.std::__1::basic_istream"*
  %67 = getelementptr inbounds %"class.std::__1::basic_ifstream", %"class.std::__1::basic_ifstream"* %53, i32 0, i32 1
  %68 = bitcast %"class.std::__1::basic_filebuf"* %67 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_istream"* %66, %"class.std::__1::basic_istream"** %31, align 8
  store i8** getelementptr inbounds ([4 x i8*], [4 x i8*]* @_ZTTNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE, i64 0, i64 1), i8*** %32, align 8
  store %"class.std::__1::basic_streambuf"* %68, %"class.std::__1::basic_streambuf"** %33, align 8
  %69 = load %"class.std::__1::basic_istream"*, %"class.std::__1::basic_istream"** %31, align 8
  %70 = load i8**, i8*** %32, align 8
  %71 = load i8*, i8** %70, align 8
  %72 = bitcast %"class.std::__1::basic_istream"* %69 to i32 (...)***
  %73 = bitcast i8* %71 to i32 (...)**
  store i32 (...)** %73, i32 (...)*** %72, align 8
  %74 = getelementptr inbounds i8*, i8** %70, i64 1
  %75 = load i8*, i8** %74, align 8
  %76 = bitcast %"class.std::__1::basic_istream"* %69 to i8**
  %77 = load i8*, i8** %76, align 8
  %78 = getelementptr i8, i8* %77, i64 -24
  %79 = bitcast i8* %78 to i64*
  %80 = load i64, i64* %79, align 8
  %81 = bitcast %"class.std::__1::basic_istream"* %69 to i8*
  %82 = getelementptr inbounds i8, i8* %81, i64 %80
  %83 = bitcast i8* %82 to i32 (...)***
  %84 = bitcast i8* %75 to i32 (...)**
  store i32 (...)** %84, i32 (...)*** %83, align 8
  %85 = getelementptr inbounds %"class.std::__1::basic_istream", %"class.std::__1::basic_istream"* %69, i32 0, i32 1
  store i64 0, i64* %85, align 8
  %86 = bitcast %"class.std::__1::basic_istream"* %69 to i8**
  %87 = load i8*, i8** %86, align 8
  %88 = getelementptr i8, i8* %87, i64 -24
  %89 = bitcast i8* %88 to i64*
  %90 = load i64, i64* %89, align 8
  %91 = bitcast %"class.std::__1::basic_istream"* %69 to i8*
  %92 = getelementptr inbounds i8, i8* %91, i64 %90
  %93 = bitcast i8* %92 to %"class.std::__1::basic_ios"*
  %94 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %33, align 8
  store %"class.std::__1::basic_ios"* %93, %"class.std::__1::basic_ios"** %29, align 8
  store %"class.std::__1::basic_streambuf"* %94, %"class.std::__1::basic_streambuf"** %30, align 8
  %95 = load %"class.std::__1::basic_ios"*, %"class.std::__1::basic_ios"** %29, align 8
  %96 = bitcast %"class.std::__1::basic_ios"* %95 to %"class.std::__1::ios_base"*
  %97 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %30, align 8
  %98 = bitcast %"class.std::__1::basic_streambuf"* %97 to i8*
  invoke void @_ZNSt3__18ios_base4initEPv(%"class.std::__1::ios_base"* %96, i8* %98)
          to label %99 unwind label %134

; <label>:99:                                     ; preds = %0
  %100 = getelementptr inbounds %"class.std::__1::basic_ios", %"class.std::__1::basic_ios"* %95, i32 0, i32 1
  store %"class.std::__1::basic_ostream"* null, %"class.std::__1::basic_ostream"** %100, align 8
  %101 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  %102 = getelementptr inbounds %"class.std::__1::basic_ios", %"class.std::__1::basic_ios"* %95, i32 0, i32 2
  store i32 %101, i32* %102, align 8
  %103 = bitcast %"class.std::__1::basic_ifstream"* %53 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([10 x i8*], [10 x i8*]* @_ZTVNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE, i32 0, i32 3) to i32 (...)**), i32 (...)*** %103, align 8
  %104 = bitcast %"class.std::__1::basic_ifstream"* %53 to i8*
  %105 = getelementptr inbounds i8, i8* %104, i64 424
  %106 = bitcast i8* %105 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([10 x i8*], [10 x i8*]* @_ZTVNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE, i32 0, i32 8) to i32 (...)**), i32 (...)*** %106, align 8
  %107 = getelementptr inbounds %"class.std::__1::basic_ifstream", %"class.std::__1::basic_ifstream"* %53, i32 0, i32 1
  invoke void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEEC1Ev(%"class.std::__1::basic_filebuf"* %107)
          to label %108 unwind label %138

; <label>:108:                                    ; preds = %99
  %109 = getelementptr inbounds %"class.std::__1::basic_ifstream", %"class.std::__1::basic_ifstream"* %53, i32 0, i32 1
  %110 = load i8*, i8** %37, align 8
  %111 = load i32, i32* %38, align 4
  %112 = or i32 %111, 8
  %113 = invoke %"class.std::__1::basic_filebuf"* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE4openEPKcj(%"class.std::__1::basic_filebuf"* %109, i8* %110, i32 %112)
          to label %114 unwind label %142

; <label>:114:                                    ; preds = %108
  %115 = icmp eq %"class.std::__1::basic_filebuf"* %113, null
  br i1 %115, label %116, label %156

; <label>:116:                                    ; preds = %114
  %117 = bitcast %"class.std::__1::basic_ifstream"* %53 to i8**
  %118 = load i8*, i8** %117, align 8
  %119 = getelementptr i8, i8* %118, i64 -24
  %120 = bitcast i8* %119 to i64*
  %121 = load i64, i64* %120, align 8
  %122 = bitcast %"class.std::__1::basic_ifstream"* %53 to i8*
  %123 = getelementptr inbounds i8, i8* %122, i64 %121
  %124 = bitcast i8* %123 to %"class.std::__1::basic_ios"*
  store %"class.std::__1::basic_ios"* %124, %"class.std::__1::basic_ios"** %27, align 8
  store i32 4, i32* %28, align 4
  %125 = load %"class.std::__1::basic_ios"*, %"class.std::__1::basic_ios"** %27, align 8
  %126 = bitcast %"class.std::__1::basic_ios"* %125 to %"class.std::__1::ios_base"*
  %127 = load i32, i32* %28, align 4
  store %"class.std::__1::ios_base"* %126, %"class.std::__1::ios_base"** %25, align 8
  store i32 %127, i32* %26, align 4
  %128 = load %"class.std::__1::ios_base"*, %"class.std::__1::ios_base"** %25, align 8
  %129 = getelementptr inbounds %"class.std::__1::ios_base", %"class.std::__1::ios_base"* %128, i32 0, i32 4
  %130 = load i32, i32* %129, align 8
  %131 = load i32, i32* %26, align 4
  %132 = or i32 %130, %131
  invoke void @_ZNSt3__18ios_base5clearEj(%"class.std::__1::ios_base"* %128, i32 %132)
          to label %133 unwind label %142

; <label>:133:                                    ; preds = %116
  br label %156

; <label>:134:                                    ; preds = %0
  %135 = landingpad { i8*, i32 }
          cleanup
  %136 = extractvalue { i8*, i32 } %135, 0
  store i8* %136, i8** %39, align 8
  %137 = extractvalue { i8*, i32 } %135, 1
  store i32 %137, i32* %40, align 4
  br label %148

; <label>:138:                                    ; preds = %99
  %139 = landingpad { i8*, i32 }
          cleanup
  %140 = extractvalue { i8*, i32 } %139, 0
  store i8* %140, i8** %39, align 8
  %141 = extractvalue { i8*, i32 } %139, 1
  store i32 %141, i32* %40, align 4
  br label %146

; <label>:142:                                    ; preds = %116, %108
  %143 = landingpad { i8*, i32 }
          cleanup
  %144 = extractvalue { i8*, i32 } %143, 0
  store i8* %144, i8** %39, align 8
  %145 = extractvalue { i8*, i32 } %143, 1
  store i32 %145, i32* %40, align 4
  call void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEED1Ev(%"class.std::__1::basic_filebuf"* %107) #15
  br label %146

; <label>:146:                                    ; preds = %142, %138
  %147 = bitcast %"class.std::__1::basic_ifstream"* %53 to %"class.std::__1::basic_istream"*
  call void @_ZNSt3__113basic_istreamIcNS_11char_traitsIcEEED2Ev(%"class.std::__1::basic_istream"* %147, i8** getelementptr inbounds ([4 x i8*], [4 x i8*]* @_ZTTNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE, i64 0, i64 1)) #15
  br label %148

; <label>:148:                                    ; preds = %146, %134
  %149 = bitcast %"class.std::__1::basic_ifstream"* %53 to i8*
  %150 = getelementptr inbounds i8, i8* %149, i64 424
  %151 = bitcast i8* %150 to %"class.std::__1::basic_ios"*
  call void @_ZNSt3__19basic_iosIcNS_11char_traitsIcEEED2Ev(%"class.std::__1::basic_ios"* %151) #15
  %152 = load i8*, i8** %39, align 8
  %153 = load i32, i32* %40, align 4
  %154 = insertvalue { i8*, i32 } undef, i8* %152, 0
  %155 = insertvalue { i8*, i32 } %154, i32 %153, 1
  resume { i8*, i32 } %155

; <label>:156:                                    ; preds = %114, %133
  %157 = bitcast %"class.std::__1::basic_ifstream"* %42 to %"class.std::__1::basic_istream"*
  store %"class.std::__1::istreambuf_iterator"* %44, %"class.std::__1::istreambuf_iterator"** %23, align 8
  store %"class.std::__1::basic_istream"* %157, %"class.std::__1::basic_istream"** %24, align 8
  %158 = load %"class.std::__1::istreambuf_iterator"*, %"class.std::__1::istreambuf_iterator"** %23, align 8
  %159 = load %"class.std::__1::basic_istream"*, %"class.std::__1::basic_istream"** %24, align 8
  store %"class.std::__1::istreambuf_iterator"* %158, %"class.std::__1::istreambuf_iterator"** %21, align 8
  store %"class.std::__1::basic_istream"* %159, %"class.std::__1::basic_istream"** %22, align 8
  %160 = load %"class.std::__1::istreambuf_iterator"*, %"class.std::__1::istreambuf_iterator"** %21, align 8
  %161 = bitcast %"class.std::__1::istreambuf_iterator"* %160 to %"struct.std::__1::iterator"*
  %162 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %160, i32 0, i32 0
  %163 = load %"class.std::__1::basic_istream"*, %"class.std::__1::basic_istream"** %22, align 8
  %164 = bitcast %"class.std::__1::basic_istream"* %163 to i8**
  %165 = load i8*, i8** %164, align 8
  %166 = getelementptr i8, i8* %165, i64 -24
  %167 = bitcast i8* %166 to i64*
  %168 = load i64, i64* %167, align 8
  %169 = bitcast %"class.std::__1::basic_istream"* %163 to i8*
  %170 = getelementptr inbounds i8, i8* %169, i64 %168
  %171 = bitcast i8* %170 to %"class.std::__1::basic_ios"*
  store %"class.std::__1::basic_ios"* %171, %"class.std::__1::basic_ios"** %20, align 8
  %172 = load %"class.std::__1::basic_ios"*, %"class.std::__1::basic_ios"** %20, align 8
  %173 = bitcast %"class.std::__1::basic_ios"* %172 to %"class.std::__1::ios_base"*
  store %"class.std::__1::ios_base"* %173, %"class.std::__1::ios_base"** %19, align 8
  %174 = load %"class.std::__1::ios_base"*, %"class.std::__1::ios_base"** %19, align 8
  %175 = getelementptr inbounds %"class.std::__1::ios_base", %"class.std::__1::ios_base"* %174, i32 0, i32 6
  %176 = load i8*, i8** %175, align 8
  %177 = bitcast i8* %176 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %177, %"class.std::__1::basic_streambuf"** %162, align 8
  store %"class.std::__1::istreambuf_iterator"* %45, %"class.std::__1::istreambuf_iterator"** %18, align 8
  %178 = load %"class.std::__1::istreambuf_iterator"*, %"class.std::__1::istreambuf_iterator"** %18, align 8
  store %"class.std::__1::istreambuf_iterator"* %178, %"class.std::__1::istreambuf_iterator"** %17, align 8
  %179 = load %"class.std::__1::istreambuf_iterator"*, %"class.std::__1::istreambuf_iterator"** %17, align 8
  %180 = bitcast %"class.std::__1::istreambuf_iterator"* %179 to %"struct.std::__1::iterator"*
  %181 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %179, i32 0, i32 0
  store %"class.std::__1::basic_streambuf"* null, %"class.std::__1::basic_streambuf"** %181, align 8
  %182 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %44, i32 0, i32 0
  %183 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %182, align 8
  %184 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %45, i32 0, i32 0
  %185 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %184, align 8
  %186 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %14, i32 0, i32 0
  store %"class.std::__1::basic_streambuf"* %183, %"class.std::__1::basic_streambuf"** %186, align 8
  %187 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %15, i32 0, i32 0
  store %"class.std::__1::basic_streambuf"* %185, %"class.std::__1::basic_streambuf"** %187, align 8
  store %"class.std::__1::basic_string"* %43, %"class.std::__1::basic_string"** %16, align 8
  %188 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %16, align 8
  %189 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %14, i32 0, i32 0
  %190 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %189, align 8
  %191 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %15, i32 0, i32 0
  %192 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %191, align 8
  %193 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %9, i32 0, i32 0
  store %"class.std::__1::basic_streambuf"* %190, %"class.std::__1::basic_streambuf"** %193, align 8
  %194 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %10, i32 0, i32 0
  store %"class.std::__1::basic_streambuf"* %192, %"class.std::__1::basic_streambuf"** %194, align 8
  store %"class.std::__1::basic_string"* %188, %"class.std::__1::basic_string"** %11, align 8
  %195 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8
  %196 = bitcast %"class.std::__1::basic_string"* %195 to %"class.std::__1::__basic_string_common"*
  %197 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %195, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %197, %"class.std::__1::__compressed_pair"** %8, align 8
  %198 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %8, align 8
  store %"class.std::__1::__compressed_pair"* %198, %"class.std::__1::__compressed_pair"** %7, align 8
  %199 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %7, align 8
  %200 = bitcast %"class.std::__1::__compressed_pair"* %199 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %200, %"class.std::__1::__libcpp_compressed_pair_imp"** %6, align 8
  %201 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %6, align 8
  %202 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %201 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %202, %"class.std::__1::allocator"** %5, align 8
  %203 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %5, align 8
  %204 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %201, i32 0, i32 0
  %205 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %204 to i8*
  call void @llvm.memset.p0i8.i64(i8* %205, i8 0, i64 24, i32 8, i1 false) #15
  %206 = bitcast %"class.std::__1::istreambuf_iterator"* %12 to i8*
  %207 = bitcast %"class.std::__1::istreambuf_iterator"* %9 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %206, i8* %207, i64 8, i32 8, i1 false)
  %208 = bitcast %"class.std::__1::istreambuf_iterator"* %13 to i8*
  %209 = bitcast %"class.std::__1::istreambuf_iterator"* %10 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %208, i8* %209, i64 8, i32 8, i1 false)
  %210 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %12, i32 0, i32 0
  %211 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %210, align 8
  %212 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %13, i32 0, i32 0
  %213 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %212, align 8
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initINS_19istreambuf_iteratorIcS2_EEEENS_9enable_ifIXaasr19__is_input_iteratorIT_EE5valuentsr21__is_forward_iteratorISA_EE5valueEvE4typeESA_SA_(%"class.std::__1::basic_string"* %195, %"class.std::__1::basic_streambuf"* %211, %"class.std::__1::basic_streambuf"* %213)
          to label %214 unwind label %219

; <label>:214:                                    ; preds = %156
  br label %215

; <label>:215:                                    ; preds = %214
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"* %49, %"class.std::__1::basic_string"* dereferenceable(24) %43)
          to label %216 unwind label %223

; <label>:216:                                    ; preds = %215
  invoke void @_ZN6ParserC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Parser* %48, %"class.std::__1::basic_string"* %49)
          to label %217 unwind label %227

; <label>:217:                                    ; preds = %216
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %49) #15
  invoke void @_ZN6Parser3runEb(%class.Parser* %48, i1 zeroext false)
          to label %218 unwind label %231

; <label>:218:                                    ; preds = %217
  call void @_ZN6ParserD1Ev(%class.Parser* %48) #15
  br label %257

; <label>:219:                                    ; preds = %156
  %220 = landingpad { i8*, i32 }
          cleanup
  %221 = extractvalue { i8*, i32 } %220, 0
  store i8* %221, i8** %46, align 8
  %222 = extractvalue { i8*, i32 } %220, 1
  store i32 %222, i32* %47, align 4
  br label %295

; <label>:223:                                    ; preds = %215
  %224 = landingpad { i8*, i32 }
          cleanup
          catch i8* bitcast ({ i8*, i8*, i8* }* @_ZTI10ParseError to i8*)
          catch i8* bitcast ({ i8*, i8*, i8* }* @_ZTI12RuntimeError to i8*)
  %225 = extractvalue { i8*, i32 } %224, 0
  store i8* %225, i8** %46, align 8
  %226 = extractvalue { i8*, i32 } %224, 1
  store i32 %226, i32* %47, align 4
  br label %235

; <label>:227:                                    ; preds = %216
  %228 = landingpad { i8*, i32 }
          cleanup
          catch i8* bitcast ({ i8*, i8*, i8* }* @_ZTI10ParseError to i8*)
          catch i8* bitcast ({ i8*, i8*, i8* }* @_ZTI12RuntimeError to i8*)
  %229 = extractvalue { i8*, i32 } %228, 0
  store i8* %229, i8** %46, align 8
  %230 = extractvalue { i8*, i32 } %228, 1
  store i32 %230, i32* %47, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %49) #15
  br label %235

; <label>:231:                                    ; preds = %217
  %232 = landingpad { i8*, i32 }
          cleanup
          catch i8* bitcast ({ i8*, i8*, i8* }* @_ZTI10ParseError to i8*)
          catch i8* bitcast ({ i8*, i8*, i8* }* @_ZTI12RuntimeError to i8*)
  %233 = extractvalue { i8*, i32 } %232, 0
  store i8* %233, i8** %46, align 8
  %234 = extractvalue { i8*, i32 } %232, 1
  store i32 %234, i32* %47, align 4
  call void @_ZN6ParserD1Ev(%class.Parser* %48) #15
  br label %235

; <label>:235:                                    ; preds = %231, %227, %223
  %236 = load i32, i32* %47, align 4
  %237 = call i32 @llvm.eh.typeid.for(i8* bitcast ({ i8*, i8*, i8* }* @_ZTI10ParseError to i8*)) #15
  %238 = icmp eq i32 %236, %237
  br i1 %238, label %239, label %259

; <label>:239:                                    ; preds = %235
  %240 = load i8*, i8** %46, align 8
  %241 = call i8* @__cxa_begin_catch(i8* %240) #15
  %242 = bitcast i8* %241 to %class.ParseError*
  store %class.ParseError* %242, %class.ParseError** %51, align 8
  %243 = load %class.ParseError*, %class.ParseError** %51, align 8
  %244 = bitcast %class.ParseError* %243 to i8* (%class.ParseError*)***
  %245 = load i8* (%class.ParseError*)**, i8* (%class.ParseError*)*** %244, align 8
  %246 = getelementptr inbounds i8* (%class.ParseError*)*, i8* (%class.ParseError*)** %245, i64 2
  %247 = load i8* (%class.ParseError*)*, i8* (%class.ParseError*)** %246, align 8
  %248 = call i8* %247(%class.ParseError* %243) #15
  %249 = invoke dereferenceable(160) %"class.std::__1::basic_ostream"* @_ZNSt3__1lsINS_11char_traitsIcEEEERNS_13basic_ostreamIcT_EES6_PKc(%"class.std::__1::basic_ostream"* dereferenceable(160) @_ZNSt3__14coutE, i8* %248)
          to label %250 unwind label %289

; <label>:250:                                    ; preds = %239
  store %"class.std::__1::basic_ostream"* %249, %"class.std::__1::basic_ostream"** %3, align 8
  store %"class.std::__1::basic_ostream"* (%"class.std::__1::basic_ostream"*)* @_ZNSt3__14endlIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_, %"class.std::__1::basic_ostream"* (%"class.std::__1::basic_ostream"*)** %4, align 8
  %251 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %3, align 8
  %252 = load %"class.std::__1::basic_ostream"* (%"class.std::__1::basic_ostream"*)*, %"class.std::__1::basic_ostream"* (%"class.std::__1::basic_ostream"*)** %4, align 8
  %253 = invoke dereferenceable(160) %"class.std::__1::basic_ostream"* %252(%"class.std::__1::basic_ostream"* dereferenceable(160) %251)
          to label %254 unwind label %289

; <label>:254:                                    ; preds = %250
  br label %255

; <label>:255:                                    ; preds = %254
  invoke void @__cxa_end_catch()
          to label %256 unwind label %284

; <label>:256:                                    ; preds = %255
  br label %257

; <label>:257:                                    ; preds = %256, %279, %218
  store i32 0, i32* %41, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %43) #15
  call void @_ZNSt3__114basic_ifstreamIcNS_11char_traitsIcEEED1Ev(%"class.std::__1::basic_ifstream"* %42) #15
  %258 = load i32, i32* %41, align 4
  ret i32 %258

; <label>:259:                                    ; preds = %235
  %260 = call i32 @llvm.eh.typeid.for(i8* bitcast ({ i8*, i8*, i8* }* @_ZTI12RuntimeError to i8*)) #15
  %261 = icmp eq i32 %236, %260
  br i1 %261, label %262, label %294

; <label>:262:                                    ; preds = %259
  %263 = load i8*, i8** %46, align 8
  %264 = call i8* @__cxa_begin_catch(i8* %263) #15
  %265 = bitcast i8* %264 to %class.RuntimeError*
  store %class.RuntimeError* %265, %class.RuntimeError** %50, align 8
  %266 = load %class.RuntimeError*, %class.RuntimeError** %50, align 8
  %267 = bitcast %class.RuntimeError* %266 to i8* (%class.RuntimeError*)***
  %268 = load i8* (%class.RuntimeError*)**, i8* (%class.RuntimeError*)*** %267, align 8
  %269 = getelementptr inbounds i8* (%class.RuntimeError*)*, i8* (%class.RuntimeError*)** %268, i64 2
  %270 = load i8* (%class.RuntimeError*)*, i8* (%class.RuntimeError*)** %269, align 8
  %271 = call i8* %270(%class.RuntimeError* %266) #15
  %272 = invoke dereferenceable(160) %"class.std::__1::basic_ostream"* @_ZNSt3__1lsINS_11char_traitsIcEEEERNS_13basic_ostreamIcT_EES6_PKc(%"class.std::__1::basic_ostream"* dereferenceable(160) @_ZNSt3__14coutE, i8* %271)
          to label %273 unwind label %280

; <label>:273:                                    ; preds = %262
  store %"class.std::__1::basic_ostream"* %272, %"class.std::__1::basic_ostream"** %1, align 8
  store %"class.std::__1::basic_ostream"* (%"class.std::__1::basic_ostream"*)* @_ZNSt3__14endlIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_, %"class.std::__1::basic_ostream"* (%"class.std::__1::basic_ostream"*)** %2, align 8
  %274 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %1, align 8
  %275 = load %"class.std::__1::basic_ostream"* (%"class.std::__1::basic_ostream"*)*, %"class.std::__1::basic_ostream"* (%"class.std::__1::basic_ostream"*)** %2, align 8
  %276 = invoke dereferenceable(160) %"class.std::__1::basic_ostream"* %275(%"class.std::__1::basic_ostream"* dereferenceable(160) %274)
          to label %277 unwind label %280

; <label>:277:                                    ; preds = %273
  br label %278

; <label>:278:                                    ; preds = %277
  invoke void @__cxa_end_catch()
          to label %279 unwind label %284

; <label>:279:                                    ; preds = %278
  br label %257

; <label>:280:                                    ; preds = %273, %262
  %281 = landingpad { i8*, i32 }
          cleanup
  %282 = extractvalue { i8*, i32 } %281, 0
  store i8* %282, i8** %46, align 8
  %283 = extractvalue { i8*, i32 } %281, 1
  store i32 %283, i32* %47, align 4
  invoke void @__cxa_end_catch()
          to label %288 unwind label %301

; <label>:284:                                    ; preds = %255, %278
  %285 = landingpad { i8*, i32 }
          cleanup
  %286 = extractvalue { i8*, i32 } %285, 0
  store i8* %286, i8** %46, align 8
  %287 = extractvalue { i8*, i32 } %285, 1
  store i32 %287, i32* %47, align 4
  br label %294

; <label>:288:                                    ; preds = %280
  br label %294

; <label>:289:                                    ; preds = %250, %239
  %290 = landingpad { i8*, i32 }
          cleanup
  %291 = extractvalue { i8*, i32 } %290, 0
  store i8* %291, i8** %46, align 8
  %292 = extractvalue { i8*, i32 } %290, 1
  store i32 %292, i32* %47, align 4
  invoke void @__cxa_end_catch()
          to label %293 unwind label %301

; <label>:293:                                    ; preds = %289
  br label %294

; <label>:294:                                    ; preds = %293, %288, %284, %259
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %43) #15
  br label %295

; <label>:295:                                    ; preds = %294, %219
  call void @_ZNSt3__114basic_ifstreamIcNS_11char_traitsIcEEED1Ev(%"class.std::__1::basic_ifstream"* %42) #15
  br label %296

; <label>:296:                                    ; preds = %295
  %297 = load i8*, i8** %46, align 8
  %298 = load i32, i32* %47, align 4
  %299 = insertvalue { i8*, i32 } undef, i8* %297, 0
  %300 = insertvalue { i8*, i32 } %299, i32 %298, 1
  resume { i8*, i32 } %300

; <label>:301:                                    ; preds = %289, %280
  %302 = landingpad { i8*, i32 }
          catch i8* null
  %303 = extractvalue { i8*, i32 } %302, 0
  call void @__clang_call_terminate(i8* %303) #16
  unreachable
}

; Function Attrs: nounwind
declare void ()* @_ZSt13set_terminatePFvvE(void ()*) #3

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_(%"class.std::__1::basic_string"*, %"class.std::__1::basic_string"* dereferenceable(24)) unnamed_addr #1

declare void @_ZN6ParserC1ENSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE(%class.Parser*, %"class.std::__1::basic_string"*) unnamed_addr #1

declare void @_ZN6Parser3runEb(%class.Parser*, i1 zeroext) #1

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN6ParserD1Ev(%class.Parser*) unnamed_addr #5 align 2 {
  %2 = alloca %class.Parser*, align 8
  store %class.Parser* %0, %class.Parser** %2, align 8
  %3 = load %class.Parser*, %class.Parser** %2, align 8
  call void @_ZN6ParserD2Ev(%class.Parser* %3) #15
  ret void
}

; Function Attrs: nounwind readnone
declare i32 @llvm.eh.typeid.for(i8*) #6

declare i8* @__cxa_begin_catch(i8*)

; Function Attrs: alwaysinline inlinehint ssp uwtable
define linkonce_odr hidden dereferenceable(160) %"class.std::__1::basic_ostream"* @_ZNSt3__14endlIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_(%"class.std::__1::basic_ostream"* dereferenceable(160)) #7 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::ctype"*, align 8
  %3 = alloca i8, align 1
  %4 = alloca %"class.std::__1::locale"*, align 8
  %5 = alloca %"class.std::__1::basic_ios"*, align 8
  %6 = alloca i8, align 1
  %7 = alloca %"class.std::__1::locale", align 8
  %8 = alloca i8*
  %9 = alloca i32
  %10 = alloca %"class.std::__1::basic_ostream"*, align 8
  store %"class.std::__1::basic_ostream"* %0, %"class.std::__1::basic_ostream"** %10, align 8
  %11 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %10, align 8
  %12 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %10, align 8
  %13 = bitcast %"class.std::__1::basic_ostream"* %12 to i8**
  %14 = load i8*, i8** %13, align 8
  %15 = getelementptr i8, i8* %14, i64 -24
  %16 = bitcast i8* %15 to i64*
  %17 = load i64, i64* %16, align 8
  %18 = bitcast %"class.std::__1::basic_ostream"* %12 to i8*
  %19 = getelementptr inbounds i8, i8* %18, i64 %17
  %20 = bitcast i8* %19 to %"class.std::__1::basic_ios"*
  store %"class.std::__1::basic_ios"* %20, %"class.std::__1::basic_ios"** %5, align 8
  store i8 10, i8* %6, align 1
  %21 = load %"class.std::__1::basic_ios"*, %"class.std::__1::basic_ios"** %5, align 8
  %22 = bitcast %"class.std::__1::basic_ios"* %21 to %"class.std::__1::ios_base"*
  call void @_ZNKSt3__18ios_base6getlocEv(%"class.std::__1::locale"* sret %7, %"class.std::__1::ios_base"* %22)
  store %"class.std::__1::locale"* %7, %"class.std::__1::locale"** %4, align 8
  %23 = load %"class.std::__1::locale"*, %"class.std::__1::locale"** %4, align 8
  %24 = invoke %"class.std::__1::locale::facet"* @_ZNKSt3__16locale9use_facetERNS0_2idE(%"class.std::__1::locale"* %23, %"class.std::__1::locale::id"* dereferenceable(16) @_ZNSt3__15ctypeIcE2idE)
          to label %25 unwind label %35

; <label>:25:                                     ; preds = %1
  %26 = bitcast %"class.std::__1::locale::facet"* %24 to %"class.std::__1::ctype"*
  %27 = load i8, i8* %6, align 1
  store %"class.std::__1::ctype"* %26, %"class.std::__1::ctype"** %2, align 8
  store i8 %27, i8* %3, align 1
  %28 = load %"class.std::__1::ctype"*, %"class.std::__1::ctype"** %2, align 8
  %29 = bitcast %"class.std::__1::ctype"* %28 to i8 (%"class.std::__1::ctype"*, i8)***
  %30 = load i8 (%"class.std::__1::ctype"*, i8)**, i8 (%"class.std::__1::ctype"*, i8)*** %29, align 8
  %31 = getelementptr inbounds i8 (%"class.std::__1::ctype"*, i8)*, i8 (%"class.std::__1::ctype"*, i8)** %30, i64 7
  %32 = load i8 (%"class.std::__1::ctype"*, i8)*, i8 (%"class.std::__1::ctype"*, i8)** %31, align 8
  %33 = load i8, i8* %3, align 1
  %34 = invoke signext i8 %32(%"class.std::__1::ctype"* %28, i8 signext %33)
          to label %43 unwind label %35

; <label>:35:                                     ; preds = %25, %1
  %36 = landingpad { i8*, i32 }
          cleanup
  %37 = extractvalue { i8*, i32 } %36, 0
  store i8* %37, i8** %8, align 8
  %38 = extractvalue { i8*, i32 } %36, 1
  store i32 %38, i32* %9, align 4
  call void @_ZNSt3__16localeD1Ev(%"class.std::__1::locale"* %7) #15
  %39 = load i8*, i8** %8, align 8
  %40 = load i32, i32* %9, align 4
  %41 = insertvalue { i8*, i32 } undef, i8* %39, 0
  %42 = insertvalue { i8*, i32 } %41, i32 %40, 1
  resume { i8*, i32 } %42

; <label>:43:                                     ; preds = %25
  call void @_ZNSt3__16localeD1Ev(%"class.std::__1::locale"* %7) #15
  %44 = call dereferenceable(160) %"class.std::__1::basic_ostream"* @_ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE3putEc(%"class.std::__1::basic_ostream"* %11, i8 signext %34)
  %45 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %10, align 8
  %46 = call dereferenceable(160) %"class.std::__1::basic_ostream"* @_ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE5flushEv(%"class.std::__1::basic_ostream"* %45)
  %47 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %10, align 8
  ret %"class.std::__1::basic_ostream"* %47
}

declare void @__cxa_end_catch()

; Function Attrs: noinline noreturn nounwind
define linkonce_odr hidden void @__clang_call_terminate(i8*) #8 {
  %2 = call i8* @__cxa_begin_catch(i8* %0) #15
  call void @_ZSt9terminatev() #16
  unreachable
}

declare void @_ZSt9terminatev()

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__114basic_ifstreamIcNS_11char_traitsIcEEED1Ev(%"class.std::__1::basic_ifstream"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.std::__1::basic_ifstream"*, align 8
  store %"class.std::__1::basic_ifstream"* %0, %"class.std::__1::basic_ifstream"** %2, align 8
  %3 = load %"class.std::__1::basic_ifstream"*, %"class.std::__1::basic_ifstream"** %2, align 8
  call void @_ZNSt3__114basic_ifstreamIcNS_11char_traitsIcEEED2Ev(%"class.std::__1::basic_ifstream"* %3, i8** getelementptr inbounds ([4 x i8*], [4 x i8*]* @_ZTTNSt3__114basic_ifstreamIcNS_11char_traitsIcEEEE, i64 0, i64 0)) #15
  %4 = bitcast %"class.std::__1::basic_ifstream"* %3 to i8*
  %5 = getelementptr inbounds i8, i8* %4, i64 424
  %6 = bitcast i8* %5 to %"class.std::__1::basic_ios"*
  call void @_ZNSt3__19basic_iosIcNS_11char_traitsIcEEED2Ev(%"class.std::__1::basic_ios"* %6) #15
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZN6ParserD2Ev(%class.Parser*) unnamed_addr #5 align 2 {
  %2 = alloca %class.Parser*, align 8
  store %class.Parser* %0, %class.Parser** %2, align 8
  %3 = load %class.Parser*, %class.Parser** %2, align 8
  %4 = getelementptr inbounds %class.Parser, %class.Parser* %3, i32 0, i32 2
  call void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED1Ev(%"class.std::__1::vector.1"* %4) #15
  %5 = getelementptr inbounds %class.Parser, %class.Parser* %3, i32 0, i32 0
  call void @_ZNSt3__16vectorINS_10shared_ptrI9StatementEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector"* %5) #15
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED1Ev(%"class.std::__1::vector.1"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.std::__1::vector.1"*, align 8
  store %"class.std::__1::vector.1"* %0, %"class.std::__1::vector.1"** %2, align 8
  %3 = load %"class.std::__1::vector.1"*, %"class.std::__1::vector.1"** %2, align 8
  call void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::vector.1"* %3) #15
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorINS_10shared_ptrI9StatementEENS_9allocatorIS3_EEED1Ev(%"class.std::__1::vector"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.std::__1::vector"*, align 8
  store %"class.std::__1::vector"* %0, %"class.std::__1::vector"** %2, align 8
  %3 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %2, align 8
  call void @_ZNSt3__16vectorINS_10shared_ptrI9StatementEENS_9allocatorIS3_EEED2Ev(%"class.std::__1::vector"* %3) #15
  ret void
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::vector.1"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.std::__1::vector.1"*, align 8
  store %"class.std::__1::vector.1"* %0, %"class.std::__1::vector.1"** %2, align 8
  %3 = load %"class.std::__1::vector.1"*, %"class.std::__1::vector.1"** %2, align 8
  %4 = bitcast %"class.std::__1::vector.1"* %3 to %"class.std::__1::__vector_base.2"*
  call void @_ZNSt3__113__vector_baseINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::__vector_base.2"* %4) #15
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113__vector_baseINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev(%"class.std::__1::__vector_base.2"*) unnamed_addr #9 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.4"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.3"*, align 8
  %4 = alloca %"class.std::__1::__vector_base.2"*, align 8
  %5 = alloca %"class.std::__1::__vector_base.2"*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %"class.std::__1::allocator.5"*, align 8
  %8 = alloca %"class.std::__1::basic_string"*, align 8
  %9 = alloca i64, align 8
  %10 = alloca %"class.std::__1::allocator.5"*, align 8
  %11 = alloca %"class.std::__1::basic_string"*, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.4"*, align 8
  %14 = alloca %"class.std::__1::__compressed_pair.3"*, align 8
  %15 = alloca %"class.std::__1::__vector_base.2"*, align 8
  %16 = alloca %"class.std::__1::allocator.5"*, align 8
  %17 = alloca %"class.std::__1::basic_string"*, align 8
  %18 = alloca %"struct.std::__1::integral_constant", align 1
  %19 = alloca %"class.std::__1::allocator.5"*, align 8
  %20 = alloca %"class.std::__1::basic_string"*, align 8
  %21 = alloca %"class.std::__1::allocator.5"*, align 8
  %22 = alloca %"class.std::__1::basic_string"*, align 8
  %23 = alloca %"struct.std::__1::integral_constant", align 1
  %24 = alloca %"struct.std::__1::__has_destroy", align 1
  %25 = alloca %"class.std::__1::basic_string"*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.4"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.3"*, align 8
  %28 = alloca %"class.std::__1::__vector_base.2"*, align 8
  %29 = alloca %"class.std::__1::__vector_base.2"*, align 8
  %30 = alloca %"class.std::__1::basic_string"*, align 8
  %31 = alloca %"class.std::__1::__vector_base.2"*, align 8
  %32 = alloca %"class.std::__1::__vector_base.2"*, align 8
  store %"class.std::__1::__vector_base.2"* %0, %"class.std::__1::__vector_base.2"** %32, align 8
  %33 = load %"class.std::__1::__vector_base.2"*, %"class.std::__1::__vector_base.2"** %32, align 8
  %34 = getelementptr inbounds %"class.std::__1::__vector_base.2", %"class.std::__1::__vector_base.2"* %33, i32 0, i32 0
  %35 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %34, align 8
  %36 = icmp ne %"class.std::__1::basic_string"* %35, null
  br i1 %36, label %37, label %95

; <label>:37:                                     ; preds = %1
  store %"class.std::__1::__vector_base.2"* %33, %"class.std::__1::__vector_base.2"** %31, align 8
  %38 = load %"class.std::__1::__vector_base.2"*, %"class.std::__1::__vector_base.2"** %31, align 8
  %39 = getelementptr inbounds %"class.std::__1::__vector_base.2", %"class.std::__1::__vector_base.2"* %38, i32 0, i32 0
  %40 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %39, align 8
  store %"class.std::__1::__vector_base.2"* %38, %"class.std::__1::__vector_base.2"** %29, align 8
  store %"class.std::__1::basic_string"* %40, %"class.std::__1::basic_string"** %30, align 8
  %41 = load %"class.std::__1::__vector_base.2"*, %"class.std::__1::__vector_base.2"** %29, align 8
  br label %42

; <label>:42:                                     ; preds = %47, %37
  %43 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %30, align 8
  %44 = getelementptr inbounds %"class.std::__1::__vector_base.2", %"class.std::__1::__vector_base.2"* %41, i32 0, i32 1
  %45 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %44, align 8
  %46 = icmp ne %"class.std::__1::basic_string"* %43, %45
  br i1 %46, label %47, label %65

; <label>:47:                                     ; preds = %42
  store %"class.std::__1::__vector_base.2"* %41, %"class.std::__1::__vector_base.2"** %28, align 8
  %48 = load %"class.std::__1::__vector_base.2"*, %"class.std::__1::__vector_base.2"** %28, align 8
  %49 = getelementptr inbounds %"class.std::__1::__vector_base.2", %"class.std::__1::__vector_base.2"* %48, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.3"* %49, %"class.std::__1::__compressed_pair.3"** %27, align 8
  %50 = load %"class.std::__1::__compressed_pair.3"*, %"class.std::__1::__compressed_pair.3"** %27, align 8
  %51 = bitcast %"class.std::__1::__compressed_pair.3"* %50 to %"class.std::__1::__libcpp_compressed_pair_imp.4"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.4"* %51, %"class.std::__1::__libcpp_compressed_pair_imp.4"** %26, align 8
  %52 = load %"class.std::__1::__libcpp_compressed_pair_imp.4"*, %"class.std::__1::__libcpp_compressed_pair_imp.4"** %26, align 8
  %53 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.4"* %52 to %"class.std::__1::allocator.5"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base.2", %"class.std::__1::__vector_base.2"* %41, i32 0, i32 1
  %55 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %54, align 8
  %56 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %55, i32 -1
  store %"class.std::__1::basic_string"* %56, %"class.std::__1::basic_string"** %54, align 8
  store %"class.std::__1::basic_string"* %56, %"class.std::__1::basic_string"** %25, align 8
  %57 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %25, align 8
  store %"class.std::__1::allocator.5"* %53, %"class.std::__1::allocator.5"** %21, align 8
  store %"class.std::__1::basic_string"* %57, %"class.std::__1::basic_string"** %22, align 8
  %58 = bitcast %"struct.std::__1::__has_destroy"* %24 to %"struct.std::__1::integral_constant"*
  %59 = load %"class.std::__1::allocator.5"*, %"class.std::__1::allocator.5"** %21, align 8
  %60 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %22, align 8
  store %"class.std::__1::allocator.5"* %59, %"class.std::__1::allocator.5"** %19, align 8
  store %"class.std::__1::basic_string"* %60, %"class.std::__1::basic_string"** %20, align 8
  %61 = load %"class.std::__1::allocator.5"*, %"class.std::__1::allocator.5"** %19, align 8
  %62 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %20, align 8
  store %"class.std::__1::allocator.5"* %61, %"class.std::__1::allocator.5"** %16, align 8
  store %"class.std::__1::basic_string"* %62, %"class.std::__1::basic_string"** %17, align 8
  %63 = load %"class.std::__1::allocator.5"*, %"class.std::__1::allocator.5"** %16, align 8
  %64 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %64) #15
  br label %42

; <label>:65:                                     ; preds = %42
  store %"class.std::__1::__vector_base.2"* %33, %"class.std::__1::__vector_base.2"** %15, align 8
  %66 = load %"class.std::__1::__vector_base.2"*, %"class.std::__1::__vector_base.2"** %15, align 8
  %67 = getelementptr inbounds %"class.std::__1::__vector_base.2", %"class.std::__1::__vector_base.2"* %66, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.3"* %67, %"class.std::__1::__compressed_pair.3"** %14, align 8
  %68 = load %"class.std::__1::__compressed_pair.3"*, %"class.std::__1::__compressed_pair.3"** %14, align 8
  %69 = bitcast %"class.std::__1::__compressed_pair.3"* %68 to %"class.std::__1::__libcpp_compressed_pair_imp.4"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.4"* %69, %"class.std::__1::__libcpp_compressed_pair_imp.4"** %13, align 8
  %70 = load %"class.std::__1::__libcpp_compressed_pair_imp.4"*, %"class.std::__1::__libcpp_compressed_pair_imp.4"** %13, align 8
  %71 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.4"* %70 to %"class.std::__1::allocator.5"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base.2", %"class.std::__1::__vector_base.2"* %33, i32 0, i32 0
  %73 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %72, align 8
  store %"class.std::__1::__vector_base.2"* %33, %"class.std::__1::__vector_base.2"** %5, align 8
  %74 = load %"class.std::__1::__vector_base.2"*, %"class.std::__1::__vector_base.2"** %5, align 8
  store %"class.std::__1::__vector_base.2"* %74, %"class.std::__1::__vector_base.2"** %4, align 8
  %75 = load %"class.std::__1::__vector_base.2"*, %"class.std::__1::__vector_base.2"** %4, align 8
  %76 = getelementptr inbounds %"class.std::__1::__vector_base.2", %"class.std::__1::__vector_base.2"* %75, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.3"* %76, %"class.std::__1::__compressed_pair.3"** %3, align 8
  %77 = load %"class.std::__1::__compressed_pair.3"*, %"class.std::__1::__compressed_pair.3"** %3, align 8
  %78 = bitcast %"class.std::__1::__compressed_pair.3"* %77 to %"class.std::__1::__libcpp_compressed_pair_imp.4"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.4"* %78, %"class.std::__1::__libcpp_compressed_pair_imp.4"** %2, align 8
  %79 = load %"class.std::__1::__libcpp_compressed_pair_imp.4"*, %"class.std::__1::__libcpp_compressed_pair_imp.4"** %2, align 8
  %80 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.4", %"class.std::__1::__libcpp_compressed_pair_imp.4"* %79, i32 0, i32 0
  %81 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %80, align 8
  %82 = getelementptr inbounds %"class.std::__1::__vector_base.2", %"class.std::__1::__vector_base.2"* %74, i32 0, i32 0
  %83 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %82, align 8
  %84 = ptrtoint %"class.std::__1::basic_string"* %81 to i64
  %85 = ptrtoint %"class.std::__1::basic_string"* %83 to i64
  %86 = sub i64 %84, %85
  %87 = sdiv exact i64 %86, 24
  store %"class.std::__1::allocator.5"* %71, %"class.std::__1::allocator.5"** %10, align 8
  store %"class.std::__1::basic_string"* %73, %"class.std::__1::basic_string"** %11, align 8
  store i64 %87, i64* %12, align 8
  %88 = load %"class.std::__1::allocator.5"*, %"class.std::__1::allocator.5"** %10, align 8
  %89 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %11, align 8
  %90 = load i64, i64* %12, align 8
  store %"class.std::__1::allocator.5"* %88, %"class.std::__1::allocator.5"** %7, align 8
  store %"class.std::__1::basic_string"* %89, %"class.std::__1::basic_string"** %8, align 8
  store i64 %90, i64* %9, align 8
  %91 = load %"class.std::__1::allocator.5"*, %"class.std::__1::allocator.5"** %7, align 8
  %92 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %8, align 8
  %93 = bitcast %"class.std::__1::basic_string"* %92 to i8*
  store i8* %93, i8** %6, align 8
  %94 = load i8*, i8** %6, align 8
  call void @_ZdlPv(i8* %94) #17
  br label %95

; <label>:95:                                     ; preds = %65, %1
  ret void
}

; Function Attrs: nobuiltin nounwind
declare void @_ZdlPv(i8*) #10

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__16vectorINS_10shared_ptrI9StatementEENS_9allocatorIS3_EEED2Ev(%"class.std::__1::vector"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.std::__1::vector"*, align 8
  store %"class.std::__1::vector"* %0, %"class.std::__1::vector"** %2, align 8
  %3 = load %"class.std::__1::vector"*, %"class.std::__1::vector"** %2, align 8
  %4 = bitcast %"class.std::__1::vector"* %3 to %"class.std::__1::__vector_base"*
  call void @_ZNSt3__113__vector_baseINS_10shared_ptrI9StatementEENS_9allocatorIS3_EEED2Ev(%"class.std::__1::__vector_base"* %4) #15
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113__vector_baseINS_10shared_ptrI9StatementEENS_9allocatorIS3_EEED2Ev(%"class.std::__1::__vector_base"*) unnamed_addr #9 align 2 personality i32 (...)* @__gxx_personality_v0 {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %4 = alloca %"class.std::__1::__vector_base"*, align 8
  %5 = alloca %"class.std::__1::__vector_base"*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %"class.std::__1::allocator.31"*, align 8
  %8 = alloca %"class.std::__1::shared_ptr"*, align 8
  %9 = alloca i64, align 8
  %10 = alloca %"class.std::__1::allocator.31"*, align 8
  %11 = alloca %"class.std::__1::shared_ptr"*, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %14 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %15 = alloca %"class.std::__1::__vector_base"*, align 8
  %16 = alloca %"class.std::__1::allocator.31"*, align 8
  %17 = alloca %"class.std::__1::shared_ptr"*, align 8
  %18 = alloca %"struct.std::__1::integral_constant", align 1
  %19 = alloca %"class.std::__1::allocator.31"*, align 8
  %20 = alloca %"class.std::__1::shared_ptr"*, align 8
  %21 = alloca %"class.std::__1::allocator.31"*, align 8
  %22 = alloca %"class.std::__1::shared_ptr"*, align 8
  %23 = alloca %"struct.std::__1::integral_constant", align 1
  %24 = alloca %"struct.std::__1::__has_destroy.54", align 1
  %25 = alloca %"class.std::__1::shared_ptr"*, align 8
  %26 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.30"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair.29"*, align 8
  %28 = alloca %"class.std::__1::__vector_base"*, align 8
  %29 = alloca %"class.std::__1::__vector_base"*, align 8
  %30 = alloca %"class.std::__1::shared_ptr"*, align 8
  %31 = alloca %"class.std::__1::__vector_base"*, align 8
  %32 = alloca %"class.std::__1::__vector_base"*, align 8
  store %"class.std::__1::__vector_base"* %0, %"class.std::__1::__vector_base"** %32, align 8
  %33 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %32, align 8
  %34 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %33, i32 0, i32 0
  %35 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %34, align 8
  %36 = icmp ne %"class.std::__1::shared_ptr"* %35, null
  br i1 %36, label %37, label %95

; <label>:37:                                     ; preds = %1
  store %"class.std::__1::__vector_base"* %33, %"class.std::__1::__vector_base"** %31, align 8
  %38 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %31, align 8
  %39 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %38, i32 0, i32 0
  %40 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %39, align 8
  store %"class.std::__1::__vector_base"* %38, %"class.std::__1::__vector_base"** %29, align 8
  store %"class.std::__1::shared_ptr"* %40, %"class.std::__1::shared_ptr"** %30, align 8
  %41 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %29, align 8
  br label %42

; <label>:42:                                     ; preds = %47, %37
  %43 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %30, align 8
  %44 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %41, i32 0, i32 1
  %45 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %44, align 8
  %46 = icmp ne %"class.std::__1::shared_ptr"* %43, %45
  br i1 %46, label %47, label %65

; <label>:47:                                     ; preds = %42
  store %"class.std::__1::__vector_base"* %41, %"class.std::__1::__vector_base"** %28, align 8
  %48 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %28, align 8
  %49 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %48, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %49, %"class.std::__1::__compressed_pair.29"** %27, align 8
  %50 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %27, align 8
  %51 = bitcast %"class.std::__1::__compressed_pair.29"* %50 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %51, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %26, align 8
  %52 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %26, align 8
  %53 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.30"* %52 to %"class.std::__1::allocator.31"*
  %54 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %41, i32 0, i32 1
  %55 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %54, align 8
  %56 = getelementptr inbounds %"class.std::__1::shared_ptr", %"class.std::__1::shared_ptr"* %55, i32 -1
  store %"class.std::__1::shared_ptr"* %56, %"class.std::__1::shared_ptr"** %54, align 8
  store %"class.std::__1::shared_ptr"* %56, %"class.std::__1::shared_ptr"** %25, align 8
  %57 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %25, align 8
  store %"class.std::__1::allocator.31"* %53, %"class.std::__1::allocator.31"** %21, align 8
  store %"class.std::__1::shared_ptr"* %57, %"class.std::__1::shared_ptr"** %22, align 8
  %58 = bitcast %"struct.std::__1::__has_destroy.54"* %24 to %"struct.std::__1::integral_constant"*
  %59 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %21, align 8
  %60 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %22, align 8
  store %"class.std::__1::allocator.31"* %59, %"class.std::__1::allocator.31"** %19, align 8
  store %"class.std::__1::shared_ptr"* %60, %"class.std::__1::shared_ptr"** %20, align 8
  %61 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %19, align 8
  %62 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %20, align 8
  store %"class.std::__1::allocator.31"* %61, %"class.std::__1::allocator.31"** %16, align 8
  store %"class.std::__1::shared_ptr"* %62, %"class.std::__1::shared_ptr"** %17, align 8
  %63 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %16, align 8
  %64 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %17, align 8
  call void @_ZNSt3__110shared_ptrI9StatementED1Ev(%"class.std::__1::shared_ptr"* %64) #15
  br label %42

; <label>:65:                                     ; preds = %42
  store %"class.std::__1::__vector_base"* %33, %"class.std::__1::__vector_base"** %15, align 8
  %66 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %15, align 8
  %67 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %66, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %67, %"class.std::__1::__compressed_pair.29"** %14, align 8
  %68 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %14, align 8
  %69 = bitcast %"class.std::__1::__compressed_pair.29"* %68 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %69, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %13, align 8
  %70 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %13, align 8
  %71 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp.30"* %70 to %"class.std::__1::allocator.31"*
  %72 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %33, i32 0, i32 0
  %73 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %72, align 8
  store %"class.std::__1::__vector_base"* %33, %"class.std::__1::__vector_base"** %5, align 8
  %74 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %5, align 8
  store %"class.std::__1::__vector_base"* %74, %"class.std::__1::__vector_base"** %4, align 8
  %75 = load %"class.std::__1::__vector_base"*, %"class.std::__1::__vector_base"** %4, align 8
  %76 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %75, i32 0, i32 2
  store %"class.std::__1::__compressed_pair.29"* %76, %"class.std::__1::__compressed_pair.29"** %3, align 8
  %77 = load %"class.std::__1::__compressed_pair.29"*, %"class.std::__1::__compressed_pair.29"** %3, align 8
  %78 = bitcast %"class.std::__1::__compressed_pair.29"* %77 to %"class.std::__1::__libcpp_compressed_pair_imp.30"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.30"* %78, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %2, align 8
  %79 = load %"class.std::__1::__libcpp_compressed_pair_imp.30"*, %"class.std::__1::__libcpp_compressed_pair_imp.30"** %2, align 8
  %80 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.30", %"class.std::__1::__libcpp_compressed_pair_imp.30"* %79, i32 0, i32 0
  %81 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %80, align 8
  %82 = getelementptr inbounds %"class.std::__1::__vector_base", %"class.std::__1::__vector_base"* %74, i32 0, i32 0
  %83 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %82, align 8
  %84 = ptrtoint %"class.std::__1::shared_ptr"* %81 to i64
  %85 = ptrtoint %"class.std::__1::shared_ptr"* %83 to i64
  %86 = sub i64 %84, %85
  %87 = sdiv exact i64 %86, 16
  store %"class.std::__1::allocator.31"* %71, %"class.std::__1::allocator.31"** %10, align 8
  store %"class.std::__1::shared_ptr"* %73, %"class.std::__1::shared_ptr"** %11, align 8
  store i64 %87, i64* %12, align 8
  %88 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %10, align 8
  %89 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %11, align 8
  %90 = load i64, i64* %12, align 8
  store %"class.std::__1::allocator.31"* %88, %"class.std::__1::allocator.31"** %7, align 8
  store %"class.std::__1::shared_ptr"* %89, %"class.std::__1::shared_ptr"** %8, align 8
  store i64 %90, i64* %9, align 8
  %91 = load %"class.std::__1::allocator.31"*, %"class.std::__1::allocator.31"** %7, align 8
  %92 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %8, align 8
  %93 = bitcast %"class.std::__1::shared_ptr"* %92 to i8*
  store i8* %93, i8** %6, align 8
  %94 = load i8*, i8** %6, align 8
  call void @_ZdlPv(i8* %94) #17
  br label %95

; <label>:95:                                     ; preds = %65, %1
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__110shared_ptrI9StatementED1Ev(%"class.std::__1::shared_ptr"*) unnamed_addr #9 align 2 {
  %2 = alloca %"class.std::__1::shared_ptr"*, align 8
  store %"class.std::__1::shared_ptr"* %0, %"class.std::__1::shared_ptr"** %2, align 8
  %3 = load %"class.std::__1::shared_ptr"*, %"class.std::__1::shared_ptr"** %2, align 8
  call void @_ZNSt3__110shared_ptrI9StatementED2Ev(%"class.std::__1::shared_ptr"* %3) #15
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__110shared_ptrI9StatementED2Ev(%"class.std::__1::shared_ptr"*) unnamed_addr #9 align 2 {
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
  call void @_ZNSt3__119__shared_weak_count16__release_sharedEv(%"class.std::__1::__shared_weak_count"* %9) #15
  br label %10

; <label>:10:                                     ; preds = %7, %1
  ret void
}

; Function Attrs: nounwind
declare void @_ZNSt3__119__shared_weak_count16__release_sharedEv(%"class.std::__1::__shared_weak_count"*) #3

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__114basic_ifstreamIcNS_11char_traitsIcEEED2Ev(%"class.std::__1::basic_ifstream"*, i8**) unnamed_addr #5 align 2 {
  %3 = alloca %"class.std::__1::basic_ifstream"*, align 8
  %4 = alloca i8**, align 8
  store %"class.std::__1::basic_ifstream"* %0, %"class.std::__1::basic_ifstream"** %3, align 8
  store i8** %1, i8*** %4, align 8
  %5 = load %"class.std::__1::basic_ifstream"*, %"class.std::__1::basic_ifstream"** %3, align 8
  %6 = load i8**, i8*** %4, align 8
  %7 = load i8*, i8** %6, align 8
  %8 = bitcast %"class.std::__1::basic_ifstream"* %5 to i32 (...)***
  %9 = bitcast i8* %7 to i32 (...)**
  store i32 (...)** %9, i32 (...)*** %8, align 8
  %10 = getelementptr inbounds i8*, i8** %6, i64 3
  %11 = load i8*, i8** %10, align 8
  %12 = bitcast %"class.std::__1::basic_ifstream"* %5 to i8**
  %13 = load i8*, i8** %12, align 8
  %14 = getelementptr i8, i8* %13, i64 -24
  %15 = bitcast i8* %14 to i64*
  %16 = load i64, i64* %15, align 8
  %17 = bitcast %"class.std::__1::basic_ifstream"* %5 to i8*
  %18 = getelementptr inbounds i8, i8* %17, i64 %16
  %19 = bitcast i8* %18 to i32 (...)***
  %20 = bitcast i8* %11 to i32 (...)**
  store i32 (...)** %20, i32 (...)*** %19, align 8
  %21 = getelementptr inbounds %"class.std::__1::basic_ifstream", %"class.std::__1::basic_ifstream"* %5, i32 0, i32 1
  call void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEED1Ev(%"class.std::__1::basic_filebuf"* %21) #15
  %22 = bitcast %"class.std::__1::basic_ifstream"* %5 to %"class.std::__1::basic_istream"*
  %23 = getelementptr inbounds i8*, i8** %6, i64 1
  call void @_ZNSt3__113basic_istreamIcNS_11char_traitsIcEEED2Ev(%"class.std::__1::basic_istream"* %22, i8** %23) #15
  ret void
}

; Function Attrs: nounwind
declare void @_ZNSt3__19basic_iosIcNS_11char_traitsIcEEED2Ev(%"class.std::__1::basic_ios"*) unnamed_addr #3

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZTv0_n24_NSt3__114basic_ifstreamIcNS_11char_traitsIcEEED1Ev(%"class.std::__1::basic_ifstream"*) unnamed_addr #9 align 2 {
  %2 = alloca %"class.std::__1::basic_ifstream"*, align 8
  store %"class.std::__1::basic_ifstream"* %0, %"class.std::__1::basic_ifstream"** %2, align 8
  %3 = load %"class.std::__1::basic_ifstream"*, %"class.std::__1::basic_ifstream"** %2, align 8
  %4 = bitcast %"class.std::__1::basic_ifstream"* %3 to i8*
  %5 = bitcast i8* %4 to i8**
  %6 = load i8*, i8** %5, align 8
  %7 = getelementptr inbounds i8, i8* %6, i64 -24
  %8 = bitcast i8* %7 to i64*
  %9 = load i64, i64* %8, align 8
  %10 = getelementptr inbounds i8, i8* %4, i64 %9
  %11 = bitcast i8* %10 to %"class.std::__1::basic_ifstream"*
  call void @_ZNSt3__114basic_ifstreamIcNS_11char_traitsIcEEED1Ev(%"class.std::__1::basic_ifstream"* %11) #15
  ret void
}

; Function Attrs: nounwind
declare void @_ZNSt3__113basic_istreamIcNS_11char_traitsIcEEED1Ev(%"class.std::__1::basic_istream"*) unnamed_addr #3

; Function Attrs: nounwind
declare void @_ZNSt3__113basic_istreamIcNS_11char_traitsIcEEED0Ev(%"class.std::__1::basic_istream"*) unnamed_addr #3

; Function Attrs: nounwind
declare void @_ZTv0_n24_NSt3__113basic_istreamIcNS_11char_traitsIcEEED1Ev(%"class.std::__1::basic_istream"*) unnamed_addr #3

; Function Attrs: nounwind
declare void @_ZTv0_n24_NSt3__113basic_istreamIcNS_11char_traitsIcEEED0Ev(%"class.std::__1::basic_istream"*) unnamed_addr #3

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__114basic_ifstreamIcNS_11char_traitsIcEEED0Ev(%"class.std::__1::basic_ifstream"*) unnamed_addr #5 align 2 {
  %2 = alloca %"class.std::__1::basic_ifstream"*, align 8
  store %"class.std::__1::basic_ifstream"* %0, %"class.std::__1::basic_ifstream"** %2, align 8
  %3 = load %"class.std::__1::basic_ifstream"*, %"class.std::__1::basic_ifstream"** %2, align 8
  call void @_ZNSt3__114basic_ifstreamIcNS_11char_traitsIcEEED1Ev(%"class.std::__1::basic_ifstream"* %3) #15
  %4 = bitcast %"class.std::__1::basic_ifstream"* %3 to i8*
  call void @_ZdlPv(i8* %4) #17
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZTv0_n24_NSt3__114basic_ifstreamIcNS_11char_traitsIcEEED0Ev(%"class.std::__1::basic_ifstream"*) unnamed_addr #9 align 2 {
  %2 = alloca %"class.std::__1::basic_ifstream"*, align 8
  store %"class.std::__1::basic_ifstream"* %0, %"class.std::__1::basic_ifstream"** %2, align 8
  %3 = load %"class.std::__1::basic_ifstream"*, %"class.std::__1::basic_ifstream"** %2, align 8
  %4 = bitcast %"class.std::__1::basic_ifstream"* %3 to i8*
  %5 = bitcast i8* %4 to i8**
  %6 = load i8*, i8** %5, align 8
  %7 = getelementptr inbounds i8, i8* %6, i64 -24
  %8 = bitcast i8* %7 to i64*
  %9 = load i64, i64* %8, align 8
  %10 = getelementptr inbounds i8, i8* %4, i64 %9
  %11 = bitcast i8* %10 to %"class.std::__1::basic_ifstream"*
  call void @_ZNSt3__114basic_ifstreamIcNS_11char_traitsIcEEED0Ev(%"class.std::__1::basic_ifstream"* %11) #15
  ret void
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEED1Ev(%"class.std::__1::basic_filebuf"*) unnamed_addr #9 align 2 {
  %2 = alloca %"class.std::__1::basic_filebuf"*, align 8
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %2, align 8
  %3 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %2, align 8
  call void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEED2Ev(%"class.std::__1::basic_filebuf"* %3) #15
  ret void
}

; Function Attrs: nounwind
declare void @_ZNSt3__113basic_istreamIcNS_11char_traitsIcEEED2Ev(%"class.std::__1::basic_istream"*, i8**) unnamed_addr #3

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEED2Ev(%"class.std::__1::basic_filebuf"*) unnamed_addr #9 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"class.std::__1::basic_filebuf"*, align 8
  %3 = alloca i8*
  %4 = alloca i32
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %2, align 8
  %5 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %2, align 8
  %6 = bitcast %"class.std::__1::basic_filebuf"* %5 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([16 x i8*], [16 x i8*]* @_ZTVNSt3__113basic_filebufIcNS_11char_traitsIcEEEE, i32 0, i32 2) to i32 (...)**), i32 (...)*** %6, align 8
  %7 = invoke %"class.std::__1::basic_filebuf"* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE5closeEv(%"class.std::__1::basic_filebuf"* %5)
          to label %8 unwind label %9

; <label>:8:                                      ; preds = %1
  br label %17

; <label>:9:                                      ; preds = %1
  %10 = landingpad { i8*, i32 }
          catch i8* null
  %11 = extractvalue { i8*, i32 } %10, 0
  store i8* %11, i8** %3, align 8
  %12 = extractvalue { i8*, i32 } %10, 1
  store i32 %12, i32* %4, align 4
  br label %13

; <label>:13:                                     ; preds = %9
  %14 = load i8*, i8** %3, align 8
  %15 = call i8* @__cxa_begin_catch(i8* %14) #15
  invoke void @__cxa_end_catch()
          to label %16 unwind label %27

; <label>:16:                                     ; preds = %13
  br label %17

; <label>:17:                                     ; preds = %16, %8
  %18 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %5, i32 0, i32 14
  %19 = load i8, i8* %18, align 8
  %20 = trunc i8 %19 to i1
  br i1 %20, label %21, label %32

; <label>:21:                                     ; preds = %17
  %22 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %5, i32 0, i32 1
  %23 = load i8*, i8** %22, align 8
  %24 = icmp eq i8* %23, null
  br i1 %24, label %26, label %25

; <label>:25:                                     ; preds = %21
  call void @_ZdaPv(i8* %23) #17
  br label %26

; <label>:26:                                     ; preds = %25, %21
  br label %32

; <label>:27:                                     ; preds = %13
  %28 = landingpad { i8*, i32 }
          catch i8* null
  %29 = extractvalue { i8*, i32 } %28, 0
  store i8* %29, i8** %3, align 8
  %30 = extractvalue { i8*, i32 } %28, 1
  store i32 %30, i32* %4, align 4
  %31 = bitcast %"class.std::__1::basic_filebuf"* %5 to %"class.std::__1::basic_streambuf"*
  call void @_ZNSt3__115basic_streambufIcNS_11char_traitsIcEEED2Ev(%"class.std::__1::basic_streambuf"* %31) #15
  br label %44

; <label>:32:                                     ; preds = %26, %17
  %33 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %5, i32 0, i32 15
  %34 = load i8, i8* %33, align 1
  %35 = trunc i8 %34 to i1
  br i1 %35, label %36, label %42

; <label>:36:                                     ; preds = %32
  %37 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %5, i32 0, i32 6
  %38 = load i8*, i8** %37, align 8
  %39 = icmp eq i8* %38, null
  br i1 %39, label %41, label %40

; <label>:40:                                     ; preds = %36
  call void @_ZdaPv(i8* %38) #17
  br label %41

; <label>:41:                                     ; preds = %40, %36
  br label %42

; <label>:42:                                     ; preds = %41, %32
  %43 = bitcast %"class.std::__1::basic_filebuf"* %5 to %"class.std::__1::basic_streambuf"*
  call void @_ZNSt3__115basic_streambufIcNS_11char_traitsIcEEED2Ev(%"class.std::__1::basic_streambuf"* %43) #15
  ret void

; <label>:44:                                     ; preds = %27
  %45 = load i8*, i8** %3, align 8
  call void @__clang_call_terminate(i8* %45) #16
  unreachable
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.std::__1::basic_filebuf"* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE5closeEv(%"class.std::__1::basic_filebuf"*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.57"*, align 8
  %3 = alloca %"class.std::__1::__compressed_pair.56"*, align 8
  %4 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.57"*, align 8
  %5 = alloca %"class.std::__1::__compressed_pair.56"*, align 8
  %6 = alloca %"class.std::__1::unique_ptr.55"*, align 8
  %7 = alloca %struct.__sFILE*, align 8
  %8 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.57"*, align 8
  %9 = alloca %"class.std::__1::__compressed_pair.56"*, align 8
  %10 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.57"*, align 8
  %11 = alloca %"class.std::__1::__compressed_pair.56"*, align 8
  %12 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.57"*, align 8
  %13 = alloca %"class.std::__1::__compressed_pair.56"*, align 8
  %14 = alloca %"class.std::__1::unique_ptr.55"*, align 8
  %15 = alloca %struct.__sFILE*, align 8
  %16 = alloca %struct.__sFILE*, align 8
  %17 = alloca %"class.std::__1::unique_ptr.55"*, align 8
  %18 = alloca %"class.std::__1::unique_ptr.55"*, align 8
  %19 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.57"*, align 8
  %20 = alloca %"class.std::__1::__compressed_pair.56"*, align 8
  %21 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.57"*, align 8
  %22 = alloca %"class.std::__1::__compressed_pair.56"*, align 8
  %23 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.57"*, align 8
  %24 = alloca %"class.std::__1::__compressed_pair.56"*, align 8
  %25 = alloca %"class.std::__1::unique_ptr.55"*, align 8
  %26 = alloca %struct.__sFILE*, align 8
  %27 = alloca %struct.__sFILE*, align 8
  %28 = alloca %"class.std::__1::unique_ptr.55"*, align 8
  %29 = alloca %"class.std::__1::unique_ptr.55"*, align 8
  %30 = alloca i32 (%struct.__sFILE*)**, align 8
  %31 = alloca i32 (%struct.__sFILE*)**, align 8
  %32 = alloca %struct.__sFILE**, align 8
  %33 = alloca %"class.std::__1::__libcpp_compressed_pair_imp.57"*, align 8
  %34 = alloca %struct.__sFILE*, align 8
  %35 = alloca i32 (%struct.__sFILE*)*, align 8
  %36 = alloca %struct.__sFILE**, align 8
  %37 = alloca %"class.std::__1::__compressed_pair.56"*, align 8
  %38 = alloca %struct.__sFILE*, align 8
  %39 = alloca i32 (%struct.__sFILE*)*, align 8
  %40 = alloca %"class.std::__1::__compressed_pair.56"*, align 8
  %41 = alloca %struct.__sFILE*, align 8
  %42 = alloca i32 (%struct.__sFILE*)*, align 8
  %43 = alloca i32 (%struct.__sFILE*)**, align 8
  %44 = alloca %"class.std::__1::unique_ptr.55"*, align 8
  %45 = alloca %struct.__sFILE*, align 8
  %46 = alloca i32 (%struct.__sFILE*)**, align 8
  %47 = alloca %"class.std::__1::unique_ptr.55"*, align 8
  %48 = alloca %struct.__sFILE*, align 8
  %49 = alloca i32 (%struct.__sFILE*)**, align 8
  %50 = alloca %"class.std::__1::basic_filebuf"*, align 8
  %51 = alloca %"class.std::__1::basic_filebuf"*, align 8
  %52 = alloca %"class.std::__1::unique_ptr.55", align 8
  %53 = alloca i32 (%struct.__sFILE*)*, align 8
  %54 = alloca i8*
  %55 = alloca i32
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %50, align 8
  %56 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %50, align 8
  store %"class.std::__1::basic_filebuf"* null, %"class.std::__1::basic_filebuf"** %51, align 8
  %57 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %56, i32 0, i32 8
  %58 = load %struct.__sFILE*, %struct.__sFILE** %57, align 8
  %59 = icmp ne %struct.__sFILE* %58, null
  br i1 %59, label %60, label %183

; <label>:60:                                     ; preds = %1
  store %"class.std::__1::basic_filebuf"* %56, %"class.std::__1::basic_filebuf"** %51, align 8
  %61 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %56, i32 0, i32 8
  %62 = load %struct.__sFILE*, %struct.__sFILE** %61, align 8
  store i32 (%struct.__sFILE*)* @fclose, i32 (%struct.__sFILE*)** %53, align 8
  store %"class.std::__1::unique_ptr.55"* %52, %"class.std::__1::unique_ptr.55"** %47, align 8
  store %struct.__sFILE* %62, %struct.__sFILE** %48, align 8
  store i32 (%struct.__sFILE*)** %53, i32 (%struct.__sFILE*)*** %49, align 8
  %63 = load %"class.std::__1::unique_ptr.55"*, %"class.std::__1::unique_ptr.55"** %47, align 8
  %64 = load %struct.__sFILE*, %struct.__sFILE** %48, align 8
  %65 = load i32 (%struct.__sFILE*)**, i32 (%struct.__sFILE*)*** %49, align 8
  store %"class.std::__1::unique_ptr.55"* %63, %"class.std::__1::unique_ptr.55"** %44, align 8
  store %struct.__sFILE* %64, %struct.__sFILE** %45, align 8
  store i32 (%struct.__sFILE*)** %65, i32 (%struct.__sFILE*)*** %46, align 8
  %66 = load %"class.std::__1::unique_ptr.55"*, %"class.std::__1::unique_ptr.55"** %44, align 8
  %67 = getelementptr inbounds %"class.std::__1::unique_ptr.55", %"class.std::__1::unique_ptr.55"* %66, i32 0, i32 0
  %68 = load %struct.__sFILE*, %struct.__sFILE** %45, align 8
  %69 = load i32 (%struct.__sFILE*)**, i32 (%struct.__sFILE*)*** %46, align 8
  store i32 (%struct.__sFILE*)** %69, i32 (%struct.__sFILE*)*** %43, align 8
  %70 = load i32 (%struct.__sFILE*)**, i32 (%struct.__sFILE*)*** %43, align 8
  %71 = load i32 (%struct.__sFILE*)*, i32 (%struct.__sFILE*)** %70, align 8
  store %"class.std::__1::__compressed_pair.56"* %67, %"class.std::__1::__compressed_pair.56"** %40, align 8
  store %struct.__sFILE* %68, %struct.__sFILE** %41, align 8
  store i32 (%struct.__sFILE*)* %71, i32 (%struct.__sFILE*)** %42, align 8
  %72 = load %"class.std::__1::__compressed_pair.56"*, %"class.std::__1::__compressed_pair.56"** %40, align 8
  %73 = load %struct.__sFILE*, %struct.__sFILE** %41, align 8
  %74 = load i32 (%struct.__sFILE*)*, i32 (%struct.__sFILE*)** %42, align 8
  store %"class.std::__1::__compressed_pair.56"* %72, %"class.std::__1::__compressed_pair.56"** %37, align 8
  store %struct.__sFILE* %73, %struct.__sFILE** %38, align 8
  store i32 (%struct.__sFILE*)* %74, i32 (%struct.__sFILE*)** %39, align 8
  %75 = load %"class.std::__1::__compressed_pair.56"*, %"class.std::__1::__compressed_pair.56"** %37, align 8
  %76 = bitcast %"class.std::__1::__compressed_pair.56"* %75 to %"class.std::__1::__libcpp_compressed_pair_imp.57"*
  store %struct.__sFILE** %38, %struct.__sFILE*** %36, align 8
  %77 = load %struct.__sFILE**, %struct.__sFILE*** %36, align 8
  %78 = load %struct.__sFILE*, %struct.__sFILE** %77, align 8
  store i32 (%struct.__sFILE*)** %39, i32 (%struct.__sFILE*)*** %30, align 8
  %79 = load i32 (%struct.__sFILE*)**, i32 (%struct.__sFILE*)*** %30, align 8
  %80 = load i32 (%struct.__sFILE*)*, i32 (%struct.__sFILE*)** %79, align 8
  store %"class.std::__1::__libcpp_compressed_pair_imp.57"* %76, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %33, align 8
  store %struct.__sFILE* %78, %struct.__sFILE** %34, align 8
  store i32 (%struct.__sFILE*)* %80, i32 (%struct.__sFILE*)** %35, align 8
  %81 = load %"class.std::__1::__libcpp_compressed_pair_imp.57"*, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %33, align 8
  %82 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.57", %"class.std::__1::__libcpp_compressed_pair_imp.57"* %81, i32 0, i32 0
  store %struct.__sFILE** %34, %struct.__sFILE*** %32, align 8
  %83 = load %struct.__sFILE**, %struct.__sFILE*** %32, align 8
  %84 = load %struct.__sFILE*, %struct.__sFILE** %83, align 8
  store %struct.__sFILE* %84, %struct.__sFILE** %82, align 8
  %85 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.57", %"class.std::__1::__libcpp_compressed_pair_imp.57"* %81, i32 0, i32 1
  store i32 (%struct.__sFILE*)** %35, i32 (%struct.__sFILE*)*** %31, align 8
  %86 = load i32 (%struct.__sFILE*)**, i32 (%struct.__sFILE*)*** %31, align 8
  %87 = load i32 (%struct.__sFILE*)*, i32 (%struct.__sFILE*)** %86, align 8
  store i32 (%struct.__sFILE*)* %87, i32 (%struct.__sFILE*)** %85, align 8
  %88 = bitcast %"class.std::__1::basic_filebuf"* %56 to i32 (%"class.std::__1::basic_filebuf"*)***
  %89 = load i32 (%"class.std::__1::basic_filebuf"*)**, i32 (%"class.std::__1::basic_filebuf"*)*** %88, align 8
  %90 = getelementptr inbounds i32 (%"class.std::__1::basic_filebuf"*)*, i32 (%"class.std::__1::basic_filebuf"*)** %89, i64 6
  %91 = load i32 (%"class.std::__1::basic_filebuf"*)*, i32 (%"class.std::__1::basic_filebuf"*)** %90, align 8
  %92 = invoke i32 %91(%"class.std::__1::basic_filebuf"* %56)
          to label %93 unwind label %96

; <label>:93:                                     ; preds = %60
  %94 = icmp ne i32 %92, 0
  br i1 %94, label %95, label %131

; <label>:95:                                     ; preds = %93
  store %"class.std::__1::basic_filebuf"* null, %"class.std::__1::basic_filebuf"** %51, align 8
  br label %131

; <label>:96:                                     ; preds = %131, %60
  %97 = landingpad { i8*, i32 }
          cleanup
  %98 = extractvalue { i8*, i32 } %97, 0
  store i8* %98, i8** %54, align 8
  %99 = extractvalue { i8*, i32 } %97, 1
  store i32 %99, i32* %55, align 4
  store %"class.std::__1::unique_ptr.55"* %52, %"class.std::__1::unique_ptr.55"** %18, align 8
  %100 = load %"class.std::__1::unique_ptr.55"*, %"class.std::__1::unique_ptr.55"** %18, align 8
  store %"class.std::__1::unique_ptr.55"* %100, %"class.std::__1::unique_ptr.55"** %17, align 8
  %101 = load %"class.std::__1::unique_ptr.55"*, %"class.std::__1::unique_ptr.55"** %17, align 8
  store %"class.std::__1::unique_ptr.55"* %101, %"class.std::__1::unique_ptr.55"** %14, align 8
  store %struct.__sFILE* null, %struct.__sFILE** %15, align 8
  %102 = load %"class.std::__1::unique_ptr.55"*, %"class.std::__1::unique_ptr.55"** %14, align 8
  %103 = getelementptr inbounds %"class.std::__1::unique_ptr.55", %"class.std::__1::unique_ptr.55"* %102, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.56"* %103, %"class.std::__1::__compressed_pair.56"** %13, align 8
  %104 = load %"class.std::__1::__compressed_pair.56"*, %"class.std::__1::__compressed_pair.56"** %13, align 8
  %105 = bitcast %"class.std::__1::__compressed_pair.56"* %104 to %"class.std::__1::__libcpp_compressed_pair_imp.57"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.57"* %105, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %12, align 8
  %106 = load %"class.std::__1::__libcpp_compressed_pair_imp.57"*, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %12, align 8
  %107 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.57", %"class.std::__1::__libcpp_compressed_pair_imp.57"* %106, i32 0, i32 0
  %108 = load %struct.__sFILE*, %struct.__sFILE** %107, align 8
  store %struct.__sFILE* %108, %struct.__sFILE** %16, align 8
  %109 = load %struct.__sFILE*, %struct.__sFILE** %15, align 8
  %110 = getelementptr inbounds %"class.std::__1::unique_ptr.55", %"class.std::__1::unique_ptr.55"* %102, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.56"* %110, %"class.std::__1::__compressed_pair.56"** %11, align 8
  %111 = load %"class.std::__1::__compressed_pair.56"*, %"class.std::__1::__compressed_pair.56"** %11, align 8
  %112 = bitcast %"class.std::__1::__compressed_pair.56"* %111 to %"class.std::__1::__libcpp_compressed_pair_imp.57"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.57"* %112, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %10, align 8
  %113 = load %"class.std::__1::__libcpp_compressed_pair_imp.57"*, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %10, align 8
  %114 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.57", %"class.std::__1::__libcpp_compressed_pair_imp.57"* %113, i32 0, i32 0
  store %struct.__sFILE* %109, %struct.__sFILE** %114, align 8
  %115 = load %struct.__sFILE*, %struct.__sFILE** %16, align 8
  %116 = icmp ne %struct.__sFILE* %115, null
  br i1 %116, label %117, label %130

; <label>:117:                                    ; preds = %96
  %118 = getelementptr inbounds %"class.std::__1::unique_ptr.55", %"class.std::__1::unique_ptr.55"* %102, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.56"* %118, %"class.std::__1::__compressed_pair.56"** %9, align 8
  %119 = load %"class.std::__1::__compressed_pair.56"*, %"class.std::__1::__compressed_pair.56"** %9, align 8
  %120 = bitcast %"class.std::__1::__compressed_pair.56"* %119 to %"class.std::__1::__libcpp_compressed_pair_imp.57"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.57"* %120, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %8, align 8
  %121 = load %"class.std::__1::__libcpp_compressed_pair_imp.57"*, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %8, align 8
  %122 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.57", %"class.std::__1::__libcpp_compressed_pair_imp.57"* %121, i32 0, i32 1
  %123 = load i32 (%struct.__sFILE*)*, i32 (%struct.__sFILE*)** %122, align 8
  %124 = load %struct.__sFILE*, %struct.__sFILE** %16, align 8
  %125 = invoke i32 %123(%struct.__sFILE* %124)
          to label %126 unwind label %127

; <label>:126:                                    ; preds = %117
  br label %130

; <label>:127:                                    ; preds = %117
  %128 = landingpad { i8*, i32 }
          catch i8* null
  %129 = extractvalue { i8*, i32 } %128, 0
  call void @__clang_call_terminate(i8* %129) #16
  unreachable

; <label>:130:                                    ; preds = %96, %126
  br label %185

; <label>:131:                                    ; preds = %95, %93
  store %"class.std::__1::unique_ptr.55"* %52, %"class.std::__1::unique_ptr.55"** %6, align 8
  %132 = load %"class.std::__1::unique_ptr.55"*, %"class.std::__1::unique_ptr.55"** %6, align 8
  %133 = getelementptr inbounds %"class.std::__1::unique_ptr.55", %"class.std::__1::unique_ptr.55"* %132, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.56"* %133, %"class.std::__1::__compressed_pair.56"** %5, align 8
  %134 = load %"class.std::__1::__compressed_pair.56"*, %"class.std::__1::__compressed_pair.56"** %5, align 8
  %135 = bitcast %"class.std::__1::__compressed_pair.56"* %134 to %"class.std::__1::__libcpp_compressed_pair_imp.57"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.57"* %135, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %4, align 8
  %136 = load %"class.std::__1::__libcpp_compressed_pair_imp.57"*, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %4, align 8
  %137 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.57", %"class.std::__1::__libcpp_compressed_pair_imp.57"* %136, i32 0, i32 0
  %138 = load %struct.__sFILE*, %struct.__sFILE** %137, align 8
  store %struct.__sFILE* %138, %struct.__sFILE** %7, align 8
  %139 = getelementptr inbounds %"class.std::__1::unique_ptr.55", %"class.std::__1::unique_ptr.55"* %132, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.56"* %139, %"class.std::__1::__compressed_pair.56"** %3, align 8
  %140 = load %"class.std::__1::__compressed_pair.56"*, %"class.std::__1::__compressed_pair.56"** %3, align 8
  %141 = bitcast %"class.std::__1::__compressed_pair.56"* %140 to %"class.std::__1::__libcpp_compressed_pair_imp.57"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.57"* %141, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %2, align 8
  %142 = load %"class.std::__1::__libcpp_compressed_pair_imp.57"*, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %2, align 8
  %143 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.57", %"class.std::__1::__libcpp_compressed_pair_imp.57"* %142, i32 0, i32 0
  store %struct.__sFILE* null, %struct.__sFILE** %143, align 8
  %144 = load %struct.__sFILE*, %struct.__sFILE** %7, align 8
  %145 = invoke i32 @fclose(%struct.__sFILE* %144)
          to label %146 unwind label %96

; <label>:146:                                    ; preds = %131
  %147 = icmp eq i32 %145, 0
  br i1 %147, label %148, label %150

; <label>:148:                                    ; preds = %146
  %149 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %56, i32 0, i32 8
  store %struct.__sFILE* null, %struct.__sFILE** %149, align 8
  br label %151

; <label>:150:                                    ; preds = %146
  store %"class.std::__1::basic_filebuf"* null, %"class.std::__1::basic_filebuf"** %51, align 8
  br label %151

; <label>:151:                                    ; preds = %150, %148
  store %"class.std::__1::unique_ptr.55"* %52, %"class.std::__1::unique_ptr.55"** %29, align 8
  %152 = load %"class.std::__1::unique_ptr.55"*, %"class.std::__1::unique_ptr.55"** %29, align 8
  store %"class.std::__1::unique_ptr.55"* %152, %"class.std::__1::unique_ptr.55"** %28, align 8
  %153 = load %"class.std::__1::unique_ptr.55"*, %"class.std::__1::unique_ptr.55"** %28, align 8
  store %"class.std::__1::unique_ptr.55"* %153, %"class.std::__1::unique_ptr.55"** %25, align 8
  store %struct.__sFILE* null, %struct.__sFILE** %26, align 8
  %154 = load %"class.std::__1::unique_ptr.55"*, %"class.std::__1::unique_ptr.55"** %25, align 8
  %155 = getelementptr inbounds %"class.std::__1::unique_ptr.55", %"class.std::__1::unique_ptr.55"* %154, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.56"* %155, %"class.std::__1::__compressed_pair.56"** %24, align 8
  %156 = load %"class.std::__1::__compressed_pair.56"*, %"class.std::__1::__compressed_pair.56"** %24, align 8
  %157 = bitcast %"class.std::__1::__compressed_pair.56"* %156 to %"class.std::__1::__libcpp_compressed_pair_imp.57"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.57"* %157, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %23, align 8
  %158 = load %"class.std::__1::__libcpp_compressed_pair_imp.57"*, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %23, align 8
  %159 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.57", %"class.std::__1::__libcpp_compressed_pair_imp.57"* %158, i32 0, i32 0
  %160 = load %struct.__sFILE*, %struct.__sFILE** %159, align 8
  store %struct.__sFILE* %160, %struct.__sFILE** %27, align 8
  %161 = load %struct.__sFILE*, %struct.__sFILE** %26, align 8
  %162 = getelementptr inbounds %"class.std::__1::unique_ptr.55", %"class.std::__1::unique_ptr.55"* %154, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.56"* %162, %"class.std::__1::__compressed_pair.56"** %22, align 8
  %163 = load %"class.std::__1::__compressed_pair.56"*, %"class.std::__1::__compressed_pair.56"** %22, align 8
  %164 = bitcast %"class.std::__1::__compressed_pair.56"* %163 to %"class.std::__1::__libcpp_compressed_pair_imp.57"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.57"* %164, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %21, align 8
  %165 = load %"class.std::__1::__libcpp_compressed_pair_imp.57"*, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %21, align 8
  %166 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.57", %"class.std::__1::__libcpp_compressed_pair_imp.57"* %165, i32 0, i32 0
  store %struct.__sFILE* %161, %struct.__sFILE** %166, align 8
  %167 = load %struct.__sFILE*, %struct.__sFILE** %27, align 8
  %168 = icmp ne %struct.__sFILE* %167, null
  br i1 %168, label %169, label %182

; <label>:169:                                    ; preds = %151
  %170 = getelementptr inbounds %"class.std::__1::unique_ptr.55", %"class.std::__1::unique_ptr.55"* %154, i32 0, i32 0
  store %"class.std::__1::__compressed_pair.56"* %170, %"class.std::__1::__compressed_pair.56"** %20, align 8
  %171 = load %"class.std::__1::__compressed_pair.56"*, %"class.std::__1::__compressed_pair.56"** %20, align 8
  %172 = bitcast %"class.std::__1::__compressed_pair.56"* %171 to %"class.std::__1::__libcpp_compressed_pair_imp.57"*
  store %"class.std::__1::__libcpp_compressed_pair_imp.57"* %172, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %19, align 8
  %173 = load %"class.std::__1::__libcpp_compressed_pair_imp.57"*, %"class.std::__1::__libcpp_compressed_pair_imp.57"** %19, align 8
  %174 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp.57", %"class.std::__1::__libcpp_compressed_pair_imp.57"* %173, i32 0, i32 1
  %175 = load i32 (%struct.__sFILE*)*, i32 (%struct.__sFILE*)** %174, align 8
  %176 = load %struct.__sFILE*, %struct.__sFILE** %27, align 8
  %177 = invoke i32 %175(%struct.__sFILE* %176)
          to label %178 unwind label %179

; <label>:178:                                    ; preds = %169
  br label %182

; <label>:179:                                    ; preds = %169
  %180 = landingpad { i8*, i32 }
          catch i8* null
  %181 = extractvalue { i8*, i32 } %180, 0
  call void @__clang_call_terminate(i8* %181) #16
  unreachable

; <label>:182:                                    ; preds = %151, %178
  br label %183

; <label>:183:                                    ; preds = %182, %1
  %184 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %51, align 8
  ret %"class.std::__1::basic_filebuf"* %184

; <label>:185:                                    ; preds = %130
  %186 = load i8*, i8** %54, align 8
  %187 = load i32, i32* %55, align 4
  %188 = insertvalue { i8*, i32 } undef, i8* %186, 0
  %189 = insertvalue { i8*, i32 } %188, i32 %187, 1
  resume { i8*, i32 } %189
}

; Function Attrs: nobuiltin nounwind
declare void @_ZdaPv(i8*) #10

; Function Attrs: nounwind
declare void @_ZNSt3__115basic_streambufIcNS_11char_traitsIcEEED2Ev(%"class.std::__1::basic_streambuf"*) unnamed_addr #3

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEED0Ev(%"class.std::__1::basic_filebuf"*) unnamed_addr #9 align 2 {
  %2 = alloca %"class.std::__1::basic_filebuf"*, align 8
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %2, align 8
  %3 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %2, align 8
  call void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEED1Ev(%"class.std::__1::basic_filebuf"* %3) #15
  %4 = bitcast %"class.std::__1::basic_filebuf"* %3 to i8*
  call void @_ZdlPv(i8* %4) #17
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE5imbueERKNS_6localeE(%"class.std::__1::basic_filebuf"*, %"class.std::__1::locale"* dereferenceable(8)) unnamed_addr #0 align 2 {
  %3 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %4 = alloca i8*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i8*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca %"class.std::__1::codecvt"*, align 8
  %11 = alloca %"class.std::__1::locale"*, align 8
  %12 = alloca %"class.std::__1::basic_filebuf"*, align 8
  %13 = alloca %"class.std::__1::locale"*, align 8
  %14 = alloca i8, align 1
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %12, align 8
  store %"class.std::__1::locale"* %1, %"class.std::__1::locale"** %13, align 8
  %15 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %12, align 8
  %16 = bitcast %"class.std::__1::basic_filebuf"* %15 to i32 (%"class.std::__1::basic_filebuf"*)***
  %17 = load i32 (%"class.std::__1::basic_filebuf"*)**, i32 (%"class.std::__1::basic_filebuf"*)*** %16, align 8
  %18 = getelementptr inbounds i32 (%"class.std::__1::basic_filebuf"*)*, i32 (%"class.std::__1::basic_filebuf"*)** %17, i64 6
  %19 = load i32 (%"class.std::__1::basic_filebuf"*)*, i32 (%"class.std::__1::basic_filebuf"*)** %18, align 8
  %20 = call i32 %19(%"class.std::__1::basic_filebuf"* %15)
  %21 = load %"class.std::__1::locale"*, %"class.std::__1::locale"** %13, align 8
  store %"class.std::__1::locale"* %21, %"class.std::__1::locale"** %11, align 8
  %22 = load %"class.std::__1::locale"*, %"class.std::__1::locale"** %11, align 8
  %23 = call %"class.std::__1::locale::facet"* @_ZNKSt3__16locale9use_facetERNS0_2idE(%"class.std::__1::locale"* %22, %"class.std::__1::locale::id"* dereferenceable(16) @_ZNSt3__17codecvtIcc11__mbstate_tE2idE)
  %24 = bitcast %"class.std::__1::locale::facet"* %23 to %"class.std::__1::codecvt"*
  %25 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 9
  store %"class.std::__1::codecvt"* %24, %"class.std::__1::codecvt"** %25, align 8
  %26 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 16
  %27 = load i8, i8* %26, align 2
  %28 = trunc i8 %27 to i1
  %29 = zext i1 %28 to i8
  store i8 %29, i8* %14, align 1
  %30 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 9
  %31 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %30, align 8
  store %"class.std::__1::codecvt"* %31, %"class.std::__1::codecvt"** %10, align 8
  %32 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %10, align 8
  %33 = bitcast %"class.std::__1::codecvt"* %32 to i1 (%"class.std::__1::codecvt"*)***
  %34 = load i1 (%"class.std::__1::codecvt"*)**, i1 (%"class.std::__1::codecvt"*)*** %33, align 8
  %35 = getelementptr inbounds i1 (%"class.std::__1::codecvt"*)*, i1 (%"class.std::__1::codecvt"*)** %34, i64 7
  %36 = load i1 (%"class.std::__1::codecvt"*)*, i1 (%"class.std::__1::codecvt"*)** %35, align 8
  %37 = call zeroext i1 %36(%"class.std::__1::codecvt"* %32) #15
  %38 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 16
  %39 = zext i1 %37 to i8
  store i8 %39, i8* %38, align 2
  %40 = load i8, i8* %14, align 1
  %41 = trunc i8 %40 to i1
  %42 = zext i1 %41 to i32
  %43 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 16
  %44 = load i8, i8* %43, align 2
  %45 = trunc i8 %44 to i1
  %46 = zext i1 %45 to i32
  %47 = icmp ne i32 %42, %46
  br i1 %47, label %48, label %126

; <label>:48:                                     ; preds = %2
  %49 = bitcast %"class.std::__1::basic_filebuf"* %15 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %49, %"class.std::__1::basic_streambuf"** %6, align 8
  store i8* null, i8** %7, align 8
  store i8* null, i8** %8, align 8
  store i8* null, i8** %9, align 8
  %50 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %6, align 8
  %51 = load i8*, i8** %7, align 8
  %52 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %50, i32 0, i32 2
  store i8* %51, i8** %52, align 8
  %53 = load i8*, i8** %8, align 8
  %54 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %50, i32 0, i32 3
  store i8* %53, i8** %54, align 8
  %55 = load i8*, i8** %9, align 8
  %56 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %50, i32 0, i32 4
  store i8* %55, i8** %56, align 8
  %57 = bitcast %"class.std::__1::basic_filebuf"* %15 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %57, %"class.std::__1::basic_streambuf"** %3, align 8
  store i8* null, i8** %4, align 8
  store i8* null, i8** %5, align 8
  %58 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %3, align 8
  %59 = load i8*, i8** %4, align 8
  %60 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %58, i32 0, i32 6
  store i8* %59, i8** %60, align 8
  %61 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %58, i32 0, i32 5
  store i8* %59, i8** %61, align 8
  %62 = load i8*, i8** %5, align 8
  %63 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %58, i32 0, i32 7
  store i8* %62, i8** %63, align 8
  %64 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 16
  %65 = load i8, i8* %64, align 2
  %66 = trunc i8 %65 to i1
  br i1 %66, label %67, label %92

; <label>:67:                                     ; preds = %48
  %68 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 14
  %69 = load i8, i8* %68, align 8
  %70 = trunc i8 %69 to i1
  br i1 %70, label %71, label %77

; <label>:71:                                     ; preds = %67
  %72 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 1
  %73 = load i8*, i8** %72, align 8
  %74 = icmp eq i8* %73, null
  br i1 %74, label %76, label %75

; <label>:75:                                     ; preds = %71
  call void @_ZdaPv(i8* %73) #17
  br label %76

; <label>:76:                                     ; preds = %75, %71
  br label %77

; <label>:77:                                     ; preds = %76, %67
  %78 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 15
  %79 = load i8, i8* %78, align 1
  %80 = trunc i8 %79 to i1
  %81 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 14
  %82 = zext i1 %80 to i8
  store i8 %82, i8* %81, align 8
  %83 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 7
  %84 = load i64, i64* %83, align 8
  %85 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 5
  store i64 %84, i64* %85, align 8
  %86 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 6
  %87 = load i8*, i8** %86, align 8
  %88 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 1
  store i8* %87, i8** %88, align 8
  %89 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 7
  store i64 0, i64* %89, align 8
  %90 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 6
  store i8* null, i8** %90, align 8
  %91 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 15
  store i8 0, i8* %91, align 1
  br label %125

; <label>:92:                                     ; preds = %48
  %93 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 14
  %94 = load i8, i8* %93, align 8
  %95 = trunc i8 %94 to i1
  br i1 %95, label %115, label %96

; <label>:96:                                     ; preds = %92
  %97 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 1
  %98 = load i8*, i8** %97, align 8
  %99 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 4
  %100 = getelementptr inbounds [8 x i8], [8 x i8]* %99, i32 0, i32 0
  %101 = icmp ne i8* %98, %100
  br i1 %101, label %102, label %115

; <label>:102:                                    ; preds = %96
  %103 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 5
  %104 = load i64, i64* %103, align 8
  %105 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 7
  store i64 %104, i64* %105, align 8
  %106 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 1
  %107 = load i8*, i8** %106, align 8
  %108 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 6
  store i8* %107, i8** %108, align 8
  %109 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 15
  store i8 0, i8* %109, align 1
  %110 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 5
  %111 = load i64, i64* %110, align 8
  %112 = call i8* @_Znam(i64 %111) #18
  %113 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 1
  store i8* %112, i8** %113, align 8
  %114 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 14
  store i8 1, i8* %114, align 8
  br label %124

; <label>:115:                                    ; preds = %96, %92
  %116 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 5
  %117 = load i64, i64* %116, align 8
  %118 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 7
  store i64 %117, i64* %118, align 8
  %119 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 7
  %120 = load i64, i64* %119, align 8
  %121 = call i8* @_Znam(i64 %120) #18
  %122 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 6
  store i8* %121, i8** %122, align 8
  %123 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 15
  store i8 1, i8* %123, align 1
  br label %124

; <label>:124:                                    ; preds = %115, %102
  br label %125

; <label>:125:                                    ; preds = %124, %77
  br label %126

; <label>:126:                                    ; preds = %125, %2
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.std::__1::basic_streambuf"* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE6setbufEPcl(%"class.std::__1::basic_filebuf"*, i8*, i64) unnamed_addr #0 align 2 {
  %4 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca %"struct.std::__1::__less"*, align 8
  %8 = alloca i64*, align 8
  %9 = alloca i64*, align 8
  %10 = alloca %"struct.std::__1::__less", align 1
  %11 = alloca i64*, align 8
  %12 = alloca i64*, align 8
  %13 = alloca i64*, align 8
  %14 = alloca i64*, align 8
  %15 = alloca %"struct.std::__1::__less", align 1
  %16 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %17 = alloca i8*, align 8
  %18 = alloca i8*, align 8
  %19 = alloca i8*, align 8
  %20 = alloca %"class.std::__1::basic_filebuf"*, align 8
  %21 = alloca i8*, align 8
  %22 = alloca i64, align 8
  %23 = alloca i64, align 8
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %20, align 8
  store i8* %1, i8** %21, align 8
  store i64 %2, i64* %22, align 8
  %24 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %20, align 8
  %25 = bitcast %"class.std::__1::basic_filebuf"* %24 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %25, %"class.std::__1::basic_streambuf"** %16, align 8
  store i8* null, i8** %17, align 8
  store i8* null, i8** %18, align 8
  store i8* null, i8** %19, align 8
  %26 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %16, align 8
  %27 = load i8*, i8** %17, align 8
  %28 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %26, i32 0, i32 2
  store i8* %27, i8** %28, align 8
  %29 = load i8*, i8** %18, align 8
  %30 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %26, i32 0, i32 3
  store i8* %29, i8** %30, align 8
  %31 = load i8*, i8** %19, align 8
  %32 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %26, i32 0, i32 4
  store i8* %31, i8** %32, align 8
  %33 = bitcast %"class.std::__1::basic_filebuf"* %24 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %33, %"class.std::__1::basic_streambuf"** %4, align 8
  store i8* null, i8** %5, align 8
  store i8* null, i8** %6, align 8
  %34 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %4, align 8
  %35 = load i8*, i8** %5, align 8
  %36 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %34, i32 0, i32 6
  store i8* %35, i8** %36, align 8
  %37 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %34, i32 0, i32 5
  store i8* %35, i8** %37, align 8
  %38 = load i8*, i8** %6, align 8
  %39 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %34, i32 0, i32 7
  store i8* %38, i8** %39, align 8
  %40 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 14
  %41 = load i8, i8* %40, align 8
  %42 = trunc i8 %41 to i1
  br i1 %42, label %43, label %49

; <label>:43:                                     ; preds = %3
  %44 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 1
  %45 = load i8*, i8** %44, align 8
  %46 = icmp eq i8* %45, null
  br i1 %46, label %48, label %47

; <label>:47:                                     ; preds = %43
  call void @_ZdaPv(i8* %45) #17
  br label %48

; <label>:48:                                     ; preds = %47, %43
  br label %49

; <label>:49:                                     ; preds = %48, %3
  %50 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 15
  %51 = load i8, i8* %50, align 1
  %52 = trunc i8 %51 to i1
  br i1 %52, label %53, label %59

; <label>:53:                                     ; preds = %49
  %54 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 6
  %55 = load i8*, i8** %54, align 8
  %56 = icmp eq i8* %55, null
  br i1 %56, label %58, label %57

; <label>:57:                                     ; preds = %53
  call void @_ZdaPv(i8* %55) #17
  br label %58

; <label>:58:                                     ; preds = %57, %53
  br label %59

; <label>:59:                                     ; preds = %58, %49
  %60 = load i64, i64* %22, align 8
  %61 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 5
  store i64 %60, i64* %61, align 8
  %62 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 5
  %63 = load i64, i64* %62, align 8
  %64 = icmp ugt i64 %63, 8
  br i1 %64, label %65, label %83

; <label>:65:                                     ; preds = %59
  %66 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 16
  %67 = load i8, i8* %66, align 2
  %68 = trunc i8 %67 to i1
  br i1 %68, label %69, label %76

; <label>:69:                                     ; preds = %65
  %70 = load i8*, i8** %21, align 8
  %71 = icmp ne i8* %70, null
  br i1 %71, label %72, label %76

; <label>:72:                                     ; preds = %69
  %73 = load i8*, i8** %21, align 8
  %74 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 1
  store i8* %73, i8** %74, align 8
  %75 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 14
  store i8 0, i8* %75, align 8
  br label %82

; <label>:76:                                     ; preds = %69, %65
  %77 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 5
  %78 = load i64, i64* %77, align 8
  %79 = call i8* @_Znam(i64 %78) #18
  %80 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 1
  store i8* %79, i8** %80, align 8
  %81 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 14
  store i8 1, i8* %81, align 8
  br label %82

; <label>:82:                                     ; preds = %76, %72
  br label %89

; <label>:83:                                     ; preds = %59
  %84 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 4
  %85 = getelementptr inbounds [8 x i8], [8 x i8]* %84, i32 0, i32 0
  %86 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 1
  store i8* %85, i8** %86, align 8
  %87 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 5
  store i64 8, i64* %87, align 8
  %88 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 14
  store i8 0, i8* %88, align 8
  br label %89

; <label>:89:                                     ; preds = %83, %82
  %90 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 16
  %91 = load i8, i8* %90, align 2
  %92 = trunc i8 %91 to i1
  br i1 %92, label %129, label %93

; <label>:93:                                     ; preds = %89
  store i64 8, i64* %23, align 8
  store i64* %22, i64** %13, align 8
  store i64* %23, i64** %14, align 8
  %94 = load i64*, i64** %13, align 8
  %95 = load i64*, i64** %14, align 8
  store i64* %94, i64** %11, align 8
  store i64* %95, i64** %12, align 8
  %96 = load i64*, i64** %11, align 8
  %97 = load i64*, i64** %12, align 8
  store %"struct.std::__1::__less"* %10, %"struct.std::__1::__less"** %7, align 8
  store i64* %96, i64** %8, align 8
  store i64* %97, i64** %9, align 8
  %98 = load %"struct.std::__1::__less"*, %"struct.std::__1::__less"** %7, align 8
  %99 = load i64*, i64** %8, align 8
  %100 = load i64, i64* %99, align 8
  %101 = load i64*, i64** %9, align 8
  %102 = load i64, i64* %101, align 8
  %103 = icmp slt i64 %100, %102
  br i1 %103, label %104, label %106

; <label>:104:                                    ; preds = %93
  %105 = load i64*, i64** %12, align 8
  br label %108

; <label>:106:                                    ; preds = %93
  %107 = load i64*, i64** %11, align 8
  br label %108

; <label>:108:                                    ; preds = %104, %106
  %109 = phi i64* [ %105, %104 ], [ %107, %106 ]
  %110 = load i64, i64* %109, align 8
  %111 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 7
  store i64 %110, i64* %111, align 8
  %112 = load i8*, i8** %21, align 8
  %113 = icmp ne i8* %112, null
  br i1 %113, label %114, label %122

; <label>:114:                                    ; preds = %108
  %115 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 7
  %116 = load i64, i64* %115, align 8
  %117 = icmp uge i64 %116, 8
  br i1 %117, label %118, label %122

; <label>:118:                                    ; preds = %114
  %119 = load i8*, i8** %21, align 8
  %120 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 6
  store i8* %119, i8** %120, align 8
  %121 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 15
  store i8 0, i8* %121, align 1
  br label %128

; <label>:122:                                    ; preds = %114, %108
  %123 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 7
  %124 = load i64, i64* %123, align 8
  %125 = call i8* @_Znam(i64 %124) #18
  %126 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 6
  store i8* %125, i8** %126, align 8
  %127 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 15
  store i8 1, i8* %127, align 1
  br label %128

; <label>:128:                                    ; preds = %122, %118
  br label %133

; <label>:129:                                    ; preds = %89
  %130 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 7
  store i64 0, i64* %130, align 8
  %131 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 6
  store i8* null, i8** %131, align 8
  %132 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %24, i32 0, i32 15
  store i8 0, i8* %132, align 1
  br label %133

; <label>:133:                                    ; preds = %129, %128
  %134 = bitcast %"class.std::__1::basic_filebuf"* %24 to %"class.std::__1::basic_streambuf"*
  ret %"class.std::__1::basic_streambuf"* %134
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE7seekoffExNS_8ios_base7seekdirEj(%"class.std::__1::fpos"* noalias sret, %"class.std::__1::basic_filebuf"*, i64, i32, i32) unnamed_addr #0 align 2 {
  %6 = alloca %"class.std::__1::fpos"*, align 8
  %7 = alloca i64, align 8
  %8 = alloca %"class.std::__1::fpos"*, align 8
  %9 = alloca i64, align 8
  %10 = alloca %"class.std::__1::fpos"*, align 8
  %11 = alloca i64, align 8
  %12 = alloca %"class.std::__1::fpos"*, align 8
  %13 = alloca i64, align 8
  %14 = alloca %"class.std::__1::fpos"*, align 8
  %15 = alloca i64, align 8
  %16 = alloca %"class.std::__1::fpos"*, align 8
  %17 = alloca i64, align 8
  %18 = alloca %"class.std::__1::fpos"*, align 8
  %19 = alloca %union.__mbstate_t, align 8
  %20 = alloca %"class.std::__1::fpos"*, align 8
  %21 = alloca i64, align 8
  %22 = alloca %"class.std::__1::fpos"*, align 8
  %23 = alloca i64, align 8
  %24 = alloca %"class.std::__1::codecvt"*, align 8
  %25 = alloca %"class.std::__1::basic_filebuf"*, align 8
  %26 = alloca i64, align 8
  %27 = alloca i32, align 4
  %28 = alloca i32, align 4
  %29 = alloca i32, align 4
  %30 = alloca i32, align 4
  %31 = alloca %"class.std::__1::fpos", align 8
  %32 = alloca %union.__mbstate_t, align 8
  store %"class.std::__1::basic_filebuf"* %1, %"class.std::__1::basic_filebuf"** %25, align 8
  store i64 %2, i64* %26, align 8
  store i32 %3, i32* %27, align 4
  store i32 %4, i32* %28, align 4
  %33 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %25, align 8
  %34 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %33, i32 0, i32 9
  %35 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %34, align 8
  %36 = icmp ne %"class.std::__1::codecvt"* %35, null
  br i1 %36, label %40, label %37

; <label>:37:                                     ; preds = %5
  %38 = call i8* @__cxa_allocate_exception(i64 8) #15
  %39 = bitcast i8* %38 to %"class.std::bad_cast"*
  call void @_ZNSt8bad_castC1Ev(%"class.std::bad_cast"* %39) #15
  call void @__cxa_throw(i8* %38, i8* bitcast (i8** @_ZTISt8bad_cast to i8*), i8* bitcast (void (%"class.std::bad_cast"*)* @_ZNSt8bad_castD1Ev to i8*)) #14
  unreachable

; <label>:40:                                     ; preds = %5
  %41 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %33, i32 0, i32 9
  %42 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %41, align 8
  store %"class.std::__1::codecvt"* %42, %"class.std::__1::codecvt"** %24, align 8
  %43 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %24, align 8
  %44 = bitcast %"class.std::__1::codecvt"* %43 to i32 (%"class.std::__1::codecvt"*)***
  %45 = load i32 (%"class.std::__1::codecvt"*)**, i32 (%"class.std::__1::codecvt"*)*** %44, align 8
  %46 = getelementptr inbounds i32 (%"class.std::__1::codecvt"*)*, i32 (%"class.std::__1::codecvt"*)** %45, i64 6
  %47 = load i32 (%"class.std::__1::codecvt"*)*, i32 (%"class.std::__1::codecvt"*)** %46, align 8
  %48 = call i32 %47(%"class.std::__1::codecvt"* %43) #15
  store i32 %48, i32* %29, align 4
  %49 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %33, i32 0, i32 8
  %50 = load %struct.__sFILE*, %struct.__sFILE** %49, align 8
  %51 = icmp eq %struct.__sFILE* %50, null
  br i1 %51, label %65, label %52

; <label>:52:                                     ; preds = %40
  %53 = load i32, i32* %29, align 4
  %54 = icmp sle i32 %53, 0
  br i1 %54, label %55, label %58

; <label>:55:                                     ; preds = %52
  %56 = load i64, i64* %26, align 8
  %57 = icmp ne i64 %56, 0
  br i1 %57, label %65, label %58

; <label>:58:                                     ; preds = %55, %52
  %59 = bitcast %"class.std::__1::basic_filebuf"* %33 to i32 (%"class.std::__1::basic_filebuf"*)***
  %60 = load i32 (%"class.std::__1::basic_filebuf"*)**, i32 (%"class.std::__1::basic_filebuf"*)*** %59, align 8
  %61 = getelementptr inbounds i32 (%"class.std::__1::basic_filebuf"*)*, i32 (%"class.std::__1::basic_filebuf"*)** %60, i64 6
  %62 = load i32 (%"class.std::__1::basic_filebuf"*)*, i32 (%"class.std::__1::basic_filebuf"*)** %61, align 8
  %63 = call i32 %62(%"class.std::__1::basic_filebuf"* %33)
  %64 = icmp ne i32 %63, 0
  br i1 %64, label %65, label %73

; <label>:65:                                     ; preds = %58, %55, %40
  store %"class.std::__1::fpos"* %0, %"class.std::__1::fpos"** %22, align 8
  store i64 -1, i64* %23, align 8
  %66 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %22, align 8
  %67 = load i64, i64* %23, align 8
  store %"class.std::__1::fpos"* %66, %"class.std::__1::fpos"** %20, align 8
  store i64 %67, i64* %21, align 8
  %68 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %20, align 8
  %69 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %68, i32 0, i32 0
  %70 = bitcast %union.__mbstate_t* %69 to i8*
  call void @llvm.memset.p0i8.i64(i8* %70, i8 0, i64 128, i32 8, i1 false) #15
  %71 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %68, i32 0, i32 1
  %72 = load i64, i64* %21, align 8
  store i64 %72, i64* %71, align 8
  br label %132

; <label>:73:                                     ; preds = %58
  %74 = load i32, i32* %27, align 4
  switch i32 %74, label %78 [
    i32 0, label %75
    i32 1, label %76
    i32 2, label %77
  ]

; <label>:75:                                     ; preds = %73
  store i32 0, i32* %30, align 4
  br label %86

; <label>:76:                                     ; preds = %73
  store i32 1, i32* %30, align 4
  br label %86

; <label>:77:                                     ; preds = %73
  store i32 2, i32* %30, align 4
  br label %86

; <label>:78:                                     ; preds = %73
  store %"class.std::__1::fpos"* %0, %"class.std::__1::fpos"** %8, align 8
  store i64 -1, i64* %9, align 8
  %79 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %8, align 8
  %80 = load i64, i64* %9, align 8
  store %"class.std::__1::fpos"* %79, %"class.std::__1::fpos"** %6, align 8
  store i64 %80, i64* %7, align 8
  %81 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %6, align 8
  %82 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %81, i32 0, i32 0
  %83 = bitcast %union.__mbstate_t* %82 to i8*
  call void @llvm.memset.p0i8.i64(i8* %83, i8 0, i64 128, i32 8, i1 false) #15
  %84 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %81, i32 0, i32 1
  %85 = load i64, i64* %7, align 8
  store i64 %85, i64* %84, align 8
  br label %132

; <label>:86:                                     ; preds = %77, %76, %75
  %87 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %33, i32 0, i32 8
  %88 = load %struct.__sFILE*, %struct.__sFILE** %87, align 8
  %89 = load i32, i32* %29, align 4
  %90 = icmp sgt i32 %89, 0
  br i1 %90, label %91, label %96

; <label>:91:                                     ; preds = %86
  %92 = load i32, i32* %29, align 4
  %93 = sext i32 %92 to i64
  %94 = load i64, i64* %26, align 8
  %95 = mul nsw i64 %93, %94
  br label %97

; <label>:96:                                     ; preds = %86
  br label %97

; <label>:97:                                     ; preds = %96, %91
  %98 = phi i64 [ %95, %91 ], [ 0, %96 ]
  %99 = load i32, i32* %30, align 4
  %100 = call i32 @fseeko(%struct.__sFILE* %88, i64 %98, i32 %99)
  %101 = icmp ne i32 %100, 0
  br i1 %101, label %102, label %110

; <label>:102:                                    ; preds = %97
  store %"class.std::__1::fpos"* %0, %"class.std::__1::fpos"** %12, align 8
  store i64 -1, i64* %13, align 8
  %103 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %12, align 8
  %104 = load i64, i64* %13, align 8
  store %"class.std::__1::fpos"* %103, %"class.std::__1::fpos"** %10, align 8
  store i64 %104, i64* %11, align 8
  %105 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %10, align 8
  %106 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %105, i32 0, i32 0
  %107 = bitcast %union.__mbstate_t* %106 to i8*
  call void @llvm.memset.p0i8.i64(i8* %107, i8 0, i64 128, i32 8, i1 false) #15
  %108 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %105, i32 0, i32 1
  %109 = load i64, i64* %11, align 8
  store i64 %109, i64* %108, align 8
  br label %132

; <label>:110:                                    ; preds = %97
  %111 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %33, i32 0, i32 8
  %112 = load %struct.__sFILE*, %struct.__sFILE** %111, align 8
  %113 = call i64 @ftello(%struct.__sFILE* %112)
  store %"class.std::__1::fpos"* %31, %"class.std::__1::fpos"** %16, align 8
  store i64 %113, i64* %17, align 8
  %114 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %16, align 8
  %115 = load i64, i64* %17, align 8
  store %"class.std::__1::fpos"* %114, %"class.std::__1::fpos"** %14, align 8
  store i64 %115, i64* %15, align 8
  %116 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %14, align 8
  %117 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %116, i32 0, i32 0
  %118 = bitcast %union.__mbstate_t* %117 to i8*
  call void @llvm.memset.p0i8.i64(i8* %118, i8 0, i64 128, i32 8, i1 false) #15
  %119 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %116, i32 0, i32 1
  %120 = load i64, i64* %15, align 8
  store i64 %120, i64* %119, align 8
  %121 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %33, i32 0, i32 10
  %122 = bitcast %union.__mbstate_t* %32 to i8*
  %123 = bitcast %union.__mbstate_t* %121 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %122, i8* %123, i64 128, i32 8, i1 false)
  %124 = bitcast %union.__mbstate_t* %19 to i8*
  %125 = bitcast %union.__mbstate_t* %32 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %124, i8* %125, i64 128, i32 1, i1 false) #15
  store %"class.std::__1::fpos"* %31, %"class.std::__1::fpos"** %18, align 8
  %126 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %18, align 8
  %127 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %126, i32 0, i32 0
  %128 = bitcast %union.__mbstate_t* %127 to i8*
  %129 = bitcast %union.__mbstate_t* %19 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %128, i8* %129, i64 128, i32 8, i1 false) #15
  %130 = bitcast %"class.std::__1::fpos"* %0 to i8*
  %131 = bitcast %"class.std::__1::fpos"* %31 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %130, i8* %131, i64 136, i32 8, i1 false)
  br label %132

; <label>:132:                                    ; preds = %110, %102, %78, %65
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE7seekposENS_4fposI11__mbstate_tEEj(%"class.std::__1::fpos"* noalias sret, %"class.std::__1::basic_filebuf"*, %"class.std::__1::fpos"* byval align 8, i32) unnamed_addr #0 align 2 {
  %5 = alloca %"class.std::__1::fpos"*, align 8
  %6 = alloca %"class.std::__1::fpos"*, align 8
  %7 = alloca i64, align 8
  %8 = alloca %"class.std::__1::fpos"*, align 8
  %9 = alloca i64, align 8
  %10 = alloca %"class.std::__1::fpos"*, align 8
  %11 = alloca %"class.std::__1::fpos"*, align 8
  %12 = alloca i64, align 8
  %13 = alloca %"class.std::__1::fpos"*, align 8
  %14 = alloca i64, align 8
  %15 = alloca %"class.std::__1::basic_filebuf"*, align 8
  %16 = alloca i32, align 4
  %17 = alloca %union.__mbstate_t, align 8
  store %"class.std::__1::basic_filebuf"* %1, %"class.std::__1::basic_filebuf"** %15, align 8
  store i32 %3, i32* %16, align 4
  %18 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %15, align 8
  %19 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %18, i32 0, i32 8
  %20 = load %struct.__sFILE*, %struct.__sFILE** %19, align 8
  %21 = icmp eq %struct.__sFILE* %20, null
  br i1 %21, label %29, label %22

; <label>:22:                                     ; preds = %4
  %23 = bitcast %"class.std::__1::basic_filebuf"* %18 to i32 (%"class.std::__1::basic_filebuf"*)***
  %24 = load i32 (%"class.std::__1::basic_filebuf"*)**, i32 (%"class.std::__1::basic_filebuf"*)*** %23, align 8
  %25 = getelementptr inbounds i32 (%"class.std::__1::basic_filebuf"*)*, i32 (%"class.std::__1::basic_filebuf"*)** %24, i64 6
  %26 = load i32 (%"class.std::__1::basic_filebuf"*)*, i32 (%"class.std::__1::basic_filebuf"*)** %25, align 8
  %27 = call i32 %26(%"class.std::__1::basic_filebuf"* %18)
  %28 = icmp ne i32 %27, 0
  br i1 %28, label %29, label %37

; <label>:29:                                     ; preds = %22, %4
  store %"class.std::__1::fpos"* %0, %"class.std::__1::fpos"** %13, align 8
  store i64 -1, i64* %14, align 8
  %30 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %13, align 8
  %31 = load i64, i64* %14, align 8
  store %"class.std::__1::fpos"* %30, %"class.std::__1::fpos"** %11, align 8
  store i64 %31, i64* %12, align 8
  %32 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %11, align 8
  %33 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %32, i32 0, i32 0
  %34 = bitcast %union.__mbstate_t* %33 to i8*
  call void @llvm.memset.p0i8.i64(i8* %34, i8 0, i64 128, i32 8, i1 false) #15
  %35 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %32, i32 0, i32 1
  %36 = load i64, i64* %12, align 8
  store i64 %36, i64* %35, align 8
  br label %63

; <label>:37:                                     ; preds = %22
  %38 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %18, i32 0, i32 8
  %39 = load %struct.__sFILE*, %struct.__sFILE** %38, align 8
  store %"class.std::__1::fpos"* %2, %"class.std::__1::fpos"** %5, align 8
  %40 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %5, align 8
  %41 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %40, i32 0, i32 1
  %42 = load i64, i64* %41, align 8
  %43 = call i32 @fseeko(%struct.__sFILE* %39, i64 %42, i32 0)
  %44 = icmp ne i32 %43, 0
  br i1 %44, label %45, label %53

; <label>:45:                                     ; preds = %37
  store %"class.std::__1::fpos"* %0, %"class.std::__1::fpos"** %8, align 8
  store i64 -1, i64* %9, align 8
  %46 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %8, align 8
  %47 = load i64, i64* %9, align 8
  store %"class.std::__1::fpos"* %46, %"class.std::__1::fpos"** %6, align 8
  store i64 %47, i64* %7, align 8
  %48 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %6, align 8
  %49 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %48, i32 0, i32 0
  %50 = bitcast %union.__mbstate_t* %49 to i8*
  call void @llvm.memset.p0i8.i64(i8* %50, i8 0, i64 128, i32 8, i1 false) #15
  %51 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %48, i32 0, i32 1
  %52 = load i64, i64* %7, align 8
  store i64 %52, i64* %51, align 8
  br label %63

; <label>:53:                                     ; preds = %37
  %54 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %18, i32 0, i32 10
  store %"class.std::__1::fpos"* %2, %"class.std::__1::fpos"** %10, align 8, !noalias !2
  %55 = load %"class.std::__1::fpos"*, %"class.std::__1::fpos"** %10, align 8, !noalias !2
  %56 = getelementptr inbounds %"class.std::__1::fpos", %"class.std::__1::fpos"* %55, i32 0, i32 0
  %57 = bitcast %union.__mbstate_t* %17 to i8*
  %58 = bitcast %union.__mbstate_t* %56 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %57, i8* %58, i64 128, i32 8, i1 false) #15
  %59 = bitcast %union.__mbstate_t* %54 to i8*
  %60 = bitcast %union.__mbstate_t* %17 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %59, i8* %60, i64 128, i32 8, i1 false)
  %61 = bitcast %"class.std::__1::fpos"* %0 to i8*
  %62 = bitcast %"class.std::__1::fpos"* %2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %61, i8* %62, i64 136, i32 8, i1 false)
  br label %63

; <label>:63:                                     ; preds = %53, %45, %29
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr i32 @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE4syncEv(%"class.std::__1::basic_filebuf"*) unnamed_addr #0 align 2 {
  %2 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %3 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %4 = alloca %"class.std::__1::codecvt"*, align 8
  %5 = alloca %union.__mbstate_t*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i8**, align 8
  %9 = alloca %"class.std::__1::codecvt"*, align 8
  %10 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %11 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %12 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %13 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %14 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %15 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %16 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %17 = alloca %"class.std::__1::codecvt"*, align 8
  %18 = alloca %union.__mbstate_t*, align 8
  %19 = alloca i8*, align 8
  %20 = alloca i8*, align 8
  %21 = alloca i64, align 8
  %22 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %23 = alloca i8*, align 8
  %24 = alloca i8*, align 8
  %25 = alloca i8*, align 8
  %26 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %27 = alloca i32, align 4
  %28 = alloca %"class.std::__1::basic_filebuf"*, align 8
  %29 = alloca i32, align 4
  %30 = alloca i8*, align 8
  %31 = alloca i64, align 8
  %32 = alloca i64, align 8
  %33 = alloca %union.__mbstate_t, align 8
  %34 = alloca i8, align 1
  %35 = alloca i32, align 4
  %36 = alloca i32, align 4
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %28, align 8
  %37 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %28, align 8
  %38 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 8
  %39 = load %struct.__sFILE*, %struct.__sFILE** %38, align 8
  %40 = icmp eq %struct.__sFILE* %39, null
  br i1 %40, label %41, label %42

; <label>:41:                                     ; preds = %1
  store i32 0, i32* %27, align 4
  br label %269

; <label>:42:                                     ; preds = %1
  %43 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 9
  %44 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %43, align 8
  %45 = icmp ne %"class.std::__1::codecvt"* %44, null
  br i1 %45, label %49, label %46

; <label>:46:                                     ; preds = %42
  %47 = call i8* @__cxa_allocate_exception(i64 8) #15
  %48 = bitcast i8* %47 to %"class.std::bad_cast"*
  call void @_ZNSt8bad_castC1Ev(%"class.std::bad_cast"* %48) #15
  call void @__cxa_throw(i8* %47, i8* bitcast (i8** @_ZTISt8bad_cast to i8*), i8* bitcast (void (%"class.std::bad_cast"*)* @_ZNSt8bad_castD1Ev to i8*)) #14
  unreachable

; <label>:49:                                     ; preds = %42
  %50 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 13
  %51 = load i32, i32* %50, align 4
  %52 = and i32 %51, 16
  %53 = icmp ne i32 %52, 0
  br i1 %53, label %54, label %127

; <label>:54:                                     ; preds = %49
  %55 = bitcast %"class.std::__1::basic_filebuf"* %37 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %55, %"class.std::__1::basic_streambuf"** %26, align 8
  %56 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %26, align 8
  %57 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %56, i32 0, i32 6
  %58 = load i8*, i8** %57, align 8
  %59 = bitcast %"class.std::__1::basic_filebuf"* %37 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %59, %"class.std::__1::basic_streambuf"** %16, align 8
  %60 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %16, align 8
  %61 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %60, i32 0, i32 5
  %62 = load i8*, i8** %61, align 8
  %63 = icmp ne i8* %58, %62
  br i1 %63, label %64, label %75

; <label>:64:                                     ; preds = %54
  %65 = bitcast %"class.std::__1::basic_filebuf"* %37 to i32 (%"class.std::__1::basic_filebuf"*, i32)***
  %66 = load i32 (%"class.std::__1::basic_filebuf"*, i32)**, i32 (%"class.std::__1::basic_filebuf"*, i32)*** %65, align 8
  %67 = getelementptr inbounds i32 (%"class.std::__1::basic_filebuf"*, i32)*, i32 (%"class.std::__1::basic_filebuf"*, i32)** %66, i64 13
  %68 = load i32 (%"class.std::__1::basic_filebuf"*, i32)*, i32 (%"class.std::__1::basic_filebuf"*, i32)** %67, align 8
  %69 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  %70 = call i32 %68(%"class.std::__1::basic_filebuf"* %37, i32 %69)
  %71 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  %72 = icmp eq i32 %70, %71
  br i1 %72, label %73, label %74

; <label>:73:                                     ; preds = %64
  store i32 -1, i32* %27, align 4
  br label %269

; <label>:74:                                     ; preds = %64
  br label %75

; <label>:75:                                     ; preds = %74, %54
  br label %76

; <label>:76:                                     ; preds = %113, %75
  %77 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 9
  %78 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %77, align 8
  %79 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 10
  %80 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 1
  %81 = load i8*, i8** %80, align 8
  %82 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 1
  %83 = load i8*, i8** %82, align 8
  %84 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 5
  %85 = load i64, i64* %84, align 8
  %86 = getelementptr inbounds i8, i8* %83, i64 %85
  store %"class.std::__1::codecvt"* %78, %"class.std::__1::codecvt"** %4, align 8
  store %union.__mbstate_t* %79, %union.__mbstate_t** %5, align 8
  store i8* %81, i8** %6, align 8
  store i8* %86, i8** %7, align 8
  store i8** %30, i8*** %8, align 8
  %87 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %4, align 8
  %88 = bitcast %"class.std::__1::codecvt"* %87 to i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**)***
  %89 = load i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**)**, i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**)*** %88, align 8
  %90 = getelementptr inbounds i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**)*, i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**)** %89, i64 5
  %91 = load i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**)*, i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**)** %90, align 8
  %92 = load %union.__mbstate_t*, %union.__mbstate_t** %5, align 8
  %93 = load i8*, i8** %6, align 8
  %94 = load i8*, i8** %7, align 8
  %95 = load i8**, i8*** %8, align 8
  %96 = call i32 %91(%"class.std::__1::codecvt"* %87, %union.__mbstate_t* dereferenceable(128) %92, i8* %93, i8* %94, i8** dereferenceable(8) %95)
  store i32 %96, i32* %29, align 4
  %97 = load i8*, i8** %30, align 8
  %98 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 1
  %99 = load i8*, i8** %98, align 8
  %100 = ptrtoint i8* %97 to i64
  %101 = ptrtoint i8* %99 to i64
  %102 = sub i64 %100, %101
  store i64 %102, i64* %31, align 8
  %103 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 1
  %104 = load i8*, i8** %103, align 8
  %105 = load i64, i64* %31, align 8
  %106 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 8
  %107 = load %struct.__sFILE*, %struct.__sFILE** %106, align 8
  %108 = call i64 @"\01_fwrite"(i8* %104, i64 1, i64 %105, %struct.__sFILE* %107)
  %109 = load i64, i64* %31, align 8
  %110 = icmp ne i64 %108, %109
  br i1 %110, label %111, label %112

; <label>:111:                                    ; preds = %76
  store i32 -1, i32* %27, align 4
  br label %269

; <label>:112:                                    ; preds = %76
  br label %113

; <label>:113:                                    ; preds = %112
  %114 = load i32, i32* %29, align 4
  %115 = icmp eq i32 %114, 1
  br i1 %115, label %76, label %116

; <label>:116:                                    ; preds = %113
  %117 = load i32, i32* %29, align 4
  %118 = icmp eq i32 %117, 2
  br i1 %118, label %119, label %120

; <label>:119:                                    ; preds = %116
  store i32 -1, i32* %27, align 4
  br label %269

; <label>:120:                                    ; preds = %116
  %121 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 8
  %122 = load %struct.__sFILE*, %struct.__sFILE** %121, align 8
  %123 = call i32 @fflush(%struct.__sFILE* %122)
  %124 = icmp ne i32 %123, 0
  br i1 %124, label %125, label %126

; <label>:125:                                    ; preds = %120
  store i32 -1, i32* %27, align 4
  br label %269

; <label>:126:                                    ; preds = %120
  br label %268

; <label>:127:                                    ; preds = %49
  %128 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 13
  %129 = load i32, i32* %128, align 4
  %130 = and i32 %129, 8
  %131 = icmp ne i32 %130, 0
  br i1 %131, label %132, label %267

; <label>:132:                                    ; preds = %127
  %133 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 11
  %134 = bitcast %union.__mbstate_t* %33 to i8*
  %135 = bitcast %union.__mbstate_t* %133 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %134, i8* %135, i64 128, i32 8, i1 false)
  store i8 0, i8* %34, align 1
  %136 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 16
  %137 = load i8, i8* %136, align 2
  %138 = trunc i8 %137 to i1
  br i1 %138, label %139, label %151

; <label>:139:                                    ; preds = %132
  %140 = bitcast %"class.std::__1::basic_filebuf"* %37 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %140, %"class.std::__1::basic_streambuf"** %3, align 8
  %141 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %3, align 8
  %142 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %141, i32 0, i32 4
  %143 = load i8*, i8** %142, align 8
  %144 = bitcast %"class.std::__1::basic_filebuf"* %37 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %144, %"class.std::__1::basic_streambuf"** %2, align 8
  %145 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %2, align 8
  %146 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %145, i32 0, i32 3
  %147 = load i8*, i8** %146, align 8
  %148 = ptrtoint i8* %143 to i64
  %149 = ptrtoint i8* %147 to i64
  %150 = sub i64 %148, %149
  store i64 %150, i64* %32, align 8
  br label %238

; <label>:151:                                    ; preds = %132
  %152 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 9
  %153 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %152, align 8
  store %"class.std::__1::codecvt"* %153, %"class.std::__1::codecvt"** %9, align 8
  %154 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %9, align 8
  %155 = bitcast %"class.std::__1::codecvt"* %154 to i32 (%"class.std::__1::codecvt"*)***
  %156 = load i32 (%"class.std::__1::codecvt"*)**, i32 (%"class.std::__1::codecvt"*)*** %155, align 8
  %157 = getelementptr inbounds i32 (%"class.std::__1::codecvt"*)*, i32 (%"class.std::__1::codecvt"*)** %156, i64 6
  %158 = load i32 (%"class.std::__1::codecvt"*)*, i32 (%"class.std::__1::codecvt"*)** %157, align 8
  %159 = call i32 %158(%"class.std::__1::codecvt"* %154) #15
  store i32 %159, i32* %35, align 4
  %160 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 3
  %161 = load i8*, i8** %160, align 8
  %162 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 2
  %163 = load i8*, i8** %162, align 8
  %164 = ptrtoint i8* %161 to i64
  %165 = ptrtoint i8* %163 to i64
  %166 = sub i64 %164, %165
  store i64 %166, i64* %32, align 8
  %167 = load i32, i32* %35, align 4
  %168 = icmp sgt i32 %167, 0
  br i1 %168, label %169, label %186

; <label>:169:                                    ; preds = %151
  %170 = load i32, i32* %35, align 4
  %171 = sext i32 %170 to i64
  %172 = bitcast %"class.std::__1::basic_filebuf"* %37 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %172, %"class.std::__1::basic_streambuf"** %10, align 8
  %173 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %10, align 8
  %174 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %173, i32 0, i32 4
  %175 = load i8*, i8** %174, align 8
  %176 = bitcast %"class.std::__1::basic_filebuf"* %37 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %176, %"class.std::__1::basic_streambuf"** %11, align 8
  %177 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %11, align 8
  %178 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %177, i32 0, i32 3
  %179 = load i8*, i8** %178, align 8
  %180 = ptrtoint i8* %175 to i64
  %181 = ptrtoint i8* %179 to i64
  %182 = sub i64 %180, %181
  %183 = mul nsw i64 %171, %182
  %184 = load i64, i64* %32, align 8
  %185 = add nsw i64 %184, %183
  store i64 %185, i64* %32, align 8
  br label %237

; <label>:186:                                    ; preds = %151
  %187 = bitcast %"class.std::__1::basic_filebuf"* %37 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %187, %"class.std::__1::basic_streambuf"** %12, align 8
  %188 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %12, align 8
  %189 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %188, i32 0, i32 3
  %190 = load i8*, i8** %189, align 8
  %191 = bitcast %"class.std::__1::basic_filebuf"* %37 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %191, %"class.std::__1::basic_streambuf"** %13, align 8
  %192 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %13, align 8
  %193 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %192, i32 0, i32 4
  %194 = load i8*, i8** %193, align 8
  %195 = icmp ne i8* %190, %194
  br i1 %195, label %196, label %236

; <label>:196:                                    ; preds = %186
  %197 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 9
  %198 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %197, align 8
  %199 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 1
  %200 = load i8*, i8** %199, align 8
  %201 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 2
  %202 = load i8*, i8** %201, align 8
  %203 = bitcast %"class.std::__1::basic_filebuf"* %37 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %203, %"class.std::__1::basic_streambuf"** %14, align 8
  %204 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %14, align 8
  %205 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %204, i32 0, i32 3
  %206 = load i8*, i8** %205, align 8
  %207 = bitcast %"class.std::__1::basic_filebuf"* %37 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %207, %"class.std::__1::basic_streambuf"** %15, align 8
  %208 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %15, align 8
  %209 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %208, i32 0, i32 2
  %210 = load i8*, i8** %209, align 8
  %211 = ptrtoint i8* %206 to i64
  %212 = ptrtoint i8* %210 to i64
  %213 = sub i64 %211, %212
  store %"class.std::__1::codecvt"* %198, %"class.std::__1::codecvt"** %17, align 8
  store %union.__mbstate_t* %33, %union.__mbstate_t** %18, align 8
  store i8* %200, i8** %19, align 8
  store i8* %202, i8** %20, align 8
  store i64 %213, i64* %21, align 8
  %214 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %17, align 8
  %215 = bitcast %"class.std::__1::codecvt"* %214 to i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i64)***
  %216 = load i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i64)**, i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i64)*** %215, align 8
  %217 = getelementptr inbounds i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i64)*, i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i64)** %216, i64 8
  %218 = load i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i64)*, i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i64)** %217, align 8
  %219 = load %union.__mbstate_t*, %union.__mbstate_t** %18, align 8
  %220 = load i8*, i8** %19, align 8
  %221 = load i8*, i8** %20, align 8
  %222 = load i64, i64* %21, align 8
  %223 = call i32 %218(%"class.std::__1::codecvt"* %214, %union.__mbstate_t* dereferenceable(128) %219, i8* %220, i8* %221, i64 %222)
  store i32 %223, i32* %36, align 4
  %224 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 2
  %225 = load i8*, i8** %224, align 8
  %226 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 1
  %227 = load i8*, i8** %226, align 8
  %228 = ptrtoint i8* %225 to i64
  %229 = ptrtoint i8* %227 to i64
  %230 = sub i64 %228, %229
  %231 = load i32, i32* %36, align 4
  %232 = sext i32 %231 to i64
  %233 = sub nsw i64 %230, %232
  %234 = load i64, i64* %32, align 8
  %235 = add nsw i64 %234, %233
  store i64 %235, i64* %32, align 8
  store i8 1, i8* %34, align 1
  br label %236

; <label>:236:                                    ; preds = %196, %186
  br label %237

; <label>:237:                                    ; preds = %236, %169
  br label %238

; <label>:238:                                    ; preds = %237, %139
  %239 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 8
  %240 = load %struct.__sFILE*, %struct.__sFILE** %239, align 8
  %241 = load i64, i64* %32, align 8
  %242 = sub nsw i64 0, %241
  %243 = call i32 @fseeko(%struct.__sFILE* %240, i64 %242, i32 1)
  %244 = icmp ne i32 %243, 0
  br i1 %244, label %245, label %246

; <label>:245:                                    ; preds = %238
  store i32 -1, i32* %27, align 4
  br label %269

; <label>:246:                                    ; preds = %238
  %247 = load i8, i8* %34, align 1
  %248 = trunc i8 %247 to i1
  br i1 %248, label %249, label %253

; <label>:249:                                    ; preds = %246
  %250 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 10
  %251 = bitcast %union.__mbstate_t* %250 to i8*
  %252 = bitcast %union.__mbstate_t* %33 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %251, i8* %252, i64 128, i32 8, i1 false)
  br label %253

; <label>:253:                                    ; preds = %249, %246
  %254 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 1
  %255 = load i8*, i8** %254, align 8
  %256 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 3
  store i8* %255, i8** %256, align 8
  %257 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 2
  store i8* %255, i8** %257, align 8
  %258 = bitcast %"class.std::__1::basic_filebuf"* %37 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %258, %"class.std::__1::basic_streambuf"** %22, align 8
  store i8* null, i8** %23, align 8
  store i8* null, i8** %24, align 8
  store i8* null, i8** %25, align 8
  %259 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %22, align 8
  %260 = load i8*, i8** %23, align 8
  %261 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %259, i32 0, i32 2
  store i8* %260, i8** %261, align 8
  %262 = load i8*, i8** %24, align 8
  %263 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %259, i32 0, i32 3
  store i8* %262, i8** %263, align 8
  %264 = load i8*, i8** %25, align 8
  %265 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %259, i32 0, i32 4
  store i8* %264, i8** %265, align 8
  %266 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %37, i32 0, i32 13
  store i32 0, i32* %266, align 4
  br label %267

; <label>:267:                                    ; preds = %253, %127
  br label %268

; <label>:268:                                    ; preds = %267, %126
  store i32 0, i32* %27, align 4
  br label %269

; <label>:269:                                    ; preds = %268, %245, %125, %119, %111, %73, %41
  %270 = load i32, i32* %27, align 4
  ret i32 %270
}

declare i64 @_ZNSt3__115basic_streambufIcNS_11char_traitsIcEEE9showmanycEv(%"class.std::__1::basic_streambuf"*) unnamed_addr #1

declare i64 @_ZNSt3__115basic_streambufIcNS_11char_traitsIcEEE6xsgetnEPcl(%"class.std::__1::basic_streambuf"*, i8*, i64) unnamed_addr #1

; Function Attrs: ssp uwtable
define linkonce_odr i32 @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE9underflowEv(%"class.std::__1::basic_filebuf"*) unnamed_addr #0 align 2 {
  %2 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %3 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %4 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %5 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %6 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %7 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %8 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %9 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %10 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %11 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %12 = alloca i8*, align 8
  %13 = alloca i8*, align 8
  %14 = alloca i8*, align 8
  %15 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %16 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %17 = alloca %"struct.std::__1::__less.58"*, align 8
  %18 = alloca i64*, align 8
  %19 = alloca i64*, align 8
  %20 = alloca %"struct.std::__1::__less.58", align 1
  %21 = alloca i64*, align 8
  %22 = alloca i64*, align 8
  %23 = alloca i64*, align 8
  %24 = alloca i64*, align 8
  %25 = alloca %"struct.std::__1::__less.58", align 1
  %26 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %27 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %28 = alloca %"struct.std::__1::__less.58"*, align 8
  %29 = alloca i64*, align 8
  %30 = alloca i64*, align 8
  %31 = alloca %"struct.std::__1::__less.58", align 1
  %32 = alloca i64*, align 8
  %33 = alloca i64*, align 8
  %34 = alloca i64*, align 8
  %35 = alloca i64*, align 8
  %36 = alloca %"struct.std::__1::__less.58", align 1
  %37 = alloca %"class.std::__1::codecvt"*, align 8
  %38 = alloca %union.__mbstate_t*, align 8
  %39 = alloca i8*, align 8
  %40 = alloca i8*, align 8
  %41 = alloca i8**, align 8
  %42 = alloca i8*, align 8
  %43 = alloca i8*, align 8
  %44 = alloca i8**, align 8
  %45 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %46 = alloca i8*, align 8
  %47 = alloca i8*, align 8
  %48 = alloca i8*, align 8
  %49 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %50 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %51 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %52 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %53 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %54 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %55 = alloca i8*, align 8
  %56 = alloca i8*, align 8
  %57 = alloca i8*, align 8
  %58 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %59 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %60 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %61 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %62 = alloca i8*, align 8
  %63 = alloca i8*, align 8
  %64 = alloca i8*, align 8
  %65 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %66 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %67 = alloca i8*, align 8
  %68 = alloca i8*, align 8
  %69 = alloca i8*, align 8
  %70 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %71 = alloca i32, align 4
  %72 = alloca %"class.std::__1::basic_filebuf"*, align 8
  %73 = alloca i8, align 1
  %74 = alloca i8, align 1
  %75 = alloca i64, align 8
  %76 = alloca i64, align 8
  %77 = alloca i64, align 8
  %78 = alloca i32, align 4
  %79 = alloca i64, align 8
  %80 = alloca i64, align 8
  %81 = alloca i64, align 8
  %82 = alloca i64, align 8
  %83 = alloca i32, align 4
  %84 = alloca i64, align 8
  %85 = alloca i8*, align 8
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %72, align 8
  %86 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %72, align 8
  %87 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 8
  %88 = load %struct.__sFILE*, %struct.__sFILE** %87, align 8
  %89 = icmp eq %struct.__sFILE* %88, null
  br i1 %89, label %90, label %92

; <label>:90:                                     ; preds = %1
  %91 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  store i32 %91, i32* %71, align 4
  br label %445

; <label>:92:                                     ; preds = %1
  %93 = call zeroext i1 @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE11__read_modeEv(%"class.std::__1::basic_filebuf"* %86)
  %94 = zext i1 %93 to i8
  store i8 %94, i8* %73, align 1
  %95 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %95, %"class.std::__1::basic_streambuf"** %70, align 8
  %96 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %70, align 8
  %97 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %96, i32 0, i32 3
  %98 = load i8*, i8** %97, align 8
  %99 = icmp eq i8* %98, null
  br i1 %99, label %100, label %111

; <label>:100:                                    ; preds = %92
  %101 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  %102 = getelementptr inbounds i8, i8* %74, i64 1
  %103 = getelementptr inbounds i8, i8* %74, i64 1
  store %"class.std::__1::basic_streambuf"* %101, %"class.std::__1::basic_streambuf"** %61, align 8
  store i8* %74, i8** %62, align 8
  store i8* %102, i8** %63, align 8
  store i8* %103, i8** %64, align 8
  %104 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %61, align 8
  %105 = load i8*, i8** %62, align 8
  %106 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %104, i32 0, i32 2
  store i8* %105, i8** %106, align 8
  %107 = load i8*, i8** %63, align 8
  %108 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %104, i32 0, i32 3
  store i8* %107, i8** %108, align 8
  %109 = load i8*, i8** %64, align 8
  %110 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %104, i32 0, i32 4
  store i8* %109, i8** %110, align 8
  br label %111

; <label>:111:                                    ; preds = %100, %92
  %112 = load i8, i8* %73, align 1
  %113 = trunc i8 %112 to i1
  br i1 %113, label %114, label %115

; <label>:114:                                    ; preds = %111
  br label %145

; <label>:115:                                    ; preds = %111
  %116 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %116, %"class.std::__1::basic_streambuf"** %59, align 8
  %117 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %59, align 8
  %118 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %117, i32 0, i32 4
  %119 = load i8*, i8** %118, align 8
  %120 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %120, %"class.std::__1::basic_streambuf"** %50, align 8
  %121 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %50, align 8
  %122 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %121, i32 0, i32 2
  %123 = load i8*, i8** %122, align 8
  %124 = ptrtoint i8* %119 to i64
  %125 = ptrtoint i8* %123 to i64
  %126 = sub i64 %124, %125
  %127 = sdiv i64 %126, 2
  store i64 %127, i64* %76, align 8
  store i64 4, i64* %77, align 8
  store i64* %76, i64** %34, align 8
  store i64* %77, i64** %35, align 8
  %128 = load i64*, i64** %34, align 8
  %129 = load i64*, i64** %35, align 8
  store i64* %128, i64** %32, align 8
  store i64* %129, i64** %33, align 8
  %130 = load i64*, i64** %33, align 8
  %131 = load i64*, i64** %32, align 8
  store %"struct.std::__1::__less.58"* %31, %"struct.std::__1::__less.58"** %28, align 8
  store i64* %130, i64** %29, align 8
  store i64* %131, i64** %30, align 8
  %132 = load %"struct.std::__1::__less.58"*, %"struct.std::__1::__less.58"** %28, align 8
  %133 = load i64*, i64** %29, align 8
  %134 = load i64, i64* %133, align 8
  %135 = load i64*, i64** %30, align 8
  %136 = load i64, i64* %135, align 8
  %137 = icmp ult i64 %134, %136
  br i1 %137, label %138, label %140

; <label>:138:                                    ; preds = %115
  %139 = load i64*, i64** %33, align 8
  br label %142

; <label>:140:                                    ; preds = %115
  %141 = load i64*, i64** %32, align 8
  br label %142

; <label>:142:                                    ; preds = %138, %140
  %143 = phi i64* [ %139, %138 ], [ %141, %140 ]
  %144 = load i64, i64* %143, align 8
  br label %145

; <label>:145:                                    ; preds = %142, %114
  %146 = phi i64 [ 0, %114 ], [ %144, %142 ]
  store i64 %146, i64* %75, align 8
  %147 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  store i32 %147, i32* %78, align 4
  %148 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %148, %"class.std::__1::basic_streambuf"** %16, align 8
  %149 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %16, align 8
  %150 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %149, i32 0, i32 3
  %151 = load i8*, i8** %150, align 8
  %152 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %152, %"class.std::__1::basic_streambuf"** %2, align 8
  %153 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %2, align 8
  %154 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %153, i32 0, i32 4
  %155 = load i8*, i8** %154, align 8
  %156 = icmp eq i8* %151, %155
  br i1 %156, label %157, label %421

; <label>:157:                                    ; preds = %145
  %158 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %158, %"class.std::__1::basic_streambuf"** %3, align 8
  %159 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %3, align 8
  %160 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %159, i32 0, i32 2
  %161 = load i8*, i8** %160, align 8
  %162 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %162, %"class.std::__1::basic_streambuf"** %4, align 8
  %163 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %4, align 8
  %164 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %163, i32 0, i32 4
  %165 = load i8*, i8** %164, align 8
  %166 = load i64, i64* %75, align 8
  %167 = sub i64 0, %166
  %168 = getelementptr inbounds i8, i8* %165, i64 %167
  %169 = load i64, i64* %75, align 8
  %170 = mul i64 %169, 1
  call void @llvm.memmove.p0i8.p0i8.i64(i8* %161, i8* %168, i64 %170, i32 1, i1 false)
  %171 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 16
  %172 = load i8, i8* %171, align 2
  %173 = trunc i8 %172 to i1
  br i1 %173, label %174, label %234

; <label>:174:                                    ; preds = %157
  %175 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %175, %"class.std::__1::basic_streambuf"** %5, align 8
  %176 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %5, align 8
  %177 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %176, i32 0, i32 4
  %178 = load i8*, i8** %177, align 8
  %179 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %179, %"class.std::__1::basic_streambuf"** %6, align 8
  %180 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %6, align 8
  %181 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %180, i32 0, i32 2
  %182 = load i8*, i8** %181, align 8
  %183 = ptrtoint i8* %178 to i64
  %184 = ptrtoint i8* %182 to i64
  %185 = sub i64 %183, %184
  %186 = load i64, i64* %75, align 8
  %187 = sub i64 %185, %186
  store i64 %187, i64* %79, align 8
  %188 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %188, %"class.std::__1::basic_streambuf"** %7, align 8
  %189 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %7, align 8
  %190 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %189, i32 0, i32 2
  %191 = load i8*, i8** %190, align 8
  %192 = load i64, i64* %75, align 8
  %193 = getelementptr inbounds i8, i8* %191, i64 %192
  %194 = load i64, i64* %79, align 8
  %195 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 8
  %196 = load %struct.__sFILE*, %struct.__sFILE** %195, align 8
  %197 = call i64 @fread(i8* %193, i64 1, i64 %194, %struct.__sFILE* %196)
  store i64 %197, i64* %79, align 8
  %198 = load i64, i64* %79, align 8
  %199 = icmp ne i64 %198, 0
  br i1 %199, label %200, label %233

; <label>:200:                                    ; preds = %174
  %201 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  %202 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %202, %"class.std::__1::basic_streambuf"** %8, align 8
  %203 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %8, align 8
  %204 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %203, i32 0, i32 2
  %205 = load i8*, i8** %204, align 8
  %206 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %206, %"class.std::__1::basic_streambuf"** %9, align 8
  %207 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %9, align 8
  %208 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %207, i32 0, i32 2
  %209 = load i8*, i8** %208, align 8
  %210 = load i64, i64* %75, align 8
  %211 = getelementptr inbounds i8, i8* %209, i64 %210
  %212 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %212, %"class.std::__1::basic_streambuf"** %10, align 8
  %213 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %10, align 8
  %214 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %213, i32 0, i32 2
  %215 = load i8*, i8** %214, align 8
  %216 = load i64, i64* %75, align 8
  %217 = getelementptr inbounds i8, i8* %215, i64 %216
  %218 = load i64, i64* %79, align 8
  %219 = getelementptr inbounds i8, i8* %217, i64 %218
  store %"class.std::__1::basic_streambuf"* %201, %"class.std::__1::basic_streambuf"** %11, align 8
  store i8* %205, i8** %12, align 8
  store i8* %211, i8** %13, align 8
  store i8* %219, i8** %14, align 8
  %220 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %11, align 8
  %221 = load i8*, i8** %12, align 8
  %222 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %220, i32 0, i32 2
  store i8* %221, i8** %222, align 8
  %223 = load i8*, i8** %13, align 8
  %224 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %220, i32 0, i32 3
  store i8* %223, i8** %224, align 8
  %225 = load i8*, i8** %14, align 8
  %226 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %220, i32 0, i32 4
  store i8* %225, i8** %226, align 8
  %227 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %227, %"class.std::__1::basic_streambuf"** %15, align 8
  %228 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %15, align 8
  %229 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %228, i32 0, i32 3
  %230 = load i8*, i8** %229, align 8
  %231 = load i8, i8* %230, align 1
  %232 = call i32 @_ZNSt3__111char_traitsIcE11to_int_typeEc(i8 signext %231) #15
  store i32 %232, i32* %78, align 4
  br label %233

; <label>:233:                                    ; preds = %200, %174
  br label %420

; <label>:234:                                    ; preds = %157
  %235 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 1
  %236 = load i8*, i8** %235, align 8
  %237 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 2
  %238 = load i8*, i8** %237, align 8
  %239 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 3
  %240 = load i8*, i8** %239, align 8
  %241 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 2
  %242 = load i8*, i8** %241, align 8
  %243 = ptrtoint i8* %240 to i64
  %244 = ptrtoint i8* %242 to i64
  %245 = sub i64 %243, %244
  call void @llvm.memmove.p0i8.p0i8.i64(i8* %236, i8* %238, i64 %245, i32 1, i1 false)
  %246 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 1
  %247 = load i8*, i8** %246, align 8
  %248 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 3
  %249 = load i8*, i8** %248, align 8
  %250 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 2
  %251 = load i8*, i8** %250, align 8
  %252 = ptrtoint i8* %249 to i64
  %253 = ptrtoint i8* %251 to i64
  %254 = sub i64 %252, %253
  %255 = getelementptr inbounds i8, i8* %247, i64 %254
  %256 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 2
  store i8* %255, i8** %256, align 8
  %257 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 1
  %258 = load i8*, i8** %257, align 8
  %259 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 1
  %260 = load i8*, i8** %259, align 8
  %261 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 4
  %262 = getelementptr inbounds [8 x i8], [8 x i8]* %261, i32 0, i32 0
  %263 = icmp eq i8* %260, %262
  br i1 %263, label %264, label %265

; <label>:264:                                    ; preds = %234
  br label %268

; <label>:265:                                    ; preds = %234
  %266 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 5
  %267 = load i64, i64* %266, align 8
  br label %268

; <label>:268:                                    ; preds = %265, %264
  %269 = phi i64 [ 8, %264 ], [ %267, %265 ]
  %270 = getelementptr inbounds i8, i8* %258, i64 %269
  %271 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 3
  store i8* %270, i8** %271, align 8
  %272 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 7
  %273 = load i64, i64* %272, align 8
  %274 = load i64, i64* %75, align 8
  %275 = sub i64 %273, %274
  store i64 %275, i64* %81, align 8
  %276 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 3
  %277 = load i8*, i8** %276, align 8
  %278 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 2
  %279 = load i8*, i8** %278, align 8
  %280 = ptrtoint i8* %277 to i64
  %281 = ptrtoint i8* %279 to i64
  %282 = sub i64 %280, %281
  store i64 %282, i64* %82, align 8
  store i64* %81, i64** %23, align 8
  store i64* %82, i64** %24, align 8
  %283 = load i64*, i64** %23, align 8
  %284 = load i64*, i64** %24, align 8
  store i64* %283, i64** %21, align 8
  store i64* %284, i64** %22, align 8
  %285 = load i64*, i64** %22, align 8
  %286 = load i64*, i64** %21, align 8
  store %"struct.std::__1::__less.58"* %20, %"struct.std::__1::__less.58"** %17, align 8
  store i64* %285, i64** %18, align 8
  store i64* %286, i64** %19, align 8
  %287 = load %"struct.std::__1::__less.58"*, %"struct.std::__1::__less.58"** %17, align 8
  %288 = load i64*, i64** %18, align 8
  %289 = load i64, i64* %288, align 8
  %290 = load i64*, i64** %19, align 8
  %291 = load i64, i64* %290, align 8
  %292 = icmp ult i64 %289, %291
  br i1 %292, label %293, label %295

; <label>:293:                                    ; preds = %268
  %294 = load i64*, i64** %22, align 8
  br label %297

; <label>:295:                                    ; preds = %268
  %296 = load i64*, i64** %21, align 8
  br label %297

; <label>:297:                                    ; preds = %293, %295
  %298 = phi i64* [ %294, %293 ], [ %296, %295 ]
  %299 = load i64, i64* %298, align 8
  store i64 %299, i64* %80, align 8
  %300 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 11
  %301 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 10
  %302 = bitcast %union.__mbstate_t* %300 to i8*
  %303 = bitcast %union.__mbstate_t* %301 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %302, i8* %303, i64 128, i32 8, i1 false)
  %304 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 2
  %305 = load i8*, i8** %304, align 8
  %306 = load i64, i64* %80, align 8
  %307 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 8
  %308 = load %struct.__sFILE*, %struct.__sFILE** %307, align 8
  %309 = call i64 @fread(i8* %305, i64 1, i64 %306, %struct.__sFILE* %308)
  store i64 %309, i64* %84, align 8
  %310 = load i64, i64* %84, align 8
  %311 = icmp ne i64 %310, 0
  br i1 %311, label %312, label %419

; <label>:312:                                    ; preds = %297
  %313 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 9
  %314 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %313, align 8
  %315 = icmp ne %"class.std::__1::codecvt"* %314, null
  br i1 %315, label %319, label %316

; <label>:316:                                    ; preds = %312
  %317 = call i8* @__cxa_allocate_exception(i64 8) #15
  %318 = bitcast i8* %317 to %"class.std::bad_cast"*
  call void @_ZNSt8bad_castC1Ev(%"class.std::bad_cast"* %318) #15
  call void @__cxa_throw(i8* %317, i8* bitcast (i8** @_ZTISt8bad_cast to i8*), i8* bitcast (void (%"class.std::bad_cast"*)* @_ZNSt8bad_castD1Ev to i8*)) #14
  unreachable

; <label>:319:                                    ; preds = %312
  %320 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 2
  %321 = load i8*, i8** %320, align 8
  %322 = load i64, i64* %84, align 8
  %323 = getelementptr inbounds i8, i8* %321, i64 %322
  %324 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 3
  store i8* %323, i8** %324, align 8
  %325 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 9
  %326 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %325, align 8
  %327 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 10
  %328 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 1
  %329 = load i8*, i8** %328, align 8
  %330 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 3
  %331 = load i8*, i8** %330, align 8
  %332 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 2
  %333 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %333, %"class.std::__1::basic_streambuf"** %26, align 8
  %334 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %26, align 8
  %335 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %334, i32 0, i32 2
  %336 = load i8*, i8** %335, align 8
  %337 = load i64, i64* %75, align 8
  %338 = getelementptr inbounds i8, i8* %336, i64 %337
  %339 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %339, %"class.std::__1::basic_streambuf"** %27, align 8
  %340 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %27, align 8
  %341 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %340, i32 0, i32 2
  %342 = load i8*, i8** %341, align 8
  %343 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 7
  %344 = load i64, i64* %343, align 8
  %345 = getelementptr inbounds i8, i8* %342, i64 %344
  store %"class.std::__1::codecvt"* %326, %"class.std::__1::codecvt"** %37, align 8
  store %union.__mbstate_t* %327, %union.__mbstate_t** %38, align 8
  store i8* %329, i8** %39, align 8
  store i8* %331, i8** %40, align 8
  store i8** %332, i8*** %41, align 8
  store i8* %338, i8** %42, align 8
  store i8* %345, i8** %43, align 8
  store i8** %85, i8*** %44, align 8
  %346 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %37, align 8
  %347 = bitcast %"class.std::__1::codecvt"* %346 to i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**, i8*, i8*, i8**)***
  %348 = load i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**, i8*, i8*, i8**)**, i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**, i8*, i8*, i8**)*** %347, align 8
  %349 = getelementptr inbounds i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**, i8*, i8*, i8**)*, i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**, i8*, i8*, i8**)** %348, i64 4
  %350 = load i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**, i8*, i8*, i8**)*, i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**, i8*, i8*, i8**)** %349, align 8
  %351 = load %union.__mbstate_t*, %union.__mbstate_t** %38, align 8
  %352 = load i8*, i8** %39, align 8
  %353 = load i8*, i8** %40, align 8
  %354 = load i8**, i8*** %41, align 8
  %355 = load i8*, i8** %42, align 8
  %356 = load i8*, i8** %43, align 8
  %357 = load i8**, i8*** %44, align 8
  %358 = call i32 %350(%"class.std::__1::codecvt"* %346, %union.__mbstate_t* dereferenceable(128) %351, i8* %352, i8* %353, i8** dereferenceable(8) %354, i8* %355, i8* %356, i8** dereferenceable(8) %357)
  store i32 %358, i32* %83, align 4
  %359 = load i32, i32* %83, align 4
  %360 = icmp eq i32 %359, 3
  br i1 %360, label %361, label %382

; <label>:361:                                    ; preds = %319
  %362 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  %363 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 1
  %364 = load i8*, i8** %363, align 8
  %365 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 1
  %366 = load i8*, i8** %365, align 8
  %367 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %86, i32 0, i32 3
  %368 = load i8*, i8** %367, align 8
  store %"class.std::__1::basic_streambuf"* %362, %"class.std::__1::basic_streambuf"** %45, align 8
  store i8* %364, i8** %46, align 8
  store i8* %366, i8** %47, align 8
  store i8* %368, i8** %48, align 8
  %369 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %45, align 8
  %370 = load i8*, i8** %46, align 8
  %371 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %369, i32 0, i32 2
  store i8* %370, i8** %371, align 8
  %372 = load i8*, i8** %47, align 8
  %373 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %369, i32 0, i32 3
  store i8* %372, i8** %373, align 8
  %374 = load i8*, i8** %48, align 8
  %375 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %369, i32 0, i32 4
  store i8* %374, i8** %375, align 8
  %376 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %376, %"class.std::__1::basic_streambuf"** %49, align 8
  %377 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %49, align 8
  %378 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %377, i32 0, i32 3
  %379 = load i8*, i8** %378, align 8
  %380 = load i8, i8* %379, align 1
  %381 = call i32 @_ZNSt3__111char_traitsIcE11to_int_typeEc(i8 signext %380) #15
  store i32 %381, i32* %78, align 4
  br label %418

; <label>:382:                                    ; preds = %319
  %383 = load i8*, i8** %85, align 8
  %384 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %384, %"class.std::__1::basic_streambuf"** %51, align 8
  %385 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %51, align 8
  %386 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %385, i32 0, i32 2
  %387 = load i8*, i8** %386, align 8
  %388 = load i64, i64* %75, align 8
  %389 = getelementptr inbounds i8, i8* %387, i64 %388
  %390 = icmp ne i8* %383, %389
  br i1 %390, label %391, label %417

; <label>:391:                                    ; preds = %382
  %392 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  %393 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %393, %"class.std::__1::basic_streambuf"** %52, align 8
  %394 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %52, align 8
  %395 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %394, i32 0, i32 2
  %396 = load i8*, i8** %395, align 8
  %397 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %397, %"class.std::__1::basic_streambuf"** %53, align 8
  %398 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %53, align 8
  %399 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %398, i32 0, i32 2
  %400 = load i8*, i8** %399, align 8
  %401 = load i64, i64* %75, align 8
  %402 = getelementptr inbounds i8, i8* %400, i64 %401
  %403 = load i8*, i8** %85, align 8
  store %"class.std::__1::basic_streambuf"* %392, %"class.std::__1::basic_streambuf"** %54, align 8
  store i8* %396, i8** %55, align 8
  store i8* %402, i8** %56, align 8
  store i8* %403, i8** %57, align 8
  %404 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %54, align 8
  %405 = load i8*, i8** %55, align 8
  %406 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %404, i32 0, i32 2
  store i8* %405, i8** %406, align 8
  %407 = load i8*, i8** %56, align 8
  %408 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %404, i32 0, i32 3
  store i8* %407, i8** %408, align 8
  %409 = load i8*, i8** %57, align 8
  %410 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %404, i32 0, i32 4
  store i8* %409, i8** %410, align 8
  %411 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %411, %"class.std::__1::basic_streambuf"** %58, align 8
  %412 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %58, align 8
  %413 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %412, i32 0, i32 3
  %414 = load i8*, i8** %413, align 8
  %415 = load i8, i8* %414, align 1
  %416 = call i32 @_ZNSt3__111char_traitsIcE11to_int_typeEc(i8 signext %415) #15
  store i32 %416, i32* %78, align 4
  br label %417

; <label>:417:                                    ; preds = %391, %382
  br label %418

; <label>:418:                                    ; preds = %417, %361
  br label %419

; <label>:419:                                    ; preds = %418, %297
  br label %420

; <label>:420:                                    ; preds = %419, %233
  br label %428

; <label>:421:                                    ; preds = %145
  %422 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %422, %"class.std::__1::basic_streambuf"** %60, align 8
  %423 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %60, align 8
  %424 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %423, i32 0, i32 3
  %425 = load i8*, i8** %424, align 8
  %426 = load i8, i8* %425, align 1
  %427 = call i32 @_ZNSt3__111char_traitsIcE11to_int_typeEc(i8 signext %426) #15
  store i32 %427, i32* %78, align 4
  br label %428

; <label>:428:                                    ; preds = %421, %420
  %429 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %429, %"class.std::__1::basic_streambuf"** %65, align 8
  %430 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %65, align 8
  %431 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %430, i32 0, i32 2
  %432 = load i8*, i8** %431, align 8
  %433 = icmp eq i8* %432, %74
  br i1 %433, label %434, label %443

; <label>:434:                                    ; preds = %428
  %435 = bitcast %"class.std::__1::basic_filebuf"* %86 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %435, %"class.std::__1::basic_streambuf"** %66, align 8
  store i8* null, i8** %67, align 8
  store i8* null, i8** %68, align 8
  store i8* null, i8** %69, align 8
  %436 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %66, align 8
  %437 = load i8*, i8** %67, align 8
  %438 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %436, i32 0, i32 2
  store i8* %437, i8** %438, align 8
  %439 = load i8*, i8** %68, align 8
  %440 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %436, i32 0, i32 3
  store i8* %439, i8** %440, align 8
  %441 = load i8*, i8** %69, align 8
  %442 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %436, i32 0, i32 4
  store i8* %441, i8** %442, align 8
  br label %443

; <label>:443:                                    ; preds = %434, %428
  %444 = load i32, i32* %78, align 4
  store i32 %444, i32* %71, align 4
  br label %445

; <label>:445:                                    ; preds = %443, %90
  %446 = load i32, i32* %71, align 4
  ret i32 %446
}

declare i32 @_ZNSt3__115basic_streambufIcNS_11char_traitsIcEEE5uflowEv(%"class.std::__1::basic_streambuf"*) unnamed_addr #1

; Function Attrs: ssp uwtable
define linkonce_odr i32 @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE9pbackfailEi(%"class.std::__1::basic_filebuf"*, i32) unnamed_addr #0 align 2 {
  %3 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %4 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %5 = alloca i32, align 4
  %6 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %7 = alloca i32, align 4
  %8 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %9 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %10 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %11 = alloca i32, align 4
  %12 = alloca %"class.std::__1::basic_filebuf"*, align 8
  %13 = alloca i32, align 4
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %12, align 8
  store i32 %1, i32* %13, align 4
  %14 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %12, align 8
  %15 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %14, i32 0, i32 8
  %16 = load %struct.__sFILE*, %struct.__sFILE** %15, align 8
  %17 = icmp ne %struct.__sFILE* %16, null
  br i1 %17, label %18, label %73

; <label>:18:                                     ; preds = %2
  %19 = bitcast %"class.std::__1::basic_filebuf"* %14 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %19, %"class.std::__1::basic_streambuf"** %10, align 8
  %20 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %10, align 8
  %21 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %20, i32 0, i32 2
  %22 = load i8*, i8** %21, align 8
  %23 = bitcast %"class.std::__1::basic_filebuf"* %14 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %23, %"class.std::__1::basic_streambuf"** %9, align 8
  %24 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %9, align 8
  %25 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %24, i32 0, i32 3
  %26 = load i8*, i8** %25, align 8
  %27 = icmp ult i8* %22, %26
  br i1 %27, label %28, label %73

; <label>:28:                                     ; preds = %18
  %29 = load i32, i32* %13, align 4
  %30 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  %31 = call zeroext i1 @_ZNSt3__111char_traitsIcE11eq_int_typeEii(i32 %29, i32 %30) #15
  br i1 %31, label %32, label %42

; <label>:32:                                     ; preds = %28
  %33 = bitcast %"class.std::__1::basic_filebuf"* %14 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %33, %"class.std::__1::basic_streambuf"** %6, align 8
  store i32 -1, i32* %7, align 4
  %34 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %6, align 8
  %35 = load i32, i32* %7, align 4
  %36 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %34, i32 0, i32 3
  %37 = load i8*, i8** %36, align 8
  %38 = sext i32 %35 to i64
  %39 = getelementptr inbounds i8, i8* %37, i64 %38
  store i8* %39, i8** %36, align 8
  %40 = load i32, i32* %13, align 4
  %41 = call i32 @_ZNSt3__111char_traitsIcE7not_eofEi(i32 %40) #15
  store i32 %41, i32* %11, align 4
  br label %75

; <label>:42:                                     ; preds = %28
  %43 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %14, i32 0, i32 12
  %44 = load i32, i32* %43, align 8
  %45 = and i32 %44, 16
  %46 = icmp ne i32 %45, 0
  br i1 %46, label %57, label %47

; <label>:47:                                     ; preds = %42
  %48 = load i32, i32* %13, align 4
  %49 = call signext i8 @_ZNSt3__111char_traitsIcE12to_char_typeEi(i32 %48) #15
  %50 = bitcast %"class.std::__1::basic_filebuf"* %14 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %50, %"class.std::__1::basic_streambuf"** %3, align 8
  %51 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %3, align 8
  %52 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %51, i32 0, i32 3
  %53 = load i8*, i8** %52, align 8
  %54 = getelementptr inbounds i8, i8* %53, i64 -1
  %55 = load i8, i8* %54, align 1
  %56 = call zeroext i1 @_ZNSt3__111char_traitsIcE2eqEcc(i8 signext %49, i8 signext %55) #15
  br i1 %56, label %57, label %72

; <label>:57:                                     ; preds = %47, %42
  %58 = bitcast %"class.std::__1::basic_filebuf"* %14 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %58, %"class.std::__1::basic_streambuf"** %4, align 8
  store i32 -1, i32* %5, align 4
  %59 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %4, align 8
  %60 = load i32, i32* %5, align 4
  %61 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %59, i32 0, i32 3
  %62 = load i8*, i8** %61, align 8
  %63 = sext i32 %60 to i64
  %64 = getelementptr inbounds i8, i8* %62, i64 %63
  store i8* %64, i8** %61, align 8
  %65 = load i32, i32* %13, align 4
  %66 = call signext i8 @_ZNSt3__111char_traitsIcE12to_char_typeEi(i32 %65) #15
  %67 = bitcast %"class.std::__1::basic_filebuf"* %14 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %67, %"class.std::__1::basic_streambuf"** %8, align 8
  %68 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %8, align 8
  %69 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %68, i32 0, i32 3
  %70 = load i8*, i8** %69, align 8
  store i8 %66, i8* %70, align 1
  %71 = load i32, i32* %13, align 4
  store i32 %71, i32* %11, align 4
  br label %75

; <label>:72:                                     ; preds = %47
  br label %73

; <label>:73:                                     ; preds = %72, %18, %2
  %74 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  store i32 %74, i32* %11, align 4
  br label %75

; <label>:75:                                     ; preds = %73, %57, %32
  %76 = load i32, i32* %11, align 4
  ret i32 %76
}

declare i64 @_ZNSt3__115basic_streambufIcNS_11char_traitsIcEEE6xsputnEPKcl(%"class.std::__1::basic_streambuf"*, i8*, i64) unnamed_addr #1

; Function Attrs: ssp uwtable
define linkonce_odr i32 @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE8overflowEi(%"class.std::__1::basic_filebuf"*, i32) unnamed_addr #0 align 2 {
  %3 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %4 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %5 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %6 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %7 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %8 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %9 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %10 = alloca %"class.std::__1::codecvt"*, align 8
  %11 = alloca %union.__mbstate_t*, align 8
  %12 = alloca i8*, align 8
  %13 = alloca i8*, align 8
  %14 = alloca i8**, align 8
  %15 = alloca i8*, align 8
  %16 = alloca i8*, align 8
  %17 = alloca i8**, align 8
  %18 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %19 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %20 = alloca i32, align 4
  %21 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %22 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %23 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %24 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %25 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %26 = alloca i8*, align 8
  %27 = alloca i8*, align 8
  %28 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %29 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %30 = alloca i8*, align 8
  %31 = alloca i8*, align 8
  %32 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %33 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %34 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %35 = alloca i32, align 4
  %36 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %37 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %38 = alloca i8*, align 8
  %39 = alloca i8*, align 8
  %40 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %41 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %42 = alloca i32, align 4
  %43 = alloca %"class.std::__1::basic_filebuf"*, align 8
  %44 = alloca i32, align 4
  %45 = alloca i8, align 1
  %46 = alloca i8*, align 8
  %47 = alloca i8*, align 8
  %48 = alloca i64, align 8
  %49 = alloca i8*, align 8
  %50 = alloca i32, align 4
  %51 = alloca i8*, align 8
  %52 = alloca i64, align 8
  %53 = alloca i64, align 8
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %43, align 8
  store i32 %1, i32* %44, align 4
  %54 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %43, align 8
  %55 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %54, i32 0, i32 8
  %56 = load %struct.__sFILE*, %struct.__sFILE** %55, align 8
  %57 = icmp eq %struct.__sFILE* %56, null
  br i1 %57, label %58, label %60

; <label>:58:                                     ; preds = %2
  %59 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  store i32 %59, i32* %42, align 4
  br label %298

; <label>:60:                                     ; preds = %2
  call void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE12__write_modeEv(%"class.std::__1::basic_filebuf"* %54)
  %61 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %61, %"class.std::__1::basic_streambuf"** %41, align 8
  %62 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %41, align 8
  %63 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %62, i32 0, i32 5
  %64 = load i8*, i8** %63, align 8
  store i8* %64, i8** %46, align 8
  %65 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %65, %"class.std::__1::basic_streambuf"** %40, align 8
  %66 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %40, align 8
  %67 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %66, i32 0, i32 7
  %68 = load i8*, i8** %67, align 8
  store i8* %68, i8** %47, align 8
  %69 = load i32, i32* %44, align 4
  %70 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  %71 = call zeroext i1 @_ZNSt3__111char_traitsIcE11eq_int_typeEii(i32 %69, i32 %70) #15
  br i1 %71, label %101, label %72

; <label>:72:                                     ; preds = %60
  %73 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %73, %"class.std::__1::basic_streambuf"** %36, align 8
  %74 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %36, align 8
  %75 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %74, i32 0, i32 6
  %76 = load i8*, i8** %75, align 8
  %77 = icmp eq i8* %76, null
  br i1 %77, label %78, label %87

; <label>:78:                                     ; preds = %72
  %79 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  %80 = getelementptr inbounds i8, i8* %45, i64 1
  store %"class.std::__1::basic_streambuf"* %79, %"class.std::__1::basic_streambuf"** %25, align 8
  store i8* %45, i8** %26, align 8
  store i8* %80, i8** %27, align 8
  %81 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %25, align 8
  %82 = load i8*, i8** %26, align 8
  %83 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %81, i32 0, i32 6
  store i8* %82, i8** %83, align 8
  %84 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %81, i32 0, i32 5
  store i8* %82, i8** %84, align 8
  %85 = load i8*, i8** %27, align 8
  %86 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %81, i32 0, i32 7
  store i8* %85, i8** %86, align 8
  br label %87

; <label>:87:                                     ; preds = %78, %72
  %88 = load i32, i32* %44, align 4
  %89 = call signext i8 @_ZNSt3__111char_traitsIcE12to_char_typeEi(i32 %88) #15
  %90 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %90, %"class.std::__1::basic_streambuf"** %24, align 8
  %91 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %24, align 8
  %92 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %91, i32 0, i32 6
  %93 = load i8*, i8** %92, align 8
  store i8 %89, i8* %93, align 1
  %94 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %94, %"class.std::__1::basic_streambuf"** %19, align 8
  store i32 1, i32* %20, align 4
  %95 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %19, align 8
  %96 = load i32, i32* %20, align 4
  %97 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %95, i32 0, i32 6
  %98 = load i8*, i8** %97, align 8
  %99 = sext i32 %96 to i64
  %100 = getelementptr inbounds i8, i8* %98, i64 %99
  store i8* %100, i8** %97, align 8
  br label %101

; <label>:101:                                    ; preds = %87, %60
  %102 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %102, %"class.std::__1::basic_streambuf"** %9, align 8
  %103 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %9, align 8
  %104 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %103, i32 0, i32 6
  %105 = load i8*, i8** %104, align 8
  %106 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %106, %"class.std::__1::basic_streambuf"** %6, align 8
  %107 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %6, align 8
  %108 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %107, i32 0, i32 5
  %109 = load i8*, i8** %108, align 8
  %110 = icmp ne i8* %105, %109
  br i1 %110, label %111, label %295

; <label>:111:                                    ; preds = %101
  %112 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %54, i32 0, i32 16
  %113 = load i8, i8* %112, align 2
  %114 = trunc i8 %113 to i1
  br i1 %114, label %115, label %140

; <label>:115:                                    ; preds = %111
  %116 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %116, %"class.std::__1::basic_streambuf"** %3, align 8
  %117 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %3, align 8
  %118 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %117, i32 0, i32 6
  %119 = load i8*, i8** %118, align 8
  %120 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %120, %"class.std::__1::basic_streambuf"** %4, align 8
  %121 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %4, align 8
  %122 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %121, i32 0, i32 5
  %123 = load i8*, i8** %122, align 8
  %124 = ptrtoint i8* %119 to i64
  %125 = ptrtoint i8* %123 to i64
  %126 = sub i64 %124, %125
  store i64 %126, i64* %48, align 8
  %127 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %127, %"class.std::__1::basic_streambuf"** %5, align 8
  %128 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %5, align 8
  %129 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %128, i32 0, i32 5
  %130 = load i8*, i8** %129, align 8
  %131 = load i64, i64* %48, align 8
  %132 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %54, i32 0, i32 8
  %133 = load %struct.__sFILE*, %struct.__sFILE** %132, align 8
  %134 = call i64 @"\01_fwrite"(i8* %130, i64 1, i64 %131, %struct.__sFILE* %133)
  %135 = load i64, i64* %48, align 8
  %136 = icmp ne i64 %134, %135
  br i1 %136, label %137, label %139

; <label>:137:                                    ; preds = %115
  %138 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  store i32 %138, i32* %42, align 4
  br label %298

; <label>:139:                                    ; preds = %115
  br label %285

; <label>:140:                                    ; preds = %111
  %141 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %54, i32 0, i32 1
  %142 = load i8*, i8** %141, align 8
  store i8* %142, i8** %49, align 8
  br label %143

; <label>:143:                                    ; preds = %281, %140
  %144 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %54, i32 0, i32 9
  %145 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %144, align 8
  %146 = icmp ne %"class.std::__1::codecvt"* %145, null
  br i1 %146, label %150, label %147

; <label>:147:                                    ; preds = %143
  %148 = call i8* @__cxa_allocate_exception(i64 8) #15
  %149 = bitcast i8* %148 to %"class.std::bad_cast"*
  call void @_ZNSt8bad_castC1Ev(%"class.std::bad_cast"* %149) #15
  call void @__cxa_throw(i8* %148, i8* bitcast (i8** @_ZTISt8bad_cast to i8*), i8* bitcast (void (%"class.std::bad_cast"*)* @_ZNSt8bad_castD1Ev to i8*)) #14
  unreachable

; <label>:150:                                    ; preds = %143
  %151 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %54, i32 0, i32 9
  %152 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %151, align 8
  %153 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %54, i32 0, i32 10
  %154 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %154, %"class.std::__1::basic_streambuf"** %7, align 8
  %155 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %7, align 8
  %156 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %155, i32 0, i32 5
  %157 = load i8*, i8** %156, align 8
  %158 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %158, %"class.std::__1::basic_streambuf"** %8, align 8
  %159 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %8, align 8
  %160 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %159, i32 0, i32 6
  %161 = load i8*, i8** %160, align 8
  %162 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %54, i32 0, i32 1
  %163 = load i8*, i8** %162, align 8
  %164 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %54, i32 0, i32 1
  %165 = load i8*, i8** %164, align 8
  %166 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %54, i32 0, i32 5
  %167 = load i64, i64* %166, align 8
  %168 = getelementptr inbounds i8, i8* %165, i64 %167
  store %"class.std::__1::codecvt"* %152, %"class.std::__1::codecvt"** %10, align 8
  store %union.__mbstate_t* %153, %union.__mbstate_t** %11, align 8
  store i8* %157, i8** %12, align 8
  store i8* %161, i8** %13, align 8
  store i8** %51, i8*** %14, align 8
  store i8* %163, i8** %15, align 8
  store i8* %168, i8** %16, align 8
  store i8** %49, i8*** %17, align 8
  %169 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %10, align 8
  %170 = bitcast %"class.std::__1::codecvt"* %169 to i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**, i8*, i8*, i8**)***
  %171 = load i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**, i8*, i8*, i8**)**, i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**, i8*, i8*, i8**)*** %170, align 8
  %172 = getelementptr inbounds i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**, i8*, i8*, i8**)*, i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**, i8*, i8*, i8**)** %171, i64 3
  %173 = load i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**, i8*, i8*, i8**)*, i32 (%"class.std::__1::codecvt"*, %union.__mbstate_t*, i8*, i8*, i8**, i8*, i8*, i8**)** %172, align 8
  %174 = load %union.__mbstate_t*, %union.__mbstate_t** %11, align 8
  %175 = load i8*, i8** %12, align 8
  %176 = load i8*, i8** %13, align 8
  %177 = load i8**, i8*** %14, align 8
  %178 = load i8*, i8** %15, align 8
  %179 = load i8*, i8** %16, align 8
  %180 = load i8**, i8*** %17, align 8
  %181 = call i32 %173(%"class.std::__1::codecvt"* %169, %union.__mbstate_t* dereferenceable(128) %174, i8* %175, i8* %176, i8** dereferenceable(8) %177, i8* %178, i8* %179, i8** dereferenceable(8) %180)
  store i32 %181, i32* %50, align 4
  %182 = load i8*, i8** %51, align 8
  %183 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %183, %"class.std::__1::basic_streambuf"** %18, align 8
  %184 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %18, align 8
  %185 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %184, i32 0, i32 5
  %186 = load i8*, i8** %185, align 8
  %187 = icmp eq i8* %182, %186
  br i1 %187, label %188, label %190

; <label>:188:                                    ; preds = %150
  %189 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  store i32 %189, i32* %42, align 4
  br label %298

; <label>:190:                                    ; preds = %150
  %191 = load i32, i32* %50, align 4
  %192 = icmp eq i32 %191, 3
  br i1 %192, label %193, label %218

; <label>:193:                                    ; preds = %190
  %194 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %194, %"class.std::__1::basic_streambuf"** %21, align 8
  %195 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %21, align 8
  %196 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %195, i32 0, i32 6
  %197 = load i8*, i8** %196, align 8
  %198 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %198, %"class.std::__1::basic_streambuf"** %22, align 8
  %199 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %22, align 8
  %200 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %199, i32 0, i32 5
  %201 = load i8*, i8** %200, align 8
  %202 = ptrtoint i8* %197 to i64
  %203 = ptrtoint i8* %201 to i64
  %204 = sub i64 %202, %203
  store i64 %204, i64* %52, align 8
  %205 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %205, %"class.std::__1::basic_streambuf"** %23, align 8
  %206 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %23, align 8
  %207 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %206, i32 0, i32 5
  %208 = load i8*, i8** %207, align 8
  %209 = load i64, i64* %52, align 8
  %210 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %54, i32 0, i32 8
  %211 = load %struct.__sFILE*, %struct.__sFILE** %210, align 8
  %212 = call i64 @"\01_fwrite"(i8* %208, i64 1, i64 %209, %struct.__sFILE* %211)
  %213 = load i64, i64* %52, align 8
  %214 = icmp ne i64 %212, %213
  br i1 %214, label %215, label %217

; <label>:215:                                    ; preds = %193
  %216 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  store i32 %216, i32* %42, align 4
  br label %298

; <label>:217:                                    ; preds = %193
  br label %280

; <label>:218:                                    ; preds = %190
  %219 = load i32, i32* %50, align 4
  %220 = icmp eq i32 %219, 0
  br i1 %220, label %224, label %221

; <label>:221:                                    ; preds = %218
  %222 = load i32, i32* %50, align 4
  %223 = icmp eq i32 %222, 1
  br i1 %223, label %224, label %277

; <label>:224:                                    ; preds = %221, %218
  %225 = load i8*, i8** %49, align 8
  %226 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %54, i32 0, i32 1
  %227 = load i8*, i8** %226, align 8
  %228 = ptrtoint i8* %225 to i64
  %229 = ptrtoint i8* %227 to i64
  %230 = sub i64 %228, %229
  store i64 %230, i64* %53, align 8
  %231 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %54, i32 0, i32 1
  %232 = load i8*, i8** %231, align 8
  %233 = load i64, i64* %53, align 8
  %234 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %54, i32 0, i32 8
  %235 = load %struct.__sFILE*, %struct.__sFILE** %234, align 8
  %236 = call i64 @"\01_fwrite"(i8* %232, i64 1, i64 %233, %struct.__sFILE* %235)
  %237 = load i64, i64* %53, align 8
  %238 = icmp ne i64 %236, %237
  br i1 %238, label %239, label %241

; <label>:239:                                    ; preds = %224
  %240 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  store i32 %240, i32* %42, align 4
  br label %298

; <label>:241:                                    ; preds = %224
  %242 = load i32, i32* %50, align 4
  %243 = icmp eq i32 %242, 1
  br i1 %243, label %244, label %276

; <label>:244:                                    ; preds = %241
  %245 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  %246 = load i8*, i8** %51, align 8
  %247 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %247, %"class.std::__1::basic_streambuf"** %28, align 8
  %248 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %28, align 8
  %249 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %248, i32 0, i32 6
  %250 = load i8*, i8** %249, align 8
  store %"class.std::__1::basic_streambuf"* %245, %"class.std::__1::basic_streambuf"** %29, align 8
  store i8* %246, i8** %30, align 8
  store i8* %250, i8** %31, align 8
  %251 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %29, align 8
  %252 = load i8*, i8** %30, align 8
  %253 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %251, i32 0, i32 6
  store i8* %252, i8** %253, align 8
  %254 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %251, i32 0, i32 5
  store i8* %252, i8** %254, align 8
  %255 = load i8*, i8** %31, align 8
  %256 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %251, i32 0, i32 7
  store i8* %255, i8** %256, align 8
  %257 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  %258 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %258, %"class.std::__1::basic_streambuf"** %32, align 8
  %259 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %32, align 8
  %260 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %259, i32 0, i32 7
  %261 = load i8*, i8** %260, align 8
  %262 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %262, %"class.std::__1::basic_streambuf"** %33, align 8
  %263 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %33, align 8
  %264 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %263, i32 0, i32 5
  %265 = load i8*, i8** %264, align 8
  %266 = ptrtoint i8* %261 to i64
  %267 = ptrtoint i8* %265 to i64
  %268 = sub i64 %266, %267
  %269 = trunc i64 %268 to i32
  store %"class.std::__1::basic_streambuf"* %257, %"class.std::__1::basic_streambuf"** %34, align 8
  store i32 %269, i32* %35, align 4
  %270 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %34, align 8
  %271 = load i32, i32* %35, align 4
  %272 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %270, i32 0, i32 6
  %273 = load i8*, i8** %272, align 8
  %274 = sext i32 %271 to i64
  %275 = getelementptr inbounds i8, i8* %273, i64 %274
  store i8* %275, i8** %272, align 8
  br label %276

; <label>:276:                                    ; preds = %244, %241
  br label %279

; <label>:277:                                    ; preds = %221
  %278 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  store i32 %278, i32* %42, align 4
  br label %298

; <label>:279:                                    ; preds = %276
  br label %280

; <label>:280:                                    ; preds = %279, %217
  br label %281

; <label>:281:                                    ; preds = %280
  %282 = load i32, i32* %50, align 4
  %283 = icmp eq i32 %282, 1
  br i1 %283, label %143, label %284

; <label>:284:                                    ; preds = %281
  br label %285

; <label>:285:                                    ; preds = %284, %139
  %286 = bitcast %"class.std::__1::basic_filebuf"* %54 to %"class.std::__1::basic_streambuf"*
  %287 = load i8*, i8** %46, align 8
  %288 = load i8*, i8** %47, align 8
  store %"class.std::__1::basic_streambuf"* %286, %"class.std::__1::basic_streambuf"** %37, align 8
  store i8* %287, i8** %38, align 8
  store i8* %288, i8** %39, align 8
  %289 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %37, align 8
  %290 = load i8*, i8** %38, align 8
  %291 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %289, i32 0, i32 6
  store i8* %290, i8** %291, align 8
  %292 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %289, i32 0, i32 5
  store i8* %290, i8** %292, align 8
  %293 = load i8*, i8** %39, align 8
  %294 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %289, i32 0, i32 7
  store i8* %293, i8** %294, align 8
  br label %295

; <label>:295:                                    ; preds = %285, %101
  %296 = load i32, i32* %44, align 4
  %297 = call i32 @_ZNSt3__111char_traitsIcE7not_eofEi(i32 %296) #15
  store i32 %297, i32* %42, align 4
  br label %298

; <label>:298:                                    ; preds = %295, %277, %239, %215, %188, %137, %58
  %299 = load i32, i32* %42, align 4
  ret i32 %299
}

declare i32 @fclose(%struct.__sFILE*) #1

; Function Attrs: nobuiltin
declare noalias i8* @_Znam(i64) #11

declare %"class.std::__1::locale::facet"* @_ZNKSt3__16locale9use_facetERNS0_2idE(%"class.std::__1::locale"*, %"class.std::__1::locale::id"* dereferenceable(16)) #1

declare i8* @__cxa_allocate_exception(i64)

; Function Attrs: nounwind
declare void @_ZNSt8bad_castC1Ev(%"class.std::bad_cast"*) unnamed_addr #3

; Function Attrs: nounwind
declare void @_ZNSt8bad_castD1Ev(%"class.std::bad_cast"*) unnamed_addr #3

declare void @__cxa_throw(i8*, i8*, i8*)

declare i32 @fseeko(%struct.__sFILE*, i64, i32) #1

declare i64 @ftello(%struct.__sFILE*) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #12

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #12

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr i32 @_ZNSt3__111char_traitsIcE3eofEv() #5 align 2 {
  ret i32 -1
}

declare i64 @"\01_fwrite"(i8*, i64, i64, %struct.__sFILE*) #1

declare i32 @fflush(%struct.__sFILE*) #1

; Function Attrs: nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE11__read_modeEv(%"class.std::__1::basic_filebuf"*) #9 align 2 {
  %2 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %3 = alloca i8*, align 8
  %4 = alloca i8*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i8*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %11 = alloca i8*, align 8
  %12 = alloca i8*, align 8
  %13 = alloca i1, align 1
  %14 = alloca %"class.std::__1::basic_filebuf"*, align 8
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %14, align 8
  %15 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %14, align 8
  %16 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 13
  %17 = load i32, i32* %16, align 4
  %18 = and i32 %17, 8
  %19 = icmp ne i32 %18, 0
  br i1 %19, label %75, label %20

; <label>:20:                                     ; preds = %1
  %21 = bitcast %"class.std::__1::basic_filebuf"* %15 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %21, %"class.std::__1::basic_streambuf"** %10, align 8
  store i8* null, i8** %11, align 8
  store i8* null, i8** %12, align 8
  %22 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %10, align 8
  %23 = load i8*, i8** %11, align 8
  %24 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %22, i32 0, i32 6
  store i8* %23, i8** %24, align 8
  %25 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %22, i32 0, i32 5
  store i8* %23, i8** %25, align 8
  %26 = load i8*, i8** %12, align 8
  %27 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %22, i32 0, i32 7
  store i8* %26, i8** %27, align 8
  %28 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 16
  %29 = load i8, i8* %28, align 2
  %30 = trunc i8 %29 to i1
  br i1 %30, label %31, label %52

; <label>:31:                                     ; preds = %20
  %32 = bitcast %"class.std::__1::basic_filebuf"* %15 to %"class.std::__1::basic_streambuf"*
  %33 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 1
  %34 = load i8*, i8** %33, align 8
  %35 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 1
  %36 = load i8*, i8** %35, align 8
  %37 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 5
  %38 = load i64, i64* %37, align 8
  %39 = getelementptr inbounds i8, i8* %36, i64 %38
  %40 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 1
  %41 = load i8*, i8** %40, align 8
  %42 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 5
  %43 = load i64, i64* %42, align 8
  %44 = getelementptr inbounds i8, i8* %41, i64 %43
  store %"class.std::__1::basic_streambuf"* %32, %"class.std::__1::basic_streambuf"** %2, align 8
  store i8* %34, i8** %3, align 8
  store i8* %39, i8** %4, align 8
  store i8* %44, i8** %5, align 8
  %45 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %2, align 8
  %46 = load i8*, i8** %3, align 8
  %47 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %45, i32 0, i32 2
  store i8* %46, i8** %47, align 8
  %48 = load i8*, i8** %4, align 8
  %49 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %45, i32 0, i32 3
  store i8* %48, i8** %49, align 8
  %50 = load i8*, i8** %5, align 8
  %51 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %45, i32 0, i32 4
  store i8* %50, i8** %51, align 8
  br label %73

; <label>:52:                                     ; preds = %20
  %53 = bitcast %"class.std::__1::basic_filebuf"* %15 to %"class.std::__1::basic_streambuf"*
  %54 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 6
  %55 = load i8*, i8** %54, align 8
  %56 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 6
  %57 = load i8*, i8** %56, align 8
  %58 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 7
  %59 = load i64, i64* %58, align 8
  %60 = getelementptr inbounds i8, i8* %57, i64 %59
  %61 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 6
  %62 = load i8*, i8** %61, align 8
  %63 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 7
  %64 = load i64, i64* %63, align 8
  %65 = getelementptr inbounds i8, i8* %62, i64 %64
  store %"class.std::__1::basic_streambuf"* %53, %"class.std::__1::basic_streambuf"** %6, align 8
  store i8* %55, i8** %7, align 8
  store i8* %60, i8** %8, align 8
  store i8* %65, i8** %9, align 8
  %66 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %6, align 8
  %67 = load i8*, i8** %7, align 8
  %68 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %66, i32 0, i32 2
  store i8* %67, i8** %68, align 8
  %69 = load i8*, i8** %8, align 8
  %70 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %66, i32 0, i32 3
  store i8* %69, i8** %70, align 8
  %71 = load i8*, i8** %9, align 8
  %72 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %66, i32 0, i32 4
  store i8* %71, i8** %72, align 8
  br label %73

; <label>:73:                                     ; preds = %52, %31
  %74 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %15, i32 0, i32 13
  store i32 8, i32* %74, align 4
  store i1 true, i1* %13, align 1
  br label %76

; <label>:75:                                     ; preds = %1
  store i1 false, i1* %13, align 1
  br label %76

; <label>:76:                                     ; preds = %75, %73
  %77 = load i1, i1* %13, align 1
  ret i1 %77
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memmove.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #12

declare i64 @fread(i8*, i64, i64, %struct.__sFILE*) #1

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr i32 @_ZNSt3__111char_traitsIcE11to_int_typeEc(i8 signext) #5 align 2 {
  %2 = alloca i8, align 1
  store i8 %0, i8* %2, align 1
  %3 = load i8, i8* %2, align 1
  %4 = zext i8 %3 to i32
  ret i32 %4
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZNSt3__111char_traitsIcE11eq_int_typeEii(i32, i32) #5 align 2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %0, i32* %3, align 4
  store i32 %1, i32* %4, align 4
  %5 = load i32, i32* %3, align 4
  %6 = load i32, i32* %4, align 4
  %7 = icmp eq i32 %5, %6
  ret i1 %7
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr i32 @_ZNSt3__111char_traitsIcE7not_eofEi(i32) #5 align 2 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  %5 = call zeroext i1 @_ZNSt3__111char_traitsIcE11eq_int_typeEii(i32 %3, i32 %4) #15
  br i1 %5, label %6, label %9

; <label>:6:                                      ; preds = %1
  %7 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  %8 = xor i32 %7, -1
  br label %11

; <label>:9:                                      ; preds = %1
  %10 = load i32, i32* %2, align 4
  br label %11

; <label>:11:                                     ; preds = %9, %6
  %12 = phi i32 [ %8, %6 ], [ %10, %9 ]
  ret i32 %12
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr zeroext i1 @_ZNSt3__111char_traitsIcE2eqEcc(i8 signext, i8 signext) #5 align 2 {
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  store i8 %0, i8* %3, align 1
  store i8 %1, i8* %4, align 1
  %5 = load i8, i8* %3, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, i8* %4, align 1
  %8 = sext i8 %7 to i32
  %9 = icmp eq i32 %6, %8
  ret i1 %9
}

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr signext i8 @_ZNSt3__111char_traitsIcE12to_char_typeEi(i32) #5 align 2 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = trunc i32 %3 to i8
  ret i8 %4
}

; Function Attrs: nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE12__write_modeEv(%"class.std::__1::basic_filebuf"*) #9 align 2 {
  %2 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %3 = alloca i8*, align 8
  %4 = alloca i8*, align 8
  %5 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca i8*, align 8
  %11 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %12 = alloca i8*, align 8
  %13 = alloca i8*, align 8
  %14 = alloca i8*, align 8
  %15 = alloca %"class.std::__1::basic_filebuf"*, align 8
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %15, align 8
  %16 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %15, align 8
  %17 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %16, i32 0, i32 13
  %18 = load i32, i32* %17, align 4
  %19 = and i32 %18, 16
  %20 = icmp ne i32 %19, 0
  br i1 %20, label %80, label %21

; <label>:21:                                     ; preds = %1
  %22 = bitcast %"class.std::__1::basic_filebuf"* %16 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %22, %"class.std::__1::basic_streambuf"** %11, align 8
  store i8* null, i8** %12, align 8
  store i8* null, i8** %13, align 8
  store i8* null, i8** %14, align 8
  %23 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %11, align 8
  %24 = load i8*, i8** %12, align 8
  %25 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %23, i32 0, i32 2
  store i8* %24, i8** %25, align 8
  %26 = load i8*, i8** %13, align 8
  %27 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %23, i32 0, i32 3
  store i8* %26, i8** %27, align 8
  %28 = load i8*, i8** %14, align 8
  %29 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %23, i32 0, i32 4
  store i8* %28, i8** %29, align 8
  %30 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %16, i32 0, i32 5
  %31 = load i64, i64* %30, align 8
  %32 = icmp ugt i64 %31, 8
  br i1 %32, label %33, label %70

; <label>:33:                                     ; preds = %21
  %34 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %16, i32 0, i32 16
  %35 = load i8, i8* %34, align 2
  %36 = trunc i8 %35 to i1
  br i1 %36, label %37, label %53

; <label>:37:                                     ; preds = %33
  %38 = bitcast %"class.std::__1::basic_filebuf"* %16 to %"class.std::__1::basic_streambuf"*
  %39 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %16, i32 0, i32 1
  %40 = load i8*, i8** %39, align 8
  %41 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %16, i32 0, i32 1
  %42 = load i8*, i8** %41, align 8
  %43 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %16, i32 0, i32 5
  %44 = load i64, i64* %43, align 8
  %45 = sub i64 %44, 1
  %46 = getelementptr inbounds i8, i8* %42, i64 %45
  store %"class.std::__1::basic_streambuf"* %38, %"class.std::__1::basic_streambuf"** %2, align 8
  store i8* %40, i8** %3, align 8
  store i8* %46, i8** %4, align 8
  %47 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %2, align 8
  %48 = load i8*, i8** %3, align 8
  %49 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %47, i32 0, i32 6
  store i8* %48, i8** %49, align 8
  %50 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %47, i32 0, i32 5
  store i8* %48, i8** %50, align 8
  %51 = load i8*, i8** %4, align 8
  %52 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %47, i32 0, i32 7
  store i8* %51, i8** %52, align 8
  br label %69

; <label>:53:                                     ; preds = %33
  %54 = bitcast %"class.std::__1::basic_filebuf"* %16 to %"class.std::__1::basic_streambuf"*
  %55 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %16, i32 0, i32 6
  %56 = load i8*, i8** %55, align 8
  %57 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %16, i32 0, i32 6
  %58 = load i8*, i8** %57, align 8
  %59 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %16, i32 0, i32 7
  %60 = load i64, i64* %59, align 8
  %61 = sub i64 %60, 1
  %62 = getelementptr inbounds i8, i8* %58, i64 %61
  store %"class.std::__1::basic_streambuf"* %54, %"class.std::__1::basic_streambuf"** %5, align 8
  store i8* %56, i8** %6, align 8
  store i8* %62, i8** %7, align 8
  %63 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %5, align 8
  %64 = load i8*, i8** %6, align 8
  %65 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %63, i32 0, i32 6
  store i8* %64, i8** %65, align 8
  %66 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %63, i32 0, i32 5
  store i8* %64, i8** %66, align 8
  %67 = load i8*, i8** %7, align 8
  %68 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %63, i32 0, i32 7
  store i8* %67, i8** %68, align 8
  br label %69

; <label>:69:                                     ; preds = %53, %37
  br label %78

; <label>:70:                                     ; preds = %21
  %71 = bitcast %"class.std::__1::basic_filebuf"* %16 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %71, %"class.std::__1::basic_streambuf"** %8, align 8
  store i8* null, i8** %9, align 8
  store i8* null, i8** %10, align 8
  %72 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %8, align 8
  %73 = load i8*, i8** %9, align 8
  %74 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %72, i32 0, i32 6
  store i8* %73, i8** %74, align 8
  %75 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %72, i32 0, i32 5
  store i8* %73, i8** %75, align 8
  %76 = load i8*, i8** %10, align 8
  %77 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %72, i32 0, i32 7
  store i8* %76, i8** %77, align 8
  br label %78

; <label>:78:                                     ; preds = %70, %69
  %79 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %16, i32 0, i32 13
  store i32 16, i32* %79, align 4
  br label %80

; <label>:80:                                     ; preds = %78, %1
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr dereferenceable(160) %"class.std::__1::basic_ostream"* @_ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m(%"class.std::__1::basic_ostream"* dereferenceable(160), i8*, i64) #0 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::ctype"*, align 8
  %5 = alloca i8, align 1
  %6 = alloca %"class.std::__1::locale"*, align 8
  %7 = alloca %"class.std::__1::basic_ios"*, align 8
  %8 = alloca i8, align 1
  %9 = alloca %"class.std::__1::locale", align 8
  %10 = alloca i8*
  %11 = alloca i32
  %12 = alloca %"class.std::__1::basic_ios"*, align 8
  %13 = alloca %"class.std::__1::ostreambuf_iterator"*, align 8
  %14 = alloca %"class.std::__1::ios_base"*, align 8
  %15 = alloca %"class.std::__1::ios_base"*, align 8
  %16 = alloca i32, align 4
  %17 = alloca %"class.std::__1::basic_ios"*, align 8
  %18 = alloca i32, align 4
  %19 = alloca %"class.std::__1::ios_base"*, align 8
  %20 = alloca %"class.std::__1::basic_ios"*, align 8
  %21 = alloca %"class.std::__1::ostreambuf_iterator"*, align 8
  %22 = alloca %"class.std::__1::basic_ostream"*, align 8
  %23 = alloca %"class.std::__1::ostreambuf_iterator"*, align 8
  %24 = alloca %"class.std::__1::basic_ostream"*, align 8
  %25 = alloca %"class.std::__1::basic_ostream<char, std::__1::char_traits<char> >::sentry"*, align 8
  %26 = alloca %"class.std::__1::basic_ostream"*, align 8
  %27 = alloca i8*, align 8
  %28 = alloca i64, align 8
  %29 = alloca %"class.std::__1::basic_ostream<char, std::__1::char_traits<char> >::sentry", align 8
  %30 = alloca i8*
  %31 = alloca i32
  %32 = alloca %"class.std::__1::ostreambuf_iterator", align 8
  %33 = alloca %"class.std::__1::ostreambuf_iterator", align 8
  store %"class.std::__1::basic_ostream"* %0, %"class.std::__1::basic_ostream"** %26, align 8
  store i8* %1, i8** %27, align 8
  store i64 %2, i64* %28, align 8
  %34 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %26, align 8
  invoke void @_ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE6sentryC1ERS3_(%"class.std::__1::basic_ostream<char, std::__1::char_traits<char> >::sentry"* %29, %"class.std::__1::basic_ostream"* dereferenceable(160) %34)
          to label %35 unwind label %174

; <label>:35:                                     ; preds = %3
  store %"class.std::__1::basic_ostream<char, std::__1::char_traits<char> >::sentry"* %29, %"class.std::__1::basic_ostream<char, std::__1::char_traits<char> >::sentry"** %25, align 8
  %36 = load %"class.std::__1::basic_ostream<char, std::__1::char_traits<char> >::sentry"*, %"class.std::__1::basic_ostream<char, std::__1::char_traits<char> >::sentry"** %25, align 8
  %37 = getelementptr inbounds %"class.std::__1::basic_ostream<char, std::__1::char_traits<char> >::sentry", %"class.std::__1::basic_ostream<char, std::__1::char_traits<char> >::sentry"* %36, i32 0, i32 0
  %38 = load i8, i8* %37, align 8
  %39 = trunc i8 %38 to i1
  br label %40

; <label>:40:                                     ; preds = %35
  br i1 %39, label %41, label %200

; <label>:41:                                     ; preds = %40
  %42 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %26, align 8
  store %"class.std::__1::ostreambuf_iterator"* %32, %"class.std::__1::ostreambuf_iterator"** %23, align 8
  store %"class.std::__1::basic_ostream"* %42, %"class.std::__1::basic_ostream"** %24, align 8
  %43 = load %"class.std::__1::ostreambuf_iterator"*, %"class.std::__1::ostreambuf_iterator"** %23, align 8
  %44 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %24, align 8
  store %"class.std::__1::ostreambuf_iterator"* %43, %"class.std::__1::ostreambuf_iterator"** %21, align 8
  store %"class.std::__1::basic_ostream"* %44, %"class.std::__1::basic_ostream"** %22, align 8
  %45 = load %"class.std::__1::ostreambuf_iterator"*, %"class.std::__1::ostreambuf_iterator"** %21, align 8
  %46 = bitcast %"class.std::__1::ostreambuf_iterator"* %45 to %"struct.std::__1::iterator.59"*
  %47 = getelementptr inbounds %"class.std::__1::ostreambuf_iterator", %"class.std::__1::ostreambuf_iterator"* %45, i32 0, i32 0
  %48 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %22, align 8
  %49 = bitcast %"class.std::__1::basic_ostream"* %48 to i8**
  %50 = load i8*, i8** %49, align 8
  %51 = getelementptr i8, i8* %50, i64 -24
  %52 = bitcast i8* %51 to i64*
  %53 = load i64, i64* %52, align 8
  %54 = bitcast %"class.std::__1::basic_ostream"* %48 to i8*
  %55 = getelementptr inbounds i8, i8* %54, i64 %53
  %56 = bitcast i8* %55 to %"class.std::__1::basic_ios"*
  store %"class.std::__1::basic_ios"* %56, %"class.std::__1::basic_ios"** %20, align 8
  %57 = load %"class.std::__1::basic_ios"*, %"class.std::__1::basic_ios"** %20, align 8
  %58 = bitcast %"class.std::__1::basic_ios"* %57 to %"class.std::__1::ios_base"*
  store %"class.std::__1::ios_base"* %58, %"class.std::__1::ios_base"** %19, align 8
  %59 = load %"class.std::__1::ios_base"*, %"class.std::__1::ios_base"** %19, align 8
  %60 = getelementptr inbounds %"class.std::__1::ios_base", %"class.std::__1::ios_base"* %59, i32 0, i32 6
  %61 = load i8*, i8** %60, align 8
  %62 = bitcast i8* %61 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %62, %"class.std::__1::basic_streambuf"** %47, align 8
  %63 = load i8*, i8** %27, align 8
  %64 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %26, align 8
  %65 = bitcast %"class.std::__1::basic_ostream"* %64 to i8**
  %66 = load i8*, i8** %65, align 8
  %67 = getelementptr i8, i8* %66, i64 -24
  %68 = bitcast i8* %67 to i64*
  %69 = load i64, i64* %68, align 8
  %70 = bitcast %"class.std::__1::basic_ostream"* %64 to i8*
  %71 = getelementptr inbounds i8, i8* %70, i64 %69
  %72 = bitcast i8* %71 to %"class.std::__1::ios_base"*
  store %"class.std::__1::ios_base"* %72, %"class.std::__1::ios_base"** %14, align 8
  %73 = load %"class.std::__1::ios_base"*, %"class.std::__1::ios_base"** %14, align 8
  %74 = getelementptr inbounds %"class.std::__1::ios_base", %"class.std::__1::ios_base"* %73, i32 0, i32 1
  %75 = load i32, i32* %74, align 8
  br label %76

; <label>:76:                                     ; preds = %41
  %77 = and i32 %75, 176
  %78 = icmp eq i32 %77, 32
  br i1 %78, label %79, label %83

; <label>:79:                                     ; preds = %76
  %80 = load i8*, i8** %27, align 8
  %81 = load i64, i64* %28, align 8
  %82 = getelementptr inbounds i8, i8* %80, i64 %81
  br label %85

; <label>:83:                                     ; preds = %76
  %84 = load i8*, i8** %27, align 8
  br label %85

; <label>:85:                                     ; preds = %83, %79
  %86 = phi i8* [ %82, %79 ], [ %84, %83 ]
  %87 = load i8*, i8** %27, align 8
  %88 = load i64, i64* %28, align 8
  %89 = getelementptr inbounds i8, i8* %87, i64 %88
  %90 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %26, align 8
  %91 = bitcast %"class.std::__1::basic_ostream"* %90 to i8**
  %92 = load i8*, i8** %91, align 8
  %93 = getelementptr i8, i8* %92, i64 -24
  %94 = bitcast i8* %93 to i64*
  %95 = load i64, i64* %94, align 8
  %96 = bitcast %"class.std::__1::basic_ostream"* %90 to i8*
  %97 = getelementptr inbounds i8, i8* %96, i64 %95
  %98 = bitcast i8* %97 to %"class.std::__1::ios_base"*
  %99 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %26, align 8
  %100 = bitcast %"class.std::__1::basic_ostream"* %99 to i8**
  %101 = load i8*, i8** %100, align 8
  %102 = getelementptr i8, i8* %101, i64 -24
  %103 = bitcast i8* %102 to i64*
  %104 = load i64, i64* %103, align 8
  %105 = bitcast %"class.std::__1::basic_ostream"* %99 to i8*
  %106 = getelementptr inbounds i8, i8* %105, i64 %104
  %107 = bitcast i8* %106 to %"class.std::__1::basic_ios"*
  store %"class.std::__1::basic_ios"* %107, %"class.std::__1::basic_ios"** %12, align 8
  %108 = load %"class.std::__1::basic_ios"*, %"class.std::__1::basic_ios"** %12, align 8
  %109 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  %110 = getelementptr inbounds %"class.std::__1::basic_ios", %"class.std::__1::basic_ios"* %108, i32 0, i32 2
  %111 = load i32, i32* %110, align 8
  %112 = call zeroext i1 @_ZNSt3__111char_traitsIcE11eq_int_typeEii(i32 %109, i32 %111) #15
  br i1 %112, label %113, label %140

; <label>:113:                                    ; preds = %85
  store %"class.std::__1::basic_ios"* %108, %"class.std::__1::basic_ios"** %7, align 8
  store i8 32, i8* %8, align 1
  %114 = load %"class.std::__1::basic_ios"*, %"class.std::__1::basic_ios"** %7, align 8
  %115 = bitcast %"class.std::__1::basic_ios"* %114 to %"class.std::__1::ios_base"*
  invoke void @_ZNKSt3__18ios_base6getlocEv(%"class.std::__1::locale"* sret %9, %"class.std::__1::ios_base"* %115)
          to label %116 unwind label %178

; <label>:116:                                    ; preds = %113
  store %"class.std::__1::locale"* %9, %"class.std::__1::locale"** %6, align 8
  %117 = load %"class.std::__1::locale"*, %"class.std::__1::locale"** %6, align 8
  %118 = invoke %"class.std::__1::locale::facet"* @_ZNKSt3__16locale9use_facetERNS0_2idE(%"class.std::__1::locale"* %117, %"class.std::__1::locale::id"* dereferenceable(16) @_ZNSt3__15ctypeIcE2idE)
          to label %119 unwind label %129

; <label>:119:                                    ; preds = %116
  %120 = bitcast %"class.std::__1::locale::facet"* %118 to %"class.std::__1::ctype"*
  %121 = load i8, i8* %8, align 1
  store %"class.std::__1::ctype"* %120, %"class.std::__1::ctype"** %4, align 8
  store i8 %121, i8* %5, align 1
  %122 = load %"class.std::__1::ctype"*, %"class.std::__1::ctype"** %4, align 8
  %123 = bitcast %"class.std::__1::ctype"* %122 to i8 (%"class.std::__1::ctype"*, i8)***
  %124 = load i8 (%"class.std::__1::ctype"*, i8)**, i8 (%"class.std::__1::ctype"*, i8)*** %123, align 8
  %125 = getelementptr inbounds i8 (%"class.std::__1::ctype"*, i8)*, i8 (%"class.std::__1::ctype"*, i8)** %124, i64 7
  %126 = load i8 (%"class.std::__1::ctype"*, i8)*, i8 (%"class.std::__1::ctype"*, i8)** %125, align 8
  %127 = load i8, i8* %5, align 1
  %128 = invoke signext i8 %126(%"class.std::__1::ctype"* %122, i8 signext %127)
          to label %137 unwind label %129

; <label>:129:                                    ; preds = %119, %116
  %130 = landingpad { i8*, i32 }
          cleanup
          catch i8* null
  %131 = extractvalue { i8*, i32 } %130, 0
  store i8* %131, i8** %10, align 8
  %132 = extractvalue { i8*, i32 } %130, 1
  store i32 %132, i32* %11, align 4
  call void @_ZNSt3__16localeD1Ev(%"class.std::__1::locale"* %9) #15
  %133 = load i8*, i8** %10, align 8
  %134 = load i32, i32* %11, align 4
  %135 = insertvalue { i8*, i32 } undef, i8* %133, 0
  %136 = insertvalue { i8*, i32 } %135, i32 %134, 1
  br label %180

; <label>:137:                                    ; preds = %119
  call void @_ZNSt3__16localeD1Ev(%"class.std::__1::locale"* %9) #15
  %138 = sext i8 %128 to i32
  %139 = getelementptr inbounds %"class.std::__1::basic_ios", %"class.std::__1::basic_ios"* %108, i32 0, i32 2
  store i32 %138, i32* %139, align 8
  br label %140

; <label>:140:                                    ; preds = %85, %137
  %141 = getelementptr inbounds %"class.std::__1::basic_ios", %"class.std::__1::basic_ios"* %108, i32 0, i32 2
  %142 = load i32, i32* %141, align 8
  %143 = trunc i32 %142 to i8
  br label %144

; <label>:144:                                    ; preds = %140
  %145 = getelementptr inbounds %"class.std::__1::ostreambuf_iterator", %"class.std::__1::ostreambuf_iterator"* %32, i32 0, i32 0
  %146 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %145, align 8
  %147 = invoke %"class.std::__1::basic_streambuf"* @_ZNSt3__116__pad_and_outputIcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_(%"class.std::__1::basic_streambuf"* %146, i8* %63, i8* %86, i8* %89, %"class.std::__1::ios_base"* dereferenceable(136) %98, i8 signext %143)
          to label %148 unwind label %178

; <label>:148:                                    ; preds = %144
  %149 = getelementptr inbounds %"class.std::__1::ostreambuf_iterator", %"class.std::__1::ostreambuf_iterator"* %33, i32 0, i32 0
  store %"class.std::__1::basic_streambuf"* %147, %"class.std::__1::basic_streambuf"** %149, align 8
  store %"class.std::__1::ostreambuf_iterator"* %33, %"class.std::__1::ostreambuf_iterator"** %13, align 8
  %150 = load %"class.std::__1::ostreambuf_iterator"*, %"class.std::__1::ostreambuf_iterator"** %13, align 8
  %151 = getelementptr inbounds %"class.std::__1::ostreambuf_iterator", %"class.std::__1::ostreambuf_iterator"* %150, i32 0, i32 0
  %152 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %151, align 8
  %153 = icmp eq %"class.std::__1::basic_streambuf"* %152, null
  br i1 %153, label %154, label %199

; <label>:154:                                    ; preds = %148
  %155 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %26, align 8
  %156 = bitcast %"class.std::__1::basic_ostream"* %155 to i8**
  %157 = load i8*, i8** %156, align 8
  %158 = getelementptr i8, i8* %157, i64 -24
  %159 = bitcast i8* %158 to i64*
  %160 = load i64, i64* %159, align 8
  %161 = bitcast %"class.std::__1::basic_ostream"* %155 to i8*
  %162 = getelementptr inbounds i8, i8* %161, i64 %160
  %163 = bitcast i8* %162 to %"class.std::__1::basic_ios"*
  store %"class.std::__1::basic_ios"* %163, %"class.std::__1::basic_ios"** %17, align 8
  store i32 5, i32* %18, align 4
  %164 = load %"class.std::__1::basic_ios"*, %"class.std::__1::basic_ios"** %17, align 8
  %165 = bitcast %"class.std::__1::basic_ios"* %164 to %"class.std::__1::ios_base"*
  %166 = load i32, i32* %18, align 4
  store %"class.std::__1::ios_base"* %165, %"class.std::__1::ios_base"** %15, align 8
  store i32 %166, i32* %16, align 4
  %167 = load %"class.std::__1::ios_base"*, %"class.std::__1::ios_base"** %15, align 8
  %168 = getelementptr inbounds %"class.std::__1::ios_base", %"class.std::__1::ios_base"* %167, i32 0, i32 4
  %169 = load i32, i32* %168, align 8
  %170 = load i32, i32* %16, align 4
  %171 = or i32 %169, %170
  invoke void @_ZNSt3__18ios_base5clearEj(%"class.std::__1::ios_base"* %167, i32 %171)
          to label %172 unwind label %178

; <label>:172:                                    ; preds = %154
  br label %173

; <label>:173:                                    ; preds = %172
  br label %199

; <label>:174:                                    ; preds = %3
  %175 = landingpad { i8*, i32 }
          catch i8* null
  %176 = extractvalue { i8*, i32 } %175, 0
  store i8* %176, i8** %30, align 8
  %177 = extractvalue { i8*, i32 } %175, 1
  store i32 %177, i32* %31, align 4
  br label %184

; <label>:178:                                    ; preds = %113, %154, %144
  %179 = landingpad { i8*, i32 }
          catch i8* null
  br label %180

; <label>:180:                                    ; preds = %129, %178
  %181 = phi { i8*, i32 } [ %179, %178 ], [ %136, %129 ]
  %182 = extractvalue { i8*, i32 } %181, 0
  store i8* %182, i8** %30, align 8
  %183 = extractvalue { i8*, i32 } %181, 1
  store i32 %183, i32* %31, align 4
  call void @_ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE6sentryD1Ev(%"class.std::__1::basic_ostream<char, std::__1::char_traits<char> >::sentry"* %29) #15
  br label %184

; <label>:184:                                    ; preds = %180, %174
  %185 = load i8*, i8** %30, align 8
  %186 = call i8* @__cxa_begin_catch(i8* %185) #15
  %187 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %26, align 8
  %188 = bitcast %"class.std::__1::basic_ostream"* %187 to i8**
  %189 = load i8*, i8** %188, align 8
  %190 = getelementptr i8, i8* %189, i64 -24
  %191 = bitcast i8* %190 to i64*
  %192 = load i64, i64* %191, align 8
  %193 = bitcast %"class.std::__1::basic_ostream"* %187 to i8*
  %194 = getelementptr inbounds i8, i8* %193, i64 %192
  %195 = bitcast i8* %194 to %"class.std::__1::ios_base"*
  invoke void @_ZNSt3__18ios_base33__set_badbit_and_consider_rethrowEv(%"class.std::__1::ios_base"* %195)
          to label %196 unwind label %201

; <label>:196:                                    ; preds = %184
  call void @__cxa_end_catch()
  br label %197

; <label>:197:                                    ; preds = %196, %200
  %198 = load %"class.std::__1::basic_ostream"*, %"class.std::__1::basic_ostream"** %26, align 8
  ret %"class.std::__1::basic_ostream"* %198

; <label>:199:                                    ; preds = %173, %148
  br label %200

; <label>:200:                                    ; preds = %199, %40
  call void @_ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE6sentryD1Ev(%"class.std::__1::basic_ostream<char, std::__1::char_traits<char> >::sentry"* %29) #15
  br label %197

; <label>:201:                                    ; preds = %184
  %202 = landingpad { i8*, i32 }
          cleanup
  %203 = extractvalue { i8*, i32 } %202, 0
  store i8* %203, i8** %30, align 8
  %204 = extractvalue { i8*, i32 } %202, 1
  store i32 %204, i32* %31, align 4
  invoke void @__cxa_end_catch()
          to label %205 unwind label %211

; <label>:205:                                    ; preds = %201
  br label %206

; <label>:206:                                    ; preds = %205
  %207 = load i8*, i8** %30, align 8
  %208 = load i32, i32* %31, align 4
  %209 = insertvalue { i8*, i32 } undef, i8* %207, 0
  %210 = insertvalue { i8*, i32 } %209, i32 %208, 1
  resume { i8*, i32 } %210

; <label>:211:                                    ; preds = %201
  %212 = landingpad { i8*, i32 }
          catch i8* null
  %213 = extractvalue { i8*, i32 } %212, 0
  call void @__clang_call_terminate(i8* %213) #16
  unreachable
}

; Function Attrs: inlinehint ssp uwtable
define linkonce_odr i64 @_ZNSt3__111char_traitsIcE6lengthEPKc(i8*) #13 align 2 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = load i8*, i8** %2, align 8
  %4 = call i64 @strlen(i8* %3)
  ret i64 %4
}

declare void @_ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE6sentryC1ERS3_(%"class.std::__1::basic_ostream<char, std::__1::char_traits<char> >::sentry"*, %"class.std::__1::basic_ostream"* dereferenceable(160)) unnamed_addr #1

; Function Attrs: ssp uwtable
define linkonce_odr hidden %"class.std::__1::basic_streambuf"* @_ZNSt3__116__pad_and_outputIcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_(%"class.std::__1::basic_streambuf"*, i8*, i8*, i8*, %"class.std::__1::ios_base"* dereferenceable(136), i8 signext) #0 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %7 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %8 = alloca i8*, align 8
  %9 = alloca i64, align 8
  %10 = alloca i8*, align 8
  %11 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %12 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %13 = alloca %"class.std::__1::basic_string"*, align 8
  %14 = alloca i8*, align 8
  %15 = alloca i8*, align 8
  %16 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %17 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %18 = alloca %"class.std::__1::basic_string"*, align 8
  %19 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %20 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %21 = alloca %"class.std::__1::basic_string"*, align 8
  %22 = alloca %"class.std::__1::basic_string"*, align 8
  %23 = alloca %"class.std::__1::basic_string"*, align 8
  %24 = alloca %"class.std::__1::allocator"*, align 8
  %25 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %26 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %27 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %28 = alloca %"class.std::__1::basic_string"*, align 8
  %29 = alloca i64, align 8
  %30 = alloca i8, align 1
  %31 = alloca %"class.std::__1::basic_string"*, align 8
  %32 = alloca i64, align 8
  %33 = alloca i8, align 1
  %34 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %35 = alloca i8*, align 8
  %36 = alloca i64, align 8
  %37 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %38 = alloca i8*, align 8
  %39 = alloca i64, align 8
  %40 = alloca %"class.std::__1::ios_base"*, align 8
  %41 = alloca i64, align 8
  %42 = alloca i64, align 8
  %43 = alloca %"class.std::__1::ios_base"*, align 8
  %44 = alloca %"class.std::__1::ostreambuf_iterator", align 8
  %45 = alloca %"class.std::__1::ostreambuf_iterator", align 8
  %46 = alloca i8*, align 8
  %47 = alloca i8*, align 8
  %48 = alloca i8*, align 8
  %49 = alloca %"class.std::__1::ios_base"*, align 8
  %50 = alloca i8, align 1
  %51 = alloca i64, align 8
  %52 = alloca i64, align 8
  %53 = alloca i64, align 8
  %54 = alloca %"class.std::__1::basic_string", align 8
  %55 = alloca i8*
  %56 = alloca i32
  %57 = alloca i32
  %58 = getelementptr inbounds %"class.std::__1::ostreambuf_iterator", %"class.std::__1::ostreambuf_iterator"* %45, i32 0, i32 0
  store %"class.std::__1::basic_streambuf"* %0, %"class.std::__1::basic_streambuf"** %58, align 8
  store i8* %1, i8** %46, align 8
  store i8* %2, i8** %47, align 8
  store i8* %3, i8** %48, align 8
  store %"class.std::__1::ios_base"* %4, %"class.std::__1::ios_base"** %49, align 8
  store i8 %5, i8* %50, align 1
  %59 = getelementptr inbounds %"class.std::__1::ostreambuf_iterator", %"class.std::__1::ostreambuf_iterator"* %45, i32 0, i32 0
  %60 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %59, align 8
  %61 = icmp eq %"class.std::__1::basic_streambuf"* %60, null
  br i1 %61, label %62, label %65

; <label>:62:                                     ; preds = %6
  %63 = bitcast %"class.std::__1::ostreambuf_iterator"* %44 to i8*
  %64 = bitcast %"class.std::__1::ostreambuf_iterator"* %45 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %63, i8* %64, i64 8, i32 8, i1 false)
  br label %241

; <label>:65:                                     ; preds = %6
  %66 = load i8*, i8** %48, align 8
  %67 = load i8*, i8** %46, align 8
  %68 = ptrtoint i8* %66 to i64
  %69 = ptrtoint i8* %67 to i64
  %70 = sub i64 %68, %69
  store i64 %70, i64* %51, align 8
  %71 = load %"class.std::__1::ios_base"*, %"class.std::__1::ios_base"** %49, align 8
  store %"class.std::__1::ios_base"* %71, %"class.std::__1::ios_base"** %43, align 8
  %72 = load %"class.std::__1::ios_base"*, %"class.std::__1::ios_base"** %43, align 8
  %73 = getelementptr inbounds %"class.std::__1::ios_base", %"class.std::__1::ios_base"* %72, i32 0, i32 3
  %74 = load i64, i64* %73, align 8
  store i64 %74, i64* %52, align 8
  %75 = load i64, i64* %52, align 8
  %76 = load i64, i64* %51, align 8
  %77 = icmp sgt i64 %75, %76
  br i1 %77, label %78, label %82

; <label>:78:                                     ; preds = %65
  %79 = load i64, i64* %51, align 8
  %80 = load i64, i64* %52, align 8
  %81 = sub nsw i64 %80, %79
  store i64 %81, i64* %52, align 8
  br label %83

; <label>:82:                                     ; preds = %65
  store i64 0, i64* %52, align 8
  br label %83

; <label>:83:                                     ; preds = %82, %78
  %84 = load i8*, i8** %47, align 8
  %85 = load i8*, i8** %46, align 8
  %86 = ptrtoint i8* %84 to i64
  %87 = ptrtoint i8* %85 to i64
  %88 = sub i64 %86, %87
  store i64 %88, i64* %53, align 8
  %89 = load i64, i64* %53, align 8
  %90 = icmp sgt i64 %89, 0
  br i1 %90, label %91, label %111

; <label>:91:                                     ; preds = %83
  %92 = getelementptr inbounds %"class.std::__1::ostreambuf_iterator", %"class.std::__1::ostreambuf_iterator"* %45, i32 0, i32 0
  %93 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %92, align 8
  %94 = load i8*, i8** %46, align 8
  %95 = load i64, i64* %53, align 8
  store %"class.std::__1::basic_streambuf"* %93, %"class.std::__1::basic_streambuf"** %34, align 8
  store i8* %94, i8** %35, align 8
  store i64 %95, i64* %36, align 8
  %96 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %34, align 8
  %97 = bitcast %"class.std::__1::basic_streambuf"* %96 to i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)***
  %98 = load i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)**, i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)*** %97, align 8
  %99 = getelementptr inbounds i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)*, i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)** %98, i64 12
  %100 = load i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)*, i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)** %99, align 8
  %101 = load i8*, i8** %35, align 8
  %102 = load i64, i64* %36, align 8
  %103 = call i64 %100(%"class.std::__1::basic_streambuf"* %96, i8* %101, i64 %102)
  %104 = load i64, i64* %53, align 8
  %105 = icmp ne i64 %103, %104
  br i1 %105, label %106, label %110

; <label>:106:                                    ; preds = %91
  %107 = getelementptr inbounds %"class.std::__1::ostreambuf_iterator", %"class.std::__1::ostreambuf_iterator"* %45, i32 0, i32 0
  store %"class.std::__1::basic_streambuf"* null, %"class.std::__1::basic_streambuf"** %107, align 8
  %108 = bitcast %"class.std::__1::ostreambuf_iterator"* %44 to i8*
  %109 = bitcast %"class.std::__1::ostreambuf_iterator"* %45 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %108, i8* %109, i64 8, i32 8, i1 false)
  br label %241

; <label>:110:                                    ; preds = %91
  br label %111

; <label>:111:                                    ; preds = %110, %83
  %112 = load i64, i64* %52, align 8
  %113 = icmp sgt i64 %112, 0
  br i1 %113, label %114, label %203

; <label>:114:                                    ; preds = %111
  %115 = load i64, i64* %52, align 8
  %116 = load i8, i8* %50, align 1
  store %"class.std::__1::basic_string"* %54, %"class.std::__1::basic_string"** %31, align 8
  store i64 %115, i64* %32, align 8
  store i8 %116, i8* %33, align 1
  %117 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %31, align 8
  %118 = load i64, i64* %32, align 8
  %119 = load i8, i8* %33, align 1
  store %"class.std::__1::basic_string"* %117, %"class.std::__1::basic_string"** %28, align 8
  store i64 %118, i64* %29, align 8
  store i8 %119, i8* %30, align 1
  %120 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8
  %121 = bitcast %"class.std::__1::basic_string"* %120 to %"class.std::__1::__basic_string_common"*
  %122 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %120, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %122, %"class.std::__1::__compressed_pair"** %27, align 8
  %123 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %27, align 8
  store %"class.std::__1::__compressed_pair"* %123, %"class.std::__1::__compressed_pair"** %26, align 8
  %124 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %26, align 8
  %125 = bitcast %"class.std::__1::__compressed_pair"* %124 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %125, %"class.std::__1::__libcpp_compressed_pair_imp"** %25, align 8
  %126 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %25, align 8
  %127 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %126 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %127, %"class.std::__1::allocator"** %24, align 8
  %128 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %24, align 8
  %129 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %126, i32 0, i32 0
  %130 = bitcast %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %129 to i8*
  call void @llvm.memset.p0i8.i64(i8* %130, i8 0, i64 24, i32 8, i1 false) #15
  %131 = load i64, i64* %29, align 8
  %132 = load i8, i8* %30, align 1
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEmc(%"class.std::__1::basic_string"* %120, i64 %131, i8 signext %132)
  %133 = getelementptr inbounds %"class.std::__1::ostreambuf_iterator", %"class.std::__1::ostreambuf_iterator"* %45, i32 0, i32 0
  %134 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %133, align 8
  store %"class.std::__1::basic_string"* %54, %"class.std::__1::basic_string"** %23, align 8
  %135 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %23, align 8
  store %"class.std::__1::basic_string"* %135, %"class.std::__1::basic_string"** %22, align 8
  %136 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %22, align 8
  store %"class.std::__1::basic_string"* %136, %"class.std::__1::basic_string"** %21, align 8
  %137 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %21, align 8
  %138 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %137, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %138, %"class.std::__1::__compressed_pair"** %20, align 8
  %139 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %20, align 8
  %140 = bitcast %"class.std::__1::__compressed_pair"* %139 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %140, %"class.std::__1::__libcpp_compressed_pair_imp"** %19, align 8
  %141 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %19, align 8
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

; <label>:151:                                    ; preds = %114
  store %"class.std::__1::basic_string"* %136, %"class.std::__1::basic_string"** %13, align 8
  %152 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8
  %153 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %152, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %153, %"class.std::__1::__compressed_pair"** %12, align 8
  %154 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %12, align 8
  %155 = bitcast %"class.std::__1::__compressed_pair"* %154 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %155, %"class.std::__1::__libcpp_compressed_pair_imp"** %11, align 8
  %156 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %11, align 8
  %157 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %156, i32 0, i32 0
  %158 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %157, i32 0, i32 0
  %159 = bitcast %union.anon* %158 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %160 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %159, i32 0, i32 2
  %161 = load i8*, i8** %160, align 8
  br label %175

; <label>:162:                                    ; preds = %114
  store %"class.std::__1::basic_string"* %136, %"class.std::__1::basic_string"** %18, align 8
  %163 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %18, align 8
  %164 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %163, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %164, %"class.std::__1::__compressed_pair"** %17, align 8
  %165 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %17, align 8
  %166 = bitcast %"class.std::__1::__compressed_pair"* %165 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %166, %"class.std::__1::__libcpp_compressed_pair_imp"** %16, align 8
  %167 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %16, align 8
  %168 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %167, i32 0, i32 0
  %169 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %168, i32 0, i32 0
  %170 = bitcast %union.anon* %169 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %171 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %170, i32 0, i32 1
  %172 = getelementptr inbounds [23 x i8], [23 x i8]* %171, i64 0, i64 0
  store i8* %172, i8** %15, align 8
  %173 = load i8*, i8** %15, align 8
  store i8* %173, i8** %14, align 8
  %174 = load i8*, i8** %14, align 8
  br label %175

; <label>:175:                                    ; preds = %151, %162
  %176 = phi i8* [ %161, %151 ], [ %174, %162 ]
  store i8* %176, i8** %10, align 8
  %177 = load i8*, i8** %10, align 8
  %178 = load i64, i64* %52, align 8
  store %"class.std::__1::basic_streambuf"* %134, %"class.std::__1::basic_streambuf"** %7, align 8
  store i8* %177, i8** %8, align 8
  store i64 %178, i64* %9, align 8
  %179 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %7, align 8
  %180 = bitcast %"class.std::__1::basic_streambuf"* %179 to i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)***
  %181 = load i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)**, i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)*** %180, align 8
  %182 = getelementptr inbounds i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)*, i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)** %181, i64 12
  %183 = load i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)*, i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)** %182, align 8
  %184 = load i8*, i8** %8, align 8
  %185 = load i64, i64* %9, align 8
  %186 = invoke i64 %183(%"class.std::__1::basic_streambuf"* %179, i8* %184, i64 %185)
          to label %187 unwind label %195

; <label>:187:                                    ; preds = %175
  br label %188

; <label>:188:                                    ; preds = %187
  %189 = load i64, i64* %52, align 8
  %190 = icmp ne i64 %186, %189
  br i1 %190, label %191, label %199

; <label>:191:                                    ; preds = %188
  %192 = getelementptr inbounds %"class.std::__1::ostreambuf_iterator", %"class.std::__1::ostreambuf_iterator"* %45, i32 0, i32 0
  store %"class.std::__1::basic_streambuf"* null, %"class.std::__1::basic_streambuf"** %192, align 8
  %193 = bitcast %"class.std::__1::ostreambuf_iterator"* %44 to i8*
  %194 = bitcast %"class.std::__1::ostreambuf_iterator"* %45 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %193, i8* %194, i64 8, i32 8, i1 false)
  store i32 1, i32* %57, align 4
  br label %200

; <label>:195:                                    ; preds = %175
  %196 = landingpad { i8*, i32 }
          cleanup
  %197 = extractvalue { i8*, i32 } %196, 0
  store i8* %197, i8** %55, align 8
  %198 = extractvalue { i8*, i32 } %196, 1
  store i32 %198, i32* %56, align 4
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %54) #15
  br label %244

; <label>:199:                                    ; preds = %188
  store i32 0, i32* %57, align 4
  br label %200

; <label>:200:                                    ; preds = %199, %191
  call void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED1Ev(%"class.std::__1::basic_string"* %54) #15
  %201 = load i32, i32* %57, align 4
  switch i32 %201, label %249 [
    i32 0, label %202
    i32 1, label %241
  ]

; <label>:202:                                    ; preds = %200
  br label %203

; <label>:203:                                    ; preds = %202, %111
  %204 = load i8*, i8** %48, align 8
  %205 = load i8*, i8** %47, align 8
  %206 = ptrtoint i8* %204 to i64
  %207 = ptrtoint i8* %205 to i64
  %208 = sub i64 %206, %207
  store i64 %208, i64* %53, align 8
  %209 = load i64, i64* %53, align 8
  %210 = icmp sgt i64 %209, 0
  br i1 %210, label %211, label %231

; <label>:211:                                    ; preds = %203
  %212 = getelementptr inbounds %"class.std::__1::ostreambuf_iterator", %"class.std::__1::ostreambuf_iterator"* %45, i32 0, i32 0
  %213 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %212, align 8
  %214 = load i8*, i8** %47, align 8
  %215 = load i64, i64* %53, align 8
  store %"class.std::__1::basic_streambuf"* %213, %"class.std::__1::basic_streambuf"** %37, align 8
  store i8* %214, i8** %38, align 8
  store i64 %215, i64* %39, align 8
  %216 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %37, align 8
  %217 = bitcast %"class.std::__1::basic_streambuf"* %216 to i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)***
  %218 = load i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)**, i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)*** %217, align 8
  %219 = getelementptr inbounds i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)*, i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)** %218, i64 12
  %220 = load i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)*, i64 (%"class.std::__1::basic_streambuf"*, i8*, i64)** %219, align 8
  %221 = load i8*, i8** %38, align 8
  %222 = load i64, i64* %39, align 8
  %223 = call i64 %220(%"class.std::__1::basic_streambuf"* %216, i8* %221, i64 %222)
  %224 = load i64, i64* %53, align 8
  %225 = icmp ne i64 %223, %224
  br i1 %225, label %226, label %230

; <label>:226:                                    ; preds = %211
  %227 = getelementptr inbounds %"class.std::__1::ostreambuf_iterator", %"class.std::__1::ostreambuf_iterator"* %45, i32 0, i32 0
  store %"class.std::__1::basic_streambuf"* null, %"class.std::__1::basic_streambuf"** %227, align 8
  %228 = bitcast %"class.std::__1::ostreambuf_iterator"* %44 to i8*
  %229 = bitcast %"class.std::__1::ostreambuf_iterator"* %45 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %228, i8* %229, i64 8, i32 8, i1 false)
  br label %241

; <label>:230:                                    ; preds = %211
  br label %231

; <label>:231:                                    ; preds = %230, %203
  %232 = load %"class.std::__1::ios_base"*, %"class.std::__1::ios_base"** %49, align 8
  store %"class.std::__1::ios_base"* %232, %"class.std::__1::ios_base"** %40, align 8
  store i64 0, i64* %41, align 8
  %233 = load %"class.std::__1::ios_base"*, %"class.std::__1::ios_base"** %40, align 8
  %234 = getelementptr inbounds %"class.std::__1::ios_base", %"class.std::__1::ios_base"* %233, i32 0, i32 3
  %235 = load i64, i64* %234, align 8
  store i64 %235, i64* %42, align 8
  %236 = load i64, i64* %41, align 8
  %237 = getelementptr inbounds %"class.std::__1::ios_base", %"class.std::__1::ios_base"* %233, i32 0, i32 3
  store i64 %236, i64* %237, align 8
  %238 = load i64, i64* %42, align 8
  %239 = bitcast %"class.std::__1::ostreambuf_iterator"* %44 to i8*
  %240 = bitcast %"class.std::__1::ostreambuf_iterator"* %45 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %239, i8* %240, i64 8, i32 8, i1 false)
  br label %241

; <label>:241:                                    ; preds = %231, %226, %200, %106, %62
  %242 = getelementptr inbounds %"class.std::__1::ostreambuf_iterator", %"class.std::__1::ostreambuf_iterator"* %44, i32 0, i32 0
  %243 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %242, align 8
  ret %"class.std::__1::basic_streambuf"* %243

; <label>:244:                                    ; preds = %195
  %245 = load i8*, i8** %55, align 8
  %246 = load i32, i32* %56, align 4
  %247 = insertvalue { i8*, i32 } undef, i8* %245, 0
  %248 = insertvalue { i8*, i32 } %247, i32 %246, 1
  resume { i8*, i32 } %248

; <label>:249:                                    ; preds = %200
  unreachable
}

; Function Attrs: nounwind
declare void @_ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE6sentryD1Ev(%"class.std::__1::basic_ostream<char, std::__1::char_traits<char> >::sentry"*) unnamed_addr #3

declare void @_ZNSt3__18ios_base33__set_badbit_and_consider_rethrowEv(%"class.std::__1::ios_base"*) #1

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initEmc(%"class.std::__1::basic_string"*, i64, i8 signext) #1

declare void @_ZNKSt3__18ios_base6getlocEv(%"class.std::__1::locale"* sret, %"class.std::__1::ios_base"*) #1

; Function Attrs: nounwind
declare void @_ZNSt3__16localeD1Ev(%"class.std::__1::locale"*) unnamed_addr #3

declare void @_ZNSt3__18ios_base5clearEj(%"class.std::__1::ios_base"*, i32) #1

declare i64 @strlen(i8*) #1

; Function Attrs: ssp uwtable
define linkonce_odr dereferenceable(168) %"class.std::__1::basic_istream"* @_ZNSt3__17getlineIcNS_11char_traitsIcEENS_9allocatorIcEEEERNS_13basic_istreamIT_T0_EES9_RNS_12basic_stringIS6_S7_T1_EES6_(%"class.std::__1::basic_istream"* dereferenceable(168), %"class.std::__1::basic_string"* dereferenceable(24), i8 signext) #0 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca %"class.std::__1::ios_base"*, align 8
  %5 = alloca i32, align 4
  %6 = alloca %"class.std::__1::basic_ios"*, align 8
  %7 = alloca i32, align 4
  %8 = alloca %"class.std::__1::allocator"*, align 8
  %9 = alloca %"struct.std::__1::integral_constant", align 1
  %10 = alloca %"class.std::__1::allocator"*, align 8
  %11 = alloca %"class.std::__1::allocator"*, align 8
  %12 = alloca %"struct.std::__1::integral_constant", align 1
  %13 = alloca %"struct.std::__1::__has_max_size", align 1
  %14 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %15 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %16 = alloca %"class.std::__1::basic_string"*, align 8
  %17 = alloca %"class.std::__1::basic_string"*, align 8
  %18 = alloca i64, align 8
  %19 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %20 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %21 = alloca %"class.std::__1::basic_string"*, align 8
  %22 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %23 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %24 = alloca %"class.std::__1::basic_string"*, align 8
  %25 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %26 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %27 = alloca %"class.std::__1::basic_string"*, align 8
  %28 = alloca %"class.std::__1::basic_string"*, align 8
  %29 = alloca i32, align 4
  %30 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %31 = alloca %"class.std::__1::ios_base"*, align 8
  %32 = alloca %"class.std::__1::basic_ios"*, align 8
  %33 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %34 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %35 = alloca %"class.std::__1::basic_string"*, align 8
  %36 = alloca i64, align 8
  %37 = alloca i8*, align 8
  %38 = alloca i8*, align 8
  %39 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %40 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %41 = alloca %"class.std::__1::basic_string"*, align 8
  %42 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %43 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %44 = alloca %"class.std::__1::basic_string"*, align 8
  %45 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %46 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %47 = alloca %"class.std::__1::basic_string"*, align 8
  %48 = alloca i64, align 8
  %49 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %50 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %51 = alloca %"class.std::__1::basic_string"*, align 8
  %52 = alloca %"class.std::__1::basic_string"*, align 8
  %53 = alloca %"class.std::__1::basic_string"*, align 8
  %54 = alloca i8, align 1
  %55 = alloca i8, align 1
  %56 = alloca %"class.std::__1::basic_istream<char, std::__1::char_traits<char> >::sentry"*, align 8
  %57 = alloca %"class.std::__1::basic_istream"*, align 8
  %58 = alloca %"class.std::__1::basic_string"*, align 8
  %59 = alloca i8, align 1
  %60 = alloca %"class.std::__1::basic_istream<char, std::__1::char_traits<char> >::sentry", align 1
  %61 = alloca i8*
  %62 = alloca i32
  %63 = alloca i32, align 4
  %64 = alloca i64, align 8
  %65 = alloca i32, align 4
  %66 = alloca i8, align 1
  store %"class.std::__1::basic_istream"* %0, %"class.std::__1::basic_istream"** %57, align 8
  store %"class.std::__1::basic_string"* %1, %"class.std::__1::basic_string"** %58, align 8
  store i8 %2, i8* %59, align 1
  %67 = load %"class.std::__1::basic_istream"*, %"class.std::__1::basic_istream"** %57, align 8
  invoke void @_ZNSt3__113basic_istreamIcNS_11char_traitsIcEEE6sentryC1ERS3_b(%"class.std::__1::basic_istream<char, std::__1::char_traits<char> >::sentry"* %60, %"class.std::__1::basic_istream"* dereferenceable(168) %67, i1 zeroext true)
          to label %68 unwind label %186

; <label>:68:                                     ; preds = %3
  store %"class.std::__1::basic_istream<char, std::__1::char_traits<char> >::sentry"* %60, %"class.std::__1::basic_istream<char, std::__1::char_traits<char> >::sentry"** %56, align 8
  %69 = load %"class.std::__1::basic_istream<char, std::__1::char_traits<char> >::sentry"*, %"class.std::__1::basic_istream<char, std::__1::char_traits<char> >::sentry"** %56, align 8
  %70 = getelementptr inbounds %"class.std::__1::basic_istream<char, std::__1::char_traits<char> >::sentry", %"class.std::__1::basic_istream<char, std::__1::char_traits<char> >::sentry"* %69, i32 0, i32 0
  %71 = load i8, i8* %70, align 1
  %72 = trunc i8 %71 to i1
  br label %73

; <label>:73:                                     ; preds = %68
  br i1 %72, label %74, label %308

; <label>:74:                                     ; preds = %73
  %75 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8
  store %"class.std::__1::basic_string"* %75, %"class.std::__1::basic_string"** %53, align 8
  %76 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %53, align 8
  store %"class.std::__1::basic_string"* %76, %"class.std::__1::basic_string"** %52, align 8
  %77 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %52, align 8
  store %"class.std::__1::basic_string"* %76, %"class.std::__1::basic_string"** %51, align 8
  %78 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %51, align 8
  %79 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %78, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %79, %"class.std::__1::__compressed_pair"** %50, align 8
  %80 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %50, align 8
  %81 = bitcast %"class.std::__1::__compressed_pair"* %80 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %81, %"class.std::__1::__libcpp_compressed_pair_imp"** %49, align 8
  %82 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %49, align 8
  %83 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %82, i32 0, i32 0
  %84 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %83, i32 0, i32 0
  %85 = bitcast %union.anon* %84 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %86 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %85, i32 0, i32 0
  %87 = bitcast %union.anon.0* %86 to i8*
  %88 = load i8, i8* %87, align 8
  %89 = zext i8 %88 to i32
  %90 = and i32 %89, 1
  %91 = icmp ne i32 %90, 0
  br i1 %91, label %92, label %113

; <label>:92:                                     ; preds = %74
  store %"class.std::__1::basic_string"* %76, %"class.std::__1::basic_string"** %44, align 8
  %93 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %44, align 8
  %94 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %93, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %94, %"class.std::__1::__compressed_pair"** %43, align 8
  %95 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %43, align 8
  %96 = bitcast %"class.std::__1::__compressed_pair"* %95 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %96, %"class.std::__1::__libcpp_compressed_pair_imp"** %42, align 8
  %97 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %42, align 8
  %98 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %97, i32 0, i32 0
  %99 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %98, i32 0, i32 0
  %100 = bitcast %union.anon* %99 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %101 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %100, i32 0, i32 2
  %102 = load i8*, i8** %101, align 8
  store i8 0, i8* %54, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %102, i8* dereferenceable(1) %54) #15
  store %"class.std::__1::basic_string"* %76, %"class.std::__1::basic_string"** %35, align 8
  store i64 0, i64* %36, align 8
  %103 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %35, align 8
  %104 = load i64, i64* %36, align 8
  %105 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %103, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %105, %"class.std::__1::__compressed_pair"** %34, align 8
  %106 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %34, align 8
  %107 = bitcast %"class.std::__1::__compressed_pair"* %106 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %107, %"class.std::__1::__libcpp_compressed_pair_imp"** %33, align 8
  %108 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %33, align 8
  %109 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %108, i32 0, i32 0
  %110 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %109, i32 0, i32 0
  %111 = bitcast %union.anon* %110 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %112 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %111, i32 0, i32 1
  store i64 %104, i64* %112, align 8
  br label %139

; <label>:113:                                    ; preds = %74
  store %"class.std::__1::basic_string"* %76, %"class.std::__1::basic_string"** %41, align 8
  %114 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8
  %115 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %114, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %115, %"class.std::__1::__compressed_pair"** %40, align 8
  %116 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %40, align 8
  %117 = bitcast %"class.std::__1::__compressed_pair"* %116 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %117, %"class.std::__1::__libcpp_compressed_pair_imp"** %39, align 8
  %118 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %39, align 8
  %119 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %118, i32 0, i32 0
  %120 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %119, i32 0, i32 0
  %121 = bitcast %union.anon* %120 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %122 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %121, i32 0, i32 1
  %123 = getelementptr inbounds [23 x i8], [23 x i8]* %122, i64 0, i64 0
  store i8* %123, i8** %38, align 8
  %124 = load i8*, i8** %38, align 8
  store i8* %124, i8** %37, align 8
  %125 = load i8*, i8** %37, align 8
  store i8 0, i8* %55, align 1
  call void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1) %125, i8* dereferenceable(1) %55) #15
  store %"class.std::__1::basic_string"* %76, %"class.std::__1::basic_string"** %47, align 8
  store i64 0, i64* %48, align 8
  %126 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %47, align 8
  %127 = load i64, i64* %48, align 8
  %128 = shl i64 %127, 1
  %129 = trunc i64 %128 to i8
  %130 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %126, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %130, %"class.std::__1::__compressed_pair"** %46, align 8
  %131 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %46, align 8
  %132 = bitcast %"class.std::__1::__compressed_pair"* %131 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %132, %"class.std::__1::__libcpp_compressed_pair_imp"** %45, align 8
  %133 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %45, align 8
  %134 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %133, i32 0, i32 0
  %135 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %134, i32 0, i32 0
  %136 = bitcast %union.anon* %135 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %137 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %136, i32 0, i32 0
  %138 = bitcast %union.anon.0* %137 to i8*
  store i8 %129, i8* %138, align 8
  br label %139

; <label>:139:                                    ; preds = %92, %113
  store i32 0, i32* %63, align 4
  store i64 0, i64* %64, align 8
  br label %140

; <label>:140:                                    ; preds = %280, %139
  br label %141

; <label>:141:                                    ; preds = %140
  %142 = load %"class.std::__1::basic_istream"*, %"class.std::__1::basic_istream"** %57, align 8
  %143 = bitcast %"class.std::__1::basic_istream"* %142 to i8**
  %144 = load i8*, i8** %143, align 8
  %145 = getelementptr i8, i8* %144, i64 -24
  %146 = bitcast i8* %145 to i64*
  %147 = load i64, i64* %146, align 8
  %148 = bitcast %"class.std::__1::basic_istream"* %142 to i8*
  %149 = getelementptr inbounds i8, i8* %148, i64 %147
  %150 = bitcast i8* %149 to %"class.std::__1::basic_ios"*
  store %"class.std::__1::basic_ios"* %150, %"class.std::__1::basic_ios"** %32, align 8
  %151 = load %"class.std::__1::basic_ios"*, %"class.std::__1::basic_ios"** %32, align 8
  %152 = bitcast %"class.std::__1::basic_ios"* %151 to %"class.std::__1::ios_base"*
  store %"class.std::__1::ios_base"* %152, %"class.std::__1::ios_base"** %31, align 8
  %153 = load %"class.std::__1::ios_base"*, %"class.std::__1::ios_base"** %31, align 8
  %154 = getelementptr inbounds %"class.std::__1::ios_base", %"class.std::__1::ios_base"* %153, i32 0, i32 6
  %155 = load i8*, i8** %154, align 8
  %156 = bitcast i8* %155 to %"class.std::__1::basic_streambuf"*
  br label %157

; <label>:157:                                    ; preds = %141
  store %"class.std::__1::basic_streambuf"* %156, %"class.std::__1::basic_streambuf"** %30, align 8
  %158 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %30, align 8
  %159 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %158, i32 0, i32 3
  %160 = load i8*, i8** %159, align 8
  %161 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %158, i32 0, i32 4
  %162 = load i8*, i8** %161, align 8
  %163 = icmp eq i8* %160, %162
  br i1 %163, label %164, label %171

; <label>:164:                                    ; preds = %157
  %165 = bitcast %"class.std::__1::basic_streambuf"* %158 to i32 (%"class.std::__1::basic_streambuf"*)***
  %166 = load i32 (%"class.std::__1::basic_streambuf"*)**, i32 (%"class.std::__1::basic_streambuf"*)*** %165, align 8
  %167 = getelementptr inbounds i32 (%"class.std::__1::basic_streambuf"*)*, i32 (%"class.std::__1::basic_streambuf"*)** %166, i64 10
  %168 = load i32 (%"class.std::__1::basic_streambuf"*)*, i32 (%"class.std::__1::basic_streambuf"*)** %167, align 8
  %169 = invoke i32 %168(%"class.std::__1::basic_streambuf"* %158)
          to label %170 unwind label %186

; <label>:170:                                    ; preds = %164
  store i32 %169, i32* %29, align 4
  br label %177

; <label>:171:                                    ; preds = %157
  %172 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %158, i32 0, i32 3
  %173 = load i8*, i8** %172, align 8
  %174 = getelementptr inbounds i8, i8* %173, i32 1
  store i8* %174, i8** %172, align 8
  %175 = load i8, i8* %173, align 1
  %176 = call i32 @_ZNSt3__111char_traitsIcE11to_int_typeEc(i8 signext %175) #15
  store i32 %176, i32* %29, align 4
  br label %177

; <label>:177:                                    ; preds = %170, %171
  %178 = load i32, i32* %29, align 4
  br label %179

; <label>:179:                                    ; preds = %177
  store i32 %178, i32* %65, align 4
  %180 = load i32, i32* %65, align 4
  %181 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  %182 = call zeroext i1 @_ZNSt3__111char_traitsIcE11eq_int_typeEii(i32 %180, i32 %181) #15
  br i1 %182, label %183, label %205

; <label>:183:                                    ; preds = %179
  %184 = load i32, i32* %63, align 4
  %185 = or i32 %184, 2
  store i32 %185, i32* %63, align 4
  br label %281

; <label>:186:                                    ; preds = %287, %164, %214, %3
  %187 = landingpad { i8*, i32 }
          catch i8* null
  %188 = extractvalue { i8*, i32 } %187, 0
  store i8* %188, i8** %61, align 8
  %189 = extractvalue { i8*, i32 } %187, 1
  store i32 %189, i32* %62, align 4
  br label %190

; <label>:190:                                    ; preds = %186
  %191 = load i8*, i8** %61, align 8
  %192 = call i8* @__cxa_begin_catch(i8* %191) #15
  %193 = load %"class.std::__1::basic_istream"*, %"class.std::__1::basic_istream"** %57, align 8
  %194 = bitcast %"class.std::__1::basic_istream"* %193 to i8**
  %195 = load i8*, i8** %194, align 8
  %196 = getelementptr i8, i8* %195, i64 -24
  %197 = bitcast i8* %196 to i64*
  %198 = load i64, i64* %197, align 8
  %199 = bitcast %"class.std::__1::basic_istream"* %193 to i8*
  %200 = getelementptr inbounds i8, i8* %199, i64 %198
  %201 = bitcast i8* %200 to %"class.std::__1::ios_base"*
  invoke void @_ZNSt3__18ios_base33__set_badbit_and_consider_rethrowEv(%"class.std::__1::ios_base"* %201)
          to label %202 unwind label %309

; <label>:202:                                    ; preds = %190
  call void @__cxa_end_catch()
  br label %203

; <label>:203:                                    ; preds = %202, %308
  %204 = load %"class.std::__1::basic_istream"*, %"class.std::__1::basic_istream"** %57, align 8
  ret %"class.std::__1::basic_istream"* %204

; <label>:205:                                    ; preds = %179
  %206 = load i64, i64* %64, align 8
  %207 = add nsw i64 %206, 1
  store i64 %207, i64* %64, align 8
  %208 = load i32, i32* %65, align 4
  %209 = call signext i8 @_ZNSt3__111char_traitsIcE12to_char_typeEi(i32 %208) #15
  store i8 %209, i8* %66, align 1
  %210 = load i8, i8* %66, align 1
  %211 = load i8, i8* %59, align 1
  %212 = call zeroext i1 @_ZNSt3__111char_traitsIcE2eqEcc(i8 signext %210, i8 signext %211) #15
  br i1 %212, label %213, label %214

; <label>:213:                                    ; preds = %205
  br label %281

; <label>:214:                                    ; preds = %205
  %215 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8
  %216 = load i8, i8* %66, align 1
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE9push_backEc(%"class.std::__1::basic_string"* %215, i8 signext %216)
          to label %217 unwind label %186

; <label>:217:                                    ; preds = %214
  %218 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8
  store %"class.std::__1::basic_string"* %218, %"class.std::__1::basic_string"** %28, align 8
  %219 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %28, align 8
  store %"class.std::__1::basic_string"* %219, %"class.std::__1::basic_string"** %27, align 8
  %220 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %27, align 8
  %221 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %220, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %221, %"class.std::__1::__compressed_pair"** %26, align 8
  %222 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %26, align 8
  %223 = bitcast %"class.std::__1::__compressed_pair"* %222 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %223, %"class.std::__1::__libcpp_compressed_pair_imp"** %25, align 8
  %224 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %25, align 8
  %225 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %224, i32 0, i32 0
  %226 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %225, i32 0, i32 0
  %227 = bitcast %union.anon* %226 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %228 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %227, i32 0, i32 0
  %229 = bitcast %union.anon.0* %228 to i8*
  %230 = load i8, i8* %229, align 8
  %231 = zext i8 %230 to i32
  %232 = and i32 %231, 1
  %233 = icmp ne i32 %232, 0
  br i1 %233, label %234, label %245

; <label>:234:                                    ; preds = %217
  store %"class.std::__1::basic_string"* %219, %"class.std::__1::basic_string"** %21, align 8
  %235 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %21, align 8
  %236 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %235, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %236, %"class.std::__1::__compressed_pair"** %20, align 8
  %237 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %20, align 8
  %238 = bitcast %"class.std::__1::__compressed_pair"* %237 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %238, %"class.std::__1::__libcpp_compressed_pair_imp"** %19, align 8
  %239 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %19, align 8
  %240 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %239, i32 0, i32 0
  %241 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %240, i32 0, i32 0
  %242 = bitcast %union.anon* %241 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %243 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %242, i32 0, i32 1
  %244 = load i64, i64* %243, align 8
  br label %260

; <label>:245:                                    ; preds = %217
  store %"class.std::__1::basic_string"* %219, %"class.std::__1::basic_string"** %24, align 8
  %246 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %24, align 8
  %247 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %246, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %247, %"class.std::__1::__compressed_pair"** %23, align 8
  %248 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %23, align 8
  %249 = bitcast %"class.std::__1::__compressed_pair"* %248 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %249, %"class.std::__1::__libcpp_compressed_pair_imp"** %22, align 8
  %250 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %22, align 8
  %251 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %250, i32 0, i32 0
  %252 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %251, i32 0, i32 0
  %253 = bitcast %union.anon* %252 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %254 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %253, i32 0, i32 0
  %255 = bitcast %union.anon.0* %254 to i8*
  %256 = load i8, i8* %255, align 8
  %257 = zext i8 %256 to i32
  %258 = ashr i32 %257, 1
  %259 = sext i32 %258 to i64
  br label %260

; <label>:260:                                    ; preds = %234, %245
  %261 = phi i64 [ %244, %234 ], [ %259, %245 ]
  %262 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %58, align 8
  store %"class.std::__1::basic_string"* %262, %"class.std::__1::basic_string"** %17, align 8
  %263 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %17, align 8
  store %"class.std::__1::basic_string"* %263, %"class.std::__1::basic_string"** %16, align 8
  %264 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %16, align 8
  %265 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %264, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %265, %"class.std::__1::__compressed_pair"** %15, align 8
  %266 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %15, align 8
  %267 = bitcast %"class.std::__1::__compressed_pair"* %266 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %267, %"class.std::__1::__libcpp_compressed_pair_imp"** %14, align 8
  %268 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %14, align 8
  %269 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %268 to %"class.std::__1::allocator"*
  store %"class.std::__1::allocator"* %269, %"class.std::__1::allocator"** %11, align 8
  %270 = bitcast %"struct.std::__1::__has_max_size"* %13 to %"struct.std::__1::integral_constant"*
  %271 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %11, align 8
  store %"class.std::__1::allocator"* %271, %"class.std::__1::allocator"** %10, align 8
  %272 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %10, align 8
  store %"class.std::__1::allocator"* %272, %"class.std::__1::allocator"** %8, align 8
  %273 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %8, align 8
  store i64 -1, i64* %18, align 8
  %274 = load i64, i64* %18, align 8
  %275 = sub i64 %274, 16
  %276 = icmp eq i64 %261, %275
  br i1 %276, label %277, label %280

; <label>:277:                                    ; preds = %260
  %278 = load i32, i32* %63, align 4
  %279 = or i32 %278, 4
  store i32 %279, i32* %63, align 4
  br label %281

; <label>:280:                                    ; preds = %260
  br label %140

; <label>:281:                                    ; preds = %277, %213, %183
  %282 = load i64, i64* %64, align 8
  %283 = icmp eq i64 %282, 0
  br i1 %283, label %284, label %287

; <label>:284:                                    ; preds = %281
  %285 = load i32, i32* %63, align 4
  %286 = or i32 %285, 4
  store i32 %286, i32* %63, align 4
  br label %287

; <label>:287:                                    ; preds = %284, %281
  %288 = load %"class.std::__1::basic_istream"*, %"class.std::__1::basic_istream"** %57, align 8
  %289 = bitcast %"class.std::__1::basic_istream"* %288 to i8**
  %290 = load i8*, i8** %289, align 8
  %291 = getelementptr i8, i8* %290, i64 -24
  %292 = bitcast i8* %291 to i64*
  %293 = load i64, i64* %292, align 8
  %294 = bitcast %"class.std::__1::basic_istream"* %288 to i8*
  %295 = getelementptr inbounds i8, i8* %294, i64 %293
  %296 = bitcast i8* %295 to %"class.std::__1::basic_ios"*
  %297 = load i32, i32* %63, align 4
  store %"class.std::__1::basic_ios"* %296, %"class.std::__1::basic_ios"** %6, align 8
  store i32 %297, i32* %7, align 4
  %298 = load %"class.std::__1::basic_ios"*, %"class.std::__1::basic_ios"** %6, align 8
  %299 = bitcast %"class.std::__1::basic_ios"* %298 to %"class.std::__1::ios_base"*
  %300 = load i32, i32* %7, align 4
  store %"class.std::__1::ios_base"* %299, %"class.std::__1::ios_base"** %4, align 8
  store i32 %300, i32* %5, align 4
  %301 = load %"class.std::__1::ios_base"*, %"class.std::__1::ios_base"** %4, align 8
  %302 = getelementptr inbounds %"class.std::__1::ios_base", %"class.std::__1::ios_base"* %301, i32 0, i32 4
  %303 = load i32, i32* %302, align 8
  %304 = load i32, i32* %5, align 4
  %305 = or i32 %303, %304
  invoke void @_ZNSt3__18ios_base5clearEj(%"class.std::__1::ios_base"* %301, i32 %305)
          to label %306 unwind label %186

; <label>:306:                                    ; preds = %287
  br label %307

; <label>:307:                                    ; preds = %306
  br label %308

; <label>:308:                                    ; preds = %307, %73
  br label %203

; <label>:309:                                    ; preds = %190
  %310 = landingpad { i8*, i32 }
          cleanup
  %311 = extractvalue { i8*, i32 } %310, 0
  store i8* %311, i8** %61, align 8
  %312 = extractvalue { i8*, i32 } %310, 1
  store i32 %312, i32* %62, align 4
  invoke void @__cxa_end_catch()
          to label %313 unwind label %319

; <label>:313:                                    ; preds = %309
  br label %314

; <label>:314:                                    ; preds = %313
  %315 = load i8*, i8** %61, align 8
  %316 = load i32, i32* %62, align 4
  %317 = insertvalue { i8*, i32 } undef, i8* %315, 0
  %318 = insertvalue { i8*, i32 } %317, i32 %316, 1
  resume { i8*, i32 } %318

; <label>:319:                                    ; preds = %309
  %320 = landingpad { i8*, i32 }
          catch i8* null
  %321 = extractvalue { i8*, i32 } %320, 0
  call void @__clang_call_terminate(i8* %321) #16
  unreachable
}

declare void @_ZNSt3__113basic_istreamIcNS_11char_traitsIcEEE6sentryC1ERS3_b(%"class.std::__1::basic_istream<char, std::__1::char_traits<char> >::sentry"*, %"class.std::__1::basic_istream"* dereferenceable(168), i1 zeroext) unnamed_addr #1

declare void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE9push_backEc(%"class.std::__1::basic_string"*, i8 signext) #1

; Function Attrs: inlinehint nounwind ssp uwtable
define linkonce_odr void @_ZNSt3__111char_traitsIcE6assignERcRKc(i8* dereferenceable(1), i8* dereferenceable(1)) #5 align 2 {
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

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEEC1Ev(%"class.std::__1::basic_filebuf"*) unnamed_addr #0 align 2 {
  %2 = alloca %"class.std::__1::basic_filebuf"*, align 8
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %2, align 8
  %3 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %2, align 8
  call void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEEC2Ev(%"class.std::__1::basic_filebuf"* %3)
  ret void
}

; Function Attrs: ssp uwtable
define linkonce_odr %"class.std::__1::basic_filebuf"* @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEE4openEPKcj(%"class.std::__1::basic_filebuf"*, i8*, i32) #0 align 2 {
  %4 = alloca %"class.std::__1::basic_filebuf"*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i32, align 4
  %7 = alloca %"class.std::__1::basic_filebuf"*, align 8
  %8 = alloca i8*, align 8
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %4, align 8
  store i8* %1, i8** %5, align 8
  store i32 %2, i32* %6, align 4
  %9 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %4, align 8
  store %"class.std::__1::basic_filebuf"* null, %"class.std::__1::basic_filebuf"** %7, align 8
  %10 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %9, i32 0, i32 8
  %11 = load %struct.__sFILE*, %struct.__sFILE** %10, align 8
  %12 = icmp eq %struct.__sFILE* %11, null
  br i1 %12, label %13, label %61

; <label>:13:                                     ; preds = %3
  store %"class.std::__1::basic_filebuf"* %9, %"class.std::__1::basic_filebuf"** %7, align 8
  %14 = load i32, i32* %6, align 4
  %15 = and i32 %14, -3
  switch i32 %15, label %28 [
    i32 16, label %16
    i32 48, label %16
    i32 17, label %17
    i32 1, label %17
    i32 8, label %18
    i32 24, label %19
    i32 56, label %20
    i32 25, label %21
    i32 9, label %21
    i32 20, label %22
    i32 52, label %22
    i32 21, label %23
    i32 5, label %23
    i32 12, label %24
    i32 28, label %25
    i32 60, label %26
    i32 29, label %27
    i32 13, label %27
  ]

; <label>:16:                                     ; preds = %13, %13
  store i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8** %8, align 8
  br label %29

; <label>:17:                                     ; preds = %13, %13
  store i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.3, i32 0, i32 0), i8** %8, align 8
  br label %29

; <label>:18:                                     ; preds = %13
  store i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.4, i32 0, i32 0), i8** %8, align 8
  br label %29

; <label>:19:                                     ; preds = %13
  store i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.5, i32 0, i32 0), i8** %8, align 8
  br label %29

; <label>:20:                                     ; preds = %13
  store i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.6, i32 0, i32 0), i8** %8, align 8
  br label %29

; <label>:21:                                     ; preds = %13, %13
  store i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.7, i32 0, i32 0), i8** %8, align 8
  br label %29

; <label>:22:                                     ; preds = %13, %13
  store i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.8, i32 0, i32 0), i8** %8, align 8
  br label %29

; <label>:23:                                     ; preds = %13, %13
  store i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.9, i32 0, i32 0), i8** %8, align 8
  br label %29

; <label>:24:                                     ; preds = %13
  store i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.10, i32 0, i32 0), i8** %8, align 8
  br label %29

; <label>:25:                                     ; preds = %13
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.11, i32 0, i32 0), i8** %8, align 8
  br label %29

; <label>:26:                                     ; preds = %13
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.12, i32 0, i32 0), i8** %8, align 8
  br label %29

; <label>:27:                                     ; preds = %13, %13
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.13, i32 0, i32 0), i8** %8, align 8
  br label %29

; <label>:28:                                     ; preds = %13
  store %"class.std::__1::basic_filebuf"* null, %"class.std::__1::basic_filebuf"** %7, align 8
  br label %29

; <label>:29:                                     ; preds = %28, %27, %26, %25, %24, %23, %22, %21, %20, %19, %18, %17, %16
  %30 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %7, align 8
  %31 = icmp ne %"class.std::__1::basic_filebuf"* %30, null
  br i1 %31, label %32, label %60

; <label>:32:                                     ; preds = %29
  %33 = load i8*, i8** %5, align 8
  %34 = load i8*, i8** %8, align 8
  %35 = call %struct.__sFILE* @"\01_fopen"(i8* %33, i8* %34)
  %36 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %9, i32 0, i32 8
  store %struct.__sFILE* %35, %struct.__sFILE** %36, align 8
  %37 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %9, i32 0, i32 8
  %38 = load %struct.__sFILE*, %struct.__sFILE** %37, align 8
  %39 = icmp ne %struct.__sFILE* %38, null
  br i1 %39, label %40, label %58

; <label>:40:                                     ; preds = %32
  %41 = load i32, i32* %6, align 4
  %42 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %9, i32 0, i32 12
  store i32 %41, i32* %42, align 8
  %43 = load i32, i32* %6, align 4
  %44 = and i32 %43, 2
  %45 = icmp ne i32 %44, 0
  br i1 %45, label %46, label %57

; <label>:46:                                     ; preds = %40
  %47 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %9, i32 0, i32 8
  %48 = load %struct.__sFILE*, %struct.__sFILE** %47, align 8
  %49 = call i32 @fseek(%struct.__sFILE* %48, i64 0, i32 2)
  %50 = icmp ne i32 %49, 0
  br i1 %50, label %51, label %56

; <label>:51:                                     ; preds = %46
  %52 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %9, i32 0, i32 8
  %53 = load %struct.__sFILE*, %struct.__sFILE** %52, align 8
  %54 = call i32 @fclose(%struct.__sFILE* %53)
  %55 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %9, i32 0, i32 8
  store %struct.__sFILE* null, %struct.__sFILE** %55, align 8
  store %"class.std::__1::basic_filebuf"* null, %"class.std::__1::basic_filebuf"** %7, align 8
  br label %56

; <label>:56:                                     ; preds = %51, %46
  br label %57

; <label>:57:                                     ; preds = %56, %40
  br label %59

; <label>:58:                                     ; preds = %32
  store %"class.std::__1::basic_filebuf"* null, %"class.std::__1::basic_filebuf"** %7, align 8
  br label %59

; <label>:59:                                     ; preds = %58, %57
  br label %60

; <label>:60:                                     ; preds = %59, %29
  br label %61

; <label>:61:                                     ; preds = %60, %3
  %62 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %7, align 8
  ret %"class.std::__1::basic_filebuf"* %62
}

declare void @_ZNSt3__18ios_base4initEPv(%"class.std::__1::ios_base"*, i8*) #1

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__113basic_filebufIcNS_11char_traitsIcEEEC2Ev(%"class.std::__1::basic_filebuf"*) unnamed_addr #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %2 = alloca %"class.std::__1::codecvt"*, align 8
  %3 = alloca %"class.std::__1::locale"*, align 8
  %4 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %5 = alloca %"class.std::__1::locale"*, align 8
  %6 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %7 = alloca %"class.std::__1::basic_filebuf"*, align 8
  %8 = alloca %"class.std::__1::locale", align 8
  %9 = alloca i8*
  %10 = alloca i32
  %11 = alloca %"class.std::__1::locale", align 8
  store %"class.std::__1::basic_filebuf"* %0, %"class.std::__1::basic_filebuf"** %7, align 8
  %12 = load %"class.std::__1::basic_filebuf"*, %"class.std::__1::basic_filebuf"** %7, align 8
  %13 = bitcast %"class.std::__1::basic_filebuf"* %12 to %"class.std::__1::basic_streambuf"*
  call void @_ZNSt3__115basic_streambufIcNS_11char_traitsIcEEEC2Ev(%"class.std::__1::basic_streambuf"* %13)
  %14 = bitcast %"class.std::__1::basic_filebuf"* %12 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ([16 x i8*], [16 x i8*]* @_ZTVNSt3__113basic_filebufIcNS_11char_traitsIcEEEE, i32 0, i32 2) to i32 (...)**), i32 (...)*** %14, align 8
  %15 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 1
  store i8* null, i8** %15, align 8
  %16 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 2
  store i8* null, i8** %16, align 8
  %17 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 3
  store i8* null, i8** %17, align 8
  %18 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 5
  store i64 0, i64* %18, align 8
  %19 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 6
  store i8* null, i8** %19, align 8
  %20 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 7
  store i64 0, i64* %20, align 8
  %21 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 8
  store %struct.__sFILE* null, %struct.__sFILE** %21, align 8
  %22 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 9
  store %"class.std::__1::codecvt"* null, %"class.std::__1::codecvt"** %22, align 8
  %23 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 10
  %24 = bitcast %union.__mbstate_t* %23 to i8*
  call void @llvm.memset.p0i8.i64(i8* %24, i8 0, i64 128, i32 8, i1 false)
  %25 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 11
  %26 = bitcast %union.__mbstate_t* %25 to i8*
  call void @llvm.memset.p0i8.i64(i8* %26, i8 0, i64 128, i32 8, i1 false)
  %27 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 12
  store i32 0, i32* %27, align 8
  %28 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 13
  store i32 0, i32* %28, align 4
  %29 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 14
  store i8 0, i8* %29, align 8
  %30 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 15
  store i8 0, i8* %30, align 1
  %31 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 16
  store i8 0, i8* %31, align 2
  %32 = bitcast %"class.std::__1::basic_filebuf"* %12 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %32, %"class.std::__1::basic_streambuf"** %6, align 8, !noalias !5
  %33 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %6, align 8, !noalias !5
  %34 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %33, i32 0, i32 1
  call void @_ZNSt3__16localeC1ERKS0_(%"class.std::__1::locale"* %8, %"class.std::__1::locale"* dereferenceable(8) %34) #15
  br label %35

; <label>:35:                                     ; preds = %1
  store %"class.std::__1::locale"* %8, %"class.std::__1::locale"** %5, align 8
  %36 = load %"class.std::__1::locale"*, %"class.std::__1::locale"** %5, align 8
  %37 = invoke zeroext i1 @_ZNKSt3__16locale9has_facetERNS0_2idE(%"class.std::__1::locale"* %36, %"class.std::__1::locale::id"* dereferenceable(16) @_ZNSt3__17codecvtIcc11__mbstate_tE2idE)
          to label %41 unwind label %38

; <label>:38:                                     ; preds = %35
  %39 = landingpad { i8*, i32 }
          catch i8* null
  %40 = extractvalue { i8*, i32 } %39, 0
  call void @__clang_call_terminate(i8* %40) #16
  unreachable

; <label>:41:                                     ; preds = %35
  call void @_ZNSt3__16localeD1Ev(%"class.std::__1::locale"* %8) #15
  br i1 %37, label %42, label %71

; <label>:42:                                     ; preds = %41
  %43 = bitcast %"class.std::__1::basic_filebuf"* %12 to %"class.std::__1::basic_streambuf"*
  store %"class.std::__1::basic_streambuf"* %43, %"class.std::__1::basic_streambuf"** %4, align 8, !noalias !8
  %44 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %4, align 8, !noalias !8
  %45 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %44, i32 0, i32 1
  call void @_ZNSt3__16localeC1ERKS0_(%"class.std::__1::locale"* %11, %"class.std::__1::locale"* dereferenceable(8) %45) #15
  br label %46

; <label>:46:                                     ; preds = %42
  store %"class.std::__1::locale"* %11, %"class.std::__1::locale"** %3, align 8
  %47 = load %"class.std::__1::locale"*, %"class.std::__1::locale"** %3, align 8
  %48 = invoke %"class.std::__1::locale::facet"* @_ZNKSt3__16locale9use_facetERNS0_2idE(%"class.std::__1::locale"* %47, %"class.std::__1::locale::id"* dereferenceable(16) @_ZNSt3__17codecvtIcc11__mbstate_tE2idE)
          to label %49 unwind label %67

; <label>:49:                                     ; preds = %46
  %50 = bitcast %"class.std::__1::locale::facet"* %48 to %"class.std::__1::codecvt"*
  br label %51

; <label>:51:                                     ; preds = %49
  %52 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 9
  store %"class.std::__1::codecvt"* %50, %"class.std::__1::codecvt"** %52, align 8
  call void @_ZNSt3__16localeD1Ev(%"class.std::__1::locale"* %11) #15
  %53 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 9
  %54 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %53, align 8
  store %"class.std::__1::codecvt"* %54, %"class.std::__1::codecvt"** %2, align 8
  %55 = load %"class.std::__1::codecvt"*, %"class.std::__1::codecvt"** %2, align 8
  %56 = bitcast %"class.std::__1::codecvt"* %55 to i1 (%"class.std::__1::codecvt"*)***
  %57 = load i1 (%"class.std::__1::codecvt"*)**, i1 (%"class.std::__1::codecvt"*)*** %56, align 8
  %58 = getelementptr inbounds i1 (%"class.std::__1::codecvt"*)*, i1 (%"class.std::__1::codecvt"*)** %57, i64 7
  %59 = load i1 (%"class.std::__1::codecvt"*)*, i1 (%"class.std::__1::codecvt"*)** %58, align 8
  %60 = call zeroext i1 %59(%"class.std::__1::codecvt"* %55) #15
  %61 = getelementptr inbounds %"class.std::__1::basic_filebuf", %"class.std::__1::basic_filebuf"* %12, i32 0, i32 16
  %62 = zext i1 %60 to i8
  store i8 %62, i8* %61, align 2
  br label %71

; <label>:63:                                     ; preds = %71
  %64 = landingpad { i8*, i32 }
          cleanup
  %65 = extractvalue { i8*, i32 } %64, 0
  store i8* %65, i8** %9, align 8
  %66 = extractvalue { i8*, i32 } %64, 1
  store i32 %66, i32* %10, align 4
  br label %78

; <label>:67:                                     ; preds = %46
  %68 = landingpad { i8*, i32 }
          cleanup
  %69 = extractvalue { i8*, i32 } %68, 0
  store i8* %69, i8** %9, align 8
  %70 = extractvalue { i8*, i32 } %68, 1
  store i32 %70, i32* %10, align 4
  call void @_ZNSt3__16localeD1Ev(%"class.std::__1::locale"* %11) #15
  br label %78

; <label>:71:                                     ; preds = %51, %41
  %72 = bitcast %"class.std::__1::basic_filebuf"* %12 to %"class.std::__1::basic_streambuf"* (%"class.std::__1::basic_filebuf"*, i8*, i64)***
  %73 = load %"class.std::__1::basic_streambuf"* (%"class.std::__1::basic_filebuf"*, i8*, i64)**, %"class.std::__1::basic_streambuf"* (%"class.std::__1::basic_filebuf"*, i8*, i64)*** %72, align 8
  %74 = getelementptr inbounds %"class.std::__1::basic_streambuf"* (%"class.std::__1::basic_filebuf"*, i8*, i64)*, %"class.std::__1::basic_streambuf"* (%"class.std::__1::basic_filebuf"*, i8*, i64)** %73, i64 3
  %75 = load %"class.std::__1::basic_streambuf"* (%"class.std::__1::basic_filebuf"*, i8*, i64)*, %"class.std::__1::basic_streambuf"* (%"class.std::__1::basic_filebuf"*, i8*, i64)** %74, align 8
  %76 = invoke %"class.std::__1::basic_streambuf"* %75(%"class.std::__1::basic_filebuf"* %12, i8* null, i64 4096)
          to label %77 unwind label %63

; <label>:77:                                     ; preds = %71
  ret void

; <label>:78:                                     ; preds = %67, %63
  %79 = bitcast %"class.std::__1::basic_filebuf"* %12 to %"class.std::__1::basic_streambuf"*
  call void @_ZNSt3__115basic_streambufIcNS_11char_traitsIcEEED2Ev(%"class.std::__1::basic_streambuf"* %79) #15
  br label %80

; <label>:80:                                     ; preds = %78
  %81 = load i8*, i8** %9, align 8
  %82 = load i32, i32* %10, align 4
  %83 = insertvalue { i8*, i32 } undef, i8* %81, 0
  %84 = insertvalue { i8*, i32 } %83, i32 %82, 1
  resume { i8*, i32 } %84
}

declare void @_ZNSt3__115basic_streambufIcNS_11char_traitsIcEEEC2Ev(%"class.std::__1::basic_streambuf"*) unnamed_addr #1

declare zeroext i1 @_ZNKSt3__16locale9has_facetERNS0_2idE(%"class.std::__1::locale"*, %"class.std::__1::locale::id"* dereferenceable(16)) #1

; Function Attrs: nounwind
declare void @_ZNSt3__16localeC1ERKS0_(%"class.std::__1::locale"*, %"class.std::__1::locale"* dereferenceable(8)) unnamed_addr #3

declare %struct.__sFILE* @"\01_fopen"(i8*, i8*) #1

declare i32 @fseek(%struct.__sFILE*, i64, i32) #1

; Function Attrs: ssp uwtable
define linkonce_odr void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6__initINS_19istreambuf_iteratorIcS2_EEEENS_9enable_ifIXaasr19__is_input_iteratorIT_EE5valuentsr21__is_forward_iteratorISA_EE5valueEvE4typeESA_SA_(%"class.std::__1::basic_string"*, %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"*) #0 align 2 personality i8* bitcast (i32 (...)* @__gxx_personality_v0 to i8*) {
  %4 = alloca i8*, align 8
  %5 = alloca %"class.std::__1::allocator"*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca i64, align 8
  %8 = alloca %"class.std::__1::allocator"*, align 8
  %9 = alloca i8*, align 8
  %10 = alloca i64, align 8
  %11 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %12 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %13 = alloca %"class.std::__1::basic_string"*, align 8
  %14 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %15 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %16 = alloca %"class.std::__1::basic_string"*, align 8
  %17 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %18 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %19 = alloca %"class.std::__1::basic_string"*, align 8
  %20 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %21 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %22 = alloca %"class.std::__1::basic_string"*, align 8
  %23 = alloca i32, align 4
  %24 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %25 = alloca %"class.std::__1::istreambuf_iterator"*, align 8
  %26 = alloca i32, align 4
  %27 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %28 = alloca %"class.std::__1::istreambuf_iterator"*, align 8
  %29 = alloca i32, align 4
  %30 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %31 = alloca %"class.std::__1::istreambuf_iterator"*, align 8
  %32 = alloca i32, align 4
  %33 = alloca %"class.std::__1::basic_streambuf"*, align 8
  %34 = alloca %"class.std::__1::istreambuf_iterator"*, align 8
  %35 = alloca %"class.std::__1::istreambuf_iterator"*, align 8
  %36 = alloca %"class.std::__1::istreambuf_iterator"*, align 8
  %37 = alloca %"class.std::__1::istreambuf_iterator"*, align 8
  %38 = alloca %"class.std::__1::istreambuf_iterator"*, align 8
  %39 = alloca %"class.std::__1::__libcpp_compressed_pair_imp"*, align 8
  %40 = alloca %"class.std::__1::__compressed_pair"*, align 8
  %41 = alloca %"class.std::__1::basic_string"*, align 8
  %42 = alloca [3 x i64]*, align 8
  %43 = alloca i32, align 4
  %44 = alloca %"class.std::__1::istreambuf_iterator", align 8
  %45 = alloca %"class.std::__1::istreambuf_iterator", align 8
  %46 = alloca %"class.std::__1::basic_string"*, align 8
  %47 = alloca i8*
  %48 = alloca i32
  %49 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %44, i32 0, i32 0
  store %"class.std::__1::basic_streambuf"* %1, %"class.std::__1::basic_streambuf"** %49, align 8
  %50 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %45, i32 0, i32 0
  store %"class.std::__1::basic_streambuf"* %2, %"class.std::__1::basic_streambuf"** %50, align 8
  store %"class.std::__1::basic_string"* %0, %"class.std::__1::basic_string"** %46, align 8
  %51 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %46, align 8
  store %"class.std::__1::basic_string"* %51, %"class.std::__1::basic_string"** %41, align 8
  %52 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %41, align 8
  %53 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %52, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %53, %"class.std::__1::__compressed_pair"** %40, align 8
  %54 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %40, align 8
  %55 = bitcast %"class.std::__1::__compressed_pair"* %54 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %55, %"class.std::__1::__libcpp_compressed_pair_imp"** %39, align 8
  %56 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %39, align 8
  %57 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %56, i32 0, i32 0
  %58 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %57, i32 0, i32 0
  %59 = bitcast %union.anon* %58 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"*
  %60 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__raw"* %59, i32 0, i32 0
  store [3 x i64]* %60, [3 x i64]** %42, align 8
  store i32 0, i32* %43, align 4
  br label %61

; <label>:61:                                     ; preds = %64, %3
  %62 = load i32, i32* %43, align 4
  %63 = icmp ult i32 %62, 3
  br i1 %63, label %64, label %71

; <label>:64:                                     ; preds = %61
  %65 = load i32, i32* %43, align 4
  %66 = zext i32 %65 to i64
  %67 = load [3 x i64]*, [3 x i64]** %42, align 8
  %68 = getelementptr inbounds [3 x i64], [3 x i64]* %67, i64 0, i64 %66
  store i64 0, i64* %68, align 8
  %69 = load i32, i32* %43, align 4
  %70 = add i32 %69, 1
  store i32 %70, i32* %43, align 4
  br label %61

; <label>:71:                                     ; preds = %61
  br label %72

; <label>:72:                                     ; preds = %204, %71
  store %"class.std::__1::istreambuf_iterator"* %44, %"class.std::__1::istreambuf_iterator"** %37, align 8
  store %"class.std::__1::istreambuf_iterator"* %45, %"class.std::__1::istreambuf_iterator"** %38, align 8
  %73 = load %"class.std::__1::istreambuf_iterator"*, %"class.std::__1::istreambuf_iterator"** %37, align 8
  %74 = load %"class.std::__1::istreambuf_iterator"*, %"class.std::__1::istreambuf_iterator"** %38, align 8
  store %"class.std::__1::istreambuf_iterator"* %73, %"class.std::__1::istreambuf_iterator"** %35, align 8
  store %"class.std::__1::istreambuf_iterator"* %74, %"class.std::__1::istreambuf_iterator"** %36, align 8
  %75 = load %"class.std::__1::istreambuf_iterator"*, %"class.std::__1::istreambuf_iterator"** %35, align 8
  store %"class.std::__1::istreambuf_iterator"* %75, %"class.std::__1::istreambuf_iterator"** %34, align 8
  %76 = load %"class.std::__1::istreambuf_iterator"*, %"class.std::__1::istreambuf_iterator"** %34, align 8
  %77 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %76, i32 0, i32 0
  %78 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %77, align 8
  %79 = icmp ne %"class.std::__1::basic_streambuf"* %78, null
  br i1 %79, label %80, label %107

; <label>:80:                                     ; preds = %72
  %81 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %76, i32 0, i32 0
  %82 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %81, align 8
  store %"class.std::__1::basic_streambuf"* %82, %"class.std::__1::basic_streambuf"** %33, align 8
  %83 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %33, align 8
  %84 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %83, i32 0, i32 3
  %85 = load i8*, i8** %84, align 8
  %86 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %83, i32 0, i32 4
  %87 = load i8*, i8** %86, align 8
  %88 = icmp eq i8* %85, %87
  br i1 %88, label %89, label %96

; <label>:89:                                     ; preds = %80
  %90 = bitcast %"class.std::__1::basic_streambuf"* %83 to i32 (%"class.std::__1::basic_streambuf"*)***
  %91 = load i32 (%"class.std::__1::basic_streambuf"*)**, i32 (%"class.std::__1::basic_streambuf"*)*** %90, align 8
  %92 = getelementptr inbounds i32 (%"class.std::__1::basic_streambuf"*)*, i32 (%"class.std::__1::basic_streambuf"*)** %91, i64 9
  %93 = load i32 (%"class.std::__1::basic_streambuf"*)*, i32 (%"class.std::__1::basic_streambuf"*)** %92, align 8
  %94 = invoke i32 %93(%"class.std::__1::basic_streambuf"* %83)
          to label %95 unwind label %205

; <label>:95:                                     ; preds = %89
  store i32 %94, i32* %32, align 4
  br label %101

; <label>:96:                                     ; preds = %80
  %97 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %83, i32 0, i32 3
  %98 = load i8*, i8** %97, align 8
  %99 = load i8, i8* %98, align 1
  %100 = call i32 @_ZNSt3__111char_traitsIcE11to_int_typeEc(i8 signext %99) #15
  store i32 %100, i32* %32, align 4
  br label %101

; <label>:101:                                    ; preds = %96, %95
  %102 = load i32, i32* %32, align 4
  %103 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  %104 = call zeroext i1 @_ZNSt3__111char_traitsIcE11eq_int_typeEii(i32 %102, i32 %103) #15
  br i1 %104, label %105, label %107

; <label>:105:                                    ; preds = %101
  %106 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %76, i32 0, i32 0
  store %"class.std::__1::basic_streambuf"* null, %"class.std::__1::basic_streambuf"** %106, align 8
  br label %107

; <label>:107:                                    ; preds = %105, %101, %72
  %108 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %76, i32 0, i32 0
  %109 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %108, align 8
  %110 = icmp eq %"class.std::__1::basic_streambuf"* %109, null
  %111 = zext i1 %110 to i32
  %112 = load %"class.std::__1::istreambuf_iterator"*, %"class.std::__1::istreambuf_iterator"** %36, align 8
  store %"class.std::__1::istreambuf_iterator"* %112, %"class.std::__1::istreambuf_iterator"** %31, align 8
  %113 = load %"class.std::__1::istreambuf_iterator"*, %"class.std::__1::istreambuf_iterator"** %31, align 8
  %114 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %113, i32 0, i32 0
  %115 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %114, align 8
  %116 = icmp ne %"class.std::__1::basic_streambuf"* %115, null
  br i1 %116, label %117, label %144

; <label>:117:                                    ; preds = %107
  %118 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %113, i32 0, i32 0
  %119 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %118, align 8
  store %"class.std::__1::basic_streambuf"* %119, %"class.std::__1::basic_streambuf"** %30, align 8
  %120 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %30, align 8
  %121 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %120, i32 0, i32 3
  %122 = load i8*, i8** %121, align 8
  %123 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %120, i32 0, i32 4
  %124 = load i8*, i8** %123, align 8
  %125 = icmp eq i8* %122, %124
  br i1 %125, label %126, label %133

; <label>:126:                                    ; preds = %117
  %127 = bitcast %"class.std::__1::basic_streambuf"* %120 to i32 (%"class.std::__1::basic_streambuf"*)***
  %128 = load i32 (%"class.std::__1::basic_streambuf"*)**, i32 (%"class.std::__1::basic_streambuf"*)*** %127, align 8
  %129 = getelementptr inbounds i32 (%"class.std::__1::basic_streambuf"*)*, i32 (%"class.std::__1::basic_streambuf"*)** %128, i64 9
  %130 = load i32 (%"class.std::__1::basic_streambuf"*)*, i32 (%"class.std::__1::basic_streambuf"*)** %129, align 8
  %131 = invoke i32 %130(%"class.std::__1::basic_streambuf"* %120)
          to label %132 unwind label %205

; <label>:132:                                    ; preds = %126
  store i32 %131, i32* %29, align 4
  br label %138

; <label>:133:                                    ; preds = %117
  %134 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %120, i32 0, i32 3
  %135 = load i8*, i8** %134, align 8
  %136 = load i8, i8* %135, align 1
  %137 = call i32 @_ZNSt3__111char_traitsIcE11to_int_typeEc(i8 signext %136) #15
  store i32 %137, i32* %29, align 4
  br label %138

; <label>:138:                                    ; preds = %133, %132
  %139 = load i32, i32* %29, align 4
  %140 = call i32 @_ZNSt3__111char_traitsIcE3eofEv() #15
  %141 = call zeroext i1 @_ZNSt3__111char_traitsIcE11eq_int_typeEii(i32 %139, i32 %140) #15
  br i1 %141, label %142, label %144

; <label>:142:                                    ; preds = %138
  %143 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %113, i32 0, i32 0
  store %"class.std::__1::basic_streambuf"* null, %"class.std::__1::basic_streambuf"** %143, align 8
  br label %144

; <label>:144:                                    ; preds = %107, %138, %142
  %145 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %113, i32 0, i32 0
  %146 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %145, align 8
  %147 = icmp eq %"class.std::__1::basic_streambuf"* %146, null
  %148 = zext i1 %147 to i32
  %149 = icmp eq i32 %111, %148
  %150 = xor i1 %149, true
  br label %151

; <label>:151:                                    ; preds = %144
  br i1 %150, label %152, label %260

; <label>:152:                                    ; preds = %151
  store %"class.std::__1::istreambuf_iterator"* %44, %"class.std::__1::istreambuf_iterator"** %28, align 8
  %153 = load %"class.std::__1::istreambuf_iterator"*, %"class.std::__1::istreambuf_iterator"** %28, align 8
  %154 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %153, i32 0, i32 0
  %155 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %154, align 8
  store %"class.std::__1::basic_streambuf"* %155, %"class.std::__1::basic_streambuf"** %27, align 8
  %156 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %27, align 8
  %157 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %156, i32 0, i32 3
  %158 = load i8*, i8** %157, align 8
  %159 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %156, i32 0, i32 4
  %160 = load i8*, i8** %159, align 8
  %161 = icmp eq i8* %158, %160
  br i1 %161, label %162, label %169

; <label>:162:                                    ; preds = %152
  %163 = bitcast %"class.std::__1::basic_streambuf"* %156 to i32 (%"class.std::__1::basic_streambuf"*)***
  %164 = load i32 (%"class.std::__1::basic_streambuf"*)**, i32 (%"class.std::__1::basic_streambuf"*)*** %163, align 8
  %165 = getelementptr inbounds i32 (%"class.std::__1::basic_streambuf"*)*, i32 (%"class.std::__1::basic_streambuf"*)** %164, i64 9
  %166 = load i32 (%"class.std::__1::basic_streambuf"*)*, i32 (%"class.std::__1::basic_streambuf"*)** %165, align 8
  %167 = invoke i32 %166(%"class.std::__1::basic_streambuf"* %156)
          to label %168 unwind label %205

; <label>:168:                                    ; preds = %162
  store i32 %167, i32* %26, align 4
  br label %174

; <label>:169:                                    ; preds = %152
  %170 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %156, i32 0, i32 3
  %171 = load i8*, i8** %170, align 8
  %172 = load i8, i8* %171, align 1
  %173 = call i32 @_ZNSt3__111char_traitsIcE11to_int_typeEc(i8 signext %172) #15
  store i32 %173, i32* %26, align 4
  br label %174

; <label>:174:                                    ; preds = %168, %169
  %175 = load i32, i32* %26, align 4
  %176 = trunc i32 %175 to i8
  br label %177

; <label>:177:                                    ; preds = %174
  invoke void @_ZNSt3__112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE9push_backEc(%"class.std::__1::basic_string"* %51, i8 signext %176)
          to label %178 unwind label %205

; <label>:178:                                    ; preds = %177
  br label %179

; <label>:179:                                    ; preds = %178
  store %"class.std::__1::istreambuf_iterator"* %44, %"class.std::__1::istreambuf_iterator"** %25, align 8
  %180 = load %"class.std::__1::istreambuf_iterator"*, %"class.std::__1::istreambuf_iterator"** %25, align 8
  %181 = getelementptr inbounds %"class.std::__1::istreambuf_iterator", %"class.std::__1::istreambuf_iterator"* %180, i32 0, i32 0
  %182 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %181, align 8
  store %"class.std::__1::basic_streambuf"* %182, %"class.std::__1::basic_streambuf"** %24, align 8
  %183 = load %"class.std::__1::basic_streambuf"*, %"class.std::__1::basic_streambuf"** %24, align 8
  %184 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %183, i32 0, i32 3
  %185 = load i8*, i8** %184, align 8
  %186 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %183, i32 0, i32 4
  %187 = load i8*, i8** %186, align 8
  %188 = icmp eq i8* %185, %187
  br i1 %188, label %189, label %196

; <label>:189:                                    ; preds = %179
  %190 = bitcast %"class.std::__1::basic_streambuf"* %183 to i32 (%"class.std::__1::basic_streambuf"*)***
  %191 = load i32 (%"class.std::__1::basic_streambuf"*)**, i32 (%"class.std::__1::basic_streambuf"*)*** %190, align 8
  %192 = getelementptr inbounds i32 (%"class.std::__1::basic_streambuf"*)*, i32 (%"class.std::__1::basic_streambuf"*)** %191, i64 10
  %193 = load i32 (%"class.std::__1::basic_streambuf"*)*, i32 (%"class.std::__1::basic_streambuf"*)** %192, align 8
  %194 = invoke i32 %193(%"class.std::__1::basic_streambuf"* %183)
          to label %195 unwind label %205

; <label>:195:                                    ; preds = %189
  store i32 %194, i32* %23, align 4
  br label %202

; <label>:196:                                    ; preds = %179
  %197 = getelementptr inbounds %"class.std::__1::basic_streambuf", %"class.std::__1::basic_streambuf"* %183, i32 0, i32 3
  %198 = load i8*, i8** %197, align 8
  %199 = getelementptr inbounds i8, i8* %198, i32 1
  store i8* %199, i8** %197, align 8
  %200 = load i8, i8* %198, align 1
  %201 = call i32 @_ZNSt3__111char_traitsIcE11to_int_typeEc(i8 signext %200) #15
  store i32 %201, i32* %23, align 4
  br label %202

; <label>:202:                                    ; preds = %195, %196
  %203 = load i32, i32* %23, align 4
  br label %204

; <label>:204:                                    ; preds = %202
  br label %72

; <label>:205:                                    ; preds = %189, %162, %126, %89, %177
  %206 = landingpad { i8*, i32 }
          catch i8* null
  %207 = extractvalue { i8*, i32 } %206, 0
  store i8* %207, i8** %47, align 8
  %208 = extractvalue { i8*, i32 } %206, 1
  store i32 %208, i32* %48, align 4
  br label %209

; <label>:209:                                    ; preds = %205
  %210 = load i8*, i8** %47, align 8
  %211 = call i8* @__cxa_begin_catch(i8* %210) #15
  store %"class.std::__1::basic_string"* %51, %"class.std::__1::basic_string"** %22, align 8
  %212 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %22, align 8
  %213 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %212, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %213, %"class.std::__1::__compressed_pair"** %21, align 8
  %214 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %21, align 8
  %215 = bitcast %"class.std::__1::__compressed_pair"* %214 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %215, %"class.std::__1::__libcpp_compressed_pair_imp"** %20, align 8
  %216 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %20, align 8
  %217 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %216, i32 0, i32 0
  %218 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %217, i32 0, i32 0
  %219 = bitcast %union.anon* %218 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"*
  %220 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__short"* %219, i32 0, i32 0
  %221 = bitcast %union.anon.0* %220 to i8*
  %222 = load i8, i8* %221, align 8
  %223 = zext i8 %222 to i32
  %224 = and i32 %223, 1
  %225 = icmp ne i32 %224, 0
  br i1 %225, label %226, label %261

; <label>:226:                                    ; preds = %209
  store %"class.std::__1::basic_string"* %51, %"class.std::__1::basic_string"** %19, align 8
  %227 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %19, align 8
  %228 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %227, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %228, %"class.std::__1::__compressed_pair"** %18, align 8
  %229 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %18, align 8
  %230 = bitcast %"class.std::__1::__compressed_pair"* %229 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %230, %"class.std::__1::__libcpp_compressed_pair_imp"** %17, align 8
  %231 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %17, align 8
  %232 = bitcast %"class.std::__1::__libcpp_compressed_pair_imp"* %231 to %"class.std::__1::allocator"*
  store %"class.std::__1::basic_string"* %51, %"class.std::__1::basic_string"** %16, align 8
  %233 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %16, align 8
  %234 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %233, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %234, %"class.std::__1::__compressed_pair"** %15, align 8
  %235 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %15, align 8
  %236 = bitcast %"class.std::__1::__compressed_pair"* %235 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %236, %"class.std::__1::__libcpp_compressed_pair_imp"** %14, align 8
  %237 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %14, align 8
  %238 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %237, i32 0, i32 0
  %239 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %238, i32 0, i32 0
  %240 = bitcast %union.anon* %239 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %241 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %240, i32 0, i32 2
  %242 = load i8*, i8** %241, align 8
  store %"class.std::__1::basic_string"* %51, %"class.std::__1::basic_string"** %13, align 8
  %243 = load %"class.std::__1::basic_string"*, %"class.std::__1::basic_string"** %13, align 8
  %244 = getelementptr inbounds %"class.std::__1::basic_string", %"class.std::__1::basic_string"* %243, i32 0, i32 0
  store %"class.std::__1::__compressed_pair"* %244, %"class.std::__1::__compressed_pair"** %12, align 8
  %245 = load %"class.std::__1::__compressed_pair"*, %"class.std::__1::__compressed_pair"** %12, align 8
  %246 = bitcast %"class.std::__1::__compressed_pair"* %245 to %"class.std::__1::__libcpp_compressed_pair_imp"*
  store %"class.std::__1::__libcpp_compressed_pair_imp"* %246, %"class.std::__1::__libcpp_compressed_pair_imp"** %11, align 8
  %247 = load %"class.std::__1::__libcpp_compressed_pair_imp"*, %"class.std::__1::__libcpp_compressed_pair_imp"** %11, align 8
  %248 = getelementptr inbounds %"class.std::__1::__libcpp_compressed_pair_imp", %"class.std::__1::__libcpp_compressed_pair_imp"* %247, i32 0, i32 0
  %249 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__rep"* %248, i32 0, i32 0
  %250 = bitcast %union.anon* %249 to %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"*
  %251 = getelementptr inbounds %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long", %"struct.std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >::__long"* %250, i32 0, i32 0
  %252 = load i64, i64* %251, align 8
  %253 = and i64 %252, -2
  store %"class.std::__1::allocator"* %232, %"class.std::__1::allocator"** %8, align 8
  store i8* %242, i8** %9, align 8
  store i64 %253, i64* %10, align 8
  %254 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %8, align 8
  %255 = load i8*, i8** %9, align 8
  %256 = load i64, i64* %10, align 8
  store %"class.std::__1::allocator"* %254, %"class.std::__1::allocator"** %5, align 8
  store i8* %255, i8** %6, align 8
  store i64 %256, i64* %7, align 8
  %257 = load %"class.std::__1::allocator"*, %"class.std::__1::allocator"** %5, align 8
  %258 = load i8*, i8** %6, align 8
  store i8* %258, i8** %4, align 8
  %259 = load i8*, i8** %4, align 8
  call void @_ZdlPv(i8* %259) #17
  br label %261

; <label>:260:                                    ; preds = %151
  br label %267

; <label>:261:                                    ; preds = %226, %209
  invoke void @__cxa_rethrow() #14
          to label %276 unwind label %262

; <label>:262:                                    ; preds = %261
  %263 = landingpad { i8*, i32 }
          cleanup
  %264 = extractvalue { i8*, i32 } %263, 0
  store i8* %264, i8** %47, align 8
  %265 = extractvalue { i8*, i32 } %263, 1
  store i32 %265, i32* %48, align 4
  invoke void @__cxa_end_catch()
          to label %266 unwind label %273

; <label>:266:                                    ; preds = %262
  br label %268

; <label>:267:                                    ; preds = %260
  ret void

; <label>:268:                                    ; preds = %266
  %269 = load i8*, i8** %47, align 8
  %270 = load i32, i32* %48, align 4
  %271 = insertvalue { i8*, i32 } undef, i8* %269, 0
  %272 = insertvalue { i8*, i32 } %271, i32 %270, 1
  resume { i8*, i32 } %272

; <label>:273:                                    ; preds = %262
  %274 = landingpad { i8*, i32 }
          catch i8* null
  %275 = extractvalue { i8*, i32 } %274, 0
  call void @__clang_call_terminate(i8* %275) #16
  unreachable

; <label>:276:                                    ; preds = %261
  unreachable
}

declare void @__cxa_rethrow()

declare dereferenceable(160) %"class.std::__1::basic_ostream"* @_ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE3putEc(%"class.std::__1::basic_ostream"*, i8 signext) #1

declare dereferenceable(160) %"class.std::__1::basic_ostream"* @_ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE5flushEv(%"class.std::__1::basic_ostream"*) #1

attributes #0 = { ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noreturn "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { norecurse ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { inlinehint nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { nounwind readnone }
attributes #7 = { alwaysinline inlinehint ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { noinline noreturn nounwind }
attributes #9 = { nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #10 = { nobuiltin nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #11 = { nobuiltin "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #12 = { argmemonly nounwind }
attributes #13 = { inlinehint ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #14 = { noreturn }
attributes #15 = { nounwind }
attributes #16 = { noreturn nounwind }
attributes #17 = { builtin nounwind }
attributes #18 = { builtin }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"Apple LLVM version 8.1.0 (clang-802.0.42)"}
!2 = !{!3}
!3 = distinct !{!3, !4, !"_ZNKSt3__14fposI11__mbstate_tE5stateEv: argument 0"}
!4 = distinct !{!4, !"_ZNKSt3__14fposI11__mbstate_tE5stateEv"}
!5 = !{!6}
!6 = distinct !{!6, !7, !"_ZNKSt3__115basic_streambufIcNS_11char_traitsIcEEE6getlocEv: argument 0"}
!7 = distinct !{!7, !"_ZNKSt3__115basic_streambufIcNS_11char_traitsIcEEE6getlocEv"}
!8 = !{!9}
!9 = distinct !{!9, !10, !"_ZNKSt3__115basic_streambufIcNS_11char_traitsIcEEE6getlocEv: argument 0"}
!10 = distinct !{!10, !"_ZNKSt3__115basic_streambufIcNS_11char_traitsIcEEE6getlocEv"}
