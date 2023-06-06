#pragma once

#include "InputEvent.h"

#include <string>

namespace Galactose {
	class GT_API KeyEvent : public InputEvent {
	public:
		enum Key {
			// Printable keys
			KeySpace = ' ',
			KeyApostrophe = '\'',
			KeyComma = ',',
			KeyMinus = '-',
			KeyPeriod = '.',
			KeySlash = '/',
			Key0 = '0',
			Key1 = '1',
			Key2 = '2',
			Key3 = '3',
			Key4 = '4',
			Key5 = '5',
			Key6 = '6',
			Key7 = '7',
			Key8 = '8',
			Key9 = '9',
			KeySemicolon = ';',
			KeyEqual = '=',
			KeyA = 'A',
			KeyB = 'B',
			KeyC = 'C',
			KeyD = 'D',
			KeyE = 'E',
			KeyF = 'F',
			KeyG = 'G',
			KeyH = 'H',
			KeyI = 'I',
			KeyJ = 'J',
			KeyK = 'K',
			KeyL = 'L',
			KeyM = 'M',
			KeyN = 'N',
			KeyO = 'O',
			KeyP = 'P',
			KeyQ = 'Q',
			KeyR = 'R',
			KeyS = 'S',
			KeyT = 'T',
			KeyU = 'U',
			KeyV = 'V',
			KeyW = 'W',
			KeyX = 'X',
			KeyY = 'Y',
			KeyZ = 'Z',
			KeyLeftBracket = '[',
			KeyBackslash = '\\',
			KeyRightBracket = ']',
			KeyGraveAccent = '`',
			KeyWorld1 = 161, // GLFW_KEY_WORLD_1 // non-US #1
			KeyWorld2 = 162, // GLFW_KEY_WORLD_2 // non-US #2

			// Function keys
			KeyEscape = 256, // GLFW_KEY_ESCAPE
			KeyEnter = 257, // GLFW_KEY_ENTER
			KeyTab = 258, // GLFW_KEY_TAB
			KeyBackspace = 259, // GLFW_KEY_BACKSPACE
			KeyInsert = 260, // GLFW_KEY_INSERT
			KeyDelete = 261, // GLFW_KEY_DELETE
			KeyRight = 262, // GLFW_KEY_RIGHT
			KeyLeft = 263, // GLFW_KEY_LEFT
			KeyDown = 264, // GLFW_KEY_DOWN
			KeyUp = 265, // GLFW_KEY_UP
			KeyPageUp = 266, // GLFW_KEY_PAGE_UP
			KeyPageDown = 267, // GLFW_KEY_PAGE_DOWN
			KeyHome = 268, // GLFW_KEY_HOME
			KeyEnd = 269, // GLFW_KEY_END
			KeyCapsLock = 280, // GLFW_KEY_CAPS_LOCK
			KeyScrollLock = 281, // GLFW_KEY_SCROLL_LOCK
			KeyNumLock = 282, // GLFW_KEY_NUM_LOCK
			KeyPrintScreen = 283, // GLFW_KEY_PRINT_SCREEN
			KeyPause = 284, // GLFW_KEY_PAUSE
			KeyF1 = 290, // GLFW_KEY_F1
			KeyF2 = 291, // GLFW_KEY_F2
			KeyF3 = 292, // GLFW_KEY_F3
			KeyF4 = 293, // GLFW_KEY_F4
			KeyF5 = 294, // GLFW_KEY_F5
			KeyF6 = 295, // GLFW_KEY_F6
			KeyF7 = 296, // GLFW_KEY_F7
			KeyF8 = 297, // GLFW_KEY_F8
			KeyF9 = 298, // GLFW_KEY_F9
			KeyF10 = 299, // GLFW_KEY_F10
			KeyF11 = 300, // GLFW_KEY_F11
			KeyF12 = 301, // GLFW_KEY_F12
			KeyF13 = 302, // GLFW_KEY_F13
			KeyF14 = 303, // GLFW_KEY_F14
			KeyF15 = 304, // GLFW_KEY_F15
			KeyF16 = 305, // GLFW_KEY_F16
			KeyF17 = 306, // GLFW_KEY_F17
			KeyF18 = 307, // GLFW_KEY_F18
			KeyF19 = 308, // GLFW_KEY_F19
			KeyF20 = 309, // GLFW_KEY_F20
			KeyF21 = 310, // GLFW_KEY_F21
			KeyF22 = 311, // GLFW_KEY_F22
			KeyF23 = 312, // GLFW_KEY_F23
			KeyF24 = 313, // GLFW_KEY_F24
			KeyF25 = 314, // GLFW_KEY_F25
			KeyPad0 = 320, // GLFW_KEY_KP_0
			KeyPad1 = 321, // GLFW_KEY_KP_1
			KeyPad2 = 322, // GLFW_KEY_KP_2
			KeyPad3 = 323, // GLFW_KEY_KP_3
			KeyPad4 = 324, // GLFW_KEY_KP_4
			KeyPad5 = 325, // GLFW_KEY_KP_5
			KeyPad6 = 326, // GLFW_KEY_KP_6
			KeyPad7 = 327, // GLFW_KEY_KP_7
			KeyPad8 = 328, // GLFW_KEY_KP_8
			KeyPad9 = 329, // GLFW_KEY_KP_9
			KeyPadDecimal = 330, // GLFW_KEY_KP_DECIMAL
			KeyPadDivide = 331, // GLFW_KEY_KP_DIVIDE
			KeyPadMultiply = 332, // GLFW_KEY_KP_MULTIPLY
			KeyPadSubtract = 333, // GLFW_KEY_KP_SUBTRACT
			KeyPadAdd = 334, // GLFW_KEY_KP_ADD
			KeyPadEnter = 335, // GLFW_KEY_KP_ENTER
			KeyPadEqual = 336, // GLFW_KEY_KP_EQUAL
			KeyLeftShift = 340, // GLFW_KEY_LEFT_SHIFT
			KeyLeftControl = 341, // GLFW_KEY_LEFT_CONTROL
			KeyLeftAlt = 342, // GLFW_KEY_LEFT_ALT
			KeyLeftSuper = 343, // GLFW_KEY_LEFT_SUPER
			KeyRightShift = 344, // GLFW_KEY_RIGHT_SHIFT
			KeyRightControl = 345, // GLFW_KEY_RIGHT_CONTROL
			KeyRightAlt = 346, // GLFW_KEY_RIGHT_ALT
			KeyRightSuper = 347, // GLFW_KEY_RIGHT_SUPER
			KeyMenu = 348 // GLFW_KEY_MENU
		};

