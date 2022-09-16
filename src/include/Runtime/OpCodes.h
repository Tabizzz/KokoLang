#pragma once

enum OpCodes
{
	/*
	 *	no code, used for labels only, can be used manually to do "nothing" in one instruction.
	 *
	 *	no stack transitions.
	 */
	noc,

#pragma region local jumps

	/*
	 *  move to a target instruction by index, can be used with labels
	 *
	 *  no stack transitions.
	 */
	go,
	goif,
	goifn,

#pragma endregion

#pragma region stack operations

	/*
	 * push a value in the stack, can push variables.
	 *
	 * stack transition:
	 * ... -> ... value
	 */
	push,
	pop,
	dup,
	clear,
	stackl,

#pragma endregion

#pragma region variable manipulation

	stvar,
	ldvar,
	set,
	get,
	starg,
	ldarg,

#pragma endregion

#pragma region boolean operations

	and,
	or,
	xor,
	oplt,
	ople,
	opgt,
	opge,
	opeq,
	opne,

#pragma endregion

#pragma region arithmetic operations

	add,
	sub,
	mul,
	div,
	mod,
	pow,
	root,
	log,

#pragma endregion

#pragma region convertion to builtin types

	tstr,
	hash,
	fash,
	tbit,

#pragma endregion

#pragma region function related jumps

	jump,
	ret,
	call,

#pragma endregion

#pragma region memory

	aloc,
	free,
	copy,
	fill,

#pragma endregion

#pragma region arrays

	arr,
	arl,
	lde,
	ste,

#pragma endregion

#pragma region struct

	has,
	is,
	as,
	size,
	ins,
	ref,
	deref,
	build,

#pragma endregion


};