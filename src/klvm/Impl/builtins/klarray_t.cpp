#include "global.h"

KLType* klarray_t = nullptr;

void karr_init(KlObject *obj) {
	auto ptr = KLCAST(kl_sptr, obj);
	ptr->size = 0;
	ptr->content = nullptr;
}

void karr_end(KlObject *obj) {
	auto ptr = KLCAST(kl_sptr, obj);
	auto arr = KLCAST(KlObject*, ptr->content);
	if(arr)
	{
		// dereference each element on the array
		for (int i = 0; i < ptr->size; ++i) {
			klDeref(arr[i]);
		}
		delete[] arr;
	}
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
		karr_init,
		karr_end
	};
	KLTYPE_METADATA(klarray_t)
}