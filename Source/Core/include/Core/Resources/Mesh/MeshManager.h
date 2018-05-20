//
// Created by bentoo on 10/27/16.
//

#ifndef GAME_MESHMANAGER_H
#define GAME_MESHMANAGER_H

#include "Core/IManager.h"
#include "Mesh.h"
#include "MeshSchemeManager.h"

#include <Utils/MemoryBlock.h>
#include <Utils/Container.h>

namespace Core
{
    class Engine;
}

namespace Gfx
{
    class StaticBuffer;
}

namespace Resources
{
    class SchemeBuffer
    {
        MeshSchemeId _scheme;
        Utils::List<Utils::Handle<Gfx::StaticBuffer>> _buffers;

    public:
        inline explicit SchemeBuffer(Memory::IMemoryBlock& memory, MeshSchemeId scheme)
                : _scheme(scheme), _buffers(memory)
        { }

        inline Utils::List<Utils::Handle<Gfx::StaticBuffer>>& getBuffers()
        { return _buffers; }

        inline MeshSchemeId getSchemeId() const
        { return _scheme; }
    };

    class MeshManager : public Core::IManager
    {
        MANAGER(MeshManager);

        friend class MeshBuilder;

    public:
        using handle_t = Core::Mesh::handle_t;

    protected:
        MeshSchemeManager		_schemeManager;

        Utils::Container<Core::MeshTrait>	_meshContainer;
        Utils::List<SchemeBuffer>			_schemeBuffers;

        Gfx::StaticBuffer* findBestFitBuffer(MeshSchemeId scheme, std::size_t vertexCount, std::size_t indexCount);

        Gfx::StaticBuffer* getOrCreateBuffer(MeshSchemeId scheme, std::size_t vertexCount, std::size_t indexCount);

        void disposeMesh(Core::Mesh& mesh);

        bool uploadToBuffer(handle_t handle);

    public:
        explicit MeshManager(Core::Engine& engine, Memory::IMemoryBlock& memory);
        virtual ~MeshManager() noexcept;

        MeshManager(MeshManager&&) = delete;
        MeshManager(const MeshManager&) = delete;
        MeshManager& operator=(MeshManager&&) = delete;
        MeshManager& operator=(const MeshManager&) = delete;

        void disposeAll();
        void reloadAll();

        void disposeMesh(handle_t mesh);

        Core::Mesh* tryGetMesh(handle_t);

        inline bool hasMesh(handle_t handle)
        { return _meshContainer.contains(handle); }

        inline handle_t createMesh()
        { return _meshContainer.emplace(_memory); }

        inline Core::Mesh& getMesh(handle_t handle)
        { return _meshContainer.get(handle); }

        inline Resources::MeshSchemeId getSchemeId(const Core::MeshScheme& scheme)
        { return _schemeManager.getSchemeId(scheme); }

        inline Core::MeshScheme* tryGetMeshScheme(Resources::MeshSchemeId id)
        { return _schemeManager.tryGetMeshScheme(id); }

        inline Core::MeshScheme& getMeshScheme(Resources::MeshSchemeId id)
        { return *_schemeManager.tryGetMeshScheme(id); }

        inline bool hasScheme(const Core::MeshScheme& scheme) const
        { return _schemeManager.contains(scheme); }
    };
}

#endif //GAME_MESHMANAGER_H
