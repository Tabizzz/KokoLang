#pragma once
#include "klvm.h"
#include "DataTypes/KLFunctionImpl.h"
#include "klresolverImpl.h"

using namespace std;

#define KLISNATIVE(x) (x->type == &klBType_Int ? 1 : x->type == &klBType_Float ? 2 : x->type == &klBType_Bool ? 3 : 0)

void kliDerefAndDeleteMap(MetaMap* pMap);

map<string, KLPackage*>* kliRootPackages();