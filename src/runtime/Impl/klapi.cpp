
#include "KokoLangInternal.h"
#include "klapi.h"
#include <stdexcept>
#include <iostream>

#define  STDREGTYPE(x) klDefType(&x); klPackageRegType(globalPackage, &x);
#define STDCHECKTYPE(x) if(x.inscount) cout << "type "<< x.name << " still has " << x.inscount << " instances in memory" << endl;

static KLPackage* globalPackage = nullptr;
static map<string, KLPackage*>* packages;

void kliBuildGlobalPackage()
{
	globalPackage = klCreatePackage();
	globalPackage->name = KLSTR("global");
}

CAPI void klInit()
{
	static_assert(sizeof(KLCAST(kl_int, nullptr)->value) == sizeof(KLCAST(kl_float, nullptr)->value), "kl_int and kl_float dont have the same size.");

	kliBuildGlobalPackage();

	// the ref count of types is always 1, and you should never call
	// deref with a type as this will try to destroy it but types are
	// not freeable

	// first init the base type.
	klBType_Type.klbase.refs = 1;
	klBType_Type.klbase.type = &klBType_Type;
	klBType_Type.inscount = 1;

	// define builtin types
	STDREGTYPE(klBType_Int)
	STDREGTYPE(klBType_Float)
	STDREGTYPE(klBType_Bool)
	STDREGTYPE(klBType_String)
	STDREGTYPE(klBType_Ptr)
	STDREGTYPE(klBType_OPtr)
	STDREGTYPE(klBType_Arr)
	STDREGTYPE(klBType_Reg)

	// define runtime specific types
	STDREGTYPE(klBType_Instruction)
	STDREGTYPE(klBType_Func)
	STDREGTYPE(klBType_Package)
	STDCHECKTYPE(klBType_Variable)

	packages = new map<string, KLPackage*>();

	klRegisterPackage(globalPackage);
}

CAPI void klEnd() {
	for (const auto& pack: *packages) {
		klDestroyPackage(pack.second);
	}
	delete packages;

	// check instance counts
	STDCHECKTYPE(klBType_Int)
	STDCHECKTYPE(klBType_Float)
	STDCHECKTYPE(klBType_Bool)
	STDCHECKTYPE(klBType_String)
	STDCHECKTYPE(klBType_Ptr)
	STDCHECKTYPE(klBType_OPtr)
	STDCHECKTYPE(klBType_Arr)
	STDCHECKTYPE(klBType_Reg)

	// define runtime specific types
	STDCHECKTYPE(klBType_Instruction)
	STDCHECKTYPE(klBType_Func)
	STDCHECKTYPE(klBType_Package)
	STDCHECKTYPE(klBType_Variable)
}

CAPI KLPackage* klGlobalPackage() {
	return globalPackage;
}

CAPI void klRegisterPackage(KLPackage *klPackage) {
	string name = KLCAST(kl_string, klPackage->name)->value;
	auto find = packages->find(name);
	if(find == packages->end())
	{
		packages->insert(pair<string, KLPackage*>(name, klPackage));
		return;
	}
	throw invalid_argument("trying to load a package but another package with the same name already exists");
}

CAPI void klDefType(KLType *type) {
	// set the type
	type->klbase.type = &klBType_Type;
	// increase the instance count of type
	klBType_Type.inscount ++;
	// the type is defined, so it only have one ref.
	type->klbase.refs = 1;
}

void inline kliCopyA(KlObject *pObject, KlObject **pObject1) {
	klDeref(*pObject1);
	*pObject1 = nullptr;
}

void inline kliCopyB(KlObject *pObject, KlObject **pObject1) {
	if(pObject->type->clone) {
		*pObject1 = pObject->type->clone(pObject);
	} else {
		klRef(pObject);
		*pObject1 = pObject;
	}
}

void inline kliCopyD(KlObject *pObject, KlObject **pObject1) {
	if((pObject->type == (*pObject1)->type) && pObject->type->copy) {
		pObject->type->copy(pObject, *pObject1);
		return;
	} else if((pObject->type != (*pObject1)->type) && pObject->type->clone) {
		klDeref(*pObject1);
		*pObject1 = pObject->type->clone(pObject);
		return;
	}
	klRef(pObject);
	klDeref(*pObject1);
	*pObject1 = pObject;
}

CAPI void klCopy(KlObject *src, KlObject **dest) {
	if(!src && *dest) {
		kliCopyA(src, dest);
	} else if (src && !*dest) {
		kliCopyB(src, dest);
	} else if(src && *dest) {
		kliCopyD(src, dest);
	}
}

CAPI void klClone(KlObject *src, KlObject **dest) {
	if(!src && *dest) {
		kliCopyA(src, dest);
	} else if (src && !*dest) {
		kliCopyB(src, dest);
	} else if(src && *dest) {
		klRef(src);
		klDeref(*dest);
		*dest = src;
	}
}

CAPI void klMove(KlObject *src, KlObject **dest) {
	klDeref(*dest);
	klRef(src);
	*dest = src;
}
