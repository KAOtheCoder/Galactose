#pragma once

#include "ImGuiLayer.h"
#include "Panels/SceneViewport.h"
#include "Panels/GameViewport.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/Inspector/Inspector.h"
#include "Widgets/MenuBar.h"
#include "Widgets/ToolBar.h"

namespace GalactoseEditor {
	class EditorContext;

	class EditorLayer : public ImGuiLayer {
	public:
		EditorLayer(Galactose::Window* window, const std::string& projectFilePath);

		void updateContent() override;
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event) override;

	private:
		enum Layout {
			None,
			Default
		};

		void updateLayout();
		void updateUpBar();

		std::shared_ptr<EditorContext> m_editorContext;
		SceneViewport m_sceneViewport;
		GameViewport m_gameViewport;
		SceneHierarchy m_sceneHierarchy;
		Inspector m_inspector;
		MenuBar m_menuBar;
		ToolBar m_toolBar;
		Layout m_layout = Layout::None;
	};
}
