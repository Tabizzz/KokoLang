#pragma once

/*
 * builtin kl types
 */
#include "DataTypes/KLObject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Kokolang's builtin int structure.
 *
 * A int is always a signed integer of 64 bits.
 * The range allowed for an int value is from -9223372036854775808 to 9223372036854775807.
 */
struct CPPAPI kl_int {
	KLOBJECTHEAD
	/**
	 * @brief Internal native value of the int.
	 */
	kint value;
};

/**
 * @brief Kokolang's builtin float structure.
 *
 * A float is always a double precision floating point number.
 * The range allowed for a float value is 1.7E +/- 308 (15 digits).
 */
struct CPPAPI kl_float {
	KLOBJECTHEAD
	/**
	 * @brief Internal native value of the float.
	 */
	kfloat value;
};

/**
 * @brief Kokolang's builtin string structure.
 *
 * A string value can hold any text, kl_string dont end in a null terminate char, instead it always knows its size.
 */
struct CPPAPI kl_string {
	KLOBJECTHEAD
	/**
	 * @brief The native array with the chars of the string.
	 *
	 * Strings are immutable and cannot be modified, that is, when you concatenate one string to another a new string is created and the
	 * originals are unmodified.
	 */
	const char *value;
	/**
	 * @brief The size of the string.
	 *
	 * The size represent the number of bytes allocated in the native char array, dont is the number of utf-8 chars in the string.
	 * For example: "a" has a size of 1 while "💀" has a size of 4.
	 */
	ksize size;
	/**
	 * @brief Reserved value.
	 */
	ksize reserved; // in future use as hash or as real count.
};

/**
 * @brief Kokolang's builtin bool structure.
 *
 * A bool can only be true or false, cannon be copied or cloned and has the flag KLTYPE_FLAG_NOINSTANCE, that is, is not possible to create
 * custom instances of bool type, you must use the two values given by the the runtime with the the function klBuiltinBool.
 *
 * @see KLTYPE_FLAG_NOINSTANCE
 * @see klBuiltinBool
 */
struct CPPAPI kl_bool {
	KLOBJECTHEAD
	/**
	 * @brief The value of the bool, is 0 or 1.
	 *
	 * This value is immutable and must be only read.
	 */
	const char value;
};

/**
 * @brief Kokolang's builtin pointer structure.
 *
 * This struct holds a pointer to any location and is used in kokolang to represent arbitrary memory regions.
 */
struct CPPAPI kl_ptr {
	KLOBJECTHEAD
	/**
	 * @brief The native pointer value.
	 */
	kptr value;
};

/**
 *  @brief Kokolang's builtin array structure.
 *
 * An array has a fixed size.
 */
struct CPPAPI kl_sptr {
	KLOBJECTHEAD
	/**
	 * @brief The native array containing the objects.
	 */
	kptr value;
	/**
	 * @brief The number of elements of the array.
	 */
	ksize size;
};

#ifdef __cplusplus
}
#endif