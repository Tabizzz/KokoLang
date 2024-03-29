﻿#include "klvm_internal.h"
#include "klapi_types.h"
#include <cassert>

KLObject *klself_return(KLObject *base) {
	klRef(base);
	return base;
}


CAPI KLObject *klIns(KLType *type) {
	if (type->rflags.no_instance) {
		throw runtime_error("Unable to instance not instantiable type");
	}
	auto size = type->size;
	auto space = KLWRAP(klConfig.alloc(size));
	space->type = type;
	space->refs = 1;
	space->flags = 0;
	type->inscount++;
	// call the initializer
	KLINVOKE(type->KLManagingFunctions.initializer)(space);
	return space;
}

CAPI void klDeref(KLObject *object) {
	if (!object) return;
	if (object->type->rflags.no_instance) return;
	if (object->rflags.ignore_ref) return;

	assert(object->refs > 0);
	if (!object->rflags.no_inscount)
		assert(object->type->inscount > 0);
	object->refs--;
	if (object->refs == 0) {
		if (object->type == kltype_t && KLCAST(KLType, object)->inscount > 0) {
			return;
		}
		klDestroy(object);
	}
}

static inline void freeSpace(void *space, bool delet, size_t size) {
	if (delet) {
		::operator delete(space);
	} else {
		klConfig.dealloc(space, size);
	}
}

CAPI void klDestroy(KLObject *object) { // NOLINT(misc-no-recursion)
	if (!object) return;
	auto size = object->type->size;
	// call finalizer
	KLINVOKE(object->type->KLManagingFunctions.finalizer)(object);
	if (!object->rflags.no_inscount) {
		object->type->inscount--;
		if(object->type->inscount <= 0 && object->type->klbase.refs <= 0) {
			klDestroy(KLWRAP(object->type));
		}
	}
	freeSpace(object, object->rflags.use_delete, size);
}

static inline KLObject *klInvokeCore(KLFunction *func, KLObject **argv, kbyte argc) {
	if (argc < func->margs) {
		throw runtime_error(string_format("Error calling function %s, expected at least %i args but received %i",
										  KSTRING(func->name).c_str(),
										  func->margs,
										  argc
		));
	} else if (func->args >= 0 && argc > func->args) {
		throw runtime_error(string_format("Error calling function %s, expected maximum %i args but received %i",
										  KSTRING(func->name).c_str(),
										  func->args,
										  argc
		));
	}
	auto st = klGetThreadState();
	st->callStack.push_back(func);
	auto ret = func->invokable(KLWRAP(func), argv, argc);
	st->callStack.pop_back();
	return ret;
}

CAPI KLObject *klNew(KLType *type, KLObject **args, kbyte argc) {
	if (!type) return nullptr;
	// type must have a constructor
	if (!type->KLManagingFunctions.constructor) {
		throw runtime_error(string_format("The type %s dont have a constructor to invoke", type->name));
	}
	auto size = argc + 1;
	auto obj = klIns(type);
	// we need to inject instance on args
	vector<KLObject *> argv(size);
	argv[0] = obj;
	for (int i = 1; i < size; ++i) {
		argv[i] = args[i - 1];
	}
	// we ignore the
	klDeref(klInvokeCore(KLCAST(KLFunction, type->KLManagingFunctions.constructor), argv.data(), size));
	return obj;
}

CAPI KLObject *klInvoke(KLObject *target, KLObject **argv, kbyte argc) {
	if (!target) return nullptr;
	if (target->type == klfunc_t) {
		auto func = KLCAST(KLFunction, target);
		return klInvokeCore(func, argv, argc);
	} else if (target->type == kltype_t) {
		return klNew(KLCAST(KLType, target), argv, argc);
	} else {
		KLFunction *out;
		if (klGetFunc(target, "call", &out)) {
			return klInvokeCore(KLCAST(KLFunction, out), argv, argc);
		}
	}
	return nullptr;
}
