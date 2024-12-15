#ifndef ICOSAHEDRON_H
#define ICOSAHEDRON_H

#include "engine/object.h"


namespace ruya
{


	class Icosahedron : public ruya::Object
	{
	public: 
		Icosahedron();
		void print_model_data() const;
		static std::shared_ptr<Mesh> create_icosahedron_mesh();
	private:
		static std::shared_ptr<Mesh> mMesh;
	};

}


#endif // !ICOSAHEDRON_H
