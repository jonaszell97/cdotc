; ModuleID = '/Users/Jonas/CDot/cmake-build-debug/test/main.dot'
source_filename = "/Users/Jonas/CDot/cmake-build-debug/test/main.dot"

define i64 @main() !dbg !5 {
alloca_block:
  br label %entry, !dbg !8

entry:                                            ; preds = %alloca_block
  ret i64 0, !dbg !8
}

define internal void @_CNW5hello2hiE9otherFileEv() !dbg !9 {
alloca_block:
  br label %entry, !dbg !11

entry:                                            ; preds = %alloca_block
  ret void, !dbg !11
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "cdotc v0.1", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "/Users/Jonas/CDot/cmake-build-debug/test/main.dot", directory: "")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = distinct !DISubprogram(name: "main", linkageName: "main", scope: null, file: !6, line: 4, type: !7, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!6 = !DIFile(filename: "main.dot", directory: "/Users/Jonas/CDot/cmake-build-debug/test/")
!7 = !DISubroutineType(types: !2)
!8 = !DILocation(line: 4, column: 13, scope: !5)
!9 = distinct !DISubprogram(name: "_CNW5hello2hiE9otherFileEv", linkageName: "_CNW5hello2hiE9otherFileEv", scope: null, file: !10, line: 4, type: !7, isLocal: false, isDefinition: true, isOptimized: false, unit: !0, variables: !2)
!10 = !DIFile(filename: "main3.dot", directory: "/Users/Jonas/CDot/cmake-build-debug/test/")
!11 = !DILocation(line: 4, column: 18, scope: !9)
