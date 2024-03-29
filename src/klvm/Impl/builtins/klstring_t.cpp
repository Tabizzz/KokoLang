#include "global.h"
#include <cstring>

#define STR_FUNC(x) func_##x->name->flags = KLOBJ_FLAG_NO_INSCOUNT; klstring_t->inscount--;

thread_local kl_string tmp_str = {
	KLObject{
		klstring_t,
		2,
		KLOBJ_FLAG_IGNORE_REF
	},
	nullptr,
	0
};

kl_string &temp_str() {
#ifdef WIN
	auto& dev = tmp_str;
	dev.klbase.type = klstring_t;
	return dev;
#else
	return tmp_str;
#endif
}

KLType *klstring_t = nullptr;

static void kstring_init(KLObject *obj) {
	auto ptr = KLCAST(kl_string, obj);
	ptr->value = nullptr;
	ptr->size = 0;
}

static void kstring_end(KLObject *obj) {
	auto ptr = KLCAST(kl_string, obj);
	delete[] ptr->value;
}

static int8_t kstring_compare(KLObject *x, KLObject *y) {
	auto first = KLCAST(kl_string, x);
	kl_string *second = nullptr;
	bool flag = false;
	if (y) {
		if (y->type == klstring_t) {
			second = KLCAST(kl_string, y);
		} else if (y->type->KLConversionFunctions.toString) {
			flag = true;
			second = KLCAST(kl_string, y->type->KLConversionFunctions.toString(y));
		}
	}
	if (!second) {
		if (first->size == 0) {
			// first is empty and second is null, we consider that case equal.
			return 0;
		} else {
			// first is not empty so is bigger than null.
			return -1;
		}
	}

	auto tmp = strncmp(second->value, first->value, min(first->size, second->size));
	int8_t dev = 0;
	if (tmp < 0) dev = -1;
	else if (tmp > 0) dev = 1;

	if (dev == 0 && first->size != second->size) {
		if (first->size < second->size)
			dev = 1;
		else
			dev = -1;
	}

	if (flag) {
		klDeref(KLWRAP(second));
	}
	return dev;
}

static int8_t kstring_equals(KLObject *x, KLObject *y) {
	auto first = KLCAST(kl_string, x);
	kl_string *second = nullptr;
	bool flag = false;
	if (y) {
		if (y->type == klstring_t) {
			second = KLCAST(kl_string, y);
		} else if (y->type->KLConversionFunctions.toString) {
			flag = true;
			second = KLCAST(kl_string, y->type->KLConversionFunctions.toString(y));
		}
	}
	if (!second) {
		if (first->size == 0) {
			// first is empty and second is null, we consider that case equal.
			return 1;
		} else {
			// first is not empty so is different from second.
			return 0;
		}
	}
	int8_t dev = false;

	if (first->size == second->size) {
		dev = strncmp(first->value, second->value, min(first->size, second->size)) == 0 ? 1 : 0;
	}

	if (flag) {
		klDeref(KLWRAP(second));
	}
	return dev;
}

static KLObject *kstring_add(KLObject *x, KLObject *y) {
	auto first = KLCAST(kl_string, x);
	kl_string *second = nullptr;
	bool flag = false;
	if (y) {
		if (y->type == klstring_t) {
			second = KLCAST(kl_string, y);
		} else if (y->type->KLConversionFunctions.toString) {
			flag = true;
			second = KLCAST(kl_string, y->type->KLConversionFunctions.toString(y));
		}
	}
	if (!second) {
		KLObject *tmp = nullptr;
		// second is null, so the resulting string is equals to first
		klClone(x, &tmp);
		return tmp;
	}

	if (first->size == 0) {
		KLObject *tmp = nullptr;
		// first is empty, so directly clone second
		klClone(KLWRAP(second), &tmp);
		if (flag) klDeref(KLWRAP(second));
		return tmp;
	} else if (second->size == 0) {
		KLObject *tmp = nullptr;
		// second is empty, so directly clone first
		klClone(x, &tmp);
		if (flag) klDeref(KLWRAP(second));
		return tmp;
	}

	auto size = first->size + second->size;
	auto value = new char[size];

	memcpy(value, (void *) first->value, first->size);
	memcpy(value + first->size, (void *) second->value, second->size);

	auto dev = klIns(klstring_t);
	KLCAST(kl_string, dev)->size = size;
	KLCAST(kl_string, dev)->value = value;

	if (flag) {
		klDeref(KLWRAP(second));
	}
	return dev;
}

