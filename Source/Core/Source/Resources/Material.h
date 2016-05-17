//
// Created by mrjaqbq on 13.05.16.
//

#ifndef GAME_MATERIAL_H
#define GAME_MATERIAL_H

#include <map>
#include <string>
#include <DefaultTrait.h>
#include "Resource.h"

namespace Core
{
	// uniform - pass name, get location
	// set value to location

	// when updating value - update buffer directly
	// then, on draw, only bind that buffer
	// without setting everything once again

	DECL_RESOURCE(Material)
	{
		// Shader only has program ID
		// this is where uniform buffers are stored


	};

	class MaterialTrait : public Utils::DefaultTrait<Material> {};
}

#endif //GAME_MATERIAL_H
