#pragma once
#include "glm/glm.hpp"

namespace Hexy
{
	struct AABB 
	{
		glm::vec3 min, max;
		AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max) {}
		AABB() : min(std::numeric_limits<float>::max()), max(std::numeric_limits<float>::min()) {}

		AABB operator *(const glm::mat4& mat) const
		{
			AABB aabb;
			aabb.min = glm::vec3(mat * glm::vec4(min, 1));
			aabb.max = glm::vec3(mat * glm::vec4(max, 1));

			if (aabb.min.x > aabb.max.x) std::swap<float>(aabb.min.x, aabb.max.x);
			if (aabb.min.y > aabb.max.y) std::swap<float>(aabb.min.y, aabb.max.y);
			if (aabb.min.z > aabb.max.z) std::swap<float>(aabb.min.z, aabb.max.z);
			return aabb;
		}

		void operator &=(const AABB& other) 
		{
			min.x = other.min.x > min.x ? other.min.x : min.x;
			min.y = other.min.y > min.y ? other.min.y : min.y;
			min.z = other.min.z > min.z ? other.min.z : min.z;

			max.x = other.max.x < max.x ? other.max.x : max.x;
			max.y = other.max.y < max.y ? other.max.y : max.y;
			max.z = other.max.z < max.z ? other.max.z : max.z;
		}

		void operator |=(const AABB& other)
		{
			min.x = other.min.x < min.x ? other.min.x : min.x;
			min.y = other.min.y < min.y ? other.min.y : min.y;
			min.z = other.min.z < min.z ? other.min.z : min.z;

			max.x = other.max.x > max.x ? other.max.x : max.x;
			max.y = other.max.y > max.y ? other.max.y : max.y;
			max.z = other.max.z > max.z ? other.max.z : max.z;
		}

		bool operator ==(const AABB& other) const
		{
			return min == other.max && max == other.max;
		}
	};
}