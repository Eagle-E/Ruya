#include "Renderer.h"
#include <list>
#include "Texture.h"

using std::list;

ruya::Renderer::Renderer(Shader& shader)
	: mIndexVertexAttrib(0), mIndexTextureAttrib(1), mShader(shader)
{

}

void ruya::Renderer::render_scene(Scene& scene)
{
	// make shader current
	mShader.use();

	// render scene objects
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
	// TODO:	when a mesh gets deleted that is in the unordered_map, it needs to be removed.
	//			Consider attaching a custom destroctor on the shared_ptr that is being used as
	//			key, that will remove the 'null' shared_ptr.

	// [1] Check if the Mesh of the object already had been buffered, if not create the buffers
	if (mMeshVaoMap.find(obj.mesh()) == mMeshVaoMap.end())
	{
		GLuint meshVaoID = buffer_mesh(*obj.mesh());
		mMeshVaoMap[obj.mesh()] = meshVaoID;
	}
	
	// [2] Bind the textures and set their uniform location
	if (obj.texture())
	{
		GLuint textureSlot = mSlotManager.bind_texture(*obj.texture());
		mShader.setInt("ourTexture", textureSlot - GL_TEXTURE0); // TODO: save texture uniform name in Shader class instead of hardcoding
	}

	// [3] Bind the vao and render
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
	glVertexAttribPointer(mIndexVertexAttrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // loc data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(mIndexTextureAttrib, 2, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.size_vertices())); // texture data
	glEnableVertexAttribArray(1);

	return vaoID;
}


/************************************************************************************************
*
*	CLASS TextureSlotManager
*	 
************************************************************************************************/
ruya::Renderer::TextureSlotManager::TextureSlotManager()
{
	// init slot maps and priority list
	int fragShaderMaxSlots = Texture::get_num_texture_slots_fragment_shader();
	for (int i = GL_TEXTURE0; i < GL_TEXTURE0 + fragShaderMaxSlots; i++)
	{
		// init all slots to 0 (= contains no texture)
		mSlotTextureMap[i] = 0; 
		// init priorities in numerical order
		list<GLuint>::iterator slotIter = mSlotPriority.insert(mSlotPriority.end(), i); 
		// init slot iterators
		mSlotPriorityRefMap[i] = slotIter;
	}
}

/*
* Binds given texture to one of the slots and returns the slot number so that the
* caller set the uniform location of the texture sampler to the correct slot number.
* 
* @pre: the given texture must have already been created with glTexImage2D()
*		(which is done by default by the Texture class' constructor)
*/
GLuint ruya::Renderer::TextureSlotManager::bind_texture(const ruya::Texture& texture)
{
	// Check whether the texture is already bound
	if (mTextureSlotMap[texture.ID()] == 0)
	{
		// free new slot and set map values to new texture id
		GLuint newSlot = free_slot();
		mSlotTextureMap[newSlot] = texture.ID();
		mTextureSlotMap[texture.ID()] = newSlot;

		// make new slot top priority
		set_top_priority(mSlotPriorityRefMap[newSlot]);

		// bind texture to new slot
		glActiveTexture(newSlot);
		glBindTexture(GL_TEXTURE_2D, texture.ID());
	}
	else
	{
		// texture is already bound to a slot, increment slot priority
		GLuint textureSlot = mTextureSlotMap[texture.ID()];
		increment_priority(mSlotPriorityRefMap[textureSlot]);
	}

	// return slot number the texture has been (or was already) bound to
	return mTextureSlotMap[texture.ID()];
}

/*
* Frees a texture slot to be used by a new Texture.
*	Is responsible for internal state changes of TextureSlotManager() when freeing a slot
* @returns slot number that has been freed.
*/
GLuint ruya::Renderer::TextureSlotManager::free_slot()
{
	// get number of least priority slot
	GLuint slot = mSlotPriority.back();

	// unregister texture residing in that slot
	GLuint oldTextureId = mSlotTextureMap[slot];
	mTextureSlotMap[oldTextureId] = 0; // tex id might be 0 (invalid) but does no harm

	// mark texture slot as free by setting its mapped texture value to 0
	mSlotTextureMap[slot] = 0;

	// return slot num
	return slot;
}

/*
* Moves given slot up one position in the priority list.
* (this is done when an already-binded texture is being rendered again)
*/
void ruya::Renderer::TextureSlotManager::increment_priority(list<GLuint>::iterator& slotIt)
{
	// can't increment position if it's the first in the list
	if (slotIt == mSlotPriority.begin())
		return;

	// swap values of elements at pos "slotIt" and the one before
	GLuint temp1 = *slotIt;
	slotIt--;
	GLuint temp2 = *slotIt;
	*slotIt = temp1;
	slotIt++;
	*slotIt = temp2;
}

/*
* Makes given slot from the mSlotPriority list top priority by placing it at the front of the list.
* @post: slotIt has been updated but is still pointing to the same slot (that is now at the front of the priority list)
*/
void ruya::Renderer::TextureSlotManager::set_top_priority(list<GLuint>::iterator& slotIt)
{
	// save slot number and remove slot from priority list
	GLuint slot = *slotIt;
	mSlotPriority.erase(slotIt);

	// reinsert slot to the front
	mSlotPriority.push_front(slot);

	// update slotIt iterator since it was invalidated with the erase operation
	slotIt = mSlotPriority.begin();
}




