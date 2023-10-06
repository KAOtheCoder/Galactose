#include "ToolBar.h"
#include "EditorContext.h"

#include <Galactose/Renderer/Texture.h>
#include <Galactose/Scene/Scene.h>

#include <array>

using namespace Galactose;

namespace GalactoseEditor {
	ToolBar::ToolBar(const std::shared_ptr<EditorContext>& a_editorContext) :
		m_editorContext(a_editorContext)
	{
		insertIcon("Move");
		insertIcon("Rotate");
		insertIcon("Scale");
		insertIcon("Play");
		insertIcon("Pause");
		insertIcon("Stop");
	}

	void ToolBar::insertIcon(const std::string& a_name) {
		m_icons.emplace(a_name, Texture::create("assets/textures/" + a_name + ".png"));
	}

	float ToolBar::size() const {
		const auto& style = ImGui::GetStyle();
		return (2.f * ImGui::GetFontSize()) + (2 * (style.FramePadding.y + style.WindowPadding.y));
	}

	float ToolBar::buttonWidth() const {
		return  2.f * (ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.x);
	}

	bool ToolBar::button(const std::string& a_icon, const bool a_enabled) {
		const float iconSize = 2.f * ImGui::GetFontSize();
		
		if (!a_enabled)
			ImGui::BeginDisabled();

		const auto cursorPos = ImGui::GetCursorPos();
		const bool clicked = ImGui::ImageButton(("##" + a_icon).c_str(), (void*)(intptr_t)m_icons[a_icon]->rendererId(), { iconSize, iconSize }, { 0, 1 }, { 1, 0 });

		if (!a_enabled)
			ImGui::EndDisabled();

		ImGui::SetCursorPos({ cursorPos.x + iconSize + 2.f * ImGui::GetStyle().FramePadding.x, cursorPos.y });

		return clicked;
	}

	void ToolBar::update() {
		std::array<std::pair<std::string, ImGuizmo::OPERATION>, 3> operations = { {
			{ "Move", ImGuizmo::TRANSLATE },
			{ "Rotate", ImGuizmo::ROTATE },
			{ "Scale", ImGuizmo::SCALE }
		} };

		for (const auto& operation : operations)
			if (button(operation.first))
				m_editorContext->setManipulatorOperation(operation.second);

		ImGui::SetCursorPosX(std::max(ImGui::GetCursorPosX(), (ImGui::GetWindowSize().x - 3 * buttonWidth()) / 2));
		const auto state = m_editorContext->state();

		if (button("Play", state != EditorContext::Playing))
			m_editorContext->setState(EditorContext::Playing);

		if (button("Pause", state == EditorContext::Playing))
			m_editorContext->setState(EditorContext::Paused);

		if (button("Stop", state != EditorContext::Stopped))
			m_editorContext->setState(EditorContext::Stopped);
	}
}
