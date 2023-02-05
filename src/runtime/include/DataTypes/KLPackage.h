﻿#pragma once

#include <map>
#include "klimports.h"
#include "DataTypes/KLFunction.h"
#include "DataTypes/KLInstruction.h"

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
	KlObject* name;
	std::map<std::string, KlObject *>* functions;		// the functions defined in the package
	std::map<std::string, KlObject *>* variables;		// the globals var of the package.
	std::map<std::string, KlObject *>* types;			// the types defined in the package.
	std::map<std::string, KlObject *>* subpacks;        // subpacks of the package
	std::map<std::string, KlObject *>* metadata;        // metadata of the package
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
CAPI void klPackage_Build(KLPackage* klPackage);

/*
 * Run the 'main' function in a package.
 */
CAPI int klPackage_Run(KLPackage* klPackage, int argc, const char* argv[]);

CAPI KLType klBType_Package;