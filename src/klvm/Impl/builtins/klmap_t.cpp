#include <map>
#include <sstream>
#include "global.h"

#define KASMAP(x) KLCAST(map_t , KLCAST(kl_sptr, x)->value)

/**
 * @brief Evaluates if two object are different
 */
struct KLObjectComparer {
	bool operator()(KLObject *i1, KLObject *i2) const {
		if (!i1) return i2;
		if (!i2) return true;
		return i1->type->KLComparerFunctions.equal(i1, i2) == 0;
	}
};

typedef map<KLObject *, KLObject *, KLObjectComparer> map_t;
typedef pair<KLObject *, KLObject *> pair_t;

KLType *klmap_t = nullptr;

static KLObject *kmap_ctor(KLObject *, KLObject **argv, kbyte) {
	auto ptr = KLCAST(kl_sptr, argv[0]);
	ptr->value = new map_t();
	return nullptr;
}

static void kmap_end(KLObject *obj) {
	auto map = KASMAP(obj);
	if (map) {
		for (auto pair: *map) {
			klDeref(pair.first);
			klDeref(pair.second);
		}
		delete map;
	}
}

static KLObject *kmap_tostr(KLObject *obj) {
	std::stringstream ss;
	auto size = KASARRSIZE(obj);
	auto map = KASMAP(obj);
	ss << '{';

	for (auto pair: *map) {
		auto val = pair.first;

		if (val->type == klstring_t) {
			ss << '"';
			ss.write(KASSTR(val), KASSTRSIZE(val)) << "\": ";
		} else if (val->type == kllist_t) {
			ss << "list(" << KASARRSIZE(val) << "): ";
		} else if (val->type == klarray_t) {
			ss << "array(" << KASARRSIZE(val) << "): ";
		} else if (val->type->KLConversionFunctions.toString) {
			auto str = val->type->KLConversionFunctions.toString(val);
			ss.write(KASSTR(str), KASSTRSIZE(str)) << ": ";
			klDeref(str);
		}

		val = pair.second;
		if (val) {
			if (val->type == klstring_t) {
				ss << '"';
				ss.write(KASSTR(val), KASSTRSIZE(val)) << "\", ";
			} else if (val->type == kllist_t) {
				ss << "list(" << KASARRSIZE(val) << "), ";
			} else if (val->type == klarray_t) {
				ss << "array(" << KASARRSIZE(val) << "), ";
			} else if (val->type->KLConversionFunctions.toString) {
				auto str = val->type->KLConversionFunctions.toString(val);
				ss.write(KASSTR(str), KASSTRSIZE(str)) << ", ";
				klDeref(str);
			}
		} else {
			ss << "null, ";
		}
	}

	ss << '}';
	auto s = ss.tellp();
	auto buff = new char[s];
	ss.read(buff, s);

	// create the string
	auto str = KLCAST(kl_string, klIns(klstring_t));
	str->size = s;
	str->value = buff;

	return KLWRAP(str);
}

/**
 * @brief Add a key value pair to the map, if the map already contains the key nothing happens.
 */
static KLObject *kmap_add(KLObject *, KLObject **argv, kbyte) {
	auto ptr = KLCAST(kl_sptr, argv[0]);
	auto map = KASMAP(argv[0]);
	auto key = argv[1];
	auto value = argv[2];
	if (!key || map->find(key) != map->end()) {
		return KLBOOL(false);
	}

	KLObject *tkey = nullptr;
	KLObject *tval = nullptr;

	// copy key and value
	klCopy(key, &tkey);
	klCopy(value, &tval);

	map->insert(pair_t(tkey, tval));

	ptr->size = map->size();
	return KLBOOL(true);
}

static KLObject *kmap_clear(KLObject *, KLObject **argv, kbyte) {
	auto ptr = KLCAST(kl_sptr, argv[0]);
	auto map = KASMAP(argv[0]);
	// we deref every object before clear
	for (auto pair: *map) {
		klDeref(pair.first);
		klDeref(pair.second);
	}
	map->clear();
	ptr->size = map->size();
	return nullptr;
}

static KLObject *kmap_contains(KLObject *, KLObject **argv, kbyte) { return nullptr; }

static KLObject *kmap_haskey(KLObject *, KLObject **argv, kbyte) { return nullptr; }

static KLObject *kmap_remove(KLObject *, KLObject **argv, kbyte) { return nullptr; }

void global_klmap_t() {
	auto func = KLCAST(KLFunction, klIns(klfunc_t));
	func->external = true;
	func->name = KLSTR(".ctor");
	func->invokable = kmap_ctor;
	func->margs = 1;
	func->args = 1;

	klmap_t = new KLType{
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"map",
		sizeof(kl_sptr),
		0, 0,
		karr_init,
		kmap_end,
		KLWRAP(func),
		kmap_tostr
	};

	KLTYPE_METADATA(klmap_t)

	ADD_FUNCTION(add, klmap_t, kmap_add, 3, 3)
	ADD_FUNCTION(clear, klmap_t, kmap_clear, 1, 1)
	ADD_FUNCTION(contains, klmap_t, kmap_contains, 2, 2)
	ADD_FUNCTION(haskey, klmap_t, kmap_haskey, 2, 2)
	ADD_FUNCTION(remove, klmap_t, kmap_remove, 2, 2)
}