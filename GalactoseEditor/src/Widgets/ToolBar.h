#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace Galactose {
	class Texture;
}

namespace GalactoseEditor {
	class EditorContext;

	class ToolBar {
	public:
		static const char* id() { return "##ToolBar"; }

		ToolBar();

		void update(const std::shared_ptr<EditorContext>& editorContext);
		
		float height() const;

	private:
		void insertIcon(const std::string& name);
		bool button(const std::string& name, const float x);

		std::unordered_map<std::string, std::shared_ptr<Galactose::Texture>> m_icons;
	};
}
