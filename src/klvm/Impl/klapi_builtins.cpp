#include <cstring>
#include "klvm_internal.h"
#include "klapi_builtins.h"

CAPI KLObject *klBuiltinInt(kint val) {
	auto base = klIns(klint_t);
	auto obj = KLCAST(kl_int, base);
	obj->value = val;
	return base;
}

CAPI KLObject *klBuiltinFloat(kfloat val) {
	auto base = klIns(klfloat_t);
	auto obj = KLCAST(kl_float, base);
	obj->value = val;
	return base;
}

CAPI KLObject *klBuiltinPtr(void *val) {
	auto base = klIns(klptr_t);
	auto obj = KLCAST(kl_ptr, base);
	obj->value = val;
	return base;
}

CPPAPI KLObject *klBuiltinString(const string &val) {
	auto base = klIns(klstring_t);
	auto obj = KLCAST(kl_string, base);
	obj->size = val.size();
	obj->value = new char[obj->size];
	val.copy(const_cast<char *>(obj->value), obj->size);
	return base;
}

CAPI KLObject *klBuiltinArr(ksize size) {
	auto base = klIns(klarray_t);
	auto arr = KLCAST(kl_sptr, base);

	arr->size = size;
	if (size > 0) {
		arr->value = new KLObject *[size]{};
	}

	return base;
}

void klDefaultInitializer(KLObject *obj) {
	std::memset(obj + 1, 0, obj->type->size - sizeof(KLObject));
	for (const auto &var: *obj->type->variables) {
		auto v = KLCAST(KLVariable, var.second);
		if (v->data.typevar.defaultValue) klSetVariable(v, obj, v->data.typevar.defaultValue);
	}
}

void klDefaultFinalizer(KLObject *obj) {
	for (const auto &var: *obj->type->variables) {
		auto v = KLCAST(KLVariable, var.second);
		klSetVariable(v, obj, nullptr);
	}
}

static KLObject *ctorImpl(KLObject *, KLObject **, kbyte) { return nullptr; }

KLObject *klDefaultConstructor() {
	auto defualtCtor = KLCAST(KLFunction, klIns(klfunc_t));
	defualtCtor->external = true;
	defualtCtor->margs = 1;
	defualtCtor->args = 1;
	defualtCtor->name = KLSTR("ctor");
	defualtCtor->invokable = ctorImpl;

	return KLWRAP(defualtCtor);
}

