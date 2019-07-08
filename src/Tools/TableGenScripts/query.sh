tblgen="/Users/Jonas/CDot/cmake-build-debug/cdot-tblgen"

$tblgen ../../Query/Queries.tg -query-defs /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/src/Query/Inc/Queries.def

$tblgen ../../Query/Queries.tg -query-decls /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/src/Query/Inc/QueryDecls.inc

$tblgen ../../Query/Queries.tg -query-impls /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/src/Query/Inc/QueryImpls.inc

$tblgen ../../Query/Queries.tg -query-context-fields /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/src/Query/Inc/QueryContextFields.inc

$tblgen ../../Query/Queries.tg -query-context-decls /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/src/Query/Inc/QueryContextDecls.inc

$tblgen ../../Query/Queries.tg -query-context-specializations /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/src/Query/Inc/QueryContextSpecializations.inc

$tblgen ../../Query/Queries.tg -query-context-impls /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/src/Query/Inc/QueryContextImpls.inc
