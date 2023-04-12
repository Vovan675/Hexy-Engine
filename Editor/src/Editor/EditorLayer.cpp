#include "EditorLayer.h"

namespace Hexy
{
	namespace Editor 
	{
		std::unordered_map<entt::entity, bool> EditorLayer::Selection;
		std::vector<entt::entity> EditorLayer::SelectionContext;

		void EditorLayer::SetUniforms()
		{
			entityMaterial->Set("u_AlbedoTexture", m_AlbedoInput.texture);
			entityMaterial->Set("u_useAlbedoTexture", (float)m_AlbedoInput.useTexture);

			entityMaterial->Set("u_MetalnessTexture", m_MetalnessInput.texture);
			entityMaterial->Set("u_useMetalnessTexture", (float)m_MetalnessInput.useTexture);

			entityMaterial->Set("u_RoughnessTexture", m_RoughnessInput.texture);
			entityMaterial->Set("u_useRoughnessTexture", (float)m_RoughnessInput.useTexture);

			entityMaterial->Set("u_NormalTexture", m_NormalInput.texture);
			entityMaterial->Set("u_useNormalTexture", (float)m_NormalInput.useTexture);

			entityMaterial->Set("u_AOTexture", m_AOInput.texture);
			entityMaterial->Set("u_useAOTexture", (float)m_AOInput.useTexture);
		}

		void EditorLayer::OnAttach()
		{
			rendererSettings = SceneRenderer::GetSettings();
			ScriptEngine::LoadAssembly("assets/scripts/ScriptCustom.dll");
			
			int width;
			int height;
			glfwGetWindowSize(Application::Instance->GetWindow(), &width, &height);

			m_scene = SceneManager::CreateScene();

			Entity e = m_scene->CreateEntity();
			auto& mesh = e.AddComponent<MeshComponent>().mesh;
			//mesh = Mesh::Create("assets/models/blender/test2.fbx");
			e.AddComponent<ScriptComponent>("ScriptCustom.TestScript");
			mesh = Mesh::Create("assets/models/1/Cerberus_LP.fbx");
			e.GetComponent<TransformComponent>().scale = { 0.01, 0.01, 0.01 };
			//mesh = Mesh::Create("assets/models/sphere.fbx");
			entityMaterial = mesh->GetMaterials()[0];


			editorHierarchy = new EditorHierarchy();
			editorInspector = new EditorInspector();
			editorScene = new EditorScene();
			editorMenu = new EditorMenu();

			/*
			m_AlbedoInput.texture = TextureLibrary::Load("assets/textures/sci/albedo.png");
			m_MetalnessInput.texture = TextureLibrary::Load("assets/textures/sci/metalness.png");
			m_RoughnessInput.texture = TextureLibrary::Load("assets/textures/sci/roughness.png");
			m_NormalInput.texture = TextureLibrary::Load("assets/textures/sci/normal.png");
			m_AOInput.texture = TextureLibrary::Load("assets/textures/sci/ao.png");
			*/
			
			m_AlbedoInput.texture = TextureLibrary::Load("assets/models/1/Textures/Cerberus_A.tga");
			m_MetalnessInput.texture = TextureLibrary::Load("assets/models/1/Textures/Cerberus_M.tga");
			m_RoughnessInput.texture = TextureLibrary::Load("assets/models/1/Textures/Cerberus_R.tga");
			m_NormalInput.texture = TextureLibrary::Load("assets/models/1/Textures/Cerberus_N.tga");
			m_AOInput.texture = TextureLibrary::Load("assets/models/1/Textures/Raw/Cerberus_AO.tga");
			
			/*
			entityMaterial->Set("u_MetalnessTexture", TextureLibrary::Load("assets/models/1/Textures/Cerberus_M.tga"));
			entityMaterial->Set("u_useMetalnessTexture", 1.0f);

			entityMaterial->Set("u_RoughnessTexture", TextureLibrary::Load("assets/models/1/Textures/Cerberus_R.tga"));
			entityMaterial->Set("u_useRoughnessTexture", 1.0f);

			entityMaterial->Set("u_NormalTexture", TextureLibrary::Load("assets/models/1/Textures/Cerberus_N.tga"));
			entityMaterial->Set("u_useNormalTexture", 1.0f);

			entityMaterial->Set("u_AOTexture", TextureLibrary::Load("assets/models/1/Textures/Raw/Cerberus_AO.tga"));
			entityMaterial->Set("u_useAOTexture", 1.0f);
			*/

			SetUniforms();
		}
		void EditorLayer::OnUpdate(double deltaTime)
		{
			SceneRenderer::SetSettings(rendererSettings);
			if (m_editorState == PLAY)
			{
				SceneManager::GetCurrentScene()->OnUpdate(deltaTime);
				SceneManager::GetCurrentScene()->OnRenderRuntime(deltaTime);
			}
			else if (m_editorState == STOP)
			{
				SceneManager::GetCurrentScene()->OnRenderEditor(deltaTime, SelectionContext);
			}

			editorScene->OnUpdate(); //For rendering into editor camera
			///Renderer::Render(CameraComponent::activeCamera);
			SceneRenderer::ClearRenderingQueue();
		}

