#pragma once

enum OpCodes : unsigned char
{
	/*
	 *	no code, used for labels only, can be used manually to do "nothing" in one instruction.
	 *
	 *	usage:
	 *	noc
	 *
	 *	no stack transitions.
	 */
	noc,

#pragma region local jumps

	/*
	 *  move to a target instruction by index, can be used with labels.
	 *
	 * usage:
	 * go <label>
	 *
	 * operators:
	 * -label: the target label name or the index of the target instruction.
	 *
	 *  no stack transitions.
	 */
	go,
	/*
	 *  move to a target instruction by index if the value in the stack is true
	 *  or not null, can be used with labels.
	 *
	 * usage:
	 * goif <label>
	 *
	 * operators:
	 * -label: the target label name or the index of the target instruction.
	 *
	 *  stack transition:
	 *  ... value -> ...
	 *
	 *  - value is taken from the stack and evaluated, if is true or not null then
	 *  jump to label, otherwise continue with the next instruction.
	 */
	goif,
	/*
	 *  move to a target instruction by index if the value in the stack is false
	 *  or null, can be used with labels.
	 *
	 * usage:
	 * goifn <label>
	 *
	 * operators:
	 * -label: the target label name or the index of the target instruction.
	 *
	 *  stack transition:
	 *  ... value -> ...
	 *
	 *  - value is taken from the stack and evaluated, if is false or null then
	 *  jump to label, otherwise continue with the next instruction.
	 */
	goifn,

#pragma endregion

#pragma region stack operations

	/*
	 * push a value in the stack, can push variables.
	 *
	 * usage:
	 * push <obj>
	 *
	 * operators:
	 * -obj: any primitive value to push in the stack, if an identifier is used
	 * the opcode will be replaced with a 'get' code.
	 *
	 * stack transition:
	 * ... -> ... value
	 *
	 * - obj is pushed on the stack, if is a variable the value stored is pushed.
	 */
	push,
	/*
	 * pop a value from the stack.
	 *
	 * usage:
	 * pop
	 *
	 * stack transition:
	 * ... obj -> ...
	 *
	 * - obj, the last element in the stack is removed.
	 */
	pop,
	/*
	 * duplicate the element in top of the stack.
	 *
	 * usage:
	 * dup
	 *
	 * stack transition:
	 *
	 * ... obj -> .. obj obj
	 *
	 * - obj is popped off of the stack duplicated and pushed again with the duplicated.
	 */
	dup,
	/*
	 * clear the stack.
	 *
	 * usage:
	 * clear
	 *
	 * stack transition:
	 * ... ->
	 *
	 * - any object in the stack will be popped off.
	 */
	clear,
	/*
	 * push the current amount of items in the stack as an int.
	 *
	 * usage:
	 * stackl
	 *
	 * stack transition:
	 * ... -> ... count
	 *
	 * - the stack count is pushed in the stack.
	 */
	stackl,

#pragma endregion

#pragma region variable manipulation

	/*
	 * store the current value in the stack in the local var by index.
	 *
	 * usage:
	 * stvar <index>
	 *
	 * stack transition:
	 * ... value -> ...
	 *
	 * -value is popped off of stack and stored in the given local var index
	 */
	stvar,
	/*
	 * load the value of a local var by index onto the stack.
	 *
	 * usage:
	 * ldvar <index>
	 *
	 * stack transition:
	 * ... -> ... value
	 *
	 * - the value of the local var is pushed onto the stack.
	 */
	ldvar,
	/*
	 * store the current value in the stack in the global var by name.
	 *
	 * usage:
	 * set <name>
	 *
	 * stack transition:
	 * ... value -> ...
	 *
	 * -value is popped off of stack and stored in the given global var name
	 */
	set,
	/*
	 * load the value of a global var by name onto the stack.
	 *
	 * usage:
	 * ldvar <name>
	 *
	 * stack transition:
	 * ... -> ... value
	 *
	 * - the value of the global var is pushed onto the stack.
	 */
	get,
	/*
	 * store the current value in the stack in the arg slot by index.
	 *
	 * usage:
	 * starg <index>
	 *
	 * stack transition:
	 * ... value -> ...
	 *
	 * -value is popped off of stack and stored in the given arg slot index
	 */
	starg,
	/*
	 * load the value of an arg slot by index onto the stack.
	 *
	 * usage:
	 * ldarg <index>
	 *
	 * stack transition:
	 * ... -> ... value
	 *
	 * - the value of the arg slot is pushed onto the stack.
	 */
	ldarg,

#pragma endregion

#pragma region boolean operations

