#include "../Source/Mesh.h"
#include <gtest/gtest.h>

namespace CoreTest
{
    class MeshTest : public ::testing::Test
    {
    public:
        Core::Mesh mesh;

        MeshTest()
        {

        }

        ~MeshTest()
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