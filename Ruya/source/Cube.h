#ifndef CUBE_H
#define CUBE_H

#include "Object.h"

namespace ruya
{

	class Cube : public ruya::Object
	{
	public:
		Cube();
		~Cube();

	private:
		static std::shared_ptr<Mesh> init_mesh();
		static std::shared_ptr<Mesh> mMesh;
	};

}
#endif
