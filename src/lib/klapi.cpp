
#include "KokoLangInternal.h"

CAPI KLProgram* klCreateProgram()
{
	return new KLProgram();
}

CAPI void klDestroyProgram(KLProgram* obj) {
	delete obj;
}
