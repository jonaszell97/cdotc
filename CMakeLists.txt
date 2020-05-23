cmake_minimum_required(VERSION 3.7)

project(CDot)

# find tblgen
find_package(TblGen REQUIRED CONFIG)
include_directories("${TBLGEN_INCLUDE_DIR}")
message(STATUS "Found TblGen ${TBLGEN_MAIN_INCLUDE_DIR}")

# find LLVM
find_package(LLVM 10 REQUIRED CONFIG)

# llvm libraries
set(llvm_libs ${LLVM_AVAILABLE_LIBS})
string(REPLACE ";LLVM;" ";" llvm_libs "${llvm_libs}")
string(REPLACE ";LLVM-C" "" llvm_libs "${llvm_libs}")

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Found LLVM libs ${llvm_libs}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# find clang
set(Clang_DIR "${LLVM_DIR}/../clang")
find_package(Clang REQUIRED CONFIG HINTS ${Clang_DIR})

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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pedantic")
if(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")
endif()

# set Linker flags
if(UNIX AND NOT APPLE)
    set(linker_flags "-fuse-ld=/usr/bin/ld.lld-10 -Wl,--start-group")
else()
    set(linker_flags "")
endif()

# add include directories
include_directories(include)

# find source files
file(GLOB_RECURSE CDot_SRC "src/*.cpp")
file(GLOB_RECURSE CDot_INCLUDE include/cdotc "src/*.h")
set(SOURCE_FILES ${CDot_SRC} ${CDot_INCLUDE})

set(CDOTC_TBLGENS_SOURCE_FILES
        tools/TableGenBackends/BuiltinOperatorBackend.cpp
        tools/TableGenBackends/DiagnosticsBackend.cpp
        tools/TableGenBackends/BuiltinTypesBackend.cpp
        tools/TableGenBackends/AttributesBackend.cpp
        tools/TableGenBackends/QueryBackend.cpp)

set (RUNTIME_SUPPORT_SOURCE_FILES
        Runtime/RuntimeSupport.cpp)

add_executable(cdotc src/Driver/main.cpp ${SOURCE_FILES})
add_executable(cdotc_asan src/Driver/main.cpp ${SOURCE_FILES})
add_executable(cdotc_ubsan src/Driver/main.cpp ${SOURCE_FILES})
add_executable(cdotc_coverage src/Driver/main.cpp ${SOURCE_FILES})

add_library(cdotc-tblgens SHARED ${CDOTC_TBLGENS_SOURCE_FILES})
add_library(cdotrt SHARED ${RUNTIME_SUPPORT_SOURCE_FILES})

target_compile_options(cdotc
        PUBLIC "-fvisibility=hidden")
target_compile_options(cdotc_asan
        PUBLIC "-fsanitize=address"
        PUBLIC "-fno-omit-frame-pointer"
        PUBLIC "-fvisibility=hidden")
target_compile_options(cdotc_ubsan
        PUBLIC "-fsanitize=undefined"
        PUBLIC "-fno-omit-frame-pointer"
        PUBLIC "-fvisibility=hidden")
target_compile_options(cdotc_coverage
        PUBLIC "-fprofile-instr-generate"
        PUBLIC "-fcoverage-mapping"
        PUBLIC "-fvisibility=hidden")

target_link_libraries(cdotc LLVM Polly ${clang_libs} -fvisibility=hidden -w ${linker_flags})
target_link_libraries(cdotc-tblgens LLVMSupport -fvisibility=hidden)

target_link_libraries(cdotc_asan
        ${llvm_libs} ${clang_libs}
        -fsanitize=address
        -fno-omit-frame-pointer
        -fvisibility=hidden)
target_link_libraries(cdotc_ubsan
        ${llvm_libs} ${clang_libs}
        -fsanitize=undefined
        -fno-omit-frame-pointer
        -fvisibility=hidden)

target_link_libraries(cdotc_coverage ${llvm_libs} ${clang_libs}
        "-fprofile-instr-generate" "-fcoverage-mapping" -fvisibility=hidden)