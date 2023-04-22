#include "Time.h"

namespace Galactose {
	void Time::tick() {
		if (!m_running) {
			m_lastAwakening = std::chrono::steady_clock::now();
			m_lastTick = m_lastAwakening;
			m_running = true;
		}
		else {
			const auto previousTick = m_lastTick;
			m_lastTick = std::chrono::steady_clock::now();
			m_delta = std::chrono::duration<float>(m_lastTick - previousTick).count();
		}
	}

	float Time::sinceAwakening() const {
		return std::chrono::duration<float>(m_lastTick - m_lastAwakening).count();
	}

	void Time::suspend() {
		m_elapsedTime += sinceAwakening();
		m_running = false;
	}

	void Time::reset() {
		m_lastAwakening = std::chrono::time_point<std::chrono::steady_clock>();
		m_lastTick = m_lastAwakening;
		m_delta = 0;
		m_elapsedTime = 0;
	}

	float Time::elapsedTime() const {
		return m_elapsedTime + sinceAwakening();
	}
}
