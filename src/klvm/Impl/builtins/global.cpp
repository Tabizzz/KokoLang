#include "global.h"

#define ADD_TYPE(x) global_##x(); klDefType(x); klPackageRegType(dev, x);

KLPackage* kliBuildGlobalPackage() {
	auto dev = new KLPackage();
	dev->klbase.refs = 1;
	dev->klbase.type = &klpack_t;
	klpack_t.initializer(KLWRAP(dev));
	dev->klbase.flags |= KLOBJ_FLAG_USE_DELETE | KLOBJ_FLAG_NO_INSCOUNT;


	global_kltype_t();
	klPackageRegType(dev, kltype_t);

	ADD_TYPE(klfloat_t);
	ADD_TYPE(klint_t);

	return dev;
}