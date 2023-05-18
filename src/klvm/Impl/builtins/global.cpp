#include "global.h"

#define ADD_TYPE(x) global_##x(); klDefType(x); klPackageRegType(dev, x);
#define ADD_FUNTIONS(x) global_##x##_m();

KLConfig klConfig;

KLPackage *kliBuildGlobalPackage() {
	auto dev = new KLPackage();
	dev->klbase.refs = 1;
	dev->klbase.flags |= KLOBJ_FLAG_USE_DELETE | KLOBJ_FLAG_NO_INSCOUNT | KLOBJ_FLAG_IGNORE_REF;

	// kltype_t and klpack_t must be init before all others.
	global_kltype_t();
	global_klpack_t();
	dev->klbase.type = klpack_t;
	klDefType(klpack_t);
	klpack_t->initializer(KLWRAP(dev));
	klPackageRegType(dev, kltype_t);
	klPackageRegType(dev, klpack_t);

	ADD_TYPE(klfloat_t);
	ADD_TYPE(klint_t);
	klDefType(klreg_t);
	klPackageRegType(dev, klreg_t);
	ADD_TYPE(klbool_t);
	ADD_TYPE(klstring_t);
	ADD_TYPE(klptr_t);
	ADD_TYPE(klarray_t);

	ADD_TYPE(klvar_t);
	ADD_TYPE(klfunc_t);
	ADD_TYPE(klinstruction_t);

	ADD_TYPE(kllist_t);
	ADD_TYPE(klmap_t);

	ADD_FUNTIONS(klstring_t)

	return dev;
}