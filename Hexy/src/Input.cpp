#include "pch.h"
#include "Input.h"

namespace Hexy
{
	glm::vec2 Input::m_mouseDelta = { 0, 0 };
	glm::vec2 Input::m_lastMousePos = { -1, -1 };

	glm::vec2 Input::m_mouseScrollDelta = { 0, 0 };
}