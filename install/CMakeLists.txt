# these are cache variables, so they could be overwritten with -D,
set(CPACK_PACKAGE_NAME ${PROJECT_NAME}
        CACHE STRING "The resulting package name"
        )
# which is useful in case of packing only selected components instead of the whole thing
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Kokolang runtime"
        CACHE STRING "Package description for the package metadata"
        )
set(CPACK_PACKAGE_VENDOR "Kynet")

set(CPACK_VERBATIM_VARIABLES YES)

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
SET(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_SOURCE_DIR}/build/packages")

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(CPACK_PACKAGE_CONTACT "imkotokya@gmail.com")

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.md")
set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")

# that is if you want every group to have its own package,
# although the same will happen if this is not set (so it defaults to ONE_PER_GROUP)
# and CPACK_DEB_COMPONENT_INSTALL is set to YES
set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)#ONE_PER_GROUP)

if(WIN32)
    # https://cmake.org/cmake/help/latest/cpack_gen/nsis.html
    set(CPACK_NSIS_MODIFY_PATH true)
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL true)
    set(CPACK_NSIS_UNINSTALL_NAME "Uninstall Kokolang")
    set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/logo.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/logo.ico")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/Tabizzz/KokoLang")
    #set(CPACK_NSIS_MENU_LINKS "kl.exe" "Kokolang Interactive")
    set(CPACK_GENERATOR NSIS)
else()
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Tabizzz")
    set(CPACK_DEB_COMPONENT_INSTALL false)
    set(CPACK_GENERATOR DEB)
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/share/kokolang")
endif()

install(TARGETS klhost klvm kilasm
        RUNTIME DESTINATION "."
        LIBRARY DESTINATION "lib"
        ARCHIVE DESTINATION "bin")
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/link DESTINATION /bin RENAME kl)
include(CPack)