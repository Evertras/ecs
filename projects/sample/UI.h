#pragma once

#include <vector>
#include <glm/vec2.hpp>

namespace UI {
	enum AnchorPoint {
		AP_CENTER,
		AP_TOP,
		AP_RIGHT,
		AP_LEFT,
		AP_BOTTOM
	};

	struct Attachment {
		Attachment() : parent(AP_CENTER), child(AP_CENTER) {}
		Attachment(AnchorPoint parent, AnchorPoint child) : parent(parent), child(child) {}

		AnchorPoint parent;
		AnchorPoint child;
	};

	struct Dimensions {
		float width;
		float height;
	};

	class Element {
	public:
		void SetDimensions(Dimensions d) { m_Dimensions = d; UpdateAbsoluteCenter(); }
		void SetParentAnchor(AnchorPoint point) { m_Attachment.parent = point; UpdateAbsoluteCenter(); }
		void SetChildAnchor(AnchorPoint point) { m_Attachment.child = point; UpdateAbsoluteCenter(); }
		void SetAttachment(Attachment attachment) { m_Attachment = attachment; UpdateAbsoluteCenter(); }
		void SetRelativeCenter(const glm::vec2& center) { m_RelativeCenter = center; UpdateAbsoluteCenter(); }

		const glm::vec2& GetAbsoluteCenter() const { return m_AbsoluteCenter; }
		const Dimensions& GetDimensions() const { return m_Dimensions; }

	protected:
		Element(const Element* parent, Dimensions d, Attachment a)
			: m_Parent(parent), m_Dimensions(d), m_Attachment(a) {
			UpdateAbsoluteCenter();
		}
		virtual ~Element() {}
		Element(const Element &rhs) = default;

		void UpdateAbsoluteCenter() {
			if (m_Parent == nullptr) {
				m_AbsoluteCenter = { m_Dimensions.width * 0.5f, m_Dimensions.height * 0.5f };
				return;
			}

			m_AbsoluteCenter = m_Parent->m_AbsoluteCenter;

			switch (m_Attachment.parent) {
			case AP_TOP:
				m_AbsoluteCenter += glm::vec2{ 0.f, -0.5f * m_Parent->m_Dimensions.height };
				break;

			case AP_BOTTOM:
				m_AbsoluteCenter += glm::vec2{ 0.f, 0.5f * m_Parent->m_Dimensions.height };
				break;

			case AP_LEFT:
				m_AbsoluteCenter += glm::vec2{ -0.5f * m_Parent->m_Dimensions.width, 0.f };
				break;

			case AP_RIGHT:
				m_AbsoluteCenter += glm::vec2{ 0.5f * m_Parent->m_Dimensions.width, 0.f };
				break;
			}

			switch (m_Attachment.child) {
			case AP_TOP:
				m_AbsoluteCenter += glm::vec2{ 0.f, -0.5f * m_Dimensions.height };
				break;

			case AP_BOTTOM:
				m_AbsoluteCenter += glm::vec2{ 0.f, 0.5f * m_Dimensions.height };
				break;

			case AP_RIGHT:
				m_AbsoluteCenter += glm::vec2{ 0.5f * m_Dimensions.width, 0.f };
				break;

			case AP_LEFT:
				m_AbsoluteCenter += glm::vec2{ -0.5f * m_Dimensions.width, 0.f };
				break;
			}
		}

		Dimensions m_Dimensions;
		Attachment m_Attachment;
		const Element* m_Parent;
		std::vector<Element*> m_Children;
		glm::vec2 m_RelativeCenter;
		glm::vec2 m_AbsoluteCenter;
	};

	class BasePanel : public Element {
	public:
		BasePanel(Dimensions d) : Element(nullptr, d, Attachment(AP_CENTER, AP_CENTER)) {}
	};

	class Panel : public Element {
	public:
		Panel(const Element* parent, Dimensions d, Attachment a) : Element(parent, d, a) {}
		~Panel() = default;
		Panel(const Panel &rhs) = default;
	};
}
