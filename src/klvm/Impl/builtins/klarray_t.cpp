#include "global.h"
#include <sstream>

KLType *klarray_t = nullptr;

void karr_init(KlObject *obj) {
	auto ptr = KLCAST(kl_sptr, obj);
	ptr->size = 0;
	ptr->value = nullptr;
}

void karr_end(KlObject *obj) {
	auto ptr = KLCAST(kl_sptr, obj);
	auto arr = KLCAST(KlObject*, ptr->value);
	if (arr) {
		// dereference each element on the array
		for (int i = 0; i < ptr->size; ++i) {
			klDeref(arr[i]);
		}
		delete[] arr;
	}
}

static KlObject *karr_tostr(KlObject *obj) {
	std::ostringstream ss;
	auto size = KASARRSIZE(obj);
	auto arr = KASARR(obj);
	ss << "(";
	if (size > 0) {
		for (int i = 0; i < size - 1; ++i) {
			auto val = arr[i];
			if (val) {
				if (val->type == klstring_t) {
					ss.write(KASSTR(val), KASSTRSIZE(val)) << ", ";
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
		auto val = arr[size - 1];
		if (val) {
			if (val->type == klstring_t) {
				ss.write(KASSTR(val), KASSTRSIZE(val));
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
	ss << ')';
	return KLSTR(ss.str());
}

void global_klarray_t() {
	klarray_t = new KLType{
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"array",
		0,
		sizeof(kl_sptr),
		karr_init,
		karr_end,
		nullptr,
		karr_tostr
	};
	KLTYPE_METADATA(klarray_t)
}