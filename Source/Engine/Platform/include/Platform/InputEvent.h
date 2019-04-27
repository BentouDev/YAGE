//
// Created by Bentoo on 2017-02-14.
//

#ifndef YAGE_INPUTEVENT_H
#define YAGE_INPUTEVENT_H

#include <cstdint>
#include <Utils/Assert.h>
#include "DeviceType.h"

namespace Input
{
	namespace EventType
	{
		enum Enum : std::int32_t
		{
			BUTTON,
			AXIS,
			CONNECTED,
			DISCONNECTED
		};
	}

	namespace ButtonStatus
	{
		enum Enum : std::int16_t
		{
			PRESSED,
			RELEASED,
			REPEAT
		};
	}

	struct ButtonEvent
	{
		EventType::Enum 	type;
		std::int32_t 		deviceId;
		DeviceType::Enum	deviceType;

		ButtonStatus::Enum	status;
		std::int32_t		keycode;
		std::int32_t 		scancode;
	};

	struct AxisEvent
	{
		EventType::Enum 	type;
		std::int32_t 		deviceId;
		DeviceType::Enum	deviceType;

		std::int16_t		axisId;
		float	            x;
		float               y;
	};

	struct ConnectionEvent
	{
		EventType::Enum 	type;
		std::int32_t 		deviceId;
		DeviceType::Enum	deviceType;
	};

	struct InputEvent
	{
		union
		{
			struct 
			{
				EventType::Enum 	type;
				std::int32_t 		deviceId;
				DeviceType::Enum	deviceType;
			};

			ButtonEvent 		button;
			AxisEvent			axis;
			ConnectionEvent		connection;
		};

		inline bool isKeyboardAndMouseEvent() const noexcept
		{
			return deviceType == DeviceType::MOUSE ||
				   deviceType == DeviceType::KEYBOARD;
		}

		inline bool isControllerEvent() const noexcept
		{
			return deviceType == DeviceType::CONTROLLER;
		}

		static_assert(sizeof(AxisEvent) == sizeof(ButtonEvent),
					  "Both Event structs must be of the same size!");
	};

	static_assert(offsetof(InputEvent, type) == offsetof(ButtonEvent, type),
				  "Union member offsets must be the same!");

	static_assert(offsetof(InputEvent, deviceId) == offsetof(ButtonEvent, deviceId),
				  "Union member offsets must be the same!");

	static_assert(offsetof(InputEvent, deviceType) == offsetof(ButtonEvent, deviceType),
				  "Union member offsets must be the same!");

	static_assert(offsetof(InputEvent, type) == offsetof(AxisEvent, type),
				  "Union member offsets must be the same!");

	static_assert(offsetof(InputEvent, deviceId) == offsetof(AxisEvent, deviceId),
				  "Union member offsets must be the same!");

	static_assert(offsetof(InputEvent, deviceType) == offsetof(AxisEvent, deviceType),
				  "Union member offsets must be the same!");

	static_assert(offsetof(InputEvent, type) == offsetof(ConnectionEvent, type),
				  "Union member offsets must be the same!");

	static_assert(offsetof(InputEvent, deviceId) == offsetof(ConnectionEvent, deviceId),
				  "Union member offsets must be the same!");

	static_assert(offsetof(InputEvent, deviceType) == offsetof(ConnectionEvent, deviceType),
				  "Union member offsets must be the same!");
}

#endif //YAGE_INPUTEVENT_H
