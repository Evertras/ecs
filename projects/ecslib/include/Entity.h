#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "Component.h"

namespace ECS
{
	typedef unsigned long EntityID;

	const EntityID UnassignedID = static_cast<unsigned long>(-1);

	class Entity
	{
	public:
		Entity() : m_ID(UnassignedID) {}
		~Entity() {}
		Entity(const Entity& rhs) = delete;

		size_t NumComponents() const
		{
			return m_Components.size();
		}

		template<typename T>
		void AddComponent(const T& data)
		{
			m_Components.insert_or_assign(typeid(T), std::make_unique<Component<T>>(data));
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Components.erase(typeid(T));
		}

		template<typename T>
		bool Has() const
		{
			return m_Components.find(typeid(T)) != m_Components.end();
		}

		template<typename T, typename ...Components>
		typename std::enable_if < sizeof...(Components) != 0, bool >::type
		Has() const
		{
			return Has<T>() && Has<Components...>();
		}

		template<typename T>
		T& Data()
		{
			return std::ref(dynamic_cast<Component<T>*>(m_Components[typeid(
			                    T)].get())->data);
		}

		template<typename T, typename ...Others>
		typename std::enable_if < sizeof...(Others) != 0,
		         std::tuple<T&, Others& ... >>::type
		         Data()
		{
			return std::tuple<T&, Others& ...>(
			           std::ref(dynamic_cast<Component<T>*>(m_Components[typeid(T)].get())->data),
			           Data<Others...>()
			       );
		}

		EntityID ID() const
		{
			return m_ID;
		}

	private:
		std::unordered_map<std::type_index, std::unique_ptr<Internal::BaseComponent>>
		        m_Components;
		EntityID m_ID;

		// EntityList should call this instead of manipulating m_ID directly
		void SetID(EntityID id)
		{
			m_ID = id;
		}

		// EntityList has free reign with Entity
		friend class EntityList;
	};
}
