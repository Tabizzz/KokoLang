#include "klvm_internal.h"
#include "klapi_builtins.h"

CAPI KlObject* klBuiltinInt(kint val)
{
	auto base = klIns(klint_t);
	auto obj = KLCAST(kl_int, base);
	obj->value = val;
	return base;
}

CAPI KlObject *klBuiltinFloat(kfloat val) {
	auto base = klIns(klfloat_t);
	auto obj = KLCAST(kl_float , base);
	obj->value = val;
	return base;
}

CAPI KlObject *klBuiltinPtr(void* val) {
	auto base = klIns(klptr_t);
	auto obj = KLCAST(kl_ptr , base);
	obj->value = val;
	return base;
}

CPPAPI KlObject *klBuiltinString(const string &val) {
	auto base = klIns(klstring_t);
	auto obj = KLCAST(kl_string , base);
	obj->size = val.size();
	obj->value = new char[obj->size];
	val.copy(const_cast<char*>(obj->value), obj->size);
	return base;
}

CAPI KlObject *klBuiltinArr(size_t size) {
	auto base = klIns(klarray_t);
	auto arr = KLCAST(kl_sptr, base);

	arr->size = size;
	if(size > 0) {
		arr->value = new KlObject*[size]{};
	}

	return base;
}

