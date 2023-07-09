#include "Application.h"
#include "Window.h"
#include "Events/Event.h"

namespace Galactose {
	Application::Application(const std::string& a_name, int a_argc, char** a_argv)
		: m_name(a_name)
	{
		for (int i = 0; i < a_argc; ++i)
			m_args.emplace_back(a_argv[i]);

		init();
	}

	Application::Application(const std::string& a_name, const std::vector<std::string>& a_args)
		: m_name(a_name),
		  m_args(a_args)
	{
		init();
	}

	Application* Application::instance() { return s_instance; }

	int Application::exec() {
		m_run = true;

		do {
			for (const auto& event : m_eventQueue)
				event->process();

			m_eventQueue.clear();

			for (auto& window : Window::s_windows)
				window->update();

			if (m_run && m_quitOnLastWindowClosed)
				m_run = !Window::areAllWindowsClosed();
		} while (m_run);

		m_eventQueue.clear();

		return m_exitCode;
	}

	void Application::exit(const int a_exitCode) {
		m_exitCode = a_exitCode;
		m_run = false;
	}

	void Application::init() {
		GT_ASSERT(!s_instance, "Only one application can be exist at a time.");

		s_instance = this;
	}
}