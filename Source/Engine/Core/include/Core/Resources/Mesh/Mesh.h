//
// Created by MrJaqbq on 2016-02-10.
//

#ifndef YAGE_MESH_H
#define YAGE_MESH_H

#include <Gfx/Graphics/OpenGl/OpenGLBase.h>

#include <Utils/List.h>
#include <Utils/String.h>
#include <Utils/SafeDelete.h>
#include <Utils/MemoryBlock.h>

#include "Core/Resources/Resource.h"
#include "MeshScheme.h"

// temporary
#include "Core/Gfx/StaticBuffer.h"

namespace Gfx
{
    class StaticBuffer;
}

namespace Resources
{
    class MeshBuilder;
    class MeshManager;
}

namespace Meta
{
	class RegisterClass;
}

namespace Core
{
    class MeshScheme
    {
    public:
        struct PropertyInfo
        {
            explicit PropertyInfo(const Utils::String& Name, type_t Type, std::size_t propSize, uint32_t PropertyCount, bool Normalize)
                    : Name(Name), Type(Type), PropertyCount(PropertyCount), PropertySize(propSize), Normalize(Normalize)
            { }

			PropertyInfo(const PropertyInfo& other) = default;
			PropertyInfo(PropertyInfo&& other) = default;
			
			PropertyInfo& operator=(const PropertyInfo&) = default;
			PropertyInfo& operator=(PropertyInfo&&) = default;

            Utils::String Name;
            type_t Type;
            uint32_t PropertyCount;
            std::size_t PropertySize;
            bool Normalize;

            inline bool operator==(const PropertyInfo& other) const
            {
                if(this == &other)
                    return true;

                if(Type != other.Type || PropertyCount != other.PropertyCount
                    || PropertySize != other.PropertySize || Normalize != other.Normalize)
                {
                    return false;
                }

                int result = Name.compare(other.Name) == 0;
                return result == 0;
            }

            inline bool operator!=(const PropertyInfo& other) const
            {
                return !(*this == other);
            }
        };

    protected:
        Utils::List<PropertyInfo>	PropertiesInfo;
        std::size_t					IndexSize;
        GLenum						IndexType;

    public:
        explicit MeshScheme(Memory::IMemoryBlock& memory)
            : PropertiesInfo(), IndexSize(0), IndexType(0)// #NewAlloc
        {

        }

        MeshScheme(const MeshScheme& other)
            : PropertiesInfo(other.PropertiesInfo),
              IndexSize(other.IndexSize), IndexType(other.IndexType)
        {

        }

        MeshScheme(MeshScheme&& other)
            : PropertiesInfo(other.PropertiesInfo),
              IndexSize(other.IndexSize), IndexType(other.IndexType)
        {
            other.PropertiesInfo.clear();
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
            IndexType = OpenGL::toOpenGlType(indexType);
        }

        inline void addPropertyInfo(const char* name, type_t type, std::size_t propSize, uint32_t propCount, bool normalize)
        {
            PropertiesInfo.emplace_back(name, type, propSize, propCount, normalize);
        }

        inline const Utils::List<PropertyInfo>& getPropertiesInfo() const
        { return PropertiesInfo; }

        inline std::size_t getIndexSize() const
        { return IndexSize; }

        inline GLenum getIndexType() const
        { return IndexType; }

        inline std::size_t vertexSize() const
        {
            std::size_t size = 0;
            for (const PropertyInfo& data : PropertiesInfo)
            {
                size += data.PropertyCount * data.PropertySize;
            }
            return size;
        }

        inline bool operator==(const MeshScheme& other) const
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

        inline bool operator!=(const MeshScheme& other) const
        {
            return !(*this == other);
        }
    };

    class MeshData
    {
    public:
        class PropertyData
        {
            const void* _dataPtr;
            std::size_t _vertexSize;
            uint32_t _count;
            std::size_t _rawSize;

        public:
            explicit PropertyData(const void* data, std::size_t vertexSize, std::uint32_t count)
                    : _dataPtr(data), _vertexSize(vertexSize), _count(count), _rawSize(_vertexSize * _count)
            {

            }

            inline const void* getDataPtr() const
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
            : PropertiesData()// #NewAlloc
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

        inline void addPropertyData(const void* ptr, std::size_t vertexSize, std::uint32_t count)
        {
            PropertiesData.emplace_back(ptr, vertexSize, count);
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
            for (const PropertyData& data : PropertiesData)
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

        const void*	_indicesPtr;
        std::size_t	_indiceCount;

        std::size_t _baseVertex;

    public:
        inline explicit Submesh(const void* ptr, std::size_t count)
            : _indicesPtr(ptr), _indiceCount(count), _baseVertex(0)
        { }

		Submesh(const Submesh&) = default;
		Submesh(Submesh&&) = default;

		Submesh& operator=(const Submesh&) = default;
		Submesh& operator=(Submesh&&) = default;

        inline std::size_t getIndiceCount() const noexcept
        { return _indiceCount; }

        inline const void* getIndicePtr() const noexcept
        { return _indicesPtr; }

        inline std::size_t getBaseVertex() const noexcept
        { return _baseVertex; }
    };

    enum MeshStorageType
    {
        STATIC,
        DYNAMIC
    };

	YClass(Serialize())
    class Mesh : public IResource
    {
		friend class Meta::RegisterClass;
        friend class Resources::MeshBuilder;
        friend class Resources::MeshManager;

        Memory::IMemoryBlock*	_memory;
        Utils::List<Submesh>	_submeshes;

        MeshData*				_data;
        MeshStorageType			_storageType;

        Resources::MeshSchemeId _schemeId;
        Utils::Handle<Gfx::StaticBuffer> _buffer;

    public:
        Mesh();
        
        explicit Mesh(Memory::IMemoryBlock& memory);

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        Mesh& operator=(Mesh&&) = delete;

        Mesh(Mesh&& other)
            : IResource(std::move(other)),
              _memory(other._memory),
              _submeshes(std::move(other._submeshes)),
              _data(other._data),
              _storageType(other._storageType),
              _schemeId(other._schemeId),
              _buffer(std::move(other._buffer))
        {
            other._memory = nullptr;
            other._data = nullptr;
            other._buffer = Utils::Handle<Gfx::StaticBuffer>::invalid();
        }

        inline virtual ~Mesh() noexcept
        {
            _submeshes.clear();
            Memory::Delete(*_memory, _data);
        }

		inline auto getSubmeshes() noexcept -> Utils::List<Submesh> &
		{ return _submeshes; }

        inline auto getSubmeshes() const noexcept -> const Utils::List<Submesh>&
        { return _submeshes; }

        inline auto getMeshData() const noexcept -> MeshData&
        { return *_data; }

        inline auto getMeshSchemeId() const noexcept -> Resources::MeshSchemeId
        { return _schemeId; }

        inline auto getStorageType() const noexcept -> MeshStorageType
        { return _storageType; }

        inline auto getBuffer() const noexcept -> Utils::Handle<Gfx::StaticBuffer>
        { return _buffer; }

        std::size_t getIndiceCount() const;

        std::size_t getVertexCount() const;
    };
}

#include "CTTI/Core/Mesh.gen.h"

#endif