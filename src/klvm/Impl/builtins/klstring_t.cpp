#include <cstring>
#include "global.h"

KLType *klstring_t = nullptr;

static void kstring_init(KlObject *obj) {
	auto ptr = KLCAST(kl_string, obj);
	ptr->value = nullptr;
	ptr->size = 0;
}

static void kstring_end(KlObject *obj) {
	auto ptr = KLCAST(kl_string, obj);
	delete[] ptr->value;
}

static int8_t kstring_compare(KlObject *x, KlObject *y) {
	auto first = KLCAST(kl_string, x);
	kl_string *second = nullptr;
	bool flag = false;
	if (y) {
		if (y->type == klstring_t) {
			second = KLCAST(kl_string, y);
		} else if (y->type->toString) {
			flag = true;
			second = KLCAST(kl_string, y->type->toString(y));
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

	int8_t dev = strncmp(first->value, second->value, min(first->size, second->size)) * -1; // NOLINT(cppcoreguidelines-narrowing-conversions)

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

static int8_t kstring_equals(KlObject *x, KlObject *y) {
	auto first = KLCAST(kl_string, x);
	kl_string *second = nullptr;
	bool flag = false;
	if (y) {
		if (y->type == klstring_t) {
			second = KLCAST(kl_string, y);
		} else if (y->type->toString) {
			flag = true;
			second = KLCAST(kl_string, y->type->toString(y));
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

static void kstring_add(KlObject *x, KlObject *y, KlObject **target) {
	auto first = KLCAST(kl_string, x);
	kl_string *second = nullptr;
	bool flag = false;
	if (y) {
		if (y->type == klstring_t) {
			second = KLCAST(kl_string, y);
		} else if (y->type->toString) {
			flag = true;
			second = KLCAST(kl_string, y->type->toString(y));
		}
	}
	if (!second) {
		// second is null, so the resulting string is equals to first
		klClone(x, target);
		return;
	}

	if (first->size == 0) {
		// first is empty, so directly clone second
		klClone(KLWRAP(second), target);
		if (flag) klDeref(KLWRAP(second));
		return;
	} else if (second->size == 0) {
		// second is empty, so directly clone first
		klClone(x, target);
		if (flag) klDeref(KLWRAP(second));
		return;
	}

	auto size = first->size + second->size;
	auto value = new char[size];
	value[0] = 0;
	value[first->size] = 0;

	strncat(value, first->value, first->size);
	strncat(value, second->value, second->size);

	auto dev = klIns(klstring_t);
	KLCAST(kl_string, dev)->size = size;
	KLCAST(kl_string, dev)->value = value;
	// dereference any current value
	klDeref(*target);
	*target = dev;

	if (flag) {
		klDeref(KLWRAP(second));
	}
}

static KlObject *kstring_clone(KlObject *obj) {
	return KLSTR(KSTRING(obj));
}

void global_klstring_t() {
	klstring_t = new KLType{
		{
			.flags = KLOBJ_FLAG_USE_DELETE
		},
		"str",
		0,
		sizeof(kl_string),
		kstring_init,
		nullptr,
		kstring_end,
		klself_return,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		kstring_compare,
		kstring_equals,
		kstring_add,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		kstring_clone,
		nullptr,
		0,
		new MetaMap(),
		new MetaMap(),
		new MetaMap(),
	};
}