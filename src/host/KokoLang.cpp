#include "kilasm.h"
#include "mempool.h"
#define TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES
#include "termcolor/termcolor.hpp"
#include "boost/nowide/iostream.hpp"

#include <chrono>
#include <cstring>
#include <iostream>

#if _WIN32 || _WIN64
#include <cstdio>
#include <windows.h>
#endif

using namespace std::chrono;
using namespace boost;

#define MEASURE(x, y)if(time) start = high_resolution_clock::now();    \
y;                                                                      \
if (time)  {                                                            \
sub = high_resolution_clock::now() - start;                                \
duration = duration_cast<microseconds>(sub);                            \
if(duration.count() < 1000)    {                                        \
nowide::cout << termcolor::yellow <<                                    \
(x) << termcolor::reset << duration.count() << u8"μs" << std::endl;    \
}else {                                                                \
durationms = duration_cast<milliseconds>(sub);                        \
if(durationms.count() < 1000)    {                                      \
nowide::cout << termcolor::yellow <<                                    \
(x) << termcolor::reset << durationms.count() << "ms" << std::endl;        \
}else {                                                                \
durations = duration_cast<seconds>(sub);                                \
nowide::cout << termcolor::yellow <<                                    \
(x) << termcolor::reset << durations.count() << "s" << std::endl;        \
}}}

KlObject *outImpl(KlObject *caller, KlObject **argv, kbyte passedArgs) {
	auto val = argv[0];
	if (val) {
		if (val->type == klstring_t) {
			nowide::cout.write(KASSTR(val), KASSTRSIZE(val)) << std::endl;
		} else if (val->type->toString) {
			auto str = val->type->toString(val);
			nowide::cout.write(KASSTR(str), KASSTRSIZE(str)) << std::endl;
			klDeref(str);
		}
	} else {
		nowide::cout << "null" << std::endl;
	}
	return nullptr;
}

int main(int argc, const char *argv[]) {
	if (termcolor::_internal::is_atty(std::cout))
		nowide::cout << termcolor::colorize;
#if _WIN32 || _WIN64
	SetConsoleOutputCP( 65001 );
#endif
	if (argc > 1) {
		bool time = false;
		if (argc > 2) {
			if (!strcmp("-t", argv[2])) {
				time = true;
			}
		}
		auto start = high_resolution_clock::now();
		duration<long, std::ratio<1, 1000000000>> sub{};
		microseconds duration;
		milliseconds durationms;
		seconds durations;

		klConfig.argc = argc;
		klConfig.argv = argv;
		klConfig.alloc = mempoolAlloc;
		klConfig.dealloc = mempoolDealloc;

		klInit();

		MEASURE("Program parse: ", KLPackage *program = klLoadIntermediateFile(argv[1]))

		auto out = KLCAST(KLFunction, klIns(klfunc_t));
		out->external = true;
		out->invokable = outImpl;
		out->args = 1;
		out->margs = 1;
		out->name = KLSTR("out");
		klGlobalPackage()->functions->insert(MetaPair("out", KLWRAP(out)));

		int exit = EXIT_SUCCESS;
		if (program) {
			klRegisterPackage(program);

			MEASURE("Program build: ", klBuildPackage(program))
			MEASURE("Program run: ", exit = klRunPackage(program, argc, argv))
		} else {
			exit = EXIT_FAILURE;
		}

		klEnd();
		freeAllocs();
		return exit;
	}

	nowide::cout << termcolor::red << "Error: " << termcolor::reset << "No input files" << std::endl;
	return EXIT_FAILURE;
}