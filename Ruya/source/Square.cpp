#include "Square.h"


ruya::Square::Square()
{
	mMesh = {
		vec3(-0.5f, -0.5f, 0.0f),
		vec3(-0.5f,  0.5f, 0.0f),
		vec3( 0.5f,  0.5f, 0.0f),
		vec3( 0.5f, -0.5f, 0.0f)
	};
	set_mesh(&mMesh);
}

ruya::Square::~Square()
{

}
