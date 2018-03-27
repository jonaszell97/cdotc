cmake_minimum_required(VERSION 3.7)
project(CDot)

set(CMAKE_CXX_STANDARD 17)

set(LLVM_DIR /usr/local/Cellar/llvm/5.0.1/lib/cmake/llvm)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your computer

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic ")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(src)
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs all)

set(SOURCE_FILES src/Lex/Token.cpp src/Lex/Token.h src/Basic/Variant.cpp src/Basic/Variant.h src/Util.cpp src/Util.h
        src/Lex/Lexer.cpp src/Lex/Lexer.h src/Parse/Parser.cpp src/Parse/Parser.h src/AST/AstNode.cpp src/AST/AstNode.h src/AST/Statement.cpp src/AST/Statement.h src/AST/Expression.cpp src/AST/Expression.h src/AST/AbstractPass.cpp src/AST/AbstractPass.h
        src/Sema/SemaPass.cpp src/Sema/SemaPass.h src/AST/Type.cpp src/AST/Type.h src/AST/Attribute.h src/AST/Attribute.cpp src/Sema/Builtin.h src/Compiler.cpp src/Compiler.h src/Sema/SemaDecl.cpp src/Message/Diagnostics.cpp src/Message/Diagnostics.h src/Message/def/DiagnosticParser.cpp src/Message/def/DiagnosticParser.h src/AST/AstDeclarations.h src/Parse/ParseRecord.cpp src/ILGen/DebugInfo.cpp src/ILGen/DebugInfo.h src/Basic/FileUtils.h src/Basic/FileUtils.cpp src/Basic/FileManager.cpp src/Basic/FileManager.h src/Sema/SemaClass.cpp src/Support/BitstreamWriter.h src/Support/BitstreamReader.h src/Sema/SemaMemberExpr.cpp src/Sema/SemaCall.cpp src/Sema/SemaOperator.cpp src/Sema/SemaTemplate.cpp src/Sema/OverloadResolver.cpp src/Sema/OverloadResolver.h src/Lex/SourceLocation.h src/IL/Value/Value.cpp src/IL/Value/Value.h src/IL/Value/Instruction/Instruction.cpp src/IL/Value/Instruction/Instruction.h src/IL/Value/Constant/Constant.cpp src/IL/Value/Constant/Constant.h src/IL/Module/Module.cpp src/IL/Module/Module.h src/IL/Value/Function/BasicBlock.cpp src/IL/Value/Function/BasicBlock.h src/IL/Value/Function/Function.cpp src/IL/Value/Function/Function.h src/IL/Value/Record/AggregateType.cpp src/IL/Value/Record/AggregateType.h src/IL/Value/Function/Method.cpp src/IL/Value/Function/Method.h src/IL/Value/GlobalVariable.cpp src/IL/Value/GlobalVariable.h src/IL/Value/Constant/ConstantVal.cpp src/IL/Value/Constant/ConstantVal.h src/IL/Value/Instruction/Memory/AllocaInst.cpp src/IL/Value/Instruction/Memory/AllocaInst.h src/IL/Value/Instruction/Memory/InitInst.cpp src/IL/Value/Instruction/Memory/InitInst.h src/IL/Value/Instruction/Memory/GEPInst.cpp src/IL/Value/Instruction/Memory/GEPInst.h src/IL/Value/Instruction/Memory/StoreInst.cpp src/IL/Value/Instruction/Memory/StoreInst.h src/IL/Value/Instruction/Operator/OperatorInst.cpp src/IL/Value/Instruction/Operator/OperatorInst.h src/ILGen/ILGenPass.cpp src/ILGen/ILGenPass.h src/IL/ILBuilder.cpp src/IL/ILBuilder.h src/IL/Value/Constant/ConstantExpr.cpp src/IL/Value/Constant/ConstantExpr.h src/IL/Value/Instruction/Cast/CastInst.cpp src/IL/Value/Instruction/Cast/CastInst.h src/IL/Value/Instruction/CallInst.cpp src/IL/Value/Instruction/CallInst.h src/IL/Module/Context.cpp src/IL/Module/Context.h src/IL/Writer/ModuleWriter.cpp src/IL/Writer/ModuleWriter.h src/IL/Value/Function/Argument.cpp src/IL/Value/Function/Argument.h src/IL/Value/Instruction/Terminator/TerminatorInst.cpp src/IL/Value/Instruction/Terminator/TerminatorInst.h src/IL/Value/Instruction/ControlFlow/ControlFlowInst.cpp src/IL/Value/Instruction/ControlFlow/ControlFlowInst.h src/ILGen/ILGenCast.cpp src/Support/Format.cpp src/Support/Format.h src/IL/Value/MetaData/MetaData.cpp src/IL/Value/MetaData/MetaData.h src/Support/Casting.h src/ILGen/ILGenClass.cpp src/IL/Value/Use.cpp src/IL/Value/Use.h src/IL/Value/Instruction/MultiOperandInst.h src/IL/Passes/PassManager.cpp src/IL/Passes/PassManager.h src/IL/Passes/VerifierPass.cpp src/IL/Passes/VerifierPass.h src/IL/Passes/InstructionVisitor.h src/IL/Value/ValueSymbolTable.cpp src/IL/Value/ValueSymbolTable.h src/IL/Value/SymbolTableList.cpp src/IL/Value/SymbolTableList.h src/Sema/ExpressionResolver.cpp src/Sema/ExpressionResolver.h src/Basic/CastKind.h src/Basic/CastKind.cpp src/Module/ModuleManager.cpp src/Module/ModuleManager.h src/Module/Module.cpp src/Module/Module.cpp src/Module/Module.h src/Module/ModuleParser.cpp src/Module/ModuleParser.h src/Module/ModuleWriter.cpp src/Module/ModuleWriter.h src/Support/WriterBase.h src/Lex/Persist.cpp src/Lex/Persist.h src/Parse/DeclParser.cpp src/Parse/DeclParser.h src/Parse/DelayedParseLoc.h src/Sema/TemplateInstantiator.cpp src/Sema/TemplateInstantiator.h src/IRGen/IRGen.cpp src/IRGen/IRGen.h src/IRGen/IRGenModule.cpp src/Basic/Precedence.h src/IRGen/DIGen.cpp src/Basic/DependencyGraph.cpp src/Basic/DependencyGraph.h src/Sema/ConformanceChecker.cpp src/Sema/ConformanceChecker.h src/CTFE/CTFEEngine.cpp src/CTFE/CTFEEngine.h src/CTFE/Value.h src/CTFE/Value.cpp src/Support/Various.h src/Sema/Template.cpp src/Sema/Template.h src/AST/PrettyPrinter.cpp src/AST/PrettyPrinter.h src/Basic/Mangle.cpp src/Basic/Mangle.h src/Serialization/ASTSerialize.cpp src/Serialization/ASTSerialize.h src/Module/Serialization.cpp src/Module/Serialization.h src/Support/SerializerBase.h src/IL/Serialize/Serialization.cpp src/IL/Serialize/Serialization.h src/Module/ModuleMap.cpp src/Module/ModuleMap.h src/Support/LLVM.h src/AST/ASTContext.cpp src/AST/ASTContext.h src/AST/Transform.cpp src/AST/Transform.h src/AST/Traverse.cpp src/AST/Traverse.h src/AST/Decl.h src/Support/ConditionalIterator.h src/Module/IdentifierTable.cpp src/Module/IdentifierTable.h src/Support/ExtendedSerializerBase.h src/AST/Decl.cpp src/Sema/Scope/Scope.cpp src/Sema/Scope/Scope.h src/Basic/IdentifierInfo.cpp src/Basic/IdentifierInfo.h src/Lex/TokenKinds.h src/Lex/Preprocessor.h src/Lex/Preprocessor.cpp src/Basic/Precedence.cpp src/TableGen/Parser.cpp src/TableGen/Parser.h src/TableGen/Record.cpp src/TableGen/Record.h src/TableGen/Type.cpp src/TableGen/Type.h src/TableGen/TableGen.cpp src/TableGen/TableGen.h src/TableGen/Value.cpp src/TableGen/Value.h src/TableGen/Backend/TableGenBackends.h src/TableGen/Backend/PrintRecords.cpp src/TableGen/Backend/EmitClassHierarchy.cpp src/AST/ParentMap.cpp src/AST/ParentMap.h src/AST/ASTVisitor.h src/CTFE/StaticEvaluator.cpp src/CTFE/StaticEvaluator.h src/AST/ASTDumper.cpp src/AST/ASTDumper.h src/Support/LiteralParser.cpp src/Support/LiteralParser.h src/Sema/CandidateSet.h src/Sema/CandidateSet.cpp src/Sema/BuiltinCandidateBuilder.cpp src/Sema/BuiltinCandidateBuilder.h src/Sema/ActionResult.h src/Sema/ConversionSequence.cpp src/Sema/ConversionSequence.h src/Sema/SemaCast.cpp src/IL/Passes/UseBeforeInit.cpp src/IL/Passes/UseBeforeInit.h src/IL/Utils/BlockIterator.cpp src/IL/Utils/BlockIterator.h src/Sema/SemaCTFE.cpp src/Message/DiagnosticsEngine.cpp src/Message/DiagnosticsEngine.h src/Basic/TargetInfo.cpp src/Basic/TargetInfo.h src/Parse/ParseAttr.cpp src/AST/Attr.cpp src/AST/Attr.h src/Sema/SemaAttr.cpp src/AST/SourceType.h src/Basic/DeclarationName.cpp src/Basic/DeclarationName.h)

