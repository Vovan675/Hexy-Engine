#pragma once
#include "ECS/Entity.h"

namespace Hexy
{
	class ScriptEngine
	{
	private:
		
	public:
		static void Init();
		static void LoadAssembly(const std::string& assembly);

		static bool Exists(const std::string& name);

		static void RegisterEntity(const Entity& entity);
		static void DestroyScript(const Entity& entity);
		static void InstantiateClass(const Entity& entity);

		static void OnStart(const Entity& entity);
		static void OnUpdate(const Entity& entity, float deltaTime);
	};
}