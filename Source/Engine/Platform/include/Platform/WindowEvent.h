//
// Created by Bentoo on 2017-02-15.
//

#ifndef YAGE_WINDOWEVENT_H
#define YAGE_WINDOWEVENT_H

#include <cstdint>

namespace Core
{
	namespace WindowEventType
	{
		enum Enum : std::int32_t
		{
			FOCUS,
			LOST_FOCUS,
			MAXIMIZE,
			MINIMIZE,
			ICONIFY,
			MOVE,
			RESIZE,
			DROP,
			CLOSE
		};
	}

	struct StatusData
	{
		WindowEventType::Enum	type;
		std::int32_t 			status;
	};

	struct CoordData
	{
		WindowEventType::Enum	type;
		std::int32_t			x;
		std::int32_t			y;
	};

	struct WindowEvent
	{
		union
		{
			WindowEventType::Enum	type;
			StatusData				status;
			CoordData				coord;
		};
	};
}

#endif //YAGE_WINDOWEVENT_H

