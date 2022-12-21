#include "KokoLangInternal.h"
#include "klapi_builtins.h"


CAPI KlObject* klBuiltinInt(int64_t val)
{
	auto base = klIns(&klBType_Int);
	auto obj = KLCAST(kl_int, base);
	obj->value = val;
	return base;
}

KlObject *klBuiltinFloat(double val) {
	auto base = klIns(&klBType_Float);
	auto obj = KLCAST(kl_float , base);
	obj->value = val;
	return base;
}

KlObject* baseBools( bool val)
{
	auto base = klIns(&klBType_Bool);
	auto obj = KLCAST(kl_bool, base);
	obj->value = val;
	return base;
}

KlObject *klBuiltinBool(bool val) {
	static KlObject* True = baseBools(true);
	static KlObject* False = baseBools(false);
	return val ? True : False;
}

CPPAPI KlObject *klBuiltinString(const string &val) {
	auto base = klIns(&klBType_String);
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
