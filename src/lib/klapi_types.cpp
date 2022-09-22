#include "KokoLangInternal.h"
#include "klapi_types.h"
#include <cassert>
#include <iostream>

void kint_init(KlObject* obj) {
	auto ptr = KLCAST(kl_int, obj);
	ptr->value = 0;
}

int kint_comparer(KlObject* x, KlObject* y) {
	int first = KASINT(x);
	int second;
	if(y->type == &klBType_Int) {
		second = KASINT(y);
	} else {
		second = KASINT(y->type->toInt(y));
	}

	if(first < second) {
		return 1;
	}
	else if( second < first) {
		return -1;
	}
	return 0;
}

void kfloat_init(KlObject* obj) {
	auto ptr = KLCAST(kl_float, obj);
	ptr->value = 0;
}

void kbool_init(KlObject* obj) {
	auto ptr = KLCAST(kl_bool, obj);
	ptr->value = false;
}

void kstring_init(KlObject* obj) {
	auto ptr = KLCAST(kl_string, obj);
	ptr->value = nullptr;
	ptr->size = 0;
}

void kstring_end(KlObject* obj) {
	auto ptr = KLCAST(kl_string, obj);
	delete[] ptr->value;
}

void kptr_init(KlObject* obj) {
	auto ptr = KLCAST(kl_ptr, obj);
	ptr->value = nullptr;
}

void koptr_init(KlObject* obj) {
	auto ptr = KLCAST(kl_optr, obj);
	ptr->value = nullptr;
}

void karr_init(KlObject* obj) {
	auto ptr = KLCAST(kl_arr, obj);
	ptr->dimension = 0;
	ptr->size = 0;
	ptr->content = nullptr;
}

KlType klBType_Int =
{
		KlObject(),
		"int",
		0,
		sizeof(kl_int),
		kint_init,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		kint_comparer
};

KlType klBType_Float =
{
		KlObject(),
		"fltt",
		0,
		sizeof(kl_float),
		kfloat_init,
		nullptr,
		nullptr
};

KlType klBType_Bool =
{
		KlObject(),
		"bit",
		0,
		sizeof(kl_bool),
		kbool_init,
		nullptr,
		nullptr
};

KlType klBType_String =
{
		KlObject(),
		"str",
		0,
		sizeof(kl_string),
		kstring_init,
		nullptr,
		kstring_end
};

KlType klBType_Ptr =
{
		KlObject(),
		"ptr",
		0,
		sizeof (kl_ptr),
		kptr_init,
		nullptr,
		nullptr
};

KlType klBType_OPtr =
{
		KlObject(),
		"optr",
		0,
		sizeof (kl_optr),
		koptr_init,
		nullptr,
		nullptr
};

KlType klBType_Arr =
{
		KlObject(),
		"arr",
		0,
		sizeof (kl_arr),
		karr_init,
		nullptr,
		nullptr
};

KlType klBType_Type =
{
	KlObject(),
	"type",
	0,
	sizeof (KlType),
	nullptr,
	nullptr,
	nullptr
};

KlObject *klNewVar(KlType *type, KlObject *args, ...) {
	return nullptr;
}

KlObject *klNew(KlType *type, KlObject **args, int argc) {
	return nullptr;
}

CAPI KlObject *klIns(KlType *type) {
	auto size = type->size;
	auto space = (KlObject*)malloc(size);
	space->type = type;
	space->refs = 1;
	type->inscount++;
	// call the initializer
	KLINVOKE(type->initializer)(space);
	return space;
}

CAPI void klRef(KlObject *object) {
	if(object) object->refs++;
}

CAPI void klDeref(KlObject* object) {
	if(!object) return;
	assert(object->refs > 0);
	assert(object->type->inscount > 0);
	object->refs--;
	if(object->refs == 0) {
		klDestroy(object);
	}
}

CAPI void klDestroy(KlObject *object) {
	if(!object) return;
	object->type->inscount--;
	// call finalizer
	KLINVOKE(object->type->finalizer)(object);
	free(object);
}
