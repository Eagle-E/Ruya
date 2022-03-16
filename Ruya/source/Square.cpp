#include "Square.h"
#include <memory>
#include <Texture.h>
#include "Mesh.h"

using ruya::Texture;
using ruya::Mesh;

std::shared_ptr<Mesh> ruya::Square::mMesh = init_square_mesh();

ruya::Square::Square()
{
	// pass pointer to parent class
	set_mesh(mMesh);
	mMesh->update_surface_normals();
}

ruya::Square::~Square()
{

}

std::shared_ptr<Mesh> ruya::Square::init_square_mesh()
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	mesh->vertices = {
		vec3(-0.5f, -0.5f, 0.0f),
		vec3(-0.5f,  0.5f, 0.0f),
		vec3(0.5f,  0.5f, 0.0f),
		vec3(0.5f, -0.5f, 0.0f)
	};

	mesh->textureCoordinates = {
		vec2(0.0f, 0.0f), // left bottom
		vec2(0.0f, 1.0f), // left top
		vec2(1.0f, 1.0f), // right top
		vec2(1.0f, 0.0f)  // right bottom
	};

	mesh->faces = {
		uvec3(0, 1, 3),   // first triangle
		uvec3(1, 2, 3)    // second triangle
	};

	return mesh;
}
