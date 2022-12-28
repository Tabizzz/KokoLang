﻿#pragma once

#include "DataTypes/KLObject.h"

/*
 * builtin kl types
 */

#ifdef __cplusplus
extern "C" {
#endif


typedef struct kl_int {
	KLOBJECTHEAD
	int64_t value;
} kl_int;

typedef struct kl_float {
	KLOBJECTHEAD
	double_t value;
} kl_float;

typedef struct kl_string {
	KLOBJECTHEAD
	char* value;
	size_t size;
} kl_string;

typedef struct kl_bool {
	KLOBJECTHEAD
	bool value;
} kl_bool;

typedef struct kl_ptr {
	KLOBJECTHEAD
	void* value;
} kl_ptr;

typedef struct kl_optr {
	KLOBJECTHEAD
	KlObject** value;
} kl_optr;

typedef struct kl_arr {
	KLOBJECTHEAD
	unsigned int dimension;
	unsigned int size;
	KlObject** content;
} kl_arr;
#ifdef __cplusplus
};
#endif