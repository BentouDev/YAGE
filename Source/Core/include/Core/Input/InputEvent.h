//
// Created by Bentoo on 2017-02-14.
//

#ifndef YAGE_INPUTEVENT_H
#define YAGE_INPUTEVENT_H

#include <cstdint>

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

	namespace DeviceType
	{
		enum Enum : std::int16_t
		{
			MOUSE,
			KEYBOARD,
			CONTROLLER
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
		std::int32_t		x;
		std::int32_t 		y;
	};

	struct ConnectionEvent
	{
		EventType::Enum 	type;
		std::int32_t 		deviceId;
		DeviceType::Enum	deviceType;
	};

	union InputEvent
	{
		static_assert(sizeof(AxisEvent) == sizeof(ButtonEvent),
					  "Both Event structs must be of the same size!");

		EventType::Enum type;
		ButtonEvent 	button;
		AxisEvent		axis;
		ConnectionEvent	connection;

		inline bool isKeyboardAndMouseEvent() const noexcept
		{
			switch(type)
			{
				case EventType::BUTTON:
					return  button.deviceType == DeviceType::MOUSE ||
							button.deviceType == DeviceType::KEYBOARD;
				case EventType::AXIS:
					return  axis.deviceType == DeviceType::MOUSE ||
							axis.deviceType == DeviceType::KEYBOARD;
				case EventType::CONNECTED:
				case EventType::DISCONNECTED:
					return  connection.deviceType == DeviceType::MOUSE ||
							connection.deviceType == DeviceType::KEYBOARD;
			}

			return false;
		}

		inline bool isControllerEvent() const noexcept
		{
			return !isKeyboardAndMouseEvent();
		}

		inline std::int32_t getDeviceId() const noexcept
		{
			switch(type)
			{
				case EventType::BUTTON:
					return  button.deviceId;
				case EventType::AXIS:
					return  axis.deviceId;
				case EventType::CONNECTED:
				case EventType::DISCONNECTED:
					return  connection.deviceId;
			}

			return -1;
		}
	};
}

#endif //YAGE_INPUTEVENT_H