	/*
	 * Computes the AND operation of two values and pushes the result onto the evaluation stack.
	 * AND evaluates to true only if the two values are true.
	 *
	 * *A value count as true if is different from 0, false or is not null
	 *
	 * usage:
	 * and
	 *
	 * stack transition:
	 * ... obj1 obj2 -> ... result
	 *
	 * - obj1 and obj2 are popped off of the stack and the operation is executed.
	 * - the result of the AND operation is pushed onto the stack.
	 */
	andi,
	/*
	 * Computes the OR operation of two values and pushes the result onto the evaluation stack.
	 * OR evaluates to true if one or two of the operands are true.
	 *
	 * *A value count as true if is different from 0, false or is not null
	 *
	 * usage:
	 * or
	 *
	 * stack transition:
	 * ... obj1 obj2 -> ... result
	 *
	 * - obj1 and obj2 are popped off of the stack and the operation is executed.
	 * - the result of the OR operation is pushed onto the stack.
	 */
	ori,
	/*
	 * Computes the XOR operation of two values and pushes the result onto the evaluation stack.
	 * XOR evaluates to true if and only if one of the operands are true.
	 *
	 * *A value count as true if is different from 0, false or is not null
	 *
	 * usage:
	 * xor
	 *
	 * stack transition:
	 * ... obj1 obj2 -> ... result
	 *
	 * - obj1 and obj2 are popped off of the stack and the operation is executed.
	 * - the result of the XOR operation is pushed onto the stack.
	 */
	xori,
	/*
	 * Computes the LT operation of two values and pushes the result onto the evaluation stack.
	 * LT evaluates to true if the first operand is less than the second.
	 *
	 * usage:
	 * oplt
	 *
	 * stack transition:
	 * ... obj1 obj2 -> ... result
	 *|
	 * - obj1 and obj2 are popped off of the stack and the operation is executed.
	 * - the result of the LT operation is pushed onto the stack.
	 */
	oplt,
	/*
	 * Computes the LE operation of two values and pushes the result onto the evaluation stack.
	 * LE evaluates to true if the first operand is less than or equal to the second.
	 *
	 * usage:
	 * ople
	 *
	 * stack transition:
	 * ... obj1 obj2 -> ... result
	 *
	 * - obj1 and obj2 are popped off of the stack and the operation is executed.
	 * - the result of the LE operation is pushed onto the stack.
	 */
	ople,
	/*
	 * Computes the GT operation of two values and pushes the result onto the evaluation stack.
	 * GT evaluates to true if the first operand is greater than the second.
	 *
	 * usage:
	 * opgt
	 *
	 * stack transition:
	 * ... obj1 obj2 -> ... result
	 *
	 * - obj1 and obj2 are popped off of the stack and the operation is executed.
	 * - the result of the GT operation is pushed onto the stack.
	 */
	opgt,
	/*
	 * Computes the GE operation of two values and pushes the result onto the evaluation stack.
	 * GE evaluates to true if the first operand is greater than or equal to the second.
	 *
	 * usage:
	 * opge
	 *
	 * stack transition:
	 * ... obj1 obj2 -> ... result
	 *
	 * - obj1 and obj2 are popped off of the stack and the operation is executed.
	 * - the result of the GE operation is pushed onto the stack.
	 */
	opge,
	/*
	 * Computes the EQ operation of two values and pushes the result onto the evaluation stack.
	 * EQ evaluates to true if the operands are equal, by default this means that are the same
	 * pointer.
	 *
	 * usage:
	 * opeq
	 *
	 * stack transition:
	 * ... obj1 obj2 -> ... result
	 *
	 * - obj1 and obj2 are popped off of the stack and the operation is executed.
	 * - the result of the EQ operation is pushed onto the stack.
	 */
	opeq,
	/*
	 * Computes the NE operation of two values and pushes the result onto the evaluation stack.
	 * NE evaluates to true if the operands are not equal, by default this means that are not the
	 * same pointer.
	 *
	 * usage:
	 * opne
	 *
	 * stack transition:
	 * ... obj1 obj2 -> ... result
	 *
	 * - obj1 and obj2 are popped off of the stack and the operation is executed.
	 * - the result of the NE operation is pushed onto the stack.
	 */
	opne,

#pragma endregion

#pragma region arithmetic operations

