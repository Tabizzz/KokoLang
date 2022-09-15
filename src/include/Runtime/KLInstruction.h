#pragma once

#include "Imports.h"
#include "Runtime/OpCodes.h"
#include "Runtime/KLObject.h"

using namespace std;

class CPPAPI KLInstruction
{
public:
	explicit KLInstruction(string& name);
	KLInstruction(OpCodes opcode, KlObject* operand);
	~KLInstruction();

	char* label;
	OpCodes opcode;
	KlObject* operand;
};