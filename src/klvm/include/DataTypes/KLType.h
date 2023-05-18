#pragma once

#include "DataTypes/KLObject.h"

#include "kldelegates.h"

#define KLTYPE_FLAG_NOINSTANCE 1
#define KLTYPE_IS_STATIC(x) (x->flags & KLTYPE_FLAG_NOINSTANCE)

#define KLTYPE_METADATA(type)        \
type->functions = new MetaMap();     \
type->variables = new MetaMap();     \
type->metadata = new MetaMap();

struct CPPAPI KLType {
	KLOBJECTHEAD
	const char *name;             // the name of the types
	size_t inscount;              // the number of instances
	size_t size;                  // the size to allocate this an object of this type
	kltypefunc initializer;       // the initializer to this type
	kltypefunc finalizer;         // the finalizer to this type
	KlObject *constructor;        // the constructor of the type

	klunaryop toString;           // return the string representation of the type
	klunaryop toInt;              // return the int representation of the type
	klunaryop toFloat;            // return the float representation of the type
	klunaryop toBool;             // return the bool representation of the type
	klbinaryop toType;            // convert the type to another type
	klunaryop cast;               // convert the instance of another type to this type

	/*
	 * The comparer must return 1 if second is bigger and -1 is first is bigger.
	 */
	klcomparer comparer;          // the comparer of the type
	klcomparer equal;             // the equality checker of the type

	klresoperation opAdd;         // addition operation
	klresoperation opSub;         // subtraction operation
	klresoperation opMul;         // multiplication operation
	klresoperation opDiv;         // division operation
	klresoperation opMod;         // modulo operation

	klunaryop clone;              // clone operation
	klcopyop copy;                // copy operation

	klbinaryop getter;            // array indexer, get operation
	klsetop setter;               // array indexer, set operation

	/*
	 * flags of this type:
	 * 0: if the first flag is set, then the type cannot be instanced, this is for types that you will manually
	 * instance and cannot be instanced by klruntime.
	 *
	 * other flags are currently unused.
	 */
	kbyte flags;

	MetaMap *functions;           // the functions defined in the type
	MetaMap *variables;           // the globals var of the type.
	MetaMap *metadata;            // metadata of the type
};