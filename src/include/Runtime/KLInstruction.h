#pragma once

#include "Imports.h"
#include "Runtime/OpCodes.h"
#include "Runtime/KLObject.h"

using namespace std;

class CPPAPI KLInstruction
{
public:
	KLInstruction();
	KLInstruction(OpCodes opcode, KlObject* operand);
	~KLInstruction();

	OpCodes opcode;
	KlObject* operand;
};