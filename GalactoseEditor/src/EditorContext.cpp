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
			m_scene->load(scene_path.string());
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
		GT_ASSERT(result != NFD_ERROR, NFD_GetError());

		auto scene = std::make_shared<Scene>();
		if (result == NFD_OKAY && scene->load(path)) {
			m_scene = scene;
			m_sceneFilePath = path;
			m_selectedEntity = nullptr;

			const auto& relative_path = std::filesystem::relative(path, m_project.directory());
			if (m_project.scenes().contains(relative_path))
				m_project.setEditorScene(relative_path);

			std::cout << "Scene '" << m_sceneFilePath << "' loaded." << std::endl;
		}
	}

	void EditorContext::saveSceneAs() {
		nfdchar_t* path;
		nfdfilteritem_t filter = { "Scene", "yaml" };
		const auto result = NFD_SaveDialog(&path, &filter, 1, nullptr, m_scene->name().c_str());
		GT_ASSERT(result != NFD_ERROR, NFD_GetError());

		if (result == NFD_OKAY) {
			// TODO: When Project Explorer added remove following if statement.
			if (m_sceneFilePath.empty()) {
				const auto& relative_path = std::filesystem::relative(path, m_project.directory());
				m_project.addScene(relative_path);
				m_project.setEditorScene(relative_path);
			}

			m_sceneFilePath = path;
			saveAndPrint();
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

	void EditorContext::addScript() {
		nfdchar_t* path;
		nfdfilteritem_t filter = { "Script", "h,cpp,hpp" };
		const auto result = NFD_OpenDialog(&path, &filter, 1, nullptr);
		GT_ASSERT(result != NFD_ERROR, NFD_GetError());

		auto scene = std::make_shared<Scene>();
		if (result == NFD_OKAY) {
			const auto& relativePath = std::filesystem::relative(path, m_project.directory());
			m_project.addScript(relativePath);
		}
	}
}
