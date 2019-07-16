; ModuleID = '/Users/Jonas/CDot/StdLib/main/main.dot'
source_filename = "/Users/Jonas/CDot/StdLib/main/main.dot"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-darwin18.6.0"

%"struct.main.<private name>.TypeInfo" = type <{ %"struct.main.<private name>.TypeInfo"*, i8**, void (i8*)*, i8*, %"struct.main.<private name>.ValueWitnessTable"*, %"struct.main.<private name>.ProtocolConformance"*, i64, i64, i64 }>
%"struct.main.<private name>.ValueWitnessTable" = type <{ void (i8*, i8*)*, void (i8*)* }>
%"struct.main.<private name>.ProtocolConformance" = type <{ %"struct.main.<private name>.TypeInfo"*, i8** }>
%"struct.main.<private name>.Int64" = type <{ i64 }>
%"struct.main.Atomic<i64>" = type <{ i64 }>
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
@_CTINW4mainE6AtomicIlEE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.Atomic<i64>"*)* @_CNW4mainE6AtomicIlED0Ev to void (i8*)*), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @6, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"2", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([4 x %"struct.main.<private name>.ProtocolConformance"], [4 x %"struct.main.<private name>.ProtocolConformance"]* @"3", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 8 }> }>, align 8
@"2" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (%"struct.main.Atomic<i64>" (%"struct.main.Atomic<i64>")* @_CNW4mainE6AtomicIlE4copyES0_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (%"struct.main.Atomic<i64>"*)* @_CNW4mainE6AtomicIlED0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE6AtomicIlEENW_0E3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTPNW4mainE6AtomicIlEENW_0E8CopyableE = constant [1 x i8*] [i8* bitcast (%"struct.main.Atomic<i64>" (%"struct.main.Atomic<i64>")* @_CNW4mainE6AtomicIlE4copyES0_L0 to i8*)], align 8
@_CTPNW4mainE6AtomicIlEENW_0E18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"3" = constant [4 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE6AtomicIlEENW_0E3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE6AtomicIlEENW_0E8CopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE6AtomicIlEENW_0E18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE4BoolE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.Bool"*)* @_CNW4mainE4BoolD0Ev to void (i8*)*), i8* getelementptr inbounds ([25 x i8], [25 x i8]* @7, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"6", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([4 x %"struct.main.<private name>.ProtocolConformance"], [4 x %"struct.main.<private name>.ProtocolConformance"]* @"7", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 1 }>, %"struct.main.<private name>.Int64" <{ i64 1 }>, %"struct.main.<private name>.Int64" <{ i64 1 }> }>, align 8
@"6" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (%"struct.main.<private name>.Bool" (%"struct.main.<private name>.Bool")* @_CNW4mainE4Bool4copyES0_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (%"struct.main.<private name>.Bool"*)* @_CNW4mainE4BoolD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE4BoolENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTPNW4mainE4BoolENS_8CopyableE = constant [1 x i8*] [i8* bitcast (%"struct.main.<private name>.Bool" (%"struct.main.<private name>.Bool")* @_CNW4mainE4Bool4copyES0_L0 to i8*)], align 8
@_CTPNW4mainE4BoolENS_18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"7" = constant [4 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE4BoolENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE4BoolENS_8CopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE4BoolENS_18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE17ValueWitnessTableE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.ValueWitnessTable"*)* @_CNW4mainE17ValueWitnessTableD0Ev to void (i8*)*), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @8, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"8", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"9", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 16 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 16 }> }>, align 8
@"8" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.ValueWitnessTable"*)* @_CNW4mainE17ValueWitnessTableD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE17ValueWitnessTableENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"9" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE17ValueWitnessTableENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE19ProtocolConformanceE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.ProtocolConformance"*)* @_CNW4mainE19ProtocolConformanceD0Ev to void (i8*)*), i8* getelementptr inbounds ([40 x i8], [40 x i8]* @9, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"10", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"11", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 16 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 16 }> }>, align 8
@"10" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.ProtocolConformance"*)* @_CNW4mainE19ProtocolConformanceD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE19ProtocolConformanceENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"11" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE19ProtocolConformanceENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE8TypeInfoE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.TypeInfo"*)* @_CNW4mainE8TypeInfoD0Ev to void (i8*)*), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @10, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"12", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"13", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 72 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 72 }> }>, align 8
@"12" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.TypeInfo"*)* @_CNW4mainE8TypeInfoD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE8TypeInfoENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"13" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE8TypeInfoENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE5Int64E = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.Int64"*)* @_CNW4mainE5Int64D0Ev to void (i8*)*), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @11, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"14", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([4 x %"struct.main.<private name>.ProtocolConformance"], [4 x %"struct.main.<private name>.ProtocolConformance"]* @"15", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 8 }> }>, align 8
@"14" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (%"struct.main.<private name>.Int64" (%"struct.main.<private name>.Int64")* @_CNW4mainE5Int644copyES0_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (%"struct.main.<private name>.Int64"*)* @_CNW4mainE5Int64D0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE5Int64ENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTPNW4mainE5Int64ENS_8CopyableE = constant [1 x i8*] [i8* bitcast (%"struct.main.<private name>.Int64" (%"struct.main.<private name>.Int64")* @_CNW4mainE5Int644copyES0_L0 to i8*)], align 8
@_CTPNW4mainE5Int64ENS_18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"15" = constant [4 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE5Int64ENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE5Int64ENS_8CopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE5Int64ENS_18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE20ExistentialContainerE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.ExistentialContainer"*)* @_CNW4mainE20ExistentialContainerD0Ev to void (i8*)*), i8* getelementptr inbounds ([41 x i8], [41 x i8]* @12, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"16", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"17", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@"16" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.ExistentialContainer"*)* @_CNW4mainE20ExistentialContainerD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE20ExistentialContainerENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"17" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE20ExistentialContainerENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@0 = private unnamed_addr constant [17 x i8] c"main.MemoryOrder\00"
@1 = private unnamed_addr constant [24 x i8] c"main.<private name>.Any\00"
@2 = private unnamed_addr constant [30 x i8] c"main.<private name>.Equatable\00"
@3 = private unnamed_addr constant [37 x i8] c"main.<private name>.RawRepresentable\00"
@4 = private unnamed_addr constant [29 x i8] c"main.<private name>.Copyable\00"
@5 = private unnamed_addr constant [39 x i8] c"main.<private name>.ImplicitlyCopyable\00"
@6 = private unnamed_addr constant [17 x i8] c"main.Atomic<i64>\00"
@"4.1" = private global %"struct.main.<private name>.TypeInfo"* undef
@"5.2" = private global %"struct.main.<private name>.TypeInfo"* undef
@7 = private unnamed_addr constant [25 x i8] c"main.<private name>.Bool\00"
@8 = private unnamed_addr constant [38 x i8] c"main.<private name>.ValueWitnessTable\00"
@9 = private unnamed_addr constant [40 x i8] c"main.<private name>.ProtocolConformance\00"
@10 = private unnamed_addr constant [29 x i8] c"main.<private name>.TypeInfo\00"
@11 = private unnamed_addr constant [26 x i8] c"main.<private name>.Int64\00"
@12 = private unnamed_addr constant [41 x i8] c"main.<private name>.ExistentialContainer\00"
@13 = private unnamed_addr constant [13 x i8] c"value: %llu\0A\00"
@14 = private unnamed_addr constant [40 x i8] c"memory order %d is not valid for store\0A\00"
@15 = private unnamed_addr constant [39 x i8] c"memory order %d is not valid for load\0A\00"

define void @_CNW4mainE6_startEv() !dbg !5 {
entry:
  %0 = alloca i64, align 8
  %1 = alloca %"struct.main.Atomic<i64>", align 8
  %2 = alloca %"struct.main.Atomic<i64>", align 8
  call void @_CNW4mainE6AtomicIlEC1ElL10weirdValue(%"struct.main.Atomic<i64>"* %2, i64 12), !dbg !8
  %3 = load %"struct.main.Atomic<i64>", %"struct.main.Atomic<i64>"* %2, !dbg !8
  call void @llvm.dbg.value(metadata %"struct.main.Atomic<i64>" %3, metadata !9, metadata !DIExpression()), !dbg !8
  call void @llvm.dbg.declare(metadata %"struct.main.Atomic<i64>"* %1, metadata !9, metadata !DIExpression()), !dbg !8
  store %"struct.main.Atomic<i64>" %3, %"struct.main.Atomic<i64>"* %1, align 8, !dbg !8
  %4 = bitcast %"struct.main.Atomic<i64>"* %1 to i8*, !dbg !8
  call void @llvm.lifetime.start.p0i8(i64 8, i8* %4), !dbg !8
  call void @_CNW4mainE6AtomicIlE5storeERS0_lNW_0E11MemoryOrderEL0L0L11memoryOrder(%"struct.main.Atomic<i64>"* %1, i64 69, i32 6), !dbg !15
  %5 = load %"struct.main.Atomic<i64>", %"struct.main.Atomic<i64>"* %1, align 8, !dbg !16
  %6 = call i64 @_CNW4mainE6AtomicIlE4loadES0_NW_0E11MemoryOrderEL0L11memoryOrder(%"struct.main.Atomic<i64>" %5, i32 6), !dbg !16
  call void @llvm.dbg.value(metadata i64 %6, metadata !17, metadata !DIExpression()), !dbg !16
  call void @llvm.dbg.declare(metadata i64* %0, metadata !17, metadata !DIExpression()), !dbg !16
  store i64 %6, i64* %0, align 8, !dbg !16
  %7 = bitcast i64* %0 to i8*, !dbg !16
  call void @llvm.lifetime.start.p0i8(i64 8, i8* %7), !dbg !16
  %8 = load i64, i64* %0, align 8, !dbg !18
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @13, i32 0, i32 0), i64 %8), !dbg !19
  %10 = bitcast i64* %0 to i8*, !dbg !19
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %10), !dbg !19
  %11 = bitcast %"struct.main.Atomic<i64>"* %1 to i8*, !dbg !19
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %11), !dbg !19
  call void @_CNW4mainE6AtomicIlED0Ev(%"struct.main.Atomic<i64>"* %1), !dbg !19
  ret void, !dbg !19
}

define i32 @main() !dbg !20 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE6_startEv(), !dbg !21
  ret i32 0, !dbg !21
}

define void @_CNW4mainE6AtomicIlEC1ElL10weirdValue(%"struct.main.Atomic<i64>"*, i64) !dbg !22 {
alloca_block:
  %2 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.main.Atomic<i64>", %"struct.main.Atomic<i64>"* %0, align 8, !dbg !26
  call void @_CNW4mainE6AtomicIlE14__default_initES0_L0(%"struct.main.Atomic<i64>" %3), !dbg !26
  call void @llvm.dbg.declare(metadata %"struct.main.Atomic<i64>"* %0, metadata !27, metadata !DIExpression()), !dbg !26
  call void @llvm.dbg.declare(metadata i64* %2, metadata !28, metadata !DIExpression()), !dbg !26
  call void @llvm.dbg.value(metadata i64 %1, metadata !28, metadata !DIExpression()), !dbg !26
  store i64 %1, i64* %2, align 8, !dbg !26
  %4 = load i64, i64* %2, align 8, !dbg !29
  call void @_CNW4mainE6AtomicIlEC2ERS0_lL0L5value(%"struct.main.Atomic<i64>"* %0, i64 %4), !dbg !30
  %5 = bitcast i64* %2 to i8*, !dbg !30
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %5), !dbg !30
  ret void, !dbg !30
}

