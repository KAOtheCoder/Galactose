#include "Project.h"

#include <Galactose/Serialization/OutSerializer.h>
#include <Galactose/Serialization/NodeSerializer.h>

#include <fstream>
#include <filesystem>

using namespace Galactose;

namespace GalactoseEditor {
	Project::Project(const std::filesystem::path& a_filePath) :
		m_filePath(a_filePath.is_absolute() ? a_filePath : std::filesystem::canonical(a_filePath))
	{
		if (std::filesystem::exists(a_filePath)) {
			const auto& node = NodeSerializer::loadFile(a_filePath.string());
			if (node.isNull())
				return;

			m_startScene = node["startScene"].as<std::string>();

			for (const auto& sceneNode : node["scenes"])
				m_scenes.insert(sceneNode.as<std::filesystem::path>());

			for (const auto& scriptNode : node["scripts"])
				m_scripts.insert(scriptNode.as<std::filesystem::path>());
		}
	}

	std::filesystem::path Project::editorScene(const bool absolute) const {
		return absolute ? std::filesystem::canonical(directory() / m_editorScene) : m_editorScene;
	}

	void Project::save() {
		OutSerializer out;
		out << OutSerializer::BeginMap
			<< OutSerializer::Key << "scenes" << OutSerializer::Value << OutSerializer::BeginSeq;

		for (const auto& scene : m_scenes)
			out << scene;

		out << OutSerializer::EndSeq
			<< OutSerializer::Key << "startScene" << OutSerializer::Value << m_startScene
			<< OutSerializer::Key << "scripts" << OutSerializer::Value << OutSerializer::BeginSeq;

		for (const auto& script : m_scripts)
			out << script;

		out << OutSerializer::EndSeq << OutSerializer::EndMap;

		out.save(m_filePath.string());
	}

	void Project::addScript(const std::filesystem::path& a_path) {
		m_scripts.insert(a_path);
		make();
	}

	void Project::make() {
		// TODO: create premake file and run it
	}
}
