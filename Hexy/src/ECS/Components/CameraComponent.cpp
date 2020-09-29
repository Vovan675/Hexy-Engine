#include "pch.h"
#include "CameraComponent.h"

namespace Hexy 
{
	CameraComponent* CameraComponent::activeCamera;

	CameraComponent::CameraComponent(int width, int height)
	{
		m_view = glm::mat4(1);

		m_aspect = (float)width / (float)height;
		UpdateProjection();

		m_framebuffer = Framebuffer::Create(width, height);

		if (activeCamera == NULL)
		{
			activeCamera = this;
		}
	}

	CameraComponent::~CameraComponent()
	{
		if (activeCamera == this)
		{
			activeCamera = NULL;
		}
	}

	void CameraComponent::OnWindowSizeEvent(int width, int height)
	{
		if (this->m_aspect == (float)width / (float)height)
			return;
		m_aspect = (float)width / (float)height;
		m_framebuffer->Resize(width, height);
		UpdateProjection();
	}

	glm::mat4 CameraComponent::GetViewMatrix()
	{
		//TODO
		///m_view = glm::translate(glm::mat4(1.0), owner->transform.position) * glm::toMat4(owner->transform.GetOrientation());
		m_view = glm::inverse(m_view);
		return m_view;
	}

	void CameraComponent::UpdateProjection()
	{
		//m_projection = glm::ortho(-m_aspect * m_distance, m_aspect * m_distance, -m_distance, m_distance, -1.0f, 1.0f);
		m_projection = glm::perspectiveRH(glm::radians(45.0f), m_aspect, 0.1f, 100.0f);
	}

	glm::vec2 CameraComponent::ScreenToWorldPoint(glm::vec2 position)
	{
		glm::vec2 size = Application::Instance->GetWindowSize();
		position.y = size.y - position.y;
		position -= size / 2.0f;

		glm::vec2 vec = position / (size.y / 2.0f);


		///vec *= m_distance;
		///vec += glm::vec2(m_transform);
		return vec;
	}

	glm::vec2 CameraComponent::ScreenToWorldPointDelta(glm::vec2 position)
	{
		return ScreenToWorldPoint(position + 0.5f);
	}

	//Convert [0; 1] to world point
	glm::vec2 CameraComponent::ViewportToWorldPoint(glm::vec2 position)
	{
		glm::vec2 size = Application::Instance->GetWindowSize();
		position *= size;
		position.y = size.y - position.y;
		position -= size / 2.0f;

		glm::vec2 vec = position / (size.y / 2.0f);

		///vec *= m_distance;
		///vec += glm::vec2(m_transform);
		return vec;
	}

	glm::vec2 CameraComponent::ViewportToWorldPointDelta(glm::vec2 position)
	{
		return ViewportToWorldPoint(position + 0.5f);
	}

	Ray CameraComponent::ViewportPointToRay(glm::vec2 position)
	{
		//TODO
		Ray ray;
		/*

		//glm::vec3 rayOrigin = glm::translate(glm::mat4(1.0), owner->transform.position) * glm::toMat4(owner->transform.GetOrientation()) * glm::vec4(position.x, position.y, 0.0f, 1.0f);
		//ray.pos = rayOrigin;
		ray.pos = owner->transform.position;

		glm::vec3 rayEnd = glm::translate(glm::mat4(1.0), owner->transform.position) * glm::toMat4(owner->transform.GetOrientation()) * glm::vec4(position.x, position.y, 1.0f, 1.0f);

		//ray.dir = -glm::normalize(glm::vec3(rayEnd - rayOrigin));

		glm::vec3 dir = glm::inverse(glm::mat3(GetViewMatrix())) * glm::vec3(glm::inverse(GetProjectionMatrix()) * glm::vec4(position.x, position.y, -1.0f, 1.0f));
		ray.dir = glm::normalize(dir);
		*/
		return ray;
	}
}
