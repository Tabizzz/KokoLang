#include "catch.hpp"
#include "KokoLang.h"

TEST_CASE( "klapi.h", "[klapi]" ) {

	SECTION("pre klInit")
	{
		SECTION("Global package is not valid before init") {
			REQUIRE(!klGlobalPackage());
		}
	}

	klInit();

	SECTION("post klInit")
	{
		SECTION("Global package is valid after init") {
			REQUIRE(klGlobalPackage());
		}

		SECTION("klBType_Type is it own type")
		{
			REQUIRE(klBType_Type.klbase.type == &klBType_Type);
		}
	}

	SECTION("klCopy")
	{
		GIVEN("A not null src")
		{
			WHEN( "dest is null" )
			{


			}
			WHEN( "dest is not null" )
			{

			}
		}
		GIVEN("A null src")
		{
			WHEN( "dest is null" )
			{

			}
			WHEN( "dest is not null" )
			{

			}
		}
	}

	SECTION("kClone")
	{
		GIVEN("A not null src")
		{
			WHEN( "dest is null" )
			{


			}
			WHEN( "dest is not null" )
			{

			}
		}
		GIVEN("A null src")
		{
			WHEN( "dest is null" )
			{

			}
			WHEN( "dest is not null" )
			{

			}
		}
	}

	SECTION("klMove")
	{
		GIVEN("A not null src")
		{
			auto src = KLSTR("");
			WHEN( "dest is null" )
			{
				KlObject* dest = nullptr;
				CHECK(src->refs == 1);

				klMove(src, &dest);

				REQUIRE(src == dest);
				REQUIRE(src->refs == 2);
				klDeref(dest);
			}
			WHEN( "dest is not null" )
			{
				auto dest = KLSTR("");
				CHECK(src->refs == 1);
				CHECK(dest->refs == 1);

				auto oldDest = dest;
				klRef(oldDest);

				klMove(src, &dest);

				// the old value should be deref once
				REQUIRE(oldDest->refs == 1);
				klDeref(oldDest);

				REQUIRE(src == dest);
				REQUIRE(src->refs == 2);
				klDeref(dest);
			}
			klDeref(src);
		}
		GIVEN("A null src")
		{
			KlObject* src = nullptr;
			WHEN( "dest is null" )
			{
				KlObject* dest = nullptr;
				klMove(src, &dest);
				// in this case nothing occurs so both value must be still null.
				REQUIRE(src == nullptr);
				REQUIRE(dest == nullptr);
			}
			WHEN( "dest is not null" )
			{
				KlObject* dest = KLSTR("");
				auto oldDest = dest;
				CHECK(dest->refs == 1);
				klRef(dest);

				klMove(src, &dest);

				// dest is now null and the old value is deref once
				REQUIRE(dest == nullptr);
				REQUIRE(oldDest->refs == 1);

				klDeref(oldDest);
			}
		}
	}

	klEnd();
}