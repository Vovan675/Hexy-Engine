#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "FrameBuffer.h"
#include "Rendering/Camera.h"

namespace Hexy 
{
	class Renderer
	{
	private:
		friend class SceneRenderer;
		static std::shared_ptr<Shader> m_lineShader;
	public:
		static void Init();

		static void Begin();
		static void End();

		static void DrawMesh(std::shared_ptr<Mesh> mesh, glm::mat4 transform);

		static void DrawLine(glm::vec3 a, glm::vec3 b);
		static void DrawAABB(const AABB& aabb, glm::mat4& transform);
	};
}