set(TBLGEN_SOURCE_FILES src/TableGen/main.cpp src/TableGen/Parser.h
        src/TableGen/Parser.cpp src/TableGen/Record.h src/TableGen/Record.cpp
        src/TableGen/TableGen.h src/TableGen/TableGen.cpp src/TableGen/Type.h
        src/TableGen/Type.cpp src/TableGen/Value.h src/TableGen/Value.cpp
        src/TableGen/Backend/TableGenBackends.h
        src/TableGen/Backend/PrintRecords.cpp
        src/TableGen/Backend/EmitClassHierarchy.cpp
        src/Message/Diagnostics.h
        src/Message/Diagnostics.cpp src/Lex/Lexer.h src/Lex/Lexer.cpp
        src/Lex/TokenKinds.h src/Lex/Token.h src/Lex/Token.cpp
        src/Lex/Preprocessor.h src/Lex/Preprocessor.cpp
        src/Lex/SourceLocation.h src/Basic/FileManager.h
        src/Basic/FileManager.cpp src/Basic/FileUtils.h src/Basic/FileUtils.cpp
        src/Basic/IdentifierInfo.h src/Basic/IdentifierInfo.cpp
        src/Basic/Precedence.h src/Basic/Precedence.cpp src/Support/Casting.h
        src/Support/Format.h src/Support/Format.cpp src/Basic/Variant.h
        src/Basic/Variant.cpp src/Basic/DependencyGraph.h src/Support/LiteralParser.cpp src/Support/LiteralParser.h src/Message/DiagnosticsEngine.cpp src/Message/DiagnosticsEngine.h)

set(CDOTC_TBLGENS_SOURCE_FILES
        src/Tools/TableGenBackends/BuiltinOperatorBackend.cpp src/Tools/TableGenBackends/DiagnosticsBackend.cpp src/Tools/TableGenBackends/BuiltinTypesBackend.cpp src/Tools/TableGenBackends/AttributesBackend.cpp)

add_executable(cdotc src/main.cpp ${SOURCE_FILES})
add_executable(cdot-tblgen ${TBLGEN_SOURCE_FILES})

add_library(cdotc-tblgens SHARED ${CDOTC_TBLGENS_SOURCE_FILES})

target_compile_definitions(cdot-tblgen PRIVATE CDOT_SMALL_VARIANT=1)

target_link_libraries(cdotc ${llvm_libs})
target_link_libraries(cdot-tblgen ${llvm_libs})
target_link_libraries(cdotc-tblgens ${llvm_libs})