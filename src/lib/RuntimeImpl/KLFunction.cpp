#include <utility>

#include "KokoLangInternal.h"

KLFunction::KLFunction(string name)
{
	FunctionName = std::move(name);
	locals = 0;
	stack = 10;
	args = 0;
	margs = 0;
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
