//
// Created by MrJaqbq on 2016-02-10.
//

#ifndef YAGE_MESH_H
#define YAGE_MESH_H

#include "../Resource.h"

#include <Utils/List.h>
#include <Utils/String.h>
#include <Utils/DefaultTrait.h>
#include <Utils/SafeDelete.h>
#include <Utils/MemoryBlock.h>

namespace Gfx
{
	class StaticBuffer;
}

namespace Resources
{
	class MeshBuilder;
	class MeshManager;
	typedef int32_t MeshSchemeId;
}

namespace Core
{
	class MeshScheme
	{
	public:
		struct PropertyInfo
		{
			explicit PropertyInfo(const char* Name, type_t Type, std::size_t propSize, uint32_t PropertyCount)
					: Name(Name), Type(Type), PropertySize(propSize), PropertyCount(PropertyCount)
			{ }

			const char* Name;
			const type_t Type;
			const uint32_t PropertyCount;
			const std::size_t PropertySize;

			inline bool operator==(const PropertyInfo& other)
			{
				if(this == &other)
					return true;

				if(Type != other.Type || PropertyCount != other.PropertyCount)
				{
					return false;
				}

				int result = std::strcmp(Name, other.Name);
				return result == 0;
			}

			inline bool operator!=(const PropertyInfo& other)
			{
				return !(*this == other);
			}
		};

	protected:
		Utils::List<PropertyInfo>	PropertiesInfo;
		std::size_t					IndexSize;
		type_t						IndexType;

	public:
		explicit MeshScheme(Memory::IMemoryBlock &memory)
			: PropertiesInfo(memory), IndexSize(0), IndexType(0)
		{

		}

		MeshScheme(const MeshScheme &other)
			: PropertiesInfo(other.PropertiesInfo),
			  IndexSize(other.IndexSize), IndexType(other.IndexType)
		{

		}

		MeshScheme(MeshScheme &&other)
			: PropertiesInfo(other.PropertiesInfo),
			  IndexSize(other.IndexSize), IndexType(other.IndexType)
		{
			other.PropertiesInfo.clear();
		}

		inline virtual ~MeshScheme() noexcept
		{
			PropertiesInfo.clear();
		}

		MeshScheme& operator=(const MeshScheme& other)
		{
			if(this != &other)
			{
				IndexSize = other.IndexSize;
				IndexType = other.IndexType;
				PropertiesInfo.clear();
				PropertiesInfo = other.PropertiesInfo;
			}
			return *this;
		}

		MeshScheme& operator=(MeshScheme&& other)
		{
			if(this != &other)
			{
				IndexSize = other.IndexSize;
				IndexType = other.IndexType;
				PropertiesInfo = std::move(other.PropertiesInfo);
			}
			return *this;
		}

		inline void setIndexType(std::size_t indexSize, type_t indexType)
		{
			IndexSize = indexSize;
			IndexType = indexType;
		}

		inline void addPropertyInfo(const char* name, type_t type, std::size_t propSize, uint32_t propCount)
		{
			PropertiesInfo.emplace(name, type, propSize, propCount);
		}

		inline const Utils::List<PropertyInfo>& getPropertiesInfo() const
		{ return PropertiesInfo; }

		inline std::size_t getIndexSize() const
		{ return IndexSize; }

		inline type_t getIndexType() const
		{ return IndexType; }

		inline bool operator==(const MeshScheme& other)
		{
			if(this == &other)
				return true;

			if(PropertiesInfo.size() != other.PropertiesInfo.size())
				return false;

			if(IndexSize != other.IndexSize || IndexType != other.IndexType)
				return false;

			for(std::size_t i = 0; i < PropertiesInfo.size(); i++)
			{
				if(PropertiesInfo[i] != other.PropertiesInfo[i])
					return false;
			}

			return true;
		}

		inline bool operator!=(const MeshScheme& other)
		{
			return !(*this == other);
		}
	};

	class MeshData
	{
	public:
		class PropertyData
		{
			void* _dataPtr;
			std::size_t _rawSize;
			std::size_t _vertexSize;
			uint32_t _count;

