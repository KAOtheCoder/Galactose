#pragma once

#include <string>

namespace GalactoseEditor {
	class InputString {
	public:
		static bool inputText(const char* id, std::string& text, size_t size = std::string::npos);
	};
}