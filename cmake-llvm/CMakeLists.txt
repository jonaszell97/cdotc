cmake_minimum_required(VERSION 3.7)
project(CDot)

set(CMAKE_CXX_STANDARD 14)
set(D_GLIBCXX_DEBUG)
SET(CMAKE_CXX_OUTPUT_EXTENSION ".ll")
SET(CLANG_COMPILE_FLAGS "-S -emit-llvm")

add_definitions(${CLANG_COMPILE_FLAGS})

set(LLVM_DIR /usr/local/Cellar/llvm/4.0.0_1/lib/cmake/llvm)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs all)

set(SOURCE_FILES ../src/Token.cpp ../src/Token.h ../src/Variant/Variant.cpp ../src/Variant/Variant.h ../src/Util.cpp ../src/Util.h
        ../src/Message/Exceptions.cpp ../src/Message/Exceptions.h ../src/Lexer.cpp ../src/Lexer.h ../src/Parser.cpp ../src/Parser.h ../src/AST/AstNode.cpp ../src/AST/AstNode.h ../src/AST/Statement/Statement.cpp ../src/AST/Statement/Statement.h ../src/AST/Expression/Expression.cpp ../src/AST/Expression/Expression.h ../src/AST/Statement/ControlFlow/IfStmt.cpp ../src/AST/Statement/ControlFlow/IfStmt.h ../src/AST/Statement/ControlFlow/WhileStmt.cpp ../src/AST/Statement/ControlFlow/WhileStmt.h ../src/AST/Statement/Block/CompoundStmt.cpp ../src/AST/Statement/Block/CompoundStmt.h ../src/AST/Statement/Declaration/DeclStmt.cpp ../src/AST/Statement/Declaration/DeclStmt.h ../src/AST/Expression/Literal/NumericLiteral.cpp ../src/AST/Expression/Literal/NumericLiteral.h ../src/AST/Statement/ControlFlow/ReturnStmt.cpp ../src/AST/Statement/ControlFlow/ReturnStmt.h ../src/AST/Operator/BinaryOperator.cpp ../src/AST/Operator/BinaryOperator.h ../src/AST/Operator/UnaryOperator.cpp ../src/AST/Operator/UnaryOperator.h ../src/AST/Operator/TertiaryOperator.cpp ../src/AST/Operator/TertiaryOperator.h ../src/AST/Expression/RefExpr/IdentifierRefExpr.cpp ../src/AST/Expression/RefExpr/IdentifierRefExpr.h ../src/AST/Statement/Declaration/FunctionDecl.cpp ../src/AST/Statement/Declaration/FunctionDecl.h ../src/AST/Statement/Declaration/FuncArgDecl.cpp ../src/AST/Statement/Declaration/FuncArgDecl.h ../src/AST/Expression/RefExpr/MemberRefExpr.cpp ../src/AST/Expression/RefExpr/MemberRefExpr.h ../src/AST/Expression/RefExpr/CallExpr.cpp ../src/AST/Expression/RefExpr/CallExpr.h ../src/AST/Expression/Literal/CollectionLiteral.cpp ../src/AST/Expression/Literal/CollectionLiteral.h ../src/AST/Expression/RefExpr/SubscriptExpr.cpp ../src/AST/Expression/RefExpr/SubscriptExpr.h ../src/AST/Statement/ControlFlow/ForStmt.cpp ../src/AST/Statement/ControlFlow/ForStmt.h ../src/AST/Passes/AbstractPass.cpp ../src/AST/Passes/AbstractPass.h ../src/AST/Statement/ControlFlow/ContinueStmt.cpp ../src/AST/Statement/ControlFlow/ContinueStmt.h ../src/AST/Statement/ControlFlow/BreakStmt.cpp ../src/AST/Statement/ControlFlow/BreakStmt.h ../src/AST/Context.cpp ../src/AST/Context.h ../src/AST/Statement/Declaration/Class/ClassDecl.cpp ../src/AST/Statement/Declaration/Class/ClassDecl.h ../src/AST/Statement/Declaration/Class/ConstrDecl.cpp ../src/AST/Statement/Declaration/Class/ConstrDecl.h ../src/AST/Statement/Declaration/Class/FieldDecl.cpp ../src/AST/Statement/Declaration/Class/FieldDecl.h ../src/AST/Statement/Declaration/Class/MethodDecl.cpp ../src/AST/Statement/Declaration/Class/MethodDecl.h ../src/AST/Statement/ControlFlow/MatchStmt.cpp ../src/AST/Statement/ControlFlow/MatchStmt.h ../src/AST/Statement/ControlFlow/CaseStmt.cpp ../src/AST/Statement/ControlFlow/CaseStmt.h ../src/AST/Statement/ControlFlow/LabelStmt.cpp ../src/AST/Statement/ControlFlow/LabelStmt.h ../src/AST/Statement/ControlFlow/GotoStmt.cpp ../src/AST/Statement/ControlFlow/GotoStmt.h ../src/AST/Expression/Literal/StringLiteral.cpp ../src/AST/Expression/Literal/StringLiteral.h ../src/AST/Expression/Literal/LambdaExpr.cpp ../src/AST/Expression/Literal/LambdaExpr.h
        ../src/AST/Statement/Declaration/NamespaceDecl.cpp
        ../src/AST/Statement/Declaration/NamespaceDecl.h ../src/AST/Statement/EndOfFileStmt.h
        ../src/AST/Statement/UsingStmt.cpp ../src/AST/Statement/UsingStmt.h ../src/AST/Statement/Declaration/TypedefDecl.cpp ../src/AST/Statement/Declaration/TypedefDecl.h
        ../src/AST/Passes/StaticAnalysis/TypeCheckPass.cpp ../src/AST/Passes/StaticAnalysis/TypeCheckPass.h ../src/Message/Warning.cpp ../src/Message/Warning.h ../src/AST/Passes/StaticAnalysis/ConstExprPass.cpp ../src/AST/Passes/StaticAnalysis/ConstExprPass.h ../src/AST/SymbolTable.cpp ../src/AST/SymbolTable.h ../src/AST/Passes/StaticAnalysis/Record/Class.cpp ../src/AST/Passes/StaticAnalysis/Record/Class.h ../src/AST/Passes/CodeGen/CodeGen.cpp ../src/AST/Passes/CodeGen/CodeGen.h ../src/AST/Passes/CodeGen/CGBinaryOperator.cpp ../src/AST/Passes/CodeGen/CGBinaryOperator.h ../src/AST/Operator/Conversion/ImplicitCastExpr.cpp ../src/AST/Operator/Conversion/ImplicitCastExpr.h ../src/AST/Passes/CodeGen/CGMemory.cpp ../src/AST/Passes/CodeGen/CGMemory.h ../src/AST/Expression/TypeRef.cpp ../src/AST/Expression/TypeRef.h)

add_executable(CDot ../src/main.cpp ${SOURCE_FILES})

target_link_libraries(CDot ${llvm_libs})