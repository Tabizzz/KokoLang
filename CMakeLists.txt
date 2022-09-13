cmake_minimum_required(VERSION 3.7)

project("Kokolang")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# compiler must be at least 17
set(CMAKE_CXX_STANDARD 17)

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

set(ANTLR4_ZIP_REPOSITORY ${CMAKE_CURRENT_SOURCE_DIR}/antlr/antlr4-master.zip)

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
antlr_target(KokoLangParser KokoLang.g4 VISITOR OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/)

# add our project source directories
include_directories(${PROJECT_SOURCE_DIR})


add_library(KokoLang SHARED
			${ANTLR_KokoLangParser_CXX_OUTPUTS}
			"klapi.cpp" 
			"Interpreter/ProgramVisitor.cpp" 
			"Runtime/KLProgram.cpp")

add_executable(KokoLangTest "KokoLang.cpp")


# link required libraries
target_link_libraries(KokoLang antlr4_static)

target_link_libraries(KokoLangTest KokoLang)
