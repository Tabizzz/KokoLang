#pragma once

/*
 * std headers used in kokolang
 */

#include <string>
#include <any>
#include <vector>

#if defined _WIN32 || defined __CYGWIN__
#ifdef KokoLangRuntime_EXPORTS
#define EXPORT __declspec(dllexport)
#elif defined KOKOLANG_STATIC
#define EXPORT 
#else
#define EXPORT __declspec(dllimport)
#endif
#else
#ifdef __GNUC__
#define EXPORT  __attribute__((__visibility__("default")))
#else
#define EXPORT
#endif
#endif

#if defined __cplusplus
#define EXTERN extern "C"
#else
#include <stdarg.h>
#include <stdbool.h>
#define EXTERN extern
#endif

#define CAPI EXTERN EXPORT
#define CPPAPI EXPORT

typedef unsigned char kbyte;