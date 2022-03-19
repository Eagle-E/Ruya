#include "TestApplication.hpp"
#include "Window.h"

int main()
{
	ruya::Window window(1450, 875);
	window.make_context_current();
	
	ruya::TestApplication app(window);

	try
	{
		app.run();
	}
	catch (std::exception e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}



