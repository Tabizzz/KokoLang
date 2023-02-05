#include "KokoLangInternal.h"
#include "klapi_types.h"
#include <cassert>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"

KlObject* klself_return(KlObject* base) { return base; }

#pragma region temps

// static object used to temporary store the value of the int operations
static kl_int temp_int = {
	KlObject{
			&klBType_Int,
			1
	},
	0
};
static kl_float temp_float = {
	KlObject{
			&klBType_Float,
			1
	},
	0
};

#pragma endregion

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
			auto frees = y->type->toInt(y);
			second = KASINT(frees);
			klDeref(frees);
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
		auto x = KASINT(first);
		int64_t y = 0;
		if (second->type == &klBType_Int) {
			y = KASINT(second);
		} else if (second->type->toInt) {
			auto frees = second->type->toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int.value = x + y;
		regop(KLWRAP(&temp_int), target);
	} else {
		regop(first, target);
	}
}

void kint_sub(KlObject* first, KlObject* second, KlObject** target, klRegOp regop)
{
	if(second) {
		if(second->type == &klBType_Float) {
			temp_float.value = KASINT(first);
			klBType_Float.opSub(KLWRAP(&temp_float), second, target, regop);
			return;
		}
		auto x = KASINT(first);
		int64_t y = 0;
		if (second->type == &klBType_Int) {
			y = KASINT(second);
		} else if (second->type->toInt) {
			auto frees = second->type->toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int.value = x + y;
		regop(KLWRAP(&temp_int), target);
	} else {
		regop(first, target);
	}
}

void kint_mul(KlObject* first, KlObject* second, KlObject** target, klRegOp regop)
{
	if(second) {
		if(second->type == &klBType_Float) {
			klBType_Float.opSub(second, first, target, regop);
			return;
		}
		auto x = KASINT(first);
		int64_t y = 0;
		if (second->type == &klBType_Int) {
			y = KASINT(second);
		} else if (second->type->toInt) {
			auto frees = second->type->toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int.value = x * y;
		regop(KLWRAP(&temp_int), target);
	} else {
		regop(nullptr, target);
	}
}

void kint_div(KlObject* first, KlObject* second, KlObject** target, klRegOp regop)
{
	if(second) {
		if(second->type == &klBType_Float) {
			temp_float.value = KASINT(first);
			klBType_Float.opSub(KLWRAP(&temp_float), second, target, regop);
			return;
		}
		auto x = KASINT(first);
		int64_t y = 0;
		if (second->type == &klBType_Int) {
			y = KASINT(second);
		} else if (second->type->toInt) {
			auto frees = second->type->toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int.value = x / y;
		regop(KLWRAP(&temp_int), target);
	} else {
		regop(nullptr, target);
	}
}

void kint_mod(KlObject* first, KlObject* second, KlObject** target, klRegOp regop)
{
	if(second) {
		auto x = KASINT(first);
		int64_t y = 0;
		if (second->type == &klBType_Int) {
			y = KASINT(second);
		} else if (second->type->toInt) {
			auto frees = second->type->toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int.value = x % y;
		regop(KLWRAP(&temp_int), target);
	} else {
		regop(nullptr, target);
	}
}

KLType klBType_Int =
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
		kint_sub,
		kint_mul,
		kint_div,
		kint_mod,
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
			auto frees = y->type->toFloat(y);
			second = KASFLOAT(frees);
			klDeref(frees);
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

void kfloat_add(KlObject* first, KlObject* second, KlObject** target, klRegOp regop)
{
	if(second) {
		auto x = KASFLOAT(first);
		double y = 0;
		if (second->type == &klBType_Float) {
			y = KASFLOAT(second);
		} else if (second->type == &klBType_Int) {
			y = KASINT(second);
		}  else if (second->type->toFloat) {
			auto frees = second->type->toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		temp_float.value = x + y;
		regop(KLWRAP(&temp_float), target);
	} else {
		regop(first, target);
	}
}

void kfloat_sub(KlObject* first, KlObject* second, KlObject** target, klRegOp regop)
{
	if(second) {
		auto x = KASFLOAT(first);
		double y = 0;
		if (second->type == &klBType_Float) {
			y = KASFLOAT(second);
		} else if (second->type == &klBType_Int) {
			y = KASINT(second);
		}  else if (second->type->toFloat) {
			auto frees = second->type->toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		temp_float.value = x - y;
		regop(KLWRAP(&temp_float), target);
	} else {
		regop(first, target);
	}
}

void kfloat_mul(KlObject* first, KlObject* second, KlObject** target, klRegOp regop)
{
	if(second) {
		auto x = KASFLOAT(first);
		double y = 0;
		if (second->type == &klBType_Float) {
			y = KASFLOAT(second);
		} else if (second->type == &klBType_Int) {
			y = KASINT(second);
		}  else if (second->type->toFloat) {
			auto frees = second->type->toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		temp_float.value = x * y;
		regop(KLWRAP(&temp_float), target);
	} else {
		regop(nullptr, target);
	}
}

void kfloat_div(KlObject* first, KlObject* second, KlObject** target, klRegOp regop)
{
	if(second) {
		auto x = KASFLOAT(first);
		double y = 0;
		if (second->type == &klBType_Float) {
			y = KASFLOAT(second);
		} else if (second->type == &klBType_Int) {
			y = KASINT(second);
		}  else if (second->type->toFloat) {
			auto frees = second->type->toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		temp_float.value = x / y;
		regop(KLWRAP(&temp_float), target);
	} else {
		regop(nullptr, target);
	}
}

KLType klBType_Float =
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
		kfloat_add,
		kfloat_sub,
		kfloat_mul,
		kfloat_div,
		nullptr,
		kfloat_clone,
		kfloat_copy,
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

KLType klBType_Bool =
{
		KlObject(),
		"bit",
		0,
		sizeof(kl_bool),
		kbool_init,
};

KLType klBType_String =
{
		KlObject(),
		"str",
		0,
		sizeof(kl_string),
		kstring_init,
		nullptr,
		kstring_end
};

KLType klBType_Ptr =
{
		KlObject(),
		"ptr",
		0,
		sizeof (kl_ptr),
		kptr_init,
};

KLType klBType_OPtr =
{
		KlObject(),
		"optr",
		0,
		sizeof (kl_optr),
		koptr_init,
};

KLType klBType_Arr =
{
		KlObject(),
		"arr",
		0,
		sizeof (kl_arr),
		karr_init,
};

KLType klBType_Type =
{
	KlObject(),
	"type",
	0,
	sizeof (KLType),
};

KLType klBType_Reg =
{
		KlObject(),
		"reg",
		0,
		sizeof (kl_int),
		kint_init,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		kint_copy,
};

CAPI KlObject *klNewVar(KLType *type, KlObject *args, ...) {
	return nullptr;
}

CAPI KlObject *klNew(KLType *type, KlObject **args, int argc) {
	return nullptr;
}

CAPI KlObject *klIns(KLType *type) {
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