#pragma once

#include "KLImports.h"
#include "Runtime/KLObject.h"
#include "Runtime/KLInstruction.h"

using namespace std;

class CPPAPI KLFunction
{
public:
	KLFunction(string name, int locals, int stack);
	~KLFunction();
	string getName();
	vector<KLInstruction*> body;
private:
	string FunctionName;
};