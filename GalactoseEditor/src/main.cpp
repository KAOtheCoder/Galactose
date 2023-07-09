#include "EditorLayer.h"
#include "HubLayer.h"

#include <Galactose/Core/Application.h>
#include <Galactose/Core/Window.h>
#include <Galactose/Renderer/Renderer.h>

int main(int argc, char** argv) {
	Galactose::Application app("Galactose", argc, argv);

	std::string projectFilePath;

	{
		auto window = Galactose::Window::create("Galactose Hub", 1366, 768);
		auto layer = std::make_shared<GalactoseEditor::HubLayer>(window.get());
		window->layers.push_back(layer);

		const int exitCode = app.exec();
		if (exitCode != 0)
			return exitCode;

		projectFilePath = layer->projectFilePath();
		layer.reset();
		window.reset();
	}

	if (!projectFilePath.empty()) {
		auto window = Galactose::Window::create("Galactose", 1920, 1080);
		auto renderer = Galactose::Renderer::create(window.get());
		window->layers.push_back(std::make_shared<GalactoseEditor::EditorLayer>(window.get(), projectFilePath));

		return app.exec();
	}

	return -1;
}
