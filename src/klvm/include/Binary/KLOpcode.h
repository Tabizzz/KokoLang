#pragma once

/**
 * Available instructions for the klruntime.
 */
enum class KLOpcode : unsigned char
{
	/**
	 *	no code, used for labels only, can be used manually to do "nothing" in one instruction.
	 *
	 *	usage:
	 *	noc
	 */
	noc,

#pragma region local jumps

	/**
	 *  move to a target instruction by index, can be used with labels.
	 *
	 * usage:
	 * go <label>
	 *
	 * operators:
	 * -label: the target label name or the index of the target instruction.
	 */
	go,
	/**
	 *  move to a target instruction by index if the value in the reg is true
	 *  or not null, can be used with labels.
	 *
	 * usage:
	 * goif <label>
	 *
	 * operators:
	 * -label: the target label name or the index of the target instruction.
	 */
	goif,
	/**
	 *  move to a target instruction by index if the value in the reg is false
	 *  or null, can be used with labels.
	 *
	 * usage:
	 * goifn <label>
	 *
	 * operators:
	 * -label: the target label name or the index of the target instruction.
	 */
	goifn,

#pragma endregion

#pragma region register operations

	/**
	 * push an inline value into a register.
	 *
	 * usage:
	 * push <obj> <reg>
	 *
	 * operators:
	 * -obj: any primitive value to push in the stack.
	 * -reg: the register to save the value.
	 */
	push,
	/**
	 * pop a value from the specific register.
	 *
	 * usage:
	 * pop <reg>
	 *
	 * operators:
	 * -reg: the register to remove, this register will be set to null.
	 */
	pop,
	/**
	 * clone the element in one register to other register.
	 * this operation clone the object only if the type support the clone
	 * operation, if is not supported then a move operation is used.
	 *
	 * usage:
	 * cl <src> <dest>
	 *
	 * operators:
	 * -src: the source register to duplicate.
	 * -dest: the destination register, any current content will be
	 * overwritten.
	 */
	cl,
	/**
	 * Copy the value from one register to another.
	 * this operation is a value copy, if the source and destination
	 * objects are of the same type the value is copied based on the
	 * type, if the types are different the source type can define an
	 * operation to copy between types, if not operation is defined
	 * then this code act like cl code.
	 *
	 * usage:
	 * cp <src> <dest>
	 *
	 * operators:
	 * -src: the source register to copy.
	 * -dest: the destination register, any current content will be
	 * overwritten.
	 */
	cp,
	/**
	 * Move an object by reference, the current value in dest is deref.
	 *
	 * usage:
	 * mv <src> <dest>
	 *
	 * operators:
	 * -src: the source register to copy.
	 * -dest: the destination register, any current content will be
	 * overwritten.
	 */
	mv,
	/**
	 * Get the value of a flag of the call and save it as a bool.
	 *
	 * usage:
	 * lflag <index> <reg>
	 *
	 * operators:
	 * -index: the index of the flag.
	 * -reg: the register to save the value.
	 */
	lflag,

#pragma endregion

#pragma region variable manipulation

	/**
	 * Copy one register to a global variable.
	 * this copy operation acts like the cp code.
	 *
	 * usage:
	 * set <name> <reg>
	 *
	 * operators:
	 * -name: the identifier of the global variable.
	 * -reg: the register to operate.
	 */
	set,
	/**
	 * Copy one global variable to a register.
	 * this copy operation acts like the cp code.
	 *
	 * usage:
	 * get <name> <reg>
	 *
	 * operators:
	 * -name: the identifier of the global variable.
	 * -reg: the register to operate.
	 */
	get,
	/**
	 * Copy one register to an argument by index.
	 * this copy operation acts like the cp code.
	 *
	 * usage:
	 * starg <index> <reg>
	 *
	 * operators:
	 * -index: the index of the argument.
	 * -reg: the register to operate.
	 */
	starg,
	/**
	 * Copy one argument by index to a register.
	 * this copy operation acts like the cp code.
	 *
	 * usage:
	 * ldarg <index> <reg>
	 *
	 * operators:
	 * -index: the index of the argument.
	 * -reg: the register to operate.
	 */
	ldarg,

#pragma endregion

#pragma region boolean operations

