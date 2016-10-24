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

namespace Core
{
	class MeshScheme
	{
		struct PropertyInfo
		{
			explicit PropertyInfo(const Utils::String &Name, type_t Type, uint32_t PropertyCount, uint32_t ArrayLength)
					: Name(Name), Type(Type), PropertyCount(PropertyCount), ArrayLength(ArrayLength)
			{}

			Utils::String Name;
			type_t Type;
			uint32_t PropertyCount;
			uint32_t ArrayLength;
		};

		Utils::List<PropertyInfo> PropertiesInfo;

	public:
		explicit MeshScheme(Memory::IMemoryBlock &memory)
			: PropertiesInfo(memory)
		{

		}

		MeshScheme(const MeshScheme &other)
			: PropertiesInfo(other.PropertiesInfo)
		{

		}

		MeshScheme(MeshScheme &&other)
			: PropertiesInfo(other.PropertiesInfo)
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
				PropertiesInfo.clear();
				PropertiesInfo = other.PropertiesInfo;
			}
			return *this;
		}

		MeshScheme& operator=(MeshScheme&& other)
		{
			if(this != &other)
			{
				PropertiesInfo = std::move(other.PropertiesInfo);
			}
			return *this;
		}

		inline void addPropertyInfo(Utils::String name, type_t type, uint32_t propCount, uint32_t arrayLength)
		{
			PropertiesInfo.emplace(name, type, propCount, arrayLength);
		}

		inline const Utils::List<PropertyInfo>& getPropertiesInfo() const
		{ return PropertiesInfo; }
	};

	class MeshData
	{
		class PropertyData
		{
			void *_dataPtr;
			size_t _rawSize;

		public:
			explicit PropertyData(void *data, size_t size)
					: _dataPtr(data), _rawSize(size)
			{

			}

			inline void *getDataPtr() const
			{ return _dataPtr; }

			inline size_t size() const
			{ return _rawSize; }
		};

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

		inline void addPropertyData(void* ptr, std::size_t size)
		{
			PropertiesData.emplace(ptr, size);
		}

		inline const Utils::List<PropertyData>& getPropertiesData() const
		{ return PropertiesData; }
	};

	class Submesh
	{
		const void*			_indicesPtr;
		const std::size_t	_indiceTypeSize;
		const uint32_t		_indiceCount;
		const type_t		_indiceType;

	public:
		inline explicit Submesh(void* ptr, std::size_t size, uint32_t count, type_t type)
			: _indicesPtr(ptr), _indiceTypeSize(size), _indiceCount(count), _indiceType(type)
		{

		}
	};

	DECL_RESOURCE(Mesh)
	{
		Memory::IMemoryBlock&	_memory;
		Utils::List<Submesh>	_submeshes;
		MeshData*				_data;
		MeshScheme*				_scheme;

	public:
		inline explicit Mesh(Memory::IMemoryBlock &memory)
			: _memory(memory), _submeshes(_memory), _data(nullptr), _scheme(nullptr)
		{
			_data	= YAGE_CREATE_NEW(_memory, MeshData)(_memory);
			_scheme	= YAGE_CREATE_NEW(_memory, MeshScheme)(_memory);
		}

		inline virtual ~Mesh() noexcept
		{
			cleanUp();
		}

		inline const Utils::List<Submesh> &getSubmeshes() const noexcept
		{ return _submeshes; }

		inline MeshData& getMeshData() const noexcept
		{ return *_data; }

		inline MeshScheme& getMeshScheme() const noexcept
		{ return *_scheme; }

		template<typename T>
		inline Core::Mesh& addProperty(Utils::String name, T *ptr, uint32_t propCount, uint32_t arrayLength)
		{
			_scheme->addPropertyInfo(name, TypeInfo<T>::id(), propCount, arrayLength);
			_data->addPropertyData(ptr, sizeof(T) * propCount * arrayLength);
			return *this;
		}

		template<typename T>
		inline Core::Mesh& addSubmeshIndices(T *ptr, uint32_t indiceCount)
		{
			_submeshes.emplace(Submesh(ptr, sizeof(T), indiceCount, TypeInfo<T>::id()));
			return *this;
		}

		template<typename T>
		inline Core::Mesh& setSubmeshIndices(T *ptr, uint32_t indiceCount, uint32_t subMesh)
		{
			if (_submeshes.size() <= subMesh) {
				_submeshes.resize(subMesh);
			}

			_submeshes[subMesh] = std::move(Submesh(ptr, sizeof(T), indiceCount, TypeInfo<T>::id()));
			return *this;
		}

		void swap(Mesh &other) noexcept override
		{
			assert(false && "SWAP IN MESH NOT IMPLEMENTED");
		}

		void cleanUp() noexcept override
		{
			_submeshes.clear();
			Memory::Delete(_memory, _data);
			Memory::Delete(_memory, _scheme);
		}
	};

	class MeshTrait : public Utils::DefaultTrait<Mesh> { };
}

#endif