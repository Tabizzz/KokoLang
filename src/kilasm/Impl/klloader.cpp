#include "kilasm_internal.h"
#include "klloader.h"

using namespace antlr4;
using namespace std;

KLIBAPI KLPackage* klLoadIntermediateFile(const char* filename)
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
	auto ret =  any_cast<KLPackage*>(visitor.visitProgram(tree));

	stream.close();
	return ret;
}