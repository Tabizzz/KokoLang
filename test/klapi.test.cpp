#include <cstring>
#include "catch.hpp"
#include "klvm.h"
#include "testmacros.h"

KLType testT =
{
		KlObject(),
		"test",
		0,
		sizeof (kl_int)
};

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

		SECTION("Global package is named 'global'") {
			auto package = klGlobalPackage();
			REQUIRE(strncmp(KASSTR(package->name), "global", KASSTRSIZE(package->name)) == 0);
		}

		SECTION("There must be only one package")
		{
			COUNT_ARRAY(count, klRootPackages())
			REQUIRE(count == 1);

		}

		SECTION("klBType_Type is it own type")
		{
			REQUIRE(klBType_Type.klbase.type == &klBType_Type);
		}
	}

	SECTION("klRegisterPackage")
	{
		SECTION("Cant add a package with a repeated name")
		{
			REQUIRE_THROWS(klRegisterPackage(klGlobalPackage()));
		}
		SECTION("Dont allow empty or null name")
		{
			auto package = klCreatePackage();
			// null name
			REQUIRE_THROWS(klRegisterPackage(package));
			package->name = KLSTR("");
			REQUIRE_THROWS(klRegisterPackage(package));
			klDestroyPackage(package);
		}
		SECTION("Register increase the root package count")
		{
			COUNT_ARRAY(preCount, klRootPackages())
			auto package = klCreatePackage();
			package->name = KLSTR("testPackage");
			klRegisterPackage(package);
			COUNT_ARRAY(postCount, klRootPackages())
			REQUIRE(postCount == preCount + 1);
			// package will be destroyed on end
		}
	}

	SECTION("klDefType")
	{
		SECTION("Correctly set the type and inscount")
		{
			CHECK(testT.klbase.type != &klBType_Type);
			klDefType(&testT);
			REQUIRE(testT.inscount == 0);
			REQUIRE(testT.klbase.type == &klBType_Type);
		}
		SECTION("Dont allow names with invalid characters")
		{
			testT.name = "test.type";
			REQUIRE_THROWS(klDefType(&testT));
			testT.name = "test:type";
			REQUIRE_THROWS(klDefType(&testT));
			testT.name = "test%type";
			REQUIRE_THROWS(klDefType(&testT));
		}
		SECTION("Dont allow null or empty names")
		{
			testT.name = "";
			REQUIRE_THROWS(klDefType(&testT));
			testT.name = nullptr;
			REQUIRE_THROWS(klDefType(&testT));
		}
	}

	SECTION("klCopy")
	{
		SECTION("Copy throws on null target pointer")
		{
			REQUIRE_THROWS(klCopy(nullptr, nullptr));
		}
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
		GIVEN("A not null src without copy and with clone")
		{
			auto src = KLSTR("value");
			WHEN( "dest is null" )
			{
				KlObject* dest = nullptr;

				REQUIRE(src->refs == 1);

				klCopy(src, &dest);

				// is a clone, object mus be different references
				REQUIRE(src != dest);
				REQUIRE(src->refs == 1);
				REQUIRE(dest->refs == 1);

				// the size of the copy must be the same
				REQUIRE(STR_EQUALS(src, dest));

				klDeref(dest);
			}
			WHEN( "dest is not null and the same type of src" )
			{
				KlObject* dest = KLSTR("test");
				auto oldDest = dest;
				klRef(oldDest);
				REQUIRE(src->refs == 1);

				CHECK(STR_NOT_EQUALS(src, dest));

				klCopy(src, &dest);

				// is a clone, object mus be different references
				REQUIRE(src != dest);
				REQUIRE(dest != oldDest);
				REQUIRE(src->refs == 1);
				REQUIRE(dest->refs == 1);
				REQUIRE(oldDest->refs == 1);

				// the size of the copy must be the same
				REQUIRE(STR_EQUALS(src, dest));
				REQUIRE(STR_NOT_EQUALS(oldDest, dest));

				klDeref(dest);
				klDeref(oldDest);
			}
			WHEN( "dest is not null and not the same type of src" )
			{
				KlObject* dest = KLINT(0);
				auto oldDest = dest;
				klRef(oldDest);
				REQUIRE(src->refs == 1);

				klCopy(src, &dest);

				// is a clone, object mus be different references
				REQUIRE(src != dest);
				REQUIRE(dest != oldDest);
				REQUIRE(src->refs == 1);
				REQUIRE(dest->refs == 1);
				REQUIRE(oldDest->refs == 1);

				// the size of the copy must be the same
				REQUIRE(STR_EQUALS(src, dest));

				klDeref(dest);
				klDeref(oldDest);
			}
			klDeref(src);
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
		SECTION("clone throws on null target pointer")
		{
			REQUIRE_THROWS(klClone(nullptr, nullptr));
		}
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
		SECTION("move throws on null target pointer")
		{
			REQUIRE_THROWS(klMove(nullptr, nullptr));
		}
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