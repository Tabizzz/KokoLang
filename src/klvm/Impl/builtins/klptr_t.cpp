#include "global.h"
#include <sstream> //for std::stringstream
#include <string>  //for std::string

KLType *klptr_t = nullptr;

kl_ptr temp_ptr = {
	KLObject{
		nullptr,
		1,
		KLOBJ_FLAG_IGNORE_REF
	},
	nullptr
};

static void kptr_init(KLObject *obj) {
	auto ptr = KLCAST(kl_ptr, obj);
	ptr->value = nullptr;
}

static int8_t kptr_equal(KLObject *x, KLObject *y) {
	if (y) {
		if (y->type == klptr_t) {
			return KASPTR(x) == KASPTR(y);
		} else {
			return 0;
		}
	}

	return KASPTR(x) == nullptr;
}

static KLObject *kptr_clone(KLObject *base) {
	return KLPTR(KASPTR(base));
}

static void kptr_copy(KLObject *a, KLObject *b) {
	KASPTR(b) = KASPTR(a);
}

static KLObject *kptr_tostr(KLObject *base) {
	auto val = KASPTR(base);
	std::stringstream ss;
	ss << val;
	return KLSTR(ss.str());
}

static KLObject *kptr_tobit(KLObject *base) {
	auto val = KASPTR(base);
	return KLBOOL(val ? true : false);
}

static void kptr_add(KLObject *first, KLObject *second, KLObject **target) {
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

static void kptr_sub(KLObject *first, KLObject *second, KLObject **target) {
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
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"ptr",
		0,
		 sizeof(kl_ptr),
		kptr_init,
		REP2(nullptr)
		kptr_tostr,
		REP2(nullptr)
		kptr_tobit,
		REP3(nullptr)
		kptr_equal,
		kptr_add,
		kptr_sub,
		REP3(nullptr)
		kptr_clone,
		kptr_copy,
	};
	KLTYPE_METADATA(klptr_t)

	temp_ptr.klbase.type = klptr_t;
}