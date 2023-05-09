project( Tests CXX )

add_executable(kltest
        main.cpp
        catch.hpp
        klapi.test.cpp
        testmacros.h
        klapi_builtins.test.cpp
        klapi_types.test.cpp
        kl_int.test.cpp
        kl_float.test.cpp
        kl_string.test.cpp)

target_link_libraries(kltest klvm)