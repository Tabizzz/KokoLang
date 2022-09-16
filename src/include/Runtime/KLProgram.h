#pragma once

#include "KLImports.h"
#include "Runtime/KLFunction.h"
#include "Runtime/KLInstruction.h"

class CPPAPI KLProgram
{
public:
	~KLProgram();
	void Build();
	static int Run(const KLProgram* program);

	vector<KLFunction*> functions;
private:
	bool build;
};
