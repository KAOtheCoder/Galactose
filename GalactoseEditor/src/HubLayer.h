#pragma once

#include "ImGuiLayer.h"

namespace GalactoseEditor {
	class HubLayer : public ImGuiLayer {
	public:
		HubLayer(Galactose::Window* window);

		std::string projectFilePath() { return m_projectPath; }

		void updateContent() override;

	private:
		std::string m_projectPath;
		Galactose::Window* m_window = nullptr;
	};
}
