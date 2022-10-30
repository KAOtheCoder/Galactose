#pragma once

#include "GalactosePCH.h"

#define GT_EVENT_TYPE_IMP(_type) static Event::Type staticType() { return Event::_type; }\
	Event::Type type() const override { return staticType(); }\
	std::string name() const override { return GT_STRINGIFY(_type); }

namespace Galactose {
	class Event : public std::enable_shared_from_this<Event> {
	public:
		enum Type {
			None = 0,
			Input = bit(3),
			Key = Input | bit(4),
			KeyPress,
			KeyRelease,
			Mouse = Input | bit(5),
			MouseMove,
			MousePress,
			MouseRelease,
			Close = bit(6)
		};

		virtual ~Event() = default;

		virtual Type type() const = 0;
		virtual std::string name() const = 0;
		virtual std::string toString() const { return name(); }
		virtual void process() {}

		void setHandled() { m_handled = true; }
		bool isHandled() const { return m_handled; }

	private:
		bool m_handled = false;
	};
}
