#include "global.h"

thread_local kl_int tmp_int{
	KLObject{
		klint_t,
		2,
		KLOBJ_FLAG_IGNORE_REF
	},
	0
};

kl_int &temp_int() {
	return tmp_int;
}

KLType *klint_t = nullptr;

KLType *klreg_t = nullptr;

static void kint_init(KLObject *obj) {
	auto ptr = KLCAST(kl_int, obj);
	ptr->value = 0;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantFunctionResult"

static int8_t kint_comparer(KLObject *x, KLObject *y) {
	kint first = KASINT(x);
	kint second = 0;
	if (y) {
		if (y->type == klint_t) {
			second = KASINT(y);
		} else if (y->type == klfloat_t) {
			int8_t t = klfloat_t->KLComparerFunctions.comparer(y, x);
			t *= -1;    // we invert the operation because we are passing x first and y second
			return t;
		} else if (y == KLBOOL(true)) {
			second = 1;
		} else if (y->type->KLConversionFunctions.toInt) {
			auto frees = y->type->KLConversionFunctions.toInt(y);
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

static int8_t kint_equal(KLObject *x, KLObject *y) {
	if (y) {
		if (y->type == klint_t) {
			return KASINT(x) == KASINT(y);
		} else if (y->type == klfloat_t) {
			return KASINT(x) == KASFLOAT(y);
		} else if (y->type->KLConversionFunctions.toInt) {
			auto frees = y->type->KLConversionFunctions.toInt(y);
			auto dev = KASINT(frees);
			klDeref(frees);
			return KASINT(x) == dev;
		}
	}

	return KASINT(x) == 0;
}

static KLObject *kint_clone(KLObject *base) {
	return KLINT(KASINT(base));
}

static void kint_copy(KLObject *a, KLObject *b) {
	KASINT(b) = KASINT(a);
}

static KLObject *klint_tostr(KLObject *base) {
	auto val = KASINT(base);
	return KLSTR(to_string(val));
}

static KLObject *klint_toflt(KLObject *base) {
	return KLFLOAT(KASINT(base));
}

static KLObject *klint_tobit(KLObject *base) {
	auto val = KASINT(base);
	return KLBOOL(val);
}

static KLObject *kint_add(KLObject *first, KLObject *second) {
	if (second) {
		if (second->type == klfloat_t) {
			temp_float().value = KASINT(first);
			return klfloat_t->KLNumericFunctions.opAdd(KLWRAP(&temp_float()), second);
		}
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toInt) {
			auto frees = second->type->KLConversionFunctions.toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int().value = x + y;
	} else {
		temp_int().value = KASINT(first);
	}
	return KLWRAP(&temp_int());
}

static KLObject *kint_sub(KLObject *first, KLObject *second) {
	if (second) {
		if (second->type == klfloat_t) {
			temp_float().value = KASINT(first);
			return klfloat_t->KLNumericFunctions.opSub(KLWRAP(&temp_float()), second);
		}
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toInt) {
			auto frees = second->type->KLConversionFunctions.toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int().value = x - y;
	} else {
		temp_int().value = KASINT(first);
	}
	return KLWRAP(&temp_int());
}

static KLObject *kint_mul(KLObject *first, KLObject *second) {
	if (second) {
		if (second->type == klfloat_t) {
			return klfloat_t->KLNumericFunctions.opMul(second, first);
		}
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toInt) {
			auto frees = second->type->KLConversionFunctions.toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int().value = x * y;
	} else {
		temp_int().value = 0;
	}
	return KLWRAP(&temp_int());
}

static KLObject *kint_div(KLObject *first, KLObject *second) {
	if (second) {
		if (second->type == klfloat_t) {
			temp_float().value = KASINT(first);
			return klfloat_t->KLNumericFunctions.opDiv(KLWRAP(&temp_float()), second);
		}
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toInt) {
			auto frees = second->type->KLConversionFunctions.toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		// throw before hardware error
		if (y == 0) throw logic_error("Division by 0");
		temp_int().value = x / y;
		return KLWRAP(&temp_int());
	} else {
		throw logic_error("Division by 0");
	}
}

static KLObject *kint_mod(KLObject *first, KLObject *second) {
	if (second) {
		if (second->type == klfloat_t) {
			temp_float().value = KASINT(first);
			return klfloat_t->KLNumericFunctions.opMod(KLWRAP(&temp_float()), second);
		}
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toInt) {
			auto frees = second->type->KLConversionFunctions.toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		// throw before hardware error
		if (y == 0) throw logic_error("Division by 0");
		temp_int().value = x % y;
		return KLWRAP(&temp_int());
	} else {
		throw logic_error("Division by 0");
	}
}

// int pow do not exist, we call float pow so this always return a float
static KLObject *kint_pow(KLObject *first, KLObject *second) {
	temp_float().value = KASINT(first);
	return klfloat_t->KLNumericFunctions.opPow(KLWRAP(&temp_float()), second);
}

static KLObject *kint_inc(KLObject *first) {
	temp_int().value = KASINT(first) + 1;
	return KLWRAP(&temp_int());
}

static KLObject *kint_dec(KLObject *first) {
	temp_int().value = KASINT(first) - 1;
	return KLWRAP(&temp_int());
}

static KLObject *kint_bit(KLObject *first) {
	temp_int().value = ~KASINT(first);
	return KLWRAP(&temp_int());
}

static KLObject *kint_lshift(KLObject *first, KLObject *second) {
	if (second) {
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toInt) {
			auto frees = second->type->KLConversionFunctions.toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int().value = x << y;
	} else {
		temp_int().value = KASINT(first);
	}
	return KLWRAP(&temp_int());
}

static KLObject *kint_rshift(KLObject *first, KLObject *second) {
	if (second) {
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toInt) {
			auto frees = second->type->KLConversionFunctions.toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int().value = x << y;
	} else {
		temp_int().value = KASINT(first);
	}
	return KLWRAP(&temp_int());
}

static KLObject *kint_and(KLObject *first, KLObject *second) {
	if (second) {
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toInt) {
			auto frees = second->type->KLConversionFunctions.toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int().value = x & y;
	} else {
		temp_int().value = 0;
	}
	return KLWRAP(&temp_int());
}

static KLObject *kint_or(KLObject *first, KLObject *second) {
	if (second) {
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toInt) {
			auto frees = second->type->KLConversionFunctions.toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int().value = x | y;
	} else {
		temp_int().value = KASINT(first);
	}
	return KLWRAP(&temp_int());
}

static KLObject *kint_xor(KLObject *first, KLObject *second) {
	if (second) {
		auto x = KASINT(first);
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toInt) {
			auto frees = second->type->KLConversionFunctions.toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_int().value = x ^ y;
	} else {
		temp_int().value = KASINT(first) ^ 0;
	}
	return KLWRAP(&temp_int());
}

void global_klint_t() {
	klint_t = new KLType{
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"int",
		sizeof(kl_int),
		0, 0,
		{
			kint_init
		},
		{
			klint_tostr,
			klself_return,
			klint_toflt,
			klint_tobit
		},
		kint_comparer,
		kint_equal,
		{
			kint_add,
			kint_sub,
			kint_mul,
			kint_div,
			kint_mod,
			kint_pow,
			kint_inc,
			kint_dec
		},
		kint_clone,
		kint_copy,
		{
			kint_bit,
			kint_lshift,
			kint_rshift,
			kint_and,
			kint_or,
			kint_xor
		}
	};

	KLTYPE_METADATA(klint_t)

	klreg_t = new KLType{
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"reg",
		sizeof(kl_int),
		0, 0,
		{
			kint_init
		},
		REP3({})
		{
			nullptr,
			kint_copy
		}

	};

	KLTYPE_METADATA(klreg_t)
}