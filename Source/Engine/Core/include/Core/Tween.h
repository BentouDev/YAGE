//
// Created by Bentoo on 2017-03-18.
//

#ifndef YAGE_TWEEN_H
#define YAGE_TWEEN_H

#include <Utils/MemoryBlock.h>
#include <Utils/List.h>
#include <functional>

namespace Core
{
	struct GameTime;
	class Tweener;

	class Tween
	{
		friend class Tweener;

		explicit Tween();

	public:
		virtual ~Tween();

		std::function<void(const Tween&)> OnStart;
		std::function<void(const Tween&)> OnUpdate;
		std::function<void(const Tween&)> OnStep;
		std::function<void(const Tween&)> OnEnd;
	};

	class Tweener
	{
		Memory::IMemoryBlock&	_memory;
		Utils::List<Tween>		_tweens;

	public:
		explicit Tweener(Memory::IMemoryBlock& memory)
			: _memory(memory), _tweens(_memory)
		{ }

		Tween& createNew()
		{ return _tweens.emplace(); }

		void update(const GameTime& time);
	};
}

#endif //YAGE_TWEEN_H
