#include "catch.hpp"
#include "klvm.h"
#include "testmacros.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-const-int-float-conversion"
TEST_CASE("kl_int", "[klapi_types][kl_int]")
{
	klInit();

	SECTION("comparer")
	{
		auto x = KLINT(10);
		SECTION("With others ints")
		{
			SECTION("Same value")
			{
				auto y = KLINT(10);
				REQUIRE(klint_t->KLComparerFunctions.comparer(x, y) == 0);
				klDeref(y);
			}
			SECTION("Lesser value")
			{
				auto y = KLINT(0);
				REQUIRE(klint_t->KLComparerFunctions.comparer(x, y) < 0);
				klDeref(y);
			}
			SECTION("Greater value")
			{
				auto y = KLINT(500);
				REQUIRE(klint_t->KLComparerFunctions.comparer(x, y) > 0);
				klDeref(y);
			}
		}
		SECTION("With types convertible to ints")
		{
			SECTION("Same value")
			{
				auto y = KLFLOAT(10);
				REQUIRE(klint_t->KLComparerFunctions.comparer(x, y) == 0);
				klDeref(y);
			}
			SECTION("Lesser value")
			{
				auto y = KLFLOAT(0);
				REQUIRE(klint_t->KLComparerFunctions.comparer(x, y) < 0);
				klDeref(y);
			}
			SECTION("Greater value")
			{
				auto y = KLFLOAT(500);
				REQUIRE(klint_t->KLComparerFunctions.comparer(x, y) > 0);
				klDeref(y);
			}
		}
		klDeref(x);

		SECTION("With types not convertible to ints")
		{
			// other types are always considered 0 if are not convertible to ints

			GIVEN("A string")
			{
				auto val = KLSTR("10");
				SECTION("Same value")
				{
					auto y = KLINT(0);
					REQUIRE(klint_t->KLComparerFunctions.comparer(y, val) == 0);
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLINT(-12);
					REQUIRE(klint_t->KLComparerFunctions.comparer(y, val) > 0);
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLINT(16);
					REQUIRE(klint_t->KLComparerFunctions.comparer(y, val) < 0);
					klDeref(y);
				}
				klDeref(val);
			}
			GIVEN("A bool")
			{
				auto val = KLBOOL(true);
				SECTION("Same value")
				{
					auto y = KLINT(1);
					REQUIRE(klint_t->KLComparerFunctions.comparer(y, val) == 0);
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLINT(-12);
					REQUIRE(klint_t->KLComparerFunctions.comparer(y, val) > 0);
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLINT(16);
					REQUIRE(klint_t->KLComparerFunctions.comparer(y, val) < 0);
					klDeref(y);
				}
				klDeref(val);
			}
			GIVEN("A null")
			{
				auto val = nullptr;
				SECTION("Same value")
				{
					auto y = KLINT(0);
					REQUIRE(klint_t->KLComparerFunctions.comparer(y, val) == 0);
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLINT(-12);
					REQUIRE(klint_t->KLComparerFunctions.comparer(y, val) > 0);
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLINT(16);
					REQUIRE(klint_t->KLComparerFunctions.comparer(y, val) < 0);
					klDeref(y);
				}
			}
		}
	}

	SECTION("Equality")
	{
		auto x = KLINT(10);
		SECTION("With others ints")
		{
			SECTION("Same value")
			{
				auto y = KLINT(10);
				REQUIRE(klint_t->KLComparerFunctions.equal(x, y));
				klDeref(y);
			}
			SECTION("Lesser value")
			{
				auto y = KLINT(0);
				REQUIRE_FALSE(klint_t->KLComparerFunctions.equal(x, y));
				klDeref(y);
			}
			SECTION("Greater value")
			{
				auto y = KLINT(500);
				REQUIRE_FALSE(klint_t->KLComparerFunctions.equal(x, y));
				klDeref(y);
			}
		}
		SECTION("With types convertible to ints")
		{
			SECTION("Same value")
			{
				auto y = KLFLOAT(10);
				REQUIRE(klint_t->KLComparerFunctions.equal(x, y));
				klDeref(y);
			}
			SECTION("Lesser value")
			{
				auto y = KLFLOAT(0);
				REQUIRE_FALSE(klint_t->KLComparerFunctions.equal(x, y));
				klDeref(y);
			}
			SECTION("Greater value")
			{
				auto y = KLFLOAT(500);
				REQUIRE_FALSE(klint_t->KLComparerFunctions.equal(x, y));
				klDeref(y);
			}
		}
		klDeref(x);

		SECTION("With types not convertible to ints")
		{
			// other types are always considered 0 if are not convertible to ints

			GIVEN("A string")
			{
				auto val = KLSTR("10");
				SECTION("Same value")
				{
					auto y = KLINT(0);
					REQUIRE(klint_t->KLComparerFunctions.equal(y, val));
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLINT(-12);
					REQUIRE_FALSE(klint_t->KLComparerFunctions.equal(y, val));
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLINT(16);
					REQUIRE_FALSE(klint_t->KLComparerFunctions.equal(y, val));
					klDeref(y);
				}
				klDeref(val);
			}
			GIVEN("A bool")
			{
				auto val = KLBOOL(true);
				SECTION("Same value")
				{
					auto y = KLINT(0);
					REQUIRE(klint_t->KLComparerFunctions.equal(y, val));
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLINT(-12);
					REQUIRE_FALSE(klint_t->KLComparerFunctions.equal(y, val));
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLINT(16);
					REQUIRE_FALSE(klint_t->KLComparerFunctions.equal(y, val));
					klDeref(y);
				}
				klDeref(val);
			}
			GIVEN("A null")
			{
				auto val = nullptr;
				SECTION("Same value")
				{
					auto y = KLINT(0);
					REQUIRE(klint_t->KLComparerFunctions.equal(y, val));
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLINT(-12);
					REQUIRE_FALSE(klint_t->KLComparerFunctions.equal(y, val));
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLINT(16);
					REQUIRE_FALSE(klint_t->KLComparerFunctions.equal(y, val));
					klDeref(y);
				}
			}
		}
	}

	SECTION("to_str")
	{
		KLObject* value = nullptr;
		KLObject* str = nullptr;
		KLObject* convert = nullptr;

		EXCHANGE(value, KLINT(10), str, KLSTR("10"), convert, klint_t->KLConversionFunctions.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLINT(10000000), str, KLSTR("10000000"), convert, klint_t->KLConversionFunctions.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLINT(0), str, KLSTR("0"), convert, klint_t->KLConversionFunctions.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLINT(-100), str, KLSTR("-100"), convert, klint_t->KLConversionFunctions.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLINT(-0), str, KLSTR("0"), convert, klint_t->KLConversionFunctions.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLINT(INT64_MAX), str, KLSTR("9223372036854775807"), convert, klint_t->KLConversionFunctions.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		klDeref(value);
		klDeref(str);
		klDeref(convert);
	}

	SECTION("to_flt")
	{
		KLObject* value = nullptr;
		KLObject* str = nullptr;
		KLObject* convert = nullptr;

		EXCHANGE(value, KLINT(10), str, KLFLOAT(10), convert, klint_t->KLConversionFunctions.toFloat(value))
		REQUIRE(KASFLOAT(convert) == KASFLOAT(str));

		EXCHANGE(value, KLINT(10000000), str, KLFLOAT(10000000), convert, klint_t->KLConversionFunctions.toFloat(value))
		REQUIRE(KASFLOAT(convert) == KASFLOAT(str));

		EXCHANGE(value, KLINT(0), str, KLFLOAT(0), convert, klint_t->KLConversionFunctions.toFloat(value))
		REQUIRE(KASFLOAT(convert) == KASFLOAT(str));

		EXCHANGE(value, KLINT(-100), str, KLFLOAT(-100), convert, klint_t->KLConversionFunctions.toFloat(value))
		REQUIRE(KASFLOAT(convert) == KASFLOAT(str));

		EXCHANGE(value, KLINT(-0), str, KLFLOAT(0), convert, klint_t->KLConversionFunctions.toFloat(value))
		REQUIRE(KASFLOAT(convert) == KASFLOAT(str));

		EXCHANGE(value, KLINT(INT64_MAX), str, KLFLOAT(INT64_MAX), convert, klint_t->KLConversionFunctions.toFloat(value))
		REQUIRE(KASFLOAT(convert) == KASFLOAT(str));

		klDeref(value);
		klDeref(str);
		klDeref(convert);
	}

	SECTION("to_bit")
	{
		KLObject* value = nullptr;
		KLObject* str = nullptr;
		KLObject* convert = nullptr;

		EXCHANGE(value, KLINT(10), str, nullptr, convert, klint_t->KLConversionFunctions.toBool(value))
		REQUIRE(KASBOOL(convert) == true);

		EXCHANGE(value, KLINT(0), str, nullptr, convert, klint_t->KLConversionFunctions.toBool(value))
		REQUIRE(KASBOOL(convert) == false);

		EXCHANGE(value, KLINT(-10), str, nullptr, convert, klint_t->KLConversionFunctions.toBool(value))
		REQUIRE(KASBOOL(convert) == true);

		EXCHANGE(value, KLINT(-0), str, nullptr, convert, klint_t->KLConversionFunctions.toBool(value))
		REQUIRE(KASBOOL(convert) == false);

		klDeref(value);
		klDeref(str);
		klDeref(convert);
	}

	SECTION("add")
	{
		auto x = KLINT(10);
		SECTION("With others ints")
		{
			SECTION("Positive")
			{
				auto y = KLINT(10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				REQUIRE(KASINT(res) == 20);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLINT(-10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				REQUIRE(KASINT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLINT(INT64_MAX);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == -9223372036854775799);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLINT(INT64_MIN);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == -9223372036854775798);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLINT(0);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
		}
		SECTION("With floats")
		{
			SECTION("Positive")
			{
				auto y = KLFLOAT(10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 20);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLFLOAT(-10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLFLOAT(INT64_MAX);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 9223372036854775808.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLFLOAT(INT64_MIN);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == -9223372036854775798);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLFLOAT(0);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
		}
		SECTION("With type not convertible to int")
		{
			SECTION("Positive")
			{
				auto y = KLSTR("10");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLSTR("-10");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLSTR("INT64_MAX");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLSTR("INT64_MIN");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLSTR("0");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opAdd(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
		}
		klDeref(x);
	}

	SECTION("subtract")
	{
		auto x = KLINT(10);
		SECTION("With others ints")
		{
			SECTION("Positive")
			{
				auto y = KLINT(10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				REQUIRE(KASINT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLINT(-10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				REQUIRE(KASINT(res) == 20);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLINT(INT64_MAX);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == -9223372036854775797);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLINT(INT64_MIN);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == -9223372036854775798);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLINT(0);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
		}
		SECTION("With floats")
		{
			SECTION("Positive")
			{
				auto y = KLFLOAT(10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLFLOAT(-10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 20);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLFLOAT(INT64_MAX);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == -9223372036854775797);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLFLOAT(INT64_MIN);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 9223372036854775808.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLFLOAT(0);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
		}
		SECTION("With type not convertible to int")
		{
			SECTION("Positive")
			{
				auto y = KLSTR("10");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLSTR("-10");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLSTR("INT64_MAX");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLSTR("INT64_MIN");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLSTR("0");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opSub(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
		}
		klDeref(x);
	}

	SECTION("multiplication")
	{
		auto x = KLINT(10);
		SECTION("With others ints")
		{
			SECTION("Positive")
			{
				auto y = KLINT(10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				REQUIRE(KASINT(res) == 100);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLINT(-10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				REQUIRE(KASINT(res) == -100);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLINT(INT64_MAX);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == -10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLINT(INT64_MIN);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLINT(0);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
		}
		SECTION("With floats")
		{
			SECTION("Positive")
			{
				auto y = KLFLOAT(10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 100);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLFLOAT(-10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == -100);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLFLOAT(INT64_MAX);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 92233720368547758080.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLFLOAT(INT64_MIN);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == -92233720368547758080.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLFLOAT(0);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
		}
		SECTION("With type not convertible to int")
		{
			SECTION("Positive")
			{
				auto y = KLSTR("10");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				REQUIRE(KASINT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLSTR("-10");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				REQUIRE(KASINT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLSTR("INT64_MAX");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLSTR("INT64_MIN");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLSTR("0");
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMul(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
		}
		klDeref(x);
	}

	SECTION("division")
	{
		auto x = KLINT(10);
		SECTION("With others ints")
		{
			SECTION("Positive")
			{
				auto y = KLINT(10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res);
				REQUIRE(KASINT(res) == 1);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLINT(-10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res);
				REQUIRE(KASINT(res) == -1);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLINT(INT64_MAX);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLINT(INT64_MIN);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLINT(0);
				KLObject* res = nullptr;
				REQUIRE_THROWS(klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res));
				klDeref(y);
			}
		}
		SECTION("With floats")
		{
			SECTION("Positive")
			{
				auto y = KLFLOAT(10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 1);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLFLOAT(-10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == -1);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLFLOAT(INT64_MAX);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) > 0);
				REQUIRE(KASFLOAT(res) < 0.000000001);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLFLOAT(INT64_MIN);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) < 0);
				REQUIRE(KASFLOAT(res) > -0.000000001);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLFLOAT(0);
				KLObject* res = nullptr;
				REQUIRE_THROWS(klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res));
				klDeref(y);
			}
		}
		SECTION("With type not convertible to int")
		{
			SECTION("Positive")
			{
				auto y = KLSTR("10");
				KLObject* res = nullptr;
				REQUIRE_THROWS(klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res));
				klDeref(y);
			}
			SECTION("Negative")
			{
				auto y = KLSTR("-10");
				KLObject* res = nullptr;
				REQUIRE_THROWS(klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res));
				klDeref(y);
			}
			SECTION("Max value")
			{
				auto y = KLSTR("INT64_MAX");
				KLObject* res = nullptr;
				REQUIRE_THROWS(klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res));
				klDeref(y);
			}
			SECTION("Min Value")
			{
				auto y = KLSTR("INT64_MIN");
				KLObject* res = nullptr;
				REQUIRE_THROWS(klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res));
				klDeref(y);
			}
			SECTION("Zero")
			{
				auto y = KLSTR("0");
				KLObject* res = nullptr;
				REQUIRE_THROWS(klTransfer(klint_t->KLNumericFunctions.opDiv(x, y), &res));
				klDeref(y);
			}
		}
		klDeref(x);
	}

	SECTION("modulo")
	{
		auto x = KLINT(10);
		SECTION("With others ints")
		{
			SECTION("Positive")
			{
				auto y = KLINT(10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res);
				REQUIRE(KASINT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLINT(-15);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res);
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLINT(INT64_MAX);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLINT(INT64_MIN);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res);
				// overflow
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLINT(0);
				KLObject* res = nullptr;
				REQUIRE_THROWS(klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res));
				klDeref(y);
			}
		}
		SECTION("With floats")
		{
			SECTION("Positive")
			{
				auto y = KLFLOAT(10);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLFLOAT(-15);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLFLOAT(INT64_MAX);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLFLOAT(INT64_MIN);
				KLObject* res = nullptr;
				klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res);
				REQUIRE(res->type == klfloat_t);
				REQUIRE(KASFLOAT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLFLOAT(0);
				KLObject* res = nullptr;
				REQUIRE_THROWS(klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res));
				klDeref(y);
			}
		}
		SECTION("With type not convertible to int")
		{
			SECTION("Positive")
			{
				auto y = KLSTR("10");
				KLObject* res = nullptr;
				REQUIRE_THROWS(klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res));
				klDeref(y);
			}
			SECTION("Negative")
			{
				auto y = KLSTR("-10");
				KLObject* res = nullptr;
				REQUIRE_THROWS(klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res));
				klDeref(y);
			}
			SECTION("Max value")
			{
				auto y = KLSTR("INT64_MAX");
				KLObject* res = nullptr;
				REQUIRE_THROWS(klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res));
				klDeref(y);
			}
			SECTION("Min Value")
			{
				auto y = KLSTR("INT64_MIN");
				KLObject* res = nullptr;
				REQUIRE_THROWS(klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res));
				klDeref(y);
			}
			SECTION("Zero")
			{
				auto y = KLSTR("0");
				KLObject* res = nullptr;
				REQUIRE_THROWS(klTransfer(klint_t->KLNumericFunctions.opMod(x, y), &res));
				klDeref(y);
			}
		}
		klDeref(x);
	}

	klEnd();
}
#pragma clang diagnostic pop