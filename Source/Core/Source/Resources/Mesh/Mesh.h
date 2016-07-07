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
#include <DefaultTrait.h>
#include <SafeDelete.h>

#include "../../Gfx/Buffer.h"
#include "../../Gfx/Api/BaseApi.h"

#include "../Resource.h"

namespace Core
{
	/*class MeshVertexData;

	struct MeshVertexComponentInfo
	{
		MeshVertexComponentInfo(uint32_t new_count, type_t new_type)
				: count(new_count), type(new_type) { }

		uint32_t size;
		type_t type;
		// todo: may want some sort of ID here
		// to easy distinguish between those components
	};*/

	/*struct MeshScheme
	{
		std::vector<MeshVertexComponentInfo> VertexComponents;
		std::map<std::string, uint32_t > Names;

		template <typename T>
		auto createComponent(uint32_t count, std::string name) -> void
		{
			VertexComponents.emplace_back(TypeInfo<T>::id(), size);
			Names[name] = VertexComponents.size() - 1;
		}

		auto getComponent(std::string name) noexcept -> Utils::borrowed_ptr<MeshVertexComponentInfo>
		{
			Utils::borrowed_ptr<MeshVertexComponentInfo> result;

			auto index = Names.find(name);
			if(index != Names.end())
			{
				result.reset(&VertexComponents[index->second]);
			}

			return result;
		}
	};*/

	// this may work other way later one
	// it will mainly depend on how i will want to use this data
	// i mean, how would i want to upload it to gpu, since any
	// processing on cpu would have to be done on copy anyway
	// and the actual data would be replaced by that copy later on
	/*class MeshVertexData
	{
		using byte = uint8_t;

		byte* data;
		uint32_t size;

	public:
		MeshVertexData(byte* new_data, uint32_t new_size)
				: data(new_data), size(new_size)
		{ }

		auto size() const noexcept -> uint32_t
		{
			return size;
		}

		auto getDataPtr() const noexcept -> byte*
		{
			return data;
		}
	};*/

	class Submesh
	{
		using api_id = uint32_t;

		// todo: also - indices, material
		// SOA approach
		std::vector<Gfx::BufferData> Data;
		std::vector<api_id> IDs;

		Gfx::BufferData* index;
		Gfx::BufferScheme scheme;

	public:
		~Submesh()
		{
			Data.clear();
			IDs.clear();
			Memory::SafeDelete(index);
		}

		template <typename T>
		auto createNewProperty(std::string name, T* data, uint32_t size, uint32_t count) noexcept -> void
		{
			Data.emplace_back(reinterpret_cast<uint8_t*>(data), size * count);
			scheme.createComponent<T>(size, name);
		}

		template <typename T>
		auto setIndices(T* indices, uint8_t size) noexcept -> void
		{
			if(index == nullptr)
			{
				index = new Gfx::BufferData(reinterpret_cast<uint8_t*>(indices), size);
			}
			else
			{
				index->~BufferData();
				new (index) Gfx::BufferData(reinterpret_cast<uint8_t*>(indices), size);
			}
		}
	};

	DECL_RESOURCE(MeshResource)
	{
	public:
		 MeshResource() : Resource() { }
		 //MeshResource(const MeshResource& other);
		~MeshResource() { }

		//auto operator=(const MeshResource& other) -> MeshResource&;

		// we do not care about memory over here. yet.
		// todo: this has to be done as handles as well, real submeshes in their container (renderer?)
		std::vector<Submesh> Submeshes;

		void swap(MeshResource& other) noexcept override { }
		void cleanUp() noexcept override { }
	};

	class MeshTrait : public Utils::DefaultTrait<MeshResource> {};
}

#endif