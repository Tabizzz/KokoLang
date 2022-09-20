#pragma once

#include <map>
#include "KLImports.h"
#include "Runtime/KLFunction.h"
#include "Runtime/KLInstruction.h"

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
	 * he Tname of the package, is not possible to have multiples packages with the
	 * same name.
	 */
	KlObject* name;
	map<string, KlObject *>* functions;		// the functions defined in the package
	map<string, KlObject *>* variables;		// the globals var of the package.
	map<string, KlObject *>* types;			// the types defined in the package.
	map<string, KlObject *>* subpacks;
};

/*
 * Register a type in the package.
 */
CAPI void klPackageRegType(KLPackage* klPackage, KlType* type);

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
CAPI void klPackage_Build(KLPackage* klPackage);

/*
 * Run the 'main' function in a package.
 */
CAPI int klPackage_Run(KLPackage* klPackage, int argc, const char* argv[]);

extern KlType klBType_Package;