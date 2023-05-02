
#include "klvm_internal.h"
#include "klapi.h"
#include <stdexcept>
#include <cstring>

#define  STDREGTYPE(x) klDefType(&x); klPackageRegType(globalPackage, &x);


#if _NDEGUG
STDCHECKTYPE(x)
#else
#define STDCHECKTYPE(x) if(x.inscount) cout << "type "<< x.name << " still has " << x.inscount << " instances in memory" << endl;
#endif

KLPackage *globalPackage = nullptr;
map<string, KLPackage *> *packages;

map<string, KLPackage *> *kliRootPackages() {
	return packages;
}

void kliBuildGlobalPackage() {
	globalPackage = new KLPackage();
	globalPackage->klbase.type = &klBType_Package;
	klBType_Package.initializer(KLWRAP(globalPackage));
}

CAPI void klInit() {
	static_assert(sizeof(KLCAST(kl_int, nullptr)->value) == sizeof(KLCAST(kl_float, nullptr)->value),
				  "kl_int and kl_float dont have the same size.");

	klRestoreResolver();

	kliBuildGlobalPackage();

	// the ref count of types is always 1, and you should never call
	// deref with a type as this will try to destroy it but types are
	// not freeable

	// first init the base type.
	kltype_t.klbase.refs = 1;
	kltype_t.klbase.type = &kltype_t;
	kltype_t.inscount = 1;

	// define builtin types
	STDREGTYPE(klint_t)
	STDREGTYPE(klfloat_t)
	STDREGTYPE(klbool_t)
	STDREGTYPE(klstring_t)
	STDREGTYPE(klptr_t)
	STDREGTYPE(kloptr_t)
	STDREGTYPE(klarray_t)
	STDREGTYPE(klreg_t)

	// define runtime specific types
	STDREGTYPE(klBType_Instruction)
	STDREGTYPE(klBType_Func)
	STDREGTYPE(klBType_Package)
	STDREGTYPE(klBType_Variable)

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NullDereference"
	globalPackage->name = KLSTR("global");
#pragma clang diagnostic pop

	packages = new map<string, KLPackage *>();
}

CAPI void klEnd() {
	for (const auto &pack: *packages) {
		klDestroyPackage(pack.second);
	}
	delete packages;
	klBType_Package.finalizer(KLWRAP(globalPackage));
	delete globalPackage;
	globalPackage = nullptr;

	// check instance counts
	STDCHECKTYPE(klint_t)
	STDCHECKTYPE(klfloat_t)
	STDCHECKTYPE(klbool_t)
	STDCHECKTYPE(klstring_t)
	STDCHECKTYPE(klptr_t)
	STDCHECKTYPE(kloptr_t)
	STDCHECKTYPE(klarray_t)
	STDCHECKTYPE(klreg_t)

	// define runtime specific types
	STDCHECKTYPE(klBType_Instruction)
	STDCHECKTYPE(klBType_Func)
	STDCHECKTYPE(klBType_Package)
	STDCHECKTYPE(klBType_Variable)
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
	throw invalid_argument("trying to add a package but another package with the same name already exists");
}

CAPI void klDefType(KLType *type) {
	if (!type->name || strlen(type->name) == 0) {
		throw runtime_error("Types must have a name");
	}
	if (strchr(type->name, '.') || strchr(type->name, ':') || strchr(type->name, '%')) {
		throw runtime_error("Types cant contain the following characters in name: '.', ':' and '%'");
	}
	// set the type
	type->klbase.type = &kltype_t;
	// increase the instance count of type
	kltype_t.inscount++;
	// the type is defined, so it only have one ref.
	type->klbase.refs = 1;
	type->inscount = 0;
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
