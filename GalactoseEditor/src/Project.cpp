#include "Project.h"

#include <Galactose/Serialization/OutSerializer.h>
#include <Galactose/Serialization/NodeSerializer.h>

#include <fstream>
#include <filesystem>
#include <array>

#include <windows.h>

extern "C"
{
#include <host/premake.h>
}

using namespace Galactose;

namespace GalactoseEditor {
	Project::Project(const std::filesystem::path& a_filePath) :
		m_filePath(a_filePath.is_absolute() ? a_filePath : std::filesystem::canonical(a_filePath))
	{
		if (std::filesystem::exists(a_filePath)) {
			const auto& node = NodeSerializer::loadFile(a_filePath.generic_string());
			if (node.isNull())
				return;

			m_startScene = node["startScene"].as<std::string>();

			for (const auto& sceneNode : node["scenes"])
				m_scenes.insert(sceneNode.as<std::filesystem::path>());

			for (const auto& scriptNode : node["scripts"])
				m_scripts.insert(scriptNode.as<std::filesystem::path>());
		}
	}

	Project::~Project() {
		unloadScripts();
	}

	std::filesystem::path Project::editorScene(const bool absolute) const {
		return absolute && !m_editorScene.empty() ? std::filesystem::canonical(directory() / m_editorScene) : m_editorScene;
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

		out << OutSerializer::EndSeq 
			<< OutSerializer::EndMap;

		out.save(m_filePath.generic_string());
	}

	void Project::addScripts(const std::vector<std::filesystem::path>& a_paths) {
		const auto prevScriptsSize = m_scripts.size();
		m_scripts.insert(a_paths.begin(), a_paths.end());

		if (m_scripts.size() > prevScriptsSize)
			make();
	}

	void Project::removeScripts(const std::vector<std::filesystem::path>& a_paths) {
		size_t deletedScriptsSize = 0;

		for (const auto& path : a_paths)
			deletedScriptsSize += m_scripts.erase(path);

		if (deletedScriptsSize > 0)
			make();
	}

	void Project::renameScript(const std::filesystem::path& oldPath, const std::filesystem::path& newPath) {
		if (oldPath == newPath)
			return;

		if (m_scripts.erase(oldPath) > 0) {
			m_scripts.insert(newPath);
			make();
		}
	}

	void Project::addScene(const std::filesystem::path& a_path) {
		if (m_scenes.insert(a_path).second && m_startScene.empty())
			m_startScene = a_path;
	}

	void Project::removeScene(const std::filesystem::path& a_path) {
		if (m_scenes.erase(a_path) > 0 && m_startScene == a_path) {
			if (m_scenes.empty())
				m_startScene.clear();
			else
				m_startScene = *m_scenes.begin();
		}
	}

	void Project::renameScene(const std::filesystem::path& oldPath, const std::filesystem::path& newPath) {
		if (oldPath == newPath)
			return;

		if (m_scenes.erase(oldPath) > 0) {
			m_scenes.insert(newPath);

			if (m_startScene == oldPath)
				m_startScene = newPath;
		}
	}

	void Project::replaceAll(std::string& str, const std::string& from, const std::string& to) {
		size_t pos = str.find(from);

		while (pos != std::string::npos) {
			str.replace(pos, from.size(), to);
			pos = str.find(from, pos + to.size());
		}
	}

	void Project::make() {
		std::string premakeScript = R"(
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
editordir = "<EDITOR_DIR>"
enginedir = "<ENGINE_DIR>"

workspace "<PROJECT>"
	architecture "x86_64"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

project "<PROJECT>"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	implibdir ("bin/" .. outputdir .. "/%{prj.name}")

	libdirs {
		(enginedir .. "/bin/" .. outputdir .. "/Galactose")
	}

	files
	{
		<FILES>
	}

	links
	{
		"Galactose.lib"
	}

	includedirs
	{
		(enginedir .. "/src"),
		(enginedir .. "/vendor/glm"),
		(enginedir .. "/vendor/entt/src/entt")
	}

	filter "system:windows"
		defines "GT_WINDOWS"
		systemversion "latest"
		postbuildcommands {
			("{echo} postbuild"),
			("{copy} %{cfg.targetdir}/%{prj.name}.dll " ..editordir.. "/bin/" .. outputdir .. "/GalactoseEditor"),
		}

	filter "system:linux"
		defines "GT_LINUX"
		systemversion "latest"

	filter "configurations:Debug"
		defines "GT_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "GT_RELEASE"
		runtime "Release"
		optimize "On"
)";

		const auto& editordir = std::filesystem::current_path().generic_string();
		const auto& enginedir = std::filesystem::canonical(std::filesystem::path(editordir) / "../Galactose").generic_string();

		std::string files;

		if (!m_scripts.empty()) {
			const std::string POSTFIX(",\n\t\t");
			for (const auto& script : m_scripts)
				files += "\"" + script.generic_string() + "\"" + POSTFIX;

			files.resize(files.size() - POSTFIX.size()); // remove last postfix
		}

		replaceAll(premakeScript, "<EDITOR_DIR>", editordir);
		replaceAll(premakeScript, "<ENGINE_DIR>", enginedir);
		replaceAll(premakeScript, "<PROJECT>", name());
		replaceAll(premakeScript, "<FILES>", files);

		// inspired from premake_main.c
		auto luaState = luaL_newstate();
		luaL_openlibs(luaState);
		int result = premake_init(luaState);

		if (result == OKAY) {
			const auto& premakeFilePath = (directory() / premakePath()).generic_string();
			std::ofstream premakeFile(premakeFilePath);
			
			if (!premakeFile) {
				std::cerr << "Failed to open file '" << premakeFilePath << "'." << std::endl;
				return;
			}

			premakeFile << premakeScript;
			// ensure file is closed, otherwise premake might fail to generate files.
			premakeFile.close();

			const int argc = 3;
			const std::array<std::string, argc> args = { "premake5", "vs2022", "--file=" + premakeFilePath };
			std::array<const char*, argc> argv = { args[0].data(), args[1].data(), args[2].data() };

			result = premake_execute(luaState, argc, argv.data(), "vendor/Premake5/Premake5/src/_premake_main.lua");

			if (result != OKAY)
				std::cerr << "Premake5 exited with code " << result << std::endl;
		}

		lua_close(luaState);
	}

	bool Project::loadScripts() {
		unloadScripts();

		m_scriptLib = LoadLibraryA((name() + ".dll").c_str());
		return m_scriptLib;
	}

	void Project::unloadScripts() {
		if (!m_scriptLib)
			return;

		if (!FreeLibrary(static_cast<HMODULE>(m_scriptLib)))
			std::cerr << "Failed to free library: " << name() << std::endl;
	}

	bool Project::contains(const std::filesystem::path& a_relativePath) const {
		return m_scenes.contains(a_relativePath) || m_scripts.contains(a_relativePath) || a_relativePath == m_filePath;
	}

	bool Project::isPathReserved(const std::filesystem::path& a_relativePath) const {
		const auto& projectName = name();

		return a_relativePath == premakePath() || a_relativePath == projectName + ".pro" || a_relativePath == "bin" 
			|| a_relativePath == "bin-int" || a_relativePath == projectName + ".sln" || a_relativePath == projectName + ".vcxproj";
	}
}
