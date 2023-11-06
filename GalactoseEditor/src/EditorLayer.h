#pragma once

#include "ImGuiLayer.h"
#include "Panels/Preferences.h"
#include "Panels/SceneViewport.h"
#include "Panels/GameViewport.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/Inspector/Inspector.h"
#include "Panels/AssetExplorer.h"
#include "SideBars/SideBar.h"

namespace GalactoseEditor {
	class EditorContext;

	class EditorLayer : public ImGuiLayer {
	public:
		EditorLayer(Galactose::Window* window, const std::filesystem::path& projectFilePath);

		void updateContent() override;
		void onEvent(const std::shared_ptr<Galactose::Event>& a_event) override;

	private:
		enum Layout {
			None,
			Default
		};

		void updateLayout();

		std::shared_ptr<EditorContext> m_editorContext;
		Preferences m_preferences;
		SceneViewport m_sceneViewport;
		GameViewport m_gameViewport;
		SceneHierarchy m_sceneHierarchy;
		Inspector m_inspector;
		AssetExplorer m_assetExplorer;
		SideBar m_upBar;
		SideBar m_downBar;
		Layout m_layout;
		bool m_openExitDialog = false;
	};
}
