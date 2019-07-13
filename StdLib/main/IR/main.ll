; ModuleID = '/Users/Jonas/CDot/StdLib/main/main.dot'
source_filename = "/Users/Jonas/CDot/StdLib/main/main.dot"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-darwin18.0.0"

%"struct.main.<private name>.TypeInfo" = type <{ %"struct.main.<private name>.TypeInfo"*, i8**, void (i8*)*, i8*, %"struct.main.<private name>.ValueWitnessTable"*, %"struct.main.<private name>.ProtocolConformance"*, i64, i64, i64 }>
%"struct.main.<private name>.ValueWitnessTable" = type <{ void (i8*, i8*)*, void (i8*)* }>
%"struct.main.<private name>.ProtocolConformance" = type <{ %"struct.main.<private name>.TypeInfo"*, i8** }>
%"struct.main.<private name>.Int64" = type <{ i64 }>
%"struct.main.<private name>.Bool" = type <{ i1 }>
%"struct.main.<private name>.ExistentialContainer" = type <{ i8*, %"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.ProtocolConformance"* }>

@_CTINW4mainE11MemoryOrderE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (i32*)* @_CNW4mainE11MemoryOrderD0Ev to void (i8*)*), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @0, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"0", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([6 x %"struct.main.<private name>.ProtocolConformance"], [6 x %"struct.main.<private name>.ProtocolConformance"]* @"1", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 4 }>, %"struct.main.<private name>.Int64" <{ i64 4 }>, %"struct.main.<private name>.Int64" <{ i64 4 }> }>, align 8
@"0" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (i32 (i32)* @_CNW4mainE11MemoryOrder4copyES_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (i32*)* @_CNW4mainE11MemoryOrderD0Ev to void (i8*)*) }>, align 8
@_CTINW4mainE3AnyE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([24 x i8], [24 x i8]* @1, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* null, %"struct.main.<private name>.ProtocolConformance"* null, %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW4mainE11MemoryOrderENW_0E3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTINW4mainE9EquatableE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([30 x i8], [30 x i8]* @2, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* null, %"struct.main.<private name>.ProtocolConformance"* null, %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW4mainE11MemoryOrderENW_0E9EquatableE = constant [2 x i8*] [i8* bitcast (%"struct.main.<private name>.Bool" (i32, i32)* @"_CNW4mainE11MemoryOrderXi2==ES_S_L0L0" to i8*), i8* bitcast (%"struct.main.<private name>.Bool" (i32, i32)* @"_CNW4mainE11MemoryOrderXi2!=ES_S_L0L0" to i8*)], align 8
@_CTINW4mainE16RawRepresentableE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([37 x i8], [37 x i8]* @3, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* null, %"struct.main.<private name>.ProtocolConformance"* null, %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW4mainE11MemoryOrderENW_0E16RawRepresentableE = constant [2 x i8*] [i8* bitcast (void (i32*, i32)* @_CNW4mainE11MemoryOrderC1EiL8rawValue to i8*), i8* bitcast (i32 (i32)* @_CNW4mainE11MemoryOrderGa8rawValueES_L0 to i8*)], align 8
@_CTINW4mainE8CopyableE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([29 x i8], [29 x i8]* @4, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* null, %"struct.main.<private name>.ProtocolConformance"* null, %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW4mainE11MemoryOrderENW_0E8CopyableE = constant [1 x i8*] [i8* bitcast (i32 (i32)* @_CNW4mainE11MemoryOrder4copyES_L0 to i8*)], align 8
@_CTINW4mainE18ImplicitlyCopyableE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([39 x i8], [39 x i8]* @5, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* null, %"struct.main.<private name>.ProtocolConformance"* null, %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW4mainE11MemoryOrderENW_0E18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"1" = constant [6 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE11MemoryOrderENW_0E3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE9EquatableE, i8** getelementptr inbounds ([2 x i8*], [2 x i8*]* @_CTPNW4mainE11MemoryOrderENW_0E9EquatableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE16RawRepresentableE, i8** getelementptr inbounds ([2 x i8*], [2 x i8*]* @_CTPNW4mainE11MemoryOrderENW_0E16RawRepresentableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE11MemoryOrderENW_0E8CopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE11MemoryOrderENW_0E18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE4BoolE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.Bool"*)* @_CNW4mainE4BoolD0Ev to void (i8*)*), i8* getelementptr inbounds ([25 x i8], [25 x i8]* @6, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"2", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([4 x %"struct.main.<private name>.ProtocolConformance"], [4 x %"struct.main.<private name>.ProtocolConformance"]* @"3", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 1 }>, %"struct.main.<private name>.Int64" <{ i64 1 }>, %"struct.main.<private name>.Int64" <{ i64 1 }> }>, align 8
@"2" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (%"struct.main.<private name>.Bool" (%"struct.main.<private name>.Bool")* @_CNW4mainE4Bool4copyES0_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (%"struct.main.<private name>.Bool"*)* @_CNW4mainE4BoolD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE4BoolENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTPNW4mainE4BoolENS_8CopyableE = constant [1 x i8*] [i8* bitcast (%"struct.main.<private name>.Bool" (%"struct.main.<private name>.Bool")* @_CNW4mainE4Bool4copyES0_L0 to i8*)], align 8
@_CTPNW4mainE4BoolENS_18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"3" = constant [4 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE4BoolENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE4BoolENS_8CopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE4BoolENS_18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE17ValueWitnessTableE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.ValueWitnessTable"*)* @_CNW4mainE17ValueWitnessTableD0Ev to void (i8*)*), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @7, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"4", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"5", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 16 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 16 }> }>, align 8
@"4" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.ValueWitnessTable"*)* @_CNW4mainE17ValueWitnessTableD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE17ValueWitnessTableENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"5" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE17ValueWitnessTableENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE19ProtocolConformanceE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.ProtocolConformance"*)* @_CNW4mainE19ProtocolConformanceD0Ev to void (i8*)*), i8* getelementptr inbounds ([40 x i8], [40 x i8]* @8, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"6", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"7", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 16 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 16 }> }>, align 8
@"6" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.ProtocolConformance"*)* @_CNW4mainE19ProtocolConformanceD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE19ProtocolConformanceENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"7" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE19ProtocolConformanceENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE8TypeInfoE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.TypeInfo"*)* @_CNW4mainE8TypeInfoD0Ev to void (i8*)*), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @9, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"8", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"9", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 72 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 72 }> }>, align 8
@"8" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.TypeInfo"*)* @_CNW4mainE8TypeInfoD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE8TypeInfoENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"9" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE8TypeInfoENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE5Int64E = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.Int64"*)* @_CNW4mainE5Int64D0Ev to void (i8*)*), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @10, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"10", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([4 x %"struct.main.<private name>.ProtocolConformance"], [4 x %"struct.main.<private name>.ProtocolConformance"]* @"11", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 8 }> }>, align 8
@"10" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (%"struct.main.<private name>.Int64" (%"struct.main.<private name>.Int64")* @_CNW4mainE5Int644copyES0_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (%"struct.main.<private name>.Int64"*)* @_CNW4mainE5Int64D0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE5Int64ENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTPNW4mainE5Int64ENS_8CopyableE = constant [1 x i8*] [i8* bitcast (%"struct.main.<private name>.Int64" (%"struct.main.<private name>.Int64")* @_CNW4mainE5Int644copyES0_L0 to i8*)], align 8
@_CTPNW4mainE5Int64ENS_18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"11" = constant [4 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE5Int64ENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE5Int64ENS_8CopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE5Int64ENS_18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE20ExistentialContainerE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.ExistentialContainer"*)* @_CNW4mainE20ExistentialContainerD0Ev to void (i8*)*), i8* getelementptr inbounds ([41 x i8], [41 x i8]* @11, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"12", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"13", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@"12" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.ExistentialContainer"*)* @_CNW4mainE20ExistentialContainerD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE20ExistentialContainerENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"13" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE20ExistentialContainerENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@0 = private unnamed_addr constant [17 x i8] c"main.MemoryOrder\00"
@1 = private unnamed_addr constant [24 x i8] c"main.<private name>.Any\00"
@2 = private unnamed_addr constant [30 x i8] c"main.<private name>.Equatable\00"
@3 = private unnamed_addr constant [37 x i8] c"main.<private name>.RawRepresentable\00"
@4 = private unnamed_addr constant [29 x i8] c"main.<private name>.Copyable\00"
@5 = private unnamed_addr constant [39 x i8] c"main.<private name>.ImplicitlyCopyable\00"
@6 = private unnamed_addr constant [25 x i8] c"main.<private name>.Bool\00"
@7 = private unnamed_addr constant [38 x i8] c"main.<private name>.ValueWitnessTable\00"
@8 = private unnamed_addr constant [40 x i8] c"main.<private name>.ProtocolConformance\00"
@9 = private unnamed_addr constant [29 x i8] c"main.<private name>.TypeInfo\00"
@10 = private unnamed_addr constant [26 x i8] c"main.<private name>.Int64\00"
@11 = private unnamed_addr constant [41 x i8] c"main.<private name>.ExistentialContainer\00"

define void @_CNW4mainE6_startEv() !dbg !5 {
alloca_block:
  %0 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %0, metadata !8, metadata !DIExpression()), !dbg !10
  call void @llvm.dbg.value(metadata i64 0, metadata !8, metadata !DIExpression()), !dbg !10
  store i64 0, i64* %0, align 8, !dbg !10
  %1 = bitcast i64* %0 to i8*, !dbg !10
  call void @llvm.lifetime.start.p0i8(i64 8, i8* %1), !dbg !10
  %2 = load i64, i64* %0, align 8, !dbg !11
  call void @_CZNW4mainE6_startEvE5funkyIlETvlL0(i64 %2), !dbg !12
  %3 = bitcast i64* %0 to i8*, !dbg !12
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %3), !dbg !12
  ret void, !dbg !12
}

