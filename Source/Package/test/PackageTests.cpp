//
// Created by bentoo on 12.01.17.
//

#include <gtest/gtest.h>

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	printf("GTest finished with: %d", result);
	return 0;
}