#include "../klvm_internal.h"
#include "global.h"

KLPackage* kliBuildGlobalPackage() {
	auto dev = new KLPackage();
	dev->klbase.refs = 1;
	dev->klbase.type = &klpack_t;
	klpack_t.initializer(KLWRAP(dev));
	dev->klbase.flags |= KLOBJ_FLAG_USE_DELETE | KLOBJ_FLAG_NO_INSCOUNT;
	return dev;
}