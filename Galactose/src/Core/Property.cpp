#include "Property.h"

#include <Scene/Components/Script.h>

namespace Galactose {
	void PropertyBase::registerProperty(Script* a_script, const char* a_name) {
		a_script->m_properties[a_name] = this;
	}
}
