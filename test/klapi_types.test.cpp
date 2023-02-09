#include "catch.hpp"
#include "KokoLang.h"
#include "testmacros.h"

TEST_CASE("klapi_types.h","[klapi_types]")
{
	klInit();

	SECTION("klIns")
	{
		SECTION("Throws on not instantiable types")
		{
			REQUIRE_THROWS(klIns(&klBType_Bool));
		}
		SECTION("Check type"){
			auto val = klIns(&klBType_Float);

			REQUIRE(val->type == &klBType_Float);
			REQUIRE(val->refs == 1);
			REQUIRE(klBType_Float.inscount == 1);
			klDeref(val);
		}
	}
	SECTION("klRef")
	{
		auto val = KLINT(1);

		SECTION("Dont increase on static types")
		{
			auto bol = KLBOOL(true);
			auto old = bol->refs;
			klRef(bol);
			REQUIRE(old == bol->refs);
		}
		SECTION("Increase refcount on normal types")
		{
			auto old = val->refs;
			klRef(val);
			REQUIRE(old + 1 == val->refs);
			klDeref(val);
		}
		SECTION("Dont do nothing on null")
		{
			REQUIRE_NOTHROW(klRef(nullptr));
		}

		klDeref(val);
	}

	SECTION("klDeref")
	{
		auto val = KLINT(1);

		SECTION("Dont decrease on static types")
		{
			auto bol = KLBOOL(true);
			auto old = bol->refs;
			klDeref(bol);
			REQUIRE(old == bol->refs);
		}
		SECTION("Decrease refcount on normal types")
		{
			klRef(val);

			auto old = val->refs;
			klDeref(val);
			REQUIRE(old - 1 == val->refs);
		}
		SECTION("Destroy object")
		{
			auto destory = KLINT(0);
			klDeref(destory);
			REQUIRE_FALSE(is_valid(destory));
		}
		SECTION("Dont do nothing on null")
		{
			REQUIRE_NOTHROW(klDeref(nullptr));
		}

		klDeref(val);
	}

	// test for klDestroy is in klDeref, section "Destroy object"

	klEnd();
}

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
				REQUIRE(klBType_Int.comparer(x,y) == 0);
				klDeref(y);
			}
			SECTION("Lesser value")
			{
				auto y = KLINT(0);
				REQUIRE(klBType_Int.comparer(x, y) < 0);
				klDeref(y);
			}
			SECTION("Greater value")
			{
				auto y = KLINT(500);
				REQUIRE(klBType_Int.comparer(x, y) > 0);
				klDeref(y);
			}
		}
		SECTION("With types convertible to ints")
		{
			SECTION("Same value")
			{
				auto y = KLFLOAT(10);
				REQUIRE(klBType_Int.comparer(x,y) == 0);
				klDeref(y);
			}
			SECTION("Lesser value")
			{
				auto y = KLFLOAT(0);
				REQUIRE(klBType_Int.comparer(x, y) < 0);
				klDeref(y);
			}
			SECTION("Greater value")
			{
				auto y = KLFLOAT(500);
				REQUIRE(klBType_Int.comparer(x, y) > 0);
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
					REQUIRE(klBType_Int.comparer(y, val) == 0);
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLINT(-12);
					REQUIRE(klBType_Int.comparer(y, val) > 0);
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLINT(16);
					REQUIRE(klBType_Int.comparer(y, val) < 0);
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
					REQUIRE(klBType_Int.comparer(y, val) == 0);
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLINT(-12);
					REQUIRE(klBType_Int.comparer(y, val) > 0);
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLINT(16);
					REQUIRE(klBType_Int.comparer(y, val) < 0);
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
					REQUIRE(klBType_Int.comparer(y, val) == 0);
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLINT(-12);
					REQUIRE(klBType_Int.comparer(y, val) > 0);
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLINT(16);
					REQUIRE(klBType_Int.comparer(y, val) < 0);
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
				REQUIRE(klBType_Int.equal(x,y));
				klDeref(y);
			}
			SECTION("Lesser value")
			{
				auto y = KLINT(0);
				REQUIRE_FALSE(klBType_Int.equal(x, y));
				klDeref(y);
			}
			SECTION("Greater value")
			{
				auto y = KLINT(500);
				REQUIRE_FALSE(klBType_Int.equal(x, y));
				klDeref(y);
			}
		}
		SECTION("With types convertible to ints")
		{
			SECTION("Same value")
			{
				auto y = KLFLOAT(10);
				REQUIRE(klBType_Int.equal(x,y));
				klDeref(y);
			}
			SECTION("Lesser value")
			{
				auto y = KLFLOAT(0);
				REQUIRE_FALSE(klBType_Int.equal(x, y));
				klDeref(y);
			}
			SECTION("Greater value")
			{
				auto y = KLFLOAT(500);
				REQUIRE_FALSE(klBType_Int.equal(x, y));
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
					REQUIRE(klBType_Int.equal(y, val));
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLINT(-12);
					REQUIRE_FALSE(klBType_Int.equal(y, val));
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLINT(16);
					REQUIRE_FALSE(klBType_Int.equal(y, val));
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
					REQUIRE(klBType_Int.equal(y, val));
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLINT(-12);
					REQUIRE_FALSE(klBType_Int.equal(y, val));
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLINT(16);
					REQUIRE_FALSE(klBType_Int.equal(y, val));
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
					REQUIRE(klBType_Int.equal(y, val));
					klDeref(y);
				}
				SECTION("Lesser value")
				{
					auto y = KLINT(-12);
					REQUIRE_FALSE(klBType_Int.equal(y, val));
					klDeref(y);
				}
				SECTION("Greater value")
				{
					auto y = KLINT(16);
					REQUIRE_FALSE(klBType_Int.equal(y, val));
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

		EXCHANGE(value, KLINT(10), str, KLSTR("10"), convert, klBType_Int.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLINT(10000000), str, KLSTR("10000000"), convert, klBType_Int.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLINT(0), str, KLSTR("0"), convert, klBType_Int.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLINT(-100), str, KLSTR("-100"), convert, klBType_Int.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLINT(-0), str, KLSTR("0"), convert, klBType_Int.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		EXCHANGE(value, KLINT(INT64_MAX), str, KLSTR("9223372036854775807"), convert, klBType_Int.toString(value))
		REQUIRE(STR_EQUALS(str, convert));

		klDeref(value);
		klDeref(str);
		klDeref(convert);
	}

	SECTION("to_flt")
	{
		KlObject* value = nullptr;
		KlObject* str = nullptr;
		KlObject* convert = nullptr;

		EXCHANGE(value, KLINT(10), str, KLFLOAT(10), convert, klBType_Int.toFloat(value))
		REQUIRE(KASFLOAT(convert) == KASFLOAT(str));

		EXCHANGE(value, KLINT(10000000), str, KLFLOAT(10000000), convert, klBType_Int.toFloat(value))
		REQUIRE(KASFLOAT(convert) == KASFLOAT(str));

		EXCHANGE(value, KLINT(0), str, KLFLOAT(0), convert, klBType_Int.toFloat(value))
		REQUIRE(KASFLOAT(convert) == KASFLOAT(str));

		EXCHANGE(value, KLINT(-100), str, KLFLOAT(-100), convert, klBType_Int.toFloat(value))
		REQUIRE(KASFLOAT(convert) == KASFLOAT(str));

		EXCHANGE(value, KLINT(-0), str, KLFLOAT(0), convert, klBType_Int.toFloat(value))
		REQUIRE(KASFLOAT(convert) == KASFLOAT(str));

		EXCHANGE(value, KLINT(INT64_MAX), str, KLFLOAT(INT64_MAX), convert, klBType_Int.toFloat(value))
		REQUIRE(KASFLOAT(convert) == KASFLOAT(str));

		klDeref(value);
		klDeref(str);
		klDeref(convert);
	}

	SECTION("to_bit")
	{
		KlObject* value = nullptr;
		KlObject* str = nullptr;
		KlObject* convert = nullptr;

		EXCHANGE(value, KLINT(10), str, nullptr, convert, klBType_Int.toBool(value))
		REQUIRE(KASBOOL(convert) == true);

		EXCHANGE(value, KLINT(0), str, nullptr, convert, klBType_Int.toBool(value))
		REQUIRE(KASBOOL(convert) == false);

		EXCHANGE(value, KLINT(-10), str, nullptr, convert, klBType_Int.toBool(value))
		REQUIRE(KASBOOL(convert) == true);

		EXCHANGE(value, KLINT(-0), str, nullptr, convert, klBType_Int.toBool(value))
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
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
				REQUIRE(KASINT(res) == 20);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Positive")
			{
				auto y = KLINT(-10);
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
				REQUIRE(KASINT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLINT(INT64_MAX);
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
				// overflow
				REQUIRE(KASINT(res) == -9223372036854775799);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLINT(INT64_MIN);
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
				// overflow
				REQUIRE(KASINT(res) == -9223372036854775798);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLINT(0);
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
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
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 20);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Positive")
			{
				auto y = KLFLOAT(-10);
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLFLOAT(INT64_MAX);
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == 9223372036854775808.0);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLFLOAT(INT64_MIN);
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
				REQUIRE(KASFLOAT(res) == -9223372036854775798);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLFLOAT(0);
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
				REQUIRE(res->type == &klBType_Float);
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
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Positive")
			{
				auto y = KLSTR("-10");
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Max value")
			{
				auto y = KLSTR("INT64_MAX");
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
				// overflow
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Min Value")
			{
				auto y = KLSTR("INT64_MIN");
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
				// overflow
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
			SECTION("Zero")
			{
				auto y = KLSTR("0");
				KlObject* res = nullptr;
				klBType_Int.opAdd(x, y, &res);
				// overflow
				REQUIRE(KASINT(res) == 10);
				klDeref(y);
				klDeref(res);
			}
		}
		klDeref(x);
	}

	klEnd();
}
#pragma clang diagnostic pop