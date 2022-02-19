#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <list>
#include <vector>
#include "UUID.h"

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::ivec3;
using glm::mat4;

using std::vector;
using std::list;

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
		vector<vec3>* mesh() { return mMesh; }
		vector<ivec3>* mesh_faces() { return mMeshFaces; }
		vector<vec2>* texture_coordinates() { return mTextureCoords; }
		int mesh_size_in_bytes();
		int mesh_faces_size_in_bytes();
		int texture_coords_size_in_bytes();
		mat4 model_matrix();

		// MANIPULATORS
		void set_mesh(vector<vec3>* mesh) { mMesh = mesh; }
		void set_mesh_faces(vector<ivec3>* faces) { mMeshFaces = faces; }
		void set_texture_coordinates(vector<vec2>* textureCoords) { mTextureCoords = textureCoords; }
		void add_child(Object* obj);
		bool remove_child(Object& obj);

		// OPERATORS
		bool operator==(const Object& other);

	private:
		vec3 mPosition; // position relative to parent or world coordinates.
		vec4 mRotation;
		vec3 mScale;
		vec3 mColor;
		vector<vec3>* mMesh = nullptr;  // TODO: make mesh object where mesh vertices and faces will be stored
		vector<ivec3>* mMeshFaces = nullptr;
		vector<vec2>* mTextureCoords = nullptr;
		Object* mParent;
		list<Object*> mChildren;
		UUID mUUID;

		// private helper functions
		void dislodge_from_parent();
	};
}


#endif
