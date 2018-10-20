#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

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

	class ElementRenderer {
	public:
		virtual void RenderRect(glm::vec2 center, Dimensions d, glm::vec4 color) = 0;
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

		const Element* GetParent() const { return m_Parent; }
		const std::vector<Element*>& GetChildren() const { return m_Children; }

		void Draw(ElementRenderer* renderer) const {
			DrawReceive(renderer);

			for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter) {
				(*iter)->Draw(renderer);
			}
		}

	protected:
		Element(Element* parent, glm::vec2 center, Dimensions d, Attachment a)
			: m_Parent(parent), m_RelativeCenter(center), m_Dimensions(d), m_Attachment(a) {
			UpdateAbsoluteCenter();

			if (m_Parent != nullptr) {
				m_Parent->m_Children.push_back(this);
			}
		}

		virtual ~Element() {
			if (m_Parent != nullptr) {
				for (auto iter = m_Parent->m_Children.begin(); iter != m_Parent->m_Children.end(); ++iter) {
					if (*iter == this) {
						m_Parent->m_Children.erase(iter);
						break;
					}
				}
			}
		}

		Element(const Element &rhs) = default;

		virtual void DrawReceive(ElementRenderer* renderer) const = 0;

		void UpdateChildrenAbsoluteCenter() {
			for (auto e : m_Children) {
				e->UpdateAbsoluteCenter();
			}
		}

		void UpdateAbsoluteCenter() {
			if (m_Parent == nullptr) {
				m_AbsoluteCenter = { m_Dimensions.width * 0.5f, m_Dimensions.height * 0.5f };
				UpdateChildrenAbsoluteCenter();
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
				m_AbsoluteCenter += glm::vec2{ 0.f, 0.5f * m_Dimensions.height };
				break;

			case AP_BOTTOM:
				m_AbsoluteCenter += glm::vec2{ 0.f, -0.5f * m_Dimensions.height };
				break;

			case AP_RIGHT:
				m_AbsoluteCenter += glm::vec2{ -0.5f * m_Dimensions.width, 0.f };
				break;

			case AP_LEFT:
				m_AbsoluteCenter += glm::vec2{ 0.5f * m_Dimensions.width, 0.f };
				break;
			}

			UpdateChildrenAbsoluteCenter();
		}

		Dimensions m_Dimensions;
		Attachment m_Attachment;
		Element* m_Parent;
		std::vector<Element*> m_Children;
		glm::vec2 m_RelativeCenter;
		glm::vec2 m_AbsoluteCenter;
	};

	class BaseContainer : public Element {
	public:
		BaseContainer(Dimensions d) : Element(nullptr, { 0.f, 0.f }, d, Attachment(AP_CENTER, AP_CENTER)) {}

	protected:
		void DrawReceive(ElementRenderer* renderer) const override {}
	};

	class Panel : public Element {
	public:
		Panel(Element* parent, glm::vec2 center, Dimensions d, Attachment a, glm::vec4 color = { 0.f, 0.f, 0.f, 0.f })
			: Element(parent, center, d, a), m_Color(color) {}
		~Panel() = default;
		Panel(const Panel &rhs) = default;

		void SetColor(const glm::vec4& color) { m_Color = color; }
		const glm::vec4& GetColor() const { return m_Color; }

	protected:
		glm::vec4 m_Color;

		void DrawReceive(ElementRenderer* renderer) const override {
			renderer->RenderRect(m_AbsoluteCenter, m_Dimensions, m_Color);
		}
	};
}
