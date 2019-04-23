//
// Created by bentoo on 10/27/16.
//

#include "Core/Resources/Mesh/MeshBuilder.h"
#include "Core/Resources/Mesh/MeshManager.h"

namespace Resources
{
	MeshBuilder::MeshBuilder(Memory::IMemoryBlock& memory, MeshManager& manager)
		: _memory(memory), _manager(manager), _data(nullptr), _scheme(nullptr), _storageType(Core::STATIC), _submeshes()// #NewAlloc
	{
		_data = YAGE_CREATE_NEW(_memory, Core::MeshData)(_memory);
		_scheme = YAGE_CREATE_NEW(_memory, Core::MeshScheme)(_memory);
	}

	MeshBuilder::~MeshBuilder()
	{
		Memory::Delete(_memory, _data);
		Memory::Delete(_memory, _scheme);
	}

	Core::Mesh::handle_t MeshBuilder::build(const char *meshName)
	{
		Utils::Handle<Core::Mesh> mesh = _manager.createMesh();

		Core::Mesh& instance = _manager.getMesh(mesh);

		instance.Name 			= meshName;
		instance._schemeId		= _manager.getSchemeId(*_scheme);
		instance._submeshes 	= _submeshes;
		instance._storageType	= _storageType;
		*instance._data 		= *_data;

		_manager.uploadToBuffer(mesh);

		return mesh;
	}

	MeshBuilder& MeshBuilder::withStorageType(Core::MeshStorageType type)
	{
		_storageType = type;
		return *this;
	}
}
