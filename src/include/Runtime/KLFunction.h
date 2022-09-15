#pragma once

#include "KLImports.h"
#include "Runtime/KLObject.h"

using namespace std;

class CPPAPI KLFunction
{
public:
	KLFunction(string name, int locals, int stack);
	~KLFunction();

	string getName();
private:
	string FunctionName;
};