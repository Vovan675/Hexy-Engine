#include "pch.h"
#include "ScriptEngineBinder.h"
#include "Input.h"
#include "ECS/Entity.h"
#include "ECS/Components/TransformComponent.h"
#include "Utils/SceneManager.h"
#include "Log.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>


namespace Hexy
{
	void ScriptEngineBinder::BindCalls()
	{
		mono_add_internal_call("Hexy.Input::IsKeyDown", (void*)Input::IsKeyDown);

		mono_add_internal_call("Hexy.TransformComponent::GetPosition_Internal", (void*)&GetPosition_Internal);
		mono_add_internal_call("Hexy.TransformComponent::SetPosition_Internal", (void*)&SetPosition_Internal);
	}


	void ScriptEngineBinder::GetPosition_Internal(uint32_t entityID, glm::vec3& vec)
	{
		Hexy::Entity entity = Hexy::Entity((entt::entity)entityID, SceneManager::GetCurrentScene());
		vec = entity.GetComponent<TransformComponent>().position;
	}

	void ScriptEngineBinder::SetPosition_Internal(uint32_t entityID, const glm::vec3& vec)
	{
		Hexy::Entity entity = Hexy::Entity((entt::entity)entityID, SceneManager::GetCurrentScene());
		entity.GetComponent<TransformComponent>().position = vec;
	}
}