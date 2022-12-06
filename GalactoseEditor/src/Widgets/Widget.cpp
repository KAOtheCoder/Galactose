#include "Widget.h"

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	Widget::Widget(const std::string& a_name) : 
		m_name(a_name),
		m_padding(-1, -1)
	{}

	Vector2 Widget::padding() const {
		const auto& default_padding = ImGui::GetStyle().WindowPadding;
		return { m_padding.x < 0 ? default_padding.x : m_padding.x, m_padding.y < 0 ? default_padding.y : m_padding.y };
	}

	void Widget::update() {
		if (!m_visible)
			return;

		const bool customPadding = m_padding.x >= 0 || m_padding.y >= 0;
		if (customPadding) {
			const auto& paddingValue = padding();
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { paddingValue.x, paddingValue.y });
		}

		ImGui::Begin(m_name.c_str(), &m_visible);
		onUpdate();
		ImGui::End();

		if (customPadding)
			ImGui::PopStyleVar();
	}
}
