#include "pch.h"
#include "Application.h"
#include "Log.h"
#include "Timer.h"
#include "Input.h"
#include "Rendering/Renderer.h"
#include "Rendering/Renderer2D.h"
#include "Rendering/SceneRenderer.h"

namespace Hexy
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		if (id == 131218) return;
		if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) 
		{
			std::cout << message << std::endl;
		}
	}

	Application* Application::Instance = nullptr;

	Application::Application() : m_window(nullptr)
	{
		if (Application::Instance) 
		{
			std::cout << "Application already exists!" << std::endl;
			return;
		}
		Application::Instance = this;

		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_SAMPLES, 8);

		InitWindow();

		glLineWidth(2);

		glEnable(GL_MULTISAMPLE);//MSAA
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		
		Log::Init();
		Renderer::Init();
		Renderer2D::Init();
		SceneRenderer::Init();

		m_imgui = new ImGuiLayer();
		PushLayer(m_imgui);
	}

	Application::~Application()
	{
		for (Layer* layer : m_layers)
		{
			layer->OnDetach();
		}

		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Application::Run()
	{
		double lastTime = glfwGetTime();
		while (!glfwWindowShouldClose(m_window))
		{
			double deltaTime = glfwGetTime() - lastTime;
			lastTime = glfwGetTime();
			glfwSetWindowTitle(m_window, std::to_string(1 / deltaTime).c_str());

			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			for(Layer* layer : m_layers)
			{
				layer->OnUpdate(deltaTime);
			}
			
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGuizmo::BeginFrame();

			for (Layer* layer : m_layers)
			{
				layer->OnImGui();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
			
			glfwSwapBuffers(m_window);
			Input::Update();
			glfwPollEvents();
		}
	}

	
	void Application::PushLayer(Layer* layer)
	{
		m_layers.emplace_back(layer);
		layer->OnAttach();
	}

	void Application::InitWindow()
	{
		m_window = glfwCreateWindow(1280, 720, "Application", NULL, NULL);\

		glfwSetWindowSizeCallback(m_window, window_size_callback);
		glfwSetKeyCallback(m_window, key_callback);
		glfwSetMouseButtonCallback(m_window, mouse_button_callback);
		glfwSetScrollCallback(m_window, scroll_callback);

		glfwMakeContextCurrent(m_window);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		glfwSwapInterval(1);

		int width;
		int height;
		glfwGetWindowSize(m_window, &width, &height);
		glViewport(0, 0, width, height);
	}


	void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		for (Layer* layer : Application::Instance->m_layers)
		{
			if (layer->OnKeyEvent(key, action))
				break;
		}
	}

	void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		for (Layer* layer : Application::Instance->m_layers)
		{
			if (layer->OnMouseButtonEvent(button, action))
				break;
		}
	}


	void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Input::m_mouseScrollDelta = { xoffset, yoffset };
		for (Layer* layer : Application::Instance->m_layers)
		{
			if (layer->OnScrollEvent(xoffset, yoffset))
				break;
		}
	}

	void Application::window_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		for (Layer* layer : Application::Instance->m_layers)
		{
			if (layer->OnWindowSizeEvent(width, height))
				break;
		}
	}
}
