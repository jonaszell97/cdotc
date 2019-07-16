; ModuleID = '/Users/Jonas/CDot/StdLib/builtin/mod.dotm'
source_filename = "/Users/Jonas/CDot/StdLib/builtin/mod.dotm"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-darwin18.0.0"

%"struct.builtin.<private name>.TypeInfo" = type <{ %"struct.builtin.<private name>.TypeInfo"*, i8**, void (i8*)*, i8*, %"struct.builtin.<private name>.ValueWitnessTable"*, %"struct.builtin.<private name>.ProtocolConformance"*, i64, i64, i64 }>
%"struct.builtin.<private name>.ValueWitnessTable" = type <{ void (i8*, i8*)*, void (i8*)* }>
%"struct.builtin.<private name>.ProtocolConformance" = type <{ %"struct.builtin.<private name>.TypeInfo"*, i8** }>
%"struct.builtin.<private name>.Int64" = type <{ i64 }>
%"struct.builtin.<private name>.Bool" = type <{ i1 }>
%"struct.builtin.<private name>.ExistentialContainer" = type <{ i8*, %"struct.builtin.<private name>.TypeInfo"*, %"struct.builtin.<private name>.ProtocolConformance"* }>

@_CTINW7builtinE14_Builtin_NeverE = constant %"struct.builtin.<private name>.TypeInfo" <{ %"struct.builtin.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (i64*)* @_CNW7builtinE14_Builtin_NeverD0Ev to void (i8*)*), i8* getelementptr inbounds ([23 x i8], [23 x i8]* @0, i32 0, i32 0), %"struct.builtin.<private name>.ValueWitnessTable"* @"0", %"struct.builtin.<private name>.ProtocolConformance"* getelementptr inbounds ([6 x %"struct.builtin.<private name>.ProtocolConformance"], [6 x %"struct.builtin.<private name>.ProtocolConformance"]* @"1", i32 0, i32 0), %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }> }>, align 8
@"0" = constant %"struct.builtin.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (i64 (i64)* @_CNW7builtinE14_Builtin_Never4copyES_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (i64*)* @_CNW7builtinE14_Builtin_NeverD0Ev to void (i8*)*) }>, align 8
@_CTINW7builtinE3AnyE = constant %"struct.builtin.<private name>.TypeInfo" <{ %"struct.builtin.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @1, i32 0, i32 0), %"struct.builtin.<private name>.ValueWitnessTable"* null, %"struct.builtin.<private name>.ProtocolConformance"* null, %"struct.builtin.<private name>.Int64" <{ i64 24 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW7builtinE14_Builtin_NeverENW_0E3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTINW7builtinE9EquatableE = constant %"struct.builtin.<private name>.TypeInfo" <{ %"struct.builtin.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([33 x i8], [33 x i8]* @2, i32 0, i32 0), %"struct.builtin.<private name>.ValueWitnessTable"* null, %"struct.builtin.<private name>.ProtocolConformance"* null, %"struct.builtin.<private name>.Int64" <{ i64 24 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW7builtinE14_Builtin_NeverENW_0E9EquatableE = constant [2 x i8*] [i8* bitcast (%"struct.builtin.<private name>.Bool" (i64, i64)* @"_CNW7builtinE14_Builtin_NeverXi2==ES_S_L0L0" to i8*), i8* bitcast (%"struct.builtin.<private name>.Bool" (i64, i64)* @"_CNW7builtinE14_Builtin_NeverXi2!=ES_S_L0L0" to i8*)], align 8
@_CTINW7builtinE16RawRepresentableE = constant %"struct.builtin.<private name>.TypeInfo" <{ %"struct.builtin.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @3, i32 0, i32 0), %"struct.builtin.<private name>.ValueWitnessTable"* null, %"struct.builtin.<private name>.ProtocolConformance"* null, %"struct.builtin.<private name>.Int64" <{ i64 24 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW7builtinE14_Builtin_NeverENW_0E16RawRepresentableE = constant [2 x i8*] [i8* bitcast (void (i64*, i64)* @_CNW7builtinE14_Builtin_NeverC1ElL8rawValue to i8*), i8* bitcast (i64 (i64)* @_CNW7builtinE14_Builtin_NeverGa8rawValueES_L0 to i8*)], align 8
@_CTINW7builtinE8CopyableE = constant %"struct.builtin.<private name>.TypeInfo" <{ %"struct.builtin.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([32 x i8], [32 x i8]* @4, i32 0, i32 0), %"struct.builtin.<private name>.ValueWitnessTable"* null, %"struct.builtin.<private name>.ProtocolConformance"* null, %"struct.builtin.<private name>.Int64" <{ i64 24 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW7builtinE14_Builtin_NeverENW_0E8CopyableE = constant [1 x i8*] [i8* bitcast (i64 (i64)* @_CNW7builtinE14_Builtin_Never4copyES_L0 to i8*)], align 8
@_CTINW7builtinE18ImplicitlyCopyableE = constant %"struct.builtin.<private name>.TypeInfo" <{ %"struct.builtin.<private name>.TypeInfo"* null, i8** null, void (i8*)* null, i8* getelementptr inbounds ([42 x i8], [42 x i8]* @5, i32 0, i32 0), %"struct.builtin.<private name>.ValueWitnessTable"* null, %"struct.builtin.<private name>.ProtocolConformance"* null, %"struct.builtin.<private name>.Int64" <{ i64 24 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 24 }> }>, align 8
@_CTPNW7builtinE14_Builtin_NeverENW_0E18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"1" = constant [6 x %"struct.builtin.<private name>.ProtocolConformance"] [%"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW7builtinE14_Builtin_NeverENW_0E3AnyE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE9EquatableE, i8** getelementptr inbounds ([2 x i8*], [2 x i8*]* @_CTPNW7builtinE14_Builtin_NeverENW_0E9EquatableE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE16RawRepresentableE, i8** getelementptr inbounds ([2 x i8*], [2 x i8*]* @_CTPNW7builtinE14_Builtin_NeverENW_0E16RawRepresentableE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW7builtinE14_Builtin_NeverENW_0E8CopyableE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW7builtinE14_Builtin_NeverENW_0E18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW7builtinE14AtomicRMWBinOpE = constant %"struct.builtin.<private name>.TypeInfo" <{ %"struct.builtin.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (i64*)* @_CNW7builtinE14AtomicRMWBinOpD0Ev to void (i8*)*), i8* getelementptr inbounds ([23 x i8], [23 x i8]* @6, i32 0, i32 0), %"struct.builtin.<private name>.ValueWitnessTable"* @"2", %"struct.builtin.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.builtin.<private name>.ProtocolConformance"], [2 x %"struct.builtin.<private name>.ProtocolConformance"]* @"3", i32 0, i32 0), %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }> }>, align 8
@"2" = constant %"struct.builtin.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (i64*)* @_CNW7builtinE14AtomicRMWBinOpD0Ev to void (i8*)*) }>, align 8
@_CTPNW7builtinE14AtomicRMWBinOpENW_0E3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"3" = constant [2 x %"struct.builtin.<private name>.ProtocolConformance"] [%"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW7builtinE14AtomicRMWBinOpENW_0E3AnyE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW7builtinE4BoolE = constant %"struct.builtin.<private name>.TypeInfo" <{ %"struct.builtin.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.builtin.<private name>.Bool"*)* @_CNW7builtinE4BoolD0Ev to void (i8*)*), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @7, i32 0, i32 0), %"struct.builtin.<private name>.ValueWitnessTable"* @"4", %"struct.builtin.<private name>.ProtocolConformance"* getelementptr inbounds ([4 x %"struct.builtin.<private name>.ProtocolConformance"], [4 x %"struct.builtin.<private name>.ProtocolConformance"]* @"5", i32 0, i32 0), %"struct.builtin.<private name>.Int64" <{ i64 1 }>, %"struct.builtin.<private name>.Int64" <{ i64 1 }>, %"struct.builtin.<private name>.Int64" <{ i64 1 }> }>, align 8
@"4" = constant %"struct.builtin.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (%"struct.builtin.<private name>.Bool" (%"struct.builtin.<private name>.Bool")* @_CNW7builtinE4Bool4copyES0_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (%"struct.builtin.<private name>.Bool"*)* @_CNW7builtinE4BoolD0Ev to void (i8*)*) }>, align 8
@_CTPNW7builtinE4BoolENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTPNW7builtinE4BoolENS_8CopyableE = constant [1 x i8*] [i8* bitcast (%"struct.builtin.<private name>.Bool" (%"struct.builtin.<private name>.Bool")* @_CNW7builtinE4Bool4copyES0_L0 to i8*)], align 8
@_CTPNW7builtinE4BoolENS_18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"5" = constant [4 x %"struct.builtin.<private name>.ProtocolConformance"] [%"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW7builtinE4BoolENS_3AnyE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW7builtinE4BoolENS_8CopyableE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW7builtinE4BoolENS_18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW7builtinE17ValueWitnessTableE = constant %"struct.builtin.<private name>.TypeInfo" <{ %"struct.builtin.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.builtin.<private name>.ValueWitnessTable"*)* @_CNW7builtinE17ValueWitnessTableD0Ev to void (i8*)*), i8* getelementptr inbounds ([41 x i8], [41 x i8]* @8, i32 0, i32 0), %"struct.builtin.<private name>.ValueWitnessTable"* @"6", %"struct.builtin.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.builtin.<private name>.ProtocolConformance"], [2 x %"struct.builtin.<private name>.ProtocolConformance"]* @"7", i32 0, i32 0), %"struct.builtin.<private name>.Int64" <{ i64 16 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 16 }> }>, align 8
@"6" = constant %"struct.builtin.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.builtin.<private name>.ValueWitnessTable"*)* @_CNW7builtinE17ValueWitnessTableD0Ev to void (i8*)*) }>, align 8
@_CTPNW7builtinE17ValueWitnessTableENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"7" = constant [2 x %"struct.builtin.<private name>.ProtocolConformance"] [%"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW7builtinE17ValueWitnessTableENS_3AnyE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW7builtinE19ProtocolConformanceE = constant %"struct.builtin.<private name>.TypeInfo" <{ %"struct.builtin.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.builtin.<private name>.ProtocolConformance"*)* @_CNW7builtinE19ProtocolConformanceD0Ev to void (i8*)*), i8* getelementptr inbounds ([43 x i8], [43 x i8]* @9, i32 0, i32 0), %"struct.builtin.<private name>.ValueWitnessTable"* @"8", %"struct.builtin.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.builtin.<private name>.ProtocolConformance"], [2 x %"struct.builtin.<private name>.ProtocolConformance"]* @"9", i32 0, i32 0), %"struct.builtin.<private name>.Int64" <{ i64 16 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 16 }> }>, align 8
@"8" = constant %"struct.builtin.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.builtin.<private name>.ProtocolConformance"*)* @_CNW7builtinE19ProtocolConformanceD0Ev to void (i8*)*) }>, align 8
@_CTPNW7builtinE19ProtocolConformanceENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"9" = constant [2 x %"struct.builtin.<private name>.ProtocolConformance"] [%"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW7builtinE19ProtocolConformanceENS_3AnyE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW7builtinE8TypeInfoE = constant %"struct.builtin.<private name>.TypeInfo" <{ %"struct.builtin.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.builtin.<private name>.TypeInfo"*)* @_CNW7builtinE8TypeInfoD0Ev to void (i8*)*), i8* getelementptr inbounds ([32 x i8], [32 x i8]* @10, i32 0, i32 0), %"struct.builtin.<private name>.ValueWitnessTable"* @"10", %"struct.builtin.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.builtin.<private name>.ProtocolConformance"], [2 x %"struct.builtin.<private name>.ProtocolConformance"]* @"11", i32 0, i32 0), %"struct.builtin.<private name>.Int64" <{ i64 72 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 72 }> }>, align 8
@"10" = constant %"struct.builtin.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.builtin.<private name>.TypeInfo"*)* @_CNW7builtinE8TypeInfoD0Ev to void (i8*)*) }>, align 8
@_CTPNW7builtinE8TypeInfoENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"11" = constant [2 x %"struct.builtin.<private name>.ProtocolConformance"] [%"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW7builtinE8TypeInfoENS_3AnyE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW7builtinE5Int64E = constant %"struct.builtin.<private name>.TypeInfo" <{ %"struct.builtin.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.builtin.<private name>.Int64"*)* @_CNW7builtinE5Int64D0Ev to void (i8*)*), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @11, i32 0, i32 0), %"struct.builtin.<private name>.ValueWitnessTable"* @"12", %"struct.builtin.<private name>.ProtocolConformance"* getelementptr inbounds ([4 x %"struct.builtin.<private name>.ProtocolConformance"], [4 x %"struct.builtin.<private name>.ProtocolConformance"]* @"13", i32 0, i32 0), %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }> }>, align 8
@"12" = constant %"struct.builtin.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* bitcast (%"struct.builtin.<private name>.Int64" (%"struct.builtin.<private name>.Int64")* @_CNW7builtinE5Int644copyES0_L0 to void (i8*, i8*)*), void (i8*)* bitcast (void (%"struct.builtin.<private name>.Int64"*)* @_CNW7builtinE5Int64D0Ev to void (i8*)*) }>, align 8
@_CTPNW7builtinE5Int64ENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@_CTPNW7builtinE5Int64ENS_8CopyableE = constant [1 x i8*] [i8* bitcast (%"struct.builtin.<private name>.Int64" (%"struct.builtin.<private name>.Int64")* @_CNW7builtinE5Int644copyES0_L0 to i8*)], align 8
@_CTPNW7builtinE5Int64ENS_18ImplicitlyCopyableE = constant [0 x i8*] zeroinitializer, align 8
@"13" = constant [4 x %"struct.builtin.<private name>.ProtocolConformance"] [%"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW7builtinE5Int64ENS_3AnyE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE8CopyableE, i8** getelementptr inbounds ([1 x i8*], [1 x i8*]* @_CTPNW7builtinE5Int64ENS_8CopyableE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE18ImplicitlyCopyableE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW7builtinE5Int64ENS_18ImplicitlyCopyableE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" zeroinitializer], align 8
@_CTINW7builtinE20ExistentialContainerE = constant %"struct.builtin.<private name>.TypeInfo" <{ %"struct.builtin.<private name>.TypeInfo"* null, i8** null, void (i8*)* bitcast (void (%"struct.builtin.<private name>.ExistentialContainer"*)* @_CNW7builtinE20ExistentialContainerD0Ev to void (i8*)*), i8* getelementptr inbounds ([44 x i8], [44 x i8]* @12, i32 0, i32 0), %"struct.builtin.<private name>.ValueWitnessTable"* @"14", %"struct.builtin.<private name>.ProtocolConformance"* getelementptr inbounds ([2 x %"struct.builtin.<private name>.ProtocolConformance"], [2 x %"struct.builtin.<private name>.ProtocolConformance"]* @"15", i32 0, i32 0), %"struct.builtin.<private name>.Int64" <{ i64 24 }>, %"struct.builtin.<private name>.Int64" <{ i64 8 }>, %"struct.builtin.<private name>.Int64" <{ i64 24 }> }>, align 8
@"14" = constant %"struct.builtin.<private name>.ValueWitnessTable" <{ void (i8*, i8*)* null, void (i8*)* bitcast (void (%"struct.builtin.<private name>.ExistentialContainer"*)* @_CNW7builtinE20ExistentialContainerD0Ev to void (i8*)*) }>, align 8
@_CTPNW7builtinE20ExistentialContainerENS_3AnyE = constant [0 x i8*] zeroinitializer, align 8
@"15" = constant [2 x %"struct.builtin.<private name>.ProtocolConformance"] [%"struct.builtin.<private name>.ProtocolConformance" <{ %"struct.builtin.<private name>.TypeInfo"* @_CTINW7builtinE3AnyE, i8** getelementptr inbounds ([0 x i8*], [0 x i8*]* @_CTPNW7builtinE20ExistentialContainerENS_3AnyE, i32 0, i32 0) }>, %"struct.builtin.<private name>.ProtocolConformance" zeroinitializer], align 8
@0 = private unnamed_addr constant [23 x i8] c"builtin._Builtin_Never\00"
@1 = private unnamed_addr constant [27 x i8] c"builtin.<private name>.Any\00"
@2 = private unnamed_addr constant [33 x i8] c"builtin.<private name>.Equatable\00"
@3 = private unnamed_addr constant [40 x i8] c"builtin.<private name>.RawRepresentable\00"
@4 = private unnamed_addr constant [32 x i8] c"builtin.<private name>.Copyable\00"
@5 = private unnamed_addr constant [42 x i8] c"builtin.<private name>.ImplicitlyCopyable\00"
@6 = private unnamed_addr constant [23 x i8] c"builtin.AtomicRMWBinOp\00"
@7 = private unnamed_addr constant [28 x i8] c"builtin.<private name>.Bool\00"
@8 = private unnamed_addr constant [41 x i8] c"builtin.<private name>.ValueWitnessTable\00"
@9 = private unnamed_addr constant [43 x i8] c"builtin.<private name>.ProtocolConformance\00"
@10 = private unnamed_addr constant [32 x i8] c"builtin.<private name>.TypeInfo\00"
@11 = private unnamed_addr constant [29 x i8] c"builtin.<private name>.Int64\00"
@12 = private unnamed_addr constant [44 x i8] c"builtin.<private name>.ExistentialContainer\00"
@13 = private unnamed_addr constant [17 x i8] c"llvm.coro.resume\00"
@14 = private unnamed_addr constant [18 x i8] c"llvm.coro.destroy\00"
@15 = private unnamed_addr constant [15 x i8] c"llvm.coro.done\00"
@16 = private unnamed_addr constant [15 x i8] c"llvm.log10.f32\00"
@17 = private unnamed_addr constant [14 x i8] c"llvm.log2.f32\00"
@18 = private unnamed_addr constant [14 x i8] c"llvm.log1.f32\00"
@19 = private unnamed_addr constant [15 x i8] c"llvm.floor.f32\00"
@20 = private unnamed_addr constant [14 x i8] c"llvm.ceil.f32\00"
@21 = private unnamed_addr constant [15 x i8] c"llvm.round.f32\00"
@22 = private unnamed_addr constant [14 x i8] c"llvm.rint.f32\00"
@23 = private unnamed_addr constant [15 x i8] c"llvm.trunc.f32\00"
@24 = private unnamed_addr constant [14 x i8] c"llvm.sqrt.f32\00"
@25 = private unnamed_addr constant [14 x i8] c"llvm.fabs.f32\00"
@26 = private unnamed_addr constant [13 x i8] c"llvm.sin.f32\00"
@27 = private unnamed_addr constant [13 x i8] c"llvm.cos.f32\00"
@28 = private unnamed_addr constant [13 x i8] c"llvm.exp.f32\00"
@29 = private unnamed_addr constant [14 x i8] c"llvm.exp2.f32\00"
@30 = private unnamed_addr constant [13 x i8] c"llvm.pow.f32\00"
@31 = private unnamed_addr constant [14 x i8] c"llvm.powi.f32\00"
@32 = private unnamed_addr constant [16 x i8] c"llvm.minnum.f32\00"
@33 = private unnamed_addr constant [16 x i8] c"llvm.maxnum.f32\00"
@34 = private unnamed_addr constant [17 x i8] c"llvm.minimum.f32\00"
@35 = private unnamed_addr constant [17 x i8] c"llvm.maximum.f32\00"
@36 = private unnamed_addr constant [18 x i8] c"llvm.copysign.f32\00"
@37 = private unnamed_addr constant [13 x i8] c"llvm.fma.f32\00"
@38 = private unnamed_addr constant [15 x i8] c"llvm.log10.f64\00"
@39 = private unnamed_addr constant [14 x i8] c"llvm.log2.f64\00"
@40 = private unnamed_addr constant [14 x i8] c"llvm.log1.f64\00"
@41 = private unnamed_addr constant [15 x i8] c"llvm.floor.f64\00"
@42 = private unnamed_addr constant [14 x i8] c"llvm.ceil.f64\00"
@43 = private unnamed_addr constant [15 x i8] c"llvm.round.f64\00"
@44 = private unnamed_addr constant [14 x i8] c"llvm.rint.f64\00"
@45 = private unnamed_addr constant [15 x i8] c"llvm.trunc.f64\00"
@46 = private unnamed_addr constant [14 x i8] c"llvm.sqrt.f64\00"
@47 = private unnamed_addr constant [14 x i8] c"llvm.fabs.f64\00"
@48 = private unnamed_addr constant [13 x i8] c"llvm.sin.f64\00"
@49 = private unnamed_addr constant [13 x i8] c"llvm.cos.f64\00"
@50 = private unnamed_addr constant [13 x i8] c"llvm.exp.f64\00"
@51 = private unnamed_addr constant [14 x i8] c"llvm.exp2.f64\00"
@52 = private unnamed_addr constant [13 x i8] c"llvm.pow.f64\00"
@53 = private unnamed_addr constant [14 x i8] c"llvm.powi.f64\00"
@54 = private unnamed_addr constant [16 x i8] c"llvm.minnum.f64\00"
@55 = private unnamed_addr constant [16 x i8] c"llvm.maxnum.f64\00"
@56 = private unnamed_addr constant [17 x i8] c"llvm.minimum.f64\00"
@57 = private unnamed_addr constant [17 x i8] c"llvm.maximum.f64\00"
@58 = private unnamed_addr constant [18 x i8] c"llvm.copysign.f64\00"
@59 = private unnamed_addr constant [13 x i8] c"llvm.fma.f64\00"
@60 = private unnamed_addr constant [13 x i8] c"llvm.ctlz.i1\00"
@61 = private unnamed_addr constant [13 x i8] c"llvm.cttz.i1\00"
@62 = private unnamed_addr constant [14 x i8] c"llvm.ctpop.i1\00"
@63 = private unnamed_addr constant [19 x i8] c"llvm.bitreverse.i1\00"
@64 = private unnamed_addr constant [27 x i8] c"llvm.sadd.with.overflow.i1\00"
@65 = private unnamed_addr constant [27 x i8] c"llvm.uadd.with.overflow.i1\00"
@66 = private unnamed_addr constant [27 x i8] c"llvm.ssub.with.overflow.i1\00"
@67 = private unnamed_addr constant [27 x i8] c"llvm.usub.with.overflow.i1\00"
@68 = private unnamed_addr constant [27 x i8] c"llvm.smul.with.overflow.i1\00"
@69 = private unnamed_addr constant [27 x i8] c"llvm.umul.with.overflow.i1\00"
@70 = private unnamed_addr constant [13 x i8] c"llvm.ctlz.i8\00"
@71 = private unnamed_addr constant [13 x i8] c"llvm.cttz.i8\00"
@72 = private unnamed_addr constant [14 x i8] c"llvm.ctpop.i8\00"
@73 = private unnamed_addr constant [19 x i8] c"llvm.bitreverse.i8\00"
@74 = private unnamed_addr constant [27 x i8] c"llvm.sadd.with.overflow.i8\00"
@75 = private unnamed_addr constant [27 x i8] c"llvm.uadd.with.overflow.i8\00"
@76 = private unnamed_addr constant [27 x i8] c"llvm.ssub.with.overflow.i8\00"
@77 = private unnamed_addr constant [27 x i8] c"llvm.usub.with.overflow.i8\00"
@78 = private unnamed_addr constant [27 x i8] c"llvm.smul.with.overflow.i8\00"
@79 = private unnamed_addr constant [27 x i8] c"llvm.umul.with.overflow.i8\00"
@80 = private unnamed_addr constant [14 x i8] c"llvm.ctlz.i16\00"
@81 = private unnamed_addr constant [14 x i8] c"llvm.cttz.i16\00"
@82 = private unnamed_addr constant [15 x i8] c"llvm.ctpop.i16\00"
@83 = private unnamed_addr constant [20 x i8] c"llvm.bitreverse.i16\00"
@84 = private unnamed_addr constant [28 x i8] c"llvm.sadd.with.overflow.i16\00"
@85 = private unnamed_addr constant [28 x i8] c"llvm.uadd.with.overflow.i16\00"
@86 = private unnamed_addr constant [28 x i8] c"llvm.ssub.with.overflow.i16\00"
@87 = private unnamed_addr constant [28 x i8] c"llvm.usub.with.overflow.i16\00"
@88 = private unnamed_addr constant [28 x i8] c"llvm.smul.with.overflow.i16\00"
@89 = private unnamed_addr constant [28 x i8] c"llvm.umul.with.overflow.i16\00"
@90 = private unnamed_addr constant [14 x i8] c"llvm.ctlz.i32\00"
@91 = private unnamed_addr constant [14 x i8] c"llvm.cttz.i32\00"
@92 = private unnamed_addr constant [15 x i8] c"llvm.ctpop.i32\00"
@93 = private unnamed_addr constant [20 x i8] c"llvm.bitreverse.i32\00"
@94 = private unnamed_addr constant [28 x i8] c"llvm.sadd.with.overflow.i32\00"
@95 = private unnamed_addr constant [28 x i8] c"llvm.uadd.with.overflow.i32\00"
@96 = private unnamed_addr constant [28 x i8] c"llvm.ssub.with.overflow.i32\00"
@97 = private unnamed_addr constant [28 x i8] c"llvm.usub.with.overflow.i32\00"
@98 = private unnamed_addr constant [28 x i8] c"llvm.smul.with.overflow.i32\00"
@99 = private unnamed_addr constant [28 x i8] c"llvm.umul.with.overflow.i32\00"
@100 = private unnamed_addr constant [14 x i8] c"llvm.ctlz.i64\00"
@101 = private unnamed_addr constant [14 x i8] c"llvm.cttz.i64\00"
@102 = private unnamed_addr constant [15 x i8] c"llvm.ctpop.i64\00"
@103 = private unnamed_addr constant [20 x i8] c"llvm.bitreverse.i64\00"
@104 = private unnamed_addr constant [28 x i8] c"llvm.sadd.with.overflow.i64\00"
@105 = private unnamed_addr constant [28 x i8] c"llvm.uadd.with.overflow.i64\00"
@106 = private unnamed_addr constant [28 x i8] c"llvm.ssub.with.overflow.i64\00"
@107 = private unnamed_addr constant [28 x i8] c"llvm.usub.with.overflow.i64\00"
@108 = private unnamed_addr constant [28 x i8] c"llvm.smul.with.overflow.i64\00"
@109 = private unnamed_addr constant [28 x i8] c"llvm.umul.with.overflow.i64\00"
@110 = private unnamed_addr constant [15 x i8] c"llvm.ctlz.i128\00"
@111 = private unnamed_addr constant [15 x i8] c"llvm.cttz.i128\00"
@112 = private unnamed_addr constant [16 x i8] c"llvm.ctpop.i128\00"
@113 = private unnamed_addr constant [21 x i8] c"llvm.bitreverse.i128\00"
@114 = private unnamed_addr constant [29 x i8] c"llvm.sadd.with.overflow.i128\00"
@115 = private unnamed_addr constant [29 x i8] c"llvm.uadd.with.overflow.i128\00"
@116 = private unnamed_addr constant [29 x i8] c"llvm.ssub.with.overflow.i128\00"
@117 = private unnamed_addr constant [29 x i8] c"llvm.usub.with.overflow.i128\00"
@118 = private unnamed_addr constant [29 x i8] c"llvm.smul.with.overflow.i128\00"
@119 = private unnamed_addr constant [29 x i8] c"llvm.umul.with.overflow.i128\00"
@120 = private unnamed_addr constant [15 x i8] c"llvm.bswap.i16\00"
@121 = private unnamed_addr constant [15 x i8] c"llvm.bswap.i32\00"
@122 = private unnamed_addr constant [15 x i8] c"llvm.bswap.i64\00"
@123 = private unnamed_addr constant [16 x i8] c"llvm.bswap.i128\00"

define i1 @"_CNW7builtin3opsEXi2==EKPvS_L0L0"(i8*, i8*) !dbg !5 {
alloca_block:
  %2 = alloca i8*, align 8
  %3 = alloca i8*, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8** %3, metadata !10, metadata !DIExpression()), !dbg !11
  call void @llvm.dbg.value(metadata i8* %0, metadata !10, metadata !DIExpression()), !dbg !11
  store i8* %0, i8** %3, align 8, !dbg !11
  call void @llvm.dbg.declare(metadata i8** %2, metadata !12, metadata !DIExpression()), !dbg !11
  call void @llvm.dbg.value(metadata i8* %1, metadata !12, metadata !DIExpression()), !dbg !11
  store i8* %1, i8** %2, align 8, !dbg !11
  %4 = load i8*, i8** %3, align 8, !dbg !13
  %5 = load i8*, i8** %2, align 8, !dbg !14
  %6 = call i1 @_CNW7builtinE11icmp_eq_ptrEKPvS_L0L0(i8* %4, i8* %5), !dbg !15
  ret i1 %6, !dbg !15
}

define i1 @_CNW7builtinE11icmp_eq_ptrEKPvS_L0L0(i8*, i8*) !dbg !16 {
alloca_block:
  %2 = alloca i8*, align 8
  %3 = alloca i8*, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8** %3, metadata !18, metadata !DIExpression()), !dbg !19
  call void @llvm.dbg.value(metadata i8* %0, metadata !18, metadata !DIExpression()), !dbg !19
  store i8* %0, i8** %3, align 8, !dbg !19
  call void @llvm.dbg.declare(metadata i8** %2, metadata !20, metadata !DIExpression()), !dbg !19
  call void @llvm.dbg.value(metadata i8* %1, metadata !20, metadata !DIExpression()), !dbg !19
  store i8* %1, i8** %2, align 8, !dbg !19
  %4 = load i8*, i8** %3, align 8, !dbg !21
  %5 = ptrtoint i8* %4 to i64, !dbg !21
  %6 = load i8*, i8** %2, align 8, !dbg !21
  %7 = ptrtoint i8* %6 to i64, !dbg !21
  %8 = icmp eq i64 %5, %7, !dbg !21
  ret i1 %8, !dbg !21
}

define i1 @"_CNW7builtin3opsEXi2!=EKPvS_L0L0"(i8*, i8*) !dbg !22 {
alloca_block:
  %2 = alloca i8*, align 8
  %3 = alloca i8*, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8** %3, metadata !23, metadata !DIExpression()), !dbg !24
  call void @llvm.dbg.value(metadata i8* %0, metadata !23, metadata !DIExpression()), !dbg !24
  store i8* %0, i8** %3, align 8, !dbg !24
  call void @llvm.dbg.declare(metadata i8** %2, metadata !25, metadata !DIExpression()), !dbg !24
  call void @llvm.dbg.value(metadata i8* %1, metadata !25, metadata !DIExpression()), !dbg !24
  store i8* %1, i8** %2, align 8, !dbg !24
  %4 = load i8*, i8** %3, align 8, !dbg !26
  %5 = load i8*, i8** %2, align 8, !dbg !27
  %6 = call i1 @_CNW7builtinE11icmp_ne_ptrEKPvS_L0L0(i8* %4, i8* %5), !dbg !28
  ret i1 %6, !dbg !28
}

define i1 @_CNW7builtinE11icmp_ne_ptrEKPvS_L0L0(i8*, i8*) !dbg !29 {
alloca_block:
  %2 = alloca i8*, align 8
  %3 = alloca i8*, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8** %3, metadata !30, metadata !DIExpression()), !dbg !31
  call void @llvm.dbg.value(metadata i8* %0, metadata !30, metadata !DIExpression()), !dbg !31
  store i8* %0, i8** %3, align 8, !dbg !31
  call void @llvm.dbg.declare(metadata i8** %2, metadata !32, metadata !DIExpression()), !dbg !31
  call void @llvm.dbg.value(metadata i8* %1, metadata !32, metadata !DIExpression()), !dbg !31
  store i8* %1, i8** %2, align 8, !dbg !31
  %4 = load i8*, i8** %3, align 8, !dbg !33
  %5 = ptrtoint i8* %4 to i64, !dbg !33
  %6 = load i8*, i8** %2, align 8, !dbg !33
  %7 = ptrtoint i8* %6 to i64, !dbg !33
  %8 = icmp ne i64 %5, %7, !dbg !33
  ret i1 %8, !dbg !33
}

define i8 @"_CNW7builtin3opsEXi1+EaaL0L0"(i8, i8) !dbg !34 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !38, metadata !DIExpression()), !dbg !39
  call void @llvm.dbg.value(metadata i8 %0, metadata !38, metadata !DIExpression()), !dbg !39
  store i8 %0, i8* %3, align 1, !dbg !39
  call void @llvm.dbg.declare(metadata i8* %2, metadata !40, metadata !DIExpression()), !dbg !39
  call void @llvm.dbg.value(metadata i8 %1, metadata !40, metadata !DIExpression()), !dbg !39
  store i8 %1, i8* %2, align 1, !dbg !39
  %4 = load i8, i8* %3, align 1, !dbg !41
  %5 = load i8, i8* %2, align 1, !dbg !41
  %6 = add nsw i8 %4, %5, !dbg !41
  ret i8 %6, !dbg !41
}

define void @"_CNW7builtin3opsEXi2+=EaaL0L0"(i8*, i8) !dbg !42 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !46, metadata !DIExpression()), !dbg !47
  call void @llvm.dbg.declare(metadata i8* %2, metadata !48, metadata !DIExpression()), !dbg !47
  call void @llvm.dbg.value(metadata i8 %1, metadata !48, metadata !DIExpression()), !dbg !47
  store i8 %1, i8* %2, align 1, !dbg !47
  %3 = load i8, i8* %0, align 1, !dbg !49
  %4 = load i8, i8* %2, align 1, !dbg !49
  %5 = add nsw i8 %3, %4, !dbg !49
  call void @llvm.dbg.value(metadata i8 %5, metadata !46, metadata !DIExpression()), !dbg !49
  %6 = load i8, i8* %0, align 1, !dbg !49
  store i8 %5, i8* %0, align 1, !dbg !49
  ret void, !dbg !49
}

define i8 @"_CNW7builtin3opsEXi1+EhhL0L0"(i8, i8) !dbg !50 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !54, metadata !DIExpression()), !dbg !55
  call void @llvm.dbg.value(metadata i8 %0, metadata !54, metadata !DIExpression()), !dbg !55
  store i8 %0, i8* %3, align 1, !dbg !55
  call void @llvm.dbg.declare(metadata i8* %2, metadata !56, metadata !DIExpression()), !dbg !55
  call void @llvm.dbg.value(metadata i8 %1, metadata !56, metadata !DIExpression()), !dbg !55
  store i8 %1, i8* %2, align 1, !dbg !55
  %4 = load i8, i8* %3, align 1, !dbg !57
  %5 = load i8, i8* %2, align 1, !dbg !57
  %6 = add nuw i8 %4, %5, !dbg !57
  ret i8 %6, !dbg !57
}

define void @"_CNW7builtin3opsEXi2+=EhhL0L0"(i8*, i8) !dbg !58 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !62, metadata !DIExpression()), !dbg !63
  call void @llvm.dbg.declare(metadata i8* %2, metadata !64, metadata !DIExpression()), !dbg !63
  call void @llvm.dbg.value(metadata i8 %1, metadata !64, metadata !DIExpression()), !dbg !63
  store i8 %1, i8* %2, align 1, !dbg !63
  %3 = load i8, i8* %0, align 1, !dbg !65
  %4 = load i8, i8* %2, align 1, !dbg !65
  %5 = add nuw i8 %3, %4, !dbg !65
  call void @llvm.dbg.value(metadata i8 %5, metadata !62, metadata !DIExpression()), !dbg !65
  %6 = load i8, i8* %0, align 1, !dbg !65
  store i8 %5, i8* %0, align 1, !dbg !65
  ret void, !dbg !65
}

define i16 @"_CNW7builtin3opsEXi1+EssL0L0"(i16, i16) !dbg !66 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !70, metadata !DIExpression()), !dbg !71
  call void @llvm.dbg.value(metadata i16 %0, metadata !70, metadata !DIExpression()), !dbg !71
  store i16 %0, i16* %3, align 2, !dbg !71
  call void @llvm.dbg.declare(metadata i16* %2, metadata !72, metadata !DIExpression()), !dbg !71
  call void @llvm.dbg.value(metadata i16 %1, metadata !72, metadata !DIExpression()), !dbg !71
  store i16 %1, i16* %2, align 2, !dbg !71
  %4 = load i16, i16* %3, align 2, !dbg !73
  %5 = load i16, i16* %2, align 2, !dbg !73
  %6 = add nsw i16 %4, %5, !dbg !73
  ret i16 %6, !dbg !73
}

define void @"_CNW7builtin3opsEXi2+=EssL0L0"(i16*, i16) !dbg !74 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !78, metadata !DIExpression()), !dbg !79
  call void @llvm.dbg.declare(metadata i16* %2, metadata !80, metadata !DIExpression()), !dbg !79
  call void @llvm.dbg.value(metadata i16 %1, metadata !80, metadata !DIExpression()), !dbg !79
  store i16 %1, i16* %2, align 2, !dbg !79
  %3 = load i16, i16* %0, align 2, !dbg !81
  %4 = load i16, i16* %2, align 2, !dbg !81
  %5 = add nsw i16 %3, %4, !dbg !81
  call void @llvm.dbg.value(metadata i16 %5, metadata !78, metadata !DIExpression()), !dbg !81
  %6 = load i16, i16* %0, align 2, !dbg !81
  store i16 %5, i16* %0, align 2, !dbg !81
  ret void, !dbg !81
}

define i16 @"_CNW7builtin3opsEXi1+EttL0L0"(i16, i16) !dbg !82 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !86, metadata !DIExpression()), !dbg !87
  call void @llvm.dbg.value(metadata i16 %0, metadata !86, metadata !DIExpression()), !dbg !87
  store i16 %0, i16* %3, align 2, !dbg !87
  call void @llvm.dbg.declare(metadata i16* %2, metadata !88, metadata !DIExpression()), !dbg !87
  call void @llvm.dbg.value(metadata i16 %1, metadata !88, metadata !DIExpression()), !dbg !87
  store i16 %1, i16* %2, align 2, !dbg !87
  %4 = load i16, i16* %3, align 2, !dbg !89
  %5 = load i16, i16* %2, align 2, !dbg !89
  %6 = add nuw i16 %4, %5, !dbg !89
  ret i16 %6, !dbg !89
}

define void @"_CNW7builtin3opsEXi2+=EttL0L0"(i16*, i16) !dbg !90 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !94, metadata !DIExpression()), !dbg !95
  call void @llvm.dbg.declare(metadata i16* %2, metadata !96, metadata !DIExpression()), !dbg !95
  call void @llvm.dbg.value(metadata i16 %1, metadata !96, metadata !DIExpression()), !dbg !95
  store i16 %1, i16* %2, align 2, !dbg !95
  %3 = load i16, i16* %0, align 2, !dbg !97
  %4 = load i16, i16* %2, align 2, !dbg !97
  %5 = add nuw i16 %3, %4, !dbg !97
  call void @llvm.dbg.value(metadata i16 %5, metadata !94, metadata !DIExpression()), !dbg !97
  %6 = load i16, i16* %0, align 2, !dbg !97
  store i16 %5, i16* %0, align 2, !dbg !97
  ret void, !dbg !97
}

define i32 @"_CNW7builtin3opsEXi1+EiiL0L0"(i32, i32) !dbg !98 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !102, metadata !DIExpression()), !dbg !103
  call void @llvm.dbg.value(metadata i32 %0, metadata !102, metadata !DIExpression()), !dbg !103
  store i32 %0, i32* %3, align 4, !dbg !103
  call void @llvm.dbg.declare(metadata i32* %2, metadata !104, metadata !DIExpression()), !dbg !103
  call void @llvm.dbg.value(metadata i32 %1, metadata !104, metadata !DIExpression()), !dbg !103
  store i32 %1, i32* %2, align 4, !dbg !103
  %4 = load i32, i32* %3, align 4, !dbg !105
  %5 = load i32, i32* %2, align 4, !dbg !105
  %6 = add nsw i32 %4, %5, !dbg !105
  ret i32 %6, !dbg !105
}

define void @"_CNW7builtin3opsEXi2+=EiiL0L0"(i32*, i32) !dbg !106 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !110, metadata !DIExpression()), !dbg !111
  call void @llvm.dbg.declare(metadata i32* %2, metadata !112, metadata !DIExpression()), !dbg !111
  call void @llvm.dbg.value(metadata i32 %1, metadata !112, metadata !DIExpression()), !dbg !111
  store i32 %1, i32* %2, align 4, !dbg !111
  %3 = load i32, i32* %0, align 4, !dbg !113
  %4 = load i32, i32* %2, align 4, !dbg !113
  %5 = add nsw i32 %3, %4, !dbg !113
  call void @llvm.dbg.value(metadata i32 %5, metadata !110, metadata !DIExpression()), !dbg !113
  %6 = load i32, i32* %0, align 4, !dbg !113
  store i32 %5, i32* %0, align 4, !dbg !113
  ret void, !dbg !113
}

define i32 @"_CNW7builtin3opsEXi1+EjjL0L0"(i32, i32) !dbg !114 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !118, metadata !DIExpression()), !dbg !119
  call void @llvm.dbg.value(metadata i32 %0, metadata !118, metadata !DIExpression()), !dbg !119
  store i32 %0, i32* %3, align 4, !dbg !119
  call void @llvm.dbg.declare(metadata i32* %2, metadata !120, metadata !DIExpression()), !dbg !119
  call void @llvm.dbg.value(metadata i32 %1, metadata !120, metadata !DIExpression()), !dbg !119
  store i32 %1, i32* %2, align 4, !dbg !119
  %4 = load i32, i32* %3, align 4, !dbg !121
  %5 = load i32, i32* %2, align 4, !dbg !121
  %6 = add nuw i32 %4, %5, !dbg !121
  ret i32 %6, !dbg !121
}

define void @"_CNW7builtin3opsEXi2+=EjjL0L0"(i32*, i32) !dbg !122 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !126, metadata !DIExpression()), !dbg !127
  call void @llvm.dbg.declare(metadata i32* %2, metadata !128, metadata !DIExpression()), !dbg !127
  call void @llvm.dbg.value(metadata i32 %1, metadata !128, metadata !DIExpression()), !dbg !127
  store i32 %1, i32* %2, align 4, !dbg !127
  %3 = load i32, i32* %0, align 4, !dbg !129
  %4 = load i32, i32* %2, align 4, !dbg !129
  %5 = add nuw i32 %3, %4, !dbg !129
  call void @llvm.dbg.value(metadata i32 %5, metadata !126, metadata !DIExpression()), !dbg !129
  %6 = load i32, i32* %0, align 4, !dbg !129
  store i32 %5, i32* %0, align 4, !dbg !129
  ret void, !dbg !129
}

define i64 @"_CNW7builtin3opsEXi1+EllL0L0"(i64, i64) !dbg !130 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !134, metadata !DIExpression()), !dbg !135
  call void @llvm.dbg.value(metadata i64 %0, metadata !134, metadata !DIExpression()), !dbg !135
  store i64 %0, i64* %3, align 8, !dbg !135
  call void @llvm.dbg.declare(metadata i64* %2, metadata !136, metadata !DIExpression()), !dbg !135
  call void @llvm.dbg.value(metadata i64 %1, metadata !136, metadata !DIExpression()), !dbg !135
  store i64 %1, i64* %2, align 8, !dbg !135
  %4 = load i64, i64* %3, align 8, !dbg !137
  %5 = load i64, i64* %2, align 8, !dbg !137
  %6 = add nsw i64 %4, %5, !dbg !137
  ret i64 %6, !dbg !137
}

define void @"_CNW7builtin3opsEXi2+=EllL0L0"(i64*, i64) !dbg !138 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !142, metadata !DIExpression()), !dbg !143
  call void @llvm.dbg.declare(metadata i64* %2, metadata !144, metadata !DIExpression()), !dbg !143
  call void @llvm.dbg.value(metadata i64 %1, metadata !144, metadata !DIExpression()), !dbg !143
  store i64 %1, i64* %2, align 8, !dbg !143
  %3 = load i64, i64* %0, align 8, !dbg !145
  %4 = load i64, i64* %2, align 8, !dbg !145
  %5 = add nsw i64 %3, %4, !dbg !145
  call void @llvm.dbg.value(metadata i64 %5, metadata !142, metadata !DIExpression()), !dbg !145
  %6 = load i64, i64* %0, align 8, !dbg !145
  store i64 %5, i64* %0, align 8, !dbg !145
  ret void, !dbg !145
}

define i64 @"_CNW7builtin3opsEXi1+EmmL0L0"(i64, i64) !dbg !146 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !150, metadata !DIExpression()), !dbg !151
  call void @llvm.dbg.value(metadata i64 %0, metadata !150, metadata !DIExpression()), !dbg !151
  store i64 %0, i64* %3, align 8, !dbg !151
  call void @llvm.dbg.declare(metadata i64* %2, metadata !152, metadata !DIExpression()), !dbg !151
  call void @llvm.dbg.value(metadata i64 %1, metadata !152, metadata !DIExpression()), !dbg !151
  store i64 %1, i64* %2, align 8, !dbg !151
  %4 = load i64, i64* %3, align 8, !dbg !153
  %5 = load i64, i64* %2, align 8, !dbg !153
  %6 = add nuw i64 %4, %5, !dbg !153
  ret i64 %6, !dbg !153
}

define void @"_CNW7builtin3opsEXi2+=EmmL0L0"(i64*, i64) !dbg !154 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !158, metadata !DIExpression()), !dbg !159
  call void @llvm.dbg.declare(metadata i64* %2, metadata !160, metadata !DIExpression()), !dbg !159
  call void @llvm.dbg.value(metadata i64 %1, metadata !160, metadata !DIExpression()), !dbg !159
  store i64 %1, i64* %2, align 8, !dbg !159
  %3 = load i64, i64* %0, align 8, !dbg !161
  %4 = load i64, i64* %2, align 8, !dbg !161
  %5 = add nuw i64 %3, %4, !dbg !161
  call void @llvm.dbg.value(metadata i64 %5, metadata !158, metadata !DIExpression()), !dbg !161
  %6 = load i64, i64* %0, align 8, !dbg !161
  store i64 %5, i64* %0, align 8, !dbg !161
  ret void, !dbg !161
}

define i128 @"_CNW7builtin3opsEXi1+EnnL0L0"(i128, i128) !dbg !162 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !166, metadata !DIExpression()), !dbg !167
  call void @llvm.dbg.value(metadata i128 %0, metadata !166, metadata !DIExpression()), !dbg !167
  store i128 %0, i128* %3, align 16, !dbg !167
  call void @llvm.dbg.declare(metadata i128* %2, metadata !168, metadata !DIExpression()), !dbg !167
  call void @llvm.dbg.value(metadata i128 %1, metadata !168, metadata !DIExpression()), !dbg !167
  store i128 %1, i128* %2, align 16, !dbg !167
  %4 = load i128, i128* %3, align 16, !dbg !169
  %5 = load i128, i128* %2, align 16, !dbg !169
  %6 = add nsw i128 %4, %5, !dbg !169
  ret i128 %6, !dbg !169
}

define void @"_CNW7builtin3opsEXi2+=EnnL0L0"(i128*, i128) !dbg !170 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !174, metadata !DIExpression()), !dbg !175
  call void @llvm.dbg.declare(metadata i128* %2, metadata !176, metadata !DIExpression()), !dbg !175
  call void @llvm.dbg.value(metadata i128 %1, metadata !176, metadata !DIExpression()), !dbg !175
  store i128 %1, i128* %2, align 16, !dbg !175
  %3 = load i128, i128* %0, align 16, !dbg !177
  %4 = load i128, i128* %2, align 16, !dbg !177
  %5 = add nsw i128 %3, %4, !dbg !177
  call void @llvm.dbg.value(metadata i128 %5, metadata !174, metadata !DIExpression()), !dbg !177
  %6 = load i128, i128* %0, align 16, !dbg !177
  store i128 %5, i128* %0, align 16, !dbg !177
  ret void, !dbg !177
}

define i128 @"_CNW7builtin3opsEXi1+EooL0L0"(i128, i128) !dbg !178 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !182, metadata !DIExpression()), !dbg !183
  call void @llvm.dbg.value(metadata i128 %0, metadata !182, metadata !DIExpression()), !dbg !183
  store i128 %0, i128* %3, align 16, !dbg !183
  call void @llvm.dbg.declare(metadata i128* %2, metadata !184, metadata !DIExpression()), !dbg !183
  call void @llvm.dbg.value(metadata i128 %1, metadata !184, metadata !DIExpression()), !dbg !183
  store i128 %1, i128* %2, align 16, !dbg !183
  %4 = load i128, i128* %3, align 16, !dbg !185
  %5 = load i128, i128* %2, align 16, !dbg !185
  %6 = add nuw i128 %4, %5, !dbg !185
  ret i128 %6, !dbg !185
}

define void @"_CNW7builtin3opsEXi2+=EooL0L0"(i128*, i128) !dbg !186 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !190, metadata !DIExpression()), !dbg !191
  call void @llvm.dbg.declare(metadata i128* %2, metadata !192, metadata !DIExpression()), !dbg !191
  call void @llvm.dbg.value(metadata i128 %1, metadata !192, metadata !DIExpression()), !dbg !191
  store i128 %1, i128* %2, align 16, !dbg !191
  %3 = load i128, i128* %0, align 16, !dbg !193
  %4 = load i128, i128* %2, align 16, !dbg !193
  %5 = add nuw i128 %3, %4, !dbg !193
  call void @llvm.dbg.value(metadata i128 %5, metadata !190, metadata !DIExpression()), !dbg !193
  %6 = load i128, i128* %0, align 16, !dbg !193
  store i128 %5, i128* %0, align 16, !dbg !193
  ret void, !dbg !193
}

define i8 @_CNW7builtin3opsEXi1-EaaL0L0(i8, i8) !dbg !194 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !195, metadata !DIExpression()), !dbg !196
  call void @llvm.dbg.value(metadata i8 %0, metadata !195, metadata !DIExpression()), !dbg !196
  store i8 %0, i8* %3, align 1, !dbg !196
  call void @llvm.dbg.declare(metadata i8* %2, metadata !197, metadata !DIExpression()), !dbg !196
  call void @llvm.dbg.value(metadata i8 %1, metadata !197, metadata !DIExpression()), !dbg !196
  store i8 %1, i8* %2, align 1, !dbg !196
  %4 = load i8, i8* %3, align 1, !dbg !198
  %5 = load i8, i8* %2, align 1, !dbg !198
  %6 = sub nsw i8 %4, %5, !dbg !198
  ret i8 %6, !dbg !198
}

define void @"_CNW7builtin3opsEXi2-=EaaL0L0"(i8*, i8) !dbg !199 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !200, metadata !DIExpression()), !dbg !201
  call void @llvm.dbg.declare(metadata i8* %2, metadata !202, metadata !DIExpression()), !dbg !201
  call void @llvm.dbg.value(metadata i8 %1, metadata !202, metadata !DIExpression()), !dbg !201
  store i8 %1, i8* %2, align 1, !dbg !201
  %3 = load i8, i8* %0, align 1, !dbg !203
  %4 = load i8, i8* %2, align 1, !dbg !203
  %5 = sub nsw i8 %3, %4, !dbg !203
  call void @llvm.dbg.value(metadata i8 %5, metadata !200, metadata !DIExpression()), !dbg !203
  %6 = load i8, i8* %0, align 1, !dbg !203
  store i8 %5, i8* %0, align 1, !dbg !203
  ret void, !dbg !203
}

define i8 @_CNW7builtin3opsEXi1-EhhL0L0(i8, i8) !dbg !204 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !205, metadata !DIExpression()), !dbg !206
  call void @llvm.dbg.value(metadata i8 %0, metadata !205, metadata !DIExpression()), !dbg !206
  store i8 %0, i8* %3, align 1, !dbg !206
  call void @llvm.dbg.declare(metadata i8* %2, metadata !207, metadata !DIExpression()), !dbg !206
  call void @llvm.dbg.value(metadata i8 %1, metadata !207, metadata !DIExpression()), !dbg !206
  store i8 %1, i8* %2, align 1, !dbg !206
  %4 = load i8, i8* %3, align 1, !dbg !208
  %5 = load i8, i8* %2, align 1, !dbg !208
  %6 = sub nuw i8 %4, %5, !dbg !208
  ret i8 %6, !dbg !208
}

define void @"_CNW7builtin3opsEXi2-=EhhL0L0"(i8*, i8) !dbg !209 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !210, metadata !DIExpression()), !dbg !211
  call void @llvm.dbg.declare(metadata i8* %2, metadata !212, metadata !DIExpression()), !dbg !211
  call void @llvm.dbg.value(metadata i8 %1, metadata !212, metadata !DIExpression()), !dbg !211
  store i8 %1, i8* %2, align 1, !dbg !211
  %3 = load i8, i8* %0, align 1, !dbg !213
  %4 = load i8, i8* %2, align 1, !dbg !213
  %5 = sub nuw i8 %3, %4, !dbg !213
  call void @llvm.dbg.value(metadata i8 %5, metadata !210, metadata !DIExpression()), !dbg !213
  %6 = load i8, i8* %0, align 1, !dbg !213
  store i8 %5, i8* %0, align 1, !dbg !213
  ret void, !dbg !213
}

define i16 @_CNW7builtin3opsEXi1-EssL0L0(i16, i16) !dbg !214 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !215, metadata !DIExpression()), !dbg !216
  call void @llvm.dbg.value(metadata i16 %0, metadata !215, metadata !DIExpression()), !dbg !216
  store i16 %0, i16* %3, align 2, !dbg !216
  call void @llvm.dbg.declare(metadata i16* %2, metadata !217, metadata !DIExpression()), !dbg !216
  call void @llvm.dbg.value(metadata i16 %1, metadata !217, metadata !DIExpression()), !dbg !216
  store i16 %1, i16* %2, align 2, !dbg !216
  %4 = load i16, i16* %3, align 2, !dbg !218
  %5 = load i16, i16* %2, align 2, !dbg !218
  %6 = sub nsw i16 %4, %5, !dbg !218
  ret i16 %6, !dbg !218
}

define void @"_CNW7builtin3opsEXi2-=EssL0L0"(i16*, i16) !dbg !219 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !220, metadata !DIExpression()), !dbg !221
  call void @llvm.dbg.declare(metadata i16* %2, metadata !222, metadata !DIExpression()), !dbg !221
  call void @llvm.dbg.value(metadata i16 %1, metadata !222, metadata !DIExpression()), !dbg !221
  store i16 %1, i16* %2, align 2, !dbg !221
  %3 = load i16, i16* %0, align 2, !dbg !223
  %4 = load i16, i16* %2, align 2, !dbg !223
  %5 = sub nsw i16 %3, %4, !dbg !223
  call void @llvm.dbg.value(metadata i16 %5, metadata !220, metadata !DIExpression()), !dbg !223
  %6 = load i16, i16* %0, align 2, !dbg !223
  store i16 %5, i16* %0, align 2, !dbg !223
  ret void, !dbg !223
}

define i16 @_CNW7builtin3opsEXi1-EttL0L0(i16, i16) !dbg !224 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !225, metadata !DIExpression()), !dbg !226
  call void @llvm.dbg.value(metadata i16 %0, metadata !225, metadata !DIExpression()), !dbg !226
  store i16 %0, i16* %3, align 2, !dbg !226
  call void @llvm.dbg.declare(metadata i16* %2, metadata !227, metadata !DIExpression()), !dbg !226
  call void @llvm.dbg.value(metadata i16 %1, metadata !227, metadata !DIExpression()), !dbg !226
  store i16 %1, i16* %2, align 2, !dbg !226
  %4 = load i16, i16* %3, align 2, !dbg !228
  %5 = load i16, i16* %2, align 2, !dbg !228
  %6 = sub nuw i16 %4, %5, !dbg !228
  ret i16 %6, !dbg !228
}

define void @"_CNW7builtin3opsEXi2-=EttL0L0"(i16*, i16) !dbg !229 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !230, metadata !DIExpression()), !dbg !231
  call void @llvm.dbg.declare(metadata i16* %2, metadata !232, metadata !DIExpression()), !dbg !231
  call void @llvm.dbg.value(metadata i16 %1, metadata !232, metadata !DIExpression()), !dbg !231
  store i16 %1, i16* %2, align 2, !dbg !231
  %3 = load i16, i16* %0, align 2, !dbg !233
  %4 = load i16, i16* %2, align 2, !dbg !233
  %5 = sub nuw i16 %3, %4, !dbg !233
  call void @llvm.dbg.value(metadata i16 %5, metadata !230, metadata !DIExpression()), !dbg !233
  %6 = load i16, i16* %0, align 2, !dbg !233
  store i16 %5, i16* %0, align 2, !dbg !233
  ret void, !dbg !233
}

define i32 @_CNW7builtin3opsEXi1-EiiL0L0(i32, i32) !dbg !234 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !235, metadata !DIExpression()), !dbg !236
  call void @llvm.dbg.value(metadata i32 %0, metadata !235, metadata !DIExpression()), !dbg !236
  store i32 %0, i32* %3, align 4, !dbg !236
  call void @llvm.dbg.declare(metadata i32* %2, metadata !237, metadata !DIExpression()), !dbg !236
  call void @llvm.dbg.value(metadata i32 %1, metadata !237, metadata !DIExpression()), !dbg !236
  store i32 %1, i32* %2, align 4, !dbg !236
  %4 = load i32, i32* %3, align 4, !dbg !238
  %5 = load i32, i32* %2, align 4, !dbg !238
  %6 = sub nsw i32 %4, %5, !dbg !238
  ret i32 %6, !dbg !238
}

define void @"_CNW7builtin3opsEXi2-=EiiL0L0"(i32*, i32) !dbg !239 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !240, metadata !DIExpression()), !dbg !241
  call void @llvm.dbg.declare(metadata i32* %2, metadata !242, metadata !DIExpression()), !dbg !241
  call void @llvm.dbg.value(metadata i32 %1, metadata !242, metadata !DIExpression()), !dbg !241
  store i32 %1, i32* %2, align 4, !dbg !241
  %3 = load i32, i32* %0, align 4, !dbg !243
  %4 = load i32, i32* %2, align 4, !dbg !243
  %5 = sub nsw i32 %3, %4, !dbg !243
  call void @llvm.dbg.value(metadata i32 %5, metadata !240, metadata !DIExpression()), !dbg !243
  %6 = load i32, i32* %0, align 4, !dbg !243
  store i32 %5, i32* %0, align 4, !dbg !243
  ret void, !dbg !243
}

define i32 @_CNW7builtin3opsEXi1-EjjL0L0(i32, i32) !dbg !244 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !245, metadata !DIExpression()), !dbg !246
  call void @llvm.dbg.value(metadata i32 %0, metadata !245, metadata !DIExpression()), !dbg !246
  store i32 %0, i32* %3, align 4, !dbg !246
  call void @llvm.dbg.declare(metadata i32* %2, metadata !247, metadata !DIExpression()), !dbg !246
  call void @llvm.dbg.value(metadata i32 %1, metadata !247, metadata !DIExpression()), !dbg !246
  store i32 %1, i32* %2, align 4, !dbg !246
  %4 = load i32, i32* %3, align 4, !dbg !248
  %5 = load i32, i32* %2, align 4, !dbg !248
  %6 = sub nuw i32 %4, %5, !dbg !248
  ret i32 %6, !dbg !248
}

define void @"_CNW7builtin3opsEXi2-=EjjL0L0"(i32*, i32) !dbg !249 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !250, metadata !DIExpression()), !dbg !251
  call void @llvm.dbg.declare(metadata i32* %2, metadata !252, metadata !DIExpression()), !dbg !251
  call void @llvm.dbg.value(metadata i32 %1, metadata !252, metadata !DIExpression()), !dbg !251
  store i32 %1, i32* %2, align 4, !dbg !251
  %3 = load i32, i32* %0, align 4, !dbg !253
  %4 = load i32, i32* %2, align 4, !dbg !253
  %5 = sub nuw i32 %3, %4, !dbg !253
  call void @llvm.dbg.value(metadata i32 %5, metadata !250, metadata !DIExpression()), !dbg !253
  %6 = load i32, i32* %0, align 4, !dbg !253
  store i32 %5, i32* %0, align 4, !dbg !253
  ret void, !dbg !253
}

define i64 @_CNW7builtin3opsEXi1-EllL0L0(i64, i64) !dbg !254 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !255, metadata !DIExpression()), !dbg !256
  call void @llvm.dbg.value(metadata i64 %0, metadata !255, metadata !DIExpression()), !dbg !256
  store i64 %0, i64* %3, align 8, !dbg !256
  call void @llvm.dbg.declare(metadata i64* %2, metadata !257, metadata !DIExpression()), !dbg !256
  call void @llvm.dbg.value(metadata i64 %1, metadata !257, metadata !DIExpression()), !dbg !256
  store i64 %1, i64* %2, align 8, !dbg !256
  %4 = load i64, i64* %3, align 8, !dbg !258
  %5 = load i64, i64* %2, align 8, !dbg !258
  %6 = sub nsw i64 %4, %5, !dbg !258
  ret i64 %6, !dbg !258
}

define void @"_CNW7builtin3opsEXi2-=EllL0L0"(i64*, i64) !dbg !259 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !260, metadata !DIExpression()), !dbg !261
  call void @llvm.dbg.declare(metadata i64* %2, metadata !262, metadata !DIExpression()), !dbg !261
  call void @llvm.dbg.value(metadata i64 %1, metadata !262, metadata !DIExpression()), !dbg !261
  store i64 %1, i64* %2, align 8, !dbg !261
  %3 = load i64, i64* %0, align 8, !dbg !263
  %4 = load i64, i64* %2, align 8, !dbg !263
  %5 = sub nsw i64 %3, %4, !dbg !263
  call void @llvm.dbg.value(metadata i64 %5, metadata !260, metadata !DIExpression()), !dbg !263
  %6 = load i64, i64* %0, align 8, !dbg !263
  store i64 %5, i64* %0, align 8, !dbg !263
  ret void, !dbg !263
}

define i64 @_CNW7builtin3opsEXi1-EmmL0L0(i64, i64) !dbg !264 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !265, metadata !DIExpression()), !dbg !266
  call void @llvm.dbg.value(metadata i64 %0, metadata !265, metadata !DIExpression()), !dbg !266
  store i64 %0, i64* %3, align 8, !dbg !266
  call void @llvm.dbg.declare(metadata i64* %2, metadata !267, metadata !DIExpression()), !dbg !266
  call void @llvm.dbg.value(metadata i64 %1, metadata !267, metadata !DIExpression()), !dbg !266
  store i64 %1, i64* %2, align 8, !dbg !266
  %4 = load i64, i64* %3, align 8, !dbg !268
  %5 = load i64, i64* %2, align 8, !dbg !268
  %6 = sub nuw i64 %4, %5, !dbg !268
  ret i64 %6, !dbg !268
}

define void @"_CNW7builtin3opsEXi2-=EmmL0L0"(i64*, i64) !dbg !269 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !270, metadata !DIExpression()), !dbg !271
  call void @llvm.dbg.declare(metadata i64* %2, metadata !272, metadata !DIExpression()), !dbg !271
  call void @llvm.dbg.value(metadata i64 %1, metadata !272, metadata !DIExpression()), !dbg !271
  store i64 %1, i64* %2, align 8, !dbg !271
  %3 = load i64, i64* %0, align 8, !dbg !273
  %4 = load i64, i64* %2, align 8, !dbg !273
  %5 = sub nuw i64 %3, %4, !dbg !273
  call void @llvm.dbg.value(metadata i64 %5, metadata !270, metadata !DIExpression()), !dbg !273
  %6 = load i64, i64* %0, align 8, !dbg !273
  store i64 %5, i64* %0, align 8, !dbg !273
  ret void, !dbg !273
}

define i128 @_CNW7builtin3opsEXi1-EnnL0L0(i128, i128) !dbg !274 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !275, metadata !DIExpression()), !dbg !276
  call void @llvm.dbg.value(metadata i128 %0, metadata !275, metadata !DIExpression()), !dbg !276
  store i128 %0, i128* %3, align 16, !dbg !276
  call void @llvm.dbg.declare(metadata i128* %2, metadata !277, metadata !DIExpression()), !dbg !276
  call void @llvm.dbg.value(metadata i128 %1, metadata !277, metadata !DIExpression()), !dbg !276
  store i128 %1, i128* %2, align 16, !dbg !276
  %4 = load i128, i128* %3, align 16, !dbg !278
  %5 = load i128, i128* %2, align 16, !dbg !278
  %6 = sub nsw i128 %4, %5, !dbg !278
  ret i128 %6, !dbg !278
}

define void @"_CNW7builtin3opsEXi2-=EnnL0L0"(i128*, i128) !dbg !279 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !280, metadata !DIExpression()), !dbg !281
  call void @llvm.dbg.declare(metadata i128* %2, metadata !282, metadata !DIExpression()), !dbg !281
  call void @llvm.dbg.value(metadata i128 %1, metadata !282, metadata !DIExpression()), !dbg !281
  store i128 %1, i128* %2, align 16, !dbg !281
  %3 = load i128, i128* %0, align 16, !dbg !283
  %4 = load i128, i128* %2, align 16, !dbg !283
  %5 = sub nsw i128 %3, %4, !dbg !283
  call void @llvm.dbg.value(metadata i128 %5, metadata !280, metadata !DIExpression()), !dbg !283
  %6 = load i128, i128* %0, align 16, !dbg !283
  store i128 %5, i128* %0, align 16, !dbg !283
  ret void, !dbg !283
}

define i128 @_CNW7builtin3opsEXi1-EooL0L0(i128, i128) !dbg !284 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !285, metadata !DIExpression()), !dbg !286
  call void @llvm.dbg.value(metadata i128 %0, metadata !285, metadata !DIExpression()), !dbg !286
  store i128 %0, i128* %3, align 16, !dbg !286
  call void @llvm.dbg.declare(metadata i128* %2, metadata !287, metadata !DIExpression()), !dbg !286
  call void @llvm.dbg.value(metadata i128 %1, metadata !287, metadata !DIExpression()), !dbg !286
  store i128 %1, i128* %2, align 16, !dbg !286
  %4 = load i128, i128* %3, align 16, !dbg !288
  %5 = load i128, i128* %2, align 16, !dbg !288
  %6 = sub nuw i128 %4, %5, !dbg !288
  ret i128 %6, !dbg !288
}

define void @"_CNW7builtin3opsEXi2-=EooL0L0"(i128*, i128) !dbg !289 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !290, metadata !DIExpression()), !dbg !291
  call void @llvm.dbg.declare(metadata i128* %2, metadata !292, metadata !DIExpression()), !dbg !291
  call void @llvm.dbg.value(metadata i128 %1, metadata !292, metadata !DIExpression()), !dbg !291
  store i128 %1, i128* %2, align 16, !dbg !291
  %3 = load i128, i128* %0, align 16, !dbg !293
  %4 = load i128, i128* %2, align 16, !dbg !293
  %5 = sub nuw i128 %3, %4, !dbg !293
  call void @llvm.dbg.value(metadata i128 %5, metadata !290, metadata !DIExpression()), !dbg !293
  %6 = load i128, i128* %0, align 16, !dbg !293
  store i128 %5, i128* %0, align 16, !dbg !293
  ret void, !dbg !293
}

define i8 @"_CNW7builtin3opsEXi1*EaaL0L0"(i8, i8) !dbg !294 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !295, metadata !DIExpression()), !dbg !296
  call void @llvm.dbg.value(metadata i8 %0, metadata !295, metadata !DIExpression()), !dbg !296
  store i8 %0, i8* %3, align 1, !dbg !296
  call void @llvm.dbg.declare(metadata i8* %2, metadata !297, metadata !DIExpression()), !dbg !296
  call void @llvm.dbg.value(metadata i8 %1, metadata !297, metadata !DIExpression()), !dbg !296
  store i8 %1, i8* %2, align 1, !dbg !296
  %4 = load i8, i8* %3, align 1, !dbg !298
  %5 = load i8, i8* %2, align 1, !dbg !298
  %6 = mul nsw i8 %4, %5, !dbg !298
  ret i8 %6, !dbg !298
}

define void @"_CNW7builtin3opsEXi2*=EaaL0L0"(i8*, i8) !dbg !299 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !300, metadata !DIExpression()), !dbg !301
  call void @llvm.dbg.declare(metadata i8* %2, metadata !302, metadata !DIExpression()), !dbg !301
  call void @llvm.dbg.value(metadata i8 %1, metadata !302, metadata !DIExpression()), !dbg !301
  store i8 %1, i8* %2, align 1, !dbg !301
  %3 = load i8, i8* %0, align 1, !dbg !303
  %4 = load i8, i8* %2, align 1, !dbg !303
  %5 = mul nsw i8 %3, %4, !dbg !303
  call void @llvm.dbg.value(metadata i8 %5, metadata !300, metadata !DIExpression()), !dbg !303
  %6 = load i8, i8* %0, align 1, !dbg !303
  store i8 %5, i8* %0, align 1, !dbg !303
  ret void, !dbg !303
}

define i8 @"_CNW7builtin3opsEXi1*EhhL0L0"(i8, i8) !dbg !304 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !305, metadata !DIExpression()), !dbg !306
  call void @llvm.dbg.value(metadata i8 %0, metadata !305, metadata !DIExpression()), !dbg !306
  store i8 %0, i8* %3, align 1, !dbg !306
  call void @llvm.dbg.declare(metadata i8* %2, metadata !307, metadata !DIExpression()), !dbg !306
  call void @llvm.dbg.value(metadata i8 %1, metadata !307, metadata !DIExpression()), !dbg !306
  store i8 %1, i8* %2, align 1, !dbg !306
  %4 = load i8, i8* %3, align 1, !dbg !308
  %5 = load i8, i8* %2, align 1, !dbg !308
  %6 = mul nuw i8 %4, %5, !dbg !308
  ret i8 %6, !dbg !308
}

define void @"_CNW7builtin3opsEXi2*=EhhL0L0"(i8*, i8) !dbg !309 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !310, metadata !DIExpression()), !dbg !311
  call void @llvm.dbg.declare(metadata i8* %2, metadata !312, metadata !DIExpression()), !dbg !311
  call void @llvm.dbg.value(metadata i8 %1, metadata !312, metadata !DIExpression()), !dbg !311
  store i8 %1, i8* %2, align 1, !dbg !311
  %3 = load i8, i8* %0, align 1, !dbg !313
  %4 = load i8, i8* %2, align 1, !dbg !313
  %5 = mul nuw i8 %3, %4, !dbg !313
  call void @llvm.dbg.value(metadata i8 %5, metadata !310, metadata !DIExpression()), !dbg !313
  %6 = load i8, i8* %0, align 1, !dbg !313
  store i8 %5, i8* %0, align 1, !dbg !313
  ret void, !dbg !313
}

define i16 @"_CNW7builtin3opsEXi1*EssL0L0"(i16, i16) !dbg !314 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !315, metadata !DIExpression()), !dbg !316
  call void @llvm.dbg.value(metadata i16 %0, metadata !315, metadata !DIExpression()), !dbg !316
  store i16 %0, i16* %3, align 2, !dbg !316
  call void @llvm.dbg.declare(metadata i16* %2, metadata !317, metadata !DIExpression()), !dbg !316
  call void @llvm.dbg.value(metadata i16 %1, metadata !317, metadata !DIExpression()), !dbg !316
  store i16 %1, i16* %2, align 2, !dbg !316
  %4 = load i16, i16* %3, align 2, !dbg !318
  %5 = load i16, i16* %2, align 2, !dbg !318
  %6 = mul nsw i16 %4, %5, !dbg !318
  ret i16 %6, !dbg !318
}

define void @"_CNW7builtin3opsEXi2*=EssL0L0"(i16*, i16) !dbg !319 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !320, metadata !DIExpression()), !dbg !321
  call void @llvm.dbg.declare(metadata i16* %2, metadata !322, metadata !DIExpression()), !dbg !321
  call void @llvm.dbg.value(metadata i16 %1, metadata !322, metadata !DIExpression()), !dbg !321
  store i16 %1, i16* %2, align 2, !dbg !321
  %3 = load i16, i16* %0, align 2, !dbg !323
  %4 = load i16, i16* %2, align 2, !dbg !323
  %5 = mul nsw i16 %3, %4, !dbg !323
  call void @llvm.dbg.value(metadata i16 %5, metadata !320, metadata !DIExpression()), !dbg !323
  %6 = load i16, i16* %0, align 2, !dbg !323
  store i16 %5, i16* %0, align 2, !dbg !323
  ret void, !dbg !323
}

define i16 @"_CNW7builtin3opsEXi1*EttL0L0"(i16, i16) !dbg !324 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !325, metadata !DIExpression()), !dbg !326
  call void @llvm.dbg.value(metadata i16 %0, metadata !325, metadata !DIExpression()), !dbg !326
  store i16 %0, i16* %3, align 2, !dbg !326
  call void @llvm.dbg.declare(metadata i16* %2, metadata !327, metadata !DIExpression()), !dbg !326
  call void @llvm.dbg.value(metadata i16 %1, metadata !327, metadata !DIExpression()), !dbg !326
  store i16 %1, i16* %2, align 2, !dbg !326
  %4 = load i16, i16* %3, align 2, !dbg !328
  %5 = load i16, i16* %2, align 2, !dbg !328
  %6 = mul nuw i16 %4, %5, !dbg !328
  ret i16 %6, !dbg !328
}

define void @"_CNW7builtin3opsEXi2*=EttL0L0"(i16*, i16) !dbg !329 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !330, metadata !DIExpression()), !dbg !331
  call void @llvm.dbg.declare(metadata i16* %2, metadata !332, metadata !DIExpression()), !dbg !331
  call void @llvm.dbg.value(metadata i16 %1, metadata !332, metadata !DIExpression()), !dbg !331
  store i16 %1, i16* %2, align 2, !dbg !331
  %3 = load i16, i16* %0, align 2, !dbg !333
  %4 = load i16, i16* %2, align 2, !dbg !333
  %5 = mul nuw i16 %3, %4, !dbg !333
  call void @llvm.dbg.value(metadata i16 %5, metadata !330, metadata !DIExpression()), !dbg !333
  %6 = load i16, i16* %0, align 2, !dbg !333
  store i16 %5, i16* %0, align 2, !dbg !333
  ret void, !dbg !333
}

define i32 @"_CNW7builtin3opsEXi1*EiiL0L0"(i32, i32) !dbg !334 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !335, metadata !DIExpression()), !dbg !336
  call void @llvm.dbg.value(metadata i32 %0, metadata !335, metadata !DIExpression()), !dbg !336
  store i32 %0, i32* %3, align 4, !dbg !336
  call void @llvm.dbg.declare(metadata i32* %2, metadata !337, metadata !DIExpression()), !dbg !336
  call void @llvm.dbg.value(metadata i32 %1, metadata !337, metadata !DIExpression()), !dbg !336
  store i32 %1, i32* %2, align 4, !dbg !336
  %4 = load i32, i32* %3, align 4, !dbg !338
  %5 = load i32, i32* %2, align 4, !dbg !338
  %6 = mul nsw i32 %4, %5, !dbg !338
  ret i32 %6, !dbg !338
}

define void @"_CNW7builtin3opsEXi2*=EiiL0L0"(i32*, i32) !dbg !339 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !340, metadata !DIExpression()), !dbg !341
  call void @llvm.dbg.declare(metadata i32* %2, metadata !342, metadata !DIExpression()), !dbg !341
  call void @llvm.dbg.value(metadata i32 %1, metadata !342, metadata !DIExpression()), !dbg !341
  store i32 %1, i32* %2, align 4, !dbg !341
  %3 = load i32, i32* %0, align 4, !dbg !343
  %4 = load i32, i32* %2, align 4, !dbg !343
  %5 = mul nsw i32 %3, %4, !dbg !343
  call void @llvm.dbg.value(metadata i32 %5, metadata !340, metadata !DIExpression()), !dbg !343
  %6 = load i32, i32* %0, align 4, !dbg !343
  store i32 %5, i32* %0, align 4, !dbg !343
  ret void, !dbg !343
}

define i32 @"_CNW7builtin3opsEXi1*EjjL0L0"(i32, i32) !dbg !344 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !345, metadata !DIExpression()), !dbg !346
  call void @llvm.dbg.value(metadata i32 %0, metadata !345, metadata !DIExpression()), !dbg !346
  store i32 %0, i32* %3, align 4, !dbg !346
  call void @llvm.dbg.declare(metadata i32* %2, metadata !347, metadata !DIExpression()), !dbg !346
  call void @llvm.dbg.value(metadata i32 %1, metadata !347, metadata !DIExpression()), !dbg !346
  store i32 %1, i32* %2, align 4, !dbg !346
  %4 = load i32, i32* %3, align 4, !dbg !348
  %5 = load i32, i32* %2, align 4, !dbg !348
  %6 = mul nuw i32 %4, %5, !dbg !348
  ret i32 %6, !dbg !348
}

define void @"_CNW7builtin3opsEXi2*=EjjL0L0"(i32*, i32) !dbg !349 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !350, metadata !DIExpression()), !dbg !351
  call void @llvm.dbg.declare(metadata i32* %2, metadata !352, metadata !DIExpression()), !dbg !351
  call void @llvm.dbg.value(metadata i32 %1, metadata !352, metadata !DIExpression()), !dbg !351
  store i32 %1, i32* %2, align 4, !dbg !351
  %3 = load i32, i32* %0, align 4, !dbg !353
  %4 = load i32, i32* %2, align 4, !dbg !353
  %5 = mul nuw i32 %3, %4, !dbg !353
  call void @llvm.dbg.value(metadata i32 %5, metadata !350, metadata !DIExpression()), !dbg !353
  %6 = load i32, i32* %0, align 4, !dbg !353
  store i32 %5, i32* %0, align 4, !dbg !353
  ret void, !dbg !353
}

define i64 @"_CNW7builtin3opsEXi1*EllL0L0"(i64, i64) !dbg !354 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !355, metadata !DIExpression()), !dbg !356
  call void @llvm.dbg.value(metadata i64 %0, metadata !355, metadata !DIExpression()), !dbg !356
  store i64 %0, i64* %3, align 8, !dbg !356
  call void @llvm.dbg.declare(metadata i64* %2, metadata !357, metadata !DIExpression()), !dbg !356
  call void @llvm.dbg.value(metadata i64 %1, metadata !357, metadata !DIExpression()), !dbg !356
  store i64 %1, i64* %2, align 8, !dbg !356
  %4 = load i64, i64* %3, align 8, !dbg !358
  %5 = load i64, i64* %2, align 8, !dbg !358
  %6 = mul nsw i64 %4, %5, !dbg !358
  ret i64 %6, !dbg !358
}

define void @"_CNW7builtin3opsEXi2*=EllL0L0"(i64*, i64) !dbg !359 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !360, metadata !DIExpression()), !dbg !361
  call void @llvm.dbg.declare(metadata i64* %2, metadata !362, metadata !DIExpression()), !dbg !361
  call void @llvm.dbg.value(metadata i64 %1, metadata !362, metadata !DIExpression()), !dbg !361
  store i64 %1, i64* %2, align 8, !dbg !361
  %3 = load i64, i64* %0, align 8, !dbg !363
  %4 = load i64, i64* %2, align 8, !dbg !363
  %5 = mul nsw i64 %3, %4, !dbg !363
  call void @llvm.dbg.value(metadata i64 %5, metadata !360, metadata !DIExpression()), !dbg !363
  %6 = load i64, i64* %0, align 8, !dbg !363
  store i64 %5, i64* %0, align 8, !dbg !363
  ret void, !dbg !363
}

define i64 @"_CNW7builtin3opsEXi1*EmmL0L0"(i64, i64) !dbg !364 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !365, metadata !DIExpression()), !dbg !366
  call void @llvm.dbg.value(metadata i64 %0, metadata !365, metadata !DIExpression()), !dbg !366
  store i64 %0, i64* %3, align 8, !dbg !366
  call void @llvm.dbg.declare(metadata i64* %2, metadata !367, metadata !DIExpression()), !dbg !366
  call void @llvm.dbg.value(metadata i64 %1, metadata !367, metadata !DIExpression()), !dbg !366
  store i64 %1, i64* %2, align 8, !dbg !366
  %4 = load i64, i64* %3, align 8, !dbg !368
  %5 = load i64, i64* %2, align 8, !dbg !368
  %6 = mul nuw i64 %4, %5, !dbg !368
  ret i64 %6, !dbg !368
}

define void @"_CNW7builtin3opsEXi2*=EmmL0L0"(i64*, i64) !dbg !369 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !370, metadata !DIExpression()), !dbg !371
  call void @llvm.dbg.declare(metadata i64* %2, metadata !372, metadata !DIExpression()), !dbg !371
  call void @llvm.dbg.value(metadata i64 %1, metadata !372, metadata !DIExpression()), !dbg !371
  store i64 %1, i64* %2, align 8, !dbg !371
  %3 = load i64, i64* %0, align 8, !dbg !373
  %4 = load i64, i64* %2, align 8, !dbg !373
  %5 = mul nuw i64 %3, %4, !dbg !373
  call void @llvm.dbg.value(metadata i64 %5, metadata !370, metadata !DIExpression()), !dbg !373
  %6 = load i64, i64* %0, align 8, !dbg !373
  store i64 %5, i64* %0, align 8, !dbg !373
  ret void, !dbg !373
}

define i128 @"_CNW7builtin3opsEXi1*EnnL0L0"(i128, i128) !dbg !374 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !375, metadata !DIExpression()), !dbg !376
  call void @llvm.dbg.value(metadata i128 %0, metadata !375, metadata !DIExpression()), !dbg !376
  store i128 %0, i128* %3, align 16, !dbg !376
  call void @llvm.dbg.declare(metadata i128* %2, metadata !377, metadata !DIExpression()), !dbg !376
  call void @llvm.dbg.value(metadata i128 %1, metadata !377, metadata !DIExpression()), !dbg !376
  store i128 %1, i128* %2, align 16, !dbg !376
  %4 = load i128, i128* %3, align 16, !dbg !378
  %5 = load i128, i128* %2, align 16, !dbg !378
  %6 = mul nsw i128 %4, %5, !dbg !378
  ret i128 %6, !dbg !378
}

define void @"_CNW7builtin3opsEXi2*=EnnL0L0"(i128*, i128) !dbg !379 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !380, metadata !DIExpression()), !dbg !381
  call void @llvm.dbg.declare(metadata i128* %2, metadata !382, metadata !DIExpression()), !dbg !381
  call void @llvm.dbg.value(metadata i128 %1, metadata !382, metadata !DIExpression()), !dbg !381
  store i128 %1, i128* %2, align 16, !dbg !381
  %3 = load i128, i128* %0, align 16, !dbg !383
  %4 = load i128, i128* %2, align 16, !dbg !383
  %5 = mul nsw i128 %3, %4, !dbg !383
  call void @llvm.dbg.value(metadata i128 %5, metadata !380, metadata !DIExpression()), !dbg !383
  %6 = load i128, i128* %0, align 16, !dbg !383
  store i128 %5, i128* %0, align 16, !dbg !383
  ret void, !dbg !383
}

define i128 @"_CNW7builtin3opsEXi1*EooL0L0"(i128, i128) !dbg !384 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !385, metadata !DIExpression()), !dbg !386
  call void @llvm.dbg.value(metadata i128 %0, metadata !385, metadata !DIExpression()), !dbg !386
  store i128 %0, i128* %3, align 16, !dbg !386
  call void @llvm.dbg.declare(metadata i128* %2, metadata !387, metadata !DIExpression()), !dbg !386
  call void @llvm.dbg.value(metadata i128 %1, metadata !387, metadata !DIExpression()), !dbg !386
  store i128 %1, i128* %2, align 16, !dbg !386
  %4 = load i128, i128* %3, align 16, !dbg !388
  %5 = load i128, i128* %2, align 16, !dbg !388
  %6 = mul nuw i128 %4, %5, !dbg !388
  ret i128 %6, !dbg !388
}

define void @"_CNW7builtin3opsEXi2*=EooL0L0"(i128*, i128) !dbg !389 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !390, metadata !DIExpression()), !dbg !391
  call void @llvm.dbg.declare(metadata i128* %2, metadata !392, metadata !DIExpression()), !dbg !391
  call void @llvm.dbg.value(metadata i128 %1, metadata !392, metadata !DIExpression()), !dbg !391
  store i128 %1, i128* %2, align 16, !dbg !391
  %3 = load i128, i128* %0, align 16, !dbg !393
  %4 = load i128, i128* %2, align 16, !dbg !393
  %5 = mul nuw i128 %3, %4, !dbg !393
  call void @llvm.dbg.value(metadata i128 %5, metadata !390, metadata !DIExpression()), !dbg !393
  %6 = load i128, i128* %0, align 16, !dbg !393
  store i128 %5, i128* %0, align 16, !dbg !393
  ret void, !dbg !393
}

define i8 @"_CNW7builtin3opsEXi1/EaaL0L0"(i8, i8) !dbg !394 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !395, metadata !DIExpression()), !dbg !396
  call void @llvm.dbg.value(metadata i8 %0, metadata !395, metadata !DIExpression()), !dbg !396
  store i8 %0, i8* %3, align 1, !dbg !396
  call void @llvm.dbg.declare(metadata i8* %2, metadata !397, metadata !DIExpression()), !dbg !396
  call void @llvm.dbg.value(metadata i8 %1, metadata !397, metadata !DIExpression()), !dbg !396
  store i8 %1, i8* %2, align 1, !dbg !396
  %4 = load i8, i8* %3, align 1, !dbg !398
  %5 = load i8, i8* %2, align 1, !dbg !398
  %6 = sdiv i8 %4, %5, !dbg !398
  ret i8 %6, !dbg !398
}

define void @"_CNW7builtin3opsEXi2/=EaaL0L0"(i8*, i8) !dbg !399 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !400, metadata !DIExpression()), !dbg !401
  call void @llvm.dbg.declare(metadata i8* %2, metadata !402, metadata !DIExpression()), !dbg !401
  call void @llvm.dbg.value(metadata i8 %1, metadata !402, metadata !DIExpression()), !dbg !401
  store i8 %1, i8* %2, align 1, !dbg !401
  %3 = load i8, i8* %0, align 1, !dbg !403
  %4 = load i8, i8* %2, align 1, !dbg !403
  %5 = sdiv i8 %3, %4, !dbg !403
  call void @llvm.dbg.value(metadata i8 %5, metadata !400, metadata !DIExpression()), !dbg !403
  %6 = load i8, i8* %0, align 1, !dbg !403
  store i8 %5, i8* %0, align 1, !dbg !403
  ret void, !dbg !403
}

define i8 @"_CNW7builtin3opsEXi1/EhhL0L0"(i8, i8) !dbg !404 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !405, metadata !DIExpression()), !dbg !406
  call void @llvm.dbg.value(metadata i8 %0, metadata !405, metadata !DIExpression()), !dbg !406
  store i8 %0, i8* %3, align 1, !dbg !406
  call void @llvm.dbg.declare(metadata i8* %2, metadata !407, metadata !DIExpression()), !dbg !406
  call void @llvm.dbg.value(metadata i8 %1, metadata !407, metadata !DIExpression()), !dbg !406
  store i8 %1, i8* %2, align 1, !dbg !406
  %4 = load i8, i8* %3, align 1, !dbg !408
  %5 = load i8, i8* %2, align 1, !dbg !408
  %6 = udiv i8 %4, %5, !dbg !408
  ret i8 %6, !dbg !408
}

define void @"_CNW7builtin3opsEXi2/=EhhL0L0"(i8*, i8) !dbg !409 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !410, metadata !DIExpression()), !dbg !411
  call void @llvm.dbg.declare(metadata i8* %2, metadata !412, metadata !DIExpression()), !dbg !411
  call void @llvm.dbg.value(metadata i8 %1, metadata !412, metadata !DIExpression()), !dbg !411
  store i8 %1, i8* %2, align 1, !dbg !411
  %3 = load i8, i8* %0, align 1, !dbg !413
  %4 = load i8, i8* %2, align 1, !dbg !413
  %5 = udiv i8 %3, %4, !dbg !413
  call void @llvm.dbg.value(metadata i8 %5, metadata !410, metadata !DIExpression()), !dbg !413
  %6 = load i8, i8* %0, align 1, !dbg !413
  store i8 %5, i8* %0, align 1, !dbg !413
  ret void, !dbg !413
}

define i16 @"_CNW7builtin3opsEXi1/EssL0L0"(i16, i16) !dbg !414 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !415, metadata !DIExpression()), !dbg !416
  call void @llvm.dbg.value(metadata i16 %0, metadata !415, metadata !DIExpression()), !dbg !416
  store i16 %0, i16* %3, align 2, !dbg !416
  call void @llvm.dbg.declare(metadata i16* %2, metadata !417, metadata !DIExpression()), !dbg !416
  call void @llvm.dbg.value(metadata i16 %1, metadata !417, metadata !DIExpression()), !dbg !416
  store i16 %1, i16* %2, align 2, !dbg !416
  %4 = load i16, i16* %3, align 2, !dbg !418
  %5 = load i16, i16* %2, align 2, !dbg !418
  %6 = sdiv i16 %4, %5, !dbg !418
  ret i16 %6, !dbg !418
}

define void @"_CNW7builtin3opsEXi2/=EssL0L0"(i16*, i16) !dbg !419 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !420, metadata !DIExpression()), !dbg !421
  call void @llvm.dbg.declare(metadata i16* %2, metadata !422, metadata !DIExpression()), !dbg !421
  call void @llvm.dbg.value(metadata i16 %1, metadata !422, metadata !DIExpression()), !dbg !421
  store i16 %1, i16* %2, align 2, !dbg !421
  %3 = load i16, i16* %0, align 2, !dbg !423
  %4 = load i16, i16* %2, align 2, !dbg !423
  %5 = sdiv i16 %3, %4, !dbg !423
  call void @llvm.dbg.value(metadata i16 %5, metadata !420, metadata !DIExpression()), !dbg !423
  %6 = load i16, i16* %0, align 2, !dbg !423
  store i16 %5, i16* %0, align 2, !dbg !423
  ret void, !dbg !423
}

define i16 @"_CNW7builtin3opsEXi1/EttL0L0"(i16, i16) !dbg !424 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !425, metadata !DIExpression()), !dbg !426
  call void @llvm.dbg.value(metadata i16 %0, metadata !425, metadata !DIExpression()), !dbg !426
  store i16 %0, i16* %3, align 2, !dbg !426
  call void @llvm.dbg.declare(metadata i16* %2, metadata !427, metadata !DIExpression()), !dbg !426
  call void @llvm.dbg.value(metadata i16 %1, metadata !427, metadata !DIExpression()), !dbg !426
  store i16 %1, i16* %2, align 2, !dbg !426
  %4 = load i16, i16* %3, align 2, !dbg !428
  %5 = load i16, i16* %2, align 2, !dbg !428
  %6 = udiv i16 %4, %5, !dbg !428
  ret i16 %6, !dbg !428
}

define void @"_CNW7builtin3opsEXi2/=EttL0L0"(i16*, i16) !dbg !429 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !430, metadata !DIExpression()), !dbg !431
  call void @llvm.dbg.declare(metadata i16* %2, metadata !432, metadata !DIExpression()), !dbg !431
  call void @llvm.dbg.value(metadata i16 %1, metadata !432, metadata !DIExpression()), !dbg !431
  store i16 %1, i16* %2, align 2, !dbg !431
  %3 = load i16, i16* %0, align 2, !dbg !433
  %4 = load i16, i16* %2, align 2, !dbg !433
  %5 = udiv i16 %3, %4, !dbg !433
  call void @llvm.dbg.value(metadata i16 %5, metadata !430, metadata !DIExpression()), !dbg !433
  %6 = load i16, i16* %0, align 2, !dbg !433
  store i16 %5, i16* %0, align 2, !dbg !433
  ret void, !dbg !433
}

define i32 @"_CNW7builtin3opsEXi1/EiiL0L0"(i32, i32) !dbg !434 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !435, metadata !DIExpression()), !dbg !436
  call void @llvm.dbg.value(metadata i32 %0, metadata !435, metadata !DIExpression()), !dbg !436
  store i32 %0, i32* %3, align 4, !dbg !436
  call void @llvm.dbg.declare(metadata i32* %2, metadata !437, metadata !DIExpression()), !dbg !436
  call void @llvm.dbg.value(metadata i32 %1, metadata !437, metadata !DIExpression()), !dbg !436
  store i32 %1, i32* %2, align 4, !dbg !436
  %4 = load i32, i32* %3, align 4, !dbg !438
  %5 = load i32, i32* %2, align 4, !dbg !438
  %6 = sdiv i32 %4, %5, !dbg !438
  ret i32 %6, !dbg !438
}

define void @"_CNW7builtin3opsEXi2/=EiiL0L0"(i32*, i32) !dbg !439 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !440, metadata !DIExpression()), !dbg !441
  call void @llvm.dbg.declare(metadata i32* %2, metadata !442, metadata !DIExpression()), !dbg !441
  call void @llvm.dbg.value(metadata i32 %1, metadata !442, metadata !DIExpression()), !dbg !441
  store i32 %1, i32* %2, align 4, !dbg !441
  %3 = load i32, i32* %0, align 4, !dbg !443
  %4 = load i32, i32* %2, align 4, !dbg !443
  %5 = sdiv i32 %3, %4, !dbg !443
  call void @llvm.dbg.value(metadata i32 %5, metadata !440, metadata !DIExpression()), !dbg !443
  %6 = load i32, i32* %0, align 4, !dbg !443
  store i32 %5, i32* %0, align 4, !dbg !443
  ret void, !dbg !443
}

define i32 @"_CNW7builtin3opsEXi1/EjjL0L0"(i32, i32) !dbg !444 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !445, metadata !DIExpression()), !dbg !446
  call void @llvm.dbg.value(metadata i32 %0, metadata !445, metadata !DIExpression()), !dbg !446
  store i32 %0, i32* %3, align 4, !dbg !446
  call void @llvm.dbg.declare(metadata i32* %2, metadata !447, metadata !DIExpression()), !dbg !446
  call void @llvm.dbg.value(metadata i32 %1, metadata !447, metadata !DIExpression()), !dbg !446
  store i32 %1, i32* %2, align 4, !dbg !446
  %4 = load i32, i32* %3, align 4, !dbg !448
  %5 = load i32, i32* %2, align 4, !dbg !448
  %6 = udiv i32 %4, %5, !dbg !448
  ret i32 %6, !dbg !448
}

define void @"_CNW7builtin3opsEXi2/=EjjL0L0"(i32*, i32) !dbg !449 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !450, metadata !DIExpression()), !dbg !451
  call void @llvm.dbg.declare(metadata i32* %2, metadata !452, metadata !DIExpression()), !dbg !451
  call void @llvm.dbg.value(metadata i32 %1, metadata !452, metadata !DIExpression()), !dbg !451
  store i32 %1, i32* %2, align 4, !dbg !451
  %3 = load i32, i32* %0, align 4, !dbg !453
  %4 = load i32, i32* %2, align 4, !dbg !453
  %5 = udiv i32 %3, %4, !dbg !453
  call void @llvm.dbg.value(metadata i32 %5, metadata !450, metadata !DIExpression()), !dbg !453
  %6 = load i32, i32* %0, align 4, !dbg !453
  store i32 %5, i32* %0, align 4, !dbg !453
  ret void, !dbg !453
}

define i64 @"_CNW7builtin3opsEXi1/EllL0L0"(i64, i64) !dbg !454 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !455, metadata !DIExpression()), !dbg !456
  call void @llvm.dbg.value(metadata i64 %0, metadata !455, metadata !DIExpression()), !dbg !456
  store i64 %0, i64* %3, align 8, !dbg !456
  call void @llvm.dbg.declare(metadata i64* %2, metadata !457, metadata !DIExpression()), !dbg !456
  call void @llvm.dbg.value(metadata i64 %1, metadata !457, metadata !DIExpression()), !dbg !456
  store i64 %1, i64* %2, align 8, !dbg !456
  %4 = load i64, i64* %3, align 8, !dbg !458
  %5 = load i64, i64* %2, align 8, !dbg !458
  %6 = sdiv i64 %4, %5, !dbg !458
  ret i64 %6, !dbg !458
}

define void @"_CNW7builtin3opsEXi2/=EllL0L0"(i64*, i64) !dbg !459 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !460, metadata !DIExpression()), !dbg !461
  call void @llvm.dbg.declare(metadata i64* %2, metadata !462, metadata !DIExpression()), !dbg !461
  call void @llvm.dbg.value(metadata i64 %1, metadata !462, metadata !DIExpression()), !dbg !461
  store i64 %1, i64* %2, align 8, !dbg !461
  %3 = load i64, i64* %0, align 8, !dbg !463
  %4 = load i64, i64* %2, align 8, !dbg !463
  %5 = sdiv i64 %3, %4, !dbg !463
  call void @llvm.dbg.value(metadata i64 %5, metadata !460, metadata !DIExpression()), !dbg !463
  %6 = load i64, i64* %0, align 8, !dbg !463
  store i64 %5, i64* %0, align 8, !dbg !463
  ret void, !dbg !463
}

define i64 @"_CNW7builtin3opsEXi1/EmmL0L0"(i64, i64) !dbg !464 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !465, metadata !DIExpression()), !dbg !466
  call void @llvm.dbg.value(metadata i64 %0, metadata !465, metadata !DIExpression()), !dbg !466
  store i64 %0, i64* %3, align 8, !dbg !466
  call void @llvm.dbg.declare(metadata i64* %2, metadata !467, metadata !DIExpression()), !dbg !466
  call void @llvm.dbg.value(metadata i64 %1, metadata !467, metadata !DIExpression()), !dbg !466
  store i64 %1, i64* %2, align 8, !dbg !466
  %4 = load i64, i64* %3, align 8, !dbg !468
  %5 = load i64, i64* %2, align 8, !dbg !468
  %6 = udiv i64 %4, %5, !dbg !468
  ret i64 %6, !dbg !468
}

define void @"_CNW7builtin3opsEXi2/=EmmL0L0"(i64*, i64) !dbg !469 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !470, metadata !DIExpression()), !dbg !471
  call void @llvm.dbg.declare(metadata i64* %2, metadata !472, metadata !DIExpression()), !dbg !471
  call void @llvm.dbg.value(metadata i64 %1, metadata !472, metadata !DIExpression()), !dbg !471
  store i64 %1, i64* %2, align 8, !dbg !471
  %3 = load i64, i64* %0, align 8, !dbg !473
  %4 = load i64, i64* %2, align 8, !dbg !473
  %5 = udiv i64 %3, %4, !dbg !473
  call void @llvm.dbg.value(metadata i64 %5, metadata !470, metadata !DIExpression()), !dbg !473
  %6 = load i64, i64* %0, align 8, !dbg !473
  store i64 %5, i64* %0, align 8, !dbg !473
  ret void, !dbg !473
}

define i128 @"_CNW7builtin3opsEXi1/EnnL0L0"(i128, i128) !dbg !474 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !475, metadata !DIExpression()), !dbg !476
  call void @llvm.dbg.value(metadata i128 %0, metadata !475, metadata !DIExpression()), !dbg !476
  store i128 %0, i128* %3, align 16, !dbg !476
  call void @llvm.dbg.declare(metadata i128* %2, metadata !477, metadata !DIExpression()), !dbg !476
  call void @llvm.dbg.value(metadata i128 %1, metadata !477, metadata !DIExpression()), !dbg !476
  store i128 %1, i128* %2, align 16, !dbg !476
  %4 = load i128, i128* %3, align 16, !dbg !478
  %5 = load i128, i128* %2, align 16, !dbg !478
  %6 = sdiv i128 %4, %5, !dbg !478
  ret i128 %6, !dbg !478
}

define void @"_CNW7builtin3opsEXi2/=EnnL0L0"(i128*, i128) !dbg !479 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !480, metadata !DIExpression()), !dbg !481
  call void @llvm.dbg.declare(metadata i128* %2, metadata !482, metadata !DIExpression()), !dbg !481
  call void @llvm.dbg.value(metadata i128 %1, metadata !482, metadata !DIExpression()), !dbg !481
  store i128 %1, i128* %2, align 16, !dbg !481
  %3 = load i128, i128* %0, align 16, !dbg !483
  %4 = load i128, i128* %2, align 16, !dbg !483
  %5 = sdiv i128 %3, %4, !dbg !483
  call void @llvm.dbg.value(metadata i128 %5, metadata !480, metadata !DIExpression()), !dbg !483
  %6 = load i128, i128* %0, align 16, !dbg !483
  store i128 %5, i128* %0, align 16, !dbg !483
  ret void, !dbg !483
}

define i128 @"_CNW7builtin3opsEXi1/EooL0L0"(i128, i128) !dbg !484 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !485, metadata !DIExpression()), !dbg !486
  call void @llvm.dbg.value(metadata i128 %0, metadata !485, metadata !DIExpression()), !dbg !486
  store i128 %0, i128* %3, align 16, !dbg !486
  call void @llvm.dbg.declare(metadata i128* %2, metadata !487, metadata !DIExpression()), !dbg !486
  call void @llvm.dbg.value(metadata i128 %1, metadata !487, metadata !DIExpression()), !dbg !486
  store i128 %1, i128* %2, align 16, !dbg !486
  %4 = load i128, i128* %3, align 16, !dbg !488
  %5 = load i128, i128* %2, align 16, !dbg !488
  %6 = udiv i128 %4, %5, !dbg !488
  ret i128 %6, !dbg !488
}

define void @"_CNW7builtin3opsEXi2/=EooL0L0"(i128*, i128) !dbg !489 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !490, metadata !DIExpression()), !dbg !491
  call void @llvm.dbg.declare(metadata i128* %2, metadata !492, metadata !DIExpression()), !dbg !491
  call void @llvm.dbg.value(metadata i128 %1, metadata !492, metadata !DIExpression()), !dbg !491
  store i128 %1, i128* %2, align 16, !dbg !491
  %3 = load i128, i128* %0, align 16, !dbg !493
  %4 = load i128, i128* %2, align 16, !dbg !493
  %5 = udiv i128 %3, %4, !dbg !493
  call void @llvm.dbg.value(metadata i128 %5, metadata !490, metadata !DIExpression()), !dbg !493
  %6 = load i128, i128* %0, align 16, !dbg !493
  store i128 %5, i128* %0, align 16, !dbg !493
  ret void, !dbg !493
}

define i8 @"_CNW7builtin3opsEXi1%EaaL0L0"(i8, i8) !dbg !494 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !495, metadata !DIExpression()), !dbg !496
  call void @llvm.dbg.value(metadata i8 %0, metadata !495, metadata !DIExpression()), !dbg !496
  store i8 %0, i8* %3, align 1, !dbg !496
  call void @llvm.dbg.declare(metadata i8* %2, metadata !497, metadata !DIExpression()), !dbg !496
  call void @llvm.dbg.value(metadata i8 %1, metadata !497, metadata !DIExpression()), !dbg !496
  store i8 %1, i8* %2, align 1, !dbg !496
  %4 = load i8, i8* %3, align 1, !dbg !498
  %5 = load i8, i8* %2, align 1, !dbg !498
  %6 = srem i8 %4, %5, !dbg !498
  ret i8 %6, !dbg !498
}

define void @"_CNW7builtin3opsEXi2%=EaaL0L0"(i8*, i8) !dbg !499 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !500, metadata !DIExpression()), !dbg !501
  call void @llvm.dbg.declare(metadata i8* %2, metadata !502, metadata !DIExpression()), !dbg !501
  call void @llvm.dbg.value(metadata i8 %1, metadata !502, metadata !DIExpression()), !dbg !501
  store i8 %1, i8* %2, align 1, !dbg !501
  %3 = load i8, i8* %0, align 1, !dbg !503
  %4 = load i8, i8* %2, align 1, !dbg !503
  %5 = srem i8 %3, %4, !dbg !503
  call void @llvm.dbg.value(metadata i8 %5, metadata !500, metadata !DIExpression()), !dbg !503
  %6 = load i8, i8* %0, align 1, !dbg !503
  store i8 %5, i8* %0, align 1, !dbg !503
  ret void, !dbg !503
}

define i8 @"_CNW7builtin3opsEXi1%EhhL0L0"(i8, i8) !dbg !504 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !505, metadata !DIExpression()), !dbg !506
  call void @llvm.dbg.value(metadata i8 %0, metadata !505, metadata !DIExpression()), !dbg !506
  store i8 %0, i8* %3, align 1, !dbg !506
  call void @llvm.dbg.declare(metadata i8* %2, metadata !507, metadata !DIExpression()), !dbg !506
  call void @llvm.dbg.value(metadata i8 %1, metadata !507, metadata !DIExpression()), !dbg !506
  store i8 %1, i8* %2, align 1, !dbg !506
  %4 = load i8, i8* %3, align 1, !dbg !508
  %5 = load i8, i8* %2, align 1, !dbg !508
  %6 = urem i8 %4, %5, !dbg !508
  ret i8 %6, !dbg !508
}

define void @"_CNW7builtin3opsEXi2%=EhhL0L0"(i8*, i8) !dbg !509 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !510, metadata !DIExpression()), !dbg !511
  call void @llvm.dbg.declare(metadata i8* %2, metadata !512, metadata !DIExpression()), !dbg !511
  call void @llvm.dbg.value(metadata i8 %1, metadata !512, metadata !DIExpression()), !dbg !511
  store i8 %1, i8* %2, align 1, !dbg !511
  %3 = load i8, i8* %0, align 1, !dbg !513
  %4 = load i8, i8* %2, align 1, !dbg !513
  %5 = urem i8 %3, %4, !dbg !513
  call void @llvm.dbg.value(metadata i8 %5, metadata !510, metadata !DIExpression()), !dbg !513
  %6 = load i8, i8* %0, align 1, !dbg !513
  store i8 %5, i8* %0, align 1, !dbg !513
  ret void, !dbg !513
}

define i16 @"_CNW7builtin3opsEXi1%EssL0L0"(i16, i16) !dbg !514 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !515, metadata !DIExpression()), !dbg !516
  call void @llvm.dbg.value(metadata i16 %0, metadata !515, metadata !DIExpression()), !dbg !516
  store i16 %0, i16* %3, align 2, !dbg !516
  call void @llvm.dbg.declare(metadata i16* %2, metadata !517, metadata !DIExpression()), !dbg !516
  call void @llvm.dbg.value(metadata i16 %1, metadata !517, metadata !DIExpression()), !dbg !516
  store i16 %1, i16* %2, align 2, !dbg !516
  %4 = load i16, i16* %3, align 2, !dbg !518
  %5 = load i16, i16* %2, align 2, !dbg !518
  %6 = srem i16 %4, %5, !dbg !518
  ret i16 %6, !dbg !518
}

define void @"_CNW7builtin3opsEXi2%=EssL0L0"(i16*, i16) !dbg !519 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !520, metadata !DIExpression()), !dbg !521
  call void @llvm.dbg.declare(metadata i16* %2, metadata !522, metadata !DIExpression()), !dbg !521
  call void @llvm.dbg.value(metadata i16 %1, metadata !522, metadata !DIExpression()), !dbg !521
  store i16 %1, i16* %2, align 2, !dbg !521
  %3 = load i16, i16* %0, align 2, !dbg !523
  %4 = load i16, i16* %2, align 2, !dbg !523
  %5 = srem i16 %3, %4, !dbg !523
  call void @llvm.dbg.value(metadata i16 %5, metadata !520, metadata !DIExpression()), !dbg !523
  %6 = load i16, i16* %0, align 2, !dbg !523
  store i16 %5, i16* %0, align 2, !dbg !523
  ret void, !dbg !523
}

define i16 @"_CNW7builtin3opsEXi1%EttL0L0"(i16, i16) !dbg !524 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !525, metadata !DIExpression()), !dbg !526
  call void @llvm.dbg.value(metadata i16 %0, metadata !525, metadata !DIExpression()), !dbg !526
  store i16 %0, i16* %3, align 2, !dbg !526
  call void @llvm.dbg.declare(metadata i16* %2, metadata !527, metadata !DIExpression()), !dbg !526
  call void @llvm.dbg.value(metadata i16 %1, metadata !527, metadata !DIExpression()), !dbg !526
  store i16 %1, i16* %2, align 2, !dbg !526
  %4 = load i16, i16* %3, align 2, !dbg !528
  %5 = load i16, i16* %2, align 2, !dbg !528
  %6 = urem i16 %4, %5, !dbg !528
  ret i16 %6, !dbg !528
}

define void @"_CNW7builtin3opsEXi2%=EttL0L0"(i16*, i16) !dbg !529 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !530, metadata !DIExpression()), !dbg !531
  call void @llvm.dbg.declare(metadata i16* %2, metadata !532, metadata !DIExpression()), !dbg !531
  call void @llvm.dbg.value(metadata i16 %1, metadata !532, metadata !DIExpression()), !dbg !531
  store i16 %1, i16* %2, align 2, !dbg !531
  %3 = load i16, i16* %0, align 2, !dbg !533
  %4 = load i16, i16* %2, align 2, !dbg !533
  %5 = urem i16 %3, %4, !dbg !533
  call void @llvm.dbg.value(metadata i16 %5, metadata !530, metadata !DIExpression()), !dbg !533
  %6 = load i16, i16* %0, align 2, !dbg !533
  store i16 %5, i16* %0, align 2, !dbg !533
  ret void, !dbg !533
}

define i32 @"_CNW7builtin3opsEXi1%EiiL0L0"(i32, i32) !dbg !534 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !535, metadata !DIExpression()), !dbg !536
  call void @llvm.dbg.value(metadata i32 %0, metadata !535, metadata !DIExpression()), !dbg !536
  store i32 %0, i32* %3, align 4, !dbg !536
  call void @llvm.dbg.declare(metadata i32* %2, metadata !537, metadata !DIExpression()), !dbg !536
  call void @llvm.dbg.value(metadata i32 %1, metadata !537, metadata !DIExpression()), !dbg !536
  store i32 %1, i32* %2, align 4, !dbg !536
  %4 = load i32, i32* %3, align 4, !dbg !538
  %5 = load i32, i32* %2, align 4, !dbg !538
  %6 = srem i32 %4, %5, !dbg !538
  ret i32 %6, !dbg !538
}

define void @"_CNW7builtin3opsEXi2%=EiiL0L0"(i32*, i32) !dbg !539 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !540, metadata !DIExpression()), !dbg !541
  call void @llvm.dbg.declare(metadata i32* %2, metadata !542, metadata !DIExpression()), !dbg !541
  call void @llvm.dbg.value(metadata i32 %1, metadata !542, metadata !DIExpression()), !dbg !541
  store i32 %1, i32* %2, align 4, !dbg !541
  %3 = load i32, i32* %0, align 4, !dbg !543
  %4 = load i32, i32* %2, align 4, !dbg !543
  %5 = srem i32 %3, %4, !dbg !543
  call void @llvm.dbg.value(metadata i32 %5, metadata !540, metadata !DIExpression()), !dbg !543
  %6 = load i32, i32* %0, align 4, !dbg !543
  store i32 %5, i32* %0, align 4, !dbg !543
  ret void, !dbg !543
}

define i32 @"_CNW7builtin3opsEXi1%EjjL0L0"(i32, i32) !dbg !544 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !545, metadata !DIExpression()), !dbg !546
  call void @llvm.dbg.value(metadata i32 %0, metadata !545, metadata !DIExpression()), !dbg !546
  store i32 %0, i32* %3, align 4, !dbg !546
  call void @llvm.dbg.declare(metadata i32* %2, metadata !547, metadata !DIExpression()), !dbg !546
  call void @llvm.dbg.value(metadata i32 %1, metadata !547, metadata !DIExpression()), !dbg !546
  store i32 %1, i32* %2, align 4, !dbg !546
  %4 = load i32, i32* %3, align 4, !dbg !548
  %5 = load i32, i32* %2, align 4, !dbg !548
  %6 = urem i32 %4, %5, !dbg !548
  ret i32 %6, !dbg !548
}

define void @"_CNW7builtin3opsEXi2%=EjjL0L0"(i32*, i32) !dbg !549 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !550, metadata !DIExpression()), !dbg !551
  call void @llvm.dbg.declare(metadata i32* %2, metadata !552, metadata !DIExpression()), !dbg !551
  call void @llvm.dbg.value(metadata i32 %1, metadata !552, metadata !DIExpression()), !dbg !551
  store i32 %1, i32* %2, align 4, !dbg !551
  %3 = load i32, i32* %0, align 4, !dbg !553
  %4 = load i32, i32* %2, align 4, !dbg !553
  %5 = urem i32 %3, %4, !dbg !553
  call void @llvm.dbg.value(metadata i32 %5, metadata !550, metadata !DIExpression()), !dbg !553
  %6 = load i32, i32* %0, align 4, !dbg !553
  store i32 %5, i32* %0, align 4, !dbg !553
  ret void, !dbg !553
}

define i64 @"_CNW7builtin3opsEXi1%EllL0L0"(i64, i64) !dbg !554 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !555, metadata !DIExpression()), !dbg !556
  call void @llvm.dbg.value(metadata i64 %0, metadata !555, metadata !DIExpression()), !dbg !556
  store i64 %0, i64* %3, align 8, !dbg !556
  call void @llvm.dbg.declare(metadata i64* %2, metadata !557, metadata !DIExpression()), !dbg !556
  call void @llvm.dbg.value(metadata i64 %1, metadata !557, metadata !DIExpression()), !dbg !556
  store i64 %1, i64* %2, align 8, !dbg !556
  %4 = load i64, i64* %3, align 8, !dbg !558
  %5 = load i64, i64* %2, align 8, !dbg !558
  %6 = srem i64 %4, %5, !dbg !558
  ret i64 %6, !dbg !558
}

define void @"_CNW7builtin3opsEXi2%=EllL0L0"(i64*, i64) !dbg !559 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !560, metadata !DIExpression()), !dbg !561
  call void @llvm.dbg.declare(metadata i64* %2, metadata !562, metadata !DIExpression()), !dbg !561
  call void @llvm.dbg.value(metadata i64 %1, metadata !562, metadata !DIExpression()), !dbg !561
  store i64 %1, i64* %2, align 8, !dbg !561
  %3 = load i64, i64* %0, align 8, !dbg !563
  %4 = load i64, i64* %2, align 8, !dbg !563
  %5 = srem i64 %3, %4, !dbg !563
  call void @llvm.dbg.value(metadata i64 %5, metadata !560, metadata !DIExpression()), !dbg !563
  %6 = load i64, i64* %0, align 8, !dbg !563
  store i64 %5, i64* %0, align 8, !dbg !563
  ret void, !dbg !563
}

define i64 @"_CNW7builtin3opsEXi1%EmmL0L0"(i64, i64) !dbg !564 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !565, metadata !DIExpression()), !dbg !566
  call void @llvm.dbg.value(metadata i64 %0, metadata !565, metadata !DIExpression()), !dbg !566
  store i64 %0, i64* %3, align 8, !dbg !566
  call void @llvm.dbg.declare(metadata i64* %2, metadata !567, metadata !DIExpression()), !dbg !566
  call void @llvm.dbg.value(metadata i64 %1, metadata !567, metadata !DIExpression()), !dbg !566
  store i64 %1, i64* %2, align 8, !dbg !566
  %4 = load i64, i64* %3, align 8, !dbg !568
  %5 = load i64, i64* %2, align 8, !dbg !568
  %6 = urem i64 %4, %5, !dbg !568
  ret i64 %6, !dbg !568
}

define void @"_CNW7builtin3opsEXi2%=EmmL0L0"(i64*, i64) !dbg !569 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !570, metadata !DIExpression()), !dbg !571
  call void @llvm.dbg.declare(metadata i64* %2, metadata !572, metadata !DIExpression()), !dbg !571
  call void @llvm.dbg.value(metadata i64 %1, metadata !572, metadata !DIExpression()), !dbg !571
  store i64 %1, i64* %2, align 8, !dbg !571
  %3 = load i64, i64* %0, align 8, !dbg !573
  %4 = load i64, i64* %2, align 8, !dbg !573
  %5 = urem i64 %3, %4, !dbg !573
  call void @llvm.dbg.value(metadata i64 %5, metadata !570, metadata !DIExpression()), !dbg !573
  %6 = load i64, i64* %0, align 8, !dbg !573
  store i64 %5, i64* %0, align 8, !dbg !573
  ret void, !dbg !573
}

define i128 @"_CNW7builtin3opsEXi1%EnnL0L0"(i128, i128) !dbg !574 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !575, metadata !DIExpression()), !dbg !576
  call void @llvm.dbg.value(metadata i128 %0, metadata !575, metadata !DIExpression()), !dbg !576
  store i128 %0, i128* %3, align 16, !dbg !576
  call void @llvm.dbg.declare(metadata i128* %2, metadata !577, metadata !DIExpression()), !dbg !576
  call void @llvm.dbg.value(metadata i128 %1, metadata !577, metadata !DIExpression()), !dbg !576
  store i128 %1, i128* %2, align 16, !dbg !576
  %4 = load i128, i128* %3, align 16, !dbg !578
  %5 = load i128, i128* %2, align 16, !dbg !578
  %6 = srem i128 %4, %5, !dbg !578
  ret i128 %6, !dbg !578
}

define void @"_CNW7builtin3opsEXi2%=EnnL0L0"(i128*, i128) !dbg !579 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !580, metadata !DIExpression()), !dbg !581
  call void @llvm.dbg.declare(metadata i128* %2, metadata !582, metadata !DIExpression()), !dbg !581
  call void @llvm.dbg.value(metadata i128 %1, metadata !582, metadata !DIExpression()), !dbg !581
  store i128 %1, i128* %2, align 16, !dbg !581
  %3 = load i128, i128* %0, align 16, !dbg !583
  %4 = load i128, i128* %2, align 16, !dbg !583
  %5 = srem i128 %3, %4, !dbg !583
  call void @llvm.dbg.value(metadata i128 %5, metadata !580, metadata !DIExpression()), !dbg !583
  %6 = load i128, i128* %0, align 16, !dbg !583
  store i128 %5, i128* %0, align 16, !dbg !583
  ret void, !dbg !583
}

define i128 @"_CNW7builtin3opsEXi1%EooL0L0"(i128, i128) !dbg !584 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !585, metadata !DIExpression()), !dbg !586
  call void @llvm.dbg.value(metadata i128 %0, metadata !585, metadata !DIExpression()), !dbg !586
  store i128 %0, i128* %3, align 16, !dbg !586
  call void @llvm.dbg.declare(metadata i128* %2, metadata !587, metadata !DIExpression()), !dbg !586
  call void @llvm.dbg.value(metadata i128 %1, metadata !587, metadata !DIExpression()), !dbg !586
  store i128 %1, i128* %2, align 16, !dbg !586
  %4 = load i128, i128* %3, align 16, !dbg !588
  %5 = load i128, i128* %2, align 16, !dbg !588
  %6 = urem i128 %4, %5, !dbg !588
  ret i128 %6, !dbg !588
}

define void @"_CNW7builtin3opsEXi2%=EooL0L0"(i128*, i128) !dbg !589 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !590, metadata !DIExpression()), !dbg !591
  call void @llvm.dbg.declare(metadata i128* %2, metadata !592, metadata !DIExpression()), !dbg !591
  call void @llvm.dbg.value(metadata i128 %1, metadata !592, metadata !DIExpression()), !dbg !591
  store i128 %1, i128* %2, align 16, !dbg !591
  %3 = load i128, i128* %0, align 16, !dbg !593
  %4 = load i128, i128* %2, align 16, !dbg !593
  %5 = urem i128 %3, %4, !dbg !593
  call void @llvm.dbg.value(metadata i128 %5, metadata !590, metadata !DIExpression()), !dbg !593
  %6 = load i128, i128* %0, align 16, !dbg !593
  store i128 %5, i128* %0, align 16, !dbg !593
  ret void, !dbg !593
}

define i8 @"_CNW7builtin3opsEXi2<<EaaL0L0"(i8, i8) !dbg !594 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !595, metadata !DIExpression()), !dbg !596
  call void @llvm.dbg.value(metadata i8 %0, metadata !595, metadata !DIExpression()), !dbg !596
  store i8 %0, i8* %3, align 1, !dbg !596
  call void @llvm.dbg.declare(metadata i8* %2, metadata !597, metadata !DIExpression()), !dbg !596
  call void @llvm.dbg.value(metadata i8 %1, metadata !597, metadata !DIExpression()), !dbg !596
  store i8 %1, i8* %2, align 1, !dbg !596
  %4 = load i8, i8* %3, align 1, !dbg !598
  %5 = load i8, i8* %2, align 1, !dbg !598
  %6 = shl i8 %4, %5, !dbg !598
  ret i8 %6, !dbg !598
}

define void @"_CNW7builtin3opsEXi3<<=EaaL0L0"(i8*, i8) !dbg !599 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !600, metadata !DIExpression()), !dbg !601
  call void @llvm.dbg.declare(metadata i8* %2, metadata !602, metadata !DIExpression()), !dbg !601
  call void @llvm.dbg.value(metadata i8 %1, metadata !602, metadata !DIExpression()), !dbg !601
  store i8 %1, i8* %2, align 1, !dbg !601
  %3 = load i8, i8* %0, align 1, !dbg !603
  %4 = load i8, i8* %2, align 1, !dbg !603
  %5 = shl i8 %3, %4, !dbg !603
  call void @llvm.dbg.value(metadata i8 %5, metadata !600, metadata !DIExpression()), !dbg !603
  %6 = load i8, i8* %0, align 1, !dbg !603
  store i8 %5, i8* %0, align 1, !dbg !603
  ret void, !dbg !603
}

define i8 @"_CNW7builtin3opsEXi2<<EhhL0L0"(i8, i8) !dbg !604 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !605, metadata !DIExpression()), !dbg !606
  call void @llvm.dbg.value(metadata i8 %0, metadata !605, metadata !DIExpression()), !dbg !606
  store i8 %0, i8* %3, align 1, !dbg !606
  call void @llvm.dbg.declare(metadata i8* %2, metadata !607, metadata !DIExpression()), !dbg !606
  call void @llvm.dbg.value(metadata i8 %1, metadata !607, metadata !DIExpression()), !dbg !606
  store i8 %1, i8* %2, align 1, !dbg !606
  %4 = load i8, i8* %3, align 1, !dbg !608
  %5 = load i8, i8* %2, align 1, !dbg !608
  %6 = shl i8 %4, %5, !dbg !608
  ret i8 %6, !dbg !608
}

define void @"_CNW7builtin3opsEXi3<<=EhhL0L0"(i8*, i8) !dbg !609 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !610, metadata !DIExpression()), !dbg !611
  call void @llvm.dbg.declare(metadata i8* %2, metadata !612, metadata !DIExpression()), !dbg !611
  call void @llvm.dbg.value(metadata i8 %1, metadata !612, metadata !DIExpression()), !dbg !611
  store i8 %1, i8* %2, align 1, !dbg !611
  %3 = load i8, i8* %0, align 1, !dbg !613
  %4 = load i8, i8* %2, align 1, !dbg !613
  %5 = shl i8 %3, %4, !dbg !613
  call void @llvm.dbg.value(metadata i8 %5, metadata !610, metadata !DIExpression()), !dbg !613
  %6 = load i8, i8* %0, align 1, !dbg !613
  store i8 %5, i8* %0, align 1, !dbg !613
  ret void, !dbg !613
}

define i16 @"_CNW7builtin3opsEXi2<<EssL0L0"(i16, i16) !dbg !614 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !615, metadata !DIExpression()), !dbg !616
  call void @llvm.dbg.value(metadata i16 %0, metadata !615, metadata !DIExpression()), !dbg !616
  store i16 %0, i16* %3, align 2, !dbg !616
  call void @llvm.dbg.declare(metadata i16* %2, metadata !617, metadata !DIExpression()), !dbg !616
  call void @llvm.dbg.value(metadata i16 %1, metadata !617, metadata !DIExpression()), !dbg !616
  store i16 %1, i16* %2, align 2, !dbg !616
  %4 = load i16, i16* %3, align 2, !dbg !618
  %5 = load i16, i16* %2, align 2, !dbg !618
  %6 = shl i16 %4, %5, !dbg !618
  ret i16 %6, !dbg !618
}

define void @"_CNW7builtin3opsEXi3<<=EssL0L0"(i16*, i16) !dbg !619 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !620, metadata !DIExpression()), !dbg !621
  call void @llvm.dbg.declare(metadata i16* %2, metadata !622, metadata !DIExpression()), !dbg !621
  call void @llvm.dbg.value(metadata i16 %1, metadata !622, metadata !DIExpression()), !dbg !621
  store i16 %1, i16* %2, align 2, !dbg !621
  %3 = load i16, i16* %0, align 2, !dbg !623
  %4 = load i16, i16* %2, align 2, !dbg !623
  %5 = shl i16 %3, %4, !dbg !623
  call void @llvm.dbg.value(metadata i16 %5, metadata !620, metadata !DIExpression()), !dbg !623
  %6 = load i16, i16* %0, align 2, !dbg !623
  store i16 %5, i16* %0, align 2, !dbg !623
  ret void, !dbg !623
}

define i16 @"_CNW7builtin3opsEXi2<<EttL0L0"(i16, i16) !dbg !624 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !625, metadata !DIExpression()), !dbg !626
  call void @llvm.dbg.value(metadata i16 %0, metadata !625, metadata !DIExpression()), !dbg !626
  store i16 %0, i16* %3, align 2, !dbg !626
  call void @llvm.dbg.declare(metadata i16* %2, metadata !627, metadata !DIExpression()), !dbg !626
  call void @llvm.dbg.value(metadata i16 %1, metadata !627, metadata !DIExpression()), !dbg !626
  store i16 %1, i16* %2, align 2, !dbg !626
  %4 = load i16, i16* %3, align 2, !dbg !628
  %5 = load i16, i16* %2, align 2, !dbg !628
  %6 = shl i16 %4, %5, !dbg !628
  ret i16 %6, !dbg !628
}

define void @"_CNW7builtin3opsEXi3<<=EttL0L0"(i16*, i16) !dbg !629 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !630, metadata !DIExpression()), !dbg !631
  call void @llvm.dbg.declare(metadata i16* %2, metadata !632, metadata !DIExpression()), !dbg !631
  call void @llvm.dbg.value(metadata i16 %1, metadata !632, metadata !DIExpression()), !dbg !631
  store i16 %1, i16* %2, align 2, !dbg !631
  %3 = load i16, i16* %0, align 2, !dbg !633
  %4 = load i16, i16* %2, align 2, !dbg !633
  %5 = shl i16 %3, %4, !dbg !633
  call void @llvm.dbg.value(metadata i16 %5, metadata !630, metadata !DIExpression()), !dbg !633
  %6 = load i16, i16* %0, align 2, !dbg !633
  store i16 %5, i16* %0, align 2, !dbg !633
  ret void, !dbg !633
}

define i32 @"_CNW7builtin3opsEXi2<<EiiL0L0"(i32, i32) !dbg !634 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !635, metadata !DIExpression()), !dbg !636
  call void @llvm.dbg.value(metadata i32 %0, metadata !635, metadata !DIExpression()), !dbg !636
  store i32 %0, i32* %3, align 4, !dbg !636
  call void @llvm.dbg.declare(metadata i32* %2, metadata !637, metadata !DIExpression()), !dbg !636
  call void @llvm.dbg.value(metadata i32 %1, metadata !637, metadata !DIExpression()), !dbg !636
  store i32 %1, i32* %2, align 4, !dbg !636
  %4 = load i32, i32* %3, align 4, !dbg !638
  %5 = load i32, i32* %2, align 4, !dbg !638
  %6 = shl i32 %4, %5, !dbg !638
  ret i32 %6, !dbg !638
}

define void @"_CNW7builtin3opsEXi3<<=EiiL0L0"(i32*, i32) !dbg !639 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !640, metadata !DIExpression()), !dbg !641
  call void @llvm.dbg.declare(metadata i32* %2, metadata !642, metadata !DIExpression()), !dbg !641
  call void @llvm.dbg.value(metadata i32 %1, metadata !642, metadata !DIExpression()), !dbg !641
  store i32 %1, i32* %2, align 4, !dbg !641
  %3 = load i32, i32* %0, align 4, !dbg !643
  %4 = load i32, i32* %2, align 4, !dbg !643
  %5 = shl i32 %3, %4, !dbg !643
  call void @llvm.dbg.value(metadata i32 %5, metadata !640, metadata !DIExpression()), !dbg !643
  %6 = load i32, i32* %0, align 4, !dbg !643
  store i32 %5, i32* %0, align 4, !dbg !643
  ret void, !dbg !643
}

define i32 @"_CNW7builtin3opsEXi2<<EjjL0L0"(i32, i32) !dbg !644 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !645, metadata !DIExpression()), !dbg !646
  call void @llvm.dbg.value(metadata i32 %0, metadata !645, metadata !DIExpression()), !dbg !646
  store i32 %0, i32* %3, align 4, !dbg !646
  call void @llvm.dbg.declare(metadata i32* %2, metadata !647, metadata !DIExpression()), !dbg !646
  call void @llvm.dbg.value(metadata i32 %1, metadata !647, metadata !DIExpression()), !dbg !646
  store i32 %1, i32* %2, align 4, !dbg !646
  %4 = load i32, i32* %3, align 4, !dbg !648
  %5 = load i32, i32* %2, align 4, !dbg !648
  %6 = shl i32 %4, %5, !dbg !648
  ret i32 %6, !dbg !648
}

define void @"_CNW7builtin3opsEXi3<<=EjjL0L0"(i32*, i32) !dbg !649 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !650, metadata !DIExpression()), !dbg !651
  call void @llvm.dbg.declare(metadata i32* %2, metadata !652, metadata !DIExpression()), !dbg !651
  call void @llvm.dbg.value(metadata i32 %1, metadata !652, metadata !DIExpression()), !dbg !651
  store i32 %1, i32* %2, align 4, !dbg !651
  %3 = load i32, i32* %0, align 4, !dbg !653
  %4 = load i32, i32* %2, align 4, !dbg !653
  %5 = shl i32 %3, %4, !dbg !653
  call void @llvm.dbg.value(metadata i32 %5, metadata !650, metadata !DIExpression()), !dbg !653
  %6 = load i32, i32* %0, align 4, !dbg !653
  store i32 %5, i32* %0, align 4, !dbg !653
  ret void, !dbg !653
}

define i64 @"_CNW7builtin3opsEXi2<<EllL0L0"(i64, i64) !dbg !654 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !655, metadata !DIExpression()), !dbg !656
  call void @llvm.dbg.value(metadata i64 %0, metadata !655, metadata !DIExpression()), !dbg !656
  store i64 %0, i64* %3, align 8, !dbg !656
  call void @llvm.dbg.declare(metadata i64* %2, metadata !657, metadata !DIExpression()), !dbg !656
  call void @llvm.dbg.value(metadata i64 %1, metadata !657, metadata !DIExpression()), !dbg !656
  store i64 %1, i64* %2, align 8, !dbg !656
  %4 = load i64, i64* %3, align 8, !dbg !658
  %5 = load i64, i64* %2, align 8, !dbg !658
  %6 = shl i64 %4, %5, !dbg !658
  ret i64 %6, !dbg !658
}

define void @"_CNW7builtin3opsEXi3<<=EllL0L0"(i64*, i64) !dbg !659 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !660, metadata !DIExpression()), !dbg !661
  call void @llvm.dbg.declare(metadata i64* %2, metadata !662, metadata !DIExpression()), !dbg !661
  call void @llvm.dbg.value(metadata i64 %1, metadata !662, metadata !DIExpression()), !dbg !661
  store i64 %1, i64* %2, align 8, !dbg !661
  %3 = load i64, i64* %0, align 8, !dbg !663
  %4 = load i64, i64* %2, align 8, !dbg !663
  %5 = shl i64 %3, %4, !dbg !663
  call void @llvm.dbg.value(metadata i64 %5, metadata !660, metadata !DIExpression()), !dbg !663
  %6 = load i64, i64* %0, align 8, !dbg !663
  store i64 %5, i64* %0, align 8, !dbg !663
  ret void, !dbg !663
}

define i64 @"_CNW7builtin3opsEXi2<<EmmL0L0"(i64, i64) !dbg !664 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !665, metadata !DIExpression()), !dbg !666
  call void @llvm.dbg.value(metadata i64 %0, metadata !665, metadata !DIExpression()), !dbg !666
  store i64 %0, i64* %3, align 8, !dbg !666
  call void @llvm.dbg.declare(metadata i64* %2, metadata !667, metadata !DIExpression()), !dbg !666
  call void @llvm.dbg.value(metadata i64 %1, metadata !667, metadata !DIExpression()), !dbg !666
  store i64 %1, i64* %2, align 8, !dbg !666
  %4 = load i64, i64* %3, align 8, !dbg !668
  %5 = load i64, i64* %2, align 8, !dbg !668
  %6 = shl i64 %4, %5, !dbg !668
  ret i64 %6, !dbg !668
}

define void @"_CNW7builtin3opsEXi3<<=EmmL0L0"(i64*, i64) !dbg !669 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !670, metadata !DIExpression()), !dbg !671
  call void @llvm.dbg.declare(metadata i64* %2, metadata !672, metadata !DIExpression()), !dbg !671
  call void @llvm.dbg.value(metadata i64 %1, metadata !672, metadata !DIExpression()), !dbg !671
  store i64 %1, i64* %2, align 8, !dbg !671
  %3 = load i64, i64* %0, align 8, !dbg !673
  %4 = load i64, i64* %2, align 8, !dbg !673
  %5 = shl i64 %3, %4, !dbg !673
  call void @llvm.dbg.value(metadata i64 %5, metadata !670, metadata !DIExpression()), !dbg !673
  %6 = load i64, i64* %0, align 8, !dbg !673
  store i64 %5, i64* %0, align 8, !dbg !673
  ret void, !dbg !673
}

define i128 @"_CNW7builtin3opsEXi2<<EnnL0L0"(i128, i128) !dbg !674 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !675, metadata !DIExpression()), !dbg !676
  call void @llvm.dbg.value(metadata i128 %0, metadata !675, metadata !DIExpression()), !dbg !676
  store i128 %0, i128* %3, align 16, !dbg !676
  call void @llvm.dbg.declare(metadata i128* %2, metadata !677, metadata !DIExpression()), !dbg !676
  call void @llvm.dbg.value(metadata i128 %1, metadata !677, metadata !DIExpression()), !dbg !676
  store i128 %1, i128* %2, align 16, !dbg !676
  %4 = load i128, i128* %3, align 16, !dbg !678
  %5 = load i128, i128* %2, align 16, !dbg !678
  %6 = shl i128 %4, %5, !dbg !678
  ret i128 %6, !dbg !678
}

define void @"_CNW7builtin3opsEXi3<<=EnnL0L0"(i128*, i128) !dbg !679 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !680, metadata !DIExpression()), !dbg !681
  call void @llvm.dbg.declare(metadata i128* %2, metadata !682, metadata !DIExpression()), !dbg !681
  call void @llvm.dbg.value(metadata i128 %1, metadata !682, metadata !DIExpression()), !dbg !681
  store i128 %1, i128* %2, align 16, !dbg !681
  %3 = load i128, i128* %0, align 16, !dbg !683
  %4 = load i128, i128* %2, align 16, !dbg !683
  %5 = shl i128 %3, %4, !dbg !683
  call void @llvm.dbg.value(metadata i128 %5, metadata !680, metadata !DIExpression()), !dbg !683
  %6 = load i128, i128* %0, align 16, !dbg !683
  store i128 %5, i128* %0, align 16, !dbg !683
  ret void, !dbg !683
}

define i128 @"_CNW7builtin3opsEXi2<<EooL0L0"(i128, i128) !dbg !684 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !685, metadata !DIExpression()), !dbg !686
  call void @llvm.dbg.value(metadata i128 %0, metadata !685, metadata !DIExpression()), !dbg !686
  store i128 %0, i128* %3, align 16, !dbg !686
  call void @llvm.dbg.declare(metadata i128* %2, metadata !687, metadata !DIExpression()), !dbg !686
  call void @llvm.dbg.value(metadata i128 %1, metadata !687, metadata !DIExpression()), !dbg !686
  store i128 %1, i128* %2, align 16, !dbg !686
  %4 = load i128, i128* %3, align 16, !dbg !688
  %5 = load i128, i128* %2, align 16, !dbg !688
  %6 = shl i128 %4, %5, !dbg !688
  ret i128 %6, !dbg !688
}

define void @"_CNW7builtin3opsEXi3<<=EooL0L0"(i128*, i128) !dbg !689 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !690, metadata !DIExpression()), !dbg !691
  call void @llvm.dbg.declare(metadata i128* %2, metadata !692, metadata !DIExpression()), !dbg !691
  call void @llvm.dbg.value(metadata i128 %1, metadata !692, metadata !DIExpression()), !dbg !691
  store i128 %1, i128* %2, align 16, !dbg !691
  %3 = load i128, i128* %0, align 16, !dbg !693
  %4 = load i128, i128* %2, align 16, !dbg !693
  %5 = shl i128 %3, %4, !dbg !693
  call void @llvm.dbg.value(metadata i128 %5, metadata !690, metadata !DIExpression()), !dbg !693
  %6 = load i128, i128* %0, align 16, !dbg !693
  store i128 %5, i128* %0, align 16, !dbg !693
  ret void, !dbg !693
}

define i8 @"_CNW7builtin3opsEXi2>>EaaL0L0"(i8, i8) !dbg !694 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !695, metadata !DIExpression()), !dbg !696
  call void @llvm.dbg.value(metadata i8 %0, metadata !695, metadata !DIExpression()), !dbg !696
  store i8 %0, i8* %3, align 1, !dbg !696
  call void @llvm.dbg.declare(metadata i8* %2, metadata !697, metadata !DIExpression()), !dbg !696
  call void @llvm.dbg.value(metadata i8 %1, metadata !697, metadata !DIExpression()), !dbg !696
  store i8 %1, i8* %2, align 1, !dbg !696
  %4 = load i8, i8* %3, align 1, !dbg !698
  %5 = load i8, i8* %2, align 1, !dbg !698
  %6 = ashr i8 %4, %5, !dbg !698
  ret i8 %6, !dbg !698
}

define void @"_CNW7builtin3opsEXi3>>=EaaL0L0"(i8*, i8) !dbg !699 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !700, metadata !DIExpression()), !dbg !701
  call void @llvm.dbg.declare(metadata i8* %2, metadata !702, metadata !DIExpression()), !dbg !701
  call void @llvm.dbg.value(metadata i8 %1, metadata !702, metadata !DIExpression()), !dbg !701
  store i8 %1, i8* %2, align 1, !dbg !701
  %3 = load i8, i8* %0, align 1, !dbg !703
  %4 = load i8, i8* %2, align 1, !dbg !703
  %5 = ashr i8 %3, %4, !dbg !703
  call void @llvm.dbg.value(metadata i8 %5, metadata !700, metadata !DIExpression()), !dbg !703
  %6 = load i8, i8* %0, align 1, !dbg !703
  store i8 %5, i8* %0, align 1, !dbg !703
  ret void, !dbg !703
}

define i16 @"_CNW7builtin3opsEXi2>>EssL0L0"(i16, i16) !dbg !704 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !705, metadata !DIExpression()), !dbg !706
  call void @llvm.dbg.value(metadata i16 %0, metadata !705, metadata !DIExpression()), !dbg !706
  store i16 %0, i16* %3, align 2, !dbg !706
  call void @llvm.dbg.declare(metadata i16* %2, metadata !707, metadata !DIExpression()), !dbg !706
  call void @llvm.dbg.value(metadata i16 %1, metadata !707, metadata !DIExpression()), !dbg !706
  store i16 %1, i16* %2, align 2, !dbg !706
  %4 = load i16, i16* %3, align 2, !dbg !708
  %5 = load i16, i16* %2, align 2, !dbg !708
  %6 = ashr i16 %4, %5, !dbg !708
  ret i16 %6, !dbg !708
}

define void @"_CNW7builtin3opsEXi3>>=EssL0L0"(i16*, i16) !dbg !709 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !710, metadata !DIExpression()), !dbg !711
  call void @llvm.dbg.declare(metadata i16* %2, metadata !712, metadata !DIExpression()), !dbg !711
  call void @llvm.dbg.value(metadata i16 %1, metadata !712, metadata !DIExpression()), !dbg !711
  store i16 %1, i16* %2, align 2, !dbg !711
  %3 = load i16, i16* %0, align 2, !dbg !713
  %4 = load i16, i16* %2, align 2, !dbg !713
  %5 = ashr i16 %3, %4, !dbg !713
  call void @llvm.dbg.value(metadata i16 %5, metadata !710, metadata !DIExpression()), !dbg !713
  %6 = load i16, i16* %0, align 2, !dbg !713
  store i16 %5, i16* %0, align 2, !dbg !713
  ret void, !dbg !713
}

define i32 @"_CNW7builtin3opsEXi2>>EiiL0L0"(i32, i32) !dbg !714 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !715, metadata !DIExpression()), !dbg !716
  call void @llvm.dbg.value(metadata i32 %0, metadata !715, metadata !DIExpression()), !dbg !716
  store i32 %0, i32* %3, align 4, !dbg !716
  call void @llvm.dbg.declare(metadata i32* %2, metadata !717, metadata !DIExpression()), !dbg !716
  call void @llvm.dbg.value(metadata i32 %1, metadata !717, metadata !DIExpression()), !dbg !716
  store i32 %1, i32* %2, align 4, !dbg !716
  %4 = load i32, i32* %3, align 4, !dbg !718
  %5 = load i32, i32* %2, align 4, !dbg !718
  %6 = ashr i32 %4, %5, !dbg !718
  ret i32 %6, !dbg !718
}

define void @"_CNW7builtin3opsEXi3>>=EiiL0L0"(i32*, i32) !dbg !719 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !720, metadata !DIExpression()), !dbg !721
  call void @llvm.dbg.declare(metadata i32* %2, metadata !722, metadata !DIExpression()), !dbg !721
  call void @llvm.dbg.value(metadata i32 %1, metadata !722, metadata !DIExpression()), !dbg !721
  store i32 %1, i32* %2, align 4, !dbg !721
  %3 = load i32, i32* %0, align 4, !dbg !723
  %4 = load i32, i32* %2, align 4, !dbg !723
  %5 = ashr i32 %3, %4, !dbg !723
  call void @llvm.dbg.value(metadata i32 %5, metadata !720, metadata !DIExpression()), !dbg !723
  %6 = load i32, i32* %0, align 4, !dbg !723
  store i32 %5, i32* %0, align 4, !dbg !723
  ret void, !dbg !723
}

define i64 @"_CNW7builtin3opsEXi2>>EllL0L0"(i64, i64) !dbg !724 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !725, metadata !DIExpression()), !dbg !726
  call void @llvm.dbg.value(metadata i64 %0, metadata !725, metadata !DIExpression()), !dbg !726
  store i64 %0, i64* %3, align 8, !dbg !726
  call void @llvm.dbg.declare(metadata i64* %2, metadata !727, metadata !DIExpression()), !dbg !726
  call void @llvm.dbg.value(metadata i64 %1, metadata !727, metadata !DIExpression()), !dbg !726
  store i64 %1, i64* %2, align 8, !dbg !726
  %4 = load i64, i64* %3, align 8, !dbg !728
  %5 = load i64, i64* %2, align 8, !dbg !728
  %6 = ashr i64 %4, %5, !dbg !728
  ret i64 %6, !dbg !728
}

define void @"_CNW7builtin3opsEXi3>>=EllL0L0"(i64*, i64) !dbg !729 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !730, metadata !DIExpression()), !dbg !731
  call void @llvm.dbg.declare(metadata i64* %2, metadata !732, metadata !DIExpression()), !dbg !731
  call void @llvm.dbg.value(metadata i64 %1, metadata !732, metadata !DIExpression()), !dbg !731
  store i64 %1, i64* %2, align 8, !dbg !731
  %3 = load i64, i64* %0, align 8, !dbg !733
  %4 = load i64, i64* %2, align 8, !dbg !733
  %5 = ashr i64 %3, %4, !dbg !733
  call void @llvm.dbg.value(metadata i64 %5, metadata !730, metadata !DIExpression()), !dbg !733
  %6 = load i64, i64* %0, align 8, !dbg !733
  store i64 %5, i64* %0, align 8, !dbg !733
  ret void, !dbg !733
}

define i128 @"_CNW7builtin3opsEXi2>>EnnL0L0"(i128, i128) !dbg !734 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !735, metadata !DIExpression()), !dbg !736
  call void @llvm.dbg.value(metadata i128 %0, metadata !735, metadata !DIExpression()), !dbg !736
  store i128 %0, i128* %3, align 16, !dbg !736
  call void @llvm.dbg.declare(metadata i128* %2, metadata !737, metadata !DIExpression()), !dbg !736
  call void @llvm.dbg.value(metadata i128 %1, metadata !737, metadata !DIExpression()), !dbg !736
  store i128 %1, i128* %2, align 16, !dbg !736
  %4 = load i128, i128* %3, align 16, !dbg !738
  %5 = load i128, i128* %2, align 16, !dbg !738
  %6 = ashr i128 %4, %5, !dbg !738
  ret i128 %6, !dbg !738
}

define void @"_CNW7builtin3opsEXi3>>=EnnL0L0"(i128*, i128) !dbg !739 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !740, metadata !DIExpression()), !dbg !741
  call void @llvm.dbg.declare(metadata i128* %2, metadata !742, metadata !DIExpression()), !dbg !741
  call void @llvm.dbg.value(metadata i128 %1, metadata !742, metadata !DIExpression()), !dbg !741
  store i128 %1, i128* %2, align 16, !dbg !741
  %3 = load i128, i128* %0, align 16, !dbg !743
  %4 = load i128, i128* %2, align 16, !dbg !743
  %5 = ashr i128 %3, %4, !dbg !743
  call void @llvm.dbg.value(metadata i128 %5, metadata !740, metadata !DIExpression()), !dbg !743
  %6 = load i128, i128* %0, align 16, !dbg !743
  store i128 %5, i128* %0, align 16, !dbg !743
  ret void, !dbg !743
}

define i8 @"_CNW7builtin3opsEXi2>>EhhL0L0"(i8, i8) !dbg !744 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !745, metadata !DIExpression()), !dbg !746
  call void @llvm.dbg.value(metadata i8 %0, metadata !745, metadata !DIExpression()), !dbg !746
  store i8 %0, i8* %3, align 1, !dbg !746
  call void @llvm.dbg.declare(metadata i8* %2, metadata !747, metadata !DIExpression()), !dbg !746
  call void @llvm.dbg.value(metadata i8 %1, metadata !747, metadata !DIExpression()), !dbg !746
  store i8 %1, i8* %2, align 1, !dbg !746
  %4 = load i8, i8* %3, align 1, !dbg !748
  %5 = load i8, i8* %2, align 1, !dbg !748
  %6 = lshr i8 %4, %5, !dbg !748
  ret i8 %6, !dbg !748
}

define void @"_CNW7builtin3opsEXi3>>=EhhL0L0"(i8*, i8) !dbg !749 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !750, metadata !DIExpression()), !dbg !751
  call void @llvm.dbg.declare(metadata i8* %2, metadata !752, metadata !DIExpression()), !dbg !751
  call void @llvm.dbg.value(metadata i8 %1, metadata !752, metadata !DIExpression()), !dbg !751
  store i8 %1, i8* %2, align 1, !dbg !751
  %3 = load i8, i8* %0, align 1, !dbg !753
  %4 = load i8, i8* %2, align 1, !dbg !753
  %5 = lshr i8 %3, %4, !dbg !753
  call void @llvm.dbg.value(metadata i8 %5, metadata !750, metadata !DIExpression()), !dbg !753
  %6 = load i8, i8* %0, align 1, !dbg !753
  store i8 %5, i8* %0, align 1, !dbg !753
  ret void, !dbg !753
}

define i16 @"_CNW7builtin3opsEXi2>>EttL0L0"(i16, i16) !dbg !754 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !755, metadata !DIExpression()), !dbg !756
  call void @llvm.dbg.value(metadata i16 %0, metadata !755, metadata !DIExpression()), !dbg !756
  store i16 %0, i16* %3, align 2, !dbg !756
  call void @llvm.dbg.declare(metadata i16* %2, metadata !757, metadata !DIExpression()), !dbg !756
  call void @llvm.dbg.value(metadata i16 %1, metadata !757, metadata !DIExpression()), !dbg !756
  store i16 %1, i16* %2, align 2, !dbg !756
  %4 = load i16, i16* %3, align 2, !dbg !758
  %5 = load i16, i16* %2, align 2, !dbg !758
  %6 = lshr i16 %4, %5, !dbg !758
  ret i16 %6, !dbg !758
}

define void @"_CNW7builtin3opsEXi3>>=EttL0L0"(i16*, i16) !dbg !759 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !760, metadata !DIExpression()), !dbg !761
  call void @llvm.dbg.declare(metadata i16* %2, metadata !762, metadata !DIExpression()), !dbg !761
  call void @llvm.dbg.value(metadata i16 %1, metadata !762, metadata !DIExpression()), !dbg !761
  store i16 %1, i16* %2, align 2, !dbg !761
  %3 = load i16, i16* %0, align 2, !dbg !763
  %4 = load i16, i16* %2, align 2, !dbg !763
  %5 = lshr i16 %3, %4, !dbg !763
  call void @llvm.dbg.value(metadata i16 %5, metadata !760, metadata !DIExpression()), !dbg !763
  %6 = load i16, i16* %0, align 2, !dbg !763
  store i16 %5, i16* %0, align 2, !dbg !763
  ret void, !dbg !763
}

define i32 @"_CNW7builtin3opsEXi2>>EjjL0L0"(i32, i32) !dbg !764 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !765, metadata !DIExpression()), !dbg !766
  call void @llvm.dbg.value(metadata i32 %0, metadata !765, metadata !DIExpression()), !dbg !766
  store i32 %0, i32* %3, align 4, !dbg !766
  call void @llvm.dbg.declare(metadata i32* %2, metadata !767, metadata !DIExpression()), !dbg !766
  call void @llvm.dbg.value(metadata i32 %1, metadata !767, metadata !DIExpression()), !dbg !766
  store i32 %1, i32* %2, align 4, !dbg !766
  %4 = load i32, i32* %3, align 4, !dbg !768
  %5 = load i32, i32* %2, align 4, !dbg !768
  %6 = lshr i32 %4, %5, !dbg !768
  ret i32 %6, !dbg !768
}

define void @"_CNW7builtin3opsEXi3>>=EjjL0L0"(i32*, i32) !dbg !769 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !770, metadata !DIExpression()), !dbg !771
  call void @llvm.dbg.declare(metadata i32* %2, metadata !772, metadata !DIExpression()), !dbg !771
  call void @llvm.dbg.value(metadata i32 %1, metadata !772, metadata !DIExpression()), !dbg !771
  store i32 %1, i32* %2, align 4, !dbg !771
  %3 = load i32, i32* %0, align 4, !dbg !773
  %4 = load i32, i32* %2, align 4, !dbg !773
  %5 = lshr i32 %3, %4, !dbg !773
  call void @llvm.dbg.value(metadata i32 %5, metadata !770, metadata !DIExpression()), !dbg !773
  %6 = load i32, i32* %0, align 4, !dbg !773
  store i32 %5, i32* %0, align 4, !dbg !773
  ret void, !dbg !773
}

define i64 @"_CNW7builtin3opsEXi2>>EmmL0L0"(i64, i64) !dbg !774 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !775, metadata !DIExpression()), !dbg !776
  call void @llvm.dbg.value(metadata i64 %0, metadata !775, metadata !DIExpression()), !dbg !776
  store i64 %0, i64* %3, align 8, !dbg !776
  call void @llvm.dbg.declare(metadata i64* %2, metadata !777, metadata !DIExpression()), !dbg !776
  call void @llvm.dbg.value(metadata i64 %1, metadata !777, metadata !DIExpression()), !dbg !776
  store i64 %1, i64* %2, align 8, !dbg !776
  %4 = load i64, i64* %3, align 8, !dbg !778
  %5 = load i64, i64* %2, align 8, !dbg !778
  %6 = lshr i64 %4, %5, !dbg !778
  ret i64 %6, !dbg !778
}

define void @"_CNW7builtin3opsEXi3>>=EmmL0L0"(i64*, i64) !dbg !779 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !780, metadata !DIExpression()), !dbg !781
  call void @llvm.dbg.declare(metadata i64* %2, metadata !782, metadata !DIExpression()), !dbg !781
  call void @llvm.dbg.value(metadata i64 %1, metadata !782, metadata !DIExpression()), !dbg !781
  store i64 %1, i64* %2, align 8, !dbg !781
  %3 = load i64, i64* %0, align 8, !dbg !783
  %4 = load i64, i64* %2, align 8, !dbg !783
  %5 = lshr i64 %3, %4, !dbg !783
  call void @llvm.dbg.value(metadata i64 %5, metadata !780, metadata !DIExpression()), !dbg !783
  %6 = load i64, i64* %0, align 8, !dbg !783
  store i64 %5, i64* %0, align 8, !dbg !783
  ret void, !dbg !783
}

define i128 @"_CNW7builtin3opsEXi2>>EooL0L0"(i128, i128) !dbg !784 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !785, metadata !DIExpression()), !dbg !786
  call void @llvm.dbg.value(metadata i128 %0, metadata !785, metadata !DIExpression()), !dbg !786
  store i128 %0, i128* %3, align 16, !dbg !786
  call void @llvm.dbg.declare(metadata i128* %2, metadata !787, metadata !DIExpression()), !dbg !786
  call void @llvm.dbg.value(metadata i128 %1, metadata !787, metadata !DIExpression()), !dbg !786
  store i128 %1, i128* %2, align 16, !dbg !786
  %4 = load i128, i128* %3, align 16, !dbg !788
  %5 = load i128, i128* %2, align 16, !dbg !788
  %6 = lshr i128 %4, %5, !dbg !788
  ret i128 %6, !dbg !788
}

define void @"_CNW7builtin3opsEXi3>>=EooL0L0"(i128*, i128) !dbg !789 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !790, metadata !DIExpression()), !dbg !791
  call void @llvm.dbg.declare(metadata i128* %2, metadata !792, metadata !DIExpression()), !dbg !791
  call void @llvm.dbg.value(metadata i128 %1, metadata !792, metadata !DIExpression()), !dbg !791
  store i128 %1, i128* %2, align 16, !dbg !791
  %3 = load i128, i128* %0, align 16, !dbg !793
  %4 = load i128, i128* %2, align 16, !dbg !793
  %5 = lshr i128 %3, %4, !dbg !793
  call void @llvm.dbg.value(metadata i128 %5, metadata !790, metadata !DIExpression()), !dbg !793
  %6 = load i128, i128* %0, align 16, !dbg !793
  store i128 %5, i128* %0, align 16, !dbg !793
  ret void, !dbg !793
}

define i8 @"_CNW7builtin3opsEXi1&EaaL0L0"(i8, i8) !dbg !794 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !795, metadata !DIExpression()), !dbg !796
  call void @llvm.dbg.value(metadata i8 %0, metadata !795, metadata !DIExpression()), !dbg !796
  store i8 %0, i8* %3, align 1, !dbg !796
  call void @llvm.dbg.declare(metadata i8* %2, metadata !797, metadata !DIExpression()), !dbg !796
  call void @llvm.dbg.value(metadata i8 %1, metadata !797, metadata !DIExpression()), !dbg !796
  store i8 %1, i8* %2, align 1, !dbg !796
  %4 = load i8, i8* %3, align 1, !dbg !798
  %5 = load i8, i8* %2, align 1, !dbg !798
  %6 = and i8 %4, %5, !dbg !798
  ret i8 %6, !dbg !798
}

define void @"_CNW7builtin3opsEXi2&=EaaL0L0"(i8*, i8) !dbg !799 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !800, metadata !DIExpression()), !dbg !801
  call void @llvm.dbg.declare(metadata i8* %2, metadata !802, metadata !DIExpression()), !dbg !801
  call void @llvm.dbg.value(metadata i8 %1, metadata !802, metadata !DIExpression()), !dbg !801
  store i8 %1, i8* %2, align 1, !dbg !801
  %3 = load i8, i8* %0, align 1, !dbg !803
  %4 = load i8, i8* %2, align 1, !dbg !803
  %5 = and i8 %3, %4, !dbg !803
  call void @llvm.dbg.value(metadata i8 %5, metadata !800, metadata !DIExpression()), !dbg !803
  %6 = load i8, i8* %0, align 1, !dbg !803
  store i8 %5, i8* %0, align 1, !dbg !803
  ret void, !dbg !803
}

define i8 @"_CNW7builtin3opsEXi1&EhhL0L0"(i8, i8) !dbg !804 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !805, metadata !DIExpression()), !dbg !806
  call void @llvm.dbg.value(metadata i8 %0, metadata !805, metadata !DIExpression()), !dbg !806
  store i8 %0, i8* %3, align 1, !dbg !806
  call void @llvm.dbg.declare(metadata i8* %2, metadata !807, metadata !DIExpression()), !dbg !806
  call void @llvm.dbg.value(metadata i8 %1, metadata !807, metadata !DIExpression()), !dbg !806
  store i8 %1, i8* %2, align 1, !dbg !806
  %4 = load i8, i8* %3, align 1, !dbg !808
  %5 = load i8, i8* %2, align 1, !dbg !808
  %6 = and i8 %4, %5, !dbg !808
  ret i8 %6, !dbg !808
}

define void @"_CNW7builtin3opsEXi2&=EhhL0L0"(i8*, i8) !dbg !809 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !810, metadata !DIExpression()), !dbg !811
  call void @llvm.dbg.declare(metadata i8* %2, metadata !812, metadata !DIExpression()), !dbg !811
  call void @llvm.dbg.value(metadata i8 %1, metadata !812, metadata !DIExpression()), !dbg !811
  store i8 %1, i8* %2, align 1, !dbg !811
  %3 = load i8, i8* %0, align 1, !dbg !813
  %4 = load i8, i8* %2, align 1, !dbg !813
  %5 = and i8 %3, %4, !dbg !813
  call void @llvm.dbg.value(metadata i8 %5, metadata !810, metadata !DIExpression()), !dbg !813
  %6 = load i8, i8* %0, align 1, !dbg !813
  store i8 %5, i8* %0, align 1, !dbg !813
  ret void, !dbg !813
}

define i16 @"_CNW7builtin3opsEXi1&EssL0L0"(i16, i16) !dbg !814 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !815, metadata !DIExpression()), !dbg !816
  call void @llvm.dbg.value(metadata i16 %0, metadata !815, metadata !DIExpression()), !dbg !816
  store i16 %0, i16* %3, align 2, !dbg !816
  call void @llvm.dbg.declare(metadata i16* %2, metadata !817, metadata !DIExpression()), !dbg !816
  call void @llvm.dbg.value(metadata i16 %1, metadata !817, metadata !DIExpression()), !dbg !816
  store i16 %1, i16* %2, align 2, !dbg !816
  %4 = load i16, i16* %3, align 2, !dbg !818
  %5 = load i16, i16* %2, align 2, !dbg !818
  %6 = and i16 %4, %5, !dbg !818
  ret i16 %6, !dbg !818
}

define void @"_CNW7builtin3opsEXi2&=EssL0L0"(i16*, i16) !dbg !819 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !820, metadata !DIExpression()), !dbg !821
  call void @llvm.dbg.declare(metadata i16* %2, metadata !822, metadata !DIExpression()), !dbg !821
  call void @llvm.dbg.value(metadata i16 %1, metadata !822, metadata !DIExpression()), !dbg !821
  store i16 %1, i16* %2, align 2, !dbg !821
  %3 = load i16, i16* %0, align 2, !dbg !823
  %4 = load i16, i16* %2, align 2, !dbg !823
  %5 = and i16 %3, %4, !dbg !823
  call void @llvm.dbg.value(metadata i16 %5, metadata !820, metadata !DIExpression()), !dbg !823
  %6 = load i16, i16* %0, align 2, !dbg !823
  store i16 %5, i16* %0, align 2, !dbg !823
  ret void, !dbg !823
}

define i16 @"_CNW7builtin3opsEXi1&EttL0L0"(i16, i16) !dbg !824 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !825, metadata !DIExpression()), !dbg !826
  call void @llvm.dbg.value(metadata i16 %0, metadata !825, metadata !DIExpression()), !dbg !826
  store i16 %0, i16* %3, align 2, !dbg !826
  call void @llvm.dbg.declare(metadata i16* %2, metadata !827, metadata !DIExpression()), !dbg !826
  call void @llvm.dbg.value(metadata i16 %1, metadata !827, metadata !DIExpression()), !dbg !826
  store i16 %1, i16* %2, align 2, !dbg !826
  %4 = load i16, i16* %3, align 2, !dbg !828
  %5 = load i16, i16* %2, align 2, !dbg !828
  %6 = and i16 %4, %5, !dbg !828
  ret i16 %6, !dbg !828
}

define void @"_CNW7builtin3opsEXi2&=EttL0L0"(i16*, i16) !dbg !829 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !830, metadata !DIExpression()), !dbg !831
  call void @llvm.dbg.declare(metadata i16* %2, metadata !832, metadata !DIExpression()), !dbg !831
  call void @llvm.dbg.value(metadata i16 %1, metadata !832, metadata !DIExpression()), !dbg !831
  store i16 %1, i16* %2, align 2, !dbg !831
  %3 = load i16, i16* %0, align 2, !dbg !833
  %4 = load i16, i16* %2, align 2, !dbg !833
  %5 = and i16 %3, %4, !dbg !833
  call void @llvm.dbg.value(metadata i16 %5, metadata !830, metadata !DIExpression()), !dbg !833
  %6 = load i16, i16* %0, align 2, !dbg !833
  store i16 %5, i16* %0, align 2, !dbg !833
  ret void, !dbg !833
}

define i32 @"_CNW7builtin3opsEXi1&EiiL0L0"(i32, i32) !dbg !834 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !835, metadata !DIExpression()), !dbg !836
  call void @llvm.dbg.value(metadata i32 %0, metadata !835, metadata !DIExpression()), !dbg !836
  store i32 %0, i32* %3, align 4, !dbg !836
  call void @llvm.dbg.declare(metadata i32* %2, metadata !837, metadata !DIExpression()), !dbg !836
  call void @llvm.dbg.value(metadata i32 %1, metadata !837, metadata !DIExpression()), !dbg !836
  store i32 %1, i32* %2, align 4, !dbg !836
  %4 = load i32, i32* %3, align 4, !dbg !838
  %5 = load i32, i32* %2, align 4, !dbg !838
  %6 = and i32 %4, %5, !dbg !838
  ret i32 %6, !dbg !838
}

define void @"_CNW7builtin3opsEXi2&=EiiL0L0"(i32*, i32) !dbg !839 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !840, metadata !DIExpression()), !dbg !841
  call void @llvm.dbg.declare(metadata i32* %2, metadata !842, metadata !DIExpression()), !dbg !841
  call void @llvm.dbg.value(metadata i32 %1, metadata !842, metadata !DIExpression()), !dbg !841
  store i32 %1, i32* %2, align 4, !dbg !841
  %3 = load i32, i32* %0, align 4, !dbg !843
  %4 = load i32, i32* %2, align 4, !dbg !843
  %5 = and i32 %3, %4, !dbg !843
  call void @llvm.dbg.value(metadata i32 %5, metadata !840, metadata !DIExpression()), !dbg !843
  %6 = load i32, i32* %0, align 4, !dbg !843
  store i32 %5, i32* %0, align 4, !dbg !843
  ret void, !dbg !843
}

define i32 @"_CNW7builtin3opsEXi1&EjjL0L0"(i32, i32) !dbg !844 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !845, metadata !DIExpression()), !dbg !846
  call void @llvm.dbg.value(metadata i32 %0, metadata !845, metadata !DIExpression()), !dbg !846
  store i32 %0, i32* %3, align 4, !dbg !846
  call void @llvm.dbg.declare(metadata i32* %2, metadata !847, metadata !DIExpression()), !dbg !846
  call void @llvm.dbg.value(metadata i32 %1, metadata !847, metadata !DIExpression()), !dbg !846
  store i32 %1, i32* %2, align 4, !dbg !846
  %4 = load i32, i32* %3, align 4, !dbg !848
  %5 = load i32, i32* %2, align 4, !dbg !848
  %6 = and i32 %4, %5, !dbg !848
  ret i32 %6, !dbg !848
}

define void @"_CNW7builtin3opsEXi2&=EjjL0L0"(i32*, i32) !dbg !849 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !850, metadata !DIExpression()), !dbg !851
  call void @llvm.dbg.declare(metadata i32* %2, metadata !852, metadata !DIExpression()), !dbg !851
  call void @llvm.dbg.value(metadata i32 %1, metadata !852, metadata !DIExpression()), !dbg !851
  store i32 %1, i32* %2, align 4, !dbg !851
  %3 = load i32, i32* %0, align 4, !dbg !853
  %4 = load i32, i32* %2, align 4, !dbg !853
  %5 = and i32 %3, %4, !dbg !853
  call void @llvm.dbg.value(metadata i32 %5, metadata !850, metadata !DIExpression()), !dbg !853
  %6 = load i32, i32* %0, align 4, !dbg !853
  store i32 %5, i32* %0, align 4, !dbg !853
  ret void, !dbg !853
}

define i64 @"_CNW7builtin3opsEXi1&EllL0L0"(i64, i64) !dbg !854 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !855, metadata !DIExpression()), !dbg !856
  call void @llvm.dbg.value(metadata i64 %0, metadata !855, metadata !DIExpression()), !dbg !856
  store i64 %0, i64* %3, align 8, !dbg !856
  call void @llvm.dbg.declare(metadata i64* %2, metadata !857, metadata !DIExpression()), !dbg !856
  call void @llvm.dbg.value(metadata i64 %1, metadata !857, metadata !DIExpression()), !dbg !856
  store i64 %1, i64* %2, align 8, !dbg !856
  %4 = load i64, i64* %3, align 8, !dbg !858
  %5 = load i64, i64* %2, align 8, !dbg !858
  %6 = and i64 %4, %5, !dbg !858
  ret i64 %6, !dbg !858
}

define void @"_CNW7builtin3opsEXi2&=EllL0L0"(i64*, i64) !dbg !859 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !860, metadata !DIExpression()), !dbg !861
  call void @llvm.dbg.declare(metadata i64* %2, metadata !862, metadata !DIExpression()), !dbg !861
  call void @llvm.dbg.value(metadata i64 %1, metadata !862, metadata !DIExpression()), !dbg !861
  store i64 %1, i64* %2, align 8, !dbg !861
  %3 = load i64, i64* %0, align 8, !dbg !863
  %4 = load i64, i64* %2, align 8, !dbg !863
  %5 = and i64 %3, %4, !dbg !863
  call void @llvm.dbg.value(metadata i64 %5, metadata !860, metadata !DIExpression()), !dbg !863
  %6 = load i64, i64* %0, align 8, !dbg !863
  store i64 %5, i64* %0, align 8, !dbg !863
  ret void, !dbg !863
}

define i64 @"_CNW7builtin3opsEXi1&EmmL0L0"(i64, i64) !dbg !864 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !865, metadata !DIExpression()), !dbg !866
  call void @llvm.dbg.value(metadata i64 %0, metadata !865, metadata !DIExpression()), !dbg !866
  store i64 %0, i64* %3, align 8, !dbg !866
  call void @llvm.dbg.declare(metadata i64* %2, metadata !867, metadata !DIExpression()), !dbg !866
  call void @llvm.dbg.value(metadata i64 %1, metadata !867, metadata !DIExpression()), !dbg !866
  store i64 %1, i64* %2, align 8, !dbg !866
  %4 = load i64, i64* %3, align 8, !dbg !868
  %5 = load i64, i64* %2, align 8, !dbg !868
  %6 = and i64 %4, %5, !dbg !868
  ret i64 %6, !dbg !868
}

define void @"_CNW7builtin3opsEXi2&=EmmL0L0"(i64*, i64) !dbg !869 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !870, metadata !DIExpression()), !dbg !871
  call void @llvm.dbg.declare(metadata i64* %2, metadata !872, metadata !DIExpression()), !dbg !871
  call void @llvm.dbg.value(metadata i64 %1, metadata !872, metadata !DIExpression()), !dbg !871
  store i64 %1, i64* %2, align 8, !dbg !871
  %3 = load i64, i64* %0, align 8, !dbg !873
  %4 = load i64, i64* %2, align 8, !dbg !873
  %5 = and i64 %3, %4, !dbg !873
  call void @llvm.dbg.value(metadata i64 %5, metadata !870, metadata !DIExpression()), !dbg !873
  %6 = load i64, i64* %0, align 8, !dbg !873
  store i64 %5, i64* %0, align 8, !dbg !873
  ret void, !dbg !873
}

define i128 @"_CNW7builtin3opsEXi1&EnnL0L0"(i128, i128) !dbg !874 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !875, metadata !DIExpression()), !dbg !876
  call void @llvm.dbg.value(metadata i128 %0, metadata !875, metadata !DIExpression()), !dbg !876
  store i128 %0, i128* %3, align 16, !dbg !876
  call void @llvm.dbg.declare(metadata i128* %2, metadata !877, metadata !DIExpression()), !dbg !876
  call void @llvm.dbg.value(metadata i128 %1, metadata !877, metadata !DIExpression()), !dbg !876
  store i128 %1, i128* %2, align 16, !dbg !876
  %4 = load i128, i128* %3, align 16, !dbg !878
  %5 = load i128, i128* %2, align 16, !dbg !878
  %6 = and i128 %4, %5, !dbg !878
  ret i128 %6, !dbg !878
}

define void @"_CNW7builtin3opsEXi2&=EnnL0L0"(i128*, i128) !dbg !879 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !880, metadata !DIExpression()), !dbg !881
  call void @llvm.dbg.declare(metadata i128* %2, metadata !882, metadata !DIExpression()), !dbg !881
  call void @llvm.dbg.value(metadata i128 %1, metadata !882, metadata !DIExpression()), !dbg !881
  store i128 %1, i128* %2, align 16, !dbg !881
  %3 = load i128, i128* %0, align 16, !dbg !883
  %4 = load i128, i128* %2, align 16, !dbg !883
  %5 = and i128 %3, %4, !dbg !883
  call void @llvm.dbg.value(metadata i128 %5, metadata !880, metadata !DIExpression()), !dbg !883
  %6 = load i128, i128* %0, align 16, !dbg !883
  store i128 %5, i128* %0, align 16, !dbg !883
  ret void, !dbg !883
}

define i128 @"_CNW7builtin3opsEXi1&EooL0L0"(i128, i128) !dbg !884 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !885, metadata !DIExpression()), !dbg !886
  call void @llvm.dbg.value(metadata i128 %0, metadata !885, metadata !DIExpression()), !dbg !886
  store i128 %0, i128* %3, align 16, !dbg !886
  call void @llvm.dbg.declare(metadata i128* %2, metadata !887, metadata !DIExpression()), !dbg !886
  call void @llvm.dbg.value(metadata i128 %1, metadata !887, metadata !DIExpression()), !dbg !886
  store i128 %1, i128* %2, align 16, !dbg !886
  %4 = load i128, i128* %3, align 16, !dbg !888
  %5 = load i128, i128* %2, align 16, !dbg !888
  %6 = and i128 %4, %5, !dbg !888
  ret i128 %6, !dbg !888
}

define void @"_CNW7builtin3opsEXi2&=EooL0L0"(i128*, i128) !dbg !889 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !890, metadata !DIExpression()), !dbg !891
  call void @llvm.dbg.declare(metadata i128* %2, metadata !892, metadata !DIExpression()), !dbg !891
  call void @llvm.dbg.value(metadata i128 %1, metadata !892, metadata !DIExpression()), !dbg !891
  store i128 %1, i128* %2, align 16, !dbg !891
  %3 = load i128, i128* %0, align 16, !dbg !893
  %4 = load i128, i128* %2, align 16, !dbg !893
  %5 = and i128 %3, %4, !dbg !893
  call void @llvm.dbg.value(metadata i128 %5, metadata !890, metadata !DIExpression()), !dbg !893
  %6 = load i128, i128* %0, align 16, !dbg !893
  store i128 %5, i128* %0, align 16, !dbg !893
  ret void, !dbg !893
}

define i8 @"_CNW7builtin3opsEXi1|EaaL0L0"(i8, i8) !dbg !894 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !895, metadata !DIExpression()), !dbg !896
  call void @llvm.dbg.value(metadata i8 %0, metadata !895, metadata !DIExpression()), !dbg !896
  store i8 %0, i8* %3, align 1, !dbg !896
  call void @llvm.dbg.declare(metadata i8* %2, metadata !897, metadata !DIExpression()), !dbg !896
  call void @llvm.dbg.value(metadata i8 %1, metadata !897, metadata !DIExpression()), !dbg !896
  store i8 %1, i8* %2, align 1, !dbg !896
  %4 = load i8, i8* %3, align 1, !dbg !898
  %5 = load i8, i8* %2, align 1, !dbg !898
  %6 = or i8 %4, %5, !dbg !898
  ret i8 %6, !dbg !898
}

define void @"_CNW7builtin3opsEXi2|=EaaL0L0"(i8*, i8) !dbg !899 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !900, metadata !DIExpression()), !dbg !901
  call void @llvm.dbg.declare(metadata i8* %2, metadata !902, metadata !DIExpression()), !dbg !901
  call void @llvm.dbg.value(metadata i8 %1, metadata !902, metadata !DIExpression()), !dbg !901
  store i8 %1, i8* %2, align 1, !dbg !901
  %3 = load i8, i8* %0, align 1, !dbg !903
  %4 = load i8, i8* %2, align 1, !dbg !903
  %5 = or i8 %3, %4, !dbg !903
  call void @llvm.dbg.value(metadata i8 %5, metadata !900, metadata !DIExpression()), !dbg !903
  %6 = load i8, i8* %0, align 1, !dbg !903
  store i8 %5, i8* %0, align 1, !dbg !903
  ret void, !dbg !903
}

define i8 @"_CNW7builtin3opsEXi1|EhhL0L0"(i8, i8) !dbg !904 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !905, metadata !DIExpression()), !dbg !906
  call void @llvm.dbg.value(metadata i8 %0, metadata !905, metadata !DIExpression()), !dbg !906
  store i8 %0, i8* %3, align 1, !dbg !906
  call void @llvm.dbg.declare(metadata i8* %2, metadata !907, metadata !DIExpression()), !dbg !906
  call void @llvm.dbg.value(metadata i8 %1, metadata !907, metadata !DIExpression()), !dbg !906
  store i8 %1, i8* %2, align 1, !dbg !906
  %4 = load i8, i8* %3, align 1, !dbg !908
  %5 = load i8, i8* %2, align 1, !dbg !908
  %6 = or i8 %4, %5, !dbg !908
  ret i8 %6, !dbg !908
}

define void @"_CNW7builtin3opsEXi2|=EhhL0L0"(i8*, i8) !dbg !909 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !910, metadata !DIExpression()), !dbg !911
  call void @llvm.dbg.declare(metadata i8* %2, metadata !912, metadata !DIExpression()), !dbg !911
  call void @llvm.dbg.value(metadata i8 %1, metadata !912, metadata !DIExpression()), !dbg !911
  store i8 %1, i8* %2, align 1, !dbg !911
  %3 = load i8, i8* %0, align 1, !dbg !913
  %4 = load i8, i8* %2, align 1, !dbg !913
  %5 = or i8 %3, %4, !dbg !913
  call void @llvm.dbg.value(metadata i8 %5, metadata !910, metadata !DIExpression()), !dbg !913
  %6 = load i8, i8* %0, align 1, !dbg !913
  store i8 %5, i8* %0, align 1, !dbg !913
  ret void, !dbg !913
}

define i16 @"_CNW7builtin3opsEXi1|EssL0L0"(i16, i16) !dbg !914 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !915, metadata !DIExpression()), !dbg !916
  call void @llvm.dbg.value(metadata i16 %0, metadata !915, metadata !DIExpression()), !dbg !916
  store i16 %0, i16* %3, align 2, !dbg !916
  call void @llvm.dbg.declare(metadata i16* %2, metadata !917, metadata !DIExpression()), !dbg !916
  call void @llvm.dbg.value(metadata i16 %1, metadata !917, metadata !DIExpression()), !dbg !916
  store i16 %1, i16* %2, align 2, !dbg !916
  %4 = load i16, i16* %3, align 2, !dbg !918
  %5 = load i16, i16* %2, align 2, !dbg !918
  %6 = or i16 %4, %5, !dbg !918
  ret i16 %6, !dbg !918
}

define void @"_CNW7builtin3opsEXi2|=EssL0L0"(i16*, i16) !dbg !919 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !920, metadata !DIExpression()), !dbg !921
  call void @llvm.dbg.declare(metadata i16* %2, metadata !922, metadata !DIExpression()), !dbg !921
  call void @llvm.dbg.value(metadata i16 %1, metadata !922, metadata !DIExpression()), !dbg !921
  store i16 %1, i16* %2, align 2, !dbg !921
  %3 = load i16, i16* %0, align 2, !dbg !923
  %4 = load i16, i16* %2, align 2, !dbg !923
  %5 = or i16 %3, %4, !dbg !923
  call void @llvm.dbg.value(metadata i16 %5, metadata !920, metadata !DIExpression()), !dbg !923
  %6 = load i16, i16* %0, align 2, !dbg !923
  store i16 %5, i16* %0, align 2, !dbg !923
  ret void, !dbg !923
}

define i16 @"_CNW7builtin3opsEXi1|EttL0L0"(i16, i16) !dbg !924 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !925, metadata !DIExpression()), !dbg !926
  call void @llvm.dbg.value(metadata i16 %0, metadata !925, metadata !DIExpression()), !dbg !926
  store i16 %0, i16* %3, align 2, !dbg !926
  call void @llvm.dbg.declare(metadata i16* %2, metadata !927, metadata !DIExpression()), !dbg !926
  call void @llvm.dbg.value(metadata i16 %1, metadata !927, metadata !DIExpression()), !dbg !926
  store i16 %1, i16* %2, align 2, !dbg !926
  %4 = load i16, i16* %3, align 2, !dbg !928
  %5 = load i16, i16* %2, align 2, !dbg !928
  %6 = or i16 %4, %5, !dbg !928
  ret i16 %6, !dbg !928
}

define void @"_CNW7builtin3opsEXi2|=EttL0L0"(i16*, i16) !dbg !929 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !930, metadata !DIExpression()), !dbg !931
  call void @llvm.dbg.declare(metadata i16* %2, metadata !932, metadata !DIExpression()), !dbg !931
  call void @llvm.dbg.value(metadata i16 %1, metadata !932, metadata !DIExpression()), !dbg !931
  store i16 %1, i16* %2, align 2, !dbg !931
  %3 = load i16, i16* %0, align 2, !dbg !933
  %4 = load i16, i16* %2, align 2, !dbg !933
  %5 = or i16 %3, %4, !dbg !933
  call void @llvm.dbg.value(metadata i16 %5, metadata !930, metadata !DIExpression()), !dbg !933
  %6 = load i16, i16* %0, align 2, !dbg !933
  store i16 %5, i16* %0, align 2, !dbg !933
  ret void, !dbg !933
}

define i32 @"_CNW7builtin3opsEXi1|EiiL0L0"(i32, i32) !dbg !934 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !935, metadata !DIExpression()), !dbg !936
  call void @llvm.dbg.value(metadata i32 %0, metadata !935, metadata !DIExpression()), !dbg !936
  store i32 %0, i32* %3, align 4, !dbg !936
  call void @llvm.dbg.declare(metadata i32* %2, metadata !937, metadata !DIExpression()), !dbg !936
  call void @llvm.dbg.value(metadata i32 %1, metadata !937, metadata !DIExpression()), !dbg !936
  store i32 %1, i32* %2, align 4, !dbg !936
  %4 = load i32, i32* %3, align 4, !dbg !938
  %5 = load i32, i32* %2, align 4, !dbg !938
  %6 = or i32 %4, %5, !dbg !938
  ret i32 %6, !dbg !938
}

define void @"_CNW7builtin3opsEXi2|=EiiL0L0"(i32*, i32) !dbg !939 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !940, metadata !DIExpression()), !dbg !941
  call void @llvm.dbg.declare(metadata i32* %2, metadata !942, metadata !DIExpression()), !dbg !941
  call void @llvm.dbg.value(metadata i32 %1, metadata !942, metadata !DIExpression()), !dbg !941
  store i32 %1, i32* %2, align 4, !dbg !941
  %3 = load i32, i32* %0, align 4, !dbg !943
  %4 = load i32, i32* %2, align 4, !dbg !943
  %5 = or i32 %3, %4, !dbg !943
  call void @llvm.dbg.value(metadata i32 %5, metadata !940, metadata !DIExpression()), !dbg !943
  %6 = load i32, i32* %0, align 4, !dbg !943
  store i32 %5, i32* %0, align 4, !dbg !943
  ret void, !dbg !943
}

define i32 @"_CNW7builtin3opsEXi1|EjjL0L0"(i32, i32) !dbg !944 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !945, metadata !DIExpression()), !dbg !946
  call void @llvm.dbg.value(metadata i32 %0, metadata !945, metadata !DIExpression()), !dbg !946
  store i32 %0, i32* %3, align 4, !dbg !946
  call void @llvm.dbg.declare(metadata i32* %2, metadata !947, metadata !DIExpression()), !dbg !946
  call void @llvm.dbg.value(metadata i32 %1, metadata !947, metadata !DIExpression()), !dbg !946
  store i32 %1, i32* %2, align 4, !dbg !946
  %4 = load i32, i32* %3, align 4, !dbg !948
  %5 = load i32, i32* %2, align 4, !dbg !948
  %6 = or i32 %4, %5, !dbg !948
  ret i32 %6, !dbg !948
}

define void @"_CNW7builtin3opsEXi2|=EjjL0L0"(i32*, i32) !dbg !949 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !950, metadata !DIExpression()), !dbg !951
  call void @llvm.dbg.declare(metadata i32* %2, metadata !952, metadata !DIExpression()), !dbg !951
  call void @llvm.dbg.value(metadata i32 %1, metadata !952, metadata !DIExpression()), !dbg !951
  store i32 %1, i32* %2, align 4, !dbg !951
  %3 = load i32, i32* %0, align 4, !dbg !953
  %4 = load i32, i32* %2, align 4, !dbg !953
  %5 = or i32 %3, %4, !dbg !953
  call void @llvm.dbg.value(metadata i32 %5, metadata !950, metadata !DIExpression()), !dbg !953
  %6 = load i32, i32* %0, align 4, !dbg !953
  store i32 %5, i32* %0, align 4, !dbg !953
  ret void, !dbg !953
}

define i64 @"_CNW7builtin3opsEXi1|EllL0L0"(i64, i64) !dbg !954 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !955, metadata !DIExpression()), !dbg !956
  call void @llvm.dbg.value(metadata i64 %0, metadata !955, metadata !DIExpression()), !dbg !956
  store i64 %0, i64* %3, align 8, !dbg !956
  call void @llvm.dbg.declare(metadata i64* %2, metadata !957, metadata !DIExpression()), !dbg !956
  call void @llvm.dbg.value(metadata i64 %1, metadata !957, metadata !DIExpression()), !dbg !956
  store i64 %1, i64* %2, align 8, !dbg !956
  %4 = load i64, i64* %3, align 8, !dbg !958
  %5 = load i64, i64* %2, align 8, !dbg !958
  %6 = or i64 %4, %5, !dbg !958
  ret i64 %6, !dbg !958
}

define void @"_CNW7builtin3opsEXi2|=EllL0L0"(i64*, i64) !dbg !959 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !960, metadata !DIExpression()), !dbg !961
  call void @llvm.dbg.declare(metadata i64* %2, metadata !962, metadata !DIExpression()), !dbg !961
  call void @llvm.dbg.value(metadata i64 %1, metadata !962, metadata !DIExpression()), !dbg !961
  store i64 %1, i64* %2, align 8, !dbg !961
  %3 = load i64, i64* %0, align 8, !dbg !963
  %4 = load i64, i64* %2, align 8, !dbg !963
  %5 = or i64 %3, %4, !dbg !963
  call void @llvm.dbg.value(metadata i64 %5, metadata !960, metadata !DIExpression()), !dbg !963
  %6 = load i64, i64* %0, align 8, !dbg !963
  store i64 %5, i64* %0, align 8, !dbg !963
  ret void, !dbg !963
}

define i64 @"_CNW7builtin3opsEXi1|EmmL0L0"(i64, i64) !dbg !964 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !965, metadata !DIExpression()), !dbg !966
  call void @llvm.dbg.value(metadata i64 %0, metadata !965, metadata !DIExpression()), !dbg !966
  store i64 %0, i64* %3, align 8, !dbg !966
  call void @llvm.dbg.declare(metadata i64* %2, metadata !967, metadata !DIExpression()), !dbg !966
  call void @llvm.dbg.value(metadata i64 %1, metadata !967, metadata !DIExpression()), !dbg !966
  store i64 %1, i64* %2, align 8, !dbg !966
  %4 = load i64, i64* %3, align 8, !dbg !968
  %5 = load i64, i64* %2, align 8, !dbg !968
  %6 = or i64 %4, %5, !dbg !968
  ret i64 %6, !dbg !968
}

define void @"_CNW7builtin3opsEXi2|=EmmL0L0"(i64*, i64) !dbg !969 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !970, metadata !DIExpression()), !dbg !971
  call void @llvm.dbg.declare(metadata i64* %2, metadata !972, metadata !DIExpression()), !dbg !971
  call void @llvm.dbg.value(metadata i64 %1, metadata !972, metadata !DIExpression()), !dbg !971
  store i64 %1, i64* %2, align 8, !dbg !971
  %3 = load i64, i64* %0, align 8, !dbg !973
  %4 = load i64, i64* %2, align 8, !dbg !973
  %5 = or i64 %3, %4, !dbg !973
  call void @llvm.dbg.value(metadata i64 %5, metadata !970, metadata !DIExpression()), !dbg !973
  %6 = load i64, i64* %0, align 8, !dbg !973
  store i64 %5, i64* %0, align 8, !dbg !973
  ret void, !dbg !973
}

define i128 @"_CNW7builtin3opsEXi1|EnnL0L0"(i128, i128) !dbg !974 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !975, metadata !DIExpression()), !dbg !976
  call void @llvm.dbg.value(metadata i128 %0, metadata !975, metadata !DIExpression()), !dbg !976
  store i128 %0, i128* %3, align 16, !dbg !976
  call void @llvm.dbg.declare(metadata i128* %2, metadata !977, metadata !DIExpression()), !dbg !976
  call void @llvm.dbg.value(metadata i128 %1, metadata !977, metadata !DIExpression()), !dbg !976
  store i128 %1, i128* %2, align 16, !dbg !976
  %4 = load i128, i128* %3, align 16, !dbg !978
  %5 = load i128, i128* %2, align 16, !dbg !978
  %6 = or i128 %4, %5, !dbg !978
  ret i128 %6, !dbg !978
}

define void @"_CNW7builtin3opsEXi2|=EnnL0L0"(i128*, i128) !dbg !979 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !980, metadata !DIExpression()), !dbg !981
  call void @llvm.dbg.declare(metadata i128* %2, metadata !982, metadata !DIExpression()), !dbg !981
  call void @llvm.dbg.value(metadata i128 %1, metadata !982, metadata !DIExpression()), !dbg !981
  store i128 %1, i128* %2, align 16, !dbg !981
  %3 = load i128, i128* %0, align 16, !dbg !983
  %4 = load i128, i128* %2, align 16, !dbg !983
  %5 = or i128 %3, %4, !dbg !983
  call void @llvm.dbg.value(metadata i128 %5, metadata !980, metadata !DIExpression()), !dbg !983
  %6 = load i128, i128* %0, align 16, !dbg !983
  store i128 %5, i128* %0, align 16, !dbg !983
  ret void, !dbg !983
}

define i128 @"_CNW7builtin3opsEXi1|EooL0L0"(i128, i128) !dbg !984 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !985, metadata !DIExpression()), !dbg !986
  call void @llvm.dbg.value(metadata i128 %0, metadata !985, metadata !DIExpression()), !dbg !986
  store i128 %0, i128* %3, align 16, !dbg !986
  call void @llvm.dbg.declare(metadata i128* %2, metadata !987, metadata !DIExpression()), !dbg !986
  call void @llvm.dbg.value(metadata i128 %1, metadata !987, metadata !DIExpression()), !dbg !986
  store i128 %1, i128* %2, align 16, !dbg !986
  %4 = load i128, i128* %3, align 16, !dbg !988
  %5 = load i128, i128* %2, align 16, !dbg !988
  %6 = or i128 %4, %5, !dbg !988
  ret i128 %6, !dbg !988
}

define void @"_CNW7builtin3opsEXi2|=EooL0L0"(i128*, i128) !dbg !989 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !990, metadata !DIExpression()), !dbg !991
  call void @llvm.dbg.declare(metadata i128* %2, metadata !992, metadata !DIExpression()), !dbg !991
  call void @llvm.dbg.value(metadata i128 %1, metadata !992, metadata !DIExpression()), !dbg !991
  store i128 %1, i128* %2, align 16, !dbg !991
  %3 = load i128, i128* %0, align 16, !dbg !993
  %4 = load i128, i128* %2, align 16, !dbg !993
  %5 = or i128 %3, %4, !dbg !993
  call void @llvm.dbg.value(metadata i128 %5, metadata !990, metadata !DIExpression()), !dbg !993
  %6 = load i128, i128* %0, align 16, !dbg !993
  store i128 %5, i128* %0, align 16, !dbg !993
  ret void, !dbg !993
}

define i8 @"_CNW7builtin3opsEXi1^EaaL0L0"(i8, i8) !dbg !994 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !995, metadata !DIExpression()), !dbg !996
  call void @llvm.dbg.value(metadata i8 %0, metadata !995, metadata !DIExpression()), !dbg !996
  store i8 %0, i8* %3, align 1, !dbg !996
  call void @llvm.dbg.declare(metadata i8* %2, metadata !997, metadata !DIExpression()), !dbg !996
  call void @llvm.dbg.value(metadata i8 %1, metadata !997, metadata !DIExpression()), !dbg !996
  store i8 %1, i8* %2, align 1, !dbg !996
  %4 = load i8, i8* %3, align 1, !dbg !998
  %5 = load i8, i8* %2, align 1, !dbg !998
  %6 = xor i8 %4, %5, !dbg !998
  ret i8 %6, !dbg !998
}

define void @"_CNW7builtin3opsEXi2^=EaaL0L0"(i8*, i8) !dbg !999 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !1000, metadata !DIExpression()), !dbg !1001
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1002, metadata !DIExpression()), !dbg !1001
  call void @llvm.dbg.value(metadata i8 %1, metadata !1002, metadata !DIExpression()), !dbg !1001
  store i8 %1, i8* %2, align 1, !dbg !1001
  %3 = load i8, i8* %0, align 1, !dbg !1003
  %4 = load i8, i8* %2, align 1, !dbg !1003
  %5 = xor i8 %3, %4, !dbg !1003
  call void @llvm.dbg.value(metadata i8 %5, metadata !1000, metadata !DIExpression()), !dbg !1003
  %6 = load i8, i8* %0, align 1, !dbg !1003
  store i8 %5, i8* %0, align 1, !dbg !1003
  ret void, !dbg !1003
}

define i8 @"_CNW7builtin3opsEXi1^EhhL0L0"(i8, i8) !dbg !1004 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !1005, metadata !DIExpression()), !dbg !1006
  call void @llvm.dbg.value(metadata i8 %0, metadata !1005, metadata !DIExpression()), !dbg !1006
  store i8 %0, i8* %3, align 1, !dbg !1006
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1007, metadata !DIExpression()), !dbg !1006
  call void @llvm.dbg.value(metadata i8 %1, metadata !1007, metadata !DIExpression()), !dbg !1006
  store i8 %1, i8* %2, align 1, !dbg !1006
  %4 = load i8, i8* %3, align 1, !dbg !1008
  %5 = load i8, i8* %2, align 1, !dbg !1008
  %6 = xor i8 %4, %5, !dbg !1008
  ret i8 %6, !dbg !1008
}

define void @"_CNW7builtin3opsEXi2^=EhhL0L0"(i8*, i8) !dbg !1009 {
entry:
  %2 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %0, metadata !1010, metadata !DIExpression()), !dbg !1011
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1012, metadata !DIExpression()), !dbg !1011
  call void @llvm.dbg.value(metadata i8 %1, metadata !1012, metadata !DIExpression()), !dbg !1011
  store i8 %1, i8* %2, align 1, !dbg !1011
  %3 = load i8, i8* %0, align 1, !dbg !1013
  %4 = load i8, i8* %2, align 1, !dbg !1013
  %5 = xor i8 %3, %4, !dbg !1013
  call void @llvm.dbg.value(metadata i8 %5, metadata !1010, metadata !DIExpression()), !dbg !1013
  %6 = load i8, i8* %0, align 1, !dbg !1013
  store i8 %5, i8* %0, align 1, !dbg !1013
  ret void, !dbg !1013
}

define i16 @"_CNW7builtin3opsEXi1^EssL0L0"(i16, i16) !dbg !1014 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !1015, metadata !DIExpression()), !dbg !1016
  call void @llvm.dbg.value(metadata i16 %0, metadata !1015, metadata !DIExpression()), !dbg !1016
  store i16 %0, i16* %3, align 2, !dbg !1016
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1017, metadata !DIExpression()), !dbg !1016
  call void @llvm.dbg.value(metadata i16 %1, metadata !1017, metadata !DIExpression()), !dbg !1016
  store i16 %1, i16* %2, align 2, !dbg !1016
  %4 = load i16, i16* %3, align 2, !dbg !1018
  %5 = load i16, i16* %2, align 2, !dbg !1018
  %6 = xor i16 %4, %5, !dbg !1018
  ret i16 %6, !dbg !1018
}

define void @"_CNW7builtin3opsEXi2^=EssL0L0"(i16*, i16) !dbg !1019 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !1020, metadata !DIExpression()), !dbg !1021
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1022, metadata !DIExpression()), !dbg !1021
  call void @llvm.dbg.value(metadata i16 %1, metadata !1022, metadata !DIExpression()), !dbg !1021
  store i16 %1, i16* %2, align 2, !dbg !1021
  %3 = load i16, i16* %0, align 2, !dbg !1023
  %4 = load i16, i16* %2, align 2, !dbg !1023
  %5 = xor i16 %3, %4, !dbg !1023
  call void @llvm.dbg.value(metadata i16 %5, metadata !1020, metadata !DIExpression()), !dbg !1023
  %6 = load i16, i16* %0, align 2, !dbg !1023
  store i16 %5, i16* %0, align 2, !dbg !1023
  ret void, !dbg !1023
}

define i16 @"_CNW7builtin3opsEXi1^EttL0L0"(i16, i16) !dbg !1024 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !1025, metadata !DIExpression()), !dbg !1026
  call void @llvm.dbg.value(metadata i16 %0, metadata !1025, metadata !DIExpression()), !dbg !1026
  store i16 %0, i16* %3, align 2, !dbg !1026
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1027, metadata !DIExpression()), !dbg !1026
  call void @llvm.dbg.value(metadata i16 %1, metadata !1027, metadata !DIExpression()), !dbg !1026
  store i16 %1, i16* %2, align 2, !dbg !1026
  %4 = load i16, i16* %3, align 2, !dbg !1028
  %5 = load i16, i16* %2, align 2, !dbg !1028
  %6 = xor i16 %4, %5, !dbg !1028
  ret i16 %6, !dbg !1028
}

define void @"_CNW7builtin3opsEXi2^=EttL0L0"(i16*, i16) !dbg !1029 {
entry:
  %2 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %0, metadata !1030, metadata !DIExpression()), !dbg !1031
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1032, metadata !DIExpression()), !dbg !1031
  call void @llvm.dbg.value(metadata i16 %1, metadata !1032, metadata !DIExpression()), !dbg !1031
  store i16 %1, i16* %2, align 2, !dbg !1031
  %3 = load i16, i16* %0, align 2, !dbg !1033
  %4 = load i16, i16* %2, align 2, !dbg !1033
  %5 = xor i16 %3, %4, !dbg !1033
  call void @llvm.dbg.value(metadata i16 %5, metadata !1030, metadata !DIExpression()), !dbg !1033
  %6 = load i16, i16* %0, align 2, !dbg !1033
  store i16 %5, i16* %0, align 2, !dbg !1033
  ret void, !dbg !1033
}

define i32 @"_CNW7builtin3opsEXi1^EiiL0L0"(i32, i32) !dbg !1034 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !1035, metadata !DIExpression()), !dbg !1036
  call void @llvm.dbg.value(metadata i32 %0, metadata !1035, metadata !DIExpression()), !dbg !1036
  store i32 %0, i32* %3, align 4, !dbg !1036
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1037, metadata !DIExpression()), !dbg !1036
  call void @llvm.dbg.value(metadata i32 %1, metadata !1037, metadata !DIExpression()), !dbg !1036
  store i32 %1, i32* %2, align 4, !dbg !1036
  %4 = load i32, i32* %3, align 4, !dbg !1038
  %5 = load i32, i32* %2, align 4, !dbg !1038
  %6 = xor i32 %4, %5, !dbg !1038
  ret i32 %6, !dbg !1038
}

define void @"_CNW7builtin3opsEXi2^=EiiL0L0"(i32*, i32) !dbg !1039 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !1040, metadata !DIExpression()), !dbg !1041
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1042, metadata !DIExpression()), !dbg !1041
  call void @llvm.dbg.value(metadata i32 %1, metadata !1042, metadata !DIExpression()), !dbg !1041
  store i32 %1, i32* %2, align 4, !dbg !1041
  %3 = load i32, i32* %0, align 4, !dbg !1043
  %4 = load i32, i32* %2, align 4, !dbg !1043
  %5 = xor i32 %3, %4, !dbg !1043
  call void @llvm.dbg.value(metadata i32 %5, metadata !1040, metadata !DIExpression()), !dbg !1043
  %6 = load i32, i32* %0, align 4, !dbg !1043
  store i32 %5, i32* %0, align 4, !dbg !1043
  ret void, !dbg !1043
}

define i32 @"_CNW7builtin3opsEXi1^EjjL0L0"(i32, i32) !dbg !1044 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !1045, metadata !DIExpression()), !dbg !1046
  call void @llvm.dbg.value(metadata i32 %0, metadata !1045, metadata !DIExpression()), !dbg !1046
  store i32 %0, i32* %3, align 4, !dbg !1046
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1047, metadata !DIExpression()), !dbg !1046
  call void @llvm.dbg.value(metadata i32 %1, metadata !1047, metadata !DIExpression()), !dbg !1046
  store i32 %1, i32* %2, align 4, !dbg !1046
  %4 = load i32, i32* %3, align 4, !dbg !1048
  %5 = load i32, i32* %2, align 4, !dbg !1048
  %6 = xor i32 %4, %5, !dbg !1048
  ret i32 %6, !dbg !1048
}

define void @"_CNW7builtin3opsEXi2^=EjjL0L0"(i32*, i32) !dbg !1049 {
entry:
  %2 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %0, metadata !1050, metadata !DIExpression()), !dbg !1051
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1052, metadata !DIExpression()), !dbg !1051
  call void @llvm.dbg.value(metadata i32 %1, metadata !1052, metadata !DIExpression()), !dbg !1051
  store i32 %1, i32* %2, align 4, !dbg !1051
  %3 = load i32, i32* %0, align 4, !dbg !1053
  %4 = load i32, i32* %2, align 4, !dbg !1053
  %5 = xor i32 %3, %4, !dbg !1053
  call void @llvm.dbg.value(metadata i32 %5, metadata !1050, metadata !DIExpression()), !dbg !1053
  %6 = load i32, i32* %0, align 4, !dbg !1053
  store i32 %5, i32* %0, align 4, !dbg !1053
  ret void, !dbg !1053
}

define i64 @"_CNW7builtin3opsEXi1^EllL0L0"(i64, i64) !dbg !1054 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1055, metadata !DIExpression()), !dbg !1056
  call void @llvm.dbg.value(metadata i64 %0, metadata !1055, metadata !DIExpression()), !dbg !1056
  store i64 %0, i64* %3, align 8, !dbg !1056
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1057, metadata !DIExpression()), !dbg !1056
  call void @llvm.dbg.value(metadata i64 %1, metadata !1057, metadata !DIExpression()), !dbg !1056
  store i64 %1, i64* %2, align 8, !dbg !1056
  %4 = load i64, i64* %3, align 8, !dbg !1058
  %5 = load i64, i64* %2, align 8, !dbg !1058
  %6 = xor i64 %4, %5, !dbg !1058
  ret i64 %6, !dbg !1058
}

define void @"_CNW7builtin3opsEXi2^=EllL0L0"(i64*, i64) !dbg !1059 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !1060, metadata !DIExpression()), !dbg !1061
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1062, metadata !DIExpression()), !dbg !1061
  call void @llvm.dbg.value(metadata i64 %1, metadata !1062, metadata !DIExpression()), !dbg !1061
  store i64 %1, i64* %2, align 8, !dbg !1061
  %3 = load i64, i64* %0, align 8, !dbg !1063
  %4 = load i64, i64* %2, align 8, !dbg !1063
  %5 = xor i64 %3, %4, !dbg !1063
  call void @llvm.dbg.value(metadata i64 %5, metadata !1060, metadata !DIExpression()), !dbg !1063
  %6 = load i64, i64* %0, align 8, !dbg !1063
  store i64 %5, i64* %0, align 8, !dbg !1063
  ret void, !dbg !1063
}

define i64 @"_CNW7builtin3opsEXi1^EmmL0L0"(i64, i64) !dbg !1064 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1065, metadata !DIExpression()), !dbg !1066
  call void @llvm.dbg.value(metadata i64 %0, metadata !1065, metadata !DIExpression()), !dbg !1066
  store i64 %0, i64* %3, align 8, !dbg !1066
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1067, metadata !DIExpression()), !dbg !1066
  call void @llvm.dbg.value(metadata i64 %1, metadata !1067, metadata !DIExpression()), !dbg !1066
  store i64 %1, i64* %2, align 8, !dbg !1066
  %4 = load i64, i64* %3, align 8, !dbg !1068
  %5 = load i64, i64* %2, align 8, !dbg !1068
  %6 = xor i64 %4, %5, !dbg !1068
  ret i64 %6, !dbg !1068
}

define void @"_CNW7builtin3opsEXi2^=EmmL0L0"(i64*, i64) !dbg !1069 {
entry:
  %2 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %0, metadata !1070, metadata !DIExpression()), !dbg !1071
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1072, metadata !DIExpression()), !dbg !1071
  call void @llvm.dbg.value(metadata i64 %1, metadata !1072, metadata !DIExpression()), !dbg !1071
  store i64 %1, i64* %2, align 8, !dbg !1071
  %3 = load i64, i64* %0, align 8, !dbg !1073
  %4 = load i64, i64* %2, align 8, !dbg !1073
  %5 = xor i64 %3, %4, !dbg !1073
  call void @llvm.dbg.value(metadata i64 %5, metadata !1070, metadata !DIExpression()), !dbg !1073
  %6 = load i64, i64* %0, align 8, !dbg !1073
  store i64 %5, i64* %0, align 8, !dbg !1073
  ret void, !dbg !1073
}

define i128 @"_CNW7builtin3opsEXi1^EnnL0L0"(i128, i128) !dbg !1074 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !1075, metadata !DIExpression()), !dbg !1076
  call void @llvm.dbg.value(metadata i128 %0, metadata !1075, metadata !DIExpression()), !dbg !1076
  store i128 %0, i128* %3, align 16, !dbg !1076
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1077, metadata !DIExpression()), !dbg !1076
  call void @llvm.dbg.value(metadata i128 %1, metadata !1077, metadata !DIExpression()), !dbg !1076
  store i128 %1, i128* %2, align 16, !dbg !1076
  %4 = load i128, i128* %3, align 16, !dbg !1078
  %5 = load i128, i128* %2, align 16, !dbg !1078
  %6 = xor i128 %4, %5, !dbg !1078
  ret i128 %6, !dbg !1078
}

define void @"_CNW7builtin3opsEXi2^=EnnL0L0"(i128*, i128) !dbg !1079 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !1080, metadata !DIExpression()), !dbg !1081
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1082, metadata !DIExpression()), !dbg !1081
  call void @llvm.dbg.value(metadata i128 %1, metadata !1082, metadata !DIExpression()), !dbg !1081
  store i128 %1, i128* %2, align 16, !dbg !1081
  %3 = load i128, i128* %0, align 16, !dbg !1083
  %4 = load i128, i128* %2, align 16, !dbg !1083
  %5 = xor i128 %3, %4, !dbg !1083
  call void @llvm.dbg.value(metadata i128 %5, metadata !1080, metadata !DIExpression()), !dbg !1083
  %6 = load i128, i128* %0, align 16, !dbg !1083
  store i128 %5, i128* %0, align 16, !dbg !1083
  ret void, !dbg !1083
}

define i128 @"_CNW7builtin3opsEXi1^EooL0L0"(i128, i128) !dbg !1084 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !1085, metadata !DIExpression()), !dbg !1086
  call void @llvm.dbg.value(metadata i128 %0, metadata !1085, metadata !DIExpression()), !dbg !1086
  store i128 %0, i128* %3, align 16, !dbg !1086
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1087, metadata !DIExpression()), !dbg !1086
  call void @llvm.dbg.value(metadata i128 %1, metadata !1087, metadata !DIExpression()), !dbg !1086
  store i128 %1, i128* %2, align 16, !dbg !1086
  %4 = load i128, i128* %3, align 16, !dbg !1088
  %5 = load i128, i128* %2, align 16, !dbg !1088
  %6 = xor i128 %4, %5, !dbg !1088
  ret i128 %6, !dbg !1088
}

define void @"_CNW7builtin3opsEXi2^=EooL0L0"(i128*, i128) !dbg !1089 {
entry:
  %2 = alloca i128, align 16
  call void @llvm.dbg.declare(metadata i128* %0, metadata !1090, metadata !DIExpression()), !dbg !1091
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1092, metadata !DIExpression()), !dbg !1091
  call void @llvm.dbg.value(metadata i128 %1, metadata !1092, metadata !DIExpression()), !dbg !1091
  store i128 %1, i128* %2, align 16, !dbg !1091
  %3 = load i128, i128* %0, align 16, !dbg !1093
  %4 = load i128, i128* %2, align 16, !dbg !1093
  %5 = xor i128 %3, %4, !dbg !1093
  call void @llvm.dbg.value(metadata i128 %5, metadata !1090, metadata !DIExpression()), !dbg !1093
  %6 = load i128, i128* %0, align 16, !dbg !1093
  store i128 %5, i128* %0, align 16, !dbg !1093
  ret void, !dbg !1093
}

define i1 @"_CNW7builtin3opsEXi2==EaaL0L0"(i8, i8) !dbg !1094 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !1095, metadata !DIExpression()), !dbg !1096
  call void @llvm.dbg.value(metadata i8 %0, metadata !1095, metadata !DIExpression()), !dbg !1096
  store i8 %0, i8* %3, align 1, !dbg !1096
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1097, metadata !DIExpression()), !dbg !1096
  call void @llvm.dbg.value(metadata i8 %1, metadata !1097, metadata !DIExpression()), !dbg !1096
  store i8 %1, i8* %2, align 1, !dbg !1096
  %4 = load i8, i8* %3, align 1, !dbg !1098
  %5 = load i8, i8* %2, align 1, !dbg !1098
  %6 = icmp eq i8 %4, %5, !dbg !1098
  ret i1 %6, !dbg !1098
}

define i1 @"_CNW7builtin3opsEXi2==EhhL0L0"(i8, i8) !dbg !1099 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !1100, metadata !DIExpression()), !dbg !1101
  call void @llvm.dbg.value(metadata i8 %0, metadata !1100, metadata !DIExpression()), !dbg !1101
  store i8 %0, i8* %3, align 1, !dbg !1101
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1102, metadata !DIExpression()), !dbg !1101
  call void @llvm.dbg.value(metadata i8 %1, metadata !1102, metadata !DIExpression()), !dbg !1101
  store i8 %1, i8* %2, align 1, !dbg !1101
  %4 = load i8, i8* %3, align 1, !dbg !1103
  %5 = load i8, i8* %2, align 1, !dbg !1103
  %6 = icmp eq i8 %4, %5, !dbg !1103
  ret i1 %6, !dbg !1103
}

define i1 @"_CNW7builtin3opsEXi2==EssL0L0"(i16, i16) !dbg !1104 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !1105, metadata !DIExpression()), !dbg !1106
  call void @llvm.dbg.value(metadata i16 %0, metadata !1105, metadata !DIExpression()), !dbg !1106
  store i16 %0, i16* %3, align 2, !dbg !1106
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1107, metadata !DIExpression()), !dbg !1106
  call void @llvm.dbg.value(metadata i16 %1, metadata !1107, metadata !DIExpression()), !dbg !1106
  store i16 %1, i16* %2, align 2, !dbg !1106
  %4 = load i16, i16* %3, align 2, !dbg !1108
  %5 = load i16, i16* %2, align 2, !dbg !1108
  %6 = icmp eq i16 %4, %5, !dbg !1108
  ret i1 %6, !dbg !1108
}

define i1 @"_CNW7builtin3opsEXi2==EttL0L0"(i16, i16) !dbg !1109 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !1110, metadata !DIExpression()), !dbg !1111
  call void @llvm.dbg.value(metadata i16 %0, metadata !1110, metadata !DIExpression()), !dbg !1111
  store i16 %0, i16* %3, align 2, !dbg !1111
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1112, metadata !DIExpression()), !dbg !1111
  call void @llvm.dbg.value(metadata i16 %1, metadata !1112, metadata !DIExpression()), !dbg !1111
  store i16 %1, i16* %2, align 2, !dbg !1111
  %4 = load i16, i16* %3, align 2, !dbg !1113
  %5 = load i16, i16* %2, align 2, !dbg !1113
  %6 = icmp eq i16 %4, %5, !dbg !1113
  ret i1 %6, !dbg !1113
}

define i1 @"_CNW7builtin3opsEXi2==EiiL0L0"(i32, i32) !dbg !1114 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !1115, metadata !DIExpression()), !dbg !1116
  call void @llvm.dbg.value(metadata i32 %0, metadata !1115, metadata !DIExpression()), !dbg !1116
  store i32 %0, i32* %3, align 4, !dbg !1116
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1117, metadata !DIExpression()), !dbg !1116
  call void @llvm.dbg.value(metadata i32 %1, metadata !1117, metadata !DIExpression()), !dbg !1116
  store i32 %1, i32* %2, align 4, !dbg !1116
  %4 = load i32, i32* %3, align 4, !dbg !1118
  %5 = load i32, i32* %2, align 4, !dbg !1118
  %6 = icmp eq i32 %4, %5, !dbg !1118
  ret i1 %6, !dbg !1118
}

define i1 @"_CNW7builtin3opsEXi2==EjjL0L0"(i32, i32) !dbg !1119 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !1120, metadata !DIExpression()), !dbg !1121
  call void @llvm.dbg.value(metadata i32 %0, metadata !1120, metadata !DIExpression()), !dbg !1121
  store i32 %0, i32* %3, align 4, !dbg !1121
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1122, metadata !DIExpression()), !dbg !1121
  call void @llvm.dbg.value(metadata i32 %1, metadata !1122, metadata !DIExpression()), !dbg !1121
  store i32 %1, i32* %2, align 4, !dbg !1121
  %4 = load i32, i32* %3, align 4, !dbg !1123
  %5 = load i32, i32* %2, align 4, !dbg !1123
  %6 = icmp eq i32 %4, %5, !dbg !1123
  ret i1 %6, !dbg !1123
}

define i1 @"_CNW7builtin3opsEXi2==EllL0L0"(i64, i64) !dbg !1124 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1125, metadata !DIExpression()), !dbg !1126
  call void @llvm.dbg.value(metadata i64 %0, metadata !1125, metadata !DIExpression()), !dbg !1126
  store i64 %0, i64* %3, align 8, !dbg !1126
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1127, metadata !DIExpression()), !dbg !1126
  call void @llvm.dbg.value(metadata i64 %1, metadata !1127, metadata !DIExpression()), !dbg !1126
  store i64 %1, i64* %2, align 8, !dbg !1126
  %4 = load i64, i64* %3, align 8, !dbg !1128
  %5 = load i64, i64* %2, align 8, !dbg !1128
  %6 = icmp eq i64 %4, %5, !dbg !1128
  ret i1 %6, !dbg !1128
}

define i1 @"_CNW7builtin3opsEXi2==EmmL0L0"(i64, i64) !dbg !1129 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1130, metadata !DIExpression()), !dbg !1131
  call void @llvm.dbg.value(metadata i64 %0, metadata !1130, metadata !DIExpression()), !dbg !1131
  store i64 %0, i64* %3, align 8, !dbg !1131
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1132, metadata !DIExpression()), !dbg !1131
  call void @llvm.dbg.value(metadata i64 %1, metadata !1132, metadata !DIExpression()), !dbg !1131
  store i64 %1, i64* %2, align 8, !dbg !1131
  %4 = load i64, i64* %3, align 8, !dbg !1133
  %5 = load i64, i64* %2, align 8, !dbg !1133
  %6 = icmp eq i64 %4, %5, !dbg !1133
  ret i1 %6, !dbg !1133
}

define i1 @"_CNW7builtin3opsEXi2==EnnL0L0"(i128, i128) !dbg !1134 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !1135, metadata !DIExpression()), !dbg !1136
  call void @llvm.dbg.value(metadata i128 %0, metadata !1135, metadata !DIExpression()), !dbg !1136
  store i128 %0, i128* %3, align 16, !dbg !1136
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1137, metadata !DIExpression()), !dbg !1136
  call void @llvm.dbg.value(metadata i128 %1, metadata !1137, metadata !DIExpression()), !dbg !1136
  store i128 %1, i128* %2, align 16, !dbg !1136
  %4 = load i128, i128* %3, align 16, !dbg !1138
  %5 = load i128, i128* %2, align 16, !dbg !1138
  %6 = icmp eq i128 %4, %5, !dbg !1138
  ret i1 %6, !dbg !1138
}

define i1 @"_CNW7builtin3opsEXi2==EooL0L0"(i128, i128) !dbg !1139 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !1140, metadata !DIExpression()), !dbg !1141
  call void @llvm.dbg.value(metadata i128 %0, metadata !1140, metadata !DIExpression()), !dbg !1141
  store i128 %0, i128* %3, align 16, !dbg !1141
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1142, metadata !DIExpression()), !dbg !1141
  call void @llvm.dbg.value(metadata i128 %1, metadata !1142, metadata !DIExpression()), !dbg !1141
  store i128 %1, i128* %2, align 16, !dbg !1141
  %4 = load i128, i128* %3, align 16, !dbg !1143
  %5 = load i128, i128* %2, align 16, !dbg !1143
  %6 = icmp eq i128 %4, %5, !dbg !1143
  ret i1 %6, !dbg !1143
}

define i1 @"_CNW7builtin3opsEXi2!=EaaL0L0"(i8, i8) !dbg !1144 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !1145, metadata !DIExpression()), !dbg !1146
  call void @llvm.dbg.value(metadata i8 %0, metadata !1145, metadata !DIExpression()), !dbg !1146
  store i8 %0, i8* %3, align 1, !dbg !1146
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1147, metadata !DIExpression()), !dbg !1146
  call void @llvm.dbg.value(metadata i8 %1, metadata !1147, metadata !DIExpression()), !dbg !1146
  store i8 %1, i8* %2, align 1, !dbg !1146
  %4 = load i8, i8* %3, align 1, !dbg !1148
  %5 = load i8, i8* %2, align 1, !dbg !1148
  %6 = icmp ne i8 %4, %5, !dbg !1148
  ret i1 %6, !dbg !1148
}

define i1 @"_CNW7builtin3opsEXi2!=EhhL0L0"(i8, i8) !dbg !1149 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !1150, metadata !DIExpression()), !dbg !1151
  call void @llvm.dbg.value(metadata i8 %0, metadata !1150, metadata !DIExpression()), !dbg !1151
  store i8 %0, i8* %3, align 1, !dbg !1151
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1152, metadata !DIExpression()), !dbg !1151
  call void @llvm.dbg.value(metadata i8 %1, metadata !1152, metadata !DIExpression()), !dbg !1151
  store i8 %1, i8* %2, align 1, !dbg !1151
  %4 = load i8, i8* %3, align 1, !dbg !1153
  %5 = load i8, i8* %2, align 1, !dbg !1153
  %6 = icmp ne i8 %4, %5, !dbg !1153
  ret i1 %6, !dbg !1153
}

define i1 @"_CNW7builtin3opsEXi2!=EssL0L0"(i16, i16) !dbg !1154 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !1155, metadata !DIExpression()), !dbg !1156
  call void @llvm.dbg.value(metadata i16 %0, metadata !1155, metadata !DIExpression()), !dbg !1156
  store i16 %0, i16* %3, align 2, !dbg !1156
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1157, metadata !DIExpression()), !dbg !1156
  call void @llvm.dbg.value(metadata i16 %1, metadata !1157, metadata !DIExpression()), !dbg !1156
  store i16 %1, i16* %2, align 2, !dbg !1156
  %4 = load i16, i16* %3, align 2, !dbg !1158
  %5 = load i16, i16* %2, align 2, !dbg !1158
  %6 = icmp ne i16 %4, %5, !dbg !1158
  ret i1 %6, !dbg !1158
}

define i1 @"_CNW7builtin3opsEXi2!=EttL0L0"(i16, i16) !dbg !1159 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !1160, metadata !DIExpression()), !dbg !1161
  call void @llvm.dbg.value(metadata i16 %0, metadata !1160, metadata !DIExpression()), !dbg !1161
  store i16 %0, i16* %3, align 2, !dbg !1161
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1162, metadata !DIExpression()), !dbg !1161
  call void @llvm.dbg.value(metadata i16 %1, metadata !1162, metadata !DIExpression()), !dbg !1161
  store i16 %1, i16* %2, align 2, !dbg !1161
  %4 = load i16, i16* %3, align 2, !dbg !1163
  %5 = load i16, i16* %2, align 2, !dbg !1163
  %6 = icmp ne i16 %4, %5, !dbg !1163
  ret i1 %6, !dbg !1163
}

define i1 @"_CNW7builtin3opsEXi2!=EiiL0L0"(i32, i32) !dbg !1164 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !1165, metadata !DIExpression()), !dbg !1166
  call void @llvm.dbg.value(metadata i32 %0, metadata !1165, metadata !DIExpression()), !dbg !1166
  store i32 %0, i32* %3, align 4, !dbg !1166
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1167, metadata !DIExpression()), !dbg !1166
  call void @llvm.dbg.value(metadata i32 %1, metadata !1167, metadata !DIExpression()), !dbg !1166
  store i32 %1, i32* %2, align 4, !dbg !1166
  %4 = load i32, i32* %3, align 4, !dbg !1168
  %5 = load i32, i32* %2, align 4, !dbg !1168
  %6 = icmp ne i32 %4, %5, !dbg !1168
  ret i1 %6, !dbg !1168
}

define i1 @"_CNW7builtin3opsEXi2!=EjjL0L0"(i32, i32) !dbg !1169 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !1170, metadata !DIExpression()), !dbg !1171
  call void @llvm.dbg.value(metadata i32 %0, metadata !1170, metadata !DIExpression()), !dbg !1171
  store i32 %0, i32* %3, align 4, !dbg !1171
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1172, metadata !DIExpression()), !dbg !1171
  call void @llvm.dbg.value(metadata i32 %1, metadata !1172, metadata !DIExpression()), !dbg !1171
  store i32 %1, i32* %2, align 4, !dbg !1171
  %4 = load i32, i32* %3, align 4, !dbg !1173
  %5 = load i32, i32* %2, align 4, !dbg !1173
  %6 = icmp ne i32 %4, %5, !dbg !1173
  ret i1 %6, !dbg !1173
}

define i1 @"_CNW7builtin3opsEXi2!=EllL0L0"(i64, i64) !dbg !1174 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1175, metadata !DIExpression()), !dbg !1176
  call void @llvm.dbg.value(metadata i64 %0, metadata !1175, metadata !DIExpression()), !dbg !1176
  store i64 %0, i64* %3, align 8, !dbg !1176
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1177, metadata !DIExpression()), !dbg !1176
  call void @llvm.dbg.value(metadata i64 %1, metadata !1177, metadata !DIExpression()), !dbg !1176
  store i64 %1, i64* %2, align 8, !dbg !1176
  %4 = load i64, i64* %3, align 8, !dbg !1178
  %5 = load i64, i64* %2, align 8, !dbg !1178
  %6 = icmp ne i64 %4, %5, !dbg !1178
  ret i1 %6, !dbg !1178
}

define i1 @"_CNW7builtin3opsEXi2!=EmmL0L0"(i64, i64) !dbg !1179 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1180, metadata !DIExpression()), !dbg !1181
  call void @llvm.dbg.value(metadata i64 %0, metadata !1180, metadata !DIExpression()), !dbg !1181
  store i64 %0, i64* %3, align 8, !dbg !1181
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1182, metadata !DIExpression()), !dbg !1181
  call void @llvm.dbg.value(metadata i64 %1, metadata !1182, metadata !DIExpression()), !dbg !1181
  store i64 %1, i64* %2, align 8, !dbg !1181
  %4 = load i64, i64* %3, align 8, !dbg !1183
  %5 = load i64, i64* %2, align 8, !dbg !1183
  %6 = icmp ne i64 %4, %5, !dbg !1183
  ret i1 %6, !dbg !1183
}

define i1 @"_CNW7builtin3opsEXi2!=EnnL0L0"(i128, i128) !dbg !1184 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !1185, metadata !DIExpression()), !dbg !1186
  call void @llvm.dbg.value(metadata i128 %0, metadata !1185, metadata !DIExpression()), !dbg !1186
  store i128 %0, i128* %3, align 16, !dbg !1186
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1187, metadata !DIExpression()), !dbg !1186
  call void @llvm.dbg.value(metadata i128 %1, metadata !1187, metadata !DIExpression()), !dbg !1186
  store i128 %1, i128* %2, align 16, !dbg !1186
  %4 = load i128, i128* %3, align 16, !dbg !1188
  %5 = load i128, i128* %2, align 16, !dbg !1188
  %6 = icmp ne i128 %4, %5, !dbg !1188
  ret i1 %6, !dbg !1188
}

define i1 @"_CNW7builtin3opsEXi2!=EooL0L0"(i128, i128) !dbg !1189 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !1190, metadata !DIExpression()), !dbg !1191
  call void @llvm.dbg.value(metadata i128 %0, metadata !1190, metadata !DIExpression()), !dbg !1191
  store i128 %0, i128* %3, align 16, !dbg !1191
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1192, metadata !DIExpression()), !dbg !1191
  call void @llvm.dbg.value(metadata i128 %1, metadata !1192, metadata !DIExpression()), !dbg !1191
  store i128 %1, i128* %2, align 16, !dbg !1191
  %4 = load i128, i128* %3, align 16, !dbg !1193
  %5 = load i128, i128* %2, align 16, !dbg !1193
  %6 = icmp ne i128 %4, %5, !dbg !1193
  ret i1 %6, !dbg !1193
}

define i1 @"_CNW7builtin3opsEXi2>=EaaL0L0"(i8, i8) !dbg !1194 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !1195, metadata !DIExpression()), !dbg !1196
  call void @llvm.dbg.value(metadata i8 %0, metadata !1195, metadata !DIExpression()), !dbg !1196
  store i8 %0, i8* %3, align 1, !dbg !1196
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1197, metadata !DIExpression()), !dbg !1196
  call void @llvm.dbg.value(metadata i8 %1, metadata !1197, metadata !DIExpression()), !dbg !1196
  store i8 %1, i8* %2, align 1, !dbg !1196
  %4 = load i8, i8* %3, align 1, !dbg !1198
  %5 = load i8, i8* %2, align 1, !dbg !1198
  %6 = icmp sge i8 %4, %5, !dbg !1198
  ret i1 %6, !dbg !1198
}

define i1 @"_CNW7builtin3opsEXi2>=EssL0L0"(i16, i16) !dbg !1199 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !1200, metadata !DIExpression()), !dbg !1201
  call void @llvm.dbg.value(metadata i16 %0, metadata !1200, metadata !DIExpression()), !dbg !1201
  store i16 %0, i16* %3, align 2, !dbg !1201
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1202, metadata !DIExpression()), !dbg !1201
  call void @llvm.dbg.value(metadata i16 %1, metadata !1202, metadata !DIExpression()), !dbg !1201
  store i16 %1, i16* %2, align 2, !dbg !1201
  %4 = load i16, i16* %3, align 2, !dbg !1203
  %5 = load i16, i16* %2, align 2, !dbg !1203
  %6 = icmp sge i16 %4, %5, !dbg !1203
  ret i1 %6, !dbg !1203
}

define i1 @"_CNW7builtin3opsEXi2>=EiiL0L0"(i32, i32) !dbg !1204 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !1205, metadata !DIExpression()), !dbg !1206
  call void @llvm.dbg.value(metadata i32 %0, metadata !1205, metadata !DIExpression()), !dbg !1206
  store i32 %0, i32* %3, align 4, !dbg !1206
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1207, metadata !DIExpression()), !dbg !1206
  call void @llvm.dbg.value(metadata i32 %1, metadata !1207, metadata !DIExpression()), !dbg !1206
  store i32 %1, i32* %2, align 4, !dbg !1206
  %4 = load i32, i32* %3, align 4, !dbg !1208
  %5 = load i32, i32* %2, align 4, !dbg !1208
  %6 = icmp sge i32 %4, %5, !dbg !1208
  ret i1 %6, !dbg !1208
}

define i1 @"_CNW7builtin3opsEXi2>=EllL0L0"(i64, i64) !dbg !1209 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1210, metadata !DIExpression()), !dbg !1211
  call void @llvm.dbg.value(metadata i64 %0, metadata !1210, metadata !DIExpression()), !dbg !1211
  store i64 %0, i64* %3, align 8, !dbg !1211
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1212, metadata !DIExpression()), !dbg !1211
  call void @llvm.dbg.value(metadata i64 %1, metadata !1212, metadata !DIExpression()), !dbg !1211
  store i64 %1, i64* %2, align 8, !dbg !1211
  %4 = load i64, i64* %3, align 8, !dbg !1213
  %5 = load i64, i64* %2, align 8, !dbg !1213
  %6 = icmp sge i64 %4, %5, !dbg !1213
  ret i1 %6, !dbg !1213
}

define i1 @"_CNW7builtin3opsEXi2>=EnnL0L0"(i128, i128) !dbg !1214 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !1215, metadata !DIExpression()), !dbg !1216
  call void @llvm.dbg.value(metadata i128 %0, metadata !1215, metadata !DIExpression()), !dbg !1216
  store i128 %0, i128* %3, align 16, !dbg !1216
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1217, metadata !DIExpression()), !dbg !1216
  call void @llvm.dbg.value(metadata i128 %1, metadata !1217, metadata !DIExpression()), !dbg !1216
  store i128 %1, i128* %2, align 16, !dbg !1216
  %4 = load i128, i128* %3, align 16, !dbg !1218
  %5 = load i128, i128* %2, align 16, !dbg !1218
  %6 = icmp sge i128 %4, %5, !dbg !1218
  ret i1 %6, !dbg !1218
}

define i1 @"_CNW7builtin3opsEXi1>EaaL0L0"(i8, i8) !dbg !1219 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !1220, metadata !DIExpression()), !dbg !1221
  call void @llvm.dbg.value(metadata i8 %0, metadata !1220, metadata !DIExpression()), !dbg !1221
  store i8 %0, i8* %3, align 1, !dbg !1221
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1222, metadata !DIExpression()), !dbg !1221
  call void @llvm.dbg.value(metadata i8 %1, metadata !1222, metadata !DIExpression()), !dbg !1221
  store i8 %1, i8* %2, align 1, !dbg !1221
  %4 = load i8, i8* %3, align 1, !dbg !1223
  %5 = load i8, i8* %2, align 1, !dbg !1223
  %6 = icmp sgt i8 %4, %5, !dbg !1223
  ret i1 %6, !dbg !1223
}

define i1 @"_CNW7builtin3opsEXi1>EssL0L0"(i16, i16) !dbg !1224 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !1225, metadata !DIExpression()), !dbg !1226
  call void @llvm.dbg.value(metadata i16 %0, metadata !1225, metadata !DIExpression()), !dbg !1226
  store i16 %0, i16* %3, align 2, !dbg !1226
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1227, metadata !DIExpression()), !dbg !1226
  call void @llvm.dbg.value(metadata i16 %1, metadata !1227, metadata !DIExpression()), !dbg !1226
  store i16 %1, i16* %2, align 2, !dbg !1226
  %4 = load i16, i16* %3, align 2, !dbg !1228
  %5 = load i16, i16* %2, align 2, !dbg !1228
  %6 = icmp sgt i16 %4, %5, !dbg !1228
  ret i1 %6, !dbg !1228
}

define i1 @"_CNW7builtin3opsEXi1>EiiL0L0"(i32, i32) !dbg !1229 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !1230, metadata !DIExpression()), !dbg !1231
  call void @llvm.dbg.value(metadata i32 %0, metadata !1230, metadata !DIExpression()), !dbg !1231
  store i32 %0, i32* %3, align 4, !dbg !1231
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1232, metadata !DIExpression()), !dbg !1231
  call void @llvm.dbg.value(metadata i32 %1, metadata !1232, metadata !DIExpression()), !dbg !1231
  store i32 %1, i32* %2, align 4, !dbg !1231
  %4 = load i32, i32* %3, align 4, !dbg !1233
  %5 = load i32, i32* %2, align 4, !dbg !1233
  %6 = icmp sgt i32 %4, %5, !dbg !1233
  ret i1 %6, !dbg !1233
}

define i1 @"_CNW7builtin3opsEXi1>EllL0L0"(i64, i64) !dbg !1234 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1235, metadata !DIExpression()), !dbg !1236
  call void @llvm.dbg.value(metadata i64 %0, metadata !1235, metadata !DIExpression()), !dbg !1236
  store i64 %0, i64* %3, align 8, !dbg !1236
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1237, metadata !DIExpression()), !dbg !1236
  call void @llvm.dbg.value(metadata i64 %1, metadata !1237, metadata !DIExpression()), !dbg !1236
  store i64 %1, i64* %2, align 8, !dbg !1236
  %4 = load i64, i64* %3, align 8, !dbg !1238
  %5 = load i64, i64* %2, align 8, !dbg !1238
  %6 = icmp sgt i64 %4, %5, !dbg !1238
  ret i1 %6, !dbg !1238
}

define i1 @"_CNW7builtin3opsEXi1>EnnL0L0"(i128, i128) !dbg !1239 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !1240, metadata !DIExpression()), !dbg !1241
  call void @llvm.dbg.value(metadata i128 %0, metadata !1240, metadata !DIExpression()), !dbg !1241
  store i128 %0, i128* %3, align 16, !dbg !1241
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1242, metadata !DIExpression()), !dbg !1241
  call void @llvm.dbg.value(metadata i128 %1, metadata !1242, metadata !DIExpression()), !dbg !1241
  store i128 %1, i128* %2, align 16, !dbg !1241
  %4 = load i128, i128* %3, align 16, !dbg !1243
  %5 = load i128, i128* %2, align 16, !dbg !1243
  %6 = icmp sgt i128 %4, %5, !dbg !1243
  ret i1 %6, !dbg !1243
}

define i1 @"_CNW7builtin3opsEXi2<=EaaL0L0"(i8, i8) !dbg !1244 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !1245, metadata !DIExpression()), !dbg !1246
  call void @llvm.dbg.value(metadata i8 %0, metadata !1245, metadata !DIExpression()), !dbg !1246
  store i8 %0, i8* %3, align 1, !dbg !1246
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1247, metadata !DIExpression()), !dbg !1246
  call void @llvm.dbg.value(metadata i8 %1, metadata !1247, metadata !DIExpression()), !dbg !1246
  store i8 %1, i8* %2, align 1, !dbg !1246
  %4 = load i8, i8* %3, align 1, !dbg !1248
  %5 = load i8, i8* %2, align 1, !dbg !1248
  %6 = icmp sle i8 %4, %5, !dbg !1248
  ret i1 %6, !dbg !1248
}

define i1 @"_CNW7builtin3opsEXi2<=EssL0L0"(i16, i16) !dbg !1249 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !1250, metadata !DIExpression()), !dbg !1251
  call void @llvm.dbg.value(metadata i16 %0, metadata !1250, metadata !DIExpression()), !dbg !1251
  store i16 %0, i16* %3, align 2, !dbg !1251
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1252, metadata !DIExpression()), !dbg !1251
  call void @llvm.dbg.value(metadata i16 %1, metadata !1252, metadata !DIExpression()), !dbg !1251
  store i16 %1, i16* %2, align 2, !dbg !1251
  %4 = load i16, i16* %3, align 2, !dbg !1253
  %5 = load i16, i16* %2, align 2, !dbg !1253
  %6 = icmp sle i16 %4, %5, !dbg !1253
  ret i1 %6, !dbg !1253
}

define i1 @"_CNW7builtin3opsEXi2<=EiiL0L0"(i32, i32) !dbg !1254 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !1255, metadata !DIExpression()), !dbg !1256
  call void @llvm.dbg.value(metadata i32 %0, metadata !1255, metadata !DIExpression()), !dbg !1256
  store i32 %0, i32* %3, align 4, !dbg !1256
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1257, metadata !DIExpression()), !dbg !1256
  call void @llvm.dbg.value(metadata i32 %1, metadata !1257, metadata !DIExpression()), !dbg !1256
  store i32 %1, i32* %2, align 4, !dbg !1256
  %4 = load i32, i32* %3, align 4, !dbg !1258
  %5 = load i32, i32* %2, align 4, !dbg !1258
  %6 = icmp sle i32 %4, %5, !dbg !1258
  ret i1 %6, !dbg !1258
}

define i1 @"_CNW7builtin3opsEXi2<=EllL0L0"(i64, i64) !dbg !1259 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1260, metadata !DIExpression()), !dbg !1261
  call void @llvm.dbg.value(metadata i64 %0, metadata !1260, metadata !DIExpression()), !dbg !1261
  store i64 %0, i64* %3, align 8, !dbg !1261
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1262, metadata !DIExpression()), !dbg !1261
  call void @llvm.dbg.value(metadata i64 %1, metadata !1262, metadata !DIExpression()), !dbg !1261
  store i64 %1, i64* %2, align 8, !dbg !1261
  %4 = load i64, i64* %3, align 8, !dbg !1263
  %5 = load i64, i64* %2, align 8, !dbg !1263
  %6 = icmp sle i64 %4, %5, !dbg !1263
  ret i1 %6, !dbg !1263
}

define i1 @"_CNW7builtin3opsEXi2<=EnnL0L0"(i128, i128) !dbg !1264 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !1265, metadata !DIExpression()), !dbg !1266
  call void @llvm.dbg.value(metadata i128 %0, metadata !1265, metadata !DIExpression()), !dbg !1266
  store i128 %0, i128* %3, align 16, !dbg !1266
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1267, metadata !DIExpression()), !dbg !1266
  call void @llvm.dbg.value(metadata i128 %1, metadata !1267, metadata !DIExpression()), !dbg !1266
  store i128 %1, i128* %2, align 16, !dbg !1266
  %4 = load i128, i128* %3, align 16, !dbg !1268
  %5 = load i128, i128* %2, align 16, !dbg !1268
  %6 = icmp sle i128 %4, %5, !dbg !1268
  ret i1 %6, !dbg !1268
}

define i1 @"_CNW7builtin3opsEXi1<EaaL0L0"(i8, i8) !dbg !1269 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !1270, metadata !DIExpression()), !dbg !1271
  call void @llvm.dbg.value(metadata i8 %0, metadata !1270, metadata !DIExpression()), !dbg !1271
  store i8 %0, i8* %3, align 1, !dbg !1271
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1272, metadata !DIExpression()), !dbg !1271
  call void @llvm.dbg.value(metadata i8 %1, metadata !1272, metadata !DIExpression()), !dbg !1271
  store i8 %1, i8* %2, align 1, !dbg !1271
  %4 = load i8, i8* %3, align 1, !dbg !1273
  %5 = load i8, i8* %2, align 1, !dbg !1273
  %6 = icmp slt i8 %4, %5, !dbg !1273
  ret i1 %6, !dbg !1273
}

define i1 @"_CNW7builtin3opsEXi1<EssL0L0"(i16, i16) !dbg !1274 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !1275, metadata !DIExpression()), !dbg !1276
  call void @llvm.dbg.value(metadata i16 %0, metadata !1275, metadata !DIExpression()), !dbg !1276
  store i16 %0, i16* %3, align 2, !dbg !1276
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1277, metadata !DIExpression()), !dbg !1276
  call void @llvm.dbg.value(metadata i16 %1, metadata !1277, metadata !DIExpression()), !dbg !1276
  store i16 %1, i16* %2, align 2, !dbg !1276
  %4 = load i16, i16* %3, align 2, !dbg !1278
  %5 = load i16, i16* %2, align 2, !dbg !1278
  %6 = icmp slt i16 %4, %5, !dbg !1278
  ret i1 %6, !dbg !1278
}

define i1 @"_CNW7builtin3opsEXi1<EiiL0L0"(i32, i32) !dbg !1279 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !1280, metadata !DIExpression()), !dbg !1281
  call void @llvm.dbg.value(metadata i32 %0, metadata !1280, metadata !DIExpression()), !dbg !1281
  store i32 %0, i32* %3, align 4, !dbg !1281
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1282, metadata !DIExpression()), !dbg !1281
  call void @llvm.dbg.value(metadata i32 %1, metadata !1282, metadata !DIExpression()), !dbg !1281
  store i32 %1, i32* %2, align 4, !dbg !1281
  %4 = load i32, i32* %3, align 4, !dbg !1283
  %5 = load i32, i32* %2, align 4, !dbg !1283
  %6 = icmp slt i32 %4, %5, !dbg !1283
  ret i1 %6, !dbg !1283
}

define i1 @"_CNW7builtin3opsEXi1<EllL0L0"(i64, i64) !dbg !1284 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1285, metadata !DIExpression()), !dbg !1286
  call void @llvm.dbg.value(metadata i64 %0, metadata !1285, metadata !DIExpression()), !dbg !1286
  store i64 %0, i64* %3, align 8, !dbg !1286
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1287, metadata !DIExpression()), !dbg !1286
  call void @llvm.dbg.value(metadata i64 %1, metadata !1287, metadata !DIExpression()), !dbg !1286
  store i64 %1, i64* %2, align 8, !dbg !1286
  %4 = load i64, i64* %3, align 8, !dbg !1288
  %5 = load i64, i64* %2, align 8, !dbg !1288
  %6 = icmp slt i64 %4, %5, !dbg !1288
  ret i1 %6, !dbg !1288
}

define i1 @"_CNW7builtin3opsEXi1<EnnL0L0"(i128, i128) !dbg !1289 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !1290, metadata !DIExpression()), !dbg !1291
  call void @llvm.dbg.value(metadata i128 %0, metadata !1290, metadata !DIExpression()), !dbg !1291
  store i128 %0, i128* %3, align 16, !dbg !1291
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1292, metadata !DIExpression()), !dbg !1291
  call void @llvm.dbg.value(metadata i128 %1, metadata !1292, metadata !DIExpression()), !dbg !1291
  store i128 %1, i128* %2, align 16, !dbg !1291
  %4 = load i128, i128* %3, align 16, !dbg !1293
  %5 = load i128, i128* %2, align 16, !dbg !1293
  %6 = icmp slt i128 %4, %5, !dbg !1293
  ret i1 %6, !dbg !1293
}

define i1 @"_CNW7builtin3opsEXi2>=EhhL0L0"(i8, i8) !dbg !1294 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !1295, metadata !DIExpression()), !dbg !1296
  call void @llvm.dbg.value(metadata i8 %0, metadata !1295, metadata !DIExpression()), !dbg !1296
  store i8 %0, i8* %3, align 1, !dbg !1296
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1297, metadata !DIExpression()), !dbg !1296
  call void @llvm.dbg.value(metadata i8 %1, metadata !1297, metadata !DIExpression()), !dbg !1296
  store i8 %1, i8* %2, align 1, !dbg !1296
  %4 = load i8, i8* %3, align 1, !dbg !1298
  %5 = load i8, i8* %2, align 1, !dbg !1298
  %6 = icmp uge i8 %4, %5, !dbg !1298
  ret i1 %6, !dbg !1298
}

define i1 @"_CNW7builtin3opsEXi2>=EttL0L0"(i16, i16) !dbg !1299 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !1300, metadata !DIExpression()), !dbg !1301
  call void @llvm.dbg.value(metadata i16 %0, metadata !1300, metadata !DIExpression()), !dbg !1301
  store i16 %0, i16* %3, align 2, !dbg !1301
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1302, metadata !DIExpression()), !dbg !1301
  call void @llvm.dbg.value(metadata i16 %1, metadata !1302, metadata !DIExpression()), !dbg !1301
  store i16 %1, i16* %2, align 2, !dbg !1301
  %4 = load i16, i16* %3, align 2, !dbg !1303
  %5 = load i16, i16* %2, align 2, !dbg !1303
  %6 = icmp uge i16 %4, %5, !dbg !1303
  ret i1 %6, !dbg !1303
}

define i1 @"_CNW7builtin3opsEXi2>=EjjL0L0"(i32, i32) !dbg !1304 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !1305, metadata !DIExpression()), !dbg !1306
  call void @llvm.dbg.value(metadata i32 %0, metadata !1305, metadata !DIExpression()), !dbg !1306
  store i32 %0, i32* %3, align 4, !dbg !1306
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1307, metadata !DIExpression()), !dbg !1306
  call void @llvm.dbg.value(metadata i32 %1, metadata !1307, metadata !DIExpression()), !dbg !1306
  store i32 %1, i32* %2, align 4, !dbg !1306
  %4 = load i32, i32* %3, align 4, !dbg !1308
  %5 = load i32, i32* %2, align 4, !dbg !1308
  %6 = icmp uge i32 %4, %5, !dbg !1308
  ret i1 %6, !dbg !1308
}

define i1 @"_CNW7builtin3opsEXi2>=EmmL0L0"(i64, i64) !dbg !1309 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1310, metadata !DIExpression()), !dbg !1311
  call void @llvm.dbg.value(metadata i64 %0, metadata !1310, metadata !DIExpression()), !dbg !1311
  store i64 %0, i64* %3, align 8, !dbg !1311
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1312, metadata !DIExpression()), !dbg !1311
  call void @llvm.dbg.value(metadata i64 %1, metadata !1312, metadata !DIExpression()), !dbg !1311
  store i64 %1, i64* %2, align 8, !dbg !1311
  %4 = load i64, i64* %3, align 8, !dbg !1313
  %5 = load i64, i64* %2, align 8, !dbg !1313
  %6 = icmp uge i64 %4, %5, !dbg !1313
  ret i1 %6, !dbg !1313
}

define i1 @"_CNW7builtin3opsEXi2>=EooL0L0"(i128, i128) !dbg !1314 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !1315, metadata !DIExpression()), !dbg !1316
  call void @llvm.dbg.value(metadata i128 %0, metadata !1315, metadata !DIExpression()), !dbg !1316
  store i128 %0, i128* %3, align 16, !dbg !1316
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1317, metadata !DIExpression()), !dbg !1316
  call void @llvm.dbg.value(metadata i128 %1, metadata !1317, metadata !DIExpression()), !dbg !1316
  store i128 %1, i128* %2, align 16, !dbg !1316
  %4 = load i128, i128* %3, align 16, !dbg !1318
  %5 = load i128, i128* %2, align 16, !dbg !1318
  %6 = icmp uge i128 %4, %5, !dbg !1318
  ret i1 %6, !dbg !1318
}

define i1 @"_CNW7builtin3opsEXi1>EhhL0L0"(i8, i8) !dbg !1319 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !1320, metadata !DIExpression()), !dbg !1321
  call void @llvm.dbg.value(metadata i8 %0, metadata !1320, metadata !DIExpression()), !dbg !1321
  store i8 %0, i8* %3, align 1, !dbg !1321
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1322, metadata !DIExpression()), !dbg !1321
  call void @llvm.dbg.value(metadata i8 %1, metadata !1322, metadata !DIExpression()), !dbg !1321
  store i8 %1, i8* %2, align 1, !dbg !1321
  %4 = load i8, i8* %3, align 1, !dbg !1323
  %5 = load i8, i8* %2, align 1, !dbg !1323
  %6 = icmp ugt i8 %4, %5, !dbg !1323
  ret i1 %6, !dbg !1323
}

define i1 @"_CNW7builtin3opsEXi1>EttL0L0"(i16, i16) !dbg !1324 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !1325, metadata !DIExpression()), !dbg !1326
  call void @llvm.dbg.value(metadata i16 %0, metadata !1325, metadata !DIExpression()), !dbg !1326
  store i16 %0, i16* %3, align 2, !dbg !1326
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1327, metadata !DIExpression()), !dbg !1326
  call void @llvm.dbg.value(metadata i16 %1, metadata !1327, metadata !DIExpression()), !dbg !1326
  store i16 %1, i16* %2, align 2, !dbg !1326
  %4 = load i16, i16* %3, align 2, !dbg !1328
  %5 = load i16, i16* %2, align 2, !dbg !1328
  %6 = icmp ugt i16 %4, %5, !dbg !1328
  ret i1 %6, !dbg !1328
}

define i1 @"_CNW7builtin3opsEXi1>EjjL0L0"(i32, i32) !dbg !1329 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !1330, metadata !DIExpression()), !dbg !1331
  call void @llvm.dbg.value(metadata i32 %0, metadata !1330, metadata !DIExpression()), !dbg !1331
  store i32 %0, i32* %3, align 4, !dbg !1331
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1332, metadata !DIExpression()), !dbg !1331
  call void @llvm.dbg.value(metadata i32 %1, metadata !1332, metadata !DIExpression()), !dbg !1331
  store i32 %1, i32* %2, align 4, !dbg !1331
  %4 = load i32, i32* %3, align 4, !dbg !1333
  %5 = load i32, i32* %2, align 4, !dbg !1333
  %6 = icmp ugt i32 %4, %5, !dbg !1333
  ret i1 %6, !dbg !1333
}

define i1 @"_CNW7builtin3opsEXi1>EmmL0L0"(i64, i64) !dbg !1334 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1335, metadata !DIExpression()), !dbg !1336
  call void @llvm.dbg.value(metadata i64 %0, metadata !1335, metadata !DIExpression()), !dbg !1336
  store i64 %0, i64* %3, align 8, !dbg !1336
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1337, metadata !DIExpression()), !dbg !1336
  call void @llvm.dbg.value(metadata i64 %1, metadata !1337, metadata !DIExpression()), !dbg !1336
  store i64 %1, i64* %2, align 8, !dbg !1336
  %4 = load i64, i64* %3, align 8, !dbg !1338
  %5 = load i64, i64* %2, align 8, !dbg !1338
  %6 = icmp ugt i64 %4, %5, !dbg !1338
  ret i1 %6, !dbg !1338
}

define i1 @"_CNW7builtin3opsEXi1>EooL0L0"(i128, i128) !dbg !1339 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !1340, metadata !DIExpression()), !dbg !1341
  call void @llvm.dbg.value(metadata i128 %0, metadata !1340, metadata !DIExpression()), !dbg !1341
  store i128 %0, i128* %3, align 16, !dbg !1341
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1342, metadata !DIExpression()), !dbg !1341
  call void @llvm.dbg.value(metadata i128 %1, metadata !1342, metadata !DIExpression()), !dbg !1341
  store i128 %1, i128* %2, align 16, !dbg !1341
  %4 = load i128, i128* %3, align 16, !dbg !1343
  %5 = load i128, i128* %2, align 16, !dbg !1343
  %6 = icmp ugt i128 %4, %5, !dbg !1343
  ret i1 %6, !dbg !1343
}

define i1 @"_CNW7builtin3opsEXi2<=EhhL0L0"(i8, i8) !dbg !1344 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !1345, metadata !DIExpression()), !dbg !1346
  call void @llvm.dbg.value(metadata i8 %0, metadata !1345, metadata !DIExpression()), !dbg !1346
  store i8 %0, i8* %3, align 1, !dbg !1346
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1347, metadata !DIExpression()), !dbg !1346
  call void @llvm.dbg.value(metadata i8 %1, metadata !1347, metadata !DIExpression()), !dbg !1346
  store i8 %1, i8* %2, align 1, !dbg !1346
  %4 = load i8, i8* %3, align 1, !dbg !1348
  %5 = load i8, i8* %2, align 1, !dbg !1348
  %6 = icmp ule i8 %4, %5, !dbg !1348
  ret i1 %6, !dbg !1348
}

define i1 @"_CNW7builtin3opsEXi2<=EttL0L0"(i16, i16) !dbg !1349 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !1350, metadata !DIExpression()), !dbg !1351
  call void @llvm.dbg.value(metadata i16 %0, metadata !1350, metadata !DIExpression()), !dbg !1351
  store i16 %0, i16* %3, align 2, !dbg !1351
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1352, metadata !DIExpression()), !dbg !1351
  call void @llvm.dbg.value(metadata i16 %1, metadata !1352, metadata !DIExpression()), !dbg !1351
  store i16 %1, i16* %2, align 2, !dbg !1351
  %4 = load i16, i16* %3, align 2, !dbg !1353
  %5 = load i16, i16* %2, align 2, !dbg !1353
  %6 = icmp ule i16 %4, %5, !dbg !1353
  ret i1 %6, !dbg !1353
}

define i1 @"_CNW7builtin3opsEXi2<=EjjL0L0"(i32, i32) !dbg !1354 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !1355, metadata !DIExpression()), !dbg !1356
  call void @llvm.dbg.value(metadata i32 %0, metadata !1355, metadata !DIExpression()), !dbg !1356
  store i32 %0, i32* %3, align 4, !dbg !1356
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1357, metadata !DIExpression()), !dbg !1356
  call void @llvm.dbg.value(metadata i32 %1, metadata !1357, metadata !DIExpression()), !dbg !1356
  store i32 %1, i32* %2, align 4, !dbg !1356
  %4 = load i32, i32* %3, align 4, !dbg !1358
  %5 = load i32, i32* %2, align 4, !dbg !1358
  %6 = icmp ule i32 %4, %5, !dbg !1358
  ret i1 %6, !dbg !1358
}

define i1 @"_CNW7builtin3opsEXi2<=EmmL0L0"(i64, i64) !dbg !1359 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1360, metadata !DIExpression()), !dbg !1361
  call void @llvm.dbg.value(metadata i64 %0, metadata !1360, metadata !DIExpression()), !dbg !1361
  store i64 %0, i64* %3, align 8, !dbg !1361
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1362, metadata !DIExpression()), !dbg !1361
  call void @llvm.dbg.value(metadata i64 %1, metadata !1362, metadata !DIExpression()), !dbg !1361
  store i64 %1, i64* %2, align 8, !dbg !1361
  %4 = load i64, i64* %3, align 8, !dbg !1363
  %5 = load i64, i64* %2, align 8, !dbg !1363
  %6 = icmp ule i64 %4, %5, !dbg !1363
  ret i1 %6, !dbg !1363
}

define i1 @"_CNW7builtin3opsEXi2<=EooL0L0"(i128, i128) !dbg !1364 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !1365, metadata !DIExpression()), !dbg !1366
  call void @llvm.dbg.value(metadata i128 %0, metadata !1365, metadata !DIExpression()), !dbg !1366
  store i128 %0, i128* %3, align 16, !dbg !1366
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1367, metadata !DIExpression()), !dbg !1366
  call void @llvm.dbg.value(metadata i128 %1, metadata !1367, metadata !DIExpression()), !dbg !1366
  store i128 %1, i128* %2, align 16, !dbg !1366
  %4 = load i128, i128* %3, align 16, !dbg !1368
  %5 = load i128, i128* %2, align 16, !dbg !1368
  %6 = icmp ule i128 %4, %5, !dbg !1368
  ret i1 %6, !dbg !1368
}

define i1 @"_CNW7builtin3opsEXi1<EhhL0L0"(i8, i8) !dbg !1369 {
alloca_block:
  %2 = alloca i8, align 1
  %3 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %3, metadata !1370, metadata !DIExpression()), !dbg !1371
  call void @llvm.dbg.value(metadata i8 %0, metadata !1370, metadata !DIExpression()), !dbg !1371
  store i8 %0, i8* %3, align 1, !dbg !1371
  call void @llvm.dbg.declare(metadata i8* %2, metadata !1372, metadata !DIExpression()), !dbg !1371
  call void @llvm.dbg.value(metadata i8 %1, metadata !1372, metadata !DIExpression()), !dbg !1371
  store i8 %1, i8* %2, align 1, !dbg !1371
  %4 = load i8, i8* %3, align 1, !dbg !1373
  %5 = load i8, i8* %2, align 1, !dbg !1373
  %6 = icmp ult i8 %4, %5, !dbg !1373
  ret i1 %6, !dbg !1373
}

define i1 @"_CNW7builtin3opsEXi1<EttL0L0"(i16, i16) !dbg !1374 {
alloca_block:
  %2 = alloca i16, align 2
  %3 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %3, metadata !1375, metadata !DIExpression()), !dbg !1376
  call void @llvm.dbg.value(metadata i16 %0, metadata !1375, metadata !DIExpression()), !dbg !1376
  store i16 %0, i16* %3, align 2, !dbg !1376
  call void @llvm.dbg.declare(metadata i16* %2, metadata !1377, metadata !DIExpression()), !dbg !1376
  call void @llvm.dbg.value(metadata i16 %1, metadata !1377, metadata !DIExpression()), !dbg !1376
  store i16 %1, i16* %2, align 2, !dbg !1376
  %4 = load i16, i16* %3, align 2, !dbg !1378
  %5 = load i16, i16* %2, align 2, !dbg !1378
  %6 = icmp ult i16 %4, %5, !dbg !1378
  ret i1 %6, !dbg !1378
}

define i1 @"_CNW7builtin3opsEXi1<EjjL0L0"(i32, i32) !dbg !1379 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %3, metadata !1380, metadata !DIExpression()), !dbg !1381
  call void @llvm.dbg.value(metadata i32 %0, metadata !1380, metadata !DIExpression()), !dbg !1381
  store i32 %0, i32* %3, align 4, !dbg !1381
  call void @llvm.dbg.declare(metadata i32* %2, metadata !1382, metadata !DIExpression()), !dbg !1381
  call void @llvm.dbg.value(metadata i32 %1, metadata !1382, metadata !DIExpression()), !dbg !1381
  store i32 %1, i32* %2, align 4, !dbg !1381
  %4 = load i32, i32* %3, align 4, !dbg !1383
  %5 = load i32, i32* %2, align 4, !dbg !1383
  %6 = icmp ult i32 %4, %5, !dbg !1383
  ret i1 %6, !dbg !1383
}

define i1 @"_CNW7builtin3opsEXi1<EmmL0L0"(i64, i64) !dbg !1384 {
alloca_block:
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1385, metadata !DIExpression()), !dbg !1386
  call void @llvm.dbg.value(metadata i64 %0, metadata !1385, metadata !DIExpression()), !dbg !1386
  store i64 %0, i64* %3, align 8, !dbg !1386
  call void @llvm.dbg.declare(metadata i64* %2, metadata !1387, metadata !DIExpression()), !dbg !1386
  call void @llvm.dbg.value(metadata i64 %1, metadata !1387, metadata !DIExpression()), !dbg !1386
  store i64 %1, i64* %2, align 8, !dbg !1386
  %4 = load i64, i64* %3, align 8, !dbg !1388
  %5 = load i64, i64* %2, align 8, !dbg !1388
  %6 = icmp ult i64 %4, %5, !dbg !1388
  ret i1 %6, !dbg !1388
}

define i1 @"_CNW7builtin3opsEXi1<EooL0L0"(i128, i128) !dbg !1389 {
alloca_block:
  %2 = alloca i128, align 16
  %3 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %3, metadata !1390, metadata !DIExpression()), !dbg !1391
  call void @llvm.dbg.value(metadata i128 %0, metadata !1390, metadata !DIExpression()), !dbg !1391
  store i128 %0, i128* %3, align 16, !dbg !1391
  call void @llvm.dbg.declare(metadata i128* %2, metadata !1392, metadata !DIExpression()), !dbg !1391
  call void @llvm.dbg.value(metadata i128 %1, metadata !1392, metadata !DIExpression()), !dbg !1391
  store i128 %1, i128* %2, align 16, !dbg !1391
  %4 = load i128, i128* %3, align 16, !dbg !1393
  %5 = load i128, i128* %2, align 16, !dbg !1393
  %6 = icmp ult i128 %4, %5, !dbg !1393
  ret i1 %6, !dbg !1393
}

define i8 @"_CNW7builtin3opsEXp1+EaL0"(i8) !dbg !1394 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !1397, metadata !DIExpression()), !dbg !1398
  call void @llvm.dbg.value(metadata i8 %0, metadata !1397, metadata !DIExpression()), !dbg !1398
  store i8 %0, i8* %1, align 1, !dbg !1398
  %2 = load i8, i8* %1, align 1, !dbg !1399
  %3 = call i8 @_CNW7builtinE5id_i8EaL0(i8 %2), !dbg !1400
  ret i8 %3, !dbg !1400
}

define i8 @_CNW7builtinE5id_i8EaL0(i8) !dbg !1401 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !1402, metadata !DIExpression()), !dbg !1403
  call void @llvm.dbg.value(metadata i8 %0, metadata !1402, metadata !DIExpression()), !dbg !1403
  store i8 %0, i8* %1, align 1, !dbg !1403
  %2 = load i8, i8* %1, align 1, !dbg !1404
  ret i8 %2, !dbg !1404
}

define i8 @"_CNW7builtin3opsEXp1+EhL0"(i8) !dbg !1405 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !1408, metadata !DIExpression()), !dbg !1409
  call void @llvm.dbg.value(metadata i8 %0, metadata !1408, metadata !DIExpression()), !dbg !1409
  store i8 %0, i8* %1, align 1, !dbg !1409
  %2 = load i8, i8* %1, align 1, !dbg !1410
  %3 = call i8 @_CNW7builtinE5id_u8EhL0(i8 %2), !dbg !1411
  ret i8 %3, !dbg !1411
}

define i8 @_CNW7builtinE5id_u8EhL0(i8) !dbg !1412 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !1413, metadata !DIExpression()), !dbg !1414
  call void @llvm.dbg.value(metadata i8 %0, metadata !1413, metadata !DIExpression()), !dbg !1414
  store i8 %0, i8* %1, align 1, !dbg !1414
  %2 = load i8, i8* %1, align 1, !dbg !1415
  ret i8 %2, !dbg !1415
}

define i16 @"_CNW7builtin3opsEXp1+EsL0"(i16) !dbg !1416 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !1419, metadata !DIExpression()), !dbg !1420
  call void @llvm.dbg.value(metadata i16 %0, metadata !1419, metadata !DIExpression()), !dbg !1420
  store i16 %0, i16* %1, align 2, !dbg !1420
  %2 = load i16, i16* %1, align 2, !dbg !1421
  %3 = call i16 @_CNW7builtinE6id_i16EsL0(i16 %2), !dbg !1422
  ret i16 %3, !dbg !1422
}

define i16 @_CNW7builtinE6id_i16EsL0(i16) !dbg !1423 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !1424, metadata !DIExpression()), !dbg !1425
  call void @llvm.dbg.value(metadata i16 %0, metadata !1424, metadata !DIExpression()), !dbg !1425
  store i16 %0, i16* %1, align 2, !dbg !1425
  %2 = load i16, i16* %1, align 2, !dbg !1426
  ret i16 %2, !dbg !1426
}

define i16 @"_CNW7builtin3opsEXp1+EtL0"(i16) !dbg !1427 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !1430, metadata !DIExpression()), !dbg !1431
  call void @llvm.dbg.value(metadata i16 %0, metadata !1430, metadata !DIExpression()), !dbg !1431
  store i16 %0, i16* %1, align 2, !dbg !1431
  %2 = load i16, i16* %1, align 2, !dbg !1432
  %3 = call i16 @_CNW7builtinE6id_u16EtL0(i16 %2), !dbg !1433
  ret i16 %3, !dbg !1433
}

define i16 @_CNW7builtinE6id_u16EtL0(i16) !dbg !1434 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !1435, metadata !DIExpression()), !dbg !1436
  call void @llvm.dbg.value(metadata i16 %0, metadata !1435, metadata !DIExpression()), !dbg !1436
  store i16 %0, i16* %1, align 2, !dbg !1436
  %2 = load i16, i16* %1, align 2, !dbg !1437
  ret i16 %2, !dbg !1437
}

define i32 @"_CNW7builtin3opsEXp1+EiL0"(i32) !dbg !1438 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !1441, metadata !DIExpression()), !dbg !1442
  call void @llvm.dbg.value(metadata i32 %0, metadata !1441, metadata !DIExpression()), !dbg !1442
  store i32 %0, i32* %1, align 4, !dbg !1442
  %2 = load i32, i32* %1, align 4, !dbg !1443
  %3 = call i32 @_CNW7builtinE6id_i32EiL0(i32 %2), !dbg !1444
  ret i32 %3, !dbg !1444
}

define i32 @_CNW7builtinE6id_i32EiL0(i32) !dbg !1445 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !1446, metadata !DIExpression()), !dbg !1447
  call void @llvm.dbg.value(metadata i32 %0, metadata !1446, metadata !DIExpression()), !dbg !1447
  store i32 %0, i32* %1, align 4, !dbg !1447
  %2 = load i32, i32* %1, align 4, !dbg !1448
  ret i32 %2, !dbg !1448
}

define i32 @"_CNW7builtin3opsEXp1+EjL0"(i32) !dbg !1449 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !1452, metadata !DIExpression()), !dbg !1453
  call void @llvm.dbg.value(metadata i32 %0, metadata !1452, metadata !DIExpression()), !dbg !1453
  store i32 %0, i32* %1, align 4, !dbg !1453
  %2 = load i32, i32* %1, align 4, !dbg !1454
  %3 = call i32 @_CNW7builtinE6id_u32EjL0(i32 %2), !dbg !1455
  ret i32 %3, !dbg !1455
}

define i32 @_CNW7builtinE6id_u32EjL0(i32) !dbg !1456 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !1457, metadata !DIExpression()), !dbg !1458
  call void @llvm.dbg.value(metadata i32 %0, metadata !1457, metadata !DIExpression()), !dbg !1458
  store i32 %0, i32* %1, align 4, !dbg !1458
  %2 = load i32, i32* %1, align 4, !dbg !1459
  ret i32 %2, !dbg !1459
}

define i64 @"_CNW7builtin3opsEXp1+ElL0"(i64) !dbg !1460 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !1463, metadata !DIExpression()), !dbg !1464
  call void @llvm.dbg.value(metadata i64 %0, metadata !1463, metadata !DIExpression()), !dbg !1464
  store i64 %0, i64* %1, align 8, !dbg !1464
  %2 = load i64, i64* %1, align 8, !dbg !1465
  %3 = call i64 @_CNW7builtinE6id_i64ElL0(i64 %2), !dbg !1466
  ret i64 %3, !dbg !1466
}

define i64 @_CNW7builtinE6id_i64ElL0(i64) !dbg !1467 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !1468, metadata !DIExpression()), !dbg !1469
  call void @llvm.dbg.value(metadata i64 %0, metadata !1468, metadata !DIExpression()), !dbg !1469
  store i64 %0, i64* %1, align 8, !dbg !1469
  %2 = load i64, i64* %1, align 8, !dbg !1470
  ret i64 %2, !dbg !1470
}

define i64 @"_CNW7builtin3opsEXp1+EmL0"(i64) !dbg !1471 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !1474, metadata !DIExpression()), !dbg !1475
  call void @llvm.dbg.value(metadata i64 %0, metadata !1474, metadata !DIExpression()), !dbg !1475
  store i64 %0, i64* %1, align 8, !dbg !1475
  %2 = load i64, i64* %1, align 8, !dbg !1476
  %3 = call i64 @_CNW7builtinE6id_u64EmL0(i64 %2), !dbg !1477
  ret i64 %3, !dbg !1477
}

define i64 @_CNW7builtinE6id_u64EmL0(i64) !dbg !1478 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !1479, metadata !DIExpression()), !dbg !1480
  call void @llvm.dbg.value(metadata i64 %0, metadata !1479, metadata !DIExpression()), !dbg !1480
  store i64 %0, i64* %1, align 8, !dbg !1480
  %2 = load i64, i64* %1, align 8, !dbg !1481
  ret i64 %2, !dbg !1481
}

define i128 @"_CNW7builtin3opsEXp1+EnL0"(i128) !dbg !1482 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !1485, metadata !DIExpression()), !dbg !1486
  call void @llvm.dbg.value(metadata i128 %0, metadata !1485, metadata !DIExpression()), !dbg !1486
  store i128 %0, i128* %1, align 16, !dbg !1486
  %2 = load i128, i128* %1, align 16, !dbg !1487
  %3 = call i128 @_CNW7builtinE7id_i128EnL0(i128 %2), !dbg !1488
  ret i128 %3, !dbg !1488
}

define i128 @_CNW7builtinE7id_i128EnL0(i128) !dbg !1489 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !1490, metadata !DIExpression()), !dbg !1491
  call void @llvm.dbg.value(metadata i128 %0, metadata !1490, metadata !DIExpression()), !dbg !1491
  store i128 %0, i128* %1, align 16, !dbg !1491
  %2 = load i128, i128* %1, align 16, !dbg !1492
  ret i128 %2, !dbg !1492
}

define i128 @"_CNW7builtin3opsEXp1+EoL0"(i128) !dbg !1493 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !1496, metadata !DIExpression()), !dbg !1497
  call void @llvm.dbg.value(metadata i128 %0, metadata !1496, metadata !DIExpression()), !dbg !1497
  store i128 %0, i128* %1, align 16, !dbg !1497
  %2 = load i128, i128* %1, align 16, !dbg !1498
  %3 = call i128 @_CNW7builtinE7id_u128EoL0(i128 %2), !dbg !1499
  ret i128 %3, !dbg !1499
}

define i128 @_CNW7builtinE7id_u128EoL0(i128) !dbg !1500 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !1501, metadata !DIExpression()), !dbg !1502
  call void @llvm.dbg.value(metadata i128 %0, metadata !1501, metadata !DIExpression()), !dbg !1502
  store i128 %0, i128* %1, align 16, !dbg !1502
  %2 = load i128, i128* %1, align 16, !dbg !1503
  ret i128 %2, !dbg !1503
}

define i8 @_CNW7builtin3opsEXp1-EaL0(i8) !dbg !1504 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !1505, metadata !DIExpression()), !dbg !1506
  call void @llvm.dbg.value(metadata i8 %0, metadata !1505, metadata !DIExpression()), !dbg !1506
  store i8 %0, i8* %1, align 1, !dbg !1506
  %2 = load i8, i8* %1, align 1, !dbg !1507
  %3 = call i8 @_CNW7builtinE6neg_i8EaL0(i8 %2), !dbg !1508
  ret i8 %3, !dbg !1508
}

define i8 @_CNW7builtinE6neg_i8EaL0(i8) !dbg !1509 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !1510, metadata !DIExpression()), !dbg !1511
  call void @llvm.dbg.value(metadata i8 %0, metadata !1510, metadata !DIExpression()), !dbg !1511
  store i8 %0, i8* %1, align 1, !dbg !1511
  %2 = load i8, i8* %1, align 1, !dbg !1512
  %3 = sub nsw i8 0, %2, !dbg !1512
  ret i8 %3, !dbg !1512
}

define i8 @_CNW7builtin3opsEXp1-EhL0(i8) !dbg !1513 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !1514, metadata !DIExpression()), !dbg !1515
  call void @llvm.dbg.value(metadata i8 %0, metadata !1514, metadata !DIExpression()), !dbg !1515
  store i8 %0, i8* %1, align 1, !dbg !1515
  %2 = load i8, i8* %1, align 1, !dbg !1516
  %3 = call i8 @_CNW7builtinE6neg_u8EhL0(i8 %2), !dbg !1517
  ret i8 %3, !dbg !1517
}

define i8 @_CNW7builtinE6neg_u8EhL0(i8) !dbg !1518 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !1519, metadata !DIExpression()), !dbg !1520
  call void @llvm.dbg.value(metadata i8 %0, metadata !1519, metadata !DIExpression()), !dbg !1520
  store i8 %0, i8* %1, align 1, !dbg !1520
  %2 = load i8, i8* %1, align 1, !dbg !1521
  %3 = sub nuw i8 0, %2, !dbg !1521
  ret i8 %3, !dbg !1521
}

define i16 @_CNW7builtin3opsEXp1-EsL0(i16) !dbg !1522 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !1523, metadata !DIExpression()), !dbg !1524
  call void @llvm.dbg.value(metadata i16 %0, metadata !1523, metadata !DIExpression()), !dbg !1524
  store i16 %0, i16* %1, align 2, !dbg !1524
  %2 = load i16, i16* %1, align 2, !dbg !1525
  %3 = call i16 @_CNW7builtinE7neg_i16EsL0(i16 %2), !dbg !1526
  ret i16 %3, !dbg !1526
}

define i16 @_CNW7builtinE7neg_i16EsL0(i16) !dbg !1527 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !1528, metadata !DIExpression()), !dbg !1529
  call void @llvm.dbg.value(metadata i16 %0, metadata !1528, metadata !DIExpression()), !dbg !1529
  store i16 %0, i16* %1, align 2, !dbg !1529
  %2 = load i16, i16* %1, align 2, !dbg !1530
  %3 = sub nsw i16 0, %2, !dbg !1530
  ret i16 %3, !dbg !1530
}

define i16 @_CNW7builtin3opsEXp1-EtL0(i16) !dbg !1531 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !1532, metadata !DIExpression()), !dbg !1533
  call void @llvm.dbg.value(metadata i16 %0, metadata !1532, metadata !DIExpression()), !dbg !1533
  store i16 %0, i16* %1, align 2, !dbg !1533
  %2 = load i16, i16* %1, align 2, !dbg !1534
  %3 = call i16 @_CNW7builtinE7neg_u16EtL0(i16 %2), !dbg !1535
  ret i16 %3, !dbg !1535
}

define i16 @_CNW7builtinE7neg_u16EtL0(i16) !dbg !1536 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !1537, metadata !DIExpression()), !dbg !1538
  call void @llvm.dbg.value(metadata i16 %0, metadata !1537, metadata !DIExpression()), !dbg !1538
  store i16 %0, i16* %1, align 2, !dbg !1538
  %2 = load i16, i16* %1, align 2, !dbg !1539
  %3 = sub nuw i16 0, %2, !dbg !1539
  ret i16 %3, !dbg !1539
}

define i32 @_CNW7builtin3opsEXp1-EiL0(i32) !dbg !1540 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !1541, metadata !DIExpression()), !dbg !1542
  call void @llvm.dbg.value(metadata i32 %0, metadata !1541, metadata !DIExpression()), !dbg !1542
  store i32 %0, i32* %1, align 4, !dbg !1542
  %2 = load i32, i32* %1, align 4, !dbg !1543
  %3 = call i32 @_CNW7builtinE7neg_i32EiL0(i32 %2), !dbg !1544
  ret i32 %3, !dbg !1544
}

define i32 @_CNW7builtinE7neg_i32EiL0(i32) !dbg !1545 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !1546, metadata !DIExpression()), !dbg !1547
  call void @llvm.dbg.value(metadata i32 %0, metadata !1546, metadata !DIExpression()), !dbg !1547
  store i32 %0, i32* %1, align 4, !dbg !1547
  %2 = load i32, i32* %1, align 4, !dbg !1548
  %3 = sub nsw i32 0, %2, !dbg !1548
  ret i32 %3, !dbg !1548
}

define i32 @_CNW7builtin3opsEXp1-EjL0(i32) !dbg !1549 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !1550, metadata !DIExpression()), !dbg !1551
  call void @llvm.dbg.value(metadata i32 %0, metadata !1550, metadata !DIExpression()), !dbg !1551
  store i32 %0, i32* %1, align 4, !dbg !1551
  %2 = load i32, i32* %1, align 4, !dbg !1552
  %3 = call i32 @_CNW7builtinE7neg_u32EjL0(i32 %2), !dbg !1553
  ret i32 %3, !dbg !1553
}

define i32 @_CNW7builtinE7neg_u32EjL0(i32) !dbg !1554 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !1555, metadata !DIExpression()), !dbg !1556
  call void @llvm.dbg.value(metadata i32 %0, metadata !1555, metadata !DIExpression()), !dbg !1556
  store i32 %0, i32* %1, align 4, !dbg !1556
  %2 = load i32, i32* %1, align 4, !dbg !1557
  %3 = sub nuw i32 0, %2, !dbg !1557
  ret i32 %3, !dbg !1557
}

define i64 @_CNW7builtin3opsEXp1-ElL0(i64) !dbg !1558 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !1559, metadata !DIExpression()), !dbg !1560
  call void @llvm.dbg.value(metadata i64 %0, metadata !1559, metadata !DIExpression()), !dbg !1560
  store i64 %0, i64* %1, align 8, !dbg !1560
  %2 = load i64, i64* %1, align 8, !dbg !1561
  %3 = call i64 @_CNW7builtinE7neg_i64ElL0(i64 %2), !dbg !1562
  ret i64 %3, !dbg !1562
}

define i64 @_CNW7builtinE7neg_i64ElL0(i64) !dbg !1563 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !1564, metadata !DIExpression()), !dbg !1565
  call void @llvm.dbg.value(metadata i64 %0, metadata !1564, metadata !DIExpression()), !dbg !1565
  store i64 %0, i64* %1, align 8, !dbg !1565
  %2 = load i64, i64* %1, align 8, !dbg !1566
  %3 = sub nsw i64 0, %2, !dbg !1566
  ret i64 %3, !dbg !1566
}

define i64 @_CNW7builtin3opsEXp1-EmL0(i64) !dbg !1567 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !1568, metadata !DIExpression()), !dbg !1569
  call void @llvm.dbg.value(metadata i64 %0, metadata !1568, metadata !DIExpression()), !dbg !1569
  store i64 %0, i64* %1, align 8, !dbg !1569
  %2 = load i64, i64* %1, align 8, !dbg !1570
  %3 = call i64 @_CNW7builtinE7neg_u64EmL0(i64 %2), !dbg !1571
  ret i64 %3, !dbg !1571
}

define i64 @_CNW7builtinE7neg_u64EmL0(i64) !dbg !1572 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !1573, metadata !DIExpression()), !dbg !1574
  call void @llvm.dbg.value(metadata i64 %0, metadata !1573, metadata !DIExpression()), !dbg !1574
  store i64 %0, i64* %1, align 8, !dbg !1574
  %2 = load i64, i64* %1, align 8, !dbg !1575
  %3 = sub nuw i64 0, %2, !dbg !1575
  ret i64 %3, !dbg !1575
}

define i128 @_CNW7builtin3opsEXp1-EnL0(i128) !dbg !1576 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !1577, metadata !DIExpression()), !dbg !1578
  call void @llvm.dbg.value(metadata i128 %0, metadata !1577, metadata !DIExpression()), !dbg !1578
  store i128 %0, i128* %1, align 16, !dbg !1578
  %2 = load i128, i128* %1, align 16, !dbg !1579
  %3 = call i128 @_CNW7builtinE8neg_i128EnL0(i128 %2), !dbg !1580
  ret i128 %3, !dbg !1580
}

define i128 @_CNW7builtinE8neg_i128EnL0(i128) !dbg !1581 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !1582, metadata !DIExpression()), !dbg !1583
  call void @llvm.dbg.value(metadata i128 %0, metadata !1582, metadata !DIExpression()), !dbg !1583
  store i128 %0, i128* %1, align 16, !dbg !1583
  %2 = load i128, i128* %1, align 16, !dbg !1584
  %3 = sub nsw i128 0, %2, !dbg !1584
  ret i128 %3, !dbg !1584
}

define i128 @_CNW7builtin3opsEXp1-EoL0(i128) !dbg !1585 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !1586, metadata !DIExpression()), !dbg !1587
  call void @llvm.dbg.value(metadata i128 %0, metadata !1586, metadata !DIExpression()), !dbg !1587
  store i128 %0, i128* %1, align 16, !dbg !1587
  %2 = load i128, i128* %1, align 16, !dbg !1588
  %3 = call i128 @_CNW7builtinE8neg_u128EoL0(i128 %2), !dbg !1589
  ret i128 %3, !dbg !1589
}

define i128 @_CNW7builtinE8neg_u128EoL0(i128) !dbg !1590 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !1591, metadata !DIExpression()), !dbg !1592
  call void @llvm.dbg.value(metadata i128 %0, metadata !1591, metadata !DIExpression()), !dbg !1592
  store i128 %0, i128* %1, align 16, !dbg !1592
  %2 = load i128, i128* %1, align 16, !dbg !1593
  %3 = sub nuw i128 0, %2, !dbg !1593
  ret i128 %3, !dbg !1593
}

define i1 @"_CNW7builtin3opsEXp1!EbL0"(i1) !dbg !1594 {
alloca_block:
  %1 = alloca i1, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i1* %1, metadata !1598, metadata !DIExpression()), !dbg !1599
  call void @llvm.dbg.value(metadata i1 %0, metadata !1598, metadata !DIExpression()), !dbg !1599
  store i1 %0, i1* %1, align 1, !dbg !1599
  %2 = load i1, i1* %1, align 1, !dbg !1600
  %3 = call i1 @_CNW7builtinE6not_i1EbL0(i1 %2), !dbg !1601
  ret i1 %3, !dbg !1601
}

define i1 @_CNW7builtinE6not_i1EbL0(i1) !dbg !1602 {
alloca_block:
  %1 = alloca i1, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i1* %1, metadata !1603, metadata !DIExpression()), !dbg !1604
  call void @llvm.dbg.value(metadata i1 %0, metadata !1603, metadata !DIExpression()), !dbg !1604
  store i1 %0, i1* %1, align 1, !dbg !1604
  %2 = load i1, i1* %1, align 1, !dbg !1605
  %3 = xor i1 false, %2, !dbg !1605
  ret i1 %3, !dbg !1605
}

define i8 @"_CNW7builtin3opsEXp1~EaL0"(i8) !dbg !1606 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !1607, metadata !DIExpression()), !dbg !1608
  call void @llvm.dbg.value(metadata i8 %0, metadata !1607, metadata !DIExpression()), !dbg !1608
  store i8 %0, i8* %1, align 1, !dbg !1608
  %2 = load i8, i8* %1, align 1, !dbg !1609
  %3 = call i8 @_CNW7builtinE6not_i8EaL0(i8 %2), !dbg !1610
  ret i8 %3, !dbg !1610
}

define i8 @_CNW7builtinE6not_i8EaL0(i8) !dbg !1611 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !1612, metadata !DIExpression()), !dbg !1613
  call void @llvm.dbg.value(metadata i8 %0, metadata !1612, metadata !DIExpression()), !dbg !1613
  store i8 %0, i8* %1, align 1, !dbg !1613
  %2 = load i8, i8* %1, align 1, !dbg !1614
  %3 = xor i8 0, %2, !dbg !1614
  ret i8 %3, !dbg !1614
}

define i8 @"_CNW7builtin3opsEXp1~EhL0"(i8) !dbg !1615 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !1616, metadata !DIExpression()), !dbg !1617
  call void @llvm.dbg.value(metadata i8 %0, metadata !1616, metadata !DIExpression()), !dbg !1617
  store i8 %0, i8* %1, align 1, !dbg !1617
  %2 = load i8, i8* %1, align 1, !dbg !1618
  %3 = call i8 @_CNW7builtinE6not_u8EhL0(i8 %2), !dbg !1619
  ret i8 %3, !dbg !1619
}

define i8 @_CNW7builtinE6not_u8EhL0(i8) !dbg !1620 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !1621, metadata !DIExpression()), !dbg !1622
  call void @llvm.dbg.value(metadata i8 %0, metadata !1621, metadata !DIExpression()), !dbg !1622
  store i8 %0, i8* %1, align 1, !dbg !1622
  %2 = load i8, i8* %1, align 1, !dbg !1623
  %3 = xor i8 0, %2, !dbg !1623
  ret i8 %3, !dbg !1623
}

define i16 @"_CNW7builtin3opsEXp1~EsL0"(i16) !dbg !1624 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !1625, metadata !DIExpression()), !dbg !1626
  call void @llvm.dbg.value(metadata i16 %0, metadata !1625, metadata !DIExpression()), !dbg !1626
  store i16 %0, i16* %1, align 2, !dbg !1626
  %2 = load i16, i16* %1, align 2, !dbg !1627
  %3 = call i16 @_CNW7builtinE7not_i16EsL0(i16 %2), !dbg !1628
  ret i16 %3, !dbg !1628
}

define i16 @_CNW7builtinE7not_i16EsL0(i16) !dbg !1629 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !1630, metadata !DIExpression()), !dbg !1631
  call void @llvm.dbg.value(metadata i16 %0, metadata !1630, metadata !DIExpression()), !dbg !1631
  store i16 %0, i16* %1, align 2, !dbg !1631
  %2 = load i16, i16* %1, align 2, !dbg !1632
  %3 = xor i16 0, %2, !dbg !1632
  ret i16 %3, !dbg !1632
}

define i16 @"_CNW7builtin3opsEXp1~EtL0"(i16) !dbg !1633 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !1634, metadata !DIExpression()), !dbg !1635
  call void @llvm.dbg.value(metadata i16 %0, metadata !1634, metadata !DIExpression()), !dbg !1635
  store i16 %0, i16* %1, align 2, !dbg !1635
  %2 = load i16, i16* %1, align 2, !dbg !1636
  %3 = call i16 @_CNW7builtinE7not_u16EtL0(i16 %2), !dbg !1637
  ret i16 %3, !dbg !1637
}

define i16 @_CNW7builtinE7not_u16EtL0(i16) !dbg !1638 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !1639, metadata !DIExpression()), !dbg !1640
  call void @llvm.dbg.value(metadata i16 %0, metadata !1639, metadata !DIExpression()), !dbg !1640
  store i16 %0, i16* %1, align 2, !dbg !1640
  %2 = load i16, i16* %1, align 2, !dbg !1641
  %3 = xor i16 0, %2, !dbg !1641
  ret i16 %3, !dbg !1641
}

define i32 @"_CNW7builtin3opsEXp1~EiL0"(i32) !dbg !1642 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !1643, metadata !DIExpression()), !dbg !1644
  call void @llvm.dbg.value(metadata i32 %0, metadata !1643, metadata !DIExpression()), !dbg !1644
  store i32 %0, i32* %1, align 4, !dbg !1644
  %2 = load i32, i32* %1, align 4, !dbg !1645
  %3 = call i32 @_CNW7builtinE7not_i32EiL0(i32 %2), !dbg !1646
  ret i32 %3, !dbg !1646
}

define i32 @_CNW7builtinE7not_i32EiL0(i32) !dbg !1647 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !1648, metadata !DIExpression()), !dbg !1649
  call void @llvm.dbg.value(metadata i32 %0, metadata !1648, metadata !DIExpression()), !dbg !1649
  store i32 %0, i32* %1, align 4, !dbg !1649
  %2 = load i32, i32* %1, align 4, !dbg !1650
  %3 = xor i32 0, %2, !dbg !1650
  ret i32 %3, !dbg !1650
}

define i32 @"_CNW7builtin3opsEXp1~EjL0"(i32) !dbg !1651 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !1652, metadata !DIExpression()), !dbg !1653
  call void @llvm.dbg.value(metadata i32 %0, metadata !1652, metadata !DIExpression()), !dbg !1653
  store i32 %0, i32* %1, align 4, !dbg !1653
  %2 = load i32, i32* %1, align 4, !dbg !1654
  %3 = call i32 @_CNW7builtinE7not_u32EjL0(i32 %2), !dbg !1655
  ret i32 %3, !dbg !1655
}

define i32 @_CNW7builtinE7not_u32EjL0(i32) !dbg !1656 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !1657, metadata !DIExpression()), !dbg !1658
  call void @llvm.dbg.value(metadata i32 %0, metadata !1657, metadata !DIExpression()), !dbg !1658
  store i32 %0, i32* %1, align 4, !dbg !1658
  %2 = load i32, i32* %1, align 4, !dbg !1659
  %3 = xor i32 0, %2, !dbg !1659
  ret i32 %3, !dbg !1659
}

define i64 @"_CNW7builtin3opsEXp1~ElL0"(i64) !dbg !1660 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !1661, metadata !DIExpression()), !dbg !1662
  call void @llvm.dbg.value(metadata i64 %0, metadata !1661, metadata !DIExpression()), !dbg !1662
  store i64 %0, i64* %1, align 8, !dbg !1662
  %2 = load i64, i64* %1, align 8, !dbg !1663
  %3 = call i64 @_CNW7builtinE7not_i64ElL0(i64 %2), !dbg !1664
  ret i64 %3, !dbg !1664
}

define i64 @_CNW7builtinE7not_i64ElL0(i64) !dbg !1665 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !1666, metadata !DIExpression()), !dbg !1667
  call void @llvm.dbg.value(metadata i64 %0, metadata !1666, metadata !DIExpression()), !dbg !1667
  store i64 %0, i64* %1, align 8, !dbg !1667
  %2 = load i64, i64* %1, align 8, !dbg !1668
  %3 = xor i64 0, %2, !dbg !1668
  ret i64 %3, !dbg !1668
}

define i64 @"_CNW7builtin3opsEXp1~EmL0"(i64) !dbg !1669 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !1670, metadata !DIExpression()), !dbg !1671
  call void @llvm.dbg.value(metadata i64 %0, metadata !1670, metadata !DIExpression()), !dbg !1671
  store i64 %0, i64* %1, align 8, !dbg !1671
  %2 = load i64, i64* %1, align 8, !dbg !1672
  %3 = call i64 @_CNW7builtinE7not_u64EmL0(i64 %2), !dbg !1673
  ret i64 %3, !dbg !1673
}

define i64 @_CNW7builtinE7not_u64EmL0(i64) !dbg !1674 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !1675, metadata !DIExpression()), !dbg !1676
  call void @llvm.dbg.value(metadata i64 %0, metadata !1675, metadata !DIExpression()), !dbg !1676
  store i64 %0, i64* %1, align 8, !dbg !1676
  %2 = load i64, i64* %1, align 8, !dbg !1677
  %3 = xor i64 0, %2, !dbg !1677
  ret i64 %3, !dbg !1677
}

define i128 @"_CNW7builtin3opsEXp1~EnL0"(i128) !dbg !1678 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !1679, metadata !DIExpression()), !dbg !1680
  call void @llvm.dbg.value(metadata i128 %0, metadata !1679, metadata !DIExpression()), !dbg !1680
  store i128 %0, i128* %1, align 16, !dbg !1680
  %2 = load i128, i128* %1, align 16, !dbg !1681
  %3 = call i128 @_CNW7builtinE8not_i128EnL0(i128 %2), !dbg !1682
  ret i128 %3, !dbg !1682
}

define i128 @_CNW7builtinE8not_i128EnL0(i128) !dbg !1683 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !1684, metadata !DIExpression()), !dbg !1685
  call void @llvm.dbg.value(metadata i128 %0, metadata !1684, metadata !DIExpression()), !dbg !1685
  store i128 %0, i128* %1, align 16, !dbg !1685
  %2 = load i128, i128* %1, align 16, !dbg !1686
  %3 = xor i128 0, %2, !dbg !1686
  ret i128 %3, !dbg !1686
}

define i128 @"_CNW7builtin3opsEXp1~EoL0"(i128) !dbg !1687 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !1688, metadata !DIExpression()), !dbg !1689
  call void @llvm.dbg.value(metadata i128 %0, metadata !1688, metadata !DIExpression()), !dbg !1689
  store i128 %0, i128* %1, align 16, !dbg !1689
  %2 = load i128, i128* %1, align 16, !dbg !1690
  %3 = call i128 @_CNW7builtinE8not_u128EoL0(i128 %2), !dbg !1691
  ret i128 %3, !dbg !1691
}

define i128 @_CNW7builtinE8not_u128EoL0(i128) !dbg !1692 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !1693, metadata !DIExpression()), !dbg !1694
  call void @llvm.dbg.value(metadata i128 %0, metadata !1693, metadata !DIExpression()), !dbg !1694
  store i128 %0, i128* %1, align 16, !dbg !1694
  %2 = load i128, i128* %1, align 16, !dbg !1695
  %3 = xor i128 0, %2, !dbg !1695
  ret i128 %3, !dbg !1695
}

define float @"_CNW7builtin3opsEXi1+EffL0L0"(float, float) !dbg !1696 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !1700, metadata !DIExpression()), !dbg !1701
  call void @llvm.dbg.value(metadata float %0, metadata !1700, metadata !DIExpression()), !dbg !1701
  store float %0, float* %3, align 4, !dbg !1701
  call void @llvm.dbg.declare(metadata float* %2, metadata !1702, metadata !DIExpression()), !dbg !1701
  call void @llvm.dbg.value(metadata float %1, metadata !1702, metadata !DIExpression()), !dbg !1701
  store float %1, float* %2, align 4, !dbg !1701
  %4 = load float, float* %3, align 4, !dbg !1703
  %5 = load float, float* %2, align 4, !dbg !1703
  %6 = fadd float %4, %5, !dbg !1703
  ret float %6, !dbg !1703
}

define void @"_CNW7builtin3opsEXi2+=EffL0L0"(float*, float) !dbg !1704 {
entry:
  %2 = alloca float, align 4
  call void @llvm.dbg.declare(metadata float* %0, metadata !1708, metadata !DIExpression()), !dbg !1709
  call void @llvm.dbg.declare(metadata float* %2, metadata !1710, metadata !DIExpression()), !dbg !1709
  call void @llvm.dbg.value(metadata float %1, metadata !1710, metadata !DIExpression()), !dbg !1709
  store float %1, float* %2, align 4, !dbg !1709
  %3 = load float, float* %0, align 4, !dbg !1711
  %4 = load float, float* %2, align 4, !dbg !1711
  %5 = fadd float %3, %4, !dbg !1711
  call void @llvm.dbg.value(metadata float %5, metadata !1708, metadata !DIExpression()), !dbg !1711
  %6 = load float, float* %0, align 4, !dbg !1711
  store float %5, float* %0, align 4, !dbg !1711
  ret void, !dbg !1711
}

define double @"_CNW7builtin3opsEXi1+EddL0L0"(double, double) !dbg !1712 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !1716, metadata !DIExpression()), !dbg !1717
  call void @llvm.dbg.value(metadata double %0, metadata !1716, metadata !DIExpression()), !dbg !1717
  store double %0, double* %3, align 8, !dbg !1717
  call void @llvm.dbg.declare(metadata double* %2, metadata !1718, metadata !DIExpression()), !dbg !1717
  call void @llvm.dbg.value(metadata double %1, metadata !1718, metadata !DIExpression()), !dbg !1717
  store double %1, double* %2, align 8, !dbg !1717
  %4 = load double, double* %3, align 8, !dbg !1719
  %5 = load double, double* %2, align 8, !dbg !1719
  %6 = fadd double %4, %5, !dbg !1719
  ret double %6, !dbg !1719
}

define void @"_CNW7builtin3opsEXi2+=EddL0L0"(double*, double) !dbg !1720 {
entry:
  %2 = alloca double, align 8
  call void @llvm.dbg.declare(metadata double* %0, metadata !1724, metadata !DIExpression()), !dbg !1725
  call void @llvm.dbg.declare(metadata double* %2, metadata !1726, metadata !DIExpression()), !dbg !1725
  call void @llvm.dbg.value(metadata double %1, metadata !1726, metadata !DIExpression()), !dbg !1725
  store double %1, double* %2, align 8, !dbg !1725
  %3 = load double, double* %0, align 8, !dbg !1727
  %4 = load double, double* %2, align 8, !dbg !1727
  %5 = fadd double %3, %4, !dbg !1727
  call void @llvm.dbg.value(metadata double %5, metadata !1724, metadata !DIExpression()), !dbg !1727
  %6 = load double, double* %0, align 8, !dbg !1727
  store double %5, double* %0, align 8, !dbg !1727
  ret void, !dbg !1727
}

define float @_CNW7builtin3opsEXi1-EffL0L0(float, float) !dbg !1728 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !1729, metadata !DIExpression()), !dbg !1730
  call void @llvm.dbg.value(metadata float %0, metadata !1729, metadata !DIExpression()), !dbg !1730
  store float %0, float* %3, align 4, !dbg !1730
  call void @llvm.dbg.declare(metadata float* %2, metadata !1731, metadata !DIExpression()), !dbg !1730
  call void @llvm.dbg.value(metadata float %1, metadata !1731, metadata !DIExpression()), !dbg !1730
  store float %1, float* %2, align 4, !dbg !1730
  %4 = load float, float* %3, align 4, !dbg !1732
  %5 = load float, float* %2, align 4, !dbg !1732
  %6 = fsub float %4, %5, !dbg !1732
  ret float %6, !dbg !1732
}

define void @"_CNW7builtin3opsEXi2-=EffL0L0"(float*, float) !dbg !1733 {
entry:
  %2 = alloca float, align 4
  call void @llvm.dbg.declare(metadata float* %0, metadata !1734, metadata !DIExpression()), !dbg !1735
  call void @llvm.dbg.declare(metadata float* %2, metadata !1736, metadata !DIExpression()), !dbg !1735
  call void @llvm.dbg.value(metadata float %1, metadata !1736, metadata !DIExpression()), !dbg !1735
  store float %1, float* %2, align 4, !dbg !1735
  %3 = load float, float* %0, align 4, !dbg !1737
  %4 = load float, float* %2, align 4, !dbg !1737
  %5 = fsub float %3, %4, !dbg !1737
  call void @llvm.dbg.value(metadata float %5, metadata !1734, metadata !DIExpression()), !dbg !1737
  %6 = load float, float* %0, align 4, !dbg !1737
  store float %5, float* %0, align 4, !dbg !1737
  ret void, !dbg !1737
}

define double @_CNW7builtin3opsEXi1-EddL0L0(double, double) !dbg !1738 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !1739, metadata !DIExpression()), !dbg !1740
  call void @llvm.dbg.value(metadata double %0, metadata !1739, metadata !DIExpression()), !dbg !1740
  store double %0, double* %3, align 8, !dbg !1740
  call void @llvm.dbg.declare(metadata double* %2, metadata !1741, metadata !DIExpression()), !dbg !1740
  call void @llvm.dbg.value(metadata double %1, metadata !1741, metadata !DIExpression()), !dbg !1740
  store double %1, double* %2, align 8, !dbg !1740
  %4 = load double, double* %3, align 8, !dbg !1742
  %5 = load double, double* %2, align 8, !dbg !1742
  %6 = fsub double %4, %5, !dbg !1742
  ret double %6, !dbg !1742
}

define void @"_CNW7builtin3opsEXi2-=EddL0L0"(double*, double) !dbg !1743 {
entry:
  %2 = alloca double, align 8
  call void @llvm.dbg.declare(metadata double* %0, metadata !1744, metadata !DIExpression()), !dbg !1745
  call void @llvm.dbg.declare(metadata double* %2, metadata !1746, metadata !DIExpression()), !dbg !1745
  call void @llvm.dbg.value(metadata double %1, metadata !1746, metadata !DIExpression()), !dbg !1745
  store double %1, double* %2, align 8, !dbg !1745
  %3 = load double, double* %0, align 8, !dbg !1747
  %4 = load double, double* %2, align 8, !dbg !1747
  %5 = fsub double %3, %4, !dbg !1747
  call void @llvm.dbg.value(metadata double %5, metadata !1744, metadata !DIExpression()), !dbg !1747
  %6 = load double, double* %0, align 8, !dbg !1747
  store double %5, double* %0, align 8, !dbg !1747
  ret void, !dbg !1747
}

define float @"_CNW7builtin3opsEXi1*EffL0L0"(float, float) !dbg !1748 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !1749, metadata !DIExpression()), !dbg !1750
  call void @llvm.dbg.value(metadata float %0, metadata !1749, metadata !DIExpression()), !dbg !1750
  store float %0, float* %3, align 4, !dbg !1750
  call void @llvm.dbg.declare(metadata float* %2, metadata !1751, metadata !DIExpression()), !dbg !1750
  call void @llvm.dbg.value(metadata float %1, metadata !1751, metadata !DIExpression()), !dbg !1750
  store float %1, float* %2, align 4, !dbg !1750
  %4 = load float, float* %3, align 4, !dbg !1752
  %5 = load float, float* %2, align 4, !dbg !1752
  %6 = fmul float %4, %5, !dbg !1752
  ret float %6, !dbg !1752
}

define void @"_CNW7builtin3opsEXi2*=EffL0L0"(float*, float) !dbg !1753 {
entry:
  %2 = alloca float, align 4
  call void @llvm.dbg.declare(metadata float* %0, metadata !1754, metadata !DIExpression()), !dbg !1755
  call void @llvm.dbg.declare(metadata float* %2, metadata !1756, metadata !DIExpression()), !dbg !1755
  call void @llvm.dbg.value(metadata float %1, metadata !1756, metadata !DIExpression()), !dbg !1755
  store float %1, float* %2, align 4, !dbg !1755
  %3 = load float, float* %0, align 4, !dbg !1757
  %4 = load float, float* %2, align 4, !dbg !1757
  %5 = fmul float %3, %4, !dbg !1757
  call void @llvm.dbg.value(metadata float %5, metadata !1754, metadata !DIExpression()), !dbg !1757
  %6 = load float, float* %0, align 4, !dbg !1757
  store float %5, float* %0, align 4, !dbg !1757
  ret void, !dbg !1757
}

define double @"_CNW7builtin3opsEXi1*EddL0L0"(double, double) !dbg !1758 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !1759, metadata !DIExpression()), !dbg !1760
  call void @llvm.dbg.value(metadata double %0, metadata !1759, metadata !DIExpression()), !dbg !1760
  store double %0, double* %3, align 8, !dbg !1760
  call void @llvm.dbg.declare(metadata double* %2, metadata !1761, metadata !DIExpression()), !dbg !1760
  call void @llvm.dbg.value(metadata double %1, metadata !1761, metadata !DIExpression()), !dbg !1760
  store double %1, double* %2, align 8, !dbg !1760
  %4 = load double, double* %3, align 8, !dbg !1762
  %5 = load double, double* %2, align 8, !dbg !1762
  %6 = fmul double %4, %5, !dbg !1762
  ret double %6, !dbg !1762
}

define void @"_CNW7builtin3opsEXi2*=EddL0L0"(double*, double) !dbg !1763 {
entry:
  %2 = alloca double, align 8
  call void @llvm.dbg.declare(metadata double* %0, metadata !1764, metadata !DIExpression()), !dbg !1765
  call void @llvm.dbg.declare(metadata double* %2, metadata !1766, metadata !DIExpression()), !dbg !1765
  call void @llvm.dbg.value(metadata double %1, metadata !1766, metadata !DIExpression()), !dbg !1765
  store double %1, double* %2, align 8, !dbg !1765
  %3 = load double, double* %0, align 8, !dbg !1767
  %4 = load double, double* %2, align 8, !dbg !1767
  %5 = fmul double %3, %4, !dbg !1767
  call void @llvm.dbg.value(metadata double %5, metadata !1764, metadata !DIExpression()), !dbg !1767
  %6 = load double, double* %0, align 8, !dbg !1767
  store double %5, double* %0, align 8, !dbg !1767
  ret void, !dbg !1767
}

define float @"_CNW7builtin3opsEXi1/EffL0L0"(float, float) !dbg !1768 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !1769, metadata !DIExpression()), !dbg !1770
  call void @llvm.dbg.value(metadata float %0, metadata !1769, metadata !DIExpression()), !dbg !1770
  store float %0, float* %3, align 4, !dbg !1770
  call void @llvm.dbg.declare(metadata float* %2, metadata !1771, metadata !DIExpression()), !dbg !1770
  call void @llvm.dbg.value(metadata float %1, metadata !1771, metadata !DIExpression()), !dbg !1770
  store float %1, float* %2, align 4, !dbg !1770
  %4 = load float, float* %3, align 4, !dbg !1772
  %5 = load float, float* %2, align 4, !dbg !1772
  %6 = fdiv float %4, %5, !dbg !1772
  ret float %6, !dbg !1772
}

define void @"_CNW7builtin3opsEXi2/=EffL0L0"(float*, float) !dbg !1773 {
entry:
  %2 = alloca float, align 4
  call void @llvm.dbg.declare(metadata float* %0, metadata !1774, metadata !DIExpression()), !dbg !1775
  call void @llvm.dbg.declare(metadata float* %2, metadata !1776, metadata !DIExpression()), !dbg !1775
  call void @llvm.dbg.value(metadata float %1, metadata !1776, metadata !DIExpression()), !dbg !1775
  store float %1, float* %2, align 4, !dbg !1775
  %3 = load float, float* %0, align 4, !dbg !1777
  %4 = load float, float* %2, align 4, !dbg !1777
  %5 = fdiv float %3, %4, !dbg !1777
  call void @llvm.dbg.value(metadata float %5, metadata !1774, metadata !DIExpression()), !dbg !1777
  %6 = load float, float* %0, align 4, !dbg !1777
  store float %5, float* %0, align 4, !dbg !1777
  ret void, !dbg !1777
}

define double @"_CNW7builtin3opsEXi1/EddL0L0"(double, double) !dbg !1778 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !1779, metadata !DIExpression()), !dbg !1780
  call void @llvm.dbg.value(metadata double %0, metadata !1779, metadata !DIExpression()), !dbg !1780
  store double %0, double* %3, align 8, !dbg !1780
  call void @llvm.dbg.declare(metadata double* %2, metadata !1781, metadata !DIExpression()), !dbg !1780
  call void @llvm.dbg.value(metadata double %1, metadata !1781, metadata !DIExpression()), !dbg !1780
  store double %1, double* %2, align 8, !dbg !1780
  %4 = load double, double* %3, align 8, !dbg !1782
  %5 = load double, double* %2, align 8, !dbg !1782
  %6 = fdiv double %4, %5, !dbg !1782
  ret double %6, !dbg !1782
}

define void @"_CNW7builtin3opsEXi2/=EddL0L0"(double*, double) !dbg !1783 {
entry:
  %2 = alloca double, align 8
  call void @llvm.dbg.declare(metadata double* %0, metadata !1784, metadata !DIExpression()), !dbg !1785
  call void @llvm.dbg.declare(metadata double* %2, metadata !1786, metadata !DIExpression()), !dbg !1785
  call void @llvm.dbg.value(metadata double %1, metadata !1786, metadata !DIExpression()), !dbg !1785
  store double %1, double* %2, align 8, !dbg !1785
  %3 = load double, double* %0, align 8, !dbg !1787
  %4 = load double, double* %2, align 8, !dbg !1787
  %5 = fdiv double %3, %4, !dbg !1787
  call void @llvm.dbg.value(metadata double %5, metadata !1784, metadata !DIExpression()), !dbg !1787
  %6 = load double, double* %0, align 8, !dbg !1787
  store double %5, double* %0, align 8, !dbg !1787
  ret void, !dbg !1787
}

define float @"_CNW7builtin3opsEXi1%EffL0L0"(float, float) !dbg !1788 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !1789, metadata !DIExpression()), !dbg !1790
  call void @llvm.dbg.value(metadata float %0, metadata !1789, metadata !DIExpression()), !dbg !1790
  store float %0, float* %3, align 4, !dbg !1790
  call void @llvm.dbg.declare(metadata float* %2, metadata !1791, metadata !DIExpression()), !dbg !1790
  call void @llvm.dbg.value(metadata float %1, metadata !1791, metadata !DIExpression()), !dbg !1790
  store float %1, float* %2, align 4, !dbg !1790
  %4 = load float, float* %3, align 4, !dbg !1792
  %5 = load float, float* %2, align 4, !dbg !1792
  %6 = frem float %4, %5, !dbg !1792
  ret float %6, !dbg !1792
}

define void @"_CNW7builtin3opsEXi2%=EffL0L0"(float*, float) !dbg !1793 {
entry:
  %2 = alloca float, align 4
  call void @llvm.dbg.declare(metadata float* %0, metadata !1794, metadata !DIExpression()), !dbg !1795
  call void @llvm.dbg.declare(metadata float* %2, metadata !1796, metadata !DIExpression()), !dbg !1795
  call void @llvm.dbg.value(metadata float %1, metadata !1796, metadata !DIExpression()), !dbg !1795
  store float %1, float* %2, align 4, !dbg !1795
  %3 = load float, float* %0, align 4, !dbg !1797
  %4 = load float, float* %2, align 4, !dbg !1797
  %5 = frem float %3, %4, !dbg !1797
  call void @llvm.dbg.value(metadata float %5, metadata !1794, metadata !DIExpression()), !dbg !1797
  %6 = load float, float* %0, align 4, !dbg !1797
  store float %5, float* %0, align 4, !dbg !1797
  ret void, !dbg !1797
}

define double @"_CNW7builtin3opsEXi1%EddL0L0"(double, double) !dbg !1798 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !1799, metadata !DIExpression()), !dbg !1800
  call void @llvm.dbg.value(metadata double %0, metadata !1799, metadata !DIExpression()), !dbg !1800
  store double %0, double* %3, align 8, !dbg !1800
  call void @llvm.dbg.declare(metadata double* %2, metadata !1801, metadata !DIExpression()), !dbg !1800
  call void @llvm.dbg.value(metadata double %1, metadata !1801, metadata !DIExpression()), !dbg !1800
  store double %1, double* %2, align 8, !dbg !1800
  %4 = load double, double* %3, align 8, !dbg !1802
  %5 = load double, double* %2, align 8, !dbg !1802
  %6 = frem double %4, %5, !dbg !1802
  ret double %6, !dbg !1802
}

define void @"_CNW7builtin3opsEXi2%=EddL0L0"(double*, double) !dbg !1803 {
entry:
  %2 = alloca double, align 8
  call void @llvm.dbg.declare(metadata double* %0, metadata !1804, metadata !DIExpression()), !dbg !1805
  call void @llvm.dbg.declare(metadata double* %2, metadata !1806, metadata !DIExpression()), !dbg !1805
  call void @llvm.dbg.value(metadata double %1, metadata !1806, metadata !DIExpression()), !dbg !1805
  store double %1, double* %2, align 8, !dbg !1805
  %3 = load double, double* %0, align 8, !dbg !1807
  %4 = load double, double* %2, align 8, !dbg !1807
  %5 = frem double %3, %4, !dbg !1807
  call void @llvm.dbg.value(metadata double %5, metadata !1804, metadata !DIExpression()), !dbg !1807
  %6 = load double, double* %0, align 8, !dbg !1807
  store double %5, double* %0, align 8, !dbg !1807
  ret void, !dbg !1807
}

define i1 @"_CNW7builtin3opsEXi2==EffL0L0"(float, float) !dbg !1808 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !1809, metadata !DIExpression()), !dbg !1810
  call void @llvm.dbg.value(metadata float %0, metadata !1809, metadata !DIExpression()), !dbg !1810
  store float %0, float* %3, align 4, !dbg !1810
  call void @llvm.dbg.declare(metadata float* %2, metadata !1811, metadata !DIExpression()), !dbg !1810
  call void @llvm.dbg.value(metadata float %1, metadata !1811, metadata !DIExpression()), !dbg !1810
  store float %1, float* %2, align 4, !dbg !1810
  %4 = load float, float* %3, align 4, !dbg !1812
  %5 = load float, float* %2, align 4, !dbg !1812
  %6 = fcmp oeq float %4, %5, !dbg !1812
  ret i1 %6, !dbg !1812
}

define i1 @"_CNW7builtin3opsEXi2==EddL0L0"(double, double) !dbg !1813 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !1814, metadata !DIExpression()), !dbg !1815
  call void @llvm.dbg.value(metadata double %0, metadata !1814, metadata !DIExpression()), !dbg !1815
  store double %0, double* %3, align 8, !dbg !1815
  call void @llvm.dbg.declare(metadata double* %2, metadata !1816, metadata !DIExpression()), !dbg !1815
  call void @llvm.dbg.value(metadata double %1, metadata !1816, metadata !DIExpression()), !dbg !1815
  store double %1, double* %2, align 8, !dbg !1815
  %4 = load double, double* %3, align 8, !dbg !1817
  %5 = load double, double* %2, align 8, !dbg !1817
  %6 = fcmp oeq double %4, %5, !dbg !1817
  ret i1 %6, !dbg !1817
}

define i1 @"_CNW7builtin3opsEXi2!=EffL0L0"(float, float) !dbg !1818 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !1819, metadata !DIExpression()), !dbg !1820
  call void @llvm.dbg.value(metadata float %0, metadata !1819, metadata !DIExpression()), !dbg !1820
  store float %0, float* %3, align 4, !dbg !1820
  call void @llvm.dbg.declare(metadata float* %2, metadata !1821, metadata !DIExpression()), !dbg !1820
  call void @llvm.dbg.value(metadata float %1, metadata !1821, metadata !DIExpression()), !dbg !1820
  store float %1, float* %2, align 4, !dbg !1820
  %4 = load float, float* %3, align 4, !dbg !1822
  %5 = load float, float* %2, align 4, !dbg !1822
  %6 = fcmp one float %4, %5, !dbg !1822
  ret i1 %6, !dbg !1822
}

define i1 @"_CNW7builtin3opsEXi2!=EddL0L0"(double, double) !dbg !1823 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !1824, metadata !DIExpression()), !dbg !1825
  call void @llvm.dbg.value(metadata double %0, metadata !1824, metadata !DIExpression()), !dbg !1825
  store double %0, double* %3, align 8, !dbg !1825
  call void @llvm.dbg.declare(metadata double* %2, metadata !1826, metadata !DIExpression()), !dbg !1825
  call void @llvm.dbg.value(metadata double %1, metadata !1826, metadata !DIExpression()), !dbg !1825
  store double %1, double* %2, align 8, !dbg !1825
  %4 = load double, double* %3, align 8, !dbg !1827
  %5 = load double, double* %2, align 8, !dbg !1827
  %6 = fcmp one double %4, %5, !dbg !1827
  ret i1 %6, !dbg !1827
}

define i1 @"_CNW7builtin3opsEXi2>=EffL0L0"(float, float) !dbg !1828 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !1829, metadata !DIExpression()), !dbg !1830
  call void @llvm.dbg.value(metadata float %0, metadata !1829, metadata !DIExpression()), !dbg !1830
  store float %0, float* %3, align 4, !dbg !1830
  call void @llvm.dbg.declare(metadata float* %2, metadata !1831, metadata !DIExpression()), !dbg !1830
  call void @llvm.dbg.value(metadata float %1, metadata !1831, metadata !DIExpression()), !dbg !1830
  store float %1, float* %2, align 4, !dbg !1830
  %4 = load float, float* %3, align 4, !dbg !1832
  %5 = load float, float* %2, align 4, !dbg !1832
  %6 = fcmp oge float %4, %5, !dbg !1832
  ret i1 %6, !dbg !1832
}

define i1 @"_CNW7builtin3opsEXi2>=EddL0L0"(double, double) !dbg !1833 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !1834, metadata !DIExpression()), !dbg !1835
  call void @llvm.dbg.value(metadata double %0, metadata !1834, metadata !DIExpression()), !dbg !1835
  store double %0, double* %3, align 8, !dbg !1835
  call void @llvm.dbg.declare(metadata double* %2, metadata !1836, metadata !DIExpression()), !dbg !1835
  call void @llvm.dbg.value(metadata double %1, metadata !1836, metadata !DIExpression()), !dbg !1835
  store double %1, double* %2, align 8, !dbg !1835
  %4 = load double, double* %3, align 8, !dbg !1837
  %5 = load double, double* %2, align 8, !dbg !1837
  %6 = fcmp oge double %4, %5, !dbg !1837
  ret i1 %6, !dbg !1837
}

define i1 @"_CNW7builtin3opsEXi1>EffL0L0"(float, float) !dbg !1838 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !1839, metadata !DIExpression()), !dbg !1840
  call void @llvm.dbg.value(metadata float %0, metadata !1839, metadata !DIExpression()), !dbg !1840
  store float %0, float* %3, align 4, !dbg !1840
  call void @llvm.dbg.declare(metadata float* %2, metadata !1841, metadata !DIExpression()), !dbg !1840
  call void @llvm.dbg.value(metadata float %1, metadata !1841, metadata !DIExpression()), !dbg !1840
  store float %1, float* %2, align 4, !dbg !1840
  %4 = load float, float* %3, align 4, !dbg !1842
  %5 = load float, float* %2, align 4, !dbg !1842
  %6 = fcmp ogt float %4, %5, !dbg !1842
  ret i1 %6, !dbg !1842
}

define i1 @"_CNW7builtin3opsEXi1>EddL0L0"(double, double) !dbg !1843 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !1844, metadata !DIExpression()), !dbg !1845
  call void @llvm.dbg.value(metadata double %0, metadata !1844, metadata !DIExpression()), !dbg !1845
  store double %0, double* %3, align 8, !dbg !1845
  call void @llvm.dbg.declare(metadata double* %2, metadata !1846, metadata !DIExpression()), !dbg !1845
  call void @llvm.dbg.value(metadata double %1, metadata !1846, metadata !DIExpression()), !dbg !1845
  store double %1, double* %2, align 8, !dbg !1845
  %4 = load double, double* %3, align 8, !dbg !1847
  %5 = load double, double* %2, align 8, !dbg !1847
  %6 = fcmp ogt double %4, %5, !dbg !1847
  ret i1 %6, !dbg !1847
}

define i1 @"_CNW7builtin3opsEXi2<=EffL0L0"(float, float) !dbg !1848 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !1849, metadata !DIExpression()), !dbg !1850
  call void @llvm.dbg.value(metadata float %0, metadata !1849, metadata !DIExpression()), !dbg !1850
  store float %0, float* %3, align 4, !dbg !1850
  call void @llvm.dbg.declare(metadata float* %2, metadata !1851, metadata !DIExpression()), !dbg !1850
  call void @llvm.dbg.value(metadata float %1, metadata !1851, metadata !DIExpression()), !dbg !1850
  store float %1, float* %2, align 4, !dbg !1850
  %4 = load float, float* %3, align 4, !dbg !1852
  %5 = load float, float* %2, align 4, !dbg !1852
  %6 = fcmp ole float %4, %5, !dbg !1852
  ret i1 %6, !dbg !1852
}

define i1 @"_CNW7builtin3opsEXi2<=EddL0L0"(double, double) !dbg !1853 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !1854, metadata !DIExpression()), !dbg !1855
  call void @llvm.dbg.value(metadata double %0, metadata !1854, metadata !DIExpression()), !dbg !1855
  store double %0, double* %3, align 8, !dbg !1855
  call void @llvm.dbg.declare(metadata double* %2, metadata !1856, metadata !DIExpression()), !dbg !1855
  call void @llvm.dbg.value(metadata double %1, metadata !1856, metadata !DIExpression()), !dbg !1855
  store double %1, double* %2, align 8, !dbg !1855
  %4 = load double, double* %3, align 8, !dbg !1857
  %5 = load double, double* %2, align 8, !dbg !1857
  %6 = fcmp ole double %4, %5, !dbg !1857
  ret i1 %6, !dbg !1857
}

define i1 @"_CNW7builtin3opsEXi1<EffL0L0"(float, float) !dbg !1858 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !1859, metadata !DIExpression()), !dbg !1860
  call void @llvm.dbg.value(metadata float %0, metadata !1859, metadata !DIExpression()), !dbg !1860
  store float %0, float* %3, align 4, !dbg !1860
  call void @llvm.dbg.declare(metadata float* %2, metadata !1861, metadata !DIExpression()), !dbg !1860
  call void @llvm.dbg.value(metadata float %1, metadata !1861, metadata !DIExpression()), !dbg !1860
  store float %1, float* %2, align 4, !dbg !1860
  %4 = load float, float* %3, align 4, !dbg !1862
  %5 = load float, float* %2, align 4, !dbg !1862
  %6 = fcmp olt float %4, %5, !dbg !1862
  ret i1 %6, !dbg !1862
}

define i1 @"_CNW7builtin3opsEXi1<EddL0L0"(double, double) !dbg !1863 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !1864, metadata !DIExpression()), !dbg !1865
  call void @llvm.dbg.value(metadata double %0, metadata !1864, metadata !DIExpression()), !dbg !1865
  store double %0, double* %3, align 8, !dbg !1865
  call void @llvm.dbg.declare(metadata double* %2, metadata !1866, metadata !DIExpression()), !dbg !1865
  call void @llvm.dbg.value(metadata double %1, metadata !1866, metadata !DIExpression()), !dbg !1865
  store double %1, double* %2, align 8, !dbg !1865
  %4 = load double, double* %3, align 8, !dbg !1867
  %5 = load double, double* %2, align 8, !dbg !1867
  %6 = fcmp olt double %4, %5, !dbg !1867
  ret i1 %6, !dbg !1867
}

define float @"_CNW7builtin3opsEXp1+EfL0"(float) !dbg !1868 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !1871, metadata !DIExpression()), !dbg !1872
  call void @llvm.dbg.value(metadata float %0, metadata !1871, metadata !DIExpression()), !dbg !1872
  store float %0, float* %1, align 4, !dbg !1872
  %2 = load float, float* %1, align 4, !dbg !1873
  %3 = call float @_CNW7builtinE6id_f32EfL0(float %2), !dbg !1874
  ret float %3, !dbg !1874
}

define float @_CNW7builtinE6id_f32EfL0(float) !dbg !1875 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !1876, metadata !DIExpression()), !dbg !1877
  call void @llvm.dbg.value(metadata float %0, metadata !1876, metadata !DIExpression()), !dbg !1877
  store float %0, float* %1, align 4, !dbg !1877
  %2 = load float, float* %1, align 4, !dbg !1878
  ret float %2, !dbg !1878
}

define double @"_CNW7builtin3opsEXp1+EdL0"(double) !dbg !1879 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !1882, metadata !DIExpression()), !dbg !1883
  call void @llvm.dbg.value(metadata double %0, metadata !1882, metadata !DIExpression()), !dbg !1883
  store double %0, double* %1, align 8, !dbg !1883
  %2 = load double, double* %1, align 8, !dbg !1884
  %3 = call double @_CNW7builtinE6id_f64EdL0(double %2), !dbg !1885
  ret double %3, !dbg !1885
}

define double @_CNW7builtinE6id_f64EdL0(double) !dbg !1886 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !1887, metadata !DIExpression()), !dbg !1888
  call void @llvm.dbg.value(metadata double %0, metadata !1887, metadata !DIExpression()), !dbg !1888
  store double %0, double* %1, align 8, !dbg !1888
  %2 = load double, double* %1, align 8, !dbg !1889
  ret double %2, !dbg !1889
}

define float @_CNW7builtin3opsEXp1-EfL0(float) !dbg !1890 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !1891, metadata !DIExpression()), !dbg !1892
  call void @llvm.dbg.value(metadata float %0, metadata !1891, metadata !DIExpression()), !dbg !1892
  store float %0, float* %1, align 4, !dbg !1892
  %2 = load float, float* %1, align 4, !dbg !1893
  %3 = fsub float -0.000000e+00, %2, !dbg !1893
  ret float %3, !dbg !1893
}

define double @_CNW7builtin3opsEXp1-EdL0(double) !dbg !1894 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !1895, metadata !DIExpression()), !dbg !1896
  call void @llvm.dbg.value(metadata double %0, metadata !1895, metadata !DIExpression()), !dbg !1896
  store double %0, double* %1, align 8, !dbg !1896
  %2 = load double, double* %1, align 8, !dbg !1897
  %3 = fsub double -0.000000e+00, %2, !dbg !1897
  ret double %3, !dbg !1897
}

declare i8* @_CNW7builtinE10allocStackEmL5bytes(i64)

declare void @_CNW7builtinE19printCTFEStacktraceEv()

declare void @_CNW7builtinE6memcpyEKPvS_mmL3dstL3srcL4sizeL5align(i8*, i8*, i64, i64)

declare void @_CNW7builtinE6memsetEKPvhmL6memoryL5valueL4size(i8*, i8, i64)

declare i1 @_CNW7builtinE6likelyEbL0(i1)

declare i1 @_CNW7builtinE8unlikelyEbL0(i1)

define void @_CNW7builtinE14_Builtin_NeverD0Ev(i64*) !dbg !1898 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW7builtinE14_Builtin_NeverD3Ev(i64* %0), !dbg !1903
  ret void, !dbg !1903
}

define i64 @_CNW7builtinE14_Builtin_Never4copyES_L0(i64) !dbg !1904 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret i64 %0, !dbg !1907
}

define %"struct.builtin.<private name>.Bool" @"_CNW7builtinE14_Builtin_NeverXi2==ES_S_L0L0"(i64, i64) !dbg !1908 {
alloca_block:
  %2 = alloca %"struct.builtin.<private name>.Bool", align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = icmp eq i64 %0, %1, !dbg !1911
  %4 = load %"struct.builtin.<private name>.Bool", %"struct.builtin.<private name>.Bool"* %2, align 1, !dbg !1911
  %5 = extractvalue %"struct.builtin.<private name>.Bool" %4, 0, !dbg !1911
  %6 = alloca i1, !dbg !1911
  store i1 %5, i1* %6, !dbg !1911
  store i1 %3, i1* %6, !dbg !1911
  ret %"struct.builtin.<private name>.Bool" %4, !dbg !1911
}

define %"struct.builtin.<private name>.Bool" @"_CNW7builtinE14_Builtin_NeverXi2!=ES_S_L0L0"(i64, i64) !dbg !1912 {
alloca_block:
  %2 = alloca %"struct.builtin.<private name>.Bool", align 1
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %4, metadata !1914, metadata !DIExpression()), !dbg !1915
  call void @llvm.dbg.value(metadata i64 %0, metadata !1914, metadata !DIExpression()), !dbg !1915
  store i64 %0, i64* %4, align 8, !dbg !1915
  call void @llvm.dbg.declare(metadata i64* %3, metadata !1916, metadata !DIExpression()), !dbg !1915
  call void @llvm.dbg.value(metadata i64 %1, metadata !1916, metadata !DIExpression()), !dbg !1915
  store i64 %1, i64* %3, align 8, !dbg !1915
  %5 = load i64, i64* %4, align 8, !dbg !1915
  %6 = load i64, i64* %3, align 8, !dbg !1915
  %7 = call %"struct.builtin.<private name>.Bool" @"_CNW7builtinE14_Builtin_NeverXi2==ES_S_L0L0"(i64 %5, i64 %6), !dbg !1915
  %8 = call %"struct.builtin.<private name>.Bool" @"_CNW7builtinE4BoolXp1!ES0_L0"(%"struct.builtin.<private name>.Bool" %7), !dbg !1915
  store %"struct.builtin.<private name>.Bool" %7, %"struct.builtin.<private name>.Bool"* %2, align 1, !dbg !1915
  call void @_CNW7builtinE4BoolD0Ev(%"struct.builtin.<private name>.Bool"* %2), !dbg !1915
  ret %"struct.builtin.<private name>.Bool" %8, !dbg !1915
}

define void @_CNW7builtinE14_Builtin_NeverC1ElL8rawValue(i64*, i64) !dbg !1917 {
init.failure:
  %2 = load i64, i64* %0, align 8, !dbg !1920
  %3 = bitcast i64* %0 to i8*, !dbg !1920
  call void @llvm.memset.p0i8.i64(i8* %3, i8 0, i64 8, i32 1, i1 false), !dbg !1920
  ret void, !dbg !1920
}

define i64 @_CNW7builtinE14_Builtin_NeverGa8rawValueES_L0(i64) !dbg !1921 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret i64 %0, !dbg !1922
}

define void @_CNW7builtinE14_Builtin_NeverD3Ev(i64*) !dbg !1923 {
unreachable:
  unreachable, !dbg !1924
}

define %"struct.builtin.<private name>.Bool" @"_CNW7builtinE4BoolXp1!ES0_L0"(%"struct.builtin.<private name>.Bool") !dbg !1925 {
entry:
  %1 = alloca %"struct.builtin.<private name>.Bool", align 1
  %2 = alloca %"struct.builtin.<private name>.Bool", align 1
  call void @llvm.dbg.declare(metadata %"struct.builtin.<private name>.Bool"* %2, metadata !1931, metadata !DIExpression()), !dbg !1932
  call void @llvm.dbg.value(metadata %"struct.builtin.<private name>.Bool" %0, metadata !1931, metadata !DIExpression()), !dbg !1932
  store %"struct.builtin.<private name>.Bool" %0, %"struct.builtin.<private name>.Bool"* %2, align 1, !dbg !1932
  %3 = bitcast %"struct.builtin.<private name>.Bool"* %2 to i1*
  %4 = load i1, i1* %3, align 1, !dbg !1932
  %5 = xor i1 %4, true, !dbg !1932
  call void @_CNW7builtinE4BoolC1EbL5value(%"struct.builtin.<private name>.Bool"* %1, i1 %5), !dbg !1932
  %6 = load %"struct.builtin.<private name>.Bool", %"struct.builtin.<private name>.Bool"* %1, !dbg !1932
  ret %"struct.builtin.<private name>.Bool" %6, !dbg !1932
}

define void @_CNW7builtinE4BoolD0Ev(%"struct.builtin.<private name>.Bool"*) !dbg !1933 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW7builtinE4BoolD3Ev(%"struct.builtin.<private name>.Bool"* %0), !dbg !1937
  ret void, !dbg !1937
}

declare void @_CNW7builtinE14_Builtin_NeverC2ERS_lL0L8rawValue(i64*, i64)

declare i64 @_CNW7builtinE11unreachableEv()

define void @_CNW7builtinE14AtomicRMWBinOpD0Ev(i64*) !dbg !1938 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW7builtinE14AtomicRMWBinOpD3Ev(i64* %0), !dbg !1955
  ret void, !dbg !1955
}

define void @_CNW7builtinE14AtomicRMWBinOpD3Ev(i64*) !dbg !1956 {
merge:
  ret void, !dbg !1957
}

define void @_CNW7builtinE11coro_resumeEKPvL0(i8*) !dbg !1958 {
alloca_block:
  %1 = alloca i8*, align 8
  call void @llvm.dbg.declare(metadata i8** %1, metadata !1961, metadata !DIExpression()), !dbg !1962
  call void @llvm.dbg.value(metadata i8* %0, metadata !1961, metadata !DIExpression()), !dbg !1962
  store i8* %0, i8** %1, align 8, !dbg !1962
  %2 = load i8*, i8** %1, align 8, !dbg !1963
  %3 = bitcast i8* %2 to { i8*, i8* }*
  %4 = bitcast { i8*, i8* }* %3 to i8**
  %5 = load i8*, i8** %4
  %6 = bitcast i8* %5 to void (i8*)*
  call fastcc void %6(i8* %2), !dbg !1963
  ret void, !dbg !1963
}

define void @_CNW7builtinE12coro_destroyEKPvL0(i8*) !dbg !1964 {
alloca_block:
  %1 = alloca i8*, align 8
  call void @llvm.dbg.declare(metadata i8** %1, metadata !1965, metadata !DIExpression()), !dbg !1966
  call void @llvm.dbg.value(metadata i8* %0, metadata !1965, metadata !DIExpression()), !dbg !1966
  store i8* %0, i8** %1, align 8, !dbg !1966
  %2 = load i8*, i8** %1, align 8, !dbg !1967
  %3 = bitcast i8* %2 to { i8*, i8* }*
  %4 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %3, i32 0, i32 1
  %5 = load i8*, i8** %4
  %6 = bitcast i8* %5 to void (i8*)*
  call fastcc void %6(i8* %2), !dbg !1967
  ret void, !dbg !1967
}

define i1 @_CNW7builtinE9coro_doneEKPvL0(i8*) !dbg !1968 {
entry:
  %1 = alloca i8*, align 8
  call void @llvm.dbg.declare(metadata i8** %1, metadata !1969, metadata !DIExpression()), !dbg !1970
  call void @llvm.dbg.value(metadata i8* %0, metadata !1969, metadata !DIExpression()), !dbg !1970
  store i8* %0, i8** %1, align 8, !dbg !1970
  %2 = load i8*, i8** %1, align 8, !dbg !1971
  %3 = bitcast i8* %2 to i8**, !dbg !1971
  %4 = bitcast i8** %3 to i8**
  %5 = load i8*, i8** %4, !dbg !1971
  %6 = icmp eq i8* %5, null, !dbg !1971
  ret i1 %6, !dbg !1971
}

declare float @_CNW7builtinE8fadd_f32EffL0L0(float, float)

declare float @_CNW7builtinE8fsub_f32EffL0L0(float, float)

declare float @_CNW7builtinE8fmul_f32EffL0L0(float, float)

declare float @_CNW7builtinE8fdiv_f32EffL0L0(float, float)

declare float @_CNW7builtinE8frem_f32EffL0L0(float, float)

declare float @_CNW7builtinE8fneg_f32EfL0(float)

define i1 @_CNW7builtinE14fcmp_false_f32EffL0L0(float, float) !dbg !1972 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !1973, metadata !DIExpression()), !dbg !1974
  call void @llvm.dbg.value(metadata float %0, metadata !1973, metadata !DIExpression()), !dbg !1974
  store float %0, float* %3, align 4, !dbg !1974
  call void @llvm.dbg.declare(metadata float* %2, metadata !1975, metadata !DIExpression()), !dbg !1974
  call void @llvm.dbg.value(metadata float %1, metadata !1975, metadata !DIExpression()), !dbg !1974
  store float %1, float* %2, align 4, !dbg !1974
  ret i1 false, !dbg !1976
}

define i1 @_CNW7builtinE13fcmp_true_f32EffL0L0(float, float) !dbg !1977 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !1978, metadata !DIExpression()), !dbg !1979
  call void @llvm.dbg.value(metadata float %0, metadata !1978, metadata !DIExpression()), !dbg !1979
  store float %0, float* %3, align 4, !dbg !1979
  call void @llvm.dbg.declare(metadata float* %2, metadata !1980, metadata !DIExpression()), !dbg !1979
  call void @llvm.dbg.value(metadata float %1, metadata !1980, metadata !DIExpression()), !dbg !1979
  store float %1, float* %2, align 4, !dbg !1979
  ret i1 true, !dbg !1981
}

declare i1 @_CNW7builtinE12fcmp_oeq_f32EffL0L0(float, float)

declare i1 @_CNW7builtinE12fcmp_one_f32EffL0L0(float, float)

declare i1 @_CNW7builtinE12fcmp_ogt_f32EffL0L0(float, float)

declare i1 @_CNW7builtinE12fcmp_oge_f32EffL0L0(float, float)

declare i1 @_CNW7builtinE12fcmp_olt_f32EffL0L0(float, float)

declare i1 @_CNW7builtinE12fcmp_ole_f32EffL0L0(float, float)

declare i1 @_CNW7builtinE12fcmp_ord_f32EffL0L0(float, float)

declare i1 @_CNW7builtinE12fcmp_ueq_f32EffL0L0(float, float)

declare i1 @_CNW7builtinE12fcmp_une_f32EffL0L0(float, float)

declare i1 @_CNW7builtinE12fcmp_ugt_f32EffL0L0(float, float)

declare i1 @_CNW7builtinE12fcmp_uge_f32EffL0L0(float, float)

declare i1 @_CNW7builtinE12fcmp_ult_f32EffL0L0(float, float)

declare i1 @_CNW7builtinE12fcmp_ule_f32EffL0L0(float, float)

declare i1 @_CNW7builtinE12fcmp_uno_f32EffL0L0(float, float)

define float @_CNW7builtinE9log10_f32EfL0(float) !dbg !1982 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !1983, metadata !DIExpression()), !dbg !1984
  call void @llvm.dbg.value(metadata float %0, metadata !1983, metadata !DIExpression()), !dbg !1984
  store float %0, float* %1, align 4, !dbg !1984
  %2 = load float, float* %1, align 4, !dbg !1985
  %3 = call float @llvm.log10.f32(float %2), !dbg !1985
  ret float %3, !dbg !1985
}

define float @_CNW7builtinE8log2_f32EfL0(float) !dbg !1986 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !1987, metadata !DIExpression()), !dbg !1988
  call void @llvm.dbg.value(metadata float %0, metadata !1987, metadata !DIExpression()), !dbg !1988
  store float %0, float* %1, align 4, !dbg !1988
  %2 = load float, float* %1, align 4, !dbg !1989
  %3 = call float @llvm.log2.f32(float %2), !dbg !1989
  ret float %3, !dbg !1989
}

define float @_CNW7builtinE7log_f32EfL0(float) !dbg !1990 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !1991, metadata !DIExpression()), !dbg !1992
  call void @llvm.dbg.value(metadata float %0, metadata !1991, metadata !DIExpression()), !dbg !1992
  store float %0, float* %1, align 4, !dbg !1992
  %2 = load float, float* %1, align 4, !dbg !1993
  %3 = call float @llvm.log1.f32(float %2), !dbg !1993
  ret float %3, !dbg !1993
}

define float @_CNW7builtinE9floor_f32EfL0(float) !dbg !1994 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !1995, metadata !DIExpression()), !dbg !1996
  call void @llvm.dbg.value(metadata float %0, metadata !1995, metadata !DIExpression()), !dbg !1996
  store float %0, float* %1, align 4, !dbg !1996
  %2 = load float, float* %1, align 4, !dbg !1997
  %3 = call float @llvm.floor.f32(float %2), !dbg !1997
  ret float %3, !dbg !1997
}

define float @_CNW7builtinE8ceil_f32EfL0(float) !dbg !1998 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !1999, metadata !DIExpression()), !dbg !2000
  call void @llvm.dbg.value(metadata float %0, metadata !1999, metadata !DIExpression()), !dbg !2000
  store float %0, float* %1, align 4, !dbg !2000
  %2 = load float, float* %1, align 4, !dbg !2001
  %3 = call float @llvm.ceil.f32(float %2), !dbg !2001
  ret float %3, !dbg !2001
}

define float @_CNW7builtinE9round_f32EfL0(float) !dbg !2002 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !2003, metadata !DIExpression()), !dbg !2004
  call void @llvm.dbg.value(metadata float %0, metadata !2003, metadata !DIExpression()), !dbg !2004
  store float %0, float* %1, align 4, !dbg !2004
  %2 = load float, float* %1, align 4, !dbg !2005
  %3 = call float @llvm.round.f32(float %2), !dbg !2005
  ret float %3, !dbg !2005
}

define float @_CNW7builtinE8rint_f32EfL0(float) !dbg !2006 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !2007, metadata !DIExpression()), !dbg !2008
  call void @llvm.dbg.value(metadata float %0, metadata !2007, metadata !DIExpression()), !dbg !2008
  store float %0, float* %1, align 4, !dbg !2008
  %2 = load float, float* %1, align 4, !dbg !2009
  %3 = call float @llvm.rint.f32(float %2), !dbg !2009
  ret float %3, !dbg !2009
}

define float @_CNW7builtinE9trunc_f32EfL0(float) !dbg !2010 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !2011, metadata !DIExpression()), !dbg !2012
  call void @llvm.dbg.value(metadata float %0, metadata !2011, metadata !DIExpression()), !dbg !2012
  store float %0, float* %1, align 4, !dbg !2012
  %2 = load float, float* %1, align 4, !dbg !2013
  %3 = call float @llvm.trunc.f32(float %2), !dbg !2013
  ret float %3, !dbg !2013
}

define float @_CNW7builtinE8sqrt_f32EfL0(float) !dbg !2014 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !2015, metadata !DIExpression()), !dbg !2016
  call void @llvm.dbg.value(metadata float %0, metadata !2015, metadata !DIExpression()), !dbg !2016
  store float %0, float* %1, align 4, !dbg !2016
  %2 = load float, float* %1, align 4, !dbg !2017
  %3 = call float @llvm.sqrt.f32(float %2), !dbg !2017
  ret float %3, !dbg !2017
}

define float @_CNW7builtinE8fabs_f32EfL0(float) !dbg !2018 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !2019, metadata !DIExpression()), !dbg !2020
  call void @llvm.dbg.value(metadata float %0, metadata !2019, metadata !DIExpression()), !dbg !2020
  store float %0, float* %1, align 4, !dbg !2020
  %2 = load float, float* %1, align 4, !dbg !2021
  %3 = call float @llvm.fabs.f32(float %2), !dbg !2021
  ret float %3, !dbg !2021
}

define float @_CNW7builtinE7sin_f32EfL0(float) !dbg !2022 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !2023, metadata !DIExpression()), !dbg !2024
  call void @llvm.dbg.value(metadata float %0, metadata !2023, metadata !DIExpression()), !dbg !2024
  store float %0, float* %1, align 4, !dbg !2024
  %2 = load float, float* %1, align 4, !dbg !2025
  %3 = call float @llvm.sin.f32(float %2), !dbg !2025
  ret float %3, !dbg !2025
}

define float @_CNW7builtinE7cos_f32EfL0(float) !dbg !2026 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !2027, metadata !DIExpression()), !dbg !2028
  call void @llvm.dbg.value(metadata float %0, metadata !2027, metadata !DIExpression()), !dbg !2028
  store float %0, float* %1, align 4, !dbg !2028
  %2 = load float, float* %1, align 4, !dbg !2029
  %3 = call float @llvm.cos.f32(float %2), !dbg !2029
  ret float %3, !dbg !2029
}

define float @_CNW7builtinE7exp_f32EfL0(float) !dbg !2030 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !2031, metadata !DIExpression()), !dbg !2032
  call void @llvm.dbg.value(metadata float %0, metadata !2031, metadata !DIExpression()), !dbg !2032
  store float %0, float* %1, align 4, !dbg !2032
  %2 = load float, float* %1, align 4, !dbg !2033
  %3 = call float @llvm.exp.f32(float %2), !dbg !2033
  ret float %3, !dbg !2033
}

define float @_CNW7builtinE8exp2_f32EfL0(float) !dbg !2034 {
alloca_block:
  %1 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %1, metadata !2035, metadata !DIExpression()), !dbg !2036
  call void @llvm.dbg.value(metadata float %0, metadata !2035, metadata !DIExpression()), !dbg !2036
  store float %0, float* %1, align 4, !dbg !2036
  %2 = load float, float* %1, align 4, !dbg !2037
  %3 = call float @llvm.exp2.f32(float %2), !dbg !2037
  ret float %3, !dbg !2037
}

define float @_CNW7builtinE7pow_f32EffL0L0(float, float) !dbg !2038 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !2039, metadata !DIExpression()), !dbg !2040
  call void @llvm.dbg.value(metadata float %0, metadata !2039, metadata !DIExpression()), !dbg !2040
  store float %0, float* %3, align 4, !dbg !2040
  call void @llvm.dbg.declare(metadata float* %2, metadata !2041, metadata !DIExpression()), !dbg !2040
  call void @llvm.dbg.value(metadata float %1, metadata !2041, metadata !DIExpression()), !dbg !2040
  store float %1, float* %2, align 4, !dbg !2040
  %4 = load float, float* %3, align 4, !dbg !2042
  %5 = load float, float* %2, align 4, !dbg !2042
  %6 = call float @llvm.pow.f32(float %4, float %5), !dbg !2042
  ret float %6, !dbg !2042
}

define float @_CNW7builtinE8powi_f32EfiL0L0(float, i32) !dbg !2043 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !2046, metadata !DIExpression()), !dbg !2047
  call void @llvm.dbg.value(metadata float %0, metadata !2046, metadata !DIExpression()), !dbg !2047
  store float %0, float* %3, align 4, !dbg !2047
  call void @llvm.dbg.declare(metadata i32* %2, metadata !2048, metadata !DIExpression()), !dbg !2047
  call void @llvm.dbg.value(metadata i32 %1, metadata !2048, metadata !DIExpression()), !dbg !2047
  store i32 %1, i32* %2, align 4, !dbg !2047
  %4 = load float, float* %3, align 4, !dbg !2049
  %5 = load i32, i32* %2, align 4, !dbg !2049
  %6 = call float @llvm.powi.f32(float %4, i32 %5), !dbg !2049
  ret float %6, !dbg !2049
}

define float @_CNW7builtinE10minnum_f32EffL0L0(float, float) !dbg !2050 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !2051, metadata !DIExpression()), !dbg !2052
  call void @llvm.dbg.value(metadata float %0, metadata !2051, metadata !DIExpression()), !dbg !2052
  store float %0, float* %3, align 4, !dbg !2052
  call void @llvm.dbg.declare(metadata float* %2, metadata !2053, metadata !DIExpression()), !dbg !2052
  call void @llvm.dbg.value(metadata float %1, metadata !2053, metadata !DIExpression()), !dbg !2052
  store float %1, float* %2, align 4, !dbg !2052
  %4 = load float, float* %3, align 4, !dbg !2054
  %5 = load float, float* %2, align 4, !dbg !2054
  %6 = call float @llvm.minnum.f32(float %4, float %5), !dbg !2054
  ret float %6, !dbg !2054
}

define float @_CNW7builtinE10maxnum_f32EffL0L0(float, float) !dbg !2055 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !2056, metadata !DIExpression()), !dbg !2057
  call void @llvm.dbg.value(metadata float %0, metadata !2056, metadata !DIExpression()), !dbg !2057
  store float %0, float* %3, align 4, !dbg !2057
  call void @llvm.dbg.declare(metadata float* %2, metadata !2058, metadata !DIExpression()), !dbg !2057
  call void @llvm.dbg.value(metadata float %1, metadata !2058, metadata !DIExpression()), !dbg !2057
  store float %1, float* %2, align 4, !dbg !2057
  %4 = load float, float* %3, align 4, !dbg !2059
  %5 = load float, float* %2, align 4, !dbg !2059
  %6 = call float @llvm.maxnum.f32(float %4, float %5), !dbg !2059
  ret float %6, !dbg !2059
}

define float @_CNW7builtinE11minimum_f32EffL0L0(float, float) !dbg !2060 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !2061, metadata !DIExpression()), !dbg !2062
  call void @llvm.dbg.value(metadata float %0, metadata !2061, metadata !DIExpression()), !dbg !2062
  store float %0, float* %3, align 4, !dbg !2062
  call void @llvm.dbg.declare(metadata float* %2, metadata !2063, metadata !DIExpression()), !dbg !2062
  call void @llvm.dbg.value(metadata float %1, metadata !2063, metadata !DIExpression()), !dbg !2062
  store float %1, float* %2, align 4, !dbg !2062
  %4 = load float, float* %3, align 4, !dbg !2064
  %5 = load float, float* %2, align 4, !dbg !2064
  %6 = call float @llvm.minimum.f32(float %4, float %5), !dbg !2064
  ret float %6, !dbg !2064
}

define float @_CNW7builtinE11maximum_f32EffL0L0(float, float) !dbg !2065 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !2066, metadata !DIExpression()), !dbg !2067
  call void @llvm.dbg.value(metadata float %0, metadata !2066, metadata !DIExpression()), !dbg !2067
  store float %0, float* %3, align 4, !dbg !2067
  call void @llvm.dbg.declare(metadata float* %2, metadata !2068, metadata !DIExpression()), !dbg !2067
  call void @llvm.dbg.value(metadata float %1, metadata !2068, metadata !DIExpression()), !dbg !2067
  store float %1, float* %2, align 4, !dbg !2067
  %4 = load float, float* %3, align 4, !dbg !2069
  %5 = load float, float* %2, align 4, !dbg !2069
  %6 = call float @llvm.maximum.f32(float %4, float %5), !dbg !2069
  ret float %6, !dbg !2069
}

define float @_CNW7builtinE12copysign_f32EffL0L0(float, float) !dbg !2070 {
alloca_block:
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %3, metadata !2071, metadata !DIExpression()), !dbg !2072
  call void @llvm.dbg.value(metadata float %0, metadata !2071, metadata !DIExpression()), !dbg !2072
  store float %0, float* %3, align 4, !dbg !2072
  call void @llvm.dbg.declare(metadata float* %2, metadata !2073, metadata !DIExpression()), !dbg !2072
  call void @llvm.dbg.value(metadata float %1, metadata !2073, metadata !DIExpression()), !dbg !2072
  store float %1, float* %2, align 4, !dbg !2072
  %4 = load float, float* %3, align 4, !dbg !2074
  %5 = load float, float* %2, align 4, !dbg !2074
  %6 = call float @llvm.copysign.f32(float %4, float %5), !dbg !2074
  ret float %6, !dbg !2074
}

define float @_CNW7builtinE7fma_f32EfffL0L0L0(float, float, float) !dbg !2075 {
alloca_block:
  %3 = alloca float, align 4
  %4 = alloca float, align 4
  %5 = alloca float, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata float* %5, metadata !2078, metadata !DIExpression()), !dbg !2079
  call void @llvm.dbg.value(metadata float %0, metadata !2078, metadata !DIExpression()), !dbg !2079
  store float %0, float* %5, align 4, !dbg !2079
  call void @llvm.dbg.declare(metadata float* %4, metadata !2080, metadata !DIExpression()), !dbg !2079
  call void @llvm.dbg.value(metadata float %1, metadata !2080, metadata !DIExpression()), !dbg !2079
  store float %1, float* %4, align 4, !dbg !2079
  call void @llvm.dbg.declare(metadata float* %3, metadata !2081, metadata !DIExpression()), !dbg !2079
  call void @llvm.dbg.value(metadata float %2, metadata !2081, metadata !DIExpression()), !dbg !2079
  store float %2, float* %3, align 4, !dbg !2079
  %6 = load float, float* %5, align 4, !dbg !2082
  %7 = load float, float* %4, align 4, !dbg !2082
  %8 = load float, float* %3, align 4, !dbg !2082
  %9 = call float @llvm.fma.f32(float %6, float %7, float %8), !dbg !2082
  ret float %9, !dbg !2082
}

declare double @_CNW7builtinE8fadd_f64EddL0L0(double, double)

declare double @_CNW7builtinE8fsub_f64EddL0L0(double, double)

declare double @_CNW7builtinE8fmul_f64EddL0L0(double, double)

declare double @_CNW7builtinE8fdiv_f64EddL0L0(double, double)

declare double @_CNW7builtinE8frem_f64EddL0L0(double, double)

declare double @_CNW7builtinE8fneg_f64EdL0(double)

define i1 @_CNW7builtinE14fcmp_false_f64EddL0L0(double, double) !dbg !2083 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !2084, metadata !DIExpression()), !dbg !2085
  call void @llvm.dbg.value(metadata double %0, metadata !2084, metadata !DIExpression()), !dbg !2085
  store double %0, double* %3, align 8, !dbg !2085
  call void @llvm.dbg.declare(metadata double* %2, metadata !2086, metadata !DIExpression()), !dbg !2085
  call void @llvm.dbg.value(metadata double %1, metadata !2086, metadata !DIExpression()), !dbg !2085
  store double %1, double* %2, align 8, !dbg !2085
  ret i1 false, !dbg !2087
}

define i1 @_CNW7builtinE13fcmp_true_f64EddL0L0(double, double) !dbg !2088 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !2089, metadata !DIExpression()), !dbg !2090
  call void @llvm.dbg.value(metadata double %0, metadata !2089, metadata !DIExpression()), !dbg !2090
  store double %0, double* %3, align 8, !dbg !2090
  call void @llvm.dbg.declare(metadata double* %2, metadata !2091, metadata !DIExpression()), !dbg !2090
  call void @llvm.dbg.value(metadata double %1, metadata !2091, metadata !DIExpression()), !dbg !2090
  store double %1, double* %2, align 8, !dbg !2090
  ret i1 true, !dbg !2092
}

declare i1 @_CNW7builtinE12fcmp_oeq_f64EddL0L0(double, double)

declare i1 @_CNW7builtinE12fcmp_one_f64EddL0L0(double, double)

declare i1 @_CNW7builtinE12fcmp_ogt_f64EddL0L0(double, double)

declare i1 @_CNW7builtinE12fcmp_oge_f64EddL0L0(double, double)

declare i1 @_CNW7builtinE12fcmp_olt_f64EddL0L0(double, double)

declare i1 @_CNW7builtinE12fcmp_ole_f64EddL0L0(double, double)

declare i1 @_CNW7builtinE12fcmp_ord_f64EddL0L0(double, double)

declare i1 @_CNW7builtinE12fcmp_ueq_f64EddL0L0(double, double)

declare i1 @_CNW7builtinE12fcmp_une_f64EddL0L0(double, double)

declare i1 @_CNW7builtinE12fcmp_ugt_f64EddL0L0(double, double)

declare i1 @_CNW7builtinE12fcmp_uge_f64EddL0L0(double, double)

declare i1 @_CNW7builtinE12fcmp_ult_f64EddL0L0(double, double)

declare i1 @_CNW7builtinE12fcmp_ule_f64EddL0L0(double, double)

declare i1 @_CNW7builtinE12fcmp_uno_f64EddL0L0(double, double)

define double @_CNW7builtinE9log10_f64EdL0(double) !dbg !2093 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !2094, metadata !DIExpression()), !dbg !2095
  call void @llvm.dbg.value(metadata double %0, metadata !2094, metadata !DIExpression()), !dbg !2095
  store double %0, double* %1, align 8, !dbg !2095
  %2 = load double, double* %1, align 8, !dbg !2096
  %3 = call double @llvm.log10.f64(double %2), !dbg !2096
  ret double %3, !dbg !2096
}

define double @_CNW7builtinE8log2_f64EdL0(double) !dbg !2097 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !2098, metadata !DIExpression()), !dbg !2099
  call void @llvm.dbg.value(metadata double %0, metadata !2098, metadata !DIExpression()), !dbg !2099
  store double %0, double* %1, align 8, !dbg !2099
  %2 = load double, double* %1, align 8, !dbg !2100
  %3 = call double @llvm.log2.f64(double %2), !dbg !2100
  ret double %3, !dbg !2100
}

define double @_CNW7builtinE7log_f64EdL0(double) !dbg !2101 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !2102, metadata !DIExpression()), !dbg !2103
  call void @llvm.dbg.value(metadata double %0, metadata !2102, metadata !DIExpression()), !dbg !2103
  store double %0, double* %1, align 8, !dbg !2103
  %2 = load double, double* %1, align 8, !dbg !2104
  %3 = call double @llvm.log1.f64(double %2), !dbg !2104
  ret double %3, !dbg !2104
}

define double @_CNW7builtinE9floor_f64EdL0(double) !dbg !2105 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !2106, metadata !DIExpression()), !dbg !2107
  call void @llvm.dbg.value(metadata double %0, metadata !2106, metadata !DIExpression()), !dbg !2107
  store double %0, double* %1, align 8, !dbg !2107
  %2 = load double, double* %1, align 8, !dbg !2108
  %3 = call double @llvm.floor.f64(double %2), !dbg !2108
  ret double %3, !dbg !2108
}

define double @_CNW7builtinE8ceil_f64EdL0(double) !dbg !2109 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !2110, metadata !DIExpression()), !dbg !2111
  call void @llvm.dbg.value(metadata double %0, metadata !2110, metadata !DIExpression()), !dbg !2111
  store double %0, double* %1, align 8, !dbg !2111
  %2 = load double, double* %1, align 8, !dbg !2112
  %3 = call double @llvm.ceil.f64(double %2), !dbg !2112
  ret double %3, !dbg !2112
}

define double @_CNW7builtinE9round_f64EdL0(double) !dbg !2113 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !2114, metadata !DIExpression()), !dbg !2115
  call void @llvm.dbg.value(metadata double %0, metadata !2114, metadata !DIExpression()), !dbg !2115
  store double %0, double* %1, align 8, !dbg !2115
  %2 = load double, double* %1, align 8, !dbg !2116
  %3 = call double @llvm.round.f64(double %2), !dbg !2116
  ret double %3, !dbg !2116
}

define double @_CNW7builtinE8rint_f64EdL0(double) !dbg !2117 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !2118, metadata !DIExpression()), !dbg !2119
  call void @llvm.dbg.value(metadata double %0, metadata !2118, metadata !DIExpression()), !dbg !2119
  store double %0, double* %1, align 8, !dbg !2119
  %2 = load double, double* %1, align 8, !dbg !2120
  %3 = call double @llvm.rint.f64(double %2), !dbg !2120
  ret double %3, !dbg !2120
}

define double @_CNW7builtinE9trunc_f64EdL0(double) !dbg !2121 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !2122, metadata !DIExpression()), !dbg !2123
  call void @llvm.dbg.value(metadata double %0, metadata !2122, metadata !DIExpression()), !dbg !2123
  store double %0, double* %1, align 8, !dbg !2123
  %2 = load double, double* %1, align 8, !dbg !2124
  %3 = call double @llvm.trunc.f64(double %2), !dbg !2124
  ret double %3, !dbg !2124
}

define double @_CNW7builtinE8sqrt_f64EdL0(double) !dbg !2125 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !2126, metadata !DIExpression()), !dbg !2127
  call void @llvm.dbg.value(metadata double %0, metadata !2126, metadata !DIExpression()), !dbg !2127
  store double %0, double* %1, align 8, !dbg !2127
  %2 = load double, double* %1, align 8, !dbg !2128
  %3 = call double @llvm.sqrt.f64(double %2), !dbg !2128
  ret double %3, !dbg !2128
}

define double @_CNW7builtinE8fabs_f64EdL0(double) !dbg !2129 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !2130, metadata !DIExpression()), !dbg !2131
  call void @llvm.dbg.value(metadata double %0, metadata !2130, metadata !DIExpression()), !dbg !2131
  store double %0, double* %1, align 8, !dbg !2131
  %2 = load double, double* %1, align 8, !dbg !2132
  %3 = call double @llvm.fabs.f64(double %2), !dbg !2132
  ret double %3, !dbg !2132
}

define double @_CNW7builtinE7sin_f64EdL0(double) !dbg !2133 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !2134, metadata !DIExpression()), !dbg !2135
  call void @llvm.dbg.value(metadata double %0, metadata !2134, metadata !DIExpression()), !dbg !2135
  store double %0, double* %1, align 8, !dbg !2135
  %2 = load double, double* %1, align 8, !dbg !2136
  %3 = call double @llvm.sin.f64(double %2), !dbg !2136
  ret double %3, !dbg !2136
}

define double @_CNW7builtinE7cos_f64EdL0(double) !dbg !2137 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !2138, metadata !DIExpression()), !dbg !2139
  call void @llvm.dbg.value(metadata double %0, metadata !2138, metadata !DIExpression()), !dbg !2139
  store double %0, double* %1, align 8, !dbg !2139
  %2 = load double, double* %1, align 8, !dbg !2140
  %3 = call double @llvm.cos.f64(double %2), !dbg !2140
  ret double %3, !dbg !2140
}

define double @_CNW7builtinE7exp_f64EdL0(double) !dbg !2141 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !2142, metadata !DIExpression()), !dbg !2143
  call void @llvm.dbg.value(metadata double %0, metadata !2142, metadata !DIExpression()), !dbg !2143
  store double %0, double* %1, align 8, !dbg !2143
  %2 = load double, double* %1, align 8, !dbg !2144
  %3 = call double @llvm.exp.f64(double %2), !dbg !2144
  ret double %3, !dbg !2144
}

define double @_CNW7builtinE8exp2_f64EdL0(double) !dbg !2145 {
alloca_block:
  %1 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %1, metadata !2146, metadata !DIExpression()), !dbg !2147
  call void @llvm.dbg.value(metadata double %0, metadata !2146, metadata !DIExpression()), !dbg !2147
  store double %0, double* %1, align 8, !dbg !2147
  %2 = load double, double* %1, align 8, !dbg !2148
  %3 = call double @llvm.exp2.f64(double %2), !dbg !2148
  ret double %3, !dbg !2148
}

define double @_CNW7builtinE7pow_f64EddL0L0(double, double) !dbg !2149 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !2150, metadata !DIExpression()), !dbg !2151
  call void @llvm.dbg.value(metadata double %0, metadata !2150, metadata !DIExpression()), !dbg !2151
  store double %0, double* %3, align 8, !dbg !2151
  call void @llvm.dbg.declare(metadata double* %2, metadata !2152, metadata !DIExpression()), !dbg !2151
  call void @llvm.dbg.value(metadata double %1, metadata !2152, metadata !DIExpression()), !dbg !2151
  store double %1, double* %2, align 8, !dbg !2151
  %4 = load double, double* %3, align 8, !dbg !2153
  %5 = load double, double* %2, align 8, !dbg !2153
  %6 = call double @llvm.pow.f64(double %4, double %5), !dbg !2153
  ret double %6, !dbg !2153
}

define double @_CNW7builtinE8powi_f64EdiL0L0(double, i32) !dbg !2154 {
alloca_block:
  %2 = alloca i32, align 4
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !2157, metadata !DIExpression()), !dbg !2158
  call void @llvm.dbg.value(metadata double %0, metadata !2157, metadata !DIExpression()), !dbg !2158
  store double %0, double* %3, align 8, !dbg !2158
  call void @llvm.dbg.declare(metadata i32* %2, metadata !2159, metadata !DIExpression()), !dbg !2158
  call void @llvm.dbg.value(metadata i32 %1, metadata !2159, metadata !DIExpression()), !dbg !2158
  store i32 %1, i32* %2, align 4, !dbg !2158
  %4 = load double, double* %3, align 8, !dbg !2160
  %5 = load i32, i32* %2, align 4, !dbg !2160
  %6 = call double @llvm.powi.f64(double %4, i32 %5), !dbg !2160
  ret double %6, !dbg !2160
}

define double @_CNW7builtinE10minnum_f64EddL0L0(double, double) !dbg !2161 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !2162, metadata !DIExpression()), !dbg !2163
  call void @llvm.dbg.value(metadata double %0, metadata !2162, metadata !DIExpression()), !dbg !2163
  store double %0, double* %3, align 8, !dbg !2163
  call void @llvm.dbg.declare(metadata double* %2, metadata !2164, metadata !DIExpression()), !dbg !2163
  call void @llvm.dbg.value(metadata double %1, metadata !2164, metadata !DIExpression()), !dbg !2163
  store double %1, double* %2, align 8, !dbg !2163
  %4 = load double, double* %3, align 8, !dbg !2165
  %5 = load double, double* %2, align 8, !dbg !2165
  %6 = call double @llvm.minnum.f64(double %4, double %5), !dbg !2165
  ret double %6, !dbg !2165
}

define double @_CNW7builtinE10maxnum_f64EddL0L0(double, double) !dbg !2166 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !2167, metadata !DIExpression()), !dbg !2168
  call void @llvm.dbg.value(metadata double %0, metadata !2167, metadata !DIExpression()), !dbg !2168
  store double %0, double* %3, align 8, !dbg !2168
  call void @llvm.dbg.declare(metadata double* %2, metadata !2169, metadata !DIExpression()), !dbg !2168
  call void @llvm.dbg.value(metadata double %1, metadata !2169, metadata !DIExpression()), !dbg !2168
  store double %1, double* %2, align 8, !dbg !2168
  %4 = load double, double* %3, align 8, !dbg !2170
  %5 = load double, double* %2, align 8, !dbg !2170
  %6 = call double @llvm.maxnum.f64(double %4, double %5), !dbg !2170
  ret double %6, !dbg !2170
}

define double @_CNW7builtinE11minimum_f64EddL0L0(double, double) !dbg !2171 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !2172, metadata !DIExpression()), !dbg !2173
  call void @llvm.dbg.value(metadata double %0, metadata !2172, metadata !DIExpression()), !dbg !2173
  store double %0, double* %3, align 8, !dbg !2173
  call void @llvm.dbg.declare(metadata double* %2, metadata !2174, metadata !DIExpression()), !dbg !2173
  call void @llvm.dbg.value(metadata double %1, metadata !2174, metadata !DIExpression()), !dbg !2173
  store double %1, double* %2, align 8, !dbg !2173
  %4 = load double, double* %3, align 8, !dbg !2175
  %5 = load double, double* %2, align 8, !dbg !2175
  %6 = call double @llvm.minimum.f64(double %4, double %5), !dbg !2175
  ret double %6, !dbg !2175
}

define double @_CNW7builtinE11maximum_f64EddL0L0(double, double) !dbg !2176 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !2177, metadata !DIExpression()), !dbg !2178
  call void @llvm.dbg.value(metadata double %0, metadata !2177, metadata !DIExpression()), !dbg !2178
  store double %0, double* %3, align 8, !dbg !2178
  call void @llvm.dbg.declare(metadata double* %2, metadata !2179, metadata !DIExpression()), !dbg !2178
  call void @llvm.dbg.value(metadata double %1, metadata !2179, metadata !DIExpression()), !dbg !2178
  store double %1, double* %2, align 8, !dbg !2178
  %4 = load double, double* %3, align 8, !dbg !2180
  %5 = load double, double* %2, align 8, !dbg !2180
  %6 = call double @llvm.maximum.f64(double %4, double %5), !dbg !2180
  ret double %6, !dbg !2180
}

define double @_CNW7builtinE12copysign_f64EddL0L0(double, double) !dbg !2181 {
alloca_block:
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %3, metadata !2182, metadata !DIExpression()), !dbg !2183
  call void @llvm.dbg.value(metadata double %0, metadata !2182, metadata !DIExpression()), !dbg !2183
  store double %0, double* %3, align 8, !dbg !2183
  call void @llvm.dbg.declare(metadata double* %2, metadata !2184, metadata !DIExpression()), !dbg !2183
  call void @llvm.dbg.value(metadata double %1, metadata !2184, metadata !DIExpression()), !dbg !2183
  store double %1, double* %2, align 8, !dbg !2183
  %4 = load double, double* %3, align 8, !dbg !2185
  %5 = load double, double* %2, align 8, !dbg !2185
  %6 = call double @llvm.copysign.f64(double %4, double %5), !dbg !2185
  ret double %6, !dbg !2185
}

define double @_CNW7builtinE7fma_f64EdddL0L0L0(double, double, double) !dbg !2186 {
alloca_block:
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  %5 = alloca double, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata double* %5, metadata !2189, metadata !DIExpression()), !dbg !2190
  call void @llvm.dbg.value(metadata double %0, metadata !2189, metadata !DIExpression()), !dbg !2190
  store double %0, double* %5, align 8, !dbg !2190
  call void @llvm.dbg.declare(metadata double* %4, metadata !2191, metadata !DIExpression()), !dbg !2190
  call void @llvm.dbg.value(metadata double %1, metadata !2191, metadata !DIExpression()), !dbg !2190
  store double %1, double* %4, align 8, !dbg !2190
  call void @llvm.dbg.declare(metadata double* %3, metadata !2192, metadata !DIExpression()), !dbg !2190
  call void @llvm.dbg.value(metadata double %2, metadata !2192, metadata !DIExpression()), !dbg !2190
  store double %2, double* %3, align 8, !dbg !2190
  %6 = load double, double* %5, align 8, !dbg !2193
  %7 = load double, double* %4, align 8, !dbg !2193
  %8 = load double, double* %3, align 8, !dbg !2193
  %9 = call double @llvm.fma.f64(double %6, double %7, double %8), !dbg !2193
  ret double %9, !dbg !2193
}

define i1 @_CNW7builtinE7ctlz_i1EbL0(i1) !dbg !2194 {
entry:
  %1 = alloca i1, align 1
  call void @llvm.dbg.declare(metadata i1* %1, metadata !2195, metadata !DIExpression()), !dbg !2196
  call void @llvm.dbg.value(metadata i1 %0, metadata !2195, metadata !DIExpression()), !dbg !2196
  store i1 %0, i1* %1, align 1, !dbg !2196
  %2 = load i1, i1* %1, align 1, !dbg !2197
  %cmpz = icmp eq i1 %2, false, !dbg !2197
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2197

cond.false:                                       ; preds = %entry
  %3 = call i1 @llvm.ctlz.i1(i1 %2, i1 true), !dbg !2197
  br label %cond.end, !dbg !2197

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i1 [ true, %entry ], [ %3, %cond.false ], !dbg !2197
  ret i1 %ctz, !dbg !2197
}

define i1 @_CNW7builtinE7cttz_i1EbL0(i1) !dbg !2198 {
entry:
  %1 = alloca i1, align 1
  call void @llvm.dbg.declare(metadata i1* %1, metadata !2199, metadata !DIExpression()), !dbg !2200
  call void @llvm.dbg.value(metadata i1 %0, metadata !2199, metadata !DIExpression()), !dbg !2200
  store i1 %0, i1* %1, align 1, !dbg !2200
  %2 = load i1, i1* %1, align 1, !dbg !2201
  %cmpz = icmp eq i1 %2, false, !dbg !2201
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2201

cond.false:                                       ; preds = %entry
  %3 = call i1 @llvm.cttz.i1(i1 %2, i1 true), !dbg !2201
  br label %cond.end, !dbg !2201

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i1 [ true, %entry ], [ %3, %cond.false ], !dbg !2201
  ret i1 %ctz, !dbg !2201
}

define i1 @_CNW7builtinE8ctpop_i1EbL0(i1) !dbg !2202 {
alloca_block:
  %1 = alloca i1, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i1* %1, metadata !2203, metadata !DIExpression()), !dbg !2204
  call void @llvm.dbg.value(metadata i1 %0, metadata !2203, metadata !DIExpression()), !dbg !2204
  store i1 %0, i1* %1, align 1, !dbg !2204
  %2 = load i1, i1* %1, align 1, !dbg !2205
  %3 = call i1 @llvm.ctpop.i1(i1 %2), !dbg !2205
  ret i1 %3, !dbg !2205
}

define i1 @_CNW7builtinE13bitreverse_i1EbL0(i1) !dbg !2206 {
alloca_block:
  %1 = alloca i1, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i1* %1, metadata !2207, metadata !DIExpression()), !dbg !2208
  call void @llvm.dbg.value(metadata i1 %0, metadata !2207, metadata !DIExpression()), !dbg !2208
  store i1 %0, i1* %1, align 1, !dbg !2208
  %2 = load i1, i1* %1, align 1, !dbg !2209
  %3 = call i1 @llvm.bitreverse.i1(i1 %2), !dbg !2209
  ret i1 %3, !dbg !2209
}

define i1 @_CNW7builtinE6neg_i1EbL0(i1) !dbg !2210 {
alloca_block:
  %1 = alloca i1, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i1* %1, metadata !2211, metadata !DIExpression()), !dbg !2212
  call void @llvm.dbg.value(metadata i1 %0, metadata !2211, metadata !DIExpression()), !dbg !2212
  store i1 %0, i1* %1, align 1, !dbg !2212
  %2 = load i1, i1* %1, align 1, !dbg !2213
  %3 = sub nuw i1 false, %2, !dbg !2213
  ret i1 %3, !dbg !2213
}

define i1 @_CNW7builtinE5id_i1EbL0(i1) !dbg !2214 {
alloca_block:
  %1 = alloca i1, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i1* %1, metadata !2215, metadata !DIExpression()), !dbg !2216
  call void @llvm.dbg.value(metadata i1 %0, metadata !2215, metadata !DIExpression()), !dbg !2216
  store i1 %0, i1* %1, align 1, !dbg !2216
  %2 = load i1, i1* %1, align 1, !dbg !2217
  ret i1 %2, !dbg !2217
}

declare i1 @_CNW7builtinE6add_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE6sub_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE6mul_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE6div_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE6rem_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE6shl_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE7lshr_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE7ashr_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE6and_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE5or_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE6xor_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE10icmp_eq_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE10icmp_ne_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE11icmp_ugt_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE11icmp_uge_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE11icmp_ult_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE11icmp_ule_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE11icmp_sgt_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE11icmp_sge_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE11icmp_slt_i1EbbL0L0(i1, i1)

declare i1 @_CNW7builtinE11icmp_sle_i1EbbL0L0(i1, i1)

define { i1, i1 } @_CNW7builtinE21sadd_with_overflow_i1EbbL0L0(i1, i1) !dbg !2218 {
alloca_block:
  %2 = alloca { i1, i1 }, align 1
  %3 = alloca i1, align 1
  %4 = alloca i1, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i1* %4, metadata !2221, metadata !DIExpression()), !dbg !2222
  call void @llvm.dbg.value(metadata i1 %0, metadata !2221, metadata !DIExpression()), !dbg !2222
  store i1 %0, i1* %4, align 1, !dbg !2222
  call void @llvm.dbg.declare(metadata i1* %3, metadata !2223, metadata !DIExpression()), !dbg !2222
  call void @llvm.dbg.value(metadata i1 %1, metadata !2223, metadata !DIExpression()), !dbg !2222
  store i1 %1, i1* %3, align 1, !dbg !2222
  %5 = load i1, i1* %4, align 1, !dbg !2224
  %6 = load i1, i1* %3, align 1, !dbg !2224
  %7 = call { i1, i1 } @llvm.sadd.with.overflow.i1(i1 %5, i1 %6), !dbg !2224
  store { i1, i1 } %7, { i1, i1 }* %2, align 1, !dbg !2224
  %8 = load { i1, i1 }, { i1, i1 }* %2, align 1, !dbg !2224
  ret { i1, i1 } %8, !dbg !2224
}

define { i1, i1 } @_CNW7builtinE21uadd_with_overflow_i1EbbL0L0(i1, i1) !dbg !2225 {
alloca_block:
  %2 = alloca { i1, i1 }, align 1
  %3 = alloca i1, align 1
  %4 = alloca i1, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i1* %4, metadata !2226, metadata !DIExpression()), !dbg !2227
  call void @llvm.dbg.value(metadata i1 %0, metadata !2226, metadata !DIExpression()), !dbg !2227
  store i1 %0, i1* %4, align 1, !dbg !2227
  call void @llvm.dbg.declare(metadata i1* %3, metadata !2228, metadata !DIExpression()), !dbg !2227
  call void @llvm.dbg.value(metadata i1 %1, metadata !2228, metadata !DIExpression()), !dbg !2227
  store i1 %1, i1* %3, align 1, !dbg !2227
  %5 = load i1, i1* %4, align 1, !dbg !2229
  %6 = load i1, i1* %3, align 1, !dbg !2229
  %7 = call { i1, i1 } @llvm.uadd.with.overflow.i1(i1 %5, i1 %6), !dbg !2229
  store { i1, i1 } %7, { i1, i1 }* %2, align 1, !dbg !2229
  %8 = load { i1, i1 }, { i1, i1 }* %2, align 1, !dbg !2229
  ret { i1, i1 } %8, !dbg !2229
}

define { i1, i1 } @_CNW7builtinE21ssub_with_overflow_i1EbbL0L0(i1, i1) !dbg !2230 {
alloca_block:
  %2 = alloca { i1, i1 }, align 1
  %3 = alloca i1, align 1
  %4 = alloca i1, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i1* %4, metadata !2231, metadata !DIExpression()), !dbg !2232
  call void @llvm.dbg.value(metadata i1 %0, metadata !2231, metadata !DIExpression()), !dbg !2232
  store i1 %0, i1* %4, align 1, !dbg !2232
  call void @llvm.dbg.declare(metadata i1* %3, metadata !2233, metadata !DIExpression()), !dbg !2232
  call void @llvm.dbg.value(metadata i1 %1, metadata !2233, metadata !DIExpression()), !dbg !2232
  store i1 %1, i1* %3, align 1, !dbg !2232
  %5 = load i1, i1* %4, align 1, !dbg !2234
  %6 = load i1, i1* %3, align 1, !dbg !2234
  %7 = call { i1, i1 } @llvm.ssub.with.overflow.i1(i1 %5, i1 %6), !dbg !2234
  store { i1, i1 } %7, { i1, i1 }* %2, align 1, !dbg !2234
  %8 = load { i1, i1 }, { i1, i1 }* %2, align 1, !dbg !2234
  ret { i1, i1 } %8, !dbg !2234
}

define { i1, i1 } @_CNW7builtinE21usub_with_overflow_i1EbbL0L0(i1, i1) !dbg !2235 {
alloca_block:
  %2 = alloca { i1, i1 }, align 1
  %3 = alloca i1, align 1
  %4 = alloca i1, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i1* %4, metadata !2236, metadata !DIExpression()), !dbg !2237
  call void @llvm.dbg.value(metadata i1 %0, metadata !2236, metadata !DIExpression()), !dbg !2237
  store i1 %0, i1* %4, align 1, !dbg !2237
  call void @llvm.dbg.declare(metadata i1* %3, metadata !2238, metadata !DIExpression()), !dbg !2237
  call void @llvm.dbg.value(metadata i1 %1, metadata !2238, metadata !DIExpression()), !dbg !2237
  store i1 %1, i1* %3, align 1, !dbg !2237
  %5 = load i1, i1* %4, align 1, !dbg !2239
  %6 = load i1, i1* %3, align 1, !dbg !2239
  %7 = call { i1, i1 } @llvm.usub.with.overflow.i1(i1 %5, i1 %6), !dbg !2239
  store { i1, i1 } %7, { i1, i1 }* %2, align 1, !dbg !2239
  %8 = load { i1, i1 }, { i1, i1 }* %2, align 1, !dbg !2239
  ret { i1, i1 } %8, !dbg !2239
}

define { i1, i1 } @_CNW7builtinE21smul_with_overflow_i1EbbL0L0(i1, i1) !dbg !2240 {
alloca_block:
  %2 = alloca { i1, i1 }, align 1
  %3 = alloca i1, align 1
  %4 = alloca i1, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i1* %4, metadata !2241, metadata !DIExpression()), !dbg !2242
  call void @llvm.dbg.value(metadata i1 %0, metadata !2241, metadata !DIExpression()), !dbg !2242
  store i1 %0, i1* %4, align 1, !dbg !2242
  call void @llvm.dbg.declare(metadata i1* %3, metadata !2243, metadata !DIExpression()), !dbg !2242
  call void @llvm.dbg.value(metadata i1 %1, metadata !2243, metadata !DIExpression()), !dbg !2242
  store i1 %1, i1* %3, align 1, !dbg !2242
  %5 = load i1, i1* %4, align 1, !dbg !2244
  %6 = load i1, i1* %3, align 1, !dbg !2244
  %7 = call { i1, i1 } @llvm.smul.with.overflow.i1(i1 %5, i1 %6), !dbg !2244
  store { i1, i1 } %7, { i1, i1 }* %2, align 1, !dbg !2244
  %8 = load { i1, i1 }, { i1, i1 }* %2, align 1, !dbg !2244
  ret { i1, i1 } %8, !dbg !2244
}

define { i1, i1 } @_CNW7builtinE21umul_with_overflow_i1EbbL0L0(i1, i1) !dbg !2245 {
alloca_block:
  %2 = alloca { i1, i1 }, align 1
  %3 = alloca i1, align 1
  %4 = alloca i1, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i1* %4, metadata !2246, metadata !DIExpression()), !dbg !2247
  call void @llvm.dbg.value(metadata i1 %0, metadata !2246, metadata !DIExpression()), !dbg !2247
  store i1 %0, i1* %4, align 1, !dbg !2247
  call void @llvm.dbg.declare(metadata i1* %3, metadata !2248, metadata !DIExpression()), !dbg !2247
  call void @llvm.dbg.value(metadata i1 %1, metadata !2248, metadata !DIExpression()), !dbg !2247
  store i1 %1, i1* %3, align 1, !dbg !2247
  %5 = load i1, i1* %4, align 1, !dbg !2249
  %6 = load i1, i1* %3, align 1, !dbg !2249
  %7 = call { i1, i1 } @llvm.umul.with.overflow.i1(i1 %5, i1 %6), !dbg !2249
  store { i1, i1 } %7, { i1, i1 }* %2, align 1, !dbg !2249
  %8 = load { i1, i1 }, { i1, i1 }* %2, align 1, !dbg !2249
  ret { i1, i1 } %8, !dbg !2249
}

define i8 @_CNW7builtinE7ctlz_i8EaL0(i8) !dbg !2250 {
entry:
  %1 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %1, metadata !2251, metadata !DIExpression()), !dbg !2252
  call void @llvm.dbg.value(metadata i8 %0, metadata !2251, metadata !DIExpression()), !dbg !2252
  store i8 %0, i8* %1, align 1, !dbg !2252
  %2 = load i8, i8* %1, align 1, !dbg !2253
  %cmpz = icmp eq i8 %2, 0, !dbg !2253
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2253

cond.false:                                       ; preds = %entry
  %3 = call i8 @llvm.ctlz.i8(i8 %2, i1 true), !dbg !2253
  br label %cond.end, !dbg !2253

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i8 [ 8, %entry ], [ %3, %cond.false ], !dbg !2253
  ret i8 %ctz, !dbg !2253
}

define i8 @_CNW7builtinE7cttz_i8EaL0(i8) !dbg !2254 {
entry:
  %1 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %1, metadata !2255, metadata !DIExpression()), !dbg !2256
  call void @llvm.dbg.value(metadata i8 %0, metadata !2255, metadata !DIExpression()), !dbg !2256
  store i8 %0, i8* %1, align 1, !dbg !2256
  %2 = load i8, i8* %1, align 1, !dbg !2257
  %cmpz = icmp eq i8 %2, 0, !dbg !2257
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2257

cond.false:                                       ; preds = %entry
  %3 = call i8 @llvm.cttz.i8(i8 %2, i1 true), !dbg !2257
  br label %cond.end, !dbg !2257

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i8 [ 8, %entry ], [ %3, %cond.false ], !dbg !2257
  ret i8 %ctz, !dbg !2257
}

define i8 @_CNW7builtinE8ctpop_i8EaL0(i8) !dbg !2258 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !2259, metadata !DIExpression()), !dbg !2260
  call void @llvm.dbg.value(metadata i8 %0, metadata !2259, metadata !DIExpression()), !dbg !2260
  store i8 %0, i8* %1, align 1, !dbg !2260
  %2 = load i8, i8* %1, align 1, !dbg !2261
  %3 = call i8 @llvm.ctpop.i8(i8 %2), !dbg !2261
  ret i8 %3, !dbg !2261
}

define i8 @_CNW7builtinE13bitreverse_i8EaL0(i8) !dbg !2262 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !2263, metadata !DIExpression()), !dbg !2264
  call void @llvm.dbg.value(metadata i8 %0, metadata !2263, metadata !DIExpression()), !dbg !2264
  store i8 %0, i8* %1, align 1, !dbg !2264
  %2 = load i8, i8* %1, align 1, !dbg !2265
  %3 = call i8 @llvm.bitreverse.i8(i8 %2), !dbg !2265
  ret i8 %3, !dbg !2265
}

declare i8 @_CNW7builtinE6add_i8EaaL0L0(i8, i8)

declare i8 @_CNW7builtinE6sub_i8EaaL0L0(i8, i8)

declare i8 @_CNW7builtinE6mul_i8EaaL0L0(i8, i8)

declare i8 @_CNW7builtinE6div_i8EaaL0L0(i8, i8)

declare i8 @_CNW7builtinE6rem_i8EaaL0L0(i8, i8)

declare i8 @_CNW7builtinE6shl_i8EaaL0L0(i8, i8)

declare i8 @_CNW7builtinE7lshr_i8EaaL0L0(i8, i8)

declare i8 @_CNW7builtinE7ashr_i8EaaL0L0(i8, i8)

declare i8 @_CNW7builtinE6and_i8EaaL0L0(i8, i8)

declare i8 @_CNW7builtinE5or_i8EaaL0L0(i8, i8)

declare i8 @_CNW7builtinE6xor_i8EaaL0L0(i8, i8)

declare i1 @_CNW7builtinE10icmp_eq_i8EaaL0L0(i8, i8)

declare i1 @_CNW7builtinE10icmp_ne_i8EaaL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_ugt_i8EaaL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_uge_i8EaaL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_ult_i8EaaL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_ule_i8EaaL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_sgt_i8EaaL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_sge_i8EaaL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_slt_i8EaaL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_sle_i8EaaL0L0(i8, i8)

define { i8, i1 } @_CNW7builtinE21sadd_with_overflow_i8EaaL0L0(i8, i8) !dbg !2266 {
alloca_block:
  %2 = alloca { i8, i1 }, align 1
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %4, metadata !2267, metadata !DIExpression()), !dbg !2268
  call void @llvm.dbg.value(metadata i8 %0, metadata !2267, metadata !DIExpression()), !dbg !2268
  store i8 %0, i8* %4, align 1, !dbg !2268
  call void @llvm.dbg.declare(metadata i8* %3, metadata !2269, metadata !DIExpression()), !dbg !2268
  call void @llvm.dbg.value(metadata i8 %1, metadata !2269, metadata !DIExpression()), !dbg !2268
  store i8 %1, i8* %3, align 1, !dbg !2268
  %5 = load i8, i8* %4, align 1, !dbg !2270
  %6 = load i8, i8* %3, align 1, !dbg !2270
  %7 = call { i8, i1 } @llvm.sadd.with.overflow.i8(i8 %5, i8 %6), !dbg !2270
  store { i8, i1 } %7, { i8, i1 }* %2, align 1, !dbg !2270
  %8 = load { i8, i1 }, { i8, i1 }* %2, align 1, !dbg !2270
  ret { i8, i1 } %8, !dbg !2270
}

define { i8, i1 } @_CNW7builtinE21uadd_with_overflow_i8EaaL0L0(i8, i8) !dbg !2271 {
alloca_block:
  %2 = alloca { i8, i1 }, align 1
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %4, metadata !2272, metadata !DIExpression()), !dbg !2273
  call void @llvm.dbg.value(metadata i8 %0, metadata !2272, metadata !DIExpression()), !dbg !2273
  store i8 %0, i8* %4, align 1, !dbg !2273
  call void @llvm.dbg.declare(metadata i8* %3, metadata !2274, metadata !DIExpression()), !dbg !2273
  call void @llvm.dbg.value(metadata i8 %1, metadata !2274, metadata !DIExpression()), !dbg !2273
  store i8 %1, i8* %3, align 1, !dbg !2273
  %5 = load i8, i8* %4, align 1, !dbg !2275
  %6 = load i8, i8* %3, align 1, !dbg !2275
  %7 = call { i8, i1 } @llvm.uadd.with.overflow.i8(i8 %5, i8 %6), !dbg !2275
  store { i8, i1 } %7, { i8, i1 }* %2, align 1, !dbg !2275
  %8 = load { i8, i1 }, { i8, i1 }* %2, align 1, !dbg !2275
  ret { i8, i1 } %8, !dbg !2275
}

define { i8, i1 } @_CNW7builtinE21ssub_with_overflow_i8EaaL0L0(i8, i8) !dbg !2276 {
alloca_block:
  %2 = alloca { i8, i1 }, align 1
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %4, metadata !2277, metadata !DIExpression()), !dbg !2278
  call void @llvm.dbg.value(metadata i8 %0, metadata !2277, metadata !DIExpression()), !dbg !2278
  store i8 %0, i8* %4, align 1, !dbg !2278
  call void @llvm.dbg.declare(metadata i8* %3, metadata !2279, metadata !DIExpression()), !dbg !2278
  call void @llvm.dbg.value(metadata i8 %1, metadata !2279, metadata !DIExpression()), !dbg !2278
  store i8 %1, i8* %3, align 1, !dbg !2278
  %5 = load i8, i8* %4, align 1, !dbg !2280
  %6 = load i8, i8* %3, align 1, !dbg !2280
  %7 = call { i8, i1 } @llvm.ssub.with.overflow.i8(i8 %5, i8 %6), !dbg !2280
  store { i8, i1 } %7, { i8, i1 }* %2, align 1, !dbg !2280
  %8 = load { i8, i1 }, { i8, i1 }* %2, align 1, !dbg !2280
  ret { i8, i1 } %8, !dbg !2280
}

define { i8, i1 } @_CNW7builtinE21usub_with_overflow_i8EaaL0L0(i8, i8) !dbg !2281 {
alloca_block:
  %2 = alloca { i8, i1 }, align 1
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %4, metadata !2282, metadata !DIExpression()), !dbg !2283
  call void @llvm.dbg.value(metadata i8 %0, metadata !2282, metadata !DIExpression()), !dbg !2283
  store i8 %0, i8* %4, align 1, !dbg !2283
  call void @llvm.dbg.declare(metadata i8* %3, metadata !2284, metadata !DIExpression()), !dbg !2283
  call void @llvm.dbg.value(metadata i8 %1, metadata !2284, metadata !DIExpression()), !dbg !2283
  store i8 %1, i8* %3, align 1, !dbg !2283
  %5 = load i8, i8* %4, align 1, !dbg !2285
  %6 = load i8, i8* %3, align 1, !dbg !2285
  %7 = call { i8, i1 } @llvm.usub.with.overflow.i8(i8 %5, i8 %6), !dbg !2285
  store { i8, i1 } %7, { i8, i1 }* %2, align 1, !dbg !2285
  %8 = load { i8, i1 }, { i8, i1 }* %2, align 1, !dbg !2285
  ret { i8, i1 } %8, !dbg !2285
}

define { i8, i1 } @_CNW7builtinE21smul_with_overflow_i8EaaL0L0(i8, i8) !dbg !2286 {
alloca_block:
  %2 = alloca { i8, i1 }, align 1
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %4, metadata !2287, metadata !DIExpression()), !dbg !2288
  call void @llvm.dbg.value(metadata i8 %0, metadata !2287, metadata !DIExpression()), !dbg !2288
  store i8 %0, i8* %4, align 1, !dbg !2288
  call void @llvm.dbg.declare(metadata i8* %3, metadata !2289, metadata !DIExpression()), !dbg !2288
  call void @llvm.dbg.value(metadata i8 %1, metadata !2289, metadata !DIExpression()), !dbg !2288
  store i8 %1, i8* %3, align 1, !dbg !2288
  %5 = load i8, i8* %4, align 1, !dbg !2290
  %6 = load i8, i8* %3, align 1, !dbg !2290
  %7 = call { i8, i1 } @llvm.smul.with.overflow.i8(i8 %5, i8 %6), !dbg !2290
  store { i8, i1 } %7, { i8, i1 }* %2, align 1, !dbg !2290
  %8 = load { i8, i1 }, { i8, i1 }* %2, align 1, !dbg !2290
  ret { i8, i1 } %8, !dbg !2290
}

define { i8, i1 } @_CNW7builtinE21umul_with_overflow_i8EaaL0L0(i8, i8) !dbg !2291 {
alloca_block:
  %2 = alloca { i8, i1 }, align 1
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %4, metadata !2292, metadata !DIExpression()), !dbg !2293
  call void @llvm.dbg.value(metadata i8 %0, metadata !2292, metadata !DIExpression()), !dbg !2293
  store i8 %0, i8* %4, align 1, !dbg !2293
  call void @llvm.dbg.declare(metadata i8* %3, metadata !2294, metadata !DIExpression()), !dbg !2293
  call void @llvm.dbg.value(metadata i8 %1, metadata !2294, metadata !DIExpression()), !dbg !2293
  store i8 %1, i8* %3, align 1, !dbg !2293
  %5 = load i8, i8* %4, align 1, !dbg !2295
  %6 = load i8, i8* %3, align 1, !dbg !2295
  %7 = call { i8, i1 } @llvm.umul.with.overflow.i8(i8 %5, i8 %6), !dbg !2295
  store { i8, i1 } %7, { i8, i1 }* %2, align 1, !dbg !2295
  %8 = load { i8, i1 }, { i8, i1 }* %2, align 1, !dbg !2295
  ret { i8, i1 } %8, !dbg !2295
}

define i16 @_CNW7builtinE8ctlz_i16EsL0(i16) !dbg !2296 {
entry:
  %1 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %1, metadata !2297, metadata !DIExpression()), !dbg !2298
  call void @llvm.dbg.value(metadata i16 %0, metadata !2297, metadata !DIExpression()), !dbg !2298
  store i16 %0, i16* %1, align 2, !dbg !2298
  %2 = load i16, i16* %1, align 2, !dbg !2299
  %cmpz = icmp eq i16 %2, 0, !dbg !2299
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2299

cond.false:                                       ; preds = %entry
  %3 = call i16 @llvm.ctlz.i16(i16 %2, i1 true), !dbg !2299
  br label %cond.end, !dbg !2299

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i16 [ 16, %entry ], [ %3, %cond.false ], !dbg !2299
  ret i16 %ctz, !dbg !2299
}

define i16 @_CNW7builtinE8cttz_i16EsL0(i16) !dbg !2300 {
entry:
  %1 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %1, metadata !2301, metadata !DIExpression()), !dbg !2302
  call void @llvm.dbg.value(metadata i16 %0, metadata !2301, metadata !DIExpression()), !dbg !2302
  store i16 %0, i16* %1, align 2, !dbg !2302
  %2 = load i16, i16* %1, align 2, !dbg !2303
  %cmpz = icmp eq i16 %2, 0, !dbg !2303
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2303

cond.false:                                       ; preds = %entry
  %3 = call i16 @llvm.cttz.i16(i16 %2, i1 true), !dbg !2303
  br label %cond.end, !dbg !2303

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i16 [ 16, %entry ], [ %3, %cond.false ], !dbg !2303
  ret i16 %ctz, !dbg !2303
}

define i16 @_CNW7builtinE9ctpop_i16EsL0(i16) !dbg !2304 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !2305, metadata !DIExpression()), !dbg !2306
  call void @llvm.dbg.value(metadata i16 %0, metadata !2305, metadata !DIExpression()), !dbg !2306
  store i16 %0, i16* %1, align 2, !dbg !2306
  %2 = load i16, i16* %1, align 2, !dbg !2307
  %3 = call i16 @llvm.ctpop.i16(i16 %2), !dbg !2307
  ret i16 %3, !dbg !2307
}

define i16 @_CNW7builtinE14bitreverse_i16EsL0(i16) !dbg !2308 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !2309, metadata !DIExpression()), !dbg !2310
  call void @llvm.dbg.value(metadata i16 %0, metadata !2309, metadata !DIExpression()), !dbg !2310
  store i16 %0, i16* %1, align 2, !dbg !2310
  %2 = load i16, i16* %1, align 2, !dbg !2311
  %3 = call i16 @llvm.bitreverse.i16(i16 %2), !dbg !2311
  ret i16 %3, !dbg !2311
}

declare i16 @_CNW7builtinE7add_i16EssL0L0(i16, i16)

declare i16 @_CNW7builtinE7sub_i16EssL0L0(i16, i16)

declare i16 @_CNW7builtinE7mul_i16EssL0L0(i16, i16)

declare i16 @_CNW7builtinE7div_i16EssL0L0(i16, i16)

declare i16 @_CNW7builtinE7rem_i16EssL0L0(i16, i16)

declare i16 @_CNW7builtinE7shl_i16EssL0L0(i16, i16)

declare i16 @_CNW7builtinE8lshr_i16EssL0L0(i16, i16)

declare i16 @_CNW7builtinE8ashr_i16EssL0L0(i16, i16)

declare i16 @_CNW7builtinE7and_i16EssL0L0(i16, i16)

declare i16 @_CNW7builtinE6or_i16EssL0L0(i16, i16)

declare i16 @_CNW7builtinE7xor_i16EssL0L0(i16, i16)

declare i1 @_CNW7builtinE11icmp_eq_i16EssL0L0(i16, i16)

declare i1 @_CNW7builtinE11icmp_ne_i16EssL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_ugt_i16EssL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_uge_i16EssL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_ult_i16EssL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_ule_i16EssL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_sgt_i16EssL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_sge_i16EssL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_slt_i16EssL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_sle_i16EssL0L0(i16, i16)

define { i16, i1 } @_CNW7builtinE22sadd_with_overflow_i16EssL0L0(i16, i16) !dbg !2312 {
alloca_block:
  %2 = alloca { i16, i1 }, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %4, metadata !2313, metadata !DIExpression()), !dbg !2314
  call void @llvm.dbg.value(metadata i16 %0, metadata !2313, metadata !DIExpression()), !dbg !2314
  store i16 %0, i16* %4, align 2, !dbg !2314
  call void @llvm.dbg.declare(metadata i16* %3, metadata !2315, metadata !DIExpression()), !dbg !2314
  call void @llvm.dbg.value(metadata i16 %1, metadata !2315, metadata !DIExpression()), !dbg !2314
  store i16 %1, i16* %3, align 2, !dbg !2314
  %5 = load i16, i16* %4, align 2, !dbg !2316
  %6 = load i16, i16* %3, align 2, !dbg !2316
  %7 = call { i16, i1 } @llvm.sadd.with.overflow.i16(i16 %5, i16 %6), !dbg !2316
  store { i16, i1 } %7, { i16, i1 }* %2, align 2, !dbg !2316
  %8 = load { i16, i1 }, { i16, i1 }* %2, align 2, !dbg !2316
  ret { i16, i1 } %8, !dbg !2316
}

define { i16, i1 } @_CNW7builtinE22uadd_with_overflow_i16EssL0L0(i16, i16) !dbg !2317 {
alloca_block:
  %2 = alloca { i16, i1 }, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %4, metadata !2318, metadata !DIExpression()), !dbg !2319
  call void @llvm.dbg.value(metadata i16 %0, metadata !2318, metadata !DIExpression()), !dbg !2319
  store i16 %0, i16* %4, align 2, !dbg !2319
  call void @llvm.dbg.declare(metadata i16* %3, metadata !2320, metadata !DIExpression()), !dbg !2319
  call void @llvm.dbg.value(metadata i16 %1, metadata !2320, metadata !DIExpression()), !dbg !2319
  store i16 %1, i16* %3, align 2, !dbg !2319
  %5 = load i16, i16* %4, align 2, !dbg !2321
  %6 = load i16, i16* %3, align 2, !dbg !2321
  %7 = call { i16, i1 } @llvm.uadd.with.overflow.i16(i16 %5, i16 %6), !dbg !2321
  store { i16, i1 } %7, { i16, i1 }* %2, align 2, !dbg !2321
  %8 = load { i16, i1 }, { i16, i1 }* %2, align 2, !dbg !2321
  ret { i16, i1 } %8, !dbg !2321
}

define { i16, i1 } @_CNW7builtinE22ssub_with_overflow_i16EssL0L0(i16, i16) !dbg !2322 {
alloca_block:
  %2 = alloca { i16, i1 }, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %4, metadata !2323, metadata !DIExpression()), !dbg !2324
  call void @llvm.dbg.value(metadata i16 %0, metadata !2323, metadata !DIExpression()), !dbg !2324
  store i16 %0, i16* %4, align 2, !dbg !2324
  call void @llvm.dbg.declare(metadata i16* %3, metadata !2325, metadata !DIExpression()), !dbg !2324
  call void @llvm.dbg.value(metadata i16 %1, metadata !2325, metadata !DIExpression()), !dbg !2324
  store i16 %1, i16* %3, align 2, !dbg !2324
  %5 = load i16, i16* %4, align 2, !dbg !2326
  %6 = load i16, i16* %3, align 2, !dbg !2326
  %7 = call { i16, i1 } @llvm.ssub.with.overflow.i16(i16 %5, i16 %6), !dbg !2326
  store { i16, i1 } %7, { i16, i1 }* %2, align 2, !dbg !2326
  %8 = load { i16, i1 }, { i16, i1 }* %2, align 2, !dbg !2326
  ret { i16, i1 } %8, !dbg !2326
}

define { i16, i1 } @_CNW7builtinE22usub_with_overflow_i16EssL0L0(i16, i16) !dbg !2327 {
alloca_block:
  %2 = alloca { i16, i1 }, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %4, metadata !2328, metadata !DIExpression()), !dbg !2329
  call void @llvm.dbg.value(metadata i16 %0, metadata !2328, metadata !DIExpression()), !dbg !2329
  store i16 %0, i16* %4, align 2, !dbg !2329
  call void @llvm.dbg.declare(metadata i16* %3, metadata !2330, metadata !DIExpression()), !dbg !2329
  call void @llvm.dbg.value(metadata i16 %1, metadata !2330, metadata !DIExpression()), !dbg !2329
  store i16 %1, i16* %3, align 2, !dbg !2329
  %5 = load i16, i16* %4, align 2, !dbg !2331
  %6 = load i16, i16* %3, align 2, !dbg !2331
  %7 = call { i16, i1 } @llvm.usub.with.overflow.i16(i16 %5, i16 %6), !dbg !2331
  store { i16, i1 } %7, { i16, i1 }* %2, align 2, !dbg !2331
  %8 = load { i16, i1 }, { i16, i1 }* %2, align 2, !dbg !2331
  ret { i16, i1 } %8, !dbg !2331
}

define { i16, i1 } @_CNW7builtinE22smul_with_overflow_i16EssL0L0(i16, i16) !dbg !2332 {
alloca_block:
  %2 = alloca { i16, i1 }, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %4, metadata !2333, metadata !DIExpression()), !dbg !2334
  call void @llvm.dbg.value(metadata i16 %0, metadata !2333, metadata !DIExpression()), !dbg !2334
  store i16 %0, i16* %4, align 2, !dbg !2334
  call void @llvm.dbg.declare(metadata i16* %3, metadata !2335, metadata !DIExpression()), !dbg !2334
  call void @llvm.dbg.value(metadata i16 %1, metadata !2335, metadata !DIExpression()), !dbg !2334
  store i16 %1, i16* %3, align 2, !dbg !2334
  %5 = load i16, i16* %4, align 2, !dbg !2336
  %6 = load i16, i16* %3, align 2, !dbg !2336
  %7 = call { i16, i1 } @llvm.smul.with.overflow.i16(i16 %5, i16 %6), !dbg !2336
  store { i16, i1 } %7, { i16, i1 }* %2, align 2, !dbg !2336
  %8 = load { i16, i1 }, { i16, i1 }* %2, align 2, !dbg !2336
  ret { i16, i1 } %8, !dbg !2336
}

define { i16, i1 } @_CNW7builtinE22umul_with_overflow_i16EssL0L0(i16, i16) !dbg !2337 {
alloca_block:
  %2 = alloca { i16, i1 }, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %4, metadata !2338, metadata !DIExpression()), !dbg !2339
  call void @llvm.dbg.value(metadata i16 %0, metadata !2338, metadata !DIExpression()), !dbg !2339
  store i16 %0, i16* %4, align 2, !dbg !2339
  call void @llvm.dbg.declare(metadata i16* %3, metadata !2340, metadata !DIExpression()), !dbg !2339
  call void @llvm.dbg.value(metadata i16 %1, metadata !2340, metadata !DIExpression()), !dbg !2339
  store i16 %1, i16* %3, align 2, !dbg !2339
  %5 = load i16, i16* %4, align 2, !dbg !2341
  %6 = load i16, i16* %3, align 2, !dbg !2341
  %7 = call { i16, i1 } @llvm.umul.with.overflow.i16(i16 %5, i16 %6), !dbg !2341
  store { i16, i1 } %7, { i16, i1 }* %2, align 2, !dbg !2341
  %8 = load { i16, i1 }, { i16, i1 }* %2, align 2, !dbg !2341
  ret { i16, i1 } %8, !dbg !2341
}

define i32 @_CNW7builtinE8ctlz_i32EiL0(i32) !dbg !2342 {
entry:
  %1 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %1, metadata !2343, metadata !DIExpression()), !dbg !2344
  call void @llvm.dbg.value(metadata i32 %0, metadata !2343, metadata !DIExpression()), !dbg !2344
  store i32 %0, i32* %1, align 4, !dbg !2344
  %2 = load i32, i32* %1, align 4, !dbg !2345
  %cmpz = icmp eq i32 %2, 0, !dbg !2345
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2345

cond.false:                                       ; preds = %entry
  %3 = call i32 @llvm.ctlz.i32(i32 %2, i1 true), !dbg !2345
  br label %cond.end, !dbg !2345

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i32 [ 32, %entry ], [ %3, %cond.false ], !dbg !2345
  ret i32 %ctz, !dbg !2345
}

define i32 @_CNW7builtinE8cttz_i32EiL0(i32) !dbg !2346 {
entry:
  %1 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %1, metadata !2347, metadata !DIExpression()), !dbg !2348
  call void @llvm.dbg.value(metadata i32 %0, metadata !2347, metadata !DIExpression()), !dbg !2348
  store i32 %0, i32* %1, align 4, !dbg !2348
  %2 = load i32, i32* %1, align 4, !dbg !2349
  %cmpz = icmp eq i32 %2, 0, !dbg !2349
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2349

cond.false:                                       ; preds = %entry
  %3 = call i32 @llvm.cttz.i32(i32 %2, i1 true), !dbg !2349
  br label %cond.end, !dbg !2349

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i32 [ 32, %entry ], [ %3, %cond.false ], !dbg !2349
  ret i32 %ctz, !dbg !2349
}

define i32 @_CNW7builtinE9ctpop_i32EiL0(i32) !dbg !2350 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !2351, metadata !DIExpression()), !dbg !2352
  call void @llvm.dbg.value(metadata i32 %0, metadata !2351, metadata !DIExpression()), !dbg !2352
  store i32 %0, i32* %1, align 4, !dbg !2352
  %2 = load i32, i32* %1, align 4, !dbg !2353
  %3 = call i32 @llvm.ctpop.i32(i32 %2), !dbg !2353
  ret i32 %3, !dbg !2353
}

define i32 @_CNW7builtinE14bitreverse_i32EiL0(i32) !dbg !2354 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !2355, metadata !DIExpression()), !dbg !2356
  call void @llvm.dbg.value(metadata i32 %0, metadata !2355, metadata !DIExpression()), !dbg !2356
  store i32 %0, i32* %1, align 4, !dbg !2356
  %2 = load i32, i32* %1, align 4, !dbg !2357
  %3 = call i32 @llvm.bitreverse.i32(i32 %2), !dbg !2357
  ret i32 %3, !dbg !2357
}

declare i32 @_CNW7builtinE7add_i32EiiL0L0(i32, i32)

declare i32 @_CNW7builtinE7sub_i32EiiL0L0(i32, i32)

declare i32 @_CNW7builtinE7mul_i32EiiL0L0(i32, i32)

declare i32 @_CNW7builtinE7div_i32EiiL0L0(i32, i32)

declare i32 @_CNW7builtinE7rem_i32EiiL0L0(i32, i32)

declare i32 @_CNW7builtinE7shl_i32EiiL0L0(i32, i32)

declare i32 @_CNW7builtinE8lshr_i32EiiL0L0(i32, i32)

declare i32 @_CNW7builtinE8ashr_i32EiiL0L0(i32, i32)

declare i32 @_CNW7builtinE7and_i32EiiL0L0(i32, i32)

declare i32 @_CNW7builtinE6or_i32EiiL0L0(i32, i32)

declare i32 @_CNW7builtinE7xor_i32EiiL0L0(i32, i32)

declare i1 @_CNW7builtinE11icmp_eq_i32EiiL0L0(i32, i32)

declare i1 @_CNW7builtinE11icmp_ne_i32EiiL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_ugt_i32EiiL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_uge_i32EiiL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_ult_i32EiiL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_ule_i32EiiL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_sgt_i32EiiL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_sge_i32EiiL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_slt_i32EiiL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_sle_i32EiiL0L0(i32, i32)

define { i32, i1 } @_CNW7builtinE22sadd_with_overflow_i32EiiL0L0(i32, i32) !dbg !2358 {
alloca_block:
  %2 = alloca { i32, i1 }, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !2359, metadata !DIExpression()), !dbg !2360
  call void @llvm.dbg.value(metadata i32 %0, metadata !2359, metadata !DIExpression()), !dbg !2360
  store i32 %0, i32* %4, align 4, !dbg !2360
  call void @llvm.dbg.declare(metadata i32* %3, metadata !2361, metadata !DIExpression()), !dbg !2360
  call void @llvm.dbg.value(metadata i32 %1, metadata !2361, metadata !DIExpression()), !dbg !2360
  store i32 %1, i32* %3, align 4, !dbg !2360
  %5 = load i32, i32* %4, align 4, !dbg !2362
  %6 = load i32, i32* %3, align 4, !dbg !2362
  %7 = call { i32, i1 } @llvm.sadd.with.overflow.i32(i32 %5, i32 %6), !dbg !2362
  store { i32, i1 } %7, { i32, i1 }* %2, align 4, !dbg !2362
  %8 = load { i32, i1 }, { i32, i1 }* %2, align 4, !dbg !2362
  ret { i32, i1 } %8, !dbg !2362
}

define { i32, i1 } @_CNW7builtinE22uadd_with_overflow_i32EiiL0L0(i32, i32) !dbg !2363 {
alloca_block:
  %2 = alloca { i32, i1 }, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !2364, metadata !DIExpression()), !dbg !2365
  call void @llvm.dbg.value(metadata i32 %0, metadata !2364, metadata !DIExpression()), !dbg !2365
  store i32 %0, i32* %4, align 4, !dbg !2365
  call void @llvm.dbg.declare(metadata i32* %3, metadata !2366, metadata !DIExpression()), !dbg !2365
  call void @llvm.dbg.value(metadata i32 %1, metadata !2366, metadata !DIExpression()), !dbg !2365
  store i32 %1, i32* %3, align 4, !dbg !2365
  %5 = load i32, i32* %4, align 4, !dbg !2367
  %6 = load i32, i32* %3, align 4, !dbg !2367
  %7 = call { i32, i1 } @llvm.uadd.with.overflow.i32(i32 %5, i32 %6), !dbg !2367
  store { i32, i1 } %7, { i32, i1 }* %2, align 4, !dbg !2367
  %8 = load { i32, i1 }, { i32, i1 }* %2, align 4, !dbg !2367
  ret { i32, i1 } %8, !dbg !2367
}

define { i32, i1 } @_CNW7builtinE22ssub_with_overflow_i32EiiL0L0(i32, i32) !dbg !2368 {
alloca_block:
  %2 = alloca { i32, i1 }, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !2369, metadata !DIExpression()), !dbg !2370
  call void @llvm.dbg.value(metadata i32 %0, metadata !2369, metadata !DIExpression()), !dbg !2370
  store i32 %0, i32* %4, align 4, !dbg !2370
  call void @llvm.dbg.declare(metadata i32* %3, metadata !2371, metadata !DIExpression()), !dbg !2370
  call void @llvm.dbg.value(metadata i32 %1, metadata !2371, metadata !DIExpression()), !dbg !2370
  store i32 %1, i32* %3, align 4, !dbg !2370
  %5 = load i32, i32* %4, align 4, !dbg !2372
  %6 = load i32, i32* %3, align 4, !dbg !2372
  %7 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 %5, i32 %6), !dbg !2372
  store { i32, i1 } %7, { i32, i1 }* %2, align 4, !dbg !2372
  %8 = load { i32, i1 }, { i32, i1 }* %2, align 4, !dbg !2372
  ret { i32, i1 } %8, !dbg !2372
}

define { i32, i1 } @_CNW7builtinE22usub_with_overflow_i32EiiL0L0(i32, i32) !dbg !2373 {
alloca_block:
  %2 = alloca { i32, i1 }, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !2374, metadata !DIExpression()), !dbg !2375
  call void @llvm.dbg.value(metadata i32 %0, metadata !2374, metadata !DIExpression()), !dbg !2375
  store i32 %0, i32* %4, align 4, !dbg !2375
  call void @llvm.dbg.declare(metadata i32* %3, metadata !2376, metadata !DIExpression()), !dbg !2375
  call void @llvm.dbg.value(metadata i32 %1, metadata !2376, metadata !DIExpression()), !dbg !2375
  store i32 %1, i32* %3, align 4, !dbg !2375
  %5 = load i32, i32* %4, align 4, !dbg !2377
  %6 = load i32, i32* %3, align 4, !dbg !2377
  %7 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 %5, i32 %6), !dbg !2377
  store { i32, i1 } %7, { i32, i1 }* %2, align 4, !dbg !2377
  %8 = load { i32, i1 }, { i32, i1 }* %2, align 4, !dbg !2377
  ret { i32, i1 } %8, !dbg !2377
}

define { i32, i1 } @_CNW7builtinE22smul_with_overflow_i32EiiL0L0(i32, i32) !dbg !2378 {
alloca_block:
  %2 = alloca { i32, i1 }, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !2379, metadata !DIExpression()), !dbg !2380
  call void @llvm.dbg.value(metadata i32 %0, metadata !2379, metadata !DIExpression()), !dbg !2380
  store i32 %0, i32* %4, align 4, !dbg !2380
  call void @llvm.dbg.declare(metadata i32* %3, metadata !2381, metadata !DIExpression()), !dbg !2380
  call void @llvm.dbg.value(metadata i32 %1, metadata !2381, metadata !DIExpression()), !dbg !2380
  store i32 %1, i32* %3, align 4, !dbg !2380
  %5 = load i32, i32* %4, align 4, !dbg !2382
  %6 = load i32, i32* %3, align 4, !dbg !2382
  %7 = call { i32, i1 } @llvm.smul.with.overflow.i32(i32 %5, i32 %6), !dbg !2382
  store { i32, i1 } %7, { i32, i1 }* %2, align 4, !dbg !2382
  %8 = load { i32, i1 }, { i32, i1 }* %2, align 4, !dbg !2382
  ret { i32, i1 } %8, !dbg !2382
}

define { i32, i1 } @_CNW7builtinE22umul_with_overflow_i32EiiL0L0(i32, i32) !dbg !2383 {
alloca_block:
  %2 = alloca { i32, i1 }, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !2384, metadata !DIExpression()), !dbg !2385
  call void @llvm.dbg.value(metadata i32 %0, metadata !2384, metadata !DIExpression()), !dbg !2385
  store i32 %0, i32* %4, align 4, !dbg !2385
  call void @llvm.dbg.declare(metadata i32* %3, metadata !2386, metadata !DIExpression()), !dbg !2385
  call void @llvm.dbg.value(metadata i32 %1, metadata !2386, metadata !DIExpression()), !dbg !2385
  store i32 %1, i32* %3, align 4, !dbg !2385
  %5 = load i32, i32* %4, align 4, !dbg !2387
  %6 = load i32, i32* %3, align 4, !dbg !2387
  %7 = call { i32, i1 } @llvm.umul.with.overflow.i32(i32 %5, i32 %6), !dbg !2387
  store { i32, i1 } %7, { i32, i1 }* %2, align 4, !dbg !2387
  %8 = load { i32, i1 }, { i32, i1 }* %2, align 4, !dbg !2387
  ret { i32, i1 } %8, !dbg !2387
}

define i64 @_CNW7builtinE8ctlz_i64ElL0(i64) !dbg !2388 {
entry:
  %1 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %1, metadata !2389, metadata !DIExpression()), !dbg !2390
  call void @llvm.dbg.value(metadata i64 %0, metadata !2389, metadata !DIExpression()), !dbg !2390
  store i64 %0, i64* %1, align 8, !dbg !2390
  %2 = load i64, i64* %1, align 8, !dbg !2391
  %cmpz = icmp eq i64 %2, 0, !dbg !2391
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2391

cond.false:                                       ; preds = %entry
  %3 = call i64 @llvm.ctlz.i64(i64 %2, i1 true), !dbg !2391
  br label %cond.end, !dbg !2391

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i64 [ 64, %entry ], [ %3, %cond.false ], !dbg !2391
  ret i64 %ctz, !dbg !2391
}

define i64 @_CNW7builtinE8cttz_i64ElL0(i64) !dbg !2392 {
entry:
  %1 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %1, metadata !2393, metadata !DIExpression()), !dbg !2394
  call void @llvm.dbg.value(metadata i64 %0, metadata !2393, metadata !DIExpression()), !dbg !2394
  store i64 %0, i64* %1, align 8, !dbg !2394
  %2 = load i64, i64* %1, align 8, !dbg !2395
  %cmpz = icmp eq i64 %2, 0, !dbg !2395
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2395

cond.false:                                       ; preds = %entry
  %3 = call i64 @llvm.cttz.i64(i64 %2, i1 true), !dbg !2395
  br label %cond.end, !dbg !2395

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i64 [ 64, %entry ], [ %3, %cond.false ], !dbg !2395
  ret i64 %ctz, !dbg !2395
}

define i64 @_CNW7builtinE9ctpop_i64ElL0(i64) !dbg !2396 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !2397, metadata !DIExpression()), !dbg !2398
  call void @llvm.dbg.value(metadata i64 %0, metadata !2397, metadata !DIExpression()), !dbg !2398
  store i64 %0, i64* %1, align 8, !dbg !2398
  %2 = load i64, i64* %1, align 8, !dbg !2399
  %3 = call i64 @llvm.ctpop.i64(i64 %2), !dbg !2399
  ret i64 %3, !dbg !2399
}

define i64 @_CNW7builtinE14bitreverse_i64ElL0(i64) !dbg !2400 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !2401, metadata !DIExpression()), !dbg !2402
  call void @llvm.dbg.value(metadata i64 %0, metadata !2401, metadata !DIExpression()), !dbg !2402
  store i64 %0, i64* %1, align 8, !dbg !2402
  %2 = load i64, i64* %1, align 8, !dbg !2403
  %3 = call i64 @llvm.bitreverse.i64(i64 %2), !dbg !2403
  ret i64 %3, !dbg !2403
}

declare i64 @_CNW7builtinE7add_i64EllL0L0(i64, i64)

declare i64 @_CNW7builtinE7sub_i64EllL0L0(i64, i64)

declare i64 @_CNW7builtinE7mul_i64EllL0L0(i64, i64)

declare i64 @_CNW7builtinE7div_i64EllL0L0(i64, i64)

declare i64 @_CNW7builtinE7rem_i64EllL0L0(i64, i64)

declare i64 @_CNW7builtinE7shl_i64EllL0L0(i64, i64)

declare i64 @_CNW7builtinE8lshr_i64EllL0L0(i64, i64)

declare i64 @_CNW7builtinE8ashr_i64EllL0L0(i64, i64)

declare i64 @_CNW7builtinE7and_i64EllL0L0(i64, i64)

declare i64 @_CNW7builtinE6or_i64EllL0L0(i64, i64)

declare i64 @_CNW7builtinE7xor_i64EllL0L0(i64, i64)

declare i1 @_CNW7builtinE11icmp_eq_i64EllL0L0(i64, i64)

declare i1 @_CNW7builtinE11icmp_ne_i64EllL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_ugt_i64EllL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_uge_i64EllL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_ult_i64EllL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_ule_i64EllL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_sgt_i64EllL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_sge_i64EllL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_slt_i64EllL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_sle_i64EllL0L0(i64, i64)

define { i64, i1 } @_CNW7builtinE22sadd_with_overflow_i64EllL0L0(i64, i64) !dbg !2404 {
alloca_block:
  %2 = alloca { i64, i1 }, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %4, metadata !2405, metadata !DIExpression()), !dbg !2406
  call void @llvm.dbg.value(metadata i64 %0, metadata !2405, metadata !DIExpression()), !dbg !2406
  store i64 %0, i64* %4, align 8, !dbg !2406
  call void @llvm.dbg.declare(metadata i64* %3, metadata !2407, metadata !DIExpression()), !dbg !2406
  call void @llvm.dbg.value(metadata i64 %1, metadata !2407, metadata !DIExpression()), !dbg !2406
  store i64 %1, i64* %3, align 8, !dbg !2406
  %5 = load i64, i64* %4, align 8, !dbg !2408
  %6 = load i64, i64* %3, align 8, !dbg !2408
  %7 = call { i64, i1 } @llvm.sadd.with.overflow.i64(i64 %5, i64 %6), !dbg !2408
  store { i64, i1 } %7, { i64, i1 }* %2, align 8, !dbg !2408
  %8 = load { i64, i1 }, { i64, i1 }* %2, align 8, !dbg !2408
  ret { i64, i1 } %8, !dbg !2408
}

define { i64, i1 } @_CNW7builtinE22uadd_with_overflow_i64EllL0L0(i64, i64) !dbg !2409 {
alloca_block:
  %2 = alloca { i64, i1 }, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %4, metadata !2410, metadata !DIExpression()), !dbg !2411
  call void @llvm.dbg.value(metadata i64 %0, metadata !2410, metadata !DIExpression()), !dbg !2411
  store i64 %0, i64* %4, align 8, !dbg !2411
  call void @llvm.dbg.declare(metadata i64* %3, metadata !2412, metadata !DIExpression()), !dbg !2411
  call void @llvm.dbg.value(metadata i64 %1, metadata !2412, metadata !DIExpression()), !dbg !2411
  store i64 %1, i64* %3, align 8, !dbg !2411
  %5 = load i64, i64* %4, align 8, !dbg !2413
  %6 = load i64, i64* %3, align 8, !dbg !2413
  %7 = call { i64, i1 } @llvm.uadd.with.overflow.i64(i64 %5, i64 %6), !dbg !2413
  store { i64, i1 } %7, { i64, i1 }* %2, align 8, !dbg !2413
  %8 = load { i64, i1 }, { i64, i1 }* %2, align 8, !dbg !2413
  ret { i64, i1 } %8, !dbg !2413
}

define { i64, i1 } @_CNW7builtinE22ssub_with_overflow_i64EllL0L0(i64, i64) !dbg !2414 {
alloca_block:
  %2 = alloca { i64, i1 }, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %4, metadata !2415, metadata !DIExpression()), !dbg !2416
  call void @llvm.dbg.value(metadata i64 %0, metadata !2415, metadata !DIExpression()), !dbg !2416
  store i64 %0, i64* %4, align 8, !dbg !2416
  call void @llvm.dbg.declare(metadata i64* %3, metadata !2417, metadata !DIExpression()), !dbg !2416
  call void @llvm.dbg.value(metadata i64 %1, metadata !2417, metadata !DIExpression()), !dbg !2416
  store i64 %1, i64* %3, align 8, !dbg !2416
  %5 = load i64, i64* %4, align 8, !dbg !2418
  %6 = load i64, i64* %3, align 8, !dbg !2418
  %7 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %5, i64 %6), !dbg !2418
  store { i64, i1 } %7, { i64, i1 }* %2, align 8, !dbg !2418
  %8 = load { i64, i1 }, { i64, i1 }* %2, align 8, !dbg !2418
  ret { i64, i1 } %8, !dbg !2418
}

define { i64, i1 } @_CNW7builtinE22usub_with_overflow_i64EllL0L0(i64, i64) !dbg !2419 {
alloca_block:
  %2 = alloca { i64, i1 }, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %4, metadata !2420, metadata !DIExpression()), !dbg !2421
  call void @llvm.dbg.value(metadata i64 %0, metadata !2420, metadata !DIExpression()), !dbg !2421
  store i64 %0, i64* %4, align 8, !dbg !2421
  call void @llvm.dbg.declare(metadata i64* %3, metadata !2422, metadata !DIExpression()), !dbg !2421
  call void @llvm.dbg.value(metadata i64 %1, metadata !2422, metadata !DIExpression()), !dbg !2421
  store i64 %1, i64* %3, align 8, !dbg !2421
  %5 = load i64, i64* %4, align 8, !dbg !2423
  %6 = load i64, i64* %3, align 8, !dbg !2423
  %7 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %5, i64 %6), !dbg !2423
  store { i64, i1 } %7, { i64, i1 }* %2, align 8, !dbg !2423
  %8 = load { i64, i1 }, { i64, i1 }* %2, align 8, !dbg !2423
  ret { i64, i1 } %8, !dbg !2423
}

define { i64, i1 } @_CNW7builtinE22smul_with_overflow_i64EllL0L0(i64, i64) !dbg !2424 {
alloca_block:
  %2 = alloca { i64, i1 }, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %4, metadata !2425, metadata !DIExpression()), !dbg !2426
  call void @llvm.dbg.value(metadata i64 %0, metadata !2425, metadata !DIExpression()), !dbg !2426
  store i64 %0, i64* %4, align 8, !dbg !2426
  call void @llvm.dbg.declare(metadata i64* %3, metadata !2427, metadata !DIExpression()), !dbg !2426
  call void @llvm.dbg.value(metadata i64 %1, metadata !2427, metadata !DIExpression()), !dbg !2426
  store i64 %1, i64* %3, align 8, !dbg !2426
  %5 = load i64, i64* %4, align 8, !dbg !2428
  %6 = load i64, i64* %3, align 8, !dbg !2428
  %7 = call { i64, i1 } @llvm.smul.with.overflow.i64(i64 %5, i64 %6), !dbg !2428
  store { i64, i1 } %7, { i64, i1 }* %2, align 8, !dbg !2428
  %8 = load { i64, i1 }, { i64, i1 }* %2, align 8, !dbg !2428
  ret { i64, i1 } %8, !dbg !2428
}

define { i64, i1 } @_CNW7builtinE22umul_with_overflow_i64EllL0L0(i64, i64) !dbg !2429 {
alloca_block:
  %2 = alloca { i64, i1 }, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %4, metadata !2430, metadata !DIExpression()), !dbg !2431
  call void @llvm.dbg.value(metadata i64 %0, metadata !2430, metadata !DIExpression()), !dbg !2431
  store i64 %0, i64* %4, align 8, !dbg !2431
  call void @llvm.dbg.declare(metadata i64* %3, metadata !2432, metadata !DIExpression()), !dbg !2431
  call void @llvm.dbg.value(metadata i64 %1, metadata !2432, metadata !DIExpression()), !dbg !2431
  store i64 %1, i64* %3, align 8, !dbg !2431
  %5 = load i64, i64* %4, align 8, !dbg !2433
  %6 = load i64, i64* %3, align 8, !dbg !2433
  %7 = call { i64, i1 } @llvm.umul.with.overflow.i64(i64 %5, i64 %6), !dbg !2433
  store { i64, i1 } %7, { i64, i1 }* %2, align 8, !dbg !2433
  %8 = load { i64, i1 }, { i64, i1 }* %2, align 8, !dbg !2433
  ret { i64, i1 } %8, !dbg !2433
}

define i128 @_CNW7builtinE9ctlz_i128EnL0(i128) !dbg !2434 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !2435, metadata !DIExpression()), !dbg !2436
  call void @llvm.dbg.value(metadata i128 %0, metadata !2435, metadata !DIExpression()), !dbg !2436
  store i128 %0, i128* %1, align 16, !dbg !2436
  %2 = load i128, i128* %1, align 16, !dbg !2437
  %3 = call i128 @llvm.ctlz.i128(i128 %2, i1 false), !dbg !2437
  ret i128 %3, !dbg !2437
}

define i128 @_CNW7builtinE9cttz_i128EnL0(i128) !dbg !2438 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !2439, metadata !DIExpression()), !dbg !2440
  call void @llvm.dbg.value(metadata i128 %0, metadata !2439, metadata !DIExpression()), !dbg !2440
  store i128 %0, i128* %1, align 16, !dbg !2440
  %2 = load i128, i128* %1, align 16, !dbg !2441
  %3 = call i128 @llvm.cttz.i128(i128 %2, i1 false), !dbg !2441
  ret i128 %3, !dbg !2441
}

define i128 @_CNW7builtinE10ctpop_i128EnL0(i128) !dbg !2442 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !2443, metadata !DIExpression()), !dbg !2444
  call void @llvm.dbg.value(metadata i128 %0, metadata !2443, metadata !DIExpression()), !dbg !2444
  store i128 %0, i128* %1, align 16, !dbg !2444
  %2 = load i128, i128* %1, align 16, !dbg !2445
  %3 = call i128 @llvm.ctpop.i128(i128 %2), !dbg !2445
  ret i128 %3, !dbg !2445
}

define i128 @_CNW7builtinE15bitreverse_i128EnL0(i128) !dbg !2446 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !2447, metadata !DIExpression()), !dbg !2448
  call void @llvm.dbg.value(metadata i128 %0, metadata !2447, metadata !DIExpression()), !dbg !2448
  store i128 %0, i128* %1, align 16, !dbg !2448
  %2 = load i128, i128* %1, align 16, !dbg !2449
  %3 = call i128 @llvm.bitreverse.i128(i128 %2), !dbg !2449
  ret i128 %3, !dbg !2449
}

declare i128 @_CNW7builtinE8add_i128EnnL0L0(i128, i128)

declare i128 @_CNW7builtinE8sub_i128EnnL0L0(i128, i128)

declare i128 @_CNW7builtinE8mul_i128EnnL0L0(i128, i128)

declare i128 @_CNW7builtinE8div_i128EnnL0L0(i128, i128)

declare i128 @_CNW7builtinE8rem_i128EnnL0L0(i128, i128)

declare i128 @_CNW7builtinE8shl_i128EnnL0L0(i128, i128)

declare i128 @_CNW7builtinE9lshr_i128EnnL0L0(i128, i128)

declare i128 @_CNW7builtinE9ashr_i128EnnL0L0(i128, i128)

declare i128 @_CNW7builtinE8and_i128EnnL0L0(i128, i128)

declare i128 @_CNW7builtinE7or_i128EnnL0L0(i128, i128)

declare i128 @_CNW7builtinE8xor_i128EnnL0L0(i128, i128)

declare i1 @_CNW7builtinE12icmp_eq_i128EnnL0L0(i128, i128)

declare i1 @_CNW7builtinE12icmp_ne_i128EnnL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_ugt_i128EnnL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_uge_i128EnnL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_ult_i128EnnL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_ule_i128EnnL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_sgt_i128EnnL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_sge_i128EnnL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_slt_i128EnnL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_sle_i128EnnL0L0(i128, i128)

define { i128, i1 } @_CNW7builtinE23sadd_with_overflow_i128EnnL0L0(i128, i128) !dbg !2450 {
alloca_block:
  %2 = alloca { i128, i1 }, align 16
  %3 = alloca i128, align 16
  %4 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %4, metadata !2451, metadata !DIExpression()), !dbg !2452
  call void @llvm.dbg.value(metadata i128 %0, metadata !2451, metadata !DIExpression()), !dbg !2452
  store i128 %0, i128* %4, align 16, !dbg !2452
  call void @llvm.dbg.declare(metadata i128* %3, metadata !2453, metadata !DIExpression()), !dbg !2452
  call void @llvm.dbg.value(metadata i128 %1, metadata !2453, metadata !DIExpression()), !dbg !2452
  store i128 %1, i128* %3, align 16, !dbg !2452
  %5 = load i128, i128* %4, align 16, !dbg !2454
  %6 = load i128, i128* %3, align 16, !dbg !2454
  %7 = call { i128, i1 } @llvm.sadd.with.overflow.i128(i128 %5, i128 %6), !dbg !2454
  store { i128, i1 } %7, { i128, i1 }* %2, align 16, !dbg !2454
  %8 = load { i128, i1 }, { i128, i1 }* %2, align 16, !dbg !2454
  ret { i128, i1 } %8, !dbg !2454
}

define { i128, i1 } @_CNW7builtinE23uadd_with_overflow_i128EnnL0L0(i128, i128) !dbg !2455 {
alloca_block:
  %2 = alloca { i128, i1 }, align 16
  %3 = alloca i128, align 16
  %4 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %4, metadata !2456, metadata !DIExpression()), !dbg !2457
  call void @llvm.dbg.value(metadata i128 %0, metadata !2456, metadata !DIExpression()), !dbg !2457
  store i128 %0, i128* %4, align 16, !dbg !2457
  call void @llvm.dbg.declare(metadata i128* %3, metadata !2458, metadata !DIExpression()), !dbg !2457
  call void @llvm.dbg.value(metadata i128 %1, metadata !2458, metadata !DIExpression()), !dbg !2457
  store i128 %1, i128* %3, align 16, !dbg !2457
  %5 = load i128, i128* %4, align 16, !dbg !2459
  %6 = load i128, i128* %3, align 16, !dbg !2459
  %7 = call { i128, i1 } @llvm.uadd.with.overflow.i128(i128 %5, i128 %6), !dbg !2459
  store { i128, i1 } %7, { i128, i1 }* %2, align 16, !dbg !2459
  %8 = load { i128, i1 }, { i128, i1 }* %2, align 16, !dbg !2459
  ret { i128, i1 } %8, !dbg !2459
}

define { i128, i1 } @_CNW7builtinE23ssub_with_overflow_i128EnnL0L0(i128, i128) !dbg !2460 {
alloca_block:
  %2 = alloca { i128, i1 }, align 16
  %3 = alloca i128, align 16
  %4 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %4, metadata !2461, metadata !DIExpression()), !dbg !2462
  call void @llvm.dbg.value(metadata i128 %0, metadata !2461, metadata !DIExpression()), !dbg !2462
  store i128 %0, i128* %4, align 16, !dbg !2462
  call void @llvm.dbg.declare(metadata i128* %3, metadata !2463, metadata !DIExpression()), !dbg !2462
  call void @llvm.dbg.value(metadata i128 %1, metadata !2463, metadata !DIExpression()), !dbg !2462
  store i128 %1, i128* %3, align 16, !dbg !2462
  %5 = load i128, i128* %4, align 16, !dbg !2464
  %6 = load i128, i128* %3, align 16, !dbg !2464
  %7 = call { i128, i1 } @llvm.ssub.with.overflow.i128(i128 %5, i128 %6), !dbg !2464
  store { i128, i1 } %7, { i128, i1 }* %2, align 16, !dbg !2464
  %8 = load { i128, i1 }, { i128, i1 }* %2, align 16, !dbg !2464
  ret { i128, i1 } %8, !dbg !2464
}

define { i128, i1 } @_CNW7builtinE23usub_with_overflow_i128EnnL0L0(i128, i128) !dbg !2465 {
alloca_block:
  %2 = alloca { i128, i1 }, align 16
  %3 = alloca i128, align 16
  %4 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %4, metadata !2466, metadata !DIExpression()), !dbg !2467
  call void @llvm.dbg.value(metadata i128 %0, metadata !2466, metadata !DIExpression()), !dbg !2467
  store i128 %0, i128* %4, align 16, !dbg !2467
  call void @llvm.dbg.declare(metadata i128* %3, metadata !2468, metadata !DIExpression()), !dbg !2467
  call void @llvm.dbg.value(metadata i128 %1, metadata !2468, metadata !DIExpression()), !dbg !2467
  store i128 %1, i128* %3, align 16, !dbg !2467
  %5 = load i128, i128* %4, align 16, !dbg !2469
  %6 = load i128, i128* %3, align 16, !dbg !2469
  %7 = call { i128, i1 } @llvm.usub.with.overflow.i128(i128 %5, i128 %6), !dbg !2469
  store { i128, i1 } %7, { i128, i1 }* %2, align 16, !dbg !2469
  %8 = load { i128, i1 }, { i128, i1 }* %2, align 16, !dbg !2469
  ret { i128, i1 } %8, !dbg !2469
}

define { i128, i1 } @_CNW7builtinE23smul_with_overflow_i128EnnL0L0(i128, i128) !dbg !2470 {
alloca_block:
  %2 = alloca { i128, i1 }, align 16
  %3 = alloca i128, align 16
  %4 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %4, metadata !2471, metadata !DIExpression()), !dbg !2472
  call void @llvm.dbg.value(metadata i128 %0, metadata !2471, metadata !DIExpression()), !dbg !2472
  store i128 %0, i128* %4, align 16, !dbg !2472
  call void @llvm.dbg.declare(metadata i128* %3, metadata !2473, metadata !DIExpression()), !dbg !2472
  call void @llvm.dbg.value(metadata i128 %1, metadata !2473, metadata !DIExpression()), !dbg !2472
  store i128 %1, i128* %3, align 16, !dbg !2472
  %5 = load i128, i128* %4, align 16, !dbg !2474
  %6 = load i128, i128* %3, align 16, !dbg !2474
  %7 = call { i128, i1 } @llvm.smul.with.overflow.i128(i128 %5, i128 %6), !dbg !2474
  store { i128, i1 } %7, { i128, i1 }* %2, align 16, !dbg !2474
  %8 = load { i128, i1 }, { i128, i1 }* %2, align 16, !dbg !2474
  ret { i128, i1 } %8, !dbg !2474
}

define { i128, i1 } @_CNW7builtinE23umul_with_overflow_i128EnnL0L0(i128, i128) !dbg !2475 {
alloca_block:
  %2 = alloca { i128, i1 }, align 16
  %3 = alloca i128, align 16
  %4 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %4, metadata !2476, metadata !DIExpression()), !dbg !2477
  call void @llvm.dbg.value(metadata i128 %0, metadata !2476, metadata !DIExpression()), !dbg !2477
  store i128 %0, i128* %4, align 16, !dbg !2477
  call void @llvm.dbg.declare(metadata i128* %3, metadata !2478, metadata !DIExpression()), !dbg !2477
  call void @llvm.dbg.value(metadata i128 %1, metadata !2478, metadata !DIExpression()), !dbg !2477
  store i128 %1, i128* %3, align 16, !dbg !2477
  %5 = load i128, i128* %4, align 16, !dbg !2479
  %6 = load i128, i128* %3, align 16, !dbg !2479
  %7 = call { i128, i1 } @llvm.umul.with.overflow.i128(i128 %5, i128 %6), !dbg !2479
  store { i128, i1 } %7, { i128, i1 }* %2, align 16, !dbg !2479
  %8 = load { i128, i1 }, { i128, i1 }* %2, align 16, !dbg !2479
  ret { i128, i1 } %8, !dbg !2479
}

define i8 @_CNW7builtinE7ctlz_u8EhL0(i8) !dbg !2480 {
entry:
  %1 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %1, metadata !2481, metadata !DIExpression()), !dbg !2482
  call void @llvm.dbg.value(metadata i8 %0, metadata !2481, metadata !DIExpression()), !dbg !2482
  store i8 %0, i8* %1, align 1, !dbg !2482
  %2 = load i8, i8* %1, align 1, !dbg !2483
  %cmpz = icmp eq i8 %2, 0, !dbg !2483
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2483

cond.false:                                       ; preds = %entry
  %3 = call i8 @llvm.ctlz.i8(i8 %2, i1 true), !dbg !2483
  br label %cond.end, !dbg !2483

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i8 [ 8, %entry ], [ %3, %cond.false ], !dbg !2483
  ret i8 %ctz, !dbg !2483
}

define i8 @_CNW7builtinE7cttz_u8EhL0(i8) !dbg !2484 {
entry:
  %1 = alloca i8, align 1
  call void @llvm.dbg.declare(metadata i8* %1, metadata !2485, metadata !DIExpression()), !dbg !2486
  call void @llvm.dbg.value(metadata i8 %0, metadata !2485, metadata !DIExpression()), !dbg !2486
  store i8 %0, i8* %1, align 1, !dbg !2486
  %2 = load i8, i8* %1, align 1, !dbg !2487
  %cmpz = icmp eq i8 %2, 0, !dbg !2487
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2487

cond.false:                                       ; preds = %entry
  %3 = call i8 @llvm.cttz.i8(i8 %2, i1 true), !dbg !2487
  br label %cond.end, !dbg !2487

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i8 [ 8, %entry ], [ %3, %cond.false ], !dbg !2487
  ret i8 %ctz, !dbg !2487
}

define i8 @_CNW7builtinE8ctpop_u8EhL0(i8) !dbg !2488 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !2489, metadata !DIExpression()), !dbg !2490
  call void @llvm.dbg.value(metadata i8 %0, metadata !2489, metadata !DIExpression()), !dbg !2490
  store i8 %0, i8* %1, align 1, !dbg !2490
  %2 = load i8, i8* %1, align 1, !dbg !2491
  %3 = call i8 @llvm.ctpop.i8(i8 %2), !dbg !2491
  ret i8 %3, !dbg !2491
}

define i8 @_CNW7builtinE13bitreverse_u8EhL0(i8) !dbg !2492 {
alloca_block:
  %1 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %1, metadata !2493, metadata !DIExpression()), !dbg !2494
  call void @llvm.dbg.value(metadata i8 %0, metadata !2493, metadata !DIExpression()), !dbg !2494
  store i8 %0, i8* %1, align 1, !dbg !2494
  %2 = load i8, i8* %1, align 1, !dbg !2495
  %3 = call i8 @llvm.bitreverse.i8(i8 %2), !dbg !2495
  ret i8 %3, !dbg !2495
}

declare i8 @_CNW7builtinE6add_u8EhhL0L0(i8, i8)

declare i8 @_CNW7builtinE6sub_u8EhhL0L0(i8, i8)

declare i8 @_CNW7builtinE6mul_u8EhhL0L0(i8, i8)

declare i8 @_CNW7builtinE6div_u8EhhL0L0(i8, i8)

declare i8 @_CNW7builtinE6rem_u8EhhL0L0(i8, i8)

declare i8 @_CNW7builtinE6shl_u8EhhL0L0(i8, i8)

declare i8 @_CNW7builtinE7lshr_u8EhhL0L0(i8, i8)

declare i8 @_CNW7builtinE7ashr_u8EhhL0L0(i8, i8)

declare i8 @_CNW7builtinE6and_u8EhhL0L0(i8, i8)

declare i8 @_CNW7builtinE5or_u8EhhL0L0(i8, i8)

declare i8 @_CNW7builtinE6xor_u8EhhL0L0(i8, i8)

declare i1 @_CNW7builtinE10icmp_eq_u8EhhL0L0(i8, i8)

declare i1 @_CNW7builtinE10icmp_ne_u8EhhL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_ugt_u8EhhL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_uge_u8EhhL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_ult_u8EhhL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_ule_u8EhhL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_sgt_u8EhhL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_sge_u8EhhL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_slt_u8EhhL0L0(i8, i8)

declare i1 @_CNW7builtinE11icmp_sle_u8EhhL0L0(i8, i8)

define { i8, i1 } @_CNW7builtinE21sadd_with_overflow_u8EhhL0L0(i8, i8) !dbg !2496 {
alloca_block:
  %2 = alloca { i8, i1 }, align 1
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %4, metadata !2497, metadata !DIExpression()), !dbg !2498
  call void @llvm.dbg.value(metadata i8 %0, metadata !2497, metadata !DIExpression()), !dbg !2498
  store i8 %0, i8* %4, align 1, !dbg !2498
  call void @llvm.dbg.declare(metadata i8* %3, metadata !2499, metadata !DIExpression()), !dbg !2498
  call void @llvm.dbg.value(metadata i8 %1, metadata !2499, metadata !DIExpression()), !dbg !2498
  store i8 %1, i8* %3, align 1, !dbg !2498
  %5 = load i8, i8* %4, align 1, !dbg !2500
  %6 = load i8, i8* %3, align 1, !dbg !2500
  %7 = call { i8, i1 } @llvm.sadd.with.overflow.i8(i8 %5, i8 %6), !dbg !2500
  store { i8, i1 } %7, { i8, i1 }* %2, align 1, !dbg !2500
  %8 = load { i8, i1 }, { i8, i1 }* %2, align 1, !dbg !2500
  ret { i8, i1 } %8, !dbg !2500
}

define { i8, i1 } @_CNW7builtinE21uadd_with_overflow_u8EhhL0L0(i8, i8) !dbg !2501 {
alloca_block:
  %2 = alloca { i8, i1 }, align 1
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %4, metadata !2502, metadata !DIExpression()), !dbg !2503
  call void @llvm.dbg.value(metadata i8 %0, metadata !2502, metadata !DIExpression()), !dbg !2503
  store i8 %0, i8* %4, align 1, !dbg !2503
  call void @llvm.dbg.declare(metadata i8* %3, metadata !2504, metadata !DIExpression()), !dbg !2503
  call void @llvm.dbg.value(metadata i8 %1, metadata !2504, metadata !DIExpression()), !dbg !2503
  store i8 %1, i8* %3, align 1, !dbg !2503
  %5 = load i8, i8* %4, align 1, !dbg !2505
  %6 = load i8, i8* %3, align 1, !dbg !2505
  %7 = call { i8, i1 } @llvm.uadd.with.overflow.i8(i8 %5, i8 %6), !dbg !2505
  store { i8, i1 } %7, { i8, i1 }* %2, align 1, !dbg !2505
  %8 = load { i8, i1 }, { i8, i1 }* %2, align 1, !dbg !2505
  ret { i8, i1 } %8, !dbg !2505
}

define { i8, i1 } @_CNW7builtinE21ssub_with_overflow_u8EhhL0L0(i8, i8) !dbg !2506 {
alloca_block:
  %2 = alloca { i8, i1 }, align 1
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %4, metadata !2507, metadata !DIExpression()), !dbg !2508
  call void @llvm.dbg.value(metadata i8 %0, metadata !2507, metadata !DIExpression()), !dbg !2508
  store i8 %0, i8* %4, align 1, !dbg !2508
  call void @llvm.dbg.declare(metadata i8* %3, metadata !2509, metadata !DIExpression()), !dbg !2508
  call void @llvm.dbg.value(metadata i8 %1, metadata !2509, metadata !DIExpression()), !dbg !2508
  store i8 %1, i8* %3, align 1, !dbg !2508
  %5 = load i8, i8* %4, align 1, !dbg !2510
  %6 = load i8, i8* %3, align 1, !dbg !2510
  %7 = call { i8, i1 } @llvm.ssub.with.overflow.i8(i8 %5, i8 %6), !dbg !2510
  store { i8, i1 } %7, { i8, i1 }* %2, align 1, !dbg !2510
  %8 = load { i8, i1 }, { i8, i1 }* %2, align 1, !dbg !2510
  ret { i8, i1 } %8, !dbg !2510
}

define { i8, i1 } @_CNW7builtinE21usub_with_overflow_u8EhhL0L0(i8, i8) !dbg !2511 {
alloca_block:
  %2 = alloca { i8, i1 }, align 1
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %4, metadata !2512, metadata !DIExpression()), !dbg !2513
  call void @llvm.dbg.value(metadata i8 %0, metadata !2512, metadata !DIExpression()), !dbg !2513
  store i8 %0, i8* %4, align 1, !dbg !2513
  call void @llvm.dbg.declare(metadata i8* %3, metadata !2514, metadata !DIExpression()), !dbg !2513
  call void @llvm.dbg.value(metadata i8 %1, metadata !2514, metadata !DIExpression()), !dbg !2513
  store i8 %1, i8* %3, align 1, !dbg !2513
  %5 = load i8, i8* %4, align 1, !dbg !2515
  %6 = load i8, i8* %3, align 1, !dbg !2515
  %7 = call { i8, i1 } @llvm.usub.with.overflow.i8(i8 %5, i8 %6), !dbg !2515
  store { i8, i1 } %7, { i8, i1 }* %2, align 1, !dbg !2515
  %8 = load { i8, i1 }, { i8, i1 }* %2, align 1, !dbg !2515
  ret { i8, i1 } %8, !dbg !2515
}

define { i8, i1 } @_CNW7builtinE21smul_with_overflow_u8EhhL0L0(i8, i8) !dbg !2516 {
alloca_block:
  %2 = alloca { i8, i1 }, align 1
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %4, metadata !2517, metadata !DIExpression()), !dbg !2518
  call void @llvm.dbg.value(metadata i8 %0, metadata !2517, metadata !DIExpression()), !dbg !2518
  store i8 %0, i8* %4, align 1, !dbg !2518
  call void @llvm.dbg.declare(metadata i8* %3, metadata !2519, metadata !DIExpression()), !dbg !2518
  call void @llvm.dbg.value(metadata i8 %1, metadata !2519, metadata !DIExpression()), !dbg !2518
  store i8 %1, i8* %3, align 1, !dbg !2518
  %5 = load i8, i8* %4, align 1, !dbg !2520
  %6 = load i8, i8* %3, align 1, !dbg !2520
  %7 = call { i8, i1 } @llvm.smul.with.overflow.i8(i8 %5, i8 %6), !dbg !2520
  store { i8, i1 } %7, { i8, i1 }* %2, align 1, !dbg !2520
  %8 = load { i8, i1 }, { i8, i1 }* %2, align 1, !dbg !2520
  ret { i8, i1 } %8, !dbg !2520
}

define { i8, i1 } @_CNW7builtinE21umul_with_overflow_u8EhhL0L0(i8, i8) !dbg !2521 {
alloca_block:
  %2 = alloca { i8, i1 }, align 1
  %3 = alloca i8, align 1
  %4 = alloca i8, align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i8* %4, metadata !2522, metadata !DIExpression()), !dbg !2523
  call void @llvm.dbg.value(metadata i8 %0, metadata !2522, metadata !DIExpression()), !dbg !2523
  store i8 %0, i8* %4, align 1, !dbg !2523
  call void @llvm.dbg.declare(metadata i8* %3, metadata !2524, metadata !DIExpression()), !dbg !2523
  call void @llvm.dbg.value(metadata i8 %1, metadata !2524, metadata !DIExpression()), !dbg !2523
  store i8 %1, i8* %3, align 1, !dbg !2523
  %5 = load i8, i8* %4, align 1, !dbg !2525
  %6 = load i8, i8* %3, align 1, !dbg !2525
  %7 = call { i8, i1 } @llvm.umul.with.overflow.i8(i8 %5, i8 %6), !dbg !2525
  store { i8, i1 } %7, { i8, i1 }* %2, align 1, !dbg !2525
  %8 = load { i8, i1 }, { i8, i1 }* %2, align 1, !dbg !2525
  ret { i8, i1 } %8, !dbg !2525
}

define i16 @_CNW7builtinE8ctlz_u16EtL0(i16) !dbg !2526 {
entry:
  %1 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %1, metadata !2527, metadata !DIExpression()), !dbg !2528
  call void @llvm.dbg.value(metadata i16 %0, metadata !2527, metadata !DIExpression()), !dbg !2528
  store i16 %0, i16* %1, align 2, !dbg !2528
  %2 = load i16, i16* %1, align 2, !dbg !2529
  %cmpz = icmp eq i16 %2, 0, !dbg !2529
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2529

cond.false:                                       ; preds = %entry
  %3 = call i16 @llvm.ctlz.i16(i16 %2, i1 true), !dbg !2529
  br label %cond.end, !dbg !2529

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i16 [ 16, %entry ], [ %3, %cond.false ], !dbg !2529
  ret i16 %ctz, !dbg !2529
}

define i16 @_CNW7builtinE8cttz_u16EtL0(i16) !dbg !2530 {
entry:
  %1 = alloca i16, align 2
  call void @llvm.dbg.declare(metadata i16* %1, metadata !2531, metadata !DIExpression()), !dbg !2532
  call void @llvm.dbg.value(metadata i16 %0, metadata !2531, metadata !DIExpression()), !dbg !2532
  store i16 %0, i16* %1, align 2, !dbg !2532
  %2 = load i16, i16* %1, align 2, !dbg !2533
  %cmpz = icmp eq i16 %2, 0, !dbg !2533
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2533

cond.false:                                       ; preds = %entry
  %3 = call i16 @llvm.cttz.i16(i16 %2, i1 true), !dbg !2533
  br label %cond.end, !dbg !2533

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i16 [ 16, %entry ], [ %3, %cond.false ], !dbg !2533
  ret i16 %ctz, !dbg !2533
}

define i16 @_CNW7builtinE9ctpop_u16EtL0(i16) !dbg !2534 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !2535, metadata !DIExpression()), !dbg !2536
  call void @llvm.dbg.value(metadata i16 %0, metadata !2535, metadata !DIExpression()), !dbg !2536
  store i16 %0, i16* %1, align 2, !dbg !2536
  %2 = load i16, i16* %1, align 2, !dbg !2537
  %3 = call i16 @llvm.ctpop.i16(i16 %2), !dbg !2537
  ret i16 %3, !dbg !2537
}

define i16 @_CNW7builtinE14bitreverse_u16EtL0(i16) !dbg !2538 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !2539, metadata !DIExpression()), !dbg !2540
  call void @llvm.dbg.value(metadata i16 %0, metadata !2539, metadata !DIExpression()), !dbg !2540
  store i16 %0, i16* %1, align 2, !dbg !2540
  %2 = load i16, i16* %1, align 2, !dbg !2541
  %3 = call i16 @llvm.bitreverse.i16(i16 %2), !dbg !2541
  ret i16 %3, !dbg !2541
}

declare i16 @_CNW7builtinE7add_u16EttL0L0(i16, i16)

declare i16 @_CNW7builtinE7sub_u16EttL0L0(i16, i16)

declare i16 @_CNW7builtinE7mul_u16EttL0L0(i16, i16)

declare i16 @_CNW7builtinE7div_u16EttL0L0(i16, i16)

declare i16 @_CNW7builtinE7rem_u16EttL0L0(i16, i16)

declare i16 @_CNW7builtinE7shl_u16EttL0L0(i16, i16)

declare i16 @_CNW7builtinE8lshr_u16EttL0L0(i16, i16)

declare i16 @_CNW7builtinE8ashr_u16EttL0L0(i16, i16)

declare i16 @_CNW7builtinE7and_u16EttL0L0(i16, i16)

declare i16 @_CNW7builtinE6or_u16EttL0L0(i16, i16)

declare i16 @_CNW7builtinE7xor_u16EttL0L0(i16, i16)

declare i1 @_CNW7builtinE11icmp_eq_u16EttL0L0(i16, i16)

declare i1 @_CNW7builtinE11icmp_ne_u16EttL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_ugt_u16EttL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_uge_u16EttL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_ult_u16EttL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_ule_u16EttL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_sgt_u16EttL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_sge_u16EttL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_slt_u16EttL0L0(i16, i16)

declare i1 @_CNW7builtinE12icmp_sle_u16EttL0L0(i16, i16)

define { i16, i1 } @_CNW7builtinE22sadd_with_overflow_u16EttL0L0(i16, i16) !dbg !2542 {
alloca_block:
  %2 = alloca { i16, i1 }, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %4, metadata !2543, metadata !DIExpression()), !dbg !2544
  call void @llvm.dbg.value(metadata i16 %0, metadata !2543, metadata !DIExpression()), !dbg !2544
  store i16 %0, i16* %4, align 2, !dbg !2544
  call void @llvm.dbg.declare(metadata i16* %3, metadata !2545, metadata !DIExpression()), !dbg !2544
  call void @llvm.dbg.value(metadata i16 %1, metadata !2545, metadata !DIExpression()), !dbg !2544
  store i16 %1, i16* %3, align 2, !dbg !2544
  %5 = load i16, i16* %4, align 2, !dbg !2546
  %6 = load i16, i16* %3, align 2, !dbg !2546
  %7 = call { i16, i1 } @llvm.sadd.with.overflow.i16(i16 %5, i16 %6), !dbg !2546
  store { i16, i1 } %7, { i16, i1 }* %2, align 2, !dbg !2546
  %8 = load { i16, i1 }, { i16, i1 }* %2, align 2, !dbg !2546
  ret { i16, i1 } %8, !dbg !2546
}

define { i16, i1 } @_CNW7builtinE22uadd_with_overflow_u16EttL0L0(i16, i16) !dbg !2547 {
alloca_block:
  %2 = alloca { i16, i1 }, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %4, metadata !2548, metadata !DIExpression()), !dbg !2549
  call void @llvm.dbg.value(metadata i16 %0, metadata !2548, metadata !DIExpression()), !dbg !2549
  store i16 %0, i16* %4, align 2, !dbg !2549
  call void @llvm.dbg.declare(metadata i16* %3, metadata !2550, metadata !DIExpression()), !dbg !2549
  call void @llvm.dbg.value(metadata i16 %1, metadata !2550, metadata !DIExpression()), !dbg !2549
  store i16 %1, i16* %3, align 2, !dbg !2549
  %5 = load i16, i16* %4, align 2, !dbg !2551
  %6 = load i16, i16* %3, align 2, !dbg !2551
  %7 = call { i16, i1 } @llvm.uadd.with.overflow.i16(i16 %5, i16 %6), !dbg !2551
  store { i16, i1 } %7, { i16, i1 }* %2, align 2, !dbg !2551
  %8 = load { i16, i1 }, { i16, i1 }* %2, align 2, !dbg !2551
  ret { i16, i1 } %8, !dbg !2551
}

define { i16, i1 } @_CNW7builtinE22ssub_with_overflow_u16EttL0L0(i16, i16) !dbg !2552 {
alloca_block:
  %2 = alloca { i16, i1 }, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %4, metadata !2553, metadata !DIExpression()), !dbg !2554
  call void @llvm.dbg.value(metadata i16 %0, metadata !2553, metadata !DIExpression()), !dbg !2554
  store i16 %0, i16* %4, align 2, !dbg !2554
  call void @llvm.dbg.declare(metadata i16* %3, metadata !2555, metadata !DIExpression()), !dbg !2554
  call void @llvm.dbg.value(metadata i16 %1, metadata !2555, metadata !DIExpression()), !dbg !2554
  store i16 %1, i16* %3, align 2, !dbg !2554
  %5 = load i16, i16* %4, align 2, !dbg !2556
  %6 = load i16, i16* %3, align 2, !dbg !2556
  %7 = call { i16, i1 } @llvm.ssub.with.overflow.i16(i16 %5, i16 %6), !dbg !2556
  store { i16, i1 } %7, { i16, i1 }* %2, align 2, !dbg !2556
  %8 = load { i16, i1 }, { i16, i1 }* %2, align 2, !dbg !2556
  ret { i16, i1 } %8, !dbg !2556
}

define { i16, i1 } @_CNW7builtinE22usub_with_overflow_u16EttL0L0(i16, i16) !dbg !2557 {
alloca_block:
  %2 = alloca { i16, i1 }, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %4, metadata !2558, metadata !DIExpression()), !dbg !2559
  call void @llvm.dbg.value(metadata i16 %0, metadata !2558, metadata !DIExpression()), !dbg !2559
  store i16 %0, i16* %4, align 2, !dbg !2559
  call void @llvm.dbg.declare(metadata i16* %3, metadata !2560, metadata !DIExpression()), !dbg !2559
  call void @llvm.dbg.value(metadata i16 %1, metadata !2560, metadata !DIExpression()), !dbg !2559
  store i16 %1, i16* %3, align 2, !dbg !2559
  %5 = load i16, i16* %4, align 2, !dbg !2561
  %6 = load i16, i16* %3, align 2, !dbg !2561
  %7 = call { i16, i1 } @llvm.usub.with.overflow.i16(i16 %5, i16 %6), !dbg !2561
  store { i16, i1 } %7, { i16, i1 }* %2, align 2, !dbg !2561
  %8 = load { i16, i1 }, { i16, i1 }* %2, align 2, !dbg !2561
  ret { i16, i1 } %8, !dbg !2561
}

define { i16, i1 } @_CNW7builtinE22smul_with_overflow_u16EttL0L0(i16, i16) !dbg !2562 {
alloca_block:
  %2 = alloca { i16, i1 }, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %4, metadata !2563, metadata !DIExpression()), !dbg !2564
  call void @llvm.dbg.value(metadata i16 %0, metadata !2563, metadata !DIExpression()), !dbg !2564
  store i16 %0, i16* %4, align 2, !dbg !2564
  call void @llvm.dbg.declare(metadata i16* %3, metadata !2565, metadata !DIExpression()), !dbg !2564
  call void @llvm.dbg.value(metadata i16 %1, metadata !2565, metadata !DIExpression()), !dbg !2564
  store i16 %1, i16* %3, align 2, !dbg !2564
  %5 = load i16, i16* %4, align 2, !dbg !2566
  %6 = load i16, i16* %3, align 2, !dbg !2566
  %7 = call { i16, i1 } @llvm.smul.with.overflow.i16(i16 %5, i16 %6), !dbg !2566
  store { i16, i1 } %7, { i16, i1 }* %2, align 2, !dbg !2566
  %8 = load { i16, i1 }, { i16, i1 }* %2, align 2, !dbg !2566
  ret { i16, i1 } %8, !dbg !2566
}

define { i16, i1 } @_CNW7builtinE22umul_with_overflow_u16EttL0L0(i16, i16) !dbg !2567 {
alloca_block:
  %2 = alloca { i16, i1 }, align 2
  %3 = alloca i16, align 2
  %4 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %4, metadata !2568, metadata !DIExpression()), !dbg !2569
  call void @llvm.dbg.value(metadata i16 %0, metadata !2568, metadata !DIExpression()), !dbg !2569
  store i16 %0, i16* %4, align 2, !dbg !2569
  call void @llvm.dbg.declare(metadata i16* %3, metadata !2570, metadata !DIExpression()), !dbg !2569
  call void @llvm.dbg.value(metadata i16 %1, metadata !2570, metadata !DIExpression()), !dbg !2569
  store i16 %1, i16* %3, align 2, !dbg !2569
  %5 = load i16, i16* %4, align 2, !dbg !2571
  %6 = load i16, i16* %3, align 2, !dbg !2571
  %7 = call { i16, i1 } @llvm.umul.with.overflow.i16(i16 %5, i16 %6), !dbg !2571
  store { i16, i1 } %7, { i16, i1 }* %2, align 2, !dbg !2571
  %8 = load { i16, i1 }, { i16, i1 }* %2, align 2, !dbg !2571
  ret { i16, i1 } %8, !dbg !2571
}

define i32 @_CNW7builtinE8ctlz_u32EjL0(i32) !dbg !2572 {
entry:
  %1 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %1, metadata !2573, metadata !DIExpression()), !dbg !2574
  call void @llvm.dbg.value(metadata i32 %0, metadata !2573, metadata !DIExpression()), !dbg !2574
  store i32 %0, i32* %1, align 4, !dbg !2574
  %2 = load i32, i32* %1, align 4, !dbg !2575
  %cmpz = icmp eq i32 %2, 0, !dbg !2575
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2575

cond.false:                                       ; preds = %entry
  %3 = call i32 @llvm.ctlz.i32(i32 %2, i1 true), !dbg !2575
  br label %cond.end, !dbg !2575

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i32 [ 32, %entry ], [ %3, %cond.false ], !dbg !2575
  ret i32 %ctz, !dbg !2575
}

define i32 @_CNW7builtinE8cttz_u32EjL0(i32) !dbg !2576 {
entry:
  %1 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %1, metadata !2577, metadata !DIExpression()), !dbg !2578
  call void @llvm.dbg.value(metadata i32 %0, metadata !2577, metadata !DIExpression()), !dbg !2578
  store i32 %0, i32* %1, align 4, !dbg !2578
  %2 = load i32, i32* %1, align 4, !dbg !2579
  %cmpz = icmp eq i32 %2, 0, !dbg !2579
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2579

cond.false:                                       ; preds = %entry
  %3 = call i32 @llvm.cttz.i32(i32 %2, i1 true), !dbg !2579
  br label %cond.end, !dbg !2579

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i32 [ 32, %entry ], [ %3, %cond.false ], !dbg !2579
  ret i32 %ctz, !dbg !2579
}

define i32 @_CNW7builtinE9ctpop_u32EjL0(i32) !dbg !2580 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !2581, metadata !DIExpression()), !dbg !2582
  call void @llvm.dbg.value(metadata i32 %0, metadata !2581, metadata !DIExpression()), !dbg !2582
  store i32 %0, i32* %1, align 4, !dbg !2582
  %2 = load i32, i32* %1, align 4, !dbg !2583
  %3 = call i32 @llvm.ctpop.i32(i32 %2), !dbg !2583
  ret i32 %3, !dbg !2583
}

define i32 @_CNW7builtinE14bitreverse_u32EjL0(i32) !dbg !2584 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !2585, metadata !DIExpression()), !dbg !2586
  call void @llvm.dbg.value(metadata i32 %0, metadata !2585, metadata !DIExpression()), !dbg !2586
  store i32 %0, i32* %1, align 4, !dbg !2586
  %2 = load i32, i32* %1, align 4, !dbg !2587
  %3 = call i32 @llvm.bitreverse.i32(i32 %2), !dbg !2587
  ret i32 %3, !dbg !2587
}

declare i32 @_CNW7builtinE7add_u32EjjL0L0(i32, i32)

declare i32 @_CNW7builtinE7sub_u32EjjL0L0(i32, i32)

declare i32 @_CNW7builtinE7mul_u32EjjL0L0(i32, i32)

declare i32 @_CNW7builtinE7div_u32EjjL0L0(i32, i32)

declare i32 @_CNW7builtinE7rem_u32EjjL0L0(i32, i32)

declare i32 @_CNW7builtinE7shl_u32EjjL0L0(i32, i32)

declare i32 @_CNW7builtinE8lshr_u32EjjL0L0(i32, i32)

declare i32 @_CNW7builtinE8ashr_u32EjjL0L0(i32, i32)

declare i32 @_CNW7builtinE7and_u32EjjL0L0(i32, i32)

declare i32 @_CNW7builtinE6or_u32EjjL0L0(i32, i32)

declare i32 @_CNW7builtinE7xor_u32EjjL0L0(i32, i32)

declare i1 @_CNW7builtinE11icmp_eq_u32EjjL0L0(i32, i32)

declare i1 @_CNW7builtinE11icmp_ne_u32EjjL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_ugt_u32EjjL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_uge_u32EjjL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_ult_u32EjjL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_ule_u32EjjL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_sgt_u32EjjL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_sge_u32EjjL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_slt_u32EjjL0L0(i32, i32)

declare i1 @_CNW7builtinE12icmp_sle_u32EjjL0L0(i32, i32)

define { i32, i1 } @_CNW7builtinE22sadd_with_overflow_u32EjjL0L0(i32, i32) !dbg !2588 {
alloca_block:
  %2 = alloca { i32, i1 }, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !2589, metadata !DIExpression()), !dbg !2590
  call void @llvm.dbg.value(metadata i32 %0, metadata !2589, metadata !DIExpression()), !dbg !2590
  store i32 %0, i32* %4, align 4, !dbg !2590
  call void @llvm.dbg.declare(metadata i32* %3, metadata !2591, metadata !DIExpression()), !dbg !2590
  call void @llvm.dbg.value(metadata i32 %1, metadata !2591, metadata !DIExpression()), !dbg !2590
  store i32 %1, i32* %3, align 4, !dbg !2590
  %5 = load i32, i32* %4, align 4, !dbg !2592
  %6 = load i32, i32* %3, align 4, !dbg !2592
  %7 = call { i32, i1 } @llvm.sadd.with.overflow.i32(i32 %5, i32 %6), !dbg !2592
  store { i32, i1 } %7, { i32, i1 }* %2, align 4, !dbg !2592
  %8 = load { i32, i1 }, { i32, i1 }* %2, align 4, !dbg !2592
  ret { i32, i1 } %8, !dbg !2592
}

define { i32, i1 } @_CNW7builtinE22uadd_with_overflow_u32EjjL0L0(i32, i32) !dbg !2593 {
alloca_block:
  %2 = alloca { i32, i1 }, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !2594, metadata !DIExpression()), !dbg !2595
  call void @llvm.dbg.value(metadata i32 %0, metadata !2594, metadata !DIExpression()), !dbg !2595
  store i32 %0, i32* %4, align 4, !dbg !2595
  call void @llvm.dbg.declare(metadata i32* %3, metadata !2596, metadata !DIExpression()), !dbg !2595
  call void @llvm.dbg.value(metadata i32 %1, metadata !2596, metadata !DIExpression()), !dbg !2595
  store i32 %1, i32* %3, align 4, !dbg !2595
  %5 = load i32, i32* %4, align 4, !dbg !2597
  %6 = load i32, i32* %3, align 4, !dbg !2597
  %7 = call { i32, i1 } @llvm.uadd.with.overflow.i32(i32 %5, i32 %6), !dbg !2597
  store { i32, i1 } %7, { i32, i1 }* %2, align 4, !dbg !2597
  %8 = load { i32, i1 }, { i32, i1 }* %2, align 4, !dbg !2597
  ret { i32, i1 } %8, !dbg !2597
}

define { i32, i1 } @_CNW7builtinE22ssub_with_overflow_u32EjjL0L0(i32, i32) !dbg !2598 {
alloca_block:
  %2 = alloca { i32, i1 }, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !2599, metadata !DIExpression()), !dbg !2600
  call void @llvm.dbg.value(metadata i32 %0, metadata !2599, metadata !DIExpression()), !dbg !2600
  store i32 %0, i32* %4, align 4, !dbg !2600
  call void @llvm.dbg.declare(metadata i32* %3, metadata !2601, metadata !DIExpression()), !dbg !2600
  call void @llvm.dbg.value(metadata i32 %1, metadata !2601, metadata !DIExpression()), !dbg !2600
  store i32 %1, i32* %3, align 4, !dbg !2600
  %5 = load i32, i32* %4, align 4, !dbg !2602
  %6 = load i32, i32* %3, align 4, !dbg !2602
  %7 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 %5, i32 %6), !dbg !2602
  store { i32, i1 } %7, { i32, i1 }* %2, align 4, !dbg !2602
  %8 = load { i32, i1 }, { i32, i1 }* %2, align 4, !dbg !2602
  ret { i32, i1 } %8, !dbg !2602
}

define { i32, i1 } @_CNW7builtinE22usub_with_overflow_u32EjjL0L0(i32, i32) !dbg !2603 {
alloca_block:
  %2 = alloca { i32, i1 }, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !2604, metadata !DIExpression()), !dbg !2605
  call void @llvm.dbg.value(metadata i32 %0, metadata !2604, metadata !DIExpression()), !dbg !2605
  store i32 %0, i32* %4, align 4, !dbg !2605
  call void @llvm.dbg.declare(metadata i32* %3, metadata !2606, metadata !DIExpression()), !dbg !2605
  call void @llvm.dbg.value(metadata i32 %1, metadata !2606, metadata !DIExpression()), !dbg !2605
  store i32 %1, i32* %3, align 4, !dbg !2605
  %5 = load i32, i32* %4, align 4, !dbg !2607
  %6 = load i32, i32* %3, align 4, !dbg !2607
  %7 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 %5, i32 %6), !dbg !2607
  store { i32, i1 } %7, { i32, i1 }* %2, align 4, !dbg !2607
  %8 = load { i32, i1 }, { i32, i1 }* %2, align 4, !dbg !2607
  ret { i32, i1 } %8, !dbg !2607
}

define { i32, i1 } @_CNW7builtinE22smul_with_overflow_u32EjjL0L0(i32, i32) !dbg !2608 {
alloca_block:
  %2 = alloca { i32, i1 }, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !2609, metadata !DIExpression()), !dbg !2610
  call void @llvm.dbg.value(metadata i32 %0, metadata !2609, metadata !DIExpression()), !dbg !2610
  store i32 %0, i32* %4, align 4, !dbg !2610
  call void @llvm.dbg.declare(metadata i32* %3, metadata !2611, metadata !DIExpression()), !dbg !2610
  call void @llvm.dbg.value(metadata i32 %1, metadata !2611, metadata !DIExpression()), !dbg !2610
  store i32 %1, i32* %3, align 4, !dbg !2610
  %5 = load i32, i32* %4, align 4, !dbg !2612
  %6 = load i32, i32* %3, align 4, !dbg !2612
  %7 = call { i32, i1 } @llvm.smul.with.overflow.i32(i32 %5, i32 %6), !dbg !2612
  store { i32, i1 } %7, { i32, i1 }* %2, align 4, !dbg !2612
  %8 = load { i32, i1 }, { i32, i1 }* %2, align 4, !dbg !2612
  ret { i32, i1 } %8, !dbg !2612
}

define { i32, i1 } @_CNW7builtinE22umul_with_overflow_u32EjjL0L0(i32, i32) !dbg !2613 {
alloca_block:
  %2 = alloca { i32, i1 }, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %4, metadata !2614, metadata !DIExpression()), !dbg !2615
  call void @llvm.dbg.value(metadata i32 %0, metadata !2614, metadata !DIExpression()), !dbg !2615
  store i32 %0, i32* %4, align 4, !dbg !2615
  call void @llvm.dbg.declare(metadata i32* %3, metadata !2616, metadata !DIExpression()), !dbg !2615
  call void @llvm.dbg.value(metadata i32 %1, metadata !2616, metadata !DIExpression()), !dbg !2615
  store i32 %1, i32* %3, align 4, !dbg !2615
  %5 = load i32, i32* %4, align 4, !dbg !2617
  %6 = load i32, i32* %3, align 4, !dbg !2617
  %7 = call { i32, i1 } @llvm.umul.with.overflow.i32(i32 %5, i32 %6), !dbg !2617
  store { i32, i1 } %7, { i32, i1 }* %2, align 4, !dbg !2617
  %8 = load { i32, i1 }, { i32, i1 }* %2, align 4, !dbg !2617
  ret { i32, i1 } %8, !dbg !2617
}

define i64 @_CNW7builtinE8ctlz_u64EmL0(i64) !dbg !2618 {
entry:
  %1 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %1, metadata !2619, metadata !DIExpression()), !dbg !2620
  call void @llvm.dbg.value(metadata i64 %0, metadata !2619, metadata !DIExpression()), !dbg !2620
  store i64 %0, i64* %1, align 8, !dbg !2620
  %2 = load i64, i64* %1, align 8, !dbg !2621
  %cmpz = icmp eq i64 %2, 0, !dbg !2621
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2621

cond.false:                                       ; preds = %entry
  %3 = call i64 @llvm.ctlz.i64(i64 %2, i1 true), !dbg !2621
  br label %cond.end, !dbg !2621

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i64 [ 64, %entry ], [ %3, %cond.false ], !dbg !2621
  ret i64 %ctz, !dbg !2621
}

define i64 @_CNW7builtinE8cttz_u64EmL0(i64) !dbg !2622 {
entry:
  %1 = alloca i64, align 8
  call void @llvm.dbg.declare(metadata i64* %1, metadata !2623, metadata !DIExpression()), !dbg !2624
  call void @llvm.dbg.value(metadata i64 %0, metadata !2623, metadata !DIExpression()), !dbg !2624
  store i64 %0, i64* %1, align 8, !dbg !2624
  %2 = load i64, i64* %1, align 8, !dbg !2625
  %cmpz = icmp eq i64 %2, 0, !dbg !2625
  br i1 %cmpz, label %cond.end, label %cond.false, !dbg !2625

cond.false:                                       ; preds = %entry
  %3 = call i64 @llvm.cttz.i64(i64 %2, i1 true), !dbg !2625
  br label %cond.end, !dbg !2625

cond.end:                                         ; preds = %entry, %cond.false
  %ctz = phi i64 [ 64, %entry ], [ %3, %cond.false ], !dbg !2625
  ret i64 %ctz, !dbg !2625
}

define i64 @_CNW7builtinE9ctpop_u64EmL0(i64) !dbg !2626 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !2627, metadata !DIExpression()), !dbg !2628
  call void @llvm.dbg.value(metadata i64 %0, metadata !2627, metadata !DIExpression()), !dbg !2628
  store i64 %0, i64* %1, align 8, !dbg !2628
  %2 = load i64, i64* %1, align 8, !dbg !2629
  %3 = call i64 @llvm.ctpop.i64(i64 %2), !dbg !2629
  ret i64 %3, !dbg !2629
}

define i64 @_CNW7builtinE14bitreverse_u64EmL0(i64) !dbg !2630 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !2631, metadata !DIExpression()), !dbg !2632
  call void @llvm.dbg.value(metadata i64 %0, metadata !2631, metadata !DIExpression()), !dbg !2632
  store i64 %0, i64* %1, align 8, !dbg !2632
  %2 = load i64, i64* %1, align 8, !dbg !2633
  %3 = call i64 @llvm.bitreverse.i64(i64 %2), !dbg !2633
  ret i64 %3, !dbg !2633
}

declare i64 @_CNW7builtinE7add_u64EmmL0L0(i64, i64)

declare i64 @_CNW7builtinE7sub_u64EmmL0L0(i64, i64)

declare i64 @_CNW7builtinE7mul_u64EmmL0L0(i64, i64)

declare i64 @_CNW7builtinE7div_u64EmmL0L0(i64, i64)

declare i64 @_CNW7builtinE7rem_u64EmmL0L0(i64, i64)

declare i64 @_CNW7builtinE7shl_u64EmmL0L0(i64, i64)

declare i64 @_CNW7builtinE8lshr_u64EmmL0L0(i64, i64)

declare i64 @_CNW7builtinE8ashr_u64EmmL0L0(i64, i64)

declare i64 @_CNW7builtinE7and_u64EmmL0L0(i64, i64)

declare i64 @_CNW7builtinE6or_u64EmmL0L0(i64, i64)

declare i64 @_CNW7builtinE7xor_u64EmmL0L0(i64, i64)

declare i1 @_CNW7builtinE11icmp_eq_u64EmmL0L0(i64, i64)

declare i1 @_CNW7builtinE11icmp_ne_u64EmmL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_ugt_u64EmmL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_uge_u64EmmL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_ult_u64EmmL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_ule_u64EmmL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_sgt_u64EmmL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_sge_u64EmmL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_slt_u64EmmL0L0(i64, i64)

declare i1 @_CNW7builtinE12icmp_sle_u64EmmL0L0(i64, i64)

define { i64, i1 } @_CNW7builtinE22sadd_with_overflow_u64EmmL0L0(i64, i64) !dbg !2634 {
alloca_block:
  %2 = alloca { i64, i1 }, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %4, metadata !2635, metadata !DIExpression()), !dbg !2636
  call void @llvm.dbg.value(metadata i64 %0, metadata !2635, metadata !DIExpression()), !dbg !2636
  store i64 %0, i64* %4, align 8, !dbg !2636
  call void @llvm.dbg.declare(metadata i64* %3, metadata !2637, metadata !DIExpression()), !dbg !2636
  call void @llvm.dbg.value(metadata i64 %1, metadata !2637, metadata !DIExpression()), !dbg !2636
  store i64 %1, i64* %3, align 8, !dbg !2636
  %5 = load i64, i64* %4, align 8, !dbg !2638
  %6 = load i64, i64* %3, align 8, !dbg !2638
  %7 = call { i64, i1 } @llvm.sadd.with.overflow.i64(i64 %5, i64 %6), !dbg !2638
  store { i64, i1 } %7, { i64, i1 }* %2, align 8, !dbg !2638
  %8 = load { i64, i1 }, { i64, i1 }* %2, align 8, !dbg !2638
  ret { i64, i1 } %8, !dbg !2638
}

define { i64, i1 } @_CNW7builtinE22uadd_with_overflow_u64EmmL0L0(i64, i64) !dbg !2639 {
alloca_block:
  %2 = alloca { i64, i1 }, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %4, metadata !2640, metadata !DIExpression()), !dbg !2641
  call void @llvm.dbg.value(metadata i64 %0, metadata !2640, metadata !DIExpression()), !dbg !2641
  store i64 %0, i64* %4, align 8, !dbg !2641
  call void @llvm.dbg.declare(metadata i64* %3, metadata !2642, metadata !DIExpression()), !dbg !2641
  call void @llvm.dbg.value(metadata i64 %1, metadata !2642, metadata !DIExpression()), !dbg !2641
  store i64 %1, i64* %3, align 8, !dbg !2641
  %5 = load i64, i64* %4, align 8, !dbg !2643
  %6 = load i64, i64* %3, align 8, !dbg !2643
  %7 = call { i64, i1 } @llvm.uadd.with.overflow.i64(i64 %5, i64 %6), !dbg !2643
  store { i64, i1 } %7, { i64, i1 }* %2, align 8, !dbg !2643
  %8 = load { i64, i1 }, { i64, i1 }* %2, align 8, !dbg !2643
  ret { i64, i1 } %8, !dbg !2643
}

define { i64, i1 } @_CNW7builtinE22ssub_with_overflow_u64EmmL0L0(i64, i64) !dbg !2644 {
alloca_block:
  %2 = alloca { i64, i1 }, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %4, metadata !2645, metadata !DIExpression()), !dbg !2646
  call void @llvm.dbg.value(metadata i64 %0, metadata !2645, metadata !DIExpression()), !dbg !2646
  store i64 %0, i64* %4, align 8, !dbg !2646
  call void @llvm.dbg.declare(metadata i64* %3, metadata !2647, metadata !DIExpression()), !dbg !2646
  call void @llvm.dbg.value(metadata i64 %1, metadata !2647, metadata !DIExpression()), !dbg !2646
  store i64 %1, i64* %3, align 8, !dbg !2646
  %5 = load i64, i64* %4, align 8, !dbg !2648
  %6 = load i64, i64* %3, align 8, !dbg !2648
  %7 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %5, i64 %6), !dbg !2648
  store { i64, i1 } %7, { i64, i1 }* %2, align 8, !dbg !2648
  %8 = load { i64, i1 }, { i64, i1 }* %2, align 8, !dbg !2648
  ret { i64, i1 } %8, !dbg !2648
}

define { i64, i1 } @_CNW7builtinE22usub_with_overflow_u64EmmL0L0(i64, i64) !dbg !2649 {
alloca_block:
  %2 = alloca { i64, i1 }, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %4, metadata !2650, metadata !DIExpression()), !dbg !2651
  call void @llvm.dbg.value(metadata i64 %0, metadata !2650, metadata !DIExpression()), !dbg !2651
  store i64 %0, i64* %4, align 8, !dbg !2651
  call void @llvm.dbg.declare(metadata i64* %3, metadata !2652, metadata !DIExpression()), !dbg !2651
  call void @llvm.dbg.value(metadata i64 %1, metadata !2652, metadata !DIExpression()), !dbg !2651
  store i64 %1, i64* %3, align 8, !dbg !2651
  %5 = load i64, i64* %4, align 8, !dbg !2653
  %6 = load i64, i64* %3, align 8, !dbg !2653
  %7 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %5, i64 %6), !dbg !2653
  store { i64, i1 } %7, { i64, i1 }* %2, align 8, !dbg !2653
  %8 = load { i64, i1 }, { i64, i1 }* %2, align 8, !dbg !2653
  ret { i64, i1 } %8, !dbg !2653
}

define { i64, i1 } @_CNW7builtinE22smul_with_overflow_u64EmmL0L0(i64, i64) !dbg !2654 {
alloca_block:
  %2 = alloca { i64, i1 }, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %4, metadata !2655, metadata !DIExpression()), !dbg !2656
  call void @llvm.dbg.value(metadata i64 %0, metadata !2655, metadata !DIExpression()), !dbg !2656
  store i64 %0, i64* %4, align 8, !dbg !2656
  call void @llvm.dbg.declare(metadata i64* %3, metadata !2657, metadata !DIExpression()), !dbg !2656
  call void @llvm.dbg.value(metadata i64 %1, metadata !2657, metadata !DIExpression()), !dbg !2656
  store i64 %1, i64* %3, align 8, !dbg !2656
  %5 = load i64, i64* %4, align 8, !dbg !2658
  %6 = load i64, i64* %3, align 8, !dbg !2658
  %7 = call { i64, i1 } @llvm.smul.with.overflow.i64(i64 %5, i64 %6), !dbg !2658
  store { i64, i1 } %7, { i64, i1 }* %2, align 8, !dbg !2658
  %8 = load { i64, i1 }, { i64, i1 }* %2, align 8, !dbg !2658
  ret { i64, i1 } %8, !dbg !2658
}

define { i64, i1 } @_CNW7builtinE22umul_with_overflow_u64EmmL0L0(i64, i64) !dbg !2659 {
alloca_block:
  %2 = alloca { i64, i1 }, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %4, metadata !2660, metadata !DIExpression()), !dbg !2661
  call void @llvm.dbg.value(metadata i64 %0, metadata !2660, metadata !DIExpression()), !dbg !2661
  store i64 %0, i64* %4, align 8, !dbg !2661
  call void @llvm.dbg.declare(metadata i64* %3, metadata !2662, metadata !DIExpression()), !dbg !2661
  call void @llvm.dbg.value(metadata i64 %1, metadata !2662, metadata !DIExpression()), !dbg !2661
  store i64 %1, i64* %3, align 8, !dbg !2661
  %5 = load i64, i64* %4, align 8, !dbg !2663
  %6 = load i64, i64* %3, align 8, !dbg !2663
  %7 = call { i64, i1 } @llvm.umul.with.overflow.i64(i64 %5, i64 %6), !dbg !2663
  store { i64, i1 } %7, { i64, i1 }* %2, align 8, !dbg !2663
  %8 = load { i64, i1 }, { i64, i1 }* %2, align 8, !dbg !2663
  ret { i64, i1 } %8, !dbg !2663
}

define i128 @_CNW7builtinE9ctlz_u128EoL0(i128) !dbg !2664 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !2665, metadata !DIExpression()), !dbg !2666
  call void @llvm.dbg.value(metadata i128 %0, metadata !2665, metadata !DIExpression()), !dbg !2666
  store i128 %0, i128* %1, align 16, !dbg !2666
  %2 = load i128, i128* %1, align 16, !dbg !2667
  %3 = call i128 @llvm.ctlz.i128(i128 %2, i1 false), !dbg !2667
  ret i128 %3, !dbg !2667
}

define i128 @_CNW7builtinE9cttz_u128EoL0(i128) !dbg !2668 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !2669, metadata !DIExpression()), !dbg !2670
  call void @llvm.dbg.value(metadata i128 %0, metadata !2669, metadata !DIExpression()), !dbg !2670
  store i128 %0, i128* %1, align 16, !dbg !2670
  %2 = load i128, i128* %1, align 16, !dbg !2671
  %3 = call i128 @llvm.cttz.i128(i128 %2, i1 false), !dbg !2671
  ret i128 %3, !dbg !2671
}

define i128 @_CNW7builtinE10ctpop_u128EoL0(i128) !dbg !2672 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !2673, metadata !DIExpression()), !dbg !2674
  call void @llvm.dbg.value(metadata i128 %0, metadata !2673, metadata !DIExpression()), !dbg !2674
  store i128 %0, i128* %1, align 16, !dbg !2674
  %2 = load i128, i128* %1, align 16, !dbg !2675
  %3 = call i128 @llvm.ctpop.i128(i128 %2), !dbg !2675
  ret i128 %3, !dbg !2675
}

define i128 @_CNW7builtinE15bitreverse_u128EoL0(i128) !dbg !2676 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !2677, metadata !DIExpression()), !dbg !2678
  call void @llvm.dbg.value(metadata i128 %0, metadata !2677, metadata !DIExpression()), !dbg !2678
  store i128 %0, i128* %1, align 16, !dbg !2678
  %2 = load i128, i128* %1, align 16, !dbg !2679
  %3 = call i128 @llvm.bitreverse.i128(i128 %2), !dbg !2679
  ret i128 %3, !dbg !2679
}

declare i128 @_CNW7builtinE8add_u128EooL0L0(i128, i128)

declare i128 @_CNW7builtinE8sub_u128EooL0L0(i128, i128)

declare i128 @_CNW7builtinE8mul_u128EooL0L0(i128, i128)

declare i128 @_CNW7builtinE8div_u128EooL0L0(i128, i128)

declare i128 @_CNW7builtinE8rem_u128EooL0L0(i128, i128)

declare i128 @_CNW7builtinE8shl_u128EooL0L0(i128, i128)

declare i128 @_CNW7builtinE9lshr_u128EooL0L0(i128, i128)

declare i128 @_CNW7builtinE9ashr_u128EooL0L0(i128, i128)

declare i128 @_CNW7builtinE8and_u128EooL0L0(i128, i128)

declare i128 @_CNW7builtinE7or_u128EooL0L0(i128, i128)

declare i128 @_CNW7builtinE8xor_u128EooL0L0(i128, i128)

declare i1 @_CNW7builtinE12icmp_eq_u128EooL0L0(i128, i128)

declare i1 @_CNW7builtinE12icmp_ne_u128EooL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_ugt_u128EooL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_uge_u128EooL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_ult_u128EooL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_ule_u128EooL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_sgt_u128EooL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_sge_u128EooL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_slt_u128EooL0L0(i128, i128)

declare i1 @_CNW7builtinE13icmp_sle_u128EooL0L0(i128, i128)

define { i128, i1 } @_CNW7builtinE23sadd_with_overflow_u128EooL0L0(i128, i128) !dbg !2680 {
alloca_block:
  %2 = alloca { i128, i1 }, align 16
  %3 = alloca i128, align 16
  %4 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %4, metadata !2681, metadata !DIExpression()), !dbg !2682
  call void @llvm.dbg.value(metadata i128 %0, metadata !2681, metadata !DIExpression()), !dbg !2682
  store i128 %0, i128* %4, align 16, !dbg !2682
  call void @llvm.dbg.declare(metadata i128* %3, metadata !2683, metadata !DIExpression()), !dbg !2682
  call void @llvm.dbg.value(metadata i128 %1, metadata !2683, metadata !DIExpression()), !dbg !2682
  store i128 %1, i128* %3, align 16, !dbg !2682
  %5 = load i128, i128* %4, align 16, !dbg !2684
  %6 = load i128, i128* %3, align 16, !dbg !2684
  %7 = call { i128, i1 } @llvm.sadd.with.overflow.i128(i128 %5, i128 %6), !dbg !2684
  store { i128, i1 } %7, { i128, i1 }* %2, align 16, !dbg !2684
  %8 = load { i128, i1 }, { i128, i1 }* %2, align 16, !dbg !2684
  ret { i128, i1 } %8, !dbg !2684
}

define { i128, i1 } @_CNW7builtinE23uadd_with_overflow_u128EooL0L0(i128, i128) !dbg !2685 {
alloca_block:
  %2 = alloca { i128, i1 }, align 16
  %3 = alloca i128, align 16
  %4 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %4, metadata !2686, metadata !DIExpression()), !dbg !2687
  call void @llvm.dbg.value(metadata i128 %0, metadata !2686, metadata !DIExpression()), !dbg !2687
  store i128 %0, i128* %4, align 16, !dbg !2687
  call void @llvm.dbg.declare(metadata i128* %3, metadata !2688, metadata !DIExpression()), !dbg !2687
  call void @llvm.dbg.value(metadata i128 %1, metadata !2688, metadata !DIExpression()), !dbg !2687
  store i128 %1, i128* %3, align 16, !dbg !2687
  %5 = load i128, i128* %4, align 16, !dbg !2689
  %6 = load i128, i128* %3, align 16, !dbg !2689
  %7 = call { i128, i1 } @llvm.uadd.with.overflow.i128(i128 %5, i128 %6), !dbg !2689
  store { i128, i1 } %7, { i128, i1 }* %2, align 16, !dbg !2689
  %8 = load { i128, i1 }, { i128, i1 }* %2, align 16, !dbg !2689
  ret { i128, i1 } %8, !dbg !2689
}

define { i128, i1 } @_CNW7builtinE23ssub_with_overflow_u128EooL0L0(i128, i128) !dbg !2690 {
alloca_block:
  %2 = alloca { i128, i1 }, align 16
  %3 = alloca i128, align 16
  %4 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %4, metadata !2691, metadata !DIExpression()), !dbg !2692
  call void @llvm.dbg.value(metadata i128 %0, metadata !2691, metadata !DIExpression()), !dbg !2692
  store i128 %0, i128* %4, align 16, !dbg !2692
  call void @llvm.dbg.declare(metadata i128* %3, metadata !2693, metadata !DIExpression()), !dbg !2692
  call void @llvm.dbg.value(metadata i128 %1, metadata !2693, metadata !DIExpression()), !dbg !2692
  store i128 %1, i128* %3, align 16, !dbg !2692
  %5 = load i128, i128* %4, align 16, !dbg !2694
  %6 = load i128, i128* %3, align 16, !dbg !2694
  %7 = call { i128, i1 } @llvm.ssub.with.overflow.i128(i128 %5, i128 %6), !dbg !2694
  store { i128, i1 } %7, { i128, i1 }* %2, align 16, !dbg !2694
  %8 = load { i128, i1 }, { i128, i1 }* %2, align 16, !dbg !2694
  ret { i128, i1 } %8, !dbg !2694
}

define { i128, i1 } @_CNW7builtinE23usub_with_overflow_u128EooL0L0(i128, i128) !dbg !2695 {
alloca_block:
  %2 = alloca { i128, i1 }, align 16
  %3 = alloca i128, align 16
  %4 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %4, metadata !2696, metadata !DIExpression()), !dbg !2697
  call void @llvm.dbg.value(metadata i128 %0, metadata !2696, metadata !DIExpression()), !dbg !2697
  store i128 %0, i128* %4, align 16, !dbg !2697
  call void @llvm.dbg.declare(metadata i128* %3, metadata !2698, metadata !DIExpression()), !dbg !2697
  call void @llvm.dbg.value(metadata i128 %1, metadata !2698, metadata !DIExpression()), !dbg !2697
  store i128 %1, i128* %3, align 16, !dbg !2697
  %5 = load i128, i128* %4, align 16, !dbg !2699
  %6 = load i128, i128* %3, align 16, !dbg !2699
  %7 = call { i128, i1 } @llvm.usub.with.overflow.i128(i128 %5, i128 %6), !dbg !2699
  store { i128, i1 } %7, { i128, i1 }* %2, align 16, !dbg !2699
  %8 = load { i128, i1 }, { i128, i1 }* %2, align 16, !dbg !2699
  ret { i128, i1 } %8, !dbg !2699
}

define { i128, i1 } @_CNW7builtinE23smul_with_overflow_u128EooL0L0(i128, i128) !dbg !2700 {
alloca_block:
  %2 = alloca { i128, i1 }, align 16
  %3 = alloca i128, align 16
  %4 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %4, metadata !2701, metadata !DIExpression()), !dbg !2702
  call void @llvm.dbg.value(metadata i128 %0, metadata !2701, metadata !DIExpression()), !dbg !2702
  store i128 %0, i128* %4, align 16, !dbg !2702
  call void @llvm.dbg.declare(metadata i128* %3, metadata !2703, metadata !DIExpression()), !dbg !2702
  call void @llvm.dbg.value(metadata i128 %1, metadata !2703, metadata !DIExpression()), !dbg !2702
  store i128 %1, i128* %3, align 16, !dbg !2702
  %5 = load i128, i128* %4, align 16, !dbg !2704
  %6 = load i128, i128* %3, align 16, !dbg !2704
  %7 = call { i128, i1 } @llvm.smul.with.overflow.i128(i128 %5, i128 %6), !dbg !2704
  store { i128, i1 } %7, { i128, i1 }* %2, align 16, !dbg !2704
  %8 = load { i128, i1 }, { i128, i1 }* %2, align 16, !dbg !2704
  ret { i128, i1 } %8, !dbg !2704
}

define { i128, i1 } @_CNW7builtinE23umul_with_overflow_u128EooL0L0(i128, i128) !dbg !2705 {
alloca_block:
  %2 = alloca { i128, i1 }, align 16
  %3 = alloca i128, align 16
  %4 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %4, metadata !2706, metadata !DIExpression()), !dbg !2707
  call void @llvm.dbg.value(metadata i128 %0, metadata !2706, metadata !DIExpression()), !dbg !2707
  store i128 %0, i128* %4, align 16, !dbg !2707
  call void @llvm.dbg.declare(metadata i128* %3, metadata !2708, metadata !DIExpression()), !dbg !2707
  call void @llvm.dbg.value(metadata i128 %1, metadata !2708, metadata !DIExpression()), !dbg !2707
  store i128 %1, i128* %3, align 16, !dbg !2707
  %5 = load i128, i128* %4, align 16, !dbg !2709
  %6 = load i128, i128* %3, align 16, !dbg !2709
  %7 = call { i128, i1 } @llvm.umul.with.overflow.i128(i128 %5, i128 %6), !dbg !2709
  store { i128, i1 } %7, { i128, i1 }* %2, align 16, !dbg !2709
  %8 = load { i128, i1 }, { i128, i1 }* %2, align 16, !dbg !2709
  ret { i128, i1 } %8, !dbg !2709
}

define i16 @_CNW7builtinE9bswap_i16EsL0(i16) !dbg !2710 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !2711, metadata !DIExpression()), !dbg !2712
  call void @llvm.dbg.value(metadata i16 %0, metadata !2711, metadata !DIExpression()), !dbg !2712
  store i16 %0, i16* %1, align 2, !dbg !2712
  %2 = load i16, i16* %1, align 2, !dbg !2713
  %3 = call i16 @llvm.bswap.i16(i16 %2), !dbg !2713
  ret i16 %3, !dbg !2713
}

define i32 @_CNW7builtinE9bswap_i32EiL0(i32) !dbg !2714 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !2715, metadata !DIExpression()), !dbg !2716
  call void @llvm.dbg.value(metadata i32 %0, metadata !2715, metadata !DIExpression()), !dbg !2716
  store i32 %0, i32* %1, align 4, !dbg !2716
  %2 = load i32, i32* %1, align 4, !dbg !2717
  %3 = call i32 @llvm.bswap.i32(i32 %2), !dbg !2717
  ret i32 %3, !dbg !2717
}

define i64 @_CNW7builtinE9bswap_i64ElL0(i64) !dbg !2718 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !2719, metadata !DIExpression()), !dbg !2720
  call void @llvm.dbg.value(metadata i64 %0, metadata !2719, metadata !DIExpression()), !dbg !2720
  store i64 %0, i64* %1, align 8, !dbg !2720
  %2 = load i64, i64* %1, align 8, !dbg !2721
  %3 = call i64 @llvm.bswap.i64(i64 %2), !dbg !2721
  ret i64 %3, !dbg !2721
}

define i128 @_CNW7builtinE10bswap_i128EnL0(i128) !dbg !2722 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !2723, metadata !DIExpression()), !dbg !2724
  call void @llvm.dbg.value(metadata i128 %0, metadata !2723, metadata !DIExpression()), !dbg !2724
  store i128 %0, i128* %1, align 16, !dbg !2724
  %2 = load i128, i128* %1, align 16, !dbg !2725
  %3 = call i128 @llvm.bswap.i128(i128 %2), !dbg !2725
  ret i128 %3, !dbg !2725
}

define i16 @_CNW7builtinE9bswap_u16EtL0(i16) !dbg !2726 {
alloca_block:
  %1 = alloca i16, align 2
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i16* %1, metadata !2727, metadata !DIExpression()), !dbg !2728
  call void @llvm.dbg.value(metadata i16 %0, metadata !2727, metadata !DIExpression()), !dbg !2728
  store i16 %0, i16* %1, align 2, !dbg !2728
  %2 = load i16, i16* %1, align 2, !dbg !2729
  %3 = call i16 @llvm.bswap.i16(i16 %2), !dbg !2729
  ret i16 %3, !dbg !2729
}

define i32 @_CNW7builtinE9bswap_u32EjL0(i32) !dbg !2730 {
alloca_block:
  %1 = alloca i32, align 4
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i32* %1, metadata !2731, metadata !DIExpression()), !dbg !2732
  call void @llvm.dbg.value(metadata i32 %0, metadata !2731, metadata !DIExpression()), !dbg !2732
  store i32 %0, i32* %1, align 4, !dbg !2732
  %2 = load i32, i32* %1, align 4, !dbg !2733
  %3 = call i32 @llvm.bswap.i32(i32 %2), !dbg !2733
  ret i32 %3, !dbg !2733
}

define i64 @_CNW7builtinE9bswap_u64EmL0(i64) !dbg !2734 {
alloca_block:
  %1 = alloca i64, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i64* %1, metadata !2735, metadata !DIExpression()), !dbg !2736
  call void @llvm.dbg.value(metadata i64 %0, metadata !2735, metadata !DIExpression()), !dbg !2736
  store i64 %0, i64* %1, align 8, !dbg !2736
  %2 = load i64, i64* %1, align 8, !dbg !2737
  %3 = call i64 @llvm.bswap.i64(i64 %2), !dbg !2737
  ret i64 %3, !dbg !2737
}

define i128 @_CNW7builtinE10bswap_u128EoL0(i128) !dbg !2738 {
alloca_block:
  %1 = alloca i128, align 16
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @llvm.dbg.declare(metadata i128* %1, metadata !2739, metadata !DIExpression()), !dbg !2740
  call void @llvm.dbg.value(metadata i128 %0, metadata !2739, metadata !DIExpression()), !dbg !2740
  store i128 %0, i128* %1, align 16, !dbg !2740
  %2 = load i128, i128* %1, align 16, !dbg !2741
  %3 = call i128 @llvm.bswap.i128(i128 %2), !dbg !2741
  ret i128 %3, !dbg !2741
}

declare i64 @_CNW7builtinE14trunc_i128_i64EnL0(i128)

declare i32 @_CNW7builtinE14trunc_i128_i32EnL0(i128)

declare i16 @_CNW7builtinE14trunc_i128_i16EnL0(i128)

declare i8 @_CNW7builtinE13trunc_i128_i8EnL0(i128)

declare i1 @_CNW7builtinE13trunc_i128_i1EnL0(i128)

declare i32 @_CNW7builtinE13trunc_i64_i32ElL0(i64)

declare i16 @_CNW7builtinE13trunc_i64_i16ElL0(i64)

declare i8 @_CNW7builtinE12trunc_i64_i8ElL0(i64)

declare i1 @_CNW7builtinE12trunc_i64_i1ElL0(i64)

declare i16 @_CNW7builtinE13trunc_i32_i16EiL0(i32)

declare i8 @_CNW7builtinE12trunc_i32_i8EiL0(i32)

declare i1 @_CNW7builtinE12trunc_i32_i1EiL0(i32)

declare i8 @_CNW7builtinE12trunc_i16_i8EsL0(i16)

declare i1 @_CNW7builtinE12trunc_i16_i1EsL0(i16)

declare i1 @_CNW7builtinE11trunc_i8_i1EaL0(i8)

declare i64 @_CNW7builtinE14trunc_u128_u64EoL0(i128)

declare i32 @_CNW7builtinE14trunc_u128_u32EoL0(i128)

declare i16 @_CNW7builtinE14trunc_u128_u16EoL0(i128)

declare i8 @_CNW7builtinE13trunc_u128_u8EoL0(i128)

declare i32 @_CNW7builtinE13trunc_u64_u32EmL0(i64)

declare i16 @_CNW7builtinE13trunc_u64_u16EmL0(i64)

declare i8 @_CNW7builtinE12trunc_u64_u8EmL0(i64)

declare i16 @_CNW7builtinE13trunc_u32_u16EjL0(i32)

declare i8 @_CNW7builtinE12trunc_u32_u8EjL0(i32)

declare i8 @_CNW7builtinE12trunc_u16_u8EtL0(i16)

declare i8 @_CNW7builtinE10sext_i1_i8EbL0(i1)

declare i16 @_CNW7builtinE11sext_i1_i16EbL0(i1)

declare i32 @_CNW7builtinE11sext_i1_i32EbL0(i1)

declare i64 @_CNW7builtinE11sext_i1_i64EbL0(i1)

declare i128 @_CNW7builtinE12sext_i1_i128EbL0(i1)

declare i16 @_CNW7builtinE11sext_i8_i16EaL0(i8)

declare i32 @_CNW7builtinE11sext_i8_i32EaL0(i8)

declare i64 @_CNW7builtinE11sext_i8_i64EaL0(i8)

declare i128 @_CNW7builtinE12sext_i8_i128EaL0(i8)

declare i32 @_CNW7builtinE12sext_i16_i32EsL0(i16)

declare i64 @_CNW7builtinE12sext_i16_i64EsL0(i16)

declare i128 @_CNW7builtinE13sext_i16_i128EsL0(i16)

declare i64 @_CNW7builtinE12sext_i32_i64EiL0(i32)

declare i128 @_CNW7builtinE13sext_i32_i128EiL0(i32)

declare i128 @_CNW7builtinE13sext_i64_i128ElL0(i64)

declare i16 @_CNW7builtinE11sext_u8_u16EhL0(i8)

declare i32 @_CNW7builtinE11sext_u8_u32EhL0(i8)

declare i64 @_CNW7builtinE11sext_u8_u64EhL0(i8)

declare i128 @_CNW7builtinE12sext_u8_u128EhL0(i8)

declare i32 @_CNW7builtinE12sext_u16_u32EtL0(i16)

declare i64 @_CNW7builtinE12sext_u16_u64EtL0(i16)

declare i128 @_CNW7builtinE13sext_u16_u128EtL0(i16)

declare i64 @_CNW7builtinE12sext_u32_u64EjL0(i32)

declare i128 @_CNW7builtinE13sext_u32_u128EjL0(i32)

declare i128 @_CNW7builtinE13sext_u64_u128EmL0(i64)

declare i8 @_CNW7builtinE10zext_i1_i8EbL0(i1)

declare i16 @_CNW7builtinE11zext_i1_i16EbL0(i1)

declare i32 @_CNW7builtinE11zext_i1_i32EbL0(i1)

declare i64 @_CNW7builtinE11zext_i1_i64EbL0(i1)

declare i128 @_CNW7builtinE12zext_i1_i128EbL0(i1)

declare i16 @_CNW7builtinE11zext_i8_i16EaL0(i8)

declare i32 @_CNW7builtinE11zext_i8_i32EaL0(i8)

declare i64 @_CNW7builtinE11zext_i8_i64EaL0(i8)

declare i128 @_CNW7builtinE12zext_i8_i128EaL0(i8)

declare i32 @_CNW7builtinE12zext_i16_i32EsL0(i16)

declare i64 @_CNW7builtinE12zext_i16_i64EsL0(i16)

declare i128 @_CNW7builtinE13zext_i16_i128EsL0(i16)

declare i64 @_CNW7builtinE12zext_i32_i64EiL0(i32)

declare i128 @_CNW7builtinE13zext_i32_i128EiL0(i32)

declare i128 @_CNW7builtinE13zext_i64_i128ElL0(i64)

declare i16 @_CNW7builtinE11zext_u8_u16EhL0(i8)

declare i32 @_CNW7builtinE11zext_u8_u32EhL0(i8)

declare i64 @_CNW7builtinE11zext_u8_u64EhL0(i8)

declare i128 @_CNW7builtinE12zext_u8_u128EhL0(i8)

declare i32 @_CNW7builtinE12zext_u16_u32EtL0(i16)

declare i64 @_CNW7builtinE12zext_u16_u64EtL0(i16)

declare i128 @_CNW7builtinE13zext_u16_u128EtL0(i16)

declare i64 @_CNW7builtinE12zext_u32_u64EjL0(i32)

declare i128 @_CNW7builtinE13zext_u32_u128EjL0(i32)

declare i128 @_CNW7builtinE13zext_u64_u128EmL0(i64)

declare i8 @_CNW7builtinE8signcastEaL0(i8)

declare i8 @_CNW7builtinE8signcastEhL0(i8)

declare i16 @_CNW7builtinE8signcastEsL0(i16)

declare i16 @_CNW7builtinE8signcastEtL0(i16)

declare i32 @_CNW7builtinE8signcastEiL0(i32)

declare i32 @_CNW7builtinE8signcastEjL0(i32)

declare i64 @_CNW7builtinE8signcastElL0(i64)

declare i64 @_CNW7builtinE8signcastEmL0(i64)

declare i128 @_CNW7builtinE8signcastEnL0(i128)

declare i128 @_CNW7builtinE8signcastEoL0(i128)

declare float @_CNW7builtinE15fptrunc_f64_f32EdL0(double)

declare double @_CNW7builtinE13fpext_f32_f64EfL0(float)

declare i1 @_CNW7builtinE12fptoi_f32_i1EfL0(float)

declare i8 @_CNW7builtinE12fptoi_f32_i8EfL0(float)

declare i8 @_CNW7builtinE12fptoi_f32_u8EfL0(float)

declare i16 @_CNW7builtinE13fptoi_f32_i16EfL0(float)

declare i16 @_CNW7builtinE13fptoi_f32_u16EfL0(float)

declare i32 @_CNW7builtinE13fptoi_f32_i32EfL0(float)

declare i32 @_CNW7builtinE13fptoi_f32_u32EfL0(float)

declare i64 @_CNW7builtinE13fptoi_f32_i64EfL0(float)

declare i64 @_CNW7builtinE13fptoi_f32_u64EfL0(float)

declare i128 @_CNW7builtinE14fptoi_f32_i128EfL0(float)

declare i128 @_CNW7builtinE14fptoi_f32_u128EfL0(float)

declare i1 @_CNW7builtinE12fptoi_f64_i1EdL0(double)

declare i8 @_CNW7builtinE12fptoi_f64_i8EdL0(double)

declare i8 @_CNW7builtinE12fptoi_f64_u8EdL0(double)

declare i16 @_CNW7builtinE13fptoi_f64_i16EdL0(double)

declare i16 @_CNW7builtinE13fptoi_f64_u16EdL0(double)

declare i32 @_CNW7builtinE13fptoi_f64_i32EdL0(double)

declare i32 @_CNW7builtinE13fptoi_f64_u32EdL0(double)

declare i64 @_CNW7builtinE13fptoi_f64_i64EdL0(double)

declare i64 @_CNW7builtinE13fptoi_f64_u64EdL0(double)

declare i128 @_CNW7builtinE14fptoi_f64_i128EdL0(double)

declare i128 @_CNW7builtinE14fptoi_f64_u128EdL0(double)

declare float @_CNW7builtinE12itofp_i1_f32EbL0(i1)

declare double @_CNW7builtinE12itofp_i1_f64EbL0(i1)

declare float @_CNW7builtinE12itofp_i8_f32EaL0(i8)

declare double @_CNW7builtinE12itofp_i8_f64EaL0(i8)

declare float @_CNW7builtinE12itofp_u8_f32EhL0(i8)

declare double @_CNW7builtinE12itofp_u8_f64EhL0(i8)

declare float @_CNW7builtinE13itofp_i16_f32EsL0(i16)

declare double @_CNW7builtinE13itofp_i16_f64EsL0(i16)

declare float @_CNW7builtinE13itofp_u16_f32EtL0(i16)

declare double @_CNW7builtinE13itofp_u16_f64EtL0(i16)

declare float @_CNW7builtinE13itofp_i32_f32EiL0(i32)

declare double @_CNW7builtinE13itofp_i32_f64EiL0(i32)

declare float @_CNW7builtinE13itofp_u32_f32EjL0(i32)

declare double @_CNW7builtinE13itofp_u32_f64EjL0(i32)

declare float @_CNW7builtinE13itofp_i64_f32ElL0(i64)

declare double @_CNW7builtinE13itofp_i64_f64ElL0(i64)

declare float @_CNW7builtinE13itofp_u64_f32EmL0(i64)

declare double @_CNW7builtinE13itofp_u64_f64EmL0(i64)

declare float @_CNW7builtinE14itofp_i128_f32EnL0(i128)

declare double @_CNW7builtinE14itofp_i128_f64EnL0(i128)

declare float @_CNW7builtinE14itofp_u128_f32EoL0(i128)

declare double @_CNW7builtinE14itofp_u128_f64EoL0(i128)

declare i1 @_CNW7builtinE22ptrtoint_RawPointer_i1EKPvL0(i8*)

declare i8 @_CNW7builtinE22ptrtoint_RawPointer_i8EKPvL0(i8*)

declare i8 @_CNW7builtinE22ptrtoint_RawPointer_u8EKPvL0(i8*)

declare i16 @_CNW7builtinE23ptrtoint_RawPointer_i16EKPvL0(i8*)

declare i16 @_CNW7builtinE23ptrtoint_RawPointer_u16EKPvL0(i8*)

declare i32 @_CNW7builtinE23ptrtoint_RawPointer_i32EKPvL0(i8*)

declare i32 @_CNW7builtinE23ptrtoint_RawPointer_u32EKPvL0(i8*)

declare i64 @_CNW7builtinE23ptrtoint_RawPointer_i64EKPvL0(i8*)

declare i64 @_CNW7builtinE23ptrtoint_RawPointer_u64EKPvL0(i8*)

declare i128 @_CNW7builtinE24ptrtoint_RawPointer_i128EKPvL0(i8*)

declare i128 @_CNW7builtinE24ptrtoint_RawPointer_u128EKPvL0(i8*)

declare i8* @_CNW7builtinE22inttoptr_i1_RawPointerEbL0(i1)

declare i8* @_CNW7builtinE22inttoptr_i8_RawPointerEaL0(i8)

declare i8* @_CNW7builtinE22inttoptr_u8_RawPointerEhL0(i8)

declare i8* @_CNW7builtinE23inttoptr_i16_RawPointerEsL0(i16)

declare i8* @_CNW7builtinE23inttoptr_u16_RawPointerEtL0(i16)

declare i8* @_CNW7builtinE23inttoptr_i32_RawPointerEiL0(i32)

declare i8* @_CNW7builtinE23inttoptr_u32_RawPointerEjL0(i32)

declare i8* @_CNW7builtinE23inttoptr_i64_RawPointerElL0(i64)

declare i8* @_CNW7builtinE23inttoptr_u64_RawPointerEmL0(i64)

declare i8* @_CNW7builtinE24inttoptr_i128_RawPointerEnL0(i128)

declare i8* @_CNW7builtinE24inttoptr_u128_RawPointerEoL0(i128)

declare void @_CNW7builtinE14llvm_intrinsicIvJKPvEEEvS0_S0_L0L0(i8*, i8*)

declare void @_CNW7builtinE14llvm_intrinsicIvJKPvEEEvS0_S0_L0L0.0(i8*, i8*)

declare i1 @_CNW7builtinE14llvm_intrinsicIbJKPvEEEbS0_S0_L0L0(i8*, i8*)

declare float @_CNW7builtinE14llvm_intrinsicIfJfEEEfKPvfL0L0(i8*, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfEEEfKPvfL0L0.0(i8*, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfEEEfKPvfL0L0.1(i8*, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfEEEfKPvfL0L0.2(i8*, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfEEEfKPvfL0L0.3(i8*, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfEEEfKPvfL0L0.4(i8*, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfEEEfKPvfL0L0.5(i8*, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfEEEfKPvfL0L0.6(i8*, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfEEEfKPvfL0L0.7(i8*, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfEEEfKPvfL0L0.8(i8*, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfEEEfKPvfL0L0.9(i8*, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfEEEfKPvfL0L0.10(i8*, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfEEEfKPvfL0L0.11(i8*, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfEEEfKPvfL0L0.12(i8*, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJffEEEfKPvffL0L0L0(i8*, float, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfiEEEfKPvfiL0L0L0(i8*, float, i32)

declare float @_CNW7builtinE14llvm_intrinsicIfJffEEEfKPvffL0L0L0.0(i8*, float, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJffEEEfKPvffL0L0L0.1(i8*, float, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJffEEEfKPvffL0L0L0.2(i8*, float, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJffEEEfKPvffL0L0L0.3(i8*, float, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJffEEEfKPvffL0L0L0.4(i8*, float, float)

declare float @_CNW7builtinE14llvm_intrinsicIfJfffEEEfKPvfffL0L0L0L0(i8*, float, float, float)

declare double @_CNW7builtinE14llvm_intrinsicIdJdEEEdKPvdL0L0(i8*, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdEEEdKPvdL0L0.0(i8*, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdEEEdKPvdL0L0.1(i8*, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdEEEdKPvdL0L0.2(i8*, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdEEEdKPvdL0L0.3(i8*, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdEEEdKPvdL0L0.4(i8*, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdEEEdKPvdL0L0.5(i8*, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdEEEdKPvdL0L0.6(i8*, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdEEEdKPvdL0L0.7(i8*, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdEEEdKPvdL0L0.8(i8*, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdEEEdKPvdL0L0.9(i8*, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdEEEdKPvdL0L0.10(i8*, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdEEEdKPvdL0L0.11(i8*, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdEEEdKPvdL0L0.12(i8*, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJddEEEdKPvddL0L0L0(i8*, double, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdiEEEdKPvdiL0L0L0(i8*, double, i32)

declare double @_CNW7builtinE14llvm_intrinsicIdJddEEEdKPvddL0L0L0.0(i8*, double, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJddEEEdKPvddL0L0L0.1(i8*, double, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJddEEEdKPvddL0L0L0.2(i8*, double, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJddEEEdKPvddL0L0L0.3(i8*, double, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJddEEEdKPvddL0L0L0.4(i8*, double, double)

declare double @_CNW7builtinE14llvm_intrinsicIdJdddEEEdKPvdddL0L0L0L0(i8*, double, double, double)

declare i1 @_CNW7builtinE14llvm_intrinsicIbJbbEEEbKPvbbL0L0L0(i8*, i1, i1)

declare i1 @_CNW7builtinE14llvm_intrinsicIbJbbEEEbKPvbbL0L0L0.0(i8*, i1, i1)

declare i1 @_CNW7builtinE14llvm_intrinsicIbJbEEEbKPvbL0L0(i8*, i1)

declare i1 @_CNW7builtinE14llvm_intrinsicIbJbEEEbKPvbL0L0.0(i8*, i1)

declare { i1, i1 } @_CNW7builtinE14llvm_intrinsicITbbJbbEEES0_KPvbbL0L0L0(i8*, i1, i1)

declare { i1, i1 } @_CNW7builtinE14llvm_intrinsicITbbJbbEEES0_KPvbbL0L0L0.0(i8*, i1, i1)

declare { i1, i1 } @_CNW7builtinE14llvm_intrinsicITbbJbbEEES0_KPvbbL0L0L0.1(i8*, i1, i1)

declare { i1, i1 } @_CNW7builtinE14llvm_intrinsicITbbJbbEEES0_KPvbbL0L0L0.2(i8*, i1, i1)

declare { i1, i1 } @_CNW7builtinE14llvm_intrinsicITbbJbbEEES0_KPvbbL0L0L0.3(i8*, i1, i1)

declare { i1, i1 } @_CNW7builtinE14llvm_intrinsicITbbJbbEEES0_KPvbbL0L0L0.4(i8*, i1, i1)

declare i8 @_CNW7builtinE14llvm_intrinsicIaJabEEEaKPvabL0L0L0(i8*, i8, i1)

declare i8 @_CNW7builtinE14llvm_intrinsicIaJabEEEaKPvabL0L0L0.0(i8*, i8, i1)

declare i8 @_CNW7builtinE14llvm_intrinsicIaJaEEEaKPvaL0L0(i8*, i8)

declare i8 @_CNW7builtinE14llvm_intrinsicIaJaEEEaKPvaL0L0.0(i8*, i8)

declare { i8, i1 } @_CNW7builtinE14llvm_intrinsicITabJaaEEES0_KPvaaL0L0L0(i8*, i8, i8)

declare { i8, i1 } @_CNW7builtinE14llvm_intrinsicITabJaaEEES0_KPvaaL0L0L0.0(i8*, i8, i8)

declare { i8, i1 } @_CNW7builtinE14llvm_intrinsicITabJaaEEES0_KPvaaL0L0L0.1(i8*, i8, i8)

declare { i8, i1 } @_CNW7builtinE14llvm_intrinsicITabJaaEEES0_KPvaaL0L0L0.2(i8*, i8, i8)

declare { i8, i1 } @_CNW7builtinE14llvm_intrinsicITabJaaEEES0_KPvaaL0L0L0.3(i8*, i8, i8)

declare { i8, i1 } @_CNW7builtinE14llvm_intrinsicITabJaaEEES0_KPvaaL0L0L0.4(i8*, i8, i8)

declare i16 @_CNW7builtinE14llvm_intrinsicIsJsbEEEsKPvsbL0L0L0(i8*, i16, i1)

declare i16 @_CNW7builtinE14llvm_intrinsicIsJsbEEEsKPvsbL0L0L0.0(i8*, i16, i1)

declare i16 @_CNW7builtinE14llvm_intrinsicIsJsEEEsKPvsL0L0(i8*, i16)

declare i16 @_CNW7builtinE14llvm_intrinsicIsJsEEEsKPvsL0L0.0(i8*, i16)

declare { i16, i1 } @_CNW7builtinE14llvm_intrinsicITsbJssEEES0_KPvssL0L0L0(i8*, i16, i16)

declare { i16, i1 } @_CNW7builtinE14llvm_intrinsicITsbJssEEES0_KPvssL0L0L0.0(i8*, i16, i16)

declare { i16, i1 } @_CNW7builtinE14llvm_intrinsicITsbJssEEES0_KPvssL0L0L0.1(i8*, i16, i16)

declare { i16, i1 } @_CNW7builtinE14llvm_intrinsicITsbJssEEES0_KPvssL0L0L0.2(i8*, i16, i16)

declare { i16, i1 } @_CNW7builtinE14llvm_intrinsicITsbJssEEES0_KPvssL0L0L0.3(i8*, i16, i16)

declare { i16, i1 } @_CNW7builtinE14llvm_intrinsicITsbJssEEES0_KPvssL0L0L0.4(i8*, i16, i16)

declare i32 @_CNW7builtinE14llvm_intrinsicIiJibEEEiKPvibL0L0L0(i8*, i32, i1)

declare i32 @_CNW7builtinE14llvm_intrinsicIiJibEEEiKPvibL0L0L0.0(i8*, i32, i1)

declare i32 @_CNW7builtinE14llvm_intrinsicIiJiEEEiKPviL0L0(i8*, i32)

declare i32 @_CNW7builtinE14llvm_intrinsicIiJiEEEiKPviL0L0.0(i8*, i32)

declare { i32, i1 } @_CNW7builtinE14llvm_intrinsicITibJiiEEES0_KPviiL0L0L0(i8*, i32, i32)

declare { i32, i1 } @_CNW7builtinE14llvm_intrinsicITibJiiEEES0_KPviiL0L0L0.0(i8*, i32, i32)

declare { i32, i1 } @_CNW7builtinE14llvm_intrinsicITibJiiEEES0_KPviiL0L0L0.1(i8*, i32, i32)

declare { i32, i1 } @_CNW7builtinE14llvm_intrinsicITibJiiEEES0_KPviiL0L0L0.2(i8*, i32, i32)

declare { i32, i1 } @_CNW7builtinE14llvm_intrinsicITibJiiEEES0_KPviiL0L0L0.3(i8*, i32, i32)

declare { i32, i1 } @_CNW7builtinE14llvm_intrinsicITibJiiEEES0_KPviiL0L0L0.4(i8*, i32, i32)

declare i64 @_CNW7builtinE14llvm_intrinsicIlJlbEEElKPvlbL0L0L0(i8*, i64, i1)

declare i64 @_CNW7builtinE14llvm_intrinsicIlJlbEEElKPvlbL0L0L0.0(i8*, i64, i1)

declare i64 @_CNW7builtinE14llvm_intrinsicIlJlEEElKPvlL0L0(i8*, i64)

declare i64 @_CNW7builtinE14llvm_intrinsicIlJlEEElKPvlL0L0.0(i8*, i64)

declare { i64, i1 } @_CNW7builtinE14llvm_intrinsicITlbJllEEES0_KPvllL0L0L0(i8*, i64, i64)

declare { i64, i1 } @_CNW7builtinE14llvm_intrinsicITlbJllEEES0_KPvllL0L0L0.0(i8*, i64, i64)

declare { i64, i1 } @_CNW7builtinE14llvm_intrinsicITlbJllEEES0_KPvllL0L0L0.1(i8*, i64, i64)

declare { i64, i1 } @_CNW7builtinE14llvm_intrinsicITlbJllEEES0_KPvllL0L0L0.2(i8*, i64, i64)

declare { i64, i1 } @_CNW7builtinE14llvm_intrinsicITlbJllEEES0_KPvllL0L0L0.3(i8*, i64, i64)

declare { i64, i1 } @_CNW7builtinE14llvm_intrinsicITlbJllEEES0_KPvllL0L0L0.4(i8*, i64, i64)

declare i128 @_CNW7builtinE14llvm_intrinsicInJnbEEEnKPvnbL0L0L0(i8*, i128, i1)

declare i128 @_CNW7builtinE14llvm_intrinsicInJnbEEEnKPvnbL0L0L0.0(i8*, i128, i1)

declare i128 @_CNW7builtinE14llvm_intrinsicInJnEEEnKPvnL0L0(i8*, i128)

declare i128 @_CNW7builtinE14llvm_intrinsicInJnEEEnKPvnL0L0.0(i8*, i128)

declare { i128, i1 } @_CNW7builtinE14llvm_intrinsicITnbJnnEEES0_KPvnnL0L0L0(i8*, i128, i128)

declare { i128, i1 } @_CNW7builtinE14llvm_intrinsicITnbJnnEEES0_KPvnnL0L0L0.0(i8*, i128, i128)

declare { i128, i1 } @_CNW7builtinE14llvm_intrinsicITnbJnnEEES0_KPvnnL0L0L0.1(i8*, i128, i128)

declare { i128, i1 } @_CNW7builtinE14llvm_intrinsicITnbJnnEEES0_KPvnnL0L0L0.2(i8*, i128, i128)

declare { i128, i1 } @_CNW7builtinE14llvm_intrinsicITnbJnnEEES0_KPvnnL0L0L0.3(i8*, i128, i128)

declare { i128, i1 } @_CNW7builtinE14llvm_intrinsicITnbJnnEEES0_KPvnnL0L0L0.4(i8*, i128, i128)

declare i8 @_CNW7builtinE14llvm_intrinsicIhJhbEEEhKPvhbL0L0L0(i8*, i8, i1)

declare i8 @_CNW7builtinE14llvm_intrinsicIhJhbEEEhKPvhbL0L0L0.0(i8*, i8, i1)

declare i8 @_CNW7builtinE14llvm_intrinsicIhJhEEEhKPvhL0L0(i8*, i8)

declare i8 @_CNW7builtinE14llvm_intrinsicIhJhEEEhKPvhL0L0.0(i8*, i8)

declare { i8, i1 } @_CNW7builtinE14llvm_intrinsicIThbJhhEEES0_KPvhhL0L0L0(i8*, i8, i8)

declare { i8, i1 } @_CNW7builtinE14llvm_intrinsicIThbJhhEEES0_KPvhhL0L0L0.0(i8*, i8, i8)

declare { i8, i1 } @_CNW7builtinE14llvm_intrinsicIThbJhhEEES0_KPvhhL0L0L0.1(i8*, i8, i8)

declare { i8, i1 } @_CNW7builtinE14llvm_intrinsicIThbJhhEEES0_KPvhhL0L0L0.2(i8*, i8, i8)

declare { i8, i1 } @_CNW7builtinE14llvm_intrinsicIThbJhhEEES0_KPvhhL0L0L0.3(i8*, i8, i8)

declare { i8, i1 } @_CNW7builtinE14llvm_intrinsicIThbJhhEEES0_KPvhhL0L0L0.4(i8*, i8, i8)

declare i16 @_CNW7builtinE14llvm_intrinsicItJtbEEEtKPvtbL0L0L0(i8*, i16, i1)

declare i16 @_CNW7builtinE14llvm_intrinsicItJtbEEEtKPvtbL0L0L0.0(i8*, i16, i1)

declare i16 @_CNW7builtinE14llvm_intrinsicItJtEEEtKPvtL0L0(i8*, i16)

declare i16 @_CNW7builtinE14llvm_intrinsicItJtEEEtKPvtL0L0.0(i8*, i16)

declare { i16, i1 } @_CNW7builtinE14llvm_intrinsicITtbJttEEES0_KPvttL0L0L0(i8*, i16, i16)

declare { i16, i1 } @_CNW7builtinE14llvm_intrinsicITtbJttEEES0_KPvttL0L0L0.0(i8*, i16, i16)

declare { i16, i1 } @_CNW7builtinE14llvm_intrinsicITtbJttEEES0_KPvttL0L0L0.1(i8*, i16, i16)

declare { i16, i1 } @_CNW7builtinE14llvm_intrinsicITtbJttEEES0_KPvttL0L0L0.2(i8*, i16, i16)

declare { i16, i1 } @_CNW7builtinE14llvm_intrinsicITtbJttEEES0_KPvttL0L0L0.3(i8*, i16, i16)

declare { i16, i1 } @_CNW7builtinE14llvm_intrinsicITtbJttEEES0_KPvttL0L0L0.4(i8*, i16, i16)

declare i32 @_CNW7builtinE14llvm_intrinsicIjJjbEEEjKPvjbL0L0L0(i8*, i32, i1)

declare i32 @_CNW7builtinE14llvm_intrinsicIjJjbEEEjKPvjbL0L0L0.0(i8*, i32, i1)

declare i32 @_CNW7builtinE14llvm_intrinsicIjJjEEEjKPvjL0L0(i8*, i32)

declare i32 @_CNW7builtinE14llvm_intrinsicIjJjEEEjKPvjL0L0.0(i8*, i32)

declare { i32, i1 } @_CNW7builtinE14llvm_intrinsicITjbJjjEEES0_KPvjjL0L0L0(i8*, i32, i32)

declare { i32, i1 } @_CNW7builtinE14llvm_intrinsicITjbJjjEEES0_KPvjjL0L0L0.0(i8*, i32, i32)

declare { i32, i1 } @_CNW7builtinE14llvm_intrinsicITjbJjjEEES0_KPvjjL0L0L0.1(i8*, i32, i32)

declare { i32, i1 } @_CNW7builtinE14llvm_intrinsicITjbJjjEEES0_KPvjjL0L0L0.2(i8*, i32, i32)

declare { i32, i1 } @_CNW7builtinE14llvm_intrinsicITjbJjjEEES0_KPvjjL0L0L0.3(i8*, i32, i32)

declare { i32, i1 } @_CNW7builtinE14llvm_intrinsicITjbJjjEEES0_KPvjjL0L0L0.4(i8*, i32, i32)

declare i64 @_CNW7builtinE14llvm_intrinsicImJmbEEEmKPvmbL0L0L0(i8*, i64, i1)

declare i64 @_CNW7builtinE14llvm_intrinsicImJmbEEEmKPvmbL0L0L0.0(i8*, i64, i1)

declare i64 @_CNW7builtinE14llvm_intrinsicImJmEEEmKPvmL0L0(i8*, i64)

declare i64 @_CNW7builtinE14llvm_intrinsicImJmEEEmKPvmL0L0.0(i8*, i64)

declare { i64, i1 } @_CNW7builtinE14llvm_intrinsicITmbJmmEEES0_KPvmmL0L0L0(i8*, i64, i64)

declare { i64, i1 } @_CNW7builtinE14llvm_intrinsicITmbJmmEEES0_KPvmmL0L0L0.0(i8*, i64, i64)

declare { i64, i1 } @_CNW7builtinE14llvm_intrinsicITmbJmmEEES0_KPvmmL0L0L0.1(i8*, i64, i64)

declare { i64, i1 } @_CNW7builtinE14llvm_intrinsicITmbJmmEEES0_KPvmmL0L0L0.2(i8*, i64, i64)

declare { i64, i1 } @_CNW7builtinE14llvm_intrinsicITmbJmmEEES0_KPvmmL0L0L0.3(i8*, i64, i64)

declare { i64, i1 } @_CNW7builtinE14llvm_intrinsicITmbJmmEEES0_KPvmmL0L0L0.4(i8*, i64, i64)

declare i128 @_CNW7builtinE14llvm_intrinsicIoJobEEEoKPvobL0L0L0(i8*, i128, i1)

declare i128 @_CNW7builtinE14llvm_intrinsicIoJobEEEoKPvobL0L0L0.0(i8*, i128, i1)

declare i128 @_CNW7builtinE14llvm_intrinsicIoJoEEEoKPvoL0L0(i8*, i128)

declare i128 @_CNW7builtinE14llvm_intrinsicIoJoEEEoKPvoL0L0.0(i8*, i128)

declare { i128, i1 } @_CNW7builtinE14llvm_intrinsicITobJooEEES0_KPvooL0L0L0(i8*, i128, i128)

declare { i128, i1 } @_CNW7builtinE14llvm_intrinsicITobJooEEES0_KPvooL0L0L0.0(i8*, i128, i128)

declare { i128, i1 } @_CNW7builtinE14llvm_intrinsicITobJooEEES0_KPvooL0L0L0.1(i8*, i128, i128)

declare { i128, i1 } @_CNW7builtinE14llvm_intrinsicITobJooEEES0_KPvooL0L0L0.2(i8*, i128, i128)

declare { i128, i1 } @_CNW7builtinE14llvm_intrinsicITobJooEEES0_KPvooL0L0L0.3(i8*, i128, i128)

declare { i128, i1 } @_CNW7builtinE14llvm_intrinsicITobJooEEES0_KPvooL0L0L0.4(i8*, i128, i128)

declare i16 @_CNW7builtinE14llvm_intrinsicIsJsEEEsKPvsL0L0.1(i8*, i16)

declare i32 @_CNW7builtinE14llvm_intrinsicIiJiEEEiKPviL0L0.1(i8*, i32)

declare i64 @_CNW7builtinE14llvm_intrinsicIlJlEEElKPvlL0L0.1(i8*, i64)

declare i128 @_CNW7builtinE14llvm_intrinsicInJnEEEnKPvnL0L0.1(i8*, i128)

declare i16 @_CNW7builtinE14llvm_intrinsicItJtEEEtKPvtL0L0.1(i8*, i16)

declare i32 @_CNW7builtinE14llvm_intrinsicIjJjEEEjKPvjL0L0.1(i8*, i32)

declare i64 @_CNW7builtinE14llvm_intrinsicImJmEEEmKPvmL0L0.1(i8*, i64)

declare i128 @_CNW7builtinE14llvm_intrinsicIoJoEEEoKPvoL0L0.1(i8*, i128)

define %"struct.builtin.<private name>.Bool" @_CNW7builtinE4Bool4copyES0_L0(%"struct.builtin.<private name>.Bool") !dbg !2742 {
alloca_block:
  %1 = alloca %"struct.builtin.<private name>.Bool", align 1
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.builtin.<private name>.Bool", %"struct.builtin.<private name>.Bool"* %1, align 1, !dbg !2743
  %3 = extractvalue %"struct.builtin.<private name>.Bool" %2, 0, !dbg !2743
  %4 = alloca i1, !dbg !2743
  store i1 %3, i1* %4, !dbg !2743
  %5 = extractvalue %"struct.builtin.<private name>.Bool" %0, 0, !dbg !2743
  %6 = alloca i1, !dbg !2743
  store i1 %5, i1* %6, !dbg !2743
  %7 = load i1, i1* %6, !dbg !2743
  store i1 %7, i1* %4, !dbg !2743
  ret %"struct.builtin.<private name>.Bool" %2, !dbg !2743
}

define void @_CNW7builtinE4Bool14__default_initES0_L0(%"struct.builtin.<private name>.Bool") !dbg !2744 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !2745
}

define void @_CNW7builtinE4BoolC1EbL5value(%"struct.builtin.<private name>.Bool"*, i1) !dbg !2746 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.builtin.<private name>.Bool", %"struct.builtin.<private name>.Bool"* %0, align 1, !dbg !2749
  call void @_CNW7builtinE4Bool14__default_initES0_L0(%"struct.builtin.<private name>.Bool" %2), !dbg !2749
  %3 = extractvalue %"struct.builtin.<private name>.Bool" %2, 0, !dbg !2749
  %4 = alloca i1, !dbg !2749
  store i1 %3, i1* %4, !dbg !2749
  store i1 %1, i1* %4, !dbg !2749
  ret void, !dbg !2749
}

define void @_CNW7builtinE4BoolC2ERS0_bL0L5value(%"struct.builtin.<private name>.Bool"*, i1) !dbg !2750 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.builtin.<private name>.Bool", %"struct.builtin.<private name>.Bool"* %0, align 1, !dbg !2751
  %3 = extractvalue %"struct.builtin.<private name>.Bool" %2, 0, !dbg !2751
  %4 = alloca i1, !dbg !2751
  store i1 %3, i1* %4, !dbg !2751
  store i1 %1, i1* %4, !dbg !2751
  ret void, !dbg !2751
}

define void @_CNW7builtinE4BoolD3Ev(%"struct.builtin.<private name>.Bool"*) !dbg !2752 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.builtin.<private name>.Bool", %"struct.builtin.<private name>.Bool"* %0, align 1, !dbg !2753
  %2 = extractvalue %"struct.builtin.<private name>.Bool" %1, 0, !dbg !2753
  %3 = alloca i1, !dbg !2753
  store i1 %2, i1* %3, !dbg !2753
  %4 = bitcast i1* %3 to i8*, !dbg !2753
  call void @llvm.lifetime.end.p0i8(i64 1, i8* %4), !dbg !2753
  ret void, !dbg !2753
}

define void @_CNW7builtinE17ValueWitnessTableD0Ev(%"struct.builtin.<private name>.ValueWitnessTable"*) !dbg !2754 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW7builtinE17ValueWitnessTableD3Ev(%"struct.builtin.<private name>.ValueWitnessTable"* %0), !dbg !2764
  ret void, !dbg !2764
}

define void @_CNW7builtinE17ValueWitnessTable14__default_initES0_L0(%"struct.builtin.<private name>.ValueWitnessTable") !dbg !2765 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !2768
}

define void @_CNW7builtinE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit(%"struct.builtin.<private name>.ValueWitnessTable"*, void (i8*, i8*)*, void (i8*)*) !dbg !2769 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.builtin.<private name>.ValueWitnessTable", %"struct.builtin.<private name>.ValueWitnessTable"* %0, align 8, !dbg !2772
  call void @_CNW7builtinE17ValueWitnessTable14__default_initES0_L0(%"struct.builtin.<private name>.ValueWitnessTable" %3), !dbg !2772
  %4 = extractvalue %"struct.builtin.<private name>.ValueWitnessTable" %3, 0, !dbg !2772
  %5 = alloca void (i8*, i8*)*, !dbg !2772
  store void (i8*, i8*)* %4, void (i8*, i8*)** %5, !dbg !2772
  store void (i8*, i8*)* %1, void (i8*, i8*)** %5, !dbg !2772
  %6 = extractvalue %"struct.builtin.<private name>.ValueWitnessTable" %3, 1, !dbg !2772
  %7 = alloca void (i8*)*, !dbg !2772
  store void (i8*)* %6, void (i8*)** %7, !dbg !2772
  store void (i8*)* %2, void (i8*)** %7, !dbg !2772
  ret void, !dbg !2772
}

define void @_CNW7builtinE17ValueWitnessTableC2ERS0_DoFvPvKPvEDoFvS2_EL0L6copyFnL6deinit(%"struct.builtin.<private name>.ValueWitnessTable"*, void (i8*, i8*)*, void (i8*)*) !dbg !2773 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.builtin.<private name>.ValueWitnessTable", %"struct.builtin.<private name>.ValueWitnessTable"* %0, align 8, !dbg !2774
  %4 = extractvalue %"struct.builtin.<private name>.ValueWitnessTable" %3, 0, !dbg !2774
  %5 = alloca void (i8*, i8*)*, !dbg !2774
  store void (i8*, i8*)* %4, void (i8*, i8*)** %5, !dbg !2774
  store void (i8*, i8*)* %1, void (i8*, i8*)** %5, !dbg !2774
  %6 = extractvalue %"struct.builtin.<private name>.ValueWitnessTable" %3, 1, !dbg !2774
  %7 = alloca void (i8*)*, !dbg !2774
  store void (i8*)* %6, void (i8*)** %7, !dbg !2774
  store void (i8*)* %2, void (i8*)** %7, !dbg !2774
  ret void, !dbg !2774
}

define void @_CNW7builtinE17ValueWitnessTableD3Ev(%"struct.builtin.<private name>.ValueWitnessTable"*) !dbg !2775 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.builtin.<private name>.ValueWitnessTable", %"struct.builtin.<private name>.ValueWitnessTable"* %0, align 8, !dbg !2776
  %2 = extractvalue %"struct.builtin.<private name>.ValueWitnessTable" %1, 0, !dbg !2776
  %3 = alloca void (i8*, i8*)*, !dbg !2776
  store void (i8*, i8*)* %2, void (i8*, i8*)** %3, !dbg !2776
  %4 = extractvalue %"struct.builtin.<private name>.ValueWitnessTable" %1, 1, !dbg !2776
  %5 = alloca void (i8*)*, !dbg !2776
  store void (i8*)* %4, void (i8*)** %5, !dbg !2776
  %6 = bitcast void (i8*)** %5 to i8*, !dbg !2776
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %6), !dbg !2776
  %7 = bitcast void (i8*, i8*)** %3 to i8*, !dbg !2776
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %7), !dbg !2776
  ret void, !dbg !2776
}

define void @_CNW7builtinE19ProtocolConformanceD0Ev(%"struct.builtin.<private name>.ProtocolConformance"*) !dbg !2777 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW7builtinE19ProtocolConformanceD3Ev(%"struct.builtin.<private name>.ProtocolConformance"* %0), !dbg !2801
  ret void, !dbg !2801
}

define void @_CNW7builtinE19ProtocolConformance14__default_initES0_L0(%"struct.builtin.<private name>.ProtocolConformance") !dbg !2802 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !2805
}

define void @_CNW7builtinE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable(%"struct.builtin.<private name>.ProtocolConformance"*, %"struct.builtin.<private name>.TypeInfo"*, i8**) !dbg !2806 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.builtin.<private name>.ProtocolConformance", %"struct.builtin.<private name>.ProtocolConformance"* %0, align 8, !dbg !2809
  call void @_CNW7builtinE19ProtocolConformance14__default_initES0_L0(%"struct.builtin.<private name>.ProtocolConformance" %3), !dbg !2809
  %4 = extractvalue %"struct.builtin.<private name>.ProtocolConformance" %3, 0, !dbg !2809
  %5 = alloca %"struct.builtin.<private name>.TypeInfo"*, !dbg !2809
  store %"struct.builtin.<private name>.TypeInfo"* %4, %"struct.builtin.<private name>.TypeInfo"** %5, !dbg !2809
  store %"struct.builtin.<private name>.TypeInfo"* %1, %"struct.builtin.<private name>.TypeInfo"** %5, !dbg !2809
  %6 = extractvalue %"struct.builtin.<private name>.ProtocolConformance" %3, 1, !dbg !2809
  %7 = alloca i8**, !dbg !2809
  store i8** %6, i8*** %7, !dbg !2809
  store i8** %2, i8*** %7, !dbg !2809
  ret void, !dbg !2809
}

define void @_CNW7builtinE19ProtocolConformanceC2ERS0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable(%"struct.builtin.<private name>.ProtocolConformance"*, %"struct.builtin.<private name>.TypeInfo"*, i8**) !dbg !2810 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %3 = load %"struct.builtin.<private name>.ProtocolConformance", %"struct.builtin.<private name>.ProtocolConformance"* %0, align 8, !dbg !2811
  %4 = extractvalue %"struct.builtin.<private name>.ProtocolConformance" %3, 0, !dbg !2811
  %5 = alloca %"struct.builtin.<private name>.TypeInfo"*, !dbg !2811
  store %"struct.builtin.<private name>.TypeInfo"* %4, %"struct.builtin.<private name>.TypeInfo"** %5, !dbg !2811
  store %"struct.builtin.<private name>.TypeInfo"* %1, %"struct.builtin.<private name>.TypeInfo"** %5, !dbg !2811
  %6 = extractvalue %"struct.builtin.<private name>.ProtocolConformance" %3, 1, !dbg !2811
  %7 = alloca i8**, !dbg !2811
  store i8** %6, i8*** %7, !dbg !2811
  store i8** %2, i8*** %7, !dbg !2811
  ret void, !dbg !2811
}

define void @_CNW7builtinE19ProtocolConformanceD3Ev(%"struct.builtin.<private name>.ProtocolConformance"*) !dbg !2812 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.builtin.<private name>.ProtocolConformance", %"struct.builtin.<private name>.ProtocolConformance"* %0, align 8, !dbg !2813
  %2 = extractvalue %"struct.builtin.<private name>.ProtocolConformance" %1, 0, !dbg !2813
  %3 = alloca %"struct.builtin.<private name>.TypeInfo"*, !dbg !2813
  store %"struct.builtin.<private name>.TypeInfo"* %2, %"struct.builtin.<private name>.TypeInfo"** %3, !dbg !2813
  %4 = extractvalue %"struct.builtin.<private name>.ProtocolConformance" %1, 1, !dbg !2813
  %5 = alloca i8**, !dbg !2813
  store i8** %4, i8*** %5, !dbg !2813
  %6 = bitcast i8*** %5 to i8*, !dbg !2813
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %6), !dbg !2813
  %7 = bitcast %"struct.builtin.<private name>.TypeInfo"** %3 to i8*, !dbg !2813
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %7), !dbg !2813
  ret void, !dbg !2813
}

define void @_CNW7builtinE8TypeInfoD0Ev(%"struct.builtin.<private name>.TypeInfo"*) !dbg !2814 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW7builtinE8TypeInfoD3Ev(%"struct.builtin.<private name>.TypeInfo"* %0), !dbg !2818
  ret void, !dbg !2818
}

define void @_CNW7builtinE8TypeInfo14__default_initES0_L0(%"struct.builtin.<private name>.TypeInfo"*) !dbg !2819 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !2822
}

define void @_CNW7builtinE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride(%"struct.builtin.<private name>.TypeInfo"*, %"struct.builtin.<private name>.TypeInfo"*, i8**, void (i8*)*, i8*, %"struct.builtin.<private name>.ValueWitnessTable"*, %"struct.builtin.<private name>.ProtocolConformance"*, i64, i64, i64) !dbg !2823 {
entry:
  call void @_CNW7builtinE8TypeInfo14__default_initES0_L0(%"struct.builtin.<private name>.TypeInfo"* %0), !dbg !2826
  %10 = bitcast %"struct.builtin.<private name>.TypeInfo"* %0 to %"struct.builtin.<private name>.TypeInfo"**
  store %"struct.builtin.<private name>.TypeInfo"* %1, %"struct.builtin.<private name>.TypeInfo"** %10, align 8, !dbg !2826
  %11 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !2826
  store i8** %2, i8*** %11, align 8, !dbg !2826
  %12 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !2826
  store void (i8*)* %3, void (i8*)** %12, align 8, !dbg !2826
  %13 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !2826
  store i8* %4, i8** %13, align 8, !dbg !2826
  %14 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !2826
  store %"struct.builtin.<private name>.ValueWitnessTable"* %5, %"struct.builtin.<private name>.ValueWitnessTable"** %14, align 8, !dbg !2826
  %15 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !2826
  store %"struct.builtin.<private name>.ProtocolConformance"* %6, %"struct.builtin.<private name>.ProtocolConformance"** %15, align 8, !dbg !2826
  %16 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !2826
  store i64 %7, i64* %16, align 8, !dbg !2826
  %17 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !2826
  store i64 %8, i64* %17, align 8, !dbg !2826
  %18 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !2826
  store i64 %9, i64* %18, align 8, !dbg !2826
  ret void, !dbg !2826
}

define void @_CNW7builtinE8TypeInfoC2ERS0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride(%"struct.builtin.<private name>.TypeInfo"*, %"struct.builtin.<private name>.TypeInfo"*, i8**, void (i8*)*, i8*, %"struct.builtin.<private name>.ValueWitnessTable"*, %"struct.builtin.<private name>.ProtocolConformance"*, i64, i64, i64) !dbg !2827 {
entry:
  %10 = bitcast %"struct.builtin.<private name>.TypeInfo"* %0 to %"struct.builtin.<private name>.TypeInfo"**
  store %"struct.builtin.<private name>.TypeInfo"* %1, %"struct.builtin.<private name>.TypeInfo"** %10, align 8, !dbg !2828
  %11 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !2828
  store i8** %2, i8*** %11, align 8, !dbg !2828
  %12 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !2828
  store void (i8*)* %3, void (i8*)** %12, align 8, !dbg !2828
  %13 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !2828
  store i8* %4, i8** %13, align 8, !dbg !2828
  %14 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !2828
  store %"struct.builtin.<private name>.ValueWitnessTable"* %5, %"struct.builtin.<private name>.ValueWitnessTable"** %14, align 8, !dbg !2828
  %15 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !2828
  store %"struct.builtin.<private name>.ProtocolConformance"* %6, %"struct.builtin.<private name>.ProtocolConformance"** %15, align 8, !dbg !2828
  %16 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !2828
  store i64 %7, i64* %16, align 8, !dbg !2828
  %17 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !2828
  store i64 %8, i64* %17, align 8, !dbg !2828
  %18 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !2828
  store i64 %9, i64* %18, align 8, !dbg !2828
  ret void, !dbg !2828
}

define void @_CNW7builtinE8TypeInfoD3Ev(%"struct.builtin.<private name>.TypeInfo"*) !dbg !2829 {
entry:
  %1 = bitcast %"struct.builtin.<private name>.TypeInfo"* %0 to %"struct.builtin.<private name>.TypeInfo"**
  %2 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 1, !dbg !2830
  %3 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 2, !dbg !2830
  %4 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 3, !dbg !2830
  %5 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 4, !dbg !2830
  %6 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 5, !dbg !2830
  %7 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 6, !dbg !2830
  %8 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 7, !dbg !2830
  %9 = getelementptr inbounds %"struct.builtin.<private name>.TypeInfo", %"struct.builtin.<private name>.TypeInfo"* %0, i32 0, i32 8, !dbg !2830
  %10 = bitcast i64* %9 to i8*, !dbg !2830
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %10), !dbg !2830
  %11 = bitcast i64* %8 to i8*, !dbg !2830
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %11), !dbg !2830
  %12 = bitcast i64* %7 to i8*, !dbg !2830
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %12), !dbg !2830
  %13 = bitcast %"struct.builtin.<private name>.ProtocolConformance"** %6 to i8*, !dbg !2830
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %13), !dbg !2830
  %14 = bitcast %"struct.builtin.<private name>.ValueWitnessTable"** %5 to i8*, !dbg !2830
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %14), !dbg !2830
  %15 = bitcast i8** %4 to i8*, !dbg !2830
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %15), !dbg !2830
  %16 = bitcast void (i8*)** %3 to i8*, !dbg !2830
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %16), !dbg !2830
  %17 = bitcast i8*** %2 to i8*, !dbg !2830
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %17), !dbg !2830
  %18 = bitcast %"struct.builtin.<private name>.TypeInfo"** %1 to i8*, !dbg !2830
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %18), !dbg !2830
  ret void, !dbg !2830
}

define void @_CNW7builtinE5Int64D0Ev(%"struct.builtin.<private name>.Int64"*) !dbg !2831 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW7builtinE5Int64D3Ev(%"struct.builtin.<private name>.Int64"* %0), !dbg !2838
  ret void, !dbg !2838
}

define %"struct.builtin.<private name>.Int64" @_CNW7builtinE5Int644copyES0_L0(%"struct.builtin.<private name>.Int64") !dbg !2839 {
alloca_block:
  %1 = alloca %"struct.builtin.<private name>.Int64", align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.builtin.<private name>.Int64", %"struct.builtin.<private name>.Int64"* %1, align 8, !dbg !2842
  %3 = extractvalue %"struct.builtin.<private name>.Int64" %2, 0, !dbg !2842
  %4 = alloca i64, !dbg !2842
  store i64 %3, i64* %4, !dbg !2842
  %5 = extractvalue %"struct.builtin.<private name>.Int64" %0, 0, !dbg !2842
  %6 = alloca i64, !dbg !2842
  store i64 %5, i64* %6, !dbg !2842
  %7 = load i64, i64* %6, !dbg !2842
  store i64 %7, i64* %4, !dbg !2842
  ret %"struct.builtin.<private name>.Int64" %2, !dbg !2842
}

define void @_CNW7builtinE5Int6414__default_initES0_L0(%"struct.builtin.<private name>.Int64") !dbg !2843 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !2844
}

define void @_CNW7builtinE5Int64C1ElL5value(%"struct.builtin.<private name>.Int64"*, i64) !dbg !2845 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.builtin.<private name>.Int64", %"struct.builtin.<private name>.Int64"* %0, align 8, !dbg !2848
  call void @_CNW7builtinE5Int6414__default_initES0_L0(%"struct.builtin.<private name>.Int64" %2), !dbg !2848
  %3 = extractvalue %"struct.builtin.<private name>.Int64" %2, 0, !dbg !2848
  %4 = alloca i64, !dbg !2848
  store i64 %3, i64* %4, !dbg !2848
  store i64 %1, i64* %4, !dbg !2848
  ret void, !dbg !2848
}

define void @_CNW7builtinE5Int64C2ERS0_lL0L5value(%"struct.builtin.<private name>.Int64"*, i64) !dbg !2849 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %2 = load %"struct.builtin.<private name>.Int64", %"struct.builtin.<private name>.Int64"* %0, align 8, !dbg !2850
  %3 = extractvalue %"struct.builtin.<private name>.Int64" %2, 0, !dbg !2850
  %4 = alloca i64, !dbg !2850
  store i64 %3, i64* %4, !dbg !2850
  store i64 %1, i64* %4, !dbg !2850
  ret void, !dbg !2850
}

define void @_CNW7builtinE5Int64D3Ev(%"struct.builtin.<private name>.Int64"*) !dbg !2851 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.builtin.<private name>.Int64", %"struct.builtin.<private name>.Int64"* %0, align 8, !dbg !2852
  %2 = extractvalue %"struct.builtin.<private name>.Int64" %1, 0, !dbg !2852
  %3 = alloca i64, !dbg !2852
  store i64 %2, i64* %3, !dbg !2852
  %4 = bitcast i64* %3 to i8*, !dbg !2852
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %4), !dbg !2852
  ret void, !dbg !2852
}

define void @_CNW7builtinE20ExistentialContainerD0Ev(%"struct.builtin.<private name>.ExistentialContainer"*) !dbg !2853 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW7builtinE20ExistentialContainerD3Ev(%"struct.builtin.<private name>.ExistentialContainer"* %0), !dbg !2862
  ret void, !dbg !2862
}

define void @_CNW7builtinE20ExistentialContainer14__default_initES0_L0(%"struct.builtin.<private name>.ExistentialContainer") !dbg !2863 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void, !dbg !2866
}

define void @_CNW7builtinE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance(%"struct.builtin.<private name>.ExistentialContainer"*, i8*, %"struct.builtin.<private name>.TypeInfo"*, %"struct.builtin.<private name>.ProtocolConformance"*) !dbg !2867 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %4 = load %"struct.builtin.<private name>.ExistentialContainer", %"struct.builtin.<private name>.ExistentialContainer"* %0, align 8, !dbg !2870
  call void @_CNW7builtinE20ExistentialContainer14__default_initES0_L0(%"struct.builtin.<private name>.ExistentialContainer" %4), !dbg !2870
  %5 = extractvalue %"struct.builtin.<private name>.ExistentialContainer" %4, 0, !dbg !2870
  %6 = alloca i8*, !dbg !2870
  store i8* %5, i8** %6, !dbg !2870
  store i8* %1, i8** %6, !dbg !2870
  %7 = extractvalue %"struct.builtin.<private name>.ExistentialContainer" %4, 1, !dbg !2870
  %8 = alloca %"struct.builtin.<private name>.TypeInfo"*, !dbg !2870
  store %"struct.builtin.<private name>.TypeInfo"* %7, %"struct.builtin.<private name>.TypeInfo"** %8, !dbg !2870
  store %"struct.builtin.<private name>.TypeInfo"* %2, %"struct.builtin.<private name>.TypeInfo"** %8, !dbg !2870
  %9 = extractvalue %"struct.builtin.<private name>.ExistentialContainer" %4, 2, !dbg !2870
  %10 = alloca %"struct.builtin.<private name>.ProtocolConformance"*, !dbg !2870
  store %"struct.builtin.<private name>.ProtocolConformance"* %9, %"struct.builtin.<private name>.ProtocolConformance"** %10, !dbg !2870
  store %"struct.builtin.<private name>.ProtocolConformance"* %3, %"struct.builtin.<private name>.ProtocolConformance"** %10, !dbg !2870
  ret void, !dbg !2870
}

define void @_CNW7builtinE20ExistentialContainerC2ERS0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance(%"struct.builtin.<private name>.ExistentialContainer"*, i8*, %"struct.builtin.<private name>.TypeInfo"*, %"struct.builtin.<private name>.ProtocolConformance"*) !dbg !2871 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %4 = load %"struct.builtin.<private name>.ExistentialContainer", %"struct.builtin.<private name>.ExistentialContainer"* %0, align 8, !dbg !2872
  %5 = extractvalue %"struct.builtin.<private name>.ExistentialContainer" %4, 0, !dbg !2872
  %6 = alloca i8*, !dbg !2872
  store i8* %5, i8** %6, !dbg !2872
  store i8* %1, i8** %6, !dbg !2872
  %7 = extractvalue %"struct.builtin.<private name>.ExistentialContainer" %4, 1, !dbg !2872
  %8 = alloca %"struct.builtin.<private name>.TypeInfo"*, !dbg !2872
  store %"struct.builtin.<private name>.TypeInfo"* %7, %"struct.builtin.<private name>.TypeInfo"** %8, !dbg !2872
  store %"struct.builtin.<private name>.TypeInfo"* %2, %"struct.builtin.<private name>.TypeInfo"** %8, !dbg !2872
  %9 = extractvalue %"struct.builtin.<private name>.ExistentialContainer" %4, 2, !dbg !2872
  %10 = alloca %"struct.builtin.<private name>.ProtocolConformance"*, !dbg !2872
  store %"struct.builtin.<private name>.ProtocolConformance"* %9, %"struct.builtin.<private name>.ProtocolConformance"** %10, !dbg !2872
  store %"struct.builtin.<private name>.ProtocolConformance"* %3, %"struct.builtin.<private name>.ProtocolConformance"** %10, !dbg !2872
  ret void, !dbg !2872
}

define void @_CNW7builtinE20ExistentialContainerD3Ev(%"struct.builtin.<private name>.ExistentialContainer"*) !dbg !2873 {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = load %"struct.builtin.<private name>.ExistentialContainer", %"struct.builtin.<private name>.ExistentialContainer"* %0, align 8, !dbg !2874
  %2 = extractvalue %"struct.builtin.<private name>.ExistentialContainer" %1, 0, !dbg !2874
  %3 = alloca i8*, !dbg !2874
  store i8* %2, i8** %3, !dbg !2874
  %4 = extractvalue %"struct.builtin.<private name>.ExistentialContainer" %1, 1, !dbg !2874
  %5 = alloca %"struct.builtin.<private name>.TypeInfo"*, !dbg !2874
  store %"struct.builtin.<private name>.TypeInfo"* %4, %"struct.builtin.<private name>.TypeInfo"** %5, !dbg !2874
  %6 = extractvalue %"struct.builtin.<private name>.ExistentialContainer" %1, 2, !dbg !2874
  %7 = alloca %"struct.builtin.<private name>.ProtocolConformance"*, !dbg !2874
  store %"struct.builtin.<private name>.ProtocolConformance"* %6, %"struct.builtin.<private name>.ProtocolConformance"** %7, !dbg !2874
  %8 = bitcast %"struct.builtin.<private name>.ProtocolConformance"** %7 to i8*, !dbg !2874
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %8), !dbg !2874
  %9 = bitcast %"struct.builtin.<private name>.TypeInfo"** %5 to i8*, !dbg !2874
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %9), !dbg !2874
  %10 = bitcast i8** %3 to i8*, !dbg !2874
  call void @llvm.lifetime.end.p0i8(i64 8, i8* %10), !dbg !2874
  ret void, !dbg !2874
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #0

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.value(metadata, metadata, metadata) #0

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #1

declare void @llvm.coro.resume(i8*)

declare void @llvm.coro.destroy(i8*)

; Function Attrs: argmemonly nounwind
declare i1 @llvm.coro.done(i8* nocapture readonly) #1

; Function Attrs: nounwind readnone speculatable
declare float @llvm.log10.f32(float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.log2.f32(float) #0

declare float @llvm.log1.f32(float)

; Function Attrs: nounwind readnone speculatable
declare float @llvm.floor.f32(float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.ceil.f32(float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.round.f32(float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.rint.f32(float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.trunc.f32(float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.sqrt.f32(float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.fabs.f32(float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.sin.f32(float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.cos.f32(float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.exp.f32(float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.exp2.f32(float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.pow.f32(float, float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.powi.f32(float, i32) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.minnum.f32(float, float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.maxnum.f32(float, float) #0

declare float @llvm.minimum.f32(float, float)

declare float @llvm.maximum.f32(float, float)

; Function Attrs: nounwind readnone speculatable
declare float @llvm.copysign.f32(float, float) #0

; Function Attrs: nounwind readnone speculatable
declare float @llvm.fma.f32(float, float, float) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.log10.f64(double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.log2.f64(double) #0

declare double @llvm.log1.f64(double)

; Function Attrs: nounwind readnone speculatable
declare double @llvm.floor.f64(double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.ceil.f64(double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.round.f64(double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.rint.f64(double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.trunc.f64(double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.sqrt.f64(double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.fabs.f64(double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.sin.f64(double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.cos.f64(double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.exp.f64(double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.exp2.f64(double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.pow.f64(double, double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.powi.f64(double, i32) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.minnum.f64(double, double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.maxnum.f64(double, double) #0

declare double @llvm.minimum.f64(double, double)

declare double @llvm.maximum.f64(double, double)

; Function Attrs: nounwind readnone speculatable
declare double @llvm.copysign.f64(double, double) #0

; Function Attrs: nounwind readnone speculatable
declare double @llvm.fma.f64(double, double, double) #0

; Function Attrs: nounwind readnone speculatable
declare i1 @llvm.ctlz.i1(i1, i1) #0

; Function Attrs: nounwind readnone speculatable
declare i1 @llvm.cttz.i1(i1, i1) #0

; Function Attrs: nounwind readnone speculatable
declare i1 @llvm.ctpop.i1(i1) #0

; Function Attrs: nounwind readnone speculatable
declare i1 @llvm.bitreverse.i1(i1) #0

; Function Attrs: nounwind readnone speculatable
declare { i1, i1 } @llvm.sadd.with.overflow.i1(i1, i1) #0

; Function Attrs: nounwind readnone speculatable
declare { i1, i1 } @llvm.uadd.with.overflow.i1(i1, i1) #0

; Function Attrs: nounwind readnone speculatable
declare { i1, i1 } @llvm.ssub.with.overflow.i1(i1, i1) #0

; Function Attrs: nounwind readnone speculatable
declare { i1, i1 } @llvm.usub.with.overflow.i1(i1, i1) #0

; Function Attrs: nounwind readnone speculatable
declare { i1, i1 } @llvm.smul.with.overflow.i1(i1, i1) #0

; Function Attrs: nounwind readnone speculatable
declare { i1, i1 } @llvm.umul.with.overflow.i1(i1, i1) #0

; Function Attrs: nounwind readnone speculatable
declare i8 @llvm.ctlz.i8(i8, i1) #0

; Function Attrs: nounwind readnone speculatable
declare i8 @llvm.cttz.i8(i8, i1) #0

; Function Attrs: nounwind readnone speculatable
declare i8 @llvm.ctpop.i8(i8) #0

; Function Attrs: nounwind readnone speculatable
declare i8 @llvm.bitreverse.i8(i8) #0

; Function Attrs: nounwind readnone speculatable
declare { i8, i1 } @llvm.sadd.with.overflow.i8(i8, i8) #0

; Function Attrs: nounwind readnone speculatable
declare { i8, i1 } @llvm.uadd.with.overflow.i8(i8, i8) #0

; Function Attrs: nounwind readnone speculatable
declare { i8, i1 } @llvm.ssub.with.overflow.i8(i8, i8) #0

; Function Attrs: nounwind readnone speculatable
declare { i8, i1 } @llvm.usub.with.overflow.i8(i8, i8) #0

; Function Attrs: nounwind readnone speculatable
declare { i8, i1 } @llvm.smul.with.overflow.i8(i8, i8) #0

; Function Attrs: nounwind readnone speculatable
declare { i8, i1 } @llvm.umul.with.overflow.i8(i8, i8) #0

; Function Attrs: nounwind readnone speculatable
declare i16 @llvm.ctlz.i16(i16, i1) #0

; Function Attrs: nounwind readnone speculatable
declare i16 @llvm.cttz.i16(i16, i1) #0

; Function Attrs: nounwind readnone speculatable
declare i16 @llvm.ctpop.i16(i16) #0

; Function Attrs: nounwind readnone speculatable
declare i16 @llvm.bitreverse.i16(i16) #0

; Function Attrs: nounwind readnone speculatable
declare { i16, i1 } @llvm.sadd.with.overflow.i16(i16, i16) #0

; Function Attrs: nounwind readnone speculatable
declare { i16, i1 } @llvm.uadd.with.overflow.i16(i16, i16) #0

; Function Attrs: nounwind readnone speculatable
declare { i16, i1 } @llvm.ssub.with.overflow.i16(i16, i16) #0

; Function Attrs: nounwind readnone speculatable
declare { i16, i1 } @llvm.usub.with.overflow.i16(i16, i16) #0

; Function Attrs: nounwind readnone speculatable
declare { i16, i1 } @llvm.smul.with.overflow.i16(i16, i16) #0

; Function Attrs: nounwind readnone speculatable
declare { i16, i1 } @llvm.umul.with.overflow.i16(i16, i16) #0

; Function Attrs: nounwind readnone speculatable
declare i32 @llvm.ctlz.i32(i32, i1) #0

; Function Attrs: nounwind readnone speculatable
declare i32 @llvm.cttz.i32(i32, i1) #0

; Function Attrs: nounwind readnone speculatable
declare i32 @llvm.ctpop.i32(i32) #0

; Function Attrs: nounwind readnone speculatable
declare i32 @llvm.bitreverse.i32(i32) #0

; Function Attrs: nounwind readnone speculatable
declare { i32, i1 } @llvm.sadd.with.overflow.i32(i32, i32) #0

; Function Attrs: nounwind readnone speculatable
declare { i32, i1 } @llvm.uadd.with.overflow.i32(i32, i32) #0

; Function Attrs: nounwind readnone speculatable
declare { i32, i1 } @llvm.ssub.with.overflow.i32(i32, i32) #0

; Function Attrs: nounwind readnone speculatable
declare { i32, i1 } @llvm.usub.with.overflow.i32(i32, i32) #0

; Function Attrs: nounwind readnone speculatable
declare { i32, i1 } @llvm.smul.with.overflow.i32(i32, i32) #0

; Function Attrs: nounwind readnone speculatable
declare { i32, i1 } @llvm.umul.with.overflow.i32(i32, i32) #0

; Function Attrs: nounwind readnone speculatable
declare i64 @llvm.ctlz.i64(i64, i1) #0

; Function Attrs: nounwind readnone speculatable
declare i64 @llvm.cttz.i64(i64, i1) #0

; Function Attrs: nounwind readnone speculatable
declare i64 @llvm.ctpop.i64(i64) #0

; Function Attrs: nounwind readnone speculatable
declare i64 @llvm.bitreverse.i64(i64) #0

; Function Attrs: nounwind readnone speculatable
declare { i64, i1 } @llvm.sadd.with.overflow.i64(i64, i64) #0

; Function Attrs: nounwind readnone speculatable
declare { i64, i1 } @llvm.uadd.with.overflow.i64(i64, i64) #0

; Function Attrs: nounwind readnone speculatable
declare { i64, i1 } @llvm.ssub.with.overflow.i64(i64, i64) #0

; Function Attrs: nounwind readnone speculatable
declare { i64, i1 } @llvm.usub.with.overflow.i64(i64, i64) #0

; Function Attrs: nounwind readnone speculatable
declare { i64, i1 } @llvm.smul.with.overflow.i64(i64, i64) #0

; Function Attrs: nounwind readnone speculatable
declare { i64, i1 } @llvm.umul.with.overflow.i64(i64, i64) #0

; Function Attrs: nounwind readnone speculatable
declare i128 @llvm.ctlz.i128(i128, i1) #0

; Function Attrs: nounwind readnone speculatable
declare i128 @llvm.cttz.i128(i128, i1) #0

; Function Attrs: nounwind readnone speculatable
declare i128 @llvm.ctpop.i128(i128) #0

; Function Attrs: nounwind readnone speculatable
declare i128 @llvm.bitreverse.i128(i128) #0

; Function Attrs: nounwind readnone speculatable
declare { i128, i1 } @llvm.sadd.with.overflow.i128(i128, i128) #0

; Function Attrs: nounwind readnone speculatable
declare { i128, i1 } @llvm.uadd.with.overflow.i128(i128, i128) #0

; Function Attrs: nounwind readnone speculatable
declare { i128, i1 } @llvm.ssub.with.overflow.i128(i128, i128) #0

; Function Attrs: nounwind readnone speculatable
declare { i128, i1 } @llvm.usub.with.overflow.i128(i128, i128) #0

; Function Attrs: nounwind readnone speculatable
declare { i128, i1 } @llvm.smul.with.overflow.i128(i128, i128) #0

; Function Attrs: nounwind readnone speculatable
declare { i128, i1 } @llvm.umul.with.overflow.i128(i128, i128) #0

; Function Attrs: nounwind readnone speculatable
declare i16 @llvm.bswap.i16(i16) #0

; Function Attrs: nounwind readnone speculatable
declare i32 @llvm.bswap.i32(i32) #0

; Function Attrs: nounwind readnone speculatable
declare i64 @llvm.bswap.i64(i64) #0

; Function Attrs: nounwind readnone speculatable
declare i128 @llvm.bswap.i128(i128) #0

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind readonly
declare i8* @llvm.coro.subfn.addr(i8* nocapture readonly, i8) #2

; Function Attrs: nounwind
declare void @llvm.stackprotector(i8*, i8**) #3

attributes #0 = { nounwind readnone speculatable }
attributes #1 = { argmemonly nounwind }
attributes #2 = { argmemonly nounwind readonly }
attributes #3 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "cdotc v0.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "/Users/Jonas/CDot/StdLib/builtin/mod.dotm", directory: "")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2==EKPvS_L0L0", linkageName: "_CNW7builtin3opsEXi2==EKPvS_L0L0", scope: null, file: !6, line: 82, type: !7, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!6 = !DIFile(filename: "BuiltinOps.dot", directory: "/Users/Jonas/CDot/StdLib/builtin/")
!7 = !DISubroutineType(types: !8)
!8 = !{!9, !9}
!9 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64, align: 8)
!10 = !DILocalVariable(name: "lhs", scope: !5, file: !1, line: 82, type: !9)
!11 = !DILocation(line: 82, column: 2, scope: !5)
!12 = !DILocalVariable(name: "rhs", scope: !5, file: !1, line: 82, type: !9)
!13 = !DILocation(line: 83, column: 25, scope: !5)
!14 = !DILocation(line: 83, column: 30, scope: !5)
!15 = !DILocation(line: 83, column: 13, scope: !5)
!16 = distinct !DISubprogram(name: "_CNW7builtinE11icmp_eq_ptrEKPvS_L0L0", linkageName: "_CNW7builtinE11icmp_eq_ptrEKPvS_L0L0", scope: null, file: !17, line: 354, type: !7, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!17 = !DIFile(filename: "Builtin.dot", directory: "/Users/Jonas/CDot/StdLib/builtin/")
!18 = !DILocalVariable(name: "lhs", scope: !16, file: !1, line: 354, type: !9)
!19 = !DILocation(line: 354, column: 2, scope: !16)
!20 = !DILocalVariable(name: "rhs", scope: !16, file: !1, line: 354, type: !9)
!21 = !DILocation(line: 355, column: 6, scope: !16)
!22 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2!=EKPvS_L0L0", linkageName: "_CNW7builtin3opsEXi2!=EKPvS_L0L0", scope: null, file: !6, line: 85, type: !7, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!23 = !DILocalVariable(name: "lhs", scope: !22, file: !1, line: 85, type: !9)
!24 = !DILocation(line: 85, column: 2, scope: !22)
!25 = !DILocalVariable(name: "rhs", scope: !22, file: !1, line: 85, type: !9)
!26 = !DILocation(line: 86, column: 25, scope: !22)
!27 = !DILocation(line: 86, column: 30, scope: !22)
!28 = !DILocation(line: 86, column: 13, scope: !22)
!29 = distinct !DISubprogram(name: "_CNW7builtinE11icmp_ne_ptrEKPvS_L0L0", linkageName: "_CNW7builtinE11icmp_ne_ptrEKPvS_L0L0", scope: null, file: !17, line: 358, type: !7, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!30 = !DILocalVariable(name: "lhs", scope: !29, file: !1, line: 358, type: !9)
!31 = !DILocation(line: 358, column: 2, scope: !29)
!32 = !DILocalVariable(name: "rhs", scope: !29, file: !1, line: 358, type: !9)
!33 = !DILocation(line: 359, column: 6, scope: !29)
!34 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1+EaaL0L0", linkageName: "_CNW7builtin3opsEXi1+EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!35 = !DISubroutineType(types: !36)
!36 = !{!37, !37}
!37 = !DIBasicType(name: "i8", size: 8, encoding: DW_ATE_signed_char)
!38 = !DILocalVariable(name: "lhs", scope: !34, file: !1, line: 4, type: !37)
!39 = !DILocation(line: 4, column: 19, scope: !34)
!40 = !DILocalVariable(name: "rhs", scope: !34, file: !1, line: 4, type: !37)
!41 = !DILocation(line: 5, column: 26, scope: !34)
!42 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2+=EaaL0L0", linkageName: "_CNW7builtin3opsEXi2+=EaaL0L0", scope: null, file: !6, line: 6, type: !43, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!43 = !DISubroutineType(types: !44)
!44 = !{!45, !37}
!45 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !37, size: 64, align: 8)
!46 = !DILocalVariable(name: "lhs", scope: !42, file: !1, line: 6, type: !37)
!47 = !DILocation(line: 6, column: 18, scope: !42)
!48 = !DILocalVariable(name: "rhs", scope: !42, file: !1, line: 6, type: !37)
!49 = !DILocation(line: 9, column: 30, scope: !42)
!50 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1+EhhL0L0", linkageName: "_CNW7builtin3opsEXi1+EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!51 = !DISubroutineType(types: !52)
!52 = !{!53, !53}
!53 = !DIBasicType(name: "u8", size: 8, encoding: DW_ATE_unsigned_char)
!54 = !DILocalVariable(name: "lhs", scope: !50, file: !1, line: 4, type: !53)
!55 = !DILocation(line: 4, column: 19, scope: !50)
!56 = !DILocalVariable(name: "rhs", scope: !50, file: !1, line: 4, type: !53)
!57 = !DILocation(line: 5, column: 26, scope: !50)
!58 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2+=EhhL0L0", linkageName: "_CNW7builtin3opsEXi2+=EhhL0L0", scope: null, file: !6, line: 6, type: !59, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!59 = !DISubroutineType(types: !60)
!60 = !{!61, !53}
!61 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !53, size: 64, align: 8)
!62 = !DILocalVariable(name: "lhs", scope: !58, file: !1, line: 6, type: !53)
!63 = !DILocation(line: 6, column: 18, scope: !58)
!64 = !DILocalVariable(name: "rhs", scope: !58, file: !1, line: 6, type: !53)
!65 = !DILocation(line: 9, column: 30, scope: !58)
!66 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1+EssL0L0", linkageName: "_CNW7builtin3opsEXi1+EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!67 = !DISubroutineType(types: !68)
!68 = !{!69, !69}
!69 = !DIBasicType(name: "i16", size: 16, encoding: DW_ATE_signed)
!70 = !DILocalVariable(name: "lhs", scope: !66, file: !1, line: 4, type: !69)
!71 = !DILocation(line: 4, column: 19, scope: !66)
!72 = !DILocalVariable(name: "rhs", scope: !66, file: !1, line: 4, type: !69)
!73 = !DILocation(line: 5, column: 26, scope: !66)
!74 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2+=EssL0L0", linkageName: "_CNW7builtin3opsEXi2+=EssL0L0", scope: null, file: !6, line: 6, type: !75, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!75 = !DISubroutineType(types: !76)
!76 = !{!77, !69}
!77 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !69, size: 64, align: 8)
!78 = !DILocalVariable(name: "lhs", scope: !74, file: !1, line: 6, type: !69)
!79 = !DILocation(line: 6, column: 18, scope: !74)
!80 = !DILocalVariable(name: "rhs", scope: !74, file: !1, line: 6, type: !69)
!81 = !DILocation(line: 9, column: 30, scope: !74)
!82 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1+EttL0L0", linkageName: "_CNW7builtin3opsEXi1+EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!83 = !DISubroutineType(types: !84)
!84 = !{!85, !85}
!85 = !DIBasicType(name: "u16", size: 16, encoding: DW_ATE_unsigned)
!86 = !DILocalVariable(name: "lhs", scope: !82, file: !1, line: 4, type: !85)
!87 = !DILocation(line: 4, column: 19, scope: !82)
!88 = !DILocalVariable(name: "rhs", scope: !82, file: !1, line: 4, type: !85)
!89 = !DILocation(line: 5, column: 26, scope: !82)
!90 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2+=EttL0L0", linkageName: "_CNW7builtin3opsEXi2+=EttL0L0", scope: null, file: !6, line: 6, type: !91, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!91 = !DISubroutineType(types: !92)
!92 = !{!93, !85}
!93 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !85, size: 64, align: 8)
!94 = !DILocalVariable(name: "lhs", scope: !90, file: !1, line: 6, type: !85)
!95 = !DILocation(line: 6, column: 18, scope: !90)
!96 = !DILocalVariable(name: "rhs", scope: !90, file: !1, line: 6, type: !85)
!97 = !DILocation(line: 9, column: 30, scope: !90)
!98 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1+EiiL0L0", linkageName: "_CNW7builtin3opsEXi1+EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!99 = !DISubroutineType(types: !100)
!100 = !{!101, !101}
!101 = !DIBasicType(name: "i32", size: 32, encoding: DW_ATE_signed)
!102 = !DILocalVariable(name: "lhs", scope: !98, file: !1, line: 4, type: !101)
!103 = !DILocation(line: 4, column: 19, scope: !98)
!104 = !DILocalVariable(name: "rhs", scope: !98, file: !1, line: 4, type: !101)
!105 = !DILocation(line: 5, column: 26, scope: !98)
!106 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2+=EiiL0L0", linkageName: "_CNW7builtin3opsEXi2+=EiiL0L0", scope: null, file: !6, line: 6, type: !107, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!107 = !DISubroutineType(types: !108)
!108 = !{!109, !101}
!109 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !101, size: 64, align: 8)
!110 = !DILocalVariable(name: "lhs", scope: !106, file: !1, line: 6, type: !101)
!111 = !DILocation(line: 6, column: 18, scope: !106)
!112 = !DILocalVariable(name: "rhs", scope: !106, file: !1, line: 6, type: !101)
!113 = !DILocation(line: 9, column: 30, scope: !106)
!114 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1+EjjL0L0", linkageName: "_CNW7builtin3opsEXi1+EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!115 = !DISubroutineType(types: !116)
!116 = !{!117, !117}
!117 = !DIBasicType(name: "u32", size: 32, encoding: DW_ATE_unsigned)
!118 = !DILocalVariable(name: "lhs", scope: !114, file: !1, line: 4, type: !117)
!119 = !DILocation(line: 4, column: 19, scope: !114)
!120 = !DILocalVariable(name: "rhs", scope: !114, file: !1, line: 4, type: !117)
!121 = !DILocation(line: 5, column: 26, scope: !114)
!122 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2+=EjjL0L0", linkageName: "_CNW7builtin3opsEXi2+=EjjL0L0", scope: null, file: !6, line: 6, type: !123, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!123 = !DISubroutineType(types: !124)
!124 = !{!125, !117}
!125 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !117, size: 64, align: 8)
!126 = !DILocalVariable(name: "lhs", scope: !122, file: !1, line: 6, type: !117)
!127 = !DILocation(line: 6, column: 18, scope: !122)
!128 = !DILocalVariable(name: "rhs", scope: !122, file: !1, line: 6, type: !117)
!129 = !DILocation(line: 9, column: 30, scope: !122)
!130 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1+EllL0L0", linkageName: "_CNW7builtin3opsEXi1+EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!131 = !DISubroutineType(types: !132)
!132 = !{!133, !133}
!133 = !DIBasicType(name: "i64", size: 64, encoding: DW_ATE_signed)
!134 = !DILocalVariable(name: "lhs", scope: !130, file: !1, line: 4, type: !133)
!135 = !DILocation(line: 4, column: 19, scope: !130)
!136 = !DILocalVariable(name: "rhs", scope: !130, file: !1, line: 4, type: !133)
!137 = !DILocation(line: 5, column: 26, scope: !130)
!138 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2+=EllL0L0", linkageName: "_CNW7builtin3opsEXi2+=EllL0L0", scope: null, file: !6, line: 6, type: !139, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!139 = !DISubroutineType(types: !140)
!140 = !{!141, !133}
!141 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !133, size: 64, align: 8)
!142 = !DILocalVariable(name: "lhs", scope: !138, file: !1, line: 6, type: !133)
!143 = !DILocation(line: 6, column: 18, scope: !138)
!144 = !DILocalVariable(name: "rhs", scope: !138, file: !1, line: 6, type: !133)
!145 = !DILocation(line: 9, column: 30, scope: !138)
!146 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1+EmmL0L0", linkageName: "_CNW7builtin3opsEXi1+EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!147 = !DISubroutineType(types: !148)
!148 = !{!149, !149}
!149 = !DIBasicType(name: "u64", size: 64, encoding: DW_ATE_unsigned)
!150 = !DILocalVariable(name: "lhs", scope: !146, file: !1, line: 4, type: !149)
!151 = !DILocation(line: 4, column: 19, scope: !146)
!152 = !DILocalVariable(name: "rhs", scope: !146, file: !1, line: 4, type: !149)
!153 = !DILocation(line: 5, column: 26, scope: !146)
!154 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2+=EmmL0L0", linkageName: "_CNW7builtin3opsEXi2+=EmmL0L0", scope: null, file: !6, line: 6, type: !155, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!155 = !DISubroutineType(types: !156)
!156 = !{!157, !149}
!157 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !149, size: 64, align: 8)
!158 = !DILocalVariable(name: "lhs", scope: !154, file: !1, line: 6, type: !149)
!159 = !DILocation(line: 6, column: 18, scope: !154)
!160 = !DILocalVariable(name: "rhs", scope: !154, file: !1, line: 6, type: !149)
!161 = !DILocation(line: 9, column: 30, scope: !154)
!162 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1+EnnL0L0", linkageName: "_CNW7builtin3opsEXi1+EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!163 = !DISubroutineType(types: !164)
!164 = !{!165, !165}
!165 = !DIBasicType(name: "i128", size: 128, encoding: DW_ATE_signed)
!166 = !DILocalVariable(name: "lhs", scope: !162, file: !1, line: 4, type: !165)
!167 = !DILocation(line: 4, column: 19, scope: !162)
!168 = !DILocalVariable(name: "rhs", scope: !162, file: !1, line: 4, type: !165)
!169 = !DILocation(line: 5, column: 26, scope: !162)
!170 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2+=EnnL0L0", linkageName: "_CNW7builtin3opsEXi2+=EnnL0L0", scope: null, file: !6, line: 6, type: !171, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!171 = !DISubroutineType(types: !172)
!172 = !{!173, !165}
!173 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !165, size: 64, align: 8)
!174 = !DILocalVariable(name: "lhs", scope: !170, file: !1, line: 6, type: !165)
!175 = !DILocation(line: 6, column: 18, scope: !170)
!176 = !DILocalVariable(name: "rhs", scope: !170, file: !1, line: 6, type: !165)
!177 = !DILocation(line: 9, column: 30, scope: !170)
!178 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1+EooL0L0", linkageName: "_CNW7builtin3opsEXi1+EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!179 = !DISubroutineType(types: !180)
!180 = !{!181, !181}
!181 = !DIBasicType(name: "u128", size: 128, encoding: DW_ATE_unsigned)
!182 = !DILocalVariable(name: "lhs", scope: !178, file: !1, line: 4, type: !181)
!183 = !DILocation(line: 4, column: 19, scope: !178)
!184 = !DILocalVariable(name: "rhs", scope: !178, file: !1, line: 4, type: !181)
!185 = !DILocation(line: 5, column: 26, scope: !178)
!186 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2+=EooL0L0", linkageName: "_CNW7builtin3opsEXi2+=EooL0L0", scope: null, file: !6, line: 6, type: !187, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!187 = !DISubroutineType(types: !188)
!188 = !{!189, !181}
!189 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !181, size: 64, align: 8)
!190 = !DILocalVariable(name: "lhs", scope: !186, file: !1, line: 6, type: !181)
!191 = !DILocation(line: 6, column: 18, scope: !186)
!192 = !DILocalVariable(name: "rhs", scope: !186, file: !1, line: 6, type: !181)
!193 = !DILocation(line: 9, column: 30, scope: !186)
!194 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1-EaaL0L0", linkageName: "_CNW7builtin3opsEXi1-EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!195 = !DILocalVariable(name: "lhs", scope: !194, file: !1, line: 4, type: !37)
!196 = !DILocation(line: 4, column: 19, scope: !194)
!197 = !DILocalVariable(name: "rhs", scope: !194, file: !1, line: 4, type: !37)
!198 = !DILocation(line: 5, column: 26, scope: !194)
!199 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2-=EaaL0L0", linkageName: "_CNW7builtin3opsEXi2-=EaaL0L0", scope: null, file: !6, line: 6, type: !43, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!200 = !DILocalVariable(name: "lhs", scope: !199, file: !1, line: 6, type: !37)
!201 = !DILocation(line: 6, column: 18, scope: !199)
!202 = !DILocalVariable(name: "rhs", scope: !199, file: !1, line: 6, type: !37)
!203 = !DILocation(line: 9, column: 30, scope: !199)
!204 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1-EhhL0L0", linkageName: "_CNW7builtin3opsEXi1-EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!205 = !DILocalVariable(name: "lhs", scope: !204, file: !1, line: 4, type: !53)
!206 = !DILocation(line: 4, column: 19, scope: !204)
!207 = !DILocalVariable(name: "rhs", scope: !204, file: !1, line: 4, type: !53)
!208 = !DILocation(line: 5, column: 26, scope: !204)
!209 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2-=EhhL0L0", linkageName: "_CNW7builtin3opsEXi2-=EhhL0L0", scope: null, file: !6, line: 6, type: !59, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!210 = !DILocalVariable(name: "lhs", scope: !209, file: !1, line: 6, type: !53)
!211 = !DILocation(line: 6, column: 18, scope: !209)
!212 = !DILocalVariable(name: "rhs", scope: !209, file: !1, line: 6, type: !53)
!213 = !DILocation(line: 9, column: 30, scope: !209)
!214 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1-EssL0L0", linkageName: "_CNW7builtin3opsEXi1-EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!215 = !DILocalVariable(name: "lhs", scope: !214, file: !1, line: 4, type: !69)
!216 = !DILocation(line: 4, column: 19, scope: !214)
!217 = !DILocalVariable(name: "rhs", scope: !214, file: !1, line: 4, type: !69)
!218 = !DILocation(line: 5, column: 26, scope: !214)
!219 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2-=EssL0L0", linkageName: "_CNW7builtin3opsEXi2-=EssL0L0", scope: null, file: !6, line: 6, type: !75, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!220 = !DILocalVariable(name: "lhs", scope: !219, file: !1, line: 6, type: !69)
!221 = !DILocation(line: 6, column: 18, scope: !219)
!222 = !DILocalVariable(name: "rhs", scope: !219, file: !1, line: 6, type: !69)
!223 = !DILocation(line: 9, column: 30, scope: !219)
!224 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1-EttL0L0", linkageName: "_CNW7builtin3opsEXi1-EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!225 = !DILocalVariable(name: "lhs", scope: !224, file: !1, line: 4, type: !85)
!226 = !DILocation(line: 4, column: 19, scope: !224)
!227 = !DILocalVariable(name: "rhs", scope: !224, file: !1, line: 4, type: !85)
!228 = !DILocation(line: 5, column: 26, scope: !224)
!229 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2-=EttL0L0", linkageName: "_CNW7builtin3opsEXi2-=EttL0L0", scope: null, file: !6, line: 6, type: !91, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!230 = !DILocalVariable(name: "lhs", scope: !229, file: !1, line: 6, type: !85)
!231 = !DILocation(line: 6, column: 18, scope: !229)
!232 = !DILocalVariable(name: "rhs", scope: !229, file: !1, line: 6, type: !85)
!233 = !DILocation(line: 9, column: 30, scope: !229)
!234 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1-EiiL0L0", linkageName: "_CNW7builtin3opsEXi1-EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!235 = !DILocalVariable(name: "lhs", scope: !234, file: !1, line: 4, type: !101)
!236 = !DILocation(line: 4, column: 19, scope: !234)
!237 = !DILocalVariable(name: "rhs", scope: !234, file: !1, line: 4, type: !101)
!238 = !DILocation(line: 5, column: 26, scope: !234)
!239 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2-=EiiL0L0", linkageName: "_CNW7builtin3opsEXi2-=EiiL0L0", scope: null, file: !6, line: 6, type: !107, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!240 = !DILocalVariable(name: "lhs", scope: !239, file: !1, line: 6, type: !101)
!241 = !DILocation(line: 6, column: 18, scope: !239)
!242 = !DILocalVariable(name: "rhs", scope: !239, file: !1, line: 6, type: !101)
!243 = !DILocation(line: 9, column: 30, scope: !239)
!244 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1-EjjL0L0", linkageName: "_CNW7builtin3opsEXi1-EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!245 = !DILocalVariable(name: "lhs", scope: !244, file: !1, line: 4, type: !117)
!246 = !DILocation(line: 4, column: 19, scope: !244)
!247 = !DILocalVariable(name: "rhs", scope: !244, file: !1, line: 4, type: !117)
!248 = !DILocation(line: 5, column: 26, scope: !244)
!249 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2-=EjjL0L0", linkageName: "_CNW7builtin3opsEXi2-=EjjL0L0", scope: null, file: !6, line: 6, type: !123, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!250 = !DILocalVariable(name: "lhs", scope: !249, file: !1, line: 6, type: !117)
!251 = !DILocation(line: 6, column: 18, scope: !249)
!252 = !DILocalVariable(name: "rhs", scope: !249, file: !1, line: 6, type: !117)
!253 = !DILocation(line: 9, column: 30, scope: !249)
!254 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1-EllL0L0", linkageName: "_CNW7builtin3opsEXi1-EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!255 = !DILocalVariable(name: "lhs", scope: !254, file: !1, line: 4, type: !133)
!256 = !DILocation(line: 4, column: 19, scope: !254)
!257 = !DILocalVariable(name: "rhs", scope: !254, file: !1, line: 4, type: !133)
!258 = !DILocation(line: 5, column: 26, scope: !254)
!259 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2-=EllL0L0", linkageName: "_CNW7builtin3opsEXi2-=EllL0L0", scope: null, file: !6, line: 6, type: !139, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!260 = !DILocalVariable(name: "lhs", scope: !259, file: !1, line: 6, type: !133)
!261 = !DILocation(line: 6, column: 18, scope: !259)
!262 = !DILocalVariable(name: "rhs", scope: !259, file: !1, line: 6, type: !133)
!263 = !DILocation(line: 9, column: 30, scope: !259)
!264 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1-EmmL0L0", linkageName: "_CNW7builtin3opsEXi1-EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!265 = !DILocalVariable(name: "lhs", scope: !264, file: !1, line: 4, type: !149)
!266 = !DILocation(line: 4, column: 19, scope: !264)
!267 = !DILocalVariable(name: "rhs", scope: !264, file: !1, line: 4, type: !149)
!268 = !DILocation(line: 5, column: 26, scope: !264)
!269 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2-=EmmL0L0", linkageName: "_CNW7builtin3opsEXi2-=EmmL0L0", scope: null, file: !6, line: 6, type: !155, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!270 = !DILocalVariable(name: "lhs", scope: !269, file: !1, line: 6, type: !149)
!271 = !DILocation(line: 6, column: 18, scope: !269)
!272 = !DILocalVariable(name: "rhs", scope: !269, file: !1, line: 6, type: !149)
!273 = !DILocation(line: 9, column: 30, scope: !269)
!274 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1-EnnL0L0", linkageName: "_CNW7builtin3opsEXi1-EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!275 = !DILocalVariable(name: "lhs", scope: !274, file: !1, line: 4, type: !165)
!276 = !DILocation(line: 4, column: 19, scope: !274)
!277 = !DILocalVariable(name: "rhs", scope: !274, file: !1, line: 4, type: !165)
!278 = !DILocation(line: 5, column: 26, scope: !274)
!279 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2-=EnnL0L0", linkageName: "_CNW7builtin3opsEXi2-=EnnL0L0", scope: null, file: !6, line: 6, type: !171, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!280 = !DILocalVariable(name: "lhs", scope: !279, file: !1, line: 6, type: !165)
!281 = !DILocation(line: 6, column: 18, scope: !279)
!282 = !DILocalVariable(name: "rhs", scope: !279, file: !1, line: 6, type: !165)
!283 = !DILocation(line: 9, column: 30, scope: !279)
!284 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1-EooL0L0", linkageName: "_CNW7builtin3opsEXi1-EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!285 = !DILocalVariable(name: "lhs", scope: !284, file: !1, line: 4, type: !181)
!286 = !DILocation(line: 4, column: 19, scope: !284)
!287 = !DILocalVariable(name: "rhs", scope: !284, file: !1, line: 4, type: !181)
!288 = !DILocation(line: 5, column: 26, scope: !284)
!289 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2-=EooL0L0", linkageName: "_CNW7builtin3opsEXi2-=EooL0L0", scope: null, file: !6, line: 6, type: !187, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!290 = !DILocalVariable(name: "lhs", scope: !289, file: !1, line: 6, type: !181)
!291 = !DILocation(line: 6, column: 18, scope: !289)
!292 = !DILocalVariable(name: "rhs", scope: !289, file: !1, line: 6, type: !181)
!293 = !DILocation(line: 9, column: 30, scope: !289)
!294 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1*EaaL0L0", linkageName: "_CNW7builtin3opsEXi1*EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!295 = !DILocalVariable(name: "lhs", scope: !294, file: !1, line: 4, type: !37)
!296 = !DILocation(line: 4, column: 19, scope: !294)
!297 = !DILocalVariable(name: "rhs", scope: !294, file: !1, line: 4, type: !37)
!298 = !DILocation(line: 5, column: 26, scope: !294)
!299 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2*=EaaL0L0", linkageName: "_CNW7builtin3opsEXi2*=EaaL0L0", scope: null, file: !6, line: 6, type: !43, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!300 = !DILocalVariable(name: "lhs", scope: !299, file: !1, line: 6, type: !37)
!301 = !DILocation(line: 6, column: 18, scope: !299)
!302 = !DILocalVariable(name: "rhs", scope: !299, file: !1, line: 6, type: !37)
!303 = !DILocation(line: 9, column: 30, scope: !299)
!304 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1*EhhL0L0", linkageName: "_CNW7builtin3opsEXi1*EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!305 = !DILocalVariable(name: "lhs", scope: !304, file: !1, line: 4, type: !53)
!306 = !DILocation(line: 4, column: 19, scope: !304)
!307 = !DILocalVariable(name: "rhs", scope: !304, file: !1, line: 4, type: !53)
!308 = !DILocation(line: 5, column: 26, scope: !304)
!309 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2*=EhhL0L0", linkageName: "_CNW7builtin3opsEXi2*=EhhL0L0", scope: null, file: !6, line: 6, type: !59, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!310 = !DILocalVariable(name: "lhs", scope: !309, file: !1, line: 6, type: !53)
!311 = !DILocation(line: 6, column: 18, scope: !309)
!312 = !DILocalVariable(name: "rhs", scope: !309, file: !1, line: 6, type: !53)
!313 = !DILocation(line: 9, column: 30, scope: !309)
!314 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1*EssL0L0", linkageName: "_CNW7builtin3opsEXi1*EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!315 = !DILocalVariable(name: "lhs", scope: !314, file: !1, line: 4, type: !69)
!316 = !DILocation(line: 4, column: 19, scope: !314)
!317 = !DILocalVariable(name: "rhs", scope: !314, file: !1, line: 4, type: !69)
!318 = !DILocation(line: 5, column: 26, scope: !314)
!319 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2*=EssL0L0", linkageName: "_CNW7builtin3opsEXi2*=EssL0L0", scope: null, file: !6, line: 6, type: !75, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!320 = !DILocalVariable(name: "lhs", scope: !319, file: !1, line: 6, type: !69)
!321 = !DILocation(line: 6, column: 18, scope: !319)
!322 = !DILocalVariable(name: "rhs", scope: !319, file: !1, line: 6, type: !69)
!323 = !DILocation(line: 9, column: 30, scope: !319)
!324 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1*EttL0L0", linkageName: "_CNW7builtin3opsEXi1*EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!325 = !DILocalVariable(name: "lhs", scope: !324, file: !1, line: 4, type: !85)
!326 = !DILocation(line: 4, column: 19, scope: !324)
!327 = !DILocalVariable(name: "rhs", scope: !324, file: !1, line: 4, type: !85)
!328 = !DILocation(line: 5, column: 26, scope: !324)
!329 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2*=EttL0L0", linkageName: "_CNW7builtin3opsEXi2*=EttL0L0", scope: null, file: !6, line: 6, type: !91, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!330 = !DILocalVariable(name: "lhs", scope: !329, file: !1, line: 6, type: !85)
!331 = !DILocation(line: 6, column: 18, scope: !329)
!332 = !DILocalVariable(name: "rhs", scope: !329, file: !1, line: 6, type: !85)
!333 = !DILocation(line: 9, column: 30, scope: !329)
!334 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1*EiiL0L0", linkageName: "_CNW7builtin3opsEXi1*EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!335 = !DILocalVariable(name: "lhs", scope: !334, file: !1, line: 4, type: !101)
!336 = !DILocation(line: 4, column: 19, scope: !334)
!337 = !DILocalVariable(name: "rhs", scope: !334, file: !1, line: 4, type: !101)
!338 = !DILocation(line: 5, column: 26, scope: !334)
!339 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2*=EiiL0L0", linkageName: "_CNW7builtin3opsEXi2*=EiiL0L0", scope: null, file: !6, line: 6, type: !107, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!340 = !DILocalVariable(name: "lhs", scope: !339, file: !1, line: 6, type: !101)
!341 = !DILocation(line: 6, column: 18, scope: !339)
!342 = !DILocalVariable(name: "rhs", scope: !339, file: !1, line: 6, type: !101)
!343 = !DILocation(line: 9, column: 30, scope: !339)
!344 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1*EjjL0L0", linkageName: "_CNW7builtin3opsEXi1*EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!345 = !DILocalVariable(name: "lhs", scope: !344, file: !1, line: 4, type: !117)
!346 = !DILocation(line: 4, column: 19, scope: !344)
!347 = !DILocalVariable(name: "rhs", scope: !344, file: !1, line: 4, type: !117)
!348 = !DILocation(line: 5, column: 26, scope: !344)
!349 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2*=EjjL0L0", linkageName: "_CNW7builtin3opsEXi2*=EjjL0L0", scope: null, file: !6, line: 6, type: !123, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!350 = !DILocalVariable(name: "lhs", scope: !349, file: !1, line: 6, type: !117)
!351 = !DILocation(line: 6, column: 18, scope: !349)
!352 = !DILocalVariable(name: "rhs", scope: !349, file: !1, line: 6, type: !117)
!353 = !DILocation(line: 9, column: 30, scope: !349)
!354 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1*EllL0L0", linkageName: "_CNW7builtin3opsEXi1*EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!355 = !DILocalVariable(name: "lhs", scope: !354, file: !1, line: 4, type: !133)
!356 = !DILocation(line: 4, column: 19, scope: !354)
!357 = !DILocalVariable(name: "rhs", scope: !354, file: !1, line: 4, type: !133)
!358 = !DILocation(line: 5, column: 26, scope: !354)
!359 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2*=EllL0L0", linkageName: "_CNW7builtin3opsEXi2*=EllL0L0", scope: null, file: !6, line: 6, type: !139, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!360 = !DILocalVariable(name: "lhs", scope: !359, file: !1, line: 6, type: !133)
!361 = !DILocation(line: 6, column: 18, scope: !359)
!362 = !DILocalVariable(name: "rhs", scope: !359, file: !1, line: 6, type: !133)
!363 = !DILocation(line: 9, column: 30, scope: !359)
!364 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1*EmmL0L0", linkageName: "_CNW7builtin3opsEXi1*EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!365 = !DILocalVariable(name: "lhs", scope: !364, file: !1, line: 4, type: !149)
!366 = !DILocation(line: 4, column: 19, scope: !364)
!367 = !DILocalVariable(name: "rhs", scope: !364, file: !1, line: 4, type: !149)
!368 = !DILocation(line: 5, column: 26, scope: !364)
!369 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2*=EmmL0L0", linkageName: "_CNW7builtin3opsEXi2*=EmmL0L0", scope: null, file: !6, line: 6, type: !155, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!370 = !DILocalVariable(name: "lhs", scope: !369, file: !1, line: 6, type: !149)
!371 = !DILocation(line: 6, column: 18, scope: !369)
!372 = !DILocalVariable(name: "rhs", scope: !369, file: !1, line: 6, type: !149)
!373 = !DILocation(line: 9, column: 30, scope: !369)
!374 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1*EnnL0L0", linkageName: "_CNW7builtin3opsEXi1*EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!375 = !DILocalVariable(name: "lhs", scope: !374, file: !1, line: 4, type: !165)
!376 = !DILocation(line: 4, column: 19, scope: !374)
!377 = !DILocalVariable(name: "rhs", scope: !374, file: !1, line: 4, type: !165)
!378 = !DILocation(line: 5, column: 26, scope: !374)
!379 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2*=EnnL0L0", linkageName: "_CNW7builtin3opsEXi2*=EnnL0L0", scope: null, file: !6, line: 6, type: !171, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!380 = !DILocalVariable(name: "lhs", scope: !379, file: !1, line: 6, type: !165)
!381 = !DILocation(line: 6, column: 18, scope: !379)
!382 = !DILocalVariable(name: "rhs", scope: !379, file: !1, line: 6, type: !165)
!383 = !DILocation(line: 9, column: 30, scope: !379)
!384 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1*EooL0L0", linkageName: "_CNW7builtin3opsEXi1*EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!385 = !DILocalVariable(name: "lhs", scope: !384, file: !1, line: 4, type: !181)
!386 = !DILocation(line: 4, column: 19, scope: !384)
!387 = !DILocalVariable(name: "rhs", scope: !384, file: !1, line: 4, type: !181)
!388 = !DILocation(line: 5, column: 26, scope: !384)
!389 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2*=EooL0L0", linkageName: "_CNW7builtin3opsEXi2*=EooL0L0", scope: null, file: !6, line: 6, type: !187, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!390 = !DILocalVariable(name: "lhs", scope: !389, file: !1, line: 6, type: !181)
!391 = !DILocation(line: 6, column: 18, scope: !389)
!392 = !DILocalVariable(name: "rhs", scope: !389, file: !1, line: 6, type: !181)
!393 = !DILocation(line: 9, column: 30, scope: !389)
!394 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1/EaaL0L0", linkageName: "_CNW7builtin3opsEXi1/EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!395 = !DILocalVariable(name: "lhs", scope: !394, file: !1, line: 4, type: !37)
!396 = !DILocation(line: 4, column: 19, scope: !394)
!397 = !DILocalVariable(name: "rhs", scope: !394, file: !1, line: 4, type: !37)
!398 = !DILocation(line: 5, column: 26, scope: !394)
!399 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2/=EaaL0L0", linkageName: "_CNW7builtin3opsEXi2/=EaaL0L0", scope: null, file: !6, line: 6, type: !43, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!400 = !DILocalVariable(name: "lhs", scope: !399, file: !1, line: 6, type: !37)
!401 = !DILocation(line: 6, column: 18, scope: !399)
!402 = !DILocalVariable(name: "rhs", scope: !399, file: !1, line: 6, type: !37)
!403 = !DILocation(line: 9, column: 30, scope: !399)
!404 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1/EhhL0L0", linkageName: "_CNW7builtin3opsEXi1/EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!405 = !DILocalVariable(name: "lhs", scope: !404, file: !1, line: 4, type: !53)
!406 = !DILocation(line: 4, column: 19, scope: !404)
!407 = !DILocalVariable(name: "rhs", scope: !404, file: !1, line: 4, type: !53)
!408 = !DILocation(line: 5, column: 26, scope: !404)
!409 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2/=EhhL0L0", linkageName: "_CNW7builtin3opsEXi2/=EhhL0L0", scope: null, file: !6, line: 6, type: !59, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!410 = !DILocalVariable(name: "lhs", scope: !409, file: !1, line: 6, type: !53)
!411 = !DILocation(line: 6, column: 18, scope: !409)
!412 = !DILocalVariable(name: "rhs", scope: !409, file: !1, line: 6, type: !53)
!413 = !DILocation(line: 9, column: 30, scope: !409)
!414 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1/EssL0L0", linkageName: "_CNW7builtin3opsEXi1/EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!415 = !DILocalVariable(name: "lhs", scope: !414, file: !1, line: 4, type: !69)
!416 = !DILocation(line: 4, column: 19, scope: !414)
!417 = !DILocalVariable(name: "rhs", scope: !414, file: !1, line: 4, type: !69)
!418 = !DILocation(line: 5, column: 26, scope: !414)
!419 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2/=EssL0L0", linkageName: "_CNW7builtin3opsEXi2/=EssL0L0", scope: null, file: !6, line: 6, type: !75, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!420 = !DILocalVariable(name: "lhs", scope: !419, file: !1, line: 6, type: !69)
!421 = !DILocation(line: 6, column: 18, scope: !419)
!422 = !DILocalVariable(name: "rhs", scope: !419, file: !1, line: 6, type: !69)
!423 = !DILocation(line: 9, column: 30, scope: !419)
!424 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1/EttL0L0", linkageName: "_CNW7builtin3opsEXi1/EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!425 = !DILocalVariable(name: "lhs", scope: !424, file: !1, line: 4, type: !85)
!426 = !DILocation(line: 4, column: 19, scope: !424)
!427 = !DILocalVariable(name: "rhs", scope: !424, file: !1, line: 4, type: !85)
!428 = !DILocation(line: 5, column: 26, scope: !424)
!429 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2/=EttL0L0", linkageName: "_CNW7builtin3opsEXi2/=EttL0L0", scope: null, file: !6, line: 6, type: !91, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!430 = !DILocalVariable(name: "lhs", scope: !429, file: !1, line: 6, type: !85)
!431 = !DILocation(line: 6, column: 18, scope: !429)
!432 = !DILocalVariable(name: "rhs", scope: !429, file: !1, line: 6, type: !85)
!433 = !DILocation(line: 9, column: 30, scope: !429)
!434 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1/EiiL0L0", linkageName: "_CNW7builtin3opsEXi1/EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!435 = !DILocalVariable(name: "lhs", scope: !434, file: !1, line: 4, type: !101)
!436 = !DILocation(line: 4, column: 19, scope: !434)
!437 = !DILocalVariable(name: "rhs", scope: !434, file: !1, line: 4, type: !101)
!438 = !DILocation(line: 5, column: 26, scope: !434)
!439 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2/=EiiL0L0", linkageName: "_CNW7builtin3opsEXi2/=EiiL0L0", scope: null, file: !6, line: 6, type: !107, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!440 = !DILocalVariable(name: "lhs", scope: !439, file: !1, line: 6, type: !101)
!441 = !DILocation(line: 6, column: 18, scope: !439)
!442 = !DILocalVariable(name: "rhs", scope: !439, file: !1, line: 6, type: !101)
!443 = !DILocation(line: 9, column: 30, scope: !439)
!444 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1/EjjL0L0", linkageName: "_CNW7builtin3opsEXi1/EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!445 = !DILocalVariable(name: "lhs", scope: !444, file: !1, line: 4, type: !117)
!446 = !DILocation(line: 4, column: 19, scope: !444)
!447 = !DILocalVariable(name: "rhs", scope: !444, file: !1, line: 4, type: !117)
!448 = !DILocation(line: 5, column: 26, scope: !444)
!449 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2/=EjjL0L0", linkageName: "_CNW7builtin3opsEXi2/=EjjL0L0", scope: null, file: !6, line: 6, type: !123, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!450 = !DILocalVariable(name: "lhs", scope: !449, file: !1, line: 6, type: !117)
!451 = !DILocation(line: 6, column: 18, scope: !449)
!452 = !DILocalVariable(name: "rhs", scope: !449, file: !1, line: 6, type: !117)
!453 = !DILocation(line: 9, column: 30, scope: !449)
!454 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1/EllL0L0", linkageName: "_CNW7builtin3opsEXi1/EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!455 = !DILocalVariable(name: "lhs", scope: !454, file: !1, line: 4, type: !133)
!456 = !DILocation(line: 4, column: 19, scope: !454)
!457 = !DILocalVariable(name: "rhs", scope: !454, file: !1, line: 4, type: !133)
!458 = !DILocation(line: 5, column: 26, scope: !454)
!459 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2/=EllL0L0", linkageName: "_CNW7builtin3opsEXi2/=EllL0L0", scope: null, file: !6, line: 6, type: !139, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!460 = !DILocalVariable(name: "lhs", scope: !459, file: !1, line: 6, type: !133)
!461 = !DILocation(line: 6, column: 18, scope: !459)
!462 = !DILocalVariable(name: "rhs", scope: !459, file: !1, line: 6, type: !133)
!463 = !DILocation(line: 9, column: 30, scope: !459)
!464 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1/EmmL0L0", linkageName: "_CNW7builtin3opsEXi1/EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!465 = !DILocalVariable(name: "lhs", scope: !464, file: !1, line: 4, type: !149)
!466 = !DILocation(line: 4, column: 19, scope: !464)
!467 = !DILocalVariable(name: "rhs", scope: !464, file: !1, line: 4, type: !149)
!468 = !DILocation(line: 5, column: 26, scope: !464)
!469 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2/=EmmL0L0", linkageName: "_CNW7builtin3opsEXi2/=EmmL0L0", scope: null, file: !6, line: 6, type: !155, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!470 = !DILocalVariable(name: "lhs", scope: !469, file: !1, line: 6, type: !149)
!471 = !DILocation(line: 6, column: 18, scope: !469)
!472 = !DILocalVariable(name: "rhs", scope: !469, file: !1, line: 6, type: !149)
!473 = !DILocation(line: 9, column: 30, scope: !469)
!474 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1/EnnL0L0", linkageName: "_CNW7builtin3opsEXi1/EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!475 = !DILocalVariable(name: "lhs", scope: !474, file: !1, line: 4, type: !165)
!476 = !DILocation(line: 4, column: 19, scope: !474)
!477 = !DILocalVariable(name: "rhs", scope: !474, file: !1, line: 4, type: !165)
!478 = !DILocation(line: 5, column: 26, scope: !474)
!479 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2/=EnnL0L0", linkageName: "_CNW7builtin3opsEXi2/=EnnL0L0", scope: null, file: !6, line: 6, type: !171, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!480 = !DILocalVariable(name: "lhs", scope: !479, file: !1, line: 6, type: !165)
!481 = !DILocation(line: 6, column: 18, scope: !479)
!482 = !DILocalVariable(name: "rhs", scope: !479, file: !1, line: 6, type: !165)
!483 = !DILocation(line: 9, column: 30, scope: !479)
!484 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1/EooL0L0", linkageName: "_CNW7builtin3opsEXi1/EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!485 = !DILocalVariable(name: "lhs", scope: !484, file: !1, line: 4, type: !181)
!486 = !DILocation(line: 4, column: 19, scope: !484)
!487 = !DILocalVariable(name: "rhs", scope: !484, file: !1, line: 4, type: !181)
!488 = !DILocation(line: 5, column: 26, scope: !484)
!489 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2/=EooL0L0", linkageName: "_CNW7builtin3opsEXi2/=EooL0L0", scope: null, file: !6, line: 6, type: !187, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!490 = !DILocalVariable(name: "lhs", scope: !489, file: !1, line: 6, type: !181)
!491 = !DILocation(line: 6, column: 18, scope: !489)
!492 = !DILocalVariable(name: "rhs", scope: !489, file: !1, line: 6, type: !181)
!493 = !DILocation(line: 9, column: 30, scope: !489)
!494 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1%EaaL0L0", linkageName: "_CNW7builtin3opsEXi1%EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!495 = !DILocalVariable(name: "lhs", scope: !494, file: !1, line: 4, type: !37)
!496 = !DILocation(line: 4, column: 19, scope: !494)
!497 = !DILocalVariable(name: "rhs", scope: !494, file: !1, line: 4, type: !37)
!498 = !DILocation(line: 5, column: 26, scope: !494)
!499 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2%=EaaL0L0", linkageName: "_CNW7builtin3opsEXi2%=EaaL0L0", scope: null, file: !6, line: 6, type: !43, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!500 = !DILocalVariable(name: "lhs", scope: !499, file: !1, line: 6, type: !37)
!501 = !DILocation(line: 6, column: 18, scope: !499)
!502 = !DILocalVariable(name: "rhs", scope: !499, file: !1, line: 6, type: !37)
!503 = !DILocation(line: 9, column: 30, scope: !499)
!504 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1%EhhL0L0", linkageName: "_CNW7builtin3opsEXi1%EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!505 = !DILocalVariable(name: "lhs", scope: !504, file: !1, line: 4, type: !53)
!506 = !DILocation(line: 4, column: 19, scope: !504)
!507 = !DILocalVariable(name: "rhs", scope: !504, file: !1, line: 4, type: !53)
!508 = !DILocation(line: 5, column: 26, scope: !504)
!509 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2%=EhhL0L0", linkageName: "_CNW7builtin3opsEXi2%=EhhL0L0", scope: null, file: !6, line: 6, type: !59, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!510 = !DILocalVariable(name: "lhs", scope: !509, file: !1, line: 6, type: !53)
!511 = !DILocation(line: 6, column: 18, scope: !509)
!512 = !DILocalVariable(name: "rhs", scope: !509, file: !1, line: 6, type: !53)
!513 = !DILocation(line: 9, column: 30, scope: !509)
!514 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1%EssL0L0", linkageName: "_CNW7builtin3opsEXi1%EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!515 = !DILocalVariable(name: "lhs", scope: !514, file: !1, line: 4, type: !69)
!516 = !DILocation(line: 4, column: 19, scope: !514)
!517 = !DILocalVariable(name: "rhs", scope: !514, file: !1, line: 4, type: !69)
!518 = !DILocation(line: 5, column: 26, scope: !514)
!519 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2%=EssL0L0", linkageName: "_CNW7builtin3opsEXi2%=EssL0L0", scope: null, file: !6, line: 6, type: !75, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!520 = !DILocalVariable(name: "lhs", scope: !519, file: !1, line: 6, type: !69)
!521 = !DILocation(line: 6, column: 18, scope: !519)
!522 = !DILocalVariable(name: "rhs", scope: !519, file: !1, line: 6, type: !69)
!523 = !DILocation(line: 9, column: 30, scope: !519)
!524 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1%EttL0L0", linkageName: "_CNW7builtin3opsEXi1%EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!525 = !DILocalVariable(name: "lhs", scope: !524, file: !1, line: 4, type: !85)
!526 = !DILocation(line: 4, column: 19, scope: !524)
!527 = !DILocalVariable(name: "rhs", scope: !524, file: !1, line: 4, type: !85)
!528 = !DILocation(line: 5, column: 26, scope: !524)
!529 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2%=EttL0L0", linkageName: "_CNW7builtin3opsEXi2%=EttL0L0", scope: null, file: !6, line: 6, type: !91, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!530 = !DILocalVariable(name: "lhs", scope: !529, file: !1, line: 6, type: !85)
!531 = !DILocation(line: 6, column: 18, scope: !529)
!532 = !DILocalVariable(name: "rhs", scope: !529, file: !1, line: 6, type: !85)
!533 = !DILocation(line: 9, column: 30, scope: !529)
!534 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1%EiiL0L0", linkageName: "_CNW7builtin3opsEXi1%EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!535 = !DILocalVariable(name: "lhs", scope: !534, file: !1, line: 4, type: !101)
!536 = !DILocation(line: 4, column: 19, scope: !534)
!537 = !DILocalVariable(name: "rhs", scope: !534, file: !1, line: 4, type: !101)
!538 = !DILocation(line: 5, column: 26, scope: !534)
!539 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2%=EiiL0L0", linkageName: "_CNW7builtin3opsEXi2%=EiiL0L0", scope: null, file: !6, line: 6, type: !107, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!540 = !DILocalVariable(name: "lhs", scope: !539, file: !1, line: 6, type: !101)
!541 = !DILocation(line: 6, column: 18, scope: !539)
!542 = !DILocalVariable(name: "rhs", scope: !539, file: !1, line: 6, type: !101)
!543 = !DILocation(line: 9, column: 30, scope: !539)
!544 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1%EjjL0L0", linkageName: "_CNW7builtin3opsEXi1%EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!545 = !DILocalVariable(name: "lhs", scope: !544, file: !1, line: 4, type: !117)
!546 = !DILocation(line: 4, column: 19, scope: !544)
!547 = !DILocalVariable(name: "rhs", scope: !544, file: !1, line: 4, type: !117)
!548 = !DILocation(line: 5, column: 26, scope: !544)
!549 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2%=EjjL0L0", linkageName: "_CNW7builtin3opsEXi2%=EjjL0L0", scope: null, file: !6, line: 6, type: !123, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!550 = !DILocalVariable(name: "lhs", scope: !549, file: !1, line: 6, type: !117)
!551 = !DILocation(line: 6, column: 18, scope: !549)
!552 = !DILocalVariable(name: "rhs", scope: !549, file: !1, line: 6, type: !117)
!553 = !DILocation(line: 9, column: 30, scope: !549)
!554 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1%EllL0L0", linkageName: "_CNW7builtin3opsEXi1%EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!555 = !DILocalVariable(name: "lhs", scope: !554, file: !1, line: 4, type: !133)
!556 = !DILocation(line: 4, column: 19, scope: !554)
!557 = !DILocalVariable(name: "rhs", scope: !554, file: !1, line: 4, type: !133)
!558 = !DILocation(line: 5, column: 26, scope: !554)
!559 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2%=EllL0L0", linkageName: "_CNW7builtin3opsEXi2%=EllL0L0", scope: null, file: !6, line: 6, type: !139, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!560 = !DILocalVariable(name: "lhs", scope: !559, file: !1, line: 6, type: !133)
!561 = !DILocation(line: 6, column: 18, scope: !559)
!562 = !DILocalVariable(name: "rhs", scope: !559, file: !1, line: 6, type: !133)
!563 = !DILocation(line: 9, column: 30, scope: !559)
!564 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1%EmmL0L0", linkageName: "_CNW7builtin3opsEXi1%EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!565 = !DILocalVariable(name: "lhs", scope: !564, file: !1, line: 4, type: !149)
!566 = !DILocation(line: 4, column: 19, scope: !564)
!567 = !DILocalVariable(name: "rhs", scope: !564, file: !1, line: 4, type: !149)
!568 = !DILocation(line: 5, column: 26, scope: !564)
!569 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2%=EmmL0L0", linkageName: "_CNW7builtin3opsEXi2%=EmmL0L0", scope: null, file: !6, line: 6, type: !155, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!570 = !DILocalVariable(name: "lhs", scope: !569, file: !1, line: 6, type: !149)
!571 = !DILocation(line: 6, column: 18, scope: !569)
!572 = !DILocalVariable(name: "rhs", scope: !569, file: !1, line: 6, type: !149)
!573 = !DILocation(line: 9, column: 30, scope: !569)
!574 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1%EnnL0L0", linkageName: "_CNW7builtin3opsEXi1%EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!575 = !DILocalVariable(name: "lhs", scope: !574, file: !1, line: 4, type: !165)
!576 = !DILocation(line: 4, column: 19, scope: !574)
!577 = !DILocalVariable(name: "rhs", scope: !574, file: !1, line: 4, type: !165)
!578 = !DILocation(line: 5, column: 26, scope: !574)
!579 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2%=EnnL0L0", linkageName: "_CNW7builtin3opsEXi2%=EnnL0L0", scope: null, file: !6, line: 6, type: !171, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!580 = !DILocalVariable(name: "lhs", scope: !579, file: !1, line: 6, type: !165)
!581 = !DILocation(line: 6, column: 18, scope: !579)
!582 = !DILocalVariable(name: "rhs", scope: !579, file: !1, line: 6, type: !165)
!583 = !DILocation(line: 9, column: 30, scope: !579)
!584 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1%EooL0L0", linkageName: "_CNW7builtin3opsEXi1%EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!585 = !DILocalVariable(name: "lhs", scope: !584, file: !1, line: 4, type: !181)
!586 = !DILocation(line: 4, column: 19, scope: !584)
!587 = !DILocalVariable(name: "rhs", scope: !584, file: !1, line: 4, type: !181)
!588 = !DILocation(line: 5, column: 26, scope: !584)
!589 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2%=EooL0L0", linkageName: "_CNW7builtin3opsEXi2%=EooL0L0", scope: null, file: !6, line: 6, type: !187, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!590 = !DILocalVariable(name: "lhs", scope: !589, file: !1, line: 6, type: !181)
!591 = !DILocation(line: 6, column: 18, scope: !589)
!592 = !DILocalVariable(name: "rhs", scope: !589, file: !1, line: 6, type: !181)
!593 = !DILocation(line: 9, column: 30, scope: !589)
!594 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<<EaaL0L0", linkageName: "_CNW7builtin3opsEXi2<<EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!595 = !DILocalVariable(name: "lhs", scope: !594, file: !1, line: 4, type: !37)
!596 = !DILocation(line: 4, column: 19, scope: !594)
!597 = !DILocalVariable(name: "rhs", scope: !594, file: !1, line: 4, type: !37)
!598 = !DILocation(line: 5, column: 26, scope: !594)
!599 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3<<=EaaL0L0", linkageName: "_CNW7builtin3opsEXi3<<=EaaL0L0", scope: null, file: !6, line: 6, type: !43, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!600 = !DILocalVariable(name: "lhs", scope: !599, file: !1, line: 6, type: !37)
!601 = !DILocation(line: 6, column: 18, scope: !599)
!602 = !DILocalVariable(name: "rhs", scope: !599, file: !1, line: 6, type: !37)
!603 = !DILocation(line: 9, column: 30, scope: !599)
!604 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<<EhhL0L0", linkageName: "_CNW7builtin3opsEXi2<<EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!605 = !DILocalVariable(name: "lhs", scope: !604, file: !1, line: 4, type: !53)
!606 = !DILocation(line: 4, column: 19, scope: !604)
!607 = !DILocalVariable(name: "rhs", scope: !604, file: !1, line: 4, type: !53)
!608 = !DILocation(line: 5, column: 26, scope: !604)
!609 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3<<=EhhL0L0", linkageName: "_CNW7builtin3opsEXi3<<=EhhL0L0", scope: null, file: !6, line: 6, type: !59, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!610 = !DILocalVariable(name: "lhs", scope: !609, file: !1, line: 6, type: !53)
!611 = !DILocation(line: 6, column: 18, scope: !609)
!612 = !DILocalVariable(name: "rhs", scope: !609, file: !1, line: 6, type: !53)
!613 = !DILocation(line: 9, column: 30, scope: !609)
!614 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<<EssL0L0", linkageName: "_CNW7builtin3opsEXi2<<EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!615 = !DILocalVariable(name: "lhs", scope: !614, file: !1, line: 4, type: !69)
!616 = !DILocation(line: 4, column: 19, scope: !614)
!617 = !DILocalVariable(name: "rhs", scope: !614, file: !1, line: 4, type: !69)
!618 = !DILocation(line: 5, column: 26, scope: !614)
!619 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3<<=EssL0L0", linkageName: "_CNW7builtin3opsEXi3<<=EssL0L0", scope: null, file: !6, line: 6, type: !75, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!620 = !DILocalVariable(name: "lhs", scope: !619, file: !1, line: 6, type: !69)
!621 = !DILocation(line: 6, column: 18, scope: !619)
!622 = !DILocalVariable(name: "rhs", scope: !619, file: !1, line: 6, type: !69)
!623 = !DILocation(line: 9, column: 30, scope: !619)
!624 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<<EttL0L0", linkageName: "_CNW7builtin3opsEXi2<<EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!625 = !DILocalVariable(name: "lhs", scope: !624, file: !1, line: 4, type: !85)
!626 = !DILocation(line: 4, column: 19, scope: !624)
!627 = !DILocalVariable(name: "rhs", scope: !624, file: !1, line: 4, type: !85)
!628 = !DILocation(line: 5, column: 26, scope: !624)
!629 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3<<=EttL0L0", linkageName: "_CNW7builtin3opsEXi3<<=EttL0L0", scope: null, file: !6, line: 6, type: !91, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!630 = !DILocalVariable(name: "lhs", scope: !629, file: !1, line: 6, type: !85)
!631 = !DILocation(line: 6, column: 18, scope: !629)
!632 = !DILocalVariable(name: "rhs", scope: !629, file: !1, line: 6, type: !85)
!633 = !DILocation(line: 9, column: 30, scope: !629)
!634 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<<EiiL0L0", linkageName: "_CNW7builtin3opsEXi2<<EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!635 = !DILocalVariable(name: "lhs", scope: !634, file: !1, line: 4, type: !101)
!636 = !DILocation(line: 4, column: 19, scope: !634)
!637 = !DILocalVariable(name: "rhs", scope: !634, file: !1, line: 4, type: !101)
!638 = !DILocation(line: 5, column: 26, scope: !634)
!639 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3<<=EiiL0L0", linkageName: "_CNW7builtin3opsEXi3<<=EiiL0L0", scope: null, file: !6, line: 6, type: !107, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!640 = !DILocalVariable(name: "lhs", scope: !639, file: !1, line: 6, type: !101)
!641 = !DILocation(line: 6, column: 18, scope: !639)
!642 = !DILocalVariable(name: "rhs", scope: !639, file: !1, line: 6, type: !101)
!643 = !DILocation(line: 9, column: 30, scope: !639)
!644 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<<EjjL0L0", linkageName: "_CNW7builtin3opsEXi2<<EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!645 = !DILocalVariable(name: "lhs", scope: !644, file: !1, line: 4, type: !117)
!646 = !DILocation(line: 4, column: 19, scope: !644)
!647 = !DILocalVariable(name: "rhs", scope: !644, file: !1, line: 4, type: !117)
!648 = !DILocation(line: 5, column: 26, scope: !644)
!649 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3<<=EjjL0L0", linkageName: "_CNW7builtin3opsEXi3<<=EjjL0L0", scope: null, file: !6, line: 6, type: !123, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!650 = !DILocalVariable(name: "lhs", scope: !649, file: !1, line: 6, type: !117)
!651 = !DILocation(line: 6, column: 18, scope: !649)
!652 = !DILocalVariable(name: "rhs", scope: !649, file: !1, line: 6, type: !117)
!653 = !DILocation(line: 9, column: 30, scope: !649)
!654 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<<EllL0L0", linkageName: "_CNW7builtin3opsEXi2<<EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!655 = !DILocalVariable(name: "lhs", scope: !654, file: !1, line: 4, type: !133)
!656 = !DILocation(line: 4, column: 19, scope: !654)
!657 = !DILocalVariable(name: "rhs", scope: !654, file: !1, line: 4, type: !133)
!658 = !DILocation(line: 5, column: 26, scope: !654)
!659 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3<<=EllL0L0", linkageName: "_CNW7builtin3opsEXi3<<=EllL0L0", scope: null, file: !6, line: 6, type: !139, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!660 = !DILocalVariable(name: "lhs", scope: !659, file: !1, line: 6, type: !133)
!661 = !DILocation(line: 6, column: 18, scope: !659)
!662 = !DILocalVariable(name: "rhs", scope: !659, file: !1, line: 6, type: !133)
!663 = !DILocation(line: 9, column: 30, scope: !659)
!664 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<<EmmL0L0", linkageName: "_CNW7builtin3opsEXi2<<EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!665 = !DILocalVariable(name: "lhs", scope: !664, file: !1, line: 4, type: !149)
!666 = !DILocation(line: 4, column: 19, scope: !664)
!667 = !DILocalVariable(name: "rhs", scope: !664, file: !1, line: 4, type: !149)
!668 = !DILocation(line: 5, column: 26, scope: !664)
!669 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3<<=EmmL0L0", linkageName: "_CNW7builtin3opsEXi3<<=EmmL0L0", scope: null, file: !6, line: 6, type: !155, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!670 = !DILocalVariable(name: "lhs", scope: !669, file: !1, line: 6, type: !149)
!671 = !DILocation(line: 6, column: 18, scope: !669)
!672 = !DILocalVariable(name: "rhs", scope: !669, file: !1, line: 6, type: !149)
!673 = !DILocation(line: 9, column: 30, scope: !669)
!674 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<<EnnL0L0", linkageName: "_CNW7builtin3opsEXi2<<EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!675 = !DILocalVariable(name: "lhs", scope: !674, file: !1, line: 4, type: !165)
!676 = !DILocation(line: 4, column: 19, scope: !674)
!677 = !DILocalVariable(name: "rhs", scope: !674, file: !1, line: 4, type: !165)
!678 = !DILocation(line: 5, column: 26, scope: !674)
!679 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3<<=EnnL0L0", linkageName: "_CNW7builtin3opsEXi3<<=EnnL0L0", scope: null, file: !6, line: 6, type: !171, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!680 = !DILocalVariable(name: "lhs", scope: !679, file: !1, line: 6, type: !165)
!681 = !DILocation(line: 6, column: 18, scope: !679)
!682 = !DILocalVariable(name: "rhs", scope: !679, file: !1, line: 6, type: !165)
!683 = !DILocation(line: 9, column: 30, scope: !679)
!684 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<<EooL0L0", linkageName: "_CNW7builtin3opsEXi2<<EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!685 = !DILocalVariable(name: "lhs", scope: !684, file: !1, line: 4, type: !181)
!686 = !DILocation(line: 4, column: 19, scope: !684)
!687 = !DILocalVariable(name: "rhs", scope: !684, file: !1, line: 4, type: !181)
!688 = !DILocation(line: 5, column: 26, scope: !684)
!689 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3<<=EooL0L0", linkageName: "_CNW7builtin3opsEXi3<<=EooL0L0", scope: null, file: !6, line: 6, type: !187, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!690 = !DILocalVariable(name: "lhs", scope: !689, file: !1, line: 6, type: !181)
!691 = !DILocation(line: 6, column: 18, scope: !689)
!692 = !DILocalVariable(name: "rhs", scope: !689, file: !1, line: 6, type: !181)
!693 = !DILocation(line: 9, column: 30, scope: !689)
!694 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>>EaaL0L0", linkageName: "_CNW7builtin3opsEXi2>>EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!695 = !DILocalVariable(name: "lhs", scope: !694, file: !1, line: 4, type: !37)
!696 = !DILocation(line: 4, column: 19, scope: !694)
!697 = !DILocalVariable(name: "rhs", scope: !694, file: !1, line: 4, type: !37)
!698 = !DILocation(line: 5, column: 26, scope: !694)
!699 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3>>=EaaL0L0", linkageName: "_CNW7builtin3opsEXi3>>=EaaL0L0", scope: null, file: !6, line: 6, type: !43, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!700 = !DILocalVariable(name: "lhs", scope: !699, file: !1, line: 6, type: !37)
!701 = !DILocation(line: 6, column: 18, scope: !699)
!702 = !DILocalVariable(name: "rhs", scope: !699, file: !1, line: 6, type: !37)
!703 = !DILocation(line: 9, column: 30, scope: !699)
!704 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>>EssL0L0", linkageName: "_CNW7builtin3opsEXi2>>EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!705 = !DILocalVariable(name: "lhs", scope: !704, file: !1, line: 4, type: !69)
!706 = !DILocation(line: 4, column: 19, scope: !704)
!707 = !DILocalVariable(name: "rhs", scope: !704, file: !1, line: 4, type: !69)
!708 = !DILocation(line: 5, column: 26, scope: !704)
!709 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3>>=EssL0L0", linkageName: "_CNW7builtin3opsEXi3>>=EssL0L0", scope: null, file: !6, line: 6, type: !75, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!710 = !DILocalVariable(name: "lhs", scope: !709, file: !1, line: 6, type: !69)
!711 = !DILocation(line: 6, column: 18, scope: !709)
!712 = !DILocalVariable(name: "rhs", scope: !709, file: !1, line: 6, type: !69)
!713 = !DILocation(line: 9, column: 30, scope: !709)
!714 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>>EiiL0L0", linkageName: "_CNW7builtin3opsEXi2>>EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!715 = !DILocalVariable(name: "lhs", scope: !714, file: !1, line: 4, type: !101)
!716 = !DILocation(line: 4, column: 19, scope: !714)
!717 = !DILocalVariable(name: "rhs", scope: !714, file: !1, line: 4, type: !101)
!718 = !DILocation(line: 5, column: 26, scope: !714)
!719 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3>>=EiiL0L0", linkageName: "_CNW7builtin3opsEXi3>>=EiiL0L0", scope: null, file: !6, line: 6, type: !107, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!720 = !DILocalVariable(name: "lhs", scope: !719, file: !1, line: 6, type: !101)
!721 = !DILocation(line: 6, column: 18, scope: !719)
!722 = !DILocalVariable(name: "rhs", scope: !719, file: !1, line: 6, type: !101)
!723 = !DILocation(line: 9, column: 30, scope: !719)
!724 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>>EllL0L0", linkageName: "_CNW7builtin3opsEXi2>>EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!725 = !DILocalVariable(name: "lhs", scope: !724, file: !1, line: 4, type: !133)
!726 = !DILocation(line: 4, column: 19, scope: !724)
!727 = !DILocalVariable(name: "rhs", scope: !724, file: !1, line: 4, type: !133)
!728 = !DILocation(line: 5, column: 26, scope: !724)
!729 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3>>=EllL0L0", linkageName: "_CNW7builtin3opsEXi3>>=EllL0L0", scope: null, file: !6, line: 6, type: !139, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!730 = !DILocalVariable(name: "lhs", scope: !729, file: !1, line: 6, type: !133)
!731 = !DILocation(line: 6, column: 18, scope: !729)
!732 = !DILocalVariable(name: "rhs", scope: !729, file: !1, line: 6, type: !133)
!733 = !DILocation(line: 9, column: 30, scope: !729)
!734 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>>EnnL0L0", linkageName: "_CNW7builtin3opsEXi2>>EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!735 = !DILocalVariable(name: "lhs", scope: !734, file: !1, line: 4, type: !165)
!736 = !DILocation(line: 4, column: 19, scope: !734)
!737 = !DILocalVariable(name: "rhs", scope: !734, file: !1, line: 4, type: !165)
!738 = !DILocation(line: 5, column: 26, scope: !734)
!739 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3>>=EnnL0L0", linkageName: "_CNW7builtin3opsEXi3>>=EnnL0L0", scope: null, file: !6, line: 6, type: !171, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!740 = !DILocalVariable(name: "lhs", scope: !739, file: !1, line: 6, type: !165)
!741 = !DILocation(line: 6, column: 18, scope: !739)
!742 = !DILocalVariable(name: "rhs", scope: !739, file: !1, line: 6, type: !165)
!743 = !DILocation(line: 9, column: 30, scope: !739)
!744 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>>EhhL0L0", linkageName: "_CNW7builtin3opsEXi2>>EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!745 = !DILocalVariable(name: "lhs", scope: !744, file: !1, line: 4, type: !53)
!746 = !DILocation(line: 4, column: 19, scope: !744)
!747 = !DILocalVariable(name: "rhs", scope: !744, file: !1, line: 4, type: !53)
!748 = !DILocation(line: 5, column: 26, scope: !744)
!749 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3>>=EhhL0L0", linkageName: "_CNW7builtin3opsEXi3>>=EhhL0L0", scope: null, file: !6, line: 6, type: !59, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!750 = !DILocalVariable(name: "lhs", scope: !749, file: !1, line: 6, type: !53)
!751 = !DILocation(line: 6, column: 18, scope: !749)
!752 = !DILocalVariable(name: "rhs", scope: !749, file: !1, line: 6, type: !53)
!753 = !DILocation(line: 9, column: 30, scope: !749)
!754 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>>EttL0L0", linkageName: "_CNW7builtin3opsEXi2>>EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!755 = !DILocalVariable(name: "lhs", scope: !754, file: !1, line: 4, type: !85)
!756 = !DILocation(line: 4, column: 19, scope: !754)
!757 = !DILocalVariable(name: "rhs", scope: !754, file: !1, line: 4, type: !85)
!758 = !DILocation(line: 5, column: 26, scope: !754)
!759 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3>>=EttL0L0", linkageName: "_CNW7builtin3opsEXi3>>=EttL0L0", scope: null, file: !6, line: 6, type: !91, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!760 = !DILocalVariable(name: "lhs", scope: !759, file: !1, line: 6, type: !85)
!761 = !DILocation(line: 6, column: 18, scope: !759)
!762 = !DILocalVariable(name: "rhs", scope: !759, file: !1, line: 6, type: !85)
!763 = !DILocation(line: 9, column: 30, scope: !759)
!764 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>>EjjL0L0", linkageName: "_CNW7builtin3opsEXi2>>EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!765 = !DILocalVariable(name: "lhs", scope: !764, file: !1, line: 4, type: !117)
!766 = !DILocation(line: 4, column: 19, scope: !764)
!767 = !DILocalVariable(name: "rhs", scope: !764, file: !1, line: 4, type: !117)
!768 = !DILocation(line: 5, column: 26, scope: !764)
!769 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3>>=EjjL0L0", linkageName: "_CNW7builtin3opsEXi3>>=EjjL0L0", scope: null, file: !6, line: 6, type: !123, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!770 = !DILocalVariable(name: "lhs", scope: !769, file: !1, line: 6, type: !117)
!771 = !DILocation(line: 6, column: 18, scope: !769)
!772 = !DILocalVariable(name: "rhs", scope: !769, file: !1, line: 6, type: !117)
!773 = !DILocation(line: 9, column: 30, scope: !769)
!774 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>>EmmL0L0", linkageName: "_CNW7builtin3opsEXi2>>EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!775 = !DILocalVariable(name: "lhs", scope: !774, file: !1, line: 4, type: !149)
!776 = !DILocation(line: 4, column: 19, scope: !774)
!777 = !DILocalVariable(name: "rhs", scope: !774, file: !1, line: 4, type: !149)
!778 = !DILocation(line: 5, column: 26, scope: !774)
!779 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3>>=EmmL0L0", linkageName: "_CNW7builtin3opsEXi3>>=EmmL0L0", scope: null, file: !6, line: 6, type: !155, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!780 = !DILocalVariable(name: "lhs", scope: !779, file: !1, line: 6, type: !149)
!781 = !DILocation(line: 6, column: 18, scope: !779)
!782 = !DILocalVariable(name: "rhs", scope: !779, file: !1, line: 6, type: !149)
!783 = !DILocation(line: 9, column: 30, scope: !779)
!784 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>>EooL0L0", linkageName: "_CNW7builtin3opsEXi2>>EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!785 = !DILocalVariable(name: "lhs", scope: !784, file: !1, line: 4, type: !181)
!786 = !DILocation(line: 4, column: 19, scope: !784)
!787 = !DILocalVariable(name: "rhs", scope: !784, file: !1, line: 4, type: !181)
!788 = !DILocation(line: 5, column: 26, scope: !784)
!789 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi3>>=EooL0L0", linkageName: "_CNW7builtin3opsEXi3>>=EooL0L0", scope: null, file: !6, line: 6, type: !187, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!790 = !DILocalVariable(name: "lhs", scope: !789, file: !1, line: 6, type: !181)
!791 = !DILocation(line: 6, column: 18, scope: !789)
!792 = !DILocalVariable(name: "rhs", scope: !789, file: !1, line: 6, type: !181)
!793 = !DILocation(line: 9, column: 30, scope: !789)
!794 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1&EaaL0L0", linkageName: "_CNW7builtin3opsEXi1&EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!795 = !DILocalVariable(name: "lhs", scope: !794, file: !1, line: 4, type: !37)
!796 = !DILocation(line: 4, column: 19, scope: !794)
!797 = !DILocalVariable(name: "rhs", scope: !794, file: !1, line: 4, type: !37)
!798 = !DILocation(line: 5, column: 26, scope: !794)
!799 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2&=EaaL0L0", linkageName: "_CNW7builtin3opsEXi2&=EaaL0L0", scope: null, file: !6, line: 6, type: !43, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!800 = !DILocalVariable(name: "lhs", scope: !799, file: !1, line: 6, type: !37)
!801 = !DILocation(line: 6, column: 18, scope: !799)
!802 = !DILocalVariable(name: "rhs", scope: !799, file: !1, line: 6, type: !37)
!803 = !DILocation(line: 9, column: 30, scope: !799)
!804 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1&EhhL0L0", linkageName: "_CNW7builtin3opsEXi1&EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!805 = !DILocalVariable(name: "lhs", scope: !804, file: !1, line: 4, type: !53)
!806 = !DILocation(line: 4, column: 19, scope: !804)
!807 = !DILocalVariable(name: "rhs", scope: !804, file: !1, line: 4, type: !53)
!808 = !DILocation(line: 5, column: 26, scope: !804)
!809 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2&=EhhL0L0", linkageName: "_CNW7builtin3opsEXi2&=EhhL0L0", scope: null, file: !6, line: 6, type: !59, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!810 = !DILocalVariable(name: "lhs", scope: !809, file: !1, line: 6, type: !53)
!811 = !DILocation(line: 6, column: 18, scope: !809)
!812 = !DILocalVariable(name: "rhs", scope: !809, file: !1, line: 6, type: !53)
!813 = !DILocation(line: 9, column: 30, scope: !809)
!814 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1&EssL0L0", linkageName: "_CNW7builtin3opsEXi1&EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!815 = !DILocalVariable(name: "lhs", scope: !814, file: !1, line: 4, type: !69)
!816 = !DILocation(line: 4, column: 19, scope: !814)
!817 = !DILocalVariable(name: "rhs", scope: !814, file: !1, line: 4, type: !69)
!818 = !DILocation(line: 5, column: 26, scope: !814)
!819 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2&=EssL0L0", linkageName: "_CNW7builtin3opsEXi2&=EssL0L0", scope: null, file: !6, line: 6, type: !75, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!820 = !DILocalVariable(name: "lhs", scope: !819, file: !1, line: 6, type: !69)
!821 = !DILocation(line: 6, column: 18, scope: !819)
!822 = !DILocalVariable(name: "rhs", scope: !819, file: !1, line: 6, type: !69)
!823 = !DILocation(line: 9, column: 30, scope: !819)
!824 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1&EttL0L0", linkageName: "_CNW7builtin3opsEXi1&EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!825 = !DILocalVariable(name: "lhs", scope: !824, file: !1, line: 4, type: !85)
!826 = !DILocation(line: 4, column: 19, scope: !824)
!827 = !DILocalVariable(name: "rhs", scope: !824, file: !1, line: 4, type: !85)
!828 = !DILocation(line: 5, column: 26, scope: !824)
!829 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2&=EttL0L0", linkageName: "_CNW7builtin3opsEXi2&=EttL0L0", scope: null, file: !6, line: 6, type: !91, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!830 = !DILocalVariable(name: "lhs", scope: !829, file: !1, line: 6, type: !85)
!831 = !DILocation(line: 6, column: 18, scope: !829)
!832 = !DILocalVariable(name: "rhs", scope: !829, file: !1, line: 6, type: !85)
!833 = !DILocation(line: 9, column: 30, scope: !829)
!834 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1&EiiL0L0", linkageName: "_CNW7builtin3opsEXi1&EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!835 = !DILocalVariable(name: "lhs", scope: !834, file: !1, line: 4, type: !101)
!836 = !DILocation(line: 4, column: 19, scope: !834)
!837 = !DILocalVariable(name: "rhs", scope: !834, file: !1, line: 4, type: !101)
!838 = !DILocation(line: 5, column: 26, scope: !834)
!839 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2&=EiiL0L0", linkageName: "_CNW7builtin3opsEXi2&=EiiL0L0", scope: null, file: !6, line: 6, type: !107, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!840 = !DILocalVariable(name: "lhs", scope: !839, file: !1, line: 6, type: !101)
!841 = !DILocation(line: 6, column: 18, scope: !839)
!842 = !DILocalVariable(name: "rhs", scope: !839, file: !1, line: 6, type: !101)
!843 = !DILocation(line: 9, column: 30, scope: !839)
!844 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1&EjjL0L0", linkageName: "_CNW7builtin3opsEXi1&EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!845 = !DILocalVariable(name: "lhs", scope: !844, file: !1, line: 4, type: !117)
!846 = !DILocation(line: 4, column: 19, scope: !844)
!847 = !DILocalVariable(name: "rhs", scope: !844, file: !1, line: 4, type: !117)
!848 = !DILocation(line: 5, column: 26, scope: !844)
!849 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2&=EjjL0L0", linkageName: "_CNW7builtin3opsEXi2&=EjjL0L0", scope: null, file: !6, line: 6, type: !123, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!850 = !DILocalVariable(name: "lhs", scope: !849, file: !1, line: 6, type: !117)
!851 = !DILocation(line: 6, column: 18, scope: !849)
!852 = !DILocalVariable(name: "rhs", scope: !849, file: !1, line: 6, type: !117)
!853 = !DILocation(line: 9, column: 30, scope: !849)
!854 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1&EllL0L0", linkageName: "_CNW7builtin3opsEXi1&EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!855 = !DILocalVariable(name: "lhs", scope: !854, file: !1, line: 4, type: !133)
!856 = !DILocation(line: 4, column: 19, scope: !854)
!857 = !DILocalVariable(name: "rhs", scope: !854, file: !1, line: 4, type: !133)
!858 = !DILocation(line: 5, column: 26, scope: !854)
!859 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2&=EllL0L0", linkageName: "_CNW7builtin3opsEXi2&=EllL0L0", scope: null, file: !6, line: 6, type: !139, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!860 = !DILocalVariable(name: "lhs", scope: !859, file: !1, line: 6, type: !133)
!861 = !DILocation(line: 6, column: 18, scope: !859)
!862 = !DILocalVariable(name: "rhs", scope: !859, file: !1, line: 6, type: !133)
!863 = !DILocation(line: 9, column: 30, scope: !859)
!864 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1&EmmL0L0", linkageName: "_CNW7builtin3opsEXi1&EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!865 = !DILocalVariable(name: "lhs", scope: !864, file: !1, line: 4, type: !149)
!866 = !DILocation(line: 4, column: 19, scope: !864)
!867 = !DILocalVariable(name: "rhs", scope: !864, file: !1, line: 4, type: !149)
!868 = !DILocation(line: 5, column: 26, scope: !864)
!869 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2&=EmmL0L0", linkageName: "_CNW7builtin3opsEXi2&=EmmL0L0", scope: null, file: !6, line: 6, type: !155, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!870 = !DILocalVariable(name: "lhs", scope: !869, file: !1, line: 6, type: !149)
!871 = !DILocation(line: 6, column: 18, scope: !869)
!872 = !DILocalVariable(name: "rhs", scope: !869, file: !1, line: 6, type: !149)
!873 = !DILocation(line: 9, column: 30, scope: !869)
!874 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1&EnnL0L0", linkageName: "_CNW7builtin3opsEXi1&EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!875 = !DILocalVariable(name: "lhs", scope: !874, file: !1, line: 4, type: !165)
!876 = !DILocation(line: 4, column: 19, scope: !874)
!877 = !DILocalVariable(name: "rhs", scope: !874, file: !1, line: 4, type: !165)
!878 = !DILocation(line: 5, column: 26, scope: !874)
!879 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2&=EnnL0L0", linkageName: "_CNW7builtin3opsEXi2&=EnnL0L0", scope: null, file: !6, line: 6, type: !171, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!880 = !DILocalVariable(name: "lhs", scope: !879, file: !1, line: 6, type: !165)
!881 = !DILocation(line: 6, column: 18, scope: !879)
!882 = !DILocalVariable(name: "rhs", scope: !879, file: !1, line: 6, type: !165)
!883 = !DILocation(line: 9, column: 30, scope: !879)
!884 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1&EooL0L0", linkageName: "_CNW7builtin3opsEXi1&EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!885 = !DILocalVariable(name: "lhs", scope: !884, file: !1, line: 4, type: !181)
!886 = !DILocation(line: 4, column: 19, scope: !884)
!887 = !DILocalVariable(name: "rhs", scope: !884, file: !1, line: 4, type: !181)
!888 = !DILocation(line: 5, column: 26, scope: !884)
!889 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2&=EooL0L0", linkageName: "_CNW7builtin3opsEXi2&=EooL0L0", scope: null, file: !6, line: 6, type: !187, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!890 = !DILocalVariable(name: "lhs", scope: !889, file: !1, line: 6, type: !181)
!891 = !DILocation(line: 6, column: 18, scope: !889)
!892 = !DILocalVariable(name: "rhs", scope: !889, file: !1, line: 6, type: !181)
!893 = !DILocation(line: 9, column: 30, scope: !889)
!894 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1|EaaL0L0", linkageName: "_CNW7builtin3opsEXi1|EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!895 = !DILocalVariable(name: "lhs", scope: !894, file: !1, line: 4, type: !37)
!896 = !DILocation(line: 4, column: 19, scope: !894)
!897 = !DILocalVariable(name: "rhs", scope: !894, file: !1, line: 4, type: !37)
!898 = !DILocation(line: 5, column: 26, scope: !894)
!899 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2|=EaaL0L0", linkageName: "_CNW7builtin3opsEXi2|=EaaL0L0", scope: null, file: !6, line: 6, type: !43, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!900 = !DILocalVariable(name: "lhs", scope: !899, file: !1, line: 6, type: !37)
!901 = !DILocation(line: 6, column: 18, scope: !899)
!902 = !DILocalVariable(name: "rhs", scope: !899, file: !1, line: 6, type: !37)
!903 = !DILocation(line: 9, column: 30, scope: !899)
!904 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1|EhhL0L0", linkageName: "_CNW7builtin3opsEXi1|EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!905 = !DILocalVariable(name: "lhs", scope: !904, file: !1, line: 4, type: !53)
!906 = !DILocation(line: 4, column: 19, scope: !904)
!907 = !DILocalVariable(name: "rhs", scope: !904, file: !1, line: 4, type: !53)
!908 = !DILocation(line: 5, column: 26, scope: !904)
!909 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2|=EhhL0L0", linkageName: "_CNW7builtin3opsEXi2|=EhhL0L0", scope: null, file: !6, line: 6, type: !59, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!910 = !DILocalVariable(name: "lhs", scope: !909, file: !1, line: 6, type: !53)
!911 = !DILocation(line: 6, column: 18, scope: !909)
!912 = !DILocalVariable(name: "rhs", scope: !909, file: !1, line: 6, type: !53)
!913 = !DILocation(line: 9, column: 30, scope: !909)
!914 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1|EssL0L0", linkageName: "_CNW7builtin3opsEXi1|EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!915 = !DILocalVariable(name: "lhs", scope: !914, file: !1, line: 4, type: !69)
!916 = !DILocation(line: 4, column: 19, scope: !914)
!917 = !DILocalVariable(name: "rhs", scope: !914, file: !1, line: 4, type: !69)
!918 = !DILocation(line: 5, column: 26, scope: !914)
!919 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2|=EssL0L0", linkageName: "_CNW7builtin3opsEXi2|=EssL0L0", scope: null, file: !6, line: 6, type: !75, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!920 = !DILocalVariable(name: "lhs", scope: !919, file: !1, line: 6, type: !69)
!921 = !DILocation(line: 6, column: 18, scope: !919)
!922 = !DILocalVariable(name: "rhs", scope: !919, file: !1, line: 6, type: !69)
!923 = !DILocation(line: 9, column: 30, scope: !919)
!924 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1|EttL0L0", linkageName: "_CNW7builtin3opsEXi1|EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!925 = !DILocalVariable(name: "lhs", scope: !924, file: !1, line: 4, type: !85)
!926 = !DILocation(line: 4, column: 19, scope: !924)
!927 = !DILocalVariable(name: "rhs", scope: !924, file: !1, line: 4, type: !85)
!928 = !DILocation(line: 5, column: 26, scope: !924)
!929 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2|=EttL0L0", linkageName: "_CNW7builtin3opsEXi2|=EttL0L0", scope: null, file: !6, line: 6, type: !91, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!930 = !DILocalVariable(name: "lhs", scope: !929, file: !1, line: 6, type: !85)
!931 = !DILocation(line: 6, column: 18, scope: !929)
!932 = !DILocalVariable(name: "rhs", scope: !929, file: !1, line: 6, type: !85)
!933 = !DILocation(line: 9, column: 30, scope: !929)
!934 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1|EiiL0L0", linkageName: "_CNW7builtin3opsEXi1|EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!935 = !DILocalVariable(name: "lhs", scope: !934, file: !1, line: 4, type: !101)
!936 = !DILocation(line: 4, column: 19, scope: !934)
!937 = !DILocalVariable(name: "rhs", scope: !934, file: !1, line: 4, type: !101)
!938 = !DILocation(line: 5, column: 26, scope: !934)
!939 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2|=EiiL0L0", linkageName: "_CNW7builtin3opsEXi2|=EiiL0L0", scope: null, file: !6, line: 6, type: !107, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!940 = !DILocalVariable(name: "lhs", scope: !939, file: !1, line: 6, type: !101)
!941 = !DILocation(line: 6, column: 18, scope: !939)
!942 = !DILocalVariable(name: "rhs", scope: !939, file: !1, line: 6, type: !101)
!943 = !DILocation(line: 9, column: 30, scope: !939)
!944 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1|EjjL0L0", linkageName: "_CNW7builtin3opsEXi1|EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!945 = !DILocalVariable(name: "lhs", scope: !944, file: !1, line: 4, type: !117)
!946 = !DILocation(line: 4, column: 19, scope: !944)
!947 = !DILocalVariable(name: "rhs", scope: !944, file: !1, line: 4, type: !117)
!948 = !DILocation(line: 5, column: 26, scope: !944)
!949 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2|=EjjL0L0", linkageName: "_CNW7builtin3opsEXi2|=EjjL0L0", scope: null, file: !6, line: 6, type: !123, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!950 = !DILocalVariable(name: "lhs", scope: !949, file: !1, line: 6, type: !117)
!951 = !DILocation(line: 6, column: 18, scope: !949)
!952 = !DILocalVariable(name: "rhs", scope: !949, file: !1, line: 6, type: !117)
!953 = !DILocation(line: 9, column: 30, scope: !949)
!954 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1|EllL0L0", linkageName: "_CNW7builtin3opsEXi1|EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!955 = !DILocalVariable(name: "lhs", scope: !954, file: !1, line: 4, type: !133)
!956 = !DILocation(line: 4, column: 19, scope: !954)
!957 = !DILocalVariable(name: "rhs", scope: !954, file: !1, line: 4, type: !133)
!958 = !DILocation(line: 5, column: 26, scope: !954)
!959 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2|=EllL0L0", linkageName: "_CNW7builtin3opsEXi2|=EllL0L0", scope: null, file: !6, line: 6, type: !139, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!960 = !DILocalVariable(name: "lhs", scope: !959, file: !1, line: 6, type: !133)
!961 = !DILocation(line: 6, column: 18, scope: !959)
!962 = !DILocalVariable(name: "rhs", scope: !959, file: !1, line: 6, type: !133)
!963 = !DILocation(line: 9, column: 30, scope: !959)
!964 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1|EmmL0L0", linkageName: "_CNW7builtin3opsEXi1|EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!965 = !DILocalVariable(name: "lhs", scope: !964, file: !1, line: 4, type: !149)
!966 = !DILocation(line: 4, column: 19, scope: !964)
!967 = !DILocalVariable(name: "rhs", scope: !964, file: !1, line: 4, type: !149)
!968 = !DILocation(line: 5, column: 26, scope: !964)
!969 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2|=EmmL0L0", linkageName: "_CNW7builtin3opsEXi2|=EmmL0L0", scope: null, file: !6, line: 6, type: !155, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!970 = !DILocalVariable(name: "lhs", scope: !969, file: !1, line: 6, type: !149)
!971 = !DILocation(line: 6, column: 18, scope: !969)
!972 = !DILocalVariable(name: "rhs", scope: !969, file: !1, line: 6, type: !149)
!973 = !DILocation(line: 9, column: 30, scope: !969)
!974 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1|EnnL0L0", linkageName: "_CNW7builtin3opsEXi1|EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!975 = !DILocalVariable(name: "lhs", scope: !974, file: !1, line: 4, type: !165)
!976 = !DILocation(line: 4, column: 19, scope: !974)
!977 = !DILocalVariable(name: "rhs", scope: !974, file: !1, line: 4, type: !165)
!978 = !DILocation(line: 5, column: 26, scope: !974)
!979 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2|=EnnL0L0", linkageName: "_CNW7builtin3opsEXi2|=EnnL0L0", scope: null, file: !6, line: 6, type: !171, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!980 = !DILocalVariable(name: "lhs", scope: !979, file: !1, line: 6, type: !165)
!981 = !DILocation(line: 6, column: 18, scope: !979)
!982 = !DILocalVariable(name: "rhs", scope: !979, file: !1, line: 6, type: !165)
!983 = !DILocation(line: 9, column: 30, scope: !979)
!984 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1|EooL0L0", linkageName: "_CNW7builtin3opsEXi1|EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!985 = !DILocalVariable(name: "lhs", scope: !984, file: !1, line: 4, type: !181)
!986 = !DILocation(line: 4, column: 19, scope: !984)
!987 = !DILocalVariable(name: "rhs", scope: !984, file: !1, line: 4, type: !181)
!988 = !DILocation(line: 5, column: 26, scope: !984)
!989 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2|=EooL0L0", linkageName: "_CNW7builtin3opsEXi2|=EooL0L0", scope: null, file: !6, line: 6, type: !187, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!990 = !DILocalVariable(name: "lhs", scope: !989, file: !1, line: 6, type: !181)
!991 = !DILocation(line: 6, column: 18, scope: !989)
!992 = !DILocalVariable(name: "rhs", scope: !989, file: !1, line: 6, type: !181)
!993 = !DILocation(line: 9, column: 30, scope: !989)
!994 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1^EaaL0L0", linkageName: "_CNW7builtin3opsEXi1^EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!995 = !DILocalVariable(name: "lhs", scope: !994, file: !1, line: 4, type: !37)
!996 = !DILocation(line: 4, column: 19, scope: !994)
!997 = !DILocalVariable(name: "rhs", scope: !994, file: !1, line: 4, type: !37)
!998 = !DILocation(line: 5, column: 26, scope: !994)
!999 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2^=EaaL0L0", linkageName: "_CNW7builtin3opsEXi2^=EaaL0L0", scope: null, file: !6, line: 6, type: !43, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1000 = !DILocalVariable(name: "lhs", scope: !999, file: !1, line: 6, type: !37)
!1001 = !DILocation(line: 6, column: 18, scope: !999)
!1002 = !DILocalVariable(name: "rhs", scope: !999, file: !1, line: 6, type: !37)
!1003 = !DILocation(line: 9, column: 30, scope: !999)
!1004 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1^EhhL0L0", linkageName: "_CNW7builtin3opsEXi1^EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1005 = !DILocalVariable(name: "lhs", scope: !1004, file: !1, line: 4, type: !53)
!1006 = !DILocation(line: 4, column: 19, scope: !1004)
!1007 = !DILocalVariable(name: "rhs", scope: !1004, file: !1, line: 4, type: !53)
!1008 = !DILocation(line: 5, column: 26, scope: !1004)
!1009 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2^=EhhL0L0", linkageName: "_CNW7builtin3opsEXi2^=EhhL0L0", scope: null, file: !6, line: 6, type: !59, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1010 = !DILocalVariable(name: "lhs", scope: !1009, file: !1, line: 6, type: !53)
!1011 = !DILocation(line: 6, column: 18, scope: !1009)
!1012 = !DILocalVariable(name: "rhs", scope: !1009, file: !1, line: 6, type: !53)
!1013 = !DILocation(line: 9, column: 30, scope: !1009)
!1014 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1^EssL0L0", linkageName: "_CNW7builtin3opsEXi1^EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1015 = !DILocalVariable(name: "lhs", scope: !1014, file: !1, line: 4, type: !69)
!1016 = !DILocation(line: 4, column: 19, scope: !1014)
!1017 = !DILocalVariable(name: "rhs", scope: !1014, file: !1, line: 4, type: !69)
!1018 = !DILocation(line: 5, column: 26, scope: !1014)
!1019 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2^=EssL0L0", linkageName: "_CNW7builtin3opsEXi2^=EssL0L0", scope: null, file: !6, line: 6, type: !75, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1020 = !DILocalVariable(name: "lhs", scope: !1019, file: !1, line: 6, type: !69)
!1021 = !DILocation(line: 6, column: 18, scope: !1019)
!1022 = !DILocalVariable(name: "rhs", scope: !1019, file: !1, line: 6, type: !69)
!1023 = !DILocation(line: 9, column: 30, scope: !1019)
!1024 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1^EttL0L0", linkageName: "_CNW7builtin3opsEXi1^EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1025 = !DILocalVariable(name: "lhs", scope: !1024, file: !1, line: 4, type: !85)
!1026 = !DILocation(line: 4, column: 19, scope: !1024)
!1027 = !DILocalVariable(name: "rhs", scope: !1024, file: !1, line: 4, type: !85)
!1028 = !DILocation(line: 5, column: 26, scope: !1024)
!1029 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2^=EttL0L0", linkageName: "_CNW7builtin3opsEXi2^=EttL0L0", scope: null, file: !6, line: 6, type: !91, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1030 = !DILocalVariable(name: "lhs", scope: !1029, file: !1, line: 6, type: !85)
!1031 = !DILocation(line: 6, column: 18, scope: !1029)
!1032 = !DILocalVariable(name: "rhs", scope: !1029, file: !1, line: 6, type: !85)
!1033 = !DILocation(line: 9, column: 30, scope: !1029)
!1034 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1^EiiL0L0", linkageName: "_CNW7builtin3opsEXi1^EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1035 = !DILocalVariable(name: "lhs", scope: !1034, file: !1, line: 4, type: !101)
!1036 = !DILocation(line: 4, column: 19, scope: !1034)
!1037 = !DILocalVariable(name: "rhs", scope: !1034, file: !1, line: 4, type: !101)
!1038 = !DILocation(line: 5, column: 26, scope: !1034)
!1039 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2^=EiiL0L0", linkageName: "_CNW7builtin3opsEXi2^=EiiL0L0", scope: null, file: !6, line: 6, type: !107, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1040 = !DILocalVariable(name: "lhs", scope: !1039, file: !1, line: 6, type: !101)
!1041 = !DILocation(line: 6, column: 18, scope: !1039)
!1042 = !DILocalVariable(name: "rhs", scope: !1039, file: !1, line: 6, type: !101)
!1043 = !DILocation(line: 9, column: 30, scope: !1039)
!1044 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1^EjjL0L0", linkageName: "_CNW7builtin3opsEXi1^EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1045 = !DILocalVariable(name: "lhs", scope: !1044, file: !1, line: 4, type: !117)
!1046 = !DILocation(line: 4, column: 19, scope: !1044)
!1047 = !DILocalVariable(name: "rhs", scope: !1044, file: !1, line: 4, type: !117)
!1048 = !DILocation(line: 5, column: 26, scope: !1044)
!1049 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2^=EjjL0L0", linkageName: "_CNW7builtin3opsEXi2^=EjjL0L0", scope: null, file: !6, line: 6, type: !123, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1050 = !DILocalVariable(name: "lhs", scope: !1049, file: !1, line: 6, type: !117)
!1051 = !DILocation(line: 6, column: 18, scope: !1049)
!1052 = !DILocalVariable(name: "rhs", scope: !1049, file: !1, line: 6, type: !117)
!1053 = !DILocation(line: 9, column: 30, scope: !1049)
!1054 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1^EllL0L0", linkageName: "_CNW7builtin3opsEXi1^EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1055 = !DILocalVariable(name: "lhs", scope: !1054, file: !1, line: 4, type: !133)
!1056 = !DILocation(line: 4, column: 19, scope: !1054)
!1057 = !DILocalVariable(name: "rhs", scope: !1054, file: !1, line: 4, type: !133)
!1058 = !DILocation(line: 5, column: 26, scope: !1054)
!1059 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2^=EllL0L0", linkageName: "_CNW7builtin3opsEXi2^=EllL0L0", scope: null, file: !6, line: 6, type: !139, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1060 = !DILocalVariable(name: "lhs", scope: !1059, file: !1, line: 6, type: !133)
!1061 = !DILocation(line: 6, column: 18, scope: !1059)
!1062 = !DILocalVariable(name: "rhs", scope: !1059, file: !1, line: 6, type: !133)
!1063 = !DILocation(line: 9, column: 30, scope: !1059)
!1064 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1^EmmL0L0", linkageName: "_CNW7builtin3opsEXi1^EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1065 = !DILocalVariable(name: "lhs", scope: !1064, file: !1, line: 4, type: !149)
!1066 = !DILocation(line: 4, column: 19, scope: !1064)
!1067 = !DILocalVariable(name: "rhs", scope: !1064, file: !1, line: 4, type: !149)
!1068 = !DILocation(line: 5, column: 26, scope: !1064)
!1069 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2^=EmmL0L0", linkageName: "_CNW7builtin3opsEXi2^=EmmL0L0", scope: null, file: !6, line: 6, type: !155, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1070 = !DILocalVariable(name: "lhs", scope: !1069, file: !1, line: 6, type: !149)
!1071 = !DILocation(line: 6, column: 18, scope: !1069)
!1072 = !DILocalVariable(name: "rhs", scope: !1069, file: !1, line: 6, type: !149)
!1073 = !DILocation(line: 9, column: 30, scope: !1069)
!1074 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1^EnnL0L0", linkageName: "_CNW7builtin3opsEXi1^EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1075 = !DILocalVariable(name: "lhs", scope: !1074, file: !1, line: 4, type: !165)
!1076 = !DILocation(line: 4, column: 19, scope: !1074)
!1077 = !DILocalVariable(name: "rhs", scope: !1074, file: !1, line: 4, type: !165)
!1078 = !DILocation(line: 5, column: 26, scope: !1074)
!1079 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2^=EnnL0L0", linkageName: "_CNW7builtin3opsEXi2^=EnnL0L0", scope: null, file: !6, line: 6, type: !171, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1080 = !DILocalVariable(name: "lhs", scope: !1079, file: !1, line: 6, type: !165)
!1081 = !DILocation(line: 6, column: 18, scope: !1079)
!1082 = !DILocalVariable(name: "rhs", scope: !1079, file: !1, line: 6, type: !165)
!1083 = !DILocation(line: 9, column: 30, scope: !1079)
!1084 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1^EooL0L0", linkageName: "_CNW7builtin3opsEXi1^EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1085 = !DILocalVariable(name: "lhs", scope: !1084, file: !1, line: 4, type: !181)
!1086 = !DILocation(line: 4, column: 19, scope: !1084)
!1087 = !DILocalVariable(name: "rhs", scope: !1084, file: !1, line: 4, type: !181)
!1088 = !DILocation(line: 5, column: 26, scope: !1084)
!1089 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2^=EooL0L0", linkageName: "_CNW7builtin3opsEXi2^=EooL0L0", scope: null, file: !6, line: 6, type: !187, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1090 = !DILocalVariable(name: "lhs", scope: !1089, file: !1, line: 6, type: !181)
!1091 = !DILocation(line: 6, column: 18, scope: !1089)
!1092 = !DILocalVariable(name: "rhs", scope: !1089, file: !1, line: 6, type: !181)
!1093 = !DILocation(line: 9, column: 30, scope: !1089)
!1094 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2==EaaL0L0", linkageName: "_CNW7builtin3opsEXi2==EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1095 = !DILocalVariable(name: "lhs", scope: !1094, file: !1, line: 4, type: !37)
!1096 = !DILocation(line: 4, column: 27, scope: !1094)
!1097 = !DILocalVariable(name: "rhs", scope: !1094, file: !1, line: 4, type: !37)
!1098 = !DILocation(line: 2, column: 5, scope: !1094)
!1099 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2==EhhL0L0", linkageName: "_CNW7builtin3opsEXi2==EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1100 = !DILocalVariable(name: "lhs", scope: !1099, file: !1, line: 4, type: !53)
!1101 = !DILocation(line: 4, column: 27, scope: !1099)
!1102 = !DILocalVariable(name: "rhs", scope: !1099, file: !1, line: 4, type: !53)
!1103 = !DILocation(line: 2, column: 5, scope: !1099)
!1104 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2==EssL0L0", linkageName: "_CNW7builtin3opsEXi2==EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1105 = !DILocalVariable(name: "lhs", scope: !1104, file: !1, line: 4, type: !69)
!1106 = !DILocation(line: 4, column: 27, scope: !1104)
!1107 = !DILocalVariable(name: "rhs", scope: !1104, file: !1, line: 4, type: !69)
!1108 = !DILocation(line: 2, column: 5, scope: !1104)
!1109 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2==EttL0L0", linkageName: "_CNW7builtin3opsEXi2==EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1110 = !DILocalVariable(name: "lhs", scope: !1109, file: !1, line: 4, type: !85)
!1111 = !DILocation(line: 4, column: 27, scope: !1109)
!1112 = !DILocalVariable(name: "rhs", scope: !1109, file: !1, line: 4, type: !85)
!1113 = !DILocation(line: 2, column: 5, scope: !1109)
!1114 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2==EiiL0L0", linkageName: "_CNW7builtin3opsEXi2==EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1115 = !DILocalVariable(name: "lhs", scope: !1114, file: !1, line: 4, type: !101)
!1116 = !DILocation(line: 4, column: 27, scope: !1114)
!1117 = !DILocalVariable(name: "rhs", scope: !1114, file: !1, line: 4, type: !101)
!1118 = !DILocation(line: 2, column: 5, scope: !1114)
!1119 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2==EjjL0L0", linkageName: "_CNW7builtin3opsEXi2==EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1120 = !DILocalVariable(name: "lhs", scope: !1119, file: !1, line: 4, type: !117)
!1121 = !DILocation(line: 4, column: 27, scope: !1119)
!1122 = !DILocalVariable(name: "rhs", scope: !1119, file: !1, line: 4, type: !117)
!1123 = !DILocation(line: 2, column: 5, scope: !1119)
!1124 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2==EllL0L0", linkageName: "_CNW7builtin3opsEXi2==EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1125 = !DILocalVariable(name: "lhs", scope: !1124, file: !1, line: 4, type: !133)
!1126 = !DILocation(line: 4, column: 27, scope: !1124)
!1127 = !DILocalVariable(name: "rhs", scope: !1124, file: !1, line: 4, type: !133)
!1128 = !DILocation(line: 2, column: 5, scope: !1124)
!1129 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2==EmmL0L0", linkageName: "_CNW7builtin3opsEXi2==EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1130 = !DILocalVariable(name: "lhs", scope: !1129, file: !1, line: 4, type: !149)
!1131 = !DILocation(line: 4, column: 27, scope: !1129)
!1132 = !DILocalVariable(name: "rhs", scope: !1129, file: !1, line: 4, type: !149)
!1133 = !DILocation(line: 2, column: 5, scope: !1129)
!1134 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2==EnnL0L0", linkageName: "_CNW7builtin3opsEXi2==EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1135 = !DILocalVariable(name: "lhs", scope: !1134, file: !1, line: 4, type: !165)
!1136 = !DILocation(line: 4, column: 27, scope: !1134)
!1137 = !DILocalVariable(name: "rhs", scope: !1134, file: !1, line: 4, type: !165)
!1138 = !DILocation(line: 2, column: 5, scope: !1134)
!1139 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2==EooL0L0", linkageName: "_CNW7builtin3opsEXi2==EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1140 = !DILocalVariable(name: "lhs", scope: !1139, file: !1, line: 4, type: !181)
!1141 = !DILocation(line: 4, column: 27, scope: !1139)
!1142 = !DILocalVariable(name: "rhs", scope: !1139, file: !1, line: 4, type: !181)
!1143 = !DILocation(line: 2, column: 5, scope: !1139)
!1144 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2!=EaaL0L0", linkageName: "_CNW7builtin3opsEXi2!=EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1145 = !DILocalVariable(name: "lhs", scope: !1144, file: !1, line: 4, type: !37)
!1146 = !DILocation(line: 4, column: 27, scope: !1144)
!1147 = !DILocalVariable(name: "rhs", scope: !1144, file: !1, line: 4, type: !37)
!1148 = !DILocation(line: 2, column: 5, scope: !1144)
!1149 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2!=EhhL0L0", linkageName: "_CNW7builtin3opsEXi2!=EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1150 = !DILocalVariable(name: "lhs", scope: !1149, file: !1, line: 4, type: !53)
!1151 = !DILocation(line: 4, column: 27, scope: !1149)
!1152 = !DILocalVariable(name: "rhs", scope: !1149, file: !1, line: 4, type: !53)
!1153 = !DILocation(line: 2, column: 5, scope: !1149)
!1154 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2!=EssL0L0", linkageName: "_CNW7builtin3opsEXi2!=EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1155 = !DILocalVariable(name: "lhs", scope: !1154, file: !1, line: 4, type: !69)
!1156 = !DILocation(line: 4, column: 27, scope: !1154)
!1157 = !DILocalVariable(name: "rhs", scope: !1154, file: !1, line: 4, type: !69)
!1158 = !DILocation(line: 2, column: 5, scope: !1154)
!1159 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2!=EttL0L0", linkageName: "_CNW7builtin3opsEXi2!=EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1160 = !DILocalVariable(name: "lhs", scope: !1159, file: !1, line: 4, type: !85)
!1161 = !DILocation(line: 4, column: 27, scope: !1159)
!1162 = !DILocalVariable(name: "rhs", scope: !1159, file: !1, line: 4, type: !85)
!1163 = !DILocation(line: 2, column: 5, scope: !1159)
!1164 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2!=EiiL0L0", linkageName: "_CNW7builtin3opsEXi2!=EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1165 = !DILocalVariable(name: "lhs", scope: !1164, file: !1, line: 4, type: !101)
!1166 = !DILocation(line: 4, column: 27, scope: !1164)
!1167 = !DILocalVariable(name: "rhs", scope: !1164, file: !1, line: 4, type: !101)
!1168 = !DILocation(line: 2, column: 5, scope: !1164)
!1169 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2!=EjjL0L0", linkageName: "_CNW7builtin3opsEXi2!=EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1170 = !DILocalVariable(name: "lhs", scope: !1169, file: !1, line: 4, type: !117)
!1171 = !DILocation(line: 4, column: 27, scope: !1169)
!1172 = !DILocalVariable(name: "rhs", scope: !1169, file: !1, line: 4, type: !117)
!1173 = !DILocation(line: 2, column: 5, scope: !1169)
!1174 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2!=EllL0L0", linkageName: "_CNW7builtin3opsEXi2!=EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1175 = !DILocalVariable(name: "lhs", scope: !1174, file: !1, line: 4, type: !133)
!1176 = !DILocation(line: 4, column: 27, scope: !1174)
!1177 = !DILocalVariable(name: "rhs", scope: !1174, file: !1, line: 4, type: !133)
!1178 = !DILocation(line: 2, column: 5, scope: !1174)
!1179 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2!=EmmL0L0", linkageName: "_CNW7builtin3opsEXi2!=EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1180 = !DILocalVariable(name: "lhs", scope: !1179, file: !1, line: 4, type: !149)
!1181 = !DILocation(line: 4, column: 27, scope: !1179)
!1182 = !DILocalVariable(name: "rhs", scope: !1179, file: !1, line: 4, type: !149)
!1183 = !DILocation(line: 2, column: 5, scope: !1179)
!1184 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2!=EnnL0L0", linkageName: "_CNW7builtin3opsEXi2!=EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1185 = !DILocalVariable(name: "lhs", scope: !1184, file: !1, line: 4, type: !165)
!1186 = !DILocation(line: 4, column: 27, scope: !1184)
!1187 = !DILocalVariable(name: "rhs", scope: !1184, file: !1, line: 4, type: !165)
!1188 = !DILocation(line: 2, column: 5, scope: !1184)
!1189 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2!=EooL0L0", linkageName: "_CNW7builtin3opsEXi2!=EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1190 = !DILocalVariable(name: "lhs", scope: !1189, file: !1, line: 4, type: !181)
!1191 = !DILocation(line: 4, column: 27, scope: !1189)
!1192 = !DILocalVariable(name: "rhs", scope: !1189, file: !1, line: 4, type: !181)
!1193 = !DILocation(line: 2, column: 5, scope: !1189)
!1194 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>=EaaL0L0", linkageName: "_CNW7builtin3opsEXi2>=EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1195 = !DILocalVariable(name: "lhs", scope: !1194, file: !1, line: 4, type: !37)
!1196 = !DILocation(line: 4, column: 27, scope: !1194)
!1197 = !DILocalVariable(name: "rhs", scope: !1194, file: !1, line: 4, type: !37)
!1198 = !DILocation(line: 2, column: 5, scope: !1194)
!1199 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>=EssL0L0", linkageName: "_CNW7builtin3opsEXi2>=EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1200 = !DILocalVariable(name: "lhs", scope: !1199, file: !1, line: 4, type: !69)
!1201 = !DILocation(line: 4, column: 27, scope: !1199)
!1202 = !DILocalVariable(name: "rhs", scope: !1199, file: !1, line: 4, type: !69)
!1203 = !DILocation(line: 2, column: 5, scope: !1199)
!1204 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>=EiiL0L0", linkageName: "_CNW7builtin3opsEXi2>=EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1205 = !DILocalVariable(name: "lhs", scope: !1204, file: !1, line: 4, type: !101)
!1206 = !DILocation(line: 4, column: 27, scope: !1204)
!1207 = !DILocalVariable(name: "rhs", scope: !1204, file: !1, line: 4, type: !101)
!1208 = !DILocation(line: 2, column: 5, scope: !1204)
!1209 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>=EllL0L0", linkageName: "_CNW7builtin3opsEXi2>=EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1210 = !DILocalVariable(name: "lhs", scope: !1209, file: !1, line: 4, type: !133)
!1211 = !DILocation(line: 4, column: 27, scope: !1209)
!1212 = !DILocalVariable(name: "rhs", scope: !1209, file: !1, line: 4, type: !133)
!1213 = !DILocation(line: 2, column: 5, scope: !1209)
!1214 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>=EnnL0L0", linkageName: "_CNW7builtin3opsEXi2>=EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1215 = !DILocalVariable(name: "lhs", scope: !1214, file: !1, line: 4, type: !165)
!1216 = !DILocation(line: 4, column: 27, scope: !1214)
!1217 = !DILocalVariable(name: "rhs", scope: !1214, file: !1, line: 4, type: !165)
!1218 = !DILocation(line: 2, column: 5, scope: !1214)
!1219 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1>EaaL0L0", linkageName: "_CNW7builtin3opsEXi1>EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1220 = !DILocalVariable(name: "lhs", scope: !1219, file: !1, line: 4, type: !37)
!1221 = !DILocation(line: 4, column: 27, scope: !1219)
!1222 = !DILocalVariable(name: "rhs", scope: !1219, file: !1, line: 4, type: !37)
!1223 = !DILocation(line: 2, column: 5, scope: !1219)
!1224 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1>EssL0L0", linkageName: "_CNW7builtin3opsEXi1>EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1225 = !DILocalVariable(name: "lhs", scope: !1224, file: !1, line: 4, type: !69)
!1226 = !DILocation(line: 4, column: 27, scope: !1224)
!1227 = !DILocalVariable(name: "rhs", scope: !1224, file: !1, line: 4, type: !69)
!1228 = !DILocation(line: 2, column: 5, scope: !1224)
!1229 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1>EiiL0L0", linkageName: "_CNW7builtin3opsEXi1>EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1230 = !DILocalVariable(name: "lhs", scope: !1229, file: !1, line: 4, type: !101)
!1231 = !DILocation(line: 4, column: 27, scope: !1229)
!1232 = !DILocalVariable(name: "rhs", scope: !1229, file: !1, line: 4, type: !101)
!1233 = !DILocation(line: 2, column: 5, scope: !1229)
!1234 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1>EllL0L0", linkageName: "_CNW7builtin3opsEXi1>EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1235 = !DILocalVariable(name: "lhs", scope: !1234, file: !1, line: 4, type: !133)
!1236 = !DILocation(line: 4, column: 27, scope: !1234)
!1237 = !DILocalVariable(name: "rhs", scope: !1234, file: !1, line: 4, type: !133)
!1238 = !DILocation(line: 2, column: 5, scope: !1234)
!1239 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1>EnnL0L0", linkageName: "_CNW7builtin3opsEXi1>EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1240 = !DILocalVariable(name: "lhs", scope: !1239, file: !1, line: 4, type: !165)
!1241 = !DILocation(line: 4, column: 27, scope: !1239)
!1242 = !DILocalVariable(name: "rhs", scope: !1239, file: !1, line: 4, type: !165)
!1243 = !DILocation(line: 2, column: 5, scope: !1239)
!1244 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<=EaaL0L0", linkageName: "_CNW7builtin3opsEXi2<=EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1245 = !DILocalVariable(name: "lhs", scope: !1244, file: !1, line: 4, type: !37)
!1246 = !DILocation(line: 4, column: 27, scope: !1244)
!1247 = !DILocalVariable(name: "rhs", scope: !1244, file: !1, line: 4, type: !37)
!1248 = !DILocation(line: 2, column: 5, scope: !1244)
!1249 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<=EssL0L0", linkageName: "_CNW7builtin3opsEXi2<=EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1250 = !DILocalVariable(name: "lhs", scope: !1249, file: !1, line: 4, type: !69)
!1251 = !DILocation(line: 4, column: 27, scope: !1249)
!1252 = !DILocalVariable(name: "rhs", scope: !1249, file: !1, line: 4, type: !69)
!1253 = !DILocation(line: 2, column: 5, scope: !1249)
!1254 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<=EiiL0L0", linkageName: "_CNW7builtin3opsEXi2<=EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1255 = !DILocalVariable(name: "lhs", scope: !1254, file: !1, line: 4, type: !101)
!1256 = !DILocation(line: 4, column: 27, scope: !1254)
!1257 = !DILocalVariable(name: "rhs", scope: !1254, file: !1, line: 4, type: !101)
!1258 = !DILocation(line: 2, column: 5, scope: !1254)
!1259 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<=EllL0L0", linkageName: "_CNW7builtin3opsEXi2<=EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1260 = !DILocalVariable(name: "lhs", scope: !1259, file: !1, line: 4, type: !133)
!1261 = !DILocation(line: 4, column: 27, scope: !1259)
!1262 = !DILocalVariable(name: "rhs", scope: !1259, file: !1, line: 4, type: !133)
!1263 = !DILocation(line: 2, column: 5, scope: !1259)
!1264 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<=EnnL0L0", linkageName: "_CNW7builtin3opsEXi2<=EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1265 = !DILocalVariable(name: "lhs", scope: !1264, file: !1, line: 4, type: !165)
!1266 = !DILocation(line: 4, column: 27, scope: !1264)
!1267 = !DILocalVariable(name: "rhs", scope: !1264, file: !1, line: 4, type: !165)
!1268 = !DILocation(line: 2, column: 5, scope: !1264)
!1269 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1<EaaL0L0", linkageName: "_CNW7builtin3opsEXi1<EaaL0L0", scope: null, file: !6, line: 4, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1270 = !DILocalVariable(name: "lhs", scope: !1269, file: !1, line: 4, type: !37)
!1271 = !DILocation(line: 4, column: 27, scope: !1269)
!1272 = !DILocalVariable(name: "rhs", scope: !1269, file: !1, line: 4, type: !37)
!1273 = !DILocation(line: 2, column: 5, scope: !1269)
!1274 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1<EssL0L0", linkageName: "_CNW7builtin3opsEXi1<EssL0L0", scope: null, file: !6, line: 4, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1275 = !DILocalVariable(name: "lhs", scope: !1274, file: !1, line: 4, type: !69)
!1276 = !DILocation(line: 4, column: 27, scope: !1274)
!1277 = !DILocalVariable(name: "rhs", scope: !1274, file: !1, line: 4, type: !69)
!1278 = !DILocation(line: 2, column: 5, scope: !1274)
!1279 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1<EiiL0L0", linkageName: "_CNW7builtin3opsEXi1<EiiL0L0", scope: null, file: !6, line: 4, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1280 = !DILocalVariable(name: "lhs", scope: !1279, file: !1, line: 4, type: !101)
!1281 = !DILocation(line: 4, column: 27, scope: !1279)
!1282 = !DILocalVariable(name: "rhs", scope: !1279, file: !1, line: 4, type: !101)
!1283 = !DILocation(line: 2, column: 5, scope: !1279)
!1284 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1<EllL0L0", linkageName: "_CNW7builtin3opsEXi1<EllL0L0", scope: null, file: !6, line: 4, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1285 = !DILocalVariable(name: "lhs", scope: !1284, file: !1, line: 4, type: !133)
!1286 = !DILocation(line: 4, column: 27, scope: !1284)
!1287 = !DILocalVariable(name: "rhs", scope: !1284, file: !1, line: 4, type: !133)
!1288 = !DILocation(line: 2, column: 5, scope: !1284)
!1289 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1<EnnL0L0", linkageName: "_CNW7builtin3opsEXi1<EnnL0L0", scope: null, file: !6, line: 4, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1290 = !DILocalVariable(name: "lhs", scope: !1289, file: !1, line: 4, type: !165)
!1291 = !DILocation(line: 4, column: 27, scope: !1289)
!1292 = !DILocalVariable(name: "rhs", scope: !1289, file: !1, line: 4, type: !165)
!1293 = !DILocation(line: 2, column: 5, scope: !1289)
!1294 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>=EhhL0L0", linkageName: "_CNW7builtin3opsEXi2>=EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1295 = !DILocalVariable(name: "lhs", scope: !1294, file: !1, line: 4, type: !53)
!1296 = !DILocation(line: 4, column: 27, scope: !1294)
!1297 = !DILocalVariable(name: "rhs", scope: !1294, file: !1, line: 4, type: !53)
!1298 = !DILocation(line: 2, column: 5, scope: !1294)
!1299 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>=EttL0L0", linkageName: "_CNW7builtin3opsEXi2>=EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1300 = !DILocalVariable(name: "lhs", scope: !1299, file: !1, line: 4, type: !85)
!1301 = !DILocation(line: 4, column: 27, scope: !1299)
!1302 = !DILocalVariable(name: "rhs", scope: !1299, file: !1, line: 4, type: !85)
!1303 = !DILocation(line: 2, column: 5, scope: !1299)
!1304 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>=EjjL0L0", linkageName: "_CNW7builtin3opsEXi2>=EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1305 = !DILocalVariable(name: "lhs", scope: !1304, file: !1, line: 4, type: !117)
!1306 = !DILocation(line: 4, column: 27, scope: !1304)
!1307 = !DILocalVariable(name: "rhs", scope: !1304, file: !1, line: 4, type: !117)
!1308 = !DILocation(line: 2, column: 5, scope: !1304)
!1309 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>=EmmL0L0", linkageName: "_CNW7builtin3opsEXi2>=EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1310 = !DILocalVariable(name: "lhs", scope: !1309, file: !1, line: 4, type: !149)
!1311 = !DILocation(line: 4, column: 27, scope: !1309)
!1312 = !DILocalVariable(name: "rhs", scope: !1309, file: !1, line: 4, type: !149)
!1313 = !DILocation(line: 2, column: 5, scope: !1309)
!1314 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>=EooL0L0", linkageName: "_CNW7builtin3opsEXi2>=EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1315 = !DILocalVariable(name: "lhs", scope: !1314, file: !1, line: 4, type: !181)
!1316 = !DILocation(line: 4, column: 27, scope: !1314)
!1317 = !DILocalVariable(name: "rhs", scope: !1314, file: !1, line: 4, type: !181)
!1318 = !DILocation(line: 2, column: 5, scope: !1314)
!1319 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1>EhhL0L0", linkageName: "_CNW7builtin3opsEXi1>EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1320 = !DILocalVariable(name: "lhs", scope: !1319, file: !1, line: 4, type: !53)
!1321 = !DILocation(line: 4, column: 27, scope: !1319)
!1322 = !DILocalVariable(name: "rhs", scope: !1319, file: !1, line: 4, type: !53)
!1323 = !DILocation(line: 2, column: 5, scope: !1319)
!1324 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1>EttL0L0", linkageName: "_CNW7builtin3opsEXi1>EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1325 = !DILocalVariable(name: "lhs", scope: !1324, file: !1, line: 4, type: !85)
!1326 = !DILocation(line: 4, column: 27, scope: !1324)
!1327 = !DILocalVariable(name: "rhs", scope: !1324, file: !1, line: 4, type: !85)
!1328 = !DILocation(line: 2, column: 5, scope: !1324)
!1329 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1>EjjL0L0", linkageName: "_CNW7builtin3opsEXi1>EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1330 = !DILocalVariable(name: "lhs", scope: !1329, file: !1, line: 4, type: !117)
!1331 = !DILocation(line: 4, column: 27, scope: !1329)
!1332 = !DILocalVariable(name: "rhs", scope: !1329, file: !1, line: 4, type: !117)
!1333 = !DILocation(line: 2, column: 5, scope: !1329)
!1334 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1>EmmL0L0", linkageName: "_CNW7builtin3opsEXi1>EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1335 = !DILocalVariable(name: "lhs", scope: !1334, file: !1, line: 4, type: !149)
!1336 = !DILocation(line: 4, column: 27, scope: !1334)
!1337 = !DILocalVariable(name: "rhs", scope: !1334, file: !1, line: 4, type: !149)
!1338 = !DILocation(line: 2, column: 5, scope: !1334)
!1339 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1>EooL0L0", linkageName: "_CNW7builtin3opsEXi1>EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1340 = !DILocalVariable(name: "lhs", scope: !1339, file: !1, line: 4, type: !181)
!1341 = !DILocation(line: 4, column: 27, scope: !1339)
!1342 = !DILocalVariable(name: "rhs", scope: !1339, file: !1, line: 4, type: !181)
!1343 = !DILocation(line: 2, column: 5, scope: !1339)
!1344 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<=EhhL0L0", linkageName: "_CNW7builtin3opsEXi2<=EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1345 = !DILocalVariable(name: "lhs", scope: !1344, file: !1, line: 4, type: !53)
!1346 = !DILocation(line: 4, column: 27, scope: !1344)
!1347 = !DILocalVariable(name: "rhs", scope: !1344, file: !1, line: 4, type: !53)
!1348 = !DILocation(line: 2, column: 5, scope: !1344)
!1349 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<=EttL0L0", linkageName: "_CNW7builtin3opsEXi2<=EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1350 = !DILocalVariable(name: "lhs", scope: !1349, file: !1, line: 4, type: !85)
!1351 = !DILocation(line: 4, column: 27, scope: !1349)
!1352 = !DILocalVariable(name: "rhs", scope: !1349, file: !1, line: 4, type: !85)
!1353 = !DILocation(line: 2, column: 5, scope: !1349)
!1354 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<=EjjL0L0", linkageName: "_CNW7builtin3opsEXi2<=EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1355 = !DILocalVariable(name: "lhs", scope: !1354, file: !1, line: 4, type: !117)
!1356 = !DILocation(line: 4, column: 27, scope: !1354)
!1357 = !DILocalVariable(name: "rhs", scope: !1354, file: !1, line: 4, type: !117)
!1358 = !DILocation(line: 2, column: 5, scope: !1354)
!1359 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<=EmmL0L0", linkageName: "_CNW7builtin3opsEXi2<=EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1360 = !DILocalVariable(name: "lhs", scope: !1359, file: !1, line: 4, type: !149)
!1361 = !DILocation(line: 4, column: 27, scope: !1359)
!1362 = !DILocalVariable(name: "rhs", scope: !1359, file: !1, line: 4, type: !149)
!1363 = !DILocation(line: 2, column: 5, scope: !1359)
!1364 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<=EooL0L0", linkageName: "_CNW7builtin3opsEXi2<=EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1365 = !DILocalVariable(name: "lhs", scope: !1364, file: !1, line: 4, type: !181)
!1366 = !DILocation(line: 4, column: 27, scope: !1364)
!1367 = !DILocalVariable(name: "rhs", scope: !1364, file: !1, line: 4, type: !181)
!1368 = !DILocation(line: 2, column: 5, scope: !1364)
!1369 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1<EhhL0L0", linkageName: "_CNW7builtin3opsEXi1<EhhL0L0", scope: null, file: !6, line: 4, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1370 = !DILocalVariable(name: "lhs", scope: !1369, file: !1, line: 4, type: !53)
!1371 = !DILocation(line: 4, column: 27, scope: !1369)
!1372 = !DILocalVariable(name: "rhs", scope: !1369, file: !1, line: 4, type: !53)
!1373 = !DILocation(line: 2, column: 5, scope: !1369)
!1374 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1<EttL0L0", linkageName: "_CNW7builtin3opsEXi1<EttL0L0", scope: null, file: !6, line: 4, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1375 = !DILocalVariable(name: "lhs", scope: !1374, file: !1, line: 4, type: !85)
!1376 = !DILocation(line: 4, column: 27, scope: !1374)
!1377 = !DILocalVariable(name: "rhs", scope: !1374, file: !1, line: 4, type: !85)
!1378 = !DILocation(line: 2, column: 5, scope: !1374)
!1379 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1<EjjL0L0", linkageName: "_CNW7builtin3opsEXi1<EjjL0L0", scope: null, file: !6, line: 4, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1380 = !DILocalVariable(name: "lhs", scope: !1379, file: !1, line: 4, type: !117)
!1381 = !DILocation(line: 4, column: 27, scope: !1379)
!1382 = !DILocalVariable(name: "rhs", scope: !1379, file: !1, line: 4, type: !117)
!1383 = !DILocation(line: 2, column: 5, scope: !1379)
!1384 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1<EmmL0L0", linkageName: "_CNW7builtin3opsEXi1<EmmL0L0", scope: null, file: !6, line: 4, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1385 = !DILocalVariable(name: "lhs", scope: !1384, file: !1, line: 4, type: !149)
!1386 = !DILocation(line: 4, column: 27, scope: !1384)
!1387 = !DILocalVariable(name: "rhs", scope: !1384, file: !1, line: 4, type: !149)
!1388 = !DILocation(line: 2, column: 5, scope: !1384)
!1389 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1<EooL0L0", linkageName: "_CNW7builtin3opsEXi1<EooL0L0", scope: null, file: !6, line: 4, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1390 = !DILocalVariable(name: "lhs", scope: !1389, file: !1, line: 4, type: !181)
!1391 = !DILocation(line: 4, column: 27, scope: !1389)
!1392 = !DILocalVariable(name: "rhs", scope: !1389, file: !1, line: 4, type: !181)
!1393 = !DILocation(line: 2, column: 5, scope: !1389)
!1394 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1+EaL0", linkageName: "_CNW7builtin3opsEXp1+EaL0", scope: null, file: !6, line: 4, type: !1395, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1395 = !DISubroutineType(types: !1396)
!1396 = !{!37}
!1397 = !DILocalVariable(name: "val", scope: !1394, file: !1, line: 4, type: !37)
!1398 = !DILocation(line: 4, column: 19, scope: !1394)
!1399 = !DILocation(line: 2, column: 23, scope: !1394)
!1400 = !DILocation(line: 455, column: 18, scope: !1394)
!1401 = distinct !DISubprogram(name: "_CNW7builtinE5id_i8EaL0", linkageName: "_CNW7builtinE5id_i8EaL0", scope: null, file: !6, line: 87, type: !1395, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1402 = !DILocalVariable(name: "val", scope: !1401, file: !1, line: 87, type: !37)
!1403 = !DILocation(line: 87, column: 2044, scope: !1401)
!1404 = !DILocation(line: 87, column: 2098, scope: !1401)
!1405 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1+EhL0", linkageName: "_CNW7builtin3opsEXp1+EhL0", scope: null, file: !6, line: 4, type: !1406, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1406 = !DISubroutineType(types: !1407)
!1407 = !{!53}
!1408 = !DILocalVariable(name: "val", scope: !1405, file: !1, line: 4, type: !53)
!1409 = !DILocation(line: 4, column: 19, scope: !1405)
!1410 = !DILocation(line: 2, column: 23, scope: !1405)
!1411 = !DILocation(line: 455, column: 18, scope: !1405)
!1412 = distinct !DISubprogram(name: "_CNW7builtinE5id_u8EhL0", linkageName: "_CNW7builtinE5id_u8EhL0", scope: null, file: !6, line: 87, type: !1406, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1413 = !DILocalVariable(name: "val", scope: !1412, file: !1, line: 87, type: !53)
!1414 = !DILocation(line: 87, column: 2044, scope: !1412)
!1415 = !DILocation(line: 87, column: 2098, scope: !1412)
!1416 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1+EsL0", linkageName: "_CNW7builtin3opsEXp1+EsL0", scope: null, file: !6, line: 4, type: !1417, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1417 = !DISubroutineType(types: !1418)
!1418 = !{!69}
!1419 = !DILocalVariable(name: "val", scope: !1416, file: !1, line: 4, type: !69)
!1420 = !DILocation(line: 4, column: 19, scope: !1416)
!1421 = !DILocation(line: 2, column: 23, scope: !1416)
!1422 = !DILocation(line: 455, column: 18, scope: !1416)
!1423 = distinct !DISubprogram(name: "_CNW7builtinE6id_i16EsL0", linkageName: "_CNW7builtinE6id_i16EsL0", scope: null, file: !6, line: 87, type: !1417, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1424 = !DILocalVariable(name: "val", scope: !1423, file: !1, line: 87, type: !69)
!1425 = !DILocation(line: 87, column: 2044, scope: !1423)
!1426 = !DILocation(line: 87, column: 2098, scope: !1423)
!1427 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1+EtL0", linkageName: "_CNW7builtin3opsEXp1+EtL0", scope: null, file: !6, line: 4, type: !1428, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1428 = !DISubroutineType(types: !1429)
!1429 = !{!85}
!1430 = !DILocalVariable(name: "val", scope: !1427, file: !1, line: 4, type: !85)
!1431 = !DILocation(line: 4, column: 19, scope: !1427)
!1432 = !DILocation(line: 2, column: 23, scope: !1427)
!1433 = !DILocation(line: 455, column: 18, scope: !1427)
!1434 = distinct !DISubprogram(name: "_CNW7builtinE6id_u16EtL0", linkageName: "_CNW7builtinE6id_u16EtL0", scope: null, file: !6, line: 87, type: !1428, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1435 = !DILocalVariable(name: "val", scope: !1434, file: !1, line: 87, type: !85)
!1436 = !DILocation(line: 87, column: 2044, scope: !1434)
!1437 = !DILocation(line: 87, column: 2098, scope: !1434)
!1438 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1+EiL0", linkageName: "_CNW7builtin3opsEXp1+EiL0", scope: null, file: !6, line: 4, type: !1439, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1439 = !DISubroutineType(types: !1440)
!1440 = !{!101}
!1441 = !DILocalVariable(name: "val", scope: !1438, file: !1, line: 4, type: !101)
!1442 = !DILocation(line: 4, column: 19, scope: !1438)
!1443 = !DILocation(line: 2, column: 23, scope: !1438)
!1444 = !DILocation(line: 455, column: 18, scope: !1438)
!1445 = distinct !DISubprogram(name: "_CNW7builtinE6id_i32EiL0", linkageName: "_CNW7builtinE6id_i32EiL0", scope: null, file: !6, line: 87, type: !1439, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1446 = !DILocalVariable(name: "val", scope: !1445, file: !1, line: 87, type: !101)
!1447 = !DILocation(line: 87, column: 2044, scope: !1445)
!1448 = !DILocation(line: 87, column: 2098, scope: !1445)
!1449 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1+EjL0", linkageName: "_CNW7builtin3opsEXp1+EjL0", scope: null, file: !6, line: 4, type: !1450, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1450 = !DISubroutineType(types: !1451)
!1451 = !{!117}
!1452 = !DILocalVariable(name: "val", scope: !1449, file: !1, line: 4, type: !117)
!1453 = !DILocation(line: 4, column: 19, scope: !1449)
!1454 = !DILocation(line: 2, column: 23, scope: !1449)
!1455 = !DILocation(line: 455, column: 18, scope: !1449)
!1456 = distinct !DISubprogram(name: "_CNW7builtinE6id_u32EjL0", linkageName: "_CNW7builtinE6id_u32EjL0", scope: null, file: !6, line: 87, type: !1450, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1457 = !DILocalVariable(name: "val", scope: !1456, file: !1, line: 87, type: !117)
!1458 = !DILocation(line: 87, column: 2044, scope: !1456)
!1459 = !DILocation(line: 87, column: 2098, scope: !1456)
!1460 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1+ElL0", linkageName: "_CNW7builtin3opsEXp1+ElL0", scope: null, file: !6, line: 4, type: !1461, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1461 = !DISubroutineType(types: !1462)
!1462 = !{!133}
!1463 = !DILocalVariable(name: "val", scope: !1460, file: !1, line: 4, type: !133)
!1464 = !DILocation(line: 4, column: 19, scope: !1460)
!1465 = !DILocation(line: 2, column: 23, scope: !1460)
!1466 = !DILocation(line: 455, column: 18, scope: !1460)
!1467 = distinct !DISubprogram(name: "_CNW7builtinE6id_i64ElL0", linkageName: "_CNW7builtinE6id_i64ElL0", scope: null, file: !6, line: 87, type: !1461, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1468 = !DILocalVariable(name: "val", scope: !1467, file: !1, line: 87, type: !133)
!1469 = !DILocation(line: 87, column: 2044, scope: !1467)
!1470 = !DILocation(line: 87, column: 2098, scope: !1467)
!1471 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1+EmL0", linkageName: "_CNW7builtin3opsEXp1+EmL0", scope: null, file: !6, line: 4, type: !1472, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1472 = !DISubroutineType(types: !1473)
!1473 = !{!149}
!1474 = !DILocalVariable(name: "val", scope: !1471, file: !1, line: 4, type: !149)
!1475 = !DILocation(line: 4, column: 19, scope: !1471)
!1476 = !DILocation(line: 2, column: 23, scope: !1471)
!1477 = !DILocation(line: 455, column: 18, scope: !1471)
!1478 = distinct !DISubprogram(name: "_CNW7builtinE6id_u64EmL0", linkageName: "_CNW7builtinE6id_u64EmL0", scope: null, file: !6, line: 87, type: !1472, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1479 = !DILocalVariable(name: "val", scope: !1478, file: !1, line: 87, type: !149)
!1480 = !DILocation(line: 87, column: 2044, scope: !1478)
!1481 = !DILocation(line: 87, column: 2098, scope: !1478)
!1482 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1+EnL0", linkageName: "_CNW7builtin3opsEXp1+EnL0", scope: null, file: !6, line: 4, type: !1483, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1483 = !DISubroutineType(types: !1484)
!1484 = !{!165}
!1485 = !DILocalVariable(name: "val", scope: !1482, file: !1, line: 4, type: !165)
!1486 = !DILocation(line: 4, column: 19, scope: !1482)
!1487 = !DILocation(line: 2, column: 23, scope: !1482)
!1488 = !DILocation(line: 455, column: 18, scope: !1482)
!1489 = distinct !DISubprogram(name: "_CNW7builtinE7id_i128EnL0", linkageName: "_CNW7builtinE7id_i128EnL0", scope: null, file: !6, line: 87, type: !1483, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1490 = !DILocalVariable(name: "val", scope: !1489, file: !1, line: 87, type: !165)
!1491 = !DILocation(line: 87, column: 2044, scope: !1489)
!1492 = !DILocation(line: 87, column: 2098, scope: !1489)
!1493 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1+EoL0", linkageName: "_CNW7builtin3opsEXp1+EoL0", scope: null, file: !6, line: 4, type: !1494, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1494 = !DISubroutineType(types: !1495)
!1495 = !{!181}
!1496 = !DILocalVariable(name: "val", scope: !1493, file: !1, line: 4, type: !181)
!1497 = !DILocation(line: 4, column: 19, scope: !1493)
!1498 = !DILocation(line: 2, column: 23, scope: !1493)
!1499 = !DILocation(line: 455, column: 18, scope: !1493)
!1500 = distinct !DISubprogram(name: "_CNW7builtinE7id_u128EoL0", linkageName: "_CNW7builtinE7id_u128EoL0", scope: null, file: !6, line: 87, type: !1494, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1501 = !DILocalVariable(name: "val", scope: !1500, file: !1, line: 87, type: !181)
!1502 = !DILocation(line: 87, column: 2044, scope: !1500)
!1503 = !DILocation(line: 87, column: 2098, scope: !1500)
!1504 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1-EaL0", linkageName: "_CNW7builtin3opsEXp1-EaL0", scope: null, file: !6, line: 4, type: !1395, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1505 = !DILocalVariable(name: "val", scope: !1504, file: !1, line: 4, type: !37)
!1506 = !DILocation(line: 4, column: 19, scope: !1504)
!1507 = !DILocation(line: 2, column: 23, scope: !1504)
!1508 = !DILocation(line: 458, column: 63, scope: !1504)
!1509 = distinct !DISubprogram(name: "_CNW7builtinE6neg_i8EaL0", linkageName: "_CNW7builtinE6neg_i8EaL0", scope: null, file: !6, line: 87, type: !1395, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1510 = !DILocalVariable(name: "val", scope: !1509, file: !1, line: 87, type: !37)
!1511 = !DILocation(line: 87, column: 1932, scope: !1509)
!1512 = !DILocation(line: 87, column: 1987, scope: !1509)
!1513 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1-EhL0", linkageName: "_CNW7builtin3opsEXp1-EhL0", scope: null, file: !6, line: 4, type: !1406, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1514 = !DILocalVariable(name: "val", scope: !1513, file: !1, line: 4, type: !53)
!1515 = !DILocation(line: 4, column: 19, scope: !1513)
!1516 = !DILocation(line: 2, column: 23, scope: !1513)
!1517 = !DILocation(line: 458, column: 63, scope: !1513)
!1518 = distinct !DISubprogram(name: "_CNW7builtinE6neg_u8EhL0", linkageName: "_CNW7builtinE6neg_u8EhL0", scope: null, file: !6, line: 87, type: !1406, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1519 = !DILocalVariable(name: "val", scope: !1518, file: !1, line: 87, type: !53)
!1520 = !DILocation(line: 87, column: 1932, scope: !1518)
!1521 = !DILocation(line: 87, column: 1987, scope: !1518)
!1522 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1-EsL0", linkageName: "_CNW7builtin3opsEXp1-EsL0", scope: null, file: !6, line: 4, type: !1417, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1523 = !DILocalVariable(name: "val", scope: !1522, file: !1, line: 4, type: !69)
!1524 = !DILocation(line: 4, column: 19, scope: !1522)
!1525 = !DILocation(line: 2, column: 23, scope: !1522)
!1526 = !DILocation(line: 458, column: 63, scope: !1522)
!1527 = distinct !DISubprogram(name: "_CNW7builtinE7neg_i16EsL0", linkageName: "_CNW7builtinE7neg_i16EsL0", scope: null, file: !6, line: 87, type: !1417, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1528 = !DILocalVariable(name: "val", scope: !1527, file: !1, line: 87, type: !69)
!1529 = !DILocation(line: 87, column: 1932, scope: !1527)
!1530 = !DILocation(line: 87, column: 1987, scope: !1527)
!1531 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1-EtL0", linkageName: "_CNW7builtin3opsEXp1-EtL0", scope: null, file: !6, line: 4, type: !1428, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1532 = !DILocalVariable(name: "val", scope: !1531, file: !1, line: 4, type: !85)
!1533 = !DILocation(line: 4, column: 19, scope: !1531)
!1534 = !DILocation(line: 2, column: 23, scope: !1531)
!1535 = !DILocation(line: 458, column: 63, scope: !1531)
!1536 = distinct !DISubprogram(name: "_CNW7builtinE7neg_u16EtL0", linkageName: "_CNW7builtinE7neg_u16EtL0", scope: null, file: !6, line: 87, type: !1428, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1537 = !DILocalVariable(name: "val", scope: !1536, file: !1, line: 87, type: !85)
!1538 = !DILocation(line: 87, column: 1932, scope: !1536)
!1539 = !DILocation(line: 87, column: 1987, scope: !1536)
!1540 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1-EiL0", linkageName: "_CNW7builtin3opsEXp1-EiL0", scope: null, file: !6, line: 4, type: !1439, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1541 = !DILocalVariable(name: "val", scope: !1540, file: !1, line: 4, type: !101)
!1542 = !DILocation(line: 4, column: 19, scope: !1540)
!1543 = !DILocation(line: 2, column: 23, scope: !1540)
!1544 = !DILocation(line: 458, column: 63, scope: !1540)
!1545 = distinct !DISubprogram(name: "_CNW7builtinE7neg_i32EiL0", linkageName: "_CNW7builtinE7neg_i32EiL0", scope: null, file: !6, line: 87, type: !1439, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1546 = !DILocalVariable(name: "val", scope: !1545, file: !1, line: 87, type: !101)
!1547 = !DILocation(line: 87, column: 1932, scope: !1545)
!1548 = !DILocation(line: 87, column: 1987, scope: !1545)
!1549 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1-EjL0", linkageName: "_CNW7builtin3opsEXp1-EjL0", scope: null, file: !6, line: 4, type: !1450, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1550 = !DILocalVariable(name: "val", scope: !1549, file: !1, line: 4, type: !117)
!1551 = !DILocation(line: 4, column: 19, scope: !1549)
!1552 = !DILocation(line: 2, column: 23, scope: !1549)
!1553 = !DILocation(line: 458, column: 63, scope: !1549)
!1554 = distinct !DISubprogram(name: "_CNW7builtinE7neg_u32EjL0", linkageName: "_CNW7builtinE7neg_u32EjL0", scope: null, file: !6, line: 87, type: !1450, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1555 = !DILocalVariable(name: "val", scope: !1554, file: !1, line: 87, type: !117)
!1556 = !DILocation(line: 87, column: 1932, scope: !1554)
!1557 = !DILocation(line: 87, column: 1987, scope: !1554)
!1558 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1-ElL0", linkageName: "_CNW7builtin3opsEXp1-ElL0", scope: null, file: !6, line: 4, type: !1461, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1559 = !DILocalVariable(name: "val", scope: !1558, file: !1, line: 4, type: !133)
!1560 = !DILocation(line: 4, column: 19, scope: !1558)
!1561 = !DILocation(line: 2, column: 23, scope: !1558)
!1562 = !DILocation(line: 458, column: 63, scope: !1558)
!1563 = distinct !DISubprogram(name: "_CNW7builtinE7neg_i64ElL0", linkageName: "_CNW7builtinE7neg_i64ElL0", scope: null, file: !6, line: 87, type: !1461, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1564 = !DILocalVariable(name: "val", scope: !1563, file: !1, line: 87, type: !133)
!1565 = !DILocation(line: 87, column: 1932, scope: !1563)
!1566 = !DILocation(line: 87, column: 1987, scope: !1563)
!1567 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1-EmL0", linkageName: "_CNW7builtin3opsEXp1-EmL0", scope: null, file: !6, line: 4, type: !1472, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1568 = !DILocalVariable(name: "val", scope: !1567, file: !1, line: 4, type: !149)
!1569 = !DILocation(line: 4, column: 19, scope: !1567)
!1570 = !DILocation(line: 2, column: 23, scope: !1567)
!1571 = !DILocation(line: 458, column: 63, scope: !1567)
!1572 = distinct !DISubprogram(name: "_CNW7builtinE7neg_u64EmL0", linkageName: "_CNW7builtinE7neg_u64EmL0", scope: null, file: !6, line: 87, type: !1472, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1573 = !DILocalVariable(name: "val", scope: !1572, file: !1, line: 87, type: !149)
!1574 = !DILocation(line: 87, column: 1932, scope: !1572)
!1575 = !DILocation(line: 87, column: 1987, scope: !1572)
!1576 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1-EnL0", linkageName: "_CNW7builtin3opsEXp1-EnL0", scope: null, file: !6, line: 4, type: !1483, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1577 = !DILocalVariable(name: "val", scope: !1576, file: !1, line: 4, type: !165)
!1578 = !DILocation(line: 4, column: 19, scope: !1576)
!1579 = !DILocation(line: 2, column: 23, scope: !1576)
!1580 = !DILocation(line: 458, column: 63, scope: !1576)
!1581 = distinct !DISubprogram(name: "_CNW7builtinE8neg_i128EnL0", linkageName: "_CNW7builtinE8neg_i128EnL0", scope: null, file: !6, line: 87, type: !1483, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1582 = !DILocalVariable(name: "val", scope: !1581, file: !1, line: 87, type: !165)
!1583 = !DILocation(line: 87, column: 1932, scope: !1581)
!1584 = !DILocation(line: 87, column: 1987, scope: !1581)
!1585 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1-EoL0", linkageName: "_CNW7builtin3opsEXp1-EoL0", scope: null, file: !6, line: 4, type: !1494, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1586 = !DILocalVariable(name: "val", scope: !1585, file: !1, line: 4, type: !181)
!1587 = !DILocation(line: 4, column: 19, scope: !1585)
!1588 = !DILocation(line: 2, column: 23, scope: !1585)
!1589 = !DILocation(line: 458, column: 63, scope: !1585)
!1590 = distinct !DISubprogram(name: "_CNW7builtinE8neg_u128EoL0", linkageName: "_CNW7builtinE8neg_u128EoL0", scope: null, file: !6, line: 87, type: !1494, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1591 = !DILocalVariable(name: "val", scope: !1590, file: !1, line: 87, type: !181)
!1592 = !DILocation(line: 87, column: 1932, scope: !1590)
!1593 = !DILocation(line: 87, column: 1987, scope: !1590)
!1594 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1!EbL0", linkageName: "_CNW7builtin3opsEXp1!EbL0", scope: null, file: !6, line: 4, type: !1595, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1595 = !DISubroutineType(types: !1596)
!1596 = !{!1597}
!1597 = !DIBasicType(name: "i1", size: 8, encoding: DW_ATE_boolean)
!1598 = !DILocalVariable(name: "val", scope: !1594, file: !1, line: 4, type: !1597)
!1599 = !DILocation(line: 4, column: 19, scope: !1594)
!1600 = !DILocation(line: 2, column: 23, scope: !1594)
!1601 = !DILocation(line: 462, column: 11, scope: !1594)
!1602 = distinct !DISubprogram(name: "_CNW7builtinE6not_i1EbL0", linkageName: "_CNW7builtinE6not_i1EbL0", scope: null, file: !6, line: 87, type: !1595, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1603 = !DILocalVariable(name: "val", scope: !1602, file: !1, line: 87, type: !1597)
!1604 = !DILocation(line: 87, column: 1820, scope: !1602)
!1605 = !DILocation(line: 87, column: 1875, scope: !1602)
!1606 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1~EaL0", linkageName: "_CNW7builtin3opsEXp1~EaL0", scope: null, file: !6, line: 4, type: !1395, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1607 = !DILocalVariable(name: "val", scope: !1606, file: !1, line: 4, type: !37)
!1608 = !DILocation(line: 4, column: 19, scope: !1606)
!1609 = !DILocation(line: 2, column: 23, scope: !1606)
!1610 = !DILocation(line: 464, column: 44, scope: !1606)
!1611 = distinct !DISubprogram(name: "_CNW7builtinE6not_i8EaL0", linkageName: "_CNW7builtinE6not_i8EaL0", scope: null, file: !6, line: 87, type: !1395, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1612 = !DILocalVariable(name: "val", scope: !1611, file: !1, line: 87, type: !37)
!1613 = !DILocation(line: 87, column: 1820, scope: !1611)
!1614 = !DILocation(line: 87, column: 1875, scope: !1611)
!1615 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1~EhL0", linkageName: "_CNW7builtin3opsEXp1~EhL0", scope: null, file: !6, line: 4, type: !1406, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1616 = !DILocalVariable(name: "val", scope: !1615, file: !1, line: 4, type: !53)
!1617 = !DILocation(line: 4, column: 19, scope: !1615)
!1618 = !DILocation(line: 2, column: 23, scope: !1615)
!1619 = !DILocation(line: 464, column: 44, scope: !1615)
!1620 = distinct !DISubprogram(name: "_CNW7builtinE6not_u8EhL0", linkageName: "_CNW7builtinE6not_u8EhL0", scope: null, file: !6, line: 87, type: !1406, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1621 = !DILocalVariable(name: "val", scope: !1620, file: !1, line: 87, type: !53)
!1622 = !DILocation(line: 87, column: 1820, scope: !1620)
!1623 = !DILocation(line: 87, column: 1875, scope: !1620)
!1624 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1~EsL0", linkageName: "_CNW7builtin3opsEXp1~EsL0", scope: null, file: !6, line: 4, type: !1417, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1625 = !DILocalVariable(name: "val", scope: !1624, file: !1, line: 4, type: !69)
!1626 = !DILocation(line: 4, column: 19, scope: !1624)
!1627 = !DILocation(line: 2, column: 23, scope: !1624)
!1628 = !DILocation(line: 464, column: 44, scope: !1624)
!1629 = distinct !DISubprogram(name: "_CNW7builtinE7not_i16EsL0", linkageName: "_CNW7builtinE7not_i16EsL0", scope: null, file: !6, line: 87, type: !1417, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1630 = !DILocalVariable(name: "val", scope: !1629, file: !1, line: 87, type: !69)
!1631 = !DILocation(line: 87, column: 1820, scope: !1629)
!1632 = !DILocation(line: 87, column: 1875, scope: !1629)
!1633 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1~EtL0", linkageName: "_CNW7builtin3opsEXp1~EtL0", scope: null, file: !6, line: 4, type: !1428, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1634 = !DILocalVariable(name: "val", scope: !1633, file: !1, line: 4, type: !85)
!1635 = !DILocation(line: 4, column: 19, scope: !1633)
!1636 = !DILocation(line: 2, column: 23, scope: !1633)
!1637 = !DILocation(line: 464, column: 44, scope: !1633)
!1638 = distinct !DISubprogram(name: "_CNW7builtinE7not_u16EtL0", linkageName: "_CNW7builtinE7not_u16EtL0", scope: null, file: !6, line: 87, type: !1428, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1639 = !DILocalVariable(name: "val", scope: !1638, file: !1, line: 87, type: !85)
!1640 = !DILocation(line: 87, column: 1820, scope: !1638)
!1641 = !DILocation(line: 87, column: 1875, scope: !1638)
!1642 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1~EiL0", linkageName: "_CNW7builtin3opsEXp1~EiL0", scope: null, file: !6, line: 4, type: !1439, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1643 = !DILocalVariable(name: "val", scope: !1642, file: !1, line: 4, type: !101)
!1644 = !DILocation(line: 4, column: 19, scope: !1642)
!1645 = !DILocation(line: 2, column: 23, scope: !1642)
!1646 = !DILocation(line: 464, column: 44, scope: !1642)
!1647 = distinct !DISubprogram(name: "_CNW7builtinE7not_i32EiL0", linkageName: "_CNW7builtinE7not_i32EiL0", scope: null, file: !6, line: 87, type: !1439, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1648 = !DILocalVariable(name: "val", scope: !1647, file: !1, line: 87, type: !101)
!1649 = !DILocation(line: 87, column: 1820, scope: !1647)
!1650 = !DILocation(line: 87, column: 1875, scope: !1647)
!1651 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1~EjL0", linkageName: "_CNW7builtin3opsEXp1~EjL0", scope: null, file: !6, line: 4, type: !1450, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1652 = !DILocalVariable(name: "val", scope: !1651, file: !1, line: 4, type: !117)
!1653 = !DILocation(line: 4, column: 19, scope: !1651)
!1654 = !DILocation(line: 2, column: 23, scope: !1651)
!1655 = !DILocation(line: 464, column: 44, scope: !1651)
!1656 = distinct !DISubprogram(name: "_CNW7builtinE7not_u32EjL0", linkageName: "_CNW7builtinE7not_u32EjL0", scope: null, file: !6, line: 87, type: !1450, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1657 = !DILocalVariable(name: "val", scope: !1656, file: !1, line: 87, type: !117)
!1658 = !DILocation(line: 87, column: 1820, scope: !1656)
!1659 = !DILocation(line: 87, column: 1875, scope: !1656)
!1660 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1~ElL0", linkageName: "_CNW7builtin3opsEXp1~ElL0", scope: null, file: !6, line: 4, type: !1461, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1661 = !DILocalVariable(name: "val", scope: !1660, file: !1, line: 4, type: !133)
!1662 = !DILocation(line: 4, column: 19, scope: !1660)
!1663 = !DILocation(line: 2, column: 23, scope: !1660)
!1664 = !DILocation(line: 464, column: 44, scope: !1660)
!1665 = distinct !DISubprogram(name: "_CNW7builtinE7not_i64ElL0", linkageName: "_CNW7builtinE7not_i64ElL0", scope: null, file: !6, line: 87, type: !1461, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1666 = !DILocalVariable(name: "val", scope: !1665, file: !1, line: 87, type: !133)
!1667 = !DILocation(line: 87, column: 1820, scope: !1665)
!1668 = !DILocation(line: 87, column: 1875, scope: !1665)
!1669 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1~EmL0", linkageName: "_CNW7builtin3opsEXp1~EmL0", scope: null, file: !6, line: 4, type: !1472, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1670 = !DILocalVariable(name: "val", scope: !1669, file: !1, line: 4, type: !149)
!1671 = !DILocation(line: 4, column: 19, scope: !1669)
!1672 = !DILocation(line: 2, column: 23, scope: !1669)
!1673 = !DILocation(line: 464, column: 44, scope: !1669)
!1674 = distinct !DISubprogram(name: "_CNW7builtinE7not_u64EmL0", linkageName: "_CNW7builtinE7not_u64EmL0", scope: null, file: !6, line: 87, type: !1472, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1675 = !DILocalVariable(name: "val", scope: !1674, file: !1, line: 87, type: !149)
!1676 = !DILocation(line: 87, column: 1820, scope: !1674)
!1677 = !DILocation(line: 87, column: 1875, scope: !1674)
!1678 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1~EnL0", linkageName: "_CNW7builtin3opsEXp1~EnL0", scope: null, file: !6, line: 4, type: !1483, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1679 = !DILocalVariable(name: "val", scope: !1678, file: !1, line: 4, type: !165)
!1680 = !DILocation(line: 4, column: 19, scope: !1678)
!1681 = !DILocation(line: 2, column: 23, scope: !1678)
!1682 = !DILocation(line: 464, column: 44, scope: !1678)
!1683 = distinct !DISubprogram(name: "_CNW7builtinE8not_i128EnL0", linkageName: "_CNW7builtinE8not_i128EnL0", scope: null, file: !6, line: 87, type: !1483, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1684 = !DILocalVariable(name: "val", scope: !1683, file: !1, line: 87, type: !165)
!1685 = !DILocation(line: 87, column: 1820, scope: !1683)
!1686 = !DILocation(line: 87, column: 1875, scope: !1683)
!1687 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1~EoL0", linkageName: "_CNW7builtin3opsEXp1~EoL0", scope: null, file: !6, line: 4, type: !1494, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1688 = !DILocalVariable(name: "val", scope: !1687, file: !1, line: 4, type: !181)
!1689 = !DILocation(line: 4, column: 19, scope: !1687)
!1690 = !DILocation(line: 2, column: 23, scope: !1687)
!1691 = !DILocation(line: 464, column: 44, scope: !1687)
!1692 = distinct !DISubprogram(name: "_CNW7builtinE8not_u128EoL0", linkageName: "_CNW7builtinE8not_u128EoL0", scope: null, file: !6, line: 87, type: !1494, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1693 = !DILocalVariable(name: "val", scope: !1692, file: !1, line: 87, type: !181)
!1694 = !DILocation(line: 87, column: 1820, scope: !1692)
!1695 = !DILocation(line: 87, column: 1875, scope: !1692)
!1696 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1+EffL0L0", linkageName: "_CNW7builtin3opsEXi1+EffL0L0", scope: null, file: !6, line: 4, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1697 = !DISubroutineType(types: !1698)
!1698 = !{!1699, !1699}
!1699 = !DIBasicType(name: "f32", size: 32, encoding: DW_ATE_float)
!1700 = !DILocalVariable(name: "lhs", scope: !1696, file: !1, line: 4, type: !1699)
!1701 = !DILocation(line: 4, column: 19, scope: !1696)
!1702 = !DILocalVariable(name: "rhs", scope: !1696, file: !1, line: 4, type: !1699)
!1703 = !DILocation(line: 5, column: 26, scope: !1696)
!1704 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2+=EffL0L0", linkageName: "_CNW7builtin3opsEXi2+=EffL0L0", scope: null, file: !6, line: 6, type: !1705, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1705 = !DISubroutineType(types: !1706)
!1706 = !{!1707, !1699}
!1707 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !1699, size: 64, align: 8)
!1708 = !DILocalVariable(name: "lhs", scope: !1704, file: !1, line: 6, type: !1699)
!1709 = !DILocation(line: 6, column: 18, scope: !1704)
!1710 = !DILocalVariable(name: "rhs", scope: !1704, file: !1, line: 6, type: !1699)
!1711 = !DILocation(line: 9, column: 30, scope: !1704)
!1712 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1+EddL0L0", linkageName: "_CNW7builtin3opsEXi1+EddL0L0", scope: null, file: !6, line: 4, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1713 = !DISubroutineType(types: !1714)
!1714 = !{!1715, !1715}
!1715 = !DIBasicType(name: "f64", size: 64, encoding: DW_ATE_float)
!1716 = !DILocalVariable(name: "lhs", scope: !1712, file: !1, line: 4, type: !1715)
!1717 = !DILocation(line: 4, column: 19, scope: !1712)
!1718 = !DILocalVariable(name: "rhs", scope: !1712, file: !1, line: 4, type: !1715)
!1719 = !DILocation(line: 5, column: 26, scope: !1712)
!1720 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2+=EddL0L0", linkageName: "_CNW7builtin3opsEXi2+=EddL0L0", scope: null, file: !6, line: 6, type: !1721, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1721 = !DISubroutineType(types: !1722)
!1722 = !{!1723, !1715}
!1723 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !1715, size: 64, align: 8)
!1724 = !DILocalVariable(name: "lhs", scope: !1720, file: !1, line: 6, type: !1715)
!1725 = !DILocation(line: 6, column: 18, scope: !1720)
!1726 = !DILocalVariable(name: "rhs", scope: !1720, file: !1, line: 6, type: !1715)
!1727 = !DILocation(line: 9, column: 30, scope: !1720)
!1728 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1-EffL0L0", linkageName: "_CNW7builtin3opsEXi1-EffL0L0", scope: null, file: !6, line: 4, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1729 = !DILocalVariable(name: "lhs", scope: !1728, file: !1, line: 4, type: !1699)
!1730 = !DILocation(line: 4, column: 19, scope: !1728)
!1731 = !DILocalVariable(name: "rhs", scope: !1728, file: !1, line: 4, type: !1699)
!1732 = !DILocation(line: 5, column: 26, scope: !1728)
!1733 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2-=EffL0L0", linkageName: "_CNW7builtin3opsEXi2-=EffL0L0", scope: null, file: !6, line: 6, type: !1705, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1734 = !DILocalVariable(name: "lhs", scope: !1733, file: !1, line: 6, type: !1699)
!1735 = !DILocation(line: 6, column: 18, scope: !1733)
!1736 = !DILocalVariable(name: "rhs", scope: !1733, file: !1, line: 6, type: !1699)
!1737 = !DILocation(line: 9, column: 30, scope: !1733)
!1738 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1-EddL0L0", linkageName: "_CNW7builtin3opsEXi1-EddL0L0", scope: null, file: !6, line: 4, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1739 = !DILocalVariable(name: "lhs", scope: !1738, file: !1, line: 4, type: !1715)
!1740 = !DILocation(line: 4, column: 19, scope: !1738)
!1741 = !DILocalVariable(name: "rhs", scope: !1738, file: !1, line: 4, type: !1715)
!1742 = !DILocation(line: 5, column: 26, scope: !1738)
!1743 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2-=EddL0L0", linkageName: "_CNW7builtin3opsEXi2-=EddL0L0", scope: null, file: !6, line: 6, type: !1721, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1744 = !DILocalVariable(name: "lhs", scope: !1743, file: !1, line: 6, type: !1715)
!1745 = !DILocation(line: 6, column: 18, scope: !1743)
!1746 = !DILocalVariable(name: "rhs", scope: !1743, file: !1, line: 6, type: !1715)
!1747 = !DILocation(line: 9, column: 30, scope: !1743)
!1748 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1*EffL0L0", linkageName: "_CNW7builtin3opsEXi1*EffL0L0", scope: null, file: !6, line: 4, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1749 = !DILocalVariable(name: "lhs", scope: !1748, file: !1, line: 4, type: !1699)
!1750 = !DILocation(line: 4, column: 19, scope: !1748)
!1751 = !DILocalVariable(name: "rhs", scope: !1748, file: !1, line: 4, type: !1699)
!1752 = !DILocation(line: 5, column: 26, scope: !1748)
!1753 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2*=EffL0L0", linkageName: "_CNW7builtin3opsEXi2*=EffL0L0", scope: null, file: !6, line: 6, type: !1705, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1754 = !DILocalVariable(name: "lhs", scope: !1753, file: !1, line: 6, type: !1699)
!1755 = !DILocation(line: 6, column: 18, scope: !1753)
!1756 = !DILocalVariable(name: "rhs", scope: !1753, file: !1, line: 6, type: !1699)
!1757 = !DILocation(line: 9, column: 30, scope: !1753)
!1758 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1*EddL0L0", linkageName: "_CNW7builtin3opsEXi1*EddL0L0", scope: null, file: !6, line: 4, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1759 = !DILocalVariable(name: "lhs", scope: !1758, file: !1, line: 4, type: !1715)
!1760 = !DILocation(line: 4, column: 19, scope: !1758)
!1761 = !DILocalVariable(name: "rhs", scope: !1758, file: !1, line: 4, type: !1715)
!1762 = !DILocation(line: 5, column: 26, scope: !1758)
!1763 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2*=EddL0L0", linkageName: "_CNW7builtin3opsEXi2*=EddL0L0", scope: null, file: !6, line: 6, type: !1721, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1764 = !DILocalVariable(name: "lhs", scope: !1763, file: !1, line: 6, type: !1715)
!1765 = !DILocation(line: 6, column: 18, scope: !1763)
!1766 = !DILocalVariable(name: "rhs", scope: !1763, file: !1, line: 6, type: !1715)
!1767 = !DILocation(line: 9, column: 30, scope: !1763)
!1768 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1/EffL0L0", linkageName: "_CNW7builtin3opsEXi1/EffL0L0", scope: null, file: !6, line: 4, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1769 = !DILocalVariable(name: "lhs", scope: !1768, file: !1, line: 4, type: !1699)
!1770 = !DILocation(line: 4, column: 19, scope: !1768)
!1771 = !DILocalVariable(name: "rhs", scope: !1768, file: !1, line: 4, type: !1699)
!1772 = !DILocation(line: 5, column: 26, scope: !1768)
!1773 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2/=EffL0L0", linkageName: "_CNW7builtin3opsEXi2/=EffL0L0", scope: null, file: !6, line: 6, type: !1705, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1774 = !DILocalVariable(name: "lhs", scope: !1773, file: !1, line: 6, type: !1699)
!1775 = !DILocation(line: 6, column: 18, scope: !1773)
!1776 = !DILocalVariable(name: "rhs", scope: !1773, file: !1, line: 6, type: !1699)
!1777 = !DILocation(line: 9, column: 30, scope: !1773)
!1778 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1/EddL0L0", linkageName: "_CNW7builtin3opsEXi1/EddL0L0", scope: null, file: !6, line: 4, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1779 = !DILocalVariable(name: "lhs", scope: !1778, file: !1, line: 4, type: !1715)
!1780 = !DILocation(line: 4, column: 19, scope: !1778)
!1781 = !DILocalVariable(name: "rhs", scope: !1778, file: !1, line: 4, type: !1715)
!1782 = !DILocation(line: 5, column: 26, scope: !1778)
!1783 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2/=EddL0L0", linkageName: "_CNW7builtin3opsEXi2/=EddL0L0", scope: null, file: !6, line: 6, type: !1721, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1784 = !DILocalVariable(name: "lhs", scope: !1783, file: !1, line: 6, type: !1715)
!1785 = !DILocation(line: 6, column: 18, scope: !1783)
!1786 = !DILocalVariable(name: "rhs", scope: !1783, file: !1, line: 6, type: !1715)
!1787 = !DILocation(line: 9, column: 30, scope: !1783)
!1788 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1%EffL0L0", linkageName: "_CNW7builtin3opsEXi1%EffL0L0", scope: null, file: !6, line: 4, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1789 = !DILocalVariable(name: "lhs", scope: !1788, file: !1, line: 4, type: !1699)
!1790 = !DILocation(line: 4, column: 19, scope: !1788)
!1791 = !DILocalVariable(name: "rhs", scope: !1788, file: !1, line: 4, type: !1699)
!1792 = !DILocation(line: 5, column: 26, scope: !1788)
!1793 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2%=EffL0L0", linkageName: "_CNW7builtin3opsEXi2%=EffL0L0", scope: null, file: !6, line: 6, type: !1705, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1794 = !DILocalVariable(name: "lhs", scope: !1793, file: !1, line: 6, type: !1699)
!1795 = !DILocation(line: 6, column: 18, scope: !1793)
!1796 = !DILocalVariable(name: "rhs", scope: !1793, file: !1, line: 6, type: !1699)
!1797 = !DILocation(line: 9, column: 30, scope: !1793)
!1798 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1%EddL0L0", linkageName: "_CNW7builtin3opsEXi1%EddL0L0", scope: null, file: !6, line: 4, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1799 = !DILocalVariable(name: "lhs", scope: !1798, file: !1, line: 4, type: !1715)
!1800 = !DILocation(line: 4, column: 19, scope: !1798)
!1801 = !DILocalVariable(name: "rhs", scope: !1798, file: !1, line: 4, type: !1715)
!1802 = !DILocation(line: 5, column: 26, scope: !1798)
!1803 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2%=EddL0L0", linkageName: "_CNW7builtin3opsEXi2%=EddL0L0", scope: null, file: !6, line: 6, type: !1721, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1804 = !DILocalVariable(name: "lhs", scope: !1803, file: !1, line: 6, type: !1715)
!1805 = !DILocation(line: 6, column: 18, scope: !1803)
!1806 = !DILocalVariable(name: "rhs", scope: !1803, file: !1, line: 6, type: !1715)
!1807 = !DILocation(line: 9, column: 30, scope: !1803)
!1808 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2==EffL0L0", linkageName: "_CNW7builtin3opsEXi2==EffL0L0", scope: null, file: !6, line: 4, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1809 = !DILocalVariable(name: "lhs", scope: !1808, file: !1, line: 4, type: !1699)
!1810 = !DILocation(line: 4, column: 27, scope: !1808)
!1811 = !DILocalVariable(name: "rhs", scope: !1808, file: !1, line: 4, type: !1699)
!1812 = !DILocation(line: 2, column: 5, scope: !1808)
!1813 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2==EddL0L0", linkageName: "_CNW7builtin3opsEXi2==EddL0L0", scope: null, file: !6, line: 4, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1814 = !DILocalVariable(name: "lhs", scope: !1813, file: !1, line: 4, type: !1715)
!1815 = !DILocation(line: 4, column: 27, scope: !1813)
!1816 = !DILocalVariable(name: "rhs", scope: !1813, file: !1, line: 4, type: !1715)
!1817 = !DILocation(line: 2, column: 5, scope: !1813)
!1818 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2!=EffL0L0", linkageName: "_CNW7builtin3opsEXi2!=EffL0L0", scope: null, file: !6, line: 4, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1819 = !DILocalVariable(name: "lhs", scope: !1818, file: !1, line: 4, type: !1699)
!1820 = !DILocation(line: 4, column: 27, scope: !1818)
!1821 = !DILocalVariable(name: "rhs", scope: !1818, file: !1, line: 4, type: !1699)
!1822 = !DILocation(line: 2, column: 5, scope: !1818)
!1823 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2!=EddL0L0", linkageName: "_CNW7builtin3opsEXi2!=EddL0L0", scope: null, file: !6, line: 4, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1824 = !DILocalVariable(name: "lhs", scope: !1823, file: !1, line: 4, type: !1715)
!1825 = !DILocation(line: 4, column: 27, scope: !1823)
!1826 = !DILocalVariable(name: "rhs", scope: !1823, file: !1, line: 4, type: !1715)
!1827 = !DILocation(line: 2, column: 5, scope: !1823)
!1828 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>=EffL0L0", linkageName: "_CNW7builtin3opsEXi2>=EffL0L0", scope: null, file: !6, line: 4, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1829 = !DILocalVariable(name: "lhs", scope: !1828, file: !1, line: 4, type: !1699)
!1830 = !DILocation(line: 4, column: 27, scope: !1828)
!1831 = !DILocalVariable(name: "rhs", scope: !1828, file: !1, line: 4, type: !1699)
!1832 = !DILocation(line: 2, column: 5, scope: !1828)
!1833 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2>=EddL0L0", linkageName: "_CNW7builtin3opsEXi2>=EddL0L0", scope: null, file: !6, line: 4, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1834 = !DILocalVariable(name: "lhs", scope: !1833, file: !1, line: 4, type: !1715)
!1835 = !DILocation(line: 4, column: 27, scope: !1833)
!1836 = !DILocalVariable(name: "rhs", scope: !1833, file: !1, line: 4, type: !1715)
!1837 = !DILocation(line: 2, column: 5, scope: !1833)
!1838 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1>EffL0L0", linkageName: "_CNW7builtin3opsEXi1>EffL0L0", scope: null, file: !6, line: 4, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1839 = !DILocalVariable(name: "lhs", scope: !1838, file: !1, line: 4, type: !1699)
!1840 = !DILocation(line: 4, column: 27, scope: !1838)
!1841 = !DILocalVariable(name: "rhs", scope: !1838, file: !1, line: 4, type: !1699)
!1842 = !DILocation(line: 2, column: 5, scope: !1838)
!1843 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1>EddL0L0", linkageName: "_CNW7builtin3opsEXi1>EddL0L0", scope: null, file: !6, line: 4, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1844 = !DILocalVariable(name: "lhs", scope: !1843, file: !1, line: 4, type: !1715)
!1845 = !DILocation(line: 4, column: 27, scope: !1843)
!1846 = !DILocalVariable(name: "rhs", scope: !1843, file: !1, line: 4, type: !1715)
!1847 = !DILocation(line: 2, column: 5, scope: !1843)
!1848 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<=EffL0L0", linkageName: "_CNW7builtin3opsEXi2<=EffL0L0", scope: null, file: !6, line: 4, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1849 = !DILocalVariable(name: "lhs", scope: !1848, file: !1, line: 4, type: !1699)
!1850 = !DILocation(line: 4, column: 27, scope: !1848)
!1851 = !DILocalVariable(name: "rhs", scope: !1848, file: !1, line: 4, type: !1699)
!1852 = !DILocation(line: 2, column: 5, scope: !1848)
!1853 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi2<=EddL0L0", linkageName: "_CNW7builtin3opsEXi2<=EddL0L0", scope: null, file: !6, line: 4, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1854 = !DILocalVariable(name: "lhs", scope: !1853, file: !1, line: 4, type: !1715)
!1855 = !DILocation(line: 4, column: 27, scope: !1853)
!1856 = !DILocalVariable(name: "rhs", scope: !1853, file: !1, line: 4, type: !1715)
!1857 = !DILocation(line: 2, column: 5, scope: !1853)
!1858 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1<EffL0L0", linkageName: "_CNW7builtin3opsEXi1<EffL0L0", scope: null, file: !6, line: 4, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1859 = !DILocalVariable(name: "lhs", scope: !1858, file: !1, line: 4, type: !1699)
!1860 = !DILocation(line: 4, column: 27, scope: !1858)
!1861 = !DILocalVariable(name: "rhs", scope: !1858, file: !1, line: 4, type: !1699)
!1862 = !DILocation(line: 2, column: 5, scope: !1858)
!1863 = distinct !DISubprogram(name: "_CNW7builtin3opsEXi1<EddL0L0", linkageName: "_CNW7builtin3opsEXi1<EddL0L0", scope: null, file: !6, line: 4, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1864 = !DILocalVariable(name: "lhs", scope: !1863, file: !1, line: 4, type: !1715)
!1865 = !DILocation(line: 4, column: 27, scope: !1863)
!1866 = !DILocalVariable(name: "rhs", scope: !1863, file: !1, line: 4, type: !1715)
!1867 = !DILocation(line: 2, column: 5, scope: !1863)
!1868 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1+EfL0", linkageName: "_CNW7builtin3opsEXp1+EfL0", scope: null, file: !6, line: 4, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1869 = !DISubroutineType(types: !1870)
!1870 = !{!1699}
!1871 = !DILocalVariable(name: "val", scope: !1868, file: !1, line: 4, type: !1699)
!1872 = !DILocation(line: 4, column: 19, scope: !1868)
!1873 = !DILocation(line: 2, column: 23, scope: !1868)
!1874 = !DILocation(line: 533, column: 34, scope: !1868)
!1875 = distinct !DISubprogram(name: "_CNW7builtinE6id_f32EfL0", linkageName: "_CNW7builtinE6id_f32EfL0", scope: null, file: !6, line: 42, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1876 = !DILocalVariable(name: "val", scope: !1875, file: !1, line: 42, type: !1699)
!1877 = !DILocation(line: 42, column: 84, scope: !1875)
!1878 = !DILocation(line: 43, column: 53, scope: !1875)
!1879 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1+EdL0", linkageName: "_CNW7builtin3opsEXp1+EdL0", scope: null, file: !6, line: 4, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1880 = !DISubroutineType(types: !1881)
!1881 = !{!1715}
!1882 = !DILocalVariable(name: "val", scope: !1879, file: !1, line: 4, type: !1715)
!1883 = !DILocation(line: 4, column: 19, scope: !1879)
!1884 = !DILocation(line: 2, column: 23, scope: !1879)
!1885 = !DILocation(line: 533, column: 34, scope: !1879)
!1886 = distinct !DISubprogram(name: "_CNW7builtinE6id_f64EdL0", linkageName: "_CNW7builtinE6id_f64EdL0", scope: null, file: !6, line: 42, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1887 = !DILocalVariable(name: "val", scope: !1886, file: !1, line: 42, type: !1715)
!1888 = !DILocation(line: 42, column: 84, scope: !1886)
!1889 = !DILocation(line: 43, column: 53, scope: !1886)
!1890 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1-EfL0", linkageName: "_CNW7builtin3opsEXp1-EfL0", scope: null, file: !6, line: 4, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1891 = !DILocalVariable(name: "val", scope: !1890, file: !1, line: 4, type: !1699)
!1892 = !DILocation(line: 4, column: 19, scope: !1890)
!1893 = !DILocation(line: 5, column: 14, scope: !1890)
!1894 = distinct !DISubprogram(name: "_CNW7builtin3opsEXp1-EdL0", linkageName: "_CNW7builtin3opsEXp1-EdL0", scope: null, file: !6, line: 4, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1895 = !DILocalVariable(name: "val", scope: !1894, file: !1, line: 4, type: !1715)
!1896 = !DILocation(line: 4, column: 19, scope: !1894)
!1897 = !DILocation(line: 5, column: 14, scope: !1894)
!1898 = distinct !DISubprogram(name: "_CNW7builtinE14_Builtin_NeverD0Ev", linkageName: "_CNW7builtinE14_Builtin_NeverD0Ev", scope: !0, file: !17, line: 119, type: !1899, isLocal: false, isDefinition: true, scopeLine: 119, thisAdjustment: 1, isOptimized: false, unit: !0)
!1899 = !DISubroutineType(types: !1900)
!1900 = !{!1901}
!1901 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !1902, size: 64, align: 8)
!1902 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "builtin._Builtin_Never", file: !17, line: 119, flags: DIFlagFwdDecl, elements: !2)
!1903 = !DILocation(line: 119, column: 2, scope: !1898)
!1904 = distinct !DISubprogram(name: "_CNW7builtinE14_Builtin_Never4copyES_L0", linkageName: "_CNW7builtinE14_Builtin_Never4copyES_L0", scope: !0, file: !17, line: 119, type: !1905, isLocal: false, isDefinition: true, scopeLine: 119, thisAdjustment: 1, isOptimized: false, unit: !0)
!1905 = !DISubroutineType(types: !1906)
!1906 = !{!1902}
!1907 = !DILocation(line: 119, column: 2, scope: !1904)
!1908 = distinct !DISubprogram(name: "_CNW7builtinE14_Builtin_NeverXi2==ES_S_L0L0", linkageName: "_CNW7builtinE14_Builtin_NeverXi2==ES_S_L0L0", scope: !0, file: !17, line: 119, type: !1909, isLocal: false, isDefinition: true, scopeLine: 119, thisAdjustment: 1, isOptimized: false, unit: !0)
!1909 = !DISubroutineType(types: !1910)
!1910 = !{!1902, !1902}
!1911 = !DILocation(line: 119, column: 2, scope: !1908)
!1912 = distinct !DISubprogram(name: "_CNW7builtinE14_Builtin_NeverXi2!=ES_S_L0L0", linkageName: "_CNW7builtinE14_Builtin_NeverXi2!=ES_S_L0L0", scope: !0, file: !1913, line: 2, type: !1909, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!1913 = !DIFile(filename: "mod.dotm", directory: "/Users/Jonas/CDot/StdLib/builtin/")
!1914 = !DILocalVariable(name: "self", scope: !1912, file: !1, line: 2, type: !1902)
!1915 = !DILocation(line: 2, column: 1, scope: !1912)
!1916 = !DILocalVariable(name: "rhs", scope: !1912, file: !1, line: 2, type: !1902)
!1917 = distinct !DISubprogram(name: "_CNW7builtinE14_Builtin_NeverC1ElL8rawValue", linkageName: "_CNW7builtinE14_Builtin_NeverC1ElL8rawValue", scope: !0, file: !17, line: 119, type: !1918, isLocal: false, isDefinition: true, scopeLine: 119, thisAdjustment: 1, isOptimized: false, unit: !0)
!1918 = !DISubroutineType(types: !1919)
!1919 = !{!1901, !133}
!1920 = !DILocation(line: 119, column: 2, scope: !1917)
!1921 = distinct !DISubprogram(name: "_CNW7builtinE14_Builtin_NeverGa8rawValueES_L0", linkageName: "_CNW7builtinE14_Builtin_NeverGa8rawValueES_L0", scope: !0, file: !17, line: 119, type: !1905, isLocal: false, isDefinition: true, scopeLine: 119, thisAdjustment: 1, isOptimized: false, unit: !0)
!1922 = !DILocation(line: 119, column: 2, scope: !1921)
!1923 = distinct !DISubprogram(name: "_CNW7builtinE14_Builtin_NeverD3Ev", linkageName: "_CNW7builtinE14_Builtin_NeverD3Ev", scope: null, file: !17, line: 119, type: !1899, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1924 = !DILocation(line: 119, column: 2, scope: !1923)
!1925 = distinct !DISubprogram(name: "_CNW7builtinE4BoolXp1!ES0_L0", linkageName: "_CNW7builtinE4BoolXp1!ES0_L0", scope: !0, file: !1913, line: 2, type: !1926, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!1926 = !DISubroutineType(types: !1927)
!1927 = !{!1928}
!1928 = !DICompositeType(tag: DW_TAG_structure_type, name: "builtin.<private name>.Bool", file: !1913, line: 2, flags: DIFlagFwdDecl, elements: !1929)
!1929 = !{!1930}
!1930 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !1928, file: !1913, line: 2, baseType: !1597, size: 8, align: 8)
!1931 = !DILocalVariable(name: "self", scope: !1925, file: !1, line: 2, type: !1928)
!1932 = !DILocation(line: 2, column: 1, scope: !1925)
!1933 = distinct !DISubprogram(name: "_CNW7builtinE4BoolD0Ev", linkageName: "_CNW7builtinE4BoolD0Ev", scope: !0, file: !1913, line: 2, type: !1934, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!1934 = !DISubroutineType(types: !1935)
!1935 = !{!1936}
!1936 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !1928, size: 64, align: 8)
!1937 = !DILocation(line: 2, column: 1, scope: !1933)
!1938 = distinct !DISubprogram(name: "_CNW7builtinE14AtomicRMWBinOpD0Ev", linkageName: "_CNW7builtinE14AtomicRMWBinOpD0Ev", scope: !0, file: !17, line: 504, type: !1939, isLocal: false, isDefinition: true, scopeLine: 504, thisAdjustment: 1, isOptimized: false, unit: !0)
!1939 = !DISubroutineType(types: !1940)
!1940 = !{!1941}
!1941 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !1942, size: 64, align: 8)
!1942 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "builtin.AtomicRMWBinOp", file: !17, line: 504, flags: DIFlagFwdDecl, elements: !1943)
!1943 = !{!1944, !1945, !1946, !1947, !1948, !1949, !1950, !1951, !1952, !1953, !1954}
!1944 = !DIEnumerator(name: "Xchg", value: 0)
!1945 = !DIEnumerator(name: "Add", value: 1)
!1946 = !DIEnumerator(name: "Sub", value: 2)
!1947 = !DIEnumerator(name: "And", value: 3)
!1948 = !DIEnumerator(name: "Nand", value: 4)
!1949 = !DIEnumerator(name: "Or", value: 5)
!1950 = !DIEnumerator(name: "Xor", value: 6)
!1951 = !DIEnumerator(name: "Max", value: 7)
!1952 = !DIEnumerator(name: "Min", value: 8)
!1953 = !DIEnumerator(name: "UMax", value: 9)
!1954 = !DIEnumerator(name: "UMin", value: 10)
!1955 = !DILocation(line: 504, column: 11, scope: !1938)
!1956 = distinct !DISubprogram(name: "_CNW7builtinE14AtomicRMWBinOpD3Ev", linkageName: "_CNW7builtinE14AtomicRMWBinOpD3Ev", scope: null, file: !17, line: 504, type: !1939, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1957 = !DILocation(line: 504, column: 11, scope: !1956)
!1958 = distinct !DISubprogram(name: "_CNW7builtinE11coro_resumeEKPvL0", linkageName: "_CNW7builtinE11coro_resumeEKPvL0", scope: null, file: !17, line: 543, type: !1959, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1959 = !DISubroutineType(types: !1960)
!1960 = !{!9}
!1961 = !DILocalVariable(name: "hdl", scope: !1958, file: !1, line: 543, type: !9)
!1962 = !DILocation(line: 543, column: 11, scope: !1958)
!1963 = !DILocation(line: 544, column: 5, scope: !1958)
!1964 = distinct !DISubprogram(name: "_CNW7builtinE12coro_destroyEKPvL0", linkageName: "_CNW7builtinE12coro_destroyEKPvL0", scope: null, file: !17, line: 548, type: !1959, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1965 = !DILocalVariable(name: "hdl", scope: !1964, file: !1, line: 548, type: !9)
!1966 = !DILocation(line: 548, column: 11, scope: !1964)
!1967 = !DILocation(line: 549, column: 5, scope: !1964)
!1968 = distinct !DISubprogram(name: "_CNW7builtinE9coro_doneEKPvL0", linkageName: "_CNW7builtinE9coro_doneEKPvL0", scope: null, file: !17, line: 553, type: !1959, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1969 = !DILocalVariable(name: "hdl", scope: !1968, file: !1, line: 553, type: !9)
!1970 = !DILocation(line: 553, column: 11, scope: !1968)
!1971 = !DILocation(line: 554, column: 5, scope: !1968)
!1972 = distinct !DISubprogram(name: "_CNW7builtinE14fcmp_false_f32EffL0L0", linkageName: "_CNW7builtinE14fcmp_false_f32EffL0L0", scope: null, file: !6, line: 17, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1973 = !DILocalVariable(name: "lhs", scope: !1972, file: !1, line: 17, type: !1699)
!1974 = !DILocation(line: 17, column: 27, scope: !1972)
!1975 = !DILocalVariable(name: "rhs", scope: !1972, file: !1, line: 17, type: !1699)
!1976 = !DILocation(line: 22, column: 21, scope: !1972)
!1977 = distinct !DISubprogram(name: "_CNW7builtinE13fcmp_true_f32EffL0L0", linkageName: "_CNW7builtinE13fcmp_true_f32EffL0L0", scope: null, file: !6, line: 23, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1978 = !DILocalVariable(name: "lhs", scope: !1977, file: !1, line: 23, type: !1699)
!1979 = !DILocation(line: 23, column: 19, scope: !1977)
!1980 = !DILocalVariable(name: "rhs", scope: !1977, file: !1, line: 23, type: !1699)
!1981 = !DILocation(line: 24, column: 24, scope: !1977)
!1982 = distinct !DISubprogram(name: "_CNW7builtinE9log10_f32EfL0", linkageName: "_CNW7builtinE9log10_f32EfL0", scope: null, file: !6, line: 46, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1983 = !DILocalVariable(name: "val", scope: !1982, file: !1, line: 46, type: !1699)
!1984 = !DILocation(line: 46, column: 57, scope: !1982)
!1985 = !DILocation(line: 48, column: 16, scope: !1982)
!1986 = distinct !DISubprogram(name: "_CNW7builtinE8log2_f32EfL0", linkageName: "_CNW7builtinE8log2_f32EfL0", scope: null, file: !6, line: 49, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1987 = !DILocalVariable(name: "val", scope: !1986, file: !1, line: 49, type: !1699)
!1988 = !DILocation(line: 49, column: 21, scope: !1986)
!1989 = !DILocation(line: 50, column: 5, scope: !1986)
!1990 = distinct !DISubprogram(name: "_CNW7builtinE7log_f32EfL0", linkageName: "_CNW7builtinE7log_f32EfL0", scope: null, file: !6, line: 51, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1991 = !DILocalVariable(name: "val", scope: !1990, file: !1, line: 51, type: !1699)
!1992 = !DILocation(line: 51, column: 9, scope: !1990)
!1993 = !DILocation(line: 51, column: 64, scope: !1990)
!1994 = distinct !DISubprogram(name: "_CNW7builtinE9floor_f32EfL0", linkageName: "_CNW7builtinE9floor_f32EfL0", scope: null, file: !6, line: 53, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1995 = !DILocalVariable(name: "val", scope: !1994, file: !1, line: 53, type: !1699)
!1996 = !DILocation(line: 53, column: 68, scope: !1994)
!1997 = !DILocation(line: 54, column: 52, scope: !1994)
!1998 = distinct !DISubprogram(name: "_CNW7builtinE8ceil_f32EfL0", linkageName: "_CNW7builtinE8ceil_f32EfL0", scope: null, file: !6, line: 55, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!1999 = !DILocalVariable(name: "val", scope: !1998, file: !1, line: 55, type: !1699)
!2000 = !DILocation(line: 55, column: 58, scope: !1998)
!2001 = !DILocation(line: 56, column: 41, scope: !1998)
!2002 = distinct !DISubprogram(name: "_CNW7builtinE9round_f32EfL0", linkageName: "_CNW7builtinE9round_f32EfL0", scope: null, file: !6, line: 58, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2003 = !DILocalVariable(name: "val", scope: !2002, file: !1, line: 58, type: !1699)
!2004 = !DILocation(line: 58, column: 45, scope: !2002)
!2005 = !DILocation(line: 59, column: 16, scope: !2002)
!2006 = distinct !DISubprogram(name: "_CNW7builtinE8rint_f32EfL0", linkageName: "_CNW7builtinE8rint_f32EfL0", scope: null, file: !6, line: 61, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2007 = !DILocalVariable(name: "val", scope: !2006, file: !1, line: 61, type: !1699)
!2008 = !DILocation(line: 61, column: 6, scope: !2006)
!2009 = !DILocation(line: 62, column: 21, scope: !2006)
!2010 = distinct !DISubprogram(name: "_CNW7builtinE9trunc_f32EfL0", linkageName: "_CNW7builtinE9trunc_f32EfL0", scope: null, file: !6, line: 64, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2011 = !DILocalVariable(name: "val", scope: !2010, file: !1, line: 64, type: !1699)
!2012 = !DILocation(line: 64, column: 10, scope: !2010)
!2013 = !DILocation(line: 65, column: 37, scope: !2010)
!2014 = distinct !DISubprogram(name: "_CNW7builtinE8sqrt_f32EfL0", linkageName: "_CNW7builtinE8sqrt_f32EfL0", scope: null, file: !6, line: 67, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2015 = !DILocalVariable(name: "val", scope: !2014, file: !1, line: 67, type: !1699)
!2016 = !DILocation(line: 67, column: 21, scope: !2014)
!2017 = !DILocation(line: 68, column: 30, scope: !2014)
!2018 = distinct !DISubprogram(name: "_CNW7builtinE8fabs_f32EfL0", linkageName: "_CNW7builtinE8fabs_f32EfL0", scope: null, file: !6, line: 71, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2019 = !DILocalVariable(name: "val", scope: !2018, file: !1, line: 71, type: !1699)
!2020 = !DILocation(line: 71, column: 12, scope: !2018)
!2021 = !DILocation(line: 72, column: 10, scope: !2018)
!2022 = distinct !DISubprogram(name: "_CNW7builtinE7sin_f32EfL0", linkageName: "_CNW7builtinE7sin_f32EfL0", scope: null, file: !6, line: 73, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2023 = !DILocalVariable(name: "val", scope: !2022, file: !1, line: 73, type: !1699)
!2024 = !DILocation(line: 73, column: 29, scope: !2022)
!2025 = !DILocation(line: 74, column: 26, scope: !2022)
!2026 = distinct !DISubprogram(name: "_CNW7builtinE7cos_f32EfL0", linkageName: "_CNW7builtinE7cos_f32EfL0", scope: null, file: !6, line: 75, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2027 = !DILocalVariable(name: "val", scope: !2026, file: !1, line: 75, type: !1699)
!2028 = !DILocation(line: 75, column: 45, scope: !2026)
!2029 = !DILocation(line: 76, column: 42, scope: !2026)
!2030 = distinct !DISubprogram(name: "_CNW7builtinE7exp_f32EfL0", linkageName: "_CNW7builtinE7exp_f32EfL0", scope: null, file: !6, line: 79, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2031 = !DILocalVariable(name: "val", scope: !2030, file: !1, line: 79, type: !1699)
!2032 = !DILocation(line: 79, column: 14, scope: !2030)
!2033 = !DILocation(line: 81, column: 20, scope: !2030)
!2034 = distinct !DISubprogram(name: "_CNW7builtinE8exp2_f32EfL0", linkageName: "_CNW7builtinE8exp2_f32EfL0", scope: null, file: !6, line: 83, type: !1869, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2035 = !DILocalVariable(name: "val", scope: !2034, file: !1, line: 83, type: !1699)
!2036 = !DILocation(line: 83, column: 14, scope: !2034)
!2037 = !DILocation(line: 85, column: 35, scope: !2034)
!2038 = distinct !DISubprogram(name: "_CNW7builtinE7pow_f32EffL0L0", linkageName: "_CNW7builtinE7pow_f32EffL0L0", scope: null, file: !6, line: 87, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2039 = !DILocalVariable(name: "val", scope: !2038, file: !1, line: 87, type: !1699)
!2040 = !DILocation(line: 87, column: 20, scope: !2038)
!2041 = !DILocalVariable(name: "exp", scope: !2038, file: !1, line: 87, type: !1699)
!2042 = !DILocation(line: 87, column: 89, scope: !2038)
!2043 = distinct !DISubprogram(name: "_CNW7builtinE8powi_f32EfiL0L0", linkageName: "_CNW7builtinE8powi_f32EfiL0L0", scope: null, file: !6, line: 87, type: !2044, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2044 = !DISubroutineType(types: !2045)
!2045 = !{!1699, !101}
!2046 = !DILocalVariable(name: "val", scope: !2043, file: !1, line: 87, type: !1699)
!2047 = !DILocation(line: 87, column: 170, scope: !2043)
!2048 = !DILocalVariable(name: "exp", scope: !2043, file: !1, line: 87, type: !101)
!2049 = !DILocation(line: 87, column: 238, scope: !2043)
!2050 = distinct !DISubprogram(name: "_CNW7builtinE10minnum_f32EffL0L0", linkageName: "_CNW7builtinE10minnum_f32EffL0L0", scope: null, file: !6, line: 87, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2051 = !DILocalVariable(name: "lhs", scope: !2050, file: !1, line: 87, type: !1699)
!2052 = !DILocation(line: 87, column: 320, scope: !2050)
!2053 = !DILocalVariable(name: "rhs", scope: !2050, file: !1, line: 87, type: !1699)
!2054 = !DILocation(line: 87, column: 392, scope: !2050)
!2055 = distinct !DISubprogram(name: "_CNW7builtinE10maxnum_f32EffL0L0", linkageName: "_CNW7builtinE10maxnum_f32EffL0L0", scope: null, file: !6, line: 87, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2056 = !DILocalVariable(name: "lhs", scope: !2055, file: !1, line: 87, type: !1699)
!2057 = !DILocation(line: 87, column: 476, scope: !2055)
!2058 = !DILocalVariable(name: "rhs", scope: !2055, file: !1, line: 87, type: !1699)
!2059 = !DILocation(line: 87, column: 548, scope: !2055)
!2060 = distinct !DISubprogram(name: "_CNW7builtinE11minimum_f32EffL0L0", linkageName: "_CNW7builtinE11minimum_f32EffL0L0", scope: null, file: !6, line: 87, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2061 = !DILocalVariable(name: "lhs", scope: !2060, file: !1, line: 87, type: !1699)
!2062 = !DILocation(line: 87, column: 632, scope: !2060)
!2063 = !DILocalVariable(name: "rhs", scope: !2060, file: !1, line: 87, type: !1699)
!2064 = !DILocation(line: 87, column: 705, scope: !2060)
!2065 = distinct !DISubprogram(name: "_CNW7builtinE11maximum_f32EffL0L0", linkageName: "_CNW7builtinE11maximum_f32EffL0L0", scope: null, file: !6, line: 87, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2066 = !DILocalVariable(name: "lhs", scope: !2065, file: !1, line: 87, type: !1699)
!2067 = !DILocation(line: 87, column: 790, scope: !2065)
!2068 = !DILocalVariable(name: "rhs", scope: !2065, file: !1, line: 87, type: !1699)
!2069 = !DILocation(line: 87, column: 863, scope: !2065)
!2070 = distinct !DISubprogram(name: "_CNW7builtinE12copysign_f32EffL0L0", linkageName: "_CNW7builtinE12copysign_f32EffL0L0", scope: null, file: !6, line: 87, type: !1697, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2071 = !DILocalVariable(name: "mag", scope: !2070, file: !1, line: 87, type: !1699)
!2072 = !DILocation(line: 87, column: 948, scope: !2070)
!2073 = !DILocalVariable(name: "sign", scope: !2070, file: !1, line: 87, type: !1699)
!2074 = !DILocation(line: 87, column: 1023, scope: !2070)
!2075 = distinct !DISubprogram(name: "_CNW7builtinE7fma_f32EfffL0L0L0", linkageName: "_CNW7builtinE7fma_f32EfffL0L0L0", scope: null, file: !6, line: 87, type: !2076, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2076 = !DISubroutineType(types: !2077)
!2077 = !{!1699, !1699, !1699}
!2078 = !DILocalVariable(name: "a", scope: !2075, file: !1, line: 87, type: !1699)
!2079 = !DILocation(line: 87, column: 1110, scope: !2075)
!2080 = !DILocalVariable(name: "b", scope: !2075, file: !1, line: 87, type: !1699)
!2081 = !DILocalVariable(name: "c", scope: !2075, file: !1, line: 87, type: !1699)
!2082 = !DILocation(line: 87, column: 1187, scope: !2075)
!2083 = distinct !DISubprogram(name: "_CNW7builtinE14fcmp_false_f64EddL0L0", linkageName: "_CNW7builtinE14fcmp_false_f64EddL0L0", scope: null, file: !6, line: 17, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2084 = !DILocalVariable(name: "lhs", scope: !2083, file: !1, line: 17, type: !1715)
!2085 = !DILocation(line: 17, column: 27, scope: !2083)
!2086 = !DILocalVariable(name: "rhs", scope: !2083, file: !1, line: 17, type: !1715)
!2087 = !DILocation(line: 22, column: 21, scope: !2083)
!2088 = distinct !DISubprogram(name: "_CNW7builtinE13fcmp_true_f64EddL0L0", linkageName: "_CNW7builtinE13fcmp_true_f64EddL0L0", scope: null, file: !6, line: 23, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2089 = !DILocalVariable(name: "lhs", scope: !2088, file: !1, line: 23, type: !1715)
!2090 = !DILocation(line: 23, column: 19, scope: !2088)
!2091 = !DILocalVariable(name: "rhs", scope: !2088, file: !1, line: 23, type: !1715)
!2092 = !DILocation(line: 24, column: 24, scope: !2088)
!2093 = distinct !DISubprogram(name: "_CNW7builtinE9log10_f64EdL0", linkageName: "_CNW7builtinE9log10_f64EdL0", scope: null, file: !6, line: 46, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2094 = !DILocalVariable(name: "val", scope: !2093, file: !1, line: 46, type: !1715)
!2095 = !DILocation(line: 46, column: 57, scope: !2093)
!2096 = !DILocation(line: 48, column: 16, scope: !2093)
!2097 = distinct !DISubprogram(name: "_CNW7builtinE8log2_f64EdL0", linkageName: "_CNW7builtinE8log2_f64EdL0", scope: null, file: !6, line: 49, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2098 = !DILocalVariable(name: "val", scope: !2097, file: !1, line: 49, type: !1715)
!2099 = !DILocation(line: 49, column: 21, scope: !2097)
!2100 = !DILocation(line: 50, column: 5, scope: !2097)
!2101 = distinct !DISubprogram(name: "_CNW7builtinE7log_f64EdL0", linkageName: "_CNW7builtinE7log_f64EdL0", scope: null, file: !6, line: 51, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2102 = !DILocalVariable(name: "val", scope: !2101, file: !1, line: 51, type: !1715)
!2103 = !DILocation(line: 51, column: 9, scope: !2101)
!2104 = !DILocation(line: 51, column: 64, scope: !2101)
!2105 = distinct !DISubprogram(name: "_CNW7builtinE9floor_f64EdL0", linkageName: "_CNW7builtinE9floor_f64EdL0", scope: null, file: !6, line: 53, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2106 = !DILocalVariable(name: "val", scope: !2105, file: !1, line: 53, type: !1715)
!2107 = !DILocation(line: 53, column: 68, scope: !2105)
!2108 = !DILocation(line: 54, column: 52, scope: !2105)
!2109 = distinct !DISubprogram(name: "_CNW7builtinE8ceil_f64EdL0", linkageName: "_CNW7builtinE8ceil_f64EdL0", scope: null, file: !6, line: 55, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2110 = !DILocalVariable(name: "val", scope: !2109, file: !1, line: 55, type: !1715)
!2111 = !DILocation(line: 55, column: 58, scope: !2109)
!2112 = !DILocation(line: 56, column: 41, scope: !2109)
!2113 = distinct !DISubprogram(name: "_CNW7builtinE9round_f64EdL0", linkageName: "_CNW7builtinE9round_f64EdL0", scope: null, file: !6, line: 58, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2114 = !DILocalVariable(name: "val", scope: !2113, file: !1, line: 58, type: !1715)
!2115 = !DILocation(line: 58, column: 45, scope: !2113)
!2116 = !DILocation(line: 59, column: 16, scope: !2113)
!2117 = distinct !DISubprogram(name: "_CNW7builtinE8rint_f64EdL0", linkageName: "_CNW7builtinE8rint_f64EdL0", scope: null, file: !6, line: 61, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2118 = !DILocalVariable(name: "val", scope: !2117, file: !1, line: 61, type: !1715)
!2119 = !DILocation(line: 61, column: 6, scope: !2117)
!2120 = !DILocation(line: 62, column: 21, scope: !2117)
!2121 = distinct !DISubprogram(name: "_CNW7builtinE9trunc_f64EdL0", linkageName: "_CNW7builtinE9trunc_f64EdL0", scope: null, file: !6, line: 64, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2122 = !DILocalVariable(name: "val", scope: !2121, file: !1, line: 64, type: !1715)
!2123 = !DILocation(line: 64, column: 10, scope: !2121)
!2124 = !DILocation(line: 65, column: 37, scope: !2121)
!2125 = distinct !DISubprogram(name: "_CNW7builtinE8sqrt_f64EdL0", linkageName: "_CNW7builtinE8sqrt_f64EdL0", scope: null, file: !6, line: 67, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2126 = !DILocalVariable(name: "val", scope: !2125, file: !1, line: 67, type: !1715)
!2127 = !DILocation(line: 67, column: 21, scope: !2125)
!2128 = !DILocation(line: 68, column: 30, scope: !2125)
!2129 = distinct !DISubprogram(name: "_CNW7builtinE8fabs_f64EdL0", linkageName: "_CNW7builtinE8fabs_f64EdL0", scope: null, file: !6, line: 71, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2130 = !DILocalVariable(name: "val", scope: !2129, file: !1, line: 71, type: !1715)
!2131 = !DILocation(line: 71, column: 12, scope: !2129)
!2132 = !DILocation(line: 72, column: 10, scope: !2129)
!2133 = distinct !DISubprogram(name: "_CNW7builtinE7sin_f64EdL0", linkageName: "_CNW7builtinE7sin_f64EdL0", scope: null, file: !6, line: 73, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2134 = !DILocalVariable(name: "val", scope: !2133, file: !1, line: 73, type: !1715)
!2135 = !DILocation(line: 73, column: 29, scope: !2133)
!2136 = !DILocation(line: 74, column: 26, scope: !2133)
!2137 = distinct !DISubprogram(name: "_CNW7builtinE7cos_f64EdL0", linkageName: "_CNW7builtinE7cos_f64EdL0", scope: null, file: !6, line: 75, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2138 = !DILocalVariable(name: "val", scope: !2137, file: !1, line: 75, type: !1715)
!2139 = !DILocation(line: 75, column: 45, scope: !2137)
!2140 = !DILocation(line: 76, column: 42, scope: !2137)
!2141 = distinct !DISubprogram(name: "_CNW7builtinE7exp_f64EdL0", linkageName: "_CNW7builtinE7exp_f64EdL0", scope: null, file: !6, line: 79, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2142 = !DILocalVariable(name: "val", scope: !2141, file: !1, line: 79, type: !1715)
!2143 = !DILocation(line: 79, column: 14, scope: !2141)
!2144 = !DILocation(line: 81, column: 20, scope: !2141)
!2145 = distinct !DISubprogram(name: "_CNW7builtinE8exp2_f64EdL0", linkageName: "_CNW7builtinE8exp2_f64EdL0", scope: null, file: !6, line: 83, type: !1880, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2146 = !DILocalVariable(name: "val", scope: !2145, file: !1, line: 83, type: !1715)
!2147 = !DILocation(line: 83, column: 14, scope: !2145)
!2148 = !DILocation(line: 85, column: 35, scope: !2145)
!2149 = distinct !DISubprogram(name: "_CNW7builtinE7pow_f64EddL0L0", linkageName: "_CNW7builtinE7pow_f64EddL0L0", scope: null, file: !6, line: 87, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2150 = !DILocalVariable(name: "val", scope: !2149, file: !1, line: 87, type: !1715)
!2151 = !DILocation(line: 87, column: 20, scope: !2149)
!2152 = !DILocalVariable(name: "exp", scope: !2149, file: !1, line: 87, type: !1715)
!2153 = !DILocation(line: 87, column: 89, scope: !2149)
!2154 = distinct !DISubprogram(name: "_CNW7builtinE8powi_f64EdiL0L0", linkageName: "_CNW7builtinE8powi_f64EdiL0L0", scope: null, file: !6, line: 87, type: !2155, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2155 = !DISubroutineType(types: !2156)
!2156 = !{!1715, !101}
!2157 = !DILocalVariable(name: "val", scope: !2154, file: !1, line: 87, type: !1715)
!2158 = !DILocation(line: 87, column: 170, scope: !2154)
!2159 = !DILocalVariable(name: "exp", scope: !2154, file: !1, line: 87, type: !101)
!2160 = !DILocation(line: 87, column: 238, scope: !2154)
!2161 = distinct !DISubprogram(name: "_CNW7builtinE10minnum_f64EddL0L0", linkageName: "_CNW7builtinE10minnum_f64EddL0L0", scope: null, file: !6, line: 87, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2162 = !DILocalVariable(name: "lhs", scope: !2161, file: !1, line: 87, type: !1715)
!2163 = !DILocation(line: 87, column: 320, scope: !2161)
!2164 = !DILocalVariable(name: "rhs", scope: !2161, file: !1, line: 87, type: !1715)
!2165 = !DILocation(line: 87, column: 392, scope: !2161)
!2166 = distinct !DISubprogram(name: "_CNW7builtinE10maxnum_f64EddL0L0", linkageName: "_CNW7builtinE10maxnum_f64EddL0L0", scope: null, file: !6, line: 87, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2167 = !DILocalVariable(name: "lhs", scope: !2166, file: !1, line: 87, type: !1715)
!2168 = !DILocation(line: 87, column: 476, scope: !2166)
!2169 = !DILocalVariable(name: "rhs", scope: !2166, file: !1, line: 87, type: !1715)
!2170 = !DILocation(line: 87, column: 548, scope: !2166)
!2171 = distinct !DISubprogram(name: "_CNW7builtinE11minimum_f64EddL0L0", linkageName: "_CNW7builtinE11minimum_f64EddL0L0", scope: null, file: !6, line: 87, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2172 = !DILocalVariable(name: "lhs", scope: !2171, file: !1, line: 87, type: !1715)
!2173 = !DILocation(line: 87, column: 632, scope: !2171)
!2174 = !DILocalVariable(name: "rhs", scope: !2171, file: !1, line: 87, type: !1715)
!2175 = !DILocation(line: 87, column: 705, scope: !2171)
!2176 = distinct !DISubprogram(name: "_CNW7builtinE11maximum_f64EddL0L0", linkageName: "_CNW7builtinE11maximum_f64EddL0L0", scope: null, file: !6, line: 87, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2177 = !DILocalVariable(name: "lhs", scope: !2176, file: !1, line: 87, type: !1715)
!2178 = !DILocation(line: 87, column: 790, scope: !2176)
!2179 = !DILocalVariable(name: "rhs", scope: !2176, file: !1, line: 87, type: !1715)
!2180 = !DILocation(line: 87, column: 863, scope: !2176)
!2181 = distinct !DISubprogram(name: "_CNW7builtinE12copysign_f64EddL0L0", linkageName: "_CNW7builtinE12copysign_f64EddL0L0", scope: null, file: !6, line: 87, type: !1713, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2182 = !DILocalVariable(name: "mag", scope: !2181, file: !1, line: 87, type: !1715)
!2183 = !DILocation(line: 87, column: 948, scope: !2181)
!2184 = !DILocalVariable(name: "sign", scope: !2181, file: !1, line: 87, type: !1715)
!2185 = !DILocation(line: 87, column: 1023, scope: !2181)
!2186 = distinct !DISubprogram(name: "_CNW7builtinE7fma_f64EdddL0L0L0", linkageName: "_CNW7builtinE7fma_f64EdddL0L0L0", scope: null, file: !6, line: 87, type: !2187, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2187 = !DISubroutineType(types: !2188)
!2188 = !{!1715, !1715, !1715}
!2189 = !DILocalVariable(name: "a", scope: !2186, file: !1, line: 87, type: !1715)
!2190 = !DILocation(line: 87, column: 1110, scope: !2186)
!2191 = !DILocalVariable(name: "b", scope: !2186, file: !1, line: 87, type: !1715)
!2192 = !DILocalVariable(name: "c", scope: !2186, file: !1, line: 87, type: !1715)
!2193 = !DILocation(line: 87, column: 1187, scope: !2186)
!2194 = distinct !DISubprogram(name: "_CNW7builtinE7ctlz_i1EbL0", linkageName: "_CNW7builtinE7ctlz_i1EbL0", scope: null, file: !6, line: 87, type: !1595, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2195 = !DILocalVariable(name: "val", scope: !2194, file: !1, line: 87, type: !1597)
!2196 = !DILocation(line: 87, column: 1248, scope: !2194)
!2197 = !DILocation(line: 87, column: 1304, scope: !2194)
!2198 = distinct !DISubprogram(name: "_CNW7builtinE7cttz_i1EbL0", linkageName: "_CNW7builtinE7cttz_i1EbL0", scope: null, file: !6, line: 87, type: !1595, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2199 = !DILocalVariable(name: "val", scope: !2198, file: !1, line: 87, type: !1597)
!2200 = !DILocation(line: 87, column: 1391, scope: !2198)
!2201 = !DILocation(line: 87, column: 1447, scope: !2198)
!2202 = distinct !DISubprogram(name: "_CNW7builtinE8ctpop_i1EbL0", linkageName: "_CNW7builtinE8ctpop_i1EbL0", scope: null, file: !6, line: 87, type: !1595, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2203 = !DILocalVariable(name: "val", scope: !2202, file: !1, line: 87, type: !1597)
!2204 = !DILocation(line: 87, column: 1534, scope: !2202)
!2205 = !DILocation(line: 87, column: 1591, scope: !2202)
!2206 = distinct !DISubprogram(name: "_CNW7builtinE13bitreverse_i1EbL0", linkageName: "_CNW7builtinE13bitreverse_i1EbL0", scope: null, file: !6, line: 87, type: !1595, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2207 = !DILocalVariable(name: "val", scope: !2206, file: !1, line: 87, type: !1597)
!2208 = !DILocation(line: 87, column: 1672, scope: !2206)
!2209 = !DILocation(line: 87, column: 1734, scope: !2206)
!2210 = distinct !DISubprogram(name: "_CNW7builtinE6neg_i1EbL0", linkageName: "_CNW7builtinE6neg_i1EbL0", scope: null, file: !6, line: 87, type: !1595, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2211 = !DILocalVariable(name: "val", scope: !2210, file: !1, line: 87, type: !1597)
!2212 = !DILocation(line: 87, column: 1932, scope: !2210)
!2213 = !DILocation(line: 87, column: 1987, scope: !2210)
!2214 = distinct !DISubprogram(name: "_CNW7builtinE5id_i1EbL0", linkageName: "_CNW7builtinE5id_i1EbL0", scope: null, file: !6, line: 87, type: !1595, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2215 = !DILocalVariable(name: "val", scope: !2214, file: !1, line: 87, type: !1597)
!2216 = !DILocation(line: 87, column: 2044, scope: !2214)
!2217 = !DILocation(line: 87, column: 2098, scope: !2214)
!2218 = distinct !DISubprogram(name: "_CNW7builtinE21sadd_with_overflow_i1EbbL0L0", linkageName: "_CNW7builtinE21sadd_with_overflow_i1EbbL0L0", scope: null, file: !6, line: 87, type: !2219, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2219 = !DISubroutineType(types: !2220)
!2220 = !{!1597, !1597}
!2221 = !DILocalVariable(name: "lhs", scope: !2218, file: !1, line: 87, type: !1597)
!2222 = !DILocation(line: 87, column: 3476, scope: !2218)
!2223 = !DILocalVariable(name: "rhs", scope: !2218, file: !1, line: 87, type: !1597)
!2224 = !DILocation(line: 87, column: 3566, scope: !2218)
!2225 = distinct !DISubprogram(name: "_CNW7builtinE21uadd_with_overflow_i1EbbL0L0", linkageName: "_CNW7builtinE21uadd_with_overflow_i1EbbL0L0", scope: null, file: !6, line: 87, type: !2219, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2226 = !DILocalVariable(name: "lhs", scope: !2225, file: !1, line: 87, type: !1597)
!2227 = !DILocation(line: 87, column: 3665, scope: !2225)
!2228 = !DILocalVariable(name: "rhs", scope: !2225, file: !1, line: 87, type: !1597)
!2229 = !DILocation(line: 87, column: 3755, scope: !2225)
!2230 = distinct !DISubprogram(name: "_CNW7builtinE21ssub_with_overflow_i1EbbL0L0", linkageName: "_CNW7builtinE21ssub_with_overflow_i1EbbL0L0", scope: null, file: !6, line: 87, type: !2219, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2231 = !DILocalVariable(name: "lhs", scope: !2230, file: !1, line: 87, type: !1597)
!2232 = !DILocation(line: 87, column: 3854, scope: !2230)
!2233 = !DILocalVariable(name: "rhs", scope: !2230, file: !1, line: 87, type: !1597)
!2234 = !DILocation(line: 87, column: 3944, scope: !2230)
!2235 = distinct !DISubprogram(name: "_CNW7builtinE21usub_with_overflow_i1EbbL0L0", linkageName: "_CNW7builtinE21usub_with_overflow_i1EbbL0L0", scope: null, file: !6, line: 87, type: !2219, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2236 = !DILocalVariable(name: "lhs", scope: !2235, file: !1, line: 87, type: !1597)
!2237 = !DILocation(line: 87, column: 4043, scope: !2235)
!2238 = !DILocalVariable(name: "rhs", scope: !2235, file: !1, line: 87, type: !1597)
!2239 = !DILocation(line: 87, column: 4133, scope: !2235)
!2240 = distinct !DISubprogram(name: "_CNW7builtinE21smul_with_overflow_i1EbbL0L0", linkageName: "_CNW7builtinE21smul_with_overflow_i1EbbL0L0", scope: null, file: !6, line: 87, type: !2219, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2241 = !DILocalVariable(name: "lhs", scope: !2240, file: !1, line: 87, type: !1597)
!2242 = !DILocation(line: 87, column: 4232, scope: !2240)
!2243 = !DILocalVariable(name: "rhs", scope: !2240, file: !1, line: 87, type: !1597)
!2244 = !DILocation(line: 87, column: 4322, scope: !2240)
!2245 = distinct !DISubprogram(name: "_CNW7builtinE21umul_with_overflow_i1EbbL0L0", linkageName: "_CNW7builtinE21umul_with_overflow_i1EbbL0L0", scope: null, file: !6, line: 87, type: !2219, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2246 = !DILocalVariable(name: "lhs", scope: !2245, file: !1, line: 87, type: !1597)
!2247 = !DILocation(line: 87, column: 4421, scope: !2245)
!2248 = !DILocalVariable(name: "rhs", scope: !2245, file: !1, line: 87, type: !1597)
!2249 = !DILocation(line: 87, column: 4511, scope: !2245)
!2250 = distinct !DISubprogram(name: "_CNW7builtinE7ctlz_i8EaL0", linkageName: "_CNW7builtinE7ctlz_i8EaL0", scope: null, file: !6, line: 87, type: !1395, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2251 = !DILocalVariable(name: "val", scope: !2250, file: !1, line: 87, type: !37)
!2252 = !DILocation(line: 87, column: 1248, scope: !2250)
!2253 = !DILocation(line: 87, column: 1304, scope: !2250)
!2254 = distinct !DISubprogram(name: "_CNW7builtinE7cttz_i8EaL0", linkageName: "_CNW7builtinE7cttz_i8EaL0", scope: null, file: !6, line: 87, type: !1395, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2255 = !DILocalVariable(name: "val", scope: !2254, file: !1, line: 87, type: !37)
!2256 = !DILocation(line: 87, column: 1391, scope: !2254)
!2257 = !DILocation(line: 87, column: 1447, scope: !2254)
!2258 = distinct !DISubprogram(name: "_CNW7builtinE8ctpop_i8EaL0", linkageName: "_CNW7builtinE8ctpop_i8EaL0", scope: null, file: !6, line: 87, type: !1395, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2259 = !DILocalVariable(name: "val", scope: !2258, file: !1, line: 87, type: !37)
!2260 = !DILocation(line: 87, column: 1534, scope: !2258)
!2261 = !DILocation(line: 87, column: 1591, scope: !2258)
!2262 = distinct !DISubprogram(name: "_CNW7builtinE13bitreverse_i8EaL0", linkageName: "_CNW7builtinE13bitreverse_i8EaL0", scope: null, file: !6, line: 87, type: !1395, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2263 = !DILocalVariable(name: "val", scope: !2262, file: !1, line: 87, type: !37)
!2264 = !DILocation(line: 87, column: 1672, scope: !2262)
!2265 = !DILocation(line: 87, column: 1734, scope: !2262)
!2266 = distinct !DISubprogram(name: "_CNW7builtinE21sadd_with_overflow_i8EaaL0L0", linkageName: "_CNW7builtinE21sadd_with_overflow_i8EaaL0L0", scope: null, file: !6, line: 87, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2267 = !DILocalVariable(name: "lhs", scope: !2266, file: !1, line: 87, type: !37)
!2268 = !DILocation(line: 87, column: 3476, scope: !2266)
!2269 = !DILocalVariable(name: "rhs", scope: !2266, file: !1, line: 87, type: !37)
!2270 = !DILocation(line: 87, column: 3566, scope: !2266)
!2271 = distinct !DISubprogram(name: "_CNW7builtinE21uadd_with_overflow_i8EaaL0L0", linkageName: "_CNW7builtinE21uadd_with_overflow_i8EaaL0L0", scope: null, file: !6, line: 87, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2272 = !DILocalVariable(name: "lhs", scope: !2271, file: !1, line: 87, type: !37)
!2273 = !DILocation(line: 87, column: 3665, scope: !2271)
!2274 = !DILocalVariable(name: "rhs", scope: !2271, file: !1, line: 87, type: !37)
!2275 = !DILocation(line: 87, column: 3755, scope: !2271)
!2276 = distinct !DISubprogram(name: "_CNW7builtinE21ssub_with_overflow_i8EaaL0L0", linkageName: "_CNW7builtinE21ssub_with_overflow_i8EaaL0L0", scope: null, file: !6, line: 87, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2277 = !DILocalVariable(name: "lhs", scope: !2276, file: !1, line: 87, type: !37)
!2278 = !DILocation(line: 87, column: 3854, scope: !2276)
!2279 = !DILocalVariable(name: "rhs", scope: !2276, file: !1, line: 87, type: !37)
!2280 = !DILocation(line: 87, column: 3944, scope: !2276)
!2281 = distinct !DISubprogram(name: "_CNW7builtinE21usub_with_overflow_i8EaaL0L0", linkageName: "_CNW7builtinE21usub_with_overflow_i8EaaL0L0", scope: null, file: !6, line: 87, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2282 = !DILocalVariable(name: "lhs", scope: !2281, file: !1, line: 87, type: !37)
!2283 = !DILocation(line: 87, column: 4043, scope: !2281)
!2284 = !DILocalVariable(name: "rhs", scope: !2281, file: !1, line: 87, type: !37)
!2285 = !DILocation(line: 87, column: 4133, scope: !2281)
!2286 = distinct !DISubprogram(name: "_CNW7builtinE21smul_with_overflow_i8EaaL0L0", linkageName: "_CNW7builtinE21smul_with_overflow_i8EaaL0L0", scope: null, file: !6, line: 87, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2287 = !DILocalVariable(name: "lhs", scope: !2286, file: !1, line: 87, type: !37)
!2288 = !DILocation(line: 87, column: 4232, scope: !2286)
!2289 = !DILocalVariable(name: "rhs", scope: !2286, file: !1, line: 87, type: !37)
!2290 = !DILocation(line: 87, column: 4322, scope: !2286)
!2291 = distinct !DISubprogram(name: "_CNW7builtinE21umul_with_overflow_i8EaaL0L0", linkageName: "_CNW7builtinE21umul_with_overflow_i8EaaL0L0", scope: null, file: !6, line: 87, type: !35, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2292 = !DILocalVariable(name: "lhs", scope: !2291, file: !1, line: 87, type: !37)
!2293 = !DILocation(line: 87, column: 4421, scope: !2291)
!2294 = !DILocalVariable(name: "rhs", scope: !2291, file: !1, line: 87, type: !37)
!2295 = !DILocation(line: 87, column: 4511, scope: !2291)
!2296 = distinct !DISubprogram(name: "_CNW7builtinE8ctlz_i16EsL0", linkageName: "_CNW7builtinE8ctlz_i16EsL0", scope: null, file: !6, line: 87, type: !1417, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2297 = !DILocalVariable(name: "val", scope: !2296, file: !1, line: 87, type: !69)
!2298 = !DILocation(line: 87, column: 1248, scope: !2296)
!2299 = !DILocation(line: 87, column: 1304, scope: !2296)
!2300 = distinct !DISubprogram(name: "_CNW7builtinE8cttz_i16EsL0", linkageName: "_CNW7builtinE8cttz_i16EsL0", scope: null, file: !6, line: 87, type: !1417, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2301 = !DILocalVariable(name: "val", scope: !2300, file: !1, line: 87, type: !69)
!2302 = !DILocation(line: 87, column: 1391, scope: !2300)
!2303 = !DILocation(line: 87, column: 1447, scope: !2300)
!2304 = distinct !DISubprogram(name: "_CNW7builtinE9ctpop_i16EsL0", linkageName: "_CNW7builtinE9ctpop_i16EsL0", scope: null, file: !6, line: 87, type: !1417, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2305 = !DILocalVariable(name: "val", scope: !2304, file: !1, line: 87, type: !69)
!2306 = !DILocation(line: 87, column: 1534, scope: !2304)
!2307 = !DILocation(line: 87, column: 1591, scope: !2304)
!2308 = distinct !DISubprogram(name: "_CNW7builtinE14bitreverse_i16EsL0", linkageName: "_CNW7builtinE14bitreverse_i16EsL0", scope: null, file: !6, line: 87, type: !1417, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2309 = !DILocalVariable(name: "val", scope: !2308, file: !1, line: 87, type: !69)
!2310 = !DILocation(line: 87, column: 1672, scope: !2308)
!2311 = !DILocation(line: 87, column: 1734, scope: !2308)
!2312 = distinct !DISubprogram(name: "_CNW7builtinE22sadd_with_overflow_i16EssL0L0", linkageName: "_CNW7builtinE22sadd_with_overflow_i16EssL0L0", scope: null, file: !6, line: 87, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2313 = !DILocalVariable(name: "lhs", scope: !2312, file: !1, line: 87, type: !69)
!2314 = !DILocation(line: 87, column: 3476, scope: !2312)
!2315 = !DILocalVariable(name: "rhs", scope: !2312, file: !1, line: 87, type: !69)
!2316 = !DILocation(line: 87, column: 3566, scope: !2312)
!2317 = distinct !DISubprogram(name: "_CNW7builtinE22uadd_with_overflow_i16EssL0L0", linkageName: "_CNW7builtinE22uadd_with_overflow_i16EssL0L0", scope: null, file: !6, line: 87, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2318 = !DILocalVariable(name: "lhs", scope: !2317, file: !1, line: 87, type: !69)
!2319 = !DILocation(line: 87, column: 3665, scope: !2317)
!2320 = !DILocalVariable(name: "rhs", scope: !2317, file: !1, line: 87, type: !69)
!2321 = !DILocation(line: 87, column: 3755, scope: !2317)
!2322 = distinct !DISubprogram(name: "_CNW7builtinE22ssub_with_overflow_i16EssL0L0", linkageName: "_CNW7builtinE22ssub_with_overflow_i16EssL0L0", scope: null, file: !6, line: 87, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2323 = !DILocalVariable(name: "lhs", scope: !2322, file: !1, line: 87, type: !69)
!2324 = !DILocation(line: 87, column: 3854, scope: !2322)
!2325 = !DILocalVariable(name: "rhs", scope: !2322, file: !1, line: 87, type: !69)
!2326 = !DILocation(line: 87, column: 3944, scope: !2322)
!2327 = distinct !DISubprogram(name: "_CNW7builtinE22usub_with_overflow_i16EssL0L0", linkageName: "_CNW7builtinE22usub_with_overflow_i16EssL0L0", scope: null, file: !6, line: 87, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2328 = !DILocalVariable(name: "lhs", scope: !2327, file: !1, line: 87, type: !69)
!2329 = !DILocation(line: 87, column: 4043, scope: !2327)
!2330 = !DILocalVariable(name: "rhs", scope: !2327, file: !1, line: 87, type: !69)
!2331 = !DILocation(line: 87, column: 4133, scope: !2327)
!2332 = distinct !DISubprogram(name: "_CNW7builtinE22smul_with_overflow_i16EssL0L0", linkageName: "_CNW7builtinE22smul_with_overflow_i16EssL0L0", scope: null, file: !6, line: 87, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2333 = !DILocalVariable(name: "lhs", scope: !2332, file: !1, line: 87, type: !69)
!2334 = !DILocation(line: 87, column: 4232, scope: !2332)
!2335 = !DILocalVariable(name: "rhs", scope: !2332, file: !1, line: 87, type: !69)
!2336 = !DILocation(line: 87, column: 4322, scope: !2332)
!2337 = distinct !DISubprogram(name: "_CNW7builtinE22umul_with_overflow_i16EssL0L0", linkageName: "_CNW7builtinE22umul_with_overflow_i16EssL0L0", scope: null, file: !6, line: 87, type: !67, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2338 = !DILocalVariable(name: "lhs", scope: !2337, file: !1, line: 87, type: !69)
!2339 = !DILocation(line: 87, column: 4421, scope: !2337)
!2340 = !DILocalVariable(name: "rhs", scope: !2337, file: !1, line: 87, type: !69)
!2341 = !DILocation(line: 87, column: 4511, scope: !2337)
!2342 = distinct !DISubprogram(name: "_CNW7builtinE8ctlz_i32EiL0", linkageName: "_CNW7builtinE8ctlz_i32EiL0", scope: null, file: !6, line: 87, type: !1439, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2343 = !DILocalVariable(name: "val", scope: !2342, file: !1, line: 87, type: !101)
!2344 = !DILocation(line: 87, column: 1248, scope: !2342)
!2345 = !DILocation(line: 87, column: 1304, scope: !2342)
!2346 = distinct !DISubprogram(name: "_CNW7builtinE8cttz_i32EiL0", linkageName: "_CNW7builtinE8cttz_i32EiL0", scope: null, file: !6, line: 87, type: !1439, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2347 = !DILocalVariable(name: "val", scope: !2346, file: !1, line: 87, type: !101)
!2348 = !DILocation(line: 87, column: 1391, scope: !2346)
!2349 = !DILocation(line: 87, column: 1447, scope: !2346)
!2350 = distinct !DISubprogram(name: "_CNW7builtinE9ctpop_i32EiL0", linkageName: "_CNW7builtinE9ctpop_i32EiL0", scope: null, file: !6, line: 87, type: !1439, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2351 = !DILocalVariable(name: "val", scope: !2350, file: !1, line: 87, type: !101)
!2352 = !DILocation(line: 87, column: 1534, scope: !2350)
!2353 = !DILocation(line: 87, column: 1591, scope: !2350)
!2354 = distinct !DISubprogram(name: "_CNW7builtinE14bitreverse_i32EiL0", linkageName: "_CNW7builtinE14bitreverse_i32EiL0", scope: null, file: !6, line: 87, type: !1439, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2355 = !DILocalVariable(name: "val", scope: !2354, file: !1, line: 87, type: !101)
!2356 = !DILocation(line: 87, column: 1672, scope: !2354)
!2357 = !DILocation(line: 87, column: 1734, scope: !2354)
!2358 = distinct !DISubprogram(name: "_CNW7builtinE22sadd_with_overflow_i32EiiL0L0", linkageName: "_CNW7builtinE22sadd_with_overflow_i32EiiL0L0", scope: null, file: !6, line: 87, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2359 = !DILocalVariable(name: "lhs", scope: !2358, file: !1, line: 87, type: !101)
!2360 = !DILocation(line: 87, column: 3476, scope: !2358)
!2361 = !DILocalVariable(name: "rhs", scope: !2358, file: !1, line: 87, type: !101)
!2362 = !DILocation(line: 87, column: 3566, scope: !2358)
!2363 = distinct !DISubprogram(name: "_CNW7builtinE22uadd_with_overflow_i32EiiL0L0", linkageName: "_CNW7builtinE22uadd_with_overflow_i32EiiL0L0", scope: null, file: !6, line: 87, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2364 = !DILocalVariable(name: "lhs", scope: !2363, file: !1, line: 87, type: !101)
!2365 = !DILocation(line: 87, column: 3665, scope: !2363)
!2366 = !DILocalVariable(name: "rhs", scope: !2363, file: !1, line: 87, type: !101)
!2367 = !DILocation(line: 87, column: 3755, scope: !2363)
!2368 = distinct !DISubprogram(name: "_CNW7builtinE22ssub_with_overflow_i32EiiL0L0", linkageName: "_CNW7builtinE22ssub_with_overflow_i32EiiL0L0", scope: null, file: !6, line: 87, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2369 = !DILocalVariable(name: "lhs", scope: !2368, file: !1, line: 87, type: !101)
!2370 = !DILocation(line: 87, column: 3854, scope: !2368)
!2371 = !DILocalVariable(name: "rhs", scope: !2368, file: !1, line: 87, type: !101)
!2372 = !DILocation(line: 87, column: 3944, scope: !2368)
!2373 = distinct !DISubprogram(name: "_CNW7builtinE22usub_with_overflow_i32EiiL0L0", linkageName: "_CNW7builtinE22usub_with_overflow_i32EiiL0L0", scope: null, file: !6, line: 87, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2374 = !DILocalVariable(name: "lhs", scope: !2373, file: !1, line: 87, type: !101)
!2375 = !DILocation(line: 87, column: 4043, scope: !2373)
!2376 = !DILocalVariable(name: "rhs", scope: !2373, file: !1, line: 87, type: !101)
!2377 = !DILocation(line: 87, column: 4133, scope: !2373)
!2378 = distinct !DISubprogram(name: "_CNW7builtinE22smul_with_overflow_i32EiiL0L0", linkageName: "_CNW7builtinE22smul_with_overflow_i32EiiL0L0", scope: null, file: !6, line: 87, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2379 = !DILocalVariable(name: "lhs", scope: !2378, file: !1, line: 87, type: !101)
!2380 = !DILocation(line: 87, column: 4232, scope: !2378)
!2381 = !DILocalVariable(name: "rhs", scope: !2378, file: !1, line: 87, type: !101)
!2382 = !DILocation(line: 87, column: 4322, scope: !2378)
!2383 = distinct !DISubprogram(name: "_CNW7builtinE22umul_with_overflow_i32EiiL0L0", linkageName: "_CNW7builtinE22umul_with_overflow_i32EiiL0L0", scope: null, file: !6, line: 87, type: !99, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2384 = !DILocalVariable(name: "lhs", scope: !2383, file: !1, line: 87, type: !101)
!2385 = !DILocation(line: 87, column: 4421, scope: !2383)
!2386 = !DILocalVariable(name: "rhs", scope: !2383, file: !1, line: 87, type: !101)
!2387 = !DILocation(line: 87, column: 4511, scope: !2383)
!2388 = distinct !DISubprogram(name: "_CNW7builtinE8ctlz_i64ElL0", linkageName: "_CNW7builtinE8ctlz_i64ElL0", scope: null, file: !6, line: 87, type: !1461, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2389 = !DILocalVariable(name: "val", scope: !2388, file: !1, line: 87, type: !133)
!2390 = !DILocation(line: 87, column: 1248, scope: !2388)
!2391 = !DILocation(line: 87, column: 1304, scope: !2388)
!2392 = distinct !DISubprogram(name: "_CNW7builtinE8cttz_i64ElL0", linkageName: "_CNW7builtinE8cttz_i64ElL0", scope: null, file: !6, line: 87, type: !1461, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2393 = !DILocalVariable(name: "val", scope: !2392, file: !1, line: 87, type: !133)
!2394 = !DILocation(line: 87, column: 1391, scope: !2392)
!2395 = !DILocation(line: 87, column: 1447, scope: !2392)
!2396 = distinct !DISubprogram(name: "_CNW7builtinE9ctpop_i64ElL0", linkageName: "_CNW7builtinE9ctpop_i64ElL0", scope: null, file: !6, line: 87, type: !1461, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2397 = !DILocalVariable(name: "val", scope: !2396, file: !1, line: 87, type: !133)
!2398 = !DILocation(line: 87, column: 1534, scope: !2396)
!2399 = !DILocation(line: 87, column: 1591, scope: !2396)
!2400 = distinct !DISubprogram(name: "_CNW7builtinE14bitreverse_i64ElL0", linkageName: "_CNW7builtinE14bitreverse_i64ElL0", scope: null, file: !6, line: 87, type: !1461, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2401 = !DILocalVariable(name: "val", scope: !2400, file: !1, line: 87, type: !133)
!2402 = !DILocation(line: 87, column: 1672, scope: !2400)
!2403 = !DILocation(line: 87, column: 1734, scope: !2400)
!2404 = distinct !DISubprogram(name: "_CNW7builtinE22sadd_with_overflow_i64EllL0L0", linkageName: "_CNW7builtinE22sadd_with_overflow_i64EllL0L0", scope: null, file: !6, line: 87, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2405 = !DILocalVariable(name: "lhs", scope: !2404, file: !1, line: 87, type: !133)
!2406 = !DILocation(line: 87, column: 3476, scope: !2404)
!2407 = !DILocalVariable(name: "rhs", scope: !2404, file: !1, line: 87, type: !133)
!2408 = !DILocation(line: 87, column: 3566, scope: !2404)
!2409 = distinct !DISubprogram(name: "_CNW7builtinE22uadd_with_overflow_i64EllL0L0", linkageName: "_CNW7builtinE22uadd_with_overflow_i64EllL0L0", scope: null, file: !6, line: 87, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2410 = !DILocalVariable(name: "lhs", scope: !2409, file: !1, line: 87, type: !133)
!2411 = !DILocation(line: 87, column: 3665, scope: !2409)
!2412 = !DILocalVariable(name: "rhs", scope: !2409, file: !1, line: 87, type: !133)
!2413 = !DILocation(line: 87, column: 3755, scope: !2409)
!2414 = distinct !DISubprogram(name: "_CNW7builtinE22ssub_with_overflow_i64EllL0L0", linkageName: "_CNW7builtinE22ssub_with_overflow_i64EllL0L0", scope: null, file: !6, line: 87, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2415 = !DILocalVariable(name: "lhs", scope: !2414, file: !1, line: 87, type: !133)
!2416 = !DILocation(line: 87, column: 3854, scope: !2414)
!2417 = !DILocalVariable(name: "rhs", scope: !2414, file: !1, line: 87, type: !133)
!2418 = !DILocation(line: 87, column: 3944, scope: !2414)
!2419 = distinct !DISubprogram(name: "_CNW7builtinE22usub_with_overflow_i64EllL0L0", linkageName: "_CNW7builtinE22usub_with_overflow_i64EllL0L0", scope: null, file: !6, line: 87, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2420 = !DILocalVariable(name: "lhs", scope: !2419, file: !1, line: 87, type: !133)
!2421 = !DILocation(line: 87, column: 4043, scope: !2419)
!2422 = !DILocalVariable(name: "rhs", scope: !2419, file: !1, line: 87, type: !133)
!2423 = !DILocation(line: 87, column: 4133, scope: !2419)
!2424 = distinct !DISubprogram(name: "_CNW7builtinE22smul_with_overflow_i64EllL0L0", linkageName: "_CNW7builtinE22smul_with_overflow_i64EllL0L0", scope: null, file: !6, line: 87, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2425 = !DILocalVariable(name: "lhs", scope: !2424, file: !1, line: 87, type: !133)
!2426 = !DILocation(line: 87, column: 4232, scope: !2424)
!2427 = !DILocalVariable(name: "rhs", scope: !2424, file: !1, line: 87, type: !133)
!2428 = !DILocation(line: 87, column: 4322, scope: !2424)
!2429 = distinct !DISubprogram(name: "_CNW7builtinE22umul_with_overflow_i64EllL0L0", linkageName: "_CNW7builtinE22umul_with_overflow_i64EllL0L0", scope: null, file: !6, line: 87, type: !131, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2430 = !DILocalVariable(name: "lhs", scope: !2429, file: !1, line: 87, type: !133)
!2431 = !DILocation(line: 87, column: 4421, scope: !2429)
!2432 = !DILocalVariable(name: "rhs", scope: !2429, file: !1, line: 87, type: !133)
!2433 = !DILocation(line: 87, column: 4511, scope: !2429)
!2434 = distinct !DISubprogram(name: "_CNW7builtinE9ctlz_i128EnL0", linkageName: "_CNW7builtinE9ctlz_i128EnL0", scope: null, file: !6, line: 87, type: !1483, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2435 = !DILocalVariable(name: "val", scope: !2434, file: !1, line: 87, type: !165)
!2436 = !DILocation(line: 87, column: 1248, scope: !2434)
!2437 = !DILocation(line: 87, column: 1304, scope: !2434)
!2438 = distinct !DISubprogram(name: "_CNW7builtinE9cttz_i128EnL0", linkageName: "_CNW7builtinE9cttz_i128EnL0", scope: null, file: !6, line: 87, type: !1483, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2439 = !DILocalVariable(name: "val", scope: !2438, file: !1, line: 87, type: !165)
!2440 = !DILocation(line: 87, column: 1391, scope: !2438)
!2441 = !DILocation(line: 87, column: 1447, scope: !2438)
!2442 = distinct !DISubprogram(name: "_CNW7builtinE10ctpop_i128EnL0", linkageName: "_CNW7builtinE10ctpop_i128EnL0", scope: null, file: !6, line: 87, type: !1483, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2443 = !DILocalVariable(name: "val", scope: !2442, file: !1, line: 87, type: !165)
!2444 = !DILocation(line: 87, column: 1534, scope: !2442)
!2445 = !DILocation(line: 87, column: 1591, scope: !2442)
!2446 = distinct !DISubprogram(name: "_CNW7builtinE15bitreverse_i128EnL0", linkageName: "_CNW7builtinE15bitreverse_i128EnL0", scope: null, file: !6, line: 87, type: !1483, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2447 = !DILocalVariable(name: "val", scope: !2446, file: !1, line: 87, type: !165)
!2448 = !DILocation(line: 87, column: 1672, scope: !2446)
!2449 = !DILocation(line: 87, column: 1734, scope: !2446)
!2450 = distinct !DISubprogram(name: "_CNW7builtinE23sadd_with_overflow_i128EnnL0L0", linkageName: "_CNW7builtinE23sadd_with_overflow_i128EnnL0L0", scope: null, file: !6, line: 87, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2451 = !DILocalVariable(name: "lhs", scope: !2450, file: !1, line: 87, type: !165)
!2452 = !DILocation(line: 87, column: 3476, scope: !2450)
!2453 = !DILocalVariable(name: "rhs", scope: !2450, file: !1, line: 87, type: !165)
!2454 = !DILocation(line: 87, column: 3566, scope: !2450)
!2455 = distinct !DISubprogram(name: "_CNW7builtinE23uadd_with_overflow_i128EnnL0L0", linkageName: "_CNW7builtinE23uadd_with_overflow_i128EnnL0L0", scope: null, file: !6, line: 87, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2456 = !DILocalVariable(name: "lhs", scope: !2455, file: !1, line: 87, type: !165)
!2457 = !DILocation(line: 87, column: 3665, scope: !2455)
!2458 = !DILocalVariable(name: "rhs", scope: !2455, file: !1, line: 87, type: !165)
!2459 = !DILocation(line: 87, column: 3755, scope: !2455)
!2460 = distinct !DISubprogram(name: "_CNW7builtinE23ssub_with_overflow_i128EnnL0L0", linkageName: "_CNW7builtinE23ssub_with_overflow_i128EnnL0L0", scope: null, file: !6, line: 87, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2461 = !DILocalVariable(name: "lhs", scope: !2460, file: !1, line: 87, type: !165)
!2462 = !DILocation(line: 87, column: 3854, scope: !2460)
!2463 = !DILocalVariable(name: "rhs", scope: !2460, file: !1, line: 87, type: !165)
!2464 = !DILocation(line: 87, column: 3944, scope: !2460)
!2465 = distinct !DISubprogram(name: "_CNW7builtinE23usub_with_overflow_i128EnnL0L0", linkageName: "_CNW7builtinE23usub_with_overflow_i128EnnL0L0", scope: null, file: !6, line: 87, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2466 = !DILocalVariable(name: "lhs", scope: !2465, file: !1, line: 87, type: !165)
!2467 = !DILocation(line: 87, column: 4043, scope: !2465)
!2468 = !DILocalVariable(name: "rhs", scope: !2465, file: !1, line: 87, type: !165)
!2469 = !DILocation(line: 87, column: 4133, scope: !2465)
!2470 = distinct !DISubprogram(name: "_CNW7builtinE23smul_with_overflow_i128EnnL0L0", linkageName: "_CNW7builtinE23smul_with_overflow_i128EnnL0L0", scope: null, file: !6, line: 87, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2471 = !DILocalVariable(name: "lhs", scope: !2470, file: !1, line: 87, type: !165)
!2472 = !DILocation(line: 87, column: 4232, scope: !2470)
!2473 = !DILocalVariable(name: "rhs", scope: !2470, file: !1, line: 87, type: !165)
!2474 = !DILocation(line: 87, column: 4322, scope: !2470)
!2475 = distinct !DISubprogram(name: "_CNW7builtinE23umul_with_overflow_i128EnnL0L0", linkageName: "_CNW7builtinE23umul_with_overflow_i128EnnL0L0", scope: null, file: !6, line: 87, type: !163, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2476 = !DILocalVariable(name: "lhs", scope: !2475, file: !1, line: 87, type: !165)
!2477 = !DILocation(line: 87, column: 4421, scope: !2475)
!2478 = !DILocalVariable(name: "rhs", scope: !2475, file: !1, line: 87, type: !165)
!2479 = !DILocation(line: 87, column: 4511, scope: !2475)
!2480 = distinct !DISubprogram(name: "_CNW7builtinE7ctlz_u8EhL0", linkageName: "_CNW7builtinE7ctlz_u8EhL0", scope: null, file: !6, line: 87, type: !1406, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2481 = !DILocalVariable(name: "val", scope: !2480, file: !1, line: 87, type: !53)
!2482 = !DILocation(line: 87, column: 1248, scope: !2480)
!2483 = !DILocation(line: 87, column: 1304, scope: !2480)
!2484 = distinct !DISubprogram(name: "_CNW7builtinE7cttz_u8EhL0", linkageName: "_CNW7builtinE7cttz_u8EhL0", scope: null, file: !6, line: 87, type: !1406, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2485 = !DILocalVariable(name: "val", scope: !2484, file: !1, line: 87, type: !53)
!2486 = !DILocation(line: 87, column: 1391, scope: !2484)
!2487 = !DILocation(line: 87, column: 1447, scope: !2484)
!2488 = distinct !DISubprogram(name: "_CNW7builtinE8ctpop_u8EhL0", linkageName: "_CNW7builtinE8ctpop_u8EhL0", scope: null, file: !6, line: 87, type: !1406, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2489 = !DILocalVariable(name: "val", scope: !2488, file: !1, line: 87, type: !53)
!2490 = !DILocation(line: 87, column: 1534, scope: !2488)
!2491 = !DILocation(line: 87, column: 1591, scope: !2488)
!2492 = distinct !DISubprogram(name: "_CNW7builtinE13bitreverse_u8EhL0", linkageName: "_CNW7builtinE13bitreverse_u8EhL0", scope: null, file: !6, line: 87, type: !1406, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2493 = !DILocalVariable(name: "val", scope: !2492, file: !1, line: 87, type: !53)
!2494 = !DILocation(line: 87, column: 1672, scope: !2492)
!2495 = !DILocation(line: 87, column: 1734, scope: !2492)
!2496 = distinct !DISubprogram(name: "_CNW7builtinE21sadd_with_overflow_u8EhhL0L0", linkageName: "_CNW7builtinE21sadd_with_overflow_u8EhhL0L0", scope: null, file: !6, line: 87, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2497 = !DILocalVariable(name: "lhs", scope: !2496, file: !1, line: 87, type: !53)
!2498 = !DILocation(line: 87, column: 3476, scope: !2496)
!2499 = !DILocalVariable(name: "rhs", scope: !2496, file: !1, line: 87, type: !53)
!2500 = !DILocation(line: 87, column: 3566, scope: !2496)
!2501 = distinct !DISubprogram(name: "_CNW7builtinE21uadd_with_overflow_u8EhhL0L0", linkageName: "_CNW7builtinE21uadd_with_overflow_u8EhhL0L0", scope: null, file: !6, line: 87, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2502 = !DILocalVariable(name: "lhs", scope: !2501, file: !1, line: 87, type: !53)
!2503 = !DILocation(line: 87, column: 3665, scope: !2501)
!2504 = !DILocalVariable(name: "rhs", scope: !2501, file: !1, line: 87, type: !53)
!2505 = !DILocation(line: 87, column: 3755, scope: !2501)
!2506 = distinct !DISubprogram(name: "_CNW7builtinE21ssub_with_overflow_u8EhhL0L0", linkageName: "_CNW7builtinE21ssub_with_overflow_u8EhhL0L0", scope: null, file: !6, line: 87, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2507 = !DILocalVariable(name: "lhs", scope: !2506, file: !1, line: 87, type: !53)
!2508 = !DILocation(line: 87, column: 3854, scope: !2506)
!2509 = !DILocalVariable(name: "rhs", scope: !2506, file: !1, line: 87, type: !53)
!2510 = !DILocation(line: 87, column: 3944, scope: !2506)
!2511 = distinct !DISubprogram(name: "_CNW7builtinE21usub_with_overflow_u8EhhL0L0", linkageName: "_CNW7builtinE21usub_with_overflow_u8EhhL0L0", scope: null, file: !6, line: 87, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2512 = !DILocalVariable(name: "lhs", scope: !2511, file: !1, line: 87, type: !53)
!2513 = !DILocation(line: 87, column: 4043, scope: !2511)
!2514 = !DILocalVariable(name: "rhs", scope: !2511, file: !1, line: 87, type: !53)
!2515 = !DILocation(line: 87, column: 4133, scope: !2511)
!2516 = distinct !DISubprogram(name: "_CNW7builtinE21smul_with_overflow_u8EhhL0L0", linkageName: "_CNW7builtinE21smul_with_overflow_u8EhhL0L0", scope: null, file: !6, line: 87, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2517 = !DILocalVariable(name: "lhs", scope: !2516, file: !1, line: 87, type: !53)
!2518 = !DILocation(line: 87, column: 4232, scope: !2516)
!2519 = !DILocalVariable(name: "rhs", scope: !2516, file: !1, line: 87, type: !53)
!2520 = !DILocation(line: 87, column: 4322, scope: !2516)
!2521 = distinct !DISubprogram(name: "_CNW7builtinE21umul_with_overflow_u8EhhL0L0", linkageName: "_CNW7builtinE21umul_with_overflow_u8EhhL0L0", scope: null, file: !6, line: 87, type: !51, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2522 = !DILocalVariable(name: "lhs", scope: !2521, file: !1, line: 87, type: !53)
!2523 = !DILocation(line: 87, column: 4421, scope: !2521)
!2524 = !DILocalVariable(name: "rhs", scope: !2521, file: !1, line: 87, type: !53)
!2525 = !DILocation(line: 87, column: 4511, scope: !2521)
!2526 = distinct !DISubprogram(name: "_CNW7builtinE8ctlz_u16EtL0", linkageName: "_CNW7builtinE8ctlz_u16EtL0", scope: null, file: !6, line: 87, type: !1428, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2527 = !DILocalVariable(name: "val", scope: !2526, file: !1, line: 87, type: !85)
!2528 = !DILocation(line: 87, column: 1248, scope: !2526)
!2529 = !DILocation(line: 87, column: 1304, scope: !2526)
!2530 = distinct !DISubprogram(name: "_CNW7builtinE8cttz_u16EtL0", linkageName: "_CNW7builtinE8cttz_u16EtL0", scope: null, file: !6, line: 87, type: !1428, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2531 = !DILocalVariable(name: "val", scope: !2530, file: !1, line: 87, type: !85)
!2532 = !DILocation(line: 87, column: 1391, scope: !2530)
!2533 = !DILocation(line: 87, column: 1447, scope: !2530)
!2534 = distinct !DISubprogram(name: "_CNW7builtinE9ctpop_u16EtL0", linkageName: "_CNW7builtinE9ctpop_u16EtL0", scope: null, file: !6, line: 87, type: !1428, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2535 = !DILocalVariable(name: "val", scope: !2534, file: !1, line: 87, type: !85)
!2536 = !DILocation(line: 87, column: 1534, scope: !2534)
!2537 = !DILocation(line: 87, column: 1591, scope: !2534)
!2538 = distinct !DISubprogram(name: "_CNW7builtinE14bitreverse_u16EtL0", linkageName: "_CNW7builtinE14bitreverse_u16EtL0", scope: null, file: !6, line: 87, type: !1428, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2539 = !DILocalVariable(name: "val", scope: !2538, file: !1, line: 87, type: !85)
!2540 = !DILocation(line: 87, column: 1672, scope: !2538)
!2541 = !DILocation(line: 87, column: 1734, scope: !2538)
!2542 = distinct !DISubprogram(name: "_CNW7builtinE22sadd_with_overflow_u16EttL0L0", linkageName: "_CNW7builtinE22sadd_with_overflow_u16EttL0L0", scope: null, file: !6, line: 87, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2543 = !DILocalVariable(name: "lhs", scope: !2542, file: !1, line: 87, type: !85)
!2544 = !DILocation(line: 87, column: 3476, scope: !2542)
!2545 = !DILocalVariable(name: "rhs", scope: !2542, file: !1, line: 87, type: !85)
!2546 = !DILocation(line: 87, column: 3566, scope: !2542)
!2547 = distinct !DISubprogram(name: "_CNW7builtinE22uadd_with_overflow_u16EttL0L0", linkageName: "_CNW7builtinE22uadd_with_overflow_u16EttL0L0", scope: null, file: !6, line: 87, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2548 = !DILocalVariable(name: "lhs", scope: !2547, file: !1, line: 87, type: !85)
!2549 = !DILocation(line: 87, column: 3665, scope: !2547)
!2550 = !DILocalVariable(name: "rhs", scope: !2547, file: !1, line: 87, type: !85)
!2551 = !DILocation(line: 87, column: 3755, scope: !2547)
!2552 = distinct !DISubprogram(name: "_CNW7builtinE22ssub_with_overflow_u16EttL0L0", linkageName: "_CNW7builtinE22ssub_with_overflow_u16EttL0L0", scope: null, file: !6, line: 87, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2553 = !DILocalVariable(name: "lhs", scope: !2552, file: !1, line: 87, type: !85)
!2554 = !DILocation(line: 87, column: 3854, scope: !2552)
!2555 = !DILocalVariable(name: "rhs", scope: !2552, file: !1, line: 87, type: !85)
!2556 = !DILocation(line: 87, column: 3944, scope: !2552)
!2557 = distinct !DISubprogram(name: "_CNW7builtinE22usub_with_overflow_u16EttL0L0", linkageName: "_CNW7builtinE22usub_with_overflow_u16EttL0L0", scope: null, file: !6, line: 87, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2558 = !DILocalVariable(name: "lhs", scope: !2557, file: !1, line: 87, type: !85)
!2559 = !DILocation(line: 87, column: 4043, scope: !2557)
!2560 = !DILocalVariable(name: "rhs", scope: !2557, file: !1, line: 87, type: !85)
!2561 = !DILocation(line: 87, column: 4133, scope: !2557)
!2562 = distinct !DISubprogram(name: "_CNW7builtinE22smul_with_overflow_u16EttL0L0", linkageName: "_CNW7builtinE22smul_with_overflow_u16EttL0L0", scope: null, file: !6, line: 87, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2563 = !DILocalVariable(name: "lhs", scope: !2562, file: !1, line: 87, type: !85)
!2564 = !DILocation(line: 87, column: 4232, scope: !2562)
!2565 = !DILocalVariable(name: "rhs", scope: !2562, file: !1, line: 87, type: !85)
!2566 = !DILocation(line: 87, column: 4322, scope: !2562)
!2567 = distinct !DISubprogram(name: "_CNW7builtinE22umul_with_overflow_u16EttL0L0", linkageName: "_CNW7builtinE22umul_with_overflow_u16EttL0L0", scope: null, file: !6, line: 87, type: !83, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2568 = !DILocalVariable(name: "lhs", scope: !2567, file: !1, line: 87, type: !85)
!2569 = !DILocation(line: 87, column: 4421, scope: !2567)
!2570 = !DILocalVariable(name: "rhs", scope: !2567, file: !1, line: 87, type: !85)
!2571 = !DILocation(line: 87, column: 4511, scope: !2567)
!2572 = distinct !DISubprogram(name: "_CNW7builtinE8ctlz_u32EjL0", linkageName: "_CNW7builtinE8ctlz_u32EjL0", scope: null, file: !6, line: 87, type: !1450, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2573 = !DILocalVariable(name: "val", scope: !2572, file: !1, line: 87, type: !117)
!2574 = !DILocation(line: 87, column: 1248, scope: !2572)
!2575 = !DILocation(line: 87, column: 1304, scope: !2572)
!2576 = distinct !DISubprogram(name: "_CNW7builtinE8cttz_u32EjL0", linkageName: "_CNW7builtinE8cttz_u32EjL0", scope: null, file: !6, line: 87, type: !1450, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2577 = !DILocalVariable(name: "val", scope: !2576, file: !1, line: 87, type: !117)
!2578 = !DILocation(line: 87, column: 1391, scope: !2576)
!2579 = !DILocation(line: 87, column: 1447, scope: !2576)
!2580 = distinct !DISubprogram(name: "_CNW7builtinE9ctpop_u32EjL0", linkageName: "_CNW7builtinE9ctpop_u32EjL0", scope: null, file: !6, line: 87, type: !1450, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2581 = !DILocalVariable(name: "val", scope: !2580, file: !1, line: 87, type: !117)
!2582 = !DILocation(line: 87, column: 1534, scope: !2580)
!2583 = !DILocation(line: 87, column: 1591, scope: !2580)
!2584 = distinct !DISubprogram(name: "_CNW7builtinE14bitreverse_u32EjL0", linkageName: "_CNW7builtinE14bitreverse_u32EjL0", scope: null, file: !6, line: 87, type: !1450, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2585 = !DILocalVariable(name: "val", scope: !2584, file: !1, line: 87, type: !117)
!2586 = !DILocation(line: 87, column: 1672, scope: !2584)
!2587 = !DILocation(line: 87, column: 1734, scope: !2584)
!2588 = distinct !DISubprogram(name: "_CNW7builtinE22sadd_with_overflow_u32EjjL0L0", linkageName: "_CNW7builtinE22sadd_with_overflow_u32EjjL0L0", scope: null, file: !6, line: 87, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2589 = !DILocalVariable(name: "lhs", scope: !2588, file: !1, line: 87, type: !117)
!2590 = !DILocation(line: 87, column: 3476, scope: !2588)
!2591 = !DILocalVariable(name: "rhs", scope: !2588, file: !1, line: 87, type: !117)
!2592 = !DILocation(line: 87, column: 3566, scope: !2588)
!2593 = distinct !DISubprogram(name: "_CNW7builtinE22uadd_with_overflow_u32EjjL0L0", linkageName: "_CNW7builtinE22uadd_with_overflow_u32EjjL0L0", scope: null, file: !6, line: 87, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2594 = !DILocalVariable(name: "lhs", scope: !2593, file: !1, line: 87, type: !117)
!2595 = !DILocation(line: 87, column: 3665, scope: !2593)
!2596 = !DILocalVariable(name: "rhs", scope: !2593, file: !1, line: 87, type: !117)
!2597 = !DILocation(line: 87, column: 3755, scope: !2593)
!2598 = distinct !DISubprogram(name: "_CNW7builtinE22ssub_with_overflow_u32EjjL0L0", linkageName: "_CNW7builtinE22ssub_with_overflow_u32EjjL0L0", scope: null, file: !6, line: 87, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2599 = !DILocalVariable(name: "lhs", scope: !2598, file: !1, line: 87, type: !117)
!2600 = !DILocation(line: 87, column: 3854, scope: !2598)
!2601 = !DILocalVariable(name: "rhs", scope: !2598, file: !1, line: 87, type: !117)
!2602 = !DILocation(line: 87, column: 3944, scope: !2598)
!2603 = distinct !DISubprogram(name: "_CNW7builtinE22usub_with_overflow_u32EjjL0L0", linkageName: "_CNW7builtinE22usub_with_overflow_u32EjjL0L0", scope: null, file: !6, line: 87, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2604 = !DILocalVariable(name: "lhs", scope: !2603, file: !1, line: 87, type: !117)
!2605 = !DILocation(line: 87, column: 4043, scope: !2603)
!2606 = !DILocalVariable(name: "rhs", scope: !2603, file: !1, line: 87, type: !117)
!2607 = !DILocation(line: 87, column: 4133, scope: !2603)
!2608 = distinct !DISubprogram(name: "_CNW7builtinE22smul_with_overflow_u32EjjL0L0", linkageName: "_CNW7builtinE22smul_with_overflow_u32EjjL0L0", scope: null, file: !6, line: 87, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2609 = !DILocalVariable(name: "lhs", scope: !2608, file: !1, line: 87, type: !117)
!2610 = !DILocation(line: 87, column: 4232, scope: !2608)
!2611 = !DILocalVariable(name: "rhs", scope: !2608, file: !1, line: 87, type: !117)
!2612 = !DILocation(line: 87, column: 4322, scope: !2608)
!2613 = distinct !DISubprogram(name: "_CNW7builtinE22umul_with_overflow_u32EjjL0L0", linkageName: "_CNW7builtinE22umul_with_overflow_u32EjjL0L0", scope: null, file: !6, line: 87, type: !115, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2614 = !DILocalVariable(name: "lhs", scope: !2613, file: !1, line: 87, type: !117)
!2615 = !DILocation(line: 87, column: 4421, scope: !2613)
!2616 = !DILocalVariable(name: "rhs", scope: !2613, file: !1, line: 87, type: !117)
!2617 = !DILocation(line: 87, column: 4511, scope: !2613)
!2618 = distinct !DISubprogram(name: "_CNW7builtinE8ctlz_u64EmL0", linkageName: "_CNW7builtinE8ctlz_u64EmL0", scope: null, file: !6, line: 87, type: !1472, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2619 = !DILocalVariable(name: "val", scope: !2618, file: !1, line: 87, type: !149)
!2620 = !DILocation(line: 87, column: 1248, scope: !2618)
!2621 = !DILocation(line: 87, column: 1304, scope: !2618)
!2622 = distinct !DISubprogram(name: "_CNW7builtinE8cttz_u64EmL0", linkageName: "_CNW7builtinE8cttz_u64EmL0", scope: null, file: !6, line: 87, type: !1472, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2623 = !DILocalVariable(name: "val", scope: !2622, file: !1, line: 87, type: !149)
!2624 = !DILocation(line: 87, column: 1391, scope: !2622)
!2625 = !DILocation(line: 87, column: 1447, scope: !2622)
!2626 = distinct !DISubprogram(name: "_CNW7builtinE9ctpop_u64EmL0", linkageName: "_CNW7builtinE9ctpop_u64EmL0", scope: null, file: !6, line: 87, type: !1472, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2627 = !DILocalVariable(name: "val", scope: !2626, file: !1, line: 87, type: !149)
!2628 = !DILocation(line: 87, column: 1534, scope: !2626)
!2629 = !DILocation(line: 87, column: 1591, scope: !2626)
!2630 = distinct !DISubprogram(name: "_CNW7builtinE14bitreverse_u64EmL0", linkageName: "_CNW7builtinE14bitreverse_u64EmL0", scope: null, file: !6, line: 87, type: !1472, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2631 = !DILocalVariable(name: "val", scope: !2630, file: !1, line: 87, type: !149)
!2632 = !DILocation(line: 87, column: 1672, scope: !2630)
!2633 = !DILocation(line: 87, column: 1734, scope: !2630)
!2634 = distinct !DISubprogram(name: "_CNW7builtinE22sadd_with_overflow_u64EmmL0L0", linkageName: "_CNW7builtinE22sadd_with_overflow_u64EmmL0L0", scope: null, file: !6, line: 87, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2635 = !DILocalVariable(name: "lhs", scope: !2634, file: !1, line: 87, type: !149)
!2636 = !DILocation(line: 87, column: 3476, scope: !2634)
!2637 = !DILocalVariable(name: "rhs", scope: !2634, file: !1, line: 87, type: !149)
!2638 = !DILocation(line: 87, column: 3566, scope: !2634)
!2639 = distinct !DISubprogram(name: "_CNW7builtinE22uadd_with_overflow_u64EmmL0L0", linkageName: "_CNW7builtinE22uadd_with_overflow_u64EmmL0L0", scope: null, file: !6, line: 87, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2640 = !DILocalVariable(name: "lhs", scope: !2639, file: !1, line: 87, type: !149)
!2641 = !DILocation(line: 87, column: 3665, scope: !2639)
!2642 = !DILocalVariable(name: "rhs", scope: !2639, file: !1, line: 87, type: !149)
!2643 = !DILocation(line: 87, column: 3755, scope: !2639)
!2644 = distinct !DISubprogram(name: "_CNW7builtinE22ssub_with_overflow_u64EmmL0L0", linkageName: "_CNW7builtinE22ssub_with_overflow_u64EmmL0L0", scope: null, file: !6, line: 87, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2645 = !DILocalVariable(name: "lhs", scope: !2644, file: !1, line: 87, type: !149)
!2646 = !DILocation(line: 87, column: 3854, scope: !2644)
!2647 = !DILocalVariable(name: "rhs", scope: !2644, file: !1, line: 87, type: !149)
!2648 = !DILocation(line: 87, column: 3944, scope: !2644)
!2649 = distinct !DISubprogram(name: "_CNW7builtinE22usub_with_overflow_u64EmmL0L0", linkageName: "_CNW7builtinE22usub_with_overflow_u64EmmL0L0", scope: null, file: !6, line: 87, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2650 = !DILocalVariable(name: "lhs", scope: !2649, file: !1, line: 87, type: !149)
!2651 = !DILocation(line: 87, column: 4043, scope: !2649)
!2652 = !DILocalVariable(name: "rhs", scope: !2649, file: !1, line: 87, type: !149)
!2653 = !DILocation(line: 87, column: 4133, scope: !2649)
!2654 = distinct !DISubprogram(name: "_CNW7builtinE22smul_with_overflow_u64EmmL0L0", linkageName: "_CNW7builtinE22smul_with_overflow_u64EmmL0L0", scope: null, file: !6, line: 87, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2655 = !DILocalVariable(name: "lhs", scope: !2654, file: !1, line: 87, type: !149)
!2656 = !DILocation(line: 87, column: 4232, scope: !2654)
!2657 = !DILocalVariable(name: "rhs", scope: !2654, file: !1, line: 87, type: !149)
!2658 = !DILocation(line: 87, column: 4322, scope: !2654)
!2659 = distinct !DISubprogram(name: "_CNW7builtinE22umul_with_overflow_u64EmmL0L0", linkageName: "_CNW7builtinE22umul_with_overflow_u64EmmL0L0", scope: null, file: !6, line: 87, type: !147, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2660 = !DILocalVariable(name: "lhs", scope: !2659, file: !1, line: 87, type: !149)
!2661 = !DILocation(line: 87, column: 4421, scope: !2659)
!2662 = !DILocalVariable(name: "rhs", scope: !2659, file: !1, line: 87, type: !149)
!2663 = !DILocation(line: 87, column: 4511, scope: !2659)
!2664 = distinct !DISubprogram(name: "_CNW7builtinE9ctlz_u128EoL0", linkageName: "_CNW7builtinE9ctlz_u128EoL0", scope: null, file: !6, line: 87, type: !1494, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2665 = !DILocalVariable(name: "val", scope: !2664, file: !1, line: 87, type: !181)
!2666 = !DILocation(line: 87, column: 1248, scope: !2664)
!2667 = !DILocation(line: 87, column: 1304, scope: !2664)
!2668 = distinct !DISubprogram(name: "_CNW7builtinE9cttz_u128EoL0", linkageName: "_CNW7builtinE9cttz_u128EoL0", scope: null, file: !6, line: 87, type: !1494, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2669 = !DILocalVariable(name: "val", scope: !2668, file: !1, line: 87, type: !181)
!2670 = !DILocation(line: 87, column: 1391, scope: !2668)
!2671 = !DILocation(line: 87, column: 1447, scope: !2668)
!2672 = distinct !DISubprogram(name: "_CNW7builtinE10ctpop_u128EoL0", linkageName: "_CNW7builtinE10ctpop_u128EoL0", scope: null, file: !6, line: 87, type: !1494, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2673 = !DILocalVariable(name: "val", scope: !2672, file: !1, line: 87, type: !181)
!2674 = !DILocation(line: 87, column: 1534, scope: !2672)
!2675 = !DILocation(line: 87, column: 1591, scope: !2672)
!2676 = distinct !DISubprogram(name: "_CNW7builtinE15bitreverse_u128EoL0", linkageName: "_CNW7builtinE15bitreverse_u128EoL0", scope: null, file: !6, line: 87, type: !1494, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2677 = !DILocalVariable(name: "val", scope: !2676, file: !1, line: 87, type: !181)
!2678 = !DILocation(line: 87, column: 1672, scope: !2676)
!2679 = !DILocation(line: 87, column: 1734, scope: !2676)
!2680 = distinct !DISubprogram(name: "_CNW7builtinE23sadd_with_overflow_u128EooL0L0", linkageName: "_CNW7builtinE23sadd_with_overflow_u128EooL0L0", scope: null, file: !6, line: 87, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2681 = !DILocalVariable(name: "lhs", scope: !2680, file: !1, line: 87, type: !181)
!2682 = !DILocation(line: 87, column: 3476, scope: !2680)
!2683 = !DILocalVariable(name: "rhs", scope: !2680, file: !1, line: 87, type: !181)
!2684 = !DILocation(line: 87, column: 3566, scope: !2680)
!2685 = distinct !DISubprogram(name: "_CNW7builtinE23uadd_with_overflow_u128EooL0L0", linkageName: "_CNW7builtinE23uadd_with_overflow_u128EooL0L0", scope: null, file: !6, line: 87, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2686 = !DILocalVariable(name: "lhs", scope: !2685, file: !1, line: 87, type: !181)
!2687 = !DILocation(line: 87, column: 3665, scope: !2685)
!2688 = !DILocalVariable(name: "rhs", scope: !2685, file: !1, line: 87, type: !181)
!2689 = !DILocation(line: 87, column: 3755, scope: !2685)
!2690 = distinct !DISubprogram(name: "_CNW7builtinE23ssub_with_overflow_u128EooL0L0", linkageName: "_CNW7builtinE23ssub_with_overflow_u128EooL0L0", scope: null, file: !6, line: 87, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2691 = !DILocalVariable(name: "lhs", scope: !2690, file: !1, line: 87, type: !181)
!2692 = !DILocation(line: 87, column: 3854, scope: !2690)
!2693 = !DILocalVariable(name: "rhs", scope: !2690, file: !1, line: 87, type: !181)
!2694 = !DILocation(line: 87, column: 3944, scope: !2690)
!2695 = distinct !DISubprogram(name: "_CNW7builtinE23usub_with_overflow_u128EooL0L0", linkageName: "_CNW7builtinE23usub_with_overflow_u128EooL0L0", scope: null, file: !6, line: 87, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2696 = !DILocalVariable(name: "lhs", scope: !2695, file: !1, line: 87, type: !181)
!2697 = !DILocation(line: 87, column: 4043, scope: !2695)
!2698 = !DILocalVariable(name: "rhs", scope: !2695, file: !1, line: 87, type: !181)
!2699 = !DILocation(line: 87, column: 4133, scope: !2695)
!2700 = distinct !DISubprogram(name: "_CNW7builtinE23smul_with_overflow_u128EooL0L0", linkageName: "_CNW7builtinE23smul_with_overflow_u128EooL0L0", scope: null, file: !6, line: 87, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2701 = !DILocalVariable(name: "lhs", scope: !2700, file: !1, line: 87, type: !181)
!2702 = !DILocation(line: 87, column: 4232, scope: !2700)
!2703 = !DILocalVariable(name: "rhs", scope: !2700, file: !1, line: 87, type: !181)
!2704 = !DILocation(line: 87, column: 4322, scope: !2700)
!2705 = distinct !DISubprogram(name: "_CNW7builtinE23umul_with_overflow_u128EooL0L0", linkageName: "_CNW7builtinE23umul_with_overflow_u128EooL0L0", scope: null, file: !6, line: 87, type: !179, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2706 = !DILocalVariable(name: "lhs", scope: !2705, file: !1, line: 87, type: !181)
!2707 = !DILocation(line: 87, column: 4421, scope: !2705)
!2708 = !DILocalVariable(name: "rhs", scope: !2705, file: !1, line: 87, type: !181)
!2709 = !DILocation(line: 87, column: 4511, scope: !2705)
!2710 = distinct !DISubprogram(name: "_CNW7builtinE9bswap_i16EsL0", linkageName: "_CNW7builtinE9bswap_i16EsL0", scope: null, file: !6, line: 87, type: !1417, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2711 = !DILocalVariable(name: "val", scope: !2710, file: !1, line: 87, type: !69)
!2712 = !DILocation(line: 87, column: 4546, scope: !2710)
!2713 = !DILocation(line: 87, column: 4603, scope: !2710)
!2714 = distinct !DISubprogram(name: "_CNW7builtinE9bswap_i32EiL0", linkageName: "_CNW7builtinE9bswap_i32EiL0", scope: null, file: !6, line: 87, type: !1439, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2715 = !DILocalVariable(name: "val", scope: !2714, file: !1, line: 87, type: !101)
!2716 = !DILocation(line: 87, column: 4546, scope: !2714)
!2717 = !DILocation(line: 87, column: 4603, scope: !2714)
!2718 = distinct !DISubprogram(name: "_CNW7builtinE9bswap_i64ElL0", linkageName: "_CNW7builtinE9bswap_i64ElL0", scope: null, file: !6, line: 87, type: !1461, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2719 = !DILocalVariable(name: "val", scope: !2718, file: !1, line: 87, type: !133)
!2720 = !DILocation(line: 87, column: 4546, scope: !2718)
!2721 = !DILocation(line: 87, column: 4603, scope: !2718)
!2722 = distinct !DISubprogram(name: "_CNW7builtinE10bswap_i128EnL0", linkageName: "_CNW7builtinE10bswap_i128EnL0", scope: null, file: !6, line: 87, type: !1483, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2723 = !DILocalVariable(name: "val", scope: !2722, file: !1, line: 87, type: !165)
!2724 = !DILocation(line: 87, column: 4546, scope: !2722)
!2725 = !DILocation(line: 87, column: 4603, scope: !2722)
!2726 = distinct !DISubprogram(name: "_CNW7builtinE9bswap_u16EtL0", linkageName: "_CNW7builtinE9bswap_u16EtL0", scope: null, file: !6, line: 87, type: !1428, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2727 = !DILocalVariable(name: "val", scope: !2726, file: !1, line: 87, type: !85)
!2728 = !DILocation(line: 87, column: 4546, scope: !2726)
!2729 = !DILocation(line: 87, column: 4603, scope: !2726)
!2730 = distinct !DISubprogram(name: "_CNW7builtinE9bswap_u32EjL0", linkageName: "_CNW7builtinE9bswap_u32EjL0", scope: null, file: !6, line: 87, type: !1450, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2731 = !DILocalVariable(name: "val", scope: !2730, file: !1, line: 87, type: !117)
!2732 = !DILocation(line: 87, column: 4546, scope: !2730)
!2733 = !DILocation(line: 87, column: 4603, scope: !2730)
!2734 = distinct !DISubprogram(name: "_CNW7builtinE9bswap_u64EmL0", linkageName: "_CNW7builtinE9bswap_u64EmL0", scope: null, file: !6, line: 87, type: !1472, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2735 = !DILocalVariable(name: "val", scope: !2734, file: !1, line: 87, type: !149)
!2736 = !DILocation(line: 87, column: 4546, scope: !2734)
!2737 = !DILocation(line: 87, column: 4603, scope: !2734)
!2738 = distinct !DISubprogram(name: "_CNW7builtinE10bswap_u128EoL0", linkageName: "_CNW7builtinE10bswap_u128EoL0", scope: null, file: !6, line: 87, type: !1494, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2739 = !DILocalVariable(name: "val", scope: !2738, file: !1, line: 87, type: !181)
!2740 = !DILocation(line: 87, column: 4546, scope: !2738)
!2741 = !DILocation(line: 87, column: 4603, scope: !2738)
!2742 = distinct !DISubprogram(name: "_CNW7builtinE4Bool4copyES0_L0", linkageName: "_CNW7builtinE4Bool4copyES0_L0", scope: !0, file: !1913, line: 2, type: !1926, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2743 = !DILocation(line: 2, column: 1, scope: !2742)
!2744 = distinct !DISubprogram(name: "_CNW7builtinE4Bool14__default_initES0_L0", linkageName: "_CNW7builtinE4Bool14__default_initES0_L0", scope: !0, file: !1913, line: 2, type: !1926, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2745 = !DILocation(line: 2, column: 1, scope: !2744)
!2746 = distinct !DISubprogram(name: "_CNW7builtinE4BoolC1EbL5value", linkageName: "_CNW7builtinE4BoolC1EbL5value", scope: !0, file: !1913, line: 2, type: !2747, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2747 = !DISubroutineType(types: !2748)
!2748 = !{!1936, !1597}
!2749 = !DILocation(line: 2, column: 1, scope: !2746)
!2750 = distinct !DISubprogram(name: "_CNW7builtinE4BoolC2ERS0_bL0L5value", linkageName: "_CNW7builtinE4BoolC2ERS0_bL0L5value", scope: !0, file: !1913, line: 2, type: !2747, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2751 = !DILocation(line: 2, column: 1, scope: !2750)
!2752 = distinct !DISubprogram(name: "_CNW7builtinE4BoolD3Ev", linkageName: "_CNW7builtinE4BoolD3Ev", scope: null, file: !1913, line: 2, type: !1934, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2753 = !DILocation(line: 2, column: 1, scope: !2752)
!2754 = distinct !DISubprogram(name: "_CNW7builtinE17ValueWitnessTableD0Ev", linkageName: "_CNW7builtinE17ValueWitnessTableD0Ev", scope: !0, file: !1913, line: 2, type: !2755, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2755 = !DISubroutineType(types: !2756)
!2756 = !{!2757}
!2757 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !2758, size: 64, align: 8)
!2758 = !DICompositeType(tag: DW_TAG_structure_type, name: "builtin.<private name>.ValueWitnessTable", file: !1913, line: 2, flags: DIFlagFwdDecl, elements: !2759)
!2759 = !{!2760, !2762}
!2760 = !DIDerivedType(tag: DW_TAG_member, name: "copyFn", scope: !2758, file: !1913, line: 2, baseType: !2761, size: 64, align: 64)
!2761 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 64, align: 8)
!2762 = !DIDerivedType(tag: DW_TAG_member, name: "deinit", scope: !2758, file: !1913, line: 2, baseType: !2763, size: 64, align: 64, offset: 64)
!2763 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1959, size: 64, align: 8)
!2764 = !DILocation(line: 2, column: 1, scope: !2754)
!2765 = distinct !DISubprogram(name: "_CNW7builtinE17ValueWitnessTable14__default_initES0_L0", linkageName: "_CNW7builtinE17ValueWitnessTable14__default_initES0_L0", scope: !0, file: !1913, line: 2, type: !2766, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2766 = !DISubroutineType(types: !2767)
!2767 = !{!2758}
!2768 = !DILocation(line: 2, column: 1, scope: !2765)
!2769 = distinct !DISubprogram(name: "_CNW7builtinE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit", linkageName: "_CNW7builtinE17ValueWitnessTableC1EDoFvPvKPvEDoFvS1_EL6copyFnL6deinit", scope: !0, file: !1913, line: 2, type: !2770, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2770 = !DISubroutineType(types: !2771)
!2771 = !{!2757, !2761, !2763}
!2772 = !DILocation(line: 2, column: 1, scope: !2769)
!2773 = distinct !DISubprogram(name: "_CNW7builtinE17ValueWitnessTableC2ERS0_DoFvPvKPvEDoFvS2_EL0L6copyFnL6deinit", linkageName: "_CNW7builtinE17ValueWitnessTableC2ERS0_DoFvPvKPvEDoFvS2_EL0L6copyFnL6deinit", scope: !0, file: !1913, line: 2, type: !2770, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2774 = !DILocation(line: 2, column: 1, scope: !2773)
!2775 = distinct !DISubprogram(name: "_CNW7builtinE17ValueWitnessTableD3Ev", linkageName: "_CNW7builtinE17ValueWitnessTableD3Ev", scope: null, file: !1913, line: 2, type: !2755, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2776 = !DILocation(line: 2, column: 1, scope: !2775)
!2777 = distinct !DISubprogram(name: "_CNW7builtinE19ProtocolConformanceD0Ev", linkageName: "_CNW7builtinE19ProtocolConformanceD0Ev", scope: !0, file: !1913, line: 2, type: !2778, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2778 = !DISubroutineType(types: !2779)
!2779 = !{!2780}
!2780 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !2781, size: 64, align: 8)
!2781 = !DICompositeType(tag: DW_TAG_structure_type, name: "builtin.<private name>.ProtocolConformance", file: !1913, line: 2, flags: DIFlagFwdDecl, elements: !2782)
!2782 = !{!2783, !2800}
!2783 = !DIDerivedType(tag: DW_TAG_member, name: "typeInfo", scope: !2781, file: !1913, line: 2, baseType: !2784, size: 64, align: 64)
!2784 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !2785, size: 64, align: 8)
!2785 = !DICompositeType(tag: DW_TAG_structure_type, name: "builtin.<private name>.TypeInfo", file: !1913, line: 2, flags: DIFlagFwdDecl, elements: !2786)
!2786 = !{!2787, !2788, !2790, !2791, !2793, !2795, !2797, !2798, !2799}
!2787 = !DIDerivedType(tag: DW_TAG_member, name: "baseClass", scope: !2785, file: !1913, line: 2, baseType: !2784, size: 64, align: 64)
!2788 = !DIDerivedType(tag: DW_TAG_member, name: "vtable", scope: !2785, file: !1913, line: 2, baseType: !2789, size: 64, align: 64, offset: 64)
!2789 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !9, size: 64, align: 8)
!2790 = !DIDerivedType(tag: DW_TAG_member, name: "deinit", scope: !2785, file: !1913, line: 2, baseType: !2763, size: 64, align: 64, offset: 128)
!2791 = !DIDerivedType(tag: DW_TAG_member, name: "name", scope: !2785, file: !1913, line: 2, baseType: !2792, size: 64, align: 64, offset: 192)
!2792 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !53, size: 64, align: 8)
!2793 = !DIDerivedType(tag: DW_TAG_member, name: "valueWitnessTable", scope: !2785, file: !1913, line: 2, baseType: !2794, size: 64, align: 64, offset: 256)
!2794 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !2758, size: 64, align: 8)
!2795 = !DIDerivedType(tag: DW_TAG_member, name: "conformances", scope: !2785, file: !1913, line: 2, baseType: !2796, size: 64, align: 64, offset: 320)
!2796 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !2781, size: 64, align: 8)
!2797 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !2785, file: !1913, line: 2, baseType: !149, size: 64, align: 64, offset: 384)
!2798 = !DIDerivedType(tag: DW_TAG_member, name: "alignment", scope: !2785, file: !1913, line: 2, baseType: !149, size: 64, align: 64, offset: 448)
!2799 = !DIDerivedType(tag: DW_TAG_member, name: "stride", scope: !2785, file: !1913, line: 2, baseType: !149, size: 64, align: 64, offset: 512)
!2800 = !DIDerivedType(tag: DW_TAG_member, name: "vtable", scope: !2781, file: !1913, line: 2, baseType: !2789, size: 64, align: 64, offset: 64)
!2801 = !DILocation(line: 2, column: 1, scope: !2777)
!2802 = distinct !DISubprogram(name: "_CNW7builtinE19ProtocolConformance14__default_initES0_L0", linkageName: "_CNW7builtinE19ProtocolConformance14__default_initES0_L0", scope: !0, file: !1913, line: 2, type: !2803, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2803 = !DISubroutineType(types: !2804)
!2804 = !{!2781}
!2805 = !DILocation(line: 2, column: 1, scope: !2802)
!2806 = distinct !DISubprogram(name: "_CNW7builtinE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable", linkageName: "_CNW7builtinE19ProtocolConformanceC1EKPNS_8TypeInfoEKPKPvL8typeInfoL6vtable", scope: !0, file: !1913, line: 2, type: !2807, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2807 = !DISubroutineType(types: !2808)
!2808 = !{!2780, !2784, !2789}
!2809 = !DILocation(line: 2, column: 1, scope: !2806)
!2810 = distinct !DISubprogram(name: "_CNW7builtinE19ProtocolConformanceC2ERS0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable", linkageName: "_CNW7builtinE19ProtocolConformanceC2ERS0_KPNS_8TypeInfoEKPKPvL0L8typeInfoL6vtable", scope: !0, file: !1913, line: 2, type: !2807, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2811 = !DILocation(line: 2, column: 1, scope: !2810)
!2812 = distinct !DISubprogram(name: "_CNW7builtinE19ProtocolConformanceD3Ev", linkageName: "_CNW7builtinE19ProtocolConformanceD3Ev", scope: null, file: !1913, line: 2, type: !2778, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2813 = !DILocation(line: 2, column: 1, scope: !2812)
!2814 = distinct !DISubprogram(name: "_CNW7builtinE8TypeInfoD0Ev", linkageName: "_CNW7builtinE8TypeInfoD0Ev", scope: !0, file: !1913, line: 2, type: !2815, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2815 = !DISubroutineType(types: !2816)
!2816 = !{!2817}
!2817 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !2785, size: 64, align: 8)
!2818 = !DILocation(line: 2, column: 1, scope: !2814)
!2819 = distinct !DISubprogram(name: "_CNW7builtinE8TypeInfo14__default_initES0_L0", linkageName: "_CNW7builtinE8TypeInfo14__default_initES0_L0", scope: !0, file: !1913, line: 2, type: !2820, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2820 = !DISubroutineType(types: !2821)
!2821 = !{!2785}
!2822 = !DILocation(line: 2, column: 1, scope: !2819)
!2823 = distinct !DISubprogram(name: "_CNW7builtinE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", linkageName: "_CNW7builtinE8TypeInfoC1EKPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", scope: !0, file: !1913, line: 2, type: !2824, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2824 = !DISubroutineType(types: !2825)
!2825 = !{!2817, !2784, !2789, !2763, !2792, !2794, !2796, !149, !149, !149}
!2826 = !DILocation(line: 2, column: 1, scope: !2823)
!2827 = distinct !DISubprogram(name: "_CNW7builtinE8TypeInfoC2ERS0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", linkageName: "_CNW7builtinE8TypeInfoC2ERS0_KPS0_KPKPvDoFvPvEKPhKPNS_17ValueWitnessTableEKPNS_19ProtocolConformanceEmmmL0L9baseClassL6vtableL6deinitL4nameL17valueWitnessTableL12conformancesL4sizeL9alignmentL6stride", scope: !0, file: !1913, line: 2, type: !2824, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2828 = !DILocation(line: 2, column: 1, scope: !2827)
!2829 = distinct !DISubprogram(name: "_CNW7builtinE8TypeInfoD3Ev", linkageName: "_CNW7builtinE8TypeInfoD3Ev", scope: null, file: !1913, line: 2, type: !2815, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2830 = !DILocation(line: 2, column: 1, scope: !2829)
!2831 = distinct !DISubprogram(name: "_CNW7builtinE5Int64D0Ev", linkageName: "_CNW7builtinE5Int64D0Ev", scope: !0, file: !1913, line: 2, type: !2832, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2832 = !DISubroutineType(types: !2833)
!2833 = !{!2834}
!2834 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !2835, size: 64, align: 8)
!2835 = !DICompositeType(tag: DW_TAG_structure_type, name: "builtin.<private name>.Int64", file: !1913, line: 2, flags: DIFlagFwdDecl, elements: !2836)
!2836 = !{!2837}
!2837 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !2835, file: !1913, line: 2, baseType: !133, size: 64, align: 64)
!2838 = !DILocation(line: 2, column: 1, scope: !2831)
!2839 = distinct !DISubprogram(name: "_CNW7builtinE5Int644copyES0_L0", linkageName: "_CNW7builtinE5Int644copyES0_L0", scope: !0, file: !1913, line: 2, type: !2840, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2840 = !DISubroutineType(types: !2841)
!2841 = !{!2835}
!2842 = !DILocation(line: 2, column: 1, scope: !2839)
!2843 = distinct !DISubprogram(name: "_CNW7builtinE5Int6414__default_initES0_L0", linkageName: "_CNW7builtinE5Int6414__default_initES0_L0", scope: !0, file: !1913, line: 2, type: !2840, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2844 = !DILocation(line: 2, column: 1, scope: !2843)
!2845 = distinct !DISubprogram(name: "_CNW7builtinE5Int64C1ElL5value", linkageName: "_CNW7builtinE5Int64C1ElL5value", scope: !0, file: !1913, line: 2, type: !2846, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2846 = !DISubroutineType(types: !2847)
!2847 = !{!2834, !133}
!2848 = !DILocation(line: 2, column: 1, scope: !2845)
!2849 = distinct !DISubprogram(name: "_CNW7builtinE5Int64C2ERS0_lL0L5value", linkageName: "_CNW7builtinE5Int64C2ERS0_lL0L5value", scope: !0, file: !1913, line: 2, type: !2846, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2850 = !DILocation(line: 2, column: 1, scope: !2849)
!2851 = distinct !DISubprogram(name: "_CNW7builtinE5Int64D3Ev", linkageName: "_CNW7builtinE5Int64D3Ev", scope: null, file: !1913, line: 2, type: !2832, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2852 = !DILocation(line: 2, column: 1, scope: !2851)
!2853 = distinct !DISubprogram(name: "_CNW7builtinE20ExistentialContainerD0Ev", linkageName: "_CNW7builtinE20ExistentialContainerD0Ev", scope: !0, file: !1913, line: 2, type: !2854, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2854 = !DISubroutineType(types: !2855)
!2855 = !{!2856}
!2856 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !2857, size: 64, align: 8)
!2857 = !DICompositeType(tag: DW_TAG_structure_type, name: "builtin.<private name>.ExistentialContainer", file: !1913, line: 2, flags: DIFlagFwdDecl, elements: !2858)
!2858 = !{!2859, !2860, !2861}
!2859 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !2857, file: !1913, line: 2, baseType: !9, size: 64, align: 64)
!2860 = !DIDerivedType(tag: DW_TAG_member, name: "typeInfo", scope: !2857, file: !1913, line: 2, baseType: !2784, size: 64, align: 64, offset: 64)
!2861 = !DIDerivedType(tag: DW_TAG_member, name: "conformance", scope: !2857, file: !1913, line: 2, baseType: !2796, size: 64, align: 64, offset: 128)
!2862 = !DILocation(line: 2, column: 1, scope: !2853)
!2863 = distinct !DISubprogram(name: "_CNW7builtinE20ExistentialContainer14__default_initES0_L0", linkageName: "_CNW7builtinE20ExistentialContainer14__default_initES0_L0", scope: !0, file: !1913, line: 2, type: !2864, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2864 = !DISubroutineType(types: !2865)
!2865 = !{!2857}
!2866 = !DILocation(line: 2, column: 1, scope: !2863)
!2867 = distinct !DISubprogram(name: "_CNW7builtinE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance", linkageName: "_CNW7builtinE20ExistentialContainerC1EPvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL5valueL8typeInfoL11conformance", scope: !0, file: !1913, line: 2, type: !2868, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2868 = !DISubroutineType(types: !2869)
!2869 = !{!2856, !9, !2784, !2796}
!2870 = !DILocation(line: 2, column: 1, scope: !2867)
!2871 = distinct !DISubprogram(name: "_CNW7builtinE20ExistentialContainerC2ERS0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance", linkageName: "_CNW7builtinE20ExistentialContainerC2ERS0_PvKPNS_8TypeInfoEKPNS_19ProtocolConformanceEL0L5valueL8typeInfoL11conformance", scope: !0, file: !1913, line: 2, type: !2868, isLocal: false, isDefinition: true, scopeLine: 2, thisAdjustment: 1, isOptimized: false, unit: !0)
!2872 = !DILocation(line: 2, column: 1, scope: !2871)
!2873 = distinct !DISubprogram(name: "_CNW7builtinE20ExistentialContainerD3Ev", linkageName: "_CNW7builtinE20ExistentialContainerD3Ev", scope: null, file: !1913, line: 2, type: !2854, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!2874 = !DILocation(line: 2, column: 1, scope: !2873)
