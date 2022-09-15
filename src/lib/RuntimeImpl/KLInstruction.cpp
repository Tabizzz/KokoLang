#include <utility>

#include "KokoLangInternal.h"


KLInstruction::KLInstruction(string& name)
{
	label = name.data();
	opcode = noc;
	operand = nullptr;
}

KLInstruction::KLInstruction(OpCodes code, KlObject* toperand)
{
	label = nullptr;
	opcode = code;
	operand = toperand;
}

KLInstruction::~KLInstruction() = default;
