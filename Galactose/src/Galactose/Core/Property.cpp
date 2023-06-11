#include "Property.h"

#include <Galactose/Scene/Components/Script.h>

namespace Galactose {
	void PropertyBase::registerProperty(Script* a_script) {
		a_script->m_properties.push_back(this);
	}
}
