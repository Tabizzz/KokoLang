#pragma once

#include <stack>
#include <bitset>
#include "klimports.h"
#include "DataTypes/KLObject.h"

// two regs, first one for int operations and second for float operations.
#define CALL_REG_COUNT 2

#define CALL_FLAG_EXIT 		0
#define CALL_FLAG_RETURN 	1
#define CALL_FLAG_CHECK 	5
#define CALL_FLAG_CLONE	 	6
#define CALL_FLAG_COUNT		7

#define CALL_HAS_FLAG(x, y) (x.flags.test(y))
#define CALL_SET_FLAG(x, y, z) (x.flags.set(y, z))

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
	unsigned int next;
	/*
	 * The amount of args slots the function have.
	 */
	unsigned int argc;
	/*
	 * The flags of the call.
	 * the bits are:
	 * 0: exit flag, if set to true the method will end.
	 * 1: return flag, if set to true the method return something.
	 * 2-4: the index of the register that contains the return value, can be up to index 7.
	 * 5: check flag, set to true when an operation result in true, like an and.
	 * 6: clone flag, if set to true operations will set values by clone and not by copy.
	 */
	std::bitset<CALL_FLAG_COUNT> flags;
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
	std::vector<KlObject*> st;
};