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

namespace
{
	class BufferScheme;

	class BufferData;

	class BufferData
	{
		using byte = uint8_t;

		byte* data;
		uint32_t size;
		uint32_t bindId;

	public:
		auto count() const noexcept -> uint32_t
		{
			return size;
		}

		auto getDataPtr() const noexcept -> byte*
		{
			return data;
		}
	};

	class BufferComponentInfo
	{
		type_t type;
		uint32_t count;
	};

	class BufferScheme
	{
		std::map<std::string, uint32_t > NameMap;
		std::vector<BufferComponentInfo> Components;

		auto getComponentInfo(std::string name) noexcept -> Utils::borrowed_ptr<BufferComponentInfo>
		{
			Utils::borrowed_ptr<BufferComponentInfo> result;

			auto index = NameMap.find(name);
			if(index != NameMap.end())
			{
				result.reset(&Components[index->second]);
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
