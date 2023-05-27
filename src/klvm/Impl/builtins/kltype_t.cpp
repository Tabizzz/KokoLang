#include "global.h"
#include <sstream>

KLType *kltype_t = nullptr;

static void ktype_ins(KLObject *obj) {
	auto type = KLCAST(KLType, obj);
	type->variables = new MetaMap();
	type->functions = new MetaMap();
	type->metadata = new MetaMap();
}

static void ktype_end(KLObject *obj) {
	auto type = KLCAST(KLType, obj);
	klDeref(type->KLManagingFunctions.constructor);
	kliDerefAndDeleteMap(type->variables);
	kliDerefAndDeleteMap(type->functions);
	kliDerefAndDeleteMap(type->metadata);
}

static KLObject* ktype_tostr(KLObject *obj) {
	std::ostringstream ss;
	ss << "[type ";
	ss << KLCAST(KLType, obj)->name;
	ss << ']';
	return KLSTR(ss.str());
}

void global_kltype_t() {
	kltype_t = new KLType{
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE | KLOBJ_FLAG_IGNORE_REF | KLOBJ_FLAG_NO_INSCOUNT
		},
		"type",
		sizeof(KLType),
		0, 0,
		ktype_ins,
		ktype_end,
		REP1(nullptr)
		ktype_tostr
	};
	KLTYPE_METADATA(kltype_t)

	kltype_t->klbase.type = kltype_t;
}