#include "Square.h"
#include <memory>

ruya::Square::Square()
{
	mMesh.vertices = {
		vec3(-0.5f, -0.5f, 0.0f),
		vec3(-0.5f,  0.5f, 0.0f),
		vec3( 0.5f,  0.5f, 0.0f),
		vec3( 0.5f, -0.5f, 0.0f)
	};

	mMesh.textureCoordinates = {
		vec2(0.0f, 0.0f), // left bottom
		vec2(0.0f, 1.0f), // left top
		vec2(1.0f, 1.0f), // right top
		vec2(1.0f, 0.0f)  // right bottom
	};

	mMesh.faces = {
		uvec3(0, 1, 3),   // first triangle
		uvec3(1, 2, 3)    // second triangle
	};

	// pass pointer to parent class
	set_mesh(std::shared_ptr<Mesh>(&mMesh));
}

ruya::Square::~Square()
{

}
