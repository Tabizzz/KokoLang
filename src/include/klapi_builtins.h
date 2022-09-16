/*
* This file contain utility methods and macros for creating builtin types instances
*/
#pragma once
#include "klapi_types.h"
#include "Runtime/KLTypes.h"

#define KLType_int klBType_Int()
#define KLType_float klBType_Float()
#define KLType_string klBType_String()
#define KLType_bool klBType_Bool()

#define KLINT(x) klBuiltinInt(x)
#define KINT(x) KLCAST(kl_int, KLINT(x))
#define KLFLOAT(x) klBuiltinFloat(x)
#define KFLOAT(x) KLCAST(kl_float , KLFLOAT(x))
#define KLBOOL(x) klBuiltinBool(x)
#define KBOOL(x) KLCAST(kl_bool, KLBOOL(x))
#ifdef __cplusplus
#define KLSTR(x) klBuiltinString(x)
#else
#define KLSTR(x) klBuiltinString_c(x)
#endif
#define KSTR(x) KLCAST(kl_string, KLSTR(x))

CAPI KlObject* klBuiltinInt(int val);
CAPI KlObject* klBuiltinFloat(double val);
CAPI KlObject* klBuiltinBool(bool val);
CAPI KlObject* klBuiltinString_c(const char* val);
CPPAPI KlObject* klBuiltinString(const string& val);