#pragma once

#include "Imports.h"
#include "Runtime/KLFunction.h"
#include "Runtime/KLInstruction.h"

using namespace std;

class CPPAPI KLProgram
{
public:
	void Build();
	void AddFunctions(const vector<KLFunction*>& functions) const;
	static const int Run(const KLProgram* program);
private:
	bool build;
};
