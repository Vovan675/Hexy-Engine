#pragma once

namespace Hexy
{
	class Timer 
	{
	private:
		std::chrono::steady_clock::time_point m_startTime;
		std::chrono::steady_clock::time_point m_endTime;
		int m_deltaTime;
		bool m_stopped = false;
	public:
		Timer();
		~Timer();

		void Stop();
		double GetTime();
	};
}