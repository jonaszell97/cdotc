cmake_minimum_required(VERSION 3.7)
project(CDot)

set(CMAKE_CXX_STANDARD 17)

# find LLVM
set(LLVM_DIR /usr/local/Cellar/llvm/6.0.0/lib/cmake/llvm)
find_package(LLVM REQUIRED CONFIG)

# llvm libraries
llvm_map_components_to_libnames(llvm_libs all)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Found LLVM libs ${llvm_libs}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# find clang
set(CLANG_DIR /usr/local/Cellar/llvm/6.0.0/lib/cmake/llvm/tools/clang)
find_package(Clang REQUIRED)

MACRO(FIND_AND_ADD_CLANG_LIB _libname_)
    find_library(CLANG_${_libname_}_LIB ${_libname_} ${LLVM_LIBRARY_DIRS} ${CLANG_LIBRARY_DIRS})
    if (CLANG_${_libname_}_LIB)
        set(clang_libs ${clang_libs} ${CLANG_${_libname_}_LIB})
    endif (CLANG_${_libname_}_LIB)
ENDMACRO(FIND_AND_ADD_CLANG_LIB)

FIND_AND_ADD_CLANG_LIB(clangAnalysis)
FIND_AND_ADD_CLANG_LIB(clangAST)
FIND_AND_ADD_CLANG_LIB(clangASTMatchers)
FIND_AND_ADD_CLANG_LIB(clangBasic)
FIND_AND_ADD_CLANG_LIB(clangChangeNamespace)
FIND_AND_ADD_CLANG_LIB(clangCodeGen)
FIND_AND_ADD_CLANG_LIB(clangCrossTU)
FIND_AND_ADD_CLANG_LIB(clangDaemon)
FIND_AND_ADD_CLANG_LIB(clangDriver)
FIND_AND_ADD_CLANG_LIB(clangDynamicASTMatchers)
FIND_AND_ADD_CLANG_LIB(clangEdit)
FIND_AND_ADD_CLANG_LIB(clangFormat)
FIND_AND_ADD_CLANG_LIB(clangFrontend)
FIND_AND_ADD_CLANG_LIB(clangFrontendTool)
FIND_AND_ADD_CLANG_LIB(clangHandleCXX)
FIND_AND_ADD_CLANG_LIB(clangIndex)
FIND_AND_ADD_CLANG_LIB(clangLex)
FIND_AND_ADD_CLANG_LIB(clangMove)
FIND_AND_ADD_CLANG_LIB(clangParse)
FIND_AND_ADD_CLANG_LIB(clangQuery)
FIND_AND_ADD_CLANG_LIB(clangReorderFields)
FIND_AND_ADD_CLANG_LIB(clangSema)
FIND_AND_ADD_CLANG_LIB(clangSerialization)

if(clang_libs)
    message(STATUS "Found Clang libs: ${clang_libs}")
    message(STATUS "Found Clang: ${CLANG_INCLUDE_DIRS}")
endif()

# set CXX flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

# add include directories
include_directories(src)

