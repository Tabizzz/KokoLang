#include "global.h"
#include <sstream>
#include <algorithm>

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

KlObject *klist_ctor(KlObject *, KlObject **argv, kbyte argc) {
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
			throw runtime_error(string_format(
				"Invalid argument in list constructor, expected one of: int, array or list but received %s",
				argv[1]->type->name));
		}
	}

	return nullptr;
}

static KlObject *klist_add(KlObject *, KlObject **argv, kbyte argc) {
	auto ptr = KLCAST(kl_sptr, argv[0]);
	auto list = KASLIST(argv[0]);

	list->reserve(argc - 1);
	for (int i = 1; i < argc; ++i) {
		KlObject *temp = nullptr;
		klCopy(argv[i], &temp);
		list->push_back(temp);
	}

	ptr->size = list->size();
	return nullptr;
}

static KlObject *klist_tostr(KlObject *obj) {
	std::stringstream ss;
	auto size = KASARRSIZE(obj);
	auto arr = KASLIST(obj);
	ss << "[";
	if (size > 0) {
		for (int i = 0; i < size - 1; ++i) {
			auto val = arr->at(i);
			if (val) {
				if (val->type == klstring_t) {
					ss << '"';
					ss.write(KASSTR(val), KASSTRSIZE(val)) << "\", ";
				} else if (val->type == kllist_t) {
					ss << "list(" << KASARRSIZE(val) << "), ";
				} else if (val->type == klarray_t) {
					ss << "array(" << KASARRSIZE(val) << "), ";
				} else if (val->type->toString) {
					auto str = val->type->toString(val);
					ss.write(KASSTR(str), KASSTRSIZE(str)) << ", ";
					klDeref(str);
				}
			} else {
				ss << "null, ";
			}
		}
		auto val = arr->at(size - 1);
		if (val) {
			if (val->type == klstring_t) {
				ss << '"';
				ss.write(KASSTR(val), KASSTRSIZE(val)) << '"';
			} else if (val->type == kllist_t) {
				ss << "list(" << KASARRSIZE(val) << ")";
			} else if (val->type == klarray_t) {
				ss << "array(" << KASARRSIZE(val) << ")";
			} else if (val->type->toString) {
				auto str = val->type->toString(val);
				ss.write(KASSTR(str), KASSTRSIZE(str));
				klDeref(str);
			}
		} else {
			ss << "null";
		}
	}
	ss << ']';
	auto s = ss.tellp();
	auto buff = new char[s];
	ss.read(buff, s);

	// create the string
	auto str = KLCAST(kl_string, klIns(klstring_t));
	str->size = s;
	str->value = buff;

	return KLWRAP(str);
}

static KlObject *klist_addall(KlObject *, KlObject **argv, kbyte) {
	auto ptr = KLCAST(kl_sptr, argv[0]);
	auto list = KASLIST(argv[0]);
	KlObject **data = nullptr;
	ksize size = 0;
	if (argv[1]) {
		if (argv[1]->type == kllist_t) {
			size = KASARRSIZE(argv[1]);
			data = KASLIST(argv[1])->data();
		} else if (argv[1]->type == klarray_t) {
			size = KASARRSIZE(argv[1]);
			data = KASARR(argv[1]);
		} else {
			throw runtime_error("Invalid argument, expected array or list.");
		}
	}
	if (size > 0) {
		list->reserve(size);
		for (int i = 0; i < size; ++i) {
			KlObject *temp = nullptr;
			klCopy(data[i], &temp);
			list->push_back(temp);
		}
		ptr->size = list->size();
	}
	return nullptr;
}

static KlObject *klist_index(KlObject *, KlObject **argv, kbyte) {
	auto size = KASARRSIZE(argv[0]);
	auto list = KASLIST(argv[0]);
	auto obj = argv[1];
	for (auto j = 0; j < size; ++j) {
		auto comp = list->at(j);
		if (obj ? obj->type->equal(obj, comp) : !comp) {
			temp_int.value = j;
			break;
		}
	}
	return KLWRAP(&temp_int);
}

static KlObject *klist_lastindex(KlObject *, KlObject **argv, kbyte) {
	auto size = KASARRSIZE(argv[0]);
	auto list = KASLIST(argv[0]);
	auto obj = argv[1];
	for (long j = size - 1; j >= 0; --j) {
		auto comp = list->at(j);
		if (obj ? obj->type->equal(obj, comp) : !comp) {
			temp_int.value = j;
			break;
		}
	}
	return KLWRAP(&temp_int);
}

