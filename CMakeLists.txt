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

set(SOURCE_FILES src/lex/Token.cpp src/lex/Token.h src/Variant/Variant.cpp src/Variant/Variant.h src/Util.cpp src/Util.h
        src/lex/Lexer.cpp src/lex/Lexer.h src/parse/Parser.cpp src/parse/Parser.h src/AST/AstNode.cpp src/AST/AstNode.h src/AST/Statement.cpp src/AST/Statement.h src/AST/Expression.cpp src/AST/Expression.h src/AST/Passes/AbstractPass.cpp src/AST/Passes/AbstractPass.h
        src/AST/Passes/SemanticAnalysis/SemaPass.cpp src/AST/Passes/SemanticAnalysis/SemaPass.h src/AST/Passes/CodeGen/CodeGen.cpp src/AST/Passes/CodeGen/CodeGen.h src/AST/Passes/CodeGen/CGBinaryOperator.cpp src/AST/Passes/CodeGen/CGBinaryOperator.h src/AST/Passes/CodeGen/CGMemory.cpp src/AST/Passes/CodeGen/CGMemory.h src/Variant/Type/Type.cpp src/Variant/Type/Type.h src/AST/Attribute/Attribute.h src/AST/Attribute/Attribute.cpp src/AST/Passes/SemanticAnalysis/Builtin.h src/AST/Passes/CodeGen/CGCast.cpp src/AST/Passes/CodeGen/CGCast.h src/Compiler.cpp src/Compiler.h src/AST/Passes/SemanticAnalysis/SemaDecl.cpp src/AST/Passes/Output/HeaderGen.cpp src/AST/Passes/Output/HeaderGen.h src/Message/Diagnostics.cpp src/Message/Diagnostics.h src/Message/def/DiagnosticParser.cpp src/Message/def/DiagnosticParser.h src/AST/Passes/CodeGen/CGException.cpp src/AST/Passes/CodeGen/CGException.h src/AST/Passes/CodeGen/CGClass.cpp src/AST/AstDeclarations.h src/parse/ParseRecord.cpp src/AST/Passes/DebugInfo/DebugInfo.cpp src/AST/Passes/DebugInfo/DebugInfo.h src/Files/FileUtils.h src/Files/FileUtils.cpp src/Files/FileManager.cpp src/Files/FileManager.h src/AST/Passes/SemanticAnalysis/SemaClass.cpp src/Support/BitstreamWriter.h src/Support/BitstreamReader.h src/AST/Passes/SemanticAnalysis/SemaMemberExpr.cpp src/AST/Passes/SemanticAnalysis/SemaCall.cpp src/AST/Passes/SemanticAnalysis/SemaOperator.cpp src/AST/Passes/SemanticAnalysis/SemaTemplate.cpp src/AST/Passes/SemanticAnalysis/OverloadResolver.cpp src/AST/Passes/SemanticAnalysis/OverloadResolver.h src/AST/Passes/CodeGen/CGModule.cpp src/lex/SourceLocation.h src/IL/Value/Value.cpp src/IL/Value/Value.h src/IL/Value/Instruction/Instruction.cpp src/IL/Value/Instruction/Instruction.h src/IL/Value/Constant/Constant.cpp src/IL/Value/Constant/Constant.h src/IL/Module/Module.cpp src/IL/Module/Module.h src/IL/Value/Function/BasicBlock.cpp src/IL/Value/Function/BasicBlock.h src/IL/Value/Function/Function.cpp src/IL/Value/Function/Function.h src/IL/Value/Record/AggregateType.cpp src/IL/Value/Record/AggregateType.h src/IL/Value/Function/Method.cpp src/IL/Value/Function/Method.h src/IL/Value/GlobalVariable.cpp src/IL/Value/GlobalVariable.h src/IL/Value/Constant/ConstantVal.cpp src/IL/Value/Constant/ConstantVal.h src/IL/Value/Instruction/Memory/AllocaInst.cpp src/IL/Value/Instruction/Memory/AllocaInst.h src/IL/Value/Instruction/Memory/InitInst.cpp src/IL/Value/Instruction/Memory/InitInst.h src/IL/Value/Instruction/Memory/GEPInst.cpp src/IL/Value/Instruction/Memory/GEPInst.h src/IL/Value/Instruction/Memory/StoreInst.cpp src/IL/Value/Instruction/Memory/StoreInst.h src/IL/Value/Instruction/Operator/OperatorInst.cpp src/IL/Value/Instruction/Operator/OperatorInst.h src/AST/Passes/ILGen/ILGenPass.cpp src/AST/Passes/ILGen/ILGenPass.h src/IL/ILBuilder.cpp src/IL/ILBuilder.h src/IL/Value/Constant/ConstantExpr.cpp src/IL/Value/Constant/ConstantExpr.h src/IL/Value/Instruction/Cast/CastInst.cpp src/IL/Value/Instruction/Cast/CastInst.h src/IL/Value/Instruction/CallInst.cpp src/IL/Value/Instruction/CallInst.h src/IL/Module/Context.cpp src/IL/Module/Context.h src/IL/Writer/ModuleWriter.cpp src/IL/Writer/ModuleWriter.h src/IL/Value/Function/Argument.cpp src/IL/Value/Function/Argument.h src/IL/Value/Instruction/Terminator/TerminatorInst.cpp src/IL/Value/Instruction/Terminator/TerminatorInst.h src/IL/Value/Instruction/ControlFlow/ControlFlowInst.cpp src/IL/Value/Instruction/ControlFlow/ControlFlowInst.h src/AST/Passes/ILGen/ILGenCast.cpp src/Support/Format.cpp src/Support/Format.h src/IL/Value/MetaData/MetaData.cpp src/IL/Value/MetaData/MetaData.h src/Support/Casting.h src/AST/Passes/ILGen/ILGenClass.cpp src/IL/Value/Use.cpp src/IL/Value/Use.h src/IL/Value/Instruction/MultiOperandInst.h src/IL/Passes/PassManager.cpp src/IL/Passes/PassManager.h src/IL/Passes/VerifierPass.cpp src/IL/Passes/VerifierPass.h src/IL/Passes/ReturnVerifierPass.cpp src/IL/Passes/ReturnVerifierPass.h src/IL/Passes/InstructionVisitor.h src/IL/Passes/PrimitivePromoter.cpp src/IL/Passes/PrimitivePromoter.h src/IL/Value/ValueSymbolTable.cpp src/IL/Value/ValueSymbolTable.h src/IL/Value/SymbolTableList.cpp src/IL/Value/SymbolTableList.h src/AST/Passes/SemanticAnalysis/ExpressionResolver.cpp src/AST/Passes/SemanticAnalysis/ExpressionResolver.h src/Basic/CastKind.h src/Basic/CastKind.cpp src/module/ModuleManager.cpp src/module/ModuleManager.h src/module/Module.cpp src/module/Module.cpp src/module/Module.h src/module/ModuleParser.cpp src/module/ModuleParser.h src/module/ModuleWriter.cpp src/module/ModuleWriter.h src/Support/WriterBase.h src/lex/Persist.cpp src/lex/Persist.h src/parse/DeclParser.cpp src/parse/DeclParser.h src/parse/DelayedParseLoc.h src/AST/Passes/SemanticAnalysis/TemplateInstantiator.cpp src/AST/Passes/SemanticAnalysis/TemplateInstantiator.h src/IL/Passes/IRGen/IRGen.cpp src/IL/Passes/IRGen/IRGen.h src/IL/Passes/IRGen/IRGenModule.cpp src/Basic/Precedence.h src/IL/Passes/IRGen/DIGen.cpp src/Basic/DependencyGraph.cpp src/Basic/DependencyGraph.h src/AST/Passes/SemanticAnalysis/ConformanceChecker.cpp src/AST/Passes/SemanticAnalysis/ConformanceChecker.h src/AST/Passes/AggregateVisitor.cpp src/AST/Passes/AggregateVisitor.h src/CTFE/CTFEEngine.cpp src/CTFE/CTFEEngine.h src/CTFE/Value.h src/CTFE/Value.cpp src/Support/Various.h src/AST/Passes/SemanticAnalysis/Template.cpp src/AST/Passes/SemanticAnalysis/Template.h src/AST/Passes/PrettyPrint/PrettyPrinter.cpp src/AST/Passes/PrettyPrint/PrettyPrinter.h src/Basic/Mangle.cpp src/Basic/Mangle.h src/AST/Passes/Serialization/Serialize.cpp src/AST/Passes/Serialization/Serialize.h src/module/Serialization.cpp src/module/Serialization.h src/Support/SerializerBase.h src/IL/Serialize/Serialization.cpp src/IL/Serialize/Serialization.h src/module/ModuleMap.cpp src/module/ModuleMap.h src/Support/LLVM.h src/AST/ASTContext.cpp src/AST/ASTContext.h src/AST/Transform.cpp src/AST/Transform.h src/AST/Traverse.cpp src/AST/Traverse.h src/AST/NamedDecl.h src/Support/ConditionalIterator.h src/module/IdentifierTable.cpp src/module/IdentifierTable.h src/Support/ExtendedSerializerBase.h src/AST/NamedDecl.cpp src/AST/Passes/SemanticAnalysis/Scope/Scope.cpp src/AST/Passes/SemanticAnalysis/Scope/Scope.h src/Basic/IdentifierInfo.cpp src/Basic/IdentifierInfo.h src/lex/TokenKinds.h src/lex/Preprocessor.h src/lex/Preprocessor.cpp src/Basic/Precedence.cpp src/TableGen/Parser.cpp src/TableGen/Parser.h src/TableGen/Record.cpp src/TableGen/Record.h src/TableGen/Type.cpp src/TableGen/Type.h src/TableGen/TableGen.cpp src/TableGen/TableGen.h src/TableGen/Value.cpp src/TableGen/Value.h src/TableGen/Backend/TableGenBackends.h src/TableGen/Backend/PrintRecords.cpp src/TableGen/Backend/EmitClassHierarchy.cpp src/AST/ParentMap.cpp src/AST/ParentMap.h src/AST/Passes/ASTVisitor.h src/CTFE/StaticEvaluator.cpp src/CTFE/StaticEvaluator.h src/AST/Passes/ASTDumper/ASTDumper.cpp src/AST/Passes/ASTDumper/ASTDumper.h src/Support/LiteralParser.cpp src/Support/LiteralParser.h src/AST/Passes/SemanticAnalysis/CandidateSet.h src/AST/Passes/SemanticAnalysis/CandidateSet.cpp src/AST/Passes/SemanticAnalysis/BuiltinCandidateBuilder.cpp src/AST/Passes/SemanticAnalysis/BuiltinCandidateBuilder.h src/AST/Passes/SemanticAnalysis/ActionResult.h src/AST/Passes/SemanticAnalysis/ConversionSequence.cpp src/AST/Passes/SemanticAnalysis/ConversionSequence.h src/AST/Passes/SemanticAnalysis/SemaCast.cpp src/IL/Passes/UseBeforeInit.cpp src/IL/Passes/UseBeforeInit.h src/IL/Utils/BlockIterator.cpp src/IL/Utils/BlockIterator.h src/AST/Passes/SemanticAnalysis/ContinuationPoint.h src/AST/Passes/SemanticAnalysis/ContinuationPoint.cpp src/AST/Passes/SemanticAnalysis/SemaCTFE.cpp src/Message/DiagnosticsEngine.cpp src/Message/DiagnosticsEngine.h src/Basic/TargetInfo.cpp src/Basic/TargetInfo.h src/parse/ParseAttr.cpp src/AST/Attribute/Attr.cpp src/AST/Attribute/Attr.h src/AST/Passes/SemanticAnalysis/SemaAttr.cpp src/AST/SourceType.h)

