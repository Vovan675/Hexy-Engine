#pragma once
namespace Hexy
{
	class FileSystem
	{
	public:
		static void ReadStringStream(const std::string& path, std::stringstream& stream, bool binary = false);
		static void WriteStringStream(const std::string& path, std::stringstream& stream, bool binary = false);

		//In future for multiOS support it must be refactored
		static std::string SaveFileDialog();
		static std::string OpenFileDialog();
		static std::string OpenFolderDialog();
	};
}