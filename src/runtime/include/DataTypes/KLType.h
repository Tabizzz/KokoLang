#pragma once

#include <map>
#include "klimports.h"
#include "DataTypes/KLObject.h"

#include "kldelegates.h"

struct CPPAPI KLType {
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

	std::map<std::string, KlObject *> functions;		// the functions defined in the type
	std::map<std::string, KlObject *> variables;		// the globals var of the type.
	std::map<std::string, KlObject *> metadata;        // metadata of the type

};