#include "klvm_internal.h"
#include "klapi.h"
#include <stdexcept>
#include <cstring>
#include <sstream> //for std::stringstream
#include <filesystem>

KLPackage *globalPackage = nullptr;
unordered_map<string, KLPackage *> *packages;

unordered_map<string, KLPackage *> *kliRootPackages() {
	return packages;
}

CAPI void klInit() {
	static_assert(sizeof(KLCAST(kl_int, nullptr)->value) == sizeof(KLCAST(kl_float, nullptr)->value),
	              "kl_int and kl_float dont have the same size.");

	if (!klConfig.alloc) {
		klConfig.alloc = std::malloc;
	}
	if (!klConfig.dealloc) {
		klConfig.dealloc = [](void *ptr, size_t) { std::free(ptr); };
	}
	if (!klConfig.installDir) {
		static string currentPath = filesystem::current_path().string();
		klConfig.installDir = currentPath.c_str();
	}

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

	std::memset(&klConfig, 0, sizeof(KLConfig));
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

static KLObject *defaultToStr(KLObject *obj) {
	std::stringstream ss;
	ss << '[';
	ss << obj->type->name;
	ss << ' ';
	ss << obj;
	ss << ']';
	auto s = ss.tellp();
	auto buff = new char[s];
	ss.read(buff, s);

	// create the stream
	auto str = KLCAST(kl_string, klIns(klstring_t));
	str->size = s;
	str->value = buff;
	return KLWRAP(str);
}

static int8_t defaultEquals(KLObject *a, KLObject *b) {
	return a == b ? 1 : 0;
}

CAPI void klDefType(KLType *type) {
	if (!type->name || strlen(type->name) == 0) {
		throw runtime_error("Types must have a name");
	}
	if (strchr(type->name, '.') || strchr(type->name, ':') || strchr(type->name, '%')) {
		throw runtime_error("Types cant contain the following characters in name: '.', ':' and '%'");
	}

	if (type->KLManagingFunctions.constructor) {
		if (type->KLManagingFunctions.constructor->type != klfunc_t) {
			throw runtime_error("A type constructor must be a function.");
		}
		auto func = KLCAST(KLFunction, type->KLManagingFunctions.constructor);
		if (func->margs == 0) {
			throw runtime_error("Type constructors must have at least 1 argument.");
		}
	}

	// set the type
	type->klbase.type = kltype_t;
	// increase the instance count of type
	kltype_t->inscount++;
	// the type is defined, so it only have one ref.
	type->klbase.refs = 1;
	type->inscount = 0;

	// set default tostring and equals
	if (!type->KLConversionFunctions.toString) {
		type->KLConversionFunctions.toString = defaultToStr;
	}
	if (!type->KLComparerFunctions.equal) {
		type->KLComparerFunctions.equal = defaultEquals;
	}
}

/*
 * src is null, dest is not null.
 */
void inline kliCopyA(KLObject **dest) {
	klDeref(*dest);
	*dest = nullptr;
}

/*
 * src is not null, dest is null
 */
void inline kliCopyB(KLObject *src, KLObject **dest) {
	//                      | dont clone constants
	if (src->type->KLRefFunctions.clone && !src->rflags.constant) {
		// no need to deref dest because is null
		*dest = src->type->KLRefFunctions.clone(src);
	} else {
		// no need to deref dest because is null
		klRef(src);
		*dest = src;
	}
}

void inline kliCopyD(KLObject *src, KLObject **dest) {
	if (!src->rflags.constant) {
		//  | if the target is a constant we cant change the value with a copy, so we need to clone or move
		if (!(*dest)->rflags.constant && (src->type == (*dest)->type) && src->type->KLRefFunctions.copy) {
			src->type->KLRefFunctions.copy(src, *dest);
			return;
		} else if (src->type->KLRefFunctions.clone) {
			auto temp = src->type->KLRefFunctions.clone(src);
			klDeref(*dest);
			*dest = temp;
			return;
		}
	}
	klRef(src);
	klDeref(*dest);
	*dest = src;
}

#define THROW_ON_NO_VALID_TARGET if(!dest){throw std::runtime_error("Not a valid target");}

CAPI void klCopy(KLObject *src, KLObject **dest) {
	THROW_ON_NO_VALID_TARGET
	if (!src && *dest) {
		kliCopyA(dest);
	} else if (src && !*dest) {
		kliCopyB(src, dest);
	} else if (src && *dest) {
		kliCopyD(src, dest);
	}
}

CAPI void klClone(KLObject *src, KLObject **dest) {
	THROW_ON_NO_VALID_TARGET
	if (!src && *dest) {
		kliCopyA(dest);
	} else if (src) {
		// we can safely deref dest, if iss null nothing happens
		klDeref(*dest);
		if (src->type->KLRefFunctions.clone) {
			// no need to ref src, we are creating a new instance on clone.
			*dest = src->type->KLRefFunctions.clone(src);
		} else {
			klRef(src);
			*dest = src;
		}
	}
}

CAPI void klMove(KLObject *src, KLObject **dest) {
	THROW_ON_NO_VALID_TARGET
	klRef(src);
	klDeref(*dest);
	*dest = src;
}

CAPI void klTransfer(KLObject *src, KLObject **dest) {
	THROW_ON_NO_VALID_TARGET

	if (!src && dest) {
		kliCopyA(dest);
	} else if (src) {
		// requirement to copy in transfer:
		// - source is not a constant, this is we want to move constant by reference
		// - dest is not null
		// - dest is not a constant, we cant copy into a constant because that change it value.
		// - src and dest have the same type
		// - the type support copy
		if (!src->rflags.constant && *dest && !(*dest)->rflags.constant && (src->type == (*dest)->type) &&
		    src->type->KLRefFunctions.copy) {
			src->type->KLRefFunctions.copy(src, *dest);
			klDeref(src);
		} else if (!src->rflags.constant && src->refs > 1 && src->type->KLRefFunctions.clone) {
			auto temp = src->type->KLRefFunctions.clone(src);
			klDeref(*dest);
			klDeref(src);
			*dest = temp;
		} else {
			klDeref(*dest);
			*dest = src;
		}
	}
}
