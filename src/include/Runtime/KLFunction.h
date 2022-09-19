#pragma once

#include "KLImports.h"
#include "Runtime/KLObject.h"
#include "Runtime/KLInstruction.h"

using namespace std;


struct CPPAPI KLFunction
{
	string FunctionName;
	string getName();
	vector<KLInstruction*> body;
	unsigned char locals, stack, margs, size;
	char args;


	explicit KLFunction(string name);
	~KLFunction();
	void reallocateLabels();
};