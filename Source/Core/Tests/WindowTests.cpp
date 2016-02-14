//
// Created by MrJaqbq on 2016-02-14.
//

#include <gtest/gtest.h>
#include "../Source/Window.h"

namespace CoreTest
{
    class WindowTest : public ::testing::Test
    {
    public:
        Core::Window* window;

        WindowTest()
        {
            window = new Core::Window();
        }

        ~WindowTest()
        {
            delete window;
        }

        void SetUp()
        {
            // code here will execute just before the test ensues
            glfwInit();
        }

        void TearDown()
        {
            // code here will be called just after the test completes
            // ok to through exceptions from here if need be
        }
    };

    TEST_F(WindowTest, CanCreateWindowTest)
    {
        window->Create();

        EXPECT_EQ(true, window->IsAlive());
    }
}