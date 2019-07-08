; ModuleID = '/Users/Jonas/CDot/cmake-build-debug/test/main.dot'
source_filename = "/Users/Jonas/CDot/cmake-build-debug/test/main.dot"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-darwin18.0.0"

%"struct.main.<private name>.TypeInfo" = type <{ %"struct.main.<private name>.TypeInfo"*, i8**, void (i8*)*, i8*, %"struct.main.<private name>.ValueWitnessTable"*, %"struct.main.<private name>.ProtocolConformance"*, i64, i64, i64 }>
%"struct.main.<private name>.ValueWitnessTable" = type <{ void (i8*, i8*)*, void (i8*)* }>
%"struct.main.<private name>.ProtocolConformance" = type <{ %"struct.main.<private name>.TypeInfo"*, i8** }>
%"struct.main.<private name>.Int64" = type <{ i64 }>
%struct.main.S = type <{ i8* }>
%struct.main.Circle = type <{ float }>
%struct.main.X = type <{ i8* }>
%"struct.main.<private name>.Bool" = type <{ i1 }>
%"struct.main.<private name>.ExistentialContainer" = type <{ i8*, %"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.ProtocolConformance"* }>

@_CTINW4mainE1XE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%struct.main.X*)* @_CNW4mainE1XD0Ev to void (i8*)*), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @0, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"0", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([5 x %"struct.main.<private name>.ProtocolConformance"], [5 x %"struct.main.<private name>.ProtocolConformance"]* @"1", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 8 }> }>, align 8
@"0" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (%struct.main.X (%struct.main.X)* @_CNW4mainE1X4copyES_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (%struct.main.X*)* @_CNW4mainE1XD0Ev to void (i8*)*) }>, align 8
@_CTINW4mainE3AnyE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([24 x i8], [24 x i8]* @1, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* null, %"struct.main.<private name>.ProtocolConformance"* null, %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW4mainE1XENW_0E3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTINW4mainE13InitializableE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @2, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* null, %"struct.main.<private name>.ProtocolConformance"* null, %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW4mainE1XENW_0E13InitializableE = constant [1 x i8*] [i8* bitcast (void (%struct.main.X*)* @_CNW4mainE1XC1Ev to i8*)], align 8
@_CTINW4mainE8CopyableE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([29 x i8], [29 x i8]* @3, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* null, %"struct.main.<private name>.ProtocolConformance"* null, %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW4mainE1XENW_0E8CopyableE = constant [1 x i8*] [i8* bitcast (%struct.main.X (%struct.main.X)* @_CNW4mainE1X4copyES_L0 to i8*)], align 8
@_CTINW4mainE18ImplicitlyCopyableE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([39 x i8], [39 x i8]* @4, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* null, %"struct.main.<private name>.ProtocolConformance"* null, %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW4mainE1XENW_0E18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"1" = constant [5 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE1XENW_0E3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE13InitializableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE1XENW_0E13InitializableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE1XENW_0E8CopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE1XENW_0E18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE6CircleE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%struct.main.Circle*)* @_CNW4mainE6CircleD0Ev to void (i8*)*), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @5, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"2", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([5 x %"struct.main.<private name>.ProtocolConformance"], [5 x %"struct.main.<private name>.ProtocolConformance"]* @"3", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 4 }>, %"struct.main.<private name>.Int64" <{ i64 4 }>, %"struct.main.<private name>.Int64" <{ i64 4 }> }>, align 8
@"2" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (%struct.main.Circle (%struct.main.Circle)* @_CNW4mainE6Circle4copyES_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (%struct.main.Circle*)* @_CNW4mainE6CircleD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE6CircleENW_0E3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTINW4mainE5ShapeE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @6, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* null, %"struct.main.<private name>.ProtocolConformance"* null, %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW4mainE6CircleENW_0E5ShapeE = constant [1 x i8*] [i8* bitcast (float (%struct.main.Circle)* @_CNW4mainE6CircleGa4areaES_L0 to i8*)], align 8
@_CTPNW4mainE6CircleENW_0E8CopyableE = constant [1 x i8*] [i8* bitcast (%struct.main.Circle (%struct.main.Circle)* @_CNW4mainE6Circle4copyES_L0 to i8*)], align 8
@_CTPNW4mainE6CircleENW_0E18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"3" = constant [5 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE6CircleENW_0E3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE5ShapeE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE6CircleENW_0E5ShapeE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE6CircleENW_0E8CopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE6CircleENW_0E18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE1SE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%struct.main.S*)* @_CNW4mainE1SD0Ev to void (i8*)*), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @7, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"4", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([6 x %"struct.main.<private name>.ProtocolConformance"], [6 x %"struct.main.<private name>.ProtocolConformance"]* @"5", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 8 }> }>, align 8
@"4" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (%struct.main.S (%struct.main.S)* @_CNW4mainE1S4copyES_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (%struct.main.S*)* @_CNW4mainE1SD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE1SENW_0E3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTINW4mainE1PE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @8, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* null, %"struct.main.<private name>.ProtocolConformance"* null, %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW4mainE1SENW_0E1PE = constant [5 x i8*] [i8* bitcast (void (%struct.main.S)* @_CNW4mainE1S4funcES_L0 to i8*), i8* bitcast (i8 (%struct.main.S)* @_CNW4mainE1S7produceES_L0 to i8*), i8* bitcast (void (%struct.main.S, i8)* @_CNW4mainE1S7consumeES_hL0L0 to i8*), i8* bitcast (%struct.main.Circle (%struct.main.S)* @_CNW4mainE1S9makeShapeES_L0 to i8*), i8* bitcast (i32 (%struct.main.S)* @_CNW4mainE1SGa6heeheeES_L0 to i8*)], align 8
@_CTPNW4mainE1SENW_0E13InitializableE = constant [1 x i8*] [i8* bitcast (void (%struct.main.S*)* @_CNW4mainE1SC1Ev to i8*)], align 8
@_CTPNW4mainE1SENW_0E8CopyableE = constant [1 x i8*] [i8* bitcast (%struct.main.S (%struct.main.S)* @_CNW4mainE1S4copyES_L0 to i8*)], align 8
@_CTPNW4mainE1SENW_0E18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"5" = constant [6 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE1SENW_0E3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE1PE, i8** getelementptr inbounds ([5 x i8*], [5 x i8*]* @_CTPNW4mainE1SENW_0E1PE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE13InitializableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE1SENW_0E13InitializableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE1SENW_0E8CopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE1SENW_0E18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE4BoolE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.Bool"*)* @_CNW4mainE4BoolD0Ev to void (i8*)*), i8* getelementptr inbounds ([25 x i8], [25 x i8]* @9, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"6", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([4 x %"struct.main.<private name>.ProtocolConformance"], [4 x %"struct.main.<private name>.ProtocolConformance"]* @"7", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 1 }>, %"struct.main.<private name>.Int64" <{ i64 1 }>, %"struct.main.<private name>.Int64" <{ i64 1 }> }>, align 8
@"6" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (%"struct.main.<private name>.Bool" (%"struct.main.<private name>.Bool")* @_CNW4mainE4Bool4copyES0_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (%"struct.main.<private name>.Bool"*)* @_CNW4mainE4BoolD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE4BoolENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTPNW4mainE4BoolENS_8CopyableE = constant [1 x i8*] [i8* bitcast (%"struct.main.<private name>.Bool" (%"struct.main.<private name>.Bool")* @_CNW4mainE4Bool4copyES0_L0 to i8*)], align 8
@_CTPNW4mainE4BoolENS_18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"7" = constant [4 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE4BoolENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE4BoolENS_8CopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE4BoolENS_18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE17ValueWitnessTableE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.ValueWitnessTable"*)* @_CNW4mainE17ValueWitnessTableD0Ev to void (i8*)*), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @10, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"8", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"9", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 16 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 16 }> }>, align 8
@"8" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.ValueWitnessTable"*)* @_CNW4mainE17ValueWitnessTableD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE17ValueWitnessTableENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"9" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE17ValueWitnessTableENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE19ProtocolConformanceE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.ProtocolConformance"*)* @_CNW4mainE19ProtocolConformanceD0Ev to void (i8*)*), i8* getelementptr inbounds ([40 x i8], [40 x i8]* @11, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"10", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"11", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 16 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 16 }> }>, align 8
@"10" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.ProtocolConformance"*)* @_CNW4mainE19ProtocolConformanceD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE19ProtocolConformanceENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"11" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE19ProtocolConformanceENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE8TypeInfoE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.TypeInfo"*)* @_CNW4mainE8TypeInfoD0Ev to void (i8*)*), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @12, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"12", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"13", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 72 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 72 }> }>, align 8
@"12" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.TypeInfo"*)* @_CNW4mainE8TypeInfoD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE8TypeInfoENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"13" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE8TypeInfoENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE5Int64E = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.Int64"*)* @_CNW4mainE5Int64D0Ev to void (i8*)*), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @13, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"14", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([4 x %"struct.main.<private name>.ProtocolConformance"], [4 x %"struct.main.<private name>.ProtocolConformance"]* @"15", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 8 }> }>, align 8
@"14" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (%"struct.main.<private name>.Int64" (%"struct.main.<private name>.Int64")* @_CNW4mainE5Int644copyES0_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (%"struct.main.<private name>.Int64"*)* @_CNW4mainE5Int64D0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE5Int64ENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTPNW4mainE5Int64ENS_8CopyableE = constant [1 x i8*] [i8* bitcast (%"struct.main.<private name>.Int64" (%"struct.main.<private name>.Int64")* @_CNW4mainE5Int644copyES0_L0 to i8*)], align 8
@_CTPNW4mainE5Int64ENS_18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"15" = constant [4 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE5Int64ENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW4mainE5Int64ENS_8CopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE5Int64ENS_18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW4mainE20ExistentialContainerE = constant %"struct.main.<private name>.TypeInfo" <{ %"struct.main.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.main.<private name>.ExistentialContainer"*)* @_CNW4mainE20ExistentialContainerD0Ev to void (i8*)*), i8* getelementptr inbounds ([41 x i8], [41 x i8]* @14, i32 0, i32 0), %"struct.main.<private name>.ValueWitnessTable"* @"16", %"struct.main.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.main.<private name>.ProtocolConformance"], [2 x %"struct.main.<private name>.ProtocolConformance"]* @"17", i32 0, i32 0), %"struct.main.<private name>.Int64" <{ i64 24 }>, %"struct.main.<private name>.Int64" <{ i64 8 }>, %"struct.main.<private name>.Int64" <{ i64 24 }> }>, align 8
@"16" = constant %"struct.main.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.main.<private name>.ExistentialContainer"*)* @_CNW4mainE20ExistentialContainerD0Ev to void (i8*)*) }>, align 8
@_CTPNW4mainE20ExistentialContainerENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"17" = constant [2 x %"struct.main.<private name>.ProtocolConformance"] [%"struct.main.<private name>.ProtocolConformance" <{ %"struct.main.<private name>.TypeInfo"* @_CTINW4mainE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW4mainE20ExistentialContainerENS_3AnyE, i32 0, i32 0) }>, %"struct.main.<private name>.ProtocolConformance" zeroinitializer], align 8
@0 = private unnamed_addr constant [7 x i8] c"main.X\00"
@1 = private unnamed_addr constant [24 x i8] c"main.<private name>.Any\00"
@2 = private unnamed_addr constant [19 x i8] c"main.Initializable\00"
@3 = private unnamed_addr constant [29 x i8] c"main.<private name>.Copyable\00"
@4 = private unnamed_addr constant [39 x i8] c"main.<private name>.ImplicitlyCopyable\00"
@5 = private unnamed_addr constant [12 x i8] c"main.Circle\00"
@6 = private unnamed_addr constant [11 x i8] c"main.Shape\00"
@7 = private unnamed_addr constant [7 x i8] c"main.S\00"
@8 = private unnamed_addr constant [7 x i8] c"main.P\00"
@9 = private unnamed_addr constant [25 x i8] c"main.<private name>.Bool\00"
@10 = private unnamed_addr constant [38 x i8] c"main.<private name>.ValueWitnessTable\00"
@11 = private unnamed_addr constant [40 x i8] c"main.<private name>.ProtocolConformance\00"
@12 = private unnamed_addr constant [29 x i8] c"main.<private name>.TypeInfo\00"
@13 = private unnamed_addr constant [26 x i8] c"main.<private name>.Int64\00"
@14 = private unnamed_addr constant [41 x i8] c"main.<private name>.ExistentialContainer\00"
@15 = private unnamed_addr constant [10 x i8] c"henlo %c\0A\00"

