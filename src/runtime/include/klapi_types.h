#pragma once
#include "DataTypes/KLObject.h"

/*
 * here are the builtin types and functions related  creating
 * and deleting instances to types.
 */

#define KLCAST(x, y) ((x*)(y))
#define KLWRAP(x) ((KlObject*)(x))

/**
 * Builtin type int.
 */
CAPI KLType klBType_Int;

/**
 * Builtin type flt.
 */
CAPI KLType klBType_Float;

/**
 * Builtin type bit.
 */
CAPI KLType klBType_Bool;

/**
 * Builtin type str.
 */
CAPI KLType klBType_String;

/**
 * Builtin type ptr.
 */
CAPI KLType klBType_Ptr;

/**
 * Builtin type object ptr.
 */
CAPI KLType klBType_OPtr;

/**
 * Builtin type arr.
 */
CAPI KLType klBType_Arr;

/**
 * Builtin type type.
 */
CAPI KLType klBType_Type;

/**
 * Builtin type register.
 */
CAPI KLType klBType_Reg;

/**
 * Create a new object calling the constructor of the type.
 */
CAPI KlObject* klNew(KLType* type, KlObject** args, kbyte argc);


/**
 * Create a new instance of an object only calling the initializer but not the
 * constructor.
 */
CAPI KlObject* klIns(KLType* type);

/**
 * Increase the ref count of an object.
 */
CAPI void klRef(KlObject* object);

/**
 * Decrease the refcount of an object.
 * If the refcount is now 0 the object is destroyed.
 */
CAPI void klDeref(KlObject* object);

/**
 * Destroy an object, this call the finalizer and free the memory.
 */
CAPI void klDestroy(KlObject* object);

/**
 * Invokes a KlObject, if the object is a function is invoked directly, if is a
 * type with a method called "call" that method is used.
 */
CAPI KlObject* klInvoke(KlObject *target, KlObject **argv, kbyte argc);