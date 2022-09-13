
#include "KokoLangInternal.h"


using namespace antlr4;

KLAPI KLProgram* klCreateProgramFromFile(const char* filename)
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

KLAPI void DestroyAny(void* obj)
{
	delete obj;
}

KLAPI void DestroyArray(void* obj)
{
	delete[] obj;
}
