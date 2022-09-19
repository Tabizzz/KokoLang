#pragma once

#include "KLImports.h"
#include "Runtime/KLFunction.h"
#include "Runtime/KLInstruction.h"

class CPPAPI KLPackage
{
public:
	~KLPackage();
	void Build();
	static int Run(const KLPackage* program);

	vector<KLFunction*> functions;
private:
	bool build;
};
