cmake_minimum_required(VERSION 3.7)

project("Kokolang")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# compiler must be at least 17
set(CMAKE_CXX_STANDARD 17)

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

set(ANTLR4_ZIP_REPOSITORY ${CMAKE_CURRENT_SOURCE_DIR}/deps/antlr/antlr4-master.zip)

# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
# add external build for antlrcpp
include(ExternalAntlr4Cpp)
# add antrl4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})

#include deps header only libs
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps)
include_directories(${HEADER_ONLY_LIBS})

# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/tools/antlr-4.10.1-complete.jar)
# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)

# Call macro to add lexer and grammar to your build dependencies.
antlr_target(KokoLangParser KokoLang.g4 VISITOR OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/compiler/gen/)

# add our project source directories
include_directories(${PROJECT_SOURCE_DIR}/src/runtime/include)
include_directories(${PROJECT_SOURCE_DIR}/src/compiler/include)
include_directories(${PROJECT_SOURCE_DIR}/src/compiler/gen)
include_directories(${PROJECT_SOURCE_DIR}/src/translator/include)

set(include_files 
	"src/runtime/include/KokoLang.h"
	"src/runtime/include/klapi.h"
	"src/runtime/include/klimports.h"
	"src/runtime/include/DataTypes/KLRuntime.h"
	"src/runtime/include/DataTypes/KLPackage.h"
	"src/runtime/include/DataTypes/KLFunction.h"
	"src/runtime/include/DataTypes/KLInstruction.h"
	"src/runtime/include/DataTypes/KLObject.h"
	"src/runtime/include/DataTypes/KLCall.h"
	"src/runtime/include/kltypedef.h"
	"src/runtime/include/KOpcode.h"
	"src/runtime/include/klapi_types.h"
	"src/runtime/include/klapi_builtins.h"
	"src/runtime/Impl/DataTypes/KLFunctionImpl.h"
	"src/runtime/include/kldelegates.h"
	"src/runtime/include/klbinary.h" src/runtime/include/Binary/KMetaType.h src/runtime/include/Binary/KDefinitionType.h)

set(source_files 
	"src/runtime/Impl/klapi.cpp"
	"src/runtime/Impl/DataTypes/KLFunction.cpp"
	"src/runtime/Impl/DataTypes/KLPackage.cpp"
	"src/runtime/Impl/DataTypes/KLInstruction.cpp"
	"src/runtime/Impl/klapi_types.cpp"
	"src/runtime/Impl/klapi_builtins.cpp"
	"src/runtime/Impl/DataTypes/OpCodesImpl.cpp"
	"src/runtime/Impl/klbinary.cpp")

set(lib_files
	"src/runtime/include/KokoLangLib.h"
	"src/runtime/Impl/KokoLangInternal.h"
	"src/compiler/Grammar/KLInterpreter.h"
	"src/compiler/Grammar/ProgramVisitor.h"
	"src/compiler/Grammar/ProgramVisitor.cpp"
	"src/compiler/KokoLangLib.cpp"
	"src/compiler/Grammar/kopcodeshelper.cpp")

add_compile_definitions(KOKOLANGLIB_STATIC)

add_library(KokoLangRuntime SHARED
			${source_files}
			${include_files})
add_library(KokoLangRuntimeStatic STATIC
			${source_files}
			${include_files})

set_target_properties(KokoLangRuntime PROPERTIES OUTPUT_NAME "klruntime")
set_target_properties(KokoLangRuntimeStatic PROPERTIES OUTPUT_NAME "klruntime-static")
target_compile_definitions(KokoLangRuntimeStatic PRIVATE KOKOLANG_STATIC)


add_library(KokoLangLib SHARED
			${ANTLR_KokoLangParser_CXX_OUTPUTS}
			${lib_files})
add_library(KokoLangLibStatic STATIC
			${ANTLR_KokoLangParser_CXX_OUTPUTS}
			${lib_files})

set_target_properties(KokoLangLib PROPERTIES OUTPUT_NAME "kokolang")
target_compile_definitions(KokoLangLibStatic PRIVATE KOKOLANGLIB_STATIC)

add_executable(KokoLangExe "src/testing/KokoLang.cpp")
set_target_properties(KokoLangExe PROPERTIES OUTPUT_NAME "kl")

find_package(Threads REQUIRED)
# link required libraries
target_link_libraries(KokoLangLib antlr4_static KokoLangRuntime Threads::Threads)
target_link_libraries(KokoLangLibStatic antlr4_static KokoLangRuntime Threads::Threads)

target_link_libraries(KokoLangExe KokoLangLibStatic KokoLangRuntime Threads::Threads nowide)


