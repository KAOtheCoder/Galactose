#include "ToolBar.h"
#include "EditorSceneData.h"

#include <Renderer/Texture.h>

#include <imgui.h>

using namespace Galactose;

namespace GalactoseEditor {
	ToolBar::ToolBar() :
		m_playIcon(Texture::create("assets/textures/play.png")),
		m_pauseIcon(Texture::create("assets/textures/pause.png"))
	{}

	float ToolBar::height() const {
		return (2.f * ImGui::GetFontSize()) + (2 * (ImGui::GetStyle().FramePadding.y + ImGui::GetStyle().WindowPadding.y));
	}

	void ToolBar::update(const std::shared_ptr<EditorSceneData>& a_sceneData) {
		const float buttonSize = 2.f * ImGui::GetFontSize();
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - buttonSize - ImGui::GetStyle().FramePadding.x) / 2);

		const bool running = a_sceneData->isRunning();
		const auto& icon = running ? m_pauseIcon : m_playIcon;
		
		if (ImGui::ImageButton("##Play", (void*)(intptr_t)icon->rendererId(), {buttonSize, buttonSize}, {0, 1}, {1, 0}))
			a_sceneData->setRunning(!running);
	}
}
