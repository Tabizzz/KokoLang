#pragma once

#include "gen/KokoLangBaseVisitor.h"

class ProgramVisitor : KokoLangBaseVisitor
{
public:
	antlrcpp::Any visitProgram(KokoLangParser::ProgramContext* ctx) override;
	antlrcpp::Any visitFunction(KokoLangParser::FunctionContext* ctx) override;
	std::any visitSentence(KokoLangParser::SentenceContext *ctx) override;
};