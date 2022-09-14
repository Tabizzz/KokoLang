#pragma once
#include "Imports.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define KLOBJECTHEAD KlObject klbase;

struct _klObject;
struct _klTYpe;

typedef struct _klObject {
	struct _klTYpe* type; 	// the type of this object
	size_t refs;			// how many objects are referencing this
} KlObject;

typedef struct _klTYpe {
	//KLOBJECTHEAD
	const char *name;	// the name of the type
	size_t inscount;	// the number of instances
	size_t size;		// the size to allocate this a object of this type
} KlType;


#ifdef __cplusplus
};
#endif

