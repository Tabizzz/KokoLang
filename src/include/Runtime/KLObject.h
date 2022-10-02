#pragma once
#include "klimports.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define KLOBJECTHEAD KlObject klbase;

#define KLINVOKE(x) if(x) x

struct KlType;

/*
 * The global dynamic object representation in kokolang.
 * Any type can be used a pointer to this class to be a valid kokolang object.
 */
typedef struct KlObject {
	struct KlType* type; 	// the type of this object
	size_t refs;			// how many objects are referencing this
} KlObject;

typedef void (*klinitializer)(KlObject*);   // the initializer is the responsible for example set ints to 0
typedef void (*klfinalizer)(KlObject*);     // the finalizer is the responsible for example free memory allocation/ deref other object
typedef KlObject* (*klinvokable)			// a delegate for invokable objects like constructors or functions
	   (KlObject *func,						// the function object itself
		KlObject **args,					// the args passed to the function
		KlObject *argc);					// the number of arguments passed to the function

typedef int (*klcomparer)					// comparator signature
		(KlObject* first,
		 KlObject* second);

typedef KlObject* (*klbinaryop)				// a binary operator, receive two objects and returns a new one
	   (KlObject* first,
		KlObject* second);
typedef KlObject* (*klunaryop)				// a unary operation, receive one object and returns a new one
	   (KlObject* obj);

typedef struct KlType {
	KLOBJECTHEAD
	const char *name;			// the name of the type
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
	klbinaryop equal;			// the equality checker of the type

	klbinaryop opAdd;			// addition operation
	klbinaryop opSub;			// subtraction operation
	klbinaryop opMul;			// multiplication operation
	klbinaryop opDiv;			// division operation
	klbinaryop opMod;			// modulo operation

	klunaryop clone;			// clone operation
	klbinaryop copy;			// copy operation
} KlType;

#ifdef __cplusplus
};
#endif

