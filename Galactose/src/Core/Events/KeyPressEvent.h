#pragma once

#include "KeyEvent.h"

namespace Galactose {
	class KeyPressEvent : public KeyEvent {
	public:
		GT_EVENT_TYPE_IMP(KeyPress)

		KeyPressEvent(const KeyEvent::Key key) : KeyEvent(key) {}
	};
}
