#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hexy
{
	namespace Editor
	{
		void BeginGrid(int width=2);
		void EndGrid();

		bool Property(const std::string label, const std::string& value);
		bool Property(const std::string label, std::string& value, bool error = false);

		bool Property(const std::string label, glm::vec3& vec);
	}
}