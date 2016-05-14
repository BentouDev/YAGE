//
// Created by MrJaqbq on 2016-02-10.
//

#ifndef VOLKHVY_MESH_H
#define VOLKHVY_MESH_H

#include <vector>
#include <string>
#include <map>

#include <glm/vec3.hpp>
#include <Index.h>
#include <BorrowedPtr.h>

#include "Resource.h"

namespace Core
{
	class MeshVertexData;

	class Submesh
	{
		using api_id = uint32_t;

		// todo: also - indices, material
		// SOA approach
		std::vector<MeshVertexData> Data;
		std::vector<api_id> IDs;
	};

	DECL_RESOURCE(MeshResource)
	{
	public:
		~MeshResource();

		// we do not care about memory over here. yet.
		// todo: this has to be done as handles as well, real submeshes in their container (renderer?)
		std::vector<Submesh> Submeshes;
	};

	class MeshTrait : public ResourceTrait<MeshResource> {};

	// this may work other way later one
	// it will mainly depend on how i will want to use this data
	// i mean, how would i want to upload it to gpu, since any
	// processing on cpu would have to be done on copy anyway
	// and the actual data would be replaced by that copy later on
	class MeshVertexData
	{
		using byte = uint8_t;

		byte* data;
		uint32_t size;

	public:
		auto count() const noexcept -> uint32_t
		{
			return size;
		}

		auto getDataPtr() const noexcept -> byte*
		{
			return data;
		}
	};

	struct MeshVertexComponentInfo
	{
		uint32_t count;
		type_t type;
		// todo: does it need this?
		// todo: may want some sort of ID here
		// to easy distinguish between those components
		const char* name;
	};

	struct MeshScheme
	{
		std::vector<MeshVertexComponentInfo> VertexComponents;
		std::map<std::string, uint32_t > Names;

		auto GetComponent(std::string name) noexcept -> Utils::borrowed_ptr<MeshVertexComponentInfo>
		{
			Utils::borrowed_ptr<MeshVertexComponentInfo> result;

			auto index = Names.find(name);
			if(index != Names.end())
			{
				result.reset(&VertexComponents[index->second]);
			}

			return result;
		}
	};
}

#endif