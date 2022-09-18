#include <utility>

#include "KokoLangInternal.h"


KLInstruction::KLInstruction(string& name)
{
	auto size = name.size();
	label = new char[size + 1] { };
	name.copy(label, size);
	opcode = noc;
	foperand = nullptr;
	soperand = nullptr;
}

KLInstruction::KLInstruction(OpCodes code, KlObject* operandf, KlObject*operands)
{
	label = nullptr;
	opcode = code;
	foperand = operandf;
	soperand = operands;
}

KLInstruction::~KLInstruction() {
	delete[] label;

	klDeref(foperand);

	klDeref(soperand);

}
