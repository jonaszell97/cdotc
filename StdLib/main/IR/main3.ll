; ModuleID = '/Users/Jonas/CDot/cmake-build-debug/test/main3.dot'
source_filename = "/Users/Jonas/CDot/cmake-build-debug/test/main3.dot"

define internal void @_CNW5main3E7goodbyeEv() !dbg !5 {
alloca_block:
  br label %entry, !dbg !8

entry:                                            ; preds = %alloca_block
  ret void, !dbg !8
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "cdotc v0.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "/Users/Jonas/CDot/cmake-build-debug/test/main3.dot", directory: "")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = distinct !DISubprogram(name: "_CNW5main3E7goodbyeEv", linkageName: "_CNW5main3E7goodbyeEv", scope: null, file: !6, line: 2, type: !7, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!6 = !DIFile(filename: "main3.dot", directory: "/Users/Jonas/CDot/cmake-build-debug/test/")
!7 = !DISubroutineType(types: !2)
!8 = !DILocation(line: 2, column: 16, scope: !5)
