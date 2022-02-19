#ifndef RENDERER_H
#define RENDERER_H

#include "Scene.h"

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

	};
}

#endif