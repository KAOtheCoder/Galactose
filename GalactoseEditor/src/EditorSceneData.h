#pragma once

#include <Scene/Scene.h>
#include <Scene/Entity.h>

namespace GalactoseEditor {
	class EditorSceneData {
	public:
		EditorSceneData() :
			m_scene(std::make_shared<Galactose::Scene>("New Scene"))
		{}

		std::shared_ptr<Galactose::Scene> scene() const { return m_scene; }

		std::string filePath() const { return m_filePath; }
		void save();
		void saveAs();

		Galactose::Entity* selectedEntity() const { return m_selectedEntity; }
		void setSelectedEntity(Galactose::Entity* a_entity) { m_selectedEntity = a_entity; }

	private:
		void saveAndPrint();

		std::shared_ptr<Galactose::Scene> m_scene;
		std::string m_filePath;
		Galactose::Entity* m_selectedEntity = nullptr;
	};
}
