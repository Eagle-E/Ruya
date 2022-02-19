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

	mTextureCoords = {
		vec2(0.0f, 0.0f), // left bottom
		vec2(0.0f, 1.0f), // left top
		vec2(1.0f, 1.0f), // right top
		vec2(1.0f, 0.0f)  // right bottom
	};
	set_texture_coordinates(&mTextureCoords);

	mFaces = {
		ivec3(0, 1, 3),   // first triangle
		ivec3(1, 2, 3)    // second triangle
	};
	set_mesh_faces(&mFaces);
}

ruya::Square::~Square()
{

}
