#pragma once

#include "Component.h"

#include "Galactose/Core/Events/Event.h"
#include "Galactose/Core/Property.h"

#define GT_SCRIPT(S)\
GT_COMPONENT_COMMON(S)\
	inline static Script::MetaScript<S> s_meta;\
	using _ScriptType = S;\

namespace Galactose {
	class Script : public Component {
	public:
		static const std::set<std::string>& scripts() { return MetaBase::scripts(); }

		Script() = default;

		virtual void update() {};
		virtual void onEvent(const std::shared_ptr<Event>& a_event) {}
		Time& time() const { return entity()->scene()->time(); }

		const std::vector<PropertyBase*>& properties() const { return m_properties; }

	protected:
		template <class S>
		class MetaScript : public MetaComponent<S> {
		public:
			MetaScript() { MetaBase::insertScript(); }
			~MetaScript() { MetaBase::remove(); }
		};

		void saveContent(OutSerializer& emitter) const override {}
		bool loadContent(const NodeSerializer& node) override { return true; }

	private:
		std::vector<PropertyBase*> m_properties;

		friend class PropertyBase;
	};
}
