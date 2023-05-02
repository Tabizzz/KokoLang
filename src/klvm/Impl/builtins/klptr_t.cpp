#include "global.h"

KLType* klptr_t = nullptr;

static void kptr_init(KlObject *obj) {
	auto ptr = KLCAST(kl_ptr, obj);
	ptr->value = nullptr;
}

void global_klptr_t()
{
	klptr_t = new KLType {
		.klbase = {
			.flags = KLOBJ_FLAG_USE_DELETE
		},
		.name = "ptr",
		.size = sizeof(kl_ptr),
		.initializer = kptr_init,
		KLTYPE_METADATA
	};
}