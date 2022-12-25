#include "KokoLangInternal.h"
#include "klbinary.h"

#include <iostream>
#include <fstream>

#define CHECKSTREAM(x,y) if(stream.fail() || stream.eof() || stream.bad()) { y return x; }

using namespace std;

KLPackage *klCreatePackageFromFile(const char *filename) {
	std::ifstream stream;
	stream.open(R"(D:\Onedrive\Escritorio\ss)");
	auto pack = klCreatePackageFromStream(&stream);
	stream.close();
	return pack;
}

void readMeatadata(std::istream &stream, std::map<std::string, KlObject*>* metadata)
{
	kshort size;
	kbyte count;
	stream.read((char*)&size, 2);
	stream.read((char*)&count, 1);
	CHECKSTREAM(,)

	while (count--) {
		kbyte keyl;
		stream.read((char*)&keyl, 1);
		CHECKSTREAM(,)
		auto keybuff = new char[keyl + 1];
		keybuff[keyl] = 0;
		stream.read(keybuff, keyl);
		CHECKSTREAM(, delete[] keybuff;)
		std::string key = keybuff;
		delete[] keybuff;

		KMetaType type;
		stream.read((char*)&type, 1);
		CHECKSTREAM(,)

		KlObject* value = nullptr;

		switch (type) {
			case bfalse:
				value = KLBOOL(false);
				break;
			case btrue:
				value = KLBOOL(true);
				break;
			case integer:
				int64_t rvalue;
				stream.read((char*)&rvalue, sizeof(int64_t));
				value = KLINT(rvalue);
				break;
			case number:
				double_t dvalue;
				stream.read((char*)&dvalue, sizeof(double_t));
				value = KLFLOAT(dvalue);
				break;
			case KMetaType::string:
				stream.read((char*)&keyl, 1);
				CHECKSTREAM(,)
				value = klIns(&klBType_String);
				KLCAST(kl_string, value)->size = keyl;
				auto valuebuff = new char[keyl + 1];
				valuebuff[keyl] = 0;
				stream.read(valuebuff, keyl);
				CHECKSTREAM(, delete[] valuebuff;)
				KLCAST(kl_string, value)->value = valuebuff;
				delete[] valuebuff;
				break;
		}
		CHECKSTREAM(, klDeref(value);)

		metadata->insert(pair<std::string, KlObject*>(key, value));
	}
}


KLPackage* readDefinitions(KLPackage *pPackage, istream &istream) {


	return pPackage;
}

KLPackage* createBasePackage(std::istream &stream)
{
	kbyte read[6];
	stream.read((char*)read, 6);

	CHECKSTREAM(nullptr,)

	if(read[0] != 75 || read[1] != 76) return nullptr;

	KLPackage* dev = read[4] ? klCreatePackage() : klGlobalPackage();

	// if package is not global set the variables
	if(read[4]) {
		auto namebuff = new char[read[4]+1];
		namebuff[read[4]] = 0;
		stream.read(namebuff, read[4]);
		// create a new empty instance and set the values
		dev->name = klIns(&klBType_String);
		KLCAST(kl_string, dev->name)->size = read[4];
		KLCAST(kl_string, dev->name)->value = namebuff;

		KlObject* authorname;

		if(read[5]) {
			namebuff = new char[read[5]+1];
			namebuff[read[5]] = 0;
			stream.read(namebuff, read[5]);
			// create a new empty instance and set the values
			authorname = klIns(&klBType_String);
			KLCAST(kl_string, authorname)->size = read[4];
			KLCAST(kl_string, authorname)->value = namebuff;
		}

		readMeatadata(stream, dev->metadata);
		CHECKSTREAM(nullptr, klDeref(KLWRAP(dev));)

		dev->metadata->insert(pair<std::string, KlObject*>("version_major", KLINT(read[2])));
		dev->metadata->insert(pair<std::string, KlObject*>("version_minor", KLINT(read[3])));
		if(read[5])	dev->metadata->insert(pair<std::string, KlObject*>("author", authorname));
	}
	else {
		// we need to skip te author name
		if(read[5]) {
			stream.seekg(read[5], ios_base::cur);
			CHECKSTREAM(nullptr,)
		}

		// we need to skip metadata, read size of block to skip
		kshort size;
		stream.read((char*)&size, 2);
		CHECKSTREAM(nullptr,)
		stream.seekg(size - 2, ios_base::cur);
		CHECKSTREAM(nullptr,)
	}

	return readDefinitions(dev, stream);
}

KLPackage *klCreatePackageFromStream(std::istream* stream) {
	KLPackage* package;
	try {
		package = createBasePackage(*stream);
	}
	catch (...) {
		return nullptr;
	}

	if(!package) return nullptr;


	return package;
}

KLPackage *klCreatePackageFromMemory(const void *ptr, size_t size) {
	return nullptr;
}

