//
// Created by bentoo on 23.11.16.
//

#include "Core/Logic/World.h"
#include "Core/Logic/EntityManager.h"
#include "Core/Logic/ISystem.h"

namespace Logic
{
    World::World(Memory::IMemoryBlock& memory)
        : _memory(memory), _entityManager(nullptr), _componentContainers{nullptr},
          _registeredSystems(_memory), _dirtyEntities(_memory), _removedEntities(_memory)
    {
        _entityManager = YAGE_CREATE_NEW(_memory, EntityManager)(_memory);
    }

    World::~World()
    {
        for(auto& info : _registeredSystems)
        {
            Memory::Delete(_memory, info.instance);
        }

        for(Utils::IColony* ptr : _componentContainers)
        {
            if(ptr != nullptr)
            {
                Memory::Delete(_memory, ptr);
            }
        }

        Memory::Delete(_memory, _entityManager);
    }

    void World::refresh()
    {
        for(auto handle : _dirtyEntities)
        {
            Entity& entity = getEntity(handle);
            entity._status.dirty = false;

            for (SystemInfo& info : _registeredSystems)
            {
                if(matchComponentSignature(entity.componentBits, info.instance->componentBits))
                {
                    // If component wasn't in this system previously, add it!
                    if(!matchComponentSignature(entity.cachedComponentBits, info.instance->componentBits))
                        info.instance->addEntity(entity.Handle);
                }
                else
                {
                    // If component was in this system previously, remove it!
                    if(matchComponentSignature(entity.cachedComponentBits, info.instance->componentBits))
                        info.instance->removeEntity(entity.Handle);
                }
            }

            entity.cachedComponentBits = entity.componentBits;
        }

        for(auto handle : _removedEntities)
        {
            Entity& entity = getEntity(handle);

            for (SystemInfo& info : _registeredSystems)
            {
                if(matchComponentSignature(entity.componentBits, info.instance->componentBits))
                {
                    info.instance->removeEntity(handle);
                }
            }

            _entityManager->remove(handle);
        }

        _removedEntities.clear();
        _dirtyEntities.clear();
    }

    void World::update(const Core::GameTime& time, bool andRefresh)
    {
        if(andRefresh)
        {
            refresh();
        }

        // iterate over each system
        for(SystemInfo& info : _registeredSystems)
        {
            info.instance->update(time);
        }
    }

    bool World::matchComponentSignature(std::bitset<32> entity, std::bitset<32> system)
    {
        return ((entity & system) == system);
    }

    void World::setAsDirty(Entity& entity)
    {
        if(!entity._status.dirty)
        {
            entity._status.dirty = true;
            _dirtyEntities.add(entity.Handle);
        }
    }

    void World::setAsRemoved(Entity& entity)
    {
        if(!entity._status.removed)
        {
            entity._status.removed = true;
            _removedEntities.add(entity.Handle);
        }
    }

    bool World::isAlive(entity_handle_t handle) const
    {
        return isAliveInCurrentFrame(handle) ? (
            !getEntity(handle)._status.removed
        ) : false;
    }

    bool World::isAliveInCurrentFrame(entity_handle_t handle) const
    {
        return _entityManager->contains(handle);
    }

    auto World::createEntity(Scene* scene) -> World::entity_handle_t
    {
        return _entityManager->createNew(this, scene);
    }

    void World::removeEntity(entity_handle_t handle)
    {
        YAGE_ASSERT(isAliveInCurrentFrame(handle), "Cannot remove Entity with invalid handle : '{}'!", handle.key);

        setAsRemoved(getEntity(handle));
    }

    auto World::getEntity(entity_handle_t handle) const -> Entity&
    {
        YAGE_ASSERT(isAliveInCurrentFrame(handle), "Cannot get Entity with invalid handle : '{}'!", handle.key);

        return _entityManager->getEntity(handle);
    }

    auto World::tryGetEntity(entity_handle_t handle) const -> Entity*
    {
        Entity* result = nullptr;

        if(_entityManager->contains(handle))
            result = &_entityManager->getEntity(handle);

        return result;
    }

    bool World::hasComponent(Entity& entity, comp_id_t bit) const
    {
        return entity.componentBits.test(bit);
    }

    auto World::getComponent(entity_handle_t handle, comp_id_t bit) const -> Utils::RawHandle
    {
        return _entityManager->getComponentHandle(handle, bit);
    }

    auto World::getComponent(Entity& entity, comp_id_t bit) const -> Utils::RawHandle
    {
        return getComponent(entity.Handle, bit);
    }

    void World::addComponent(Entity& entity, comp_id_t bit, Utils::RawHandle handle)
    {
        entity.componentBits.set(bit, true);
        _entityManager->setComponentHandle(entity.Handle, bit, handle);

        setAsDirty(entity);
    }

    void World::removeComponent(Entity& entity, comp_id_t bit)
    {
        entity.componentBits.set(bit, false);
        _entityManager->setComponentHandle(entity.Handle, bit, Utils::RawHandle::invalid());

        setAsDirty(entity);
    }
}
