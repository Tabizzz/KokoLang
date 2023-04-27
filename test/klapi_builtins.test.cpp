#include <cstring>
#include "catch.hpp"
#include "klvm.h"
#include "testmacros.h"

TEST_CASE("klapi_builtins.h", "[klapi_builtins]")
{
	klInit();

	SECTION("klBuiltinInt")
	{
		auto oldIns = klBType_Int.inscount;
		auto val = klBuiltinInt(10);

		REQUIRE(KASINT(val) == 10);
		REQUIRE(klBType_Int.inscount == oldIns + 1);
		klDeref(val);
	}

	SECTION("klBuiltinFloat")
	{
		auto oldIns = klBType_Float.inscount;
		auto val = klBuiltinFloat(10.23);

		REQUIRE(KASFLOAT(val) == 10.23);
		REQUIRE(klBType_Float.inscount == oldIns + 1);
		klDeref(val);
	}

	SECTION("klBuiltinBool")
	{
		SECTION("Type dont increase inscount and check value")
		{
			auto oldIns = klBType_Bool.inscount;
			auto val = klBuiltinBool(true);

			REQUIRE(KASBOOL(val) == true);
			REQUIRE(klBType_Bool.inscount == oldIns);
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
		auto oldIns = klBType_String.inscount;
		auto val = klBuiltinString("test");

		REQUIRE(strncmp("test", KASSTR(val), KASSTRSIZE(val)) == 0);

		REQUIRE(klBType_String.inscount == oldIns + 1);
		klDeref(val);
	}

	SECTION("klBuiltinString_c")
	{
		auto oldIns = klBType_String.inscount;
		auto val = klBuiltinString_c("test");

		REQUIRE(strncmp("test", KASSTR(val), KASSTRSIZE(val)) == 0);

		REQUIRE(klBType_String.inscount == oldIns + 1);
		klDeref(val);
	}

	klEnd();
}