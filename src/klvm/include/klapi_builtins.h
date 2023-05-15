/*
* This file contain utility methods and macros for creating builtin types instances
*/
#pragma once
#include "DataTypes/KLObject.h"

/**
 * Macro for create a new kl_int as KLObject* from a native int
 */
#define KLINT(x) klBuiltinInt(x)
/**
 * Macro for create a new kl_int from a native int
 */
#define KINT(x) KLCAST(kl_int, KLINT(x))
/**
 * Macro for access to the internal value of a kl_int
 */
#define KASINT(x) KLCAST(kl_int, x)->value
#define KLFLOAT(x) klBuiltinFloat(x)
#define KFLOAT(x) KLCAST(kl_float , KLFLOAT(x))
#define KASFLOAT(x) KLCAST(kl_float, x)->value
#define KLBOOL(x) klBuiltinBool(x)
#define KBOOL(x) KLCAST(kl_bool, KLBOOL(x))
#define KASBOOL(x) KLCAST(kl_bool, x)->value
#define KLSTR(x) klBuiltinString(x)
#define KSTR(x) KLCAST(kl_string, KLSTR(x))
#define KASSTR(x) KLCAST(kl_string, x)->value
#define KASSTRSIZE(x) KLCAST(kl_string, x)->size
#define KSTRING(x) std::string(KASSTR(x), KASSTRSIZE(x))
#define KLPTR(x) klBuiltinPtr(x)
#define KPTR(x) KLCAST(kl_ptr, KLPTR(x))
#define KASPTR(x) KLCAST(kl_ptr, x)->value
#define KLARR(x) klBuiltinArr(x)
#define KARR(x) KLCAST(kl_arr, KLARR(x))
#define KASARR(x) KLCAST(KlObject*, KLCAST(kl_sptr, x)->value)
#define KASARRSIZE(x) KLCAST(kl_sptr, x)->size

CAPI
/**
 * @brief Creates an int instance.
 *
 * @param val The native value that will be used on the object.
 *
 * @return A new kokolang object with the same value as val.
 */
KlObject* klBuiltinInt(kint val);

CAPI
/**
 * @brief Creates a float instance.
 *
 * @param val The native value that will be used on the object.
 *
 * @return A new kokolang object with the same value as val.
 */
KlObject* klBuiltinFloat(kfloat val);

CAPI
/**
 * @brief Creates a bool instance.
 *
 * @param val The native value that will be used on the object.
 *
 * @return A new kokolang object with the same value as val.
 */
KlObject* klBuiltinBool(char val);

CPPAPI
/**
 * @brief Creates new string instance from a c++ string.
 *
 * @param val The C++ string.
 *
 * @return A kl_string with the same content as val.
 */
KlObject* klBuiltinString(const std::string& val);

CAPI
/**
 * @brief Creates new ptr instance
 *
 * @param val native pointer to wrap.
 *
 * @return A ptr object wrapping val.
 */
KlObject* klBuiltinPtr(void* val);

CAPI
/**
 * @brief Creates new array instance with specific size.
 *
 * @param size The number of elements in the array.
 *
 * @return A new array.
 */
KlObject* klBuiltinArr(size_t size);

CAPI
void klDefaultInitializer(KlObject* obj);

CAPI
void klDefaultFinalizer(KlObject* obj);

CAPI KlObject* klDefaultConstructor();