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
antlr_target(KokoLangParser KokoLang.g4 VISITOR OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/kilasm/gen/)

# add our project source directories
include_directories(${PROJECT_SOURCE_DIR}/src/klvm/include)
include_directories(${PROJECT_SOURCE_DIR}/src/kilasm/include)
include_directories(${PROJECT_SOURCE_DIR}/src/kilasm/gen)

set(lib_files
	"src/kilasm/include/KokoLangLib.h"
	"src/klvm/Impl/KokoLangInternal.h"
	"src/kilasm/Grammar/KLInterpreter.h"
	"src/kilasm/Grammar/ProgramVisitor.h"
	"src/kilasm/Grammar/ProgramVisitor.cpp"
	"src/kilasm/KokoLangLib.cpp"
	"src/kilasm/Grammar/kopcodeshelper.cpp")

add_compile_definitions(KOKOLANGLIB_STATIC)

add_subdirectory(src)

add_library(KokoLangLib SHARED
			${ANTLR_KokoLangParser_CXX_OUTPUTS}
			${lib_files})
add_library(KokoLangLibStatic STATIC
			${ANTLR_KokoLangParser_CXX_OUTPUTS}
			${lib_files})

set_target_properties(KokoLangLib PROPERTIES OUTPUT_NAME "kokolang")
target_compile_definitions(KokoLangLibStatic PRIVATE KOKOLANGLIB_STATIC)

add_executable(KokoLangExe "src/host/KokoLang.cpp")
set_target_properties(KokoLangExe PROPERTIES OUTPUT_NAME "kl")

find_package(Threads REQUIRED)
# link required libraries
target_link_libraries(KokoLangLib antlr4_static klvm Threads::Threads)
target_link_libraries(KokoLangLibStatic antlr4_static klvm Threads::Threads)

target_link_libraries(KokoLangExe KokoLangLibStatic klvm Threads::Threads nowide)

#set environment variable to ON if you want to enable testing
if (BUILD_TESTING)
	add_subdirectory(test)
endif()