define i32 @main() !dbg !13 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE6_startEv(), !dbg !14
  ret i32 0, !dbg !14
}

define void @_CZNW4mainE6_startEvE5funkyIlETvlL0(i64) !dbg !15 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !18, metadata !DIExpression()), !dbg !19
  call void @llvm.dbg.value(metadata i64 %0, metadata !18, metadata !DIExpression()), !dbg !19
  store i64 %0, i64* %1, align 8, !dbg !19
  ret void, !dbg !20
}

define void @_CNW4mainE11MemoryOrderD0Ev(i32*) !dbg !21 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE11MemoryOrderD3Ev(i32* %0), !dbg !35
  ret void, !dbg !35
}

define i32 @_CNW4mainE11MemoryOrder4copyES_L0(i32) !dbg !36 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret i32 %0, !dbg !39
}

define %"struct.main.<private name>.Bool" @"_CNW4mainE11MemoryOrderXi2==ES_S_L0L0"(i32, i32) !dbg !40 {
alloca_block:
  %2 = alloca %"struct.main.<private name>.Bool", align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = icmp eq i32 %0, %1, !dbg !43
  %4 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %2, align 1, !dbg !43
  %5 = extractvalue %"struct.main.<private name>.Bool" %4, 0, !dbg !43
  %6 = alloca i1, !dbg !43
  store i1 %5, i1* %6, !dbg !43
  store i1 %3, i1* %6, !dbg !43
  ret %"struct.main.<private name>.Bool" %4, !dbg !43
}

define %"struct.main.<private name>.Bool" @"_CNW4mainE11MemoryOrderXi2!=ES_S_L0L0"(i32, i32) !dbg !44 {
alloca_block:
  %2 = alloca %"struct.main.<private name>.Bool", align 1
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !45, metadata !DIExpression()), !dbg !46
  call void @llvm.dbg.value(metadata i32 %0, metadata !45, metadata !DIExpression()), !dbg !46
  store i32 %0, i32* %4, align 4, !dbg !46
  call void @llvm.dbg.declare(metadata i32* %3, metadata !47, metadata !DIExpression()), !dbg !46
  call void @llvm.dbg.value(metadata i32 %1, metadata !47, metadata !DIExpression()), !dbg !46
  store i32 %1, i32* %3, align 4, !dbg !46
  %5 = load i32, i32* %4, align 4, !dbg !46
  %6 = load i32, i32* %3, align 4, !dbg !46
  %7 = call %"struct.main.<private name>.Bool" @"_CNW4mainE11MemoryOrderXi2==ES_S_L0L0"(i32 %5, i32 %6), !dbg !46
  %8 = call %"struct.main.<private name>.Bool" @"_CNW4mainE4BoolXp1!ES0_L0"(%"struct.main.<private name>.Bool" %7), !dbg !46
  store %"struct.main.<private name>.Bool" %7, %"struct.main.<private name>.Bool"* %2, align 1, !dbg !46
  call void @_CNW4mainE4BoolD0Ev(%"struct.main.<private name>.Bool"* %2), !dbg !46
  ret %"struct.main.<private name>.Bool" %8, !dbg !46
}

define void @_CNW4mainE11MemoryOrderC1EiL8rawValue(i32*, i32) !dbg !48 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  switch i32 %1, label %init.failure [
    i32 0, label %init.case.NotAtomic
    i32 1, label %init.case.Relaxed
    i32 2, label %init.case.Consume
    i32 3, label %init.case.Acquire
    i32 4, label %init.case.Release
    i32 5, label %init.case.AcquireRelease
    i32 6, label %init.case.SequentiallyConsistent
  ], !dbg !52

init.failure:                                     ; preds = %entry
  %9 = load i32, i32* %0, align 4, !dbg !52
  %10 = bitcast i32* %0 to i8*, !dbg !52
  call void @llvm.memset.p0i8.i64(i8* %10, i8 0, i64 8, i32 1, i1 false), !dbg !52
  ret void, !dbg !52

init.case.NotAtomic:                              ; preds = %entry
  store i32 0, i32* %8, align 4, !dbg !52
  %11 = bitcast i32* %0 to i8*, !dbg !52
  %12 = bitcast i32* %8 to i8*, !dbg !52
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %11, i8* %12, i64 8, i32 1, i1 false), !dbg !52
  ret void, !dbg !52

init.case.Relaxed:                                ; preds = %entry
  store i32 1, i32* %7, align 4, !dbg !52
  %13 = bitcast i32* %0 to i8*, !dbg !52
  %14 = bitcast i32* %7 to i8*, !dbg !52
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %13, i8* %14, i64 8, i32 1, i1 false), !dbg !52
  ret void, !dbg !52

init.case.Consume:                                ; preds = %entry
  store i32 2, i32* %6, align 4, !dbg !52
  %15 = bitcast i32* %0 to i8*, !dbg !52
  %16 = bitcast i32* %6 to i8*, !dbg !52
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %15, i8* %16, i64 8, i32 1, i1 false), !dbg !52
  ret void, !dbg !52

init.case.Acquire:                                ; preds = %entry
  store i32 3, i32* %5, align 4, !dbg !52
  %17 = bitcast i32* %0 to i8*, !dbg !52
  %18 = bitcast i32* %5 to i8*, !dbg !52
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %17, i8* %18, i64 8, i32 1, i1 false), !dbg !52
  ret void, !dbg !52

init.case.Release:                                ; preds = %entry
  store i32 4, i32* %4, align 4, !dbg !52
  %19 = bitcast i32* %0 to i8*, !dbg !52
  %20 = bitcast i32* %4 to i8*, !dbg !52
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %19, i8* %20, i64 8, i32 1, i1 false), !dbg !52
  ret void, !dbg !52

init.case.AcquireRelease:                         ; preds = %entry
  store i32 5, i32* %3, align 4, !dbg !52
  %21 = bitcast i32* %0 to i8*, !dbg !52
  %22 = bitcast i32* %3 to i8*, !dbg !52
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %21, i8* %22, i64 8, i32 1, i1 false), !dbg !52
  ret void, !dbg !52

