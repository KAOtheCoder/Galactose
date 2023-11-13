#include "InputString.h"

#include <imgui.h>

namespace GalactoseEditor {
	int resizeCallback(ImGuiInputTextCallbackData* a_data) {
		if (a_data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			auto str = static_cast<std::string*>(a_data->UserData);
			str->resize(a_data->BufTextLen);
			a_data->Buf = str->data();
		}

		return 0;
	}

	bool InputString::inputText(const char* a_id, std::string& a_text, size_t a_size) {
		ImGui::PushItemWidth(-std::numeric_limits<float>().min());
		const auto& id = std::string("##") + a_id;
		const bool changed = ImGui::InputText(id.c_str(), a_text.data(), a_size == std::string::npos ? a_text.capacity() + 1 : a_size, ImGuiInputTextFlags_CallbackResize, &resizeCallback, &a_text);
		ImGui::PopItemWidth();

		return changed;
	}
}
