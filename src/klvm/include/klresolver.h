#pragma once

/*
 * provide methods for resolving symbols.
 */

#include "DataTypes/KLRuntime.h"

#define KLRESOLVE_PACKAGE 	0b00001
#define KLRESOLVE_VARIABLE 	0b00010
#define KLRESOLVE_FUNCTION 	0b00100
#define KLRESOLVE_TYPE		0b01000
#define KLRESOLVE_GLOBAL	0b10000

/**
 * @brief A method used to resolve a reference to a target definition by name.
 *
 * When mode is true and if necessary the resolver must load the packages.
 */
typedef KLObject*(*klresolver)(
		/**
		 * @brief The full name of the object to resolve. Always type string.
		 */
		const KLObject* fullname,
		/**
         * @brief The package resolving the object.
         */
		const KLPackage* package,
		/**
         * @brief The type resolving the object. Can be null if is a package function resolving.
         */
		const KLType* type,
		/**
         * @brief The function resolving the object.
         */
		const KLFunction* function,
		/**
		 * @brief Generally indicated that the resolver should load or create the requested object.
		 *
		 * Is always true while building functions.
		 */
		kbyte mode);

CAPI
void klRestoreResolver();

CAPI
void klSetResolver(klresolver resolver);

CAPI
klresolver klGetResolver();