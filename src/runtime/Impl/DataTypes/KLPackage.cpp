#include <iostream>
#include "../KokoLangInternal.h"
#include "DataTypes/KLPackage.h"

void kpack_init(KlObject* pack)
{
	auto ins = KLCAST(KLPackage, pack);
	ins->name = nullptr;
	ins->functions 	= new map<string, KlObject *>();
	ins->variables 	= new map<string, KlObject *>();
	ins->types 		= new map<string, KlObject *>();
	ins->subpacks	= new map<string, KlObject *>();
	ins->metadata	= new map<string, KlObject *>();
}

void kliDerefAndDeleteMap(map<std::string, KlObject *>* pMap)
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
	kliDerefAndDeleteMap(ins->functions);
	kliDerefAndDeleteMap(ins->variables);
	kliDerefAndDeleteMap(ins->subpacks);
	kliDerefAndDeleteMap(ins->metadata);
	//types are no destroyed
	delete ins->types;
}

CAPI KLPackage* klCreatePackage()
{
	auto ins = KLCAST(KLPackage, klIns(&klBType_Package));
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

inline KlObject** argstoobject(const char **pString, int i)
{
	auto ret = new KlObject* [i];

	for (int j = 0; j < i; ++j) {
		ret[j] = KLSTR(pString[j]);
	}

	return ret;
}

CAPI int klPackage_Run(KLPackage* program, int argc, const char* argv[]) {
	if (program)
	{
		KLFunction* main;
		auto find = program->functions->find("main");
		if(find != program->functions->end())
		{
			main = KLCAST(KLFunction, find->second);
			auto args = argstoobject(argv, argc);
			auto kargc = KLINT(argc);
			main->invokable(find->second, args, kargc);
			// release allocated args
			klDeref(kargc);
			for (int i = 0; i < argc; ++i) {
				klDeref(args[i]);
			}
			delete[] args;

			return 0;
		}
		cout<<"Error: unable to find entry point 'main'" << endl;
		return 1;
	}
	return 1;
}

KLType klBType_Package =
{
		KlObject(),
		"pack",
		0,
		sizeof(KLPackage),
		kpack_init,
		nullptr,
		kpack_end
};

void klPackageRegType(KLPackage *klPackage, KLType *type) {
	string name = type->name;

	auto find = klPackage->types->find(name);
	if(find == klPackage->types->end())
	{
		klPackage->types->insert(pair<string, KlObject*>(name, KLWRAP(type)));
		return;
	}
	throw invalid_argument("trying to define a type but another type with the same name already exists");
}
