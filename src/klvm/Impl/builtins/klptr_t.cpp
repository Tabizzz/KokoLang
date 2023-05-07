#include "global.h"
#include <sstream> //for std::stringstream
#include <string>  //for std::string

KLType* klptr_t = nullptr;

static void kptr_init(KlObject *obj) {
	auto ptr = KLCAST(kl_ptr, obj);
	ptr->value = nullptr;
}

static KlObject *klptr_tostr(KlObject *base) {
	auto val = KASPTR(base);
	std::stringstream ss;
	ss << val;
	return KLSTR(ss.str());
}

void global_klptr_t()
{
	klptr_t = new KLType {
		.klbase = {
			.flags = KLOBJ_FLAG_USE_DELETE
		},
		.name = "ptr",
		.size = sizeof(kl_ptr),
		.initializer = kptr_init,
		.toString = klptr_tostr,
		KLTYPE_METADATA
	};
}