#include "pch.h"
#include "Random.h"

namespace Hexy
{
	std::mt19937_64 Random::m_random = std::mt19937_64(1);

	int Random::IntRange(int min, int max)
	{
		return std::uniform_int_distribution<int>(min, max)(m_random);
	}

	float Random::FloatRange(float min, float max)
	{
		return std::uniform_real_distribution<float>(min, max)(m_random);
	}
}