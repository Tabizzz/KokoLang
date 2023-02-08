#pragma once

#include <cmath>
#include "DataTypes/KLObject.h"

/*
 * builtin kl types
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Builtin int structure.
 */
typedef struct kl_int {
	KLOBJECTHEAD
	int64_t value;
} kl_int;

/*
 * Builtin float structure.
 */
typedef struct kl_float {
	KLOBJECTHEAD
	double_t value;
} kl_float;

/*
 * Builtin string structure.
 */
typedef struct kl_string {
	KLOBJECTHEAD
	char* value;
	size_t size;
} kl_string;

/*
 * Builtin bool structure.
 */
typedef struct kl_bool {
	KLOBJECTHEAD
	const char value;
} kl_bool;

/*
 * Builtin pointer structure.
 */
typedef struct kl_ptr {
	KLOBJECTHEAD
	void* value;
} kl_ptr;

/*
 * Builtin object pointer structure.
 */
typedef struct kl_optr {
	KLOBJECTHEAD
	KlObject** value;
} kl_optr;

/*
 * Builtin array structure.
 */
typedef struct kl_arr {
	KLOBJECTHEAD
	unsigned int dimension;
	unsigned int size;
	KlObject** content;
} kl_arr;
#ifdef __cplusplus
};
#endif