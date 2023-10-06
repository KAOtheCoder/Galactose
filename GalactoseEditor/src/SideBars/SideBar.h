#pragma once

#include <vector>
#include <string>
#include <memory>

#include <imgui.h>

namespace Galactose {
	class Event;
}

namespace GalactoseEditor {
	class SideBarItem;

	class SideBar {
	public:
		SideBar(const std::string& name, ImGuiViewport* viewport, const ImGuiDir_ direction);

		void addSideBar(const std::shared_ptr<SideBarItem>& item);

		void update();
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event);

	private:
		std::string m_name;
		ImGuiViewport* m_viewport = nullptr;
		ImGuiDir_ m_direction;
		ImGuiWindowFlags m_windowFlags = 0;
		std::vector<std::shared_ptr<SideBarItem>> m_items;
	};
}
