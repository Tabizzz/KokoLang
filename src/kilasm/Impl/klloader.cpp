#include "kilasm_internal.h"
#include "klloader.h"

#define MEASURETIME

#ifdef MEASURETIME
#include <chrono>
using namespace std::chrono;
#endif
using namespace antlr4;
using namespace std;

KLIBAPI KLPackage* klLoadIntermediateFile(const char* filename)
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
	cout << "antlr parser: " << duration.count() << u8"μs" << endl;
#endif
	if (parser.getNumberOfSyntaxErrors() > 0) return nullptr;

	ProgramVisitor visitor;
	auto ret =  any_cast<KLPackage*>(visitor.visitProgram(tree));

	stream.close();
	return ret;
}