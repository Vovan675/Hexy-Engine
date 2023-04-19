#pragma once
#include "Hexy.h"
#include <filesystem>

namespace Hexy
{
	namespace Editor
	{
		class EditorAssetBrowser
		{
		public:
			EditorAssetBrowser();
			void OnImgui();
		private:
			std::filesystem::path m_ResourcesPath = std::filesystem::current_path() / "EditorResources";
			std::filesystem::path m_CurrentPath = std::filesystem::current_path() / "assets";
			std::shared_ptr<Texture2D> m_directoryTexture;
			std::shared_ptr<Texture2D> m_fileTexture;
		};
	}
}
