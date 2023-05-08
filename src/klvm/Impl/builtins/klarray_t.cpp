#include "global.h"

KLType* klarray_t = nullptr;

void karr_init(KlObject *obj) {
	auto ptr = KLCAST(kl_sptr, obj);
	ptr->size = 0;
	ptr->content = nullptr;
}

void global_klarray_t()
{
	klarray_t = new KLType {
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"array",
		0,
		sizeof(kl_sptr),
		karr_init
	};
	KLTYPE_METADATA(klarray_t)
}