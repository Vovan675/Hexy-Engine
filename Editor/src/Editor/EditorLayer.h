#pragma once
#include "Hexy.h"
#include "imgui_internal.h"
#include "ImGuizmo.h"

#include "EditorHierarchy.h"
#include "EditorInspector.h"
#include "EditorScene.h"
#include "EditorMenu.h"

namespace Hexy
{
	namespace Editor 
	{
		class EditorLayer : public Layer
		{
		private:
			SceneRenderingSettings rendererSettings;

			enum EditorState
			{
				PLAY,
				STOP
			} m_editorState = STOP;

			Scene* m_scene;
			EditorHierarchy* editorHierarchy;
			EditorInspector* editorInspector;
			EditorScene* editorScene;
			EditorMenu* editorMenu;

			std::shared_ptr<Material> entityMaterial;//In future it must be deleted(ASAP)
		public:
			static std::unordered_map<entt::entity, bool> Selection;
			static std::vector<entt::entity> SelectionContext;

			void SetUniforms();//In future it must be deleted(ASAP)
			void OnAttach();
			void OnUpdate(double deltaTime);
			bool OnMouseButtonEvent(int button, int action);
			bool OnScrollEvent(int xScroll, int yScroll);
			bool OnWindowSizeEvent(int width, int height);
			void OnImGui();
		private:
			struct AlbedoInput {
				glm::vec3 color = glm::vec3(0, 0, 0);
				std::shared_ptr<Texture2D> texture = TextureLibrary::LoadBlankTexture();
				bool useTexture = 0;
			};
			AlbedoInput m_AlbedoInput;

			struct MetalnessInput {
				float value = 0;
				std::shared_ptr<Texture2D> texture = TextureLibrary::LoadBlankTexture();
				bool useTexture = 0;
			};
			MetalnessInput m_MetalnessInput;

			struct RoughnessInput {
				float value = 1;
				std::shared_ptr<Texture2D> texture = TextureLibrary::LoadBlankTexture();
				bool useTexture = 0;
			};
			RoughnessInput m_RoughnessInput;

			struct NormalInput {
				std::shared_ptr<Texture2D> texture = TextureLibrary::LoadBlankTexture();
				bool useTexture = 0;
			};
			NormalInput m_NormalInput;

			struct AOInput {
				std::shared_ptr<Texture2D> texture = TextureLibrary::LoadBlankTexture();
				bool useTexture = 0;
			};
			AOInput m_AOInput;
		};
	}
}