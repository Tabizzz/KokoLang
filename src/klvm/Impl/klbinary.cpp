#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#include "klvm_internal.h"
#include "klbinary.h"

#include <iostream>
#include <fstream>

#define CHECKSTREAM(x,y) if(stream.fail() || stream.eof() || stream.bad()) { y return x; }

static inline void readPackageDefinition(MetaMap *target, istream &stream);

using namespace std;

KLPackage *klCreatePackageFromFile(const char *filename) {
	std::ifstream stream;
	stream.open(filename);
	auto pack = klCreatePackageFromStream(&stream);
	stream.close();
	return pack;
}

static void readMetadata(std::istream &stream, MetaMap* metadata)
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

		KLMetaType type;
		stream.read((char*)&type, 1);
		CHECKSTREAM(,)

		KLObject* value = nullptr;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
		switch (type) {
			case KLMetaType::bfalse:
				value = KLBOOL(false);
				break;
			case KLMetaType::btrue:
				value = KLBOOL(true);
				break;
			case KLMetaType::integer:
				int64_t rvalue;
				stream.read((char*)&rvalue, sizeof(int64_t));
				value = KLINT(rvalue);
				break;
			case KLMetaType::number:
				double_t dvalue;
				stream.read((char*)&dvalue, sizeof(double_t));
				value = KLFLOAT(dvalue);
				break;
			case KLMetaType::string:
				stream.read((char*)&keyl, 1);
				CHECKSTREAM(,)
				value = klIns(klstring_t);
				KLCAST(kl_string, value)->size = keyl;
				auto valuebuff = new char[keyl + 1];
				valuebuff[keyl] = 0;
				stream.read(valuebuff, keyl);
				CHECKSTREAM(, delete[] valuebuff;)
				KLCAST(kl_string, value)->value = valuebuff;
				delete[] valuebuff;
				break;
		}
#pragma clang diagnostic pop

		CHECKSTREAM(, klDeref(value);)

		metadata->insert(pair<std::string, KLObject*>(key, value));
	}
}

static KLObject* readObject(std::istream &stream)
{
	KLObject* value = nullptr;
	KLMetaType type;
	stream.read((char*)&type, 1);
	CHECKSTREAM(nullptr,)

	switch (type) {
		case KLMetaType::null:
			value = nullptr;
			break;
		case KLMetaType::bfalse:
			value = KLBOOL(false);
			break;
		case KLMetaType::btrue:
			value = KLBOOL(true);
			break;
		case KLMetaType::integer:
			int64_t rvalue;
			stream.read((char*)&rvalue, sizeof(int64_t));
			value = KLINT(rvalue);
			break;
		case KLMetaType::reg:
			int16_t ivalue;
			stream.read((char*)&ivalue, sizeof(int16_t));
			value = klIns(klreg_t);
			KLCAST(kl_int, value)->value = ivalue;
			break;
		case KLMetaType::number:
			double_t dvalue;
			stream.read((char*)&dvalue, sizeof(double_t));
			value = KLFLOAT(dvalue);
			break;
		case KLMetaType::string:
			kbyte keyl;
			stream.read((char*)&keyl, 1);
			CHECKSTREAM(nullptr,)
			value = klIns(klstring_t);
			KLCAST(kl_string, value)->size = keyl;
			auto valuebuff = new char[keyl + 1];
			valuebuff[keyl] = 0;
			stream.read(valuebuff, keyl);
			CHECKSTREAM(nullptr, delete[] valuebuff;)
			KLCAST(kl_string, value)->value = valuebuff;
			delete[] valuebuff;
			break;
	}
	CHECKSTREAM(nullptr,)
	return value;
}

