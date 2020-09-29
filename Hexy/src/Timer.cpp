#include "pch.h"
#include "Timer.h"

namespace Hexy 
{
	Timer::Timer()
	{
		m_startTime = std::chrono::high_resolution_clock::now();
	}

	Timer::~Timer()
	{
		if (!m_stopped) 
		{
			m_stopped = true;
			m_endTime = std::chrono::high_resolution_clock::now();
		}
	}

	void Timer::Stop()
	{
		if (!m_stopped) 
		{
			m_stopped = true;
			m_endTime = std::chrono::high_resolution_clock::now();
		}
	}

	double Timer::GetTime()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(m_endTime - m_startTime).count() / 1000000.0f;
	}
}