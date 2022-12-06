#include "Panel.h"

#include <imgui.h>
#include <iostream>

using namespace Galactose;

namespace GalactoseEditor {
	Panel* Panel::s_focusedPanel = nullptr;

	Panel::Panel(const std::string& a_name) : 
		m_name(a_name),
		m_padding(-1, -1)
	{}

	Panel::~Panel() {
		if (s_focusedPanel == this)
			s_focusedPanel = nullptr;
	}

	Vector2 Panel::padding() const {
		const auto& default_padding = ImGui::GetStyle().WindowPadding;
		return { m_padding.x < 0 ? default_padding.x : m_padding.x, m_padding.y < 0 ? default_padding.y : m_padding.y };
	}

	void Panel::update() {
		if (!m_visible)
			return;

		const bool customPadding = m_padding.x >= 0 || m_padding.y >= 0;
		if (customPadding) {
			const auto& paddingValue = padding();
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { paddingValue.x, paddingValue.y });
		}

		ImGui::Begin(m_name.c_str(), &m_visible);

		if (ImGui::IsWindowFocused()) {
			if (s_focusedPanel != this) {
				if (s_focusedPanel)
					s_focusedPanel->onFocusOut();

				s_focusedPanel = this;
			}
		}
		else if (s_focusedPanel == this) {
			s_focusedPanel->onFocusOut();
			s_focusedPanel = nullptr;
		}

		onUpdate();

		ImGui::End();

		if (customPadding)
			ImGui::PopStyleVar();
	}
}
