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
			static bool ShowBuildWindow = false;
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
						EditorLayer::Selection.clear();
						EditorLayer::SelectionContext.clear();
						SceneManager::LoadScene("MainScene.hexy", isBinary);
					}
					if (ImGui::Checkbox("Binary", &isBinary));
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Build"))
				{
					ImGui::MenuItem("Build", 0, &ShowBuildWindow);
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			if (ShowBuildWindow)
			{
				ImGui::Begin("Build Settings");
				if (ImGui::Button("Build"))
				{
					std::string buildFolder = FileSystem::OpenFolderDialog();
					if (buildFolder != "")
					{
						BuildProject(buildFolder);
					}
				}
				if (ImGui::Button("Build & Run"))
				{
					std::string buildFolder = FileSystem::OpenFolderDialog();
					if (buildFolder != "")
					{
						BuildProject(buildFolder);
						STARTUPINFOA si = { 0 };
						PROCESS_INFORMATION pi = { 0 };
						CreateProcessA((buildFolder + "/GamePlayer.exe").c_str(), NULL, NULL, NULL, FALSE, NULL, NULL, buildFolder.c_str(), &si, &pi);
					}
				}
				ImGui::End();
			}
		}

		void EditorMenu::BuildProject(const std::string& path)
		{
			namespace fs = std::filesystem;;
			if (!fs::exists(path))
				std::filesystem::create_directory(path);

			SceneSerializer::Serialize(SceneManager::GetCurrentScene(), path + "/MainScene.hexy");

			if (!fs::exists(path + "/GamePlayer.exe"))
				fs::copy_file("EditorResources/GamePlayer.exe", path + "/GamePlayer.exe");

			if (!fs::exists(path + "/assets/"))
				fs::create_directory(path + "/assets/");

			fs::copy("assets/", path + "/assets/", fs::copy_options::recursive | fs::copy_options::skip_existing);
		}
	}
}