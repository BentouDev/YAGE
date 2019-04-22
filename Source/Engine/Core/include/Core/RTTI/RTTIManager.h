#ifndef YAGE_RTTI_MANAGER_H
#define YAGE_RTTI_MANAGER_H

#include <RTTI/RTTIRegister.h>
#include "Core/RTTI/RTTIEngineLoader.h"
#include "Core/IManager.h"

namespace RTTI
{
    class Manager : public Core::IManager
    {
        MANAGER(Manager);

    public:
        explicit Manager(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~Manager() = default;

        template <typename T>
        void PushLayer()
        {
            static_assert(std::is_base_of<ILayer, T>::value,
                "RTTI: Can load only ILayer derivatives!");

			T* instance = LayerHelper<T>::getInstance();
			if (!instance)
			{
				instance = YAGE_CREATE_NEW(Memory::GetDefaultBlock<Manager>(), T)();
			}

            RTTI::GetRegister()->LoadLayer(instance);
        }

		template <typename T>
		static void DeleteLayer(void* ptr)
		{
			static_assert(std::is_base_of<ILayer, T>::value,
				"RTTI: Can delete only ILayer derivatives!");

			T* ptr_t = reinterpret_cast<T*>(ptr);

			RTTI::GetRegister()->UnloadLayer(ptr_t);

			if (LayerHelper<T>::getInstance() != ptr_t)
			{
				Memory::Delete(Memory::GetDefaultBlock<Manager>(), ptr_t);
			}
		}
    };
}

#endif // YAGE_RTTI_MANAGER_H