	/*
	 * Add two values and push the result in the stack.
	 *
	 * usage:
	 * add
	 *
	 * stack transition:
	 * ... obj1 obj2 -> ... result
	 *
	 * - obj1 and obj2 are popped off of the stack and the operation is executed.
	 * - the result of the operation is pushed onto the stack.
	 */
	add,
	/*
	 * Subtract two values and push the result in the stack.
	 *
	 * usage:
	 * sub
	 *
	 * stack transition:
	 * ... obj1 obj2 -> ... result
	 *
	 * - obj1 and obj2 are popped off of the stack and the operation is executed.
	 * - the result of the operation is pushed onto the stack.
	 */
	sub,
	/*
	 * Multiply two values and push the result in the stack.
	 *
	 * usage:
	 * mul
	 *
	 * stack transition:
	 * ... obj1 obj2 -> ... result
	 *
	 * - obj1 and obj2 are popped off of the stack and the operation is executed.
	 * - the result of the operation is pushed onto the stack.
	 */
	mul,
	/*
	 * Divide the first value by the second value in the stack.
	 *
	 * usage:
	 * div
	 *
	 * stack transition:
	 * ... obj1 obj2 -> ... result
	 *
	 * - obj1 and obj2 are popped off of the stack and the operation is executed.
	 * - the result of the operation is pushed onto the stack.
	 */
	divi,
	/*
	 * Compute the modulo of the first value by the second value on the stack.
	 *
	 * usage:
	 * mod
	 *
	 * stack transition:
	 * ... obj1 obj2 -> ... result
	 *
	 * - obj1 and obj2 are popped off of the stack and the operation is executed.
	 * - the result of the operation is pushed onto the stack.
	 */
	mod,

#pragma endregion

#pragma region convertion to types

	/*
	 * Convert the value to the string representation and push the converted
	 * value onto the stack.
	 *
	 * usage:
	 * tstr
	 *
	 * stack transition:
	 * ... obj -> ... str
	 *
	 * - obj is popped off of the stack and evaluated the conversion.
	 * - the result of the conversion in pushed onto the stack.
	 */
	tstr,
	/*
	 * Convert the value to the int representation and push the converted
	 * value onto the stack.
	 *
	 * usage:
	 * tint
	 *
	 * stack transition:
	 * ... obj -> ... int
	 *
	 * - obj is popped off of the stack and evaluated the conversion.
	 * - the result of the conversion in pushed onto the stack.
	 */
	tint,
	/*
	 * Convert the value to the float representation and push the converted
	 * value onto the stack.
	 *
	 * usage:
	 * tflt
	 *
	 * stack transition:
	 * ... obj -> ... flt
	 *
	 * - obj is popped off of the stack and evaluated the conversion.
	 * - the result of the conversion in pushed onto the stack.
	 */
	tflt,
	/*
	 * Convert the value to the bool representation and push the converted
	 * value onto the stack.
	 *
	 * usage:
	 * tbit
	 *
	 * stack transition:
	 * ... obj -> ... bit
	 *
	 * - obj is popped off of the stack and evaluated the conversion.
	 * - the result of the conversion in pushed onto the stack.
	 */
	tbit,
	/*
	 * Convert the object in the stack to another type, this conversion is called
	 * on the type of the object, not in the target type.
	 *
	 * usage:
	 * tobj <type>
	 *
	 * -type: the target type to convert.
	 *
	 * stack transition:
	 * ... value -> ... obj
	 *
	 * - value is popped off of the stack and evaluated the conversion.
	 * - the result of the conversion in pushed onto the stack.
	 */
	tobj,
	/*
	 * Convert the object in the stack to another type, this conversion is called
	 * on the target type, not in the current type of the object.
	 *
	 * usage:
	 * cast <type>
	 *
	 * -type: the target type to convert.
	 *
	 * stack transition:
	 * ... value -> ... obj
	 *
	 * - value is popped off of the stack and evaluated the conversion.
	 * - the result of the conversion in pushed onto the stack.
	 */
	cast,

#pragma endregion

#pragma region function related

