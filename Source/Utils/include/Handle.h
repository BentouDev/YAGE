//
// Created by mrjaqbq on 06.03.16.
//

#ifndef VOLKHVY_RESOURCEHANDLE_H
#define VOLKHVY_RESOURCEHANDLE_H

namespace Utils
{
	template<typename T>
	class Container;

	template<typename T>
	class Index;

	typedef uint32_t handle_t;

	union RawHandle
	{
	public:
		handle_t Key;

		struct {
			uint16_t LiveId;
			uint16_t Index;
		};

		RawHandle() : Key(0) {}
	};

	template<typename T>
	class Handle
	{
	public:
		RawHandle rawHandle;
	};
}

#endif //VOLKHVY_RESOURCEHANDLE_H
