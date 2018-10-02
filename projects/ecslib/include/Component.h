#pragma once
namespace ECS {
	namespace Internal {
		class BaseComponent
		{
		public:
			BaseComponent() {}
			virtual ~BaseComponent() {}
			BaseComponent(const BaseComponent &rhs) {}
		};
	}

	template<typename T>
	class Component : public Internal::BaseComponent
	{
	public:
		Component() {}
		Component(const T &rhs) : data(rhs) {}
		Component(const T &&rhs) : data(rhs) {}
		~Component() {}
		Component(const Component<T> &rhs) : data(rhs.data) { }

		T data;
	};
}

