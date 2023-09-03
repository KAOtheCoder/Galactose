#pragma once

#include "Galactose/Core/Events/KeyEvent.h"

#include <functional>
#include <set>

namespace GalactoseEditor {
	class MenuBar {
	public:
		struct Shortcut {
			std::string toString() const;

			int modifiers = Galactose::KeyEvent::None;
			Galactose::KeyEvent::Key key = Galactose::KeyEvent::KeyUnknown;
		};

		struct MenuItem {
			std::string label;
			Shortcut shortcut;
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
