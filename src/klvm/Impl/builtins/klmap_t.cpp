#include "global.h"

KLType *klmap_t = nullptr;

void global_klmap_t() {
	klmap_t = new KLType{
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"map",
		0,
		sizeof(kl_sptr),
		karr_init,
	};

	KLTYPE_METADATA(klmap_t)
}