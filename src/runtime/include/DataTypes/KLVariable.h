#pragma once

#include "DataTypes/KLObject.h"

#ifdef __cplusplus
extern "C" {
#endif

struct CPPAPI KLVariable
{
	KLOBJECTHEAD                            // base
	bool type;                              // the type of var, true is package variable, false is type variable.
	union {
		size_t offset;                      // for type variable
		KlObject* value;
	} data;
	KlObject* source;
	KlObject* defaultValue;
	MetaMap* metadata;

};

#ifdef __cplusplus
}
#endif

CAPI void klSetVariable(KLVariable* variable, KlObject* target, KlObject* value);

CAPI KlObject* klGetVariable(KLVariable* variable, KlObject* target);

CAPI void klRestoreVariable(KLVariable* variable, KlObject* target);

CAPI KLType klBType_Variable;