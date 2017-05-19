//
// Created by bentoo on 05.11.16.
//

#ifndef GAME_SIMPLEMEMORYTRACKER_H
#define GAME_SIMPLEMEMORYTRACKER_H

#include <vector>
#include <cstdio>
#include "MemoryTracker.h"

namespace Memory
{
	class SimpleMemoryTracker : public IMemoryTracker
	{
		struct PointerInfo
		{
			PointerInfo(std::size_t size, const char* file, unsigned long line, void* ptr)
				: size(size), file(file), line(line), ptr(ptr) { }

			std::size_t size;
			const char* file;
			unsigned long line;
			void* ptr;
		};

		std::vector<PointerInfo> _currentlyAllocatedAddresses;
		std::vector<PointerInfo> _allAllocatedAddresses;
		std::vector<PointerInfo> _allFreedAdresses;

	public:
		explicit SimpleMemoryTracker(Memory::IMemoryBlock& memory)
			: IMemoryTracker(memory)
		{
			_currentlyAllocatedAddresses.reserve(1000);
			_allAllocatedAddresses.reserve(1000);
			_allFreedAdresses.reserve(1000);
		}

		~SimpleMemoryTracker()
		{
			for (auto& info : _currentlyAllocatedAddresses)
			{
				std::fprintf(stderr, "%s : Address '%p' possibly leaked '%zu' bytes, allocated in file '%s' at line '%lu'\n",
							 getName(), info.ptr, info.size, info.file, info.line);
			}
		}

		inline void PrintCurrentAllocations()
		{
			for (auto& info : _currentlyAllocatedAddresses)
			{
				std::fprintf(stderr, "%s : Address '%p' allocated '%zu' bytes in file '%s' at line '%lu'\n",
							 getName(), info.ptr, info.size, info.file, info.line);
			}
		}

		inline void OnAllocation(void* ptr, std::size_t size, std::size_t, std::size_t, const Utils::DebugSourceInfo& info) override
		{
			_allAllocatedAddresses.push_back(PointerInfo(size, info.file, info.line, ptr));
			_currentlyAllocatedAddresses.push_back(PointerInfo(size, info.file, info.line, ptr));
		};

		inline void OnDeallocation(void* ptr, std::size_t) override
		{
			bool freed  = false;
			PointerInfo* info = nullptr;
			for (std::size_t i = 0; i < _currentlyAllocatedAddresses.size(); i++)
			{
				info = &_currentlyAllocatedAddresses[i];
				if(info->ptr == ptr)
				{
					freed = true;
					_currentlyAllocatedAddresses.erase(_currentlyAllocatedAddresses.begin() + i);
					_allFreedAdresses.push_back(*info);
				}
			}

			if (!freed)
			{
				for(std::size_t i = 0; i < _currentlyAllocatedAddresses.size(); i++)
				{
					info = &_currentlyAllocatedAddresses[i];
					if(info->ptr == ptr)
					{
						std::fprintf(stderr, "%s : Address '%p' with '%zu' bytes already freed, allocated in file '%s' at line '%lu'\n",
									 getName(), ptr, info->size, info->file, info->line);
					}
				}
			}
		};
	};
}

#endif //GAME_SIMPLEMEMORYTRACKER_H
