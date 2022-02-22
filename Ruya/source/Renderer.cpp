#include "Renderer.h"
#include <list>

using std::list;

ruya::Renderer::Renderer()
	: mIndexVertexAttrib(0), mIndexTextureAttrib(1)
{

}

void ruya::Renderer::render_scene(Scene& scene)
{
	// TODO: add code to use this renderers shader program (opengl call)

	list<Object*>& objects = scene.get_scene_objects();
	for (Object* obj : objects)
	{
		render_object(*obj);
	}
}

/*
* Handles the necessary OpenGL calls to render the object with the shader program
* that this Renderer has. If the Mesh of the Object is being rendered for the first
* time, the necessary buffers (VAO, VBO & EBO) will be created automatically.
* 
* @pre the correct shader program needs to be made current before calling this function.
*/
void ruya::Renderer::render_object(Object& obj)
{
	/*
		TODO:
			* use std::unordered_map to keep track of VAO/Object pairs
				- to start off just make a separate VBO, EBO and VAO for each object
				- if the buffers for a to-be-rendered objects are created for the first 
					time, add its UUID to the map as key with its VAO ID as value. When 
					it needs to be re-rendered query the VAO ID, bind and render.
			* use a FIFO queue to manage the available texture slots. 
				- create a Texture class (that'll just contain the texture image, normals, etc.)
				- keep a map of texture UUID/texture slot pairs. This way you can check if the 
					texture of an object has been loaded into a slot.
				- pass the texture slot number via a uniform to the fragment shader when rendering
					the object.
	*/
	
	// TODO:	when a mesh gets deleted that is in the unordered_map, it needs to be removed.
	//			Consider attaching a custom destroctor on the shared_ptr that is being used as
	//			key, that will remove the 'null' shared_ptr.

	// [1] Check if the Mesh of the object already had been buffered, if not create the buffers
	if (mMeshVaoMap.find(obj.mesh()) == mMeshVaoMap.end())
	{
		GLuint meshVaoID = buffer_mesh(*obj.mesh());
		mMeshVaoMap[obj.mesh()] = meshVaoID;
	}
	
	// [2] Bind the vao and render
	size_t numIndexes = obj.mesh()->faces.size() * 3;
	glBindVertexArray(mMeshVaoMap[obj.mesh()]);
	glDrawElements(GL_TRIANGLES, numIndexes, GL_UNSIGNED_INT, 0);
}

/*
* Does the necessary OpenGL calls the create VAO, VBO and EBO for the given mesh.
* 
* 
* @returns the VAO ID that contains info about the buffers containing the mesh's data.
*/
GLuint ruya::Renderer::buffer_mesh(const Mesh& mesh)
{
	// create a vertex buffer object (VAO) so we don't have to repeat VBO and vertex attribute stuff
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// element buffer
	GLuint eboID;
	glGenBuffers(1, &eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.size_faces(), mesh.faces.data(), GL_STATIC_DRAW);

	// vertex buffer, concatenate mesh data: first vertex data then texture data, ...
	GLuint vboID;
	glGenBuffers(1, &vboID); // create a buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboID); // set buffer's type to array buffer
	glBufferData(GL_ARRAY_BUFFER, mesh.size(), NULL, GL_STATIC_DRAW); // allocate memory and copy vertices to GPU
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.size_vertices(), mesh.vertices.data());
	glBufferSubData(GL_ARRAY_BUFFER, mesh.size_vertices(), mesh.size_texture_coords(), mesh.textureCoordinates.data());

	// specify vertex attributes, how the data in the VBO should be evaluated
	// UPDATED according to the concatenated data format
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // loc data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.size_vertices())); // texture data
	glEnableVertexAttribArray(1);

	return vaoID;
}
