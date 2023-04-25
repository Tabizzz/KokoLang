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

KLIBAPI KLPackage* klCreateProgramFromFile(const char* filename);

KLIBAPI KLPackage* klCreateProgramFromString(const char* code);