#pragma once

#include "DataTypes/KLObject.h"

// two regs, first one for int operations and second for float operations.
#define CALL_REG_COUNT 2

#define CALL_FLAG_EXIT 		0
#define CALL_FLAG_CHECK 	1
#define CALL_FLAG_COUNT		2

#define CALL_HAS_FLAG(x, y) (x.flags.test(y))
#define CALL_SET_FLAG(x, y, z) (x.flags.set(y, z))

/**
 * Represent the scope of a function call, this contains the evaluation stack,
 * the locals and the arguments of the function.
 * This type is generally never created with klNew or klIns, is created on the
 * stack.
 */
struct CPPAPI KLCall
{
	/**
	 * The index of the next instruction to run.
	 */
	unsigned int next;
	/**
	 * The amount of args slots the function have.
	 */
	unsigned short argc;
	/**
	 * The amount of local slots the function have.
	 */
	unsigned short locs;
	/**
	 * The flags of the call.
	 * the bits are:
	 * 0: exit flag, if set to true the method will end.
	 * 1: check flag, set to true when an operation result in true, like an and.
	 */
	std::bitset<CALL_FLAG_COUNT> flags;
	/**
	 * The local storage on the call.
	 *
	 * This vector has the locals, the args passed to the function and
	 * CALL_REG_COUNT reg objects to internal operations.
	 *
	 * The first CALL_REG_COUNT are reg values.
	 * The next objects are the locals.
	 * Finally, at the end are the passed args.
	 */
	std::vector<KLObject*> st;
	/**
	 * Object returned by the function call.
	 */
	KLObject* ret = nullptr;

	~KLCall();
};
