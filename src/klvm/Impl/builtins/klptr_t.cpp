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

static KLObject *kptr_add(KLObject *first, KLObject *second) {
	if (second) {
		auto x = KLCAST(kbyte, KASPTR(first));
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toInt) {
			auto frees = second->type->KLConversionFunctions.toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_ptr.value = x + y;
	} else {
		temp_ptr.value = KASPTR(first);
	}
	return KLWRAP(&temp_ptr);
}

static KLObject *kptr_sub(KLObject *first, KLObject *second) {
	if (second) {
		auto x = KLCAST(kbyte, KASPTR(first));
		kint y = 0;
		if (second->type == klint_t) {
			y = KASINT(second);
		} else if (second->type->KLConversionFunctions.toInt) {
			auto frees = second->type->KLConversionFunctions.toInt(second);
			y = KASINT(frees);
			klDeref(frees);
		}
		temp_ptr.value = x - y;
	} else {
		temp_ptr.value = KASPTR(first);
	}
	return KLWRAP(&temp_ptr);
}

void global_klptr_t() {
	klptr_t = new KLType{
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"ptr",
		sizeof(kl_ptr),
		0, 0,
		{
			kptr_init
		},
		{
			kptr_tostr,
			REP2(nullptr)
			kptr_tobit
		},
		{
			nullptr,
			kptr_equal
		},
		{
			kptr_add,
			kptr_sub
		},
		kptr_clone,
		kptr_copy,
	};
	KLTYPE_METADATA(klptr_t)

	temp_ptr.klbase.type = klptr_t;
}