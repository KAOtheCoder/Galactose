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
		EditorContext(const std::filesystem::path& filePath);

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

		bool isRunning() const { return m_running; }
		void setRunning(const bool a_running);

		ImGuizmo::OPERATION manipulatorOperation() const { return m_manipulatorOperation; }
		void setManipulatorOperation(const ImGuizmo::OPERATION a_operation) { m_manipulatorOperation = a_operation; }

		void addScripts();
		void loadScripts();

	private:
		void saveAndPrint();

		bool m_running = false;
		Project m_project;
		std::shared_ptr<Galactose::Scene> m_scene;
		std::string m_sceneFilePath;
		Galactose::Entity* m_selectedEntity = nullptr;
		ImGuizmo::OPERATION m_manipulatorOperation = ImGuizmo::TRANSLATE;
	};
}