define void @_CNW4mainE6_startEv() !dbg !5 {
entry:
  %0 = alloca %struct.main.S, align 8
  %1 = alloca %struct.main.S, align 8
  call void @_CNW4mainE1SC1Ev(%struct.main.S* %1), !dbg !8
  %2 = load %struct.main.S, %struct.main.S* %1, !dbg !8
  call void @llvm.dbg.value(metadata %struct.main.S %2, metadata !9, metadata !DIExpression()), !dbg !8
  call void @llvm.dbg.declare(metadata %struct.main.S* %0, metadata !9, metadata !DIExpression()), !dbg !8
  store %struct.main.S %2, %struct.main.S* %0, align 8, !dbg !8
  %3 = bitcast %struct.main.S* %0 to i8*, !dbg !8
  call void @llvm.lifetime.start.p0i8(i64 8, i8* %3), !dbg !8
  %4 = load %struct.main.S, %struct.main.S* %0, align 8, !dbg !12
  call void @_CNW4mainE1S4funcES_L0(%struct.main.S %4), !dbg !12
  %5 = load %struct.main.S, %struct.main.S* %0, align 8, !dbg !13
  call void @_CNW4mainE1S5clunkES_L0(%struct.main.S %5), !dbg !13
  %6 = bitcast %struct.main.S* %0 to i8*, !dbg !13
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %6), !dbg !13
  call void @_CNW4mainE1SD0Ev(%struct.main.S* %0), !dbg !13
  ret void, !dbg !13
}

define i32 @main() !dbg !14 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE6_startEv(), !dbg !15
  ret i32 0, !dbg !15
}

define void @_CNW4mainE1SC1Ev(%struct.main.S*) !dbg !16 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %struct.main.S, %struct.main.S* %0, align 8, !dbg !20
  call void @_CNW4mainE1S14__default_initES_L0(%struct.main.S %1), !dbg !20
  ret void, !dbg !20
}

define void @_CNW4mainE1S4funcES_L0(%struct.main.S) !dbg !21 {
alloca_block:
  %1 = alloca %struct.main.S, align 8
  %2 = alloca %struct.main.S, align 8
  %3 = alloca %struct.main.S, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata %struct.main.S* %3, metadata !24, metadata !DIExpression()), !dbg !25
  call void @llvm.dbg.value(metadata %struct.main.S %0, metadata !24, metadata !DIExpression()), !dbg !25
  store %struct.main.S %0, %struct.main.S* %3, align 8, !dbg !25
  call void @_CNW4mainE1SC1Ev(%struct.main.S* %2), !dbg !26
  %4 = load %struct.main.S, %struct.main.S* %2, !dbg !26
  store %struct.main.S %4, %struct.main.S* %1, align 8, !dbg !26
  call void @_CNW4mainE1SD0Ev(%struct.main.S* %1), !dbg !26
  %5 = load %struct.main.S, %struct.main.S* %3, align 8, !dbg !27
  %6 = load %struct.main.S, %struct.main.S* %3, align 8, !dbg !28
  %7 = call i8 @_CNW4mainE1S7produceES_L0(%struct.main.S %6), !dbg !28
  call void @_CNW4mainE1S7consumeES_hL0L0(%struct.main.S %5, i8 %7), !dbg !27
  %8 = load %struct.main.S, %struct.main.S* %3, align 8, !dbg !29
  %9 = call i32 @_CNW4mainE1SGa6heeheeES_L0(%struct.main.S %8), !dbg !29
  ret void, !dbg !29
}

define void @_CNW4mainE1S5clunkES_L0(%struct.main.S) !dbg !30 {
entry:
  %1 = alloca %struct.main.Circle, align 4
  %2 = alloca %struct.main.X, align 8
  %3 = alloca %struct.main.X, align 8
  %4 = alloca %struct.main.S, align 8
  call void @llvm.dbg.declare(metadata %struct.main.S* %4, metadata !31, metadata !DIExpression()), !dbg !32
  call void @llvm.dbg.value(metadata %struct.main.S %0, metadata !31, metadata !DIExpression()), !dbg !32
  store %struct.main.S %0, %struct.main.S* %4, align 8, !dbg !32
  call void @_CNW4mainE1XC1Ev(%struct.main.X* %3), !dbg !33
  %5 = load %struct.main.X, %struct.main.X* %3, !dbg !33
  store %struct.main.X %5, %struct.main.X* %2, align 8, !dbg !33
  call void @_CNW4mainE1XD0Ev(%struct.main.X* %2), !dbg !33
  %6 = load %struct.main.S, %struct.main.S* %4, align 8, !dbg !34
  %7 = call %struct.main.Circle @_CNW4mainE1S9makeShapeES_L0(%struct.main.S %6), !dbg !34
  call void @llvm.dbg.value(metadata %struct.main.Circle %7, metadata !35, metadata !DIExpression()), !dbg !34
  call void @llvm.dbg.declare(metadata %struct.main.Circle* %1, metadata !35, metadata !DIExpression()), !dbg !34
  store %struct.main.Circle %7, %struct.main.Circle* %1, align 4, !dbg !34
  %8 = bitcast %struct.main.Circle* %1 to i8*, !dbg !34
  call void @llvm.lifetime.start.p0i8(i64 4, i8* %8), !dbg !34
  %9 = bitcast %struct.main.Circle* %1 to i8*, !dbg !34
  call void @llvm.lifetime.end.p0i8(i64 4, i8* %9), !dbg !34
  call void @_CNW4mainE6CircleD0Ev(%struct.main.Circle* %1), !dbg !34
  ret void, !dbg !34
}

define void @_CNW4mainE1SD0Ev(%struct.main.S*) !dbg !40 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE1SD3Ev(%struct.main.S* %0), !dbg !41
  ret void, !dbg !41
}

define void @_CNW4mainE1XD0Ev(%struct.main.X*) !dbg !42 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE1XD3Ev(%struct.main.X* %0), !dbg !47
  ret void, !dbg !47
}

define %struct.main.X @_CNW4mainE1X4copyES_L0(%struct.main.X) !dbg !48 {
alloca_block:
  %1 = alloca %struct.main.X, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %struct.main.X, %struct.main.X* %1, align 8, !dbg !51
  ret %struct.main.X %2, !dbg !51
}

define void @_CNW4mainE1XC1Ev(%struct.main.X*) !dbg !52 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %struct.main.X, %struct.main.X* %0, align 8, !dbg !53
  call void @_CNW4mainE1X14__default_initES_L0(%struct.main.X %1), !dbg !53
  ret void, !dbg !53
}

define void @_CNW4mainE1X14__default_initES_L0(%struct.main.X) !dbg !54 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !55
}

define void @_CNW4mainE1XC2ES_L0(%struct.main.X) !dbg !56 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !57
}

define void @_CNW4mainE1XD3Ev(%struct.main.X*) !dbg !58 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %struct.main.X, %struct.main.X* %0, align 8, !dbg !59
  ret void, !dbg !59
}

define void @_CNW4mainE6CircleD0Ev(%struct.main.Circle*) !dbg !60 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE6CircleD3Ev(%struct.main.Circle* %0), !dbg !64
  ret void, !dbg !64
}