set(TBLGEN_SOURCE_FILES src/TableGen/main.cpp src/TableGen/Parser.h
        src/TableGen/Parser.cpp src/TableGen/Record.h src/TableGen/Record.cpp
        src/TableGen/TableGen.h src/TableGen/TableGen.cpp src/TableGen/Type.h
        src/TableGen/Type.cpp src/TableGen/Value.h src/TableGen/Value.cpp
        src/TableGen/Backend/TableGenBackends.h
        src/TableGen/Backend/PrintRecords.cpp
        src/TableGen/Backend/EmitClassHierarchy.cpp src/Message/Diagnostics.h
        src/Message/Diagnostics.cpp src/lex/Lexer.h src/lex/Lexer.cpp
        src/lex/TokenKinds.h src/lex/Token.h src/lex/Token.cpp
        src/lex/Preprocessor.h src/lex/Preprocessor.cpp
        src/lex/SourceLocation.h src/Files/FileManager.h
        src/Files/FileManager.cpp src/Files/FileUtils.h src/Files/FileUtils.cpp
        src/Basic/IdentifierInfo.h src/Basic/IdentifierInfo.cpp
        src/Basic/Precedence.h src/Basic/Precedence.cpp src/Support/Casting.h
        src/Support/Format.h src/Support/Format.cpp src/Variant/Variant.h
        src/Variant/Variant.cpp src/Basic/DependencyGraph.h src/Support/LiteralParser.cpp src/Support/LiteralParser.h src/Message/DiagnosticsEngine.cpp src/Message/DiagnosticsEngine.h)

set(CDOTC_TBLGENS_SOURCE_FILES
        src/Tools/TableGenBackends/BuiltinOperatorBackend.cpp src/Tools/TableGenBackends/DiagnosticsBackend.cpp src/Tools/TableGenBackends/BuiltinTypesBackend.cpp src/Tools/TableGenBackends/AttributesBackend.cpp)

add_executable(cdotc src/main.cpp ${SOURCE_FILES})
add_executable(cdot-tblgen ${TBLGEN_SOURCE_FILES})

add_library(cdotc-tblgens SHARED ${CDOTC_TBLGENS_SOURCE_FILES})

target_compile_definitions(cdot-tblgen PRIVATE CDOT_SMALL_VARIANT=1)

target_link_libraries(cdotc ${llvm_libs})
target_link_libraries(cdot-tblgen ${llvm_libs})
target_link_libraries(cdotc-tblgens ${llvm_libs})