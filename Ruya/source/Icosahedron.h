#ifndef ICOSAHEDRON_H
#define ICOSAHEDRON_H

#include "Object.h"


namespace ruya
{


	class Icosahedron : public ruya::Object
	{
	public: 
		Icosahedron();
		void print_model_data() const;
	private:
		static std::shared_ptr<Mesh> init_mesh();
		static std::shared_ptr<Mesh> mMesh;
	};

}


#endif // !ICOSAHEDRON_H
