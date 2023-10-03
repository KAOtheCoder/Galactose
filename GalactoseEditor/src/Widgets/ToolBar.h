#pragma once

#include "EditorContext.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace Galactose {
	class Texture;
}

namespace GalactoseEditor {
	class ToolBar {
	public:
		static const char* id() { return "##ToolBar"; }

		ToolBar(const std::shared_ptr<EditorContext>& editorContext);

		void update();
		
		float height() const;

	private:
		void insertIcon(const std::string& name);
		bool button(const std::string& icon, const bool enabled = true);
		float buttonWidth() const;

		std::shared_ptr<EditorContext> m_editorContext;
		std::unordered_map<std::string, std::shared_ptr<Galactose::Texture>> m_icons;
	};
}
