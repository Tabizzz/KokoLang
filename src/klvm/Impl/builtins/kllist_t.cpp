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
	auto ret = argv[0];
	auto ptr = KLCAST(kl_sptr, ret);

	if (argc == 1 || !argv[1]) {
		ptr->value = new vector<KlObject *>();
	} else {
		if (argv[1]->type == klint_t) {
			auto vec = new vector<KlObject *>();
			vec->reserve(KASINT(argv[1]));
			ptr->value = vec;
		} else if (argv[1]->type == klarray_t) {
			auto size = KASARRSIZE(argv[1]);
			auto arr = KASARR(argv[1]);
			auto vec = new vector<KlObject *>(size);
			for (int i = 0; i < size; ++i) {
				klCopy(arr[i], &vec->at(i));
			}
			ptr->value = vec;
			ptr->size = size;
		} else if (argv[1]->type == kllist_t) {
			auto size = KASARRSIZE(argv[1]);
			auto arr = KASLIST(argv[1]);
			auto vec = new vector<KlObject *>(size);
			for (int i = 0; i < size; ++i) {
				klCopy(arr->at(i), &vec->at(i));
			}
			ptr->value = vec;
			ptr->size = size;
		} else {
			throw runtime_error(string_format("Invalid argument in list constructor, expected one of: int, array or list but received %s",
											  argv[1]->type->name));
		}
	}

	return nullptr;
}

KlObject *klist_add(KlObject *, KlObject **argv, kbyte argc) {
	auto ptr = KLCAST(kl_sptr, argv[0]);
	auto list = KASLIST(argv[0]);

	list->reserve(argc - 1);
	for (int i = 1; i < argc; ++i) {
		KlObject* temp = nullptr;
		klCopy(argv[i], &temp);
		list->push_back(temp);
	}

	ptr->size = list->size();
	return nullptr;
}

void global_kllist_t() {

	auto func = KLCAST(KLFunction, klIns(klfunc_t));
	func->external = true;
	func->name = KLSTR(".ctor");
	func->invokable = klist_ctor;
	func->margs = 1;
	func->args = 2;

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
	
	ADD_FUNCTION(add, kllist_t, klist_add, 2, -1)
}