#pragma once

#include "Panels/SceneViewport.h"
#include "Panels/GameViewport.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/Inspector/Inspector.h"
#include "Widgets/MenuBar.h"
#include "Widgets/ToolBar.h"

#include <Core/Layer.h>

namespace Galactose {
	class Window;
}

namespace GalactoseEditor {
	class EditorContext;

	class EditorLayer : public Galactose::Layer {
	public:
		EditorLayer(Galactose::Window* window);
		~EditorLayer() override;

		void onUpdate() override;
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event) override;

	private:
		enum Layout {
			None,
			Default
		};

		void updateLayout();
		void updateUpBar();

		std::shared_ptr<EditorContext> m_sceneContext;
		SceneViewport m_sceneViewport;
		GameViewport m_gameViewport;
		SceneHierarchy m_sceneHierarchy;
		Inspector m_inspector;
		MenuBar m_menuBar;
		ToolBar m_toolBar;
		Layout m_layout = Layout::None;
	};
}
