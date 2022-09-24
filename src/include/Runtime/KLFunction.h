﻿#pragma once

#include "KLImports.h"
#include "Runtime/KLObject.h"
#include "Runtime/KLInstruction.h"
#include "Runtime/KLTypes.h"

#ifdef __cplusplus
extern "C" {
#endif
using namespace std;

/**
 * A function object, functions define the argument count it expects, the name and an invokable.
 */
struct CPPAPI KLFunction {
	KLOBJECTHEAD					// base type KLObject
	KlObject* name;					// the name of the function
	klinvokable invokable;			// the actual function,
	kbyte margs;					// minimum amount of params the function expects
	char args;						// maximum amount of params the function can receive or -1 to unlimited
	bool external;					// if true the function is not defined in kokolang

	kbyte locals;					// how many locals the function have
	kbyte stack;					// the max size of elements in the stack of the function.
	unsigned short size;			// the amount of instructions on the function body.
	vector<KLInstruction *>* body;	// the body of the function.
};

/*
 * Type definition for KLFunction.
 * This type is defined when call klInit.
 */
CAPI KlType klBType_Func;

#ifdef __cplusplus
}
#endif