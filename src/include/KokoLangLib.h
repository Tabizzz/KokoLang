#pragma once

#include "KokoLang.h"
#include "antlr4-runtime.h"

#if defined _WIN32 || defined __CYGWIN__
#ifdef KokoLangLib_EXPORTS
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

KLIBAPI KLProgram* klCreateProgramFromFile(const char* filename);

KLIBAPI KLProgram* klCreateProgramFromString(const char* code);