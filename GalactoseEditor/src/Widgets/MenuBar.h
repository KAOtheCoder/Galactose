#pragma once

#include "Core/Events/KeyEvent.h"

#include <functional>
#include <set>

namespace GalactoseEditor {
	class MenuBar {
	public:
		struct MenuItem {
			std::string shortcutAsString() const;

			std::string label;
			std::vector<Galactose::KeyEvent::Key> shortcut;
			std::function<void()> action;
		};

		struct Menu {
			std::string label;
			std::vector<MenuItem> menuItems;
		};

		void update();
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event);

		float height() const;
		std::vector<Menu> menus;

	private:
		std::set<Galactose::KeyEvent::Key> m_pressedKeys;
	};
}
