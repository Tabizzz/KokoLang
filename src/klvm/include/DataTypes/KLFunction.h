#pragma once

#include "DataTypes/KLType.h"
#include "DataTypes/KLInstruction.h"
#include "DataTypes/KLPackage.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A function object.
 *
 * Functions define the argument count it expects, the name and an invokable.
 */
struct CPPAPI KLFunction {
	KLOBJECTHEAD					// base type KLObject
	KlObject* name;					// the name of the function
	klinvokable invokable;			// the actual function,
	kbyte margs;					// minimum amount of params the function expects
	char args;						// maximum amount of params the function can receive or -1 to unlimited
	kbyte external;					// if true the function is not defined in kokolang
	kbyte locals;					// how many locals the function have
	unsigned short size;			// the amount of instructions on the function body.
	std::vector<KLInstruction *>*
	body;							// the body of the function.

	MetaMap* metadata;				// metadata of the function
};

#ifdef __cplusplus
}
#endif

CAPI
/**
 * @brief Build a function to optimize runtime.
 *
 * Some instructions use identifiers to refers to objects, for example, KOpcode::get uses as first operand a identifier of a package variable,
 * this methods find all the identifiers and replace strings with the respective runtime handlers.
 *
 * After finding and replacing all identifiers with runtime handlers, this function will reassign the labels, that is, the opcodes go, goif and goifn
 * have a string to identify the label they will jump to, reassigning the labels means that the string will be replaced by an int that points
 * to the exact instruction that must be executed after the jump, in this way it will not be necessary to look for the label every time you want
 * to make a jump.
 *
 * @param package The package building the function.
 * @param type The type which contains the function.
 * @param func The function to build.
 */
void klBuildFunction(KLPackage* package, KLType* type, KLFunction* func);

/**
 * @brief Type definition for KLFunction.
 *
 * This type is defined when call klInit.
 */
CAPI KLType klBType_Func;