#include <iostream>
#include "KokoLangInternal.h"
#include "Runtime/KLPackage.h"

/*

int KLPackage::Run(const KLPackage* program)
{

}
*/

void kpack_init(KlObject* pack)
{
	auto ins = KLCAST(KLPackage, pack);
	ins->name = nullptr;
	ins->functions 	= new map<string, KlObject *>();
	ins->variables 	= new map<string, KlObject *>();
	ins->types 		= new map<string, KlObject *>();
	ins->subpacks	= new map<string, KlObject *>();
}

void uDerefAndDelete(map<std::string, KlObject *>* pMap)
{
	for (const auto& item: *pMap) {
		klDeref(item.second);
	}

	delete pMap;
}

void kpack_end(KlObject* pack)
{
	auto ins = KLCAST(KLPackage, pack);
	klDeref(ins->name);
	uDerefAndDelete(ins->functions);
	uDerefAndDelete(ins->variables);
	uDerefAndDelete(ins->types);
	uDerefAndDelete(ins->subpacks);
}

CAPI KLPackage* klCreatePackage()
{
	auto ins = KLCAST(KLPackage, klIns(&klBType_Package));
	// temporal name
	ins->name = KLSTR("+program");
	return ins;
}

CAPI void klDestroyPackage(KLPackage *klPackage)
{
	klDestroy(KLWRAP(klPackage));
}

CAPI void klPackage_Build(KLPackage *klPackage)
{
	for (const auto& func: *klPackage->functions) {
		klFunction_reallocateLabels(KLCAST(KLFunction, func.second));
	}
}

CAPI int klPackage_Run(KLPackage *program, int argc, const char **argv) {
	if (program)
	{
		KLFunction* main;
		for (const auto& func: *program->functions) {
			if(func.first == "main")
			{
				main = KLCAST(KLFunction, func.second);
				break;
			}
		}
		if(main)
		{

			return 0;
		}
		cout<<"Error: unable to find entry point 'main'" << endl;
		return 1;
	}
	return 1;
}

KlType klBType_Package =
{
		KlObject(),
		"pack",
		0,
		sizeof(KLPackage),
		kpack_init,
		nullptr,
		kpack_end
};

void klPackageRegType(KLPackage *klPackage, KlType *type) {
	string name = type->name;

	auto find = klPackage->types->find(name);
	if(find == klPackage->types->end())
	{
		klPackage->types->insert(pair<string, KlObject*>(name, KLWRAP(type)));
		return;
	}
	throw invalid_argument("trying to define a type but another type with the same name already exists");
}
