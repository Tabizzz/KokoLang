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

kbyte klTest(KLObject *obj) {
	if (!obj) return false;
	// builtin types are inlined here to prevent an unnecessary call to toBool.
	if (obj->type == klbool_t) {
		return KASBOOL(obj);
	} else if (obj->type == klstring_t) {
		return KASSTRSIZE(obj);
	} else if (obj->type == klint_t) {
		return KASINT(obj);
	} else if (obj->type == klfloat_t) {
		return KASFLOAT(obj);
	} else if (obj->type->KLConversionFunctions.toBool) {
		return KASBOOL(obj->type->KLConversionFunctions.toBool(obj));
	}
	return true;
}
