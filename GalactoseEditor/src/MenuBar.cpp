#include "MenuBar.h"

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	std::string MenuBar::MenuItem::shortcutAsString() const {
		std::string str;
		const auto shortcutSize = shortcut.size();

		for (int i = 0; i < shortcutSize; ++i) {
			if (i > 0)
				str += "+";

			str += KeyEvent::toString(shortcut[i]);
		}

		return str;
	}

	void MenuBar::draw() {
		if (ImGui::BeginMainMenuBar()) {
			for (const auto& menu : menus) {
				if (ImGui::BeginMenu(menu.label.c_str())) {
					for (const auto& menuItem : menu.menuItems) {
						if (menuItem.label.empty())
							ImGui::Separator();
						else if (ImGui::MenuItem(menuItem.label.c_str(), menuItem.shortcutAsString().c_str()))
							menuItem.action();
					}

					ImGui::EndMenu();
				}
			}

			ImGui::EndMainMenuBar();
		}
	}

	void MenuBar::onEvent(const std::shared_ptr<Event>& a_event) {
		if (a_event->type() == Event::KeyPress) {
			auto keyPressEvent = static_cast<KeyPressEvent*>(a_event.get());
			
			if (keyPressEvent->isRepeat())
				return;

			m_pressedKeys.insert(keyPressEvent->key());

			for (const auto& menu : menus) {
				for (const auto& menuItem : menu.menuItems) {
					if (!menuItem.shortcut.empty()) {
						bool shortcutMatch = true;

						for (const auto key : menuItem.shortcut) {
							if (m_pressedKeys.find(key) == m_pressedKeys.end()) {
								shortcutMatch = false;
								break;
							}
						}

						if (shortcutMatch) {
							menuItem.action();
							a_event->setHandled();
							return;
						}
					}
				}
			}
		}
		else if (a_event->type() == Event::KeyRelease) {
			m_pressedKeys.erase(static_cast<KeyReleaseEvent*>(a_event.get())->key());
		}
	}
}
