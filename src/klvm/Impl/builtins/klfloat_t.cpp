#include "global.h"
#include <sstream>

KLType *klfloat_t = nullptr;

// static object used to temporary store the value of the operations

thread_local kl_float tmp_float = {
	KLObject{
		klfloat_t,
		2,
		KLOBJ_FLAG_IGNORE_REF
	},
	0
};

kl_float &temp_float() {
	return tmp_float;
}

static void kfloat_init(KLObject *obj) {
	auto ptr = KLCAST(kl_float, obj);
	ptr->value = 0;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantFunctionResult"

static int8_t kfloat_comparer(KLObject *x, KLObject *y) {
	kfloat first = KASFLOAT(x);
	kfloat second = 0;
	if (y) {
		if (y->type == klfloat_t) {
			second = KASFLOAT(y);
		} else if (y->type == klint_t) {
			second = KASINT(y);
		} else if (y == KLBOOL(true)) {
			second = 1;
		} else if (y->type->KLConversionFunctions.toFloat) {
			auto frees = y->type->KLConversionFunctions.toFloat(y);
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

static int8_t kfloat_equal(KLObject *x, KLObject *y) {
	if (y) {
		if (y->type == klfloat_t) {
			return KASFLOAT(x) == KASFLOAT(y);
		} else if (y->type == klint_t) {
			return KASFLOAT(x) == KASINT(y);
		} else if (y->type->KLConversionFunctions.toFloat) {
			auto frees = y->type->KLConversionFunctions.toFloat(y);
			auto dev = KASFLOAT(frees);
			klDeref(frees);
			return KASFLOAT(x) == dev;
		}
	}

	return KASFLOAT(x) == 0;
}

static KLObject *kfloat_clone(KLObject *base) {
	return KLFLOAT(KASFLOAT(base));
}

void kfloat_copy(KLObject *a, KLObject *b) {
	KASFLOAT(b) = KASFLOAT(a);
}

static KLObject *kfloat_tostr(KLObject *base) {
	auto val = KASFLOAT(base);
	std::ostringstream stream;
	auto max = std::numeric_limits<kfloat>::digits10 + 1;
	stream.precision(max);
	stream << val;
	return KLSTR(stream.str());
}

static KLObject *kfloat_toint(KLObject *base) {
	return KLINT(static_cast<kint>(KASFLOAT(base)));
}

static KLObject *kfloat_tobit(KLObject *base) {
	auto val = KASFLOAT(base);
	return KLBOOL(val);
}

static KLObject *kfloat_add(KLObject *first, KLObject *second) {
	if (second) {
		auto x = KASFLOAT(first);
		kfloat y = 0;
		if (second->type == klfloat_t) {
			y = KASFLOAT(second);
		} else if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toFloat) {
			auto frees = second->type->KLConversionFunctions.toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		temp_float().value = x + y;
	} else {
		temp_float().value = KASFLOAT(first);
	}
	return KLWRAP(&temp_float());
}

static KLObject *kfloat_sub(KLObject *first, KLObject *second) {
	if (second) {
		auto x = KASFLOAT(first);
		kfloat y = 0;
		if (second->type == klfloat_t) {
			y = KASFLOAT(second);
		} else if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toFloat) {
			auto frees = second->type->KLConversionFunctions.toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		temp_float().value = x - y;
	} else {
		temp_float().value = KASFLOAT(first);
	}
	return KLWRAP(&temp_float());
}

static KLObject *kfloat_mul(KLObject *first, KLObject *second) {
	if (second) {
		auto x = KASFLOAT(first);
		kfloat y = 0;
		if (second->type == klfloat_t) {
			y = KASFLOAT(second);
		} else if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toFloat) {
			auto frees = second->type->KLConversionFunctions.toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		temp_float().value = x * y;
	} else {
		temp_float().value = 0;
	}
	return KLWRAP(&temp_float());
}

static KLObject *kfloat_div(KLObject *first, KLObject *second) {
	if (second) {
		auto x = KASFLOAT(first);
		kfloat y = 0;
		if (second->type == klfloat_t) {
			y = KASFLOAT(second);
		} else if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toFloat) {
			auto frees = second->type->KLConversionFunctions.toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		// throw before hardware error
		if (y == 0) throw logic_error("Division by 0");
		temp_float().value = x / y;
		return KLWRAP(&temp_float());

	} else {
		throw logic_error("Division by 0");
	}
}

static KLObject *kfloat_mod(KLObject *first, KLObject *second) {
	if (second) {
		auto x = KASFLOAT(first);
		kfloat y = 0;
		if (second->type == klfloat_t) {
			y = KASFLOAT(second);
		} else if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toFloat) {
			auto frees = second->type->KLConversionFunctions.toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		// throw before hardware error
		if (y == 0) throw logic_error("Division by 0");
		temp_float().value = fmod(x, y);
		return KLWRAP(&temp_float());
	} else {
		throw logic_error("Division by 0");
	}
}

static KLObject *kfloat_pow(KLObject *first, KLObject *second) {
	if (second) {
		auto x = KASFLOAT(first);
		kfloat y = 0;
		if (second->type == klfloat_t) {
			y = KASFLOAT(second);
		} else if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toFloat) {
			auto frees = second->type->KLConversionFunctions.toFloat(second);
			y = KASFLOAT(frees);
			klDeref(frees);
		}
		temp_float().value = std::pow(x, y);
	} else {
		temp_float().value = 1;
	}
	return KLWRAP(&temp_float());
}

static KLObject *kfloat_inc(KLObject *first) {
	temp_float().value = KASFLOAT(first) + 1;
	return KLWRAP(&temp_float());
}

static KLObject *kfloat_dec(KLObject *first) {
	temp_float().value = KASFLOAT(first) - 1;
	return KLWRAP(&temp_float());
}

void global_klfloat_t() {
	klfloat_t = new KLType{
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"num",
		sizeof(kl_float),
		0, 0,
		{
			kfloat_init
		},
		{
			kfloat_tostr,
			kfloat_toint,
			klself_return,
			kfloat_tobit
		},
		kfloat_comparer,
		kfloat_equal,
		{
			kfloat_add,
			kfloat_sub,
			kfloat_mul,
			kfloat_div,
			kfloat_mod,
			kfloat_pow,
			kfloat_inc,
			kfloat_dec
		},
		kfloat_clone,
		kfloat_copy
	};
	KLTYPE_METADATA(klfloat_t)

	temp_float().klbase.type = klfloat_t;
}