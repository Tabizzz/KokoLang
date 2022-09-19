#pragma once

#include "Runtime/KLObject.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	KLOBJECTHEAD
	int value;
} kl_int;

typedef struct {
	KLOBJECTHEAD
	double value;
} kl_float;

typedef struct {
	KLOBJECTHEAD
	char* value;
	size_t size;
} kl_string;

typedef struct {
	KLOBJECTHEAD
	bool value;
} kl_bool;

typedef struct {
	KLOBJECTHEAD
	void* value;
} kl_ptr;

typedef struct {
	KLOBJECTHEAD
	KlObject** value;
} kl_optr;

typedef struct{
	KLOBJECTHEAD
	unsigned int dimension;
	unsigned int size;
	KlObject** content;
} kl_arr;
#ifdef __cplusplus
};
#endif