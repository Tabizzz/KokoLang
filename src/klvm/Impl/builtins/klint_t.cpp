#include "global.h"

kl_int temp_int = {
	KlObject{
		nullptr,
		1,
		KLOBJ_FLAG_IGNORE_REF
	},
	0
};

KLType *klint_t = nullptr;

KLType *klreg_t = nullptr;

static void kint_init(KlObject *obj) {
	auto ptr = KLCAST(kl_int, obj);
	ptr->value = 0;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantFunctionResult"

static int8_t kint_comparer(KlObject *x, KlObject *y) {
	kint first = KASINT(x);
	kint second = 0;
	if (y) {
		if (y->type == klint_t) {
			second = KASINT(y);
		} else if (y->type->toInt) {
			auto frees = y->type->toInt(y);
			second = KASINT(frees);
			klDeref(frees);
		}
	}

	if (first < second) {
		return 1;
	} else if (second < first) {
		return -1;
	}
	return 0;
}

#pragma clang diagnostic pop

static int8_t kint_equal(KlObject *x, KlObject *y) {
	if (y) {
		if (y->type == klint_t) {
			return KASINT(x) == KASINT(y);
		} else if (y->type->toInt) {
			auto frees = y->type->toInt(y);
			auto dev = KASINT(frees);
			klDeref(frees);
			return KASINT(x) == dev;
		}
	}

	return KASINT(x) == 0;
}

static KlObject *kint_clone(KlObject *base) {
	return KLINT(KASINT(base));
}

static void kint_copy(KlObject *a, KlObject *b) {
	KASINT(b) = KASINT(a);
}

static KlObject *klint_tostr(KlObject *base) {
	auto val = KASINT(base);
	return KLSTR(to_string(val));
}

static KlObject *klint_toflt(KlObject *base) {
	auto val = KASINT(base);
	return KLFLOAT(val);
}

static KlObject *klint_tobit(KlObject *base) {
	auto val = KASINT(base);
	return KLBOOL(val);
}

static void kint_add(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		if (second->type == klfloat_t) {
			temp_float.value = KASINT(first);
			klfloat_t->opAdd(KLWRAP(&temp_float), second, target);
			return;
		}
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->toInt) {
			auto frees = second->type->toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int.value = x + y;
		klCopy(KLWRAP(&temp_int), target);
	} else {
		klCopy(first, target);
	}
}

static void kint_sub(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		if (second->type == klfloat_t) {
			temp_float.value = KASINT(first);
			klfloat_t->opSub(KLWRAP(&temp_float), second, target);
			return;
		}
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->toInt) {
			auto frees = second->type->toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int.value = x - y;
		klCopy(KLWRAP(&temp_int), target);
	} else {
		klCopy(first, target);
	}
}

static void kint_mul(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		if (second->type == klfloat_t) {
			klfloat_t->opMul(second, first, target);
			return;
		}
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->toInt) {
			auto frees = second->type->toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int.value = x * y;
		klCopy(KLWRAP(&temp_int), target);
	} else {
		temp_int.value = 0;
		klCopy(KLWRAP(&temp_int), target);
	}
}

static void kint_div(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		if (second->type == klfloat_t) {
			temp_float.value = KASINT(first);
			klfloat_t->opDiv(KLWRAP(&temp_float), second, target);
			return;
		}
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->toInt) {
			auto frees = second->type->toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		// throw before hardware error
		if (y == 0) throw logic_error("Division by 0");
		temp_int.value = x / y;
		klCopy(KLWRAP(&temp_int), target);
	} else {
		throw logic_error("Division by 0");
	}
}

static void kint_mod(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		if (second->type == klfloat_t) {
			temp_float.value = KASINT(first);
			klfloat_t->opMod(KLWRAP(&temp_float), second, target);
			return;
		}
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->toInt) {
			auto frees = second->type->toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		// throw before hardware error
		if (y == 0) throw logic_error("Division by 0");
		temp_int.value = x % y;
		klCopy(KLWRAP(&temp_int), target);
	} else {
		throw logic_error("Division by 0");
	}
}

void global_klint_t() {
	klint_t = new KLType {
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
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
		kint_copy,
		0
	};

	KLTYPE_METADATA(klint_t)

	klreg_t = new KLType{
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"reg",
		0,
		sizeof(kl_int),
		kint_init,
		REP(1 , 6, nullptr)
		kint_copy
	};

	KLTYPE_METADATA(klreg_t)

	temp_int.klbase.type = klint_t;
}