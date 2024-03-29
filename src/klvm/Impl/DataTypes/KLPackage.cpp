﻿#include "../klvm_internal.h"
#include "DataTypes/KLPackage.h"
#include <iostream>

static void kpack_init(KLObject* pack)
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

static void kpack_end(KLObject* pack)
{
	auto ins = KLCAST(KLPackage, pack);
	klDeref(ins->name);
	kliDerefAndDeleteMap(ins->functions);

	kliDerefAndDeleteMap(ins->variables);
	kliDerefAndDeleteMap(ins->packs);
	kliDerefAndDeleteMap(ins->metadata);
	kliDerefAndDeleteMap(ins->types);
}

CAPI KLPackage* klCreatePackage()
{
	auto ins = KLCAST(KLPackage, klIns(klpack_t));
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
		for (const auto& func: *type->functions) {
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

static inline KLObject** argstoobject(const char **pString, int i)
{
	auto ret = new KLObject* [i];

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
			auto dev = 0;
			auto args = argstoobject(argv, argc);
			auto ret = klInvoke(find->second, args, argc);

			// release allocated args
			for (int i = 0; i < argc; ++i) {
				klDeref(args[i]);
			}
			delete[] args;
			if(ret && ret->type == klint_t) dev = KASINT(ret);
			if(ret && ret->type == klbool_t && !KASBOOL(ret)) dev = EXIT_FAILURE;
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
		klPackage->types->insert(pair<string, KLObject*>(name, KLWRAP(type)));
		return;
	}
	throw invalid_argument("trying to define a type but another type with the same name already exists");
}

KLType* klpack_t = nullptr;

void global_klpack_t()
{
	klpack_t = new KLType {
		{
			nullptr,
			0,
			KLOBJ_FLAG_USE_DELETE
		},
		"pack",
		 sizeof(KLPackage),
		 0, 0,
		kpack_init,
		kpack_end,
	};
	KLTYPE_METADATA(klpack_t)
}
