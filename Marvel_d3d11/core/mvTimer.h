#pragma once
#include <chrono>

namespace Marvel {

	class mvTimer
	{

	public:

		mvTimer();

		float mark();
		float peek() const;
	
	private:

		std::chrono::steady_clock::time_point m_last;
	};
}