init.case.SequentiallyConsistent:                 ; preds = %entry
  store i32 6, i32* %2, align 4, !dbg !52
  %23 = bitcast i32* %0 to i8*, !dbg !52
  %24 = bitcast i32* %2 to i8*, !dbg !52
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %23, i8* %24, i64 8, i32 1, i1 false), !dbg !52
  ret void, !dbg !52
}

define i32 @_CNW4mainE11MemoryOrderGa8rawValueES_L0(i32) !dbg !53 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret i32 %0, !dbg !54
}

define void @_CNW4mainE11MemoryOrderD3Ev(i32*) !dbg !55 {
merge:
  ret void, !dbg !56
}

define i1 @"_CNW4mainE11MemoryOrderXi2~=ES_S_L0L0"(i32, i32) !dbg !57 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !58, metadata !DIExpression()), !dbg !59
  call void @llvm.dbg.value(metadata i32 %0, metadata !58, metadata !DIExpression()), !dbg !59
  store i32 %0, i32* %3, align 4, !dbg !59
  call void @llvm.dbg.declare(metadata i32* %2, metadata !60, metadata !DIExpression()), !dbg !59
  call void @llvm.dbg.value(metadata i32 %1, metadata !60, metadata !DIExpression()), !dbg !59
  store i32 %1, i32* %2, align 4, !dbg !59
  %4 = load i32, i32* %3, align 4, !dbg !61
  %5 = call i32 @_CNW4mainE11MemoryOrderGa8rawValueES_L0(i32 %4), !dbg !61
  %6 = sext i32 %5 to i64, !dbg !61
  %7 = load i32, i32* %2, align 4, !dbg !61
  %8 = call i32 @_CNW4mainE11MemoryOrderGa8rawValueES_L0(i32 %7), !dbg !61
  %9 = sext i32 %8 to i64, !dbg !61
  %10 = icmp eq i64 %6, %9, !dbg !61
  ret i1 %10, !dbg !61
}

define %"struct.main.<private name>.Bool" @"_CNW4mainE4BoolXp1!ES0_L0"(%"struct.main.<private name>.Bool") !dbg !62 {
alloca_block:
  %1 = alloca %"struct.main.<private name>.Bool", align 1
  %2 = alloca %"struct.main.<private name>.Bool", align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata %"struct.main.<private name>.Bool"* %2, metadata !69, metadata !DIExpression()), !dbg !70
  call void @llvm.dbg.value(metadata %"struct.main.<private name>.Bool" %0, metadata !69, metadata !DIExpression()), !dbg !70
  store %"struct.main.<private name>.Bool" %0, %"struct.main.<private name>.Bool"* %2, align 1, !dbg !70
  %3 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %2, align 1, !dbg !70
  %4 = extractvalue %"struct.main.<private name>.Bool" %3, 0, !dbg !70
  %5 = alloca i1, !dbg !70
  store i1 %4, i1* %5, !dbg !70
  %6 = load i1, i1* %5, !dbg !70
  %7 = xor i1 %6, true, !dbg !70
  call void @_CNW4mainE4BoolC1EbL5value(%"struct.main.<private name>.Bool"* %1, i1 %7), !dbg !70
  %8 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %1, !dbg !70
  ret %"struct.main.<private name>.Bool" %8, !dbg !70
}

define void @_CNW4mainE4BoolD0Ev(%"struct.main.<private name>.Bool"*) !dbg !71 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE4BoolD3Ev(%"struct.main.<private name>.Bool"* %0), !dbg !75
  ret void, !dbg !75
}

define void @_CNW4mainE4funcElL2v1(i64) !dbg !76 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !77, metadata !DIExpression()), !dbg !78
  call void @llvm.dbg.value(metadata i64 %0, metadata !77, metadata !DIExpression()), !dbg !78
  store i64 %0, i64* %1, align 8, !dbg !78
  ret void, !dbg !79
}

define void @_CNW4mainE4funcElL2v2(i64) !dbg !80 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !81, metadata !DIExpression()), !dbg !82
  call void @llvm.dbg.value(metadata i64 %0, metadata !81, metadata !DIExpression()), !dbg !82
  store i64 %0, i64* %1, align 8, !dbg !82
  ret void, !dbg !83
}

define %"struct.main.<private name>.Bool" @_CNW4mainE4Bool4copyES0_L0(%"struct.main.<private name>.Bool") !dbg !84 {
alloca_block:
  %1 = alloca %"struct.main.<private name>.Bool", align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %1, align 1, !dbg !85
  %3 = extractvalue %"struct.main.<private name>.Bool" %2, 0, !dbg !85
  %4 = alloca i1, !dbg !85
  store i1 %3, i1* %4, !dbg !85
  %5 = extractvalue %"struct.main.<private name>.Bool" %0, 0, !dbg !85
  %6 = alloca i1, !dbg !85
  store i1 %5, i1* %6, !dbg !85
  %7 = load i1, i1* %6, !dbg !85
  store i1 %7, i1* %4, !dbg !85
  ret %"struct.main.<private name>.Bool" %2, !dbg !85
}

define void @_CNW4mainE4Bool14__default_initES0_L0(%"struct.main.<private name>.Bool") !dbg !86 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !87
}

define void @_CNW4mainE4BoolC1EbL5value(%"struct.main.<private name>.Bool"*, i1) !dbg !88 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %0, align 1, !dbg !91
  call void @_CNW4mainE4Bool14__default_initES0_L0(%"struct.main.<private name>.Bool" %2), !dbg !91
  %3 = extractvalue %"struct.main.<private name>.Bool" %2, 0, !dbg !91
  %4 = alloca i1, !dbg !91
  store i1 %3, i1* %4, !dbg !91
  store i1 %1, i1* %4, !dbg !91
  ret void, !dbg !91
}

define void @_CNW4mainE4BoolC2ES0_bL0L5value(%"struct.main.<private name>.Bool", i1) !dbg !92 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = extractvalue %"struct.main.<private name>.Bool" %0, 0, !dbg !95
  %3 = alloca i1, !dbg !95
  store i1 %2, i1* %3, !dbg !95
  store i1 %1, i1* %3, !dbg !95
  ret void, !dbg !95
}

define void @_CNW4mainE4BoolD3Ev(%"struct.main.<private name>.Bool"*) !dbg !96 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %0, align 1, !dbg !97
  %2 = extractvalue %"struct.main.<private name>.Bool" %1, 0, !dbg !97
  %3 = alloca i1, !dbg !97
  store i1 %2, i1* %3, !dbg !97
  %4 = bitcast i1* %3 to i8*, !dbg !97
  call void @llvm.lifetime.end.p0i8(i64 1, i8* %4), !dbg !97
  ret void, !dbg !97
}

define void @_CNW4mainE17ValueWitnessTableD0Ev(%"struct.main.<private name>.ValueWitnessTable"*) !dbg !98 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE17ValueWitnessTableD3Ev(%"struct.main.<private name>.ValueWitnessTable"* %0), !dbg !113
  ret void, !dbg !113
}

define void @_CNW4mainE17ValueWitnessTable14__default_initES0_L0(%"struct.main.<private name>.ValueWitnessTable") !dbg !114 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !117
}

define void @_CNW4mainE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit(%"struct.main.<private name>.ValueWitnessTable"*, void (i8*, i8*)*, void (i8*)*) !dbg !118 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.main.<private name>.ValueWitnessTable", %"struct.main.<private name>.ValueWitnessTable"* %0, align 8, !dbg !121
  call void @_CNW4mainE17ValueWitnessTable14__default_initES0_L0(%"struct.main.<private name>.ValueWitnessTable" %3), !dbg !121
  %4 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %3, 0, !dbg !121
  %5 = alloca void (i8*, i8*)*, !dbg !121
  store void (i8*, i8*)* %4, void (i8*, i8*)** %5, !dbg !121
  store void (i8*, i8*)* %1, void (i8*, i8*)** %5, !dbg !121
  %6 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %3, 1, !dbg !121
  %7 = alloca void (i8*)*, !dbg !121
  store void (i8*)* %6, void (i8*)** %7, !dbg !121
  store void (i8*)* %2, void (i8*)** %7, !dbg !121
  ret void, !dbg !121
}

