#pragma once
#include "Hexy.h"

namespace Hexy
{
	namespace Editor
	{
		class EditorCamera : public Camera 
		{
		private:
			float m_aspect = 1280.0f / 720.0f;
			float m_fov = 45, m_zNear = 0.1, m_zFar = 1000;

			glm::mat4 m_view = glm::mat4(1);

			float m_distance = 10.0f;
			glm::vec3 m_position = { 0, 0, 0 };
			glm::quat m_orientation;
			float m_pitch = 0, m_yaw = 3.1415926 / 2.0;

			glm::vec3 m_focalPoint = { 0, 0, 0 };

			glm::vec3 m_forward, m_right, m_up;
			float m_zoomVelocity = 0;
		public:
			bool isUsing = false;
		public:
			EditorCamera();

			void Update(float deltaTime);

			void SetViewportSize(float width, float height);

			glm::mat4& GetView() { return m_view; }
			glm::vec3& GetPosition() { return m_position; }
			
			void SetFocalPoint(const glm::vec3& focalPoint) { m_focalPoint = focalPoint; }
			void SetDistance(float distance) { m_distance = distance; }
		private:
			void UpdateView();
			void UpdateProjection();
			void UpdateOrientation();
			void UpdateDirections();
			void UpdatePosition();
		};
	}
}

