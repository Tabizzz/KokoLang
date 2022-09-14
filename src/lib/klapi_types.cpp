#include "KokoLangInternal.h"

#define BUILTIN_TYPE(Name, str, base) static KlType Name = {.name = str, .size = sizeof(base) };

BUILTIN_TYPE(kltype_int, "int", kl_int)
BUILTIN_TYPE(kltype_float, "float", kl_float)
BUILTIN_TYPE(kltype_string, "string", kl_string)
BUILTIN_TYPE(kltype_bool, "bool", kl_bool)

KLAPI KlType* klBType_Int()
{
	return &kltype_int;
}

KLAPI KlType* klBType_Float() {
	return &kltype_float;
}

KLAPI KlType* klBType_Bool() {
	return &kltype_bool;
}

KLAPI KlType* klBType_String() {
	return &kltype_string;
}
