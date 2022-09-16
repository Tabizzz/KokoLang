#include "KokoLangInternal.h"

KLProgram::~KLProgram()
{
	for (auto func : functions)
	{
		delete func;
	}
}

void KLProgram::Build()
{
}

int KLProgram::Run(const KLProgram* program)
{
	if (program)
	{
		return 0;
	}
	return 1;
}