		public:
			explicit PropertyData(void* data, std::size_t vertexSize, std::uint32_t count)
					: _dataPtr(data), _count(count), _vertexSize(vertexSize), _rawSize(_vertexSize * _count)
			{

			}

			inline void* getDataPtr() const
			{ return _dataPtr; }

			inline size_t size() const
			{ return _rawSize; }

			inline std::size_t vertexSize() const
			{ return _vertexSize; }

			inline std::uint32_t count() const
			{ return _count; }
		};

	protected:
		Utils::List<PropertyData> PropertiesData;

	public:
		explicit MeshData(Memory::IMemoryBlock &memory)
			: PropertiesData(memory)
		{

		}

		MeshData(const MeshData &other)
			: PropertiesData(other.PropertiesData)
		{

		}

		MeshData(MeshData &&other)
			: PropertiesData(other.PropertiesData)
		{
			other.PropertiesData.clear();
		}

		inline virtual ~MeshData()
		{
			PropertiesData.clear();
		}

		MeshData& operator=(const MeshData& other)
		{
			if(this != &other)
			{
				PropertiesData.clear();
				PropertiesData = other.PropertiesData;
			}
			return *this;
		}

		MeshData& operator=(MeshData&& other)
		{
			if(this != &other)
			{
				PropertiesData = std::move(other.PropertiesData);
			}
			return *this;
		}

		inline void addPropertyData(void* ptr, std::size_t vertexSize, std::uint32_t count)
		{
			PropertiesData.emplace(ptr, vertexSize, count);
		}

		inline const Utils::List<PropertyData>& getPropertiesData() const
		{ return PropertiesData; }

		inline uint32_t vertexCount() const
		{
			if(PropertiesData.size() > 0)
				return PropertiesData[0].count();
			else
				return 0;
		}

		inline std::size_t vertexSize() const
		{
			std::size_t size = 0;
			for(PropertyData& data : PropertiesData)
			{
				size += data.vertexSize();
			}
			return size;
		}
	};

	class Submesh
	{
		friend class Resources::MeshBuilder;
		friend class Resources::MeshManager;

		const void*			_indicesPtr;
		const std::size_t	_indiceCount;
		std::size_t 		_baseVertex;

	public:
		inline explicit Submesh(void* ptr, std::size_t count)
			: _indicesPtr(ptr), _indiceCount(count), _baseVertex(0)
		{ }

		std::size_t getIndiceCount() const
		{ return _indiceCount; }

		const void* getIndicePtr() const
		{ return _indicesPtr; }
	};

	enum MeshStorageType
	{
		STATIC,
		DYNAMIC
	};

	DECL_RESOURCE(Mesh)
	{
		friend class Resources::MeshBuilder;
		friend class Resources::MeshManager;

		Memory::IMemoryBlock&	_memory;
		Utils::List<Submesh>	_submeshes;

		Resources::MeshSchemeId _schemeId;

		MeshData*				_data;
		MeshStorageType			_storageType;

		Utils::Handle<Gfx::StaticBuffer> _buffer;

	public:
		inline explicit Mesh(Memory::IMemoryBlock &memory)
			: _memory(memory), _submeshes(_memory), _data(nullptr), _storageType(STATIC), _schemeId(-1), _buffer()
		{
			_data = YAGE_CREATE_NEW(_memory, MeshData)(_memory);
		}

		inline virtual ~Mesh() noexcept
		{
			cleanUp();
		}

		inline const Utils::List<Submesh> &getSubmeshes() const noexcept
		{ return _submeshes; }

		inline MeshData& getMeshData() const noexcept
		{ return *_data; }

		inline Resources::MeshSchemeId getMeshSchemeId() const noexcept
		{ return _schemeId; }

		inline MeshStorageType getStorageType()
		{ return _storageType; }

		std::size_t getIndiceCount() const;

		std::size_t getVertexCount() const;

		void swap(Mesh &other) noexcept override
		{
			assert(false && "SWAP IN MESH NOT IMPLEMENTED");
		}

		void cleanUp() noexcept override
		{
			_submeshes.clear();
			Memory::Delete(_memory, _data);
		}
	};

	class MeshTrait : public Utils::DefaultTrait<Mesh> { };
}

#endif