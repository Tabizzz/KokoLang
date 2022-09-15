#pragma once

#include "Imports.h"
#include "Runtime/KLObject.h"

using namespace std;

class CPPAPI KLFunction
{
public:
	KLFunction(string name, int locals, int stack);
	~KLFunction();

	const string getName();
private:
	string FunctionName;
};