#include <iostream>
#include <chrono>

#include "antlr4-runtime.h"
#include "libs/KokoLangLexer.h"
#include "libs/KokoLangParser.h"
#include "klapi.h"
#include "Interpreter/ProgramVisitor.h"

using namespace std;
using namespace std::chrono;
using namespace antlr4;

int main(int argc, const char* argv[]) 
{
	if (argc > 1)
	{
		bool time = false;
		if (argc > 2)
		{
			if (argv[2] == "-t")
			{
				time = true;
			}
		}
		auto start = high_resolution_clock::now();
		std::ifstream stream;
		stream.open(argv[1]);
		ANTLRInputStream input(stream);
		KokoLangLexer lexer(&input);
		CommonTokenStream tokens(&lexer);
		KokoLangParser parser(&tokens);

		auto tree = parser.program();

		ProgramVisitor visitor;

		auto program = visitor.visitProgram(tree);

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		// To get the value of duration use the count()
		// member function on the duration object
		if(time)
			cout << "program execution:" << duration.count()<< "ms" << endl;

		return 0;
	}

	return 1;
}