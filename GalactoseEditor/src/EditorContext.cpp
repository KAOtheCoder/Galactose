#include "EditorContext.h"
#include "FileDialog.h"

#include <Galactose/Core/Global.h>
#include <Galactose/Scene/Scene.h>

using namespace Galactose;

namespace GalactoseEditor {
	EditorContext::EditorContext(const std::filesystem::path& a_projectFilePath) :
		m_project(a_projectFilePath),
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
		const auto& path = FileDialog::open({ {"Scene", "scene"} });

		auto scene = std::make_shared<Scene>();
		if (!path.empty() && scene->load(path)) {
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
		const auto& path = FileDialog::save({ {"Scene", "scene"} }, "", m_scene->name().c_str());
		if (!path.empty()) {
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

	void EditorContext::loadScripts() {
		if (!m_project.loadScripts())
			std::cerr << "Failed to load scripts." << std::endl;
	}
}
