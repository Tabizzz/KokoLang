#pragma onces

typedef void (*klinitializer)(KlObject*);   // the initializer is the responsible for example set ints to 0

typedef void (*klfinalizer)(KlObject*);     // the finalizer is the responsible for example free memory allocation/ deref other object

typedef KlObject* (*klinvokable)			// a delegate for invokable objects like constructors or functions
		(KlObject *func,						// the function object itself
		 KlObject **args,					// the args passed to the function
		 KlObject *argc);					// the number of arguments passed to the function

typedef int8_t (*klcomparer)					// comparator signature
		(KlObject* first,
		 KlObject* second);

typedef void (*klcopy)				        // the copy operation
		(KlObject* first,
		 KlObject* second);

typedef KlObject* (*klbinaryop)				// a binary operator, receive two objects and returns a new one
		(KlObject* first,
		 KlObject* second);

typedef void (* klresoperation)             // binary operation with a result
		(KlObject* first,
		 KlObject* second,
		 KlObject** target);

typedef KlObject* (*klunaryop)				// a unary operation, receive one object and returns a new one
		(KlObject* obj);