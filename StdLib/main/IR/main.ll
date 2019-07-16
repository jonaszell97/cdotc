; ModuleID = '/Users/Jonas/CDot/StdLib/main/main.dot'
source_filename = "/Users/Jonas/CDot/StdLib/main/main.dot"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-darwin18.0.0"

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
@_CTINW4mainE4BoolE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.Bool"*)* @_CNW4mainE4BoolD0Ev to void (i8*)*), i8* getelementptr inbounds ([25 x i8], [25 x i8]* @7, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"5", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([4 x %"struct.main.<private name>.ProtocolConformance"], [4 x %"struct.main.<private name>.ProtocolConformance"]* @"6", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 1 }>, %"struct.main.<private name>.Int64" <{ i64 1 }>, %"struct.main.<private name>.Int64" <{ i64 1 }> }>, align 8
@"5" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (%"struct.main.<private name>.Bool" (%"struct.main.<private name>.Bool")* @_CNW4mainE4Bool4copyES0_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (%"struct.main.<private name>.Bool"*)* @_CNW4mainE4BoolD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE4BoolENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTPNW4mainE4BoolENS_8CopyableE = constant [1 x i8*] [i8* bitcast (%"struct.main.<private name>.Bool" (%"struct.main.<private name>.Bool")* @_CNW4mainE4Bool4copyES0_L0 to i8*)], align 8
@_CTPNW4mainE4BoolENS_18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"6" = constant [4 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE4BoolENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE4BoolENS_8CopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE4BoolENS_18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE17ValueWitnessTableE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.ValueWitnessTable"*)* @_CNW4mainE17ValueWitnessTableD0Ev to void (i8*)*), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @8, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"7", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"8", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 16 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 16 }> }>, align 8
@"7" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.ValueWitnessTable"*)* @_CNW4mainE17ValueWitnessTableD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE17ValueWitnessTableENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"8" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE17ValueWitnessTableENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE19ProtocolConformanceE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.ProtocolConformance"*)* @_CNW4mainE19ProtocolConformanceD0Ev to void (i8*)*), i8* getelementptr inbounds ([40 x i8], [40 x i8]* @9, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"9", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"10", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 16 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 16 }> }>, align 8
@"9" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.ProtocolConformance"*)* @_CNW4mainE19ProtocolConformanceD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE19ProtocolConformanceENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"10" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE19ProtocolConformanceENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE8TypeInfoE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.TypeInfo"*)* @_CNW4mainE8TypeInfoD0Ev to void (i8*)*), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @10, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"11", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"12", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 72 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 72 }> }>, align 8
@"11" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.TypeInfo"*)* @_CNW4mainE8TypeInfoD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE8TypeInfoENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"12" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE8TypeInfoENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE5Int64E = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.Int64"*)* @_CNW4mainE5Int64D0Ev to void (i8*)*), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @11, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"13", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([4 x %"struct.main.<private name>.ProtocolConformance"], [4 x %"struct.main.<private name>.ProtocolConformance"]* @"14", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 8 }> }>, align 8
@"13" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (%"struct.main.<private name>.Int64" (%"struct.main.<private name>.Int64")* @_CNW4mainE5Int644copyES0_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (%"struct.main.<private name>.Int64"*)* @_CNW4mainE5Int64D0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE5Int64ENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTPNW4mainE5Int64ENS_8CopyableE = constant [1 x i8*] [i8* bitcast (%"struct.main.<private name>.Int64" (%"struct.main.<private name>.Int64")* @_CNW4mainE5Int644copyES0_L0 to i8*)], align 8
@_CTPNW4mainE5Int64ENS_18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"14" = constant [4 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE5Int64ENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE5Int64ENS_8CopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE5Int64ENS_18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE20ExistentialContainerE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.ExistentialContainer"*)* @_CNW4mainE20ExistentialContainerD0Ev to void (i8*)*), i8* getelementptr inbounds ([41 x i8], [41 x i8]* @12, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"15", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"16", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@"15" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.ExistentialContainer"*)* @_CNW4mainE20ExistentialContainerD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE20ExistentialContainerENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"16" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE20ExistentialContainerENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@0 = private unnamed_addr constant [17 x i8] c"main.MemoryOrder\00"
@1 = private unnamed_addr constant [24 x i8] c"main.<private name>.Any\00"
@2 = private unnamed_addr constant [30 x i8] c"main.<private name>.Equatable\00"
@3 = private unnamed_addr constant [37 x i8] c"main.<private name>.RawRepresentable\00"
@4 = private unnamed_addr constant [29 x i8] c"main.<private name>.Copyable\00"
@5 = private unnamed_addr constant [39 x i8] c"main.<private name>.ImplicitlyCopyable\00"
@6 = private unnamed_addr constant [17 x i8] c"main.Atomic<i64>\00"
@"4.1" = private global %"struct.main.<private name>.TypeInfo"* undef
@7 = private unnamed_addr constant [25 x i8] c"main.<private name>.Bool\00"
@8 = private unnamed_addr constant [38 x i8] c"main.<private name>.ValueWitnessTable\00"
@9 = private unnamed_addr constant [40 x i8] c"main.<private name>.ProtocolConformance\00"
@10 = private unnamed_addr constant [29 x i8] c"main.<private name>.TypeInfo\00"
@11 = private unnamed_addr constant [26 x i8] c"main.<private name>.Int64\00"
@12 = private unnamed_addr constant [41 x i8] c"main.<private name>.ExistentialContainer\00"
@13 = private unnamed_addr constant [13 x i8] c"value: %llu\0A\00"
@14 = private unnamed_addr constant [39 x i8] c"memory order %d is not valid for load\0A\00"

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
  %6 = call i64 @_CNW4mainE6AtomicIlE4loadES0_NW_0E11MemoryOrderEL0L11memoryOrder(%"struct.main.Atomic<i64>" %5, i32 5), !dbg !16
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
  %6 = bitcast %"struct.main.Atomic<i64>"* %0 to i64*
  call void @llvm.dbg.value(metadata i64* %6, metadata !47, metadata !DIExpression()), !dbg !49
  %7 = load i64, i64* %6, align 8, !dbg !49
  call void @llvm.dbg.declare(metadata i64** %3, metadata !47, metadata !DIExpression()), !dbg !49
  store i64* %6, i64** %3, align 8, !dbg !49
  %8 = bitcast i64** %3 to i8*, !dbg !49
  call void @llvm.lifetime.start.p0i8(i64 8, i8* %8), !dbg !49
  %9 = load i64, i64* %5, align 8, !dbg !50
  %10 = load i64*, i64** %3, align 8, !dbg !50
  %11 = load i32, i32* %4, align 4, !dbg !50
  %12 = call i32 @_CNW4mainE11MemoryOrderGa8rawValueES_L0(i32 %11), !dbg !50
  switch i32 %12, label %unreachable [
    i32 2, label %atomic.store.relaxed
    i32 5, label %atomic.store.release
    i32 7, label %atomic.store.seq_cst
  ], !dbg !50

unreachable:                                      ; preds = %entry
  unreachable, !dbg !50

atomic.store.relaxed:                             ; preds = %entry
  store atomic i64 %9, i64* %10 monotonic, align 8, !dbg !50
  br label %atomic.store.merge, !dbg !50

atomic.store.merge:                               ; preds = %atomic.store.seq_cst, %atomic.store.release, %atomic.store.relaxed
  %13 = bitcast i64** %3 to i8*, !dbg !50
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %13), !dbg !50
  %14 = bitcast i64* %5 to i8*, !dbg !50
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %14), !dbg !50
  ret void, !dbg !50

atomic.store.release:                             ; preds = %entry
  store atomic i64 %9, i64* %10 release, align 8, !dbg !50
  br label %atomic.store.merge, !dbg !50

atomic.store.seq_cst:                             ; preds = %entry
  store atomic i64 %9, i64* %10 seq_cst, align 8, !dbg !50
  br label %atomic.store.merge, !dbg !50
}

define i64 @_CNW4mainE6AtomicIlE4loadES0_NW_0E11MemoryOrderEL0L11memoryOrder(%"struct.main.Atomic<i64>", i32) !dbg !51 {
entry:
  %2 = alloca i64*, align 8
  %3 = alloca i32, align 4
  %4 = alloca %"struct.main.Atomic<i64>", align 8
  call void @llvm.dbg.declare(metadata %"struct.main.Atomic<i64>"* %4, metadata !54, metadata !DIExpression()), !dbg !55
  call void @llvm.dbg.value(metadata %"struct.main.Atomic<i64>" %0, metadata !54, metadata !DIExpression()), !dbg !55
  store %"struct.main.Atomic<i64>" %0, %"struct.main.Atomic<i64>"* %4, align 8, !dbg !55
  call void @llvm.dbg.declare(metadata i32* %3, metadata !56, metadata !DIExpression()), !dbg !55
  call void @llvm.dbg.value(metadata i32 %1, metadata !56, metadata !DIExpression()), !dbg !55
  store i32 %1, i32* %3, align 4, !dbg !55
  %5 = load i32, i32* %3, align 4, !dbg !57
  call void @_CNW4mainE6AtomicIlE19checkValidLoadOrderEMS0_NW_0E11MemoryOrderEL0L0(%"struct.main.<private name>.TypeInfo"* bitcast (%"struct.main.<private name>.TypeInfo"** @"4.1" to %"struct.main.<private name>.TypeInfo"*), i32 %5), !dbg !58
  %6 = bitcast %"struct.main.Atomic<i64>"* %4 to i64*
  call void @llvm.dbg.value(metadata i64* %6, metadata !59, metadata !DIExpression()), !dbg !60
  %7 = load i64, i64* %6, align 8, !dbg !60
  call void @llvm.dbg.declare(metadata i64** %2, metadata !59, metadata !DIExpression()), !dbg !60
  store i64* %6, i64** %2, align 8, !dbg !60
  %8 = bitcast i64** %2 to i8*, !dbg !60
  call void @llvm.lifetime.start.p0i8(i64 8, i8* %8), !dbg !60
  %9 = load i64*, i64** %2, align 8, !dbg !61
  %10 = load i32, i32* %3, align 4, !dbg !61
  %11 = call i32 @_CNW4mainE11MemoryOrderGa8rawValueES_L0(i32 %10), !dbg !61
  switch i32 %11, label %unreachable [
    i32 2, label %atomic.load.relaxed
    i32 3, label %atomic.load.consume
    i32 4, label %atomic.load.acquire
    i32 7, label %atomic.load.seq_cst
  ], !dbg !61

unreachable:                                      ; preds = %entry
  unreachable, !dbg !61

atomic.load.relaxed:                              ; preds = %entry
  %12 = load atomic i64, i64* %9 monotonic, align 8, !dbg !61
  br label %atomic.load.merge, !dbg !61

atomic.load.merge:                                ; preds = %atomic.load.seq_cst, %atomic.load.acquire, %atomic.load.consume, %atomic.load.relaxed
  %13 = phi i64 [ %12, %atomic.load.relaxed ], [ %15, %atomic.load.consume ], [ %16, %atomic.load.acquire ], [ %17, %atomic.load.seq_cst ]
  %14 = bitcast i64** %2 to i8*, !dbg !61
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %14), !dbg !61
  ret i64 %13, !dbg !61

