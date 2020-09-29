#pragma once
#include "ECS/Components/CameraComponent.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "SceneRenderer.h"
#include "entt/entt.hpp"

namespace Hexy
{
	class Entity;

	class Scene
	{
	private:
		friend class Entity;
		entt::registry m_registry;
	public:
		Scene();
		~Scene();


		void OnRenderEditor(float deltaTime, std::vector<entt::entity>& selection);
		void OnRenderRuntime(float deltaTime);

		//Main camera will be the entity with camera component
		//void SetMainCamera(CameraComponent* mainCamera);
		//CameraComponent* GetMainCamera() { return m_mainCamera; }

		Entity CreateEntity(const std::string& name = "Empty Entity");
		void DestroyEntity(const Entity& entity);

		std::vector<entt::entity> GetEntities();

		template<typename... T>
		auto GetEntitiesWith() {
			return m_registry.view<T...>();
		}
		
		//std::stringstream Serialize(bool binary);
		//void Deserialize(std::stringstream& stream, bool binary);
	private:
		/*
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive) const {
			int entityCount = m_entities.size();
			archive(entityCount);

			for (auto entity : m_entities) {
				archive(*entity);
				//entity->Serialize(archive);
			}
			//archive(m_entities);
			//archive(cereal::make_nvp("Name", name));
			///archive(cereal::make_nvp("TransformComponent"), transform);
		}

		template <class Archive>
		void load(Archive& archive) {
			int entityCount;
			archive(entityCount);

			for (int i = 0; i < entityCount; i++) {
				Entity* entity = new Entity("");
				archive(*entity);
				//entity->Serialize(archive);
				m_entities.push_back(entity);
			}
			//archive(cereal::make_nvp("Name", name));
			///archive(cereal::make_nvp("TransformComponent"), transform);
		}
		*/
	};
}