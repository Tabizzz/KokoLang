﻿#include "kilasm_internal.h"
#include "ProgramVisitor.h"

any ProgramVisitor::visitProgram(KokoLangParser::ProgramContext* ctx)
{
	auto program = klCreatePackage();
	auto functionContexts = ctx->function();
	auto funcionCount = functionContexts.size();

	for (size_t i = 0; i < funcionCount; i++)
	{
		auto func = any_cast<KLFunction*>(visitFunction(functionContexts[i]));
		string name = KSTRING(func->name);

		auto find = program->functions->find(name);
		if(find == program->functions->end()) {
			program->functions->insert(pair<string, KLObject *>(name, KLWRAP(func)));
		} else {
			cout << "ignoring repeat function definition: " << name << endl;
		}
	}
	program->name = KLSTR(KLPROGRAM_NAME);
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
	auto function = KLCAST(KLFunction, klIns(klfunc_t));
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
	auto instruction = KLCAST(KLInstruction, klIns(klinstruction_t));

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

	auto operandv = new KLObject*[numargs]{};

	if(requiredoperands > 0 || optionalargs != 0) getOperands(&opcode, operandv, values, size);

	// every operand is marked as a constant including registers
	for (int i = 0; i < numargs; ++i) {
		KLINVOKE(operandv[i])->rflags.constant = true;
	}

	instruction->opcode = opcode;
	instruction->operandc = numargs;
	instruction->operands = operandv;

	return instruction;
}