	/**
	 * Computes the AND operation of two values and set the check flag.
	 * AND evaluates to true only if the two values are true.
	 *
	 * *A value is true if is different from 0, false or is not null
	 *
	 * usage:
	 * and <a> <b>
	 *
	 * operators:
	 * -a: first object to evaluate.
	 * -b: second object to evaluate.
	 */
	andi,
	/**
	 * Computes the OR operation of two values and set the check flag
	 * OR evaluates to true if one or two of the operands are true.
	 *
	 * *A value is true if is different from 0, false or is not null
	 *
	 * usage:
	 * or <a> <b>
	 *
	 * operators:
	 * -a: first object to evaluate.
	 * -b: second object to evaluate.
	 */
	ori,
	/**
	 * Computes the XOR operation of two values and set the check flag.
	 * XOR evaluates to true if and only if one of the operands are true.
	 *
	 * *A value is true if is different from 0, false or is not null
	 *
	 * usage:
	 * xor <a> <b>
	 *
	 * operators:
	 * -a: first object to evaluate.
	 * -b: second object to evaluate.
	 */
	xori,
	/**
	 * Computes the LT operation of two values and set the check flag.
	 * LT evaluates to true if the first operand is less than the second.
	 *
	 * usage:
	 * oplt <a> <b>
	 *
	 * operators:
	 * -a: first object to evaluate.
	 * -b: second object to evaluate.
	 */
	oplt,
	/**
	 * Computes the LE operation of two values and set the check flag.
	 * LE evaluates to true if the first operand is less than or equal to the second.
	 *
	 * usage:
	 * ople <a> <b>
	 *
	 * operators:
	 * -a: first object to evaluate.
	 * -b: second object to evaluate.
	 */
	ople,
	/**
	 * Computes the GT operation of two values and set the check flag.
	 * GT evaluates to true if the first operand is greater than the second.
	 *
	 * usage:
	 * opgt <a> <b>
	 *
	 * operators:
	 * -a: first object to evaluate.
	 * -b: second object to evaluate.
	 */
	opgt,
	/**
	 * Computes the GE operation of two values and set the check flag.
	 * GE evaluates to true if the first operand is greater than or equal to the second.
	 *
	 * usage:
	 * opge <a> <b>
	 *
	 * operators:
	 * -a: first object to evaluate.
	 * -b: second object to evaluate.
	 */
	opge,
	/**
	 * Computes the EQ operation of two values and set the check flag.
	 * EQ evaluates to true if the operands are equal, by default this means that are
	 * the same pointer.
	 *
	 * usage:
	 * opeq <a> <b>
	 *
	 * operators:
	 * -a: first object to evaluate.
	 * -b: second object to evaluate.
	 */
	opeq,
	/**
	 * Computes the NE operation of two values and set the check flag.
	 * NE evaluates to true if the operands are not equal, by default this means that
	 * are not the same pointer.
	 *
	 * usage:
	 * opne <a> <b>
	 *
	 * operators:
	 * -a: first object to evaluate.
	 * -b: second object to evaluate.
	 */
	opne,

#pragma endregion

#pragma region arithmetic operations

	/**
	 * Add two values and save the result in the given register.
	 *
	 * usage:
	 * add <a> <b> <res>
	 *
	 * operators:
	 * -a: the first register to operate.
	 * -b: the second register to operate.
	 * -res: the register to save the result.
	 */
	add,
	/**
	 * Subtract two values and save the result in the given register.
	 *
	 * usage:
	 * sub <a> <b> <res>
	 *
	 * operators:
	 * -a: the first register to operate.
	 * -b: the second register to operate.
	 * -res: the register to save the result.
	 */
	sub,
	/**
	 * Multiply two values and save the result in the given register.
	 *
	 * usage:
	 * mul <a> <b> <res>
	 *
	 * operators:
	 * -a: the first register to operate.
	 * -b: the second register to operate.
	 * -res: the register to save the result.
	 */
	mul,
	/**
	 * Divide the first value by the second value and save the result
	 * in the given register.
	 *
	 * usage:
	 * div <a> <b> <res>
	 *
	 * operators:
	 * -a: the first register to operate.
	 * -b: the second register to operate.
	 * -res: the register to save the result.
	 */
	div,
	/**
	 * Compute the modulo of the first value by the second value and
	 * save the result in the given register.
	 *
	 * usage:
	 * mod <a> <b> <res>
	 *
	 * operators:
	 * -a: the first register to operate.
	 * -b: the second register to operate.
	 * -res: the register to save the result.
	 */
	mod,

#pragma endregion

#pragma region convertion to types

