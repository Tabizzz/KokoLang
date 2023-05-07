#include "global.h"

KLType* klmap_t = nullptr;

void global_klmap_t()
{
	klmap_t = new KLType {
		.klbase = {
			.flags = KLOBJ_FLAG_USE_DELETE
		},
		.name = "map",
		.size = sizeof(kl_sptr),
		.initializer = karr_init,
		KLTYPE_METADATA
	};
}