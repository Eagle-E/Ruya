#ifndef ICOSAHEDRON_H
#define ICOSAHEDRON_H

#include "Object.h"


namespace ruya
{


	class Icosahedron : public ruya::Object
	{
	public: 
		Icosahedron();

	private:
		static std::shared_ptr<Mesh> init_mesh();
		static std::shared_ptr<Mesh> mMesh;
	};

}


#endif // !ICOSAHEDRON_H
