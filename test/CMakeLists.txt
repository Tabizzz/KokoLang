project( Tests CXX )

find_package(Catch2 CONFIG REQUIRED)

add_executable(kltest
        main.cpp
        klapi.test.cpp
        testmacros.h
        klapi_builtins.test.cpp
        klapi_types.test.cpp
        kl_int.test.cpp
        kl_float.test.cpp
        kl_string.test.cpp)

target_link_libraries(kltest PRIVATE klvm Catch2::Catch2 Catch2::Catch2WithMain)