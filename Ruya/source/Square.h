#ifndef SQUARE_H
#define SQUARE_H

#include "Object.h"
#include <glm/glm.hpp>
#include <vector>

using std::vector;
using glm::vec3;
using glm::vec2;
using glm::uvec3;

namespace ruya
{

	/*
	* Just a square object.
	*/
	class Square : public ruya::Object
	{
	public:
		Square();
		~Square();

	private:
		static std::shared_ptr<Mesh> init_square_mesh();
		static std::shared_ptr<Mesh> mMesh;
	};


}


#endif