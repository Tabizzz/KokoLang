#pragma once

#include "Binary/KLOpcode.h"
#include "DataTypes/KLObject.h"
#include "DataTypes/KLCall.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*opcodecall)(const KLObject* caller, KLCall& call, KLObject* operands[], size_t operandc);

/*
 * A part of a function which operate on the stack.
 */
struct CPPAPI KLInstruction
{
	KLOBJECTHEAD
	KLObject* label;	// the name of the label if this instruction is a label, null otherwise
	KLOpcode opcode;		// the opcode this instruction represent
	KLObject** operands;// the operands of the op code
	size_t operandc;	// the amount of operands passed to the opcode
	opcodecall call;	// the function invoked with the opcode logic
};

#ifdef __cplusplus
}
#endif

/*
 * Type definition for KLInstruction.
 * This type is defined when call klInit.
 */
CAPI KLType* klinstruction_t;