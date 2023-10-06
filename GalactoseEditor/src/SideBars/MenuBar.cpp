#include "MenuBar.h"

#include <Galactose/Core/Window.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	std::string MenuBar::Shortcut::toString() const {
		std::string str;

		if (modifiers != KeyEvent::None) {
			if (modifiers & KeyEvent::Control)
				str += "Ctrl+";

			if (modifiers & KeyEvent::Shift)
				str += "Shift+";

			if (modifiers & KeyEvent::Alt)
				str += "Alt+";

			if (modifiers & KeyEvent::Super)
				str += "Super+";
		}

		if (key != KeyEvent::KeyUnknown)
			str += KeyEvent::toString(key);

		return str;
	}

	float MenuBar::size() const {
		return ImGui::GetFrameHeight();
	}

	int MenuBar::additionalWindowFlags() const { return ImGuiWindowFlags_MenuBar; }

	void MenuBar::update() {
		if (ImGui::BeginMenuBar()) {
			for (const auto& menu : menus) {
				if (ImGui::BeginMenu(menu.label.c_str())) {
					for (const auto& menuItem : menu.menuItems) {
						if (menuItem.label.empty())
							ImGui::Separator();
						else if (ImGui::MenuItem(menuItem.label.c_str(), menuItem.shortcut.toString().c_str()))
							menuItem.action();
					}

					ImGui::EndMenu();
				}
			}

			ImGui::EndMenuBar();
		}
	}

	void MenuBar::onEvent(const std::shared_ptr<Event>& a_event) {
		if (a_event->type() == Event::KeyPress) {
			auto keyPressEvent = static_cast<KeyPressEvent*>(a_event.get());
			
			if (keyPressEvent->isRepeat())
				return;

			const auto modifiers = keyPressEvent->window()->keyboardModifiers();
			const auto key = keyPressEvent->key();

			if (key == KeyEvent::KeyUnknown)
				return;

			for (const auto& menu : menus) {
				for (const auto& menuItem : menu.menuItems) {
					if (menuItem.shortcut.modifiers == modifiers && menuItem.shortcut.key == key) {
						menuItem.action();
						a_event->setHandled();
						m_pressedKeys.clear();
						return;
					}
				}
			}
		}
	}
}
