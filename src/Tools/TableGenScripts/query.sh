tblgen=tblgen

$tblgen ../../Query/Queries.tg -query-defs /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/include/cdotc/Query/Inc/Queries.def

$tblgen ../../Query/Queries.tg -query-decls /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/include/cdotc/Query/Inc/QueryDecls.inc

$tblgen ../../Query/Queries.tg -query-impls /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/include/cdotc/Query/Inc/QueryImpls.inc

$tblgen ../../Query/Queries.tg -query-context-fields /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/include/cdotc/Query/Inc/QueryContextFields.inc

$tblgen ../../Query/Queries.tg -query-context-decls /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/include/cdotc/Query/Inc/QueryContextDecls.inc

$tblgen ../../Query/Queries.tg -query-context-specializations /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/include/cdotc/Query/Inc/QueryContextSpecializations.inc

$tblgen ../../Query/Queries.tg -query-context-impls /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/include/cdotc/Query/Inc/QueryContextImpls.inc
