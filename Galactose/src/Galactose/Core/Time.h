#pragma once

#include "Galactose/Core/Global.h"

#include <chrono>

namespace Galactose {
	class Time {
	public:
		GT_API void tick();
		GT_API void suspend();
		void reset();

		float deltaTime() const { return m_delta; }
		float elapsedTime() const;

	private:
		float sinceAwakening() const;

		std::chrono::time_point<std::chrono::steady_clock> m_lastAwakening;
		std::chrono::time_point<std::chrono::steady_clock> m_lastTick;
		float m_delta = 0;
		float m_maxDelta = 1;
		float m_elapsedTime = 0;
		bool m_running = false;
	};
}