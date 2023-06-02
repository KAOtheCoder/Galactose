#include "InputEvent.h"
#include "Core/Window.h"

namespace Galactose {
	void InputEvent::process() { m_window->onEvent(shared_from_this()); }
}
