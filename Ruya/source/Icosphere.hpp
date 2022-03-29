#ifndef ICOSPHERE_H
#define ICOSPHERE_H

#include "Object.h"
#include "Icosahedron.h"
#include <unordered_map>

using std::unordered_map;
using std::vector;

namespace ruya
{
	class Icosphere : public Object
	{
	public:
		Icosphere()
		{
			set_mesh(mMesh);
		}

		~Icosphere()
		{

		}

	private:
		/*
		* Basically: 
		*		take an icosahedron, divide each of its triangles into 4 smaller triangles by
		*		adding a new vertex at the center of each edge and forming new triangles with the
		*		old and new vertices. When the new triangles are created, normalize each vertex
		*		to make the result more spherish.
		* 
		*		Repeat this a couple times to get more detail
		*/
		static std::shared_ptr<Mesh> init_mesh()
		{
			std::shared_ptr<Mesh> icoMesh = Icosahedron::create_icosahedron_mesh();
			const int LEVEL_OF_DETAIL = 1; // amount of iterations of refining the icosahedron into a sphere

			auto contains = [](const vector<vec3>& arr, const vec3 &v)
			{
				for (const vec3& e : arr)
					if (e == v) return true;
				return false;
			};

			auto add_if_not_added_and_return_index = [](vector<vec3>& arr, const vec3& v) -> int
			{
				for (int i = 0; i < arr.size(); i++)
				{
					if (arr[i] == v)
						return i;
				}

				arr.push_back(v);
				return arr.size() - 1;
			};

			for (int i = 0; i < LEVEL_OF_DETAIL; i++)
			{
				vector<uvec3> newFaces;
				for (uvec3& face : icoMesh->faces)
				{
					// get the face vertices and make new ones that are
					// in the middle of each edge of the triangle
					vec3 v0 = icoMesh->vertices[face[0]];
					vec3 v1 = icoMesh->vertices[face[1]];
					vec3 v2 = icoMesh->vertices[face[2]];
					vec3 v3 = (v0 + v1) / 2.0f;
					vec3 v4 = (v1 + v2) / 2.0f;
					vec3 v5 = (v2 + v0) / 2.0f;

					// indexes of the vertices
					unsigned int iv0 = face[0];
					unsigned int iv1 = face[1];
					unsigned int iv2 = face[2];
					unsigned int iv3 = add_if_not_added_and_return_index(icoMesh->vertices, v3);
					unsigned int iv4 = add_if_not_added_and_return_index(icoMesh->vertices, v4);
					unsigned int iv5 = add_if_not_added_and_return_index(icoMesh->vertices, v5);
					
					// add 4 new faces
					newFaces.push_back(uvec3(iv0, iv3, iv5)); // top triangle
					newFaces.push_back(uvec3(iv3, iv4, iv5)); // center
					newFaces.push_back(uvec3(iv3, iv1, iv4)); // right
					newFaces.push_back(uvec3(iv5, iv4, iv2)); // left
				}

				// update the "faces" array
				icoMesh->faces.assign(newFaces.begin(), newFaces.end());
				
				// normalize all vertices
				for (vec3& v : icoMesh->vertices)
					v = glm::normalize(v);
			}

			// init vertex normals: for all vertices get sum of all surface normals of the faces
			// it is part of then divide it by the number of faces it is part of
			icoMesh->normals.clear();
			icoMesh->normals.resize(icoMesh->vertices.size(), vec3(0));
			vector<int> useCounts(icoMesh->vertices.size(), 0);

			for (const uvec3& face : icoMesh->faces)
			{
				// calculate normal of face
				vec3 v0 = icoMesh->vertices[face[0]];
				vec3 v1 = icoMesh->vertices[face[1]];
				vec3 v2 = icoMesh->vertices[face[2]];
				vec3 normal = glm::normalize(v0 + v1 + v2);

				// add normal to all vertices of the face
				icoMesh->normals[face[0]] += normal;
				icoMesh->normals[face[1]] += normal;
				icoMesh->normals[face[2]] += normal;

				// increment the counts of the vertices 
				useCounts[face[0]]++;
				useCounts[face[1]]++;
				useCounts[face[2]]++;
			}

			// normalize
			for (int i = 0; i < icoMesh->normals.size(); i++)
				icoMesh->normals[i] /= useCounts[i];

			return icoMesh;
		}

		static std::shared_ptr<Mesh> mMesh;
	};

	// init static vars
	std::shared_ptr<Mesh> ruya::Icosphere::mMesh = init_mesh();

}


#endif
