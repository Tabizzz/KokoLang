#pragma once

#include "KLImports.h"
#include "Runtime/KLObject.h"
#include "Runtime/KLInstruction.h"

using namespace std;

class CPPAPI KLFunction
{
private:
	string FunctionName;
public:
	explicit KLFunction(string name);
	~KLFunction();
	void reallocateLabels();
	string getName();
	vector<KLInstruction*> body;
	unsigned char locals, stack, margs, size;
	char args;
};