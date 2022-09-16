#include <utility>

#include "KokoLangInternal.h"

KLFunction::KLFunction(string name, int locals, int stack)
{
	FunctionName = std::move(name);
}

KLFunction::~KLFunction()
{
	for (auto ins : body)
	{
		delete ins;
	}
}


string KLFunction::getName()
{
	return FunctionName;
}
