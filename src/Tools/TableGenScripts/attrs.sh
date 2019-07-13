tblgen=tblgen

$tblgen ../../Basic/Attributes.tg -attribute-defs /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/src/AST/Attributes.def

$tblgen ../../Basic/Attributes.tg -attribute-classes /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/src/AST/Attr.inc

$tblgen ../../Basic/Attributes.tg -attribute-parse /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/src/Parse/ParseAttr.inc

$tblgen ../../Basic/Attributes.tg -attribute-serialize /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/src/Serialization/SerializeAttr.inc