define void @_CNW4mainE6AtomicIlE5storeERS0_lNW_0E11MemoryOrderEL0L0L11memoryOrder(%"struct.main.Atomic<i64>"*, i64, i32) !dbg !31 {
entry:
  %3 = alloca i64*, align 8
  %4 = alloca i32, align 4
  %5 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata %"struct.main.Atomic<i64>"* %0, metadata !43, metadata !DIExpression()), !dbg !44
  call void @llvm.dbg.declare(metadata i64* %5, metadata !45, metadata !DIExpression()), !dbg !44
  call void @llvm.dbg.value(metadata i64 %1, metadata !45, metadata !DIExpression()), !dbg !44
  store i64 %1, i64* %5, align 8, !dbg !44
  call void @llvm.dbg.declare(metadata i32* %4, metadata !46, metadata !DIExpression()), !dbg !44
  call void @llvm.dbg.value(metadata i32 %2, metadata !46, metadata !DIExpression()), !dbg !44
  store i32 %2, i32* %4, align 4, !dbg !44
  %6 = load i32, i32* %4, align 4, !dbg !47
  call void @_CNW4mainE6AtomicIlE20checkValidStoreOrderEMS0_NW_0E11MemoryOrderEL0L0(%"struct.main.<private name>.TypeInfo"* bitcast (%"struct.main.<private name>.TypeInfo"** @"4.1" to %"struct.main.<private name>.TypeInfo"*), i32 %6), !dbg !48
  %7 = bitcast %"struct.main.Atomic<i64>"* %0 to i64*
  call void @llvm.dbg.value(metadata i64* %7, metadata !49, metadata !DIExpression()), !dbg !51
  %8 = load i64, i64* %7, align 8, !dbg !51
  call void @llvm.dbg.declare(metadata i64** %3, metadata !49, metadata !DIExpression()), !dbg !51
  store i64* %7, i64** %3, align 8, !dbg !51
  %9 = bitcast i64** %3 to i8*, !dbg !51
  call void @llvm.lifetime.start.p0i8(i64 8, i8* %9), !dbg !51
  %10 = load i64, i64* %5, align 8, !dbg !52
  %11 = load i64*, i64** %3, align 8, !dbg !52
  %12 = load i32, i32* %4, align 4, !dbg !52
  %13 = call i32 @_CNW4mainE11MemoryOrderGa8rawValueES_L0(i32 %12), !dbg !52
  switch i32 %13, label %unreachable [
    i32 2, label %atomic.store.relaxed
    i32 5, label %atomic.store.release
    i32 7, label %atomic.store.seq_cst
  ], !dbg !52

unreachable:                                      ; preds = %entry
  unreachable, !dbg !52

atomic.store.relaxed:                             ; preds = %entry
  store atomic i64 %10, i64* %11 monotonic, align 8, !dbg !52
  br label %atomic.store.merge, !dbg !52

atomic.store.merge:                               ; preds = %atomic.store.seq_cst, %atomic.store.release, %atomic.store.relaxed
  %14 = bitcast i64** %3 to i8*, !dbg !52
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %14), !dbg !52
  %15 = bitcast i64* %5 to i8*, !dbg !52
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %15), !dbg !52
  ret void, !dbg !52

atomic.store.release:                             ; preds = %entry
  store atomic i64 %10, i64* %11 release, align 8, !dbg !52
  br label %atomic.store.merge, !dbg !52

atomic.store.seq_cst:                             ; preds = %entry
  store atomic i64 %10, i64* %11 seq_cst, align 8, !dbg !52
  br label %atomic.store.merge, !dbg !52
}

define i64 @_CNW4mainE6AtomicIlE4loadES0_NW_0E11MemoryOrderEL0L11memoryOrder(%"struct.main.Atomic<i64>", i32) !dbg !53 {
entry:
  %2 = alloca i64*, align 8
  %3 = alloca i32, align 4
  %4 = alloca %"struct.main.Atomic<i64>", align 8
  call void @llvm.dbg.declare(metadata %"struct.main.Atomic<i64>"* %4, metadata !56, metadata !DIExpression()), !dbg !57
  call void @llvm.dbg.value(metadata %"struct.main.Atomic<i64>" %0, metadata !56, metadata !DIExpression()), !dbg !57
  store %"struct.main.Atomic<i64>" %0, %"struct.main.Atomic<i64>"* %4, align 8, !dbg !57
  call void @llvm.dbg.declare(metadata i32* %3, metadata !58, metadata !DIExpression()), !dbg !57
  call void @llvm.dbg.value(metadata i32 %1, metadata !58, metadata !DIExpression()), !dbg !57
  store i32 %1, i32* %3, align 4, !dbg !57
  %5 = load i32, i32* %3, align 4, !dbg !59
  call void @_CNW4mainE6AtomicIlE19checkValidLoadOrderEMS0_NW_0E11MemoryOrderEL0L0(%"struct.main.<private name>.TypeInfo"* bitcast (%"struct.main.<private name>.TypeInfo"** @"5.2" to %"struct.main.<private name>.TypeInfo"*), i32 %5), !dbg !60
  %6 = bitcast %"struct.main.Atomic<i64>"* %4 to i64*
  call void @llvm.dbg.value(metadata i64* %6, metadata !61, metadata !DIExpression()), !dbg !62
  %7 = load i64, i64* %6, align 8, !dbg !62
  call void @llvm.dbg.declare(metadata i64** %2, metadata !61, metadata !DIExpression()), !dbg !62
  store i64* %6, i64** %2, align 8, !dbg !62
  %8 = bitcast i64** %2 to i8*, !dbg !62
  call void @llvm.lifetime.start.p0i8(i64 8, i8* %8), !dbg !62
  %9 = load i64*, i64** %2, align 8, !dbg !63
  %10 = load i32, i32* %3, align 4, !dbg !63
  %11 = call i32 @_CNW4mainE11MemoryOrderGa8rawValueES_L0(i32 %10), !dbg !63
  switch i32 %11, label %unreachable [
    i32 2, label %atomic.load.relaxed
    i32 3, label %atomic.load.consume
    i32 4, label %atomic.load.acquire
    i32 7, label %atomic.load.seq_cst
  ], !dbg !63

unreachable:                                      ; preds = %entry
  unreachable, !dbg !63

atomic.load.relaxed:                              ; preds = %entry
  %12 = load atomic i64, i64* %9 monotonic, align 8, !dbg !63
  br label %atomic.load.merge, !dbg !63

atomic.load.merge:                                ; preds = %atomic.load.seq_cst, %atomic.load.acquire, %atomic.load.consume, %atomic.load.relaxed
  %13 = phi i64 [ %12, %atomic.load.relaxed ], [ %15, %atomic.load.consume ], [ %16, %atomic.load.acquire ], [ %17, %atomic.load.seq_cst ]
  %14 = bitcast i64** %2 to i8*, !dbg !63
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %14), !dbg !63
  ret i64 %13, !dbg !63

atomic.load.consume:                              ; preds = %entry
  %15 = load atomic i64, i64* %9 consume, align 8, !dbg !63
  br label %atomic.load.merge, !dbg !63

atomic.load.acquire:                              ; preds = %entry
  %16 = load atomic i64, i64* %9 acquire, align 8, !dbg !63
  br label %atomic.load.merge, !dbg !63

atomic.load.seq_cst:                              ; preds = %entry
  %17 = load atomic i64, i64* %9 seq_cst, align 8, !dbg !63
  br label %atomic.load.merge, !dbg !63
}

declare i32 @printf(i8*, ...)

define void @_CNW4mainE6AtomicIlED0Ev(%"struct.main.Atomic<i64>"*) !dbg !64 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE6AtomicIlED3Ev(%"struct.main.Atomic<i64>"* %0), !dbg !67
  ret void, !dbg !67
}

define void @_CNW4mainE11MemoryOrderD0Ev(i32*) !dbg !68 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE11MemoryOrderD3Ev(i32* %0), !dbg !72
  ret void, !dbg !72
}

define i32 @_CNW4mainE11MemoryOrder4copyES_L0(i32) !dbg !73 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret i32 %0, !dbg !76
}

define %"struct.main.<private name>.Bool" @"_CNW4mainE11MemoryOrderXi2==ES_S_L0L0"(i32, i32) !dbg !77 {
alloca_block:
  %2 = alloca %"struct.main.<private name>.Bool", align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = icmp eq i32 %0, %1, !dbg !80
  %4 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %2, align 1, !dbg !80
  %5 = extractvalue %"struct.main.<private name>.Bool" %4, 0, !dbg !80
  %6 = alloca i1, !dbg !80
  store i1 %5, i1* %6, !dbg !80
  store i1 %3, i1* %6, !dbg !80
  ret %"struct.main.<private name>.Bool" %4, !dbg !80
}

define %"struct.main.<private name>.Bool" @"_CNW4mainE11MemoryOrderXi2!=ES_S_L0L0"(i32, i32) !dbg !81 {
alloca_block:
  %2 = alloca %"struct.main.<private name>.Bool", align 1
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !82, metadata !DIExpression()), !dbg !83
  call void @llvm.dbg.value(metadata i32 %0, metadata !82, metadata !DIExpression()), !dbg !83
  store i32 %0, i32* %4, align 4, !dbg !83
  call void @llvm.dbg.declare(metadata i32* %3, metadata !84, metadata !DIExpression()), !dbg !83
  call void @llvm.dbg.value(metadata i32 %1, metadata !84, metadata !DIExpression()), !dbg !83
  store i32 %1, i32* %3, align 4, !dbg !83
  %5 = load i32, i32* %4, align 4, !dbg !83
  %6 = load i32, i32* %3, align 4, !dbg !83
  %7 = call %"struct.main.<private name>.Bool" @"_CNW4mainE11MemoryOrderXi2==ES_S_L0L0"(i32 %5, i32 %6), !dbg !83
  %8 = call %"struct.main.<private name>.Bool" @"_CNW4mainE4BoolXp1!ES0_L0"(%"struct.main.<private name>.Bool" %7), !dbg !83
  store %"struct.main.<private name>.Bool" %7, %"struct.main.<private name>.Bool"* %2, align 1, !dbg !83
  call void @_CNW4mainE4BoolD0Ev(%"struct.main.<private name>.Bool"* %2), !dbg !83
  ret %"struct.main.<private name>.Bool" %8, !dbg !83
}

define void @_CNW4mainE11MemoryOrderC1EiL8rawValue(i32*, i32) !dbg !85 {
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
  ], !dbg !89