define %struct.main.Circle @_CNW4mainE6Circle4copyES_L0(%struct.main.Circle) !dbg !65 {
alloca_block:
  %1 = alloca %struct.main.Circle, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %struct.main.Circle, %struct.main.Circle* %1, align 4, !dbg !68
  %3 = extractvalue %struct.main.Circle %2, 0, !dbg !68
  %4 = alloca float, !dbg !68
  store float %3, float* %4, !dbg !68
  %5 = extractvalue %struct.main.Circle %0, 0, !dbg !68
  %6 = alloca float, !dbg !68
  store float %5, float* %6, !dbg !68
  %7 = load float, float* %6, !dbg !68
  store float %7, float* %4, !dbg !68
  ret %struct.main.Circle %2, !dbg !68
}

define float @_CNW4mainE6CircleGa4areaES_L0(%struct.main.Circle) !dbg !69 {
alloca_block:
  %1 = alloca %struct.main.Circle, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata %struct.main.Circle* %1, metadata !70, metadata !DIExpression()), !dbg !71
  call void @llvm.dbg.value(metadata %struct.main.Circle %0, metadata !70, metadata !DIExpression()), !dbg !71
  store %struct.main.Circle %0, %struct.main.Circle* %1, align 4, !dbg !71
  %2 = load %struct.main.Circle, %struct.main.Circle* %1, align 4, !dbg !72
  %3 = extractvalue %struct.main.Circle %2, 0, !dbg !72
  %4 = alloca float, !dbg !72
  store float %3, float* %4, !dbg !72
  %5 = load float, float* %4, !dbg !72
  %6 = load %struct.main.Circle, %struct.main.Circle* %1, align 4, !dbg !72
  %7 = extractvalue %struct.main.Circle %6, 0, !dbg !72
  %8 = alloca float, !dbg !72
  store float %7, float* %8, !dbg !72
  %9 = load float, float* %8, !dbg !72
  %10 = fmul float %5, %9, !dbg !72
  %11 = fmul float 0xBB74BC6A80000000, %10, !dbg !72
  ret float %11, !dbg !72
}

define void @_CNW4mainE6Circle14__default_initES_L0(%struct.main.Circle) !dbg !73 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !74
}

define void @_CNW4mainE6CircleC1EfL1r(%struct.main.Circle*, float) !dbg !75 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %struct.main.Circle, %struct.main.Circle* %0, align 4, !dbg !78
  call void @_CNW4mainE6Circle14__default_initES_L0(%struct.main.Circle %2), !dbg !78
  %3 = extractvalue %struct.main.Circle %2, 0, !dbg !78
  %4 = alloca float, !dbg !78
  store float %3, float* %4, !dbg !78
  store float %1, float* %4, !dbg !78
  ret void, !dbg !78
}

define void @_CNW4mainE6CircleC2ES_fL0L1r(%struct.main.Circle, float) !dbg !79 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = extractvalue %struct.main.Circle %0, 0, !dbg !82
  %3 = alloca float, !dbg !82
  store float %2, float* %3, !dbg !82
  store float %1, float* %3, !dbg !82
  ret void, !dbg !82
}

define void @_CNW4mainE6CircleD3Ev(%struct.main.Circle*) !dbg !83 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %struct.main.Circle, %struct.main.Circle* %0, align 4, !dbg !84
  %2 = extractvalue %struct.main.Circle %1, 0, !dbg !84
  %3 = alloca float, !dbg !84
  store float %2, float* %3, !dbg !84
  %4 = bitcast float* %3 to i8*, !dbg !84
  call void @llvm.lifetime.end.p0i8(i64 4, i8* %4), !dbg !84
  ret void, !dbg !84
}

define %struct.main.S @_CNW4mainE1S4copyES_L0(%struct.main.S) !dbg !85 {
alloca_block:
  %1 = alloca %struct.main.S, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %struct.main.S, %struct.main.S* %1, align 8, !dbg !86
  ret %struct.main.S %2, !dbg !86
}

define i8 @_CNW4mainE1S7produceES_L0(%struct.main.S) !dbg !87 {
alloca_block:
  %1 = alloca %struct.main.S, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata %struct.main.S* %1, metadata !88, metadata !DIExpression()), !dbg !89
  call void @llvm.dbg.value(metadata %struct.main.S %0, metadata !88, metadata !DIExpression()), !dbg !89
  store %struct.main.S %0, %struct.main.S* %1, align 8, !dbg !89
  ret i8 49, !dbg !90
}

define void @_CNW4mainE1S7consumeES_hL0L0(%struct.main.S, i8) !dbg !91 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca %struct.main.S, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata %struct.main.S* %3, metadata !95, metadata !DIExpression()), !dbg !96
  call void @llvm.dbg.value(metadata %struct.main.S %0, metadata !95, metadata !DIExpression()), !dbg !96
  store %struct.main.S %0, %struct.main.S* %3, align 8, !dbg !96
  call void @llvm.dbg.declare(metadata i8* %2, metadata !97, metadata !DIExpression()), !dbg !96
  call void @llvm.dbg.value(metadata i8 %1, metadata !97, metadata !DIExpression()), !dbg !96
  store i8 %1, i8* %2, align 1, !dbg !96
  %4 = load i8, i8* %2, align 1, !dbg !98
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @15, i32 0, i32 0), i8 %4), !dbg !99
  ret void, !dbg !99
}

define %struct.main.Circle @_CNW4mainE1S9makeShapeES_L0(%struct.main.S) !dbg !100 {
alloca_block:
  %1 = alloca %struct.main.Circle, align 4
  %2 = alloca %struct.main.S, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata %struct.main.S* %2, metadata !101, metadata !DIExpression()), !dbg !102
  call void @llvm.dbg.value(metadata %struct.main.S %0, metadata !101, metadata !DIExpression()), !dbg !102
  store %struct.main.S %0, %struct.main.S* %2, align 8, !dbg !102
  call void @_CNW4mainE6CircleC1EfL1r(%struct.main.Circle* %1, float 0.000000e+00), !dbg !103
  %3 = load %struct.main.Circle, %struct.main.Circle* %1, !dbg !103
  ret %struct.main.Circle %3, !dbg !103
}

define i32 @_CNW4mainE1SGa6heeheeES_L0(%struct.main.S) !dbg !104 {
alloca_block:
  %1 = alloca %struct.main.S, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata %struct.main.S* %1, metadata !105, metadata !DIExpression()), !dbg !106
  call void @llvm.dbg.value(metadata %struct.main.S %0, metadata !105, metadata !DIExpression()), !dbg !106
  store %struct.main.S %0, %struct.main.S* %1, align 8, !dbg !106
  ret i32 3, !dbg !107
}

define void @_CNW4mainE1S14__default_initES_L0(%struct.main.S) !dbg !108 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !109
}

define void @_CNW4mainE1SC2ES_L0(%struct.main.S) !dbg !110 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !111
}

define void @_CNW4mainE1SD3Ev(%struct.main.S*) !dbg !112 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %struct.main.S, %struct.main.S* %0, align 8, !dbg !113
  ret void, !dbg !113
}

declare i32 @printf(i8*, ...)

define void @_CNW4mainE4BoolD0Ev(%"struct.main.<private name>.Bool"*) !dbg !114 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE4BoolD3Ev(%"struct.main.<private name>.Bool"* %0), !dbg !122
  ret void, !dbg !122
}

define %"struct.main.<private name>.Bool" @_CNW4mainE4Bool4copyES0_L0(%"struct.main.<private name>.Bool") !dbg !123 {
alloca_block:
  %1 = alloca %"struct.main.<private name>.Bool", align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %1, align 1, !dbg !126
  %3 = extractvalue %"struct.main.<private name>.Bool" %2, 0, !dbg !126
  %4 = alloca i1, !dbg !126
  store i1 %3, i1* %4, !dbg !126
  %5 = extractvalue %"struct.main.<private name>.Bool" %0, 0, !dbg !126
  %6 = alloca i1, !dbg !126
  store i1 %5, i1* %6, !dbg !126
  %7 = load i1, i1* %6, !dbg !126
  store i1 %7, i1* %4, !dbg !126
  ret %"struct.main.<private name>.Bool" %2, !dbg !126
}

define void @_CNW4mainE4Bool14__default_initES0_L0(%"struct.main.<private name>.Bool") !dbg !127 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !128
}

define void @_CNW4mainE4BoolC1EbL5value(%"struct.main.<private name>.Bool"*, i1) !dbg !129 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %0, align 1, !dbg !132
  call void @_CNW4mainE4Bool14__default_initES0_L0(%"struct.main.<private name>.Bool" %2), !dbg !132
  %3 = extractvalue %"struct.main.<private name>.Bool" %2, 0, !dbg !132
  %4 = alloca i1, !dbg !132
  store i1 %3, i1* %4, !dbg !132
  store i1 %1, i1* %4, !dbg !132
  ret void, !dbg !132
}

define void @_CNW4mainE4BoolC2ES0_bL0L5value(%"struct.main.<private name>.Bool", i1) !dbg !133 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = extractvalue %"struct.main.<private name>.Bool" %0, 0, !dbg !136
  %3 = alloca i1, !dbg !136
  store i1 %2, i1* %3, !dbg !136
  store i1 %1, i1* %3, !dbg !136
  ret void, !dbg !136
}

define void @_CNW4mainE4BoolD3Ev(%"struct.main.<private name>.Bool"*) !dbg !137 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %0, align 1, !dbg !138
  %2 = extractvalue %"struct.main.<private name>.Bool" %1, 0, !dbg !138
  %3 = alloca i1, !dbg !138
  store i1 %2, i1* %3, !dbg !138
  %4 = bitcast i1* %3 to i8*, !dbg !138
  call void @llvm.lifetime.end.p0i8(i64 1, i8* %4), !dbg !138
  ret void, !dbg !138
}

