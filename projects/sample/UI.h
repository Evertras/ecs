#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Assets.h"

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
		virtual void RenderSprite(glm::vec2 center, Dimensions d, const Assets::Texture& tex, glm::vec4 color) = 0;
		virtual void SetBaseSize(Dimensions size) = 0;
	};

	class Element {
	public:
		virtual ~Element() { }
		Element(const Element &rhs) = delete;

		void SetDimensions(Dimensions d) { m_Dimensions = d; UpdateAbsoluteCenter(); }
		void SetParentAnchor(AnchorPoint point) { m_Attachment.parent = point; UpdateAbsoluteCenter(); }
		void SetChildAnchor(AnchorPoint point) { m_Attachment.child = point; UpdateAbsoluteCenter(); }
		void SetAttachment(Attachment attachment) { m_Attachment = attachment; UpdateAbsoluteCenter(); }
		void SetRelativeCenter(const glm::vec2& center) { m_RelativeCenter = center; UpdateAbsoluteCenter(); }

		Element* AddChild(std::unique_ptr<Element> child) {
			child->m_Parent = this;
			child->UpdateAbsoluteCenter();

			Element* e = child.get();
			m_Children.push_back(std::move(child));
			return e;
		}

		const glm::vec2& GetAbsoluteCenter() const { return m_AbsoluteCenter; }
		const Dimensions& GetDimensions() const { return m_Dimensions; }

		void Draw(ElementRenderer* renderer) const {
			DrawReceive(renderer);

			for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter) {
				(*iter)->Draw(renderer);
			}
		}

	protected:
		Element(glm::vec2 center, Dimensions d, Attachment a)
			: m_Parent(nullptr), m_RelativeCenter(center), m_Dimensions(d), m_Attachment(a), m_AbsoluteCenter(center) {
		}

		virtual void DrawReceive(ElementRenderer* renderer) const = 0;

		void UpdateChildrenAbsoluteCenter() {
			for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter) {
				(*iter)->UpdateAbsoluteCenter();
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
		std::vector<std::unique_ptr<Element>> m_Children;
		glm::vec2 m_RelativeCenter;
		glm::vec2 m_AbsoluteCenter;
	};

	class BaseContainer : public Element {
	public:
		BaseContainer(Dimensions d) : Element({ d.width*0.5f, d.height*0.5f }, d, Attachment(AP_CENTER, AP_CENTER)) {}

	protected:
		void DrawReceive(ElementRenderer* renderer) const override {
			renderer->SetBaseSize(m_Dimensions);
		}
	};

	class Panel : public Element {
	public:
		Panel(glm::vec2 center, Dimensions d, Attachment a, glm::vec4 color = { 0.f, 0.f, 0.f, 0.f })
			: Element(center, d, a), m_Color(color) {}
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

	class Icon : public Element {
	public:
		Icon(glm::vec2 center, Dimensions d, Attachment a, Assets::Texture texture, glm::vec4 color = { 1.f, 1.f, 1.f, 1.f })
			: Element(center, d, a), m_Texture(texture), m_Color(color) {}
		~Icon() = default;
		Icon(const Icon& rhs) = default;

	protected:
		glm::vec4 m_Color;
		Assets::Texture m_Texture;
		void DrawReceive(ElementRenderer* renderer) const override {
			renderer->RenderSprite(m_AbsoluteCenter, m_Dimensions, m_Texture, m_Color);
		}
	};
}
