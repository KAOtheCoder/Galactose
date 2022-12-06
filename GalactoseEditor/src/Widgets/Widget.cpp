#include "Widget.h"

#include <imgui.h>
#include <iostream>

using namespace Galactose;

namespace GalactoseEditor {
	Widget* Widget::s_focusedWidget = nullptr;

	Widget::Widget(const std::string& a_name) : 
		m_name(a_name),
		m_padding(-1, -1)
	{}

	Widget::~Widget() {
		if (s_focusedWidget == this)
			s_focusedWidget = nullptr;
	}

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

		if (ImGui::IsWindowFocused()) {
			if (s_focusedWidget != this) {
				if (s_focusedWidget)
					s_focusedWidget->onFocusOut();

				s_focusedWidget = this;
			}
		}
		else if (s_focusedWidget == this) {
			s_focusedWidget->onFocusOut();
			s_focusedWidget = nullptr;
		}

		onUpdate();

		ImGui::End();

		if (customPadding)
			ImGui::PopStyleVar();
	}
}
