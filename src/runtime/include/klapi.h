﻿#pragma once
/*
 * Generic kokolang functions exposed to the C API.
 */

#include "klimports.h"
#include "DataTypes/KLRuntime.h"

#ifndef CAPI_H
#define CAPI_H

CAPI
/*
 * Initialize the kokolang runtime, this creates the std package.
 */
void klInit();

CAPI
/*
 * Finalize the runtime, disposing all the objects in the runtime,
 * call this to ensure all the objects created by the runtime
 * being finalized.
 */
void klEnd();

CAPI
/*
 *  Get the global package.
 *  This package contains the standard types and functions.
 */
const KLPackage* klGlobalPackage();

CAPI
/*
 * Add a package to the runtime to be used by others packages.
 */
void klRegisterPackage(KLPackage* klPackage);

CAPI
/*
 * Define a type, use this to initialize the header of any type.
 */
void klDefType(KlType* type);

CAPI
/*
 * Main object copy operation. This function will try to copy objects
 * by value or by reference if is not possible by value.
 * there are four different cases:
 *
 *  case A, src is null and dest is not null:
 * in this case the copy just will deref dest and set it to null.
 *
 *  case B, src is not null and dest is null:
 * in this case the copy will clone the object if it has a clone
 * operation, if not then the same instance is used and the ref
 * count is increased.
 *
 *  case C, both src and dest are null:
 * in this case nothing occurs.
 *
 *  Case D, both src and dest are not null:
 * this is the more complex case, first of all if src and dest are
 * from different types and the types src don't support clone then
 * dest is deref, and src is ref and copied by reference. if src
 * and dest are from the same type but the type don't support copy
 * operation then dest is deref, and src is ref and copied by reference.
 * if src and dest are from different types but src support clone
 * operation then the object is cloned, dest is deref and replaced
 * with the new object. if src and dest are from the same type
 * and the type support copy operation then src is copied into
 * dest and no ref count is changed.
 */
void klCopy(KlObject *src, KlObject** dest);

CAPI
/*
 * Main object clone operation. This function will try to clone
 * objects always or pass by reference if not possible to clone.
 * there are four different cases:
 *
 *  case A, src is null and dest is not null:
 * dest is deref and set to null.
 *  case B, src is not null and dest is null:
 * in this case the function will clone the object if it has a clone
 * operation, if not then the same instance is used and the ref
 * count is increased.
 *  case C, both src and dest are null:
 * in this case nothing occurs.
 *  Case D, both src and dest are not null:
 * in this case the function will clone the object if it has a clone
 * operation, if not then the same instance is used and the ref
 * count is increased on src and decreased in dest.
 */
void klClone(KlObject* src, KlObject** dest);

CAPI
/*
 * Main object move operation. This function will always move
 * objects by reference, so no new object are created or values
 * copied.
 *
 *  case A, src is null and dest is not null:
 * dest is deref and set to null.
 *  case B, src is not null and dest is null:
 * src is ref and set by reference to dest.
 *  case C, both src and dest are null:
 * in this case nothing occurs.
 *  Case D, both src and dest are not null:
 * dest is deref, src is ref and set by reference to dest.
 */
void klMove(KlObject* src, KlObject** dest);

#endif // !CAPI_H

