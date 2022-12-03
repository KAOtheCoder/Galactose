#include "InputString.h"

#include <imgui.h>

namespace GalactoseEditor {
	std::string InputString::s_text = std::string();

	int InputString::resizeCallback(ImGuiInputTextCallbackData* a_data) {
		if (a_data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			auto str = static_cast<std::string*>(a_data->UserData);
			str->resize(a_data->BufTextLen);
			a_data->Buf = str->data();
		}

		return 0;
	}

	bool InputString::inputText(const char* a_label, const std::string& a_text, int a_size, ImGuiInputTextFlags a_flags) {
		s_text = a_text;
		a_flags |= ImGuiInputTextFlags_CallbackResize;

		return ImGui::InputText(a_label, s_text.data(), a_size < 0 ? s_text.capacity() + 1 : a_size, ImGuiInputTextFlags_CallbackResize, &InputString::resizeCallback, &s_text);
	}
}
