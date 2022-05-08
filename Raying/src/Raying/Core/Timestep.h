#pragma once

#include "Base.h"

namespace Raying {

	class Raying_API Timestep
	{
	public:
		Timestep(float time)
			: _time(time)
		{

		}

		operator float() const { return _time; }

		float GetSeconds() const { return _time; }
		float GetMilliseconds() const { return _time * 1000.0f; }
	private:
		float _time;

	};
}