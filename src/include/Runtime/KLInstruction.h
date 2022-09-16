#pragma once

#include "KLImports.h"
#include "Runtime/OpCodes.h"
#include "Runtime/KLObject.h"

using namespace std;

/// <summary>
/// A instruction 
/// </summary>
CPPAPI struct KLInstruction
{
	explicit KLInstruction(string& name);
	KLInstruction(OpCodes opcode, KlObject* operand);
	~KLInstruction();

	// we use char* here because kl_string use char*
	char* label;		// the name of the label if this instruction is a label, null otherwise
	OpCodes opcode;		// the opcode this instruccion represent
	KlObject* operand;	// the operand of the op code
};