atomic.load.consume:                              ; preds = %entry
  %15 = load atomic i64, i64* %9 consume, align 8, !dbg !61
  br label %atomic.load.merge, !dbg !61

atomic.load.acquire:                              ; preds = %entry
  %16 = load atomic i64, i64* %9 acquire, align 8, !dbg !61
  br label %atomic.load.merge, !dbg !61

atomic.load.seq_cst:                              ; preds = %entry
  %17 = load atomic i64, i64* %9 seq_cst, align 8, !dbg !61
  br label %atomic.load.merge, !dbg !61
}

declare i32 @printf(i8*, ...)

define void @_CNW4mainE6AtomicIlED0Ev(%"struct.main.Atomic<i64>"*) !dbg !62 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE6AtomicIlED3Ev(%"struct.main.Atomic<i64>"* %0), !dbg !65
  ret void, !dbg !65
}

define void @_CNW4mainE11MemoryOrderD0Ev(i32*) !dbg !66 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE11MemoryOrderD3Ev(i32* %0), !dbg !70
  ret void, !dbg !70
}

define i32 @_CNW4mainE11MemoryOrder4copyES_L0(i32) !dbg !71 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret i32 %0, !dbg !74
}

define %"struct.main.<private name>.Bool" @"_CNW4mainE11MemoryOrderXi2==ES_S_L0L0"(i32, i32) !dbg !75 {
alloca_block:
  %2 = alloca %"struct.main.<private name>.Bool", align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = icmp eq i32 %0, %1, !dbg !78
  %4 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %2, align 1, !dbg !78
  %5 = extractvalue %"struct.main.<private name>.Bool" %4, 0, !dbg !78
  %6 = alloca i1, !dbg !78
  store i1 %5, i1* %6, !dbg !78
  store i1 %3, i1* %6, !dbg !78
  ret %"struct.main.<private name>.Bool" %4, !dbg !78
}

define %"struct.main.<private name>.Bool" @"_CNW4mainE11MemoryOrderXi2!=ES_S_L0L0"(i32, i32) !dbg !79 {
alloca_block:
  %2 = alloca %"struct.main.<private name>.Bool", align 1
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !80, metadata !DIExpression()), !dbg !81
  call void @llvm.dbg.value(metadata i32 %0, metadata !80, metadata !DIExpression()), !dbg !81
  store i32 %0, i32* %4, align 4, !dbg !81
  call void @llvm.dbg.declare(metadata i32* %3, metadata !82, metadata !DIExpression()), !dbg !81
  call void @llvm.dbg.value(metadata i32 %1, metadata !82, metadata !DIExpression()), !dbg !81
  store i32 %1, i32* %3, align 4, !dbg !81
  %5 = load i32, i32* %4, align 4, !dbg !81
  %6 = load i32, i32* %3, align 4, !dbg !81
  %7 = call %"struct.main.<private name>.Bool" @"_CNW4mainE11MemoryOrderXi2==ES_S_L0L0"(i32 %5, i32 %6), !dbg !81
  %8 = call %"struct.main.<private name>.Bool" @"_CNW4mainE4BoolXp1!ES0_L0"(%"struct.main.<private name>.Bool" %7), !dbg !81
  store %"struct.main.<private name>.Bool" %7, %"struct.main.<private name>.Bool"* %2, align 1, !dbg !81
  call void @_CNW4mainE4BoolD0Ev(%"struct.main.<private name>.Bool"* %2), !dbg !81
  ret %"struct.main.<private name>.Bool" %8, !dbg !81
}

define void @_CNW4mainE11MemoryOrderC1EiL8rawValue(i32*, i32) !dbg !83 {
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
  ], !dbg !87

init.failure:                                     ; preds = %entry
  %9 = load i32, i32* %0, align 4, !dbg !87
  %10 = bitcast i32* %0 to i8*, !dbg !87
  call void @llvm.memset.p0i8.i64(i8* %10, i8 0, i64 8, i32 1, i1 false), !dbg !87
  ret void, !dbg !87

init.case.NotAtomic:                              ; preds = %entry
  store i32 0, i32* %8, align 4, !dbg !87
  %11 = bitcast i32* %0 to i8*, !dbg !87
  %12 = bitcast i32* %8 to i8*, !dbg !87
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %11, i8* %12, i64 8, i32 1, i1 false), !dbg !87
  ret void, !dbg !87

init.case.Relaxed:                                ; preds = %entry
  store i32 1, i32* %7, align 4, !dbg !87
  %13 = bitcast i32* %0 to i8*, !dbg !87
  %14 = bitcast i32* %7 to i8*, !dbg !87
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %13, i8* %14, i64 8, i32 1, i1 false), !dbg !87
  ret void, !dbg !87

init.case.Consume:                                ; preds = %entry
  store i32 2, i32* %6, align 4, !dbg !87
  %15 = bitcast i32* %0 to i8*, !dbg !87
  %16 = bitcast i32* %6 to i8*, !dbg !87
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %15, i8* %16, i64 8, i32 1, i1 false), !dbg !87
  ret void, !dbg !87

init.case.Acquire:                                ; preds = %entry
  store i32 3, i32* %5, align 4, !dbg !87
  %17 = bitcast i32* %0 to i8*, !dbg !87
  %18 = bitcast i32* %5 to i8*, !dbg !87
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %17, i8* %18, i64 8, i32 1, i1 false), !dbg !87
  ret void, !dbg !87

init.case.Release:                                ; preds = %entry
  store i32 4, i32* %4, align 4, !dbg !87
  %19 = bitcast i32* %0 to i8*, !dbg !87
  %20 = bitcast i32* %4 to i8*, !dbg !87
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %19, i8* %20, i64 8, i32 1, i1 false), !dbg !87
  ret void, !dbg !87

init.case.AcquireRelease:                         ; preds = %entry
  store i32 5, i32* %3, align 4, !dbg !87
  %21 = bitcast i32* %0 to i8*, !dbg !87
  %22 = bitcast i32* %3 to i8*, !dbg !87
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %21, i8* %22, i64 8, i32 1, i1 false), !dbg !87
  ret void, !dbg !87

init.case.SequentiallyConsistent:                 ; preds = %entry
  store i32 6, i32* %2, align 4, !dbg !87
  %23 = bitcast i32* %0 to i8*, !dbg !87
  %24 = bitcast i32* %2 to i8*, !dbg !87
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %23, i8* %24, i64 8, i32 1, i1 false), !dbg !87
  ret void, !dbg !87
}

define i32 @_CNW4mainE11MemoryOrderGa8rawValueES_L0(i32) !dbg !88 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret i32 %0, !dbg !89
}

define void @_CNW4mainE11MemoryOrderD3Ev(i32*) !dbg !90 {
merge:
  ret void, !dbg !91
}

define i1 @"_CNW4mainE11MemoryOrderXi2~=ES_S_L0L0"(i32, i32) !dbg !92 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !93, metadata !DIExpression()), !dbg !94
  call void @llvm.dbg.value(metadata i32 %0, metadata !93, metadata !DIExpression()), !dbg !94
  store i32 %0, i32* %3, align 4, !dbg !94
  call void @llvm.dbg.declare(metadata i32* %2, metadata !95, metadata !DIExpression()), !dbg !94
  call void @llvm.dbg.value(metadata i32 %1, metadata !95, metadata !DIExpression()), !dbg !94
  store i32 %1, i32* %2, align 4, !dbg !94
  %4 = load i32, i32* %3, align 4, !dbg !96
  %5 = call i32 @_CNW4mainE11MemoryOrderGa8rawValueES_L0(i32 %4), !dbg !96
  %6 = sext i32 %5 to i64, !dbg !96
  %7 = load i32, i32* %2, align 4, !dbg !96
  %8 = call i32 @_CNW4mainE11MemoryOrderGa8rawValueES_L0(i32 %7), !dbg !96
  %9 = sext i32 %8 to i64, !dbg !96
  %10 = icmp eq i64 %6, %9, !dbg !96
  ret i1 %10, !dbg !96
}

