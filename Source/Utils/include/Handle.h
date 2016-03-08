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
			uint8_t LiveId;
			uint8_t Type;
			uint16_t Index;
		};

		RawHandle() : Key(0) {}
	};

	/*template<typename T>
	class Handle
	{
		RawHandle rawHandle;
	public:

		explicit Handle() : rawHandle()
		{

		}

		auto get() const noexcept -> RawHandle
		{
			return rawHandle;
		}

		explicit operator bool() const noexcept
		{
			return rawHandle.Key != 0;
		}
	};*/

	template<typename Traits>
	class Handle
	{
		using ptr_t = typename Traits::ptr_t;

		ptr_t value;

		auto close() noexcept -> void
		{
			if(*this)
			{
				Traits::close(value);
			}
		}

		Handle(Handle const &) = delete;
		auto operator=(Handle const&) -> Handle& = delete;

	public:

		explicit Handle(ptr_t raw_value = Traits::invalid()) noexcept :
			value { raw_value } { }

		Handle(Handle && other) noexcept : value { other.release() } { }

		auto operator=(Handle && other) noexcept -> Handle&
		{
			if(this != &other)
			{
				reset(other.release());
			}

			return *this;
		}

		~Handle() noexcept
		{
			close();
		}

		explicit operator bool() const noexcept
		{
			return value != Traits::invalid();
		}

		auto get() const noexcept -> ptr_t
		{
			return value;
		}

		auto release() noexcept -> ptr_t
		{
			auto raw_value = value;
			value = Traits::invalid();
			return raw_value;
		}

		auto reset(ptr_t raw_value = Traits::invalid()) noexcept -> bool
		{
			if(value != raw_value)
			{
				close();
				value = raw_value;
			}

			return static_cast<bool>(*this);
		}
	};
}

#endif //VOLKHVY_RESOURCEHANDLE_H
