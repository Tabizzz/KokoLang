#pragma once
#include "klapi.h"

#define KLCAST(x,y) (x*)y;

KLAPI KlType* klBType_Int();

KLAPI KlType* klBType_Float();

KLAPI KlType* klBType_Bool();

KLAPI KlType* klBType_String();

KLAPI KlObject* klNew(KlType* type);

KLAPI void klDeref(KlObject* object);