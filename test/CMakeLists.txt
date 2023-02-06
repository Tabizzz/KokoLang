project( Tests CXX )

add_executable(kltest main.cpp catch.hpp klapi.test.cpp testmacros.h)

target_link_libraries(kltest KokoLangRuntime Threads::Threads nowide)