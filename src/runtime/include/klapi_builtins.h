/*
* This file contain utility methods and macros for creating builtin types instances
*/
#pragma once
#include "klapi_types.h"
#include "kltypedef.h"

#define KLINT(x) klBuiltinInt(x)
#define KINT(x) KLCAST(kl_int, KLINT(x))
#define KASINT(x) KLCAST(kl_int, x)->value
#define KLFLOAT(x) klBuiltinFloat(x)
#define KFLOAT(x) KLCAST(kl_float , KLFLOAT(x))
#define KASFLOAT(x) KLCAST(kl_float, x)->value
#define KLBOOL(x) klBuiltinBool(x)
#define KBOOL(x) KLCAST(kl_bool, KLBOOL(x))
#define KASBOOL(x) KLCAST(kl_bool, x)->value
#ifdef __cplusplus
#define KLSTR(x) klBuiltinString(x)
#else
#define KLSTR(x) klBuiltinString_c(x)
#endif
#define KSTR(x) KLCAST(kl_string, KLSTR(x))
#define KLPTR(x) klBuiltinPtr(x)
#define KPTR(x) KLCAST(kl_ptr, KLPTR(x))
#define KLOPTR(x) klBuiltinOPtr(x)
#define KOPTR(x) KLCAST(kl_optr, KLOPTR(x))
#define KLARR(x, y) klBuiltinArr(x, y)
#define KARR(x, y) KLCAST(kl_arr, KLARR(x, y))

/*
 * Creates new int instance
 */
CAPI KlObject* klBuiltinInt(int64_t val);

/*
 * Creates new float instance
 */
CAPI KlObject* klBuiltinFloat(double_t val);

/*
 * Creates bool int instance
 */
CAPI KlObject* klBuiltinBool(bool val);

/*
 * Creates new string instance from a c string
 */
CAPI KlObject* klBuiltinString_c(const char* val);

/*
 * Creates new string instance from a c++ string
 */
CPPAPI KlObject* klBuiltinString(const std::string& val);

/*
 * Creates new ptr instance
 */
CAPI KlObject* klBuiltinPtr(void* val);

/*
 * Creates new optr instance
 */
CAPI KlObject* klBuiltinOPtr(KlObject** val);

/*
 * Creates new array instance, all internal index are null.
 */
CAPI KlObject* klBuiltinArr(int size, int dimensions = 0);