#pragma once

#include <filesystem>
#include <set>

namespace GalactoseEditor {
	class Project {
	public:
		Project(const std::filesystem::path& filePath);
		~Project();

		bool isValid() const { return !m_filePath.empty(); }

		std::string name() const { return m_filePath.stem().generic_string(); }
		std::filesystem::path filePath() const { return m_filePath; }
		std::filesystem::path directory() const { return m_filePath.parent_path(); }

		std::filesystem::path editorScene(const bool absolute = false) const;
		void setEditorScene(const std::filesystem::path& a_path, const bool absolute = false);

		const std::set<std::filesystem::path>& scripts() const { return m_scripts; }
		const std::set<std::filesystem::path>& scenes() const { return m_scenes; }

		void save();

		void addScripts(const std::vector<std::filesystem::path>& paths);
		void removeScripts(const std::vector<std::filesystem::path>& paths);
		void renameScript(const std::filesystem::path& oldPath, const std::filesystem::path& newPath);
		bool loadScripts();
		void make();

		void addScene(const std::filesystem::path& path);
		void removeScene(const std::filesystem::path& path);
		void renameScene(const std::filesystem::path& oldPath, const std::filesystem::path& newPath);

		bool contains(const std::filesystem::path& relativePath) const;
		bool isPathReserved(const std::filesystem::path& relativePath) const;

		bool openSolution();

	private:
		static std::filesystem::path premakePath() { return "premake5.lua"; }
		static void replaceAll(std::string& str, const std::string& from, const std::string& to);

		void unloadScripts();
		std::filesystem::path reservedPath(const std::string& extension, const bool absolute = false) const;
		std::filesystem::path workspacePath(const bool absolute = false) const { return reservedPath(".sln", absolute); }
		std::filesystem::path scripProjectPath(const bool absolute = false) const { return reservedPath(".vcxproj", absolute); }

		std::filesystem::path m_filePath;
		std::filesystem::path m_startScene; // scene to load when project builded
		std::filesystem::path m_editorScene; // scene to load when project opened in editor
		std::set<std::filesystem::path> m_scenes;
		std::set<std::filesystem::path> m_scripts;
		void* m_scriptLib = nullptr;
	};
}
