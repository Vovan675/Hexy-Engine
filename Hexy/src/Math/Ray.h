#pragma once
#include "glm/glm.hpp"
#include "AABB.h"

namespace Hexy
{
	struct Ray 
	{
		glm::vec3 pos, dir;
		Ray(glm::vec3 pos, glm::vec3 dir) : pos(pos), dir(dir) {}
		Ray() : pos(0), dir(0) {}

		bool IntersectsAABB(AABB& aabb, float& t)
		{
			// r.dir is unit direction vector of ray
			glm::vec3 dirfrac = glm::vec3(
				dir.x ? 1.0f / dir.x : std::numeric_limits<float>::max(), 
				dir.y ? 1.0f / dir.y : std::numeric_limits<float>::max(),
				dir.z ? 1.0f / dir.z : std::numeric_limits<float>::max()
			);

			// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
			// r.org is origin of ray
			float t1 = (aabb.min.x - pos.x) * dirfrac.x;
			float t2 = (aabb.max.x - pos.x) * dirfrac.x;
			float t3 = (aabb.min.y - pos.y) * dirfrac.y;
			float t4 = (aabb.max.y - pos.y) * dirfrac.y;
			float t5 = (aabb.min.z - pos.z) * dirfrac.z;
			float t6 = (aabb.max.z - pos.z) * dirfrac.z;

			float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
			float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

			// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
			if (tmax < 0)
			{
				t = tmax;
				return false;
			}

			// if tmin > tmax, ray doesn't intersect AABB
			if (tmin > tmax)
			{
				t = tmax;
				return false;
			}

			t = tmin;
			return true;
		}
	};
}