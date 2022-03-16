#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <list>
#include <memory>
#include <vector>
#include "UUID.h"
#include "Mesh.h"
#include "Texture.h"

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
	* 
	* TODO: consider changing the return type of the getters for the Mesh 
	*		and Texture (mesh(), texture()) from shared_ptr<> to a 
	*		const reference to their pointers, like: const (Mesh*) & mesh()
	*		This way the mesh can be accessed through a pointer, and we can
	*		also evaluate the state of the pointer itself. So when the owner
	*		of the mesh deletes it (delete mMesh) and sets it to nullptr, then
	*		everyone possessing the reference to the pointer can check whether
	*		the pointer has become a nullptr, and if so discard it so avoid 
	*		memory violations.
	*		Another benefit is that since it's a reference, we are sure it is
	*		an actual pointer pointing to a Mesh obj (unless it's been set to
	*		nullptr). And the 'const' keyword ensures that the pointer itself
	*		cannot be altered by the users or observers of that reference to 
	*		the pointer.
	*		Downsides:
	*			* return type is less readable
	*			* users can force a cast to bypass the const, but this is kind
	*			  of irrelevant because if you do something that is not supposed
	*			  to be done, you can expect errors to happen. shared_ptr is the
	*			  safe variant in this case but comes with additional overhead.
	*		
	*/
	class Object
	{
	public:
		// CONSTRUCTORS & DESTRUCTOR
		Object();
		~Object();

		// GETTERS & QUERIES
		shared_ptr<Mesh> mesh() { return mMesh; }
		shared_ptr<Texture> texture() { return mTexture; }
		mat4 model_matrix();
		vec3 color() const { return mColor; }
		vec3 position() const { return mPosition; }

		// MANIPULATORS
		inline void set_mesh(const shared_ptr<Mesh>& mesh) { mMesh = mesh; }
		inline void set_texture(const shared_ptr<Texture>& texture) { mTexture = texture; }
		void set_position(const glm::vec3& position) { mPosition = position; }
		void set_scale(const glm::vec3& scale) { mScale = scale; }
		void set_color(const glm::vec3& color) { mColor = color; }
		void set_rotation(const glm::vec3& rotation) { mRotation = rotation; } // resets rotation to given amount per axis
		
		void rotate(float x, float y, float z);
		void rotate_x(float degrees) { mRotation.x = fmod(mRotation.x + degrees, 360); } // idem rotate() but on 1 axis
		void rotate_y(float degrees) { mRotation.y = fmod(mRotation.y + degrees, 360); } 
		void rotate_z(float degrees) { mRotation.z = fmod(mRotation.z + degrees, 360); }

		void add_child(Object* obj);
		bool remove_child(Object& obj);

		// OPERATORS
		bool operator==(const Object& other);

	private:
		vec3 mPosition; // position relative to parent or world coordinates.
		vec3 mRotation;
		vec3 mScale;
		vec3 mColor;
		shared_ptr<Mesh> mMesh; // vertices, faces, texture coords
		shared_ptr<Texture> mTexture; // vertices, faces, texture coords

		Object* mParent;
		list<Object*> mChildren;
		UUID mUUID;

		// private helper functions
		void dislodge_from_parent();
	};
}


#endif
