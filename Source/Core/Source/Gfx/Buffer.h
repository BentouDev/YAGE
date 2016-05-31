//
// Created by mrjaqbq on 13.05.16.
//

#ifndef GAME_BUFFER_H
#define GAME_BUFFER_H

#include <cstdint>
#include <TypeInfo.h>
#include <map>
#include <vector>
#include <BorrowedPtr.h>

namespace Gfx
{
	class BufferScheme;

	class BufferData;

	// this may work other way later one
	// it will mainly depend on how i will want to use this data
	// i mean, how would i want to upload it to gpu, since any
	// processing on cpu would have to be done on copy anyway
	// and the actual data would be replaced by that copy later on
	class BufferData
	{
		using byte = uint8_t;

		byte* data;
		uint32_t size;

	public:
		BufferData(byte* new_data, uint32_t new_size)
				: data(new_data), size(new_size)
		{ }

		auto count() const noexcept -> uint32_t
		{
			return size;
		}

		auto getDataPtr() const noexcept -> byte*
		{
			return data;
		}
	};

	struct BufferComponentInfo
	{
		BufferComponentInfo(uint32_t new_count, type_t new_type)
				: count(new_count), type(new_type) { }

		uint32_t count;
		type_t type;
		// todo: may want some sort of ID here
		// to easy distinguish between those components
	};

	struct BufferScheme
	{
		std::vector<BufferComponentInfo> VertexComponents;
		std::map<std::string, uint32_t > Names;

		template <typename T>
		auto createComponent(uint32_t count, std::string name) -> void
		{
			VertexComponents.emplace_back(TypeInfo<T>::id(), count);
			Names[name] = VertexComponents.size() - 1;
		}

		auto getComponent(std::string name) noexcept -> Utils::borrowed_ptr<BufferComponentInfo>
		{
			Utils::borrowed_ptr<BufferComponentInfo> result;

			auto index = Names.find(name);
			if(index != Names.end())
			{
				result.reset(&VertexComponents[index->second]);
			}

			return result;
		}
	};

	class Buffer
	{
		std::vector<BufferData> Data;
		BufferScheme scheme;
	};
}



#endif //GAME_BUFFER_H
