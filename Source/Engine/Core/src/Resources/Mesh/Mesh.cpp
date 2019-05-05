//
// Created by MrJaqbq on 2016-02-10.
//

#include "Core/Resources/Mesh/Mesh.h"

namespace Core
{
    Mesh::Mesh()
        : _memory{ nullptr }, _data{ nullptr }
    { }

    Mesh::Mesh(Memory::IMemoryBlock& memory)
        : _memory(&memory), _submeshes(), _data(nullptr), _storageType(STATIC), _schemeId(-1), _buffer() // #NewAlloc
    {
        _data = YAGE_CREATE_NEW((*_memory), MeshData)(*_memory);
    }

	std::size_t Mesh::getVertexCount() const
	{
		std::size_t result = 0;

		if(_data != nullptr && _data->getPropertiesData().size() > 0)
		{
			result = _data->getPropertiesData()[0].count();
		}

		return result;
	}

	std::size_t Mesh::getIndiceCount() const
	{
		std::size_t result = 0;
		for(auto& submesh : _submeshes)
		{
			result += submesh.getIndiceCount();
		}

		return result;
	}
}
