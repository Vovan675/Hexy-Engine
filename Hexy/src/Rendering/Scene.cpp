#include "pch.h"
#include "Scene.h"
#include "ECS/Entity.h"
#include "Utils/FileSystem.h"

#include "ECS/Components/NameComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/ScriptComponent.h"

#include "Scripting/ScriptEngine.h"

#include "Utils/SceneManager.h"

namespace Hexy 
{
	void OnScriptComponentConstuct(entt::registry& registry, entt::entity entity)
	{
		ScriptEngine::RegisterEntity(Entity(entity, SceneManager::GetCurrentScene()));
	}

	Scene::Scene()
	{
		m_registry.on_construct<ScriptComponent>().connect<&OnScriptComponentConstuct>();
	}

	Scene::~Scene()
	{
		m_registry.clear();
	}

	void Scene::OnUpdate(float deltaTime)
	{
		{
			auto view = m_registry.view<ScriptComponent>();
			for (auto entity : view) {
				Entity e(entity, this);
				if (ScriptEngine::Exists(e.GetComponent<ScriptComponent>().script))
				{
					ScriptEngine::OnUpdate(e, deltaTime);
				}
			}
		}
	}

	void Scene::OnRuntimeStart()
	{
		{
			auto view = m_registry.view<ScriptComponent>();
			for (auto entity : view) {
				Entity e(entity, this);
				if (ScriptEngine::Exists(e.GetComponent<ScriptComponent>().script))
				{
					ScriptEngine::InstantiateClass(e);
					ScriptEngine::OnStart(e);
				}
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
	}

	void Scene::OnRenderEditor(float deltaTime, std::vector<entt::entity>& selection)
	{
		OnRenderRuntime(deltaTime);
		{
			auto group = m_registry.group<MeshComponent, TransformComponent>();
			for (auto entity : group) 
			{
				auto [mesh, transform] = group.get<MeshComponent, TransformComponent>(entity);
				auto sel = std::find(selection.begin(), selection.end(), entity);
				if (sel != selection.end()) {
					SceneRenderer::SubmitSelectedMesh(mesh.mesh, transform.matrix);
				}
			}
		}
	}

	void Scene::OnRenderRuntime(float deltaTime)
	{
		{
			auto view = m_registry.view<TransformComponent>();
			for (auto entity : view) {
				view.get<TransformComponent>(entity).UpdateMatrix();
			}
		}

		{
			auto group = m_registry.group<MeshComponent, TransformComponent>();
			for (auto entity : group) {
				auto [mesh, transform] = group.get<MeshComponent, TransformComponent>(entity);
				SceneRenderer::SubmitMesh(mesh.mesh, transform.matrix);
			}
		}	
	}

	/*
	void Scene::SetMainCamera(CameraComponent* mainCamera)
	{
		m_mainCamera = mainCamera;
	}
	*/

	Entity Scene::CreateEntity(const std::string& name)
	{
		auto entity = Entity(m_registry.create(), this);
		entity.AddComponent<NameComponent>(name);
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		m_registry.destroy(entity.m_entity);
	}

	std::vector<entt::entity> Scene::GetEntities()
	{
		auto& registry = m_registry;
		std::vector<entt::entity> entities;
		m_registry.each([&entities, &registry](auto entity) {
			entities.push_back(entity);
		});
		return entities;
	}
}