#pragma once
#include "klapi.h"

#define KLCAST(x,y) (x*)y;

CAPI KlType* klBType_Int();

CAPI KlType* klBType_Float();

CAPI KlType* klBType_Bool();

CAPI KlType* klBType_String();

CAPI KlObject* klNew(KlType* type);

CAPI void klDeref(KlObject* object);