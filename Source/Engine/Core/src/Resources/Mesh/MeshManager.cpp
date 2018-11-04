//
// Created by bentoo on 10/27/16.
//

#include <Utils/MemorySizes.h>

#include "Core/Resources/Mesh/MeshManager.h"
#include "Core/Gfx/BufferManager.h"
#include "Core/Engine.h"
#include "Core/Logger.h"

namespace Resources
{
    MeshManager::MeshManager(Core::Engine &engine, Memory::IMemoryBlock &memory)
        : IManager(engine, memory), _schemeManager(_memory), _meshContainer(_memory), _schemeBuffers(_memory)
    {
        Core::Logger::info("Created mesh manager with capacity {}", _meshContainer.capacity());
    }

    MeshManager::~MeshManager() noexcept
    {
        disposeAll();
    }

    void MeshManager::disposeAll()
    {
        Core::Logger::info("Releasing all meshes...");

        _meshContainer.clear();

        Core::Logger::info("Released all meshes");
    }

    Core::Mesh* MeshManager::tryGetMesh(handle_t handle)
    {
        Core::Mesh* ptr = nullptr;
        if (_meshContainer.contains(handle))
        {
            ptr = &getMesh(handle);
        }
        return ptr;
    }

    void MeshManager::disposeMesh(Core::Mesh &mesh)
    {
        disposeMesh(mesh.Handle);
    }

    void MeshManager::disposeMesh(handle_t handle)
    {
        // TODO: ADDITIONAL THINGS, LIKE STATIC BUFFER INVALIDATION

        _meshContainer.erase(handle);
    }

    Gfx::StaticBuffer* MeshManager::findBestFitBuffer(MeshSchemeId scheme, std::size_t vertexCount, std::size_t indexCount)
    {
        Gfx::StaticBuffer* bestFitBuffer = nullptr;
        std::size_t smallestDiff = vertexCount;

        for (SchemeBuffer& schemeBuffer : _schemeBuffers)
        {
            if (schemeBuffer.getSchemeId() == scheme)
            {
                for (auto handle : schemeBuffer.getBuffers())
                {
                    Gfx::StaticBuffer& buffer = _engine.BufferManager->getBuffer(handle);
                    if (buffer.hasSize(vertexCount, indexCount))
                    {
                        const std::size_t sizeDiff = buffer.getVertexCount() - buffer.getUsedVertexCount();
                        if (sizeDiff < smallestDiff)
                        {
                            smallestDiff	= sizeDiff;
                            bestFitBuffer	= &buffer;
                        }
                    }
                }
            }
        }

        return bestFitBuffer;
    }

    Gfx::StaticBuffer* MeshManager::getOrCreateBuffer(MeshSchemeId schemeID, std::size_t vertexCount, std::size_t indexCount)
    {
        Gfx::StaticBuffer* result = findBestFitBuffer(schemeID, vertexCount, indexCount);

        if(result == nullptr)
        {
            // Dereference scheme
            // Calc vertex size
            // Calc appropriate buffer size (1-2MB)

            Core::MeshScheme* schemePtr = tryGetMeshScheme(schemeID);

            // TODO: log errors
            if(schemePtr == nullptr)
            {
                return nullptr;
            }

            const Core::MeshScheme& scheme = (*schemePtr);
            std::size_t vertexSize = 0;

            for(uint32_t i = 0; i < scheme.getPropertiesInfo().size(); i++)
            {
                const Core::MeshScheme::PropertyInfo& propInfo = scheme.getPropertiesInfo()[i];
                vertexSize += propInfo.PropertySize * propInfo.PropertyCount;
            }

            std::size_t maxVertexCount = Memory::MB(1) / vertexSize;
            std::size_t maxIndexCount = Memory::MB(1) / scheme.getIndexSize();

            if(maxVertexCount < vertexCount)
            {
                maxVertexCount = vertexCount * vertexSize;
            }

            if(maxIndexCount < vertexCount)
            {
                maxIndexCount = vertexCount * scheme.getIndexSize();
            }

            Gfx::StaticBuffer::handle_t	handle = _engine.BufferManager->createNew(vertexSize, scheme.getIndexSize(), maxVertexCount, maxIndexCount);
            Gfx::StaticBuffer&			buffer = _engine.BufferManager->getBuffer(handle);

            result = &buffer;
        }

        return result;
    }

