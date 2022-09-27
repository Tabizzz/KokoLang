#pragma once

#include "klimports.h"
#include "KOpcode.h"
#include "Runtime/KLObject.h"
#include "Runtime/KLCall.h"

using namespace std;

typedef void (*opcodecall)(const KlObject& caller, const KLCall& call, KlObject* operands[], size_t operandc);

/*
 * A part of a function which operate on the stack.
 */
struct CPPAPI KLInstruction
{
	KLOBJECTHEAD
	KlObject* label;	// the name of the label if this instruction is a label, null otherwise
	KOpcode opcode;		// the opcode this instruction represent
	KlObject** operands;// the operands of the op code
	size_t operandc;	// the amount of operands passed to the opcode
	opcodecall call;	// the function invoked with the opcode logic
};

/*
 * Type definition for KLInstruction.
 * This type is defined when call klInit.
 */
CAPI KlType klBType_Instruction;