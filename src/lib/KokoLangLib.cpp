﻿#include "antlr4-runtime.h"
#include "Interpreter/KLInterpreter.h"
#include "gen/KokoLangLexer.h"
#include "gen/KokoLangParser.h"
#include "KokoLangLib.h"

using namespace antlr4;
using namespace std;


KLIBAPI KLPackage* klCreateProgramFromFile(const char* filename)
{
	std::ifstream stream;
	stream.open(filename);
	ANTLRInputStream input(stream);
	KokoLangLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	KokoLangParser parser(&tokens);

	auto tree = parser.program();
	if (parser.getNumberOfSyntaxErrors() > 0) return nullptr;

	ProgramVisitor visitor;
	return any_cast<KLPackage*>(visitor.visitProgram(tree));
}

KLIBAPI KLPackage* klCreateProgramFromString(const char *code)
{
	ANTLRInputStream input(code);
	KokoLangLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	KokoLangParser parser(&tokens);

	auto tree = parser.program();
	if (parser.getNumberOfSyntaxErrors() > 0) return nullptr;

	ProgramVisitor visitor;
	return any_cast<KLPackage*>(visitor.visitProgram(tree));
}
