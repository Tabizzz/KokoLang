#include "KokoLangInternal.h"
#include "klbinary.h"

#include <iostream>
#include <fstream>

using namespace std;

KLPackage *klCreatePackageFromFile(const char *filename) {
	std::ifstream stream;
	stream.open(R"(D:\Onedrive\Escritorio\ss)");
	auto pack = klCreatePackageFromStream(&stream);
	stream.close();
	return pack;
}

KLPackage* createBasePackage(std::istream* &stream)
{
	kbyte sig1, sig2, major, minor, namesize, authorsize;


	return nullptr;
}

KLPackage *klCreatePackageFromStream(std::istream* stream) {
	KLPackage* package;
	try {
		package = createBasePackage(stream);
	}
	catch (std::runtime_error &error) {
		return nullptr;
	}

	if(!package) return nullptr;


	return package;
}

KLPackage *klCreatePackageFromMemory(const void *ptr, size_t size) {
	return nullptr;
}

