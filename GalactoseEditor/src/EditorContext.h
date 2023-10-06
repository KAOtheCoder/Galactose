#pragma once

#include "Project.h"

#include <imgui.h>
#include <ImGuizmo.h>

namespace Galactose {
	class Scene;
	class Entity;
}

namespace GalactoseEditor {
	class EditorContext {
	public:
		enum State {
			Stopped,
			Playing,
			Paused
		};

		enum MessageType {
			Info,
			Warning,
			Error
		};

		EditorContext(const std::filesystem::path& projectFilePath);

		Project& project() { return m_project; }
		const Project& project() const { return m_project; }

		std::shared_ptr<Galactose::Scene> scene() const { return m_scene; }

		void newScene();
		void openScene();
		void saveScene() { saveSceneAs(m_project.editorScene(true)); }
		void saveSceneAs(const std::filesystem::path& filePath = "");
		void saveProject();

		Galactose::Entity* selectedEntity() const { return m_selectedEntity; }
		void setSelectedEntity(Galactose::Entity* a_entity) { m_selectedEntity = a_entity; }

		State state() const { return m_state; }
		void setState(const State state);

		ImGuizmo::OPERATION manipulatorOperation() const { return m_manipulatorOperation; }
		void setManipulatorOperation(const ImGuizmo::OPERATION a_operation) { m_manipulatorOperation = a_operation; }

		std::string message() const { return m_message; }
		MessageType messageType() const { return m_messageType; }
		void setMessage(const std::string a_message, const MessageType type = MessageType::Info);

		void loadScripts();

	private:
		State m_state = Stopped;
		Project m_project;
		std::shared_ptr<Galactose::Scene> m_scene;
		std::string m_sceneSave;
		Galactose::Entity* m_selectedEntity = nullptr;
		ImGuizmo::OPERATION m_manipulatorOperation = ImGuizmo::TRANSLATE;
		MessageType m_messageType = MessageType::Info;
		std::string m_message;
	};
}
