#include "EditorLayer.h"

#include <Galactose/Core/Application.h>
#include <Galactose/Core/Window.h>
#include <Galactose/Renderer/Renderer.h>

int main(int argc, char** argv) {
	Galactose::Application app("Galactose", argc, argv);

	// TODO: open hub
	//const auto projectFilePath = argc > 1 ? argv[1] : "";
	const auto projectFilePath = "C:/dev/MyProject/MyProject.pro";
	auto window = Galactose::Window::create("Galactose", 1920, 1080);
	auto renderer = Galactose::Renderer::create(window);
	window->layers.push_back(std::make_shared<GalactoseEditor::EditorLayer>(window.get(), projectFilePath));

	return app.exec();
}
