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

		AND_GIVEN("the base panel's dimensions are changed") {
			auto newDimensions = UI::Dimensions{ 0.5f, 5.0f };
			p.SetDimensions(newDimensions);

			THEN("the panel's absolute center has been updated") {
				REQUIRE(p.GetAbsoluteCenter().x == Approx(newDimensions.width * 0.5f));
				REQUIRE(p.GetAbsoluteCenter().y == Approx(newDimensions.height * 0.5f));
			}
		}
	}
}

SCENARIO("UI Structure for Panel") {
	GIVEN("a BasePanel with different width and height to match a screen ratio") {
		UI::Dimensions baseDimensions;

		// Approximate wide screen
		baseDimensions.width = 1.2f;
		baseDimensions.height = 1.f;

		UI::BasePanel basePanel(baseDimensions);

		AND_GIVEN("a single Panel attached from center to center") {
			UI::Dimensions d;

			d.width = 0.1f;
			d.height = 0.1f;

			UI::Attachment a;

			a.parent = UI::AP_CENTER;
			a.child = UI::AP_CENTER;

			UI::Panel p(&basePanel, d, a);

			THEN("the panel's absolute center should be the same as the base panel's") {
				REQUIRE(basePanel.GetAbsoluteCenter() == p.GetAbsoluteCenter());
			}

			THEN("the panel's dimensions should be unchanged") {
				REQUIRE(d.width == p.GetDimensions().width);
				REQUIRE(d.height == p.GetDimensions().height);
			}

			AND_GIVEN("the base panel's dimensions are changed") {
				auto newDimensions = UI::Dimensions{ 0.5f, 5.f };
				basePanel.SetDimensions(newDimensions);

				THEN("the panel's absolute center has been updated") {
					REQUIRE(p.GetAbsoluteCenter().x == basePanel.GetAbsoluteCenter().x);
					REQUIRE(p.GetAbsoluteCenter().y == basePanel.GetAbsoluteCenter().y);
				}
			}
		}

		AND_GIVEN("a single Panel attached from the base's center to its own top") {
			UI::Dimensions d;

			d.width = 0.1f;
			d.height = 0.1f;

			UI::Attachment a;

			a.parent = UI::AP_CENTER;
			a.child = UI::AP_TOP;

			UI::Panel p(&basePanel, d, a);

			THEN("the panel's absolute center should be offset correctly") {
				REQUIRE(p.GetAbsoluteCenter().x == basePanel.GetAbsoluteCenter().x);
				REQUIRE(p.GetAbsoluteCenter().y == Approx(basePanel.GetAbsoluteCenter().y - 0.5f*d.height));
			}
		}
	}
}
