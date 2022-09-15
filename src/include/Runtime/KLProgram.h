#pragma once

#include "KLImports.h"
#include "Runtime/KLFunction.h"
#include "Runtime/KLInstruction.h"

class CPPAPI KLProgram
{
public:
	void Build();
	void AddFunctions(const std::vector<KLFunction*>& functions) const;
	static int Run(const KLProgram* program);
private:
	bool build;
};
