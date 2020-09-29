#include "EditorMenu.h"
#include "EditorLayer.h"
#include "SceneSerializer.h"
#include <filesystem>

namespace Hexy 
{
	namespace Editor 
	{
		void EditorMenu::OnImgui()
		{
			if (ImGui::BeginMenuBar()) 
			{
				if (ImGui::BeginMenu("Edit")) 
				{
					static bool isBinary = false;
					if (ImGui::MenuItem("Save")) 
					{
						SceneSerializer::Serialize(SceneManager::GetCurrentScene(), "MainScene.hexy", isBinary);
					}
					if (ImGui::MenuItem("Load"))
					{
						SceneManager::LoadScene("MainScene.hexy", isBinary);
					}
					if (ImGui::Checkbox("Binary", &isBinary));
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Build"))
				{
					if (ImGui::MenuItem("Build"))
					{
						std::string buildFolder = FileSystem::OpenFolderDialog();
						if (buildFolder != "")
						{

							namespace fs = std::filesystem;;
							if (!fs::exists(buildFolder))
								std::filesystem::create_directory(buildFolder);

							SceneSerializer::Serialize(SceneManager::GetCurrentScene(), buildFolder + "/MainScene.hexy");

							if (!fs::exists(buildFolder + "/GamePlayer.exe"))
								fs::copy_file("EditorResources/GamePlayer.exe", buildFolder + "/GamePlayer.exe");

							if (!fs::exists(buildFolder + "/assets/"))
								fs::create_directory(buildFolder + "/assets/");

							fs::copy("assets/", "build/assets/", fs::copy_options::recursive | fs::copy_options::skip_existing);
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
		}
	}
}