define %"struct.main.<private name>.Bool" @"_CNW4mainE4BoolXp1!ES0_L0"(%"struct.main.<private name>.Bool") !dbg !97 {
entry:
  %1 = alloca %"struct.main.<private name>.Bool", align 1
  %2 = alloca %"struct.main.<private name>.Bool", align 1
  call void @llvm.dbg.declare(metadata %"struct.main.<private name>.Bool"* %2, metadata !104, metadata !DIExpression()), !dbg !105
  call void @llvm.dbg.value(metadata %"struct.main.<private name>.Bool" %0, metadata !104, metadata !DIExpression()), !dbg !105
  store %"struct.main.<private name>.Bool" %0, %"struct.main.<private name>.Bool"* %2, align 1, !dbg !105
  %3 = bitcast %"struct.main.<private name>.Bool"* %2 to i1*
  %4 = load i1, i1* %3, align 1, !dbg !105
  %5 = xor i1 %4, true, !dbg !105
  call void @_CNW4mainE4BoolC1EbL5value(%"struct.main.<private name>.Bool"* %1, i1 %5), !dbg !105
  %6 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %1, !dbg !105
  ret %"struct.main.<private name>.Bool" %6, !dbg !105
}

define void @_CNW4mainE4BoolD0Ev(%"struct.main.<private name>.Bool"*) !dbg !106 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE4BoolD3Ev(%"struct.main.<private name>.Bool"* %0), !dbg !110
  ret void, !dbg !110
}

declare void @_CNW4mainE11MemoryOrderC2ERS_iL0L8rawValue(i32*, i32)

define void @_CNW4mainE4funcElL2v1(i64) !dbg !111 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !114, metadata !DIExpression()), !dbg !115
  call void @llvm.dbg.value(metadata i64 %0, metadata !114, metadata !DIExpression()), !dbg !115
  store i64 %0, i64* %1, align 8, !dbg !115
  ret void, !dbg !116
}

define void @_CNW4mainE4funcElL2v2(i64) !dbg !117 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !118, metadata !DIExpression()), !dbg !119
  call void @llvm.dbg.value(metadata i64 %0, metadata !118, metadata !DIExpression()), !dbg !119
  store i64 %0, i64* %1, align 8, !dbg !119
  ret void, !dbg !120
}

define %"struct.main.Atomic<i64>" @_CNW4mainE6AtomicIlE4copyES0_L0(%"struct.main.Atomic<i64>") !dbg !121 {
alloca_block:
  %1 = alloca %"struct.main.Atomic<i64>", align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.Atomic<i64>", %"struct.main.Atomic<i64>"* %1, align 8, !dbg !124
  %3 = extractvalue %"struct.main.Atomic<i64>" %2, 0, !dbg !124
  %4 = alloca i64, !dbg !124
  store i64 %3, i64* %4, !dbg !124
  %5 = extractvalue %"struct.main.Atomic<i64>" %0, 0, !dbg !124
  %6 = alloca i64, !dbg !124
  store i64 %5, i64* %6, !dbg !124
  %7 = load i64, i64* %6, !dbg !124
  store i64 %7, i64* %4, !dbg !124
  ret %"struct.main.Atomic<i64>" %2, !dbg !124
}

define void @_CNW4mainE6AtomicIlE14__default_initES0_L0(%"struct.main.Atomic<i64>") !dbg !125 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !126
}

define void @_CNW4mainE6AtomicIlED3Ev(%"struct.main.Atomic<i64>"*) !dbg !127 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.Atomic<i64>", %"struct.main.Atomic<i64>"* %0, align 8, !dbg !128
  %2 = extractvalue %"struct.main.Atomic<i64>" %1, 0, !dbg !128
  %3 = alloca i64, !dbg !128
  store i64 %2, i64* %3, !dbg !128
  %4 = bitcast i64* %3 to i8*, !dbg !128
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %4), !dbg !128
  ret void, !dbg !128
}

define void @_CNW4mainE6AtomicIlEC2ERS0_lL0L5value(%"struct.main.Atomic<i64>"*, i64) !dbg !129 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata %"struct.main.Atomic<i64>"* %0, metadata !130, metadata !DIExpression()), !dbg !131
  call void @llvm.dbg.declare(metadata i64* %2, metadata !132, metadata !DIExpression()), !dbg !131
  call void @llvm.dbg.value(metadata i64 %1, metadata !132, metadata !DIExpression()), !dbg !131
  store i64 %1, i64* %2, align 8, !dbg !131
  %3 = bitcast %"struct.main.Atomic<i64>"* %0 to i64*
  %4 = load i64, i64* %2, align 8, !dbg !133
  %5 = bitcast i64* %3 to i8*, !dbg !133
  call void @llvm.lifetime.start.p0i8(i64 8, i8* %5), !dbg !133
  store i64 %4, i64* %3, align 8, !dbg !133
  %6 = bitcast i64* %2 to i8*, !dbg !133
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %6), !dbg !133
  ret void, !dbg !133
}

declare void @_CNW4mainE6AtomicIlEC1ElL5value(%"struct.main.Atomic<i64>"*, i64)

define void @_CNW4mainE6AtomicIlEC2ERS0_lL0L10weirdValue(%"struct.main.Atomic<i64>"*, i64) !dbg !134 {
alloca_block:
  %2 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata %"struct.main.Atomic<i64>"* %0, metadata !135, metadata !DIExpression()), !dbg !136
  call void @llvm.dbg.declare(metadata i64* %2, metadata !137, metadata !DIExpression()), !dbg !136
  call void @llvm.dbg.value(metadata i64 %1, metadata !137, metadata !DIExpression()), !dbg !136
  store i64 %1, i64* %2, align 8, !dbg !136
  %3 = load i64, i64* %2, align 8, !dbg !138
  call void @_CNW4mainE6AtomicIlEC2ERS0_lL0L5value(%"struct.main.Atomic<i64>"* %0, i64 %3), !dbg !139
  %4 = bitcast i64* %2 to i8*, !dbg !139
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %4), !dbg !139
  ret void, !dbg !139
}

define void @_CNW4mainE6AtomicIlE19checkValidLoadOrderEMS0_NW_0E11MemoryOrderEL0L0(%"struct.main.<private name>.TypeInfo"*, i32) !dbg !140 {
match.cmp:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata %"struct.main.<private name>.TypeInfo"* %0, metadata !147, metadata !DIExpression()), !dbg !148
  call void @llvm.dbg.declare(metadata i32* %2, metadata !149, metadata !DIExpression()), !dbg !148
  call void @llvm.dbg.value(metadata i32 %1, metadata !149, metadata !DIExpression()), !dbg !148
  store i32 %1, i32* %2, align 4, !dbg !148
  %3 = load i32, i32* %2, align 4, !dbg !150
  %4 = call i1 @"_CNW4mainE11MemoryOrderXi2~=ES_S_L0L0"(i32 %3, i32 4), !dbg !150
  br i1 %4, label %match.merge, label %match.cmp.0, !dbg !150

match.cmp.0:                                      ; preds = %match.cmp
  %5 = call i1 @"_CNW4mainE11MemoryOrderXi2~=ES_S_L0L0"(i32 %3, i32 5), !dbg !151
  br i1 %5, label %match.merge, label %match.cmp.1, !dbg !151

match.merge:                                      ; preds = %match.cmp.1, %match.cmp, %match.cmp.0
  %6 = load i32, i32* %2, align 4, !dbg !152
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([39 x i8], [39 x i8]* @14, i32 0, i32 0), i32 %6), !dbg !153
  ret void, !dbg !151

match.cmp.1:                                      ; preds = %match.cmp.0
  %8 = call i1 @"_CNW4mainE11MemoryOrderXi2~=ES_S_L0L0"(i32 %3, i32 0), !dbg !151
  br i1 %8, label %match.merge, label %match.body.default, !dbg !151

match.body.default:                               ; preds = %match.cmp.1
  ret void, !dbg !154
}

define %"struct.main.<private name>.Bool" @_CNW4mainE4Bool4copyES0_L0(%"struct.main.<private name>.Bool") !dbg !155 {
alloca_block:
  %1 = alloca %"struct.main.<private name>.Bool", align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %1, align 1, !dbg !156
  %3 = extractvalue %"struct.main.<private name>.Bool" %2, 0, !dbg !156
  %4 = alloca i1, !dbg !156
  store i1 %3, i1* %4, !dbg !156
  %5 = extractvalue %"struct.main.<private name>.Bool" %0, 0, !dbg !156
  %6 = alloca i1, !dbg !156
  store i1 %5, i1* %6, !dbg !156
  %7 = load i1, i1* %6, !dbg !156
  store i1 %7, i1* %4, !dbg !156
  ret %"struct.main.<private name>.Bool" %2, !dbg !156
}

define void @_CNW4mainE4Bool14__default_initES0_L0(%"struct.main.<private name>.Bool") !dbg !157 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !158
}

define void @_CNW4mainE4BoolC1EbL5value(%"struct.main.<private name>.Bool"*, i1) !dbg !159 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %0, align 1, !dbg !162
  call void @_CNW4mainE4Bool14__default_initES0_L0(%"struct.main.<private name>.Bool" %2), !dbg !162
  %3 = extractvalue %"struct.main.<private name>.Bool" %2, 0, !dbg !162
  %4 = alloca i1, !dbg !162
  store i1 %3, i1* %4, !dbg !162
  store i1 %1, i1* %4, !dbg !162
  ret void, !dbg !162
}

define void @_CNW4mainE4BoolC2ERS0_bL0L5value(%"struct.main.<private name>.Bool"*, i1) !dbg !163 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %0, align 1, !dbg !164
  %3 = extractvalue %"struct.main.<private name>.Bool" %2, 0, !dbg !164
  %4 = alloca i1, !dbg !164
  store i1 %3, i1* %4, !dbg !164
  store i1 %1, i1* %4, !dbg !164
  ret void, !dbg !164
}

