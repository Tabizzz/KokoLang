#pragma once
#include "DataTypes/KLObject.h"

/*
 * here are the builtin types and functions related  creating
 * and deleting instances to types.
 */

#define KLCAST(x, y) ((x*)(y))
/**
 * @brief Cast any object as KlObject.
 */
#define KLWRAP(x) ((KlObject*)(x))

/**
 * Builtin type int.
 */
CAPI KLType klint_t;

/**
 * Builtin type flt.
 */
CAPI KLType klfloat_t;

/**
 * Builtin type bit.
 */
CAPI KLType klbool_t;

/**
 * Builtin type str.
 */
CAPI KLType klstring_t;

/**
 * Builtin type ptr.
 */
CAPI KLType klptr_t;

/**
 * Builtin type object ptr.
 */
CAPI KLType kloptr_t;

/**
 * Builtin type arr.
 */
CAPI KLType klarray_t;

/**
 * Builtin list type.
 */
CAPI KLType kllist_t;

/**
 * Builtin map type.
 */
CAPI KLType klmap_t;

/**
 * Builtin type type.
 */
CAPI KLType kltype_t;

/**
 * Builtin type register.
 */
CAPI KLType klreg_t;

CAPI
/**
 * Create a new object calling the constructor of the type.
 */
KlObject* klNew(KLType* type, KlObject** args, kbyte argc);

CAPI
/**
 * Create a new instance of an object only calling the initializer but not the
 * constructor.
 */
KlObject* klIns(KLType* type);

CAPI
/**
 * Increase the ref count of an object.
 */
inline void klRef(KlObject *object) {
	if(object && !KLTYPE_IS_STATIC(object->type)) object->refs++;
}

CAPI
/**
 * Decrease the refcount of an object.
 * If the refcount is now 0 the object is destroyed.
 */
void klDeref(KlObject* object);

CAPI
/**
 * Destroy an object, this call the finalizer and free the memory.
 */
void klDestroy(KlObject* object);

CAPI
/**
 * @briefs Invokes a KlObject, if the object is a function is invoked directly, if is a
 * type with a method called "call" that method is used.
 *
 * Any invocable object in kokolang must be invoked using this function.
 * *
 * This method also checks the number of arguments passed to the function.
 *
 * @param target The object to invoke.
 * If is a function is invoked directly with the klinvokable.
 * If is a package, this will find a function called 'main' or 'code' on that package and invoke that function, if not present throws an error.
 * If is another object, this will find a function called 'call' on the type of the object and run that, if not present throws an error.
 * @param argv An array with all the parameters to pass to the invoked object.
 * @param argc The number of parameters in argv.
 *
 * @return The object returned by the invoked object.
 */
KlObject* klInvoke(KlObject *target, KlObject **argv, kbyte argc);