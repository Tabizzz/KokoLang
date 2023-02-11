#pragma once

#include "klapi.h"
#include <iostream>

CAPI
/*
 * Load a package from a binary file.
 * If the package has no name this return the global package and the definitions on the package are added to the global.
 */
KLPackage* klCreatePackageFromFile(const char* filename);

CAPI
/*
 * Load a package from a binary stream.
 * If the package has no name this return the global package and the definitions on the package are added to the global.
 */
KLPackage* klCreatePackageFromStream(std::istream* stream);

//CAPI
/*
 * Load a package from a memory region.
 * If the package has no name this return the global package and the definitions on the package are added to the global.
 */
//KLPackage* klCreatePackageFromMemory(const void* ptr, size_t size);