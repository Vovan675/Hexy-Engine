#include "pch.h"
#include "FileSystem.h"
#include <ShObjIdl_core.h>

namespace Hexy 
{
	void FileSystem::ReadStringStream(const std::string& path, std::stringstream& stream, bool binary)
	{
		std::ifstream file(path);
		if (file.is_open())
		{
			stream << file.rdbuf();
			file.close();
		}
	}

	void FileSystem::WriteStringStream(const std::string& path, std::stringstream& stream, bool binary)
	{
		int flags = binary ? std::ios::binary : 0;
		std::ofstream file(path, flags);
		if (file.is_open())
		{
			file << stream.rdbuf();
			file.close();
		}
	}

	std::string FileSystem::OpenFileDialog()
	{
		std::filesystem::path currentPath = std::filesystem::current_path();//Needs to be saved, because winapi will change it after open file dialog
		IFileOpenDialog* fileOpenDialog(NULL);

		HRESULT result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&fileOpenDialog));
		if (SUCCEEDED(result))
		{
			if (SUCCEEDED(fileOpenDialog->Show(nullptr)))
			{
				IShellItem* pItem;
				fileOpenDialog->GetResult(&pItem);
				PWSTR pszFilePath;
				pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
				char fileName[500];
				wcstombs(fileName, pszFilePath, 500);
				std::filesystem::current_path(currentPath);
				return fileName;
			}
		}
		return "";
	}

	std::string FileSystem::OpenFolderDialog()
	{
		std::filesystem::path currentPath = std::filesystem::current_path();//Needs to be saved, because winapi will change it after open file dialog
		IFileOpenDialog* fileOpenDialog(NULL);

		HRESULT result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&fileOpenDialog));
		if (SUCCEEDED(result))
		{
			fileOpenDialog->SetOptions(FOS_PICKFOLDERS);
			if (SUCCEEDED(fileOpenDialog->Show(nullptr)))
			{
				IShellItem* pItem;
				fileOpenDialog->GetResult(&pItem);
				PWSTR pszFilePath;
				pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
				char fileName[500];
				wcstombs(fileName, pszFilePath, 2500);
				std::filesystem::current_path(currentPath);
				return fileName;
			}
		}
		return "";
	}
}