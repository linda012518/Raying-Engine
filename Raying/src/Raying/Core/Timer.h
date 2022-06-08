#pragma once

#include <chrono>

namespace Raying {

	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		void Reset()
		{
			_start = std::chrono::high_resolution_clock::now();
		}

		float Elapsed()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - _start).count() * 0.01f * 0.01f * 0.01f;
		}

		float ElapsedMillis()
		{
			return Elapsed() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> _start;

	};
}
