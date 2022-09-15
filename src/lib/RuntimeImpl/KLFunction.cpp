#include <utility>

#include "KokoLangInternal.h"

KLFunction::KLFunction(string name, int locals, int stack)
{
	FunctionName = std::move(name);
}

KLFunction::~KLFunction() = default;

string KLFunction::getName()
{
	return FunctionName;
}
