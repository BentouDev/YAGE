#ifndef EXTERNAL_OWNED_H
#define EXTERNAL_OWNED_H

#include "Utils/OwnedPtr.h"

struct ExternMockType;
struct Sth
{
	~Sth();
	Utils::owned_ptr < ExternMockType > ptr;
};

#endif // EXTERNAL_OWNED_H
