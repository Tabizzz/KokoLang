#pragma once

/*
 * provide methods for resolving symbols.
 */

#include "DataTypes/KLRuntime.h"

/**
 * @brief A method used to resolve a reference to a target definition by name.
 *
 * When mode is true and if necessary the resolver must load the packages.
 */
typedef KlObject*(*klresolver)(
		/**
		 * @brief The full name of the object to resolve. Always type string.
		 */
		const KlObject* fullname,
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
/**
 * Change the resolvers used by klruntime to search packages, if a null is passed that resolver is not changed.
 *
 * @param package Resolver for find packages, if mode is true, the resolver should load a package if it not already loaded in the runtime,
 * otherwise the resolver must only return a package already loaded.
 *
 */
void klConfigureResolvers(klresolver package, klresolver type, klresolver function, klresolver variable);

namespace KokoLang {
	class KLDefaultResolvers {
	public:
		static klresolver getPackageResolver();

		static klresolver getTypeResolver();

		static klresolver getFunctionResolver();

		static klresolver getVariableResolver();

	};
}