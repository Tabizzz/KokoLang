#pragma once
#include "KokoLang.h"
#include "RuntimeImpl/KLFunctionImpl.h"
using namespace std;

#define KLISNATIVE(x) (x->type == &klBType_Int ? 1 : x->type == &klBType_Float ? 2 : 0)