define void @_CNW4mainE4BoolD3Ev(%"struct.main.<private name>.Bool"*) !dbg !165 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %0, align 1, !dbg !166
  %2 = extractvalue %"struct.main.<private name>.Bool" %1, 0, !dbg !166
  %3 = alloca i1, !dbg !166
  store i1 %2, i1* %3, !dbg !166
  %4 = bitcast i1* %3 to i8*, !dbg !166
  call void @llvm.lifetime.end.p0i8(i64 1, i8* %4), !dbg !166
  ret void, !dbg !166
}

define void @_CNW4mainE17ValueWitnessTableD0Ev(%"struct.main.<private name>.ValueWitnessTable"*) !dbg !167 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE17ValueWitnessTableD3Ev(%"struct.main.<private name>.ValueWitnessTable"* %0), !dbg !182
  ret void, !dbg !182
}

define void @_CNW4mainE17ValueWitnessTable14__default_initES0_L0(%"struct.main.<private name>.ValueWitnessTable") !dbg !183 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !186
}

define void @_CNW4mainE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit(%"struct.main.<private name>.ValueWitnessTable"*, void (i8*, i8*)*, void (i8*)*) !dbg !187 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.main.<private name>.ValueWitnessTable", %"struct.main.<private name>.ValueWitnessTable"* %0, align 8, !dbg !190
  call void @_CNW4mainE17ValueWitnessTable14__default_initES0_L0(%"struct.main.<private name>.ValueWitnessTable" %3), !dbg !190
  %4 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %3, 0, !dbg !190
  %5 = alloca void (i8*, i8*)*, !dbg !190
  store void (i8*, i8*)* %4, void (i8*, i8*)** %5, !dbg !190
  store void (i8*, i8*)* %1, void (i8*, i8*)** %5, !dbg !190
  %6 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %3, 1, !dbg !190
  %7 = alloca void (i8*)*, !dbg !190
  store void (i8*)* %6, void (i8*)** %7, !dbg !190
  store void (i8*)* %2, void (i8*)** %7, !dbg !190
  ret void, !dbg !190
}

define void @_CNW4mainE17ValueWitnessTableC2ERS0_DoFvPvKPvEDoFvS2_EL0L6copyFnL6deinit(%"struct.main.<private name>.ValueWitnessTable"*, void (i8*, i8*)*, void (i8*)*) !dbg !191 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.main.<private name>.ValueWitnessTable", %"struct.main.<private name>.ValueWitnessTable"* %0, align 8, !dbg !192
  %4 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %3, 0, !dbg !192
  %5 = alloca void (i8*, i8*)*, !dbg !192
  store void (i8*, i8*)* %4, void (i8*, i8*)** %5, !dbg !192
  store void (i8*, i8*)* %1, void (i8*, i8*)** %5, !dbg !192
  %6 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %3, 1, !dbg !192
  %7 = alloca void (i8*)*, !dbg !192
  store void (i8*)* %6, void (i8*)** %7, !dbg !192
  store void (i8*)* %2, void (i8*)** %7, !dbg !192
  ret void, !dbg !192
}

define void @_CNW4mainE17ValueWitnessTableD3Ev(%"struct.main.<private name>.ValueWitnessTable"*) !dbg !193 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.ValueWitnessTable", %"struct.main.<private name>.ValueWitnessTable"* %0, align 8, !dbg !194
  %2 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %1, 0, !dbg !194
  %3 = alloca void (i8*, i8*)*, !dbg !194
  store void (i8*, i8*)* %2, void (i8*, i8*)** %3, !dbg !194
  %4 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %1, 1, !dbg !194
  %5 = alloca void (i8*)*, !dbg !194
  store void (i8*)* %4, void (i8*)** %5, !dbg !194
  %6 = bitcast void (i8*)** %5 to i8*, !dbg !194
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %6), !dbg !194
  %7 = bitcast void (i8*, i8*)** %3 to i8*, !dbg !194
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %7), !dbg !194
  ret void, !dbg !194
}

define void @_CNW4mainE19ProtocolConformanceD0Ev(%"struct.main.<private name>.ProtocolConformance"*) !dbg !195 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE19ProtocolConformanceD3Ev(%"struct.main.<private name>.ProtocolConformance"* %0), !dbg !221
  ret void, !dbg !221
}

define void @_CNW4mainE19ProtocolConformance14__default_initES0_L0(%"struct.main.<private name>.ProtocolConformance") !dbg !222 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !225
}

define void @_CNW4mainE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable(%"struct.main.<private name>.ProtocolConformance"*, %"struct.main.<private name>.TypeInfo"*, i8**) !dbg !226 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.main.<private name>.ProtocolConformance", %"struct.main.<private name>.ProtocolConformance"* %0, align 8, !dbg !229
  call void @_CNW4mainE19ProtocolConformance14__default_initES0_L0(%"struct.main.<private name>.ProtocolConformance" %3), !dbg !229
  %4 = extractvalue %"struct.main.<private name>.ProtocolConformance" %3, 0, !dbg !229
  %5 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !229
  store %"struct.main.<private name>.TypeInfo"* %4, %"struct.main.<private name>.TypeInfo"** %5, !dbg !229
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %5, !dbg !229
  %6 = extractvalue %"struct.main.<private name>.ProtocolConformance" %3, 1, !dbg !229
  %7 = alloca i8**, !dbg !229
  store i8** %6, i8*** %7, !dbg !229
  store i8** %2, i8*** %7, !dbg !229
  ret void, !dbg !229
}

define void @_CNW4mainE19ProtocolConformanceC2ERS0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable(%"struct.main.<private name>.ProtocolConformance"*, %"struct.main.<private name>.TypeInfo"*, i8**) !dbg !230 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.main.<private name>.ProtocolConformance", %"struct.main.<private name>.ProtocolConformance"* %0, align 8, !dbg !231
  %4 = extractvalue %"struct.main.<private name>.ProtocolConformance" %3, 0, !dbg !231
  %5 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !231
  store %"struct.main.<private name>.TypeInfo"* %4, %"struct.main.<private name>.TypeInfo"** %5, !dbg !231
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %5, !dbg !231
  %6 = extractvalue %"struct.main.<private name>.ProtocolConformance" %3, 1, !dbg !231
  %7 = alloca i8**, !dbg !231
  store i8** %6, i8*** %7, !dbg !231
  store i8** %2, i8*** %7, !dbg !231
  ret void, !dbg !231
}

define void @_CNW4mainE19ProtocolConformanceD3Ev(%"struct.main.<private name>.ProtocolConformance"*) !dbg !232 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.ProtocolConformance", %"struct.main.<private name>.ProtocolConformance"* %0, align 8, !dbg !233
  %2 = extractvalue %"struct.main.<private name>.ProtocolConformance" %1, 0, !dbg !233
  %3 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !233
  store %"struct.main.<private name>.TypeInfo"* %2, %"struct.main.<private name>.TypeInfo"** %3, !dbg !233
  %4 = extractvalue %"struct.main.<private name>.ProtocolConformance" %1, 1, !dbg !233
  %5 = alloca i8**, !dbg !233
  store i8** %4, i8*** %5, !dbg !233
  %6 = bitcast i8*** %5 to i8*, !dbg !233
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %6), !dbg !233
  %7 = bitcast %"struct.main.<private name>.TypeInfo"** %3 to i8*, !dbg !233
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %7), !dbg !233
  ret void, !dbg !233
}

define void @_CNW4mainE8TypeInfoD0Ev(%"struct.main.<private name>.TypeInfo"*) !dbg !234 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE8TypeInfoD3Ev(%"struct.main.<private name>.TypeInfo"* %0), !dbg !238
  ret void, !dbg !238
}

define void @_CNW4mainE8TypeInfo14__default_initES0_L0(%"struct.main.<private name>.TypeInfo"*) !dbg !239 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !242
}

define void @_CNW4mainE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride(%"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.TypeInfo"*, i8**, void (i8*)*, i8*, %"struct.main.<private name>.ValueWitnessTable"*, %"struct.main.<private name>.ProtocolConformance"*, i64, i64, i64) !dbg !243 {
entry:
  call void @_CNW4mainE8TypeInfo14__default_initES0_L0(%"struct.main.<private name>.TypeInfo"* %0), !dbg !246
  %10 = bitcast %"struct.main.<private name>.TypeInfo"* %0 to %"struct.main.<private name>.TypeInfo"**
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %10, align 8, !dbg !246
  %11 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !246
  store i8** %2, i8*** %11, align 8, !dbg !246
  %12 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !246
  store void (i8*)* %3, void (i8*)** %12, align 8, !dbg !246
  %13 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !246
  store i8* %4, i8** %13, align 8, !dbg !246
  %14 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !246
  store %"struct.main.<private name>.ValueWitnessTable"* %5, %"struct.main.<private name>.ValueWitnessTable"** %14, align 8, !dbg !246
  %15 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !246
  store %"struct.main.<private name>.ProtocolConformance"* %6, %"struct.main.<private name>.ProtocolConformance"** %15, align 8, !dbg !246
  %16 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !246
  store i64 %7, i64* %16, align 8, !dbg !246
  %17 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !246
  store i64 %8, i64* %17, align 8, !dbg !246
  %18 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !246
  store i64 %9, i64* %18, align 8, !dbg !246
  ret void, !dbg !246
}

