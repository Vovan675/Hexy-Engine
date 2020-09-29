#include "EditorInspector.h"
#include "EditorLayer.h"
#include "EditorUtils.h"

namespace Hexy
{
	namespace Editor
	{
		template<typename T, typename Func>
		static void DrawComponent(const std::string& label, Entity& entity, Func func)
		{
			auto component = entity.TryGetComponent<T>();
			if (component)
			{
				if (ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(T).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, label.c_str()))
				{
					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Remove"))
							int i = 0;
						ImGui::EndPopup();
					}
					
					func(*component);
					ImGui::TreePop();
				}
				ImGui::Separator();
			}
		}

		void EditorInspector::OnImgui()
		{
			ImGui::Begin("Inspector");
			
			if (!EditorLayer::SelectionContext.empty())
			{
				Entity entity = Entity(*EditorLayer::SelectionContext.begin(), SceneManager::GetCurrentScene());
				auto& tagComponent = entity.GetComponent<NameComponent>();
				
				Property("", tagComponent.Name, 0);
				ImGui::Separator();

				auto& transformComponent = entity.GetComponent<TransformComponent>();
				if (ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(TransformComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
				{
					BeginGrid(2);
					bool changed = Property("Position", transformComponent.position);
					changed |= Property("Rotation", transformComponent.rotation);
					changed |= Property("Scale", transformComponent.scale);
					if (changed) 
					{
						transformComponent.UpdateMatrix();
					}
					EndGrid();
					ImGui::TreePop();
				}


				DrawComponent<MeshComponent>("Mesh", entity, [](MeshComponent& mesh) {
					BeginGrid(3);
					Property("Path", mesh.mesh->GetPath());
					if (ImGui::Button("...##MeshPath")) 
					{
						std::string path = FileSystem::OpenFileDialog();
						if (path != "")
						{
							mesh.mesh->Load(path);
						}
					}
					EndGrid();
				});

				if (ImGui::Button("Add Component"))
					ImGui::OpenPopup("add_component_popup");

				if (ImGui::BeginPopup("add_component_popup"))
				{
					if (ImGui::MenuItem("Mesh Component"))
						entity.AddComponent<MeshComponent>().mesh = Mesh::Create("assets/models/sphere.fbx");
					if (ImGui::MenuItem("Sprite Component"))
						entity.AddComponent<SpriteComponent>();
					if (ImGui::MenuItem("Camera Component"))
						entity.AddComponent<CameraComponent>();

					ImGui::EndPopup();
				}
			}
			ImGui::End();
		}
	}
}