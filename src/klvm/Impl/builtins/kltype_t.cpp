#include "global.h"
#include <sstream>

KLType *kltype_t = nullptr;

static void ktype_ins(KlObject *obj) {
	auto type = KLCAST(KLType, obj);
	type->variables = new MetaMap();
	type->functions = new MetaMap();
	type->metadata = new MetaMap();
}

static void ktype_end(KlObject *obj) {
	auto type = KLCAST(KLType, obj);
	kliDerefAndDeleteMap(type->variables);
	kliDerefAndDeleteMap(type->functions);
	kliDerefAndDeleteMap(type->metadata);
}

static KlObject* ktype_tostr(KlObject *obj) {
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
		0,
		sizeof(KLType),
		ktype_ins,
		ktype_end,
		REP1(nullptr)
		ktype_tostr
	};
	KLTYPE_METADATA(kltype_t)

	kltype_t->klbase.type = kltype_t;
}