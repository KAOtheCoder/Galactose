#pragma once

#include <memory>

namespace Galactose {
	class Event;
}

namespace GalactoseEditor {
	class SideBarItem {
	public:
		virtual float size() const = 0;
		virtual int additionalWindowFlags() const { return 0; }

		virtual void update() = 0;
		virtual void onEvent(const std::shared_ptr<Galactose::Event>& a_event) {}
	};
}
