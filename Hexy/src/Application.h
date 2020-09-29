#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "ImGui/ImGuiLayer.h"

//in future next should be deleted if need other OS
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

namespace Hexy 
{
	class Application
	{
	private:
		GLFWwindow* m_window;
		ImGuiLayer* m_imgui;
		std::vector<Layer*> m_layers;
	public:
		static Application* Instance;

		Application();
		~Application();

		void Run();

		void PushLayer(Layer* layer);

		inline GLFWwindow* GetWindow() { return m_window; }
		inline glm::vec2 GetWindowSize()
		{ 
			int width, height;
			glfwGetWindowSize(m_window, &width, &height);
			return { width, height };
		}

		inline float GetWindowAspect() 
		{
			int width, height;
			glfwGetWindowSize(m_window, &width, &height);
			return (float)width / (float)height;
		}

		
	private:
		void InitWindow();
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void window_size_callback(GLFWwindow* window, int width, int height);
	};
}