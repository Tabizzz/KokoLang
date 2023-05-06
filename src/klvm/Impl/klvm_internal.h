#pragma once
#include "klvm.h"
#include "DataTypes/KLFunctionImpl.h"
#include <memory>
#include <string>
#include <stdexcept>

using namespace std;

#define KLISNATIVE(x) (x->type == &klBType_Int ? 1 : x->type == &klBType_Float ? 2 : x->type == &klBType_Bool ? 3 : 0)

extern kl_int temp_int;

extern kl_float temp_float;

void kliDerefAndDeleteMap(MetaMap* pMap);

map<string, KLPackage*>* kliRootPackages();

KLPackage* kliBuildGlobalPackage();

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
	int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
	if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
	auto size = static_cast<size_t>( size_s );
	std::unique_ptr<char[]> buf( new char[ size ] );
	std::snprintf( buf.get(), size, format.c_str(), args ... );
	return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}