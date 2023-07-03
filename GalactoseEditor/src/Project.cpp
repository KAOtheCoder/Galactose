#include "Project.h"

#include <Galactose/Serialization/OutSerializer.h>
#include <Galactose/Serialization/NodeSerializer.h>

#include <fstream>
#include <filesystem>

#include <windows.h>

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

		out << OutSerializer::EndSeq << OutSerializer::EndMap;

		out.save(m_filePath.generic_string());
	}

	void Project::addScripts(const std::vector<std::filesystem::path>& a_path) {
		m_scripts.insert(a_path.begin(), a_path.end());
		make();
	}

	void Project::make() {
		std::string files;

		if (!m_scripts.empty()) {
			for (const auto& script : m_scripts)
				files += "\t\t\"" + script.generic_string() + "\",\n";

			files.resize(files.size() - 2); // remove last ",\n"
		}

		const auto& projectName = name();
		const auto& editordir = std::filesystem::current_path().generic_string();
		auto enginedir = std::filesystem::canonical(std::filesystem::path(editordir) / "../Galactose").generic_string();
		auto premake = R"(outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
editordir = ")" + editordir + R"("
enginedir = ")" + enginedir + R"("

workspace ")" + projectName + R"("
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

project ")" + projectName + R"("
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
)" + files + R"(
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

		const auto& projectDirectory = directory();
		const auto& premakeFilePath = (projectDirectory / "premake5.lua").generic_string();
		std::ofstream premakeFile(premakeFilePath);
		if (!premakeFile) {
			std::cerr << "Failed to open file '" << premakeFilePath << "'." << std::endl;
			return;
		}

		premakeFile << premake;

		const auto outputFileName = (projectDirectory / "PremakeOut.txt").generic_string();

		// TODO: Use Premake Api instead. std::system doesn't create solution files.
		const auto& command = "premake5.exe --file=" + premakeFilePath + " vs2022 > " + outputFileName;
		std::cout << command << std::endl;
		const int result = std::system(command.c_str());
		
		std::ifstream outputFile(outputFileName);

		if (outputFile)
			std::cout << outputFile.rdbuf() << std::endl;
		else
			std::cerr << "Failed to open file '" << outputFileName << "'." << std::endl;

		outputFile.close();

		if (result != 0)
			std::cerr << "Premake5 exited with code " << result << std::endl;
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
}
