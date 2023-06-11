#include "Component.h"
#include "Scene/Scene.h"

#include <yaml-cpp/yaml.h>

namespace Galactose {
	void Component::MetaBase::insert() {
		const auto typeValue = type();
		if (s_metasByType.find(typeValue) != s_metasByType.end())
			return; // already inserted

		const auto& nameValue = name();
		GT_ASSERT(s_metasByName.find(nameValue) == s_metasByName.end(), "Component name '" + nameValue + "' is not unique.");
		s_metasByName.emplace(nameValue, this);
		s_metasByType.emplace(typeValue, this);
	}

	Component::MetaBase* Component::MetaBase::meta(const std::string& a_name) {
		const auto& iter = s_metasByName.find(a_name);
		GT_ASSERT(iter != s_metasByName.end(), "No such component '" + a_name + "' exist.");
		return iter->second;
	}

	void Component::save(YAML::Emitter& a_out) const {
		a_out << YAML::BeginMap
			<< YAML::Key << name() << YAML::Value << YAML::BeginMap;
		saveContent(a_out);
		a_out << YAML::EndMap << YAML::EndMap;
	}

	bool Component::load(const YAML::Node& a_node) {
		const auto& contentNode = a_node[name()];

		if (contentNode.IsMap())
			return loadContent(contentNode);

		return false;
	}
}
