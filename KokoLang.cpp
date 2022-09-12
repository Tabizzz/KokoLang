#include "KokoLang.h"
#include <iostream>
#include <chrono>


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
			if (!strcmp("-t", argv[2]))
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

		KLProgram program = any_cast<KLProgram>(visitor.visitProgram(tree));

		auto instructionList = program.Build();

		auto exit = KLProgram::Run(instructionList);

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		if(time)
			cout << "program execution:" << duration.count()<< "ms" << endl;

		return exit;
	}

	return 1;
}