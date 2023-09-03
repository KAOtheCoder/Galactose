#include "KeyEvent.h"

namespace Galactose {
	std::string KeyEvent::toString(const KeyEvent::Key a_key) {
		if (a_key >= KeyEvent::KeySpace && a_key <= KeyEvent::KeyGraveAccent)
			return std::string(1, char(a_key));

		if (a_key >= KeyEvent::KeyF1 && a_key <= KeyEvent::KeyF25)
			return "F" + std::to_string(a_key - KeyEvent::KeyF1 + 1);

		switch (a_key) {
		case KeyEvent::KeyLeftShift: return "Shift";
		case KeyEvent::KeyLeftControl: return "Ctrl";
		case KeyEvent::KeyLeftAlt: return "Alt";
		default:
			GT_ASSERT(false, "Unknown keycode: " + std::to_string(a_key));
		}
		return "";
	}

	KeyEvent::Modifier KeyEvent::toModifier(const KeyEvent::Key a_key, KeyEvent::Key* otherKey) {
		if (otherKey) {
			*otherKey = KeyEvent::KeyUnknown;

			if (a_key >= KeyEvent::KeyLeftShift && a_key <= KeyEvent::KeyLeftSuper)
				*otherKey = static_cast<KeyEvent::Key>(a_key + 4);
			else if (a_key >= KeyEvent::KeyRightShift && a_key <= KeyEvent::KeyRightSuper)
				*otherKey = static_cast<KeyEvent::Key>(a_key - 4);
		}

		switch (a_key) {
		case KeyEvent::KeyLeftShift:
		case KeyEvent::KeyRightShift: return KeyEvent::Shift;
		case KeyEvent::KeyLeftControl:
		case KeyEvent::KeyRightControl: return KeyEvent::Control;
		case KeyEvent::KeyLeftAlt:
		case KeyEvent::KeyRightAlt: return KeyEvent::Alt;
		case KeyEvent::KeyLeftSuper:
		case KeyEvent::KeyRightSuper: return KeyEvent::Super;
		default: return KeyEvent::None;
		}
		
		return KeyEvent::None;
	}

	std::string KeyEvent::toString() const {
		std::string keyName = m_key >= ' ' && m_key <= '`' ? std::string(1, char(m_key)) : std::to_string(m_key);
		return name() + ": " + keyName;
	}
}
