#include <cstring>
#include "catch.hpp"
#include "klvm.h"
#include "testmacros.h"

TEST_CASE("klapi_builtins.h", "[klapi_builtins]")
{
	klInit();

	SECTION("klBuiltinInt")
	{
		auto oldIns = klint_t->inscount;
		auto val = klBuiltinInt(10);

		REQUIRE(KASINT(val) == 10);
		REQUIRE(klint_t->inscount == oldIns + 1);
		klDeref(val);
	}

	SECTION("klBuiltinFloat")
	{
		auto oldIns = klfloat_t->inscount;
		auto val = klBuiltinFloat(10.23);

		REQUIRE(KASFLOAT(val) == 10.23);
		REQUIRE(klfloat_t->inscount == oldIns + 1);
		klDeref(val);
	}

	SECTION("klBuiltinBool")
	{
		SECTION("Type dont increase inscount and check value")
		{
			auto oldIns = klbool_t->inscount;
			auto val = klBuiltinBool(true);

			REQUIRE(KASBOOL(val) == true);
			REQUIRE(klbool_t->inscount == oldIns);
		}
		SECTION("All bools are the same instance")
		{
			auto truea = KLBOOL(true);
			auto trueb = KLBOOL(true);
			REQUIRE(truea == trueb);
			auto falsea = KLBOOL(false);
			auto falseb = KLBOOL(false);
			REQUIRE(falsea == falseb);

			REQUIRE(truea != falsea);

			// bools are not instanced so no need to deref
		}
	}

	SECTION("klBuiltinString")
	{
		auto oldIns = klstring_t->inscount;
		auto val = klBuiltinString("test");

		REQUIRE(strncmp("test", KASSTR(val), KASSTRSIZE(val)) == 0);

		REQUIRE(klstring_t->inscount == oldIns + 1);
		klDeref(val);
	}

	SECTION("klBuiltinString_c")
	{
		auto oldIns = klstring_t->inscount;
		auto val = klBuiltinString_c("test");

		REQUIRE(strncmp("test", KASSTR(val), KASSTRSIZE(val)) == 0);

		REQUIRE(klstring_t->inscount == oldIns + 1);
		klDeref(val);
	}

	klEnd();
}