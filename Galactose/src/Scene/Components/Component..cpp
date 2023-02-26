#include "Component.h"
#include "Scene/Scene.h"

#include <yaml-cpp/yaml.h>

namespace Galactose {
	Component::Meta::Meta(const std::string& a_name, const std::function<Component* (Entity* a_entity)>& a_creator) :
		creator(a_creator)
	{
		GT_ASSERT(s_metas.find(a_name) == s_metas.end(), "Component name '" + a_name + "' is not unique.");
		s_metas[a_name] = this;
	}

	void Component::save(YAML::Emitter& a_out) const {
		a_out << YAML::BeginMap
			<< YAML::Key << name() << YAML::Value << YAML::BeginMap;
		saveContent(a_out);
		a_out << YAML::EndMap << YAML::EndMap;
	}
}