define void @_CNW4mainE17ValueWitnessTableC2ES0_DoFvPvKPvEDoFvS1_EL0L6copyFnL6deinit(%"struct.main.<private name>.ValueWitnessTable", void (i8*, i8*)*, void (i8*)*) !dbg !122 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %0, 0, !dbg !125
  %4 = alloca void (i8*, i8*)*, !dbg !125
  store void (i8*, i8*)* %3, void (i8*, i8*)** %4, !dbg !125
  store void (i8*, i8*)* %1, void (i8*, i8*)** %4, !dbg !125
  %5 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %0, 1, !dbg !125
  %6 = alloca void (i8*)*, !dbg !125
  store void (i8*)* %5, void (i8*)** %6, !dbg !125
  store void (i8*)* %2, void (i8*)** %6, !dbg !125
  ret void, !dbg !125
}

define void @_CNW4mainE17ValueWitnessTableD3Ev(%"struct.main.<private name>.ValueWitnessTable"*) !dbg !126 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.ValueWitnessTable", %"struct.main.<private name>.ValueWitnessTable"* %0, align 8, !dbg !127
  %2 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %1, 0, !dbg !127
  %3 = alloca void (i8*, i8*)*, !dbg !127
  store void (i8*, i8*)* %2, void (i8*, i8*)** %3, !dbg !127
  %4 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %1, 1, !dbg !127
  %5 = alloca void (i8*)*, !dbg !127
  store void (i8*)* %4, void (i8*)** %5, !dbg !127
  %6 = bitcast void (i8*)** %5 to i8*, !dbg !127
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %6), !dbg !127
  %7 = bitcast void (i8*, i8*)** %3 to i8*, !dbg !127
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %7), !dbg !127
  ret void, !dbg !127
}

define void @_CNW4mainE19ProtocolConformanceD0Ev(%"struct.main.<private name>.ProtocolConformance"*) !dbg !128 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE19ProtocolConformanceD3Ev(%"struct.main.<private name>.ProtocolConformance"* %0), !dbg !154
  ret void, !dbg !154
}

define void @_CNW4mainE19ProtocolConformance14__default_initES0_L0(%"struct.main.<private name>.ProtocolConformance") !dbg !155 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !158
}

define void @_CNW4mainE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable(%"struct.main.<private name>.ProtocolConformance"*, %"struct.main.<private name>.TypeInfo"*, i8**) !dbg !159 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.main.<private name>.ProtocolConformance", %"struct.main.<private name>.ProtocolConformance"* %0, align 8, !dbg !162
  call void @_CNW4mainE19ProtocolConformance14__default_initES0_L0(%"struct.main.<private name>.ProtocolConformance" %3), !dbg !162
  %4 = extractvalue %"struct.main.<private name>.ProtocolConformance" %3, 0, !dbg !162
  %5 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !162
  store %"struct.main.<private name>.TypeInfo"* %4, %"struct.main.<private name>.TypeInfo"** %5, !dbg !162
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %5, !dbg !162
  %6 = extractvalue %"struct.main.<private name>.ProtocolConformance" %3, 1, !dbg !162
  %7 = alloca i8**, !dbg !162
  store i8** %6, i8*** %7, !dbg !162
  store i8** %2, i8*** %7, !dbg !162
  ret void, !dbg !162
}

define void @_CNW4mainE19ProtocolConformanceC2ES0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable(%"struct.main.<private name>.ProtocolConformance", %"struct.main.<private name>.TypeInfo"*, i8**) !dbg !163 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = extractvalue %"struct.main.<private name>.ProtocolConformance" %0, 0, !dbg !166
  %4 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !166
  store %"struct.main.<private name>.TypeInfo"* %3, %"struct.main.<private name>.TypeInfo"** %4, !dbg !166
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %4, !dbg !166
  %5 = extractvalue %"struct.main.<private name>.ProtocolConformance" %0, 1, !dbg !166
  %6 = alloca i8**, !dbg !166
  store i8** %5, i8*** %6, !dbg !166
  store i8** %2, i8*** %6, !dbg !166
  ret void, !dbg !166
}

define void @_CNW4mainE19ProtocolConformanceD3Ev(%"struct.main.<private name>.ProtocolConformance"*) !dbg !167 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.ProtocolConformance", %"struct.main.<private name>.ProtocolConformance"* %0, align 8, !dbg !168
  %2 = extractvalue %"struct.main.<private name>.ProtocolConformance" %1, 0, !dbg !168
  %3 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !168
  store %"struct.main.<private name>.TypeInfo"* %2, %"struct.main.<private name>.TypeInfo"** %3, !dbg !168
  %4 = extractvalue %"struct.main.<private name>.ProtocolConformance" %1, 1, !dbg !168
  %5 = alloca i8**, !dbg !168
  store i8** %4, i8*** %5, !dbg !168
  %6 = bitcast i8*** %5 to i8*, !dbg !168
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %6), !dbg !168
  %7 = bitcast %"struct.main.<private name>.TypeInfo"** %3 to i8*, !dbg !168
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %7), !dbg !168
  ret void, !dbg !168
}

define void @_CNW4mainE8TypeInfoD0Ev(%"struct.main.<private name>.TypeInfo"*) !dbg !169 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE8TypeInfoD3Ev(%"struct.main.<private name>.TypeInfo"* %0), !dbg !173
  ret void, !dbg !173
}

define void @_CNW4mainE8TypeInfo14__default_initES0_L0(%"struct.main.<private name>.TypeInfo"*) !dbg !174 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !177
}

define void @_CNW4mainE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride(%"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.TypeInfo"*, i8**, void (i8*)*, i8*, %"struct.main.<private name>.ValueWitnessTable"*, %"struct.main.<private name>.ProtocolConformance"*, i64, i64, i64) !dbg !178 {
entry:
  call void @_CNW4mainE8TypeInfo14__default_initES0_L0(%"struct.main.<private name>.TypeInfo"* %0), !dbg !181
  %10 = bitcast %"struct.main.<private name>.TypeInfo"* %0 to %"struct.main.<private name>.TypeInfo"**
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %10, align 8, !dbg !181
  %11 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !181
  store i8** %2, i8*** %11, align 8, !dbg !181
  %12 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !181
  store void (i8*)* %3, void (i8*)** %12, align 8, !dbg !181
  %13 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !181
  store i8* %4, i8** %13, align 8, !dbg !181
  %14 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !181
  store %"struct.main.<private name>.ValueWitnessTable"* %5, %"struct.main.<private name>.ValueWitnessTable"** %14, align 8, !dbg !181
  %15 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !181
  store %"struct.main.<private name>.ProtocolConformance"* %6, %"struct.main.<private name>.ProtocolConformance"** %15, align 8, !dbg !181
  %16 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !181
  store i64 %7, i64* %16, align 8, !dbg !181
  %17 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !181
  store i64 %8, i64* %17, align 8, !dbg !181
  %18 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !181
  store i64 %9, i64* %18, align 8, !dbg !181
  ret void, !dbg !181
}

define void @_CNW4mainE8TypeInfoC2ES0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride(%"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.TypeInfo"*, i8**, void (i8*)*, i8*, %"struct.main.<private name>.ValueWitnessTable"*, %"struct.main.<private name>.ProtocolConformance"*, i64, i64, i64) !dbg !182 {
entry:
  %10 = bitcast %"struct.main.<private name>.TypeInfo"* %0 to %"struct.main.<private name>.TypeInfo"**
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %10, align 8, !dbg !185
  %11 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !185
  store i8** %2, i8*** %11, align 8, !dbg !185
  %12 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !185
  store void (i8*)* %3, void (i8*)** %12, align 8, !dbg !185
  %13 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !185
  store i8* %4, i8** %13, align 8, !dbg !185
  %14 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !185
  store %"struct.main.<private name>.ValueWitnessTable"* %5, %"struct.main.<private name>.ValueWitnessTable"** %14, align 8, !dbg !185
  %15 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !185
  store %"struct.main.<private name>.ProtocolConformance"* %6, %"struct.main.<private name>.ProtocolConformance"** %15, align 8, !dbg !185
  %16 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !185
  store i64 %7, i64* %16, align 8, !dbg !185
  %17 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !185
  store i64 %8, i64* %17, align 8, !dbg !185
  %18 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !185
  store i64 %9, i64* %18, align 8, !dbg !185
  ret void, !dbg !185
}

