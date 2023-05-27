#include "klvm_internal.h"


void kliCreateMetaType() {
	klState->metaType = new KLType{
		{
			kltype_t,
			2,
			KLOBJ_FLAG_IGNORE_REF | KLOBJ_FLAG_CONST
		},
		"",
		0, 0, 0,
		{},
		{

		}
	};
}