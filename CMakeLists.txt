cmake_minimum_required(VERSION 3.7)

project("Kokolang")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# compiler must be at least 17
set(CMAKE_CXX_STANDARD 17)

find_package(antlr4-runtime CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS nowide)

include_directories(${ANTLR4_INCLUDE_DIR})

# add our project source directories
include_directories(${PROJECT_SOURCE_DIR}/src/klvm/include)
include_directories(${PROJECT_SOURCE_DIR}/src/kilasm/include)
include_directories(${PROJECT_SOURCE_DIR}/src/kilasm/gen)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts)

if (TARGET_32B)
	add_compile_definitions(TARGET_32B)
endif()

list(APPEND CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib )

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-pedantic temp)
if(temp)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
endif()

if(NOT WIN32)
	set(CMAKE_INSTALL_PREFIX "/usr/share/kokolang")
	set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

add_subdirectory(src)

#set environment variable to ON if you want to enable testing
if (BUILD_TESTING)
	add_subdirectory(test)
endif()

add_subdirectory(install)