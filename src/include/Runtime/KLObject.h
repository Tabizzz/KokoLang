#pragma once
#include "KLImports.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define KLOBJECTHEAD KlObject klbase;

#define KLINVOKE(x) if(x) x

struct _klTYpe;

/*
 * The global dynamic object representation in kokolang.
 * Any type can be used a pointer to this class to be a valid kokolang object.
 */
typedef struct _klObject {
	struct _klTYpe* type; 	// the type of this object
	size_t refs;			// how many objects are referencing this
} KlObject;

typedef void (*klinitializer)(KlObject*);   // the initializer is the responsible for example set ints to 0
typedef void (*klfinalizer)(KlObject*);     // the finalizer is the responsible for example free memory allocation/ deref other object

typedef struct _klTYpe {
	//KLOBJECTHEAD
	const char *name;			// the name of the type
	size_t inscount;			// the number of instances
	size_t size;				// the size to allocate this a object of this type
	klinitializer initializer;	// the initializer to this type
	klfinalizer finalizer;      // the finalizer to this type
} KlType;


#ifdef __cplusplus
};
#endif

