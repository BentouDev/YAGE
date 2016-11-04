//
// Created by bentoo on 10/27/16.
//

#ifndef GAME_MESHBUILDER_H
#define GAME_MESHBUILDER_H

#include <Utils/MemoryBlock.h>
#include "Mesh.h"

namespace Resources
{
	class MeshManager;

	class MeshBuilder
	{
		Memory::IMemoryBlock&	_memory;
		Resources::MeshManager&	_manager;

		Core::MeshData*			_data;
		Core::MeshScheme*		_scheme;
		Core::MeshStorageType	_storageType;

		Utils::List<Core::Submesh> _submeshes;

	public:
		explicit MeshBuilder(Memory::IMemoryBlock& memory, MeshManager& manager);

		template<typename T>
		MeshBuilder& withProperty(const char* name, T *ptr, uint32_t propCount, uint32_t arrayLength)
		{
			_scheme->addPropertyInfo(name, TypeInfo<T>::id(), sizeof(T), propCount);
			_data->addPropertyData(ptr, propCount * sizeof(T), arrayLength);
			return *this;
		}

		template<typename T>
		MeshBuilder& withIndexType()
		{
			_scheme->setIndexType(sizeof(T), TypeInfo<T>::id());
			return *this;
		}

		template<typename T>
		MeshBuilder& withSubmeshIndices(T *ptr, uint32_t indiceCount)
		{
			assert(_scheme->getIndexSize() == sizeof(T) && _scheme->getIndexType() == TypeInfo<T>::id()
				   && "All submeshes must have same index type!");

			_submeshes.emplace(ptr, indiceCount);
			return *this;
		}

		MeshBuilder& withStorageType(Core::MeshStorageType type);

		Core::Mesh::handle_t build(const char* meshName);
	};
}

#endif //GAME_MESHBUILDER_H