static KLObject *kstring_clone(KLObject *obj) {
	return KLSTR(KSTRING(obj));
}

static inline kint
isSubstring(const char *s1, const char *s2, uint32_t M, uint32_t N, uint32_t start = 0, uint32_t e = UINT32_MAX) {
	if (M > N) return -1;
	auto end = min(e, N);

	for (auto i = start; i <= end - M; i++) {
		int j;

		/* For current index i, check for pattern match */
		for (j = 0; j < M; j++)
			if (s2[i + j] != s1[j])
				break;

		if (j == M)
			return i;
	}

	return -1;
}

static KLObject *kstring_find(KLObject *, KLObject **argv, kbyte argc) {
	auto str = KLCAST(kl_string, argv[0]);
	const char *find = "null";
	int32_t size = 4;
	uint32_t start = 0;
	uint32_t end = UINT32_MAX;
	KLObject *temp = nullptr;

	if (argc > 2 && argv[2]) { GET_INT(start, argv[2])}
	if (argc > 3 && argv[3]) { GET_INT(end, argv[3])}
	if (argv[1]) {
		if (argv[1]->type == klstring_t) {
			find = KASSTR(argv[1]);
			size = KASSTRSIZE(argv[1]);
		} else {
			temp = argv[1]->type->KLConversionFunctions.toString(argv[1]);
			find = KASSTR(temp);
			size = KASSTRSIZE(temp);
		}
	}
	temp_int().value = isSubstring(find, str->value, size, str->size, start, end);
	klDeref(temp);
	return KLWRAP(&temp_int());
}

static KLObject *kstring_contains(KLObject *s, KLObject **argv, kbyte argc) {
	// this call sets temp_int()
	kstring_find(s, argv, argc);
	return KLBOOL(temp_int().value != -1);
}

static KLObject *kstring_count(KLObject *, KLObject **argv, kbyte argc) {
	kint count = 0;
	auto str = KLCAST(kl_string, argv[0]);
	const char *find = "null";
	int32_t size = 4;
	uint32_t start = 0;
	uint32_t end = UINT32_MAX;
	KLObject *temp = nullptr;

	if (argc > 2 && argv[2]) { GET_INT(start, argv[2])}
	if (argc > 3 && argv[3]) { GET_INT(end, argv[3])}
	if (argv[1]) {
		if (argv[1]->type == klstring_t) {
			find = KASSTR(argv[1]);
			size = KASSTRSIZE(argv[1]);
		} else {
			temp = argv[1]->type->KLConversionFunctions.toString(argv[1]);
			find = KASSTR(temp);
			size = KASSTRSIZE(temp);
		}
	}
	kint i;
	while ((i = isSubstring(find, str->value, size, str->size, start, end)) != -1) {
		count++;
		start = i + max(1, size);
	}

	klDeref(temp);
	temp_int().value = count;
	return KLWRAP(&temp_int());
}

static KLObject *kstring_startswith(KLObject *, KLObject **argv, kbyte) {
	auto str = KLCAST(kl_string, argv[0]);
	const char *find = "null";
	int32_t size = 4;
	KLObject *temp = nullptr;

	if (argv[1]) {
		if (argv[1]->type == klstring_t) {
			find = KASSTR(argv[1]);
			size = KASSTRSIZE(argv[1]);
		} else {
			temp = argv[1]->type->KLConversionFunctions.toString(argv[1]);
			find = KASSTR(temp);
			size = KASSTRSIZE(temp);
		}
	}
	auto out = isSubstring(find, str->value, size, str->size, 0, size);
	klDeref(temp);
	return KLBOOL(out == 0);
}

static KLObject *kstring_endswith(KLObject *, KLObject **argv, kbyte) {
	auto str = KLCAST(kl_string, argv[0]);
	const char *find = "null";
	int32_t size = 4;
	KLObject *temp = nullptr;

	if (argv[1]) {
		if (argv[1]->type == klstring_t) {
			find = KASSTR(argv[1]);
			size = KASSTRSIZE(argv[1]);
		} else {
			temp = argv[1]->type->KLConversionFunctions.toString(argv[1]);
			find = KASSTR(temp);
			size = KASSTRSIZE(temp);
		}
	}
	auto out = isSubstring(find, str->value, size, str->size, str->size - size);
	klDeref(temp);
	return KLBOOL(out == 0);
}

