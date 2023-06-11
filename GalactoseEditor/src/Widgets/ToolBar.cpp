#include "ToolBar.h"
#include "EditorContext.h"

#include <Galactose/Renderer/Texture.h>

using namespace Galactose;

namespace GalactoseEditor {
	ToolBar::ToolBar()
	{
		insertIcon("Move");
		insertIcon("Rotate");
		insertIcon("Scale");
		insertIcon("Play");
		insertIcon("Pause");
	}

	void ToolBar::insertIcon(const std::string& a_name) {
		m_icons.emplace(a_name, Texture::create("assets/textures/" + a_name + ".png"));
	}

	float ToolBar::height() const {
		return (2.f * ImGui::GetFontSize()) + (2 * (ImGui::GetStyle().FramePadding.y + ImGui::GetStyle().WindowPadding.y));
	}

	bool ToolBar::button(const std::string& a_icon, const float a_x) {
		const float buttonSize = 2.f * ImGui::GetFontSize();
		
		if (a_x > ImGui::GetStyle().WindowPadding.x)
			ImGui::SameLine(a_x);
		return ImGui::ImageButton(("##" + a_icon).c_str(), (void*)(intptr_t)m_icons[a_icon]->rendererId(), { buttonSize, buttonSize }, { 0, 1 }, { 1, 0 });
	}

	void ToolBar::update(const std::shared_ptr<EditorContext>& a_editorContext) {
		const auto& style = ImGui::GetStyle();
		const float buttonSize = 2.f * (ImGui::GetFontSize() + style.FramePadding.x);

		std::array<std::pair<std::string, ImGuizmo::OPERATION>, 3> operations = { {
			{ "Move", ImGuizmo::TRANSLATE },
			{ "Rotate", ImGuizmo::ROTATE },
			{ "Scale", ImGuizmo::SCALE }
		} };

		float x = style.WindowPadding.x;

		for (const auto& operation : operations) {
			if (button(operation.first, x))
				a_editorContext->setManipulatorOperation(operation.second);

			x += buttonSize;// + style.WindowPadding.x;
		}

		const bool running = a_editorContext->isRunning();
		x = std::max(x, (ImGui::GetWindowSize().x - buttonSize) / 2);

		if (button(running ? "Pause" : "Play", x))
			a_editorContext->setRunning(!running);
	}
}