init.failure:                                     ; preds = %entry
  %9 = load i32, i32* %0, align 4, !dbg !89
  %10 = bitcast i32* %0 to i8*, !dbg !89
  call void @llvm.memset.p0i8.i64(i8* %10, i8 0, i64 8, i32 1, i1 false), !dbg !89
  ret void, !dbg !89

init.case.NotAtomic:                              ; preds = %entry
  store i32 0, i32* %8, align 4, !dbg !89
  %11 = bitcast i32* %0 to i8*, !dbg !89
  %12 = bitcast i32* %8 to i8*, !dbg !89
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %11, i8* %12, i64 8, i32 1, i1 false), !dbg !89
  ret void, !dbg !89

init.case.Relaxed:                                ; preds = %entry
  store i32 1, i32* %7, align 4, !dbg !89
  %13 = bitcast i32* %0 to i8*, !dbg !89
  %14 = bitcast i32* %7 to i8*, !dbg !89
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %13, i8* %14, i64 8, i32 1, i1 false), !dbg !89
  ret void, !dbg !89

init.case.Consume:                                ; preds = %entry
  store i32 2, i32* %6, align 4, !dbg !89
  %15 = bitcast i32* %0 to i8*, !dbg !89
  %16 = bitcast i32* %6 to i8*, !dbg !89
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %15, i8* %16, i64 8, i32 1, i1 false), !dbg !89
  ret void, !dbg !89

init.case.Acquire:                                ; preds = %entry
  store i32 3, i32* %5, align 4, !dbg !89
  %17 = bitcast i32* %0 to i8*, !dbg !89
  %18 = bitcast i32* %5 to i8*, !dbg !89
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %17, i8* %18, i64 8, i32 1, i1 false), !dbg !89
  ret void, !dbg !89

init.case.Release:                                ; preds = %entry
  store i32 4, i32* %4, align 4, !dbg !89
  %19 = bitcast i32* %0 to i8*, !dbg !89
  %20 = bitcast i32* %4 to i8*, !dbg !89
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %19, i8* %20, i64 8, i32 1, i1 false), !dbg !89
  ret void, !dbg !89

init.case.AcquireRelease:                         ; preds = %entry
  store i32 5, i32* %3, align 4, !dbg !89
  %21 = bitcast i32* %0 to i8*, !dbg !89
  %22 = bitcast i32* %3 to i8*, !dbg !89
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %21, i8* %22, i64 8, i32 1, i1 false), !dbg !89
  ret void, !dbg !89

init.case.SequentiallyConsistent:                 ; preds = %entry
  store i32 6, i32* %2, align 4, !dbg !89
  %23 = bitcast i32* %0 to i8*, !dbg !89
  %24 = bitcast i32* %2 to i8*, !dbg !89
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %23, i8* %24, i64 8, i32 1, i1 false), !dbg !89
  ret void, !dbg !89
}

define i32 @_CNW4mainE11MemoryOrderGa8rawValueES_L0(i32) !dbg !90 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret i32 %0, !dbg !91
}

define void @_CNW4mainE11MemoryOrderD3Ev(i32*) !dbg !92 {
merge:
  ret void, !dbg !93
}

define %"struct.main.<private name>.Bool" @"_CNW4mainE4BoolXp1!ES0_L0"(%"struct.main.<private name>.Bool") !dbg !94 {
entry:
  %1 = alloca %"struct.main.<private name>.Bool", align 1
  %2 = alloca %"struct.main.<private name>.Bool", align 1
  call void @llvm.dbg.declare(metadata %"struct.main.<private name>.Bool"* %2, metadata !101, metadata !DIExpression()), !dbg !102
  call void @llvm.dbg.value(metadata %"struct.main.<private name>.Bool" %0, metadata !101, metadata !DIExpression()), !dbg !102
  store %"struct.main.<private name>.Bool" %0, %"struct.main.<private name>.Bool"* %2, align 1, !dbg !102
  %3 = bitcast %"struct.main.<private name>.Bool"* %2 to i1*
  %4 = load i1, i1* %3, align 1, !dbg !102
  %5 = xor i1 %4, true, !dbg !102
  call void @_CNW4mainE4BoolC1EbL5value(%"struct.main.<private name>.Bool"* %1, i1 %5), !dbg !102
  %6 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %1, !dbg !102
  ret %"struct.main.<private name>.Bool" %6, !dbg !102
}

define void @_CNW4mainE4BoolD0Ev(%"struct.main.<private name>.Bool"*) !dbg !103 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE4BoolD3Ev(%"struct.main.<private name>.Bool"* %0), !dbg !107
  ret void, !dbg !107
}

declare void @_CNW4mainE11MemoryOrderC2ERS_iL0L8rawValue(i32*, i32)

define %"struct.main.Atomic<i64>" @_CNW4mainE6AtomicIlE4copyES0_L0(%"struct.main.Atomic<i64>") !dbg !108 {
alloca_block:
  %1 = alloca %"struct.main.Atomic<i64>", align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.Atomic<i64>", %"struct.main.Atomic<i64>"* %1, align 8, !dbg !111
  %3 = extractvalue %"struct.main.Atomic<i64>" %2, 0, !dbg !111
  %4 = alloca i64, !dbg !111
  store i64 %3, i64* %4, !dbg !111
  %5 = extractvalue %"struct.main.Atomic<i64>" %0, 0, !dbg !111
  %6 = alloca i64, !dbg !111
  store i64 %5, i64* %6, !dbg !111
  %7 = load i64, i64* %6, !dbg !111
  store i64 %7, i64* %4, !dbg !111
  ret %"struct.main.Atomic<i64>" %2, !dbg !111
}

define void @_CNW4mainE6AtomicIlE14__default_initES0_L0(%"struct.main.Atomic<i64>") !dbg !112 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !113
}

define void @_CNW4mainE6AtomicIlED3Ev(%"struct.main.Atomic<i64>"*) !dbg !114 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.Atomic<i64>", %"struct.main.Atomic<i64>"* %0, align 8, !dbg !115
  %2 = extractvalue %"struct.main.Atomic<i64>" %1, 0, !dbg !115
  %3 = alloca i64, !dbg !115
  store i64 %2, i64* %3, !dbg !115
  %4 = bitcast i64* %3 to i8*, !dbg !115
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %4), !dbg !115
  ret void, !dbg !115
}

define void @_CNW4mainE6AtomicIlEC2ERS0_lL0L5value(%"struct.main.Atomic<i64>"*, i64) !dbg !116 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata %"struct.main.Atomic<i64>"* %0, metadata !117, metadata !DIExpression()), !dbg !118
  call void @llvm.dbg.declare(metadata i64* %2, metadata !119, metadata !DIExpression()), !dbg !118
  call void @llvm.dbg.value(metadata i64 %1, metadata !119, metadata !DIExpression()), !dbg !118
  store i64 %1, i64* %2, align 8, !dbg !118
  %3 = bitcast %"struct.main.Atomic<i64>"* %0 to i64*
  %4 = load i64, i64* %2, align 8, !dbg !120
  %5 = bitcast i64* %3 to i8*, !dbg !120
  call void @llvm.lifetime.start.p0i8(i64 8, i8* %5), !dbg !120
  store i64 %4, i64* %3, align 8, !dbg !120
  %6 = bitcast i64* %2 to i8*, !dbg !120
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %6), !dbg !120
  ret void, !dbg !120
}

declare void @_CNW4mainE6AtomicIlEC1ElL5value(%"struct.main.Atomic<i64>"*, i64)

define void @_CNW4mainE6AtomicIlEC2ERS0_lL0L10weirdValue(%"struct.main.Atomic<i64>"*, i64) !dbg !121 {
alloca_block:
  %2 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata %"struct.main.Atomic<i64>"* %0, metadata !122, metadata !DIExpression()), !dbg !123
  call void @llvm.dbg.declare(metadata i64* %2, metadata !124, metadata !DIExpression()), !dbg !123
  call void @llvm.dbg.value(metadata i64 %1, metadata !124, metadata !DIExpression()), !dbg !123
  store i64 %1, i64* %2, align 8, !dbg !123
  %3 = load i64, i64* %2, align 8, !dbg !125
  call void @_CNW4mainE6AtomicIlEC2ERS0_lL0L5value(%"struct.main.Atomic<i64>"* %0, i64 %3), !dbg !126
  %4 = bitcast i64* %2 to i8*, !dbg !126
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %4), !dbg !126
  ret void, !dbg !126
}

define void @_CNW4mainE6AtomicIlE20checkValidStoreOrderEMS0_NW_0E11MemoryOrderEL0L0(%"struct.main.<private name>.TypeInfo"*, i32) !dbg !127 {
match.cmp:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata %"struct.main.<private name>.TypeInfo"* %0, metadata !134, metadata !DIExpression()), !dbg !135
  call void @llvm.dbg.declare(metadata i32* %2, metadata !136, metadata !DIExpression()), !dbg !135
  call void @llvm.dbg.value(metadata i32 %1, metadata !136, metadata !DIExpression()), !dbg !135
  store i32 %1, i32* %2, align 4, !dbg !135
  %3 = load i32, i32* %2, align 4, !dbg !137
  %4 = call i1 @"_CNW4mainEXi2~=INW_0E11MemoryOrderEEEbNW_0E11MemoryOrderENW_0E11MemoryOrderEL0L0"(i32 %3, i32 2), !dbg !137
  br i1 %4, label %match.merge, label %match.cmp.0, !dbg !137

match.cmp.0:                                      ; preds = %match.cmp
  %5 = call i1 @"_CNW4mainEXi2~=INW_0E11MemoryOrderEEEbNW_0E11MemoryOrderENW_0E11MemoryOrderEL0L0"(i32 %3, i32 3), !dbg !138
  br i1 %5, label %match.merge, label %match.cmp.1, !dbg !138

match.merge:                                      ; preds = %match.cmp.2, %match.cmp.0, %match.cmp, %match.cmp.1
  %6 = load i32, i32* %2, align 4, !dbg !139
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([40 x i8], [40 x i8]* @14, i32 0, i32 0), i32 %6), !dbg !140
  ret void, !dbg !138

match.cmp.1:                                      ; preds = %match.cmp.0
  %8 = call i1 @"_CNW4mainEXi2~=INW_0E11MemoryOrderEEEbNW_0E11MemoryOrderENW_0E11MemoryOrderEL0L0"(i32 %3, i32 5), !dbg !138
  br i1 %8, label %match.merge, label %match.cmp.2, !dbg !138

match.cmp.2:                                      ; preds = %match.cmp.1
  %9 = call i1 @"_CNW4mainEXi2~=INW_0E11MemoryOrderEEEbNW_0E11MemoryOrderENW_0E11MemoryOrderEL0L0"(i32 %3, i32 0), !dbg !138
  br i1 %9, label %match.merge, label %match.body.default, !dbg !138

match.body.default:                               ; preds = %match.cmp.2
  ret void, !dbg !141
}

