#pragma once

#include "Runtime/KLObject.h"

#ifdef __cplusplus
extern "C" {
#endif
#define KLTYPE_int klBType_Int()
#define KLType_float klBType_Float()
#define KLtype_string klBType_String()
#define KLtype_bool klBType_Bool()

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
	const char *value;
	int size;
} kl_string;

typedef struct {
	KLOBJECTHEAD
	bool value;
} kl_bool;

#ifdef __cplusplus
};
#endif