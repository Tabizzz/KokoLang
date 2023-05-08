#include <sstream>
#include "global.h"

KLType *klfloat_t = nullptr;

// static object used to temporary store the value of the operations

kl_float temp_float = {
	KlObject{
		nullptr,
		1,
		KLOBJ_FLAG_IGNORE_REF
	},
	0
};

static void kfloat_init(KlObject *obj) {
	auto ptr = KLCAST(kl_float, obj);
	ptr->value = 0;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantFunctionResult"

static int8_t kfloat_comparer(KlObject *x, KlObject *y) {
	kfloat first = KASFLOAT(x);
	kfloat second = 0;
	if (y) {
		if (y->type == klfloat_t) {
			second = KASFLOAT(y);
		} else if (y->type == klint_t) {
			second = KASINT(y);
		} else if (y->type->toFloat) {
			auto frees = y->type->toFloat(y);
			second = KASFLOAT(frees);
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

static int8_t kfloat_equal(KlObject *x, KlObject *y) {
	if (y) {
		if (y->type == klfloat_t) {
			return KASFLOAT(x) == KASFLOAT(y);
		} else if (y->type == klint_t) {
			return KASFLOAT(x) == KASINT(y);
		} else if (y->type->toFloat) {
			auto frees = y->type->toFloat(y);
			auto dev = KASFLOAT(frees);
			klDeref(frees);
			return KASFLOAT(x) == dev;
		}
	}

	return KASFLOAT(x) == 0;
}

static KlObject *kfloat_clone(KlObject *base) {
	return KLFLOAT(KASFLOAT(base));
}

void kfloat_copy(KlObject *a, KlObject *b) {
	KASFLOAT(b) = KASFLOAT(a);
}

static KlObject *kfloat_tostr(KlObject *base) {
	auto val = KASFLOAT(base);
	std::ostringstream stream;
	auto max = std::numeric_limits<kfloat>::digits10 + 1;
	stream.precision(max);
	stream << val;
	return KLSTR(stream.str());
}

static KlObject *kfloat_toint(KlObject *base) {
	auto val = KASFLOAT(base);
	return KLINT((int64_t) val);
}

static KlObject *kfloat_tobit(KlObject *base) {
	auto val = KASFLOAT(base);
	return KLBOOL(val);
}

static void kfloat_add(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		auto x = KASFLOAT(first);
		kfloat y = 0;
		if (second->type == klfloat_t) {
			y = KASFLOAT(second);
		} else if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->toFloat) {
			auto frees = second->type->toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		temp_float.value = x + y;
		klCopy(KLWRAP(&temp_float), target);
	} else {
		klCopy(nullptr, target);
	}
}

static void kfloat_sub(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		auto x = KASFLOAT(first);
		kfloat y = 0;
		if (second->type == klfloat_t) {
			y = KASFLOAT(second);
		} else if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->toFloat) {
			auto frees = second->type->toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		temp_float.value = x - y;
		klCopy(KLWRAP(&temp_float), target);
	} else {
		klCopy(nullptr, target);
	}
}

static void kfloat_mul(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		auto x = KASFLOAT(first);
		kfloat y = 0;
		if (second->type == klfloat_t) {
			y = KASFLOAT(second);
		} else if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->toFloat) {
			auto frees = second->type->toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		temp_float.value = x * y;
		klCopy(KLWRAP(&temp_float), target);
	} else {
		temp_float.value = 0;
		klCopy(KLWRAP(&temp_float), target);
	}
}

static void kfloat_div(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		auto x = KASFLOAT(first);
		kfloat y = 0;
		if (second->type == klfloat_t) {
			y = KASFLOAT(second);
		} else if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->toFloat) {
			auto frees = second->type->toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		// throw before hardware error
		if (y == 0) throw logic_error("Division by 0");
		temp_float.value = x / y;
		klCopy(KLWRAP(&temp_float), target);
	} else {
		throw logic_error("Division by 0");
	}
}

static void kfloat_mod(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		auto x = KASFLOAT(first);
		kfloat y = 0;
		if (second->type == klfloat_t) {
			y = KASFLOAT(second);
		} else if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->toFloat) {
			auto frees = second->type->toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		// throw before hardware error
		if (y == 0) throw logic_error("Division by 0");
		temp_float.value = fmod(x, y);
		klCopy(KLWRAP(&temp_float), target);
	} else {
		throw logic_error("Division by 0");
	}
}

void global_klfloat_t() {
	klfloat_t = new KLType {
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
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
		kfloat_mod,
		kfloat_clone,
		kfloat_copy
	};
	KLTYPE_METADATA(klfloat_t)

	temp_float.klbase.type = klfloat_t;
}