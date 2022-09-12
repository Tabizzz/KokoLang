cmake_minimum_required(VERSION 3.7)

project("Antlr-cpp-tutorial")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# compiler must be at least 17
set(CMAKE_CXX_STANDARD 17)

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

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

# CMake commands copied by the CMake file for CImg

# To use PKG_CHECK_MODULES to find some optional packages
find_package(PkgConfig)

# Call macro to add lexer and grammar to your build dependencies.
antlr_target(KokoLangParser KokoLang.g4 VISITOR OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/)

# add our project source directories
include_directories(${PROJECT_SOURCE_DIR}
					 ${PROJECT_SOURCE_DIR}/libs/
					 ${PROJECT_SOURCE_DIR}/Interpreter/
					 ${PROJECT_SOURCE_DIR}/Runtime/
  )
  

add_executable(KokoLang "KokoLang.cpp"
				${ANTLR_KokoLangParser_CXX_OUTPUTS} "klapi.c" "Interpreter/ProgramVisitor.cpp")

# link required libraries
target_link_libraries(KokoLang antlr4_static)

# copy the input file next to the generated executable
add_custom_command(TARGET KokoLang POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${PROJECT_SOURCE_DIR}/test.kl"
  $<TARGET_FILE_DIR:KokoLang>)

# necessary because the default working directory of Visual Studio
# is not the same as the one in which the binary is created
# so we copy the input file twice:
# one for when you launch the executale in Visual Studio (CTRL+F5)
# one for when you launch it normally
if (WIN32)
add_custom_command(TARGET KokoLang POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${PROJECT_SOURCE_DIR}/test.kl"
  $<TARGET_FILE_DIR:KokoLang>/../)
endif()

