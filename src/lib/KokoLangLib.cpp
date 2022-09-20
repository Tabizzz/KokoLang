﻿#include "antlr4-runtime.h"
#include "Interpreter/KLInterpreter.h"
#include "gen/KokoLangLexer.h"
#include "gen/KokoLangParser.h"
#include "KokoLangLib.h"

#define MEASURETIME

#ifdef MEASURETIME
#include <chrono>
using namespace std::chrono;
#endif
using namespace antlr4;
using namespace std;


KLIBAPI KLPackage* klCreateProgramFromFile(const char* filename)
{
	std::ifstream stream;
	stream.open(filename);
#ifdef MEASURETIME
	auto start  = std::chrono::high_resolution_clock::now();
#endif
	ANTLRInputStream input(stream);
	KokoLangLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	KokoLangParser parser(&tokens);

	auto tree = parser.program();
#ifdef MEASURETIME
	auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start);
	cout << "antlr parser: " << duration.count() << "μs" << endl;
#endif
	if (parser.getNumberOfSyntaxErrors() > 0) return nullptr;

	ProgramVisitor visitor;
	auto ret =  any_cast<KLPackage*>(visitor.visitProgram(tree));

	stream.close();
	return ret;
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
