#pragma once
#include "glm/glm.hpp"
#include "VertexArray.h"
#include "Shader.h"
#include "ECS/Components/CameraComponent.h"
#include "Texture.h"
#include "Mesh.h"

namespace Hexy
{
	class Renderer2D
	{
	private:
		static std::shared_ptr<VertexArray> quadVA;
		static std::shared_ptr<Shader> basicShader;
		static std::shared_ptr<Texture2D> basicTexture;

		static std::shared_ptr<VertexArray> fullscreenQuadVA;
	public:

		static void Init();

		static void Begin(CameraComponent* camera);
		static void End();

		static void DrawQuad(glm::vec2 position, glm::vec2 scale = { 1, 1 }, glm::vec3 color = { 1, 1, 1 });
		static void DrawQuad(glm::vec2 position, std::shared_ptr<Texture2D>& texture, glm::vec2 scale = { 1, 1 }, glm::vec3 color = { 1, 1, 1 });
		static void DrawQuad(glm::mat4 transform, std::shared_ptr<Texture2D>& texture, glm::vec2 scale = { 1, 1 }, glm::vec3 color = { 1, 1, 1 });

		static void DrawQuad(glm::mat4 transform, glm::vec3 color = { 1, 1, 1 });
		
		static void DrawFulscreenQuad();
	};
}