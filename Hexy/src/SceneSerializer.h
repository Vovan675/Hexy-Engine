#pragma once
#include "Rendering/Scene.h"
#include "ECS/Entity.h"

namespace Hexy
{
	class SceneSerializer 
	{
	public:
		static void Serialize(Scene* scene, const std::string& fileName, bool binary = false);
		static void Deserialize(Scene* scene, const std::string& fileName, bool binary = false);
	};
}