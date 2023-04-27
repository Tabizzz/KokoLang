#pragma once

KLIBAPI
/**
 * @brief Load a package from a file in Intermediate Language format.
 *
 * Loads a file in kil format, parse the code and create a package.
 * The package is not built.
 */
KLPackage* klLoadIntermediateFile(const char* filename);

KLIBAPI
/**
 * @brief Create a new package from a string in Intermediate Language format.
 *
 * @param code A null terminated string containing the code to parse.
 * @return A package parsed from the code, the package is not built.
 */
KLPackage* klCreateIntermediatePackage(const char* code);

KLIBAPI
/**
 * @brief Load a package from a file in Script format.
 *
 * Loads a file in ks format, parse the code and create a package.
 * The package is not built.
 */
KLPackage* klLoadScriptFile(const char* filename);

KLIBAPI
/**
 * @brief Create a new package from a string in Script format.
 *
 * @param code A null terminated string containing the code to parse.
 * @return A package parsed from the code, the package is not built.
 */
KLPackage* klCreateScriptPackage(const char* code);