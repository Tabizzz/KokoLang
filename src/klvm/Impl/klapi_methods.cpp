#include "klvm_internal.h"
#include "klapi_types.h"
#include <cassert>

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

CAPI void klDestroy(KlObject *object) {
	if (!object) return;
	if (!(object->flags & KLOBJ_FLAG_NO_INSCOUNT))
		object->type->inscount--;
	// call finalizer
	KLINVOKE(object->type->finalizer)(object);
	if (object->flags & KLOBJ_FLAG_USE_DELETE) {
		::operator delete(object);
	} else {
		free(object);
	}
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
	return nullptr;
}

CAPI KlObject *klInvoke(KlObject *target, KlObject **argv, kbyte argc) {
	if (!target) return nullptr;
	if (target->type == &klfunc_t) {
		auto func = KLCAST(KLFunction, target);
		return klInvokeCore(func, argv, argc);
	} else if (target->type == kltype_t) {
		return klNew(KLCAST(KLType, target), argv, argc);
	} else {
		auto find = target->type->functions->find("call");
		if (find != target->type->functions->end()) {
			return klInvokeCore(KLCAST(KLFunction, find->second), argv, argc);
		}
	}
	return nullptr;
}
