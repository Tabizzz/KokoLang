#include <utility>

#include "KokoLangInternal.h"


KLInstruction::KLInstruction(string& name)
{
	auto size = name.size();
	label = new char[size + 1] { };
	name.copy(label, size);
	opcode = noc;
	operand = nullptr;
}

KLInstruction::KLInstruction(OpCodes code, KlObject* toperand)
{
	label = nullptr;
	opcode = code;
	operand = toperand;
}

KLInstruction::~KLInstruction()
{
	if (label)
	{
		delete[] label;
	}
	if (operand)
	{
		klDeref(operand);
	}
}
