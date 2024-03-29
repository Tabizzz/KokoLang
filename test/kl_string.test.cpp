#include "catch.hpp"
#include "klvm.h"
#include "testmacros.h"

TEST_CASE("kl_string", "[klapi_types][kl_string]")
{
	klInit();

	SECTION("compare")
	{
		KLObject* first = nullptr;
		KLObject* second = nullptr;

		SWAP(first, KLSTR(""), second, nullptr)
		REQUIRE(klstring_t->KLComparerFunctions.comparer(first, second) == 0);

		SWAP(first, KLSTR(""), second, KLSTR(""))
		REQUIRE(klstring_t->KLComparerFunctions.comparer(first, second) == 0);

		SWAP(first, KLSTR("a"), second, nullptr)
		REQUIRE(klstring_t->KLComparerFunctions.comparer(first, second) < 0);

		SWAP(first, KLSTR("a"), second, KLSTR(""))
		REQUIRE(klstring_t->KLComparerFunctions.comparer(first, second) < 0);

		SWAP(first, KLSTR("aaa"), second, KLSTR("aba"))
		REQUIRE(klstring_t->KLComparerFunctions.comparer(first, second) > 0);

		SWAP(first, KLSTR("aaaa"), second, KLSTR("aaaaa"))
		REQUIRE(klstring_t->KLComparerFunctions.comparer(first, second) > 0);

		SWAP(first, KLSTR("aba"), second, KLSTR("aaba"))
		REQUIRE(klstring_t->KLComparerFunctions.comparer(first, second) < 0);

		SWAP(first, KLSTR("bba"), second, KLSTR("bba"))
		REQUIRE(klstring_t->KLComparerFunctions.comparer(first, second) == 0);

		SWAP(first, nullptr, second, nullptr)
	}

	SECTION("equal")
	{
		KLObject* first = nullptr;
		KLObject* second = nullptr;

		SWAP(first, KLSTR(""), second, nullptr)
		REQUIRE(klstring_t->KLComparerFunctions.equal(first, second));

		SWAP(first, KLSTR(""), second, KLSTR(""))
		REQUIRE(klstring_t->KLComparerFunctions.equal(first, second));

		SWAP(first, KLSTR("bba"), second, KLSTR("bba"))
		REQUIRE(klstring_t->KLComparerFunctions.equal(first, second));

		SWAP(first, KLSTR(u8"μs"), second, KLSTR(u8"μs"))
		REQUIRE(klstring_t->KLComparerFunctions.equal(first, second));

		SWAP(first, KLSTR("bba"), second, KLSTR("bab"))
		REQUIRE_FALSE(klstring_t->KLComparerFunctions.equal(first, second));

		SWAP(first, KLSTR(""), second, KLSTR(" "))
		REQUIRE_FALSE(klstring_t->KLComparerFunctions.equal(first, second));

		SWAP(first, KLSTR("hello"), second, nullptr)
		REQUIRE_FALSE(klstring_t->KLComparerFunctions.equal(first, second));

		SWAP(first, nullptr, second, nullptr)
	}

	SECTION("add")
	{
		KLObject* first = nullptr;
		KLObject* second = nullptr;
		KLObject* add = nullptr;
		KLObject* cmp = nullptr;

		EXCHANGE(first, KLSTR("Hello"), second, KLSTR(""), cmp, KLSTR("Hello"))
		klTransfer(klstring_t->KLNumericFunctions.opAdd(first, second), &add);
		REQUIRE(klstring_t->KLComparerFunctions.equal(add, cmp));
		klDeref(add);
		add = nullptr;

		EXCHANGE(first, KLSTR("Hello"), second, nullptr, cmp, KLSTR("Hello"))
		klTransfer(klstring_t->KLNumericFunctions.opAdd(first, second), &add);
		REQUIRE(klstring_t->KLComparerFunctions.equal(add, cmp));
		klDeref(add);
		add = nullptr;

		EXCHANGE(first, KLSTR("Hello"), second, KLSTR(u8" μ"), cmp, KLSTR(u8"Hello μ"))
		klTransfer(klstring_t->KLNumericFunctions.opAdd(first, second), &add);
		REQUIRE(klstring_t->KLComparerFunctions.equal(add, cmp));
		klDeref(add);
		add = nullptr;

		EXCHANGE(first, KLSTR("Hello"), second, KLSTR(" world"), cmp, KLSTR("Hello world"))
		klTransfer(klstring_t->KLNumericFunctions.opAdd(first, second), &add);
		REQUIRE(klstring_t->KLComparerFunctions.equal(add, cmp));
		klDeref(add);
		add = nullptr;

		EXCHANGE(first, nullptr, second, nullptr, cmp, nullptr)
	}

	klEnd();
}