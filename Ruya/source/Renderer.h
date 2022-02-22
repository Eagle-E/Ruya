#ifndef RENDERER_H
#define RENDERER_H

#include <unordered_map>
#include "Scene.h"
#include "UUID.h"
#include "glad/glad.h"

using std::unordered_map;

namespace ruya
{
	/*
	* A basic temporary renderer which will render a given scene
	*/
	class Renderer
	{
	public:
		void render_scene(Scene& scene);


	private:
		void render_object(Object& obj);
		unordered_map<UUID, GLuint> mBufferMap;
	};
}

#endif