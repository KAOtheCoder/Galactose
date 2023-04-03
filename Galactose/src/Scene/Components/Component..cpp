#include "Component.h"
#include "Scene/Scene.h"

#include <yaml-cpp/yaml.h>

namespace Galactose {
	Component::Meta::Meta(const std::string& a_name, const uint32_t a_id, Component* (*a_creator)(Entity*)) :
		id(a_id),
		creator(a_creator)
	{
		GT_ASSERT(s_metas.find(a_name) == s_metas.end(), "Component name '" + a_name + "' is not unique.");
		s_metas[a_name] = this;
	}

	Component::Meta* Component::Meta::meta(const std::string& a_name) {
		const auto& iter = Component::Meta::s_metas.find(a_name);
		GT_ASSERT(iter != Component::Meta::s_metas.end(), "No such component '" + a_name + "' exist.");
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
