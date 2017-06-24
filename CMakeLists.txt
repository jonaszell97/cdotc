cmake_minimum_required(VERSION 3.7)
project(MathParser)

set(CMAKE_CXX_STANDARD 14)

set(LDFLAGS -L/usr/local/Cellar/libpng/1.6.29/lib)

# add extra include directories
include_directories(/usr/local/include /usr/local/Cellar/imagemagick/7.0.6-0/include/ImageMagick-7)

# add extra lib directories
link_directories(/usr/local/lib /usr/local/Cellar/imagemagick/7.0.6-0/lib)

set(SOURCE_FILES main.cpp Token.cpp Token.h Variant/Variant.cpp Variant/Variant.h Util.cpp Util.h Exceptions.cpp Exceptions.h Debug.cpp Debug.h Tokenizer.cpp Tokenizer.h Interpreter.cpp Interpreter.h Objects/Object.cpp Objects/Object.h Objects/Function.cpp Objects/Function.h Objects/Array.cpp Objects/Array.h AST/AstNode.cpp AST/AstNode.h AST/Statement/Statement.cpp AST/Statement/Statement.h AST/Expression/Expression.cpp AST/Expression/Expression.h AST/Statement/ControlFlow/IfStmt.cpp AST/Statement/ControlFlow/IfStmt.h AST/Statement/ControlFlow/WhileStmt.cpp AST/Statement/ControlFlow/WhileStmt.h AST/Statement/CompoundStmt.cpp AST/Statement/CompoundStmt.h AST/Statement/DeclStmt.cpp AST/Statement/DeclStmt.h AST/Expression/Literal/LiteralExpr.cpp AST/Expression/Literal/LiteralExpr.h AST/Statement/Function/ReturnStmt.cpp AST/Statement/Function/ReturnStmt.h AST/Operator/BinaryOperator.cpp AST/Operator/BinaryOperator.h AST/Operator/UnaryOperator.cpp AST/Operator/UnaryOperator.h AST/Operator/TertiaryOperator.cpp AST/Operator/TertiaryOperator.h AST/Operator/Operator.cpp AST/Operator/Operator.h AST/Expression/RefExpr/IdentifierRefExpr.cpp AST/Expression/RefExpr/IdentifierRefExpr.h AST/Statement/IO/InputStmt.cpp AST/Statement/IO/InputStmt.h AST/Statement/IO/OutputStmt.cpp AST/Statement/IO/OutputStmt.h AST/Expression/Literal/ObjectLiteral.cpp AST/Expression/Literal/ObjectLiteral.h AST/Expression/Literal/ObjectPropExpr.cpp AST/Expression/Literal/ObjectPropExpr.h AST/Statement/Function/FunctionDecl.cpp AST/Statement/Function/FunctionDecl.h AST/Statement/Function/FuncArgDecl.cpp AST/Statement/Function/FuncArgDecl.h AST/Expression/RefExpr/MemberRefExpr.cpp AST/Expression/RefExpr/MemberRefExpr.h AST/Expression/RefExpr/CallExpr.cpp AST/Expression/RefExpr/CallExpr.h AST/Expression/Literal/ArrayLiteral.cpp AST/Expression/Literal/ArrayLiteral.h AST/Expression/RefExpr/ArrayAccessExpr.cpp AST/Expression/RefExpr/ArrayAccessExpr.h AST/Expression/RefExpr/RefExpr.cpp AST/Expression/RefExpr/RefExpr.h AST/Operator/ExplicitCastExpr.cpp AST/Operator/ExplicitCastExpr.h Variant/Arithmetic.cpp Variant/Arithmetic.h Variant/Conversion.cpp Variant/Conversion.h)
add_executable(MathParser ${SOURCE_FILES})

add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )

target_link_libraries(MathParser libMagick++-7.Q16HDRI.2.dylib)