#include "klvm_internal.h"
#include "klutil.h"

KLObject *klStringRepr(KLObject *val) {
	if (val) {
		if (val->type == klstring_t) {
			klRef(val);
			return val;
		} else if (val->type->KLConversionFunctions.toString) { // this is always true but just for more safety
			return val->type->KLConversionFunctions.toString(val);
		}
	}
	return KLSTR("null");
}