//
// Created by MrJaqbq on 2016-02-10.
//

#include "../Source/BaseParser.h"
#include <gtest/gtest.h>

namespace ParserTests
{
    class ParserTests : public ::testing::Test
    {
    public:
        Parser::BaseParser parser;

        ParserTests()
        {

        }

        ~ParserTests()
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

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    return 0;
}