//
// Created by bentoo on 19.05.17.
//

#ifndef YAGE_MODEL_H
#define YAGE_MODEL_H

#include <Utils/List.h>
#include <Utils/Handle.h>

#include "Core/Resources/Resource.h"

namespace Logic
{
	class Entity;
}

namespace Resources
{
	class Mesh;

	// Wraps around entity
	DECL_RESOURCE(Model)
	{
	public:
		using mesh_t   = Utils::Handle<Resources::Mesh>;

		Utils::List<mesh_t> 			meshes;

		// Shouldn't this be a prefab ?
		Utils::Handle<Logic::Entity> 	entity;

	public:
		explicit Model();
		virtual ~Model();


	};
}

#endif //YAGE_MODEL_H
