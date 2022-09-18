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
	string getName();
	vector<KLInstruction*> body;
	unsigned char locals, stack, margs;
	char args;
};