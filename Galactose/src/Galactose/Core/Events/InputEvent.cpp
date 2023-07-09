#include "InputEvent.h"
#include "Galactose/Core/Window.h"

namespace Galactose {
	void InputEvent::process() { 
		const auto& sharedWindow = window();
		if (sharedWindow)
			sharedWindow->onEvent(shared_from_this()); 
	}
}
