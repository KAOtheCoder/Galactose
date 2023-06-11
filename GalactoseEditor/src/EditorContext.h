#pragma once

#include <Galactose/Scene/Scene.h>
#include <Galactose/Scene/Entity.h>

#include <imgui.h>
#include <ImGuizmo.h>

namespace GalactoseEditor {
	class EditorContext {
	public:
		EditorContext() :
			m_scene(std::make_shared<Galactose::Scene>("Untitled"))
		{}

		std::shared_ptr<Galactose::Scene> scene() const { return m_scene; }

		std::string sceneFilePath() const { return m_sceneFilePath; }
		void newScene();
		void openScene();
		void save();
		void saveAs();

		Galactose::Entity* selectedEntity() const { return m_selectedEntity; }
		void setSelectedEntity(Galactose::Entity* a_entity) { m_selectedEntity = a_entity; }

		bool isRunning() const { return m_running; }
		void setRunning(const bool a_running);

		ImGuizmo::OPERATION manipulatorOperation() const { return m_manipulatorOperation; }
		void setManipulatorOperation(const ImGuizmo::OPERATION a_operation) { m_manipulatorOperation = a_operation; }

	private:
		void saveAndPrint();

		bool m_running = false;
		std::shared_ptr<Galactose::Scene> m_scene;
		std::string m_sceneFilePath;
		Galactose::Entity* m_selectedEntity = nullptr;
		ImGuizmo::OPERATION m_manipulatorOperation = ImGuizmo::TRANSLATE;
	};
}
