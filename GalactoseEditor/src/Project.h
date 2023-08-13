#pragma once

#include <filesystem>
#include <set>

namespace GalactoseEditor {
	class Project {
	public:
		Project(const std::filesystem::path& filePath);
		~Project();

		std::string name() const { return m_filePath.stem().generic_string(); }
		std::filesystem::path filePath() const { return m_filePath; }
		std::filesystem::path directory() const { return m_filePath.parent_path(); }

		std::filesystem::path editorScene(const bool absolute = false) const;
		void setEditorScene(const std::filesystem::path& a_path) { m_editorScene = a_path; }

		const std::set<std::filesystem::path>& scripts() const { return m_scripts; }
		const std::set<std::filesystem::path>& scenes() const { return m_scenes; }

		void save();

		void addScripts(const std::vector<std::filesystem::path>& paths);
		void removeScripts(const std::vector<std::filesystem::path>& paths);
		bool loadScripts();
		void make();

		void addScene(const std::filesystem::path& path);
		void removeScene(const std::filesystem::path& path);
		
		bool contains(const std::filesystem::path& relativePath) const;
		bool isPathReserved(const std::filesystem::path& relativePath) const;

	private:
		static std::filesystem::path premakePath() { return "premake5.lua"; }

		void unloadScripts();
		static void replaceAll(std::string& str, const std::string& from, const std::string& to);

		std::filesystem::path m_filePath;
		std::filesystem::path m_startScene; // scene to load when project builded
		std::filesystem::path m_editorScene; // scene to load when project opened in editor
		std::set<std::filesystem::path> m_scenes;
		std::set<std::filesystem::path> m_scripts;
		void* m_scriptLib = nullptr;
	};
}
