#include <Galactose/Core/Application.h>
#include <Galactose/Core/Window.h>
#include <Galactose/Renderer/Renderer.h>

int main(int argc, char** argv) {
	Galactose::Application app("Galactose", argc, argv);

	auto window = Galactose::Window::create("Galactose", 1366, 768);
	auto renderer = Galactose::Renderer::create(window);

	return app.exec();
}