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
		.klbase = {
			.flags = KLOBJ_FLAG_USE_DELETE
		},
		.name = "array",
		.size = sizeof(kl_sptr),
		.initializer = karr_init,
		KLTYPE_METADATA
	};
}