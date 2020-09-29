#include "pch.h"
#include "SceneCamera.h"

namespace Hexy {
	void SceneCamera::SetViewportSize(float width, float height)
	{
		m_framebuffer->Resize(width, height);
		m_aspect = width / height;
		UpdateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float zNear, float zFar)
	{
		m_fov = fov;
		m_zNear = zNear;
		m_zFar = zFar;
		UpdateProjection();
	}

	void SceneCamera::UpdateProjection()
	{
		m_projectionMatrix = glm::perspectiveRH(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
	}
}