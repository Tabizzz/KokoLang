#include <iostream>
#include "../klvm_internal.h"
#include "DataTypes/KLPackage.h"

void kpack_init(KlObject* pack)
{
	auto ins = KLCAST(KLPackage, pack);
	ins->name = nullptr;
	ins->functions 	= new MetaMap();
	ins->variables 	= new MetaMap();
	ins->types 		= new MetaMap();
	ins->packs		= new MetaMap();
	ins->metadata	= new MetaMap();
}

void kliDerefAndDeleteMap(MetaMap* pMap)
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

	/*
	 * Look for all defined variables on this package and deref all.
	 */
	for (const auto& var : *ins->variables) {
		auto variable = KLCAST(KLVariable, var.second);
		if(variable->data.packvar.defined)
		{
			klDeref(variable->data.packvar.value);
		}
	}

	kliDerefAndDeleteMap(ins->variables);
	kliDerefAndDeleteMap(ins->packs);
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

CAPI void klBuildPackage(KLPackage *klPackage, kbyte recursive) // NOLINT(misc-no-recursion)
{
	// Build package functions
	for (const auto& func: *klPackage->functions) {
		klBuildFunction(klPackage, nullptr, KLCAST(KLFunction, func.second));
	}

	// build functions in types
	for (const auto& typer: *klPackage->types) {
		auto type = KLCAST(KLType, typer.second);
		for (const auto& func: type->functions) {
			klBuildFunction(klPackage, type, KLCAST(KLFunction, func.second));
		}
	}

	if (recursive) {
		// Build subpackages
		for (const auto& pack: *klPackage->packs) {
			klBuildPackage(KLCAST(KLPackage, pack.second), true);
		}
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

CAPI int klRunPackage(KLPackage* klPackage, int argc, const char* argv[]) {
	if (klPackage)
	{
		KLFunction* main;
		auto find = klPackage->functions->find("main");
		if(find != klPackage->functions->end())
		{
			main = KLCAST(KLFunction, find->second);
			auto dev = 0;
			auto args = argstoobject(argv, argc);
			auto ret = main->invokable(find->second, args, argc);

			// release allocated args
			for (int i = 0; i < argc; ++i) {
				klDeref(args[i]);
			}
			delete[] args;
			if(ret && ret->type == &klBType_Int) dev = KASINT(ret);
			if(ret && ret->type == &klBType_Bool && !KASBOOL(ret)) dev = EXIT_FAILURE;
			klDeref(ret);
			return dev;
		}
		cout<<"Error: unable to find entry point 'main'" << endl;
		return 1;
	}
	return 1;
}

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

KLType klBType_Package = KLBASIC_TYPE("pack", KLPackage, kpack_init, kpack_end)};
