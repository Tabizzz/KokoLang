#pragma once

/**
 * Types of raw object available in binary format
 */
enum class KMetaType : unsigned char
{
	null,
	bfalse,
	btrue,
	integer,
	number,
	string,
	reg
};