	/*
	 * Exit from this function and call the target function in the context of the
	 * caller of this function.
	 *
	 * usage:
	 * jump <function> [params]
	 *
	 * operators:
	 * -function: the identifier of the function to call.
	 * -params: an optional int to specify the amount of params to send to the function,
	 * if is not defined will be the amount defined by the function itself or all the
	 * stack.
	 *
	 * *before returning the current method the stack must be empty.
	 *
	 * stack transition:
	 * arg1 arg2 argN ->
	 *
	 * - all the args for the function are popped off of the stack, if the function
	 * return something that result will be pushed on the caller stack, not in the
	 * current stack.
	 */
	jump,
	/*
	 * Exit from the current function and return to the caller pushing the object
	 * in the stack onto the caller stack, if there is one present.
	 *
	 * usage:
	 * ret
	 *
	 * stack transition:
	 * [object] ->
	 *
	 * - if there is an object in the stack will be popped off and pushed onto
	 * the caller stack.
	 */
	ret,
	/*
	 * Invoke a target function, if the function return something the value will
	 * be pushed onto the stack.
	 *
	 * usage:
	 * call <function> [params]
	 *
	 * -function: the identifier of the function to call.
	 * -params: an optional int to specify the amount of params to send to the function,
	 * if is not defined will be the amount defined by the function itself or all the
	 * stack.
	 *
	 * stack transition:
	 * ... arg1 arg2 argN -> ... [result]
	 *
	 * - all the args for the function are popped off of the stack, if the function
	 * return something that result will be pushed onto the stack.
	 */
	call,
	/*
	 * Push onto the stack the amount of arguments passed to this function.
	 *
	 * usage:
	 * argc
	 *
	 * stack transition:
	 * ... -> ... value
	 *
	 * -the amount of parameters passed to the function is pushed onto th stack.
	 */
	argc,

#pragma endregion

#pragma region memory

	/*
	 * Allocates a block of memory of arbitrary size and push the pointer
	 * to the beginning of the block onto the stack.
	 *
	 * usage:
	 * aloc <size>
	 *
	 * operators:
	 * -size: an int indicating the size in bytes of the block.
	 *
	 * stack transition:
	 * ... -> ... ptr
	 *
	 * - the ptr to the block created will be pushed onto the stack.
	 */
	aloc,
	/* Free a previously allocated block of memory by the pointer to the block.
	 *
	 * usage:
	 * free
	 *
	 * stack transition:
	 * ... ptr -> ...
	 *
	 * - the pointer to the block of memory is popped off of the stack and the
	 * memory is freed.
	 */
	freei,
	/*
	 * Copy bytes from a memory block to another block, memory blocks are referenced
	 * by pointers.
	 *
	 * usage:
	 * copy
	 *
	 * stack transition:
	 * ... src dest size -> ...
	 *
	 * - the source pointer, destination pointer and size to copy are popped off of the
	 * stack.
	 * - the size is in bytes.
	 */
	copy,
	/*
	 * Fill the target block of memory with a specific value.
	 *
	 * usage:
	 * fill
	 *
	 * stack transition:
	 * ... dest value size -> ...
	 *
	 * - the destination pointer, the int value and the size to copy are popped off of
	 * the stack.
	 * - the size is in bytes.
	 */
	fill,

#pragma endregion

#pragma region arrays

	/*
	 * Creates a new array object of the specific size.
	 *
	 * usage:
	 * arr [dimensions]
	 *
	 * operands:
	 * -dimension: the number extra dimensions of the array. by default is 0,
	 * this means 0 extra dimensions so the array will have only 1.
	 *
	 * stack transition:
	 * ... size1 size2 sizeN -> ... arr
	 *
	 * - depending on the number of dimensions sizes are popped off of the
	 * stack and the array object is pushed onto the stack.
	 */
	arr,
	/*
	 * Push onto the stack the length of the array
	 *
	 * usage:
	 * arl [dimension]
	 *
	 * operands:
	 * -dimension: the dimension of the array to et the size, default to 0.
	 *
	 * stack transition:
	 * ... arr -> ... length
	 *
	 * - the array is popped off of the stack and the length is pushed.
	 */
	arl,
	/*
	 * Push onto the stack the dimension count of an array.
	 *
	 * usage:
	 * ard
	 *
	 * stack transition:
	 * ... arr -> ... dims
	 *
	 * - the array is popped off of the stack and the dimension count is pushed.
	 */
	ard,
	/*
	 * Load the element of the array at the specific index.
	 *
	 * usage:
	 * lde [dimensions]
	 *
	 * operators:
	 * -dimension: the number extra dimensions of the array. by default is 0,
	 * this means 0 extra dimensions so the array will have only 1.
	 *
	 * stack transition:
	 * ... index1 index2 indexN -> ... obj
	 *
	 * - the indices are popped off of the stack depending on the dimension.
	 * - the object is pushed onto the stack.
	 */
	lde,
	/*
	 * Store the element of the array at the specific index.
	 *
	 * usage:
	 * ste [dimensions]
	 *
	 * operators:
	 * -dimension: the number extra dimensions of the array. by default is 0,
	 * this means 0 extra dimensions so the array will have only 1.
	 *
	 * stack transition:
	 * ... value index1 index2 indexN -> ...
	 *
	 * - the value and the indices are popped off of the stack depending on
	 * the dimension.
	 * - the object is stored in the array.
	 */
	ste,

#pragma endregion

#pragma region types

