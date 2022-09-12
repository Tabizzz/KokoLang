#include "KLInterpreter.h"
#include "Runtime/KLRuntime.h"

using namespace std;

any ProgramVisitor::visitProgram(KokoLangParser::ProgramContext* ctx)
{
	auto program = new KLProgram();
	auto functionContexts = ctx->function();
	auto funcionCount = functionContexts.size();
	auto functions = new vector<KLFunction>(funcionCount);
	for (size_t i = 0; i < funcionCount; i++)
	{
		functions->push_back(any_cast<KLFunction>(visitFunction(functionContexts[i])));
	}

	

	return program;
}

any ProgramVisitor::visitFunction(KokoLangParser::FunctionContext* ctx)
{
	auto name = ctx->Id()->getText();
	cout << "Find function with name: " << name << endl;
	return KLFunction();
}