set(SOURCE_FILES src/Lex/Token.cpp src/Lex/Token.h src/Basic/Variant.cpp src/Basic/Variant.h
        src/Lex/Lexer.cpp src/Lex/Lexer.h src/Parse/Parser.cpp src/Parse/Parser.h src/AST/AstNode.cpp src/AST/AstNode.h src/AST/Statement.cpp src/AST/Statement.h src/AST/Expression.cpp src/AST/Expression.h
        src/Sema/SemaPass.cpp src/Sema/SemaPass.h src/AST/Type.cpp src/AST/ASTVector.h src/ClangImporter/ImportExpr.cpp
        src/AST/Type.h src/AST/Attribute.h src/AST/Attribute.cpp src/Sema/Builtin.h src/Driver/Compiler.cpp src/Driver/Compiler.h src/Sema/SemaDecl.cpp src/Message/Diagnostics.cpp src/Message/Diagnostics.h src/Message/def/DiagnosticParser.cpp src/Message/def/DiagnosticParser.h src/AST/AstDeclarations.h src/Parse/ParseRecord.cpp src/ILGen/DebugInfo.cpp src/ILGen/DebugInfo.h src/Basic/FileUtils.h src/Basic/FileUtils.cpp src/Basic/FileManager.cpp src/Basic/FileManager.h src/Sema/SemaClass.cpp src/Sema/SemaMemberExpr.cpp src/Sema/SemaCall.cpp src/Sema/SemaOperator.cpp src/Sema/SemaTemplate.cpp src/Sema/OverloadResolver.cpp src/Sema/OverloadResolver.h src/Lex/SourceLocation.h src/IL/Value.cpp src/IL/Value.h src/IL/Instruction.cpp src/IL/Instruction.h src/IL/Constant.cpp src/IL/Constant.h src/IL/Module.cpp src/IL/Module.h src/IL/BasicBlock.cpp src/IL/BasicBlock.h src/IL/Function.cpp src/IL/Function.h src/IL/GlobalVariable.cpp src/IL/GlobalVariable.h src/IL/Constants.cpp src/IL/Constants.h src/ILGen/ILGenPass.cpp src/ILGen/ILGenPass.h src/IL/ILBuilder.cpp src/IL/ILBuilder.h src/IL/Context.cpp src/IL/Context.h src/IL/Writer/ModuleWriter.cpp src/IL/Writer/ModuleWriter.h src/IL/Argument.cpp src/IL/Argument.h src/ILGen/ILGenCast.cpp src/Support/Format.cpp src/Support/Format.h src/IL/MetaData.cpp src/IL/MetaData.h src/Support/Casting.h src/ILGen/ILGenClass.cpp src/IL/Use.cpp src/IL/Use.h src/IL/Passes/PassManager.cpp src/IL/Passes/PassManager.h src/IL/Passes/VerifierPass.cpp src/IL/Passes/VerifierPass.h src/IL/Passes/InstructionVisitor.h src/IL/ValueSymbolTable.cpp src/IL/ValueSymbolTable.h src/IL/SymbolTableList.cpp src/IL/SymbolTableList.h src/Sema/ExpressionResolver.cpp src/Sema/ExpressionResolver.h src/Basic/CastKind.h src/Basic/CastKind.cpp src/Module/ModuleManager.cpp src/Module/ModuleManager.h src/Module/Module.cpp src/Module/Module.cpp src/Module/Module.h src/Support/WriterBase.h src/Lex/Persist.cpp src/Lex/Persist.h src/Sema/TemplateInstantiator.cpp src/Sema/TemplateInstantiator.h src/IRGen/IRGen.cpp src/IRGen/IRGen.h src/IRGen/IRGenModule.cpp src/Basic/Precedence.h src/IRGen/DIGen.cpp src/Basic/DependencyGraph.cpp src/Basic/DependencyGraph.h src/Sema/ConformanceChecker.cpp src/Sema/ConformanceChecker.h src/CTFE/CTFEEngine.cpp src/CTFE/CTFEEngine.h src/CTFE/Value.h src/CTFE/Value.cpp src/Support/Various.h src/Sema/Template.cpp src/Sema/Template.h src/AST/PrettyPrinter.cpp src/AST/PrettyPrinter.h src/Basic/Mangle.cpp src/Basic/Mangle.h src/Support/LLVM.h src/AST/ASTContext.cpp src/AST/ASTContext.h src/AST/Decl.h src/Support/ConditionalIterator.h src/AST/Decl.cpp src/Sema/Scope/Scope.cpp src/Sema/Scope/Scope.h src/Basic/IdentifierInfo.cpp src/Basic/IdentifierInfo.h src/Lex/TokenKinds.h src/Lex/Preprocessor.h src/Lex/Preprocessor.cpp src/Basic/Precedence.cpp src/TableGen/Parser.cpp src/TableGen/Parser.h src/TableGen/Record.cpp src/TableGen/Record.h src/TableGen/Type.cpp src/TableGen/Type.h src/TableGen/TableGen.cpp src/TableGen/TableGen.h src/TableGen/Value.cpp src/TableGen/Value.h src/TableGen/Backend/TableGenBackends.h src/TableGen/Backend/PrintRecords.cpp src/TableGen/Backend/EmitClassHierarchy.cpp src/AST/ParentMap.cpp src/AST/ParentMap.h src/AST/ASTVisitor.h src/CTFE/StaticEvaluator.cpp src/CTFE/StaticEvaluator.h src/AST/ASTDumper.cpp src/AST/ASTDumper.h src/Support/LiteralParser.cpp src/Support/LiteralParser.h src/Sema/CandidateSet.h src/Sema/CandidateSet.cpp src/Sema/BuiltinCandidateBuilder.cpp src/Sema/BuiltinCandidateBuilder.h src/Sema/ActionResult.h src/Sema/ConversionSequence.cpp src/Sema/ConversionSequence.h src/Sema/SemaCast.cpp src/IL/Passes/DefinitiveInitializationPass.cpp src/IL/Passes/DefinitiveInitializationPass.h src/IL/Utils/BlockIterator.cpp src/IL/Utils/BlockIterator.h src/Sema/SemaCTFE.cpp src/Message/DiagnosticsEngine.cpp src/Message/DiagnosticsEngine.h src/Basic/TargetInfo.cpp src/Basic/TargetInfo.h src/Parse/ParseAttr.cpp src/AST/Attr.cpp src/AST/Attr.h src/Sema/SemaAttr.cpp src/AST/SourceType.h src/Basic/DeclarationName.cpp src/Basic/DeclarationName.h src/AST/TypeVisitor.h src/Support/Optional.h src/Sema/SemaReflect.cpp src/AST/DeclDenseMapInfo.h src/IL/Instructions.cpp src/IL/Instructions.h src/AST/TypeBuilder.h src/AST/StmtOrDecl.h src/AST/StmtOrDecl.cpp src/AST/ConformanceTable.cpp src/AST/ConformanceTable.h src/AST/ContinuationPoint.h src/Parse/ParseMacro.cpp src/ILGen/Cleanup.cpp src/ILGen/Cleanup.h src/Basic/DiverseStack.cpp src/Basic/DiverseStack.h src/IL/Passes/DataflowProblem.h src/IL/Passes/BorrowCheckPass.cpp src/IL/Passes/BorrowCheckPass.h src/IL/Analysis/Dominance.cpp src/IL/Analysis/Dominance.h src/IL/CFG.h src/Sema/SemaBuiltin.cpp src/Basic/Builtins.h src/IL/Passes/FinalizeFunctionPass.cpp src/IL/Passes/FinalizeFunctionPass.h src/IL/Analysis/MemoryLocation.cpp src/IL/Analysis/MemoryLocation.h src/IL/Analysis/MemoryUse.cpp src/IL/Analysis/MemoryUse.h src/IL/Analysis/AccessPathDescriptor.cpp src/IL/Analysis/AccessPathDescriptor.h src/IL/Analysis/AccessPathIterator.h src/IL/Analysis/AccessPathIterator.cpp src/IL/Transforms/StackPromotion.cpp src/IL/Transforms/StackPromotion.h src/IL/Analysis/EscapeAnalysis.cpp src/IL/Analysis/EscapeAnalysis.h src/IL/Passes/Passes.cpp src/IL/Passes/Passes.h src/IL/Analysis/Analysis.cpp src/IL/Analysis/Analysis.h src/IL/Analysis/AnalysisKinds.h src/IL/Passes/PassKinds.h src/IL/Analysis/BottomUpIPAnalysis.h src/IL/Analysis/UnsafeAnalysis.cpp src/IL/Analysis/UnsafeAnalysis.h src/Serialization/ASTWriter.cpp src/Serialization/ASTWriter.h src/AST/EmptyASTVisitor.h src/Serialization/ASTWriterStmt.cpp src/Serialization/BitCodes.h src/Serialization/ASTWriterDecl.cpp src/Serialization/ASTCommon.h src/Serialization/ASTReaderInternals.h src/Serialization/ASTReader.cpp src/Serialization/ASTReader.h src/Serialization/ASTReaderStmt.cpp src/Serialization/ASTReaderDecl.cpp src/Serialization/ILWriter.cpp src/Serialization/ILWriter.h src/Serialization/ILReader.cpp src/Serialization/ILReader.h src/Serialization/ModuleWriter.cpp src/Serialization/ModuleWriter.h src/Serialization/ModuleReader.cpp src/Serialization/ModuleReader.h src/Serialization/ModuleFile.cpp src/Serialization/ModuleFile.h src/Sema/Lookup.cpp src/Sema/Lookup.h src/AST/DeclBase.cpp src/AST/DeclBase.h src/Driver/Job.cpp src/Driver/Job.h src/Serialization/IncrementalCompilation.cpp src/Serialization/IncrementalCompilation.h src/IL/Passes/ReorderBasicBlockPass.cpp src/IL/Passes/ReorderBasicBlockPass.h src/ILGen/ILGenCoroutine.cpp src/Support/Timer.cpp src/Support/Timer.h src/ClangImporter/ClangImporter.cpp src/ClangImporter/ClangImporter.h src/ClangImporter/ImportMacro.cpp src/ClangImporter/ImporterImpl.h src/ClangImporter/ImportExpr.cpp src/ClangImporter/ImportType.cpp src/ClangImporter/ImportDecl.cpp src/Support/SaveAndRestore.h src/Basic/NestedNameSpecifier.cpp src/Basic/NestedNameSpecifier.h src/IL/Utils/InstructionCloner.h src/ILGen/ILGenSpecialize.cpp src/Tools/IRDebug/IRDebugAnnotatePass.cpp src/Tools/IRDebug/IRDebugAnnotatePass.h src/AST/TypeComparer.h src/Sema/NameBinding.cpp src/Sema/NameBinding.h src/Query/Query.cpp src/Query/Query.h src/Query/QueryContext.cpp src/Query/QueryContext.h src/Query/TypeQueries.cpp src/Query/BuiltinQueries.cpp src/Query/LookupQueries.cpp src/Query/DriverQueries.cpp src/Query/SemaQueries.cpp src/Query/UtilQueries.cpp src/Query/CTFEQueries.cpp src/Support/Config.h src/Query/ILGenQueries.cpp src/Query/InstantiationQueries.cpp)

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
        src/Basic/Variant.cpp src/Basic/DependencyGraph.h src/Support/LiteralParser.cpp src/Support/LiteralParser.h src/Message/DiagnosticsEngine.cpp src/Message/DiagnosticsEngine.h src/Support/StringSwitch.h)

