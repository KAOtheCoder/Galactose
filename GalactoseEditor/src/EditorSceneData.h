#pragma once

#include <Scene/Scene.h>
#include <Scene/Entity.h>

namespace GalactoseEditor {
	class EditorSceneData {
	public:
		EditorSceneData() :
			m_scene(std::make_shared<Galactose::Scene>("Untitled"))
		{}

		std::shared_ptr<Galactose::Scene> scene() const { return m_scene; }

		std::string filePath() const { return m_filePath; }
		void newScene();
		void openScene();
		void save();
		void saveAs();

		Galactose::Entity* selectedEntity() const { return m_selectedEntity; }
		void setSelectedEntity(Galactose::Entity* a_entity) { m_selectedEntity = a_entity; }

		bool isRunning() const { return m_running; }
		void setRunning(const bool a_running);

	private:
		void saveAndPrint();

		std::shared_ptr<Galactose::Scene> m_scene;
		std::string m_filePath;
		Galactose::Entity* m_selectedEntity = nullptr;
		bool m_running = false;
	};
}
