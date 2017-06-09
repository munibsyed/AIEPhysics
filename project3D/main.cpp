#include "Application3D.h"
#include <iostream>
int main() {

	// allocation
	auto app = new Application3D();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}