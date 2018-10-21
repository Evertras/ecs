#include <catch.hpp>

#include <UI.h>

SCENARIO("UI Structure for BasePanel") {
	GIVEN("a BasePanel with different width and height to match a screen ratio") {
		UI::Dimensions d;

		// Approximate wide screen
		d.width = 1.2f;
		d.height = 1.f;

		UI::BaseContainer p(d);

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

		UI::BaseContainer basePanel(baseDimensions);

		AND_GIVEN("a single Panel attached from center to center") {
			UI::Dimensions d;

			d.width = 0.1f;
			d.height = 0.1f;

			UI::Attachment a;

			a.parent = UI::AP_CENTER;
			a.child = UI::AP_CENTER;

			glm::vec2 center{ 0.f, 0.f };

			UI::Element *p = basePanel.AddChild(std::make_unique<UI::Panel>(center, d, a));

			THEN("the panel's absolute center should be the same as the base panel's") {
				REQUIRE(basePanel.GetAbsoluteCenter().x == p->GetAbsoluteCenter().x);
				REQUIRE(basePanel.GetAbsoluteCenter().y == p->GetAbsoluteCenter().y);
			}

			THEN("the panel's dimensions should be unchanged") {
				REQUIRE(d.width == p->GetDimensions().width);
				REQUIRE(d.height == p->GetDimensions().height);
			}

			AND_GIVEN("the base panel's dimensions are changed") {
				auto newDimensions = UI::Dimensions{ 0.5f, 5.f };
				basePanel.SetDimensions(newDimensions);

				THEN("the panel's absolute center has been updated") {
					REQUIRE(p->GetAbsoluteCenter().x == basePanel.GetAbsoluteCenter().x);
					REQUIRE(p->GetAbsoluteCenter().y == basePanel.GetAbsoluteCenter().y);
				}
			}

			AND_GIVEN("the panel's anchor point changes to match its center to the base panel's top") {
				UI::Attachment newAttachment;

				newAttachment.child = UI::AP_CENTER;
				newAttachment.parent = UI::AP_TOP;

				p->SetAttachment(newAttachment);

				THEN("the panel's absolute center should be at the top center of the base panel") {
					REQUIRE(p->GetAbsoluteCenter().x == basePanel.GetAbsoluteCenter().x);
					REQUIRE(p->GetAbsoluteCenter().y == Approx(0.f));
				}
			}

			AND_GIVEN("the panel's anchor point changes to match its right to the base panel's right") {
				UI::Attachment newAttachment;

				newAttachment.child = UI::AP_RIGHT;
				newAttachment.parent = UI::AP_RIGHT;

				p->SetAttachment(newAttachment);

				THEN("the panel's absolute center should be at the right of the screen, offset inwards") {
					REQUIRE(p->GetAbsoluteCenter().x == Approx(baseDimensions.width - d.width * 0.5f));
					REQUIRE(p->GetAbsoluteCenter().y == basePanel.GetAbsoluteCenter().y);
				}
			}

			AND_GIVEN("the panel's anchor point changes to match its left to the base panel's left") {
				UI::Attachment newAttachment;

				newAttachment.child = UI::AP_LEFT;
				newAttachment.parent = UI::AP_LEFT;

				p->SetAttachment(newAttachment);

				THEN("the panel's absolute center should be at the left of the screen, offset inwards") {
					REQUIRE(p->GetAbsoluteCenter().x == Approx(d.width * 0.5f));
					REQUIRE(p->GetAbsoluteCenter().y == basePanel.GetAbsoluteCenter().y);
				}
			}

			AND_GIVEN("the panel's anchor point changes to match its bottom to the base panel's bottom") {
				UI::Attachment newAttachment;

				newAttachment.child = UI::AP_BOTTOM;
				newAttachment.parent = UI::AP_BOTTOM;

				p->SetAttachment(newAttachment);

				THEN("the panel's absolute center should be at the bottom of the screen, offset inwards") {
					REQUIRE(p->GetAbsoluteCenter().x == basePanel.GetAbsoluteCenter().x);
					REQUIRE(p->GetAbsoluteCenter().y == Approx(baseDimensions.height - d.height * 0.5f));
				}
			}

			AND_GIVEN("the panel's anchor point changes to match its top to the base panel's top") {
				UI::Attachment newAttachment;

				newAttachment.child = UI::AP_TOP;
				newAttachment.parent = UI::AP_TOP;

				p->SetAttachment(newAttachment);

				THEN("the panel's absolute center should be at the top of the screen, offset inwards") {
					REQUIRE(p->GetAbsoluteCenter().x == basePanel.GetAbsoluteCenter().x);
					REQUIRE(p->GetAbsoluteCenter().y == Approx(d.height * 0.5f));
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

			UI::Element* p = basePanel.AddChild(std::make_unique<UI::Panel>(glm::vec2{ 0.f, 0.f }, d, a));

			THEN("the panel's absolute center should be offset correctly") {
				REQUIRE(p->GetAbsoluteCenter().x == basePanel.GetAbsoluteCenter().x);
				REQUIRE(p->GetAbsoluteCenter().y == Approx(basePanel.GetAbsoluteCenter().y + 0.5f*d.height));
			}
		}
	}
}

SCENARIO("Rendering UIs") {
	class MockRenderer : public UI::ElementRenderer {
	public:
		MockRenderer() : m_RenderRectCount(0), m_SetBaseSizeCount(0), m_RenderSpriteCount(0) {}

		void RenderRect(glm::vec2 center, UI::Dimensions dimensions, glm::vec4 color) override {
			++m_RenderRectCount;

			m_LastRectCenter = center;
			m_LastRectDimensions = dimensions;
			m_LastRectColor = color;
		}

		void RenderSprite(glm::vec2 center, UI::Dimensions dimensions, const Assets::Texture& tex, glm::vec4 color) override {
			++m_RenderSpriteCount;
		}

		void SetBaseSize(UI::Dimensions size) override {
			++m_SetBaseSizeCount;

			m_LastBaseSize = size;
		}

		int m_RenderRectCount;
		int m_RenderSpriteCount;
		int m_SetBaseSizeCount;
		glm::vec4 m_LastRectColor;
		glm::vec2 m_LastRectCenter;
		UI::Dimensions m_LastRectDimensions;
		UI::Dimensions m_LastBaseSize;
	};

	GIVEN("a UI with a base container and one child panel") {
		UI::Dimensions screenDimensions;

		screenDimensions.width = 1.2f;
		screenDimensions.height = 1.f;

		UI::BaseContainer root(screenDimensions);

		UI::Dimensions panelDimensions;
		UI::Attachment panelAttach(UI::AP_CENTER, UI::AP_CENTER);
		glm::vec2 panelCenter{ 0.1f, 0.1f };
		glm::vec4 panelColor{ 0.1f, 0.2f, 0.3f, 0.4f };

		panelDimensions.width = 0.1f;
		panelDimensions.height = 0.2f;

		UI::Element* panel = root.AddChild(std::make_unique<UI::Panel>(glm::vec2{ 0.f, 0.f }, panelDimensions, panelAttach, panelColor));

		AND_GIVEN("Draw is called on the root") {
			MockRenderer mock;

			root.Draw(&mock);

			THEN("the base size is set correctly") {
				REQUIRE(mock.m_SetBaseSizeCount == 1);
				REQUIRE(mock.m_LastBaseSize.width == screenDimensions.width);
				REQUIRE(mock.m_LastBaseSize.height == screenDimensions.height);
			}

			THEN("the panel is drawn with the expected parameters") {
				REQUIRE(mock.m_RenderRectCount == 1);

				auto expectedCenter = panel->GetAbsoluteCenter();
				REQUIRE(mock.m_LastRectCenter.x == expectedCenter.x);
				REQUIRE(mock.m_LastRectCenter.y == expectedCenter.y);

				REQUIRE(mock.m_LastRectColor.r == panelColor.r);
				REQUIRE(mock.m_LastRectColor.g == panelColor.g);
				REQUIRE(mock.m_LastRectColor.b == panelColor.b);
				REQUIRE(mock.m_LastRectColor.a == panelColor.a);

				REQUIRE(mock.m_LastRectDimensions.width == panelDimensions.width);
				REQUIRE(mock.m_LastRectDimensions.height == panelDimensions.height);
			}
		}
	}

	GIVEN("a UI with a base container and three child panels that have one child panel each") {
		UI::Dimensions screenDimensions;

		screenDimensions.width = 1.2f;
		screenDimensions.height = 1.f;

		UI::BaseContainer root(screenDimensions);

		UI::Dimensions panelDimensions;
		UI::Attachment panelAttach(UI::AP_CENTER, UI::AP_CENTER);
		glm::vec2 panelCenter{ 0.1f, 0.1f };
		glm::vec4 panelColor{ 0.1f, 0.2f, 0.3f, 0.4f };

		panelDimensions.width = 0.1f;
		panelDimensions.height = 0.2f;

		// Don't care that they're all on top of each other
		UI::Element* panel = root.AddChild(std::make_unique<UI::Panel>(glm::vec2{ 0.f, 0.f }, panelDimensions, panelAttach, panelColor));
		panel->AddChild(std::make_unique<UI::Panel>(glm::vec2{ 0.f, 0.f }, panelDimensions, panelAttach, panelColor));

		UI::Element* panel2 = root.AddChild(std::make_unique<UI::Panel>(glm::vec2{ 0.f, 0.f }, panelDimensions, panelAttach, panelColor));
		panel2->AddChild(std::make_unique<UI::Panel>(glm::vec2{ 0.f, 0.f }, panelDimensions, panelAttach, panelColor));

		UI::Element* panel3 = root.AddChild(std::make_unique<UI::Panel>(glm::vec2{ 0.f, 0.f }, panelDimensions, panelAttach, panelColor));
		panel3->AddChild(std::make_unique<UI::Panel>(glm::vec2{ 0.f, 0.f }, panelDimensions, panelAttach, panelColor));

		AND_GIVEN("Draw is called on the root") {
			MockRenderer mock;

			root.Draw(&mock);

			THEN("six rects are drawn") {
				REQUIRE(mock.m_RenderRectCount == 6);
			}
		}
	}
}
