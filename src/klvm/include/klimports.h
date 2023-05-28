#pragma once

/*
 * std headers used in kokolang
 */

#include <string>
#include <any>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <stack>
#include <bitset>
#include <cmath>

#if defined _WIN32 || defined __CYGWIN__
#define WIN
#ifdef klvm_EXPORTS
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

typedef uint8_t kbyte;
typedef uint16_t kshort;

typedef void* kptr;

typedef uint32_t ksize;

#ifdef TARGET_32B
typedef int32_t kint;
typedef float kfloat;
#else
typedef int64_t kint;
typedef double kfloat;
#endif