define void @_CNW4mainE8TypeInfoD3Ev(%"struct.main.<private name>.TypeInfo"*) !dbg !186 {
entry:
  %1 = bitcast %"struct.main.<private name>.TypeInfo"* %0 to %"struct.main.<private name>.TypeInfo"**
  %2 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !187
  %3 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !187
  %4 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !187
  %5 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !187
  %6 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !187
  %7 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !187
  %8 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !187
  %9 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !187
  %10 = bitcast i64* %9 to i8*, !dbg !187
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %10), !dbg !187
  %11 = bitcast i64* %8 to i8*, !dbg !187
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %11), !dbg !187
  %12 = bitcast i64* %7 to i8*, !dbg !187
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %12), !dbg !187
  %13 = bitcast %"struct.main.<private name>.ProtocolConformance"** %6 to i8*, !dbg !187
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %13), !dbg !187
  %14 = bitcast %"struct.main.<private name>.ValueWitnessTable"** %5 to i8*, !dbg !187
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %14), !dbg !187
  %15 = bitcast i8** %4 to i8*, !dbg !187
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %15), !dbg !187
  %16 = bitcast void (i8*)** %3 to i8*, !dbg !187
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %16), !dbg !187
  %17 = bitcast i8*** %2 to i8*, !dbg !187
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %17), !dbg !187
  %18 = bitcast %"struct.main.<private name>.TypeInfo"** %1 to i8*, !dbg !187
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %18), !dbg !187
  ret void, !dbg !187
}

define void @_CNW4mainE5Int64D0Ev(%"struct.main.<private name>.Int64"*) !dbg !188 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE5Int64D3Ev(%"struct.main.<private name>.Int64"* %0), !dbg !195
  ret void, !dbg !195
}

define %"struct.main.<private name>.Int64" @_CNW4mainE5Int644copyES0_L0(%"struct.main.<private name>.Int64") !dbg !196 {
alloca_block:
  %1 = alloca %"struct.main.<private name>.Int64", align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Int64", %"struct.main.<private name>.Int64"* %1, align 8, !dbg !199
  %3 = extractvalue %"struct.main.<private name>.Int64" %2, 0, !dbg !199
  %4 = alloca i64, !dbg !199
  store i64 %3, i64* %4, !dbg !199
  %5 = extractvalue %"struct.main.<private name>.Int64" %0, 0, !dbg !199
  %6 = alloca i64, !dbg !199
  store i64 %5, i64* %6, !dbg !199
  %7 = load i64, i64* %6, !dbg !199
  store i64 %7, i64* %4, !dbg !199
  ret %"struct.main.<private name>.Int64" %2, !dbg !199
}

define void @_CNW4mainE5Int6414__default_initES0_L0(%"struct.main.<private name>.Int64") !dbg !200 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !201
}

define void @_CNW4mainE5Int64C1ElL5value(%"struct.main.<private name>.Int64"*, i64) !dbg !202 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Int64", %"struct.main.<private name>.Int64"* %0, align 8, !dbg !205
  call void @_CNW4mainE5Int6414__default_initES0_L0(%"struct.main.<private name>.Int64" %2), !dbg !205
  %3 = extractvalue %"struct.main.<private name>.Int64" %2, 0, !dbg !205
  %4 = alloca i64, !dbg !205
  store i64 %3, i64* %4, !dbg !205
  store i64 %1, i64* %4, !dbg !205
  ret void, !dbg !205
}

define void @_CNW4mainE5Int64C2ES0_lL0L5value(%"struct.main.<private name>.Int64", i64) !dbg !206 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = extractvalue %"struct.main.<private name>.Int64" %0, 0, !dbg !209
  %3 = alloca i64, !dbg !209
  store i64 %2, i64* %3, !dbg !209
  store i64 %1, i64* %3, !dbg !209
  ret void, !dbg !209
}

define void @_CNW4mainE5Int64D3Ev(%"struct.main.<private name>.Int64"*) !dbg !210 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.Int64", %"struct.main.<private name>.Int64"* %0, align 8, !dbg !211
  %2 = extractvalue %"struct.main.<private name>.Int64" %1, 0, !dbg !211
  %3 = alloca i64, !dbg !211
  store i64 %2, i64* %3, !dbg !211
  %4 = bitcast i64* %3 to i8*, !dbg !211
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %4), !dbg !211
  ret void, !dbg !211
}

define void @_CNW4mainE20ExistentialContainerD0Ev(%"struct.main.<private name>.ExistentialContainer"*) !dbg !212 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE20ExistentialContainerD3Ev(%"struct.main.<private name>.ExistentialContainer"* %0), !dbg !221
  ret void, !dbg !221
}

define void @_CNW4mainE20ExistentialContainer14__default_initES0_L0(%"struct.main.<private name>.ExistentialContainer") !dbg !222 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !225
}

define void @_CNW4mainE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance(%"struct.main.<private name>.ExistentialContainer"*, i8*, %"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.ProtocolConformance"*) !dbg !226 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %4 = load %"struct.main.<private name>.ExistentialContainer", %"struct.main.<private name>.ExistentialContainer"* %0, align 8, !dbg !229
  call void @_CNW4mainE20ExistentialContainer14__default_initES0_L0(%"struct.main.<private name>.ExistentialContainer" %4), !dbg !229
  %5 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 0, !dbg !229
  %6 = alloca i8*, !dbg !229
  store i8* %5, i8** %6, !dbg !229
  store i8* %1, i8** %6, !dbg !229
  %7 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 1, !dbg !229
  %8 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !229
  store %"struct.main.<private name>.TypeInfo"* %7, %"struct.main.<private name>.TypeInfo"** %8, !dbg !229
  store %"struct.main.<private name>.TypeInfo"* %2, %"struct.main.<private name>.TypeInfo"** %8, !dbg !229
  %9 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 2, !dbg !229
  %10 = alloca %"struct.main.<private name>.ProtocolConformance"*, !dbg !229
  store %"struct.main.<private name>.ProtocolConformance"* %9, %"struct.main.<private name>.ProtocolConformance"** %10, !dbg !229
  store %"struct.main.<private name>.ProtocolConformance"* %3, %"struct.main.<private name>.ProtocolConformance"** %10, !dbg !229
  ret void, !dbg !229
}

define void @_CNW4mainE20ExistentialContainerC2ES0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance(%"struct.main.<private name>.ExistentialContainer", i8*, %"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.ProtocolConformance"*) !dbg !230 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %4 = extractvalue %"struct.main.<private name>.ExistentialContainer" %0, 0, !dbg !233
  %5 = alloca i8*, !dbg !233
  store i8* %4, i8** %5, !dbg !233
  store i8* %1, i8** %5, !dbg !233
  %6 = extractvalue %"struct.main.<private name>.ExistentialContainer" %0, 1, !dbg !233
  %7 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !233
  store %"struct.main.<private name>.TypeInfo"* %6, %"struct.main.<private name>.TypeInfo"** %7, !dbg !233
  store %"struct.main.<private name>.TypeInfo"* %2, %"struct.main.<private name>.TypeInfo"** %7, !dbg !233
  %8 = extractvalue %"struct.main.<private name>.ExistentialContainer" %0, 2, !dbg !233
  %9 = alloca %"struct.main.<private name>.ProtocolConformance"*, !dbg !233
  store %"struct.main.<private name>.ProtocolConformance"* %8, %"struct.main.<private name>.ProtocolConformance"** %9, !dbg !233
  store %"struct.main.<private name>.ProtocolConformance"* %3, %"struct.main.<private name>.ProtocolConformance"** %9, !dbg !233
  ret void, !dbg !233
}

