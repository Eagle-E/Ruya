#include "other.h"

namespace
{
	int b;
}
static int c;

void static_func()
{
	printf("This function is static and should not be seen\n");
}

static void local_function()
{
	printf("This is weird.\n");
}