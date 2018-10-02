#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <functional>

#include "Entity.h"

namespace ECS {
	typedef float DeltaSeconds;

	class EntityList
	{
	public:
		EntityList() : m_IDCounter(0) {}
		~EntityList() {}
		EntityList(EntityList &rhs) = delete;

		size_t Size() const { return m_Entities.size(); }

		EntityID Add(std::unique_ptr<Entity> entity) {
			++m_IDCounter;
			entity->SetID(m_IDCounter);
			m_Entities.insert({ m_IDCounter, std::move(entity) });
			return m_IDCounter;
		}

		bool Delete(EntityID id) { return m_Entities.erase(id) != 0; }

		template<typename ...T>
		typename std::enable_if<sizeof...(T) != 0, void>::type
		Run(std::function<void(DeltaSeconds, Entity&)> f, DeltaSeconds deltaSeconds) {
			for (auto i = m_Entities.begin(); i != m_Entities.end(); ++i) {
				if (i->second->Has<T...>()) {
					f(deltaSeconds, *i->second);
				}
			}
		}

		void Run(std::function<void(DeltaSeconds, Entity&)> f, DeltaSeconds deltaSeconds) {
			for (auto i = m_Entities.begin(); i != m_Entities.end(); ++i) {
				f(deltaSeconds, *i->second);
			}
		}

	private:
		std::unordered_map<EntityID, std::unique_ptr<Entity>> m_Entities;

		EntityID m_IDCounter;
	};
}
