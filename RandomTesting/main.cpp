#include <stdio.h>
#include "other.h"

namespace
{
	int b;
}

static int c;

void local_function()
{
	printf("This is the local function in the main file\n");
}

int main()
{

	static_func();
	static_func();
	static_func();
	static_func();
	local_function();
	return 0;
}