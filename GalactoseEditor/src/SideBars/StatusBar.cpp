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
		case EditorContext::Info: color = { 1, 1, 1, 1 }; 
			break;
		case EditorContext::Warning: color = { 0, 1, 1, 1 };
			break;
		case EditorContext::Error: color = { 1, 0, 0, 1 };
			break;
		}

		ImGui::TextColored(color, m_editorContext->message().c_str());
	}
}