static inline void readVariableDefinition(MetaMap* target, istream &stream, bool type) {
	auto var = KLCAST(KLVariable, klIns(klvar_t));
	kbyte namesize;
	bool hasdefaultvalue;
	kbyte offset;

	stream.read((char*)&namesize, 1);
	stream.read((char*)&hasdefaultvalue, 1);
	stream.read((char*)&offset, 1);
	CHECKSTREAM(, klDeref(KLWRAP(var));)
	auto namebuff = new char[namesize + 1];
	namebuff[namesize] = 0;
	stream.read(namebuff, namesize);
	CHECKSTREAM(, klDeref(KLWRAP(var)); delete [] namebuff;)
	KLObject* defaultValue = nullptr;
	if(hasdefaultvalue)
		defaultValue = readObject(stream);
	readMetadata(stream, var->metadata);
	CHECKSTREAM(, klDeref(KLWRAP(var)); delete [] namebuff;)
	
	if(!type) { // type var
		var->data.typevar.offset = offset * sizeof(KLObject*);
		var->data.typevar.defaultValue = defaultValue;
	}

	string name(namebuff);
	delete [] namebuff;
	auto find = target->find(name);
	if(find == target->end()) {
		target->insert(std::pair<std::string, KLObject*>(std::move(name), KLWRAP(var)));
	} else {
		klDeref(KLWRAP(var));
	}
}

static vector<KLInstruction *> *readFuntionBody(istream &stream, kshort size) {
	auto dev = new vector<KLInstruction*>();
	while (size--) {
		KLOpcode opcode;
		kshort count;
		kbyte labelsize;
		stream.read((char*)&opcode, 1);
		stream.read((char*)&count, 2);
		stream.read((char*)&labelsize, 1);
		CHECKSTREAM(dev, )
		auto ins = KLCAST(KLInstruction, klIns(klinstruction_t));
		if(labelsize) {
			auto namebuff = new char[labelsize + 1];
			namebuff[labelsize] = 0;
			stream.read(namebuff, labelsize);
			CHECKSTREAM(dev, delete[] namebuff;klDeref(KLWRAP(ins));)
			ins->label = klIns(klstring_t);
			KLCAST(kl_string, ins->label)->value = namebuff;
			KLCAST(kl_string, ins->label)->size = labelsize;
		}
		ins->operands = new KLObject*[count]{};
		for (int i = 0; i < count; ++i) {
			ins->operands[i] = readObject(stream);
		}
		CHECKSTREAM(dev, klDeref(KLWRAP(ins));)
		ins->opcode = opcode;
		ins->operandc = count;
		dev->push_back(ins);
	}

	return dev;
}

static inline void readFunctionDefinition(MetaMap *target, istream &stream, bool type) {
	kbyte read[4];
	kshort size;
	stream.read((char*)&read, 4);
	stream.read((char*)&size, 2);
	CHECKSTREAM(,)

	auto namebuff = new char[read[0] +1];
	namebuff[read[0]] = 0;
	stream.read(namebuff, read[0]);
	CHECKSTREAM(, delete [] namebuff;)

	auto func = KLCAST(KLFunction, klIns(klfunc_t));
	func->name = klIns(klstring_t);
	KLCAST(kl_string, func->name)->value = namebuff;
	KLCAST(kl_string, func->name)->size = read[0];
	func->body = readFuntionBody(stream, size);
	CHECKSTREAM(, klDeref(KLWRAP(func));)
	readMetadata(stream, func->metadata);
	CHECKSTREAM(, klDeref(KLWRAP(func));)

	func->size = size;
	func->locals = read[3];
	char* args = (char*)&read[2];
	func->margs = read[1];
	func->args = *args;
	if(!type && !func->args) {
		//invalid function args count
		klDeref(KLWRAP(func));
		return;
	}

	auto find = target->find(namebuff);
	if(find == target->end()) {
		target->insert(std::pair<std::string, KLObject*>(namebuff, KLWRAP(func)));
	} else {
		klDeref(KLWRAP(func));
	}

}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "DanglingPointer"
static inline void readTypeDefinition(istream &stream, KLPackage *parent) {
	kbyte namesize;
	stream.read((char*)&namesize, 1);
	CHECKSTREAM(,)
	auto namebuff = new char[namesize + 1];
	namebuff[namesize] = 0;
	stream.read(namebuff, namesize);
	CHECKSTREAM(, delete [] namebuff;)
	auto type = new KLType{
		KLObject (),
		namebuff,
		0,
		sizeof(KLObject)
	};
	readMetadata(stream, type->metadata);
	KLDefinitionType def;
	do
	{
		stream.read((char*)&def, 1);
		CHECKSTREAM(, delete type;)

		switch (def) {

			case KLDefinitionType::variable:
				readVariableDefinition(type->variables, stream, false);
				break;
			case KLDefinitionType::function:
				readFunctionDefinition(type->functions, stream, false);
				break;
			case KLDefinitionType::close:
			case KLDefinitionType::type:
			case KLDefinitionType::subpackage:
				break;
		}
		CHECKSTREAM(, delete type;)
	} while (def == KLDefinitionType::variable || def == KLDefinitionType::function);
	type->size += type->variables->size() * sizeof(KLObject*);
	// todo: convert functions in the type to builtin operations
	klDefType(type);
	klPackageRegType(parent, type);
}
#pragma clang diagnostic pop

