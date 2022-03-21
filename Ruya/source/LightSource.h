#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "Object.h"
#include "UUID.h"
#include <glm/glm.hpp>

using glm::vec3;

namespace ruya
{
	
	class LightSource
	{
	public:
		LightSource() : mColor(1.0f) {}
		LightSource(const vec3& color) : mColor(color) {}

		// GETTERS
		vec3 position() const { return mModel.position(); }
		vec3 color() const { return mColor; }
		Object& model() { return mModel; }

		// SETTERS
		void set_color(const vec3& color) { mColor = color; }
		void set_position(const vec3& position) { mModel.set_position(position); }

	private:
		vec3 mColor;
		Object mModel;
		UUID mUUID;
	};


}


#endif