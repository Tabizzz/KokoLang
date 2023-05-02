#include "global.h"

kl_int temp_int = {
	KlObject{
		nullptr,
		1,
		KLOBJ_FLAG_IGNORE_REF
	},
	0
};
kl_float aux_float = {
	KlObject{
		nullptr,
		1,
		KLOBJ_FLAG_IGNORE_REF
	},
	0
};

KLType *klint_t = nullptr;

void kint_init(KlObject *obj) {
	auto ptr = KLCAST(kl_int, obj);
	ptr->value = 0;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantFunctionResult"

int8_t kint_comparer(KlObject *x, KlObject *y) {
	int64_t first = KASINT(x);
	int64_t second = 0;
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

int8_t kint_equal(KlObject *x, KlObject *y) {
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

KlObject *kint_clone(KlObject *base) {
	return KLINT(KASINT(base));
}

void kint_copy(KlObject *a, KlObject *b) {
	KASINT(b) = KASINT(a);
}

KlObject *klint_tostr(KlObject *base) {
	auto val = KASINT(base);
	return KLSTR(to_string(val));
}

KlObject *klint_toflt(KlObject *base) {
	auto val = KASINT(base);
	return KLFLOAT(val);
}

KlObject *klint_tobit(KlObject *base) {
	auto val = KASINT(base);
	return KLBOOL(val);
}

void kint_add(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		if (second->type == klfloat_t) {
			aux_float.value = KASINT(first);
			klfloat_t->opAdd(KLWRAP(&aux_float), second, target);
			return;
		}
		auto x = KASINT(first);
		int64_t y = 0;
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

void kint_sub(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		if (second->type == klfloat_t) {
			aux_float.value = KASINT(first);
			klfloat_t->opSub(KLWRAP(&aux_float), second, target);
			return;
		}
		auto x = KASINT(first);
		int64_t y = 0;
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

void kint_mul(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		if (second->type == klfloat_t) {
			klfloat_t->opMul(second, first, target);
			return;
		}
		auto x = KASINT(first);
		int64_t y = 0;
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

void kint_div(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		if (second->type == klfloat_t) {
			aux_float.value = KASINT(first);
			klfloat_t->opDiv(KLWRAP(&aux_float), second, target);
			return;
		}
		auto x = KASINT(first);
		int64_t y = 0;
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

void kint_mod(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		if (second->type == klfloat_t) {
			aux_float.value = KASINT(first);
			klfloat_t->opMod(KLWRAP(&aux_float), second, target);
			return;
		}
		auto x = KASINT(first);
		int64_t y = 0;
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
		KlObject {
			.flags = KLOBJ_FLAG_USE_DELETE
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
		kint_copy
	};

	temp_int.klbase.type = klint_t;
	aux_float.klbase.type = klfloat_t;
}