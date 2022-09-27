#pragma once
#include "klapi.h"

/*
 * here are the builtin types and functions related  creating
 * and deleting instances to types.
 */

#define KLCAST(x, y) ((x*)y)
#define KLWRAP(x) ((KlObject*)x)

/*
 * Builtin type int.
 */
CAPI KlType klBType_Int;

/*
 * Builtin type flt.
 */
CAPI KlType klBType_Float;

/*
 * Builtin type bit.
 */
CAPI KlType klBType_Bool;

/*
 * Builtin type str.
 */
CAPI KlType klBType_String;

/*
 * Builtin type ptr.
 */
CAPI KlType klBType_Ptr;

/*
 * Builtin type object ptr.
 */
CAPI KlType klBType_OPtr;

/*
 * Builtin type arr.
 */
CAPI KlType klBType_Arr;

/*
 * Builtin type type.
 */
CAPI KlType klBType_Type;

/*
 * Builtin type register.
 */
CAPI KlType klBType_Reg;

/*
 * Create a new object calling the constructor of the type.
 */
CAPI KlObject* klNew(KlType* type, KlObject** args, int argc);

/*
 * Create a new object calling the constructor of the type.
 */
CAPI KlObject* klNewVar(KlType* type, KlObject* args...);

/*
 * Create a new instance of an object only calling the initializer but not the
 * constructor.
 */
CAPI KlObject* klIns(KlType* type);

/*
 * Increase the ref count of an object.
 */
CAPI void klRef(KlObject* object);

/*
 * Decrease the refcount of an object.
 * If the refcount is now 0 the object is destroyed.
 */
CAPI void klDeref(KlObject* object);

/*
 * Destroy an object, this call the finalizer and free the memory.
 */
CAPI void klDestroy(KlObject* object);