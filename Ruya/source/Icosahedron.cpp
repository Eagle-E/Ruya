#include "Icosahedron.h"
#include "Mesh.h"
#include <memory>


using ruya::Mesh;

std::shared_ptr<Mesh> ruya::Icosahedron::mMesh = init_mesh();

ruya::Icosahedron::Icosahedron()
{
	set_mesh(mMesh);
}

std::shared_ptr<Mesh> ruya::Icosahedron::init_mesh()
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	/*
		well make the icohasedron with 3 orthogonal golden rectangles as described in 
		https://en.wikipedia.org/wiki/Regular_icosahedron
		This image:
		https://en.wikipedia.org/wiki/File:Icosahedron-golden-rectangles.svg

		1)	There are three rectangles that are orthogonal, or in other words perpendicular to
			each other.
				* The first lies flat on the xz plane, its long side along the x-axis and the short
				  one along the z-axis. The center of the rectangle is at the origin (0,0).
				* The second one, in a similar manner, is on the xy plane with its length along the y-axis
				* The third one is on the yz plane with its length along the z-axis
		
		2)	The icosahedron will consist of the vertices of the rectangles. 
		3)	The faces will be determined as follows"
				* For each vertex, find the 5 closest vertices, these are its neighbors.
				* For each pair of neighbors that are closest with eachother make a triangle
				  with the original vertex and the pair of neighbors. There are a total of 5
				  closest pairs of neighbors per vertex on the icosahedron.
		
	*/ 

	float s1 = 1.0f; // short side
	float s2 = (1.0f + sqrt(5.0f)) / 2; // long side


	// rect 1
	mesh->vertices = {
		// rectangle 1
		vec3( -s2 / 2.0f,  0.0f,  s1/2.0f ),	// left front
		vec3( -s2 / 2.0f,  0.0f, -s1/2.0f),	// left back
		vec3(  s2 / 2.0f,  0.0f,  s1/2.0f),	// right back
		vec3(  s2 / 2.0f,  0.0f,  s1/2.0f),	// right front
	};


	return mesh;
}
