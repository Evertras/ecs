#pragma once

#include "EntityList.h"

namespace ECS {
	typedef std::function<void(DeltaSeconds, Entity&)> SystemFunction;

	class BaseSystem {
	public:
		virtual void Run(EntityList &list, DeltaSeconds deltaSeconds) = 0;
	};

	template<typename ...T>
	class System : public BaseSystem
	{
	public:
		virtual ~System() {}

		void Run(EntityList &list, DeltaSeconds deltaSeconds) override {
			list.Run<T...>(m_F, deltaSeconds);
		}

	protected:
		System(SystemFunction f) : m_F(f) {}
		System() {}

		SystemFunction m_F;
	};
}
