#pragma once

/**
 * @brief A function that receives an object an changes its state.
 */
typedef void (*kltypefunc)(KlObject* obj);

typedef KlObject* (*klinvokable)			// a delegate for invokable objects like constructors or functions
		(KlObject* func,					// the function object itself
		 KlObject* args[],					// the args passed to the function
		 kbyte);							// the number of arguments passed to the function

typedef int8_t (*klcomparer)				// comparator signature
		(KlObject* first,
		 KlObject* second);

typedef void (*klcopyop)				    // the copy operation
		(KlObject* first,
		 KlObject* second);

typedef KlObject* (*klbinaryop)				// a binary operator, receive two objects and returns a new one
		(KlObject* first,
		 KlObject* second);

typedef void (*klsetop)						// a ternary operator, receive three objects
	(KlObject* first,
	 KlObject* second,
	 KlObject* value);

typedef void (* klresoperation)             // binary operation with a result
		(KlObject* first,
		 KlObject* second,
		 KlObject** target);

typedef KlObject* (*klunaryop)				// a unary operation, receive one object and returns a new one
		(KlObject* obj);