//
// Created by mrjaqbq on 06.03.16.
//

#ifndef VOLKHVY_RESOURCEHANDLE_H
#define VOLKHVY_RESOURCEHANDLE_H

#define DECL_HANDLE(resource) using Handle = ResourceHandle<resource>

namespace Core
{
	typedef uint32_t handle_t;

	union RawHandle
	{
		handle_t Key;

		struct {
			uint16_t LiveId;
			uint16_t Index;
		};
	};

	class BaseHandle
	{
	protected:
		RawHandle handle;
	};

	template<class Resource>
	class ResourceHandle : BaseHandle
	{

	};
}

#endif //VOLKHVY_RESOURCEHANDLE_H