static KLPackage* readDefinitions(KLPackage *pPackage, std::istream &stream) {
	KLDefinitionType def;

	do
	{
		stream.read((char*)&def, 1);
		CHECKSTREAM(nullptr, klDeref(KLWRAP(pPackage));)

		switch (def) {

			case KLDefinitionType::close:
				break;
			case KLDefinitionType::variable:
				readVariableDefinition(pPackage->variables, stream, true);
				break;
			case KLDefinitionType::function:
				readFunctionDefinition(pPackage->functions, stream, true);
				break;
			case KLDefinitionType::type:
				readTypeDefinition(stream, pPackage);
				break;
			case KLDefinitionType::subpackage:
				readPackageDefinition(pPackage->packs, stream);
				break;
		}
		CHECKSTREAM(nullptr, klDeref(KLWRAP(pPackage));)
	} while (def != KLDefinitionType::close);

	return pPackage;
}

static inline void readPackageDefinition(MetaMap *target, istream &stream) {
	kbyte namesize;
	stream.read((char*)&namesize, 1);
	CHECKSTREAM(,)
	auto namebuff = new char[namesize + 1];
	namebuff[namesize] = 0;
	stream.read(namebuff, namesize);
	CHECKSTREAM(, delete [] namebuff;)
	auto package = KLCAST(KLPackage, klIns(klpack_t));
	package->name = klIns(klstring_t);
	KLCAST(kl_string, package->name)->value = namebuff;
	KLCAST(kl_string, package->name)->size = namesize;
	readMetadata(stream, package->metadata);
	CHECKSTREAM(, klDeref(KLWRAP(package));)
	package = readDefinitions(package, stream);
	CHECKSTREAM(,) // don't deref the package, readDefinitions already deref it.

	auto find = target->find(namebuff);
	if(find == target->end()) {
		target->insert(std::pair<std::string, KLObject*>(namebuff, KLWRAP(package)));
	} else {
		klDeref(KLWRAP(package));
	}

}

static KLPackage* createBasePackage(std::istream &stream)
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
		dev->name = klIns(klstring_t);
		KLCAST(kl_string, dev->name)->size = read[4];
		KLCAST(kl_string, dev->name)->value = namebuff;

		KLObject* authorname;

		if(read[5]) {
			namebuff = new char[read[5]+1];
			namebuff[read[5]] = 0;
			stream.read(namebuff, read[5]);
			// create a new empty instance and set the values
			authorname = klIns(klstring_t);
			KLCAST(kl_string, authorname)->size = read[4];
			KLCAST(kl_string, authorname)->value = namebuff;
		}

		readMetadata(stream, dev->metadata);
		CHECKSTREAM(nullptr, klDeref(KLWRAP(dev));)

		dev->metadata->insert(pair<std::string, KLObject*>("version_major", KLINT(read[2])));
		dev->metadata->insert(pair<std::string, KLObject*>("version_minor", KLINT(read[3])));
		if(read[5])	dev->metadata->insert(pair<std::string, KLObject*>("author", authorname));
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
		klRef(KLWRAP(dev));
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
	return package;
}



#pragma clang diagnostic pop