#define CATCH_CONFIG_MAIN

#include <Catch.hpp>

TEST_CASE("Temp tests", "[ECSLib]")
{
	int twoPlusTwo = 2 + 2;

	REQUIRE(twoPlusTwo == 4);
}
