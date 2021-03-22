#include "mvTimer.h"

namespace Marvel {

	using namespace std::chrono;

	mvTimer::mvTimer()
	{
		m_last = steady_clock::now();
		m_start = steady_clock::now();
	}

	float mvTimer::mark()
	{
		const auto old = m_last;
		m_last = steady_clock::now();
		const duration<float> frameTime = m_last - old;
		return frameTime.count();
	}

	float mvTimer::peek() const
	{
		return duration<float>(steady_clock::now() - m_last).count();
	}

	float mvTimer::now() const
	{
		const duration<float> totalTime = steady_clock::now() - m_start;
		return totalTime.count();
	}

}