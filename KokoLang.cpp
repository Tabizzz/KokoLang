#include <iostream>
#include <chrono>

#include "antlr4-runtime.h"
#include "libs/KokoLangLexer.h"
#include "libs/KokoLangParser.h"

using namespace std;
using namespace std::chrono;
using namespace antlr4;

int main(int argc, const char* argv[]) {
	auto start = high_resolution_clock::now();
	std::ifstream stream;
	stream.open("C:\\Users\\User\\Source\\CLionProjects\\KokoLang\\test.kl");
	ANTLRInputStream input(stream);
	KokoLangLexer lexer(&input);
	CommonTokenStream tokens(&lexer);
	KokoLangParser parser(&tokens);

	auto tree = parser.program();


	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	// To get the value of duration use the count()
	// member function on the duration object
	cout << duration.count() << endl;

	return 0;
}