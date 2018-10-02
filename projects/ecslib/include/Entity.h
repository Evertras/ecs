#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "Component.h"

namespace ECS {
	class Entity
	{
	public:
		Entity() {}
		~Entity() {}
		Entity(const Entity &rhs) = delete;

		size_t NumComponents() const { return m_Components.size(); }

		template<typename T>
		void AddComponent(const T &data) {
			m_Components.insert_or_assign(typeid(T), std::make_unique<Component<T>>(data));
		}

		template<typename T>
		bool Has() const { return m_Components.find(typeid(T)) != m_Components.end(); }

		template<typename T, typename ...Components>
		typename std::enable_if<sizeof...(Components) != 0, bool>::type
		Has() const { return Has<T>() && Has<Components...>(); }

		template<typename T>
		T& Data() {
			return std::ref(dynamic_cast<Component<T>*>(m_Components[typeid(T)].get())->data);
		}

		template<typename T, typename ...Others> 
		typename std::enable_if<sizeof...(Others) != 0, std::tuple<T&, Others&...>>::type
		Data() {
			return std::tuple<T&, Others&...>(
				std::ref(dynamic_cast<Component<T>*>(m_Components[typeid(T)].get())->data),
				Data<Others...>()
				);
		}

	private:
		std::unordered_map<std::type_index, std::unique_ptr<Internal::BaseComponent>> m_Components;
	};
}
