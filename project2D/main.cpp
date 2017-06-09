#include "Application2D.h"
#include "PhysicsApplication2D.h"

int main() {
	
	// allocation
	auto app = new PhysicsApplication2D();
	
	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}