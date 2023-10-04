#include "Component.h"
#include "Galactose/Scene/Scene.h"
#include "Galactose/Serialization/OutSerializer.h"
#include "Galactose/Serialization/NodeSerializer.h"

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

	void Component::MetaBase::insertScript() {
		s_scripts.insert(name());
	}

	const std::set<std::string>& Component::MetaBase::scripts() { return s_scripts; }

	void Component::MetaBase::remove() {
		const auto& nameValue = name();

		s_metasByType.erase(type());
		s_metasByName.erase(nameValue);
		s_scripts.erase(nameValue);
	}

	Component::MetaBase* Component::MetaBase::meta(const std::string& a_name) {
		const auto& iter = s_metasByName.find(a_name);
		GT_ASSERT(iter != s_metasByName.end(), "No such component '" + a_name + "' exist.");
		return iter->second;
	}

	void Component::save(OutSerializer& a_out) const {
		a_out << OutSerializer::BeginMap
			<< OutSerializer::Key << name() << OutSerializer::Value << OutSerializer::BeginMap;
		saveContent(a_out);
		a_out << OutSerializer::EndMap << OutSerializer::EndMap;
	}

	bool Component::load(const NodeSerializer& a_node) {
		const auto& contentNode = a_node[name()];

		if (contentNode.isMap())
			return loadContent(contentNode);

		return false;
	}
}
