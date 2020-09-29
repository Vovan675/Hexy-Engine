#pragma once
#include "Camera.h"

namespace Hexy 
{
	class SceneCamera : public Camera {
	private:
		float m_aspect = 1280.0f / 720.0f;
		float m_fov, m_zNear, m_zFar;
	public:
		SceneCamera() = default;

		void SetViewportSize(float width, float height);

		void SetPerspective(float fov, float zNear, float zFar);
	private:
		void UpdateProjection();
	};
}