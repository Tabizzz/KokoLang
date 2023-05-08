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
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE | KLOBJ_FLAG_IGNORE_REF | KLOBJ_FLAG_NO_INSCOUNT
		},
		"type",
		0,
		sizeof(KLType),
		ktype_ins,
		ktype_end
	};
	KLTYPE_METADATA(kltype_t)

	kltype_t->klbase.type = kltype_t;
}