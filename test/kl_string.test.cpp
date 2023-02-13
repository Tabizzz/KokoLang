#include "catch.hpp"
#include "KokoLang.h"
#include "testmacros.h"

TEST_CASE("kl_string", "[klapi_types][kl_string]")
{
	klInit();

	SECTION("compare")
	{
		KlObject* first = nullptr;
		KlObject* second = nullptr;

		SWAP(first, KLSTR(""), second, nullptr)
		REQUIRE(klBType_String.comparer(first, second) == 0);

		SWAP(first, KLSTR(""), second, KLSTR(""))
		REQUIRE(klBType_String.comparer(first, second) == 0);

		SWAP(first, KLSTR("a"), second, nullptr)
		REQUIRE(klBType_String.comparer(first, second) < 0);

		SWAP(first, KLSTR("a"), second, KLSTR(""))
		REQUIRE(klBType_String.comparer(first, second) < 0);

		SWAP(first, KLSTR("aaa"), second, KLSTR("aba"))
		REQUIRE(klBType_String.comparer(first, second) > 0);

		SWAP(first, KLSTR("aaaa"), second, KLSTR("aaaaa"))
		REQUIRE(klBType_String.comparer(first, second) > 0);

		SWAP(first, KLSTR("aba"), second, KLSTR("aaba"))
		REQUIRE(klBType_String.comparer(first, second) < 0);

		SWAP(first, KLSTR("bba"), second, KLSTR("bba"))
		REQUIRE(klBType_String.comparer(first, second) == 0);

		SWAP(first, nullptr, second, nullptr)
	}

	SECTION("equal")
	{
		KlObject* first = nullptr;
		KlObject* second = nullptr;

		SWAP(first, KLSTR(""), second, nullptr)
		REQUIRE(klBType_String.equal(first, second));

		SWAP(first, KLSTR(""), second, KLSTR(""))
		REQUIRE(klBType_String.equal(first, second));

		SWAP(first, KLSTR("bba"), second, KLSTR("bba"))
		REQUIRE(klBType_String.equal(first, second));

		SWAP(first, KLSTR(u8"μs"), second, KLSTR(u8"μs"))
		REQUIRE(klBType_String.equal(first, second));

		SWAP(first, KLSTR("bba"), second, KLSTR("bab"))
		REQUIRE_FALSE(klBType_String.equal(first, second));

		SWAP(first, KLSTR(""), second, KLSTR(" "))
		REQUIRE_FALSE(klBType_String.equal(first, second));

		SWAP(first, KLSTR("hello"), second, nullptr)
		REQUIRE_FALSE(klBType_String.equal(first, second));

		SWAP(first, nullptr, second, nullptr)
	}

	SECTION("add")
	{
		KlObject* first = nullptr;
		KlObject* second = nullptr;
		KlObject* add = nullptr;
		KlObject* cmp = nullptr;

		EXCHANGE(first, KLSTR("Hello"), second, KLSTR(""), cmp, KLSTR("Hello"))
		klBType_String.opAdd(first, second, &add);
		REQUIRE(klBType_String.equal(add, cmp));
		klDeref(add);
		add = nullptr;

		EXCHANGE(first, KLSTR("Hello"), second, nullptr, cmp, KLSTR("Hello"))
		klBType_String.opAdd(first, second, &add);
		REQUIRE(klBType_String.equal(add, cmp));
		klDeref(add);
		add = nullptr;

		EXCHANGE(first, KLSTR("Hello"), second, KLSTR(u8" μ"), cmp, KLSTR("Hello μ"))
		klBType_String.opAdd(first, second, &add);
		REQUIRE(klBType_String.equal(add, cmp));
		klDeref(add);
		add = nullptr;

		EXCHANGE(first, KLSTR("Hello"), second, KLSTR(" world"), cmp, KLSTR("Hello world"))
		klBType_String.opAdd(first, second, &add);
		REQUIRE(klBType_String.equal(add, cmp));
		klDeref(add);
		add = nullptr;

		EXCHANGE(first, nullptr, second, nullptr, cmp, nullptr)
	}

	klEnd();
}