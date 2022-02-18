#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <list>
#include <vector>

namespace ruya
{
	/*
	* The Object class is for representing objects that are
	* going to be rendered on the scene. Each instance of 
	* this class needs a mesh to be rendered, so we don't
	* have duplicates of the same mesh for each reuse of 
	* the object.
	*/
	class Object
	{
	public:
		Object();
		~Object();

		glm::mat4 getModelMatrix();

		std::vector<glm::vec3>* getMesh() { return mMesh; }
		void setMesh(std::vector<glm::vec3>* mesh) { mMesh = mesh; }

	private:
		glm::vec3 mPosition; // position relative to parent or world coordinates.
		glm::vec4 mRotation;
		glm::vec3 mScale;
		std::vector<glm::vec3>* mMesh = nullptr;
	};
}


#endif
