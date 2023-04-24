# header files
set(include_files
        "include/KokoLang.h"
        "include/klapi.h"
        "include/klimports.h"
        "include/DataTypes/KLRuntime.h"
        "include/DataTypes/KLPackage.h"
        "include/DataTypes/KLFunction.h"
        "include/DataTypes/KLInstruction.h"
        "include/DataTypes/KLObject.h"
        "include/DataTypes/KLCall.h"
        "include/kltypedef.h"
        "include/KOpcode.h"
        "include/klapi_types.h"
        "include/klapi_builtins.h"
        "Impl/DataTypes/KLFunctionImpl.h"
        "include/kldelegates.h"
        "include/klbinary.h"
        "include/Binary/KMetaType.h"
        "include/Binary/KDefinitionType.h"
        "include/DataTypes/KLType.h"
        "include/DataTypes/KLVariable.h"
        "Impl/fnptr.h"
        "include/klresolver.h" Impl/klresolverImpl.h)
#source files
set(source_files
        "Impl/klapi.cpp"
        "Impl/DataTypes/KLFunction.cpp"
        "Impl/DataTypes/KLPackage.cpp"
        "Impl/DataTypes/KLInstruction.cpp"
        "Impl/klapi_types.cpp"
        "Impl/klapi_builtins.cpp"
        "Impl/DataTypes/OpCodesImpl.cpp"
        "Impl/klbinary.cpp"
        "Impl/DataTypes/KLVariable.cpp" Impl/klresolver.cpp)

add_library(klvm SHARED
        ${source_files}
        ${include_files})
add_library(klvm_static STATIC
        ${source_files}
        ${include_files})

set_target_properties(klvm PROPERTIES OUTPUT_NAME "klvm")
set_target_properties(klvm PROPERTIES OUTPUT_NAME "klvm-static")
target_compile_definitions(klvm_static PRIVATE KOKOLANG_STATIC)

