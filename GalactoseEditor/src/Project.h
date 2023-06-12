#pragma once

#include <filesystem>
#include <set>

namespace GalactoseEditor {
	class Scene;

	class Project {
	public:
		Project(const std::filesystem::path& filePath);

		std::filesystem::path filePath() const { return m_filePath; }
		std::filesystem::path directory() const { return m_filePath.parent_path(); }

		void save();

		std::set<std::filesystem::path> scenes;
		std::set<std::filesystem::path> scripts;

	private:
		std::filesystem::path m_filePath;
		std::filesystem::path m_startScene;
	};
}
