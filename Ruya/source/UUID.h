#ifndef _UUID_H_
#define _UUID_H_

#include <cstdint>

namespace ruya
{
	/*
	* Universal Unique ID.
	* Will be used to distinguish between entities such as Objects, ...
	*/
	class UUID
	{
	public:
		UUID();
		bool operator==(const UUID& other) const;

	private:
		uint64_t mUUID;
	};
}

#endif