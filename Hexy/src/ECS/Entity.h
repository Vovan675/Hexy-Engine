#pragma once
#include "Rendering/Scene.h"
#include <entt/entt.hpp>

namespace Hexy 
{
	class Entity
	{
	private:
		friend class Scene;

		entt::entity m_entity = entt::null;
		Scene* m_scene = nullptr;
	public:
		std::string name = "";

		Entity() = default;
		Entity(entt::entity entity, Scene* scene) : m_entity(entity), m_scene(scene) {}

		template<typename ...T>
		decltype(auto) GetComponent() 
		{
			return m_scene->m_registry.get<T...>(m_entity);
		}

		template<typename ...T>
		decltype(auto) GetComponent() const
		{
			return m_scene->m_registry.get<T...>(m_entity);
		}

		template<typename ...T>
		decltype(auto) TryGetComponent() 
		{
			return m_scene->m_registry.try_get<T...>(m_entity);
		}

		template<typename ...T>
		decltype(auto) TryGetComponent() const
		{
			return m_scene->m_registry.try_get<T...>(m_entity);
		}

		template<typename T, typename ...Args>
		T& AddComponent(Args&&... args)
		{
			return m_scene->m_registry.get_or_emplace<T>(m_entity, std::forward<Args>(args)...);
		}

		template<typename T>
		bool HasComponent() const
		{
			return m_scene->m_registry.has<T>(m_entity);
		}

		operator uint32_t() 
		{
			return (uint32_t)m_entity;
		}
	/*
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive) const { //TODO: FIX NAME IN BINARY SERIALIZATION
			archive(cereal::make_nvp("Name", name));
			archive(cereal::make_nvp("TransformComponent", transform));
			std::vector<ComponentType> componentTypes;
			for (Component* component : m_components) {
				componentTypes.push_back(component->GetType());
			}
			archive(componentTypes);

			for (Component* component : m_components) {
				component->Serialize(archive);
			}

		}

		template <class Archive>
		void load(Archive& archive) {
			archive(cereal::make_nvp("Name", name));
			archive(cereal::make_nvp("TransformComponent", transform));
			
			std::vector<ComponentType> componentTypes;
			archive(componentTypes);
			
			for (ComponentType type : componentTypes) {
				Component* component = CreateComponentByType(type);
				component->owner = this;
				component->Deserialize(archive);

				m_components.push_back(component);
			}
			
		}
	*/
	};
}