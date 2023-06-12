#pragma once

#include "Component.h"

#include "Galactose/Core/Events/Event.h"
#include "Galactose/Core/Property.h"

#define GT_SCRIPT(a_script)\
GT_COMPONENT(a_script)\
	using _ScriptType = a_script;\

namespace Galactose {
	class Script : public Component {
	public:
		Script() = default;

		virtual void update() {};
		virtual void onEvent(const std::shared_ptr<Event>& a_event) {}
		Time& time() const { return entity()->scene()->time(); }

		const std::vector<PropertyBase*>& properties() const { return m_properties; }

	protected:
		void saveContent(OutSerializer& emitter) const override {}
		bool loadContent(const YAML::Node& node) override { return true; }

	private:
		std::vector<PropertyBase*> m_properties;

		friend class PropertyBase;
	};
}
