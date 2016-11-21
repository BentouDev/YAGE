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
	class Camera;
	class RenderTarget;

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
		Renderer& 					_renderer;
		Camera*						_camera;
		RenderTarget*				_renderTarget;

		Memory::IMemoryBlock&		_memory;
		Memory::LinearAllocator*	_frameMemory;
		Utils::List<CommandKey>		_keys;
		Utils::List<CommandData>	_data;
	//	ICommandSorter<CommandKey>&	_sorter;

	public:
		inline explicit CommandQueue(Memory::IMemoryBlock& memory, Renderer& renderer)//, ICommandSorter<CommandKey>& sorter)
			: _renderer(renderer), _camera(nullptr), _renderTarget(nullptr),
			  _memory(memory), _frameMemory(nullptr), _keys(_memory), _data(memory) //, _sorter(sorter)
		{

		}

		inline virtual ~CommandQueue() noexcept
		{
			if(_frameMemory)
				_frameMemory->clear();
		}

		inline void setCamera(Camera* cam)
		{ _camera = cam; }

		inline void setRenderTarget(RenderTarget* target)
		{ _renderTarget = target; }

		CommandData& createCommands(CommandKey key)
		{
			CommandData& data = _data.emplace();
			_keys.emplace(key);
			return data;
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
