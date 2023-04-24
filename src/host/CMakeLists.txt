
add_executable(klhost "KokoLang.cpp")
set_target_properties(klhost PROPERTIES OUTPUT_NAME "kl")
find_package(Threads REQUIRED)
find_path(TERMCOLOR_INCLUDE_DIRS "termcolor/termcolor.hpp")
target_link_libraries(klhost kilasm klvm Boost::boost Boost::nowide)
target_include_directories(klhost PRIVATE ${TERMCOLOR_INCLUDE_DIRS})