	/**
	 * Convert the value to the string representation and save the value
	 * on the given register.
	 *
	 * usage:
	 * tstr <src> <dest>
	 *
	 * operators:
	 * -src: the object to convert.
	 * -dest: the register to save the conversion.
	 */
	tstr,
	/**
	 * Convert the value to the int representation and save the value
	 * on the given register.
	 *
	 * usage:
	 * tint <src> <dest>
	 *
	 * operators:
	 * -src: the object to convert.
	 * -dest: the register to save the conversion.
	 */
	tint,
	/**
	 * Convert the value to the float representation and save the value
	 * on the given register.
	 *
	 * usage:
	 * tflt <src> <dest>
	 *
	 * operators:
	 * -src: the object to convert.
	 * -dest: the register to save the conversion.
	 */
	tflt,
	/**
	 * Convert the value to the bool representation and save the value
	 * on the given register.
	 *
	 * usage:
	 * tbit <src> <dest>
	 *
	 * operators:
	 * -src: the object to convert.
	 * -dest: the register to save the conversion.
	 */
	tbit,
	/**
	 * Convert an object to another type, this conversion is called
	 * on the type of the object, not in the target type.
	 *
	 * usage:
	 * tobj <type> <src> <dest>
	 *
	 * -type: the target type identifier to convert.
	 * -src: the object to convert.
	 * -dest: the register to save the conversion.
	 */
	tobj,
	/**
	 * Convert an object to another type, this conversion is called
	 * on the target type, not in the current type of the object.
	 *
	 * usage:
	 * cast <type> <src> <dest>
	 *
	 * -type: the target type identifier to convert.
	 * -src: the object to convert.
	 * -dest: the register to save the conversion.
	 */
	cast,

#pragma endregion

#pragma region function related

	/**
	 * Invoke a target object, if the function return something the value will be set
	 * in the given register.
	 *
	 * usage:
	 * jump <obj> <ret> [params]*
	 *
	 * operators:
	 * -obj: the register with the object to invoke.
	 * -ret: a register to save the return value of the function, can be null to ignore.
	 * -params: a list of registers used as params of the called object.
	 */
	ivk,
	/**
	 * Invoke a object, if the invocation return something the value will be set in
	 * the given register.
	 *
	 * usage:
	 * jump <obj> <ret> <params>
	 *
	 * operators:
	 * -obj: The register with the object to invoke.
	 * -ret: a register to save the return value of the function, can be null to ignore.
	 * -params: the register with an array to be used as args of the invoke if the
	 * register not contains an array the call will fail.
	 */
	ivka,
	/**
	 * Invoke a target function, if the function return something the value will be set
	 * in the given register.
	 *
	 * usage:
	 * call <function> <ret> [params]*
	 *
	 * -function: the identifier of the function to call.
	 * -ret: a register to save the return value of the function, can be null to ignore.
	 * -params: a list of registers used as params of the called function.
	 */
	call,
	/**
	 * Invoke a target function, if the function return something the value will be set
	 * in the given register.
	 *
	 * usage:
	 * call <function> <ret> <params>
	 *
	 * -function: the identifier of the function to call.
	 * -ret: a register to save the return value of the function, can be null to ignore.
	 * -params: the register with an array to be used as args of the function if the
	 * register not contains an array the call will fail.
	 */
	calla,
	/**
	 * Set in a register the number of arguments passed to the function.
	 *
	 * usage:
	 * argc <reg>
	 *
	 * -reg: register to save the value.
	 */
	argc,
	/**
	 * Exit from the current function and return to the caller with an optional
	 * value.
	 *
	 * usage:
	 * ret [reg]
	 *
	 * operators:
	 * -reg: an optional register to return.
	 */
	ret,

#pragma endregion

#pragma region memory

	/**
	 * Allocates a block of memory of arbitrary size and save the pointer
	 * to the beginning of the block in the given register.
	 *
	 * usage:
	 * aloc <size> <reg>
	 *
	 * operators:
	 * -size: an int indicating the size in bytes of the block.
	 * -reg: the register to save the pointer.
	 */
	aloc,
	/** Free a previously allocated block of memory by the pointer to the block.
	 *
	 * usage:
	 * free <ptr>
	 *
	 * operators:
	 * -ptr: the register that contains the pointer to free.
	 */
	freei,
	/**
	 * Copy bytes from a memory block to another block, memory blocks are referenced
	 * by pointers.
	 *
	 * usage:
	 * copy <src> <dest> <size>
	 *
	 * operators:
	 * -src: the register with the source pointer.
	 * -dest: the register with the destination pointer.
	 * -size: an integer with the amount of bytes to copy or a register with the
	 * integer.
	 */
	copy,
	/**
	 * Fill the target block of memory with a specific value.
	 *
	 * usage:
	 * fill <dest> <value> <size>
	 *
	 * operators:
	 * -dest: the register with the target pointer.
	 * -value: a register with the value to set or an integer.
	 * -size: an integer with the amount of bytes to copy or a register with the
	 * integer.
	 */
	fill,

#pragma endregion

#pragma region arrays