define void @_CNW4mainE6AtomicIlE19checkValidLoadOrderEMS0_NW_0E11MemoryOrderEL0L0(%"struct.main.<private name>.TypeInfo"*, i32) !dbg !142 {
match.cmp:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata %"struct.main.<private name>.TypeInfo"* %0, metadata !143, metadata !DIExpression()), !dbg !144
  call void @llvm.dbg.declare(metadata i32* %2, metadata !145, metadata !DIExpression()), !dbg !144
  call void @llvm.dbg.value(metadata i32 %1, metadata !145, metadata !DIExpression()), !dbg !144
  store i32 %1, i32* %2, align 4, !dbg !144
  %3 = load i32, i32* %2, align 4, !dbg !146
  %4 = call i1 @"_CNW4mainEXi2~=INW_0E11MemoryOrderEEEbNW_0E11MemoryOrderENW_0E11MemoryOrderEL0L0"(i32 %3, i32 4), !dbg !146
  br i1 %4, label %match.merge, label %match.cmp.0, !dbg !146

match.cmp.0:                                      ; preds = %match.cmp
  %5 = call i1 @"_CNW4mainEXi2~=INW_0E11MemoryOrderEEEbNW_0E11MemoryOrderENW_0E11MemoryOrderEL0L0"(i32 %3, i32 5), !dbg !147
  br i1 %5, label %match.merge, label %match.cmp.1, !dbg !147

match.merge:                                      ; preds = %match.cmp.1, %match.cmp, %match.cmp.0
  %6 = load i32, i32* %2, align 4, !dbg !148
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([39 x i8], [39 x i8]* @15, i32 0, i32 0), i32 %6), !dbg !149
  ret void, !dbg !147

match.cmp.1:                                      ; preds = %match.cmp.0
  %8 = call i1 @"_CNW4mainEXi2~=INW_0E11MemoryOrderEEEbNW_0E11MemoryOrderENW_0E11MemoryOrderEL0L0"(i32 %3, i32 0), !dbg !147
  br i1 %8, label %match.merge, label %match.body.default, !dbg !147

match.body.default:                               ; preds = %match.cmp.1
  ret void, !dbg !150
}

define i1 @"_CNW4mainEXi2~=INW_0E11MemoryOrderEEEbNW_0E11MemoryOrderENW_0E11MemoryOrderEL0L0"(i32, i32) !dbg !151 {
entry:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %5, metadata !152, metadata !DIExpression()), !dbg !153
  call void @llvm.dbg.value(metadata i32 %0, metadata !152, metadata !DIExpression()), !dbg !153
  store i32 %0, i32* %5, align 4, !dbg !153
  call void @llvm.dbg.declare(metadata i32* %4, metadata !154, metadata !DIExpression()), !dbg !153
  call void @llvm.dbg.value(metadata i32 %1, metadata !154, metadata !DIExpression()), !dbg !153
  store i32 %1, i32* %4, align 4, !dbg !153
  %6 = load i32, i32* %5, align 4, !dbg !155
  %7 = bitcast i32* %5 to i64*, !dbg !155
  %8 = load i64, i64* %7, align 8, !dbg !155
  call void @llvm.dbg.value(metadata i64 %8, metadata !156, metadata !DIExpression()), !dbg !155
  call void @llvm.dbg.declare(metadata i64* %3, metadata !156, metadata !DIExpression()), !dbg !155
  store i64 %8, i64* %3, align 8, !dbg !155
  %9 = bitcast i64* %3 to i8*, !dbg !155
  call void @llvm.lifetime.start.p0i8(i64 8, i8* %9), !dbg !155
  %10 = load i32, i32* %4, align 4, !dbg !157
  %11 = bitcast i32* %4 to i64*, !dbg !157
  %12 = load i64, i64* %11, align 8, !dbg !157
  call void @llvm.dbg.value(metadata i64 %12, metadata !158, metadata !DIExpression()), !dbg !157
  call void @llvm.dbg.declare(metadata i64* %2, metadata !158, metadata !DIExpression()), !dbg !157
  store i64 %12, i64* %2, align 8, !dbg !157
  %13 = bitcast i64* %2 to i8*, !dbg !157
  call void @llvm.lifetime.start.p0i8(i64 8, i8* %13), !dbg !157
  %14 = load i64, i64* %3, align 8, !dbg !159
  %15 = load i64, i64* %2, align 8, !dbg !160
  %16 = call i1 @"_CNW7builtin3opsEXi2==EllL0L0"(i64 %14, i64 %15), !dbg !159
  %17 = bitcast i64* %2 to i8*, !dbg !159
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %17), !dbg !159
  %18 = bitcast i64* %3 to i8*, !dbg !159
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %18), !dbg !159
  ret i1 %16, !dbg !159
}

declare i1 @"_CNW7builtin3opsEXi2==EllL0L0"(i64, i64)

define %"struct.main.<private name>.Bool" @_CNW4mainE4Bool4copyES0_L0(%"struct.main.<private name>.Bool") !dbg !161 {
alloca_block:
  %1 = alloca %"struct.main.<private name>.Bool", align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %1, align 1, !dbg !162
  %3 = extractvalue %"struct.main.<private name>.Bool" %2, 0, !dbg !162
  %4 = alloca i1, !dbg !162
  store i1 %3, i1* %4, !dbg !162
  %5 = extractvalue %"struct.main.<private name>.Bool" %0, 0, !dbg !162
  %6 = alloca i1, !dbg !162
  store i1 %5, i1* %6, !dbg !162
  %7 = load i1, i1* %6, !dbg !162
  store i1 %7, i1* %4, !dbg !162
  ret %"struct.main.<private name>.Bool" %2, !dbg !162
}

define void @_CNW4mainE4Bool14__default_initES0_L0(%"struct.main.<private name>.Bool") !dbg !163 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !164
}

define void @_CNW4mainE4BoolC1EbL5value(%"struct.main.<private name>.Bool"*, i1) !dbg !165 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %0, align 1, !dbg !168
  call void @_CNW4mainE4Bool14__default_initES0_L0(%"struct.main.<private name>.Bool" %2), !dbg !168
  %3 = extractvalue %"struct.main.<private name>.Bool" %2, 0, !dbg !168
  %4 = alloca i1, !dbg !168
  store i1 %3, i1* %4, !dbg !168
  store i1 %1, i1* %4, !dbg !168
  ret void, !dbg !168
}

define void @_CNW4mainE4BoolC2ERS0_bL0L5value(%"struct.main.<private name>.Bool"*, i1) !dbg !169 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %0, align 1, !dbg !170
  %3 = extractvalue %"struct.main.<private name>.Bool" %2, 0, !dbg !170
  %4 = alloca i1, !dbg !170
  store i1 %3, i1* %4, !dbg !170
  store i1 %1, i1* %4, !dbg !170
  ret void, !dbg !170
}

define void @_CNW4mainE4BoolD3Ev(%"struct.main.<private name>.Bool"*) !dbg !171 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %0, align 1, !dbg !172
  %2 = extractvalue %"struct.main.<private name>.Bool" %1, 0, !dbg !172
  %3 = alloca i1, !dbg !172
  store i1 %2, i1* %3, !dbg !172
  %4 = bitcast i1* %3 to i8*, !dbg !172
  call void @llvm.lifetime.end.p0i8(i64 1, i8* %4), !dbg !172
  ret void, !dbg !172
}

define void @_CNW4mainE17ValueWitnessTableD0Ev(%"struct.main.<private name>.ValueWitnessTable"*) !dbg !173 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE17ValueWitnessTableD3Ev(%"struct.main.<private name>.ValueWitnessTable"* %0), !dbg !188
  ret void, !dbg !188
}

define void @_CNW4mainE17ValueWitnessTable14__default_initES0_L0(%"struct.main.<private name>.ValueWitnessTable") !dbg !189 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !192
}

define void @_CNW4mainE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit(%"struct.main.<private name>.ValueWitnessTable"*, void (i8*, i8*)*, void (i8*)*) !dbg !193 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.main.<private name>.ValueWitnessTable", %"struct.main.<private name>.ValueWitnessTable"* %0, align 8, !dbg !196
  call void @_CNW4mainE17ValueWitnessTable14__default_initES0_L0(%"struct.main.<private name>.ValueWitnessTable" %3), !dbg !196
  %4 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %3, 0, !dbg !196
  %5 = alloca void (i8*, i8*)*, !dbg !196
  store void (i8*, i8*)* %4, void (i8*, i8*)** %5, !dbg !196
  store void (i8*, i8*)* %1, void (i8*, i8*)** %5, !dbg !196
  %6 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %3, 1, !dbg !196
  %7 = alloca void (i8*)*, !dbg !196
  store void (i8*)* %6, void (i8*)** %7, !dbg !196
  store void (i8*)* %2, void (i8*)** %7, !dbg !196
  ret void, !dbg !196
}

define void @_CNW4mainE17ValueWitnessTableC2ERS0_DoFvPvKPvEDoFvS2_EL0L6copyFnL6deinit(%"struct.main.<private name>.ValueWitnessTable"*, void (i8*, i8*)*, void (i8*)*) !dbg !197 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.main.<private name>.ValueWitnessTable", %"struct.main.<private name>.ValueWitnessTable"* %0, align 8, !dbg !198
  %4 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %3, 0, !dbg !198
  %5 = alloca void (i8*, i8*)*, !dbg !198
  store void (i8*, i8*)* %4, void (i8*, i8*)** %5, !dbg !198
  store void (i8*, i8*)* %1, void (i8*, i8*)** %5, !dbg !198
  %6 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %3, 1, !dbg !198
  %7 = alloca void (i8*)*, !dbg !198
  store void (i8*)* %6, void (i8*)** %7, !dbg !198
  store void (i8*)* %2, void (i8*)** %7, !dbg !198
  ret void, !dbg !198
}

define void @_CNW4mainE17ValueWitnessTableD3Ev(%"struct.main.<private name>.ValueWitnessTable"*) !dbg !199 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.ValueWitnessTable", %"struct.main.<private name>.ValueWitnessTable"* %0, align 8, !dbg !200
  %2 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %1, 0, !dbg !200
  %3 = alloca void (i8*, i8*)*, !dbg !200
  store void (i8*, i8*)* %2, void (i8*, i8*)** %3, !dbg !200
  %4 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %1, 1, !dbg !200
  %5 = alloca void (i8*)*, !dbg !200
  store void (i8*)* %4, void (i8*)** %5, !dbg !200
  %6 = bitcast void (i8*)** %5 to i8*, !dbg !200
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %6), !dbg !200
  %7 = bitcast void (i8*, i8*)** %3 to i8*, !dbg !200
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %7), !dbg !200
  ret void, !dbg !200
}

define void @_CNW4mainE19ProtocolConformanceD0Ev(%"struct.main.<private name>.ProtocolConformance"*) !dbg !201 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE19ProtocolConformanceD3Ev(%"struct.main.<private name>.ProtocolConformance"* %0), !dbg !227
  ret void, !dbg !227
}

define void @_CNW4mainE19ProtocolConformance14__default_initES0_L0(%"struct.main.<private name>.ProtocolConformance") !dbg !228 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !231
}

