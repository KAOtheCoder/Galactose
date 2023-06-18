#include "Project.h"

#include <Galactose/Serialization/OutSerializer.h>
#include <Galactose/Serialization/NodeSerializer.h>

#include <fstream>

using namespace Galactose;

namespace GalactoseEditor {
	Project::Project(const std::filesystem::path& a_filePath) :
		m_filePath(a_filePath)
	{
		if (std::filesystem::exists(a_filePath)) {
			const auto& node = NodeSerializer::loadFile(a_filePath.string());
			if (node.isNull())
				return;

			m_startScene = node["startScene"].as<std::string>();

			for (const auto& sceneNode : node["scenes"])
				scenes.insert(sceneNode.as<std::filesystem::path>());

			for (const auto& scriptNode : node["scripts"])
				scenes.insert(scriptNode.as<std::filesystem::path>());
		}
	}

	void Project::save() {
		OutSerializer out;
		out << OutSerializer::BeginMap
			<< OutSerializer::Key << "scenes" << OutSerializer::Value << OutSerializer::BeginSeq;

		for (const auto& scene : scenes)
			out << scene;

		out << OutSerializer::EndSeq
			<< OutSerializer::Key << "startScene" << OutSerializer::Value << m_startScene
			<< OutSerializer::Key << "scripts" << OutSerializer::Value << OutSerializer::BeginSeq;

		for (const auto& script : scripts)
			out << script;

		out << OutSerializer::EndSeq << OutSerializer::EndMap;

		out.save(m_filePath.string());
	}
}
