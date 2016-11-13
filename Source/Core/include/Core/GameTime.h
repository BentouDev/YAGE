//
// Created by bentoo on 10/27/16.
//

#ifndef GAME_GAMETIME_H
#define GAME_GAMETIME_H

namespace Core
{
	struct GameTime
	{
	public:
		float DeltaTime;
		float Time;
		float Fps;
		float FrameTime;
		float RealTime;
		float Speed;
		float TimeStep;
		float MaxFrameTime;
	};
}

#endif //GAME_GAMETIME_H
