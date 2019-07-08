; ModuleID = '/Users/Jonas/CDot/cmake-build-debug/test/inc/A.dot'
source_filename = "/Users/Jonas/CDot/cmake-build-debug/test/inc/A.dot"

%cdot.TypeInfo = type { i8*, [0 x i8*]*, [0 x i8*]*, i8*, i8*, [0 x i8*]* }
%"struct.inc.e.Template<i64>" = type <{ i8* }>
%struct.inc.a.Unprintable = type <{ i8* }>
%class.std.prelude.String = type <{ i64, i64, i8*, i8*, i64, i64 }>

@_CTINW3std7preludeE3AnyE = external constant %cdot.TypeInfo, align 8
@_CTINW3inc1dE18HelloSayerProtocolE = constant %cdot.TypeInfo { i8* null, [0 x i8*]* null, [0 x i8*]* null, i8* getelementptr inbounds ([25 x i8], [25 x i8]* @0, i32 0, i32 0), i8* null, [0 x i8*]* bitcast ([2 x i8*]* @1 to [0 x i8*]*) }, align 8
@_CTINW3inc1aE11UnprintableE = constant %cdot.TypeInfo { i8* null, [0 x i8*]* null, [0 x i8*]* null, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @2, i32 0, i32 0), i8* bitcast (void (%struct.inc.a.Unprintable*)* @_CNW3inc1aE11UnprintableD0Ev to i8*), [0 x i8*]* bitcast ([3 x i8*]* @3 to [0 x i8*]*) }, align 8
@_CTINW3inc1eE8TemplateIlEE = constant %cdot.TypeInfo { i8* null, [0 x i8*]* null, [0 x i8*]* null, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @4, i32 0, i32 0), i8* bitcast (void (%"struct.inc.e.Template<i64>"*)* @_CNW3inc1eE8TemplateIlED0Ev to i8*), [0 x i8*]* bitcast ([2 x i8*]* @5 to [0 x i8*]*) }, align 8
@_CNW3inc1bE1xE = global %"struct.inc.e.Template<i64>" zeroinitializer, align 8
@_CIFNW3inc1bE1xE = global i1 false, align 1
@0 = private unnamed_addr constant [25 x i8] c"inc.d.HelloSayerProtocol\00"
@1 = private constant [2 x i8*] [i8* bitcast (%cdot.TypeInfo* @_CTINW3std7preludeE3AnyE to i8*), i8* null]
@2 = private unnamed_addr constant [18 x i8] c"inc.a.Unprintable\00"
@3 = private constant [3 x i8*] [i8* bitcast (%cdot.TypeInfo* @_CTINW3std7preludeE3AnyE to i8*), i8* bitcast (%cdot.TypeInfo* @_CTINW3inc1dE18HelloSayerProtocolE to i8*), i8* null]
@4 = private unnamed_addr constant [20 x i8] c"inc.e.Template<i64>\00"
@5 = private constant [2 x i8*] [i8* bitcast (%cdot.TypeInfo* @_CTINW3std7preludeE3AnyE to i8*), i8* null]
@6 = private unnamed_addr constant [15 x i8] c"hello, friend!\00"
@7 = private unnamed_addr constant [18 x i8] c"404 I am a teapot\00"

define void @_CNW3inc1aE1CEv() {
alloca_block:
  br label %"0"

"0":                                              ; preds = %alloca_block
  ret void
}

define void @_CNW3inc1aE1AEv(%struct.inc.a.Unprintable* sret) {
alloca_block:
  %1 = alloca %struct.inc.a.Unprintable, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW3inc1bE1BEv()
  call void @_CNW3inc1aE11UnprintableC1Ev(%struct.inc.a.Unprintable* %1)
  %2 = bitcast %struct.inc.a.Unprintable* %0 to i8*
  %3 = bitcast %struct.inc.a.Unprintable* %1 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* %3, i64 8, i32 1, i1 false)
  ret void
}

define void @_CNW3inc1aE2C_Ev() {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %0 = load i1, i1* @_CIFNW3inc1bE1xE, align 1
  br i1 %0, label %glob.init.merge, label %glob.init

glob.init.merge:                                  ; preds = %glob.init, %entry
  call void @_CNW3inc1eE8TemplateIlE6methodES0_L0(%"struct.inc.e.Template<i64>"* @_CNW3inc1bE1xE)
  ret void

glob.init:                                        ; preds = %entry
  call void @_CNW3inc1bE1xE.init()
  store i1 true, i1* @_CIFNW3inc1bE1xE, align 1
  br label %glob.init.merge
}

define void @_CNW3inc1bE1BEv() {
alloca_block:
  %0 = alloca %struct.inc.a.Unprintable, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW3inc1aE1AEv(%struct.inc.a.Unprintable* sret %0)
  ret void
}

define void @_CNW3inc1dE1DEv() {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void
}

