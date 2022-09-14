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

		KLProgram* program = klCreateProgramFromFile(argv[1]);

		program->Build();

		auto exit = KLProgram::Run(program);

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		if (time)
			cout << "program execution:" << duration.count() << "ms" << endl;

		return exit;
	}

	return 1;
}