#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "FrameBuffer.h"
#include "Rendering/Camera.h"

namespace Hexy 
{
	struct SceneRenderingSettings
	{
		bool ShowBoundingBoxes = true;
	};

	class SceneRenderer
	{
	private:
		static SceneRenderingSettings m_settings;
	public:
		static void Init();

		static void Begin();
		static void End();

		static void SetViewportSize(int width, int height);

		//Renders scene from point of camera view
		static void Render(Camera& camera, glm::mat4& viewMatrix);

		static void SubmitMesh(std::shared_ptr<Mesh> mesh, glm::mat4 transform);
		static void SubmitSelectedMesh(std::shared_ptr<Mesh> mesh, glm::mat4 transform);

		static void ClearRenderingQueue();

		static SceneRenderingSettings& GetSettings();
		static void SetSettings(SceneRenderingSettings settings);
	};
}