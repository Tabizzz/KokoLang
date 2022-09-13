#pragma once

#include "Imports.h"
#include "Runtime/KLFunction.h"
#include "Runtime/KLInstruction.h"

using namespace std;

class CPPAPI KLProgram
{
public:
	vector<KLInstruction> Build();
	void AddFunctions(vector<KLFunction> functions);
	static int Run(KLProgram* program);
};
