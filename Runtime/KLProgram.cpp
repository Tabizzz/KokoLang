#include "KLRuntime.h"

void KLProgram::Build()
{
}

void KLProgram::AddFunctions(const vector<KLFunction*>& functions) const
{
	for each (KLFunction* function in functions)
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
