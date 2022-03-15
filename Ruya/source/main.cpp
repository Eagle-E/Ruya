#include "TestApplication.hpp"
#include "Window.h"

int main()
{
	ruya::Window window(1000, 1200);
	window.make_context_current();
	
	ruya::TestApplication app(window);
	app.run();

	// main loop
	
	return 0;
}



