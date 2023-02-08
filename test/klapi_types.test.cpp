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

		klDeref(val);
	}

	klEnd();
}