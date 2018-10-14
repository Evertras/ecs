#pragma once
#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <memory>
#include <functional>

#include "Entity.h"

namespace ECS {
	typedef float DeltaSeconds;
	typedef std::function<void(ECS::Entity&, ECS::DeltaSeconds)> EntityListFunction;

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

		Entity* Get(EntityID id) {
			auto iter = m_Entities.find(id);

			if (iter != m_Entities.end()) {
				return iter->second.get();
			}
			else {
				return nullptr;
			}
		}

		template<typename ...T>
		typename std::enable_if<sizeof...(T) != 0, Entity*>::type
		First()
		{
			for (auto i = m_Entities.begin(); i != m_Entities.end(); ++i) {
				if (i->second->Has<T...>()) {
					return i->second.get();
				}
			}

			return nullptr;
		}

		// Marks an entity to be deleted in batch, but does not remove it from the list yet
		void MarkDeleted(EntityID id) { m_Deleted.insert(id); }

		void RemoveAllDeleted() {
			for (auto id : m_Deleted) { m_Entities.erase(id); }
			m_Deleted.clear();
		}

		template<typename ...T>
		typename std::enable_if<sizeof...(T) != 0, void>::type
		Run(EntityListFunction f, DeltaSeconds deltaSeconds) {
			for (auto i = m_Entities.begin(); i != m_Entities.end(); ++i) {
				if (i->second->Has<T...>()) {
					f(*i->second, deltaSeconds);
				}
			}
		}

		// Use sparingly
		void RunAll(EntityListFunction f, DeltaSeconds deltaSeconds) {
			for (auto i = m_Entities.begin(); i != m_Entities.end(); ++i) {
				f(*i->second, deltaSeconds);
			}
		}

	private:
		std::unordered_map<EntityID, std::unique_ptr<Entity>> m_Entities;
		std::unordered_set<EntityID> m_Deleted;

		EntityID m_IDCounter;
	};
}