    // TODO: Only prepare data to some pointer, then create separate CMD queue and do unsynchronized bit and fence sync!
    bool MeshManager::uploadToBuffer(handle_t handle)
    {
        // TODO: log errors
        Core::Mesh* meshPtr = tryGetMesh(handle);

        if(meshPtr == nullptr)
            return false;

        if(meshPtr->_buffer)
        {
            disposeMesh(meshPtr->Handle);
        }

        Core::MeshScheme* schemePtr = tryGetMeshScheme(meshPtr->getMeshSchemeId());

        if(schemePtr == nullptr)
            return false;

        Core::Mesh&			mesh	= (*meshPtr);
        Core::MeshScheme&	scheme	= (*schemePtr);
        Core::MeshData&		data	= mesh.getMeshData();

        const std::size_t allVertexCount = mesh.getVertexCount();
        const std::size_t allIndiceCount = mesh.getIndiceCount();

        if(allVertexCount == 0 || allIndiceCount == 0)
            return false;

        // Get buffer or create one
        // Aquire pointer through mapping

        // GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_UNSYNCHRONIZED_BIT - requires fence sync!
        Gfx::StaticBuffer* buffer = getOrCreateBuffer(mesh.getMeshSchemeId(), allVertexCount, allIndiceCount);

        if(buffer == nullptr)
        {
            return false;
        }

        void* vertexMapPtr	= buffer->mapVertexMemory(allVertexCount, gl::MAP_WRITE_BIT);
        char* vertexPtr		= static_cast<char*>(vertexMapPtr);

        OpenGL::checkError();

        for(uint32_t vertex = 0; vertex < allVertexCount; vertex++)
        {
            // for every vertex
            // interleaved data
            for(uint32_t i = 0; i < scheme.getPropertiesInfo().size(); i++)
            {
                // go through each property
                const Core::MeshData::PropertyData&		propData	=   data.getPropertiesData()[i];
                const Core::MeshScheme::PropertyInfo&	propInfo	= scheme.getPropertiesInfo()[i];
                const std::size_t 						vertexSize	= propData.vertexSize();
                const char*								dataPtr		= static_cast<const char*>(propData.getDataPtr());

                dataPtr += vertex * vertexSize;//propInfo.PropertySize * propInfo.PropertyCount;

                // TODO: we may have some sort of alignment/offset here, get this from propInfo!
                // push its data to pointer
                memcpy(vertexPtr, dataPtr, vertexSize);

                // advance pointer by bytes
                vertexPtr += vertexSize;
            }
        }

        buffer->unmapVertexMemory();

        void* indexMapPtr	= buffer->mapIndexMemory(allIndiceCount, gl::MAP_WRITE_BIT);
        char* indexPtr		= static_cast<char*>(indexMapPtr);

        std::size_t startingOffset	= buffer->getUsedIndexCount();
        std::size_t indexSize		= scheme.getIndexSize();

        for(Core::Submesh& submesh : mesh.getSubmeshes())
        {
            // push to IBO
            // save baseVertex

            const std::size_t submeshIndexSize = indexSize * submesh.getIndiceCount();

            memcpy(indexPtr, submesh.getIndicePtr(), submeshIndexSize);

            indexPtr 			+= submeshIndexSize;
            submesh._baseVertex  = startingOffset;
            startingOffset		+= submesh.getIndiceCount();
        }

        buffer->unmapIndexMemory();

        // Set handle to buffer
        mesh._buffer = buffer->Handle;

        // Get scheme
        // Walk through scheme
        // Calc Needed size
        // Map Ptr of size from GPU
        // Create particular vertices
        // Pack them into pointer
        // Upload pointer to GPU
        // Unmap

        return true;
    }
}
