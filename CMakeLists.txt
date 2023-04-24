cmake_minimum_required(VERSION 3.7)

project("Kokolang")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# compiler must be at least 17
set(CMAKE_CXX_STANDARD 17)

find_package(antlr4-runtime CONFIG REQUIRED)

#include deps header only libs
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps)

include_directories(${HEADER_ONLY_LIBS})
include_directories(${ANTLR4_INCLUDE_DIR})

# add our project source directories
include_directories(${PROJECT_SOURCE_DIR}/src/klvm/include)
include_directories(${PROJECT_SOURCE_DIR}/src/kilasm/include)
include_directories(${PROJECT_SOURCE_DIR}/src/kilasm/gen)

add_subdirectory(src)

add_executable(KokoLangExe "src/host/KokoLang.cpp")
set_target_properties(KokoLangExe PROPERTIES OUTPUT_NAME "kl")
find_package(Threads REQUIRED)
target_link_libraries(KokoLangExe kilasm_static klvm Threads::Threads nowide)

#set environment variable to ON if you want to enable testing
if (BUILD_TESTING)
	add_subdirectory(test)
endif()