#pragma once
#include <tuple>

namespace Hexy 
{
	namespace Math 
	{
		std::tuple<glm::vec3, glm::quat, glm::vec3> DecomposeTransform(const glm::mat4& transform);
	}
}