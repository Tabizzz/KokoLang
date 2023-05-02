#include "global.h"

KLType* klarray_t = nullptr;

void karr_init(KlObject *obj) {
	auto ptr = KLCAST(kl_arr, obj);
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
		.size = sizeof(kl_arr),
		.initializer = karr_init,
		KLTYPE_METADATA
	};
}