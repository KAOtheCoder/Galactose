#include <iostream>
#include <Galactose/Core/Application.h>
#include <Galactose/Core/Window.h>

int main(int argc, char** argv) {
	std::cout << "Hello Galactose" << std::endl;
	Galactose::Application app("Galactose", argc, argv);

	auto window2 = Galactose::Window::create("Galactose", 1600, 900);
	auto window = Galactose::Window::create("Galactose", 1366, 768);

	return app.exec();
}