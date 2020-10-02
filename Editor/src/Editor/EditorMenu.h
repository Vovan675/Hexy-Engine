#pragma once
#include "Hexy.h"

namespace Hexy
{
	namespace Editor
	{
		class EditorMenu 
		{
		public:
			EditorMenu() {}
			void OnImgui();
			void BuildProject(const std::string& path);
		};
	}
}