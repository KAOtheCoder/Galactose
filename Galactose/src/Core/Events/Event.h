#pragma once

#include "GalactosePCH.h"

#define GT_EVENT_TYPE_IMP(_type) static Event::Type staticType() { return Event::_type; }\
	Event::Type type() const override { return staticType(); }\
	std::string name() const override { return #_type; }

namespace Galactose {
	class Event {
	public:
		enum Type {
			None = 0,
			Key = 1,
			KeyPress = 2
		};

		virtual ~Event() = default;

		virtual Type type() const = 0;
		virtual std::string name() const = 0;
		virtual std::string toString() const { return name(); }

	private:
		bool m_handled = false;
	};
}
