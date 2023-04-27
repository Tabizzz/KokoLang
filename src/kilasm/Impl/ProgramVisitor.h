#pragma once

#include "kilasm.h"
#include "KokoLangBaseVisitor.h"

class ProgramVisitor : KokoLangBaseVisitor
{
public:
	antlrcpp::Any visitProgram(KokoLangParser::ProgramContext* ctx) override;
	antlrcpp::Any visitFunction(KokoLangParser::FunctionContext* ctx) override;
	std::any visitSentence(KokoLangParser::SentenceContext *ctx) override;

	static KLOpcode getOpcode(KokoLangParser::OpcodeContext *pContext);

	static void getOperands(KLOpcode *pOpcode, KlObject **operands, const std::vector<KokoLangParser::ValueContext*>& vector, size_t size);

	static int CheckOperandCount(size_t size, KLOpcode opcode, int* optionals);
};