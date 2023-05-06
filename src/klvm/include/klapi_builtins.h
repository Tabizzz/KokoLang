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
#ifdef __cplusplus
#define KLSTR(x) klBuiltinString(x)
#else
#define KLSTR(x) klBuiltinString_c(x)
#endif
#define KSTR(x) KLCAST(kl_string, KLSTR(x))
#define KASSTR(x) KLCAST(kl_string, x)->value
#define KASSTRSIZE(x) KLCAST(kl_string, x)->size
#define KSTRING(x) std::string(KASSTR(x), KASSTRSIZE(x))
#define KLPTR(x) klBuiltinPtr(x)
#define KPTR(x) KLCAST(kl_ptr, KLPTR(x))
#define KLARR(x) klBuiltinArr(x)
#define KARR(x) KLCAST(kl_arr, KLARR(x))

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
 * @brief Creates new string instance from a c string
 *
 * This function exist to allow interop with languages that not support c++ strings.
 *
 * @param val A null terminating C string.
 *
 * @return A kl_string with the same content as val.
 */
inline KlObject* klBuiltinString_c(const char* val){
	return klBuiltinString(val);
}

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
 * @brief Creates new array instance with multiple dimensions.
 *
 * @param dimensions The number of dimensions on the array plus 1, that is, if you want to create a 0 dimensional array pass 1 as parameters.
 * @param sizes A array containing the sizes of each dimension on the array, the size of the array must be greater or equal than dimensions.
 *
 * @return A new array of arbitrary dimension and with the given sizes.
 */
KlObject* klBuiltinMultiArr(uint32_t dimensions, uint32_t* sizes);

CAPI
/**
 * @brief Creates new array instance with one dimension.
 *
 * @param size The number of elements in the array.
 *
 * @return A new array of dimension 0.
 */
inline KlObject* klBuiltinArr(uint32_t size)
{
	uint32_t sizes []{size};
	return klBuiltinMultiArr(1, sizes);
}