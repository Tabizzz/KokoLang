#pragma once

#include "KokoLangLib.h"
#include "gen/KokoLangBaseVisitor.h"

class ProgramVisitor : KokoLangBaseVisitor
{
public:
	antlrcpp::Any visitProgram(KokoLangParser::ProgramContext* ctx) override;
	antlrcpp::Any visitFunction(KokoLangParser::FunctionContext* ctx) override;
	std::any visitSentence(KokoLangParser::SentenceContext *ctx) override;

	static KOpcode getOpcode(KokoLangParser::OpcodeContext *pContext);

	static void getOperands(KOpcode *pOpcode, KlObject **operands, const std::vector<KokoLangParser::ValueContext*>& vector, size_t size);

	static int CheckOperandCount(size_t size, KOpcode opcode, int* optionals);
};