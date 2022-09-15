
#include "KokoLangInternal.h"

using namespace antlr4;

CAPI KLProgram* klCreateProgramFromFile(const char* filename)
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
	return any_cast<KLProgram*>(visitor.visitProgram(tree));
}

CAPI KLProgram* klCreateProgramFromString(const char *code)
{
	ANTLRInputStream input(code);
	KokoLangLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	KokoLangParser parser(&tokens);

	auto tree = parser.program();
	if (parser.getNumberOfSyntaxErrors() > 0) return nullptr;

	ProgramVisitor visitor;
	return any_cast<KLProgram*>(visitor.visitProgram(tree));
}

void klDestroyProgram(KLProgram* obj) {
	delete obj;
}