static KlObject *klist_contains(KlObject *s, KlObject **argv, kbyte argc) {
	// this call sets temp_int
	klist_index(s, argv, argc);
	return KLBOOL(temp_int.value != -1);
}

static bool objectComparer(KlObject *i1, KlObject *i2) {
	if (!i1) return true;
	if (!i2) return false;
	if (i1->type->comparer) {
		return i1->type->comparer(i1, i2) == 1;
	} else if (i2->type->comparer) {
		return i2->type->comparer(i2, i1) == -1;
	}
	return false;
}

static KlObject *klist_sort(KlObject *, KlObject **argv, kbyte) {
	auto list = KASLIST(argv[0]);
	std::sort(list->begin(), list->end(), objectComparer);
	return nullptr;
}

static KlObject *klist_clear(KlObject *, KlObject **argv, kbyte) {
	auto list = KASLIST(argv[0]);
	// we deref every object before clear
	for (const auto obj: *list) {
		klDeref(obj);
	}
	list->clear();
	return nullptr;
}

static KlObject *klist_trim(KlObject *, KlObject **argv, kbyte) {
	auto list = KASLIST(argv[0]);
	list->shrink_to_fit();
	return nullptr;
}

static KlObject *klist_capacity(KlObject *, KlObject **argv, kbyte) {
	auto list = KASLIST(argv[0]);
	return KLINT(list->capacity());
}

static KlObject *klist_insert(KlObject *, KlObject **argv, kbyte argc) {
	auto ptr = KLCAST(kl_sptr, argv[0]);
	auto list = KASLIST(argv[0]);
	kint index = 0;
	auto size = argc - 2;
	if (argv[1]) {
		GET_INT(index, argv[1])
		if (index < 0 || index > ptr->size) {
			throw runtime_error("Index out of bounds");
		}
	}
	// reserve required space
	list->reserve(size);
	// insert null values first
	list->insert(list->begin() + index, size, nullptr);

	for (int i = 0; i < size; ++i) {
		vector<KlObject *>::reference ref = list->at(i + index);
		klCopy(argv[2 + i], &ref);
	}
	ptr->size = list->size();
	return nullptr;
}

static KlObject *klist_insertall(KlObject *, KlObject **argv, kbyte) {
	auto ptr = KLCAST(kl_sptr, argv[0]);
	auto list = KASLIST(argv[0]);
	kint index = 0;
	KlObject **data = nullptr;
	ksize size = 0;
	if (argv[1]) {
		GET_INT(index, argv[1])
		if (index < 0 || index > ptr->size) {
			throw runtime_error("Index out of bounds");
		}
	}

	if (argv[2]) {
		if (argv[2]->type == kllist_t) {
			size = KASARRSIZE(argv[2]);
			data = KASLIST(argv[2])->data();
		} else if (argv[2]->type == klarray_t) {
			size = KASARRSIZE(argv[2]);
			data = KASARR(argv[2]);
		} else {
			throw runtime_error("Invalid argument, expected array or list.");
		}
	}
	if (size > 0) {
		list->reserve(size);
		// insert null values first
		list->insert(list->begin() + index, size, nullptr);
		for (int i = 0; i < size; ++i) {
			vector<KlObject *>::reference ref = list->at(i + index);
			klCopy(data[i], &ref);
		}
		ptr->size = list->size();
	}
	return nullptr;
}

static KlObject *klist_remove(KlObject *, KlObject **argv, kbyte) {
	auto ptr = KLCAST(kl_sptr, argv[0]);
	auto list = KASLIST(argv[0]);
	auto obj = argv[1];
	auto i = 0;
	for (auto ref: *list) {
		if (obj ? obj->type->equal(obj, ref) : !ref) {
			// derefence the element before remove from list
			klDeref(ref);
			list->erase(list->begin() + i);
			break;
		}
		i++;
	}
	ptr->size = list->size();
	return nullptr;
}

static KlObject *klist_removeat(KlObject *, KlObject **argv, kbyte) {
	auto ptr = KLCAST(kl_sptr, argv[0]);
	auto list = KASLIST(argv[0]);
	kint index = 0;
	if (argv[1]) {
		GET_INT(index, argv[1])
		if (index < 0 || index > ptr->size) {
			throw runtime_error("Index out of bounds");
		}
	}

	auto ref = list->begin() + index;
	// derefence the element before remove from list
	klDeref(*ref);
	list->erase(ref);
	ptr->size = list->size();
	return nullptr;
}

