#include "KokoLangInternal.h"
#include "klapi_builtins.h"


CAPI KlObject* klBuiltinInt(int val)
{
	auto base = klNew(KLType_int);
	auto obj = KLCAST(kl_int, base);
	obj->value = val;
	return base;
}

KlObject *klBuiltinFloat(double val) {
	auto base = klNew(KLType_float);
	auto obj = KLCAST(kl_float , base);
	obj->value = val;
	return base;
}

KlObject *klBuiltinBool(bool val) {
	auto base = klNew(KLType_bool);
	auto obj = KLCAST(kl_bool, base);
	obj->value = val;
	return base;
}

CPPAPI KlObject *klBuiltinString(const string &val) {
	auto base = klNew(KLType_string);
	auto obj = KLCAST(kl_string , base);
	obj->size = val.size();
	obj->value = new char[obj->size + 1]{};
	val.copy(obj->value, obj->size);
	return base;
}

CAPI KlObject *klBuiltinString_c(const char *val) {
	string source = val;
	return klBuiltinString(source);
}
