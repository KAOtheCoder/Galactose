#pragma once

#include "SideBarItem.h"

#include <string>
#include <unordered_map>

namespace Galactose {
	class Texture;
}

namespace GalactoseEditor {
	class EditorContext;

	class ToolBar : public SideBarItem {
	public:
		ToolBar(const std::shared_ptr<EditorContext>& editorContext);

		float size() const override;

		void update() override;
		
	private:
		void insertIcon(const std::string& name);
		bool button(const std::string& icon, const bool enabled = true);
		float buttonWidth() const;

		std::shared_ptr<EditorContext> m_editorContext;
		std::unordered_map<std::string, std::shared_ptr<Galactose::Texture>> m_icons;
	};
}
