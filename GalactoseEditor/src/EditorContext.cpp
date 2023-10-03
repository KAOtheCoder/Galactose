#include "EditorContext.h"
#include "FileDialog.h"

#include <Galactose/Core/Global.h>
#include <Galactose/Scene/Scene.h>
#include <Galactose/Scene/Entity.h>

#include <fstream>

using namespace Galactose;

namespace GalactoseEditor {
	EditorContext::EditorContext(const std::filesystem::path& a_projectFilePath) :
		m_project(a_projectFilePath),
		m_scene(std::make_shared<Galactose::Scene>("Untitled"))
	{
		const auto& scene_path = m_project.editorScene(true);
		if (!scene_path.empty()) {
			std::ifstream stream(scene_path);
			m_scene->load(stream);
		}
	}

	void EditorContext::saveAndPrint() {
		std::ofstream fileStream(m_sceneFilePath);
		m_scene->save(fileStream);
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
		if (!path.empty()) {
			std::ifstream stream(path);
			
			if (!scene->load(stream)) {
				std::cerr << "Failed to load scene: " << path << std::endl;
				return;
			}

			m_scene = scene;
			m_sceneFilePath = path;
			m_selectedEntity = nullptr;

			const auto& relativePath = std::filesystem::relative(path, m_project.directory());
			if (m_project.scenes().contains(relativePath))
				m_project.setEditorScene(relativePath);

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

	void EditorContext::setState(const State a_state) {
		if (a_state == m_state)
			return;

		switch (a_state) {
		case State::Playing:
			if (m_state == State::Stopped) {
				std::ostringstream stream;
				m_scene->save(stream);
				m_sceneSave = stream.str();
			}
			break;

		case State::Paused:
			if (m_state == State::Stopped)
				return;

			if (m_state == State::Playing)
				m_scene->time().suspend();

			break;

		case State::Stopped:
			const auto& uuid = m_selectedEntity ? m_selectedEntity->uuid() : Uuid();

			m_scene->clear();
			std::istringstream stream(m_sceneSave);

			// TODO: restore entity order
			if (!m_scene->load(stream))
				std::cerr << "Failed to load scene: " << m_scene->name() << std::endl;

			if (m_selectedEntity)
				m_selectedEntity = m_scene->getEntity(uuid);

			break;
		}

		m_state = a_state;
	}

	void EditorContext::loadScripts() {
		if (!m_project.loadScripts())
			std::cerr << "Failed to load scripts." << std::endl;
	}
}
