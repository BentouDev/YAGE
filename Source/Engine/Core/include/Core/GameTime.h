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
		GameTime()
			: DeltaTime(0), Time(0), FixedDeltaTime(1/60.0f),
			  RealTime(0), Speed(1), Fps(0), FrameTime(0)
		{ }

		float DeltaTime;
		float Time;
		float FixedDeltaTime;
		float RealTime;
		float Speed;
		float Fps;
		float FrameTime;
	};
}

#endif //GAME_GAMETIME_H
