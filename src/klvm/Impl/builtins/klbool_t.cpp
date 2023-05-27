#include "global.h"

static kl_bool kl_bool_true = {
	KLObject{
		nullptr,
		1,
		KLOBJ_FLAG_IGNORE_REF | KLOBJ_FLAG_CONST
	},
	true
};

static kl_bool kl_bool_false = {
	KLObject{
		nullptr,
		1,
		KLOBJ_FLAG_IGNORE_REF  | KLOBJ_FLAG_CONST
	},
	false
};

CAPI KLObject *klBuiltinBool(char val) {
	return KLWRAP(val ? &kl_bool_true : &kl_bool_false);
}

static KLObject *kbool_tostr(KLObject *obj) {
	if (KASBOOL(obj)) {
		return KLSTR("true");
	} else {
		return KLSTR("false");
	}
}

KLType *klbool_t = nullptr;

void global_klbool_t() {
	klbool_t = new KLType{
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"bool",
		sizeof(kl_bool),
		0,
		KLTYPE_FLAG_NOINSTANCE,
		{},
		kbool_tostr,
		REP2(nullptr)
		klself_return
	};
	KLTYPE_METADATA(klbool_t)

	kl_bool_true.klbase.type = klbool_t;
	kl_bool_false.klbase.type = klbool_t;
}