	/*
	 * Push onto the stack the runtime pointer of the given type.
	 *
	 * usage:
	 * type <typename>
	 *
	 * operands:
	 * -typename: the identifier of the type
	 *
	 * stack transition:
	 * ... -> typeptr
	 *
	 * -the type pointer is pushed onto the stack.
	 */
	type,
	/*
	 * Push onto the stack the runtime pointer of the type from the
	 * object in the stack.
	 *
	 * usage:
	 * typeof
	 *
	 *
	 * stack transition:
	 * ... obj -> typeptr
	 *
	 * -obj is popped off of the stack and the type is evaluated.
	 * -the type pointer is pushed onto the stack.
	 */
	typeofi,
	/*
	 * Determines if an object is an instance of the given type.
	 *
	 * usage:
	 * is <typename>
	 *
	 * operands:
	 * -typename: the name of the type.
	 *
	 * stack transition:
	 * ... obj -> ... result
	 *
	 * - the object is popped off of the stack and evaluated.
	 * - if the object is an instance of the type then true is pushed
	 * onto the stack, otherwise false is pushed.
	 */
	is,

#pragma endregion

#pragma region structs

	/*
	 * Create a new instance of the given type calling the constructor of the type.
	 *
	 * usage:
	 * new <typename> [params]
	 *
	 * operands:
	 * -typename: the type identifier to create.
	 * -params: an optional int to specify the amount of params to send to the constructor,
	 * if is not defined will be the amount defined by the type itself or all the stack.
	 */
	newi,
	/*
	 * Push onto the stack the size in bytes of memory required by the type.
	 * This opcode can be used in two ways, with operand and without operand, if the
	 * operand is defined nothing is popped from the stack, and the size will be
	 * the size to create an instance of the target type.
	 *
	 * usage:
	 * sizeof [typename]
	 *
	 * operands:
	 * -typename: if defined, is the type identifier to get the size.
	 *
	 * stack transition:
	 * ... obj -> ... size
	 *
	 * - if no type is defined then a object will be popped off of the stack and
	 * push onto the stack the size of the type from the object.
	 */
	size,
	/*
	 * Store the value in a field of the object.
	 * Fields names depends on the type of the object.
	 *
	 * usage:
	 * stfld <field>
	 *
	 * operands:
	 * -field: the name of the field to store.
	 *
	 * stack transition:
	 * ... obj value -> ...
	 *
	 * - the object and the value are popped off of the stack and passed to the
	 * type to set the field.
	 */
	stfld,
	/*
	 * Load the value od a field from the object.
	 * Fields names depends on the type of the object.
	 *
	 * usage:
	 * ldfld <field>
	 *
	 * operands:
	 * -field: the name of the field to store.
	 *
	 * stack transition:
	 * ... obj -> ... value
	 *
	 * - the object is popped off of the stack and passed to the type to get
	 * the field.
	 * - the value of the field is pushed onto the stack.
	 */
	ldfld,
	/*
	 * Increase the ref count of the element on top of the stack but without change
	 * the stack.
	 *
	 * usage:
	 * ref
	 *
	 * no stack transitions.
	 */
	ref,
	/*
	 * Decrease the ref count of the element on top of the stack but without change
	 * the stack.
	 *
	 *  if the ref count of the object is now 0 the object will be destroyed leaving
	 *  a null reference in the stack, this is planned to be use only in destructors
	 *  followed with a pop.
	 *
	 * usage:
	 * deref
	 *
	 * no stack transitions.
	 */
	deref,
	/*
	 * Create a new instance of the target type and initialize it but not
	 * call the constructor.
	 *
	 * usage:
	 * ins <type>
	 *
	 * operands:
	 * -type: the type identifier to create.
	 *
	 * stack transition:
	 * ... -> ... obj
	 *
	 * - the newly created object will be pushed onto the stack.
	 */
	ins,

#pragma endregion

	/*
	 * Reserved
	 */
	reserved_ext = UINT8_MAX
};