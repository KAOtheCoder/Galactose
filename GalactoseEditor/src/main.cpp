#include "EditorLayer.h"
#include "HubLayer.h"

#include <Galactose/Core/Application.h>
#include <Galactose/Core/Window.h>
#include <Galactose/Renderer/Renderer.h>

using namespace Galactose;
using namespace GalactoseEditor;

int main(int argc, char** argv) {
	Application app("Galactose", argc, argv);

	std::filesystem::path projectFilePath;

	{ // Open Hub
		const auto& window = Window::create("Galactose Hub", 1366, 768);
		window->layers.push_back(std::make_shared<HubLayer>(window.get()));

		const int exitCode = app.exec();
		if (exitCode != 0)
			return exitCode;

		if (!window->isClosed()) // layers cleared on close
			projectFilePath = static_cast<HubLayer*>(window->layers.front().get())->projectFilePath();
	}

	if (!projectFilePath.empty()) { // Open Editor
		auto window = Window::create("Galactose", 1920, 1080);
		auto renderer = Renderer::create(window.get());
		window->layers.push_back(std::make_shared<EditorLayer>(window.get(), projectFilePath));

		return app.exec();
	}

	return 0;
}