static KlObject *klist_toarray(KlObject *, KlObject **argv, kbyte) {
	auto ptr = KLCAST(kl_sptr, argv[0]);
	auto list = KASLIST(argv[0]);
	auto obj = KLARR(ptr->size);
	auto arr = KASARR(obj);

	for (int i = 0; i < ptr->size; ++i) {
		klCopy(list->at(i), &arr[i]);
	}

	return obj;
}

static void klist_add(KlObject *x, KlObject *y, KlObject **res) {
	auto a = KASLIST(x);
	if (y) {
		if (y->type == kllist_t) {
			auto b = KASLIST(y);
			auto obj = klIns(kllist_t);
			auto ptr = KLCAST(kl_sptr, obj);
			ptr->size = a->size() + b->size();
			auto list = new vector<KlObject *>(ptr->size);
			ptr->value = list;

			for (int i = 0; i < a->size(); ++i) {
				klCopy(a->at(i), &list->at(i));
			}
			for (int i = 0; i < b->size(); ++i) {
				klCopy(b->at(i), &list->at(i + a->size()));
			}

			klTransfer(&obj, res);
		} else {
			throw runtime_error("Can only add list to list");
		}
	} else {
		auto obj = klIns(kllist_t);
		auto ptr = KLCAST(kl_sptr, obj);
		ptr->size = a->size();
		auto list = new vector<KlObject *>(ptr->size);
		ptr->value = list;

		for (int i = 0; i < ptr->size; ++i) {
			klCopy(a->at(i), &list->at(i));
		}

		klTransfer(&obj, res);
	}
}

static void klist_mul(KlObject *x, KlObject *y, KlObject **res) {
	auto a = KASLIST(x);
	if (y) {
		if (y->type != klint_t && !y->type->toInt) throw runtime_error("can only multiply list with int");

		ksize size = 0;
		GET_INT(size, y);

		auto obj1 = klIns(kllist_t);
		auto ptr1 = KLCAST(kl_sptr, obj1);
		ptr1->size = a->size() * size;
		auto list = new vector<KlObject *>(ptr1->size);
		ptr1->value = list;
		if (ptr1->size > 0) {
			for (int i = 0; i < size; ++i) {
				for (int j = 0; j < a->size(); ++j) {
					klCopy(a->at(j), &list->at(j + (i * a->size())));
				}
			}
		}
		klTransfer(&obj1, res);
		return;
	} else {
		auto obj = klIns(kllist_t);
		auto ptr = KLCAST(kl_sptr, obj);
		ptr->value = new vector<KlObject *>();

		klTransfer(&obj, res);
	}
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
		KLWRAP(func),
		klist_tostr,
		REP7(nullptr)
		klist_add,
		REP1(nullptr)
		klist_mul,
		REP4(nullptr)
		nullptr,
		nullptr
	};
	KLTYPE_METADATA(kllist_t)

	ADD_FUNCTION(add, kllist_t, klist_add, 2, -1)
	ADD_FUNCTION(addall, kllist_t, klist_addall, 2, 2)
	ADD_FUNCTION(index, kllist_t, klist_index, 2, 2)
	ADD_FUNCTION(lastindex, kllist_t, klist_lastindex, 2, 2)
	ADD_FUNCTION(contains, kllist_t, klist_contains, 2, 2)
	ADD_FUNCTION(sort, kllist_t, klist_sort, 1, 1)
	ADD_FUNCTION(clear, kllist_t, klist_clear, 1, 1)
	ADD_FUNCTION(trim, kllist_t, klist_trim, 1, 1)
	ADD_FUNCTION(capacity, kllist_t, klist_capacity, 1, 1)
	ADD_FUNCTION(insert, kllist_t, klist_insert, 3, -1)
	ADD_FUNCTION(insertall, kllist_t, klist_insertall, 3, 3)
	ADD_FUNCTION(remove, kllist_t, klist_remove, 2, 2)
	ADD_FUNCTION(removeat, kllist_t, klist_removeat, 2, 2)
	ADD_FUNCTION(toarray, kllist_t, klist_toarray, 1, 1)
}