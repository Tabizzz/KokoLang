#pragma once

#include "KokoLangLib.h"
#include "gen/KokoLangBaseVisitor.h"

class ProgramVisitor : KokoLangBaseVisitor
{
public:
	antlrcpp::Any visitProgram(KokoLangParser::ProgramContext* ctx) override;
	antlrcpp::Any visitFunction(KokoLangParser::FunctionContext* ctx) override;
	std::any visitSentence(KokoLangParser::SentenceContext *ctx) override;

	static OpCodes getOpcode(KokoLangParser::OpcodeContext *pContext);

	static void
	getOperands(OpCodes *pCodes, KlObject **fOperand, KlObject **sOperand,
				KokoLangParser::ValueContext *fContext, KokoLangParser::ValueContext *sContext);
};