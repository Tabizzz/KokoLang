#pragma once

#include <stack>
#include "klimports.h"
#include "Runtime/KLObject.h"
using namespace std;

// two regs, first one for int operations and second for float operations.
#define CALL_REG_COUNT 2

#define CALL_FLAG_EXIT 		1 << 0
#define CALL_FLAG_RETURN 	1 << 1
#define CALL_FLAG_CHECK 	1 << 5
#define CALL_FLAG_DUP	 	1 << 6

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
	 * The flags of the call.
	 * the bits are:
	 * 0: exit flag, if set to true the method will end.
	 * 1: return flag, if set to true the method return something.
	 * 2-4: the index of the register that contains the return value, can be up to index 7.
	 * 5: check flag, set to true when an operation result in true, like an and.
	 * 6: dup flag, is set to true operations will set values by duplication and not by copy.
	 * 7-15: reserved.
	 */
	unsigned short flags;
	/*
	 * The amount of args slots the function have.
	 */
	unsigned short argc;
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