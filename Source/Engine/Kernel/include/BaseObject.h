//
// Created by bentoo on 23/04/19.
//

#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "Utils/Handle.h"

namespace yage
{
	class BaseObject
	{
	public:
		virtual ~BaseObject() {}
	};

	class SafeObject : public BaseObject
	{
	public:
		virtual void cleanUp() {}
		Utils::RawHandle Handle;
	};
}

#endif // BASE_OBJECT_H