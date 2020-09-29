#pragma once
#include "FrameBuffer.h"
#include "glm/glm.hpp"

namespace Hexy
{
	class Camera
	{
	protected:
		glm::mat4 m_projectionMatrix;
		std::shared_ptr<Framebuffer> m_framebuffer = 0;
	public:
		Camera() = default;
		Camera(glm::mat4& projectionMatrix) : m_projectionMatrix(projectionMatrix) {}

		inline std::shared_ptr<Framebuffer> GetFramebuffer() { return m_framebuffer; }

		void SetProjection(const glm::mat4& projectionMatrix) { m_projectionMatrix = projectionMatrix; }
		const glm::mat4& GetProjection() { return m_projectionMatrix; }
	};
}