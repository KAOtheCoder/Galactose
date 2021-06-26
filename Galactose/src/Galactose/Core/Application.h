#pragma once
#include <vector>
#include <string>
#include <algorithm>

namespace Galactose {
	class Application
	{
	public:
		static Application* instance() { return s_instance; }

		Application(const std::string& a_name, int a_argc, char** a_argv);
		Application(const std::string& a_name, const std::vector<std::string>& a_args);
		virtual ~Application() = default;

		std::string name() const { return m_name; }
		std::vector<std::string> arguments() const { return m_args; }

		int exec();
		void exit(const int a_exitCode = 0);

	private:
		void init();

		inline static Application* s_instance = nullptr;

		std::string m_name;
		std::vector<std::string> m_args;
		int m_exitCode = 0;
		bool m_run = true;
		bool m_quitOnLastWindowClosed = true;
	};
}
