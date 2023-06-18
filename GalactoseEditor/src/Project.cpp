#include "Project.h"

#include <Galactose/Serialization/OutSerializer.h>

#include <fstream>

using namespace Galactose;

namespace GalactoseEditor {
	Project::Project(const std::filesystem::path& a_filePath) :
		m_filePath(a_filePath)
	{
		if (std::filesystem::exists(a_filePath))
			;//load
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
