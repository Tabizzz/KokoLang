#include "KokoLang.h"
#include "KokoLangLib.h"

#include <chrono>
#include <cstring>
#include <iostream>

using namespace std;
using namespace std::chrono;

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
		KLPackage* program = klCreateProgramFromFile(argv[1]);
		program->Build();

		auto start = high_resolution_clock::now();
		auto exit = KLPackage::Run(program);
		auto stop = high_resolution_clock::now();
		// destroy any created program!
		klDestroyProgram(program);
		auto duration = duration_cast<milliseconds>(stop - start);
		if (time)
			cout << "program execution:" << duration.count() << "ms" << endl;
		return exit;
	}

	return 1;
}