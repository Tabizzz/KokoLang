#pragma once

#include "DataTypes/KLObject.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represent a variable of the runtime.
 *
 * Variables can be of two types, package variables and type variables.
 *
 * @section Type Type variables:
 *
 * Type variables are always defined, and can have a default value, the default value is set to the variable in an object when is instanced.
 *
 * Type variables works with offset, the offset tells the runtime where in the memory space of an object is saved the variable.
 *
 * @section Pack Package variables:
 *
 * Package variables can be in two states: defined or undefined, a defined variable can be read, while an undefined variable will
 * give an error when trying to read it, a variable is always undefined by default and is defined by assigning any value to it.
 *
 * Package variables store the value in themselves since they are global so they do not need to be offset.
 */
struct CPPAPI KLVariable
{
	KLOBJECTHEAD
	/**
	 * @brief Holds the union of type and package variables.
	 */
	union {
		/**
		 * @brief Save the info for a type variable.
		 */
		struct {
			/**
			 * @brief Default value of the variable.
			 *
			 * By default is null, that is, when an new instance of an object is created this variable will be set to null.
			 */
			KlObject* defaultValue;
			/**
			 * @brief This byte is always 0 in type variable.
			 */
			kbyte type;
			/**
			 * @brief The offset of the variable.
			 *
			 * An offset of 0 indicates that the variable is in memory just after the object header.
			 * An offset of 1 indicates that the variable is in the second pointer after the header, in 32bits systems
			 * the pointers have a size of 4 bytes and in 64bits systems it is 8 bytes, the offset is measured by pointer
			 * size and not by bytes.
			 */
			kbyte offset;
		} typevar;
		/**
		 * @brief Save the info for a package variable.
		 */
		struct {
			/**
			 * @brief The value of the variable.
			 *
			 * This is not set on creation and can contain any value while undefined.
			 */
			KlObject* value;
			/**
			 * @brief This byte is always 1 in package variable.
			 */
			kbyte type;
			/**
			 * @brief Indicates that the variable is defined and have a valid value.
			 */
			kbyte defined;
		} packvar;
	} data;
	/**
	 * @brief Custom metadata of the variable.
	 */
	MetaMap* metadata;
};

#ifdef __cplusplus
}
#endif

CAPI
/**
 * @brief Set the value of a variable.
 *
 * This method is the responsible of mark package variables as defined.
 *
 * @param variable The variable to set.
 * @param target The object in which you want to set the variable, if is null then is a package variable.
 * @param value The value to save in the variable.
 */
void klSetVariable(KLVariable* variable, KlObject* target, KlObject* value);

CAPI
/**
 * @brief Get the value of a variable.
 *
 * This throws and error if try to read a global undefined variable.
 *
 * @param variable The variable to read.
 * @param target The object in which you want to get the variable, if is null then is a package variable.
 * @return The value held by the variable.
 */
KlObject* klGetVariable(KLVariable* variable, KlObject* target);

CAPI
/**
 * @brief Type for variables.
 */
KLType* klvar_t;