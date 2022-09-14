﻿#include "Runtime/KLRuntime.h"

void KLProgram::Build()
{
}

void KLProgram::AddFunctions(const vector<KLFunction*>& functions) const
{
	for (KLFunction* function : functions)
	{
		cout << "Adding function " << function->getName() << endl;
	}
}

const int KLProgram::Run(const KLProgram* program)
{
	if (program)
	{
		return 0;
	}
	return 1;
}
