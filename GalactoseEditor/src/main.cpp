#include <iostream>
#include <Galactose/Core/Application.h>

int main(int argc, char** argv) {
	std::cout << "Hello Galactose" << std::endl;
	Galactose::Application app("Galactose", argc, argv);

	return app.exec();
}