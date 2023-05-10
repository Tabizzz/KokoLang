#include "klvm_internal.h"
#include "klapi.h"
#include <stdexcept>
#include <cstring>
#include <sstream> //for std::stringstream

KLPackage *globalPackage = nullptr;
unordered_map<string, KLPackage *> *packages;

unordered_map<string, KLPackage *> *kliRootPackages() {
	return packages;
}

CAPI void klInit() {
	static_assert(sizeof(KLCAST(kl_int, nullptr)->value) == sizeof(KLCAST(kl_float, nullptr)->value),
				  "kl_int and kl_float dont have the same size.");

	klRestoreResolver();

	globalPackage = kliBuildGlobalPackage();

	// we need to set the name after types definition
	globalPackage->name = KLSTR("global");

	packages = new unordered_map<string, KLPackage *>();
}

CAPI void klEnd() {
	for (const auto &pack: *packages) {
		klDestroyPackage(pack.second);
	}
	delete packages;
	klDestroyPackage(globalPackage);
	globalPackage = nullptr;

	klDestroy(KLWRAP(kltype_t));

	// reset all lib types
	kltype_t = nullptr;
	klint_t = nullptr;
	klfloat_t = nullptr;
	klbool_t = nullptr;
	klstring_t = nullptr;
	klarray_t = nullptr;
	klptr_t = nullptr;
	kllist_t = nullptr;
	klmap_t = nullptr;
	klfunc_t = nullptr;
	klpack_t = nullptr;
	klinstruction_t = nullptr;
}

CAPI KLPackage *klGlobalPackage() {
	return globalPackage;
}

CAPI KLPackage **klRootPackages() {
	auto size = packages->size() + 1;
	auto dev = new KLPackage *[size];
	dev[size - 1] = nullptr;
	size_t index = 0;
	for (const auto &p: *packages) {
		dev[index++] = p.second;
	}
	return dev;
}

CAPI void klRegisterPackage(KLPackage *klPackage) {
	if (!klPackage->name || KLCAST(kl_string, klPackage->name)->size == 0) {
		throw runtime_error("Packages must have a name");
	}
	string name = KSTRING(klPackage->name);
	auto find = packages->find(name);
	if (find == packages->end()) {
		packages->insert(pair<string, KLPackage *>(name, klPackage));
		return;
	}
	throw runtime_error("trying to add a package but another package with the same name already exists");
}

static KlObject *defaultToStr(KlObject *obj) {
	std::stringstream ss;
	ss << '[';
	ss << obj->type->name;
	ss << ' ';
	ss << obj;
	ss << ']';
	return KLSTR(ss.str());
}

static int8_t defaultEquals(KlObject *a, KlObject *b) {
	return a == b ? 1 : 0;
}

CAPI void klDefType(KLType *type) {
	if (!type->name || strlen(type->name) == 0) {
		throw runtime_error("Types must have a name");
	}
	if (strchr(type->name, '.') || strchr(type->name, ':') || strchr(type->name, '%')) {
		throw runtime_error("Types cant contain the following characters in name: '.', ':' and '%'");
	}
	// set the type
	type->klbase.type = kltype_t;
	// increase the instance count of type
	kltype_t->inscount++;
	// the type is defined, so it only have one ref.
	type->klbase.refs = 1;
	type->inscount = 0;

	// set default tostring and equals
	if (!type->toString) {
		type->toString = defaultToStr;
	}
	if (!type->equal) {
		type->equal = defaultEquals;
	}
}

/*
 * src is null, dest is not null.
 */
void inline kliCopyA(KlObject **dest) {
	klDeref(*dest);
	*dest = nullptr;
}

/*
 * src is not null, dest is null
 */
void inline kliCopyB(KlObject *src, KlObject **dest) {
	if (src->type->clone) {
		// no need to deref dest because is null
		*dest = src->type->clone(src);
	} else {
		// no need to deref dest because is null
		klRef(src);
		*dest = src;
	}
}

void inline kliCopyD(KlObject *src, KlObject **dest) {
	if ((src->type == (*dest)->type) && src->type->copy) {
		src->type->copy(src, *dest);
		return;
	} else if (src->type->clone) {
		klDeref(*dest);
		*dest = src->type->clone(src);
		return;
	}
	klRef(src);
	klDeref(*dest);
	*dest = src;
}

#define THROW_ON_NO_VALID_TARGET if(!dest){throw std::runtime_error("Not a valid target");}

CAPI void klCopy(KlObject *src, KlObject **dest) {
	THROW_ON_NO_VALID_TARGET
	if (!src && *dest) {
		kliCopyA(dest);
	} else if (src && !*dest) {
		kliCopyB(src, dest);
	} else if (src && *dest) {
		kliCopyD(src, dest);
	}
}

CAPI void klClone(KlObject *src, KlObject **dest) {
	THROW_ON_NO_VALID_TARGET
	if (!src && *dest) {
		kliCopyA(dest);
	} else if (src && !*dest) {
		kliCopyB(src, dest);
	} else if (src && *dest) {
		klDeref(*dest);
		if (src->type->clone) {
			// no need to ref src, we are creating a new instance on clone.
			*dest = src->type->clone(src);
		} else {
			klRef(src);
			*dest = src;
		}
	}
}

CAPI void klMove(KlObject *src, KlObject **dest) {
	THROW_ON_NO_VALID_TARGET
	klDeref(*dest);
	klRef(src);
	*dest = src;
}

CAPI void klTransfer(KlObject **src, KlObject **dest) {
	THROW_ON_NO_VALID_TARGET
	auto val = src ? *src : nullptr;

	if (!val && dest) {
		kliCopyA(dest);
	} else if (val && !*dest) {
		*dest = val;
		*src = nullptr;
	} else if (val && *dest) {
		if ((val->type == (*dest)->type) && val->type->copy) {
			val->type->copy(val, *dest);
			klDeref(val);
			*src = nullptr;
		} else {
			klDeref(*dest);
			*dest = val;
			*src = nullptr;
		}
	}
}
