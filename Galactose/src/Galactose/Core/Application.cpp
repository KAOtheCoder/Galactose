#include <iostream>

#include "Application.h"

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

	int Application::exec() {
		while (m_run) {
		}

		return m_exitCode;
	}

	void Application::exit(const int a_exitCode) {
		m_exitCode = a_exitCode;
		m_run = false;
	}

	void Application::init() {
		//ASSERT(s_instance);

		s_instance = this;
	}
}