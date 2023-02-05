project( Tests CXX )

add_executable(kltest main.cpp catch.hpp klapi.test.cpp)

target_link_libraries(kltest KokoLangRuntime Threads::Threads nowide)