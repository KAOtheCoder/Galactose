#include "Component.h"
#include "Scene/Scene.h"

#include <yaml-cpp/yaml.h>

namespace Galactose {
	void Component::beginSave(YAML::Emitter& a_out, const char* a_name) {
		a_out << YAML::BeginMap
			<< YAML::Key << a_name << YAML::Value << YAML::BeginMap;
	}

	void Component::endSave(YAML::Emitter& a_out) {
		a_out << YAML::EndMap << YAML::EndMap;
	}
}
