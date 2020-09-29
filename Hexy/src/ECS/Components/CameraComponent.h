#pragma once
#include "Application.h"
#include "Component.h"
#include "Input.h"
#include "Rendering/FrameBuffer.h"
#include "Math/Ray.h"

#include "glm/gtx/quaternion.hpp"

namespace Hexy {

	class CameraComponent : public Component {
	private:
		glm::mat4 m_projection;
		glm::mat4 m_view;
		float m_aspect;

		//In future maybe hold there not postprocessed frame
		std::shared_ptr<Framebuffer> m_framebuffer;
	public:
		static CameraComponent* activeCamera;

		CameraComponent(int width = 800, int height = 600);
		~CameraComponent();

		void OnWindowSizeEvent(int width, int height);

		glm::mat4 GetViewMatrix();
		inline glm::mat4 GetProjectionMatrix() { return m_projection; }
		inline glm::mat4 GetMatrix() { return m_projection * GetViewMatrix(); }

		inline std::shared_ptr<Framebuffer> GetFramebuffer() { return m_framebuffer; }

		glm::vec2 ScreenToWorldPoint(glm::vec2 position);
		glm::vec2 ScreenToWorldPointDelta(glm::vec2 position);
		glm::vec2 ViewportToWorldPoint(glm::vec2 position);
		glm::vec2 ViewportToWorldPointDelta(glm::vec2 position);
		Ray ViewportPointToRay(glm::vec2 position);

		void UpdateProjection();
	};
}