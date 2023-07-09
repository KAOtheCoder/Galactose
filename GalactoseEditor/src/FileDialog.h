#pragma once

#include <string>
#include <vector>

namespace GalactoseEditor {
	namespace FileDialog {
		bool init();
		void quit();

		struct Filter {
			std::string name;
			std::string spec;
		};

		std::string open(const std::vector<Filter>& filters, const std::string& defaultPath = "");
		std::vector<std::string> openMultiple(const std::vector<Filter>& filters, const std::string& defaultPath = "");
		std::string save(const std::vector<Filter>& filters, const std::string& defaultPath = "", const std::string& defaultName = "");
	}
}
