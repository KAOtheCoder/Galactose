#pragma once

#include <Galactose/Core/Layer.h>

namespace Galactose {
	class Window;
}

namespace GalactoseEditor {
	class ImGuiLayer : public Galactose::Layer {
	public:
		ImGuiLayer(Galactose::Window* window, 
			const bool dockingEnabled, 
			const bool viewportsEnabled, 
			const std::string& iniFile = ""
		);
		~ImGuiLayer() override;

		void update() override;
		virtual void updateContent() = 0;

	private:
		std::string m_iniFile;
	};
}
