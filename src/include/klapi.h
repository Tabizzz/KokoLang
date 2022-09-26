#pragma once
/*
 * Generic kokolang functions exposed to the C API.
 */

#include "klimports.h"
#include "Runtime/KLRuntime.h"

#ifndef CAPI_H
#define CAPI_H

/*
 * Initialize the kokolang runtime, this creates the std package.
 */
CAPI void klInit();

/*
 * Finalize the runtime, disposing all the objects in the runtime,
 * call this to ensure all the objects created by the runtime
 * being finalized.
 */
CAPI void klEnd();

/*
 *  Get the standard package.
 *  This package contains the standard types and functions.
 */
CAPI KLPackage* klStdPackage();

/*
 * Add a package to the runtime to be used by others packages.
 */
CAPI void klRegisterPackage(KLPackage* klPackage);

/*
 * Define a type, use this to initialize the header of any type.
 */
CAPI void klDefType(KlType* type);

#endif // !CAPI_H

