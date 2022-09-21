#pragma once

#include <stack>
#include "KLImports.h"
#include "Runtime/KLObject.h"
using namespace std;

/*
 * Represent the scope of a function call, this contains the evaluation stack, the locals and the arguments of the function.
 * This type is generally never created with klNew or klIns, is created on the stack and assigned the type.
 */
struct CPPAPI KLCall
{
	unsigned short next;
	bool exit;
	kbyte stackc;
	kbyte argc;
	stack<KlObject*, vector<KlObject*>>
	evaluationStack;
	KlObject** args;
	KlObject** locals;
};

/*
 * Type definition for KLCall.
 * This type is defined when call klInit.
 */
extern KlType klBType_Call;
