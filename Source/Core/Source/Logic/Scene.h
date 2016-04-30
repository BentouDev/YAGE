//
// Created by mrjaqbq on 17.04.16.
//

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

namespace Core
{
	class Engine;
	struct GameTime;
}

namespace Logic
{
	class Scene
	{
		// Reference to script module
		// Containers for entites
		// Containers for entity components

		Core::Engine& engine;

	public:
		Scene(Core::Engine& engine);

		virtual ~Scene();

		virtual void Update(const Core::GameTime& time);

		virtual void Draw(const Core::GameTime& time);

		virtual void Start();

		virtual void End();
	};
}

#endif //GAME_SCENE_H
