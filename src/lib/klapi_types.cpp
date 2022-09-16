#include "KokoLangInternal.h"
#include "klapi_types.h"
#include <cassert>

#define BUILTIN_TYPE(Name, str, base) static KlType Name = { str, 0, sizeof(base),

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

void kstring_finalizer(KlObject* obj) {
	auto ptr = KLCAST(kl_string, obj);
	delete[] ptr->value;
}

BUILTIN_TYPE(kltype_int, "int", kl_int)
	kint_initializer,
	nullptr
};
BUILTIN_TYPE(kltype_float, "float", kl_float)
	kfloat_initializer,
	nullptr
};
BUILTIN_TYPE(kltype_string, "string", kl_string)
	kstring_initializer,
	kstring_finalizer
};
BUILTIN_TYPE(kltype_bool, "bool", kl_bool)
	kbool_initializer,
	nullptr
};

CAPI KlType* klBType_Int() {
	return &kltype_int;
}

CAPI KlType* klBType_Float() {
	return &kltype_float;
}

CAPI KlType* klBType_Bool() {
	return &kltype_bool;
}

CAPI KlType* klBType_String() {
	return &kltype_string;
}

CAPI KlObject *klNew(KlType *type) {
	auto size = type->size;
	auto space = (KlObject*)malloc(size);
	space->type = type;
	space->refs = 1;
	type->inscount++;
	KLINVOKE(type->initializer)(space);
	// call constructor
	return space;
}

CAPI void klDeref(KlObject* object) {
	assert(object->refs > 0);
	assert(object->type->inscount > 0);
	object->refs--;
	object->type->inscount--;
	if(object->refs == 0)
	{
		// call destructor
		KLINVOKE(object->type->finalizer)(object);
		free(object);
	}
}

CAPI void klRef(KlObject *object) {
	object->refs++;
}
