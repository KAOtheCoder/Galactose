#pragma once

#include "Panel.h"

#include <filesystem>
#include <unordered_map>

namespace Galactose {
	class Texture;
}

namespace GalactoseEditor {
	class EditorContext;

	class AssetExplorer : public Panel {
	public:
		AssetExplorer(const std::shared_ptr<EditorContext>& editorContext);

		void onUpdate() override;
		//void onEvent(const std::shared_ptr<Galactose::Event>& a_event) override;

	private:
		std::shared_ptr<EditorContext> m_editorContext;
		std::filesystem::path m_directoryPath;
		std::unordered_map<std::string, std::shared_ptr<Galactose::Texture>> m_icons;
		float m_thumbnailSize = 0;
		std::filesystem::path m_contextFile;
	};
}