	/**
	 * Creates a new array object of the specific size.
	 *
	 * usage:
	 * arr <reg> <sizes>+
	 *
	 * operands:
	 * -reg: the register to save the new array.
	 * -sizes: this can be a register to an array of sizes, or you can pass a
	 * list of sizes or registers to sizes.
	 */
	arr,
	/**
	 * Save in the given register the length of the array.
	 *
	 * usage:
	 * arl <reg> <out> <dimension>
	 *
	 * operands:
	 * -reg: the register with the array.
	 * -out: the register to save the length.
	 * -dimension: the dimension of the array to get the size. Can be an int or a register.
	 */
	arl,
	/**
	 * Save in the given register the dimension count of an array.
	 *
	 * usage:
	 * ard <reg>
	 *
	 * operators:
	 * -reg: the register containing the array.
	 */
	ard,
	/**
	 * Load the element of the array at the specific index.
	 *
	 * usage:
	 * lde <reg> <dest> <index>+
	 *
	 * operators:
	 * -reg: the array to get the element.
	 * -dest: the register to save the element.
	 * -index: this can be a register to an array of indices, or you can pass a
	 * list of indices or registers to indices.
	 */
	lde,
	/**
	 * Store the element of the array at the specific index.
	 *
	 * usage:
	 * ste <reg> <src> <index>+
	 *
	 * operators:
	 * -reg: the array to get the element.
	 * -src: the element to set in the array.
	 * -index: this can be a register to an array of indices, or you can pass a
	 * list of indices or register to indices.
	 */
	ste,

#pragma endregion

#pragma region types

	/**
	 * Save in the given register the runtime pointer of the given type.
	 *
	 * usage:
	 * type <typename> <reg>
	 *
	 * operands:
	 * -typename: the identifier of the type.
	 * -reg: the register to save the type.
	 */
	type,
	/**
	 * Save in the given register the runtime pointer of the type from the
	 * object.
	 *
	 * usage:
	 * typeof <obj> <reg>
	 *
	 * operators:
	 * -obj: the object to get the type.
	 * -reg: where to save the type.
	 */
	typeofi,
	/**
	 * Determines if an object is an instance of the given type.
	 *
	 * usage:
	 * is <typename> <obj>
	 *
	 * operands:
	 * -typename: the name of the type.
	 * -obj: the register with the object to check.
	 */
	is,

#pragma endregion

#pragma region structs

	/**
	 * Create a new instance of the given type calling the constructor of the type.
	 *
	 * usage:
	 * new <typename> <reg> [params]*
	 *
	 * operands:
	 * -typename: the type identifier to create.
	 * -reg: a register to save the created object.
	 * -params: a list of registers or values used as params of the constructor.
	 */
	newi,
	/**
	 * Create a new instance of the given type calling the constructor of the type.
	 *
	 * usage:
	 * new <typename> <reg> <params>
	 *
	 * operands:
	 * -typename: the type identifier to create.
	 * -reg: a register to save the created object.
	 * -params: a register with an array with the params to pass to the constructor.
	 */
	newa,
	/**
	 * Push onto the stack the size in bytes of memory required by the type.
	 * This value don't include the header size.
	 *
	 * usage:
	 * sizeof (typename|register) <out>
	 *
	 * operands:
	 * -typename: if defined, is the type identifier to get the size.
	 * -register: if defined, is a register to the object to get the size.
	 * -out: the register to save the size.
	 */
	sizeofi,
	/**
	 * Store the value in a field of the object.
	 * Fields names depends on the type of the object.
	 *
	 * usage:
	 * stfld <field> <obj> <reg>
	 *
	 * operands:
	 * -field: the name of the field to store.
	 * -obj: the object to set the field.
	 * -reg: the register with the value to set in the field.
	 */
	stfld,
	/**
	 * Load the value of a field from the object.
	 * Fields names depends on the type of the object.
	 *
	 * usage:
	 * ldfld <field> <obj> <reg>
	 *
	 * operands:
	 * -field: the name of the field to store.
	 * -obj: the object to get the field.
	 * -reg: the register to save the field.
	 */
	ldfld,
	/**
	 * Increase the ref count of the object.
	 *
	 * usage:
	 * ref <reg>
	 *
	 * operators:
	 * -reg: the register with the object to increase the ref count.
	 */
	ref,
	/**
	 * Decrease the ref count of the object.
	 *
	 *  if the ref count of the object is now 0 the object will be destroyed leaving
	 *  a null reference in the register, this is planned to be use only in destructors.
	 *
	 * usage:
	 * deref <reg>
	 *
	 * operators:
	 * -reg: the register with the object to increase the ref count.
	 */
	deref,
	/**
	 * Create a new instance of the target type and initialize it but not
	 * call the constructor.
	 *
	 * usage:
	 * ins <type> <reg>
	 *
	 * operands:
	 * -type: the type identifier to create.
	 * -reg: register to save the new object.
	 */
	ins,
	/**
	 * Loads a function object from a type and save it in the given register.
	 *
	 * usage:
	 * ldfnd <identifier> <obj> <reg>
	 *
	 * operands:
	 * -identifier: the name of the function to search.
	 * -obj: the object on which to get the function.
	 * -reg: register to save the function.
	 */
	ldfnd,

#pragma endregion

	/**
	 * Reserved
	 */
	reserved_ext = UINT8_MAX
};