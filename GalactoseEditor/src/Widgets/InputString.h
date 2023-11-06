#pragma once

#include <imgui.h>
#include <string>

namespace GalactoseEditor {
	class InputString {
	public:
		static std::string text() { return s_text; }
		static bool inputText(const std::string& label, const std::string& text, int size = -1, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);

	private:
		static std::string s_text;
	};
}