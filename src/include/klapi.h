#pragma once
#ifndef KLAPI_H
#define KLAPI_H
#if defined _WIN32 || defined __CYGWIN__
#ifdef KLNO_EXPORT
#define API
#else
#define API __declspec(dllexport)
#endif
#else
#ifdef __GNUC__
#define API  __attribute__((__visibility__("default")))
#else
#define API
#endif
#endif

#if defined __cplusplus
#define EXTERN extern "C"
#else
#include <stdarg.h>
#include <stdbool.h>
#define EXTERN extern
#endif

#define KLAPI EXTERN API

#include "Runtime/KLRuntime.h"

KLAPI KLProgram* klCreateProgramFromFile(const char* filename);

KLAPI KLProgram* klCreateProgramFromString(const char* code);

KLAPI void DestroyAny(void* obj);

KLAPI void DestroyArray(void* obj);

#endif // !KLAPI_H

