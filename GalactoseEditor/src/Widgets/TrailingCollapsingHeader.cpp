#include "TrailingCollapsingHeader.h"

#include <iostream>

namespace GalactoseEditor {
	namespace TrailingCollapsingHeader {
		bool draw(const char* a_label, const char* a_buttonLabel, bool& a_clicked, const ImGuiTreeNodeFlags a_flags)
		{
			const bool opened = ImGui::CollapsingHeader(a_label, a_flags 
				| ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap);
			const auto frameHeight = ImGui::GetFrameHeight();
			ImGui::SameLine(ImGui::GetWindowWidth() - (ImGui::GetStyle().WindowPadding.x * 0.5f) - frameHeight);
			ImGui::PushID(a_label);
			a_clicked = ImGui::Button(a_buttonLabel, { frameHeight, frameHeight });
			ImGui::PopID();

			return opened;
		}
	}
}
