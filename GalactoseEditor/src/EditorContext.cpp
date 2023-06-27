#include "EditorContext.h"

#include <Galactose/Core/Global.h>
#include <Galactose/Scene/Scene.h>

#include <nfd.hpp>

using namespace Galactose;

namespace GalactoseEditor {
	EditorContext::EditorContext(const std::filesystem::path& a_filePath) :
		m_project(a_filePath),
		m_scene(std::make_shared<Galactose::Scene>("Untitled"))
	{
		const auto& scene_path = m_project.editorScene(true);
		if (!scene_path.empty())
			m_scene->load(scene_path.generic_string());
	}

	void EditorContext::saveAndPrint() {
		m_scene->save(m_sceneFilePath);
		std::cout << "Scene saved to '" << m_sceneFilePath << "'." << std::endl;
	}

	void EditorContext::newScene() {
		m_scene = std::make_shared<Scene>("Untitled");
		m_sceneFilePath.clear();
		m_selectedEntity = nullptr;
	}

	void EditorContext::openScene() {
		nfdchar_t* path;
		nfdfilteritem_t filter = { "Scene", "yaml" };
		const auto result = NFD_OpenDialog(&path, &filter, 1, nullptr);

		auto scene = std::make_shared<Scene>();
		if (result == NFD_OKAY) {
			if (scene->load(path)) {
				m_scene = scene;
				m_sceneFilePath = path;
				m_selectedEntity = nullptr;

				const auto& relative_path = std::filesystem::relative(path, m_project.directory());
				if (m_project.scenes().contains(relative_path))
					m_project.setEditorScene(relative_path);

				std::cout << "Scene '" << m_sceneFilePath << "' loaded." << std::endl;
			}

			NFD_FreePath(path);
		}
		else if (result == NFD_ERROR) {
			std::cerr << NFD_GetError() << std::endl;
		}
	}

	void EditorContext::saveSceneAs() {
		nfdchar_t* path;
		nfdfilteritem_t filter = { "Scene", "yaml" };
		const auto result = NFD_SaveDialog(&path, &filter, 1, nullptr, m_scene->name().c_str());

		if (result == NFD_OKAY) {
			// TODO: When Project Explorer added remove following if statement.
			if (m_sceneFilePath.empty()) {
				const auto& relative_path = std::filesystem::relative(path, m_project.directory());
				m_project.addScene(relative_path);
				m_project.setEditorScene(relative_path);
			}

			m_sceneFilePath = path;
			saveAndPrint();

			NFD_FreePath(path);
		}
		else if (result == NFD_ERROR) {
			std::cerr << NFD_GetError() << std::endl;
		}
	}

	void EditorContext::saveScene() {
		if (m_sceneFilePath.empty())
			saveSceneAs();
		else
			saveAndPrint();
	}

	void EditorContext::saveProject() {
		saveScene();
		m_project.save();
		std::cout << "Project '" << m_project.filePath() << "' saved." << std::endl;
	}

	void EditorContext::setRunning(const bool a_running) {
		if (a_running == m_running)
			return;

		if (!a_running)
			m_scene->time().suspend();

		m_running = a_running;
	}

	void EditorContext::addScripts() {
		const nfdpathset_t* paths;
		nfdfilteritem_t filter = { "Script", "h,cpp,hpp" };
		const auto result = NFD_OpenDialogMultiple(&paths, &filter, 1, nullptr);

		if (result == NFD_OKAY) {
			nfdpathsetsize_t pathCount;
			NFD_PathSet_GetCount(paths, &pathCount);
			std::vector<std::filesystem::path> relativePaths;
			relativePaths.reserve(pathCount);

			for (nfdpathsetsize_t i = 0; i < pathCount; ++i) {
				nfdchar_t* path;
				NFD_PathSet_GetPath(paths, i, &path);
				relativePaths.push_back(std::filesystem::relative(path, m_project.directory()));
				NFD_PathSet_FreePath(path);
			}

			NFD_PathSet_Free(paths);
			m_project.addScripts(relativePaths);
		}
		else if (result == NFD_ERROR) {
			std::cerr << NFD_GetError() << std::endl;
		}
	}
}
