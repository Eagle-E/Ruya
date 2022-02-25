#include "Cube.h"
#include "Mesh.h"
#include "Texture.h"
#include <memory>

using ruya::Mesh;
using ruya::Texture;




std::shared_ptr<Mesh> ruya::Cube::mMesh = init_mesh();


ruya::Cube::Cube()
{
	set_mesh(mMesh);
}

ruya::Cube::~Cube()
{
}

std::shared_ptr<Mesh> ruya::Cube::init_mesh()
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	
	

	mesh->vertices = {
		vec3( 0.5f, -0.5f, -0.5f), 
		vec3( 0.5f, -0.5f, -0.5f), 
		vec3( 0.5f,  0.5f, -0.5f), 
		vec3( 0.5f,  0.5f, -0.5f), 
		vec3(-0.5f,  0.5f, -0.5f), 
		vec3(-0.5f, -0.5f, -0.5f), 

		vec3(-0.5f, -0.5f,  0.5f), 
		vec3( 0.5f, -0.5f,  0.5f),  
		vec3( 0.5f,  0.5f,  0.5f),  
		vec3( 0.5f,  0.5f,  0.5f),  
		vec3(-0.5f,  0.5f,  0.5f),  
		vec3(-0.5f, -0.5f,  0.5f),  

		vec3(-0.5f,  0.5f,  0.5f),  
		vec3(-0.5f,  0.5f, -0.5f),  
		vec3(-0.5f, -0.5f, -0.5f),  
		vec3(-0.5f, -0.5f, -0.5f),  
		vec3(-0.5f, -0.5f,  0.5f),  
		vec3(-0.5f,  0.5f,  0.5f),  

		vec3( 0.5f,  0.5f,  0.5f),  
		vec3( 0.5f,  0.5f, -0.5f),  
		vec3( 0.5f, -0.5f, -0.5f),  
		vec3( 0.5f, -0.5f, -0.5f),  
		vec3( 0.5f, -0.5f,  0.5f),  
		vec3( 0.5f,  0.5f,  0.5f),  

		vec3(-0.5f, -0.5f, -0.5f),  
		vec3( 0.5f, -0.5f, -0.5f),  
		vec3( 0.5f, -0.5f,  0.5f),  
		vec3( 0.5f, -0.5f,  0.5f),  
		vec3(-0.5f, -0.5f,  0.5f),  
		vec3(-0.5f, -0.5f, -0.5f),  

		vec3(-0.5f,  0.5f, -0.5f),  
		vec3( 0.5f,  0.5f, -0.5f),  
		vec3( 0.5f,  0.5f,  0.5f),  
		vec3( 0.5f,  0.5f,  0.5f),  
		vec3(-0.5f,  0.5f,  0.5f),  
		vec3(-0.5f,  0.5f, -0.5f)   
	};

	mesh->faces = {
		uvec3(0, 1, 2), 
		uvec3(3, 4, 5),
		uvec3(6, 7, 8), 
		uvec3(9,10,11),
		uvec3(12,13,14), 
		uvec3(15,16,17),
		uvec3(18,19,20), 
		uvec3(21,22,23),
		uvec3(24,25,26),
		uvec3(27,28,29),
		uvec3(30,31,32),
		uvec3(33,34,35)
	};

	mesh->textureCoordinates = {
		vec2(0.0f, 0.0f), // left bottom
		vec2(1.0f, 0.0f), // left top
		vec2(1.0f, 1.0f), // right top
		vec2(1.0f, 1.0f),  // right bottom
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f),
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f),
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(0.0f, 1.0f),
		vec2(1.0f, 1.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(0.0f, 1.0f),
		vec2(0.0f, 1.0f),
		vec2(1.0f, 1.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(0.0f, 1.0f)
	};

	return mesh;
}
