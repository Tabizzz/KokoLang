#pragma once

#include "libs/KokoLangBaseVisitor.h"

class  ProgramVisitor : KokoLangBaseVisitor 
{
public:
	antlrcpp::Any visitProgram(KokoLangParser::ProgramContext* ctx);
	antlrcpp::Any visitFunction(KokoLangParser::FunctionContext* ctx);
};