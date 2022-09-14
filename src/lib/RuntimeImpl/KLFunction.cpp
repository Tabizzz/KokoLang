#include "KokoLangInternal.h"

KLFunction::KLFunction(string name, int locals, int stack)
{
	FunctionName = name;
}

KLFunction::~KLFunction()
{
}

const string KLFunction::getName()
{
	return FunctionName;
}
