#include "KokoLangInternal.h"
#include "ProgramVisitor.h"


using namespace std;

any ProgramVisitor::visitProgram(KokoLangParser::ProgramContext* ctx)
{
	auto program = new KLProgram();
	auto functionContexts = ctx->function();
	auto funcionCount = functionContexts.size();

	vector<KLFunction*> functions;
	for (size_t i = 0; i < funcionCount; i++)
	{
		functions.push_back(any_cast<KLFunction*>(visitFunction(functionContexts[i])));
	}
	program->AddFunctions(functions);

	return program;
}

any ProgramVisitor::visitFunction(KokoLangParser::FunctionContext* ctx)
{
	auto name = ctx->Id()->getText();
	auto body = ctx->funcblock();
	auto localstext = body->local()->Number()->getText();
	auto locals = stoi(localstext);
	if (locals < 0) locals = 0;
	auto stackctx = body->stack();
	int stack = 10;
	if (stackctx)
	{
		stack = stoi(stackctx->Number()->getText());
		if (stack < 0) stack = 0;
	}
	auto function = new KLFunction(name, locals, stack);
	auto sentences = body->sentence();
	auto sentencecount = sentences.size();
	vector<KLInstruction*> instructions;
	for (int i = 0; i < sentencecount; ++i) {
		instructions.push_back(any_cast<KLInstruction*>(visitSentence(sentences[i])));
	}

	cout << "Find function with name: " << name << " with " << locals << " locals" << endl;
	return function;
}

any ProgramVisitor::visitSentence(KokoLangParser::SentenceContext *ctx) {
	KLInstruction* instruction;
	auto opcodectx = ctx->opcode();
	string name;
	if(!opcodectx)
	{
		name = ctx->label()->Id()->getText();
		
	}
	return instruction;
}
