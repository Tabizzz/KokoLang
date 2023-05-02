#include "global.h"

static kl_bool kl_bool_true = {
	KlObject{
		nullptr,
		1,
		KLOBJ_FLAG_IGNORE_REF
	},
	true
};

static kl_bool kl_bool_false = {
	KlObject{
		nullptr,
		1,
		KLOBJ_FLAG_IGNORE_REF
	},
	false
};

CAPI KlObject *klBuiltinBool(char val) {
	return KLWRAP(val ? &kl_bool_true : &kl_bool_false);
}

static KlObject *kbool_tostr(KlObject *obj) {
	if (KASBOOL(obj)) {
		return KLSTR("true");
	} else {
		return KLSTR("false");
	}
}

KLType *klbool_t = nullptr;

void global_klbool_t() {
	klbool_t = new KLType {
		.klbase = {
			.flags = KLOBJ_FLAG_USE_DELETE
		},
		.name = "bool",
		.size = sizeof(kl_bool),
		.toString = kbool_tostr,
		.toBool = klself_return,
		.flags = KLTYPE_FLAG_NOINSTANCE,        // cannot instance
		KLTYPE_METADATA
	};

	kl_bool_true.klbase.type = klbool_t;
	kl_bool_false.klbase.type = klbool_t;
}
