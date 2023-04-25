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
KLPackage* klCreateIntermediatePackage(const char* code);

KLIBAPI
KLPackage* klLoadScriptFile(const char* filename);

KLIBAPI
KLPackage* klCreateScriptPackage(const char* code);

KLIBAPI
kbyte klCompileIntermediateFile(const char* filename, const char* output);

KLIBAPI
kbyte klCompileIntermediateStrings(const char* code, const char* output);

KLIBAPI
kbyte klCompileScriptFile(const char* filename, const char* output);

KLIBAPI
kbyte klCompileScriptString(const char* code, const char* output);

KLIBAPI
kbyte klConvertScriptToIntermediateFile(const char* filename, const char* output);

KLIBAPI
kbyte klConvertScriptToIntermediateString(const char* code, const char* output);