define %"struct.main.<private name>.Bool" @"_CNW4mainE4BoolXp1!ES0_L0"(%"struct.main.<private name>.Bool") !dbg !139 {
alloca_block:
  %1 = alloca %"struct.main.<private name>.Bool", align 1
  %2 = alloca %"struct.main.<private name>.Bool", align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata %"struct.main.<private name>.Bool"* %2, metadata !140, metadata !DIExpression()), !dbg !141
  call void @llvm.dbg.value(metadata %"struct.main.<private name>.Bool" %0, metadata !140, metadata !DIExpression()), !dbg !141
  store %"struct.main.<private name>.Bool" %0, %"struct.main.<private name>.Bool"* %2, align 1, !dbg !141
  %3 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %2, align 1, !dbg !141
  %4 = extractvalue %"struct.main.<private name>.Bool" %3, 0, !dbg !141
  %5 = alloca i1, !dbg !141
  store i1 %4, i1* %5, !dbg !141
  %6 = load i1, i1* %5, !dbg !141
  %7 = xor i1 %6, true, !dbg !141
  call void @_CNW4mainE4BoolC1EbL5value(%"struct.main.<private name>.Bool"* %1, i1 %7), !dbg !141
  %8 = load %"struct.main.<private name>.Bool", %"struct.main.<private name>.Bool"* %1, !dbg !141
  ret %"struct.main.<private name>.Bool" %8, !dbg !141
}

define void @_CNW4mainE17ValueWitnessTableD0Ev(%"struct.main.<private name>.ValueWitnessTable"*) !dbg !142 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE17ValueWitnessTableD3Ev(%"struct.main.<private name>.ValueWitnessTable"* %0), !dbg !157
  ret void, !dbg !157
}

define void @_CNW4mainE17ValueWitnessTable14__default_initES0_L0(%"struct.main.<private name>.ValueWitnessTable") !dbg !158 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !161
}

define void @_CNW4mainE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit(%"struct.main.<private name>.ValueWitnessTable"*, void (i8*, i8*)*, void (i8*)*) !dbg !162 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.main.<private name>.ValueWitnessTable", %"struct.main.<private name>.ValueWitnessTable"* %0, align 8, !dbg !165
  call void @_CNW4mainE17ValueWitnessTable14__default_initES0_L0(%"struct.main.<private name>.ValueWitnessTable" %3), !dbg !165
  %4 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %3, 0, !dbg !165
  %5 = alloca void (i8*, i8*)*, !dbg !165
  store void (i8*, i8*)* %4, void (i8*, i8*)** %5, !dbg !165
  store void (i8*, i8*)* %1, void (i8*, i8*)** %5, !dbg !165
  %6 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %3, 1, !dbg !165
  %7 = alloca void (i8*)*, !dbg !165
  store void (i8*)* %6, void (i8*)** %7, !dbg !165
  store void (i8*)* %2, void (i8*)** %7, !dbg !165
  ret void, !dbg !165
}

define void @_CNW4mainE17ValueWitnessTableC2ES0_DoFvPvKPvEDoFvS1_EL0L6copyFnL6deinit(%"struct.main.<private name>.ValueWitnessTable", void (i8*, i8*)*, void (i8*)*) !dbg !166 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %0, 0, !dbg !169
  %4 = alloca void (i8*, i8*)*, !dbg !169
  store void (i8*, i8*)* %3, void (i8*, i8*)** %4, !dbg !169
  store void (i8*, i8*)* %1, void (i8*, i8*)** %4, !dbg !169
  %5 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %0, 1, !dbg !169
  %6 = alloca void (i8*)*, !dbg !169
  store void (i8*)* %5, void (i8*)** %6, !dbg !169
  store void (i8*)* %2, void (i8*)** %6, !dbg !169
  ret void, !dbg !169
}

define void @_CNW4mainE17ValueWitnessTableD3Ev(%"struct.main.<private name>.ValueWitnessTable"*) !dbg !170 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.ValueWitnessTable", %"struct.main.<private name>.ValueWitnessTable"* %0, align 8, !dbg !171
  %2 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %1, 0, !dbg !171
  %3 = alloca void (i8*, i8*)*, !dbg !171
  store void (i8*, i8*)* %2, void (i8*, i8*)** %3, !dbg !171
  %4 = extractvalue %"struct.main.<private name>.ValueWitnessTable" %1, 1, !dbg !171
  %5 = alloca void (i8*)*, !dbg !171
  store void (i8*)* %4, void (i8*)** %5, !dbg !171
  %6 = bitcast void (i8*)** %5 to i8*, !dbg !171
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %6), !dbg !171
  %7 = bitcast void (i8*, i8*)** %3 to i8*, !dbg !171
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %7), !dbg !171
  ret void, !dbg !171
}

define void @_CNW4mainE19ProtocolConformanceD0Ev(%"struct.main.<private name>.ProtocolConformance"*) !dbg !172 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE19ProtocolConformanceD3Ev(%"struct.main.<private name>.ProtocolConformance"* %0), !dbg !197
  ret void, !dbg !197
}

define void @_CNW4mainE19ProtocolConformance14__default_initES0_L0(%"struct.main.<private name>.ProtocolConformance") !dbg !198 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !201
}

define void @_CNW4mainE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable(%"struct.main.<private name>.ProtocolConformance"*, %"struct.main.<private name>.TypeInfo"*, i8**) !dbg !202 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.main.<private name>.ProtocolConformance", %"struct.main.<private name>.ProtocolConformance"* %0, align 8, !dbg !205
  call void @_CNW4mainE19ProtocolConformance14__default_initES0_L0(%"struct.main.<private name>.ProtocolConformance" %3), !dbg !205
  %4 = extractvalue %"struct.main.<private name>.ProtocolConformance" %3, 0, !dbg !205
  %5 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !205
  store %"struct.main.<private name>.TypeInfo"* %4, %"struct.main.<private name>.TypeInfo"** %5, !dbg !205
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %5, !dbg !205
  %6 = extractvalue %"struct.main.<private name>.ProtocolConformance" %3, 1, !dbg !205
  %7 = alloca i8**, !dbg !205
  store i8** %6, i8*** %7, !dbg !205
  store i8** %2, i8*** %7, !dbg !205
  ret void, !dbg !205
}

define void @_CNW4mainE19ProtocolConformanceC2ES0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable(%"struct.main.<private name>.ProtocolConformance", %"struct.main.<private name>.TypeInfo"*, i8**) !dbg !206 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = extractvalue %"struct.main.<private name>.ProtocolConformance" %0, 0, !dbg !209
  %4 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !209
  store %"struct.main.<private name>.TypeInfo"* %3, %"struct.main.<private name>.TypeInfo"** %4, !dbg !209
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %4, !dbg !209
  %5 = extractvalue %"struct.main.<private name>.ProtocolConformance" %0, 1, !dbg !209
  %6 = alloca i8**, !dbg !209
  store i8** %5, i8*** %6, !dbg !209
  store i8** %2, i8*** %6, !dbg !209
  ret void, !dbg !209
}

define void @_CNW4mainE19ProtocolConformanceD3Ev(%"struct.main.<private name>.ProtocolConformance"*) !dbg !210 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.ProtocolConformance", %"struct.main.<private name>.ProtocolConformance"* %0, align 8, !dbg !211
  %2 = extractvalue %"struct.main.<private name>.ProtocolConformance" %1, 0, !dbg !211
  %3 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !211
  store %"struct.main.<private name>.TypeInfo"* %2, %"struct.main.<private name>.TypeInfo"** %3, !dbg !211
  %4 = extractvalue %"struct.main.<private name>.ProtocolConformance" %1, 1, !dbg !211
  %5 = alloca i8**, !dbg !211
  store i8** %4, i8*** %5, !dbg !211
  %6 = bitcast i8*** %5 to i8*, !dbg !211
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %6), !dbg !211
  %7 = bitcast %"struct.main.<private name>.TypeInfo"** %3 to i8*, !dbg !211
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %7), !dbg !211
  ret void, !dbg !211
}

define void @_CNW4mainE8TypeInfoD0Ev(%"struct.main.<private name>.TypeInfo"*) !dbg !212 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE8TypeInfoD3Ev(%"struct.main.<private name>.TypeInfo"* %0), !dbg !216
  ret void, !dbg !216
}

define void @_CNW4mainE8TypeInfo14__default_initES0_L0(%"struct.main.<private name>.TypeInfo"*) !dbg !217 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !220
}

define void @_CNW4mainE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride(%"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.TypeInfo"*, i8**, void (i8*)*, i8*, %"struct.main.<private name>.ValueWitnessTable"*, %"struct.main.<private name>.ProtocolConformance"*, i64, i64, i64) !dbg !221 {
entry:
  call void @_CNW4mainE8TypeInfo14__default_initES0_L0(%"struct.main.<private name>.TypeInfo"* %0), !dbg !224
  %10 = bitcast %"struct.main.<private name>.TypeInfo"* %0 to %"struct.main.<private name>.TypeInfo"**
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %10, align 8, !dbg !224
  %11 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !224
  store i8** %2, i8*** %11, align 8, !dbg !224
  %12 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !224
  store void (i8*)* %3, void (i8*)** %12, align 8, !dbg !224
  %13 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !224
  store i8* %4, i8** %13, align 8, !dbg !224
  %14 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !224
  store %"struct.main.<private name>.ValueWitnessTable"* %5, %"struct.main.<private name>.ValueWitnessTable"** %14, align 8, !dbg !224
  %15 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !224
  store %"struct.main.<private name>.ProtocolConformance"* %6, %"struct.main.<private name>.ProtocolConformance"** %15, align 8, !dbg !224
  %16 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !224
  store i64 %7, i64* %16, align 8, !dbg !224
  %17 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !224
  store i64 %8, i64* %17, align 8, !dbg !224
  %18 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !224
  store i64 %9, i64* %18, align 8, !dbg !224
  ret void, !dbg !224
}

