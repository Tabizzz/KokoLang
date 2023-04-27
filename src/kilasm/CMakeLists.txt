
# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
set(ANTLR_EXECUTABLE ${CMAKE_SOURCE_DIR}/tools/antlr-4.10.1-complete.jar)

# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)
find_package(antlr4-runtime CONFIG REQUIRED)
find_package(Threads REQUIRED)

# Call macro to add lexer and grammar to your build dependencies.
antlr_target(KokoLangParser ${CMAKE_SOURCE_DIR}/KokoLang.g4 VISITOR OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/gen/)

set(lib_files
        "include/kilasm.h"
        "Impl/kilasm_internal.h"
        "Impl/ProgramVisitor.h"
        "Impl/ProgramVisitor.cpp"
        "Impl/klloader.cpp"
        "Impl/kopcodeshelper.cpp"
        "include/klloader.h"
        "include/klcompiler.h"
        "include/klibimports.h")

add_library(kilasm SHARED
        ${ANTLR_KokoLangParser_CXX_OUTPUTS}
        ${lib_files})
add_library(kilasm_static STATIC
        ${ANTLR_KokoLangParser_CXX_OUTPUTS}
        ${lib_files})

set_target_properties(kilasm PROPERTIES OUTPUT_NAME "kilasm")
target_compile_definitions(kilasm_static PRIVATE KOKOLANGLIB_STATIC)

# link required libraries
if (TARGET antlr4_shared)
    target_link_libraries(kilasm antlr4_shared klvm Threads::Threads)
else ()
    target_link_libraries(kilasm antlr4_static klvm Threads::Threads)
endif ()
target_link_libraries(kilasm_static antlr4_static klvm Threads::Threads)
