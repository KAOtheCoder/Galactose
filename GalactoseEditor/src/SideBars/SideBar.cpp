#include "SideBar.h"
#include "SideBarItem.h"

#include <Galactose/Core/Events/Event.h>

#include <imgui_internal.h>

using namespace Galactose;

namespace GalactoseEditor {
	SideBar::SideBar(const std::string& a_name, ImGuiViewport* a_viewport, const ImGuiDir_ a_direction) :
		m_name(a_name),
		m_viewport(a_viewport),
		m_direction(a_direction),
		m_windowFlags(ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings)
	{}

	void SideBar::addSideBar(const std::shared_ptr<SideBarItem>& a_item) {
		m_windowFlags |= a_item->additionalWindowFlags();
		m_items.push_back(a_item);
	}

	void SideBar::update() {
		float size = 0;

		for (const auto& item : m_items)
			size += item->size();

		if (ImGui::BeginViewportSideBar(m_name.c_str(), m_viewport, m_direction, size, m_windowFlags)) {
			for (const auto& item : m_items)
				item->update();

			ImGui::End();
		}
	}

	void SideBar::onEvent(const std::shared_ptr<Event>& a_event) {
		for (const auto& item : m_items) {
			if (a_event->isHandled())
				return;

			item->onEvent(a_event);
		}
	}
}
