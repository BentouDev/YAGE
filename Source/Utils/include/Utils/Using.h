//
// Created by mrjaqbq on 12.03.16.
//

#ifndef YAGE_USING_H_H
#define YAGE_USING_H_H

#include <functional>
#include "SafeDelete.h"

#define USING(type, name) for(auto name = new type; name != nullptr; Memory::SafeDelete(name))
#define TRY_USING(type, name, body) \
for(auto name = new type; name != nullptr; Memory::SafeDelete(name))\
try {\
	body \
} catch(...) {\
	Memory::SafeDelete(name); \
}

#endif //YAGE_USING_H_H