define void @_CNW4mainE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable(%"struct.main.<private name>.ProtocolConformance"*, %"struct.main.<private name>.TypeInfo"*, i8**) !dbg !232 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.main.<private name>.ProtocolConformance", %"struct.main.<private name>.ProtocolConformance"* %0, align 8, !dbg !235
  call void @_CNW4mainE19ProtocolConformance14__default_initES0_L0(%"struct.main.<private name>.ProtocolConformance" %3), !dbg !235
  %4 = extractvalue %"struct.main.<private name>.ProtocolConformance" %3, 0, !dbg !235
  %5 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !235
  store %"struct.main.<private name>.TypeInfo"* %4, %"struct.main.<private name>.TypeInfo"** %5, !dbg !235
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %5, !dbg !235
  %6 = extractvalue %"struct.main.<private name>.ProtocolConformance" %3, 1, !dbg !235
  %7 = alloca i8**, !dbg !235
  store i8** %6, i8*** %7, !dbg !235
  store i8** %2, i8*** %7, !dbg !235
  ret void, !dbg !235
}

define void @_CNW4mainE19ProtocolConformanceC2ERS0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable(%"struct.main.<private name>.ProtocolConformance"*, %"struct.main.<private name>.TypeInfo"*, i8**) !dbg !236 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.main.<private name>.ProtocolConformance", %"struct.main.<private name>.ProtocolConformance"* %0, align 8, !dbg !237
  %4 = extractvalue %"struct.main.<private name>.ProtocolConformance" %3, 0, !dbg !237
  %5 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !237
  store %"struct.main.<private name>.TypeInfo"* %4, %"struct.main.<private name>.TypeInfo"** %5, !dbg !237
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %5, !dbg !237
  %6 = extractvalue %"struct.main.<private name>.ProtocolConformance" %3, 1, !dbg !237
  %7 = alloca i8**, !dbg !237
  store i8** %6, i8*** %7, !dbg !237
  store i8** %2, i8*** %7, !dbg !237
  ret void, !dbg !237
}

define void @_CNW4mainE19ProtocolConformanceD3Ev(%"struct.main.<private name>.ProtocolConformance"*) !dbg !238 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.ProtocolConformance", %"struct.main.<private name>.ProtocolConformance"* %0, align 8, !dbg !239
  %2 = extractvalue %"struct.main.<private name>.ProtocolConformance" %1, 0, !dbg !239
  %3 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !239
  store %"struct.main.<private name>.TypeInfo"* %2, %"struct.main.<private name>.TypeInfo"** %3, !dbg !239
  %4 = extractvalue %"struct.main.<private name>.ProtocolConformance" %1, 1, !dbg !239
  %5 = alloca i8**, !dbg !239
  store i8** %4, i8*** %5, !dbg !239
  %6 = bitcast i8*** %5 to i8*, !dbg !239
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %6), !dbg !239
  %7 = bitcast %"struct.main.<private name>.TypeInfo"** %3 to i8*, !dbg !239
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %7), !dbg !239
  ret void, !dbg !239
}

define void @_CNW4mainE8TypeInfoD0Ev(%"struct.main.<private name>.TypeInfo"*) !dbg !240 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE8TypeInfoD3Ev(%"struct.main.<private name>.TypeInfo"* %0), !dbg !244
  ret void, !dbg !244
}

define void @_CNW4mainE8TypeInfo14__default_initES0_L0(%"struct.main.<private name>.TypeInfo"*) !dbg !245 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !248
}

define void @_CNW4mainE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride(%"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.TypeInfo"*, i8**, void (i8*)*, i8*, %"struct.main.<private name>.ValueWitnessTable"*, %"struct.main.<private name>.ProtocolConformance"*, i64, i64, i64) !dbg !249 {
entry:
  call void @_CNW4mainE8TypeInfo14__default_initES0_L0(%"struct.main.<private name>.TypeInfo"* %0), !dbg !252
  %10 = bitcast %"struct.main.<private name>.TypeInfo"* %0 to %"struct.main.<private name>.TypeInfo"**
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %10, align 8, !dbg !252
  %11 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !252
  store i8** %2, i8*** %11, align 8, !dbg !252
  %12 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !252
  store void (i8*)* %3, void (i8*)** %12, align 8, !dbg !252
  %13 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !252
  store i8* %4, i8** %13, align 8, !dbg !252
  %14 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !252
  store %"struct.main.<private name>.ValueWitnessTable"* %5, %"struct.main.<private name>.ValueWitnessTable"** %14, align 8, !dbg !252
  %15 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !252
  store %"struct.main.<private name>.ProtocolConformance"* %6, %"struct.main.<private name>.ProtocolConformance"** %15, align 8, !dbg !252
  %16 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !252
  store i64 %7, i64* %16, align 8, !dbg !252
  %17 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !252
  store i64 %8, i64* %17, align 8, !dbg !252
  %18 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !252
  store i64 %9, i64* %18, align 8, !dbg !252
  ret void, !dbg !252
}

define void @_CNW4mainE8TypeInfoC2ERS0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride(%"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.TypeInfo"*, i8**, void (i8*)*, i8*, %"struct.main.<private name>.ValueWitnessTable"*, %"struct.main.<private name>.ProtocolConformance"*, i64, i64, i64) !dbg !253 {
entry:
  %10 = bitcast %"struct.main.<private name>.TypeInfo"* %0 to %"struct.main.<private name>.TypeInfo"**
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %10, align 8, !dbg !254
  %11 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !254
  store i8** %2, i8*** %11, align 8, !dbg !254
  %12 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !254
  store void (i8*)* %3, void (i8*)** %12, align 8, !dbg !254
  %13 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !254
  store i8* %4, i8** %13, align 8, !dbg !254
  %14 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !254
  store %"struct.main.<private name>.ValueWitnessTable"* %5, %"struct.main.<private name>.ValueWitnessTable"** %14, align 8, !dbg !254
  %15 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !254
  store %"struct.main.<private name>.ProtocolConformance"* %6, %"struct.main.<private name>.ProtocolConformance"** %15, align 8, !dbg !254
  %16 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !254
  store i64 %7, i64* %16, align 8, !dbg !254
  %17 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !254
  store i64 %8, i64* %17, align 8, !dbg !254
  %18 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !254
  store i64 %9, i64* %18, align 8, !dbg !254
  ret void, !dbg !254
}

define void @_CNW4mainE8TypeInfoD3Ev(%"struct.main.<private name>.TypeInfo"*) !dbg !255 {
entry:
  %1 = bitcast %"struct.main.<private name>.TypeInfo"* %0 to %"struct.main.<private name>.TypeInfo"**
  %2 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !256
  %3 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !256
  %4 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !256
  %5 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !256
  %6 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !256
  %7 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !256
  %8 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !256
  %9 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !256
  %10 = bitcast i64* %9 to i8*, !dbg !256
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %10), !dbg !256
  %11 = bitcast i64* %8 to i8*, !dbg !256
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %11), !dbg !256
  %12 = bitcast i64* %7 to i8*, !dbg !256
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %12), !dbg !256
  %13 = bitcast %"struct.main.<private name>.ProtocolConformance"** %6 to i8*, !dbg !256
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %13), !dbg !256
  %14 = bitcast %"struct.main.<private name>.ValueWitnessTable"** %5 to i8*, !dbg !256
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %14), !dbg !256
  %15 = bitcast i8** %4 to i8*, !dbg !256
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %15), !dbg !256
  %16 = bitcast void (i8*)** %3 to i8*, !dbg !256
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %16), !dbg !256
  %17 = bitcast i8*** %2 to i8*, !dbg !256
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %17), !dbg !256
  %18 = bitcast %"struct.main.<private name>.TypeInfo"** %1 to i8*, !dbg !256
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %18), !dbg !256
  ret void, !dbg !256
}

define void @_CNW4mainE5Int64D0Ev(%"struct.main.<private name>.Int64"*) !dbg !257 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE5Int64D3Ev(%"struct.main.<private name>.Int64"* %0), !dbg !264
  ret void, !dbg !264
}

define %"struct.main.<private name>.Int64" @_CNW4mainE5Int644copyES0_L0(%"struct.main.<private name>.Int64") !dbg !265 {
alloca_block:
  %1 = alloca %"struct.main.<private name>.Int64", align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Int64", %"struct.main.<private name>.Int64"* %1, align 8, !dbg !268
  %3 = extractvalue %"struct.main.<private name>.Int64" %2, 0, !dbg !268
  %4 = alloca i64, !dbg !268
  store i64 %3, i64* %4, !dbg !268
  %5 = extractvalue %"struct.main.<private name>.Int64" %0, 0, !dbg !268
  %6 = alloca i64, !dbg !268
  store i64 %5, i64* %6, !dbg !268
  %7 = load i64, i64* %6, !dbg !268
  store i64 %7, i64* %4, !dbg !268
  ret %"struct.main.<private name>.Int64" %2, !dbg !268
}

define void @_CNW4mainE5Int6414__default_initES0_L0(%"struct.main.<private name>.Int64") !dbg !269 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !270
}

define void @_CNW4mainE5Int64C1ElL5value(%"struct.main.<private name>.Int64"*, i64) !dbg !271 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Int64", %"struct.main.<private name>.Int64"* %0, align 8, !dbg !274
  call void @_CNW4mainE5Int6414__default_initES0_L0(%"struct.main.<private name>.Int64" %2), !dbg !274
  %3 = extractvalue %"struct.main.<private name>.Int64" %2, 0, !dbg !274
  %4 = alloca i64, !dbg !274
  store i64 %3, i64* %4, !dbg !274
  store i64 %1, i64* %4, !dbg !274
  ret void, !dbg !274
}

define void @_CNW4mainE5Int64C2ERS0_lL0L5value(%"struct.main.<private name>.Int64"*, i64) !dbg !275 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Int64", %"struct.main.<private name>.Int64"* %0, align 8, !dbg !276
  %3 = extractvalue %"struct.main.<private name>.Int64" %2, 0, !dbg !276
  %4 = alloca i64, !dbg !276
  store i64 %3, i64* %4, !dbg !276
  store i64 %1, i64* %4, !dbg !276
  ret void, !dbg !276
}

define void @_CNW4mainE5Int64D3Ev(%"struct.main.<private name>.Int64"*) !dbg !277 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.Int64", %"struct.main.<private name>.Int64"* %0, align 8, !dbg !278
  %2 = extractvalue %"struct.main.<private name>.Int64" %1, 0, !dbg !278
  %3 = alloca i64, !dbg !278
  store i64 %2, i64* %3, !dbg !278
  %4 = bitcast i64* %3 to i8*, !dbg !278
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %4), !dbg !278
  ret void, !dbg !278
}

define void @_CNW4mainE20ExistentialContainerD0Ev(%"struct.main.<private name>.ExistentialContainer"*) !dbg !279 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE20ExistentialContainerD3Ev(%"struct.main.<private name>.ExistentialContainer"* %0), !dbg !288
  ret void, !dbg !288
}

define void @_CNW4mainE20ExistentialContainer14__default_initES0_L0(%"struct.main.<private name>.ExistentialContainer") !dbg !289 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !292
}

