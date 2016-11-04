//
// Created by bentoo on 10/29/16.
//

#ifndef GAME_COMMANDQUEUE_H
#define GAME_COMMANDQUEUE_H

#include <Utils/List.h>
#include <Utils/LinearAllocator.h>

namespace Gfx
{
	class Renderer;

	template <typename CommandKey, typename CommandData, typename Component>
	class CommandQueue
	{
		friend class Renderer;

		static_assert(std::is_pod<CommandData>::value, "CommandData type must be POD!");
		static_assert(std::is_pod<CommandKey>::value, "CommandKey type must be POD!");

	public:
		using CommandFunction = void(*)(CommandData&);
		using CommandCreator = void(*)(Component&);

	protected:
		Memory::IMemoryBlock&		_memory;
		Memory::LinearAllocator*	_frameMemory;
		Utils::List<CommandData>	_data;
		Utils::List<CommandKey>		_keys;
	//	ICommandSorter<CommandKey>&	_sorter;

		// TODO maybe we can move this to be template paramter?
		// Does this make sense?
		CommandFunction 			_command;
		CommandCreator				_creator;

		Renderer& 					_renderer;

	public:
		explicit CommandQueue(Memory::IMemoryBlock& memory, Renderer& renderer)//, ICommandSorter<CommandKey>& sorter)
			: _memory(memory), _renderer(renderer), _frameMemory(nullptr), _keys(_memory), _data(memory) //, _sorter(sorter)
		{

		}

		virtual ~CommandQueue() noexcept
		{
			if(_frameMemory)
				_frameMemory->clear();
		}

		void createCommands(Component& component)
		{
	//		_renderer.createDrawCallData(component);
	//		_creator(component);
		}

		void reserve(std::size_t commandCount)
		{
			_data.reserve(commandCount);
			_keys.reserve(commandCount);
		}

		void clear()
		{
			_keys.resize(0);
			_data.resize(0);

			if(_frameMemory != nullptr)
				_frameMemory->clear();
		}

		void sort()
		{
		//	_sorter.sort(_keys);
		}

		void issueCommands()
		{
			// SETUP RENDER TARGET
			// SETUP RENDER STATE
			// SETUP STATE UNIFORMS (BUFFER) ?

			// Trash...
			// What with MultiDrawIndirect?
			// That would produce renderdata straight for GPU
			// Only few state changes

	//		for(CommandKey& key : _keys)
	//		{
	//			CommandData& cmdData = _data[key.dataIndex];
	//			_command(cmdData);
	//		}

		//	for(CommandData& data : _data)
		//	{
		//		_renderer.drawCall(data);
		//	}
		}
	};
}

#endif //GAME_COMMANDQUEUE_H
