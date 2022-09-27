
#include "KokoLangInternal.h"
#include "klapi.h"
#include <stdexcept>

#define  STDREGTYPE(x) klDefType(&x); klPackageRegType(stdPackage, &x);

static KLPackage* stdPackage = nullptr;
static map<string, KLPackage*>* packages;

void kliBuildStdLib()
{
	stdPackage = klCreatePackage();
	klDeref(stdPackage->name);
	stdPackage->name = KLSTR("klstd");
}

CAPI void klInit()
{
	static_assert(sizeof(KLCAST(kl_int, nullptr)->value) <= sizeof(KLCAST(kl_float, nullptr)->value), "kl_int and kl_float dont have the same size.");

	kliBuildStdLib();

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

	packages = new map<string, KLPackage*>();

	klRegisterPackage(stdPackage);
}

CAPI void klEnd() {

}

CAPI KLPackage *klStdPackage() {
	return nullptr;
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

CAPI inline void klDefType(KlType *type) {
	// set the type
	type->klbase.type = &klBType_Type;
	// increase the instance count of type
	klBType_Type.inscount ++;
	// the type is defined, so it only have one ref.
	type->klbase.refs = 1;
}
