#include "EditorAssetBrowser.h"

namespace Hexy
{
	namespace Editor
	{
		extern const std::filesystem::path s_AssetsPath = std::filesystem::current_path() / "assets";
		EditorAssetBrowser::EditorAssetBrowser()
		{
			m_directoryTexture = TextureLibrary::Load((m_ResourcesPath / "AssetsIcons/directory.png").string());
			m_fileTexture = TextureLibrary::Load((m_ResourcesPath / "AssetsIcons/file.png").string());
		}

		void EditorAssetBrowser::OnImgui()
		{
			ImGui::Begin("Asset Browser");

			if (m_CurrentPath != s_AssetsPath)
			{
				if (ImGui::Button("Back"))
				{
					m_CurrentPath = m_CurrentPath.parent_path();
				}
			}

			ImGui::Columns(5, 0, false);
			for (auto& entry : std::filesystem::directory_iterator(m_CurrentPath))
			{
				std::string fileName = entry.path().filename().string();
				ImGui::PushID(fileName.c_str());

				auto& texture = entry.is_directory() ? m_directoryTexture : m_fileTexture;

				if (ImGui::ImageButton((ImTextureID)texture->GetTexture(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0)))
				{
					if (entry.is_directory())
					{
						m_CurrentPath = entry.path();
					}

				}

				ImGui::TextWrapped(fileName.c_str());
				ImGui::NextColumn();
				ImGui::PopID();
			}
			ImGui::Columns(1);
			ImGui::End();
		}
	}
}
