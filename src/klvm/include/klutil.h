#pragma once

#include "DataTypes/KLObject.h"

CAPI
/**
 * @brief Get the string representation of an object.
 *
 * This method always return a valid strong referenced object.
 */
KLObject *klStringRepr(KLObject *obj);

CAPI
/**
 * @brief This method test the boolean value of an object.
 *
 * null pointer return false.
 */
kbyte klTest(KLObject *object);

inline std::ostream &
operator<<(std::ostream &stream, KLObject *val) {
	if (val) {
		if (val->type->KLStreamFunctions.sRepr) {
			val->type->KLStreamFunctions.sRepr(val, &stream);
		} else if (val->type == klstring_t) {
			stream.write(KASSTR(val), KASSTRSIZE(val));
		} else if (val->type->KLConversionFunctions.toString) { // this is always true but just for more safety
			auto str = val->type->KLConversionFunctions.toString(val);
			stream.write(KASSTR(str), KASSTRSIZE(str));
			klDeref(str);
		}
	} else {
		stream.write("null", 4);
	}
	return stream;
}