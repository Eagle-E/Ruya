#ifndef RENDERER_H
#define RENDERER_H

#include <unordered_map>
#include <memory>
#include "Scene.h"
#include "Mesh.h"
#include "glad/glad.h"

using std::unordered_map;
using std::shared_ptr;

namespace ruya
{
	/*
	* A basic temporary renderer which will render a given scene
	*/
	class Renderer
	{
	public:
		Renderer();
		void render_scene(Scene& scene);
		void render_object(Object& obj);


	private:
		GLuint buffer_mesh(const Mesh& mesh);

		unordered_map<shared_ptr<Mesh>, GLuint> mMeshVaoMap;
		GLuint mIndexVertexAttrib; // indexes of the attributes used in the vertex shader
		GLuint mIndexTextureAttrib;
	};
}

#endif