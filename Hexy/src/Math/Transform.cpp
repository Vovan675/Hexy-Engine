#include "pch.h"
#include "Transform.h"
#include "glm/gtx/matrix_decompose.hpp"

namespace Hexy 
{
	namespace Math 
	{
		std::tuple<glm::vec3, glm::quat, glm::vec3> DecomposeTransform(const glm::mat4& transform)
		{
			glm::vec3 translation, scale, skew;
			glm::quat orientation;
			glm::vec4 perspective;

			glm::decompose(transform, scale, orientation, translation, skew, perspective);
			return { translation, orientation, scale };
		}
	}
}
