#pragma once

#include "Event.h"
#include <GLFW/glfw3.h>

namespace Galactose {
	class KeyEvent : public Event {
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
			KeyWorld1 = GLFW_KEY_WORLD_1, // 161 non-US #1
			KeyWorld2 = GLFW_KEY_WORLD_2, // 162 non-US #2

			// Function keys
			KeyEscape = GLFW_KEY_ESCAPE, // 256
			KeyEnter = GLFW_KEY_ENTER, // 257
			KeyTab = GLFW_KEY_TAB, // 258
			KeyBackspace = GLFW_KEY_BACKSPACE, // 259
			KeyInsert = GLFW_KEY_INSERT, // 260
			KeyDelete = GLFW_KEY_DELETE, // 261
			KeyRight = GLFW_KEY_RIGHT, // 262
			KeyLeft = GLFW_KEY_LEFT, // 263
			KeyDown = GLFW_KEY_DOWN, // 264
			KeyUp = GLFW_KEY_UP, // 265
			KeyPageUp = GLFW_KEY_PAGE_UP, // 266
			KeyPageDown = GLFW_KEY_PAGE_DOWN, // 267
			KeyHome = GLFW_KEY_HOME, // 268
			KeyEnd = GLFW_KEY_END, // 269
			KeyCapsLock = GLFW_KEY_CAPS_LOCK, // 280
			KeyScrollLock = GLFW_KEY_SCROLL_LOCK, // 281
			KeyNumLock = GLFW_KEY_NUM_LOCK, // 282
			KeyPrintScreen = GLFW_KEY_PRINT_SCREEN, // 283
			KeyPause = GLFW_KEY_PAUSE, // 284
			KeyF1 = GLFW_KEY_F1, // 290
			KeyF2 = GLFW_KEY_F2, // 291
			KeyF3 = GLFW_KEY_F3, // 292
			KeyF4 = GLFW_KEY_F4, // 293
			KeyF5 = GLFW_KEY_F5, // 294
			KeyF6 = GLFW_KEY_F6, // 295
			KeyF7 = GLFW_KEY_F7, // 296
			KeyF8 = GLFW_KEY_F8, // 297
			KeyF9 = GLFW_KEY_F9, // 298
			KeyF10 = GLFW_KEY_F10, // 299
			KeyF11 = GLFW_KEY_F11, // 300
			KeyF12 = GLFW_KEY_F12, // 301
			KeyF13 = GLFW_KEY_F13, // 302
			KeyF14 = GLFW_KEY_F14, // 303
			KeyF15 = GLFW_KEY_F15, // 304
			KeyF16 = GLFW_KEY_F16, // 305
			KeyF17 = GLFW_KEY_F17, // 306
			KeyF18 = GLFW_KEY_F18, // 307
			KeyF19 = GLFW_KEY_F19, // 308
			KeyF20 = GLFW_KEY_F20, // 309
			KeyF21 = GLFW_KEY_F21, // 310
			KeyF22 = GLFW_KEY_F22, // 311
			KeyF23 = GLFW_KEY_F23, // 312
			KeyF24 = GLFW_KEY_F24, // 313
			KeyF25 = GLFW_KEY_F25, // 314
			KeyPad0 = GLFW_KEY_KP_0, // 320
			KeyPad1 = GLFW_KEY_KP_1, // 321
			KeyPad2 = GLFW_KEY_KP_2, // 322
			KeyPad3 = GLFW_KEY_KP_3, // 323
			KeyPad4 = GLFW_KEY_KP_4, // 324
			KeyPad5 = GLFW_KEY_KP_5, // 325
			KeyPad6 = GLFW_KEY_KP_6, // 326
			KeyPad7 = GLFW_KEY_KP_7, // 327
			KeyPad8 = GLFW_KEY_KP_8, // 328
			KeyPad9 = GLFW_KEY_KP_9, // 329
			KeyPadDecimal = GLFW_KEY_KP_DECIMAL, // 330
			KeyPadDivide = GLFW_KEY_KP_DIVIDE, // 331
			KeyPadMultiply = GLFW_KEY_KP_MULTIPLY, // 332
			KeyPadSubtract = GLFW_KEY_KP_SUBTRACT, // 333
			KeyPadAdd = GLFW_KEY_KP_ADD, // 334
			KeyPadEnter = GLFW_KEY_KP_ENTER, // 335
			KeyPadEqual = GLFW_KEY_KP_EQUAL, // 336
			KeyLeftShift = GLFW_KEY_LEFT_SHIFT, // 340
			KeyLeftControl = GLFW_KEY_LEFT_CONTROL, // 341
			KeyLeftAlt = GLFW_KEY_LEFT_ALT, // 342
			KeyLeftSuper = GLFW_KEY_LEFT_SUPER, // 343
			KeyRightShift = GLFW_KEY_RIGHT_SHIFT, // 344
			KeyRightControl = GLFW_KEY_RIGHT_CONTROL, // 345
			KeyRightAlt = GLFW_KEY_RIGHT_ALT, // 346
			KeyRightSuper = GLFW_KEY_RIGHT_SUPER, // 347
			KeyMenu = GLFW_KEY_MENU // 348
		};

		GT_EVENT_TYPE_IMP(Key)

		std::string toString() const override { 
			const auto keyName = glfwGetKeyName(m_key, 0);
			return name() + ": " + (keyName ? std::string(keyName) : std::to_string(m_key)); 
		}

		protected:
			KeyEvent(const Key a_key) : m_key(a_key) {}

		private:
			Key m_key;
	};

	class KeyPressEvent : public KeyEvent {
	public:
		GT_EVENT_TYPE_IMP(KeyPress)

		KeyPressEvent(const KeyEvent::Key a_key) : KeyEvent(a_key) {}
	};

	class KeyRepeatEvent : public KeyEvent {
	public:
		GT_EVENT_TYPE_IMP(KeyRepeat)

		KeyRepeatEvent(const KeyEvent::Key a_key) : KeyEvent(a_key) {}
	};

	class KeyReleaseEvent : public KeyEvent {
	public:
		GT_EVENT_TYPE_IMP(KeyRelease)

		KeyReleaseEvent(const KeyEvent::Key a_key) : KeyEvent(a_key) {}
	};
}
