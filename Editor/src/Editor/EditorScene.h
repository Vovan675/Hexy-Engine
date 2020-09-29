#pragma once
#include "Hexy.h"
#include "EditorCamera.h"

namespace Hexy 
{
	namespace Editor 
	{
		class EditorScene 
		{
		private:
			EditorCamera editorCamera;
			float cameraZoom = 4.0f;
			glm::vec3 cameraFocalPoint = { 0, 0, 0 };
		public:
			EditorScene();

			void OnUpdate();
			void OnImgui();
			std::pair<glm::vec3, glm::vec3> CastRay(glm::vec2 mousePos);
		};
	}
}