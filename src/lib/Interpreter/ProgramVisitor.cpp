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
		return new KLInstruction(name);
	}
	auto opcode = getOpcode(opcodectx);
	auto operand = getOperand(opcodectx, opcode);

	return instruction;
}

OpCodes ProgramVisitor::getOpcode(KokoLangParser::OpcodeContext *pContext) {
	auto code = pContext->Id()->getText();
	// https://stackoverflow.com/a/16388594
	static const std::map<std::string, OpCodes> optionStrings {
			{ "noc", noc },
			{ "go", go },
			{ "push", push },
			{ "stvar", stvar },
			{ "ldvar", ldvar },
			{ "oplt", oplt },
			{ "goif", goif },
			{ "add", add },
			{ "ret", ret },
			{ "call", call },
			//...
	};

	auto itr = optionStrings.find(code);
	if( itr != optionStrings.end() ) {
		return itr->second;
	}

	throw std::invalid_argument( "invalid opcode in program" );
}

KlObject* ProgramVisitor::getOperand(KokoLangParser::OpcodeContext *pContext, OpCodes codes) {
	switch (codes) {
		case noc:
		case add:
		case ret:
			break;
		case oplt:
			break;
		case go:
			break;
		case push:
			break;
		case stvar:
			break;
		case ldvar:
			break;
		case goif:
			break;
		case call:
			break;
	}
	return nullptr;
}