static KLObject *kstring_lower(KLObject *, KLObject **argv, kbyte) {
	auto str = KLCAST(kl_string, argv[0]);
	auto ret = KLCAST(kl_string, klIns(klstring_t));
	ret->size = str->size;
	auto buff = new char[str->size];
	ret->value = buff;
	for (int i = 0; i < str->size; ++i) {
		buff[i] = static_cast<char>(tolower(str->value[i]));
	}
	return KLWRAP(ret);
}

static KLObject *kstring_upper(KLObject *, KLObject **argv, kbyte) {
	auto str = KLCAST(kl_string, argv[0]);
	auto ret = KLCAST(kl_string, klIns(klstring_t));
	ret->size = str->size;
	auto buff = new char[str->size];
	ret->value = buff;
	for (int i = 0; i < str->size; ++i) {
		buff[i] = static_cast<char>(toupper(str->value[i]));
	}
	return KLWRAP(ret);
}

static KLObject *kstring_capitalize(KLObject *, KLObject **argv, kbyte) {
	auto str = KLCAST(kl_string, argv[0]);
	auto ret = KLCAST(kl_string, klIns(klstring_t));
	ret->size = str->size;
	auto buff = new char[str->size];
	ret->value = buff;
	if (ret->size > 0) {
		buff[0] = static_cast<char>(toupper(str->value[0]));
	}
	for (int i = 1; i < str->size; ++i) {
		buff[i] = static_cast<char>(tolower(str->value[i]));
	}
	return KLWRAP(ret);
}

static KLObject *kstring_split(KLObject *, KLObject **argv, kbyte argc) {
	auto str = KLCAST(kl_string, argv[0]);
	const char *sep = " ";
	int32_t size = 1;
	KLObject *temp = nullptr;
	uint32_t start = 0;
	if (argc > 1 && argv[1]) {
		if (argv[1]->type == klstring_t) {
			sep = KASSTR(argv[1]);
			size = KASSTRSIZE(argv[1]);
		} else {
			temp = argv[1]->type->KLConversionFunctions.toString(argv[1]);
			sep = KASSTR(temp);
			size = KASSTRSIZE(temp);
		}
	}
	auto list = KLCAST(kl_sptr, klIns(kllist_t));
	auto vec = new vector<KLObject *>();
	list->value = vec;

	kint i;
	while ((i = isSubstring(sep, str->value, size, str->size, start)) != -1) {
		auto ns = i - start;
		if (ns > 0) {
			auto split = KLCAST(kl_string, klIns(klstring_t));
			split->size = ns;
			auto buff = new char[ns];
			split->value = buff;
			memcpy(buff, str->value + start, ns);

			vec->push_back(KLWRAP(split));
			list->size++;
		}
		start = i + max(1, size);
	}
	// insert the last string on the list
	if (str->size > start) {
		auto ns = str->size - start;
		auto split = KLCAST(kl_string, klIns(klstring_t));
		split->size = ns;
		auto buff = new char[ns];
		split->value = buff;
		memcpy(buff, str->value + start, ns);

		vec->push_back(KLWRAP(split));
		list->size++;
	}

	klDeref(temp);
	vec->shrink_to_fit();
	return KLWRAP(list);
}

void global_klstring_t_m() {
	ADD_FUNCTION(find, klstring_t, kstring_find, 2, 4)
	STR_FUNC(find)
	ADD_FUNCTION(contains, klstring_t, kstring_contains, 2, 4)
	STR_FUNC(contains)
	ADD_FUNCTION(count, klstring_t, kstring_count, 2, 4)
	STR_FUNC(count)
	ADD_FUNCTION(startswith, klstring_t, kstring_startswith, 2, 2)
	STR_FUNC(startswith)
	ADD_FUNCTION(endswith, klstring_t, kstring_endswith, 2, 2)
	STR_FUNC(endswith)
	ADD_FUNCTION(lower, klstring_t, kstring_lower, 1, 1)
	STR_FUNC(lower)
	ADD_FUNCTION(upper, klstring_t, kstring_upper, 1, 1)
	STR_FUNC(upper)
	ADD_FUNCTION(capitalize, klstring_t, kstring_capitalize, 1, 1)
	STR_FUNC(capitalize)
	ADD_FUNCTION(split, klstring_t, kstring_split, 1, 2)
	STR_FUNC(split)
}

void global_klstring_t() {
	klstring_t = new KLType{
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"str",
		sizeof(kl_string),
		0, 0,
		kstring_init,
		kstring_end,
		nullptr,
		{klself_return
		},
		kstring_compare,
		kstring_equals,
		{kstring_add
		},
		kstring_clone
	};
	KLTYPE_METADATA(klstring_t)
}