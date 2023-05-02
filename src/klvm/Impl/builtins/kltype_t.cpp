#include "global.h"

KLType* kltype_t = nullptr;

void ktype_ins(KlObject* obj)
{
	auto type = KLCAST(KLType, obj);
	type->variables = new MetaMap();
	type->functions = new MetaMap();
	type->metadata = new MetaMap();
}

void ktype_end(KlObject* obj)
{
	auto type = KLCAST(KLType, obj);
	kliDerefAndDeleteMap(type->variables);
	kliDerefAndDeleteMap(type->functions);
	kliDerefAndDeleteMap(type->metadata);
}

void global_kltype_t() {
	kltype_t = new KLType{
		.klbase = {
			.flags = KLOBJ_FLAG_IGNORE_REF | KLOBJ_FLAG_USE_DELETE | KLOBJ_FLAG_NO_INSCOUNT,
		},
		.name = "type",
		.size = sizeof(KLType),
		.initializer = ktype_ins,
		.finalizer = ktype_end,
		KLTYPE_METADATA
	};

	kltype_t->klbase.type = kltype_t;
}