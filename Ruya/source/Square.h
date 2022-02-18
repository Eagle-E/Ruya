#ifndef SQUARE_H
#define SQUARE_H

#include "Object.h"
#include <glm/glm.hpp>
#include <vector>

using std::vector;
using glm::vec3;

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
		vector<vec3> mMesh;
	};


}


#endif