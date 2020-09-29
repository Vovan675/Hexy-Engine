#include "EditorCamera.h"

namespace Hexy
{
	namespace Editor
	{
		EditorCamera::EditorCamera()
		{
			auto size = Application::Instance->GetWindowSize();
			m_framebuffer = Framebuffer::Create(size.x, size.y, 0, true);
			UpdateProjection();
			Update(0);
		}

		void EditorCamera::Update(float deltaTime)
		{
			glm::vec2 mouseDelta = Input::GetMousePositionDelta() * 0.003f;
			glm::vec2 mouseScroll = Input::GetMouseScrollPositionDelta();
			
			bool isFlying = false;
			//Panning
			if (Input::IsMouseButtonDown(2) && Input::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) 
			{
				m_focalPoint += -m_right * mouseDelta.x * m_distance;
				m_focalPoint += m_up * mouseDelta.y * m_distance;

				isUsing = true;
			}
			//Rotate around
			else if (Input::IsMouseButtonDown(2))
			{
				float yawSign = m_up.y < 0.0f ? -1.0f : 1.0f;
				m_pitch -= mouseDelta.y;
				m_yaw -= mouseDelta.x * yawSign;

				isUsing = true;
			}
			//Zooming in/out
			else if (mouseScroll.y != 0) 
			{
				m_distance -= mouseScroll.y;

				isUsing = true;
			}
			//Flying around
			else if (Input::IsMouseButtonDown(1))
			{
				float yawSign = m_up.y < 0.0f ? -1.0f : 1.0f;
				m_pitch -= mouseDelta.y;
				m_yaw -= mouseDelta.x * yawSign;

				isFlying = true;
				isUsing = true;
			}
			else 
			{
				isUsing = false;
			}
			
			if (isFlying) 
			{
				UpdateOrientation();
				UpdateDirections();

				//Calculate new focal point
				m_focalPoint = m_position + m_forward * m_distance;

				float flySpeed = Input::IsKeyDown(GLFW_KEY_LEFT_SHIFT) ? 8.0f : 4.0f;
				if (Input::IsKeyDown('A'))
					m_focalPoint += -m_right * deltaTime * flySpeed;

				if (Input::IsKeyDown('D'))
					m_focalPoint += m_right * deltaTime * flySpeed;

				if (Input::IsKeyDown('W'))
					m_focalPoint += m_forward * deltaTime * flySpeed;

				if (Input::IsKeyDown('S'))
					m_focalPoint += -m_forward * deltaTime * flySpeed;

				UpdatePosition();
				UpdateView();
			}
			else
			{
				UpdateOrientation();
				UpdateDirections();
				UpdatePosition();
				UpdateView();
			}
		}

		void EditorCamera::SetViewportSize(float width, float height)
		{
			m_framebuffer->Resize(width, height);
			m_aspect = width / height;
			UpdateProjection();
		}

		void EditorCamera::UpdateView()
		{
			m_view = glm::translate(glm::mat4(1.0), m_position) * glm::toMat4(m_orientation);
			m_view = glm::inverse(m_view);
		}

		void EditorCamera::UpdateProjection()
		{
			m_projectionMatrix = glm::perspectiveRH(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
		}

		void EditorCamera::UpdateOrientation()
		{
			m_orientation = glm::quat(glm::vec3(m_pitch, m_yaw, 0));
		}

		void EditorCamera::UpdateDirections()
		{
			m_forward = glm::rotate(m_orientation, glm::vec3(0, 0, -1));
			m_right = glm::rotate(m_orientation, glm::vec3(1, 0, 0));
			m_up = glm::rotate(m_orientation, glm::vec3(0, 1, 0));
		}

		void EditorCamera::UpdatePosition()
		{
			m_position = m_focalPoint - m_forward * m_distance;
		}
	}
}