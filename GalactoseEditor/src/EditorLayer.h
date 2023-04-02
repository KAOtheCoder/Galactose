#pragma once

#include "Panels/SceneViewport.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/Inspector/Inspector.h"
#include "Widgets/MenuBar.h"

#include <Core/Layer.h>

namespace Galactose {
	class Window;
}

namespace GalactoseEditor {
	class EditorSceneData;

	class EditorLayer : public Galactose::Layer {
	public:
		EditorLayer(Galactose::Window* window);
		~EditorLayer() override;

		void onUpdate() override;
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event) override;

	private:
		std::shared_ptr<EditorSceneData> m_sceneData;
		SceneViewport m_sceneViewport;
		SceneHierarchy m_sceneHierarchy;
		Inspector m_inspector;
		MenuBar m_menuBar;
	};
}
