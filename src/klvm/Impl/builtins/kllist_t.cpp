#include "global.h"

KLType* kllist_t = nullptr;

void global_kllist_t()
{
	kllist_t= new KLType {
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"list",
		0,
		sizeof(kl_sptr),
		karr_init,
	};
	KLTYPE_METADATA(kllist_t)
}