		bool EditorLayer::OnMouseButtonEvent(int button, int action)
		{
			return 0;
		}

		bool EditorLayer::OnScrollEvent(int xScroll, int yScroll)
		{
			return 0;
		}

		bool EditorLayer::OnWindowSizeEvent(int width, int height)
		{
			return 0;
		}

		void EditorLayer::OnImGui()
		{
			ImGui::Begin("Debug Info");
			ImGui::Value("Draw Calls", DebugInfo::DrawCalls);
			DebugInfo::DrawCalls = 0;
			ImGui::Value("Shaders", DebugInfo::ShaderLoaded);
			ImGui::Value("Textures", DebugInfo::TextureLoaded);
			if (ImGui::Button("Reload Shader")) 
			{
				SetUniforms();
			}
			ImGui::End();
			
			//Viewport
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
				ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;
			ImGui::Begin("DockSpace", 0, window_flags);

			ImGuiIO& io = ImGui::GetIO();
			ImGuiID dockId = ImGui::GetID("MyDockSpace");

			if (ImGui::DockBuilderGetNode(dockId) == NULL)
			{
				ImGui::DockBuilderRemoveNode(dockId);
				ImGui::DockBuilderAddNode(dockId, ImGuiDockNodeFlags_None);
				ImGui::DockBuilderSetNodeSize(dockId, viewport->Size);

				ImGuiID leftId, leftUpId, leftDownId, middleId, middleUpId, middleDownId, rightId, rightUpId, rightDownId;
				ImGui::DockBuilderSplitNode(dockId, ImGuiDir_Left, 0.3f, &leftId, &middleId); //Left and middle
				ImGui::DockBuilderSplitNode(leftId, ImGuiDir_Up, 0.3f, &leftUpId, &leftDownId); //Left(up and down)
				ImGui::DockBuilderSplitNode(middleId, ImGuiDir_Left, 0.65f, &middleId, &rightId); //middle and right
				ImGui::DockBuilderSplitNode(middleId, ImGuiDir_Down, 0.85f, &middleUpId, &middleDownId); //middle up and middle down
				ImGui::DockBuilderSplitNode(rightId, ImGuiDir_Up, 0.65f, &rightUpId, &rightDownId); //right up and right down
				ImGui::DockBuilderDockWindow("Hierarchy", leftUpId);
				ImGui::DockBuilderDockWindow("Inspector", leftDownId);
				ImGui::DockBuilderDockWindow("MaterialEditor", rightUpId);
				ImGui::DockBuilderDockWindow("Settings", rightDownId);
				ImGui::DockBuilderDockWindow("Toolbar", middleUpId);
				ImGui::DockBuilderDockWindow("Viewport", middleDownId);

				ImGui::DockBuilderFinish(dockId);
			}
			ImGui::DockSpace(dockId, ImVec2(0, 0));

			editorHierarchy->OnImgui();
			editorInspector->OnImgui();
			editorScene->OnImgui();
			editorMenu->OnImgui();

			//Settings
			ImGui::Begin("Settings");
			ImGui::Checkbox("Show AABB", &rendererSettings.ShowBoundingBoxes);
			ImGui::End();

			//Toolbar
			ImGui::Begin("Toolbar");
			if (ImGui::Button("Play/Stop"))
			{
				if (m_editorState == PLAY)
				{
					SceneManager::GetCurrentScene()->OnRuntimeStop();
					m_editorState = STOP;
				}
				else if (m_editorState == STOP)
				{
					SceneManager::GetCurrentScene()->OnRuntimeStart();
					m_editorState = PLAY;
				}
			}
			ImGui::End();

			//Material Editor
			
			ImGui::Begin("MaterialEditor");
			
			//Albedo
			if (ImGui::CollapsingHeader("Albedo", ImGuiTreeNodeFlags_DefaultOpen)) 
			{
				if (ImGui::Checkbox("Use Albedo Texture", &m_AlbedoInput.useTexture)) 
				{
					entityMaterial->Set("u_useAlbedoTexture", (float)m_AlbedoInput.useTexture);
				}

				ImGui::Image((ImTextureID)m_AlbedoInput.texture->GetTexture(), { 64, 64 });
				if (ImGui::IsItemClicked())
				{
					std::string file = FileSystem::OpenFileDialog();
					if (file != "") 
					{
						m_AlbedoInput.texture = Texture2D::Create(file);
						entityMaterial->Set("u_AlbedoTexture", m_AlbedoInput.texture);
					}
				}

				if (ImGui::ColorEdit3("Color", glm::value_ptr(m_AlbedoInput.color), ImGuiColorEditFlags_NoInputs))
				{
					entityMaterial->Set("u_Albedo", m_AlbedoInput.color);
				}
			}
			//Metalness
			if (ImGui::CollapsingHeader("Metalness", ImGuiTreeNodeFlags_DefaultOpen)) 
			{
				if (ImGui::Checkbox("Use Metalness Texture", &m_MetalnessInput.useTexture)) 
				{
					entityMaterial->Set("u_useMetalnessTexture", (float)m_MetalnessInput.useTexture);
				}

				ImGui::Image((ImTextureID)m_MetalnessInput.texture->GetTexture(), { 64, 64 });
				if (ImGui::IsItemClicked())
				{
					std::string file = FileSystem::OpenFileDialog();
					if (file != "")
					{
						m_MetalnessInput.texture = Texture2D::Create(file);
						entityMaterial->Set("u_MetalnessTexture", m_MetalnessInput.texture);
					}
				}

				if (ImGui::DragFloat("Metalness##Color", &m_MetalnessInput.value, 0.01f, 0.0f, 1.0f))
				{
					entityMaterial->Set("u_Metalness", m_MetalnessInput.value);
				}
			}
			//Roughness
			if (ImGui::CollapsingHeader("Roughness", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::Checkbox("Use Roughness Texture", &m_RoughnessInput.useTexture))
				{
					entityMaterial->Set("u_useRoughnessTexture", (float)m_RoughnessInput.useTexture);
				}

				ImGui::Image((ImTextureID)m_RoughnessInput.texture->GetTexture(), { 64, 64 });
				if (ImGui::IsItemClicked()) 
				{
					std::string file = FileSystem::OpenFileDialog();
					if (file != "") 
					{
						m_RoughnessInput.texture = Texture2D::Create(file);
						entityMaterial->Set("u_RoughnessTexture", m_RoughnessInput.texture);
					}
				}

				if (ImGui::DragFloat("Roughness##Color", &m_RoughnessInput.value, 0.01f, 0.0f, 1.0f)) 
				{
					entityMaterial->Set("u_Roughness", m_RoughnessInput.value);
				}
			}
			//Normal
			if (ImGui::CollapsingHeader("Normal", ImGuiTreeNodeFlags_DefaultOpen)) 
			{
				if (ImGui::Checkbox("Use Normal Texture", &m_NormalInput.useTexture)) 
				{
					entityMaterial->Set("u_useNormalTexture", (float)m_NormalInput.useTexture);
				}

				ImGui::Image((ImTextureID)m_NormalInput.texture->GetTexture(), { 64, 64 });
				if (ImGui::IsItemClicked())
				{
					std::string file = FileSystem::OpenFileDialog();
					if (file != "")
					{
						m_NormalInput.texture = Texture2D::Create(file);
						entityMaterial->Set("u_NormalTexture", m_NormalInput.texture);
					}
				}
			}
			//AO
			if (ImGui::CollapsingHeader("AO", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::Checkbox("Use AO Texture", &m_AOInput.useTexture)) 
				{
					entityMaterial->Set("u_useAOTexture", (float)m_AOInput.useTexture);
				}

				ImGui::Image((ImTextureID)m_AOInput.texture->GetTexture(), { 64, 64 });
				if (ImGui::IsItemClicked()) 
				{
					std::string file = FileSystem::OpenFileDialog();
					if (file != "") 
					{
						m_AOInput.texture = Texture2D::Create(file);
						entityMaterial->Set("u_AOTexture", m_AOInput.texture);
					}
				}
			}
			ImGui::End();

			ImGui::End();
		}
	}
}