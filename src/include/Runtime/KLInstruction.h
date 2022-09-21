#pragma once

#include "KLImports.h"
#include "Runtime/OpCodes.h"
#include "Runtime/KLObject.h"
#include "Runtime/KLCall.h"

using namespace std;

typedef void (*opcodecall)(KlObject* function, KLCall* call, KlObject* opf, KlObject* ops);

/*
 * A part of a function with operate on the stack.
 */
struct CPPAPI KLInstruction
{
	KLOBJECTHEAD
	KlObject* label;	// the name of the label if this instruction is a label, null otherwise
	OpCodes opcode;		// the opcode this instruction represent
	KlObject* foperand;	// the first operand of the op code
	KlObject* soperand;	// the second operand of the op code
	opcodecall call;	// the function invoked with the opcode logic
};

/*
 * Type definition for KLInstruction.
 * This type is defined when call klInit.
 */
extern KlType klBType_Instruction;