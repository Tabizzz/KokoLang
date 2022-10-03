#include "KokoLangInternal.h"
#include "klapi_types.h"
#include <cassert>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"

KlObject* klself_return(KlObject* base) { return base; }

#pragma region int
void kint_init(KlObject* obj) {
	auto ptr = KLCAST(kl_int, obj);
	ptr->value = 0;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantFunctionResult"
int8_t kint_comparer(KlObject* x, KlObject* y) {
	int64_t first = KASINT(x);
	int64_t second = 0;
	if(y) {
		if (y->type == &klBType_Int) {
			second = KASINT(y);
		} else if (y->type->toInt) {
			second = KASINT(y->type->toInt(y));
		}
	}

	if(first < second) {
		return 1;
	}
	else if(second < first) {
		return -1;
	}
	return 0;
}
#pragma clang diagnostic pop

int8_t kint_equal(KlObject* x, KlObject* y) {
	return KASINT(x) == KASINT(y);
}

KlObject* kint_clone(KlObject* base) {
	return KLINT(KASINT(base));
}

void kint_copy(KlObject* a, KlObject* b) {
	KASINT(b) = KASINT(a);
}

KlObject* klint_tostr(KlObject* base)
{
	auto val = KASINT(base);
	return KLSTR(to_string(val));
}

KlObject* klint_toflt(KlObject* base)
{
	auto val = KASINT(base);
	return KLFLOAT(val);
}

KlObject* klint_tobit(KlObject* base)
{
	auto val = KASINT(base);
	return KLBOOL(val);
}

void kint_add(KlObject* first, KlObject* second, KlObject** target, klRegOp regop)
{
	if(second) {
		if(second->type == &klBType_Float) {
			klBType_Float.opAdd(second, first, target, regop);
			return;
		}
		static kl_int temp = {
				KlObject{
						&klBType_Int,
						1
				},
				0
		};
		auto x = KASINT(first);
		int64_t y = 0;
		if (second->type == &klBType_Int) {
			y = KASINT(second);
		} else if (second->type->toInt) {
			y = KASINT(second->type->toInt(second));
		}
		temp.value = x + y;
		regop(KLWRAP(&temp), target);
	} else {
		regop(first, target);
	}
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
		klint_tostr,
		klself_return,
		klint_toflt,
		klint_tobit,
		nullptr,
		nullptr,
		kint_comparer,
		kint_equal,
		kint_add,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		kint_clone,
		kint_copy
};
#pragma endregion

#pragma region float

void kfloat_init(KlObject* obj) {
	auto ptr = KLCAST(kl_float, obj);
	ptr->value = 0;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantFunctionResult"
int8_t kfloat_comparer(KlObject* x, KlObject* y) {
	double first = KASFLOAT(x);
	double second = 0;
	if(y) {
		if (y->type == &klBType_Float) {
			second = KASFLOAT(y);
		} else if (y->type->toFloat) {
			second = KASFLOAT(y->type->toFloat(y));
		}
	}

	if(first < second) {
		return 1;
	}
	else if(second < first) {
		return -1;
	}
	return 0;
}
#pragma clang diagnostic pop

int8_t kfloat_equal(KlObject* x, KlObject* y) {
	return KASFLOAT(x) == KASFLOAT(y);
}

KlObject* kfloat_clone(KlObject* base) {
	return KLFLOAT(KASFLOAT(base));
}

void kfloat_copy(KlObject* a, KlObject* b) {
	KASFLOAT(b) = KASFLOAT(a);
}

KlObject* kfloat_tostr(KlObject* base)
{
	auto val = KASFLOAT(base);
	return KLSTR(to_string(val));
}

KlObject* kfloat_toint(KlObject* base)
{
	auto val = KASFLOAT(base);
	return KLINT((int64_t)val);
}

KlObject* kfloat_tobit(KlObject* base)
{
	auto val = KASFLOAT(base);
	return KLBOOL(val);
}

KlType klBType_Float =
{
		KlObject(),
		"flt",
		0,
		sizeof(kl_float),
		kfloat_init,
		nullptr,
		nullptr,
		kfloat_tostr,
		kfloat_toint,
		klself_return,
		kfloat_tobit,
		nullptr,
		nullptr,
		kfloat_comparer,
		kfloat_equal,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		kfloat_clone,
		kfloat_copy
};

#pragma endregion

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

KlType klBType_Bool =
{
		KlObject(),
		"bit",
		0,
		sizeof(kl_bool),
		kbool_init,
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
};

KlType klBType_OPtr =
{
		KlObject(),
		"optr",
		0,
		sizeof (kl_optr),
		koptr_init,
};

KlType klBType_Arr =
{
		KlObject(),
		"arr",
		0,
		sizeof (kl_arr),
		karr_init,
};

KlType klBType_Type =
{
	KlObject(),
	"type",
	0,
	sizeof (KlType),
};

KlType klBType_Reg =
{
		KlObject(),
		"reg",
		0,
		sizeof (kl_int),
		kint_init,
};

CAPI KlObject *klNewVar(KlType *type, KlObject *args, ...) {
	return nullptr;
}

CAPI KlObject *klNew(KlType *type, KlObject **args, int argc) {
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

#pragma clang diagnostic pop