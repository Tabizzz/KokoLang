#include "global.h"
#include <sstream> //for std::stringstream
#include <string>  //for std::string

KLType *klptr_t = nullptr;

kl_ptr temp_ptr = {
	KlObject{
		nullptr,
		1,
		KLOBJ_FLAG_IGNORE_REF
	},
	nullptr
};

static void kptr_init(KlObject *obj) {
	auto ptr = KLCAST(kl_ptr, obj);
	ptr->value = nullptr;
}

static int8_t kptr_equal(KlObject *x, KlObject *y) {
	if (y) {
		if (y->type == klptr_t) {
			return KASPTR(x) == KASPTR(y);
		} else {
			return 0;
		}
	}

	return KASPTR(x) == nullptr;
}

static KlObject *kptr_clone(KlObject *base) {
	return KLPTR(KASPTR(base));
}

static void kptr_copy(KlObject *a, KlObject *b) {
	KASPTR(b) = KASPTR(a);
}

static KlObject *kptr_tostr(KlObject *base) {
	auto val = KASPTR(base);
	std::stringstream ss;
	ss << val;
	return KLSTR(ss.str());
}

static KlObject *kptr_tobit(KlObject *base) {
	auto val = KASPTR(base);
	return KLBOOL(val ? true : false);
}

static void kptr_add(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		auto x = KLCAST(kbyte, KASPTR(first));
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->toInt) {
			auto frees = second->type->toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_ptr.value = x + y;
		klCopy(KLWRAP(&temp_ptr), target);
	} else {
		klCopy(first, target);
	}
}

static void kptr_sub(KlObject *first, KlObject *second, KlObject **target) {
	if (second) {
		auto x = KLCAST(kbyte, KASPTR(first));
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->toInt) {
			auto frees = second->type->toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_ptr.value = x - y;
		klCopy(KLWRAP(&temp_ptr), target);
	} else {
		klCopy(first, target);
	}
}

void global_klptr_t() {
	klptr_t = new KLType{
		.klbase = {
			.flags = KLOBJ_FLAG_USE_DELETE
		},
		.name = "ptr",
		.size = sizeof(kl_ptr),
		.initializer = kptr_init,
		.toString = kptr_tostr,
		.toBool = kptr_tobit,
		.equal = kptr_equal,
		.opAdd = kptr_add,
		.opSub = kptr_sub,
		.clone = kptr_clone,
		.copy = kptr_copy,
		KLTYPE_METADATA
	};

	temp_ptr.klbase.type = klptr_t;
}