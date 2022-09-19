#include <iostream>
#include "KokoLangInternal.h"

KLPackage::~KLPackage()
{
	for (auto func : functions)
	{
		delete func;
	}
}

void KLPackage::Build()
{
	for (auto func: functions) {
		func->reallocateLabels();
	}

}

int KLPackage::Run(const KLPackage* program)
{
	if (program)
	{
		KLFunction* main;
		for (auto func: program->functions) {
			if(func->getName() == "main")
			{
				main = func;
				break;
			}
		}
		if(main)
		{
			return 0;
		}
		cout<<"Error: unable to find entry point 'main'" << endl;
		return 1;
	}
	return 1;
}
