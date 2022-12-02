#pragma once

#include <Scene/Scene.h>
#include <Scene/Entity.h>

namespace GalactoseEditor {
	class EditorSceneData {
	public:
		EditorSceneData() :
			m_scene(std::make_shared<Galactose::Scene>("Scene"))
		{}

		std::shared_ptr<Galactose::Scene> scene() const { return m_scene; }
		Galactose::Entity* selectedEntity() const { return m_selectedEntity; }
		void setSelectedEntity(Galactose::Entity* a_entity) { m_selectedEntity = a_entity; }

	private:
		std::shared_ptr<Galactose::Scene> m_scene;
		Galactose::Entity* m_selectedEntity = nullptr;
	};
}