define void @_CNW4mainE8TypeInfoC2ES0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride(%"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.TypeInfo"*, i8**, void (i8*)*, i8*, %"struct.main.<private name>.ValueWitnessTable"*, %"struct.main.<private name>.ProtocolConformance"*, i64, i64, i64) !dbg !225 {
entry:
  %10 = bitcast %"struct.main.<private name>.TypeInfo"* %0 to %"struct.main.<private name>.TypeInfo"**
  store %"struct.main.<private name>.TypeInfo"* %1, %"struct.main.<private name>.TypeInfo"** %10, align 8, !dbg !228
  %11 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !228
  store i8** %2, i8*** %11, align 8, !dbg !228
  %12 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !228
  store void (i8*)* %3, void (i8*)** %12, align 8, !dbg !228
  %13 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !228
  store i8* %4, i8** %13, align 8, !dbg !228
  %14 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !228
  store %"struct.main.<private name>.ValueWitnessTable"* %5, %"struct.main.<private name>.ValueWitnessTable"** %14, align 8, !dbg !228
  %15 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !228
  store %"struct.main.<private name>.ProtocolConformance"* %6, %"struct.main.<private name>.ProtocolConformance"** %15, align 8, !dbg !228
  %16 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !228
  store i64 %7, i64* %16, align 8, !dbg !228
  %17 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !228
  store i64 %8, i64* %17, align 8, !dbg !228
  %18 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !228
  store i64 %9, i64* %18, align 8, !dbg !228
  ret void, !dbg !228
}

define void @_CNW4mainE8TypeInfoD3Ev(%"struct.main.<private name>.TypeInfo"*) !dbg !229 {
entry:
  %1 = bitcast %"struct.main.<private name>.TypeInfo"* %0 to %"struct.main.<private name>.TypeInfo"**
  %2 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !230
  %3 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !230
  %4 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !230
  %5 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !230
  %6 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !230
  %7 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !230
  %8 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !230
  %9 = getelementptr inbounds %"struct.main.<private name>.TypeInfo", %"struct.main.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !230
  %10 = bitcast i64* %9 to i8*, !dbg !230
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %10), !dbg !230
  %11 = bitcast i64* %8 to i8*, !dbg !230
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %11), !dbg !230
  %12 = bitcast i64* %7 to i8*, !dbg !230
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %12), !dbg !230
  %13 = bitcast %"struct.main.<private name>.ProtocolConformance"** %6 to i8*, !dbg !230
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %13), !dbg !230
  %14 = bitcast %"struct.main.<private name>.ValueWitnessTable"** %5 to i8*, !dbg !230
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %14), !dbg !230
  %15 = bitcast i8** %4 to i8*, !dbg !230
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %15), !dbg !230
  %16 = bitcast void (i8*)** %3 to i8*, !dbg !230
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %16), !dbg !230
  %17 = bitcast i8*** %2 to i8*, !dbg !230
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %17), !dbg !230
  %18 = bitcast %"struct.main.<private name>.TypeInfo"** %1 to i8*, !dbg !230
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %18), !dbg !230
  ret void, !dbg !230
}

define void @_CNW4mainE5Int64D0Ev(%"struct.main.<private name>.Int64"*) !dbg !231 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE5Int64D3Ev(%"struct.main.<private name>.Int64"* %0), !dbg !239
  ret void, !dbg !239
}

define %"struct.main.<private name>.Int64" @_CNW4mainE5Int644copyES0_L0(%"struct.main.<private name>.Int64") !dbg !240 {
alloca_block:
  %1 = alloca %"struct.main.<private name>.Int64", align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Int64", %"struct.main.<private name>.Int64"* %1, align 8, !dbg !243
  %3 = extractvalue %"struct.main.<private name>.Int64" %2, 0, !dbg !243
  %4 = alloca i64, !dbg !243
  store i64 %3, i64* %4, !dbg !243
  %5 = extractvalue %"struct.main.<private name>.Int64" %0, 0, !dbg !243
  %6 = alloca i64, !dbg !243
  store i64 %5, i64* %6, !dbg !243
  %7 = load i64, i64* %6, !dbg !243
  store i64 %7, i64* %4, !dbg !243
  ret %"struct.main.<private name>.Int64" %2, !dbg !243
}

define void @_CNW4mainE5Int6414__default_initES0_L0(%"struct.main.<private name>.Int64") !dbg !244 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !245
}

define void @_CNW4mainE5Int64C1ElL5value(%"struct.main.<private name>.Int64"*, i64) !dbg !246 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.main.<private name>.Int64", %"struct.main.<private name>.Int64"* %0, align 8, !dbg !249
  call void @_CNW4mainE5Int6414__default_initES0_L0(%"struct.main.<private name>.Int64" %2), !dbg !249
  %3 = extractvalue %"struct.main.<private name>.Int64" %2, 0, !dbg !249
  %4 = alloca i64, !dbg !249
  store i64 %3, i64* %4, !dbg !249
  store i64 %1, i64* %4, !dbg !249
  ret void, !dbg !249
}

define void @_CNW4mainE5Int64C2ES0_lL0L5value(%"struct.main.<private name>.Int64", i64) !dbg !250 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = extractvalue %"struct.main.<private name>.Int64" %0, 0, !dbg !253
  %3 = alloca i64, !dbg !253
  store i64 %2, i64* %3, !dbg !253
  store i64 %1, i64* %3, !dbg !253
  ret void, !dbg !253
}

define void @_CNW4mainE5Int64D3Ev(%"struct.main.<private name>.Int64"*) !dbg !254 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.Int64", %"struct.main.<private name>.Int64"* %0, align 8, !dbg !255
  %2 = extractvalue %"struct.main.<private name>.Int64" %1, 0, !dbg !255
  %3 = alloca i64, !dbg !255
  store i64 %2, i64* %3, !dbg !255
  %4 = bitcast i64* %3 to i8*, !dbg !255
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %4), !dbg !255
  ret void, !dbg !255
}

define void @_CNW4mainE20ExistentialContainerD0Ev(%"struct.main.<private name>.ExistentialContainer"*) !dbg !256 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW4mainE20ExistentialContainerD3Ev(%"struct.main.<private name>.ExistentialContainer"* %0), !dbg !265
  ret void, !dbg !265
}

define void @_CNW4mainE20ExistentialContainer14__default_initES0_L0(%"struct.main.<private name>.ExistentialContainer") !dbg !266 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !269
}

define void @_CNW4mainE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance(%"struct.main.<private name>.ExistentialContainer"*, i8*, %"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.ProtocolConformance"*) !dbg !270 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %4 = load %"struct.main.<private name>.ExistentialContainer", %"struct.main.<private name>.ExistentialContainer"* %0, align 8, !dbg !273
  call void @_CNW4mainE20ExistentialContainer14__default_initES0_L0(%"struct.main.<private name>.ExistentialContainer" %4), !dbg !273
  %5 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 0, !dbg !273
  %6 = alloca i8*, !dbg !273
  store i8* %5, i8** %6, !dbg !273
  store i8* %1, i8** %6, !dbg !273
  %7 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 1, !dbg !273
  %8 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !273
  store %"struct.main.<private name>.TypeInfo"* %7, %"struct.main.<private name>.TypeInfo"** %8, !dbg !273
  store %"struct.main.<private name>.TypeInfo"* %2, %"struct.main.<private name>.TypeInfo"** %8, !dbg !273
  %9 = extractvalue %"struct.main.<private name>.ExistentialContainer" %4, 2, !dbg !273
  %10 = alloca %"struct.main.<private name>.ProtocolConformance"*, !dbg !273
  store %"struct.main.<private name>.ProtocolConformance"* %9, %"struct.main.<private name>.ProtocolConformance"** %10, !dbg !273
  store %"struct.main.<private name>.ProtocolConformance"* %3, %"struct.main.<private name>.ProtocolConformance"** %10, !dbg !273
  ret void, !dbg !273
}

define void @_CNW4mainE20ExistentialContainerC2ES0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance(%"struct.main.<private name>.ExistentialContainer", i8*, %"struct.main.<private name>.TypeInfo"*, %"struct.main.<private name>.ProtocolConformance"*) !dbg !274 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %4 = extractvalue %"struct.main.<private name>.ExistentialContainer" %0, 0, !dbg !277
  %5 = alloca i8*, !dbg !277
  store i8* %4, i8** %5, !dbg !277
  store i8* %1, i8** %5, !dbg !277
  %6 = extractvalue %"struct.main.<private name>.ExistentialContainer" %0, 1, !dbg !277
  %7 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !277
  store %"struct.main.<private name>.TypeInfo"* %6, %"struct.main.<private name>.TypeInfo"** %7, !dbg !277
  store %"struct.main.<private name>.TypeInfo"* %2, %"struct.main.<private name>.TypeInfo"** %7, !dbg !277
  %8 = extractvalue %"struct.main.<private name>.ExistentialContainer" %0, 2, !dbg !277
  %9 = alloca %"struct.main.<private name>.ProtocolConformance"*, !dbg !277
  store %"struct.main.<private name>.ProtocolConformance"* %8, %"struct.main.<private name>.ProtocolConformance"** %9, !dbg !277
  store %"struct.main.<private name>.ProtocolConformance"* %3, %"struct.main.<private name>.ProtocolConformance"** %9, !dbg !277
  ret void, !dbg !277
}

define void @_CNW4mainE20ExistentialContainerD3Ev(%"struct.main.<private name>.ExistentialContainer"*) !dbg !278 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.main.<private name>.ExistentialContainer", %"struct.main.<private name>.ExistentialContainer"* %0, align 8, !dbg !279
  %2 = extractvalue %"struct.main.<private name>.ExistentialContainer" %1, 0, !dbg !279
  %3 = alloca i8*, !dbg !279
  store i8* %2, i8** %3, !dbg !279
  %4 = extractvalue %"struct.main.<private name>.ExistentialContainer" %1, 1, !dbg !279
  %5 = alloca %"struct.main.<private name>.TypeInfo"*, !dbg !279
  store %"struct.main.<private name>.TypeInfo"* %4, %"struct.main.<private name>.TypeInfo"** %5, !dbg !279
  %6 = extractvalue %"struct.main.<private name>.ExistentialContainer" %1, 2, !dbg !279
  %7 = alloca %"struct.main.<private name>.ProtocolConformance"*, !dbg !279
  store %"struct.main.<private name>.ProtocolConformance"* %6, %"struct.main.<private name>.ProtocolConformance"** %7, !dbg !279
  %8 = bitcast %"struct.main.<private name>.ProtocolConformance"** %7 to i8*, !dbg !279
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %8), !dbg !279
  %9 = bitcast %"struct.main.<private name>.TypeInfo"** %5 to i8*, !dbg !279
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %9), !dbg !279
  %10 = bitcast i8** %3 to i8*, !dbg !279
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %10), !dbg !279
  ret void, !dbg !279
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #0

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.value(metadata, metadata, metadata) #0

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #1

