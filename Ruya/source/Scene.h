#ifndef SCENE_H
#define SCENE_H

#include <list>
#include "Object.h"

using std::list;

namespace ruya
{
	/*
	* Represents the to-be-rendered scene.
	*/
	class Scene
	{
	public:
		Scene();
		~Scene();

		list<Object*>& get_scene_objects() { return mObjects; }
		void add_object(Object* obj);
		//bool remove_object(Object* obj); // TODO: is this necessary?

	private:
		list<Object*> mObjects;
	};
}

#endif