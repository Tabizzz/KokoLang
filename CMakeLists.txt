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

# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/tools/antlr-4.10.1-complete.jar)
# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)

# Call macro to add lexer and grammar to your build dependencies.
antlr_target(KokoLangParser KokoLang.g4 VISITOR OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/lib/gen/)

# add our project source directories
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/src/lib)

set(include_files 
	"src/include/KokoLang.h"
	"src/include/klapi.h"
	"src/include/KLImports.h"
	"src/include/Runtime/KLRuntime.h"
	"src/include/Runtime/KLPackage.h"
	"src/include/Runtime/KLFunction.h"
	"src/include/Runtime/KLInstruction.h"
	"src/include/Runtime/KLObject.h"
	"src/include/Runtime/KLTypes.h"
	"src/include/Runtime/OpCodes.h"
	"src/include/klapi_types.h"
	"src/include/Runtime/KLCall.h" 
	"src/include/klapi_builtins.h"
		src/lib/RuntimeImpl/KLFunctionImpl.h)

set(source_files 
	"src/lib/klapi.cpp" 
	"src/lib/RuntimeImpl/KLFunction.cpp"
	"src/lib/RuntimeImpl/KLPackage.cpp"
	"src/lib/RuntimeImpl/KLInstruction.cpp"
	"src/lib/klapi_types.cpp"
	"src/lib/klapi_builtins.cpp" src/lib/RuntimeImpl/KLCall.cpp)

set(lib_files
	"src/include/KokoLangLib.h"
	"src/lib/KokoLangInternal.h"
	"src/lib/Interpreter/KLInterpreter.h"
	"src/lib/Interpreter/ProgramVisitor.h"
	"src/lib/Interpreter/ProgramVisitor.cpp" 
	"src/lib/KokoLangLib.cpp")

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
target_compile_definitions(KokoLangLibStatic PRIVATE KOKOLANG_STATIC)

add_executable(KokoLangExe "src/kokolang/KokoLang.cpp")
set_target_properties(KokoLangExe PROPERTIES OUTPUT_NAME "kl")

find_package(Threads REQUIRED)
# link required libraries
target_link_libraries(KokoLangLib antlr4_static KokoLangRuntime Threads::Threads)
target_link_libraries(KokoLangLibStatic antlr4_static KokoLangRuntime Threads::Threads)

target_link_libraries(KokoLangExe KokoLangLib KokoLangRuntime)