define void @_CNW3inc1dE7__helloEv() {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void
}

define void @_CNW3inc1eE1EEv() {
alloca_block:
  %0 = alloca %struct.inc.a.Unprintable, align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW3inc1aE11UnprintableC1Ev(%struct.inc.a.Unprintable* %0)
  call void @_CNW3inc1aE11Unprintable8sayHelloES_L0(%struct.inc.a.Unprintable* %0)
  ret void
}

define void @_CNW3inc1aE11Unprintable14__default_initES_L0(%struct.inc.a.Unprintable*) {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void
}

define void @_CNW3inc1aE11UnprintableC1Ev(%struct.inc.a.Unprintable*) {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW3inc1aE11Unprintable14__default_initES_L0(%struct.inc.a.Unprintable* %0)
  ret void
}

define void @_CNW3inc1aE11UnprintableC2ES_L0(%struct.inc.a.Unprintable*) {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void
}

define void @_CNW3inc1aE11UnprintableD0Ev(%struct.inc.a.Unprintable*) {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW3inc1aE11UnprintableD3Ev(%struct.inc.a.Unprintable* %0)
  ret void
}

define void @_CNW3inc1aE11Unprintable8sayHelloES_L0(%struct.inc.a.Unprintable*) {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = call i8* @_cdot_Malloc(i64 48)
  %2 = bitcast i8* %1 to %class.std.prelude.String*
  call void @_CNW3std7preludeE6StringC1EKPhmL0L0(%class.std.prelude.String* %2, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @6, i32 0, i32 0), i64 14)
  call void @_CNW3std7preludeE5printINW_1E6StringEJEEETvNW_1E6StringEhhL0L9separatorL10terminator(%class.std.prelude.String* %2, i8 32, i8 10)
  %3 = bitcast %class.std.prelude.String* %2 to i8*
  call void @_cdot_AtomicRelease(i8* %3)
  ret void
}

define void @_CNW3inc1eE8TemplateIlE6methodES0_L0(%"struct.inc.e.Template<i64>"*) {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  %1 = call i8* @_cdot_Malloc(i64 48)
  %2 = bitcast i8* %1 to %class.std.prelude.String*
  call void @_CNW3std7preludeE6StringC1EKPhmL0L0(%class.std.prelude.String* %2, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @7, i32 0, i32 0), i64 17)
  call void @_CNW3std7preludeE5printINW_1E6StringEJEEETvNW_1E6StringEhhL0L9separatorL10terminator(%class.std.prelude.String* %2, i8 32, i8 10)
  %3 = bitcast %class.std.prelude.String* %2 to i8*
  call void @_cdot_AtomicRelease(i8* %3)
  ret void
}

define void @_CNW3inc1eE8TemplateIlE14__default_initES0_L0(%"struct.inc.e.Template<i64>"*) {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void
}

define void @_CNW3inc1eE8TemplateIlEC1Ev(%"struct.inc.e.Template<i64>"*) {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW3inc1eE8TemplateIlE14__default_initES0_L0(%"struct.inc.e.Template<i64>"* %0)
  ret void
}

define void @_CNW3inc1eE8TemplateIlEC2ES0_L0(%"struct.inc.e.Template<i64>"*) {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void
}

define void @_CNW3inc1eE8TemplateIlED0Ev(%"struct.inc.e.Template<i64>"*) {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW3inc1eE8TemplateIlED3Ev(%"struct.inc.e.Template<i64>"* %0)
  ret void
}

define void @_CNW3inc1bE1xE.init() {
alloca_block:
  %0 = alloca %"struct.inc.e.Template<i64>", align 8
  br label %entry

entry:                                            ; preds = %alloca_block
  call void @_CNW3inc1eE8TemplateIlEC1Ev(%"struct.inc.e.Template<i64>"* %0)
  %1 = bitcast %"struct.inc.e.Template<i64>"* @_CNW3inc1bE1xE to i8*
  %2 = bitcast %"struct.inc.e.Template<i64>"* %0 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1, i8* %2, i64 8, i32 1, i1 false)
  ret void
}

define void @_CNW3inc1aE11UnprintableD3Ev(%struct.inc.a.Unprintable*) {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void
}

declare void @_CNW3std7preludeE5printINW_1E6StringEJEEETvNW_1E6StringEhhL0L9separatorL10terminator(%class.std.prelude.String*, i8, i8)

declare void @_CNW3std7preludeE6StringC1EKPhmL0L0(%class.std.prelude.String*, i8*, i64)

define void @_CNW3inc1eE8TemplateIlED3Ev(%"struct.inc.e.Template<i64>"*) {
alloca_block:
  br label %entry

entry:                                            ; preds = %alloca_block
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #0

declare i8* @_cdot_Malloc(i64)

declare void @_cdot_AtomicRelease(i8*)

attributes #0 = { argmemonly nounwind }
