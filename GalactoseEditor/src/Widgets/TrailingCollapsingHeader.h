#pragma once

#include <imgui.h>

namespace GalactoseEditor {
	namespace TrailingCollapsingHeader {
		bool draw(const char* label, const char* buttonLabel, bool& clicked, const ImGuiTreeNodeFlags flags = 0);
	}
}
