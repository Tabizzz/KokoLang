#pragma once

/*
 * provide methods for resolving symbols.
 */

#include "DataTypes/KLRuntime.h"

/*
 * A method used to resolve a reference to a target definition by name, if necessary the resolver must load the package
 * and register it.
 *
 */
typedef KlObject*(*klresolver)(
		/*
		 * The full name of the object to resolve.
		 */
		const char* fullname,
		/*
		 * The package that contains the function or the type with the function requesting the resolve.
		 */
		KLPackage* callerPackage,
		/*
		 * If present, is the type that contains the function requesting the resolve.
		 */
		KLType* callerType,
		/*
		 * The function requesting the resolve
		 */
		KLFunction* callerFunction,
		/*
		 * if is zero then the request include a resolve for the type, else the resolve look only for package.
		 */
		kbyte mode);

CAPI
/*
 * Change the resolvers used by klruntime to search packages, if a null is passed that resolver is not changed.
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