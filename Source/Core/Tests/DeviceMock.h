//
// Created by mrjaqbq on 03.04.16.
//

#ifndef GAME_DUMMYAPI_H
#define GAME_DUMMYAPI_H

#include <gmock/gmock.h>
#include "../Source/Gfx/Api/BaseApi.h"

class DeviceMock : public Gfx::BaseApi
{
public:
	DeviceMock() : BaseApi("mock") { };

	MOCK_METHOD0(initialize, bool());
	MOCK_METHOD1(registerWindow, bool(const Core::Window& window));
	MOCK_METHOD1(destroyWindow, bool(const Core::Window& window));
	MOCK_METHOD1(resizeWindow, void(const Core::Window& window));
	MOCK_METHOD0(destroyWindows, void());
	MOCK_METHOD0(cleanUp, void());
	MOCK_METHOD1(beginDraw, void(const Core::Window& window));
	MOCK_METHOD1(endDraw, void(const Core::Window& window));

	void InitWillFail()
	{
		ON_CALL(*this, initialize()).WillByDefault(testing::Return(false));
	}

	void InitWillSucced()
	{
		ON_CALL(*this, initialize()).WillByDefault(testing::Return(true));
	}

};


#endif //GAME_DUMMYAPI_H
