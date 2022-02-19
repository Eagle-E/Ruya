#include "Renderer.h"
#include <list>

using std::list;

void ruya::Renderer::render_scene(Scene& scene)
{
	list<Object*>& objects = scene.get_scene_objects();
	for (Object* obj : objects)
	{
		render_object(*obj);
	}
}

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
}