define void @_CNW4mainE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance(%"struct.main.<private name>.ExistentialContainer"*, i8*, %"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.ProtocolConformance"*) !dbg !293 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %4 = load %"struct.main.<private name>.ExistentialContainer", %"struct.main.<private name>.ExistentialContainer"* %0, align 8, !dbg !296
  call void @_CNW4mainE20ExistentialContainer14__default_initES0_L0(%"struct.main.<private name>.ExistentialContainer" %4), !dbg !296
  %5 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 0, !dbg !296
  %6 = alloca i8*, !dbg !296
  store i8* %5, i8** %6, !dbg !296
  store i8* %1, i8** %6, !dbg !296
  %7 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 1, !dbg !296
  %8 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !296
  store %"struct.main.<private name>.TypeInfo"* %7, %"struct.main.<private name>.TypeInfo"** %8, !dbg !296
  store %"struct.main.<private name>.TypeInfo"* %2, %"struct.main.<private name>.TypeInfo"** %8, !dbg !296
  %9 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 2, !dbg !296
  %10 = alloca %"struct.main.<private name>.ProtocolConformance"*, !dbg !296
  store %"struct.main.<private name>.ProtocolConformance"* %9, %"struct.main.<private name>.ProtocolConformance"** %10, !dbg !296
  store %"struct.main.<private name>.ProtocolConformance"* %3, %"struct.main.<private name>.ProtocolConformance"** %10, !dbg !296
  ret void, !dbg !296
}

define void @_CNW4mainE20ExistentialContainerC2ERS0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance(%"struct.main.<private name>.ExistentialContainer"*, i8*, %"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.ProtocolConformance"*) !dbg !297 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %4 = load %"struct.main.<private name>.ExistentialContainer", %"struct.main.<private name>.ExistentialContainer"* %0, align 8, !dbg !298
  %5 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 0, !dbg !298
  %6 = alloca i8*, !dbg !298
  store i8* %5, i8** %6, !dbg !298
  store i8* %1, i8** %6, !dbg !298
  %7 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 1, !dbg !298
  %8 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !298
  store %"struct.main.<private name>.TypeInfo"* %7, %"struct.main.<private name>.TypeInfo"** %8, !dbg !298
  store %"struct.main.<private name>.TypeInfo"* %2, %"struct.main.<private name>.TypeInfo"** %8, !dbg !298
  %9 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 2, !dbg !298
  %10 = alloca %"struct.main.<private name>.ProtocolConformance"*, !dbg !298
  store %"struct.main.<private name>.ProtocolConformance"* %9, %"struct.main.<private name>.ProtocolConformance"** %10, !dbg !298
  store %"struct.main.<private name>.ProtocolConformance"* %3, %"struct.main.<private name>.ProtocolConformance"** %10, !dbg !298
  ret void, !dbg !298
}

