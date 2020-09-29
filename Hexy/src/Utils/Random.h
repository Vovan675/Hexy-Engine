#pragma once
#include <random>

namespace Hexy 
{
	class Random
	{
	private:
		static std::mt19937_64 m_random;

		Random() = delete;
		~Random() = delete;
	public:
		static int IntRange(int min, int max);
		static float FloatRange(float min, float max);
	};
}