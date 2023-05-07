#include "global.h"

KLType* kllist_t = nullptr;

void global_kllist_t()
{
	kllist_t= new KLType {
		.klbase = {
			.flags = KLOBJ_FLAG_USE_DELETE
		},
		.name = "list",
		.size = sizeof(kl_sptr),
		.initializer = karr_init,
		KLTYPE_METADATA
	};
}