define void @_CNW4mainE20ExistentialContainerD3Ev(%"struct.main.<private name>.ExistentialContainer"*) !dbg !234 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.ExistentialContainer", %"struct.main.<private name>.ExistentialContainer"* %0, align 8, !dbg !235
  %2 = extractvalue %"struct.main.<private name>.ExistentialContainer" %1, 0, !dbg !235
  %3 = alloca i8*, !dbg !235
  store i8* %2, i8** %3, !dbg !235
  %4 = extractvalue %"struct.main.<private name>.ExistentialContainer" %1, 1, !dbg !235
  %5 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !235
  store %"struct.main.<private name>.TypeInfo"* %4, %"struct.main.<private name>.TypeInfo"** %5, !dbg !235
  %6 = extractvalue %"struct.main.<private name>.ExistentialContainer" %1, 2, !dbg !235
  %7 = alloca %"struct.main.<private name>.ProtocolConformance"*, !dbg !235
  store %"struct.main.<private name>.ProtocolConformance"* %6, %"struct.main.<private name>.ProtocolConformance"** %7, !dbg !235
  %8 = bitcast %"struct.main.<private name>.ProtocolConformance"** %7 to i8*, !dbg !235
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %8), !dbg !235
  %9 = bitcast %"struct.main.<private name>.TypeInfo"** %5 to i8*, !dbg !235
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %9), !dbg !235
  %10 = bitcast i8** %3 to i8*, !dbg !235
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %10), !dbg !235
  ret void, !dbg !235
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #0

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.value(metadata, metadata, metadata) #0

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #1

; Function Attrs: nounwind
declare void @llvm.stackprotector(i8*, i8**) #2

