#pragma once
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "imguizmo.h"
#include "Layer.h"

namespace Hexy
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() {}

		void OnAttach();
		void OnDetach();
		void OnUpdate(float deltaTime);
	};
}