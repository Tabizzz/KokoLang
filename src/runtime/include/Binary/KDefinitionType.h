#pragma once

/**
 * Type of definitions available for binary format
 */
enum class KDefinitionType : unsigned char
{
	close,
	variable,
	function,
	type,
	subpackage
};