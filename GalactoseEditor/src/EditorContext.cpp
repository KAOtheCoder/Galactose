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

	void EditorContext::newScene() {
		m_scene = std::make_shared<Scene>("Untitled");
		m_project.setEditorScene("");
		m_selectedEntity = nullptr;
	}

	void EditorContext::openScene() {
		const auto& path = FileDialog::open({ {"Scene", "scene"} });

		auto scene = std::make_shared<Scene>();
		if (!path.empty()) {
			std::ifstream stream(path);
			
			if (!scene->load(stream)) {
				setMessage("Failed to load scene: " + path, MessageType::Error);
				return;
			}

			m_scene = scene;
			m_project.setEditorScene(path, true);
			m_selectedEntity = nullptr;

			setMessage("Scene '" + path + "' loaded.");
		}
	}

	void EditorContext::saveSceneAs(const std::filesystem::path& a_filePath) {
		if (m_state != State::Stopped) {
			setMessage("You must exit play mode to save the scene.");
			return;
		}

		const auto& path = a_filePath.empty() ? FileDialog::save({ {"Scene", "scene"} }, "", m_scene->name().c_str()) : a_filePath;
		if (!path.empty()) {
			m_project.setEditorScene(path, true);
			std::ofstream fileStream(path);
			m_scene->save(fileStream);
			setMessage("Scene saved to '" + path.generic_string() + "'.");
		}
	}

	void EditorContext::saveProject() {
		saveScene();
		m_project.save();
		setMessage("Project '" + m_project.filePath().generic_string() + "' saved.");
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

			if (!m_scene->load(stream))
				setMessage("Failed to load scene: " + m_scene->name(), MessageType::Error);

			if (m_selectedEntity)
				m_selectedEntity = m_scene->getEntity(uuid);

			break;
		}

		m_state = a_state;
	}

	void EditorContext::setMessage(const std::string a_message, const MessageType a_type) {
		if (a_message == m_message && a_type == m_messageType)
			return;

		m_message = a_message;
		m_messageType = a_type;

		auto& stream = a_type == MessageType::Info ? std::cout : std::cerr;
		stream << a_message << std::endl;
	}

	void EditorContext::loadScripts() {
		if (!m_project.loadScripts())
			setMessage("Failed to load scripts.", MessageType::Error);
	}
}
