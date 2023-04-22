#pragma once

#include <chrono>

namespace Galactose {
	class Time {
	public:
		void tick();
		void suspend();
		void reset();

		float deltaTime() const { return m_delta; }
		float elapsedTime() const;

	private:
		float sinceAwakening() const;

		std::chrono::time_point<std::chrono::steady_clock> m_lastAwakening;
		std::chrono::time_point<std::chrono::steady_clock> m_lastTick;
		float m_delta = 0;
		float m_elapsedTime = 0;
		bool m_running = false;
	};
}