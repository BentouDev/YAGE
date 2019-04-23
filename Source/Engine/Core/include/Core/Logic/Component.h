//
// Created by bentoo on 10/14/16.
//

#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#include <Utils/Defines.h>
#include <Utils/Handle.h>
#include <BaseObject.h>

namespace Logic
{
	using comp_id_t = type_t;

	class YAGE_API Component : public yage::SafeObject
	{
		static std::atomic<comp_id_t> _lastTypeId;

	protected:
		explicit Component() { }
		Component(Component&& other) noexcept { }

	public:
		template <typename T>
		static comp_id_t GetComponentId()
		{
			static_assert(std::is_base_of<Component, T>::value, "T Must derive from System!");
			static comp_id_t id = _lastTypeId++;
			return id;
		}

		virtual ~Component() noexcept { }

		Component(const Component&) = delete;
		Component& operator=(Component&&) = delete;
		Component& operator=(const Component&) = delete;
	};
}

#endif //GAME_COMPONENT_H
