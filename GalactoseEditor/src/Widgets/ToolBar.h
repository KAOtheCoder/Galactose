#pragma once

#include <memory>

namespace Galactose {
	class Texture;
}

namespace GalactoseEditor {
	class EditorSceneData;

	class ToolBar {
	public:
		static const char* id() { return "##ToolBar"; }

		ToolBar();

		void update(const std::shared_ptr<EditorSceneData>& sceneData);
		
		float height() const;

	private:
		std::shared_ptr<Galactose::Texture> m_playIcon;
		std::shared_ptr<Galactose::Texture> m_pauseIcon;
	};
}
