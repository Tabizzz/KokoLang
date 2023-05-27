#pragma once

#include "DataTypes/KLObject.h"

CAPI
KLObject *klStringRepr(KLObject *obj);

template<typename Traits>
inline std::basic_ostream<char, Traits>&
operator<<(std::basic_ostream<char, Traits>& stream, KLObject* val)
{
	if (val) {
		if (val->type == klstring_t) {
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