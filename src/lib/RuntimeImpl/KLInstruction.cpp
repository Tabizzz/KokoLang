#include "KokoLangInternal.h"


KLInstruction::KLInstruction()
{
	opcode = OpCodes::empty;
	operand = nullptr;
}

KLInstruction::KLInstruction(OpCodes code, KlObject* toperand)
{
	opcode = code;
	operand = toperand;
}

KLInstruction::~KLInstruction() = default;
