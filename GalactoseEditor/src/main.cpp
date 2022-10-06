#include <Core/Application.h>
#include <Core/Window.h>
#include <Renderer/Renderer.h>

int main(int argc, char** argv) {
	Galactose::Application app("Galactose", argc, argv);

	auto window = Galactose::Window::create("Galactose", 1366, 768);
	auto renderer = Galactose::Renderer::create(window);

	return app.exec();
}