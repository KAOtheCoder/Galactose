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

		EditorContext(const std::filesystem::path& projectFilePath);

		Project& project() { return m_project; }
		const Project& project() const { return m_project; }

		std::shared_ptr<Galactose::Scene> scene() const { return m_scene; }

		std::string sceneFilePath() const { return m_sceneFilePath; }
		void newScene();
		void openScene();
		void saveScene();
		void saveSceneAs();
		void saveProject();

		Galactose::Entity* selectedEntity() const { return m_selectedEntity; }
		void setSelectedEntity(Galactose::Entity* a_entity) { m_selectedEntity = a_entity; }

		State state() const { return m_state; }
		void setState(const State state);

		ImGuizmo::OPERATION manipulatorOperation() const { return m_manipulatorOperation; }
		void setManipulatorOperation(const ImGuizmo::OPERATION a_operation) { m_manipulatorOperation = a_operation; }

		void loadScripts();

	private:
		void saveAndPrint();

		State m_state = Stopped;
		Project m_project;
		std::shared_ptr<Galactose::Scene> m_scene;
		std::string m_sceneSave;
		std::string m_sceneFilePath;
		Galactose::Entity* m_selectedEntity = nullptr;
		ImGuizmo::OPERATION m_manipulatorOperation = ImGuizmo::TRANSLATE;
	};
}
