#pragma once

#include "Galactose/Core/Window.h"

namespace Galactose {
	class Renderer {
	public:
		static std::shared_ptr<Renderer> create(const std::shared_ptr<Window>& a_window);
	};
}