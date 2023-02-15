#pragma once

/*
 * provide methods for resolving symbols.
 */

#include "DataTypes/KLRuntime.h"

/**
 * A method used to resolve a reference to a target definition by name, if necessary the resolver must load the package
 * and register it.
 *
 */
typedef KlObject*(*klresolver)(
		/**
		 * The full name of the object to resolve. Always type string.
		 */
		KlObject* fullname,
		/**
		 * The object requesting the resolver, generally a function.
		 */
		const KlObject* caller,
		/**
		 * For variable resolves: if true we need to create the variable if is not founds, if false we don't create the variable.
		 * For packages resolves: if true the package must be loaded if is not loaded, if false we don't load the package.
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