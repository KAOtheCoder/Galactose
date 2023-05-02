#pragma once

#include "Component.h"

#include <Core/Events/Event.h>
#include <Core/Property.h>

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
		void saveContent(YAML::Emitter& emitter) const override {}
		bool loadContent(const YAML::Node& node) override { return true; }

	private:
		std::vector<PropertyBase*> m_properties;

		friend class PropertyBase;
	};

	class TestScript : public Script {
		GT_SCRIPT(TestScript);

	public:
		TestScript() :
			m_a(3),
			GT_INIT_PROPERTY(readOnlyA),
			/*readOnlyGetA("readOnlyAGet", this),
			getMA("getMA", this),
			mSetA("mSetA", this),*/
			GT_INIT_PROPERTY(getSetA)
		{}

		float m_a;

		float get2A() const { return 2 * m_a; }
		float getAC() const { return m_a; }
		void setA(const float& a_a) { m_a = a_a; }
		GT_READONLY_PROPERTY(float, readOnlyA, get2A);
		/*ReadOnlyProperty<TestScript, float, &getA> readOnlyGetA;
		Property<TestScript, float, &getA, &TestScript::m_a> getMA;
		Property<TestScript, float, &TestScript::m_a, &setA> mSetA;*/
		GT_PROPERTY(float, getSetA, m_a, setA);
	};
}
