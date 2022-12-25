#pragma once
#include "klimports.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define KLOBJECTHEAD KlObject klbase;

#define KLINVOKE(x) if(x) x

struct KLType;

/*
 * The global dynamic object representation in kokolang.
 * Any type can be used a pointer to this class to be a valid kokolang object.
 */
typedef struct KlObject {
	struct KLType* type; 	// the type of this object
	size_t refs;			// how many objects are referencing this
} KlObject;

#include "kldelegates.h"

typedef struct KLType {
	KLOBJECTHEAD
	const char *name;			// the name of the types
	size_t inscount;			// the number of instances
	size_t size;				// the size to allocate this an object of this type
	klinitializer initializer;	// the initializer to this type
	KlObject* constructor;		// the constructor of the type
	klfinalizer finalizer;      // the finalizer to this type

	klunaryop toString;			// return the string representation of the type
	klunaryop toInt;			// return the int representation of the type
	klunaryop toFloat;			// return the float representation of the type
	klunaryop toBool;			// return the bool representation of the type
	klbinaryop toType;			// convert the type to another type
	klunaryop cast;				// convert the instance of another type to this type

	/*
	 * The comparer must return 1 if second is bigger and -1 is first is bigger.
	 */
	klcomparer comparer;		// the comparer of the type
	klcomparer equal;			// the equality checker of the type

	klregoperation opAdd;		// addition operation
	klregoperation opSub;		// subtraction operation
	klregoperation opMul;		// multiplication operation
	klregoperation opDiv;		// division operation
	klregoperation opMod;		// modulo operation

	klunaryop clone;			// clone operation
	klcopy copy;				// copy operation
} KlType;

#ifdef __cplusplus
}
#endif

