#pragma once

#include "DataTypes/KLObject.h"

#include "kldelegates.h"

#define KLTYPE_FLAG_NOINSTANCE (1 << 0)

#define KLTYPE_METADATA(type)        \
type->functions = new MetaMap();     \
type->variables = new MetaMap();     \
type->metadata = new MetaMap();

struct CPPAPI KLTypeFlags {
	/**
	 * @brief The type cannot be instanced, this is for types that you will manually
	 * instance and cannot be instanced by klruntime.
	 */
	kbyte no_instance: 1;
};

/**
 * @brief Contains the functions and info of how a object behaves.
 */
struct CPPAPI KLType {
	KLOBJECTHEAD
	/**
	 * @brief The name of the type.
	 *
	 * Is a C null terminated string.
	 */
	const char *name;
	/**
	 * @brief The size to allocate an object of this type.
	 */
	size_t size;

	/**
	 * @brief The number of instances of this type created by the runtime.
	 *
	 * When creating a new type this field must be 0.
	 */
	size_t inscount;

	union {
		/**
		 * @brief Flags to enable special features on types.
		 */
		kbyte flags;
		/**
		 * @brief Strcut version of the type flags.
		 */
		KLTypeFlags rflags;
	};

	/**
	 * @brief Functions for creating or destroying a object.
	 */
	struct {
		/**
		 * @brief The initializer of this type. Has the work of prepare the new allocated memory region.
		 *
		 * In the initializer dont create pointers to dynamic memory, do that on the constructor, use the initializer
		 * just to prepare raw memory. The default initializer set all the memory to 0.
		 */
		kltypefunc initializer;
		/**
		 * @brief The finalizer of this type. Has the work of deallocate owned pointers and deref others KLObjects.
		 */
		kltypefunc finalizer;
		/**
		 * @brief The object constructor. Has the work to allocate dynamic memory if necessary and configure the
		 * object based on passed arguments.
		 *
		 * This must be a KLFunction.
		 */
		KLObject *constructor;      // the constructor of the type
	} KLManagingFunctions;

	/**
	 * @brief Functions for converting objects to another type.
	 *
	 * Each builtin type has its own conversion operation. Other conversions are implemented on the toType operation.
	 * Each conversion return a strong reference, if the target type is not supported must return null.
	 */
	struct {
		/**
		 * @brief Convert a object to string type.
		 *
		 * This operation is used to get the string representation of a object.
		 * The runtime provides a default implementation if none is provided.
		 */
		klunaryop toString;
		/**
		 * @brief Convert a object to int.
		 *
		 * If a type is convertible to int then can be used to access a list by index.
		 */
		klunaryop toInt;
		/**
		 * @brief Convert a object to float.
		 */
		klunaryop toFloat;
		/**
		 * @brief Convert a object to bool.
		 *
		 * This operation used in logical operations like 'or', 'and' and 'xor'.
		 * The runtime default implementation always return true.
		 */
		klunaryop toBool;
		/**
		 * @brief Convert a object to list.
		 */
		klunaryop toList;
		/**
		 * @brief Convert a object to map.
		 */
		klunaryop toMap;
		/**
		 * @brief Convert a object to another type.
		 */
		klbinaryop toType;
		/**
		 * @brief Called on this type to convert a object of another type to this type.
		 */
		klbinaryop cast;
	} KLConversionFunctions;

	/**
	 * @brief Functions to comparing objects with others
	 */
	struct {
		/**
		 * @brief The type comparer. This is a comparing for ordering not for equality.
		 *
		 * The comparer must return:
		 * @return -1 if first is considered greater than second.
		 * @return 0 if first and second are considered equal.
		 * @return 1 if first is considered less than second
		 *
		 * Adding this operation to a type enable the operators: \<, >, \<= and >=
		 */
		klcomparer comparer;
		/**
		 * @brief The type equaily checker. This check if two instances are considered equals.
		 *
		 * @return 1 if the objects are considered equals, 0 otherwise.
		 *
		 * This operation is used for the operator == and !=.
		 */
		klcomparer equal;
	} KLComparerFunctions;

	/**
	 * @brief Functions to add support for arithmetic operations.
	 */
	struct {
		/**
		 * @brief Add support for addition operation (+ operator).
		 */
		klbinaryop opAdd;
		/**
		 * @brief Add support for subtraction operation (- operator).
		 */
		klbinaryop opSub;
		/**
		 * @brief Add support for multiplication operation (* operator).
		 */
		klbinaryop opMul;
		/**
		 * @brief Add support for division operation (/ operator).
		 */
		klbinaryop opDiv;
		/**
		 * @brief Add support for modulo operation (% operator).
		 */
		klbinaryop opMod;
		/**
		 * @brief Add support for power operation (° operator).
		 */
		klbinaryop opPow;
		/**
		 * @brief Add support for the increment operation (++ operator).
		 */
		klunaryop opInc;
		/**
		 * @brief Add support for the decrement operation (-- operator).
		 */
		klunaryop opDec;
	} KLNumericFunctions;

	/**
	 * @brief Functions to define how objects behaves when the runtime try to move objects.
	 *
	 * By default the runtime copies object by reference so assigning values will not create new objects, this can
	 * be change by implementing the operations of this struct.
	 */
	struct {
		/**
		 * @brief Create a copy of the object that should be the same as the original object.
		 *
		 * If this operation is implemented the runtime will create a clone of the object when assigning values.
		 */
		klunaryop clone;
		/**
		 * @brief Copy the raw value of and object to another object. This work on two objects of the same
		 * type an never receives a null value.
		 */
		klcopyop copy;
	} KLRefFunctions;

	/**
	 * @brief Logical, bitwise and shift support functions.
	 */
	struct {
		/**
		 * @brief Add support for bitwise operation (~ operator).
		 */
		klunaryop bitWise;
		/**
		 * @brief Add support for left shift operation (<< operator).
		 */
		klbinaryop leftShift;
		/**
		 * @brief Add support for right shift operation (>> operator).
		 */
		klbinaryop rightShift;
		/**
		 * @brief Add support for logical and operation (& operator).
		 */
		klbinaryop logAnd;
		/**
		 * @brief Add support for logical or operation (| operator).
		 */
		klbinaryop logOr;
		/**
		 * @brief Add support for logical xor operation (^ operator).
		 */
		klbinaryop logXor;
	} KlBitFunctions;

	/**
	 * @brief Functions related to enumerable objects.
	 */
	struct {
		/**
		 * @brief Allow this type to support custom size getter.
		 */
		klsizeget osize;
		/**
		 * @brief Item getter for this type (a[b]).
		 */
		klbinaryop getter;
		/**
		 * @brief Item setter for this type (a[b] = c).
		 */
		klsetop setter;
	} KLIndexingFunctions;

	/**
	 * @brief Functions for binary i/o.
	 */
	struct {
		/**
		 * @brief Write the text representation of this object into a stream.
		 */
		klserializer sRepr;
		/**
		 * @brief Try to create a object from a stream using the text representation.
		 *
		 * This operation is used to parse things.
		 */
		kldeserializer dRepr;
		/**
		 * @brief Serialize this type in binary format.
		 */
		klserializer sBin;
		/**
		 * @brief Read this type in binary format.
		 */
		kldeserializer dBin;
	} KLStreamFunctions;

	/* Insert new operations here before maps */

	/**
	 * @brief The functions defined in the type.
	 */
	MetaMap *functions;
	/**
	 * @brief The public variable fields of the type.
	 */
	MetaMap *variables;
	/**
	 * @brief The metadata of the type.
	 */
	MetaMap *metadata;
};