#include "KokoLangInternal.h"

#define BUILTIN_TYPE(Name, str, base) static KlType Name = {.name = str, .size = sizeof(base),

void kint_initializer(KlObject* obj) {
	auto ptr = KLCAST(kl_int, obj);
	ptr->value = 0;
}

void kfloat_initializer(KlObject* obj) {
	auto ptr = KLCAST(kl_float, obj);
	ptr->value = 0;
}

void kbool_initializer(KlObject* obj) {
	auto ptr = KLCAST(kl_bool, obj);
	ptr->value = false;
}

void kstring_initializer(KlObject* obj) {
	auto ptr = KLCAST(kl_string, obj);
	ptr->value = nullptr;
	ptr->size = 0;
}

BUILTIN_TYPE(kltype_int, "int", kl_int)
	.initializer = kint_initializer
};
BUILTIN_TYPE(kltype_float, "float", kl_float)
	.initializer = kfloat_initializer
};
BUILTIN_TYPE(kltype_string, "string", kl_string)
	.initializer = kstring_initializer
};
BUILTIN_TYPE(kltype_bool, "bool", kl_bool)
	.initializer = kbool_initializer
};

KLAPI KlType* klBType_Int() {
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

KLAPI KlObject *klNew(KlType *type) {
	auto size = type->size;
	auto space = (KlObject*)malloc(size);
	space->type = type;
	space->refs = 1;
	type->inscount++;
	KLINVOKE(type->initializer)(space);
	// call constructor
	return space;
}

KLAPI void klDeref(KlObject* object) {
	assert(object->refs > 0);
	object->refs--;
	object->type->inscount--;
	if(object->refs == 0)
	{
		// call destructor
		// call finalize
		free(object);
	}
}
