#pragma once

#include "Component.h"

#include <Core/Events/Event.h>

namespace Galactose {
	class Script : public Component {
	public:
		Script() = default;

		virtual void onEvent(const std::shared_ptr<Event>& a_event) {}

	protected:
		void saveContent(YAML::Emitter& emitter) const override {}
		bool loadContent(const YAML::Node& node) override { return true; }
	};
}
