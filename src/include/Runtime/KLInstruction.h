#pragma once

#include "KLImports.h"
#include "Runtime/OpCodes.h"
#include "Runtime/KLObject.h"

using namespace std;

/// <summary>
/// A instruction 
/// </summary>
struct CPPAPI KLInstruction
{
	// we use char* here because kl_string use char*
	char* label;		// the name of the label if this instruction is a label, null otherwise
	OpCodes opcode;		// the opcode this instruction represent
	KlObject* foperand;	// the first operand of the op code
	KlObject* soperand;	// the second operand of the op code



	explicit KLInstruction(string& name);
	KLInstruction(OpCodes opcode, KlObject* operandf, KlObject*operands);
	~KLInstruction();
};