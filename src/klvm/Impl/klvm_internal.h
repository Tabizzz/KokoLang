#pragma once

#include "klvm.h"
#include "DataTypes/KLFunctionImpl.h"
#include <memory>
#include <string>
#include <stdexcept>

using namespace std;

#define KLISNATIVE(x) (x->type == &klBType_Int ? 1 : x->type == &klBType_Float ? 2 : x->type == &klBType_Bool ? 3 : 0)

#define REP0(X)
#define REP1(X) X,
#define REP2(X) REP1(X) X,
#define REP3(X) REP2(X) X,
#define REP4(X) REP3(X) X,
#define REP5(X) REP4(X) X,
#define REP6(X) REP5(X) X,
#define REP7(X) REP6(X) X,
#define REP8(X) REP7(X) X,
#define REP9(X) REP8(X) X,
#define REP10(X) REP9(X) X

#define REP(TENS, ONES, X) \
  REP##TENS(REP10(X)) \
  REP##ONES(X)

#define GET_INT(x, y) if (y->type == klint_t) { \
x = KASINT(y);                                  \
} else if (y->type->toInt) {                    \
auto temp##x = y->type->toInt(y);               \
x = KASINT(temp##x);                            \
klDeref(temp##x);}

extern kl_int temp_int;

extern kl_float temp_float;

void kliDerefAndDeleteMap(MetaMap *pMap);

unordered_map<string, KLPackage *> *kliRootPackages();

KLPackage *kliBuildGlobalPackage();

template<typename ... Args>
std::string string_format(const std::string &format, Args ... args) {
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>( size_s );
	std::unique_ptr<char[]> buf(new char[size]);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside NOLINT(modernize-return-braced-init-list)
}