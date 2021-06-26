#include <Galactose/Core/Application.h>
#include <Galactose/Core/Window.h>

int main(int argc, char** argv) {
	Galactose::Application app("Galactose", argc, argv);

	auto window = Galactose::Window::create("Galactose", 1366, 768);

	return app.exec();
}