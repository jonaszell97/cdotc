cmake_minimum_required(VERSION 3.7)

set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk")
project(CDot)

set(CMAKE_CXX_STANDARD 17)

# include tblgen
set(TBLGEN_DIR /Users/Jonas/CLionProjects/TableGen)
include_directories("${TBLGEN_DIR}/include")

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
include_directories(include)

set(SOURCE_FILES src/Lex/Token.cpp include/cdotc/Lex/Token.h src/Basic/Variant.cpp include/cdotc/Basic/Variant.h
        src/Lex/Lexer.cpp include/cdotc/Lex/Lexer.h src/Parse/Parser.cpp include/cdotc/Parse/Parser.h
        src/AST/AstNode.cpp include/cdotc/AST/AstNode.h src/AST/Statement.cpp include/cdotc/AST/Statement.h
        src/AST/Expression.cpp include/cdotc/AST/Expression.h src/Sema/SemaPass.cpp include/cdotc/Sema/SemaPass.h
        src/AST/Type.cpp include/cdotc/AST/ASTVector.h src/ClangImporter/ImportExpr.cpp
        include/cdotc/AST/Type.h
        include/cdotc/Sema/Builtin.h src/Driver/Compiler.cpp include/cdotc/Driver/Compiler.h src/Sema/SemaDecl.cpp
        src/Diagnostics/Diagnostics.cpp include/cdotc/Diagnostics/Diagnostics.h
        include/cdotc/Diagnostics/def/DiagnosticParser.h include/cdotc/AST/AstDeclarations.h src/Parse/ParseRecord.cpp
        include/cdotc/Basic/FileUtils.h src/Basic/FileUtils.cpp
        src/Basic/FileManager.cpp include/cdotc/Basic/FileManager.h src/Sema/SemaClass.cpp src/Sema/SemaMemberExpr.cpp
        src/Sema/SemaCall.cpp src/Sema/SemaOperator.cpp src/Sema/SemaTemplate.cpp src/Sema/OverloadResolver.cpp
        include/cdotc/Sema/OverloadResolver.h include/cdotc/Lex/SourceLocation.h src/IL/Value.cpp include/cdotc/IL/Value.h
        src/IL/Instruction.cpp include/cdotc/IL/Instruction.h src/IL/Constant.cpp include/cdotc/IL/Constant.h src/IL/Module.cpp
        include/cdotc/IL/Module.h src/IL/BasicBlock.cpp include/cdotc/IL/BasicBlock.h src/IL/Function.cpp include/cdotc/IL/Function.h
        src/IL/GlobalVariable.cpp include/cdotc/IL/GlobalVariable.h src/IL/Constants.cpp include/cdotc/IL/Constants.h
        src/ILGen/ILGenPass.cpp include/cdotc/ILGen/ILGenPass.h src/IL/ILBuilder.cpp include/cdotc/IL/ILBuilder.h
        src/IL/Context.cpp include/cdotc/IL/Context.h src/IL/Writer/ModuleWriter.cpp include/cdotc/IL/Writer/ModuleWriter.h
        src/IL/Argument.cpp include/cdotc/IL/Argument.h src/ILGen/ILGenCast.cpp src/Support/Format.cpp include/cdotc/Support/Format.h
        src/IL/MetaData.cpp include/cdotc/IL/MetaData.h include/cdotc/Support/Casting.h src/ILGen/ILGenClass.cpp src/IL/Use.cpp
        include/cdotc/IL/Use.h src/IL/Passes/PassManager.cpp include/cdotc/IL/Passes/PassManager.h src/IL/Passes/VerifierPass.cpp
        include/cdotc/IL/Passes/VerifierPass.h include/cdotc/IL/Passes/InstructionVisitor.h src/IL/ValueSymbolTable.cpp
        include/cdotc/IL/ValueSymbolTable.h include/cdotc/IL/SymbolTableList.h src/Sema/ExpressionResolver.cpp
        include/cdotc/Sema/ExpressionResolver.h include/cdotc/Basic/CastKind.h src/Basic/CastKind.cpp src/Module/ModuleManager.cpp
        include/cdotc/Module/ModuleManager.h src/Module/Module.cpp src/Module/Module.cpp include/cdotc/Module/Module.h
        include/cdotc/Support/WriterBase.h src/Sema/TemplateInstantiator.cpp
        include/cdotc/Sema/TemplateInstantiator.h src/IRGen/IRGen.cpp include/cdotc/IRGen/IRGen.h src/IRGen/IRGenModule.cpp
        include/cdotc/Basic/Precedence.h src/IRGen/DIGen.cpp include/cdotc/Basic/DependencyGraph.h
        src/Sema/ConformanceChecker.cpp src/CTFE/CTFEEngine.cpp include/cdotc/CTFE/CTFEEngine.h include/cdotc/CTFE/Value.h
        src/CTFE/Value.cpp include/cdotc/Support/Various.h src/Sema/Template.cpp include/cdotc/Sema/Template.h src/AST/PrettyPrinter.cpp
        include/cdotc/AST/PrettyPrinter.h src/Basic/Mangle.cpp include/cdotc/Basic/Mangle.h include/cdotc/Support/LLVM.h src/AST/ASTContext.cpp
        include/cdotc/AST/ASTContext.h include/cdotc/AST/Decl.h include/cdotc/Support/ConditionalIterator.h src/AST/Decl.cpp
        src/Sema/Scope/Scope.cpp include/cdotc/Sema/Scope/Scope.h src/Basic/IdentifierInfo.cpp include/cdotc/Basic/IdentifierInfo.h
        include/cdotc/Lex/TokenKinds.h src/Basic/Precedence.cpp
        include/cdotc/AST/ASTVisitor.h
        src/CTFE/StaticEvaluator.cpp include/cdotc/CTFE/StaticEvaluator.h src/AST/ASTDumper.cpp include/cdotc/AST/ASTDumper.h src/Support/LiteralParser.cpp
        include/cdotc/Support/LiteralParser.h include/cdotc/Sema/CandidateSet.h src/Sema/CandidateSet.cpp src/Sema/BuiltinCandidateBuilder.cpp
        include/cdotc/Sema/BuiltinCandidateBuilder.h include/cdotc/Sema/ActionResult.h src/Sema/ConversionSequence.cpp
        include/cdotc/Sema/ConversionSequence.h src/Sema/SemaCast.cpp src/IL/Passes/DefinitiveInitializationPass.cpp
        include/cdotc/IL/Passes/DefinitiveInitializationPass.h include/cdotc/IL/Utils/BlockIterator.h
        src/Sema/SemaCTFE.cpp src/Diagnostics/DiagnosticsEngine.cpp include/cdotc/Diagnostics/DiagnosticsEngine.h src/Basic/TargetInfo.cpp
        include/cdotc/Basic/TargetInfo.h src/Parse/ParseAttr.cpp src/AST/Attr.cpp include/cdotc/AST/Attr.h src/Sema/SemaAttr.cpp
        include/cdotc/AST/SourceType.h src/Basic/DeclarationName.cpp include/cdotc/Basic/DeclarationName.h include/cdotc/AST/TypeVisitor.h
        include/cdotc/Support/Optional.h src/Sema/SemaReflect.cpp include/cdotc/AST/DeclDenseMapInfo.h src/IL/Instructions.cpp
        include/cdotc/IL/Instructions.h include/cdotc/AST/TypeBuilder.h include/cdotc/AST/StmtOrDecl.h src/AST/StmtOrDecl.cpp
        src/AST/ConformanceTable.cpp include/cdotc/AST/ConformanceTable.h src/Parse/ParseMacro.cpp
        src/ILGen/Cleanup.cpp include/cdotc/ILGen/Cleanup.h src/Basic/DiverseStack.cpp include/cdotc/Basic/DiverseStack.h
        include/cdotc/IL/Passes/DataflowProblem.h src/IL/Passes/BorrowCheckPass.cpp include/cdotc/IL/Passes/BorrowCheckPass.h
        src/IL/Analysis/Dominance.cpp include/cdotc/IL/Analysis/Dominance.h include/cdotc/IL/CFG.h src/Sema/SemaBuiltin.cpp
        include/cdotc/Basic/Builtins.h src/IL/Passes/FinalizeFunctionPass.cpp include/cdotc/IL/Passes/FinalizeFunctionPass.h
        src/IL/Analysis/MemoryLocation.cpp include/cdotc/IL/Analysis/MemoryLocation.h
        include/cdotc/IL/Analysis/MemoryUse.h src/IL/Analysis/AccessPathDescriptor.cpp include/cdotc/IL/Analysis/AccessPathDescriptor.h
        include/cdotc/IL/Analysis/AccessPathIterator.h src/IL/Analysis/AccessPathIterator.cpp src/IL/Transforms/StackPromotion.cpp
        include/cdotc/IL/Transforms/StackPromotion.h src/IL/Analysis/EscapeAnalysis.cpp include/cdotc/IL/Analysis/EscapeAnalysis.h
        src/IL/Passes/Passes.cpp include/cdotc/IL/Passes/Passes.h src/IL/Analysis/Analysis.cpp include/cdotc/IL/Analysis/Analysis.h
        include/cdotc/IL/Analysis/AnalysisKinds.h include/cdotc/IL/Passes/PassKinds.h include/cdotc/IL/Analysis/BottomUpIPAnalysis.h
        src/IL/Analysis/UnsafeAnalysis.cpp include/cdotc/IL/Analysis/UnsafeAnalysis.h src/Serialization/ASTWriter.cpp
        include/cdotc/Serialization/ASTWriter.h include/cdotc/AST/EmptyASTVisitor.h src/Serialization/ASTWriterStmt.cpp
        include/cdotc/Serialization/BitCodes.h src/Serialization/ASTWriterDecl.cpp include/cdotc/Serialization/ASTCommon.h
        include/cdotc/Serialization/ASTReaderInternals.h src/Serialization/ASTReader.cpp include/cdotc/Serialization/ASTReader.h
        src/Serialization/ASTReaderStmt.cpp src/Serialization/ASTReaderDecl.cpp src/Serialization/ILWriter.cpp
        include/cdotc/Serialization/ILWriter.h src/Serialization/ILReader.cpp include/cdotc/Serialization/ILReader.h
        src/Serialization/ModuleWriter.cpp include/cdotc/Serialization/ModuleWriter.h src/Serialization/ModuleReader.cpp
        include/cdotc/Serialization/ModuleReader.h src/Serialization/ModuleFile.cpp include/cdotc/Serialization/ModuleFile.h
        src/Sema/Lookup.cpp include/cdotc/Sema/Lookup.h src/AST/DeclBase.cpp include/cdotc/AST/DeclBase.h src/Driver/Job.cpp
        include/cdotc/Driver/Job.h src/Serialization/IncrementalCompilation.cpp include/cdotc/Serialization/IncrementalCompilation.h
        src/IL/Passes/ReorderBasicBlockPass.cpp include/cdotc/IL/Passes/ReorderBasicBlockPass.h src/ILGen/ILGenCoroutine.cpp
        src/Support/Timer.cpp include/cdotc/Support/Timer.h src/ClangImporter/ClangImporter.cpp include/cdotc/ClangImporter/ClangImporter.h
        src/ClangImporter/ImportMacro.cpp src/ClangImporter/ImporterImpl.h src/ClangImporter/ImportExpr.cpp
        src/ClangImporter/ImportType.cpp src/ClangImporter/ImportDecl.cpp include/cdotc/Support/SaveAndRestore.h
        src/Basic/NestedNameSpecifier.cpp include/cdotc/Basic/NestedNameSpecifier.h include/cdotc/IL/Utils/InstructionCloner.h
        src/ILGen/ILGenSpecialize.cpp src/Tools/IRDebug/IRDebugAnnotatePass.cpp include/cdotc/Tools/IRDebug/IRDebugAnnotatePass.h
        src/Query/Query.cpp include/cdotc/Query/Query.h src/Query/QueryContext.cpp
        include/cdotc/Query/QueryContext.h src/Query/TypeQueries.cpp src/Query/BuiltinQueries.cpp src/Query/LookupQueries.cpp
        src/Query/DriverQueries.cpp src/Query/SemaQueries.cpp src/Query/UtilQueries.cpp src/Query/CTFEQueries.cpp include/cdotc/Support/Config.h
        src/Query/ILGenQueries.cpp src/Query/InstantiationQueries.cpp src/Parse/ParseModule.cpp src/Sema/ConstraintSystem.cpp
        include/cdotc/Sema/ConstraintSystem.h src/Sema/ConstraintBuilder.cpp include/cdotc/Sema/ConstraintBuilder.h
        src/Sema/ConstraintGraph.cpp include/cdotc/Sema/ConstraintGraph.h src/Sema/ConstraintStep.cpp include/cdotc/Sema/ConstraintStep.h
        src/Sema/CSDiags.cpp src/Sema/CSApply.cpp src/Sema/OverloadResolver.cpp)

