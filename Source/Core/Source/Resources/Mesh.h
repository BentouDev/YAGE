//
// Created by MrJaqbq on 2016-02-10.
//

#ifndef VOLKHVY_MESH_H
#define VOLKHVY_MESH_H

#include <vector>

#include <glm/vec3.hpp>
#include "Resource.h"

namespace Core
{
	struct SubMesh
	{
		std::vector<glm::vec3> Position;
	};

	DECL_RESOURCE(Mesh)
	{
	public:
		~Mesh();

		std::vector<SubMesh> SubMeshes;
	};
}

#endif