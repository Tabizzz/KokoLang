#pragma once

#include <stack>
#include "klimports.h"
#include "Runtime/KLObject.h"
using namespace std;

// two regs, first one for int operations and second for float operations.
#define CALL_REG_COUNT 2

/*
 * Represent the scope of a function call, this contains the evaluation stack,
 * the locals and the arguments of the function.
 * This type is generally never created with klNew or klIns, is created on the
 * stack.
 */
struct CPPAPI KLCall
{
	/*
	 * The index of the next instruction to run.
	 */
	unsigned short next;
	/*
	 * todo
	 */
	bool exit;
	/*
	 * The current amount of objects in the stack, same as evaluationStack.size().
	 */
	kbyte stackc;
	/*
	 * The amount of args slots the function have.
	 */
	kbyte argc;
	/*
	 * The evaluation stack holding the objects to use in opcodes.
	 */
	stack<KlObject*,
	vector<KlObject*>>
	evaluationStack;
	/*
	 * The local storage on the call.
	 *
	 * This vector has the locals, the args passed to the function and
	 * CALL_REG_COUNT reg objects to internal operations.
	 *
	 * The first CALL_REG_COUNT are reg values.
	 * The next objects are the locals.
	 * Finally, at the end are the passed args.
	 */
	vector<KlObject*> st;
};