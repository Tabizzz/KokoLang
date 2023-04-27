#pragma once

/**
 * Type of definitions available for binary format
 */
enum class KLDefinitionType : unsigned char
{
	close,
	variable,
	function,
	type,
	subpackage
};