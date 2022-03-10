#include "TestApplication.hpp"
#include "Window.h"

int main()
{
	ruya::Window window(720, 720);
	window.make_context_current();
	
	ruya::TestApplication app(window);
	app.run();

	// main loop
	
	return 0;
}



