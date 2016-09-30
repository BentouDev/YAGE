//
// Created by mrjaqbq on 13.05.16.
//

#ifndef GAME_MATERIAL_H
#define GAME_MATERIAL_H

#include <map>
#include <string>
#include <DefaultTrait.h>
#include "Resources/Resource.h"

#include <vector>
#include <map>

namespace Core
{
	// uniform - pass name, get location
	// set value to location

	// when updating value - update buffer directly
	// then, on draw, only bind that buffer
	// without setting everything once again

	struct Uniform
	{
		uint32_t location;
		type_t type;
	};

	DECL_RESOURCE(Material)
	{
	protected:
		// Shader only has program ID
		// this is where uniform buffers are stored

		std::map<std::string, uint32_t > uniformMap;
		std::vector<Uniform> uniforms;

	public:
	};

	class MaterialTrait : public Utils::DefaultTrait<Material> {};
}

#endif //GAME_MATERIAL_H
