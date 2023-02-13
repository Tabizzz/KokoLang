﻿#include "KokoLangInternal.h"
#include "klapi_builtins.h"

CAPI KlObject* klBuiltinInt(int64_t val)
{
	auto base = klIns(&klBType_Int);
	auto obj = KLCAST(kl_int, base);
	obj->value = val;
	return base;
}

CAPI KlObject *klBuiltinFloat(double_t val) {
	auto base = klIns(&klBType_Float);
	auto obj = KLCAST(kl_float , base);
	obj->value = val;
	return base;
}

static kl_bool kl_bool_true = {
		KlObject{
				&klBType_Bool,
				1
		},
		true
};

static kl_bool kl_bool_false = {
		KlObject{
				&klBType_Bool,
				1
		},
		false
};

CAPI KlObject *klBuiltinBool(char val) {
	return KLWRAP(val ? &kl_bool_true : &kl_bool_false);
}

CPPAPI KlObject *klBuiltinString(const string &val) {
	auto base = klIns(&klBType_String);
	auto obj = KLCAST(kl_string , base);
	obj->size = val.size();
	obj->value = new char[obj->size];
	val.copy(obj->value, obj->size);
	return base;
}

CAPI KlObject *klBuiltinString_c(const char *val) {
	return klBuiltinString(val);
}
