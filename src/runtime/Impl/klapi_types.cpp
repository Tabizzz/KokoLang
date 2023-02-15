#include "KokoLangInternal.h"
#include "klapi_types.h"
#include <cassert>
#include <cstring>
#include <sstream>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"

KlObject* klself_return(KlObject* base) {
	klRef(base);
	return base;
}

#pragma region temps

// static object used to temporary store the value of the operations
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
	if(y)
	{
		if (y->type == &klBType_Int) {
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

void kint_add(KlObject* first, KlObject* second, KlObject** target)
{
	if(second) {
		if(second->type == &klBType_Float) {
			temp_float.value = KASINT(first);
			klBType_Float.opAdd(KLWRAP(&temp_float), second, target);
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
		klCopy(KLWRAP(&temp_int), target);
	} else {
		klCopy(first, target);
	}
}

void kint_sub(KlObject* first, KlObject* second, KlObject** target)
{
	if(second) {
		if(second->type == &klBType_Float) {
			temp_float.value = KASINT(first);
			klBType_Float.opSub(KLWRAP(&temp_float), second, target);
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
		temp_int.value = x - y;
		klCopy(KLWRAP(&temp_int), target);
	} else {
		klCopy(first, target);
	}
}

void kint_mul(KlObject* first, KlObject* second, KlObject** target)
{
	if(second) {
		if(second->type == &klBType_Float) {
			klBType_Float.opMul(second, first, target);
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
		klCopy(KLWRAP(&temp_int), target);
	} else {
		temp_int.value = 0;
		klCopy(KLWRAP(&temp_int), target);
	}
}

void kint_div(KlObject* first, KlObject* second, KlObject** target)
{
	if(second) {
		if(second->type == &klBType_Float) {
			temp_float.value = KASINT(first);
			klBType_Float.opDiv(KLWRAP(&temp_float), second, target);
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
		// throw before hardware error
		if (y == 0) throw logic_error("Division by 0");
		temp_int.value = x / y;
		klCopy(KLWRAP(&temp_int), target);
	} else {
		throw logic_error("Division by 0");
	}
}

void kint_mod(KlObject* first, KlObject* second, KlObject** target)
{
	if(second) {
		if(second->type == &klBType_Float) {
			temp_float.value = KASINT(first);
			klBType_Float.opMod(KLWRAP(&temp_float), second, target);
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
		// throw before hardware error
		if (y == 0) throw logic_error("Division by 0");
		temp_int.value = x % y;
		klCopy(KLWRAP(&temp_int), target);
	} else {
		throw logic_error("Division by 0");
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
#pragma endregion int

#pragma region float

void kfloat_init(KlObject* obj) {
	auto ptr = KLCAST(kl_float, obj);
	ptr->value = 0;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantFunctionResult"
int8_t kfloat_comparer(KlObject* x, KlObject* y) {
	double_t first = KASFLOAT(x);
	double_t second = 0;
	if(y) {
		if (y->type == &klBType_Float) {
			second = KASFLOAT(y);
		} else if (y->type == &klBType_Int) {
			second = KASINT(y);
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
	if(y)
	{
		if (y->type == &klBType_Float) {
			return KASFLOAT(x) == KASFLOAT(y);
		} else if (y->type == &klBType_Int) {
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

KlObject* kfloat_clone(KlObject* base) {
	return KLFLOAT(KASFLOAT(base));
}

void kfloat_copy(KlObject* a, KlObject* b) {
	KASFLOAT(b) = KASFLOAT(a);
}

KlObject* kfloat_tostr(KlObject* base)
{
	auto val = KASFLOAT(base);
	std::ostringstream stream;
	auto max = std::numeric_limits<double_t>::digits10 + 1;
	stream.precision(max);
	stream << val;
	return KLSTR(stream.str());
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

void kfloat_add(KlObject* first, KlObject* second, KlObject** target)
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
		klCopy(KLWRAP(&temp_float), target);
	} else {
		klCopy(nullptr, target);
	}
}

void kfloat_sub(KlObject* first, KlObject* second, KlObject** target)
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
		klCopy(KLWRAP(&temp_float), target);
	} else {
		klCopy(nullptr, target);
	}
}

void kfloat_mul(KlObject* first, KlObject* second, KlObject** target)
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
		klCopy(KLWRAP(&temp_float), target);
	} else {
		temp_float.value = 0;
		klCopy(KLWRAP(&temp_float), target);
	}
}

void kfloat_div(KlObject* first, KlObject* second, KlObject** target)
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
		// throw before hardware error
		if (y == 0) throw logic_error("Division by 0");
		temp_float.value = x / y;
		klCopy(KLWRAP(&temp_float), target);
	} else {
		throw logic_error("Division by 0");
	}
}

void kfloat_mod(KlObject* first, KlObject* second, KlObject** target)
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
		// throw before hardware error
		if (y == 0) throw logic_error("Division by 0");
		temp_float.value = fmod(x, y);
		klCopy(KLWRAP(&temp_float), target);
	} else {
		throw logic_error("Division by 0");
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
		kfloat_mod,
		kfloat_clone,
		kfloat_copy,
};

#pragma endregion float

#pragma region bool

KlObject* kbool_tostr(KlObject* obj)
{
	if(KASBOOL(obj))
	{
		return KLSTR("true");
	}
	else
	{
		return KLSTR("false");
	}
}

KLType klBType_Bool =
{
		KlObject(),
		"bit",
		0,
		sizeof(kl_bool),
		nullptr,
		nullptr,
		nullptr,
		kbool_tostr,
		nullptr,
		nullptr,
		klself_return,
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
		MetaMap(),
		MetaMap(),
		MetaMap(),
		KLTYPE_FLAG_NOINSTANCE				// cannot instance
};

#pragma endregion bool

#pragma region string

void kstring_init(KlObject* obj) {
	auto ptr = KLCAST(kl_string, obj);
	ptr->value = nullptr;
	ptr->size = 0;
}

void kstring_end(KlObject* obj) {
	auto ptr = KLCAST(kl_string, obj);
	delete[] ptr->value;
}

int8_t kstring_compare(KlObject* x, KlObject* y)
{
	auto first = KLCAST(kl_string, x);
	kl_string* second = nullptr;
	bool flag = false;
	if(y)
	{
		if(y->type == &klBType_String)
		{
			second = KLCAST(kl_string, y);
		}
		else if(y->type->toString)
		{
			flag = true;
			second = KLCAST(kl_string, y->type->toString(y));
		}
	}
	if(!second) {
		if (first->size == 0) {
			// first is empty and second is null, we consider that case equal.
			return 0;
		} else {
			// first is not empty so is bigger than null.
			return -1;
		}
	}

	int8_t dev = strncmp(first->value, second->value, min(first->size, second->size)) * -1; // NOLINT(cppcoreguidelines-narrowing-conversions)

	if(dev == 0 && first->size != second->size)
	{
		if(first->size < second->size)
			dev = 1;
		else
			dev = -1;
	}

	if(flag)
	{
		klDeref(KLWRAP(second));
	}
	return dev;
}

int8_t kstring_equals(KlObject* x, KlObject* y)
{
	auto first = KLCAST(kl_string, x);
	kl_string* second = nullptr;
	bool flag = false;
	if(y) {
		if (y->type == &klBType_String) {
			second = KLCAST(kl_string, y);
		} else if (y->type->toString) {
			flag = true;
			second = KLCAST(kl_string, y->type->toString(y));
		}
	}
	if(!second) {
		if (first->size == 0) {
			// first is empty and second is null, we consider that case equal.
			return 1;
		} else {
			// first is not empty so is different from second.
			return 0;
		}
	}
	int8_t dev = false;

	if(first->size == second->size)
	{
		dev = strncmp(first->value, second->value, min(first->size, second->size)) == 0 ? 1 : 0;
	}

	if(flag)
	{
		klDeref(KLWRAP(second));
	}
	return dev;
}

void kstring_add(KlObject* x, KlObject* y, KlObject** target)
{
	auto first = KLCAST(kl_string, x);
	kl_string* second = nullptr;
	bool flag = false;
	if(y) {
		if (y->type == &klBType_String) {
			second = KLCAST(kl_string, y);
		} else if (y->type->toString) {
			flag = true;
			second = KLCAST(kl_string, y->type->toString(y));
		}
	}
	if(!second) {
		// second is null, so the resulting string is equals to first
		klClone(x, target);
		return;
	}

	if(first->size == 0)
	{
		// first is empty, so directly clone second
		klClone(KLWRAP(second), target);
		if(flag) klDeref(KLWRAP(second));
		return;
	}
	else if(second->size == 0)
	{
		// second is empty, so directly clone second
		klClone(x, target);
		if(flag) klDeref(KLWRAP(second));
		return;
	}

	auto size = first->size + second->size;
	auto value = new char[size];
	value[0] = 0;
	value[first->size] = 0;

	strncat(value, first->value, first->size);
	strncat(value, second->value, second->size);

	auto dev = klIns(&klBType_String);
	KLCAST(kl_string, dev)->size = size;
	KLCAST(kl_string, dev)->value = value;
	// dereference anything current value
	klDeref(*target);
	*target = dev;

	if(flag)
	{
		klDeref(KLWRAP(second));
	}
}

KlObject* kstring_clone(KlObject* obj)
{
	return KLSTR(KSTRING(obj));
}

KLType klBType_String =
{
		KlObject(),
		"str",
		0,
		sizeof(kl_string),
		kstring_init,
		nullptr,
		kstring_end,
		klself_return,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		kstring_compare,
		kstring_equals,
		kstring_add,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		kstring_clone,
		nullptr,
};

#pragma endregion string


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
		"weak",
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
	nullptr,
	nullptr,
	MetaMap(),
	MetaMap(),
	MetaMap(),
	KLTYPE_FLAG_NOINSTANCE
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

CAPI KlObject *klNew(KLType *type, KlObject **args, kbyte argc) {
	return nullptr;
}

CAPI KlObject *klIns(KLType *type) {
	if(KLTYPE_IS_STATIC(type)) {
		throw runtime_error("Unable to instance not instantiable type");
	}
	auto size = type->size;
	auto space = KLWRAP(malloc(size));
	space->type = type;
	space->refs = 1;
	type->inscount++;
	// call the initializer
	KLINVOKE(type->initializer)(space);
	return space;
}

CAPI void klRef(KlObject *object) {
	if(object && !KLTYPE_IS_STATIC(object->type)) object->refs++;
}

CAPI void klDeref(KlObject* object) {
	if(!object) return;
	if( KLTYPE_IS_STATIC(object->type)) return;

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

CAPI KlObject *klInvoke(KlObject *target, KlObject **argv, kbyte argc) {
	return nullptr;
}

#pragma clang diagnostic pop