#pragma once

/**
 * @brief A function that receives an object an changes its state.
 */
typedef void (*kltypefunc)(KLObject* obj);

typedef KLObject* (*klinvokable)			// a delegate for invokable objects like constructors or functions
		(KLObject* func,					// the function object itself
		 KLObject* args[],					// the args passed to the function
		 kbyte);							// the number of arguments passed to the function

typedef int8_t (*klcomparer)				// comparator signature
		(KLObject* first,
		 KLObject* second);

typedef void (*klcopyop)				    // the copy operation
		(KLObject* first,
		 KLObject* second);

typedef KLObject* (*klbinaryop)				// a binary operator, receive two objects and returns a new one
		(KLObject* first,
		 KLObject* second);

typedef void (*klsetop)						// a ternary operator, receive three objects
	(KLObject* first,
	 KLObject* second,
	 KLObject* value);

typedef void (* klresoperation)             // binary operation with a result
		(KLObject* first,
		 KLObject* second,
		 KLObject** target);

typedef KLObject* (*klunaryop)				// a unary operation, receive one object and returns a new one
		(KLObject* obj);