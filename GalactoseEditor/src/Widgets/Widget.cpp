#include "Widget.h"

#include <imgui.h>

namespace GalactoseEditor {
	void Widget::update() {
		if (!m_visible)
			return;

		ImGui::Begin(m_name.c_str(), &m_visible);
		onUpdate();
		ImGui::End();
	}
}
