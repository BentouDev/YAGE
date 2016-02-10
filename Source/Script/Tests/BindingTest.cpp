//
// Created by MrJaqbq on 2016-02-10.
//

#include "../Source/Binding.h"
#include <gtest/gtest.h>

namespace ScriptTests
{
    class BindingTests : public ::testing::Test
    {
    public:
        Script::Binding binding;

        BindingTests()
        {

        }

        ~BindingTests()
        {

        }

        void SetUp()
        {
            // code here will execute just before the test ensues
        }

        void TearDown()
        {
            // code here will be called just after the test completes
            // ok to through exceptions from here if need be
        }
    };
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}