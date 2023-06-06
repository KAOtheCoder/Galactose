#pragma once

#include "Core/Global.h"

#include <memory>

namespace Galactose {
	class Event;

	class GT_API Layer {
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void onUpdate() {}
		virtual void onEvent(const std::shared_ptr<Event>& a_event) {}
	};
}