define void @_CNW4mainE20ExistentialContainerD3Ev(%"struct.main.<private name>.ExistentialContainer"*) !dbg !299 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.ExistentialContainer", %"struct.main.<private name>.ExistentialContainer"* %0, align 8, !dbg !300
  %2 = extractvalue %"struct.main.<private name>.ExistentialContainer" %1, 0, !dbg !300
  %3 = alloca i8*, !dbg !300
  store i8* %2, i8** %3, !dbg !300
  %4 = extractvalue %"struct.main.<private name>.ExistentialContainer" %1, 1, !dbg !300
  %5 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !300
  store %"struct.main.<private name>.TypeInfo"* %4, %"struct.main.<private name>.TypeInfo"** %5, !dbg !300
  %6 = extractvalue %"struct.main.<private name>.ExistentialContainer" %1, 2, !dbg !300
  %7 = alloca %"struct.main.<private name>.ProtocolConformance"*, !dbg !300
  store %"struct.main.<private name>.ProtocolConformance"* %6, %"struct.main.<private name>.ProtocolConformance"** %7, !dbg !300
  %8 = bitcast %"struct.main.<private name>.ProtocolConformance"** %7 to i8*, !dbg !300
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %8), !dbg !300
  %9 = bitcast %"struct.main.<private name>.TypeInfo"** %5 to i8*, !dbg !300
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %9), !dbg !300
  %10 = bitcast i8** %3 to i8*, !dbg !300
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %10), !dbg !300
  ret void, !dbg !300
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
!5 = distinct !DISubprogram(name: "_CNW4mainE6_startEv", linkageName: "_CNW4mainE6_startEv", scope: null, file: !6, line: 1, type: !7, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!6 = !DIFile(filename: "main.dot", directory: "/Users/Jonas/CDot/StdLib/main/")
!7 = !DISubroutineType(types: !2)
!8 = !DILocation(line: 16, column: 10, scope: !5)
!9 = !DILocalVariable(name: "a", scope: !5, file: !1, line: 16, type: !10)
!10 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.Atomic<i64>", file: !11, line: 36, flags: DIFlagFwdDecl, elements: !12)
!11 = !DIFile(filename: "main4.dot", directory: "/Users/Jonas/CDot/StdLib/main/")
!12 = !{!13}
!13 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !10, file: !11, line: 36, baseType: !14, size: 64, align: 64)
!14 = !DIBasicType(name: "i64", size: 64, encoding: DW_ATE_signed)
!15 = !DILocation(line: 19, column: 2, scope: !5)
!16 = !DILocation(line: 20, column: 10, scope: !5)
!17 = !DILocalVariable(name: "x", scope: !5, file: !1, line: 20, type: !14)
!18 = !DILocation(line: 23, column: 41, scope: !5)
!19 = !DILocation(line: 23, column: 6, scope: !5)
!20 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !6, line: 1, type: !7, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!21 = !DILocation(line: 1, column: 2, scope: !20)
!22 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlEC1ElL10weirdValue", linkageName: "_CNW4mainE6AtomicIlEC1ElL10weirdValue", scope: !0, file: !11, line: 45, type: !23, isLocal: false, isDefinition: true, scopeLine: 45, thisAdjustment: 1, isOptimized: false, unit: !0)
!23 = !DISubroutineType(types: !24)
!24 = !{!25, !14}
!25 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !10, size: 64, align: 8)
!26 = !DILocation(line: 45, column: 6, scope: !22)
!27 = !DILocalVariable(name: "self", scope: !22, file: !1, line: 45, type: !10)
!28 = !DILocalVariable(name: "weirdValue", scope: !22, file: !1, line: 45, type: !14)
!29 = !DILocation(line: 46, column: 27, scope: !22)
!30 = !DILocation(line: 46, column: 10, scope: !22)
!31 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlE5storeERS0_lNW_0E11MemoryOrderEL0L0L11memoryOrder", linkageName: "_CNW4mainE6AtomicIlE5storeERS0_lNW_0E11MemoryOrderEL0L0L11memoryOrder", scope: !0, file: !11, line: 99, type: !32, isLocal: false, isDefinition: true, scopeLine: 99, thisAdjustment: 1, isOptimized: false, unit: !0)
!32 = !DISubroutineType(types: !33)
!33 = !{!25, !14, !34}
!34 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "main.MemoryOrder", file: !11, line: 12, flags: DIFlagFwdDecl, elements: !35)
!35 = !{!36, !37, !38, !39, !40, !41, !42}
!36 = !DIEnumerator(name: "NotAtomic", value: 0)
!37 = !DIEnumerator(name: "Relaxed", value: 1)
!38 = !DIEnumerator(name: "Consume", value: 2)
!39 = !DIEnumerator(name: "Acquire", value: 3)
!40 = !DIEnumerator(name: "Release", value: 4)
!41 = !DIEnumerator(name: "AcquireRelease", value: 5)
!42 = !DIEnumerator(name: "SequentiallyConsistent", value: 6)
!43 = !DILocalVariable(name: "self", scope: !31, file: !1, line: 99, type: !10)
!44 = !DILocation(line: 99, column: 6, scope: !31)
!45 = !DILocalVariable(name: "storeVal", scope: !31, file: !1, line: 99, type: !14)
!46 = !DILocalVariable(name: "memoryOrder", scope: !31, file: !1, line: 99, type: !34)
!47 = !DILocation(line: 101, column: 31, scope: !31)
!48 = !DILocation(line: 101, column: 10, scope: !31)
!49 = !DILocalVariable(name: "addr", scope: !31, file: !1, line: 102, type: !50)
!50 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 64, align: 8)
!51 = !DILocation(line: 102, column: 10, scope: !31)
!52 = !DILocation(line: 103, column: 10, scope: !31)
!53 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlE4loadES0_NW_0E11MemoryOrderEL0L11memoryOrder", linkageName: "_CNW4mainE6AtomicIlE4loadES0_NW_0E11MemoryOrderEL0L11memoryOrder", scope: !0, file: !11, line: 92, type: !54, isLocal: false, isDefinition: true, scopeLine: 92, thisAdjustment: 1, isOptimized: false, unit: !0)
!54 = !DISubroutineType(types: !55)
!55 = !{!10, !34}
!56 = !DILocalVariable(name: "self", scope: !53, file: !1, line: 92, type: !10)
!57 = !DILocation(line: 92, column: 6, scope: !53)
!58 = !DILocalVariable(name: "memoryOrder", scope: !53, file: !1, line: 92, type: !34)
!59 = !DILocation(line: 93, column: 30, scope: !53)
!60 = !DILocation(line: 93, column: 10, scope: !53)
!61 = !DILocalVariable(name: "addr", scope: !53, file: !1, line: 94, type: !50)
!62 = !DILocation(line: 94, column: 10, scope: !53)
!63 = !DILocation(line: 95, column: 10, scope: !53)
!64 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlED0Ev", linkageName: "_CNW4mainE6AtomicIlED0Ev", scope: !0, file: !11, line: 36, type: !65, isLocal: false, isDefinition: true, scopeLine: 36, thisAdjustment: 1, isOptimized: false, unit: !0)
!65 = !DISubroutineType(types: !66)
!66 = !{!25}
!67 = !DILocation(line: 36, column: 9, scope: !64)
!68 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderD0Ev", linkageName: "_CNW4mainE11MemoryOrderD0Ev", scope: !0, file: !11, line: 12, type: !69, isLocal: false, isDefinition: true, scopeLine: 12, thisAdjustment: 1, isOptimized: false, unit: !0)
!69 = !DISubroutineType(types: !70)
!70 = !{!71}
!71 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !34, size: 64, align: 8)
!72 = !DILocation(line: 12, column: 9, scope: !68)
!73 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrder4copyES_L0", linkageName: "_CNW4mainE11MemoryOrder4copyES_L0", scope: !0, file: !11, line: 12, type: !74, isLocal: false, isDefinition: true, scopeLine: 12, thisAdjustment: 1, isOptimized: false, unit: !0)
!74 = !DISubroutineType(types: !75)
!75 = !{!34}
!76 = !DILocation(line: 12, column: 9, scope: !73)
!77 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderXi2==ES_S_L0L0", linkageName: "_CNW4mainE11MemoryOrderXi2==ES_S_L0L0", scope: !0, file: !11, line: 12, type: !78, isLocal: false, isDefinition: true, scopeLine: 12, thisAdjustment: 1, isOptimized: false, unit: !0)
!78 = !DISubroutineType(types: !79)
!79 = !{!34, !34}
!80 = !DILocation(line: 12, column: 9, scope: !77)
!81 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderXi2!=ES_S_L0L0", linkageName: "_CNW4mainE11MemoryOrderXi2!=ES_S_L0L0", scope: !0, file: !6, line: 1, type: !78, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!82 = !DILocalVariable(name: "self", scope: !81, file: !1, line: 1, type: !34)
!83 = !DILocation(line: 1, column: 2, scope: !81)
!84 = !DILocalVariable(name: "rhs", scope: !81, file: !1, line: 1, type: !34)
!85 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderC1EiL8rawValue", linkageName: "_CNW4mainE11MemoryOrderC1EiL8rawValue", scope: !0, file: !11, line: 12, type: !86, isLocal: false, isDefinition: true, scopeLine: 12, thisAdjustment: 1, isOptimized: false, unit: !0)
!86 = !DISubroutineType(types: !87)
!87 = !{!71, !88}
!88 = !DIBasicType(name: "i32", size: 32, encoding: DW_ATE_signed)
!89 = !DILocation(line: 12, column: 9, scope: !85)
!90 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderGa8rawValueES_L0", linkageName: "_CNW4mainE11MemoryOrderGa8rawValueES_L0", scope: !0, file: !11, line: 12, type: !74, isLocal: false, isDefinition: true, scopeLine: 12, thisAdjustment: 1, isOptimized: false, unit: !0)
!91 = !DILocation(line: 12, column: 9, scope: !90)
!92 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderD3Ev", linkageName: "_CNW4mainE11MemoryOrderD3Ev", scope: null, file: !11, line: 12, type: !69, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!93 = !DILocation(line: 12, column: 9, scope: !92)
!94 = distinct !DISubprogram(name: "_CNW4mainE4BoolXp1!ES0_L0", linkageName: "_CNW4mainE4BoolXp1!ES0_L0", scope: !0, file: !6, line: 1, type: !95, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!95 = !DISubroutineType(types: !96)
!96 = !{!97}
!97 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.Bool", file: !6, line: 1, flags: DIFlagFwdDecl, elements: !98)
!98 = !{!99}
!99 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !97, file: !6, line: 1, baseType: !100, size: 8, align: 8)
!100 = !DIBasicType(name: "i1", size: 8, encoding: DW_ATE_boolean)
!101 = !DILocalVariable(name: "self", scope: !94, file: !1, line: 1, type: !97)
!102 = !DILocation(line: 1, column: 2, scope: !94)
!103 = distinct !DISubprogram(name: "_CNW4mainE4BoolD0Ev", linkageName: "_CNW4mainE4BoolD0Ev", scope: !0, file: !6, line: 1, type: !104, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!104 = !DISubroutineType(types: !105)
!105 = !{!106}
!106 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !97, size: 64, align: 8)
!107 = !DILocation(line: 1, column: 2, scope: !103)
!108 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlE4copyES0_L0", linkageName: "_CNW4mainE6AtomicIlE4copyES0_L0", scope: !0, file: !11, line: 36, type: !109, isLocal: false, isDefinition: true, scopeLine: 36, thisAdjustment: 1, isOptimized: false, unit: !0)
!109 = !DISubroutineType(types: !110)
!110 = !{!10}
!111 = !DILocation(line: 36, column: 9, scope: !108)
!112 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlE14__default_initES0_L0", linkageName: "_CNW4mainE6AtomicIlE14__default_initES0_L0", scope: !0, file: !11, line: 36, type: !109, isLocal: false, isDefinition: true, scopeLine: 36, thisAdjustment: 1, isOptimized: false, unit: !0)
!113 = !DILocation(line: 36, column: 9, scope: !112)
!114 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlED3Ev", linkageName: "_CNW4mainE6AtomicIlED3Ev", scope: null, file: !11, line: 36, type: !65, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!115 = !DILocation(line: 36, column: 9, scope: !114)
!116 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlEC2ERS0_lL0L5value", linkageName: "_CNW4mainE6AtomicIlEC2ERS0_lL0L5value", scope: !0, file: !11, line: 41, type: !23, isLocal: false, isDefinition: true, scopeLine: 41, thisAdjustment: 1, isOptimized: false, unit: !0)
!117 = !DILocalVariable(name: "self", scope: !116, file: !1, line: 41, type: !10)
!118 = !DILocation(line: 41, column: 6, scope: !116)
!119 = !DILocalVariable(name: "value", scope: !116, file: !1, line: 41, type: !14)
!120 = !DILocation(line: 42, column: 10, scope: !116)
!121 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlEC2ERS0_lL0L10weirdValue", linkageName: "_CNW4mainE6AtomicIlEC2ERS0_lL0L10weirdValue", scope: !0, file: !11, line: 45, type: !23, isLocal: false, isDefinition: true, scopeLine: 45, thisAdjustment: 1, isOptimized: false, unit: !0)
!122 = !DILocalVariable(name: "self", scope: !121, file: !1, line: 45, type: !10)
!123 = !DILocation(line: 45, column: 6, scope: !121)
!124 = !DILocalVariable(name: "weirdValue", scope: !121, file: !1, line: 45, type: !14)
!125 = !DILocation(line: 46, column: 27, scope: !121)
!126 = !DILocation(line: 46, column: 10, scope: !121)
!127 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlE20checkValidStoreOrderEMS0_NW_0E11MemoryOrderEL0L0", linkageName: "_CNW4mainE6AtomicIlE20checkValidStoreOrderEMS0_NW_0E11MemoryOrderEL0L0", scope: !0, file: !11, line: 64, type: !128, isLocal: false, isDefinition: true, scopeLine: 64, thisAdjustment: 1, isOptimized: false, unit: !0)
!128 = !DISubroutineType(types: !129)
!129 = !{!130, !34}
!130 = !DICompositeType(tag: DW_TAG_structure_type, name: "MetaType<main.Atomic<i64>>", file: !1, size: 64, align: 64, elements: !131)
!131 = !{!132, !132, !132, !132, !132}
!132 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !133, size: 64, align: 8)
!133 = !DIBasicType(name: "i8", size: 8, encoding: DW_ATE_signed_char)
!134 = !DILocalVariable(name: "self", scope: !127, file: !1, line: 64, type: !130)
!135 = !DILocation(line: 64, column: 21, scope: !127)
!136 = !DILocalVariable(name: "memoryOrder", scope: !127, file: !1, line: 64, type: !34)
!137 = !DILocation(line: 65, column: 16, scope: !127)
!138 = !DILocation(line: 74, column: 14, scope: !127)
!139 = !DILocation(line: 72, column: 80, scope: !127)
!140 = !DILocation(line: 72, column: 18, scope: !127)
!141 = !DILocation(line: 67, column: 14, scope: !127)
!142 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlE19checkValidLoadOrderEMS0_NW_0E11MemoryOrderEL0L0", linkageName: "_CNW4mainE6AtomicIlE19checkValidLoadOrderEMS0_NW_0E11MemoryOrderEL0L0", scope: !0, file: !11, line: 50, type: !128, isLocal: false, isDefinition: true, scopeLine: 50, thisAdjustment: 1, isOptimized: false, unit: !0)
!143 = !DILocalVariable(name: "self", scope: !142, file: !1, line: 50, type: !130)
!144 = !DILocation(line: 50, column: 21, scope: !142)
!145 = !DILocalVariable(name: "memoryOrder", scope: !142, file: !1, line: 50, type: !34)
!146 = !DILocation(line: 51, column: 16, scope: !142)
!147 = !DILocation(line: 59, column: 14, scope: !142)
!148 = !DILocation(line: 57, column: 79, scope: !142)
!149 = !DILocation(line: 57, column: 18, scope: !142)
!150 = !DILocation(line: 53, column: 14, scope: !142)
!151 = distinct !DISubprogram(name: "_CNW4mainEXi2~=INW_0E11MemoryOrderEEEbNW_0E11MemoryOrderENW_0E11MemoryOrderEL0L0", linkageName: "_CNW4mainEXi2~=INW_0E11MemoryOrderEEEbNW_0E11MemoryOrderENW_0E11MemoryOrderEL0L0", scope: null, file: !11, line: 5, type: !78, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!152 = !DILocalVariable(name: "lhs", scope: !151, file: !1, line: 5, type: !34)
!153 = !DILocation(line: 5, column: 9, scope: !151)
!154 = !DILocalVariable(name: "rhs", scope: !151, file: !1, line: 5, type: !34)
!155 = !DILocation(line: 6, column: 6, scope: !151)
!156 = !DILocalVariable(name: "v1", scope: !151, file: !1, line: 6, type: !14)
!157 = !DILocation(line: 7, column: 6, scope: !151)
!158 = !DILocalVariable(name: "v2", scope: !151, file: !1, line: 7, type: !14)
!159 = !DILocation(line: 9, column: 13, scope: !151)
!160 = !DILocation(line: 9, column: 19, scope: !151)
!161 = distinct !DISubprogram(name: "_CNW4mainE4Bool4copyES0_L0", linkageName: "_CNW4mainE4Bool4copyES0_L0", scope: !0, file: !6, line: 1, type: !95, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!162 = !DILocation(line: 1, column: 2, scope: !161)
!163 = distinct !DISubprogram(name: "_CNW4mainE4Bool14__default_initES0_L0", linkageName: "_CNW4mainE4Bool14__default_initES0_L0", scope: !0, file: !6, line: 1, type: !95, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!164 = !DILocation(line: 1, column: 2, scope: !163)
!165 = distinct !DISubprogram(name: "_CNW4mainE4BoolC1EbL5value", linkageName: "_CNW4mainE4BoolC1EbL5value", scope: !0, file: !6, line: 1, type: !166, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!166 = !DISubroutineType(types: !167)
!167 = !{!106, !100}
!168 = !DILocation(line: 1, column: 2, scope: !165)
!169 = distinct !DISubprogram(name: "_CNW4mainE4BoolC2ERS0_bL0L5value", linkageName: "_CNW4mainE4BoolC2ERS0_bL0L5value", scope: !0, file: !6, line: 1, type: !166, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!170 = !DILocation(line: 1, column: 2, scope: !169)
!171 = distinct !DISubprogram(name: "_CNW4mainE4BoolD3Ev", linkageName: "_CNW4mainE4BoolD3Ev", scope: null, file: !6, line: 1, type: !104, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!172 = !DILocation(line: 1, column: 2, scope: !171)
!173 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableD0Ev", linkageName: "_CNW4mainE17ValueWitnessTableD0Ev", scope: !0, file: !6, line: 1, type: !174, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!174 = !DISubroutineType(types: !175)
!175 = !{!176}
!176 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !177, size: 64, align: 8)
!177 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.ValueWitnessTable", file: !6, line: 1, flags: DIFlagFwdDecl, elements: !178)
!178 = !{!179, !184}
!179 = !DIDerivedType(tag: DW_TAG_member, name: "copyFn", scope: !177, file: !6, line: 1, baseType: !180, size: 64, align: 64)
!180 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !181, size: 64, align: 8)
!181 = !DISubroutineType(types: !182)
!182 = !{!183, !183}
!183 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64, align: 8)
!184 = !DIDerivedType(tag: DW_TAG_member, name: "deinit", scope: !177, file: !6, line: 1, baseType: !185, size: 64, align: 64, offset: 64)
!185 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !186, size: 64, align: 8)
!186 = !DISubroutineType(types: !187)
!187 = !{!183}
!188 = !DILocation(line: 1, column: 2, scope: !173)
!189 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTable14__default_initES0_L0", linkageName: "_CNW4mainE17ValueWitnessTable14__default_initES0_L0", scope: !0, file: !6, line: 1, type: !190, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!190 = !DISubroutineType(types: !191)
!191 = !{!177}
!192 = !DILocation(line: 1, column: 2, scope: !189)
!193 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit", linkageName: "_CNW4mainE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit", scope: !0, file: !6, line: 1, type: !194, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!194 = !DISubroutineType(types: !195)
!195 = !{!176, !180, !185}
!196 = !DILocation(line: 1, column: 2, scope: !193)
!197 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableC2ERS0_DoFvPvKPvEDoFvS2_EL0L6copyFnL6deinit", linkageName: "_CNW4mainE17ValueWitnessTableC2ERS0_DoFvPvKPvEDoFvS2_EL0L6copyFnL6deinit", scope: !0, file: !6, line: 1, type: !194, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!198 = !DILocation(line: 1, column: 2, scope: !197)
!199 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableD3Ev", linkageName: "_CNW4mainE17ValueWitnessTableD3Ev", scope: null, file: !6, line: 1, type: !174, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!200 = !DILocation(line: 1, column: 2, scope: !199)
!201 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceD0Ev", linkageName: "_CNW4mainE19ProtocolConformanceD0Ev", scope: !0, file: !6, line: 1, type: !202, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!202 = !DISubroutineType(types: !203)
!203 = !{!204}
!204 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !205, size: 64, align: 8)
!205 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.ProtocolConformance", file: !6, line: 1, flags: DIFlagFwdDecl, elements: !206)
!206 = !{!207, !226}
!207 = !DIDerivedType(tag: DW_TAG_member, name: "typeInfo", scope: !205, file: !6, line: 1, baseType: !208, size: 64, align: 64)
!208 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !209, size: 64, align: 8)
!209 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.TypeInfo", file: !6, line: 1, flags: DIFlagFwdDecl, elements: !210)
!210 = !{!211, !212, !214, !215, !218, !220, !222, !224, !225}
!211 = !DIDerivedType(tag: DW_TAG_member, name: "baseClass", scope: !209, file: !6, line: 1, baseType: !208, size: 64, align: 64)
!212 = !DIDerivedType(tag: DW_TAG_member, name: "vtable", scope: !209, file: !6, line: 1, baseType: !213, size: 64, align: 64, offset: 64)
!213 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !183, size: 64, align: 8)
!214 = !DIDerivedType(tag: DW_TAG_member, name: "deinit", scope: !209, file: !6, line: 1, baseType: !185, size: 64, align: 64, offset: 128)
!215 = !DIDerivedType(tag: DW_TAG_member, name: "name", scope: !209, file: !6, line: 1, baseType: !216, size: 64, align: 64, offset: 192)
!216 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !217, size: 64, align: 8)
!217 = !DIBasicType(name: "u8", size: 8, encoding: DW_ATE_unsigned_char)
!218 = !DIDerivedType(tag: DW_TAG_member, name: "valueWitnessTable", scope: !209, file: !6, line: 1, baseType: !219, size: 64, align: 64, offset: 256)
!219 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !177, size: 64, align: 8)
!220 = !DIDerivedType(tag: DW_TAG_member, name: "conformances", scope: !209, file: !6, line: 1, baseType: !221, size: 64, align: 64, offset: 320)
!221 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !205, size: 64, align: 8)
!222 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !209, file: !6, line: 1, baseType: !223, size: 64, align: 64, offset: 384)
!223 = !DIBasicType(name: "u64", size: 64, encoding: DW_ATE_unsigned)
!224 = !DIDerivedType(tag: DW_TAG_member, name: "alignment", scope: !209, file: !6, line: 1, baseType: !223, size: 64, align: 64, offset: 448)
!225 = !DIDerivedType(tag: DW_TAG_member, name: "stride", scope: !209, file: !6, line: 1, baseType: !223, size: 64, align: 64, offset: 512)
!226 = !DIDerivedType(tag: DW_TAG_member, name: "vtable", scope: !205, file: !6, line: 1, baseType: !213, size: 64, align: 64, offset: 64)
!227 = !DILocation(line: 1, column: 2, scope: !201)
!228 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformance14__default_initES0_L0", linkageName: "_CNW4mainE19ProtocolConformance14__default_initES0_L0", scope: !0, file: !6, line: 1, type: !229, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!229 = !DISubroutineType(types: !230)
!230 = !{!205}
!231 = !DILocation(line: 1, column: 2, scope: !228)
!232 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable", linkageName: "_CNW4mainE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable", scope: !0, file: !6, line: 1, type: !233, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!233 = !DISubroutineType(types: !234)
!234 = !{!204, !208, !213}
!235 = !DILocation(line: 1, column: 2, scope: !232)
!236 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceC2ERS0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable", linkageName: "_CNW4mainE19ProtocolConformanceC2ERS0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable", scope: !0, file: !6, line: 1, type: !233, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!237 = !DILocation(line: 1, column: 2, scope: !236)
!238 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceD3Ev", linkageName: "_CNW4mainE19ProtocolConformanceD3Ev", scope: null, file: !6, line: 1, type: !202, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!239 = !DILocation(line: 1, column: 2, scope: !238)
!240 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoD0Ev", linkageName: "_CNW4mainE8TypeInfoD0Ev", scope: !0, file: !6, line: 1, type: !241, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!241 = !DISubroutineType(types: !242)
!242 = !{!243}
!243 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !209, size: 64, align: 8)
!244 = !DILocation(line: 1, column: 2, scope: !240)
!245 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfo14__default_initES0_L0", linkageName: "_CNW4mainE8TypeInfo14__default_initES0_L0", scope: !0, file: !6, line: 1, type: !246, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!246 = !DISubroutineType(types: !247)
!247 = !{!209}
!248 = !DILocation(line: 1, column: 2, scope: !245)
!249 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", linkageName: "_CNW4mainE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", scope: !0, file: !6, line: 1, type: !250, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!250 = !DISubroutineType(types: !251)
!251 = !{!243, !208, !213, !185, !216, !219, !221, !223, !223, !223}
!252 = !DILocation(line: 1, column: 2, scope: !249)
!253 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoC2ERS0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", linkageName: "_CNW4mainE8TypeInfoC2ERS0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", scope: !0, file: !6, line: 1, type: !250, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!254 = !DILocation(line: 1, column: 2, scope: !253)
!255 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoD3Ev", linkageName: "_CNW4mainE8TypeInfoD3Ev", scope: null, file: !6, line: 1, type: !241, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!256 = !DILocation(line: 1, column: 2, scope: !255)
!257 = distinct !DISubprogram(name: "_CNW4mainE5Int64D0Ev", linkageName: "_CNW4mainE5Int64D0Ev", scope: !0, file: !6, line: 1, type: !258, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!258 = !DISubroutineType(types: !259)
!259 = !{!260}
!260 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !261, size: 64, align: 8)
!261 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.Int64", file: !6, line: 1, flags: DIFlagFwdDecl, elements: !262)
!262 = !{!263}
!263 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !261, file: !6, line: 1, baseType: !14, size: 64, align: 64)
!264 = !DILocation(line: 1, column: 2, scope: !257)
!265 = distinct !DISubprogram(name: "_CNW4mainE5Int644copyES0_L0", linkageName: "_CNW4mainE5Int644copyES0_L0", scope: !0, file: !6, line: 1, type: !266, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!266 = !DISubroutineType(types: !267)
!267 = !{!261}
!268 = !DILocation(line: 1, column: 2, scope: !265)
!269 = distinct !DISubprogram(name: "_CNW4mainE5Int6414__default_initES0_L0", linkageName: "_CNW4mainE5Int6414__default_initES0_L0", scope: !0, file: !6, line: 1, type: !266, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!270 = !DILocation(line: 1, column: 2, scope: !269)
!271 = distinct !DISubprogram(name: "_CNW4mainE5Int64C1ElL5value", linkageName: "_CNW4mainE5Int64C1ElL5value", scope: !0, file: !6, line: 1, type: !272, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!272 = !DISubroutineType(types: !273)
!273 = !{!260, !14}
!274 = !DILocation(line: 1, column: 2, scope: !271)
!275 = distinct !DISubprogram(name: "_CNW4mainE5Int64C2ERS0_lL0L5value", linkageName: "_CNW4mainE5Int64C2ERS0_lL0L5value", scope: !0, file: !6, line: 1, type: !272, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!276 = !DILocation(line: 1, column: 2, scope: !275)
!277 = distinct !DISubprogram(name: "_CNW4mainE5Int64D3Ev", linkageName: "_CNW4mainE5Int64D3Ev", scope: null, file: !6, line: 1, type: !258, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!278 = !DILocation(line: 1, column: 2, scope: !277)
!279 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerD0Ev", linkageName: "_CNW4mainE20ExistentialContainerD0Ev", scope: !0, file: !6, line: 1, type: !280, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!280 = !DISubroutineType(types: !281)
!281 = !{!282}
!282 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !283, size: 64, align: 8)
!283 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.ExistentialContainer", file: !6, line: 1, flags: DIFlagFwdDecl, elements: !284)
!284 = !{!285, !286, !287}
!285 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !283, file: !6, line: 1, baseType: !183, size: 64, align: 64)
!286 = !DIDerivedType(tag: DW_TAG_member, name: "typeInfo", scope: !283, file: !6, line: 1, baseType: !208, size: 64, align: 64, offset: 64)
!287 = !DIDerivedType(tag: DW_TAG_member, name: "conformance", scope: !283, file: !6, line: 1, baseType: !221, size: 64, align: 64, offset: 128)
!288 = !DILocation(line: 1, column: 2, scope: !279)
!289 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainer14__default_initES0_L0", linkageName: "_CNW4mainE20ExistentialContainer14__default_initES0_L0", scope: !0, file: !6, line: 1, type: !290, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!290 = !DISubroutineType(types: !291)
!291 = !{!283}
!292 = !DILocation(line: 1, column: 2, scope: !289)
!293 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance", linkageName: "_CNW4mainE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance", scope: !0, file: !6, line: 1, type: !294, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!294 = !DISubroutineType(types: !295)
!295 = !{!282, !183, !208, !221}
!296 = !DILocation(line: 1, column: 2, scope: !293)
!297 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerC2ERS0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance", linkageName: "_CNW4mainE20ExistentialContainerC2ERS0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance", scope: !0, file: !6, line: 1, type: !294, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!298 = !DILocation(line: 1, column: 2, scope: !297)
!299 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerD3Ev", linkageName: "_CNW4mainE20ExistentialContainerD3Ev", scope: null, file: !6, line: 1, type: !280, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!300 = !DILocation(line: 1, column: 2, scope: !299)
