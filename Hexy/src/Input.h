#pragma once
#include "Application.h"
#include "glm/glm.hpp"
#include <iostream>

namespace Hexy
{
	class Input 
	{
	private:
		friend class Application;
		static glm::vec2 m_mouseDelta;
		static glm::vec2 m_lastMousePos;

		static glm::vec2 m_mouseScrollDelta;
		Input() = delete;
	public:
		static inline bool IsKeyDown(int key) {
			return glfwGetKey(Application::Instance->GetWindow(), key);
		}

		static inline bool IsMouseButtonDown(int button) {
			return glfwGetMouseButton(Application::Instance->GetWindow(), button);
		}

		static inline glm::vec2 GetMousePosition () {
			double x, y;
			glfwGetCursorPos(Application::Instance->GetWindow(), &x, &y);
			return { x, y };
		}

		static inline glm::vec2 GetMousePositionRelative(glm::vec2 pos) {
			double x, y;
			glfwGetCursorPos(Application::Instance->GetWindow(), &x, &y);
			return { x - pos.x, y - pos.y};
		}

		static inline glm::vec2 GetMousePositionDelta() {
			return m_mouseDelta;
		}

		static inline glm::vec2 GetMouseScrollPositionDelta() {
			return m_mouseScrollDelta;
		}

		static inline bool IsInsideWindow(glm::vec2 mousePos, glm::vec2 pos, glm::vec2 size) {
			if (mousePos.x >= pos.x && mousePos.x <= pos.x + size.x && mousePos.y >= pos.y && mousePos.y <= pos.y + size.y) {
				return true;
			}
			return false;
		}

		static inline void Update()
		{
			if (m_lastMousePos == glm::vec2(-1, -1))
			{
				m_lastMousePos = GetMousePosition();
				m_mouseDelta = { 0, 0 };
			}
			else 
			{
				glm::vec2 mouse = GetMousePosition();
				m_mouseDelta = mouse - m_lastMousePos;
				m_lastMousePos = mouse;
			}
			m_mouseScrollDelta = { 0, 0 };
		}
	};
}