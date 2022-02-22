#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>

using glm::vec4;	using glm::uvec3; // vec<uint32>
using glm::vec3;	using glm::mat4;
using glm::vec2;
using std::vector;


namespace ruya
{
	struct Mesh
	{
		vector<vec3> vertices;
		vector<uvec3> faces;
		vector<vec2> textureCoordinates;

		long int size() const;
		long int size_vertices() const;
		long int size_faces() const;
		long int size_texture_coords() const;
	};
}

#endif MESH_H