attributes #0 = { nounwind readnone speculatable }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "cdotc v0.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "/Users/Jonas/CDot/StdLib/main/main.dot", directory: "")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = distinct !DISubprogram(name: "_CNW4mainE6_startEv", linkageName: "_CNW4mainE6_startEv", scope: null, file: !6, line: 2, type: !7, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!6 = !DIFile(filename: "main.dot", directory: "/Users/Jonas/CDot/StdLib/main/")
!7 = !DISubroutineType(types: !2)
!8 = !DILocalVariable(name: "x", scope: !5, file: !1, line: 10, type: !9)
!9 = !DIBasicType(name: "i64", size: 64, encoding: DW_ATE_signed)
!10 = !DILocation(line: 10, column: 2, scope: !5)
!11 = !DILocation(line: 11, column: 8, scope: !5)
!12 = !DILocation(line: 11, column: 2, scope: !5)
!13 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !6, line: 2, type: !7, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!14 = !DILocation(line: 2, column: 1, scope: !13)
!15 = distinct !DISubprogram(name: "_CZNW4mainE6_startEvE5funkyIlETvlL0", linkageName: "_CZNW4mainE6_startEvE5funkyIlETvlL0", scope: null, file: !6, line: 8, type: !16, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!16 = !DISubroutineType(types: !17)
!17 = !{!9}
!18 = !DILocalVariable(name: "val", scope: !15, file: !1, line: 8, type: !9)
!19 = !DILocation(line: 8, column: 2, scope: !15)
!20 = !DILocation(line: 8, column: 35, scope: !15)
!21 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderD0Ev", linkageName: "_CNW4mainE11MemoryOrderD0Ev", scope: !0, file: !22, line: 6, type: !23, isLocal: false, isDefinition: true, scopeLine: 6, thisAdjustment: 1, isOptimized: false, unit: !0)
!22 = !DIFile(filename: "main4.dot", directory: "/Users/Jonas/CDot/StdLib/main/")
!23 = !DISubroutineType(types: !24)
!24 = !{!25}
!25 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !26, size: 64, align: 8)
!26 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "main.MemoryOrder", file: !22, line: 6, flags: DIFlagFwdDecl, elements: !27)
!27 = !{!28, !29, !30, !31, !32, !33, !34}
!28 = !DIEnumerator(name: "NotAtomic", value: 0)
!29 = !DIEnumerator(name: "Relaxed", value: 1)
!30 = !DIEnumerator(name: "Consume", value: 2)
!31 = !DIEnumerator(name: "Acquire", value: 3)
!32 = !DIEnumerator(name: "Release", value: 4)
!33 = !DIEnumerator(name: "AcquireRelease", value: 5)
!34 = !DIEnumerator(name: "SequentiallyConsistent", value: 6)
!35 = !DILocation(line: 6, column: 9, scope: !21)
!36 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrder4copyES_L0", linkageName: "_CNW4mainE11MemoryOrder4copyES_L0", scope: !0, file: !22, line: 6, type: !37, isLocal: false, isDefinition: true, scopeLine: 6, thisAdjustment: 1, isOptimized: false, unit: !0)
!37 = !DISubroutineType(types: !38)
!38 = !{!26}
!39 = !DILocation(line: 6, column: 9, scope: !36)
!40 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderXi2==ES_S_L0L0", linkageName: "_CNW4mainE11MemoryOrderXi2==ES_S_L0L0", scope: !0, file: !22, line: 6, type: !41, isLocal: false, isDefinition: true, scopeLine: 6, thisAdjustment: 1, isOptimized: false, unit: !0)
!41 = !DISubroutineType(types: !42)
!42 = !{!26, !26}
!43 = !DILocation(line: 6, column: 9, scope: !40)
!44 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderXi2!=ES_S_L0L0", linkageName: "_CNW4mainE11MemoryOrderXi2!=ES_S_L0L0", scope: !0, file: !6, line: 2, type: !41, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!45 = !DILocalVariable(name: "self", scope: !44, file: !1, line: 2, type: !26)
!46 = !DILocation(line: 2, column: 1, scope: !44)
!47 = !DILocalVariable(name: "rhs", scope: !44, file: !1, line: 2, type: !26)
!48 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderC1EiL8rawValue", linkageName: "_CNW4mainE11MemoryOrderC1EiL8rawValue", scope: !0, file: !22, line: 6, type: !49, isLocal: false, isDefinition: true, scopeLine: 6, thisAdjustment: 1, isOptimized: false, unit: !0)
!49 = !DISubroutineType(types: !50)
!50 = !{!25, !51}
!51 = !DIBasicType(name: "i32", size: 32, encoding: DW_ATE_signed)
!52 = !DILocation(line: 6, column: 9, scope: !48)
!53 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderGa8rawValueES_L0", linkageName: "_CNW4mainE11MemoryOrderGa8rawValueES_L0", scope: !0, file: !22, line: 6, type: !37, isLocal: false, isDefinition: true, scopeLine: 6, thisAdjustment: 1, isOptimized: false, unit: !0)
!54 = !DILocation(line: 6, column: 9, scope: !53)
!55 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderD3Ev", linkageName: "_CNW4mainE11MemoryOrderD3Ev", scope: null, file: !22, line: 6, type: !23, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!56 = !DILocation(line: 6, column: 9, scope: !55)
!57 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderXi2~=ES_S_L0L0", linkageName: "_CNW4mainE11MemoryOrderXi2~=ES_S_L0L0", scope: !0, file: !22, line: 25, type: !41, isLocal: false, isDefinition: true, scopeLine: 25, thisAdjustment: 1, isOptimized: false, unit: !0)
!58 = !DILocalVariable(name: "self", scope: !57, file: !1, line: 25, type: !26)
!59 = !DILocation(line: 25, column: 6, scope: !57)
!60 = !DILocalVariable(name: "rhs", scope: !57, file: !1, line: 25, type: !26)
!61 = !DILocation(line: 26, column: 10, scope: !57)
!62 = distinct !DISubprogram(name: "_CNW4mainE4BoolXp1!ES0_L0", linkageName: "_CNW4mainE4BoolXp1!ES0_L0", scope: !0, file: !6, line: 2, type: !63, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!63 = !DISubroutineType(types: !64)
!64 = !{!65}
!65 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.Bool", file: !6, line: 2, flags: DIFlagFwdDecl, elements: !66)
!66 = !{!67}
!67 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !65, file: !6, line: 2, baseType: !68, size: 8, align: 8)
!68 = !DIBasicType(name: "i1", size: 8, encoding: DW_ATE_boolean)
!69 = !DILocalVariable(name: "self", scope: !62, file: !1, line: 2, type: !65)
!70 = !DILocation(line: 2, column: 1, scope: !62)
!71 = distinct !DISubprogram(name: "_CNW4mainE4BoolD0Ev", linkageName: "_CNW4mainE4BoolD0Ev", scope: !0, file: !6, line: 2, type: !72, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!72 = !DISubroutineType(types: !73)
!73 = !{!74}
!74 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !65, size: 64, align: 8)
!75 = !DILocation(line: 2, column: 1, scope: !71)
!76 = distinct !DISubprogram(name: "_CNW4mainE4funcElL2v1", linkageName: "_CNW4mainE4funcElL2v1", scope: null, file: !22, line: 30, type: !16, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!77 = !DILocalVariable(name: "v1", scope: !76, file: !1, line: 30, type: !9)
!78 = !DILocation(line: 30, column: 2, scope: !76)
!79 = !DILocation(line: 30, column: 28, scope: !76)
!80 = distinct !DISubprogram(name: "_CNW4mainE4funcElL2v2", linkageName: "_CNW4mainE4funcElL2v2", scope: null, file: !22, line: 31, type: !16, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!81 = !DILocalVariable(name: "v2", scope: !80, file: !1, line: 31, type: !9)
!82 = !DILocation(line: 31, column: 2, scope: !80)
!83 = !DILocation(line: 31, column: 28, scope: !80)
!84 = distinct !DISubprogram(name: "_CNW4mainE4Bool4copyES0_L0", linkageName: "_CNW4mainE4Bool4copyES0_L0", scope: !0, file: !6, line: 2, type: !63, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!85 = !DILocation(line: 2, column: 1, scope: !84)
!86 = distinct !DISubprogram(name: "_CNW4mainE4Bool14__default_initES0_L0", linkageName: "_CNW4mainE4Bool14__default_initES0_L0", scope: !0, file: !6, line: 2, type: !63, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!87 = !DILocation(line: 2, column: 1, scope: !86)
!88 = distinct !DISubprogram(name: "_CNW4mainE4BoolC1EbL5value", linkageName: "_CNW4mainE4BoolC1EbL5value", scope: !0, file: !6, line: 2, type: !89, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!89 = !DISubroutineType(types: !90)
!90 = !{!74, !68}
!91 = !DILocation(line: 2, column: 1, scope: !88)
!92 = distinct !DISubprogram(name: "_CNW4mainE4BoolC2ES0_bL0L5value", linkageName: "_CNW4mainE4BoolC2ES0_bL0L5value", scope: !0, file: !6, line: 2, type: !93, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!93 = !DISubroutineType(types: !94)
!94 = !{!65, !68}
!95 = !DILocation(line: 2, column: 1, scope: !92)
!96 = distinct !DISubprogram(name: "_CNW4mainE4BoolD3Ev", linkageName: "_CNW4mainE4BoolD3Ev", scope: null, file: !6, line: 2, type: !72, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!97 = !DILocation(line: 2, column: 1, scope: !96)
!98 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableD0Ev", linkageName: "_CNW4mainE17ValueWitnessTableD0Ev", scope: !0, file: !6, line: 2, type: !99, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!99 = !DISubroutineType(types: !100)
!100 = !{!101}
!101 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !102, size: 64, align: 8)
!102 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.ValueWitnessTable", file: !6, line: 2, flags: DIFlagFwdDecl, elements: !103)
!103 = !{!104, !109}
!104 = !DIDerivedType(tag: DW_TAG_member, name: "copyFn", scope: !102, file: !6, line: 2, baseType: !105, size: 64, align: 64)
!105 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !106, size: 64, align: 8)
!106 = !DISubroutineType(types: !107)
!107 = !{!108, !108}
!108 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64, align: 8)
!109 = !DIDerivedType(tag: DW_TAG_member, name: "deinit", scope: !102, file: !6, line: 2, baseType: !110, size: 64, align: 64, offset: 64)
!110 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !111, size: 64, align: 8)
!111 = !DISubroutineType(types: !112)
!112 = !{!108}
!113 = !DILocation(line: 2, column: 1, scope: !98)
!114 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTable14__default_initES0_L0", linkageName: "_CNW4mainE17ValueWitnessTable14__default_initES0_L0", scope: !0, file: !6, line: 2, type: !115, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!115 = !DISubroutineType(types: !116)
!116 = !{!102}
!117 = !DILocation(line: 2, column: 1, scope: !114)
!118 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit", linkageName: "_CNW4mainE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit", scope: !0, file: !6, line: 2, type: !119, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!119 = !DISubroutineType(types: !120)
!120 = !{!101, !105, !110}
!121 = !DILocation(line: 2, column: 1, scope: !118)
!122 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableC2ES0_DoFvPvKPvEDoFvS1_EL0L6copyFnL6deinit", linkageName: "_CNW4mainE17ValueWitnessTableC2ES0_DoFvPvKPvEDoFvS1_EL0L6copyFnL6deinit", scope: !0, file: !6, line: 2, type: !123, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!123 = !DISubroutineType(types: !124)
!124 = !{!102, !105, !110}
!125 = !DILocation(line: 2, column: 1, scope: !122)
!126 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableD3Ev", linkageName: "_CNW4mainE17ValueWitnessTableD3Ev", scope: null, file: !6, line: 2, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!127 = !DILocation(line: 2, column: 1, scope: !126)
!128 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceD0Ev", linkageName: "_CNW4mainE19ProtocolConformanceD0Ev", scope: !0, file: !6, line: 2, type: !129, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!129 = !DISubroutineType(types: !130)
!130 = !{!131}
!131 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !132, size: 64, align: 8)
!132 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.ProtocolConformance", file: !6, line: 2, flags: DIFlagFwdDecl, elements: !133)
!133 = !{!134, !153}
!134 = !DIDerivedType(tag: DW_TAG_member, name: "typeInfo", scope: !132, file: !6, line: 2, baseType: !135, size: 64, align: 64)
!135 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !136, size: 64, align: 8)
!136 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.TypeInfo", file: !6, line: 2, flags: DIFlagFwdDecl, elements: !137)
!137 = !{!138, !139, !141, !142, !145, !147, !149, !151, !152}
!138 = !DIDerivedType(tag: DW_TAG_member, name: "baseClass", scope: !136, file: !6, line: 2, baseType: !135, size: 64, align: 64)
!139 = !DIDerivedType(tag: DW_TAG_member, name: "vtable", scope: !136, file: !6, line: 2, baseType: !140, size: 64, align: 64, offset: 64)
!140 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !108, size: 64, align: 8)
!141 = !DIDerivedType(tag: DW_TAG_member, name: "deinit", scope: !136, file: !6, line: 2, baseType: !110, size: 64, align: 64, offset: 128)
!142 = !DIDerivedType(tag: DW_TAG_member, name: "name", scope: !136, file: !6, line: 2, baseType: !143, size: 64, align: 64, offset: 192)
!143 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !144, size: 64, align: 8)
!144 = !DIBasicType(name: "u8", size: 8, encoding: DW_ATE_unsigned_char)
!145 = !DIDerivedType(tag: DW_TAG_member, name: "valueWitnessTable", scope: !136, file: !6, line: 2, baseType: !146, size: 64, align: 64, offset: 256)
!146 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !102, size: 64, align: 8)
!147 = !DIDerivedType(tag: DW_TAG_member, name: "conformances", scope: !136, file: !6, line: 2, baseType: !148, size: 64, align: 64, offset: 320)
!148 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !132, size: 64, align: 8)
!149 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !136, file: !6, line: 2, baseType: !150, size: 64, align: 64, offset: 384)
!150 = !DIBasicType(name: "u64", size: 64, encoding: DW_ATE_unsigned)
!151 = !DIDerivedType(tag: DW_TAG_member, name: "alignment", scope: !136, file: !6, line: 2, baseType: !150, size: 64, align: 64, offset: 448)
!152 = !DIDerivedType(tag: DW_TAG_member, name: "stride", scope: !136, file: !6, line: 2, baseType: !150, size: 64, align: 64, offset: 512)
!153 = !DIDerivedType(tag: DW_TAG_member, name: "vtable", scope: !132, file: !6, line: 2, baseType: !140, size: 64, align: 64, offset: 64)
!154 = !DILocation(line: 2, column: 1, scope: !128)
!155 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformance14__default_initES0_L0", linkageName: "_CNW4mainE19ProtocolConformance14__default_initES0_L0", scope: !0, file: !6, line: 2, type: !156, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!156 = !DISubroutineType(types: !157)
!157 = !{!132}
!158 = !DILocation(line: 2, column: 1, scope: !155)
!159 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable", linkageName: "_CNW4mainE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable", scope: !0, file: !6, line: 2, type: !160, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!160 = !DISubroutineType(types: !161)
!161 = !{!131, !135, !140}
!162 = !DILocation(line: 2, column: 1, scope: !159)
!163 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceC2ES0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable", linkageName: "_CNW4mainE19ProtocolConformanceC2ES0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable", scope: !0, file: !6, line: 2, type: !164, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!164 = !DISubroutineType(types: !165)
!165 = !{!132, !135, !140}
!166 = !DILocation(line: 2, column: 1, scope: !163)
!167 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceD3Ev", linkageName: "_CNW4mainE19ProtocolConformanceD3Ev", scope: null, file: !6, line: 2, type: !129, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!168 = !DILocation(line: 2, column: 1, scope: !167)
!169 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoD0Ev", linkageName: "_CNW4mainE8TypeInfoD0Ev", scope: !0, file: !6, line: 2, type: !170, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!170 = !DISubroutineType(types: !171)
!171 = !{!172}
!172 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !136, size: 64, align: 8)
!173 = !DILocation(line: 2, column: 1, scope: !169)
!174 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfo14__default_initES0_L0", linkageName: "_CNW4mainE8TypeInfo14__default_initES0_L0", scope: !0, file: !6, line: 2, type: !175, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!175 = !DISubroutineType(types: !176)
!176 = !{!136}
!177 = !DILocation(line: 2, column: 1, scope: !174)
!178 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", linkageName: "_CNW4mainE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", scope: !0, file: !6, line: 2, type: !179, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!179 = !DISubroutineType(types: !180)
!180 = !{!172, !135, !140, !110, !143, !146, !148, !150, !150, !150}
!181 = !DILocation(line: 2, column: 1, scope: !178)
!182 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoC2ES0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", linkageName: "_CNW4mainE8TypeInfoC2ES0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", scope: !0, file: !6, line: 2, type: !183, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!183 = !DISubroutineType(types: !184)
!184 = !{!136, !135, !140, !110, !143, !146, !148, !150, !150, !150}
!185 = !DILocation(line: 2, column: 1, scope: !182)
!186 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoD3Ev", linkageName: "_CNW4mainE8TypeInfoD3Ev", scope: null, file: !6, line: 2, type: !170, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!187 = !DILocation(line: 2, column: 1, scope: !186)
!188 = distinct !DISubprogram(name: "_CNW4mainE5Int64D0Ev", linkageName: "_CNW4mainE5Int64D0Ev", scope: !0, file: !6, line: 2, type: !189, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!189 = !DISubroutineType(types: !190)
!190 = !{!191}
!191 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !192, size: 64, align: 8)
!192 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.Int64", file: !6, line: 2, flags: DIFlagFwdDecl, elements: !193)
!193 = !{!194}
!194 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !192, file: !6, line: 2, baseType: !9, size: 64, align: 64)
!195 = !DILocation(line: 2, column: 1, scope: !188)
!196 = distinct !DISubprogram(name: "_CNW4mainE5Int644copyES0_L0", linkageName: "_CNW4mainE5Int644copyES0_L0", scope: !0, file: !6, line: 2, type: !197, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!197 = !DISubroutineType(types: !198)
!198 = !{!192}
!199 = !DILocation(line: 2, column: 1, scope: !196)
!200 = distinct !DISubprogram(name: "_CNW4mainE5Int6414__default_initES0_L0", linkageName: "_CNW4mainE5Int6414__default_initES0_L0", scope: !0, file: !6, line: 2, type: !197, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!201 = !DILocation(line: 2, column: 1, scope: !200)
!202 = distinct !DISubprogram(name: "_CNW4mainE5Int64C1ElL5value", linkageName: "_CNW4mainE5Int64C1ElL5value", scope: !0, file: !6, line: 2, type: !203, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!203 = !DISubroutineType(types: !204)
!204 = !{!191, !9}
!205 = !DILocation(line: 2, column: 1, scope: !202)
!206 = distinct !DISubprogram(name: "_CNW4mainE5Int64C2ES0_lL0L5value", linkageName: "_CNW4mainE5Int64C2ES0_lL0L5value", scope: !0, file: !6, line: 2, type: !207, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!207 = !DISubroutineType(types: !208)
!208 = !{!192, !9}
!209 = !DILocation(line: 2, column: 1, scope: !206)
!210 = distinct !DISubprogram(name: "_CNW4mainE5Int64D3Ev", linkageName: "_CNW4mainE5Int64D3Ev", scope: null, file: !6, line: 2, type: !189, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!211 = !DILocation(line: 2, column: 1, scope: !210)
!212 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerD0Ev", linkageName: "_CNW4mainE20ExistentialContainerD0Ev", scope: !0, file: !6, line: 2, type: !213, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!213 = !DISubroutineType(types: !214)
!214 = !{!215}
!215 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !216, size: 64, align: 8)
!216 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.ExistentialContainer", file: !6, line: 2, flags: DIFlagFwdDecl, elements: !217)
!217 = !{!218, !219, !220}
!218 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !216, file: !6, line: 2, baseType: !108, size: 64, align: 64)
!219 = !DIDerivedType(tag: DW_TAG_member, name: "typeInfo", scope: !216, file: !6, line: 2, baseType: !135, size: 64, align: 64, offset: 64)
!220 = !DIDerivedType(tag: DW_TAG_member, name: "conformance", scope: !216, file: !6, line: 2, baseType: !148, size: 64, align: 64, offset: 128)
!221 = !DILocation(line: 2, column: 1, scope: !212)
!222 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainer14__default_initES0_L0", linkageName: "_CNW4mainE20ExistentialContainer14__default_initES0_L0", scope: !0, file: !6, line: 2, type: !223, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!223 = !DISubroutineType(types: !224)
!224 = !{!216}
!225 = !DILocation(line: 2, column: 1, scope: !222)
!226 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance", linkageName: "_CNW4mainE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance", scope: !0, file: !6, line: 2, type: !227, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!227 = !DISubroutineType(types: !228)
!228 = !{!215, !108, !135, !148}
!229 = !DILocation(line: 2, column: 1, scope: !226)
!230 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerC2ES0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance", linkageName: "_CNW4mainE20ExistentialContainerC2ES0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance", scope: !0, file: !6, line: 2, type: !231, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!231 = !DISubroutineType(types: !232)
!232 = !{!216, !108, !135, !148}
!233 = !DILocation(line: 2, column: 1, scope: !230)
!234 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerD3Ev", linkageName: "_CNW4mainE20ExistentialContainerD3Ev", scope: null, file: !6, line: 2, type: !213, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!235 = !DILocation(line: 2, column: 1, scope: !234)
