#include "catch.hpp"
#include "klvm.h"
#include "testmacros.h"

using namespace Catch::literals;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-const-int-float-conversion"
TEST_CASE("kl_float", "[klapi_types][kl_float]")
{
	klInit();

	SECTION("comparer")
	{
		auto x = KLFLOAT(10.5);
		SECTION("With others floats")
		{
			SECTION("Same value")
			{
				auto y = KLFLOAT(10.5);
				REQUIRE(klBType_Float.comparer(x,y) == 0);
				klDeref(y);
			}
			SECTION("Lesser value")
			{
				auto y = KLFLOAT(0);
				REQUIRE(klBType_Float.comparer(x, y) < 0);
				klDeref(y);
			}
			SECTION("Greater value")
			{
				auto y = KLFLOAT(500);
				REQUIRE(klBType_Float.comparer(x, y) > 0);
				klDeref(y);
			}
		}
		SECTION("With ints")
		{
			SECTION("Same value")
			{
				KASFLOAT(x) = 10;
				auto y = KLINT(10);
				REQUIRE(klBType_Float.comparer(x,y) == 0);
				klDeref(y);
			}
			SECTION("Lesser value")
			{
				auto y = KLINT(0);
				REQUIRE(klBType_Float.comparer(x, y) < 0);
				klDeref(y);
			}
			SECTION("Greater value")
			{
				auto y = KLINT(500);
				REQUIRE(klBType_Float.comparer(x, y) > 0);
				klDeref(y);
			}
		}
		klDeref(x);
		SECTION("With types not convertible to floats")
		{
			// other types are always considered 0 if are not convertible to floats

			GIVEN("A string")
			{
				auto val = KLSTR("10");
				SECTION("Same value")
				{
					auto y = KLFLOAT(0);
					REQUIRE(klBType_Float.comparer(y, val) == 0);
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLFLOAT(-12);
					REQUIRE(klBType_Float.comparer(y, val) > 0);
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLFLOAT(16);
					REQUIRE(klBType_Float.comparer(y, val) < 0);
					klDeref(y);
				}
				klDeref(val);
			}
			GIVEN("A bool")
			{
				auto val = KLBOOL(true);
				SECTION("Same value")
				{
					auto y = KLFLOAT(0);
					REQUIRE(klBType_Float.comparer(y, val) == 0);
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLFLOAT(-12);
					REQUIRE(klBType_Float.comparer(y, val) > 0);
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLFLOAT(16);
					REQUIRE(klBType_Float.comparer(y, val) < 0);
					klDeref(y);
				}
				klDeref(val);
			}
			GIVEN("A null")
			{
				auto val = nullptr;
				SECTION("Same value")
				{
					auto y = KLFLOAT(0);
					REQUIRE(klBType_Float.comparer(y, val) == 0);
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLFLOAT(-12);
					REQUIRE(klBType_Float.comparer(y, val) > 0);
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLFLOAT(16);
					REQUIRE(klBType_Float.comparer(y, val) < 0);
					klDeref(y);
				}
			}
		}
	}

	SECTION("Equality")
	{
		auto x = KLFLOAT(10.5);
		SECTION("With others floats")
		{
			SECTION("Same value")
			{
				auto y = KLFLOAT(10.5);
				REQUIRE(klBType_Float.equal(x,y));
				klDeref(y);
			}
			SECTION("Lesser value")
			{
				auto y = KLFLOAT(0);
				REQUIRE_FALSE(klBType_Float.equal(x, y));
				klDeref(y);
			}
			SECTION("Greater value")
			{
				auto y = KLFLOAT(500);
				REQUIRE_FALSE(klBType_Float.equal(x, y));
				klDeref(y);
			}
		}
		SECTION("With ints")
		{
			SECTION("Same value")
			{
				KASFLOAT(x) = 10;
				auto y = KLINT(10);
				REQUIRE(klBType_Float.equal(x,y));
				klDeref(y);
			}
			SECTION("Lesser value")
			{
				auto y = KLINT(0);
				REQUIRE_FALSE(klBType_Float.equal(x, y));
				klDeref(y);
			}
			SECTION("Greater value")
			{
				auto y = KLINT(500);
				REQUIRE_FALSE(klBType_Float.equal(x, y));
				klDeref(y);
			}
		}
		klDeref(x);

		SECTION("With types not convertible to floats")
		{
			// other types are always considered 0 if are not convertible to floats

			GIVEN("A string")
			{
				auto val = KLSTR("10");
				SECTION("Same value")
				{
					auto y = KLFLOAT(0);
					REQUIRE(klBType_Float.equal(y, val));
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLFLOAT(-12);
					REQUIRE_FALSE(klBType_Float.equal(y, val));
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLFLOAT(16);
					REQUIRE_FALSE(klBType_Float.equal(y, val));
					klDeref(y);
				}
				klDeref(val);
			}
			GIVEN("A bool")
			{
				auto val = KLBOOL(true);
				SECTION("Same value")
				{
					auto y = KLFLOAT(0);
					REQUIRE(klBType_Float.equal(y, val));
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLFLOAT(-12);
					REQUIRE_FALSE(klBType_Float.equal(y, val));
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLFLOAT(16);
					REQUIRE_FALSE(klBType_Float.equal(y, val));
					klDeref(y);
				}
				klDeref(val);
			}
			GIVEN("A null")
			{
				auto val = nullptr;
				SECTION("Same value")
				{
					auto y = KLFLOAT(0);
					REQUIRE(klBType_Float.equal(y, val));
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLFLOAT(-12);
					REQUIRE_FALSE(klBType_Float.equal(y, val));
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLFLOAT(16);
					REQUIRE_FALSE(klBType_Float.equal(y, val));
					klDeref(y);
				}
			}
		}
	}

	SECTION("to_str")
	{
		KlObject* value = nullptr;
		KlObject* str = nullptr;
		KlObject* convert = nullptr;

		EXCHANGE(value, KLFLOAT(10), str, KLSTR("10"), convert, klBType_Float.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLFLOAT(10.25), str, KLSTR("10.25"), convert, klBType_Float.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLFLOAT(10000000), str, KLSTR("10000000"), convert, klBType_Float.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLFLOAT(0), str, KLSTR("0"), convert, klBType_Float.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLFLOAT(-100), str, KLSTR("-100"), convert, klBType_Float.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLFLOAT(-0), str, KLSTR("0"), convert, klBType_Float.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLFLOAT(INT64_MAX), str, KLSTR("9.223372036854776e+18"), convert, klBType_Float.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		klDeref(value);
		klDeref(str);
		klDeref(convert);
	}

	SECTION("to_int")
	{
		KlObject* value = nullptr;
		KlObject* str = nullptr;
		KlObject* convert = nullptr;

		EXCHANGE(value, KLINT(10), str, KLFLOAT(10), convert, klBType_Float.toInt(str))
		REQUIRE(KASINT(convert) == KASINT(value));

		EXCHANGE(value, KLINT(10000000), str, KLFLOAT(10000000), convert, klBType_Float.toInt(str))
		REQUIRE(KASINT(convert) == KASINT(value));

		EXCHANGE(value, KLINT(0), str, KLFLOAT(0), convert, klBType_Float.toInt(str))
		REQUIRE(KASINT(convert) == KASINT(value));

		EXCHANGE(value, KLINT(-100), str, KLFLOAT(-100), convert, klBType_Float.toInt(str))
		REQUIRE(KASINT(convert) == KASINT(value));

		EXCHANGE(value, KLINT(-0), str, KLFLOAT(0), convert, klBType_Float.toInt(str))
		REQUIRE(KASINT(convert) == KASINT(value));

		EXCHANGE(value, KLINT(10), str, KLFLOAT(10.3), convert, klBType_Float.toInt(str))
		REQUIRE(KASINT(convert) == KASINT(value));

		EXCHANGE(value, KLINT(10), str, KLFLOAT(10.8), convert, klBType_Float.toInt(str))
		REQUIRE(KASINT(convert) == KASINT(value));

		klDeref(value);
		klDeref(str);
		klDeref(convert);
	}

	SECTION("to_bit")
	{
		KlObject* value = nullptr;
		KlObject* str = nullptr;
		KlObject* convert = nullptr;

		EXCHANGE(value, KLFLOAT(10), str, nullptr, convert, klBType_Float.toBool(value))
		REQUIRE(KASBOOL(convert) == true);

		EXCHANGE(value, KLFLOAT(0), str, nullptr, convert, klBType_Float.toBool(value))
		REQUIRE(KASBOOL(convert) == false);

		EXCHANGE(value, KLFLOAT(-10.25), str, nullptr, convert, klBType_Float.toBool(value))
		REQUIRE(KASBOOL(convert) == true);

		EXCHANGE(value, KLFLOAT(-0), str, nullptr, convert, klBType_Float.toBool(value))
		REQUIRE(KASBOOL(convert) == false);

		klDeref(value);
		klDeref(str);
		klDeref(convert);
	}

	SECTION("add")
	{
		auto x = KLFLOAT(10.5);
		SECTION("With others floats")
		{
			SECTION("Positive")
			{
				auto y = KLFLOAT(10.2);
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				REQUIRE(KASFLOAT(res) == 20.7);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLFLOAT(-10.8);
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				REQUIRE(KASFLOAT(res) == -0.3_a);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Big value")
			{
				auto y = KLFLOAT(INT64_MAX);
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 9223372036854775808.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Small Value")
			{
				auto y = KLFLOAT(INT64_MIN);
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == -9223372036854775798);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Big value")
			{
				auto y = KLFLOAT(INT64_MAX);
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 9223372036854775808.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLFLOAT(0);
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
		}
		SECTION("With ints")
		{
			SECTION("Positive")
			{
				auto y = KLINT(10);
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 20.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLINT(-10);
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 0.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLINT(INT64_MAX);
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 9223372036854775808.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLINT(INT64_MIN);
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == -9223372036854775798);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLINT(0);
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
		}
		SECTION("With type not convertible to float")
		{
			SECTION("Positive")
			{
				auto y = KLSTR("10");
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLSTR("-10");
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLSTR("INT64_MAX");
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLSTR("INT64_MIN");
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLSTR("0");
				KlObject* res = nullptr;
				klBType_Float.opAdd(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
		}
		klDeref(x);
	}

	SECTION("subtract")
	{
		auto x = KLFLOAT(10.5);
		SECTION("With others floats")
		{
			SECTION("Positive")
			{
				auto y = KLFLOAT(10);
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				REQUIRE(KASFLOAT(res) == 0.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLFLOAT(-10.5);
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				REQUIRE(KASFLOAT(res) == 21);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLFLOAT(INT64_MAX);
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == -9223372036854775808.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLFLOAT(INT64_MIN);
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 9223372036854775808.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLFLOAT(0);
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
		}
		SECTION("With ints")
		{
			SECTION("Positive")
			{
				auto y = KLINT(10);
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 0.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLINT(-10);
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 20.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLINT(INT64_MAX);
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == -9223372036854775797);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLINT(INT64_MIN);
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 9223372036854775808.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLFLOAT(0);
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
		}
		SECTION("With type not convertible to float")
		{
			SECTION("Positive")
			{
				auto y = KLSTR("10");
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLSTR("-10");
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLSTR("INT64_MAX");
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLSTR("INT64_MIN");
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLSTR("0");
				KlObject* res = nullptr;
				klBType_Float.opSub(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
		}
		klDeref(x);
	}

	SECTION("multiplication")
	{
		auto x = KLFLOAT(10.5);
		SECTION("With others floats")
		{
			SECTION("Positive")
			{
				auto y = KLFLOAT(10.2);
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				REQUIRE(KASFLOAT(res) == 107.1);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLFLOAT(-10.8);
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				REQUIRE(KASFLOAT(res) == -113.4);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLFLOAT(INT64_MAX);
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 96845406386975145984.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLFLOAT(INT64_MIN);
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == -96845406386975145984.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLFLOAT(0);
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
		}
		SECTION("With ints")
		{
			SECTION("Positive")
			{
				auto y = KLINT(10);
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 105);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLINT(-10);
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == -105);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLINT(INT64_MAX);
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 96845406386975145984.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLINT(INT64_MIN);
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == -96845406386975145984.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLINT(0);
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
		}
		SECTION("With type not convertible to float")
		{
			SECTION("Positive")
			{
				auto y = KLSTR("10");
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				REQUIRE(KASFLOAT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLSTR("-10");
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				REQUIRE(KASFLOAT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLSTR("INT64_MAX");
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLSTR("INT64_MIN");
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLSTR("0");
				KlObject* res = nullptr;
				klBType_Float.opMul(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
		}
		klDeref(x);
	}

	SECTION("division")
	{
		auto x = KLFLOAT(10.5);
		SECTION("With others floats")
		{
			SECTION("Positive")
			{
				auto y = KLFLOAT(10.2);
				KlObject* res = nullptr;
				klBType_Float.opDiv(x, y, &res);
				REQUIRE(KASFLOAT(res) == 1.02941176471_a);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLFLOAT(-10.8);
				KlObject* res = nullptr;
				klBType_Float.opDiv(x, y, &res);
				REQUIRE(KASFLOAT(res) == -0.972222222222_a);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLFLOAT(INT64_MAX);
				KlObject* res = nullptr;
				klBType_Float.opDiv(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 1.13841228111e-18_a);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLFLOAT(INT64_MIN);
				KlObject* res = nullptr;
				klBType_Float.opDiv(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == -1.13841228111e-18_a);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLFLOAT(0);
				KlObject* res = nullptr;
				REQUIRE_THROWS(klBType_Float.opDiv(x, y, &res));
				klDeref(y);
			}
		}
		SECTION("With ints")
		{
			SECTION("Positive")
			{
				auto y = KLINT(10);
				KlObject* res = nullptr;
				klBType_Float.opDiv(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 1.05);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLINT(-10);
				KlObject* res = nullptr;
				klBType_Float.opDiv(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == -1.05);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLINT(INT64_MAX);
				KlObject* res = nullptr;
				klBType_Float.opDiv(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 1.13841228111e-18_a);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLINT(INT64_MIN);
				KlObject* res = nullptr;
				klBType_Float.opDiv(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == -1.13841228111e-18_a);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLINT(0);
				KlObject* res = nullptr;
				REQUIRE_THROWS(klBType_Float.opDiv(x, y, &res));
				klDeref(y);
			}
		}
		SECTION("With type not convertible to float")
		{
			SECTION("Positive")
			{
				auto y = KLSTR("10");
				KlObject* res = nullptr;
				REQUIRE_THROWS(klBType_Float.opDiv(x, y, &res));
				klDeref(y);
			}
			SECTION("Negative")
			{
				auto y = KLSTR("-10");
				KlObject* res = nullptr;
				REQUIRE_THROWS(klBType_Float.opDiv(x, y, &res));
				klDeref(y);
			}
			SECTION("Max value")
			{
				auto y = KLSTR("INT64_MAX");
				KlObject* res = nullptr;
				REQUIRE_THROWS(klBType_Float.opDiv(x, y, &res));
				klDeref(y);
			}
			SECTION("Min Value")
			{
				auto y = KLSTR("INT64_MIN");
				KlObject* res = nullptr;
				REQUIRE_THROWS(klBType_Float.opDiv(x, y, &res));
				klDeref(y);
			}
			SECTION("Zero")
			{
				auto y = KLSTR("0");
				KlObject* res = nullptr;
				REQUIRE_THROWS(klBType_Float.opDiv(x, y, &res));
				klDeref(y);
			}
		}
		klDeref(x);
	}

	SECTION("modulo")
	{
		auto x = KLFLOAT(10.5);
		SECTION("With others floats")
		{
			SECTION("Positive")
			{
				auto y = KLFLOAT(10.2);
				KlObject* res = nullptr;
				klBType_Float.opMod(x, y, &res);
				REQUIRE(KASFLOAT(res) == 0.3_a);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLFLOAT(-10.8);
				KlObject* res = nullptr;
				klBType_Float.opMod(x, y, &res);
				REQUIRE(KASFLOAT(res) == 10.5_a);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLFLOAT(INT64_MAX);
				KlObject* res = nullptr;
				klBType_Float.opMod(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLFLOAT(INT64_MIN);
				KlObject* res = nullptr;
				klBType_Float.opMod(x, y, &res);
				// overflow
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLFLOAT(0);
				KlObject* res = nullptr;
				REQUIRE_THROWS(klBType_Float.opMod(x, y, &res));
				klDeref(y);
			}
		}
		SECTION("With ints")
		{
			SECTION("Positive")
			{
				auto y = KLINT(10);
				KlObject* res = nullptr;
				klBType_Float.opMod(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 0.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Negative")
			{
				auto y = KLINT(-15);
				KlObject* res = nullptr;
				klBType_Float.opMod(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLINT(INT64_MAX);
				KlObject* res = nullptr;
				klBType_Float.opMod(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLINT(INT64_MIN);
				KlObject* res = nullptr;
				klBType_Float.opMod(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 10.5);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLFLOAT(0);
				KlObject* res = nullptr;
				REQUIRE_THROWS(klBType_Float.opMod(x, y, &res));
				klDeref(y);
			}
		}
		SECTION("With type not convertible to float")
		{
			SECTION("Positive")
			{
				auto y = KLSTR("10");
				KlObject* res = nullptr;
				REQUIRE_THROWS(klBType_Float.opMod(x, y, &res));
				klDeref(y);
			}
			SECTION("Negative")
			{
				auto y = KLSTR("-10");
				KlObject* res = nullptr;
				REQUIRE_THROWS(klBType_Float.opMod(x, y, &res));
				klDeref(y);
			}
			SECTION("Max value")
			{
				auto y = KLSTR("INT64_MAX");
				KlObject* res = nullptr;
				REQUIRE_THROWS(klBType_Float.opMod(x, y, &res));
				klDeref(y);
			}
			SECTION("Min Value")
			{
				auto y = KLSTR("INT64_MIN");
				KlObject* res = nullptr;
				REQUIRE_THROWS(klBType_Float.opMod(x, y, &res));
				klDeref(y);
			}
			SECTION("Zero")
			{
				auto y = KLSTR("0");
				KlObject* res = nullptr;
				REQUIRE_THROWS(klBType_Float.opMod(x, y, &res));
				klDeref(y);
			}
		}
		klDeref(x);
	}

	klEnd();
}
#pragma clang diagnostic pop