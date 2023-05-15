#include "klvm_internal.h"
#include "klapi_types.h"
#include <cassert>

KlObject *klself_return(KlObject *base) {
	klRef(base);
	return base;
}


CAPI KlObject *klIns(KLType *type) {
	if (KLTYPE_IS_STATIC(type)) {
		throw runtime_error("Unable to instance not instantiable type");
	}
	auto size = type->size;
	auto space = KLWRAP(malloc(size));
	space->type = type;
	space->refs = 1;
	space->flags = 0;
	type->inscount++;
	// call the initializer
	KLINVOKE(type->initializer)(space);
	return space;
}

CAPI void klDeref(KlObject *object) {
	if (!object) return;
	if (KLTYPE_IS_STATIC(object->type)) return;
	if (object->flags & KLOBJ_FLAG_IGNORE_REF) return;

	assert(object->refs > 0);
	if (!(object->flags & KLOBJ_FLAG_NO_INSCOUNT))
		assert(object->type->inscount > 0);
	object->refs--;
	if (object->refs == 0) {
		klDestroy(object);
	}
}

static inline void freeSpace(void *space, bool delet) {
	if (delet) {
		::operator delete(space);
	} else {
		free(space);
	}
}

CAPI void klDestroy(KlObject *object) {
	if (!object) return;
	if (!(object->flags & KLOBJ_FLAG_NO_INSCOUNT))
		object->type->inscount--;
	// call finalizer
	KLINVOKE(object->type->finalizer)(object);
	freeSpace(object, object->flags & KLOBJ_FLAG_USE_DELETE);
}

static inline KlObject *klInvokeCore(KLFunction *func, KlObject **argv, kbyte argc) {
	// implement call stack:
	// add the function to the call stack
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
	return func->invokable(KLWRAP(func), argv, argc);
}

CAPI KlObject *klNew(KLType *type, KlObject **args, kbyte argc) {
	if (!type) return nullptr;
	// type must have a constructor
	if (!type->constructor) {
		throw runtime_error(string_format("The type %s dont have a constructor to invoke", type->name));
	}
	auto size = argc + 1;
	auto obj = klIns(type);
	// we need to inject instance on args
	vector<KlObject *> argv(size);
	argv[0] = obj;
	for (int i = 1; i < size; ++i) {
		argv[i] = args[i - 1];
	}
	// we ignore the
	klDeref(klInvokeCore(KLCAST(KLFunction, type->constructor), argv.data(), size));
	return obj;
}

CAPI KlObject *klInvoke(KlObject *target, KlObject **argv, kbyte argc) {
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
