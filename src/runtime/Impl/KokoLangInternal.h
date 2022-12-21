#pragma once
#include "KokoLang.h"
#include "DataTypes/KLFunctionImpl.h"
using namespace std;

#define KLISNATIVE(x) (x->type == &klBType_Int ? 1 : x->type == &klBType_Float ? 2 : x->type == &klBType_Bool ? 3 : 0)