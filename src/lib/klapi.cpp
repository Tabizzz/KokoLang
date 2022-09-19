
#include "KokoLangInternal.h"

CAPI KLPackage* klCreateProgram()
{
	return new KLPackage();
}

CAPI void klDestroyProgram(KLPackage* obj) {
	delete obj;
}