		GT_EVENT_TYPE_IMP(Key);

		static std::string toString(const Key key) {
			if (key >= Key::KeySpace && key <= Key::KeyGraveAccent)
				return std::string(1, char(key));

			if (key >= Key::KeyF1 && key <= Key::KeyF25)
				return "F" + std::to_string(key - Key::KeyF1 + 1);

			switch (key) {
			case Key::KeyLeftShift: return "Shift";
			case Key::KeyLeftControl: return "Ctrl";
			case Key::KeyLeftAlt: return "Alt";
			default:
				GT_ASSERT(false, "Unknown keycode: " + std::to_string(key));
			}
			return "";
		}

		std::string toString() const override { 
			std::string keyName = m_key >= ' ' && m_key <= '`' ? std::string(1, char(m_key)) : std::to_string(m_key);
			return name() + ": " + keyName;
		}

		Key key() const { return m_key; }

		protected:
			KeyEvent(const std::shared_ptr<Window>& a_window, const Key a_key) : InputEvent(a_window), m_key(a_key) {}

		private:
			const Key m_key;
	};

	class KeyPressEvent : public KeyEvent {
	public:
		GT_EVENT_TYPE_IMP(KeyPress)

		KeyPressEvent(const std::shared_ptr<Window>& a_window, const KeyEvent::Key a_key, const bool repeat = false) : 
			KeyEvent(a_window, a_key),
			m_repeat(repeat)
		{}

		bool isRepeat() const { return m_repeat; }

	private:
		const bool m_repeat;
	};

	class KeyReleaseEvent : public KeyEvent {
	public:
		GT_EVENT_TYPE_IMP(KeyRelease)

		KeyReleaseEvent(const std::shared_ptr<Window>& a_window, const KeyEvent::Key a_key) : KeyEvent(a_window, a_key) {}
	};
}