set(CDOTC_TBLGENS_SOURCE_FILES
        src/Tools/TableGenBackends/BuiltinOperatorBackend.cpp
        src/Tools/TableGenBackends/DiagnosticsBackend.cpp
        src/Tools/TableGenBackends/BuiltinTypesBackend.cpp
        src/Tools/TableGenBackends/AttributesBackend.cpp
        src/Support/StringSwitch.h src/Tools/TableGenBackends/QueryBackend.cpp)

set (RUNTIME_SUPPORT_SOURCE_FILES
        Runtime/RuntimeSupport.cpp)

add_executable(cdotc src/Driver/main.cpp ${SOURCE_FILES})
add_executable(cdotc_asan src/Driver/main.cpp ${SOURCE_FILES})
add_executable(cdotc_ubsan src/Driver/main.cpp ${SOURCE_FILES})
add_executable(cdot-tblgen ${TBLGEN_SOURCE_FILES})

add_library(cdotc-tblgens SHARED ${CDOTC_TBLGENS_SOURCE_FILES})
add_library(cdotrt SHARED ${RUNTIME_SUPPORT_SOURCE_FILES})

set_target_properties(cdotrt
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "/usr/local/lib"
    LIBRARY_OUTPUT_DIRECTORY "/usr/local/lib"
    RUNTIME_OUTPUT_DIRECTORY "/usr/local/lib")

install(TARGETS cdotrt
        DESTINATION "/usr/local/lib")

target_compile_definitions(cdot-tblgen PRIVATE CDOT_SMALL_VARIANT=1)
target_compile_options(cdotc_asan
        PUBLIC "-fsanitize=address"
        PUBLIC "-fno-omit-frame-pointer"
        PUBLIC "-fvisibility=hidden")
target_compile_options(cdotc_ubsan
        PUBLIC "-fsanitize=undefined"
        PUBLIC "-fno-omit-frame-pointer"
        PUBLIC "-fvisibility=hidden")

target_compile_options(cdotc PUBLIC "-fvisibility=hidden")

target_link_libraries(cdotc ${llvm_libs} ${clang_libs})
target_link_libraries(cdot-tblgen ${llvm_libs})
target_link_libraries(cdotc-tblgens ${llvm_libs})

target_link_libraries(cdotc_asan
        ${llvm_libs} ${clang_libs}
        -fsanitize=address
        -fno-omit-frame-pointer)
target_link_libraries(cdotc_ubsan
        ${llvm_libs} ${clang_libs}
        -fsanitize=undefined
        -fno-omit-frame-pointer)