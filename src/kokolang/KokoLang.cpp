#include "KokoLang.h"
#include "KokoLangLib.h"
#include "termcolor/termcolor.hpp"

#include <chrono>
#include <cstring>
#include <iostream>

using namespace std::chrono;

#define MEASURE(x, y)if(time) start = high_resolution_clock::now(); 	\
y;                                                                      \
if (time)  {															\
sub = high_resolution_clock::now() - start;								\
duration = duration_cast<microseconds>(sub);                         	\
if(duration.count() < 1000)    {                                      	\
std::cout << (x) << duration.count() << u8"μs" << std::endl; 			\
}else {                                                              	\
durationms = duration_cast<milliseconds>(sub);                      	\
if(durationms.count() < 1000)    {                                      \
std::cout << (x) << durationms.count() << "ms" << std::endl;			\
}else {  																\
durations = duration_cast<seconds>(sub);								\
std::cout << (x) << durations.count() << "s" << std::endl;				\
}}}

int main(int argc, const char* argv[])
{
	if (argc > 1) {
		bool time = false;
		if (argc > 2) {
			if (!strcmp("-t", argv[2])) {
				time = true;
			}
		}
		auto start = high_resolution_clock::now();
		auto sub = high_resolution_clock::now() - start;
		auto duration = duration_cast<microseconds>(sub);
		auto durationms = duration_cast<milliseconds>(sub);
		auto durations = duration_cast<seconds>(sub);

		klInit();

		MEASURE("Program parse: ", KLPackage *program = klCreateProgramFromFile(argv[1]))

		int exit;
		if(program) {
			klRegisterPackage(program);

			MEASURE("Program build: ", klPackage_Build(program))
			MEASURE("Program run: ", exit = klPackage_Run(program, argc, argv))
		} else {
			exit = EXIT_FAILURE;
		}
		klEnd();
		return exit;
	}
	std::cout << termcolor::red << "Error: " << termcolor::reset << "No input files" << std::endl;
	return EXIT_FAILURE;
}