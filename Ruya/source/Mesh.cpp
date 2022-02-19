#include "Mesh.h"

namespace
{
    /*
    * Size of number-typed vector in bytes
    */
    template <class T>
    long int vector_size_in_bytes(const vector<T>& vec)
    {
        if (vec.size() == 0) return 0;
        else return vec.size() * sizeof(vec[0]);
    }
}

/*
* Size of mesh in bytes.
* Includes byte sizes of:
*   - vertices
*   - face indexes
*   - texture coordinates
*/
long int ruya::Mesh::size()
{
    long int total = 0;
    total += size_vertices();
    total += size_faces();
    total += size_texture_coords();
    return total;
}

/*
* Size of vertices in bytes
*/
long int ruya::Mesh::size_vertices()
{
    return vector_size_in_bytes(vertices);
}

/*
* Size of face indexes in bytes
*/
long int ruya::Mesh::size_faces()
{
	return vector_size_in_bytes(faces);
}

/*
* Size of texture coordinates in bytes
*/
long int ruya::Mesh::size_texture_coords()
{
	return vector_size_in_bytes(textureCoordinates);
}