define void @_CNW4mainE8TypeInfoC2ERS0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride(%"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.TypeInfo"*, i8**, void (i8*)*, i8*, %"struct.main.<private name>.ValueWitnessTable"*, %"struct.main.<private name>.ProtocolConformance"*, i64, i64, i64) !dbg !247 {
entry:
  %10 = bitcast %"struct.main.<private name>.TypeInfo"* %0 to %"struct.main.<private name>.TypeInfo"**
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %10, align 8, !dbg !248
  %11 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !248
  store i8** %2, i8*** %11, align 8, !dbg !248
  %12 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !248
  store void (i8*)* %3, void (i8*)** %12, align 8, !dbg !248
  %13 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !248
  store i8* %4, i8** %13, align 8, !dbg !248
  %14 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !248
  store %"struct.main.<private name>.ValueWitnessTable"* %5, %"struct.main.<private name>.ValueWitnessTable"** %14, align 8, !dbg !248
  %15 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !248
  store %"struct.main.<private name>.ProtocolConformance"* %6, %"struct.main.<private name>.ProtocolConformance"** %15, align 8, !dbg !248
  %16 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !248
  store i64 %7, i64* %16, align 8, !dbg !248
  %17 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !248
  store i64 %8, i64* %17, align 8, !dbg !248
  %18 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !248
  store i64 %9, i64* %18, align 8, !dbg !248
  ret void, !dbg !248
}

define void @_CNW4mainE8TypeInfoD3Ev(%"struct.main.<private name>.TypeInfo"*) !dbg !249 {
entry:
  %1 = bitcast %"struct.main.<private name>.TypeInfo"* %0 to %"struct.main.<private name>.TypeInfo"**
  %2 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !250
  %3 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !250
  %4 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !250
  %5 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !250
  %6 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !250
  %7 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !250
  %8 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !250
  %9 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !250
  %10 = bitcast i64* %9 to i8*, !dbg !250
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %10), !dbg !250
  %11 = bitcast i64* %8 to i8*, !dbg !250
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %11), !dbg !250
  %12 = bitcast i64* %7 to i8*, !dbg !250
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %12), !dbg !250
  %13 = bitcast %"struct.main.<private name>.ProtocolConformance"** %6 to i8*, !dbg !250
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %13), !dbg !250
  %14 = bitcast %"struct.main.<private name>.ValueWitnessTable"** %5 to i8*, !dbg !250
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %14), !dbg !250
  %15 = bitcast i8** %4 to i8*, !dbg !250
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %15), !dbg !250
  %16 = bitcast void (i8*)** %3 to i8*, !dbg !250
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %16), !dbg !250
  %17 = bitcast i8*** %2 to i8*, !dbg !250
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %17), !dbg !250
  %18 = bitcast %"struct.main.<private name>.TypeInfo"** %1 to i8*, !dbg !250
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %18), !dbg !250
  ret void, !dbg !250
}

define void @_CNW4mainE5Int64D0Ev(%"struct.main.<private name>.Int64"*) !dbg !251 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE5Int64D3Ev(%"struct.main.<private name>.Int64"* %0), !dbg !258
  ret void, !dbg !258
}

define %"struct.main.<private name>.Int64" @_CNW4mainE5Int644copyES0_L0(%"struct.main.<private name>.Int64") !dbg !259 {
alloca_block:
  %1 = alloca %"struct.main.<private name>.Int64", align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Int64", %"struct.main.<private name>.Int64"* %1, align 8, !dbg !262
  %3 = extractvalue %"struct.main.<private name>.Int64" %2, 0, !dbg !262
  %4 = alloca i64, !dbg !262
  store i64 %3, i64* %4, !dbg !262
  %5 = extractvalue %"struct.main.<private name>.Int64" %0, 0, !dbg !262
  %6 = alloca i64, !dbg !262
  store i64 %5, i64* %6, !dbg !262
  %7 = load i64, i64* %6, !dbg !262
  store i64 %7, i64* %4, !dbg !262
  ret %"struct.main.<private name>.Int64" %2, !dbg !262
}

define void @_CNW4mainE5Int6414__default_initES0_L0(%"struct.main.<private name>.Int64") !dbg !263 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !264
}

define void @_CNW4mainE5Int64C1ElL5value(%"struct.main.<private name>.Int64"*, i64) !dbg !265 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Int64", %"struct.main.<private name>.Int64"* %0, align 8, !dbg !268
  call void @_CNW4mainE5Int6414__default_initES0_L0(%"struct.main.<private name>.Int64" %2), !dbg !268
  %3 = extractvalue %"struct.main.<private name>.Int64" %2, 0, !dbg !268
  %4 = alloca i64, !dbg !268
  store i64 %3, i64* %4, !dbg !268
  store i64 %1, i64* %4, !dbg !268
  ret void, !dbg !268
}

define void @_CNW4mainE5Int64C2ERS0_lL0L5value(%"struct.main.<private name>.Int64"*, i64) !dbg !269 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Int64", %"struct.main.<private name>.Int64"* %0, align 8, !dbg !270
  %3 = extractvalue %"struct.main.<private name>.Int64" %2, 0, !dbg !270
  %4 = alloca i64, !dbg !270
  store i64 %3, i64* %4, !dbg !270
  store i64 %1, i64* %4, !dbg !270
  ret void, !dbg !270
}

define void @_CNW4mainE5Int64D3Ev(%"struct.main.<private name>.Int64"*) !dbg !271 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.Int64", %"struct.main.<private name>.Int64"* %0, align 8, !dbg !272
  %2 = extractvalue %"struct.main.<private name>.Int64" %1, 0, !dbg !272
  %3 = alloca i64, !dbg !272
  store i64 %2, i64* %3, !dbg !272
  %4 = bitcast i64* %3 to i8*, !dbg !272
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %4), !dbg !272
  ret void, !dbg !272
}

define void @_CNW4mainE20ExistentialContainerD0Ev(%"struct.main.<private name>.ExistentialContainer"*) !dbg !273 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE20ExistentialContainerD3Ev(%"struct.main.<private name>.ExistentialContainer"* %0), !dbg !282
  ret void, !dbg !282
}

define void @_CNW4mainE20ExistentialContainer14__default_initES0_L0(%"struct.main.<private name>.ExistentialContainer") !dbg !283 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !286
}

define void @_CNW4mainE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance(%"struct.main.<private name>.ExistentialContainer"*, i8*, %"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.ProtocolConformance"*) !dbg !287 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %4 = load %"struct.main.<private name>.ExistentialContainer", %"struct.main.<private name>.ExistentialContainer"* %0, align 8, !dbg !290
  call void @_CNW4mainE20ExistentialContainer14__default_initES0_L0(%"struct.main.<private name>.ExistentialContainer" %4), !dbg !290
  %5 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 0, !dbg !290
  %6 = alloca i8*, !dbg !290
  store i8* %5, i8** %6, !dbg !290
  store i8* %1, i8** %6, !dbg !290
  %7 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 1, !dbg !290
  %8 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !290
  store %"struct.main.<private name>.TypeInfo"* %7, %"struct.main.<private name>.TypeInfo"** %8, !dbg !290
  store %"struct.main.<private name>.TypeInfo"* %2, %"struct.main.<private name>.TypeInfo"** %8, !dbg !290
  %9 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 2, !dbg !290
  %10 = alloca %"struct.main.<private name>.ProtocolConformance"*, !dbg !290
  store %"struct.main.<private name>.ProtocolConformance"* %9, %"struct.main.<private name>.ProtocolConformance"** %10, !dbg !290
  store %"struct.main.<private name>.ProtocolConformance"* %3, %"struct.main.<private name>.ProtocolConformance"** %10, !dbg !290
  ret void, !dbg !290
}

define void @_CNW4mainE20ExistentialContainerC2ERS0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance(%"struct.main.<private name>.ExistentialContainer"*, i8*, %"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.ProtocolConformance"*) !dbg !291 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %4 = load %"struct.main.<private name>.ExistentialContainer", %"struct.main.<private name>.ExistentialContainer"* %0, align 8, !dbg !292
  %5 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 0, !dbg !292
  %6 = alloca i8*, !dbg !292
  store i8* %5, i8** %6, !dbg !292
  store i8* %1, i8** %6, !dbg !292
  %7 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 1, !dbg !292
  %8 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !292
  store %"struct.main.<private name>.TypeInfo"* %7, %"struct.main.<private name>.TypeInfo"** %8, !dbg !292
  store %"struct.main.<private name>.TypeInfo"* %2, %"struct.main.<private name>.TypeInfo"** %8, !dbg !292
  %9 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 2, !dbg !292
  %10 = alloca %"struct.main.<private name>.ProtocolConformance"*, !dbg !292
  store %"struct.main.<private name>.ProtocolConformance"* %9, %"struct.main.<private name>.ProtocolConformance"** %10, !dbg !292
  store %"struct.main.<private name>.ProtocolConformance"* %3, %"struct.main.<private name>.ProtocolConformance"** %10, !dbg !292
  ret void, !dbg !292
}

