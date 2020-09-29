#include "EditorHierarchy.h"
#include "EditorLayer.h"
#include <string>

namespace Hexy 
{
	namespace Editor 
	{
		EditorHierarchy::EditorHierarchy()
		{

		}

		void EditorHierarchy::OnImgui()
		{
			ImGui::Begin("Hierarchy");
			auto entities = SceneManager::GetCurrentScene()->GetEntities();
			static int selected = -1;
			bool entityClicked = false;
			for (auto entityID = entities.rbegin(); entityID < entities.rend(); entityID++) 
			{
				Entity entity = Entity(*entityID, SceneManager::GetCurrentScene());
				std::string name = entity.GetComponent<NameComponent>().Name + "##" + std::to_string((uint32_t)*entityID);

				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
				flags = flags | (EditorLayer::Selection[*entityID] ? ImGuiTreeNodeFlags_Selected : 0);

				ImGui::TreeNodeEx(name.c_str(), flags);

				if (ImGui::BeginPopupContextItem(std::to_string((uint32_t)*entityID).c_str()))
				{
					if (ImGui::MenuItem("Remove"))
					{
						SceneManager::GetCurrentScene()->DestroyEntity(entity);

						EditorLayer::SelectionContext.clear();
						EditorLayer::Selection.clear();

						ImGui::EndPopup();
						continue;
					}
					ImGui::EndPopup();
				}

				bool clicked = ImGui::IsItemClicked();
				entityClicked |= clicked;
				if (clicked) 
				{
					if (Input::IsKeyDown(GLFW_KEY_LEFT_CONTROL)) 
					{
						if (EditorLayer::Selection[*entityID])
						{
							auto it = std::find(EditorLayer::SelectionContext.begin(), EditorLayer::SelectionContext.end(), *entityID);
							if (it != EditorLayer::SelectionContext.end()) 
							{
								EditorLayer::SelectionContext.erase(it);
							}
							EditorLayer::Selection[*entityID] = false;
						}
						else 
						{
							EditorLayer::SelectionContext.push_back(*entityID);
							EditorLayer::Selection[*entityID] = true;
						}
					}
					else
					{
						EditorLayer::SelectionContext.clear();
						EditorLayer::SelectionContext.push_back(*entityID);

						EditorLayer::Selection.clear();
						EditorLayer::Selection[*entityID] = true;
					}
				}
			}

			if (!entityClicked && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) 
			{
				EditorLayer::SelectionContext.clear();
				EditorLayer::Selection.clear();
			}
			
			if (ImGui::BeginPopupContextWindow(nullptr, ImGuiMouseButton_Right, false))
			{
				if (ImGui::MenuItem("Create Entity"))
				{
					SceneManager::GetCurrentScene()->CreateEntity();
				}
				ImGui::EndPopup();
			}
			ImGui::End();
		}
	}
}