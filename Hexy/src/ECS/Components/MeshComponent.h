#pragma once
#include "Rendering/Mesh.h"
#include "ECS/Components/Component.h"

namespace Hexy
{
	struct MeshComponent : public Component
	{
		std::shared_ptr<Mesh> mesh;
		MeshComponent() = default;
		~MeshComponent() = default;
		operator std::shared_ptr<Mesh>() { return mesh; }
	};
}