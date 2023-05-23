#pragma once

#include "DataTypes/KLObject.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * A package is an object that contains definitions of functions, types and variables.
 * Is possible to have sub packages in a package.
 * To run a package it need to have a main method that receive minimum 1 arg.
 *
 * Definitions don't collapse, that is, a package can contain a type named x and a
 * function named x.
 */
struct CPPAPI KLPackage
{
	KLOBJECTHEAD
	/*
	* The name of the package, is not possible to have multiples packages with the
	* same name.
	*/
	KLObject* name;
	MetaMap* functions;		// the functions defined in the package
	MetaMap* variables;		// the globals var of the package.
	MetaMap* types;			// the types defined in the package.
	MetaMap* packs;     	// packs of the package
	MetaMap* metadata;      // metadata of the package
};

#ifdef __cplusplus
}
#endif

/*
 * Register a type in the package.
 */
CAPI void klPackageRegType(KLPackage* klPackage, KLType* type);

/*
 * Create a new empty package, name must be changed.
 */
CAPI KLPackage* klCreatePackage();

/*
 * Destroy a package.
 */
CAPI void klDestroyPackage(KLPackage* klPackage);

/*
 * Build a package.
 */
CAPI void klBuildPackage(KLPackage* klPackage, kbyte recursive = 1);

/*
 * Run the 'main' function in a package.
 */
CAPI int klRunPackage(KLPackage* klPackage, int argc, const char* argv[]);

CAPI KLType* klpack_t;