#include "pch.h"
#include "SceneSerializer.h"
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/unordered_set.hpp>

#include "ECS/Components/NameComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/SpriteComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/ScriptComponent.h"

namespace glm
{
    template<class Archive>
    static void serialize(Archive& archive, glm::vec3& vector)
    {
        archive(cereal::make_nvp("x", vector.x),
                cereal::make_nvp("y", vector.y),
                cereal::make_nvp("z", vector.z));
    }
}

namespace Hexy
{
    enum class ObjectIds
    {
        NameComponentID,
        TransformComponentID,
        MeshComponentID,
        ScriptComponentID
    };


    template<class Archive>
    static void save(Archive& archive, TransformComponent const& transform)
    {
        archive(cereal::make_nvp("Position", transform.position));
        archive(cereal::make_nvp("Rotation", transform.rotation));
        archive(cereal::make_nvp("Scale", transform.scale));
    }

    template<class Archive>
    static void load(Archive& archive, TransformComponent& transform)
    {
        archive(cereal::make_nvp("Position", transform.position));
        archive(cereal::make_nvp("Rotation", transform.rotation));
        archive(cereal::make_nvp("Scale", transform.scale));
        transform.UpdateMatrix();
    }


    template<class Archive>
    static void save(Archive& archive, MeshComponent const& mesh)
    {
        archive(cereal::make_nvp("Mesh path", mesh.mesh->GetPath()));
    }

    template<class Archive>
    static void load(Archive& archive, MeshComponent& mesh)
    {
        std::string path;
        archive(cereal::make_nvp("Mesh path", path));
        mesh.mesh = Mesh::Create(path);
    }

	template<class Archive>
	static void serialize(Archive& archive, ScriptComponent& script)
	{
        archive(script.script);
	}


    template<class Archive>
    static void save(Archive& archive, Entity const& entity)
    {
        int componentsCount = 0;

        if (entity.HasComponent<NameComponent>()) componentsCount++;
        if (entity.HasComponent<TransformComponent>()) componentsCount++;
		if (entity.HasComponent<MeshComponent>()) componentsCount++;
		if (entity.HasComponent<ScriptComponent>()) componentsCount++;

        archive(cereal::make_nvp("ComponentsCount", componentsCount));

        if (entity.HasComponent<NameComponent>())
        {
            archive(cereal::make_nvp("Id", ObjectIds::NameComponentID));
            archive(cereal::make_nvp("NameComponent", entity.GetComponent<NameComponent>().Name));
        }
        if (entity.HasComponent<TransformComponent>())
        {
            archive(cereal::make_nvp("Id", ObjectIds::TransformComponentID));
            archive(cereal::make_nvp("TransformComponent", entity.GetComponent<TransformComponent>()));
        }
        if (entity.HasComponent<MeshComponent>())
        {
            archive(cereal::make_nvp("Id", ObjectIds::MeshComponentID));
            archive(cereal::make_nvp("MeshComponent", entity.GetComponent<MeshComponent>()));
        }
		if (entity.HasComponent<ScriptComponent>())
		{
			archive(cereal::make_nvp("Id", ObjectIds::ScriptComponentID));
			archive(cereal::make_nvp("ScriptComponent", entity.GetComponent<ScriptComponent>()));
		}
    }

    template<class Archive>
    static void load(Archive& archive, Entity& entity)
    {
        int componentsCount = 0;
        archive(cereal::make_nvp("ComponentsCount", componentsCount));

        ObjectIds id;
        for (int i = 0; i < componentsCount; i++)
        {
            archive(cereal::make_nvp("Id", id));
            if (id == ObjectIds::NameComponentID)
            {
                entity.AddComponent<NameComponent>();
                archive(cereal::make_nvp("NameComponent", entity.GetComponent<NameComponent>().Name));
            }
            else if (id == ObjectIds::TransformComponentID)
            {
                entity.AddComponent<TransformComponent>();
                archive(cereal::make_nvp("TransformComponent", entity.GetComponent<TransformComponent>()));
            }
            else if (id == ObjectIds::MeshComponentID)
            {
                entity.AddComponent<MeshComponent>();
                archive(cereal::make_nvp("MeshComponent", entity.GetComponent<MeshComponent>()));
            }
			else if (id == ObjectIds::ScriptComponentID)
			{
				entity.AddComponent<ScriptComponent>();
				archive(cereal::make_nvp("ScriptComponent", entity.GetComponent<ScriptComponent>()));
			}
        }
    }

    void SceneSerializer::Serialize(Scene* scene, const std::string& fileName, bool binary)
    {
        std::ofstream os(fileName);
        auto& entities = scene->GetEntities();
        if (binary)
        {
            cereal::BinaryOutputArchive archive(os);

            archive(cereal::make_size_tag(entities.size()));
            for (auto& entityId : entities)
            {
                Entity entity(entityId, scene);
                archive(entity);
            }
        }
        else
        {
            cereal::JSONOutputArchive archive(os);

            archive(cereal::make_size_tag(entities.size()));
            for (auto& entityId : entities)
            {
                Entity entity(entityId, scene);
                archive(entity);
            }
        }
    }

    void SceneSerializer::Deserialize(Scene* scene, const std::string& fileName, bool binary)
    {
        std::ifstream is(fileName);
        if (binary)
        {
            cereal::BinaryInputArchive archive(is);

            size_t entitiesCount;
            archive(cereal::make_size_tag(entitiesCount));
            for (int i = 0; i < entitiesCount; i++)
            {
                Entity entity = scene->CreateEntity();
                archive(entity);
            }
        }
        else
        {
            cereal::JSONInputArchive archive(is);

            size_t entitiesCount;
            archive(cereal::make_size_tag(entitiesCount));
            for (int i = 0; i < entitiesCount; i++)
            {
                Entity entity = scene->CreateEntity();
                archive(entity);
            }
        }
    }
}