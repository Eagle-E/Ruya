#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <list>
#include <memory>
#include <vector>
#include "UUID.h"
#include "Mesh.h"

using glm::vec4;
using glm::vec3;
using glm::mat4;

using std::list;
using std::shared_ptr;
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
		// CONSTRUCTORS & DESTRUCTOR
		Object();
		~Object();

		// GETTERS & QUERIES
		shared_ptr<Mesh> mesh() { return mMesh; }
		mat4 model_matrix();

		// MANIPULATORS
		void set_mesh(shared_ptr<Mesh>& mesh) { mMesh = mesh; }
		void add_child(Object* obj);
		bool remove_child(Object& obj);

		// OPERATORS
		bool operator==(const Object& other);

	private:
		vec3 mPosition; // position relative to parent or world coordinates.
		vec4 mRotation;
		vec3 mScale;
		vec3 mColor;
		shared_ptr<Mesh> mMesh; // vertices, faces, texture coords

		Object* mParent;
		list<Object*> mChildren;
		UUID mUUID;

		// private helper functions
		void dislodge_from_parent();
	};
}


#endif