; Function Attrs: nounwind
declare void @llvm.stackprotector(i8*, i8**) #2

attributes #0 = { nounwind readnone speculatable }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "cdotc v0.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "/Users/Jonas/CDot/cmake-build-debug/test/main.dot", directory: "")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = distinct !DISubprogram(name: "_CNW4mainE6_startEv", linkageName: "_CNW4mainE6_startEv", scope: null, file: !6, line: 2, type: !7, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!6 = !DIFile(filename: "main.dot", directory: "/Users/Jonas/CDot/cmake-build-debug/test/")
!7 = !DISubroutineType(types: !2)
!8 = !DILocation(line: 2, column: 10, scope: !5)
!9 = !DILocalVariable(name: "s", scope: !5, file: !1, line: 2, type: !10)
!10 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.S", file: !11, line: 54, flags: DIFlagFwdDecl, elements: !2)
!11 = !DIFile(filename: "main2.dot", directory: "/Users/Jonas/CDot/cmake-build-debug/test/")
!12 = !DILocation(line: 3, column: 2, scope: !5)
!13 = !DILocation(line: 4, column: 2, scope: !5)
!14 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !6, line: 2, type: !7, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!15 = !DILocation(line: 2, column: 1, scope: !14)
!16 = distinct !DISubprogram(name: "_CNW4mainE1SC1Ev", linkageName: "_CNW4mainE1SC1Ev", scope: !0, file: !11, line: 54, type: !17, isLocal: false, isDefinition: true, scopeLine: 54, thisAdjustment: 1, isOptimized: false, unit: !0)
!17 = !DISubroutineType(types: !18)
!18 = !{!19}
!19 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !10, size: 64, align: 8)
!20 = !DILocation(line: 54, column: 2, scope: !16)
!21 = distinct !DISubprogram(name: "_CNW4mainE1S4funcES_L0", linkageName: "_CNW4mainE1S4funcES_L0", scope: !0, file: !11, line: 28, type: !22, isLocal: false, isDefinition: true, scopeLine: 28, thisAdjustment: 1, isOptimized: false, unit: !0)
!22 = !DISubroutineType(types: !23)
!23 = !{!10}
!24 = !DILocalVariable(name: "self", scope: !21, file: !1, line: 28, type: !10)
!25 = !DILocation(line: 28, column: 14, scope: !21)
!26 = !DILocation(line: 29, column: 14, scope: !21)
!27 = !DILocation(line: 30, column: 10, scope: !21)
!28 = !DILocation(line: 30, column: 23, scope: !21)
!29 = !DILocation(line: 31, column: 10, scope: !21)
!30 = distinct !DISubprogram(name: "_CNW4mainE1S5clunkES_L0", linkageName: "_CNW4mainE1S5clunkES_L0", scope: !0, file: !11, line: 34, type: !22, isLocal: false, isDefinition: true, scopeLine: 34, thisAdjustment: 1, isOptimized: false, unit: !0)
!31 = !DILocalVariable(name: "self", scope: !30, file: !1, line: 34, type: !10)
!32 = !DILocation(line: 34, column: 6, scope: !30)
!33 = !DILocation(line: 35, column: 14, scope: !30)
!34 = !DILocation(line: 37, column: 18, scope: !30)
!35 = !DILocalVariable(name: "s", scope: !30, file: !1, line: 37, type: !36)
!36 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.Circle", file: !11, line: 47, flags: DIFlagFwdDecl, elements: !37)
!37 = !{!38}
!38 = !DIDerivedType(tag: DW_TAG_member, name: "r", scope: !36, file: !11, line: 47, baseType: !39, size: 32, align: 32)
!39 = !DIBasicType(name: "f32", size: 32, encoding: DW_ATE_float)
!40 = distinct !DISubprogram(name: "_CNW4mainE1SD0Ev", linkageName: "_CNW4mainE1SD0Ev", scope: !0, file: !11, line: 54, type: !17, isLocal: false, isDefinition: true, scopeLine: 54, thisAdjustment: 1, isOptimized: false, unit: !0)
!41 = !DILocation(line: 54, column: 2, scope: !40)
!42 = distinct !DISubprogram(name: "_CNW4mainE1XD0Ev", linkageName: "_CNW4mainE1XD0Ev", scope: !0, file: !11, line: 43, type: !43, isLocal: false, isDefinition: true, scopeLine: 43, thisAdjustment: 1, isOptimized: false, unit: !0)
!43 = !DISubroutineType(types: !44)
!44 = !{!45}
!45 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !46, size: 64, align: 8)
!46 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.X", file: !11, line: 43, flags: DIFlagFwdDecl, elements: !2)
!47 = !DILocation(line: 43, column: 2, scope: !42)
!48 = distinct !DISubprogram(name: "_CNW4mainE1X4copyES_L0", linkageName: "_CNW4mainE1X4copyES_L0", scope: !0, file: !11, line: 43, type: !49, isLocal: false, isDefinition: true, scopeLine: 43, thisAdjustment: 1, isOptimized: false, unit: !0)
!49 = !DISubroutineType(types: !50)
!50 = !{!46}
!51 = !DILocation(line: 43, column: 2, scope: !48)
!52 = distinct !DISubprogram(name: "_CNW4mainE1XC1Ev", linkageName: "_CNW4mainE1XC1Ev", scope: !0, file: !11, line: 43, type: !43, isLocal: false, isDefinition: true, scopeLine: 43, thisAdjustment: 1, isOptimized: false, unit: !0)
!53 = !DILocation(line: 43, column: 2, scope: !52)
!54 = distinct !DISubprogram(name: "_CNW4mainE1X14__default_initES_L0", linkageName: "_CNW4mainE1X14__default_initES_L0", scope: !0, file: !11, line: 43, type: !49, isLocal: false, isDefinition: true, scopeLine: 43, thisAdjustment: 1, isOptimized: false, unit: !0)
!55 = !DILocation(line: 43, column: 2, scope: !54)
!56 = distinct !DISubprogram(name: "_CNW4mainE1XC2ES_L0", linkageName: "_CNW4mainE1XC2ES_L0", scope: !0, file: !11, line: 43, type: !49, isLocal: false, isDefinition: true, scopeLine: 43, thisAdjustment: 1, isOptimized: false, unit: !0)
!57 = !DILocation(line: 43, column: 2, scope: !56)
!58 = distinct !DISubprogram(name: "_CNW4mainE1XD3Ev", linkageName: "_CNW4mainE1XD3Ev", scope: null, file: !11, line: 43, type: !43, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!59 = !DILocation(line: 43, column: 2, scope: !58)
!60 = distinct !DISubprogram(name: "_CNW4mainE6CircleD0Ev", linkageName: "_CNW4mainE6CircleD0Ev", scope: !0, file: !11, line: 47, type: !61, isLocal: false, isDefinition: true, scopeLine: 47, thisAdjustment: 1, isOptimized: false, unit: !0)
!61 = !DISubroutineType(types: !62)
!62 = !{!63}
!63 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !36, size: 64, align: 8)
!64 = !DILocation(line: 47, column: 2, scope: !60)
!65 = distinct !DISubprogram(name: "_CNW4mainE6Circle4copyES_L0", linkageName: "_CNW4mainE6Circle4copyES_L0", scope: !0, file: !11, line: 47, type: !66, isLocal: false, isDefinition: true, scopeLine: 47, thisAdjustment: 1, isOptimized: false, unit: !0)
!66 = !DISubroutineType(types: !67)
!67 = !{!36}
!68 = !DILocation(line: 47, column: 2, scope: !65)
!69 = distinct !DISubprogram(name: "_CNW4mainE6CircleGa4areaES_L0", linkageName: "_CNW4mainE6CircleGa4areaES_L0", scope: !0, file: !11, line: 49, type: !66, isLocal: false, isDefinition: true, scopeLine: 49, thisAdjustment: 1, isOptimized: false, unit: !0)
!70 = !DILocalVariable(name: "self", scope: !69, file: !1, line: 49, type: !36)
!71 = !DILocation(line: 49, column: 6, scope: !69)
!72 = !DILocation(line: 50, column: 10, scope: !69)
!73 = distinct !DISubprogram(name: "_CNW4mainE6Circle14__default_initES_L0", linkageName: "_CNW4mainE6Circle14__default_initES_L0", scope: !0, file: !11, line: 47, type: !66, isLocal: false, isDefinition: true, scopeLine: 47, thisAdjustment: 1, isOptimized: false, unit: !0)
!74 = !DILocation(line: 47, column: 2, scope: !73)
!75 = distinct !DISubprogram(name: "_CNW4mainE6CircleC1EfL1r", linkageName: "_CNW4mainE6CircleC1EfL1r", scope: !0, file: !11, line: 47, type: !76, isLocal: false, isDefinition: true, scopeLine: 47, thisAdjustment: 1, isOptimized: false, unit: !0)
!76 = !DISubroutineType(types: !77)
!77 = !{!63, !39}
!78 = !DILocation(line: 47, column: 2, scope: !75)
!79 = distinct !DISubprogram(name: "_CNW4mainE6CircleC2ES_fL0L1r", linkageName: "_CNW4mainE6CircleC2ES_fL0L1r", scope: !0, file: !11, line: 47, type: !80, isLocal: false, isDefinition: true, scopeLine: 47, thisAdjustment: 1, isOptimized: false, unit: !0)
!80 = !DISubroutineType(types: !81)
!81 = !{!36, !39}
!82 = !DILocation(line: 47, column: 2, scope: !79)
!83 = distinct !DISubprogram(name: "_CNW4mainE6CircleD3Ev", linkageName: "_CNW4mainE6CircleD3Ev", scope: null, file: !11, line: 47, type: !61, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!84 = !DILocation(line: 47, column: 2, scope: !83)
!85 = distinct !DISubprogram(name: "_CNW4mainE1S4copyES_L0", linkageName: "_CNW4mainE1S4copyES_L0", scope: !0, file: !11, line: 54, type: !22, isLocal: false, isDefinition: true, scopeLine: 54, thisAdjustment: 1, isOptimized: false, unit: !0)
!86 = !DILocation(line: 54, column: 2, scope: !85)
!87 = distinct !DISubprogram(name: "_CNW4mainE1S7produceES_L0", linkageName: "_CNW4mainE1S7produceES_L0", scope: !0, file: !11, line: 58, type: !22, isLocal: false, isDefinition: true, scopeLine: 58, thisAdjustment: 1, isOptimized: false, unit: !0)
!88 = !DILocalVariable(name: "self", scope: !87, file: !1, line: 58, type: !10)
!89 = !DILocation(line: 58, column: 6, scope: !87)
!90 = !DILocation(line: 59, column: 10, scope: !87)
!91 = distinct !DISubprogram(name: "_CNW4mainE1S7consumeES_hL0L0", linkageName: "_CNW4mainE1S7consumeES_hL0L0", scope: !0, file: !11, line: 62, type: !92, isLocal: false, isDefinition: true, scopeLine: 62, thisAdjustment: 1, isOptimized: false, unit: !0)
!92 = !DISubroutineType(types: !93)
!93 = !{!10, !94}
!94 = !DIBasicType(name: "u8", size: 8, encoding: DW_ATE_unsigned_char)
!95 = !DILocalVariable(name: "self", scope: !91, file: !1, line: 62, type: !10)
!96 = !DILocation(line: 62, column: 6, scope: !91)
!97 = !DILocalVariable(name: "x", scope: !91, file: !1, line: 62, type: !94)
!98 = !DILocation(line: 63, column: 35, scope: !91)
!99 = !DILocation(line: 63, column: 14, scope: !91)
!100 = distinct !DISubprogram(name: "_CNW4mainE1S9makeShapeES_L0", linkageName: "_CNW4mainE1S9makeShapeES_L0", scope: !0, file: !11, line: 66, type: !22, isLocal: false, isDefinition: true, scopeLine: 66, thisAdjustment: 1, isOptimized: false, unit: !0)
!101 = !DILocalVariable(name: "self", scope: !100, file: !1, line: 66, type: !10)
!102 = !DILocation(line: 66, column: 6, scope: !100)
!103 = !DILocation(line: 67, column: 17, scope: !100)
!104 = distinct !DISubprogram(name: "_CNW4mainE1SGa6heeheeES_L0", linkageName: "_CNW4mainE1SGa6heeheeES_L0", scope: !0, file: !11, line: 70, type: !22, isLocal: false, isDefinition: true, scopeLine: 70, thisAdjustment: 1, isOptimized: false, unit: !0)
!105 = !DILocalVariable(name: "self", scope: !104, file: !1, line: 70, type: !10)
!106 = !DILocation(line: 70, column: 6, scope: !104)
!107 = !DILocation(line: 70, column: 33, scope: !104)
!108 = distinct !DISubprogram(name: "_CNW4mainE1S14__default_initES_L0", linkageName: "_CNW4mainE1S14__default_initES_L0", scope: !0, file: !11, line: 54, type: !22, isLocal: false, isDefinition: true, scopeLine: 54, thisAdjustment: 1, isOptimized: false, unit: !0)
!109 = !DILocation(line: 54, column: 2, scope: !108)
!110 = distinct !DISubprogram(name: "_CNW4mainE1SC2ES_L0", linkageName: "_CNW4mainE1SC2ES_L0", scope: !0, file: !11, line: 54, type: !22, isLocal: false, isDefinition: true, scopeLine: 54, thisAdjustment: 1, isOptimized: false, unit: !0)
!111 = !DILocation(line: 54, column: 2, scope: !110)
!112 = distinct !DISubprogram(name: "_CNW4mainE1SD3Ev", linkageName: "_CNW4mainE1SD3Ev", scope: null, file: !11, line: 54, type: !17, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!113 = !DILocation(line: 54, column: 2, scope: !112)
!114 = distinct !DISubprogram(name: "_CNW4mainE4BoolD0Ev", linkageName: "_CNW4mainE4BoolD0Ev", scope: !0, file: !6, line: 2, type: !115, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!115 = !DISubroutineType(types: !116)
!116 = !{!117}
!117 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !118, size: 64, align: 8)
!118 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.Bool", file: !6, line: 2, flags: DIFlagFwdDecl, elements: !119)
!119 = !{!120}
!120 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !118, file: !6, line: 2, baseType: !121, size: 8, align: 8)
!121 = !DIBasicType(name: "i1", size: 8, encoding: DW_ATE_boolean)
!122 = !DILocation(line: 2, column: 1, scope: !114)
!123 = distinct !DISubprogram(name: "_CNW4mainE4Bool4copyES0_L0", linkageName: "_CNW4mainE4Bool4copyES0_L0", scope: !0, file: !6, line: 2, type: !124, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!124 = !DISubroutineType(types: !125)
!125 = !{!118}
!126 = !DILocation(line: 2, column: 1, scope: !123)
!127 = distinct !DISubprogram(name: "_CNW4mainE4Bool14__default_initES0_L0", linkageName: "_CNW4mainE4Bool14__default_initES0_L0", scope: !0, file: !6, line: 2, type: !124, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!128 = !DILocation(line: 2, column: 1, scope: !127)
!129 = distinct !DISubprogram(name: "_CNW4mainE4BoolC1EbL5value", linkageName: "_CNW4mainE4BoolC1EbL5value", scope: !0, file: !6, line: 2, type: !130, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!130 = !DISubroutineType(types: !131)
!131 = !{!117, !121}
!132 = !DILocation(line: 2, column: 1, scope: !129)
!133 = distinct !DISubprogram(name: "_CNW4mainE4BoolC2ES0_bL0L5value", linkageName: "_CNW4mainE4BoolC2ES0_bL0L5value", scope: !0, file: !6, line: 2, type: !134, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!134 = !DISubroutineType(types: !135)
!135 = !{!118, !121}
!136 = !DILocation(line: 2, column: 1, scope: !133)
!137 = distinct !DISubprogram(name: "_CNW4mainE4BoolD3Ev", linkageName: "_CNW4mainE4BoolD3Ev", scope: null, file: !6, line: 2, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!138 = !DILocation(line: 2, column: 1, scope: !137)
!139 = distinct !DISubprogram(name: "_CNW4mainE4BoolXp1!ES0_L0", linkageName: "_CNW4mainE4BoolXp1!ES0_L0", scope: !0, file: !6, line: 2, type: !124, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!140 = !DILocalVariable(name: "self", scope: !139, file: !1, line: 2, type: !118)
!141 = !DILocation(line: 2, column: 1, scope: !139)
!142 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableD0Ev", linkageName: "_CNW4mainE17ValueWitnessTableD0Ev", scope: !0, file: !6, line: 2, type: !143, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!143 = !DISubroutineType(types: !144)
!144 = !{!145}
!145 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !146, size: 64, align: 8)
!146 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.ValueWitnessTable", file: !6, line: 2, flags: DIFlagFwdDecl, elements: !147)
!147 = !{!148, !153}
!148 = !DIDerivedType(tag: DW_TAG_member, name: "copyFn", scope: !146, file: !6, line: 2, baseType: !149, size: 64, align: 64)
!149 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !150, size: 64, align: 8)
!150 = !DISubroutineType(types: !151)
!151 = !{!152, !152}
!152 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64, align: 8)
!153 = !DIDerivedType(tag: DW_TAG_member, name: "deinit", scope: !146, file: !6, line: 2, baseType: !154, size: 64, align: 64, offset: 64)
!154 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !155, size: 64, align: 8)
!155 = !DISubroutineType(types: !156)
!156 = !{!152}
!157 = !DILocation(line: 2, column: 1, scope: !142)
!158 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTable14__default_initES0_L0", linkageName: "_CNW4mainE17ValueWitnessTable14__default_initES0_L0", scope: !0, file: !6, line: 2, type: !159, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!159 = !DISubroutineType(types: !160)
!160 = !{!146}
!161 = !DILocation(line: 2, column: 1, scope: !158)
!162 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit", linkageName: "_CNW4mainE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit", scope: !0, file: !6, line: 2, type: !163, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!163 = !DISubroutineType(types: !164)
!164 = !{!145, !149, !154}
!165 = !DILocation(line: 2, column: 1, scope: !162)
!166 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableC2ES0_DoFvPvKPvEDoFvS1_EL0L6copyFnL6deinit", linkageName: "_CNW4mainE17ValueWitnessTableC2ES0_DoFvPvKPvEDoFvS1_EL0L6copyFnL6deinit", scope: !0, file: !6, line: 2, type: !167, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!167 = !DISubroutineType(types: !168)
!168 = !{!146, !149, !154}
!169 = !DILocation(line: 2, column: 1, scope: !166)
!170 = distinct !DISubprogram(name: "_CNW4mainE17ValueWitnessTableD3Ev", linkageName: "_CNW4mainE17ValueWitnessTableD3Ev", scope: null, file: !6, line: 2, type: !143, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!171 = !DILocation(line: 2, column: 1, scope: !170)
!172 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceD0Ev", linkageName: "_CNW4mainE19ProtocolConformanceD0Ev", scope: !0, file: !6, line: 2, type: !173, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!173 = !DISubroutineType(types: !174)
!174 = !{!175}
!175 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !176, size: 64, align: 8)
!176 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.ProtocolConformance", file: !6, line: 2, flags: DIFlagFwdDecl, elements: !177)
!177 = !{!178, !196}
!178 = !DIDerivedType(tag: DW_TAG_member, name: "typeInfo", scope: !176, file: !6, line: 2, baseType: !179, size: 64, align: 64)
!179 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !180, size: 64, align: 8)
!180 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.TypeInfo", file: !6, line: 2, flags: DIFlagFwdDecl, elements: !181)
!181 = !{!182, !183, !185, !186, !188, !190, !192, !194, !195}
!182 = !DIDerivedType(tag: DW_TAG_member, name: "baseClass", scope: !180, file: !6, line: 2, baseType: !179, size: 64, align: 64)
!183 = !DIDerivedType(tag: DW_TAG_member, name: "vtable", scope: !180, file: !6, line: 2, baseType: !184, size: 64, align: 64, offset: 64)
!184 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !152, size: 64, align: 8)
!185 = !DIDerivedType(tag: DW_TAG_member, name: "deinit", scope: !180, file: !6, line: 2, baseType: !154, size: 64, align: 64, offset: 128)
!186 = !DIDerivedType(tag: DW_TAG_member, name: "name", scope: !180, file: !6, line: 2, baseType: !187, size: 64, align: 64, offset: 192)
!187 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !94, size: 64, align: 8)
!188 = !DIDerivedType(tag: DW_TAG_member, name: "valueWitnessTable", scope: !180, file: !6, line: 2, baseType: !189, size: 64, align: 64, offset: 256)
!189 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !146, size: 64, align: 8)
!190 = !DIDerivedType(tag: DW_TAG_member, name: "conformances", scope: !180, file: !6, line: 2, baseType: !191, size: 64, align: 64, offset: 320)
!191 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !176, size: 64, align: 8)
!192 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !180, file: !6, line: 2, baseType: !193, size: 64, align: 64, offset: 384)
!193 = !DIBasicType(name: "u64", size: 64, encoding: DW_ATE_unsigned)
!194 = !DIDerivedType(tag: DW_TAG_member, name: "alignment", scope: !180, file: !6, line: 2, baseType: !193, size: 64, align: 64, offset: 448)
!195 = !DIDerivedType(tag: DW_TAG_member, name: "stride", scope: !180, file: !6, line: 2, baseType: !193, size: 64, align: 64, offset: 512)
!196 = !DIDerivedType(tag: DW_TAG_member, name: "vtable", scope: !176, file: !6, line: 2, baseType: !184, size: 64, align: 64, offset: 64)
!197 = !DILocation(line: 2, column: 1, scope: !172)
!198 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformance14__default_initES0_L0", linkageName: "_CNW4mainE19ProtocolConformance14__default_initES0_L0", scope: !0, file: !6, line: 2, type: !199, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!199 = !DISubroutineType(types: !200)
!200 = !{!176}
!201 = !DILocation(line: 2, column: 1, scope: !198)
!202 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable", linkageName: "_CNW4mainE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable", scope: !0, file: !6, line: 2, type: !203, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!203 = !DISubroutineType(types: !204)
!204 = !{!175, !179, !184}
!205 = !DILocation(line: 2, column: 1, scope: !202)
!206 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceC2ES0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable", linkageName: "_CNW4mainE19ProtocolConformanceC2ES0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable", scope: !0, file: !6, line: 2, type: !207, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!207 = !DISubroutineType(types: !208)
!208 = !{!176, !179, !184}
!209 = !DILocation(line: 2, column: 1, scope: !206)
!210 = distinct !DISubprogram(name: "_CNW4mainE19ProtocolConformanceD3Ev", linkageName: "_CNW4mainE19ProtocolConformanceD3Ev", scope: null, file: !6, line: 2, type: !173, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!211 = !DILocation(line: 2, column: 1, scope: !210)
!212 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoD0Ev", linkageName: "_CNW4mainE8TypeInfoD0Ev", scope: !0, file: !6, line: 2, type: !213, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!213 = !DISubroutineType(types: !214)
!214 = !{!215}
!215 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !180, size: 64, align: 8)
!216 = !DILocation(line: 2, column: 1, scope: !212)
!217 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfo14__default_initES0_L0", linkageName: "_CNW4mainE8TypeInfo14__default_initES0_L0", scope: !0, file: !6, line: 2, type: !218, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!218 = !DISubroutineType(types: !219)
!219 = !{!180}
!220 = !DILocation(line: 2, column: 1, scope: !217)
!221 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", linkageName: "_CNW4mainE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", scope: !0, file: !6, line: 2, type: !222, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!222 = !DISubroutineType(types: !223)
!223 = !{!215, !179, !184, !154, !187, !189, !191, !193, !193, !193}
!224 = !DILocation(line: 2, column: 1, scope: !221)
!225 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoC2ES0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", linkageName: "_CNW4mainE8TypeInfoC2ES0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", scope: !0, file: !6, line: 2, type: !226, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!226 = !DISubroutineType(types: !227)
!227 = !{!180, !179, !184, !154, !187, !189, !191, !193, !193, !193}
!228 = !DILocation(line: 2, column: 1, scope: !225)
!229 = distinct !DISubprogram(name: "_CNW4mainE8TypeInfoD3Ev", linkageName: "_CNW4mainE8TypeInfoD3Ev", scope: null, file: !6, line: 2, type: !213, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!230 = !DILocation(line: 2, column: 1, scope: !229)
!231 = distinct !DISubprogram(name: "_CNW4mainE5Int64D0Ev", linkageName: "_CNW4mainE5Int64D0Ev", scope: !0, file: !6, line: 2, type: !232, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!232 = !DISubroutineType(types: !233)
!233 = !{!234}
!234 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !235, size: 64, align: 8)
!235 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.Int64", file: !6, line: 2, flags: DIFlagFwdDecl, elements: !236)
!236 = !{!237}
!237 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !235, file: !6, line: 2, baseType: !238, size: 64, align: 64)
!238 = !DIBasicType(name: "i64", size: 64, encoding: DW_ATE_signed)
!239 = !DILocation(line: 2, column: 1, scope: !231)
!240 = distinct !DISubprogram(name: "_CNW4mainE5Int644copyES0_L0", linkageName: "_CNW4mainE5Int644copyES0_L0", scope: !0, file: !6, line: 2, type: !241, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!241 = !DISubroutineType(types: !242)
!242 = !{!235}
!243 = !DILocation(line: 2, column: 1, scope: !240)
!244 = distinct !DISubprogram(name: "_CNW4mainE5Int6414__default_initES0_L0", linkageName: "_CNW4mainE5Int6414__default_initES0_L0", scope: !0, file: !6, line: 2, type: !241, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!245 = !DILocation(line: 2, column: 1, scope: !244)
!246 = distinct !DISubprogram(name: "_CNW4mainE5Int64C1ElL5value", linkageName: "_CNW4mainE5Int64C1ElL5value", scope: !0, file: !6, line: 2, type: !247, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!247 = !DISubroutineType(types: !248)
!248 = !{!234, !238}
!249 = !DILocation(line: 2, column: 1, scope: !246)
!250 = distinct !DISubprogram(name: "_CNW4mainE5Int64C2ES0_lL0L5value", linkageName: "_CNW4mainE5Int64C2ES0_lL0L5value", scope: !0, file: !6, line: 2, type: !251, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!251 = !DISubroutineType(types: !252)
!252 = !{!235, !238}
!253 = !DILocation(line: 2, column: 1, scope: !250)
!254 = distinct !DISubprogram(name: "_CNW4mainE5Int64D3Ev", linkageName: "_CNW4mainE5Int64D3Ev", scope: null, file: !6, line: 2, type: !232, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!255 = !DILocation(line: 2, column: 1, scope: !254)
!256 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerD0Ev", linkageName: "_CNW4mainE20ExistentialContainerD0Ev", scope: !0, file: !6, line: 2, type: !257, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!257 = !DISubroutineType(types: !258)
!258 = !{!259}
!259 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !260, size: 64, align: 8)
!260 = !DICompositeType(tag: DW_TAG_structure_type, name: "main.<private name>.ExistentialContainer", file: !6, line: 2, flags: DIFlagFwdDecl, elements: !261)
!261 = !{!262, !263, !264}
!262 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !260, file: !6, line: 2, baseType: !152, size: 64, align: 64)
!263 = !DIDerivedType(tag: DW_TAG_member, name: "typeInfo", scope: !260, file: !6, line: 2, baseType: !179, size: 64, align: 64, offset: 64)
!264 = !DIDerivedType(tag: DW_TAG_member, name: "conformance", scope: !260, file: !6, line: 2, baseType: !191, size: 64, align: 64, offset: 128)
!265 = !DILocation(line: 2, column: 1, scope: !256)
!266 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainer14__default_initES0_L0", linkageName: "_CNW4mainE20ExistentialContainer14__default_initES0_L0", scope: !0, file: !6, line: 2, type: !267, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!267 = !DISubroutineType(types: !268)
!268 = !{!260}
!269 = !DILocation(line: 2, column: 1, scope: !266)
!270 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance", linkageName: "_CNW4mainE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance", scope: !0, file: !6, line: 2, type: !271, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!271 = !DISubroutineType(types: !272)
!272 = !{!259, !152, !179, !191}
!273 = !DILocation(line: 2, column: 1, scope: !270)
!274 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerC2ES0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance", linkageName: "_CNW4mainE20ExistentialContainerC2ES0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance", scope: !0, file: !6, line: 2, type: !275, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!275 = !DISubroutineType(types: !276)
!276 = !{!260, !152, !179, !191}
!277 = !DILocation(line: 2, column: 1, scope: !274)
!278 = distinct !DISubprogram(name: "_CNW4mainE20ExistentialContainerD3Ev", linkageName: "_CNW4mainE20ExistentialContainerD3Ev", scope: null, file: !6, line: 2, type: !257, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!279 = !DILocation(line: 2, column: 1, scope: !278)
