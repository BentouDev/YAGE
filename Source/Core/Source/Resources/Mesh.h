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
	DECL_RESOURCE(MeshResource)
	{
	public:
		~MeshResource();

	};

	struct SubMesh
	{
		std::vector<glm::vec3> Position;
	};

	struct MeshData
	{
		std::vector<SubMesh> SubMeshes;
		MeshResource::Handle Resource;
	};

}

#endif