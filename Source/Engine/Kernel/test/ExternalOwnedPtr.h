#ifndef EXTERNAL_OWNED_H
#define EXTERNAL_OWNED_H

#include "Utils/OwnedPtr.h"

struct MockValue;
struct Sth
{
	~Sth();
	Utils::owned_ptr < MockValue > ptr;
};

#endif // EXTERNAL_OWNED_H
