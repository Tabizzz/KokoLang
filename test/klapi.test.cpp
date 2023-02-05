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
		GIVEN("A not null src with copy and with clone")
		{
			auto src = KLINT(1);

			WHEN( "dest is null" )
			{
				// the object will be cloned
				KlObject* dest = nullptr;
				REQUIRE(src->refs == 1);

				klCopy(src, &dest);

				REQUIRE(src != dest);
				REQUIRE(src->refs == 1);
				REQUIRE(dest->refs == 1);

				REQUIRE(KASINT(src) == KASINT(dest));

				klDeref(dest);
			}
			WHEN( "dest is not null and the same type of src" )
			{
				KlObject* dest = KLINT(0);
				REQUIRE(src->refs == 1);
				REQUIRE(dest->refs == 1);
				auto oldDest = dest;

				REQUIRE(KASINT(src) != KASINT(dest));

				klCopy(src, &dest);

				REQUIRE(src != dest);
				REQUIRE(dest == oldDest);
				REQUIRE(src->refs == 1);
				REQUIRE(dest->refs == 1);

				REQUIRE(KASINT(src) == KASINT(dest));

				klDeref(dest);
			}
			WHEN( "dest is not null and not the same type of src" )
			{
				// dest can be any type, it must be overwritten anyway
				KlObject* dest = KLSTR("");
				auto oldDest = dest;
				klRef(oldDest);

				klCopy(src, &dest);

				// is a clone, object must be different references
				REQUIRE(src != dest);
				REQUIRE(src != oldDest);
				REQUIRE(dest != oldDest);
				REQUIRE(src->refs == 1);
				REQUIRE(dest->refs == 1);
				REQUIRE(oldDest->refs == 1);

				/*
				 * Here we are explicit working with int, so compare the values must be equals.
				 */
				REQUIRE(KASINT(src) == KASINT(dest));

				klDeref(dest);
				klDeref(oldDest);
			}
			klDeref(src);
		}
		GIVEN("A not null src with copy and without clone")
		{
			auto src = klIns(&klBType_Reg);

			WHEN( "dest is null" )
			{
				KlObject* dest = nullptr;
				CHECK(src->refs == 1);

				klClone(src, &dest);

				REQUIRE(src == dest);
				REQUIRE(src->refs == 2);
				klDeref(dest);
			}
			WHEN( "dest is not null and the same type of src" )
			{
				KlObject* dest = klIns(&klBType_Reg);
				KASINT(dest) = 1;
				REQUIRE(src->refs == 1);
				REQUIRE(dest->refs == 1);
				auto oldDest = dest;

				REQUIRE(KASINT(src) != KASINT(dest));

				klCopy(src, &dest);

				REQUIRE(src != dest);
				REQUIRE(dest == oldDest);
				REQUIRE(src->refs == 1);
				REQUIRE(dest->refs == 1);

				REQUIRE(KASINT(src) == KASINT(dest));

				klDeref(dest);
			}
			WHEN( "dest is not null and not the same type of src" )
			{
				auto dest = KLSTR("");
				CHECK(src->refs == 1);
				CHECK(dest->refs == 1);

				auto oldDest = dest;
				klRef(oldDest);

				klCopy(src, &dest);

				// the old value should be deref once
				REQUIRE(oldDest->refs == 1);
				klDeref(oldDest);

				REQUIRE(src == dest);
				REQUIRE(src->refs == 2);
				klDeref(dest);
			}
			klDeref(src);
		}
		GIVEN("A not null src without copy and with clone") // TODO: implement clone in strings and test
		{
			WHEN( "dest is null" )
			{

			}
			WHEN( "dest is not null and the same type of src" )
			{

			}
			WHEN( "dest is not null and not the same type of src" )
			{

			}
		}
		GIVEN("A null src")
		{
			KlObject* src = nullptr;
			WHEN( "dest is null" )
			{
				KlObject* dest = nullptr;
				klCopy(src, &dest);
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

				klCopy(src, &dest);

				// dest is now null and the old value is deref once
				REQUIRE(dest == nullptr);
				REQUIRE(oldDest->refs == 1);

				klDeref(oldDest);
			}
		}
	}

	SECTION("kClone")
	{
		GIVEN("A not null src with clone")
		{
			auto src = KLINT(0);

			WHEN( "dest is null" )
			{
				KlObject* dest = nullptr;

				klClone(src, &dest);

				// is a clone, object mus be different references
				REQUIRE(src != dest);
				REQUIRE(src->refs == 1);
				REQUIRE(dest->refs == 1);

				/*
				 * Here we are explicit working with int, so compare the values must be equals.
				 */
				REQUIRE(KASINT(src) == KASINT(dest));

				klDeref(dest);
			}
			WHEN( "dest is not null" )
			{
				// dest can be any type, it must be overwritten anyway
				KlObject* dest = KLSTR("");
				auto oldDest = dest;
				klRef(oldDest);

				klClone(src, &dest);

				// is a clone, object must be different references
				REQUIRE(src != dest);
				REQUIRE(src != oldDest);
				REQUIRE(dest != oldDest);
				REQUIRE(src->refs == 1);
				REQUIRE(dest->refs == 1);
				REQUIRE(oldDest->refs == 1);

				/*
				 * Here we are explicit working with int, so compare the values must be equals.
				 */
				REQUIRE(KASINT(src) == KASINT(dest));

				klDeref(dest);
				klDeref(oldDest);
			}
			klDeref(src);
		}
		GIVEN("A not null src without clone")
		{
			auto src = klIns(&klBType_Reg);

			WHEN( "dest is null" )
			{
				KlObject* dest = nullptr;
				CHECK(src->refs == 1);

				klClone(src, &dest);

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
				klClone(src, &dest);
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

				klClone(src, &dest);

				// dest is now null and the old value is deref once
				REQUIRE(dest == nullptr);
				REQUIRE(oldDest->refs == 1);

				klDeref(oldDest);
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