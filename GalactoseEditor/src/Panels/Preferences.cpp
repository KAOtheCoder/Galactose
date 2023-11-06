#include "Preferences.h"
#include "Widgets/PropertyTable.h"
#include "FileDialog.h"

#include <Galactose/Serialization/NodeSerializer.h>
#include <Galactose/Serialization/OutSerializer.h>

#include <fstream>

using namespace Galactose;

namespace GalactoseEditor {
	Preferences::Preferences() :
		Panel("Preferences")
	{
		setVisible(false);

		std::ifstream fileStream(PREFERENCES_FILENAME);
		const auto& node = NodeSerializer::load(fileStream);

		m_visualStudioPath = node["visualStudioPath"].as<std::filesystem::path>();
	}

	void Preferences::save() {
		if (!m_changed)
			return;

		OutSerializer out;
		out << OutSerializer::BeginMap
			<< OutSerializer::Key << "visualStudioPath" << OutSerializer::Value << m_visualStudioPath
			<< OutSerializer::EndMap;

		std::ofstream fileStream(PREFERENCES_FILENAME);
		out.save(fileStream);
	}

	void Preferences::onUpdate() {
		if (PropertyTable::beginTable("Preferences")) {
			if (PropertyTable::fileInput("Visual Studio Path", m_visualStudioPath, { { "Visual Studio Path", "exe" } }))
				m_changed = true;

			save();
			PropertyTable::endTable();
		}
	}

	void Preferences::setVisualStudioPath(const std::filesystem::path& a_path) {
		if (a_path == m_visualStudioPath)
			return;

		m_visualStudioPath = a_path;
		m_changed = true;
	}
}
