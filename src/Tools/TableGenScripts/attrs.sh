tblgen=tblgen

$tblgen ../../Basic/Attributes.tg -attribute-defs /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/include/cdotc/AST/Attributes.def

$tblgen ../../Basic/Attributes.tg -attribute-classes /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/include/cdotc/AST/Attr.inc

$tblgen ../../Basic/Attributes.tg -attribute-parse /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/include/cdotc/Parse/ParseAttr.inc

$tblgen ../../Basic/Attributes.tg -attribute-serialize /Users/Jonas/CDot/cmake-build-debug/libcdotc-tblgens.dylib > /Users/Jonas/CDot/include/cdotc/Serialization/SerializeAttr.inc

