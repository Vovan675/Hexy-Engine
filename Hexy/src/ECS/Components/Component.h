#pragma once
namespace Hexy 
{
	class Entity;

	class Component 
	{
	protected:
		friend class Entity;
		Entity* owner = nullptr;
		bool notDeleted = 1;
	public:
		virtual ~Component() {}
		//Maybe in future for loading assets state of something, all components will init their assets in OnBegin
		virtual void OnUpdate(float deltaTime) {};

		Entity* GetOwner() { return owner; }
	};
}