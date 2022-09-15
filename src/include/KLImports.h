#pragma once

#include <string>
#include <any>
#include <vector>
#if defined _WIN32 || defined __CYGWIN__
#ifdef KLNO_EXPORT
#define CPPAPI
#else
#define CPPAPI __declspec(dllexport)
#endif
#else
#ifdef __GNUC__
#define CPPAPI  __attribute__((__visibility__("default")))
#else
#define CPPAPI
#endif
#endif