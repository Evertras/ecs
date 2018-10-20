#include <catch.hpp>

#include <UI.h>

SCENARIO("UI Structure for BasePanel") {
	GIVEN("a BasePanel with different width and height to match a screen ratio") {
		UI::Dimensions d;

		// Approximate wide screen
		d.width = 1.2f;
		d.height = 1.f;

		UI::BasePanel p(d);

		THEN("the absolute center is calculated at the center of the screen") {
			auto center = p.GetAbsoluteCenter();

			REQUIRE(center.x == Approx(d.width * 0.5f));
			REQUIRE(center.y == Approx(d.height * 0.5f));
		}
	}
}
