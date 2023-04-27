#pragma once

#include "KokoLang.h"
#include "antlr4-runtime.h"
#include "KokoLangLexer.h"
#include "KokoLangParser.h"

#if defined _WIN32 || defined __CYGWIN__
#ifdef kilasm_EXPORTS
#define LIBEXPORT __declspec(dllexport)
#elif defined KOKOLANGLIB_STATIC
#define LIBEXPORT 
#else
#define LIBEXPORT __declspec(dllimport)
#endif
#else
#define LIBEXPORT EXPORT
#endif

#define KLIBAPI EXTERN LIBEXPORT

#define SWITCH_TYPE(x, y, z)	\
y.inscount--;			\
z.inscount++;			\
x->type = &z;

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

KLIBAPI
/**
 * @brief Convert a Intermediate Language file into a Binary file.
 *
 * @param filename A file with Intermediate Language code.
 * @param output Path of the file to write the output.
 * @return 0 on failure, 1 on success.
 */
kbyte klCompileIntermediateFile(const char* filename, const char* output);
