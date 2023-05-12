#include "global.h"
#include <sstream>

#define KASLIST(x) KLCAST(vector<KlObject*>, KLCAST(kl_sptr, x)->value)

KLType *kllist_t = nullptr;

void klist_end(KlObject *obj) {
	auto vec = KASLIST(obj);
	if (vec) {
		for (auto elem: *vec) {
			klDeref(elem);
		}
		delete vec;
	}
}

KlObject *klist_ctor(KlObject *self, KlObject **argv, kbyte argc) {
	auto ret = klIns(kllist_t);
	auto ptr = KLCAST(kl_sptr, ret);

	if (argc == 0 || !argv[0]) {
		ptr->value = new vector<KlObject *>();
	} else {
		if (argv[0]->type == klint_t) {
			auto vec = new vector<KlObject *>();
			vec->reserve(KASINT(argv[0]));
			ptr->value = vec;
		} else if (argv[0]->type == klarray_t) {
			auto size = KASARRSIZE(argv[0]);
			auto arr = KASARR(argv[0]);
			auto vec = new vector<KlObject *>(size);
			for (int i = 0; i < size; ++i) {
				klCopy(arr[i], &vec->at(i));
			}
			ptr->value = vec;
			ptr->size = size;
		} else if (argv[0]->type == kllist_t) {
			auto size = KASARRSIZE(argv[0]);
			auto arr = KASLIST(argv[0]);
			auto vec = new vector<KlObject *>(size);
			for (int i = 0; i < size; ++i) {
				klCopy(arr->at(i), &vec->at(i));
			}
			ptr->value = vec;
			ptr->size = size;
		} else {
			throw runtime_error(string_format("Invalid argument in list constructor, expected one of: int, array or list but received %s",
											  argv[0]->type->name));
		}
	}

	return ret;
}

void global_kllist_t() {

	auto func = KLCAST(KLFunction, klIns(klfunc_t));
	func->external = true;
	func->name = KLSTR(".ctor");
	func->invokable = klist_ctor;
	func->args = 1;

	kllist_t = new KLType{
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"list",
		0,
		sizeof(kl_sptr),
		karr_init,
		klist_end,
		KLWRAP(func)
	};
	KLTYPE_METADATA(kllist_t)
}