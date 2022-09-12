#pragma once
#include <string>
#include "antlr4-runtime.h"
#include "libs/KokoLangBaseVisitor.h"

class  ProgramVisitor : KokoLangBaseVisitor 
{
public:
	antlrcpp::Any visitProgram(KokoLangParser::ProgramContext* ctx);

};