set(CDOTC_TBLGENS_SOURCE_FILES
        src/Tools/TableGenBackends/BuiltinOperatorBackend.cpp
        src/Tools/TableGenBackends/DiagnosticsBackend.cpp
        src/Tools/TableGenBackends/BuiltinTypesBackend.cpp
        src/Tools/TableGenBackends/AttributesBackend.cpp
        include/cdotc/Support/StringSwitch.h src/Tools/TableGenBackends/QueryBackend.cpp)

set (RUNTIME_SUPPORT_SOURCE_FILES
        Runtime/RuntimeSupport.cpp)

add_executable(cdotc src/Driver/main.cpp ${SOURCE_FILES} src/Support/Log.cpp include/cdotc/Support/Log.h src/Sema/SemaDebug.cpp src/Lex/SourceLocation.cpp)
add_executable(cdotc_asan src/Driver/main.cpp ${SOURCE_FILES})
add_executable(cdotc_ubsan src/Driver/main.cpp ${SOURCE_FILES})

add_library(cdotc-tblgens SHARED ${CDOTC_TBLGENS_SOURCE_FILES})
add_library(cdotrt SHARED ${RUNTIME_SUPPORT_SOURCE_FILES})

set_target_properties(cdotrt
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "/usr/local/lib"
    LIBRARY_OUTPUT_DIRECTORY "/usr/local/lib"
    RUNTIME_OUTPUT_DIRECTORY "/usr/local/lib")

install(TARGETS cdotrt
        DESTINATION "/usr/local/lib")

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
target_link_libraries(cdotc-tblgens ${llvm_libs})

target_link_libraries(cdotc_asan
        ${llvm_libs} ${clang_libs}
        -fsanitize=address
        -fno-omit-frame-pointer)
target_link_libraries(cdotc_ubsan
        ${llvm_libs} ${clang_libs}
        -fsanitize=undefined
        -fno-omit-frame-pointer)