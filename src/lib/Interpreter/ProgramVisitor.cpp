#include "KokoLangLib.h"
#include "ProgramVisitor.h"

using namespace std;

any ProgramVisitor::visitProgram(KokoLangParser::ProgramContext* ctx)
{
	auto program = klCreatePackage();
	auto functionContexts = ctx->function();
	auto funcionCount = functionContexts.size();

	for (size_t i = 0; i < funcionCount; i++)
	{
		auto func = any_cast<KLFunction*>(visitFunction(functionContexts[i]));
		string name = KLCAST(kl_string, func->name)->value;

		auto find = program->functions->find(name);
		if(find == program->functions->end()) {
			program->functions->insert(pair<string, KlObject *>(name, KLWRAP(func)));
		} else {
			cout << "ignoring repeat function definition: " << name << endl;
		}
	}

	return program;
}

#define FUNCATTR(x) auto x##c = attr->x(); \
					if(x##c) *x = stoi(x##c->Number()->getText());

void parseFunctionAttributes(KokoLangParser::FuncattrsContext *pContext, unsigned char* local, char *args,
							 unsigned char *margs)
{
	auto vect = pContext->funcattr();

	for (auto attr : vect) {
		FUNCATTR(local)
		FUNCATTR(args)
		FUNCATTR(margs)
	}
}

any ProgramVisitor::visitFunction(KokoLangParser::FunctionContext* ctx)
{
	auto name = ctx->Id()->getText();
	auto body = ctx->funcblock();
	auto function = KLCAST(KLFunction, klIns(&klBType_Func));
	function->name = KLSTR(name);
	function->body = new vector<KLInstruction*>();

	parseFunctionAttributes(ctx->funcblock()->funcattrs(), &function->locals, &function->args, &function->margs);
	auto sentences = body->sentence();
	auto sentencecount = sentences.size();
	for (int i = 0; i < sentencecount; ++i) {
		function->body->push_back(any_cast<KLInstruction*>(visitSentence(sentences[i])));
	}
	function->size = function->body->size();
	return function;
}

any ProgramVisitor::visitSentence(KokoLangParser::SentenceContext *ctx) {
	auto opcodectx = ctx->opcode();
	string name;
	auto instruction = KLCAST(KLInstruction, klIns(&klBType_Instruction));

	if(!opcodectx)
	{
		name = ctx->label()->Id()->getText();
		instruction->label = KLSTR(name);
		return instruction;
	}
	auto opcode = getOpcode(opcodectx);
	auto values = opcodectx->value();
	int size = values.size(); // NOLINT(cppcoreguidelines-narrowing-conversions)
	int optionalargs = 0;
	auto requiredoperands = CheckOperandCount(size, opcode, &optionalargs);

	auto numargs = requiredoperands;
	if(optionalargs != 0)
	{
		numargs = optionalargs == -1 ? size : requiredoperands + optionalargs;
	}

	auto operandv = new KlObject*[numargs]{};

	if(requiredoperands > 0 || optionalargs != 0) getOperands(&opcode, operandv, values, size);

	instruction->opcode = opcode;
	instruction->operandc = numargs;
	instruction->operands = operandv;

	return instruction;
}