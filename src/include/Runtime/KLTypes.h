#pragma once

#include "Runtime/KLObject.h"

typedef struct {
	KLOBJECTHEAD
	int value;
} kl_int;

static KlType kltype_int = {
	.name = "int",
	.size = sizeof (kl_int)
};

typedef struct {
	KLOBJECTHEAD
	double value;
} kl_float;

static KlType kltype_float = {
	.name = "float",
	.size = sizeof (kl_float)
};

typedef struct {
	KLOBJECTHEAD
	const char* value;
	int size;
} kl_string;

static KlType kltype_string = {
		.name = "string",
		.size = sizeof (kl_string)
};