define void @_CNW4mainE20ExistentialContainerD3Ev(%"struct.main.<private name>.ExistentialContainer"*) !dbg !293 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.ExistentialContainer", %"struct.main.<private name>.ExistentialContainer"* %0, align 8, !dbg !294
  %2 = extractvalue %"struct.main.<private name>.ExistentialContainer" %1, 0, !dbg !294
  %3 = alloca i8*, !dbg !294
  store i8* %2, i8** %3, !dbg !294
  %4 = extractvalue %"struct.main.<private name>.ExistentialContainer" %1, 1, !dbg !294
  %5 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !294
  store %"struct.main.<private name>.TypeInfo"* %4, %"struct.main.<private name>.TypeInfo"** %5, !dbg !294
  %6 = extractvalue %"struct.main.<private name>.ExistentialContainer" %1, 2, !dbg !294
  %7 = alloca %"struct.main.<private name>.ProtocolConformance"*, !dbg !294
  store %"struct.main.<private name>.ProtocolConformance"* %6, %"struct.main.<private name>.ProtocolConformance"** %7, !dbg !294
  %8 = bitcast %"struct.main.<private name>.ProtocolConformance"** %7 to i8*, !dbg !294
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %8), !dbg !294
  %9 = bitcast %"struct.main.<private name>.TypeInfo"** %5 to i8*, !dbg !294
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %9), !dbg !294
  %10 = bitcast i8** %3 to i8*, !dbg !294
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %10), !dbg !294
  ret void, !dbg !294
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
!10 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.Atomic<i64>", file: !11, line: 39, flags: DIFlagFwdDecl, elements: !12)
!11 = !DIFile(filename: "main4.dot", directory: "/Users/Jonas/CDot/StdLib/main/")
!12 = !{!13}
!13 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !10, file: !11, line: 39, baseType: !14, size: 64, align: 64)
!14 = !DIBasicType(name: "i64", size: 64, encoding: DW_ATE_signed)
!15 = !DILocation(line: 19, column: 2, scope: !5)
!16 = !DILocation(line: 20, column: 10, scope: !5)
!17 = !DILocalVariable(name: "x", scope: !5, file: !1, line: 20, type: !14)
!18 = !DILocation(line: 23, column: 41, scope: !5)
!19 = !DILocation(line: 23, column: 6, scope: !5)
!20 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !6, line: 1, type: !7, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!21 = !DILocation(line: 1, column: 2, scope: !20)
!22 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlEC1ElL10weirdValue", linkageName: "_CNW4mainE6AtomicIlEC1ElL10weirdValue", scope: !0, file: !11, line: 48, type: !23, isLocal: false, isDefinition: true, scopeLine: 48, thisAdjustment: 1, isOptimized: false, unit: !0)
!23 = !DISubroutineType(types: !24)
!24 = !{!25, !14}
!25 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !10, size: 64, align: 8)
!26 = !DILocation(line: 48, column: 6, scope: !22)
!27 = !DILocalVariable(name: "self", scope: !22, file: !1, line: 48, type: !10)
!28 = !DILocalVariable(name: "weirdValue", scope: !22, file: !1, line: 48, type: !14)
!29 = !DILocation(line: 49, column: 27, scope: !22)
!30 = !DILocation(line: 49, column: 10, scope: !22)
!31 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlE5storeERS0_lNW_0E11MemoryOrderEL0L0L11memoryOrder", linkageName: "_CNW4mainE6AtomicIlE5storeERS0_lNW_0E11MemoryOrderEL0L0L11memoryOrder", scope: !0, file: !11, line: 102, type: !32, isLocal: false, isDefinition: true, scopeLine: 102, thisAdjustment: 1, isOptimized: false, unit: !0)
!32 = !DISubroutineType(types: !33)
!33 = !{!25, !14, !34}
!34 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "main.MemoryOrder", file: !11, line: 8, flags: DIFlagFwdDecl, elements: !35)
!35 = !{!36, !37, !38, !39, !40, !41, !42}
!36 = !DIEnumerator(name: "NotAtomic", value: 0)
!37 = !DIEnumerator(name: "Relaxed", value: 1)
!38 = !DIEnumerator(name: "Consume", value: 2)
!39 = !DIEnumerator(name: "Acquire", value: 3)
!40 = !DIEnumerator(name: "Release", value: 4)
!41 = !DIEnumerator(name: "AcquireRelease", value: 5)
!42 = !DIEnumerator(name: "SequentiallyConsistent", value: 6)
!43 = !DILocalVariable(name: "self", scope: !31, file: !1, line: 102, type: !10)
!44 = !DILocation(line: 102, column: 6, scope: !31)
!45 = !DILocalVariable(name: "storeVal", scope: !31, file: !1, line: 102, type: !14)
!46 = !DILocalVariable(name: "memoryOrder", scope: !31, file: !1, line: 102, type: !34)
!47 = !DILocalVariable(name: "addr", scope: !31, file: !1, line: 105, type: !48)
!48 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 64, align: 8)
!49 = !DILocation(line: 105, column: 10, scope: !31)
!50 = !DILocation(line: 106, column: 10, scope: !31)
!51 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlE4loadES0_NW_0E11MemoryOrderEL0L11memoryOrder", linkageName: "_CNW4mainE6AtomicIlE4loadES0_NW_0E11MemoryOrderEL0L11memoryOrder", scope: !0, file: !11, line: 94, type: !52, isLocal: false, isDefinition: true, scopeLine: 94, thisAdjustment: 1, isOptimized: false, unit: !0)
!52 = !DISubroutineType(types: !53)
!53 = !{!10, !34}
!54 = !DILocalVariable(name: "self", scope: !51, file: !1, line: 94, type: !10)
!55 = !DILocation(line: 94, column: 6, scope: !51)
!56 = !DILocalVariable(name: "memoryOrder", scope: !51, file: !1, line: 94, type: !34)
!57 = !DILocation(line: 95, column: 30, scope: !51)
!58 = !DILocation(line: 95, column: 10, scope: !51)
!59 = !DILocalVariable(name: "addr", scope: !51, file: !1, line: 96, type: !48)
!60 = !DILocation(line: 96, column: 10, scope: !51)
!61 = !DILocation(line: 97, column: 10, scope: !51)
!62 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlED0Ev", linkageName: "_CNW4mainE6AtomicIlED0Ev", scope: !0, file: !11, line: 39, type: !63, isLocal: false, isDefinition: true, scopeLine: 39, thisAdjustment: 1, isOptimized: false, unit: !0)
!63 = !DISubroutineType(types: !64)
!64 = !{!25}
!65 = !DILocation(line: 39, column: 9, scope: !62)
!66 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderD0Ev", linkageName: "_CNW4mainE11MemoryOrderD0Ev", scope: !0, file: !11, line: 8, type: !67, isLocal: false, isDefinition: true, scopeLine: 8, thisAdjustment: 1, isOptimized: false, unit: !0)
!67 = !DISubroutineType(types: !68)
!68 = !{!69}
!69 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !34, size: 64, align: 8)
!70 = !DILocation(line: 8, column: 9, scope: !66)
!71 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrder4copyES_L0", linkageName: "_CNW4mainE11MemoryOrder4copyES_L0", scope: !0, file: !11, line: 8, type: !72, isLocal: false, isDefinition: true, scopeLine: 8, thisAdjustment: 1, isOptimized: false, unit: !0)
!72 = !DISubroutineType(types: !73)
!73 = !{!34}
!74 = !DILocation(line: 8, column: 9, scope: !71)
!75 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderXi2==ES_S_L0L0", linkageName: "_CNW4mainE11MemoryOrderXi2==ES_S_L0L0", scope: !0, file: !11, line: 8, type: !76, isLocal: false, isDefinition: true, scopeLine: 8, thisAdjustment: 1, isOptimized: false, unit: !0)
!76 = !DISubroutineType(types: !77)
!77 = !{!34, !34}
!78 = !DILocation(line: 8, column: 9, scope: !75)
!79 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderXi2!=ES_S_L0L0", linkageName: "_CNW4mainE11MemoryOrderXi2!=ES_S_L0L0", scope: !0, file: !6, line: 1, type: !76, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!80 = !DILocalVariable(name: "self", scope: !79, file: !1, line: 1, type: !34)
!81 = !DILocation(line: 1, column: 2, scope: !79)
!82 = !DILocalVariable(name: "rhs", scope: !79, file: !1, line: 1, type: !34)
!83 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderC1EiL8rawValue", linkageName: "_CNW4mainE11MemoryOrderC1EiL8rawValue", scope: !0, file: !11, line: 8, type: !84, isLocal: false, isDefinition: true, scopeLine: 8, thisAdjustment: 1, isOptimized: false, unit: !0)
!84 = !DISubroutineType(types: !85)
!85 = !{!69, !86}
!86 = !DIBasicType(name: "i32", size: 32, encoding: DW_ATE_signed)
!87 = !DILocation(line: 8, column: 9, scope: !83)
!88 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderGa8rawValueES_L0", linkageName: "_CNW4mainE11MemoryOrderGa8rawValueES_L0", scope: !0, file: !11, line: 8, type: !72, isLocal: false, isDefinition: true, scopeLine: 8, thisAdjustment: 1, isOptimized: false, unit: !0)
!89 = !DILocation(line: 8, column: 9, scope: !88)
!90 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderD3Ev", linkageName: "_CNW4mainE11MemoryOrderD3Ev", scope: null, file: !11, line: 8, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!91 = !DILocation(line: 8, column: 9, scope: !90)
!92 = distinct !DISubprogram(name: "_CNW4mainE11MemoryOrderXi2~=ES_S_L0L0", linkageName: "_CNW4mainE11MemoryOrderXi2~=ES_S_L0L0", scope: !0, file: !11, line: 27, type: !76, isLocal: false, isDefinition: true, scopeLine: 27, thisAdjustment: 1, isOptimized: false, unit: !0)
!93 = !DILocalVariable(name: "self", scope: !92, file: !1, line: 27, type: !34)
!94 = !DILocation(line: 27, column: 6, scope: !92)
!95 = !DILocalVariable(name: "rhs", scope: !92, file: !1, line: 27, type: !34)
!96 = !DILocation(line: 28, column: 10, scope: !92)
!97 = distinct !DISubprogram(name: "_CNW4mainE4BoolXp1!ES0_L0", linkageName: "_CNW4mainE4BoolXp1!ES0_L0", scope: !0, file: !6, line: 1, type: !98, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!98 = !DISubroutineType(types: !99)
!99 = !{!100}
!100 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.Bool", file: !6, line: 1, flags: DIFlagFwdDecl, elements: !101)
!101 = !{!102}
!102 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !100, file: !6, line: 1, baseType: !103, size: 8, align: 8)
!103 = !DIBasicType(name: "i1", size: 8, encoding: DW_ATE_boolean)
!104 = !DILocalVariable(name: "self", scope: !97, file: !1, line: 1, type: !100)
!105 = !DILocation(line: 1, column: 2, scope: !97)
!106 = distinct !DISubprogram(name: "_CNW4mainE4BoolD0Ev", linkageName: "_CNW4mainE4BoolD0Ev", scope: !0, file: !6, line: 1, type: !107, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!107 = !DISubroutineType(types: !108)
!108 = !{!109}
!109 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !100, size: 64, align: 8)
!110 = !DILocation(line: 1, column: 2, scope: !106)
!111 = distinct !DISubprogram(name: "_CNW4mainE4funcElL2v1", linkageName: "_CNW4mainE4funcElL2v1", scope: null, file: !11, line: 32, type: !112, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!112 = !DISubroutineType(types: !113)
!113 = !{!14}
!114 = !DILocalVariable(name: "v1", scope: !111, file: !1, line: 32, type: !14)
!115 = !DILocation(line: 32, column: 2, scope: !111)
!116 = !DILocation(line: 32, column: 28, scope: !111)
!117 = distinct !DISubprogram(name: "_CNW4mainE4funcElL2v2", linkageName: "_CNW4mainE4funcElL2v2", scope: null, file: !11, line: 33, type: !112, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!118 = !DILocalVariable(name: "v2", scope: !117, file: !1, line: 33, type: !14)
!119 = !DILocation(line: 33, column: 2, scope: !117)
!120 = !DILocation(line: 33, column: 28, scope: !117)
!121 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlE4copyES0_L0", linkageName: "_CNW4mainE6AtomicIlE4copyES0_L0", scope: !0, file: !11, line: 39, type: !122, isLocal: false, isDefinition: true, scopeLine: 39, thisAdjustment: 1, isOptimized: false, unit: !0)
!122 = !DISubroutineType(types: !123)
!123 = !{!10}
!124 = !DILocation(line: 39, column: 9, scope: !121)
!125 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlE14__default_initES0_L0", linkageName: "_CNW4mainE6AtomicIlE14__default_initES0_L0", scope: !0, file: !11, line: 39, type: !122, isLocal: false, isDefinition: true, scopeLine: 39, thisAdjustment: 1, isOptimized: false, unit: !0)
!126 = !DILocation(line: 39, column: 9, scope: !125)
!127 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlED3Ev", linkageName: "_CNW4mainE6AtomicIlED3Ev", scope: null, file: !11, line: 39, type: !63, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!128 = !DILocation(line: 39, column: 9, scope: !127)
!129 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlEC2ERS0_lL0L5value", linkageName: "_CNW4mainE6AtomicIlEC2ERS0_lL0L5value", scope: !0, file: !11, line: 44, type: !23, isLocal: false, isDefinition: true, scopeLine: 44, thisAdjustment: 1, isOptimized: false, unit: !0)
!130 = !DILocalVariable(name: "self", scope: !129, file: !1, line: 44, type: !10)
!131 = !DILocation(line: 44, column: 6, scope: !129)
!132 = !DILocalVariable(name: "value", scope: !129, file: !1, line: 44, type: !14)
!133 = !DILocation(line: 45, column: 10, scope: !129)
!134 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlEC2ERS0_lL0L10weirdValue", linkageName: "_CNW4mainE6AtomicIlEC2ERS0_lL0L10weirdValue", scope: !0, file: !11, line: 48, type: !23, isLocal: false, isDefinition: true, scopeLine: 48, thisAdjustment: 1, isOptimized: false, unit: !0)
!135 = !DILocalVariable(name: "self", scope: !134, file: !1, line: 48, type: !10)
!136 = !DILocation(line: 48, column: 6, scope: !134)
!137 = !DILocalVariable(name: "weirdValue", scope: !134, file: !1, line: 48, type: !14)
!138 = !DILocation(line: 49, column: 27, scope: !134)
!139 = !DILocation(line: 49, column: 10, scope: !134)
!140 = distinct !DISubprogram(name: "_CNW4mainE6AtomicIlE19checkValidLoadOrderEMS0_NW_0E11MemoryOrderEL0L0", linkageName: "_CNW4mainE6AtomicIlE19checkValidLoadOrderEMS0_NW_0E11MemoryOrderEL0L0", scope: !0, file: !11, line: 53, type: !141, isLocal: false, isDefinition: true, scopeLine: 53, thisAdjustment: 1, isOptimized: false, unit: !0)
!141 = !DISubroutineType(types: !142)
!142 = !{!143, !34}
!143 = !DICompositeType(tag: DW_TAG_structure_type, name: "MetaType<main.Atomic<i64>>", file: !1, size: 64, align: 64, elements: !144)
!144 = !{!145, !145, !145, !145, !145}
!145 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !146, size: 64, align: 8)
!146 = !DIBasicType(name: "i8", size: 8, encoding: DW_ATE_signed_char)
!147 = !DILocalVariable(name: "self", scope: !140, file: !1, line: 53, type: !143)
!148 = !DILocation(line: 53, column: 21, scope: !140)
!149 = !DILocalVariable(name: "memoryOrder", scope: !140, file: !1, line: 53, type: !34)
!150 = !DILocation(line: 54, column: 16, scope: !140)
!151 = !DILocation(line: 62, column: 14, scope: !140)
!152 = !DILocation(line: 60, column: 79, scope: !140)
!153 = !DILocation(line: 60, column: 18, scope: !140)
!154 = !DILocation(line: 56, column: 14, scope: !140)
!155 = distinct !DISubprogram(name: "_CNW4mainE4Bool4copyES0_L0", linkageName: "_CNW4mainE4Bool4copyES0_L0", scope: !0, file: !6, line: 1, type: !98, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!156 = !DILocation(line: 1, column: 2, scope: !155)
!157 = distinct !DISubprogram(name: "_CNW4mainE4Bool14__default_initES0_L0", linkageName: "_CNW4mainE4Bool14__default_initES0_L0", scope: !0, file: !6, line: 1, type: !98, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!158 = !DILocation(line: 1, column: 2, scope: !157)
!159 = distinct !DISubprogram(name: "_CNW4mainE4BoolC1EbL5value", linkageName: "_CNW4mainE4BoolC1EbL5value", scope: !0, file: !6, line: 1, type: !160, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!160 = !DISubroutineType(types: !161)
!161 = !{!109, !103}
!162 = !DILocation(line: 1, column: 2, scope: !159)
!163 = distinct !DISubprogram(name: "_CNW4mainE4BoolC2ERS0_bL0L5value", linkageName: "_CNW4mainE4BoolC2ERS0_bL0L5value", scope: !0, file: !6, line: 1, type: !160, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!164 = !DILocation(line: 1, column: 2, scope: !163)
!165 = distinct !DISubprogram(name: "_CNW4mainE4BoolD3Ev", linkageName: "_CNW4mainE4BoolD3Ev", scope: null, file: !6, line: 1, type: !107, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!166 = !DILocation(line: 1, column: 2, scope: !165)
!167 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableD0Ev", linkageName: "_CNW4mainE17ValueWitnessTableD0Ev", scope: !0, file: !6, line: 1, type: !168, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!168 = !DISubroutineType(types: !169)
!169 = !{!170}
!170 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !171, size: 64, align: 8)
!171 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.ValueWitnessTable", file: !6, line: 1, flags: DIFlagFwdDecl, elements: !172)
!172 = !{!173, !178}
!173 = !DIDerivedType(tag: DW_TAG_member, name: "copyFn", scope: !171, file: !6, line: 1, baseType: !174, size: 64, align: 64)
!174 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !175, size: 64, align: 8)
!175 = !DISubroutineType(types: !176)
!176 = !{!177, !177}
!177 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64, align: 8)
!178 = !DIDerivedType(tag: DW_TAG_member, name: "deinit", scope: !171, file: !6, line: 1, baseType: !179, size: 64, align: 64, offset: 64)
!179 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !180, size: 64, align: 8)
!180 = !DISubroutineType(types: !181)
!181 = !{!177}
!182 = !DILocation(line: 1, column: 2, scope: !167)
!183 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTable14__default_initES0_L0", linkageName: "_CNW4mainE17ValueWitnessTable14__default_initES0_L0", scope: !0, file: !6, line: 1, type: !184, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!184 = !DISubroutineType(types: !185)
!185 = !{!171}
!186 = !DILocation(line: 1, column: 2, scope: !183)
!187 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit", linkageName: "_CNW4mainE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit", scope: !0, file: !6, line: 1, type: !188, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!188 = !DISubroutineType(types: !189)
!189 = !{!170, !174, !179}
!190 = !DILocation(line: 1, column: 2, scope: !187)
!191 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableC2ERS0_DoFvPvKPvEDoFvS2_EL0L6copyFnL6deinit", linkageName: "_CNW4mainE17ValueWitnessTableC2ERS0_DoFvPvKPvEDoFvS2_EL0L6copyFnL6deinit", scope: !0, file: !6, line: 1, type: !188, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!192 = !DILocation(line: 1, column: 2, scope: !191)
!193 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableD3Ev", linkageName: "_CNW4mainE17ValueWitnessTableD3Ev", scope: null, file: !6, line: 1, type: !168, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!194 = !DILocation(line: 1, column: 2, scope: !193)
!195 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceD0Ev", linkageName: "_CNW4mainE19ProtocolConformanceD0Ev", scope: !0, file: !6, line: 1, type: !196, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!196 = !DISubroutineType(types: !197)
!197 = !{!198}
!198 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !199, size: 64, align: 8)
!199 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.ProtocolConformance", file: !6, line: 1, flags: DIFlagFwdDecl, elements: !200)
!200 = !{!201, !220}
!201 = !DIDerivedType(tag: DW_TAG_member, name: "typeInfo", scope: !199, file: !6, line: 1, baseType: !202, size: 64, align: 64)
!202 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !203, size: 64, align: 8)
!203 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.TypeInfo", file: !6, line: 1, flags: DIFlagFwdDecl, elements: !204)
!204 = !{!205, !206, !208, !209, !212, !214, !216, !218, !219}
!205 = !DIDerivedType(tag: DW_TAG_member, name: "baseClass", scope: !203, file: !6, line: 1, baseType: !202, size: 64, align: 64)
!206 = !DIDerivedType(tag: DW_TAG_member, name: "vtable", scope: !203, file: !6, line: 1, baseType: !207, size: 64, align: 64, offset: 64)
!207 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !177, size: 64, align: 8)
!208 = !DIDerivedType(tag: DW_TAG_member, name: "deinit", scope: !203, file: !6, line: 1, baseType: !179, size: 64, align: 64, offset: 128)
!209 = !DIDerivedType(tag: DW_TAG_member, name: "name", scope: !203, file: !6, line: 1, baseType: !210, size: 64, align: 64, offset: 192)
!210 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !211, size: 64, align: 8)
!211 = !DIBasicType(name: "u8", size: 8, encoding: DW_ATE_unsigned_char)
!212 = !DIDerivedType(tag: DW_TAG_member, name: "valueWitnessTable", scope: !203, file: !6, line: 1, baseType: !213, size: 64, align: 64, offset: 256)
!213 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !171, size: 64, align: 8)
!214 = !DIDerivedType(tag: DW_TAG_member, name: "conformances", scope: !203, file: !6, line: 1, baseType: !215, size: 64, align: 64, offset: 320)
!215 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !199, size: 64, align: 8)
!216 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !203, file: !6, line: 1, baseType: !217, size: 64, align: 64, offset: 384)
!217 = !DIBasicType(name: "u64", size: 64, encoding: DW_ATE_unsigned)
!218 = !DIDerivedType(tag: DW_TAG_member, name: "alignment", scope: !203, file: !6, line: 1, baseType: !217, size: 64, align: 64, offset: 448)
!219 = !DIDerivedType(tag: DW_TAG_member, name: "stride", scope: !203, file: !6, line: 1, baseType: !217, size: 64, align: 64, offset: 512)
!220 = !DIDerivedType(tag: DW_TAG_member, name: "vtable", scope: !199, file: !6, line: 1, baseType: !207, size: 64, align: 64, offset: 64)
!221 = !DILocation(line: 1, column: 2, scope: !195)
!222 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformance14__default_initES0_L0", linkageName: "_CNW4mainE19ProtocolConformance14__default_initES0_L0", scope: !0, file: !6, line: 1, type: !223, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!223 = !DISubroutineType(types: !224)
!224 = !{!199}
!225 = !DILocation(line: 1, column: 2, scope: !222)
!226 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable", linkageName: "_CNW4mainE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable", scope: !0, file: !6, line: 1, type: !227, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!227 = !DISubroutineType(types: !228)
!228 = !{!198, !202, !207}
!229 = !DILocation(line: 1, column: 2, scope: !226)
!230 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceC2ERS0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable", linkageName: "_CNW4mainE19ProtocolConformanceC2ERS0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable", scope: !0, file: !6, line: 1, type: !227, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!231 = !DILocation(line: 1, column: 2, scope: !230)
!232 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceD3Ev", linkageName: "_CNW4mainE19ProtocolConformanceD3Ev", scope: null, file: !6, line: 1, type: !196, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!233 = !DILocation(line: 1, column: 2, scope: !232)
!234 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoD0Ev", linkageName: "_CNW4mainE8TypeInfoD0Ev", scope: !0, file: !6, line: 1, type: !235, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!235 = !DISubroutineType(types: !236)
!236 = !{!237}
!237 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !203, size: 64, align: 8)
!238 = !DILocation(line: 1, column: 2, scope: !234)
!239 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfo14__default_initES0_L0", linkageName: "_CNW4mainE8TypeInfo14__default_initES0_L0", scope: !0, file: !6, line: 1, type: !240, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!240 = !DISubroutineType(types: !241)
!241 = !{!203}
!242 = !DILocation(line: 1, column: 2, scope: !239)
!243 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", linkageName: "_CNW4mainE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", scope: !0, file: !6, line: 1, type: !244, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!244 = !DISubroutineType(types: !245)
!245 = !{!237, !202, !207, !179, !210, !213, !215, !217, !217, !217}
!246 = !DILocation(line: 1, column: 2, scope: !243)
!247 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoC2ERS0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", linkageName: "_CNW4mainE8TypeInfoC2ERS0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", scope: !0, file: !6, line: 1, type: !244, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!248 = !DILocation(line: 1, column: 2, scope: !247)
!249 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoD3Ev", linkageName: "_CNW4mainE8TypeInfoD3Ev", scope: null, file: !6, line: 1, type: !235, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!250 = !DILocation(line: 1, column: 2, scope: !249)
!251 = distinct !DISubprogram(name: "_CNW4mainE5Int64D0Ev", linkageName: "_CNW4mainE5Int64D0Ev", scope: !0, file: !6, line: 1, type: !252, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!252 = !DISubroutineType(types: !253)
!253 = !{!254}
!254 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !255, size: 64, align: 8)
!255 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.Int64", file: !6, line: 1, flags: DIFlagFwdDecl, elements: !256)
!256 = !{!257}
!257 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !255, file: !6, line: 1, baseType: !14, size: 64, align: 64)
!258 = !DILocation(line: 1, column: 2, scope: !251)
!259 = distinct !DISubprogram(name: "_CNW4mainE5Int644copyES0_L0", linkageName: "_CNW4mainE5Int644copyES0_L0", scope: !0, file: !6, line: 1, type: !260, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!260 = !DISubroutineType(types: !261)
!261 = !{!255}
!262 = !DILocation(line: 1, column: 2, scope: !259)
!263 = distinct !DISubprogram(name: "_CNW4mainE5Int6414__default_initES0_L0", linkageName: "_CNW4mainE5Int6414__default_initES0_L0", scope: !0, file: !6, line: 1, type: !260, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!264 = !DILocation(line: 1, column: 2, scope: !263)
!265 = distinct !DISubprogram(name: "_CNW4mainE5Int64C1ElL5value", linkageName: "_CNW4mainE5Int64C1ElL5value", scope: !0, file: !6, line: 1, type: !266, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!266 = !DISubroutineType(types: !267)
!267 = !{!254, !14}
!268 = !DILocation(line: 1, column: 2, scope: !265)
!269 = distinct !DISubprogram(name: "_CNW4mainE5Int64C2ERS0_lL0L5value", linkageName: "_CNW4mainE5Int64C2ERS0_lL0L5value", scope: !0, file: !6, line: 1, type: !266, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!270 = !DILocation(line: 1, column: 2, scope: !269)
!271 = distinct !DISubprogram(name: "_CNW4mainE5Int64D3Ev", linkageName: "_CNW4mainE5Int64D3Ev", scope: null, file: !6, line: 1, type: !252, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!272 = !DILocation(line: 1, column: 2, scope: !271)
!273 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerD0Ev", linkageName: "_CNW4mainE20ExistentialContainerD0Ev", scope: !0, file: !6, line: 1, type: !274, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!274 = !DISubroutineType(types: !275)
!275 = !{!276}
!276 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !277, size: 64, align: 8)
!277 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.ExistentialContainer", file: !6, line: 1, flags: DIFlagFwdDecl, elements: !278)
!278 = !{!279, !280, !281}
!279 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !277, file: !6, line: 1, baseType: !177, size: 64, align: 64)
!280 = !DIDerivedType(tag: DW_TAG_member, name: "typeInfo", scope: !277, file: !6, line: 1, baseType: !202, size: 64, align: 64, offset: 64)
!281 = !DIDerivedType(tag: DW_TAG_member, name: "conformance", scope: !277, file: !6, line: 1, baseType: !215, size: 64, align: 64, offset: 128)
!282 = !DILocation(line: 1, column: 2, scope: !273)
!283 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainer14__default_initES0_L0", linkageName: "_CNW4mainE20ExistentialContainer14__default_initES0_L0", scope: !0, file: !6, line: 1, type: !284, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!284 = !DISubroutineType(types: !285)
!285 = !{!277}
!286 = !DILocation(line: 1, column: 2, scope: !283)
!287 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance", linkageName: "_CNW4mainE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance", scope: !0, file: !6, line: 1, type: !288, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!288 = !DISubroutineType(types: !289)
!289 = !{!276, !177, !202, !215}
!290 = !DILocation(line: 1, column: 2, scope: !287)
!291 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerC2ERS0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance", linkageName: "_CNW4mainE20ExistentialContainerC2ERS0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance", scope: !0, file: !6, line: 1, type: !288, isLocal: false, isDefinition: true, scopeLine: 1, thisAdjustment: 1, isOptimized: false, unit: !0)
!292 = !DILocation(line: 1, column: 2, scope: !291)
!293 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerD3Ev", linkageName: "_CNW4mainE20ExistentialContainerD3Ev", scope: null, file: !6, line: 1, type: !274, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!294 = !DILocation(line: 1, column: 2, scope: !293)
