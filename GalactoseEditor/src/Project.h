#pragma once

#include <filesystem>
#include <set>

namespace GalactoseEditor {
	class Project {
	public:
		Project(const std::filesystem::path& filePath);

		std::filesystem::path filePath() const { return m_filePath; }
		std::filesystem::path directory() const { return m_filePath.parent_path(); }

		std::filesystem::path editorScene(const bool absolute = false) const;
		void setEditorScene(const std::filesystem::path& a_path) { m_editorScene = a_path; }

		const std::set<std::filesystem::path>& scripts() const { return m_scripts; }
		const std::set<std::filesystem::path>& scenes() const { return m_scenes; }

		void save();

		void addScript(const std::filesystem::path& path);
		void addScene(const std::filesystem::path& a_path) { m_scenes.insert(a_path); }
		void make();

	private:
		std::filesystem::path m_filePath;
		std::filesystem::path m_startScene; // scene to load when project builded
		std::filesystem::path m_editorScene; // scene to load when project opened in editor
		std::set<std::filesystem::path> m_scenes;
		std::set<std::filesystem::path> m_scripts;
	};
}
