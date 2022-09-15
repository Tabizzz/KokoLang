#pragma once

enum OpCodes
{
	/*
	 *	no code, used for labels only, can be used manually to do "nothing" in one instruction.
	 *
	 *	no stack transitions.
	 */
	noc,
	/*
	 *  move to a target instruction by index, can be used with labels
	 *
	 *  no stack transitions.
	 */
	go,
	/*
	 * push a value in the stack, can push variables.
	 *
	 * stack transition:
	 * ... -> ... value
	 */
	push,
	stvar,	// set a local variable with
	ldvar,
	oplt,
	goif,
	add,
	ret,
	call
};