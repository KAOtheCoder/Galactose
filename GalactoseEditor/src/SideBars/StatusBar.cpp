#include "StatusBar.h"
#include "EditorContext.h"

namespace GalactoseEditor {
	StatusBar::StatusBar(const std::shared_ptr<EditorContext>& a_editorContext) :
		m_editorContext(a_editorContext)
	{}

	float StatusBar::size() const { 
		return ImGui::GetTextLineHeight() + 2 * ImGui::GetStyle().WindowPadding.y; 
	}

	void StatusBar::update() {
		ImVec4 color;

		switch (m_editorContext->messageType()) {
		case EditorContext::Info: color = ImGui::GetStyle().Colors[ImGuiCol_Text];
			break;
		case EditorContext::Warning: color = { 1, 0.8f, 0.01f, 1 };
			break;
		case EditorContext::Error: color = { 0.9f, 0.11f, 0.04f, 1 };
			break;
		}

		ImGui::TextColored(color, m_editorContext->message().c_str());
	}
}
